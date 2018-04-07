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
#include <linux/seq_file.h>
#include <linux/printk.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <net/datapath_api.h>
#include <net/lantiq_cbm.h>
#include <net/lantiq_cbm_api.h>
#include <net/vrx318_dsl_api.h>

#include <lantiq.h>
#include <lantiq_soc.h>
#include <lantiq_irq.h>
#include <lantiq_pcie.h>

#include "include/vrx318_dfe.h"
#include "include/vrx318_ppe_fw_ds.h"

#include "include/vrx318_common.h"
#include "include/vrx318_framework.h"
#include "include/vrx318_api.h"

void reset_ppe(u32 base)
{
	int max_loop = 10000;
	u32 reset_mask
		= RST_PPE_ATM_TC | RST_DMA_CORE | RST_PPE_DSL_IF | RST_PPE_DFE;
	u32 ppe_rst;

	vrx318_w32(reset_mask, RST_REQ, base);
	udelay(1000);

	while (max_loop-- > 0 && vrx318_r32(RST_REQ, base) & reset_mask)
		;

	if ((vrx318_r32(RST_REQ, base) & reset_mask)) {
		vrx318_dbg(DBG_ERR, "failed in reset PPE\n");
		BUG();
	}

	/* Software Reset PPM1/PPM0/QSB/PDMA/SSL1/SAR/SSL0/DREG/RTHA/TTHA */
	ppe_rst = vrx318_r32(PP32_SRST, base);
	ppe_rst &= ~0x303CF;
	vrx318_w32(ppe_rst, PP32_SRST, base);
	udelay(1000);

	/* Set back to working status */
	ppe_rst = vrx318_r32(PP32_SRST, base);
	ppe_rst |= 0x303CF;
	vrx318_w32(ppe_rst, PP32_SRST, base);
	udelay(1000);
}

int pp32_download_code(u32 base, int pp32, const u32 *code_src,
			unsigned int code_dword_len, const u32 *data_src,
			unsigned int data_dword_len)
{
	unsigned int clr, set, cdm_cfg;

	if (code_src == 0 || ((unsigned long)code_src & 0x03) != 0
		|| data_src == 0 || ((unsigned long)data_src & 0x03) != 0) {
		vrx318_dbg(DBG_ERR, "FW code src or data src check fail\n");
		return -ENOEXEC;
	}

	vrx318_dbg(DBG_INIT, "pp32: %d, code_src: 0x%x, code dw len: 0x%x\n",
		pp32, (unsigned int)code_src, code_dword_len);

	clr = pp32 ? 0xF0 : 0x0F;
	if (code_dword_len <= CDM_CODE_MEMORYN_DWLEN(0))
		set = pp32 ? (3 << 6) : (2 << 2);
	else
		set = 0x00;
	cdm_cfg = vrx318_r32(CDM_CFG, base);
	cdm_cfg = (cdm_cfg & ~clr) | set;
	vrx318_w32(cdm_cfg, CDM_CFG, base);
	vrx318_dbg(DBG_INIT, "addr(CDM_CFG) = 0x%08x\n", cdm_cfg);

	/*  clear code memory   */
	dword_ep_clear(CDM_CODE_MEMORY(pp32, 0),
		CDM_CODE_MEMORYN_DWLEN(0) << 2, base);

	if (code_dword_len > CDM_CODE_MEMORYN_DWLEN(0))
		dword_ep_clear((CDM_CODE_MEMORY(pp32, 0) +
			(CDM_CODE_MEMORYN_DWLEN(0) << 2)),
			CDM_CODE_MEMORYN_DWLEN(1) << 2, base);

	/*  copy code   */
	dword_mem_write(CDM_CODE_MEMORY(pp32, 0), code_src,
			code_dword_len << 2, base);

	/*  copy data   */
	dword_mem_write(CDM_DATA_MEMORY(pp32, 0), data_src,
			data_dword_len << 2, base);

	return 0;
}

void enable_ppe_ema(u32 base)
{
	u32 val;
	int cnt;

	/* some version of chip by default power on */
	if ((vrx318_r32(PMU_SR, base) & PMU_PPE_EMA) == 0)
		return;

	val = vrx318_r32(PMU_PWDCR, base);
	val &= ~PMU_PPE_EMA; /* set 0 to power on */
	vrx318_w32(val, PMU_PWDCR, base);

	for (cnt = 0; cnt < 1000; cnt++) {
		if ((vrx318_r32(PMU_SR, base) & PMU_PPE_EMA) == 0)
			return;
	}
	vrx318_dbg(DBG_INIT, "Power on PPE EMA failed\n");
	BUG();
}

