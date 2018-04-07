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
#ifndef __REG_ADDR_H__
#define __REG_ADDR_H__

/**
 * PPE SB address
 */
#define SB_REG_BASE		0x0
#define SB_REG_END		0xFFF
#define SB_SEG0_BASE		0x2000
#define SB_SEG0_END		0x7FFF
#define SB_SEG1_BASE		0x8000
#define SB_SEG1_END		0xBFFF
#define SB_SEG2_BASE		0xF000
#define SB_SEG2_END		0xF7FF

/**
 * Note: SB4/7/8 is not real HW Memory.
 * So it's calculated SB address mapping
 * but not included in real SEG0/SEG1 size
 */

/* SB0-SB3, size in bytes */
#define SB_SEG0_SIZE		(0x4000 << 2)
/* SB6, 9, 10, 11, (6A,6B,6C,6D), size in bytes */
#define SB_SEG1_SIZE		(0x4000 << 2)

/**
 * PPE FPI address
 */
#define PPE_BASE		(0x300000)
#define PPM_CODE_MEM_BASE(x)	(PPE_BASE + (((x) * 0x10000 + 0x1000) << 2))
#define PPM_DATA_MEM_BASE(x)	(PPE_BASE + (((x) * 0x10000 + 0x4000) << 2))
#define PPM0_DATA_EXT_BASE	(PPE_BASE + ((0xF000) << 2))
#define FPI_SB_SEG_BASE(x)	(((x) << 16) + 0x8000)
#define CDM_SZ(x)		\
	(((x) == 0 || (x) == 2) ? (0x1000 << 2) : (0x800 << 2))
#define PPE_SB_ADDR(x, off)	\
	(PPE_BASE + ((FPI_SB_SEG_BASE(x) + (off)) << 2))
#define PPE_REG_ADDR(off)	(PPE_BASE + ((0x0D000 + (off)) << 2))


/* PPE REG FPI Address */
#define PPE_FREEZE			PPE_REG_ADDR(0x0)
#define FREEZE_PP32(n)			\
	(((n) == 2) ? ((n) << 23) :	\
	(((n) == 1) ? ((n) << 16) : 1))

#define PP32_RST			PPE_REG_ADDR(0x20)
#define PPE_CDM				PPE_REG_ADDR(0x0100)

/* RTHA /TTHA REG */
#define RFBI_CFG			PPE_REG_ADDR(0x0400)
#define RBA_CFG0			PPE_REG_ADDR(0x0404)
#define RBA_CFG1			PPE_REG_ADDR(0x0405)
#define RCA_CFG0			PPE_REG_ADDR(0x0408)
#define RCA_CFG1			PPE_REG_ADDR(0x0409)
#define RDES_CFG0			PPE_REG_ADDR(0x040C)
#define RDES_CFG1			PPE_REG_ADDR(0x040D)
#define SFSM_STATE0			PPE_REG_ADDR(0x0410)
#define SFSM_STATE1			PPE_REG_ADDR(0x0411)
#define SFSM_DBA0			PPE_REG_ADDR(0x0412)
#define SFSM_DBA1			PPE_REG_ADDR(0x0413)
#define SFSM_CBA0			PPE_REG_ADDR(0x0414)
#define SFSM_CBA1			PPE_REG_ADDR(0x0415)
#define SFSM_CFG0			PPE_REG_ADDR(0x0416)
#define SFSM_CFG1			PPE_REG_ADDR(0x0417)
#define SFSM_PGCNT0			PPE_REG_ADDR(0x041C)
#define SFSM_PGCNT1			PPE_REG_ADDR(0x041D)
#define FFSM_DBA0			PPE_REG_ADDR(0x0508)
#define FFSM_DBA1			PPE_REG_ADDR(0x0509)
#define FFSM_CFG0			PPE_REG_ADDR(0x050A)
#define FFSM_CFG1			PPE_REG_ADDR(0x050B)
#define FFSM_IDLE_HEAD_BC0		PPE_REG_ADDR(0x050E)
#define FFSM_IDLE_HEAD_BC1		PPE_REG_ADDR(0x050F)
#define FFSM_PGCNT0			PPE_REG_ADDR(0x0514)
#define FFSM_PGCNT1			PPE_REG_ADDR(0x0515)

