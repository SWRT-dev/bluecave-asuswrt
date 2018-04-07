/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (C) 2015 Zhu YiXin<yixin.zhu@lantiq.com>
 */
#define DEBUG
#include <linux/ctype.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/jiffies.h>
#include <linux/seq_file.h>
#include <linux/printk.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/completion.h>
#include <net/datapath_api.h>
#include <net/lantiq_cbm.h>
#include <net/lantiq_cbm_api.h>
#include <net/datapath_proc_api.h>
#include <net/vrx318_dsl_api.h>
#include <net/ppa_stack_al.h>

#include <lantiq.h>
#include <lantiq_soc.h>
#include <lantiq_irq.h>
#include <lantiq_pcie.h>
#include <lantiq_ptm.h>

#include "include/vrx318_ppe_fw_const.h"
#include "include/vrx318_edma.h"
#include "include/vrx318_fw_prereq.h"
#include "include/vrx318_ppe_ptm_tc_ds.h"
#include "include/vrx318_ppe_fw_ds.h"
#include "include/vrx318_ppe_bonding_ds.h"
#include "include/unified_qos_ds_be.h"
#include "include/vrx318_e1_addr_def.h"

#include "include/vrx318_common.h"
#include "include/vrx318_framework.h"
#include "include/vrx318_ptm_tc.h"
#include "include/vrx318_api.h"

#include "include/vrx318_proc.h"
#include "fw/vrx318_e1.h"
#include "fw/vrx318_pp32_1.h"


static struct vrx318_ptm_tc *g_ptm_tc;
static const char const *g_ptm_dev_name = "VRX318_PTM_SL";
static const char const *g_bnd_dev_name[2]
	= {"VRX318_US_BONDING", "VRX318_DS_BONDING"};

#ifdef CONFIG_LTQ_VRX318_CPUFREQ
enum ltq_cpufreq_state ptm_coc_state = LTQ_CPUFREQ_PS_D0;
static void do_coc_tasklet(unsigned long);
static DECLARE_TASKLET(g_coc_tasklet, do_coc_tasklet, 0);
#endif

static int ptm_open(struct net_device *);
static int ptm_stop(struct net_device *);
static struct rtnl_link_stats64 *ptm_get_stats (struct net_device *dev,
		struct rtnl_link_stats64 *storage);
static int ptm_set_mac_address(struct net_device *, void *);
static void ptm_tx_timeout(struct net_device *);
static int ptm_ioctl(struct net_device *, struct ifreq *, int);
static int ptm_change_mtu(struct net_device *, int);
static int ptm_xmit(struct sk_buff *, struct net_device *);

static inline void ether_addr_copy(u8 *dst, const u8 *src)
{
#if defined(CONFIG_HAVE_EFFICIENT_UNALIGNED_ACCESS)
	*(u32 *)dst = *(const u32 *)src;
	*(u16 *)(dst + 4) = *(const u16 *)(src + 4);
#else
	u16 *a = (u16 *)dst;
	const u16 *b = (const u16 *)src;

	a[0] = b[0];
	a[1] = b[1];
	a[2] = b[2];
#endif
}

void vrx318_ptm_param_setup(struct vrx318_priv *priv, int *q_map)
{
	int i, j;
	int *q_g_map = priv->param.q_gamma_map;

	for (i = 0; i < ARRAY_SIZE(priv->param.q_gamma_map); i++) {
		q_g_map[i] = q_map[i] & ((1 << 8) - 1);
		for (j = 0; j < i; j++)
			q_g_map[i] &= ~(q_g_map[j]);
	}
}

static int vrx318_bonding_init(struct vrx318_ptm_tc *priv,
				struct vrx318_priv *frwk_priv)
{
	if (!frwk_priv->param.p2p_en) {
		priv->ptm_bnd_cfg.soc_peer_membase
			= get_zeroed_page(GFP_KERNEL);
		if (!priv->ptm_bnd_cfg.soc_peer_membase) {
			vrx318_dbg(DBG_INIT,
				"peer sync memory allocation fail\n");
			return -ENOMEM;
		}
		priv->ptm_bnd_cfg.soc_peer_phybase
			= dma_map_single(frwk_priv->pdev,
				(void *)priv->ptm_bnd_cfg.soc_peer_membase,
				PAGE_SIZE, DMA_TO_DEVICE);
		if (dma_mapping_error(frwk_priv->pdev,
			priv->ptm_bnd_cfg.soc_peer_phybase)) {
			free_page(priv->ptm_bnd_cfg.soc_peer_membase);
			return -EFAULT;
		}
	}

	return 0;
}

static struct net_device_ops g_ptm_netdev_ops = {
	.ndo_open		= ptm_open,
	.ndo_stop		= ptm_stop,
	.ndo_get_stats64	= ptm_get_stats,
	.ndo_set_mac_address	= ptm_set_mac_address,
	.ndo_start_xmit		= ptm_xmit,
	.ndo_tx_timeout		= ptm_tx_timeout,
	.ndo_do_ioctl		= ptm_ioctl,
	.ndo_change_mtu		= ptm_change_mtu,
};

/*TODO: LED RELATED STAFF */
static int ptm_open(struct net_device *dev)
{
	struct vrx318_ptm_tc *priv = netdev_priv(dev);
	dp_subif_t dp_id;

	vrx318_dbg(DBG_EVENT, "ptm open\n");

	dp_id.port_id = priv->frwk->port_id;
	dp_id.subif = -1;

	if (dp_register_subif(priv->frwk->owner, dev,
		dev->name, &dp_id, 0) != DP_SUCCESS) {
		vrx318_dbg(DBG_ERR, "%s: failed to register subif: %s\n",
			__func__, dev->name);
		return -EIO;
	}
	priv->subif_id = dp_id.subif;
	netif_tx_start_all_queues(dev);

	return 0;
}

static int ptm_stop(struct net_device *dev)
{
	struct vrx318_ptm_tc *priv = netdev_priv(dev);
	dp_subif_t dp_id;

	vrx318_dbg(DBG_EVENT, "ptm stop\n");

	dp_id.port_id = priv->frwk->port_id;
	dp_id.subif = priv->subif_id;

	if (dp_register_subif(priv->frwk->owner, dev,
		dev->name, &dp_id, DP_F_DEREGISTER) != DP_SUCCESS) {
		vrx318_dbg(DBG_ERR, "%s: failed to close for device: %s\n",
			__func__, dev->name);
		return -EIO;
	}

	netif_tx_stop_all_queues(dev);
	return 0;
}

static struct rtnl_link_stats64 *ptm_get_stats (struct net_device *dev,
	struct rtnl_link_stats64 *storage)
{
	struct vrx318_ptm_tc *priv;
	
	priv = netdev_priv(dev);
	if (IS_ENABLED(CONFIG_LTQ_DATAPATH_MIB)) {
		dp_get_netif_stats(dev, NULL, storage, 0);
		storage->tx_errors += priv->stats.tx_errors;
	} else 
		*storage = priv->stats;

	return storage;
}

static int ptm_set_mac_address(struct net_device *dev, void *p)
{
	struct sockaddr *addr = (struct sockaddr *)p;

	if (netif_running(dev))
		return -EBUSY;

	vrx318_dbg(DBG_EVENT, "ptm mac address update!\n");
	ether_addr_copy(dev->dev_addr, addr->sa_data);
	return 0;
}

static void ptm_tx_timeout(struct net_device *dev)
{
	struct vrx318_ptm_tc *priv = netdev_priv(dev);

	vrx318_dbg(DBG_EVENT, "PTM tx timeout\n");
	dev->trans_start = jiffies;
	priv->stats.tx_errors++;

	netif_tx_wake_all_queues(dev);
}

static int ptm_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	switch (cmd) {

	default:
		return -EINVAL;
	}

	return 0;
}

/* Change the MTU value of the netwrok device interfaces */
static int ptm_change_mtu(struct net_device *dev, int new_mtu)
{
	if (new_mtu < ETH_ZLEN || new_mtu > (MAX_MTU + PMAC_SIZE - ETH_HLEN))
		return -EINVAL;

	vrx318_dbg(DBG_EVENT, "PTM MTU update: %d\n", new_mtu);
	dev->mtu = new_mtu;

	return 0;
}

static int ptm_showtime_enter(const unsigned char line_idx,
		struct port_cell_info *port_cell, void *xdata_addr)
{
	struct vrx318_priv *priv;

	priv = g_ptm_tc->frwk;
	if (line_idx >= priv->ep_num)
		return -EINVAL;

	priv->show_time_stat[line_idx] = 1;
    netif_carrier_on(g_ptm_tc->dev);
	return 0;
}

static int ptm_showtime_exit(const unsigned char line_idx)
{
	struct vrx318_priv *priv;

	priv = g_ptm_tc->frwk;
	if (line_idx >= priv->ep_num)
		return -EINVAL;

	priv->show_time_stat[line_idx] = 0;
    netif_carrier_off(g_ptm_tc->dev);
	return 0;
}

int ptm_in_showtime(void)
{
	int i;

	struct vrx318_ptm_tc *priv = g_ptm_tc;
	if (priv->frwk->param.dfe_loopback)
		return 1;

	if (priv->tc_mode == TC_PTM_BND_MODE) {
		for (i = 0; i < MAX_VRX318_NUM; i++) {
			if (priv->frwk->show_time_stat[i] != 0)
				return 1;
		}
		return 0;
	} else {
		return priv->frwk->show_time_stat[priv->ep_id];
	}
}

/**
 *  ps_en:  1 - Enable powersaving, 0 -Disable Powersaving
 *  flag:   0 - Get the powersaving cfg, 1 - Set the powersaving
 */
int ptm_power_saving(struct vrx318_ptm_tc *priv, int ps_en, int flag)
{
	struct psave_cfg ps_cfg;
	u32 base;

	base = (u32)priv->frwk->ep_dev[priv->ep_id].membase;
	dword_clear(&ps_cfg, sizeof(ps_cfg));
	dword_mem_read(&ps_cfg, SB_BUFFER(__PSAVE_CFG),
			sizeof(ps_cfg), base);
	if (!flag)
		return ps_cfg.sleep_en;
	else {
		ps_cfg.sleep_en = !!(ps_en);
		dword_mem_write(SB_BUFFER(__PSAVE_CFG), &ps_cfg,
			sizeof(ps_cfg), base);
	}

	return 0;
}

static inline void dump_skb_info(struct sk_buff *skb)
{
	vrx318_dbg(DBG_PKT_TX,
		"skb: head: 0x%x, data: 0x%x, tail: 0x%x, end: 0x%x, len: %d\n",
		(u32)skb->head, (u32)skb->data, (u32)skb->tail,
		(u32)skb->end, skb->len);
	vrx318_dbg(DBG_PKT_TX,
		"skb: clone: %d, users: %d\n",
		skb->cloned, atomic_read(&skb->users));
	vrx318_dbg(DBG_PKT_TX,
		"skb: nfrag: %d\n", skb_shinfo(skb)->nr_frags);
}

int ptm_qos_qnum()
{
	if (!g_ptm_tc)
		return PTM_PRIO_Q_NUM;
	return PTM_PRIO_Q_NUM / g_ptm_tc->frwk->param.atm_qos;
}

u32 ptm_qos_mib_base()
{
	return __QOSQ_MIB_BASE;
}

static int ptm_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct vrx318_ptm_tc *priv = netdev_priv(dev);
	dp_subif_t dp_id;
	int qid, prio;
	int qnum;

	if (!ptm_in_showtime())
		goto PTM_XMIT_DROP;

	if (skb_padto(skb, ETH_ZLEN))
		goto PTM_XMIT_DROP;

	dump_skb_info(skb);

	qnum = ptm_qos_qnum();
	prio = skb->priority < qnum ?
			skb->priority : qnum - 1;
	qid = priv->ptm_prio_queue_map[prio];
	dp_id.subif = priv->subif_id | PTM_DESC_QID(qid);
	/* Set EP */
	skb->DW1 = (skb->DW1 & (~0xF00)) | ((priv->frwk->port_id & 0xF) << 8);
	/* Set Subif */
	skb->DW0 = (skb->DW0 & (~0x7FFF)) | (dp_id.subif);

	dp_id.port_id = priv->frwk->port_id;

	if (dp_xmit(dev, &dp_id, skb, skb->len, 0) == 0) {
		priv->stats.tx_packets++;
		priv->stats.tx_bytes += skb->len;
	} else {
		priv->stats.tx_dropped++;
	}

	return 0;

PTM_XMIT_DROP:
	dev_kfree_skb_any(skb);
	priv->stats.tx_dropped++;
	return 0;
}

static int32_t ptm_dp_stop_tx(struct net_device *dev)
{
	netif_tx_stop_all_queues(dev);

	return 0;
}

static int32_t ptm_dp_restart_tx(struct net_device *dev)
{
	netif_tx_start_all_queues(dev);
	return 0;
}

static void ptm_rx(struct vrx318_ptm_tc *priv,
	int len, struct sk_buff *skb)
{
	if (skb->len < ETH_ZLEN || skb->len > MAX_MTU) {
		vrx318_dbg(DBG_ERR, "illegal packet len: %d\n", skb->len);
		priv->stats.rx_length_errors++;
		goto rx_err_exit;
	}

	skb->protocol = eth_type_trans(skb, priv->dev);

	priv->stats.rx_packets++;
	priv->stats.rx_bytes += len;

	if (netif_rx(skb) == NET_RX_DROP)
		priv->stats.rx_dropped++;

	return;

rx_err_exit:
	dev_kfree_skb_any(skb);
	priv->stats.rx_errors++;

	return;
}


static int32_t ptm_dp_rx(struct net_device *rx_dev,
		struct net_device *tx_dev, struct sk_buff *skb, int32_t len)
{
	int32_t err;
	struct vrx318_ptm_tc *priv = g_ptm_tc;

	if (likely(rx_dev == priv->dev)) {
		vrx318_dbg(DBG_PKT_RX, "Receive Packet from VRX318\n");
	} else if (tx_dev == priv->dev)
		vrx318_dbg(DBG_PKT_RX, "rcv a undeliver packet from DP lib, itf:%s\n",
			tx_dev->name);
	else {
		vrx318_dbg(DBG_PKT_RX,
			"unknown packet forward to VRX318 ptm driver!!!, RX: %s, TX: %s\n",
			!rx_dev ? "None" : rx_dev->name,
			!tx_dev ? "None" : tx_dev->name);
		err = -ENODEV;
		goto rx_err_exit;
	}

	/* remove PMAC header */
	skb_pull(skb, 8);

	ptm_rx(priv, len, skb);
	return 0;

rx_err_exit:
	dev_kfree_skb_any(skb);
	priv->stats.rx_errors++;

	return err;
}

static int ptm_get_subif_id(struct net_device *netif, struct sk_buff *skb,
			struct atm_vcc *vcc, uint8_t dst_mac[MAX_ETH_ALEN],
			dp_subif_t *subif, uint32_t flags)
{
	struct vrx318_ptm_tc *priv = g_ptm_tc;
	int qid, prio, qnum;

	if (subif->port_id != priv->frwk->port_id) {
		vrx318_dbg(DBG_ERR,
			"%s: Port_id not compliance: Input: %d, Local: %d\n",
			__func__, subif->port_id, priv->frwk->port_id);
		return -EINVAL;
	}

	qnum = ptm_qos_qnum();
	if (skb)
			prio = skb->priority < qnum ?
					skb->priority : qnum - 1;
	else
			prio = 0;
	qid = priv->ptm_prio_queue_map[prio];
	subif->subif = priv->subif_id | PTM_DESC_QID(qid);

	return 0;
}

static void ptm_setup(struct net_device *dev)
{
	ether_setup(dev);
	dev->watchdog_timeo = 10 * HZ;
	dev->netdev_ops = &g_ptm_netdev_ops;
}

static void vrx318_ptm_cfg_init(struct vrx318_priv *priv,
				enum dsl_tc_mode tc_mode)
{
	/* PTM Single Line or bonding P2P CDMA WR is *must* enabled */
	if (tc_mode == TC_PTM_SL_MODE
		|| (tc_mode == TC_PTM_BND_MODE && priv->param.p2p_en))
		priv->param.cdma_wr_en = 1;
	else
		priv->param.cdma_wr_en = 0;
}

static int vrx318_ptm_dev_init(struct vrx318_priv *frwk_priv,
				enum dsl_tc_mode tc_mode)
{
	int err;
	struct net_device *dev;
	struct vrx318_ptm_tc *priv;
	const char macaddr[ETH_ALEN]
		= {0xAC, 0x9A, 0x96, 0x11, 0x22, 0x33};

	dev = alloc_netdev(sizeof(*priv), "ptm%d", ptm_setup);
	if (!dev) {
		vrx318_dbg(DBG_INIT, "Cannot alloc net device\n");
		return -ENOMEM;
	}
	priv = netdev_priv(dev);
	priv->dev = dev;
	priv->frwk = frwk_priv;
	ether_addr_copy(dev->dev_addr, macaddr);
	SET_NETDEV_DEV(dev, frwk_priv->pdev);
	spin_lock_init(&priv->ptm_lock);
	init_completion(&priv->comp);

	/* Initial bonding */
	if (tc_mode == TC_PTM_BND_MODE) {
		err = vrx318_bonding_init(priv, frwk_priv);
		if (err)
			goto err1;
	}

	err = register_netdev(priv->dev);
	if (err)
		goto err1;

	g_ptm_tc = priv;
	netif_carrier_off(dev);

	return 0;
err1:
	free_netdev(priv->dev);
	g_ptm_tc = NULL;

	return err;
}

void vrx318_ptm_exit(void)
{
	struct vrx318_ptm_tc *priv = g_ptm_tc;

	if (!priv)
		return;

	unregister_netdev(priv->dev);
	free_netdev(priv->dev);

	if (priv->ptm_bnd_cfg.soc_peer_membase != 0)
		free_page(priv->ptm_bnd_cfg.soc_peer_membase);
	kfree(priv);
	g_ptm_tc = 0;

	vrx318_dbg(DBG_INFO, "VRX318 PTM exited!\n");
}

