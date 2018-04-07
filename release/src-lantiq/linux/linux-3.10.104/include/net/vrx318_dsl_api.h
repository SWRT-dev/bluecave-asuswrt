/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2015 Zhu YiXin<yixin.zhu@lantiq.com>
 */

#ifndef __VRX318_DSL_API_H__
#define __VRX318_DSL_API_H__

#include <net/genetlink.h>

#define VERSION_NR 1

enum vrx318_attr {
	VRX_TC_A_UNSPEC,
	VRX_TC_A_TC_MODE,
	VRX_TC_A_TC_ACTION,
	VRX_TC_A_BOND,
	VRX_TC_A_LINENO,
	__VRX_TC_A_MAX,
};
#define VRX_TC_A_MAX (__VRX_TC_A_MAX - 1)

enum vrx318_cmd {
	VRX_TC_C_UNSPEC,
	VRX_TC_C_NOTIFY,
	__VRX_TC_C_MAX,
};
#define VRX_TC_C_MAX (__VRX_TC_C_MAX - 1)

#define VRX_MCAST_GRP_NAME "vrx_mcgrp"
#define VRX_FAMILY_NAME "vrx_tc_notify"

enum vrx318_tc_type {
	ATM_TC = 1,
	PTM_TC,
};

enum vrx318_tc_action {
	TC_UNLOADED = 0,
	TC_LOADED,
};

enum vrx318_bonding_stat {
	NON_BONDING_MODE = 0,
	BONDING_MODE,
};

#endif /* __VRX318_DSL_API_H__ */
