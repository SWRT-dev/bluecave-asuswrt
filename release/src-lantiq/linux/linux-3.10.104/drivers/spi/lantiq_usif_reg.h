/******************************************************************************
**
** FILE NAME    : ltqmips_usif_reg.h
** PROJECT      : IFX UEIP
** MODULES      : USIF for UART and SPI
**
** DATE         : 21 Aug 2008
** AUTHOR       : Lei Chuanhua
** DESCRIPTION  : USIF module register definitioin
** COPYRIGHT    :       Copyright (c) 2008 ~ 2009
**                      Infineon Technologies AG
**                      Am Campeon 1-12, 85579 Neubiberg, Germany
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
** HISTORY
** $Date         $Author         $Comment
** 21 Aug, 2009  Lei Chuanhua    Initial version
** 26 Jun, 2009  Lei Chuanhua    Code review modification
*******************************************************************************/

#ifndef LTQMIPS_USIF_REG_H
#define LTQMIPS_USIF_REG_H

/*!
 \file ltqmips_usif_reg.h
 \ingroup LANTIQ_USIF_SPI
 \brief ifx usif module register definition
*/
#include <asm/io.h>

#define LANTIQ_USIF_PHY_BASE    0x1DA00000
#define LANTIQ_USIF_VIRT_BASE  (LANTIQ_USIF_PHY_BASE + KSEG1)
#define LANTIQ_USIF_SIZE       0x100000 /* 1MB */

/* USIF System Registers */
#define LANTIQ_USIF_CLC             0x00 /* Clock Control Register */

#define LANTIQ_USIF_CLC_RUN_STAT    0x00000001
#define LANTIQ_USIF_CLC_CFG_STAT    0x00000002
#define LANTIQ_USIF_CLC_STAT_M      0x00000003
#define LANTIQ_USIF_CLC_MOD_EN      0x00000004
#define LANTIQ_USIF_CLC_MOD_DIS     0x00000008
#define LANTIQ_USIF_CLC_SPEN_EN     0x00000010
#define LANTIQ_USIF_CLC_SPEN_DIS    0x00000020
#define LANTIQ_USIF_CLC_FSOE_EN     0x00000040
#define LANTIQ_USIF_CLC_FSOE_DIS    0x00000080
#define LANTIQ_USIF_CLC_EDR_EN      0x00000100
#define LANTIQ_USIF_CLC_EDR_DIS     0x00000200
#define LANTIQ_USIF_CLC_IDR_EN      0x00000400
#define LANTIQ_USIF_CLC_IDR_DIS     0x00000800

#define LANTIQ_USIF_CLC_CNT         0x04 /* Clock Control Counter Register */

#define LANTIQ_USIF_CLC_CNT_RMC     0x000000ff
#define LANTIQ_USIF_CLC_CNT_RMC_S   0
#define LANTIQ_USIF_CLC_CNT_ORMC    0x0000ff00
#define LANTIQ_USIF_CLC_CNT_ORMC_S  8

#define LANTIQ_USIF_CLC_STAT        0x08 /* Clock Control Status Register */

#define LANTIQ_USIF_CLC_STAT_RUN              0x00000001
#define LANTIQ_USIF_CLC_STAT_MODEN            0x00000002
#define LANTIQ_USIF_CLC_STAT_SPEN             0x00000004
#define LANTIQ_USIF_CLC_STAT_FSOE             0x00000008
#define LANTIQ_USIF_CLC_STAT_EDRE             0x00000010
#define LANTIQ_USIF_CLC_STAT_IDRE             0x00000020
#define LANTIQ_USIF_CLC_STAT_CNT_UPDATE_DONE  0x00000040
#define LANTIQ_USIF_CLC_STAT_KERN_IDL         0x00000080
#define LANTIQ_USIF_CLC_STAT_FIFO_IDL         0x00000100

#define LANTIQ_USIF_CLC_STAT_CFG_UPDATE_DONE  (LANTIQ_USIF_CLC_STAT_MODEN |\
	LANTIQ_USIF_CLC_STAT_CNT_UPDATE_DONE | LANTIQ_USIF_CLC_STAT_KERN_IDL\
	| LANTIQ_USIF_CLC_STAT_FIFO_IDL)

#define LANTIQ_USIF_CLC_STAT_RUN_UPDATE_DONE  (LANTIQ_USIF_CLC_STAT_MODEN |\
	LANTIQ_USIF_CLC_STAT_CNT_UPDATE_DONE | LANTIQ_USIF_CLC_STAT_KERN_IDL\
	| LANTIQ_USIF_CLC_STAT_RUN | LANTIQ_USIF_CLC_STAT_FIFO_IDL)

#define LANTIQ_USIF_ID                        0x0c

#define LANTIQ_USIF_ID_TS_VER_NR              0xffff0000
#define LANTIQ_USIF_ID_TS_VER_NR_S            16
#define LANTIQ_USIF_ID_MOD_ID                 0x0000ff00
#define LANTIQ_USIF_ID_MOD_ID_S               8
#define LANTIQ_USIF_ID_REV_NR                 0x000000ff
#define LANTIQ_USIF_ID_REV_NR_S               0

#define LANTIQ_USIF_FIFO_ID                   0x10

#define LANTIQ_USIF_FIFO_ID_TX                0x000000ff
#define LANTIQ_USIF_FIFO_ID_TX_S              0
#define LANTIQ_USIF_FIFO_ID_RX                0x0000ff00
#define LANTIQ_USIF_FIFO_ID_RX_S              8
#define LANTIQ_USIF_FIFO_ID_RPS               0x003f0000
#define LANTIQ_USIF_FIFO_ID_RPS_S             16
#define LANTIQ_USIF_FIFO_ID_DPLUS_CLS_ID      0x07000000
#define LANTIQ_USIF_FIFO_ID_DPLUS_CLS_ID_S    24

