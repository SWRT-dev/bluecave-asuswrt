#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/if.h>

#include <net/ppa_api.h>

#include "../stat_helper_platform.h"
#include "stat_helper_xrx300.h"

struct rtnl_link_stats64*
stat_helper_platform_dev_get_extended_stats64(struct net_device *dev,
									 struct rtnl_link_stats64 *storage)
{
	stat_helper_dev_stats64_t stats;
	PPA_NETIF_ACCEL_STATS ppa_stats;

	/* Loopback, or any other Unknown interface */
	if ( (dev->type == ARPHRD_LOOPBACK) || (dev->type == ARPHRD_NONE) || (dev->type == ARPHRD_VOID) ) {
		return storage;
	}

	/* Upper layer interface, such as PPP, Dslite, 6RD, VLAN, Bridge, MACVLAN etc. */
	if (_stat_helper_is_upper_layer_dev(dev)) {
		/* NOTE : Linux driver maintains only MIPS counters */

#if (defined(CONFIG_LTQ_PPA_API) || defined(CONFIG_LTQ_PPA_API_MODULE))
		/* FIXME : Currently packet counters are not available from PPA */
		/* FIXME : Some extra bytes are there depending on the interface layer */

		memset(&ppa_stats, 0, sizeof(PPA_NETIF_ACCEL_STATS));

		/* get PPA HW and SW accelerated session stats */
		if (ppa_hook_get_netif_accel_stats_fn) {
			ppa_hook_get_netif_accel_stats_fn(dev->name, &ppa_stats, (PPA_F_NETIF_HW_ACCEL | PPA_F_NETIF_SW_ACCEL));
		}

		/* add with ppa HW and SW accelerated stats */
		storage->rx_bytes += ppa_stats.hw_accel_stats.rx_bytes + ppa_stats.sw_accel_stats.rx_bytes;
		storage->tx_bytes += ppa_stats.hw_accel_stats.tx_bytes + ppa_stats.sw_accel_stats.tx_bytes;
#endif
	}

	/* Ethernet LAN interface, say eth0 */
	else if (_stat_helper_is_ethlan_basedev(dev)) {
		/* NOTE : Linux driver maintains MIPS and PPE FW counters */
		int32_t i;
		stat_helper_physport_list_t portlist = {0};

		/* Accumulate all the LAN ports switch counters */
		if (_stat_helper_get_all_lan_physport(&portlist)) {
			return storage;
		}

		memset(&stats, 0, sizeof(stat_helper_dev_stats64_t));

		/* get all the lan switch port stats */
		for (i = 0; i < portlist.port_num; i++) {
			_stat_helper_get_switch_rmon_counters(CONFIG_SWITCH_DEV, portlist.port[i], &stats);
		}

		storage->rx_packets = stats.rx_packets - storage->rx_dropped; /* NOTE : exclude switch port drops */
		storage->tx_packets = stats.tx_packets;
		/* FIXME : Here, switch accelerated counters are missing in storage->rx_bytes */
		storage->tx_bytes = stats.tx_bytes;
		storage->rx_errors += stats.rx_errors;
		storage->tx_errors += stats.tx_errors;
		storage->rx_dropped += stats.rx_dropped;
		storage->tx_dropped += stats.tx_dropped;

	/* Ethernet LAN port interface, such as eth0_1, eth0_2, eth0_3, eth0_4 and so on. */
	} else if (_stat_helper_is_ethlan_portdev(dev)) {
		/* NOTE : Linux driver maintains only MIPS counters */

		int32_t physport = -1;

		if ( (physport = _stat_helper_get_physport_by_dev(dev)) < 0) {
			/* No physical port exists */
			return storage;
		}

		memset(&stats, 0, sizeof(stat_helper_dev_stats64_t));

		/* get switch port stats */
		_stat_helper_get_switch_rmon_counters(CONFIG_SWITCH_DEV, physport, &stats);

		storage->rx_packets = stats.rx_packets - storage->rx_dropped; /* NOTE : exclude switch port drops */
		storage->tx_packets = stats.tx_packets;
#if (defined(CONFIG_LTQ_PPA_API) || defined(CONFIG_LTQ_PPA_API_MODULE))
		/* FIXME : Here, switch accelerated counters are missing in storage->rx_bytes */
		/* FIXME : Currently, PPA session counters are used */

		memset(&ppa_stats, 0, sizeof(PPA_NETIF_ACCEL_STATS));

		/* get PPA HW and SW accelerated session stats */
		if (ppa_hook_get_netif_accel_stats_fn) {
			ppa_hook_get_netif_accel_stats_fn(dev->name, &ppa_stats, (PPA_F_NETIF_HW_ACCEL | PPA_F_NETIF_SW_ACCEL));
		}

		/* add with ppa HW and SW accelerated session stats */
		storage->rx_bytes += ppa_stats.hw_accel_stats.rx_bytes + ppa_stats.sw_accel_stats.rx_bytes;
#endif
		storage->tx_bytes = stats.tx_bytes;
		storage->rx_errors += stats.rx_errors;
		storage->tx_errors += stats.tx_errors;
		storage->rx_dropped += stats.rx_dropped;
		storage->tx_dropped += stats.tx_dropped;
	}

