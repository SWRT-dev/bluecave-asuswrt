/* MMS extension for TCP NAT alteration.
 * (C) 2002 by Filip Sneppe <filip.sneppe@cronos.be>
 * based on ip_nat_ftp.c and ip_nat_irc.c
 *
 * ip_nat_mms.c v0.3 2002-09-22
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 *      Module load syntax:
 *      insmod ip_nat_mms.o ports=port1,port2,...port<MAX_PORTS>
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

 */

/* FIXME: issue with UDP & fragmentation with this URL:
   http://www.cnn.com/video/world/2002/01/21/jb.shoe.bomb.cafe.cnn.low.asx
   may be related to out-of-order first packets:
   basically the expectation is set up correctly, then the server sends
   a first UDP packet which is fragmented plus arrives out-of-order.
   the MASQUERADING firewall with ip_nat_mms loaded responds with
   an ICMP unreachable back to the server */

#include <linux/module.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <net/tcp.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_helper.h>
//#include <net/netfilter/nf_nat_rule.h>
#include <linux/netfilter/nf_conntrack_mms.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_expect.h>

#if 0
#define DEBUGP printk
#define DUMP_BYTES(address, counter)                                \
({                                                                  \
	int temp_counter;                                           \
	for(temp_counter=0; temp_counter<counter; ++temp_counter) { \
		DEBUGP("%u ", (u8)*(address+temp_counter));         \
	};                                                          \
	DEBUGP("\n");                                               \
})
#else
#define DEBUGP(format, args...)
#define DUMP_BYTES(address, counter)
#endif

MODULE_AUTHOR("Filip Sneppe <filip.sneppe@cronos.be>");
MODULE_DESCRIPTION("Microsoft Windows Media Services (MMS) NAT module");
MODULE_LICENSE("GPL");

static unsigned int mms_data_fixup(struct sk_buff *skb,
                          enum ip_conntrack_info ctinfo,
			  const struct nf_ct_mms_expect *ct_mms_info,
                          struct nf_conntrack_expect *expect, unsigned int protoff_nat_mms)
{
	u_int32_t newip;
	struct nf_conn *ct = expect->master;
	struct iphdr *iph = ip_hdr(skb);
	struct tcphdr *tcph = (void *) iph + iph->ihl * 4;
	char *data = (char *)tcph + tcph->doff * 4;
	int i, j, k, port;
	u_int16_t mms_proto;

	u_int32_t *mms_chunkLenLV    = (u_int32_t *)(data + MMS_SRV_CHUNKLENLV_OFFSET);
	u_int32_t *mms_chunkLenLM    = (u_int32_t *)(data + MMS_SRV_CHUNKLENLM_OFFSET);
	u_int32_t *mms_messageLength = (u_int32_t *)(data + MMS_SRV_MESSAGELENGTH_OFFSET);

	int zero_padding;

	char buffer[28];         /* "\\255.255.255.255\UDP\65635" * 2
				    (for unicode) */
	char unicode_buffer[75]; /* 27*2 (unicode) + 20 + 1 */
	char proto_string[6];

	/* what was the protocol again ? */
	mms_proto = expect->tuple.dst.protonum;
	sprintf(proto_string, "%u", mms_proto);

	newip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip;
	expect->saved_proto.tcp.port = expect->tuple.dst.u.tcp.port;
	expect->expectfn = nf_nat_follow_master;

	/* Alter conntrack's expectations. */
	for (port = ct_mms_info->port; port != 0; port++) {
		expect->tuple.dst.u.tcp.port = htons(port);
		if (nf_ct_expect_related(expect) == 0) {
			DEBUGP("ip_nat_mms: mms_data_fixup: using port %d\n",
				port);
			break;
		}
	}

	if (port == 0)
		return NF_DROP;

	sprintf(buffer, "\\\\%u.%u.%u.%u\\%s\\%u",
	        NIPQUAD(newip),
		expect->tuple.dst.protonum == IPPROTO_UDP ? "UDP"
		: expect->tuple.dst.protonum == IPPROTO_TCP ? "TCP":proto_string,
		port);
	DEBUGP("ip_nat_mms: new unicode string=%s\n", buffer);

	memset(unicode_buffer, 0, sizeof(char)*75);