enum {
	LANTIQ_USIF_FIFO_ID_DPLUS_CLS_WIDTH0 = 0, /* Dplus disabled */
	LANTIQ_USIF_FIFO_ID_DPLUS_CLS_WIDTH2,
	LANTIQ_USIF_FIFO_ID_DPLUS_CLS_WIDTH4,
	LANTIQ_USIF_FIFO_ID_DPLUS_CLS_WIDTH8,
};

#define LANTIQ_USIF_SRB_MSCONF_ID      0x14

#define LANTIQ_USIF_SRB_ERRCONF_ID     0x18

#define LANTIQ_USIF_SWCID           0x1c /* USIF sw check identity */

/* USIF Special Function Registers */
#define LANTIQ_USIF_MSS_SET         0x100 /* Modemstatus Set Register */

#define LANTIQ_USIF_MSS_SET_FCO     0x00000001
#define LANTIQ_USIF_MSS_SET_DTR     0x00000004
#define LANTIQ_USIF_MSS_SET_DSR     0x00000008
#define LANTIQ_USIF_MSS_SET_DCD     0x00000010
#define LANTIQ_USIF_MSS_SET_RI      0x00000020

#define LANTIQ_USIF_MSS_CLR         0x104

#define LANTIQ_USIF_MSS_CLR_FCO     0x00000001
#define LANTIQ_USIF_MSS_CLR_DTR     0x00000004
#define LANTIQ_USIF_MSS_CLR_DSR     0x00000008
#define LANTIQ_USIF_MSS_CLR_DCD     0x00000010
#define LANTIQ_USIF_MSS_CLR_RI      0x00000020

#define LANTIQ_USIF_MSS_STAT        0x108

#define LANTIQ_USIF_MSS_STAT_FCO    0x00000001
#define LANTIQ_USIF_MSS_STAT_FCI    0x00000002
#define LANTIQ_USIF_MSS_STAT_DTR    0x00000004
#define LANTIQ_USIF_MSS_STAT_DSR    0x00000008
#define LANTIQ_USIF_MSS_STAT_DCD    0x00000010
#define LANTIQ_USIF_MSS_STAT_RI     0x00000020

#define LANTIQ_USIF_MSS_CTRL         0x10c

#define LANTIQ_USIF_MSS_CTRL_FCIEN   0x00000001
#define LANTIQ_USIF_MSS_CTRL_FCOEN   0x00000002

#define LANTIQ_USIF_MODE_CFG         0x110

#define LANTIQ_USIF_MODE_CFG_SYNC    0x00000001
#define LANTIQ_USIF_MODE_CFG_MA      0x00000002
#define LANTIQ_USIF_MODE_CFG_RX_EN   0x00000004
#define LANTIQ_USIF_MODE_CFG_FC_EN   0x00000008
#define LANTIQ_USIF_MODE_CFG_LB      0x00000010
#define LANTIQ_USIF_MODE_CFG_ECH     0x00000020
#define LANTIQ_USIF_MODE_CFG_TX_INV  0x00000040
#define LANTIQ_USIF_MODE_CFG_RX_INV  0x00000080
#define LANTIQ_USIF_MODE_CFG_SC_FRC  0x00000100
#define LANTIQ_USIF_MODE_CFG_SC_POL  0x00000200
#define LANTIQ_USIF_MODE_CFG_SC_PH   0x00000400
#define LANTIQ_USIF_MODE_CFG_TX_IDLE 0x00000800
#define LANTIQ_USIF_MODE_CFG_AB_EN   0x00001000
#define LANTIQ_USIF_MODE_CFG_EC_EN   0x00002000
#define LANTIQ_USIF_MODE_CFG_EQA     0x00004000
#define LANTIQ_USIF_MODE_CFG_HDEN    0x00008000
#define LANTIQ_USIF_MODE_CFG_TX_EN   0x00010000

#define LANTIQ_USIF_MODE_CFG_SC_MODE_MASK  (LANTIQ_USIF_MODE_CFG_SC_POL |\
	LANTIQ_USIF_MODE_CFG_SC_PH)

#define LANTIQ_USIF_MODE_CFG_TXRX_EN       (LANTIQ_USIF_MODE_CFG_RX_EN |\
	LANTIQ_USIF_MODE_CFG_TX_EN)

#define LANTIQ_USIF_MODE_CFG_TXRX_MASK      LANTIQ_USIF_MODE_CFG_TXRX_EN

#define LANTIQ_USIF_PRTC_CFG          0x114 /* Protocol Configuration Register */

#define LANTIQ_USIF_PRTC_CFG_CLEN5    0x05
#define LANTIQ_USIF_PRTC_CFG_CLEN6    0x06
#define LANTIQ_USIF_PRTC_CFG_CLEN7    0x07
#define LANTIQ_USIF_PRTC_CFG_CLEN8    0x08
#define LANTIQ_USIF_PRTC_CFG_CLEN16   0x10
#define LANTIQ_USIF_PRTC_CFG_CLEN32   0x00
#define LANTIQ_USIF_PRTC_CFG_CLEN     0x0000001f
#define LANTIQ_USIF_PRTC_CFG_CLEN_S   0
#define LANTIQ_USIF_PRTC_CFG_HD_MSB   0x00000080
#define LANTIQ_USIF_PRTC_CFG_PAR      0x00000100
#define LANTIQ_USIF_PRTC_CFG_ODD      0x00000200
#define LANTIQ_USIF_PRTC_CFG_ERR_FE   0x00000400
#define LANTIQ_USIF_PRTC_CFG_STP2     0x00001000
#define LANTIQ_USIF_PRTC_CFG_WK_UP    0x00002000
#define LANTIQ_USIF_PRTC_CFG_FCS_INV  0x00040000
#define LANTIQ_USIF_PRTC_CFG_FCO_TL   0x03F00000
#define LANTIQ_USIF_PRTC_CFG_FCO_TL_S 20
#define LANTIQ_USIF_PRTC_CFG_SFCO_EN  0x04000000
#define LANTIQ_USIF_PRTC_CFG_SFCI_EN  0x08000000
#define LANTIQ_USIF_PRTC_CFG_SLIP_EN  0x10000000
#define LANTIQ_USIF_PRTC_CFG_SLIP_EXT 0x20000000
#define LANTIQ_USIF_PRTC_CFG_HDLC     0x40000000

