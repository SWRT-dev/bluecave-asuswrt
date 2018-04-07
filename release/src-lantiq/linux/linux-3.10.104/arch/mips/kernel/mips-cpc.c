/*
 * Copyright (C) 2013 Imagination Technologies
 * Author: Paul Burton <paul.burton@imgtec.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#include <linux/bitmap.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/threads.h>

#include <asm/gcmpregs.h>
#include <asm/gic.h>
#include <asm/mips-cpc.h>

extern int gcmp_present;
void __iomem *mips_cpc_base;

phys_t __weak mips_cpc_phys_base(void)
{
	u32 cpc_base;

	if (!gcmp_present)
		return 0;

	if (!(GCMPGCB(CPCST) & GCMP_GCB_CPCST_EX_MSK))
		return 0;

	/* If the CPC is already enabled, leave it so */
	cpc_base = GCMPGCB(CPCBA);
	if (cpc_base & GCMP_GCB_CPCBA_EN_MSK)
		return cpc_base & GCMP_GCB_CPCBA_BASE_MSK;

	/* Otherwise, give it the default address & enable it */
	cpc_base = mips_cpc_default_phys_base();
	GCMPGCB(CPCBA) = cpc_base | GCMP_GCB_CPCBA_EN_MSK;
	return cpc_base;
}

int mips_cpc_probe(void)
{
	phys_t addr;

	addr = mips_cpc_phys_base();
	if (!addr)
		return -ENODEV;

	mips_cpc_base = ioremap_nocache(addr, 0x8000);
	if (!mips_cpc_base)
		return -ENXIO;

	return 0;
}
