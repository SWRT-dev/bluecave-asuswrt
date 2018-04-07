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
#ifndef __TC_API_H__
#define __TC_API_H__

#include <net/genetlink.h>

static inline int tc_clkon(struct dc_ep_dev *ep_dev, u32 pset)
{
	return ep_dev->hw_ops->clk_on(ep_dev, pset);
}

static inline int tc_clkoff(struct dc_ep_dev *ep_dev, u32 pset)
{
	return ep_dev->hw_ops->clk_off(ep_dev, pset);
}

static inline int tc_ppe_rst(struct dc_ep_dev *ep_dev)
{
	return ep_dev->hw_ops->reset_device(ep_dev, RST_PPE_ALL);
}

static inline int tc_ppe_clkset(struct dc_ep_dev *ep_dev, u32 ppe_clk)
{
	return ep_dev->hw_ops->clk_set(ep_dev, SYS_CLK_288MHZ, ppe_clk);
}

static inline void set_tc_status(struct tc_priv *tcpriv, enum tc_status status)
{
	tcpriv->tc_stat = status;
}

static inline int tc_ppe_clkget(struct dc_ep_dev *ep_dev)
{
	u32 sys_clk, ppe_clk;

	ep_dev->hw_ops->clk_get(ep_dev, &sys_clk, &ppe_clk);

	switch (ppe_clk) {
	case PPE_CLK_576MHZ:
		return 576000000;
	case PPE_CLK_494MHZ:
		return 494000000;
	case PPE_CLK_432MHZ:
		return 432000000;
	case PPE_CLK_288MHZ:
		return 288000000;
	case PPE_CLK_36MHZ:
		return 36000000;
	default:
		WARN(1, "PPE CLK error: 0x%x\n", ppe_clk);
		return 0;
	}
}
static inline void tc_stat_indicate(void *priv, enum dsl_tc_mode tc, int set)
{
	if (set) {
		if (tc == TC_PTM_BND_MODE || tc == TC_PTM_SL_MODE)
			tc_w32(BIT(TC_PTM_SL_MODE), DSL_TC_INDICATE);
		else
			tc_w32(BIT(tc), DSL_TC_INDICATE);
	} else
		tc_w32(0, DSL_TC_INDICATE);
}

/**
 * API to debug PP32, set PC instruction trigger on/off
 * Input:
 *	priv: ATM/PTM/Bonding priv structure
 *	pp32:  PPM index (0 -2)
 *	pc: break point (0 - 2)
 *	addr: break point instruction address
 *	flag: 0 - off, 1 - on
 */
static inline void pp32_dbg_set_pc(void *priv, int pp32,
			int pc, u32 addr, int flag)
{
	if (!flag) { /* off */
		tc_w32(BRK_GRP_PC_OFF(pc, pc), BRK_TRIG(pp32));
		tc_w32(BRK_CONTEXT_MASK_EN, BRK_PC_MASK(pp32, pc));
		tc_w32(0, BRK_PC(pp32, pc));
	} else { /* on */
		tc_w32(addr, BRK_PC(pp32, pc));
		tc_w32(BRK_CONTEXT_MASK_EN | BRK_CONTEXT_MASK,
			BRK_PC_MASK(pp32, pc));
		tc_w32(BRK_GRP_PC_ON(pc, pc), BRK_TRIG(pp32));
	}

	if (tc_r32(BRK_TRIG(pp32)))
		tc_w32(STOP_ON_BREAKPOINT_ON, PP32_CTRL_OPT(pp32));
	else
		tc_w32(STOP_ON_BREAKPOINT_OFF, PP32_CTRL_OPT(pp32));
}

/**
 * API to debug PP32, set data trigger on/off
 * Input:
 *	priv: ATM/PTM/Bonding priv structure
 *	pp32:  PPM index (0 -2)
 *	pc: break point (0 - 2)
 *	addr: break point data address
 *	flag: 0 - off, 1 - on
 */
