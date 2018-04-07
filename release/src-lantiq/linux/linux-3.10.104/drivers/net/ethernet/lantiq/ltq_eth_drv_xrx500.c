/******************************************************************************
**
** FILE NAME    : lantiq_eth_drv.c
** AUTHOR       : Suresh Nagaraj
** DESCRIPTION  : Lantiq ethernet driver for XRX500 series
** COPYRIGHT    :       Copyright (c) 2015
**                      Lantiq Deutschland
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date                $Author                 $Comment
*******************************************************************************/
#include <linux/version.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/kernel.h> /* printk() */
#include <linux/types.h>  /* size_t */
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/proc_fs.h>
#include <linux/etherdevice.h> /* eth_type_trans */
#include <asm/delay.h>
#include <linux/init.h>
#include <linux/clk.h>

#include <linux/of_net.h>
#include <linux/of_mdio.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>

#include <lantiq.h>
#include <net/datapath_api.h>
#include <net/lantiq_cbm.h>
#include <net/lantiq_cbm_api.h>
#include <xway/switch-api/lantiq_gsw_api.h>
#include "ltq_eth_drv_xrx500.h"

#define CONFIG_XRX500_MDIO_SUPPORT
#define CONFIG_USERSPACE_LINK_NOTIFICATION

#define LTQ_ETH_DATA_LEN 8000

#ifdef CONFIG_USERSPACE_LINK_NOTIFICATION
#include <net/genetlink.h>
#endif

static int xrx500_mdio_probe(struct net_device *dev, struct xrx500_port *port);

static int ltq_gsw_pmac_init(void);

#define LTQ_RXCSUM
#define DRV_MODULE_NAME             "lantiq_eth_drv_xrx500"
#define DRV_MODULE_VERSION          "1.1"

#ifdef CONFIG_OFFLOAD_FWD_MARK
#define SWITCH_ID 									12345
#define SWITCH_ID2 									23456
#endif
/* length of time before we decide the hardware is borked,
 * and dev->eth_tx_timeout() should be called to fix the problem
 */
#define LTQ_TX_TIMEOUT                  (10 * HZ)

static u8 g_my_ethaddr[MAX_ADDR_LEN * 2] = {0};
#define MY_ETH0_ADDR g_my_ethaddr
#define LTQ_ETHWAN_PORT 6

static char wan_iface[IFNAMSIZ] = "eth1";

#if defined(CONFIG_HGU_BONDING) && CONFIG_HGU_BONDING
static struct net_device *eth_wan_dev = NULL;
#endif

static struct net_device *eth_dev[NUM_ETH_INF];
static struct module g_ltq_eth_module[NUM_ETH_INF];
static u32 g_rx_csum_offload = 0;

/* PMAC header structure */
ltq_pmac_header_t eg_pkt_hdr;
ltq_pmac_header_t ig_pkt_hdr;

/* Start the  network device interface queue */
static int ltq_eth_open(struct net_device *dev);
/* Stop the  network device interface queue */
static int ltq_eth_close(struct net_device *dev);
/* Transmit packet from Tx Queue to MAC */
static int ltq_start_xmit (struct sk_buff *skb, struct net_device *dev);
/* Hardware specific IOCTL's  */
static int ltq_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);
/* Get the network device statistics */
static struct rtnl_link_stats64 *ltq_get_stats (struct net_device *dev, struct rtnl_link_stats64 *storage);
/* change MTU values */
static int ltq_change_mtu (struct net_device *dev, int new_mtu);
/*  Set mac address*/
static int ltq_set_mac_address(struct net_device *dev, void *p);
/* Transmit timeout*/
static void ltq_tx_timeout (struct net_device *dev);
/* Init of the network device */
static int ltq_switch_init(struct net_device *dev);

/* interface change event handler */
static int phy_netdevice_event(struct notifier_block *nb, unsigned long action, void *ptr);
/* Get the ether addr from u-boot */
/*static unsigned char my_ethaddr[MAX_ADDR_LEN];*/
static struct xrx500_hw xrx500_hw;
#define MDIO_PHY_ADDR_MASK	0x001f
#define MDIO_PHY_LINK_DOWN	0x4000
#define MDIO_PHY_LINK_UP	0x2000

#define MDIO_PHY_FDUP_EN	0x0600
#define MDIO_PHY_FDUP_DIS	0x0200

#ifdef CONFIG_XRX500_ETH_DRV_COC_SUPPORT
#define LTQ_ETH_NUM_INTERRUPTS 5
DECLARE_BITMAP(g_ltq_eth_intr_type, LTQ_ETH_NUM_INTERRUPTS);
static u32 g_ltq_eth_gswl_irq;
struct tasklet_struct gswl_tasklet;

DECLARE_BITMAP(g_ltq_pae_intr_type, LTQ_ETH_NUM_INTERRUPTS);
static u32 g_ltq_pae_irq;
struct tasklet_struct pae_tasklet;

enum ltq_cpufreq_state g_ltq_eth_drv_coc_state = LTQ_CPUFREQ_PS_D0;
static int32_t dp_fp_coc_confirm(enum ltq_cpufreq_state new_state, 
								enum ltq_cpufreq_state old_state, u32 flags);
#endif

/*
 *  Datapath directpath functions
 */
static int32_t dp_fp_stop_tx (struct net_device *);
static int32_t dp_fp_restart_tx (struct net_device *);
static int32_t dp_fp_rx (struct net_device *, struct net_device *, struct sk_buff *, int32_t);

static struct net_device_ops ltq_eth_drv_ops = {
        .ndo_init           = ltq_switch_init,
        .ndo_open           = ltq_eth_open,
        .ndo_stop           = ltq_eth_close,
        .ndo_start_xmit     = ltq_start_xmit,
        .ndo_set_mac_address= ltq_set_mac_address,
        .ndo_change_mtu     = ltq_change_mtu,
        .ndo_get_stats64      = ltq_get_stats,
        .ndo_do_ioctl       = ltq_ioctl,
        .ndo_tx_timeout     = ltq_tx_timeout,
};

static struct notifier_block netdevice_notifier = {
		.notifier_call = phy_netdevice_event
};

#undef DUMP_PACKET

#ifdef DUMP_PACKET
/*
* \brief	dump skb data
* \param[in] len length of the data buffer
* \param[in] pData Pointer to data to dump
*
* \return void No Value
*/
static inline void dump_skb(u32 len, char *pData){
	int i;
	for(i=0;i<len;i++){
		printk("%2.2x ",(u8)(pData[i]));
		if (i % 16 == 15)
			printk("\n");
	}
	printk("\n");
}
#endif

/* Get the driver information, used by ethtool_ops  */
static void get_drvinfo (struct net_device *dev, struct ethtool_drvinfo *info)
{
    /* driver driver short name (Max 32 characters) */
    strcpy (info->driver, DRV_MODULE_NAME);
    /* driver version (Max 32 characters) */
    strcpy (info->version, DRV_MODULE_VERSION);
}

/* Get the network device settings  */
static int get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	ltq_switch_priv_t *priv = netdev_priv(dev);
	int i;

	for (i=0; i<priv->num_port; i++)
		if (priv->port[i].phydev)
			return phy_ethtool_gset(priv->port[i].phydev, cmd);

	return -ENODEV;
}

/* Set the network device settings */
static int set_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	ltq_switch_priv_t *priv = netdev_priv(dev);
	int i;

	pr_info("%s: called\n", __func__);
	for (i=0; i<priv->num_port; i++)
		if (priv->port[i].phydev)
			return phy_ethtool_sset(priv->port[i].phydev, cmd);

	return -ENODEV;
}

/* Reset the device */
static int nway_reset(struct net_device *dev)
{
     /*TODO*/
    return 0;
}

/* Structure of the ether tool operation  */
static const struct ethtool_ops ethtool_ops = {
        .get_drvinfo            = get_drvinfo,
        .get_settings           = get_settings,
        .set_settings           = set_settings,
        .nway_reset             = nway_reset,
        .get_link               = ethtool_op_get_link,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
        .get_perm_addr          = ethtool_op_get_perm_addr,
#endif
};

/* open the network device interface*/
static int ltq_eth_open(struct net_device *dev)
{
    ltq_switch_priv_t *priv             = netdev_priv(dev);

    priv->dp_subif.subif = 0;
    priv->dp_subif.port_id = priv->dp_port_id; 

    if (dp_register_subif (priv->owner, dev, dev->name, &priv->dp_subif, 0) != DP_SUCCESS) {
	pr_err("%s: failed to open for device: %s \n", __FUNCTION__, dev->name);
	return -1;
    } 

	/* Enable the p2p channel at CBM */
	if (!turn_on_DMA_p2p())
			pr_info("p2p channel turned ON !\n");
	else
			pr_info("p2p channel already ON !\n");
    return 0;
}

/* Close the network device interface*/
static int ltq_eth_close(struct net_device *dev)
{
    ltq_switch_priv_t *priv             = netdev_priv(dev);

    priv->dp_subif.subif = 0;
    priv->dp_subif.port_id = priv->dp_port_id; 

    if (dp_register_subif (priv->owner, dev, dev->name, &priv->dp_subif, DP_F_DEREGISTER) != DP_SUCCESS) {
	pr_err("%s: failed to close for device: %s \n", __FUNCTION__, dev->name);
	return -1;
    } 
//    netif_stop_queue(dev);
    return 0;
}

