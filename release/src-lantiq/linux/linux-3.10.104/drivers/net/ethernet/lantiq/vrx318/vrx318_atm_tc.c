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
#include <lantiq_atm.h>

#if defined(CONFIG_LTQ_PPA_API) || defined(CONFIG_LTQ_PPA_API_MODULE)
#include <net/ppa_api.h>
#endif

#include "include/vrx318_ppe_fw_ds.h"
#include "include/vrx318_ppe_atm_ds.h"
#include "include/vrx318_fw_prereq.h"
#include "include/vrx318_a1plus_addr_def.h"
#include "include/vrx318_ppe_fw_const.h"
#include "include/unified_qos_ds_be.h"
#include "include/vrx318_edma.h"
#include "include/vrx318_common.h"
#include "include/vrx318_framework.h"
#include "include/vrx318_atm_tc.h"
#include "include/vrx318_api.h"
#include "include/vrx318_proc.h"

#include "fw/vrx318_a1plus.h"

static struct vrx318_atm_tc *g_atm_tc;
static const char *g_atm_dev_name = "vrx318_atm";

static void do_oam_tasklet(unsigned long);
static DECLARE_TASKLET(g_oam_tasklet, do_oam_tasklet, 0);
#ifdef CONFIG_LTQ_VRX318_CPUFREQ
static void do_coc_tasklet(unsigned long);
static DECLARE_TASKLET(g_coc_tasklet, do_coc_tasklet, 0);
#endif
static int ppe_open(struct atm_vcc *);
static void ppe_close(struct atm_vcc *);
static int ppe_ioctl(struct atm_dev *, unsigned int, void *);
static int ppe_send(struct atm_vcc *, struct sk_buff *);
static int ppe_send_oam(struct atm_vcc *, void *, int);
static int ppe_change_qos(struct atm_vcc *, struct atm_qos *, int);
static int find_vpi(unsigned int);
static int find_vcc(struct atm_vcc *);
static void qsb_global_set(struct vrx318_atm_tc *, u32);
static u32 get_qsb_clk(u32);



static struct atmdev_ops g_ppe_atm_ops = {
	.owner =	THIS_MODULE,
	.open =		ppe_open,
	.close =	ppe_close,
	.ioctl =	ppe_ioctl,
	.send =		ppe_send,
	.send_oam =	ppe_send_oam,
	.change_qos =	ppe_change_qos,
};

#ifdef CONFIG_LTQ_VRX318_CPUFREQ
enum ltq_cpufreq_state atm_coc_state = LTQ_CPUFREQ_PS_D0;
#endif

static inline void dump_oam_cell(void *cell, int rx)
{
	if (rx) {
		if (g_vrx318_dbg & DBG_OAM_RX_DUMP)
			dp_dump_raw_data(cell, CELL_SIZE, "OAM RX Cell");
	} else {
		if (g_vrx318_dbg & DBG_OAM_TX_DUMP)
			dp_dump_raw_data(cell, CELL_SIZE, "OAM TX Cell");
	}
}

int max_pvc_num(u32 qos_en)
{
	return ATM_TXQ_NUM(qos_en);
}

/*Note: FW RXQ number is 16, TXQ number is 15 */
static inline int txq_num(void)
{
	return ATM_TXQ_NUM(g_atm_tc->frwk->param.atm_qos);
}

static inline int pvc_num(void)
{
	return ATM_PVC_NUM(g_atm_tc->frwk->param.atm_qos);
}

void vrx318_atm_param_setup(struct vrx318_priv *priv, int qsb_tau, int qsb_srvm,
			int qsb_tstep, int aal5r_max_pktsz,
			int aal5r_min_pktsz, int aal5s_max_pktsz)
{
	struct vrx318_param *cfg = &priv->param;

	cfg->qsb_tau = qsb_tau & 0x3F;
	cfg->qsb_srvm = qsb_srvm & 0x0F;
	cfg->qsb_tstep = qsb_tstep;
	cfg->aal5r_max_pktsz = aal5r_max_pktsz < DMA_PACKET_SIZE
		? aal5r_max_pktsz : DMA_PACKET_SIZE;
	cfg->aal5r_min_pktsz = aal5r_min_pktsz < 0 ? 0 : aal5r_min_pktsz;
	cfg->aal5s_max_pktsz = aal5s_max_pktsz < DMA_PACKET_SIZE
		? aal5s_max_pktsz : DMA_PACKET_SIZE;
}

/**
 *  Description:
 *    Loop up for connection ID with virtual path ID and virtual channel ID.
 *  Input:
 *    vpi --- unsigned int, virtual path ID
 *    vci --- unsigned int, virtual channel ID
 *  Output:
 *    int --- negative value: failed
 *            else          : connection ID
 */
int find_vpivci(unsigned int vpi, unsigned int vci)
{
	int i;
	struct atm_pvc *conn = g_atm_tc->conn;
	int max_pvc = pvc_num();

	for (i = 0; i < max_pvc; i++)
		if ((g_atm_tc->pvc_tbl & BIT(i))
			&& conn[i].vcc != NULL
			&& vpi == conn[i].vcc->vpi
			&& vci == conn[i].vcc->vci)
			return i;

	return -ENODEV;
}

/**
 * Description:
 *   Loop up for connection ID with virtual path ID.
 * Input:
 *   vpi --- unsigned int, virtual path ID
 * Output:
 *   int --- negative value: failed
 *   else          : connection ID
 */
static int find_vpi(unsigned int vpi)
{
	int i;
	struct atm_pvc *conn = g_atm_tc->conn;
	int max_pvc = pvc_num();

	for (i = 0; i < max_pvc; i++)
		if ((g_atm_tc->pvc_tbl & (1 << i)) != 0
			&& conn[i].vcc != NULL
			&& vpi == conn[i].vcc->vpi)
			return i;

	return -ENODEV;
}

/**
 *  ps_en:  1 - Enable powersaving, 0 -Disable Powersaving
 *  flag:   0 - Get the powersaving cfg, 1 - Set the powersaving
 */
int atm_power_saving(struct vrx318_atm_tc *priv, int ps_en, int flag)
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

int atm_in_showtime(void)
{
	if (g_atm_tc->frwk->param.dfe_loopback)
		return 1;

	return g_atm_tc->frwk->show_time_stat[g_atm_tc->ep_id];
}

static inline void get_fw_version(struct fw_ver_id *ver, u32 base)
{
	u32 version;

	version = vrx318_r32_sb(__FW_VER_ID, base);
	*ver = *(struct fw_ver_id *)&version;
}

