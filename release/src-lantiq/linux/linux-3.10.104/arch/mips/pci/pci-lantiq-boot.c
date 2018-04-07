/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2011~2013 Lei Chuanhua <chuanhua.lei@lantiq.com>
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/of_platform.h>
#include <linux/of_gpio.h>
#include <linux/of_irq.h>
#include <linux/of_pci.h>

#include <linux/gpio.h>
#include <asm/addrspace.h>

#include <lantiq.h>
#include <lantiq_soc.h>
#include <lantiq_irq.h>

#include "pci-lantiq-boot-dev-fw.h"


#define ltq_pci_w32(x, y)	ltq_w32((x), ltq_pci_membase + (y))
#define ltq_pci_r32(x)		ltq_r32(ltq_pci_membase + (x))

#define ltq_pci_w32_mask(clr, set, off)			\
	ltq_pci_w32((ltq_pci_r32(off) & ~(clr))		\
		| (set), (off));			\


#define ltq_pci_cfg_w32(x, y)	ltq_w32((x), ltq_pci_mapped_cfg + (y))
#define ltq_pci_cfg_r32(x)	ltq_r32(ltq_pci_mapped_cfg + (x))

#define MS(_v, _f)  (((_v) & (_f)) >> _f##_S)
#define SM(_v, _f)  (((_v) << _f##_S) & (_f))


/* Bonding daughter board IDSEL 30, family board IDSEL 29 */
#define PCI_BOOT_DEV_IDSEL			30
/*
 * FPI ==> PCI MEM address mapping
 * base: 0xb8000000 == > 0x1e000001
 * size: 8x4M = 32M
 */
#define PCI_BOOT_FPI_BASE			0x1E000001
#define PCI_BOOT_SRAM_BASE			0x1F000000
#define PCI_BOOT_BLOCK_SIZE			0x00400000

/* The following three window base must match PCI device outbound
   window configuration */
#define PCI_INBOUND_WINDOW_ONE_BASE		0xB8000000
#define PCI_INBOUND_WINDOW_TWO_BASE		0xB9000000
#define PCI_INBOUND_WINDOW_THREE_BASE		0xB9400000

#define PCI_INBOUND_WINDOW_ONE_16MB_DDR_BASE	0x01000000
#define PCI_INBOUND_WINDOW_ONE_14MB_DDR_BASE	0x0e000000

#define PCI_INBOUND_WINDOW_TWO_SRAM_BASE	0x1f000000
#define PCI_INBOUND_WINDOW_THREE_FPI_BASE	0x1e000000

#define PCI_BOOT_DEV_BAR2_BASE			0x10000000

#define PCI_DEV_PPE_ADDR			0xb8220000 /* 0x1e220000 */
#define PCI_DEV_PPE_LOCAL_ADDR			0xbe220000
/*
 * Access PCI device MPS register 0xbf2001c0 which is equal to 0x1f2001c0
 * ->host side  0xb92001c0 as the miniloader entry address 0xbe220000
 */
#define PCI_BOOT_DEV_MPS_ENTRY			(u32 *)(0xb92001c0)

/*
 * Program PCI device PCI_RDY bit so that bootrom can start miniloader
 * PCI device RDY is in 0xbf203028 which mapped into host side as 0xb9203028
 */
#define PCI_BOOT_DEV_RDY			(u32 *)(0xb9203028)
#define PCI_BOOT_READY				0x00000001

#define PCI_BOOT_POR_OUT			(u32 *)(0xbe100b10)
#define PCI_BOOT_PORT_RST			0x00008000


/* PCI Host Controller Register */
#define PCI_CLK_CTRL				0x0000

#define PCI_CLK_CTRL_FPI_CLK_DIS		BIT(0)
#define PCI_CLK_CTRL_EBU_PCI_SWITCH_EN		BIT(1)
#define PCI_CLK_CTRL_FIXED_ARB_EN		BIT(2)
#define PCI_CLK_CTRL_FPI_NORMAL_ACK		BIT(3)

#define PCI_MOD					0x0030
#define PCI_MOD_CFG_OK				BIT(24)

#define PCI_BAR11MASK				0x0044
#define PCI_BAR12MASK				0x0048
#define PCI_BAR13MASK				0x004C

#define PCI_ADDR_MAP11				0x0064
#define PCI_ADDR_MAP12				0x0068
#define PCI_ADDR_MAP13				0x006C
#define PCI_ADDR_MAP_SUPERVISOR			BIT(0)