/* Send the packet to netwrok rx queue */
static void eth_rx(struct net_device *dev, int len,struct sk_buff* skb)
{
    ltq_switch_priv_t *priv             = netdev_priv(dev);

    skb->dev = dev;
    skb->protocol = eth_type_trans(skb, dev);

#if defined(CONFIG_HGU_BONDING) && CONFIG_HGU_BONDING
    if( priv->dp_port_id == CONFIG_HGU_BOND_PORT ) {
        if(!eth_wan_dev){
            eth_wan_dev=dev_get_by_name(&init_net, wan_iface);
        }
        skb->dev=eth_wan_dev;
	dev=eth_wan_dev;
    }
#endif
	if (dev->features & NETIF_F_RXCSUM) {
		skb->ip_summed = CHECKSUM_UNNECESSARY;
	}
#ifdef CONFIG_OFFLOAD_FWD_MARK
		if(strncmp(dev->name, wan_iface, IFNAMSIZ) == 0)
			skb->offload_fwd_mark = SWITCH_ID2;
		else
			skb->offload_fwd_mark = SWITCH_ID;
#endif
    pr_debug ("passing to stack: protocol: %x\n", skb->protocol);
    netif_rx(skb);
    priv->stats.rx_packets++;
    priv->stats.rx_bytes+=len;
}

static int32_t dp_fp_stop_tx (struct net_device *netif)
{
	return 0;
}

static int32_t dp_fp_restart_tx (struct net_device *netif)
{
	return 0;
}

static int32_t dp_fp_rx (struct net_device *rxif, struct net_device *txif, struct sk_buff *skb, int32_t len)
{

/*This check is removed as the upper layers are taking care of the headers and the size*/
#if 0
#ifndef CONFIG_LTQ_TOE_DRIVER
   if ((len > (rxif->mtu + ETH_HLEN + sizeof(ltq_pmac_header_t))) || (len < ETH_ZLEN) ) {
       	 pr_err("%s[%d]: Packet is too large/small (%d)!!!\n", __func__,__LINE__,len);
       	 goto rx_err_exit;
   }
#endif
#endif

   /*skb_put(skb,len);*/
#ifdef DUMP_PACKET
    if (skb->data) {
        printk("raw data len:%d\n",len);
        dump_skb(len, (char *)skb->data);
    }
#endif

   if (skb) {
   	 len -= (sizeof(ltq_pmac_header_t));  /*Remove PMAC to DMA header */
   	 skb_pull(skb,(sizeof(ltq_pmac_header_t)));
   } else {
	 pr_err("%s: skb from DP is null !\n", __func__);
	 goto rx_err_exit;
   }

   /* Pass it to the stack */
#ifdef DUMP_PACKET
    if (skb->data) {
        printk("data sent to stack \n");
        dump_skb(len, (char *)skb->data);
    }
#endif
   if (rxif) {
   	  pr_debug("%s: rxed a packet from DP lib on interface %s ..\n",
	  							__func__, rxif->name);
      eth_rx(rxif,len,skb);
   } else {
   	  pr_err("%s: error: rxed a packet from DP lib on interface %x ..\n",
	  							__func__, (unsigned int)rxif);
	  goto rx_err_exit;
   }
      
   return 0;
rx_err_exit:
    if (skb)
        dev_kfree_skb_any(skb);
   return -1;
}

/* Get the network device stats information */
static struct rtnl_link_stats64 *ltq_get_stats (struct net_device *dev,
	struct rtnl_link_stats64 *storage)
{
#ifndef CONFIG_LTQ_DATAPATH_MIB
    ltq_switch_priv_t *priv = netdev_priv(dev);
    *storage = priv->stats;
#else
    dp_get_netif_stats (dev, NULL, storage, 0);
#endif
    return storage;
}

/* Trasmit timeout */
static void ltq_tx_timeout(struct net_device *dev)
{
//    ltq_switch_priv_t *priv		= netdev_priv(dev);
}

/* Set the MAC address */
static int ltq_set_mac_address (struct net_device *dev, void *p)
{
#if 0
    ltq_switch_priv_t *priv		= netdev_priv(dev);
#endif
    struct sockaddr *addr = p;

    if (netif_running(dev))
        return -EBUSY;

    if (!is_valid_ether_addr(addr->sa_data))
        return -EINVAL;

    memcpy(dev->dev_addr, addr->sa_data, dev->addr_len);
#if 0
    /* TODO: is it required to set MAC address in the HW registers?*/
    spin_lock_irq(&priv->lock);
    if (!(dev->flags & IFF_PROMISC)) {
        __set_mac_addr(dev->dev_addr);
    }
    spin_unlock_irq(&priv->lock);
#endif
    return 0;
}

static int ltq_enable_gsw_l_jumbo(struct net_device *dev) 
{
	ltq_switch_priv_t *priv = netdev_priv(dev);
	GSW_API_HANDLE gswl;
	GSW_register_t regCfg;

	pr_info("%s called for id: %d\n", __func__, priv->id);

	/* Do the GSW-L configuration */
	pr_info("doing the GSW-L configuration for jumbo\n");
	gswl = gsw_api_kopen("/dev/switch_api/0");
	if (gswl == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}

	/* Set the MAC control register 2 to enable Jumbo frames */
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x8f9 + (0xc * (priv->id + 1));
	gsw_api_kioctl(gswl, GSW_REGISTER_GET, (unsigned int)&regCfg);
	regCfg.nData |= 0x8;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);

	/* PMAC control register 2 to enable jumbo frames */
	pr_info("doing the PMAC configuration for jumbo at GSW-L\n");
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0xd05;
	gsw_api_kioctl(gswl, GSW_REGISTER_GET, (unsigned int)&regCfg);
	regCfg.nData |= 0x8;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);

	/* Set the frame length */
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0xd06;
	regCfg.nData = 9216;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);

	gsw_api_kclose(gswl);


	return 0;
}

static int ltq_disable_gsw_l_jumbo(struct net_device *dev) 
{
	ltq_switch_priv_t *priv = netdev_priv(dev);
	GSW_API_HANDLE gswl;
	GSW_register_t regCfg;

	pr_info("%s called for id: %d\n", __func__, priv->id);

	/* Do the GSW-L configuration */
	pr_info("doing the GSW-L configuration for jumbo\n");
	gswl = gsw_api_kopen("/dev/switch_api/0");
	if (gswl == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}

	/* Set the MAC control register 2 to enable Jumbo frames */
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x8f9 + (0xc * (priv->id + 1));
	gsw_api_kioctl(gswl, GSW_REGISTER_GET, (unsigned int)&regCfg);
	regCfg.nData &= ~8;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);

	gsw_api_kclose(gswl);
	return 0;
}

static int ltq_enable_gsw_r_pmac_jumbo (struct net_device *dev)
{
	GSW_API_HANDLE gswr;
	GSW_register_t regCfg;

	/* Do the GSW-R configuration */
	gswr = gsw_api_kopen("/dev/switch_api/1");
	if (gswr == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}
	/* PMAC control register 2 to enable jumbo frames */
	pr_info("doing the PMAC configuration for jumbo at GSW-R\n");
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0xd05;
	gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg);
	regCfg.nData |= 0x8;
	gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg);

	/* Set the frame length */
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0xd06;
	regCfg.nData = 1728;
	gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg);

	gsw_api_kclose(gswr);

	return 0;
}

static int ltq_enable_gsw_r_jumbo(struct net_device *dev) 
{
	GSW_API_HANDLE gswr;
	GSW_register_t regCfg;

	/* Do the GSW-R configuration */
	gswr = gsw_api_kopen("/dev/switch_api/1");
	if (gswr == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}

	/* Set the MAC control register 2 to enable Jumbo frames */
	pr_info("doing the GSW-R configuration for jumbo\n");
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x905;
	gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg);
	regCfg.nData |= 0x8;
	gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg);

	gsw_api_kclose(gswr);

	return 0;
}

static int ltq_disable_gsw_r_jumbo(struct net_device *dev) 
{
	GSW_API_HANDLE gswr;
	GSW_register_t regCfg;

	/* Do the GSW-R configuration */
	gswr = gsw_api_kopen("/dev/switch_api/1");
	if (gswr == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}

	/* Set the MAC control register 2 to enable Jumbo frames */
	pr_info("doing the GSW-R configuration for jumbo\n");
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x905;
	gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg);
	regCfg.nData |= 0x8;
	gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg);

	gsw_api_kclose(gswr);

	return 0;
}

/* Change the MTU value of the netwrok device interfaces */
static int ltq_change_mtu (struct net_device *dev, int new_mtu)
{
	ltq_switch_priv_t *priv = netdev_priv(dev);

	if(new_mtu < ETH_ZLEN || new_mtu > LTQ_ETH_DATA_LEN)
		return -EINVAL;

	/* if the MTU > 1500, do the jumbo config in switch */
	if (new_mtu > ETH_DATA_LEN && !(priv->jumbo_enabled)) {
		if (priv->wan) {
			if (ltq_enable_gsw_r_jumbo(dev) < 0)
				return -EIO;
		} else {
			if (ltq_enable_gsw_l_jumbo(dev) < 0)
				return -EIO;
		}
		ltq_enable_gsw_r_pmac_jumbo(dev);
		priv->jumbo_enabled = 1;
	} else if (priv->jumbo_enabled) {
		if (priv->wan)
			ltq_disable_gsw_r_jumbo(dev);
		else
			ltq_disable_gsw_l_jumbo(dev);
		priv->jumbo_enabled = 0;
	}

	dev->mtu = new_mtu;
	return 0;
}

