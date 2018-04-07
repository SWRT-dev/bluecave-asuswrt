/*
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 */
#include <linux/export.h>
#include <net/ipv6.h>

int (*ipv6_dev_get_saddr_hook)(struct net *net, const struct net_device *dev,
			const struct in6_addr *daddr, unsigned int prefs,
			struct in6_addr *saddr);

EXPORT_SYMBOL(ipv6_dev_get_saddr_hook);

int ipv6_dev_get_saddr(struct net *net, const struct net_device *dst_dev,
			const struct in6_addr *daddr, unsigned int prefs,
			struct in6_addr *saddr)
{
	int ret = -EADDRNOTAVAIL;
	typeof(ipv6_dev_get_saddr_hook) dev_get_saddr;

	rcu_read_lock();
	dev_get_saddr = rcu_dereference(ipv6_dev_get_saddr_hook);

	if (dev_get_saddr)
		ret = dev_get_saddr(net, dst_dev, daddr, prefs, saddr);

	rcu_read_unlock();
	return ret;
}
EXPORT_SYMBOL(ipv6_dev_get_saddr);

