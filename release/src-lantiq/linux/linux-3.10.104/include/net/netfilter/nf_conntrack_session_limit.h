
#ifndef CONNTRACK_SESSION_START_BIT_POS
#define CONNTRACK_SESSION_START_BIT_POS 10
#define CONNTRACK_SESSION_NO_OF_BITS 3
#define CONNTRACK_SESSION_MASK 0x00001C00
#endif

extern int nf_conntrack_low_prio_max;
extern unsigned int nf_conntrack_low_prio_thresh;
extern int nf_conntrack_default_prio_max;
extern unsigned int nf_conntrack_default_prio_thresh;
extern unsigned int nf_conntrack_low_prio_data_rate;
extern unsigned int nf_conntrack_default_prio_data_rate;
extern unsigned int nf_conntrack_session_limit_enable;
extern unsigned int nf_conntrack_tcp_steady_offset;
extern unsigned int nf_conntrack_tcp_initial_offset;
extern unsigned int nf_conntrack_sessionmgmt_add_time;

extern unsigned int nf_ct_check_reject_low_prio_conntrack(struct net *net,unsigned int hook,struct sk_buff *skb,int ret);
extern unsigned int do_session_limit(unsigned int hook,struct sk_buff *skb,const struct net_device *in,const struct net_device *out);
extern __u32 skb_get_session_priority(struct sk_buff *skb);