static void vrx318_tc_reg_init(unsigned long base)
{
	u32 val;

	/* TODO: convert to meaningful configuration */
	vrx318_w32(0x00020000, PPE_SFSM_PGCNT0, base);
	vrx318_w32(0x00020000, PPE_SFSM_PGCNT1, base);

	vrx318_w32(0x000001E0, PPE_DREG_AT_CFG0, base);
	vrx318_w32(0x000001E0, PPE_DREG_AT_CFG1, base);
	vrx318_w32(0x00000000, PPE_DREG_AT_IDLE0, base);
	vrx318_w32(0x00000000, PPE_DREG_AT_IDLE1, base);

	vrx318_w32(0x000001F0, PPE_DREG_AR_CFG0, base);
	vrx318_w32(0x000001F0, PPE_DREG_AR_CFG1, base);
	vrx318_w32(0x00000000, PPE_DREG_AR_IDLE0, base);
	vrx318_w32(0x00000000, PPE_DREG_AR_IDLE1, base);

	vrx318_w32(0x0000080C, PPE_DREG_B0_LADR, base);
	vrx318_w32(0x0000080C, PPE_DREG_B1_LADR, base);

	vrx318_w32(0x00003000, PPE_SFSM_DBA0, base);
	vrx318_w32(0x00003EE0, PPE_SFSM_CBA0, base);
	vrx318_w32(0x00003770, PPE_SFSM_DBA1, base);
	vrx318_w32(0x00003F50, PPE_SFSM_CBA1, base);

	vrx318_w32(0x00002000, PPE_FFSM_DBA0, base);
	vrx318_w32(0x00002550, PPE_FFSM_DBA1, base);

	vrx318_w32(0x00030070, PPE_SFSM_CFG0, base);
	vrx318_w32(0x00030070, PPE_SFSM_CFG1, base);

	/* KEEP IDLE (BC0 & BC1) */
	val = vrx318_r32(PPE_SFSM_CFG0, base) | (1 << 15);
	vrx318_w32(val, PPE_SFSM_CFG0, base);
	val = vrx318_r32(PPE_SFSM_CFG1, base) | (1 << 15);
	vrx318_w32(val, PPE_SFSM_CFG1, base);

	/* Enable SFSM (BC0 & BC1) */
	val = vrx318_r32(PPE_SFSM_CFG0, base) | (1 << 14);
	vrx318_w32(val, PPE_SFSM_CFG0, base);
	val = vrx318_r32(PPE_SFSM_CFG1, base) | (1 << 14);
	vrx318_w32(val, PPE_SFSM_CFG1, base);

	vrx318_w32(0xF0D10000, PPE_FFSM_IDLE_HEAD_BC0, base);
	vrx318_w32(0xF0D10000, PPE_FFSM_IDLE_HEAD_BC1, base);

	vrx318_w32(0x00030050, PPE_FFSM_CFG0, base);
	vrx318_w32(0x00030050, PPE_FFSM_CFG1, base);

	val = (((0x70 * CW_PAGE_SZ) << 16) |
		(0x50 * CW_PAGE_SZ));
	vrx318_w32(val, PPE_BC0_CW_SIZE, base);

	return;
}

static void vrx318_pdma_init(unsigned long base, int is_bonding)
{
	/* TODO: convert to meaningful configuration */
	vrx318_w32(0x00000001, PPE_PDMA_CFG, base);
	vrx318_w32(0x00082C00, PPE_PDMA_RX_CTX_CFG, base);
	vrx318_w32(0x00081B00, PPE_PDMA_TX_CTX_CFG, base);
	vrx318_w32(PPE_PDMA_RX_MAX_LEN_REG_CONF, PPE_PDMA_RX_MAX_LEN_REG, base);

	vrx318_w32(0x000F003F, PPE_PDMA_RX_DELAY_CFG, base);
	vrx318_dbg(DBG_INIT, "%s: Configure PDMA for jumbo 0x%08X\n",
						__func__, PPE_PDMA_RX_MAX_LEN_REG_CONF);

	vrx318_w32(0x00000011, PPE_SAR_MODE_CFG, base);
	vrx318_w32(0x00081200, PPE_SAR_RX_CTX_CFG, base);
	vrx318_w32(0x00082E00, PPE_SAR_TX_CTX_CFG, base);
	vrx318_w32(0x00001021, PPE_SAR_POLY_CFG_SET0, base);
	vrx318_w32(0x1EDC6F41, PPE_SAR_POLY_CFG_SET1, base);
	vrx318_w32(0x04C11DB7, PPE_SAR_POLY_CFG_SET2, base);
	vrx318_w32(0x00000F3E, PPE_SAR_CRC_SIZE_CFG, base);

	vrx318_w32(0x01001900, PPE_SAR_PDMA_RX_CMDBUF_CFG, base);
	vrx318_w32(0x01001A00, PPE_SAR_PDMA_TX_CMDBUF_CFG, base);

	vrx318_w32(0x00203FC0, PPE_SAR_PDMA_RX_FW_CMDBUF_CFG, base);
	vrx318_w32(0x00203FE0, PPE_SAR_PDMA_TX_FW_CMDBUF_CFG, base);

	vrx318_w32(0x00000007, PPE_PDMA_IER, base);

	return;
}

static void vrx318_tc_hw_init(u32 base, int is_bonding)
{
	vrx318_tc_reg_init(base);
	vrx318_pdma_init(base, is_bonding);
}

void vrx318_tc_fw_init(u32 base, int is_bonding, int pmac_en)
{
	std_des_cfg_t std_des_cfg;
	qos_cfg_t qos_cfg;
	qosq_flow_ctrl_cfg_t qosq_flow_ctrl_cfg;
	struct psave_cfg ps_cfg;
	struct test_mode test_mode;
	struct rx_bc_cfg rx_bc_cfg;
	struct tx_bc_cfg tx_bc_cfg;
	struct rx_gamma_itf_cfg rx_gamma_itf_cfg;
	struct tx_gamma_itf_cfg tx_gamma_itf_cfg;

	u32 dst_addr;
	int i;

	vrx318_w32(0x3C30, DS_FLOW_CTRL_CFG, base);

	dword_clear(&std_des_cfg, sizeof(std_des_cfg));
	/* this field replaces byte_off in rx descriptor of VDSL ingress */
	std_des_cfg.byte_off = is_bonding ? 2 : 0;
	std_des_cfg.data_len = DMA_PACKET_SIZE;
	dword_mem_write(SB_BUFFER(__STD_DES_CFG), &std_des_cfg,
		sizeof(std_des_cfg), base);

	dword_clear(&qos_cfg, sizeof(qos_cfg));
	/* 16 * (cgu_get_pp32_clock() / 1000000) */
	qos_cfg.time_tick = get_ppe_clk(base) / 62500;
	qos_cfg.qosq_num = QOSQ_NUM;
	qos_cfg.qos_en = 0;
	dword_mem_write(SB_BUFFER(__QOS_CFG), &qos_cfg, sizeof(qos_cfg), base);

	dword_clear(&ps_cfg, sizeof(ps_cfg));
	ps_cfg.start_state = 0;
	ps_cfg.sleep_en = 0; /* Disable sleep mode by default */
	dword_mem_write(SB_BUFFER(__PSAVE_CFG), &ps_cfg, sizeof(ps_cfg), base);

	dword_clear(&qosq_flow_ctrl_cfg, sizeof(qosq_flow_ctrl_cfg));
	qosq_flow_ctrl_cfg.large_frame_size = 128;
	qosq_flow_ctrl_cfg.large_frame_drop_th = 16;
	dword_mem_write(SB_BUFFER(__QOSQ_FLOW_CTRL_CFG), &qosq_flow_ctrl_cfg,
			sizeof(qosq_flow_ctrl_cfg), base);

	dword_clear(&test_mode, sizeof(test_mode));
	test_mode.mib_clear_mode    = 0;
	test_mode.test_mode         = 0;
	dword_mem_write(SB_BUFFER(__TEST_MODE), &test_mode,
		sizeof(test_mode), base);

	vrx318_w32(DMA_PACKET_SIZE, SB_BUFFER(__MAX_PKT_SIZE_CFG), base);

	dword_clear(&rx_bc_cfg, sizeof(rx_bc_cfg));
	rx_bc_cfg.local_state   = 0;
	rx_bc_cfg.remote_state  = 0;
	rx_bc_cfg.to_false_th   = 7;
	rx_bc_cfg.to_looking_th = 3;
	for (i = 0; i < 2; i++) {
		dst_addr = __RX_BC0_CFG_STATS_CFG + (i * sizeof(rx_bc_cfg) / 4);
		dword_mem_write(SB_BUFFER(dst_addr), &rx_bc_cfg,
			sizeof(rx_bc_cfg), base);
	}

	dword_clear(&tx_bc_cfg, sizeof(tx_bc_cfg));
	tx_bc_cfg.fill_wm = 2;
	tx_bc_cfg.uflw_wm = 2;
	for (i = 0; i < 2; i++) {
		dst_addr = __TX_BC0_CFG_STATS_CFG + (i * sizeof(tx_bc_cfg) / 4);
		dword_mem_write(SB_BUFFER(dst_addr), &tx_bc_cfg,
			sizeof(tx_bc_cfg), base);
	}

	dword_clear(&rx_gamma_itf_cfg, sizeof(rx_gamma_itf_cfg));
	rx_gamma_itf_cfg.receive_state		= 0;
	rx_gamma_itf_cfg.rx_min_len		= 64;
	rx_gamma_itf_cfg.rx_pad_en		= 1;
	/* disable Ethernet FCS verification during bonding */
	rx_gamma_itf_cfg.rx_eth_fcs_ver_dis	= is_bonding ? 1 : 0;
	rx_gamma_itf_cfg.rx_rm_eth_fcs		= is_bonding ? 0 : 1;
	rx_gamma_itf_cfg.rx_tc_crc_ver_dis	= 0;
	rx_gamma_itf_cfg.rx_tc_crc_size		= 1;
	rx_gamma_itf_cfg.rx_eth_fcs_result	= 0xC704DD7B;
	rx_gamma_itf_cfg.rx_tc_crc_result	= 0x1D0F1D0F;
	rx_gamma_itf_cfg.rx_crc_cfg		= 0x2500;
	rx_gamma_itf_cfg.rx_eth_fcs_init_value	= 0xFFFFFFFF;
	rx_gamma_itf_cfg.rx_tc_crc_init_value	= 0x0000FFFF;
	rx_gamma_itf_cfg.rx_max_len_sel		= 0;
	rx_gamma_itf_cfg.rx_edit_num2		= 0;
	rx_gamma_itf_cfg.rx_edit_pos2		= 0;
	rx_gamma_itf_cfg.rx_edit_type2		= 0;
	rx_gamma_itf_cfg.rx_edit_en2		= 0;
	/* no PMAC header insertion during bonding or grx350 platform*/
	rx_gamma_itf_cfg.rx_edit_num1		= (is_bonding || !pmac_en) ? 0 : 4;
	rx_gamma_itf_cfg.rx_edit_pos1		= 0;
	/* no PMAC header insertion during bonding or grx350 platform*/
	rx_gamma_itf_cfg.rx_edit_type1		= (is_bonding || !pmac_en) ? 0 : 1;
	rx_gamma_itf_cfg.rx_edit_en1		= (is_bonding || !pmac_en) ? 0 : 1;
	/*  E5: byte swap of value 0x07000000 */
	rx_gamma_itf_cfg.rx_inserted_bytes_1l	= 0x00000007;
	rx_gamma_itf_cfg.rx_inserted_bytes_1h	= 0;
	rx_gamma_itf_cfg.rx_inserted_bytes_2l	= 0;
	rx_gamma_itf_cfg.rx_inserted_bytes_2h	= 0;
	rx_gamma_itf_cfg.rx_len_adj		= -2;
	for (i = 0; i < 4; i++) {
		dst_addr = __RX_GIF0_CFG_STATS_CFG +
			(i * sizeof(rx_gamma_itf_cfg) / 4);
		dword_mem_write(SB_BUFFER(dst_addr), &rx_gamma_itf_cfg,
			sizeof(rx_gamma_itf_cfg), base);
	}

	dword_clear(&tx_gamma_itf_cfg, sizeof(tx_gamma_itf_cfg));
	tx_gamma_itf_cfg.tx_len_adj		= is_bonding ? 2 : 6;
	tx_gamma_itf_cfg.tx_crc_off_adj		= 6;
	tx_gamma_itf_cfg.tx_min_len		= 0;
	tx_gamma_itf_cfg.tx_eth_fcs_gen_dis	= is_bonding ? 1 : 0;
	tx_gamma_itf_cfg.tx_tc_crc_size		= 1;
	tx_gamma_itf_cfg.tx_crc_cfg		= is_bonding ? 0x2F02 : 0x2F00;
	tx_gamma_itf_cfg.tx_eth_fcs_init_value	= 0xFFFFFFFF;
	tx_gamma_itf_cfg.tx_tc_crc_init_value	= 0x0000FFFF;
	for (i = 0; i < 4; i++) {
		/*  queue_mapping is reserved in VRX318
		* tx_gamma_itf_cfg.queue_mapping = g_queue_gamma_map[i];
		*/
		dst_addr = __TX_GIF0_CFG_STATS_CFG +
			(i * sizeof(tx_gamma_itf_cfg) / 4);
		dword_mem_write(SB_BUFFER(dst_addr), &tx_gamma_itf_cfg,
			sizeof(tx_gamma_itf_cfg), base);
	}

	/* Initialize TX Ctrl K Table */
	vrx318_w32(0x90111293, TX_CTRL_K_TABLE(0), base);
	vrx318_w32(0x14959617, TX_CTRL_K_TABLE(1), base);
	vrx318_w32(0x18999A1B, TX_CTRL_K_TABLE(2), base);
	vrx318_w32(0x9C1D1E9F, TX_CTRL_K_TABLE(3), base);
	vrx318_w32(0xA02122A3, TX_CTRL_K_TABLE(4), base);
	vrx318_w32(0x24A5A627, TX_CTRL_K_TABLE(5), base);
	vrx318_w32(0x28A9AA2B, TX_CTRL_K_TABLE(6), base);
	vrx318_w32(0xAC2D2EAF, TX_CTRL_K_TABLE(7), base);
	vrx318_w32(0x30B1B233, TX_CTRL_K_TABLE(8), base);
	vrx318_w32(0xB43536B7, TX_CTRL_K_TABLE(9), base);
	vrx318_w32(0xB8393ABB, TX_CTRL_K_TABLE(10), base);
	vrx318_w32(0x3CBDBE3F, TX_CTRL_K_TABLE(11), base);
	vrx318_w32(0xC04142C3, TX_CTRL_K_TABLE(12), base);
	vrx318_w32(0x44C5C647, TX_CTRL_K_TABLE(13), base);
	vrx318_w32(0x48C9CA4B, TX_CTRL_K_TABLE(14), base);
	vrx318_w32(0xCC4D4ECF, TX_CTRL_K_TABLE(15), base);

	return;
}

/**
* Initialize VRX318 PPE FW General Configuration
* is_bonding = 0               : single line application
* is_bonding = 1, ep_id = 0: bonding application, us_bonding_master
* is_bonding = 1, ep_id = 1: bonding application, ds_bonding_master
*/
static void vrx318_gen_cfg_init(struct vrx318_ptm_tc *priv, u32 base,
		int is_bonding, u32 ep_id, u32 peer_base,
		int cdma_write_data_en, int pmac_en)
{
	task_cfg_t task_cfg[2];

	dword_clear(&task_cfg, sizeof(task_cfg));
	/* Default Task Configuration (Single-Line) */
	task_cfg[0].pp32_core_id	= 0;	task_cfg[1].pp32_core_id	= 1;
	task_cfg[0].us_bonding_master	= 0;	task_cfg[1].us_bonding_master	= 0;
	task_cfg[0].us_segment_en	= 0;	task_cfg[1].us_segment_en	= 0;
	task_cfg[0].us_buf_release_en	= 0;	task_cfg[1].us_buf_release_en	= 0;

	task_cfg[0].ds_bonding_master	= 0;	task_cfg[1].ds_bonding_master	= 0;
	task_cfg[0].ds_pkt_dispatch_en	= 0;	task_cfg[1].ds_pkt_dispatch_en	= 0;
	task_cfg[0].ds_pkt_reconstruct_en = 0;	task_cfg[1].ds_pkt_reconstruct_en = 0;
	task_cfg[0].ds_pkt_flush_en	= 0;	task_cfg[1].ds_pkt_flush_en	= 0;

	task_cfg[0].tc_us_en		= 1;	task_cfg[1].tc_us_en		= 0;
	task_cfg[0].tc_ds_en		= 1;	task_cfg[1].tc_ds_en		= 0;

	task_cfg[0].des_sync_en		= 0;	task_cfg[1].des_sync_en		= 1;
	task_cfg[0].edma_write_lle_gen_en = 0;	task_cfg[1].edma_write_lle_gen_en = 1;
	task_cfg[0].edma_read_lle_gen_en = 0;   task_cfg[1].edma_read_lle_gen_en = 1;
	task_cfg[0].edma_post_proc_en	= 0;	task_cfg[1].edma_post_proc_en	= 1;
	task_cfg[0].qos_wfq_shaping_en	= 0;	task_cfg[1].qos_wfq_shaping_en	= 1;
	task_cfg[0].qos_dispatch_en	= 0;	task_cfg[1].qos_dispatch_en	= 1;
	task_cfg[0].qos_replenish_en	= 0;	task_cfg[1].qos_replenish_en	= 1; /* enable for QoS */

	if (is_bonding == 1) {
		bond_conf_t bond_conf;
		dword_clear(&bond_conf, sizeof(bond_conf));

		bond_conf.dplus_fp_fcs_en   = 0x1;
		bond_conf.max_frag_size     = 0x200;
		/* 0x4 if two bearer channels are enabled */
		bond_conf.bg_num            = 0x2;
		bond_conf.bond_mode         = 0x0;
		bond_conf.e1_bond_en        = 0x1;
		bond_conf.d5_acc_dis        = 0x1;
		bond_conf.d5_b1_en          = 0x1;

		dword_mem_write(SB_BUFFER(__BOND_CONF), &bond_conf,
			sizeof(bond_conf), base);

		/**
		* Get Peer-Base Address
		* address = ((PPE_BASE + 0x6000 * 4) & FPI_address_mask) + 0x20000000 + peer_base
		* PI concept required to use peer to peer write. pcie switch issue is retained.
		* The address from driver's perspective is 0x6000 larger than FW. so by default, driver factor in 0x6000
		* so that FW no need to re-calcuate the address.
		* Give FW the base address, by put 6000 DWORDS offset by default.
		*/
		if (cdma_write_data_en) {/* TODO: for GRX350, need check the new peer addr mapping */
			vrx318_w32_sb(((PPE_BASE + FW_BASE_ADDR_OFFSET * 4) & VRX318_SMASK) + VRX318_OB_BADDR + peer_base,
				__BOND_PEER_SB_BASE, base);
		} else {
			/**
			* As FW cannot write to the peer due to pcie switch don't support it.
			* Driver will read it out and write to the peer.
			* The peer address will be write to 0 to remind FW to use local address.
			*/
			vrx318_w32_sb(0, __BOND_PEER_SB_BASE, base);
		}
		vrx318_w32_sb(0x07800000, __DS_PKT_PMAC_HEADER, base);
	}

	if ((is_bonding == 1) && (ep_id == US_BONDING_MASTER)) {

		vrx318_w32_sb(0x0000FE01, __US_BG_QMAP, base);
		vrx318_w32_sb(0x00002211, __US_BG_GMAP, base);

		task_cfg[1].us_bonding_master	= 1;
		task_cfg[1].us_segment_en	= 1;
		task_cfg[1].us_buf_release_en	= 1;

		task_cfg[0].us_bonding_des_sync	= 0;	task_cfg[1].us_bonding_des_sync	= 1;
		task_cfg[0].ds_bonding_des_sync	= 0;	task_cfg[1].ds_bonding_des_sync	= 1;

		task_cfg[0].des_sync_en		= 0;	task_cfg[1].des_sync_en		= 1;
		task_cfg[0].edma_write_lle_gen_en = 0;	task_cfg[1].edma_write_lle_gen_en = 1;
		task_cfg[0].edma_read_lle_gen_en = 0;	task_cfg[1].edma_read_lle_gen_en = 1;
		task_cfg[0].edma_post_proc_en	= 0;	task_cfg[1].edma_post_proc_en	= 1;

		task_cfg[0].qos_wfq_shaping_en	= 1;	task_cfg[1].qos_wfq_shaping_en	= 0;
		task_cfg[0].qos_dispatch_en	= 1;	task_cfg[1].qos_dispatch_en	= 0;
		task_cfg[0].qos_replenish_en	= 1;	task_cfg[1].qos_replenish_en	= 0;

	} else if ((is_bonding == 1) && (ep_id == DS_BONDING_MASTER)) {

		task_cfg[1].ds_bonding_master	= 1;
		task_cfg[1].ds_pkt_dispatch_en	= 1;
		task_cfg[1].ds_pkt_reconstruct_en = 1;
		task_cfg[1].ds_pkt_flush_en	= 1;

		task_cfg[0].us_bonding_des_sync	= 0;	task_cfg[1].us_bonding_des_sync	= 1;
		task_cfg[0].ds_bonding_des_sync	= 0;	task_cfg[1].ds_bonding_des_sync	= 1;

		task_cfg[0].des_sync_en		= 1;	task_cfg[1].des_sync_en		= 0;
		task_cfg[0].edma_write_lle_gen_en = 0;	task_cfg[1].edma_write_lle_gen_en = 1;
		task_cfg[0].edma_read_lle_gen_en = 0;	task_cfg[1].edma_read_lle_gen_en = 1;
		task_cfg[0].edma_post_proc_en	= 0;	task_cfg[1].edma_post_proc_en	= 1;

		task_cfg[0].qos_wfq_shaping_en	= 0;	task_cfg[1].qos_wfq_shaping_en	= 0;
		task_cfg[0].qos_dispatch_en	= 0;	task_cfg[1].qos_dispatch_en	= 0;
		task_cfg[0].qos_replenish_en	= 0;	task_cfg[1].qos_replenish_en	= 0;

		vrx318_w32_sb(0x00002211, __DS_BG_GMAP, base);
	}

	dword_mem_write(SB_BUFFER(__PP32_0_TASK_CFG), &task_cfg, sizeof(task_cfg), base);

	if (!pmac_en)
		vrx318_w32_sb(1, __SOC_FAMILY, base);
}

