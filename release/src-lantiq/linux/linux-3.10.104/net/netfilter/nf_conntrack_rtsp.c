/*
 * RTSP extension for IP connection tracking
 * (C) 2003 by Tom Marshall <tmarshall@real.com>
 * based on ip_conntrack_irc.c
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 * Module load syntax:
 *   insmod nf_conntrack_rtsp.o ports=port1,port2,...port<MAX_PORTS>
 *                              max_outstanding=n setup_timeout=secs
 *
 * If no ports are specified, the default will be port 554.
 *
 * With max_outstanding you can define the maximum number of not yet
 * answered SETUP requests per RTSP session (default 8).
 * With setup_timeout you can specify how long the system waits for
 * an expected data channel (default 300 seconds).
 */


#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include <net/checksum.h>
#include <net/tcp.h>


#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <linux/netfilter/nf_conntrack_rtsp.h>
#include <net/netfilter/nf_conntrack_expect.h>

#include <linux/ctype.h>
#define NF_NEED_STRNCASECMP
#define NF_NEED_STRTOU16
#define NF_NEED_STRTOU32
#define NF_NEED_NEXTLINE
#include <linux/netfilter_helpers.h>
#define NF_NEED_MIME_NEXTLINE
#include <linux/netfilter_mime.h>

#define MAX_SIMUL_SETUP 8 /* XXX: use max_outstanding */
#define INFOP(args...) printk(KERN_INFO __FILE__ ":" __FUNCTION__ ":" args)

#ifdef IP_DYNAMIC_RTSP_DEBUG
#define NCDEBUG(format,args...) printk(" \n Neeraj_CONN " ": " __FILE__  " : " __FUNCTION__ " : "format,##args)
#else
#define NCDEBUG(format,args...)
#endif

#ifdef IP_NF_RTSP_DEBUG
#define DEBUGP printk
#else
#define DEBUGP(args...)
#endif

#define MAX_PORTS 8

static int ports[MAX_PORTS];
static int num_ports = 0;

static int max_outstanding = 8;
static unsigned int setup_timeout =100; //300;


MODULE_AUTHOR("Tom Marshall <tmarshall@real.com>");
MODULE_DESCRIPTION("RTSP connection tracking module");
MODULE_LICENSE("GPL");

#ifdef MODULE_PARM

MODULE_PARM(ports, "1-" __MODULE_STRING(MAX_PORTS) "i");
MODULE_PARM_DESC(ports, "port numbers of RTSP servers");
MODULE_PARM(max_outstanding, "i");
MODULE_PARM_DESC(max_outstanding, "max number of outstanding SETUP requests per RTSP session");
MODULE_PARM(setup_timeout, "i");
MODULE_PARM_DESC(setup_timeout, "timeout on for unestablished data channels");

#endif

DEFINE_SPINLOCK(ip_rtsp_lock);

struct module* nf_conntrack_rtsp = THIS_MODULE;
extern spinlock_t ip_rtsp_lock;


/*
 * Max mappings we will allow for one RTSP connection (for RTP, the number
 * of allocated ports is twice this value).  Note that SMIL burns a lot of
 * ports so keep this reasonably high.  If this is too low, you will see a
 * lot of "no free client map entries" messages.
 */
#define MAX_PORT_MAPS 16

static char rtsp_buffer[65536];

unsigned int proto_nat_off ;
unsigned int (*nf_nat_rtsp_hook)(struct sk_buff **pskb,
                                 enum ip_conntrack_info ctinfo,
				 unsigned int matchoff, unsigned int matchlen,
                                 struct nf_ct_rtsp_expect *expinfo,
                                 struct nf_conntrack_expect *exp, unsigned int protoff);

void (*nf_nat_rtsp_hook_expectfn)(struct nf_conn *ct, struct nf_conntrack_expect *exp);

extern void nf_ct_expect_insert_rtcp (struct nf_conntrack_expect *exp) ;

EXPORT_SYMBOL_GPL(nf_nat_rtsp_hook);


/*** default port list was here in the masq code: 554, 3030, 4040 ***/

#define SKIP_WSPACE(ptr,len,off) while(off < len && isspace(*(ptr+off))) { off++; }

