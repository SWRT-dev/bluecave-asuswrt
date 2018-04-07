/****************************************************************************
**
** FILE NAME	: xway_dmanand.c
** PROJECT		: UEIP
** MODULES		: NAND Flash
**
** DATE			: 06 October 2011
** AUTHOR		: Mohammad Firdaus B Alias Thani
** DESCRIPTION	: High Speed NAND Flash MTD Driver
** COPYRIGHT	: Copyright (c) 2006
**                Infineon Technologies AG
**				  Am Campeon 1-12, 85579 Neubiberg, Germany
**
**	  This program is free software; you can
**	  redistribute it and/or modify
**	  it under the terms of the GNU General Public
**	  License as published by
**	  the Free Software Foundation;
**	  either version 2 of the License, or
**	  (at your option) any later version.
**
** HISTORY
** $Date		 $Author						  $Version	 $Comment
** 06  Oct 2011  Mohammad Firdaus B Alias Thani    1.0		First release
** 03  Sept 2013 Mohammad Firdaus B Alias Thani    2.0		Migrate to OpenWRT
*****************************************************************************/

/*!
  \defgroup LTQ_NAND_DRV UEIP Project - nand flash driver
  \brief UEIP Project - Nand flash driver, supports LANTIQ xRx300C
 */

/*!
  \defgroup LTQ_NAND_DRV_API External APIs
  \ingroup LTQ_NAND_DRV
  \brief External APIs definitions for other modules.
 */

/*!
  \defgroup LTQ_NAND_DRV_STRUCTURE Driver Structures
  \ingroup LTQ_NAND_DRV
  \brief Definitions/Structures of nand module.
 */

/*!
  \file xway_dmanand.h
  \ingroup LTQ_NAND_DRV
  \brief Header file for LANTIQ nand driver
 */

/*!
  \file xway_dmanand.c
  \ingroup LTQ_NAND_DRV
  \brief nand driver main source file.
*/

#include <linux/version.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/module.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>


/* Project header */
#ifdef CONFIG_SOC_GRX500
#include <lantiq_dma.h>
#else
#include <lantiq_dma.h>
#endif /* CONFIG_SOC_GRX500 */
#include <lantiq_soc.h>

#include "xway_dmanand.h"

/* From Opensource */
/* nand registers */
#define EBU_ADDSEL0			0x20
#define EBU_ADDSEL1			0x24
#define EBU_NAND_CON		0xB0
#define EBU_NAND_WAIT		0xB4
#define EBU_NAND_ECC0		0xB8
#define EBU_NAND_ECC_AC		0xBC

/* DMA nand registers */
#ifndef CONFIG_SOC_GRX500
#define DMA_NAND_NDAC_CTL1	0x10
#define DMA_NAND_NDAC_CTL2	0x14
#define DMA_NAND_INT_MSK	0x24
#define DMA_NAND_INT_STAT	0x28
#define DMA_NAND_HSMD_CTL	0x30
#define DMA_NAND_CSMSG0		0x34
#define DMA_NAND_CSMSG1		0x38
#define DMA_NAND_ND_PARA0	0x3C
#define DMA_NAND_ODD_ECC0	0x40
#define DMA_NAND_ODD_ECC1	0x44
#define DMA_NAND_EVEN_ECC0	0x48
#define DMA_NAND_EVEN_ECC1	0x4C
#else
#define DMA_NAND_NDAC_CTL1	0x110
#define DMA_NAND_NDAC_CTL2	0x114
#define DMA_NAND_INT_MSK	0x124
#define DMA_NAND_INT_STAT	0x128
#define DMA_NAND_HSMD_CTL	0x130
#define DMA_NAND_ND_PARA0	0x13C
#define DMA_NAND_ODD_ECC0	0x140
#define DMA_NAND_ODD_ECC1	0x144
#define DMA_NAND_EVEN_ECC0	0x148
#define DMA_NAND_EVEN_ECC1	0x14C
#define DMA_NAND_CSMSG0		0x150
#define DMA_NAND_CSMSG1		0x154
#endif /* CONFIG_SOC_GRX500 */

/* nand commands */
#define NAND_CMD_ALE		(1 << 2)
#define NAND_CMD_CLE		(1 << 3)
#define NAND_CMD_CS			(1 << 4)
#define NAND_WRITE_CMD_RESET	0xff
#define NAND_WRITE_CMD		(NAND_CMD_CS | NAND_CMD_CLE)
#define NAND_WRITE_ADDR		(NAND_CMD_CS | NAND_CMD_ALE)
#define NAND_WRITE_DATA		(NAND_CMD_CS)
#define NAND_READ_DATA		(NAND_CMD_CS)
#define NAND_WAIT_WR_C		(1 << 3)
#define NAND_WAIT_RD		(0x1)

/* we need to tel the ebu which addr we mapped the nand to */
#define ADDSEL1_MASK(x)		(x << 4)
#define ADDSEL0_REGEN		1
#define ADDSEL1_REGEN		1

/* we need to tell the EBU that we have
 * nand attached and set it up properly
 */
#define BUSCON1_SETUP		(1 << 22)
#define BUSCON1_ALEC		(3 << 14)
#define BUSCON1_BCGEN_RES	(3 << 12)
#define BUSCON1_WAITWRC2	(2 << 8)
#define BUSCON1_WAITRDC2	(2 << 6)
#define BUSCON1_HOLDC1		(1 << 4)
#define BUSCON1_RECOVC1		(1 << 2)
#define BUSCON1_CMULT4		1

#define BUSCON0_SETUP		(1 << 22)
#define BUSCON0_ALEC		(3 << 14)
#define BUSCON0_WAITWRC2	(7 << 8)
#define BUSCON0_WAITRDC2	(3 << 6)
#define BUSCON0_HOLDC1		(3 << 4)
#define BUSCON0_RECOVC1		(3 << 2)
#define BUSCON0_CMULT4		2

#define NAND_CON_CE			(1 << 20)
#define NAND_CON_OUT_CS1	(1 << 10)
#define NAND_CON_IN_CS1		(1 << 8)
#define NAND_CON_PRE_P		(1 << 7)
#define NAND_CON_WP_P		(1 << 6)
#define NAND_CON_SE_P		(1 << 5)
#define NAND_CON_CS_P		(1 << 4)
#define NAND_CON_CSMUX		(1 << 1)
#define NAND_CON_NANDM		1

#define LTQ_NAND_TYPE_S			18
#define LTQ_NAND_TYPE			0x40000
#define LTQ_NAND_PCOUNT_S		10
#define LTQ_NAND_PCOUNT			0x3FC00
#define LTQ_NAND_PLANE_MODE_S	9
#define LTQ_NAND_PLANE_MODE		0x0200
#define LTQ_NAND_ECC_MODE_S		8
#define LTQ_NAND_ECC_MODE		0x0100
#define LTQ_NAND_ECC_LOC_S		7
#define LTQ_NAND_ECC_LOC		0x80
#define LTQ_NAND_ECC_STRENGTH_S 6
#define LTQ_NAND_ECC_STRENGTH	0x40
#define LTQ_NAND_PIB_S			4
#define LTQ_NAND_PIB			0x30
#define LTQ_NAND_PAGE_SIZE_S	0
#define LTQ_NAND_PAGE_SIZE		0x0F

#define NAND_ALE_SET		ltq_ebu_w32(ltq_ebu_r32(EBU_NAND_CON) | \
							(1 << 18), EBU_NAND_CON);
#define NAND_ALE_CLEAR		ltq_ebu_w32(ltq_ebu_r32(EBU_NAND_CON) & \
							~(1 << 18), EBU_NAND_CON);
#define NAND_CLE_SET		ltq_ebu_w32(ltq_ebu_r32(EBU_NAND_CON) | \
							(1<<19), EBU_NAND_CON);
#define NAND_CLE_CLEAR		ltq_ebu_w32(ltq_ebu_r32(EBU_NAND_CON) & \
							~(1<<19), EBU_NAND_CON);
#define NAND_CE_SET			ltq_ebu_w32(ltq_ebu_r32(EBU_NAND_CON) | \
							(1<<20), EBU_NAND_CON);
#define NAND_CE_CLEAR		ltq_ebu_w32(ltq_ebu_r32(EBU_NAND_CON) & \
							~(1<<20), EBU_NAND_CON);
#define NAND_READY_CLEAR	ltq_ebu_w32(0, EBU_NAND_WAIT);

#define LTQ_REG_SM(_v, _f)	(((_v) << _f##_S) & (_f))

#define MLC_NAND_DMA_BURST_LEN	  DMA_BURSTL_8DW
#define LQ_MTD_MLC_NAND_BANK_NAME "ifx_nand"
#define DMA_NAND_EVENT		  1
#define DMA_NAND_WRITE		  0x80

/* special nand flashes which need care in
 * calculating page addresses
*/
#define ST_512WB2_NAND		0x2076
#define SAMSUNG_512_3ADDR	0xec75
#define HYNIX_MLC_FLASH		0xaddc

#ifndef CONFIG_EVA
#define NANDPHYSADDR(x)    CPHYSADDR(x)
#else
#define NANDPHYSADDR(x) RPHYSADDR(x)
#endif /* CONFIG_EVA */

static struct dma_device_info *dma_device;
static void update_mlc_nand_addr_lp(struct mtd_info *mtd,
							int page_addr, int col, int cmd);
static void update_mlc_nand_addr_sp(struct mtd_info *mtd,
							int page, int cmd);
static u32 xway_latchcmd;

/* allow users to override the partition in DT using the cmdline */
static const char * const part_probes[] = { "cmdlinepart", "ofpart", NULL };

struct dma_nand_priv {
	u32 ndac_ctl_1;
	u32 ndac_ctl_2;
	u32 current_page;

	int type;		/* ONFI / SAMSUNG */
	int plane_mode; /* Single / 2-plane mode */
	int pcount;		/* no. of cont. rd/wr operations */
	int nand_mode;
	int ecc_mode;		/* 3Byte / 4 Byte mode*/
	int ecc_location;	/* Embedded / Spare area mode */
	int ecc_strength;	/* Safe / Advance mode */
	int pib;		/* Number pages in a block*/
	int pagesize;		/* NAND page size */

	int chip_id;
	int addr_cycle;
	int ecc_status;
	int write_partial;	 /* write partial oob flag */

	int dma_ecc_mode;	 /* toggle between ECC and transparent mode */
	int chip_select;	 /* chip select mode */
	int cs_trigger;		 /* trigger value for CS modes */

	int partial_page_attr;
	int oob_data_status;  /* determines whether oob has data or not */

