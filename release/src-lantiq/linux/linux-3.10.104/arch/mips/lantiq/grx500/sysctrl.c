/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2011-2012 John Crispin <blogic@openwrt.org>
 *  Copyright (C) 2015 Lei Chuanhua <Chuanhua.lei@lantiq.com>
 */
#include <linux/ioport.h>
#include <linux/export.h>
#include <linux/clkdev.h>
#include <linux/spinlock.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>

#include <lantiq_soc.h>

#include "clk.h"
#include "prom.h"


/* First register set */
#define CGU_CLK_SR1	0x120 /* status */
#define CGU_CLK_CR1_A	0x124 /* Enable */
#define CGU_CLK_CR1_B	0x128 /* Disable */
/* Second register set */
#define CGU_CLK_SR2	0x130 /* status */
#define CGU_CLK_CR2_A	0x134 /* Enable */
#define CGU_CLK_CR2_B	0x138 /* Disable */

/* clock gates that we can en/disable */
/* Register 0 /module 0 */
#define CGU_VCODEC	BIT(2)
#define CGU_I2C		BIT(4) /* I2C uses reserved bit 4 */
#define CGU_DMA		BIT(5)
#define CGU_USB0	BIT(6)
#define CGU_SPI1	BIT(7)
#define CGU_SPI0	BIT(8)
#define CGU_CBM		BIT(9)
#define CGU_EBU		BIT(10)
#define CGU_LEDC	BIT(11)
#define CGU_GPTC0	BIT(12)
#define CGU_GPTC1	BIT(13)
#define CGU_GPTC2	BIT(14)
#define CGU_ASC0	BIT(17) /* ASC1 enabled by default */
#define CGU_EIP97	BIT(20)
#define CGU_EIP123	BIT(21)
#define CGU_TOE		BIT(22)
#define CGU_MPE		BIT(23)
#define CGU_TDM		BIT(25)
#define CGU_SWITCH_R	BIT(26)
#define CGU_USB1	BIT(27)
#define CGU_SWITCH_L	BIT(28)

#define CGU_MODULE0_ALL	0x1EF27FE4

/* Register 1 /module 1*/
#define CGU_PCIE_CTRL0	BIT(1)
#define CGU_PCIE_CTRL1	BIT(17)
#define CGU_PCIE_CTRL2	BIT(25)

#define CGU_MODULE1_ALL	0x02020002

#define CGU_REG_MODULE_0	0
#define CGU_REG_MODULE_1	1

#define CGU_IF_CLK		0x00C4
#define GPHY_CLK_MODE_CMOS	BIT(0)
#define GPHY_CLK_SRC		0x1C
#define GPHY_CLK_SRC_S		2
#define GPHY_CLK_SRC_XTAL	1
#define GPHY_CLK_SRC_EXT	2
#define CGU_CLK_OUT2		0xC00
#define CGU_CLK_OUT2_S		10
#define CGU_CLK_OUT1		0x3000
#define CGU_CLK_OUT1_S		12
#define CGU_CLK_OUT0		0xC000
#define CGU_CLK_OUT0_S		14

void __iomem *ltq_cgu_membase;
void __iomem *ltq_ebu_membase;
void __iomem *ltq_dmanand_membase;

/* Status */
static u32 cgu_clk_sr[] = {
	CGU_CLK_SR1,
	CGU_CLK_SR2,
};

/* Enable */
static u32 cgu_clk_cr_a[] = {
	CGU_CLK_CR1_A,
	CGU_CLK_CR2_A,
};

/* Disable */
static u32 cgu_clk_cr_b[] = {
	CGU_CLK_CR1_B,
	CGU_CLK_CR2_B,
};

/* power control register */
#define CGU_CR_EN_XRX(x)	(cgu_clk_cr_a[(x)])
#define CGU_CR_DIS_XRX(x)	(cgu_clk_cr_b[(x)])
/* power status register */
#define CGU_SR_XRX(x)		(cgu_clk_sr[(x)])

/* enable a clock gate */
static int cgu_enable(struct clk *clk)
{
	int retry = 1000000;

	if (!clk->always_on) {
		pr_debug("%s module %d position 0x%08x always on\n",
			__func__, clk->module, clk->bits);
		return 0;
	}

	if (atomic_read(&clk->refcount) == 0) {
		ltq_cgu_w32(clk->bits, CGU_CR_EN_XRX(clk->module));
		do {} while (--retry
			&& (!(ltq_cgu_r32(CGU_SR_XRX(clk->module))
			& clk->bits)));
		if (!retry)
			panic("Activating CGU module failed!\n");
		pr_debug("%s module %d position 0x%08x enabled\n",
			__func__, clk->module, clk->bits);

	} else {
		pr_debug("%s module %d position 0x%08x already enabled\n",
			__func__, clk->module, clk->bits);
	}
	atomic_inc(&clk->refcount);
	return 0;
}

