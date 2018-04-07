/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2013 John Crispin <blogic@openwrt.org>
 *  Copyright (C) 2009~2015 Lei Chuanhua <chuanhua.lei@lantiq.com>
 *  Copyright(c) 2016 Intel Corporation.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/pci_regs.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_pci.h>
#include <linux/of_irq.h>
#include <lantiq.h>
#include <lantiq_irq.h>
#include <lantiq_soc.h>

#include "pcie-lantiq.h"

#define ltq_pcie_r32		ltq_r32
#define ltq_pcie_w32		ltq_w32
#define ltq_pcie_w32_mask	ltq_w32_mask

/* #define LTQ_PCIE_PHY_DBG */

/* Enable 32bit io due to its mem mapped io nature */
#define LTQ_PCIE_IO_32BIT

static int pcie_read_config(struct pci_bus *bus, u32 devfn,
				int where, int size, u32 *value);
static int pcie_write_config(struct pci_bus *bus, u32 devfn,
				 int where, int size, u32 value);

static int pcie_force_gen1;

static struct pci_ops pcie_ops = {
	.read = pcie_read_config,
	.write = pcie_write_config,
};

#if defined(LTQ_PCIE_DBG)
static u32 pcie_debug_flag = PCIE_MSG_ANY & (~PCIE_MSG_CFG);
static void pcie_debug(const char *fmt, ...)
{
	static char buf[256] = { 0 }; /* XXX */
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	pr_info("%s", buf);
}
#define pcie_dbg(_m, _fmt, args...) do {	\
	if (pcie_debug_flag & (_m))		\
		pcie_debug((_fmt), ##args);	\
} while (0)

#else
#define pcie_dbg(_m, _fmt, args...)	do {} while (0)
#endif

static int __init setup_pcie_gen1(char *str)
{
	if (!strcmp(str, "off"))
		pcie_force_gen1 = 0;
	else if (!strcmp(str, "on"))
		pcie_force_gen1 = 1;
	else
		return 0;
	return 1;
}
__setup("pcie_force_gen1=", setup_pcie_gen1);

static inline u32 pcie_cfg_rd(struct ltq_pcie_port *lpp, u32 reg)
{
	return ltq_pcie_r32(lpp->devcfg_base + reg);
}

static inline  void pcie_cfg_wr(struct ltq_pcie_port *lpp,
	u32 val, unsigned int reg)
{
	ltq_pcie_w32(val, lpp->devcfg_base + reg);
}

static inline u32 pcie_app_rd(struct ltq_pcie_port *lpp, u32 reg)
{
	return ltq_pcie_r32(lpp->app_base + reg);
}

static inline void pcie_app_wr(struct ltq_pcie_port *lpp,
	u32 val, unsigned int reg)
{
	ltq_pcie_w32(val, lpp->app_base + reg);
}

static inline void pcie_app_wr_mask(struct ltq_pcie_port *lpp,
	u32 clear, u32 set, unsigned int reg)
{
	pcie_app_wr(lpp, (pcie_app_rd(lpp, reg) & ~(clear)) | (set), reg);
}

static inline u32 pcie_rc_cfg_rd(struct ltq_pcie_port *lpp, u32 reg)
{
	return ltq_pcie_r32(lpp->mem_base + reg);
}

static inline void pcie_rc_cfg_wr(struct ltq_pcie_port *lpp,
	 u32 val, unsigned int reg)
{
	ltq_pcie_w32(val, lpp->mem_base + reg);
}

static inline void pcie_rc_cfg_wr_mask(struct ltq_pcie_port *lpp,
	u32 clear, u32 set, unsigned int reg)
{
	pcie_rc_cfg_wr(lpp,
		(pcie_rc_cfg_rd(lpp, reg) & ~(clear)) | (set), reg);
}

struct ltq_pcie_port *bus_to_ltq_pcie_port(struct pci_bus *bus)
{
	struct pci_controller *hose;

	hose = (struct pci_controller *)bus->sysdata;
	return container_of(hose, struct ltq_pcie_port, pcic);
}

#ifdef LTQ_PCI_PHY_DBG
/* Generate hot reset, XXX must catpure to verify */
static void pcie_secondary_bus_reset(struct ltq_pcie_port *lpp)
{
	int i;
#define PCIE_RESET_TIME		20

	/* Assert Secondary Bus Reset */
	pcie_rc_cfg_wr_mask(lpp, 0, PCIE_INTRBCTRL_RST_SECONDARY_BUS,
		PCIE_INTRBCTRL);

	/* De-assert Secondary Bus Reset */
	pcie_rc_cfg_wr_mask(lpp, PCIE_INTRBCTRL_RST_SECONDARY_BUS, 0,
		PCIE_INTRBCTRL);

	/* XXX, wait at least 100 ms, then restore again */
	for (i = 0; i < PCIE_RESET_TIME; i++)
		mdelay(10);
#undef PCIE_RESET_TIME
}

/* Error or L0s to L0 */
static int pcie_retrain_link(struct ltq_pcie_port *lpp)
{
	int i;
#define PCIE_RETRAIN_TIME  1000

	pcie_rc_cfg_wr_mask(lpp, 0, PCIE_LCTLSTS_RETRIAN_LINK, PCIE_LCTLSTS);

	/* Wait for the link to come up */
	for (i = 0; i < PCIE_RETRAIN_TIME; i++) {
		if (!(pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS)
			& PCIE_LCTLSTS_RETRAIN_PENDING))
			break;
		udelay(100);
	}
	if (i >= PCIE_RETRAIN_TIME) {
		dev_err(lpp->dev, "%s retrain timeout\n", __func__);
		return -1;
	}
	return 0;
#undef PCIE_RETRAIN_TIME
}

static void pcie_disable_scrambling(struct ltq_pcie_port *lpp)
{
	pcie_rc_cfg_wr_mask(lpp, 0, PCIE_PLCR_SCRAMBLE_DISABLE, PCIE_PLCR);
}
#endif /* LTQ_PCI_PHY_DBG */

static int pcie_ltssm_enable(struct ltq_pcie_port *lpp)
{
	int i;
#define PCIE_LTSSM_ENABLE_TIMEOUT 10
	/* Enable LTSSM */
	pcie_app_wr(lpp, PCIE_RC_CCR_LTSSM_ENABLE, PCIE_RC_CCR);

	/* Wait for the link to come up */
	for (i = 0; i < PCIE_LTSSM_ENABLE_TIMEOUT; i++) {
		if (!(pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS)
			& PCIE_LCTLSTS_RETRAIN_PENDING))
			break;
		udelay(10);
	}
	if (i >= PCIE_LTSSM_ENABLE_TIMEOUT) {
		dev_err(lpp->dev, "%s link timeout!!!!!\n", __func__);
		return -1;
	}
	return 0;
#undef PCIE_LTSSM_ENABLE_TIMEOUT
}

static void pcie_ltssm_disable(struct ltq_pcie_port *lpp)
{
	pcie_app_wr(lpp, 0, PCIE_RC_CCR); /* Disable LTSSM */
	dev_dbg(lpp->dev, "%s PCIE_RC_CCR 0x%08x\n",
		__func__, pcie_app_rd(lpp, PCIE_RC_CCR));
}

static void pcie_ahb_bus_error_suppress(struct ltq_pcie_port *lpp)
{
	pcie_app_wr(lpp, PCIE_AHB_CTRL_BUS_ERROR_SUPPRESS, PCIE_AHB_CTRL);
}

static void pcie_status_register_clear(struct ltq_pcie_port *lpp)
{
	/* Clear the status register, XXX, seperate function */
	pcie_app_wr(lpp, 0, PCIE_RC_DR);
	pcie_rc_cfg_wr(lpp, 0, PCIE_PCICMDSTS);
	pcie_rc_cfg_wr(lpp, 0, PCIE_DCTLSTS);
	pcie_rc_cfg_wr(lpp, 0, PCIE_LCTLSTS);
	pcie_rc_cfg_wr(lpp, 0, PCIE_SLCTLSTS);
	pcie_rc_cfg_wr(lpp, 0, PCIE_RSTS);
	pcie_rc_cfg_wr(lpp, 0, PCIE_UES_R);
	pcie_rc_cfg_wr(lpp, 0, PCIE_UEMR);
	pcie_rc_cfg_wr(lpp, 0, PCIE_UESR);
	pcie_rc_cfg_wr(lpp, 0, PCIE_CESR);
	pcie_rc_cfg_wr(lpp, 0, PCIE_CEMR);
	pcie_rc_cfg_wr(lpp, 0, PCIE_RESR);
	pcie_rc_cfg_wr(lpp, 0, PCIE_PVCCRSR);
	pcie_rc_cfg_wr(lpp, 0, PCIE_VC0_RSR0);
	pcie_rc_cfg_wr(lpp, 0, PCIE_TPFCS);
	pcie_rc_cfg_wr(lpp, 0, PCIE_TNPFCS);
	pcie_rc_cfg_wr(lpp, 0, PCIE_TCFCS);
	pcie_rc_cfg_wr(lpp, 0, PCIE_QSR);
	pcie_rc_cfg_wr(lpp, 0, PCIE_IOBLSECS);
}

/*
 * Must be done after ltssm due to based on negotiated link
 * width and payload size
 * Update the Replay Time Limit. Empirically, some PCIe
 * devices take a little longer to respond than expected under
 * load. As a workaround for this we configure the Replay Time
 * Limit to the value expected for a 512 byte MPS instead of
 * our actual 128 byte MPS. The numbers below are directly
 * from the PCIe spec table 3-4/5.
 * Please note, our PIPE interface is 16b/125MHz, so the table
 * value has to be divided by 2
 */