	/* xrx500 extra features */
	int empty_page_check;	/* Prevent ecc error for blank pages*/
	int bbt_msg_offset;		/* write bbt msg in dma txcn */
	int oob_ecc_offset;		/* ecc offset at 8th byte in OOB area */
	int supports_msg_offset; /* allows customer msg to be written by HW */

	u8 chip_info[8];
	u8 multiplane_wr_cmd;
	u8 multiplane_rd_cmd;

	struct mtd_info *mtd_priv;
	//struct dma_device_info *dma_device;

	wait_queue_head_t dma_nand_wait;
	volatile unsigned long wait_flag;

};

static u8 *tmp_wr_buf;
static u8 *tmp_rd_buf;

/* Reed Solomon calculation based on hardware implementation
 * of ECC generator/calculator
 */
static u8 g_mul(u8 arg1, u8 arg2)
{
	u8 s = 0;
	u8 g_num2alpha[256] = {
		0, 0, 1, 99, 2, 198, 100, 106,
		3, 205, 199, 188, 101, 126, 107, 42,
		4, 141, 206, 78, 200, 212, 189, 225,
		102, 221, 127, 49, 108, 32, 43, 243,
		5, 87, 142, 232, 207, 172, 79, 131,
		201, 217, 213, 65, 190, 148, 226, 180,
		103, 39, 222, 240, 128, 177, 50, 53,
		109, 69, 33, 18, 44, 13, 244, 56,
		6, 155, 88, 26, 143, 121, 233, 112,
		208, 194, 173, 168, 80, 117, 132, 72,
		202, 252, 218, 138, 214, 84, 66, 36,
		191, 152, 149, 249, 227, 94, 181, 21,
		104, 97, 40, 186, 223, 76, 241, 47,
		129, 230, 178, 63, 51, 238, 54, 16,
		110, 24, 70, 166, 34, 136, 19, 247,
		45, 184, 14, 61, 245, 164, 57, 59,
		7, 158, 156, 157, 89, 159, 27, 8,
		144, 9, 122, 28, 234, 160, 113, 90,
		209, 29, 195, 123, 174, 10, 169, 145,
		81, 91, 118, 114, 133, 161, 73, 235,
		203, 124, 253, 196, 219, 30, 139, 210,
		215, 146, 85, 170, 67, 11, 37, 175,
		192, 115, 153, 119, 150, 92, 250, 82,
		228, 236, 95, 74, 182, 162, 22, 134,
		105, 197, 98, 254, 41, 125, 187, 204,
		224, 211, 77, 140, 242, 31, 48, 220,
		130, 171, 231, 86, 179, 147, 64, 216,
		52, 176, 239, 38, 55, 12, 17, 68,
		111, 120, 25, 154, 71, 116, 167, 193,
		35, 83, 137, 251, 20, 93, 248, 151,
		46, 75, 185, 96, 15, 237, 62, 229,
		246, 135, 165, 23, 58, 163, 60, 183
		};

	u8 g_alpha2num[256] = {
		1, 2, 4, 8, 16, 32, 64, 128, 135,
		137, 149, 173, 221, 61, 122, 244, 111,
		222, 59, 118, 236, 95, 190, 251, 113,
		226, 67, 134, 139, 145, 165, 205, 29,
		58, 116, 232, 87, 174, 219, 49, 98,
		196, 15, 30, 60, 120, 240, 103, 206,
		27, 54, 108, 216, 55, 110, 220, 63,
		126, 252, 127, 254, 123, 246, 107, 214,
		43, 86, 172, 223, 57, 114, 228, 79,
		158, 187, 241, 101, 202, 19, 38, 76,
		152, 183, 233, 85, 170, 211, 33, 66,
		132, 143, 153, 181, 237, 93, 186, 243,
		97, 194, 3, 6, 12, 24, 48, 96,
		192, 7, 14, 28, 56, 112, 224, 71,
		142, 155, 177, 229, 77, 154, 179, 225, 69,
		138, 147, 161, 197, 13, 26, 52, 104,
		208, 39, 78, 156, 191, 249, 117, 234,
		83, 166, 203, 17, 34, 68, 136, 151,
		169, 213, 45, 90, 180, 239, 89, 178,
		227, 65, 130, 131, 129, 133, 141, 157,
		189, 253, 125, 250, 115, 230, 75, 150,
		171, 209, 37, 74, 148, 175, 217, 53,
		106, 212, 47, 94, 188, 255, 121, 242,
		99, 198, 11, 22, 44, 88, 176, 231,
		73, 146, 163, 193, 5, 10, 20, 40,
		80, 160, 199, 9, 18, 36, 72, 144, 167,
		201, 21, 42, 84, 168, 215, 41, 82,
		164, 207, 25, 50, 100, 200, 23, 46, 92,
		184, 247, 105, 210, 35, 70, 140, 159,
		185, 245, 109, 218, 51, 102, 204, 31, 62,
		124, 248, 119, 238, 91, 182, 235,
		81, 162, 195, 1
		};

	if ((arg1 == 0) | (arg2 == 0))
		return 0;

	s = (s + g_num2alpha[arg1]) % 255;
	s = (s + g_num2alpha[arg2]) % 255;
	return g_alpha2num[s];
}

static u8 g_add(u8 arg3, u8 arg4)
{
	u8 s = 0;
	s = s ^ arg3;
	s = s ^ arg4;
	return s;
};

static void reed_solomn_128bytes_ecc(struct dma_nand_priv *mlc, 
									const u8 *data_bytes_partial,
									u8 *ecc_data)
{
	u8 g[4];
	u8 temp[3] = {0, 0, 0};
	u8 s[4] = {0, 0, 0, 0};
	u8 degree;
	u8 bytes;
	u8 y;
	u8 i;
	
	/* 3 bytes ECC */
	if (mlc->ecc_mode == 0) {
		g[3] = 14;
		g[2] = 56;
		g[1] = 64;
		g[0] = 0;
		degree = 3;
	} else {
		/* 4 bytes ECC */
		g[3] = 205;
		g[2] =	63;
		g[1] =	92;
		g[0] =	32;
		degree = 4;
	}
	/*Spare mode */
	if (mlc->ecc_location == 0)
		bytes = 128;
	else   /* Embedded mode */
		bytes = 124;

	for (i = 0; i < bytes; i++) {
		y = g_add(s[3], data_bytes_partial[i]);
		temp[0] = g_mul(y, g[3]);
		temp[1] = g_mul(y, g[2]);
		temp[2] = g_mul(y, g[1]);
		s[3] = g_add(s[2], temp[0]);
		s[2] = g_add(s[1], temp[1]);
		s[1] = g_add(s[0], temp[2]);
		s[0] = g_mul(y, g[0]);
	};

	if (mlc->ecc_mode == 0)  /* 3bytes ECC mode */
		s[0] = 255;

	for (i = 0; i < degree; i++)
		ecc_data[i] = s[(degree - 1) - i];
}

static void xway_reset_chip(struct nand_chip *chip)
{
	unsigned long nandaddr = (unsigned long) chip->IO_ADDR_W;
	unsigned long flags;
	unsigned long timeout;

	nandaddr &= ~NAND_WRITE_ADDR;
	nandaddr |= NAND_WRITE_CMD;

	/* finish with a reset */
	timeout = jiffies + msecs_to_jiffies(200);

	spin_lock_irqsave(&ebu_lock, flags);

	writeb(NAND_WRITE_CMD_RESET, (void __iomem *) nandaddr);
	do {
		if ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			break;
		cond_resched();
	} while (!time_after_eq(jiffies, timeout));

	spin_unlock_irqrestore(&ebu_lock, flags);
}

static int ltq_dmanand_ready(struct mtd_info *mtd)
{
	return ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_RD;
}

static void ltq_init_read_buf(struct mtd_info *mtd, u8 *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	unsigned long nandaddr = (unsigned long) this->IO_ADDR_R;
	unsigned long flags;
	int i;

	spin_lock_irqsave(&ebu_lock, flags);
	for (i = 0; i < len; i++) {
		buf[i] = ltq_r8((void __iomem *)(nandaddr | NAND_READ_DATA));
		while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			;
	}
	spin_unlock_irqrestore(&ebu_lock, flags);

#if defined(NAND_DEBUG)
	pr_debug("output tmp read buf: ");
	for (i = 0; i < len; i++) {
		if (!(i % 4))
			pr_debug("\nbyte[%d]: ", i);
		pr_debug("%02x", buf[i]);
	}
	pr_debug("\n");
#endif
}

static unsigned char ltq_dmanand_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	unsigned long nandaddr = (unsigned long) this->IO_ADDR_R;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&ebu_lock, flags);
	ret = ltq_r8((void __iomem *)(nandaddr | NAND_READ_DATA));
	while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
		;
	spin_unlock_irqrestore(&ebu_lock, flags);

	return ret;
}

static void ltq_dmanand_select_chip(struct mtd_info *mtd, int chip)
{
	switch (chip) {
	case -1:
		ltq_ebu_w32_mask(NAND_CON_CE, 0, EBU_NAND_CON);
		ltq_ebu_w32_mask(NAND_CON_NANDM, 0, EBU_NAND_CON);
		break;
	case 0:
		ltq_ebu_w32_mask(0, NAND_CON_NANDM, EBU_NAND_CON);
		ltq_ebu_w32_mask(0, NAND_CON_CE, EBU_NAND_CON);
		break;
	default:
		BUG();
	}
}

static void ltq_dmanand_cmd_ctrl(struct mtd_info *mtd,
			int cmd, unsigned int ctrl)
{
	struct nand_chip *this = mtd->priv;
	unsigned long nandaddr = (unsigned long) this->IO_ADDR_W;
	unsigned long flags;

	if (ctrl & NAND_CTRL_CHANGE) {
		if (ctrl & NAND_CLE) {
			NAND_ALE_CLEAR;
			NAND_CLE_SET;
			xway_latchcmd = NAND_WRITE_CMD;
		} else if (ctrl & NAND_ALE) {
			NAND_CLE_CLEAR;
			NAND_ALE_SET;
			xway_latchcmd = NAND_WRITE_ADDR;
		}  
	}

	if (cmd != NAND_CMD_NONE) {
		spin_lock_irqsave(&ebu_lock, flags);
		writeb(cmd, (void __iomem *) (nandaddr | xway_latchcmd));
		while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			;
		spin_unlock_irqrestore(&ebu_lock, flags);
	} else {
		NAND_ALE_CLEAR;
		NAND_CLE_CLEAR;
	}

	return;
}

