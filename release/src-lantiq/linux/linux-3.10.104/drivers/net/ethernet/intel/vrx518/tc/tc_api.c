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
#include <linux/firmware.h>
#include <linux/crypto.h>
#include <net/dc_ep.h>

#include "inc/tc_main.h"
#include "inc/reg_addr.h"
#include "inc/dfe.h"
#include "inc/tc_common.h"
#include "inc/tc_api.h"
#include "inc/fw/vrx518_ppe_fw.h"

static const char ppe_fw_name[] = "ppe_fw.bin";
#define VRX518_PPE_FW_ID		0xB
#define MD5_LEN				16

/* TC message genelink family */
static struct genl_family tc_gnl_family = {
	.id = GENL_ID_GENERATE,	/* To generate an id for the family*/
	.hdrsize = 0,
	.name = TC_FAMILY_NAME,	/*family name, used by userspace application*/
	.version = 1,		/*version number  */
	.maxattr = TC_A_MAX - 1,
};

/* TC message multicast group */
static struct genl_multicast_group tc_ml_grp = {
	.name = TC_MCAST_GRP_NAME,
};

/**
 * API to copy DWORD data from SoC DDR to VRX518
 * Input:
 *	priv: ATM/PTM/Bonding priv structure
 *	src: SoC DDR address
 *	dst: VRX518 FPI address
 *	len: byte len (should be mutiple of 4)
 */
void tc_mem_write(void *priv, u32 dst, const void *src, size_t len)
{
	int i;
	const u32 *src_addr = src;

	if (WARN_ON((len & 0x3) != 0))
		return;

	len = TO_DWSZ(len);
	for (i = 0; i < len; i++)
		tc_w32(src_addr[i], (dst + (i << 2)));
}

/**
 *API to copy DWORD data from VRX518 to SoC DDR
 *Input:
 *	priv: ATM/PTM/Bonding priv structure
 *	src:  VRX518 FPI address
 *	dst:  SoC DDR address
 *	len:  byte len  (should be mutiple of 4)
 *
 * This API assume src/dst is DWORD aligned address.
 */
void tc_mem_read(void *priv, void *dst, u32 src, size_t len)
{
	int i;
	u32 *dst_addr = (u32 *)dst;

	if (WARN_ON((len & 0x3) != 0))
		return;

	len = TO_DWSZ(len);
	for (i = 0; i < len; i++)
		dst_addr[i] = tc_r32(src + (i << 2));
}


/**
 * API to set write given content to vrx518 address
 * Input:
 *	priv: ATM/PTM/Bonding priv structure
 *	dst:  VRX518 FPI address
 *	val:  data to write
 *	len:  byte len  (should be mutiple of 4)
 */

void tc_memset(void *priv, u32 dst, int val, size_t len)
{
	int i;

	if (WARN_ON((len & 0x3) != 0))
		return;

	len = TO_DWSZ(len);
	for (i = 0; i < len; i++)
		tc_w32(val, (dst + (i << 2)));
}

/**
 * API to start pp32
 * Input:
 *	priv: ATM/PTM/Bonding priv structure
 *	pp32: PP32 CPU ID: 0 - 2
 */
void pp32_start(void *priv, int pp32)
{
	u32 val;

	if (WARN_ON(pp32 < 0 || pp32 >= PP32_MAX))
		return;

	val = tc_r32(PPE_FREEZE) & (~(FREEZE_PP32(pp32)));
	tc_w32(val, PPE_FREEZE);

	/*  idle for a while to let PP32 init itself */
	udelay(50);

	if (tc_r32(PPE_FREEZE) & FREEZE_PP32(pp32)) {
		tc_err(to_tcpriv(priv), MSG_INIT,
			"Restart PP32(%d) failed: 0x%x,\n",
			pp32, tc_r32(PPE_FREEZE));
		return;
	}
}

/**
 * API to convert SB address to request address
 */
u32 tc_addr(u32 sb_addr, u32 target_addr_type, u32 base)
{
	if (target_addr_type == TC_SRAM_ADDR)
		return SB_XBAR_ADDR(sb_addr) | base;
	else if (target_addr_type == TC_FPI_ADDR)
		return fpi_addr(sb_addr) | base;
	else
		pr_err("Address type is not correct: %d\n", target_addr_type);

	return 0;
}

/* API to stop(freeze) pp32 */
void pp32_stop(void *priv, int pp32)
{
	u32 val;

	if (WARN_ON(pp32 < 0 || pp32 >= PP32_MAX))
		return;

	val = tc_r32(PPE_FREEZE);
	tc_w32(val | FREEZE_PP32(pp32), PPE_FREEZE);
}


