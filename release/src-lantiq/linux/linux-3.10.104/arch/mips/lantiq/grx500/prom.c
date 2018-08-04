/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 * Copyright (C) 2010 John Crispin <blogic@openwrt.org>
 * Copyright (C) 2014~2015 Lei Chuanhua <Chuanhua.lei@lantiq.com>
 */

#include <linux/export.h>
#include <linux/clk.h>
#include <linux/bootmem.h>
#include <linux/of_platform.h>
#include <linux/of_fdt.h>

#include <asm/traps.h>
#include <asm/bootinfo.h>
#include <linux/time.h>
#include <asm/gcmpregs.h>
#include <asm/mips-cpc.h>
#include <asm/smp-ops.h>
#include <lantiq.h>
#include <asm/dma-coherence.h>
#include <asm/gic.h>
#include <asm/gcmpregs.h>
#include <asm/mach-lantiq/grx500/lantiq_soc.h>

#include "prom.h"
#include "clk.h"


#define SOC_GRX500		"GRX500"
#define SOC_GRX350		"GRX350"

#ifdef CONFIG_USE_EMULATOR
#define SOC_ID_PD	0
#endif

#define COMP_GRX500		"lantiq,grx500"

#define PART_SHIFT		12
#define PART_MASK		0x0FFFFFFF
#define REV_SHIFT		28
#define REV_MASK		0x70000000

/* access to the ebu needs to be locked between different drivers */
DEFINE_SPINLOCK(ebu_lock);
EXPORT_SYMBOL_GPL(ebu_lock);

extern struct boot_param_header __image_dtb;

/*
 * this struct is filled by the soc specific detection code and holds
 * information about the specific soc type, revision and name
 */
static struct ltq_soc_info soc_info;
#ifdef CONFIG_LTQ_CBM
void *ltq_cbm_std_buf_base;
void *ltq_cbm_jbo_buf_base;
#endif
unsigned int ltq_get_cpu_id(void)
{
	return soc_info.partnum;
}
EXPORT_SYMBOL(ltq_get_cpu_id);

unsigned int ltq_get_soc_type(void)
{
	return soc_info.type;
}
EXPORT_SYMBOL(ltq_get_soc_type);

unsigned int ltq_get_soc_rev(void)
{
	return soc_info.rev;
}
EXPORT_SYMBOL(ltq_get_soc_rev);

const char *get_system_type(void)
{
	return soc_info.sys_type;
}

void prom_free_prom_memory(void)
{
}

static void __init prom_init_cmdline(void)
{
	int argc = fw_arg0;
	char **argv = (char **) KSEG1ADDR(fw_arg1);
	int i;

	arcs_cmdline[0] = '\0';

	for (i = 0; i < argc; i++) {
		char *p = (char *) KSEG1ADDR(argv[i]);

#ifdef CONFIG_EVA
		if (RPHYSADDR(p) && *p) {
			strlcat(arcs_cmdline, p, sizeof(arcs_cmdline));
			strlcat(arcs_cmdline, " ", sizeof(arcs_cmdline));
		}
#else
		if (CPHYSADDR(p) && *p) {
			strlcat(arcs_cmdline, p, sizeof(arcs_cmdline));
			strlcat(arcs_cmdline, " ", sizeof(arcs_cmdline));
		}
#endif
	}
}

#ifdef CONFIG_EVA
extern int gcmp_present;
void BEV_overlay_segment(void);
#endif

/* Return the number of IOCU's present */
int __init gcmp_niocu(void)
{
	return gcmp_present ?
		(GCMPGCB(GC) & GCMP_GCB_GC_NUMIOCU_MSK)
		>> GCMP_GCB_GC_NUMIOCU_SHF : 0;
}

/* Set GCMP region attributes */
void __init gcmp_setregion(int region, unsigned long base,
			   unsigned long mask, int type)
{
	GCMPGCBn(CMxBASE, region) = base;
	GCMPGCBn(CMxMASK, region) = mask | type;
}

static int __init plat_enable_iocoherency(void)
{
	int supported = 0;

	if (gcmp_niocu() != 0) {
		/* Nothing special needs to be done to enable coherency */
		pr_info("CMP IOCU detected\n");
		/* Second IOCU for MPE or other master access register */
		gcmp_setregion(0, 0xa0000000, 0xf8000000,
			GCMP_GCB_GCMPB_CMDEFTGT_IOCU2);
		supported = 1;
	}

	/* hw_coherentio = supported; */

	return supported;
}

static void __init plat_setup_iocoherency(void)
{
#if defined(CONFIG_DMA_NONCOHERENT)
	/*
	 * Kernel has been configured with software coherency
	 * but we might choose to turn it off and use hardware
	 * coherency instead.
	 */
	if (plat_enable_iocoherency()) {
		if (coherentio == 0)
			pr_info("Hardware DMA cache coherency disabled\n");
		else
			pr_info("Hardware DMA cache coherency enabled\n");
	} else {
		if (coherentio == 1)
			pr_info("Hardware DMA cache coherency unsupported, but enabled from command line!\n");
		else
			pr_info("Software DMA cache coherency enabled\n");
	}
#else
	if (!plat_enable_iocoherency())
		panic("Hardware DMA cache coherency not supported!");
#endif /* CONFIG_DMA_NONCOHERENT || CONFIG_DMA_MAYBE_COHERENT */
}