	for (i=0; i<strlen(buffer); ++i)
		*(unicode_buffer+i*2)=*(buffer+i);

	DEBUGP("ip_nat_mms: mms_data_fixup: padding: %u len: %u\n",
		ct_mms_info->padding, ct_mms_info->len);
	DEBUGP("ip_nat_mms: mms_data_fixup: offset: %u\n",
		MMS_SRV_UNICODE_STRING_OFFSET+ct_mms_info->len);
	DUMP_BYTES(data+MMS_SRV_UNICODE_STRING_OFFSET, 60);

	/* add end of packet to it */
	for (j=0; j<ct_mms_info->padding; ++j) {
		DEBUGP("ip_nat_mms: mms_data_fixup: i=%u j=%u byte=%u\n",
		       i, j, (u8)*(data+MMS_SRV_UNICODE_STRING_OFFSET+ct_mms_info->len+j));
		*(unicode_buffer+i*2+j) = *(data+MMS_SRV_UNICODE_STRING_OFFSET+ct_mms_info->len+j);
	}

	/* pad with zeroes at the end ? see explanation of weird math below */
	zero_padding = (8-(strlen(buffer)*2 + ct_mms_info->padding + 4)%8)%8;
	for (k=0; k<zero_padding; ++k)
		*(unicode_buffer+i*2+j+k)= (char)0;

	DEBUGP("ip_nat_mms: mms_data_fixup: zero_padding = %u\n", zero_padding);

	 // Add le32_to_cpu code for big enddien platform
#ifdef CONFIG_CPU_BIG_ENDIAN
DEBUGP("ip_nat_mms: original=> chunkLenLV=%u chunkLenLM=%u messageLength=%u\n",
           le32_to_cpu(*mms_chunkLenLV), le32_to_cpu(*mms_chunkLenLM), le32_to_cpu(*mms_messageLength));
#else
DEBUGP("ip_nat_mms: original=> chunkLenLV=%u chunkLenLM=%u "
      "messageLength=%u\n", *mms_chunkLenLV, *mms_chunkLenLM,
      *mms_messageLength);
						  
#endif

	/* explanation,
	   strlen(buffer)*2 + ct_mms_info->padding + 4 must be divisable by 8;
	   divide by 8 and add 3 to compute the mms_chunkLenLM field,
	   but note that things may have to be padded with zeroes to align by 8
	   bytes, hence we add 7 and divide by 8 to get the correct length */

        // Add le32_to_cpu code for big enddien platform
#ifdef CONFIG_CPU_BIG_ENDIAN
	*mms_chunkLenLM    = cpu_to_le32((u_int32_t) (3+(strlen(buffer)*2+ct_mms_info->padding+11)/8));
	*mms_chunkLenLV    = cpu_to_le32(le32_to_cpu(*mms_chunkLenLM)+2);
	*mms_messageLength = cpu_to_le32(le32_to_cpu(*mms_chunkLenLV)*8);
#else
	*mms_chunkLenLM    = (u_int32_t) (3+(strlen(buffer)*2+ct_mms_info->padding+11)/8);
    *mms_chunkLenLV    = *mms_chunkLenLM+2;
    *mms_messageLength = *mms_chunkLenLV*8;
					

#endif

	DEBUGP("ip_nat_mms: modified=> chunkLenLV=%u chunkLenLM=%u"
	       " messageLength=%u\n", *mms_chunkLenLV, *mms_chunkLenLM,
	       *mms_messageLength);

	nf_nat_mangle_tcp_packet(skb, ct, ctinfo, protoff_nat_mms,
	                         ct_mms_info->offset,
	                         ct_mms_info->len + ct_mms_info->padding,
				 unicode_buffer, strlen(buffer)*2 +
				 ct_mms_info->padding + zero_padding);
	DUMP_BYTES(unicode_buffer, 60);

	return NF_ACCEPT;
}

static void __exit fini(void)
{
	 rcu_assign_pointer(nf_nat_mms_hook,NULL);
	synchronize_net();
}

static int __init init(void)
{

	printk("nf_nat_mms loading ..\n");
	BUG_ON(rcu_dereference(nf_nat_mms_hook));

       rcu_assign_pointer(nf_nat_mms_hook,mms_data_fixup);

	return 0;
}

module_init(init);
module_exit(fini);