static void ltq_dmanand_hwctl(struct mtd_info *mtd, int mode)
{

	if (mode & NAND_CMD_READID)
		NAND_CE_CLEAR
	else if (mode & NAND_CLE)
		NAND_CE_SET;

	return;
}

static int ltq_dmanand_write_oob(struct mtd_info *mtd,
			struct nand_chip *chip, int page)
{
	u8 *buf = chip->oob_poi;
	int i, length = mtd->oobsize;
	int offs = 0;
	struct nand_oobfree *free = chip->ecc.layout->oobfree;
	struct dma_nand_priv *mlc = chip->priv;
	unsigned long nandaddr = (unsigned long) chip->IO_ADDR_W;
	unsigned long flags;

	/* for partial oob writes [location after hw based ecc] */
	if (mlc->write_partial) {
		offs = free->offset;
		length = free->length;
		chip->cmdfunc(mtd, NAND_CMD_SEQIN, mtd->writesize + offs, page);
	} else
	   chip->cmdfunc(mtd, NAND_CMD_SEQIN, mtd->writesize, page);

	spin_lock_irqsave(&ebu_lock, flags);
	for (i = 0; i < length; i++) {
		ltq_w8(buf[i], (void __iomem *) (nandaddr | NAND_WRITE_DATA));
		while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			;
	}
	spin_unlock_irqrestore(&ebu_lock, flags);

	NAND_CE_SET;
	NAND_READY_CLEAR;

	chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);
	chip->dev_ready(mtd);

	NAND_CE_CLEAR;

#if defined(NAND_DEBUG)
	if (debug == 1) {
		pr_debug("output: ");
		for (i = 0; i < mtd->oobsize; i++) {
			if (!(i % 16))
				pr_debug("\nbyte[%d]: ", i);
			pr_debug("%02x", buf[i]);
		}
		pr_debug("\n");
	}
#endif

	return 0;
}

static int ltq_dmanand_read_oob(struct mtd_info *mtd,
					struct nand_chip *chip, int page)
{
	int i;
	unsigned long nandaddr = (unsigned long) chip->IO_ADDR_R;
	unsigned long flags;
	u8 *buf  = chip->oob_poi;

	chip->cmdfunc(mtd, NAND_CMD_READOOB, 0, page);
	chip->dev_ready(mtd);

	spin_lock_irqsave(&ebu_lock, flags);
	for (i = 0; i < mtd->oobsize; i++) {
		buf[i] = ltq_r8((void __iomem *)(nandaddr | NAND_READ_DATA));
		while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			;
	}
	spin_unlock_irqrestore(&ebu_lock, flags);

#if defined(NAND_DEBUG)
		pr_debug("output read_oob, page: %d", page);
		for (i = 0; i < mtd->oobsize; i++) {
			if (!(i % 8))
				pr_debug("\nbyte[%d]: ", i);
		}
		pr_debug("\n");
#endif
	return 0;
}

/* [Work around] - Check whether the page is empty
 * because when fs erase nand eraseblock,
 * there will be no HW ECC data in OOB area causing subsequent
 * H/W reads to fail
 *
 * /return 1 if not empty and 0 if page is empty
 */

/* [Work around] - Check whether the page is empty
 * because when fs erase nand eraseblock,
 * there will be no HW ECC data in OOB area causing subsequent
 * H/W reads to fail
 *
 * /return 1 if not empty and 0 if page is empty
 */

static int check_empty_page_read(struct nand_chip *chip,
								u8 *buf, int len)
{
	int i, j = 0;
	int err_flag = 0;
	int err_cnt = 0;
	int ecc_size = chip->ecc.size;

	for (i = 0; i < len; i++, j++) {
		if (buf[i] != 0xff)
			err_cnt++;
		
		/* if err is larger than threshold, we return bad */
		if (err_cnt > 3)
			return -1;
		
		/* Keep track of bitflips for 
		 * every ecc quantum which is in this
		 * case 128-byte in RS hardware module
		 */
		if ((j >= ecc_size - 1)) {
			if (err_cnt)
				err_flag = 1;
				
			err_cnt = 0;
			j = 0;
		}
	}

	/* If bitflip found on a page read, we set the empty 
	 * page to all 0xff 
	 */
	if (err_flag)
		memset(buf, 0xff, len);

	return 0;
}

static int check_empty_page_write(const u8 *buf, int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if (buf[i] != 0xff)
			return 1;
	}
	return 0;
}

static int ltq_dmanand_write_page_raw(struct mtd_info *mtd,
			struct nand_chip *chip,
			const u8 *buf, int oob_required)
{
	int writesize = mtd->writesize;
	struct dma_nand_priv *mlc = chip->priv;

	chip->write_buf(mtd, buf, writesize);

	if (oob_required)
		chip->ecc.write_oob(mtd, chip, mlc->current_page);

	return 0;
}

static int ltq_dmanand_read_page_raw(struct mtd_info *mtd,
			struct nand_chip *chip,
			u8 *buf, int oob_required, int page)
{
	int writesize = mtd->writesize;
	struct dma_nand_priv *mlc = chip->priv;

	/* read bypassing ecc hardware block */
	mlc->dma_ecc_mode = 1;

	chip->read_buf(mtd, tmp_rd_buf, writesize);
	memcpy(buf, tmp_rd_buf, writesize);
	if (oob_required)
		chip->ecc.read_oob(mtd, chip, page);

	mlc->dma_ecc_mode = 1;

	return 0;
}

static int ltq_dmanand_read_page_hwecc(struct mtd_info *mtd,
			struct nand_chip *chip,
			u8 *buf, int oob_required, int page)
{
	int ret, writesize = mtd->writesize;
	struct dma_nand_priv *mlc = chip->priv;

	NAND_CE_SET;
	asm("sync");

	memset(tmp_rd_buf, 0, writesize);
	chip->read_buf(mtd, tmp_rd_buf, writesize);
	memcpy(buf, tmp_rd_buf, writesize);

	if (oob_required)
		chip->ecc.read_oob(mtd, chip, mlc->current_page);

	/* if intr has occured, let mtd layer handle the badblock */
	if ((mlc->ecc_status) && (mlc->dma_ecc_mode == 1)) {
		if (mlc->empty_page_check == 0) {
			ret = check_empty_page_read(chip, buf, writesize);
			if (!ret) {
				mlc->ecc_status = 0;
				return 0;
			}
		}
#if defined(NAND_DEBUG)
	int i
	for (i = 0; i < mtd->writesize; i++) {
		if (!(i%32))
			pr_debug("\n pos %d: ", i);
		pr_debug("%02x ", buf[i]);
	}
	pr_debug("\n");
#endif
		pr_warn("ECC calculation failed @ page: %d\n",
				mlc->current_page);
		mtd->ecc_stats.failed++;
		mlc->ecc_status = 0;
	}

	return 0;
}

/* write nand via ebu only for special cases where we need to write data+oob
 * hardware ecc engine does not provide a mechanism for writing data to oob
 */
static void write_nand_via_ebu(struct mtd_info *mtd,
			struct nand_chip *chip, const u8 *buf,
			int len, int page)
{
	int i;
	int oobsize = mtd->oobsize;
	unsigned long flags;
	unsigned long nandaddr = (unsigned long) chip->IO_ADDR_W;
	u8 *oobbuf = chip->oob_poi;

	chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00 , page);

	spin_lock_irqsave(&ebu_lock, flags);
	for (i = 0; i < len; i++) {
		ltq_w8(buf[i], (void __iomem *) (nandaddr |
										NAND_WRITE_DATA));
		while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			;
	}

	for (i = 0; i < oobsize; i++) {
		ltq_w8(oobbuf[i], (void __iomem *) (nandaddr |
										NAND_WRITE_DATA));
		while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			;
	}

	spin_unlock_irqrestore(&ebu_lock, flags);

	NAND_CE_SET;
	NAND_READY_CLEAR;

	chip->cmdfunc(mtd, NAND_CMD_PAGEPROG, -1, -1);
	chip->dev_ready(mtd);

	NAND_CE_CLEAR;
}

static int ltq_dmanand_write_page_hwecc(struct mtd_info *mtd,
			struct nand_chip *chip,
			const u8 *buf, int oob_required)
{
	struct dma_nand_priv *mlc = chip->priv;
	int i, writesize = mtd->writesize;
	int pagestatus = check_empty_page_write(buf, writesize);
	int eccbytes = chip->ecc.bytes;
	int eccsteps = chip->ecc.steps;
	int eccsize = chip->ecc.size;
	int *eccpos = chip->ecc.layout->eccpos;
	int page_addr = mlc->current_page;
	int bbt_offs = chip->bbt_td->offs;
	int ver_offs = chip->bbt_td->veroffs;
	u8 *ecc_data = chip->buffers->ecccalc;
	const u8 *p = buf;

	memset(tmp_wr_buf, 0, writesize);
	memcpy(tmp_wr_buf, buf, writesize);

	/* We would only want to use the EBU method to write into the NAND
	 * flash when there are valid data in the OOB area. Otherwise, it
	 * would suffice to just use the DMA method to write to the NAND
	 *
	 * From GRX500, support for customer msg (2 x 32 bit = 8B) is available
	 * and this method uses the existing DMA based NAND hardware to transfer
	 * the data to NAND flash (instead of using EBU method. However, note that
	 * the limitation is only 8B of data is allowed and this is mainly used 
	 * for BBT signatures. Other means of using the OOB (Filesystem marker, etc)
	 * needs the EBU method to write into the NAND flash. i.e set supports_msg_offset 
	 * to zero
	*/
	if (oob_required == 1) {
		pr_info("writing NAND with OOB data at page %d, size: %d oobsize: %d\n",
				page_addr, writesize, mtd->oobsize);

		mlc->oob_data_status = 1;
		if (mlc->supports_msg_offset) {
			if (bbt_offs >= mtd->oobsize || 
				ver_offs >= mtd->oobsize) {
				mlc->oob_data_status = 0;
				pr_err("%s: OOB offsets bbt: %d, ver: %d, more than oobsize: %d\n",
						__func__, bbt_offs, ver_offs, mtd->oobsize);
				return -EBADMSG;
			}

			mlc->dma_ecc_mode = 0x1;
			NAND_CE_SET;
			chip->write_buf(mtd, tmp_wr_buf, writesize);
			mlc->oob_data_status = 0;
			
			return 0;
		} else {
			/* Update address again for EBU NAND write */
			chip->cmdfunc(mtd, NAND_CMD_SEQIN, 0x00, mlc->current_page);

			for (i = 0; eccsteps; eccsteps--, i += eccbytes, p += eccsize)
				reed_solomn_128bytes_ecc(mlc, p, &ecc_data[i]);

			for (i = 0; i < chip->ecc.total; i++)
				chip->oob_poi[eccpos[i]] = ecc_data[i];

#if defined(NAND_DEBUG)
			for (i = 0; i < writesize; i++) {
				if (!(i % 16))
					pr_debug("\nbuf[%d]: ", i);
				pr_debug("%02x", buf[i]);
			}
#endif

			write_nand_via_ebu(mtd, chip, buf, writesize, page_addr);
			mlc->oob_data_status = 0;
			
			return 0;
		}
	}

	/* jffs2 FS does not erase empty pages before writing into them.
	 * this is always an issue especially for padded images where
	 * the empty padded areas are written to NAND flash. This generates
	 * ECC data written to spare area. Subsequent writes to this
	 * empty area will write
	 * over these ECC data causing inaccurate ECC data stored to flash
	 *
	 * for NAND flash that does not support partial page, we try not to write
	 * the page even if it's all 0xff because we will not be able to guarantee
	 * that the next write has no bit errors.
	 */

	/* blank page check need for new filesystem? */
	if (pagestatus) {
		mlc->dma_ecc_mode = 0x1;
		NAND_CE_SET;
		chip->write_buf(mtd, tmp_wr_buf, writesize);
	}

	return 0;
}

