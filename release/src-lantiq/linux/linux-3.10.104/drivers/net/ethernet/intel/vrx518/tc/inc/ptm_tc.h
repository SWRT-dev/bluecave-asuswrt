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
#ifndef __PTM_TC_H__
#define __PTM_TC_H__

#define OUTQ_DESC_PER_Q		32
#define OUTQ_PNUM		2
#define OUTQ_DESC_NUM		(OUTQ_DESC_PER_Q * OUTQ_PNUM)
#define MAX_FRAGQ_NUM		1
#define BOND_DESC_SZ		1 /* DW Size, struct not open to driver */

#define QOSQ_ID_MASK		((QOSQ_NUM - 1) | ((QOSQ_NUM - 1) >> 1) | \
				((QOSQ_NUM - 1) >> 2) | ((QOSQ_NUM - 1) >> 3))
#define QOSQ_PORT_SSID		16 /* PORT SHAPER STARD ID */
#define QOSQ_L3_SHAPER_ID	20 /* All the outqss share one L3 shaper */

#define US_FP_DES_OWN		1
#define US_OUTQ_DES_OWN		1
#define US_FRAGQ_DES_OWN	1
#define US_LOCALQ_DES_OWN	0
#define DS_LOCALQ_DES_OWN	1
#define DS_FRAGQ_DES_OWN	0
#define DS_PKT_DES_OWN		0
#define ERB_DES_PPE_OWN		1
#define ERB_FRAG_DES_OWN	1

#define PTM_PRIO_Q_NUM		8
#define PTM_DESC_QID(qid)	(((qid) & 0xF) << 3)
#define US_BG_NUM		4
#define DS_BG_NUM		4
#define BOND_LL_NUM		9
#define BOND_LL_DES_NUM		1536
#define BOND_GIF_NUM		8
#define DEF_BOND_FRAG_SIZE	512
#define QOS_DES_NUM		510
#define ERB_PKT_SIZE		1056

/* SB ADDRESS */
#define TX_CTRL_K_TABLE(i)		fpi_addr(__CTRL_K_TBL_BASE + (i))
#define DS_FRAGQ_DES_LIST_BASE(off, i)		\
	fpi_addr(__DS_FRAGQ_DES_LIST_BASE + (off) + ((i) << 1))
#define DS_BOND_GIF_LL_DES_BA(i)		\
	fpi_addr(__DS_BOND_GIF_LL_DES_BA + ((i) << 1))
#define DES_SYNC_CFG_CTXT(idx)			\
	fpi_addr(__DES_SYNC_CFG_CTXT + ((idx) * 32))
#define BOND_US_DES_SYNC_CFG_CTXT(i)		\
	fpi_addr(__BOND_US_DES_SYNC_CFG_CTXT + ((i) * 8))
#define INQ_QOS_CFG_BASE(off)		fpi_addr(__INQ_QOS_CFG_BASE + (off))
#define PSAVE_CFG(x)			(__PSAVE_CFG_TX + (x))
#define US_BOND_GIF_MIB(i)		fpi_addr(__US_E1_FRAG_Q_FRAG_MIB + (i))
#define DS_BOND_GIF_MIB(i)		fpi_addr(__DS_BOND_GIF_MIB + ((i) << 4))
#define DS_BG_MIB(i)			fpi_addr(__DS_BG_MIB + ((i) << 2))
#define CW_PAGE_SZ			65
#define FFSM_PNUM			0xA0
#define SFSM_PNUM			0xF0
#define SFSM_DBACE			0x6000
#define SFSM_CBACE			0x7100

enum {
	US_DMA_PRE_RXCH = 0,
	US_DMA_PRE_TXCH,
	US_DMA_NOPRE_RXCH,
	US_DMA_NOPRE_TXCH,
	DS_DMA_RXCH,
	DS_DMA_TXCH
};

enum {
	PDBRAM_US,
	PDBRAM_DS,
	PDBRAM_ACA_TXOUT,
	PDBRAM_ACA_RXOUT,
	PDBRAM_ERB,
};

struct ptm_hw_mib {
	unsigned int rx_total_pdu[4];
	unsigned int rx_crc_err_pdu[4];
	unsigned int rx_cv_cw_cnt[4];
	unsigned int rx_bc_overdrop_cnt[2];
	unsigned int tx_total_pdu[4];
	unsigned int tx_total_bytes[4];
};

/* First three parameter should be same as tc_comm structure */
struct ptm_ep_priv {
	struct dc_ep_dev *ep;
	struct tc_priv *tc_priv;
	u32 ep_id;
	struct ptm_priv *ptm_tc;
	struct ptm_hw_mib ptm_mib;
	struct proc_dir_entry *proc_dir;
	u32 dfe_rate;
};

struct ptm_priv {
	struct tc_priv *tc_priv;
	u32 ep_id;
	struct ppe_fw fw;
	struct net_device *dev;
	spinlock_t ptm_lock;
	struct rtnl_link_stats64 stats64;
	int subif_id;
	u32 prio_q_map[PTM_PRIO_Q_NUM];
	u32 outq_map[OUTQ_PNUM];
	struct completion comp;
};

enum {
	US_BOND = 0,
	DS_BOND = 1,
	BOND_MAX,
};

extern int proc_read_ptm_wanmib(struct seq_file *, void *);
extern ssize_t proc_write_ptm_wanmib(struct file *, const char __user *,
				size_t, loff_t *);
extern int ptm_power_saving(struct ptm_ep_priv *, int, int);
extern int proc_ptm_read_bond(struct seq_file *, void *);
extern int proc_ptm_read_bondmib(struct seq_file *, void *);
extern ssize_t proc_ptm_write_bondmib(struct file *, const char __user *,
				size_t, loff_t *);
extern void ptm_set_q_preempt(struct ptm_ep_priv *, u32, u32);
extern u32 ptm_get_q_preempt(struct ptm_ep_priv *, u32);



#endif /* __PTM_TC_H__ */
