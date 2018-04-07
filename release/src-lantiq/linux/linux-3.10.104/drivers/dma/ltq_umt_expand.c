/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (C) 2015 Zhu YiXin<yixin.zhu@lantiq.com>
 * UMT Driver for GRX350 A21
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
#include <asm/ltq_vmb.h>

#include <lantiq_dmax.h>
#include <lantiq.h>
#include <lantiq_soc.h>
#include <lantiq_irq.h>

#include <net/datapath_proc_api.h>
#include "ltq_hwmcpy_addr.h"
#include <linux/ltq_hwmcpy.h>
#include "ltq_hwmcpy.h"

#ifdef CONFIG_LTQ_UMT_SW_MODE
#include "net/lantiq_cbm_api.h"

static u8 umt_tc_thread_stack[4096]__attribute__((aligned(16)));
static u8 umt_tc_thread_gp[4096]__attribute__((aligned(4096)));
#define GPTC_1A_GIC_IRQ 172
#define GPTC_3A 4
#define GPT_IRNCR 0xFC
#define GPTC1_MODULE_BASE 0x16300000u
#define GPTC1_BASE 		(GPTC1_MODULE_BASE | KSEG1)
#define GPTC1_IRNCR     (GPTC1_BASE + GPT_IRNCR)
u32 jiffies1;
#define UMT_GIC_BASE_ADDR		0xb2320000  // KSEG0 address of the GIC
#define GIC_SH_RMASK31_0 0x0300
#define GIC_SH_SMASK31_00 0x0380
#define GIC_SH_MASK31_00 0x0400
#define GIC_SH_PEND31_00 0x0480
#define GIC_SH_WEDGE 0x0280

#define GPTC_1A_PMASK(irq_no)		  (UMT_GIC_BASE_ADDR+GIC_SH_PEND31_00+((irq_no>>5)*0x4))
#define GPTC_1A_MASK_BIT(irq_no)      (irq_no - ((irq_no>>5)*32))
#define GIC_SH_WEDGE_REG    (GIC_SH_WEDGE | UMT_GIC_BASE_ADDR)

#define LTQ_UMT_SW_INTERVAL_DEFAULT 40    // the default interval is 40ms
static u32 gptc_pmask, gptc_maskbit, g_umt_interval = LTQ_UMT_SW_INTERVAL_DEFAULT; 
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

struct thrd_param {
	u32 dummy; //for reserve
};

static struct thrd_param g_umt_thread_info;
static volatile u32 g_tot_dq_cnt[UMT_PORTS_NUM];
#endif

static u32 g_dma_ctrl = DMA1TX;

static inline void umt_set_mode(u32 umt_id, enum umt_mode umt_mode)
{
	u32 val, off;

	if (!umt_id)
		ltq_mcpy_w32_mask(0x2, ((u32)umt_mode) << 1, MCPY_GCTRL);
	else {
		off = 16 + (umt_id - 1) * 3;
		val = ltq_mcpy_r32(MCPY_GCTRL) & ~(BIT(off));
		ltq_mcpy_w32(val | (((u32)umt_mode) << off), MCPY_GCTRL);
	}
}

static inline void umt_set_msgmode(u32 umt_id, enum umt_msg_mode msg_mode)
{
	if (!umt_id)
		ltq_mcpy_w32((u32)msg_mode, MCPY_UMT_SW_MODE);
	else
		ltq_mcpy_w32((u32)msg_mode,
			MCPY_UMT_X_ADDR(umt_id, MCPY_UMT_XSW_MODE));
}

/* input in term of microseconds */
static inline u32 umt_us_to_cnt(int usec)
{
	struct clk *ngi_clk = clk_get_xbar();

	return usec * (clk_get_rate(ngi_clk) / 1000000);
}

static inline void umt_set_period(u32 umt_id, u32 umt_period)
{
	umt_period = umt_us_to_cnt(umt_period);

	if (!umt_id)
		ltq_mcpy_w32(umt_period, MCPY_UMT_PERD);
	else
		ltq_mcpy_w32(umt_period,
			MCPY_UMT_X_ADDR(umt_id, MCPY_UMT_XPERIOD));
}

static inline void umt_set_dst(u32 umt_id, u32 umt_dst)
{
	if (!umt_id)
		ltq_mcpy_w32(umt_dst, MCPY_UMT_DEST);
	else
		ltq_mcpy_w32(umt_dst,
			MCPY_UMT_X_ADDR(umt_id, MCPY_UMT_XDEST));
}

static inline void umt_set_mux(u32 umt_id, u32 cbm_pid, u32 dma_cid)
{
	u32 mux_sel;

	cbm_pid = cbm_pid & 0xF;
	dma_cid = dma_cid & 0xF;
	mux_sel = ltq_mcpy_r32(MCPY_UMT_TRG_MUX) &
			(~((0xF000F) << (umt_id * 4)));
	mux_sel |= (dma_cid << (umt_id * 4)) |
			(cbm_pid << (16 + (umt_id * 4)));
	ltq_mcpy_w32(mux_sel, MCPY_UMT_TRG_MUX);
}

static inline void umt_set_endian(int dw_swp, int byte_swp)
{
	u32 val;

	val = ltq_mcpy_r32(MCPY_GCTRL);
	if (byte_swp)
		val |= BIT(28);
	else
		val &= ~(BIT(28));

	if (dw_swp)
		val |= BIT(29);
	else
		val &= ~(BIT(29));

	ltq_mcpy_w32(val, MCPY_GCTRL);
}

static inline void umt_en_expand_mode(void)
{
	u32 val;

	val = ltq_mcpy_r32(MCPY_GCTRL) | BIT(31);
	ltq_mcpy_w32(val, MCPY_GCTRL);

	if (IS_ENABLED(CONFIG_CPU_BIG_ENDIAN))
		umt_set_endian(1, 0);
	else
		umt_set_endian(1, 1);
}

