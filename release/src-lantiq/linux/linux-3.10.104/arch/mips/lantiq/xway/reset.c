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

#include "../prom.h"

/* reset request register */
#define RCU_RST_REQ		0x0010
/* reset status register */
#define RCU_RST_STAT		0x0014

#define RCU_RST_STAT2		0x0024
#define RCU_RST_REQ2		0x0048

/* vr9 gphy registers */
#define RCU_GFS_ADD0_XRX200	0x0020
#define RCU_GFS_ADD1_XRX200	0x0068

#define RCU_AHB_ENDIAN		0x004C
#define RCU_VR9_BE_AHB1S	3

/* reboot bit */
#define RCU_RD_GPHY0_XRX200	BIT(31)
#define RCU_RD_SRST		BIT(30)
#define RCU_RD_GPHY1_XRX200	BIT(29)

/* xRX300 bits */
#define RCU_RD_GPHY0_XRX300	BIT(31)
#define RCU_RD_GPHY1_XRX300	BIT(29)
#define RCU_RD_GPHY2_XRX300	BIT(28)

/* xRX330 bits */
#define RCU_RD_GPHY0_XRX330	BIT(31)
#define RCU_RD_GPHY1_XRX330	BIT(29)
#define RCU_RD_GPHY2_XRX330	BIT(28)
#define RCU_RD_GPHY3_XRX330	BIT(10)

/* xRX300 gphy registers */
#define RCU_GFS_ADD0_XRX300	0x0020
#define RCU_GFS_ADD1_XRX300	0x0058
#define RCU_GFS_ADD2_XRX300	0x00AC

/* xRX330 gphy registers */
#define RCU_GFS_ADD0_XRX330	0x0020
#define RCU_GFS_ADD1_XRX330	0x0058
#define RCU_GFS_ADD2_XRX330	0x00AC
#define RCU_GFS_ADD3_XRX330	0x0264

/* usb configure registers */
#define RCU_USB0_CFG		0x0018
#define RCU_USB1_CFG		0x0034

/* usb configure register bits */
#define RCU_RD_HOST_DEV		BIT(11)
#define RCU_RD_HOST_END		BIT(10)
#define RCU_RD_DEV_END		BIT(9)

/* bits of RCU_RST_REQ */
#define RCU_RD_USB0_RESET	BIT(4)
#define RCU_RD_USB1_RESET	BIT(3)

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

/* reset / boot a gphy */
static struct ltq_xrx200_gphy_reset {
	u32 rd;
	u32 addr;
} xrx200_gphy[] = {
	{RCU_RD_GPHY0_XRX200, RCU_GFS_ADD0_XRX200},
	{RCU_RD_GPHY1_XRX200, RCU_GFS_ADD1_XRX200},
};

/* reset / boot a gphy */
static struct ltq_xrx300_gphy_reset {
	u32 rd;
	u32 addr;
} xrx300_gphy[] = {
	{RCU_RD_GPHY0_XRX300, RCU_GFS_ADD0_XRX300},
	{RCU_RD_GPHY1_XRX300, RCU_GFS_ADD1_XRX300},
	{RCU_RD_GPHY2_XRX300, RCU_GFS_ADD2_XRX300},
};

/* reset / boot a gphy */
static struct ltq_xrx330_gphy_reset {
	u32 rd;
	u32 addr;
} xrx330_gphy[] = {
	{RCU_RD_GPHY0_XRX330, RCU_GFS_ADD0_XRX330},
	{RCU_RD_GPHY1_XRX330, RCU_GFS_ADD1_XRX330},
	{RCU_RD_GPHY2_XRX330, RCU_GFS_ADD2_XRX330},
	{RCU_RD_GPHY3_XRX330, RCU_GFS_ADD3_XRX330},
};

