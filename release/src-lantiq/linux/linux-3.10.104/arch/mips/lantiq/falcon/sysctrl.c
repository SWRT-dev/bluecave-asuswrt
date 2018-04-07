/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 *
 * Copyright (C) 2011 Thomas Langer <thomas.langer@lantiq.com>
 * Copyright (C) 2011 John Crispin <blogic@openwrt.org>
 */

#include <linux/ioport.h>
#include <linux/export.h>
#include <linux/clkdev.h>
#include <linux/of_address.h>
#include <linux/spinlock.h>
#include <falcon/sysctrl.h>
#include <asm/delay.h>

#include <lantiq_soc.h>

#include "../clk.h"


/* CPU0 Clock Control Register */
#define SYS1_CPU0CC		0x0040
/* Sleep Configuration Register */
#define SYS1_SCFG		0x00b0
/* CLKO Pad Control Register */
#define SYS1_CLKOC		0x00b8
/* infrastructure control register */
#define SYS1_INFRAC		0x00bc
/* HRST_OUT_N Control Register */
#define SYS1_HRSTOUTC		0x00c0

/* Chip Identification Register */
#define STATUS_CHIPID		0x000C
/* SPARE fuse register 0 */
#define STATUS_FUSE0		0x0038
/* Fuses for Analog modules */
#define STATUS_ANALOG		0x003C
/* Configuration fuses for drivers and pll */
#define STATUS_CONFIG		0x0040
/* SPARE fuse register 1 */
#define STATUS_FUSE1		0x0044

/* External PHY Control Register */
#define SYS_ETH_EXTPHYC		0x00B0
/* Datarate Control Register */
#define SYS_ETH_DRC		0x00B8
/* GMAC Multiplexer Control Register */
#define SYS_ETH_GMUXC		0x00BC
/* Datarate Status Register */
#define SYS_ETH_DRS		0x00C0

/* GPE frequency selection */
#define GPPC_OFFSET		24
#define GPEFREQ_MASK		0x00000C0
#define GPEFREQ_OFFSET		10
/* Clock status register */
#define SYSCTL_CLKS		0x0000
/* Clock enable register */
#define SYSCTL_CLKEN		0x0004
/* Clock clear register */
#define SYSCTL_CLKCLR		0x0008
/* Activation Status Register */
#define SYSCTL_ACTS		0x0020
/* Activation Register */
#define SYSCTL_ACT		0x0024
/* Deactivation Register */
#define SYSCTL_DEACT		0x0028
/* reboot Register */
#define SYSCTL_RBT		0x002c

/* clock divider bit */
#define CPU0CC_CPUDIV		0x0001

/* Enable XBAR Clockoff When All XBAR masters Clockoff */
#define SCFG_XBAR		0x00010000
/* CPU0 Clockoff On Sleep */
#define SCFG_CPU0		0x00000001

/* CLKO Pad Control Register */
#define CLKOC_OEN 		0x00000001

/* Infrastructure Control Register */
#define INFRAC_DGASPEN 		0x00000040
#define INFRAC_DGASPHYS_MASK 	0x00000030
#define INFRAC_DGASPHYS_OFFSET 	4
#define INFRAC_LIN1V5C_MASK 	0x00000007
#define INFRAC_LIN1V5EN 	0x00000008

/* External PHY Control Register */
#define EXTPHYC_CLKEN 		0x80000000

/* Data Rate Control Register */
#define DRC_xMII0_MASK 	 	0x00700000
#define DRC_xMII0_OFFSET 	20
#define DRC_xMII1_MASK		0x07000000
#define DRC_xMII1_OFFSET 	24
#define DRC_SGMII_MASK 		0x00070000
#define DRC_SGMII_OFFSET	16

/* GMAC Multiplexer Control Register */
#define GMUXC_GMAC0_MASK 	0x00000007

/* GMAC Data Rate Status register */
#define DRS_GMAC1_OFFSET 	4

#define GMAC_MASK(mac)		(GMUXC_GMAC0_MASK << (4 * (mac)))
#define GMAC_OFFSET(mac)	(DRS_GMAC1_OFFSET * (mac))