static void ltq_nand_write_buf(struct mtd_info *mtd, const u8 *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	struct dma_nand_priv *mlc = chip->priv;
	struct dma_device_info *dma_dev = dma_device;
	int ret_len;
	int type = mlc->type;
	int page_count = mlc->pcount;
	int ecc_mode = mlc->ecc_mode;
	int plane_mode = mlc->plane_mode;
	int ecc_loc = mlc->ecc_location;
	int ecc_strength = mlc->ecc_strength;
	int block_cnt = mlc->pib;
	int page, pagesize = mlc->pagesize;
	int dma_ecc_mode = mlc->dma_ecc_mode;
	int bbt_offs = chip->bbt_td->offs;
	int ver_offs = chip->bbt_td->veroffs;
	u32 reg;

	NAND_ALE_SET;

	if (pagesize == 512)
		page = 0;
	else if (pagesize == 2048)
		page = 1;
	else if (pagesize == 4096)
		page = 2;
	else
		page = 3;

	reg = LTQ_REG_SM(type, LTQ_NAND_TYPE) |
		  LTQ_REG_SM(page_count, LTQ_NAND_PCOUNT) |
		  LTQ_REG_SM(plane_mode, LTQ_NAND_PLANE_MODE) |
		  LTQ_REG_SM(ecc_mode, LTQ_NAND_ECC_MODE) |
		  LTQ_REG_SM(ecc_loc, LTQ_NAND_ECC_LOC) |
		  LTQ_REG_SM(ecc_strength, LTQ_NAND_ECC_STRENGTH) |
		  LTQ_REG_SM(block_cnt, LTQ_NAND_PIB) | page;

	pr_debug("NAND write PARA0 reg: %08x\n", reg);
	pr_debug("NAND write length: %d, page: %08x\n", len, mlc->current_page);

	if ((mlc->bbt_msg_offset) && (mlc->oob_data_status)) {
		/* BBT pattern @ offset bbt_offs */
		ltq_dma_nand_w32((chip->oob_poi[bbt_offs + 3] << 24) | 
						(chip->oob_poi[bbt_offs + 2] << 16) |
						(chip->oob_poi[bbt_offs + 1] << 8) |
						chip->oob_poi[bbt_offs], DMA_NAND_CSMSG0);
		/* version number @ offset ver_offs */
		ltq_dma_nand_w32((chip->oob_poi[ver_offs + 3] << 24) | 
						(chip->oob_poi[ver_offs + 2] << 16) |
						(chip->oob_poi[ver_offs + 1] << 8) |
						chip->oob_poi[ver_offs], DMA_NAND_CSMSG1);
	} else {
		ltq_dma_nand_w32(0xffffffff, DMA_NAND_CSMSG0);
		ltq_dma_nand_w32(0xffffffff, DMA_NAND_CSMSG1);
	}

	ltq_dma_nand_w32(reg, DMA_NAND_ND_PARA0);
	reg = 0x0;
	//ltq_dma_nand_w32(0, DMA_NAND_HSMD_CTL);
	reg |= ((mlc->bbt_msg_offset << 17) | (mlc->empty_page_check << 16) | 
			(mlc->oob_ecc_offset << 11) | mlc->cs_trigger |
			(1 << 10) | dma_ecc_mode);
	
	ltq_dma_nand_w32(reg, DMA_NAND_HSMD_CTL);

	/* Update ndac address registers */
	ltq_dma_nand_w32(mlc->ndac_ctl_1, DMA_NAND_NDAC_CTL1);
	ltq_dma_nand_w32(mlc->ndac_ctl_2, DMA_NAND_NDAC_CTL2);

	ltq_dma_nand_w32((ltq_dma_nand_r32(DMA_NAND_HSMD_CTL) | (1 << 2)),
		DMA_NAND_HSMD_CTL);

	asm("sync");
	
	pr_debug("HSMD_CTL: %08x\n", ltq_dma_nand_r32(DMA_NAND_HSMD_CTL));

	ret_len = dma_device_write(dma_dev, (u8 *)buf, len, NULL);
	if (ret_len != len) {
		pr_err("DMA write to NAND failed!\n");
		return;
	}

	wait_event_interruptible(mlc->dma_nand_wait, test_bit(DMA_NAND_EVENT,
							 &mlc->wait_flag));
	clear_bit(DMA_NAND_EVENT, &mlc->wait_flag);

	/* Just to make sure that the write to NAND flash is fully done */
	while (!(ltq_dma_nand_r32(DMA_NAND_INT_STAT) & (1 << 4)))
		;

	ltq_dma_nand_w32((ltq_dma_nand_r32(DMA_NAND_INT_STAT) |
				(1 << 4)), DMA_NAND_INT_STAT);


	return;

}

static void ltq_nand_read_buf(struct mtd_info *mtd, u8 *buf, int len)
{
	struct nand_chip *chip = mtd->priv;
	struct dma_nand_priv *mlc = chip->priv;
	struct dma_device_info *dma_dev = dma_device;
	int type = mlc->type;
	int page_count = mlc->pcount;
	int ecc_mode = mlc->ecc_mode;
	int plane_mode = mlc->plane_mode;
	int ecc_loc = mlc->ecc_location;
	int ecc_strength = mlc->ecc_strength;
	int block_cnt = mlc->pib;
	int page, pagesize = mlc->pagesize;
	int chan = dma_dev->current_rx_chan;
	int dma_ecc_mode = mlc->dma_ecc_mode;
	u32 reg = 0;

	NAND_ALE_SET;

	if (pagesize == 512)
		page = 0;
	else if (pagesize == 2048)
		page = 1;
	else if (pagesize == 4096)
		page = 2;
	else
		page = 3;

	pr_debug("type: %d, pagecnt: %d, ecc_mode: %d, plane_mode: %d\n",
			type, page_count, ecc_mode, plane_mode);
	pr_debug("ecc_loc: %d, ecc_strength: %d, blk_cnt: %d, pagesize: %d\n",
			ecc_loc, ecc_strength, block_cnt, page);

	reg = LTQ_REG_SM(type, LTQ_NAND_TYPE) |
		  LTQ_REG_SM(page_count, LTQ_NAND_PCOUNT) |
		  LTQ_REG_SM(plane_mode, LTQ_NAND_PLANE_MODE) |
		  LTQ_REG_SM(ecc_mode, LTQ_NAND_ECC_MODE) |
		  LTQ_REG_SM(ecc_loc, LTQ_NAND_ECC_LOC) |
		  LTQ_REG_SM(ecc_strength, LTQ_NAND_ECC_STRENGTH) |
		  LTQ_REG_SM(block_cnt, LTQ_NAND_PIB) | page;

	pr_debug("NAND READ PARA0 reg: %08x\n", reg);
	pr_debug("NAND read length: %d\n", len);

	ltq_dma_nand_w32(reg, DMA_NAND_ND_PARA0);

	/* pr_debug("reg: %08x, PARA0: %08x, dma chan: %d\n",
	   reg, LTQ_REG_R32(LTQ_ND_PARA0), chan); */
	dma_device_desc_setup(dma_dev, (u8 *) buf, len);

	reg = ((mlc->bbt_msg_offset << 17) |
			(mlc->empty_page_check << 16) |
			(mlc->oob_ecc_offset << 11) |
			mlc->cs_trigger | dma_ecc_mode);

	ltq_dma_nand_w32(reg, DMA_NAND_HSMD_CTL);

	/* Update ndac address registers */
	ltq_dma_nand_w32(mlc->ndac_ctl_1, DMA_NAND_NDAC_CTL1);
	ltq_dma_nand_w32(mlc->ndac_ctl_2, DMA_NAND_NDAC_CTL2);

	dma_dev->rx_chan[chan]->open(dma_dev->rx_chan[chan]);

	ltq_dma_nand_w32((ltq_dma_nand_r32(DMA_NAND_HSMD_CTL) | (1 << 2)),
					DMA_NAND_HSMD_CTL);

	pr_debug("HSMD_CTL: %08x\n", ltq_dma_nand_r32(DMA_NAND_HSMD_CTL));

	wait_event_interruptible(mlc->dma_nand_wait, test_bit(DMA_NAND_EVENT,
							 &mlc->wait_flag));
	clear_bit(DMA_NAND_EVENT, &mlc->wait_flag);

	ltq_dma_nand_w32((ltq_dma_nand_r32(DMA_NAND_INT_STAT) | (7 << 4)),
					DMA_NAND_INT_STAT);
		
	/* we have to poll the complete and OWN bit so as to allow the bits
	 * to be cleared before the next read, otherwise DMA descriptor update
	 * will fail */
	poll_dma_ownership_bit(dma_dev);


	return;
}

