/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2011-2012 John Crispin <blogic@openwrt.org>
 *  Copyright (C) 2013 Lei Chuanhua <Chuanhua.lei@lantiq.com>
 */
#include <linux/ioport.h>
#include <linux/export.h>
#include <linux/clkdev.h>
#include <linux/spinlock.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>

#include <lantiq_soc.h>

#include "../clk.h"
#include "../prom.h"

/* clock control register for legacy */
#define CGU_IFCCR	0x0018
#define CGU_IFCCR_VR9	0x0024
/* system clock register for legacy */
#define CGU_SYS		0x0010
/* pci control register */
#define CGU_PCICR	0x0034
#define CGU_PCICR_VR9	0x0038
/* ephy configuration register */
#define CGU_EPHY	0x10
/* power control register */
#define PMU_PWDCR	0x1C /* Legacy such as ar9, ase, danube */
#define PMU_PWDCR1	0x24 /* New for vr9 */

/* power status register */
#define PMU_PWDSR	0x20 /* Legacy such as ar9, ase, danube */
#define PMU_PWDSR1	0x28 /* New for vr9 */

/* vr9 tempsensor */
#define CGU_GPHY1_CFG_VR9	0x40


/* ar10 and grx390 has different implementation */

/* First register set */
#define PMU_CLK_SR	0x20 /* status */
#define PMU_CLK_CR_A	0x24 /* Enable */
#define PMU_CLK_CR_B	0x28 /* Disable */
/* Second register set */
#define PMU_CLK_SR1	0x30 /* status */
#define PMU_CLK_CR1_A	0x34 /* Enable */
#define PMU_CLK_CR1_B	0x38 /* Disable */
/* Third register set */
#define PMU_ANA_SR	0x40 /* status */
#define PMU_ANA_CR_A	0x44 /* Enable */
#define PMU_ANA_CR_B	0x48 /* Disable */

/* clock gates that we can en/disable */
#define PMU_USB0_P	BIT(0)
#define PMU_FPIS	BIT(1)
#define PMU_FPI1	PMU_FPIS
#define PMU_FPI2	PMU_FPI1
#define PMU_DFEV0	BIT(2)
#define PMU_ASE_SDIO	BIT(2) /* ASE special */
#define PMU_VO_MIPS	BIT(2)
#define PMU_DFEV1	BIT(3)
#define PMU_VODEC	BIT(3)
#define PMU_PCI		BIT(4)
#define PMU_DMA		BIT(5)
#define PMU_USB0	BIT(6)
#define PMU_ASC0	BIT(7)
#define PMU_EPHY	BIT(7) /* ase */
#define PMU_USIF	BIT(7) /* from vr9 until grx390 */
#define PMU_SPI		BIT(8)
#define PMU_DFE		BIT(9)
#define PMU_EBU		BIT(10)
#define PMU_STP		BIT(11)
#define PMU_LEDC	BIT(11) /*vr9 */
#define PMU_GPT		BIT(12)
#define PMU_AHBS	BIT(13) /* vr9 */
#define PMU_PPE_TPE	BIT(13)
#define PMU_VLYNQ	BIT(13)
#define PMU_FPI		BIT(14)
#define PMU_FPI0	BIT(14)
#define PMU_AHBM	BIT(15)
#define PMU_SDIO	BIT(16) /* danube, ar9, vr9 */
#define PMU_ASC1	BIT(17)
#define PMU_PPE_QSB	BIT(18)
#define PMU_WDT0	BIT(18)
#define PMU_PPE_SLL01	BIT(19)
#define PMU_WDT1	BIT(19)
#define PMU_DEU		BIT(20)
#define PMU_PPE_TC	BIT(21)
#define PMU_PPE_EMA	BIT(22)
#define PMU_ENET1	BIT(22)
#define PMU_ENET0	BIT(23)
#define PMU_PPE_DPLUM	BIT(23)
#define PMU_PPE_DP	BIT(23)
#define PMU_PPE_DPLUS	BIT(24)
#define PMU_TDM		BIT(25)
#define PMU_USB1_P	BIT(26) /* ar9 and vr9 */
#define PMU_USB1	BIT(27)
#define PMU_SWITCH	BIT(28)
#define PMU_PPE_TOP	BIT(29)
#define PMU_GPHY	BIT(30)
#define PMU_PCIE_CLK	BIT(31)

#define PMU1_PCIE_PHY	BIT(0) /* vr9-specific,moved in ar10/grx390 */
#define PMU1_PCIE_CTL	BIT(1)
#define PMU1_PCIE_PDI	BIT(4)
#define PMU1_PCIE_MSI	BIT(5)
#define PMU1_CKE	BIT(6)
#define PMU1_PCIE1_CTL	BIT(17)
#define PMU1_PCIE1_PDI	BIT(20)
#define PMU1_PCIE1_MSI	BIT(21)
#define PMU1_PCIE2_CTL	BIT(25)
#define PMU1_PCIE2_PDI	BIT(26)
#define PMU1_PCIE2_MSI	BIT(27)

