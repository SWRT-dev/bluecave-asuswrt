/******************************************************************************
 *
 *                        Copyright (c) 2012, 2014, 2015
 *                           Lantiq Deutschland GmbH
 *
 *  For licensing information, see the file 'LICENSE' in the root folder of
 *  this software module.
 *
 ******************************************************************************/
#ifndef __VRX218_COMMON_H__
#define __VRX218_COMMON_H__

#include <lantiq.h>

/**
 * ####################################
 *              Definition
 * ####################################
 */
/* 8M window space */
#define VRX318_WINDOW_SPACE		0x00800000
#define VRX318_SMASK			(VRX318_WINDOW_SPACE - 1)
#define vrx318_r32(reg, base)		ltq_r32((void *)(((reg) & VRX318_SMASK) + (base)))
#define vrx318_w32(val, reg, base)	ltq_w32(val, (void *)(((reg) & VRX318_SMASK) + (base)))
#define vrx318_w32_sb(val, reg, base)	vrx318_w32(val, SB_BUFFER(reg), base)
#define vrx318_r32_sb(reg, base)	vrx318_r32(SB_BUFFER(reg), base)

/**
 *  Address Mapping
 *  VRX318 FPI Configuration Bus Register and Memory Address Mapping
 */
#define PPE_BASE			0x1E200000
#define PP32_DEBUG_REG_ADDR(i, x)	(PPE_BASE + (((x) + 0x000000 + (i) * 0x00010000) << 2))
#define CDM_CODE_MEMORY(i, x)		(PPE_BASE + (((x) + 0x001000 + (i) * 0x00010000) << 2))
#define CDM_DATA_MEMORY(i, x)		(PPE_BASE + (((x) + 0x004000 + (i) * 0x00010000) << 2))
#define SB_RAM0_ADDR(x)			(PPE_BASE + (((x) + 0x008000) << 2))
#define SB_RAM1_ADDR(x)			(PPE_BASE + (((x) + 0x009000) << 2))
#define SB_RAM2_ADDR(x)			(PPE_BASE + (((x) + 0x00A000) << 2))
#define SB_RAM3_ADDR(x)			(PPE_BASE + (((x) + 0x00B000) << 2))
#define SB_RAM4_ADDR(x)			(PPE_BASE + (((x) + 0x00C000) << 2))
#define PPE_REG_ADDR(x)			(PPE_BASE + (((x) + 0x00D000) << 2))
#define QSB_CONF_REG_ADDR(x)		(PPE_BASE + (((x) + 0x00E000) << 2))
#define SB_RAM6_ADDR(x)			(PPE_BASE + (((x) + 0x018000) << 2))

/*  DWORD-Length of Memory Blocks   */
#define PP32_DEBUG_REG_DWLEN		0x0030
#define CDM_CODE_MEMORYN_DWLEN(n)	((n) == 0 ? 0x1000 : 0x0800)
#define CDM_DATA_MEMORY_DWLEN		CDM_CODE_MEMORYN_DWLEN(1)
#define SB_RAM0_DWLEN			0x1000
#define SB_RAM1_DWLEN			0x1000
#define SB_RAM2_DWLEN			0x1000
#define SB_RAM3_DWLEN			0x1000
#define SB_RAM4_DWLEN			0x1000
#define SB_RAM6_DWLEN			0x8000
#define QSB_CONF_REG_DWLEN		0x0100

#define SB_BUFFER(__sb_addr)		((((__sb_addr) >= 0x0000) && ((__sb_addr) <= 0x1FFF)) ? PPE_REG_ADDR((__sb_addr)) :         \
					(((__sb_addr) >= 0x2000) && ((__sb_addr) <= 0x2FFF)) ? SB_RAM0_ADDR((__sb_addr) - 0x2000) : \
					(((__sb_addr) >= 0x3000) && ((__sb_addr) <= 0x3FFF)) ? SB_RAM1_ADDR((__sb_addr) - 0x3000) : \
					(((__sb_addr) >= 0x4000) && ((__sb_addr) <= 0x4FFF)) ? SB_RAM2_ADDR((__sb_addr) - 0x4000) : \
					(((__sb_addr) >= 0x5000) && ((__sb_addr) <= 0x5FFF)) ? SB_RAM3_ADDR((__sb_addr) - 0x5000) : \
					(((__sb_addr) >= 0x6000) && ((__sb_addr) <= 0x6FFF)) ? SB_RAM4_ADDR((__sb_addr) - 0x6000) : \
					(((__sb_addr) >= 0x7000) && ((__sb_addr) <= 0x7FFF)) ? PPE_REG_ADDR((__sb_addr) - 0x7000) : \
					(((__sb_addr) >= 0x8000) && ((__sb_addr) <= 0xFFFF)) ? SB_RAM6_ADDR((__sb_addr) - 0x8000) : \
					__sb_addr)