#define PCI_ARB					0x0080

#define PCI_ARB_INTERNAL_EN			0x00000001
#define PCI_ARB_FIXED_EN			0x00000002
#define PCI_ARB_MASTER_REQ0			0x0000000C
#define PCI_ARB_MASTER_REQ0_S			2
#define PCI_ARB_MASTER_REQ1			0x00000030
#define PCI_ARB_MASTER_REQ1_S			4
#define PCI_ARB_MASTER_REQ2			0x000000C0
#define PCI_ARB_MASTER_REQ2_S			6
#define PCI_ARB_PCI_MASTER_REQ0			0x00000300
#define PCI_ARB_PCI_MASTER_REQ0_S		8
#define PCI_ARB_PCI_MASTER_REQ1			0x00000C00
#define PCI_ARB_PCI_MASTER_REQ1_S		10
#define PCI_ARB_PCI_MASTER_REQ2			0x00003000
#define PCI_ARB_PCI_MASTER_REQ2_S		12
#define PCI_ARB_PCI_PORT_ARB			0x000F0000
#define PCI_ARB_PCI_PORT_ARB_S			16
#define PCI_ARB_PCI_NOT_READY			0x00100000
#define PCI_ARB_PCI_NO_FRM			0x00200000
#define PCI_ARB_EBU_IDLE			0x00400000
#define PCI_ART_PCI_IDLE			\
	(PCI_ARB_PCI_NOT_READY | PCI_ARB_PCI_NO_FRM)

#define PCI_FPI_ADDR_MAP0			0x00C0
#define PCI_FPI_ADDR_MAP1			0x00C4
#define PCI_FPI_ADDR_MAP2			0x00C8
#define PCI_FPI_ADDR_MAP3			0x00CC
#define PCI_FPI_ADDR_MAP4			0x00D0
#define PCI_FPI_ADDR_MAP5			0x00D4
#define PCI_FPI_ADDR_MAP6			0x00D8
#define PCI_FPI_ADDR_MAP7			0x00DC
#define PCI_FPI_BURST_LENGTH			0x00E8
#define PCI_FPI_RD_BURST_LEN		0x00000003
#define PCI_FPI_RD_BURST_LEN_S		0
#define PCI_FPI_WR_BURST_LEN		0x00000300
#define PCI_FPI_WR_BURST_LEN_S		8

enum {
	PCI_FPI_BURST_LEN1 = 0,
	PCI_FPI_BURST_LEN2,
	PCI_FPI_BURST_LEN4,
	PCI_FPI_BURST_LEN8,
};

/* Configuration space */
#define PCI_CMD				0x0004
#define PCI_CMD_IO_EN			BIT(0)
#define PCI_CMD_MEM_EN			BIT(1)
#define PCI_CMD_MASTER_EN		BIT(2)
#define PCI_BAR1			0x0010
#define PCI_BAR2			0x0014
#define PCI_BAR3			0x0018


static __iomem void *ltq_pci_mapped_cfg;
static __iomem void *ltq_pci_membase;

#define PCI_BOOT_VER "2.0.0"

static int pci_dev_rst(struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	int reset_gpio;

	reset_gpio = of_get_named_gpio(node, "gpio-reset", 0);
	if (gpio_is_valid(reset_gpio)) {
		int ret = devm_gpio_request(&pdev->dev,
						reset_gpio, "pci-reset");
		if (ret) {
			dev_err(&pdev->dev,
				"failed to request gpio %d\n", reset_gpio);
			return ret;
		}
		gpio_direction_output(reset_gpio, 1);
		__gpio_set_value(reset_gpio, 1);
		mdelay(1);
		if (gpio_is_valid(reset_gpio)) {
			__gpio_set_value(reset_gpio, 0);
			wmb();
			mdelay(5);
			__gpio_set_value(reset_gpio, 1);
				smp_wmb();
			mdelay(1);
		}
		return 0;
	}
	return -EIO;
}

static void pci_host_por_out(void)
{
	u32 reg;
	reg = ltq_r32(PCI_BOOT_POR_OUT);
	reg |= PCI_BOOT_PORT_RST;
	ltq_w32(reg, PCI_BOOT_POR_OUT);
	mdelay(5);
}

