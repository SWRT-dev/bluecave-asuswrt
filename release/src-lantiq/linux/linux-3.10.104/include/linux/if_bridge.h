/*
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
#ifndef _LINUX_IF_BRIDGE_H
#define _LINUX_IF_BRIDGE_H


#include <linux/netdevice.h>
#include <uapi/linux/if_bridge.h>

extern void brioctl_set(int (*ioctl_hook)(struct net *, unsigned int, void __user *));

typedef int br_should_route_hook_t(struct sk_buff *skb);
extern br_should_route_hook_t __rcu *br_should_route_hook;

#ifdef CONFIG_WAVE_WIFI_PROXYARP
typedef int br_do_wave_wifi_proxyarp_hook_t(struct sk_buff *skb, const struct net_device *in, const struct net_device *out);
extern br_do_wave_wifi_proxyarp_hook_t *br_out_do_wave_wifi_proxyarp_hook;
extern br_do_wave_wifi_proxyarp_hook_t *br_pre_do_wave_wifi_proxyarp_hook;
extern br_do_wave_wifi_proxyarp_hook_t *br_fwd_do_wave_wifi_proxyarp_hook;
#endif

#endif