static int ltq_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	ltq_switch_priv_t *priv		= netdev_priv(dev);
	int ret;
	int len;

    /* Call the Datapath Library's TX function */
	((struct dma_tx_desc_1 *)&(skb->DW1))->field.ep = priv->dp_subif.port_id;
	((struct dma_tx_desc_0 *)&(skb->DW0))->field.dest_sub_if_id =
		priv->dp_subif.subif;

	len = skb->len;
	if (dev->features & NETIF_F_HW_CSUM) {
    	ret = dp_xmit(dev, &(priv->dp_subif), skb, skb->len, DP_TX_CAL_CHKSUM);
	} else {
		ret = dp_xmit(dev, &(priv->dp_subif), skb, skb->len, 0);
	}

	if (!ret) {
			priv->stats.tx_packets++;
			priv->stats.tx_bytes += len;
	} else {
			priv->stats.tx_dropped++;
	}
    return 0;
}

/* Platform specific IOCTL's handler */
static int ltq_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
  /* TODO*/

    return -EOPNOTSUPP;
}

/* init of the network device */
static int ltq_switch_init(struct net_device *dev)
{
#ifdef CONFIG_XRX500_MDIO_SUPPORT
	int i;
	ltq_switch_priv_t *priv;

	priv = netdev_priv(dev);
	pr_debug("probing for number of ports = %d\n", priv->num_port);
	for (i = 0; i < priv->num_port; i++)
		if (xrx500_mdio_probe(dev, &priv->port[i])) {
			pr_warn("xrx500-mdio: probing phy of port %d failed\n",
					 priv->port[i].num);
		} else {
			dev->ethtool_ops = &ethtool_ops;
		}
#endif

    return 0;
}

/* Driver version info */
static inline int eth_drv_ver(char *buf)
{
    return sprintf(buf, "Lantiq ethernet driver for XRX500 !\n");
}

static int ltq_gsw_pmac_init(void)
{
	u8 i = 0, j = 0, k = 0;
	GSW_PMAC_Eg_Cfg_t egCfg;
	GSW_PMAC_Ig_Cfg_t igCfg;
	GSW_CPU_PortCfg_t cpuPortCfg;
	GSW_portCfg_t gswPortCfg;
	GSW_register_t regCfg;
	GSW_API_HANDLE gswl, gswr;

	/* Do the GSW-L configuration */
	gswl = gsw_api_kopen("/dev/switch_api/0");
	if (gswl == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}

	memset((void *)&egCfg, 0x00, sizeof(egCfg));
	memset((void *)&igCfg, 0x00, sizeof(igCfg));
	memset((void *)&cpuPortCfg, 0x00, sizeof(cpuPortCfg));

	/*
	 * 1. GSWIP-L PMAC Egress Configuration Table
	 * Entry:
	 * Address: (i from 0 to 15, j from 0 to 3)
	 * Destination Port = 0, Traffic Class = i, Others field = j
	 * Value:
	 * DMA Channel = if i < 3? i: 3
	 * PMAC_Flag = 1
	 * Other fields = 0
	 */
	pr_info ("PMAC_EG_CFG_SET for GSW-L\n");
	for (i = 0; i <= 15; i++)
	{
		for (j = 0; j <= 3; j++)
		{
#if defined(SINGLE_RX_CH0_ONLY) && SINGLE_RX_CH0_ONLY
                        egCfg.nRxDmaChanId  = 0; 
#else
                        egCfg.nRxDmaChanId  = (i < 3) ? i : 3; 
#endif
                        egCfg.bRemL2Hdr     = 0; 
                        egCfg.numBytesRem   = 0; 
                        egCfg.bFcsEna       = 0;
                        egCfg.bPmacEna      = 1; 
                        egCfg.nResDW1       = 0;
                        egCfg.nRes1DW0      = 0;
                        egCfg.nRes2DW0      = 0;
                        egCfg.nDestPortId   = 0; 
                        egCfg.nTrafficClass = i;
                        egCfg.nFlowIDMsb    = j;
                        egCfg.bDecFlag      = 0;
                        egCfg.bEncFlag      = 0;
                        egCfg.bMpe1Flag     = 0;
                        egCfg.bMpe2Flag     = 0;
			egCfg.bTCEnable	    = 1;
			gsw_api_kioctl(gswl, GSW_PMAC_EG_CFG_SET, (unsigned int)&egCfg);
		}
	}
	/*
	 * 2. GSWIP-L PMAC Ingress Configuration Table
	 * Entry:
	 * Address: (i from 0 to 15)
	 * DMA Channel = i
	 * Value:
	 * PMAC_Flag = 1
	 * SPPID_Mode = 1
	 * SUBID_Mode = 1
	 * CLASSEN_Mode = 1
	 * CLASS_Mode = 1 
	 * PMAPEN_Mode = 1
	 * PMAP_Mode = 1
	 * TCPERR_DP = 1
	 * DF_PMAC_HD.PMAP_EN = 1, DF_PMAC_HD.PMAP = 1 << i[2:0], DF_PMAC_HD.CLASS_EN = 1, DF_PMAC_HD.CLASS = i[3]*2 + 1
	 * Other fields = 0
	 */
	pr_info ("PMAC_IG_CFG_SET for GSW-L\n");
	for (i = 0; i <= 15; i++)
	{
                igCfg.nTxDmaChanId  = i; 
                igCfg.bErrPktsDisc  = 1; 
                igCfg.bPmapDefault  = 1; 
                igCfg.bPmapEna      = 1; 
                igCfg.bClassDefault = 1; 
                igCfg.bClassEna     = 1; 
                igCfg.bSubIdDefault = 1; 
                igCfg.bSpIdDefault  = 1; 
                igCfg.bPmacPresent  = 1; 
                igCfg.defPmacHdr[0] = 0;
                igCfg.defPmacHdr[1] = 0;
                igCfg.defPmacHdr[2] = ((((i & 8) >> 3) * 2) + 1);/* 1/3 */
                igCfg.defPmacHdr[3] = 0x90; //(1 << 7) | (1 << 4);
                igCfg.defPmacHdr[4] = 0;
                igCfg.defPmacHdr[5] = 0;
                /*if ( i >= 8)
                	igCfg.defPmacHdr[6] = 1 << (i & 0x7);
                else*/
                igCfg.defPmacHdr[7] = 1 << (i & 0x7);
		gsw_api_kioctl(gswl, GSW_PMAC_IG_CFG_SET, (unsigned int)&igCfg);
	}

	/* Enable the Ingress Special Tag */
	cpuPortCfg.nPortId = 0;
	cpuPortCfg.bCPU_PortValid = 0;
	cpuPortCfg.bSpecialTagIngress = 1;
	cpuPortCfg.bSpecialTagEgress = 0;
	cpuPortCfg.bFcsCheck = 0;
	cpuPortCfg.bFcsGenerate = 1;
	gsw_api_kioctl(gswl, GSW_CPU_PORT_CFG_SET, (unsigned int)&cpuPortCfg);

#ifdef CONFIG_USE_EMULATOR
	/* Add some extra register writes for the 1.4.1 */
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x90f;
	regCfg.nData = 0x182;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);

	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x91b;
	regCfg.nData = 0x182;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);

	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x927;
	regCfg.nData = 0x182;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);

	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x933;
	regCfg.nData = 0x182;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);

	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x93f;
	regCfg.nData = 0x182;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);