#define CGU_PCI_CR		0x0038
#define PCI_DELAY_SHIFT		21
#define PCI_DELAY_MASK		(0x7 << PCI_DELAY_SHIFT)
#define LTQ_PCI_RESET_DOMAIN	BIT(13)

static int pci_host_init(struct platform_device *pdev)
{
	u32 reg;
	u32 devaddr;
	struct device_node *node = pdev->dev.of_node;
	const __be32 *bus_clk, *delay_hi;
	struct clk *clk_pci, *clk_external;

	/* get our clocks */
	clk_pci = clk_get(&pdev->dev, NULL);
	if (IS_ERR(clk_pci)) {
		dev_err(&pdev->dev, "failed to get pci clock\n");
		return PTR_ERR(clk_pci);
	}

	clk_external = clk_get(&pdev->dev, "external");
	if (IS_ERR(clk_external)) {
		clk_put(clk_pci);
		dev_err(&pdev->dev, "failed to get external pci clock\n");
		return PTR_ERR(clk_external);
	}
	if (of_find_property(node, "lantiq,external-clock", NULL))
		clk_enable(clk_external);
	else
		clk_disable(clk_external);

	/* read the bus speed that we want */
	bus_clk = of_get_property(node, "lantiq,bus-clock", NULL);
	if (bus_clk)
		clk_set_rate(clk_pci, *bus_clk);

	ltq_hw_reset(LTQ_PCI_RESET_DOMAIN);
	/* and enable the clocks */
	clk_enable(clk_pci);

	delay_hi = of_get_property(node, "lantiq,delay-hi", NULL);
	if (delay_hi)
		ltq_cgu_w32((ltq_cgu_r32(CGU_PCI_CR) & ~PCI_DELAY_MASK)
			| *delay_hi, CGU_PCI_CR);
	udelay(1);

	reg = PCI_CLK_CTRL_EBU_PCI_SWITCH_EN | PCI_CLK_CTRL_FPI_NORMAL_ACK;
	/* Enable auto-switching between PCI and EBU, normal ack */
	ltq_pci_w32(reg, PCI_CLK_CTRL);

	/* Configuration mode, i.e. configuration is not done,
	   PCI access has to be retried */
	ltq_pci_w32_mask(PCI_MOD_CFG_OK, 0, PCI_MOD);
	smp_wmb();
	reg = ltq_pci_r32(PCI_ARB);

	/* Enable external 1 PCI master, ie, PCI device */
	reg &= ~(SM(1, PCI_ARB_PCI_PORT_ARB));

	/* Enable internal PCI master reqest */
	reg &= ~(SM(3, PCI_ARB_PCI_MASTER_REQ0));

	/*
	 * Enable EBU reqest for NOR and NAND boot on SMB and u-boot
	 * needs to put MB in reset through POR_OUT <GPIO 15>
	 * For SPI boot, we can disable it.
	 */
	reg |= (SM(3, PCI_ARB_PCI_MASTER_REQ1));

	/* reg &= ~(SM(3, PCI_ARB_PCI_MASTER_REQ1)); */

	/* Enable all external masters request */
	reg &= ~(SM(3, PCI_ARB_PCI_MASTER_REQ2));

	/* Enable internal arbiter, only host has arbiter */
	reg |= PCI_ARB_INTERNAL_EN;
	ltq_pci_w32(reg, PCI_ARB);
	smp_wmb();

	/* PCI host outbound memory window configuration to access pci
	   device register bank */
	reg = PCI_BOOT_FPI_BASE;
	ltq_pci_w32(reg, PCI_FPI_ADDR_MAP0);
	reg += PCI_BOOT_BLOCK_SIZE;
	ltq_pci_w32(reg, PCI_FPI_ADDR_MAP1);
	reg += PCI_BOOT_BLOCK_SIZE;
	ltq_pci_w32(reg, PCI_FPI_ADDR_MAP2);
	reg += PCI_BOOT_BLOCK_SIZE;
	ltq_pci_w32(reg, PCI_FPI_ADDR_MAP3);
	reg = PCI_BOOT_SRAM_BASE;
	ltq_pci_w32(reg, PCI_FPI_ADDR_MAP4);
	reg += PCI_BOOT_BLOCK_SIZE;
	ltq_pci_w32(reg, PCI_FPI_ADDR_MAP5);
	reg += PCI_BOOT_BLOCK_SIZE;
	ltq_pci_w32(reg, PCI_FPI_ADDR_MAP6);
	reg += PCI_BOOT_BLOCK_SIZE;
	ltq_pci_w32(reg, PCI_FPI_ADDR_MAP7);
	/* PCI host inbound memory window configuration for pci device
	   to access host ddr */
#if defined(CONFIG_LANTIQ_PPE_E5_OFFCHIP_BONDING)
	reg = 0x0f000008; /* BAR1 MASK, 16MB, prefetchable */
	ltq_pci_w32(reg, PCI_BAR11MASK);

	/* DDR from 16~32MMB */
	ltq_pci_w32(PCI_INBOUND_WINDOW_ONE_16MB_DDR_BASE,
		PCI_ADDR_MAP11);

#else /*  */
	reg = 0x0fe00008; /* BAR1 MASK, 2MB, prefetchable */
	ltq_pci_w32(reg, PCI_BAR11MASK);

	/* DDR from 14~16:MB */
	ltq_pci_w32(PCI_INBOUND_WINDOW_ONE_14MB_DDR_BASE,
		PCI_ADDR_MAP11);

#endif /*  */
	/* BAR1 value, it should cover PCI device bus address */
	ltq_pci_cfg_w32(PCI_INBOUND_WINDOW_ONE_BASE, PCI_BAR1);
	/* PCI host inbound memory window configuration for pci device
	   to access host register bank */
#if defined(CONFIG_LANTIQ_PPE_E5_OFFCHIP_BONDING)
	reg = 0x0fc00000; /* BAR2 MASK, 4MB, non-prefetchable */
	ltq_pci_w32(reg, PCI_BAR12MASK);

#else /*  */
	reg = 0x0f000000; /* BAR2 MASK, 16MB, non-prefetchable */
	ltq_pci_w32(reg, PCI_BAR12MASK);

#endif /*  */
	reg = PCI_INBOUND_WINDOW_TWO_SRAM_BASE;
	reg |= PCI_ADDR_MAP_SUPERVISOR;
	/* Register bank, supervisor mode */
	ltq_pci_w32(reg, PCI_ADDR_MAP12);

	/* BAR2 value, it should cover PCI device bus address */
	ltq_pci_cfg_w32(PCI_INBOUND_WINDOW_TWO_BASE, PCI_BAR2);
#if defined(CONFIG_LANTIQ_PPE_E5_OFFCHIP_BONDING)
	/* PCI host inbound memory window configuration for pci device to
	   access host PPE share buffer */
	reg = 0x0fc00000; /* BAR3 MASK, 4MB, non-prefetchable */
	ltq_pci_w32(reg, PCI_BAR13MASK);
	reg = PCI_INBOUND_WINDOW_THREE_FPI_BASE;
	reg |= PCI_ADDR_MAP_SUPERVISOR;
	/* PPE share buffer, supervisor mode */
	ltq_pci_w32(reg, PCI_ADDR_MAP13);

	/* BAR3 value, it should cover PCI device bus address */
	ltq_pci_cfg_w32(PCI_INBOUND_WINDOW_THREE_BASE, PCI_BAR3);

#endif /*  */
	reg = SM(PCI_FPI_BURST_LEN8, PCI_FPI_RD_BURST_LEN)
		| SM(PCI_FPI_BURST_LEN8, PCI_FPI_WR_BURST_LEN);
	ltq_pci_w32(reg, PCI_FPI_BURST_LENGTH);

	/* Enable bus master/IO/MEM access */
	reg = ltq_pci_cfg_r32(PCI_CMD);
	reg |= PCI_CMD_IO_EN | PCI_CMD_MEM_EN |
		PCI_CMD_MASTER_EN;
	ltq_pci_cfg_w32(reg, PCI_CMD);
	/* Configuration OK. */
	ltq_pci_w32_mask(0, PCI_MOD_CFG_OK, PCI_MOD);
	smp_wmb();
	mdelay(1);

	/* Get PCI POR <whole chip> out of reset */
	pci_host_por_out();
	/* Reset PCI device here, IMO */
	pci_dev_rst(pdev);

	/* PCI device BAR configuration */
	devaddr = (u32)ltq_pci_mapped_cfg
		+ (PCI_BOOT_DEV_IDSEL - 16) * 0x800;
	ltq_w32(PCI_BOOT_DEV_BAR2_BASE,
		(u32 *) (devaddr + 0x14));
	/* PCI device IO/memory/bus master enabled */
	reg = ltq_r32((u32 *) (devaddr + 0x04));
	reg |= PCI_CMD_IO_EN | PCI_CMD_MEM_EN | PCI_CMD_MASTER_EN;
	ltq_w32(reg, (u32 *) (devaddr + 0x04));
	udelay(10);
	return 0;
}

