/*
 *	Forwarding decision
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/err.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/netpoll.h>
#include <linux/skbuff.h>
#include <linux/if_vlan.h>
#include <linux/netfilter_bridge.h>

#ifdef CONFIG_LTQ_IPQOS_BRIDGE_EBT_IMQ
#include <linux/netfilter_bridge/ebt_imq.h>
extern int nf_ebt_queue(struct sk_buff *skb);
#endif

#include "br_private.h"

#if defined(CONFIG_LTQ_PPA_API) || defined(CONFIG_LTQ_PPA_API_MODULE)
  #include <net/ppa_api.h>
#endif

#ifdef CONFIG_LTQ_MCAST_SNOOPING
#define IS_MCAST_ADDR 0x1
#endif

#ifdef CONFIG_LANTIQ_MCAST_HELPER_MODULE
void (*five_tuple_br_info_ptr)(struct sk_buff *skb) = NULL;
EXPORT_SYMBOL(five_tuple_br_info_ptr);
int mch_br_capture_pkt=0;
EXPORT_SYMBOL(mch_br_capture_pkt);
#endif

#ifdef CONFIG_WAVE_WIFI_PROXYARP
br_do_wave_wifi_proxyarp_hook_t *br_out_do_wave_wifi_proxyarp_hook;
EXPORT_SYMBOL(br_out_do_wave_wifi_proxyarp_hook);
br_do_wave_wifi_proxyarp_hook_t *br_fwd_do_wave_wifi_proxyarp_hook;
EXPORT_SYMBOL(br_fwd_do_wave_wifi_proxyarp_hook);
#endif

static int deliver_clone(const struct net_bridge_port *prev,
			 struct sk_buff *skb,
			 void (*__packet_hook)(const struct net_bridge_port *p,
					       struct sk_buff *skb));

/* Don't forward packets to originating port or forwarding diasabled */
static inline int should_deliver(const struct net_bridge_port *p,
				 const struct sk_buff *skb)
{
	return (((p->flags & BR_HAIRPIN_MODE) || skb->dev != p->dev) &&
		br_allowed_egress(p->br, nbp_get_vlan_info(p), skb) &&
		p->state == BR_STATE_FORWARDING);
}

static inline unsigned int packet_length(const struct sk_buff *skb)
{
	return skb->len - (skb->protocol == htons(ETH_P_8021Q) ? VLAN_HLEN : 0);
}

int __ebt_optimized br_dev_queue_push_xmit(struct sk_buff *skb)
{
	#ifdef CONFIG_LTQ_IPQOS_BRIDGE_EBT_IMQ
	int retval = -1;
	#endif
	/* ip_fragment doesn't copy the MAC header */
	if (nf_bridge_maybe_copy_header(skb) ||
	    (packet_length(skb) > skb->dev->mtu && !skb_is_gso(skb))) {
		kfree_skb(skb);
	} else {
		skb_push(skb, ETH_HLEN);
		br_drop_fake_rtable(skb);

#if (defined(CONFIG_LTQ_PPA_API) || defined(CONFIG_LTQ_PPA_API_MODULE)) && (defined( CONFIG_LTQ_BRIDGE_LEARNING))
		if ( ppa_hook_session_add_fn != NULL )
		{
			ppa_hook_session_add_fn(skb, 
					NULL, 
					PPA_F_BRIDGED_SESSION);
		}
#endif
		#ifdef CONFIG_LTQ_IPQOS_BRIDGE_EBT_IMQ
			if (skb->imq_flags & IMQ_F_ENQUEUE) {
				retval = nf_ebt_queue(skb);
			}
			if (retval < 0) {
		dev_queue_xmit(skb);
	}
		#else
			dev_queue_xmit(skb);
		#endif
	}

	return 0;
}

int __ebt_optimized br_forward_finish(struct sk_buff *skb)
{
#ifdef CONFIG_LANTIQ_IPQOS_MARK_SKBPRIO
	skb_mark_priority(skb);
#endif
	return BR_HOOK(NFPROTO_BRIDGE, NF_BR_POST_ROUTING, skb, NULL, skb->dev,
		       br_dev_queue_push_xmit);

}