#define sysctl_w32(m, x, y)	ltq_w32((x), sysctl_membase[m] + (y))
#define sysctl_r32(m, x)	ltq_r32(sysctl_membase[m] + (x))
#define sysctl_w32_mask(m, clear, set, reg)	\
		sysctl_w32(m, (sysctl_r32(m, reg) & ~(clear)) | (set), reg)

#define status_w32(x, y)	ltq_w32((x), status_membase + (y))
#define status_r32(x)		ltq_r32(status_membase + (x))

static void __iomem *sysctl_membase[3], *status_membase;
void __iomem *ltq_sys1_membase, *ltq_ebu_membase;

static DEFINE_SPINLOCK(sysctrl_lock);

void falcon_trigger_hrst(int level)
{
	sysctl_w32(SYSCTL_SYS1, level & 1, SYS1_HRSTOUTC);
}

static inline void sysctl_wait(struct clk *clk,
		unsigned int test, unsigned int reg)
{
	int err = 1000000;

	do {} while (--err && ((sysctl_r32(clk->module, reg)
					& clk->bits) != test));
	if (!err)
		pr_err("module de/activation failed %d %08X %08X %08X\n",
			clk->module, clk->bits, test,
			sysctl_r32(clk->module, reg) & clk->bits);
}

static int sysctl_activate(struct clk *clk)
{
	sysctl_w32(clk->module, clk->bits, SYSCTL_CLKEN);
	sysctl_w32(clk->module, clk->bits, SYSCTL_ACT);
	sysctl_wait(clk, clk->bits, SYSCTL_ACTS);
	return 0;
}

static void sysctl_deactivate(struct clk *clk)
{
	sysctl_w32(clk->module, clk->bits, SYSCTL_CLKCLR);
	sysctl_w32(clk->module, clk->bits, SYSCTL_DEACT);
	sysctl_wait(clk, 0, SYSCTL_ACTS);
}

static int sysctl_clken(struct clk *clk)
{
	sysctl_w32(clk->module, clk->bits, SYSCTL_CLKEN);
	sysctl_w32(clk->module, clk->bits, SYSCTL_ACT);
	sysctl_wait(clk, clk->bits, SYSCTL_CLKS);
	return 0;
}

static void sysctl_clkdis(struct clk *clk)
{
	sysctl_w32(clk->module, clk->bits, SYSCTL_CLKCLR);
	sysctl_wait(clk, 0, SYSCTL_CLKS);
}

static void sysctl_reboot(struct clk *clk)
{
	unsigned int act;
	unsigned int bits;

	act = sysctl_r32(clk->module, SYSCTL_ACT);
	bits = ~act & clk->bits;
	if (bits != 0) {
		sysctl_w32(clk->module, bits, SYSCTL_CLKEN);
		sysctl_w32(clk->module, bits, SYSCTL_ACT);
		sysctl_wait(clk, bits, SYSCTL_ACTS);
	}
	sysctl_w32(clk->module, act & clk->bits, SYSCTL_RBT);
	sysctl_wait(clk, clk->bits, SYSCTL_ACTS);
}

static int gpe_clk_is_enabled(void)
{
	unsigned int status;

	/* if if the clock is already enabled */
	status = sysctl_r32(SYSCTL_SYS1, SYS1_INFRAC);
	if (status & (1 << (GPPC_OFFSET + 1)))
		return 1;
	return 0;
}

/* enable the ONU core */
static void falcon_gpe_enable(void)
{
	unsigned int freq;
	unsigned long flags;

	if (gpe_clk_is_enabled())
		return;

	if (status_r32(STATUS_CONFIG) == 0)
		freq = 1; /* use 625MHz on unfused chip */
	else
		freq = (status_r32(STATUS_CONFIG) &
			GPEFREQ_MASK) >>
			GPEFREQ_OFFSET;

	spin_lock_irqsave(&sysctrl_lock, flags);

	/* apply new frequency */
	sysctl_w32_mask(SYSCTL_SYS1, 7 << (GPPC_OFFSET + 1),
		freq << (GPPC_OFFSET + 2) , SYS1_INFRAC);
	udelay(1);

	/* enable new frequency */
	sysctl_w32_mask(SYSCTL_SYS1, 0, 1 << (GPPC_OFFSET + 1), SYS1_INFRAC);
	udelay(1);

	spin_unlock_irqrestore(&sysctrl_lock, flags);
}

