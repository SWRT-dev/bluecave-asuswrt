/*******************************************************************************

  Intel SmartPHY DSL PCIe TC driver
  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

*******************************************************************************/
#define DEBUG
#define pr_fmt(fmt) KBUILD_MODNAME ":%s: " fmt, __func__

#include <linux/module.h>
#include <linux/init.h>
#include <linux/atomic.h>
#include <linux/uaccess.h>
#include <linux/bitops.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/wait.h>
#include <linux/seq_file.h>
#include <linux/printk.h>
#include <linux/etherdevice.h>
#include <net/dsl_tc.h>
#include <net/datapath_proc_api.h>

#include "inc/tc_main.h"
#include "inc/reg_addr.h"
#include "inc/tc_common.h"
#include "inc/tc_api.h"
#include "inc/ptm_tc.h"
#include "inc/tc_proc.h"

#include "inc/fw/unified_qos_ds_base_vrx518_be.h"
#include "inc/fw/vrx518_addr_def.h"
#include "inc/fw/vrx518_ds_be.h"
#include "inc/fw/vrx518_ppe_fw.h"
#include "inc/fw/vr9_bonding_fw_data_structure_base_vrx518_be.h"


static struct ptm_priv *g_ptm_priv;
static struct ptm_ep_priv g_ep_priv[BOND_MAX];

static const u32 tx_kvec[] = {
	0x90111293, 0x14959617, 0x18999A1B, 0x9C1D1E9F,
	0xA02122A3, 0x24A5A627, 0x28A9AA2B, 0xAC2D2EAF,
	0x30B1B233, 0xB43536B7, 0xB8393ABB, 0x3CBDBE3F,
	0xC04142C3, 0x44C5C647, 0x48C9CA4B, 0xCC4D4ECF
};
static const u32 def_outq_map[OUTQ_PNUM] = {0x1, 0xFE};
static const char ptm_drv_name[] = "PTM SL";
static const char ptm_bond_name[][IFNAMSIZ] = {"PTM US BOND", "PTM DS BOND"};

/*Function Declaration */
static int ptm_erb_addr_get(const unsigned char idx,
	unsigned int *data_addr, unsigned int *desc_addr);


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

static inline int is_ptm_sl(struct ptm_ep_priv *priv)
{
	if (priv->tc_priv->tc_mode == TC_PTM_SL_MODE)
		return 1;

	return 0;
}

static inline u32 ptm_id(struct ptm_ep_priv *priv)
{
	if (priv->tc_priv->tc_mode == TC_PTM_SL_MODE)
		return 0;
	else
		return priv->ep_id;
}

static inline int is_us_bond(struct ptm_ep_priv *priv)
{
	if (priv->ep_id == US_BOND)
		return 1;
	else
		return 0;
}

static inline struct ptm_ep_priv *tc_ep_priv(u32 ep_id)
{
	WARN_ON(ep_id >= BOND_MAX);

	return &g_ep_priv[ep_id];
}

static int ptm_open(struct net_device *dev)
{
	struct ptm_priv *ptm_tc = netdev_priv(dev);

	tc_info(ptm_tc->tc_priv, MSG_EVENT, "ptm open\n");

	netif_tx_start_all_queues(dev);

	return 0;
}

static int ptm_stop(struct net_device *dev)
{
	struct ptm_priv *ptm_tc = netdev_priv(dev);

	tc_info(ptm_tc->tc_priv, MSG_EVENT, "ptm stop\n");

	netif_tx_stop_all_queues(dev);
	return 0;
}

static int ptm_get_qid(struct net_device *dev, struct sk_buff *skb,
		void *vcc, uint32_t flags)
{
	struct ptm_priv *ptm_tc = netdev_priv(dev);
	int qid, prio;

	if (skb)
		prio = skb->priority < PTM_PRIO_Q_NUM ?
			skb->priority : PTM_PRIO_Q_NUM - 1;
	else
			prio = 0;
	qid = ptm_tc->prio_q_map[prio];
	qid = ptm_tc->subif_id | PTM_DESC_QID(qid);

	return qid;
}

static struct rtnl_link_stats64 *ptm_get_stats(struct net_device *dev,
					struct rtnl_link_stats64 *storage)
{
	struct ptm_priv *ptm_tc = netdev_priv(dev);

	if (ptm_tc->tc_priv->tc_ops.get_mib(dev, storage) < 0)
		memcpy(storage, &ptm_tc->stats64, sizeof(ptm_tc->stats64));
	else
		storage->tx_errors += ptm_tc->stats64.tx_errors;

	return storage;
}

static int ptm_set_mac_address(struct net_device *dev, void *p)
{
	struct sockaddr *addr = (struct sockaddr *)p;
	struct ptm_priv *ptm_tc = netdev_priv(dev);

	if (netif_running(dev))
		return -EBUSY;

	tc_info(ptm_tc->tc_priv, MSG_EVENT, "ptm mac address update!\n");
	ether_addr_copy(dev->dev_addr, addr->sa_data);

	return 0;
}

static void ptm_tx_timeout(struct net_device *dev)
{
	struct ptm_priv *ptm_tc = netdev_priv(dev);

	tc_info(ptm_tc->tc_priv, MSG_TX, "PTM tx timeout\n");
	dev->trans_start = jiffies;
	ptm_tc->stats64.tx_errors++;

	netif_tx_wake_all_queues(dev);
}

static int ptm_tc_bond_get_stats(struct ptm_ep_priv *priv,
	struct intel_tc_ptm_bonding_stats *ptm_bonding_stats)
{
	ds_bond_gif_mib_t ds_gif_mib;
	struct intel_tc_ptm_bonding_ds_mib *ds_mib;
	struct intel_tc_ptm_bonding_ds_bg_mib *bg_mib;
	ds_bg_mib_t ds_bg_mib;
	int idx = 0;
	/* Get US information */
	for (idx = 0; idx < BOND_GIF_NUM; idx++) {
		ptm_bonding_stats->us_gif_mib[idx] =
			tc_r32(US_BOND_GIF_MIB(idx));
	}

	/* Get DS information */
	for (idx = 0; idx < BOND_GIF_NUM; idx++) {
		ds_mib = &(ptm_bonding_stats->ds_gif_mib[idx]);
		tc_mem_read(priv, &ds_gif_mib,
			DS_BOND_GIF_MIB(idx),
			sizeof(ds_gif_mib)
		);
		ds_mib->rx_frag_byte_cnt =
			ds_gif_mib.total_rx_frag_cnt;
		ds_mib->rx_byte_cnt =
			ds_gif_mib.total_rx_byte_cnt;
		ds_mib->rx_of_frag_byte_cnt =
			ds_gif_mib.overflow_frag_cnt;
		ds_mib->rx_of_byte_cnt =
			ds_gif_mib.overflow_byte_cnt;
		ds_mib->rx_oor_frag_byte_cnt =
			ds_gif_mib.out_of_range_frag_cnt;
		ds_mib->rx_miss_frag_byte_cnt =
			ds_gif_mib.missing_frag_cnt;
		ds_mib->rx_to_frag_byte_cnt =
			ds_gif_mib.timeout_frag_cnt;
	}

	for (idx = 0; idx < DS_BG_NUM; idx++) {
		bg_mib = &(ptm_bonding_stats->ds_bg_mib[idx]);
		tc_mem_read(priv, &ds_bg_mib, DS_BG_MIB(idx), sizeof(ds_bg_mib));
		bg_mib->conform_pkt_cnt = ds_bg_mib.conform_pkt_cnt;
		bg_mib->conform_frag_cnt = ds_bg_mib.conform_frag_cnt;
		bg_mib->conform_byte_cnt = ds_bg_mib.conform_byte_cnt;
		bg_mib->no_sop_pkt_cnt = ds_bg_mib.no_sop_pkt_cnt;
		bg_mib->no_sop_frag_cnt = ds_bg_mib.no_sop_frag_cnt;
		bg_mib->no_sop_byte_cnt = ds_bg_mib.no_sop_byte_cnt;
		bg_mib->no_eop_pkt_cnt = ds_bg_mib.no_eop_pkt_cnt;
		bg_mib->no_eop_frag_cnt = ds_bg_mib.no_eop_frag_cnt;
		bg_mib->no_eop_byte_cnt = ds_bg_mib.no_eop_byte_cnt;
		bg_mib->oversize_pkt_cnt = ds_bg_mib.oversize_pkt_cnt;
		bg_mib->oversize_frag_cnt = ds_bg_mib.oversize_frag_cnt;
		bg_mib->oversize_pkt_cnt = ds_bg_mib.oversize_pkt_cnt;
		bg_mib->oversize_byte_cnt = ds_bg_mib.oversize_byte_cnt;
		bg_mib->noncosec_pkt_cnt = ds_bg_mib.noncosec_pkt_cnt;
		bg_mib->noncosec_frag_cnt = ds_bg_mib.noncosec_frag_cnt;
		bg_mib->noncosec_byte_cnt = ds_bg_mib.noncosec_byte_cnt;
	}
	return 0;
}

static int ptm_tc_get_stats(struct ptm_priv *ptm_tc,
	struct intel_tc_stats *stats,
	int bonding
)
{
	struct rtnl_link_stats64 *stat;
	struct wan_rx_mib_table rx_mib;
	unsigned int cur_cnt, last_cnt;
	struct tc_priv *priv_tc;
	struct ptm_ep_priv *priv;
	struct intel_tc_ptm_sl_stats *ptm_sl_stats;
	if (!ptm_tc || !stats)
		return -EFAULT;
	priv_tc = ptm_tc->tc_priv;
	if (!priv_tc)
		return -EFAULT;
	priv = tc_ep_priv(ptm_tc->ep_id);
	if (!priv)
		return -EFAULT;
	if (bonding)
		stats->tc_info = TC_PTM_BND_MODE;
	else
		stats->tc_info = TC_PTM_SL_MODE;
	/* get generic MIB */
	stat = &ptm_tc->stats64;
	ptm_sl_stats = &(stats->stats.ptm_tc_stats.ptm_sl_stats);
	ptm_sl_stats->rx_pkts = stat->rx_packets;
	ptm_sl_stats->rx_bytes = stat->rx_bytes;
	ptm_sl_stats->rx_errs = stat->rx_errors;
	ptm_sl_stats->rx_drops = stat->rx_dropped;
	ptm_sl_stats->tx_pkts = stat->tx_packets;
	ptm_sl_stats->tx_bytes = stat->tx_bytes;
	ptm_sl_stats->tx_errs = stat->tx_errors;
	ptm_sl_stats->tx_drops = stat->tx_dropped;
	/* get bearer channel information, only need the first bearer */
	cur_cnt = tc_r32(B0_OVERDROP_CNT);
	last_cnt = priv->ptm_mib.rx_bc_overdrop_cnt[0];
	ptm_sl_stats->wrx_bc_overdrop = cur_cnt >= last_cnt
		? cur_cnt - last_cnt
		: cur_cnt + ((unsigned int)(-1) - last_cnt);

	ptm_sl_stats->wrx_bc_user_cw = sb_r32(__RECEIVE_NON_IDLE_CELL_CNT_0);
	ptm_sl_stats->wrx_bc_idle_cw = sb_r32(__RECEIVE_IDLE_CELL_CNT_0);
	ptm_sl_stats->wtx_bc_cw = sb_r32(__TRANSMIT_CELL_CNT_0);

	/* gamma interface information, only the first interface */;
	cur_cnt = tc_r32(DREG_AR_AIIDLE_CNT0);
	last_cnt = priv->ptm_mib.rx_total_pdu[0];
	ptm_sl_stats->wrx_gif_total_pdu = cur_cnt >= last_cnt
		? cur_cnt - last_cnt
		: cur_cnt + ((unsigned int)(-1) - last_cnt);

	tc_mem_read(priv, &rx_mib,
		fpi_addr(__RX_GIF_MIB_BASE),
		sizeof(rx_mib)
	);
	ptm_sl_stats->wrx_gif_drop_pdu = rx_mib.wrx_dropdes_pdu;

	cur_cnt = tc_r32(GIF0_RX_CRC_ERR_CNT);
	last_cnt = priv->ptm_mib.rx_crc_err_pdu[0];
	ptm_sl_stats->wrx_gif_crc_err_pdu = cur_cnt >= last_cnt
		? cur_cnt - last_cnt
		: cur_cnt + ((unsigned int)(-1) - last_cnt);

	cur_cnt = tc_r32(GIF0_RX_CV_CNT);
	last_cnt = priv->ptm_mib.rx_cv_cw_cnt[0];
	ptm_sl_stats->wrx_gif_violated_cw = cur_cnt >= last_cnt
		? cur_cnt - last_cnt
		: cur_cnt + ((unsigned int)(-1) - last_cnt);

	ptm_sl_stats->wrx_gif_total_bytes = rx_mib.wrx_total_bytes;
	cur_cnt = sb_r32(__US_TC_LOCAL_Q_CFG_CTXT_BASE +
		offsetof(desq_cfg_ctxt_t, deq_pkt_cnt) / 4);
	last_cnt = priv->ptm_mib.tx_total_pdu[0];
	ptm_sl_stats->wtx_gif_total_pdu = cur_cnt >= last_cnt
		? cur_cnt - last_cnt
		: cur_cnt + ((unsigned int)(-1) - last_cnt);

	cur_cnt = sb_r32(__US_TC_LOCAL_Q_CFG_CTXT_BASE +
			offsetof(desq_cfg_ctxt_t, deq_byte_cnt) / 4);
	last_cnt = priv->ptm_mib.tx_total_bytes[0];
	ptm_sl_stats->wtx_gif_total_bytes = cur_cnt >= last_cnt
		? cur_cnt - last_cnt
		: cur_cnt + ((unsigned int)(-1) - last_cnt);

	/* For bonding information */
	if (bonding) {
		int i;
		struct intel_tc_ptm_bonding_stats ptm_ds;
		struct intel_tc_ptm_bonding_stats ptm_us;
		struct intel_tc_ptm_bonding_stats *ptm_bonding_stats;
		priv = tc_ep_priv(0);
		ptm_tc_bond_get_stats(priv, &ptm_ds);
		priv = tc_ep_priv(1);
		ptm_tc_bond_get_stats(priv, &ptm_us);
		ptm_bonding_stats =
			&(stats->stats.ptm_tc_stats.pmt_bonding_stats);
		for (i = 0; i < 8; i++)
			ptm_bonding_stats->us_gif_mib[i] =
				ptm_ds.us_gif_mib[i] + ptm_us.us_gif_mib[i];
		for (i = 0; i < 8; i++) {
			ptm_bonding_stats->ds_gif_mib[i].rx_frag_byte_cnt =
				ptm_ds.ds_gif_mib[i].rx_frag_byte_cnt
				+ ptm_us.ds_gif_mib[i].rx_frag_byte_cnt;
			ptm_bonding_stats->ds_gif_mib[i].rx_byte_cnt =
				ptm_ds.ds_gif_mib[i].rx_byte_cnt
				+ ptm_us.ds_gif_mib[i].rx_byte_cnt;
			ptm_bonding_stats->ds_gif_mib[i].rx_of_frag_byte_cnt =
				ptm_ds.ds_gif_mib[i].rx_of_frag_byte_cnt
				+ ptm_us.ds_gif_mib[i].rx_of_frag_byte_cnt;
			ptm_bonding_stats->ds_gif_mib[i].rx_of_byte_cnt =
				ptm_ds.ds_gif_mib[i].rx_of_byte_cnt
				+ ptm_us.ds_gif_mib[i].rx_of_byte_cnt;
			ptm_bonding_stats->ds_gif_mib[i].rx_oor_frag_byte_cnt =
				ptm_ds.ds_gif_mib[i].rx_oor_frag_byte_cnt
				+ ptm_us.ds_gif_mib[i].rx_oor_frag_byte_cnt;
			ptm_bonding_stats->ds_gif_mib[i].rx_miss_frag_byte_cnt =
				ptm_ds.ds_gif_mib[i].rx_miss_frag_byte_cnt
				+ ptm_us.ds_gif_mib[i].rx_miss_frag_byte_cnt;
			ptm_bonding_stats->ds_gif_mib[i].rx_to_frag_byte_cnt =
				ptm_ds.ds_gif_mib[i].rx_to_frag_byte_cnt
				+ ptm_us.ds_gif_mib[i].rx_to_frag_byte_cnt;
		}
		for (i = 0; i < 4; i++) {
			ptm_bonding_stats->ds_bg_mib[i].conform_pkt_cnt =
				ptm_ds.ds_bg_mib[i].conform_pkt_cnt
				+ ptm_us.ds_bg_mib[i].conform_pkt_cnt;
			ptm_bonding_stats->ds_bg_mib[i].conform_frag_cnt =
				ptm_ds.ds_bg_mib[i].conform_frag_cnt
				+ ptm_us.ds_bg_mib[i].conform_frag_cnt;
			ptm_bonding_stats->ds_bg_mib[i].conform_byte_cnt =
				ptm_ds.ds_bg_mib[i].conform_byte_cnt
				+ ptm_us.ds_bg_mib[i].conform_byte_cnt;
			ptm_bonding_stats->ds_bg_mib[i].no_sop_pkt_cnt =
				ptm_ds.ds_bg_mib[i].no_sop_pkt_cnt
				+ ptm_us.ds_bg_mib[i].no_sop_pkt_cnt;
			ptm_bonding_stats->ds_bg_mib[i].no_sop_frag_cnt =
				ptm_ds.ds_bg_mib[i].no_sop_frag_cnt
				+ ptm_us.ds_bg_mib[i].no_sop_frag_cnt;
			ptm_bonding_stats->ds_bg_mib[i].no_sop_byte_cnt =
				ptm_ds.ds_bg_mib[i].no_sop_byte_cnt
				+ ptm_us.ds_bg_mib[i].no_sop_byte_cnt;
			ptm_bonding_stats->ds_bg_mib[i].no_eop_pkt_cnt =
				ptm_ds.ds_bg_mib[i].no_eop_pkt_cnt
				+ ptm_us.ds_bg_mib[i].no_eop_pkt_cnt;
			ptm_bonding_stats->ds_bg_mib[i].no_eop_frag_cnt =
				ptm_ds.ds_bg_mib[i].no_eop_frag_cnt
				+ ptm_us.ds_bg_mib[i].no_eop_frag_cnt;
			ptm_bonding_stats->ds_bg_mib[i].no_eop_byte_cnt =
				ptm_ds.ds_bg_mib[i].no_eop_byte_cnt
				+ ptm_us.ds_bg_mib[i].no_eop_byte_cnt;
			ptm_bonding_stats->ds_bg_mib[i].oversize_pkt_cnt =
				ptm_ds.ds_bg_mib[i].oversize_pkt_cnt
				+ ptm_us.ds_bg_mib[i].oversize_pkt_cnt;
			ptm_bonding_stats->ds_bg_mib[i].oversize_frag_cnt =
				ptm_ds.ds_bg_mib[i].oversize_frag_cnt
				+ ptm_us.ds_bg_mib[i].oversize_frag_cnt;
			ptm_bonding_stats->ds_bg_mib[i].oversize_byte_cnt =
				ptm_ds.ds_bg_mib[i].oversize_byte_cnt
				+ ptm_us.ds_bg_mib[i].oversize_byte_cnt;
			ptm_bonding_stats->ds_bg_mib[i].noncosec_pkt_cnt =
				ptm_ds.ds_bg_mib[i].noncosec_pkt_cnt
				+ ptm_us.ds_bg_mib[i].noncosec_pkt_cnt;
			ptm_bonding_stats->ds_bg_mib[i].noncosec_frag_cnt =
				ptm_ds.ds_bg_mib[i].noncosec_frag_cnt
				+ ptm_us.ds_bg_mib[i].noncosec_frag_cnt;
			ptm_bonding_stats->ds_bg_mib[i].noncosec_byte_cnt =
				ptm_ds.ds_bg_mib[i].noncosec_byte_cnt
				+ ptm_us.ds_bg_mib[i].noncosec_byte_cnt;
		}
	}
	return 0;
}
static int ptm_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	int ret;
	int bonding;
	struct intel_tc_stats tc_stats;
	struct ptm_priv *ptm_tc = netdev_priv(dev);
	bonding = ptm_tc->tc_priv->tc_mode == TC_PTM_BND_MODE;
	/* sanity check */
	if (!ifr->ifr_data) {
		return EFAULT;

	}
	ret = EFAULT;
	switch (cmd) {
	case INTEL_PTM_TC_IOCTL_STATS:
		if (!ptm_tc_get_stats(ptm_tc, &tc_stats, bonding))
			ret = copy_to_user(ifr->ifr_data,
					(void *)&tc_stats,
					sizeof(tc_stats));
		break;
	default:
		ret = -ENOIOCTLCMD;
		break;
	}
	return ret;
}