static inline void umt_enable(u32 umt_id, enum umt_status status)
{
	u32 val, off;

	if (!umt_id)
		ltq_mcpy_w32_mask(0x4, ((u32)status) << 2, MCPY_GCTRL);
	else {
		off = 17 + (umt_id - 1) * 3;
		val = (ltq_mcpy_r32(MCPY_GCTRL) & ~BIT(off))
				| (((u32)status) << off);
		ltq_mcpy_w32(val, MCPY_GCTRL);
	}
}

static inline void umt_suspend(u32 umt_id, enum umt_status status)
{
	u32 val;

	if (status)
		val = ltq_mcpy_r32(MCPY_UMT_CNT_CTRL) | BIT(umt_id);
	else
		val = ltq_mcpy_r32(MCPY_UMT_CNT_CTRL) & (~(BIT(umt_id)));

	ltq_mcpy_w32(val, MCPY_UMT_CNT_CTRL);
}

/*This function will disable umt */
static inline void umt_reset_umt(u32 umt_id)
{
	u32 mode;
	umt_enable(umt_id, UMT_DISABLE);

	mode = ltq_mcpy_r32(MCPY_UMT_X_ADDR(umt_id, MCPY_UMT_XSW_MODE));

	if (mode == UMT_SELFCNT_MODE) {
		umt_set_mode(umt_id, UMT_USER_MODE);
		umt_set_mode(umt_id, UMT_SELFCNT_MODE);
	} else {
		umt_set_mode(umt_id, UMT_SELFCNT_MODE);
		umt_set_mode(umt_id, UMT_USER_MODE);
	}

	return;
}

/**
 * intput:
 * @umt_id: UMT port id, (0 - 3)
 * @ep_id:  Aligned with datapath lib ep_id
 * @period: measured in microseconds.
 * ret:  Fail < 0 / Success: 0
 */
int ltq_umt_set_period(u32 umt_id, u32 ep_id, u32 period)
{
	struct mcpy_umt *pumt = mcpy_get_umt();
	struct umt_port *port;

	if (period < MIN_UMT_PRD || umt_id >= UMT_PORTS_NUM)
		goto period_err;

	if (pumt->status != UMT_ENABLE) {
		mcpy_dbg(MCPY_ERR, "UMT is not initialized!\n");
		return -ENODEV;
	}

	port = &pumt->ports[umt_id];

	spin_lock_bh(&port->umt_port_lock);
	if (port->ep_id != ep_id) {
		spin_unlock_bh(&port->umt_port_lock);
		goto period_err;
	}

	if (port->umt_period != period) {
		port->umt_period = period;
		umt_set_period(umt_id, port->umt_period);
	}
	spin_unlock_bh(&port->umt_port_lock);

	return 0;

period_err:
	mcpy_dbg(MCPY_ERR, "umt_id: %d, ep_id: %d, period: %d\n",
		umt_id, ep_id, period);

	return -EINVAL;
}
EXPORT_SYMBOL(ltq_umt_set_period);

/**
 * API to configure the UMT port.
 * input:
 * @umt_id: (0 - 3)
 * @ep_id: aligned with datapath lib EP
 * @umt_mode:  0-self-counting mode, 1-user mode.
 * @msg_mode:  0-No MSG, 1-MSG0 Only, 2-MSG1 Only, 3-MSG0 & MSG1.
 * @dst:  Destination PHY address.
 * @period(ms): only applicable when set to self-counting mode.
 *              self-counting interval time. if 0, use the original setting.
 * @enable: 1-Enable/0-Disable
 * @ret:  Fail < 0 , SUCCESS:0
 */
int ltq_umt_set_mode(u32 umt_id, u32 ep_id, u32 umt_mode, u32 msg_mode,
			u32 phy_dst, u32 period, u32 enable)
{
	struct mcpy_umt *pumt = mcpy_get_umt();
	struct umt_port *port;

	if (pumt->status != UMT_ENABLE) {
		mcpy_dbg(MCPY_ERR, "UMT is not initialized!!\n");
		return -ENODEV;
	}
	if ((umt_mode >= (u32)UMT_MODE_MAX)
			|| (msg_mode >= (u32)UMT_MSG_MAX)
			|| (enable >= (u32)UMT_STATUS_MAX)
			|| (phy_dst == 0)
			|| (period == 0)
			|| (umt_id >= UMT_PORTS_NUM)) {
		mcpy_dbg(MCPY_ERR, "umt_id: %d, umt_mode: %d, msg_mode: %d, enable: %d, phy_dst: %d\n",
			umt_id, umt_mode, msg_mode, enable, phy_dst);
		return -EINVAL;
	}

	port = &pumt->ports[umt_id];

	spin_lock_bh(&port->umt_port_lock);
	if (port->ep_id != ep_id) {
		mcpy_dbg(MCPY_ERR, "input ep_id: %d, port ep_id: %d\n",
			ep_id, port->ep_id);
		spin_unlock_bh(&port->umt_port_lock);
		return -EINVAL;
	}

	umt_reset_umt(umt_id);

	port->umt_mode = (enum umt_mode)umt_mode;
	port->msg_mode = (enum umt_msg_mode)msg_mode;
	port->umt_dst	= phy_dst;
	port->umt_period = period;
	port->status = (enum umt_status)enable;

#ifdef CONFIG_LTQ_UMT_SW_MODE
	if (IS_ENABLED(CONFIG_LTQ_UMT_SW_MODE)) {
		port->umt_ep_dst = phy_dst | KSEG3;
	}
#endif
	umt_set_mode(umt_id, port->umt_mode);
	umt_set_msgmode(umt_id, port->msg_mode);
	umt_set_dst(umt_id, port->umt_dst);
	umt_set_period(umt_id, port->umt_period);
	umt_enable(umt_id, port->status);
	/* setup the CBM/DMA mapping */
	spin_unlock_bh(&port->umt_port_lock);

	return 0;
}
EXPORT_SYMBOL(ltq_umt_set_mode);

