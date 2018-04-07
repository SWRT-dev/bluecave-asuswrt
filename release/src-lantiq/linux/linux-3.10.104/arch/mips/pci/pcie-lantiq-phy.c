/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2009~2015 Lei Chuanhua <chuanhua.lei@lantiq.com>
 *  Copyright(c) 2016 Intel Corporation.
 */
/*!
 * \file pcie-lantiq-phy.c
 * \ingroup PCIE
 * \brief PCIe PHY PLL register programming source file
 */
#include <linux/types.h>
#include <linux/kernel.h>
#include <asm/paccess.h>
#include <linux/delay.h>

#include "pcie-lantiq.h"

enum {
	PCIE_PHY_36MHZ = 0,
	PCIE_PHY_36MHZ_SSC,
	PCIE_PHY_25MHZ,
	PCIE_PHY_25MHZ_SSC,
	PCIE_PHY_100MHZ,
	PCIE_PHY_100MHZ_SSC,
	PCIE_PHY_MAX,
};

/* PCIe PDI only supports 16 bit operation */

#define LTQ_PCIE_PLL_TIMEOUT 1000 /* Tunnable */

static bool lcpll_ssc_init;

static inline u16 pcie_phy_r16(struct ltq_pcie_port *lpp, unsigned int reg)
{
	return ltq_r16(lpp->phy_base + reg);
}

static inline void pcie_phy_w16(struct ltq_pcie_port *lpp,
	u16 val, unsigned int reg)
{
	ltq_w16(val, lpp->phy_base + reg);
}

static inline void pcie_phy_mask(struct ltq_pcie_port *lpp,
	unsigned int reg, u16 val, u16 mask)
{
	u16 data;

	data = pcie_phy_r16(lpp, reg);
	data = (data & (~mask)) | (val & mask);
	pcie_phy_w16(lpp, data, reg);
}

static inline u32 pcie_phy_r32(struct ltq_pcie_port *lpp, unsigned int reg)
{
	return ltq_r32(lpp->phy_base + reg);
}

static inline void pcie_phy_w32(struct ltq_pcie_port *lpp,
	u32 val, unsigned int reg)
{
	ltq_w32(val, lpp->phy_base + reg);
}

static inline void pcie_phy_w32_mask(struct ltq_pcie_port *lpp, u32 clr,
	u32 set, u32 reg)
{
	ltq_w32_mask(clr, set, lpp->phy_base + reg);
}

/* #define LTQ_PCIE_PHY_REG_DUMP */