/* Initialize eDMA Configuration Registers and PPE FW eDMA Context
* Single line applcation:   MUST be called
* Bonding line application: MUST be called for both VRX318
*/
static void vrx318_edma_init(u32 base, int lle_in_sb, int cdma_write_data_en)
{
	unsigned int dst_addr;
	edma_ch_ctrl_t edma_ch_ctrl;
	edma_lle_link_t edma_lle_link;
	edma_ch_ctxt_t edma_ch_ctxt;

	/* edma hang detection init James */
	vrx318_w32_sb(10, __EDMA_HANG_DETECT_TH, base);

	/* Initialize eDMA Channel Control Register - Data Structure */
	dword_clear(&edma_ch_ctrl, sizeof(edma_ch_ctrl));

	edma_ch_ctrl.at = 0; /* ??? Address Translation (AT) */
	edma_ch_ctrl.tc = 0; /* Traffic Class (TC) */
	edma_ch_ctrl.td = 0; /* Traffic Digest, the PCIe core adds the ECRC */
	/* field and sets the TD bit in TLP header */
	edma_ch_ctrl.ro = 1; /* Releaxed Ording */
	edma_ch_ctrl.ns = 0; /* No Snoop */
	/* ??? Function Number (FN) for generated MRd/MWr DMA TLPs */
	edma_ch_ctrl.fn = 0;
	/* The core uses this when generating the RID for the MRd/MWr DMA TLP */
	edma_ch_ctrl.ll_en = 1;	/* link list enable */
	/* Consumer Cycles State (CCS).
	* Used in Link List mode only. It is used to synchronize
	* the Producer (Software) and the Consumer (DMA).
	* *You must initialize this bit.* The DMA updates
	* this bit during linked list operation
	*/
	edma_ch_ctrl.ccs = 1;
	/* Channel Status (CS)
	* The Channel Status bits identify the current operational
	* state of the DMA write or read channel.
	* 00: Reserved
	* 01: Running, this channel is active and transferring data
	* 10: Halted. An error condition has been detected.
	* and the DMA has stopped this channel
	* 11: Stopped. The DMA has transferred all data for
	* this channel or you have prematurely stopped
	* this channel by writing to the Stop field of
	* of the DMA R/W doorbell register
	*/
	edma_ch_ctrl.cs	= 3;

	edma_ch_ctrl.rie = 0; /* remote interrupt enable */
	edma_ch_ctrl.lie = 0; /* local interrupt enable */
	/* load link pointer (LLP)
	* Used in link list mode only. Indicates that
	* this linked list element is a link element, and
	* it's LL element pointer DWORDs are pointint to the next
	* (non-contiguous) element
	* the DMA loads this field with the LLP of the
	* linked list element
	*/
	edma_ch_ctrl.llp = 0;
	/* Toggle Cycle Bit (TCB)
	* Indicates to the DMA to toglle its intepreation of
	* the CB. Used in linked list mode only. It is used
	* to synchorize the Producer (Software) and the
	* Consumer (DMA).
	* The DMA loads this field with the TCB of the linked
	* list element.
	* Note: this field is not defined in a ata LL element
	*/
	edma_ch_ctrl.tcb = 0;
	/* Cycle Bit (CB)
	* Unsed in Linked list mode only.  It is used
	* to synchorize the Producer (Software) and the
	* Consumer (DMA).
	* The DMA loads this field with the CB of the linked
	* list elment
	*/
	edma_ch_ctrl.cb = 0;

	if (cdma_write_data_en == 0) {
		/* Configure eDMA Write Channel */

		/* Disable eDMA Write Channel */
		vrx318_w32(0, EDMA_WCH_EN, base);

		/* Select Write Channel */
		vrx318_w32(EDMA_WRITE_CH, EDMA_CH_IDX, base);

		/* Setup eDMA Channel Control Register */
		dword_mem_write(EDMA_CH_CTRL, &edma_ch_ctrl,
				sizeof(edma_ch_ctrl), base);

		vrx318_w32(0, EDMA_TRANSFER_SIZE, base);
		vrx318_w32(0, EDMA_SAR_LOW, base);
		vrx318_w32(0, EDMA_SAR_HIGH, base);
		vrx318_w32(0, EDMA_DAR_LOW, base);
		vrx318_w32(0, EDMA_DAR_HIGH, base);

		if (lle_in_sb == 1)
			vrx318_w32(SB_BUFFER(__DS_EDMA_LLE_BASE),
					EDMA_LL_PTR_LOW, base);
		else
			vrx318_w32(__DS_EDMA_LLE_FPI_BASE,
					EDMA_LL_PTR_LOW, base);

		vrx318_w32(0, EDMA_LL_PTR_HIGH, base);

		/* Setup Write Channel Link List Elements */
		dword_clear(&edma_lle_link, sizeof(edma_lle_link));
		edma_lle_link.tcb = 1;
		edma_lle_link.llp = 1;
		if (lle_in_sb == 1) {
			edma_lle_link.lle_ptr_low
				= SB_BUFFER(__DS_EDMA_LLE_BASE);
			dst_addr = SB_BUFFER(__DS_EDMA_LLE_BASE);
		} else {
			edma_lle_link.lle_ptr_low = __DS_EDMA_LLE_FPI_BASE;
			dst_addr = __DS_EDMA_LLE_FPI_BASE;
		}

		dword_ep_clear(dst_addr,
			(EDMA_WCH_DATA_LLE_NUM * sizeof(edma_lle_data_t)),
			base);
		dst_addr += EDMA_WCH_DATA_LLE_NUM * sizeof(edma_lle_data_t);
		dword_mem_write(dst_addr, &edma_lle_link,
				sizeof(edma_lle_link), base);

		/* Setup EDMA Write Channel Context */
		dword_clear(&edma_ch_ctxt, sizeof(edma_ch_ctxt));
		edma_ch_ctxt.edma_ch_type = EDMA_WRITE_CH;
		edma_ch_ctxt.edma_pcs = 1;
		edma_ch_ctxt.edma_lle_num = EDMA_WCH_DATA_LLE_NUM;
		edma_ch_ctxt.edma_lle_sb_size = EDMA_WCH_DATA_LLE_NUM * 6;
		edma_ch_ctxt.edma_lle_sb_base = __DS_EDMA_LLE_BASE;
		edma_ch_ctxt.edma_lle_ext_sb_base = __DS_EDMA_LLE_EXT_BASE;
		if (lle_in_sb == 1)
			edma_ch_ctxt.edma_lle_fpi_base
				= SB_BUFFER(__DS_EDMA_LLE_BASE);
		else
			edma_ch_ctxt.edma_lle_fpi_base
				= __DS_EDMA_LLE_FPI_BASE;

		edma_ch_ctxt.edma_ch_status = EDMA_STOPPED;
		dword_mem_write(SB_BUFFER(__DS_EDMA_WRITE_CH_CFG_CTXT),
			&edma_ch_ctxt, sizeof(edma_ch_ctxt), base);

		/* Enable eDMA Write Channel */
		vrx318_w32(1, EDMA_WCH_EN, base);
	}

	/* Configure eDMA Read Channel */

	/* Disable eDMA Read Channel */
	vrx318_w32(0, EDMA_RCH_EN, base);

	/* Select Read Channel */
	vrx318_w32(EDMA_READ_CH, EDMA_CH_IDX, base);

	/* Setup eDMA Channel Control Register */
	dword_mem_write(EDMA_CH_CTRL, &edma_ch_ctrl,
			sizeof(edma_ch_ctrl), base);

	vrx318_w32(0, EDMA_TRANSFER_SIZE, base);
	vrx318_w32(0, EDMA_SAR_LOW, base);
	vrx318_w32(0, EDMA_SAR_HIGH, base);
	vrx318_w32(0, EDMA_DAR_LOW, base);
	vrx318_w32(0, EDMA_DAR_HIGH, base);

	if (lle_in_sb == 1)
		vrx318_w32(SB_BUFFER(__US_EDMA_LLE_BASE),
			EDMA_LL_PTR_LOW, base);
	else
		vrx318_w32(__US_EDMA_LLE_FPI_BASE, EDMA_LL_PTR_LOW, base);

	vrx318_w32(0, EDMA_LL_PTR_HIGH, base);

	/* Setup Read Channel Link List Elements */
	dword_clear(&edma_lle_link, sizeof(edma_lle_link));
	edma_lle_link.tcb = 1;
	edma_lle_link.llp = 1;
	if (lle_in_sb == 1) {
		edma_lle_link.lle_ptr_low = SB_BUFFER(__US_EDMA_LLE_BASE);
		dst_addr = SB_BUFFER(__US_EDMA_LLE_BASE);
	} else {
		edma_lle_link.lle_ptr_low = __US_EDMA_LLE_FPI_BASE;
		dst_addr = __US_EDMA_LLE_FPI_BASE;
	}

	dword_ep_clear(dst_addr,
		(EDMA_RCH_DATA_LLE_NUM * sizeof(edma_lle_data_t)),
		base);
	dst_addr += EDMA_RCH_DATA_LLE_NUM * sizeof(edma_lle_data_t);
	dword_mem_write(dst_addr, &edma_lle_link, sizeof(edma_lle_link), base);

	/* Setup EDMA Read Channel Context */
	dword_clear(&edma_ch_ctxt, sizeof(edma_ch_ctxt));
	edma_ch_ctxt.edma_ch_type = EDMA_READ_CH;
	edma_ch_ctxt.edma_pcs = 1;
	edma_ch_ctxt.edma_lle_num = EDMA_RCH_DATA_LLE_NUM;
	edma_ch_ctxt.edma_lle_sb_size = EDMA_RCH_DATA_LLE_NUM * 6;
	edma_ch_ctxt.edma_lle_sb_base = __US_EDMA_LLE_BASE;
	edma_ch_ctxt.edma_lle_ext_sb_base = __US_EDMA_LLE_EXT_BASE;
	if (lle_in_sb == 1)
		edma_ch_ctxt.edma_lle_fpi_base
			= SB_BUFFER(__US_EDMA_LLE_BASE);
	else
		edma_ch_ctxt.edma_lle_fpi_base
			= __US_EDMA_LLE_FPI_BASE;

	edma_ch_ctxt.edma_ch_status = EDMA_STOPPED;
	dword_mem_write(SB_BUFFER(__US_EDMA_READ_CH_CFG_CTXT), &edma_ch_ctxt,
			sizeof(edma_ch_ctxt), base);

	/* Enable eDMA Read Channel */
	vrx318_w32(1, EDMA_RCH_EN, base);

	return;
}

/**
 *  Initialize Fastpath descriptor queue config contxt
 *  Initialize dest list of the desq_cfg_ctxt
 */
static void vrx318_us_fastpath_desq_cfg_ctxt_init(struct vrx318_priv *priv, u32 base)
{
	desq_cfg_ctxt_t desq_cfg;
	int i;
	u32 desc_addr;
	rx_descriptor_t rx_desc;

	if (!priv->soc_cfg.soc_fp_us_dnum)
		return;

	dword_clear(&desq_cfg, sizeof(desq_cfg));
	/* Initialize US Fast-Path Descriptor Queue Config/Context */
	desq_cfg.des_in_own_val	= US_FASTPATH_DES_OWN;
	desq_cfg.fast_path	= 1;
	desq_cfg.mbox_int_en	= 0;
	desq_cfg.des_sync_needed = 1;
	desq_cfg.des_num	= priv->soc_cfg.soc_fp_us_dnum; /* max: 64 */
	desq_cfg.des_base_addr	= __US_FAST_PATH_DES_LIST_BASE;

	dword_mem_write(SB_BUFFER(__US_FP_INQ_DES_CFG_CTXT),
		&desq_cfg, sizeof(desq_cfg), base);

	/* Initialize Descriptors in Shadow Fast Path Descriptor List */
	/* Shadow descriptor list of SoC, only own bit is required for init */
	dword_clear(&rx_desc, sizeof(rx_desc));
	rx_desc.own = !desq_cfg.des_in_own_val;
	desc_addr = desq_cfg.des_base_addr;
	for (i = 0; i < desq_cfg.des_num; i++) {
		dword_mem_write(SB_BUFFER(desc_addr),
			&rx_desc, sizeof(rx_desc), base);
		desc_addr += sizeof(rx_desc)/sizeof(u32);
	}
}

/**
 *  Initialize CPU path descriptor queue config contxt
 *  Initialize dest list of the desq_cfg_ctxt
 */
static void vrx318_us_cpupath_desq_cfg_ctxt_init(struct vrx318_priv *priv, u32 base)
{
	desq_cfg_ctxt_t desq_cfg;
	int i;
	u32 desc_addr;
	rx_descriptor_t rx_desc;

	if (!priv->soc_cfg.soc_cpu_us_dnum)
		return;

	dword_clear(&desq_cfg, sizeof(desq_cfg));
	/* Initialize US CPU-Path Descriptor Queue Config/Context */
	desq_cfg.des_in_own_val	= US_CPUPATH_DES_OWN;
	desq_cfg.fast_path	= 0;
	desq_cfg.mbox_int_en	= 0;
	desq_cfg.des_sync_needed = 1;
	desq_cfg.des_num	= priv->soc_cfg.soc_cpu_us_dnum; /* max: 64 */
	desq_cfg.des_base_addr	= __US_CPU_PATH_DEST_LIST_BASE;

	dword_mem_write(SB_BUFFER(__US_CPU_INQ_DES_CFG_CTXT),
		&desq_cfg, sizeof(desq_cfg), base);

	/* Initialize Descriptors in Shadow CPU Path Descriptor List */
	/* Shadow descriptor list of SoC, only own bit is required for init */
	dword_clear(&rx_desc, sizeof(rx_desc));
	rx_desc.own = !desq_cfg.des_in_own_val;
	desc_addr = desq_cfg.des_base_addr;
	for (i = 0; i < desq_cfg.des_num; i++) {
		dword_mem_write(SB_BUFFER(desc_addr),
			&rx_desc, sizeof(rx_desc), base);
		desc_addr += sizeof(rx_desc)/sizeof(u32);
	}
}