/* API to clear all SB */
void ppe_sb_clear(void *priv)
{
	tc_memset(priv, fpi_addr(SB_SEG0_BASE), 0, SB_SEG0_SIZE);
	tc_memset(priv, fpi_addr(SB_SEG1_BASE), 0, SB_SEG1_SIZE);
}

/* API to clear PDBRAM */
void pdbram_clear(void *priv)
{
	tc_memset(priv, PDBRAM_PPE_BASE, 0, PDBRAM_PPE_SIZE);
}

/* API to reset PP32 internal modules, zero to reset */
void pp32_reset(void *priv, u32 reset_set)
{
	u32 pp32_rst;

	pp32_rst = tc_r32(PP32_RST) & (~reset_set);
	tc_w32(PP32_RST, pp32_rst);
	udelay(100);

	pp32_rst |= reset_set;
	tc_w32(PP32_RST, pp32_rst);

	pp32_rst = tc_r32(PP32_RST);
	if ((pp32_rst & reset_set) != reset_set) {
		tc_err(to_tcpriv(priv), MSG_INIT,
			"reset PPE fail, reset set: 0x%x, after reset: 0x%x\n",
			reset_set, pp32_rst);
		return;
	}
}

static void fw_cmp(void *priv, int pp32,
	const u32 *fw_code, u32 fw_addr, size_t size)
{
	int i;

	size = TO_DWSZ(size);
	for (i = 0; i < size; i++) {
		if (fw_code[i] != tc_r32(fw_addr + TO_BYSZ(i))) {
			tc_err(to_tcpriv(priv), MSG_INIT,
				"PP32(%d): fw mem data: 0x%x != original 0x%x @0x%x\n",
				pp32, tc_r32(fw_addr + TO_BYSZ(i)),
				fw_code[i], fw_addr + TO_BYSZ(i));
			return;
		}
	}
}

/**
 * API to download PPE FW code
 * Input:
 *	priv: ATM/PTM/Bonding priv structure
 *	pp32:  PPM index (0 -2)
 *	code_src: FW code array
 *	code_len: FW code array length
 */
static int pp32_fw_download(void *priv, int pp32,
		const u32 *fw_code, size_t size)
{
	u32 code_base;
	size_t code_ram_sz = 0;

	if (size <= 0)
		return 0;

	switch (pp32) {
	case 0:
		code_ram_sz = CDM_SZ(0) + CDM_SZ(1);
		break;

	case 1:
		code_ram_sz = CDM_SZ(2) + CDM_SZ(3);
		break;

	case 2:
		code_ram_sz = CDM_SZ(4) + CDM_SZ(5);
		break;

	default:
		WARN_ON(1);
		break;
	}

	if (code_ram_sz < size) {
		tc_err(to_tcpriv(priv), MSG_INIT,
			"Download Fail!, ram size: 0x%x less than code size: 0x%x\n",
			code_ram_sz, size);
		WARN_ON(1);
		return -ENOMEM;
	}

	code_base = PPM_CODE_MEM_BASE(pp32);

	/* Clear code and data mem */
	tc_memset(priv, code_base, 0, code_ram_sz);

	/* Download FW code and data */
	tc_mem_write(priv, code_base, fw_code, size);

	tc_r32(code_base);
	fw_cmp(priv, pp32, fw_code, code_base, size);

	return 0;
}

void pp32_load(void *priv, struct ppe_fw *fw, int is_atm)
{
	int i;
	struct tc_priv *tcpriv = to_tcpriv(priv);

	if (is_atm) {
		tc_info(tcpriv, MSG_SWITCH,
			"Loading ATM FW ver: %d.%d.%d\n",
			fw->atm_ver.major, fw->atm_ver.mid, fw->atm_ver.minor);
		for (i = 0; i < PP32_MAX; i++) {
			if (TO_BYSZ(fw->atm_fw.size[i])) {
				tc_dbg(tcpriv, MSG_INIT,
				"%s: ATM load data [%p][%d]\n", __func__,
					fw->atm_fw.data[i],
					fw->atm_fw.size[i]);
				pp32_fw_download(priv, i, fw->atm_fw.data[i],
						TO_BYSZ(fw->atm_fw.size[i]));
			} else
				tc_dbg(tcpriv, MSG_INIT,
					"%s: invalid data\n", __func__);
		}
	} else {
		tc_info(tcpriv, MSG_SWITCH,
			"Loading PTM FW ver: %d.%d.%d\n",
			fw->ptm_ver.major, fw->ptm_ver.mid, fw->ptm_ver.minor);
		for (i = 0; i < PP32_MAX; i++)
			pp32_fw_download(priv, i, fw->ptm_fw.data[i],
				TO_BYSZ(fw->ptm_fw.size[i]));
	}
}