#ifdef LTQ_PCIE_PHY_REG_DUMP
static void pcie_phy_reg_dump(struct ltq_pcie_port *lpp)
{
	dev_info(lpp->dev, "Port %d PLL REGFILE\n", lpp->id);
	dev_info(lpp->dev, "PCIE_PHY_PLL_CTRL1    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_CTRL1));
	dev_info(lpp->dev, "PCIE_PHY_PLL_CTRL2    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_CTRL2));
	dev_info(lpp->dev, "PCIE_PHY_PLL_CTRL3    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_CTRL3));
	dev_info(lpp->dev, "PCIE_PHY_PLL_CTRL4    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_CTRL4));
	dev_info(lpp->dev, "PCIE_PHY_PLL_CTRL5    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_CTRL5));
	dev_info(lpp->dev, "PCIE_PHY_PLL_CTRL6    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_CTRL6));
	dev_info(lpp->dev, "PCIE_PHY_PLL_CTRL7    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_CTRL7));
	dev_info(lpp->dev, "PCIE_PHY_PLL_A_CTRL1  0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_A_CTRL1));
	dev_info(lpp->dev, "PCIE_PHY_PLL_A_CTRL2  0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_A_CTRL2));
	dev_info(lpp->dev, "PCIE_PHY_PLL_A_CTRL3  0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_A_CTRL3));
	dev_info(lpp->dev, "PCIE_PHY_PLL_STATUS   0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_PLL_STATUS));

	dev_info(lpp->dev, "TX1 REGFILE\n");
	dev_info(lpp->dev, "PCIE_PHY_TX1_CTRL1    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX1_CTRL1));
	dev_info(lpp->dev, "PCIE_PHY_TX1_CTRL2    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX1_CTRL2));
	dev_info(lpp->dev, "PCIE_PHY_TX1_CTRL3    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX1_CTRL3));
	dev_info(lpp->dev, "PCIE_PHY_TX1_A_CTRL1  0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX1_A_CTRL1));
	dev_info(lpp->dev, "PCIE_PHY_TX1_A_CTRL2  0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX1_A_CTRL2));
	dev_info(lpp->dev, "PCIE_PHY_TX1_MOD1     0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX1_MOD1));
	dev_info(lpp->dev, "PCIE_PHY_TX1_MOD2     0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX1_MOD2));
	dev_info(lpp->dev, "PCIE_PHY_TX1_MOD3     0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX1_MOD3));

	dev_info(lpp->dev, "TX2 REGFILE\n");
	dev_info(lpp->dev, "PCIE_PHY_TX2_CTRL1    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX2_CTRL1));
	dev_info(lpp->dev, "PCIE_PHY_TX2_CTRL2    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX2_CTRL2));
	dev_info(lpp->dev, "PCIE_PHY_TX2_A_CTRL1  0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX2_A_CTRL1));
	dev_info(lpp->dev, "PCIE_PHY_TX2_A_CTRL2  0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX2_A_CTRL2));
	dev_info(lpp->dev, "PCIE_PHY_TX2_MOD1     0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX2_MOD1));
	dev_info(lpp->dev, "PCIE_PHY_TX2_MOD2     0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX2_MOD2));
	dev_info(lpp->dev, "PCIE_PHY_TX2_MOD3     0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_TX2_MOD3));

	dev_info(lpp->dev, "RX1 REGFILE\n");
	dev_info(lpp->dev, "PCIE_PHY_RX1_CTRL1    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_RX1_CTRL1));
	dev_info(lpp->dev, "PCIE_PHY_RX1_CTRL2    0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_RX1_CTRL2));
	dev_info(lpp->dev, "PCIE_PHY_RX1_CDR      0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_RX1_CDR));
	dev_info(lpp->dev, "PCIE_PHY_RX1_EI       0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_RX1_EI));
	dev_info(lpp->dev, "PCIE_PHY_RX1_A_CTRL   0x%04x\n",
		pcie_phy_r16(lpp, PCIE_PHY_RX1_A_CTRL));
}
#endif /* LTQ_PCIE_PHY_REG_DUMP */

static void pcie_phy_comm_setup(struct ltq_pcie_port *lpp)
{
	/* PLL Setting */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL1, 0x120e, 0xFFFF);
	/* increase the bias reference voltage */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL2, 0x39D7, 0xFFFF);

	/* Endcnt */
	pcie_phy_mask(lpp, PCIE_PHY_RX1_EI, 0x0004, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_RX1_A_CTRL, 0x6803, 0xFFFF);

	/* force */
	pcie_phy_mask(lpp, PCIE_PHY_TX1_CTRL1, 0x0008, 0x0008);

	/* predrv_ser_en */
	pcie_phy_mask(lpp, PCIE_PHY_TX1_A_CTRL2, 0x0706, 0xFFFF);
	/* ctrl_lim */
	pcie_phy_mask(lpp, PCIE_PHY_TX1_CTRL3, 0x1FFF, 0xFFFF);
	/* ctrl */
	pcie_phy_mask(lpp, PCIE_PHY_TX1_A_CTRL1, 0x0810, 0xFFFF);
	/* predrv_ser_en */
	pcie_phy_mask(lpp, PCIE_PHY_TX2_A_CTRL2, 0x4702, 0x7F00);

	/* RTERM */
	pcie_phy_mask(lpp, PCIE_PHY_TX1_CTRL2, 0x2e00, 0xFFFF);

	/* Improved 100MHz clock output  */
	pcie_phy_mask(lpp, PCIE_PHY_TX2_CTRL2, 0x3096, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_A_CTRL2, 0x4707, 0xFFFF);
	/* Reduced CDR BW to avoid glitches */
	pcie_phy_mask(lpp, PCIE_PHY_RX1_CDR, 0x0235, 0xFFFF);
}

static int pcie_ppm_enabled;

static int __init setup_pcie_ppm(char *str)
{
	if (!strcmp(str, "off"))
		pcie_ppm_enabled = 0;
	else if (!strcmp(str, "on"))
		pcie_ppm_enabled = 1;
	else
		return 0;
	return 1;
}

__setup("pcie_ppm=", setup_pcie_ppm);

int pcie_phy_clock_ppm_enabled(void)
{
	return pcie_ppm_enabled;
}

static void pcie_phy_36mhz_mode_setup(struct ltq_pcie_port *lpp)
{
	int rc_port = lpp->id;

	dev_dbg(lpp->dev, "%s pcie_port %d ppm %s enter\n",
		__func__, rc_port,
		pcie_ppm_enabled ? "enable" : "disable");
#ifdef LTQ_PCIE_PHY_REG_DUMP
	dev_dbg(lpp->dev, "Initial PHY register dump\n");
	pcie_phy_reg_dump(lpp);
#endif
	if (pcie_ppm_enabled) {
		int p0_ppm = 400;
		int p1_ppm = -400;
		u16 sdm_val1 = 0x38e4;
		u16 sdm_val2 = 0xee;

		/* en_ext_mmd_div_ratio */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL3, 0x0000, 0xFFFF);

		/* pll_ensdm */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL2, 0x0300, 0xFF00);

		/* en_const_sdm */
		/* mmd */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL3, 0x2900,
				 0xFFFF);

		/* lf_mode */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL2, 0x0000,
				 0x4000);

		if (rc_port == LTQ_PCIE_PORT0) {
			if (p0_ppm == 200) {
				sdm_val1 = 0xaaa9;
				sdm_val2 = 0xee;
			} else if (p0_ppm == 300) {
				sdm_val1 = 0xe38d;
				sdm_val2 = 0xee;
			} else if (p0_ppm == 350) {
				sdm_val1 = 0xffff;
				sdm_val2 = 0xee;
			} else if (p0_ppm == 400) {
				sdm_val1 = 0x1c70;
				sdm_val2 = 0xef;
			} else if (p0_ppm == 500) {
				sdm_val1 = 0x5554;
				sdm_val2 = 0xef;
			} else {
				/* default 0 ppm */
				sdm_val1 = 0x38e4;
				sdm_val2 = 0xee;
			}
		}

		if (rc_port == LTQ_PCIE_PORT1) {
			if (p1_ppm == -200) {
				sdm_val1 = 0xc71b;
				sdm_val2 = 0xed;
			} else if (p1_ppm == -300) {
				sdm_val1 = 0x8e37;
				sdm_val2 = 0xed;
			} else if (p1_ppm == -350) {
				sdm_val1 = 0x71c6;
				sdm_val2 = 0xed;
			} else if (p1_ppm == -400) {
				sdm_val1 = 0x5554;
				sdm_val2 = 0xed;
			} else if (p1_ppm == -500) {
				sdm_val1 = 0x1c70;
				sdm_val2 = 0xed;
			} else {
				/* default 0 ppm */
				sdm_val1 = 0x38e4;
				sdm_val2 = 0xee;
			}
		}
		/* const_sdm */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL1, sdm_val1, 0xFFFF);
		/* const sdm */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL2, sdm_val2, 0x00FF);

		/* pllmod */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL7, 0x0002, 0xFFFF);
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL6, 0x3a04, 0xFFFF);
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL5, 0xfae3, 0xFFFF);
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL4, 0x1b72, 0xFFFF);
	} else {
		/* en_ext_mmd_div_ratio */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL3, 0x0000, 0xFFFF);
		/* const sdm */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL2, 0x03ee, 0xFFFF);

		/* mmd */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL3, 0x2900, 0xFFFF);

		/* lf_mode */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL2, 0x0000, 0x4000);
		/* const_sdm */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL1, 0x38e4, 0xFFFF);
		/* pllmod */
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL7, 0x0002, 0xFFFF);
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL6, 0x3a04, 0xFFFF);
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL5, 0xfae3, 0xFFFF);
		pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL4, 0x1b72, 0xFFFF);
	}
	dev_dbg(lpp->dev, "%s pcie_port %d ppm %s exit\n", __func__,
		rc_port, pcie_ppm_enabled ? "enable" : "disable");
}

static void pcie_phy_36mhz_ssc_mode_setup(struct ltq_pcie_port *lpp)
{
	dev_dbg(lpp->dev, "%s pcie_port %d enter\n", __func__, lpp->id);
#ifdef LTQ_PCIE_PHY_REG_DUMP
	dev_dbg(lpp->dev, "Initial PHY register dump\n");
	pcie_phy_reg_dump(lpp);
#endif
	/* Increase the bias reference voltage */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL2, 0x39D7, 0xFFFF);
	/* Endcnt */
	pcie_phy_mask(lpp, PCIE_PHY_RX1_EI, 0x0004, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_RX1_A_CTRL, 0x6803, 0xFFFF);
	/* Force */
	pcie_phy_mask(lpp, PCIE_PHY_TX1_CTRL1, 0x0008, 0x0008);
	/* Predrv_ser_en */
	pcie_phy_mask(lpp, PCIE_PHY_TX1_A_CTRL2, 0x0706, 0xFFFF);
	/* ctrl_lim */
	pcie_phy_mask(lpp, PCIE_PHY_TX1_CTRL3, 0x1FFF, 0xFFFF);
	/* ctrl */
	pcie_phy_mask(lpp, PCIE_PHY_TX1_A_CTRL1, 0x0810, 0xFFFF);
	/* predrv_ser_en */
	pcie_phy_mask(lpp, PCIE_PHY_TX2_A_CTRL2, 0x4702, 0x7F00);
	/* RTERM */
	pcie_phy_mask(lpp, PCIE_PHY_TX1_CTRL2, 0x2e00, 0xFFFF);
	/* en_ext_mmd_div_ratio */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL3, 0x0000, 0xFFFF);
	/* const sdm */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL2, 0x06ee, 0xFFFF);
	/* ext_mmd_div_ratio */
	/* pll_ensdm */
	/* mmd */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL3, 0x2900, 0xFFFF);
	/* lf_mode */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL2, 0x0000, 0x4000);
	/* const_sdm */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL1, 0x38e4, 0xFFFF);
	/* pllmod */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL7, 0x0002, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL6, 0x3a04, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL5, 0xfae3, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL4, 0x1c72, 0xFFFF);
	/* improved 100MHz clock output  */
	pcie_phy_mask(lpp, PCIE_PHY_TX2_CTRL2, 0x3096, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_A_CTRL2, 0x4707, 0xFFFF);
	/* reduced CDR BW to avoid glitches */
	pcie_phy_mask(lpp, PCIE_PHY_RX1_CDR, 0x0235, 0xFFFF);

	dev_dbg(lpp->dev, "%s pcie_port %d exit\n", __func__, lpp->id);
}

static void pcie_phy_25mhz_mode_setup(struct ltq_pcie_port *lpp)
{
	dev_dbg(lpp->dev, "%s pcie_port %d enter\n", __func__, lpp->id);
#ifdef LTQ_PCIE_PHY_REG_DUMP
	dev_dbg(lpp->dev, "Initial PHY register dump\n");
	pcie_phy_reg_dump(lpp);
#endif
	/* en_const_sdm */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL1, 0x130e, 0xFFFF);
	/* en_ext_mmd_div_ratio */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL3, 0x0042, 0xFFFF);
	/* pll_ensdm */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL2, 0x0183, 0x0200);
	/* ext_mmd_div_ratio */
	/* mmd */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL3, 0x6900, 0xFFFF);
	/* lf_mode */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL2, 0x4000, 0x4000);

	dev_dbg(lpp->dev, "%s pcie_port %d exit\n", __func__, lpp->id);
}

