/*
 *  Copyright (C) 2012 Thomas Langer <thomas.langer@lantiq.com>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
*/

#include <linux/delay.h>

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/clocksource.h>
#include <linux/device.h>

#include <asm/addrspace.h>
#include <asm/io.h>
#include <asm/time.h>

#include <lantiq.h>
#include <falcon/lantiq_soc.h>

#include <falcon/falcon_irq.h>
#include <falcon/sysctrl.h>

#include "../clk.h"

#include "gptc_reg.h"

/* mapping to linux hw-accessor routines */
#define reg_r32(reg)			__raw_readl(reg)
#define reg_w32(val, reg)		__raw_writel(val, reg)
#define reg_w32_mask(clear, set, reg)	reg_w32((reg_r32(reg) & ~(clear)) | (set), reg)

/* Voice FW uses TIMER 0A! */
#define CLOCKSOURCE_TIMER	2
#define CLOCKEVENT_TIMER	4

/** address range for gptc
    0x1E100E00--0x1E100EFF */
#define GPON_GPTC_BASE		0x1E100E00
#define GPON_GPTC_END		0x1E100EFF
#define GPON_GPTC_SIZE		0x00000100

static struct gpon_reg_gptc * gptc;

/* helper routines for GPTC timers */

static inline unsigned int lq_get_irq_nr(unsigned int timer)
{
	irq_hw_number_t irq = FALCON_IRQ_GPTC_TC1A + timer;
	return irq_create_mapping(NULL, irq);
}

static inline void lq_set_reload(unsigned int timer, unsigned int reload)
{
	gptc_w32(reload, tc[timer>>1].reload[timer&1]);
}

static inline void lq_start_timer(unsigned int timer)
{
	gptc_w32(GPTC_RUN_RL|GPTC_RUN_SEN, tc[timer>>1].run[timer&1]);
}

static inline void lq_stop_and_clear_timer(unsigned int timer)
{
	gptc_w32(GPTC_RUN_RL|GPTC_RUN_CEN, tc[timer>>1].run[timer&1]);
}

static int lq_setup_timer(unsigned int timer, unsigned int reload, int is_32_bit,
	int is_cyclic, int is_inverse, int is_dir_up, int counter, int use_irq)
{
	u32 con_reg = 0;

	pr_devel("lq_setup_timer(%d, 0x%08X, %d-bit, %s%s%s%s, %suse irq)\n",
		timer, reload, is_32_bit?32:16,
		is_cyclic?"cylic ":"one shot ",
		is_inverse?"inv ":"",
		is_dir_up?"up ":"down ",
		counter?"counter":"timer",
		use_irq?"":"don't ");

	if (is_32_bit) {
		if ((timer & 1) == 1)
			return -EINVAL;
		con_reg |= GPTC_CON_A_EXT;
	} else {
		reload &= 0xFFFF;
	}
	if (is_inverse)
		con_reg |= GPTC_CON_INV;
	if (is_dir_up)
		con_reg |= GPTC_CON_DIR;
	if (!is_cyclic)
		con_reg |= GPTC_CON_STP;

	/* always disable irq first */
	gptc_w32_mask(1<<timer, 0, irnen);
	gptc_w32(con_reg, tc[timer>>1].con[timer&1]);
	lq_set_reload(timer, reload);
	if (use_irq)
		gptc_w32_mask(0, 1<<timer, irnen);

	return 0;
}

/* not 0 means "clock disable" feature is activated in HW */
static int falcon_timer_clkdis = 0;

/*
 * ClockSource related definitions and routines
 */
struct falcon_clocksource {
	struct clocksource cs;
	u32 __iomem *cntreg;
};

static cycle_t falcon_cs_read(struct clocksource *cs)
{
	struct falcon_clocksource *falcon_cs =
		container_of(cs, struct falcon_clocksource, cs);
	return __raw_readl(falcon_cs->cntreg);
}

static struct falcon_clocksource falcon_clocksource = {
	.cs = {
		.name		= "FALCON",
		.rating		= 300,
		.read		= falcon_cs_read,
		.mask		= CLOCKSOURCE_MASK(32),
		.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
	},
};

static unsigned long get_fpi_hz(void)
{
	return clk_get_rate(clk_get_fpi());

}

static void __init falcon_clocksource_init(unsigned int timer)
{
	falcon_clocksource.cntreg =
		&(gptc->tc[timer>>1].count[timer&1]);
	lq_setup_timer(timer, 0, 1, 1, 0, 1, 0, 0);
	lq_start_timer(timer);
	clocksource_register_hz(&falcon_clocksource.cs, get_fpi_hz());
}

/*
 * ClockEvent related definitions and routines
 */
struct falcon_clockevent {
	struct clock_event_device cd;
	unsigned int timer;
	enum clock_event_mode mode;
};

static void falcon_set_mode(enum clock_event_mode mode,
			    struct clock_event_device *evt)
{
	struct falcon_clockevent *cd =
		container_of(evt, struct falcon_clockevent, cd);

	cd->mode = mode;
	switch (mode) {
#if 0
	case CLOCK_EVT_MODE_ONESHOT:
	case CLOCK_EVT_MODE_PERIODIC:
		ltq_sysctl_sleep_enable(0, 0);
		break;
#endif
	case CLOCK_EVT_MODE_SHUTDOWN:
		ltq_sysctl_sleep_enable(1, 1);
		break;
	default:
		break;
	}
}