#define RBA_CFG(x)			(RBA_CFG0 + ((x) << 2))
#define RCA_CFG(x)			(RCA_CFG0 + ((x) << 2))
#define RDES_CFG(x)			(RDES_CFG0 + ((x) << 2))
#define SFSM_STATE(x)			(SFSM_STATE0 + ((x) << 2))
#define SFSM_DBA(x)			(SFSM_DBA0 + ((x) << 2))
#define SFSM_CBA(x)			(SFSM_CBA0 + ((x) << 2))
#define SFSM_CFG(x)			(SFSM_CFG0 + ((x) << 2))
#define SFSM_PGCNT(x)			(SFSM_PGCNT0 + ((x) << 2))
#define FFSM_DBA(x)			(FFSM_DBA0 + ((x) << 2))
#define FFSM_CFG(x)			(FFSM_CFG0 + ((x) << 2))
#define FFSM_IDLE_HEAD_BC(x)		(FFSM_IDLE_HEAD_BC0 + ((x) << 2))
#define FFSM_PGCNT(x)			(FFSM_PGCNT0 + ((x) << 2))

/* SAR REG */
#define SAR_MODE_CFG			PPE_REG_ADDR(0x080A)
#define SAR_RX_CMDCNT			PPE_REG_ADDR(0x080B)
#define SAR_TX_CMDCNT			PPE_REG_ADDR(0x080C)
#define SAR_RX_CTX_CFG			PPE_REG_ADDR(0x080D)
#define SAR_TX_CTX_CFG			PPE_REG_ADDR(0x080E)
#define SAR_TX_CMD_DONE_CNT		PPE_REG_ADDR(0x080F)
#define SAR_ISR				PPE_REG_ADDR(0x0810)
#define SAR_IER				PPE_REG_ADDR(0x0811)
#define SAR_POLY_CFG_SET0		PPE_REG_ADDR(0x0812)
#define SAR_POLY_CFG_SET1		PPE_REG_ADDR(0x0813)
#define SAR_POLY_CFG_SET2		PPE_REG_ADDR(0x0814)
#define SAR_POLY_CFG_SET3		PPE_REG_ADDR(0x0815)
#define SAR_CRC_SIZE_CFG		PPE_REG_ADDR(0x0816)

/* PDMA/EMA REG */
#define PDMA_CFG			PPE_REG_ADDR(0x0A00)
#define PDMA_RX_CMDCNT			PPE_REG_ADDR(0x0A01)
#define PDMA_TX_CMDCNT			PPE_REG_ADDR(0x0A02)
#define PDMA_RX_FWDATACNT		PPE_REG_ADDR(0x0A03)
#define PDMA_TX_FWDATACNT		PPE_REG_ADDR(0x0A04)
#define PDMA_RX_CTX_CFG			PPE_REG_ADDR(0x0A05)
#define PDMA_TX_CTX_CFG			PPE_REG_ADDR(0x0A06)
#define PDMA_RX_MAX_LEN_REG		PPE_REG_ADDR(0x0A07)
#define PDMA_RX_DELAY_CFG		PPE_REG_ADDR(0x0A08)
#define PDMA_INT_FIFO_RD		PPE_REG_ADDR(0x0A09)
#define PDMA_ISR			PPE_REG_ADDR(0x0A0A)
#define PDMA_IER			PPE_REG_ADDR(0x0A0B)
#define PDMA_SUBID			PPE_REG_ADDR(0x0A0C)
#define PDMA_BAR0			PPE_REG_ADDR(0x0A0D)
#define PDMA_BAR1			PPE_REG_ADDR(0x0A0E)
#define EMA_CMDCFG			PPE_REG_ADDR(0x0F00)
#define EMA_DATACFG			PPE_REG_ADDR(0x0F02)
#define EMA_CMDCNT			PPE_REG_ADDR(0x0A02)
#define EMA_RX_DATACNT			PPE_REG_ADDR(0x0A03)
#define EMA_TX_DATACNT			PPE_REG_ADDR(0x0A04)
#define EMA_IER				PPE_REG_ADDR(0x0A05)
#define EMA_CFG				PPE_REG_ADDR(0x0A06)
#define EMA_SUBID			PPE_REG_ADDR(0x0A07)