void set_ppe_clk(u32 clk, u32 base)
{
	u32 clk_val;

	clk_val = vrx318_r32(CGU_CLKFSR, base);

	switch (clk) {
	case 288:
		vrx318_w32(SET_PPE_CLK(clk_val, 2), CGU_CLKFSR, base);
		break;
	case 432:
		vrx318_w32(SET_PPE_CLK(clk_val, 1), CGU_CLKFSR, base);
		break;
	default:
		vrx318_dbg(DBG_ERR, "Set PPE CLK err:%d\n", clk);
		break;
	}
}

u32 get_ppe_clk(u32 base)
{
	u32 clk_val;

	clk_val = (vrx318_r32(CGU_CLKFSR, base) >> 16) & 0x7;
	switch (clk_val) {
	case 1:
		return 432000000;
	case 2:
		return 288000000;
	default:
		vrx318_dbg(DBG_ERR, "PPE CLK error: %d\n", clk_val);
	}
	/* Default */
	return 288000000;
}

void print_fw_version(char *dstbuf, struct fw_ver_id *ver)
{
	char buf[128];
	int len;

	static char *fw_ver_package_str[] = {
		"Reserved - 0",
		"A1",
		"B1 - (PTM_BONDING)",
		"E1",
		"A5",
		"D5",
		"D5v2",
		"E5",
	};

	len = 0;

	if (ver->family == 9)
		len += sprintf(buf + len, "FW Family:\t VRX318\n");
	else
		len += sprintf(buf + len, "FW Family:\t Unknown: %d\n",
			ver->family);

	if (ver->package < ARRAY_SIZE(fw_ver_package_str))
		len += sprintf(buf + len,
			"FW Package:\t %s Package\n",
			fw_ver_package_str[ver->package]);
	else
		len += sprintf(buf + len,
			"FW Package:\t Unknow Package: %d\n", ver->package);

	len += sprintf(buf + len, "FW Version:\t %d.%d.%d\n",
			ver->major, ver->middle, ver->minor);
	buf[len] = 0;

	if (dstbuf)
		strcpy(dstbuf, buf);
	else
		vrx318_dbg(DBG_INIT, "%s", buf);
}

void dword_clear(void *addr, int byte_len)
{
	int i;
	u32 *dst = (u32 *)addr;
	int dw_len;

	dw_len = byte_len / 4;

	if (dw_len <= 0)
		return;

	if ((u32)dst & 0x3) {
		vrx318_dbg(DBG_ERR, "%s: address is not aligned: 0x%x\n",
			__func__, (u32)addr);
	}

	for (i = 0; i < dw_len; i++)
		dst[i] = 0;
}

void dword_ep_clear(u32 addr, int byte_len, u32 base)
{
	int i;
	int dw_len;

	dw_len = byte_len / 4;

	if (dw_len <= 0)
		return;

	for (i = 0; i < dw_len; i++)
		vrx318_w32(0, addr + (i << 2), base);

	return;
}

void dword_mem_write(u32 dst_addr, const void *src_addr, int byte_len, u32 base)
{
	int i;
	const u32 *src = (const u32 *)src_addr;
	int dw_len;

	dw_len = byte_len / 4;

	if (dw_len <= 0)
		return;

	if ((dst_addr & 0x3) || ((u32)src & 0x3)) {
		vrx318_dbg(DBG_ERR, "%s: address is not dw aligned: dst: 0x%x, src: 0x%x\n",
			__func__, dst_addr, (u32)src);
	}

	for (i = 0; i < dw_len; i++)
		vrx318_w32(src[i], dst_addr + (i << 2), base);
}

void dword_mem_read(void *dst_addr, u32 src_addr, int byte_len, u32 base)
{
	int i;
	u32 *dst = (u32 *)dst_addr;
	int dw_len;

	dw_len = byte_len / 4;

	if (dw_len <= 0)
		return;

	if (((u32)dst & 0x3) || (src_addr & 0x3)) {
		vrx318_dbg(DBG_ERR, "%s: address is not dw aligned: dst: 0x%x, src: 0x%x\n",
			__func__, (u32)dst, src_addr);
	}

	for (i = 0; i < dw_len; i++)
		dst[i] = vrx318_r32(src_addr + (i << 2), base);
}