void vrx318_atm_pmac_cfg_init(int pid)
{
	struct dp_pmac_cfg pmac_cfg;

	memset(&pmac_cfg, 0, sizeof(pmac_cfg));

	pmac_cfg.eg_pmac_flags
		= EG_PMAC_F_PMAC | EG_PMAC_F_FCS | EG_PMAC_F_TCENA;

	pmac_cfg.eg_pmac.pmac = 0;
	pmac_cfg.eg_pmac.fcs = 0;
	pmac_cfg.eg_pmac.tc_enable = 1;

	pmac_cfg.ig_pmac_flags = IG_PMAC_F_PRESENT | IG_PMAC_F_SPID
				| IG_PMAC_F_PMACHDR3 | IG_PMAC_F_PMACHDR5;

	pmac_cfg.ig_pmac.pmac = 0;
	pmac_cfg.ig_pmac.def_pmac_src_port = 1;
	pmac_cfg.ig_pmac.def_pmac_hdr[2] = (pid & 0xF) << 4;
	pmac_cfg.ig_pmac.def_pmac_hdr[4] = 0; /* Packet has no FCS */

	dp_pmac_set(pid, &pmac_cfg);
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

static void atm_free_tx_skb_vcc(struct sk_buff *skb)
{
	struct atm_vcc *vcc;

	vcc = ATM_SKB(skb)->vcc;

	if (vcc != NULL && vcc->pop != NULL) {
		VRX318_ASSERT(atomic_read(&skb->users) != 0,
			"atm_free_tx_skb_vcc(vcc->pop): skb->users == 0, skb = %08X",
			(unsigned int)skb);
		vcc->pop(vcc, skb);
	} else {
		VRX318_ASSERT(atomic_read(&skb->users) != 0,
			"atm_free_tx_skb_vcc(dev_kfree_skb_any): skb->users == 0, skb = %08X",
			(unsigned int)skb);
		dev_kfree_skb_any(skb);
	}
}

static void pp32_load(u32 base)
{
	int ret;

	/* Download firmware */
	vrx318_dbg(DBG_INIT, "Download A1 FW\n");
	ret = pp32_download_code(base, 0,
		vrx218_a1plus_fw_code, ARRAY_SIZE(vrx218_a1plus_fw_code),
		vrx218_a1plus_fw_data, ARRAY_SIZE(vrx218_a1plus_fw_data));
	if (ret) {
		vrx318_dbg(DBG_ERR, "Download A1 FW code fail\n");
		BUG();
	}
}

static void set_qsb_clk(u32 div, u32 base)
{
	u32 clk_val;
	u32 divisor = 0;

	if (div > 8) {
		vrx318_dbg(DBG_ERR, "QSB CLK divisor invailid: %d\n", div);
		return;
	}

	clk_val = vrx318_r32(CGU_CLKFSR, base);
	for (; div > 1; div >>= 1)
		divisor += 1;

	clk_val |= divisor << 28;
	vrx318_w32(clk_val, CGU_CLKFSR, base);
}

static int __init vrx318_atm_init(struct vrx318_atm_tc *priv)
{
	int i, err;
	for (i = 0; i < ATM_PORT_NUMBER; i++) {
		priv->port[i].tx_max_cell_rate = DEFAULT_CELL_RATE;
		priv->port[i].dev = atm_dev_register(g_atm_dev_name,
				priv->frwk->pdev, &g_ppe_atm_ops, -1, 0UL);
		if (!priv->port[i].dev) {
			err = -EIO;
			goto ATM_DEV_REGISTER_FAIL;
		} else {
			priv->port[i].dev->ci_range.vpi_bits = 8;
			priv->port[i].dev->ci_range.vci_bits = 16;
			/* assume 3200 cell rate before get real information*/
			priv->port[i].dev->link_rate = DEFAULT_CELL_RATE;
			priv->port[i].dev->dev_data = priv;
			priv->port[i].dev->phy_data = (void *)(unsigned long)i;
			atm_dev_signal_change(priv->port[i].dev, ATM_PHY_SIG_LOST);
		}
	}

	return 0;

ATM_DEV_REGISTER_FAIL:
	vrx318_dbg(DBG_ERR, "ATM device registration fail\n");
	for (i = 0; i < ATM_PORT_NUMBER; i++)
		if (priv->port[i].dev)
			atm_dev_deregister(priv->port[i].dev);

	return err;
}

/* Note: Upper interface must be deleted by system before exit */
void vrx318_atm_exit(void)
{
	struct vrx318_atm_tc *priv = g_atm_tc;
	int i;

	if (!priv)
		return;

	for (i = 0; i < ATM_PORT_NUMBER; i++) {
		if (priv->port[i].dev)
			atm_dev_deregister(priv->port[i].dev);
	}

	kfree(priv);
	g_atm_tc = NULL;

	vrx318_dbg(DBG_INFO, "VRX318 ATM exited!\n");
}


static inline void vrx318_atm_pdma_init(u32 base)
{
	vrx318_w32(0x00000008, PPE_PDMA_CFG, base);
	vrx318_w32(0x00203580, PPE_SAR_PDMA_RX_CMDBUF_CFG, base);
	vrx318_w32(0x004035A0, PPE_SAR_PDMA_RX_FW_CMDBUF_CFG, base);

	return;
}

static void vrx318_atm_fw_init(u32 base)
{
	qos_cfg_t qos_cfg;
	struct psave_cfg ps_cfg;

	vrx318_w32(0x3C30, DS_FLOW_CTRL_CFG, base);

	dword_clear(&qos_cfg, sizeof(qos_cfg));
	/* PPE FW use this timer to wake up in the sleep mode.
	    It's the only way to wake up FW in the ATM mode.
	*/
	/* cgu_get_pp32_clock() / 62500 / 3 */
	qos_cfg.time_tick = get_ppe_clk(base) / (62500 * 3);
	dword_mem_write(SB_BUFFER(__QOS_CFG), &qos_cfg, sizeof(qos_cfg), base);

	dword_clear(&ps_cfg, sizeof(ps_cfg));
	ps_cfg.start_state = 0;
	ps_cfg.sleep_en = 0;  /* Disable sleep mode by default */
	dword_mem_write(SB_BUFFER(__PSAVE_CFG), &ps_cfg, sizeof(ps_cfg), base);

	vrx318_w32_sb(15 + 3, __CFG_WRX_HTUTS, base);
	vrx318_w32_sb(0x03, __CFG_WRX_DMACH_ON, base);
	vrx318_w32_sb(4, __CFG_WRX_HUNT_BITTH, base);
	vrx318_w32_sb(1, __SOC_FAMILY, base);

	return;
}

static void vrx318_atm_htu_init(u32 base)
{
	/* OAM_F4_SEG_HTU_ENTRY */
	vrx318_w32_sb(0x00000031, HTU_ENTRY_TBL(OAM_F4_SEG_HTU_ENTRY), base);
	vrx318_w32_sb(0xfff0000e, HTU_MASK_TBL(OAM_F4_SEG_HTU_ENTRY), base);
	vrx318_w32_sb(0x00000600, HTU_RESULT_TBL(OAM_F4_SEG_HTU_ENTRY), base);

	/* OAM_F4_TOT_HTU_ENTRY */
	vrx318_w32_sb(0x00000041, HTU_ENTRY_TBL(OAM_F4_TOT_HTU_ENTRY), base);
	vrx318_w32_sb(0xfff0000e, HTU_MASK_TBL(OAM_F4_TOT_HTU_ENTRY), base);
	vrx318_w32_sb(0x00000600, HTU_RESULT_TBL(OAM_F4_TOT_HTU_ENTRY), base);

	/* OAM_F5_HTU_ENTRY */
	vrx318_w32_sb(0x00000009, HTU_ENTRY_TBL(OAM_F5_HTU_ENTRY), base);
	vrx318_w32_sb(0xfffffff2, HTU_MASK_TBL(OAM_F5_HTU_ENTRY), base);
	vrx318_w32_sb(0x00000600, HTU_RESULT_TBL(OAM_F5_HTU_ENTRY), base);

	return;
}

static void vrx318_atm_wtx_queue_cfg_init(u32 base, int qsb_en)
{
	int i;
	wtx_queue_config_t wtx_queue_config;

	dword_clear(&wtx_queue_config, sizeof(wtx_queue_config));
	wtx_queue_config.same_vc_qmap = 0;
	wtx_queue_config.uu           = 0;
	wtx_queue_config.cpi          = 0;
	wtx_queue_config.sbid         = 0;
	/*  Which QSB queue (VCID) does this TX queue map to. */
	wtx_queue_config.qsb_vcid     = 0;
	wtx_queue_config.mpoa_mode    = 0; /*  0: VCmux, 1: LLC */
	wtx_queue_config.qsben        = qsb_en;
	wtx_queue_config.atm_header   = 0;

	for (i = 0; i < txq_num(); i++) {
		dword_mem_write(SB_BUFFER(WTX_Q_CFG(i)),
			&wtx_queue_config, sizeof(wtx_queue_config_t), base);
	}

	return;
}

static void vrx318_atm_wtx_port_cfg_init(u32 base, int qsb_en)
{
	wtx_port_config_t wtx_port_config;

	dword_clear(&wtx_port_config, sizeof(wtx_port_config));
	wtx_port_config.qsben = qsb_en;
	/* BC0: Use TX Queue 0 - if QSB is Disabled */
	wtx_port_config.qid = 0;
	dword_mem_write(SB_BUFFER(__WTX_PORT_CONFIG0), &wtx_port_config,
		sizeof(wtx_port_config), base);
	/* BC1: Use TX Queue 1 - if QSB is Disabled */
	wtx_port_config.qid = 1;
	dword_mem_write(SB_BUFFER(__WTX_PORT_CONFIG1), &wtx_port_config,
		sizeof(wtx_port_config), base);

	return;
}

static void vrx318_atm_wrx_queue_cfg_init(u32 base)
{
	int i;
	wrx_queue_config_t wrx_queue_config;

	dword_clear(&wrx_queue_config, sizeof(wrx_queue_config));

	wrx_queue_config.new_vlan  = 0;
	wrx_queue_config.vlan_ins  = 0;
	/* 0: EoA without FCS, 1: EoA with FCS, 2: PPPoA, 3:IPoA */
	wrx_queue_config.mpoa_type = MPOA_IPOA;
	wrx_queue_config.ip_ver    = ATM_IP_IPV4; /* 0: IPv4, 1: IPv6 */
	wrx_queue_config.mpoa_mode = MPOA_VCMUX_MODE; /* 0: VCmux, 1: LLC */
	wrx_queue_config.oversize  = DMA_PACKET_SIZE;
	wrx_queue_config.undersize = 0;
	wrx_queue_config.mfs       = DMA_PACKET_SIZE; /* Max fragment size */
	wrx_queue_config.uumask    = 0xFF;
	wrx_queue_config.cpimask   = 0xFF;
	wrx_queue_config.uuexp     = 0;
	wrx_queue_config.cpiexp    = 0;

	for (i = 0; i < 2; i++)
		dword_mem_write(SB_BUFFER(WRX_Q_CFG(i)),
			&wrx_queue_config, sizeof(wrx_queue_config_t), base);

	return;
}

static int vrx318_ds_aal5_desq_cfg_ctxt_init(struct vrx318_priv *priv,
							u32 base)
{
	int i;
	rx_descriptor_t rx_descriptor;
	dma_addr_t phy_addr;
	desq_cfg_ctxt_t ds_desq_cfg_ctxt;
	unsigned int *soc_ds_base
		= (unsigned int *)priv->soc_cfg.soc_ds_dbase;

	dword_clear(&ds_desq_cfg_ctxt, sizeof(ds_desq_cfg_ctxt));

	/* Initialize Downstream Descriptor Queue Config/Context */
	ds_desq_cfg_ctxt.des_in_own_val		= 0;
	ds_desq_cfg_ctxt.fast_path		= 0;
	ds_desq_cfg_ctxt.mbox_int_en		= 0;
	ds_desq_cfg_ctxt.des_sync_needed	= 1;
	ds_desq_cfg_ctxt.des_num		= priv->soc_cfg.soc_ds_dnum;
	ds_desq_cfg_ctxt.des_base_addr		= __DS_PKT_DES_LIST_BASE;
	dword_mem_write(SB_BUFFER(__DS_PKT_DESQ_CFG_CTXT),
		&ds_desq_cfg_ctxt, sizeof(desq_cfg_ctxt_t), base);

	dword_clear(&rx_descriptor, sizeof(rx_descriptor));
	rx_descriptor.own = !ds_desq_cfg_ctxt.des_in_own_val;
	rx_descriptor.sop = 1;
	rx_descriptor.eop = 1;
	rx_descriptor.data_len = DMA_PACKET_SIZE;

	/* Initialize DS Descriptors */
	for (i = 0; i < ds_desq_cfg_ctxt.des_num; i++) {
		if (!priv->param.pmac_en)
			rx_descriptor.data_ptr
				= soc_ds_base[i * 2 + 1] +  VRX318_OB_BADDR;
		else {
			if (alloc_data_buffer(priv, DMA_PACKET_SIZE,
					&phy_addr, (unsigned int *)NULL)) {
				vrx318_dbg(DBG_ERR,
					"Failed to alloc buffer for ds des\n");
				return -ENOMEM;
			}
			rx_descriptor.data_ptr
				= (unsigned int)phy_addr + VRX318_OB_BADDR;
		}
		dword_mem_write(
			SB_BUFFER(ds_desq_cfg_ctxt.des_base_addr + (i * 2)),
			&rx_descriptor, sizeof(rx_descriptor), base);
	}

	return 0;
}

static int vrx318_ds_oam_desq_cfg_ctxt_init(struct vrx318_atm_tc *priv,
							u32 base)
{
	int i;
	desq_cfg_ctxt_t oam_desq_cfg_ctxt;
	rx_descriptor_t rx_descriptor;
	dma_addr_t phy_addr;

	/* Initialize Downstream OAM Descriptor Queue Config/Context */
	dword_clear(&oam_desq_cfg_ctxt, sizeof(oam_desq_cfg_ctxt));
	oam_desq_cfg_ctxt.des_in_own_val	= 1;
	oam_desq_cfg_ctxt.fast_path		= 0;
	oam_desq_cfg_ctxt.mbox_int_en		= 0;
	/* FW does not it copy to SoC, driver launch a tasklet
	to directly fetch from vrx318 des list
	*/
	oam_desq_cfg_ctxt.des_sync_needed	= 0;
	oam_desq_cfg_ctxt.des_num		= DS_OAM_DES_NUM;
	oam_desq_cfg_ctxt.des_base_addr		= __DS_OAM_DES_LIST_BASE;
	dword_mem_write(SB_BUFFER(__DS_OAM_DESQ_CFG_CTXT),
		&oam_desq_cfg_ctxt, sizeof(oam_desq_cfg_ctxt), base);

	priv->oam_llst.oam_num = oam_desq_cfg_ctxt.des_num;
	priv->oam_llst.oam_des_list
		= kmalloc(priv->oam_llst.oam_num * sizeof(unsigned int),
			GFP_KERNEL);
	if (!priv->oam_llst.oam_des_list)
		return -ENOMEM;
	priv->oam_llst.oam_idx = 0;

	/* Init OWN bit of all the Descriptors in Shadow DS Descriptor List */
	rx_descriptor.own = !oam_desq_cfg_ctxt.des_in_own_val;
	rx_descriptor.sop = 1;
	rx_descriptor.eop = 1;
	rx_descriptor.data_len = 0; /*EDMA doesn't require the data_len */
	for (i = 0; i < oam_desq_cfg_ctxt.des_num; i++) {
		if (alloc_data_buffer(priv->frwk, ATM_OAM_SIZE, &phy_addr,
			&priv->oam_llst.oam_des_list[i])) {
			vrx318_dbg(DBG_ERR, "Failed to allocate buffer for DS OAM\n");
			return -ENOMEM;
		}
		rx_descriptor.data_ptr = phy_addr + VRX318_OB_BADDR;
		dword_mem_write(
			SB_BUFFER(oam_desq_cfg_ctxt.des_base_addr + (i * 2)),
			&rx_descriptor, sizeof(rx_descriptor), base);
	}

	return 0;
}

static void vrx318_us_fastpath_desq_cfg_ctxt_init(struct vrx318_priv *priv,
							u32 base)
{
	int i, own;
	desq_cfg_ctxt_t us_fp_desq_cfg_ctxt;

	if (!priv->soc_cfg.soc_fp_us_dnum)
		return;

	dword_clear(&us_fp_desq_cfg_ctxt, sizeof(us_fp_desq_cfg_ctxt));
	/* Initialize Up-Stream Fast-Path Descriptor Queue Config/Context */
	/*1 - owned by SoC CDMA, 0 - owned by VRX318 */
	us_fp_desq_cfg_ctxt.des_in_own_val	= 0;
	us_fp_desq_cfg_ctxt.fast_path		= 1;
	us_fp_desq_cfg_ctxt.mbox_int_en		= 0;
	us_fp_desq_cfg_ctxt.des_sync_needed	= 1;
	/* max: soc_fp_us_dnum: 64 */
	us_fp_desq_cfg_ctxt.des_num		= priv->soc_cfg.soc_fp_us_dnum;
	us_fp_desq_cfg_ctxt.des_base_addr	= __US_FAST_PATH_DES_LIST_BASE;

	dword_mem_write(SB_BUFFER(__US_FP_INQ_DES_CFG_CTXT),
		&us_fp_desq_cfg_ctxt, sizeof(desq_cfg_ctxt_t), base);

	/* Init OWN bit of all the Descriptors in Shadow Fast Path Des List */
	own = !us_fp_desq_cfg_ctxt.des_in_own_val;
	own = (own & 1) << 31;

	for (i = 0; i < us_fp_desq_cfg_ctxt.des_num; i++) {
		vrx318_w32_sb(own,
			us_fp_desq_cfg_ctxt.des_base_addr + (i * 2), base);
	}

	return;
}

static void vrx318_us_cpupath_desq_cfg_ctxt_init(struct vrx318_priv *priv,
							u32 base)
{
	int i, own;
	desq_cfg_ctxt_t us_cpu_desq_cfg_ctxt;

	if (!priv->soc_cfg.soc_cpu_us_dnum)
		return;

	dword_clear(&us_cpu_desq_cfg_ctxt, sizeof(us_cpu_desq_cfg_ctxt));
	/* Initialize Up-Stream CPU-Path Descriptor Queue Config/Context */
	us_cpu_desq_cfg_ctxt.des_in_own_val  = 1;
	us_cpu_desq_cfg_ctxt.fast_path       = 0;
	us_cpu_desq_cfg_ctxt.mbox_int_en     = 0;
	us_cpu_desq_cfg_ctxt.des_sync_needed = 1;
	/* max: cpu us dnum: 64 */
	us_cpu_desq_cfg_ctxt.des_num         = priv->soc_cfg.soc_cpu_us_dnum;
	us_cpu_desq_cfg_ctxt.des_base_addr   = __US_CPU_PATH_DEST_LIST_BASE;

	dword_mem_write(SB_BUFFER(__US_CPU_INQ_DES_CFG_CTXT),
		&us_cpu_desq_cfg_ctxt, sizeof(desq_cfg_ctxt_t), base);

	/* Init OWN bit of all the Descriptors in Shadow CPU Path Des List */
	own = !us_cpu_desq_cfg_ctxt.des_in_own_val;
	own = (own & 1) << 31;

	for (i = 0; i < us_cpu_desq_cfg_ctxt.des_num; i++) {
		vrx318_w32_sb(own,
			us_cpu_desq_cfg_ctxt.des_base_addr + (i * 2), base);
	}

	return;
}



/* Initialize QoS related configuration for VRX218
* Initializes the below PPE FW Data Structures
* 1. INQ_QoS_CFG
* 2. QoSQ_CFG_CTXT
* 4. SHAPING_WFQ_CFG
* 5. QOSQ_MIB
* 6. QOSQ_FLOW_CTRL_CFG
* 7. STD_DES_CFG
*/
static void vrx318_atm_us_qos_cfg_init(struct vrx318_priv *priv,
						u32 base)
{
	int i, offset;
	qosq_flow_ctrl_cfg_t qosq_flow_ctrl_cfg;
	std_des_cfg_t std_des_cfg;
	inq_qos_cfg_t fp_qos_cfg, cpu_qos_cfg;
	qosq_cfg_ctxt_t qosq_cfg_ctxt;

	/* Initialize QOSQ_FLOW_CTRL_CFG */
	dword_clear(&qosq_flow_ctrl_cfg, sizeof(qosq_flow_ctrl_cfg));
	qosq_flow_ctrl_cfg.large_frame_size = 1024;
	qosq_flow_ctrl_cfg.large_frame_drop_th = 28;
	dword_mem_write(SB_BUFFER(__QOSQ_FLOW_CTRL_CFG),
		&qosq_flow_ctrl_cfg, sizeof(qosq_flow_ctrl_cfg), base);

	/* Initialize STD_DES_CFG */
	dword_clear(&std_des_cfg, sizeof(std_des_cfg));
	std_des_cfg.byte_off = 0;
	std_des_cfg.data_len = DMA_PACKET_SIZE - 32;
	dword_mem_write(SB_BUFFER(__STD_DES_CFG),
		&std_des_cfg, sizeof(std_des_cfg), base);

	/* Setup INQ_QoS_CFG for Fast-Path & CPU-Path */
	dword_clear(&fp_qos_cfg, sizeof(fp_qos_cfg));
	dword_clear(&cpu_qos_cfg, sizeof(cpu_qos_cfg));

	/* By default, support 8 queues only */
	if (priv->soc_cfg.soc_fp_us_dnum) {
		fp_qos_cfg.qos_en         = 1;
		fp_qos_cfg.qid_mask       = 0xF;
		fp_qos_cfg.qosq_base_qid  = 0;
		fp_qos_cfg.desq_cfg_ctxt  = __US_FP_INQ_DES_CFG_CTXT;
		dword_mem_write(INQ_QOS_CFG_BASE(0), &fp_qos_cfg,
				sizeof(fp_qos_cfg), base);
	}

	if (priv->soc_cfg.soc_cpu_us_dnum) {
		cpu_qos_cfg.qos_en        = 1;
		cpu_qos_cfg.qid_mask      = 0xF;
		cpu_qos_cfg.qosq_base_qid = 0;
		cpu_qos_cfg.desq_cfg_ctxt = __US_CPU_INQ_DES_CFG_CTXT;
		dword_mem_write(INQ_QOS_CFG_BASE(1), &cpu_qos_cfg,
				sizeof(cpu_qos_cfg), base);
	}

	/* Setup Fast-Path DESQ_CFG_CTXT */
	vrx318_us_fastpath_desq_cfg_ctxt_init(priv, base);

	/* Setup CPU-Path DESQ_CFG_CTXT */
	vrx318_us_cpupath_desq_cfg_ctxt_init(priv, base);

	/* Setup QoSQ_CFG_CTXT */
	dword_clear(&qosq_cfg_ctxt, sizeof(qosq_cfg_ctxt));

	qosq_cfg_ctxt.threshold = 8;
	qosq_cfg_ctxt.des_num   = TOTAL_QOS_DES_NUM / txq_num();

	offset = 0;
	for (i = 0; i < txq_num(); i++) {
		qosq_cfg_ctxt.des_base_addr = __US_QOSQ_DES_LIST_BASE + offset;
		dword_mem_write(
			SB_BUFFER((__QOSQ_CFG_CTXT_BASE
				+ (i * __QOSQ_CFG_CTXT_SIZE))),
			&qosq_cfg_ctxt, sizeof(qosq_cfg_ctxt), base);
		offset += qosq_cfg_ctxt.des_num * 2;
	}
	return;
}

static int vrx318_atm_us_qos_des_cfg_ctxt_init(struct vrx318_priv *priv,
							u32 base)
{
	int i, offset;
	tx_descriptor_t tx_descriptor;
	desq_cfg_ctxt_t desq_cfg_ctxt;
	u32 dst_addr;
	dma_addr_t phy_addr;

	/* Setup QoSQ PSEUDO DES_CFG_CTXT */
	dword_clear(&desq_cfg_ctxt, sizeof(desq_cfg_ctxt));

	desq_cfg_ctxt.des_in_own_val	= 1;
	desq_cfg_ctxt.mbox_int_en	= 0;
	desq_cfg_ctxt.des_sync_needed	= 0;
	desq_cfg_ctxt.des_num		= TOTAL_QOS_DES_NUM / txq_num();
	offset				= 0;

	for (i = 0; i < txq_num(); i++) {
		desq_cfg_ctxt.des_base_addr = __US_QOSQ_DES_LIST_BASE + offset;
		dst_addr = SB_BUFFER(__QOSQ_PSEUDO_DES_CFG_BASE +
			(i * sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int)));
		dword_mem_write(dst_addr, &desq_cfg_ctxt,
			sizeof(desq_cfg_ctxt), base);
		offset += desq_cfg_ctxt.des_num * 2;
	}

	dword_clear(&tx_descriptor, sizeof(tx_descriptor));
	tx_descriptor.own = !desq_cfg_ctxt.des_in_own_val;
	tx_descriptor.data_len = DMA_PACKET_SIZE;

	/* Initialize QoSQ Descriptors */
	for (i = 0; i < TOTAL_QOS_DES_NUM; i++) {
		if (alloc_data_buffer(priv, tx_descriptor.data_len,
				&phy_addr, (unsigned int *)NULL)) {
			vrx318_dbg(DBG_ERR, "Failed alloc buffer for QoSQ\n");
			return -ENOMEM;
		}
		tx_descriptor.data_ptr = phy_addr + VRX318_OB_BADDR;
		dword_mem_write(SB_BUFFER(__US_QOSQ_DES_LIST_BASE + (i * 2)),
			&tx_descriptor, sizeof(tx_descriptor), base);
	}

	return 0;
}

void vrx318_atm_des_sync_init(struct vrx318_priv *priv, u32 base)
{
	des_sync_cfg_ctxt_t des_sync_cfg_ctxt;
	int idx;

	idx = 0;

	/* Fast-Path Sync Config/Context */
	dword_clear(&des_sync_cfg_ctxt, sizeof(des_sync_cfg_ctxt));
	if (priv->soc_cfg.soc_fp_us_dnum) {
		des_sync_cfg_ctxt.sync_type = US_READ_WRITE_SYNC;
		/* des_sync_cfg_ctxt.us_des_polling_needed = 0; */
		des_sync_cfg_ctxt.max_polling_intv = 4;
		des_sync_cfg_ctxt.desq_cfg_ctxt	= __US_FP_INQ_DES_CFG_CTXT;
		des_sync_cfg_ctxt.soc_des_own_val
			= priv->soc_cfg.soc_us_desc_own;
		des_sync_cfg_ctxt.ext_desc_base_addr
			= priv->soc_cfg.soc_fp_us_phybase + VRX318_OB_BADDR;
		dword_mem_write(DES_SYNC_CFG_CTXT(idx), &des_sync_cfg_ctxt,
				sizeof(des_sync_cfg_ctxt), base);
		idx++;
	}

	/* CPU-Path Sync Config/Context */
	dword_clear(&des_sync_cfg_ctxt, sizeof(des_sync_cfg_ctxt));
	if (priv->soc_cfg.soc_cpu_us_dnum) {
		des_sync_cfg_ctxt.sync_type = US_READ_WRITE_SYNC;
		/* des_sync_cfg_ctxt.us_des_polling_needed = 0; */
		des_sync_cfg_ctxt.max_polling_intv = 10;
		des_sync_cfg_ctxt.desq_cfg_ctxt	= __US_CPU_INQ_DES_CFG_CTXT;
		des_sync_cfg_ctxt.soc_des_own_val
			= priv->soc_cfg.soc_us_desc_own;
		des_sync_cfg_ctxt.ext_desc_base_addr
			= priv->soc_cfg.soc_cpu_us_phybase + VRX318_OB_BADDR;
		dword_mem_write(DES_SYNC_CFG_CTXT(idx), &des_sync_cfg_ctxt,
				sizeof(des_sync_cfg_ctxt), base);
		idx++;
	}

	/* Downstream AAL5Q Sync Config/Context */
	dword_clear(&des_sync_cfg_ctxt, sizeof(des_sync_cfg_ctxt));
	if (priv->soc_cfg.soc_ds_dnum) {
		des_sync_cfg_ctxt.sync_type = DS_WRITE_READ_SYNC;
		/* des_sync_cfg_ctxt.us_des_polling_needed = 0; */
		des_sync_cfg_ctxt.max_polling_intv = 1;
		des_sync_cfg_ctxt.desq_cfg_ctxt	= __DS_PKT_DESQ_CFG_CTXT;
		des_sync_cfg_ctxt.ext_desc_base_addr
			= priv->soc_cfg.soc_ds_phybase + VRX318_OB_BADDR;
		des_sync_cfg_ctxt.soc_des_own_val
			= priv->soc_cfg.soc_ds_desc_own;
		dword_mem_write(DES_SYNC_CFG_CTXT(idx), &des_sync_cfg_ctxt,
				sizeof(des_sync_cfg_ctxt), base);
		idx++;
	}

	/* Downstream OAM Sync Config/Context */
	/* OAM packet is not synced to SoC in current design */

	return;
}

