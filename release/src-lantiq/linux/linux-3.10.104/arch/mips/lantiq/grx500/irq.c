/*
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000, 2001, 2004 MIPS Technologies, Inc.
 * Copyright (C) 2001 Ralf Baechle
 * Copyright (C) 2014 ~ 2015 Chuanhua.lei@lantiq.com
 * Copyright (C) 2016 Intel Corporation
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * Routines for generic manipulation of the interrupts found on the MIPS
 * Malta board.
 * The interrupt controller is located in the South Bridge a PIIX4 device
 * with two internal 82C95 interrupt controllers.
 */
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel_stat.h>
#include <linux/kernel.h>
#include <linux/random.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <asm/traps.h>
#include <asm/irq_cpu.h>
#include <asm/irq_regs.h>
#include <asm/mach-lantiq/grx500/irq.h>
#include <asm/mach-lantiq/grx500/lantiq_soc.h>
#include <asm/gic.h>
#include <asm/gcmpregs.h>
#include <asm/setup.h>

enum {
	NO_OS	= 0,
	LINUX_OS,
	VOICE_FW,
	MPE_FW,
};

static struct irq_chip *irq_gic;
int gcmp_present = -1;
unsigned long _gcmp_base;
#ifdef CONFIG_MIPS_MT_SMP
static int gic_resched_int_base = 56;
static int gic_call_int_base = 60;
#define GIC_RESCHED_INT(cpu)	(gic_resched_int_base + (cpu))
#define GIC_CALL_INT(cpu)	(gic_call_int_base + (cpu))
#endif /* CONFIG_MIPS_MT_SMP */

/*
 * This GIC specific tabular array defines the association between External
 * Interrupts and CPUs/Core Interrupts. The nature of the External
 * Interrupts is also defined here - polarity/trigger.
 */

#define GIC_CPU_NMI GIC_MAP_TO_NMI_MSK
#define GIC_CPU_YQ  GIC_MAP_TO_YQ_MSK
#define GIC_YQ_PIN(X) (GIC_CPU_YQ + (X))

#define X GIC_UNUSED

static struct gic_intr_map gic_intr_map[GIC_NUM_INTRS + GIC_NUM_LOCAL_INTRS] = {
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },/* 0 MPS */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 09 */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },/* PCIe1 legacy*/
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* DMA FCC */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 }, /* SSC0 */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 19 */
	{ X, X,	   X,		X,	GIC_FLAG_VMB_IPI}, /* VMB IPI to FW */
	{ X, X,	   X,		X,	GIC_FLAG_VMB_IPI}, /* VMB IPI to FW */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* I2C FIFO eint */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 }, /* PCIe2 msi */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },/* 29 */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 32 I2C b */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 33 I2C lb */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 34 I2C s */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 35 I2C ls */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 36 I2C p */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },/* PCIe 1 msi*/
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },/* 39 */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 49 pcie1 ir */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI}, /* IPI */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI},
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI},
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI}, /* 59 */
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI},
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI},
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI},
	{ 0, GIC_CPU_INT0, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* DMA */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 69 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* TSO */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* ASC1 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 79 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ X, X,	   X,		X,	GIC_FLAG_VMB_IPI}, /* VMB IPI to FW */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI}, /* VMB */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI}, /* VMB */
#ifdef CONFIG_LTQ_PPA_MPE_IP97
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 89 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* Ring 0 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* Ring 1 */
	{ X, X,	   X,		X,		0 }, /* MPE FW */
	{ X, X,	   X,		X,		0 }, /* MPE FW */