static void __br_deliver(const struct net_bridge_port *to, struct sk_buff *skb)
{
	skb = br_handle_vlan(to->br, nbp_get_vlan_info(to), skb);
	if (!skb)
		return;

	skb->dev = to->dev;

	if (unlikely(netpoll_tx_running(to->br->dev))) {
		if (packet_length(skb) > skb->dev->mtu && !skb_is_gso(skb))
			kfree_skb(skb);
		else {
			skb_push(skb, ETH_HLEN);
			br_netpoll_send_skb(to, skb);
		}
		return;
	}

#ifdef CONFIG_WAVE_WIFI_PROXYARP
	if (br_out_do_wave_wifi_proxyarp_hook) {
		if (NF_DROP == br_out_do_wave_wifi_proxyarp_hook(skb, NULL, skb->dev)) {
			kfree_skb(skb);
			return;
		}
	}
#endif

	BR_HOOK(NFPROTO_BRIDGE, NF_BR_LOCAL_OUT, skb, NULL, skb->dev,
		br_forward_finish);
}

static void __ebt_optimized __br_forward(const struct net_bridge_port *to, struct sk_buff *skb)
{
	struct net_device *indev;

	if (skb_warn_if_lro(skb)) {
		kfree_skb(skb);
		return;
	}

	skb = br_handle_vlan(to->br, nbp_get_vlan_info(to), skb);
	if (!skb)
		return;

#ifdef CONFIG_LANTIQ_MCAST_HELPER_MODULE
	/* Send five tuple info to mcast helper */
	if(mch_br_capture_pkt == 1){
		if(ip_hdr(skb)->protocol == 17){
			if(five_tuple_br_info_ptr != NULL){
				five_tuple_br_info_ptr(skb);
			}
		}
	}
#endif

	indev = skb->dev;
	skb->dev = to->dev;
	skb_forward_csum(skb);

#ifdef CONFIG_WAVE_WIFI_PROXYARP
	if (br_fwd_do_wave_wifi_proxyarp_hook) {
		if (NF_DROP == br_fwd_do_wave_wifi_proxyarp_hook(skb, indev, skb->dev)) {
			kfree_skb(skb);
			return;
		}
	}
#endif

	BR_HOOK(NFPROTO_BRIDGE, NF_BR_FORWARD, skb, indev, skb->dev,
		br_forward_finish);
}

/* called with rcu_read_lock */
void br_deliver(const struct net_bridge_port *to, struct sk_buff *skb)
{
	if (to && should_deliver(to, skb)) {
		__br_deliver(to, skb);
		return;
	}

	kfree_skb(skb);
}

/* called with rcu_read_lock */
void __ebt_optimized br_forward(const struct net_bridge_port *to, struct sk_buff *skb, struct sk_buff *skb0)
{
	if (should_deliver(to, skb) && !(to->flags & BR_ISOLATE_MODE)) {
		if (skb0)
			deliver_clone(to, skb, __br_forward);
		else
			__br_forward(to, skb);
		return;
	}

	if (!skb0)
		kfree_skb(skb);
}

static int deliver_clone(const struct net_bridge_port *prev,
			 struct sk_buff *skb,
			 void (*__packet_hook)(const struct net_bridge_port *p,
					       struct sk_buff *skb))
{
	struct net_device *dev = BR_INPUT_SKB_CB(skb)->brdev;

	skb = skb_clone(skb, GFP_ATOMIC);
	if (!skb) {
		dev->stats.tx_dropped++;
		return -ENOMEM;
	}

	__packet_hook(prev, skb);
	return 0;
}

static struct net_bridge_port *maybe_deliver(
	struct net_bridge_port *prev, struct net_bridge_port *p,
	struct sk_buff *skb,
	void (*__packet_hook)(const struct net_bridge_port *p,
			      struct sk_buff *skb))
{
	int err;

	if (!should_deliver(p, skb))
		return prev;

	if (!prev)
		goto out;

#ifdef CONFIG_LTQ_MCAST_SNOOPING
		if ((bridge_igmp_snooping || bridge_mld_snooping) && 
			(eth_hdr(skb)->h_dest[0] & IS_MCAST_ADDR) && 
			(br_selective_flood(prev, skb) == 0)) {
				prev = p;
				return p;
		}
#endif
			