static void pcie_phy_25mhz_ssc_mode_setup(struct ltq_pcie_port *lpp)
{
	dev_dbg(lpp->dev, "%s pcie_port %d enter\n",
		__func__, lpp->id);
#ifdef LTQ_PCIE_PHY_REG_DUMP
	dev_dbg(lpp->dev, "Initial PHY register dump\n");
	pcie_phy_reg_dump(lpp);
#endif
	/* en_const_sdm */
	/* pll_wavegen */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL2, 0x0683, 0x0400);
	/* en_ext_mmd_div_ratio */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL3, 0x0040, 0xFFFF);
	/* ext_mmd_div_ratio */
	/* mmd */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL3, 0x6900, 0xFFFF);
	/* lf_mode */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL2, 0x4000, 0x4000);
	/* pll_mod */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL7, 0x0001, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL6, 0x8C0A, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL5, 0x52F0, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL4, 0x0000, 0xFFFF);

	dev_dbg(lpp->dev, "%s pcie_port %d exit\n", __func__, lpp->id);
}

static void pcie_phy_100mhz_mode_setup(struct ltq_pcie_port *lpp)
{
	dev_dbg(lpp->dev, "%s pcie_port %d enter\n", __func__, lpp->id);
#ifdef LTQ_PCIE_PHY_REG_DUMP
	dev_dbg(lpp->dev, "Initial PHY register dump\n");
	pcie_phy_reg_dump(lpp);
#endif
	/* en_ext_mmd_div_ratio */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL3, 0x0000, 0xFFFF);
	/* const sdm */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL2, 0x00ee, 0xFFFF);
	/* ext_mmd_div_ratio */
	/* pll_ensdm */
	/* mmd */
	/* ref_clk_sel */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL3, 0x8B00, 0xFFFF);
	/* lf_mode */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_A_CTRL2, 0x4000, 0x4000);
	/* const_sdm */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL1, 0x38e4, 0xFFFF);
	/* pllmod */
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL7, 0x0002, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL6, 0x3a04, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL5, 0xfae3, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_PLL_CTRL4, 0x1b72, 0xFFFF);
	/* Tx2 settings required to receive the external differential clock */
	/* rxck_en =1; pd =1; */
	pcie_phy_mask(lpp, PCIE_PHY_TX2_A_CTRL1, 0x4000, 0x4000);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_CTRL1, 0x0000, 0x0001);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_CTRL1, 0x0004, 0x0004);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_A_CTRL1, 0x1000, 0x1000);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_A_CTRL2, 0x8000, 0x8000);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_A_CTRL1, 0x0000, 0x0001);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_A_CTRL1, 0x0000, 0x0010);
	dev_dbg(lpp->dev, "%s pcie_port %d exit\n", __func__, lpp->id);
}