static void pcie_replay_time_update(struct ltq_pcie_port *lpp)
{
	u32 reg;
	int nlw;
	int rtl;

	reg = pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS);
	nlw = MS(reg, PCIE_LCTLSTS_NEGOTIATED_LINK_WIDTH);
	switch (nlw) {
	case PCIE_MAX_LENGTH_WIDTH_X1:
		if (pcie_phy_clock_ppm_enabled())
			rtl = 6215; /* 4KB replay timeout */
		else
			rtl = 839; /* 512B replay timeout */

		break;
	case PCIE_MAX_LENGTH_WIDTH_X2:
		rtl = 818;
		break;
	case PCIE_MAX_LENGTH_WIDTH_X4:
		rtl = 423;
		break;
	case PCIE_MAX_LENGTH_WIDTH_X8:
		rtl = 225;
		break;
	default:
		rtl = 1606;
		break;
	}
	reg = pcie_rc_cfg_rd(lpp, PCIE_ALTRT);
	reg &= ~PCIE_ALTRT_REPLAY_TIME_LIMIT;
	reg |= SM(rtl, PCIE_ALTRT_REPLAY_TIME_LIMIT);
	pcie_rc_cfg_wr(lpp, reg, PCIE_ALTRT);
	dev_dbg(lpp->dev, "%s PCIE_ALTRT 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_ALTRT));
}

static int pcie_link_up(struct ltq_pcie_port *lpp)
{
	return (pcie_app_rd(lpp, PCIE_PHY_SR) &
		PCIE_PHY_SR_PHY_LINK_UP) ? 1 : 0;
}

#ifdef LTQ_PCIE_DBG
static void pcie_status_registers_dump(struct ltq_pcie_port *lpp)
{
	pcie_dbg(PCIE_MSG_INIT, "PCIe_PCICMDSTS: 0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_PCICMDSTS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_RC_DR:     0x%08x\n",
		pcie_app_rd(lpp, PCIE_RC_DR));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_DCTLSTS:   0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_DCTLSTS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_LCTLSTS:   0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_SLCTLSTS:  0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_SLCTLSTS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_RSTS:      0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_RSTS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_UES_R:     0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_UES_R));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_UEMR:      0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_UEMR));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_UESR:      0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_UESR));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_CESR:      0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_CESR));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_CEMR:      0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_CEMR));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_RESR:      0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_RESR));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_ESIR:      0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_ESIR));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_PVCCRSR:   0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_PVCCRSR));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_VC0_RSR0:  0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_VC0_RSR0));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_TPFCS:     0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_TPFCS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_TNPFCS:    0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_TNPFCS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_TCFCS:     0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_TCFCS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_QSR:       0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_QSR));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_VCTAR1:    0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_VCTAR1));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_VCTAR2:    0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_VCTAR2));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_IOBLSECS:  0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_IOBLSECS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_ALTRT:     0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_ALTRT));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_SNR:       0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_SNR));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_DBR0:      0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_DBR0));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_DBR1:      0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_DBR1));
}