static void vrx318_ds_pkt_desq_cfg_ctxt_init(struct vrx318_priv *priv, u32 base)
{
	desq_cfg_ctxt_t desq_cfg;
	int i;
	u32 desc_addr, *soc_ds_base;
	rx_descriptor_t rx_desc;
	dma_addr_t phy_addr;

	if (!priv->soc_cfg.soc_ds_dnum)
		return;

	dword_clear(&desq_cfg, sizeof(desq_cfg));
	/* Initialize DS PKT Descriptor Queue Config/Context */
	desq_cfg.des_in_own_val	= DS_PKT_DES_OWN;
	desq_cfg.fast_path	= 0;
	desq_cfg.mbox_int_en	= 0;
	desq_cfg.des_sync_needed = 1;
	desq_cfg.des_num	= priv->soc_cfg.soc_ds_dnum;
	desq_cfg.des_base_addr	= __DS_PKT_DES_LIST_BASE;

	dword_mem_write(SB_BUFFER(__DS_PKT_DESQ_CFG_CTXT),
		&desq_cfg, sizeof(desq_cfg), base);

	/* Initialize Descriptors in Shadow DS Path Descriptor List */
	/**
	* Shadow descriptor list of SoC, in GRX350, the descriptor pointer
	* should be synced from SoC. in GRX330, it should be allocated by
	* VRX318 itself.
	*/
	dword_clear(&rx_desc, sizeof(rx_desc));
	rx_desc.own = !desq_cfg.des_in_own_val;
	rx_desc.sop = 1;
	rx_desc.eop = 1;
	rx_desc.data_len = DMA_PACKET_SIZE;

	desc_addr = desq_cfg.des_base_addr;
	soc_ds_base = (u32 *)priv->soc_cfg.soc_ds_dbase;

	for (i = 0; i < desq_cfg.des_num; i++) {
		if (!priv->param.pmac_en) /* GRX500 */
			rx_desc.data_ptr
				= soc_ds_base[i * 2 + 1] + VRX318_OB_BADDR;
		else { /* GRX330 */
			if (alloc_data_buffer(priv, rx_desc.data_len,
					&phy_addr, NULL)) {
				vrx318_dbg(DBG_INIT, "%s/%d: Alloc buffer failed\n",
					__func__, __LINE__);
				BUG();
				return;
			}
			rx_desc.data_ptr = VRX318_OB_BADDR + phy_addr;
		}
		dword_mem_write(SB_BUFFER(desc_addr),
			&rx_desc, sizeof(rx_desc), base);
		desc_addr += sizeof(rx_desc)/sizeof(u32);
	}
}

/**
 * 1. Initialize FASTPATH/CPU PATH/DS PATH SoC Sync Config Context
 * 2. Initialize FASTPATH/CPU PATH/DS PATH Des Config Context
 * 3. Initialize Descriptor list that associated to the above Des Config Context
 * 4. Initialize (if required) SoC part Descriptor list
 */
void vrx318_des_sync_init(struct vrx318_priv *priv,
			u32 base, int is_bonding, u32 ep_id)
{
	des_sync_cfg_ctxt_t sync_cfg;
	int idx = 0;
	struct vrx318_soc_cfg *soc_cfg = &priv->soc_cfg;

	if ((is_bonding == 0) ||
		((is_bonding == 1) && (ep_id == US_BONDING_MASTER))) {
		/* Fast-Path Sync Config/Context */
		if (soc_cfg->soc_fp_us_dnum) {
			dword_clear(&sync_cfg, sizeof(sync_cfg));
			sync_cfg.sync_type = US_READ_WRITE_SYNC;
			sync_cfg.max_polling_intv = 4;
			sync_cfg.desq_cfg_ctxt = __US_FP_INQ_DES_CFG_CTXT;
			sync_cfg.soc_des_own_val = soc_cfg->soc_us_desc_own;
			sync_cfg.ext_desc_base_addr
				= VRX318_OB_BADDR + soc_cfg->soc_fp_us_phybase;
			dword_mem_write(DES_SYNC_CFG_CTXT(idx),
				&sync_cfg, sizeof(sync_cfg), base);
			idx += 1;
			vrx318_us_fastpath_desq_cfg_ctxt_init(priv, base);
		}

		/* CPU-Path Sync Config/Context */
		if (soc_cfg->soc_cpu_us_dnum) {
			dword_clear(&sync_cfg, sizeof(sync_cfg));
			sync_cfg.sync_type = US_READ_WRITE_SYNC;
			sync_cfg.max_polling_intv = 10;
			sync_cfg.desq_cfg_ctxt = __US_CPU_INQ_DES_CFG_CTXT;
			sync_cfg.soc_des_own_val = soc_cfg->soc_us_desc_own;
			sync_cfg.ext_desc_base_addr
				= VRX318_OB_BADDR + soc_cfg->soc_cpu_us_phybase;
			dword_mem_write(DES_SYNC_CFG_CTXT(idx),
				&sync_cfg, sizeof(sync_cfg), base);
			idx += 1;
			vrx318_us_cpupath_desq_cfg_ctxt_init(priv, base);
		}
	}

	if ((is_bonding == 0) ||
		((is_bonding == 1) && (ep_id == DS_BONDING_MASTER))) {

		/* Downstream Sync Config/Context */
		if (soc_cfg->soc_ds_dnum) {
			sync_cfg.sync_type = DS_WRITE_READ_SYNC;
			sync_cfg.max_polling_intv = 1;
			sync_cfg.desq_cfg_ctxt = __DS_PKT_DESQ_CFG_CTXT;
			sync_cfg.soc_des_own_val = soc_cfg->soc_ds_desc_own;
			sync_cfg.ext_desc_base_addr
				= VRX318_OB_BADDR + soc_cfg->soc_ds_phybase;
			dword_mem_write(DES_SYNC_CFG_CTXT(idx),
				&sync_cfg, sizeof(sync_cfg), base);
			idx += 1;
			vrx318_ds_pkt_desq_cfg_ctxt_init(priv, base);
		}
	}

	return;
}

void vrx318_us_fragq_desq_cfg_ctxt_init(struct vrx318_ptm_tc *priv,
					u32 base, int is_bonding, u32 ep_id)
{
	int i, j;
	unsigned int start_fragq_id;
	desq_cfg_ctxt_t desq_cfg;
	tx_descriptor_t *p_tx_desc;
	rx_descriptor_t rx_desc;
	dma_addr_t phy_addr;
	unsigned int dst_addr;

	if (is_bonding == 0)
		return;

	dword_clear(&desq_cfg, sizeof(desq_cfg));

	if (ep_id == DS_BONDING_MASTER)
		start_fragq_id = 4;
	else
		start_fragq_id = 0;

	for (i = start_fragq_id; i < MAX_FRAGQ_NUM; i++) {
		/* Initialize Upstream Descriptor Queue Config/Context */
		desq_cfg.des_in_own_val  = 1;
		desq_cfg.fast_path       = 0;
		desq_cfg.mbox_int_en     = 0;
		desq_cfg.des_sync_needed = 0;
		desq_cfg.gif_id          = i;
		desq_cfg.des_num         = 16;
		desq_cfg.des_base_addr
			= __US_FRAGQ_DES_LIST_BASE + (i * 3 * desq_cfg.des_num);
		desq_cfg.bp_des_base_addr
			= __US_FRAGQ_DES_LIST_BASE + (i * 3 * desq_cfg.des_num)
				+ (2 * desq_cfg.des_num);

		dword_mem_write(SB_BUFFER(__US_FRAGQ_CFG_CTXT_BASE + (i * (sizeof(desq_cfg) / sizeof(unsigned int)))),
			&desq_cfg, sizeof(desq_cfg), base);

		/* Only OWN bit initialization is required in FragmentQ Descriptor List */
		dword_clear(&rx_desc, sizeof(rx_desc));
		rx_desc.own = !US_FRAGQ_DES_OWN;
		dst_addr = __US_FRAGQ_DES_LIST_BASE;
		for (j = 0; j < desq_cfg.des_num; j++) {
			dword_mem_write(SB_BUFFER(dst_addr), &rx_desc, sizeof(rx_desc), base);
			dst_addr += i * 48 + j * 2;
		}

		if (ep_id == DS_BONDING_MASTER) {
			/* Initialize the Upstream Shadow Descriptors in SoC */
			for (j = 0; j < desq_cfg.des_num; j++) {
				if ((j % 4) == 0) {
					p_tx_desc = (tx_descriptor_t *)(priv->ptm_bnd_cfg.soc_peer_membase
						+ ((i - 4) * (128 * 4)) + ((j / 4) * 64) + 8);
					p_tx_desc->own = !desq_cfg.des_in_own_val;
				}
			}
		}
	}

	phy_addr = dma_map_single(priv->frwk->pdev,
			(void *)priv->ptm_bnd_cfg.soc_peer_membase,
			PAGE_SIZE, DMA_TO_DEVICE);
	if (unlikely(dma_mapping_error(priv->frwk->pdev, phy_addr)))
		BUG();
	dma_unmap_single(priv->frwk->pdev, phy_addr, PAGE_SIZE, DMA_TO_DEVICE);

	return;
}


/**
 * SOC Memory format:
 * Enqueue Counter | Dequeue Counter | Des 0 | BP_Des 0 | Des 1 | BP_Des 1 |
 * Des 2 | BP_Des2 | Des 3 | BP_Des 3 |  res 0 | res 1 |
 * Enequeue counter:  1 DW
 * Dequeue  counter:  1 DW
 * Des x                 :  2 DWs, total 2 * 4 = 8 DWs
 * BP_Des x            :  1 DW,  total 1 * 4 = 4 DW
 * res 0                  :  1 DW
 * res 1                  :  1 DW
 * Total = 16 DWs
 * FW sync structure by structure, which means it will sync
 * 4 Descriptors in one time. Driver only need to set 1st descriptor's owerbit
 * Each GIF has 4 of above structure.  4 * 16 = 64 DWs
 * Total 4 GIFs. so total 4* 64 = 256 DWs.
 *
 * Driver pre-alloc one page(4KB) for bonding descriptor synchronization
 * for US sync, max 512 DWs, by now , we use 256 DWs.
 * for DS sync, max 512 DWs, by now,  we use 256 DWs.
 */
void vrx318_bonding_des_sync_init(struct vrx318_ptm_tc *priv,
				u32 base, int is_bonding, u32 ep_id)
{
	unsigned int i;
	bond_des_sync_cfg_ctxt_t sync_cfg;

	if (is_bonding == 0 || priv->frwk->param.p2p_en)
		return;

	dword_clear(&sync_cfg, sizeof(sync_cfg));

	/* Initialize Upstream Bonding Descriptor Synchronization Data Structures */
	for (i = 0; i < 4; i++) {
		sync_cfg.des_idx = 0;
		sync_cfg.dir = UPSTREAM;
		sync_cfg.state = __BOND_DES_SYNC_IDLE_STATE;
		if (ep_id == US_BONDING_MASTER)
			sync_cfg.sync_type = WRITE_READ_SYNC;
		else
			sync_cfg.sync_type = READ_WRITE_SYNC;
		/*sync only need sync frag q from q4 to q7, that's reason offset (4 * 8) */
		sync_cfg.desq_cfg_ctxt
			= (__US_FRAGQ_CFG_CTXT_BASE + (4 * 8)) + (i * 8);
		sync_cfg.soc_sync_addr = VRX318_OB_BADDR +
			priv->ptm_bnd_cfg.soc_peer_phybase + (i * 64 * 4);
		sync_cfg.cdma_rx_des_dw1
			= SB_BUFFER(__BOND_US_DES_SYNC_BUF_BASE);

		dword_mem_write(BOND_US_DES_SYNC_CFG_CTXT(i),
			&sync_cfg, sizeof(sync_cfg), base);

		vrx318_w32_sb(VRX318_OB_BADDR + priv->ptm_bnd_cfg.soc_peer_phybase + (i * 64 * 4),
			__US_BOND_SOC_SYNC_ADDR_GIF4 + i, base);

		/* Only Initialize 1 US Bonding Descriptor Synchornization Config/Context */
		/* Only one GIF interface available by now */
		i = 3;
	}
	vrx318_w32_sb(SB_BUFFER(__BOND_US_DES_SYNC_BUF_BASE),
		__BOND_US_DES_SYNC_RX_DES_DW1, base);

	/* Initialize Downstream Bonding Descriptor Synchronization Data Structures */
	dword_clear(&sync_cfg, sizeof(sync_cfg));

	sync_cfg.des_idx = 0;
	sync_cfg.dir = DOWNSTREAM;
	sync_cfg.state = __BOND_DES_SYNC_IDLE_STATE;
	if (ep_id == US_BONDING_MASTER)
		sync_cfg.sync_type = WRITE_READ_SYNC;
	else
		sync_cfg.sync_type = READ_WRITE_SYNC;
	sync_cfg.desq_cfg_ctxt = __DS_FRAGQ_CFG_CTXT_BASE;

	sync_cfg.soc_sync_addr
		= VRX318_OB_BADDR + priv->ptm_bnd_cfg.soc_peer_phybase
		+ BOND_SOC_SYNC_US_BUF_SIZE;

	sync_cfg.cdma_rx_des_dw1 = SB_BUFFER(__BOND_DS_DES_SYNC_BUF_BASE);

	dword_mem_write(SB_BUFFER(__BOND_DS_DES_SYNC_CFG_CTXT),
		&sync_cfg, sizeof(sync_cfg), base);

	vrx318_w32_sb(VRX318_OB_BADDR + priv->ptm_bnd_cfg.soc_peer_phybase + BOND_SOC_SYNC_US_BUF_SIZE,
		__DS_BOND_SOC_SYNC_ADDR, base);
	vrx318_w32_sb(VRX318_OB_BADDR + priv->ptm_bnd_cfg.soc_peer_phybase + BOND_SOC_SYNC_US_BUF_SIZE,
		__DS_BOND_SOC_SYNC_ENQ_CNT_ADDR, base);
	vrx318_w32_sb(VRX318_OB_BADDR + priv->ptm_bnd_cfg.soc_peer_phybase + BOND_SOC_SYNC_US_BUF_SIZE + 4,
		__DS_BOND_SOC_SYNC_DEQ_CNT_ADDR, base);

	/* Initialize fragq(GIF0-GIF7) des_cfg_ctxt and descriptor list */
	vrx318_us_fragq_desq_cfg_ctxt_init(priv, base, is_bonding, ep_id);

	return;
}

static void vrx318_us_qos_desq_cfg_ctxt_init(struct vrx318_ptm_tc *priv,
			u32 base, int is_bonding, u32 ep_id)
{
	qosq_cfg_ctxt_t qosq_cfg_ctxt;
	int offset, i;
	rx_descriptor_t rx_desc;
	dma_addr_t phy_addr;
	u32 qnum;

	if ((is_bonding == 1) && (ep_id == DS_BONDING_MASTER))
		return;

	/* Setup QoSQ_CFG_CTXT */
	dword_clear(&qosq_cfg_ctxt, sizeof(qosq_cfg_ctxt));
	qnum = QOSQ_NUM / priv->frwk->param.atm_qos;

	qosq_cfg_ctxt.threshold = 8;
	qosq_cfg_ctxt.des_num	= TOTAL_QOS_DES_NUM / qnum;

	offset = 0;
	for (i = 0; i < qnum; i++) {
		qosq_cfg_ctxt.des_base_addr = __US_QOSQ_DES_LIST_BASE + offset;
		dword_mem_write(SB_BUFFER((__QOSQ_CFG_CTXT_BASE + (i * __QOSQ_CFG_CTXT_SIZE))),
			&qosq_cfg_ctxt, sizeof(qosq_cfg_ctxt), base);
		offset += qosq_cfg_ctxt.des_num * sizeof(qosq_cfg_ctxt) / sizeof(unsigned int);
	}

	dword_clear(&rx_desc, sizeof(rx_desc));
	rx_desc.own = __QOS_DISPATCH_OWN;
	rx_desc.data_len = DMA_PACKET_SIZE;

	/* Initialize QoSQ Descriptors */
	for (i = 0; i < TOTAL_QOS_DES_NUM; i++) {
		if (alloc_data_buffer(priv->frwk, rx_desc.data_len,
			&phy_addr, NULL)) {
			vrx318_dbg(DBG_INIT, "%s/%d: Alloc buffer failed\n",
				__func__, __LINE__);
			BUG();
			return;
		}
		rx_desc.data_ptr = phy_addr + VRX318_OB_BADDR;
		dword_mem_write(SB_BUFFER(__US_QOSQ_DES_LIST_BASE + (i * 2)),
			&rx_desc, sizeof(rx_desc), base);
	}
}