static int pcie_phy_wait_startup_ready(struct ltq_pcie_port *lpp)
{
	int i;

	for (i = 0; i < LTQ_PCIE_PLL_TIMEOUT; i++) {
		if ((pcie_phy_r16(lpp, PCIE_PHY_PLL_STATUS)
			& 0x0070) == 0x0070) {
			break;
		}
		udelay(10);
	}
	if (i >= LTQ_PCIE_PLL_TIMEOUT) {
		dev_err(lpp->dev, "%s port %d PLL Link timeout\n", __func__,
			lpp->id);
		return -1;
	}
	return 0;
}

static void pcie_phy_load_enable(struct ltq_pcie_port *lpp, int slice)
{
	/* Set the load_en of tx/rx slice to '1' */
	switch (slice) {
	case 1:
		pcie_phy_mask(lpp, PCIE_PHY_TX1_CTRL1, 0x0010, 0x0010);
		break;
	case 2:
		pcie_phy_mask(lpp, PCIE_PHY_TX2_CTRL1, 0x0010, 0x0010);
		break;
	case 3:
		pcie_phy_mask(lpp, PCIE_PHY_RX1_CTRL1, 0x0002, 0x0002);
		break;
	}
}

static void pcie_phy_load_disable(struct ltq_pcie_port *lpp, int slice)
{
	/* set the load_en of tx/rx slice to '0' */
	switch (slice) {
	case 1:
		pcie_phy_mask(lpp, PCIE_PHY_TX1_CTRL1, 0x0000, 0x0010);
		break;
	case 2:
		pcie_phy_mask(lpp, PCIE_PHY_TX2_CTRL1, 0x0000, 0x0010);
		break;
	case 3:
		pcie_phy_mask(lpp, PCIE_PHY_RX1_CTRL1, 0x0000, 0x0002);
		break;
	}
}

