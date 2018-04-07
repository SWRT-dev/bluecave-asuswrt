/* MMS extension for IP connection tracking
 * (C) 2002 by Filip Sneppe <filip.sneppe@cronos.be>
 * based on ip_conntrack_ftp.c and ip_conntrack_irc.c
 *
 * ip_conntrack_mms.c v0.3 2002-09-22
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 *      Module load syntax:
 *      insmod ip_conntrack_mms.o ports=port1,port2,...port<MAX_PORTS>
 *
 *      Please give the ports of all MMS servers You wish to connect to.
 *      If you don't specify ports, the default will be TCP port 1755.
 *
 *      More info on MMS protocol, firewalls and NAT:
 *      http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnwmt/html/MMSFirewall.asp
 *      http://www.microsoft.com/windows/windowsmedia/serve/firewall.asp
 *
 *      The SDP project people are reverse-engineering MMS:
 *      http://get.to/sdp
 *
 *  2005-02-13: Harald Welte <laforge@netfilter.org>
 *  	- port to 2.6.x
 *  	- update to work with post 2.6.11 helper API changes
 *  2010-12-08: LTQ changes
 *  	- port to 2.6.32
 *  	- reference to (*pskb)->nh.iph->ihl*4 has changed to protoff
 */

//#include <linux/config.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <linux/ctype.h>
#include <net/checksum.h>
#include <net/tcp.h>

#include <net/netfilter/nf_conntrack_helper.h>
#include <linux/netfilter/nf_conntrack_mms.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_expect.h>

#define MAX_PORTS 8
static int ports[MAX_PORTS];
static int ports_c;
module_param_array(ports, int, &ports_c, 0400);
MODULE_PARM_DESC(ports, "port numbers of MMS");

unsigned int proto_nat_mms_off ;
static char mms_buffer[65536];
static DEFINE_SPINLOCK(mms_buffer_lock);

unsigned int (*nf_nat_mms_hook)(struct sk_buff *skb,
				enum ip_conntrack_info ctinfo,
				const struct nf_ct_mms_expect *exp_mms_info,
				struct nf_conntrack_expect *exp, unsigned int proto_nat_mms_off);
EXPORT_SYMBOL(nf_nat_mms_hook);

#if 0
#define DEBUGP printk
#else
#define DEBUGP(format, args...)
#endif

MODULE_AUTHOR("Filip Sneppe <filip.sneppe@cronos.be>");
MODULE_DESCRIPTION("Microsoft Windows Media Services (MMS) connection tracking module");
MODULE_LICENSE("GPL");

/* #define isdigit(c) (c >= '0' && c <= '9') */

/* copied from drivers/usb/serial/io_edgeport.c - not perfect but will do the trick */
static void unicode_to_ascii (char *string, short *unicode, int unicode_size)
{
	int i;
	for (i = 0; i < unicode_size; ++i) {
                 
	 // add le16_to_cpu code for big enddien platform

		#ifdef CONFIG_CPU_BIG_ENDIAN
         	string[i] = (char)(le16_to_cpu(unicode[i])); // added for big endian
        #else 
		   string[i] = (char)(unicode[i]);
		#endif

	}
	string[unicode_size] = 0x00;
}

__inline static int atoi(char *s)
{
	int i=0;
	while (isdigit(*s)) {
		i = i*10 + *(s++) - '0';
	}
	return i;
}

/* convert ip address string like "192.168.0.10" to unsigned int */
__inline static u_int32_t asciiiptoi(char *s)
{
	unsigned int i, j, k;

	for(i=k=0; k<3; ++k, ++s, i<<=8) {
		i+=atoi(s);
		for(j=0; (*(++s) != '.') && (j<3); ++j)
			;
	}
	i+=atoi(s);
	return ntohl(i);
}