/**
* Initialize VRX218 ATM-TC Local Descriptor List
* Performs below actions
* (1) Allocate and Initialize TC UpStream Local Descriptors
* (2) Allocate and Initialize TC DownStream Local Descriptors
*/
void vrx318_atm_local_des_cfg_ctxt_init(u32 base)
{
	int i, des_cnt;
	tx_descriptor_t tx_descriptor;
	rx_descriptor_t rx_descriptor;
	desq_cfg_ctxt_t desq_cfg_ctxt;
	u32 dst_addr;

	unsigned int all5_sb_addr[] = {
		__DS_SB_PKT_DATA_PTR_0, __DS_SB_PKT_DATA_PTR_1,
		__DS_SB_PKT_DATA_PTR_2, __DS_SB_PKT_DATA_PTR_3,
		__DS_SB_PKT_DATA_PTR_4, __DS_SB_PKT_DATA_PTR_5,
		__DS_SB_PKT_DATA_PTR_6, __DS_SB_PKT_DATA_PTR_7,
		__DS_SB_PKT_DATA_PTR_8, __DS_SB_PKT_DATA_PTR_9,
		__DS_SB_PKT_DATA_PTR_10, __DS_SB_PKT_DATA_PTR_11};

	unsigned int oam_sb_addr[] = {
		__DS_SB_OAM_DATA_PTR_0, __DS_SB_OAM_DATA_PTR_1,
		__DS_SB_OAM_DATA_PTR_2, __DS_SB_OAM_DATA_PTR_3,
		__DS_SB_OAM_DATA_PTR_4, __DS_SB_OAM_DATA_PTR_5,
		__DS_SB_OAM_DATA_PTR_6, __DS_SB_OAM_DATA_PTR_7,
		__DS_SB_OAM_DATA_PTR_8, __DS_SB_OAM_DATA_PTR_9};

	/* Setup the Local DESQ Configuration/Context for 16 UpStream Queues */
	dword_clear(&desq_cfg_ctxt, sizeof(desq_cfg_ctxt));
	desq_cfg_ctxt.des_in_own_val = 1;
	des_cnt = 0;
	for (i = 0; i < txq_num(); i++) {
		desq_cfg_ctxt.des_num
			= ATM_PDBRAM_US_BUF_LEN / txq_num();
		desq_cfg_ctxt.des_base_addr
			= __US_TC_LOCAL_Q_DES_LIST_BASE + (des_cnt * 2);
		dst_addr = SB_BUFFER(__US_TC_LOCAL_Q_CFG_CTXT_BASE
			+ i * (sizeof(desq_cfg_ctxt_t)/sizeof(unsigned int)));
		dword_mem_write(dst_addr, &desq_cfg_ctxt,
				sizeof(desq_cfg_ctxt), base);
		des_cnt += desq_cfg_ctxt.des_num;
	}

	/* Initialize UpStream Descriptors */
	dword_clear(&tx_descriptor, sizeof(tx_descriptor));
	tx_descriptor.own = !desq_cfg_ctxt.des_in_own_val;
	tx_descriptor.data_len = DMA_PACKET_SIZE;

	/* Note: ATM US PDBRAM pointer need convert to DWORD pointer */
	for (i = 0; i < ATM_PDBRAM_US_BUF_LEN; i++) {
		tx_descriptor.data_ptr
			= (PDBRAM_TX_PKT_BUF_BASE
				+ (i * tx_descriptor.data_len)) / 4;
		dst_addr = SB_BUFFER(__US_TC_LOCAL_Q_DES_LIST_BASE + (i * 2));
		dword_mem_write(dst_addr, &tx_descriptor,
				sizeof(tx_descriptor), base);
	}

	/* Setup the Local DESQ Configuration/Context for AAL5 DownStream Q*/
	dword_clear(&desq_cfg_ctxt, sizeof(desq_cfg_ctxt));
	desq_cfg_ctxt.des_in_own_val = 0;
	desq_cfg_ctxt.des_num
		= ARRAY_SIZE(all5_sb_addr) + ATM_PDBRAM_DS_BUF_LEN;
	desq_cfg_ctxt.des_base_addr = __DS_TC_LOCAL_AAL5Q_DES_LIST_BASE;
	dword_mem_write(SB_BUFFER(__DS_TC_AAL5_LOCAL_Q_CFG_CTXT),
			&desq_cfg_ctxt, sizeof(desq_cfg_ctxt), base);

	dword_clear(&rx_descriptor, sizeof(rx_descriptor));
	rx_descriptor.own = !desq_cfg_ctxt.des_in_own_val;
	rx_descriptor.data_len = DMA_PACKET_SIZE;

	/* Initialize DownStream AAL5 Descriptors */
	for (i = 0; i < ATM_PDBRAM_DS_BUF_LEN; i++) {
		rx_descriptor.data_ptr = (PDBRAM_TX_PKT_BUF_BASE +
				(ATM_PDBRAM_US_BUF_LEN * DMA_PACKET_SIZE) +
				(i * DMA_PACKET_SIZE));
		dst_addr = SB_BUFFER(__DS_TC_LOCAL_AAL5Q_DES_LIST_BASE +
				(i * 2));
		dword_mem_write(dst_addr, &rx_descriptor,
				sizeof(rx_descriptor), base);
	}

	for (i = 0; i < ARRAY_SIZE(all5_sb_addr); i++) {
		rx_descriptor.data_ptr = SB_BUFFER(all5_sb_addr[i]);
		dst_addr = SB_BUFFER(__DS_TC_LOCAL_AAL5Q_DES_LIST_BASE +
				(ATM_PDBRAM_DS_BUF_LEN * 2) + (i * 2));
		dword_mem_write(dst_addr, &rx_descriptor,
				sizeof(rx_descriptor), base);
	}

	/* Setup the Local DESQ Configuration/Context for OAM DownStream Q */
	dword_clear(&desq_cfg_ctxt, sizeof(desq_cfg_ctxt));
	desq_cfg_ctxt.des_in_own_val = 0;
	desq_cfg_ctxt.des_num = ATM_PDBRAM_DS_OAM_LEN;
	desq_cfg_ctxt.des_base_addr = __DS_TC_LOCAL_OAMQ_DES_LIST_BASE;
	dword_mem_write(SB_BUFFER(__DS_TC_OAM_LOCAL_Q_CFG_CTXT),
			&desq_cfg_ctxt, sizeof(desq_cfg_ctxt), base);

	dword_clear(&rx_descriptor, sizeof(rx_descriptor));
	rx_descriptor.own = !desq_cfg_ctxt.des_in_own_val;
	rx_descriptor.data_len = 128;

	/* Initialize DownStream OAM Descriptors */
	for (i = 0; i < ARRAY_SIZE(oam_sb_addr); i++) {
		rx_descriptor.data_ptr = SB_BUFFER(oam_sb_addr[i]);
		dst_addr = SB_BUFFER(__DS_TC_LOCAL_OAMQ_DES_LIST_BASE +
					(i * 2));
		dword_mem_write(dst_addr, &rx_descriptor,
				sizeof(rx_descriptor), base);
	}

	return;
}

void vrx318_atm_edma_copy_ch_init(u32 base)
{
	edma_copy_ch_cfg_t copy_ch_cfg;
	int i;
	u32 dst_addr;

	/* Setup up to 16 UpStream eDMA Copy Channel */
	for (i = 0; i < txq_num(); i++) {
		copy_ch_cfg.srcq_ctxt_ptr = __QOSQ_PSEUDO_DES_CFG_BASE +
			i * sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
		copy_ch_cfg.dstq_ctxt_ptr = __US_TC_LOCAL_Q_CFG_CTXT_BASE +
			i * sizeof(desq_cfg_ctxt_t) / sizeof(unsigned int);
		dst_addr = SB_BUFFER(__US_EDMA_COPY_CH_CFG +
			i * sizeof(edma_copy_ch_cfg_t) / sizeof(unsigned int));
		dword_mem_write(dst_addr, &copy_ch_cfg,
				sizeof(copy_ch_cfg), base);
	}

	/* Setup AAL5 DownStream eDMA Copy Channel */
	copy_ch_cfg.srcq_ctxt_ptr = __DS_TC_AAL5_LOCAL_Q_CFG_CTXT;
	copy_ch_cfg.dstq_ctxt_ptr = __DS_PKT_DESQ_CFG_CTXT;
	dst_addr = __DS_EDMA_COPY_CH_CFG;
	dword_mem_write(SB_BUFFER(dst_addr), &copy_ch_cfg,
			sizeof(copy_ch_cfg), base);

	/* Setup OAM DownStream eDMA Copy Channel */
	copy_ch_cfg.srcq_ctxt_ptr = __DS_TC_OAM_LOCAL_Q_CFG_CTXT;
	copy_ch_cfg.dstq_ctxt_ptr = __DS_OAM_DESQ_CFG_CTXT;
	dst_addr += sizeof(copy_ch_cfg) / sizeof(int);
	dword_mem_write(SB_BUFFER(dst_addr), &copy_ch_cfg,
			sizeof(copy_ch_cfg), base);

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
	/* Calculate all the offset for RX descriptor base address,
	    RX data pointer, and Tx descriptor base address
	*/
	/* offset 11 */
	const u32 rx_desc_off
		= offsetof(des_sync_cfg_ctxt_t, cdma_dst_des_dw0) / 4;
	/* offset 16 */
	const u32 rx_data_off
		= offsetof(des_sync_cfg_ctxt_t, rd_des_buf[0]) / 4;
	/* offset 6 */
	const u32 tx_desc_off = offsetof(des_sync_cfg_ctxt_t, rd_cmd_sar) / 4;

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

	vrx318_cfg_cdma_cpch(rx_cid, tx_cid, SB_BUFFER(rx_dbase),
				SB_BUFFER(tx_dbase), 1, 1, base);
}


/* Driver initialize and turn on all the required CDMA channel. */
static void vrx318_atm_cdma_init(u32 base)
{
	u32 reg_val;
	int off;

	/* Enable POWER to CDMA */
	reg_val = vrx318_r32(PMU_PWDCR, base) & (~0x00000004);
	vrx318_w32(reg_val, PMU_PWDCR, base);

	vrx318_w32(0x80000040, DMA_CPOLL, base);
	vrx318_w32(0x00000004, DMA_PS, base);
	reg_val = (vrx318_r32(DMA_PCTRL, base) & ~(0x0000003C)) | (0x0000003C);
	vrx318_w32(reg_val, DMA_PCTRL, base);

	/* Setup DMA COPYCH (RXCH0 & TXCH1): 1 Descriptor */
	off = 0;
	vrx318_setup_sync_cdma(0, 1, off, base); /*CPU US descriptor SYNC */
	off++;

	/* Setup DMA COPYCH (RXCH2 & TXCH3): 1 Descriptor */
	/* FastPath US descriptor SYNC */
	vrx318_setup_sync_cdma(2, 3, off, base);
	off++;

	/* Setup DMA COPYCH (RXCH4 & TXCH5): 1 Descriptor */
	/* DS AAL5 descriptor SYNC */
	vrx318_setup_sync_cdma(4, 5, off, base);

	return;
}

/**
 * ATM TC - Initialization : Entry Point
 * dev     - PCIe Endpoint Device
 * qsb_en  - 0: QSB Disabled, 1: QSB Enabled
 */
static void vrx318_ppe_atm_init(struct vrx318_atm_tc *priv, int qsb_en)
{
	u32 base;
	struct vrx318_priv *frwk = priv->frwk;

	base = (u32)frwk->ep_dev[priv->ep_id].membase;

	vrx318_sb_clear(base);
	vrx318_atm_pdma_init(base);

	/* vrx318_tc_reg_init(base); */ /* Testing only */
	vrx318_atm_fw_init(base);
	vrx318_atm_htu_init(base);
	vrx318_atm_wtx_queue_cfg_init(base, qsb_en);
	vrx318_atm_wtx_port_cfg_init(base, qsb_en);
	vrx318_atm_wrx_queue_cfg_init(base);

	/* Initialize Downstream AAL5 Descriptors A1 and desc config context */
	vrx318_ds_aal5_desq_cfg_ctxt_init(frwk, base);

	/* Initialize Downstream OAM Descriptors and desc config context */
	vrx318_ds_oam_desq_cfg_ctxt_init(priv, base);

	vrx318_atm_us_qos_cfg_init(frwk, base);
	vrx318_atm_us_qos_des_cfg_ctxt_init(frwk, base);

	vrx318_atm_des_sync_init(frwk, base);
	vrx318_atm_local_des_cfg_ctxt_init(base);
	vrx318_atm_edma_copy_ch_init(base);

	/* eDMA LLE in PDBRAM */
	vrx318_edma_init(base, frwk->param.lle_in_sb, frwk->param.cdma_wr_en);

	/* Init CDMA for ATM TC-Mode */
	vrx318_atm_cdma_init(base);
	return;
}

static void do_oam_tasklet(unsigned long arg)
{
	unsigned long sys_flag;
	rx_descriptor_t desc;
	struct uni_cell_header *header;
	int conn, ep_id;
	struct atm_vcc *vcc;
	desq_cfg_ctxt_t ds_oam_cfg_ctxt, local_oam_cfg_ctxt;
	dma_addr_t phy_addr;
	struct vrx318_atm_tc *priv;
	u32 base, ds_oam_dbase, oam_idx, oam_max_num;
	u32 mbox1_ier, rx_cnt, dq_cnt, *oam_dlist;

	priv = g_atm_tc;
	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;
	oam_dlist = priv->oam_llst.oam_des_list;
	oam_idx = priv->oam_llst.oam_idx;
	oam_max_num = priv->oam_llst.oam_num;

	dword_mem_read(&ds_oam_cfg_ctxt, SB_BUFFER(__DS_OAM_DESQ_CFG_CTXT),
			sizeof(ds_oam_cfg_ctxt), base);
	ds_oam_dbase = ds_oam_cfg_ctxt.des_base_addr;
	rx_cnt = ds_oam_cfg_ctxt.enq_pkt_cnt;
	dq_cnt = ds_oam_cfg_ctxt.deq_pkt_cnt;

	while (1) {
		dword_mem_read(&desc, SB_BUFFER(ds_oam_dbase + oam_idx * 2),
				sizeof(desc), base);
		if (!desc.own) { /* desc not belong to CPU */
			/* Update idx as will exit loop */
			priv->oam_llst.oam_idx = oam_idx;
			/* probe if there's still availble oam packet */
			dword_mem_read(&ds_oam_cfg_ctxt,
				SB_BUFFER(__DS_OAM_DESQ_CFG_CTXT),
				sizeof(ds_oam_cfg_ctxt), base);
			dword_mem_write(
				SB_BUFFER(__DS_OAM_DESQ_CFG_CTXT + offsetof(desq_cfg_ctxt_t, deq_pkt_cnt) / sizeof(u32)),
				&dq_cnt, sizeof(dq_cnt), base);
			/* Update deq pkt cnt, may have race condition */
			/* ds_oam_cfg_ctxt.deq_pkt_cnt += rx_cnt;
			rx_cnt = ((ds_oam_cfg_ctxt.deq_pkt_cnt & 0xFFFF) << 16)
				| (ds_oam_cfg_ctxt.enq_pkt_cnt & 0xFFFF);
			dword_mem_write(SB_BUFFER(__DS_OAM_DESQ_CFG_CTXT + 2),
					&rx_cnt, 4, base); */
			dword_mem_read(&local_oam_cfg_ctxt,
				SB_BUFFER(__DS_TC_OAM_LOCAL_Q_CFG_CTXT),
				sizeof(local_oam_cfg_ctxt), base);
			if ((local_oam_cfg_ctxt.enq_pkt_cnt !=
					ds_oam_cfg_ctxt.enq_pkt_cnt) ||
				(ds_oam_cfg_ctxt.enq_pkt_cnt !=
					ds_oam_cfg_ctxt.deq_pkt_cnt))
				tasklet_schedule(&g_oam_tasklet);
			else { /* No more oam pkt, should enable irq again */
				spin_lock_irqsave(&priv->oam_lock, sys_flag);
				mbox1_ier = vrx318_r32(MBOX_IGU1_IER, base);
				mbox_set_ier(MBOX_IGU1,
					mbox1_ier | OAM_RX_INTR, base);
				spin_unlock_irqrestore(&priv->oam_lock,
							sys_flag);
			}
			break;
		}

		phy_addr = dma_map_single(priv->frwk->pdev,
				(void *)oam_dlist[oam_idx],
				ATM_OAM_SIZE, DMA_FROM_DEVICE);

		dma_unmap_single(priv->frwk->pdev, phy_addr,
				ATM_OAM_SIZE, DMA_FROM_DEVICE);

		header = (struct uni_cell_header *)oam_dlist[oam_idx];

		if (header->pti == ATM_PTI_SEGF5 ||
			header->pti == ATM_PTI_E2EF5)
			conn = find_vpivci(header->vpi, header->vci);
		else if (header->vci == 0x03 || header->vci == 0x04)
			conn = find_vpi(header->vpi);
		else
			conn = -1;

		if (conn >= 0 && priv->conn[conn].vcc != NULL) {
			vcc = priv->conn[conn].vcc;
			priv->conn[conn].access_time = current_kernel_time();

			vrx318_dbg(DBG_OAM_RX, "conn=%d, vpi: %d, vci:%d\n",
				conn, header->vpi, header->vci);
			dump_oam_cell(header, 1);
			if (vcc->push_oam != NULL)
				vcc->push_oam(vcc, header);
			else {
				#if defined(CONFIG_LTQ_OAM) || \
					defined(CONFIG_LTQ_OAM_MODULE)
					ifx_push_oam((unsigned char *)header);
				#else
					vrx318_dbg(DBG_OAM_RX, "Receive OAM packet\n");
				#endif
			}
			priv->conn[conn].stats.oam_rx_pkts++;
			priv->conn[conn].stats.oam_rx_bytes
						+= desc.data_len;
			priv->stats.oam_rx_pkts++;
			priv->stats.oam_rx_bytes += desc.data_len;
		} else {
			priv->stats.oam_rx_errors++;
		}

		/* update desc */
		desc.c = 0;
		desc.own = 0;
		desc.data_len = 0;
		dword_mem_write(SB_BUFFER(ds_oam_dbase + oam_idx * 2), &desc,
				sizeof(desc), base);

		oam_idx = (oam_idx + 1) % oam_max_num;
		dq_cnt++;
	}

}