/* PPE TC Logic REG (partial) */
#define DREG_A_VERSION			PPE_REG_ADDR(0x0D00)
#define DREG_A_CFG			PPE_REG_ADDR(0x0D01)
#define DREG_AT_CTRL			PPE_REG_ADDR(0x0D02)
#define DREG_AT_CB_CFG0			PPE_REG_ADDR(0x0D03)
#define DREG_AT_CB_CFG1			PPE_REG_ADDR(0x0D04)
#define DREG_AR_CTRL			PPE_REG_ADDR(0x0D08)
#define DREG_AR_CB_CFG0			PPE_REG_ADDR(0x0D09)
#define DREG_AR_CB_CFG1			PPE_REG_ADDR(0x0D0A)
#define DREG_A_UTPCFG			PPE_REG_ADDR(0x0D0E)
#define DREG_A_STATUS			PPE_REG_ADDR(0x0D0F)
#define DREG_AT_CFG0			PPE_REG_ADDR(0x0D20)
#define DREG_AT_CFG1			PPE_REG_ADDR(0x0D21)
#define DREG_AT_FB_SIZE0		PPE_REG_ADDR(0x0D22)
#define DREG_AT_FB_SIZE1		PPE_REG_ADDR(0x0D23)
#define DREG_AT_CELL0			PPE_REG_ADDR(0x0D24)
#define DREG_AT_CELL1			PPE_REG_ADDR(0x0D25)
#define DREG_AT_IDLE_CNT0		PPE_REG_ADDR(0x0D26)
#define DREG_AT_IDLE_CNT1		PPE_REG_ADDR(0x0D27)
#define DREG_AT_IDLE0			PPE_REG_ADDR(0x0D28)
#define DREG_AT_IDLE1			PPE_REG_ADDR(0x0D29)
#define DREG_AR_CFG0			PPE_REG_ADDR(0x0D60)
#define DREG_AR_CFG1			PPE_REG_ADDR(0x0D61)
#define DREG_AR_CELL0			PPE_REG_ADDR(0x0D68)
#define DREG_AR_CELL1			PPE_REG_ADDR(0x0D69)
#define DREG_AR_IDLE_CNT0		PPE_REG_ADDR(0x0D6A)
#define DREG_AR_IDLE_CNT1		PPE_REG_ADDR(0x0D6B)
#define DREG_AR_AIIDLE_CNT0		PPE_REG_ADDR(0x0D6C)
#define DREG_AR_AIIDLE_CNT1		PPE_REG_ADDR(0x0D6D)
#define DREG_AR_BE_CNT0			PPE_REG_ADDR(0x0D6E)
#define DREG_AR_BE_CNT1			PPE_REG_ADDR(0x0D6F)
#define DREG_AR_HEC_CNT0		PPE_REG_ADDR(0x0D70)
#define DREG_AR_HEC_CNT1		PPE_REG_ADDR(0x0D71)
#define DREG_AR_IDLE0			PPE_REG_ADDR(0x0D74)
#define DREG_AR_IDLE1			PPE_REG_ADDR(0x0D75)
#define DREG_AR_OVDROP_CNT0		PPE_REG_ADDR(0x0D98)
#define DREG_AR_OVDROP_CNT1		PPE_REG_ADDR(0x0D99)
#define DREG_AR_CERRN_CNT0		PPE_REG_ADDR(0x0DA0)
#define DREG_AR_CERRN_CNT1		PPE_REG_ADDR(0x0DA1)
#define DREG_AR_CERRNP_CNT0		PPE_REG_ADDR(0x0DA2)
#define DREG_AR_CERRNP_CNT1		PPE_REG_ADDR(0x0DA3)
#define DREG_AR_CVN_CNT0		PPE_REG_ADDR(0x0DA4)
#define DREG_AR_CVN_CNT1		PPE_REG_ADDR(0x0DA5)
#define DREG_AR_CVNP_CNT0		PPE_REG_ADDR(0x0DA6)
#define DREG_AR_CVNP_CNT1		PPE_REG_ADDR(0x0DA7)
#define DREG_B0_LADR			PPE_REG_ADDR(0x0DA8)
#define DREG_B1_LADR			PPE_REG_ADDR(0x0DA9)
#define BC0_LINK_STATUS			PPE_REG_ADDR(0x0DC0)
#define PDBRAM_OWN_STATUS		PPE_REG_ADDR(0x0DC2)
#define DSL_TC_INDICATE			PPE_REG_ADDR(0x0DC4)
#define PPE_BC0_CW_SIZE			PPE_REG_ADDR(0x0DC5)
#define DSL_FRAMER_REQ_START		PPE_REG_ADDR(0x0DC6)
#define DSL_FRAMER_REQ_STOP		PPE_REG_ADDR(0x0DC7)
#define ERB_DESC_BASE			PPE_REG_ADDR(0x0DC8)
#define ERB_DESC_NUM			PPE_REG_ADDR(0x0DC9)

