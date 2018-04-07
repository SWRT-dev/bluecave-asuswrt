/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (C) 2015 Zhu YiXin<yixin.zhu@lantiq.com>
 * HISTORY
 * $Date		$Author		$Version	$Comment
 * 01/02/2015	Zhu YiXin		1.0.0	Basic PTM datapath & TC driver
 * 13/02/2015	Zhu YiXin		1.0.1
 *		1. FW(Ver: 03.02.01)  add supporting unaligned data address
 *		2. Driver support built as module
 * 06/03/2015	Zhu YiXin		1.0.2
 *		1. Driver fix smp_proccess_id()
 *		called in Preemption enabled state.
 *		2. FW(Ver: 03.02.02) Fixed isse: data corruption in ARP packet
 * 06/03/2015	Zhu YiXin		1.0.3
 *		1. Fix PTM driver enabled unnecessary per-packet interrupt.
 * 29/04/2015      Zhu YiXin		2.0.0
 *		1. Unified ATM/PTM driver. Support switch ATM/PTM/Bonding
 *		TC layer without unload/load the driver module.
 * 29/04/2015      Eswaran Rekha	2.0.1
 *		1. Add Dedicated proc file to manage all the proc function for
 *		framework/ATM/PTM/Bonding
 *		2. Improve proc write function by using standard parse function
 *		from Datapath libarary.
 * 25/05/2015      Eswaran Rekha	2.0.2
 *		1. Add TC Status Genetlink Notification feature
 * 26/10/2015      Ho Nghia Duc		2.0.4
 *		1. CoC Support
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
#include <linux/seq_file.h>
#include <linux/printk.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <net/datapath_api.h>
#include <net/lantiq_cbm.h>
#include <net/lantiq_cbm_api.h>
#include <net/ppa_stack_al.h>

#include <lantiq.h>
#include <lantiq_soc.h>
#include <lantiq_irq.h>
#include <lantiq_pcie.h>
#include <lantiq_ptm.h>

#include "include/vrx318_fw_prereq.h"
#include "include/vrx318_common.h"
#include "include/vrx318_framework.h"
#include "include/vrx318_proc.h"
#include "include/vrx318_api.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yixin.zhu@lantiq.com");
MODULE_DESCRIPTION("LTQ CPE SmartPHY PCIe EP ATM/PTM driver");
MODULE_SUPPORTED_DEVICE("LTQ CPE SmartPHY PCIe EP for Devices GRX35X, GRX5XX");

#define DRV_MODULE_NAME	"VRX318_DRIVER"
#define DRV_MODULE_VERSION "2.0.8"
static int queue_gamma_map[4] = {0x00FF, 0x0000, 0x0000, 0x0000};
static int p2p_en;
static int loopback_en;
static int qos_en;
static int oam_prio;
static int qsb_tau = 1;	/*  QSB cell delay variation due to concurrency */
static int qsb_srvm = 0x0F; /*  QSB scheduler burst length */
static int qsb_tstep = 4; /*  QSB time step, all legal values are 1, 2, 4 */
static int aal5r_max_pktsz = 0x0686; /*  Max frame size for RX */
static int aal5r_min_pktsz = 0x0000; /*  Min frame size for RX */
static int aal5s_max_pktsz = 0x0686; /*  Max frame size for TX */