void vrx318_sb_clear(u32 base)
{
	dword_ep_clear(SB_BUFFER(0x2000), 0x5000 * 4, base);
	dword_ep_clear(SB_BUFFER(0x8000), 0x8000 * 4, base);
}

int alloc_data_buffer(struct vrx318_priv *priv,
				unsigned int size, dma_addr_t *dma_addr,
				unsigned int *v_addr)
{
	dma_addr_t phy_addr;
	unsigned int *buf;

	buf = (unsigned int *)cbm_buffer_alloc(0, CBM_PORT_F_STANDARD_BUF);
	if (!buf)
		return -ENOMEM;

	phy_addr = dma_map_single(priv->pdev, buf, size, DMA_FROM_DEVICE);
	if (dma_mapping_error(priv->pdev, phy_addr)) {
		vrx318_dbg(DBG_ERR, "DMA address mapping error: buf: 0x%x, size: %d, dir: %d\n",
			(u32)buf, size, DMA_FROM_DEVICE);
		BUG();
	}
	dma_unmap_single(priv->pdev, phy_addr, size, DMA_FROM_DEVICE);
	*dma_addr = phy_addr;
	if (v_addr)
		*v_addr = (unsigned int)buf;

	return 0;
}

int free_data_buffer(dma_addr_t phy_addr)
{
	return cbm_buffer_free(0, phy_addr, 1);
}

static u32 mei_rd(u32 addr, u32 base)
{
	vrx318_w32(addr, MEIAD(ME_DBG_RD_AD), base);
	while (!(vrx318_r32(MEIAD(ME_ARC2ME_STAT), base) & 0x10))
		;
	vrx318_w32(0x10, MEIAD(ME_ARC2ME_STAT), base);

	return vrx318_r32(MEIAD(ME_DBG_DATA), base);
}

static void mei_wr(u32 addr, u32 val, u32 base)
{
	vrx318_w32(addr, MEIAD(ME_DBG_WR_AD), base);
	vrx318_w32(val, MEIAD(ME_DBG_DATA), base);
	while (!(vrx318_r32(MEIAD(ME_ARC2ME_STAT), base) & 0x10))
		;
	vrx318_w32(0x10, MEIAD(ME_ARC2ME_STAT), base);
}

static void setup_zephyr(u32 base)
{

	u32 i, read_data;

	for (i = 0; i < NUM_ME_VAR; i++) {
		switch (my_cfg_seq[i].type) {
		case ME_DBG_RD:
			read_data = vrx318_r32(my_cfg_seq[i].addr +
					0x1e116000, base);
			break;
		case ME_DBG_WR:
			vrx318_w32(my_cfg_seq[i].data, my_cfg_seq[i].addr +
					0x1e116000, base);
			break;
		}
	}

	/* Read back to make sure Zephry started */
	read_data = mei_rd(0x00020c40c, base);
	vrx318_dbg(DBG_LOOPBACK, "CRI_TSC_CTRL: 0x%08x\n", read_data);

	read_data = mei_rd(0x00020c4dc, base);
	vrx318_dbg(DBG_LOOPBACK, "CRI_RXFFT_STALL_CTRL: 0x%08x\n", read_data);

	read_data = mei_rd(0x00020c4d8, base);
	vrx318_dbg(DBG_LOOPBACK, "CRI_RXQT_STALL_CTRL: 0x%08x\n", read_data);

	read_data = mei_rd(0x00020c4d0, base);
	vrx318_dbg(DBG_LOOPBACK, "CRI_RXPMS_CTRL: 0x%08x\n", read_data);

	read_data = mei_rd(0x00020c4b4, base);
	vrx318_dbg(DBG_LOOPBACK, "CRI_TXPMS_CTRL: 0x%08x\n", read_data);

	return;
}


static inline void vrx218_dfe_reset(u32 base)
{
	vrx318_w32(((0x1 << 7) | (0x1 << 3)), RST_STAT, base);
	mdelay(1);
}