#ifdef CONFIG_LTQ_VRX318_CPUFREQ
extern int dp_coc_new_stat_req(enum ltq_cpufreq_state new_state
	, uint32_t flag);
static void do_coc_tasklet(unsigned long arg)
{
	/* change state to D0 */
	atm_coc_state = LTQ_CPUFREQ_PS_D0;
	/* call datapath to inform about the new state */
	dp_coc_new_stat_req(atm_coc_state, DP_COC_REQ_VRX318);
}
#endif

/* Note, must lock oam lock if change the ier other than OAM_INT bit */
static irqreturn_t mailbox_irq_handler(int irq, void *dev_id)
{
	u32 mbox1_isr, mbox1_ier, mbox1_isrc;
	u32 base;
	struct vrx318_atm_tc *priv = dev_id;
	int ep_id = priv->ep_id;

	base = (u32)priv->frwk->ep_dev[ep_id].membase;
	mbox1_ier = vrx318_r32(MBOX_IGU1_IER, base);
	mbox1_isrc = vrx318_r32(MBOX_IGU1_ISR, base);

	mbox1_isr = mbox1_isrc & mbox1_ier;

	/* Any other bit except OAM_INT_BIT, need lock oam_lock */
	if (mbox1_isr & OAM_RX_INTR) {
		mbox1_ier &= ~OAM_RX_INTR;
		mbox_set_ier(MBOX_IGU1, mbox1_ier, base);
		tasklet_schedule(&g_oam_tasklet);
	}

	if (mbox1_isr & FW_UNLOAD_INTR) {
		spin_lock_bh(&priv->oam_lock);
		mbox1_ier &= ~FW_UNLOAD_INTR;
		mbox_set_ier(MBOX_IGU1, mbox1_ier, base);
		spin_unlock_bh(&priv->oam_lock);
		complete(&priv->comp);
	}
#ifdef CONFIG_LTQ_VRX318_CPUFREQ
	if (mbox1_isr & PKT_RX_INTR) {
		spin_lock_bh(&priv->oam_lock);
		mbox1_ier &= ~PKT_RX_INTR;
		mbox_set_ier(MBOX_IGU1, mbox1_ier, base);
		spin_unlock_bh(&priv->oam_lock);
		tasklet_schedule(&g_coc_tasklet);
	}
#endif
	/**
	 *Clear TX interrupt at this moment.
	 * Implement flow control mechansim if there is specific requirement.
	 */
	mbox_clr_isr(MBOX_IGU1, mbox1_isrc, base);

	return IRQ_HANDLED;
}

/* Description:
 *    Enable/Disable HTU entries to capture OAM cell.
 *  Input:
 *    none
 *  Output:
 *    none
 */
static void validate_oam_htu_entry(u32 base, int en)
{
	struct htu_entry entry;
	int i;

	for (i = OAM_F4_SEG_HTU_ENTRY; i <= OAM_F5_HTU_ENTRY; i++) {
		dword_mem_read(&entry, SB_BUFFER(HTU_ENTRY_TBL(i)),
			sizeof(entry), base);
		entry.vld = en;
		dword_mem_write(SB_BUFFER(HTU_ENTRY_TBL(i)), &entry,
			sizeof(entry), base);
	}
	/*  idle for a while to finish running HTU search   */
	udelay(10);
}

/* Description:
 *    Add one entry to HTU table.
 *  Input:
 *    vpi        --- unsigned int, virtual path ID
 *    vci        --- unsigned int, virtual channel ID
 *    connection --- unsigned int, connection ID
 *    aal5       --- int, 0 means AAL0, else means AAL5
 *  Output:
 *    none
 */
static void set_htu_entry(struct vrx318_atm_tc *priv, unsigned int vpi,
			unsigned int vci, unsigned int conn, int aal5)
{
	int ep_id = priv->ep_id;
	u32 base = (u32)priv->frwk->ep_dev[ep_id].membase;

	struct htu_entry htu_entry = {
		.res1 = 0x00,
		.pid  = priv->conn[conn].port & 0x01,
		.vpi  = vpi,
		.vci  = vci,
		.pti  = 0x00,
		.vld  = 0x01};

	struct htu_mask htu_mask = {
		.set      = 0x03,
		.pid_mask = 0x02,
		.vpi_mask = 0x00,
		.vci_mask = 0x0000,
		.pti_mask = 0x03, /* 0xx, user data */
		.clear    = 0x00};

	struct htu_result htu_result = {
		.res1   = 0x00,
		.cellid = conn,
		.res2   = 0x00,
		.type   = aal5 ? 0x00 : 0x01,
		.ven    = 0x01,
		.res3   = 0x00,
		.qid    = conn};

	dword_mem_write(SB_BUFFER(HTU_RESULT_TBL(conn + OAM_HTU_ENTRY_NUMBER)),
		&htu_result, sizeof(htu_result), base);
	dword_mem_write(SB_BUFFER(HTU_MASK_TBL(conn + OAM_HTU_ENTRY_NUMBER)),
		&htu_mask, sizeof(htu_mask), base);
	dword_mem_write(SB_BUFFER(HTU_ENTRY_TBL(conn + OAM_HTU_ENTRY_NUMBER)),
		&htu_entry, sizeof(htu_entry), base);
}

/* Description:
 *    Remove one entry from HTU table.
 *  Input:
 *    conn --- unsigned int, connection ID
 *  Output:
 *    none
 */
static void clear_htu_entry(unsigned int conn, u32 base)
{
	struct htu_entry entry;

	dword_mem_read(&entry,
		SB_BUFFER(HTU_ENTRY_TBL(conn + OAM_HTU_ENTRY_NUMBER)),
		sizeof(entry), base);
	entry.vld = 0;
	dword_mem_write(SB_BUFFER(HTU_ENTRY_TBL(conn + OAM_HTU_ENTRY_NUMBER)),
		&entry,	sizeof(entry), base);
}


/* QSB in VRX318 is fixed CLK 288Mhz (System CLK)*/
static u32 get_qsb_clk(u32 base)
{
	u32 fpi_dvsn;
	u32 freq;

	fpi_dvsn = (vrx318_r32(CGU_CLKFSR, base) >> 28) & 0x03;

	freq = VRX318_SYS_CLK;

	freq = freq >> fpi_dvsn;

	return freq;
}

/* Description: Setup QSB. */
static void qsb_global_set(struct vrx318_atm_tc *priv, u32 base)
{
	u32 qsb_clk = get_qsb_clk(base);
	int i;
	u32 tmp1, tmp2, tmp3;
	u32 reg_val;
	struct vrx318_param *param = &priv->frwk->param;

	vrx318_w32(param->qsb_tau, QSB_ICDV, base);
	vrx318_w32(param->qsb_srvm, QSB_SBL, base);
	vrx318_w32(param->qsb_tstep >> 1, QSB_CFG, base);

	/* set SCT and SPT per port */
	for (i = 0; i < ATM_PORT_NUMBER; i++)
		if (priv->port[i].tx_max_cell_rate != 0) {
			tmp1 = ((qsb_clk * priv->frwk->param.qsb_tstep) >> 1) /
					priv->port[i].tx_max_cell_rate;
			tmp2 = tmp1 >> 6; /* integer value of Tsb */
			/* fractional part of Tsb */
			tmp3 = (tmp1 & ((1 << 6) - 1)) + 1;
			/*  carry over to integer part (?)  */
			if (tmp3 == (1 << 6)) {
				tmp3 = 0;
				tmp2++;
			}
			if (tmp2 == 0)
				tmp2 = tmp3 = 1;
			/*  1. set mask  */
			/*  2. write value to data transfer register */
			/*  3. start the tranfer */
			/*  SCT (FracRate)  */
			vrx318_w32(QSB_SET_SCT_MASK, QSB_RTM, base);
			vrx318_w32(tmp3, QSB_RTD, base);
			reg_val = QSB_RAMAC_RW_SET(QSB_RAMAC_RW_WRITE) |
					QSB_RAMAC_TSEL_SET(QSB_RAMAC_TSEL_SCT) |
					QSB_RAMAC_LH_SET(QSB_RAMAC_LH_LOW) |
					QSB_RAMAC_TESEL_SET(i & 0x01);
			vrx318_w32(reg_val, QSB_RAMAC, base);

			/*  SPT (SBV + PN + IntRage)    */
			vrx318_w32(QSB_SET_SPT_MASK, QSB_RTM, base);
			reg_val = QSB_RTD_TTV_SET(QSB_SPT_SBV_VALID) |
					QSB_SPT_PN_SET(i & 0x01) |
					QSB_SPT_INTRATE_SET(tmp2);
			vrx318_w32(reg_val, QSB_RTD, base);
			reg_val = QSB_RAMAC_RW_SET(QSB_RAMAC_RW_WRITE) |
					QSB_RAMAC_TSEL_SET(QSB_RAMAC_TSEL_SPT) |
					QSB_RAMAC_LH_SET(QSB_RAMAC_LH_LOW) |
					QSB_RAMAC_TESEL_SET(i & 0x01);
			vrx318_w32(reg_val, QSB_RAMAC, base);

		}
}

static inline void qsb_qos_dbg(struct atm_qos *qos)
{
	static char *str_tc[] = {
	    "ATM_NONE",
	    "ATM_UBR",
	    "ATM_CBR",
	    "ATM_VBR",
	    "ATM_ABR",
	    "ATM_ANYCLASS",
	    "ATM_VBR_RT",
	    "ATM_UBR_PLUS",
	    "ATM_MAX_PCR"
	};

	unsigned char tc = qos->txtp.traffic_class;
	int max_pcr = qos->txtp.max_pcr;
	int pcr = qos->txtp.pcr;
	int min_pcr = qos->txtp.min_pcr;
	int scr = qos->txtp.scr;
	int mbs = qos->txtp.mbs;
	int cdv = qos->txtp.cdv;

	if (tc == (unsigned char)ATM_MAX_PCR)
		vrx318_dbg(DBG_QOS, "Traffic Class: %s\n",
			str_tc[ARRAY_SIZE(str_tc) - 1]);
	else if (tc > ATM_UBR_PLUS)
		vrx318_dbg(DBG_QOS, "Traffic Class: Unknown Traffic Class\n");
	else
		vrx318_dbg(DBG_QOS, "%s\n", str_tc[tc]);

	vrx318_dbg(DBG_QOS, "max pcr: %d\n", max_pcr);
	vrx318_dbg(DBG_QOS, "desired pcr: %d\n", pcr);
	vrx318_dbg(DBG_QOS, "min pcr: %d\n", min_pcr);

	vrx318_dbg(DBG_QOS, "sustained rate: %d\n", scr);
	vrx318_dbg(DBG_QOS, "max burst size: %d\n", mbs);
	vrx318_dbg(DBG_QOS, "cell delay var: %d\n", cdv);
}

static inline void qsb_param_dbg(unsigned int qsb_clk,
			union qsb_queue_parameter_table *q_parm_tbl,
			union qsb_queue_vbr_parameter_table *q_vbr_parm_tbl)
{
	vrx318_dbg(DBG_QOS, "qsb_clk = %lu\n", (unsigned long)qsb_clk);
	vrx318_dbg(DBG_QOS, "qsb_queue_parameter_table.bit.tp       = %d\n",
				(int)q_parm_tbl->bit.tp);
	vrx318_dbg(DBG_QOS, "qsb_queue_parameter_table.bit.wfqf     = %d (0x%08X)\n",
				(int)q_parm_tbl->bit.wfqf,
				(int)q_parm_tbl->bit.wfqf);
	vrx318_dbg(DBG_QOS, "qsb_queue_parameter_table.bit.vbr      = %d\n",
				(int)q_parm_tbl->bit.vbr);
	vrx318_dbg(DBG_QOS, "qsb_queue_parameter_table.dword        = 0x%08X\n",
				(int)q_parm_tbl->dword);
	vrx318_dbg(DBG_QOS, "qsb_queue_vbr_parameter_table.bit.ts   = %d\n",
				(int)q_vbr_parm_tbl->bit.ts);
	vrx318_dbg(DBG_QOS, "qsb_queue_vbr_parameter_table.bit.taus = %d\n",
				(int)q_vbr_parm_tbl->bit.taus);
	vrx318_dbg(DBG_QOS, "qsb_queue_vbr_parameter_table.dword    = 0x%08X\n",
				(int)q_vbr_parm_tbl->dword);
}
/* Description:
 *    Setup QSB queue.
 *  Input:
 *    vcc        --- struct atm_vcc *, structure of an opened connection
 *    qos        --- struct atm_qos *, QoS parameter of the connection
 *    connection --- unsigned int, QSB queue ID, which is same as connection ID
 *  Output:
 *    none
 */
static void set_qsb(struct vrx318_atm_tc *priv, struct atm_vcc *vcc,
		struct atm_qos *qos, unsigned int conn)
{
	union qsb_queue_parameter_table q_parm_tbl = { { 0 } };
	union qsb_queue_vbr_parameter_table q_vbr_parm_tbl = { { 0 } };
	unsigned int tmp, reg_val;
	int ep_id = priv->ep_id;
	u32 base = (u32)priv->frwk->ep_dev[ep_id].membase;
	unsigned int qsb_clk = get_qsb_clk(base);
	struct vrx318_param *param = &priv->frwk->param;
	int port;
	unsigned int max_pcr;

	qsb_qos_dbg(qos);
	conn += QSB_QUEUE_NUMBER_BASE; /* qsb qid = firmware qid + 1 */

	/* Peak Cell Rate (PCR) Limiter */
	if (qos->txtp.max_pcr == 0)
		q_parm_tbl.bit.tp = 0; /* disable PCR limiter */
	else {
		/*  peak cell rate would be slightly lower than requested
		 [maximum_rate / pcr = (qsb_clock / 8) * (time_step / 4) / pcr]
		*/
		tmp = ((qsb_clk * param->qsb_tstep) >> 5) / qos->txtp.max_pcr
			+ 1;
		/*  check if overflow takes place   */
		q_parm_tbl.bit.tp
			= tmp > QSB_TP_TS_MAX ? QSB_TP_TS_MAX : tmp;
	}

	/* A funny issue. Create two PVCs, one UBR and one UBR with max_pcr.
	* Send packets to these two PVCs at same time,
	* it triggers strange behavior.
	* In A1, RAM from 0x80000000 to 0x0x8007FFFF was corrupted
	* with fixed pattern 0x00000000 0x40000000.
	* In A4, PPE firmware keep emiting unknown cell
	* and no more response to driver.
	* To work around, create UBR always with max_pcr.
	* If user want to create UBR without max_pcr,
	* we give a default one larger than line-rate.
	*/
	if (qos->txtp.traffic_class == ATM_UBR &&
		q_parm_tbl.bit.tp == 0) {
		port = priv->conn[conn - QSB_QUEUE_NUMBER_BASE].port;
		max_pcr = priv->port[port].tx_max_cell_rate + 1000;

		tmp = ((qsb_clk * param->qsb_tstep) >> 5) / max_pcr + 1;
		if (tmp > QSB_TP_TS_MAX)
			tmp = QSB_TP_TS_MAX;
		else if (tmp < 1)
			tmp = 1;
		q_parm_tbl.bit.tp = tmp;
	}