#define LANTIQ_USIF_PRTC_STAT         0x118 /* Protocol Status Register */

#define LANTIQ_USIF_PRTC_STAT_CLEN    0x0000001f
#define LANTIQ_USIF_PRTC_STAT_CLEN_S  0
#define LANTIQ_USIF_PRTC_STAT_PAR     0x00000100
#define LANTIQ_USIF_PRTC_STAT_ODD     0x00000200
#define LANTIQ_USIF_PRTC_STAT_AB_DONE 0x00008000
#define LANTIQ_USIF_PRTC_STAT_BCRV    0x0fff0000
#define LANTIQ_USIF_PRTC_STAT_BRCV_S  16
#define LANTIQ_USIF_PRTC_STAT_TXACT   0x80000000

#define LANTIQ_USIF_FRM_CTRL          0x11c /* Frame Control Register */

#define LANTIQ_USIF_FRM_CTRL_SFEN     0x00000001
#define LANTIQ_USIF_FRM_CTRL_IBEN     0x00000004
#define LANTIQ_USIF_FRM_CTRL_IAEN     0x00000008
#define LANTIQ_USIF_FRM_CTRL_DLEN     0x0000fff0
#define LANTIQ_USIF_FRM_CTRL_DLEN_S   4
#define LANTIQ_USIF_FRM_CTRL_ICLK     0x00040000
#define LANTIQ_USIF_FRM_CTRL_STOP     0x00100000
#define LANTIQ_USIF_FRM_CTRL_PLEN     0xffc00000
#define LANTIQ_USIF_FRM_CTRL_PLEN_S   22

#define LANTIQ_USIF_FRM_STAT          0x120 /* Frame Status Register */

#define LANTIQ_USIF_FRM_STAT_SFEN     0x00000001
#define LANTIQ_USIF_FRM_STAT_DBSY     0x00000002
#define LANTIQ_USIF_FRM_STAT_PBSY     0x00000004
#define LANTIQ_USIF_FRM_STAT_DCNT     0x0000fff0
#define LANTIQ_USIF_FRM_STAT_DCNT_S   4
#define LANTIQ_USIF_FRM_STAT_PCNT     0xffc00000
#define LANTIQ_USIF_FRM_STAT_PCNT_S   22

#define LANTIQ_USIF_CRC_CFG           0x124 /* CRC Configuration Register */

#define LANTIQ_USIF_CRC_CFG_GRADE8    0x00000008
#define LANTIQ_USIF_CRC_CFG_GRADE16   0x00000010
#define LANTIQ_USIF_CRC_CFG_GRADE32   0x00
#define LANTIQ_USIF_CRC_CFG_INIT      0x00000100
#define LANTIQ_USIF_CRC_CFG_HD_MSB    0x00000200
#define LANTIQ_USIF_CRC_CFG_REFO      0x00000400
#define LANTIQ_USIF_CRC_CFG_FIN_XOR   0x00000800
#define LANTIQ_USIF_CRC_CFG_ENDIAN    0x00001000

#define LANTIQ_USIF_CRCPOLY_CFG       0x128

#define LANTIQ_USIF_CRC_CTRL          0x12c

#define LANTIQ_USIF_CRC_CTRL_RX_EN    0x00000001
#define LANTIQ_USIF_CRC_CTRL_TX_EN    0x00000002

#define LANTIQ_USIF_CS_CFG            0x130

#define LANTIQ_USIF_CS_CFG_EN         0x00000001
#define LANTIQ_USIF_CS_CFG_EACS       0x00000002
#define LANTIQ_USIF_CS_CFG_INV        0x00000010
#define LANTIQ_USIF_CS_CFG_CS0_HI     0x00000100
#define LANTIQ_USIF_CS_CFG_CS1_HI     0x00000200
#define LANTIQ_USIF_CS_CFG_CS2_HI     0x00000400
#define LANTIQ_USIF_CS_CFG_CS3_HI     0x00000800
#define LANTIQ_USIF_CS_CFG_CS4_HI     0x00001000
#define LANTIQ_USIF_CS_CFG_CS5_HI     0x00002000
#define LANTIQ_USIF_CS_CFG_CS6_HI     0x00004000
#define LANTIQ_USIF_CS_CFG_CS7_HI     0x00008000
#define LANTIQ_USIF_CS_CFG_CSO_CLK    0x000f0000
#define LANTIQ_USIF_CS_CFG_CS0_CLK_S  16
#define LANTIQ_USIF_CS_CFG_CS0_FRM    0x00f00000
#define LANTIQ_USIF_CS_CFG_CS0_FRM_S  20
#define LANTIQ_USIF_CS_CFG_CSO_INV    0xFF000000
#define LANTIQ_USIF_CS_CFG_CSO_INV_S  24
#define LANTIQ_USIF_CS_CFG_CSX_DEFAULT 0xFF000000

#define LANTIQ_USIF_CS_OFFSET         8
#define LANTIQ_USIF_CS_INV_OFFSET     24

#define LANTIQ_USIF_FDIV_CFG          0x140

#define LANTIQ_USIF_FDIV_CFG_INC      0x0001ff
#define LANTIQ_USIF_FDIV_CFG_INC_S    0
#define LANTIQ_USIF_FDIV_CFG_DEC      0xffff0000
#define LANTIQ_USIF_FDIV_CFG_DEC_S    16

#define LANTIQ_USIF_BC_CFG            0x144

#define LANTIQ_USIF_BC_CFG_BCRV       0x00001fff
#define LANTIQ_USIF_BC_CFG_BCRV_S     0
#define LANTIQ_USIF_BC_CFG_SCDIV_HALF 0x01000000