static int falcon_set_next_event(unsigned long delta,
				 struct clock_event_device *evt)
{
	struct falcon_clockevent *cd =
		container_of(evt, struct falcon_clockevent, cd);
	unsigned int timer = cd->timer;

	lq_stop_and_clear_timer(timer);
	lq_set_reload(timer, delta);
	lq_start_timer(timer);
	gptc_w32_mask(0, 1<<timer, irnen); /* enable interrupt */

	return 0;
}

static struct falcon_clockevent falcon_clockevent = {
	.cd = {
		.name		= "FALCON_CLK_EVT",
		.features	= CLOCK_EVT_FEAT_ONESHOT,
		.rating		= 500,
		.set_mode	= falcon_set_mode,
		.set_next_event	= falcon_set_next_event,
	},
	.mode = CLOCK_EVT_MODE_UNUSED,
};

static irqreturn_t falcon_timer_interrupt(int irq, void *dev_id)
{
	struct falcon_clockevent *falcon_cd = dev_id;
	struct clock_event_device *cd = &falcon_cd->cd;
	unsigned int timer = falcon_cd->timer;

	gptc_w32_mask(1<<timer, 0, irnen); /* disable interrupt */
	gptc_w32(1 << timer, irncr);	/* ack interrupt */

	cd->event_handler(cd);
	return IRQ_HANDLED;
}

static struct irqaction falcon_timer_irq = {
	.handler	= falcon_timer_interrupt,
	.flags		= IRQF_DISABLED | IRQF_PERCPU | IRQF_TIMER,
	.name		= "falcon_timer",
	.dev_id		= &falcon_clockevent,
};

static void __init falcon_clockevent_init(unsigned int timer)
{
	struct clock_event_device *cd = &falcon_clockevent.cd;

	lq_stop_and_clear_timer(timer);
	falcon_clockevent.timer = timer;

	/* use cyclic timer here, 32bit-timer have problems with automatic stop! */
	lq_setup_timer(timer, 0x7FFFFFFF, 1, 1, 0, 0, 0, 1);

	clockevent_set_clock(cd, get_fpi_hz());
	cd->max_delta_ns = clockevent_delta2ns(0xFFFFFFFF, cd);
	cd->min_delta_ns = clockevent_delta2ns(1, cd);
	cd->irq = lq_get_irq_nr(timer);
	cd->cpumask = cpumask_of(0),
	clockevents_register_device(cd);
	setup_irq(cd->irq, &falcon_timer_irq);
	pr_info("clockevent device %s with timer %d, irq %d\n",
		cd->name, timer, cd->irq);
	pr_devel("\tmult=0x%X shift=%u\n", cd->mult, cd->shift);
}

/*
 * sysfs interface
 */
static struct class falcon_timer_class = {
	.name		= "falcon_timer",
};

static ssize_t falcon_timer_clkdis_show(struct class *class,
					struct class_attribute *attr,
					char *buf)
{
	return sprintf(buf, "%d\n", falcon_timer_clkdis);
}

static ssize_t
falcon_timer_clkdis_store(struct class *class,
			  struct class_attribute *attr,
			  const char *buf,
			  size_t count)
{
	int clkdis = simple_strtol(buf, NULL, 10);
	if (clkdis <= 0)
		clkdis = 0;
	else
		clkdis = 1;

	switch (falcon_clockevent.mode) {
	case CLOCK_EVT_MODE_ONESHOT:
	case CLOCK_EVT_MODE_PERIODIC:
		if (clkdis)
			ltq_sysctl_sleep_enable(0, 0);
		else
			ltq_sysctl_sleep_enable(1, 1);
		falcon_timer_clkdis = clkdis;
		return count;
	default:
		return 0;
	}
}

static CLASS_ATTR(clkdis, 0644, falcon_timer_clkdis_show,
				falcon_timer_clkdis_store);

static int __init falcon_timer_sysfs_init(void)
{
	int error;

	error = class_register(&falcon_timer_class);
	if (error) {
		pr_err("%s: class_register failed\n", __func__);
		return error;
	}
	error = class_create_file(&falcon_timer_class, &class_attr_clkdis);
	if (error) {
		pr_err("%s: class_create_file failed\n", __func__);
		class_unregister(&falcon_timer_class);
	}
	return error;
}

/*
 * timer module init
 */
static int __init falcon_timer_init(void)
{
	u32 id;

	pr_info("FALC(tm) ON clock driver\n");

	ltq_sysctl_sys1_activate(SYSCTRL_SYS1_GPTC);

	if (request_mem_region(GPON_GPTC_BASE, GPON_GPTC_SIZE, "GPTC") < 0) {
		pr_crit("Failed to request GPTC memory\n");
		return -ENXIO;
	}

	gptc = ioremap_nocache(GPON_GPTC_BASE, GPON_GPTC_SIZE);
	if (!gptc) {
		release_mem_region(GPON_GPTC_BASE, GPON_GPTC_SIZE);
		return -ENXIO;
	}
	id = gptc_r32(id);
	pr_devel("GPTC HW V%d with %d*2 Timer\n",
		id & GPTC_ID_REV_MASK,
		(id & GPTC_ID_CFG_MASK) >> GPTC_ID_CFG_OFFSET);

	/* enable GPTC module by setting prescaler to 1 */
	gptc_w32(GPTC_CLC_RMC_ENRMC, clc);

	falcon_clocksource_init(CLOCKSOURCE_TIMER);
	falcon_clockevent_init(CLOCKEVENT_TIMER);

	falcon_timer_sysfs_init();

	return 0;
}

arch_initcall(falcon_timer_init);