/* Change the MTU value of the netwrok device interfaces */
static int ptm_change_mtu(struct net_device *dev, int new_mtu)
{
	struct ptm_priv *ptm_tc = netdev_priv(dev);

	if (new_mtu < ETH_ZLEN || new_mtu > MAX_MTU)
		return -EINVAL;

	tc_info(ptm_tc->tc_priv, MSG_EVENT, "PTM MTU update: %d\n", new_mtu);
	dev->mtu = new_mtu;

	return 0;
}

static int ptm_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct ptm_priv *ptm_tc = netdev_priv(dev);
	int qid;
	enum tc_pkt_type type;

	if (!showtime_stat(ptm_tc->tc_priv))
		goto PTM_XMIT_DROP;

	if (skb_padto(skb, ETH_ZLEN))
		goto PTM_XMIT_DROP;

	dump_skb_info(ptm_tc->tc_priv, skb, (MSG_TX | MSG_TXDATA));

	qid = ptm_get_qid(dev, skb, NULL, 0);
	type = ptm_tc->tc_priv->tc_mode == TC_PTM_BND_MODE
		? PTM_BOND_PKT : PTM_SL_PKT;

	if (ptm_tc->tc_priv->tc_ops.send(dev, skb, qid, type) < 0)
		ptm_tc->stats64.tx_dropped++;
	else {
		ptm_tc->stats64.tx_packets++;
		ptm_tc->stats64.tx_bytes += skb->len;
	}

	return 0;

PTM_XMIT_DROP:
	dev_kfree_skb_any(skb);
	ptm_tc->stats64.tx_dropped++;
	return 0;
}