static void cdma_ctrl_init(void *priv)
{
	u32 val;
	struct dc_ep_dev *ep_dev = to_epdev(priv);
	struct tc_priv *tcpriv = to_tcpriv(priv);

	/* power up CDMA */
	ep_dev->hw_ops->clk_on(ep_dev, PMU_CDMA);

	/* Global software reset CDMA */
	rw32_mask(priv, 1, 1, CTRL_RST_S, CDMA_CTRL);
	while (tc_r32(CDMA_CTRL) & BIT(CTRL_RST_S))
		;

	val = tc_r32(CDMA_ID);
	tcpriv->dma.chans = MS(val, ID_CHNR);

	val = tc_r32(CDMA_CTRL);

	/**
	* Enable:
	* Packet Arbitration, Meta data copy, Dyanamic Data burst read
	* Byte Enable, Dedicated Descriptor Access port
	*/
	set_mask_bit(val, 1, 1, CTRL_PKTARB_S);
	set_mask_bit(val, 1, 1, CTRL_MDC_S);
	set_mask_bit(val, 1, 1, CTRL_DSRAM_S);
	set_mask_bit(val, 1, 1, CTRL_ENBE_S);
	set_mask_bit(val, 1, 1, CTRL_DCNF_S);  /* 2DW descriptor format */
	set_mask_bit(val, 1, 1, CTRL_DDBR_S);
	tc_w32(val, CDMA_CTRL);

	/* Enable DMA polling */
	val = tc_r32(CDMA_CPOLL);
	val = SM(1, POLL_EN) | SM(POLL_DEF_CNT, POLL_CNT);
	tc_w32(val, CDMA_CPOLL);
}

static void cdma_port_init(void *priv)
{
	u32 val;

	/* Only one port /port 0 */
	tc_w32(0, CDMA_PS);
	val = tc_r32(CDMA_PCTRL);

	/* Set burst size to 16DWs */
	set_mask_bit(val, 1, 1, PCTRL_RXBL16_S);
	set_mask_bit(val, 1, 1, PCTRL_TXBL16_S);
	set_mask_bit(val, 0, 3, PCTRL_RXBL_S);
	set_mask_bit(val, 0, 3, PCTRL_TXBL_S);

	/* Set DMA tx/rx endian */
	set_mask_bit(val, NO_SWAP, 3, PCTRL_TXENDI_S);
	set_mask_bit(val, BYTE_SWAP, 3, PCTRL_RXENDI_S);
	tc_w32(val, CDMA_PCTRL);
}

void tc_cdma_init(void *priv)
{
	cdma_ctrl_init(priv);
	cdma_port_init(priv);
}

static void cdma_ch_set(void *priv, int cid, u32 dbase, u32 dnum)
{
	/* Channel select */
	tc_w32(cid, CDMA_CS);

	/* Channel Reset */
	rw32_mask(priv, 1, 1, CCTRL_RST_S, CDMA_CCTRL);
	while (tc_r32(CDMA_CCTRL) & BIT(CCTRL_RST_S))
		;

	tc_w32(dbase, CDMA_CDBA);
	tc_w32(dnum, CDMA_CDLEN);

	/* Clear All interrupts */
	tc_w32(CDMA_CI_ALL, CDMA_CIS);
	/* Disable all interrupts */
	tc_w32(0, CDMA_CIE);
}

void cdma_ch_cfg(void *priv, int rxid, int txid, u32 rx_base,
	u32 rx_num, u32 tx_base, u32 tx_num)
{
	struct tc_priv *tc_priv;

	tc_priv = to_tcpriv(priv);

	WARN_ON(rxid >= tc_priv->dma.chans);
	WARN_ON(txid >= tc_priv->dma.chans);

	cdma_ch_set(priv, rxid, rx_base, rx_num);
	cdma_ch_set(priv, txid, tx_base, tx_num);
}

static inline void cdma_ch_onoff(void *priv, int cid, enum cdma_onoff onoff)
{
	/* Channel select */
	tc_w32(cid, CDMA_CS);

	/* Channel on/off */
	rw32_mask(priv, onoff, 1, CCTRL_ONOFF_S, CDMA_CCTRL);
}

void cdma_ch_on(void *priv, int rxid, int txid)
{
	struct tc_priv *tc_priv;

	tc_priv = to_tcpriv(priv);
	WARN_ON(rxid >= tc_priv->dma.chans);
	WARN_ON(txid >= tc_priv->dma.chans);

	cdma_ch_onoff(priv, rxid, DMA_CH_ON);
	cdma_ch_onoff(priv, txid, DMA_CH_ON);
}