/* reset and boot a gphy. these phys only exist on xrx200 SoC */
int xrx200_gphy_boot(struct device *dev, unsigned int id, dma_addr_t dev_addr)
{

	if (!of_device_is_compatible(ltq_rcu_np, "lantiq,rcu-xrx200")) {
		dev_err(dev, "this SoC has no GPHY\n");
		return -EINVAL;
	}

	if (of_machine_is_compatible("lantiq,vr9")) {
		struct clk *clk;
		clk = clk_get_sys("1f203000.rcu", "gphy");
		if (IS_ERR(clk))
			return PTR_ERR(clk);
		clk_enable(clk);
	}

	dev_info(dev, "booting GPHY%u firmware at %X\n", id, dev_addr);

	if (of_machine_is_compatible("lantiq,vr9")) {
		if (id > 1) {
			dev_err(dev, "%u is an invalid gphy id\n", id);
			return -EINVAL;
		}
		ltq_rcu_w32_mask(0, xrx200_gphy[id].rd, RCU_RST_REQ);
		ltq_rcu_w32(dev_addr, xrx200_gphy[id].addr);
		ltq_rcu_w32_mask(xrx200_gphy[id].rd, 0,  RCU_RST_REQ);
	} else if (of_machine_is_compatible("lantiq,ar10")) {
		if (id > 2) {
			dev_err(dev, "%u is an invalid gphy id\n", id);
			return -EINVAL;
		}
		ltq_rcu_w32_mask(0, xrx300_gphy[id].rd, RCU_RST_REQ);
		ltq_rcu_w32(dev_addr, xrx300_gphy[id].addr);
		ltq_rcu_w32_mask(xrx300_gphy[id].rd, 0,  RCU_RST_REQ);
	} else if (of_machine_is_compatible("lantiq,grx390")) {
		if (id > 3) {
			dev_err(dev, "%u is an invalid gphy id\n", id);
			return -EINVAL;
		}
		ltq_rcu_w32_mask(0, xrx330_gphy[id].rd, RCU_RST_REQ);
		ltq_rcu_w32(dev_addr, xrx330_gphy[id].addr);
		ltq_rcu_w32_mask(xrx330_gphy[id].rd, 0,  RCU_RST_REQ);
	}
	return 0;
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

void ltq_usb_config(int usb_id, int usb_cfg_state)
{
	pr_devel("%s: usb_id %d, usb_cfg_state %d\n", __func__, usb_id, usb_cfg_state);

	if (usb_id == 0)
	{
		if (usb_cfg_state == 1)
		{
			// set host mode; 0:host, 1:device
			ltq_rcu_w32_mask(RCU_RD_HOST_DEV, 0, RCU_USB0_CFG);

			// set endian
			ltq_rcu_w32_mask(0, RCU_RD_HOST_END, RCU_USB0_CFG);
			mdelay(50);
			ltq_rcu_w32_mask(RCU_RD_DEV_END, 0, RCU_USB0_CFG);
			mdelay(50);

			// reset
			ltq_rcu_w32_mask(0, RCU_RD_USB0_RESET, RCU_RST_REQ);
			mdelay(50);
			ltq_rcu_w32_mask(RCU_RD_USB0_RESET, 0, RCU_RST_REQ);
			mdelay(50);
		}
		else
		{
			// clear host mode
			ltq_rcu_w32_mask(RCU_RD_HOST_DEV, RCU_RD_HOST_DEV, RCU_USB0_CFG);
		}
	}
	else
	{
		if (usb_cfg_state == 1)
		{
			// set host mode; 0:host, 1:device
			ltq_rcu_w32_mask(RCU_RD_HOST_DEV, 0, RCU_USB1_CFG);

			// set endian
			ltq_rcu_w32_mask(0, RCU_RD_HOST_END, RCU_USB1_CFG);
			mdelay(50);
			ltq_rcu_w32_mask(RCU_RD_DEV_END, 0, RCU_USB1_CFG);
			mdelay(50);

			// reset
			ltq_rcu_w32_mask(0, RCU_RD_USB1_RESET, RCU_RST_REQ);
			mdelay(50);
			ltq_rcu_w32_mask(RCU_RD_USB1_RESET, 0, RCU_RST_REQ);
			mdelay(50);
		}
		else
		{
			// clear host mode
			ltq_rcu_w32_mask(RCU_RD_HOST_DEV, RCU_RD_HOST_DEV, RCU_USB1_CFG);
		}
	}
}
EXPORT_SYMBOL_GPL(ltq_usb_config);

void ltq_endian_set(int bitn)
{
	ltq_rcu_w32_mask(0, BIT(bitn), RCU_AHB_ENDIAN);
}
EXPORT_SYMBOL_GPL(ltq_endian_set);

void ltq_endian_clr(int bitn)
{
	ltq_rcu_w32_mask(BIT(bitn), 0, RCU_AHB_ENDIAN);
}
EXPORT_SYMBOL_GPL(ltq_endian_clr);

static void ltq_xbar_ahb1s_big_endian(void)
{
	ltq_endian_set(RCU_VR9_BE_AHB1S);
}

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

void ltq_rst_init(void)
{
	reset_dev.of_node = of_find_compatible_node(NULL, NULL,
				"lantiq,rcu-xway");
	if (!reset_dev.of_node)
		reset_dev.of_node = of_find_compatible_node(NULL, NULL,
					"lantiq,rcu-xrx200");
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

	ltq_rcu_np = of_find_compatible_node(NULL, NULL, "lantiq,rcu-xway");
	if (!ltq_rcu_np)
		ltq_rcu_np = of_find_compatible_node(NULL, NULL,
							"lantiq,rcu-xrx200");

	/* check if all the reset register range is available */
	if (!ltq_rcu_np)
		panic("Failed to load reset resources from devicetree");

	if (of_address_to_resource(ltq_rcu_np, 0, &res))
		panic("Failed to get rcu memory range");

	if (request_mem_region(res.start, resource_size(&res), res.name) < 0)
		pr_err("Failed to request rcu memory");

	ltq_rcu_membase = ioremap_nocache(res.start, resource_size(&res));
	if (!ltq_rcu_membase)
		panic("Failed to remap core memory");

	_machine_restart = ltq_machine_restart;
	_machine_halt = ltq_machine_halt;
	pm_power_off = ltq_machine_power_off;
	if (of_machine_is_compatible("lantiq,vr9"))
		ltq_xbar_ahb1s_big_endian();
	return 0;
}

arch_initcall(mips_reboot_setup);