module_param_array(queue_gamma_map, int, NULL, 0444);
MODULE_PARM_DESC(queue_gamma_map, "TX QoS queues mapping to 4 TX Gamma interfaces.");
module_param(oam_prio, int, 0444);
MODULE_PARM_DESC(oam_prio, "OAM Priority (0 to max queue number id (7/15)).");
module_param(qos_en, int, 0444);
MODULE_PARM_DESC(qos_en, "QoS queue divider. (ATM: 0/1/2, PTM: 0/1/2)");
module_param(p2p_en, int, 0444);
MODULE_PARM_DESC(p2p_en, "PTM Bonding directly access Peer Enable flag.");
module_param(loopback_en, int, 0444);
MODULE_PARM_DESC(loopback_en, "Enable-1/Disable-0 the DFE loopback mode.");
module_param(qsb_tau, int, 0444);
MODULE_PARM_DESC(qsb_tau, "Cell delay variation. Value must be > 0");
module_param(qsb_srvm, int, 0444);
MODULE_PARM_DESC(qsb_srvm, "Maximum burst size.");
module_param(qsb_tstep, int, 0444);
MODULE_PARM_DESC(qsb_tstep, "n*32 cycles per sbs cycles n=1,2,4");
module_param(aal5r_max_pktsz, int, 0444);
MODULE_PARM_DESC(aal5r_max_pktsz, "Max packet size in byte for downstream AAL5 frames");
module_param(aal5r_min_pktsz, int, 0444);
MODULE_PARM_DESC(aal5r_min_pktsz, "Min packet size in byte for downstream AAL5 frames");
module_param(aal5s_max_pktsz, int, 0444);
MODULE_PARM_DESC(aal5s_max_pktsz, "Max packet size in byte for upstream AAL5 frames");

static struct vrx318_priv *g_vrx318_priv;
u32 g_vrx318_dbg = VRX318_DEF_MSG_EN;

static dp_rx_fn_t vrx318_rx_fn;
static dp_stop_tx_fn_t vrx318_stop_fn;
static dp_restart_tx_fn_t vrx318_restart_fn;
static dp_get_netif_subifid_fn_t vrx318_get_subifid_fn;
static dp_coc_confirm_stat vrx318_coc_confirm_stat_fn;
#ifndef MODULE

#define MAX_OPT_NUM 1
static int __init vrx318_opt_setup(char *line)
{
	int ints[MAX_OPT_NUM];

	line = get_options(line, MAX_OPT_NUM, ints);

	if (ints[0] >= 1)
		loopback_en = ints[0];

	return 0;
}

__setup("vrx318_opt=", vrx318_opt_setup);
#endif

void get_vrx318_drv_ver(char **name, char **ver)
{
	*name = DRV_MODULE_NAME;
	*ver = DRV_MODULE_VERSION;
}

void vrx318_cb_setup(void *rx_fn, void *stop_fn,
	void *restart_fn, void *get_subifid_fn
	, void *coc_confirm_stat_fn)
{
	vrx318_rx_fn = rx_fn;
	vrx318_stop_fn = stop_fn;
	vrx318_restart_fn = restart_fn;
	vrx318_get_subifid_fn = get_subifid_fn;
	vrx318_coc_confirm_stat_fn = coc_confirm_stat_fn;
}

static int pcie_ep_probe(struct vrx318_priv *pcie_ep)
{
	int dev_num;
	int i;

	if (ifx_pcie_ep_dev_num_get(&dev_num)) {
		vrx318_dbg(DBG_ERR, "Failed to get total PCIe device number\n");
		return -EIO;
	}

	for (i = 0; i < dev_num && i < MAX_VRX318_NUM; i++) {
		if (ifx_pcie_ep_dev_info_req(i,
			IFX_PCIE_EP_INT_PPE, &pcie_ep->ep_dev[i])) {
			vrx318_dbg(DBG_ERR, "%s failed to get pcie ep %d information\n",
				__func__, i);
			continue;
		}
	}
	vrx318_dbg(DBG_INIT, "Probe VRX318 total num: %d\n", i);
	pcie_ep->ep_num = i;

	if (!pcie_ep->ep_num)
		return -EIO;

	return 0;
}

static void pcie_ep_release(struct vrx318_priv *pcie_ep)
{
	int i;

	for (i = 0; i < pcie_ep->ep_num; i++)
		ifx_pcie_ep_dev_info_release(i);
}

