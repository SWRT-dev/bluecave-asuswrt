/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2010 John Crispin <blogic@openwrt.org>
 *  Copyright (C) 2014 Lei Chuanhua <Chuanhua.lei@lantiq.com>
 */

#ifndef _LTQ_SOC_H
#define _LTQ_SOC_H

#include <lantiq.h>
#include <linux/pinctrl/pinctrl.h>

/* GCR_CUS are 64 bit register which needs special handling */
#ifdef CONFIG_CPU_BIG_ENDIAN
#define ltq_wk_r32(reg)		__raw_readl((void *)(((u32)reg)^4))
#define ltq_wk_w32(val, reg)	__raw_writel((val), (void *)(((u32)reg)^4))
#else
#define ltq_wk_r32(reg)		ltq_r32((reg))
#define ltq_wk_w32(val, reg)	ltq_w32((val), (reg))
#endif /* CONFIG_CPU_BIG_ENDIAN */

/* Chip IDs */
#define SOC_ID_GRX562		0x020
#define SOC_ID_GRX582		0x021
#define SOC_ID_GRX583		0x022
#define SOC_ID_GRX580		0x023
#define SOC_ID_GRX584		0x024
#define SOC_ID_GRX585		0x025
#define SOC_ID_GRX3506		0x026
#define SOC_ID_GRX3508		0x028
#define SOC_ID_IRX200		0x02F

/* SoC Types */
#define SOC_TYPE_GRX500		0x09

static inline int ltq_is_grx500(void)
{
	return (ltq_get_soc_type() == SOC_TYPE_GRX500);
}

/* BOOT_SEL - find what boot media we have */
#define BS_EXT_ROM		0x0
#define BS_FLASH		0x1
#define BS_MII0			0x2
#define BS_PCI			0x3
#define BS_UART1		0x4
#define BS_SPI			0x5
#define BS_NAND			0x6
#define BS_RMII0		0x7

/* helpers used to access the cgu and xbar register */
#define ltq_cgu_w32(x, y)	ltq_w32((x), ltq_cgu_membase + (y))
#define ltq_cgu_r32(x)		ltq_r32(ltq_cgu_membase + (x))
extern __iomem void *ltq_cgu_membase;

/* helpers used to access the rcu */
void ltq_rcu_w32(uint32_t val, uint32_t reg_off);
uint32_t ltq_rcu_r32(uint32_t reg_off);
void ltq_rcu_w32_mask(uint32_t clr, uint32_t set, uint32_t reg_off);

void ltq_endian_set(int bitn);
void ltq_endian_clr(int bitn);
void ltq_pcie_clk_out(int bitn);

void ltq_usb_phy_reset(unsigned int bit);
void ltq_usb0_port_endian_set(unsigned int clr, unsigned int set);
void ltq_usb1_port_endian_set(unsigned int clr, unsigned int set);
/*
 * GCMP Specific definitions
 */
#define GCMP_BASE_ADDR			0x12300000
#define GCMP_ADDRSPACE_SZ		(256 * 1024)

/*
 * GIC Specific definitions
 */
#define GIC_BASE_ADDR			(0x12320000)
#define GIC_ADDRSPACE_SZ		(128 * 1024)

/*
 * CPC Specific definitions
 */
#define CPC_BASE_ADDR			0x12310000

/*
 * during early_printk no ioremap is possible
 * lets use KSEG1 instead
 */
#define LTQ_ASC0_BASE_ADDR	0x16600000
#define LTQ_EARLY_ASC		KSEG1ADDR(LTQ_ASC0_BASE_ADDR)

/* EBU - external bus unit */
#define LTQ_EBU_BUSCON0		0x0060
#define LTQ_EBU_PCC_CON		0x0090
#define LTQ_EBU_PCC_IEN		0x00A4
#define LTQ_EBU_PCC_ISTAT	0x00A0
#define LTQ_EBU_BUSCON1		0x0064
#define LTQ_EBU_ADDRSEL1	0x0024
#define EBU_WRDIS		0x80000000

/* WDT */
#define LTQ_RST_CAUSE_WDTRST	0x20

/* MPS - multi processor unit (voice) */
#define LTQ_MPS_BASE_ADDR	(KSEG1 + 0x1F107000)
#define LTQ_MPS_CHIPID		((u32 __iomem *)(LTQ_MPS_BASE_ADDR + 0x0344))
#define LTQ_FUSE_ID_CFG 	((u32 __iomem *)(LTQ_MPS_BASE_ADDR + 0x0350))

/* allow the ethernet driver to load a flash mapped mac addr */
const u8 *ltq_get_eth_mac(void);

int ltq_reset_get_status(void);

/* allow the gpio and pinctrl drivers to talk to eachother */
extern int pinctrl_xrx500_get_range_size(int id);
extern void pinctrl_xrx500_add_gpio_range(struct pinctrl_gpio_range *range);

#endif /* _LTQ_SOC_H */