static void dfe_zephyr_lb_init(u32 base, u32 arc_mode)
{
	vrx218_dfe_reset(base);

	if (arc_mode)
		/* start_arc(base); */
		vrx318_dbg(DBG_ERR, "ARC loopback mode is not supported\n");
	else
		setup_zephyr(base);

	return;
}

static void set_dfe_data_rate(u32 base, u32 nbc_switches, u32 nbc0bytes,
				u32 nbc1bytes, u32 numtimeslots)
{

	/* Num of BC switches for Tx to load into register ZT_R0 */
	mei_wr(0x000542F4, nbc_switches, base);

	/* Num of BC switches for Rx to load into register ZR_R0 */
	mei_wr(0x0005B94C, nbc_switches, base);

	/* Num of BC0 and BC1 bytes for Tx to load into register ZT_VBC_SIZE */
	mei_wr(0x00054308, (nbc1bytes << 16) + nbc0bytes, base);

	/* Num of BC0 and BC1 bytes for Rx to load into register ZR_VBC_SIZE */
	mei_wr(0x0005B960, (nbc1bytes << 16) + nbc0bytes, base);

	/* Num of BC0 and BC1 error bytes for Tx to load into register ZT_R12 */
	/* me_dbg_wr(0x00054300, (nBC1ErrBytes << 16) + nBC0ErrBytes); */
	mei_wr(0x00054300, 0, base);

	/* Num of BC0 and BC1 error bytes for Rx to load into REG ZR_R12 */
	/* me_dbg_wr(0x0005B958, (nBC1ErrBytes << 16) + nBC0ErrBytes); */
	mei_wr(0x0005B958, 0, base);

	/* kick of by writing to CRI registers */
	/* me_dbg_wr(0x0020c40c, 0x8007ffe1); */
	mei_wr(0x0020c40c, 0x8007ffe0 | numtimeslots, base);
	mei_wr(0x0020c49c, 0x00000078, base);

	return;
}

void setup_dfe_loopback(struct vrx318_priv *priv, u32 base)
{
	if (!priv->param.dfe_loopback)
		return;

	vrx318_dbg(DBG_INIT, "enable DFE loopback ...\n");

	/* Set DFE to Zephyr loopback mode */
	/* 0 - ARC core disable ; 1 - ARC core enable */
	dfe_zephyr_lb_init(base, 0);

	/* Function to change DFE data rate
	 * Bit_rate (MBps) = num_bc_switch *
	 *	(bc0_payld + bc1_payld)  / ((num_time_slot + 1) * 28 us);
	 * Bit_rate (Mbps) = 8 * num_bc_switch *
	 *	(bc0_payld + bc1_payld)  / ((num_time_slot + 1) * 28 us);
	 * set_dfe_data_rate(u8 pcie_port, UINT32 num_bc_switch,
	 *	UINT32 bc0_payld, UINT32 bc1_payld, UINT32 num_time_slot)
	 */
	set_dfe_data_rate(base, 4, 427, 427, 1);
}

static inline int in_sync(u32 base, u32 bc)
{
	int state = 0;

	if (bc == 0)
		state = vrx318_r32(PPE_SFSM_STATE0, base) & 0x1;
	else if (bc == 1)
		state = vrx318_r32(PPE_SFSM_STATE1, base) & 0x1;

	return state;
}

void dfe_loopback_linkup(struct vrx318_priv *priv, u32 base)
{
	u32 val;
	int i = 0;
	if (!priv->param.dfe_loopback)
		return;

	vrx318_dbg(DBG_LOOPBACK, "wait dfe_loopback_linkup\n");
	vrx318_w32(0x07, BC0_LINK_STATUS, base);
	udelay(15);

	while (!in_sync(base, 0) && !in_sync(base, 1) && (i++) < 10000)
		;

	vrx318_dbg(DBG_INIT, "%s\n", in_sync(base, 0) ?
		"BC0 is in sync\n" : "BC0 is not in sync\n");
	vrx318_dbg(DBG_INIT, "%s\n", in_sync(base, 1) ?
		"BC1 is in sync\n" : "BC1 is not in sync\n");

	/* Don't keep idle for emulation, but must keep idle for real case. */
	val = vrx318_r32(PPE_SFSM_CFG0, base) & (~(1 << 15));
	vrx318_w32(val, PPE_SFSM_CFG0, base);
	val = vrx318_r32(PPE_SFSM_CFG1, base) & (~(1 << 15));
	vrx318_w32(val, PPE_SFSM_CFG1, base);

	udelay(5);
}