#define DREG_AT_CB_CFG(x)		(DREG_AT_CB_CFG0 + ((x) << 2))
#define DREG_AR_CB_CFG(x)		(DREG_AR_CB_CFG0 + ((x) << 2))
#define DREG_AT_CFG(x)			(DREG_AT_CFG0 + ((x) << 2))
#define DREG_AT_FB_SIZE(x)		(DREG_AT_FB_SIZE0 + ((x) << 2))
#define DREG_AT_CELL(x)			(DREG_AT_CELL0 + ((x) << 2))
#define DREG_AT_IDLE_CNT(x)		(DREG_AT_IDLE_CNT0 + ((x) << 2))
#define DREG_AT_IDLE(x)			(DREG_AT_IDLE0 + ((x) << 2))
#define DREG_AR_CFG(x)			(DREG_AR_CFG0 + ((x) << 2))
#define DREG_AR_CELL(x)			(DREG_AR_CELL0 + ((x) << 2))
#define DREG_AR_IDLE_CNT(x)		(DREG_AR_IDLE_CNT0 + ((x) << 2))
#define DREG_AR_AIIDLE_CNT(x)		(DREG_AR_AIIDLE_CNT0 + ((x) << 2))
#define DREG_AR_BE_CNT(x)		(DREG_AR_BE_CNT0 + ((x) << 2))
#define DREG_AR_HEC_CNT(x)		(DREG_AR_HEC_CNT0 + ((x) << 2))
#define DREG_AR_IDLE(x)			(DREG_AR_IDLE0 + ((x) << 2))
#define DREG_AR_OVDROP_CNT(x)		(DREG_AR_OVDROP_CNT0 + ((x) << 2))
#define DREG_AR_CERRN_CNT(x)		(DREG_AR_CERRN_CNT0 + ((x) << 2))
#define DREG_AR_CVN_CNT(x)		(DREG_AR_CVN_CNT0 + ((x) << 2))
#define DREG_AR_CVNP_CNT(x)		(DREG_AR_CVNP_CNT0 + ((x) << 2))
#define DREG_B_LADR(x)			(DREG_B0_LADR + ((x) << 2))

/* SAR_PDMA REG*/
#define SAR_PDMA_RX_CMDBUF_CFG		PPE_REG_ADDR(0x0F00)
#define SAR_PDMA_TX_CMDBUF_CFG		PPE_REG_ADDR(0x0F01)
#define SAR_PDMA_RX_FW_CMDBUF_CFG	PPE_REG_ADDR(0x0F02)
#define SAR_PDMA_TX_FW_CMDBUF_CFG	PPE_REG_ADDR(0x0F03)
#define SAR_PDMA_RX_CMDBUF_STATUS	PPE_REG_ADDR(0x0F04)
#define SAR_PDMA_TX_CMDBUF_STATUS	PPE_REG_ADDR(0x0F05)

#define GIF0_RX_CRC_ERR_CNT		DREG_AR_CERRN_CNT0
#define GIF1_RX_CRC_ERR_CNT		DREG_AR_CERRNP_CNT0
#define GIF2_RX_CRC_ERR_CNT		DREG_AR_CERRN_CNT1
#define GIF3_RX_CRC_ERR_CNT		DREG_AR_CERRNP_CNT1
#define GIF0_RX_CV_CNT			DREG_AR_CVN_CNT0
#define GIF1_RX_CV_CNT			DREG_AR_CVNP_CNT0
#define GIF2_RX_CV_CNT			DREG_AR_CVN_CNT1
#define GIF3_RX_CV_CNT			DREG_AR_CVNP_CNT1
#define B0_OVERDROP_CNT			DREG_AR_OVDROP_CNT0
#define B1_OVERDROP_CNT			DREG_AR_OVDROP_CNT1

/* QSB REG */
#define QSB_BASE			0x338000
#define QOS_REG(off)			(QSB_BASE + (off))
#define QSB_ICDV			QOS_REG(0x1C)
#define QSB_SBL				QOS_REG(0x24)
#define QSB_CFG				QOS_REG(0x28)
#define QSB_RTM				QOS_REG(0x2C)
#define QSB_RTD				QOS_REG(0x30)
#define QSB_RAMAC			QOS_REG(0x34)
#define QSB_ISTAT			QOS_REG(0x38)
#define QSB_IMR				QOS_REG(0x3C)
#define QSB_SRC				QOS_REG(0x40)