static inline void clkdev_add_sys(const char *dev, unsigned int module,
					unsigned int bits)
{
	struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);

	clk->cl.dev_id = dev;
	clk->cl.con_id = NULL;
	clk->cl.clk = clk;
	clk->module = module;
	clk->bits = bits;
	clk->activate = sysctl_activate;
	clk->deactivate = sysctl_deactivate;
	clk->enable = sysctl_clken;
	clk->disable = sysctl_clkdis;
	clk->reboot = sysctl_reboot;
	clkdev_add(&clk->cl);
}

void __init ltq_soc_init(void)
{
	struct clk *clk;

	struct device_node *np_status =
		of_find_compatible_node(NULL, NULL, "lantiq,status-falcon");
	struct device_node *np_ebu =
		of_find_compatible_node(NULL, NULL, "lantiq,ebu-falcon");
	struct device_node *np_sys1 =
		of_find_compatible_node(NULL, NULL, "lantiq,sys1-falcon");
	struct device_node *np_syseth =
		of_find_compatible_node(NULL, NULL, "lantiq,syseth-falcon");
	struct device_node *np_sysgpe =
		of_find_compatible_node(NULL, NULL, "lantiq,sysgpe-falcon");
	struct resource res_status, res_ebu, res_sys[3];
	int i;

	/* check if all the core register ranges are available */
	if (!np_status || !np_ebu || !np_sys1 || !np_syseth || !np_sysgpe)
		panic("Failed to load core nodes from devicetree");

	if (of_address_to_resource(np_status, 0, &res_status) ||
			of_address_to_resource(np_ebu, 0, &res_ebu) ||
			of_address_to_resource(np_sys1, 0, &res_sys[0]) ||
			of_address_to_resource(np_syseth, 0, &res_sys[1]) ||
			of_address_to_resource(np_sysgpe, 0, &res_sys[2]))
		panic("Failed to get core resources");

	if ((request_mem_region(res_status.start, resource_size(&res_status),
				res_status.name) < 0) ||
		(request_mem_region(res_ebu.start, resource_size(&res_ebu),
				res_ebu.name) < 0) ||
		(request_mem_region(res_sys[0].start,
				resource_size(&res_sys[0]),
				res_sys[0].name) < 0) ||
		(request_mem_region(res_sys[1].start,
				resource_size(&res_sys[1]),
				res_sys[1].name) < 0) ||
		(request_mem_region(res_sys[2].start,
				resource_size(&res_sys[2]),
				res_sys[2].name) < 0))
		pr_err("Failed to request core reources");

	status_membase = ioremap_nocache(res_status.start,
					resource_size(&res_status));
	ltq_ebu_membase = ioremap_nocache(res_ebu.start,
					resource_size(&res_ebu));

	if (!status_membase || !ltq_ebu_membase)
		panic("Failed to remap core resources");

	for (i = 0; i < 3; i++) {
		sysctl_membase[i] = ioremap_nocache(res_sys[i].start,
						resource_size(&res_sys[i]));
		if (!sysctl_membase[i])
			panic("Failed to remap sysctrl resources");
	}
	ltq_sys1_membase = sysctl_membase[0];

	falcon_gpe_enable();

	/* get our 3 static rates for cpu, fpi and io clocks */
	clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk) {
		clk->cl.dev_id = "cpu";
		clk->cl.con_id = "cpu";
		clk->cl.clk = clk;
		clk->rate = CLOCK_400M;
		clkdev_add(&clk->cl);
	}
	clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk) {
		clk->cl.dev_id = "fpi";
		clk->cl.con_id = "fpi";
		clk->cl.clk = clk;
		clk->rate = CLOCK_100M;
		clkdev_add(&clk->cl);
	}
	clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk) {
		clk->cl.dev_id = "io";
		clk->cl.con_id = "io";
		clk->cl.clk = clk;
		clk->rate = CLOCK_200M;
		clkdev_add(&clk->cl);
	}

	/* add our clock domains */
	clkdev_add_sys("1d810000.gpio", SYSCTL_SYSETH, SYSCTRL_SYS_ETH_P0);
	clkdev_add_sys("1d810100.gpio", SYSCTL_SYSETH, SYSCTRL_SYS_ETH_P2);
	clkdev_add_sys("1e800100.gpio", SYSCTL_SYS1, SYSCTRL_SYS1_P1);
	clkdev_add_sys("1e800200.gpio", SYSCTL_SYS1, SYSCTRL_SYS1_P3);
	clkdev_add_sys("1e800300.gpio", SYSCTL_SYS1, SYSCTRL_SYS1_P4);
	clkdev_add_sys("1db01000.pad", SYSCTL_SYSETH, SYSCTRL_SYS_ETH_PADCTRL0);
	clkdev_add_sys("1db02000.pad", SYSCTL_SYSETH, SYSCTRL_SYS_ETH_PADCTRL2);
	clkdev_add_sys("1e800400.pad", SYSCTL_SYS1, SYSCTRL_SYS1_PADCTRL1);
	clkdev_add_sys("1e800500.pad", SYSCTL_SYS1, SYSCTRL_SYS1_PADCTRL3);
	clkdev_add_sys("1e800600.pad", SYSCTL_SYS1, SYSCTRL_SYS1_PADCTRL4);
	clkdev_add_sys("1e100b00.serial", SYSCTL_SYS1, SYSCTRL_SYS1_ASC1);
	clkdev_add_sys("1e100c00.serial", SYSCTL_SYS1, SYSCTRL_SYS1_ASC0);
	clkdev_add_sys("1e200000.i2c", SYSCTL_SYS1, SYSCTRL_SYS1_I2C);
	clkdev_add_sys("1e100d00.spi", SYSCTL_SYS1, SYSCTRL_SYS1_SSC0);
}