/*
 * Parse an RTSP packet.
 *
 * Returns zero if parsing failed.
 *
 * Parameters:
 *  IN      ptcp        tcp data pointer
 *  IN      tcplen      tcp data len
 *  IN/OUT  ptcpoff     points to current tcp offset
 *  OUT     phdrsoff    set to offset of rtsp headers
 *  OUT     phdrslen    set to length of rtsp headers
 *  OUT     pcseqoff    set to offset of CSeq header
 *  OUT     pcseqlen    set to length of CSeq header
 */
static int
rtsp_parse_message(char* ptcp, uint tcplen, uint* ptcpoff,
		uint* phdrsoff, uint* phdrslen,
		uint* pcseqoff, uint* pcseqlen)
{
	uint    entitylen = 0;
	uint    lineoff;
	uint    linelen;

	if (!nf_nextline(ptcp, tcplen, ptcpoff, &lineoff, &linelen))
	{
		return 0;
	}

	*phdrsoff = *ptcpoff;
	while (nf_mime_nextline(ptcp, tcplen, ptcpoff, &lineoff, &linelen))
	{
		if (linelen == 0)
		{
			if (entitylen > 0)
			{
				*ptcpoff += min(entitylen, tcplen - *ptcpoff);
			}
			break;
		}
		if (lineoff+linelen > tcplen)
		{
			DEBUGP("!! overrun !!\n");
			break;
		}

		if (nf_strncasecmp(ptcp+lineoff, "CSeq:", 5) == 0)
		{
			*pcseqoff = lineoff;
			*pcseqlen = linelen;
		}
		if (nf_strncasecmp(ptcp+lineoff, "Content-Length:", 15) == 0)
		{
			uint off = lineoff+15;
			SKIP_WSPACE(ptcp+lineoff, linelen, off);
			nf_strtou32(ptcp+off, &entitylen);
		}
	}
	*phdrslen = (*ptcpoff) - (*phdrsoff);

	return 1;
}

/*
 * Find lo/hi client ports (if any) in transport header
 * In:
 *   ptcp, tcplen = packet
 *   tranoff, tranlen = buffer to search
 *
 * Out:
 *   pport_lo, pport_hi = lo/hi ports (host endian)
 *
 * Returns nonzero if any client ports found
 *
 * Note: it is valid (and expected) for the client to request multiple
 * transports, so we need to parse the entire line.
 */
static int
rtsp_parse_transport(char* ptran, uint tranlen,
		struct nf_ct_rtsp_expect* prtspexp)
{
	int     rc = 0;
	uint    off = 0;
	char    szextaddr1[15+1];

	if (tranlen < 10 || !iseol(ptran[tranlen-1]) ||
			nf_strncasecmp(ptran, "Transport:", 10) != 0)
	{
		printk("sanity check failed\n");
		return 0;
	}
	DEBUGP("tran='%.*s'\n", (int)tranlen, ptran);
	off += 10;
	SKIP_WSPACE(ptran, tranlen, off);