/* SB FPI address on xBar SSX1 */
#define SB_XBAR_BASE			0x280000
#define SB_XBAR_ADDR(x)			\
	(SB_XBAR_BASE + (((x) - 0xA000) << 2))

/* PDBRAM FPI address */
#define PDBRAM_BASE			0x200000
#define PDBRAM_PPE_BASE			(PDBRAM_BASE + 0x18000)
#define PDBRAM_PPE_SIZE			(0x20000) /* Size in Bytes */

/* Mailbox interrupt address */
#define MBOX_BASE			0x334800
#define MBOX_OFF(id)			\
	(((id) < 2) ? ((id) << 4) : (0x40 + (((id) - 2) << 4)))
#define MBOX_IGU_ISRS(id)		(MBOX_BASE + MBOX_OFF(id))
#define MBOX_IGU_ISRC(id)		(MBOX_BASE + MBOX_OFF(id) + 0x4)
#define MBOX_IGU_ISR(id)		(MBOX_BASE + MBOX_OFF(id) + 0x8)
#define MBOX_IGU_IER(id)		(MBOX_BASE + MBOX_OFF(id) + 0xC)
#define MBOX_MASK(id)			((id == 2) ? 0xFF : 0xFFFFFFFF)

/* Mailbox interrupt bit */
#define MBOX_PKT_RX			BIT(0)
#define MBOX_OAM_RX			BIT(1)
#define MBOX_P2P_STAT			BIT(4)
#define MBOX_FRAME_STOP			BIT(6)
#define MBOX_INT_ALL			\
	(MBOX_PKT_RX | MBOX_OAM_RX | MBOX_P2P_STAT | MBOX_FRAME_STOP)

/* Mailbox interrupt to PPE FW */
#define MBOX_FW_WAKEUP			BIT(0)
#define PPE_FW_MBOX0			MBOX_IGU5

/**
 * PP32 Debug Register and Macro
 */
#define LSH(n, bits)			((n) << (bits))
#define LLSH(a, b, bits)		(LSH(a, LSH(b, bits)))
#define PP32_DBG_REG(n, off)		(PPE_BASE + (((off) + LSH(n, 16)) << 2))

#define PP32_CTRL_CMD(n)		PP32_DBG_REG(n, 0x0B00)
#define CTRL_CMD_RESTART		BIT(0)
#define CTRL_CMD_STOP			BIT(1)
#define CTRL_CMD_STEP			BIT(2)
#define CTRL_CMD_BREAKOUT		BIT(3)

#define PP32_CTRL_OPT(n)		PP32_DBG_REG(n, 0x0C00)
#define BREAKOUT_ON_STOP_ON		(3 << 0)
#define BREAKOUT_ON_STOP_OFF		(2 << 0)
#define BREAKOUT_ON_BREAKIN_ON		(3 << 2)
#define BREAKOUT_ON_BREAKIN_OFF		(2 << 2)
#define STOP_ON_BREAKIN_ON		(3 << 4)
#define STOP_ON_BREAKIN_OFF		(2 << 4)
#define STOP_ON_BREAKPOINT_ON		(3 << 6)
#define STOP_ON_BREAKPOINT_OFF		(2 << 6)
#define BREAKOUT_ON_STOP(n)		(tc_r32(PP32_CTRL_OPT(n)) & BIT(0))
#define BREAKOUT_ON_BREAKIN(n)		(tc_r32(PP32_CTRL_OPT(n)) & BIT(2))
#define STOP_ON_BREAKIN(n)		(tc_r32(PP32_CTRL_OPT(n)) & BIT(4))
#define STOP_ON_BREAKPOINT(n)		(tc_r32(PP32_CTRL_OPT(n)) & BIT(6))

