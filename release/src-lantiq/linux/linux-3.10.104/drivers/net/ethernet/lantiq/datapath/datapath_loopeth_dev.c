/*
* ####################################
*              Head File

cd /proc/dp/loopeth

#create loopeth devices
echo add > dev

#dynamically register loopeth1 to datapath
echo register dev   loopeth1 1 DIRECTPATH > directpath
echo register subif loopeth1 > directpath

#dynamically register loopeth2 to datapath
echo register dev   loopeth2 2 DIRECTPATH > directpath
echo register subif loopeth2 > directpath

#dynamically register loopeth3 to datapath
echo register dev   loopeth3 3 DIRECTPATH > directpath
echo register subif loopeth3 > directpath

#register loopeth4 to datapath with specified port_id 12
echo register dev   loopeth4 4 DIRECTPATH 12 > directpath
echo register subif loopeth4 > directpath
echo register subif loopeth5 12 > directpath

* ####################################
*/

/*  Common Head File*/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/atmdev.h>
#include <linux/init.h>
#include <linux/etherdevice.h>	/*  eth_type_trans  */
#include <linux/ethtool.h>	/*  ethtool_cmd     */
#include <linux/if_ether.h>
#include <linux/skbuff.h>
#include <linux/inetdevice.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <net/tcp.h>
#include <linux/uaccess.h>
#include <asm/unistd.h>
#include <asm/irq.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <asm/checksum.h>
#include <linux/errno.h>
#ifdef CONFIG_XFRM
#include <net/xfrm.h>
#endif

#include <net/datapath_api.h>
#include <net/datapath_proc_api.h>
#include "datapath.h"

/*####################################
*              Definition
* ####################################
*/

#define ENABLE_DEBUG                            1

#define ENABLE_ASSERT                           1

#define DEBUG_DUMP_SKB                          1

#if defined(ENABLE_DEBUG) && ENABLE_DEBUG
#define ENABLE_DEBUG_PRINT                    1
#define DISABLE_INLINE                        1
#else
#define ENABLE_DEBUG_PRINT                    0
#define DISABLE_INLINE                        0
#endif

#if !defined(DISABLE_INLINE) || !DISABLE_INLINE
#define INLINE                                inline
#else
#define INLINE
#endif

#define err(format, arg...)    do {;\
		PRINTK(KERN_ERR ":%d:%s: " format "\n", \
		       __LINE__, __FUNCTION__, ##arg); } \
	while (0)

#if defined(ENABLE_DEBUG_PRINT) && ENABLE_DEBUG_PRINT
#undef  dbg
#define dbg(format, arg...) do { if ((g_dbg_enable &\
					      DBG_ENABLE_MASK_DEBUG_PRINT)) \
		PRINTK(KERN_WARNING ":%d:%s: " format "\n",\
		       __LINE__, __FUNCTION__, ##arg); } \
while (0)
#else
#if !defined(dbg)
#define dbg(format, arg...)
#endif
#endif

#if defined(ENABLE_ASSERT) && ENABLE_ASSERT
#define ASSERT(cond, format, arg...)      do {                \
		if ((g_dbg_enable & DBG_ENABLE_MASK_ASSERT) && !(cond)) \
			pr_err(KERN_ERR ":%d:%s: " format "\n",      \
			       __LINE__, __FUNCTION__, ##arg); } \
	while (0)
#else
#define ASSERT(cond, format, arg...)
#endif

#if defined(DEBUG_DUMP_SKB) && DEBUG_DUMP_SKB
#define DUMP_SKB_LEN                          ~0
#endif

#if (defined(DEBUG_DUMP_SKB) && DEBUG_DUMP_SKB)                     \
|| (defined(ENABLE_DEBUG_PRINT) && ENABLE_DEBUG_PRINT)          \
|| (defined(ENABLE_ASSERT) && ENABLE_ASSERT)
#define ENABLE_DBG_PROC                       1
#else
#define ENABLE_DBG_PROC                       0
#endif

/* Debug Print Mask*/
#define DBG_ENABLE_MASK_ERR                     0x0001
#define DBG_ENABLE_MASK_DEBUG_PRINT             0x0002
#define DBG_ENABLE_MASK_ASSERT                  0x0004
#define DBG_ENABLE_MASK_DUMP_SKB_RX             0x0008
#define DBG_ENABLE_MASK_DUMP_SKB_TX             0x0010
#define DBG_ENABLE_MASK_ALL     (DBG_ENABLE_MASK_ERR | \
				 DBG_ENABLE_MASK_DEBUG_PRINT |\
				 DBG_ENABLE_MASK_ASSERT |\
				 DBG_ENABLE_MASK_DUMP_SKB_RX |\
				 DBG_ENABLE_MASK_DUMP_SKB_TX)

/* Constant Definition*/
#define ETH_WATCHDOG_TIMEOUT                    (10 * HZ)
#define MAX_RX_QUEUE_LENGTH                     100
#define TASKLET_HANDLE_BUDGET                   25

/* Ethernet Frame Definitions*/
#define ETH_CRC_LENGTH                          4
#define ETH_MAX_DATA_LENGTH                     ETH_DATA_LEN
#define ETH_MIN_TX_PACKET_LENGTH                ETH_ZLEN

/* ####################################
*              Data Type
* ####################################
*/

/* Internal Structure of Devices (ETH/ATM)*/
#define LOOPETH_F_FREE           0
#define LOOPETH_F_REGISTER_DEV   1
#define LOOPETH_F_REGISTER_SUBIF 2

struct loop_eth_priv_data {
	int id;
	struct net_device_stats stats;
	unsigned int rx_preprocess_drop;
	struct sk_buff_head rx_queue;
	struct tasklet_struct rx_tasklet;
	int f_tx_queue_stopped;
	unsigned char dev_addr[MAX_ADDR_LEN];
	unsigned int dp_pkts_to_ppe;
	unsigned int dp_pkts_to_ppe_fail;
	unsigned int dp_pkts_from_ppe;
	unsigned int dp_pkts_tx;

	struct module *owner;
	dp_subif_t dp_subif;
	int32_t dev_port;	/*dev  instance */
	int32_t f_dp;		/* status for register to datapath */
};

static const char *const dbg_enable_mask_str[] = {
	"err",			/*DBG_ENABLE_MASK_ERR */
	"dbg",			/*DBG_ENABLE_MASK_DEBUG_PRINT */
	"assert",		/*DBG_ENABLE_MASK_ASSERT */
	"rx",			/*DBG_ENABLE_MASK_DUMP_SKB_RX */
	"tx"			/*DBG_ENABLE_MASK_DUMP_SKB_TX */
};

/*####################################
*            Local Variable
* ####################################
*/

#define MAX_LOOPETH_NUM ((PMAC_MAX_NUM*MAX_SUBIF_PER_PORT)+2)	/* 16 PMAC and each port support 16 subif */
static struct net_device *g_loop_eth_dev[MAX_LOOPETH_NUM] = { 0 };
static uint32_t g_loop_eth_dev_flag[MAX_LOOPETH_NUM] = { 0 };


static struct module g_loop_eth_module[MAX_LOOPETH_NUM];

#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
static int g_dbg_enable = DBG_ENABLE_MASK_ERR | DBG_ENABLE_MASK_ASSERT;
#endif

/* Network Operations*/
static void eth_setup(struct net_device *);
static struct net_device_stats *eth_get_stats(struct net_device *);
static int eth_open(struct net_device *);
static int eth_stop(struct net_device *);
static int eth_hard_start_xmit(struct sk_buff *, struct net_device *);
static int eth_ioctl(struct net_device *, struct ifreq *, int);
static void eth_tx_timeout(struct net_device *);

/* RX path functions*/
static INLINE int eth_rx_preprocess(struct sk_buff *, int);
static INLINE void eth_rx_handler(struct sk_buff *, int);
static void do_loop_eth_rx_tasklet(unsigned long);

/* Datapath directpath functions*/
static int32_t dp_fp_stop_tx(struct net_device *);
static int32_t dp_fp_restart_tx(struct net_device *);
static int32_t dp_fp_rx(struct net_device *, struct net_device *,
			struct sk_buff *, int32_t);

static const struct net_device_ops loop_netdev_ops = {
	.ndo_open = eth_open,
	.ndo_stop = eth_stop,
	.ndo_start_xmit = eth_hard_start_xmit,
	.ndo_do_ioctl = eth_ioctl,
	.ndo_tx_timeout = eth_tx_timeout,
	.ndo_get_stats = eth_get_stats,
	.ndo_set_mac_address = eth_mac_addr,
	.ndo_change_mtu = eth_change_mtu,
};

/*###################################
*           Global Variable
* ####################################
*/

/*####################################
*             Declaration
* ####################################
*/

/* Wrapper for Different Kernel Version
*/
static inline struct net_device *ltq_dev_get_by_name(const char *name)
{
	return dev_get_by_name(&init_net, name);
}

/*find the loopeth index via its device name
return -1: not found
>=0: index

*/
int find_loopeth_index_via_name(char *ifname)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(g_loop_eth_dev); i++) {
		if (g_loop_eth_dev[i]
		    && dp_strcmpi(ifname, g_loop_eth_dev[i]->name) == 0)
			return i;
	}

	return -1;
}