int parse_mms(const char *data,
	      const unsigned int datalen,
	      u_int32_t *mms_ip,
	      u_int16_t *mms_proto,
	      u_int16_t *mms_port,
	      char **mms_string_b,
	      char **mms_string_e,
	      char **mms_padding_e)
{
	int unicode_size, i;
	char tempstring[28];       /* "\\255.255.255.255\UDP\65535" */
	char getlengthstring[28];

	for(unicode_size=0;
	    (char) *(data+(MMS_SRV_UNICODE_STRING_OFFSET+unicode_size*2)) != (char)0;
	    unicode_size++)
		if ((unicode_size == 28) || (MMS_SRV_UNICODE_STRING_OFFSET+unicode_size*2 >= datalen))
			return -1; /* out of bounds - incomplete packet */

	unicode_to_ascii(tempstring, (short *)(data+MMS_SRV_UNICODE_STRING_OFFSET), unicode_size);
	DEBUGP("nf_conntrack_mms: offset 60: %s\n", (const char *)(tempstring));

	/* IP address ? */
	*mms_ip = asciiiptoi(tempstring+2);

	i=sprintf(getlengthstring, "%u.%u.%u.%u", NIPQUAD(*mms_ip));

	/* protocol ? */
	if(strncmp(tempstring+3+i, "TCP", 3)==0)
		*mms_proto = IPPROTO_TCP;
	else if(strncmp(tempstring+3+i, "UDP", 3)==0)
		*mms_proto = IPPROTO_UDP;

	/* port ? */
	*mms_port = atoi(tempstring+7+i);

	/* we store a pointer to the beginning of the "\\a.b.c.d\proto\port"
	   unicode string, one to the end of the string, and one to the end
	   of the packet, since we must keep track of the number of bytes
	   between end of the unicode string and the end of packet (padding) */
	*mms_string_b  = (char *)(data + MMS_SRV_UNICODE_STRING_OFFSET);
	*mms_string_e  = (char *)(data + MMS_SRV_UNICODE_STRING_OFFSET + unicode_size * 2);
	*mms_padding_e = (char *)(data + datalen); /* looks funny, doesn't it */
	return 0;
}


/* FIXME: This should be in userspace.  Later. */
static int help(struct sk_buff *skb, unsigned int protoff,
		struct nf_conn *ct,
		enum ip_conntrack_info ctinfo)
{
	int ret = NF_DROP;
	struct tcphdr _tcph, *th;
	char *data, *mb_ptr;
	unsigned int datalen, dataoff;
        typeof(nf_nat_mms_hook) mms_data_fixup; 

	//struct tcphdr *tcph = (void *)iph + iph->ihl * 4;
	//unsigned int tcplen = len - iph->ihl * 4;
	//unsigned int datalen = tcplen - tcph->doff * 4;
	int dir = CTINFO2DIR(ctinfo);
	struct nf_conntrack_expect *exp;
	struct nf_ct_mms_expect _emmi, *exp_mms_info = &_emmi;
	struct nf_conntrack_man cmd = {};
	u_int32_t mms_ip;
	union nf_inet_addr *mms_ip_addr;	
	u_int16_t mms_proto;
	char mms_proto_string[8];
	u_int16_t mms_port;
	__be16 mms_port_t;
	char *mms_string_b, *mms_string_e, *mms_padding_e;

	/* Until there's been traffic both ways, don't look in packets. */
	if (ctinfo != IP_CT_ESTABLISHED
	    && ctinfo != IP_CT_ESTABLISHED+IP_CT_IS_REPLY) {
		DEBUGP("ip_conntrack_mms: Conntrackinfo = %u\n", ctinfo);
		return NF_ACCEPT;
	}

	proto_nat_mms_off = protoff;

	/* Not whole TCP header? */
	th = skb_header_pointer(skb, protoff, sizeof(_tcph), &_tcph);
	if (!th)
		return NF_ACCEPT;

	/* No data ? */
	/*reference to (*pskb)->nh.iph->ihl*4 has changed to protoff */
	dataoff = protoff  + th->doff*4;
	datalen = skb->len - dataoff;
	if (dataoff >= skb->len)
		return NF_ACCEPT;

	spin_lock_bh(&mms_buffer_lock);
	mb_ptr = skb_header_pointer(skb, dataoff,
				    skb->len - dataoff, mms_buffer);
	BUG_ON(mb_ptr == NULL);