/**
 * API to enable/disable umt port
 * input:
 * @umt_id (0 - 3)
 * @ep_id: aligned with datapath lib EP
 * @enable: Enable: 1 / Disable: 0
 * ret:  Fail < 0, Success: 0
 */
int ltq_umt_enable(u32 umt_id, u32 ep_id, u32 enable)
{
	struct mcpy_umt *pumt = mcpy_get_umt();
	struct umt_port *port;

	if (umt_id >= UMT_PORTS_NUM)
		return -EINVAL;
	if (enable >= (u32)UMT_STATUS_MAX
			|| pumt->status != UMT_ENABLE)
		return -ENODEV;

	port = &pumt->ports[umt_id];

	spin_lock_bh(&port->umt_port_lock);
	if (port->ep_id != ep_id || port->umt_dst == 0 || port->ep_id == 0) {
		mcpy_dbg(MCPY_ERR, "input ep_id: %d, umt port ep_id: %d, umt_dst: 0x%x\n",
			ep_id, port->ep_id, port->umt_dst);
		goto en_err;
	}

	if (port->status != enable) {
		port->status = (enum umt_status)enable;
		umt_enable(umt_id, port->status);
	}
	spin_unlock_bh(&port->umt_port_lock);

	return 0;

en_err:
	spin_unlock_bh(&port->umt_port_lock);
	return -EINVAL;
}
EXPORT_SYMBOL(ltq_umt_enable);

/**
 * API to suspend/resume umt US/DS counter
 * input:
 * @umt_id (0 - 3)
 * @ep_id: aligned with datapath lib EP
 * @enable: suspend: 1 / resume: 0
 * ret:  Fail < 0, Success: 0
 */
int ltq_umt_suspend(u32 umt_id, u32 ep_id, u32 enable)
{
	struct mcpy_umt *pumt = mcpy_get_umt();
	struct umt_port *port;

	if (umt_id >= UMT_PORTS_NUM)
		return -EINVAL;
	if (enable >= (u32)UMT_STATUS_MAX
			|| pumt->status != UMT_ENABLE)
		return -ENODEV;

	port = &pumt->ports[umt_id];

	spin_lock_bh(&port->umt_port_lock);
	if (port->ep_id != ep_id || port->umt_dst == 0 || port->ep_id == 0) {
		mcpy_dbg(MCPY_ERR, "input ep_id: %d, umt port ep_id: %d, umt_dst: 0x%x\n",
			ep_id, port->ep_id, port->umt_dst);
		goto en_err;
	}

	if (port->suspend != enable) {
		port->suspend = (enum umt_status)enable;
		umt_enable(umt_id, port->status);
		umt_suspend(umt_id, port->suspend);
	}
	spin_unlock_bh(&port->umt_port_lock);

	return 0;

en_err:
	spin_unlock_bh(&port->umt_port_lock);
	return -EINVAL;
}
EXPORT_SYMBOL(ltq_umt_suspend);

/**
 * API to request and allocate UMT port
 * input:
 * @ep_id: aligned with datapath lib EP.
 * @cbm_pid: CBM Port ID(0-3), 0 - CBM port 4, 1 - CBM port 24,
 * 2 - CBM port 25, 3 - CBM port 26
 * output:
 * @dma_ctrlid: DMA controller ID. aligned with DMA driver DMA controller ID
 * @dma_cid: DMA channel ID.
 * @umt_id: (0 - 3)
 * ret: Fail: < 0,  Success: 0
 */
int ltq_umt_request(u32 ep_id, u32 cbm_pid,
		u32 *dma_ctrlid, u32 *dma_cid, u32 *umt_id)
{
	int i, pid;
	struct mcpy_umt *pumt = mcpy_get_umt();
	struct umt_port *port;

	if (!dma_ctrlid || !dma_cid || !umt_id) {
		mcpy_dbg(MCPY_ERR, "Output pointer is NULL!\n");
		goto param_err;
	}

	if (pumt->status != UMT_ENABLE) {
		mcpy_dbg(MCPY_ERR, "UMT not initialized!\n");
		goto param_err;
	}
	if (!ep_id) {
		mcpy_dbg(MCPY_ERR, "%s: ep_id cannot be zero!\n", __func__);
		goto param_err;
	}

	if (cbm_pid >= UMT_PORTS_NUM) {
		mcpy_dbg(MCPY_ERR, "%s: cbm pid must be in ranage(0 - %d)\n",
			__func__, UMT_PORTS_NUM);
		goto param_err;
	}

	pid = -1;
	spin_lock_bh(&pumt->umt_lock);
	for (i = 0; i < UMT_PORTS_NUM; i++) {
		port = &pumt->ports[i];
		spin_lock_bh(&port->umt_port_lock);
		if (port->ep_id == ep_id && port->cbm_pid == cbm_pid) {
			pid = i;
			spin_unlock_bh(&port->umt_port_lock);
			break;
		} else if (port->ep_id == 0 && pid == -1)
			pid = i;
		spin_unlock_bh(&port->umt_port_lock);
	}
	spin_unlock_bh(&pumt->umt_lock);

	if (pid < 0) {
		mcpy_dbg(MCPY_ERR, "No free UMT port!\n");
		return -ENODEV;
	}

	port = &pumt->ports[pid];
	spin_lock_bh(&port->umt_port_lock);
	port->ep_id = ep_id;
	port->cbm_pid = cbm_pid;
	umt_set_mux(port->umt_pid, port->cbm_pid, port->dma_cid);
	*dma_ctrlid = pumt->dma_ctrlid;
	*dma_cid = port->dma_cid;
	*umt_id = port->umt_pid;
	spin_unlock_bh(&port->umt_port_lock);

#ifdef CONFIG_LTQ_UMT_SW_MODE
       {
               uint32_t flag = 0, cbm_pid_l = 0;
               int ret = 0;
               cbm_dq_port_res_t dqport;

               ret = cbm_get_wlan_umt_pid( ep_id,  &cbm_pid_l);
               if (ret != 0) {
                        mcpy_dbg(MCPY_ERR, "Failed to get cbm port using ep id !\n");
                        goto param_err;
                }

               memset(&dqport, 0, sizeof (dqport));
               ret = cbm_dequeue_port_resources_get(ep_id, &dqport, flag);

               if (ret != 0) {
                        mcpy_dbg(MCPY_ERR, "Failed to get dp port using ep id !\n");
                        goto param_err;
                }
                
                if (dqport.deq_info->port_no == 4 ||
                        dqport.deq_info->port_no == 24 ||
                        dqport.deq_info->port_no == 25 ||
                        dqport.deq_info->port_no == 26)
                {
                        spin_lock_bh(&port->umt_port_lock);
                        if (cbm_pid == cbm_pid_l)
                        {
                                port->umtid_map_cbmid = dqport.deq_info->port_no;
                        } else {
                                port->umtid_map_cbmid = 0;
                        }
                        spin_unlock_bh(&port->umt_port_lock);
                }
                else
                {
                     if (dqport.deq_info) 
			kfree(dqport.deq_info);
                     mcpy_dbg(MCPY_ERR, "port no %d not valid !\n", dqport.deq_info->port_no);
                     goto param_err;
                }

               if (dqport.deq_info) 
			kfree(dqport.deq_info);
       }
#endif

	return 0;

param_err:
	return -EINVAL;
}
EXPORT_SYMBOL(ltq_umt_request);

