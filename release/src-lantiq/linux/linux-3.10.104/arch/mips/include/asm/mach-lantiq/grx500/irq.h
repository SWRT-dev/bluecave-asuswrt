/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2014 Lei Chuanhua <Chuanhua.lei@lantiq.com>
 */

#ifndef __LANTIQ_IRQ_H
#define __LANTIQ_IRQ_H

#define MIPS_CPU_IRQ_BASE	0
#define MIPS_GIC_IRQ_BASE	(MIPS_CPU_IRQ_BASE + 8)

#define GPIO0_IRQ_BASE		271
#define GPIO1_IRQ_BASE		303
#define DMA0_IRQ_BASE		335
#define DMA1TX_IRQ_BASE		351
#define DMA1RX_IRQ_BASE		367
#define DMA2TX_IRQ_BASE		399
#define DMA2RX_IRQ_BASE		415
#define DMA3_IRQ_BASE		447
#define DMA4_IRQ_BASE		511

#define NR_IRQS 527

#include_next <irq.h>

#endif