/*
 * for compatibility to external drivers from Lantiq
 * see arch/mips/include/asm/mach-lantiq/falcon/sysctrl.h
 */

unsigned int ltq_sysctl_sys_gpe_hw_is_activated(unsigned int mask)
{
	if (!gpe_clk_is_enabled())
		return 0;

	if (!ltq_sysctl_sys_gpe_is_clocked(mask))
		return 0;

	return ltq_sysctl_sys_gpe_is_activated(mask);
}
EXPORT_SYMBOL(ltq_sysctl_sys_gpe_hw_is_activated);

void ltq_sysctl_activate(int module, unsigned int mask)
{
	struct clk clk = { .module = module, .bits = mask };
	sysctl_activate(&clk);
}
EXPORT_SYMBOL(ltq_sysctl_activate);

void ltq_sysctl_deactivate(int module, unsigned int mask)
{
	struct clk clk = { .module = module, .bits = mask };
	sysctl_deactivate(&clk);
}
EXPORT_SYMBOL(ltq_sysctl_deactivate);

unsigned int ltq_sysctl_is_activated(const int module, const unsigned int mask)
{
	switch(module) {
	case SYSCTL_SYS1:
	case SYSCTL_SYSETH:
	case SYSCTL_SYSGPE:
		return (sysctl_r32(module, SYSCTL_ACTS) & mask) == mask ? 1 : 0;
	}
	return 0;
}
EXPORT_SYMBOL(ltq_sysctl_is_activated);

unsigned int ltq_sysctl_is_clocked(const int module, const unsigned int mask)
{
	switch(module) {
	case SYSCTL_SYS1:
	case SYSCTL_SYSETH:
	case SYSCTL_SYSGPE:
		return (sysctl_r32(module, SYSCTL_CLKS) & mask) == mask ? 1 : 0;
	}
	return 0;
}
EXPORT_SYMBOL(ltq_sysctl_is_clocked);

void ltq_sysctl_clken(int module, unsigned int mask)
{
	struct clk clk = { .module = module, .bits = mask };
	sysctl_clken(&clk);
}
EXPORT_SYMBOL(ltq_sysctl_clken);

void ltq_sysctl_clkdis(int module, unsigned int mask)
{
	struct clk clk = { .module = module, .bits = mask };
	sysctl_clkdis(&clk);
}
EXPORT_SYMBOL(ltq_sysctl_clkdis);