void cdma_ch_off(void *priv, int rxid, int txid)
{
	struct tc_priv *tc_priv;

	tc_priv = to_tcpriv(priv);
	WARN_ON(rxid >= tc_priv->dma.chans);
	WARN_ON(txid >= tc_priv->dma.chans);

	cdma_ch_onoff(priv, txid, DMA_CH_OFF);
	cdma_ch_onoff(priv, rxid, DMA_CH_OFF);
}

static void halt_ttha(struct tc_comm *priv)
{
	int i;
	u32 val;

	/* Disable idle cell */
	for (i = 0; i < 2; i++) {
		val = tc_r32(FFSM_CFG(i)) & (~(BIT(17)));
		tc_w32_flush(val, FFSM_CFG(i));
	}

	/* DSL FW should stop frame request, Reset DFE if in looback mode */
	if (priv->tc_priv->param.dfe_loopback == 1)
		priv->ep->hw_ops->reset_device(priv->ep, RST_DFE);

	/* Halt TC DMA */
	val = tc_r32(DREG_AT_CFG(0)) | BIT(30);
	tc_w32_flush(val, DREG_AT_CFG(0));

	/* Disable TTHA */
	val = tc_r32(FFSM_CFG(0)) & (~(BIT(31)));
	tc_w32_flush(val, FFSM_CFG(0));

	/* Flush TC DMA */
	val = tc_r32(DREG_AT_CFG(0)) | BIT(31) | BIT(30);
	tc_w32_flush(val, DREG_AT_CFG(0));

	/**
	* After Flush TCDMA, HW page index will automatically reset to 0
	* FW need reset its pointer(0x3EC5) to zero before trigger ttha flush
	*/
}

void ttha_disable(void *priv, int reset)
{
	halt_ttha(priv);
	/* Reset TC DMA */
	if (reset)
		pp32_reset(priv, TCDMA0_RST);
}

void ttha_enable(void *priv)
{
	u32 val;
	int i;

	/* Enable TTHA */
	for (i = 0; i < 2; i++) {
		val = tc_r32(FFSM_CFG(i)) | BIT(31);
		tc_w32(val, FFSM_CFG(i));
	}

	/* Indicate DSL FW Frame request is OK */
	tc_w32(1, DSL_FRAMER_REQ_START);
}

void rtha_idle_keep_disable(void *priv)
{
	u32 val;
	int i;

	/* Disable the Idle keep for RX */
	for (i = 0; i < 2; i++) {
		val = tc_r32(SFSM_CFG(i)) & (~(BIT(15)));
		tc_w32(val, SFSM_CFG(i));
	}
}

/**
 * API defintion for the driver to send TC notify messages to user application
 * using genetlink method.
 * priv: ATM/PTM/BONDING priv structure
 * pid: process id
 * tc_mode: ATM_TC/PTM_TC
 * tc_action: TC_LOADED/TC_UNLOADED
 * bnd_mode: BONDING_MODE/NON_BONDING_MODE
 * ln_no: line id
 */
int tc_ntlk_msg_send(struct tc_priv *priv, int pid, int tc_mode, int tc_action,
			int bnd_mode, int ln_no)
{
	struct sk_buff *skb;
	int ret;
	void *msg_head;

	skb = genlmsg_new(NLMSG_GOODSIZE, GFP_KERNEL);
	if (!skb)
		return -ENOMEM;

	/* create the message headers */
	msg_head = genlmsg_put(skb, 0, 0, &tc_gnl_family, 0, TC_C_NOTIFY);
	if (msg_head == NULL) {
		ret = -ENOMEM;
		tc_err(priv, MSG_EVENT, "Create TC message header fail!\n");
		goto err1;
	}

	nla_put_u32(skb, TC_A_TC_MODE, tc_mode);
	nla_put_u32(skb, TC_A_TC_ACTION, tc_action);
	nla_put_u32(skb, TC_A_BOND, bnd_mode);
	nla_put_u32(skb, TC_A_LINENO, ln_no);

	genlmsg_end(skb, msg_head);
	ret = genlmsg_multicast(skb, pid, tc_ml_grp.id, GFP_KERNEL);
	if (ret) {
		tc_err(priv, MSG_EVENT, "Sent TC multicast message Fail!\n");
		goto err1;
	}
	return 0;
err1:
	return ret;
}

int tc_gentlk_init(struct tc_priv *priv)
{
	int ret;

	/*register new family*/
	ret = genl_register_family(&tc_gnl_family);
	if (ret) {
		tc_err(priv, MSG_EVENT, "Family registeration fail:%s\n",
			tc_gnl_family.name);
		return ret;
	}

	ret = genl_register_mc_group(&tc_gnl_family, &tc_ml_grp);
	if (ret) {
		tc_err(priv, MSG_EVENT, "register mc group fail: %i, grp name: %s\n",
			ret, tc_ml_grp.name);
		genl_unregister_family(&tc_gnl_family);
		return ret;
	}

	return 0;
}