	/* Transport: tran;field;field=val,tran;field;field=val,... */
	while (off < tranlen)
	{
		const char* pparamend;
		uint        nextparamoff;

		pparamend = memchr(ptran+off, ',', tranlen-off);
		pparamend = (pparamend == NULL) ? ptran+tranlen : pparamend+1;
		nextparamoff = pparamend-ptran;

		while (off < nextparamoff)
		{
			const char* pfieldend;
			uint        nextfieldoff;

			pfieldend = memchr(ptran+off, ';', nextparamoff-off);
			nextfieldoff = (pfieldend == NULL) ? nextparamoff : pfieldend-ptran+1;

			if (strncmp(ptran+off, "client_port=", 12) == 0)
			{
				u_int16_t   port;
				uint        numlen;

				off += 12;
				numlen = nf_strtou16(ptran+off, &port);
				off += numlen;
				if (prtspexp->loport != 0 && prtspexp->loport != port)
				{
					DEBUGP("multiple ports found, port %hu ignored\n", port);
				}
				else
				{
					prtspexp->loport = prtspexp->hiport = port;
					if (ptran[off] == '-')
					{
						off++;
						numlen = nf_strtou16(ptran+off, &port);
						off += numlen;
						prtspexp->pbtype = pb_range;
						prtspexp->hiport = port;
						// If we have a range, assume rtp:
						// loport must be even, hiport must be loport+1
						if ((prtspexp->loport & 0x0001) != 0 ||
								prtspexp->hiport != prtspexp->loport+1)
						{
							DEBUGP("incorrect range: %hu-%hu, correcting\n",
									prtspexp->loport, prtspexp->hiport);
							prtspexp->loport &= 0xfffe;
							prtspexp->hiport = prtspexp->loport+1;
						}
					}
					else if (ptran[off] == '/')
					{
						off++;
						numlen = nf_strtou16(ptran+off, &port);
						off += numlen;
						prtspexp->pbtype = pb_discon;
						prtspexp->hiport = port;
					}
					rc = 1;
				}
			}

			/*
			 * Note we don't look for the destination parameter here.
			 * If we are using NAT, the NAT module will handle it.  If not,
			 * and the client is sending packets elsewhere, the expectation
			 * will quietly time out.
			 */

			if (strncmp(ptran+off, "source=", 7) == 0)
			{
				DEBUGP("tran=222'%.*s', %d, %d, %d, %d\n", (int)tranlen, ptran, ptran+off+7, nextparamoff, nextfieldoff, tranlen);

				memset(szextaddr1,'\0',16);
			if (nextfieldoff == nextparamoff)
				strncpy(szextaddr1, ptran+off+7, ((nextfieldoff - (off + 7)-2)));
			else
				strncpy(szextaddr1, ptran+off+7, ((nextfieldoff - (off + 7)-1)));

				prtspexp->reply_source = in_aton(szextaddr1);	
				DEBUGP("SOURCE1  %u.%u.%u.%u, %s , %u ,%u, %u \n",
			             NIPQUAD(prtspexp->reply_source), szextaddr1, nextparamoff, nextfieldoff, off);

			}

			off = nextfieldoff;
		}

		off = nextparamoff;
	}


	return rc;
}

/* Expect for RTCP port */
static void expected(struct nf_conn *ct, struct nf_conntrack_expect *exp)                                                                            
{                                                                                                                                                    
    if(nf_nat_rtsp_hook_expectfn) {                                                                                                                  
        nf_nat_rtsp_hook_expectfn(ct,exp);                                                                                                           
    }                                                                                                                                                
}                                                                                                                                                    
  

/*** conntrack functions ***/

/* inbound packet: server->client */
int
help_in(struct sk_buff *skb,unsigned char *rb_ptr,unsigned int datalen,
		struct nf_conn* ct, enum ip_conntrack_info ctinfo)
{
	int dir = CTINFO2DIR(ctinfo);   /* = IP_CT_DIR_ORIGINAL */

	struct nf_ct_rtsp_expect expinfo;
	uint    dataoff = 0;
        char *pdata = rb_ptr;
	struct nf_conntrack_expect *exp, *exp1;
	int ret = NF_ACCEPT;

        struct nf_conn *ct1;                                                                                                             
        u_int32_t   newip1;
	char    replyaddr[15+1];
	char    destaddr[15+1];

	DEBUGP("dir %u\n", dir);