#endif

	/* Enable the CPU port MAC address spoofing detection */
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x480;
	gsw_api_kioctl(gswl, GSW_REGISTER_GET, (unsigned int)&regCfg);
	regCfg.nData |= 0x4000;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);

	/* PMAC control register 2 to disable LCHKS */
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0xd05;
	gsw_api_kioctl(gswl, GSW_REGISTER_GET, (unsigned int)&regCfg);
	regCfg.nData &= ~0x3;
	gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg);

	gsw_api_kclose(gswl);

	/* Do the GSW-R configuration */
	gswr = gsw_api_kopen("/dev/switch_api/1");
	if (gswr == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}

	/*
	 * 3. GSWIP-R PMAC Egress Configuration Table
	 * Entry:
	 * Address: (k i from 0 to 15,j from 0 to 63)
	 * Destination Port = k, Others field = j
	 * Value:
	 * DMA Channel = if k <= 13? 0: k
	 * PMAC_Flag = if k ==15 or k < 7? 1: 0
	 * FCS_Mode = if k ==13? PTM_Bonding_Mode:0
	 * L2_HD_RM = if k == 12 or k == 14? 1:0
	 * L2_HD_Bytes = if k == 14? CAPWAP_HD_BYTES:0
	 * Other fields = 0
	 */
	pr_info ("PMAC_EG_CFG_SET for GSW-R\n");
	for (k =0; k<=15; k++) {
		for (i = 0; i <= 15; i++) {
			for (j = 0; j <= 3; j++) {
#ifdef CONFIG_USE_EMULATOR
				egCfg.nRxDmaChanId  = (k <= 14) ? 0 : 5;
#else
				if (ltq_get_soc_rev() == 2)
					egCfg.nRxDmaChanId  = (k <= 13) ? 0 : 5;
				else
					egCfg.nRxDmaChanId  = (k <= 13) ? 0 : k;
#endif
				egCfg.bPmacEna      = 1; //((k == 15) || (k < 7)) ? 1 : 0;
				egCfg.bFcsEna       = (k == 13) ? 1 /*PTM_Bonding_Mode*/: 0;
				egCfg.bRemL2Hdr     = 0; //((k == 12) || (k == 14))? 1 : 0;
				egCfg.numBytesRem   = (k == 14) ? 1 /*CAPWAP_HD_BYTES*/: 0; 
				egCfg.nResDW1       = 0;
				egCfg.nRes1DW0      = 0;
				egCfg.nRes2DW0      = 0;
				egCfg.nDestPortId   = k;
				egCfg.nTrafficClass = i;
				egCfg.bMpe1Flag     = 0;
				egCfg.bMpe2Flag     = 0;
				egCfg.bEncFlag      = 0;
				egCfg.bDecFlag      = 0;
				egCfg.nFlowIDMsb    = j;
				egCfg.bTCEnable	    = 1;

				gsw_api_kioctl(gswr, GSW_PMAC_EG_CFG_SET, (unsigned int)&egCfg);		
			}
		}
	}

	/*
	 * 4. GSWIP-R PMAC Ingress Configuration Table
	 * Entry:
	 * Address: (i from 0 to 15)
	 * DMA Channel  = i
	 * Value:
	 * PMAC_Flag = if 5 < i < 15? 0: 1
	 * SPPID_Mode = 0
	 * SUBID_Mode = If 7 <= i <=8? 0: 1
	 * CLASSEN_Mode = 0
	 * CLASS_Mode = 0
	 * PMAPEN_Mode = if i < 4 or i == 15? 1:0
	 * PMAP_Mode = if i < 4 or i == 15? 1:0
	 * TCPERR_DP = 1 
	 * DF_PMAC_HD.SPPID = i, DF_PMAC_HD.PMAP = if i == 15? 0x8000:0xFFFF
	 * DF_PMAC_HD.PMAC_EN = if i==15?1:0
	 * Other fields = 0
	 */
	pr_info ("PMAC_IG_CFG_SET for GSW-R\n");
	for (i = 0; i <= 15; i++)
	{
                igCfg.nTxDmaChanId  = i; 
                igCfg.bPmacPresent  = 1; /*((i > 5) && (i < 15)) ? 0 : 1; */
                /*igCfg.bSpIdDefault  = 0; */
                igCfg.bSpIdDefault  = (i == 15) ? 1 : 0; // For channel 15, use source port ID from default PMAC header
                igCfg.bSubIdDefault = 0; //((i == 6) || (i == 13)) ? 0 : 1;
                igCfg.bClassDefault = 0; 
                igCfg.bClassEna     = 0; 
                igCfg.bErrPktsDisc  = 1; 
                
                igCfg.bPmapDefault  = ((i < 4) || (i == 15)) ? 1 : 0;
                igCfg.bPmapEna      = ((i < 4) || (i == 15)) ? 1 : 0; 

                igCfg.defPmacHdr[0] = 0;
                igCfg.defPmacHdr[1] = 0;
                /*igCfg.defPmacHdr[2] = (i << 4); // Byte 2 (Bits 7:4)*/
				igCfg.defPmacHdr[2] =  (i == 15) ? 0 : (i<<4); // For channel 15, source port is 0.
                igCfg.defPmacHdr[3] = (i == 15) ? 0x80 : 0; // Byte 3 (Bit 7)
                igCfg.defPmacHdr[4] = 0;
                igCfg.defPmacHdr[5] = 0;
                igCfg.defPmacHdr[6] = (i == 15) ? 0x80 : 0xFF;
                igCfg.defPmacHdr[7] = (i == 15) ? 0x00 : 0xFF;

		gsw_api_kioctl(gswr, GSW_PMAC_IG_CFG_SET, (unsigned int)&igCfg);
	}

	/* Enable the Ingress Special Tag */
	for (k =0; k<15; k++) {

		cpuPortCfg.nPortId = k;

		/* get the current values first */	
		gsw_api_kioctl(gswr, GSW_CPU_PORT_CFG_GET, (unsigned int)&cpuPortCfg);

		/* change the required values and set it back */
		cpuPortCfg.bCPU_PortValid = 0;
		cpuPortCfg.bSpecialTagIngress = 1;
		cpuPortCfg.bSpecialTagEgress = 0;
		cpuPortCfg.bFcsCheck = 0;
		cpuPortCfg.bFcsGenerate = 1;
		gsw_api_kioctl(gswr, GSW_CPU_PORT_CFG_SET, (unsigned int)&cpuPortCfg);
	}

#if 1
	/* Disable the Learning in the GSWIP-R */
	for (k =0; k<15; k++) {
		memset((void *)&gswPortCfg, 0x00, sizeof(gswPortCfg));

		gswPortCfg.nPortId = k;
		gsw_api_kioctl(gswr, GSW_PORT_CFG_GET, (unsigned int)&gswPortCfg);

		gswPortCfg.bLearning = 1;
		gsw_api_kioctl(gswr, GSW_PORT_CFG_SET, (unsigned int)&gswPortCfg);
	}
#endif

#ifdef CONFIG_USE_EMULATOR
	/* Add some extra register writes for the 1.4.1 */
	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x903;
	regCfg.nData = 0x182;
	gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg);
#endif

	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x454;
	regCfg.nData = 0x1;
	gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg);

	memset((void *)&regCfg, 0x00, sizeof(regCfg));
	regCfg.nRegAddr = 0x455;
	regCfg.nData = 0x1;
	gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg);

	gsw_api_kclose(gswr);
	pr_info ("\n\t GSW PMAC Init Done!!!\n");
	return 0;
}

static inline void ethaddr_setup(unsigned int port, char *line)
{
    u8 *p;
    char *ep;
    int i;

    p = MY_ETH0_ADDR + (port ? MAX_ADDR_LEN : 0);
    memset(p, 0, MAX_ADDR_LEN * sizeof(*p));
    for ( i = 0; i < 6; i++ )
    {
        p[i] = line ? simple_strtoul(line, &ep, 16) : 0;
        if ( line )
            line = *ep ? ep + 1 : ep;
    }
    pr_info("eth%d mac address %02X-%02X-%02X-%02X-%02X-%02X",
        port ? 1 : 0,
        (u32)p[0] & 0xFF, (u32)p[1] & 0xFF, (u32)p[2] & 0xFF,
        (u32)p[3] & 0xFF, (u32)p[4] & 0xFF, (u32)p[5] & 0xFF);
}

static int __init ltq_eth_drv_eth0addr_setup(char *line)
{
    ethaddr_setup(0, line);

    return 0;
}

static int __init ltq_eth_drv_eth1addr_setup(char *line)
{
    ethaddr_setup(1, line);

    return 0;
}

static void ltq_eth_drv_eth_addr_setup(struct net_device *dev, int port)
{
    u8 *ethaddr;
    u32 val;
	int i;

	if (port == LTQ_ETHWAN_PORT)
    	ethaddr = MY_ETH0_ADDR + MAX_ADDR_LEN; 
	else
    	ethaddr = MY_ETH0_ADDR; 

    /*  read MAC address from the MAC table and put them into device    */
    for ( i = 0, val = 0; i < 6; i++ )
        val += dev->dev_addr[i];
    if ( val == 0 )
    {
        for ( i = 0, val = 0; i < 6; i++ )
            val += ethaddr[i];
        if ( val == 0 )
        {
            /*  ethaddr not set in u-boot   */
            dev->dev_addr[0] = 0x00;
            dev->dev_addr[1] = 0x20;
            dev->dev_addr[2] = 0xda;
            dev->dev_addr[3] = 0x86;
            dev->dev_addr[4] = 0x23;
            dev->dev_addr[5] = 0x74 + port;
        }
        else
        {
            for ( i = 0; i < 5; i++ )
                dev->dev_addr[i] = ethaddr[i];
            // dev->dev_addr[5] = ethaddr[i] + port;
            dev->dev_addr[5] = ethaddr[i];
        }
    }
	return;
}

#ifdef CONFIG_USERSPACE_LINK_NOTIFICATION

enum eth_cmd{
	ETH_C_UNSPEC,
	ETH_C_NOTIFY,
	__ETH_C_MAX,
};
#define ETH_C_MAX (__ETH_C_MAX - 1)

enum eth_attr{
	ETH_A_UNSPEC,
	ETH_A_LINK_NAME,
	ETH_A_LINK_STATUS,
	ETH_A_LINK_SPEED,
    __ETH_A_MAX,
};
#define ETH_LINK_A_MAX (__ETH_A_MAX - 1)

/* VRX318 TC message genelink family */
struct genl_family eth_gnl_family = {
	.id = GENL_ID_GENERATE, /* To generate an id for the family*/
	.hdrsize = 0,
	.name = "eth_drv_notify", /*family name, used by userspace application*/
	.version = 1, /*version number  */
	.maxattr = ETH_LINK_A_MAX,
};

/* VRX318 TC message multicast group */
struct genl_multicast_group eth_grp= {
    .name = "eth_mcgrp", 
};

/**
 * API defintion for the driver to send TC notify messages to user application 
 * using genetlink method.
 * pid: process id
 * tc_msg: 1-link is up,0- link is down
 * ln_no: interface name
 */
int ltq_eth_nl_msg_send(int pid, int link_status, int speed, char *if_name)
{
	struct sk_buff *skb;
	int ret;
	void *msg_head;

	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	/* create the message headers */
	msg_head = genlmsg_put(skb, 0, 0,
			&eth_gnl_family, 0, ETH_C_NOTIFY);
	if (msg_head == NULL) {
		ret = -ENOMEM;
		pr_err("create message header fail!\n");
		goto out;
	}
	
	nla_put_string(skb, ETH_A_LINK_NAME, if_name);
	if (link_status) {
		nla_put_string(skb, ETH_A_LINK_STATUS, "up");
		nla_put_u16(skb, ETH_A_LINK_SPEED, speed);
	}
	else
		nla_put_string(skb, ETH_A_LINK_STATUS, "down");

	genlmsg_end(skb, msg_head);
	ret = genlmsg_multicast(skb, pid, eth_grp.id, GFP_KERNEL);
	if (ret != 0 && ret != -ESRCH) {
		pr_err("failed to send out the multicast message with ret = %d\n", ret);
		goto out;
	}
	return 0;
out:
	return ret;
}