#define PMU_ANALOG_USB0_P	BIT(0)
#define PMU_ANALOG_USB1_P	BIT(1)
#define PMU_ANALOG_PCIE0_P	BIT(8)
#define PMU_ANALOG_PCIE1_P	BIT(9)
#define PMU_ANALOG_PCIE2_P	BIT(10)
#define PMU_ANALOG_DSL_AFE	BIT(16)
#define PMU_ANALOG_DCDC_2V5	BIT(17)
#define PMU_ANALOG_DCDC_1VX	BIT(18)
#define PMU_ANALOG_DCDC_1V0	BIT(19)

#define XBAR_ALWAYS_LAST	0x430
#define XBAR_FPI_BURST_EN	0x00000002
#define XBAR_AHB_BURST_EN	0x00000004

#define pmu_w32(x, y)	ltq_w32((x), pmu_membase + (y))
#define pmu_r32(x)	ltq_r32(pmu_membase + (x))

#define xbar_w32(x, y)	ltq_w32((x), ltq_xbar_membase + (y))
#define xbar_r32(x)	ltq_r32(ltq_xbar_membase + (x))

static void __iomem *pmu_membase;
static void __iomem *ltq_xbar_membase;
void __iomem *ltq_cgu_membase;
void __iomem *ltq_ebu_membase;
void __iomem *ltq_dmanand_membase;

static u32 ifccr = CGU_IFCCR;
static u32 pcicr = CGU_PCICR;

/* Status */
static u32 pmu_clk_sr[] = {
	PMU_CLK_SR,
	PMU_CLK_SR1,
	PMU_ANA_SR,
};

/* Enable */
static u32 pmu_clk_cr_a[] = {
	PMU_CLK_CR_A,
	PMU_CLK_CR1_A,
	PMU_ANA_CR_A,
};

/* Disable */
static u32 pmu_clk_cr_b[] = {
	PMU_CLK_CR_B,
	PMU_CLK_CR1_B,
	PMU_ANA_CR_B,
};

/* power control register */
#define PWDCR(x) ((x) ? (PMU_PWDCR1) : (PMU_PWDCR))
#define PWDCR_EN_XRX(x)  (pmu_clk_cr_a[(x)])
#define PWDCR_DIS_XRX(x)  (pmu_clk_cr_b[(x)])
/* power status register */
#define PWDSR(x) ((x) ? (PMU_PWDSR1) : (PMU_PWDSR))
#define PWDSR_XRX(x)  (pmu_clk_sr[(x)])
static DEFINE_SPINLOCK(g_pmu_lock);

unsigned int ltq_xrx200_get_tempsensor(void)
{
	return ltq_cgu_r32(CGU_GPHY1_CFG_VR9);
}
EXPORT_SYMBOL_GPL(ltq_xrx200_get_tempsensor);

void ltq_xrx200_set_tempsensor(unsigned int val)
{
	ltq_cgu_w32(val, CGU_GPHY1_CFG_VR9);
}
EXPORT_SYMBOL_GPL(ltq_xrx200_set_tempsensor);

/* enable a hw clock */
static int cgu_enable(struct clk *clk)
{
	ltq_cgu_w32(ltq_cgu_r32(ifccr) | clk->bits, ifccr);
	return 0;
}

/* disable a hw clock */
static void cgu_disable(struct clk *clk)
{
	ltq_cgu_w32(ltq_cgu_r32(ifccr) & ~clk->bits, ifccr);
}

/* enable a clock gate */
static int pmu_enable(struct clk *clk)
{
	int retry = 1000000;

	if (!clk->always_on) {
		pr_debug("%s module %d position 0x%08x always on\n",
			__func__, clk->module, clk->bits);
		return 0;
	}

	if (atomic_read(&clk->refcount) == 0) {
		if (of_machine_is_compatible("lantiq,ar10")
			|| of_machine_is_compatible("lantiq,grx390")) {
			pmu_w32(clk->bits, PWDCR_EN_XRX(clk->module));
			do {} while (--retry
			&& (!(pmu_r32(PWDSR_XRX(clk->module)) & clk->bits)));
		} else {
			spin_lock(&g_pmu_lock);
			pmu_w32(pmu_r32(PWDCR(clk->module)) & ~clk->bits,
				PWDCR(clk->module));
			do {} while (--retry
				&& (pmu_r32(PWDSR(clk->module)) & clk->bits));
			spin_unlock(&g_pmu_lock);
		}
		if (!retry)
			panic("Activating PMU module failed!\n");
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
static void pmu_disable(struct clk *clk)
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
		if (of_machine_is_compatible("lantiq,ar10")
			|| of_machine_is_compatible("lantiq,grx390")) {
			pmu_w32(clk->bits, PWDCR_DIS_XRX(clk->module));
			do {} while (--retry
			&& (pmu_r32(PWDSR_XRX(clk->module)) & clk->bits));
		} else {
			spin_lock(&g_pmu_lock);
			pmu_w32(pmu_r32(PWDCR(clk->module)) | clk->bits,
				PWDCR(clk->module));
			do {} while (--retry
			&& (!(pmu_r32(PWDSR(clk->module)) & clk->bits)));
			spin_unlock(&g_pmu_lock);
		}
		if (!retry)
			panic("activating PMU module failed!\n");
		pr_debug("%s module %d position 0x%08x disabled\n",
			__func__, clk->module, clk->bits);
	} else {
		pr_debug("%s module %d position 0x%08x still used\n",
			__func__, clk->module, clk->bits);
	}
}