	data = mb_ptr;

#if 0
	/* Checksum invalid?  Ignore. */
	/* FIXME: Source route IP option packets --RR */
	if (tcp_v4_check(tcph, tcplen, iph->saddr, iph->daddr,
	    csum_partial((char *)tcph, tcplen, 0))) {
		DEBUGP("mms_help: bad csum: %p %u %u.%u.%u.%u %u.%u.%u.%u\n",
		       tcph, tcplen, NIPQUAD(iph->saddr),
		       NIPQUAD(iph->daddr));
		return NF_ACCEPT;
	}
#endif

	/* Only look at packets with 0x00030002/196610 on bytes 36->39 of TCP
	 * payload */

	/* FIXME: There is an issue with only looking at this packet: before
	 * this packet, the client has already sent a packet to the server with
	 * the server's hostname according to the client (think of it as the
	 * "Host: " header in HTTP/1.1). The server will break the connection
	 * if this doesn't correspond to its own host header. The client can
	 * also connect to an IP address; if it's the server's IP address, it
	 * will not break the connection. When doing DNAT on a connection where
	 * the client uses a server's IP address, the nat module should detect
	 * this and change this string accordingly to the DNATed address. This
	 * should probably be done by checking for an IP address, then storing
	 * it as a member of struct ip_ct_mms_expect and checking for it in
	 * ip_nat_mms...
	 */
	
	// add le32_to_cpu code for big enddien platform

	#ifdef CONFIG_CPU_BIG_ENDIAN
	if ((MMS_SRV_MSG_OFFSET >= datalen) ||
		(le32_to_cpu(*(u32 *)(data+MMS_SRV_MSG_OFFSET)) != MMS_SRV_MSG_ID)) 
		goto out;
    #else
     if ((MMS_SRV_MSG_OFFSET >= datalen) ||
        ((*(u32 *)(data+MMS_SRV_MSG_OFFSET)) != MMS_SRV_MSG_ID))
         goto out;
						 
	#endif
	DEBUGP("nf_conntrack_mms: offset 37: %u %u %u %u, datalen:%u\n",
		   (u8)*(data+36), (u8)*(data+37), (u8)*(data+38), (u8)*(data+39),
		   datalen);
	if (parse_mms(data, datalen, &mms_ip, &mms_proto, &mms_port,
				  &mms_string_b, &mms_string_e, &mms_padding_e))
		if (net_ratelimit())
			/* FIXME: more verbose debugging ? */
			printk(KERN_WARNING
				   "ip_conntrack_mms: Unable to parse "
				   "data payload\n");

	sprintf(mms_proto_string, "(%u)", mms_proto);
	DEBUGP("nf_conntrack_mms: adding %s expectation "
		   "%u.%u.%u.%u -> %u.%u.%u.%u:%u\n",
		   mms_proto == IPPROTO_TCP ? "TCP"
		   : mms_proto == IPPROTO_UDP ? "UDP":mms_proto_string,
		   NIPQUAD(ct->tuplehash[!dir].tuple.src.ip),
		   NIPQUAD(mms_ip),
		   mms_port);

	/* it's possible that the client will just ask the server to
	 * tunnel the stream over the same TCP session (from port
	 * 1755): there's shouldn't be a need to add an expectation in
	 * that case, but it makes NAT packet mangling so much easier
	 * */

	//DEBUGP("ip_conntrack_mms: tcph->seq = %u\n", tcph->seq);

	cmd.l3num = nf_ct_l3num(ct);
	memcpy(cmd.u3.all, &ct->tuplehash[dir].tuple.src.u3.all,
			sizeof(cmd.u3.all));

	if ((exp = nf_ct_expect_alloc(ct)) == NULL) {
		ret = NF_DROP;
		goto out;
	}

	exp_mms_info->offset  = (mms_string_b - data);
	exp_mms_info->len     = (mms_string_e  - mms_string_b);
	exp_mms_info->padding = (mms_padding_e - mms_string_e);
	exp_mms_info->port    = mms_port;