static void update_mlc_nand_addr_sp(struct mtd_info *mtd, int page, int cmd)
{
	struct nand_chip *chip = mtd->priv;
	struct dma_nand_priv *mlc = chip->priv;
	int writesize = mlc->pagesize;
	u32 tmp_addr, addr_0, addr_1, addr_2 = 0;
	u32 addr_3 = 0;
	u32 addr_4 = 0;

	tmp_addr = page * writesize;
	addr_0 = tmp_addr & 0x000000FF;
	addr_1 = (tmp_addr & 0x0001FE00) >> 9;
	addr_2 = (tmp_addr & 0x001FE000) >> 17;

	mlc->current_page = page;

	if (mlc->addr_cycle == 4) {
		if (mlc->chip_id == ST_512WB2_NAND)
			addr_3 = (tmp_addr & 0x00300000) >> 25;
		else
			addr_3 = (tmp_addr & 0x00200000) >> 25;
	}

	mlc->ndac_ctl_1 = (addr_2 << 24) | (addr_1 << 16) |
					  (addr_0 << 8) | cmd;

	if (cmd == DMA_NAND_WRITE) {
		mlc->ndac_ctl_2 = (mlc->multiplane_wr_cmd << 19) |
				((mlc->addr_cycle) << 16) | (addr_4 << 8) |
				addr_3;
	} else {
		mlc->ndac_ctl_2 =	(mlc->multiplane_rd_cmd << 19) |
							((mlc->addr_cycle) << 16) |
							(addr_4 << 8) | addr_3;
	}

	pr_debug("ndac_1: %08x, ndac_2: %08x\n", mlc->ndac_ctl_1,
				 mlc->ndac_ctl_2);

	return;
}

static void update_mlc_nand_addr_lp(struct mtd_info *mtd,
		int page_addr, int col, int cmd)
{
	struct nand_chip *chip = mtd->priv;
	struct dma_nand_priv *mlc = chip->priv;
	int writesize = mlc->pagesize;
	u32 tmp_addr;
	u32 addr_0 = 0;
	u32 addr_1 = 0, addr_2 = 0;
	u32 addr_3 = 0, addr_4 = 0;

	mlc->current_page = page_addr;

	 pr_debug("page address: %d\n", page_addr); 
	 pr_debug("multiplane: %d, address cycle: %d",
			mlc->multiplane_rd_cmd, (mlc->addr_cycle)); 

	if (writesize == 2048) {
		tmp_addr = page_addr << 12;
		addr_0 = tmp_addr & 0x000000FF;
		addr_1 = (tmp_addr & 0x00000F00) >> 8;
		addr_2 = (tmp_addr & 0x000FF000) >> 12;
		addr_3 = (tmp_addr & 0x0FF00000) >> 20;

		if (mlc->chip_id == HYNIX_MLC_FLASH)
			addr_4 = (tmp_addr & 0xF0000000) >> 28;
		else
			addr_4 = (tmp_addr & 0x70000000) >> 28;
	} else if (writesize == 4096) {
		tmp_addr = page_addr << 13;
		addr_0 = tmp_addr & 0x000000FF;
		addr_1 = (tmp_addr & 0x00001F00) >> 8;
		addr_2 = (tmp_addr & 0x001FE000) >> 13;
		addr_3 = (tmp_addr & 0x1FE00000) >> 21;
		addr_4 = (tmp_addr & 0xE0000000) >> 29;
	} else if (writesize == 8192) {
		tmp_addr = page_addr << 14;
		addr_0 = tmp_addr & 0x000000FF;
		addr_1 = (tmp_addr & 0x00003F00) >> 8;
		addr_2 = (tmp_addr & 0x003FC000) >> 14;
		addr_3 = (tmp_addr & 0x3FC00000) >> 22;
		addr_4 = (tmp_addr & 0xC0000000) >> 30;
	}

	mlc->ndac_ctl_1 = (addr_2 << 24) | (addr_1 << 16) | (addr_0 << 8) |
						cmd;

	if (cmd == DMA_NAND_WRITE) {
		mlc->ndac_ctl_2 = (mlc->multiplane_wr_cmd << 19) |
						  ((mlc->addr_cycle) << 16) |
						  (addr_4 << 8) | addr_3;
	} else {
		mlc->ndac_ctl_2 = (mlc->multiplane_rd_cmd << 19) |
						  ((mlc->addr_cycle) << 16) |
						  (addr_4 << 8) | addr_3;
	}

	pr_debug("cur page: %d, ndac_1: %08x, ndac_2: %08x\n",
		mlc->current_page,
		mlc->ndac_ctl_1, mlc->ndac_ctl_2);

	return;
}

static void ltq_dmanand_command_sp(struct mtd_info *mtd,
			unsigned int command,
			int column, int page_addr)
{
		register struct nand_chip *chip = mtd->priv;
		int ctrl = NAND_CTRL_CLE | NAND_CTRL_CHANGE;
		struct dma_nand_priv *mlc = chip->priv;

		/* for commands with NAND_CMD_SEQIN or NAND_CMD_READ0 with
		 * column bigger than pagesize are meant for oob reads which
		 * we still want it to do in normal nand mode.
		 */

		/* write page command */
		if ((command == NAND_CMD_SEQIN) &&
						(column < mtd->writesize)) {
			update_mlc_nand_addr_sp(mtd, page_addr, DMA_NAND_WRITE);
			return;
		}

		/* read page command */
		if (command == NAND_CMD_READ0 &&
						(column < mtd->writesize)) {
			update_mlc_nand_addr_sp(mtd, page_addr,
							mlc->multiplane_rd_cmd);
			return;
		}

		/*
		 * Write out the command to the device.
		 */
		if (command == NAND_CMD_SEQIN) {
				int readcmd;

				if (column >= mtd->writesize) {
						/* OOB area */
						column -= mtd->writesize;
						readcmd = NAND_CMD_READOOB;
				} else if (column < 256) {
						/* First 256 bytes --> READ0 */
						readcmd = NAND_CMD_READ0;
				} else {
						column -= 256;
						readcmd = NAND_CMD_READ1;
				}
				chip->cmd_ctrl(mtd, readcmd, ctrl);
				ctrl &= ~NAND_CTRL_CHANGE;
		}
		chip->cmd_ctrl(mtd, command, ctrl);

		/*
		 * Address cycle, when necessary
		 */
		ctrl = NAND_CTRL_ALE | NAND_CTRL_CHANGE;

		/* Samsung requirements for the chip during OOB access */
		if ((command == NAND_CMD_READOOB) &&
			(mlc->chip_id == SAMSUNG_512_3ADDR))
			ndelay(10);

		/* Serially input address */
		if (column != -1) {
				/* Adjust columns for 16 bit buswidth */
				if (chip->options & NAND_BUSWIDTH_16)
						column >>= 1;
				chip->cmd_ctrl(mtd, column, ctrl);
				ctrl &= ~NAND_CTRL_CHANGE;
		}
		if (page_addr != -1) {
				chip->cmd_ctrl(mtd, page_addr, ctrl);
				ctrl &= ~NAND_CTRL_CHANGE;
				chip->cmd_ctrl(mtd, page_addr >> 8, ctrl);
				/* One more address cycle for devices > 32MiB */
				if (chip->chipsize > (32 << 20))
						chip->cmd_ctrl(mtd, page_addr >> 16, ctrl);
		}
		chip->cmd_ctrl(mtd, NAND_CMD_NONE, NAND_NCE |
						NAND_CTRL_CHANGE);

		/*
		 * program and erase have their own busy handlers
		 * status and sequential in needs no delay
		 */
		switch (command) {

		case NAND_CMD_PAGEPROG:
		case NAND_CMD_ERASE1:
		case NAND_CMD_ERASE2:
		case NAND_CMD_SEQIN:
		case NAND_CMD_STATUS:
				return;

		case NAND_CMD_RESET:
				if (chip->dev_ready)
						break;
				udelay(chip->chip_delay);
				chip->cmd_ctrl(mtd, NAND_CMD_STATUS,
							   NAND_CTRL_CLE |
							   NAND_CTRL_CHANGE);
				chip->cmd_ctrl(mtd,
							   NAND_CMD_NONE, NAND_NCE |
							   NAND_CTRL_CHANGE);
				while (!(chip->read_byte(mtd) &
						NAND_STATUS_READY))
					;
				return;

				/* This applies to read commands */

		default:
				/*
				 * If we don't have access to the busy pin,
				 * we apply the given
				 * command delay
				 */
				if (!chip->dev_ready) {
						udelay(chip->chip_delay);
						return;
				}
		}
		/* Apply this short delay always to
		 * ensure that we do wait tWB in
		 * any case on any machine. */
		ndelay(100);

		nand_wait_ready(mtd);
}

static void ltq_dmanand_command_lp(struct mtd_info *mtd,
			unsigned int command,
			int column, int page_addr)
{
		register struct nand_chip *chip = mtd->priv;
		struct dma_nand_priv *mlc = chip->priv;

		/* write page command */
		if ((command == NAND_CMD_SEQIN) && (column < mtd->writesize)) {
			if (!mlc->oob_data_status || (mlc->supports_msg_offset == 1)) {
				update_mlc_nand_addr_lp(mtd, page_addr, column,
							DMA_NAND_WRITE);
				return;
			} else {
				mlc->current_page = page_addr;
			}
		}

		/* read page command */
		if (command == NAND_CMD_READ0 &&
						(column < mtd->writesize))  {
			update_mlc_nand_addr_lp(mtd, page_addr, column,
						mlc->multiplane_rd_cmd);
			 return;
		}

		/* Emulate NAND_CMD_READOOB */
		if (command == NAND_CMD_READOOB) {
				column += mtd->writesize;
				command = NAND_CMD_READ0;
		}

		NAND_CLE_SET;

		/* Command latch cycle */
		chip->cmd_ctrl(mtd, command & 0xff,
					   NAND_NCE | NAND_CLE
					   | NAND_CTRL_CHANGE);
		asm("sync");

		NAND_CLE_CLEAR;
		if (column != -1 || page_addr != -1) {
				int ctrl = NAND_CTRL_CHANGE
							| NAND_ALE | NAND_NCE;

				pr_debug("Reading @ column 0x%x, page: %d\n",
					  column, page_addr);

				/* Serially input address */
				if (column != -1) {
					/* Adjust columns for 16 bit buswidth */
					if (chip->options & NAND_BUSWIDTH_16)
						column >>= 1;
						chip->cmd_ctrl(mtd, column & 0xff, ctrl);
						ctrl &= ~NAND_CTRL_CHANGE;
						chip->cmd_ctrl(mtd, (column >> 8) & 0xff, ctrl);
				}
				if (page_addr != -1) {
					chip->cmd_ctrl(mtd, page_addr & 0xff, ctrl);
					chip->cmd_ctrl(mtd, (page_addr >> 8) & 0xff,
									   ctrl);
					/* One more address cycle for devices > 128MiB */
					if (chip->chipsize > (128 << 20))
						chip->cmd_ctrl(mtd,
							(page_addr >> 16) & 0xff,
							NAND_NCE | NAND_ALE);
				}
		}

		NAND_ALE_CLEAR;
		chip->cmd_ctrl(mtd, NAND_CMD_NONE,
							NAND_NCE |
							NAND_CTRL_CHANGE);
		/*
		 * program and erase have their own busy handlers
		 * status, sequential in, and deplete1 need no delay
		 */
		switch (command) {

		case NAND_CMD_CACHEDPROG:
		case NAND_CMD_PAGEPROG:
		case NAND_CMD_ERASE1:
		case NAND_CMD_ERASE2:
		case NAND_CMD_SEQIN:
		case NAND_CMD_RNDIN:
		case NAND_CMD_STATUS:
				return;

		case NAND_CMD_RESET:
				if (chip->dev_ready)
						break;
				udelay(chip->chip_delay);
				chip->cmd_ctrl(mtd, NAND_CMD_STATUS,
							  NAND_NCE | NAND_CLE |
							  NAND_CTRL_CHANGE);
				chip->cmd_ctrl(mtd, NAND_CMD_NONE,
							   NAND_NCE |
							   NAND_CTRL_CHANGE);
				while (!(chip->read_byte(mtd) &
							NAND_STATUS_READY))
					;
				return;

		case NAND_CMD_RNDOUT:
				/* No ready / busy check necessary */
				chip->cmd_ctrl(mtd, NAND_CMD_RNDOUTSTART,
							   NAND_NCE | NAND_CLE |
							   NAND_CTRL_CHANGE);
				chip->cmd_ctrl(mtd, NAND_CMD_NONE,
							   NAND_NCE |
							   NAND_CTRL_CHANGE);
				return;

		case NAND_CMD_READ0:
				/* This applies to read
				 * commands, newer high density
				 * flash device needs a
				 * 2nd read cmd for READ0.
				 */
				chip->cmd_ctrl(mtd, NAND_CMD_READSTART,
							   NAND_CLE |
							   NAND_CTRL_CHANGE);
				chip->cmd_ctrl(mtd, NAND_CMD_NONE,
							   NAND_NCE |
							   NAND_CTRL_CHANGE);

			NAND_CLE_CLEAR;

		default:
				/*
				 * If we don't have access to
				 * the busy pin, we apply the given
				 * command delay
				 */
				if (!chip->dev_ready) {
						udelay(chip->chip_delay);
						return;
				}
		}

		/* Apply this short delay always to
		 * ensure that we do wait tWB in
		 * any case on any machine. */
		ndelay(100);

		nand_wait_ready(mtd);
}