/* the pci enable helper */
static int pci_enable(struct clk *clk)
{
	unsigned int val = ltq_cgu_r32(ifccr);
	/* set bus clock speed */
	if (of_machine_is_compatible("lantiq,ar9") ||
			of_machine_is_compatible("lantiq,vr9")) {
		val &= ~0x1f00000;
		if (clk->rate == CLOCK_33M)
			val |= 0xe00000;
		else
			val |= 0x700000; /* 62.5M */
	} else {
		val &= ~0xf00000;
		if (clk->rate == CLOCK_33M)
			val |= 0x800000;
		else
			val |= 0x400000; /* 62.5M */
	}
	ltq_cgu_w32(val, ifccr);
	pmu_enable(clk);
	return 0;
}

/* enable the external clock as a source */
static int pci_ext_enable(struct clk *clk)
{
	ltq_cgu_w32(ltq_cgu_r32(ifccr) & ~(1 << 16), ifccr);
	ltq_cgu_w32((1 << 30), pcicr);
	return 0;
}

/* disable the external clock as a source */
static void pci_ext_disable(struct clk *clk)
{
	ltq_cgu_w32(ltq_cgu_r32(ifccr) | (1 << 16), ifccr);
	ltq_cgu_w32((1 << 31) | (1 << 30), pcicr);
}

/* enable a clockout source */
static int clkout_enable(struct clk *clk)
{
	int i;

	/* get the correct rate */
	for (i = 0; i < 4; i++) {
		if (clk->rates[i] == clk->rate) {
			int shift = 14 - (2 * clk->module);
			int enable = 7 - clk->module;
			unsigned int val = ltq_cgu_r32(ifccr);

			val &= ~(3 << shift);
			val |= i << shift;
			val |= enable;
			ltq_cgu_w32(val, ifccr);
			return 0;
		}
	}
	return -1;
}

/* manage the clock gates via PMU */
static void clkdev_add_pmu(const char *dev, const char *con,
		bool used, unsigned int module, unsigned int bits)
{
	struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);

	clk->cl.dev_id = dev;
	clk->cl.con_id = con;
	clk->cl.clk = clk;
	clk->always_on = used;
	clk->enable = pmu_enable;
	clk->disable = pmu_disable;
	clk->module = module;
	clk->bits = bits;
	atomic_set(&clk->refcount, 0);
	/* Disable it during the initialitin. Module should enable when used */
	pmu_disable(clk);
	clkdev_add(&clk->cl);
}

/* manage the clock generator */
static void clkdev_add_cgu(const char *dev, const char *con,
				unsigned int bits)
{
	struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);

	clk->cl.dev_id = dev;
	clk->cl.con_id = con;
	clk->cl.clk = clk;
	clk->enable = cgu_enable;
	clk->disable = cgu_disable;
	clk->bits = bits;
	clkdev_add(&clk->cl);
}

/* pci needs its own enable function as the setup is a bit more complex */
static unsigned long valid_pci_rates[] = {CLOCK_33M, CLOCK_62_5M, 0};

static void clkdev_add_pci(bool used)
{
	struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
	struct clk *clk_ext = kzalloc(sizeof(struct clk), GFP_KERNEL);

	/* main pci clock */
	clk->cl.dev_id = "17000000.pci";
	clk->cl.con_id = NULL;
	clk->cl.clk = clk;
	clk->rate = CLOCK_33M;
	clk->rates = valid_pci_rates;
	clk->enable = pci_enable;
	clk->disable = pmu_disable;
	clk->module = 0;
	clk->bits = PMU_PCI;
	clk->always_on = used;
	clkdev_add(&clk->cl);

	/* use internal/external bus clock */
	clk_ext->cl.dev_id = "17000000.pci";
	clk_ext->cl.con_id = "external";
	clk_ext->cl.clk = clk_ext;
	clk_ext->enable = pci_ext_enable;
	clk_ext->disable = pci_ext_disable;
	clkdev_add(&clk_ext->cl);
}

/* xway socs can generate clocks on gpio pins */
static unsigned long valid_clkout_rates[4][5] = {
	{CLOCK_32_768K, CLOCK_1_536M, CLOCK_2_5M, CLOCK_12M, 0},
	{CLOCK_40M, CLOCK_12M, CLOCK_24M, CLOCK_48M, 0},
	{CLOCK_25M, CLOCK_40M, CLOCK_30M, CLOCK_60M, 0},
	{CLOCK_12M, CLOCK_50M, CLOCK_32_768K, CLOCK_25M, 0},
};

static void clkdev_add_clkout(void)
{
	int i;

	for (i = 0; i < 4; i++) {
		struct clk *clk;
		char *name;

		name = kzalloc(sizeof("clkout0"), GFP_KERNEL);
		sprintf(name, "clkout%d", i);

		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		clk->cl.dev_id = "1f103000.cgu";
		clk->cl.con_id = name;
		clk->cl.clk = clk;
		clk->rate = 0;
		clk->rates = valid_clkout_rates[i];
		clk->enable = clkout_enable;
		clk->module = i;
		clkdev_add(&clk->cl);
	}
}