static int pci_host_load_dev_fw(void)
{
	int i;
	u32 reg;
	u32 devaddr;

	/* Load miniFW from host to PCI device */

	/* Device PPE share buffer address 0x1e220000 */
	devaddr = PCI_DEV_PPE_ADDR;
	for (i = 0; i < ARRAY_SIZE(pci_dev_bm_fw); i++) {
		ltq_w32(pci_dev_bm_fw[i],
			(u32 *) (devaddr + (i << 2)));
	}

	/* Read back and check if download is successful */
	for (i = 0; i < ARRAY_SIZE(pci_dev_bm_fw); i++) {
		if (ltq_r32((u32 *) (devaddr + (i << 2))) !=
			pci_dev_bm_fw[i]) {
			pr_err("%s miniloader download check failed!!!!!\n",
				__func__);
			return -1;
		}
	}
	ltq_w32(PCI_DEV_PPE_LOCAL_ADDR, PCI_BOOT_DEV_MPS_ENTRY);
	reg = ltq_r32(PCI_BOOT_DEV_RDY);
	reg |= PCI_BOOT_READY;
	ltq_w32(reg, PCI_BOOT_DEV_RDY);

	/* Disable arbiter to improve throughput */
	ltq_pci_w32_mask(PCI_MOD_CFG_OK, 0,  PCI_MOD);
	reg = ltq_pci_r32(PCI_ARB);
	reg |= PCI_ARB_PCI_PORT_ARB;
	reg &= ~PCI_ARB_INTERNAL_EN;
	ltq_pci_w32(reg, PCI_ARB);
	ltq_pci_w32_mask(0, PCI_MOD_CFG_OK, PCI_MOD);
	smp_wmb();

	/* Wait for PCI device to be reconfigured */
	mdelay(100);

	/* PCI device should be up and running, wait for PCI device
	   to identify it is ready */
	while ((ltq_r32(PCI_BOOT_DEV_RDY) & PCI_BOOT_READY) != 0)
		;

	return 0;
}