/* disable a clock gate */
static void cgu_disable(struct clk *clk)
{
	int retry = 1000000;

	if (!clk->always_on) {
		pr_debug("%s module %d position 0x%08x always on\n",
			__func__, clk->module, clk->bits);
		return;
	}

	/* Consider disabling the module when powering on the system */
	if (atomic_read(&clk->refcount) > 0)
		atomic_dec(&clk->refcount);

	if (atomic_read(&clk->refcount) == 0) {
		ltq_cgu_w32(clk->bits, CGU_CR_DIS_XRX(clk->module));
		do {} while (--retry
			&& (ltq_cgu_r32(CGU_SR_XRX(clk->module)) & clk->bits));

		if (!retry)
			panic("activating CGU module failed!\n");
		pr_debug("%s module %d position 0x%08x disabled\n",
			__func__, clk->module, clk->bits);
	} else {
		pr_debug("%s module %d position 0x%08x still used\n",
			__func__, clk->module, clk->bits);
	}
}

/* manage the clock gates via CGU */
static void clkdev_add_pmu(const char *dev, const char *con,
		bool used, unsigned int module, unsigned int bits)
{
	struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk == NULL)
		return;
	clk->cl.dev_id = dev;
	clk->cl.con_id = con;
	clk->cl.clk = clk;
	clk->always_on = used;
	clk->enable = cgu_enable;
	clk->disable = cgu_disable;
	clk->module = module;
	clk->bits = bits;
	atomic_set(&clk->refcount, 0);
	/* Disable it during the initialitin. Module should enable when used */
	cgu_disable(clk);
	clkdev_add(&clk->cl);
}

/* enable a clockout source */
static int clkout0_enable(struct clk *clk)
{
	u32 val = ltq_cgu_r32(CGU_IF_CLK);

	val &= ~(3 << CGU_CLK_OUT0_S);
	val |= 2 << CGU_CLK_OUT0_S;
	ltq_cgu_w32(val, CGU_IF_CLK);
	return 0;
}

static void clkout0_disable(struct clk *clk)
{
	u32 val = ltq_cgu_r32(CGU_IF_CLK);

	val &= ~(3 << CGU_CLK_OUT0_S);
	ltq_cgu_w32(val, CGU_IF_CLK);
}

static void clkdev_add_clkout0(int used)
{
	struct clk *clk;

	clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk == NULL)
		return;
	clk->cl.dev_id = NULL;
	clk->cl.con_id = "clkout0";
	clk->cl.clk = clk;
	clk->rate = CLOCK_8M_192K;
	clk->enable = clkout0_enable;
	clk->disable = clkout0_disable;
	clk->always_on = used;
	clk->module = 0;
	clkout0_disable(clk);
	clkdev_add(&clk->cl);
}