static inline void pp32_dbg_set_addr(void *priv, int pp32,
			int pc, u32 addr, int flag)
{
	if (!flag) { /* off */
		tc_w32(BRK_GRP_DATA_ADDR_OFF(pc, pc), BRK_TRIG(pp32));
		tc_w32(BRK_CONTEXT_MASK_EN, BRK_DATA_ADDR_MASK(pp32, pc));
		tc_w32(0, BRK_DATA_ADDR(pp32, pc));
	} else { /* on */
		tc_w32(addr, BRK_DATA_ADDR(pp32, pc));
		tc_w32(BRK_CONTEXT_MASK_EN | BRK_CONTEXT_MASK,
			BRK_DATA_ADDR_MASK(pp32, pc));
		tc_w32(BRK_GRP_DATA_ADDR_ON(pc, pc), BRK_TRIG(pp32));
	}
	if (tc_r32(BRK_TRIG(pp32)))
		tc_w32(STOP_ON_BREAKPOINT_ON, PP32_CTRL_OPT(pp32));
	else
		tc_w32(STOP_ON_BREAKPOINT_OFF, PP32_CTRL_OPT(pp32));
}

/* APIs for pp32 debug*/
static inline void pp32_dbg_start(void *priv, int pp32)
{
	tc_w32(CTRL_CMD_RESTART, PP32_CTRL_CMD(pp32));
}

static inline void pp32_dbg_stop(void *priv, int pp32)
{
	tc_w32(CTRL_CMD_STOP, PP32_CTRL_CMD(pp32));
}

static inline void pp32_dbg_step(void *priv, int pp32)
{
	tc_w32(0, PP32_CTRL_CMD(pp32));
	tc_w32(CTRL_CMD_STEP, PP32_CTRL_CMD(pp32));
}

static inline void mbox_set_ier(void *priv, u32 id, u32 ier_en_mask)
{
	tc_w32_flush(ier_en_mask, MBOX_IGU_IER(id));
}

static inline void mbox_clr_isr(void *priv, u32 id, u32 isr_mask)
{
	tc_w32_flush(isr_mask, MBOX_IGU_ISRC(id));
}

static inline void mbox_set_isr(void *priv, u32 id, u32 isr_set)
{
	tc_w32(isr_set, MBOX_IGU_ISRS(id));
}

extern int showtime_stat(struct tc_priv *);
extern void dump_skb_info(struct tc_priv *, struct sk_buff *, u32);
extern void *tc_buf_alloc(void *, size_t, dma_addr_t *,
		enum tc_dir, struct device *);
extern void setup_dfe_loopback(void *, u32);
extern void dfe_loopback_linkup(void *);
extern void tc_gentlk_exit(void);
extern int tc_gentlk_init(struct tc_priv *);
extern int tc_ntlk_msg_send(struct tc_priv *, int, int, int, int, int);
extern void cdma_ch_off(void *, int, int);
extern void cdma_ch_on(void *, int, int);
extern void cdma_ch_cfg(void *, int, int, u32, u32, u32, u32);
extern void tc_cdma_init(void *);
extern void pp32_load(void *, struct ppe_fw *, int);
extern void pp32_reset(void *, u32);
extern void pdbram_clear(void *);
extern void ppe_sb_clear(void *);
extern void pp32_stop(void *, int);
extern void pp32_start(void *, int);
extern void tc_memset(void *, u32, int, size_t);
extern void tc_mem_read(void *, void *, u32, size_t);
extern void tc_mem_write(void *, u32, const void *, size_t);
extern int ppe_fw_load(struct tc_priv *);
extern void ppe_fw_unload(struct tc_priv *);
extern int fw_version_info(struct tc_priv *, char *);
extern u32 tc_addr(u32, u32, u32);
extern void ttha_disable(void *, int);
extern void ttha_enable(void *);
extern void rtha_idle_keep_disable(void *);

#endif /* __TC_API_H__ */