int lq_dma_mlcnand_intr_handler(
			struct dma_device_info *dma_dev,
			int status)
{
	int i;
	u32 ecc_stat;
	struct dma_nand_priv *mlc = dma_dev->priv;

	switch (status) {
	case RCV_INT:
		for (i = 0; i < dma_dev->num_rx_chan; i++)
			dma_dev->rx_chan[i]->close(dma_dev->rx_chan[i]);

		ecc_stat = ltq_dma_nand_r32(DMA_NAND_INT_STAT);
		/* check whether ecc intr has occured
		 * clear ecc intr
		 */
		if (((ecc_stat >> 5) & 0x3) &&
			(mlc->dma_ecc_mode == 1)) {
			mlc->ecc_status = 1;
			ltq_dma_nand_w32((
				ltq_dma_nand_r32(DMA_NAND_INT_STAT) |
				(3 << 5)), DMA_NAND_INT_STAT);
		} else 
			mlc->ecc_status = 0;
			
		asm("sync");
		pr_debug("--ecc_stat: %08x--\n", ecc_stat);

		NAND_ALE_CLEAR;
		set_bit(DMA_NAND_EVENT, &mlc->wait_flag);
		wake_up_interruptible(&mlc->dma_nand_wait);
		break;

	case TX_BUF_FULL_INT:
		for (i = 0; i < dma_dev->num_tx_chan; i++) {
			if (dma_dev->tx_chan[i]->control == IFX_DMA_CH_ON)
				dma_dev->tx_chan[i]->enable_irq(dma_dev->tx_chan[i]);
		}
		break;

	case TRANSMIT_CPT_INT:
		for (i = 0; i < dma_dev->num_tx_chan; i++)
			 dma_dev->tx_chan[i]->disable_irq(dma_dev->tx_chan[i]);

		set_bit(DMA_NAND_EVENT, &mlc->wait_flag);
		wake_up_interruptible(&mlc->dma_nand_wait);
		break;
	}

	return 0;
}

static u8 *mlc_nand_buffer_alloc(int len, int *byte_offset, void **opt)
{
	return NULL;
}

static int mlc_nand_buffer_free(u8 *dataptr, void *opt)
{
	return 0;
}

static int dma_nand_info_query(struct dma_nand_priv *mlc, struct mtd_info *mtd)
{
	int writesize = mtd->writesize;
	int blocksize = mtd->erasesize;
	int addr_cycle, chipshift, pageshift, addr_cycle_count = 0;
	int pg_per_blk;
	struct nand_chip *chip = mtd->priv;

	mlc->pagesize = writesize;

	/* if EMBEDDED loc, change mtd write size, otherwise
	 * use as default nand pagesize
	 */
	if (mlc->ecc_location) { /* spare: 0, embed: 1 */
		if (!mlc->ecc_mode) {  /* 3b: 0, 4b: 1 */
			pr_err("NAND embedded mode does not support 3B ECC\n");
			return -EINVAL;
		}
		/* Overwrite the nand page size */
		mtd->writesize = writesize - (4 * (writesize / 128));
	}

	pg_per_blk = ((blocksize / writesize) >> 6);
	mlc->pib = (pg_per_blk >= 4) ? 3 : pg_per_blk;

	/* if addr cycle is not calculated (for non ONFI flash) */
	if (!(mlc->addr_cycle)) {
		chipshift = chip->chip_shift;
		pageshift = chip->page_shift;

		/* column */
		for (; writesize > 0; ) {
			addr_cycle_count++;
			writesize >>= 10;
		}
		/* rows */
		addr_cycle = chipshift - (pageshift + 1);
		for (; addr_cycle > 0; ) {
			addr_cycle_count++;
			addr_cycle >>= 2;
		}
		mlc->addr_cycle = addr_cycle_count - 2;
		if (mlc->addr_cycle > 2)
			mlc->addr_cycle = 2;
	}
	pr_debug("writesize: %d, pib: %d\n",
				mtd->writesize,  mlc->pib);
	pr_debug("writesize: %d, addr cycle: %d\n",
				mlc->pagesize, mlc->addr_cycle);

	return 0;
}

static int ltq_nand_dma_setup(struct dma_nand_priv *mlc)
{
	int i;

	dma_device = dma_device_reserve("HSNAND");
	if (dma_device == NULL) {
		pr_err("Reserve DMA for MLC NAND failed!\n");
		return -EAGAIN;
	}

	dma_device->intr_handler = &lq_dma_mlcnand_intr_handler;
	dma_device->buffer_alloc = &mlc_nand_buffer_alloc;
	dma_device->buffer_free = &mlc_nand_buffer_free;
	dma_device->tx_endianness_mode = IFX_DMA_ENDIAN_TYPE3;
	dma_device->rx_endianness_mode = IFX_DMA_ENDIAN_TYPE3;
	dma_device->tx_burst_len = MLC_NAND_DMA_BURST_LEN;
	dma_device->rx_burst_len = MLC_NAND_DMA_BURST_LEN;
	dma_device->num_rx_chan = 1;
	dma_device->num_tx_chan = 1;
	dma_device->priv = mlc;

	/* DMA Channel Config for TX direction */
	for (i = 0; i < dma_device->num_tx_chan; i++) {
		dma_device->tx_chan[i]->desc_len = 1;
		dma_device->tx_chan[i]->byte_offset = 0;
		dma_device->tx_chan[i]->control = IFX_DMA_CH_ON;
	}

	/* DMA Channel Config for RX direction */
	for (i = 0; i < dma_device->num_rx_chan; i++) {
		dma_device->rx_chan[i]->desc_len = 1;
		dma_device->rx_chan[i]->byte_offset = 0;
		dma_device->rx_chan[i]->control = IFX_DMA_CH_ON;
	}

	dma_device->current_tx_chan = 0;
	dma_device->current_rx_chan = 0;

	i = dma_device_register(dma_device);

	if (i < 0) {
		pr_err("%s[%d]: DMA register failed!\n",
					__func__, __LINE__);
		return -EAGAIN;
	}

	return 0;
}

static void pre_allocate_ecc_location(struct mtd_info *mtd,
	struct dma_nand_priv *mlc, struct nand_chip *chip,
	int oobsize)
{
	/* Allocate ecc layout info which are not
	 * supported by Linux kernel due to the diff.
	 * oobsize
	 */

	pr_debug("oobsize: %d\n", oobsize);
	/* check if spare area mode */
	if (!(mlc->ecc_location)) {
		switch (oobsize) {
		case 64:
			if (mlc->ecc_mode)
				chip->ecc.layout = &B4_byte_ecc_oobinfo_2048;
			else
				chip->ecc.layout = &B3_byte_ecc_oobinfo_2048;
			break;
		case 218:
			if (mlc->ecc_mode)
				chip->ecc.layout = &B4_byte_ecc_oobinfo_4096_other;
			else
				chip->ecc.layout = &B3_byte_ecc_oobinfo_4096_other;
			break;
		case 224:
			if (mlc->ecc_mode)
				chip->ecc.layout = &B4_byte_ecc_oobinfo_4096;
			else
				chip->ecc.layout = &B3_byte_ecc_oobinfo_4096;
			break;
		case 436:
		case 448:
			if (mlc->ecc_mode)
				chip->ecc.layout = &B4_byte_ecc_oobinfo_8192;
			else
				chip->ecc.layout = &B3_byte_ecc_oobinfo_8192;
			break;

		default:
			pr_err("Unknown flash oobsize\n");
		}
	} else {
		/* embedded mode can ignore ecc layout */
		chip->ecc.layout = &embedded_ecc_generic;
	}

	mtd->ecclayout =  chip->ecc.layout;
	return;
}

/* Populate hardware specific configurations depending on
 * user needs via kernel tree config file
 */
static int ltq_pre_scan_chip_info(struct mtd_info *mtd,
	struct nand_chip *chip, u8 *id)
{
	struct dma_nand_priv *mlc = chip->priv;
	int extid, busw;
	u16 tmp_id;

	mlc->pagesize = 0;	/* set to zero first and config later */
	mlc->pcount = 0; /* try out one page read/write 1st */
	mlc->ecc_status = 0;
	mlc->addr_cycle = 0;
	mlc->pib = 0;
	mlc->oob_data_status = 0;

	mtd->bitflip_threshold = chip->ecc.strength;

	/* meant to identify specific chips during page address
	 * calculation
	*/
	tmp_id = (id[0] << 8) | id[1];
	mlc->chip_id = (int) tmp_id;