#define LANTIQ_USIF_ICTMO_CFG         0x148

#define LANTIQ_USIF_ICTMO_CFG_TMO     0x0000ffff
#define LANTIQ_USIF_ICTMO_CFG_TMO_S   0

#define LANTIQ_USIF_ICTM_CFG          0x14c

#define LANTIQ_USIF_ICTM_CFG_TM       0x00003fff
#define LANTIQ_USIF_ICTM_CFG_TM_S     0

#define LANTIQ_USIF_ECTM_CFG          0x150

#define LANTIQ_USIF_ECTM_CFG_ECTM     0x000003ff
#define LANTIQ_USIF_ECTM_CFG_ECTM_S   0

/* USIF Chip Select Timing Configuration */
#define LANTIQ_USIF_CS_TIM0_CFG       0x154
#define LANTIQ_USIF_CS_TIM1_CFG       0x158
#define LANTIQ_USIF_CS_TIM2_CFG       0x15C
#define LANTIQ_USIF_CS_TIM3_CFG       0x160
#define LANTIQ_USIF_CS_TIM4_CFG       0x164
#define LANTIQ_USIF_CS_TIM5_CFG       0x168
#define LANTIQ_USIF_CS_TIM6_CFG       0x16C
#define LANTIQ_USIF_CS_TIM7_CFG       0x170
#define LANTIQ_USIF_CS_TIMX_CFG(cs)   (LANTIQ_USIF_CS_TIM0_CFG + (cs << 2))

/* Setup/Hold/Idle defintion in CS timing configuration */
#define LANTIQ_USIF_CS_TIM_CFG_IDLE    0x000000FF
#define LANTIQ_USIF_CS_TIM_CFG_IDLE_S  0
#define LANTIQ_USIF_CS_TIM_CFG_HOLD    0x0000FF00
#define LANTIQ_USIF_CS_TIM_CFG_HOLD_S  8
#define LANTIQ_USIF_CS_TIM_CFG_SETUP   0x00FF0000
#define LANTIQ_USIF_CS_TIM_CFG_SETUP_S 16

/* USIF Data Buffer Registers */
#define LANTIQ_USIF_FIFO_CFG          0x30 /* FIFO configuration register */

#define LANTIQ_USIF_FIFO_CFG_RXBS     0x00000007
#define LANTIQ_USIF_FIFO_CFG_RXBS_S   0
#define LANTIQ_USIF_FIFO_CFG_RXFC     0x00000008
#define LANTIQ_USIF_FIFO_CFG_TXBS     0x00000070
#define LANTIQ_USIF_FIFO_CFG_TXBS_S   4
#define LANTIQ_USIF_FIFO_CFG_TXFC     0x00000080
#define LANTIQ_USIF_FIFO_CFG_RXFA     0x00000700
#define LANTIQ_USIF_FIFO_CFG_RX_SWAP  0x00000800
#define LANTIQ_USIF_FIFO_CFG_RXFA_S   8
#define LANTIQ_USIF_FIFO_CFG_TXFA     0x00007000
#define LANTIQ_USIF_FIFO_CFG_TXFA_S   12
#define LANTIQ_USIF_FIFO_CFG_TX_SWAP  0x00008000
enum {
	LANTIQ_USIF_RXFIFO_BURST_WORD1 = 0,
	LANTIQ_USIF_RXFIFO_BURST_WORD2,
	LANTIQ_USIF_RXFIFO_BURST_WORD4,
	LANTIQ_USIF_RXFIFO_BURST_WORD8,
	LANTIQ_USIF_RXFIFO_BURST_WORD16,
};

enum {
	LANTIQ_USIF_TXFIFO_BURST_WORD1 = 0,
	LANTIQ_USIF_TXFIFO_BURST_WORD2,
	LANTIQ_USIF_TXFIFO_BURST_WORD4,
	LANTIQ_USIF_TXFIFO_BURST_WORD8,
	LANTIQ_USIF_TXFIFO_BURST_WORD16,
};

enum {
	LANTIQ_USIF_RXFIFO_ALIGN_BYTE = 0,
	LANTIQ_USIF_RXFIFO_ALIGN_HALFWORD,
	LANTIQ_USIF_RXFIFO_ALIGN_WORD,
	LANTIQ_USIF_RXFIFO_ALIGN_DWORD,
};

enum {
	LANTIQ_USIF_TXFIFO_ALIGN_BYTE = 0,
	LANTIQ_USIF_TXFIFO_ALIGN_HALFWORD,
	LANTIQ_USIF_TXFIFO_ALIGN_WORD,
	LANTIQ_USIF_TXFIFO_ALIGN_DWORD,
};

#define LANTIQ_USIF_FIFO_CTRL              0x34

#define LANTIQ_USIF_FIFO_CTRL_TX_START     0x00000001
#define LANTIQ_USIF_FIFO_CTRL_TX_ABORT     0x00000002
#define LANTIQ_USIF_FIFO_CTRL_TX_SETEOP    0x00000004
#define LANTIQ_USIF_FIFO_CTRL_RX_START     0x00000010
#define LANTIQ_USIF_FIFO_CTRL_RX_ABORT     0x00000020
#define LANTIQ_USIF_FIFO_CTRL_RX_AR_ON     0x00000040
#define LANTIQ_USIF_FIFO_CTRL_RX_AR_OFF    0x00000080

#define LANTIQ_USIF_MRPS_CTRL            0x38

#define LANTIQ_USIF_MRPS_CTRL_MRPS       0x00003fff
#define LANTIQ_USIF_MRPS_CTRL_MRPS_S     0

#define LANTIQ_USIF_MRPS_MIN             1
#define LANTIQ_USIF_MRPS_MAX             16383

#define LANTIQ_USIF_FIFO_STAT              0x44

