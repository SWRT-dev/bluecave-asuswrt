
#ifndef STAT_HELPER_VR9_H
#define STAT_HELPER_VR9_H

#define CONFIG_SWITCH_WAN_PORT		5
#define CONFIG_SWITCH_LAN_PORT_1	4
#define CONFIG_SWITCH_LAN_PORT_2	2
#define CONFIG_SWITCH_LAN_PORT_3	1
#define CONFIG_SWITCH_LAN_PORT_4	0

static inline int32_t _stat_helper_get_physport_by_dev(struct net_device *dev)
{
	if (!strcmp(dev->name, "eth0_1")) {
		return CONFIG_SWITCH_LAN_PORT_1;
	} else if (!strcmp(dev->name, "eth0_2")) {
		return CONFIG_SWITCH_LAN_PORT_2;
	} else if (!strcmp(dev->name, "eth0_3")) {
		return CONFIG_SWITCH_LAN_PORT_3;
	} else if (!strcmp(dev->name, "eth0_4")) {
		return CONFIG_SWITCH_LAN_PORT_4;
	} else if (!strcmp(dev->name, "eth1")) {
		return CONFIG_SWITCH_WAN_PORT;
	} else {
		return -1;
	}
}

static inline int32_t _stat_helper_get_all_lan_physport(stat_helper_physport_list_t *portlist)
{
	portlist->port_num = 4;
	portlist->port[0] = CONFIG_SWITCH_LAN_PORT_1;
	portlist->port[1] = CONFIG_SWITCH_LAN_PORT_2;
	portlist->port[2] = CONFIG_SWITCH_LAN_PORT_3;
	portlist->port[3] = CONFIG_SWITCH_LAN_PORT_4;

	return 0;
}

#endif