static void ptm_rx(struct net_device *dev, struct sk_buff *skb)
{
	struct ptm_priv *ptm_tc;

	ptm_tc = netdev_priv(dev);
	if (skb->len < ETH_ZLEN || skb->len > MAX_MTU) {
		tc_dbg(ptm_tc->tc_priv, MSG_RX,
			"illegal packet len: %d\n", skb->len);
		ptm_tc->stats64.rx_length_errors++;
		goto rx_err_exit;
	}

	dump_skb_info(ptm_tc->tc_priv, skb, (MSG_RX | MSG_RXDATA));
	skb->protocol = eth_type_trans(skb, dev);

	ptm_tc->stats64.rx_packets++;
	ptm_tc->stats64.rx_bytes += skb->len;

	if (netif_rx(skb) == NET_RX_DROP)
		ptm_tc->stats64.rx_dropped++;

	return;

rx_err_exit:
	dev_kfree_skb_any(skb);
	ptm_tc->stats64.rx_errors++;

	return;
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

/* onoff: 0 -- off, 1 -- on */
static void irq_set(struct ptm_priv *ptm_tc, struct ptm_ep_priv *priv,
				u32 irq_id, int onoff)
{
	u32 irq_ier;

	spin_lock_irq(&ptm_tc->ptm_lock);
	irq_ier = tc_r32(MBOX_IGU_IER(MBOX_IGU1));
	if (!onoff)
		irq_ier &= ~irq_id;
	else
		irq_ier |= irq_id;
	mbox_set_ier(priv, MBOX_IGU1, irq_ier);
	mbox_clr_isr(priv, MBOX_IGU1, irq_id);
	spin_unlock_irq(&ptm_tc->ptm_lock);
}

static void ptm_irq_set(u32 irq_id, int onoff)
{
	int i;
	struct ptm_priv *ptm_tc = g_ptm_priv;
	struct ptm_ep_priv *priv;

	if (ptm_tc->tc_priv->tc_mode == TC_PTM_BND_MODE) {
		for (i = 0; i < BOND_MAX; i++) {
			priv = tc_ep_priv(i);
			irq_set(ptm_tc, priv, irq_id, onoff);
		}
	} else {
		priv = tc_ep_priv(ptm_tc->ep_id);
		irq_set(ptm_tc, priv, irq_id, onoff);
	}
}

static void ptm_irq_on(u32 irq_id)
{
	ptm_irq_set(irq_id, 1);
}

static void ptm_irq_off(u32 irq_id)
{
	ptm_irq_set(irq_id, 0);
}

static void ptm_setup(struct net_device *dev)
{
	ether_setup(dev);
	dev->watchdog_timeo = 10 * HZ;
	dev->netdev_ops = &g_ptm_netdev_ops;
}

static int ptm_dev_init(struct tc_priv *tc_priv, u32 id,
		enum dsl_tc_mode tc_mode)
{
	int err, i;
	struct net_device *dev;
	struct ptm_priv *ptm_tc;
	const char macaddr[ETH_ALEN]
		= {0xAC, 0x9A, 0x96, 0x11, 0x22, 0x33};

	dev = alloc_netdev_mq(sizeof(*ptm_tc), "ptm%d", ptm_setup, 4);
	if (!dev) {
		tc_dbg(tc_priv, MSG_INIT, "Cannot alloc net device\n");
		return -ENOMEM;
	}
	ptm_tc = netdev_priv(dev);
	ptm_tc->dev = dev;
	ptm_tc->tc_priv = tc_priv;
	ether_addr_copy(dev->dev_addr, macaddr);
	spin_lock_init(&ptm_tc->ptm_lock);
	/*init_completion(&ptm_tc->comp);*/
	memcpy(ptm_tc->outq_map, def_outq_map, sizeof(def_outq_map));
	SET_NETDEV_DEV(ptm_tc->dev, tc_priv->ep_dev[id].dev);

	err = register_netdev(ptm_tc->dev);
	if (err)
		goto err1;

	netif_carrier_off(ptm_tc->dev);
	g_ptm_priv = ptm_tc;

	for (i = 0; i < tc_priv->ep_num; i++) {
		memset(&g_ep_priv[i], 0, sizeof(g_ep_priv[i]));
		g_ep_priv[i].ep = &tc_priv->ep_dev[i];
		g_ep_priv[i].tc_priv = tc_priv;
		g_ep_priv[i].ep_id = i;
		g_ep_priv[i].ptm_tc = ptm_tc;
	}

	return 0;
err1:
	free_netdev(ptm_tc->dev);
	return err;
}

static void ppe_stop(struct ptm_ep_priv *priv)
{
	int i;

	for (i = PP32_TX; i < PP32_MAX; i++)
		pp32_stop(priv, i);
}

static void ppe_start(struct ptm_ep_priv *priv)
{
	int i;

	for (i = PP32_TX; i < PP32_MAX; i++)
		pp32_start(priv, i);
}

static void ptm_reg_init(struct ptm_ep_priv *priv)
{
	u32 val;
	struct ffsm_cfg ffsmcfg;
	struct sfsm_cfg sfsmcfg;
	int i;

	/* TODO: convert to meaningful configuration */
	tc_w32(0x00000000, PPE_CDM);
	tc_w32(0x00020000, SFSM_PGCNT(0));
	tc_w32(0x00000160, DREG_AT_CFG(0));
	tc_w32(0x00000000, DREG_AT_IDLE(0));
	tc_w32(0x000001F0, DREG_AR_CFG(0));
	tc_w32(0x00000000, DREG_AR_IDLE(0));
	tc_w32(0x00000000, DREG_AR_IDLE(0));
	tc_w32(0x00001FFF, DREG_B_LADR(0));
	val = (((SFSM_PNUM * CW_PAGE_SZ) << 16) |
		(FFSM_PNUM * CW_PAGE_SZ));
	tc_w32(val, PPE_BC0_CW_SIZE);

	memset(&ffsmcfg, 0, sizeof(ffsmcfg));
	ffsmcfg.endian = 1;
	ffsmcfg.fidle = 1;
	ffsmcfg.pnum = FFSM_PNUM;
	tc_mem_write(priv, FFSM_CFG(0),
		&ffsmcfg, sizeof(ffsmcfg));

	tc_w32(SFSM_DBACE, SFSM_DBA(0));
	tc_w32(SFSM_CBACE, SFSM_CBA(0));
	tc_w32(0xF0D10000, FFSM_IDLE_HEAD_BC0);
	tc_w32(0xF0D10000, FFSM_IDLE_HEAD_BC1);

	memset(&sfsmcfg, 0, sizeof(sfsmcfg));
	sfsmcfg.endian = 1;
	sfsmcfg.idlekeep = 1;
	sfsmcfg.sen = 1;
	sfsmcfg.pnum = SFSM_PNUM;
	for (i = 0; i < 2; i++) {
		tc_mem_write(priv, SFSM_CFG(i),
			&sfsmcfg, sizeof(sfsmcfg));
	}

	/* Enable TTHA BC0 & BC1*/
	/**
	* Note: Although BC1 is not used in VRX518,
	* But if not enabled BC1, it will cause DSL FW
	* leave showtime soon after it reach showtime
	*/
	ttha_enable(priv);

	return;
}

static void pdma_init(struct ptm_ep_priv *priv)
{
	/* TODO: convert to meaningful configuration */
	tc_w32(0x00000001, PDMA_CFG);
	tc_w32(0x00082C00, PDMA_RX_CTX_CFG);
	tc_w32(0x00081B00, PDMA_TX_CTX_CFG);
	tc_w32(0x02040800, PDMA_RX_MAX_LEN_REG);

	tc_w32(0x000F003F, PDMA_RX_DELAY_CFG);

	tc_w32(0x00000011, SAR_MODE_CFG);
	tc_w32(0x00081200, SAR_RX_CTX_CFG);
	tc_w32(0x00082E00, SAR_TX_CTX_CFG);
	tc_w32(0x00001021, SAR_POLY_CFG_SET0);
	tc_w32(0x1EDC6F41, SAR_POLY_CFG_SET1);
	tc_w32(0x04C11DB7, SAR_POLY_CFG_SET2);
	tc_w32(0x00000F3E, SAR_CRC_SIZE_CFG);

	tc_w32(0x00FE1900, SAR_PDMA_RX_CMDBUF_CFG);
	tc_w32(0x00FE1A00, SAR_PDMA_TX_CMDBUF_CFG);

	tc_w32(0x00203FC0, SAR_PDMA_RX_FW_CMDBUF_CFG);
	tc_w32(0x00203FE0, SAR_PDMA_TX_FW_CMDBUF_CFG);

	tc_w32(0x00000007, PDMA_IER);

	return;
}

static void ptm_hw_init(struct ptm_ep_priv *priv)
{
	ptm_reg_init(priv);
	pdma_init(priv);
}

static void ptm_fw_init(struct ptm_ep_priv *priv, int is_bonding)
{
	std_des_cfg_t std_des_cfg;
	qos_cfg_t qos_cfg;
	qosq_flow_ctrl_cfg_t flow_ctrl;
	struct psave_cfg ps_cfg;
	struct rx_bc_cfg rx_bc_cfg;
	struct tx_bc_cfg tx_bc_cfg;
	struct rx_gamma_itf_cfg rx_gitf_cfg;
	struct tx_gamma_itf_cfg tx_gitf_cfg;

	u32 dst_addr;
	int i;

	sb_w32(0x3C30, __VRX218_FLOW_CTRL_CFG);

	memset(&std_des_cfg, 0, sizeof(std_des_cfg));
	/* this field replaces byte_off in rx descriptor of VDSL ingress */
	std_des_cfg.byte_off = is_bonding ? 2 : 0;
	std_des_cfg.data_len = DMA_PACKET_SZ;
	tc_mem_write(priv, fpi_addr(__STD_DES_CFG),
		&std_des_cfg, sizeof(std_des_cfg));

	memset(&qos_cfg, 0, sizeof(qos_cfg));
	/* 16 * (cgu_get_pp32_clock() / 1000000) */
	qos_cfg.time_tick = tc_ppe_clkget(priv->ep) / 62500;
	qos_cfg.qosq_num = QOSQ_NUM;
	qos_cfg.qos_en = 0;
	tc_mem_write(priv, fpi_addr(__QOS_CFG), &qos_cfg, sizeof(qos_cfg));

	for (i = PP32_TX; i < PP32_MAX; i++) {
		memset(&ps_cfg, 0, sizeof(ps_cfg));
		ps_cfg.start_state = 0;
		ps_cfg.sleep_en = priv->tc_priv->param.ps;
		tc_mem_write(priv, fpi_addr(PSAVE_CFG(i)),
			&ps_cfg, sizeof(ps_cfg));
	}

	memset(&flow_ctrl, 0, sizeof(flow_ctrl));
	flow_ctrl.large_frame_size = 128;
	flow_ctrl.large_frame_drop_th = 0xFE;
	tc_mem_write(priv, fpi_addr(__QOSQ_FLOW_CTRL_CFG),
		&flow_ctrl, sizeof(flow_ctrl));

	tc_w32(DMA_PACKET_SZ, fpi_addr(__MAX_PKT_SIZE_CFG));

	memset(&rx_bc_cfg, 0, sizeof(rx_bc_cfg));
	rx_bc_cfg.local_state   = 0;
	rx_bc_cfg.remote_state  = 0;
	rx_bc_cfg.to_false_th   = 7;
	rx_bc_cfg.to_looking_th = 3;
	for (i = 0; i < 2; i++) {
		dst_addr = __RX_BC0_CFG_STATS_CFG + (i * DW_SZ(rx_bc_cfg));
		tc_mem_write(priv, fpi_addr(dst_addr),
			&rx_bc_cfg, sizeof(rx_bc_cfg));
	}

	memset(&tx_bc_cfg, 0, sizeof(tx_bc_cfg));
	tx_bc_cfg.fill_wm = 16;
	tx_bc_cfg.uflw_wm = 2;
	for (i = 0; i < 2; i++) {
		dst_addr = __TX_BC0_CFG_STATS_CFG + (i * DW_SZ(tx_bc_cfg));
		tc_mem_write(priv, fpi_addr(dst_addr),
			&tx_bc_cfg, sizeof(tx_bc_cfg));
	}

	memset(&rx_gitf_cfg, 0, sizeof(rx_gitf_cfg));
	rx_gitf_cfg.receive_state	= 0;
	rx_gitf_cfg.rx_min_len		= 64;
	rx_gitf_cfg.rx_pad_en		= 1;
	/* disable Ethernet FCS verification during bonding */
	rx_gitf_cfg.rx_eth_fcs_ver_dis	= is_bonding ? 1 : 0;
	rx_gitf_cfg.rx_rm_eth_fcs	= is_bonding ? 0 : 1;
	rx_gitf_cfg.rx_tc_crc_ver_dis	= 0;
	rx_gitf_cfg.rx_tc_crc_size	= 1;
	rx_gitf_cfg.rx_eth_fcs_result	= 0xC704DD7B;
	rx_gitf_cfg.rx_tc_crc_result	= 0x1D0F1D0F;
	rx_gitf_cfg.rx_crc_cfg		= 0x2500;
	rx_gitf_cfg.rx_eth_fcs_init_value = 0xFFFFFFFF;
	rx_gitf_cfg.rx_tc_crc_init_value = 0x0000FFFF;
	rx_gitf_cfg.rx_max_len_sel	= 0;
	rx_gitf_cfg.rx_edit_num2	= 0;
	rx_gitf_cfg.rx_edit_pos2	= 0;
	rx_gitf_cfg.rx_edit_type2	= 0;
	rx_gitf_cfg.rx_edit_en2		= 0;
	/* no PMAC header insertion during bonding or grx350 platform*/
	rx_gitf_cfg.rx_edit_num1	= 0;
	rx_gitf_cfg.rx_edit_pos1	= 0;
	/* no PMAC header insertion during bonding or grx350 platform*/
	rx_gitf_cfg.rx_edit_type1	= 0;
	rx_gitf_cfg.rx_edit_en1		= 0;
	/*  E5: byte swap of value 0x07000000 */
	rx_gitf_cfg.rx_inserted_bytes_1l = 0x00000007;
	rx_gitf_cfg.rx_inserted_bytes_1h = 0;
	rx_gitf_cfg.rx_inserted_bytes_2l = 0;
	rx_gitf_cfg.rx_inserted_bytes_2h = 0;
	rx_gitf_cfg.rx_len_adj		= -2;
	for (i = 0; i < 4; i++) {
		dst_addr = __RX_GIF0_CFG_STATS_CFG +
			(i * DW_SZ(rx_gitf_cfg));
		tc_mem_write(priv, fpi_addr(dst_addr),
			&rx_gitf_cfg, sizeof(rx_gitf_cfg));
	}

	memset(&tx_gitf_cfg, 0, sizeof(tx_gitf_cfg));
	tx_gitf_cfg.tx_len_adj		= is_bonding ? 2 : 6;
	tx_gitf_cfg.tx_crc_off_adj	= 6;
	tx_gitf_cfg.tx_min_len		= 0;
	tx_gitf_cfg.tx_eth_fcs_gen_dis	= is_bonding ? 1 : 0;
	tx_gitf_cfg.tx_tc_crc_size	= 1;
	tx_gitf_cfg.tx_crc_cfg		= is_bonding ? 0x2F02 : 0x2F00;
	tx_gitf_cfg.tx_eth_fcs_init_value = 0xFFFFFFFF;
	tx_gitf_cfg.tx_tc_crc_init_value = 0x0000FFFF;
	for (i = 0; i < 4; i++) {
		/**
		* tx_gitf_cfg.queue_mapping = g_queue_gamma_map[i];
		*/
		dst_addr = __TX_GIF0_CFG_STATS_CFG +
			(i * DW_SZ(tx_gitf_cfg));
		tc_mem_write(priv, fpi_addr(dst_addr),
			&tx_gitf_cfg, sizeof(tx_gitf_cfg));
	}

	/* Initialize TX Ctrl K Table */
	for (i = 0; i < ARRAY_SIZE(tx_kvec); i++)
		tc_w32(tx_kvec[i], TX_CTRL_K_TABLE(i));

	return;
}

/**
* Initialize PPE FW General Configuration
*/
static void ptm_gen_cfg_init(struct ptm_ep_priv *priv, int bonding)
{
	int i;
	u32 us_bond, ds_bond;
	task_cfg_t task_cfg[PP32_MAX];
	bond_conf_t bond_conf;
	const u32 sb_addr[] = {
		__PP32_0_TASK_CFG, __PP32_1_TASK_CFG, __PP32_2_TASK_CFG};
	u32 peer_sb_addr;

	memset(&task_cfg, 0, sizeof(task_cfg));
	/* Default Task Configuration */
	for (i = 0; i < PP32_MAX; i++) {
		us_bond = (bonding && is_us_bond(priv) && (i == PP32_BOND));
		ds_bond = (bonding && !is_us_bond(priv) && (i == PP32_BOND));
		task_cfg[i].pp32_core_id	= i;
		task_cfg[i].us_bonding_master	= bonding && is_us_bond(priv);
		task_cfg[i].us_segment_en	= us_bond;
		task_cfg[i].us_buf_release_en	= us_bond;

		task_cfg[i].ds_bonding_master	= bonding && !is_us_bond(priv);
		task_cfg[i].ds_pkt_dispatch_en	= ds_bond;
		task_cfg[i].ds_pkt_reconstruct_en = ds_bond;
		task_cfg[i].ds_pkt_flush_en	= ds_bond;

		task_cfg[i].tc_us_en		= (i == PP32_TX);
		task_cfg[i].tc_ds_en		= (i == PP32_RX);
		task_cfg[i].des_sync_en		= 0;
		task_cfg[i].edma_write_lle_gen_en = 0;
		task_cfg[i].edma_read_lle_gen_en = 0;
		task_cfg[i].edma_post_proc_en	= 0;
		task_cfg[i].qos_wfq_shaping_en	= (i == PP32_BOND);
		task_cfg[i].qos_dispatch_en	= (i == PP32_BOND);
		task_cfg[i].qos_replenish_en	= (i == PP32_BOND);
	}

	for (i = 0; i < PP32_MAX; i++) {
		tc_mem_write(priv, fpi_addr(sb_addr[i]),
				&task_cfg[i], sizeof(task_cfg));
	}

	if (bonding) {
		memset(&bond_conf, 0, sizeof(bond_conf));
		bond_conf.dplus_fp_fcs_en   = 0x1;
		bond_conf.max_frag_size     = DEF_BOND_FRAG_SIZE;
		/* 0x4 if two bearer channels are enabled */
		bond_conf.bg_num            = 0x2;
		bond_conf.bond_mode         = 0x0;
		bond_conf.e1_bond_en        = 0x1;
		bond_conf.d5_acc_dis        = 0x1;
		bond_conf.d5_b1_en          = 0x1;

		tc_mem_write(priv, fpi_addr(__BOND_CONF),
			&bond_conf, sizeof(bond_conf));
		/**
		 * Set Peer SB base address
		 * FW guarantee it will only write to SB Segment 1
		 * Driver provide address to FW
		 * address = PPE BASE (0x300000) + (FPI_SB_SEG1_BASE(0x8000)
		 *		- PPE_SB_SEG1_BASE (0x8000)) * 4
		 * FW will add SB address when it use
		 * FW usage: (address + SB_ADDR * 4)
		 */
		peer_sb_addr = priv->ep->peer_phy_membase[0] + PPE_BASE
			+ ((FPI_SB_SEG_BASE(1) - SB_SEG1_BASE) << 2);
		sb_w32(peer_sb_addr, __BOND_PEER_SB_BASE);
		sb_w32(0x07800000, __DS_PKT_PMAC_HEADER);
		sb_w32(priv->tc_priv->switch_exist, __PCIE_SWITCH_PRESENT);
	}

	/**
	 * Forever loops for each CDMA post processing code to be run
	 * set to 1 to accelerate processing
	 */
	sb_w32(1, __MAINLOOP_CFG_ADDR);
	sb_w32(1, __SOC_FAMILY);
}

/**
 *  Initialize Fastpath descriptor queue config contxt
 *  Initialize dest list of the desq_cfg_ctxt
 */
static void us_fp_desq_cfg_ctxt_init(struct ptm_ep_priv *priv)
{
	desq_cfg_ctxt_t desq_cfg;
	int i;
	u32 desc_addr;
	rx_descriptor_t desc;

	memset(&desq_cfg, 0, sizeof(desq_cfg));
	/* Initialize US Fast-Path Descriptor Queue Config/Context */
	desq_cfg.des_in_own_val	= US_FP_DES_OWN;
	desq_cfg.fast_path	= 1;
	desq_cfg.mbox_int_en	= 0;
	desq_cfg.des_sync_needed = 0;
	desq_cfg.des_num	= __US_FAST_PATH_DES_LIST_NUM;
	desq_cfg.des_base_addr	= __US_FAST_PATH_DES_LIST_BASE;

	tc_mem_write(priv, fpi_addr(__US_FP_INQ_DES_CFG_CTXT),
		&desq_cfg, sizeof(desq_cfg));

	/* Initialize Descriptors in Shadow Fast Path Descriptor List */
	/* Shadow descriptor list of SoC, only own bit is required for init */
	memset(&desc, 0, sizeof(desc));
	desc.own = !desq_cfg.des_in_own_val;
	desc_addr = desq_cfg.des_base_addr;
	for (i = 0; i < desq_cfg.des_num; i++) {
		tc_mem_write(priv, fpi_addr(desc_addr),
				&desc, sizeof(desc));
		desc_addr += DW_SZ(desc);
	}
}

static void us_qos_desq_cfg_ctxt_init(struct ptm_ep_priv *priv)
{
	qosq_cfg_ctxt_t qosq_cfg_ctxt;
	int offset, i;
	rx_descriptor_t desc;
	dma_addr_t phy_addr;

	/* Setup QoSQ_CFG_CTXT */
	memset(&qosq_cfg_ctxt, 0, sizeof(qosq_cfg_ctxt));

	qosq_cfg_ctxt.threshold = 8;
	qosq_cfg_ctxt.des_num	= QOS_DES_NUM / QOSQ_NUM;

	offset = 0;
	for (i = 0; i < QOSQ_NUM; i++) {
		qosq_cfg_ctxt.des_base_addr = __US_QOSQ_DES_LIST_BASE + offset;
		tc_mem_write(priv,
			fpi_addr((__QOSQ_CFG_CTXT_BASE +
					(i * __QOSQ_CFG_CTXT_SIZE))),
			&qosq_cfg_ctxt, sizeof(qosq_cfg_ctxt));
		offset += qosq_cfg_ctxt.des_num * DW_SZ(qosq_cfg_ctxt);
	}

	memset(&desc, 0, sizeof(desc));
	desc.own = QOS_DISPATCH_OWN;
	desc.data_len = DMA_PACKET_SZ;

	/* Initialize QoSQ Descriptors */
	for (i = 0; i < QOS_DES_NUM; i++) {
		if (tc_buf_alloc(priv, DMA_PACKET_SZ,
			&phy_addr, US_DIR, priv->ep->dev) == NULL) {
			set_tc_status(priv->tc_priv, TC_ERR);
			return;
		}
		desc.data_ptr = phy_addr;
		tc_mem_write(priv,
			fpi_addr(__US_QOSQ_DES_LIST_BASE + (i * DW_SZ(desc))),
			&desc, sizeof(desc));
	}
}

static void us_outq_desq_cfg_ctxt_init(struct ptm_ep_priv *priv)
{
	outq_qos_cfg_ctxt_t outq_cfg;
	desq_cfg_ctxt_t desq_cfg;
	u32 addr;
	rx_descriptor_t rx_desc;
	dma_addr_t phy_addr;
	int i;
	u32 offset;

	/* Setup OUTQ_QoS_CFG_CTXT */
	/* NOTE: By default, Shaping & WFQ both are DISABLED!! */
	memset(&outq_cfg, 0, sizeof(outq_cfg));
	/* outq_cfg.overhd_bytes  = bonding ? 20 : 24;*/

	/* outq_cfg 0/desc_cfg 1 -> pre-emption queue */
	/* outq_cfg 1/desc_cfg 0 -> none pre-emption queue */
	for (i = 0; i < OUTQ_PNUM; i++) {
		outq_cfg.qmap			= g_ptm_priv->outq_map[i];
		outq_cfg.l2_shaping_cfg_ptr	= 0;
		outq_cfg.l2_shaping_cfg_idx	= QOSQ_PORT_SSID + i;
		outq_cfg.l3_shaping_cfg_ptr	= 0;
		outq_cfg.l3_shaping_cfg_idx	= QOSQ_L3_SHAPER_ID;
		outq_cfg.desq_cfg_ctxt
			= __US_QOS_OUTQ_DES_CFG_CTXT_BASE +
				i * DW_SZ(desq_cfg_ctxt_t);

		addr = __OUTQ_QOS_CFG_CTXT_BASE + i * DW_SZ(outq_cfg);
		tc_mem_write(priv, fpi_addr(addr), &outq_cfg, sizeof(outq_cfg));
	}

	/* Setup DESQ_CFG_CTXT for Output Queues */
	memset(&desq_cfg, 0, sizeof(desq_cfg));

	desq_cfg.des_in_own_val	= US_OUTQ_DES_OWN;
	desq_cfg.mbox_int_en	= 0;
	desq_cfg.des_sync_needed = 0;
	desq_cfg.des_num	= 32;
	/**
	* Only BC0 is used in VRX518
	*/
	offset = 0;
	addr = __US_QOS_OUTQ_DES_CFG_CTXT_BASE;
	for (i = 0; i < OUTQ_PNUM; i++) {
		desq_cfg.des_base_addr = __US_OUTQ_DES_LIST_BASE + offset;
		tc_mem_write(priv, fpi_addr(addr), &desq_cfg, sizeof(desq_cfg));
		offset += desq_cfg.des_num * 2;
		addr += DW_SZ(desq_cfg_ctxt_t);
	}

	/* Initialize the outq descriptor list */
	memset(&rx_desc, 0, sizeof(rx_desc));
	rx_desc.own = !desq_cfg.des_in_own_val;
	rx_desc.data_len = DMA_PACKET_SZ;

	/* Initialize QoSQ Descriptors */
	for (i = 0; i < OUTQ_DESC_NUM; i++) {
		if (tc_buf_alloc(priv, DMA_PACKET_SZ,
			&phy_addr, US_DIR, priv->ep->dev) == NULL) {
			set_tc_status(priv->tc_priv, TC_ERR);
			return;
		}

		rx_desc.data_ptr = phy_addr;
		addr = fpi_addr(__US_OUTQ_DES_LIST_BASE + (i * DW_SZ(rx_desc)));
		tc_mem_write(priv, addr, &rx_desc, sizeof(rx_desc));
	}

	return;
}

/**
* Initialize QoS related configuration for VRX518
* Initializes the below PPE FW Data Structures
* 1. INQ_QoS_CFG
* 2. QoSQ_CFG_CTXT
* 3. OUTQ_QoS_CFG_CTXT
* 4. SHAPING_WFQ_CFG
* 5. QOSQ_MIB
* 6. QOSQ_FLOW_CTRL_CFG
* 7. STD_DES_CFG
*/
static void us_qos_cfg_init(struct ptm_ep_priv *priv)
{
	inq_qos_cfg_t fp_qos_cfg;

	/* Setup INQ_QoS_CFG for Fast-Path & CPU-Path */
	memset(&fp_qos_cfg, 0, sizeof(fp_qos_cfg));

	fp_qos_cfg.qos_en         = 1;
	fp_qos_cfg.qid_mask       = QOSQ_ID_MASK;
	fp_qos_cfg.qosq_base_qid  = 0;
	fp_qos_cfg.desq_cfg_ctxt  = __US_FP_INQ_DES_CFG_CTXT;
	tc_mem_write(priv, INQ_QOS_CFG_BASE(0),
		&fp_qos_cfg, sizeof(fp_qos_cfg));

	us_fp_desq_cfg_ctxt_init(priv);
	us_qos_desq_cfg_ctxt_init(priv);
	us_outq_desq_cfg_ctxt_init(priv);

	/* Set QoS NO DROP */
	sb_w32(1, __QOSQ_NO_DROP);
	/* Enable Preemption function/Disable QoS by default */
	sb_w32(1, _CHK_PREEMP_MAP);
	/* By default, all qid mappint to non-preemption queue */
	sb_w32(0x0, _QID2PREEMP_MAP);

	return;
}

/**
 * PDBRAM LAYOUT
 * 1. US Local data
 * 2. DS Local data
 * 3. ACA shadow descriptors
 * 4. ERB descriptors
 * Summary for current PDBRAM usage
 * Total PDBRAM for PPE: 0x218000 - 0x237FFF (128KB)
 * US local data base: 0x218000 size: 0x9000 bytes
 * DS local data base: 0x221000 size: 0xF000 bytes
 * ACA TXOUT desc base:0x230000 size: 0x200 bytes
 * ACA RXOUT desc base:0x230200 size: 0x7F8 bytes
 * ERB desc base:      0x2309F8 size: 0x2520 bytes
 * Left space:         0x232F18 - 0x237FFF (around 20KB)
 * Bonding parameter is reserved for future usage.
 * return request layout base physical address
 * return 0 as invalid address
 */
static u32 ptm_pdbram_mem_layout(struct ptm_ep_priv *priv, u32 req, int bonding)
{
	u32 addr, num;

	addr = PDBRAM_PPE_BASE;
	if (req == PDBRAM_US)
		return addr;

	/* Total US local des counter */
	num = __US_TC_LOCAL_Q0_DES_LIST_NUM + __US_TC_LOCAL_Q1_DES_LIST_NUM;
	addr += num * DMA_PACKET_SZ;
	if (req == PDBRAM_DS)
		return addr;

	num = __DS_TC_LOCAL_Q_DES_LIST_NUM;
	addr += num * DMA_PACKET_SZ;
	if (req == PDBRAM_ACA_TXOUT)
		return addr;

	num = __ACA_TX_OUT_PD_LIST_NUM;
	addr += num * DESC_SZ;
	if (req == PDBRAM_ACA_RXOUT)
		return addr;

	num = __ACA_RX_OUT_PD_LIST_NUM;
	addr += num * DESC_SZ;
	if (req == PDBRAM_ERB)
		return addr;

	return 0;
}

/**
 * Initialize ERB (Total 9 ERB buffer)
 */
static void ptm_erb_init(struct ptm_ep_priv *priv, int bonding)
{
	desq_cfg_ctxt_t des_cfg, frag_des_cfg;
	bg_erb_cfg_ctxt_t erb_bg;
	u32 erb_pdbram_base, addr;
	rx_descriptor_t desc;
	int i, reg_val;

	/* Init ERB desq_cfg_ctxt */
	if (is_ptm_sl(priv) || (bonding && is_us_bond(priv))) {
		memset(&des_cfg, 0, sizeof(des_cfg));
		des_cfg.des_in_own_val = ERB_DES_PPE_OWN;
		des_cfg.des_base_addr = __OUTQ_ERB0_BASE;
		des_cfg.des_num = __OUTQ_ERB0_NUM;
		tc_mem_write(priv, fpi_addr(__OUTQ_ERB0_CFG_CTXT_BASE),
			&des_cfg, sizeof(des_cfg));

		/* Init ERB descriptor in PDBRAM */
		erb_pdbram_base
			= ptm_pdbram_mem_layout(priv, PDBRAM_ERB, bonding);
		erb_pdbram_base |= priv->ep->phy_membase;

		for (i = 0; i < __OUTQ_ERB0_NUM; i++) {
			memset(&desc, 0, sizeof(desc));
			desc.own = !ERB_DES_PPE_OWN;
			desc.data_len = ERB_PKT_SIZE;
			desc.data_ptr = erb_pdbram_base + i * ERB_PKT_SIZE;
			tc_mem_write(priv,
				fpi_addr(des_cfg.des_base_addr + i * DESC_DWSZ),
				&desc, sizeof(desc));
		}
	}

	if (bonding) {
		/* Init OUTQ_ERB_PEER_CFG_CTXT */
		memset(&des_cfg, 0, sizeof(des_cfg));
		des_cfg.des_in_own_val = ERB_DES_PPE_OWN;
		des_cfg.des_base_addr = __OUTQ_ERB_PEER_BASE;
		des_cfg.des_num = __OUTQ_ERB_PEER_NUM;
		tc_mem_write(priv, fpi_addr(__OUTQ_ERB_PEER_CFG_CTXT_BASE),
			&des_cfg, sizeof(des_cfg));

		/* Init ERB descriptor in DS_BOND */
		/* US_Bond doesn't need init descriptor list */
		if (!is_us_bond(priv)) {
			erb_pdbram_base
				= ptm_pdbram_mem_layout(priv,
					PDBRAM_ERB, bonding);
			erb_pdbram_base |= priv->ep->phy_membase;

			for (i = 0; i < __OUTQ_ERB_PEER_NUM; i++) {
				memset(&desc, 0, sizeof(desc));
				desc.own = !ERB_DES_PPE_OWN;
				desc.data_len = ERB_PKT_SIZE;
				desc.data_ptr = erb_pdbram_base
							+ i * ERB_PKT_SIZE;
				tc_mem_write(priv,
					fpi_addr(des_cfg.des_base_addr
						+ i * DESC_DWSZ),
					&desc, sizeof(desc));
			}
		}

		/* Initialize FRAGQ_ERB0_CFG_CTXT */
		if (is_us_bond(priv)) {
			memset(&frag_des_cfg, 0, sizeof(frag_des_cfg));
			frag_des_cfg.des_in_own_val = ERB_FRAG_DES_OWN;
			frag_des_cfg.des_base_addr = __FRAGQ_ERB0_BASE;
			frag_des_cfg.bp_des_base_addr = __FRAGQ_ERB0_BP_BASE;
			frag_des_cfg.des_num = __FRAGQ_ERB0_NUM;
			tc_mem_write(priv, fpi_addr(__FRAGQ_ERB0_CFG_CTXT_BASE),
				&frag_des_cfg, sizeof(frag_des_cfg));
		}

		/* Initialize FRAGQ_ERB_PEER_CFG_CTXT */
		memset(&frag_des_cfg, 0, sizeof(frag_des_cfg));
		frag_des_cfg.des_in_own_val = ERB_FRAG_DES_OWN;
		frag_des_cfg.des_base_addr = __FRAGQ_ERB_PEER_BASE;
		frag_des_cfg.bp_des_base_addr
			= __FRAGQ_ERB_PEER_BP_BASE;
		frag_des_cfg.des_num = __FRAGQ_ERB_PEER_NUM;
		tc_mem_write(priv,
			fpi_addr(__FRAGQ_ERB_PEER_CFG_CTXT_BASE),
			&frag_des_cfg, sizeof(frag_des_cfg));

		/* Initialize FRAGQ_ERB descriptor list Only in US_BOND */
		/* PPE FW Sync __FRAGQ_ERB_PEER descriptor list from US_BOND to DS_BOND */
		if (is_us_bond(priv)) {
			addr = __FRAGQ_ERB0_BASE;
			for (i = 0; i < __FRAGQ_ERB0_NUM; i++) {
				memset(&desc, 0, sizeof(desc));
				desc.own = !ERB_DES_PPE_OWN;
				tc_mem_write(priv,
					fpi_addr(addr + i * DESC_DWSZ),
					&desc, sizeof(desc));
			}

			addr = __FRAGQ_ERB_PEER_BASE;
			for (i = 0; i < __FRAGQ_ERB_PEER_NUM; i++) {
				memset(&desc, 0, sizeof(desc));
				desc.own = !ERB_DES_PPE_OWN;
				tc_mem_write(priv,
					fpi_addr(addr + i * DESC_DWSZ),
					&desc, sizeof(desc));
			}

			memset(&erb_bg, 0, sizeof(erb_bg));
			erb_bg.outq_erb_cfg_ctxt = __OUTQ_ERB0_CFG_CTXT_BASE;
			erb_bg.outq_erb_peer_cfg_ctxt
				= __OUTQ_ERB_PEER_CFG_CTXT_BASE;
			/* erb only work for non-preemption queue */
			addr = __BG_ERB_CFG_CTXT_BASE + DW_SZ(erb_bg);
			tc_mem_write(priv, fpi_addr(addr),
				&erb_bg, sizeof(erb_bg));
		}
	}

	/* Init DSL TC register, DSL FW can only access 64KB range */
	if (is_ptm_sl(priv) || (bonding && is_us_bond(priv)))
		addr = __OUTQ_ERB0_BASE;
	else
		addr = __OUTQ_ERB_PEER_BASE;
	reg_val = fpi_addr(addr) & 0xFFFF;
	sb_w32(reg_val, ERB_DESC_BASE);
	sb_w32(des_cfg.des_num, ERB_DESC_NUM);
	sb_w32(ERB_PKT_SIZE, __ERB_STD_DATA_LEN);
}

/**
 * Initialize VRX318 TC Local Descriptor List
 * Performs below actions
 *      (1) Allocate and Initialize TC UpStream Local Descriptors
 *      (2) Allocate and Initialize TC DownStream Local Descriptors
 */
static void ptm_local_desq_cfg_ctxt_init(struct ptm_ep_priv *priv, int bonding)
{
	int i;
	rx_descriptor_t desc;
	desq_cfg_ctxt_t desq_cfg;
	u32 dcnt, addr, pdbram_base;
	unsigned int us_des_alloc[] = {
		__US_TC_LOCAL_Q0_DES_LIST_NUM, __US_TC_LOCAL_Q1_DES_LIST_NUM};

	/* Setup the Local DESQ Configuration/Context for UpStream Queues */
	memset(&desq_cfg, 0, sizeof(desq_cfg));
	desq_cfg.des_in_own_val = US_LOCALQ_DES_OWN;
	for (i = 0, dcnt = 0; i < ARRAY_SIZE(us_des_alloc); i++) {
		desq_cfg.des_num = us_des_alloc[i];
		desq_cfg.des_base_addr
			= __US_TC_LOCAL_Q_DES_LIST_BASE + (dcnt * DW_SZ(desc));
		if (bonding)
			desq_cfg.bp_des_base_addr
				= __US_TC_LOCAL_Q_DES_LIST_BP_BASE +
							dcnt * BOND_DESC_SZ;
		addr = fpi_addr(__US_TC_LOCAL_Q_CFG_CTXT_BASE
				+ (i * DW_SZ(desq_cfg)));
		tc_mem_write(priv, addr, &desq_cfg, sizeof(desq_cfg));
		dcnt += us_des_alloc[i];
	}

	/* Initialize UpStream Descriptors */
	memset(&desc, 0, sizeof(desc));
	desc.own = !US_LOCALQ_DES_OWN;
	desc.data_len = DMA_PACKET_SZ;

	/* Initialize UpStream Descriptors */
	dcnt = __US_TC_LOCAL_Q0_DES_LIST_NUM + __US_TC_LOCAL_Q1_DES_LIST_NUM;
	pdbram_base = ptm_pdbram_mem_layout(priv, PDBRAM_US, bonding);
	for (i = 0; i < dcnt; i++) {
		desc.data_ptr = priv->ep->phy_membase +
				(pdbram_base + (i * DMA_PACKET_SZ));
		addr = fpi_addr(__US_TC_LOCAL_Q_DES_LIST_BASE
			+ (i * DW_SZ(desc)));
		tc_mem_write(priv, addr, &desc, sizeof(desc));
	}

	/* Setup the Local DESQ Configuration/Context for DownStream Queues */
	memset(&desq_cfg, 0, sizeof(desq_cfg));
	desq_cfg.des_in_own_val = DS_LOCALQ_DES_OWN;
	desq_cfg.des_num = __DS_TC_LOCAL_Q_DES_LIST_NUM;
	desq_cfg.des_base_addr = __DS_TC_LOCAL_Q_DES_LIST_BASE;
	if (bonding)
		desq_cfg.bp_des_base_addr = __DS_TC_LOCAL_Q_DES_LIST_BP_BASE;
	tc_mem_write(priv, fpi_addr(__DS_TC_LOCAL_Q_CFG_CTXT),
		&desq_cfg, sizeof(desq_cfg));

	/* Initialize DownStream Descriptors */
	memset(&desc, 0, sizeof(desc));
	/* CDMA: own: 0--->CPU, 1--->DMA */
	desc.own = !DS_LOCALQ_DES_OWN;
	desc.data_len = DMA_PACKET_SZ;

	pdbram_base = ptm_pdbram_mem_layout(priv, PDBRAM_DS, bonding);
	for (i = 0; i < __DS_TC_LOCAL_Q_DES_LIST_NUM; i++) {
		desc.data_ptr = priv->ep->phy_membase
				+ (pdbram_base + (i * DMA_PACKET_SZ));
		addr = fpi_addr(__DS_TC_LOCAL_Q_DES_LIST_BASE
				+ (i * DW_SZ(desc)));
		tc_mem_write(priv, addr, &desc, sizeof(desc));
	}

	return;
}

/**
 * SL/US Bond/DS Bond
 * OutQ0/US_fragQ1/5 ---->US LocalQ1 (pre-emption)
 * OutQ1/US_fragQ0/4 ---->US LocalQ0 (none pre-emption)
 * DS LocalQ0 ---->DS PKT Q0/DS FragQ0/DS FragQ1
 */
static void cdma_copy_ch_init(struct ptm_ep_priv *priv, int ep_id, int bonding)
{
	cdma_copy_ch_cfg_t ch_cfg;
	u32 addr, size, dbase;
	int i, id;

	size = DW_SZ(desq_cfg_ctxt_t);
	for (i = 0; i < 2; i++) {
		addr = fpi_addr(__US_CDMA_COPY_CH_CFG + i);
		if (bonding) {
			if (is_us_bond(priv))
				id = 0;
			else
				id = 4;
			dbase = __US_FRAGQ_CFG_CTXT_BASE + id * size;
		} else
			dbase = __US_QOS_OUTQ_DES_CFG_CTXT_BASE;

		if (!bonding)
			ch_cfg.srcq_ctxt_ptr = dbase + ((1 - i) * size);
		else
			ch_cfg.srcq_ctxt_ptr = dbase + (i * size);
		ch_cfg.dstq_ctxt_ptr
			= __US_TC_LOCAL_Q_CFG_CTXT_BASE + (i * size);
		tc_mem_write(priv, addr, &ch_cfg, sizeof(ch_cfg));
	}

	addr = fpi_addr(__DS_CDMA_COPY_CH_CFG);
	if (bonding) {
		if (is_us_bond(priv))
			id = 0;
		else
			id = 1;
		dbase = __DS_FRAGQ_CFG_CTXT_BASE + id * size;
	} else
		dbase = __DS_PKT_DESQ_CFG_CTXT;
	ch_cfg.srcq_ctxt_ptr = __DS_TC_LOCAL_Q_CFG_CTXT;
	ch_cfg.dstq_ctxt_ptr = dbase;
	tc_mem_write(priv, addr, &ch_cfg, sizeof(ch_cfg));
	return;
}

static void ptm_cdma_ch_setup(struct ptm_ep_priv *priv, u32 cpch_sb_addr,
					int rx_cid, int tx_cid)
{
	u32 rxbase, txbase;
	u32 rxnum, txnum, addr;
	const u32 phybase = priv->ep->phy_membase;
	cdma_copy_ch_cfg_t ch_cfg;
	desq_cfg_ctxt_t des_cfg;

	addr = fpi_addr(cpch_sb_addr);
	tc_mem_read(priv, &ch_cfg, addr, sizeof(ch_cfg));

	addr = fpi_addr(ch_cfg.srcq_ctxt_ptr);
	tc_mem_read(priv, &des_cfg, addr, sizeof(des_cfg));
	if (priv->tc_priv->param.cdma_desc_loc == LOC_IN_SRAM)
		txbase = phybase + SB_XBAR_ADDR(des_cfg.des_base_addr);
	else
		txbase = phybase + fpi_addr(des_cfg.des_base_addr);
	txnum = des_cfg.des_num;

	addr = fpi_addr(ch_cfg.dstq_ctxt_ptr);
	tc_mem_read(priv, &des_cfg, addr, sizeof(des_cfg));
	if (priv->tc_priv->param.cdma_desc_loc == LOC_IN_SRAM)
		rxbase = phybase + SB_XBAR_ADDR(des_cfg.des_base_addr);
	else
		rxbase = phybase + fpi_addr(des_cfg.des_base_addr);
	rxnum = des_cfg.des_num;

	cdma_ch_cfg(priv, rx_cid, tx_cid,
		rxbase, rxnum, txbase, txnum);

	return;
}

static void ptm_cdma_init(struct ptm_ep_priv *priv)
{
	int i;
	u32 addr;

	tc_cdma_init(priv);

	/* US CDMA channel init */
	for (i = 0; i < 2; i++) {
		addr = __US_CDMA_COPY_CH_CFG + i * DW_SZ(cdma_copy_ch_cfg_t);
		ptm_cdma_ch_setup(priv, addr, (i << 1), (i << 1) + 1);
	}

	/* DS CDMA channel init */
	addr = __DS_CDMA_COPY_CH_CFG;
	ptm_cdma_ch_setup(priv, addr, DS_DMA_RXCH, DS_DMA_TXCH);

	for (i = US_DMA_PRE_RXCH; i <= DS_DMA_TXCH; i += 2)
		cdma_ch_on(priv, i, i + 1);

	return;
}

static void ptm_ds_pkt_desq_cfg_ctxt_init(struct ptm_ep_priv *priv)
{
	desq_cfg_ctxt_t desq_cfg;
	int i;
	rx_descriptor_t desc;
	dma_addr_t phyaddr;
	u32 addr;

	memset(&desq_cfg, 0, sizeof(desq_cfg));
	/* Initialize DS PKT Descriptor Queue Config/Context */
	desq_cfg.des_in_own_val	= DS_PKT_DES_OWN;
	desq_cfg.fast_path	= 0;
	desq_cfg.mbox_int_en	= 0;
	desq_cfg.des_sync_needed = 0;
	desq_cfg.des_num	= __DS_PKT_DES_LIST_NUM;
	desq_cfg.des_base_addr	= __DS_PKT_DES_LIST_BASE;

	tc_mem_write(priv, fpi_addr(__DS_PKT_DESQ_CFG_CTXT),
		&desq_cfg, sizeof(desq_cfg));

	/* Initialize Descriptors in Shadow DS Path Descriptor List */
	/**
	* Shadow descriptor list of SoC, in GRX350, the descriptor pointer
	* should be synced from SoC. in GRX330, it should be allocated by
	* VRX318 itself.
	*/
	memset(&desc, 0, sizeof(desc));
	desc.own = !desq_cfg.des_in_own_val;
	desc.sop = 1;
	desc.eop = 1;
	desc.data_len = DMA_PACKET_SZ;
	addr = desq_cfg.des_base_addr;

	for (i = 0; i < desq_cfg.des_num; i++) {
		if (tc_buf_alloc(priv, desc.data_len, &phyaddr,
				DS_DIR, priv->ep->dev) == NULL) {
			set_tc_status(priv->tc_priv, TC_ERR);
			return;
		}
		desc.data_ptr = phyaddr;
		tc_mem_write(priv, fpi_addr(addr), &desc, sizeof(desc));
		addr += DW_SZ(desc);
	}

	return;
}

/*TODO: Need calulate PDBRAM ERB size when ERB feature is available */
static void sram_workaround(struct ptm_ep_priv *priv)
{
	u32 pdbram_txout_dbase;
	u32 pdbram_rxout_dbase;
	u32 phybase;
	int bonding;

	sb_w32(1, __PD_SHADOW_PRESENT);

	phybase = priv->ep->phy_membase;
	bonding = !is_ptm_sl(priv);

	pdbram_txout_dbase
		= ptm_pdbram_mem_layout(priv, PDBRAM_ACA_TXOUT, bonding);
	pdbram_rxout_dbase
		= ptm_pdbram_mem_layout(priv, PDBRAM_ACA_RXOUT, bonding);
	sb_w32(pdbram_txout_dbase | phybase, __TX_OUT_SHADOW_PTR);
	sb_w32(pdbram_rxout_dbase | phybase, __RX_OUT_SHADOW_PTR);
}

static void ppe_aca_cfg(struct ptm_ep_priv *priv)
{
	us_aca_cfg_ctxt_t us_ctxt;
	ds_aca_cfg_ctxt_t ds_ctxt;
	struct aca_event_reg_addr aca;
	u32 phybase;

	/* 0 means belong to PPE FW, otherwise belong to ACA FW */
	sb_w32(1, __TX_IN_ACA_ACCUM_STATUS);
	sb_w32(1, __TX_OUT_ACA_ACCUM_STATUS);
	sb_w32(1, __RX_IN_ACA_ACCUM_STATUS);
	sb_w32(1, __RX_OUT_ACA_ACCUM_STATUS);

	memset(&us_ctxt, 0, sizeof(us_ctxt));
	memset(&ds_ctxt, 0, sizeof(ds_ctxt));

	phybase = priv->ep->phy_membase;
	priv->ep->hw_ops->aca_event_addr_get(priv->ep, &aca);
	us_ctxt.tx_in_aca_msg_ptr = aca.txin_acc_sub | phybase;
	us_ctxt.tx_out_aca_msg_ptr = aca.txout_acc_add | phybase;
	us_ctxt.tx_out_aca_status_ptr
		= SB_XBAR_ADDR(__TX_OUT_ACA_ACCUM_STATUS) | phybase;
	ds_ctxt.rx_in_aca_msg_ptr = aca.rxin_acc_sub | phybase;
	ds_ctxt.rx_out_aca_msg_ptr = aca.rxout_acc_add | phybase;
	ds_ctxt.rx_in_aca_status_ptr
		= SB_XBAR_ADDR(__RX_IN_ACA_ACCUM_STATUS) | phybase;
	ds_ctxt.rx_out_aca_status_ptr
		= SB_XBAR_ADDR(__RX_OUT_ACA_ACCUM_STATUS) | phybase;
	tc_mem_write(priv, fpi_addr(__US_ACA_CFG_CTXT_BASE),
		&us_ctxt, sizeof(us_ctxt));
	tc_mem_write(priv, fpi_addr(__DS_ACA_CFG_CTXT_BASE),
		&ds_ctxt, sizeof(ds_ctxt));

	sb_w32(phybase, __EP_ADDR_BAR);
	if (priv->tc_priv->param.cdma_desc_loc == LOC_IN_FPI)
		sram_workaround(priv);
}

/**
 * Pre-emption queue(Bonding Group 0) ---> GIF 1 and GIF 5
 * Non-preemption queue(Bonding Group 1) ---> GIF 0 and GIF 4
 */
static void ptm_us_bg_ctxt_init(struct ptm_ep_priv *priv, int ep_id)
{
	int i;
	us_bg_ctxt_t us_bg_ctxt;
	us_bg_gmap_t us_bg_gmap;
	u32 addr;

	if (!is_us_bond(priv))
		return;

	memset(&us_bg_ctxt, 0, sizeof(us_bg_ctxt));
	memset(&us_bg_gmap, 0, sizeof(us_bg_gmap));

	for (i = 0; i < US_BG_NUM; i++) {
		us_bg_ctxt.desq_cfg_ctxt_ptr = __US_QOS_OUTQ_DES_CFG_CTXT_BASE
			+ (i * (DW_SZ(desq_cfg_ctxt_t)));
		addr = fpi_addr(__US_BG_CTXT_BASE + (i * DW_SZ(us_bg_ctxt_t)));
		tc_mem_write(priv, addr, &us_bg_ctxt, sizeof(us_bg_ctxt_t));
	}

	/* bonding group 0 mapping to GIF 1 and GIF 5 */
	us_bg_gmap.gif_map0 = BIT(1) | BIT(5);
	/* bonding group 1 mapping to GIF 0 and GIF 4 */
	us_bg_gmap.gif_map1 = BIT(0) | BIT(4);
	tc_mem_write(priv, fpi_addr(__US_BG_GMAP),
			&us_bg_gmap, sizeof(us_bg_gmap));

	return;
}

static void ptm_ds_bg_ctxt_init(struct ptm_ep_priv *priv, int ep_id)
{
	int i;
	u32 addr;
	ds_bg_ctxt_t ds_bg_ctxt;
	ds_bg_gmap_t ds_bg_gmap;

	if (ep_id != DS_BOND)
		return;

	memset(&ds_bg_ctxt, 0, sizeof(ds_bg_ctxt_t));
	ds_bg_ctxt.last_eop = 1;

	for (i = 0; i < 4; i++) {
		addr = fpi_addr(__DS_BG_CTXT_BASE + (i * DW_SZ(ds_bg_ctxt_t)));
		tc_mem_write(priv, addr, &ds_bg_ctxt, sizeof(ds_bg_ctxt_t));
	}

	/* bonding group 0 mapping to GIF 0 and GIF 4*/
	memset(&ds_bg_gmap, 0, sizeof(ds_bg_gmap));
	ds_bg_gmap.gif_map0 = BIT(0) | BIT(4);
	tc_mem_write(priv, fpi_addr(__DS_BG_GMAP),
			&ds_bg_gmap, sizeof(ds_bg_gmap));

	return;
}

/**
 * Bonding descriptor format:
 * 16 DMA descriptrs followd by 16 Bonding descriptors
 */
static void us_fragq_des_cfg_ctxt_init(struct ptm_ep_priv *priv, int ep_id)
{
	int i, j;
	u32 size;
	desq_cfg_ctxt_t des_cfg;
	rx_descriptor_t desc;
	unsigned int addr;

	memset(&des_cfg, 0, sizeof(des_cfg));

	if (ep_id == DS_BOND)
		i = 4;
	else
		i = 0;

	size = DW_SZ(rx_descriptor_t) + BOND_DESC_SZ;
	for (; i < __US_FRAGQ_DES_LIST_NUM_Q; i++) {
		/* Initialize Upstream Descriptor Queue Config/Context */
		des_cfg.des_in_own_val  = US_FRAGQ_DES_OWN;
		des_cfg.fast_path       = 0;
		des_cfg.mbox_int_en     = 0;
		des_cfg.des_sync_needed = 0;
		des_cfg.gif_id          = i;
		des_cfg.des_num         = __US_FRAGQ_DES_LIST_PERQ_NUM;
		des_cfg.des_base_addr = __US_FRAGQ_DES_LIST_BASE
					+ (i * size * des_cfg.des_num);
		des_cfg.bp_des_base_addr = des_cfg.des_base_addr
					+ (DW_SZ(desc) * des_cfg.des_num);
		addr = fpi_addr(__US_FRAGQ_CFG_CTXT_BASE + i * DW_SZ(des_cfg));
		tc_mem_write(priv, addr, &des_cfg, sizeof(des_cfg));

		/* Only OWN bit init is required in FragmentQ Desc List */
		memset(&desc, 0, sizeof(desc));
		desc.own = !US_FRAGQ_DES_OWN;

		for (j = 0; j < des_cfg.des_num; j++) {
			addr = __US_FRAGQ_DES_LIST_BASE
				+ i * size * des_cfg.des_num + j * DW_SZ(desc);
			tc_mem_write(priv, fpi_addr(addr), &desc, sizeof(desc));
		}
	}

	return;
}

static void ds_fragq_des_cfg_ctxt_init(struct ptm_ep_priv *priv, int ep_id)
{
	int i;
	u32 addr, size, id;
	desq_cfg_ctxt_t des_cfg;
	rx_descriptor_t desc;
	dma_addr_t phy_addr;

	memset(&des_cfg, 0, sizeof(des_cfg));

	if (ep_id == DS_BOND)
		id = 2;
	else if (ep_id == US_BOND)
		id = 1;
	else {
		tc_err(priv->tc_priv, MSG_INIT, "ep id wrong: %d\n", ep_id);
		return;
	}

	size = DW_SZ(desc) + BOND_DESC_SZ;
	for (i = 0; i < id; i++) {
		/* Initialize Downstream Descriptor Queue Config/Context */
		des_cfg.des_in_own_val  = DS_FRAGQ_DES_OWN;
		des_cfg.fast_path       = 0;
		des_cfg.mbox_int_en     = 0;
		des_cfg.des_sync_needed = 0;
		des_cfg.gif_id          = (i == 1) ? 4 : 0;
		des_cfg.des_num         = __DS_FRAGQ_DES_LIST_PERQ_NUM;
		des_cfg.des_base_addr   = __DS_FRAGQ_DES_LIST_BASE +
				(i * size * des_cfg.des_num);
		des_cfg.bp_des_base_addr = des_cfg.des_base_addr
				+ (DW_SZ(desc) * des_cfg.des_num);

		addr = fpi_addr(__DS_FRAGQ_CFG_CTXT_BASE
				+ (i * DW_SZ(des_cfg)));
		tc_mem_write(priv, addr, &des_cfg, sizeof(des_cfg));
	}
	/* Initialize OWN bit of desc in FragmentQ Descriptor List */
	memset(&desc, 0, sizeof(desc));
	desc.own = !DS_FRAGQ_DES_OWN;
	desc.data_len = DMA_PACKET_SZ;

	for (i = 0; i < des_cfg.des_num; i++) {
		addr = fpi_addr(des_cfg.des_base_addr + i * DW_SZ(desc));
		if (tc_buf_alloc(priv, DMA_PACKET_SZ,
				&phy_addr, DS_DIR, priv->ep->dev) == NULL) {
			set_tc_status(priv->tc_priv, TC_ERR);
			return;
		}

		desc.data_ptr = (unsigned int)phy_addr;
		tc_mem_write(priv, addr, &desc, sizeof(desc));
	}

	/**
	 * DS bonding master need init shadow descriptor list
	 * for US bonding master to sync
	 * Only ownbit init is required
	 */
	if (ep_id == DS_BOND) {
		memset(&desc, 0, sizeof(desc));
		desc.own = !DS_FRAGQ_DES_OWN;
		for (i = 0; i < __DS_FRAGQ_DES_LIST_PERQ_NUM; i++) {
			addr = fpi_addr(__DS_FRAGQ_DES_LIST_BASE
				+ i * DW_SZ(desc));
			tc_mem_write(priv, addr, &desc, sizeof(desc));
		}
	}

	return;
}

/**
 * Total 256 free descriptor, 9(1 free frag link list, 8 (2(BC) * 4(GIF)) frag link list
 * 256 free desciptor in free frag link list on initialization.
 */
static void ds_ll_ctxt_init(struct ptm_ep_priv *priv, int ep_id)
{
	int i;
	u32 addr;
	ds_bond_gif_ll_ctxt_t ll_ctxt;
	ds_bond_gif_ll_des_t ll_desc;
	dma_addr_t phy_addr;

	if (ep_id != DS_BOND)
		return;

	memset(&ll_ctxt, 0, sizeof(ll_ctxt));

	ll_ctxt.max_des_num = ((BOND_LL_DES_NUM / 8) < 0xFF) ?
				(BOND_LL_DES_NUM / 8) : 0xFF;
	/* if free_des_num <= 16, timeout any frag */
	ll_ctxt.to_buff_thres = 16;
	ll_ctxt.max_delay = 0xFFFF; /*  TODO: fix the number */

	addr = __DS_BOND_LL_CTXT_BASE;
	for (i = 0; i < BOND_LL_NUM; i++) {
		addr = fpi_addr(__DS_BOND_LL_CTXT_BASE + i * DW_SZ(ll_ctxt));
		tc_mem_write(priv, addr, &ll_ctxt, sizeof(ll_ctxt));
	}

	ll_ctxt.head_ptr = __DS_BOND_GIF_LL_DES_BA;
	ll_ctxt.tail_ptr = __DS_BOND_GIF_LL_DES_BA +
				((BOND_LL_DES_NUM - 1) * 2);
	ll_ctxt.des_num = BOND_LL_DES_NUM;
	tc_mem_write(priv, fpi_addr(__DS_BOND_LL_CTXT_BASE),
		&ll_ctxt, sizeof(ll_ctxt));

	/* Initialize DS GIF LL Descriptors */
	/* link list, last one pointer back to the first one */
	for (i = 0; i < BOND_LL_DES_NUM; i++) {
		ll_desc.next_des_ptr
			= __DS_BOND_GIF_LL_DES_BA +
			(((i + 1) % BOND_LL_DES_NUM) * DW_SZ(ll_desc));
		ll_desc.data_len = DMA_PACKET_SZ;
		if (tc_buf_alloc(priv, ll_desc.data_len,
			&phy_addr, DS_DIR, priv->ep->dev) == NULL) {
			set_tc_status(priv->tc_priv, TC_ERR);
			return;
		}
		ll_desc.data_ptr = phy_addr;
		tc_mem_write(priv, DS_BOND_GIF_LL_DES_BA(i),
			&ll_desc, sizeof(ll_desc));
	}

	return;
}

static void ptm_fw_bonding_init(struct ptm_ep_priv *priv,
						u32 ep_id, int bonding)
{
	if (!bonding)
		return;

	ptm_us_bg_ctxt_init(priv, ep_id);
	ptm_ds_bg_ctxt_init(priv, ep_id);
	us_fragq_des_cfg_ctxt_init(priv, ep_id);
	ds_fragq_des_cfg_ctxt_init(priv, ep_id);
	ds_ll_ctxt_init(priv, ep_id);
}

/**
 *  PTM TC HW and FW Initialization
 */
static void ppe_ptm_fw_hw_init(struct ptm_ep_priv *priv,
				u32 ep_id, int bonding)
{
	/* Clear PPE SB */
	ppe_sb_clear(priv);
	/* Clear PDBRAM */
	pdbram_clear(priv);

	ptm_hw_init(priv);
	ptm_fw_init(priv, bonding);

	ptm_gen_cfg_init(priv, bonding);
	us_qos_cfg_init(priv);
	ptm_local_desq_cfg_ctxt_init(priv, bonding);
	ptm_erb_init(priv, bonding);
	ptm_ds_pkt_desq_cfg_ctxt_init(priv);
	ptm_fw_bonding_init(priv, ep_id, bonding);

	cdma_copy_ch_init(priv, ep_id, bonding);
	ptm_cdma_init(priv);
	ppe_aca_cfg(priv);
}

/**
 * Disable Powersaving
 * Enable MBOX1 Interrupt (MBOX_FW_UNLOAD)
 * Send MBOX5 Interrupt to wake up PPE FW
 */
static void ptm_ppe_wakeup(struct ptm_ep_priv *priv)
{
	u32 mbox_ier;

	if (priv->tc_priv->param.ps) {
		ptm_power_saving(priv, 0, 1);
		priv->tc_priv->param.ps = 1;
	}

	mbox_ier = tc_r32(MBOX_IGU_IER(MBOX_IGU1));
	mbox_ier |= MBOX_FRAME_STOP;
	mbox_set_ier(priv, MBOX_IGU1, mbox_ier);

	if (priv->tc_priv->param.ps)
		mbox_set_isr(priv, PPE_FW_MBOX0, MBOX_FW_WAKEUP);
}

/**
 * Restore Powersaving setting.
 */
static void ptm_ppe_ps_restore(struct ptm_ep_priv *priv)
{
	u32 mbox_ier;

	mbox_ier = tc_r32(MBOX_IGU_IER(MBOX_IGU1));
	mbox_ier &= ~MBOX_FRAME_STOP;
	mbox_set_ier(priv, MBOX_IGU1, mbox_ier);

	if (priv->tc_priv->param.ps)
		ptm_power_saving(priv, 1, 1);
}

/**
 * Driver lift ACA TXIN after enter showtime
 * Default DFE rate is 1500, which mapping to max value 0x1FFF.
 */
static int ptm_showtime_enter(const unsigned char idx,
		struct port_cell_info *port_cell, void *xdata_addr)
{
	struct ptm_priv *ptm_tc = g_ptm_priv;
	struct ptm_ep_priv *priv = tc_ep_priv(idx);
	struct ptm_ep_priv *uspriv;
	struct dc_ep_dev *ep;
	u32 rate;

	tc_info(priv->tc_priv, MSG_EVENT, "Line[%d]: show time enter!!\n", idx);
	netif_carrier_on(ptm_tc->dev);
	ptm_ppe_ps_restore(priv);

	if (is_ptm_sl(priv)) {
		ep = priv->ep;
		ep->hw_ops->aca_start(priv->ep, ACA_TXIN_EN, 0);
	} else { /* in bonding mode, must enable us_bonding master */
		uspriv = tc_ep_priv(US_BOND);
		ep = priv->ep;
		ep->hw_ops->aca_start(priv->ep, ACA_TXIN_EN, 0);
	}

	if (ptm_tc->tc_priv->param.dfe_loopback == 1) {
		priv->dfe_rate = port_cell->tx_link_rate[0];
		rate = (priv->dfe_rate * 0x2000) / 1500;
		if (rate >= 0x1FFF)
			rate = 0x1FFF;
		tc_w32(rate, DREG_B_LADR(0));
	}

	return 0;
}

/**
 * Driver Halt ACA TXIN after leave showtime
 * TTHA reset is required if showtime exit.
 */
static int ptm_showtime_exit(const unsigned char idx)
{
	struct ptm_priv *ptm_tc = g_ptm_priv;
	struct ptm_ep_priv *priv = tc_ep_priv(idx);
	u32 stop = ACA_TXIN_EN;
	struct dc_ep_dev *ep;

	tc_info(priv->tc_priv, MSG_EVENT, "Line[%d]: show time exit!\n", idx);

	if (!showtime_stat(ptm_tc->tc_priv)) {
		ep = priv->ep;
		ep->hw_ops->aca_stop(ep, &stop, 0);
		netif_carrier_off(ptm_tc->dev);
	}
	ptm_ppe_wakeup(priv);

	return 0;
}

static int ptm_erb_addr_get(const unsigned char idx,
	unsigned int *data_addr, unsigned int *desc_addr)
{
	struct ptm_ep_priv *priv = tc_ep_priv(idx);
	unsigned int addr;
	int bonding;

	bonding = !is_ptm_sl(priv);

	addr = ptm_pdbram_mem_layout(priv, PDBRAM_ERB, bonding);
	*data_addr  = (priv->ep->phy_membase | addr) & 0xFFFF0000;
	if (!bonding || is_us_bond(priv))
		addr = __OUTQ_ERB0_BASE;
	else
		addr = __OUTQ_ERB_PEER_BASE;

	addr = fpi_addr(addr);
	*desc_addr = (priv->ep->phy_membase | addr) & 0xFFFF0000;

	tc_dbg(priv->tc_priv, MSG_EVENT,
		"idx: %d, data addr: 0x%x,  desc_addr: 0x%x\n",
		idx, *data_addr, *desc_addr);

	return 0;
}

static int ptm_framer_requst_en(const unsigned char idx)
{
	struct ptm_ep_priv *priv = tc_ep_priv(idx);

	ttha_enable(priv);

	return 0;
}

static void ptm_cb_setup(struct ptm_priv *ptm_tc, int flag)
{
	spin_lock_bh(&ptm_tc->ptm_lock);

	if (!flag) {
		ptm_tc->tc_priv->tc_ops.recv = NULL;
		ptm_tc->tc_priv->tc_ops.get_qid = NULL;
		ptm_tc->tc_priv->tc_ops.irq_on = NULL;
		ptm_tc->tc_priv->tc_ops.irq_off = NULL;
		ptm_tc->tc_priv->tc_ops.showtime_enter = NULL;
		ptm_tc->tc_priv->tc_ops.showtime_exit = NULL;
		ptm_tc->tc_priv->tc_ops.erb_addr_get = NULL;
		ptm_tc->tc_priv->tc_ops.framer_request_en = NULL;
	} else {
		ptm_tc->tc_priv->tc_ops.recv = ptm_rx;
		ptm_tc->tc_priv->tc_ops.get_qid = ptm_get_qid;
		ptm_tc->tc_priv->tc_ops.irq_on = ptm_irq_on;
		ptm_tc->tc_priv->tc_ops.irq_off = ptm_irq_off;
		ptm_tc->tc_priv->tc_ops.showtime_enter = ptm_showtime_enter;
		ptm_tc->tc_priv->tc_ops.showtime_exit = ptm_showtime_exit;
		ptm_tc->tc_priv->tc_ops.erb_addr_get = ptm_erb_addr_get;
		ptm_tc->tc_priv->tc_ops.framer_request_en
			= ptm_framer_requst_en;
	}
	spin_unlock_bh(&ptm_tc->ptm_lock);
}

static irqreturn_t ptm_irq_handler(int irq, void *dev_id)
{
	u32 mbox1_isr, mbox1_ier, mbox1_isrc;
	/*u32 peer_state, peer_base;*/
	struct ptm_ep_priv *priv = dev_id;

	mbox1_ier = tc_r32(MBOX_IGU_IER(MBOX_IGU1));
	mbox1_isrc = tc_r32(MBOX_IGU_ISR(MBOX_IGU1));

	mbox1_isr = mbox1_isrc & mbox1_ier;

	if (mbox1_isr & MBOX_PKT_RX) {
		mbox1_ier &= ~MBOX_PKT_RX;
		mbox_set_ier(priv, MBOX_IGU1, mbox1_ier);
		if (priv->tc_priv->tc_ops.coc_req != NULL)
			priv->tc_priv->tc_ops.coc_req();
	}

	if (mbox1_isr & MBOX_FRAME_STOP)
		ttha_disable(priv, 0);

	mbox_clr_isr(priv, MBOX_IGU1, mbox1_isrc);

	return IRQ_HANDLED;
}

static void ptm_datapath_init(struct ptm_ep_priv *priv, const char *dev_name)
{
	int ret;

	ret = devm_request_irq(priv->ep->dev, priv->ep->irq,
			ptm_irq_handler, 0, dev_name, priv);

	if (ret) {
		tc_err(priv->tc_priv, MSG_INIT, "PTM Register Interrupt Fail!\n");
		return;
	}

	return;
}

static void ptm_fw_load(struct ptm_priv *ptm_tc)
{
	int i;
	u32 size;
	struct ppe_fw *fw = &ptm_tc->fw;
	struct fw_bin *binfw = &ptm_tc->tc_priv->fw;

	memcpy(&fw->atm_ver, &binfw->fw_hdr.atm_ver, sizeof(fw->atm_ver));
	memcpy(&fw->ptm_ver, &binfw->fw_hdr.ptm_ver, sizeof(fw->ptm_ver));
	for (i = 0; i < PP32_MAX; i++) {
		size = binfw->fw_hdr.fw_info[i].fw_size;
		fw->ptm_fw.size[i] = binfw->fw_hdr.fw_info[i].fw_size;
		fw->ptm_fw.data[i] = (const u32 *)binfw->fw_ptr[i];
	}
}

/**
 * 1. Powerup PPE component
 * 2. Init PPE, enable Idle cell, Enable TTHA
 * 3. Powerup and configure CDMA
 * 4. Enable loopback if required
 * 5. PPE start
 */
static void ptm_tc_hw_fw_init(struct ptm_ep_priv *priv,
				u32 ep_id, int bonding)
{
	int i;
	const char *drv_name;
	struct ptm_priv *ptm_tc;

	tc_dbg(priv->tc_priv, MSG_SWITCH, "id(Line)\t\t= %d\n", ep_id);
	tc_dbg(priv->tc_priv, MSG_SWITCH, "irq\t\t\t= %u\n", priv->ep->irq);
	tc_dbg(priv->tc_priv, MSG_SWITCH, "membase\t\t= 0x%08x\n",
		(unsigned int __force)priv->ep->membase);
	tc_dbg(priv->tc_priv, MSG_SWITCH,
		"phy_membase\t\t= 0x%08x\n", priv->ep->phy_membase);
	tc_dbg(priv->tc_priv, MSG_SWITCH,
		"peer_num\t\t= %u\n", priv->ep->peer_num);
	tc_dbg(priv->tc_priv, MSG_SWITCH, "tc_mode\t\t= %s\n",
		bonding ? "PTM Bonding" : "PTM Single Line");

	ptm_tc = g_ptm_priv;
	/* Power on Modem component */
	tc_clkon(priv->ep, PMU_PTM);
	/* Reset PPE HW */
	tc_ppe_rst(priv->ep);
	/* Set VRX318 PPE clock 576MHz */
	tc_ppe_clkset(priv->ep, PPE_CLK_576MHZ);
	/* Clear and disable mailbox */
	for (i = MBOX_IGU0; i <= MBOX_IGU2; i++) {
		mbox_set_ier(priv, i, 0);
		mbox_clr_isr(priv, i, MBOX_MASK(i));
	}

	/* Freeze PP32 */
	ppe_stop(priv);
	ppe_ptm_fw_hw_init(priv, ep_id, bonding);
	ptm_fw_load(ptm_tc);
	pp32_load(priv, &ptm_tc->fw, 0);

	/* init datapath */
	if (bonding)
		drv_name = ptm_bond_name[ep_id];
	else
		drv_name = ptm_drv_name;
	ptm_datapath_init(priv, drv_name);

	/* start hardware */
	setup_dfe_loopback(priv, priv->dfe_rate);
	ppe_start(priv);
	dfe_loopback_linkup(priv);

	/* Indicate DSL FW all configuration is ready */
	/* PPE FW is ready to receive/send packets */
	tc_stat_indicate(priv, priv->tc_priv->tc_mode, 1);

	/* enable mailbox */
	mbox_set_ier(priv, MBOX_IGU1, MBOX_PKT_RX | MBOX_FRAME_STOP);

	tc_info(priv->tc_priv, MSG_SWITCH, "PTM TC init successfully\n");
	return;
}

static void ptm_aca_init(struct ptm_ep_priv *priv)
{
	struct aca_param param;
	struct aca_modem_param mdm;
	struct aca_cfg_param *txin;
	struct aca_cfg_param *txout;
	struct aca_cfg_param *rxout;
	struct soc_cfg *cfg;
	u32 phybase = priv->ep->phy_membase;
	u32 start;
	u32 type;

	priv->tc_priv->tc_ops.soc_cfg_get(&priv->tc_priv->cfg, ptm_id(priv));
	memset(&param, 0, sizeof(param));
	cfg = &priv->tc_priv->cfg;

	txin = &param.aca_txin;
	txin->byteswap = 1;
	txin->hd_size_in_dw = cfg->desc_dw_sz;
	txin->pd_desc_base = SB_XBAR_ADDR(__ACA_TX_IN_PD_LIST_BASE);
	txin->pd_desc_num = __ACA_TX_IN_PD_LIST_NUM;
	txin->pd_size_in_dw = DESC_DWSZ;
	txin->soc_desc_base = cfg->txin_dbase;
	txin->soc_desc_num = cfg->txin_dnum;
	txin->pp_buf_desc_num = ACA_TXIN_HD_DESC_NUM;

	tc_dbg(priv->tc_priv, MSG_INIT,
		"txin: bswp: %d, hdsz:%d, pd: dbase(0x%x), dnum(%d), sz_indw(%d), soc_dbase:0x%x, soc_dnum:0x%x\n",
		txin->byteswap, txin->hd_size_in_dw, txin->pd_desc_base,
		txin->pd_desc_num, txin->pd_size_in_dw,
		txin->soc_desc_base, txin->soc_desc_num);

	txout = &param.aca_txout;
	txout->byteswap = 1;
	txout->hd_size_in_dw = 1;
	if (priv->tc_priv->param.cdma_desc_loc == LOC_IN_FPI)
		txout->pd_desc_base = sb_r32(__TX_OUT_SHADOW_PTR);
	else
		txout->pd_desc_base = SB_XBAR_ADDR(__ACA_TX_OUT_PD_LIST_BASE);
	txout->pd_desc_num = __ACA_TX_OUT_PD_LIST_NUM;
	txout->pd_size_in_dw = DESC_DWSZ;
	txout->soc_desc_base = cfg->txout_dbase;
	txout->soc_desc_num = cfg->txout_dnum;
	txout->pp_buf_desc_num = ACA_TXOUT_HD_DESC_NUM;

	tc_dbg(priv->tc_priv, MSG_INIT,
		"txout: bswp: %d, hdsz:%d, pd: dbase(0x%x), dnum(%d), sz_indw(%d), soc_dbase:0x%x, soc_dnum:0x%x\n",
		txout->byteswap, txout->hd_size_in_dw, txout->pd_desc_base,
		txout->pd_desc_num, txout->pd_size_in_dw,
		txout->soc_desc_base, txout->soc_desc_num);

	rxout = &param.aca_rxout;
	rxout->byteswap = 1;
	rxout->hd_size_in_dw = cfg->desc_dw_sz;
	if (priv->tc_priv->param.cdma_desc_loc == LOC_IN_FPI)
		rxout->pd_desc_base = sb_r32(__RX_OUT_SHADOW_PTR);
	else
		rxout->pd_desc_base = SB_XBAR_ADDR(__ACA_RX_OUT_PD_LIST_BASE);
	rxout->pd_desc_num = __ACA_RX_OUT_PD_LIST_NUM;
	rxout->pd_size_in_dw = DESC_DWSZ;
	rxout->soc_desc_base = cfg->rxout_dbase;
	rxout->soc_desc_num = cfg->rxout_dnum;
	rxout->pp_buf_desc_num = ACA_RXOUT_HD_DESC_NUM;

	tc_dbg(priv->tc_priv, MSG_INIT,
		"rxout: bswp: %d, hdsz:%d, pd: dbase(0x%x), dnum(%d), sz_indw(%d), soc_dbase:0x%x, soc_dnum:0x%x\n",
		rxout->byteswap, rxout->hd_size_in_dw, rxout->pd_desc_base,
		rxout->pd_desc_num, rxout->pd_size_in_dw,
		rxout->soc_desc_base, rxout->soc_desc_num);

	if (priv->tc_priv->param.cdma_desc_loc == LOC_IN_SRAM)
		type = TC_SRAM_ADDR;
	else
		type = TC_FPI_ADDR;
	mdm.mdm_txout.stat
		= tc_addr(__TX_OUT_ACA_ACCUM_STATUS, type, phybase);
	mdm.mdm_txout.pd
		= tc_addr(__TX_OUT_QUEUE_PD_BASE_ADDR_OFFSET, type, phybase);
	mdm.mdm_txout.acc_cnt
		= tc_addr(__TX_OUT_ACA_ACCUM_COUNT, type , phybase);

	mdm.mdm_rxout.stat
		= tc_addr(__RX_OUT_ACA_ACCUM_STATUS, type, phybase);
	mdm.mdm_rxout.pd
		= tc_addr(__RX_OUT_QUEUE_PD_BASE_ADDR_OFFSET, type, phybase);
	mdm.mdm_rxout.acc_cnt
		= tc_addr(__RX_OUT_ACA_ACCUM_COUNT, type, phybase);

	mdm.mdm_rxin.stat
		= tc_addr(__RX_IN_ACA_ACCUM_STATUS, type, phybase);
	mdm.mdm_rxin.pd
		= tc_addr(__RX_IN_QUEUE_PD_BASE_ADDR_OFFSET, type, phybase);
	mdm.mdm_rxin.acc_cnt
		= tc_addr(__RX_IN_ACA_ACCUM_COUNT, type, phybase);

	tc_dbg(priv->tc_priv, MSG_INIT,
		"txout: (stat:0x%x, pd: 0x%x, cnt: 0x%x)\n",
		mdm.mdm_txout.stat, mdm.mdm_txout.pd, mdm.mdm_txout.acc_cnt);

	tc_dbg(priv->tc_priv, MSG_INIT,
		"rxout: (stat:0x%x, pd: 0x%x, cnt: 0x%x)\n",
		mdm.mdm_rxout.stat, mdm.mdm_rxout.pd, mdm.mdm_rxout.acc_cnt);

	tc_dbg(priv->tc_priv, MSG_INIT,
		"rxin: (stat:0x%x, pd: 0x%x, cnt: 0x%x)\n",
		mdm.mdm_rxin.stat, mdm.mdm_rxin.pd, mdm.mdm_rxin.acc_cnt);

	priv->ep->hw_ops->aca_init(priv->ep, &param, &mdm);

	start = ACA_ALL_EN;
	priv->ep->hw_ops->aca_stop(priv->ep, &start, 0);

	if (is_ptm_sl(priv))
		start = ACA_TXOUT_EN | ACA_RXIN_EN | ACA_RXOUT_EN;
	else {
		if (is_us_bond(priv))
			start = ACA_TXOUT_EN;
		else
			start = ACA_RXIN_EN | ACA_RXOUT_EN;
	}

	priv->ep->hw_ops->aca_start(priv->ep, start, 1);
}

static void ptm_umt_init(struct ptm_ep_priv *priv)
{
	struct tc_priv *tcpriv;
	u32 dst;
	u32 period;

	tcpriv = priv->tc_priv;
	dst = priv->ep->hw_ops->umt_msg_addr(priv->ep);
	dst |= priv->ep->phy_membase;
	period = tcpriv->param.umt_period;

	tcpriv->tc_ops.umt_init(ptm_id(priv), period, dst);
}

static void ptm_umt_start(struct ptm_ep_priv *priv)
{
	priv->tc_priv->tc_ops.umt_start(ptm_id(priv));
}

/**
 * 1. Powerup PPE component
 * 2. Init PPE, enable Idle cell, Enable TTHA
 * 3. Powerup and configure CDMA
 * 4. Enable DFE loopback if required
 * 5. PPE start
 * 6. Init UMT and SoC DMA
 * 7. Init and start ACA
 * 8. Start UMT and SoC DMA
 */
void ptm_tc_load(struct tc_priv *tc_priv,
			u32 id, enum dsl_tc_mode tc_mode)
{
	struct ptm_priv *ptm_tc;
	struct ptm_ep_priv *priv;
	int bonding, i;

	if (!g_ptm_priv)
		ptm_dev_init(tc_priv, id, tc_mode);

	ptm_tc = g_ptm_priv;

	/**
	 * Sequence must be guaranteed
	 * 1. umt init
	 * 2. soc cfg get
	 * 3. aca init
	 * 4. umt start
	 */
	bonding = (tc_mode == TC_PTM_BND_MODE);
	if (!bonding) {
		priv = tc_ep_priv(id);
		ptm_tc_hw_fw_init(priv, id, bonding);
		ptm_umt_init(priv);
		ptm_aca_init(priv);
		ptm_umt_start(priv);
		ptm_tc_proc_init(priv);
		ptm_tc->ep_id = id;
	} else {
		for (i = 0; i < BOND_MAX; i++) {
			priv = tc_ep_priv(i);
			ptm_tc_hw_fw_init(priv, i, bonding);
			ptm_umt_init(priv);
			ptm_aca_init(priv);
			ptm_umt_start(priv);
			ptm_tc_proc_init(priv);
		}
	}
	tc_priv->priv = ptm_tc;
	ptm_cb_setup(ptm_tc, 1);
	if (priv->tc_priv->tc_ops.dev_reg != NULL)
		priv->tc_priv->tc_ops.dev_reg(ptm_tc->dev,
			ptm_tc->dev->name, &ptm_tc->subif_id, 0);
	if (priv->tc_priv->tc_ops.disable_us != NULL)
		priv->tc_priv->tc_ops.disable_us(0);

	/* Send userspace TC UP message */
	tc_ntlk_msg_send(tc_priv, 0, PTM_TC, TC_LOADED,
		(tc_mode == TC_PTM_BND_MODE) ? BONDING_MODE : NON_BONDING_MODE,
		id);

	tc_info(tc_priv, MSG_SWITCH, "PTM TC is successfully loaded\n");

	return;
}

static void ptm_datapath_exit(struct ptm_ep_priv *priv)
{
	devm_free_irq(priv->ep->dev, priv->ep->irq, priv);
}

static inline void aca_txin_wait(struct ptm_ep_priv *priv)
{
	msleep(ACA_TXIN_POLL_INTVAL);
}

static void ptm_aca_txin_poll(struct ptm_ep_priv *priv,
			u32 dbase, u32 *idx, u32 dnum, u32 *pkt_num)
{
	rx_descriptor_t desc;
	u32 off, cnt, dst, wrp_rd;

	off = *idx;
	cnt = 0;
	wrp_rd = dnum * DW_SZ(desc);

	tc_dbg(priv->tc_priv, MSG_SWITCH,
		"dbase: 0x%x, idx: %u, pending num: %u, wrap_round: %u\n",
		dbase, off, *pkt_num, wrp_rd);

	while (1) {
		dst = fpi_addr(dbase + off);
		tc_mem_read(priv, &desc, dst, sizeof(desc));
		if (desc.own == US_FP_DES_OWN) {
			tc_dbg(priv->tc_priv, MSG_SWITCH,
				"addr: 0x%x, desc ptr: 0x%x\n",
				dbase + off, desc.data_ptr);
			priv->tc_priv->tc_ops.free(desc.data_ptr, US_DIR);
			desc.own = !desc.own;
			tc_mem_write(priv, dst, &desc, sizeof(desc));
			cnt++;
			off = (off + 2) % wrp_rd;
		} else
			break;
	}

	if (cnt != 0) {
		*idx = off;
		priv->ep->hw_ops->aca_txin_ack_sub(priv->ep, cnt);
		sb_w32(1, __TX_IN_ACA_ACCUM_STATUS);
		*pkt_num -= cnt;
	} else {
		tc_err(priv->tc_priv, MSG_SWITCH,
			"No ACA TXIN PENDING packets!!!\n");
	}

	tc_dbg(priv->tc_priv, MSG_SWITCH,
		"ACA TXIN POLL free %d packets\n", cnt);
}

static int clear_aca_txin(struct ptm_ep_priv *priv,
			u32 dbase, u32 *idx, u32 dnum, u32 pkt_num)
{
	if (!sb_r32(__RX_IN_ACA_ACCUM_STATUS)) {
		tc_err(priv->tc_priv, MSG_SWITCH,
			"ACA RXIN not clean\n");
		return -EBUSY;
	}

	while (pkt_num > 0) {
		tc_dbg(priv->tc_priv, MSG_SWITCH,
			"pending pkt num: %d\n", pkt_num);
		aca_txin_wait(priv);
		ptm_aca_txin_poll(priv, dbase, idx, dnum, &pkt_num);
	}
	return 0;
}

/**
 * Polling all pendinng packets in CBM/ACAs
 */
static void ptm_aca_exit(struct ptm_ep_priv *priv, u32 pkt_num)
{
	struct dc_ep_dev *ep;
	u32 stop, idx;
	desq_cfg_ctxt_t desq_cfg;

	if (!is_ptm_sl(priv) && priv->ep_id == DS_BOND)
		return;

	stop = ACA_TXIN_EN | ACA_TXOUT_EN | ACA_RXIN_EN | ACA_RXOUT_EN;
	ep = priv->ep;

	tc_mem_read(priv, &desq_cfg,
		fpi_addr(__US_FP_INQ_DES_CFG_CTXT), sizeof(desq_cfg));
	idx = desq_cfg.deq_idx;

	clear_aca_txin(priv, desq_cfg.des_base_addr,
			&idx, desq_cfg.des_num, pkt_num);
	if (ep->hw_ops->aca_stop(ep, &stop, 1) < 0)
		tc_err(priv->tc_priv, MSG_SWITCH, "ACA is busy: %u!\n", stop);
}

static void ptm_umt_exit(struct ptm_ep_priv *priv)
{
	struct tc_priv *tcpriv;

	tcpriv = priv->tc_priv;
	tcpriv->tc_ops.umt_exit(ptm_id(priv));
}

static void ptm_cdma_exit(struct ptm_ep_priv *priv, enum dsl_tc_mode mode)
{
	cdma_ch_off(priv, US_DMA_PRE_RXCH, US_DMA_PRE_TXCH);
	cdma_ch_off(priv, US_DMA_NOPRE_RXCH, US_DMA_NOPRE_TXCH);
	cdma_ch_off(priv, DS_DMA_RXCH, DS_DMA_TXCH);
}

static void ptm_free_mem(struct ptm_ep_priv *priv, enum dsl_tc_mode mode)
{
	int i;
	u32 phy_addr, phy_src;
	ds_bond_gif_ll_des_t ll_desc;

	/* Free QoS buffers */
	for (i = 0; i < QOS_DES_NUM; i++) {
		phy_addr = sb_r32(__US_QOSQ_DES_LIST_BASE + (i << 1));
		priv->tc_priv->tc_ops.free(phy_addr, US_DIR);
	}

	/* Free OUTQ queue buffers */
	for (i = 0; i < OUTQ_DESC_NUM; i++) {
		phy_addr = sb_r32(__US_OUTQ_DES_LIST_BASE + (i << 1));
		priv->tc_priv->tc_ops.free(phy_addr, US_DIR);
	}

	/* Free DS PKT queue buffers */
	for (i = 0; i < __DS_PKT_DES_LIST_NUM; i++) {
		phy_addr = sb_r32(__DS_PKT_DES_LIST_BASE + (i << 1));
		priv->tc_priv->tc_ops.free(phy_addr, DS_DIR);
	}

	/* Free DS FRAGQ dest buffers */
	if (mode == TC_PTM_BND_MODE) {
		if (priv->ep_id == US_BOND)
			phy_src = __DS_FRAGQ_DES_LIST_BASE;
		else
			phy_src = __DS_FRAGQ_DES_LIST_BASE
				+ (__DS_FRAGQ_DES_LIST_PERQ_NUM *
					(DESC_DWSZ + BOND_DESC_SZ));
		for (i = 0; i < __DS_FRAGQ_DES_LIST_PERQ_NUM; i++) {
			phy_addr = sb_r32(phy_src + (i << 1));
			priv->tc_priv->tc_ops.free(phy_addr, DS_DIR);
		}

		if (priv->ep_id == DS_BOND)
			for (i = 0; i < BOND_LL_DES_NUM; i++) {
				phy_src = fpi_addr(__DS_BOND_GIF_LL_DES_BA +
						i * DW_SZ(ll_desc));
				tc_mem_read(priv, &ll_desc,
					phy_src, sizeof(ll_desc));
				priv->tc_priv->tc_ops.free(ll_desc.data_ptr,
							DS_DIR);
			}
	}

	return;
}

/**
 * 1. CDMA channel off
 * 2. Clear and disable interrupt
 * 3. Free pre-allocated SoC memory
 * 4. Indicate DSL FW/PPE FW unload
 */
static void ptm_ppe_exit(struct ptm_ep_priv *priv, enum dsl_tc_mode mode)
{
	int i;
	struct ptm_priv *ptm_tc;

	ptm_tc = g_ptm_priv;

	/* Disable Idle cell for RX */
	rtha_idle_keep_disable(priv);

	/* Indicate PPE FW TC switch */
	sb_w32(1, __MODE_SW_CFG);

	/* Test only, without CBM flush, using PPE FW flush */
        priv->ep->hw_ops->aca_start(priv->ep, ACA_TXIN_EN, 0);

        /* Delay some time to allow PPE FW complete its task */
        msleep(1);

	/* Stop PPE FW */
	ppe_stop(priv);

	/* STOP DMA */
	ptm_cdma_exit(priv, mode);

	/* Disable and Clear all Interrupts */
	for (i = MBOX_IGU0; i < MBOX_IGU2; i++) {
		mbox_set_ier(priv, i, 0);
		mbox_clr_isr(priv, i, MBOX_MASK(i));
	}

	/* unregister irq handler */
	ptm_datapath_exit(priv);

	/* free ptm pre-allocated memory */
	ptm_free_mem(priv, mode);

	/* Indicate DSL FW TC Clear */
	tc_stat_indicate(priv, mode, 0);

	return;
}

/**
 * Unload Sequence
 * 1. Stop US traffic by drop incoming traffic in CBM.
 * 2. Stop PPE
 * 3. SoC DMA exit, UMT exit
 * 4. Power down PPE component including CDMA
 * 5. Remove proc
 * 6  unregister sub interface
 * 7. Reset soc port to start from base address
 * 8. send netlink message to Userspace
 */
void ptm_tc_unload(enum dsl_tc_mode tc_mode)
{
	struct ptm_priv *ptm_tc = g_ptm_priv;
	struct ptm_ep_priv *priv;
	int i;
	u32 pending_num;

	/* Disable US traffic */
	ptm_tc->tc_priv->tc_ops.disable_us(1);

	for (i = 0; i < ptm_tc->tc_priv->ep_num; i++) {
		if (tc_mode == TC_PTM_SL_MODE && ptm_tc->ep_id != i)
			continue;

		priv = tc_ep_priv(i);
		tc_dbg(priv->tc_priv, MSG_SWITCH,
				"Unload on id: %d\n", priv->ep_id);

		/* stop PPE */
		ptm_ppe_exit(priv, tc_mode);
		/* Get pending ACA TXIN PKT num */
		/* pending_num = priv->ep->hw_ops->aca_txin_hd_cnt(priv->ep); */
		/* Enable ACA TXIN */
		/* priv->ep->hw_ops->aca_start(priv->ep, ACA_TXIN_EN, 0); */
		/* stop UMT */
		ptm_umt_exit(priv);
		/* stop ACA */
		ptm_aca_exit(priv, pending_num);
		/* modem module power off */
		tc_clkoff(priv->ep, PMU_PTM);
		/* remove proc */
		ptm_tc_proc_exit(priv);
	}

	/* unregister sub interface */
	if (ptm_tc->tc_priv->tc_ops.dev_unreg != NULL)
		ptm_tc->tc_priv->tc_ops.dev_unreg(ptm_tc->dev,
			ptm_tc->dev->name, ptm_tc->subif_id, 0);

	/* remove PTM callback function */
	ptm_cb_setup(ptm_tc, 0);
	/* message for TC DOWN */
	tc_ntlk_msg_send(ptm_tc->tc_priv, 0, PTM_TC, TC_UNLOADED,
		(tc_mode == TC_PTM_BND_MODE) ? BONDING_MODE : NON_BONDING_MODE,
		ptm_tc->ep_id);

	tc_info(ptm_tc->tc_priv, MSG_SWITCH, "Unload PTM TC successfully!!!\n");
}

void ptm_exit(void)
{
	struct ptm_priv *priv = g_ptm_priv;

	if (!priv)
		return;
	unregister_netdev(priv->dev);
	free_netdev(priv->dev);

	g_ptm_priv = NULL;
	memset(g_ep_priv, 0, sizeof(g_ep_priv));

	pr_info("PTM TC exit!\n");
}

/**
 * PROC functions
 */
static void print_ptm_drv_mib(struct seq_file *seq,
	struct intel_tc_ptm_sl_stats *stat)
{
	seq_puts(seq, "Driver MIB:\n");

	seq_printf(seq, "RX PKTs:\t %lu\n", (unsigned long)stat->rx_pkts);
	seq_printf(seq, "RX Bytes:\t %lu\n", (unsigned long)stat->rx_bytes);
	seq_printf(seq, "RX Errs:\t %lu\n", (unsigned long)stat->rx_errs);
	seq_printf(seq, "RX Drops:\t %lu\n\n", (unsigned long)stat->rx_drops);
	seq_printf(seq, "TX PKTs:\t %lu\n", (unsigned long)stat->tx_pkts);
	seq_printf(seq, "TX Bytes:\t %lu\n", (unsigned long)stat->tx_bytes);
	seq_printf(seq, "TX Errs:\t %lu\n", (unsigned long)stat->tx_errs);
	seq_printf(seq, "TX Drops:\t %lu\n", (unsigned long)stat->tx_drops);
}

int proc_read_ptm_wanmib(struct seq_file *seq, void *v)
{
	struct ptm_ep_priv *priv;
	struct ptm_priv *ptm_tc;
	struct intel_tc_stats ptm_tc_stats;
	struct intel_tc_ptm_sl_stats *ptm_sl_stats;
	int ret;
	__u32 default_mib;
	priv = (struct ptm_ep_priv *)seq->private;
	ptm_tc = priv->ptm_tc;
	ret = ptm_tc_get_stats(ptm_tc, &ptm_tc_stats, 0);
	ptm_sl_stats = &(ptm_tc_stats.stats.ptm_tc_stats.ptm_sl_stats);
	default_mib = 0;
	seq_puts(seq, "FW MIB:\n");
	seq_puts(seq, "RX (Bearer Channels[0-1]):\n");
	seq_puts(seq, "   wrx_bc_overdrop:");
	seq_printf(seq, "%10llu %10u\n", ptm_sl_stats->wrx_bc_overdrop,
		default_mib);

	seq_puts(seq, "   wrx_bc_user_cw: ");
	seq_printf(seq, "%10llu %10u\n", ptm_sl_stats->wrx_bc_user_cw,
		default_mib);

	seq_puts(seq, "   wrx_bc_idle_cw: ");
	seq_printf(seq, "%10llu %10u\n",
		ptm_sl_stats->wrx_bc_idle_cw, default_mib);

	seq_puts(seq, "RX (Gamma Interfaces[0-3]):\n");
	seq_puts(seq, "  wrx_total_pdu:   ");
	seq_printf(seq, "%10llu %10u %10u %10u\n",
		ptm_sl_stats->wrx_gif_total_pdu,
		default_mib, default_mib, default_mib);

	seq_puts(seq, "  wrx_dropdes_pdu: ");
	seq_printf(seq, "%10llu %10u %10u %10u\n",
		ptm_sl_stats->wrx_gif_drop_pdu,
		default_mib, default_mib, default_mib);

	seq_puts(seq, "  wrx_crc_err_pdu: ");
	seq_printf(seq, "%10llu %10u %10u %10u\n",
		ptm_sl_stats->wrx_gif_crc_err_pdu,
		default_mib, default_mib, default_mib);

	seq_puts(seq, "  wrx_violated_cw: ");
	seq_printf(seq, "%10llu %10u %10u %10u\n",
		ptm_sl_stats->wrx_gif_violated_cw,
		default_mib, default_mib, default_mib);

	seq_puts(seq, "  wrx_total_bytes: ");
	seq_printf(seq, "%10llu %10u %10u %10u\n",
		ptm_sl_stats->wrx_gif_total_bytes,
		default_mib, default_mib, default_mib);

	seq_puts(seq, "TX (Bearer Channels[0-1]):\n");
	seq_printf(seq, "  total_tx_cw:     %10llu %10u\n",
		ptm_sl_stats->wtx_bc_cw, default_mib);

	seq_puts(seq, "TX (Gamma Interfaces[0-3]):\n");
	seq_puts(seq, "  tx_total_pdu:    ");
	seq_printf(seq, "%10llu %10u %10u %10u\n",
		ptm_sl_stats->wtx_gif_total_pdu,
		default_mib, default_mib, default_mib);

	seq_puts(seq, "  tx_total_bytes:  ");
	seq_printf(seq, "%10llu %10u %10u %10u\n",
		ptm_sl_stats->wtx_gif_total_bytes,
		default_mib, default_mib, default_mib);
	print_ptm_drv_mib(seq, ptm_sl_stats);

	return 0;
}

ssize_t proc_write_ptm_wanmib(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	char str[32];
	int len, rlen;
	int num;
	int i;
	u32 size, off;
	char *param_list[20];
	struct ptm_ep_priv *priv;

	unsigned int wrx_total_pdu[4] = {
		DREG_AR_AIIDLE_CNT0, DREG_AR_HEC_CNT0,
		DREG_AR_AIIDLE_CNT1, DREG_AR_HEC_CNT1};
	unsigned int wrx_crc_err_pdu[4] = {
		GIF0_RX_CRC_ERR_CNT, GIF1_RX_CRC_ERR_CNT,
		GIF2_RX_CRC_ERR_CNT, GIF3_RX_CRC_ERR_CNT};
	unsigned int wrx_cv_cw_cnt[4] = {
		GIF0_RX_CV_CNT, GIF1_RX_CV_CNT,
		GIF2_RX_CV_CNT, GIF3_RX_CV_CNT};
	unsigned int wrx_bc_overdrop_cnt[2] = {
		B0_OVERDROP_CNT, B1_OVERDROP_CNT};

	priv = (struct ptm_ep_priv *)PDE_DATA(file_inode(file));

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;

	num = vrx_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if ((vrx_strcmpi(param_list[0], "clear") == 0)
		|| (vrx_strcmpi(param_list[0], "clean") == 0)
		|| (vrx_strcmpi(param_list[0], "c") == 0)) {
		for (i = 0; i < 4; i++) {
			priv->ptm_mib.rx_total_pdu[i]
				= tc_r32(wrx_total_pdu[i]);
			priv->ptm_mib.rx_crc_err_pdu[i]
				= tc_r32(wrx_crc_err_pdu[i]);
			priv->ptm_mib.rx_cv_cw_cnt[i]
				= tc_r32(wrx_cv_cw_cnt[i]);
			off = i * DW_SZ(desq_cfg_ctxt_t) +
				offsetof(desq_cfg_ctxt_t, deq_pkt_cnt) / 4;
			priv->ptm_mib.tx_total_pdu[i]
				= sb_r32(__US_TC_LOCAL_Q_CFG_CTXT_BASE + off);
			priv->ptm_mib.tx_total_bytes[i]
				= sb_r32(__US_TC_LOCAL_Q_CFG_CTXT_BASE
					+ off + 1);

			if (i < 2) {
				priv->ptm_mib.rx_bc_overdrop_cnt[i]
					= tc_r32(wrx_bc_overdrop_cnt[i]);
				sb_w32(0, __RECEIVE_NON_IDLE_CELL_CNT_0 + i);
				sb_w32(0, __RECEIVE_IDLE_CELL_CNT_0 + i);
				sb_w32(0, __TRANSMIT_CELL_CNT_0 + i);
			}
			size = sizeof(struct wan_rx_mib_table);
			off = i * DW_SZ(struct wan_rx_mib_table);
			tc_memset(priv,
				fpi_addr(__RX_GIF_MIB_BASE + off), 0, size);
		}

		size = sizeof(qosq_mib_t);
		off = i * DW_SZ(qosq_mib_t);
		for (i = 0; i < QOSQ_NUM; i++) {
			tc_memset(priv,
				fpi_addr(__QOSQ_MIB_BASE + off), 0, size);
		}

		memset(&g_ptm_priv->stats64, 0, sizeof(g_ptm_priv->stats64));
	} else
		pr_info("echo clear/clean/c > wanmib\n");

	return count;
}

/**
 *  ps_en:  1 - Enable powersaving, 0 -Disable Powersaving
 *  flag:   0 - Get the powersaving cfg, 1 - Set the powersaving
 */
int ptm_power_saving(struct ptm_ep_priv *priv, int ps_en, int flag)
{
	struct psave_cfg ps_cfg;
	int i;

	memset(&ps_cfg, 0, sizeof(ps_cfg));

	if (!flag) {
		tc_mem_read(priv, &ps_cfg,
			fpi_addr(PSAVE_CFG(0)), sizeof(ps_cfg));
		return ps_cfg.sleep_en;
	} else {
		for (i = PP32_TX; i < PP32_MAX; i++) {
			tc_mem_read(priv, &ps_cfg,
				fpi_addr(PSAVE_CFG(i)), sizeof(ps_cfg));
			ps_cfg.sleep_en = !!(ps_en);
			tc_mem_write(priv, fpi_addr(PSAVE_CFG(i)),
				&ps_cfg, sizeof(ps_cfg));
		}
		priv->tc_priv->param.ps = ps_en;
	}

	return 0;
}

int proc_ptm_read_bond(struct seq_file *seq, void *v)
{
	struct ptm_ep_priv *priv = (struct ptm_ep_priv *)seq->private;
	bond_conf_t bconf;
	us_bg_qmap_t us_qmap;
	us_bg_gmap_t us_gmap;
	ds_bg_gmap_t ds_gmap;

	tc_mem_read(priv, &bconf, fpi_addr(__BOND_CONF), sizeof(bconf));
	seq_puts(seq, "Bonding Configuration:\n");
	seq_printf(seq, "  BOND_CONF[0x%x]:\n", __BOND_CONF);
	seq_printf(seq, "\t max_frag_size=%u, dplus_fp_fcs_en=%u\n",
		bconf.max_frag_size, bconf.dplus_fp_fcs_en);
	seq_printf(seq, "\t bg_num=%u, bond_mode=%u (%s)\n",
		bconf.bg_num, bconf.bond_mode,
		bconf.bond_mode ? "L2 Trunking" : "L1 Bonding");
	seq_printf(seq, "\t e1_bond_en=%u, d5_acc_dis=%u, d5_b1_en=%u\n",
		bconf.e1_bond_en, bconf.d5_acc_dis, bconf.d5_b1_en);

	tc_mem_read(priv, &us_qmap, fpi_addr(__US_BG_QMAP), sizeof(us_qmap));
	seq_printf(seq, "  US_BG_QMAP[0x%x]:\n", __US_BG_QMAP);
	seq_printf(seq, "\t qmap0=0x%02x, qmap1=0x%02x, qmap2=0x%02x, qmap3=0x%02x\n",
		us_qmap.queue_map0, us_qmap.queue_map1,
		us_qmap.queue_map2, us_qmap.queue_map3);

	tc_mem_read(priv, &us_gmap, fpi_addr(__US_BG_GMAP), sizeof(us_gmap));
	seq_printf(seq, "  US_BG_GMAP[0x%x]:\n", __US_BG_GMAP);
	seq_printf(seq, "\t gmap0=0x%02x, gmap1=0x%02x, gmap2=0x%02x, gmap3=0x%02x\n",
		us_gmap.gif_map0, us_gmap.gif_map1,
		us_gmap.gif_map2, us_gmap.gif_map3);

	tc_mem_read(priv, &ds_gmap, fpi_addr(__DS_BG_GMAP), sizeof(ds_gmap));
	seq_printf(seq, "  DS_BG_GMAP[0x%x]:\n", __DS_BG_GMAP);
	seq_printf(seq, "\t gmap0=0x%02x, gmap1=0x%02x, gmap2=0x%02x, gmap3=0x%02x\n",
		ds_gmap.gif_map0, ds_gmap.gif_map1,
		ds_gmap.gif_map2, ds_gmap.gif_map3);

	seq_puts(seq, "Cross Pci Debug Info:    Read \t\t Write\n");
	seq_printf(seq, "    Min clock cycles:  0x%08x, \t 0x%08x\n",
		sb_r32(__READ_MIN_CYCLES), sb_r32(__WRITE_MIN_CYCLES));
	seq_printf(seq, "    Max clock cycles:  0x%08x, \t 0x%08x\n",
		sb_r32(__READ_MAX_CYCLES), sb_r32(__WRITE_MAX_CYCLES));
	seq_printf(seq, "    Total access num:  0x%08x, \t 0x%08x\n",
		sb_r32(__READ_NUM), sb_r32(__WRITE_NUM));
	seq_printf(seq, "    Total cycle Lo:    0x%08x, \t 0x%08x\n",
		sb_r32(__TOTAL_READ_CYCLES_LO),
		sb_r32(__TOTAL_WRITE_CYCLES_LO));
	seq_printf(seq, "    Total cycle Hi:    0x%08x, \t 0x%08x\n",
		sb_r32(__TOTAL_READ_CYCLES_HI),
		sb_r32(__TOTAL_WRITE_CYCLES_HI));

	return 0;
}
int proc_ptm_read_bondmib(struct seq_file *seq, void *v)
{
	int i, ret;
	struct ptm_ep_priv *priv = (struct ptm_ep_priv *)seq->private;
	struct ptm_priv *ptm_tc;
	struct intel_tc_stats ptm_tc_stats;
	struct intel_tc_ptm_bonding_stats *stats;
	ptm_tc = priv->ptm_tc;
	ret = ptm_tc_get_stats(ptm_tc, &ptm_tc_stats, 1);
	stats = &(ptm_tc_stats.stats.ptm_tc_stats.pmt_bonding_stats);


	seq_puts(seq, "US_BONDING_GIF_MIB:\n");
	for (i = 0; i < BOND_GIF_NUM; i++)
		seq_printf(seq, "    GIF[%d]:  0x%08x\n",
			i, stats->us_gif_mib[i]);

	for (i = 0; i < BOND_GIF_NUM; i++) {
		seq_printf(seq, "DS_BONDING_GIF_MIB[%d]:\n", i);
		seq_printf(seq, "\t\t Total RX FRAG CNT: 0x%08x, Total RX BYTE CNT: 0x%08x\n",
			stats->ds_gif_mib[i].rx_frag_byte_cnt,
			stats->ds_gif_mib[i].rx_byte_cnt);
		seq_printf(seq, "\t\t Overflow FRAG CNT: 0x%08x, Overflow BYTE CNT: 0x%08x\n",
			stats->ds_gif_mib[i].rx_of_frag_byte_cnt,
			stats->ds_gif_mib[i].rx_of_byte_cnt);
		seq_printf(seq, "\t\t OOR FRAG CNT: 0x%08x, Missing FRAG CNT: 0x%08x, TO FRAG CNT:0x%08x\n",
			stats->ds_gif_mib[i].rx_oor_frag_byte_cnt,
			stats->ds_gif_mib[i].rx_miss_frag_byte_cnt,
			stats->ds_gif_mib[i].rx_to_frag_byte_cnt);
	}

	for (i = 0; i < DS_BG_NUM; i++) {
		seq_printf(seq, "DS_BG_MIB[%d]:\n", i);
		seq_printf(seq, "    Confirm PKT CNT: 0x%08x, Confirm FRAG CNT: 0x%08x, Confirm Byte CNT: 0x%08x\n",
			stats->ds_bg_mib[i].conform_pkt_cnt,
			stats->ds_bg_mib[i].conform_frag_cnt,
			stats->ds_bg_mib[i].conform_byte_cnt);
		seq_printf(seq, "    NO SOP PKT CNT: 0x%08x, NO SOP FRAG CNT: 0x%08x, NO SOP Byte CNT: 0x%08x\n",
			stats->ds_bg_mib[i].no_sop_pkt_cnt,
			stats->ds_bg_mib[i].no_sop_frag_cnt,
			stats->ds_bg_mib[i].no_sop_byte_cnt);
		seq_printf(seq, "    NO EOP PKT CNT: 0x%08x, NO EOP FRAG CNT: 0x%08x, NO EOP Byte CNT: 0x%08x\n",
			stats->ds_bg_mib[i].no_eop_pkt_cnt,
			stats->ds_bg_mib[i].no_eop_frag_cnt,
			stats->ds_bg_mib[i].no_eop_byte_cnt);
		seq_printf(seq, "    Oversize PKT CNT: 0x%08x, Oversize FRAG CNT: 0x%08x, Oversize Byte CNT: 0x%08x\n",
			stats->ds_bg_mib[i].oversize_pkt_cnt,
			stats->ds_bg_mib[i].oversize_frag_cnt,
			stats->ds_bg_mib[i].oversize_byte_cnt);
		seq_printf(seq, "    Noncosec PKT CNT: 0x%08x, Noncosec FRAG CNT: 0x%08x, Noncosec Byte CNT: 0x%08x\n",
			stats->ds_bg_mib[i].noncosec_pkt_cnt,
			stats->ds_bg_mib[i].noncosec_frag_cnt,
			stats->ds_bg_mib[i].noncosec_byte_cnt);
	}

	return ret;
}

ssize_t proc_ptm_write_bondmib(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	struct ptm_ep_priv *priv;
	char local_buf[128];
	int len;
	int num;
	int i;
	char *param_list[8];

	len = sizeof(local_buf) < count ? sizeof(local_buf) - 1 : count;
	len = len - copy_from_user(local_buf, buf, len);
	local_buf[len] = 0;

	num = vrx_split_buffer(local_buf, param_list, ARRAY_SIZE(param_list));
	priv = (struct ptm_ep_priv *)PDE_DATA(file_inode(file));
	if (priv == NULL) {
		pr_err("%s: Invalid private data\n", __func__);
		return -EINVAL;
	}

	if (vrx_strcmpi(param_list[0], "clear") == 0
		|| vrx_strcmpi(param_list[0], "clean") == 0
		|| vrx_strcmpi(param_list[0], "c") == 0) {
		for (i = 0; i < BOND_GIF_NUM; i++) {
			tc_memset(priv, US_BOND_GIF_MIB(i), 0, 4);
			tc_memset(priv, DS_BOND_GIF_MIB(i), 0,
					sizeof(ds_bond_gif_mib_t));
		}

		for (i = 0; i < DS_BG_NUM; i++)
			tc_memset(priv, DS_BG_MIB(i), 0, sizeof(ds_bg_mib_t));
	} else
		pr_info("echo clear/clean/c > bondmib\n");

	return count;
}

void ptm_set_q_preempt(struct ptm_ep_priv *priv, u32 qid, u32 preempt)
{
	u32 val;

	if (!is_ptm_sl(priv) && !is_us_bond(priv)) {
		pr_err("set queue preemption only supported in PTM Single line mode or Bonding US Master!\n");
		return;
	}

	val = sb_r32(_QID2PREEMP_MAP);
	if (preempt)
		val |= BIT(qid);
	else
		val &= ~(BIT(qid));

	sb_w32(val, _QID2PREEMP_MAP);

	return;
}

/**
 * Q0 ---> preemption queue---->OUTQ 0
 * Q1----> non preemption queue---->OUTQ 1
 */
u32 ptm_get_q_preempt(struct ptm_ep_priv *priv, u32 qid)
{
	u32 val;

	val = sb_r32(_QID2PREEMP_MAP);

	return !!(BIT(qid) & val);
}


