/*
 * RTSP extension for TCP NAT alteration
 * (C) 2003 by Tom Marshall <tmarshall@real.com>
 * based on ip_nat_irc.c
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 * Module load syntax:
 *      insmod ip_nat_rtsp.o ports=port1,port2,...port<MAX_PORTS>
 *                           stunaddr=<address>
 *                           destaction=[auto|strip|none]
 *
 * If no ports are specified, the default will be port 554 only.
 *
 * stunaddr specifies the address used to detect that a client is using STUN.
 * If this address is seen in the destination parameter, it is assumed that
 * the client has already punched a UDP hole in the firewall, so we don't
 * mangle the client_port.  If none is specified, it is autodetected.  It
 * only needs to be set if you have multiple levels of NAT.  It should be
 * set to the external address that the STUN clients detect.  Note that in
 * this case, it will not be possible for clients to use UDP with servers
 * between the NATs.
 *
 * If no destaction is specified, auto is used.
 *   destaction=auto:  strip destination parameter if it is not stunaddr.
 *   destaction=strip: always strip destination parameter (not recommended).
 *   destaction=none:  do not touch destination parameter (not recommended).
 */

#include <linux/module.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/kernel.h>
#include <net/tcp.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_helper.h>
//#include <net/netfilter/nf_nat_rule.h>
#include <linux/netfilter/nf_conntrack_rtsp.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_expect.h>
#include <net/netfilter/nf_conntrack_ecache.h>

extern rwlock_t ip_rtsp_rwlock;

#include <linux/inet.h>
#include <linux/ctype.h>
#define NF_NEED_STRNCASECMP
#define NF_NEED_STRTOU16
#include <linux/netfilter_helpers.h>
#define NF_NEED_MIME_NEXTLINE
#include <linux/netfilter_mime.h>
#include <linux/unistd.h>


#define INFOP(args...) printk(KERN_INFO __FILE__ ":", __FUNCTION__ ":" args)

#ifdef IP_NF_RTSP_DEBUG
#define DEBUGP printk
#else
#define DEBUGP(args...)
#endif

#define MAX_PORTS       8
#define DSTACT_AUTO     0
#define DSTACT_STRIP    1
#define DSTACT_NONE     2


#ifdef IP_NF_RTSP_DEBUG
char *rb_ptr1;
static char rtsp_buffer1[65536];
char *pdata1;
#endif

unsigned int dataoff1, datalen1;
static char *    stunaddr = NULL;
static char *    destaction = NULL;
static u_int32_t extip = 0;
static int       dstact = 0;


MODULE_AUTHOR("Tom Marshall <tmarshall@real.com>");
MODULE_DESCRIPTION("RTSP network address translation module");
MODULE_LICENSE("GPL");

#ifdef MODULE_PARM
MODULE_PARM(ports, "1-" __MODULE_STRING(MAX_PORTS) "i");
MODULE_PARM_DESC(ports, "port numbers of RTSP servers");
MODULE_PARM(stunaddr, "s");
MODULE_PARM_DESC(stunaddr, "Address for detecting STUN");
MODULE_PARM(destaction, "s");
MODULE_PARM_DESC(destaction, "Action for destination parameter (auto/strip/none)");
#endif

/* protects rtsp part of conntracks */
extern spinlock_t ip_rtsp_lock ;

#define SKIP_WSPACE(ptr,len,off) while(off < len && isspace(*(ptr+off))) { off++; }

struct nf_ct_rtsp_expect *exp_driver_info;

/*** helper functions ***/

static void

get_skb_tcpdata(struct sk_buff* skb, char** pptcpdata, uint* ptcpdatalen)
{
	struct iphdr*   iph  = ip_hdr(skb);
	struct tcphdr*  tcph = (struct tcphdr*)((char*)iph + iph->ihl*4);

	*pptcpdata = (char*)tcph + tcph->doff*4;
	*ptcpdatalen = ((char*)skb_transport_header(skb) + skb->len) - *pptcpdata;
}


static void expected(struct nf_conn* ct, struct nf_conntrack_expect *exp)
{
	nf_nat_follow_master(ct,exp);
}


/*** nat functions ***/