	while (dataoff < datalen)
	{
		uint    cmdoff = dataoff;
		uint    hdrsoff = 0;
		uint    hdrslen = 0;
		uint    cseqoff = 0;
		uint    cseqlen = 0;
		uint    lineoff = 0;
		uint    linelen = 0;
		uint    off;

		if (!rtsp_parse_message(pdata, datalen, &dataoff,
					&hdrsoff, &hdrslen,
					&cseqoff, &cseqlen))
		{
			DEBUGP("[%s]:[%d]\n", __FUNCTION__, __LINE__);
			break;      /* not a valid message */
		}
		
		if (strncmp(pdata+cmdoff, "RTSP/", 5) != 0)
		{
			DEBUGP("[%s]:[%d]\n", __FUNCTION__, __LINE__);
			continue;   /* not a SETUP message */
		}

		DEBUGP("found a Reply message\n");

		memset(&expinfo, 0, sizeof(expinfo));

		off = 0;
		while (nf_mime_nextline(pdata+hdrsoff, hdrslen, &off,
					&lineoff, &linelen))
		{
			if (linelen == 0)
			{
				break;
			}
			if (off > hdrsoff+hdrslen)
			{
				//INFOP("!! overrun !!");
				break;
			}

			if (nf_strncasecmp(pdata+hdrsoff+lineoff, "Transport:", 10) == 0)
			{
				rtsp_parse_transport(pdata+hdrsoff+lineoff, linelen,
						&expinfo);
			}
		}

		if (expinfo.loport == 0)
		{
			DEBUGP("no udp transports found\n");
			continue;   /* no udp transports found */
		}

                if (expinfo.reply_source == 0)
                {
                        DEBUGP("no reply_source found\n");
                        continue;   /* no udp transports found */
                }

		memset(replyaddr,'\0', 16);
		memset(destaddr,'\0', 16);
		sprintf(replyaddr, "%u.%u.%u.%u", NIPQUAD(expinfo.reply_source));
		sprintf(destaddr, "%u.%u.%u.%u", NIPQUAD(ct->tuplehash[dir].tuple.src.u3.ip));
			
		if (strncmp(replyaddr, destaddr, 16) == 0) {
			DEBUGP("replyaddr = %s , destaddr = %s %d , %d \n",replyaddr, destaddr , sizeof(replyaddr), sizeof(destaddr));
			continue;
		}
	
		DEBUGP("udp transport found, ports=(%d,%hu,%hu) \n",
				(int)expinfo.pbtype,
				expinfo.loport,
				expinfo.hiport);


		exp = nf_ct_expect_alloc(ct);
		        if (!exp) {
				ret = NF_DROP;
				goto out;
			}

		spin_lock_bh(&ip_rtsp_lock);
	
		nf_ct_expect_init(exp, NF_CT_EXPECT_CLASS_DEFAULT,
				ct->tuplehash[dir].tuple.src.l3num,
				&ct->tuplehash[dir].tuple.src.u3,
				&ct->tuplehash[dir].tuple.dst.u3,
				IPPROTO_UDP, NULL, &(expinfo.loport));

		expinfo.len = hdrslen;
        	exp->master = ct ;

		if (expinfo.reply_source == 0) {
			 exp->tuple.src.u3 = ct->tuplehash[dir].tuple.src.u3;
			 DEBUGP("expinfo.reply_source is NULL \n");
		}
		else
			exp->tuple.src.u3.ip = expinfo.reply_source;

	    	exp->tuple.src.u.tcp.port = 0;
		
	    	exp->tuple.src.l3num = PF_INET ;
		
		exp->tuple.dst.u3 = ct->tuplehash[dir].tuple.dst.u3;

		if (expinfo.pbtype == pb_range) {
		//	printk("Changing expectation mask to handle multiple ports\n");
		//	exp->mask.src.u.udp.port  = 0xfffe;
		}

		exp->tuple.dst.protonum = IPPROTO_UDP;
        
		exp->helper = NULL;
		exp->flags = 0;
		exp->expectfn = expected; /* NULL; */ 

	        expinfo.hiport = expinfo.loport + 1 ;
		
		DEBUGP("expect_related %u.%u.%u.%u:%u-%u.%u.%u.%u:%u    %u\n",
				NIPQUAD(exp->tuple.src.u3.ip),
				ntohs(exp->tuple.src.u.udp.port),
				NIPQUAD(exp->tuple.dst.u3.ip),
				ntohs(exp->tuple.dst.u.udp.port),expinfo.loport);
                 
				 
                if (nf_ct_expect_related(exp) != 0) {
                	printk("ip_conntrack_expect_related failed\n");
			nf_ct_expect_insert_rtcp(exp);
		}

		/* For RTCP port */
			exp1 = nf_ct_expect_alloc(ct);
			if (!exp1) { 
				ret = NF_DROP;
				goto out;
			}
			expinfo.loport += 1;
			nf_ct_expect_init(exp1, NF_CT_EXPECT_CLASS_DEFAULT,
					 ct->tuplehash[dir].tuple.src.l3num,
					 &ct->tuplehash[dir].tuple.src.u3,
					 &ct->tuplehash[dir].tuple.dst.u3,
					 IPPROTO_UDP, NULL, &(expinfo.loport));

                if (expinfo.reply_source == 0)
                        exp1->tuple.src.u3 = ct->tuplehash[dir].tuple.src.u3;
                else
                        exp1->tuple.src.u3.ip = expinfo.reply_source;


			exp1->mask.src.u3.ip  = 0xffffffff;
			exp1->tuple.src.u.tcp.port = 0;
			exp1->mask.src.u.udp.port  = 0 ;
			exp1->tuple.src.l3num = PF_INET ;
			exp1->tuple.dst.u3 = ct->tuplehash[dir].tuple.dst.u3;
			exp1->tuple.dst.u.udp.port = htons(expinfo.loport);
			exp1->tuple.dst.protonum = IPPROTO_UDP;
			expinfo.pbtype = pb_single;
			exp1->helper = NULL;
			exp1->flags = 0;
			exp1->expectfn = expected;
			ct1 = exp->master;
			newip1 = ct1->tuplehash[dir].tuple.dst.u3.ip;
			exp1->tuple.dst.u3.ip = newip1;
			exp1->saved_proto.udp.port = exp1->tuple.dst.u.udp.port;
			exp1->dir = dir;

		DEBUGP("expect_related_RTCP %u.%u.%u.%u:%u-%u.%u.%u.%u:%u    %u\n",
				NIPQUAD(exp1->tuple.src.u3.ip),
				ntohs(exp1->tuple.src.u.udp.port),
				NIPQUAD(exp1->tuple.dst.u3.ip),
				ntohs(exp1->tuple.dst.u.udp.port),expinfo.loport);
	
		if (nf_ct_expect_related(exp1) != 0) {
			nf_ct_expect_insert_rtcp(exp1);
		}

		spin_unlock_bh(&ip_rtsp_lock);
		
		nf_ct_expect_put(exp);
		goto out;
	}
 
out:
          return ret;
}