static int init_local_variables(struct vrx318_priv *priv)
{
	struct vrx318_param *cfg;
	int i;

	priv->owner = THIS_MODULE;
	if (!priv->owner) {
		priv->owner = kmalloc(sizeof(*priv->owner), GFP_KERNEL);
		if (!priv->owner) {
			vrx318_dbg(DBG_ERR, "Failed to alloc buffer for module structure\n");
			return -ENOMEM;
		}
		sprintf(priv->owner->name, DRV_MODULE_NAME);
	}

	cfg = &priv->param;
	cfg->dfe_loopback = loopback_en;
	cfg->lle_in_sb = LLE_IN_PDBRAM ? 0 : 1;
	cfg->p2p_en = p2p_en;
	qos_en = qos_en & 0x7;

	if (!qos_en)
		cfg->atm_qos = 1;
	else {
		for (i = 1; (qos_en >> i) != 0; i++)
			;
		cfg->atm_qos = BIT(i);
	}
	if (!cfg->atm_qos)
		cfg->atm_qos = 1;

	if (oam_prio < 0 || oam_prio >= max_pvc_num(cfg->atm_qos))
		cfg->oam_prio = 0;
	else
		cfg->oam_prio = oam_prio;

	priv->pdev = priv->ep_dev[0].dev;
	vrx318_cb_setup(NULL, NULL, NULL, NULL, NULL);

	if (IS_ENABLED(CONFIG_USE_EMULATOR))
		priv->param.dfe_loopback = 1;

	if (cfg->dfe_loopback)
		vrx318_dbg(DBG_INIT, "DFE LOOPBACK Enabled\n");

	priv->tc_stat = VRX318_INIT;
	priv->hw_type = HW_TYPE_VRX318;

	for (i = 0; i < MAX_VRX318_NUM; i++)
		priv->tc_mode[i] = TC_NONE_MODE;

	vrx318_atm_param_setup(priv, qsb_tau, qsb_srvm, qsb_tstep,
		aal5r_max_pktsz, aal5r_min_pktsz, aal5s_max_pktsz);

	vrx318_ptm_param_setup(priv, queue_gamma_map);

	init_dsl_callback();

	spin_lock_init(&priv->fw_lock);

	return 0;
}

static int32_t vrx318_stop(struct net_device *dev)
{
	struct vrx318_priv *priv = g_vrx318_priv;
	int ret = -ENODEV;

	spin_lock_bh(&priv->fw_lock);
	if (priv->tc_stat != VRX318_RUNNING) {
		spin_unlock_bh(&priv->fw_lock);
		return ret;
	}

	if (vrx318_stop_fn != NULL)
		ret = vrx318_stop_fn(dev);

	spin_unlock_bh(&priv->fw_lock);

	return ret;
}

static int32_t vrx318_restart(struct net_device *dev)
{
	struct vrx318_priv *priv = g_vrx318_priv;
	int ret = -ENODEV;

	spin_lock_bh(&priv->fw_lock);
	if (priv->tc_stat != VRX318_RUNNING) {
		spin_unlock_bh(&priv->fw_lock);
		return ret;
	}

	if (vrx318_restart_fn != NULL)
		ret = vrx318_restart_fn(dev);

	spin_unlock_bh(&priv->fw_lock);

	return ret;
}

static int32_t vrx318_rx(struct net_device *rxif, struct net_device *txif,
		struct sk_buff *skb, int32_t len)
{
	struct vrx318_priv *priv = g_vrx318_priv;
	int ret = -ENODEV;

	spin_lock_bh(&priv->fw_lock);
	if (priv->tc_stat != VRX318_RUNNING) {
		spin_unlock_bh(&priv->fw_lock);
		dev_kfree_skb_any(skb);
		return ret;
	}

	if (vrx318_rx_fn != NULL) {
		ret = vrx318_rx_fn(rxif, txif, skb, len);
	} else {
		vrx318_dbg(DBG_ERR, "TC Status Running but no rx function\n");
		dev_kfree_skb_any(skb);
	}
	spin_unlock_bh(&priv->fw_lock);

	return ret;
}