void ltq_sysctl_reboot(int module, unsigned int mask)
{
	struct clk clk = { .module = module, .bits = mask };
	sysctl_reboot(&clk);
}
EXPORT_SYMBOL(ltq_sysctl_reboot);

void ltq_sysctl_sleep_enable(int cpu0, int xbar)
{
	u32 val = 0;

	if (xbar)
		val |= SCFG_XBAR | SCFG_CPU0;

	if (cpu0)
		val |= SCFG_CPU0;

	sysctl_w32_mask(SYSCTL_SYS1,
			SCFG_CPU0 | SCFG_XBAR,
			val,
			SYS1_SCFG);
}
EXPORT_SYMBOL(ltq_sysctl_sleep_enable);

void ltq_sysctl_dgasp_cfg_set(const unsigned int enable,
			      const unsigned int hyst)
{
	unsigned long flags;

	spin_lock_irqsave(&sysctrl_lock, flags);

	if (enable)
		sysctl_w32_mask(SYSCTL_SYS1,
				INFRAC_DGASPEN | INFRAC_DGASPHYS_MASK,
				INFRAC_DGASPEN |
					((hyst << INFRAC_DGASPHYS_OFFSET) &
							INFRAC_DGASPHYS_MASK),
				SYS1_INFRAC);
	else
		sysctl_w32_mask(SYSCTL_SYS1, INFRAC_DGASPEN, 0, SYS1_INFRAC);

	spin_unlock_irqrestore(&sysctrl_lock, flags);
}
EXPORT_SYMBOL(ltq_sysctl_dgasp_cfg_set);

void ltq_sysctl_dgasp_cfg_get(unsigned int *enable, unsigned int *hyst)
{
	unsigned int infrac;

	infrac = sysctl_r32(SYSCTL_SYS1, SYS1_INFRAC);

	if (enable)
		*enable = infrac & INFRAC_DGASPEN ? 1 : 0;

	if (hyst)
		*hyst   = (infrac & INFRAC_DGASPHYS_MASK) >>
							INFRAC_DGASPHYS_OFFSET;
}
EXPORT_SYMBOL(ltq_sysctl_dgasp_cfg_get);

void ltq_sysctl_clko_enable(const unsigned int enable)
{
	sysctl_w32_mask(SYSCTL_SYS1,
			CLKOC_OEN,
			enable ? CLKOC_OEN : 0,
			SYS1_CLKOC);
}
EXPORT_SYMBOL(ltq_sysctl_clko_enable);

void ltq_sysctl_ldo1v5_cfg_set(const unsigned int enable,
			       const unsigned int control)
{
	unsigned long flags;

	spin_lock_irqsave(&sysctrl_lock, flags);

	if (enable)
		sysctl_w32_mask(SYSCTL_SYS1,
				INFRAC_LIN1V5C_MASK,
				control & INFRAC_LIN1V5C_MASK,
				SYS1_INFRAC);

	sysctl_w32_mask(SYSCTL_SYS1,
			INFRAC_LIN1V5EN,
			enable ? INFRAC_LIN1V5EN : 0,
			SYS1_INFRAC);

	spin_unlock_irqrestore(&sysctrl_lock, flags);
}
EXPORT_SYMBOL(ltq_sysctl_ldo1v5_cfg_set);

void ltq_sysctl_ldo1v5_cfg_get(unsigned int *enable,
			       unsigned int *control)
{
	unsigned int infrac;

	infrac = sysctl_r32(SYSCTL_SYS1, SYS1_INFRAC);

	if (enable)
		*enable  = infrac & INFRAC_LIN1V5EN ? 1 : 0;

	if (control)
		*control = infrac & INFRAC_LIN1V5C_MASK;
}
EXPORT_SYMBOL(ltq_sysctl_ldo1v5_cfg_get);