#define PP32_REG_ADDR_BEGIN		0x0
#define PP32_REG_ADDR_END		0x1FFF
#define PP32_SB_ADDR_BEGIN		0x2000
#define PP32_SB_ADDR_END		0xFFFF


#define VRX318_OB_BADDR			0x20000000
#define FW_BASE_ADDR_OFFSET		0x06000
#define FPI_ADDR_MASK			0x3FFFFFFF

/* VRX318 PPE REG */
#define PP32_FREEZE			PPE_REG_ADDR(0x0000)
#define PP32_SRST			PPE_REG_ADDR(0x0020)
#define CDM_CFG				PPE_REG_ADDR(0x0100)

/* RTHA /TTHA REG */
#define PPE_RFBI_CFG			PPE_REG_ADDR(0x0400)
#define PPE_RBA_CFG0			PPE_REG_ADDR(0x0404)
#define PPE_RBA_CFG1			PPE_REG_ADDR(0x0405)
#define PPE_RCA_CFG0			PPE_REG_ADDR(0x0408)
#define PPE_RCA_CFG1			PPE_REG_ADDR(0x0409)
#define PPE_RDES_CFG0			PPE_REG_ADDR(0x040C)
#define PPE_RDES_CFG1			PPE_REG_ADDR(0x040D)
#define PPE_SFSM_STATE0			PPE_REG_ADDR(0x0410)
#define PPE_SFSM_STATE1			PPE_REG_ADDR(0x0411)
#define PPE_SFSM_DBA0			PPE_REG_ADDR(0x0412)
#define PPE_SFSM_DBA1			PPE_REG_ADDR(0x0413)
#define PPE_SFSM_CBA0			PPE_REG_ADDR(0x0414)
#define PPE_SFSM_CBA1			PPE_REG_ADDR(0x0415)
#define PPE_SFSM_CFG0			PPE_REG_ADDR(0x0416)
#define PPE_SFSM_CFG1			PPE_REG_ADDR(0x0417)
#define PPE_SFSM_PGCNT0			PPE_REG_ADDR(0x041C)
#define PPE_SFSM_PGCNT1			PPE_REG_ADDR(0x041D)
#define PPE_FFSM_DBA0			PPE_REG_ADDR(0x0508)
#define PPE_FFSM_DBA1			PPE_REG_ADDR(0x0509)
#define PPE_FFSM_CFG0			PPE_REG_ADDR(0x050A)
#define PPE_FFSM_CFG1			PPE_REG_ADDR(0x050B)
#define PPE_FFSM_IDLE_HEAD_BC0		PPE_REG_ADDR(0x050E)
#define PPE_FFSM_IDLE_HEAD_BC1		PPE_REG_ADDR(0x050F)
#define PPE_FFSM_PGCNT0			PPE_REG_ADDR(0x0514)
#define PPE_FFSM_PGCNT1			PPE_REG_ADDR(0x0515)

/* SAR REG */
#define PPE_SAR_MODE_CFG		PPE_REG_ADDR(0x080A)
#define PPE_SAR_RX_CMDCNT		PPE_REG_ADDR(0x080B)
#define PPE_SAR_TX_CMDCNT		PPE_REG_ADDR(0x080C)
#define PPE_SAR_RX_CTX_CFG		PPE_REG_ADDR(0x080D)
#define PPE_SAR_TX_CTX_CFG		PPE_REG_ADDR(0x080E)
#define PPE_SAR_POLY_CFG_SET0		PPE_REG_ADDR(0x0812)
#define PPE_SAR_POLY_CFG_SET1		PPE_REG_ADDR(0x0813)
#define PPE_SAR_POLY_CFG_SET2		PPE_REG_ADDR(0x0814)
#define PPE_SAR_POLY_CFG_SET3		PPE_REG_ADDR(0x0815)
#define PPE_SAR_CRC_SIZE_CFG		PPE_REG_ADDR(0x0816)