#define LANTIQ_USIF_FIFO_STAT_RX_FFS       0x000000ff
#define LANTIQ_USIF_FIFO_STAT_RX_FFS_S     0
#define LANTIQ_USIF_FIFO_STAT_RX_AR_ON     0x00010000
#define LANTIQ_USIF_FIFO_STAT_RX_EOP       0x00020000
#define LANTIQ_USIF_FIFO_STAT_RX_THR_EXC   0x00040000
#define LANTIQ_USIF_FIFO_STAT_MRPS_WE      0x00080000
#define LANTIQ_USIF_FIFO_STAT_TX_FFS       0x00ff0000
#define LANTIQ_USIF_FIFO_STAT_TX_FFS_S     16
#define LANTIQ_USIF_FIFO_STAT_TX_THR_EXC   0x40000000
#define LANTIQ_USIF_FIFO_STAT_TPS_WE       0x80000000

#define LANTIQ_USIF_FIFO_STAT_TXRX_FFS_MASK (LANTIQ_USIF_FIFO_STAT_RX_FFS |\
	LANTIQ_USIF_FIFO_STAT_TX_FFS)

#define LANTIQ_USIF_TXD_SB                 0x48

#define LANTIQ_USIF_TXD_SB_TX_CLS          0x00000007
#define LANTIQ_USIF_TXD_SB_TX_CLS_S        0
#define LANTIQ_USIF_TXD_SB_TX_BE           0x00000030
#define LANTIQ_USIF_TXD_SB_TX_BE_S         4
#define LANTIQ_USIF_TXD_SB_XME             0x00000080

/* TX class number */
enum {
	LANTIQ_USIF_TXD_CLASS0 = 0,
	LANTIQ_USIF_TXD_CLASS1,
	LANTIQ_USIF_TXD_CLASS2,
	LANTIQ_USIF_TXD_CLASS3,
	LANTIQ_USIF_TXD_CLASS4,
	LANTIQ_USIF_TXD_CLASS5,
	LANTIQ_USIF_TXD_CLASS6,
	LANTIQ_USIF_TXD_CLASS7,
};

/* BPI byte enable */
enum {
	LANTIQ_USIF_TXD_BE4 = 0,
	LANTIQ_USIF_TXD_BE1,
	LANTIQ_USIF_TXD_BE2,
	LANTIQ_USIF_TXD_BE3,
};

#define LANTIQ_USIF_DPLUS_CTRL                 0x2c /* DPLUS control register */

#define LANTIQ_USIF_DPLUS_CTRL_SET_MASK        0x00000080
#define LANTIQ_USIF_DPLUS_CTRL_RX_MASK         0x0000ff00
#define LANTIQ_USIF_DPLUS_CTRL_RX_MASK_S       8
#define LANTIQ_USIF_DPLUS_CTRL_RX_MASK_OFFSET  LANTIQ_USIF_DPLUS_CTRL_RX_MASK_S
#define LANTIQ_USIF_DPLUS_CTRL_TX_EN           0x00400000
#define LANTIQ_USIF_DPLUS_CTRL_TX_DIS          0x00800000

#define LANTIQ_USIF_DPLUS_STAT                 0x4c /* DPLUS Status register */

#define LANTIQ_USIF_DPLUS_STAT_RX_CLS          0x00000007
#define LANTIQ_USIF_DPLUS_STAT_RX_CLS_S        0
#define LANTIQ_USIF_DPLUS_STAT_RX_BE           0x00000030
#define LANTIQ_USIF_DPLUS_STAT_RX_BE_S         4
#define LANTIQ_USIF_DPLUS_STAT_RME             0x00000080
#define LANTIQ_USIF_DPLUS_STAT_RX_MASK         0x0000ff00
#define LANTIQ_USIF_DPLUS_STAT_RX_MASK_S       8
#define LANTIQ_USIF_DPLUS_STAT_TX_EN           0x00400000
#define LANTIQ_USIF_DPLUS_STAT_TX_ACT          0x00800000
#define LANTIQ_USIF_DPLUS_STAT_RX_EN           0x01000000
#define LANTIQ_USIF_DPLUS_STAT_RX_FIFO_EMPTY   0x80000000

/* Transmit Data Register, range 0x4000-0x7FFF */
#define LANTIQ_USIF_TXD                  0x40000
/* Receive Data Register, range 0x8000-0xBFFF */
#define LANTIQ_USIF_RXD                  0x80000

/* USIF Interrupt, DMA and CRC Registers */

/* Raw Interrupt Status Register for debug */
#define LANTIQ_USIF_RIS                  0x80
/* Interrupt Mask Control Register, enable/disable */
#define LANTIQ_USIF_IMSC                 0x84
/* Masked Interrupt Status Register, status */
#define LANTIQ_USIF_MIS                  0x88
/* Interrupt Clear Register, clear */
#define LANTIQ_USIF_ICR                  0x98
/* Interrupt Set Register for debug */
#define LANTIQ_USIF_ISR                  0x90

/* Interrupt flags (Register: IMSC, MIS, RIS, ICR and ISR) */
#define LANTIQ_USIF_INT_RX_LSREQ         0x00000001
#define LANTIQ_USIF_INT_RX_SREQ          0x00000002
#define LANTIQ_USIF_INT_RX_LBREQ         0x00000004
#define LANTIQ_USIF_INT_RX_BREQ          0x00000008
#define LANTIQ_USIF_INT_TX_LSREQ         0x00000010
#define LANTIQ_USIF_INT_TX_SREQ          0x00000020
#define LANTIQ_USIF_INT_TX_LBREQ         0x00000040
#define LANTIQ_USIF_INT_TX_BREQ          0x00000080
#define LANTIQ_USIF_INT_TX_REQ           (LANTIQ_USIF_INT_TX_BREQ |\
	LANTIQ_USIF_INT_TX_SREQ)
#define LANTIQ_USIF_INT_AB               0x00000100
#define LANTIQ_USIF_INT_WKP              0x00000200
#define LANTIQ_USIF_INT_DATA_ALL         0x000000af
#define LANTIQ_USIF_INT_DATA_ALL_S       0