static inline unsigned long ltq_get_xmit_fn(struct net_device *dev)
{
	return (unsigned long)dev->netdev_ops->ndo_start_xmit;
}

/*####################################
*            Local Function
* ####################################
*/

static void eth_setup(struct net_device *dev)
{
	struct loop_eth_priv_data *priv = netdev_priv(dev);

	ether_setup(dev);	/*  assign some members */
	dev->netdev_ops = &loop_netdev_ops;
	dev->watchdog_timeo = ETH_WATCHDOG_TIMEOUT;
	priv->id = -1;
	priv->dp_subif.port_id = -1;
	skb_queue_head_init(&priv->rx_queue);
	return;
}

struct net_device_stats *eth_get_stats(struct net_device *dev)
{
	struct loop_eth_priv_data *priv = netdev_priv(dev);
	return &priv->stats;
}

int eth_open(struct net_device *dev)
{
	dbg("open %s", dev->name);
	netif_start_queue(dev);
	return 0;
}

int eth_stop(struct net_device *dev)
{
	netif_stop_queue(dev);
	return 0;
}

int eth_hard_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	unsigned long sysflag;
	struct loop_eth_priv_data *priv = netdev_priv(dev);
	int rx_queue_len;
	struct sk_buff *old_skb = skb;

	if (!skb)
		return 0;
	if (g_dbg_enable & DBG_ENABLE_MASK_DEBUG_PRINT)
		dp_dump_raw_data(skb->data, skb->len,
				 "loopeth_xmit original data");
	if (skb_cloned(old_skb) && !skb_is_gso(old_skb)) {
		/*sanity check before creating a new skb */
		if (skb_shinfo(old_skb)->frag_list) {
			pr_err(" Not support frag_list here !!\n");
			dev_kfree_skb_any(old_skb);
			return 0;
		}
		if (old_skb->data_len) {
			pr_err(" Not support nr_frags here since data_len not zero!!\n");
			dev_kfree_skb_any(old_skb);
			return 0;
		}
		skb = alloc_skb(old_skb->len, GFP_KERNEL);
		if (!skb) {
			dev_kfree_skb_any(old_skb);
			return 0;
		}
		memcpy(skb->data, old_skb->data, old_skb->len);
		skb_put(skb, old_skb->len);
		dev_kfree_skb_any(old_skb);
		if (g_dbg_enable & DBG_ENABLE_MASK_DEBUG_PRINT)
			dp_dump_raw_data(skb->data, skb->len,
				 "loopeth_xmit original data after creating skb");
	}

	ASSERT((skb->prev == NULL &&
		skb->next == NULL),
	       "skb on list: prev = 0x%08x, next = 0x%08x",
	       (unsigned int)skb->prev, (unsigned int)skb->next);

	if (g_dbg_enable & DBG_ENABLE_MASK_DEBUG_PRINT)
		dp_dump_raw_data(skb->data, skb->len,
				 "loopeth_xmit for spoofing check:");

	skb->dev = g_loop_eth_dev[priv->id];
	spin_lock_irqsave(&priv->rx_queue.lock, sysflag);
	rx_queue_len = skb_queue_len(&priv->rx_queue);

	if (rx_queue_len < MAX_RX_QUEUE_LENGTH) {
		__skb_queue_tail(&priv->rx_queue, skb);

		if (rx_queue_len == 0)
			tasklet_schedule(&priv->rx_tasklet);

		if (skb_queue_len(&priv->rx_queue) >= MAX_RX_QUEUE_LENGTH)
			netif_stop_queue(g_loop_eth_dev[priv->id]);

		priv->stats.tx_packets++;
		priv->stats.tx_bytes += skb->len;
	} else {
		dbg("drop packet for long queue\n");
		dev_kfree_skb_any(skb);
		priv->stats.tx_dropped++;
	}

	spin_unlock_irqrestore(&priv->rx_queue.lock, sysflag);
	return 0;
}