/* PDMA/EMA REG */
#define PPE_PDMA_CFG			PPE_REG_ADDR(0x0A00)
#define PPE_PDMA_RX_CMDCNT		PPE_REG_ADDR(0x0A01)
#define PPE_PDMA_TX_CMDCNT		PPE_REG_ADDR(0x0A02)
#define PPE_PDMA_RX_FWDATACNT		PPE_REG_ADDR(0x0A03)
#define PPE_PDMA_TX_FWDATACNT		PPE_REG_ADDR(0x0A04)
#define PPE_PDMA_RX_CTX_CFG		PPE_REG_ADDR(0x0A05)
#define PPE_PDMA_TX_CTX_CFG		PPE_REG_ADDR(0x0A06)
#define PPE_PDMA_RX_MAX_LEN_REG		PPE_REG_ADDR(0x0A07)
#define PPE_PDMA_RX_DELAY_CFG		PPE_REG_ADDR(0x0A08)
#define PPE_PDMA_INT_FIFO_RD		PPE_REG_ADDR(0x0A09)
#define PPE_PDMA_ISR			PPE_REG_ADDR(0x0A0A)
#define PPE_PDMA_IER			PPE_REG_ADDR(0x0A0B)
#define PPE_PDMA_SUBID			PPE_REG_ADDR(0x0A0C)
#define PPE_PDMA_BAR0			PPE_REG_ADDR(0x0A0D)
#define PPE_PDMA_BAR1			PPE_REG_ADDR(0x0A0E)

/* PPE TC Logic REG (partial) */
#define PPE_DREG_A_VERSION		PPE_REG_ADDR(0x0D00)
#define PPE_DREG_A_CFG			PPE_REG_ADDR(0x0D01)
#define PPE_DREG_AT_CTRL		PPE_REG_ADDR(0x0D02)
#define PPE_DREG_AT_CB_CFG0		PPE_REG_ADDR(0x0D03)
#define PPE_DREG_AT_CB_CFG1		PPE_REG_ADDR(0x0D04)
#define PPE_DREG_AR_CTRL		PPE_REG_ADDR(0x0D08)
#define PPE_DREG_AR_CB_CFG0		PPE_REG_ADDR(0x0D09)
#define PPE_DREG_AR_CB_CFG1		PPE_REG_ADDR(0x0D0A)
#define PPE_DREG_A_UTPCFG		PPE_REG_ADDR(0x0D0E)
#define PPE_DREG_A_STATUS		PPE_REG_ADDR(0x0D0F)
#define PPE_DREG_AT_CFG0		PPE_REG_ADDR(0x0D20)
#define PPE_DREG_AT_CFG1		PPE_REG_ADDR(0x0D21)
#define PPE_DREG_AT_FB_SIZE0		PPE_REG_ADDR(0x0D22)
#define PPE_DREG_AT_FB_SIZE1		PPE_REG_ADDR(0x0D23)
#define PPE_DREG_AT_CELL0		PPE_REG_ADDR(0x0D24)
#define PPE_DREG_AT_CELL1		PPE_REG_ADDR(0x0D25)
#define PPE_DREG_AT_IDLE_CNT0		PPE_REG_ADDR(0x0D26)
#define PPE_DREG_AT_IDLE_CNT1		PPE_REG_ADDR(0x0D27)
#define PPE_DREG_AT_IDLE0		PPE_REG_ADDR(0x0D28)
#define PPE_DREG_AT_IDLE1		PPE_REG_ADDR(0x0D29)
#define PPE_DREG_AR_CFG0		PPE_REG_ADDR(0x0D60)
#define PPE_DREG_AR_CFG1		PPE_REG_ADDR(0x0D61)
#define PPE_DREG_AR_CELL0		PPE_REG_ADDR(0x0D68)
#define PPE_DREG_AR_CELL1		PPE_REG_ADDR(0x0D69)
#define PPE_DREG_AR_IDLE_CNT0		PPE_REG_ADDR(0x0D6A)
#define PPE_DREG_AR_IDLE_CNT1		PPE_REG_ADDR(0x0D6B)
#define PPE_DREG_AR_AIIDLE_CNT0		PPE_REG_ADDR(0x0D6C)
#define PPE_DREG_AR_AIIDLE_CNT1		PPE_REG_ADDR(0x0D6D)
#define PPE_DREG_AR_BE_CNT0		PPE_REG_ADDR(0x0D6E)
#define PPE_DREG_AR_BE_CNT1		PPE_REG_ADDR(0x0D6F)
#define PPE_DREG_AR_HEC_CNT0		PPE_REG_ADDR(0x0D70)
#define PPE_DREG_AR_HEC_CNT1		PPE_REG_ADDR(0x0D71)
#define PPE_DREG_AR_IDLE0		PPE_REG_ADDR(0x0D74)
#define PPE_DREG_AR_IDLE1		PPE_REG_ADDR(0x0D75)
#define PPE_DREG_AR_OVDROP_CNT0		PPE_REG_ADDR(0x0D98)
#define PPE_DREG_AR_OVDROP_CNT1		PPE_REG_ADDR(0x0D99)
#define PPE_DREG_AR_CERRN_CNT0		PPE_REG_ADDR(0x0DA0)
#define PPE_DREG_AR_CERRN_CNT1		PPE_REG_ADDR(0x0DA1)
#define PPE_DREG_AR_CERRNP_CNT0		PPE_REG_ADDR(0x0DA2)
#define PPE_DREG_AR_CERRNP_CNT1		PPE_REG_ADDR(0x0DA3)
#define PPE_DREG_AR_CVN_CNT0		PPE_REG_ADDR(0x0DA4)
#define PPE_DREG_AR_CVN_CNT1		PPE_REG_ADDR(0x0DA5)
#define PPE_DREG_AR_CVNP_CNT0		PPE_REG_ADDR(0x0DA6)
#define PPE_DREG_AR_CVNP_CNT1		PPE_REG_ADDR(0x0DA7)
#define PPE_DREG_B0_LADR		PPE_REG_ADDR(0x0DA8)
#define PPE_DREG_B1_LADR		PPE_REG_ADDR(0x0DA9)