static void vrx318_us_outq_desq_cfg_ctxt_init(struct vrx318_ptm_tc *priv,
			u32 base, int is_bonding, u32 ep_id)
{
	outq_qos_cfg_ctxt_t outq_qos_cfg_ctxt;
	desq_cfg_ctxt_t desq_cfg;
	u32 dst_addr;
	rx_descriptor_t rx_desc;
	dma_addr_t phy_addr;
	int i;

	/* Setup OUTQ_QoS_CFG_CTXT */
	/* NOTE: By default, Shaping & WFQ both are DISABLED!! */
	dword_clear(&outq_qos_cfg_ctxt, sizeof(outq_qos_cfg_ctxt));
	outq_qos_cfg_ctxt.overhd_bytes  = is_bonding ? 20 : 24;

	/* Output Queue 0  --reserve for pre-emption queue */
	outq_qos_cfg_ctxt.qmap                  = priv->frwk->param.q_gamma_map[3];
	outq_qos_cfg_ctxt.l2_shaping_cfg_ptr    = 0;
	outq_qos_cfg_ctxt.l2_shaping_cfg_idx    = QOSQ_PORT_SSID;
	outq_qos_cfg_ctxt.l3_shaping_cfg_ptr    = 0;
	outq_qos_cfg_ctxt.l3_shaping_cfg_idx    = QOSQ_L3_SHAPER_ID;
	outq_qos_cfg_ctxt.desq_cfg_ctxt		= __US_QOS_OUTQ_DES_CFG_CTXT_BASE
			+ sizeof(desq_cfg_ctxt_t)/sizeof(unsigned int);
	dst_addr = __OUTQ_QOS_CFG_CTXT_BASE;
	dword_mem_write(SB_BUFFER(dst_addr), &outq_qos_cfg_ctxt,
		sizeof(outq_qos_cfg_ctxt), base);

	/* Output Queue 1  */
	outq_qos_cfg_ctxt.qmap                  = priv->frwk->param.q_gamma_map[2];
	outq_qos_cfg_ctxt.l2_shaping_cfg_ptr    = 0;
	outq_qos_cfg_ctxt.l2_shaping_cfg_idx    = QOSQ_PORT_SSID + 1;
	outq_qos_cfg_ctxt.l3_shaping_cfg_ptr    = 0;
	outq_qos_cfg_ctxt.l3_shaping_cfg_idx    = QOSQ_L3_SHAPER_ID;
	outq_qos_cfg_ctxt.desq_cfg_ctxt         = __US_QOS_OUTQ_DES_CFG_CTXT_BASE;
	dst_addr                                += sizeof(outq_qos_cfg_ctxt_t)/sizeof(unsigned int);
	dword_mem_write(SB_BUFFER(dst_addr), &outq_qos_cfg_ctxt,
		sizeof(outq_qos_cfg_ctxt), base);

	/* Output Queue 2 -->Point to Q1 desc queue */
	outq_qos_cfg_ctxt.qmap                  = priv->frwk->param.q_gamma_map[1];
	outq_qos_cfg_ctxt.l2_shaping_cfg_ptr    = 0;
	outq_qos_cfg_ctxt.l2_shaping_cfg_idx    = QOSQ_PORT_SSID + 2;
	outq_qos_cfg_ctxt.l3_shaping_cfg_ptr    = 0;
	outq_qos_cfg_ctxt.l3_shaping_cfg_idx    = QOSQ_L3_SHAPER_ID;
	dst_addr                                += sizeof(outq_qos_cfg_ctxt_t)/sizeof(unsigned int);
	dword_mem_write(SB_BUFFER(dst_addr), &outq_qos_cfg_ctxt,
		sizeof(outq_qos_cfg_ctxt), base);

	/* Output Queue 3 --> Point to Q1 desc queue */
	outq_qos_cfg_ctxt.qmap                  = priv->frwk->param.q_gamma_map[0];
	outq_qos_cfg_ctxt.l2_shaping_cfg_ptr    = 0;
	outq_qos_cfg_ctxt.l2_shaping_cfg_idx    = QOSQ_PORT_SSID + 3;
	outq_qos_cfg_ctxt.l3_shaping_cfg_ptr    = 0;
	outq_qos_cfg_ctxt.l3_shaping_cfg_idx    = QOSQ_L3_SHAPER_ID;
	dst_addr                                += sizeof(outq_qos_cfg_ctxt_t)/sizeof(unsigned int);
	dword_mem_write(SB_BUFFER(dst_addr), (&outq_qos_cfg_ctxt),
		sizeof(outq_qos_cfg_ctxt), base);

	/* Setup DESQ_CFG_CTXT for Output Queues */
	dword_clear(&desq_cfg, sizeof(desq_cfg));

	desq_cfg.des_in_own_val	= US_OUTQ_DES_OWN;
	desq_cfg.mbox_int_en	= 0;
	desq_cfg.des_sync_needed = 0;
	/**
	* set to 32 in order to improve performance for 129-byte frame size;
	* Please note, the BC1's memory space is overwritten by BC0, and BC1
	* use illegal memory space.
	*/
	desq_cfg.des_num = 32;
	desq_cfg.des_base_addr = __US_OUTQ_DES_LIST_BASE;
	dst_addr = __US_QOS_OUTQ_DES_CFG_CTXT_BASE;
	dword_mem_write(SB_BUFFER(dst_addr), &desq_cfg, sizeof(desq_cfg), base);

	desq_cfg.des_base_addr += desq_cfg.des_num * 2;
	dst_addr += sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
	dword_mem_write(SB_BUFFER(dst_addr), &desq_cfg, sizeof(desq_cfg), base);

	desq_cfg.des_base_addr += desq_cfg.des_num * 2;
	dst_addr += sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
	dword_mem_write(SB_BUFFER(dst_addr), &desq_cfg, sizeof(desq_cfg), base);

	desq_cfg.des_base_addr += desq_cfg.des_num * 2;
	dst_addr += sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
	dword_mem_write(SB_BUFFER(dst_addr), &desq_cfg, sizeof(desq_cfg), base);

	/* Initialize the outq descriptor list */
	dword_clear(&rx_desc, sizeof(rx_desc));
	rx_desc.own = !desq_cfg.des_in_own_val;
	rx_desc.data_len = DMA_PACKET_SIZE;

	/* Initialize QoSQ Descriptors */
	for (i = 0; i < OUTQ_NUM; i++) {
		if (alloc_data_buffer(priv->frwk, rx_desc.data_len,
			&phy_addr, NULL)) {
			vrx318_dbg(DBG_INIT, "%s/%d: Alloc buffer failed\n",
				__func__, __LINE__);
			BUG();
			return;
		}
		rx_desc.data_ptr = phy_addr + VRX318_OB_BADDR;
		dword_mem_write(SB_BUFFER(__US_OUTQ_DES_LIST_BASE + (i * 2)),
			&rx_desc, sizeof(rx_desc), base);
	}

	return;
}
/*
* Initialize QoS related configuration for VRX318
* Initializes the below PPE FW Data Structures
* 1. INQ_QoS_CFG
* 2. QoSQ_CFG_CTXT
* 3. OUTQ_QoS_CFG_CTXT
* 4. SHAPING_WFQ_CFG
* 5. QOSQ_MIB
* 6. QOSQ_FLOW_CTRL_CFG
* 7. STD_DES_CFG
* Single line applicaiton : MUST be called
* Bonding application     : MUST be called for US_BONDING_MASTER
*		MUST NOT be called for DS_BONDING_MASTER
*/
void vrx318_us_qos_cfg_init(struct vrx318_ptm_tc *priv,
			u32 base, int is_bonding, u32 ep_id)
{
	inq_qos_cfg_t fp_qos_cfg, cpu_qos_cfg;

	if ((is_bonding == 1) && (ep_id == DS_BONDING_MASTER))
		return;

	/* Setup INQ_QoS_CFG for Fast-Path & CPU-Path */
	dword_clear(&fp_qos_cfg, sizeof(fp_qos_cfg));
	dword_clear(&cpu_qos_cfg, sizeof(cpu_qos_cfg));

	/* By default, support 8 queues only */
	if (priv->frwk->soc_cfg.soc_fp_us_dnum) {
		fp_qos_cfg.qos_en         = 1;
		fp_qos_cfg.qid_mask       = QOSQ_ID_MASK;
		fp_qos_cfg.qosq_base_qid  = 0;
		fp_qos_cfg.desq_cfg_ctxt  = __US_FP_INQ_DES_CFG_CTXT;
		dword_mem_write(INQ_QOS_CFG_BASE(0), &fp_qos_cfg,
				sizeof(fp_qos_cfg), base);
	}

	if (priv->frwk->soc_cfg.soc_cpu_us_dnum) {
		cpu_qos_cfg.qos_en        = 1;
		cpu_qos_cfg.qid_mask      = QOSQ_ID_MASK;
		cpu_qos_cfg.qosq_base_qid = 0;
		cpu_qos_cfg.desq_cfg_ctxt = __US_CPU_INQ_DES_CFG_CTXT;
		dword_mem_write(INQ_QOS_CFG_BASE(1), &cpu_qos_cfg,
				 sizeof(cpu_qos_cfg), base);
	}

	vrx318_us_qos_desq_cfg_ctxt_init(priv, base, is_bonding, ep_id);

	vrx318_us_outq_desq_cfg_ctxt_init(priv, base, is_bonding, ep_id);

	return;
}

static void vrx318_us_bg_ctxt_init(u32 base, int is_bonding, int ep_id)
{
	int i;
	us_bg_ctxt_t us_bg_ctxt;
	u32 dst_addr;

	if (!is_bonding || ep_id == DS_BONDING_MASTER)
		return;

	dword_clear(&us_bg_ctxt, sizeof(us_bg_ctxt));
	for (i = 0; i < 4; i++) {
		us_bg_ctxt.desq_cfg_ctxt_ptr = __US_QOS_OUTQ_DES_CFG_CTXT_BASE
			+ (i * (sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int)));
		dst_addr = SB_BUFFER(__US_BG_CTXT_BASE + (i * (sizeof(us_bg_ctxt_t) / sizeof(unsigned int))));
		dword_mem_write(dst_addr, &us_bg_ctxt, sizeof(us_bg_ctxt_t), base);
	}

	return;
}

static void vrx318_ds_bg_ctxt_init(u32 base, int is_bonding, int ep_id)
{
	int i;
	u32 dst_addr;
	ds_bg_ctxt_t ds_bg_ctxt;

	if (!is_bonding || ep_id == US_BONDING_MASTER)
		return;

	dword_clear(&ds_bg_ctxt, sizeof(ds_bg_ctxt_t));
	ds_bg_ctxt.last_eop = 1;

	dst_addr = __DS_BG_CTXT_BASE;
	for (i = 0; i < 4; i++) {
		dword_mem_write(SB_BUFFER(dst_addr), &ds_bg_ctxt, sizeof(ds_bg_ctxt_t), base);
		dst_addr = dst_addr + sizeof(ds_bg_ctxt_t)/sizeof(unsigned int);
	}

	return;
}

/**
 * Total 256 free descriptor, 9(1 free frag link list, 8 (2(BC) * 4(GIF)) frag link list
 * 256 free desciptor in free frag link list on initialization.
 */
void vrx318_ds_ll_ctxt_init(struct vrx318_priv *priv,
			u32 base, int is_bonding, int ep_id)
{
	int i;
	u32 dst_addr;
	ds_bond_gif_ll_ctxt_t ds_bond_gif_ll_ctxt;
	ds_bond_gif_ll_des_t ll_desc;
	dma_addr_t phy_addr;

	if (!is_bonding || ep_id == US_BONDING_MASTER)
		return;

	dword_clear(&ds_bond_gif_ll_ctxt, sizeof(ds_bond_gif_ll_ctxt_t));

	ds_bond_gif_ll_ctxt.max_des_num	= 128;
	ds_bond_gif_ll_ctxt.to_buff_thres = 16; /* when free_des_num <= 16, timeout any frag */
	ds_bond_gif_ll_ctxt.max_delay	= 1000; /*  TODO: fix the number */

	dst_addr = __DS_BOND_LL_CTXT_BASE;
	for (i = 0; i < 9; i++) {
		dword_mem_write(SB_BUFFER(dst_addr), &ds_bond_gif_ll_ctxt, sizeof(ds_bond_gif_ll_ctxt_t), base);
		dst_addr += sizeof(ds_bond_gif_ll_ctxt_t)/sizeof(unsigned int);
	}

	ds_bond_gif_ll_ctxt.head_ptr = __DS_BOND_GIF_LL_DES_BA;
	ds_bond_gif_ll_ctxt.tail_ptr = __DS_BOND_GIF_LL_DES_BA + (255 * 2);
	ds_bond_gif_ll_ctxt.des_num = 256;
	dword_mem_write(SB_BUFFER(__DS_BOND_LL_CTXT_BASE),
		&ds_bond_gif_ll_ctxt,
		sizeof(ds_bond_gif_ll_ctxt), base);

	/* Initialize DS GIF LL Descriptors */
	for (i = 0; i < DS_BOND_GIF_LL_DES_LEN; i++) {/* link list, last one pointer back to the first one */
		ll_desc.next_des_ptr = __DS_BOND_GIF_LL_DES_BA + (((i + 1) & 0xFF) << 1);
		ll_desc.data_len = DMA_PACKET_SIZE;
		if (alloc_data_buffer(priv, ll_desc.data_len,
			&phy_addr, NULL)) {
			vrx318_dbg(DBG_ERR, "%s/%d: Alloc buffer failed\n",
				__func__, __LINE__);
			BUG();
			return;
		}
		ll_desc.data_ptr = VRX318_OB_BADDR + phy_addr;
		dword_mem_write(DS_BOND_GIF_LL_DES_BA(i), &ll_desc,
			sizeof(ll_desc), base);
	}

	return;
}

void vrx318_edma_copy_ch_init(u32 base, int is_bonding, u32 ep_id, int cdma_write_data_en)
{
	edma_copy_ch_cfg_t copy_ch_cfg;
	desq_cfg_ctxt_t local_desq_cfg_ctxt;
	int i, offset;
	unsigned int des_cnt;
	unsigned int us_des_alloc[] = {15, 3, 0, 0};
	u32 dst_addr;

	if (is_bonding == 0) {/* TODO: edma copy channel must be continues setting ? */
		/* Setup 4 UpStream eDMA Copy Channel 1 for each GIF */
		for (i = 0; i < 4; i++) {
			if (!us_des_alloc[i])
				continue;
			offset = i * sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
			copy_ch_cfg.srcq_ctxt_ptr
				= __US_QOS_OUTQ_DES_CFG_CTXT_BASE + offset;
			copy_ch_cfg.dstq_ctxt_ptr
				= __US_TC_LOCAL_Q_CFG_CTXT_BASE + offset;
			dst_addr = SB_BUFFER(__US_EDMA_COPY_CH_CFG + i * sizeof(edma_copy_ch_cfg_t) / sizeof(unsigned int));
			dword_mem_write(dst_addr, &copy_ch_cfg, sizeof(copy_ch_cfg), base);
		}

		/* Setup the Local DESQ Configuration/Context for 4 UpStream Queues */
		dword_clear(&local_desq_cfg_ctxt, sizeof(local_desq_cfg_ctxt));
		local_desq_cfg_ctxt.des_in_own_val = US_LOCALQ_DES_OWN;
		des_cnt = 0;
		for (i = 0; i < 4; i++) {
			local_desq_cfg_ctxt.des_num = us_des_alloc[i];
			local_desq_cfg_ctxt.des_base_addr = __US_TC_LOCAL_Q_DES_LIST_BASE + (des_cnt * 2);
			dst_addr = SB_BUFFER(__US_TC_LOCAL_Q_CFG_CTXT_BASE + (i * (sizeof(desq_cfg_ctxt_t)/sizeof(unsigned int))));
			dword_mem_write(dst_addr, &local_desq_cfg_ctxt, sizeof(desq_cfg_ctxt_t), base);
			des_cnt += us_des_alloc[i];
		}

		if (cdma_write_data_en == 0) {
			/* Setup 1 DownStream eDMA Copy Channel */
			copy_ch_cfg.srcq_ctxt_ptr = __DS_TC_LOCAL_Q_CFG_CTXT;
			copy_ch_cfg.dstq_ctxt_ptr = __DS_PKT_DESQ_CFG_CTXT;
			dword_mem_write(SB_BUFFER(__DS_EDMA_COPY_CH_CFG), &copy_ch_cfg,
					sizeof(copy_ch_cfg), base);
		}

		/* Setup the Local DESQ Configuration/Context for DownStream Queues */
		dword_clear(&local_desq_cfg_ctxt, sizeof(local_desq_cfg_ctxt));
		local_desq_cfg_ctxt.des_in_own_val = cdma_write_data_en ? 1 : 0;
		local_desq_cfg_ctxt.des_num = __DS_TC_LOCAL_Q_DES_LIST_NUM;
		local_desq_cfg_ctxt.des_base_addr = __DS_TC_LOCAL_Q_DES_LIST_BASE;
		dword_mem_write(SB_BUFFER(__DS_TC_LOCAL_Q_CFG_CTXT),
			&local_desq_cfg_ctxt, sizeof(local_desq_cfg_ctxt), base);
	} else if ((is_bonding == 1) && (ep_id == US_BONDING_MASTER)) {
		/* Setup 4 UpStream eDMA Copy Channel 1 for each GIF */
		for (i = 0; i < 4; i++) {
			if (!us_des_alloc[i])
				continue;
			offset = i * sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
			copy_ch_cfg.srcq_ctxt_ptr
				= __US_FRAGQ_CFG_CTXT_BASE + offset;
			copy_ch_cfg.dstq_ctxt_ptr
				= __US_TC_LOCAL_Q_CFG_CTXT_BASE + offset;
			dst_addr = SB_BUFFER(__US_EDMA_COPY_CH_CFG + i * sizeof(edma_copy_ch_cfg_t) / sizeof(unsigned int));
			dword_mem_write(dst_addr, &copy_ch_cfg, sizeof(copy_ch_cfg), base);
		}

		/*Setup the Local DESQ Configuration/Context for 4 UpStream Queues*/
		dword_clear(&local_desq_cfg_ctxt, sizeof(local_desq_cfg_ctxt));
		local_desq_cfg_ctxt.des_in_own_val = 1;
		des_cnt = 0;
		for (i = 0; i < 4; i++) {
			local_desq_cfg_ctxt.des_num = us_des_alloc[i];
			local_desq_cfg_ctxt.des_base_addr = __US_TC_LOCAL_Q_DES_LIST_BASE + (des_cnt * 2);
			local_desq_cfg_ctxt.bp_des_base_addr = __US_BP_TC_LOCAL_Q_CFG_CTXT_BASE + (des_cnt * 1);
			dst_addr = SB_BUFFER(__US_TC_LOCAL_Q_CFG_CTXT_BASE + (i * (sizeof(desq_cfg_ctxt_t)/sizeof(unsigned int))));
			dword_mem_write(dst_addr, (&local_desq_cfg_ctxt), sizeof(desq_cfg_ctxt_t), base);
			des_cnt = des_cnt + us_des_alloc[i];
		}

		/*Setup 1 DownStream eDMA Copy Channel*/
		copy_ch_cfg.srcq_ctxt_ptr = __DS_TC_LOCAL_Q_CFG_CTXT;
		copy_ch_cfg.dstq_ctxt_ptr = __DS_FRAGQ_CFG_CTXT_BASE;
		dword_mem_write(SB_BUFFER(__DS_EDMA_COPY_CH_CFG), &copy_ch_cfg, sizeof(copy_ch_cfg), base);

		/*Setup the Local DESQ Configuration/Context for DownStream Queues*/
		dword_clear(&local_desq_cfg_ctxt, sizeof(local_desq_cfg_ctxt));
		local_desq_cfg_ctxt.des_in_own_val = cdma_write_data_en ? 1 : 0;
		local_desq_cfg_ctxt.des_num = __DS_TC_LOCAL_Q_DES_LIST_NUM;
		local_desq_cfg_ctxt.des_base_addr = __DS_TC_LOCAL_Q_DES_LIST_BASE;
		local_desq_cfg_ctxt.bp_des_base_addr = __DS_TC_LOCAL_Q_DES_LIST_BASE + (16 * 2);
		dst_addr = SB_BUFFER(__DS_TC_LOCAL_Q_CFG_CTXT);
		dword_mem_write(dst_addr, &local_desq_cfg_ctxt, sizeof(desq_cfg_ctxt_t), base);

	} else if ((is_bonding == 1) && (ep_id == DS_BONDING_MASTER)) {

		/* Setup 4 UpStream eDMA Copy Channel 1 for each GIF */
		/*DS Master copy channel fraq start from 4 to 7 */
		for (i = 0; i < 4; i++) {
			if (!us_des_alloc[i])
				continue;
			copy_ch_cfg.srcq_ctxt_ptr = __US_FRAGQ_CFG_CTXT_BASE
				+ (i + 4) * sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
			copy_ch_cfg.dstq_ctxt_ptr = __US_TC_LOCAL_Q_CFG_CTXT_BASE
				+ i * sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
			dst_addr = SB_BUFFER(__US_EDMA_COPY_CH_CFG + i * sizeof(edma_copy_ch_cfg_t) / sizeof(unsigned int));
			dword_mem_write(dst_addr, &copy_ch_cfg, sizeof(copy_ch_cfg), base);
		}

		/* Setup the Local DESQ Configuration/Context for 4 UpStream Queues */
		dword_clear(&local_desq_cfg_ctxt, sizeof(local_desq_cfg_ctxt));
		local_desq_cfg_ctxt.des_in_own_val = 1;
		des_cnt = 0;
		for (i = 0; i < 4; i++) {
			local_desq_cfg_ctxt.des_num = us_des_alloc[i];
			local_desq_cfg_ctxt.des_base_addr = __US_TC_LOCAL_Q_DES_LIST_BASE + (des_cnt * 2);
			local_desq_cfg_ctxt.bp_des_base_addr = __US_BP_TC_LOCAL_Q_CFG_CTXT_BASE + (des_cnt * 1);
			dst_addr = SB_BUFFER(__US_TC_LOCAL_Q_CFG_CTXT_BASE + (i * (sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int))));
			dword_mem_write(dst_addr, (unsigned int *)(&local_desq_cfg_ctxt), sizeof(desq_cfg_ctxt_t), base);
			des_cnt = des_cnt + us_des_alloc[i];
		}

		if (cdma_write_data_en == 0) {
			/* Setup 1 DownStream eDMA Copy Channel */
			/* DownStream Master use eDMA copy channel to copy data from local to ds_frag_des_list2 */
			copy_ch_cfg.srcq_ctxt_ptr = __DS_TC_LOCAL_Q_CFG_CTXT;
			/* offset 0 is reserved for US master's downstream data */
			copy_ch_cfg.dstq_ctxt_ptr = __DS_FRAGQ_CFG_CTXT_BASE + sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
			dst_addr = SB_BUFFER(__DS_EDMA_COPY_CH_CFG);
			dword_mem_write(dst_addr, &copy_ch_cfg, sizeof(copy_ch_cfg), base);
		}

		/* Setup the Local DESQ Configuration/Context for DownStream Queues */
		dword_clear(&local_desq_cfg_ctxt, sizeof(local_desq_cfg_ctxt));
		local_desq_cfg_ctxt.des_in_own_val = cdma_write_data_en ? 1 : 0;
		local_desq_cfg_ctxt.des_num = __DS_TC_LOCAL_Q_DES_LIST_NUM;
		local_desq_cfg_ctxt.des_base_addr = __DS_TC_LOCAL_Q_DES_LIST_BASE;
		local_desq_cfg_ctxt.bp_des_base_addr = __DS_TC_LOCAL_Q_DES_LIST_BASE + (16 * 2);
		dst_addr = SB_BUFFER(__DS_TC_LOCAL_Q_CFG_CTXT);
		dword_mem_write(dst_addr, &local_desq_cfg_ctxt, sizeof(desq_cfg_ctxt_t), base);
	}

	return;
}