/*** conntrack functions ***/

/* outbound packet: client->server */
int help_out(struct sk_buff *skb,unsigned char *rb_ptr,unsigned int datalen,
		struct nf_conn* ct, enum ip_conntrack_info ctinfo)
{
	int dir = CTINFO2DIR(ctinfo);   /* = IP_CT_DIR_ORIGINAL */

	struct nf_ct_rtsp_expect expinfo;
	uint    dataoff = 0;
        char *pdata = rb_ptr;
	struct nf_conntrack_expect *exp, *exp1;
         int ret = NF_ACCEPT;
	typeof(nf_nat_rtsp_hook) nf_nat_rtsp;

	struct sk_buff *pskb;
         struct nf_conn *ct1;                                                                                                             
         u_int32_t   newip1;                                                                                                                   


	   DEBUGP("dir %u\n", dir);

	while (dataoff < datalen)
	{
		uint    cmdoff = dataoff;
		uint    hdrsoff = 0;
		uint    hdrslen = 0;
		uint    cseqoff = 0;
		uint    cseqlen = 0;
		uint    lineoff = 0;
		uint    linelen = 0;
		uint    off;

		if (!rtsp_parse_message(pdata, datalen, &dataoff,
					&hdrsoff, &hdrslen,
					&cseqoff, &cseqlen))
		{
			break;      /* not a valid message */
		}

		if (strncmp(pdata+cmdoff, "SETUP ", 6) != 0)
		{
			continue;   /* not a SETUP message */
		}
		DEBUGP("found a setup message\n");
		DEBUGP("whole HDR : len=%u, %.*s \n", (datalen), (int)(datalen), pdata);

		memset(&expinfo, 0, sizeof(expinfo));

		off = 0;
		while (nf_mime_nextline(pdata+hdrsoff, hdrslen, &off,
					&lineoff, &linelen))
		{
			if (linelen == 0)
			{
				break;
			}
			if (off > hdrsoff+hdrslen)
			{
				//INFOP("!! overrun !!");
				break;
			}
			
			if (nf_strncasecmp(pdata+hdrsoff+lineoff, "Transport:", 10) == 0)
			{
				rtsp_parse_transport(pdata+hdrsoff+lineoff, linelen,
						&expinfo);
			}
		}

		if (expinfo.loport == 0)
		{
			DEBUGP("no udp transports found\n");
			continue;   /* no udp transports found */
		}

		DEBUGP("udp transport found, ports=(%d,%hu,%hu) \n",
				(int)expinfo.pbtype,
				expinfo.loport,
				expinfo.hiport);


		exp = nf_ct_expect_alloc(ct);
		        if (!exp) {
                		ret = NF_DROP;
		                goto out;
			         }


		spin_lock_bh(&ip_rtsp_lock);
	
		nf_ct_expect_init(exp, NF_CT_EXPECT_CLASS_DEFAULT,
				ct->tuplehash[!dir].tuple.src.l3num,
				&ct->tuplehash[!dir].tuple.src.u3,
				&ct->tuplehash[!dir].tuple.dst.u3,
				IPPROTO_UDP, NULL, &(expinfo.loport));

		expinfo.len = hdrslen;
        	exp->master = ct ;

		if (expinfo.reply_source == 0)
			exp->tuple.src.u3 = ct->tuplehash[!dir].tuple.src.u3;
		else
			exp->tuple.src.u3.ip = expinfo.reply_source;

		exp->tuple.src.u3 = ct->tuplehash[!dir].tuple.src.u3;
	        exp->tuple.src.u.tcp.port = 0;
	    	exp->tuple.src.l3num = PF_INET ;
		exp->tuple.dst.u3 = ct->tuplehash[dir].tuple.src.u3;

		if (expinfo.pbtype == pb_range) {
		//	printk("Changing expectation mask to handle multiple ports\n");
		//	exp->mask.src.u.udp.port  = 0xfffe;
		}

		exp->tuple.dst.protonum = IPPROTO_UDP;
		exp->helper = NULL;
		exp->flags = 0;
		exp->expectfn = expected;

	        expinfo.hiport = expinfo.loport + 1 ;
		
		DEBUGP("expect_related %u.%u.%u.%u:%u-%u.%u.%u.%u:%u    %u\n",
				NIPQUAD(exp->tuple.src.u3.ip),
				ntohs(exp->tuple.src.u.udp.port),
				NIPQUAD(exp->tuple.dst.u3.ip),
				ntohs(exp->tuple.dst.u.udp.port),expinfo.loport);
                 
				 
		nf_nat_rtsp = rcu_dereference(nf_nat_rtsp_hook);

		pskb = (struct sk_buff *)skb;

		DEBUGP("whole HDR 22222 before nat : len=%u, %.*s \n", (datalen), (int)(datalen), pdata);
        
		if(nf_nat_rtsp)
                {
                     /* pass the request off to the nat helper */
                	ret = nf_nat_rtsp(&pskb, ctinfo, hdrsoff, hdrslen, &expinfo, exp, proto_nat_off);
                }
                else if (nf_ct_expect_related(exp) != 0) {
                	printk("ip_conntrack_expect_related failed\n");
		}

		/* For RTCP port */
			exp1 = nf_ct_expect_alloc(ct);
			if (!exp1) { 
				ret = NF_DROP;
				goto out;
			}
			expinfo.loport += 1;
			nf_ct_expect_init(exp1, NF_CT_EXPECT_CLASS_DEFAULT,
					 ct->tuplehash[!dir].tuple.src.l3num,
					 &ct->tuplehash[!dir].tuple.src.u3,
					 &ct->tuplehash[!dir].tuple.dst.u3,
					 IPPROTO_UDP, NULL, &(expinfo.loport));

                if (expinfo.reply_source == 0)
                        exp1->tuple.src.u3 = ct->tuplehash[!dir].tuple.src.u3;
                else
                        exp1->tuple.src.u3.ip = expinfo.reply_source;

			exp1->mask.src.u3.ip  = 0xffffffff;
			exp1->tuple.src.u.tcp.port = 0;
			exp1->mask.src.u.udp.port  = 0 ;
			exp1->tuple.src.l3num = PF_INET ;
			exp1->tuple.dst.u3 = ct->tuplehash[dir].tuple.src.u3;
			exp1->tuple.dst.u.udp.port = htons(expinfo.loport);
			exp1->tuple.dst.protonum = IPPROTO_UDP;
			expinfo.pbtype = pb_single;
			exp1->helper = NULL;
			exp1->flags = 0;
			exp1->expectfn = expected;
			ct1 = exp->master;
			newip1 = ct1->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip;
			exp1->tuple.dst.u3.ip = newip1;
			exp1->saved_proto.udp.port = exp1->tuple.dst.u.udp.port;
			exp1->dir = !dir;

		DEBUGP("expect_related_RTCP %u.%u.%u.%u:%u-%u.%u.%u.%u:%u    %u\n",
				NIPQUAD(exp->tuple.src.u3.ip),
				ntohs(exp->tuple.src.u.udp.port),
				NIPQUAD(exp->tuple.dst.u3.ip),
				ntohs(exp->tuple.dst.u.udp.port),expinfo.loport);
	
		if (nf_ct_expect_related(exp1) != 0) {
			nf_ct_expect_insert_rtcp(exp1);
		}

		spin_unlock_bh(&ip_rtsp_lock);
                
		nf_ct_expect_put(exp);
		goto out;
	}

out:
          return ret;
}


