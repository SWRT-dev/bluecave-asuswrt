
#ifndef STAT_HELPER_PLATFORM_H
#define STAT_HELPER_PLATFORM_H

#define CONFIG_SWITCH_DEV "/dev/switch_api/0"

#define CONFIG_SWITCH_MAX_LAN_PORT	6

typedef struct stat_helper_lan_physport_list {
	int32_t port_num;
	int32_t port[CONFIG_SWITCH_MAX_LAN_PORT];
} stat_helper_physport_list_t;

typedef struct stat_helper_dev_stats64 {
	uint64_t	rx_packets;		/* total packets received	*/
	uint64_t	tx_packets;		/* total packets transmitted	*/
	uint64_t	rx_bytes;		/* total bytes received 	*/
	uint64_t	tx_bytes;		/* total bytes transmitted	*/
	uint64_t	rx_errors;		/* bad packets received		*/
	uint64_t	tx_errors;		/* packet transmit problems	*/
	uint64_t	rx_dropped;		/* no space in linux buffers	*/
	uint64_t	tx_dropped;		/* no space available in linux	*/
	uint64_t	multicast;		/* multicast packets received	*/
} stat_helper_dev_stats64_t;

/* FIXME : identify a MACVALN interface as an upper layer interface */
#define _stat_helper_is_upper_layer_dev(dev) ((dev->type == ARPHRD_PPP) \
										   || (dev->type == ARPHRD_TUNNEL) \
										   || (dev->type == ARPHRD_TUNNEL6) \
										   || (dev->type == ARPHRD_SIT) \
										   || (dev->priv_flags & IFF_802_1Q_VLAN) \
										   || (dev->priv_flags & IFF_EBRIDGE))

#define _stat_helper_is_ethlan_basedev(dev) (!strcmp(dev->name, "eth0"))
#define _stat_helper_is_ethlan_portdev(dev) (!strncmp(dev->name, "eth0_", 5))
#define _stat_helper_is_ethwan_basedev(dev) (!strcmp(dev->name, "eth1"))
#define _stat_helper_is_ptmwan_basedev(dev) (!strcmp(dev->name, "ptm0"))
#define _stat_helper_is_atmwan_basedev(dev) (dev->priv_flags & IFF_BR2684)

extern void _stat_helper_get_switch_rmon_counters(int8_t *switch_dev, uint8_t port, stat_helper_dev_stats64_t *stats);

extern struct rtnl_link_stats64*
stat_helper_platform_dev_get_extended_stats64(struct net_device *dev,
									 struct rtnl_link_stats64 *storage);

#endif