void tc_gentlk_exit(void)
{
	/* unregister mc groups */
	genl_unregister_mc_group(&tc_gnl_family, &tc_ml_grp);
	/*unregister the family*/
	genl_unregister_family(&tc_gnl_family);
}


int showtime_stat(struct tc_priv *priv)
{
	int i;

	if (priv->param.dfe_loopback)
		return 1;

	for (i = 0; i < priv->ep_num; i++) {
		if (priv->showtime[i] == 1)
			return 1;
	}

	return 0;
}

static void dump_skb_data(struct tc_priv *priv,
		struct sk_buff *skb, size_t size, u32 msg_type)
{
	int i;

	if (skb->len < size)
		size = skb->len;
	if (skb->data_len != 0 && skb->data_len < size)
		size = skb->data_len;

	if (!(priv->msg_enable & msg_type))
		return;

	printk("Data dump:");
	for (i = 0; i < size; i++) {
		if (i % 32 == 0)
			printk("\n[%4d]: ", i);
		else if (i % 16 == 0)
			printk("   ");
		printk("%02x ", skb->data[i]);
	}
	printk("\n");
}

void dump_skb_info(struct tc_priv *tcpriv, struct sk_buff *skb, u32 msg_type)
{
	u32 type;

	type = msg_type & (MSG_TX | MSG_RX);
	tc_dbg(tcpriv, type,
		"skb: head: 0x%x, data: 0x%x, tail: 0x%x, end: 0x%x, len: %d\n",
		(u32)skb->head, (u32)skb->data, (u32)skb->tail,
		(u32)skb->end, skb->len);
	tc_dbg(tcpriv, type,
		"skb: clone: %d, users: %d\n",
		skb->cloned, atomic_read(&skb->users));
	tc_dbg(tcpriv, type,
		"skb: nfrag: %d\n", skb_shinfo(skb)->nr_frags);

	type = msg_type & (MSG_TXDATA | MSG_RXDATA);
	dump_skb_data(tcpriv, skb, skb->len, type);
}

/* This function may sleep */
void *tc_buf_alloc(void *priv, size_t size,
		dma_addr_t *phy_addr, enum tc_dir dir, struct device *pdev)
{
	void *buf;
	struct tc_priv *tcpriv;
	dma_addr_t dma_addr;

	tcpriv = to_tcpriv(priv);
	buf = tcpriv->tc_ops.alloc(size, dir);

	if (!buf) {
		tc_err(tcpriv, MSG_INIT,
			"TC Alloc buffer fail!, dir=%d\n", dir);
		WARN_ONCE(1, "VRX518 allocate buffer fail!\n");
		return NULL;
	}

	dma_addr = dma_map_single(pdev, buf, size, DMA_FROM_DEVICE);
	if (unlikely(dma_mapping_error(pdev, dma_addr))) {
		tc_err(tcpriv, MSG_INIT,
			"DMA address mapping error: buf: 0x%x, size: %d, dir: %d\n",
			(u32)buf, size, DMA_FROM_DEVICE);
		WARN_ONCE(1, "DMA address mapping fail!\n");
	}
	dma_unmap_single(pdev, dma_addr, size, DMA_FROM_DEVICE);
	*phy_addr = dma_addr;

	return buf;
}

static u32 mei_rd(void *priv, u32 addr)
{
	tc_w32(addr, MEIAD(ME_DBG_RD_AD));
	while (!(tc_r32(MEIAD(ME_ARC2ME_STAT)) & 0x10))
		;
	tc_w32_flush(0x10, MEIAD(ME_ARC2ME_STAT));

	return tc_r32(MEIAD(ME_DBG_DATA));
}

static void mei_wr(void *priv, u32 addr, u32 val)
{
	tc_w32(addr, MEIAD(ME_DBG_WR_AD));
	tc_w32(val, MEIAD(ME_DBG_DATA));
	while (!(tc_r32(MEIAD(ME_ARC2ME_STAT)) & 0x10))
		;
	tc_w32(0x10, MEIAD(ME_ARC2ME_STAT));
}