	chip->cellinfo = id[2];
	extid = id[3];
	
	if (id[0] == id[6] && id[1] == id[7] &&
		(id[0] == NAND_MFR_SAMSUNG ||
		id[0] == NAND_MFR_HYNIX) &&
		(id[2] & NAND_CI_CELLTYPE_MSK) &&
		id[5] != 0x00) {

		mtd->writesize = 2048 << (extid & 0x3);
		extid >>= 2;
		/* Calc oobsize */
		switch (extid & 0x03) {
		case 1:
			if (id[0] == NAND_MFR_HYNIX)
				mtd->oobsize = 224;
			else
				mtd->oobsize = 128;
			break;
		case 2:
			if (id[0] == NAND_MFR_HYNIX)
				mtd->oobsize = 448;
			else
				mtd->oobsize = 218;
			break;
		case 3:
			mtd->oobsize = 400;
			break;
		default: /* 0 */
			if (id[0] == NAND_MFR_HYNIX)
				mtd->oobsize = 128;
			else
				mtd->oobsize = 436;
		}
		extid >>= 2;
		/* Calc Blocksize */
		if (extid & 0x08) {
			mtd->erasesize = (1024 * 1024) << (extid & 0x01);
		} else {
			mtd->erasesize = (128 * 1024) <<
				(((extid >> 1) & 0x04) | (extid & 0x03));
		}
		busw = 0;
	} else {
		mtd->writesize = 1024 << (extid & 0x3);
		extid >>= 2;
		/* Calc oobsize */
		mtd->oobsize = (8 << (extid & 0x01)) * (mtd->writesize >> 9);
		extid >>= 2;
		mtd->erasesize = (64 * 1024) << (extid & 0x3);
		extid >>= 2;
		busw = (extid & 0x01) ? NAND_BUSWIDTH_16 : 0;
	}

    /* for SLC NAND, it is possible to write both data & OOB area
	 * seperately
	 */
	mlc->partial_page_attr = 2;
	mlc->plane_mode = 0;
	mlc->type = 0;
	mlc->multiplane_wr_cmd = 0x80;
	mlc->multiplane_rd_cmd = 0x00;

	if (mlc->ecc_mode)
		chip->ecc.bytes = 4;
	else
		chip->ecc.bytes = 3;
	
	pre_allocate_ecc_location(mtd, mlc, chip, mtd->oobsize);

	return busw;
}

static void ltq_onfi_driver_settings(struct mtd_info *mtd) 
{
	struct nand_chip *chip = mtd->priv;
	struct dma_nand_priv *mlc = chip->priv;
	struct nand_onfi_params *p = &chip->onfi_params;
	u16 id = (p->model[0] << 8 | p->model[1]);

	mlc->chip_id = (int) id;
	mlc->addr_cycle = (p->addr_cycles >> 4) + (p->addr_cycles & 0xf);
	mlc->addr_cycle -= 3;
	/* multiplane rd/wr commands for ONFI flash */
	mlc->type = 1; // ONFI complient
	mlc->multiplane_wr_cmd = 0x80;
	mlc->multiplane_rd_cmd = 0x0;
	mlc->partial_page_attr = p->programs_per_page;
	mlc->plane_mode = 0;
	
	if (mlc->ecc_mode)
		chip->ecc.bytes = 4;
	else
		chip->ecc.bytes = 3;
	
	chip->ecc.total = chip->ecc.bytes * chip->ecc.steps;

	pr_debug("id: 0x%04x, addr cycle: %d", id, mlc->addr_cycle);

	pre_allocate_ecc_location(mtd, mlc, chip, mtd->oobsize);
	return;
}

static int nand_define_flash_bbt(int pagesize, struct nand_chip *chip)
{	
	struct dma_nand_priv *mlc = chip->priv;

	switch (pagesize) {
	case 512:
		/* no oob area left for 4B ECC mode
		 * with spare area mode */
		if (mlc->ecc_mode)
			return -1;
	case 496:
		chip->cmdfunc = ltq_dmanand_command_sp;
		chip->badblock_pattern = &factory_default;
		chip->bbt_td = &ltq_nand_main_desc_512;
		chip->bbt_md = &ltq_nand_mirror_desc_512;
		break;
	case 2048:
	case 1984:
		/* no oob area left for 4B ECC mode
		 * with spare area mode */
		if (mlc->ecc_mode)
			return -1;
		chip->cmdfunc = ltq_dmanand_command_lp;
		chip->badblock_pattern = &factory_default;
		chip->bbt_td = &ltq_nand_main_desc_2048;
		chip->bbt_md = &ltq_nand_mirror_desc_2048;
		break;
	case 4096:
	case 3968:
		chip->cmdfunc = ltq_dmanand_command_lp;
		chip->badblock_pattern = &factory_default;
		if (mlc->ecc_mode) {
			chip->bbt_td = &ltq_nand_main_desc_4096;
			chip->bbt_md = &ltq_nand_mirror_desc_4096;
		} else {
			chip->bbt_td = &ltq_nand_main_desc_4096_3BECC;
			chip->bbt_md = &ltq_nand_mirror_desc_4096_3BECC;
		}
		break;
	case 8192:
	case 7936:
		chip->cmdfunc = ltq_dmanand_command_lp;
		chip->badblock_pattern = &factory_default;
		if (mlc->ecc_mode) {
			chip->bbt_td = &ltq_nand_main_desc_8192;
			chip->bbt_md = &ltq_nand_mirror_desc_8192;
		} else {
			chip->bbt_td = &ltq_nand_main_desc_8192_3BECC;
			chip->bbt_md = &ltq_nand_mirror_desc_8192_3BECC;
		}
		break;
	default:
		  pr_err("Unable to determind page size for BBT definition\n");
		  return -1;
	}

	return 0;
}


static int ltq_nand_chip_init(struct platform_device *pdev,
	struct mtd_info *mtd)
{
	u32 cs_flag = 0;
	struct nand_chip *this = mtd->priv;
	struct dma_nand_priv *mlc = this->priv;

	unsigned long nandaddr = (unsigned long) this->IO_ADDR_W;
	const __be32 *cs = of_get_property(pdev->dev.of_node,
					"lantiq,cs", NULL);
	const __be32 *ecc_mode = of_get_property(pdev->dev.of_node,
					"lantiq,eccmode", NULL);
	const __be32 *ecc_location = of_get_property(pdev->dev.of_node,
					"lantiq,ecclocation", NULL);
	const __be32 *ecc_strength = of_get_property(pdev->dev.of_node,
					"lantiq,eccstrength", NULL);

	if (!ecc_location || !ecc_strength ||
			!ecc_mode) {
		pr_err("Error reading DTS info. Please check your DTS configurations\n");
		return -EINVAL;
	}
	/* set configuration for hardware  */
	/* 1: 4B ecc, 0: 3B ecc */
	mlc->ecc_mode = *ecc_mode;
	/* 0: oob area, 1: embedded mode */
	mlc->ecc_location = *ecc_location;
	/* 0: safe mode, 1: advance mode */
	mlc->ecc_strength = *ecc_strength;

    if ((mlc->ecc_strength) && (mlc->ecc_mode))
        this->ecc.strength = 3;
    else if ((!mlc->ecc_strength) && (mlc->ecc_mode))
        this->ecc.strength = 2;
    else if ((mlc->ecc_strength) && (!mlc->ecc_mode))
        this->ecc.strength = 2;
    else
        this->ecc.strength = 1;

	if (cs && (*cs == 0)) {
		mlc->chip_select = 0;
		mlc->cs_trigger = 0x08;

		ltq_ebu_w32(CPHYSADDR(nandaddr)
			|  ADDSEL1_MASK(1) | ADDSEL0_REGEN, EBU_ADDSEL0);

		ltq_ebu_w32(BUSCON0_SETUP | BUSCON0_ALEC
			| BUSCON0_WAITWRC2 | BUSCON0_WAITRDC2
			| BUSCON0_HOLDC1 | BUSCON0_RECOVC1
			| BUSCON0_CMULT4, LTQ_EBU_BUSCON0);

		ltq_ebu_w32(NAND_CON_CSMUX | NAND_CON_CS_P
			| NAND_CON_SE_P | NAND_CON_WP_P
			|NAND_CON_PRE_P, EBU_NAND_CON);

	} else if (cs && (*cs == 1)) {
		mlc->chip_select = 1;
		mlc->cs_trigger = 0x10;
		cs_flag = NAND_CON_OUT_CS1 | NAND_CON_IN_CS1;

        if (of_machine_is_compatible("lantiq,vr9")) {
            ltq_ebu_w32(NANDPHYSADDR(nandaddr)
                | ADDSEL1_MASK(2) | ADDSEL1_REGEN, EBU_ADDSEL1);
        } else if (of_machine_is_compatible("lantiq,grx500")) {
            ltq_ebu_w32(0x17400051, EBU_ADDSEL0);
            ltq_ebu_w32(NANDPHYSADDR(nandaddr)
                | ADDSEL1_MASK(5) | ADDSEL1_REGEN, EBU_ADDSEL1);
        } else {
            ltq_ebu_w32(NANDPHYSADDR(nandaddr)
                | ADDSEL1_MASK(2) | ADDSEL1_REGEN, EBU_ADDSEL1);
        }

		ltq_ebu_w32(BUSCON1_SETUP | BUSCON1_ALEC
			| BUSCON1_BCGEN_RES | BUSCON1_WAITWRC2
			| BUSCON1_WAITRDC2 | BUSCON1_HOLDC1
			| BUSCON1_RECOVC1
			| BUSCON1_CMULT4, LTQ_EBU_BUSCON1);

		ltq_ebu_w32(NAND_CON_NANDM | NAND_CON_CSMUX
			| NAND_CON_CS_P	| NAND_CON_SE_P
			| NAND_CON_WP_P | NAND_CON_PRE_P
			| cs_flag, EBU_NAND_CON);

	} else {
		pr_err("Platform does not support chip select %d\n",
				cs_flag);
	}

	/* Enable ECC intr. and clear intr status */
#ifndef CONFIG_SOC_GRX500
	ltq_ebu_w32((ltq_ebu_r32(DMA_NAND_INT_MSK) |
				(0x3 << 5)), DMA_NAND_INT_MSK);
	ltq_ebu_w32((ltq_ebu_r32(DMA_NAND_INT_STAT) |
				(0x3 << 5)), DMA_NAND_INT_STAT);
#else
	ltq_dma_nand_w32((ltq_dma_nand_r32(DMA_NAND_INT_MSK) |
				(0x3 << 5)), DMA_NAND_INT_MSK);
	ltq_dma_nand_w32((ltq_dma_nand_r32(DMA_NAND_INT_STAT) |
				(0x3 << 5)), DMA_NAND_INT_STAT);
#endif
	/* Reset nand chip */
	xway_reset_chip(this);