int ltq_eth_genetlink_init(void)
{
	int ret;

	/*register new family*/
	ret = genl_register_family(&eth_gnl_family);
	if (ret != 0) {
		pr_err("Family registeration fail:%s\n",
			eth_gnl_family.name);
		goto failure;
	}

	ret = genl_register_mc_group(&eth_gnl_family, &eth_grp);
	if (ret != 0) {
		pr_err("register mc group fail: %i, grp name: %s\n",
			ret, eth_grp.name);
		genl_unregister_family(&eth_gnl_family);
		goto failure;
	} else
		pr_info("register mc group pass: %i, grp name: %s, grp id:%d\n",
			ret, eth_grp.name, eth_grp.id);

	return 0;

failure:
	return ret;
}

void ltq_eth_genetlink_exit(void)
{
	int ret;

	/* unregister mc groups */
	genl_unregister_mc_group(&eth_gnl_family, &eth_grp);

	/*unregister the family*/
	ret = genl_unregister_family(&eth_gnl_family);
	if(ret != 0)
		pr_err("unregister Genetlink family %i\n", ret);

	return;
}

#endif

#ifdef CONFIG_XRX500_MDIO_SUPPORT
static void xrx500_of_port(ltq_switch_priv_t *priv, struct device_node *port)
{
	const __be32 *addr, *id = of_get_property(port, "reg", NULL);
	struct xrx500_port *p = &priv->port[priv->num_port];
	struct resource irqres;

	if (!id)
		return;

	memset(p, 0, sizeof(struct xrx500_port));
	p->phy_node = of_parse_phandle(port, "phy-handle", 0);
	addr = of_get_property(p->phy_node, "reg", NULL);
	if (!addr) {
		pr_info("no real internal PHY attached to this interface !\n");
		return;
	}

	p->num = *id;
	p->phy_addr = *addr;
	p->phy_if = of_get_phy_mode(port);

	if (of_irq_to_resource_table(port, &irqres, 1) == 1) {
		p->irq_num = irqres.start;
	} else {
		pr_err("couldn't get irq number for gphy !!\n");
	}
	priv->num_port++;

	/* is this port a wan port ? */
	if (priv->wan)
		priv->hw->wan_map |= BIT(p->num);

	priv->port_map |= BIT(p->num);

	/* store the port id in the hw struct so we can map ports -> devices */
	priv->hw->port_map[p->num] = priv->hw->num_devs;
}

/* This function is event handler for net_device change notify */

static int phy_netdevice_event(struct notifier_block *nb, unsigned long action, void *ptr)
{
	ltq_switch_priv_t *priv;
	struct net_device *dev;

	dev = NULL;
	priv = NULL;

	if ( ptr == NULL )
		return 0;

	dev = (struct net_device *)ptr;

	switch (action) {
		case NETDEV_CHANGENAME:
			{
				priv = netdev_priv(dev);

				if (priv != NULL && priv->wan) {
					pr_debug("\nUpdate WAN iface from [%s] to [%s]\n",
							wan_iface, dev->name);
					memcpy(wan_iface, dev->name, IFNAMSIZ);
				}
			}
	}

	return NOTIFY_OK;
}

static int xrx500_of_iface(struct xrx500_hw *hw, struct device_node *iface)
{
	ltq_switch_priv_t *priv;
	struct device_node *port;
	const __be32 *wan;
    dp_cb_t cb={0};
    u32 dp_port_id = 0;
    char name[16];

	/* alloc the network device */
	hw->devs[hw->num_devs] = alloc_etherdev(sizeof(ltq_switch_priv_t));
	if (!hw->devs[hw->num_devs])
		return -ENOMEM;

	priv = netdev_priv(hw->devs[hw->num_devs]);

	/* is this the wan interface ? */
	wan = of_get_property(iface, "lantiq,wan", NULL);
	if (wan && (*wan == 1))
		priv->wan = 1;
	else
		priv->wan = 0;

	/* setup the network device */
	if (priv->wan)
		sprintf(name, wan_iface);
	else
		sprintf(name, "eth0_%d", hw->num_devs);
	strcpy(hw->devs[hw->num_devs]->name, name);
	hw->devs[hw->num_devs]->netdev_ops = &ltq_eth_drv_ops;
	hw->devs[hw->num_devs]->watchdog_timeo = LTQ_TX_TIMEOUT;
	hw->devs[hw->num_devs]->needed_headroom = sizeof(ltq_pmac_header_t);

	/* setup our private data */
	priv->hw = hw;
	priv->id = hw->num_devs;
	spin_lock_init(&priv->lock);

	priv->owner = &g_ltq_eth_module[hw->num_devs];
	sprintf(priv->owner->name, "module%02d", priv->id);
	if (priv->wan) {
		dp_port_id  = dp_alloc_port(priv->owner, hw->devs[hw->num_devs], 15, 15, NULL, DP_F_FAST_ETH_WAN);
	} else {
		dp_port_id  = dp_alloc_port(priv->owner, hw->devs[hw->num_devs], priv->id+1, priv->id+1, NULL, DP_F_FAST_ETH_LAN);
	}

	if(dp_port_id == DP_FAILURE) {
		pr_err("dp_dealloc_port failed for %s with port_id %d\n",
										hw->devs[hw->num_devs]->name, priv->id+1);
		return -ENODEV;
	}
	priv->dp_port_id = dp_port_id;
	cb.stop_fn   =(dp_stop_tx_fn_t ) dp_fp_stop_tx;
	cb.restart_fn  = (dp_restart_tx_fn_t )dp_fp_restart_tx;
	cb.rx_fn        = (dp_rx_fn_t )dp_fp_rx;
#ifdef CONFIG_XRX500_ETH_DRV_COC_SUPPORT
	cb.dp_coc_confirm_stat_fn = (dp_coc_confirm_stat) dp_fp_coc_confirm;
#endif
	if (dp_register_dev (priv->owner,  dp_port_id, &cb, 0) != DP_SUCCESS) {
		pr_err("dp_register_dev failed for %s\n and port_id %d", hw->devs[hw->num_devs]->name, dp_port_id);
		dp_alloc_port (priv->owner, hw->devs[hw->num_devs], priv->id, priv->id+1, NULL, DP_F_DEREGISTER);
		return -ENODEV;
	}   

	/* load the ports that are part of the interface */
	for_each_child_of_node(iface, port)
		if (of_device_is_compatible(port, "lantiq,xrx500-pdi-port"))
			xrx500_of_port(priv, port);

#ifdef CONFIG_LTQ_TOE_DRIVER
	hw->devs[hw->num_devs]->features = NETIF_F_SG | NETIF_F_TSO | NETIF_F_HW_CSUM;
	hw->devs[hw->num_devs]->hw_features = NETIF_F_SG | NETIF_F_TSO | NETIF_F_HW_CSUM;
	hw->devs[hw->num_devs]->vlan_features = NETIF_F_SG | NETIF_F_TSO | NETIF_F_HW_CSUM;
	hw->devs[hw->num_devs]->gso_max_size  = GSO_MAX_SIZE;
#else
	hw->devs[hw->num_devs]->features = NETIF_F_SG | NETIF_F_HW_CSUM;
	hw->devs[hw->num_devs]->hw_features = NETIF_F_SG | NETIF_F_HW_CSUM;
#endif

	if (g_rx_csum_offload) {
		pr_info("%s: rx csum offload is enabled !\n", __func__);
		hw->devs[hw->num_devs]->features |= NETIF_F_RXCSUM;
		hw->devs[hw->num_devs]->hw_features |= NETIF_F_RXCSUM;
	} else {
		pr_info("%s: rx csum offload is disabled !\n", __func__);
	}

	ltq_eth_drv_eth_addr_setup(hw->devs[hw->num_devs], priv->id);

	/* register the actual device */
	if (!register_netdev(hw->devs[hw->num_devs])){
#ifdef CONFIG_OFFLOAD_FWD_MARK
		if(strncmp(hw->devs[hw->num_devs]->name, wan_iface, IFNAMSIZ) == 0)
			hw->devs[hw->num_devs]->offload_fwd_mark = SWITCH_ID2;
		else
			hw->devs[hw->num_devs]->offload_fwd_mark = SWITCH_ID;
#endif
		hw->num_devs++;
	}
	return 0;
}

static int xrx500_mdio_pae_wr(struct mii_bus *bus, int addr, int reg, u16 val)
{
	GSW_MDIO_data_t mmdData;
	GSW_API_HANDLE gswr;

	pr_debug("%s called with phy addr:%d and reg: %x and val: %x\n", __func__, addr, reg, val);

	gswr = gsw_api_kopen("/dev/switch_api/1");
	if (gswr == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}

	memset((void *)&mmdData, 0x00, sizeof(mmdData));
	mmdData.nAddressDev = addr;
	mmdData.nAddressReg = reg;
	mmdData.nData = val;
	gsw_api_kioctl(gswr, GSW_MDIO_DATA_WRITE, (unsigned int)&mmdData);

	gsw_api_kclose(gswr);

	return 0;
}

static int xrx500_mdio_pae_rd(struct mii_bus *bus, int addr, int reg)
{
	GSW_MDIO_data_t mmdData;
	GSW_API_HANDLE gswr;

	pr_debug("%s called with phy addr:%d and reg: %x\n", __func__, addr, reg);

	gswr = gsw_api_kopen("/dev/switch_api/1");
	if (gswr == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}

	memset((void *)&mmdData, 0x00, sizeof(mmdData));
	mmdData.nAddressDev = addr;
	mmdData.nAddressReg = reg;
	gsw_api_kioctl(gswr, GSW_MDIO_DATA_READ, (unsigned int)&mmdData);

	pr_debug("returing: %x \n", mmdData.nData);
	gsw_api_kclose(gswr);
	return(mmdData.nData);
}

