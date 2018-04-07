/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2015 Zhu YiXin<yixin.zhu@lantiq.com>
 */

#ifndef __VRX318_API_H__
#define __VRX318_API_H__

static inline void mbox_clr_isr(u32 mbox_idx, u32 isr_mask, u32 base)
{
	u32 clr_reg;

	clr_reg = (mbox_idx == MBOX_IGU0) ? MBOX_IGU0_ISRC : MBOX_IGU1_ISRC;
	vrx318_w32(isr_mask, clr_reg, base);
}

static inline void mbox_set_ier(u32 mbox_idx, u32 ier_en_mask, u32 base)
{
	u32 ier_reg;

	ier_reg = (mbox_idx == MBOX_IGU0) ? MBOX_IGU0_IER : MBOX_IGU1_IER;
	vrx318_w32(ier_en_mask, ier_reg, base);
}

static inline void pp32_stop(u32 base)
{
	u32 mask = 0x00010001;

	/*  halt PP32   */
	mask |= vrx318_r32(PP32_FREEZE, base);
	vrx318_w32(mask, PP32_FREEZE, base);
}

static inline void pp32_start(int idx, u32 base)
{
	unsigned int mask;

	idx = (idx == 0) ? 0x1 : 0x10000;
	/*  run PP32    */
	mask = vrx318_r32(PP32_FREEZE, base) & (~idx);
	vrx318_dbg(DBG_INIT, "mask : 0x%x\n", mask);
	vrx318_w32(mask, PP32_FREEZE, base);

	/*  idle for a while to let PP32 init itself */
	mdelay(1);

	if (vrx318_r32(PP32_FREEZE, base) & idx) {
		vrx318_dbg(DBG_ERR, "Start PP32 failed: 0x%x, idx: %d, mask:%d\n",
			vrx318_r32(PP32_FREEZE, base), idx, mask);
		BUG();
	}
}


/* indicate TC Type to DSL FW to fast the DSL handshake */
/* set *MUST* be called after initialization */
static inline void vrx318_tc_indicate(enum dsl_tc_mode dsl_tc,
					u32 base, int set)
{
	if (set)
		vrx318_w32((1 << dsl_tc), DSL_TC_INDICATE, base);
	else
		vrx318_w32(0, DSL_TC_INDICATE, base);
}

static inline void vrx318_reset_xdsl(u32 base)
{
	vrx318_w32(2, MEI_RST_CTRL, base);
	udelay(10);
	vrx318_w32(0, MEI_RST_CTRL, base);
}

extern int pp32_download_code(u32, int, const u32 *, unsigned int,
				const u32 *, unsigned int);
extern void reset_ppe(u32);
extern void enable_ppe_ema(u32);
extern void set_ppe_clk(u32, u32);
extern u32 get_ppe_clk(u32);
extern void print_fw_version(char *, struct fw_ver_id *);
extern void dword_clear(void *, int);
extern void dword_mem_write(u32, const void *, int, u32);
extern void dword_mem_read(void *, u32, int, u32);
extern void dword_ep_clear(u32, int, u32);
extern void vrx318_sb_clear(u32);
extern int alloc_data_buffer(struct vrx318_priv *, unsigned int,
			dma_addr_t *, unsigned int *);
extern int free_data_buffer(dma_addr_t);
extern void setup_dfe_loopback(struct vrx318_priv *, u32);
extern void dfe_loopback_linkup(struct vrx318_priv *, u32);
extern int vrx318_soc_cfg_get(struct vrx318_priv *);
extern int vrx318_genetlink_init(void);
extern void vrx318_genetlink_exit(void);
extern int vrx_tc_msg_send(int, int, int, int, int);


#endif /* __VRX318_API_H__ */