#define BC0_LINK_STATUS			PPE_REG_ADDR(0x0DC0)
#define DSL_TC_INDICATE			PPE_REG_ADDR(0x0DC4)
#define PPE_BC0_CW_SIZE			PPE_REG_ADDR(0x0DC5)

/* SAR/PDMA REG*/
#define PPE_SAR_PDMA_RX_CMDBUF_CFG	PPE_REG_ADDR(0x0F00)
#define PPE_SAR_PDMA_TX_CMDBUF_CFG	PPE_REG_ADDR(0x0F01)
#define PPE_SAR_PDMA_RX_FW_CMDBUF_CFG	PPE_REG_ADDR(0x0F02)
#define PPE_SAR_PDMA_TX_FW_CMDBUF_CFG	PPE_REG_ADDR(0x0F03)
#define PPE_SAR_PDMA_RX_CMDBUF_STATUS	PPE_REG_ADDR(0x0F04)
#define PPE_SAR_PDMA_TX_CMDBUF_STATUS	PPE_REG_ADDR(0x0F05)


#define GIF0_RX_CRC_ERR_CNT		PPE_DREG_AR_CERRN_CNT0
#define GIF1_RX_CRC_ERR_CNT		PPE_DREG_AR_CERRNP_CNT0
#define GIF2_RX_CRC_ERR_CNT		PPE_DREG_AR_CERRN_CNT1
#define GIF3_RX_CRC_ERR_CNT		PPE_DREG_AR_CERRNP_CNT1
#define GIF0_RX_CV_CNT			PPE_DREG_AR_CVN_CNT0
#define GIF1_RX_CV_CNT			PPE_DREG_AR_CVNP_CNT0
#define GIF2_RX_CV_CNT			PPE_DREG_AR_CVN_CNT1
#define GIF3_RX_CV_CNT			PPE_DREG_AR_CVNP_CNT1
#define DREG_B0_OVERDROP_CNT		PPE_DREG_AR_OVDROP_CNT0
#define DREG_B1_OVERDROP_CNT		PPE_DREG_AR_OVDROP_CNT1

