/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (C) 2015 Zhu YiXin<yixin.zhu@lantiq.com>
 */

#ifndef __VRX318_ATM_TC_H__
#define __VRX318_ATM_TC_H__

#include <linux/atm.h>

/* ATM Info */
#define CELL_SIZE		ATM_AAL0_SDU
#define ATM_PORT_NUMBER		2
#define ATM_PVC_NUMBER		15
#define QSB_QUEUE_NUMBER_BASE	1
#define DEFAULT_RX_HUNT_BITTH	4
#define DEFAULT_CELL_RATE	3200
#define ATM_PRIO_Q_NUM		8

#define ATM_PDBRAM_US_BUF_LEN	16
#define ATM_PDBRAM_DS_OAM_LEN	10
#define ATM_PDBRAM_DS_BUF_LEN	12
#define ATM_SB_DS_BUF_LEN	12

/* OAM Definitions */
#define OAM_HTU_ENTRY_NUMBER	3
#define OAM_F4_SEG_HTU_ENTRY	0
#define OAM_F4_TOT_HTU_ENTRY	1
#define OAM_F5_HTU_ENTRY	2
#define OAM_F4_CELL_ID		0
#define OAM_F5_CELL_ID		15

#define HTU_ENTRY_TBL(x)	(__HTU_ENTRY_TABLE + (x)) /* x < 24 */
#define HTU_MASK_TBL(x)		(__HTU_MASK_TABLE + (x)) /* x < 24 */
#define HTU_RESULT_TBL(x)	(__HTU_RESULT_TABLE + (x)) /* x < 24 */

#define DS_OAM_DES_NUM          32 /* MAX 32 */
#define ATM_OAM_SIZE		64

/* align is required because buffer may allocated in pdb ram */
#define ATM_TXQ_NUM(qos_en)	((qos_en) ? (QOSQ_NUM / (qos_en)) : QOSQ_NUM)
#define ATM_PVC_NUM(qos_en) 	((qos_en) ? (QOSQ_NUM / (qos_en)) : ATM_PVC_NUMBER)
#define DESC_QID(qid)		(((qid) & 0xF) << 3)
/* Pass Through (No AAL5 encapsulation required) */
#define DESC_MPOA_PT(pt)	(((pt) & 0x1) << 2)
#define DESC_MPOA_MD(mode)	((mode) & 0x3) /* MPoA Type */
#define ATM_DESC_SUBIF_ID(qid, pt, mode) \
		(DESC_QID(qid) | DESC_MPOA_PT(pt) | DESC_MPOA_MD(mode))
#define SUBIF_ID_GET(id)	(((id) & 0xF00) >> 8)

#define WRX_Q_CFG(i)		(__WRX_QUEUE_CONFIG + (i) * 10) /* i < 16 */
#define WTX_Q_CFG(i)		(__WTX_QUEUE_CONFIG + (i) * 25) /* i < 16 */

/* QSB Related Registers */
#define QSB_CONF_REG(addr)	QSB_CONF_REG_ADDR(addr)

/* QSB Internal Cell Delay Variation Register */
#define QSB_ICDV		QSB_CONF_REG(0x0007)

/* QSB Scheduler Burst Limit Register */
#define QSB_SBL			QSB_CONF_REG(0x0009)

/* QSB Configuration Register */
#define QSB_CFG			QSB_CONF_REG(0x000A)

/* QSB RAM Transfer Table Register */
#define QSB_RTM			QSB_CONF_REG(0x000B)

/* QSB RAM Transfer Data Register */
#define QSB_RTD			QSB_CONF_REG(0x000C)

/* QSB RAM Access Register */
#define QSB_RAMAC		QSB_CONF_REG(0x000D)

/* QSB Queue Scheduling and Shaping Definitions */
#define QSB_WFQ_NONUBR_MAX	0x3f00
#define QSB_WFQ_UBR_BYPASS	0x3fff
#define QSB_TP_TS_MAX		65472
#define QSB_TAUS_MAX		64512
#define QSB_GCR_MIN		18