/* Error interrupt flags */
#define LANTIQ_USIF_INT_RXUR             0x00000800
#define LANTIQ_USIF_INT_RXOF             0x00001000
#define LANTIQ_USIF_INT_TXUR             0x00002000
#define LANTIQ_USIF_INT_TXOF             0x00004000
#define LANTIQ_USIF_INT_PE               0x00008000
#define LANTIQ_USIF_INT_FE               0x00010000
#define LANTIQ_USIF_INT_PHE              0x00020000
#define LANTIQ_USIF_INT_CRC              0x00080000
#define LANTIQ_USIF_INT_SLIP             0x00100000
#define LANTIQ_USIF_INT_MC               0x00200000
#define LANTIQ_USIF_INT_ERR_ALL          0x003BF800
#define LANTIQ_USIF_INT_ERR_ALL_s        11

/* Status interrupt flags */
#define LANTIQ_USIF_INT_FCO             0x00800000
#define LANTIQ_USIF_INT_FCI             0x01000000
#define LANTIQ_USIF_INT_DTR             0x02000000
#define LANTIQ_USIF_INT_DSR             0x04000000
#define LANTIQ_USIF_INT_DCD             0x08000000
#define LANTIQ_USIF_INT_RI              0x10000000
#define LANTIQ_USIF_INT_TX_FIN          0x20000000
#define LANTIQ_USIF_INT_FRM_PAUSE       0x40000000
#define LANTIQ_USIF_INT_TMO             0x80000000
#define LANTIQ_USIF_INT_STA_ALL         0xff800000
#define LANTIQ_USIF_INT_STA_ALL_S       23
#define LANTIQ_USIF_INT_STA_CLR_ALL     0xff800000

#define LANTIQ_USIF_INT_ALL       \
	(LANTIQ_USIF_INT_DATA_ALL | LANTIQ_USIF_INT_ERR_ALL |\
	LANTIQ_USIF_INT_STA_ALL | LANTIQ_USIF_INT_AB | LANTIQ_USIF_INT_WKP)

#define LANTIQ_USIF_INT_CLR_ALL  LANTIQ_USIF_INT_ALL

/* Interrupt setup flags for USIF TX */
#define LANTIQ_USIF_INT_TRANSMIT                  \
	(LANTIQ_USIF_INT_TX_BREQ | LANTIQ_USIF_INT_TX_SREQ)

/* Interrupt setup flags for USIF RX */
#define LANTIQ_USIF_INT_RECEIVE                   \
	(LANTIQ_USIF_INT_RX_BREQ | LANTIQ_USIF_INT_RX_LBREQ | \
	LANTIQ_USIF_INT_RX_SREQ | LANTIQ_USIF_INT_RX_LSREQ)

/* Interrupt mask for the error interrupt */
#define LANTIQ_USIF_INT_ERR_MASK                   \
	(LANTIQ_USIF_INT_RXUR | LANTIQ_USIF_INT_TXUR |\
	LANTIQ_USIF_INT_TXOF | LANTIQ_USIF_INT_PHE  |\
	LANTIQ_USIF_INT_CRC)

/**
 * The following recieve interrupt source need to be checked but it
 * need not generate any interrupts. These are processed in the error
 * handling function for received characters
 */
#define LANTIQ_USIF_RX_ERR                        \
	(LANTIQ_USIF_INT_RXOF | LANTIQ_USIF_INT_PE | LANTIQ_USIF_INT_FE)

/* Macro definitions, used for reading and writing to the ports */

/* Get USIF idenitifcation register */
#define LANTIQ_USIF_GET_ID(_p)                 \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_ID))

/* Set clock Control Register */
#define LANTIQ_USIF_SET_RUN_CONTROL(_v, _p)      \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_CLC))

#define LANTIQ_USIF_CLC_CONFIG_MODE(_p)  \
	LANTIQ_USIF_SET_RUN_CONTROL((LANTIQ_USIF_CLC_CFG_STAT), (_p))

#define LANTIQ_USIF_CLC_RUN_MODE(_p)  \
	LANTIQ_USIF_SET_RUN_CONTROL((LANTIQ_USIF_CLC_RUN_STAT), (_p))

/* Clock Control Counter Set */
#define LANTIQ_USIF_SET_CLOCK_COUNTER(_v, _p)    \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_CLC_CNT))

/* Get Clock Control Counter */
#define LANTIQ_USIF_GET_CLOCK_COUNTER(p)      \
	LANTIQ_REG_R32((volatile u32 *)((p)->membase + LANTIQ_USIF_CLC_CNT))

/* Get Clock Control Status */
#define LANTIQ_USIG_GET_CLOCK_STATUS(_p)       \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_CLC_STAT))

/* Modem Status Set */
#define LANTIQ_USIF_SET_MODEM_STATUS(_v, _p)     \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_MSS_SET))

/* Modemstatus Clear */
#define LANTIQ_USIF_CLR_MODEM_STATUS(_v, _p)     \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_MSS_CLR))

/* Get Modem status */
#define LANTIQ_USIF_GET_MODEM_STATUS(_p)       \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_MSS_STAT))

/* Modem Status Control */
#define LANTIQ_USIF_MODEM_FLOW_CTRL(_v, _p)      \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_MSS_CTRL))

/* Updated the mode configuration register */
#define LANTIQ_USIF_SET_MODE_CONFIG(_v, _p)      \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_MODE_CFG))

/* Get the mode configuration register */
#define LANTIQ_USIF_GET_MODE_CONFIG(p)        \
	LANTIQ_REG_R32((volatile u32 *)((p)->membase + LANTIQ_USIF_MODE_CFG))

/* Update the protocol configuration register */
#define LANTIQ_USIF_SET_PROTOCOL_CONFIG(_v, _p)  \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_PRTC_CFG))