static int32_t vrx318_get_subifid(struct net_device *netif, struct sk_buff *skb,
			void *vcc, uint8_t dst_mac[MAX_ETH_ALEN],
			dp_subif_t *subif, uint32_t flags)
{
	struct vrx318_priv *priv = g_vrx318_priv;
	int ret = -ENODEV;

	spin_lock_bh(&priv->fw_lock);
	if (priv->tc_stat != VRX318_RUNNING) {
		spin_unlock_bh(&priv->fw_lock);
		dev_kfree_skb_any(skb);
		return ret;
	}

	if (vrx318_get_subifid_fn != NULL)
		ret = vrx318_get_subifid_fn(netif, skb, vcc,
				dst_mac, subif, flags);

	spin_unlock_bh(&priv->fw_lock);

	return ret;
}

static int32_t vrx318_coc_confirm_stat(enum ltq_cpufreq_state new_state,
	enum ltq_cpufreq_state old_state,
	uint32_t flags)
{
	struct vrx318_priv *priv = g_vrx318_priv;
	int ret = -ENODEV;
	vrx318_dbg(DBG_COC, "%s: Start\n", __func__);
	vrx318_dbg(DBG_COC, "Change from old [%u] to [%u]\n", (u32)old_state, (u32)new_state);
	spin_lock_bh(&priv->fw_lock);
	if (priv->tc_stat != VRX318_RUNNING) {
		spin_unlock_bh(&priv->fw_lock);
		vrx318_dbg(DBG_COC, "VRX 318 is not RUNNING[%d]\n", priv->tc_stat);
		return ret;
	}

	if (vrx318_coc_confirm_stat_fn != NULL)
		ret = vrx318_coc_confirm_stat_fn(new_state, old_state, flags);
	else
		vrx318_dbg(DBG_COC, "There is no callback for COC\n");

	spin_unlock_bh(&priv->fw_lock);
	vrx318_dbg(DBG_COC, "%s: Finish\n", __func__);
	return ret;
}

int vrx318_unload_tc(struct vrx318_priv *priv, int ep_id)
{
	int err = 0;

	/* Unload Callback function in Framework */
	vrx318_cb_setup(NULL, NULL, NULL, NULL, NULL);

	if (priv->tc_mode[ep_id] == TC_ATM_SL_MODE)
		err = vrx318_unload_atm_tc(priv, ep_id);
	else if (priv->tc_mode[ep_id] == TC_PTM_SL_MODE ||
			priv->tc_mode[ep_id] == TC_PTM_BND_MODE)
		err = vrx318_unload_ptm_tc(priv, ep_id, priv->tc_mode[ep_id]);

	if (err)
		return err;
	priv->tc_priv = NULL;
	vrx318_dbg(DBG_TC_SWITCH, "vrx318 tc unload successfully\n");

	return err;
}

int vrx318_load_tc(struct vrx318_priv *priv, int ep_id,
			enum dsl_tc_mode tc_mode)
{
	if (ep_id >= priv->ep_num)
		return -EINVAL;

	switch (tc_mode) {
	case TC_ATM_SL_MODE:
		return vrx318_atm_tc_init(priv, ep_id, tc_mode);
	case TC_PTM_SL_MODE:
	case TC_PTM_BND_MODE:
		return vrx318_ptm_tc_init(priv, ep_id, tc_mode);
	case TC_NONE_MODE:
		return 0;
	default:
		return -ENOPROTOOPT;
	}
}

/**
 * TC swtich condition
 * 1. Current TC is not same as request TC
 * 2. Peer TC is not UP unless Peer TC and request TC both is PTM bonding.
 */
static int vrx318_tc_switch(struct vrx318_priv *priv, u32 ep_id,
				enum dsl_tc_mode tc_mode)
{
	u32 peer_id;
	int err = 0;

	peer_id = ((MAX_VRX318_NUM - 1) - ep_id);

	spin_lock_bh(&priv->fw_lock);

	if (priv->tc_stat != VRX318_RUNNING
			&& priv->tc_stat != VRX318_NO_TC) {
		err = -EAGAIN;
		vrx318_dbg(DBG_TC_SWITCH, "tc stat: %d\n", (u32)priv->tc_stat);
		goto TCSW_CHECK_FAIL;
	}