	err = deliver_clone(prev, skb, __packet_hook);
	if (err)
		return ERR_PTR(err);

out:
	return p;
}

/* called under bridge lock */
static void __ebt_optimized br_flood(struct net_bridge *br, struct sk_buff *skb,
		     struct sk_buff *skb0,
		     void (*__packet_hook)(const struct net_bridge_port *p,
					   struct sk_buff *skb),
		     bool forward)
{
	struct net_bridge_port *p;
	struct net_bridge_port *prev;

	prev = NULL;

	list_for_each_entry_rcu(p, &br->port_list, list) {
		if (forward && (p->flags & BR_ISOLATE_MODE))
			continue;

		prev = maybe_deliver(prev, p, skb, __packet_hook);
		if (IS_ERR(prev))
			goto out;
	}

	if (!prev)
		goto out;

	if (skb0)
	{
	#ifdef CONFIG_LTQ_MCAST_SNOOPING
		if ((bridge_igmp_snooping || bridge_mld_snooping) && 
			(eth_hdr(skb)->h_dest[0] & IS_MCAST_ADDR) && 
			(br_selective_flood(prev, skb) == 0)){}
//			kfree_skb(skb);
		else
	#endif
		deliver_clone(prev, skb, __packet_hook);
	}
	else
	{
#ifdef CONFIG_LTQ_MCAST_SNOOPING
		if ((bridge_igmp_snooping || bridge_mld_snooping) && 
			(eth_hdr(skb)->h_dest[0] & IS_MCAST_ADDR) && 
			(br_selective_flood(prev, skb) == 0))
			kfree_skb(skb);
		else
#endif	
		__packet_hook(prev, skb);
	}
	return;

out:
	if (!skb0)
		kfree_skb(skb);
}


/* called with rcu_read_lock */
void br_flood_deliver(struct net_bridge *br, struct sk_buff *skb)
{
	br_flood(br, skb, NULL, __br_deliver, false);
}

/* called under bridge lock */
void br_flood_forward(struct net_bridge *br, struct sk_buff *skb,
		      struct sk_buff *skb2)
{
	br_flood(br, skb, skb2, __br_forward, true);
}

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
/* called with rcu_read_lock */
static void br_multicast_flood(struct net_bridge_mdb_entry *mdst,
			       struct sk_buff *skb, struct sk_buff *skb0,
			       void (*__packet_hook)(
					const struct net_bridge_port *p,
					struct sk_buff *skb))
{
	struct net_device *dev = BR_INPUT_SKB_CB(skb)->brdev;
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_port *prev = NULL;
	struct net_bridge_port_group *p;
	struct hlist_node *rp;

	rp = rcu_dereference(hlist_first_rcu(&br->router_list));
	p = mdst ? rcu_dereference(mdst->ports) : NULL;
	while (p || rp) {
		struct net_bridge_port *port, *lport, *rport;

		lport = p ? p->port : NULL;
		rport = rp ? hlist_entry(rp, struct net_bridge_port, rlist) :
			     NULL;

		port = (unsigned long)lport > (unsigned long)rport ?
		       lport : rport;

		prev = maybe_deliver(prev, port, skb, __packet_hook);
		if (IS_ERR(prev))
			goto out;

		if ((unsigned long)lport >= (unsigned long)port)
			p = rcu_dereference(p->next);
		if ((unsigned long)rport >= (unsigned long)port)
			rp = rcu_dereference(hlist_next_rcu(rp));
	}

	if (!prev)
		goto out;

	if (skb0)
		deliver_clone(prev, skb, __packet_hook);
	else
		__packet_hook(prev, skb);
	return;

out:
	if (!skb0)
		kfree_skb(skb);
}

/* called with rcu_read_lock */
void br_multicast_deliver(struct net_bridge_mdb_entry *mdst,
			  struct sk_buff *skb)
{
	br_multicast_flood(mdst, skb, NULL, __br_deliver);
}

/* called with rcu_read_lock */
void br_multicast_forward(struct net_bridge_mdb_entry *mdst,
			  struct sk_buff *skb, struct sk_buff *skb2)
{
	br_multicast_flood(mdst, skb, skb2, __br_forward);
}
#endif
