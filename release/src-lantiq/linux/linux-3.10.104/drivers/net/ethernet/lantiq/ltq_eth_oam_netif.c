/******************************************************************************
**
** FILE NAME    : ltq_eth_oam_netif.c
** PROJECT      : UGW
** MODULES      : Ethernet OAM
**
** DATE         : 5 APRIL 2013
** AUTHOR       : Purnendu Ghosh
** DESCRIPTION  : Driver for Ethernet OAM handling
** COPYRIGHT    :   Copyright (c) 2013
**                LANTIQ DEUTSCHLAND GMBH,
**                Lilienthalstrasse 15, 85579 Neubiberg, Germany
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date        $Author                $Comment
** 5 APRIL 2013 PURNENDU GHOSH         Initiate Version
** 21 Feb 2014  Mahipati Deshpande     Ported to Kernel-3.10
*******************************************************************************/

/*
 *  Common Head File
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/miscdevice.h>
#include <linux/init.h>
#include <linux/etherdevice.h>  /*  eth_type_trans  */
#include <linux/ethtool.h>      /*  ethtool_cmd     */
#include <linux/if_ether.h>
#include <linux/if_arp.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/irq.h>
#include <asm/delay.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <net/xfrm.h>
#include <linux/if_vlan.h>
#include <../net/8021q/vlan.h>
#include <net/ltq_eth_oam_handler.h>
/*
 *  Chip Specific Head File
 */
#include <switch-api/lantiq_ethsw_api.h>
#ifdef CONFIG_LANTIQ_ETH_FRAMEWORK
#include <lantiq_eth_framework.h>
#endif


/*
 * ####################################
 *            Local Variable
 * ####################################
 */


/*
 * ####################################
 *            Local Function
 * ####################################
 */

struct net_device * (*fp_ltq_eth_oam_dev)(void)=NULL;
EXPORT_SYMBOL(fp_ltq_eth_oam_dev);