	/* Ethernet WAN interface, say eth1 */
	else if (_stat_helper_is_ethwan_basedev(dev)) {
		/* NOTE : Linux driver maintains MIPS and PPE FW counters */

		int32_t physport = -1;

		if ( (physport = _stat_helper_get_physport_by_dev(dev)) < 0) {
			/* No physical port exists */
			return storage;
		}

		memset(&stats, 0, sizeof(stat_helper_dev_stats64_t));

		/* get switch port stats */
		_stat_helper_get_switch_rmon_counters(CONFIG_SWITCH_DEV, physport, &stats);

		storage->rx_packets = stats.rx_packets - storage->rx_dropped; /* NOTE : exclude switch port drops */
		storage->tx_packets = stats.tx_packets;
		/* FIXME : Here, switch accelerated counters are missing in storage->rx_bytes */
		storage->tx_bytes = stats.tx_bytes;
		storage->rx_errors += stats.rx_errors;
		storage->tx_errors += stats.tx_errors;
		storage->rx_dropped += stats.rx_dropped;
		storage->tx_dropped += stats.tx_dropped;
	}

	/* PTM WAN interface, say ptm0 */
	else if (_stat_helper_is_ptmwan_basedev(dev)) {
		/* NOTE : Linux driver maintains MIPS and PPE FW counters */

#if 0
#if (defined(CONFIG_LTQ_PPA_API) || defined(CONFIG_LTQ_PPA_API_MODULE))

		memset(&ppa_stats, 0, sizeof(PPA_NETIF_ACCEL_STATS));

		/* get PPE port MIB stats and HW accelerated session stats (only for tx_bytes) */
		if (ppa_hook_get_netif_accel_stats_fn) {
			ppa_hook_get_netif_accel_stats_fn(dev->name, &ppa_stats, (PPA_F_NETIF_PORT_MIB | PPA_F_NETIF_HW_ACCEL));
		}

		/* add with PPE port MIB stats */
		storage->rx_packets += ppa_stats.port_mib_stats.ig_fast_brg_pkts
								+ ppa_stats.port_mib_stats.ig_fast_rt_ipv4_udp_pkts
								+ ppa_stats.port_mib_stats.ig_fast_rt_ipv4_tcp_pkts
								+ ppa_stats.port_mib_stats.ig_fast_rt_ipv4_mc_pkts
								+ ppa_stats.port_mib_stats.ig_fast_rt_ipv6_udp_pkts
								+ ppa_stats.port_mib_stats.ig_fast_rt_ipv6_tcp_pkts;
		storage->rx_bytes += ppa_stats.port_mib_stats.ig_fast_brg_bytes
								+ ppa_stats.port_mib_stats.ig_fast_rt_ipv4_bytes
								+ ppa_stats.port_mib_stats.ig_fast_rt_ipv6_bytes;
		storage->tx_packets += ppa_stats.port_mib_stats.eg_fast_pkts;
		storage->tx_bytes += ppa_stats.hw_accel_stats.tx_bytes;
		storage->rx_dropped += ppa_stats.port_mib_stats.ig_drop_pkts;
#endif
#else

		memset(&ppa_stats, 0, sizeof(PPA_NETIF_ACCEL_STATS));

		/* get PPA HW accelerated session stats (only for tx_bytes) */
		if (ppa_hook_get_netif_accel_stats_fn) {
			ppa_hook_get_netif_accel_stats_fn(dev->name, &ppa_stats, (PPA_F_NETIF_HW_ACCEL));
		}

		/* add with PPA HW accelerated session stats (only for tx_bytes) */
		storage->tx_bytes += ppa_stats.hw_accel_stats.tx_bytes;
#endif
	}

	/* ATM WAN interface, such as nas0, nas1 and so on */
	else if (_stat_helper_is_atmwan_basedev(dev)) {
		/* NOTE : Linux driver maintains only MIPS counters */

#if (defined(CONFIG_LTQ_PPA_API) || defined(CONFIG_LTQ_PPA_API_MODULE))
		/* FIXME : Currently packet counters are not available from PPA */

		memset(&ppa_stats, 0, sizeof(PPA_NETIF_ACCEL_STATS));

		/* get PPA HW accelerated session stats */
		if (ppa_hook_get_netif_accel_stats_fn) {
			ppa_hook_get_netif_accel_stats_fn(dev->name, &ppa_stats, (PPA_F_NETIF_HW_ACCEL));
		}

		/* add with ppa HW accelerated session stats */
		storage->rx_bytes += ppa_stats.hw_accel_stats.rx_bytes;
		storage->tx_bytes += ppa_stats.hw_accel_stats.tx_bytes;
#endif
	}

	/* Do nothing for any other interface */

	return storage;
}