/* Mailbox IGU0 Registers */
#define MBOX_IGU0_ISRS			PPE_REG_ADDR(0x0200)
#define MBOX_IGU0_ISRC			PPE_REG_ADDR(0x0201)
#define MBOX_IGU0_ISR			PPE_REG_ADDR(0x0202)
#define MBOX_IGU0_IER			PPE_REG_ADDR(0x0203)

/* Mailbox IGU1 Registers */
#define MBOX_IGU1_ISRS			PPE_REG_ADDR(0x0204)
#define MBOX_IGU1_ISRC			PPE_REG_ADDR(0x0205)
#define MBOX_IGU1_ISR			PPE_REG_ADDR(0x0206)
#define MBOX_IGU1_IER			PPE_REG_ADDR(0x0207)

/* PPE SB address */
#define DS_FLOW_CTRL_CFG		SB_BUFFER(0x2026)
#define TX_CTRL_K_TABLE(i)		SB_BUFFER(__CTRL_K_TBL_BASE + (i))
#define DS_FRAGQ_DES_LIST_BASE(off, i)	SB_BUFFER(__DS_FRAGQ_DES_LIST_BASE + (off) + ((i) << 1))
#define DS_BOND_GIF_LL_DES_BA(i)	SB_BUFFER(__DS_BOND_GIF_LL_DES_BA + ((i) << 1))
#define DES_SYNC_CFG_CTXT(idx)		SB_BUFFER(__DES_SYNC_CFG_CTXT + ((idx) * 32))
#define BOND_US_DES_SYNC_CFG_CTXT(i)	SB_BUFFER(__BOND_US_DES_SYNC_CFG_CTXT + ((i) * 8))
#define INQ_QOS_CFG_BASE(off)		SB_BUFFER(__INQ_QOS_CFG_BASE + (off))

/* PP32 debug register and macro */
#define PP32_DBG_CTRL(n)				PP32_DEBUG_REG_ADDR(n, 0x0000)

#define DBG_CTRL_RESTART                                0
#define DBG_CTRL_STOP                                   1

#define PP32_CTRL_CMD(n)				PP32_DEBUG_REG_ADDR(n, 0x0B00)
#define PP32_CTRL_CMD_RESTART				(1 << 0)
#define PP32_CTRL_CMD_STOP				(1 << 1)
#define PP32_CTRL_CMD_STEP				(1 << 2)
#define PP32_CTRL_CMD_BREAKOUT				(1 << 3)

#define PP32_CTRL_OPT(n)				PP32_DEBUG_REG_ADDR(n, 0x0C00)
#define PP32_CTRL_OPT_BREAKOUT_ON_STOP_ON		(3 << 0)
#define PP32_CTRL_OPT_BREAKOUT_ON_STOP_OFF		(2 << 0)
#define PP32_CTRL_OPT_BREAKOUT_ON_BREAKIN_ON		(3 << 2)
#define PP32_CTRL_OPT_BREAKOUT_ON_BREAKIN_OFF		(2 << 2)
#define PP32_CTRL_OPT_STOP_ON_BREAKIN_ON		(3 << 4)
#define PP32_CTRL_OPT_STOP_ON_BREAKIN_OFF		(2 << 4)
#define PP32_CTRL_OPT_STOP_ON_BREAKPOINT_ON		(3 << 6)
#define PP32_CTRL_OPT_STOP_ON_BREAKPOINT_OFF		(2 << 6)
#define PP32_CTRL_OPT_BREAKOUT_ON_STOP(base, n)        	((vrx318_r32(PP32_CTRL_OPT(n), base)) & (1 << 0))
#define PP32_CTRL_OPT_BREAKOUT_ON_BREAKIN(base, n)	((vrx318_r32(PP32_CTRL_OPT(n), base)) & (1 << 2))
#define PP32_CTRL_OPT_STOP_ON_BREAKIN(base, n)		((vrx318_r32(PP32_CTRL_OPT(n), base)) & (1 << 4))
#define PP32_CTRL_OPT_STOP_ON_BREAKPOINT(base, n)	((vrx318_r32(PP32_CTRL_OPT(n), base)) & (1 << 6))