/* QSB Command Set */
#define QSB_RAMAC_RW_READ	0
#define QSB_RAMAC_RW_WRITE	1

#define QSB_RAMAC_TSEL_QPT	0x01
#define QSB_RAMAC_TSEL_SCT	0x02
#define QSB_RAMAC_TSEL_SPT	0x03
#define QSB_RAMAC_TSEL_VBR	0x08

#define QSB_RAMAC_LH_LOW	0
#define QSB_RAMAC_LH_HIGH	1

#define QSB_QPT_SET_MASK	0x0
#define QSB_QVPT_SET_MASK	0x0
#define QSB_SET_SCT_MASK	0x0
#define QSB_SET_SPT_MASK	0x0
#define QSB_SET_SPT_SBVALID_MASK 0x7FFFFFFF

#define QSB_RAMAC_RW_SET(x)	(((x) & 0x1) << 31)
#define QSB_RAMAC_TSEL_SET(x)	(((x) & 0xF) << 24)
#define QSB_RAMAC_LH_SET(x)	(((x) & 0x1) << 16)
#define QSB_RAMAC_TESEL_SET(x)	((x) & 0x3FF)

#define QSB_SPT_SBV_VALID	(1 << 31)
#define QSB_SPT_PN_SET(x)	(((x) & 0x01) << 16)
#define QSB_SPT_INTRATE_SET(x)	((x) & 0x3FFF)

#define QSB_RTD_TTV_SET(x)	((x) & 0xFFFFFFFF)

/* DSL MIB */
#define DSL_WAN_MIB_TBL		0x4EF0
#define DSL_Q_RX_MIB_TBL(i)	(__WRX_VC_MIB_BASE + (i) * 2) /* i < 16 */
#define DSL_Q_TX_MIB_TBL(i)	(__WTX_VC_MIB_BASE + (i) * 2) /* i < 16 */


struct vrx318_priv;

struct htu_entry {
	unsigned int res1:2;
	unsigned int pid:2;
	unsigned int vpi:8;
	unsigned int vci:16;
	unsigned int pti:3;
	unsigned int vld:1;
};

struct htu_mask {
	unsigned int set:2;
	unsigned int pid_mask:2;
	unsigned int vpi_mask:8;
	unsigned int vci_mask:16;
	unsigned int pti_mask:3;
	unsigned int clear:1;
};

struct htu_result {
	unsigned int res1:12;
	unsigned int cellid:4;
	unsigned int res2:5;
	unsigned int type:1;
	unsigned int ven:1;
	unsigned int res3:5;
	unsigned int qid:4;
};

struct dsl_wan_mib_table {
	unsigned int res1;
	unsigned int wrx_drophtu_cell;
	unsigned int wrx_dropdes_pdu;
	unsigned int wrx_correct_pdu;
	unsigned int wrx_err_pdu;
	unsigned int wrx_dropdes_cell;
	unsigned int wrx_correct_cell;
	unsigned int wrx_err_cell;
	unsigned int wrx_total_byte;
	unsigned int res2;
	unsigned int wtx_total_pdu;
	unsigned int wtx_total_cell;
	unsigned int wtx_total_byte;
	unsigned int res3[3];
};

struct dsl_queue_mib {
	unsigned int pdu;
	unsigned int bytes;
};

struct dsl_queue_drop_mib {
	unsigned int pdu;
};

enum {
	MPOA_TYPE_EOA_WO_FCS	= 0,
	MPOA_TYPE_EOA_W_FCS	= 1,
	MPOA_TYPE_PPPOA		= 2,
	MPOA_TYPE_IPOA		= 3,
};

enum {
	MPOA_MODE_VCMUX	= 0,
	MPOA_MODE_LLC	= 1,
};

/* QSB Queue Parameter Table Entry and Queue VBR Parameter Table Entry */
union qsb_queue_parameter_table {
	struct {
		unsigned int res1:1;
		unsigned int vbr:1;
		unsigned int wfqf:14;
		unsigned int tp:16;
	} bit;
	unsigned int dword;
};

