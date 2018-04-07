/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2010 John Crispin <blogic@openwrt.org>
 *  Copyright (C) 2013 Lei Chuanhua <chuanhua.lei@lantiq.com>
 */

#include <linux/init.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/pm.h>
#include <linux/export.h>
#include <linux/delay.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/reset-controller.h>
#include <asm/reboot.h>
#include <lantiq_soc.h>

#include "prom.h"

/* reset request register */
#define RCU_RST_REQ		0x0010
/* reset status register */
#define RCU_RST_STAT		0x0014

#define RCU_RST_STAT2		0x0024
#define RCU_RST_REQ2		0x0048

#define RCU_AHB_ENDIAN		0x8004C
#define RCU_IFMUX_CFG		0x80120

#define RCU_USB0_CFG		0x80018
#define RCU_USB1_CFG		0x80034

/* reboot bit */
#define RCU_RD_SRST		BIT(30)

/* reset cause */
#define RCU_STAT_SHIFT		26
/* boot selection */
#define RCU_BOOT_SEL(x)		((x >> 18) & 0x7)
#define RCU_BOOT_SEL_XRX200(x)	(((x >> 17) & 0xf) | ((x >> 8) & 0x10))

/* remapped base addr of the reset control unit */

/* Status */
static u32 rcu_stat[] = {
	RCU_RST_STAT,
	RCU_RST_STAT2,
};

/* request */
static u32 rcu_req[] = {
	RCU_RST_REQ,
	RCU_RST_REQ2,
};

#define RCU_STAT_REG(x)		(rcu_stat[(x)])
#define RCU_REQ_REG(x)		(rcu_req[(x)])

static void __iomem *ltq_rcu_membase;
static struct device_node *ltq_rcu_np;
static DEFINE_SPINLOCK(ltq_rcu_lock);

/* This function is used by the watchdog driver */
int ltq_reset_cause(void)
{
	u32 val = ltq_rcu_r32(RCU_RST_STAT);
	return val >> RCU_STAT_SHIFT;
}
EXPORT_SYMBOL_GPL(ltq_reset_cause);

/* This function is used by the watchdog driver */
int ltq_reset_get_status (void)
{
	u32 val = ltq_rcu_r32(RCU_RST_REQ);
	return val;
}
EXPORT_SYMBOL_GPL(ltq_reset_get_status);

/* allow platform code to find out what source we booted from */
unsigned char ltq_boot_select(void)
{
	u32 val = ltq_rcu_r32(RCU_RST_STAT);

	if (of_device_is_compatible(ltq_rcu_np, "lantiq,rcu-xrx200"))
		return RCU_BOOT_SEL_XRX200(val);

	return RCU_BOOT_SEL(val);
}


/* reset a io domain for u micro seconds */
void ltq_reset_once(unsigned int module, ulong u)
{
	ltq_rcu_w32_mask(0, module, RCU_RST_REQ);
	udelay(u);
	ltq_rcu_w32_mask(module, 0, RCU_RST_REQ);
}
EXPORT_SYMBOL_GPL(ltq_reset_once);

/* reset a io domain by hardware control instead of software control */
void ltq_hw_reset(unsigned int module)
{
	ltq_rcu_w32(module, RCU_RST_REQ);
	while (!(ltq_rcu_r32(RCU_RST_STAT) & module))
		;
}
EXPORT_SYMBOL_GPL(ltq_hw_reset);

void ltq_rcu_w32(uint32_t val, uint32_t reg_off)
{
	ltq_w32(val, ltq_rcu_membase + reg_off);
}
EXPORT_SYMBOL_GPL(ltq_rcu_w32);

uint32_t ltq_rcu_r32(uint32_t reg_off)
{
	return ltq_r32(ltq_rcu_membase + reg_off);
}
EXPORT_SYMBOL_GPL(ltq_rcu_r32);

void ltq_rcu_w32_mask(uint32_t clr, uint32_t set, uint32_t reg_off)
{
	unsigned long flags;

	spin_lock_irqsave(&ltq_rcu_lock, flags);
	ltq_rcu_w32((ltq_rcu_r32(reg_off) & ~(clr)) | (set), reg_off);
	spin_unlock_irqrestore(&ltq_rcu_lock, flags);
}
EXPORT_SYMBOL_GPL(ltq_rcu_w32_mask);

void ltq_endian_set(int bitn)
{
	ltq_rcu_w32_mask(0, BIT(bitn), RCU_AHB_ENDIAN);
}
EXPORT_SYMBOL_GPL(ltq_endian_set);