static void pcie_phy_load_war(struct ltq_pcie_port *lpp)
{
	int slice;

	for (slice = 1; slice < 4; slice++) {
		pcie_phy_load_enable(lpp, slice);
		udelay(1);
		pcie_phy_load_disable(lpp, slice);
	}
}

static void pcie_phy_tx2_modulation(struct ltq_pcie_port *lpp)
{
	pcie_phy_mask(lpp, PCIE_PHY_TX2_MOD1, 0x1FFE, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_MOD2, 0xFFFE, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_MOD3, 0x0601, 0xFFFF);
	mdelay(1);
	pcie_phy_mask(lpp, PCIE_PHY_TX2_MOD3, 0x0001, 0xFFFF);
}

static void pcie_phy_tx1_modulation(struct ltq_pcie_port *lpp)
{
	pcie_phy_mask(lpp, PCIE_PHY_TX1_MOD1, 0x1FFE, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_TX1_MOD2, 0xFFFE, 0xFFFF);
	pcie_phy_mask(lpp, PCIE_PHY_TX1_MOD3, 0x0601, 0xFFFF);
	mdelay(1);
	pcie_phy_mask(lpp, PCIE_PHY_TX1_MOD3, 0x0001, 0xFFFF);
}

static void pcie_phy_tx_modulation_war(struct ltq_pcie_port *lpp)
{
	int i;

#define PCIE_PHY_MODULATION_NUM 5
	for (i = 0; i < PCIE_PHY_MODULATION_NUM; i++) {
		pcie_phy_tx2_modulation(lpp);
		pcie_phy_tx1_modulation(lpp);
	}
#undef PCIE_PHY_MODULATION_NUM
}