static int
help(struct sk_buff *skb,unsigned int protoff,
		struct nf_conn* ct, enum ip_conntrack_info ctinfo)
{
         struct tcphdr _tcph, *th;
         unsigned int dataoff, datalen;
         char *rb_ptr;
         int ret = NF_DROP;

	/* Until there's been traffic both ways, don't look in packets. */
	if (ctinfo != IP_CT_ESTABLISHED && ctinfo != IP_CT_ESTABLISHED + IP_CT_IS_REPLY)
	{
		DEBUGP("conntrackinfo = %u\n", ctinfo);
		return NF_ACCEPT;
	} 

	proto_nat_off = protoff;
        th = skb_header_pointer(skb, protoff,
                            sizeof(_tcph), &_tcph);
	if (!th)
             return NF_ACCEPT;

	/* No data ? */
	dataoff = protoff + th->doff*4;
	datalen = skb->len - dataoff;
	if (dataoff >= skb->len)
            return NF_ACCEPT;

    	rb_ptr = skb_header_pointer(skb, dataoff,
                                skb->len - dataoff, rtsp_buffer);
    	BUG_ON(rb_ptr == NULL);

	switch (CTINFO2DIR(ctinfo))
	{
		case IP_CT_DIR_ORIGINAL:
			DEBUGP("Calling help_out ********* \n");
                        ret = help_out(skb, rb_ptr, datalen, ct, ctinfo);
			break;
		case IP_CT_DIR_REPLY:
			DEBUGP("Calling help_in ********* \n");
			ret = help_in(skb, rb_ptr, datalen, ct, ctinfo);
			break;
		default:
			/* oops */
                       break;
	}