/*
 * Mangle the "Transport:" header:
 *   - Replace all occurences of "client_port=<spec>"
 *   - Handle destination parameter
 *
 * In:
 *   ct, ctinfo = conntrack context
 *   pskb       = packet
 *   tranoff    = Transport header offset from TCP data
 *   tranlen    = Transport header length (incl. CRLF)
 *   rport_lo   = replacement low  port (host endian)
 *   rport_hi   = replacement high port (host endian)
 *
 * Returns packet size difference.
 *
 * Assumes that a complete transport header is present, ending with CR or LF
 */
static int
rtsp_mangle_tran(struct nf_conn *ct, enum ip_conntrack_info ctinfo,
		struct nf_conntrack_expect *exp, struct nf_ct_rtsp_expect *prtspexp,
		struct sk_buff **pskb, uint tranoff, uint tranlen, unsigned int protoff_nat)
{
	char*       ptcp;
	uint        tcplen;
	char*       ptran;
	char        rbuf1[16];      /* Replacement buffer (one port) */
	uint        rbuf1len;       /* Replacement len (one port) */
	char        rbufa[16];      /* Replacement buffer (all ports) */
	uint        rbufalen;       /* Replacement len (all ports) */
	u_int32_t   newip;
	u_int16_t   loport, hiport;
	uint        off = 0;
	uint        diff, diff1;           /* Number of bytes we removed */
        struct nf_conntrack_tuple t;
	int dir = CTINFO2DIR(ctinfo); 

	char    szextaddr[15+1];
	uint    extaddrlen;
	int     is_stun;
     
	int off_adj = 0 ;
	struct nf_conn_nat *nat; /*= nfct_nat(ct);*/
	struct nf_nat_seq *this_way = NULL;

         ct = exp->master;
	get_skb_tcpdata(*pskb, &ptcp, &tcplen);
	ptran = ptcp+tranoff;
	
	nat = nfct_nat(ct);

	if (tranoff+tranlen > tcplen || tcplen-tranoff < tranlen ||
			tranlen < 10 || !iseol(ptran[tranlen-1]) ||
			nf_strncasecmp(ptran, "Transport:", 10) != 0)
	{
		DEBUGP("sanity check failed\n");
		return 0;
	}
	off += 10;
	SKIP_WSPACE(ptcp+tranoff, tranlen, off);

	 DEBUGP("T1 hdr: len=%u, %.*s \n", tranlen, (int)tranlen, ptcp+tranoff);
#ifdef IP_NF_RTSP_DEBUG
	 DEBUGP("whole HDR INSIDE rtsp_mangle_tran 222222 : len=%u, %.*s \n", (datalen1), (int)(datalen1), pdata1);
#endif

	if (exp->tuple.dst.u.udp.port == 0) {
		newip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
		t = exp->tuple;
		t.src.u3.ip = newip;
	exp->tuple.src.u3.ip = newip ;
	} else {
		newip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip;
		t = exp->tuple;
		t.dst.u3.ip = newip;
		exp->tuple.dst.u3.ip = newip ;
	}

	exp->expectfn = nf_nat_follow_master;

#ifdef IP_NF_RTSP_DEBUG
	DEBUGP("whole HDR INSIDE rtsp_mangle_tran 333333 : len=%u, %.*s \n", (datalen1), (int)(datalen1), pdata1);
#endif

	DEBUGP("expect_NAT %u.%u.%u.%u:%u-%u.%u.%u.%u:%u\n",
             NIPQUAD(exp->tuple.src.u3.ip),
             ntohs(exp->tuple.src.u.udp.port),
            NIPQUAD(exp->tuple.dst.u3.ip),
            ntohs(exp->tuple.dst.u.udp.port));
														

	extaddrlen = extip ? sprintf(szextaddr, "%u.%u.%u.%u", NIPQUAD(extip))
		: sprintf(szextaddr, "%u.%u.%u.%u", NIPQUAD(newip));
	DEBUGP("stunaddr=%s (%s)\n", szextaddr, (extip?"forced":"auto"));