#define BRK_PC(n, pc)			(PP32_DBG_REG(n, 0x0900 + LSH(pc, 1)))
#define BRK_PC_MASK(n, pc)		(PP32_DBG_REG(n, 0x0901 + LSH(pc, 1)))
#define BRK_DATA_ADDR(n, pc)		(PP32_DBG_REG(n, 0x0904 + LSH(pc, 1)))
#define BRK_DATA_ADDR_MASK(n, pc)	(PP32_DBG_REG(n, 0x0905 + LSH(pc, 1)))
#define BRK_DATA_VAL_RD(n, pc)		(PP32_DBG_REG(n, 0x0908 + LSH(pc, 1)))
#define BRK_DATA_VAL_RD_MASK(n, pc)	(PP32_DBG_REG(n, 0x0909 + LSH(pc, 1)))
#define BRK_DATA_VAL_WR(n, pc)		(PP32_DBG_REG(n, 0x090C + LSH(pc, 1)))
#define BRK_DATA_VAL_WR_MASK(n, pc)	(PP32_DBG_REG(n, 0x090D + LSH(pc, 1)))
#define BRK_CONTEXT_MASK		(0xF)
#define BRK_CONTEXT_MASK_EN		(BIT(4))
/* valid for break data value rd/wr only */
#define BRK_COMPARE_GREATER_EQUAL	(BIT(5))
#define BRK_COMPARE_LOWER_EQUAL		(BIT(6))
#define BRK_COMPARE_EN			(BIT(7))

#define BRK_TRIG(n)			PP32_DBG_REG(n, 0x0F00)
#define BRK_GRP_PC_ON(i, n)		(LLSH(LLSH(3, n, 1), i, 4))
#define BRK_GRP_PC_OFF(i, n)		(LLSH(LLSH(2, n, 1), i, 4))
#define BRK_GRP_DATA_ADDR_ON(i, n)	(LLSH(LSH(3, LSH(n, 1) + 4), i, 4))
#define BRK_GRP_DATA_ADDR_OFF(i, n)	(LLSH(LSH(2, LSH(n, 1) + 4), i, 4))
#define BRK_GRP_DATA_VAL_RD_ON(i, n)	(LLSH(LSH(3, LSH(n, 1) + 8), i, 4))
#define BRK_GRP_DATA_VAL_RD_OFF(i, n)	(LLSH(LSH(2, LSH(n, 1) + 8), i, 4))
#define BRK_GRP_DATA_VAL_WR_ON(i, n)	(LLSH(LSH(3, LSH(n, 1) + 12), i, 4))
#define BRK_GRP_DATA_VAL_WR_OFF(i, n)	(LLSH(LSH(2, LSH(n, 1) + 12), i, 4))
#define TRIG_VAL(k)			(tc_r32(BRK_TRIG(k)))
#define BRK_GRP_PC(k, i, n)		(TRIG_VAL(k) & LLSH(BIT(n), i, 3))
#define BRK_GRP_DATA_ADDR(k, i, n)	(TRIG_VAL(k) & LLSH(BIT((n) + 2), i, 3))
#define BRK_GRP_DATA_VAL_RD(k, i, n)	(TRIG_VAL(k) & LLSH(BIT((n) + 4), i, 3))
#define BRK_GRP_DATA_VAL_WR(k, i, n)	(TRIG_VAL(k) & LLSH(BIT((n) + 6), i, 3))

#define PP32_CPU_STATUS(n)		PP32_DBG_REG(n, 0x0D00)
#define PP32_HALT_STAT(n)		PP32_CPU_STATUS(n)
#define PP32_DBG_CUR_PC(n)		PP32_CPU_STATUS(n)
#define PP32_STATUS(n)			(tc_r32(PP32_CPU_STATUS(n)))
#define USER_STOPPED(n)			(PP32_STATUS(n) & BIT(0))
#define USER_BREAKIN_RCV(n)		(PP32_STATUS(n) & BIT(1))
#define USER_BREAKPOINT_MET(n)		(PP32_STATUS(n) & BIT(2))
#define CUR_PC(n)			(PP32_STATUS(n) >> 16)

#define BREAKPOINT_REASONS(n)		PP32_DBG_REG(n, 0x0A00)
#define BRKPT_RSNS(n)			(tc_r32(BREAKPOINT_REASONS(n)))
#define BRK_PC_MET(n, i)		(BRKPT_RSNS(n) & (1 << (i)))
#define BRK_DATA_ADDR_MET(n, i)		(BRKPT_RSNS(n) & (1 << ((i) + 2)))
#define BRK_DATA_VAL_RD_MET(n, i)	(BRKPT_RSNS(n) & (1 << ((i) + 4)))
#define BRK_DATA_VAL_WR_MET(n, i)	(BRKPT_RSNS(n) & (1 << ((i) + 6)))
#define BRK_DATA_VAL_RD_LO_EQ(n, i)	(BRKPT_RSNS(n) & (1 << ((i) * 2 + 8)))
#define BRK_DATA_VAL_RD_GT_EQ(n, i)	(BRKPT_RSNS(n) & (1 << ((i) * 2 + 9)))
#define BRK_DATA_VAL_WR_LO_EQ(n, i)	(BRKPT_RSNS(n) & (1 << ((i) * 2 + 12)))
#define BRK_DATA_VAL_WR_GT_EQ(n, i)	(BRKPT_RSNS(n) & (1 << ((i) * 2 + 13)))
#define BRK_CUR_CONTEXT(n)		((PP32_STATUS(n) >> 8) & 0x03)