/**
 * API to release umt port
 * input:
 * @umt_id (0 - 3)
 * @ep_id: aligned with datapath lib EP
 *
 * ret:  Fail < 0, Success: 0
 */
int ltq_umt_release(u32 umt_id, u32 ep_id)
{
	struct mcpy_umt *pumt;
	struct umt_port *port;

	if (umt_id >= UMT_PORTS_NUM)
		return -ENODEV;

	pumt = mcpy_get_umt();
	if (pumt->status != UMT_ENABLE) {
		mcpy_dbg(MCPY_ERR, "UMT is not initialized!\n");
		return -ENODEV;
	}

	port = &pumt->ports[umt_id];

	spin_lock_bh(&port->umt_port_lock);
	if (port->ep_id != ep_id) {
		mcpy_dbg(MCPY_ERR, "input ep_id: %d, UMT port ep_id: %d\n",
			ep_id, port->ep_id);
		spin_unlock_bh(&port->umt_port_lock);

		return -ENODEV;
	} else {
		port->ep_id = 0;
		port->cbm_pid = 0;
		port->umt_dst = 0;
		port->umt_period = 0;
		port->status = UMT_DISABLE;
#ifdef CONFIG_LTQ_UMT_SW_MODE
		port->umt_ep_dst = 0;
		port->umtid_map_cbmid = 0;
#endif
		umt_enable(port->umt_pid, UMT_DISABLE);
	}
	spin_unlock_bh(&port->umt_port_lock);

	return 0;
}
EXPORT_SYMBOL(ltq_umt_release);



static void umt_port_init(struct mcpy_umt *pumt,
		struct device_node *node, int pid)
{
	char res_cid[32];
	int cid;
	struct umt_port *port;

	port = &pumt->ports[pid];
	sprintf(res_cid, "lantiq,umt%d-dmacid", pid);
	if (of_property_read_u32(node, res_cid, &cid) < 0)
		cid = UMT_DEF_DMACID + pid;

	port->pctrl = pumt;
	port->umt_pid = pid;
	port->dma_cid = cid;
	port->ep_id = 0;
	port->status = UMT_DISABLE;
	spin_lock_init(&port->umt_port_lock);
#ifdef CONFIG_LTQ_UMT_SW_MODE
	port->dq_idx = 0;
	port->umt_ep_dst = 0;
	port->umtid_map_cbmid = 0;
#endif
}

static void *umt_port_seq_start(struct seq_file *s, loff_t *pos)
{
	struct mcpy_umt *pumt = s->private;
	struct umt_port *port;

	if (*pos >= UMT_PORTS_NUM)
		return NULL;

	port = &pumt->ports[*pos];

	return port;
}

static void *umt_port_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	struct mcpy_umt *pumt = s->private;
	struct umt_port *port;

	if (++*pos >= UMT_PORTS_NUM)
		return NULL;
	port = &pumt->ports[*pos];
	return port;
}

static void umt_port_seq_stop(struct seq_file *s, void *v)
{

}