	return NF_ACCEPT;
}

static struct nf_conntrack_helper rtsp_helpers[MAX_PORTS];
static struct nf_conntrack_expect_policy rtsp_expect_policy;

/* This function is intentionally _NOT_ defined as __exit */
static void fini(void)
{
	int i;
	for (i = 0; i < num_ports; i++)
	{
		DEBUGP("unregistering port %d\n", ports[i]);
		nf_conntrack_helper_unregister(&rtsp_helpers[i]);
	}

}

static int __init init(void)
{

	int  ret;
	struct nf_conntrack_helper *hlpr;
	int i = 0;

	printk("nf_conntrack_rtsp v" IP_NF_RTSP_VERSION " loading\n");

	if (max_outstanding < 1)
	{
		printk("nf_conntrack_rtsp: max_outstanding must be a positive integer\n");
		return -EBUSY;
	}
	if (setup_timeout < 0)
	{
		printk("nf_conntrack_rtsp: setup_timeout must be a positive integer\n");
		return -EBUSY;
	}
	
	rtsp_expect_policy.max_expected = max_outstanding;
	rtsp_expect_policy.timeout = setup_timeout;

	/* If no port given, default to standard rtsp port */
	if (ports[0] == 0)
	{
		ports[0] = RTSP_PORT;
	}

	for (i = 0; (i < MAX_PORTS) && ports[i]; i++)
	{
		hlpr = &rtsp_helpers[i];
		memset(hlpr, 0, sizeof(struct nf_conntrack_helper));
    		hlpr->tuple.src.l3num = PF_INET;
		hlpr->tuple.src.u.tcp.port = htons(ports[i]);
		hlpr->tuple.dst.protonum = IPPROTO_TCP;
		hlpr->expect_policy = &rtsp_expect_policy;
		hlpr->me = THIS_MODULE;
		hlpr->help = help;

		if (ports[i] == RTSP_PORT)
		{
			sprintf(hlpr->name, "rtsp");
		}
		else
		{
			sprintf(hlpr->name, "rtsp-%d", i);
		}

		DEBUGP("port #%d: %d\n", i, ports[i]);

		ret = nf_conntrack_helper_register(hlpr);

		if (ret)
		{
			printk("nf_conntrack_rtsp: ERROR registering port %d\n", ports[i]);
			fini();
			return -EBUSY;
		}
		num_ports++;
	}
	return 0;

}

#ifdef CONFIG_IP_NF_NAT_NEEDED
EXPORT_SYMBOL(ip_rtsp_lock);
#endif

module_init(init);
module_exit(fini);