#else
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 89 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* EIP97 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
#endif /* CONFIG_LTQ_PPA_MPE_IP97 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ X, X,	   X,		X,		0 }, /* FSC */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, 0}, /* SSC1 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, 0}, /* 99 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0}, /* ASC0 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0}, /* 109 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI}, /* VMB */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0}, /* MDIO */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0}, /* EIP123 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, GIC_FLAG_MUX},/*GPTC0*/
	{ X, X,	   X,		X,		0 }, /* 119 */
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_EDGE, GIC_FLAG_IPI},/* MPE DC*/
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT1, GIC_POL_POS, GIC_TRIG_LEVEL, 0},
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_EDGE, 0 }, /* PCIe0 MSI */
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_EDGE, 0 }, /* 129 */
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* pcie 0 legacy */
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 139 */
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* MPS */
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 149 */
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ X, X,	   X,		X,		0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* pcie 0 ir */
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* pcie 2 legacy */
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT2, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 159 */
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0}, /* GPTC1 */
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_YQ_PIN(8), GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0}, /* GPTC2 */
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0}, /* 169 */
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_EDGE, 0},
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* GPIO */
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* CBM */
	{ 1, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 2, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 3, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 1, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 179 */
	{ 2, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 3, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* TOE */
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },/* 189 */
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT3, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_YQ_PIN(0), GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_YQ_PIN(1), GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_YQ_PIN(2), GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_YQ_PIN(3), GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_YQ_PIN(4), GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_YQ_PIN(5), GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_YQ_PIN(6), GIC_POL_POS, GIC_TRIG_EDGE, 0 },
	{ 0, GIC_YQ_PIN(7), GIC_POL_POS, GIC_TRIG_EDGE, 0 }, /* 199 */
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* MCPY */
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },/* 209, DFEV*/
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* MPS2 */
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* 219 */
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* MPE */
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },/* 229 */
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },/* 239 */
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ 0, GIC_CPU_INT4, GIC_POL_POS, GIC_TRIG_LEVEL, 0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },/* 249 */
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },
	{ X, X,	   X,		X,		0 },/* 255 */
	{ 0, GIC_CPU_NMI, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* Local WDT */
	{ 0, GIC_CPU_INT5, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* GIC timer */
	{ 0, GIC_CPU_INT5, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* CP0 timer */
	{ 0, GIC_CPU_INT5, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* Perf */
	{ 0, GIC_CPU_INT5, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* Sw0 */
	{ 0, GIC_CPU_INT5, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* Sw1 */
	{ 0, GIC_CPU_INT5, GIC_POL_POS, GIC_TRIG_LEVEL, 0 }, /* fdc */
};
#undef X

static const char *os_type[3] = {
	"LINUX",
	"VOICEFW",
	"MPE",
};

static int ltq_vpe_os_type[NR_CPUS];

static int ltq_vpe_os_type_init(int cpu_id)
{
	const char *name;
	struct device_node *np;
	char str1[24] = {0};
	int ret;

	sprintf(str1, "%s%d", "/cpus/cpu@", cpu_id);

	np = of_find_node_by_path(str1);
	if (!np)
		return -ENODEV;

	ret = of_property_read_string(np, "default-OS", &name);
	if (ret < 0 && ret != -EINVAL) {
		pr_err("Failed to read OS name from DT\n");
		return ret;
	}

	if (!strncmp(name, os_type[0], strlen(os_type[0])))
		ltq_vpe_os_type[cpu_id] = LINUX_OS;
	else if (!strncmp(name, os_type[1], strlen(os_type[1])))
		ltq_vpe_os_type[cpu_id] = VOICE_FW;
	else if (!strncmp(name, os_type[2], strlen(os_type[2])))
		ltq_vpe_os_type[cpu_id] = MPE_FW;
	else
		ltq_vpe_os_type[cpu_id] = NO_OS;
	return 0;
}

bool ltq_vpe_run_linux_os(int cpu_id)
{
	return (ltq_vpe_os_type[cpu_id] == LINUX_OS) ? true : false;
}

#ifdef CONFIG_MIPS_MT_SMP
static irqreturn_t ipi_resched_interrupt(int irq, void *dev_id)
{
	scheduler_ipi();
	return IRQ_HANDLED;
}

static irqreturn_t ipi_call_interrupt(int irq, void *dev_id)
{
	smp_call_function_interrupt();
	return IRQ_HANDLED;
}

static struct irqaction irq_resched = {
	.handler	= ipi_resched_interrupt,
	.flags		= IRQF_DISABLED|IRQF_PERCPU,
	.name		= "ipi resched"
};

static struct irqaction irq_call = {
	.handler	= ipi_call_interrupt,
	.flags		= IRQF_DISABLED|IRQF_PERCPU,
	.name		= "ipi call"
};

static void __init fill_ipi_map1(int baseintr, int cpu)
{
	int intr = baseintr + cpu;
	gic_intr_map[intr].cpunum = cpu;
	gic_intr_map[intr].polarity = GIC_POL_POS;
	gic_intr_map[intr].trigtype = GIC_TRIG_EDGE;
	gic_intr_map[intr].flags = GIC_FLAG_IPI;
}

static void __init fill_ipi_map(void)
{
	int cpu;

	for (cpu = 0; cpu < nr_cpu_ids; cpu++) {
		if (ltq_vpe_run_linux_os(cpu)) {
			fill_ipi_map1(gic_resched_int_base, cpu);
			fill_ipi_map1(gic_call_int_base, cpu);
		}
	}
}

#endif /* CONFIG_MIPS_MT_SMP */

#ifdef CONFIG_SMP
unsigned int plat_ipi_call_int_xlate(unsigned int cpu)
{
	return GIC_CALL_INT(cpu);
}

unsigned int plat_ipi_resched_int_xlate(unsigned int cpu)
{
	return GIC_RESCHED_INT(cpu);
}
#endif /* CONFIG_SMP */

/*
 * GCMP needs to be detected before any SMP initialisation
 */
int __init gcmp_probe(unsigned long addr, unsigned long size)
{
	unsigned long confaddr = 0;

	if (gcmp_present >= 0)
		return gcmp_present;

	if (cpu_has_mips_r2 && (read_c0_config3() & MIPS_CONF3_CMGCR)) {
		/* try CMGCRBase */
		confaddr = read_c0_cmgcrbase() << 4;
		_gcmp_base = (unsigned long) ioremap_nocache(confaddr,
			GCMP_ADDRSPACE_SZ);
		gcmp_present = (GCMPGCB(GCMPB) & GCMP_GCB_GCMPB_GCMPBASE_MSK)
			== confaddr;
		if (gcmp_present) {
			/* reassign it to 'addr' */
			if (addr != confaddr)
				GCMPGCB(GCMPB) = (GCMPGCB(GCMPB)
				& ~GCMP_GCB_GCMPB_GCMPBASE_MSK) | addr;
			_gcmp_base = (unsigned long) ioremap_nocache(addr,
				GCMP_ADDRSPACE_SZ);
			gcmp_present = (GCMPGCB(GCMPB)
				& GCMP_GCB_GCMPB_GCMPBASE_MSK) == confaddr;
			confaddr = addr;
			if (!gcmp_present) {
				/* reassignment failed, try CMGCRBase again */
				confaddr = read_c0_cmgcrbase() << 4;
				_gcmp_base = (unsigned long)ioremap_nocache
					(confaddr, GCMP_ADDRSPACE_SZ);
				gcmp_present = (GCMPGCB(GCMPB) &
					GCMP_GCB_GCMPB_GCMPBASE_MSK)
					== confaddr;
			}
		}
	}

	if (gcmp_present <= 0) {
		/* try addr */
		_gcmp_base = (unsigned long) ioremap_nocache(addr,
		GCMP_ADDRSPACE_SZ);
		gcmp_present = (GCMPGCB(GCMPB)
			& GCMP_GCB_GCMPB_GCMPBASE_MSK) == addr;
		confaddr = addr;
	}

	if (gcmp_present <= 0) {
		/* try GCMP_BASE_ADDR */
		_gcmp_base = (unsigned long) ioremap_nocache(GCMP_BASE_ADDR,
		GCMP_ADDRSPACE_SZ);
		gcmp_present = (GCMPGCB(GCMPB) & GCMP_GCB_GCMPB_GCMPBASE_MSK)
			== GCMP_BASE_ADDR;
		confaddr = GCMP_BASE_ADDR;
	}

	if (gcmp_present) {
		pr_info("GCMP present\n");
		if (GCMPGCB(GCMPREV) >= 6)
			cpu_data[0].options |= MIPS_CPU_CM2;
		if (cpu_has_cm2 && (size > 0x8000)) {
			GCMPGCB(GCML2S) = (confaddr + 0x8000) | 1;
			cpu_data[0].options |= MIPS_CPU_CM2_L2SYNC;
			pr_info("L2-only SYNC available\n");
		}
	}

	return gcmp_present;
}

unsigned int __cpuinit get_c0_compare_int(void)
{
	return gic_get_c0_compare_int();
}

asmlinkage void plat_irq_dispatch(void)
{
	unsigned int pending = read_c0_cause() & read_c0_status() & ST0_IM;

	if (unlikely(!pending)) {
		spurious_interrupt();
		return;
	}
	gic_local_irq_dispatch();
	gic_shared_irq_dispatch();
}

int get_c0_perfcount_int(void)
{
	return gic_get_c0_perfcount_int();
}

void gic_enable_interrupt(int irq_vec)
{
	unsigned int i, irq_source;

	/* enable all the interrupts associated with this vector */
	for (i = 0; i < gic_shared_intr_map[irq_vec].num_shared_intr; i++) {
		irq_source = gic_shared_intr_map[irq_vec].intr_list[i];
		GIC_SET_INTR_MASK(irq_source);
	}
	/* enable all local interrupts associated with this vector */
	if (gic_shared_intr_map[irq_vec].local_intr_mask) {
		GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_OTHER_ADDR), 0);
		GICWRITE(GIC_REG(VPE_OTHER, GIC_VPE_SMASK),
			gic_shared_intr_map[irq_vec].local_intr_mask);
	}
}

void gic_disable_interrupt(int irq_vec)
{
	unsigned int i, irq_source;

	/* disable all the interrupts associated with this vector */
	for (i = 0; i < gic_shared_intr_map[irq_vec].num_shared_intr; i++) {
		irq_source = gic_shared_intr_map[irq_vec].intr_list[i];
		GIC_CLR_INTR_MASK(irq_source);
	}
	/* disable all local interrupts associated with this vector */
	if (gic_shared_intr_map[irq_vec].local_intr_mask) {
		GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_OTHER_ADDR), 0);
		GICWRITE(GIC_REG(VPE_OTHER, GIC_VPE_RMASK),
			gic_shared_intr_map[irq_vec].local_intr_mask);
	}
}

#ifndef CONFIG_SOC_GRX500_A21
#ifdef CONFIG_PCIE_LANTIQ
#define PCIE_IRNCR_OFF		0xF8
static u32 pcie_rc_app_base[3] = {0xb8800000, 0xb8300000, 0xb8d00000};

static u32 pcie_irq_idx_to_bit[4] = {BIT(13), BIT(14), BIT(15), BIT(16)};

void pcie_intx_war(unsigned int irq)
{
	if (irq >= 136 && irq <= 139) {/* RC0 */
		ltq_w32_mask(0, pcie_irq_idx_to_bit[irq - 136],
			(u32 *)(pcie_rc_app_base[0] + PCIE_IRNCR_OFF));
	} else if (irq >= 9 && irq <= 12) { /* RC1 */
		ltq_w32_mask(0, pcie_irq_idx_to_bit[irq - 9],
			(u32 *)(pcie_rc_app_base[1] + PCIE_IRNCR_OFF));
	} else if (irq >= 155 && irq <= 158) { /* RC2 */
		ltq_w32_mask(0, pcie_irq_idx_to_bit[irq - 155],
			(u32 *)(pcie_rc_app_base[2] + PCIE_IRNCR_OFF));
	}
}
#endif /* CONFIG_PCIE_LANTIQ */
#endif /* CONFIG_SOC_GRX500_A21 */

void __init gic_platform_init(int irqs, struct irq_chip *irq_controller)
{
	irq_gic = irq_controller;
}

static int gic_map(struct irq_domain *d, unsigned int irq, irq_hw_number_t hw)
{
	if (hw >= MIPS_GIC_LOCAL_IRQ_BASE) { /* Local Irq*/
		int intr = hw - MIPS_GIC_LOCAL_IRQ_BASE;
		/*
		* HACK: These are all really percpu interrupts, but the rest
		* of the MIPS kernel code does not use the percpu IRQ API for
		* the CP0 timer and performance counter interrupts.
		*/
		if ((intr != GIC_LOCAL_INT_TIMER)
			&& (intr != GIC_LOCAL_INT_PERFCTR)) {
			irq_set_chip_and_handler(hw,
					 irq_gic,
					 handle_percpu_devid_irq);
			irq_set_percpu_devid(hw);
		} else
			goto done;
	} else
done :
		irq_set_chip_and_handler(hw, irq_gic, handle_percpu_irq);
	return 0;
}

static const struct irq_domain_ops gic_irq_domain_ops = {
	.xlate = irq_domain_xlate_onecell,
	.map = gic_map,
};

static int __init of_gic_init(struct device_node *node,
				struct device_node *parent)
{
	struct irq_domain *domain;
	struct resource gic = { 0 };
	unsigned int gic_rev;
	int i;

	/* Not all VPE up, can't use OS API to handle it */
	for (i = 0; i < num_possible_cpus(); i++)
		ltq_vpe_os_type_init(i);

	if (of_address_to_resource(node, 0, &gic))
		panic("Failed to get gic memory range");
	if (!request_mem_region(gic.start, resource_size(&gic),
				gic.name))
		panic("Failed to request gic memory");

	if (gcmp_present)  {
		GCMPGCB(GICBA) = GIC_BASE_ADDR | GCMP_GCB_GICBA_EN_MSK;
		gic_present = 1;
	}
	pr_info("GIC: %spresent\n", (gic_present) ? "" : "not ");
	pr_info("EIC: %s\n",
		(current_cpu_data.options & MIPS_CPU_VEIC) ?  "on" : "off");
	pr_info("GIC map size %d\n", ARRAY_SIZE(gic_intr_map));

	if (!cpu_has_veic) {
		mips_cpu_irq_init();
		if (cpu_has_vint) {
			/* install generic handler */
			/* 2 - Normal/IPI
			 * 3 - Normal
			 * 4 - Normal
			 * 5 - Normal
			 * 6 - Normal
			 * 7 - Local
			 */
			set_vi_handler(2, gic_shared_irq_vi2_dispatch);
			set_vi_handler(3, gic_shared_irq_vi3_dispatch);
			set_vi_handler(4, gic_shared_irq_vi4_dispatch);
			set_vi_handler(5, gic_shared_irq_vi5_dispatch);
			set_vi_handler(6, gic_shared_irq_vi6_dispatch);
			set_vi_handler(7, gic_local_irq_dispatch);
			
			/* Performance Counter, vector 7
			 * VPE0 timer default HW5
			 * VPE0 performance counter default HW4
			 * GIC will re route perofrmance counter to HW5
			 * However, VPE0 initialized, PMU also initialized
			 * based on VPE0 default value0
			 */
			/* R4K timer handled by OS, vector 7 */
		}
	}

#ifdef CONFIG_MIPS_MT_SMP
	fill_ipi_map();
#endif /* CONFIG_MIPS_MT_SMP */
	gic_init(gic.start, resource_size(&gic), gic_intr_map,
		ARRAY_SIZE(gic_intr_map), MIPS_GIC_IRQ_BASE);

	GICREAD(GIC_REG(SHARED, GIC_SH_REVISIONID), gic_rev);
	pr_info("gic: revision %d.%d\n", (gic_rev >> 8) & 0xff, gic_rev & 0xff);
	domain = irq_domain_add_legacy(node,
		GIC_NUM_INTRS + GIC_NUM_LOCAL_INTRS,
		gic_irq_base, gic_irq_base, &gic_irq_domain_ops, NULL);
	if (!domain)
		panic("Failed to add irqdomain");
	irq_set_default_host(domain);

#ifdef CONFIG_MIPS_MT_SMP
	for (i = 0; i < nr_cpu_ids; i++) {
		if (ltq_vpe_run_linux_os(i)) {
			setup_irq(gic_irq_base + GIC_RESCHED_INT(i),
				&irq_resched);
			setup_irq(gic_irq_base + GIC_CALL_INT(i),
				&irq_call);
		}
	}
#endif
	change_c0_status(ST0_IM, STATUSF_IP2 | STATUSF_IP3 | STATUSF_IP4 |
		STATUSF_IP5 | STATUSF_IP6 | STATUSF_IP7);
	back_to_back_c0_hazard();
	pr_info("CPU%d: c0 status register %08x\n", smp_processor_id(),
		read_c0_status());

	pr_info("%s init done\n", __func__);
	return 0;
}

static struct of_device_id __initdata of_irq_ids[] = {
	{ .compatible = "lantiq,gic-grx500", .data = of_gic_init },
	{},
};

void __init arch_init_irq(void)
{
	of_irq_init(of_irq_ids);
}

void grx5xx_be_init(void)
{
	/* Could change CM error mask register */
}

static const char *tr[8] = {
	"mem",	"gcr",	"gic",	"mmio",
	"0x04", "0x05", "0x06", "0x07"
};

static const char *mcmd[32] = {
	[0x00] = "0x00",
	[0x01] = "Legacy Write",
	[0x02] = "Legacy Read",
	[0x03] = "0x03",
	[0x04] = "0x04",
	[0x05] = "0x05",
	[0x06] = "0x06",
	[0x07] = "0x07",
	[0x08] = "Coherent Read Own",
	[0x09] = "Coherent Read Share",
	[0x0a] = "Coherent Read Discard",
	[0x0b] = "Coherent Ready Share Always",
	[0x0c] = "Coherent Upgrade",
	[0x0d] = "Coherent Writeback",
	[0x0e] = "0x0e",
	[0x0f] = "0x0f",
	[0x10] = "Coherent Copyback",
	[0x11] = "Coherent Copyback Invalidate",
	[0x12] = "Coherent Invalidate",
	[0x13] = "Coherent Write Invalidate",
	[0x14] = "Coherent Completion Sync",
	[0x15] = "0x15",
	[0x16] = "0x16",
	[0x17] = "0x17",
	[0x18] = "0x18",
	[0x19] = "0x19",
	[0x1a] = "0x1a",
	[0x1b] = "0x1b",
	[0x1c] = "0x1c",
	[0x1d] = "0x1d",
	[0x1e] = "0x1e",
	[0x1f] = "0x1f"
};

static const char *l2_ins[32] = {
	[0x00] = "l2_nop",
	[0x01] = "l2_err_corr",
	[0x02] = "l2_tag_inv",
	[0x03] = "l2_ws_clean",
	[0x04] = "l2_rd_mdyfy_wr",
	[0x05] = "l2_ws_mru",
	[0x06] = "l2_evict_ln2",
	[0x07] = "0x07",
	[0x08] = "l2_evict",
	[0x09] = "l2_refl",
	[0x0a] = "l2_rd",
	[0x0b] = "l2_wr",
	[0x0c] = "l2_evict_mru",
	[0x0d] = "l2_sync",
	[0x0e] = "l2_efl_err",
	[0x0f] = "0x0f",
	[0x10] = "l2_indx_wb_inv",
	[0x11] = "l2_indx_ld_tag",
	[0x12] = "l2_indx_st_tag",
	[0x13] = "l2_indx_st_data",
	[0x14] = "l2_indx_st_ecc",
	[0x15] = "0x15",
	[0x16] = "0x16",
	[0x17] = "0x17",
	[0x18] = "l2_ftch_and_lck",
	[0x19] = "l2_hit_inv",
	[0x1a] = "l2_hit_wb_inv",
	[0x1b] = "l2_hit_wb",
	[0x1c] = "0x1c",
	[0x1d] = "0x1d",
	[0x1e] = "0x1e",
	[0x1f] = "0x1f"
};

static const char *array_type[4] = {
	[0] = "none",
	[1] = "Tag Ecc",
	[2] = "Data Ecc",
	[3] = "WS unco dirty parity"
};

static const char *core[8] = {
	"Invalid/OK",	"Invalid/Data",
	"Shared/OK",	"Shared/Data",
	"Modified/OK",	"Modified/Data",
	"Exclusive/OK", "Exclusive/Data"
};

static const char *causes[32] = {
	"None", "GC_WR_ERR", "GC_RD_ERR", "COH_WR_ERR",
	"COH_RD_ERR", "MMIO_WR_ERR", "MMIO_RD_ERR", "0x07",
	"0x08", "0x09", "0x0a", "0x0b",
	"0x0c", "0x0d", "0x0e", "0x0f",
	"0x10", "0x11", "0x12", "0x13",
	"0x14", "0x15", "0x16", "INTVN_WR_ERR",
	"INTVN_RD_ERR", "0x19", "0x1a", "0x1b",
	"0x1c", "0x1d", "0x1e", "0x1f"
};

int grx5xx_be_handler(struct pt_regs *regs, int is_fixup)
{
	/* This duplicates the handling in do_be which seems wrong */
	int retval = is_fixup ? MIPS_BE_FIXUP : MIPS_BE_FATAL;

	if (gcmp_present) {
		unsigned long cm_error = GCMPGCB(GCMEC);
		unsigned long cm_addr = GCMPGCB(GCMEA);
		unsigned long cm_other = GCMPGCB(GCMEO);
		unsigned long cause, ocause;
		char buf[256];

		cause = (cm_error & GCMP_GCB_GMEC_ERROR_TYPE_MSK);
		if (cause != 0) {
			cause >>= GCMP_GCB_GMEC_ERROR_TYPE_SHF;
			if (cause < 16) {
				unsigned long cca_bits = (cm_error >> 15) & 7;
				unsigned long tr_bits = (cm_error >> 12) & 7;
				unsigned long mcmd_bits =
					(cm_error >> 7) & 0x1f;
				unsigned long stag_bits = (cm_error >> 3) & 15;
				unsigned long sport_bits = (cm_error >> 0) & 7;

				snprintf(buf, sizeof(buf),
					 "CCA=%lu TR=%s MCmd=%s STag=%lu "
					 "SPort=%lu\n",
					 cca_bits, tr[tr_bits], mcmd[mcmd_bits],
					 stag_bits, sport_bits);
			} else  if (cause >= 16 && cause <= 23) {
				/* glob state & sresp together */
				unsigned long c3_bits = (cm_error >> 18) & 7;
				unsigned long c2_bits = (cm_error >> 15) & 7;
				unsigned long c1_bits = (cm_error >> 12) & 7;
				unsigned long c0_bits = (cm_error >> 9) & 7;
				unsigned long sc_bit = (cm_error >> 8) & 1;
				unsigned long mcmd_bits =
					(cm_error >> 3) & 0x1f;
				unsigned long sport_bits = (cm_error >> 0) & 7;
				snprintf(buf, sizeof(buf),
					 "C3=%s C2=%s C1=%s C0=%s SC=%s "
					 "MCmd=%s SPort=%lu\n",
					 core[c3_bits], core[c2_bits],
					 core[c1_bits], core[c0_bits],
					 sc_bit ? "True" : "False",
					 mcmd[mcmd_bits], sport_bits);
			} else { /* 24 ~ 26 */
				unsigned long multi_bit = (cm_error >> 23) & 1;
				unsigned long ins_bits =
					(cm_error >> 18) & 0x1f;
				unsigned long type_bits = (cm_error >> 16) & 3;
				unsigned long way_bits = (cm_error >> 9) & 7;
				unsigned long word_bits =
					(cm_error >> 12) & 0xf;
				unsigned long mway_bit = (cm_error >> 8) & 1;
				unsigned long syn_bits = (cm_error >> 0) & 0xff;
				snprintf(buf, sizeof(buf),
					 "multi unco=%s l2 ins=%s array type=%s"
					 "word_bits=%lu way_bits=%lu"
					 "multi way=%s syndrome=%lu\n",
					 multi_bit ? "True" : "False",
					 l2_ins[ins_bits],
					 array_type[type_bits], word_bits,
					 way_bits, mway_bit ? "True" : "False",
					 syn_bits);
			}
			ocause = (cm_other & GCMP_GCB_GMEO_ERROR_2ND_MSK) >>
				 GCMP_GCB_GMEO_ERROR_2ND_SHF;

			pr_err("CM_ERROR=%08lx %s <%s>\n", cm_error,
				causes[cause], buf);
			pr_err("CM_ADDR =%08lx\n", cm_addr);
			pr_err("CM_OTHER=%08lx %s\n", cm_other, causes[ocause]);

			/* reprime cause register */
			GCMPGCB(GCMEC) = 0;
		}
	}
	return retval;
}