static int xrx500_mdio_wr(struct mii_bus *bus, int addr, int reg, u16 val)
{
	GSW_MDIO_data_t mmdData;
	GSW_API_HANDLE gswl;

	pr_debug("%s called with phy addr:%d and reg: %x and val: %x\n", __func__, addr, reg, val);

	gswl = gsw_api_kopen("/dev/switch_api/0");
	if (gswl == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}

	memset((void *)&mmdData, 0x00, sizeof(mmdData));
	mmdData.nAddressDev = addr;
	mmdData.nAddressReg = reg;
	mmdData.nData = val;
	gsw_api_kioctl(gswl, GSW_MDIO_DATA_WRITE, (unsigned int)&mmdData);

	gsw_api_kclose(gswl);

	return 0;
}

static int xrx500_mdio_rd(struct mii_bus *bus, int addr, int reg)
{
	GSW_MDIO_data_t mmdData;
	GSW_API_HANDLE gswl;

	pr_debug("%s called with phy addr:%d and reg: %x\n", __func__, addr, reg);

	gswl = gsw_api_kopen("/dev/switch_api/0");
	if (gswl == 0) {
		pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
		return -EIO;
	}

	memset((void *)&mmdData, 0x00, sizeof(mmdData));
	mmdData.nAddressDev = addr;
	mmdData.nAddressReg = reg;
	gsw_api_kioctl(gswl, GSW_MDIO_DATA_READ, (unsigned int)&mmdData);

	pr_debug("returing: %x \n", mmdData.nData);
	gsw_api_kclose(gswl);
	return(mmdData.nData);
}

static int xrx500_of_mdio(struct xrx500_hw *hw, struct device_node *np)
{
	hw->mii_bus = mdiobus_alloc();
	if (!hw->mii_bus)
		return -ENOMEM;

	hw->mii_bus->read = xrx500_mdio_rd;
	hw->mii_bus->write = xrx500_mdio_wr;
	hw->mii_bus->name = "lantiq,xrx500-mdio";
	snprintf(hw->mii_bus->id, MII_BUS_ID_SIZE, "%x", 0);

	pr_info("registering one of MII bus\n");
	if (of_mdiobus_register(hw->mii_bus, np)) {
		mdiobus_free(hw->mii_bus);
		return -ENXIO;
	}

	return 0;
}

static int xrx500_of_mdio_pae(struct xrx500_hw *hw, struct device_node *np)
{
	hw->mii_bus_pae = mdiobus_alloc();
	if (!hw->mii_bus_pae)
		return -ENOMEM;

	hw->mii_bus_pae->read = xrx500_mdio_pae_rd;
	hw->mii_bus_pae->write = xrx500_mdio_pae_wr;
	hw->mii_bus_pae->name = "lantiq,xrx500-mdio-pae";
	snprintf(hw->mii_bus_pae->id, MII_BUS_ID_SIZE, "%x", 1);

	pr_info("registering PAE MII bus\n");
	if (of_mdiobus_register(hw->mii_bus_pae, np)) {
		mdiobus_free(hw->mii_bus_pae);
		return -ENXIO;
	}

	return 0;
}
#ifdef SW_POLLING
static void xrx500_gmac_update(struct xrx500_port *port)
{
	u16 phyaddr = port->phydev->addr & MDIO_PHY_ADDR_MASK;

	if (port->phydev->link)
		phyaddr |= MDIO_PHY_LINK_UP;
	else
		phyaddr |= MDIO_PHY_LINK_DOWN;

	if (port->phydev->duplex == DUPLEX_FULL)
		phyaddr |= MDIO_PHY_FDUP_EN;
	else
		phyaddr |= MDIO_PHY_FDUP_DIS;

	udelay(1);
}
#else
static void xrx500_gmac_update(struct xrx500_port *port)
{
}
#endif
static void xrx500_mdio_link(struct net_device *dev)
{
	ltq_switch_priv_t *priv = netdev_priv(dev);
	int i;

	pr_debug("%s called..\n", __func__);
	for (i = 0; i < priv->num_port; i++) {
		if (!priv->port[i].phydev)
			continue;

		if (priv->port[i].link != priv->port[i].phydev->link) {
			xrx500_gmac_update(&priv->port[i]);
			priv->port[i].link = priv->port[i].phydev->link;
			/*netdev_info(dev, "port %d %s link\n",
				priv->port[i].num,
				(priv->port[i].link)?("got"):("lost"));*/
#ifdef CONFIG_USERSPACE_LINK_NOTIFICATION
				ltq_eth_nl_msg_send(0, priv->port[i].link, priv->port[i].phydev->speed, dev->name);
#endif
			 phy_print_status(priv->port[i].phydev);
		}
	}
}

static int xrx500_mdio_probe(struct net_device *dev, struct xrx500_port *port)
{
	ltq_switch_priv_t *priv = netdev_priv(dev);
	struct phy_device *phydev = NULL;

	if(priv->wan)
		phydev = priv->hw->mii_bus_pae->phy_map[port->phy_addr];
	else
		phydev = priv->hw->mii_bus->phy_map[port->phy_addr];

	if (!phydev) {
		netdev_err(dev, "no PHY found\n");
		return -ENODEV;
	}

	pr_info("trying to connect: %s to device: %s with irq: %d\n",
				dev->name, dev_name(&phydev->dev), port->irq_num);
	phydev->irq = port->irq_num;
	irq_set_status_flags(phydev->irq, IRQ_NOAUTOEN);
	phydev = phy_connect(dev, dev_name(&phydev->dev), &xrx500_mdio_link,
				port->phy_if);

	if (IS_ERR(phydev)) {
		netdev_err(dev, "Could not attach to PHY\n");
		return PTR_ERR(phydev);
	}
	enable_irq(phydev->irq);
	phydev->supported &= (SUPPORTED_10baseT_Half
			| SUPPORTED_10baseT_Full
			| SUPPORTED_100baseT_Half
			| SUPPORTED_100baseT_Full
			| SUPPORTED_1000baseT_Half
			| SUPPORTED_1000baseT_Full
			| SUPPORTED_Autoneg
			| SUPPORTED_MII
			| SUPPORTED_TP);
	phydev->advertising = phydev->supported;
	port->phydev = phydev;

	pr_info("%s: attached PHY [%s] (phy_addr=%s, irq=%d)\n",
		dev->name, phydev->drv->name,
		dev_name(&phydev->dev), phydev->irq);

	phy_read_status(phydev);

#if 0
	val = xrx500_mdio_rd(priv->hw->mii_bus, MDIO_DEVAD_NONE, MII_CTRL1000);
	val |= ADVERTIZE_MPD;
	xrx500_mdio_wr(priv->hw->mii_bus, MDIO_DEVAD_NONE, MII_CTRL1000, val);
	xrx500_mdio_wr(priv->hw->mii_bus, 0, 0, 0x1040);
#endif

	phy_start_aneg(phydev);
	return 0;
}

#endif

#ifdef CONFIG_XRX500_ETH_DRV_COC_SUPPORT
/* switch api related routines */
static void __iomem *ltq_eth_gswl_base = (void __iomem *) KSEG1ADDR(0x1c000000);	/*Base address GSWIP-L */
static void __iomem *ltq_eth_gswr_base = (void __iomem *) KSEG1ADDR(0x1a000000);  /* Base address GSWIP-R */

/** read the gswitch register */
static void ltq_eth_gsw_r32(void __iomem *gsw_base, short offset, short shift, short size, u32 *value)
{
	u32 rvalue, mask;
	rvalue = ltq_r32(gsw_base + (offset * 4));
	mask = (1 << size) - 1;
	rvalue = (rvalue >> shift);
	*value = (rvalue & mask);
}

/** read and update the GSWIP register */
static void ltq_eth_gsw_w32(void __iomem *gsw_base, short offset, short shift, short size, u32 value)
{
	u32 rvalue, mask;
	rvalue = ltq_r32(gsw_base + (offset * 4));
	mask = (1 << size) - 1;
	mask = (mask << shift);
	value = ((value << shift) & mask);
	value = ((rvalue & ~mask) | value);
	ltq_w32(value, (gsw_base + (offset * 4)));
}

static int32_t dp_fp_coc_confirm(enum ltq_cpufreq_state new_state, 
								enum ltq_cpufreq_state old_state, u32 flags)
{
	GSW_API_HANDLE gswl;
	GSW_API_HANDLE gswr;
	GSW_register_t regCfg;