#define PP32_BRK_PC(n, i)				(PP32_DEBUG_REG_ADDR(n, 0x0900 + (i) * 2))
#define PP32_BRK_PC_MASK(n, i)				(PP32_DEBUG_REG_ADDR(n, 0x0901 + (i) * 2))
#define PP32_BRK_DATA_ADDR(n, i)			(PP32_DEBUG_REG_ADDR(n, 0x0904 + (i) * 2))
#define PP32_BRK_DATA_ADDR_MASK(n, i)			(PP32_DEBUG_REG_ADDR(n, 0x0905 + (i) * 2))
#define PP32_BRK_DATA_VALUE_RD(n, i)			(PP32_DEBUG_REG_ADDR(n, 0x0908 + (i) * 2))
#define PP32_BRK_DATA_VALUE_RD_MASK(n, i)		(PP32_DEBUG_REG_ADDR(n, 0x0909 + (i) * 2))
#define PP32_BRK_DATA_VALUE_WR(n, i)			(PP32_DEBUG_REG_ADDR(n, 0x090C + (i) * 2))
#define PP32_BRK_DATA_VALUE_WR_MASK(n, i)		(PP32_DEBUG_REG_ADDR(n, 0x090D + (i) * 2))
#define PP32_BRK_CONTEXT_MASK(i)			(1 << (i))
#define PP32_BRK_CONTEXT_MASK_EN			(1 << 4)
#define PP32_BRK_COMPARE_GREATER_EQUAL			(1 << 5)    /* valid for break data value rd/wr only */
#define PP32_BRK_COMPARE_LOWER_EQUAL			(1 << 6)
#define PP32_BRK_COMPARE_EN				(1 << 7)

#define PP32_BRK_TRIG(n)				PP32_DEBUG_REG_ADDR(n, 0x0F00)
#define PP32_BRK_GRPi_PCn_ON(i, n)			((3 << ((n) * 2)) << ((i) * 16))
#define PP32_BRK_GRPi_PCn_OFF(i, n)			((2 << ((n) * 2)) << ((i) * 16))
#define PP32_BRK_GRPi_DATA_ADDRn_ON(i, n)		((3 << ((n) * 2 + 4)) << ((i) * 16))
#define PP32_BRK_GRPi_DATA_ADDRn_OFF(i, n)		((2 << ((n) * 2 + 4)) << ((i) * 16))
#define PP32_BRK_GRPi_DATA_VALUE_RDn_ON(i, n)		((3 << ((n) * 2 + 8)) << ((i) * 16))
#define PP32_BRK_GRPi_DATA_VALUE_RDn_OFF(i, n)		((2 << ((n) * 2 + 8)) << ((i) * 16))
#define PP32_BRK_GRPi_DATA_VALUE_WRn_ON(i, n)		((3 << ((n) * 2 + 12)) << ((i) * 16))
#define PP32_BRK_GRPi_DATA_VALUE_WRn_OFF(i, n)		((2 << ((n) * 2 + 12)) << ((i) * 16))
#define PP32_BRK_GRPi_PCn(base, k, i, n)		((vrx318_r32(PP32_BRK_TRIG(k), base)) & ((1 << ((n))) << ((i) * 8)))
#define PP32_BRK_GRPi_DATA_ADDRn(base, k, i, n)		((vrx318_r32(PP32_BRK_TRIG(k), base)) & ((1 << ((n) + 2)) << ((i) * 8)))
#define PP32_BRK_GRPi_DATA_VALUE_RDn(base, k, i, n)	((vrx318_r32(PP32_BRK_TRIG(k), base)) & ((1 << ((n) + 4)) << ((i) * 8)))
#define PP32_BRK_GRPi_DATA_VALUE_WRn(base, k, i, n)	((vrx318_r32(PP32_BRK_TRIG(k), base)) & ((1 << ((n) + 6)) << ((i) * 8)))

#define PP32_CPU_STATUS(n)				PP32_DEBUG_REG_ADDR(n, 0x0D00)
#define PP32_HALT_STAT(n)				PP32_CPU_STATUS(n)
#define PP32_DBG_CUR_PC(n)				PP32_CPU_STATUS(n)
#define PP32_CPU_USER_STOPPED(base, n)			((vrx318_r32(PP32_CPU_STATUS(n), base)) & (1 << 0))
#define PP32_CPU_USER_BREAKIN_RCV(base, n)		((vrx318_r32(PP32_CPU_STATUS(n), base)) & (1 << 1))
#define PP32_CPU_USER_BREAKPOINT_MET(base, n)		((vrx318_r32(PP32_CPU_STATUS(n), base)) & (1 << 2))
#define PP32_CPU_CUR_PC(base, n)			((vrx318_r32(PP32_CPU_STATUS(n), base)) >> 16)

