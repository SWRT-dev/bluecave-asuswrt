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

#include "inc/tc_main.h"
#include "inc/platform.h"
#include "inc/reg_addr.h"
#include "inc/tc_common.h"
#include "inc/tc_api.h"
#include "inc/tc_proc.h"


#define DRV_VERSION	"1.3.3"

static const char tc_drv_name[] = "vrx518_tc";
static const char tc_drv_ver[] = DRV_VERSION;
static const char tc_drv_string[] =
			"Intel(R) SmartPHY DSL(VRX518) PCIe TC Driver";
static const char tc_drv_copyright[] =
			"Copyright (c) 2016 Intel Corporation.";
static struct tc_priv *g_tc_priv;
static int p2p_en = 1;
static int loopback_en;
static int sharing_cdma_en = 1;
static u32 umt_period = UMT_DEF_PERIOD;
static u32 cdma_desc_loc = 1;
static u32 powersaving;

module_param(p2p_en, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(p2p_en, "PTM Bonding directly access Peer Enable flag.");
module_param(loopback_en, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(loopback_en, "Enable-1/Disable-0 the DFE loopback mode.");
module_param(sharing_cdma_en, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(sharing_cdma_en, "Enable-1/Disable-0 the sharing CDMA mode.");
module_param(umt_period, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(umt_period, "Adjust the frequency of the UMT message, default: 200us");
module_param(cdma_desc_loc, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(cdma_desc_loc, "CDMA descriptor address: 0 - SRAM/1 - FPI");
module_param(powersaving, uint, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(powersaving, "Powersaving configuration: 0 - Disable/1-Enable");

static int pcie_ep_probe(struct tc_priv *priv)
{
	int dev_num;
	int i;

	if (dc_ep_dev_num_get(&dev_num)) {
		tc_err(priv, MSG_INIT, "Failed to get total PCIe device number\n");
		return -EIO;
	}
	tc_dbg(priv, MSG_INIT, "Total %d VRX518 EP detected\n", dev_num);

	for (i = 0; i < dev_num; i++) {
		if (dc_ep_dev_info_req(i, DC_EP_INT_PPE, &priv->ep_dev[i])) {
			tc_err(priv, MSG_INIT, "failed to get pcie ep %d\n", i);
			goto err1;
		}
	}

	priv->ep_num = i;
	priv->switch_exist = priv->ep_dev[0].switch_attached;

	return 0;
err1:
	for (i = i - 1; i >= 0; i--)
		dc_ep_dev_info_release(i);

	return -EIO;
}

static void pcie_ep_release(struct tc_priv *priv)
{
	int i;

	for (i = 0; i < priv->ep_num; i++)
		dc_ep_dev_info_release(i);
}

static void tc_def_recv(struct net_device *pdev, struct sk_buff *skb)
{
	tc_dbg(g_tc_priv, MSG_RX, "TC is not loaded\n");
	dev_kfree_skb_any(skb);
}

static void tc_def_irq_on(u32 irq_no)
{
	return;
}

static void tc_def_irq_off(u32 irq_no)
{
	return;
}

static int tc_def_getqid(struct net_device *pdev, struct sk_buff *skb,
		void *vcc, uint32_t flags)
{
	return -1;
}

static inline void tc_def_ops_setup(struct tc_priv *priv)
{
	spin_lock_bh(&priv->tc_lock);
	priv->tc_ops.recv = tc_def_recv;
	priv->tc_ops.irq_on = tc_def_irq_on;
	priv->tc_ops.irq_off = tc_def_irq_off;
	priv->tc_ops.get_qid = tc_def_getqid;
	priv->tc_ops.showtime_enter = NULL;
	priv->tc_ops.showtime_exit = NULL;
	/*priv->tc_ops.tc_reset = NULL;*/
	spin_unlock_bh(&priv->tc_lock);
}

static inline void init_local_param(struct tc_priv *priv)
{
	if (priv->ep_num > 1)
		priv->param.bonding_en = 1;
	priv->param.dfe_loopback = loopback_en;
	priv->param.p2p = p2p_en;
	priv->param.umt_period = umt_period;
	priv->param.sharing_cdma_en = sharing_cdma_en;
	priv->param.cdma_desc_loc = cdma_desc_loc;
	priv->param.ps = powersaving;
	priv->tc_mode = TC_NONE_MODE;
	priv->tc_stat = NO_TC;
	priv->tc_idx = -1;
	spin_lock_init(&priv->tc_lock);
	tc_def_ops_setup(priv);
}

void tc_unload(struct tc_priv *priv)
{
	if (priv->tc_mode == TC_NONE_MODE)
		return;
	tc_def_ops_setup(priv);
	if (priv->tc_mode == TC_PTM_SL_MODE ||
		priv->tc_mode == TC_PTM_BND_MODE)
		ptm_tc_unload(priv->tc_mode);
	else if (priv->tc_mode == TC_ATM_SL_MODE)
		atm_tc_unload();

	spin_lock_bh(&priv->tc_lock);
	priv->tc_mode = TC_NONE_MODE;
	priv->tc_stat = NO_TC;
	priv->priv = NULL;
	spin_unlock_bh(&priv->tc_lock);
}

void tc_load(struct tc_priv *priv, u32 id, enum dsl_tc_mode mode)
{
	if (mode == TC_NONE_MODE)
		return;

	spin_lock_bh(&priv->tc_lock);
	priv->tc_mode = mode;
	spin_unlock_bh(&priv->tc_lock);
	if (mode == TC_PTM_SL_MODE || mode == TC_PTM_BND_MODE)
		ptm_tc_load(priv, id, mode);
	else if (mode == TC_ATM_SL_MODE)
		atm_tc_load(priv, id, mode);

	spin_lock_bh(&priv->tc_lock);
	priv->tc_stat = TC_RUN;
	priv->tc_idx = id;
	spin_unlock_bh(&priv->tc_lock);
}

void tc_request(u32 id, enum dsl_tc_mode tc_mode)
{
	struct tc_priv *priv = g_tc_priv;

	tc_unload(priv);
	tc_load(priv, id, tc_mode);
}

void tc_get_drv_version(const char **drv_name, const char **drv_ver)
{
	*drv_name = tc_drv_name;
	*drv_ver = tc_drv_ver;
}

static int __init tc_drv_init(void)
{
	struct tc_priv *priv;
	int ret;
	size_t size;

	size = ALIGN(sizeof(struct tc_priv), TCPRIV_ALIGN) + plat_priv_sz();
	priv = kzalloc(size, GFP_KERNEL);
	if (!priv) {
		pr_err("Alloc tc priv fail!\n");
		return -ENOMEM;
	}
	g_tc_priv = priv;

	priv->msg_enable = TC_DEF_DBG;
	ret = pcie_ep_probe(priv);
	if (ret < 0) {
		tc_err(priv, MSG_INIT, "ep probe fail!\n");
		goto err1;
	}

	init_local_param(priv);

	ret = platform_init(priv, tc_drv_name);
	if (ret < 0) {
		tc_err(priv, MSG_INIT, "soc platform init fail!\n");
		goto err2;
	}
	ret = ppe_fw_load(priv);
	if (ret < 0)
		goto err3;

	tc_gentlk_init(priv);
	tc_proc_init(priv);

	tc_info(priv, MSG_INIT, "%s - version %s\n",
		tc_drv_string, tc_drv_ver);
	tc_info(priv, MSG_INIT, "%s\n", tc_drv_copyright);

	return 0;
err3:
	platform_exit();
err2:
	pcie_ep_release(priv);
err1:
	kfree(priv);
	g_tc_priv = NULL;
	return ret;
}
module_init(tc_drv_init);

/**
 * 1. Unregister DSL callback function
 * 2. Unload TC
 * 3. free ATM/PTM/Bonding driver resources
 * 4. free SoC resource
 * 5. free proc
 * 6. free tc_priv
 * 7. free netlink resource
 */
static void __exit tc_drv_exit(void)
{
	struct tc_priv *priv;

	priv = g_tc_priv;
	platform_dsl_exit();
	spin_lock_bh(&priv->tc_lock);
	if (priv->tc_stat == TC_INIT || priv->tc_stat == TC_SWITCHING)
		goto err1;
	spin_unlock_bh(&priv->tc_lock);
	if (priv->tc_stat == TC_RUN)
		tc_unload(priv);

	ptm_exit();
	atm_exit();
	ppe_fw_unload(priv);
	tc_proc_exit(priv);
	platform_exit();
	pcie_ep_release(priv);
	kfree(priv);
	tc_gentlk_exit();

	pr_info("%s driver unloaded\n", tc_drv_name);
	return;

err1:
	spin_unlock_bh(&priv->tc_lock);
	tc_dbg(priv, MSG_SWITCH,
		"TC is not ready to exit: %d\n", priv->tc_stat);

	return;
}
module_exit(tc_drv_exit);


MODULE_AUTHOR("Intel Corporation, <yixin.zhu@intel.com>");
MODULE_DESCRIPTION("Intel(R) SmartPHY PCIe EP/ACA Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_VERSION);