int eth_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	switch (cmd) {
	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

void eth_tx_timeout(struct net_device *dev)
{
	/*TODO:must restart the TX channels */
	struct loop_eth_priv_data *priv = netdev_priv(dev);
	priv->stats.tx_errors++;
	netif_wake_queue(dev);
	return;
}

/* Ethernet frame types according to RFC 2516 */
#define ETH_PPPOE_DISCOVERY 0x8863
#define ETH_PPPOE_SESSION   0x8864

/* A PPPoE Packet, including Ethernet headers */
struct pppoe_pkt {
#ifdef PACK_BITFIELDS_REVERSED
	unsigned int type:4;	/* PPPoE Type (must be 1) */
	unsigned int ver:4;	/* PPPoE Version (must be 1) */
#else
	unsigned int ver:4;	/* PPPoE Version (must be 1) */
	unsigned int type:4;	/* PPPoE Type (must be 1) */
#endif
	unsigned int code:8;	/* PPPoE code */
	unsigned int session:16;	/* PPPoE session */
	unsigned int length:16;	/* Payload length */
	unsigned char payload[ETH_DATA_LEN];	/* A bit of room to spare, here just for space holder only */
};

/* PPPoE Tag */

unsigned char ppp_ipv4_proto[2] = {
	0x00, 0x21
};

unsigned char ppp_ipv6_proto[2] = {
	0x00, 0x57
};

#define VLAN_HEAD_SIZE  4
#define PPPOE_HEAD_SIZE  8

/*return 1: send to directpath API dp_xmit to pae
* return 0: Should be dropped for not know how to spoof
*/
static INLINE int eth_rx_preprocess(struct sk_buff *skb, int id)
{
	unsigned char *p;
	unsigned char mac[6];
	unsigned char ip[4];
	unsigned char port[2];
	unsigned char ip_templ[4] = { 0 };
	struct iphdr *iph;
	struct icmphdr *icmph;
	struct tcphdr *tcph;
	uint32_t t, off_t, *opt;
	int csum;
	struct pppoe_pkt *pppoe;
	int offset = 0;
	int vlan_num = 0;
	unsigned char *p_new_src_mac;
	struct in_device __rcu *in_dev = NULL;
	struct in_ifaddr *if_info = NULL;
	__u8 *addr;
	if (!skb)
		return 0;
	p = skb->data;

	if (skb->data[6] & 0x1) {
		/*source mac is broadcast or multicast. no spoof */
		return 0;
	}

	if (g_dbg_enable & DBG_ENABLE_MASK_DEBUG_PRINT)
		dp_dump_raw_data(skb->data, 20,
				 "eth_rx_preprocess original skb:");

	read_lock_bh(&dev_base_lock);
	in_dev = (struct in_device *)skb->dev->ip_ptr;

	if (!in_dev)
		dbg("ip_ptr NULL. No IP\n");
	else
		if_info = in_dev->ifa_list;

	if (if_info) {
		memcpy(ip_templ, (char *)&if_info->ifa_address, 4);
		addr = (char *)&if_info->ifa_local;

		dbg("Device %s ifa_local: %u.%u.%u.%u\n", skb->dev->name,
		    (__u32) addr[0], (__u32) addr[1], (__u32) addr[2],
		    (__u32) addr[3]);

		addr = (char *)&if_info->ifa_address;
		dbg("Device %s ifa_address: %u.%u.%u.%u\n", skb->dev->name,
		    (__u32) addr[0], (__u32) addr[1], (__u32) addr[2],
		    (__u32) addr[3]);

		addr = (char *)&if_info->ifa_mask;
		dbg("Device %s ifa_mask: %u.%u.%u.%u\n", skb->dev->name,
		    (__u32) addr[0], (__u32) addr[1], (__u32) addr[2],
		    (__u32) addr[3]);

		addr = (char *)&if_info->ifa_broadcast;
		dbg("Device %s ifa_broadcast: %u.%u.%u.%u\n", skb->dev->name,
		    (__u32) addr[0], (__u32) addr[1], (__u32) addr[2],
		    (__u32) addr[3]);
	}

	read_unlock_bh(&dev_base_lock);

	if (p[offset + 12] == 0x81 && p[offset + 13] == 0x00) {	/*VLAN header */
		offset += VLAN_HEAD_SIZE;
		vlan_num++;
		dbg("Found VLAN%d\n", vlan_num);
	}

	if (p[offset + 12] == 0x88 && p[offset + 13] == 0x63) {
		/*pppoe Discover(0x9)/Offer(0x7)/request(0x19)/Confirm(0x65) */
		return 0;
	}

	if (p[offset + 12] == 0x88 && p[offset + 13] == 0x64) {	/*ppp */
		pppoe = (struct pppoe_pkt *) (p + offset + 14);

		if ((pppoe->payload[0] == ppp_ipv4_proto[0]
		     && pppoe->payload[1] == ppp_ipv4_proto[1]) /*PPP IPv4 */ ||
		    ((pppoe->payload[0] == ppp_ipv6_proto[0])
		     && (pppoe->payload[1] ==
			 ppp_ipv6_proto[1])) /*  PPP IPv6 */) {
			offset += PPPOE_HEAD_SIZE;	/* skip 8 bytes ppp header */
			dbg("Found PPP IP packet\n");
		} else
			return 0;
	}

	/*swap dst/src mac address */
	memcpy(mac, p, 6);
	memcpy(p, p + 6, 6);
	memcpy(p + 6, mac, 6);
	p_new_src_mac = p + 6;
	p += offset;		/*Note, now p[12~13] points to protocol */

	if (p[12] == 0x08 && p[13] == 0x06) {
		/* arp request */
		if (p[14] == 0x00 && p[15] == 0x01 && p[16] == 0x08 &&
		    p[17] == 0x00 && p[20] == 0x00 && p[21] == 0x01) {
			/*fill in spoof mac address */
			p_new_src_mac[0] = 0;
			p_new_src_mac[1] = 1;
			p_new_src_mac[2] = id;
			p_new_src_mac[3] = id;
			p_new_src_mac[4] = id;
			p_new_src_mac[5] = id;

			if ((p[38] == ip_templ[0] && p[39] == ip_templ[1])
			    || (*(u32 *) ip_templ == 0)) {
				dbg("Spoof arp request:%d.%d.%d.%d mac:%02x:%02x:%02x:%02x:%02x:%02x\n", p[38], p[39], p[40], p[41], p_new_src_mac[0], p_new_src_mac[1], p_new_src_mac[2], p_new_src_mac[3], p_new_src_mac[4], p_new_src_mac[5]);

				/*  arp reply */
				p[21] = 0x02;
				/*  sender mac */
				memcpy(mac, p + 22, 6);	/*save orignal sender mac */
				memcpy(p + 22, p_new_src_mac, 6);	/*set new sender mac */

				if (memcmp(p + 28, p + 38, 4) == 0) {
					dbg("Not reply arp request for:%d.%d.%d.%d\n", p[38], p[39], p[40], p[41]);
					return 1;
				}

				/* sender IP */
				memcpy(ip, p + 28, 4);	/*save original sender ip address */
				memcpy(p + 28, p + 38, 4);	/*set new sender ip address */
				/* target mac */
				memcpy(p + 32, mac, 6);
				/* target IP */
				memcpy(p + 38, ip, 4);

				if (g_dbg_enable &
				    DBG_ENABLE_MASK_DEBUG_PRINT)
					dp_dump_raw_data(skb->data, 20,
							 "skb data after arp spoof");

				return 1;
			} else
				dbg("Not reply arp request for:%d.%d.%d.%d\n",
				    p[38], p[39], p[40], p[41]);

			return 1;
		}

		return 0;
	} else if (((p[12] == 0x08) && (p[13] == 0x00)) /*Normal IPV4 */ ||
		   ((p[12] == ppp_ipv4_proto[0]) &&
		    (p[13] == ppp_ipv4_proto[1])) /*PPP IPV4 */ ||
		   ((p[12] == ppp_ipv6_proto[0]
		     && p[13] == ppp_ipv6_proto[1])) /*PPP IPV6 */) {
		/* IP */
		switch ((int)p[23]) {
		case 0x01:

			/* ICMP - request */
			if (p[34] == 0x08) {
				/* src IP */
				memcpy(ip, p + 26, 4);
				memcpy(p + 26, p + 30, 4);
				/* dest IP */
				memcpy(p + 30, ip, 4);
				/* ICMP reply */
				p[34] = 0x00;
				/* IP checksum */
				iph = (struct iphdr *)(p + 14);
				iph->check = 0;
				iph->check =
				    ip_fast_csum((unsigned char *)iph,
						 iph->ihl);
				/* ICMP checksum */
				icmph = (struct icmphdr *)(p + 34);
				icmph->checksum = 0;
				csum = csum_partial((unsigned char *)
						    icmph, skb->len - 34, 0);
				icmph->checksum = csum_fold(csum);
				dbg("spoof ping\n");
				return 1;
			}

			break;

		case 0x11:

			/* UDP */
		case 0x06:
			/* TCP */
			/*swap src/dst ip */
			/* src IP */
			dbg("spoof udp/tcp\n");
			memcpy(ip, p + 26, 4);
			memcpy(p + 26, p + 30, 4);
			/* dest IP */
			memcpy(p + 30, ip, 4);
			/*shaoguoh remove below checksum item since we
			   just swap ip and port only
			 */
#if 0
			/* IP checksum */
			iph = (struct iphdr *)(p + 14);
			iph->check = 0;
			iph->check =
			    ip_fast_csum((unsigned char *)iph, iph->ihl);
			/* no UDP checksum */
			p[40] = p[41] = 0x00;
#endif
			/*shaoguoh add below to swap src/dst port 34~35 36~37 */
			/*save src port to port array and copy original dest
			   port to new src port
			 */
			memcpy(port, p + 34, 2);
			memcpy(p + 34, p + 36, 2);
			/* copy original src port to dest port */
			memcpy(p + 36, port, 2);

			/*return if UDP */
			if ((int)p[23] == 0x11)
				return 1;

			iph = (struct iphdr *)(p + 14);
			tcph = (struct tcphdr *)(p + 34);

			if (tcph->syn == 1) {	/*set syn & ack, set seq NO same as the incoming syn TCP packet, set ack seq NO as seq NO + 1 */
				tcph->ack = 1;
				tcph->ack_seq = tcph->seq + 1;
			} else if (tcph->fin == 1) {	/*set fin & ack */
				tcph->ack = 1;
				t = tcph->ack_seq;
				tcph->ack_seq = tcph->seq + 1;
				tcph->seq = t;
			} else if (tcph->rst == 1 || (tcph->psh == 0 && tcph->ack == 1))	/*rest or only ack, we ignore it. */
				return 0;
			else if (tcph->psh == 1) {
				t = tcph->ack_seq;

				if (iph->tot_len < 40)	/*corrupt packet, ignore it. */
					return -1;

				tcph->ack_seq =
				    tcph->seq + iph->tot_len -
				    (iph->ihl * 4) - (tcph->doff * 4);
				tcph->seq = t;
			}

			/*check timestamp */
			off_t = 14 + 20 + 20;	/*mac + ip + tcp */

			while ((tcph->doff << 2) > (off_t - 34)) {	/*tcp option compare tcp header length */

				switch (p[off_t]) {
				case 0x0:	/*Option End */
					break;

				case 0x1:	/* NO Operation */
					off_t += 1;
					continue;

				case 0x2:	/*Max Segment Size */
					off_t += 4;
					continue;

				case 0x3:	/* Window Scale */
					off_t += 3;
					continue;

				case 0x4:	/*TCP Sack permitted */
					off_t += 2;
					continue;

				case 0x8:	/*TCP timestamp */
#if 1
					opt = (uint32_t *) (p + off_t + 2);
					*(opt + 1) = htons(tcp_time_stamp);
					t = *opt;
					*opt = *(opt + 1);
					*(opt + 1) = t;

#else

					for (t = 0; t < 10; t++)
						*(p + off_t + t) = 1;

#endif
					off_t += 10;	/*option max is 64-20 */
					continue;

				default:
					off_t += 64;
					break;
				}
			}

			/* IP checksum */
			iph = (struct iphdr *)(p + 14);
			iph->check = 0;
			iph->check =
			    ip_fast_csum((unsigned char *)iph, iph->ihl);
			/* TCP checksum */
			tcph->check = 0;
			t = iph->tot_len - (iph->ihl * 4);
			/*tcph->check = csum_partial((unsigned char *)tcph, iph->tot_len - 20, 0); */
			tcph->check =
			    csum_tcpudp_magic(iph->saddr, iph->daddr, t,
					      IPPROTO_TCP, csum_partial(tcph,
									t,
									0));
			return 1;

		default:
			break;
		}

	}

	dbg("Don't know how to spoof. Should be dropped\n");
	return 0;
}

static INLINE void eth_rx_handler(struct sk_buff *skb, int id)
{
	struct loop_eth_priv_data *priv = netdev_priv(g_loop_eth_dev[id]);
	int pktlen;
	int flag = 0;

	pktlen = 0;

	if (!netif_running(g_loop_eth_dev[id])) {
		dev_kfree_skb_any(skb);
		priv->stats.rx_dropped++;
		return;
	}

	if (priv->dp_subif.port_id > 0) {
		dbg("to dp_xmit: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", skb->data[0], skb->data[1], skb->data[2], skb->data[3], skb->data[4], skb->data[5], skb->data[6], skb->data[7], skb->data[8], skb->data[9], skb->data[10], skb->data[11], skb->data[12], skb->data[13]);

		((struct dma_tx_desc_1 *)&(skb->DW1))->field.ep =
		    priv->dp_subif.port_id;
		((struct dma_tx_desc_0 *)&(skb->DW0))->field.dest_sub_if_id =
		    priv->dp_subif.subif;
		if (g_loop_eth_dev_flag[id] & DP_F_FAST_DSL)
			flag = DP_TX_DSL_FCS;
		if (dp_xmit
		    (g_loop_eth_dev[id], &priv->dp_subif, skb, skb->len,
		     flag) == DP_SUCCESS) {
			priv->dp_pkts_to_ppe++;
			return;
		} else {
			priv->dp_pkts_to_ppe_fail++;
			return;
		}
	}
#if 0
	pktlen = skb->len;
	skb->dev = g_loop_eth_dev[id];
	skb->protocol = eth_type_trans(skb, g_loop_eth_dev[id]);

	if (netif_rx(skb) == NET_RX_DROP)
		priv->stats.rx_dropped++;
	else {
		priv->stats.rx_packets++;
		priv->stats.rx_bytes += pktlen;
	}

#else
	dbg("Drop packet since loopeth not registered yet to dp\n");
	dev_kfree_skb_any(skb);
	priv->stats.rx_packets++;
	priv->stats.rx_bytes += pktlen;
	return;
#endif
}

static void do_loop_eth_rx_tasklet(unsigned long id)
{
	struct loop_eth_priv_data *priv = netdev_priv(g_loop_eth_dev[id]);
	struct sk_buff *skb;
	int i = 0;

	if ((id >= ARRAY_SIZE(g_loop_eth_dev))) {
		PR_ERR("Wrong id(%ld) in do_loop_eth_rx_tasklet\n", id);
		return;
	}

	while (1) {
		if (i >= TASKLET_HANDLE_BUDGET) {
			tasklet_schedule(&priv->rx_tasklet);
			break;
		}

		skb = skb_dequeue(&priv->rx_queue);

		if (!skb)
			break;

		netif_wake_queue(g_loop_eth_dev[id]);
		dbg("dequeue one skb\n");

		if (eth_rx_preprocess(skb, (int)id))
			eth_rx_handler(skb, (int)id);
		else {
			priv->rx_preprocess_drop++;
			dev_kfree_skb_any(skb);
			dbg("Drop for eth_rx_preprocess failed\n");
		}

		i++;
	}
}

static int32_t dp_fp_stop_tx(struct net_device *netif)
{
	return 0;
}

static int32_t dp_fp_restart_tx(struct net_device *netif)
{
	return 0;
}

static int32_t dp_fp_rx(struct net_device *rxif, struct net_device *txif,
			struct sk_buff *skb, int32_t len)
{
	struct loop_eth_priv_data *priv;
	int pktlen;

	skb_pull(skb, 8);	/*remove pmac header*/

	if (g_dbg_enable & DBG_ENABLE_MASK_DEBUG_PRINT)
		dp_dump_raw_data(skb->data, 20, "dp_fp_rx raw data");

	if (rxif) {
		dbg("dp_fp_rx to stack via %s\n", rxif->name);

		if (netif_running(rxif)) {
			priv = netdev_priv(rxif);
			pktlen = skb->len;
			skb->dev = rxif;
			skb->protocol = eth_type_trans(skb, rxif);

			if (netif_rx(skb) == NET_RX_DROP)
				priv->stats.rx_dropped++;
			else {
				priv->stats.rx_packets++;
				priv->stats.rx_bytes += pktlen;
			}

			priv->dp_pkts_from_ppe++;
			return 0;
		}
	} else if (txif) {
		dbg("dp_fp_rx to loopeth_xmit via %s for specified ep\n",
		    rxif->name);
		priv = netdev_priv(txif);
		skb->dev = txif;
		dev_queue_xmit(skb);
		priv->dp_pkts_tx++;
		return 0;
	}

	dev_kfree_skb_any(skb);
	return 0;
}

#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
static void proc_read_dbg(struct seq_file *s)
{
	int i;

	SEQ_PRINTF(s, "g_dbg_enable=0x%08x\n. \tEnabled Flags:",
		   g_dbg_enable);

	for (i = 0; i < ARRAY_SIZE(dbg_enable_mask_str); i++)
		if ((g_dbg_enable & (1 << i)))
			SEQ_PRINTF(s, "%s ", dbg_enable_mask_str[i]);

	SEQ_PRINTF(s, "\n");
}

static int proc_write_dbg(struct file *file, const char *buf, size_t count,
			  loff_t *ppos)
{
	char str[100];
	int len, rlen;
	int f_enable = 0;
	int i, j;
	int num;
	char *param_list[30];

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;

	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (num < 1)
		goto help;

	if (dp_strcmpi(param_list[0], "enable") == 0)
		f_enable = 1;
	else if (dp_strcmpi(param_list[0], "disable") == 0)
		f_enable = -1;
	else
		goto help;

	if (!param_list[1])
		set_ltq_dbg_flag(g_dbg_enable, f_enable, -1);
	else {
		for (i = 1; i < num; i++) {
			for (j = 0; j < ARRAY_SIZE(dbg_enable_mask_str); j++) {
				if (dp_strcmpi
				    (param_list[i],
				     dbg_enable_mask_str[j]) == 0) {
					set_ltq_dbg_flag(g_dbg_enable,
							 f_enable, (1 << j));

					break;
				}
			}
		}
	}

	return count;
 help:
	PR_INFO("echo <enable/disable> [");

	for (i = 0; i < ARRAY_SIZE(dbg_enable_mask_str); i++) {
		if (i == 0)
			PR_INFO("%s", dbg_enable_mask_str[i]);
		else
			PR_INFO("/%s", dbg_enable_mask_str[i]);
	}

	PR_INFO("] > /proc/loopeth/dbg\n");
	return count;
}
#endif

static int proc_read_dev(struct seq_file *s, int pos)
{
	if (g_loop_eth_dev[pos])
		SEQ_PRINTF(s, "  %s\n", g_loop_eth_dev[pos]->name);

	pos++;

	if (pos >= MAX_LOOPETH_NUM)
		pos = -1;

	return pos;
}

static int unregister_dev(int i)
{
	struct loop_eth_priv_data *priv;
	struct pmac_port_info *port_info;
	int res;

	if (!g_loop_eth_dev[i])
		return -1;

	priv = netdev_priv(g_loop_eth_dev[i]);

	if (priv->dp_subif.port_id == 0)
		return 0;

	if (priv->f_dp == LOOPETH_F_REGISTER_SUBIF) {
		res =
		    dp_register_subif(priv->owner, g_loop_eth_dev[i],
				      g_loop_eth_dev[i]->name,
				      &priv->dp_subif, DP_F_DEREGISTER);

		if (res != DP_SUCCESS)
			PR_ERR
			    ("dp_register_subif failed for port %d subif %d\n",
			     priv->dp_subif.port_id, priv->dp_subif.subif);

		priv->f_dp = LOOPETH_F_REGISTER_DEV;
		priv->dp_subif.subif = -1;
	}

	port_info = get_port_info_via_dp_port(priv->dp_subif.port_id);

	if (!port_info) {
		PR_ERR
		    ("get_port_info_via_dp_port failed for %s with specified port %d subif=%d\n",
		     g_loop_eth_dev[i]->name, priv->dp_subif.port_id,
		     priv->dp_subif.subif);
		return -1;
	}

	if (port_info->num_subif == 0) {
		if (priv->f_dp == LOOPETH_F_REGISTER_DEV) {
			if (dp_register_dev
			    (priv->owner, priv->dp_subif.port_id, NULL,
			     DP_F_DEREGISTER) != DP_SUCCESS)
				PR_ERR("dp_unregister_dev failed \
					for %s with port_id/subif %d/%d\n", g_loop_eth_dev[i]->name, priv->dp_subif.port_id, priv->dp_subif.subif);

			if (dp_alloc_port
			    (priv->owner, g_loop_eth_dev[i], i,
			     priv->dp_subif.port_id, NULL,
			     DP_F_DEREGISTER) != DP_SUCCESS) {
				PR_ERR("dp_dealloc_port failed \
					for %s with port_id/subif %d/%d\n", g_loop_eth_dev[i]->name, priv->dp_subif.port_id, priv->dp_subif.subif);
			}

			priv->dp_subif.port_id = -1;
		}

		priv->f_dp = LOOPETH_F_FREE;
	}

	return 0;
}

static int delete_loopeth_dev(int i)
{
	struct loop_eth_priv_data *priv;

	if (!g_loop_eth_dev[i])
		return -1;

	priv = netdev_priv(g_loop_eth_dev[i]);

	unregister_dev(i);

	/*unregister loopeth dev itself */
	unregister_netdev(g_loop_eth_dev[i]);
	free_netdev(g_loop_eth_dev[i]);
	g_loop_eth_dev[i] = NULL;
	return 0;
}

int create_loopeth_dev(int i)
{
	char ifname[IFNAMSIZ];
	struct loop_eth_priv_data *priv;

	if (g_loop_eth_dev[i]) {
		PR_ERR("g_loop_eth_dev[%d] already exist\n", i);
		return 0;
	}

	snprintf(ifname, sizeof(ifname), "loopeth%d", i);

	g_loop_eth_dev[i] =
	    alloc_netdev(sizeof(struct loop_eth_priv_data), ifname,
			 eth_setup);

	if (g_loop_eth_dev[i] == NULL) {
		PR_ERR("alloc_netdev fail\n");
		return -1;
	}

	g_loop_eth_dev[i]->dev_addr[0] = 0x00;
	g_loop_eth_dev[i]->dev_addr[1] = 0x00;
	g_loop_eth_dev[i]->dev_addr[2] = 0x00;
	g_loop_eth_dev[i]->dev_addr[3] = 0x00;
	g_loop_eth_dev[i]->dev_addr[4] = 0x00;
	g_loop_eth_dev[i]->dev_addr[5] = i;
	priv = netdev_priv(g_loop_eth_dev[i]);
	priv->id = i;
	tasklet_init(&priv->rx_tasklet, do_loop_eth_rx_tasklet, i);

	if (register_netdev(g_loop_eth_dev[i])) {
		free_netdev(g_loop_eth_dev[i]);
		g_loop_eth_dev[i] = NULL;
		PR_INFO("register device \"%s\" fail ??\n", ifname);
	} else {
		PR_INFO("add \"%s\" successfully\n", ifname);
		priv = netdev_priv(g_loop_eth_dev[i]);
		priv->f_dp = LOOPETH_F_FREE;
	}

	return 0;
}

static int proc_write_dev(struct file *file, const char *buf, size_t count,
			  loff_t *ppos)
{
	char str[100];
	int len, rlen;
	char *param_list[10];
	int num;
	int i;

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;

	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (num < 1)
		goto help;

	if (dp_strcmpi(param_list[0], "add") == 0) {
		if (param_list[1]) {
			i = dp_atoi(param_list[1]);

			if ((i < 0) || (i >= ARRAY_SIZE(g_loop_eth_dev))) {
				PR_ERR("Wrong index value: %d\n", i);
				return count;
			}

			if (g_loop_eth_dev[i]) {
				PR_ERR
				    ("interface index %d already exist and no need to create it\n",
				     i);
				return count;
			}

			/*create one dev */
			create_loopeth_dev(i);
		} else
			for (i = 0; i < ARRAY_SIZE(g_loop_eth_dev); i++) {
				/*create all dev if not created yet */
				if (g_loop_eth_dev[i])
					continue;

				create_loopeth_dev(i);
			}
	} else if (dp_strcmpi(param_list[0], "del") == 0) {
		if (param_list[1]) {
			for (i = 0; i < ARRAY_SIZE(g_loop_eth_dev); i++) {
				if (g_loop_eth_dev[i] &&
				    (dp_strcmpi
				     (g_loop_eth_dev[i]->name,
				      param_list[1]) == 0)) {
					delete_loopeth_dev(i);
					break;
				}
			}
		} else {
			for (i = 0; i < ARRAY_SIZE(g_loop_eth_dev); i++)
				delete_loopeth_dev(i);
		}
	} else {
		PR_ERR("Wrong command: %s\n", param_list[0]);
		goto help;
	}

	return count;

 help:
	PR_INFO("echo add [index] /proc/dp/loop/dev\n");
	PR_INFO("   example: echo add 1 > /proc/dp/loop/dev\n");
	PR_INFO("   example: echo add    > /proc/dp/loop/dev\n");
	PR_INFO("        Note, the maximum index is %d\n",
		ARRAY_SIZE(g_loop_eth_dev));
	PR_INFO("echo <del> [device name] > /proc/dp/loop/dev\n");
	PR_INFO("   example: echo del loopeth1 > /proc/dp/loop/dev\n");
	PR_INFO("   example: echo del          > /proc/dp/loop/dev\n");

	return count;
}

static int proc_read_mib(struct seq_file *s, int pos)
{
	struct loop_eth_priv_data *priv;

	if (g_loop_eth_dev[pos]) {
		priv = netdev_priv(g_loop_eth_dev[pos]);
		SEQ_PRINTF(s, "  %s:\n", g_loop_eth_dev[pos]->name);
		SEQ_PRINTF(s, "    rx_packets: %lu\n",
			   priv->stats.rx_packets);
		SEQ_PRINTF(s, "    rx_bytes:   %lu\n", priv->stats.rx_bytes);
		SEQ_PRINTF(s, "    rx_errors:  %lu\n", priv->stats.rx_errors);
		SEQ_PRINTF(s, "    rx_dropped: %lu\n",
			   priv->stats.rx_dropped);
		SEQ_PRINTF(s, "    tx_packets: %lu\n",
			   priv->stats.tx_packets);
		SEQ_PRINTF(s, "    tx_bytes:   %lu\n", priv->stats.tx_bytes);
		SEQ_PRINTF(s, "    tx_errors:  %lu\n", priv->stats.tx_errors);
		SEQ_PRINTF(s, "    tx_dropped: %lu\n",
			   priv->stats.tx_dropped);
		SEQ_PRINTF(s, "    rx_preprocess_drop:  %u\n",
			   priv->rx_preprocess_drop);
		SEQ_PRINTF(s, "    dp_pkts_to_ppe:      %u\n",
			   priv->dp_pkts_to_ppe);
		SEQ_PRINTF(s, "    dp_pkts_to_ppe_fail: %u\n",
			   priv->dp_pkts_to_ppe_fail);
		SEQ_PRINTF(s, "    dp_pkts_from_ppe:    %u\n",
			   priv->dp_pkts_from_ppe);
		SEQ_PRINTF(s, "    dp_pkts_tx:          %u\n",
			   priv->dp_pkts_tx);
	}

	pos++;

	if (pos >= MAX_LOOPETH_NUM)
		pos = -1;

	return pos;
}

void clear_mib(int i)
{
	struct loop_eth_priv_data *priv;

	priv = netdev_priv(g_loop_eth_dev[i]);
	memset(&priv->stats, 0, sizeof(priv->stats));
	priv->rx_preprocess_drop = 0;
	priv->dp_pkts_to_ppe = 0;
	priv->dp_pkts_to_ppe_fail = 0;
	priv->dp_pkts_from_ppe = 0;
	priv->dp_pkts_tx = 0;
}

static int proc_write_mib(struct file *file, const char *buf, size_t count,
			  loff_t *ppos)
{
	char str[100];
	int len, rlen;
	int i;
	int num;
	char *param_list[4];

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (num < 2)
		goto help;

	if (dp_strcmpi(param_list[0], "clear") != 0) {
		PR_ERR("Wrong command:%s\n", param_list[0]);
		goto help;
	}

	if ((dp_strcmpi(param_list[1], "all") != 0)) {
		for (i = 0; i < ARRAY_SIZE(g_loop_eth_dev); i++) {
			if (g_loop_eth_dev[i] &&
			    (dp_strcmpi
			     (g_loop_eth_dev[i]->name, param_list[1]) == 0)) {
				clear_mib(i);
				break;
			}
		}

		if (i >= ARRAY_SIZE(g_loop_eth_dev))
			PR_ERR("not found device %s\n", param_list[1]);
	} else {
		for (i = 0; i < ARRAY_SIZE(g_loop_eth_dev); i++)
			clear_mib(i);
	}

	return count;
 help:
	PR_INFO("echo <clear> [all/device name] > /proc/loopeth/mib\n");
	return count;
}

static int proc_read_dp(struct seq_file *s, int pos)
{
	struct loop_eth_priv_data *priv;

	if (g_loop_eth_dev[pos]) {
		priv = netdev_priv(g_loop_eth_dev[pos]);

		if (priv->dp_subif.port_id >= 0) {
			SEQ_PRINTF(s,
				   "%s - directpath on (ifid %d subif %d)\n",
				   g_loop_eth_dev[pos]->name,
				   priv->dp_subif.port_id,
				   priv->dp_subif.subif);
		} else
			SEQ_PRINTF(s, "%s - directpath off\n",
				   g_loop_eth_dev[pos]->name);
	}

	pos++;

	if (pos >= MAX_LOOPETH_NUM)
		pos = -1;

	return pos;
}

void unregister_from_dp(struct net_device *dev)
{
	struct loop_eth_priv_data *priv;
	int32_t dp_port_id;
	int i;

	priv = netdev_priv(dev);
	dp_port_id = priv->dp_subif.port_id;

	if (dp_port_id <= 0) {
		PR_ERR
		    ("Cannot undregister %s since it is not reigstered yet\n",
		     dev->name);
		return;
	}

	/*unregister all subif with same port_id first */
	for (i = 0; i < ARRAY_SIZE(g_loop_eth_dev); i++) {
		if (g_loop_eth_dev[i]) {
			struct loop_eth_priv_data *priv =
			    netdev_priv(g_loop_eth_dev[i]);

			if (priv->dp_subif.port_id != dp_port_id)
				continue;

			if (priv->f_dp != LOOPETH_F_REGISTER_SUBIF)
				continue;

			if (dp_register_subif
			    (priv->owner, g_loop_eth_dev[i],
			     g_loop_eth_dev[i]->name, &priv->dp_subif,
			     0) != DP_SUCCESS) {
				PR_ERR("dp_unregister_subif failed for %s \
					with port_id/subif %d/%d ?\n", dev->name, priv->dp_subif.port_id, priv->dp_subif.subif);
			}

			priv->f_dp = LOOPETH_F_REGISTER_DEV;
			priv->dp_subif.subif = 0;
		}
	}

	/*unregister/deallocate devices and reset all devices with same port_id */
	if (dp_register_dev(priv->owner, dp_port_id, NULL, DP_F_DEREGISTER)
	    != DP_SUCCESS) {
		PR_INFO("dp_unregister_dev failed for %s \
			with port_id %d ??? \n", dev->name, dp_port_id);
	}

	if (dp_alloc_port
	    (priv->owner, dev, priv->dev_port, dp_port_id, NULL,
	     DP_F_DEREGISTER) != DP_SUCCESS) {
		PR_INFO("dp_dealloc_port failed for %s with port_id %d \n",
			dev->name, dp_port_id);
	}

	for (i = 0; i < ARRAY_SIZE(g_loop_eth_dev); i++) {
		if (g_loop_eth_dev[i]) {
			struct loop_eth_priv_data *priv =
			    netdev_priv(g_loop_eth_dev[i]);

			if (priv->dp_subif.port_id != dp_port_id)
				continue;

			priv->f_dp = LOOPETH_F_FREE;
			priv->dp_subif.port_id = 0;
		}
	}
}

/*to find out the device type index via its flag name. for example eth_lan, eth_wan,....
return value:
-1: not found
>=0: type index
*/
int get_dev_type_index(char *flag_name)
{
	int i;

	for (i = 1; i < get_dp_port_type_str_size(); i++) {	/*skip i = 0 */
		if (dp_strcmpi(flag_name, dp_port_type_str[i]) == 0) {
			return i;
			;
		}
	}

	return -1;
}

static int proc_write_directpath(struct file *file, const char *buf,
				 size_t count, loff_t *ppos)
{
	char str[300];
	int len, rlen;
	char *ifname = NULL;
	dp_cb_t cb = {
		0
	};
	char *param_list[10] = {
		NULL
	};
	int param_list_num = 0;
	struct loop_eth_priv_data *priv = NULL;
	int i, k;
	int32_t dp_port_id = 0;
	uint32_t dev_port = 0;
	int flag_index = 0;
	char *flag_str = NULL;
	char *dev_port_str = NULL;
	char *dp_port_str = NULL;
	struct pmac_port_info *port_info = NULL;

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	param_list_num =
	    dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));

	if (param_list_num < 3)
		goto help;

	ifname = param_list[2];
	/*device must ready before register to Datapath */
	i = find_loopeth_index_via_name(ifname);
	if (i < 0) {
		PR_ERR("Not found device %s in loopeth \n", ifname);
		goto exit;
	}

	priv = netdev_priv(g_loop_eth_dev[i]);

	if (((dp_strcmpi(param_list[0], "register") == 0) ||
	     (dp_strcmpi(param_list[0], "reg") == 0)) &&
	    (dp_strcmpi(param_list[1], "dev") == 0)) {
		if (param_list_num < 5)
			goto help;

		PR_INFO("Try to register dev %s to datapath\n", ifname);
		dev_port_str = param_list[3];
		flag_str = param_list[4];
		dp_port_str = param_list[5];
		PR_INFO("dev_port_str=%s\n",
			dev_port_str ? dev_port_str : "NULL");
		PR_INFO("flag_str=%s\n", flag_str ? flag_str : "NULL");
		PR_INFO("dp_port_str=%s\n",
			dp_port_str ? dp_port_str : "NULL");

		dev_port = dp_atoi(dev_port_str);
		flag_index = get_dev_type_index(flag_str);
		dp_port_id = dp_atoi(dp_port_str);

		if (flag_index <= 0) {
			PR_INFO("Not valid device type:%s(%d)\n", flag_str,
				flag_index);
			goto help;
		}

		priv->owner = &g_loop_eth_module[i];
		sprintf(priv->owner->name, "module%02d", i);
		dp_port_id =
		    dp_alloc_port(priv->owner, g_loop_eth_dev[i], dev_port,
				  dp_port_id, NULL, 1 << flag_index);
		g_loop_eth_dev_flag[i] = 1 << flag_index;

		if (dp_port_id <= 0) {
			PR_INFO("failed in register directpath for %s\n",
				ifname);
			goto exit;
		}

		PR_INFO("dp_alloc_port get port %d for %s\n", dp_port_id,
			ifname);
		cb.stop_fn = (dp_stop_tx_fn_t) dp_fp_stop_tx;
		cb.restart_fn = (dp_restart_tx_fn_t) dp_fp_restart_tx;
		cb.rx_fn = (dp_rx_fn_t) dp_fp_rx;
		priv->dp_subif.port_id = dp_port_id;
		priv->dev_port = dev_port;

		if (dp_register_dev(priv->owner, dp_port_id, &cb, 0) !=
		    DP_SUCCESS) {
			PR_INFO
			    ("dp_register_dev failed for %s\n and port_id %d",
			     ifname, dp_port_id);
			dp_alloc_port(priv->owner, g_loop_eth_dev[i],
				      dev_port, 0, NULL, DP_F_DEREGISTER);
			goto exit;
		}

		priv->f_dp = LOOPETH_F_REGISTER_DEV;
		PR_INFO("Succeed to register dev %s dev_port %d to \
			datapath with dp_port %d \n", ifname, dev_port, dp_port_id);
	} else if (((dp_strcmpi(param_list[0], "register") == 0)
		    || (dp_strcmpi(param_list[0], "reg") == 0))
		   && (dp_strcmpi(param_list[1], "subif") == 0)) {
		if (param_list_num < 3)
			goto help;

		PR_INFO("Try to register subif %s to datapath\n", ifname);

		if (priv->f_dp == LOOPETH_F_REGISTER_DEV) {	/*already alloc a port and registered dev */
			if (param_list[3])
				priv->dp_subif.subif = dp_atoi(param_list[3]);
			else
				priv->dp_subif.subif = -1;	/*dynamic */

			if (dp_register_subif
			    (priv->owner, g_loop_eth_dev[i],
			     g_loop_eth_dev[i]->name, &priv->dp_subif,
			     0) != DP_SUCCESS)
				goto exit;

			priv->f_dp = LOOPETH_F_REGISTER_SUBIF;
			PR_INFO("Succeed to register subitf %s dev port %d: \
				dp_port %d subif %d dp_flag=%d\n", ifname, priv->dev_port, priv->dp_subif.port_id, priv->dp_subif.subif, priv->f_dp);
		} else if (priv->f_dp == LOOPETH_F_FREE) {
			char *parent_dev_name = param_list[3];
			struct net_device *parent_dev;

			parent_dev =
			    dev_get_by_name(&init_net, parent_dev_name);

			if (!parent_dev) {
				PR_ERR("Not found device %s\n",
				       parent_dev_name);
				goto exit;
			}

			port_info = get_port_info_via_dp_name(parent_dev);

			if (!port_info) {
				PR_INFO("No such registered device %s yet\n",
					parent_dev_name);
				goto exit;
			}

			priv->dp_subif.subif = -1;	/*dynamic */
			priv->dp_subif.port_id = port_info->port_id;
			priv->dev_port = port_info->dev_port;
			priv->owner = port_info->owner;

			if (dp_register_subif
			    (priv->owner, port_info->dev,
			     g_loop_eth_dev[i]->name, &priv->dp_subif,
			     0) != DP_SUCCESS) {
				PR_INFO
				    ("dp_register_subif failed for %s to register under %s\n",
				     g_loop_eth_dev[i]->name,
				     parent_dev_name);
				goto exit;
			}

			priv->f_dp = LOOPETH_F_REGISTER_SUBIF;
			PR_INFO
			    ("Succeed to register subitf %s dev port %d:dp_port %d subif %d\n",
			     ifname, priv->dev_port, priv->dp_subif.port_id,
			     priv->dp_subif.subif);
		} else if (priv->f_dp == LOOPETH_F_REGISTER_SUBIF) {
			PR_INFO("Subif %s already registered\n", ifname);
			goto exit;
		} else {
			PR_INFO("Failed for uknown reason:%d\n", priv->f_dp);
			goto exit;
		}
	} else
	    if (((dp_strcmpi(param_list[0], "unregister") == 0) ||
		 (dp_strcmpi(param_list[0], "unreg") == 0)) &&
		(dp_strcmpi(param_list[1], "dev") == 0)) {
		PR_INFO("Try to register dev %s from datapath\n", ifname);
		priv = netdev_priv(g_loop_eth_dev[i]);
		dp_port_id = priv->dp_subif.port_id;

		for (k = 0; k < ARRAY_SIZE(g_loop_eth_dev); k++) {
			if (g_loop_eth_dev[k]) {	/*unregister all devices with same port_id */
				priv = netdev_priv(g_loop_eth_dev[k]);

				if (priv->dp_subif.port_id != dp_port_id)
					continue;

				unregister_dev(k);
			}
		}
	} else {
		PR_INFO("Wrong command: %s %s\n", param_list[0],
			param_list[1]);
		goto help;
	}

 exit:
	return count;
 help:
	/*     param_list[0]    [1]    [2]         [3]      [4]     [5]            */
	PR_INFO
	    ("echo register   dev   <Dev_name> <Dev_port> <Type> [DP_port] > /proc/loopeth/directpath\n");
	PR_INFO
	    ("echo unregister dev  <Dev_name>  > /proc/loopeth/directpath\n");
	PR_INFO
	    ("echo register   subif <Dev_name> [parent_dev_name] > /proc/loopeth/directpath\n");
	PR_INFO("Note: parent_dev_name is for register different subif\n");
	PR_INFO("Device Type:\n");

	for (i = 1; i < get_dp_port_type_str_size(); i++)	/*skip i 0 */
		PR_INFO("\t%s\n", dp_port_type_str[i]);

	PR_INFO("----16 subifs test script-----\n");
	PRINTK("\n \
	echo add 0 > /proc/dp/loop/dev\n \
	echo register dev   loopeth0 1 DIRECTPATH > /proc/dp/loop/directpath\n \
	echo register subif loopeth0 > /proc/dp/loop/directpath\n \
	\n \
	for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 \n \
		do \n \
		echo add $i > /proc/dp/loop/dev\n \
		echo register subif loopeth${i} loopeth0 > /proc/dp/loop/directpath\n \
	done\n");

	return count;
}