static void xbar_fpi_burst_disable(void)
{
	if (of_machine_is_compatible("lantiq,vr9")) {

		u32 reg;

		/* bit 1 as 1 --burst; bit 1 as 0 -- single */
		reg = xbar_r32(XBAR_ALWAYS_LAST);
		reg &= ~ XBAR_FPI_BURST_EN;
		xbar_w32(reg, XBAR_ALWAYS_LAST);
	}
}

static char g_pkt_base[2048] __initdata __attribute__((__aligned__(32)));
static unsigned int g_desc_base[2] __initdata __attribute__((__aligned__(32)));
static void __init ltq_dplus_clean(void)
{
#define IFX_DMA		(KSEG1 | 0x1E104100)
#define IFX_DMA_BASE	IFX_DMA
#define IFX_DMA_PS(i)	(volatile u32*)(IFX_DMA_BASE + 0x40 + 0x30 * (i))
#define IFX_DMA_PCTRL(i) (volatile u32*)(IFX_DMA_BASE + 0x44 + 0x30 * (i))
#define IFX_DMA_CS(i)	(volatile u32*)(IFX_DMA_BASE + 0x18 + 0x38 * (i))
#define IFX_DMA_CCTRL(i) (volatile u32*)(IFX_DMA_BASE + 0x1C + 0x38 * (i))
#define IFX_DMA_CDBA(i)	(volatile u32*)(IFX_DMA_BASE + 0x20 + 0x38 * (i))
#define IFX_DMA_CDLEN(i) (volatile u32*)(IFX_DMA_BASE + 0x24 + 0x38 * (i))
#define IFX_DMA_CIS(i)	(volatile u32*)(IFX_DMA_BASE + 0x28 + 0x38 * (i))
#define IFX_DMA_CIE(i)	(volatile u32*)(IFX_DMA_BASE + 0x2C + 0x38 * (i))
#define IFX_DMA_IRNEN	(volatile u32*)(IFX_DMA_BASE + 0xf4)
#define IFX_REG_R32(_r)	__raw_readl((volatile unsigned int *)(_r))
#define IFX_REG_W32(_v, _r) __raw_writel((_v), (volatile unsigned int *)(_r))
#define IFX_REG_W32_MASK(_clr, _set, _r)   IFX_REG_W32((IFX_REG_R32((_r)) \
						& ~(_clr)) | (_set), (_r))
#define PPE_REG_ADDR(x)   ((volatile unsigned int *)KSEG1ADDR( \
					0x1E180000 | (((x) + 0x4000) << 2)))

#define DMRX_PGCNT_XRX200          ((volatile unsigned int *)0xBE235854)
#define DMRX_PKTCNT_XRX200         ((volatile unsigned int *)0xBE235858)
#define DSRX_PGCNT_XRX200          ((volatile unsigned int *)0xBE235C4C)

#define DMRX_PGCNT_XRX300          PPE_REG_ADDR(0x0615)
#define DMRX_PKTCNT_XRX300         PPE_REG_ADDR(0x0616)
#define DSRX_PGCNT_XRX300          PPE_REG_ADDR(0x0713)

#define AR10_SWIP_MACRO            0x1E108000
#define AR10_SWIP_MACRO_REG(off)   ((volatile unsigned int *)KSEG1ADDR( \
						AR10_SWIP_MACRO + (off) * 4))
#define AR10_SWIP_TOP              (AR10_SWIP_MACRO | (0x0C40 * 4))
#define AR10_SWIP_TOP_REG(off)     ((volatile unsigned int *)KSEG1ADDR( \
						AR10_SWIP_TOP + (off) * 4))
/* port < 12, reg < 4 */
#define PCE_PCTRL_REG(port, reg)   AR10_SWIP_MACRO_REG(0x480 + (port) \
								* 0xA + (reg))