#define GP_REG_BASE(n)			PP32_DBG_REG(n, 0x0E00)
#define GP_CONTEXT_REG(n, i, j)		\
	tc_r32(PP32_DBG_REG(n, 0x0E00 + (i) * 16 + (j)))

/* PP32 RSR BIT */
#define TTHA_RST		BIT(0)
#define RTHA_RST		BIT(1)
#define DREG_RST		BIT(2)
#define SLL0_RST		BIT(3)
#define TCDMA0_RST		BIT(4)
#define SB_RST			BIT(5)
#define SAR_RST			BIT(6)
#define SLL1_RST		BIT(7)
#define PDMA_RST		BIT(8)
#define QSB_RST			BIT(9)
#define DMAL_RST		BIT(10)
#define PPM0_RST		BIT(16)
#define PPM1_RST		BIT(17)
#define PPM2_RST		BIT(18)
#define PP32_RST_ALL		(TTHA_RST | RTHA_RST | DREG_RST | SLL0_RST\
				TCDMA0_RST | SB_RST | SAR_RST | SLL1_RST |\
				PDMA_RST | QSB_RST | DMAL_RST | PPM0_RST |\
				PPM1_RST | PPM2_RST)

/* CDMA REG */
#define CDMA_BASE		0x2D0000
#define CDMA_REG(off)		(CDMA_BASE + (off))
#define CDMA_CLC		CDMA_REG(0x0)
#define CDMA_ID			CDMA_REG(0x8)
#define CDMA_CTRL		CDMA_REG(0x10)
#define CDMA_CPOLL		CDMA_REG(0x14)
#define CDMA_CS			CDMA_REG(0x18)
#define CDMA_CCTRL		CDMA_REG(0x1C)
#define CDMA_CDBA		CDMA_REG(0x20)
#define CDMA_CDLEN		CDMA_REG(0x24)
#define CDMA_CIS		CDMA_REG(0x28)
#define CDMA_CIE		CDMA_REG(0x2C)
#define CDMA_CDPTNRD		CDMA_REG(0x34)
#define CDMA_PS			CDMA_REG(0x40)
#define CDMA_PCTRL		CDMA_REG(0x44)
#define CDMA_IRNEN		CDMA_REG(0xF4)
#define CDMA_IRNCR		CDMA_REG(0xF8)
#define CDMA_IRNICR		CDMA_REG(0xFC)

/* DMA CCTRL BIT */
#define CCTRL_RST_S		1
#define CCTRL_ONOFF_S		0

/* DMA CTRL BIT */
#define CTRL_PKTARB_S		31	/* Packet Arbitration */
#define CTRL_MDC_S		15	/* Meta data copy */
#define CTRL_DDBR_S		14	/* Dynamic Burst */
#define CTRL_DCNF_S		13	/* Descriptor Length CFG*/
#define CTRL_ENBE_S		9	/* Byte Enable */
#define CTRL_DRB_S		8	/* Descriptor Read back */
#define CTRL_DSRAM_S		1	/* Dedicated Desc access port */
#define CTRL_RST_S		0	/* Global Reset */

/* DMA PORT BIT */
#define PCTRL_GPC_S		16	/* FLUSH MEMORY PORT */
#define PCTRL_TXENDI_S		10	/* TX DIR Endianess */
#define PCTRL_RXENDI_S		8	/* RX DIR Endianess */
#define PCTRL_TXBL_S		4	/* TX burst 2/4/8 */
#define PCTRL_RXBL_S		2	/* RX burst 2/4/8 */
#define PCTRL_TXBL16_S		1	/* TX burst of 16 */
#define PCTRL_RXBL16_S		0	/* RX burst of 16 */

/*DMA ID BIT */
#define ID_CHNR			0x3F
#define ID_CHNR_S		20	/* Channel Number */

/*DMA POLLING */
#define POLL_EN			1
#define POLL_EN_S		31	/* Polling Enable */
#define POLL_CNT		0xFFF
#define POLL_CNT_S		4	/* Polling Counter */
#define POLL_DEF_CNT		24