	/* Weighted Fair Queueing Factor (WFQF) */
	switch (qos->txtp.traffic_class) {
	case ATM_CBR:
	case ATM_VBR_RT:
		/*  real time queue gets weighted fair queueing bypass  */
		q_parm_tbl.bit.wfqf = 0;
		break;
	case ATM_VBR_NRT:
	case ATM_UBR_PLUS:
		/*  WFQF calculation here is based on virtual cell rates,
		      to reduce granularity for high rates
		*/
		/*  WFQF is maximum cell rate / garenteed cell rate */
		/*  wfqf = qsb_minimum_cell_rate * QSB_WFQ_NONUBR_MAX /
			requested_minimum_peak_cell_rate
		*/
		if (qos->txtp.min_pcr == 0)
			q_parm_tbl.bit.wfqf = QSB_WFQ_NONUBR_MAX;
		else {
			tmp = QSB_GCR_MIN * QSB_WFQ_NONUBR_MAX /
				qos->txtp.min_pcr;
			if (tmp == 0)
				q_parm_tbl.bit.wfqf = 1;
			else if (tmp > QSB_WFQ_NONUBR_MAX)
				q_parm_tbl.bit.wfqf
					= QSB_WFQ_NONUBR_MAX;
			else
				q_parm_tbl.bit.wfqf = tmp;
		}
		break;

	case ATM_UBR:
	default:
		q_parm_tbl.bit.wfqf = QSB_WFQ_UBR_BYPASS;
		break;
	}

	/* Sustained Cell Rate (SCR) Leaky Bucket Shaper VBR.0/VBR.1 */
	if (qos->txtp.traffic_class == ATM_VBR_RT ||
		qos->txtp.traffic_class == ATM_VBR_NRT) {
		if (qos->txtp.scr == 0) {
			/* disable shaper */
			q_vbr_parm_tbl.bit.taus = 0;
			q_vbr_parm_tbl.bit.ts = 0;
		} else {
			/* Cell Loss Priority  (CLP) */
			if ((vcc->atm_options & ATM_ATMOPT_CLP))
				/* CLP1 */
				q_parm_tbl.bit.vbr = 1;
			else
				/* CLP0 */
				q_parm_tbl.bit.vbr = 0;
			/* Rate Shaper Parameter (TS) and
			    Burst Tolerance Parameter for SCR (tauS)
			*/
			tmp = ((qsb_clk * param->qsb_tstep) >> 5) /
					qos->txtp.scr + 1;
			q_vbr_parm_tbl.bit.ts
				= tmp > QSB_TP_TS_MAX ? QSB_TP_TS_MAX : tmp;
			tmp = (qos->txtp.mbs - 1) *
				(q_vbr_parm_tbl.bit.ts -
					q_parm_tbl.bit.tp) / 64;
			if (tmp == 0)
				q_vbr_parm_tbl.bit.taus = 1;
			else if (tmp > QSB_TAUS_MAX)
				q_vbr_parm_tbl.bit.taus
					= QSB_TAUS_MAX;
			else
				q_vbr_parm_tbl.bit.taus = tmp;
		}
	} else {
		q_vbr_parm_tbl.bit.taus = 0;
		q_vbr_parm_tbl.bit.ts = 0;
	}

	/* Queue Parameter Table (QPT) */
	vrx318_w32(QSB_QPT_SET_MASK, QSB_RTM, base);
	vrx318_w32(q_parm_tbl.dword, QSB_RTD, base);
	reg_val = QSB_RAMAC_RW_SET(QSB_RAMAC_RW_WRITE) |
			QSB_RAMAC_TSEL_SET(QSB_RAMAC_TSEL_QPT) |
			QSB_RAMAC_LH_SET(QSB_RAMAC_LH_LOW) |
			QSB_RAMAC_TESEL_SET(conn);
	vrx318_w32(reg_val, QSB_RAMAC, base);

	/* Queue VBR Paramter Table (QVPT) */
	vrx318_w32(QSB_QVPT_SET_MASK, QSB_RTM, base);
	vrx318_w32(q_vbr_parm_tbl.dword, QSB_RTD, base);
	reg_val = QSB_RAMAC_RW_SET(QSB_RAMAC_RW_WRITE) |
			QSB_RAMAC_TSEL_SET(QSB_RAMAC_TSEL_VBR) |
			QSB_RAMAC_LH_SET(QSB_RAMAC_LH_LOW) |
			QSB_RAMAC_TESEL_SET(conn);
	vrx318_w32(reg_val, QSB_RAMAC, base);

	qsb_param_dbg(qsb_clk, &q_parm_tbl, &q_vbr_parm_tbl);
	vrx318_dbg(DBG_EVENT, "QSB setting for conn: %d is done\n", conn - 1);
}


/*  Description:
 *    Loop up for connection ID with atm_vcc structure.
 *  Input:
 *    vcc --- struct atm_vcc *, atm_vcc structure of opened connection
 *  Output:
 *    int --- negative value: failed
 *            else          : connection ID
 */
static int find_vcc(struct atm_vcc *vcc)
{
	int i;
	struct vrx318_atm_tc *priv;
	struct atm_pvc *conn;
	int max_pvc = pvc_num();

	if (!vcc)
		return -ENODEV;

	priv = g_atm_tc;
	conn = priv->conn;

	for (i = 0; i < max_pvc; i++)
		if ((priv->pvc_tbl & (1 << i))
			&& conn[i].vcc == vcc)
			return i;

	return -ENODEV;
}

static void mpoa_setup_sync(struct vrx318_atm_tc *priv, int conn, u32 base)
{
	unsigned int sw_txq_tbl;
	int sw_tx_queue;
	wrx_queue_config_t rx_qcfg;
	wtx_queue_config_t tx_qcfg;
	struct dp_subif dp_id;
	struct uni_cell_header *cell_header;
	struct atm_vcc *vcc;
	struct vrx318_param *param = &priv->frwk->param;
	struct net_device *dev;
	char dev_name[32];

	dp_id.port_id = priv->frwk->port_id;
	dp_id.subif = -1;

	if (conn < 0 || conn >= ATM_PVC_NUMBER) {
		vrx318_dbg(DBG_EVENT, "%s: conn: %d is illegal\n", __func__, conn);
		return;
	}

	dev = priv->conn[conn].dev;
	vrx318_dbg(DBG_EVENT, "sync: conn: %d, vpi: %d, vci: %d, mpoa_type: %d, mpoa_mode: %d\n",
		conn, priv->conn[conn].vcc->vpi, priv->conn[conn].vcc->vci,
		priv->conn[conn].mpoa_type, priv->conn[conn].mpoa_mode);

	if (!dev)
		sprintf(dev_name, "atm_%d%d",
			priv->conn[conn].vcc->vpi, priv->conn[conn].vcc->vci);

	if (dp_register_subif(priv->frwk->owner, dev, (!dev) ? dev_name : dev->name,
			&dp_id, 0) != DP_SUCCESS) {
		vrx318_dbg(DBG_ERR,
			"%s: Failed to regsiter sub itf: dev: %s, dp_id: %d, subif: %d\n",
			__func__, (!dev) ? "" : dev->name,
			dp_id.port_id, dp_id.subif);
	}
	priv->conn[conn].subif_id = dp_id.subif;
	vcc = priv->conn[conn].vcc;

	/*  set htu entry   */
	set_htu_entry(priv, vcc->vpi, vcc->vci, conn,
		vcc->qos.aal == ATM_AAL5 ? 1 : 0);

	dword_clear(&rx_qcfg, sizeof(rx_qcfg));
	/* setup RX queue cfg and TX queue cfg */
	rx_qcfg.new_vlan	= 0; /* No Vlan insert for GRX500 */
	rx_qcfg.vlan_ins	= 0; /* No Vlan insert */
	rx_qcfg.mpoa_type	= priv->conn[conn].mpoa_type;
	rx_qcfg.ip_ver		= ATM_IP_IPV4; /* set IPv4 as default */
	rx_qcfg.mpoa_mode	= priv->conn[conn].mpoa_mode;
	rx_qcfg.oversize	= param->aal5r_max_pktsz;
	rx_qcfg.undersize	= param->aal5r_min_pktsz;
	rx_qcfg.mfs		= param->aal5s_max_pktsz;
	rx_qcfg.cpimask		= 0xFF;
	rx_qcfg.uumask		= 0xFF;

	dword_mem_write(SB_BUFFER(WRX_Q_CFG(conn)), &rx_qcfg,
			sizeof(rx_qcfg), base);

	dword_clear(&tx_qcfg, sizeof(tx_qcfg));
	/* only one TX queue is assigned now */
	/* use ioctl/proc to add other TX queues */
	tx_qcfg.same_vc_qmap   = 0x00;
	tx_qcfg.sbid           = priv->conn[conn].port;
	/* qsb qid = firmware qid + 1 */
	tx_qcfg.qsb_vcid       = conn + QSB_QUEUE_NUMBER_BASE;
	tx_qcfg.mpoa_mode      = priv->conn[conn].mpoa_mode;
	/* reserved in A4, however put 1 for backward compatible */
	tx_qcfg.qsben          = 1;

	cell_header = (struct uni_cell_header *)((unsigned int *)&tx_qcfg + 2);
	cell_header->clp = (vcc->atm_options & ATM_ATMOPT_CLP) ? 1 : 0;
	cell_header->pti = ATM_PTI_US0;
	cell_header->vci = vcc->vci;
	cell_header->vpi = vcc->vpi;
	cell_header->gfc = 0;

	for (sw_txq_tbl = priv->conn[conn].sw_txq_tbl, sw_tx_queue = 0;
		sw_tx_queue < pvc_num();
		sw_tx_queue++) {
		if ((sw_txq_tbl & BIT(sw_tx_queue)) != 0) {
			tx_qcfg.same_vc_qmap
				= sw_txq_tbl & ~(BIT(sw_tx_queue));
			dword_mem_write(
				SB_BUFFER(WTX_Q_CFG(sw_tx_queue)),
				&tx_qcfg, sizeof(tx_qcfg), base);
		}
	}

	/*  set qsb */
	set_qsb(priv, vcc, &vcc->qos, conn);

}

static void mpoa_setup(struct atm_vcc *vcc, int mpoa_type,
			int f_llc, struct net_device *dev)
{
	int conn;
	struct vrx318_atm_tc *priv;
	int ep_id;
	u32 base;

	vrx318_dbg(DBG_EVENT, "Add VPC/VCI: mpoa_type: %d, f_llc: %d, dev: %s\n",
		mpoa_type, f_llc, dev->name);
	priv = (struct vrx318_atm_tc *)vcc->dev->dev_data;
	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;

	spin_lock_bh(&priv->atm_lock);
	conn = find_vcc(vcc);
	if (conn < 0)
		goto err1;

	priv->conn[conn].dev = dev;
	priv->conn[conn].mpoa_mode = f_llc;
	priv->conn[conn].mpoa_type = mpoa_type;
	spin_unlock_bh(&priv->atm_lock);

	mpoa_setup_sync(priv, conn, base);

	return;

err1:
	spin_unlock_bh(&priv->atm_lock);
	return;
}

static void setup_oam_htu_entry(struct vrx318_atm_tc *priv, u32 base)
{
	struct htu_entry htu_entry = {0};
	struct htu_result htu_result = {0};
	struct htu_mask htu_mask = {0};
	int ven = 1;
	int i;
	int max_pvc = pvc_num();

	if (priv->frwk->param.dfe_loopback)
		ven = 0;
	else
		ven = 1;

	htu_mask.set = 0x03;

	/* Clear HTU Tables */
	for (i = 0; i < max_pvc; i++) {
		htu_result.qid = (unsigned int)i;
		dword_mem_write(
			SB_BUFFER(HTU_ENTRY_TBL(i + OAM_HTU_ENTRY_NUMBER)),
			&htu_entry, sizeof(htu_entry), base);
		dword_mem_write(
			SB_BUFFER(HTU_MASK_TBL(i + OAM_HTU_ENTRY_NUMBER)),
			&htu_mask, sizeof(htu_mask), base);
		dword_mem_write(
			SB_BUFFER(HTU_RESULT_TBL(i + OAM_HTU_ENTRY_NUMBER)),
			&htu_result, sizeof(htu_result), base);
	}

	/*  OAM HTU Entry   */
	htu_entry.vci		= 0x03;
	htu_mask.pid_mask	= 0x03;
	htu_mask.vpi_mask	= 0xFF;
	htu_mask.vci_mask	= 0x0000;
	htu_mask.pti_mask	= 0x07;
	htu_result.cellid	= 0;
	htu_result.type		= 1;
	htu_result.ven		= ven;
	htu_result.qid		= 0;
	dword_mem_write(SB_BUFFER(HTU_RESULT_TBL(OAM_F4_SEG_HTU_ENTRY)),
		&htu_result, sizeof(htu_result), base);
	dword_mem_write(SB_BUFFER(HTU_MASK_TBL(OAM_F4_SEG_HTU_ENTRY)),
		&htu_mask, sizeof(htu_mask), base);
	dword_mem_write(SB_BUFFER(HTU_ENTRY_TBL(OAM_F4_SEG_HTU_ENTRY)),
		&htu_entry, sizeof(htu_entry), base);

	htu_entry.vci		= 0x04;
	htu_result.cellid	= 0;
	htu_result.type		= 1;
	htu_result.ven		= ven;
	htu_result.qid		= 0;
	dword_mem_write(SB_BUFFER(HTU_RESULT_TBL(OAM_F4_TOT_HTU_ENTRY)),
		&htu_result, sizeof(htu_result), base);
	dword_mem_write(SB_BUFFER(HTU_MASK_TBL(OAM_F4_TOT_HTU_ENTRY)),
		&htu_mask, sizeof(htu_mask), base);
	dword_mem_write(SB_BUFFER(HTU_ENTRY_TBL(OAM_F4_TOT_HTU_ENTRY)),
		&htu_entry, sizeof(htu_entry), base);

	htu_entry.vci		= 0x00;
	htu_entry.pti		= 0x04;
	htu_mask.vci_mask	= 0xFFFF;
	htu_mask.pti_mask	= 0x01;
	htu_result.cellid	= 0;
	htu_result.type		= 1;
	htu_result.ven		= ven;
	htu_result.qid		= 0;
	dword_mem_write(SB_BUFFER(HTU_RESULT_TBL(OAM_F5_HTU_ENTRY)),
		&htu_result, sizeof(htu_result), base);
	dword_mem_write(SB_BUFFER(HTU_MASK_TBL(OAM_F5_HTU_ENTRY)),
		&htu_mask, sizeof(htu_mask), base);
	dword_mem_write(SB_BUFFER(HTU_ENTRY_TBL(OAM_F5_HTU_ENTRY)),
		&htu_entry, sizeof(htu_entry), base);
}

static int atm_showtime_enter(const unsigned char line_idx,
		struct port_cell_info *port_cell, void *xdata_addr)
{
	int i, j;
	struct vrx318_atm_tc *priv = g_atm_tc;
	u32 base;

	if (!priv)
		return -ENODEV;

	base = (u32)priv->frwk->ep_dev[priv->ep_id].membase;
	VRX318_ASSERT(port_cell == NULL, "port_cell is NULL");
	VRX318_ASSERT(xdata_addr != NULL, "xdata_addr is NOT NULL");

	if (port_cell == NULL)
		return -EINVAL;

	if (line_idx != priv->ep_id)
		return 0;

	for (j = 0; j < ATM_PORT_NUMBER && j < port_cell->port_num; j++)
		if (port_cell->tx_link_rate[j] > 0)
			break;

	if (j >= ATM_PORT_NUMBER) {
		vrx318_dbg(DBG_ERR, "Invalid tx link rate: %d, %d\n",
			port_cell->tx_link_rate[0],
			port_cell->tx_link_rate[1]);
		return -EINVAL;
	}
	for (i = 0; i < ATM_PORT_NUMBER && i < port_cell->port_num; i++)
		priv->port[i].tx_max_cell_rate
			= port_cell->tx_link_rate[i] > 0 ?
				port_cell->tx_link_rate[i] :
				port_cell->tx_link_rate[j];

	qsb_global_set(priv, base);

	for (i = 0; i < pvc_num(); i++)
		if ((priv->pvc_tbl & BIT(i)) && priv->conn[i].vcc != NULL)
			set_qsb(priv, priv->conn[i].vcc,
				&priv->conn[i].vcc->qos, i);

	spin_lock_bh(&priv->frwk->fw_lock);
	if (priv->frwk->tc_stat == VRX318_RUNNING)
		priv->frwk->show_time_stat[line_idx] = 1;
	else
		vrx318_dbg(DBG_ERR, "ATM reach show time while TC status is not running!!!, line: %d, tc stat: %d\n",
			line_idx, priv->frwk->tc_stat);
	spin_unlock_bh(&priv->frwk->fw_lock);

    atm_dev_signal_change(priv->port[line_idx].dev, ATM_PHY_SIG_FOUND);
	vrx318_dbg(DBG_INFO, "ATM line[%d]:enter showtime, cell rate: 0 - %d, 1 - %d\n",
		line_idx, priv->port[0].tx_max_cell_rate,
		priv->port[1].tx_max_cell_rate);