void ltq_pcie_clk_out(int bitn)
{
	ltq_rcu_w32_mask(BIT(bitn), 0, RCU_IFMUX_CFG);
}
EXPORT_SYMBOL_GPL(ltq_pcie_clk_out);

void ltq_endian_clr(int bitn)
{
	ltq_rcu_w32_mask(BIT(bitn), 0, RCU_AHB_ENDIAN);
}
EXPORT_SYMBOL_GPL(ltq_endian_clr);

void ltq_usb_phy_reset(unsigned int nbit)
{
	ltq_rcu_w32_mask(nbit, 0, RCU_RST_REQ2);
}
EXPORT_SYMBOL_GPL(ltq_usb_phy_reset);

void ltq_usb0_port_endian_set(unsigned int clr, unsigned int set)
{
	ltq_rcu_w32_mask(clr, set, RCU_USB0_CFG);
}
EXPORT_SYMBOL_GPL(ltq_usb0_port_endian_set);

void ltq_usb1_port_endian_set(unsigned int clr, unsigned int set)
{
	ltq_rcu_w32_mask(clr, set, RCU_USB1_CFG);
}
EXPORT_SYMBOL_GPL(ltq_usb1_port_endian_set);

static int ltq_assert_device(struct reset_controller_dev *rcdev,
				unsigned long id)
{
	u32 val;
	int regidx = id >> 5;
	int regbit = id & 0x1F;

	val = ltq_rcu_r32(RCU_REQ_REG(regidx));
	val |= BIT(regbit);
	ltq_rcu_w32(val, RCU_REQ_REG(regidx));

	return 0;
}

static int ltq_deassert_device(struct reset_controller_dev *rcdev,
				  unsigned long id)
{
	u32 val;
	int regidx = id >> 5;
	int regbit = id & 0x1F;

	val = ltq_rcu_r32(RCU_REQ_REG(regidx));
	val &= ~BIT(regbit);
	ltq_rcu_w32(val, RCU_REQ_REG(regidx));

	return 0;
}

static int ltq_reset_device(struct reset_controller_dev *rcdev,
			       unsigned long id)
{
	int regidx = id >> 5;
	int regbit = id & 0x1F;
	
	ltq_rcu_w32(BIT(regbit), RCU_REQ_REG(regidx));
	while (!(ltq_rcu_r32(RCU_STAT_REG(regidx)) & (BIT(regbit))))
		;
	return 0;
}

static struct reset_control_ops reset_ops = {
	.reset = ltq_reset_device,
	.assert = ltq_assert_device,
	.deassert = ltq_deassert_device,
};

static struct reset_controller_dev reset_dev = {
	.ops			= &reset_ops,
	.owner			= THIS_MODULE,
	.nr_resets		= 64,
	.of_reset_n_cells	= 1,
};

#if 1
/* NB, put it into reset state until GPHY take it out of reset 
 * it tries to fix half-way packet issues
 */
static void gphy_rst_assert(void)
{
	ltq_rcu_w32_mask(0, BIT(31), RCU_RST_REQ); /* GPHY6F */
	ltq_rcu_w32_mask(0, BIT(29), RCU_RST_REQ); /* GPHY2 */
	ltq_rcu_w32_mask(0, BIT(28), RCU_RST_REQ); /* GPHY3 */
	ltq_rcu_w32_mask(0, BIT(26), RCU_RST_REQ); /* GPHY4 */
	ltq_rcu_w32_mask(0, BIT(25), RCU_RST_REQ); /* GPHY5 */

	ltq_rcu_w32_mask(0, BIT(12), RCU_RST_REQ); /* pcie1 */
	ltq_rcu_w32_mask(0, BIT(13), RCU_RST_REQ); /* pcie2 */
	ltq_rcu_w32_mask(0, BIT(10), RCU_RST_REQ); /* pcie3 */
}

#define ETHSW_SWRES			0x000
#define FDMA_PCTRL			0xA80
#define SDMA_PCTRL			0xBC0
#if 1
#define ltq_sw_r32(base, off)		ltq_r32((u32 __iomem *)((base)|((off)*4)))
#define ltq_sw_w32(val, base, off)	ltq_w32((val), (u32 __iomem *)((base)|((off)*4)))
#else
static u32 ltq_sw_r32(u32 base, u32 off)
{
	printk("read addr: 0x%08x\n", (base)|((off)*4));
	return ltq_r32((u32 __iomem *)((base)|((off)*4)));
}
static void ltq_sw_w32(u32 val, u32 base, u32 off)
{
	printk("write addr: 0x%08x, val: 0x%08x\n", (base)|((off)*4), val);
	ltq_w32((val), (u32 __iomem *)((base)|((off)*4)));
}
#endif