/**
 * Initialize VRX318 TC Local Descriptor List
 * Performs below actions
 *      (1) Allocate and Initialize TC UpStream Local Descriptors
 *      (2) Allocate and Initialize TC DownStream Local Descriptors
 *      Before PPA2.16: US: EDMA read, DS: EDMA write
 *      PPA2.16: US: EDMA read, DS: CDMA write
 */
void vrx318_local_des_init(u32 base, int cdma_write_data_en)
{
	int i;
	u32 dst_addr, ds_local_addr;
	rx_descriptor_t rx_desc;

	dword_clear(&rx_desc, sizeof(rx_desc));
	rx_desc.own = !US_LOCALQ_DES_OWN;
	rx_desc.data_len = DMA_PACKET_SIZE;

	/* Initialize UpStream Descriptors */
	for (i = 0; i < __US_TC_LOCAL_Q_DES_LIST_NUM; i++) {
		rx_desc.data_ptr = (PDBRAM_TX_PKT_BUF_BASE + (i * DMA_PACKET_SIZE));
		dst_addr = SB_BUFFER(__US_TC_LOCAL_Q_DES_LIST_BASE + (i * 2));
		dword_mem_write(dst_addr, &rx_desc, sizeof(rx_desc), base);
	}

	dword_clear(&rx_desc, sizeof(rx_desc));
	/* CDMA: own: 0--->CPU, 1--->DMA */
	rx_desc.own = !cdma_write_data_en;
	rx_desc.data_len = DMA_PACKET_SIZE;

	/* Initialize DownStream Descriptors */
	ds_local_addr = PDBRAM_TX_PKT_BUF_BASE + __US_TC_LOCAL_Q_DES_LIST_NUM * DMA_PACKET_SIZE;
	for (i = 0; i < __DS_TC_LOCAL_Q_DES_LIST_NUM; i++) {
		rx_desc.data_ptr = (ds_local_addr + (i * DMA_PACKET_SIZE));
		dst_addr = SB_BUFFER(__DS_TC_LOCAL_Q_DES_LIST_BASE + (i * 2));
		dword_mem_write(dst_addr, &rx_desc, sizeof(rx_desc), base);
	}

	return;
}

void vrx318_ds_fragq_desq_cfg_ctxt_init(struct vrx318_ptm_tc *priv,
					u32 base, int is_bonding, u32 ep_id)
{
	int i;
	u32 dst_addr;
	unsigned int end_fragq_id = 1;
	desq_cfg_ctxt_t desq_cfg;
	rx_descriptor_t rx_desc, *p_rx_desc;
	dma_addr_t phy_addr;

	if (!is_bonding)
		return;

	dword_clear(&desq_cfg, sizeof(desq_cfg));

	if (ep_id == DS_BONDING_MASTER)
		end_fragq_id = 2;
	else if (ep_id == US_BONDING_MASTER)
		end_fragq_id = 1;

	for (i = 0; i < end_fragq_id; i++) {
		/* Initialize Downstream Descriptor Queue Config/Context */
		desq_cfg.des_in_own_val  = DS_FRAGQ_DES_OWN;
		desq_cfg.fast_path       = 0;
		desq_cfg.mbox_int_en     = 0;
		desq_cfg.des_sync_needed = 0;
		desq_cfg.gif_id          = (i == 1) ? 4 : 0;
		desq_cfg.des_num         = (i == 0) ?
			DS_FRAG_DES_LIST1_LEN : DS_FRAG_DES_LIST2_LEN;
		desq_cfg.des_base_addr   = __DS_FRAGQ_DES_LIST_BASE
				+ (i * 3 * desq_cfg.des_num);
		desq_cfg.bp_des_base_addr = __DS_FRAGQ_DES_LIST_BASE
				+ (i * 3 * desq_cfg.des_num) + (2 * desq_cfg.des_num);

		dst_addr = SB_BUFFER(__DS_FRAGQ_CFG_CTXT_BASE + (i * (sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int))));
		dword_mem_write(dst_addr, &desq_cfg, sizeof(desq_cfg), base);
	}
	/* Initialize OWN bit of all the Descriptors in FragmentQ Descriptor List */
	dword_clear(&rx_desc, sizeof(rx_desc));
	rx_desc.own = !desq_cfg.des_in_own_val;
	rx_desc.data_len = DMA_PACKET_SIZE;

	if (ep_id == US_BONDING_MASTER) {
		dword_mem_read(&desq_cfg, SB_BUFFER(__DS_FRAGQ_CFG_CTXT_BASE),
				sizeof(desq_cfg), base);
	} else if (ep_id == DS_BONDING_MASTER) {
		dword_mem_read(&desq_cfg,
				SB_BUFFER(__DS_FRAGQ_CFG_CTXT_BASE + sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int)),
				sizeof(desq_cfg), base);
	}

	for (i = 0; i < desq_cfg.des_num; i++) {
		dst_addr = desq_cfg.des_base_addr
			+ i * sizeof(rx_desc) / sizeof(unsigned int);
		if (alloc_data_buffer(priv->frwk, DMA_PACKET_SIZE,
				&phy_addr, (unsigned int *)NULL)) {
			vrx318_dbg(DBG_ERR,
				"Failed to alloc buffer for ds fraq des\n");
			BUG();
			return;
		}
		rx_desc.data_ptr
			= (unsigned int)phy_addr + VRX318_OB_BADDR;
		dword_mem_write(SB_BUFFER(dst_addr), &rx_desc,
				sizeof(rx_desc), base);
	}

	if (ep_id == US_BONDING_MASTER) {
		/* Initialize the Downstream Shadow Descriptors in SoC */
		for (i = 0; i < desq_cfg.des_num; i++) {
			if (i % 4 == 0) {
				p_rx_desc = (rx_descriptor_t *)(priv->ptm_bnd_cfg.soc_peer_membase + BOND_SOC_SYNC_US_BUF_SIZE + ((i/4) * 64) + 8);
				p_rx_desc->own = !desq_cfg.des_in_own_val;
			}
		}
		phy_addr = dma_map_single(priv->frwk->pdev,
			(void *)priv->ptm_bnd_cfg.soc_peer_membase,
			PAGE_SIZE, DMA_TO_DEVICE);
		if (unlikely(dma_mapping_error(priv->frwk->pdev, phy_addr)))
			BUG();
		dma_unmap_single(priv->frwk->pdev, phy_addr, PAGE_SIZE, DMA_TO_DEVICE);
	}

	return;
}

void vrx318_cdma_copy_ch_init(u32 base, int is_bonding, u32 ep_id, int cdma_write_data_en)
{
	cdma_copy_ch_cfg_t cdma_ch_cfg;
	u32 dst_addr;

	if (!cdma_write_data_en)
		return;

	dst_addr = SB_BUFFER(__DS_CDMA_COPY_CH_CFG);
	if (is_bonding == 0) {
		cdma_ch_cfg.srcq_ctxt_ptr = __DS_TC_LOCAL_Q_CFG_CTXT;
		cdma_ch_cfg.dstq_ctxt_ptr = __DS_PKT_DESQ_CFG_CTXT;

	} else if (is_bonding == 1 && ep_id == US_BONDING_MASTER) {
		cdma_ch_cfg.srcq_ctxt_ptr = __DS_TC_LOCAL_Q_CFG_CTXT;
		cdma_ch_cfg.dstq_ctxt_ptr = __DS_FRAGQ_CFG_CTXT_BASE;

	} else if (is_bonding == 1 && ep_id == DS_BONDING_MASTER) {
		cdma_ch_cfg.srcq_ctxt_ptr = __DS_TC_LOCAL_Q_CFG_CTXT;
		cdma_ch_cfg.dstq_ctxt_ptr = __DS_FRAGQ_CFG_CTXT_BASE + 8;
	}

	dword_mem_write(dst_addr, &cdma_ch_cfg, sizeof(cdma_ch_cfg), base);

	return;
}

static inline void vrx318_enable_cdma(int cid, u32 base, int en)
{
	u32 reg_val;
	vrx318_w32(cid, DMA_CS, base);

	if (en)
		reg_val = vrx318_r32(DMA_CCTRL, base) | 0x00000001;
	else
		reg_val = vrx318_r32(DMA_CCTRL, base) & (~0x00000001);

	vrx318_w32(reg_val, DMA_CCTRL, base);
}

/* Only 1 descriptor is initialized per copy channel */
static inline void vrx318_cfg_cdma_cpch(int rx_cid, int tx_cid,
					u32 rx_dbase, u32 tx_dbase,
					u32 rx_dnum, u32 tx_dnum, u32 base)
{
	/* Setup RX DMA Channel */
	vrx318_w32(rx_cid, DMA_CS, base);
	vrx318_w32(rx_dbase, DMA_CBA, base);
	vrx318_w32(rx_dnum, DMA_CDLEN, base);

	/* Setup TX DMA Channel */
	vrx318_w32(tx_cid, DMA_CS, base);
	vrx318_w32(tx_dbase, DMA_CBA, base);
	vrx318_w32(tx_dnum, DMA_CDLEN, base);

	vrx318_enable_cdma(rx_cid, base, 1);
	vrx318_enable_cdma(tx_cid, base, 1);

	return;
}

static inline void vrx318_setup_sync_cdma(int rx_cid, int tx_cid,
						int off, u32 base)
{
	u32 rx_dbase, tx_dbase, rx_daddr;
	/* Calculate all the offset for RX descriptor base address, RX data pointer, and Tx descriptor base address*/
	const u32 rx_desc_off = offsetof(des_sync_cfg_ctxt_t, cdma_dst_des_dw0) / 4; /* offset 11 */
	const u32 rx_data_off = offsetof(des_sync_cfg_ctxt_t, rd_des_buf[0]) / 4; /* offset 16 */
	const u32 tx_desc_off = offsetof(des_sync_cfg_ctxt_t, rd_cmd_sar) / 4;  /* offset 6 */

	off = off * sizeof(des_sync_cfg_ctxt_t) / sizeof(int);

	rx_dbase = __DES_SYNC_CFG_CTXT + off + rx_desc_off;
	rx_daddr = __DES_SYNC_CFG_CTXT + off + rx_data_off;
	tx_dbase = __DES_SYNC_CFG_CTXT + off + tx_desc_off;

	/* Setup RX descriptor */
	vrx318_w32_sb(0xF0000040, rx_dbase, base);
	vrx318_w32_sb(SB_BUFFER(rx_daddr), rx_dbase + 1, base);

	/* Setup TX descriptor */
	vrx318_w32_sb(0x0, tx_dbase, base);
	vrx318_w32_sb(0x0, tx_dbase + 1, base);

	vrx318_cfg_cdma_cpch(rx_cid, tx_cid, SB_BUFFER(rx_dbase), SB_BUFFER(tx_dbase), 1, 1, base);
}

void vrx318_cdma_init(struct vrx318_ptm_tc *priv,
	u32 base, int is_bonding, u32 ep_id, int cdma_write_data_en)
{
	u32 reg_val;
	int off;
	u32 rx_dbase, tx_dbase;
	u32 rx_des_off, tx_des_off;

	/* Enable POWER to CDMA */
	reg_val = vrx318_r32(PMU_PWDCR, base) & (~0x00000004);
	vrx318_w32(reg_val, PMU_PWDCR, base);

	vrx318_w32(0x80000040, DMA_CPOLL, base);
	vrx318_w32(0x00000004, DMA_PS, base);
	reg_val = (vrx318_r32(DMA_PCTRL, base) & ~(0x0000003C)) | (0x0000003C);
	vrx318_w32(reg_val, DMA_PCTRL, base);

	/* Setup DMA COPY Channel (RXCH 2 /TXCH 3) : 1 Descriptor */
	off = 0;
	vrx318_setup_sync_cdma(0, 1, off, base);
	off++;

	if ((is_bonding == 0) || (is_bonding == 1 && ep_id == US_BONDING_MASTER)) {
		/* Setup DMA COPY Channel (RXCH 2 /TXCH 3) : 1 Descriptor */
		vrx318_setup_sync_cdma(2, 3, off, base);
		off++;
	} else if (is_bonding == 1 && ep_id == DS_BONDING_MASTER) {
		/* Setup DMA COPY Channel (RXCH 2 /TXCH 3) : 1 Descriptor */
		/* Bonding Descriptor Sync, obesolete */
		/* RXCH: __BOND_US_DES_SYNC_RX_DES_DW0 /__BOND_US_DES_SYNC_RX_DES_DW1 */
		/* TXCH: __BOND_US_DES_SYNC_TX_DES_DW0/ __BOND_US_DES_SYNC_TX_DES_DW1 */
		/* Disable cdma channel by default */
		vrx318_w32_sb(0x0, __BOND_US_DES_SYNC_RX_DES_DW0, base);
		vrx318_w32_sb(0x0, __BOND_US_DES_SYNC_RX_DES_DW1, base);
		vrx318_w32_sb(0x0, __BOND_US_DES_SYNC_TX_DES_DW0, base);
		vrx318_w32_sb(0x0, __BOND_US_DES_SYNC_TX_DES_DW1, base);
		vrx318_cfg_cdma_cpch(2, 3,
				SB_BUFFER(__BOND_US_DES_SYNC_RX_DES_DW0),
				SB_BUFFER(__BOND_US_DES_SYNC_TX_DES_DW0),
				1, 1, base);
		vrx318_enable_cdma(3, base, 0);
		vrx318_enable_cdma(2, base, 0);
	}

	if (is_bonding == 0) {
		/* Setup DMA COPY Channel (RXCH 4 / TXCH 5): 1 Descriptor */
		vrx318_setup_sync_cdma(4, 5, off, base);
		off++;

		if (cdma_write_data_en)
			/*Setup DMA COPY Channel (RXCH 6/ TXCH 7): 1 Descriptor */
			vrx318_cfg_cdma_cpch(6, 7,
				SB_BUFFER(__DS_PKT_DES_LIST_BASE),
				SB_BUFFER(__DS_TC_LOCAL_Q_DES_LIST_BASE),
				priv->frwk->soc_cfg.soc_ds_dnum,
				__DS_TC_LOCAL_Q_DES_LIST_NUM, base);
	}

	if (is_bonding == 1 && ep_id == US_BONDING_MASTER) {
		/* Setup DMA Channel (RXCH 4 / TXCH 5) : 1 Descriptor */
		/* RXCH __BOND_US_DES_SYNC_RX_DES_DW0 / __BOND_US_DES_SYNC_RX_DES_DW1 */
		/* TXCH __BOND_US_DES_SYNC_TX_DES_DW0 / __BOND_US_DES_SYNC_TX_DES_DW1 */
		/* obesolete, Disable cdma channel by default */
		vrx318_w32_sb(0x0, __BOND_US_DES_SYNC_RX_DES_DW0, base);
		vrx318_w32_sb(0x0, __BOND_US_DES_SYNC_RX_DES_DW1, base);
		vrx318_w32_sb(0x0, __BOND_US_DES_SYNC_TX_DES_DW0, base);
		vrx318_w32_sb(0x0, __BOND_US_DES_SYNC_TX_DES_DW1, base);
		vrx318_cfg_cdma_cpch(4, 5,
				SB_BUFFER(__BOND_US_DES_SYNC_RX_DES_DW0),
				SB_BUFFER(__BOND_US_DES_SYNC_TX_DES_DW0),
				1, 1, base);
		vrx318_enable_cdma(5, base, 0);
		vrx318_enable_cdma(4, base, 0);

		if (cdma_write_data_en)
			/* CDMA write ds_tc_local_q_des_list to ds_frag_des_list1(TX CH to RX CH) */
			/* Setup DMA Channel (RXCH 6 / TXCH 7) */
			vrx318_cfg_cdma_cpch(6, 7,
				SB_BUFFER(__DS_FRAGQ_DES_LIST_BASE),
				SB_BUFFER(__DS_TC_LOCAL_Q_DES_LIST_BASE),
				DS_FRAG_DES_LIST1_LEN,
				__DS_TC_LOCAL_Q_DES_LIST_NUM, base);
	}

	if (is_bonding == 1 && ep_id == DS_BONDING_MASTER) {
		/* Setup DMA Channel  (RX CH 4 / TX CH5) : 1 Descriptor */
		/* Bond DS des sync cfg ctxt */
		rx_des_off = offsetof(bond_des_sync_cfg_ctxt_t, cdma_rx_des_dw0) / 4;
		rx_dbase = __BOND_DS_DES_SYNC_CFG_CTXT + rx_des_off;
		vrx318_w32_sb(0x0, rx_dbase, base);
		vrx318_w32_sb(0x0, rx_dbase + 1, base);
		tx_des_off = offsetof(bond_des_sync_cfg_ctxt_t, cdma_tx_des_dw0) / 4;
		tx_dbase = __BOND_DS_DES_SYNC_CFG_CTXT + tx_des_off;
		vrx318_w32_sb(0x0, tx_dbase, base);
		vrx318_w32_sb(0x0, tx_dbase + 1, base);
		vrx318_cfg_cdma_cpch(4, 5, SB_BUFFER(rx_dbase), SB_BUFFER(tx_dbase), 1, 1, base);

		if (cdma_write_data_en) {
			/* CDMA write ds_tc_local_q_des_list to ds_frag_des_list2(TX CH to RX CH) */
			/* Setup DMA Channel (RX CH6 / TX CH7) */
			vrx318_cfg_cdma_cpch(6, 7,
				SB_BUFFER(__DS_FRAGQ_DES_LIST_BASE + (DS_FRAG_DES_LIST1_LEN_MAX * 2)),
				SB_BUFFER(__DS_TC_LOCAL_Q_DES_LIST_BASE),
				DS_FRAG_DES_LIST2_LEN, __DS_TC_LOCAL_Q_DES_LIST_NUM, base);
		}
	}

	/* if peer to peer is not enabled, then the DMA CH 6 & 7 are used to sync the bonding fragment descriptors */
	if (is_bonding == 1 && ep_id == US_BONDING_MASTER && (cdma_write_data_en == 0)) {
		/* Setup DMA Channel (RXCH 6 /TXCH 7) : 1 Descriptor */
		rx_des_off = offsetof(bond_des_sync_cfg_ctxt_t, cdma_rx_des_dw0) / 4;
		rx_dbase = __BOND_DS_DES_SYNC_CFG_CTXT + rx_des_off;
		vrx318_w32_sb(0x0, rx_dbase, base);
		vrx318_w32_sb(0x0, rx_dbase + 1, base);
		tx_des_off = offsetof(bond_des_sync_cfg_ctxt_t, cdma_tx_des_dw0) / 4;
		tx_dbase = __BOND_DS_DES_SYNC_CFG_CTXT + tx_des_off;
		vrx318_w32_sb(0x0, tx_dbase, base);
		vrx318_w32_sb(0x0, tx_dbase + 1, base);
		vrx318_cfg_cdma_cpch(6, 7, SB_BUFFER(rx_dbase), SB_BUFFER(tx_dbase), 1, 1, base);
	}

	return;
}