	return 0;
}

static int atm_showtime_exit(const unsigned char line_idx)
{
	if (!g_atm_tc || g_atm_tc->ep_id != line_idx)
		return 0;

	g_atm_tc->frwk->show_time_stat[line_idx] = 0;
	/* cbm_dp_enable(g_atm_tc->frwk->owner, g_atm_tc->frwk->port_id,
		CBM_F_DPATH_DISABLE);
	*/

    atm_dev_signal_change(g_atm_tc->port[line_idx].dev, ATM_PHY_SIG_LOST);
	vrx318_dbg(DBG_INFO, "ATM line[%d]:leave showtime\n", line_idx);

	return 0;
}

static int atm_get_pvc_id(struct sk_buff *skb)
{
	struct pmac_rx_hdr *pmac_hdr = (struct pmac_rx_hdr *)skb->data;

	return (pmac_hdr->src_sub_inf_id2 >> 1) & 0xF;
}

static inline void dump_atm_data(unsigned char *data, int len, int rx)
{
	if (rx) {
		if (g_vrx318_dbg & DBG_PKT_RX_DUMP)
			dp_dump_raw_data(data, len, "Dump atm rx data");
	} else {
		if (g_vrx318_dbg & DBG_PKT_TX_DUMP)
			dp_dump_raw_data(data, len, "Dump atm tx data");
	}
}

static void atm_encapsulate_frame(struct sk_buff *skb, int conn)
{
	unsigned int proto_type;
	int mpoa_type, mpoa_mode;

	mpoa_type = g_atm_tc->conn[conn].mpoa_type;
	mpoa_mode = g_atm_tc->conn[conn].mpoa_mode;

	/* ETH packet, need recover ATM encapsulation  */
	if (mpoa_mode) {
		/*  LLC */
		switch (mpoa_type) {
		case 0: /* EoA w/o FCS */
			VRX318_ASSERT(skb_headroom(skb) >= 10,
				"not enough skb headroom (LLC EoA w/o FCS)");
			skb_push(skb, 10);
			((u32 *)skb->data)[0] = 0xAAAA0300;
			((u32 *)skb->data)[1] = 0x80C20007;
			((u16 *)skb->data)[4] = 0x0000;
			break;

		case 1: /* EoA w FCS */
			VRX318_ASSERT(skb_headroom(skb) >= 10,
				"not enough skb headroom (LLC EoA w FCS)");
			skb_push(skb, 10);
			((u32 *)skb->data)[0] = 0xAAAA0300;
			((u32 *)skb->data)[1] = 0x80C20001;
			((u16 *)skb->data)[4] = 0x0000;
			break;

		case 2: /* PPPoA */
			proto_type = ntohs(*(u16 *)(skb->data + 12));
			switch (proto_type) {
			case 0x0800:
				proto_type = 0x0021;
				break;
			case 0x86DD:
				proto_type = 0x0057;
				break;
			}
			skb_pull(skb, ETH_HLEN - 6);
			((u32 *)skb->data)[0] = 0xFEFE03CF;
			((u16 *)skb->data)[2] = (u16)proto_type;
			break;

		case 3: /*  IPoA */
			skb_pull(skb, ETH_HLEN - 8);
			((u32 *)skb->data)[0] = 0xAAAA0300;
			((u16 *)skb->data)[2] = 0x0000;
			break;
		}
	} else {
		/*  VC-mux  */
		switch (mpoa_type) {
		case 0: /* EoA w/o FCS */
			VRX318_ASSERT(skb_headroom(skb) >= 2,
				"not enough skb headroom (VC-mux EoA w/o FCS)");
			skb_push(skb, 2);
			*(u16 *)skb->data = 0x0000;
			break;

		case 1: /* EoA w FCS */
			VRX318_ASSERT(skb_headroom(skb) >= 2,
				"not enough skb headroom (VC-mux EoA w FCS)");
			skb_push(skb, 2);
			*(u16 *)skb->data = 0x0000;
			break;

		case 2: /* PPPoA */
			proto_type = ntohs(*(u16 *)(skb->data + 12));
			switch (proto_type) {
			case 0x0800:
				proto_type = 0x0021;
				break;
			case 0x86DD:
				proto_type = 0x0057;
				break;
			}
			skb_pull(skb, ETH_HLEN - 2);
			*(u16 *)skb->data = (u16)proto_type;
			break;
		case 3: /*  IPoA */
			skb_pull(skb, ETH_HLEN);
			break;
		}
	}
	dump_atm_data(skb->data, skb->len, 1);
}


static int atm_push(struct net_device *rxif, struct net_device *txif,
			struct sk_buff *skb, int32_t len)
{
	int conn;
	struct atm_vcc *vcc = NULL;
	struct vrx318_atm_tc *priv = g_atm_tc;
	int max_pvc = pvc_num();

	conn = atm_get_pvc_id(skb);
#if defined(CONFIG_LTQ_PPA_API_SW_FASTPATH)
    skb->mark &= ~FLG_PPA_PROCESSED;
#endif

	if (unlikely(conn < 0 || conn >= max_pvc
		     || (priv->conn[conn].vcc == NULL)
		     || (priv->pvc_tbl & BIT(conn)) == 0)) {
		vrx318_dbg(DBG_PKT_RX, "invalid conn: %d or NULL Vcc\n", conn);

	} else {
		vcc = priv->conn[conn].vcc;
		if (atm_charge(vcc, skb->truesize)) {

			if (vcc->stats)
				atomic_inc(&vcc->stats->rx);

			spin_lock_bh(&priv->atm_lock);
			if (vcc->qos.aal == ATM_AAL5) {
				priv->stats.aal5_rx_pkts++;
				priv->stats.aal5_rx_bytes += skb->len;
				priv->conn[conn].stats.aal5_rx_pkts++;
				priv->conn[conn].stats.aal5_rx_bytes
					+= skb->len;
			} else
				priv->stats.aal5_rx_errors++;
			priv->conn[conn].access_time = current_kernel_time();
			spin_unlock_bh(&priv->atm_lock);

			/* remove PMAC header */
			skb_pull(skb, 8);

			ATM_SKB(skb)->vcc = vcc;
			atm_encapsulate_frame(skb, conn);

			vcc->push(vcc, skb);

			return 0;
		} else {
			vrx318_dbg(DBG_PKT_RX, "inactive qid %d\n", conn);

			if (vcc->stats)
				atomic_inc(&vcc->stats->rx_drop);

			spin_lock_bh(&priv->atm_lock);
			if (vcc->qos.aal == ATM_AAL5)
				priv->stats.aal5_rx_dropped++;
			priv->conn[conn].stats.aal5_rx_dropped++;
			spin_unlock_bh(&priv->atm_lock);
		}
	}

	dev_kfree_skb_any(skb);
	return 0;
}

static int atm_get_qid_by_vcc(struct net_device *netif, struct sk_buff *skb,
			struct atm_vcc *vcc, uint8_t dst_mac[MAX_ETH_ALEN],
			dp_subif_t *subif, uint32_t flags)
{
	struct vrx318_atm_tc *priv = g_atm_tc;
	int qid, mpoa_type, prio;
	int conn;

	if (!subif || vcc == NULL) {
		vrx318_dbg(DBG_ERR,
			"%s: VCC or subif is NULL\n", __func__);
		return -EINVAL;
	}

	if (subif->port_id != priv->frwk->port_id) {
		vrx318_dbg(DBG_ERR,
			"%s: Port_id not compliance: Input: %d, Local: %d\n",
			__func__, subif->port_id, priv->frwk->port_id);
		return -EINVAL;
	}

	spin_lock_bh(&priv->atm_lock);
	conn = find_vcc(vcc);
	if (conn < 0) {
		vrx318_dbg(DBG_ERR,
			"%s: given VCC is not attached\n", __func__);
		spin_unlock_bh(&priv->atm_lock);
		return -ENODEV;

	} else {
		if (skb)
			prio = skb->priority < ATM_PRIO_Q_NUM ?
				skb->priority : ATM_PRIO_Q_NUM - 1;
		else
			prio = 0;
		qid = priv->conn[conn].prio_queue_map[prio];
		mpoa_type = priv->conn[conn].mpoa_type;
		subif->subif = priv->conn[conn].subif_id
				| ATM_DESC_SUBIF_ID(qid, 0, mpoa_type);
	}

	spin_unlock_bh(&priv->atm_lock);

	return 0;
}

#ifdef CONFIG_LTQ_VRX318_CPUFREQ
static int32_t atm_coc_confirm_stat(enum ltq_cpufreq_state new_state,
	enum ltq_cpufreq_state old_state,
	uint32_t flags)
{
	struct vrx318_atm_tc *priv = g_atm_tc;
	int ret = -ENODEV;
	u32 base;
	int ep_id;

	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;
	vrx318_dbg(DBG_COC, "Current state: %u, old [%u] to [%u]\n",
		atm_coc_state, (u32)old_state, (u32)new_state);

	if (new_state != atm_coc_state) {
		/* assign new state */
		vrx318_dbg(DBG_COC, "Change from current state %u to new %u\n",
			atm_coc_state, new_state);
		atm_coc_state = new_state;
		/* check for D3 state */
		if (atm_coc_state == LTQ_CPUFREQ_PS_D3) {
			u32 mbox1_ier;
			vrx318_dbg(DBG_COC, "Change to D3, enable interrupt\n");
			/* Enable interrupt for DS packet */
			spin_lock_bh(&priv->oam_lock);
			mbox1_ier = vrx318_r32(MBOX_IGU1_IER, base);
			mbox_set_ier(MBOX_IGU1, mbox1_ier | PKT_RX_INTR, base);
			spin_unlock_bh(&priv->oam_lock);
		} else {
			/* Disable interrupt for DS packet */
			u32 mbox1_ier;
			vrx318_dbg(DBG_COC, "Change to %u, disable interrupt\n", atm_coc_state);
			/* Disable interrupt for DS packet */
			spin_lock_bh(&priv->oam_lock);
			mbox1_ier = vrx318_r32(MBOX_IGU1_IER, base);
			mbox1_ier &= ~PKT_RX_INTR;
			mbox_set_ier(MBOX_IGU1, mbox1_ier, base);
			spin_unlock_bh(&priv->oam_lock);
		}
	}
	vrx318_dbg(DBG_COC, "Complete\n");
	return ret;
}
#endif

static int vrx318_atm_datapath_init(struct vrx318_atm_tc *priv)
{
	int err;
	struct vrx318_priv *fw_priv = priv->frwk;
	int ep_id = priv->ep_id;
	int irq_id = fw_priv->ep_dev[ep_id].irq;
	u32 base = (u32)fw_priv->ep_dev[ep_id].membase;

	/* request irq (enable by default) */
	err = devm_request_irq(fw_priv->pdev, irq_id, mailbox_irq_handler, 0,
				g_atm_dev_name, (void *)priv);
	if (err) {
		vrx318_dbg(DBG_ERR, "Failed to request PCIe MSI irq %u\n",
			irq_id);
		return err;
	}

	qsb_global_set(priv, base);
	setup_oam_htu_entry(priv, base);

	atm_hook_mpoa_setup = mpoa_setup;
	vrx318_cb_setup(atm_push, NULL, NULL, atm_get_qid_by_vcc
#ifdef CONFIG_LTQ_VRX318_CPUFREQ
		, atm_coc_confirm_stat
#else
		, NULL
#endif
	);

	ppa_callback_set(LTQ_MEI_SHOWTIME_ENTER, atm_showtime_enter);
	ppa_callback_set(LTQ_MEI_SHOWTIME_EXIT, atm_showtime_exit);

	return 0;
}

/* This API no lock protect, should only be called in initialization part */
static void vrx318_atm_fw_cfg_init(struct vrx318_atm_tc *priv, u32 base)
{
	int conn;

	if (priv->pvc_tbl)
		validate_oam_htu_entry(base, 1);
	else
		return;

	for (conn = 0; (priv->pvc_tbl & BIT(conn)) != 0 && conn < pvc_num(); conn++)
		mpoa_setup_sync(priv, conn, base);

}

static int vrx318_atm_tc_hw_fw_init(struct vrx318_atm_tc *priv)
{
	int ep_id;
	u32 base;

	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;

	vrx318_dbg(DBG_INFO, "port	  = %d\n", ep_id);
	vrx318_dbg(DBG_INFO, "irq	  = %u\n",
		priv->frwk->ep_dev[ep_id].irq);
	vrx318_dbg(DBG_INFO, "membase	  = 0x%08x\n",
		(unsigned int)priv->frwk->ep_dev[ep_id].membase);
	vrx318_dbg(DBG_INFO, "phy_membase = 0x%08x\n",
		(unsigned int)priv->frwk->ep_dev[ep_id].phy_membase);

	/* Reset PPE HW */
	reset_ppe(base);
	/* activate VRX318 */
	enable_ppe_ema(base);
	/*  Set VRX318 PPE clock 432MHz */
	set_ppe_clk(432, base);
	/* Set QSB CLK divisor */
	set_qsb_clk(0, base);
	/* clear and disable mailbox	*/
	mbox_clr_isr(MBOX_IGU0, 0xFFFFFFFF, base);
	mbox_clr_isr(MBOX_IGU1, 0xFFFFFFFF, base);
	mbox_set_ier(MBOX_IGU0, 0, base);
	mbox_set_ier(MBOX_IGU1, 0, base);

	/*  freeze PP32 */
	pp32_stop(base);
	vrx318_ppe_atm_init(priv, 1);
	pp32_load(base);

	/* init datapath */
	vrx318_atm_datapath_init(priv);

	/* start hardware */
	setup_dfe_loopback(priv->frwk, base);
	pp32_start(0, base);
	dfe_loopback_linkup(priv->frwk, base);

	/* Init FW tx/rx queue /qsb configuration for pre-set PVCs */
	vrx318_atm_fw_cfg_init(priv, base);

	/* Indicate DSL FW all configuration is ready */
	/* PPE FW is ready to receive/send packets */
	vrx318_tc_indicate(TC_ATM_SL_MODE, base, 1);

	/* enable mailbox Interrupts */
	mbox_set_ier(MBOX_IGU0, 0, base);
	mbox_set_ier(MBOX_IGU1, OAM_RX_INTR | FW_UNLOAD_INTR, base);/* enable OAM, TC switch */

	get_fw_version(&priv->fw_ver, base);
	print_fw_version(NULL, &priv->fw_ver);

	return 0;
}

static void vrx318_atm_cfg_init(struct vrx318_priv *priv)
{
	/* Current ATM FW must disable cdma write */
	priv->param.cdma_wr_en = 0;
}

int vrx318_atm_tc_init(struct vrx318_priv *frwk_priv,
			int ep_id, enum dsl_tc_mode tc_mode)
{
	int err;
	struct vrx318_atm_tc *priv;

	if (!g_atm_tc) { /*First time initialization */
		priv = kzalloc(sizeof(*priv), GFP_KERNEL);
		if (!priv) {
			vrx318_dbg(DBG_ERR, "Failed to alloc priv buffer\n");
			err = -ENOMEM;
			goto err1;
		}
		priv->frwk = frwk_priv;
		spin_lock_init(&priv->atm_lock);
		spin_lock_init(&priv->oam_lock);
		init_completion(&priv->comp);
		err = vrx318_atm_init(priv);
		if (err)
			goto err2;

		g_atm_tc = priv;
	} else
		priv = g_atm_tc;

	vrx318_atm_cfg_init(frwk_priv);
	priv->ep_id = ep_id;
	err = vrx318_atm_tc_hw_fw_init(priv);
	if (err)
		goto err2;
	frwk_priv->tc_priv = (void *)priv;
	vrx318_atm_pmac_cfg_init(frwk_priv->port_id);
	vrx318_atm_proc_init(priv);

	/*netlink message for TC UP */
	vrx_tc_msg_send(0, ATM_TC, TC_LOADED, NON_BONDING_MODE, ep_id);
	return 0;

err2:
	kfree(priv);
	priv = NULL;
	g_atm_tc = NULL;
err1:
	return err;
}

void vrx318_atm_free_mem(struct vrx318_atm_tc *priv, u32 base)
{
	int i;
	int phy_addr;

	/* Free oam descriptors */
	for (i = 0; i < priv->oam_llst.oam_num; i++)
		cbm_buffer_free(0, priv->oam_llst.oam_des_list[i], 0);

	/* Free QoS queue buffers */
	for (i = 0; i < TOTAL_QOS_DES_NUM; i++) {
		phy_addr = vrx318_r32_sb(__US_QOSQ_DES_LIST_BASE + (i * 2) + 1,
					base) - VRX318_OB_BADDR;
		cbm_buffer_free(0, phy_addr, 1);
	}

	/* Free DS AAL5 buffers, not required in GRX350 */
}