#define PP32_BREAKPOINT_REASONS(n)			PP32_DEBUG_REG_ADDR(n, 0x0A00)
#define PP32_BRK_PC_MET(base, n, i)			((vrx318_r32(PP32_BREAKPOINT_REASONS(n), base)) & (1 << (i)))
#define PP32_BRK_DATA_ADDR_MET(base, n, i)		((vrx318_r32(PP32_BREAKPOINT_REASONS(n), base)) & (1 << ((i) + 2)))
#define PP32_BRK_DATA_VALUE_RD_MET(base, n, i)		((vrx318_r32(PP32_BREAKPOINT_REASONS(n), base)) & (1 << ((i) + 4)))
#define PP32_BRK_DATA_VALUE_WR_MET(base, n, i)		((vrx318_r32(PP32_BREAKPOINT_REASONS(n), base)) & (1 << ((i) + 6)))
#define PP32_BRK_DATA_VALUE_RD_LO_EQ(base, n, i)	((vrx318_r32(PP32_BREAKPOINT_REASONS(n), base)) & (1 << ((i) * 2 + 8)))
#define PP32_BRK_DATA_VALUE_RD_GT_EQ(base, n, i)	((vrx318_r32(PP32_BREAKPOINT_REASONS(n), base)) & (1 << ((i) * 2 + 9)))
#define PP32_BRK_DATA_VALUE_WR_LO_EQ(base, n, i)	((vrx318_r32(PP32_BREAKPOINT_REASONS(n), base)) & (1 << ((i) * 2 + 12)))
#define PP32_BRK_DATA_VALUE_WR_GT_EQ(base, n, i)	((vrx318_r32(PP32_BREAKPOINT_REASONS(n), base)) & (1 << ((i) * 2 + 13)))
#define PP32_BRK_CUR_CONTEXT(base, n)			(((vrx318_r32(PP32_BREAKPOINT_REASONS(n), base)) >> 8) & 0x03)

#define PP32_GP_REG_BASE(base, n)			vrx318_r32(PP32_DEBUG_REG_ADDR(n, 0x0E00), base)
#define PP32_GP_CONTEXTi_REGn(base, n, i, j)		vrx318_r32(PP32_DEBUG_REG_ADDR(n, 0x0E00 + (i) * 16 + (j)), base)

enum {
	MBOX_IGU0 = 0,
	MBOX_IGU1
};

enum {
	PPE_PROC_READ = 1,
	PPE_PROC_WRITE
};

/* VRX318 RST REG */
#define RST_PPE_DSL_IF	(1 << 3)
#define RST_PPE_DFE	(1 << 7)
#define RST_DMA_CORE	(1 << 9)
#define RST_PPE_ATM_TC	(1 << 23)
#define RCU_BASE	0x1E002000
#define RST_STAT	(RCU_BASE + 0x0)
#define RST_REQ		(RCU_BASE + 0x10)


/* VRX318 CGU REG */
#define CGU_BASE	0x1E003000
#define PMU_PWDCR	(CGU_BASE + 0x011C)
#define PMU_SR		(CGU_BASE + 0x0120)
#define PMU_PPE_EMA	(1 << 22)
#define CGU_CLKFSR	(CGU_BASE + 0x10)
#define SET_PPE_CLK(val, x)	((val & (~(0x7 << 16))) | (x & 0x7) << 16)
#define VRX318_SYS_CLK	288000000


/* VRX318 DMA REG */
#define DMA_BASE	0x1E104100
#define DMA_CTRL	(DMA_BASE + 0x10)
#define DMA_CPOLL	(DMA_BASE + 0x14)
#define DMA_CS		(DMA_BASE + 0x18)
#define DMA_CCTRL	(DMA_BASE + 0x1C)
#define DMA_CBA		(DMA_BASE + 0x20)
#define DMA_CDLEN	(DMA_BASE + 0x24)
#define DMA_CIS		(DMA_BASE + 0x28)
#define DMA_CIE		(DMA_BASE + 0x2C)
#define DMA_CDPTNRD	(DMA_BASE + 0x34)
#define DMA_PS		(DMA_BASE + 0x40)
#define DMA_PCTRL	(DMA_BASE + 0x44)