static int umt_port_seq_show(struct seq_file *s, void *v)
{
	struct umt_port *port = v;
	int pid = port->umt_pid;
	u32 val;

	seq_printf(s, "\nUMT port %d configuration\n", pid);
	seq_puts(s, "-----------------------------------------\n");
	seq_printf(s, "UMT port ep_id: %d\n", port->ep_id);
	seq_printf(s, "UMT Mode: \t%s\n",
		port->umt_mode == UMT_SELFCNT_MODE ?
		"UMT SelfCounting Mode" : "UMT User Mode");
	switch (port->msg_mode) {
	case UMT_NO_MSG:
		seq_puts(s, "UMT MSG Mode: \tUMT NO MSG\n");
		break;
	case UMT_MSG0_ONLY:
		seq_puts(s, "UMT MSG Mode: \tUMT MSG0 Only\n");
		break;
	case UMT_MSG1_ONLY:
		seq_puts(s, "UMT MSG Mode: \tUMT MSG1 Only\n");
		break;
	case UMT_MSG0_MSG1:
		seq_puts(s, "UMT MSG Mode: \tUMT_MSG0_And_MSG1\n");
		break;
	default:
		seq_printf(s, "UMT MSG Mode Error! Msg_mode: %d\n",
			port->msg_mode);
	}
	seq_printf(s, "UMT DST: \t0x%x\n", port->umt_dst);
	if (port->umt_mode == UMT_SELFCNT_MODE)
		seq_printf(s, "UMT Period: \t%d(us)\n", port->umt_period);
	seq_printf(s, "UMT Status: \t%s\n",
			port->status == UMT_ENABLE ? "Enable" :
			port->status == UMT_DISABLE ? "Disable" : "Init Fail");
	seq_printf(s, "UMT DMA CID: \t%d\n", port->dma_cid);
	seq_printf(s, "UMT CBM PID: \t%d\n", port->cbm_pid);

	seq_printf(s, "++++Register dump of umt port: %d++++\n", pid);
	if (pid == 0) {
		seq_printf(s, "UMT Status: \t%s\n",
			(ltq_mcpy_r32(MCPY_GCTRL) & BIT(2)) != 0 ?
			"Enable" : "Disable");
		seq_printf(s, "UMT Mode: \t%s\n",
			(ltq_mcpy_r32(MCPY_GCTRL) & BIT(1)) != 0 ?
			"UMT User MSG mode" : "UMT SelfCounting mode");
		seq_printf(s, "UMT MSG Mode: \t%d\n",
			ltq_mcpy_r32(MCPY_UMT_SW_MODE));
		seq_printf(s, "UMT Dst: \t0x%x\n",
			ltq_mcpy_r32(MCPY_UMT_DEST));
		seq_printf(s, "UMT Period: \t0x%x\n",
			ltq_mcpy_r32(MCPY_UMT_PERD));
		seq_printf(s, "UMT MSG0: \t0x%x\n",
			ltq_mcpy_r32(MCPY_UMT_MSG(0)));
		seq_printf(s, "UMT MSG1: \t0x%x\n",
			ltq_mcpy_r32(MCPY_UMT_MSG(1)));
	} else {
		seq_printf(s, "UMT Status: \t%s\n",
			(ltq_mcpy_r32(MCPY_GCTRL) &
				BIT(17 + 3 * (pid - 1))) != 0 ?
			"Enable" : "Disable");
		seq_printf(s, "UMT Mode: \t%s\n",
			(ltq_mcpy_r32(MCPY_GCTRL) &
				BIT(16 + 3 * (pid - 1))) != 0 ?
			"UMT User MSG mode" : "UMT SelfCounting mode");
		seq_printf(s, "UMT MSG Mode: \t%d\n",
			ltq_mcpy_r32(MCPY_UMT_X_ADDR(pid, MCPY_UMT_XSW_MODE)));
		seq_printf(s, "UMT Dst: \t0x%x\n",
			ltq_mcpy_r32(MCPY_UMT_X_ADDR(pid, MCPY_UMT_XDEST)));
		seq_printf(s, "UMT Period: \t0x%x\n",
			ltq_mcpy_r32(MCPY_UMT_X_ADDR(pid, MCPY_UMT_XPERIOD)));
		seq_printf(s, "UMT MSG0: \t0x%x\n",
			ltq_mcpy_r32(MCPY_UMT_X_ADDR(pid, MCPY_UMT_XMSG(0))));
		seq_printf(s, "UMT MSG1: \t0x%x\n",
			ltq_mcpy_r32(MCPY_UMT_X_ADDR(pid, MCPY_UMT_XMSG(1))));
	}

	val = ltq_mcpy_r32(MCPY_UMT_TRG_MUX);
	seq_printf(s, "DMA CID: \t%d\n",
		(val & ((0xF) << (pid * 4))) >> (pid * 4));
	seq_printf(s, "CBM PID: \t%d\n",
		(val & ((0xF) << (16 + pid * 4))) >> (16 + pid * 4));

	return 0;
}


static const struct seq_operations umt_port_seq_ops = {
	.start = umt_port_seq_start,
	.next = umt_port_seq_next,
	.stop = umt_port_seq_stop,
	.show = umt_port_seq_show,
};

static int umt_cfg_read_proc_open(struct inode *inode, struct file *file)
{
	int ret = seq_open(file, &umt_port_seq_ops);

	if (ret == 0) {
		struct seq_file *m = file->private_data;
		m->private = PDE_DATA(inode);
	}
	return ret;
}

static const struct file_operations mcpy_umt_proc_fops = {
	.open           = umt_cfg_read_proc_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = seq_release,
};

#ifdef CONFIG_LTQ_UMT_SW_MODE
#define MICROSEC_TO_SEC(x) (1000000/x)
extern u32 jiffies1;

static int umt_tc_info_read_proc(struct seq_file *s, void *v)
{
	u32 days, sec, min, hr;
	struct umt_port *port = NULL;
	struct mcpy_umt *pumt = mcpy_get_umt();
	int i = 0;

	sec = jiffies1 / MICROSEC_TO_SEC(50);

	if (sec >= 60) {
		min = sec / 60;
		sec = sec - (min * 60);
	} else
		min = 0;

	if (min >= 60) {
		hr = min / 60;
		min = min - (hr * 60);
	} else {
		hr = 0;
	}

	if (hr >= 24) {
		days = hr / 24;
		hr = hr - (days * 24);
	} else {
		days = 0;
	}

	seq_printf(s, "Jiffies : %08d\n", jiffies1);
	seq_printf(s, "Uptime(d:h:m:s): %02d:%02d:%02d:%02d\n", days, hr, min, sec);

	for (i = 0 ; i < UMT_PORTS_NUM ; i ++) {
		port = &pumt->ports[i];
		seq_printf(s, "Packets to be dequeued: %d  cbm id %d \n", g_tot_dq_cnt[i], port->umtid_map_cbmid);
		seq_printf(s, "thread info: umt dst: 0x%x, interval: 0x%x, en: %d, dq_idx: %u, ep_dst:0x%x\n",
			port->umt_dst,g_umt_interval, port->status, port->dq_idx, port->umt_ep_dst);
	}
	return 0;
}

static int umt_tc_info_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, umt_tc_info_read_proc, PDE_DATA(inode));
}