static void pcie_post_dump(struct ltq_pcie_port *lpp)
{
	pcie_dbg(PCIE_MSG_INIT, "PCIe_PCICMDSTS:  0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_PCICMDSTS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_MBML:       0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_MBML));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_PBML:       0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_PMBL));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_IOBLSECS:   0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_IOBLSECS));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_IO_BANDL:   0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_IO_BANDL));
	pcie_dbg(PCIE_MSG_INIT, "PCIe_INTRBCTRL:  0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_INTRBCTRL));
	pcie_dbg(PCIE_MSG_INIT, "Power State:     D%1d\n",
		pcie_rc_cfg_rd(lpp, PCIE_PM_CSR) &
			PCIE_PM_CSR_POWER_STATE);
	pcie_dbg(PCIE_MSG_INIT, "Negotiated Link Width: %d\n",
		MS(pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS),
			PCIE_LCTLSTS_NEGOTIATED_LINK_WIDTH));
	pcie_dbg(PCIE_MSG_INIT, "Number of VCs:    %d\n",
		pcie_rc_cfg_rd(lpp, PCIE_PVC1) & PCIE_PVC1_EXT_VC_CNT);
	pcie_dbg(PCIE_MSG_INIT, "Low-priority VCs: %d\n",
		MS(pcie_rc_cfg_rd(lpp, PCIE_PVC1),
			PCIE_PVC1_LOW_PRI_EXT_VC_CNT));
	pcie_dbg(PCIE_MSG_INIT, "VC Arbitration:   0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_PVC2) & PCIE_PVC2_VC_ARB_WRR);
	pcie_dbg(PCIE_MSG_INIT, "Port Arbitration: 0x%08x\n",
		pcie_rc_cfg_rd(lpp, PCIE_VC0_RC) & PCIE_VC0_RC_PORT_ARB);

	if (pcie_link_up(lpp))
		pcie_dbg(PCIE_MSG_INIT, "PCIe PHY Link is UP\n");
	else
		pcie_dbg(PCIE_MSG_INIT, "PCIe PHY Link is DOWN!\n");

	if ((pcie_app_rd(lpp, PCIE_RC_DR) & PCIE_RC_DR_DLL_UP))
		pcie_dbg(PCIE_MSG_INIT, "PCIe DLL is UP\n");
	else
		pcie_dbg(PCIE_MSG_INIT, "PCIe DLL is DOWN!\n");

	if ((pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS) & PCIE_LCTLSTS_DLL_ACTIVE))
		pcie_dbg(PCIE_MSG_INIT, "PCIE_LCTLSTS in DL_Active state!\n");
	else
		pcie_dbg(PCIE_MSG_INIT,
			"PCIE_LCTLSTS NOT in DL_Active state!\n");
}
#endif /* LTQ_PCIE_DBG */

/* XXX, this function is not needed in fact */
static void pcie_mem_io_setup(struct ltq_pcie_port *lpp)
{
	u32 reg;
	/*
	 * BAR[0:1] readonly register
	 * RC contains only minimal BARs for packets mapped to this device
	 * Mem/IO filters defines a range of memory occupied by memory mapped
	 * IO devices that reside on the downstream side fo the bridge.
	 */
	reg = SM(((lpp->mem_phys_base + lpp->mem_size - 1) >> 20),
		 PCIE_MBML_MEM_LIMIT_ADDR)
		| SM((lpp->mem_phys_base >> 20), PCIE_MBML_MEM_BASE_ADDR);

	pcie_rc_cfg_wr(lpp, reg, PCIE_MBML);

	dev_dbg(lpp->dev, "%s PCIE_MBML: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_MBML));

#ifdef LTQ_PCIE_PREFETCH_MEM_64BIT
	reg = SM(((lpp->mem_phys_base + lpp->mem_size - 1) >> 20),
		PCIE_PMBL_END_ADDR)
		| SM((lpp->mem_phys_base >> 20),
		PCIE_PMBL_UPPER_12BIT) | PCIE_PMBL_64BIT_ADDR;
	pcie_rc_cfg_wr(lpp, reg, PCIE_PMBL);

	/* Must configure upper 32bit */
	pcie_rc_cfg_wr(lpp, 0, PCIE_PMBU32);
	pcie_rc_cfg_wr(lpp, 0, PCIE_PMLU32);
#else
	/* PCIe_PBML, same as MBML */
	pcie_rc_cfg_wr(lpp, pcie_rc_cfg_rd(lpp, PCIE_MBML),
		PCIE_PMBL);
#endif
	dev_dbg(lpp->dev, "%s PCIE_PMBL: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_PMBL));

	/* IO Address Range */
	reg = SM(((lpp->io_phys_base + lpp->io_size - 1) >> 12),
		 PCIE_IOBLSECS_IO_LIMIT_ADDR)
		| SM((lpp->io_phys_base >> 12),
			PCIE_IOBLSECS_IO_BASE_ADDR);
#ifdef LTQ_PCIE_IO_32BIT
	reg |= PCIE_IOBLSECS_32BIT_IO_ADDR;
#endif /* LTQ_PCIE_IO_32BIT */
	pcie_rc_cfg_wr(lpp, reg, PCIE_IOBLSECS);

	dev_dbg(lpp->dev, "%s PCIE_IOBLSECS: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_IOBLSECS));
#ifdef LTQ_PCIE_IO_32BIT
	reg = SM(((lpp->io_phys_base + lpp->io_size - 1) >> 16),
		 PCIE_IO_BANDL_UPPER_16BIT_IO_LIMIT)
		| SM((lpp->io_phys_base >> 16),
			PCIE_IO_BANDL_UPPER_16BIT_IO_BASE);
	pcie_rc_cfg_wr(lpp, reg, PCIE_IO_BANDL);

	dev_dbg(lpp->dev, "%s PCIE_IO_BANDL: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_IO_BANDL));
#endif /* LTQ_PCIE_IO_32BIT */
}

static void pcie_msi_setup(struct ltq_pcie_port *lpp)
{
	u32 reg;

	/* XXX, MSI stuff should only apply to EP */
	/* MSI Capability: Only enable 32-bit addresses */
	reg = pcie_rc_cfg_rd(lpp, PCIE_MCAPR);
	reg &= ~PCIE_MCAPR_ADDR64_CAP;

	reg |= PCIE_MCAPR_MSI_ENABLE;

	/* Disable multiple message */
	reg &= ~(PCIE_MCAPR_MULTI_MSG_CAP | PCIE_MCAPR_MULTI_MSG_ENABLE);
	pcie_rc_cfg_wr(lpp, reg, PCIE_MCAPR);
	dev_dbg(lpp->dev, "%s PCIE_MCAPR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_MCAPR));
}

static void pcie_pm_setup(struct ltq_pcie_port *lpp)
{
	/* Enable PME, Soft reset enabled */
	pcie_rc_cfg_wr_mask(lpp, 0, PCIE_PM_CSR_PME_ENABLE | PCIE_PM_CSR_SW_RST,
		PCIE_PM_CSR);
	dev_dbg(lpp->dev, "%s PCIE_PM_CSR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_PM_CSR));
}

static void pcie_bus_setup(struct ltq_pcie_port *lpp)
{
	u32 reg;

	reg = SM(0, PCIE_BNR_PRIMARY_BUS_NUM)
		| SM(1, PCIE_PNR_SECONDARY_BUS_NUM)
		| SM(0xFF, PCIE_PNR_SUB_BUS_NUM);
	pcie_rc_cfg_wr(lpp, reg, PCIE_BNR);
	dev_dbg(lpp->dev, "%s PCIE_BNR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_BNR));
}

static void pcie_device_setup(struct ltq_pcie_port *lpp)
{
	u32 reg;

	/* Device capability register, set up Maximum payload size */
	reg = pcie_rc_cfg_rd(lpp, PCIE_DCAP);
	reg |= PCIE_DCAP_ROLE_BASE_ERR_REPORT;
	reg |= SM(PCIE_MAX_PAYLOAD_128, PCIE_DCAP_MAX_PAYLOAD_SIZE);

	/* Only available for EP */
	reg &= ~(PCIE_DCAP_EP_L0S_LATENCY | PCIE_DCAP_EP_L1_LATENCY);
	pcie_rc_cfg_wr(lpp, reg, PCIE_DCAP);
	dev_dbg(lpp->dev, "%s PCIE_DCAP: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_DCAP));

	/* Device control and status register */
	/* Set Maximum Read Request size for the device as a Requestor */
	reg = pcie_rc_cfg_rd(lpp, PCIE_DCTLSTS);

	/*
	 * Request size can be larger than the MPS used, but the completions
	 * returned for the read will be bounded by the MPS size.
	 * In our system, Max request size depends on AHB/DMA burst size.
	 * It is 64 bytes. but we set it as 128 as minimum one.
	 */
	reg |= SM(PCIE_MAX_PAYLOAD_128, PCIE_DCTLSTS_MAX_READ_SIZE)
		| SM(PCIE_MAX_PAYLOAD_128, PCIE_DCTLSTS_MAX_PAYLOAD_SIZE);

	/* Enable relaxed ordering, no snoop, and all kinds of errors */
	reg |= PCIE_DCTLSTS_RELAXED_ORDERING_EN | PCIE_DCTLSTS_ERR_EN |
		PCIE_DCTLSTS_NO_SNOOP_EN;

	pcie_rc_cfg_wr(lpp, reg, PCIE_DCTLSTS);
	dev_dbg(lpp->dev, "%s PCIE_DCTLSTS: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_DCTLSTS));
}

static void pcie_link_setup(struct ltq_pcie_port *lpp)
{
	u32 reg;
#ifdef CONFIG_PCIEASPM
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;
#endif
	/*
	 * XXX, Link capability register, bit 18 for EP CLKREQ# dynamic
	 * clock management for L1, L2/3 CPM  L0s is reported during
	 * link training via TS1 order set by N_FTS
	 */
	reg = pcie_rc_cfg_rd(lpp, PCIE_LCAP);
	reg &= ~PCIE_LCAP_L0S_EIXT_LATENCY;
	reg |= SM(3, PCIE_LCAP_L0S_EIXT_LATENCY);
	pcie_rc_cfg_wr(lpp, reg, PCIE_LCAP);
	dev_dbg(lpp->dev, "%s PCIE_LCAP: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_LCAP));

	/* Link control and status register */
	reg = pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS);

	/* Link Enable, ASPM enabled  */
	reg &= ~PCIE_LCTLSTS_LINK_DISABLE;

	/*
	 * We use the same physical reference clock that the platform
	 * provides on the connector It paved the way for ASPM to calculate
	 * the new exit Latency
	 */
	reg |= PCIE_LCTLSTS_SLOT_CLK_CFG;
	reg |= PCIE_LCTLSTS_COM_CLK_CFG;
	reg &= ~PCIE_LCTLSTS_ASPM_ENABLE;
#ifdef CONFIG_PCIEASPM
	/*
	 * We should disable ASPM by default except that we have dedicated
	 * power management support  Enable ASPM will cause the system
	 * hangup/instability, performance degration
	 */
	if (soc->has_gen2)
		reg |= PCIE_LCTLSTS_ASPM_ENABLE;
	/* All other jobs left to ASPM module */
#endif /* CONFIG_PCIEASPM */

	/*
	 * The maximum size of any completion with data packet is bounded
	 * by the MPS setting in  device control register
	 */

	/*
	 * RCB may cause multiple split transactions, two options available,
	 * we use 64 byte RCB
	 */
	reg &= ~PCIE_LCTLSTS_RCB128;

	pcie_rc_cfg_wr(lpp, reg, PCIE_LCTLSTS);
	dev_dbg(lpp->dev, "%s PCIE_LCTLSTS: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS));
}

static void pcie_link_gen1_setup(struct ltq_pcie_port *lpp)
{
	u32 reg;

	reg = pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS2);
	dev_dbg(lpp->dev, "%s PCIE_LCTLSTS2 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS2));

	reg &= ~PCIE_LCTLSTS2_TGT_LINK_SPEED;
	reg |= PCIE_LCTLSTS2_HW_AUTO_DIS |
		SM(PCIE_LCTLSTS2_TGT_LINK_SPEED_25GT,
		PCIE_LCTLSTS2_TGT_LINK_SPEED);
	pcie_rc_cfg_wr(lpp, reg, PCIE_LCTLSTS2);
	dev_dbg(lpp->dev, "%s PCIE_LCTLSTS2 0x%08x 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS2), reg);
}

static void pcie_error_setup(struct ltq_pcie_port *lpp)
{
	/*
	 * Forward ERR_COR, ERR_NONFATAL, ERR_FATAL to the backbone
	 * Poisoned write TLPs and completions indicating poisoned TLPs
	 * will set the PCIe_PCICMDSTS.MDPE
	 */
	pcie_rc_cfg_wr_mask(lpp, 0, PCIE_INTRBCTRL_SERR_ENABLE |
		PCIE_INTRBCTRL_PARITY_ERR_RESP_ENABLE,
		PCIE_INTRBCTRL);
	dev_dbg(lpp->dev, "%s PCIE_INTRBCTRL: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_INTRBCTRL));

	/*
	 * Uncorrectable Error Mask Register, Unmask <enable> all bits
	 * in PCIE_UESR
	 */
	pcie_rc_cfg_wr_mask(lpp, PCIE_ALL_UNCORRECTABLE_ERR, 0, PCIE_UEMR);
	dev_dbg(lpp->dev, "%s PCIE_UEMR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_UEMR));

	/* Uncorrectable Error Severity Register, ALL errors are FATAL */
	pcie_rc_cfg_wr(lpp, PCIE_ALL_UNCORRECTABLE_ERR, PCIE_UESR);
	dev_dbg(lpp->dev, "%s PCIE_UESR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_UESR));

	/* Correctable Error Mask Register, unmask <enable> all bits */
	pcie_rc_cfg_wr_mask(lpp, PCIE_CORRECTABLE_ERR, 0, PCIE_CEMR);
	dev_dbg(lpp->dev, "%s PCIE_CEMR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_CEMR));

	/* Advanced Error Capabilities and Control Register */
	/* plat_dev_init will do the related stuff */
}

static void pcie_root_setup(struct ltq_pcie_port *lpp)
{
	/* plat_dev_init will do the related stuff */
}

static void pcie_vc_setup(struct ltq_pcie_port *lpp)
{
	/* Port VC Capability Register 2 */
	pcie_rc_cfg_wr_mask(lpp, PCIE_PVC2_VC_ARB_WRR,
		PCIE_PVC2_VC_ARB_16P_FIXED_WRR, PCIE_PVC2);
	dev_dbg(lpp->dev, "%s PCIE_PVC2: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_PVC2));

	/* VC0 Resource Capability Register */
	pcie_rc_cfg_wr_mask(lpp, PCIE_VC0_RC_REJECT_SNOOP, 0, PCIE_VC0_RC);
	dev_dbg(lpp->dev, "%s PCIE_VC0_RC: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_VC0_RC));
}

static void pcie_speed_change_setup(struct ltq_pcie_port *lpp)
{
	u32 reg;

	reg = pcie_rc_cfg_rd(lpp, PCIE_GEN2_CTRL);
	reg |= PCIE_GEN2_CTRL_SPEED_CHANGE;
	reg &= ~PCIE_GEN2_CTRL_FTS;
	reg |= SM(PCIE_GEN2_DEFAULT_FTS_NUM, PCIE_GEN2_CTRL_FTS);
	pcie_rc_cfg_wr(lpp, reg, PCIE_GEN2_CTRL);
	dev_dbg(lpp->dev, "%s PCIE_GEN2_CTRL: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_GEN2_CTRL));
}

static void pcie_port_logic_setup(struct ltq_pcie_port *lpp)
{
	u32 reg;

	/*
	 * FTS number, default 12, increase to 127, may increase
	 * time from/to L0s to L0
	 */
	reg = pcie_rc_cfg_rd(lpp, PCIE_AFR);
	reg &= ~(PCIE_AFR_FTS_NUM | PCIE_AFR_COM_FTS_NUM);
	reg |= SM(PCIE_AFR_FTS_NUM_DEFAULT, PCIE_AFR_FTS_NUM)
		| SM(PCIE_AFR_FTS_NUM_DEFAULT, PCIE_AFR_COM_FTS_NUM);
	/* L0s and L1 entry latency */
	reg &= ~(PCIE_AFR_L0S_ENTRY_LATENCY | PCIE_AFR_L1_ENTRY_LATENCY);
	reg |= SM(PCIE_AFR_L0S_ENTRY_LATENCY_DEFAULT,
		  PCIE_AFR_L0S_ENTRY_LATENCY)
		| SM(PCIE_AFR_L1_ENTRY_LATENCY_DEFAULT,
			PCIE_AFR_L1_ENTRY_LATENCY);
	pcie_rc_cfg_wr(lpp, reg, PCIE_AFR);

	dev_dbg(lpp->dev, "%s PCIE_AFR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_AFR));

	/* Port Link Control Register */
	pcie_rc_cfg_wr_mask(lpp, 0, PCIE_PLCR_DLL_LINK_EN, PCIE_PLCR);
	dev_dbg(lpp->dev, "%s PCIE_PLCR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_PLCR));

	/* Lane Skew Register */
	pcie_rc_cfg_wr_mask(lpp,
		PCIE_LSR_ACKNAK_DISABLE | PCIE_LSR_FC_DISABLE, 0, PCIE_LSR);
	dev_dbg(lpp->dev, "%s PCIE_LSR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_LSR));

	/* Symbol Timer Register and Filter Mask Register 1 */
	reg =  pcie_rc_cfg_rd(lpp, PCIE_STRFMR);

	/* Default SKP interval is very accurate already, 5us */
	/* Enable IO/CFG transaction */
	reg |= PCIE_STRFMR_RX_CFG_TRANS_ENABLE |
		PCIE_STRFMR_RX_IO_TRANS_ENABLE;
	/* Disable FC WDT */
	reg &= ~PCIE_STRFMR_FC_WDT_DISABLE;
	pcie_rc_cfg_wr(lpp, reg, PCIE_STRFMR);
	dev_dbg(lpp->dev, "%s PCIE_STRFMR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_STRFMR));

	/* Filter Masker Register 2 */
	pcie_rc_cfg_wr_mask(lpp, 0, PCIE_FMR2_VENDOR_MSG1_PASSED_TO_TRGT1 |
		PCIE_FMR2_VENDOR_MSG0_PASSED_TO_TRGT1, PCIE_FMR2);
	dev_dbg(lpp->dev, "%s PCIE_FMR2: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_FMR2));

	/* VC0 Completion Receive Queue Control Register */
	reg = pcie_rc_cfg_rd(lpp, PCIE_VC0_CRQCR);
	reg &= ~PCIE_VC0_CRQCR_CPL_TLP_QUEUE_MODE;
	reg |= SM(PCIE_VC0_TLP_QUEUE_MODE_BYPASS,
			PCIE_VC0_CRQCR_CPL_TLP_QUEUE_MODE);
	pcie_rc_cfg_wr(lpp, reg, PCIE_VC0_CRQCR);
	dev_dbg(lpp->dev, "%s PCIE_VC0_CRQCR: 0x%08x\n",
		__func__, pcie_rc_cfg_rd(lpp, PCIE_VC0_CRQCR));
}

static void pcie_rc_cfg_reg_setup(struct ltq_pcie_port *lpp)
{
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	pcie_ltssm_disable(lpp);
	pcie_mem_io_setup(lpp);
	pcie_msi_setup(lpp);
	pcie_pm_setup(lpp);
	pcie_bus_setup(lpp);
	pcie_device_setup(lpp);
	pcie_link_setup(lpp);
	if (soc->has_gen2) {
		pcie_speed_change_setup(lpp);
		if (pcie_force_gen1)
			pcie_link_gen1_setup(lpp);
	}
	pcie_error_setup(lpp);
	pcie_root_setup(lpp);
	pcie_vc_setup(lpp);
	pcie_port_logic_setup(lpp);
}

static int pcie_wait_phy_link_up(struct ltq_pcie_port *lpp)
{
#define PCIE_PHY_LINK_UP_TIMEOUT	3000 /* XXX, tunable */
#define PCIE_DLL_TIMEOUT		10
	int i;
	int rc_port = lpp->id;

	/* Wait for PHY link is up */
	for (i = 0; i < PCIE_PHY_LINK_UP_TIMEOUT; i++) {
		if (pcie_link_up(lpp))
			break;

		udelay(100);
	}
	if (i >= PCIE_PHY_LINK_UP_TIMEOUT) {
		pr_err("%s port %d timeout\n", __func__, rc_port);
		return -1;
	}
	/* Check data link up or not */
	for (i = 0; i < PCIE_DLL_TIMEOUT; i++) {
		if ((pcie_app_rd(lpp, PCIE_RC_DR) & PCIE_RC_DR_DLL_UP))
			break;
		udelay(1);
	}
	if (i >= PCIE_DLL_TIMEOUT) {
		pr_err("%s port %d DLL link is still down\n",
			__func__, rc_port);
		return -1;
	}

	/* Check Data link active or not */
	if (!(pcie_rc_cfg_rd(lpp, PCIE_LCTLSTS)
		& PCIE_LCTLSTS_DLL_ACTIVE)) {
		pr_err("%s port %d DLL is not active\n",
			__func__, rc_port);
		return -1;
	}
	return 0;
#undef PCIE_DLL_TIMEOUT
#undef PCIE_PHY_LINK_UP_TIMEOUT
}

static int pcie_app_loigc_setup(struct ltq_pcie_port *lpp)
{
#ifdef LTQ_PCIE_PHY_DBG
	pcie_disable_scrambling(lpp);
#endif /* LTQ_PCIE_PHY_DBG */
	pcie_ahb_bus_error_suppress(lpp);
	/* Pull PCIe EP out of reset */
	pcie_device_rst_deassert(lpp);
	/* Start LTSSM training between RC and EP */
	pcie_ltssm_enable(lpp);
	/* Check PHY status after enabling LTSSM */
	if (pcie_wait_phy_link_up(lpp) != 0)
		return -1;

	return 0;
}

/*
 * Table 359 Enhanced Configuration Address Mapping1)
 * 1) This table is defined in Table 7-1, page 341, PCI Express
 * Base Specification v1.1
 * Memory Address PCI Express Configuration Space
 * A[(20+n-1):20] Bus Number 1 < n < 8
 * A[19:15] Device Number
 * A[14:12] Function Number
 * A[11:8] Extended Register Number
 * A[7:2] Register Number
 * A[1:0] Along with size of the access, used to generate Byte Enables
 * For VR9, only the address bits [22:0] are mapped to the configuration space:
 * . Address bits [22:20] select the target bus (1-of-8)1)
 * . Address bits [19:15] select the target device (1-of-32) on the bus
 * . Address bits [14:12] select the target function (1-of-8) within the device.
 * . Address bits [11:2] selects the target dword (1-of-1024) within
 *   the selected function.s configuration space
 * .Address bits [1:0] define the start byte location within the selected dword.
 */
static u32 pcie_bus_addr(u8 bus_num, u16 devfn, int where)
{
	u32 addr;
	u8 bus;

	if (!bus_num) {
		/* type 0 */
		addr = ((PCI_SLOT(devfn) & 0x1F) << 15)
		| ((PCI_FUNC(devfn) & 0x7) << 12)
		| ((where & 0xFFF) & ~3);
	} else {
		bus = bus_num;
		/* type 1, only support 8 buses */
		addr = ((bus & 0x7) << 20)
		| ((PCI_SLOT(devfn) & 0x1F) << 15)
		| ((PCI_FUNC(devfn) & 0x7) << 12)
		| ((where & 0xFFF) & ~3);
	}
	pcie_dbg(PCIE_MSG_CFG,
		"%s: bus addr : %02x:%02x.%01x/%02x, addr=%08x\n",
		__func__, bus_num, PCI_SLOT(devfn), PCI_FUNC(devfn),
		where, addr);
	return addr;
}

static int pcie_valid_config(struct ltq_pcie_port *lpp, int bus, int dev)
{
	/* RC itself */
	if ((bus == 0) && (dev == 0))
		return 1;

	/* No physical link */
	if (!pcie_link_up(lpp))
		return 0;

	/* Bus zero only has RC itself
	 * XXX, check if EP will be integrated
	 */
	if ((bus == 0) && (dev != 0))
		return 0;

	/* Maximum 8 buses supported */
	if (bus > 8)
		return 0;

	/*
	 * PCIe is PtP link, one bus only supports only one device
	 * except bus zero and PCIe switch which is virtual bus device
	 * The following two conditions really depends on the system design
	 * and attached the device.
	 * XXX, how about more new switch
	 */
	if ((bus == 1) && (dev != 0))
		return 0;

	if ((bus >= 3) && (dev != 0))
		return 0;
	return 1;
}

static u32 pcie_bus_enum_read_hack(int bus_nr_offset, int where, u32 value)
{
	u32 tvalue = value;

	if (where == PCI_PRIMARY_BUS) {
		u8 primary, secondary, subordinate;

		primary = tvalue & 0xFF;
		secondary = (tvalue >> 8) & 0xFF;
		subordinate = (tvalue >> 16) & 0xFF;
		primary += bus_nr_offset;
		secondary += bus_nr_offset;
		subordinate += bus_nr_offset;
		tvalue = (tvalue & 0xFF000000) | (u32) primary |
			(u32) (secondary << 8) | (u32) (subordinate << 16);
	}
	return tvalue;
}

static u32 pcie_bus_enum_write_hack(int bus_nr_offset, int where, u32 value)
{
	u32 tvalue = value;

	if (where == PCI_PRIMARY_BUS) {
		u8 primary, secondary, subordinate;

		primary = tvalue & 0xFF;
		secondary = (tvalue >> 8) & 0xFF;
		subordinate = (tvalue >> 16) & 0xFF;
		if (primary > 0 && primary != 0xFF)
			primary -= bus_nr_offset;

		if (secondary > 0 && secondary != 0xFF)
			secondary -= bus_nr_offset;

		if (subordinate > 0 && subordinate != 0xFF)
			subordinate -= bus_nr_offset;

		tvalue = (tvalue & 0xFF000000) | (u32) primary |
			(u32) (secondary << 8) | (u32) (subordinate << 16);
	} else if (where == PCI_SUBORDINATE_BUS) {
		u8 subordinate = tvalue & 0xFF;

		subordinate =
			subordinate > 0 ? subordinate - bus_nr_offset : 0;
		tvalue = subordinate;
	}
	return tvalue;
}

static int pcie_dev_header_type_bridge(struct pci_bus *bus, u32 devfn)
{
	int ret = 1;
	struct pci_dev *pdev;

	/* Sanity check */
	pdev = pci_get_slot(bus, devfn);
	if (pdev == NULL) {
		ret = 0;
		goto out;
	}

	/* Only care about PCI bridge */
	if (pdev->hdr_type != PCI_HEADER_TYPE_BRIDGE)
		ret = 0;

out:
	return ret;
}

/**
 * \fn static int pcie_read_config(struct pci_bus *bus, u32 devfn,
 *                   int where, int size, u32 *value)
 * \brief Read a value from configuration space
 *
 * \param[in] bus    Pointer to pci bus
 * \param[in] devfn  PCI device function number
 * \param[in] where  PCI register number
 * \param[in] size   Register read size
 * \param[out] value    Pointer to return value
 * \return  PCIBIOS_BAD_REGISTER_NUMBER Invalid register number
 * \return  PCIBIOS_FUNC_NOT_SUPPORTED  PCI function not supported
 * \return  PCIBIOS_DEVICE_NOT_FOUND    PCI device not found
 * \return  PCIBIOS_SUCCESSFUL          OK
 * \ingroup PCIE_OS
 */
static int pcie_read_config(struct pci_bus *bus, u32 devfn,
		int where, int size, u32 *value)
{
	u32 data = 0;
	int bus_number = bus->number;
	int bus_nr_offset;
	static const u32 mask[8] = { 0, 0xff, 0xffff, 0, 0xffffffff, 0, 0, 0 };
	int ret = PCIBIOS_SUCCESSFUL;
	unsigned long flags;
	struct ltq_pcie_port *lpp = bus_to_ltq_pcie_port(bus);


	if (unlikely(size != 1 && size != 2 && size != 4)) {
		ret = PCIBIOS_BAD_REGISTER_NUMBER;
		goto out;
	}

	/* Make sure the address is aligned to natural boundary */
	if (unlikely(((size - 1) & where))) {
		ret = PCIBIOS_BAD_REGISTER_NUMBER;
		goto out;
	}

	/*
	 * If we are not the first controller, we have to deduce the bus number
	 * before the current bus so that it assume its bus number starts
	 * from 0 in host controller
	 */
	bus_nr_offset = pci_find_preexist_bus_nr(bus);
	bus_number -= bus_nr_offset;

	/*
	 * We need to force the bus number to be zero on the root
	 * bus. Linux numbers the 2nd root bus to start after all
	 * busses on root 0.
	 */
	if (bus->parent == NULL)
		bus_number = 0;


	/*
	 * PCIe only has a single device connected to it. It is
	 * always device ID 0. Don't bother doing reads for other
	 * device IDs on the first segment.
	 */
	if ((bus_number == 0) && (PCI_SLOT(devfn) != 0)) {
		ret = PCIBIOS_FUNC_NOT_SUPPORTED;
		goto out;
	}

	if (pcie_valid_config(lpp, bus_number, PCI_SLOT(devfn)) == 0) {
		*value = 0xffffffff;
		ret = PCIBIOS_DEVICE_NOT_FOUND;
		goto out;
	}

	pcie_dbg(PCIE_MSG_READ_CFG, "%s: %02x:%02x.%01x/%02x:%01d\n",
		__func__, bus_number, PCI_SLOT(devfn),
		PCI_FUNC(devfn), where, size);

	spin_lock_irqsave(&lpp->lock, flags);
	if (bus_number == 0) { /* RC itself */
		u32 t;

		t = (where & ~3);
		data = pcie_rc_cfg_rd(lpp, t);
		pcie_dbg(PCIE_MSG_READ_CFG,
			"%s: rd local cfg, offset:%08x, data:%08x\n",
			__func__, t, data);
	} else {
		u32 addr = pcie_bus_addr(bus_number, devfn, where);

		data = pcie_cfg_rd(lpp, addr);
		if (lpp->outbound_swap)
			data = le32_to_cpu(data);
	}

	/* To get a correct PCI topology, we have to restore
	 * the bus number to OS */
	if (pcie_dev_header_type_bridge(bus, devfn))
		data = pcie_bus_enum_read_hack(bus_nr_offset, where, data);

	spin_unlock_irqrestore(&lpp->lock, flags);
	pcie_dbg(PCIE_MSG_READ_CFG,
		"%s: read config: data=%08x raw=%08x\n", __func__,
		(data >> (8 * (where & 3))) & mask[size & 7], data);

	*value = (data >> (8 * (where & 3))) & mask[size & 7];
out:
	return ret;
}

static u32 pcie_size_to_value(int where, int size, u32 data, u32 value)
{
	u32 shift;
	u32 tdata = data;

	switch (size) {
	case 1:
		shift = (where & 0x3) << 3;
		tdata &= ~(0xffU << shift);
		tdata |= ((value & 0xffU) << shift);
		break;
	case 2:
		shift = (where & 3) << 3;
		tdata &= ~(0xffffU << shift);
		tdata |= ((value & 0xffffU) << shift);
		break;
	case 4:
		tdata = value;
		break;
	}
	return tdata;
}

/**
 * \fn static static int pcie_write_config(struct pci_bus *bus, u32 devfn,
 *                 int where, int size, u32 value)
 * \brief Write a value to PCI configuration space
 *
 * \param[in] bus    Pointer to pci bus
 * \param[in] devfn  PCI device function number
 * \param[in] where  PCI register number
 * \param[in] size   The register size to be written
 * \param[in] value  The valule to be written
 * \return PCIBIOS_BAD_REGISTER_NUMBER Invalid register number
 * \return PCIBIOS_DEVICE_NOT_FOUND    PCI device not found
 * \return PCIBIOS_SUCCESSFUL          OK
 * \ingroup PCIE_OS
 */
static int pcie_write_config(struct pci_bus *bus, u32 devfn,
		      int where, int size, u32 value)
{
	int bus_number = bus->number;
	int bus_nr_offset;
	int ret = PCIBIOS_SUCCESSFUL;
	struct ltq_pcie_port *lpp = bus_to_ltq_pcie_port(bus);
	u32 tvalue = value;
	u32 data;
	unsigned long flags;

	/* Make sure the address is aligned to natural boundary */
	if (unlikely(((size - 1) & where))) {
		ret = PCIBIOS_BAD_REGISTER_NUMBER;
		goto out;
	}

	/*
	 * If we are not the first controller, we have to deduce
	 * the bus number before the current bus
	 * so that it assume its bus number starts from 0 in host controller
	 */
	bus_nr_offset = pci_find_preexist_bus_nr(bus);
	bus_number -= bus_nr_offset;

	/*
	 * We need to force the bus number to be zero on the root
	 * bus. Linux numbers the 2nd root bus to start after all
	 * busses on root 0.
	 */
	if (bus->parent == NULL)
		bus_number = 0;

	if (pcie_valid_config(lpp, bus_number, PCI_SLOT(devfn)) == 0) {
		ret = PCIBIOS_DEVICE_NOT_FOUND;
		goto out;
	}

	pcie_dbg(PCIE_MSG_WRITE_CFG,
		"%s: %02x:%02x.%01x/%02x:%01d value=%08x\n",
		__func__, bus_number, PCI_SLOT(devfn),
		PCI_FUNC(devfn), where, size, value);

	/* XXX, some PCIe device may need some delay */
	spin_lock_irqsave(&lpp->lock, flags);
	/*
	 * To configure the correct bus topology using native way,
	 * we have to cheat Os so that it can configure the PCIe
	 * hardware correctly.
	 */
	if (pcie_dev_header_type_bridge(bus, devfn))
		tvalue = pcie_bus_enum_write_hack(bus_nr_offset,
			where, value);

	if (bus_number == 0) {/* RC itself */
		u32 t;

		t = (where & ~3);
		pcie_dbg(PCIE_MSG_WRITE_CFG,
			"%s: wr local cfg, offset:%08x, fill:%08x\n",
			__func__, t, value);
		data = pcie_rc_cfg_rd(lpp, t);
		pcie_dbg(PCIE_MSG_WRITE_CFG,
			"%s: rd local cfg, offset:%08x, data:%08x\n",
			__func__, t, data);

		data = pcie_size_to_value(where, size, data, tvalue);

		pcie_dbg(PCIE_MSG_WRITE_CFG,
			"%s: wr local cfg, offset:%08x, value:%08x\n",
			__func__, t, data);
		pcie_rc_cfg_wr(lpp, data, t);
		pcie_dbg(PCIE_MSG_WRITE_CFG,
			"%s: rd local cfg, offset:%08x, value:%08x\n",
			__func__, t, pcie_rc_cfg_rd(lpp, t));
	} else {
		u32 addr = pcie_bus_addr(bus_number, devfn, where);

		pcie_dbg(PCIE_MSG_WRITE_CFG,
			"%s: wr cfg, offset:%08x, fill:%08x\n",
			__func__, addr, value);
		data = pcie_cfg_rd(lpp, addr);

		if (lpp->outbound_swap)
			data = le32_to_cpu(data);

		pcie_dbg(PCIE_MSG_WRITE_CFG,
			"%s: rd cfg, offset:%08x, data:%08x\n",
			__func__, addr, data);

		data = pcie_size_to_value(where, size, data, tvalue);
		if (lpp->outbound_swap)
			data = cpu_to_le32(data);
		pcie_dbg(PCIE_MSG_WRITE_CFG,
			"%s: wr cfg, offset:%08x, value:%08x\n",
			__func__, addr, data);
		pcie_cfg_wr(lpp, data, addr);
		pcie_dbg(PCIE_MSG_WRITE_CFG,
			"%s: rd cfg, offset:%08x, value:%08x\n",
			__func__, addr, pcie_cfg_rd(lpp, addr));
	}
	spin_unlock_irqrestore(&lpp->lock, flags);
out:
	return ret;
}

#ifdef LTQ_PCIE_BASIC_ERROR_INT

#ifdef LTQ_PCIE_DBG
static const char * const pcie_err_str[] = {
	"AER",
	"AER-MSI(X)",
	"PME interrupt",
	"Hotplug",
	"Vendor Msg",
	"Correctable Error",
	"Non-Fatal Error",
	"Fatal Error",
	"PME Msg",
	"PME Turn Off Ack",
	"AHB Fatal Error",
	"Link Auto BW",
	"BW Mgmt",
	"INTA",
	"INTB",
	"INTC",
	"INTD",
	"Wakeup"
};

static const char *pcie_errno_to_str(u32 value)
{
	/* If multiple interrupt bits set, only the first low bit reported */
	if (ffs(value) > ARRAY_SIZE(pcie_err_str))
		return "Spurious Interrupt";
	else
		return pcie_err_str[ffs(value) - 1];
}
#endif /* LTQ_PCIE_DBG */

static void pcie_core_int_clear_all(int irq, struct pci_dev *dev, void *context)
{
	u32 reg;
	struct ltq_pcie_port *lpp;
	lpp = bus_to_ltq_pcie_port(dev->bus);

	reg = pcie_app_rd(lpp, PCIE_IRNCR);
#ifdef LTQ_PCIE_DBG
	dev_dbg(lpp->dev, "%s port %d PCIE_IRNCR: %s\n",
		__func__, lpp->id,
		pcie_errno_to_str(pcie_app_rd(lpp, PCIE_IRNCR)));
#endif
#ifdef LTQ_PCIE_AER_REPORT
	if ((reg & PCIE_IRN_AER_REPORT) != 0)
		aer_irq(irq, context);

#endif /* LTQ_PCIE_AER_REPORT */
	reg &= PCIE_RC_CORE_COMBINED_INT;
	pcie_app_wr(lpp, reg, PCIE_IRNCR);
}

static irqreturn_t
pcie_rc_core_isr(int irq, void *context)
{
	struct pci_dev *dev = aer_dev_to_pci_dev(context);
	pcie_core_int_clear_all(irq, dev, context);
	return IRQ_HANDLED;
}

static void pcie_rc_core_int_init(struct ltq_pcie_port *lpp)
{
	/* Disable core interrupt */
	pcie_app_wr_mask(lpp, PCIE_RC_CORE_COMBINED_INT, 0, PCIE_IRNEN);
	dev_dbg(lpp->dev, "%s PCIE_IRNEN: 0x%08x\n",
		__func__, pcie_app_rd(lpp, PCIE_IRNEN));

	/* Clear it first */
	pcie_app_wr_mask(lpp, PCIE_RC_CORE_COMBINED_INT,
		PCIE_RC_CORE_COMBINED_INT,  PCIE_IRNCR);
	dev_dbg(lpp->dev, "%s PCIE_IRNCR: 0x%08x\n",
		__func__, pcie_app_rd(lpp, PCIE_IRNCR));
}

int pcie_rc_aer_irq_register(struct pci_dev *dev, void *context)
{
	int ret = 0;
	struct ltq_pcie_port *lpp;
	struct platform_device *pdev;

	lpp = bus_to_ltq_pcie_port(dev->bus);
	pdev = to_platform_device(lpp->dev);
	lpp->err_irq = platform_get_irq_byname(pdev, "ir");
	if (lpp->err_irq  <= 0) {
		dev_err(&pdev->dev, "failed to get pcie err irq\n");
		return -ENODEV;
	}
	sprintf(lpp->irq_name, "pci_ir%d", lpp->id);
	ret = devm_request_irq(&pdev->dev, lpp->err_irq,
		pcie_rc_core_isr, IRQF_DISABLED, lpp->irq_name, context);
	if (ret) {
		dev_err(&pdev->dev, "%s request irq %d failed\n",
			__func__, lpp->err_irq);
		return -ENODEV;
	}
	/* Enable error interrupts */
	pcie_app_wr_mask(lpp, PCIE_RC_CORE_COMBINED_INT,
		PCIE_RC_CORE_COMBINED_INT, PCIE_IRNEN);
	return ret;
}
EXPORT_SYMBOL_GPL(pcie_rc_aer_irq_register);
#endif /* LTQ_PCIE_BASIC_ERROR_INT */

/**
 * \fn int ltq_pcie_bios_map_irq(struct pci_dev *dev, u8 slot, u8 pin)
 * \brief Map a PCI device to the appropriate interrupt line
 *
 * \param[in] dev    The Linux PCI device structure for the device to map
 * \param[in] slot   The slot number for this device on __BUS 0__. Linux
 *                   enumerates through all the bridges and figures out the
 *                   slot on Bus 0 where this device eventually hooks to.
 * \param[in] pin    The PCI interrupt pin read from the device, then swizzled
 *                   as it goes through each bridge.
 * \return Interrupt number for the device
 * \ingroup PCIE_OS
 */
static int ltq_pcie_bios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
	struct of_irq dev_irq;
	u32 irq_bit = 0;
	int irq = 0;
	struct ltq_pcie_port *lpp;
	lpp = bus_to_ltq_pcie_port(dev->bus);

	dev_dbg(lpp->dev,
		"%s port %d dev %s slot %d pin %d\n", __func__,
		lpp->id, pci_name(dev), slot, pin);

	if ((pin == PCIE_LEGACY_DISABLE) || (pin > PCIE_LEGACY_INT_MAX)) {
		dev_warn(lpp->dev, "WARNING: dev %s: invalid interrupt pin %d\n",
			pci_name(dev), pin);
		return -1;
	}
	if (of_irq_map_pci(dev, &dev_irq)) {
		dev_err(lpp->dev,
		"trying to map irq for unknown slot:%d pin:%d\n",
			slot, pin);
		return 0;
	}
	irq = irq_create_of_mapping(dev_irq.controller,
		dev_irq.specifier, dev_irq.size);

	/* Pin to irq offset bit position */
	irq_bit = BIT((pin + PCIE_INTX_OFFSET));

	/* Clear possible pending interrupts first */
	pcie_app_wr_mask(lpp, 0, irq_bit, PCIE_IRNCR);
	dev_dbg(lpp->dev, "%s PCIE_IRNCR: 0x%08x\n",
		__func__, pcie_app_rd(lpp, PCIE_IRNCR));

	pcie_app_wr_mask(lpp, 0, irq_bit, PCIE_IRNEN);
	dev_dbg(lpp->dev,  "%s PCIE_IRNEN: 0x%08x\n",
		__func__, pcie_app_rd(lpp, PCIE_IRNEN));
	dev_dbg(lpp->dev, "%s dev %s irq %d assigned\n",
		__func__, pci_name(dev), irq);
	dev_dbg(lpp->dev, "%s dev %s: exit\n", __func__, pci_name(dev));
	return irq;
}

/**
 * \fn int ltq_pcie_bios_plat_dev_init(struct pci_dev *dev)
 * \brief Called to perform platform specific PCI setup
 *
 * \param[in] dev The Linux PCI device structure for the device to map
 * \return OK
 * \ingroup PCIE_OS
 */
static int ltq_pcie_bios_plat_dev_init(struct pci_dev *dev)
{
	u16 config;
#ifdef LTQ_PCIE_BASIC_ERROR_INT
	int pos;
#ifdef LTQ_PCIE_AER_REPORT
	u32 dconfig;
#endif /* LTQ_PCIE_AER_REPORT */
#endif /* LTQ_PCIE_BASIC_ERROR_INT */
	struct ltq_pcie_port *lpp;
	const struct ltq_pcie_soc_data *soc;

	lpp = bus_to_ltq_pcie_port(dev->bus);
	soc = lpp->soc_data;

	/* Enable reporting System errors and parity errors on all devices */
	/* Enable parity checking and error reporting */
	pci_read_config_word(dev, PCI_COMMAND, &config);
	config |= PCI_COMMAND_PARITY | PCI_COMMAND_SERR
		/*| PCI_COMMAND_INVALIDATE | PCI_COMMAND_FAST_BACK */;
	pci_write_config_word(dev, PCI_COMMAND, config);

	if (dev->subordinate) {
		/* Set latency timers on sub bridges */
		pci_write_config_byte(dev, PCI_SEC_LATENCY_TIMER, 0x40);
		/* More bridge error detection */
		pci_read_config_word(dev, PCI_BRIDGE_CONTROL, &config);
		config |= PCI_BRIDGE_CTL_PARITY | PCI_BRIDGE_CTL_SERR;
		pci_write_config_word(dev, PCI_BRIDGE_CONTROL, config);
	}
#ifdef LTQ_PCIE_BASIC_ERROR_INT
	/* Enable the PCIe normal error reporting */
	pos = pci_find_capability(dev, PCI_CAP_ID_EXP);
	if (pos) {
		/* Disable system error generation in response
		   to error messages */
		pci_read_config_word(dev, pos + PCI_EXP_RTCTL, &config);
		config &= ~(PCI_EXP_RTCTL_SECEE
			| PCI_EXP_RTCTL_SENFEE | PCI_EXP_RTCTL_SEFEE);
		pci_write_config_word(dev, pos + PCI_EXP_RTCTL, config);

		/* Clear PCIE Capability's Device Status */
		pci_read_config_word(dev, pos + PCI_EXP_DEVSTA, &config);
		pci_write_config_word(dev, pos + PCI_EXP_DEVSTA, config);

		/* Update Device Control */
		pci_read_config_word(dev, pos + PCI_EXP_DEVCTL, &config);
		/* Correctable Error Reporting */
		config |= PCI_EXP_DEVCTL_CERE;
		/* Non-Fatal Error Reporting */
		config |= PCI_EXP_DEVCTL_NFERE;
		/* Fatal Error Reporting */
		config |= PCI_EXP_DEVCTL_FERE;
		/* Unsupported Request */
		config |= PCI_EXP_DEVCTL_URRE;
		pci_write_config_word(dev, pos + PCI_EXP_DEVCTL, config);
	}
#ifdef LTQ_PCIE_AER_REPORT
	/* Find the Advanced Error Reporting capability */
	pos = pci_find_ext_capability(dev, PCI_EXT_CAP_ID_ERR);
	if (pos) {
		/* Clear Uncorrectable Error Status */
		pci_read_config_dword(dev, pos + PCI_ERR_UNCOR_STATUS,
				&dconfig);
		pci_write_config_dword(dev, pos + PCI_ERR_UNCOR_STATUS,
				dconfig);
		/* Enable reporting of all uncorrectable errors */
		/* Uncorrectable Error Mask - turned on bits disable errors */
		pci_write_config_dword(dev, pos + PCI_ERR_UNCOR_MASK, 0);
		/*
		 * Leave severity at HW default. This only controls if
		 * errors are reported as uncorrectable or
		 * correctable, not if the error is reported.
		 */
		/* PCI_ERR_UNCOR_SEVER - Uncorrectable Error Severity */
		/* Clear Correctable Error Status */
		pci_read_config_dword(dev, pos + PCI_ERR_COR_STATUS,
				&dconfig);
		pci_write_config_dword(dev, pos + PCI_ERR_COR_STATUS,
				dconfig);
		/* Enable reporting of all correctable errors */
		/* Correctable Error Mask - turned on bits disable errors */
		pci_write_config_dword(dev, pos + PCI_ERR_COR_MASK, 0);
		/* Advanced Error Capabilities */
		pci_read_config_dword(dev, pos + PCI_ERR_CAP, &dconfig);
		/* ECRC Generation Enable */
		if (dconfig & PCI_ERR_CAP_ECRC_GENC)
			dconfig |= PCI_ERR_CAP_ECRC_GENE;

		/* ECRC Check Enable */
		if (dconfig & PCI_ERR_CAP_ECRC_CHKC)
			dconfig |= PCI_ERR_CAP_ECRC_CHKE;

		pci_write_config_dword(dev, pos + PCI_ERR_CAP, dconfig);

		/* PCI_ERR_HEADER_LOG - Header Log Register (16 bytes) */
		/* Enable Root Port's interrupt in response to error messages */
		pci_write_config_dword(dev, pos + PCI_ERR_ROOT_COMMAND,
				PCI_ERR_ROOT_CMD_COR_EN |
				PCI_ERR_ROOT_CMD_NONFATAL_EN |
				PCI_ERR_ROOT_CMD_FATAL_EN);
		/* Clear the Root status register */
		pci_read_config_dword(dev, pos + PCI_ERR_ROOT_STATUS,
			&dconfig);
		pci_write_config_dword(dev, pos + PCI_ERR_ROOT_STATUS,
			dconfig);
	}
#endif /* LTQ_PCIE_AER_REPORT */
#endif /* LTQ_PCIE_BASIC_ERROR_INT */
	/*
	 * WAR, only 128 MRRS is supported, force all EPs to support
	 * this value for all legacy SoCs
	 */
	if (!soc->has_gen2)
		pcie_set_readrq(dev, 128);
	return 0;
}

static void pcie_phy_rst(struct ltq_pcie_port *lpp)
{
	pcie_phy_rst_assert(lpp);
	pcie_phy_rst_deassert(lpp);

	/* Make sure PHY PLL is stable */
	udelay(20);
}

static int pcie_rc_initialize(struct ltq_pcie_port *lpp)
{
	int i;
	int loopcnt;
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

#define PCIE_PHY_LOOP_CNT	5

	if (soc->has_gen2)
		loopcnt = 1;
	else
		loopcnt = PCIE_PHY_LOOP_CNT;

	pcie_ahb_pmu_setup(lpp);

	pcie_rcu_endian_setup(lpp);

	if (pcie_ep_rst_init(lpp))
		return -EINVAL;

#ifndef CONFIG_USE_EMULATOR
	if (soc->has_gen2)
		pcie_lcpll_init(lpp);
#endif /* CONFIG_USE_EMULATION */
	/*
	 * XXX, PCIe elastic buffer bug will cause not to be detected. One more
	 * reset PCIe PHY will solve this issue
	 */
	for (i = 0; i < loopcnt; i++) {
		/* Disable PCIe PHY Analog part for sanity check */
		pcie_phy_pmu_disable(lpp);
		pcie_phy_rst(lpp);
		/* PCIe Core reset enabled, low active, sw programmed */
		pcie_core_rst_assert(lpp);
		/* Put PCIe EP in reset status */
		pcie_device_rst_assert(lpp);
		/* PCI PHY & Core reset disabled, high active, sw programmed */
		pcie_core_rst_deassert(lpp);
	#ifndef CONFIG_USE_EMULATOR
		/* Already in a quiet state, program PLL, enable PHY,
		 * check ready bit */
		if (!soc->has_gen2) {
			if (pcie_phy_clock_mode_setup(lpp) != 0)
				return -EIO;
		}
	#endif /* CONFIG_USE_EMULATOR */

		/* Enable PCIe PHY and Clock */
		pcie_core_pmu_setup(lpp);

		pcie_phy_clk_out(lpp);

		/* Clear status registers */
		pcie_status_register_clear(lpp);

		if (IS_ENABLED(CONFIG_PCI_MSI))
			pcie_msi_init(lpp);

		pcie_rc_cfg_reg_setup(lpp);

		/* Once link is up, break out */
		if (pcie_app_loigc_setup(lpp) == 0)
			break;
	}
	if (i >= loopcnt) {
		pr_err("%s port %d link up failed!!!!!\n",
			__func__, lpp->id);
		return -EIO;
	}
	/*
	 * NB, don't increase ACK/NACK timer timeout value, which will
	 * cause a lot of COR errors
	 */
	pcie_replay_time_update(lpp);
#ifdef LTQ_PCIE_DBG
	pcie_post_dump(lpp);
	pcie_status_registers_dump(lpp);
#endif /* LTQ_PCIE_DBG */
	return 0;
}

static int ltq_pcie_clks_get(struct ltq_pcie_port *lpp)
{
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	if (soc->has_phy_clk) {
		lpp->phy_clk = devm_clk_get(lpp->dev, "phy");
		if (IS_ERR(lpp->phy_clk))
			return PTR_ERR(lpp->phy_clk);
	}

	lpp->core_clk = devm_clk_get(lpp->dev, "ctl");
	if (IS_ERR(lpp->core_clk))
		return PTR_ERR(lpp->core_clk);

	if (soc->has_msi_clk) {
		lpp->msi_clk = devm_clk_get(lpp->dev, "msi");
		if (IS_ERR(lpp->msi_clk))
			return PTR_ERR(lpp->msi_clk);
	}

	if (soc->has_pdi_clk) {
		lpp->pdi_clk = devm_clk_get(lpp->dev, "pdi");
		if (IS_ERR(lpp->pdi_clk))
			return PTR_ERR(lpp->pdi_clk);
	}

	if (soc->has_ahb_clk) {
		lpp->ahb_clk = clk_get(NULL, "ahb");
		if (IS_ERR(lpp->ahb_clk))
			return PTR_ERR(lpp->ahb_clk);
	}

	if (soc->has_bus_clk) {
		lpp->bus_clk = devm_clk_get(lpp->dev, "bus");
		if (IS_ERR(lpp->bus_clk))
			return PTR_ERR(lpp->bus_clk);
	}
	return 0;
}

static int ltq_pcie_resets_get(struct ltq_pcie_port *lpp)
{
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	lpp->phy_rst = devm_reset_control_get(lpp->dev, "phy");
	if (IS_ERR(lpp->phy_rst))
		return PTR_ERR(lpp->phy_rst);

	if (soc->has_core_rst) {
		lpp->core_rst = devm_reset_control_get(lpp->dev, "core");
		if (IS_ERR(lpp->core_rst))
			return PTR_ERR(lpp->core_rst);
	}
	return 0;
}

static int pcie_parse_dt(struct ltq_pcie_port *lpp)
{
	u32 prop;
	struct resource *res;
	struct device *dev = lpp->dev;
	struct device_node *np = dev->of_node;
	struct platform_device *pdev;

	pdev = to_platform_device(lpp->dev);

	if (!of_property_read_u32(np, "lantiq,rst-high-active", &prop))
		lpp->high_active_rst = prop;

	if (!of_property_read_u32(np, "lantiq,rst-interval", &prop))
		lpp->rst_interval = prop;

	if (!of_property_read_u32(np, "lantiq,inbound-swap", &prop))
		lpp->inbound_swap = prop;

	if (!of_property_read_u32(np, "lantiq,outbound-swap", &prop))
		lpp->outbound_swap = prop;

	if (!of_property_read_u32(np, "lantiq,phy-mode", &prop))
		lpp->phy_mode = prop;
	dev_dbg(dev,
		"rst %d rst_interval %d inbound_swap %d, outbound_swap %d phy_mode %d\n",
		lpp->high_active_rst, lpp->rst_interval,
		lpp->inbound_swap, lpp->outbound_swap, lpp->phy_mode);

	if (!of_property_read_u32(np, "lantiq,inbound-shift", &prop))
		lpp->inbound_shift = prop;

	if (!of_property_read_u32(np, "lantiq,outbound-shift", &prop))
		lpp->outbound_shift = prop;

	if (!of_property_read_u32(np, "lantiq,clkout", &prop))
		lpp->clk_out_pin = prop;

	dev_dbg(dev,
		"inbound-shift %d outbound-shift %d clk_out_pin %d\n",
		lpp->inbound_shift, lpp->outbound_shift, lpp->clk_out_pin);

	/* Controller Cfg Register */
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "csr");
	if (!res)
		return -EINVAL;
	lpp->mem_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(lpp->mem_base))
		return PTR_ERR(lpp->mem_base);
	dev_dbg(dev, "%s rc %d csr %p\n", __func__, lpp->id, lpp->mem_base);

	/* dev cfg space */
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "cfg");
	if (!res)
		return -EINVAL;
	lpp->devcfg_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(lpp->devcfg_base))
		return PTR_ERR(lpp->devcfg_base);
	dev_dbg(dev, "%s rc cfg %p\n", __func__, lpp->devcfg_base);

	/* Applicaton logic */
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "app");
	if (!res)
		return -EINVAL;
	lpp->app_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(lpp->app_base))
		return PTR_ERR(lpp->app_base);
	dev_dbg(dev, "%s rc app_base %p\n", __func__, lpp->app_base);

	/* phy reg */
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "phy");
	if (!res)
		return -EINVAL;
	lpp->phy_base = devm_ioremap_resource(dev, res);
	if (IS_ERR(lpp->phy_base))
		return PTR_ERR(lpp->phy_base);
	dev_dbg(dev, "%s rc phy_base %p\n", __func__, lpp->phy_base);

	/* msi physical space */
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "msi");
	if (!res)
		return -EINVAL;
	lpp->msi_base = res->start;
	dev_dbg(dev, "%s rc msi_base 0x%08x\n", __func__, (u32)lpp->msi_base);

	/* msi pic base */
	res = platform_get_resource_byname(pdev, IORESOURCE_MEM, "pic");
	if (!res)
		return -EINVAL;
	lpp->msi_pic = devm_ioremap_resource(dev, res);
	if (IS_ERR(lpp->msi_pic))
		return PTR_ERR(lpp->msi_pic);
	dev_dbg(dev, "%s rc msi_pic %p\n", __func__, lpp->msi_pic);
	return 0;
}

static int pcie_get_resources(struct platform_device *pdev)
{
	int err;
	struct ltq_pcie_port *lpp;

	lpp = platform_get_drvdata(pdev);

	err = ltq_pcie_clks_get(lpp);
	if (err) {
		dev_err(&pdev->dev, "failed to get clks: %d\n", err);
		return err;
	}

	err = ltq_pcie_resets_get(lpp);
	if (err) {
		dev_err(&pdev->dev, "failed to get resets: %d\n", err);
		return err;
	}

	return pcie_parse_dt(lpp);
}

static const struct ltq_pcie_soc_data xrx200_pcie_data = {
	.num_ports = 1,
	.has_phy_clk = true,
	.has_ahb_clk = true,
	.has_bus_clk = true,
	.has_pdi_clk = true,
	.has_msi_clk = true,
	.has_core_rst = true,
	.has_clk_out = false,
	.has_gen2 = false,
	.has_ahb2m_endian = true,
	.has_ahb4s_endian = false,
	.has_ahb3m_endian = false,
	.ahb2m_endian_ctrl_shift = 1,
	.ahb4s_endian_ctrl_shift = 0,
	.ahb3m_endian_ctrl_shift = 0,
	.per_rc_max_msi_irqs = 4,
};

static const struct ltq_pcie_soc_data xrx300_pcie_data = {
	.num_ports = 2,
	.has_phy_clk = true,
	.has_ahb_clk = false,
	.has_bus_clk = false,
	.has_pdi_clk = true,
	.has_msi_clk = true,
	.has_core_rst = true,
	.has_clk_out = false,
	.has_gen2 = false,
	.has_ahb2m_endian = false,
	.has_ahb4s_endian = true,
	.has_ahb3m_endian = true,
	.ahb2m_endian_ctrl_shift = 0,
	.ahb4s_endian_ctrl_shift = 0,
	.ahb3m_endian_ctrl_shift = 1,
	.per_rc_max_msi_irqs = 4,
};

static const struct ltq_pcie_soc_data xrx330_pcie_data = {
	.num_ports = 3,
	.has_phy_clk = true,
	.has_ahb_clk = false,
	.has_bus_clk = false,
	.has_pdi_clk = true,
	.has_msi_clk = true,
	.has_core_rst = true,
	.has_clk_out = false,
	.has_gen2 = false,
	.has_ahb2m_endian = false,
	.has_ahb4s_endian = true,
	.has_ahb3m_endian = true,
	.ahb2m_endian_ctrl_shift = 0,
	.ahb4s_endian_ctrl_shift = 0,
	.ahb3m_endian_ctrl_shift = 1,
	.per_rc_max_msi_irqs = 4,
};

static const struct ltq_pcie_soc_data xrx500_pcie_data = {
	.num_ports = 3,
	.has_phy_clk = false,
	.has_ahb_clk = false,
	.has_bus_clk = false,
	.has_pdi_clk = false,
	.has_msi_clk = false,
	.has_core_rst = false,
	.has_clk_out = true,
	.has_gen2 = true,
	.has_ahb2m_endian = false,
	.has_ahb4s_endian = false,
	.has_ahb3m_endian = false,
	.ahb2m_endian_ctrl_shift = 0,
	.ahb4s_endian_ctrl_shift = 0,
	.ahb3m_endian_ctrl_shift = 0,
	.per_rc_max_msi_irqs = 8,
};

static const struct of_device_id of_ltq_pcie_match[] = {
	{ .compatible = "lantiq,pcie-xrx200", .data = &xrx200_pcie_data },
	{ .compatible = "lantiq,pcie-xrx300", .data = &xrx300_pcie_data },
	{ .compatible = "lantiq,pcie-xrx330", .data = &xrx330_pcie_data },
	{ .compatible = "lantiq,pcie-xrx500", .data = &xrx500_pcie_data },

	{}
};
MODULE_DEVICE_TABLE(of, of_ltq_pcie_match);

static int ltq_pcie_probe(struct platform_device *pdev)
{
	int id;
	int ret;
	const struct of_device_id *match;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct ltq_pcie_port *lpp;

	ltq_pci_map_irq = ltq_pcie_bios_map_irq;
	ltq_pci_plat_arch_init = ltq_pcie_bios_plat_dev_init;
	ltq_pci_plat_dev_init = NULL;

	match = of_match_device(of_ltq_pcie_match, &pdev->dev);
	if (!match)
		return -ENODEV;

	lpp = devm_kzalloc(dev, sizeof(*lpp), GFP_KERNEL);
	if (!lpp)
		return -ENOMEM;

	/* SoC specific data */
	lpp->soc_data = match->data;

	id = of_alias_get_id(np, "pcie");
	if (id < 0) {
		dev_err(&pdev->dev, "failed to get alias id, errno %d\n", id);
		ret = id;
		goto err;
	}

	if (id > (lpp->soc_data->num_ports - 1)) {
		dev_err(&pdev->dev, "Port out of range %d\n", id);
		ret = -EINVAL;
		goto err;
	}

	lpp->id = id;

	lpp->dev = dev;

	platform_set_drvdata(pdev, lpp);

	pcie_get_resources(pdev);

	lpp->pcic.pci_ops = &pcie_ops;
	lpp->pcic.io_resource = &lpp->io;
	lpp->pcic.mem_resource = &lpp->mem;
#ifdef CONFIG_PCI_MSI
	lpp->pcic.msi_chip = &ltq_msi_chip;
#endif
	/* Load PCI I/O and memory resources from DT */
	pci_load_of_ranges(&lpp->pcic, np);
	lpp->mem_phys_base = lpp->mem.start;
	lpp->mem_size = resource_size(&lpp->mem);
	lpp->io_phys_base = lpp->io.start;
	lpp->io_size = resource_size(&lpp->io);

	ret = pcie_rc_initialize(lpp);
	if (ret == 0) {
		spin_lock_init(&lpp->lock);
		lpp->io_base = lpp->pcic.io_map_base;
		register_pci_controller(&lpp->pcic);

	#ifdef LTQ_PCIE_BASIC_ERROR_INT
		pcie_rc_core_int_init(lpp);
	#endif /* LTQ_PCIE_BASIC_ERROR_INT */
	} else
		goto err;

	pr_info("Lantiq PCIe Root Complex Port %d Init Done\n", lpp->id);
	return 0;
err:
	devm_kfree(dev, lpp);
	return ret;
}

static void ltq_pcie_shutdown(struct platform_device *pdev)
{
}

static struct platform_driver ltq_pcie_driver = {
	.probe		= ltq_pcie_probe,
	.driver = {
		.name	= "ltq-pcie",
		.owner	= THIS_MODULE,
		.of_match_table = of_ltq_pcie_match,
	},
	.shutdown	= ltq_pcie_shutdown,
};

static int __init ltq_pcie_bios_init(void)
{
	int ret = platform_driver_register(&ltq_pcie_driver);
	if (ret)
		pr_info("pcie: Error registering pcie platform driver!\n");
	return ret;
}

arch_initcall(ltq_pcie_bios_init);

MODULE_LICENSE("GPL V2");
MODULE_AUTHOR("Lei Chuanhua <Chuanhua.Lei@lantiq.com>");
MODULE_SUPPORTED_DEVICE("Lantiq builtin PCIe Controller");
MODULE_DESCRIPTION("Lantiq builtin PCIe Controller Driver");