	rbuf1len = rbufalen = 0;
	switch (prtspexp->pbtype)
	{
		case pb_single:
			for (loport = prtspexp->loport; loport != 0; loport++) /* XXX: improper wrap? */
			{
				if (exp->tuple.dst.u.udp.port == 0) {
				   	t.src.u.udp.port = htons(loport);
				} else {
					t.dst.u.udp.port = htons(loport);
				}

				exp->tuple.dst.u.udp.port = t.dst.u.udp.port ;
				exp->saved_proto.udp.port = exp->tuple.dst.u.udp.port;
				exp->dir = !dir;

             		if ((nf_ct_expect_related(exp) == 0))
			{
					prtspexp->nat_rtp_port = loport;
					DEBUGP("using port %hu\n", loport);
					break;
			}
			else
			{
			   break;
			}
			
			}
			if (loport != 0)
			{
				rbuf1len = sprintf(rbuf1, "%hu", loport);
				rbufalen = sprintf(rbufa, "%hu", loport);
			}
			break;
		case pb_range:
			for (loport = prtspexp->loport; loport != 0; loport += 2) /* XXX: improper wrap? */
			{
				t.dst.u.udp.port = htons(loport);
				exp->tuple.dst.u.udp.port = t.dst.u.udp.port ;
				exp->saved_proto.udp.port = exp->tuple.dst.u.udp.port;
				exp->dir = !dir;
   
				if (nf_ct_expect_related(exp) == 0)
				{
					hiport = loport + ~exp->mask.src.u.udp.port;
					prtspexp->nat_rtp_port = loport;
					prtspexp->nat_rtcp_port = hiport;
					DEBUGP("using ports %hu-%hu\n", loport, hiport);
					break;
				}
				else
				{
				      break;
				}
			}
			if (loport != 0)
			{
				rbuf1len = sprintf(rbuf1, "%hu", loport);
				rbufalen = sprintf(rbufa, "%hu-%hu", loport, loport+1);
			}
			break;
		case pb_discon:
			for (loport = prtspexp->loport; loport != 0; loport++) /* XXX: improper wrap? */
			{
				t.dst.u.udp.port = htons(loport);
				exp->tuple.dst.u.udp.port = t.dst.u.udp.port ;
				exp->saved_proto.udp.port = exp->tuple.dst.u.udp.port;
				exp->dir = !dir ;
   
  				if (nf_ct_expect_related(exp) == 0)
				{
					prtspexp->nat_rtp_port = loport;
					DEBUGP("using port %hu (1 of 2)\n", loport);
					break;
				}
				else
				{
				  break;
				}
			}
			for (hiport = prtspexp->hiport; hiport != 0; hiport++) /* XXX: improper wrap? */
			{
				t.dst.u.udp.port = htons(hiport);
				exp->tuple.dst.u.udp.port = t.dst.u.udp.port ;
				exp->saved_proto.udp.port = exp->tuple.dst.u.udp.port;
				exp->dir = !dir;

				if (nf_ct_expect_related(exp) == 0)
				{
					prtspexp->nat_rtcp_port = hiport;
					DEBUGP("using port %hu (2 of 2)\n", hiport);
					break;
				}
				else
				{
				break;
				}
			}
			if (loport != 0 && hiport != 0)
			{
				rbuf1len = sprintf(rbuf1, "%hu", loport);
				if (hiport == loport+1)
				{
					rbufalen = sprintf(rbufa, "%hu-%hu", loport, hiport);
				}
				else
				{
					rbufalen = sprintf(rbufa, "%hu/%hu", loport, hiport);
				}
			}
			break;
		default:
			/* oops */
   			break;
	}

	if (rbuf1len == 0)
	{
		return 0;   /* cannot get replacement port(s) */
	}

	/* Check in case this is RTCP port i.e. odd port */
	if ((prtspexp->loport % 2) != 0) {
		DEBUGP("This is RTCP request to port %d\n",prtspexp->loport);
		return 0;
	} else 
		DEBUGP("This is NOT RTCP request to port %d\n",prtspexp->loport);
	 
	DEBUGP("T2 hdr: len=%u, %.*s \n", tranlen, (int)tranlen, ptcp+tranoff);
	
