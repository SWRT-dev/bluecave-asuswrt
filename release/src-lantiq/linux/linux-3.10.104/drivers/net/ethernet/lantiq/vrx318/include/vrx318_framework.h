/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2015 Zhu YiXin<yixin.zhu@lantiq.com>
 */

#ifndef __VRX318_FRAMEWORK_H__
#define __VRX318_FRAMEWORK_H__
#include <linux/klogging.h>


enum ep_hw_type { /* Used for Future Product */
	HW_TYPE_VRX318,
	HW_TYPE_MAX,
};

enum vrx318_status {
	VRX318_RUNNING = 0, /* TC loaded succesfully */
	VRX318_INIT, /* in Initialization */
	VRX318_NO_TC, /* Init done, but NO TC loaded */
	VRX318_SWITCHING, /* TC is swiching */
	VRX318_EXIT, /* module is going to be removed */
};

enum dsl_tc_mode {
	TC_ATM_SL_MODE = 0,
	TC_PTM_SL_MODE = 1, /*PTM Single line mode */
	TC_PTM_BND_MODE = 2, /* PTM Bonding mode */
	TC_NONE_MODE,
};

struct vrx318_param {
	unsigned int p2p_en:1;
	unsigned int cdma_wr_en:1;
	unsigned int lle_in_sb:1;
	unsigned int pmac_en:1;
	unsigned int dfe_loopback:1;
	unsigned int atm_qos:3;
	unsigned int res0:24;

	int q_gamma_map[4];
	int qsb_tau;
	int qsb_srvm;
	int qsb_tstep;
	int aal5r_max_pktsz;
	int aal5r_min_pktsz;
	int aal5s_max_pktsz;
	int oam_prio;
};

struct tc_req {
	struct work_struct work;
	u32 id;
	enum dsl_tc_mode tc_mode;
};

/* Please note, any HW specific/related paramter must be accessed via MACRO.
to make it easy to add new product in future.
*/
struct vrx318_priv {
	struct pcie_ep_dev ep_dev[MAX_VRX318_NUM]; /* EP dev */
	int ep_num;
	int show_time_stat[MAX_VRX318_NUM];
	enum vrx318_status tc_stat;
	enum dsl_tc_mode tc_mode[MAX_VRX318_NUM];
	enum ep_hw_type hw_type;
	/* u32 msg_enable; */
	struct device *pdev;
	struct module *owner;
	struct proc_dir_entry *proc_dir;
	struct vrx318_soc_cfg soc_cfg;
	int port_id; /* PMAC port ID */
	dp_cb_t	cb; /* Datapath library callback function set */
	void *tc_priv;
	struct vrx318_param param;
	struct tc_req tc_wkq;
	spinlock_t fw_lock; /* Framework lock */
};

/* VRX318 back_pointer structure from ATM TC or PTM TC*/
struct vrx318_tc {
	struct vrx318_priv *frwk;
	int ep_id;
};

/* Debug Level */
#define DBG_ERR		BIT(0)
#define DBG_PKT_RX	BIT(1)
#define DBG_PKT_TX	BIT(2)
#define DBG_EVENT	BIT(3)
#define DBG_PKT_RX_DUMP	BIT(4)
#define DBG_PKT_TX_DUMP	BIT(5)
#define DBG_INIT	BIT(6)
#define DBG_INFO	BIT(7)
#define DBG_OAM_RX	BIT(8)
#define DBG_OAM_TX	BIT(9)
#define DBG_OAM_RX_DUMP	BIT(10)
#define DBG_OAM_TX_DUMP	BIT(11)
#define DBG_QOS		BIT(12)
#define DBG_TC_SWITCH	BIT(13)
#define DBG_LOOPBACK	BIT(14)
#define DBG_COC		BIT(15)

extern u32 g_vrx318_dbg;

#define vrx318_dbg(dbg_level, fmt, arg...) \
	do { \
		if ((g_vrx318_dbg & dbg_level)) { \
			if (dbg_level & DBG_ERR) { \
				LOGF_KLOG_ERR_RATELIMITED(fmt, ##arg); \
			} else if ((dbg_level & DBG_INFO) || \
					(dbg_level & DBG_INIT)) { \
				LOGF_KLOG_INFO_RATELIMITED(fmt, ##arg); \
			} else { \
				LOGF_KLOG_DEBUG_RATELIMITED(fmt, ##arg); \
			} \
		} \
	} \
	while (0)

#define VRX318_DEF_MSG_EN	(DBG_ERR | DBG_INIT | DBG_INFO \
				| DBG_EVENT | DBG_TC_SWITCH)


extern void vrx318_cb_setup(void *, void *, void *, void *, void *);
extern int vrx318_unload_atm_tc(struct vrx318_priv *, int);
extern int max_pvc_num(u32);
extern void vrx318_atm_param_setup(struct vrx318_priv *, int, int,
		int, int, int, int);
extern int vrx318_atm_tc_init(struct vrx318_priv *, int, enum dsl_tc_mode);
extern int vrx318_unload_ptm_tc(struct vrx318_priv *, int, enum dsl_tc_mode);
extern int vrx318_ptm_tc_init(struct vrx318_priv *, int, enum dsl_tc_mode);
extern int vrx318_unload_tc(struct vrx318_priv *, int);
extern int vrx318_load_tc(struct vrx318_priv *, int, enum dsl_tc_mode);
extern void vrx318_ptm_param_setup(struct vrx318_priv *, int *);
extern void get_vrx318_drv_ver(char **, char **);
extern void vrx318_atm_exit(void);
extern void vrx318_ptm_exit(void);

#endif /* __VRX318_FRAMEWORK_H__ */