/* Get the protocol configuration register */
#define LANTIQ_USIF_GET_PROTOCOL_CONFIG(_p)   \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_PRTC_CFG))

/* Get the protocol status */
#define LANTIQ_USIF_GET_PROTOCOL_STATUS(_p)   \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_PRTC_STAT))

/* Set the chip select configuration register */
#define LANTIQ_USIF_SET_CHIPSELECT_CONFIG(_v, _p)  \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_CS_CFG))

/* Get the chip select configuration register */
#define LANTIQ_USIF_GET_CHIPSELECT_CONFIG(_p)  \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_CS_CFG))

/* Update the fractional divider configuration register */
#define LANTIQ_USIF_FRAC_DIV_CONFIG(_v, _p)      \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_FDIV_CFG))

/* Get the fractional divider configuration register */
#define LANTIQ_USIF_GET_FRAC_DIV(_p)           \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_FDIV_CFG))

/* Update the baudrate counter configuration register */
#define LANTIQ_USIF_BAUDRATE_COUNTER_CONFIG(_v, _p)    \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_BC_CFG))

/* Get the baudrate counter configuration register */
#define LANTIQ_USIF_GET_BAUDRATE_COUNTER(_p)      \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_BC_CFG))

/* Update the intercharacter timeout  configuration register */
#define LANTIQ_USIF_INTERCHARACTER_TIMEOUT_CONFIG(_v, _p)  \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_ICTMO_CFG))

/* Get the intercharacter timeout */
#define LANTIQ_USIF_GET_INTERCHARACTER_TIMEOUT(_p)   \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_ICTMO_CFG))

/* Update the chip select timing configuration */
#define LANTIQ_USIF_SET_CS_TIMING_CONFIG(_v, _cs, _p)    \
	LANTIQ_REG_W32((_v), (volatile u32 *)  \
	((_p)->membase + LANTIQ_USIF_CS_TIMX_CFG((_cs))))

/* Get the chip select timing configuration */
#define LANTIQ_USIF_GET_CS_TIMING_CONFIG(_p, _cs)      \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_CS_TIMX_CFG(_cs)))

/* Update the fifo configuration register */
#define LANTIQ_USIF_SET_FIFO_CONFIG(_v, _p)      \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_FIFO_CFG))

/* Get the fifo configuration */
#define LANTIQ_USIF_GET_FIFO_CONFIG(_p)        \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_FIFO_CFG))

/* Get the fifo status */
#define LANTIQ_USIF_GET_FIFO_STATUS(_p)        \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_FIFO_STAT))

/* Get RX filling level */
#define LANTIQ_USIF_GET_RX_FILL_FIFO(_p)       \
	MS(LANTIQ_USIF_GET_FIFO_STATUS((_p)), LANTIQ_USIF_FIFO_STAT_RX_FFS)

/* Get TX filling level */
#define LANTIQ_USIF_GET_TX_FILL_FIFO(_p)       \
	MS(LANTIQ_USIF_GET_FIFO_STATUS((_p)), LANTIQ_USIF_FIFO_STAT_TX_FFS)

/* Get FIFO ID <Size> */
#define LANTIQ_USIF_GET_FIFO_ID(_p)           \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_FIFO_ID))

/* Get SW control id */
#define LANTIQ_USIF_GET_SWC_ID(_p)           \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_SWCID))

/* Get TX/RX  fifo size */
#define LANTIQ_USIF_GET_TX_FIFO_SIZE(_p)      \
	((MS(LANTIQ_USIF_GET_FIFO_ID((_p)), LANTIQ_USIF_FIFO_ID_TX)))

#define LANTIQ_USIF_GET_RX_FIFO_SIZE(_p)      \
	((MS(LANTIQ_USIF_GET_FIFO_ID((_p)), LANTIQ_USIF_FIFO_ID_RX)))

/* Get the character from receive buffer */
#define LANTIQ_USIF_GET_RX_WORD(_p)           \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_RXD))

#define LANTIQ_USIF_GET_RX_CHAR(_p)          \
	((u8)LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_RXD)))

/* Update transmission data sideband register */
#define LANTIQ_USIF_CONFIG_TXD_SIDEBAND(_v, _p) \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_TXD_SB))

/* Get transmission data sideband register */
#define LANTIQ_USIF_GET_TXD_SIDEBAND_CONFIG(_p) \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_TXD_SB))

/* Put one word to transmit buffer */
#define LANTIQ_USIF_PUT_TX_WORD(_v, _p)          \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_TXD))

/* Get one word from transmit buffer */
#define LANTIQ_USIF_GET_TX_WORD(_p)          \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_TXD))

#define LANTIQ_USIF_TXD_SIDEBAND_LAST_WORD_CONFIG(_x, _p) do {               \
	LANTIQ_USIF_CONFIG_TXD_SIDEBAND(                                 \
	(LANTIQ_USIF_TXD_SB_XME | SM(LANTIQ_USIF_TXD_CLASS0, LANTIQ_USIF_TXD_SB_TX_CLS)\
	| SM((_x), LANTIQ_USIF_TXD_SB_TX_BE)), (_p));                            \
} while (0)

/* Put one byte to transmit buffer */
#define LANTIQ_USIF_PUT_TX_CHAR(_v, _p)   do {                              \
	LANTIQ_USIF_TXD_SIDEBAND_LAST_WORD_CONFIG(LANTIQ_USIF_TXD_BE1, (_p));     \
	LANTIQ_USIF_PUT_TX_WORD((u32)(_v), (_p));                               \
} while (0)


/* Put last word to transmit buffer */
#define LANTIQ_USIF_PUT_TX_LAST_WORD(_x, _v, _p)   do {                      \
	LANTIQ_USIF_TXD_SIDEBAND_LAST_WORD_CONFIG((_x), (_p));                 \
	LANTIQ_USIF_PUT_TX_WORD((u32)(_v), (_p));                              \
} while (0)

