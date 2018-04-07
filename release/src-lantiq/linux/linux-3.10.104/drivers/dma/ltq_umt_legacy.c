/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (C) 2015 Zhu YiXin<yixin.zhu@lantiq.com>
 * UMT Driver for GRX350 A11
 */
#define DEBUG
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/seq_file.h>
#include <lantiq_dmax.h>
	 
#include <lantiq.h>
#include <lantiq_soc.h>
#include <lantiq_irq.h>
	 
#include <net/datapath_proc_api.h>
#include "ltq_hwmcpy_addr.h"
#include <linux/ltq_hwmcpy.h>
#include "ltq_hwmcpy.h"

static int umt_dma_init(struct dma_ch *pchan, struct mcpy_umt *pumt)
{
	u32 dma_rxch, dma_txch;

	pchan->rch = UMT_DMA_RX_CID;
	pchan->tch = UMT_DMA_TX_CID;
	pchan->rch_dnum = 1;
	pchan->tch_dnum = 1;
	pchan->rch_dbase =
		pumt->ctrl->phybase + UMT_DBASE;
		pchan->tch_dbase = pchan->rch_dbase + 0x10;
	pchan->onoff = DMA_CH_ON;
	sprintf(pchan->rch_name, "UMT RXCH");
	sprintf(pchan->tch_name, "UMT TXCH");

	dma_rxch = _DMA_C(pumt->ctrl->dma_ctrl_id,
		pumt->ctrl->dma_port_id,
		pchan->rch);
	mcpy_dbg(MCPY_DBG, "dma_ch: 0x%x, rch_name:%s\n",
		dma_rxch, pchan->rch_name);
	if (ltq_request_dma(dma_rxch, pchan->rch_name) < 0) {
		mcpy_dbg(MCPY_ERR,
			"umt request dma chan [0x%x] fail\n", dma_rxch);
		goto __UMT_PORT_FAIL;
	}
	if (ltq_dma_chan_desc_cfg(dma_rxch,
			pchan->rch_dbase, pchan->rch_dnum) < 0) {
		mcpy_dbg(MCPY_ERR, "setup dma chan [0x%x] fail\n", dma_rxch);
		goto __UMT_PORT_FAIL;
	}

	dma_txch = _DMA_C(pumt->ctrl->dma_ctrl_id,
			pumt->ctrl->dma_port_id,
			pchan->tch);
	mcpy_dbg(MCPY_DBG, "dma_ch: 0x%x, rch_name:%s\n",
		dma_txch, pchan->tch_name);
	if (ltq_request_dma(dma_txch, pchan->tch_name) < 0) {
		mcpy_dbg(MCPY_ERR, "request dma chan [0x%x] fail\n", dma_txch);
		goto __UMT_PORT_FAIL;
	}
	if (ltq_dma_chan_desc_cfg(dma_txch,
			pchan->tch_dbase, pchan->tch_dnum) < 0) {
		mcpy_dbg(MCPY_ERR, "setup dma chan [0x%x] fail\n", dma_txch);
		goto __UMT_PORT_FAIL;
	}

	if (pchan->onoff == DMA_CH_ON) {
		ltq_dma_chan_on(dma_rxch);
		ltq_dma_chan_on(dma_txch);
	} else {
		ltq_dma_chan_off(dma_rxch);
		ltq_dma_chan_off(dma_txch);
	}

	return 0;

__UMT_PORT_FAIL:
	pumt->status = UMT_BROKEN;
	return -1;
}

static inline void umt_set_mode(enum umt_mode umt_mode)
{
	ltq_mcpy_w32_mask(0x2, ((u32)umt_mode) << 1, MCPY_GCTRL);
}

static inline void umt_set_msgmode(enum umt_msg_mode msg_mode)
{
	ltq_mcpy_w32(msg_mode, MCPY_UMT_SW_MODE);
}

static inline u32 umt_msec_to_cnt(int msec)
{
	return (msec * 0x17AC / 20);
}

static inline void umt_set_period(u32 umt_period)
{
	umt_period = umt_msec_to_cnt(umt_period);
	ltq_mcpy_w32(umt_period, MCPY_UMT_PERD);
}