int vrx318_soc_cfg_get(struct vrx318_priv *priv)
{
	int ret;
	struct vrx318_soc_cfg *cfg = &priv->soc_cfg;

	ret = soc_vrx318_cfg_init(cfg);

	vrx318_dbg(DBG_INIT, "cpu (phybase: 0x%x, dnum: %d), fp (phybase: 0x%x, fp dnum: %d), ds (phybase: 0x%x, dnum: %d), pmac_en: %d, us desown: %d, ds desown: %d\n",
		cfg->soc_cpu_us_phybase, cfg->soc_cpu_us_dnum,
		cfg->soc_fp_us_phybase, cfg->soc_fp_us_dnum,
		cfg->soc_ds_phybase, cfg->soc_ds_dnum,
		cfg->pmac_en,
		cfg->soc_us_desc_own, cfg->soc_ds_desc_own);
	priv->param.pmac_en = cfg->pmac_en;

	return ret;
}

/* VRX318 TC message genelink family */
struct genl_family vrx_tc_gnl_family = {
	.id = GENL_ID_GENERATE, /* To generate an id for the family*/
	.hdrsize = 0,
	.name = VRX_FAMILY_NAME, /*family name, used by userspace application*/
	.version = VERSION_NR, /*version number  */
	.maxattr = VRX_TC_A_MAX,
};

/* VRX318 TC message multicast group */
struct genl_multicast_group vrx_grp = {
	.name = VRX_MCAST_GRP_NAME,
};

/**
 * API defintion for the driver to send TC notify messages to user application
 * using genetlink method.
 * pid: process id
 * tc_mode: ATM_TC/PTM_TC
 * tc_action: TC_LOADED/TC_UNLOADED
 * bnd_mode: BONDING_MODE/NON_BONDING_MODE
 * ln_no: VRX318 ep id
 */
int vrx_tc_msg_send(int pid, int tc_mode, int tc_action,
			int bnd_mode, int ln_no)
{
	struct sk_buff *skb;
	int ret;
	void *msg_head;

	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	/* create the message headers */
	msg_head = genlmsg_put(skb, 0, 0,
			&vrx_tc_gnl_family, 0, VRX_TC_C_NOTIFY);
	if (msg_head == NULL) {
		ret = -ENOMEM;
		vrx318_dbg(DBG_EVENT, "Create message header fail!\n");
		goto out;
	}

	nla_put_u32(skb, VRX_TC_A_TC_MODE, tc_mode);
	nla_put_u32(skb, VRX_TC_A_TC_ACTION, tc_action);
	nla_put_u32(skb, VRX_TC_A_BOND, bnd_mode);
	nla_put_u32(skb, VRX_TC_A_LINENO, ln_no);

	genlmsg_end(skb, msg_head);
	ret = genlmsg_multicast(skb, pid, vrx_grp.id, GFP_KERNEL);
	if (ret != 0) {
		vrx318_dbg(DBG_EVENT, "Failed to send out the multicast message\n");
		goto out;
	}
	return 0;
out:
	return ret;
}

int vrx318_genetlink_init(void)
{
	int ret;

	/*register new family*/
	ret = genl_register_family(&vrx_tc_gnl_family);
	if (ret != 0) {
		vrx318_dbg(DBG_ERR, "Family registeration fail:%s\n",
			vrx_tc_gnl_family.name);
		goto failure;
	}

	ret = genl_register_mc_group(&vrx_tc_gnl_family, &vrx_grp);
	if (ret != 0) {
		vrx318_dbg(DBG_ERR, "register mc group fail: %i, grp name: %s\n",
			ret, vrx_grp.name);
		genl_unregister_family(&vrx_tc_gnl_family);
		goto failure;
	}

	return 0;

failure:
	return ret;
}

void vrx318_genetlink_exit(void)
{
	int ret;

	/* unregister mc groups */
	genl_unregister_mc_group(&vrx_tc_gnl_family, &vrx_grp);

	/*unregister the family*/
	ret = genl_unregister_family(&vrx_tc_gnl_family);
	if (ret != 0)
		vrx318_dbg(DBG_ERR, "unregister Genetlink family %i\n", ret);

	return;
}