	DEBUGP("nf_conntrack_mms: wrote info  (ofs=%u), "
		   "len=%d, padding=%u\n",  (mms_string_e - data),
		   exp_mms_info->len, exp_mms_info->padding);

	/*exp->tuple = ((struct ip_conntrack_tuple)
		{ { ct->tuplehash[!dir].tuple.src.ip, { 0 } },
		  { mms_ip,
			{ .tcp = { (__u16) ntohs(mms_port) } },
			mms_proto } }
		);
	exp->mask  = ((struct ip_conntrack_tuple)
		{ { 0xFFFFFFFF, { 0 } },
		  { 0xFFFFFFFF, { .tcp = { 0xFFFF } }, 0xFF }});*/

	mms_ip_addr = (union nf_inet_addr *)&mms_ip;
	mms_port_t = (__be16)htons(mms_port);
	nf_ct_expect_init(exp, NF_CT_EXPECT_CLASS_DEFAULT, cmd.l3num,
			&ct->tuplehash[!dir].tuple.src.u3,
			mms_ip_addr, mms_proto, NULL, 
			&mms_port_t);

//               exp->tuple.src.u3 =  ct->tuplehash[!dir].tuple.src.u3;
                exp->mask.src.u3.ip = 0xffffffff;
                exp->tuple.src.u.tcp.port = 0 ;
                exp->mask.src.u.tcp.port = 0;

  //              exp->tuple.dst.u3.ip = mms_ip;
   //             exp->mask.src.u3.ip = 0xffffffff;
     //           exp->tuple.dst.u.tcp.port =  htons(mms_port);
       //         exp->mask.dst.u.tcp.port = 0xffff;
         //       exp->tuple.dst.protonum = mms_proto;
           //     exp->mask.dst.protonum = 0xff;


	exp->expectfn = NULL;
	exp->master = ct;


     mms_data_fixup = rcu_dereference(nf_nat_mms_hook);
	if (mms_data_fixup && ct->status & IPS_NAT_MASK)
		ret = mms_data_fixup(skb, ctinfo, exp_mms_info, exp, proto_nat_mms_off);
	else if (nf_ct_expect_related(exp) != 0)
		ret = NF_DROP;

	nf_ct_expect_put(exp);

out:
	spin_unlock_bh(&mms_buffer_lock);
	return ret;
}

static struct nf_conntrack_helper mms[MAX_PORTS];

static const struct nf_conntrack_expect_policy mms_exp_policy = {
		.max_expected   = 1,
		.timeout        = 120,
};

/* Not __exit: called from init() */
static void fini(void)
{
	int i;
	for (i = 0; (i < MAX_PORTS) && ports[i]; i++) {
		DEBUGP("ip_conntrack_mms: unregistering helper for port %d\n",
				ports[i]);
		nf_conntrack_helper_unregister(&mms[i]);
	}
}

static int __init init(void)
{
	int i, ret;

	if (ports[0] == 0)
		ports[0] = MMS_PORT;

	for (i = 0; (i < MAX_PORTS) && ports[i]; i++) {
		memset(&mms[i], 0, sizeof(struct nf_conntrack_helper));
		mms[i].tuple.src.l3num = PF_INET;
		mms[i].tuple.src.u.tcp.port = htons(ports[i]);
		mms[i].tuple.dst.protonum = IPPROTO_TCP;
		//mms[i].mask.src.u.tcp.port = 0xFFFF; //mask field not present
		//in nf_conntrack_helper
		//mms[i].mask.dst.protonum = 0xFF;
		mms[i].expect_policy = &mms_exp_policy;
		mms[i].me = THIS_MODULE;
		mms[i].help = help;

		if (ports[i] == MMS_PORT)
			sprintf(mms[i].name, "mms");
		else
			sprintf(mms[i].name, "mms-%d", ports[i]);
		
		printk("nf_conntrack_mms: registering helper for port %d\n",
				ports[i]);
		ret = nf_conntrack_helper_register(&mms[i]);

		if (ret) {
			fini();
			return ret;
		}
		ports_c++;
	}
	return 0;
}

module_init(init);
module_exit(fini);