static const struct file_operations umt_tc_info_proc_fops = {
	.owner      = THIS_MODULE,
	.open = umt_tc_info_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

/* UMT interval related proc */
static int umt_interval_read_proc(struct seq_file *s, void *v)
{
	seq_printf(s, "UMT interval: %d\n", g_umt_interval);
	return 0;
}

static ssize_t proc_write_umt_interval(struct file *file, const char __user *buf, size_t count, loff_t *data)
{
    int len, tmp=0;
    char str[64];
    char *p;
	struct clk *gptc_clk;
	u32 rate;

    len = min(count, (size_t)(sizeof(str) - 1));
    len -= copy_from_user(str, buf, len);
    while ( len && str[len - 1] <= ' ' )
        len--;
    str[len] = 0;
    for ( p = str; *p && *p <= ' '; p++, len-- );
    if ( !*p )
        return count;

    strict_strtol(p, 10, (long*)&tmp);
	g_umt_interval = tmp;

	/* restart the GPTU with new values */
	gptc_clk = clk_get_sys("16300000.gptu", "timer3a");
	if (IS_ERR(gptc_clk)) {
		pr_err("failed to get clock timer3a\r\n");
		goto out;
	}
	clk_disable(gptc_clk);

	rate = (1000 * 1000)/g_umt_interval;
	pr_info("setting the umt interval to: %d\n", (unsigned int)g_umt_interval);
	if (g_umt_interval != LTQ_UMT_SW_INTERVAL_DEFAULT)
		pr_info("warning... recommend value is : %d\n", LTQ_UMT_SW_INTERVAL_DEFAULT);
	pr_info("setting the gptu rate to: %d\n", (unsigned int)rate);
	clk_set_rate(gptc_clk, rate);

	if (clk_enable(gptc_clk)) {
		pr_err("%s enable failed\r\n", __func__);
		clk_set_rate(gptc_clk, 0);
	} else {
		pr_info("%s Timer 3A is restarted for UMT !\n", __func__);
	}
out:
	return len;
}

static int umt_interval_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, umt_interval_read_proc, PDE_DATA(inode));
}

static const struct file_operations umt_interval_proc_fops = {
    	.owner      = THIS_MODULE,
	.open = umt_interval_proc_open,
	.read = seq_read,
   	.write      = proc_write_umt_interval,
	.llseek = seq_lseek,
	.release = seq_release,
};

#endif

static int umt_proc_init(struct mcpy_umt *pumt)
{
	struct proc_dir_entry *entry;

	pumt->proc = proc_mkdir("umt", pumt->ctrl->proc);
	if (!pumt->proc)
		return -ENOMEM;

	entry = proc_create_data("umt_info", 0, pumt->proc,
			&mcpy_umt_proc_fops, pumt);
	if (!entry)
		goto err1;

#ifdef CONFIG_LTQ_UMT_SW_MODE
	entry = proc_create_data("umt_tc_info", 0, pumt->proc,
			&umt_tc_info_proc_fops, pumt);
	if (!entry)
		goto err2;
	entry = proc_create_data("umt_interval", 0, pumt->proc,
			&umt_interval_proc_fops, pumt);
	if (!entry)
		goto err3;
#endif

	return 0;

#ifdef CONFIG_LTQ_UMT_SW_MODE
err3:
	remove_proc_entry("umt_tc_info", pumt->ctrl->proc);
err2:
	remove_proc_entry("umt_info", pumt->ctrl->proc);
#endif
err1:
	remove_proc_entry("umt", pumt->ctrl->proc);
	mcpy_dbg(MCPY_ERR, "UMT proc create fail!\n");
	return -1;
}

#ifdef CONFIG_LTQ_UMT_SW_MODE
typedef unsigned long               reg32_t;

#define _m32c0_mfc0(reg, sel) \
__extension__ ({ \
  register unsigned long __r; \
  __asm__ __volatile ("mfc0 %0,$%1,%2" \
		      : "=d" (__r) \
      		      : "JK" (reg), "JK" (sel)); \
  __r; \
})

#define _m32c0_mtc0(reg, sel, val) \
do { \
    __asm__ __volatile ("%(mtc0 %z0,$%1,%2; ehb%)" \
			: \
			: "dJ" ((reg32_t)(val)), "JK" (reg), "JK" (sel) \
			: "memory"); \
} while (0)

#define _m32c0_mftc0(rt,sel)                                                   \
({                                                                      \
         unsigned long  __res;                                          \
                                                                        \
        __asm__ __volatile__(                                           \
        "       .set    push                                    \n"     \
        "       .set    mips32r2                                \n"     \
        "       .set    noat                                    \n"     \
        "       # mftc0 $1, $" #rt ", " #sel "                  \n"     \
        "       .word   0x41000800 | (" #rt " << 16) | " #sel " \n"     \
        "       move    %0, $1                                  \n"     \
        "       .set    pop                                     \n"     \
        : "=r" (__res));                                                \
                                                                        \
        __res;                                                          \
})

#define _m32c0_mttc0(rd, sel, v)                                                       \
({                                                                      \
        __asm__ __volatile__(                                           \
        "       .set    push                                    \n"     \
        "       .set    mips32r2                                \n"     \
        "       .set    noat                                    \n"     \
        "       move    $1, %0                                  \n"     \
        "       # mttc0 %0," #rd ", " #sel "                    \n"     \
        "       .word   0x41810000 | (" #rd " << 11) | " #sel " \n"     \
        "       .set    pop                                     \n"     \
        :                                                               \
        : "r" (v));                                                     \
})

/* move to gpr */
#define _m32c0_mttgpr(rd,v) \
do {									\
	__asm__ __volatile__(						\
	"	.set	push					\n"	\
	"	.set	mips32r2				\n"	\
	"	.set	noat					\n"	\
	"	move	$1, %0					\n"	\
	"	# mttgpr $1, " #rd "				\n"	\
	"	.word	0x41810020 | (" #rd " << 11)		\n"	\
	"	.set	pop					\n"	\
	: : "r" (v));							\
} while (0)

