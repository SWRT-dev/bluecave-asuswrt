/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (C) 2015 Zhu YiXin<yixin.zhu@lantiq.com>
 */

#ifndef __HWMCPY_ADDR_H__
#define __HWMCPY_ADDR_H__

#define MCPY_GCTRL				0x200
#define MCPY_INTERNAL_INT_EN			0x8C
#define MCPY_INTERNAL_INT_MASK			0x84
#define MCPY_INT_EN				0x88
#define MCPY_INT_MASK				0x80
#define MCPY_INT_STAT				0x90
#define MCPY_PORT_TO_CNT_0			0x300
#define MCPY_MRES_REG0_0			0x100
#define MCPY_MRES_REG1_0			0x104
#define MCPY_MIPS_CFG_0				0x240
#define MCPY_UMT_SW_MODE			0x94
#define MCPY_UMT_PERD				0xDC
#define MCPY_UMT_MSG(x)				(0x220 + (x) * 4)
#define MCPY_UMT_DEST				0x230
#define MCPY_UMT_XBASE				0x400
#define MCPY_UMT_XOFFSET			0x100
#define MCPY_UMT_XMSG(x)			(0x0 + (x) * 4)
#define MCPY_UMT_XPERIOD			0x20
#define MCPY_UMT_XDEST				0x30
#define MCPY_UMT_XSW_MODE			0x34
#define MCPY_UMT_TRG_MUX			0xE0
#define MCPY_UMT_CNT_CTRL			0xE4

#define MCPY_UMT_X_ADDR(x, off) (MCPY_UMT_XBASE + \
				((x) - 1) * MCPY_UMT_XOFFSET + (off))

#define MRES_ERROR				BIT(30)
#define MRES_DONE				BIT(31)
#define MCPY_CMD_ERR				BIT(16)
#define MCPY_LEN_ERR				BIT(17)

extern void __iomem *g_mcpy_addr_base;

#define ltq_mcpy_r32(x)		ltq_r32(g_mcpy_addr_base + (x))
#define ltq_mcpy_w32(x, y)	ltq_w32((x), g_mcpy_addr_base + (y))
#define ltq_mcpy_w32_mask(x, y, z)	\
			ltq_w32_mask((x), (y), g_mcpy_addr_base + (z))

#define PORT_TO_CNT(pid)	(MCPY_PORT_TO_CNT_0 + (pid) * 0x10)
#define PORT_MRES(pid)		(MCPY_MRES_REG1_0 + (pid) * 0x10)
#define MCPY_SET_RESPONSE(pid)	ltq_mcpy_w32(0, PORT_MRES(pid))
#define MCPY_GET_RESPONSE(pid)	ltq_mcpy_r32(PORT_MRES(pid))
#define MCPY_MIPS_CFG(x)	(MCPY_MIPS_CFG_0 + (x) * 0x10)

#define MCPY_CMD(cid, pid)	((cid) * 0x4 + (pid) * 0x10)

/* GCR Address */
#define GCR_BASE		0xB2300000
#define GCR_CUS_BASE            0xB23F0000
#define CCA_IC_MREQ0            0x18
#define GCR_CCA_IC_MREQ(id)     (GCR_CUS_BASE + CCA_IC_MREQ0 + (id) * 4)
#define GCR_REG0_BASE		0x0090
#define GCR_REG1_BASE		0x00A0
#define GCR_REG2_BASE		0x00B0
#define GCR_REG3_BASE		0x00C0
#define GCR_REG4_BASE		0x0190
#define GCR_REG5_BASE		0x01A0
#define GCR_REG0_MASK		0x0098
#define GCR_REG1_MASK		0x00A8
#define GCR_REG2_MASK		0x00B8
#define GCR_REG3_MASK		0x00C8
#define GCR_REG4_MASK		0x0198
#define GCR_REG5_MASK		0x01A8
#define GCR_CTRL_REG		(GCR_BASE + 0x10)
#define GCR_CFG_ENABLE		(GCR_BASE + 0x60)
#define GCR_UMT_REG		(GCR_BASE + GCR_REG3_BASE)
#define GCR_UMT_MASK		(GCR_BASE + GCR_REG3_MASK)

#define MCPY_DMA_RX_CID		12
#define MCPY_DMA_TX_CID		(MCPY_DMA_RX_CID + 1)
#define UMT_DMA_RX_CID		28
#define UMT_DMA_TX_CID		(UMT_DMA_RX_CID + 1)
#define MCPY_DBASE		0x10600
#define MCPY_DBASE_OFFSET	0x100
#define UMT_DBASE		0x11800
#define DMA_DESC_SIZE		16 /* 4 DWs */


#define DMA3_MASTERID           29

#endif  /*__HWMCPY_ADDR_H__ */