static void setup_zephyr(void *priv)
{

	u32 i, data;
	u32 addr;

	for (i = 0; i < ARRAY_SIZE(dfe_cfg_seq); i++) {
		addr = dfe_cfg_seq[i].addr + MEI_OFFSET;
		switch (dfe_cfg_seq[i].type) {
		case ME_DBG_RD:
			data = tc_r32(addr);
			break;
		case ME_DBG_WR:
			tc_w32(dfe_cfg_seq[i].data, addr);
			break;
		}
	}

	/* Read back to make sure Zephry started */
	data = mei_rd(priv, 0x00020c40c);
	tc_dbg(to_tcpriv(priv), MSG_LOOPBACK, "CRI_TSC_CTRL: 0x%08x\n", data);

	data = mei_rd(priv, 0x00020c4dc);
	tc_dbg(to_tcpriv(priv), MSG_LOOPBACK,
		"CRI_RXFFT_STALL_CTRL: 0x%08x\n", data);

	data = mei_rd(priv, 0x00020c4d8);
	tc_dbg(to_tcpriv(priv), MSG_LOOPBACK,
		"CRI_RXQT_STALL_CTRL: 0x%08x\n", data);

	data = mei_rd(priv, 0x00020c4d0);
	tc_dbg(to_tcpriv(priv), MSG_LOOPBACK, "CRI_RXPMS_CTRL: 0x%08x\n", data);

	data = mei_rd(priv, 0x00020c4b4);
	tc_dbg(to_tcpriv(priv), MSG_LOOPBACK, "CRI_TXPMS_CTRL: 0x%08x\n", data);

	return;
}

static void dfe_reset(void *priv)
{
	struct dc_ep_dev *ep = to_epdev(priv);
	ep->hw_ops->reset_device(ep, RST_DFE);
}

static void dfe_zephyr_lb_init(void *priv)
{
	dfe_reset(priv);
	setup_zephyr(priv);
}

static void set_dfe_data_rate(void *priv, u32 nbc_switches, u32 nbc0bytes,
				u32 nbc1bytes, u32 numtimeslots)
{

	/* Num of BC switches for Tx to load into register ZT_R0 */
	mei_wr(priv, 0x000542F4, nbc_switches);

	/* Num of BC switches for Rx to load into register ZR_R0 */
	mei_wr(priv, 0x0005B94C, nbc_switches);

	/* Num of BC0 and BC1 bytes for Tx to load into register ZT_VBC_SIZE */
	mei_wr(priv, 0x00054308, (nbc1bytes << 16) + nbc0bytes);

	/* Num of BC0 and BC1 bytes for Rx to load into register ZR_VBC_SIZE */
	mei_wr(priv, 0x0005B960, (nbc1bytes << 16) + nbc0bytes);

	/* Num of BC0 and BC1 error bytes for Tx to load into register ZT_R12 */
	/* me_dbg_wr(0x00054300, (nBC1ErrBytes << 16) + nBC0ErrBytes); */
	mei_wr(priv, 0x00054300, 0);

	/* Num of BC0 and BC1 error bytes for Rx to load into REG ZR_R12 */
	/* me_dbg_wr(0x0005B958, (nBC1ErrBytes << 16) + nBC0ErrBytes); */
	mei_wr(priv, 0x0005B958, 0);

	/* kick of by writing to CRI registers */
	/* me_dbg_wr(0x0020c40c, 0x8007ffe1); */
	mei_wr(priv, 0x0020c40c, 0x8007ffe0 | numtimeslots);
	mei_wr(priv, 0x0020c49c, 0x00000078);

	return;
}


void setup_dfe_loopback(void *priv, u32 rate)
{
	struct tc_priv *tcpriv = to_tcpriv(priv);
	if (!tcpriv->param.dfe_loopback)
		return;

	tc_dbg(tcpriv, MSG_LOOPBACK, "enable DFE loopback\n");

	/* Set DFE to Zephyr loopback mode */
	/* 0 - ARC core disable ; 1 - ARC core enable */
	dfe_zephyr_lb_init(priv);

	/* Function to change DFE data rate
	 * Bit_rate (MBps) = num_bc_switch *
	 *	(bc0_payld + bc1_payld)  / ((num_time_slot + 1) * 28 us);
	 * Bit_rate (Mbps) = 8 * num_bc_switch *
	 *	(bc0_payld + bc1_payld)  / ((num_time_slot + 1) * 28 us);
	 * set_dfe_data_rate(u8 pcie_port, UINT32 num_bc_switch,
	 *	UINT32 bc0_payld, UINT32 bc1_payld, UINT32 num_time_slot)
	 * rate default for vrx518: 1500
	 */
	if (!rate)
		rate = 1500;

	tc_dbg(tcpriv, MSG_LOOPBACK, "loopback rate: %d\n", rate);
	set_dfe_data_rate(priv, 4, rate, 0, 1);
}

static inline int in_sync(void *priv)
{
	return  tc_r32(SFSM_STATE0) & 0x1;
}