#define CDMA_CI_EOP		BIT(1)
#define CDMA_CI_DUR		BIT(2)
#define CDMA_CI_DESCPT		BIT(3)
#define CDMA_CI_CHOFF		BIT(4)
#define CDMA_CI_RDERR		BIT(5)
#define CDMA_CI_ALL		(CDMA_CI_EOP | CDMA_CI_DUR | CDMA_CI_DESCPT\
				| CDMA_CI_CHOFF | CDMA_CI_RDERR)

/*CGU REG BIT */
#define PMU_PPE_ALL		(PMU_CDMA | PMU_PPE_QSB | PMU_PPE_SLL01 | \
				PMU_PPE_TC | PMU_EMA | PMU_PPM2 | PMU_PPE_TOP)

#define PMU_PTM			(PMU_CDMA | PMU_PPE_SLL01 | PMU_PPE_TC | \
				PMU_EMA | PMU_PPM2 | PMU_PPE_TOP)

#define PMU_ATM			(PMU_CDMA | PMU_PPE_QSB | PMU_PPE_SLL01 | \
				 PMU_PPE_TC | PMU_EMA | PMU_PPE_TOP)

/* RCU RED BIT */
#define RST_PPE_ALL		(RST_DSL_IF | RST_PPE | RST_PPE_ATM_TC)

#define mem(p)			(((struct tc_comm *)p)->ep->membase)
#define tc_r32(reg)		(readl(mem(priv) + (reg)))
#define tc_w32(val, reg)	(writel(val, mem(priv) + (reg)))
#define tc_w32_flush(val, reg)		\
do {					\
	tc_w32(val, reg);		\
	tc_r32(reg);			\
} while (0)

#define MS(_v, _m)		((_v) >> (_m##_S) & (_m))
#define SM(_v, _m)		(((_v) & (_m)) << (_m##_S))
#define CLR(v, m, b)		((v) & (~((m) << (b))))
#define SET(s, m, b)		(((s) & (m)) << (b))

#define set_mask_bit(val, set, mask, bits)			\
	(val = (CLR(val, mask, bits) | SET(set, mask, bits)))

static inline void rw32_mask(void *priv, u32 set,
			u32 mask, u32 bits, u32 reg_addr)
{
	u32 val;

	val = tc_r32(reg_addr);
	tc_w32(set_mask_bit(val, set, mask, bits), reg_addr);
}

#define TO_DWSZ(a)	((a) >> 2)
#define TO_BYSZ(a)	((a) << 2)

/**
 * API to convert SB address to FPI address
 * input:
 *	sb_addr:  SB address
 *
 * return:
 *	Success:  FPI address
 *	Fail:  0
 */
static inline u32 fpi_addr(u32 sb_addr)
{
	u32 fpi_addr;
	if (sb_addr <= SB_REG_END)
		fpi_addr = PPE_REG_ADDR(sb_addr);
	else if (sb_addr >= SB_SEG0_BASE && sb_addr <= SB_SEG0_END)
		fpi_addr = PPE_SB_ADDR(0, (sb_addr - SB_SEG0_BASE));
	else if ((sb_addr >= SB_SEG1_BASE && sb_addr <= SB_SEG1_END)
		|| (sb_addr >= SB_SEG2_BASE && sb_addr <= SB_SEG2_END))
		fpi_addr = PPE_SB_ADDR(1, (sb_addr - SB_SEG1_BASE));
	else
		fpi_addr = sb_addr;

	return fpi_addr;
}

/* API to write DWORD to PPE SB */
#define sb_w32(val, addr)	(tc_w32((val), fpi_addr(addr)))
/* API to read DWORD from PPE SB */
#define sb_r32(addr)		(tc_r32(fpi_addr(addr)))

enum {
	PP32_TX = 0,
	PP32_BOND = 1,
	PP32_RX = 2,
	PP32_MAX
};

enum {
	NO_SWAP		= 0,	/* B0_B1_B2_B3 */
	BYTE_SWAP	= 1,	/* B1_B0_B3_B2 */
	WORD_SWAP	= 2,	/* B2_B3_B0_B1 */
	BYTE_WORD_SWAP	= 3,	/* B3_B2_B1_B0 */
};


enum cdma_onoff {
	DMA_CH_OFF	= 0,
	DMA_CH_ON	= 1,
};


#endif /*__REG_ADDR_H__ */