#define FDMA_PCTRL_REG(port)       AR10_SWIP_MACRO_REG(0xA80 + (port) * 6)
#define SDMA_PCTRL_REG(port)       AR10_SWIP_MACRO_REG(0xBC0 + (port) * 6)

	volatile unsigned int *desc_base =
	(volatile unsigned int *)KSEG1ADDR((unsigned int)g_desc_base);
	int i, j, k;

	for (i = 0; i < 6; i++)
		IFX_REG_W32_MASK(1, 0, SDMA_PCTRL_REG(i));/* stop port 0 - 5 */

	if (of_machine_is_compatible("lantiq,ar10")) {
		if ((IFX_REG_R32(DMRX_PGCNT_XRX300) & 0x00FF) == 0 &&
			(IFX_REG_R32(DSRX_PGCNT_XRX300) & 0x00FF) == 0) {
			return;
		}
	}

	if (of_machine_is_compatible("lantiq,vr9")) {
		if ((IFX_REG_R32(DMRX_PGCNT_XRX200) & 0x00FF) == 0 &&
			(IFX_REG_R32(DSRX_PGCNT_XRX200) & 0x00FF) == 0) {
			return;
		}
	}

	IFX_REG_W32(0, IFX_DMA_PS(0));
	IFX_REG_W32(0x1F68, IFX_DMA_PCTRL(0));
	IFX_REG_W32(0, IFX_DMA_IRNEN); /* disable all DMA interrupt */

	for (k = 0; k < 8; k++) {
		unsigned int imap[8] = {0, 2, 4, 6, 20, 21, 22, 23};

	i = imap[k];
	IFX_REG_W32(i, IFX_DMA_CS(0));
	IFX_REG_W32_MASK(0, 2, IFX_DMA_CCTRL(0)); /* reset channel */
	while ((IFX_REG_R32(IFX_DMA_CCTRL(0)) & 2)) /* wait reset finish */
	IFX_REG_W32(0, IFX_DMA_CIE(0)); /* disable channel interrupt */
	IFX_REG_W32(1, IFX_DMA_CDLEN(0)); /* only 1 descriptor */
	/* use local variable (array) as descriptor base address */
	IFX_REG_W32(CPHYSADDR((unsigned int)desc_base), IFX_DMA_CDBA(0));
	desc_base[0] = 0x80000000 | sizeof(g_pkt_base);
	desc_base[1] = CPHYSADDR((unsigned int)g_pkt_base);

	IFX_REG_W32_MASK(0, 1, IFX_DMA_CCTRL(0)); /* start receiving */
	while (1) {
		/* assume packet can be finished within 1000 loops */
		for (j = 0; j < 1000 && (desc_base[0]&0x80000000) != 0; j++) {
		}
		if ((desc_base[0] & 0x80000000) != 0) /* nopacket */
			break;
		desc_base[0] = 0x80000000 | sizeof(g_pkt_base);
	}
	IFX_REG_W32_MASK(1, 0, IFX_DMA_CCTRL(0)); /* stop receiving */
	}

	if (of_machine_is_compatible("lantiq,ar10")) {
		if ((IFX_REG_R32(DMRX_PGCNT_XRX300) & 0x00FF) != 0 ||
			(IFX_REG_R32(DMRX_PKTCNT_XRX300) & 0x00FF) != 0 ||
			(IFX_REG_R32(DSRX_PGCNT_XRX300) & 0x00FF) != 0) {
			pr_info("%s error: DMRX_PGCNT_XRX300 = 0x%08x\n",
					__func__,
					IFX_REG_R32(DMRX_PGCNT_XRX300));
			pr_info("DMRX_PKTCNT_XRX300 = 0x%08x\n",
					IFX_REG_R32(DMRX_PKTCNT_XRX300));
			pr_info("DSRX_PGCNT_XRX300 = 0x%08x\n",
					IFX_REG_R32(DSRX_PGCNT_XRX300));
		}
	}

	if (of_machine_is_compatible("lantiq,vr9")) {
		if ((IFX_REG_R32(DMRX_PGCNT_XRX200) & 0x00FF) != 0 ||
			(IFX_REG_R32(DMRX_PKTCNT_XRX200) & 0x00FF) != 0 ||
			(IFX_REG_R32(DSRX_PGCNT_XRX200) & 0x00FF) != 0) {
			pr_info("%s error: DMRX_PGCNT_XRX200 = 0x%08x\n",
					__func__,
					IFX_REG_R32(DMRX_PGCNT_XRX200));
			pr_info("DMRX_PKTCNT_XRX200 = 0x%08x\n",
					IFX_REG_R32(DMRX_PKTCNT_XRX200));
			pr_info("DSRX_PGCNT_XRX200 = 0x%08x\n",
					IFX_REG_R32(DSRX_PGCNT_XRX200));

		}
	}
	return;
}

