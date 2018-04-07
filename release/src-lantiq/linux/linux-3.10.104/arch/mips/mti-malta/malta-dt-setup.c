/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2012 MIPS Technologies, Inc.  All rights reserved.
 */
#include <linux/init.h>
#include <linux/of_platform.h>
#include <linux/of_fdt.h>
#include <linux/bootmem.h>

#include <asm/mips-boards/generic.h>

void __init device_tree_init(void)
{
#if 0
	unsigned long base, size;

	printk ("[%s]:[%d]  \n", __FUNCTION__, __LINE__);
	if (!initial_boot_params)
		return;

	printk ("[%s]:[%d]  \n", __FUNCTION__, __LINE__);
	base = virt_to_phys((void *)initial_boot_params);
	size = be32_to_cpu(initial_boot_params->totalsize);

	printk ("[%s]:[%d] base=%lx size=%ld \n", __FUNCTION__, __LINE__, base, size);

	/* Before we do anything, lets reserve the dt blob */
	reserve_bootmem(base, size, BOOTMEM_DEFAULT);

	printk ("[%s]:[%d]  \n", __FUNCTION__, __LINE__);
	unflatten_device_tree();
	printk ("[%s]:[%d]  \n", __FUNCTION__, __LINE__);
#else
	// printk ("[%s]:[%d]  \n", __FUNCTION__, __LINE__);
	unflatten_device_tree();
	// printk ("[%s]:[%d]  \n", __FUNCTION__, __LINE__);
#endif
}