	/* Transport: tran;field;field=val,tran;field;field=val,... */
	while (off < tranlen)
	{
		uint        saveoff;
		const char* pparamend;
		uint        nextparamoff;

		pparamend = memchr(ptran+off, ',', tranlen-off);
		pparamend = (pparamend == NULL) ? ptran+tranlen : pparamend+1;
		nextparamoff = pparamend-ptcp;

		/*
		 * We pass over each param twice.  On the first pass, we look for a
		 * destination= field.  It is handled by the security policy.  If it
		 * is present, allowed, and equal to our external address, we assume
		 * that STUN is being used and we leave the client_port= field alone.
		 */
		is_stun = 0;
		saveoff = off;
		while (off < nextparamoff)
		{
			const char* pfieldend;
			uint        nextfieldoff;
			char tran_dst[32];
			uint off_mangle ;

			pfieldend = memchr(ptran+off, ';', nextparamoff-off);
			
			nextfieldoff = (pfieldend == NULL) ? nextparamoff : pfieldend-ptran+1;

                        if (dstact != DSTACT_NONE && strncmp(ptran+off, "destination=", 12) == 0)
                        {
                                if (strncmp(ptran+off+12, szextaddr, extaddrlen) == 0)
                                {
                                        is_stun = 1;
                                }
                                if (dstact == DSTACT_STRIP || (dstact == DSTACT_AUTO && !is_stun))
                                {
					if (nextfieldoff == nextparamoff)                                  
					{
						diff1 = nextfieldoff - (((ptran-ptcp)+off) + 1 );
					}
					else 
					{
						diff1 = nextfieldoff-off;
        				}

                        		/* issue : corrupts the packet*/
                                        memset (tran_dst, '\0', 32);
					snprintf(tran_dst, sizeof(tran_dst), "destination=%s", szextaddr);

                                         off_mangle = (ptran-ptcp)+off; /* (datalen1 - ( (pdata1+datalen1) - ptran)); */
					 diff = (extaddrlen + 12) - (diff1 - 1) ;
                                         off_adj += diff ;
#ifdef IP_NF_RTSP_DEBUG
					DEBUGP("whole HDR INSIDE rtsp_mangle_tran 444444.3333 : len=%u, %.*s , off = %ld , diff= %ld, nextfieldoff = %ld, pfieldend=%ld, nextparamoff= %ld, tranlen =%ld , %ld , %ld,  %ld, %ld, %ld, %ld , %ld, %ld \n", (datalen1), (int)(datalen1), pdata1, off, diff, nextfieldoff , pfieldend, nextparamoff, tranlen, ((pdata1+datalen1) - ptran), datalen1 - ( (pdata1+datalen1) - ptran), tranoff, ((ptran-ptcp)+off), diff1, off_adj, (extaddrlen + 12), off_mangle );
#endif					
					if (!nf_nat_mangle_tcp_packet(*pskb, ct, ctinfo, protoff_nat,
                                       		(off_mangle), (diff1 - 1), tran_dst, (extaddrlen + 12) ))
                                        {
                                                /* mangle failed, all we can do is bail */
						nf_ct_unexpect_related(exp);
                                                return 0;
                                        }

#ifdef IP_NF_RTSP_DEBUG
					DEBUGP("whole HDR INSIDE rtsp_mangle_tran 444444.444444 : len=%u, %.*s %d, %ld, %ld, %ld \n", (datalen1), (int)(datalen1), pdata1,off_adj, diff, diff1);
#endif
                                        
					get_skb_tcpdata(*pskb, &ptcp, &tcplen);
                                        ptran = ptcp+tranoff;
                                        tranlen += diff;
                                        nextparamoff += diff;
                                        nextfieldoff += diff;
                                }
                        }
			off = nextfieldoff;
		}
		if (is_stun)
		{
			continue;
		}
		off = saveoff;
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
				uint        origoff;
				uint        origlen;
				char*       rbuf    = rbuf1;
				uint        rbuflen = rbuf1len;

				off += 12;
				origoff = (ptran-ptcp)+off;
				origlen = 0;
				numlen = nf_strtou16(ptran+off, &port);
				off += numlen;
				origlen += numlen;
				if (port != prtspexp->loport)
				{
					DEBUGP("multiple ports found, port %hu ignored\n", port);
				}
				else
				{
					if (ptran[off] == '-' || ptran[off] == '/')
					{
						off++;
						origlen++;
						numlen = nf_strtou16(ptran+off, &port);
						off += numlen;
						origlen += numlen;
						rbuf = rbufa;
						rbuflen = rbufalen;
						DEBUGP("NO multiple ports found, port %hu ignored\n", port);
					}

					/*
					 * note we cannot just memcpy() if the sizes are the same.
					 * the mangle function does skb resizing, checks for a
					 * cloned skb, and updates the checksums.
					 *
					 * parameter 4 below is offset from start of tcp data.
					 */
					diff = origlen-rbuflen;
					if (!nf_nat_mangle_tcp_packet(*pskb, ct, ctinfo, protoff_nat,
								origoff, origlen, rbuf, rbuflen))
					{
						/* mangle failed, all we can do is bail */
						nf_ct_unexpect_related(exp);
						return 0;
					}
					get_skb_tcpdata(*pskb, &ptcp, &tcplen);
					ptran = ptcp+tranoff;
					tranlen -= diff;
					nextparamoff -= diff;
					nextfieldoff -= diff;
				}
			}