static inline void umt_set_dst(u32 umt_dst)
{
	ltq_mcpy_w32(umt_dst, MCPY_UMT_DEST);
}

static inline void umt_enable(enum umt_status status)
{
	ltq_mcpy_w32_mask(0x4, ((u32)status) << 2, MCPY_GCTRL);
}

/*This function will disable umt */
static inline void umt_reset_umt(enum umt_mode mode)
{
	umt_enable(UMT_DISABLE);
	if (mode == UMT_SELFCNT_MODE) {
		umt_set_mode(UMT_USER_MODE);
		umt_set_mode(UMT_SELFCNT_MODE);
	} else {
		umt_set_mode(UMT_SELFCNT_MODE);
		umt_set_mode(UMT_USER_MODE);
	}
	return;
}

int ltq_umt_set_period(u32 period)
{
	struct mcpy_umt *pumt = mcpy_get_umt();
	if (period == 0)
		return -1;

	if (pumt->umt_period == period)
		return 0;
	else {
		spin_lock_bh(&pumt->umt_lock);
		pumt->umt_period = period;
		umt_set_period(pumt->umt_period);
		spin_unlock_bh(&pumt->umt_lock);
	}

	return 0;
}
EXPORT_SYMBOL(ltq_umt_set_period);

/*
* @umt_mode:	0-self-counting mode, 1-user mode.
* @msg_mode:	0-No MSG, 1-MSG0 Only, 2-MSG1 Only, 3-MSG0 & MSG1.
* @dst:  Destination PHY address.
* @period: only applicable when set to self-counting mode.
*		 self-counting interval time. if 0, use the original setting.
* @enable: 1-Enable/0-Disable
* @ret: -1 Fail/0-SUCCESS
*/
int ltq_umt_set_mode(u32 umt_mode, u32 msg_mode,
			u32 phy_dst, u32 period, u32 enable)
{
	struct mcpy_umt *pumt = mcpy_get_umt();

	if (pumt->status == UMT_BROKEN)
		return -ENODEV;
	if (umt_mode >= (u32)UMT_MODE_MAX
			|| msg_mode >= (u32)UMT_MSG_MAX
			|| enable   >= (u32)UMT_STATUS_MAX
			|| phy_dst  == 0)
		return -EINVAL;

	spin_lock_bh(&pumt->umt_lock);
	umt_reset_umt(pumt->umt_mode);

	pumt->umt_mode = (enum umt_mode)umt_mode;
	pumt->msg_mode = (enum umt_msg_mode)msg_mode;
	pumt->umt_dst	= phy_dst;
	if (period)
		pumt->umt_period = period;
	pumt->status = (enum umt_status)enable;

	umt_set_mode(pumt->umt_mode);
	umt_set_msgmode(pumt->msg_mode);
	umt_set_dst(pumt->umt_dst);
	if (period)
		umt_set_period(pumt->umt_period);
	umt_enable(pumt->status);
	spin_unlock_bh(&pumt->umt_lock);

	return 0;
}
EXPORT_SYMBOL(ltq_umt_set_mode);

/*
* Enable/Disable UMT
* @enable: 1-Enable/0-Disable
* ret: -1 Fail/0-Success
*/
int ltq_umt_enable(u32 enable)
{
	struct mcpy_umt *pumt = mcpy_get_umt();
	if (enable >= (u32)UMT_STATUS_MAX
			|| pumt->status == UMT_BROKEN)
		return -ENODEV;

	if (pumt->status != enable) {
		spin_lock_bh(&pumt->umt_lock);
		pumt->status = (enum umt_status)enable;
		umt_enable(pumt->status);
		spin_unlock_bh(&pumt->umt_lock);
	}

	return 0;
}
EXPORT_SYMBOL(ltq_umt_enable);