int vrx318_atm_unregister_pvc(struct vrx318_atm_tc *priv)
{
	int i;
	dp_subif_t dp_id;
	int err = 0;
	char dev_name[32];

	spin_lock_bh(&priv->atm_lock);
	for (i = 0; i < pvc_num(); i++) {
		if ((priv->pvc_tbl & BIT(i)) != 0) {
			dp_id.port_id = priv->frwk->port_id;
			dp_id.subif = priv->conn[i].subif_id;
			if (!priv->conn[i].dev) {
				sprintf(dev_name, "atm_%d%d",
					priv->conn[i].vcc->vpi,
					priv->conn[i].vcc->vci);
			}
			err = dp_register_subif(priv->frwk->owner,
				priv->conn[i].dev,
				(!priv->conn[i].dev) ?
					dev_name : priv->conn[i].dev->name,
				&dp_id,
				DP_F_DEREGISTER);
			if (err)
				goto DP_DEREG_FAIL;
		}
	}
	spin_unlock_bh(&priv->atm_lock);

	return err;

DP_DEREG_FAIL:
	spin_unlock_bh(&priv->atm_lock);

	return err;
}

int vrx318_unload_atm_tc(struct vrx318_priv *frwk_priv, int ep_id)
{
	struct vrx318_atm_tc *priv = g_atm_tc;
	u32 base = (u32)frwk_priv->ep_dev[ep_id].membase;
	int err;

	if (in_interrupt()) {
		vrx318_dbg(DBG_ERR, "VRX318 cannot be unloaded in interrupt context\n");
		return -EINVAL;
	}

	if (priv != frwk_priv->tc_priv) {
		vrx318_dbg(DBG_ERR, "TC pointer not match as ATM TC pointer\n");
		return -ENODEV;
	}

	/* Indicate DSL FW TC Clear */
	vrx318_tc_indicate(0, base, 0);

	/* Reset xDSL MEI */
	/* vrx318_reset_xdsl(base); */

	/* Unregister PVC/subif id */
	err = vrx318_atm_unregister_pvc(priv);
	if (err) {
		vrx318_dbg(DBG_TC_SWITCH, "Unregister PVC fail!\n");
		return err;
	}

	validate_oam_htu_entry(base, 0);

	/* Indicate PPE FW to stop */
	priv->comp.done = 0;
	err = wait_for_completion_timeout(&priv->comp,
			msecs_to_jiffies(FW_STOP_TIMEOUT));

	if (err < 0) {
		vrx318_dbg(DBG_TC_SWITCH, "Wait process got interrupted\n");
		return err;
	}

	if (!err)
		vrx318_dbg(DBG_TC_SWITCH, "Wait process Timeout\n");

	/* Stop PPE FW */
	pp32_stop(base);

	/* Unregister Interrupt handler */
	devm_free_irq(frwk_priv->pdev,
		frwk_priv->ep_dev[ep_id].irq, (void *)priv);
	/* Kill all tasklets */
	tasklet_kill(&g_oam_tasklet);
	/* Disable and Clear all Interrupts */
	mbox_set_ier(MBOX_IGU0, 0, base);
	mbox_set_ier(MBOX_IGU1, 0, base);
	mbox_clr_isr(MBOX_IGU0, 0xFFFFFFFF, base);
	mbox_clr_isr(MBOX_IGU1, 0xFFFFFFFF, base);

	/* Free CBM resources */
	vrx318_atm_free_mem(priv, base);
	/* Reset CBM EQM/DQM idx */
	soc_vrx318_cfg_reset();

	/* Free Proc */
	vrx318_atm_proc_exit(priv);

	/* netlink message for TC DOWN */
	vrx_tc_msg_send(0, ATM_TC, TC_UNLOADED, NON_BONDING_MODE, ep_id);
	vrx318_dbg(DBG_TC_SWITCH, "Unload ATM TC successfully!!!\n");

	return err;
}

static int ppe_open(struct atm_vcc *vcc)
{
	int vpi = vcc->vpi;
	int vci = vcc->vci;
	struct vrx318_atm_tc *priv = g_atm_tc;
	struct atm_port *port = &priv->port[(int)vcc->dev->phy_data];
	int sw_txq;
	int conn, i, ret;
	u32 base;

	base = (u32)priv->frwk->ep_dev[priv->ep_id].membase;

	if (priv->frwk->tc_stat != VRX318_RUNNING
		|| priv->frwk->tc_mode[priv->ep_id] != TC_ATM_SL_MODE) {
		vrx318_dbg(DBG_ERR, "PPE Open fail! TC Status: %d, TC Mode: %d\n",
			(u32)priv->frwk->tc_stat,
			(u32)priv->frwk->tc_mode[priv->ep_id]);
		return -EACCES;
	}

	if (vcc->qos.aal != ATM_AAL5 && vcc->qos.aal != ATM_AAL0) {
		vrx318_dbg(DBG_ERR, "aal proto not supported: %d\n",
			vcc->qos.aal);
		return -EPROTONOSUPPORT;
	}

	/* check bandwidth */
	if ((vcc->qos.txtp.traffic_class == ATM_CBR &&
		vcc->qos.txtp.max_pcr >
			(port->tx_max_cell_rate - port->tx_used_cell_rate))
		|| (vcc->qos.txtp.traffic_class == ATM_VBR_RT &&
		vcc->qos.txtp.max_pcr >
			(port->tx_max_cell_rate - port->tx_used_cell_rate))
		|| (vcc->qos.txtp.traffic_class == ATM_VBR_NRT &&
		vcc->qos.txtp.scr >
			(port->tx_max_cell_rate - port->tx_used_cell_rate))
		|| (vcc->qos.txtp.traffic_class == ATM_UBR_PLUS &&
		vcc->qos.txtp.min_pcr >
			(port->tx_max_cell_rate - port->tx_used_cell_rate))) {
		vrx318_dbg(DBG_ERR, "exceed TX line rate\n");
		return -EINVAL;
	}

	/*  check existing vpi,vci  */
	conn = find_vpivci(vpi, vci);
	if (conn >= 0) {
		vrx318_dbg(DBG_ERR, "existing PVC (%d.%d)\n", vpi, vci);
		return -EADDRINUSE;
	}

	spin_lock_bh(&priv->atm_lock);
	/*  allocate software TX queue  */
	for (sw_txq = 0; sw_txq < txq_num(); sw_txq++)
		if (!(priv->sw_txq_tbl & BIT(sw_txq)))
			break;

	if (sw_txq == txq_num()) {
		ret = -ENOSPC;
		vrx318_dbg(DBG_ERR, "no free TX queue\n");
		goto OPEN_ERR;
	}

	if (!priv->pvc_tbl)
		validate_oam_htu_entry(base, 1);
	/* allocate PVC */
	for (conn = 0; conn < pvc_num(); conn++)
		if (!(priv->pvc_tbl & BIT(conn))) {
			priv->sw_txq_tbl |= BIT(sw_txq);
			priv->conn[conn].vcc = vcc;
			priv->conn[conn].port
				= (unsigned int)vcc->dev->phy_data;
			priv->conn[conn].sw_txq_tbl = BIT(sw_txq);
			for (i = 0; i < 8; i++)
				priv->conn[conn].prio_queue_map[i] = sw_txq;
			priv->pvc_tbl |= BIT(conn);
			break;
		}

	if (conn >= pvc_num()) {
		ret = -ENOSPC;
		vrx318_dbg(DBG_ERR, "exceed PVC limit\n");
		goto OPEN_ERR;
	}
	spin_unlock_bh(&priv->atm_lock);

	/*  reserve bandwidth */
	switch (vcc->qos.txtp.traffic_class) {
	case ATM_CBR:
	case ATM_VBR_RT:
		port->tx_used_cell_rate += vcc->qos.txtp.max_pcr;
		break;
	case ATM_VBR_NRT:
		port->tx_used_cell_rate += vcc->qos.txtp.scr;
		break;
	case ATM_UBR_PLUS:
		port->tx_used_cell_rate += vcc->qos.txtp.min_pcr;
		break;
	}

	/* update atm_vcc structure */
	vcc->itf = (int)vcc->dev->phy_data;
	set_bit(ATM_VF_ADDR, &vcc->flags);
	set_bit(ATM_VF_READY, &vcc->flags);

	vrx318_dbg(DBG_EVENT, "ppe_open(%d.%d): conn = %d, ep_id: %d\n",
		vcc->vpi, vcc->vci, conn, priv->ep_id);

	return 0;

OPEN_ERR:
	spin_unlock_bh(&priv->atm_lock);
	return ret;
}

static void ppe_close(struct atm_vcc *vcc)
{
	int cid;
	struct atm_port *port;
	struct atm_pvc *conn;
	struct vrx318_atm_tc *priv = vcc->dev->dev_data;
	u32 base = (u32)priv->frwk->ep_dev[priv->ep_id].membase;
	struct net_device *dev;
	dp_subif_t dp_id;
	char dev_name[32];

	if (!vcc)
		return;

	if (priv->frwk->tc_stat != VRX318_RUNNING
		|| priv->frwk->tc_mode[priv->ep_id] != TC_ATM_SL_MODE) {
		vrx318_dbg(DBG_ERR, "PPE Close fail! TC Status: %d, TC Mode: %d\n",
			priv->frwk->tc_stat, priv->frwk->tc_mode[priv->ep_id]);
		return;
	}

	vrx318_dbg(DBG_EVENT, "ppe close, vpi:%d, vci:%d\n",
		vcc->vpi, vcc->vci);

	/*  get connection id   */
	cid = find_vcc(vcc);
	if (cid < 0) {
		vrx318_dbg(DBG_ERR, "can't find vcc\n");
		return;
	}

	conn = &priv->conn[cid];
	port = &priv->port[conn->port];

	clear_bit(ATM_VF_READY, &vcc->flags);
	clear_bit(ATM_VF_ADDR, &vcc->flags);

	/* clear htu */
	clear_htu_entry(cid, base);

	/*  release connection  */
	spin_lock_bh(&priv->atm_lock);
	priv->pvc_tbl &= ~(BIT(cid));
	priv->sw_txq_tbl &= ~(conn->sw_txq_tbl);
	dev = conn->dev;
	if (!dev)
		sprintf(dev_name, "atm_%d%d",
			conn->vcc->vpi, conn->vcc->vci);
	dp_id.port_id = priv->frwk->port_id;
	dp_id.subif = conn->subif_id;
	memset(conn, 0, sizeof(*conn));
	if (priv->pvc_tbl == 0)
		validate_oam_htu_entry(base, 0);
	spin_unlock_bh(&priv->atm_lock);

	/* Unregister subif from datapath lib */
	if (dp_register_subif(priv->frwk->owner, dev, (!dev) ? dev_name : dev->name,
			&dp_id, DP_F_DEREGISTER) != DP_SUCCESS)
		vrx318_dbg(DBG_ERR, "%s: Failed to deregsiter sub-itf: %s\n",
			__func__, dev ? dev->name : "");

	/* release bandwidth */
	switch (vcc->qos.txtp.traffic_class) {
	case ATM_CBR:
	case ATM_VBR_RT:
		port->tx_used_cell_rate -= vcc->qos.txtp.max_pcr;
		break;
	case ATM_VBR_NRT:
		port->tx_used_cell_rate -= vcc->qos.txtp.scr;
		break;
	case ATM_UBR_PLUS:
		port->tx_used_cell_rate -= vcc->qos.txtp.min_pcr;
		break;
	}

	/* idle for a while to let parallel operation finish */
	udelay(100);

	return;
}

static int ppe_ioctl(struct atm_dev *dev, unsigned int cmd, void *arg)
{
	int ret = 0;
	/* int conn; */

	if (_IOC_TYPE(cmd) != PPE_ATM_IOC_MAGIC
		|| _IOC_NR(cmd) >= PPE_ATM_IOC_MAXNR)
		return -ENOTTY;

	if (_IOC_DIR(cmd) & _IOC_READ)
		ret = !access_ok(VERIFY_WRITE, arg, _IOC_SIZE(cmd));
	else if (_IOC_DIR(cmd) & _IOC_WRITE)
		ret = !access_ok(VERIFY_READ, arg, _IOC_SIZE(cmd));
	if (ret)
		return -EFAULT;

	switch (cmd) {
#if 0	/* TODO: to be added */
	case PPE_ATM_MAP_PKT_PRIO_TO_Q:
	break;
	case PPE_ATM_TX_Q_OP:
	break;
	case PPE_ATM_GET_MAP_PKT_PRIO_TO_Q:
	break;
	default:
	ret = -ENOIOCTLCMD;
#endif

	}

	return ret;
}

static int ppe_send(struct atm_vcc *vcc, struct sk_buff *skb)
{
	int ret;
	int conn, qid, mpoa_pt, mpoa_type, vid;
	unsigned int prio;
	dp_subif_t dp_id;
	struct vrx318_atm_tc *priv = vcc->dev->dev_data;

	if (!skb || !vcc || vcc->qos.aal != ATM_AAL5 ||
		skb->len > priv->frwk->param.aal5s_max_pktsz)
		goto PARAM_ERR;

	ATM_SKB(skb)->vcc = vcc;

	if (priv->frwk->tc_stat != VRX318_RUNNING
		|| priv->frwk->tc_mode[priv->ep_id] != TC_ATM_SL_MODE) {
		vrx318_dbg(DBG_ERR, "Drop packet due to tc_stat: %d, tc_mode: %d, ep_id: %d\n",
			(u32)priv->frwk->tc_stat,
			(u32)priv->frwk->tc_mode[priv->ep_id],
			priv->ep_id);
		atm_free_tx_skb_vcc(skb);
		return -EACCES;
	}

	skb_get(skb);
	atm_free_tx_skb_vcc(skb);
	ATM_SKB(skb)->vcc = NULL;

	conn = find_vcc(vcc);
	if (conn < 0) {
		vrx318_dbg(DBG_ERR, "Cannot find vcc: vpi/vci: %d/%d\n",
			vcc->vpi, vcc->vci);
		ret = -EINVAL;
		goto FIND_VCC_FAIL;
	}

	if (!atm_in_showtime()) {
		vrx318_dbg(DBG_ERR, "DSL not in showtime\n");
		ret = -EIO;
		goto CHECK_SHOWTIME_FAIL;
	}

	dump_atm_data(skb->data, skb->len, 0);

	prio = skb->priority >= ATM_PRIO_Q_NUM ?
			ATM_PRIO_Q_NUM - 1 : skb->priority;

	dp_id.port_id = priv->frwk->port_id;
	mpoa_pt = 1;
	mpoa_type = 0; /* Ignore by FW if mpoa_pt set */
	qid = priv->conn[conn].prio_queue_map[prio];
	vid = priv->conn[conn].subif_id;
	dp_id.subif = vid | ATM_DESC_SUBIF_ID(qid, mpoa_pt, mpoa_type);

	/* SET EP */
	skb->DW1 = (skb->DW1 & (~0xF00)) | ((dp_id.port_id & 0xF) << 8);
	/* SET SUBIFID */
	skb->DW0 = (skb->DW0 & ~0x7FFF) | dp_id.subif;
	vrx318_dbg(DBG_PKT_TX, "port_id: 0x%x, subif_id: 0x%x\n",
		dp_id.port_id, dp_id.subif);

	if (dp_xmit(skb->dev, &dp_id, skb, skb->len, 0) == 0) {
		priv->stats.aal5_tx_pkts++;
		priv->stats.aal5_tx_bytes += skb->len;
		priv->conn[conn].stats.aal5_tx_pkts++;
		priv->conn[conn].stats.aal5_tx_bytes += skb->len;
		priv->conn[conn].prio_tx_packets[prio]++;
	} else {
		vrx318_dbg(DBG_ERR, "dp xmit fail\n");
		priv->stats.aal5_tx_dropped++;
		priv->conn[conn].stats.aal5_tx_dropped++;
	}

	if (vcc->stats)
		atomic_inc(&vcc->stats->tx);

	return 0;

PARAM_ERR:
	if (!skb) {
		vrx318_dbg(DBG_ERR, "SKB is NULL\n");
		return -EINVAL;
	} else if (!vcc) {
		vrx318_dbg(DBG_ERR, "VCC is NULL\n");
		return -EINVAL;
	} else if (vcc->qos.aal != ATM_AAL5) {
		vrx318_dbg(DBG_ERR, "AAL protocol is not ATM_ALL5\n");
		return -EPROTONOSUPPORT;
	} else if (skb->len > priv->frwk->param.aal5s_max_pktsz) {
		vrx318_dbg(DBG_ERR, "Skb size too big: %d\n", skb->len);
		return -EOVERFLOW;
	}

	return -EINVAL;

FIND_VCC_FAIL:
	if (!vcc) {
		atm_free_tx_skb_vcc(skb);
		return ret;
	} else if (vcc->stats)
		atomic_inc(&vcc->stats->tx_err);

	if (vcc->qos.aal == ATM_AAL5)
		priv->stats.aal5_tx_errors++;
	atm_free_tx_skb_vcc(skb);
	return ret;

CHECK_SHOWTIME_FAIL:
	if (vcc->stats)
		atomic_inc(&vcc->stats->tx_err);
	if (vcc->qos.aal == ATM_AAL5) {
		priv->stats.aal5_tx_dropped++;
		priv->conn[conn].stats.aal5_tx_dropped++;
	}
	atm_free_tx_skb_vcc(skb);
	return ret;
}