static int ltq_pci_boot_probe(struct platform_device *pdev)
{
	struct resource *res_cfg, *res_bridge;

	pci_clear_flags(PCI_PROBE_ONLY);

	res_cfg = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	res_bridge = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (!res_cfg || !res_bridge) {
		dev_err(&pdev->dev, "missing memory reources\n");
		return -EINVAL;
	}

	ltq_pci_membase = devm_ioremap_resource(&pdev->dev, res_bridge);
	if (IS_ERR(ltq_pci_membase))
		return PTR_ERR(ltq_pci_membase);

	ltq_pci_mapped_cfg = devm_ioremap_resource(&pdev->dev, res_cfg);
	if (IS_ERR(ltq_pci_mapped_cfg))
		return PTR_ERR(ltq_pci_mapped_cfg);

	pci_host_init(pdev);
	if (pci_host_load_dev_fw() != 0)
		return -1;

	pr_info("Lantiq PCI Bonding Boot Driver Version %s", PCI_BOOT_VER);

	return 0;
}

static const struct of_device_id ltq_pci_boot_match[] = {
	{ .compatible = "lantiq,pci-boot-xrx200" },
	{},
};
MODULE_DEVICE_TABLE(of, ltq_pci_boot_match);

static struct platform_driver ltq_pci_boot_driver = {
	.probe = ltq_pci_boot_probe,
	.driver = {
		.name = "pci-boot-xrx200",
		.owner = THIS_MODULE,
		.of_match_table = ltq_pci_boot_match,
	},
};

int __init pci_boot_init(void)
{
	int ret = platform_driver_register(&ltq_pci_boot_driver);
	if (ret)
		pr_info("pci-boot-xrx200: Error registering platform driver!");
	return ret;
}

arch_initcall(pci_boot_init);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lei Chuanhua, chuanhua.lei@lantiq.com");
MODULE_SUPPORTED_DEVICE("Lantiq XRX200 PCI bonding devices");
MODULE_DESCRIPTION("Lantiq XRX200 PCI bonding boot up driver");