			off = nextfieldoff;
		}

		off = nextparamoff;
	}

	if(nat) {
		this_way = &nat->seq[dir];
	}
	if (this_way && this_way->offset_after != off_adj) {
		printk ("inside ----------------- corpos %d %d \n", off_adj, this_way->offset_after);

		this_way->offset_after = off_adj;
		nf_conntrack_event_cache(IPCT_NATSEQADJ, ct);

		/* nf_conntrack_tcp_update(*pskb, (*pskb)->nh.iph->ihl*4,  ct, CTINFO2DIR(ctinfo));
		nf_proto_csum_replace2(&tcph->check, *pskb,  htons(oldlen), htons(datalen), 1); */
	
	}

        DEBUGP(" outside MANGLE  TRANS \n");
	return 1;
}

int  get_src_port_num2(struct sk_buff **pskb, u_int16_t *port)
{
	struct iphdr *iph = ip_hdr(*pskb);
	struct udphdr *udph;
	struct tcphdr *tcph;
	if(iph->protocol == 0x06) {
		tcph = (void*)iph+iph->ihl*4;
		*port = tcph->source;
	} else if(iph->protocol == 0x11) {
		udph = (void*)iph+iph->ihl*4;
		*port = udph->source;
	} else
		return -1;
	return 0;
}
int  get_dest_port_num2(struct sk_buff **pskb, u_int16_t *port)
{
	struct iphdr *iph = ip_hdr(*pskb);
	struct udphdr *udph;
	struct tcphdr *tcph;
	if(iph->protocol == 0x06) {
		tcph = (void*)iph+iph->ihl*4;
		*port = tcph->dest;
	} else if(iph->protocol == 0x11) {
		udph = (void*)iph+iph->ihl*4;
		*port = udph->dest;
	} else
		return -1;
	return 0;

}


static uint
help_out(struct sk_buff **pskb, enum ip_conntrack_info ctinfo,
	 unsigned int matchoff, unsigned int matchlen,
         struct nf_ct_rtsp_expect *prtspexp,
         struct nf_conntrack_expect *exp, unsigned int protoff_nat)
{
    char*   ptcp;
    uint    tcplen;
    uint    hdrsoff;
    uint    hdrslen;
    uint    lineoff;
    uint    linelen;
    uint    off;
   
    struct nf_conn *ct = NULL;
 
    get_skb_tcpdata(*pskb, &ptcp, &tcplen);
    DEBUGP(" INSIDE nat help_out \n");
    hdrsoff = matchoff;

    hdrslen = prtspexp->len;
    off = hdrsoff;


  DEBUGP("\n help-OUT %u.%u.%u.%u:%u-%u.%u.%u.%u:%u    %u, %u , %u\n",
                  NIPQUAD(exp->tuple.src.u3.ip),
                 ntohs(exp->tuple.src.u.udp.port),
                 NIPQUAD(exp->tuple.dst.u3.ip),
             ntohs(exp->tuple.dst.u.udp.port),prtspexp->loport, hdrslen, matchlen);



