/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (C) 2015 Zhu YiXin<yixin.zhu@lantiq.com>
 */

#ifndef __VRX318_PTM_TC_H__
#define __VRX318_PTM_TC_H__

#define DS_FRAG_DES_LIST1_LEN		32
#define DS_FRAG_DES_LIST1_LEN_MAX	48
#define DS_FRAG_DES_LIST2_LEN		32
#define DS_FRAG_DES_LIST2_LEN_MAX	48
#define DS_BOND_GIF_LL_DES_LEN		256
#define BOND_SOC_SYNC_US_BUF_SIZE	(512 * 4) /* 512 DWORDS */
#define BOND_SOC_SYNC_DS_BUF_SIZE	(256 * 4) /* 256 DWORDS */

#define OUTQ_NUM		64
#define MAX_FRAGQ_NUM		8

#define QOSQ_ID_MASK		((QOSQ_NUM - 1) | ((QOSQ_NUM - 1) >> 1) | \
				((QOSQ_NUM - 1) >> 2) | ((QOSQ_NUM - 1) >> 3))
#define QOSQ_PORT_SSID		16 /* PORT SHAPER STARD ID */
#define QOSQ_L3_SHAPER_ID	20 /* All the outqss share one L3 shaper */

#define US_FASTPATH_DES_OWN	0
#define US_CPUPATH_DES_OWN	1
#define US_OUTQ_DES_OWN		0
#define US_FRAGQ_DES_OWN	1
#define US_LOCALQ_DES_OWN	1
#define DS_FRAGQ_DES_OWN	0
#define DS_PKT_DES_OWN		0

#define PTM_PRIO_Q_NUM		8
#define PTM_DESC_QID(qid) (((qid) & 0xF) << 3)
#define CW_PAGE_SZ		65

struct vrx318_priv;
struct vrx318_ptm_bnd {
	dma_addr_t soc_peer_phybase; /* required if p2p disable */
	unsigned long soc_peer_membase;
};

struct vrx318_ptm_hw_mib {
	unsigned int rx_total_pdu[4];
	unsigned int rx_crc_err_pdu[4];
	unsigned int rx_cv_cw_cnt[4];
	unsigned int rx_bc_overdrop_cnt[2];
	unsigned int tx_total_pdu[4];
	unsigned int tx_total_bytes[4];
};

struct vrx318_ptm_tc {
	struct vrx318_priv *frwk;
	int ep_id;
	enum dsl_tc_mode tc_mode;
	struct vrx318_ptm_bnd ptm_bnd_cfg;
	struct fw_ver_id fw_ver[MAX_VRX318_NUM][2];
	struct proc_dir_entry *proc_dir;
	struct net_device *dev;
	dp_cb_t	ptm_cb;
	spinlock_t ptm_lock;
	struct rtnl_link_stats64 stats;
	struct vrx318_ptm_hw_mib ptm_mib[MAX_VRX318_NUM];
	int subif_id;
	int ptm_prio_queue_map[PTM_PRIO_Q_NUM];
	struct completion comp;
};

extern int ptm_in_showtime(void);
extern int ptm_power_saving(struct vrx318_ptm_tc *, int, int);
extern int ptm_qos_qnum(void);
extern u32 ptm_qos_mib_base(void);

#endif /* __VRX318_PTM_TC_H__ */