#ifdef CONFIG_USE_EMULATOR
#define		CGU_ON	0
#else
#define		CGU_ON	1
#endif
/* bring up all register ranges that we need for basic system control */
void __init ltq_soc_init(void)
{
	struct resource res_cgu, res_ebu, res_hsnand;
	struct device_node *np_cgu =
		of_find_compatible_node(NULL, NULL, "lantiq,cgu-grx500");
	struct device_node *np_ebu =
		of_find_compatible_node(NULL, NULL, "lantiq,ebu-grx500");
	struct clk *clk = NULL;


	/* check if all the core register ranges are available */
	if (!np_cgu || !np_ebu)
		panic("Failed to load core nodes from devicetree");

	if (of_address_to_resource(np_cgu, 0, &res_cgu) ||
	    of_address_to_resource(np_ebu, 0, &res_ebu))
		panic("Failed to get core resources");

	if (!(request_mem_region(res_cgu.start, resource_size(&res_cgu),
		res_cgu.name)) ||
	    !(request_mem_region(res_ebu.start, resource_size(&res_ebu),
		res_ebu.name)))
		pr_err("Failed to request core reources");

	ltq_cgu_membase = ioremap_nocache(res_cgu.start,
		resource_size(&res_cgu));
	ltq_ebu_membase = ioremap_nocache(res_ebu.start,
		resource_size(&res_ebu));

	if (!ltq_cgu_membase || !ltq_ebu_membase)
		panic("Failed to remap core resources");

	if (of_address_to_resource(np_ebu, 1, &res_hsnand))
			panic("Failed to get hsnand resource");

	/* get mem region for hsnand */
	if (!request_mem_region(res_hsnand.start, resource_size(&res_hsnand),
			res_hsnand.name))
		pr_err("Failed to request hsnand reources");

	ltq_dmanand_membase = ioremap_nocache(res_hsnand.start,
				resource_size(&res_hsnand));

	if (!ltq_dmanand_membase)
		panic("Failed to remap hsnand resources");

	ltq_ebu_w32(ltq_ebu_r32(LTQ_EBU_BUSCON0) & ~EBU_WRDIS,
			LTQ_EBU_BUSCON0);
	/* add our generic grx500 clocks */
	clkdev_add_pmu(NULL, "vcodec", CGU_ON, CGU_REG_MODULE_0, CGU_VCODEC);
	clkdev_add_pmu("16e00000.dma", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_DMA);
	clkdev_add_pmu("1a300000.usb", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_USB0);
	clkdev_add_pmu("16900000.spi", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_SPI1);
	clkdev_add_pmu("16800000.spi", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_SPI0);
	clkdev_add_pmu("1e700000.cbm", NULL, 0, CGU_REG_MODULE_0, CGU_CBM);
	clkdev_add_pmu("16f00000.ebu", NULL, 0, CGU_REG_MODULE_0, CGU_EBU);

	clkdev_add_pmu("16d00000.sso", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_LEDC);
	/* Add dummy I2C clock */	
	clkdev_add_pmu("16a00000.i2c", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_I2C);
	clkdev_add_pmu("16b00000.gptu", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_GPTC0);
	clkdev_add_pmu("16300000.gptu", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_GPTC1);
	clkdev_add_pmu("16400000.gptu", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_GPTC2);
	clkdev_add_pmu("16700000.serial", NULL, 0, CGU_REG_MODULE_0,
		CGU_ASC0);
	clkdev_add_pmu("1e100000.eip97", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_EIP97);
	clkdev_add_pmu("1e000000.eip123", NULL, 0, CGU_REG_MODULE_0,
		CGU_EIP123);
	/* DMA3/ToE / HWCpy */
	clkdev_add_pmu("1e300000.dma", NULL, CGU_ON, CGU_REG_MODULE_0, CGU_TOE);
	clkdev_add_pmu("a0000000.mpe", NULL, CGU_ON, CGU_REG_MODULE_0, CGU_MPE);
	clkdev_add_pmu("16500000.tdm", NULL, CGU_ON, CGU_REG_MODULE_0, CGU_TDM);
	clkdev_add_pmu("1a000000.gswitch", NULL, 0, CGU_REG_MODULE_0,
		CGU_SWITCH_R);
	clkdev_add_pmu("1a500000.usb", NULL, CGU_ON, CGU_REG_MODULE_0,
		CGU_USB1);
	clkdev_add_pmu("1c000000.gswitch", NULL, 0, CGU_REG_MODULE_0,
		CGU_SWITCH_L);

	clkdev_add_pmu("18900000.pcie", "ctl", 0, CGU_REG_MODULE_1,
		CGU_PCIE_CTRL0);
	clkdev_add_pmu("18400000.pcie", "ctl", 0, CGU_REG_MODULE_1,
		CGU_PCIE_CTRL1);
	clkdev_add_pmu("18e00000.pcie", "ctl", 0, CGU_REG_MODULE_1,
		CGU_PCIE_CTRL2);

	clkdev_add_clkout0(CGU_ON);

	clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk) {
		clk->cl.dev_id = "cpu";
		clk->cl.con_id = "cpu";
		clk->cl.clk = clk;
		clk->rate = ltq_grx500_cpu_hz();
		clk->set_rate = ltq_grx500_set_cpu_hz;
		clk->get_rate = ltq_grx500_cpu_hz;
		clkdev_add(&clk->cl);
	}

	clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk) {
		clk->cl.dev_id = "fpi";
		clk->cl.con_id = "fpi";
		clk->cl.clk = clk;
		clk->rate = ltq_grx500_fpi_hz();
		clk->set_rate = ltq_grx500_set_fpi_hz;
		clk->get_rate = ltq_grx500_fpi_hz;
		clkdev_add(&clk->cl);
	}

	clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk) {
		clk->cl.dev_id = "io";
		clk->cl.con_id = "io";
		clk->cl.clk = clk;
		clk->rate = ltq_grx500_fpi_hz();
		clk->set_rate = ltq_grx500_set_fpi_hz;
		clk->get_rate = ltq_grx500_fpi_hz;
		clkdev_add(&clk->cl);
	}

	clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk) {
		clk->cl.dev_id = "ppe";
		clk->cl.con_id = "ppe";
		clk->cl.clk = clk;
		clk->rate = ltq_grx500_cbm_hz();
		clk->set_rate = ltq_grx500_set_cbm_hz;
		clk->get_rate = ltq_grx500_cbm_hz;
		clkdev_add(&clk->cl);
	}

	clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk) {
		clk->cl.dev_id = "ddr";
		clk->cl.con_id = "ddr";
		clk->cl.clk = clk;
		clk->rate = ltq_grx500_ddr_hz();
		clk->get_rate = ltq_grx500_ddr_hz;
		clkdev_add(&clk->cl);
	}
	clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	if (clk) {
		clk->cl.dev_id = "xbar";
		clk->cl.con_id = "xbar";
		clk->cl.clk = clk;
		clk->rate = ltq_grx500_ngi_hz();
		clk->set_rate = ltq_grx500_set_ngi_hz;
		clk->get_rate = ltq_grx500_ngi_hz;
		clkdev_add(&clk->cl);
	}

}