	if (tc_mode == TC_PTM_BND_MODE && priv->ep_num < 2) {
		err = -EINVAL;
		vrx318_dbg(DBG_TC_SWITCH, "request bonding but only %d dev detected\n",
			priv->ep_num);
		goto TCSW_CHECK_FAIL;
	}

	if (priv->tc_mode[ep_id] == tc_mode) {
		vrx318_dbg(DBG_TC_SWITCH, "TC switch request on same TC\n");
		err = 0;
		goto TCSW_CHECK_FAIL;
	}

	priv->tc_stat = VRX318_SWITCHING;
	spin_unlock_bh(&priv->fw_lock);

	/* Unload TC */
	if (priv->tc_mode[ep_id] != TC_NONE_MODE) {
		err = vrx318_unload_tc(priv, ep_id);
		if (err)
			return err;
		if (priv->tc_mode[ep_id] == TC_PTM_BND_MODE)
			priv->tc_mode[peer_id] = TC_NONE_MODE;
		priv->tc_mode[ep_id] = TC_NONE_MODE;
	}

	/* Load requested TC */
	err = vrx318_load_tc(priv, ep_id, tc_mode);
	if (err)
		return err;
	if (priv->tc_mode[ep_id] == TC_PTM_BND_MODE)
		priv->tc_mode[peer_id] = tc_mode;
	priv->tc_mode[ep_id] = tc_mode;

	spin_lock_bh(&priv->fw_lock);
	priv->tc_stat = VRX318_RUNNING;
	spin_unlock_bh(&priv->fw_lock);

	return err;

TCSW_CHECK_FAIL:
	spin_unlock_bh(&priv->fw_lock);
	return err;

}

static void vrx318_req_workqueue(struct work_struct *wk)
{
	struct tc_req *req_work;

	req_work = container_of(wk, struct tc_req, work);
	vrx318_tc_switch(g_vrx318_priv, req_work->id, req_work->tc_mode);
}


/**
 * API defintion for the DSL MEI driver to call to do TC switch
 * line_no: the DSL line number
 * tc_type: TC mode to be loaded. ATM TC/PTM TC
 * is_bondng:  Bonding mode: 0- No, 1-Yes.
 */
static int dsl_req_tc_switch(const unsigned char line_no,
		mei_tc_request_type tc_type,
		int is_bonding)
{
	struct vrx318_priv *priv = g_vrx318_priv;
	enum dsl_tc_mode tc_mode;

	vrx318_dbg(DBG_TC_SWITCH, "%s, line_no: %d, tc_type: %d, is_bonding: %d\n",
		__func__, line_no, tc_type, is_bonding);

	if (!priv || line_no >= priv->ep_num || tc_type >= MEI_TC_REQUEST_LAST)
		return -EINVAL;

	switch (tc_type) {
	case MEI_TC_REQUEST_OFF:
		tc_mode = TC_NONE_MODE;
		break;
	case MEI_TC_REQUEST_ATM:
		tc_mode = TC_ATM_SL_MODE;
		break;
	case MEI_TC_REQUEST_PTM:
		if (is_bonding)
			tc_mode = TC_PTM_BND_MODE;
		else
			tc_mode = TC_PTM_SL_MODE;
		break;
	default:
		return -EINVAL;
	}

	priv->tc_wkq.id = line_no;
	priv->tc_wkq.tc_mode = tc_mode;
	queue_work(system_wq, &priv->tc_wkq.work);
	
	return 0;
}

static void print_ver(const char *name, const char *ver)
{
	vrx318_dbg(DBG_INFO, "%s version: %s\n", name, ver);
}

