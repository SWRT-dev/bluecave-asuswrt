/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 * Copyright (C) 2010 John Crispin <blogic@openwrt.org>
 */

#include <linux/export.h>
#include <linux/clk.h>
#include <linux/bootmem.h>
#include <linux/of_platform.h>
#include <linux/of_fdt.h>

#include <asm/bootinfo.h>
#include <asm/time.h>

#include <lantiq.h>
#include <lantiq_soc.h>

#include "prom.h"
#include "clk.h"


#define SOC_GRX500      "GRX500"
#define SOC_GRX350      "GRX350"
#define COMP_GRX500     "lantiq,grx500"

#ifdef CONFIG_USE_EMULATOR
#define SOC_ID_PD 0
#endif

#define PART_SHIFT      12
#define PART_MASK       0x0FFFFFFF
#define REV_SHIFT       28
#define REV_MASK        0x70000000

/* access to the ebu needs to be locked between different drivers */
DEFINE_SPINLOCK(ebu_lock);
EXPORT_SYMBOL_GPL(ebu_lock);

/*
 * this struct is filled by the soc specific detection code and holds
 * information about the specific soc type, revision and name
 */
static struct ltq_soc_info soc_info;

const char *get_system_type(void)
{
	return soc_info.sys_type;
}

void prom_free_prom_memory(void)
{
}

#ifndef CONFIG_USE_EMULATOR
static void __init prom_init_cmdline(void)
{
#if 0
	int argc = fw_arg0;
	char **argv = (char **) KSEG1ADDR(fw_arg1);
	int i;

	arcs_cmdline[0] = '\0';

	for (i = 0; i < argc; i++) {
		char *p = (char *) KSEG1ADDR(argv[i]);

		if (CPHYSADDR(p) && *p) {
			strlcat(arcs_cmdline, p, sizeof(arcs_cmdline));
			strlcat(arcs_cmdline, " ", sizeof(arcs_cmdline));
		}
	}
#else
	arcs_cmdline[0] = '\0';
	strlcat(arcs_cmdline,"console=ttyLTQ0,115200n8r root=/dev/ram rw loglevel=8 ",sizeof(arcs_cmdline));
	#if defined(CONFIG_TOS_SIZE_16M)
		strlcat(arcs_cmdline,"mem=16M@128M ",sizeof(arcs_cmdline));
	#elif defined(CONFIG_TOS_SIZE_64M)
		strlcat(arcs_cmdline,"mem=64M@128M ",sizeof(arcs_cmdline));
	#elif defined(CONFIG_TOS_SIZE_128M)
		strlcat(arcs_cmdline,"mem=128M@128M ",sizeof(arcs_cmdline));
	#else/*default is 32M*/
		strlcat(arcs_cmdline,"mem=32M@128M ",sizeof(arcs_cmdline));
	#endif
#endif
}
#endif

void __init plat_mem_setup(void)
{
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
}

void __init prom_init(void)
{
	/* call the soc specific detetcion code and get it to fill soc_info */
	ltq_soc_detect(&soc_info);
	snprintf(soc_info.sys_type, LTQ_SYS_TYPE_LEN - 1, "%s rev %s",
		soc_info.name, soc_info.rev_type);
	soc_info.sys_type[LTQ_SYS_TYPE_LEN - 1] = '\0';
	pr_info("SoC: %s\n", soc_info.sys_type);
#ifndef CONFIG_USE_EMULATOR
	prom_init_cmdline();
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
	return of_platform_populate(NULL, of_ids, NULL, NULL);
}

arch_initcall(plat_of_setup);