#define mips32_setmvpcontrol(x)	    _m32c0_mtc0(0,1,x)
#define mips32_getmvpcontrol()	     _m32c0_mfc0(0,1)
#define mips32_mt_settcstatus(val)	mttc0(2, 1, val)
#define mips32_getvpecontrol()	        _m32c0_mfc0(1,1)
#define mips32_setvpecontrol(x)	        _m32c0_mtc0(1,1,x)

#define VPECONTROL_TARGTC1	0x000000ff	
#define  VPECONTROL_TARGTC_SHIFT	 0	

/*
 * targeted VPE register macros
 */
#define mips32_mt_settarget(tc) \
	mips32_setvpecontrol ( \
		(mips32_getvpecontrol () & ~(VPECONTROL_TARGTC)) \
		| ((tc) << VPECONTROL_TARGTC_SHIFT))

#define mips32_mt_settchalt(val)	_m32c0_mttc0(2, 4, val)
#define mips32_mt_gettchalt()	_m32c0_mftc0(2, 4)
#define mips32_mt_settcbind(val)	_m32c0_mttc0(2, 2, val)
#define mips32_mt_gettcbind()		_m32c0_mftc0(2, 2)
#define mips32_mt_setsp(val)		_m32c0_mttgpr(29, val)
#define mips32_mt_settcrestart(val)	_m32c0_mttc0(2, 3, val)
#define mips32_mt_gettcrestart()	_m32c0_mftc0(2, 3)
#define mips32_mt_gettcstatus()     _m32c0_mftc0(2, 1)
#define mips32_mt_getvpecontrol()   _m32c0_mftc0(1, 1)
#define mips32_mt_setvpecontrol(val)    _m32c0_mttc0(1, 1, val)

void run_tc(u32 tc_num,
	    u32 vpe_num,
	    u32 start_addr,
	    u32 stack_top,
	    u32 arg0,
	    u32 arg1)
{
    mips32_setmvpcontrol((mips32_getmvpcontrol() & ~MVPCONTROL_EVP) | MVPCONTROL_VPC); // VPC=1 EVP=0.
	asm("ehb");
	pr_info("getmvpctrl = %x\n", (unsigned int)mips32_getmvpcontrol());

    // set the target to tc_num
    mips32_mt_settarget(tc_num); // Have all 'mips32_mt_*' functions target tc1.
	asm("ehb");
	pr_info("getvpectrl for target TC = %x\n", (unsigned int)mips32_getvpecontrol());

    // set DA bit so yields will work.
    mips32_mt_settcstatus(mips32_mt_gettcstatus() | TCSTATUS_DA);
	asm("ehb");
	pr_info("gettcstatus = %x\n", (unsigned int)mips32_mt_gettcstatus());

    // Make sure tc is halted. (This done in h/w as part of reset.)
    mips32_mt_settchalt(TCHALT_H); // H=1.
	asm("ehb");
	pr_info("tc is halted = %x\n", (unsigned int)mips32_mt_gettchalt());

    // Make sure tc is bound to vpe. (This is done in h/w as part of reset.)
    mips32_mt_settcbind ((vpe_num << TCBIND_CURVPE_SHIFT) | (tc_num << TCBIND_CURTC_SHIFT));
	asm("ehb");
	pr_info("gettcbind = %x\n", (unsigned int)mips32_mt_gettcbind());

    // Set the stack pointer so we can call a c function
    mips32_mt_setsp(stack_top);
	asm("ehb");
	pr_info("stack top = %x\n", stack_top);

    //_m32c0_mttgpr(25, start_addr);
	//asm("ehb");

    // Set the global pointer
     _m32c0_mttgpr(28, &umt_tc_thread_gp);
	asm("ehb");

    if(arg0)
    {
        // Pass the Arg0
        _m32c0_mttgpr(4, arg0);
	asm("ehb");
    }

    if(arg1)
    {
        // Pass the Arg1
        _m32c0_mttgpr(5, arg1);
		asm("ehb");
    }

    // Point tc to code
    mips32_mt_settcrestart(start_addr);
	asm("ehb");
	pr_info("start address of TC = %x\n", (unsigned int)mips32_mt_gettcrestart());

    mips32_mt_settcstatus(mips32_mt_gettcstatus() | (TCSTATUS_A | TCSTATUS_DA));
	asm("ehb");
	pr_info("current TC status = %x\n", (unsigned int)mips32_mt_gettcstatus());

        // Unhalt tc.
    mips32_mt_settchalt(0); // H=0 (allow vpe_num/tc_num to execute).
	asm("ehb");
	pr_info("tc is unhalted = %x\n", (unsigned int)mips32_mt_gettchalt());

    // Enable threading.
    mips32_mt_setvpecontrol(mips32_mt_getvpecontrol() | VPECONTROL_TE); // TE=1.

    // Turn off the vpe configuration flag and enable (other) vpe.
    mips32_setmvpcontrol((mips32_getmvpcontrol() & ~MVPCONTROL_VPC) | MVPCONTROL_EVP); // VPC=0 EVP=1.
	asm("ehb");
	pr_info("current mvp control = %x\n", (unsigned int)mips32_getmvpcontrol());

    return;
}

static inline void set_yield_mask(void *mask)
{
	u32 yqmask = read_c0_yqmask();

	yqmask |= (*(u32 *)mask);
	write_c0_yqmask(yqmask);
}