	if ((new_state != g_ltq_eth_drv_coc_state ) && (new_state != LTQ_CPUFREQ_PS_D0)) {

		pr_debug("%s: CoC state changed from : %d to : %d enable irq !\n", __func__, g_ltq_eth_drv_coc_state, new_state);

		/* Do the GSW-L configuration */
		gswl = gsw_api_kopen("/dev/switch_api/0");

		if (gswl == 0) {
			pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
			return -EIO;
		}

		/* Clear the BM interrupt */
		regCfg.nRegAddr = 0x55;
		regCfg.nData = 0x100;
		if (gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		/* Enable the BM interrupt */
		regCfg.nRegAddr = 0x14;
		regCfg.nData = 0;
		if (gsw_api_kioctl(gswl, GSW_REGISTER_GET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		regCfg.nData |= 0x1;
		if (gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		/* Enable the QCGN interrupt */
		regCfg.nRegAddr = 0x54;
		regCfg.nData = 0;
		if (gsw_api_kioctl(gswl, GSW_REGISTER_GET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		regCfg.nData |= 0x100;
		if (gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		/*enable_irq(g_ltq_eth_gswl_irq);*/
		gsw_api_kclose(gswl);

		/* Do the PAE configuration */
		gswr = gsw_api_kopen("/dev/switch_api/1");

		if (gswr == 0) {
			pr_err("%s: Open SWAPI device FAILED !!\n", __func__ );
			return -EIO;
		}

		/* Clear all the pending metering interrupt */
		regCfg.nRegAddr = 0x489;
		regCfg.nData = 0x100;
		if (gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		/* Enable the PCE interrupt */
		regCfg.nRegAddr = 0x14;
		regCfg.nData = 0;
		if (gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		regCfg.nData |= 0x2;
		if (gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		/* Enable PCE interrupt for port 0 */
		regCfg.nRegAddr = 0x465;
		regCfg.nData = 0;
		if (gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		regCfg.nData |= 0x1;
		if (gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		/* Enable metering interupt inside PCE */
		regCfg.nRegAddr = 0x488;
		regCfg.nData = 0;
		if (gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		regCfg.nData |= 0x100;
		if (gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
			pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

		gsw_api_kclose(gswr);
		
	} else if ((new_state == LTQ_CPUFREQ_PS_D0) && (g_ltq_eth_drv_coc_state != new_state)) {
			pr_debug("%s: CoC state changed from : %d to : %d disable irq, do nothing !\n",
						__func__, g_ltq_eth_drv_coc_state, new_state);
			/*disable_irq(g_ltq_eth_gswl_irq);*/
	} else {
		pr_debug("duplicate call to confirm to new state: %d..!\n", new_state);
	}

	g_ltq_eth_drv_coc_state =  new_state;
	return 0;
}

static void ltq_eth_gswl_tasklet(unsigned long ptr)
{
	GSW_register_t regCfg;
	GSW_API_HANDLE gswl;
	u32 intr_type, irq_status;

	pr_debug("tasklet called..\n");
	/* Do the GSW-L configuration */
	gswl = gsw_api_kopen("/dev/switch_api/0");
	if (gswl == 0) {
		pr_err("%s: open swapi device failed !!\n", __func__ );
		return;
	}

	while (!bitmap_empty(g_ltq_eth_intr_type, LTQ_ETH_NUM_INTERRUPTS)) {
			intr_type = find_first_bit(g_ltq_eth_intr_type, LTQ_ETH_NUM_INTERRUPTS);

			/* Read the status */
			switch (intr_type) {
			case 1:
				regCfg.nRegAddr = 0x55;
				regCfg.nData = 0;
				if (gsw_api_kioctl(gswl, GSW_REGISTER_GET, (unsigned int)&regCfg) < 0)
					pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);
				irq_status = regCfg.nData;
				pr_debug("BM interrupt with status: %x!\n", irq_status);

				/* Egress Queue Congestion Status Change */
				if (irq_status & 0x100) {

					/* Mask the interrupt */
					regCfg.nRegAddr = 0x54;
					regCfg.nData = 0;
					if (gsw_api_kioctl(gswl, GSW_REGISTER_GET, (unsigned int)&regCfg) < 0)
						pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

					regCfg.nData &= ~0x100;
					if (gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
						pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

					/* Clear all the interrupts */
					regCfg.nRegAddr = 0x55;
					regCfg.nData = irq_status;
					if (gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
								pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

					/* Clear at the top level */
					regCfg.nRegAddr = 0x15;
					regCfg.nData = 1;
					if (gsw_api_kioctl(gswl, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
								pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

					gsw_api_kclose(gswl);

					/* There is some traffic, so call the datapth library
					 * to change the power state to upscale */
					dp_coc_new_stat_req(LTQ_CPUFREQ_PS_D0, DP_COC_REQ_ETHERNET);

				} else {
					pr_err("non QCGN interrupts from BM are not handled !");
				}

				clear_bit(1, g_ltq_eth_intr_type);
				break;

			default:
				pr_err("%s: currently this irq is not handled..\n", __func__);
				break;
			}
	}
		
}

static irqreturn_t ltq_eth_gswl_isr(int irq, void *dev_id)
{
	u32 irq_status, irq_mask;

	ltq_eth_gsw_r32(ltq_eth_gswl_base, 0x15, 0, 16, &irq_status);

	/* Buffer Manager Interrupt */
	if (irq_status & 0x1) {

		/* Mask the BM Interrupt */
		ltq_eth_gsw_r32(ltq_eth_gswl_base, 0x14, 0, 16, &irq_mask);

		irq_mask &= ~0x1;
		ltq_eth_gsw_w32(ltq_eth_gswl_base, 0x14, 0, 16, irq_mask);

		set_bit(1, g_ltq_eth_intr_type);

		/* Schedule the tasklet */
		tasklet_schedule(&gswl_tasklet);
	} else {
		pr_err("%s: ..........: %x", __func__, irq_status);
	}
	return IRQ_HANDLED;
}

/* PAE interrupt related */

static irqreturn_t ltq_eth_pae_isr(int irq, void *dev_id)
{
	u32 irq_status, irq_mask;

	ltq_eth_gsw_r32(ltq_eth_gswr_base, 0x15, 0, 16, &irq_status);

	/* PCE interrupt */
	if (irq_status & 0x2) {

		ltq_eth_gsw_r32(ltq_eth_gswr_base, 0x14, 0, 16, &irq_mask);

		irq_mask &= ~0x2;
		ltq_eth_gsw_w32(ltq_eth_gswr_base, 0x14, 0, 16, irq_mask);

		set_bit(2, g_ltq_pae_intr_type);

		/* Schedule the tasklet */
		tasklet_schedule(&pae_tasklet);
	} else {
		pr_err("%s: ..........: %x", __func__, irq_status);
	}
	return IRQ_HANDLED;
}

static void ltq_eth_pae_tasklet(unsigned long ptr)
{
	GSW_register_t regCfg;
	GSW_API_HANDLE gswr;
	u32 intr_type, pce_irq_status;

	pr_debug("PAE tasklet called..\n");
	gswr = gsw_api_kopen("/dev/switch_api/1");
	if (gswr == 0) {
		pr_err("%s: open swapi device failed !!\n", __func__ );
		return;
	}

	while (!bitmap_empty(g_ltq_pae_intr_type, LTQ_ETH_NUM_INTERRUPTS)) {
			intr_type = find_first_bit(g_ltq_pae_intr_type, LTQ_ETH_NUM_INTERRUPTS);

			switch (intr_type) {
			/* PCE interrupt */
			case 2:

				/* Find out the port */
				regCfg.nRegAddr = 0x467;
				regCfg.nData = 0;
				if (gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg) < 0)
					pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);
				pr_debug("PCE isr_0: %x!\n", regCfg.nData);
				
				/* Support only for port 0 */
				if (regCfg.nData & 0x1) {

						regCfg.nRegAddr = 0x489;
						regCfg.nData = 0;
						if (gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg) < 0)
								pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);
						pce_irq_status = regCfg.nData;
						pr_debug("PCE isr_0 internal: %x!\n", pce_irq_status);

						/* Metering based bp status change interrupt */
						if (pce_irq_status & 0x100) {

							/* Mask the interrupt */
							regCfg.nRegAddr = 0x488;
							regCfg.nData = 0;
							if (gsw_api_kioctl(gswr, GSW_REGISTER_GET, (unsigned int)&regCfg) < 0)
								pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

							regCfg.nData &= ~0x100;
							if (gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
								pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

							/* Clear all the interrupts */
							regCfg.nRegAddr = 0x467;
							regCfg.nData = 1;
							if (gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
								pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

							regCfg.nRegAddr = 0x489;
							regCfg.nData = pce_irq_status;
							if (gsw_api_kioctl(gswr, GSW_REGISTER_SET, (unsigned int)&regCfg) < 0)
										pr_err("%s:%d gsw ioctl failed !!\n", __func__, __LINE__);

							gsw_api_kclose(gswr);

							/* There is some traffic, so call the datapth library
							 * to change the power state to upscale */
							dp_coc_new_stat_req(LTQ_CPUFREQ_PS_D0, DP_COC_REQ_ETHERNET);

						} else {
							pr_err("non metering interrupts from PCE are not handled !");
							break;
						}

						clear_bit(2, g_ltq_pae_intr_type);
				} else {
					pr_err("%s: currently only port 0 irq is handled..\n", __func__);
					return;
				}
				break;
			default:
				pr_err("%s: currently this irq is not handled..\n", __func__);
				break;
		}
	}
}
#endif

/* Initilization  Ethernet module */
static int ltq_eth_drv_init(struct platform_device *pdev)
{
    char ver_str[128] = {0};
#ifndef CONFIG_XRX500_MDIO_SUPPORT
    dp_cb_t cb={0};
    u32 dp_port_id = 0;
#endif
	struct device_node *node = pdev->dev.of_node;
#ifdef CONFIG_XRX500_MDIO_SUPPORT
	struct device_node *mdio_np, *iface_np;
#endif
#ifdef CONFIG_XRX500_ETH_DRV_COC_SUPPORT
	struct resource irqres[2];
#endif

    memset( g_ltq_eth_module, 0, sizeof(g_ltq_eth_module));

    /* HW init of the Switch */
    if (ltq_gsw_pmac_init() < 0)
		printk ("%s[%d]: switch PMAC init failed..\n", __func__, __LINE__);

	if (of_property_read_u32(node, "lantiq,eth-rx-csum-offload", &g_rx_csum_offload) < 0)
		dev_info(&(pdev->dev),
		"couldn't get the RX CSUM offload setting from DT, by default its disabled !\n");

#ifdef CONFIG_XRX500_MDIO_SUPPORT
	/* bring up the mdio bus */
	mdio_np = of_find_compatible_node(node, NULL,
				"lantiq,xrx500-mdio");
	if (mdio_np)
		if (xrx500_of_mdio(&xrx500_hw, mdio_np))
			dev_err(&pdev->dev, "mdio probe failed\n");

	/* bring up the mdio bus for PAE */
	mdio_np = of_find_compatible_node(node, NULL,
				"lantiq,xrx500-mdio-pae");
	if (mdio_np)
		if (xrx500_of_mdio_pae(&xrx500_hw, mdio_np))
			dev_err(&pdev->dev, "mdio probe of PAE failed\n");

	/* load the interfaces */
	/* add a dummy interface */
	xrx500_hw.num_devs = 0;
	for_each_child_of_node(node, iface_np)
		if (of_device_is_compatible(iface_np, "lantiq,xrx500-pdi")) {
			pr_info("adding the interface: %d\n", xrx500_hw.num_devs);
			if (xrx500_hw.num_devs < NUM_ETH_INF)
				xrx500_of_iface(&xrx500_hw, iface_np);
			else
				dev_err(&pdev->dev,
					"only %d interfaces allowed\n",
					NUM_ETH_INF);
		}

	if (!xrx500_hw.num_devs) {
		dev_err(&pdev->dev, "failed to load interfaces\n");
		return -ENOENT;
	}
#else
    ltq_switch_priv_t* priv;
    int  i,  err;

    for (i = 0; i < NUM_ETH_INF ; i++) {
        	char name[16];

			if (i == 6)
					sprintf(name, wan_iface);
			else
					sprintf(name, "eth0_%d", i);

				eth_dev[i] = alloc_etherdev(sizeof(ltq_switch_priv_t));
				if (!eth_dev[i]) {
					printk(KERN_ERR "%s[%d]: no memory for eth_dev!!!\n", __func__,__LINE__);
					err = -ENOMEM;
					goto err_out_free_res;
				}

			/* setup the network device */
			strcpy(eth_dev[i]->name, name);
			eth_dev[i]->netdev_ops = &ltq_eth_drv_ops;
			eth_dev[i]->watchdog_timeo = LTQ_TX_TIMEOUT;
			eth_dev[i]->needed_headroom = sizeof(ltq_pmac_header_t);
			/*eth_dev[i]->ifindex = i+1;*/

			/* setup the private data */
				priv = netdev_priv(eth_dev[i]);
				priv->phy_addr = i;

				/* By default, advertise supported  speed/duplex settings. */
				priv->flags |= (FLAG_ADV_10HALF         \
								| FLAG_ADV_10FULL       \
								| FLAG_ADV_100HALF      \
								| FLAG_ADV_100FULL      \
								| FLAG_ADV_1000HALF     \
								| FLAG_ADV_1000FULL);

				/* By default, auto-negotiate PAUSE. */
				priv->flags |= FLAG_PAUSE_AUTO;
				spin_lock_init(&priv->lock);

			priv->owner = &g_ltq_eth_module[i];
			sprintf(priv->owner->name, "module%02d", i);
			if (i == 6) {
				dp_port_id  = dp_alloc_port(priv->owner, eth_dev[i], 15, 15, NULL, DP_F_FAST_ETH_WAN);
			} else {
				dp_port_id  = dp_alloc_port(priv->owner, eth_dev[i], i+1, i+1, NULL, DP_F_FAST_ETH_LAN);
			}

			if(dp_port_id == DP_FAILURE) {
				pr_err("dp_dealloc_port failed for %s with port_id %d\n",
												eth_dev[i]->name, i+1);
				return -ENODEV;
			}
			priv->dp_port_id = dp_port_id;
			cb.stop_fn   =(dp_stop_tx_fn_t ) dp_fp_stop_tx;
			cb.restart_fn  = (dp_restart_tx_fn_t )dp_fp_restart_tx;
			cb.rx_fn        = (dp_rx_fn_t )dp_fp_rx;
#ifdef CONFIG_XRX500_ETH_DRV_COC_SUPPORT
			cb.dp_coc_confirm_stat_fn = (dp_coc_confirm_stat) dp_fp_coc_confirm;
#endif
			if (dp_register_dev (priv->owner,  dp_port_id, &cb, 0) != DP_SUCCESS) {
								pr_err("dp_register_dev failed for %s\n and port_id %d", eth_dev[i]->name, dp_port_id);
								dp_alloc_port (priv->owner, eth_dev[i], i, i+1, NULL, DP_F_DEREGISTER);
								return -ENODEV;
				}   
#ifdef CONFIG_LTQ_TOE_DRIVER
			eth_dev[i]->features = NETIF_F_SG | NETIF_F_TSO | NETIF_F_HW_CSUM;
			eth_dev[i]->hw_features = NETIF_F_SG | NETIF_F_TSO | NETIF_F_HW_CSUM;
			eth_dev[i]->vlan_features = NETIF_F_SG | NETIF_F_TSO | NETIF_F_HW_CSUM;
			eth_dev[i]->gso_max_size  = GSO_MAX_SIZE;
#endif

			if (g_rx_csum_offload) {
				pr_info("%s: rx csum offload is enabled !\n", __func__);
				eth_dev[i]->features |= NETIF_F_RXCSUM;
				eth_dev[i]->hw_features |= NETIF_F_RXCSUM;
			} else {
				pr_info("%s: rx csum offload is disabled !\n", __func__);
			}

			ltq_eth_drv_eth_addr_setup(eth_dev[i], i);
			err = register_netdev(eth_dev[i]);
			if ( err ) {
				printk(KERN_ERR "%s[%d]: Register with network device failed with err:%d for device: %s !!!\n", __func__,__LINE__, err, name);
				goto err_out_free_res;
			}
	}

#endif

#ifdef CONFIG_USERSPACE_LINK_NOTIFICATION
	/* Register the netlink notification */
	ltq_eth_genetlink_init();
#endif

#ifdef CONFIG_XRX500_ETH_DRV_COC_SUPPORT
	if (of_irq_to_resource_table(node, irqres, 2) == 2) {

			/* GSW-L interrupt */
			if (request_irq(irqres[0].start, ltq_eth_gswl_isr, IRQF_DISABLED, "gswl_irq", NULL)) {
				pr_err("%s: failed to request gswl irq - ", __func__);
				goto err1;
			}
			tasklet_init(&gswl_tasklet, ltq_eth_gswl_tasklet, 0);
			bitmap_zero(g_ltq_eth_intr_type, LTQ_ETH_NUM_INTERRUPTS);
			/* Keep the interrupt disabled till the low power mode */
			/*disable_irq(irqres.start);*/
			g_ltq_eth_gswl_irq = irqres[0].start;

			/* PAE interrupt */
			if (request_irq(irqres[1].start, ltq_eth_pae_isr, IRQF_DISABLED, "pae_irq", NULL)) {
				pr_err("%s: failed to request pae irq - ", __func__);
				goto err1;
			}
			tasklet_init(&pae_tasklet, ltq_eth_pae_tasklet, 0);
			bitmap_zero(g_ltq_pae_intr_type, LTQ_ETH_NUM_INTERRUPTS);
			g_ltq_pae_irq = irqres[1].start;
	} else {
		pr_err("%s: couldn't get irq from device tree for CoC. \n", __func__);
	}
#endif

err1:
 /* Print the driver version info */
    eth_drv_ver(ver_str);
    printk(KERN_INFO "%s", ver_str);
    return 0;

#ifndef CONFIG_XRX500_MDIO_SUPPORT
err_out_free_res:
    /* unregister the network devices */
    for (i=0; i< NUM_ETH_INF ; i++) {
        if(eth_dev[i])
            free_netdev(eth_dev[i]);
    }
    return err;
#endif
}

static void  ltq_eth_drv_exit (void)
{
    int i;

    /* unregister the network devices */
    for (i=0; i< NUM_ETH_INF ; i++) {
        unregister_netdev(eth_dev[i]);
        free_netdev(eth_dev[i]);
    }
#ifdef CONFIG_USERSPACE_LINK_NOTIFICATION
	ltq_eth_genetlink_exit();
#endif
}

static int ltq_eth_drv_probe(struct platform_device *pdev)
{
	/* Just do the init */
	ltq_eth_drv_init(pdev);
	register_netdevice_notifier(&netdevice_notifier);

	return 0;
}

static int ltq_eth_drv_remove(struct platform_device *pdev)
{
	/* Just do the exit */
	unregister_netdevice_notifier(&netdevice_notifier);
	ltq_eth_drv_exit ();
	return 0;
}

static const struct of_device_id ltq_eth_drv_match[] = {
	{ .compatible = "lantiq,xrx500-net" },
	{},
};
MODULE_DEVICE_TABLE(of, ltq_eth_drv_match);

static struct platform_driver ltq_eth_driver = {
	.probe = ltq_eth_drv_probe,
	.remove = ltq_eth_drv_remove,
	.driver = {
		.name = "xrx500-net",
		.of_match_table = ltq_eth_drv_match,
		.owner = THIS_MODULE,
	},
};

__setup("ethaddr=", ltq_eth_drv_eth0addr_setup);
__setup("eth1addr=", ltq_eth_drv_eth1addr_setup);

module_platform_driver(ltq_eth_driver);

MODULE_AUTHOR("Suresh Nagaraj");
MODULE_DESCRIPTION("Lantiq ethernet driver for GRX500");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_MODULE_VERSION);