void dfe_loopback_linkup(void *priv)
{
	u32 val;
	int i = 0;
	struct tc_priv *tcpriv = to_tcpriv(priv);

	if (!tcpriv->param.dfe_loopback)
		return;

	tc_w32_flush(0x07, BC0_LINK_STATUS);

	while (!in_sync(priv) && (i++) < 100)
		;
	tc_dbg(tcpriv, MSG_LOOPBACK, "BC0 is %s in sync\n",
		in_sync(priv) ? "" : "NOT");

	tc_info(tcpriv, MSG_EVENT, "disable idle keep only in emulation\n");
	/* Don't keep idle for emulation, but must keep idle for real case. */
	for (i = 0; i < 2; i++) {
		val = tc_r32(SFSM_CFG(i)) & (~(BIT(15)));
		tc_w32_flush(val, SFSM_CFG(i));
		val = tc_r32(FFSM_CFG(i)) & (~(BIT(17)));
		tc_w32_flush(val, FFSM_CFG(i));
	}
}

static void fw_print_header_info(struct tc_priv *priv,
			struct fw_bin *fw_bin)
{
	int i = 0;
	struct fw_hdr *hdr;

	hdr = &fw_bin->fw_hdr;
	tc_dbg(priv, MSG_INIT, "VRX518 PPE Firmware header info\n");
	tc_dbg(priv, MSG_INIT, "\tPTM Version: %d.%d.%d\n",
		hdr->ptm_ver.major,
		hdr->ptm_ver.mid,
		hdr->ptm_ver.minor);
	tc_dbg(priv, MSG_INIT, "\tPTM Feature: %08X\n", hdr->ptm_ver.features);
	tc_dbg(priv, MSG_INIT, "\tATM Version: %d.%d.%d\n",
		hdr->atm_ver.major,
		hdr->atm_ver.mid,
		hdr->atm_ver.minor);
	tc_dbg(priv, MSG_INIT, "\tATM Feature: %08X\n", hdr->atm_ver.features);
	tc_dbg(priv, MSG_INIT, "\tCompability ID: %08X\n", hdr->comp_id);
	tc_dbg(priv, MSG_INIT, "\tSize: %08X\n", hdr->hdr_sz);
	tc_dbg(priv, MSG_INIT, "\tFW built Date: %d-%d-%d\n",
			((hdr->date >> 24) & 0xff),
			((hdr->date >> 16) & 0xff),
			((hdr->date) & 0xffff));
	tc_dbg(priv, MSG_INIT, "\tNumber of firmware: %d\n", hdr->fw_num);
	for (i = 0; i < hdr->fw_num; i++) {
		tc_dbg(priv, MSG_INIT,
			"\t\tFirmware[%d]: ID[%X] size[%d] at[0x%p]\n",
			i,
			hdr->fw_info[i].fw_id,
			hdr->fw_info[i].fw_size,
			fw_bin->fw_ptr[hdr->fw_info[i].fw_id]);
	}
}

static int fw_md5_check(struct tc_priv *priv, const struct firmware *fw)
{
	struct scatterlist sg;
	struct crypto_hash *tfm;
	struct hash_desc desc;
	u8 output[MD5_LEN];
	const u8 *org_md5;
	int i;
	u32 data_len;

	memset(output, 0, sizeof(output));

	tfm = crypto_alloc_hash("md5", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(tfm)) {
		tc_err(priv, MSG_INIT,
			"Failed to allocate MD5 ALG HASH!\n");
		return PTR_ERR(tfm);
	}

	desc.tfm = tfm;
	desc.flags = CRYPTO_TFM_REQ_MAY_SLEEP;
	data_len = fw->size - MD5_LEN;

	sg_init_one(&sg, fw->data, data_len);
	crypto_hash_init(&desc);
	crypto_hash_update(&desc, &sg, data_len);
	crypto_hash_final(&desc, output);

	/* MD5 compare */
	org_md5 = fw->data + data_len;

	for (i = 0; i < MD5_LEN; i++) {
		if (output[i] != org_md5[i]) {
			tc_err(priv, MSG_INIT,
				"MD5 check failed: @[%d], calculated MD5 val: 0x%02x, original MD5 val: 0x%02x\n",
				i, output[i], org_md5[i]);
			WARN_ON_ONCE(1);
			return -EFAULT;
		}
	}

	tc_dbg(priv, MSG_INIT, "MD5 checksum pass!!!\n");
	crypto_free_hash(tfm);

	return 0;
}