/*###################################
*           Global Function
* ####################################
*/
#define PROC_NAME		"loop"
#define PROC_DBG		"dbg"
#define PROC_READ_DEV		"dev"
#define PROC_READ_MIB		"mib"
#define PROC_READ_DP		"directpath"
#define PROC_READ_CPUTX		"cputx"
static struct dp_proc_entry proc_entries[] = {
	/* name             single_callback_t    multi_callback_t    multi_callback_start    write_callback_t  */
#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
	{PROC_DBG, proc_read_dbg, NULL, NULL, proc_write_dbg},
#endif
	{PROC_READ_DEV, NULL, proc_read_dev, NULL, proc_write_dev},
	{PROC_READ_MIB, NULL, proc_read_mib, NULL, proc_write_mib},
	{PROC_READ_DP, NULL, proc_read_dp, NULL, proc_write_directpath},

	/*last one for place holder */
	{NULL, NULL, NULL, NULL, NULL}
};

static struct proc_dir_entry *proc_node;

static struct proc_dir_entry *proc_file_create(struct proc_dir_entry
					       *parent)
{
	proc_node = proc_mkdir(PROC_NAME, parent);

	if (proc_node != NULL) {
		int i;

		for (i = 0; i < ARRAY_SIZE(proc_entries); i++)
			dp_proc_entry_create(proc_node, &proc_entries[i]);
	} else {
		PR_INFO("cannot create proc entry");
		return NULL;
	}

	return proc_node;
}

/*####################################
*           Init/Cleanup API
* ####################################
*/

int dp_loop_eth_dev_init(struct proc_dir_entry *parent)
{
	PR_INFO("Loading loop_net_dev driver ...... ");
	memset(g_loop_eth_dev, 0, sizeof(g_loop_eth_dev));
	memset(g_loop_eth_module, 0, sizeof(g_loop_eth_module));

	proc_file_create(parent);

	PR_INFO("dp_loop_eth_dev_init Succeeded!\n");
	return 0;
}

void dp_loop_eth_dev_exit(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(g_loop_eth_dev); i++)
		if (g_loop_eth_dev[i]) {
			delete_loopeth_dev(i);
			free_netdev(g_loop_eth_dev[i]);
		}
}