/*************************************************************************
 *  PTM TC - Initialization : Entry Point
 *  dev         - PCIe endpoint device
 *  peer_dev    - PCIe endpoint device of peer bonding device
 *  tc_mode  - ptm single line /ptm bonding
 *  ep_id   - 0: upstream bonding master, 1: downstream bonding master
 *  sync_buf    - memory base address of bonding US/DS descriptor sync buffer
 *  lle_in_sb   - 0: LLE in PDBRAM, 1: LLE in SB
*************************************************************************/
static int ppe_ptm_fw_hw_init(struct vrx318_ptm_tc *priv,
				u32 ep_id, enum dsl_tc_mode tc_mode)
{
	u32 base;
	int pmac_en, cdma_wr_en, is_bonding, lle_in_sb;
	struct vrx318_priv *frwk_priv;

	frwk_priv = priv->frwk;
	base = (u32)frwk_priv->ep_dev[ep_id].membase;
	pmac_en = frwk_priv->param.pmac_en ? 1 : 0;
	cdma_wr_en = frwk_priv->param.cdma_wr_en ? 1 : 0;
	is_bonding = tc_mode == TC_PTM_BND_MODE ? 1 : 0;
	lle_in_sb = frwk_priv->param.lle_in_sb ? 1 : 0;

	vrx318_sb_clear(base);

	vrx318_tc_hw_init(base, is_bonding);
	vrx318_tc_fw_init(base, is_bonding, pmac_en);

	vrx318_gen_cfg_init(priv, base, is_bonding, ep_id,
		frwk_priv->ep_dev[ep_id].peer_phy_membase[0],
		cdma_wr_en, pmac_en);
	vrx318_edma_init(base, lle_in_sb, cdma_wr_en);

	/* US/DS des sync cfg ctxt init and related desq_cfg_ctxt init */
	vrx318_des_sync_init(frwk_priv, base, is_bonding, ep_id);
	/* Bonding des sync cfg ctxt init */
	vrx318_bonding_des_sync_init(priv, base, is_bonding, ep_id);

	vrx318_us_qos_cfg_init(priv, base, is_bonding, ep_id);

	vrx318_us_bg_ctxt_init(base, is_bonding, ep_id);

	vrx318_edma_copy_ch_init(base, is_bonding, ep_id, cdma_wr_en);

	vrx318_local_des_init(base, cdma_wr_en);

	vrx318_ds_bg_ctxt_init(base, is_bonding, ep_id);
	vrx318_ds_ll_ctxt_init(priv->frwk, base, is_bonding, ep_id);

	vrx318_ds_fragq_desq_cfg_ctxt_init(priv, base, is_bonding, ep_id);

	vrx318_cdma_copy_ch_init(base, is_bonding, ep_id, cdma_wr_en);
	vrx318_cdma_init(priv, base, is_bonding, ep_id, cdma_wr_en);

	return 0;
}

static void pp32_load(u32 base)
{
	int ret;

	/*  download firmware   */
	vrx318_dbg(DBG_INIT, "Download E1 FW\n");
	ret = pp32_download_code(base, 0,
		vrx218_e1_fw_code, ARRAY_SIZE(vrx218_e1_fw_code),
		vrx218_e1_fw_data, ARRAY_SIZE(vrx218_e1_fw_data));
	if (ret) {
		vrx318_dbg(DBG_ERR, "Download E1 FW code fail\n");
		BUG();
	}

	vrx318_dbg(DBG_INIT, "Downdload B1 FW\n");
	ret = pp32_download_code(base, 1,
		vrx218_pp32_1_fw_code, ARRAY_SIZE(vrx218_pp32_1_fw_code),
		vrx218_pp32_1_fw_data, ARRAY_SIZE(vrx218_pp32_1_fw_data));
	if (ret) {
		vrx318_dbg(DBG_ERR, "Download B1 FW code fail\n");
		BUG();
	}
}

static inline void ptm_print_fw_ver(struct vrx318_ptm_tc *priv, int ep_id)
{
	int i;

	for (i = 0; i < 2; i++)
		print_fw_version(NULL, &priv->fw_ver[ep_id][i]);
}

#ifdef CONFIG_LTQ_VRX318_CPUFREQ
extern int dp_coc_new_stat_req(enum ltq_cpufreq_state new_state
	, uint32_t flag);
static void do_coc_tasklet(unsigned long arg)
{
	/* change state to D0 */
	ptm_coc_state = LTQ_CPUFREQ_PS_D0;
	/* call datapath to inform about the new state */
	dp_coc_new_stat_req(ptm_coc_state, DP_COC_REQ_VRX318);
}
#endif

static irqreturn_t vrx318_irq_handler(int irq, void *dev_id)
{
	u32 mbox1_isr, mbox1_ier, mbox1_isrc;
	u32 base, peer_state, peer_base;
	struct vrx318_ptm_tc *priv = g_ptm_tc;
	struct pcie_ep_dev *ep_dev = dev_id;

	base = (u32)ep_dev->membase;
	mbox1_ier = vrx318_r32(MBOX_IGU1_IER, base);
	mbox1_isrc = vrx318_r32(MBOX_IGU1_ISR, base);

	mbox1_isr = mbox1_isrc & mbox1_ier;

	if (mbox1_isr & P2P_STAT_INTR) { /* Bit 4  Peer to Peer link state update */
		if (priv->tc_mode == TC_PTM_BND_MODE) {
			peer_state = vrx318_r32_sb(__PEER_GIF_LINK_STATE_TMP, base);
			peer_base = (u32)ep_dev->peer_membase[0];
			vrx318_w32_sb(peer_state, __PEER_GIF_LINK_STATE, peer_base);
		}
	}
#ifdef CONFIG_LTQ_VRX318_CPUFREQ
	if (mbox1_isr & PKT_RX_INTR) {
		mbox1_ier &= ~PKT_RX_INTR;
		mbox_set_ier(MBOX_IGU1, mbox1_ier, base);
		tasklet_schedule(&g_coc_tasklet);
	}
#endif
	vrx318_w32(mbox1_isrc, MBOX_IGU1_ISRC, base);

	return IRQ_HANDLED;
}

#ifdef CONFIG_LTQ_VRX318_CPUFREQ
static int32_t ptm_coc_confirm_stat(enum ltq_cpufreq_state new_state,
	enum ltq_cpufreq_state old_state,
	uint32_t flags)
{
	struct vrx318_ptm_tc *priv = g_ptm_tc;
	int ret = -ENODEV;
	u32 base;
	int ep_id;

	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;
	vrx318_dbg(DBG_COC, "Current state: %u, old [%u] to [%u]\n",
		ptm_coc_state, (u32)old_state, (u32)new_state);

	if (new_state != ptm_coc_state) {
		/* assign new state */
		vrx318_dbg(DBG_COC, "Change from current state %u to new %u\n",
			ptm_coc_state, new_state);
		ptm_coc_state = new_state;
		/* check for D3 state */
		if (ptm_coc_state == LTQ_CPUFREQ_PS_D3) {
			u32 mbox1_ier;
			vrx318_dbg(DBG_COC, "Change to D3, enable interrupt\n");
			/* Enable interrupt for DS packet */
			mbox1_ier = vrx318_r32(MBOX_IGU1_IER, base);
			mbox_set_ier(MBOX_IGU1, mbox1_ier | PKT_RX_INTR, base);
		} else {
			/* Disable interrupt for DS packet */
			u32 mbox1_ier;
			vrx318_dbg(DBG_COC, "Change to %u, disable interrupt\n", ptm_coc_state);
			/* Disable interrupt for DS packet */
			mbox1_ier = vrx318_r32(MBOX_IGU1_IER, base);
			mbox1_ier &= ~PKT_RX_INTR;
			mbox_set_ier(MBOX_IGU1, mbox1_ier, base);
		}
	}
	vrx318_dbg(DBG_COC, "Complete\n");
	return ret;
}
#endif

static void vrx318_ptm_datapath_init(struct vrx318_ptm_tc *priv,
				enum dsl_tc_mode tc_mode, int ep_id)
{
	const char *dev_name;
	int ret;

	if (tc_mode == TC_PTM_BND_MODE)
		dev_name = g_bnd_dev_name[ep_id];
	else
		dev_name = g_ptm_dev_name;

	ret = devm_request_irq(priv->frwk->pdev, priv->frwk->ep_dev[ep_id].irq,
			vrx318_irq_handler, 0, dev_name,
			(void *)&priv->frwk->ep_dev[ep_id]);

	if (ret) {
		vrx318_dbg(DBG_ERR, "PTM Register Interrupt Fail!\n");
		BUG();
	}

	vrx318_cb_setup(ptm_dp_rx, ptm_dp_stop_tx, ptm_dp_restart_tx, ptm_get_subif_id
#ifdef CONFIG_LTQ_VRX318_CPUFREQ
		, ptm_coc_confirm_stat
#else
		, NULL
#endif
	);
	ppa_callback_set(LTQ_MEI_SHOWTIME_ENTER, ptm_showtime_enter);
	ppa_callback_set(LTQ_MEI_SHOWTIME_EXIT, ptm_showtime_exit);
}

static inline void get_fw_version(struct fw_ver_id *ver, u32 base)
{
	u32 version;

	version = vrx318_r32_sb(__FW_VER_ID, base);
	*ver = *(struct fw_ver_id *)&version;
}

/**
*  US des sync: CDMA read and dmal write
*  US data copy: (DDR to PDBRAM) EDMA read
*  peer to peer write: dmal write
*  DS data copy: (PDBRAM to DDR) CDMA write
*  DS des sync: dmal write and CDMA read
*  EDMA no outbound base address, pcie will handle it.
*  DMAL and CDMA NEED outbound base address.
*/
static int vrx318_ptm_tc_hw_fw_init(struct vrx318_ptm_tc *priv,
				int ep_id, enum dsl_tc_mode tc_mode)
{
	u32 base;
	int ret;
	int i;

	base = (u32)priv->frwk->ep_dev[ep_id].membase;

	vrx318_dbg(DBG_TC_SWITCH, "vrx318 id  = %d\n", ep_id);
	vrx318_dbg(DBG_TC_SWITCH, "irq        = %u\n",
		priv->frwk->ep_dev[ep_id].irq);
	vrx318_dbg(DBG_TC_SWITCH, "membase    = 0x%08x\n",
		(unsigned int)priv->frwk->ep_dev[ep_id].membase);
	vrx318_dbg(DBG_TC_SWITCH, "phy_membase = 0x%08x\n",
		priv->frwk->ep_dev[ep_id].phy_membase);
	vrx318_dbg(DBG_TC_SWITCH, "peer_num    = %u\n",
		priv->frwk->ep_dev[ep_id].peer_num);
	vrx318_dbg(DBG_TC_SWITCH, "tc_mode         = %s\n",
		tc_mode == TC_PTM_SL_MODE ? "PTM Single Line" : "PTM Bonding");

	/* Reset PPE HW */
	reset_ppe(base);
	/* Activate VRX318 */
	enable_ppe_ema(base);
	/* Set VRX318 PPE clock 432MHz */
	set_ppe_clk(432, base);
	/* Clear and disable mailbox   */
	mbox_clr_isr(MBOX_IGU0, 0xFFFFFFFF, base);
	mbox_clr_isr(MBOX_IGU1, 0xFFFFFFFF, base);
	mbox_set_ier(MBOX_IGU0, 0, base);
	mbox_set_ier(MBOX_IGU1, 0, base);

	/* Freeze PP32 */
	pp32_stop(base);
	ret = ppe_ptm_fw_hw_init(priv, ep_id, tc_mode);
	if (ret)
		return ret;

	pp32_load(base);

	/* init datapath */
	vrx318_ptm_datapath_init(priv, tc_mode, ep_id);

	/* start hardware */
	setup_dfe_loopback(priv->frwk, base);
	for (i = 0; i < PP32_NUM; i++) {
		pp32_start(i, base);
		get_fw_version(&priv->fw_ver[ep_id][i], base);
	}
	dfe_loopback_linkup(priv->frwk, base);

	/* Indicate DSL FW all configuration is ready */
	/* PPE FW is ready to receive/send packets */
	vrx318_tc_indicate(tc_mode, base, 1);

	/* enable mailbox */
	if (tc_mode == TC_PTM_BND_MODE && !priv->frwk->param.p2p_en)
		mbox_set_ier(MBOX_IGU1, P2P_STAT_INTR | FW_UNLOAD_INTR, base);
	else
		mbox_set_ier(MBOX_IGU1, FW_UNLOAD_INTR, base);

	ptm_print_fw_ver(priv, ep_id);

	vrx318_dbg(DBG_TC_SWITCH, "VRX318 PTM init HW successfully\n");
	return 0;
}

void vrx318_ptm_pmac_cfg_init(int pid, enum dsl_tc_mode tc_mode)
{
	struct dp_pmac_cfg pmac_cfg;

	memset(&pmac_cfg, 0, sizeof(pmac_cfg));

	pmac_cfg.eg_pmac_flags = EG_PMAC_F_PMAC | EG_PMAC_F_FCS | EG_PMAC_F_TCENA;

	pmac_cfg.eg_pmac.pmac = 0;
	pmac_cfg.eg_pmac.tc_enable = 1;
	pmac_cfg.eg_pmac.fcs = (tc_mode == TC_PTM_BND_MODE) ? 1 : 0;

	pmac_cfg.ig_pmac_flags = IG_PMAC_F_PRESENT | IG_PMAC_F_SPID
				| IG_PMAC_F_PMACHDR3 | IG_PMAC_F_PMACHDR5;

	pmac_cfg.ig_pmac.pmac = 0;
	pmac_cfg.ig_pmac.def_pmac_src_port = 1;
	pmac_cfg.ig_pmac.def_pmac_hdr[2] = (pid & 0xF) << 4;
	pmac_cfg.ig_pmac.def_pmac_hdr[4] = 0x80; /* Packet has FCS */

	dp_pmac_set(pid, &pmac_cfg);

}

int vrx318_ptm_tc_init(struct vrx318_priv *frwk_priv,
			int ep_id, enum dsl_tc_mode tc_mode)
{
	struct vrx318_ptm_tc *priv;
	int err;
	struct net_device *dev;
	int i;

	if (frwk_priv->ep_num <= 1 && tc_mode == TC_PTM_BND_MODE)
		return -EINVAL;

	if (!g_ptm_tc) {
		err = vrx318_ptm_dev_init(frwk_priv, tc_mode);
		if (err)
			return err;
	}
	priv = g_ptm_tc;
	priv->ep_id = ep_id;
	priv->tc_mode = tc_mode;
	dev = priv->dev;

	vrx318_ptm_cfg_init(frwk_priv, tc_mode);
	if (tc_mode == TC_PTM_SL_MODE) {
		err = vrx318_ptm_tc_hw_fw_init(priv, ep_id, tc_mode);
		if (err)
			return err;
	} else if (tc_mode == TC_PTM_BND_MODE) {
		for (i = 0; i < MAX_VRX318_NUM; i++) {
			err = vrx318_ptm_tc_hw_fw_init(priv, i, tc_mode);
			if (err)
				return err;
		}
	}
	frwk_priv->tc_priv = priv;

	/* pmac configuration */
	vrx318_ptm_pmac_cfg_init(frwk_priv->port_id, tc_mode);

	/* Proc Init */
	vrx318_ptm_proc_init(priv);

	/* Send userspace TC UP message */

	vrx_tc_msg_send(0, PTM_TC, TC_LOADED,
		(tc_mode == TC_PTM_BND_MODE) ? BONDING_MODE : NON_BONDING_MODE,
		ep_id);

	vrx318_dbg(DBG_TC_SWITCH, "VRX318 PTM TC is successfully loaded\n");

	return 0;
}