#ifdef LTQ_PCIE_PHY_REG_DUMP
static void pcie_lcpll_reg_dump(struct ltq_pcie_port *lpp)
{
	dev_info(lpp->dev, "PCIE_LCPLL_CFG0    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_CFG0));
	dev_info(lpp->dev, "PCIE_LCPLL_CFG1    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_CFG1));
	dev_info(lpp->dev, "PCIE_LCPLL_SSC_CTRL    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_SSC_CTRL));
	dev_info(lpp->dev, "PCIE_LCPLL_SSC_SCALE    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_SSC_SCALE));
	dev_info(lpp->dev, "PCIE_LCPLL_COEF_0    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_COEF_0));
	dev_info(lpp->dev, "PCIE_LCPLL_COEF_1    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_COEF_1));
	dev_info(lpp->dev, "PCIE_LCPLL_COEF_2    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_COEF_2));
	dev_info(lpp->dev, "PCIE_LCPLL_COEF_3    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_COEF_3));
	dev_info(lpp->dev, "PCIE_LCPLL_COEF_4    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_COEF_4));
	dev_info(lpp->dev, "PCIE_LCPLL_COEF_5    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_COEF_5));
	dev_info(lpp->dev, "PCIE_LCPLL_COEF_6    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_COEF_6));
	dev_info(lpp->dev, "PCIE_LCPLL_COEF_7    0x%08x\n",
		ltq_cgu_r32(PCIE_LCPLL_COEF_7));
}
#endif /* LTQ_PCIE_PHY_REG_DUMP */

#define PCIE_LCPLL_LOOP_CNT	100

static void pcie_phy_power_down(struct ltq_pcie_port *lpp)
{
	pcie_phy_w32_mask(lpp, 0, PCIE_PHY_PIPE_PD_O, PCIE_PHY_CFG4);
	mdelay(1);
}

static void pcie_phy_power_up(struct ltq_pcie_port *lpp)
{
	pcie_phy_w32_mask(lpp, PCIE_PHY_PIPE_PD_O, 0, PCIE_PHY_CFG4);
	mdelay(1);
}

void pcie_lcpll_init(struct ltq_pcie_port *lpp)
{
	pcie_phy_power_down(lpp);

	if (lpp->phy_mode == PCIE_PHY_100MHZ_SSC) {
		int i;

		if (lcpll_ssc_init)
			goto out;
		/* Enable SSC and LCPLL */
		ltq_cgu_w32(0x10003004, PCIE_LCPLL_CFG1);
		ltq_cgu_w32(0x10103004, PCIE_LCPLL_CFG1);

		/* Need to bit 0 from 0 to 1 */
		ltq_cgu_w32(0x00000190, PCIE_LCPLL_CFG0);
		ltq_cgu_w32(0x00000191, PCIE_LCPLL_CFG0);

		for (i = 0; i < PCIE_LCPLL_LOOP_CNT; i++) {
			if ((ltq_cgu_r32(PCIE_LCPLL_CFG0) & LCPLL_CFG0_LOCKED))
				break;
			mdelay(1);
		}
		if (i >= PCIE_LCPLL_LOOP_CNT)
			pr_info("%s LCPLL not locked yet\n", __func__);

		/* Fixed Coefficient parameters */
		ltq_cgu_w32(0x0000FF60, PCIE_LCPLL_COEF_0);
		ltq_cgu_w32(0x0000FF60, PCIE_LCPLL_COEF_1);
		ltq_cgu_w32(0x0000FF60, PCIE_LCPLL_COEF_2);
		ltq_cgu_w32(0x0000FF60, PCIE_LCPLL_COEF_3);
		ltq_cgu_w32(0x000000A0, PCIE_LCPLL_COEF_4);
		ltq_cgu_w32(0x000000A0, PCIE_LCPLL_COEF_5);
		ltq_cgu_w32(0x000000A0, PCIE_LCPLL_COEF_6);
		ltq_cgu_w32(0x000000A0, PCIE_LCPLL_COEF_7);

		/* Program DIV and len parameters */
		ltq_cgu_w32(0x0000FF1C, PCIE_LCPLL_SSC_CTRL);
		ltq_cgu_w32(0x0000FF1F, PCIE_LCPLL_SSC_CTRL);
		ltq_cgu_w32(0x0000FF1C, PCIE_LCPLL_SSC_CTRL);
		ltq_cgu_w32(0x0000FF1D, PCIE_LCPLL_SSC_CTRL);
		mdelay(1);
#ifdef LTQ_PCIE_PHY_REG_DUMP
		pcie_lcpll_reg_dump(lpp);
#endif /* LTQ_PCIE_PHY_REG_DUMP */
		lcpll_ssc_init = true;
		pr_info("PCIe LCPLL SSC Mode Enabled\n");
	}
out:
	pcie_phy_power_up(lpp);
}

int pcie_phy_clock_mode_setup(struct ltq_pcie_port *lpp)
{
	/* Enable PDI to access PCIe PHY register */
	pcie_pdi_pmu_enable(lpp);

	/* Configure PLL and PHY clock */
	pcie_phy_comm_setup(lpp);

	switch (lpp->phy_mode) {
	case PCIE_PHY_36MHZ:
	default:
		pcie_phy_36mhz_mode_setup(lpp);
		break;
	case PCIE_PHY_36MHZ_SSC:
		pcie_phy_36mhz_ssc_mode_setup(lpp);
		break;
	case PCIE_PHY_25MHZ:
		pcie_phy_25mhz_mode_setup(lpp);
		break;
	case PCIE_PHY_25MHZ_SSC:
		pcie_phy_25mhz_ssc_mode_setup(lpp);
		break;
	case PCIE_PHY_100MHZ:
		pcie_phy_100mhz_mode_setup(lpp);
		break;
	}
	/* Enable PCIe PHY and make PLL setting take effect */
	pcie_phy_pmu_enable(lpp);
	/* Check if we are in startup_ready status */
	if (pcie_phy_wait_startup_ready(lpp) != 0)
		return -1;

	pcie_phy_load_war(lpp);

	/* Apply TX modulation workarounds */
	pcie_phy_tx_modulation_war(lpp);

#ifdef LTQ_PCIE_PHY_REG_DUMP
	dev_dbg(lpp->dev, "Modified PHY register dump\n");
	pcie_phy_reg_dump(lpp);
#endif
	return 0;
}