	return 0;
}

struct lantiq_dmanand_cfg {
	unsigned int has_ecc_offset;
	unsigned int bbt_msg_offset;
	unsigned int empty_page_ecc_check;
	unsigned int supports_msg_offset;
};

static struct lantiq_dmanand_cfg xway_nand_cfg = {
	.has_ecc_offset = 0,
	.bbt_msg_offset = 0,
	.empty_page_ecc_check = 0,
	.supports_msg_offset = 0,
};

static struct lantiq_dmanand_cfg xrx500_nand_cfg = {
	.has_ecc_offset = 1,
	.bbt_msg_offset = 1,
	.empty_page_ecc_check = 1,
	.supports_msg_offset = 1,
};

static struct of_device_id xway_dmanand_match_table[] = {
	{	.compatible = "lantiq,dma-nand-xway", 
		.data = &xway_nand_cfg
	},
	{	.compatible = "lantiq,dma-nand-xrx500",
		.data = &xrx500_nand_cfg
	},
	{},
};

static int __init ltq_dmanand_probe(struct platform_device *pdev)
{
	int err = 0;
	int n = 0;
	struct mtd_partition *mtd_parts = NULL;
	struct nand_chip *this;
	struct resource *res;
	const struct of_device_id *match;
	const struct lantiq_dmanand_cfg *config;
	void __iomem *io_base;
	struct mtd_info *dma_nand_mtd;
	struct dma_nand_priv *dma_nand_dev;

	/* clear address */
	dma_nand_dev = kmalloc(sizeof(struct dma_nand_priv),
					GFP_KERNEL);
	if (!dma_nand_dev) {
		pr_err("Unable to allocate memory for NAND device structure\n");
		err = -ENOMEM;
		return err;
	}

	match = of_match_device(xway_dmanand_match_table, &pdev->dev);
	if (match) {
		config = (const struct lantiq_dmanand_cfg *) match->data;
	} else {
		pr_err("%s failed to find matching config data\n", __func__);
		goto out;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		pr_err("Cannot get platform device for DMA NAND driver\n");
		err = -ENODEV;
		goto out;
	}

	if (!request_mem_region(res->start, resource_size(res),
		dev_name(&pdev->dev))) {
		pr_err("request mem region failed for DMA NAND\n");
		err = -EBUSY;
		goto out;
	}

	io_base = ioremap(res->start, resource_size(res));
	if (!io_base) {
		pr_err("io_remap failed for DMA NAND driver\n");
		err = -ENOMEM;
		goto out;
	}

	/* set default settings */
	dma_nand_dev->chip_id = 0x0;
	dma_nand_dev->ndac_ctl_1 = 0x0;
	dma_nand_dev->ndac_ctl_2 = 0x0;
	dma_nand_dev->current_page = 0x0;
	dma_nand_dev->dma_ecc_mode = 1;
	dma_nand_dev->pagesize = 0;	/* set to zero first and config later */
	dma_nand_dev->pcount = 0; /* try out one page read/write 1st */
	dma_nand_dev->ecc_status = 0;
	dma_nand_dev->addr_cycle = 0;
	dma_nand_dev->pib = 0;
	dma_nand_dev->oob_data_status = 0;

	dma_nand_dev->empty_page_check = config->empty_page_ecc_check;
	dma_nand_dev->bbt_msg_offset = config->bbt_msg_offset;
	dma_nand_dev->oob_ecc_offset = config->has_ecc_offset;
	dma_nand_dev->supports_msg_offset = config->supports_msg_offset;

	pr_info("empty pg: %d, bbt_offset: %d, oob_ecc_offset %d, support_msg_offset: %d\n",
		dma_nand_dev->empty_page_check, dma_nand_dev->bbt_msg_offset, 
		dma_nand_dev->oob_ecc_offset, dma_nand_dev->supports_msg_offset);

	pr_info("Initializing MLCNAND driver\n");
	dma_nand_mtd = kzalloc(sizeof(struct mtd_info) +
				 sizeof(struct nand_chip), GFP_KERNEL);
	if (!dma_nand_mtd) {
		pr_info("Unable to allocate MLC MTD device structure\n");
		err = -ENOMEM;
		goto out;
	}

	this = (struct nand_chip *)(&dma_nand_mtd[1]);
	dma_nand_mtd->name = LQ_MTD_MLC_NAND_BANK_NAME;

	/* Associate MTD priv members with the current MTD info*/
	dma_nand_mtd->priv = this;
	dma_nand_mtd->owner = THIS_MODULE;

	this->IO_ADDR_R = io_base;
	this->IO_ADDR_W = io_base;
	this->cmd_ctrl = ltq_dmanand_cmd_ctrl;

	/* 30 us command delay, similar to NAND driver specs */
	this->chip_delay = 30;

	/* hw ecc specific read/write functions */
	this->ecc.mode = NAND_ECC_HW;
	this->ecc.hwctl = ltq_dmanand_hwctl;
	this->ecc.write_page_raw = ltq_dmanand_write_page_raw;
	this->ecc.write_page = ltq_dmanand_write_page_hwecc;
	this->ecc.read_page_raw = ltq_dmanand_read_page_raw;
	this->ecc.read_page = ltq_dmanand_read_page_hwecc;
	this->ecc.read_oob = ltq_dmanand_read_oob;
	this->ecc.write_oob = ltq_dmanand_write_oob;

	this->options = NAND_SKIP_BBTSCAN | NAND_NO_SUBPAGE_WRITE;
	this->bbt_options = NAND_BBT_USE_FLASH | NAND_BBT_CREATE_EMPTY;
	this->read_byte = ltq_dmanand_read_byte;
	
	this->priv = dma_nand_dev;

	ltq_nand_chip_init(pdev, dma_nand_mtd);

	/* this is needed for the ONFI read of ~768B.
	 * the ONFI read is done per byte using traditional EBU access.
	 * DMA read is unable to handle such data width.
	 */
	this->read_buf = ltq_init_read_buf;
	this->write_buf = ltq_nand_write_buf;
	this->dev_ready = ltq_dmanand_ready;
	this->select_chip = ltq_dmanand_select_chip;

	/* Need to support for both dma-nand way of updating address
	 * and the normal nand way of updating address because we
	 * do still use the ebu nand for byte reading
	 */
	this->cmdfunc = ltq_dmanand_command_lp;
	this->init_size = ltq_pre_scan_chip_info;

	/* This is meant for ONFI chips with non-conventional
	 * oobsize which will cause the nand_scan_tail to pass
	 * Otherwise, it will complain: No oob scheme defined for oobsize
	*/
	this->ecc.layout = &B4_byte_ecc_oobinfo_2048;
	/* dummy otherwise nand scan will fail*/
	this->ecc.size = 128;
	
	err = ltq_nand_dma_setup(dma_nand_dev);
	if (err < 0) {
		pr_err("MLC NAND DMA setup failed\n");
		goto out_mtd;
	}
	
	platform_set_drvdata(pdev, dma_nand_dev);

	/* check if ONFI, initate ecc.layout to make linux kernel happy
	 * else BUG() will be called as 2.6.32 does not support nand flash
	 * with oob size > 128B
	 */

	pr_info("Probe for NAND Flash...\n");
	if (nand_scan(dma_nand_mtd, 1)) {
		pr_err("Probing for NAND flash failed, flash not found!\n");
		err = -ENXIO;
		goto out_mtd;
	}

	if (this->onfi_version) {
		pr_debug("Setting ONFI settings for MLC NAND\n");
		ltq_onfi_driver_settings(dma_nand_mtd);
	}

	/* read and write buffer MUST BE dma page aligned */
	tmp_rd_buf = kmalloc(dma_nand_mtd->writesize, GFP_KERNEL);
	if (!tmp_rd_buf) {
		pr_err("Error allocating read buffer\n");
		goto out_rd;
	}

	tmp_wr_buf = kmalloc(dma_nand_mtd->writesize, GFP_KERNEL);
	if (!tmp_wr_buf) {
		pr_err("Error allocating write buffer\n");
		goto out_rd;
	}

	this->read_buf = ltq_nand_read_buf;

	dma_nand_info_query(dma_nand_dev, dma_nand_mtd);

	pr_debug("Chip id: %08x.%08x\n", dma_nand_dev->chip_info[0],
		dma_nand_dev->chip_info[1]);

	err = nand_define_flash_bbt(dma_nand_mtd->writesize, this);

	if (err < 0)
		pr_err("There are some issues handling BBT definition\n"
				"please check your kernel configurations!\n");

	/* scan & create a bbt table where appropriate*/
	dma_nand_dev->mtd_priv = dma_nand_mtd;
	init_waitqueue_head(&dma_nand_dev->dma_nand_wait);
	this->scan_bbt(dma_nand_mtd);

	err = mtd_device_parse_register(dma_nand_mtd,
								part_probes,
								NULL,
								mtd_parts,
								n);
	if (err < 0)
		goto out_wr;

	pr_info("ecc size: %d, ecc bytes: %d, ecc total: %d\n",
		this->ecc.size, this->ecc.bytes, this->ecc.total);
	pr_info("ecc step: %d, oobsize: %d, writesize: %d\n",
		this->ecc.steps, dma_nand_mtd->oobsize,
		dma_nand_mtd->writesize);

	pr_info("Success in initializing MLC NAND\n");
	return 0;

out_wr:
	kfree(tmp_wr_buf);
out_rd:
	kfree(tmp_rd_buf);
out_mtd:
	kfree(dma_nand_mtd);
out:
	kfree(dma_nand_dev);

	return err;

}

static int ltq_dmanand_exit(struct platform_device *pdev)
{
	
	struct dma_nand_priv *mlc = platform_get_drvdata(pdev);

	nand_release(mlc->mtd_priv);

	dma_device_release(dma_device);
	dma_device_unregister(dma_device);
	kfree(mlc->mtd_priv);
	kfree(mlc);
	kfree(tmp_rd_buf);
	kfree(tmp_wr_buf);

	return 0;
}

static struct platform_driver xway_dmanand_driver = {
	.remove = ltq_dmanand_exit,
	.driver = {
		.name = "ifx_nand",
		.owner = THIS_MODULE,
		.of_match_table = of_match_ptr(xway_dmanand_match_table),
	},
};

module_platform_driver_probe(xway_dmanand_driver, ltq_dmanand_probe);