void umt_callback_fn(struct thrd_param *param)
{
	u32 dq_cnt, dq_ptr;
	int i = 0, ret = 0;
	struct umt_port *port = NULL;
	struct mcpy_umt *pumt = mcpy_get_umt();
	u32 umt_ep_dst_local = 0;

	if (pumt->status != UMT_ENABLE)
		return ;

	for (i = 0; i < UMT_PORTS_NUM; i++) {
		port = &pumt->ports[i];
		umt_ep_dst_local = port->umt_ep_dst;
		if (port->status == UMT_ENABLE && umt_ep_dst_local && port->umtid_map_cbmid)
		{
			ret = cbm_dequeue_dma_port_stats_get(port->umtid_map_cbmid, &dq_ptr, 0);
			if (ret != 0)
				continue;
			if (dq_ptr >= port->dq_idx)
				dq_cnt = dq_ptr - port->dq_idx;
			else
				dq_cnt = 0xFFFFFFFF - port->dq_idx + dq_ptr + 1;
			
			if (dq_cnt)
			{
				if (dq_cnt>32)
					dq_cnt = 32;
				writel(dq_cnt, (void * __force)(umt_ep_dst_local + 0x4));
				g_tot_dq_cnt[i]+=dq_cnt;
				port->dq_idx = port->dq_idx + dq_cnt;
			}
		}
	}
}

static inline u8 umt_process_gptc(u32 gptc_num, struct thrd_param *param)
{
	if (CHECK_BIT(REG32(gptc_pmask), gptc_maskbit)) {
		jiffies1++;
		// Clear the Interrupt Status Register
		REG32(GPTC1_IRNCR) = ((u32)0x1 << gptc_num);
		//REG32(GIC_SH_WEDGE_REG) = (GPTC_1A_GIC_IRQ-8);
		gic_clear_edge(GPTC_1A_GIC_IRQ-8);

		/* Do the UMT stuff */
		umt_callback_fn(param);
	}
	return 1;
}

asmlinkage
static void umt_tc_thread (u32 arg0, u32 arg1)
{
	u32 mask = (1 << 8);
	struct thrd_param *param = (struct thrd_param *) arg0;
	//struct thrd_param *param = &g_umt_thread_info;

	/* init the yq mask */
	set_yield_mask(&mask);

	jiffies1 = 0;

	printk( ":::: UMT FLOW started :::::\n");

	while (1) {
		mips_mt_yield(1<<8);
		umt_process_gptc(GPTC_3A, param);
	}	

}
static irqreturn_t umt_tc_dummy_isr(int irq, void *dev_id)
{
	return IRQ_HANDLED;
}

int umt_reset_port_dq_idx(u32 cbm_id)
{
	struct mcpy_umt *pumt;
	struct umt_port *port;
	u32 umt_id = 0;

        pumt = mcpy_get_umt();

        if (pumt->status != UMT_ENABLE) {
                mcpy_dbg(MCPY_ERR, "UMT is not initialized!\n");
                return -ENODEV;
        }

        for (umt_id = 0; umt_id < UMT_PORTS_NUM; umt_id++) {
                port = &pumt->ports[umt_id];
                if (port->umtid_map_cbmid == cbm_id)
                        port->dq_idx = 0;
        }

	return 0;
}

#endif

/* TODO: Register UMT error interrupt Handler */
int umt_init(struct mcpy_ctrl *pctrl)
{
	struct device_node *node = pctrl->dev->of_node;
	struct mcpy_umt *pumt;
	int i;
#ifdef CONFIG_LTQ_UMT_SW_MODE
	int tc_num;
	struct clk *gptc_clk;
	u32 rate;
#endif
	mcpy_dbg(MCPY_INFO, "UMT initialize called on processor: %d !\n", smp_processor_id());

	pumt = &pctrl->umt;
	pumt->ctrl = pctrl;
	pumt->dma_ctrlid = g_dma_ctrl;
	spin_lock_init(&pumt->umt_lock);
	umt_en_expand_mode();

	for (i = 0; i < UMT_PORTS_NUM; i++)
		umt_port_init(pumt, node, i);

	umt_proc_init(pumt);
	pumt->status = UMT_ENABLE;

#ifdef CONFIG_LTQ_UMT_SW_MODE
	gptc_maskbit = GPTC_1A_MASK_BIT((GPTC_1A_GIC_IRQ-8));
	gptc_pmask = GPTC_1A_PMASK((GPTC_1A_GIC_IRQ-8));

	/* fill the info needed by thread */
	g_umt_thread_info.dummy = 0;

	pr_info("<%s>pmask addr = %x and bit = %d\n", __FUNCTION__,gptc_pmask, gptc_maskbit);
	/* start the thread on TC */
	tc_num = vmb_tc_alloc(smp_processor_id());
	pr_info("<%s>allocated TC num %d\n", __FUNCTION__,tc_num);
	run_tc(tc_num, smp_processor_id(), (u32) &umt_tc_thread, (u32)&umt_tc_thread_stack + 4080, (u32)&g_umt_thread_info, 0);

	/* setup the interrupt and yeild */
	if (request_irq(GPTC_1A_GIC_IRQ, umt_tc_dummy_isr, IRQF_DISABLED, "umt_gptc_irq", NULL)) {
		pr_err("%s: failed to request gptu irq - ", __func__);
		return -ENOENT;
	}

	if (gic_yield_setup(smp_processor_id(), 8, GPTC_1A_GIC_IRQ)) {
		pr_info("Yield Setup Fail:\n");
		return -ENOENT;
	} else {
		pr_info("Yield setup passed for gptc_irq = %d\n", GPTC_1A_GIC_IRQ);
	}

	/* start the GPTC timer */
	gptc_clk = clk_get_sys("16300000.gptu", "timer3a");
	if (IS_ERR(gptc_clk)) {
		pr_err("failed to get clock timer3a\r\n");
		return -ENOENT;
	}
	rate = (1000 * 1000)/g_umt_interval;
	pr_info("setting the gptu rate to: %d\n", (unsigned int)rate);
	clk_set_rate(gptc_clk, rate);
	if (clk_enable(gptc_clk)) {
		pr_err("%s enable failed\r\n", __func__);
		clk_set_rate(gptc_clk, 0);
	} else {
		pr_info("%s Timer 3A is running for UMT !\n", __func__);
	}
#endif

	mcpy_dbg(MCPY_INFO, "UMT initialize success on processor: %d !\n", smp_processor_id());

	return 0;
}