static void vrx318_ptm_free_mem(struct vrx318_ptm_tc *priv,
				int ep_id, u32 base, int is_bonding)
{
	int i;
	int phy_addr;
	desq_cfg_ctxt_t desq_cfg;
	ds_bond_gif_ll_des_t ll_desc;

	/* Free QoS/OUTQ queue buffers */
	if (!is_bonding || ep_id == US_BONDING_MASTER) {
		for (i = 0; i < TOTAL_QOS_DES_NUM; i++) {
			phy_addr = vrx318_r32_sb(__US_QOSQ_DES_LIST_BASE + (i * 2) + 1,	base) - VRX318_OB_BADDR;
			free_data_buffer(phy_addr);
		}
		for (i = 0; i < OUTQ_NUM; i++) {
			phy_addr = vrx318_r32_sb(__US_OUTQ_DES_LIST_BASE + (i * 2) + 1, base) - VRX318_OB_BADDR;
			free_data_buffer(phy_addr);
		}
	}

	/* Free DS pkt des list, not required in GRX350 */

	/* Free DS FRAGQ dest buffers */
	if (is_bonding) {
		if (ep_id == US_BONDING_MASTER)
			dword_mem_read(&desq_cfg,
					SB_BUFFER(__DS_FRAGQ_CFG_CTXT_BASE),
					sizeof(desq_cfg), base);
		else
			dword_mem_read(&desq_cfg,
				SB_BUFFER(__DS_FRAGQ_CFG_CTXT_BASE) + sizeof(desq_cfg) / sizeof(unsigned int),
				sizeof(desq_cfg), base);

		for (i = 0; i < desq_cfg.des_num; i++) {
			phy_addr = vrx318_r32_sb(desq_cfg.des_base_addr + (i * 2) + 1, base) - VRX318_OB_BADDR;
			free_data_buffer(phy_addr);
		}
	}

	/* Free Link list buffers */
	if (is_bonding && ep_id == DS_BONDING_MASTER) {
		for (i = 0; i < DS_BOND_GIF_LL_DES_LEN; i++) {
			dword_mem_read(&ll_desc, DS_BOND_GIF_LL_DES_BA(i),
				sizeof(ll_desc), base);
			phy_addr = ll_desc.data_ptr - VRX318_OB_BADDR;
			free_data_buffer(phy_addr);
		}
	}

	return;
}

static void ptm_unload_tc(int ep_id, int is_bonding)
{
	struct vrx318_ptm_tc *priv = g_ptm_tc;
	struct vrx318_priv *frwk = priv->frwk;
	u32 base = (u32)frwk->ep_dev[ep_id].membase;
	int err;

	/* Indicate DSL FW TC Clear */
	vrx318_tc_indicate(0, base, 0);

	/* Reset xDSL MEI */
	/* vrx318_reset_xdsl(base); */

	/* Stop ptm device and Unregister subif id */
	/* dev_close(priv->dev); */

	/* Indicate PPE FW to stop */
	priv->comp.done = 0;
	err = wait_for_completion_timeout(&priv->comp,
			msecs_to_jiffies(FW_STOP_TIMEOUT));

	if (err < 0)
		vrx318_dbg(DBG_TC_SWITCH, "Wait process got interrupted\n");

	if (!err)
		vrx318_dbg(DBG_TC_SWITCH, "Wait process Timeout\n");

	/* Stop PPE FW */
	pp32_stop(base);

	/* Unregister Interrupt handler */
	devm_free_irq(frwk->pdev, frwk->ep_dev[ep_id].irq,
		(void *)&frwk->ep_dev[ep_id]);

	/* Disable and Clear all Interrupts */
	mbox_set_ier(MBOX_IGU0, 0, base);
	mbox_set_ier(MBOX_IGU1, 0, base);
	mbox_clr_isr(MBOX_IGU0, 0xFFFFFFFF, base);
	mbox_clr_isr(MBOX_IGU1, 0xFFFFFFFF, base);

	/* Free CBM resources */
	vrx318_ptm_free_mem(priv, ep_id, base, is_bonding);

	return;
}

int vrx318_unload_ptm_tc(struct vrx318_priv *frwk_priv,
			int ep_id, enum dsl_tc_mode tc_mode)
{
	if (in_interrupt()) {
		vrx318_dbg(DBG_ERR, "VRX318 cannot be unloaded in interrupt context\n");
		return -EINVAL;
	}

	if (frwk_priv->tc_priv != g_ptm_tc) {
		vrx318_dbg(DBG_ERR, "TC pointer not match as PTM TC pointer\n");
		return -ENODEV;
	}

	if (tc_mode == TC_PTM_BND_MODE) {
		if (frwk_priv->ep_num <= 1)
			return -EINVAL;
		ptm_unload_tc(US_BONDING_MASTER, 1);
		ptm_unload_tc(DS_BONDING_MASTER, 1);
	} else {
		ptm_unload_tc(ep_id, 0);
	}

	/* Reset CBM EQM/DQM idx */
	soc_vrx318_cfg_reset();

	/* Free Proc */
	vrx318_ptm_proc_exit(g_ptm_tc);

	/* message for TC DOWN */
	vrx_tc_msg_send(0, PTM_TC, TC_UNLOADED,
		(tc_mode == TC_PTM_BND_MODE) ? BONDING_MODE : NON_BONDING_MODE,
		ep_id);

	vrx318_dbg(DBG_TC_SWITCH, "Unload PTM TC successfully!!!\n");

	return 0;
}


static void print_ptm_drv_mib(struct seq_file *seq, struct vrx318_ptm_tc *priv)
{
	seq_puts(seq, "Driver MIB:\n");

	seq_printf(seq, "RX PKTs:\t %lu\n", (unsigned long)priv->stats.rx_packets);
	seq_printf(seq, "RX Bytes:\t %lu\n", (unsigned long)priv->stats.rx_bytes);
	seq_printf(seq, "RX Error:\t %lu\n", (unsigned long)priv->stats.rx_errors);
	seq_printf(seq, "RX Drop:\t %lu\n\n", (unsigned long)priv->stats.rx_dropped);
	seq_printf(seq, "TX PKTs:\t %lu\n", (unsigned long)priv->stats.tx_packets);
	seq_printf(seq, "TX Bytes:\t %lu\n", (unsigned long)priv->stats.tx_bytes);
	seq_printf(seq, "TX Err:\t %lu\n", (unsigned long)priv->stats.tx_errors);
	seq_printf(seq, "TX Drop:\t %lu\n", (unsigned long)priv->stats.tx_dropped);
}

int proc_read_ptm_wanmib(struct seq_file *seq, void *v)
{
	struct vrx318_ptm_tc *priv = (struct vrx318_ptm_tc *)seq->private;
	u32 base = (u32)priv->frwk->ep_dev[priv->ep_id].membase;
	unsigned int wrx_total_pdu[4]		= {PPE_DREG_AR_AIIDLE_CNT0, PPE_DREG_AR_HEC_CNT0, PPE_DREG_AR_AIIDLE_CNT1, PPE_DREG_AR_HEC_CNT1};
	unsigned int wrx_crc_err_pdu[4]		= {GIF0_RX_CRC_ERR_CNT, GIF1_RX_CRC_ERR_CNT, GIF2_RX_CRC_ERR_CNT, GIF3_RX_CRC_ERR_CNT};
	unsigned int wrx_cv_cw_cnt[4]		= {GIF0_RX_CV_CNT, GIF1_RX_CV_CNT, GIF2_RX_CV_CNT, GIF3_RX_CV_CNT};
	unsigned int wrx_bc_overdrop_cnt[2]	= {DREG_B0_OVERDROP_CNT, DREG_B1_OVERDROP_CNT};
	int i;
	int idx = priv->ep_id;
	unsigned int cur_cnt, last_cnt;
	struct wan_rx_mib_table rx_mib;

	seq_puts(seq, "FW MIB:\n");
	seq_puts(seq, "RX (Bearer Channels[0-1]):\n");
	seq_puts(seq, "   wrx_bc_overdrop:");
	for (i = 0; i < 2; i++) {
		if (i != 0)
			seq_printf(seq, ", ");
		cur_cnt = vrx318_r32(wrx_bc_overdrop_cnt[i], base);
		last_cnt = priv->ptm_mib[idx].rx_bc_overdrop_cnt[i];
		seq_printf(seq, "%10u", cur_cnt >= last_cnt
			? cur_cnt - last_cnt
			: cur_cnt + ((unsigned int)(-1) - last_cnt));
	}
	seq_puts(seq, "\n");
	seq_puts(seq, "   wrx_bc_user_cw: ");
	for (i = 0; i < 2; i++) {
		if (i != 0)
			seq_puts(seq, ", ");
		seq_printf(seq, "%10u", vrx318_r32_sb(__RECEIVE_NON_IDLE_CELL_CNT_0 + i, base));
	}
	seq_puts(seq, "\n");
	seq_puts(seq, "   wrx_bc_idle_cw: ");
	for (i = 0; i < 2; i++) {
		if (i != 0)
			seq_puts(seq, ", ");
		seq_printf(seq, "%10u", vrx318_r32_sb(__RECEIVE_IDLE_CELL_CNT_0 + i, base));
	}
	seq_puts(seq, "\n");

	seq_puts(seq, "RX (Gamma Interfaces[0-3]):\n");
	seq_puts(seq, "  wrx_total_pdu:   ");
	for (i = 0; i < 4; i++) {
		if (i != 0)
			seq_puts(seq, ", ");
		cur_cnt = vrx318_r32(wrx_total_pdu[i], base);
		last_cnt = priv->ptm_mib[idx].rx_total_pdu[i];
		seq_printf(seq, "%10u", cur_cnt >= last_cnt
			? cur_cnt - last_cnt
			: cur_cnt + ((unsigned int)(-1) - last_cnt));
	}
	seq_puts(seq, "\n");

	seq_puts(seq, "  wrx_dropdes_pdu: ");
	for (i = 0; i < 4; i++) {
		if (i != 0)
			seq_puts(seq, ", ");
		dword_mem_read(&rx_mib,
			SB_BUFFER(__RX_GIF_MIB_BASE + (i * sizeof(rx_mib) / 4)),
			sizeof(rx_mib), base);
		seq_printf(seq, "%10u", rx_mib.wrx_dropdes_pdu);
	}
	seq_puts(seq, "\n");

	seq_puts(seq, "  wrx_crc_err_pdu: ");
	for (i = 0; i < 4; i++) {
		if (i != 0)
			seq_puts(seq, ", ");
		cur_cnt = vrx318_r32(wrx_crc_err_pdu[i], base);
		last_cnt = priv->ptm_mib[idx].rx_crc_err_pdu[i];
		seq_printf(seq, "%10u", cur_cnt >= last_cnt
			? cur_cnt - last_cnt
			: cur_cnt + ((unsigned int)(-1) - last_cnt));
	}
	seq_puts(seq, "\n");

	seq_puts(seq, "  wrx_violated_cw: ");
	for (i = 0; i < 4; i++) {
		if (i != 0)
			seq_puts(seq, ", ");
		cur_cnt = vrx318_r32(wrx_cv_cw_cnt[i], base);
		last_cnt = priv->ptm_mib[idx].rx_cv_cw_cnt[i];
		seq_printf(seq, "%10u", cur_cnt >= last_cnt
			? cur_cnt - last_cnt
			: cur_cnt + ((unsigned int)(-1) - last_cnt));
	}
	seq_puts(seq, "\n");

	seq_puts(seq, "  wrx_total_bytes: ");
	for (i = 0; i < 4; i++) {
		if (i != 0)
			seq_puts(seq, ", ");
		dword_mem_read(&rx_mib,
			SB_BUFFER(__RX_GIF_MIB_BASE + (i * sizeof(rx_mib) / 4)),
			sizeof(rx_mib), base);
		seq_printf(seq, "%10u", rx_mib.wrx_total_bytes);
	}
	seq_puts(seq, "\n");

	seq_puts(seq, "TX (Bearer Channels[0-1]):\n");
	seq_printf(seq, "  total_tx_cw:     %10u, %10u\n",
		vrx318_r32_sb(__TRANSMIT_CELL_CNT_0, base),
		vrx318_r32_sb(__TRANSMIT_CELL_CNT_1, base));

	seq_puts(seq, "TX (Gamma Interfaces[0-3]):\n");
	seq_puts(seq, "  tx_total_pdu:    ");
	for (i = 0; i < 4; i++) {
		if (i != 0)
			seq_puts(seq, ", ");
		cur_cnt = vrx318_r32_sb(__US_TC_LOCAL_Q_CFG_CTXT_BASE +
				i * sizeof(desq_cfg_ctxt_t) / 4 +
				offsetof(desq_cfg_ctxt_t, deq_pkt_cnt) / 4,
				base);
		last_cnt = priv->ptm_mib[idx].tx_total_pdu[i];
		seq_printf(seq, "%10u", cur_cnt >= last_cnt
			? cur_cnt - last_cnt
			: cur_cnt + ((unsigned int)(-1) - last_cnt));
	}
	seq_puts(seq, "\n");

	seq_puts(seq, "  tx_total_bytes:  ");
	for (i = 0; i < 4; i++) {
		if (i != 0)
			seq_puts(seq, ", ");
		cur_cnt = vrx318_r32_sb(__US_TC_LOCAL_Q_CFG_CTXT_BASE +
				i * sizeof(desq_cfg_ctxt_t) / 4 +
				offsetof(desq_cfg_ctxt_t, deq_byte_cnt) / 4,
				base);
		last_cnt = priv->ptm_mib[idx].tx_total_bytes[i];
		seq_printf(seq, "%10u", cur_cnt >= last_cnt
			? cur_cnt - last_cnt
			: cur_cnt + ((unsigned int)(-1) - last_cnt));
	}
	seq_puts(seq, "\n");

	print_ptm_drv_mib(seq, priv);

	return 0;
}

ssize_t proc_write_ptm_wanmib(struct file *file, const char __user *buf, size_t count, loff_t *data)
{
	char str[32];
	int len, rlen;
	u32 base;
	int num;
	char *param_list[20];
	struct vrx318_ptm_tc *priv;
	unsigned int wrx_total_pdu[4]         = {PPE_DREG_AR_AIIDLE_CNT0, PPE_DREG_AR_HEC_CNT0, PPE_DREG_AR_AIIDLE_CNT1, PPE_DREG_AR_HEC_CNT1};
	unsigned int wrx_crc_err_pdu[4]       = {GIF0_RX_CRC_ERR_CNT, GIF1_RX_CRC_ERR_CNT, GIF2_RX_CRC_ERR_CNT, GIF3_RX_CRC_ERR_CNT};
	unsigned int wrx_cv_cw_cnt[4]         = {GIF0_RX_CV_CNT, GIF1_RX_CV_CNT, GIF2_RX_CV_CNT, GIF3_RX_CV_CNT};
	unsigned int wrx_bc_overdrop_cnt[2]   = {DREG_B0_OVERDROP_CNT, DREG_B1_OVERDROP_CNT};
	int i;
	int idx;

	priv = (struct vrx318_ptm_tc *)PDE_DATA(file_inode(file));
	idx = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[idx].membase;

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;

	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if ((dp_strcmpi(param_list[0], "clear") == 0)
		|| (dp_strcmpi(param_list[0], "clean") == 0)
		|| (dp_strcmpi(param_list[0], "c") == 0)) {
		for (i = 0; i < 4; i++) {
			priv->ptm_mib[idx].rx_total_pdu[i]
				= vrx318_r32(wrx_total_pdu[i], base);
			priv->ptm_mib[idx].rx_crc_err_pdu[i]
				= vrx318_r32(wrx_crc_err_pdu[i], base);
			priv->ptm_mib[idx].rx_cv_cw_cnt[i]
				= vrx318_r32(wrx_cv_cw_cnt[i], base);
			priv->ptm_mib[idx].tx_total_pdu[i]
				= vrx318_r32_sb(__US_TC_LOCAL_Q_CFG_CTXT_BASE +
					i * sizeof(desq_cfg_ctxt_t) / 4 +
					offsetof(desq_cfg_ctxt_t, deq_pkt_cnt) / 4,
					base);
			priv->ptm_mib[idx].tx_total_bytes[i]
				= vrx318_r32_sb(__US_TC_LOCAL_Q_CFG_CTXT_BASE +
					i * sizeof(desq_cfg_ctxt_t) / 4 +
					offsetof(desq_cfg_ctxt_t, deq_byte_cnt) / 4,
					base);

			if (i < 2) {
				priv->ptm_mib[idx].rx_bc_overdrop_cnt[i]
					= vrx318_r32(wrx_bc_overdrop_cnt[i], base);
				vrx318_w32_sb(0, __RECEIVE_NON_IDLE_CELL_CNT_0 + i, base);
				vrx318_w32_sb(0, __RECEIVE_IDLE_CELL_CNT_0 + i, base);
				vrx318_w32_sb(0, __TRANSMIT_CELL_CNT_0 + i, base);
			}
			dword_ep_clear(SB_BUFFER(__RX_GIF_MIB_BASE +
				i * sizeof(struct wan_rx_mib_table) / 4),
				sizeof(struct wan_rx_mib_table), base);
		}
		for (i = 0; i < QOSQ_NUM; i++) {
			dword_ep_clear(SB_BUFFER(__QOSQ_MIB_BASE +
				i * sizeof(qosq_mib_t) / 4),
				sizeof(qosq_mib_t), base);
		}

		memset(&priv->stats, 0, sizeof(priv->stats));
	} else
		goto proc_ptm_wanmib_help;

	return count;

proc_ptm_wanmib_help:
	pr_info("echo clear/clean/c > wanmib\n");
	return count;
}
