#ifndef MPE_FW_FAST_HOOK_TEST_H
#define MPE_FW_FAST_HOOK_TEST_H

#include <generated/autoconf.h>

enum LTQ_MPE_FASTHOOK_SKB_FLAG {
	ACC_HOOK_RX,/*hook point: device rx hook */
	ACC_HOOK_NETFILTER,/*hook point: netfilter hook */
	ACC_HOOK_TX	/*hook point: device tx hook */
};

enum LTQ_MPE_FASTHOOK_SKB_ACT {
	ACC_HOOK_CONTINUE,/*continue next hook */
	ACC_HOOK_DONE /* current packet is soft ware accelerated alreadys */
};

enum LTQ_MPE_FASTHOOK_RETURN_VALUE {
	PKT_CONTINUE,/*continue next hook */
	PKT_CONSUMED,
	PKT_DONE/* current packet is soft ware accelerated alreadys */
};

/* Note, since there is multiple accelertion engineer with different capability,
   suggest to use MACRO in the hook for different learning, otherwise, it needs to check MPE HAL layer and affects performace
 */
enum LTQ_MPE_FASTHOOK_SKB_ACC_RES_FLAG {
	ACC_UNKNOWN = 0,/* Don't know whether can be accelerated or not */
	ACC_CANNOT = 1,/* Cannot be acclerated by any accelerator engineer */
	ACC_DONOT_ = 2,/* Can be accelerated, but Dont do accleration for Session management purpose, or for session relearn, esp for tos,... */
	ACC_SOFTWARE_OK = 4,/* Can be accelerated by software acceleration */
	ACC_SWITCH_OK = 8,/* Can be accelerated by Switch Accelerator, mainly for future URX500 */
	ACC_MPE_OK = 16,/* Can be accelerated by MPE accelerator, mainly for future URX500 */
	ACC_PAE_OK = 32,/* Can be accelerated by PPE FW, for existing product, like AR10, VE9,.... */

};

typedef union {
	u32 ip; /*!< the storage buffer for ipv4 */
#ifdef CONFIG_IPV6
	u32 ip6[4];/*!< the storage buffer for ipv6 */
#endif
} LTQ_MPE_FASTHOOK_IPADDR;

#define MAX_FASTHOOK_PHYDEV 10

struct ltq_mpe_fasthook_session_info {
	/* buffer for parsing skb during rx_hook and tx_hook */
	u32 vlan_num: 2; /* for vlan header number: for both rx/tx_hook */
	u32 ppp_flag: 1; /* pppoe header: for both rx/tx_hook */

	u32 dslite_flag: 1; /*dslite header flag: for both rx/tx_hook */
	u32 rd_flag: 1;
	u32 gre_flag: 1;
	u32 route_flag: 1; /* 1 means it is routing packet */
	u32 tcp_flag: 1; /* 1 means tcp packet */
	u32 tcp_establish_flag: 1; /* 1 means tcp fully estabilished */
	u32 multicast_flag: 1;
	u32 l2tp_flag: 1;
	u32 l2tp_controlmsg_flag: 1;
	u8 ip_proto: 8;

	LTQ_MPE_FASTHOOK_IPADDR src_ip, dst_ip;
	u16 src_port, dst_port;
	u16 tos;

	/* buffer for parsing skb during tx_hook */
	u32 inner_vid;/* inner_vid: for tx_hook only */
	u32 outer_vid;/* outer_vid: for tx_hook only */
	u32 pppheader_offset;
	u32 dsliteheader_offset;
	u32 rdheader_offset;
	u32 inneripheader_offset;
	u32 greheader_offset;
	u32 grekey_en;
	u32 previous_ipheadertype; /*Incase of gre and l2tp*/
	u32 eogre_inner_macheader_offset;/*Incase of gre*/
	u32 l2tpheader_offset; /*Incase of l2tp*/
	u32 l2tpheader_udp_offset; /*Incase of l2tp*/
	u32 l2tpversion; /*Incase of l2tp*/
	u16 pppoe_session_id;/* pppoe_session_id */
	u32 ttl;
	u8 src_mac[6], dst_mac[6];
	u8 ip_version;
	struct net_device *phydev[MAX_FASTHOOK_PHYDEV];

	/* Mainly for multicast or bridging learning */
	u8 tx_count;/* how many times mpe_fasthook_info pass ltq_mpe_fasthook_info for skb_clone/copy to different interface forwarding. Mainly for multicast learning or bridging learning */
	u8 referece_count;/* When it becomes zero, it means need to delte this structure memory. Also means need to start learn multicast/bridging destiniation interface list and its action */
};

#define FASTHOOK_KEY_SIZE  64
struct ltq_mpe_fasthook_info {
	struct ltq_mpe_fasthook_session_info *rx_info;
	struct ltq_mpe_fasthook_session_info *tx_info;
	struct ltq_mpe_fasthook_session_info *tmp_tx_info;
	char key[FASTHOOK_KEY_SIZE];

	/*learning decision */
	u32 acc_hook_point: 2; /* Refer to enum LTQ_MPE_FASTHOOK_SKB_HOOK_FLAG */
	u32 acc_hook_action: 2; /* Need skip next hooks or not */
	u8 acc_learn_result;/* Multiple bit may be set  */
};

extern int (*ltq_mpe_fasthook_free_fn) (struct sk_buff *);
extern int (*ltq_mpe_fasthook_tx_fn) (struct sk_buff *, u32 , void *);
extern int (*ltq_mpe_fasthook_rx_fn) (struct sk_buff *, u32 , void *);

#endif				/*MPE_FW_FAST_HOOK_TEST_H */