static void umt_access_wkrd(void)
{
	unsigned int __iomem *addr;
	u32 val;

	addr = (unsigned int __iomem *)GCR_CFG_ENABLE;
	ltq_w32(0x123F0001, addr);
	addr = (unsigned int __iomem *)GCR_CCA_IC_MREQ(DMA3_MASTERID);
	ltq_wk_w32(2, addr);
	addr = (unsigned int __iomem *)GCR_CTRL_REG;
	val = ltq_r32(addr);
	val |= 2;
	ltq_w32(val, addr);
	mcpy_dbg(MCPY_INFO, "UMT workaround applied!!\n");
}

static void umt_chip_init(struct mcpy_umt *pumt)
{
	umt_access_wkrd();
	umt_set_mode(pumt->umt_mode);
	umt_set_msgmode(pumt->msg_mode);
	if (pumt->umt_period)
		umt_set_period(pumt->umt_period);
	if (pumt->umt_dst)
		umt_set_dst(pumt->umt_dst);
	umt_enable((u32)pumt->status);
}

/* TODO: Register UMT error interrupt Handler */
void umt_init(struct mcpy_ctrl *pctrl)
{
	struct device_node *node = pctrl->dev->of_node;
	struct mcpy_umt *pumt;

	pumt = &pctrl->umt;
	pumt->ctrl = pctrl;
	pumt->umt_mode = UMT_SELFCNT_MODE;
	pumt->status = UMT_DISABLE;
	pumt->msg_mode = UMT_MSG0_MSG1;
	pumt->ctrl = pctrl;

	if (of_property_read_u32(node,
		"lantiq,umt-period", &pumt->umt_period) < 0)
	pumt->umt_period = 0;	 
	spin_lock_init(&pumt->umt_lock);
	pumt->dev = pctrl->dev;

	if (umt_dma_init(&pumt->chan, pumt) < 0)
		return;
	umt_chip_init(pumt);
	return;
}

static int umt_cfg_read_proc(struct seq_file *s, void *v)
{
	struct mcpy_umt *pumt = s->private;

	seq_puts(s, "\nUMT configuration\n");
	seq_puts(s, "-----------------------------------------\n");
	seq_printf(s, "UMT Mode                 %s\n",
		pumt->umt_mode == UMT_SELFCNT_MODE ?
		"UMT SelfCounting Mode" : "UMT User Mode");
	switch (pumt->msg_mode) {
	case UMT_NO_MSG:
		seq_puts(s, "UMT MSG Mode             UMT NO MSG\n");
		break;
	case UMT_MSG0_ONLY:
		seq_puts(s, "UMT MSG Mode             UMT MSG0 Only\n");
		break;
	case UMT_MSG1_ONLY:
		seq_puts(s, "UMT MSG Mode             UMT MSG1 Only\n");
		break;
	case UMT_MSG0_MSG1:
		seq_puts(s, "UMT MSG Mode             UMT_MSG0_And_MSG1\n");
		break;
	default:
		seq_printf(s, "UMT MSG Mode Error! Msg_mode: %d\n",
			pumt->msg_mode);
	}
	seq_printf(s, "UMT DST                  0x%x\n", pumt->umt_dst);
	if (pumt->umt_mode == UMT_SELFCNT_MODE)
		seq_printf(s, "UMT Period               0x%x\n",
			pumt->umt_period == 0 ? 0x17AC : pumt->umt_period);
	seq_printf(s, "UMT Status               %s\n",
			pumt->status == UMT_ENABLE ? "Enable" :
			pumt->status == UMT_DISABLE ? "Disable" : "Init Fail");
	seq_printf(s, "dma rxch_id: %d, rch_base: 0x%x, rch_des_num: %d\n",
		pumt->chan.rch, pumt->chan.rch_dbase,
		pumt->chan.rch_dnum);
	seq_printf(s, "dma txch_id: %d, tch_base: 0x%x, tch_des_num: %d\n",
		pumt->chan.tch, pumt->chan.tch_dbase,
		pumt->chan.tch_dnum);
	seq_printf(s, "dma chan on/off: %s\n",
		pumt->chan.onoff == DMA_CH_ON ? "ON" : "OFF");

	return 0;
}

static int umt_cfg_read_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, umt_cfg_read_proc, PDE_DATA(inode));
}

static const struct file_operations mcpy_umt_proc_fops = {
	.open           = umt_cfg_read_proc_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};