/*
 * Reset switch and stop transmission on port of Switch-L/R.
 * This is to solve traffic stall due to DMA reset with packet pending.
 */
static void switch_reset_and_disable(void)
{
	u32 gswl_base = KSEG1 | 0x1c000000;
	u32 gswr_base = KSEG1 | 0x1a000000;
	int pidx;
	u32 off;
	u32 reg;
	u32 loop;

	/* Reset Switch-L */
	ltq_sw_w32(0x01, gswl_base, ETHSW_SWRES);
	loop = 1000;
	do {
		udelay(100);
	} while (ltq_sw_r32(gswl_base, ETHSW_SWRES) && --loop);
	if (!loop) {
		printk("Switch-L Reset fail. Reset ...\n");
		_machine_restart(NULL);
	}
	/* Reset Switch-R */
	ltq_sw_w32(0x01, gswr_base, ETHSW_SWRES);
	loop = 1000;
	do {
		udelay(100);
	} while (ltq_sw_r32(gswr_base, ETHSW_SWRES) && --loop);
	if (!loop) {
		printk("Switch-R Reset fail. Reset ...\n");
		_machine_restart(NULL);
	}

	/* Disable Ports of Switch-L */
	for(pidx=2; pidx<6; pidx++) {
		/* Set SDMA_PCTRL_PEN PORT disable */
		off = SDMA_PCTRL+pidx*6;
		reg = ltq_sw_r32(gswl_base, off) & ~BIT(0);
		ltq_sw_w32(reg, gswl_base, off);
		/* Set FDMA_PCTRL_EN PORT disable */
		off = FDMA_PCTRL+pidx*6;
		reg = ltq_sw_r32(gswl_base, off) & ~BIT(0);
		ltq_sw_w32(reg, gswl_base, off);
	}
	/* Disable Ports of Switch-R */
	for(pidx = 0; pidx < 16; pidx++) {
		/* Set SDMA_PCTRL_PEN PORT disable */
		off = SDMA_PCTRL+pidx*6;
		reg = ltq_sw_r32(gswr_base, off) & ~BIT(0);
		ltq_sw_w32(reg, gswr_base, off);
		/* Set FDMA_PCTRL_EN PORT disable */
		off = FDMA_PCTRL+pidx*6;
		reg = ltq_sw_r32(gswr_base, off) & ~BIT(0);
		ltq_sw_w32(reg, gswr_base, off);
	}
}
#endif

void ltq_rst_init(void)
{
	reset_dev.of_node = of_find_compatible_node(NULL, NULL,
				"lantiq,rcu-grx500");
	if (!reset_dev.of_node)
		pr_err("Failed to find reset controller node");
	if (IS_ENABLED(CONFIG_RESET_CONTROLLER))
		reset_controller_register(&reset_dev);
}

static void ltq_machine_restart(char *command)
{
	local_irq_disable();
	ltq_rcu_w32(ltq_rcu_r32(RCU_RST_REQ) | RCU_RD_SRST, RCU_RST_REQ);
	unreachable();
}

static void ltq_machine_halt(void)
{
	local_irq_disable();
	unreachable();
}

static void ltq_machine_power_off(void)
{
	local_irq_disable();
	unreachable();
}

static int __init mips_reboot_setup(void)
{
	struct resource res;


	ltq_rcu_np = of_find_compatible_node(NULL, NULL,
				"lantiq,rcu-grx500");

	/* check if all the reset register range is available */
	if (!ltq_rcu_np)
		panic("Failed to load reset resources from devicetree");

	if (of_address_to_resource(ltq_rcu_np, 0, &res))
		panic("Failed to get rcu memory range");

	if (!request_mem_region(res.start, resource_size(&res), res.name))
		pr_err("Failed to request rcu memory");

	ltq_rcu_membase = ioremap_nocache(res.start, resource_size(&res));
	if (!ltq_rcu_membase)
		panic("Failed to remap core memory");

	_machine_restart = ltq_machine_restart;
	_machine_halt = ltq_machine_halt;
	pm_power_off = ltq_machine_power_off;

	gphy_rst_assert();
	switch_reset_and_disable();

	printk("Resetting the switch and phy.\n");
	return 0;
}

postcore_initcall(mips_reboot_setup);