union qsb_queue_vbr_parameter_table {
	struct {
		unsigned int taus:16;
		unsigned int ts:16;
	} bit;
	unsigned int dword;
};

enum mpoa_type {
	MOPA_EOA_WO_FCS	= 0,
	MPOA_EOA_W_FCS	= 1,
	MPOA_PPPOA	= 2,
	MPOA_IPOA	= 3
};

enum mpoa_mode {
	MPOA_VCMUX_MODE	= 0,
	MPOA_LLC_MODE	= 1
};

enum ip_ver {
	ATM_IP_IPV4 = 0,
	ATM_IP_IPV6 = 1
};

struct atm_oam_llst {
	u32 *oam_des_list;
	u32 oam_num;
	u32 oam_idx;
};

struct atm_stats {
	unsigned int aal5_rx_pkts;
	unsigned int aal5_rx_bytes;
	unsigned int aal5_rx_errors;
	unsigned int aal5_rx_dropped;

	unsigned int oam_rx_pkts;
	unsigned int oam_rx_bytes;
	unsigned int oam_rx_errors;
	unsigned int oam_rx_dropped;

	unsigned int aal5_tx_pkts;
	unsigned int aal5_tx_bytes;
	unsigned int aal5_tx_errors;
	unsigned int aal5_tx_dropped;

	unsigned int oam_tx_pkts;
	unsigned int oam_tx_bytes;
	unsigned int oam_tx_errors;
	unsigned int oam_tx_dropped;
};

struct uni_cell_header {
	unsigned int gfc:4;
	unsigned int vpi:8;
	unsigned int vci:16;
	unsigned int pti:3;
	unsigned int clp:1;
};

struct atm_port {
	struct atm_dev *dev;
	unsigned int tx_max_cell_rate; /* maximum cell rate */
	unsigned int tx_used_cell_rate; /* currently used cell rate */
};

struct atm_pvc {
	struct atm_vcc *vcc; /* opened VCC */
	struct net_device *dev; /* net device associated with atm VCC */
	struct timespec	access_time; /* time when last user cell arrived */
	int prio_queue_map[ATM_PRIO_Q_NUM];
	unsigned int prio_tx_packets[ATM_PRIO_Q_NUM];
	struct atm_stats stats;
	unsigned int port; /* to which port the connection belongs */
	unsigned int sw_txq_tbl; /* Software TX Q used for this connection */
	int subif_id;
	int mpoa_type;
	int mpoa_mode;
};

struct vrx318_atm_tc {
	struct vrx318_priv *frwk;
	int ep_id;
	struct atm_port	port[ATM_PORT_NUMBER];
	struct atm_pvc	conn[ATM_PVC_NUMBER]; /*conn id is same as subif id */
	u32 pvc_tbl; /* PVC opened status, one for one connection*/
	u32 sw_txq_tbl; /*including pvc and its sw tx queue */
	spinlock_t atm_lock;
	spinlock_t oam_lock;
	struct fw_ver_id fw_ver;
	struct proc_dir_entry *proc_dir;
	struct atm_oam_llst oam_llst;
	dp_cb_t	atm_cb;
	struct atm_stats stats;
	struct completion comp;
};

extern void (*atm_hook_mpoa_setup)(struct atm_vcc *, int,
				int, struct net_device *);
#if defined(CONFIG_LTQ_OAM) || defined(CONFIG_LTQ_OAM_MODULE)
extern void ifx_push_oam(unsigned char *);
#endif

extern int atm_in_showtime(void);
extern int proc_read_queue(struct seq_file *, void *);
extern int proc_read_htu(struct seq_file *, void *);
extern int sw_tx_queue_add(struct vrx318_atm_tc *, int);
extern int sw_tx_queue_del(struct vrx318_atm_tc *, int);
extern int find_vpivci(unsigned int, unsigned int);
extern void print_drv_mib(struct seq_file *, struct vrx318_atm_tc *);
extern void print_stat_mib(struct seq_file *, struct atm_stats *);
extern int atm_power_saving(struct vrx318_atm_tc *, int, int);

#endif /* __VRX318_ATM_TC_H__ */