int ltq_sysctl_fuse_get(const unsigned int fuse, unsigned int *val)
{
	switch (fuse) {
	case SYSCTRL_FUSE_0:
		*val = status_r32(STATUS_FUSE0);
		break;
	case SYSCTRL_FUSE_1:
		*val = status_r32(STATUS_FUSE1);
		break;
	case SYSCTRL_FUSE_ANALOG:
		*val = status_r32(STATUS_ANALOG);
		break;
	case SYSCTRL_FUSE_CONFIG:
		*val = status_r32(STATUS_CONFIG);
		break;
	default:
		return -1;
	}
	return 0;
}
EXPORT_SYMBOL(ltq_sysctl_fuse_get);

void ltq_sysctl_chipid_get(unsigned int *chipid)
{
	if (chipid)
		*chipid = status_r32(STATUS_CHIPID);
}
EXPORT_SYMBOL(ltq_sysctl_chipid_get);

void ltq_sysctl_ephy_clko_set(const unsigned int enable, const unsigned int clk)
{
	unsigned int val = clk & 0x7;

	if (enable)
		val |= EXTPHYC_CLKEN;

	sysctl_w32(SYSCTL_SYSETH, val, SYS_ETH_EXTPHYC);
}
EXPORT_SYMBOL(ltq_sysctl_ephy_clko_set);

int ltq_sysctl_drc_set(const unsigned int interface, const unsigned int drc)
{
	unsigned int mask, reg;
	unsigned long flags;

	switch (drc) {
	default:
		return -1;
	case SYSCTRL_DRC_2500:
		if (interface != SYSCTRL_INTERFACE_SGMII)
			return -1;
		break;
	case SYSCTRL_DRC_200:
		if (interface == SYSCTRL_INTERFACE_SGMII)
			return -1;
		break;
	case SYSCTRL_DRC_10:
	case SYSCTRL_DRC_100:
	case SYSCTRL_DRC_1000:
		break;
	}

	switch (interface) {
	case SYSCTRL_INTERFACE_XMII0:
		mask = DRC_xMII0_MASK;
		reg  = drc << DRC_xMII0_OFFSET;
		break;
	case SYSCTRL_INTERFACE_XMII1:
		mask = DRC_xMII1_MASK;
		reg  = drc << DRC_xMII1_OFFSET;
		break;
	case SYSCTRL_INTERFACE_SGMII:
		mask = DRC_SGMII_MASK;
		reg  = drc << DRC_SGMII_OFFSET;
		break;
	default:
		return -1;
	}

	spin_lock_irqsave(&sysctrl_lock, flags);
	sysctl_w32_mask(SYSCTL_SYSETH, mask, reg & mask, SYS_ETH_DRC);
	spin_unlock_irqrestore(&sysctrl_lock, flags);

	return 0;
}
EXPORT_SYMBOL(ltq_sysctl_drc_set);

int ltq_sysctl_mac_mux_set(const unsigned int mac, const unsigned int mux)
{
	unsigned int mask;
	unsigned long flags;

	if (mac > 3)
		return -1;

	mask = GMAC_MASK(mac);

	spin_lock_irqsave(&sysctrl_lock, flags);
	sysctl_w32_mask(SYSCTL_SYSETH,
			mask,
			(mux << GMAC_OFFSET(mac)) & mask,
			SYS_ETH_GMUXC);
	spin_unlock_irqrestore(&sysctrl_lock, flags);

	return 0;
}
EXPORT_SYMBOL(ltq_sysctl_mac_mux_set);

int ltq_sysctl_mac_mux_get(const unsigned int mac, unsigned int *mux)
{
	if (mac > 3 || !mux)
		return -1;

	*mux = (sysctl_r32(SYSCTL_SYSETH, SYS_ETH_GMUXC) & GMAC_MASK(mac)) >>
							       GMAC_OFFSET(mac);

	return 0;
}
EXPORT_SYMBOL(ltq_sysctl_mac_mux_get);

int ltq_sysctl_mac_drs_get(const unsigned int mac, unsigned int *drs)
{
	if (mac > 3 || !drs)
		return -1;

	*drs = (sysctl_r32(SYSCTL_SYSETH, SYS_ETH_DRS) & GMAC_MASK(mac)) >>
							       GMAC_OFFSET(mac);

	return 0;
}
EXPORT_SYMBOL(ltq_sysctl_mac_drs_get);