 DEBUGP("whole HDR 22222.11111 before nat : len=%u, %.*s \n", (tcplen), (int)(tcplen), ptcp);

    while (nf_mime_nextline(ptcp, hdrsoff+hdrslen, &off, &lineoff, &linelen))
    {
        if (linelen == 0)
        {
            break;
        }
        if (off > hdrsoff+hdrslen)
        {
           // INFOP("!! overrun !!");
            break;
        }

	DEBUGP("hdr: len=%u, %.*s \n", linelen, (int)linelen, ptcp+lineoff);

        if (nf_strncasecmp(ptcp+lineoff, "Transport:", 10) == 0)
        {
            uint oldtcplen = tcplen;

            if (!rtsp_mangle_tran(ct, ctinfo, exp, prtspexp, pskb, lineoff, linelen, protoff_nat))
            {
                break;
            }

            get_skb_tcpdata(*pskb, &ptcp, &tcplen);
            hdrslen -= (oldtcplen-tcplen);
	    off -= (oldtcplen-tcplen);
            lineoff -= (oldtcplen-tcplen);
            linelen -= (oldtcplen-tcplen);
            DEBUGP("rep: len=%u, %.*s \n", linelen, (int)linelen, ptcp+lineoff);
        }
    }
   
    DEBUGP(" outside nat help_out \n"); 
    return NF_ACCEPT;
}

static unsigned int nf_nat_rtsp(struct sk_buff **pskb,
     enum ip_conntrack_info ctinfo,
    unsigned int matchoff, unsigned int matchlen,
     struct nf_ct_rtsp_expect *ct_rtsp_info,
     struct nf_conntrack_expect *exp, unsigned int protoff_nat)
{
	int dir = CTINFO2DIR(ctinfo);
	int rc = NF_ACCEPT;
	struct tcphdr _tcph, *th;
	
	th = skb_header_pointer(*pskb, protoff_nat, sizeof(_tcph), &_tcph);
	if (!th)
		return NF_ACCEPT;

	dataoff1 = protoff_nat + th->doff*4;
	datalen1 = (*pskb)->len - dataoff1;
	if (dataoff1 >= (*pskb)->len)
		return NF_ACCEPT;

#ifdef IP_NF_RTSP_DEBUG 
	rb_ptr1 = skb_header_pointer(*pskb, dataoff1, (*pskb)->len - dataoff1, rtsp_buffer1);

	BUG_ON(rb_ptr1 == NULL);

	pdata1 = rb_ptr1;
#endif

	exp_driver_info = ct_rtsp_info;

    switch (dir)
    {
    case IP_CT_DIR_ORIGINAL:
        rc = help_out(pskb, ctinfo, matchoff, matchlen, ct_rtsp_info, exp, protoff_nat);
        break;
    case IP_CT_DIR_REPLY:
         DEBUGP(" INSIDE nat ip_ct_dir_reply \n");
        /* XXX: unmangle */
        rc = NF_ACCEPT;
        break;
    }

    return rc;
}



/* This function is intentionally _NOT_ defined as  __exit */
static void fini(void)
{
	rcu_assign_pointer(nf_nat_rtsp_hook, NULL);
	nf_nat_rtsp_hook_expectfn = NULL;
        synchronize_net();
}

static int __init init(void)
{
	int ret = 0;

	printk("nf_nat_rtsp v" IP_NF_RTSP_VERSION " loading\n");

	 rcu_assign_pointer(nf_nat_rtsp_hook, nf_nat_rtsp );
	 nf_nat_rtsp_hook_expectfn = &expected ;

	if (stunaddr != NULL)
	{
		extip = in_aton(stunaddr);
	}
	if (destaction != NULL)
	{
		if (strcmp(destaction, "auto") == 0)
		{
			dstact = DSTACT_AUTO;
		}
		if (strcmp(destaction, "strip") == 0)
		{
			dstact = DSTACT_STRIP;
		}
		if (strcmp(destaction, "none") == 0)
		{
			dstact = DSTACT_NONE;
		}
	}
	return ret;
}

module_init(init);
module_exit(fini);