void __init plat_mem_setup(void)
{
	/* EVA has already been configured in mach-lantiq/kernel-entry-init.h */
if (config_enabled(CONFIG_EVA))
#ifdef CONFIG_EVA_2GB
	pr_info("Enhanced Virtual Addressing (EVA 2GB) activated\n");
#elif defined(CONFIG_EVA_1GB)
	pr_info("Enhanced Virtual Addressing (EVA 1GB) activated\n");
#else
	pr_info("Enhanced Virtual Addressing (EVA Legacy 512MB) activated\n");
#endif /* CONFIG_EVA_2GB */
else
	pr_info("EVA disabled, Lantiq legacy activiated\n");

#ifdef CONFIG_MIPS_CMP
	if (gcmp_present)
		BEV_overlay_segment();
#endif

	pr_info("segctr0 %x segctr1 %x segctr2 %x\n",
		read_c0_segctl0(), read_c0_segctl1(), read_c0_segctl2());
	ioport_resource.start = IOPORT_RESOURCE_START;
	ioport_resource.end = IOPORT_RESOURCE_END;
	iomem_resource.start = IOMEM_RESOURCE_START;
	iomem_resource.end = IOMEM_RESOURCE_END;

	set_io_port_base((unsigned long) KSEG1);

	/*
	 * Load the builtin devicetree. This causes the chosen node to be
	 * parsed resulting in our memory appearing
	 */
	__dt_setup_arch(&__image_dtb);

	plat_setup_iocoherency();

	board_be_init = grx5xx_be_init;
	board_be_handler = grx5xx_be_handler;
}

void __init device_tree_init(void)
{
	unsigned long base, size;

	if (!initial_boot_params)
		return;

	base = virt_to_phys((void *)initial_boot_params);
	size = be32_to_cpu(initial_boot_params->totalsize);

	/* Before we do anything, lets reserve the dt blob */
	reserve_bootmem(base, size, BOOTMEM_DEFAULT);

	unflatten_device_tree();
#ifdef CONFIG_LTQ_CBM
	ltq_cbm_std_buf_base = alloc_bootmem_low(CONFIG_CBM_STD_BUF_POOL_SIZE);
	if (!ltq_cbm_std_buf_base)
		panic("no memory for CBM standard buffers !!");
	else
		pr_info("reserving %d bytes @ %x for CBM \n", CONFIG_CBM_STD_BUF_POOL_SIZE, (unsigned int)ltq_cbm_std_buf_base);
	ltq_cbm_jbo_buf_base = alloc_bootmem_low(CONFIG_CBM_JBO_BUF_SIZE);
	if (!ltq_cbm_jbo_buf_base)
		panic("no memory for CBM Jumbo buffers !!");
	else
		pr_info("reserving %d bytes @ %x for CBM \n", CONFIG_CBM_JBO_BUF_SIZE, (unsigned int)ltq_cbm_jbo_buf_base);
#endif
}

phys_t mips_cpc_default_phys_base(void)
{
	return CPC_BASE_ADDR;
}

void __init prom_init(void)
{
	/* call the soc specific detetcion code and get it to fill soc_info */
	ltq_soc_detect(&soc_info);
	snprintf(soc_info.sys_type, LTQ_SYS_TYPE_LEN - 1, "%s rev %s",
		soc_info.name, soc_info.rev_type);
	soc_info.sys_type[LTQ_SYS_TYPE_LEN - 1] = '\0';
	pr_info("SoC: %s\n", soc_info.sys_type);

	prom_init_cmdline();

	/* Early detection of CMP support */
	if (gcmp_probe(GCMP_BASE_ADDR, GCMP_ADDRSPACE_SZ)) {

		mips_cpc_probe();

		if (!register_cmp_smp_ops())
			return;

		if (!register_cps_smp_ops())
			return;
	}
#if defined(CONFIG_MIPS_MT_SMP)
	if (!register_vsmp_smp_ops())
		return;
#endif
#ifdef CONFIG_MIPS_MT_SMTC
	register_smp_ops(&msmtc_smp_ops);
#endif
}

void __init ltq_soc_detect(struct ltq_soc_info *i)
{
#ifdef CONFIG_USE_EMULATOR
	i->partnum = SOC_ID_PD;
	i->rev = SOC_ID_PD;
#else
	i->partnum = (ltq_r32(LTQ_MPS_CHIPID) & PART_MASK) >> PART_SHIFT;
	i->rev = (ltq_r32(LTQ_MPS_CHIPID) & REV_MASK) >> REV_SHIFT;
#endif
	sprintf(i->rev_type, "1.%d", i->rev);
	switch (i->partnum) {
	case SOC_ID_GRX562:
	case SOC_ID_GRX582:
	case SOC_ID_GRX583:
	case SOC_ID_GRX580:
	case SOC_ID_GRX584:
	case SOC_ID_GRX585:
		i->name = SOC_GRX500;
		i->type = SOC_TYPE_GRX500;
		i->compatible = COMP_GRX500;
		break;
	case SOC_ID_GRX3506:
	case SOC_ID_GRX3508:
		i->name = SOC_GRX350;
		i->type = SOC_TYPE_GRX500;
		i->compatible = COMP_GRX500;
		break;
#ifdef CONFIG_USE_EMULATOR
	case SOC_ID_PD:
		i->name = SOC_GRX500;
		i->type = SOC_TYPE_GRX500;
		i->compatible = COMP_GRX500;
		break;
#endif
	default:
		unreachable();
		break;
	}
}

int __init plat_of_setup(void)
{
	static struct of_device_id of_ids[3];

	if (!of_have_populated_dt())
		panic("device tree not present");

	strncpy(of_ids[0].compatible, soc_info.compatible,
		sizeof(of_ids[0].compatible));
	strncpy(of_ids[1].compatible, "simple-bus",
		sizeof(of_ids[1].compatible));
	if (of_platform_populate(NULL, of_ids, NULL, NULL))
		panic("failed to poplate DT");
	 /* make sure ithat the reset controller is setup early */
	ltq_rst_init();
	return 0;
}

arch_initcall(plat_of_setup);