static int ppe_send_oam(struct atm_vcc *vcc, void *cell, int flags)
{
	int conn, qid, mpoa_pt, mpoa_type, vid;
	struct uni_cell_header *cell_header = (struct uni_cell_header *)cell;
	struct sk_buff *skb;
	dp_subif_t dp_id;
	struct vrx318_atm_tc *priv = g_atm_tc;

	if (!vcc)
		return -EINVAL;

	if (!atm_in_showtime()) {
		vrx318_dbg(DBG_ERR, "ATM not in showtime\n");
		return -EIO;
	}

	dump_oam_cell(cell, 0);
	if (((cell_header->pti == ATM_PTI_SEGF5 ||
		cell_header->pti == ATM_PTI_E2EF5)
		&& find_vpivci(cell_header->vpi, cell_header->vci) < 0)
		|| ((cell_header->vci == 0x03 || cell_header->vci == 0x04)
		&& find_vpi(cell_header->vpi) < 0)) {

		vrx318_dbg(DBG_ERR, "Cell pti/vpi/vci not match: %d/%d/%d\n",
			cell_header->pti, cell_header->vpi, cell_header->vci);
		return -EINVAL;
	}

	/*  find queue ID   */
	conn = find_vcc(vcc);
	if (conn < 0) {
		vrx318_dbg(DBG_ERR, "VCC is not configured: VPI/VCI: %d/%d\n",
			vcc->vpi, vcc->vci);
		return -EINVAL;
	}

	dp_id.port_id = priv->frwk->port_id;
	mpoa_pt = 1;
	mpoa_type = 0; /* Ignore by FW  */
	qid = priv->conn[conn].prio_queue_map[priv->frwk->param.oam_prio];
	vid = priv->conn[conn].subif_id;
	dp_id.subif = vid | ATM_DESC_SUBIF_ID(qid, mpoa_pt, mpoa_type);

	/*  allocate sk_buff    */
	skb = cbm_alloc_skb(DMA_PACKET_SIZE, GFP_ATOMIC);
	if (skb == NULL) {
		vrx318_dbg(DBG_ERR, "%s: allocate cbm buffer fail\n", __func__);
		return -ENOMEM;
	}

	/*  copy data   */
	skb_put(skb, CELL_SIZE);
	memcpy(skb->data, cell, CELL_SIZE);
	/* SET EP */
	skb->DW1 = (skb->DW1 & (~0xF00)) | ((dp_id.port_id & 0xF) << 8);
	/* SET SUBIFID */
	skb->DW0 = (skb->DW0 & ~0x7FFF) | dp_id.subif;
	skb->dev = priv->conn[conn].dev;

	vrx318_dbg(DBG_OAM_TX, "conn: %d, dev name: %s, dp port: %d, dp subif: 0x%x, len:%d\n",
		conn, skb->dev->name, dp_id.port_id, dp_id.subif, skb->len);

	if (dp_xmit(skb->dev, &dp_id, skb, skb->len, DP_TX_OAM) == 0) {
		priv->stats.oam_tx_pkts++;
		priv->stats.oam_tx_bytes += skb->len;
		priv->conn[conn].stats.oam_tx_pkts++;
		priv->conn[conn].stats.oam_tx_bytes += skb->len;
	} else {
		priv->stats.oam_tx_dropped++;
		priv->conn[conn].stats.oam_tx_dropped++;
	}

	return 0;
}

static int ppe_change_qos(struct atm_vcc *vcc, struct atm_qos *qos, int flags)
{
	int conn;

	if (vcc == NULL || qos == NULL)
		return -EINVAL;

	conn = find_vcc(vcc);
	if (conn < 0)
		return -EINVAL;

	set_qsb((struct vrx318_atm_tc *)vcc->dev->dev_data, vcc, qos, conn);

	return 0;
}

ssize_t proc_write_atm_wanmib(struct file *file, const char __user *buf,
				size_t count, loff_t *data)
{
	struct vrx318_atm_tc *priv;
	int ep_id;
	u32 base;
	char str[32];
	int len, rlen, i;
	int num;
	char *param_list[20];

	priv = (struct vrx318_atm_tc *)PDE_DATA(file_inode(file));
	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;

	len = count < sizeof(str) ? count : sizeof(str) - 1;
	rlen = len - copy_from_user(str, buf, len);
	str[rlen] = 0;
	num = dp_split_buffer(str, param_list, ARRAY_SIZE(param_list));
	if ((dp_strcmpi(param_list[0], "clear") == 0)
		|| (dp_strcmpi(param_list[0], "clean") == 0)
		|| (dp_strcmpi(param_list[0], "c") == 0)) {
		for (i = 0; i < sizeof(struct dsl_wan_mib_table) / 4; i++)
			vrx318_w32_sb(0, DSL_WAN_MIB_TBL + i, base);

		for (i = 0; i < QOSQ_NUM; i++) {
			dword_ep_clear(SB_BUFFER(DSL_Q_RX_MIB_TBL(i)),
				sizeof(struct dsl_queue_mib), base);
			dword_ep_clear(SB_BUFFER(DSL_Q_TX_MIB_TBL(i)),
				sizeof(struct dsl_queue_mib), base);
		}
		spin_lock_bh(&priv->atm_lock);
		for (i = 0; i < pvc_num(); i++) {
			if (priv->pvc_tbl & BIT(i)) {
				memset(&priv->conn[i].stats, 0,
					sizeof(priv->conn[i].stats));
				memset(&priv->conn[i].prio_tx_packets, 0,
					sizeof(priv->conn[i].prio_tx_packets));
			}
			memset(&priv->stats, 0, sizeof(priv->stats));
		}
		spin_unlock_bh(&priv->atm_lock);
	} else {
		goto proc_atm_wanmib_help;
	}

	return count;

proc_atm_wanmib_help:
	pr_info("echo clear/clean/c > wanmib\n");
return count;

}

int proc_read_atm_wanmib(struct seq_file *seq, void *v)
{
	struct vrx318_atm_tc *priv = (struct vrx318_atm_tc *)seq->private;
	int i, ep_id;
	u32 base;

	if (!priv) {
		pr_err("priv pointer is NULL!!!\n");
		return -EINVAL;
	}

	ep_id = priv->ep_id;
	base = (u32)priv->frwk->ep_dev[ep_id].membase;

	pr_info("ep_id: %d, base: 0x%x\n", ep_id, base);

	seq_puts(seq, "DSL WAN MIB:\n");
	seq_printf(seq, "  wrx_drophtu_cell: %u\n", vrx318_r32_sb(__WRX_DROPHTU_CELL, base));
	seq_printf(seq, "  wrx_dropdes_pdu:  %u\n", vrx318_r32_sb(__WRX_DROPDES_PDU, base));
	seq_printf(seq, "  wrx_correct_pdu:  %u\n", vrx318_r32_sb(__WRX_CORRECT_PDU, base));
	seq_printf(seq, "  wrx_err_pdu:      %u\n", vrx318_r32_sb(__WRX_ERR_PDU, base));
	seq_printf(seq, "  wrx_dropdes_cell: %u\n", vrx318_r32_sb(__WRX_DROPDES_CELL, base));
	seq_printf(seq, "  wrx_correct_cell: %u\n", vrx318_r32_sb(__WRX_CORRECT_CELL, base));
	seq_printf(seq, "  wrx_err_cell:     %u\n", vrx318_r32_sb(__WRX_ERR_CELL, base));
	seq_printf(seq, "  wrx_total_byte:   %u\n", vrx318_r32_sb(__WRX_TOTAL_BYTE, base));
	seq_printf(seq, "  wtx_total_pdu:    %u\n", vrx318_r32_sb(__WTX_TOTAL_PDU, base));
	seq_printf(seq, "  wtx_total_cell:   %u\n", vrx318_r32_sb(__WTX_TOTAL_CELL, base));
	seq_printf(seq, "  wtx_total_byte:   %u\n", vrx318_r32_sb(__WTX_TOTAL_BYTE, base));
	seq_puts(seq, "DSL RX QUEUE MIB:\n");
	seq_puts(seq, "  idx     pdu       bytes\n");
	for (i = 0; i < txq_num(); i++)
		seq_printf(seq, "   %2d %10u %10u\n", i,
			vrx318_r32_sb(DSL_Q_RX_MIB_TBL(i), base),
			vrx318_r32_sb(DSL_Q_RX_MIB_TBL(i) + 1, base));

	seq_puts(seq, "DSL TX QUEUE MIB:\n");
	seq_puts(seq, "  idx     pdu       bytes\n");

	for (i = 0; i < txq_num(); i++)
		seq_printf(seq, "   %2d %10u %10u\n", i,
			vrx318_r32_sb(DSL_Q_TX_MIB_TBL(i), base),
			vrx318_r32_sb(DSL_Q_TX_MIB_TBL(i) + 1, base));

	print_drv_mib(seq, priv);

	return 0;
}
static int print_htu(struct seq_file *seq, int i, u32 base)
{
	struct htu_entry entry;
	struct htu_mask mask;
	struct htu_result result;

	dword_mem_read(&entry, SB_BUFFER(HTU_ENTRY_TBL(i)),
			sizeof(entry), base);
	dword_mem_read(&mask, SB_BUFFER(HTU_MASK_TBL(i)),
			sizeof(mask), base);
	dword_mem_read(&result, SB_BUFFER(HTU_RESULT_TBL(i)),
			sizeof(result), base);

	if (entry.vld) {
		seq_printf(seq, "%2d. valid\n", i);
		seq_printf(seq,  "    entry  0x%08x - pid %01x, vpi %02x, vci %04x, pti %01x\n",
			*(u32 *)&entry, entry.pid, entry.vpi,
			entry.vci, entry.pti);
		seq_printf(seq,  "    mask   0x%08x - pid %01x, vpi %02x, vci %04x, pti %01x\n",
			*(u32 *)&mask, mask.pid_mask, mask.vpi_mask,
			mask.vci_mask, mask.pti_mask);
		seq_printf(seq,  "    result 0x%08x - type: %s, qid: %d",
			*(u32 *)&result, result.type ? "cell" : "AAL5",
			result.qid);
		if (result.type)
			seq_printf(seq,  ", cell id: %d, verification: %s",
				result.cellid, result.ven ? "on" : "off");
		seq_puts(seq,  "\n");
	} else
		seq_printf(seq, "%2d. invalid\n", i);

	return 0;
}

int proc_read_htu(struct seq_file *seq, void *v)
{
	int htuts;
	int i;
	u32 base;
	struct vrx318_atm_tc *priv = (struct vrx318_atm_tc *)seq->private;

	base = (u32)priv->frwk->ep_dev[priv->ep_id].membase;

	htuts = vrx318_r32_sb(__CFG_WRX_HTUTS, base);

	for (i = 0; i < htuts; i++)
		print_htu(seq, i, base);

	return 0;
}
int proc_read_queue(struct seq_file *seq, void *v)
{
	static const char const *mpoa_type_str[]
		= {"EoA w/o FCS", "EoA w FCS", "PPPoA", "IPoA"};

	wrx_queue_config_t rx;
	wtx_queue_config_t tx;
	char qmap_str[64];
	char qmap_flag;
	int qmap_str_len;
	int i, k;
	unsigned int bit;

	u32 base;
	struct vrx318_atm_tc *priv = (struct vrx318_atm_tc *)seq->private;

	base = (u32)priv->frwk->ep_dev[priv->ep_id].membase;

	seq_printf(seq, "RX Queue Config (0x%08X):\n", WRX_Q_CFG(0));

	for (i = 0; i < txq_num(); i++) {
		dword_mem_read(&rx, SB_BUFFER(WRX_Q_CFG(i)),
				sizeof(rx), base);
		seq_printf(seq, "  %d: MPoA type - %s, MPoA mode - %s, IP version %d\n",
			i, mpoa_type_str[rx.mpoa_type],
			rx.mpoa_mode ? "LLC" : "VC mux",
			rx.ip_ver ? 6 : 4);
		seq_printf(seq, "     Oversize - %d, Undersize - %d, Max Frame size - %d\n",
			rx.oversize, rx.undersize, rx.mfs);
		seq_printf(seq, "     uu mask - 0x%02X, cpi mask - 0x%02X, uu exp - 0x%02X, cpi exp - 0x%02X\n",
			rx.uumask, rx.cpimask, rx.uuexp, rx.cpiexp);
		if (rx.vlan_ins)
			seq_printf(seq, "     new_vlan = 0x%08X\n",
				rx.new_vlan);
	}

	seq_printf(seq, "TX Queue Config (0x%08X):\n", WTX_Q_CFG(0));

	for (i = 0; i < txq_num(); i++) {
		dword_mem_read(&tx, SB_BUFFER(WTX_Q_CFG(i)),
				sizeof(tx), base);
		qmap_flag = 0;
		qmap_str_len = 0;
		for (k = 0, bit = 1; k < 15; k++, bit <<= 1)
			if (tx.same_vc_qmap & bit) {
				if (qmap_flag++)
					qmap_str_len += sprintf(qmap_str + qmap_str_len, ", ");
				qmap_str_len += sprintf(qmap_str + qmap_str_len, "%d", k);
			}
		seq_printf(seq, "  %d: uu - 0x%02X, cpi - 0x%02X, same VC queue map - %s\n",
			i, tx.uu, tx.cpi, qmap_flag ? qmap_str : "null");
		seq_printf(seq, "     bearer channel - %d, QSB ID - %d, MPoA mode - %s\n",
			tx.sbid, tx.qsb_vcid, tx.mpoa_mode ? "LLC" : "VC mux");
		seq_printf(seq, "     ATM header - 0x%08X\n", tx.atm_header);
	}

	return 0;
}
int sw_tx_queue_del(struct vrx318_atm_tc *priv, int conn)
{
	int i;
	int qid, txq_id;
	wtx_queue_config_t wtx_q_cfg;
	u32 new_q_tbl, q_tbl;
	struct atm_pvc *connect;
	u32 base = (u32)priv->frwk->ep_dev[priv->ep_id].membase;

	spin_lock_bh(&priv->atm_lock);
	connect = &priv->conn[conn];
	q_tbl = connect->sw_txq_tbl;

	qid = txq_id = -1;
	for (i = (pvc_num() - 1); i >= 0; i--) {
		if (q_tbl & BIT(i)) {
			if (qid == -1)
				qid = i;
			else {
				txq_id = i;
				break;
			}
		}
	}

	/* Last Queue or something wrong */
	if (qid < 0 || q_tbl == BIT(qid)
		|| ((q_tbl & BIT(qid)) == 0) || txq_id < 0) {
		spin_unlock_bh(&priv->atm_lock);
		return -EIO;
	}

	/* Update queue map table */
	new_q_tbl = connect->sw_txq_tbl & (~(BIT(qid)));

	connect->sw_txq_tbl = new_q_tbl;
	priv->sw_txq_tbl &= ~(BIT(qid));

	for (i = 0; i < ATM_PRIO_Q_NUM; i++) {
		if (connect->prio_queue_map[i] == qid)
			connect->prio_queue_map[i] = txq_id;
	}
	spin_unlock_bh(&priv->atm_lock);

	for (i = 0; i < qid; i++) {
		dword_mem_read(&wtx_q_cfg, SB_BUFFER(WTX_Q_CFG(i)),
			sizeof(wtx_q_cfg), base);
		wtx_q_cfg.same_vc_qmap &= ~(BIT(qid));
		dword_mem_write(SB_BUFFER(WTX_Q_CFG(i)),
			&wtx_q_cfg, sizeof(wtx_q_cfg), base);
	}
	dword_ep_clear(SB_BUFFER(WTX_Q_CFG(qid)), sizeof(wtx_q_cfg), base);

	return 0;
}

int sw_tx_queue_add(struct vrx318_atm_tc *priv, int conn)
{
	int i;
	int qid;
	wtx_queue_config_t wtx_q_cfg;
	u32 new_q_tbl, q_tbl;
	struct atm_pvc *connect;
	u32 base = (u32)priv->frwk->ep_dev[priv->ep_id].membase;

	/* Get available Phyiscal Queue ID */
	spin_lock_bh(&priv->atm_lock);
	for (qid = 0; qid < pvc_num(); qid++) {
		if ((priv->sw_txq_tbl & BIT(qid)) == 0)
			break;
	}

	if (qid >= pvc_num()) {
		vrx318_dbg(DBG_ERR, "No available physical queue\n");
		spin_unlock_bh(&priv->atm_lock);
		return -ENOSPC;
	}

	connect = &priv->conn[conn];
	/* Reconfigure Existing Queue's QMAP */
	q_tbl = connect->sw_txq_tbl;
	new_q_tbl = q_tbl | BIT(qid);

	/* Update local and global tx queue table */
	connect->sw_txq_tbl = new_q_tbl;
	priv->sw_txq_tbl |= BIT(qid);
	spin_unlock_bh(&priv->atm_lock);

	for (i = 0; i < pvc_num(); i++) {
		if (q_tbl & BIT(i)) {
			dword_mem_read(&wtx_q_cfg, SB_BUFFER(WTX_Q_CFG(i)),
				sizeof(wtx_q_cfg), base);
			wtx_q_cfg.same_vc_qmap = new_q_tbl & (~(BIT(i)));
			dword_mem_write(SB_BUFFER(WTX_Q_CFG(i)),
				&wtx_q_cfg, sizeof(wtx_q_cfg), base);
		}
	}
	/* Add new Queue Setting */
	wtx_q_cfg.same_vc_qmap = new_q_tbl & (~(BIT(qid)));
	dword_mem_write(SB_BUFFER(WTX_Q_CFG(qid)),
		&wtx_q_cfg, sizeof(wtx_q_cfg), base);

	return 0;
}