/* bring up all register ranges that we need for basic system control */
void __init ltq_soc_init(void)
{
	struct resource res_pmu, res_cgu, res_ebu, res_hsnand;
	struct device_node *np_pmu =
			of_find_compatible_node(NULL, NULL, "lantiq,pmu-xway");
	struct device_node *np_cgu =
			of_find_compatible_node(NULL, NULL, "lantiq,cgu-xway");
	struct device_node *np_ebu =
			of_find_compatible_node(NULL, NULL, "lantiq,ebu-xway");

	/* check if all the core register ranges are available */
	if (!np_pmu || !np_cgu || !np_ebu)
		panic("Failed to load core nodes from devicetree");

	if (of_address_to_resource(np_pmu, 0, &res_pmu) ||
			of_address_to_resource(np_cgu, 0, &res_cgu) ||
			of_address_to_resource(np_ebu, 0, &res_ebu))
		panic("Failed to get core resources");

	if ((request_mem_region(res_pmu.start, resource_size(&res_pmu),
				res_pmu.name) < 0) ||
		(request_mem_region(res_cgu.start, resource_size(&res_cgu),
				res_cgu.name) < 0) ||
		(request_mem_region(res_ebu.start, resource_size(&res_ebu),
				res_ebu.name) < 0))
		pr_err("Failed to request core reources");
	pmu_membase = ioremap_nocache(res_pmu.start, resource_size(&res_pmu));
	ltq_cgu_membase = ioremap_nocache(res_cgu.start,
						resource_size(&res_cgu));
	ltq_ebu_membase = ioremap_nocache(res_ebu.start,
						resource_size(&res_ebu));
	if (!pmu_membase || !ltq_cgu_membase || !ltq_ebu_membase)
		panic("Failed to remap core resources");

	if (of_machine_is_compatible("lantiq,ar10") ||
		of_machine_is_compatible("lantiq,grx390")) {

		if (of_address_to_resource(np_ebu, 1, &res_hsnand))
			panic("Failed to get hsnand resource");

	if (request_mem_region(res_hsnand.start, resource_size(&res_hsnand),
			res_hsnand.name) < 0)
				pr_err("Failed to request hsnand reources");

		ltq_dmanand_membase = ioremap_nocache(res_hsnand.start,
				resource_size(&res_hsnand));
	if (!ltq_dmanand_membase)
			panic("Failed to remap hsnand resources");
	}

	if (of_machine_is_compatible("lantiq,vr9")) {
		struct resource res_xbar;
		struct device_node *np_xbar =
			of_find_compatible_node(NULL, NULL,
						"lantiq,xbar-xway");
		if (!np_xbar)
			panic("Failed to load xbar nodes from devicetree");
		if (of_address_to_resource(np_pmu, 0, &res_xbar))
			panic("Failed to get xbar resources");
		if (request_mem_region(res_xbar.start, resource_size(&res_xbar),
				res_xbar.name) < 0)
			panic("Failed to get xbar resources");

		ltq_xbar_membase = ioremap_nocache(res_xbar.start,
						resource_size(&res_xbar));
		if (!ltq_xbar_membase)
			panic("Failed to remap xbar resources");
	}

	if (of_machine_is_compatible("lantiq,ar10") ||
		of_machine_is_compatible("lantiq,vr9")) {
			ltq_dplus_clean();
	}
	/* make sure to unprotect the memory region where flash is located */
	ltq_ebu_w32(ltq_ebu_r32(LTQ_EBU_BUSCON0) & ~EBU_WRDIS,
			LTQ_EBU_BUSCON0);

	/* add our generic xway clocks */
	clkdev_add_pmu("10000000.fpi", NULL, 0, 0, PMU_FPI);
	clkdev_add_pmu("1e100c00.serial", NULL, 0, 0, PMU_ASC1);
	clkdev_add_pmu("1e100a00.gptu", NULL, 1, 0, PMU_GPT);
	clkdev_add_pmu("1e100bb0.stp", NULL, 1, 0, PMU_STP);
	clkdev_add_pmu("1e104100.dma", NULL, 1, 0, PMU_DMA);
	clkdev_add_pmu("1e100800.spi", NULL, 1, 0, PMU_SPI);
	clkdev_add_pmu("1e105300.ebu", NULL, 0, 0, PMU_EBU);
	clkdev_add_clkout();

	/* add the soc dependent clocks */
	if (of_machine_is_compatible("lantiq,vr9")) {
		ifccr = CGU_IFCCR_VR9;
		pcicr = CGU_PCICR_VR9;
	}

	if (of_machine_is_compatible("lantiq, ar9") ||
		of_machine_is_compatible("lantiq, danube"))
		clkdev_add_pmu("1e100400.serial", NULL, 1, 0, PMU_ASC0);

	if (!of_machine_is_compatible("lantiq,grx390"))
		clkdev_add_pmu("1e116000.mei", "dfe", 1, 0, PMU_DFE);

	if (of_machine_is_compatible("lantiq,ar10")) {
		clkdev_add_pmu("1e116000.mei", "afe", 1, 2,
				PMU_ANALOG_DSL_AFE);
	}

	if (!of_machine_is_compatible("lantiq,ase"))
		clkdev_add_pmu("1e103100.deu", NULL, 1, 0, PMU_DEU);

	if (of_machine_is_compatible("lantiq,ar9"))
		clkdev_add_pci(1);
	else if (of_machine_is_compatible("lantiq,vr9")) {
		clkdev_add_pci(0);
		clkdev_add_pmu(NULL, "ahb", 1, 0,
			PMU_AHBM | PMU_AHBS);
	}

	if (of_machine_is_compatible("lantiq,grx390") ||
		of_machine_is_compatible("lantiq,ar10")) {
		clkdev_add_pmu("1e101000.usb", "phy", 1, 2,
			PMU_ANALOG_USB0_P);
		clkdev_add_pmu("1e106000.usb", "phy", 1, 2,
			PMU_ANALOG_USB1_P);
	}

	if (!of_machine_is_compatible("lantiq,vr9"))
		clkdev_add_pmu("1e101000.usb", "ctl", 1, 0, PMU_USB0);

	if (of_machine_is_compatible("lantiq,grx390") ||
		of_machine_is_compatible("lantiq,ar10")) {
		/* rc 0 */
		clkdev_add_pmu("1d900000.pcie", "phy", 1, 2,
			PMU_ANALOG_PCIE0_P);
		clkdev_add_pmu("1d900000.pcie", "msi", 1, 1, PMU1_PCIE_MSI);
		clkdev_add_pmu("1d900000.pcie", "pdi", 1, 1, PMU1_PCIE_PDI);
		clkdev_add_pmu("1d900000.pcie", "ctl", 1, 1, PMU1_PCIE_CTL);
		/* rc 1 */
		clkdev_add_pmu("19900000.pcie", "phy", 1, 2,
			PMU_ANALOG_PCIE1_P);
		clkdev_add_pmu("19900000.pcie", "msi", 1, 1, PMU1_PCIE1_MSI);
		clkdev_add_pmu("19900000.pcie", "pdi", 1, 1, PMU1_PCIE1_PDI);
		clkdev_add_pmu("19900000.pcie", "ctl", 1, 1, PMU1_PCIE1_CTL);
	}

	if (of_machine_is_compatible("lantiq,ase")) {
		struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "cpu";
			clk->cl.con_id = "cpu";
			clk->cl.clk = clk;
			clk->rate = ltq_ase_cpu_hz();
			clk->set_rate = ltq_ase_set_cpu_hz;
			clk->get_rate = ltq_ase_cpu_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "fpi";
			clk->cl.con_id = "fpi";
			clk->cl.clk = clk;
			clk->rate = ltq_ase_fpi_hz();
			clk->get_rate = ltq_ase_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "io";
			clk->cl.con_id = "io";
			clk->cl.clk = clk;
			clk->rate = ltq_ase_fpi_hz();
			clk->get_rate = ltq_ase_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "ppe";
			clk->cl.con_id = "ppe";
			clk->cl.clk = clk;
			clk->rate = ltq_ase_pp32_hz();
			clk->get_rate = ltq_ase_pp32_hz;
			clkdev_add(&clk->cl);
		}
		clkdev_add_pmu("1e101000.usb", "phy", 1, 0, PMU_USB0_P);
		clkdev_add_pmu("1e180000.etop", "ppe", 1, 0, PMU_PPE);
		clkdev_add_cgu("1e180000.etop", "ephycgu", CGU_EPHY),
		clkdev_add_pmu("1e180000.etop", "ephy", 1, 0, PMU_EPHY);
		clkdev_add_pmu("1e103000.sdio", NULL, 1, 0, PMU_ASE_SDIO);

	} else if (of_machine_is_compatible("lantiq,grx390")) {
		struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "cpu";
			clk->cl.con_id = "cpu";
			clk->cl.clk = clk;
			clk->rate = ltq_grx390_cpu_hz();
			clk->set_rate = ltq_grx390_set_cpu_hz;
			clk->get_rate = ltq_grx390_cpu_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "fpi";
			clk->cl.con_id = "fpi";
			clk->cl.clk = clk;
			clk->rate = ltq_grx390_fpi_hz();
			clk->get_rate = ltq_grx390_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "io";
			clk->cl.con_id = "io";
			clk->cl.clk = clk;
			clk->rate = ltq_grx390_fpi_hz();
			clk->get_rate = ltq_grx390_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "ppe";
			clk->cl.con_id = "ppe";
			clk->cl.clk = clk;
			clk->rate = ltq_grx390_pp32_hz();
			clk->get_rate = ltq_grx390_pp32_hz;
			clkdev_add(&clk->cl);
		}
		clkdev_add_pmu("1e106000.usb", "ctl", 1, 0, PMU_USB1);
		/* rc 2 */
		clkdev_add_pmu("19b00000.pcie", "phy", 1, 2,
			PMU_ANALOG_PCIE2_P);
		clkdev_add_pmu("19b00000.pcie", "msi", 1, 1, PMU1_PCIE2_MSI);
		clkdev_add_pmu("19b00000.pcie", "pdi", 1, 1, PMU1_PCIE2_PDI);
		clkdev_add_pmu("19b00000.pcie", "ctl", 1, 1, PMU1_PCIE2_CTL);
		clkdev_add_pmu("1e108000.eth", NULL, 1, 0, PMU_SWITCH |
			PMU_PPE_DP);
		clkdev_add_pmu("1da00000.usif", "NULL", 1, 0, PMU_USIF);
	} else if (of_machine_is_compatible("lantiq,ar10")) {
		struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "cpu";
			clk->cl.con_id = "cpu";
			clk->cl.clk = clk;
			clk->rate = ltq_ar10_cpu_hz();
			clk->set_rate = ltq_ar10_set_cpu_hz;
			clk->get_rate = ltq_ar10_cpu_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "fpi";
			clk->cl.con_id = "fpi";
			clk->cl.clk = clk;
			clk->rate = ltq_ar10_fpi_hz();
			clk->get_rate = ltq_ar10_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "io";
			clk->cl.con_id = "io";
			clk->cl.clk = clk;
			clk->rate = ltq_ar10_fpi_hz();
			clk->get_rate = ltq_ar10_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "ppe";
			clk->cl.con_id = "ppe";
			clk->cl.clk = clk;
			clk->rate = ltq_ar10_pp32_hz();
			clk->get_rate = ltq_ar10_pp32_hz;
			clkdev_add(&clk->cl);
		}
		clkdev_add_pmu("1e106000.usb", "ctl", 1, 0, PMU_USB1);
		clkdev_add_pmu("1e108000.eth", NULL, 1, 0, PMU_SWITCH |
			PMU_PPE_DP | PMU_PPE_TC);
		clkdev_add_pmu("1da00000.usif", "NULL", 1, 0, PMU_USIF);
		clkdev_add_pmu("1f203000.rcu", "gphy", 1, 0, PMU_GPHY);

	} else if (of_machine_is_compatible("lantiq,vr9")) {
		struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "cpu";
			clk->cl.con_id = "cpu";
			clk->cl.clk = clk;
			clk->rate = ltq_vr9_cpu_hz();
			clk->set_rate = ltq_vr9_set_cpu_hz;
			clk->get_rate = ltq_vr9_cpu_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "fpi";
			clk->cl.con_id = "fpi";
			clk->cl.clk = clk;
			clk->rate = ltq_vr9_fpi_hz();
			clk->get_rate = ltq_vr9_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "io";
			clk->cl.con_id = "io";
			clk->cl.clk = clk;
			clk->rate = ltq_vr9_fpi_hz();
			clk->get_rate = ltq_vr9_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "ppe";
			clk->cl.con_id = "ppe";
			clk->cl.clk = clk;
			clk->rate = ltq_vr9_pp32_hz();
			clk->get_rate = ltq_vr9_pp32_hz;
			clkdev_add(&clk->cl);
		}
		clkdev_add_pmu("1e101000.usb", "phy", 1, 0, PMU_USB0_P);
		clkdev_add_pmu("1e101000.usb", "ctl", 1, 0,
			PMU_USB0 | PMU_AHBM);
		clkdev_add_pmu("1e106000.usb", "phy", 1, 0, PMU_USB1_P);
		clkdev_add_pmu("1e106000.usb", "ctl", 1, 0,
			PMU_USB1 | PMU_AHBM);
		clkdev_add_pmu("1d900000.pcie", "phy", 1, 1, PMU1_PCIE_PHY);
		clkdev_add_pmu("1d900000.pcie", "bus", 1, 0, PMU_PCIE_CLK);
		clkdev_add_pmu("1d900000.pcie", "msi", 1, 1, PMU1_PCIE_MSI);
		clkdev_add_pmu("1d900000.pcie", "pdi", 1, 1, PMU1_PCIE_PDI);
		clkdev_add_pmu("1d900000.pcie", "ctl", 1, 1, PMU1_PCIE_CTL);

		clkdev_add_pmu("1da00000.usif", "NULL", 1, 0, PMU_USIF);
		clkdev_add_pmu("1e108000.eth", NULL, 1, 0,
				PMU_SWITCH | PMU_PPE_DPLUS | PMU_PPE_DPLUM |
				PMU_PPE_EMA | PMU_PPE_TC | PMU_PPE_SLL01 |
				PMU_PPE_QSB | PMU_PPE_TOP);
		clkdev_add_pmu("1f203000.rcu", "gphy", 1, 0, PMU_GPHY);
		clkdev_add_pmu("1e103000.sdio", NULL, 1, 0, PMU_SDIO);
		pmu_w32(~0, PMU_PWDSR1);
		pmu_w32(pmu_r32(PMU_PWDSR) & ~PMU_PCIE_CLK, PMU_PWDSR);
	} else if (of_machine_is_compatible("lantiq,ar9")) {
		struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "cpu";
			clk->cl.con_id = "cpu";
			clk->cl.clk = clk;
			clk->rate = ltq_ar9_cpu_hz();
			clk->set_rate = ltq_ar9_set_cpu_hz;
			clk->get_rate = ltq_ar9_cpu_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "fpi";
			clk->cl.con_id = "fpi";
			clk->cl.clk = clk;
			clk->rate = ltq_ar9_fpi_hz();
			clk->get_rate = ltq_ar9_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "io";
			clk->cl.con_id = "io";
			clk->cl.clk = clk;
			clk->rate = ltq_ar9_fpi_hz();
			clk->get_rate = ltq_ar9_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "ppe";
			clk->cl.con_id = "ppe";
			clk->cl.clk = clk;
			clk->rate = ltq_ar9_pp32_hz();
			clk->get_rate = ltq_ar9_pp32_hz;
			clkdev_add(&clk->cl);
		}
		clkdev_add_pmu("1e101000.usb", "phy", 1, 0, PMU_USB0_P);
		clkdev_add_pmu("1e106000.usb", "phy", 1, 0, PMU_USB1_P);
		clkdev_add_pmu("1e106000.usb", "ctl", 1, 0, PMU_USB1);
		clkdev_add_pmu("1e180000.etop", "switch", 1, 0, PMU_SWITCH);
		clkdev_add_pmu("1e103000.sdio", NULL, 1, 0, PMU_SDIO);
	} else {
		struct clk *clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "cpu";
			clk->cl.con_id = "cpu";
			clk->cl.clk = clk;
			clk->rate = ltq_danube_cpu_hz();
			clk->set_rate = ltq_danube_set_cpu_hz;
			clk->get_rate = ltq_danube_cpu_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "fpi";
			clk->cl.con_id = "fpi";
			clk->cl.clk = clk;
			clk->rate = ltq_danube_fpi_hz();
			clk->get_rate = ltq_danube_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "io";
			clk->cl.con_id = "io";
			clk->cl.clk = clk;
			clk->rate = ltq_danube_fpi_hz();
			clk->get_rate = ltq_danube_fpi_hz;
			clkdev_add(&clk->cl);
		}
		clk = kzalloc(sizeof(struct clk), GFP_KERNEL);
		if (clk) {
			clk->cl.dev_id = "ppe";
			clk->cl.con_id = "ppe";
			clk->cl.clk = clk;
			clk->rate = ltq_danube_pp32_hz();
			clk->get_rate = ltq_danube_pp32_hz;
			clkdev_add(&clk->cl);
		}
		clkdev_add_pmu("1e101000.usb", "phy", 1, 0, PMU_USB0_P);
		clkdev_add_pmu("1e103000.sdio", NULL, 1, 0, PMU_SDIO);
	}
	xbar_fpi_burst_disable();
}