static int __init vrx318_drv_init(void)
{
	struct vrx318_priv *priv;
	int err;
	int dp_id;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		vrx318_dbg(DBG_ERR, "Failed to alloc priv buffer\n");
		err = -ENOMEM;
		goto err1;
	}

	err = pcie_ep_probe(priv);
	if (err)
		goto err1;

	err = vrx318_soc_cfg_get(priv);
	if (err)
		goto err2;

	err = init_local_variables(priv);
	if (err)
		goto err2;

	dp_id  = dp_alloc_port(priv->owner, NULL, 0, 0, NULL, DP_F_FAST_DSL);
	if (dp_id == DP_FAILURE) {
		vrx318_dbg(DBG_ERR, "%s: dp_alloc_port fail!\n", __func__);
		err = -ENODEV;
		goto err2;
	}
	priv->port_id		= dp_id;
	priv->cb.stop_fn	= vrx318_stop;
	priv->cb.restart_fn	= vrx318_restart;
	priv->cb.rx_fn		= vrx318_rx;
	priv->cb.get_subifid_fn = vrx318_get_subifid;
	priv->cb.dp_coc_confirm_stat_fn = vrx318_coc_confirm_stat;

	if (dp_register_dev(priv->owner, dp_id, &priv->cb, 0) != DP_SUCCESS) {
		vrx318_dbg(DBG_ERR, "%s:dp_register_dev failed for port id %d",
			__func__, dp_id);
		err = -ENODEV;
		goto err3;
	}
	vrx318_dbg(DBG_INFO, "DP register VRX318 successfully, dp_id: %d\n",
		dp_id);

	INIT_WORK(&priv->tc_wkq.work, vrx318_req_workqueue);
	print_ver(DRV_MODULE_NAME, DRV_MODULE_VERSION);
	vrx318_dbg(DBG_INFO, "Total %d device are detected\n", priv->ep_num);

	priv->tc_stat = VRX318_NO_TC;
	g_vrx318_priv = priv;
	vrx318_proc_init(priv);
	vrx318_genetlink_init();

	ppa_callback_set(LTQ_MEI_TC_REQUEST, dsl_req_tc_switch);

	return 0;

err3:
	dp_alloc_port(priv->owner, NULL, 0, dp_id, NULL, DP_F_DEREGISTER);
err2:
	pcie_ep_release(priv);
err1:
	kfree(priv);
	return err;
}

/* Warn: module unload should not happen during TC_switch */
static void __exit vrx318_drv_exit(void)
{
	struct vrx318_priv *priv = g_vrx318_priv;
	int i, ret;

	spin_lock_bh(&priv->fw_lock);
	if (priv->tc_stat == VRX318_INIT ||
		priv->tc_stat == VRX318_SWITCHING)
		goto err;
	priv->tc_stat = VRX318_EXIT;
	g_vrx318_priv = NULL;
	spin_unlock_bh(&priv->fw_lock);

	cancel_work_sync(&priv->tc_wkq.work);
	for (i = 0; i < priv->ep_num; i++) {
		if (priv->tc_mode[i] != TC_NONE_MODE) {
			vrx318_unload_tc(priv, i);
			break;
		}
	}

	vrx318_atm_exit();
	vrx318_ptm_exit();

	ret = dp_register_dev(priv->owner, priv->port_id,
			NULL, DP_F_DEREGISTER);
	if (ret)
		vrx318_dbg(DBG_ERR,
			"Unregister VRX318 dp dev error: %d\n", ret);
	ret = dp_alloc_port(priv->owner, NULL, 0,
			priv->port_id, NULL, DP_F_DEREGISTER);
	if (ret)
		vrx318_dbg(DBG_ERR, "Free VRX318 dp port err: %d\n", ret);

	vrx318_proc_exit(priv);
	kfree(priv);
	vrx318_genetlink_exit();
	vrx318_dbg(DBG_INFO, "VRX318 MODULE Unloaded!\n");

	return;
err:
	spin_unlock_bh(&priv->fw_lock);
	vrx318_dbg(DBG_ERR, "Exiting while tc mode: %d\n", (u32)priv->tc_mode);
	return;
}

module_init(vrx318_drv_init);
module_exit(vrx318_drv_exit);