#define LANTIQ_USIF_TXD_SIDEBAND_CLS_LAST_WORD_CONFIG(_x, _y, _p) do {         \
	LANTIQ_USIF_CONFIG_TXD_SIDEBAND(                                   \
	(LANTIQ_USIF_TXD_SB_XME | SM((_x), LANTIQ_USIF_TXD_SB_TX_CLS)       \
	| SM((_y), LANTIQ_USIF_TXD_SB_TX_BE)), (_p));                         \
} while (0)

/* Put one byte to transmit buffer */
#define LANTIQ_USIF_PUT_TX_CLS_CHAR(_x, _v, _p)   do {                       \
	LANTIQ_USIF_TXD_SIDEBAND_CLS_LAST_WORD_CONFIG   \
	((_x), (LANTIQ_USIF_TXD_BE1), (_p));            \
	LANTIQ_USIF_PUT_TX_WORD((u32)(_v), (_p));                          \
} while (0)

/* Put last word to transmit buffer */
#define LANTIQ_USIF_PUT_TX_CLS_LAST_WORD(_x, _y, _v, _p)   do {         \
	LANTIQ_USIF_TXD_SIDEBAND_CLS_LAST_WORD_CONFIG((_x), (_y), (_p));  \
	LANTIQ_USIF_PUT_TX_WORD((u32)(_v), (_p));                        \
} while (0)

#define LANTIQ_USIF_SET_FIFO_CTRL(_v, _p)                  \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_FIFO_CTRL))

/* Config the Maximum Received Packet Size Register */
#define LANTIQ_USIF_CONFIG_RX_MRPS(_v, _p)       \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_MRPS_CTRL))

/* Read the Maximum Received Packet Size Register */
#define LANTIQ_USIF_GET_RX_MRPS(_p)            \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_MRPS_CTRL))

/* Set DPLUS control register, write only */
#define LANTIQ_USIF_CONFIG_DPLUS_CONTROL(_v, _p)  \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_DPLUS_CTRL))

/* Get DPLUS status */
#define LANTIQ_USIF_GET_DPLUS_STATUS(_p) \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_DPLUS_STAT))

/* Get RX byte enable */
#define LANTIQ_USIF_GET_DPLUS_RXBE(_p)         \
	MS(LANTIQ_USIF_GET_DPLUS_STATUS((_p)), LANTIQ_USIF_DPLUS_STAT_RX_BE)

/* Set Interrupt Mask Control Register */
#define LANTIQ_USIF_SET_INT_MASK(_v, _p)         \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_IMSC))

/* Get Interrupt Mask Control Register */
#define LANTIQ_USIF_GET_INT_MASK(_p)           \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_IMSC))

/* Clear Interrupt Mask Control Register */
#define LANTIQ_USIF_CLR_INT_MASK(_v, _p)           \
	LANTIQ_REG_CLR_BIT((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_IMSC))

/* Get Raw Interrupt Status, from RIS register < debug only> */
#define LANTIQ_USIF_GET_RAW_INT_STATUS(_p)     \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_RIS))

/* Get Interrupt status, from MIS Register */
#define LANTIQ_USIF_GET_INT_STATUS(_p)         \
	LANTIQ_REG_R32((volatile u32 *)((_p)->membase + LANTIQ_USIF_MIS))

/* Set Interrupt Register <debug only> */
#define LANTIQ_USIF_SET_INT(_v, _p)              \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_ISR))

/* Clear Interrupt Register */
#define LANTIQ_USIF_CLR_INT(_v, _p)              \
	LANTIQ_REG_W32((_v), (volatile u32 *)((_p)->membase + LANTIQ_USIF_ICR))

/* Structure for transmit word */
typedef union lantiq_usif_txd {
	struct {
		u8 byte[4];
	} txd_byte;
	u32 txd_word;
} LANTIQ_USIF_TXD_t;

/* Structure for receive word */
typedef union lantiq_usif_rxd {
	struct {
		u8 byte[4];
	} rxd_byte;
	u32 rxd_word;
} LANTIQ_USIF_RXD_t;


/* Helper Macros */

/*
*  Register Operation
*/
#define LANTIQ_REG_R32(_r)                    __raw_readl((volatile unsigned int *)(_r))
#define LANTIQ_REG_W32(_v, _r)               __raw_writel((_v), (volatile unsigned int *)(_r))
#define LANTIQ_REG_W32_MASK(_clr, _set, _r)   LANTIQ_REG_W32((LANTIQ_REG_R32((_r)) & ~(_clr)) | (_set), (_r)) 
#define LANTIQ_REG_R16(_r)                    __raw_readw((_r)) 
#define LANTIQ_REG_W16(_v, _r)               __raw_writew((_v), (_r)) 
#define LANTIQ_REG_W16_MASK(_clr, _set, _r)   LANTIQ_REG_W16((LANTIQ_REG_R16((_r)) & ~(_clr)) | (_set), (_r))
#define LANTIQ_REG_R8(_r)                     __raw_readb((_r))
#define LANTIQ_REG_W8(_v, _r)                __raw_writeb((_v), (_r))
#define LANTIQ_REG_W8_MASK(_clr, _set, _r)    LANTIQ_REG_W8((LANTIQ_REG_R8((_r)) & ~(_clr)) | (_set), (_r))

/*
* Register manipulation macros that expect bit field defines
* to follow the convention that an _S suffix is appended for
* a shift count, while the field mask has no suffix. Or can use
* _M as suffix
*/

/* Shift first, then mask, usually for write operation */
#define SM(_v, _f)  (((_v) << _f##_S) & (_f))

/* Mask first , then shift, usually for read operation */
#define MS(_v, _f)  (((_v) & (_f)) >> _f##_S)

/* Always report fatal error */
#define LANTIQ_KASSERT(exp, msg) do {    \
    if (unlikely(!(exp))) {      \
	        printk msg;               \
			BUG();                    \
	}                             \
} while (0)

#endif /* LTQMIPS_USIF_REG_H */