static int fw_check_validity(struct tc_priv *priv, struct fw_bin *fw)
{
	u32 fw_id;
	struct fw_hdr *hdr = &fw->fw_hdr;

	if (hdr->comp_id != PPE_FW_COMPATIBILITY_ID) {
		tc_err(priv, MSG_INIT,
			"Compat ID not match! Driver: %d, FW: %d\n",
			PPE_FW_COMPATIBILITY_ID, hdr->comp_id);
		goto fw_check_fail;
	}

	fw_id = hdr->ptm_ver.family;
	if (fw_id != VRX518_PPE_FW_ID) {
		tc_err(priv, MSG_INIT,
			"PTM FW ID not match PPE Family ID: Driver: %d, FW %d\n",
			VRX518_PPE_FW_ID, fw_id);
		goto fw_check_fail;
	}

	fw_id = hdr->atm_ver.family;
	if (fw_id != VRX518_PPE_FW_ID) {
		tc_err(priv, MSG_INIT,
			"ATM FW ID not match PPE Family ID: Driver: %d, FW %d\n",
			VRX518_PPE_FW_ID, fw_id);
		goto fw_check_fail;
	}

	/* MD5 check */
	fw_md5_check(priv, fw->fw);
	return 0;

fw_check_fail:
	return -1;
}

int ppe_fw_load(struct tc_priv *priv)
{
	u32 i, id, off, size;
	struct fw_hdr *fw_hdr;
	struct fw_bin *fw_bin;
	const u32 *src;
	u32 *dst;

	if (request_firmware(&priv->fw.fw, ppe_fw_name, priv->ep_dev[0].dev)) {
		tc_err(priv, MSG_INIT,
			"load PPE Firmware fail (%s)\n", ppe_fw_name);
		return -ENODEV;
	}

	fw_bin = &priv->fw;
	fw_hdr = &fw_bin->fw_hdr;

	/* get fw information */
	src = (const u32 *)fw_bin->fw->data;
	dst = (u32 *)fw_hdr;
	/* Header convert to CPU Endian */
	for (i = 0; i < DW_SZ(*fw_hdr); i++)
		dst[i] = be32_to_cpu(src[i]);

	/* check for valid id */
	fw_check_validity(priv, fw_bin);
	off = sizeof(struct fw_hdr);
	/* alloc memory for firmware */
	for (i = 0; i < fw_hdr->fw_num; i++) {
		id = fw_hdr->fw_info[i].fw_id;
		size = TO_BYSZ(fw_hdr->fw_info[i].fw_size);
		if (id < FW_MAX) {
			fw_bin->fw_ptr[id] = fw_bin->fw->data + off;
			tc_dbg(priv, MSG_INIT, "Firmware pointer id(%d):size(%d), fw addr(%p), off(%d)\n",
				id, size, fw_bin->fw_ptr[id], off);
			off += size;
		} else {
			tc_err(priv, MSG_INIT, "FW ID not correct!(%d)\n", id);
			return -1;
		}
	}

	fw_print_header_info(priv, fw_bin);

	return 0;
}

int fw_version_info(struct tc_priv *priv, char *buffer)
{
	int i = 0;
	struct fw_hdr *hdr;
	if (priv == NULL) {
		pr_err("<%s>: Invalid private data\n", __func__);
		return -EINVAL;
	}
	if (buffer == NULL) {
		tc_err(priv, MSG_INIT, "Invalid buffer\n");
		return -EINVAL;
	}
	hdr = &(priv->fw.fw_hdr);
	sprintf(buffer, "VRX518 PPE Firmware header info\n");
	sprintf(buffer, "\tPTM Version: %d:%d:%d\n",
		hdr->ptm_ver.major,
		hdr->ptm_ver.mid,
		hdr->ptm_ver.minor);
	sprintf(buffer, "\tPTM Feature: %08X\n", hdr->ptm_ver.features);
	sprintf(buffer, "\tATM Version: %d:%d:%d\n",
		hdr->atm_ver.major,
		hdr->atm_ver.mid,
		hdr->atm_ver.minor);
	sprintf(buffer, "\tATM Feature: %08X\n", hdr->atm_ver.features);
	sprintf(buffer, "\tCompability ID: %08X\n", hdr->comp_id);
	sprintf(buffer, "\tSize: %08X\n", hdr->hdr_sz);
	sprintf(buffer, "\tFW built Date: %d-%d-%d\n",
			((hdr->date >> 24) & 0xff),
			((hdr->date >> 16) & 0xff),
			((hdr->date) & 0xffff));
	sprintf(buffer, "\tNumber of firmware: %d\n", hdr->fw_num);
	for (i = 0; i < hdr->fw_num; i++) {
		sprintf(buffer,
			"\t\tFirmware[%d]: ID[%X] size[%d] at[0x%p]\n",
			i,
			hdr->fw_info[i].fw_id,
			hdr->fw_info[i].fw_size,
			priv->fw.fw_ptr[hdr->fw_info[i].fw_id]);
	}
	return 0;
}

void ppe_fw_unload(struct tc_priv *priv)
{
	if (priv->fw.fw)
		release_firmware(priv->fw.fw);
}