/* VRX318 xDSL DFE(MEI) REG */
#define MEI_DFE_BASE	0x1E116000
#define MEI_RST_CTRL	(MEI_DFE_BASE + 0x18)

/* VRX318 DSL DFE REG */
#define DSL_DFE_BASE	0x1E400000
#define DSL_CRI_RST	(DSL_DFE_BASE + 0x20C404)

/* PDBRAM */
#define PDBRAM_TX_PKT_BUF_BASE	0x1E098000
#define PDBRAM_TX_PKT_BUF_END	0x1E09DDBF
#define PDBRAM_RX_PKT_BUF_BASE	0x1E09DDC0
#define PDBRAM_RX_PKT_BUF_END	0x1E0A3B7F


struct fw_ver_id { /*@2000*/
	/* DWORD 0 */
	unsigned int family:4;
	unsigned int package:4;
	unsigned int major:8;
	unsigned int middle:8;
	unsigned int minor:8;

	/* DWORD 1 */
	unsigned int features;
};

struct psave_cfg { /*@2014*/
	unsigned int res1:15;
	unsigned int start_state:1;  /*1: start from partial PPE reset, 0: start from full PPE reset */
	unsigned int res2:15;
	unsigned int sleep_en:1;  /*1: enable sleep mode, 0: disable sleep mode */
};

struct vrx318_soc_cfg {
	u32 soc_cpu_us_phybase;
	int soc_cpu_us_dnum;
	u32 soc_fp_us_phybase;
	int soc_fp_us_dnum;
	u32 soc_ds_phybase;
	u32 soc_ds_dbase;
	int soc_ds_dnum;
	int soc_us_desc_own; /* des belong to VRX318 */
	int soc_ds_desc_own;
	int pmac_en;
};

#define MAX_VRX318_NUM (IFX_PCIE_EP_MAX_PEER + 1)
#define PP32_NUM	2
#define MAX_MTU			1600
#define CRC_SIZE		4
#define PMAC_SIZE		8
#define DMA_ALIGNMENT		32
#define MAX_PKT_SIZE		(MAX_MTU + CRC_SIZE + PMAC_SIZE)
#define DMA_PACKET_SIZE		ALIGN(MAX_PKT_SIZE, DMA_ALIGNMENT)
#define QOSQ_NUM		16 /* MAX 16 */
#define TOTAL_QOS_DES_NUM	512
#define FW_STOP_TIMEOUT		200
#define PPE_PDMA_RX_MAX_LEN_REG_CONF (0x02040000 | MAX_MTU)
/* Interrupt bit definition */
/**
*  Bit 0 - Receive Pkt in Ds_pkt_des_list queue
*  Bit 1 - Reserved for ATM: Receive Pkt in DS OAM  queue
*  Bit 2 - Swap Queue pkt (obsolete)
*  Bit 3 - EDMA Hang
*  Bit 4 - Peer-to-peer-link-state [single link no support]
*  Bit 5 - PPE FW ready to be unloaded
*/
#define PKT_RX_INTR		BIT(0)
#define OAM_RX_INTR		BIT(1)
#define EDMA_HANG_INTR		BIT(3)
#define P2P_STAT_INTR		BIT(4)
#define FW_UNLOAD_INTR		BIT(5)

#define VRX318_ASSERT(cond, fmt, arg...)	do { if (!(cond)) pr_err_ratelimited("%d:%s: " fmt "\n", __LINE__, __func__, ##arg); } while (0)

void init_dsl_callback(void);
void *ppa_callback_get(int type);
int ppa_callback_set(int type, void *func);
int get_num_from_str(char **p, int *len, int is_hex);
void ignore_space(char **p, int *len);
int get_number_f_buf(const char __user __force **buf, int *len, int base, u32 *ret_val);
int soc_vrx318_cfg_init(struct vrx318_soc_cfg *);
void soc_vrx318_cfg_reset(void);

#endif /*__VRX218_COMMON_H__*/
