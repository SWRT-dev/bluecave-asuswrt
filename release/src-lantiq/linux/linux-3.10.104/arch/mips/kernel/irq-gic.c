/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2008 Ralf Baechle (ralf@linux-mips.org)
 * Copyright (C) 2012 MIPS Technologies, Inc.  All rights reserved.
 */
#include <linux/bitmap.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/irq.h>
#include <linux/clocksource.h>
#include <linux/io.h>

#include <asm/gic.h>
#include <asm/setup.h>
#include <asm/traps.h>
#include <asm/gcmpregs.h>
#include <linux/hardirq.h>
#include <asm-generic/bitops/find.h>

/* The following defintions must match the static interrupt routing table */
#define GIC_VI2_NUM_INTRS	64
#define GIC_VI3_NUM_INTRS	64
#define GIC_VI4_NUM_INTRS	32
#define GIC_VI5_NUM_INTRS	32
#define GIC_VI6_NUM_INTRS	64

#define GIC_VI2_INTRS_BASE	0
#define GIC_VI3_INTRS_BASE	GIC_VI2_NUM_INTRS
#define GIC_VI4_INTRS_BASE	(GIC_VI2_NUM_INTRS + GIC_VI3_NUM_INTRS)
#define GIC_VI5_INTRS_BASE	(GIC_VI2_NUM_INTRS + GIC_VI3_NUM_INTRS \
					+ GIC_VI4_NUM_INTRS)
#define GIC_VI6_INTRS_BASE	(GIC_VI2_NUM_INTRS + GIC_VI3_NUM_INTRS \
					+ GIC_VI4_NUM_INTRS + GIC_VI5_NUM_INTRS)

/* offset = (irq_base /32) *4 = irq_base >> 3 */
#define GIC_VI2_SH_PEND		(GIC_SH_PEND_31_0_OFS \
					+ (GIC_VI2_INTRS_BASE >> 3))
#define GIC_VI2_SH_MASK		(GIC_SH_MASK_31_0_OFS \
					+ (GIC_VI2_INTRS_BASE >> 3))

#define GIC_VI3_SH_PEND		(GIC_SH_PEND_31_0_OFS \
					+ (GIC_VI3_INTRS_BASE >> 3))
#define GIC_VI3_SH_MASK		(GIC_SH_MASK_31_0_OFS \
					+ (GIC_VI3_INTRS_BASE >> 3))

#define GIC_VI4_SH_PEND		(GIC_SH_PEND_31_0_OFS \
					+ (GIC_VI4_INTRS_BASE >> 3))
#define GIC_VI4_SH_MASK		(GIC_SH_MASK_31_0_OFS \
					+ (GIC_VI4_INTRS_BASE >> 3))

#define GIC_VI5_SH_PEND		(GIC_SH_PEND_31_0_OFS \
					+ (GIC_VI5_INTRS_BASE >> 3))
#define GIC_VI5_SH_MASK		(GIC_SH_MASK_31_0_OFS \
					+ (GIC_VI5_INTRS_BASE >> 3))

#define GIC_VI6_SH_PEND		(GIC_SH_PEND_31_0_OFS \
					+ (GIC_VI6_INTRS_BASE >> 3))
#define GIC_VI6_SH_MASK		(GIC_SH_MASK_31_0_OFS \
					+ (GIC_VI6_INTRS_BASE >> 3))

struct gic_pcpu_mask {
	DECLARE_BITMAP(pcpu_mask, GIC_NUM_INTRS);
};

unsigned int gic_frequency;
unsigned int gic_present;
unsigned long _gic_base;
unsigned int gic_irq_base;
unsigned int gic_irq_flags[GIC_NUM_INTRS];
static int gic_vpes;
static DEFINE_SPINLOCK(gic_lock);
/* The index into this array is the vector # of the interrupt. */
struct gic_shared_intr_map gic_shared_intr_map[GIC_NUM_INTRS];

static struct gic_pcpu_mask pcpu_masks[NR_CPUS];

static inline bool gic_is_local_irq(unsigned int hwirq)
{
	return hwirq >= GIC_NUM_INTRS;
}

static inline unsigned int gic_hw_to_local_irq(unsigned int hwirq)
{
	return hwirq - GIC_NUM_INTRS;
}

static inline unsigned int gic_local_to_hw_irq(unsigned int irq)
{
	return irq + GIC_NUM_INTRS;
}

static inline bool local_irq_is_legacy(unsigned int local_irq)
{
	if ((local_irq == GIC_LOCAL_INT_TIMER)
		|| (local_irq == GIC_LOCAL_INT_PERFCTR))
		return true;
	else
		return false;
}


#if defined(CONFIG_CSRC_GIC) || defined(CONFIG_CEVT_GIC)
cycle_t gic_read_count(void)
{
	unsigned int hi, hi2, lo;

	do {
		GICREAD(GIC_REG(SHARED, GIC_SH_COUNTER_63_32), hi);
		GICREAD(GIC_REG(SHARED, GIC_SH_COUNTER_31_00), lo);
		GICREAD(GIC_REG(SHARED, GIC_SH_COUNTER_63_32), hi2);
	} while (hi2 != hi);

	return (((cycle_t) hi) << 32) + lo;
}

void gic_write_compare(cycle_t cnt)
{
	GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_COMPARE_HI),
				(int)(cnt >> 32));
	GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_COMPARE_LO),
				(int)(cnt & 0xffffffff));
}

cycle_t gic_read_compare(void)
{
	unsigned int hi, lo;

	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_COMPARE_HI), hi);
	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_COMPARE_LO), lo);

	return (((cycle_t) hi) << 32) + lo;
}
#endif

unsigned int gic_get_timer_pending(void)
{
	unsigned int vpe_pending;

	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_PEND), vpe_pending);
	return vpe_pending & GIC_VPE_PEND_TIMER_MSK;
}

void gic_bind_eic_interrupt(int irq, int set)
{
	/* Convert irq vector # to hw int # */
	irq -= GIC_PIN_TO_VEC_OFFSET;

	/* Set irq to use shadow set */
	GICWRITE(GIC_REG_ADDR(VPE_LOCAL, GIC_VPE_EIC_SS(irq)), set);
}

void gic_send_ipi(unsigned int intr)
{
	GICWRITE(GIC_REG(SHARED, GIC_SH_WEDGE), 0x80000000 | intr);
}

void gic_clear_edge(unsigned int intr)
{
	GICWRITE(GIC_REG(SHARED, GIC_SH_WEDGE), intr);
}

static void gic_eic_irq_dispatch(void)
{
	unsigned int cause = read_c0_cause();
	int irq;

	irq = (cause & ST0_IM) >> STATUSB_IP2;
	if (irq == 0)
		irq = -1;

	if (irq >= 0)
		do_IRQ(gic_irq_base + irq);
	else
		spurious_interrupt();
}

unsigned int gic_compare_int(void)
{
	unsigned int pending;

	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_PEND), pending);
	if (pending & GIC_VPE_PEND_CMP_MSK)
		return 1;
	else
		return 0;
}

static bool gic_local_irq_is_routable(int intr)
{
	u32 vpe_ctl;

	/* All local interrupts are routable in EIC mode. */
	if (cpu_has_veic)
		return true;

	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_CTL), vpe_ctl);
	switch (intr) {
	case GIC_LOCAL_INT_TIMER:
		return vpe_ctl & GIC_VPE_CTL_TIMER_RTBL_MSK;
	case GIC_LOCAL_INT_PERFCTR:
		return vpe_ctl & GIC_VPE_CTL_PERFCNT_RTBL_MSK;
	case GIC_LOCAL_INT_FDC:
		return vpe_ctl & GIC_VPE_CTL_FDC_RTBL_MSK;
	case GIC_LOCAL_INT_SWINT0:
	case GIC_LOCAL_INT_SWINT1:
		return vpe_ctl & GIC_VPE_CTL_SWINT_RTBL_MSK;
	default:
		return true;
	}
}

int gic_get_c0_compare_int(void)
{
	if (!gic_local_irq_is_routable(GIC_LOCAL_INT_TIMER))
		return MIPS_CPU_IRQ_BASE + cp0_compare_irq;
	return MIPS_GIC_LOCAL_IRQ_BASE + GIC_LOCAL_INT_TIMER;
}

int gic_get_c0_perfcount_int(void)
{
	if (!gic_local_irq_is_routable(GIC_LOCAL_INT_PERFCTR)) {
		/* Is the erformance counter shared with the timer? */
		if (cp0_perfcount_irq < 0)
			return -1;
		return MIPS_CPU_IRQ_BASE + cp0_perfcount_irq;
	}
	return MIPS_GIC_LOCAL_IRQ_BASE + GIC_LOCAL_INT_PERFCTR;
}

void gic_shared_irq_dispatch(void)
{
	unsigned int i, intr;
	unsigned long *pcpu_mask;
	unsigned long *pending_abs, *intrmask_abs;
	DECLARE_BITMAP(pending, GIC_NUM_INTRS);
	DECLARE_BITMAP(intrmask, GIC_NUM_INTRS);

	/* Get per-cpu bitmaps */
	pcpu_mask = pcpu_masks[smp_processor_id()].pcpu_mask;

	pending_abs = (unsigned long *)GIC_REG_ABS_ADDR(SHARED,
							GIC_SH_PEND_31_0_OFS);
	intrmask_abs = (unsigned long *)GIC_REG_ABS_ADDR(SHARED,
							GIC_SH_MASK_31_0_OFS);

	for (i = 0; i < BITS_TO_LONGS(GIC_NUM_INTRS); i++) {
		GICREAD(*pending_abs, pending[i]);
		GICREAD(*intrmask_abs, intrmask[i]);
		pending_abs++;
		intrmask_abs++;
	}

	bitmap_and(pending, pending, intrmask, GIC_NUM_INTRS);
	bitmap_and(pending, pending, pcpu_mask, GIC_NUM_INTRS);

	for_each_set_bit(intr, pending, GIC_NUM_INTRS)
		do_IRQ(gic_irq_base + intr);
}

#define GIC_SHARED_IRQ_DISPATCH(X)					\
do {									\
	unsigned int i, intr;						\
	unsigned long *pcpu_mask;					\
	unsigned long *pending_abs, *intrmask_abs;			\
	DECLARE_BITMAP(pending, GIC_VI##X##_NUM_INTRS);			\
	DECLARE_BITMAP(intrmask, GIC_VI##X##_NUM_INTRS);		\
									\
	/* Get per-cpu bitmaps */					\
	pcpu_mask = pcpu_masks[smp_processor_id()].pcpu_mask;		\
	pending_abs = (unsigned long *) GIC_REG_ABS_ADDR(SHARED,	\
						GIC_VI##X##_SH_PEND);	\
	intrmask_abs = (unsigned long *) GIC_REG_ABS_ADDR(SHARED,	\
						GIC_VI##X##_SH_MASK);	\
	for (i = 0; i < BITS_TO_LONGS(GIC_VI##X##_NUM_INTRS); i++) {	\
		GICREAD(*pending_abs, pending[i]);			\
		GICREAD(*intrmask_abs, intrmask[i]);			\
		pending_abs++;						\
		intrmask_abs++;						\
	}								\
	bitmap_and(pending, pending, intrmask, GIC_VI##X##_NUM_INTRS);	\
	bitmap_and(pending, pending, pcpu_mask				\
		+ (GIC_VI##X##_INTRS_BASE >> 5), GIC_VI##X##_NUM_INTRS);\
	for_each_set_bit(intr, pending, GIC_VI##X##_NUM_INTRS)		\
		do_IRQ(gic_irq_base + GIC_VI##X##_INTRS_BASE + intr);	\
} while (0)

#define GIC_VIx_IRQ_DISPATCH(x)						\
void gic_shared_irq_vi ## x ##_dispatch(void)				\
{									\
	GIC_SHARED_IRQ_DISPATCH(x);					\
}

GIC_VIx_IRQ_DISPATCH(2)
GIC_VIx_IRQ_DISPATCH(3)
GIC_VIx_IRQ_DISPATCH(4)
GIC_VIx_IRQ_DISPATCH(5)
GIC_VIx_IRQ_DISPATCH(6)


void gic_local_irq_dispatch(void)
{
	unsigned int intr;
	unsigned long pending, masked;

	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_PEND), pending);
	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_MASK), masked);

	bitmap_and(&pending, &pending, &masked, GIC_NUM_LOCAL_INTRS);

	for_each_set_bit(intr, &pending, GIC_NUM_LOCAL_INTRS)
		do_IRQ(MIPS_GIC_LOCAL_IRQ_BASE + intr);
}

static void gic_mask_local_irq(int irq)
{
	int local_irq = gic_hw_to_local_irq(irq);

	if (local_irq_is_legacy(local_irq)) {
		int i;
		unsigned long flags;

		spin_lock_irqsave(&gic_lock, flags);
		for (i = 0; i < gic_vpes; i++) {
			if (ltq_vpe_run_linux_os(i)) {
				GICWRITE(GIC_REG(VPE_LOCAL,
					GIC_VPE_OTHER_ADDR), i);
				GICWRITE(GIC_REG(VPE_OTHER,
					GIC_VPE_RMASK), 1 << local_irq);
			}
		}
		spin_unlock_irqrestore(&gic_lock, flags);
	} else
		GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_RMASK), 1 << local_irq);
}

static void gic_unmask_local_irq(int irq)
{
	int local_irq = gic_hw_to_local_irq(irq);

	if (local_irq_is_legacy(local_irq)) {
		int i;
		unsigned long flags;

		spin_lock_irqsave(&gic_lock, flags);
		for (i = 0; i < gic_vpes; i++) {
			if (ltq_vpe_run_linux_os(i)) {
				GICWRITE(GIC_REG(VPE_LOCAL,
					GIC_VPE_OTHER_ADDR), i);
				GICWRITE(GIC_REG(VPE_OTHER,
					GIC_VPE_SMASK), 1 << local_irq);
			}
		}
		spin_unlock_irqrestore(&gic_lock, flags);
	} else
		GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_SMASK), 1 << local_irq);
}

static void gic_mask_irq(struct irq_data *d)
{
	unsigned int irq = d->irq - gic_irq_base;

	if (gic_is_local_irq(irq))
		gic_mask_local_irq(irq);
	else
		GIC_CLR_INTR_MASK(irq);
}

static void gic_unmask_irq(struct irq_data *d)
{
	unsigned int irq = d->irq - gic_irq_base;

	if (gic_is_local_irq(irq))
		gic_unmask_local_irq(irq);
	else {
#ifndef CONFIG_SOC_GRX500_A21
	#ifdef CONFIG_PCIE_LANTIQ
		pcie_intx_war(irq);
	#endif /* CONFIG_PCIE_LANTIQ */
#endif /* CONFIG_SOC_GRX500_A21 */
		GIC_SET_INTR_MASK(irq);
	}
}

void __weak gic_irq_ack(struct irq_data *d)
{
	unsigned int irq = d->irq - gic_irq_base;

	if (gic_is_local_irq(irq))
		gic_mask_local_irq(irq);
	else {
		GIC_CLR_INTR_MASK(irq);

		/* Clear edge detector */
		if (gic_irq_flags[irq] & GIC_TRIG_EDGE)
			GICWRITE(GIC_REG(SHARED, GIC_SH_WEDGE), irq);
	}
}

void __weak gic_finish_irq(struct irq_data *d)
{
	unsigned int irq = d->irq - gic_irq_base;

	if (gic_is_local_irq(irq))
		gic_unmask_local_irq(irq);
	else {
#ifndef CONFIG_SOC_GRX500_A21
	#ifdef CONFIG_PCIE_LANTIQ
		pcie_intx_war(irq);
	#endif /* CONFIG_PCIE_LANTIQ */
#endif /* CONFIG_SOC_GRX500_A21 */
		/* Fixed the disable_irq /enable_irq with percpu interrupt */
		if (!(irqd_irq_disabled(d) && irqd_irq_masked(d)))
			GIC_SET_INTR_MASK(irq);
	}
}

static int gic_set_type(struct irq_data *d, unsigned int type)
{
	unsigned int irq = d->irq - gic_irq_base;
	unsigned long flags;
	bool is_edge;

	if (gic_is_local_irq(irq))
		return -EINVAL;

	spin_lock_irqsave(&gic_lock, flags);
	switch (type & IRQ_TYPE_SENSE_MASK) {
	case IRQ_TYPE_EDGE_FALLING:
		GIC_SET_POLARITY(irq, GIC_POL_NEG);
		GIC_SET_TRIGGER(irq, GIC_TRIG_EDGE);
		GIC_SET_DUAL(irq, GIC_TRIG_DUAL_DISABLE);
		is_edge = true;
		break;
	case IRQ_TYPE_EDGE_RISING:
		GIC_SET_POLARITY(irq, GIC_POL_POS);
		GIC_SET_TRIGGER(irq, GIC_TRIG_EDGE);
		GIC_SET_DUAL(irq, GIC_TRIG_DUAL_DISABLE);
		is_edge = true;
		break;
	case IRQ_TYPE_EDGE_BOTH:
		/* polarity is irrelevant in this case */
		GIC_SET_TRIGGER(irq, GIC_TRIG_EDGE);
		GIC_SET_DUAL(irq, GIC_TRIG_DUAL_ENABLE);
		is_edge = true;
		break;
	case IRQ_TYPE_LEVEL_LOW:
		GIC_SET_POLARITY(irq, GIC_POL_NEG);
		GIC_SET_TRIGGER(irq, GIC_TRIG_LEVEL);
		GIC_SET_DUAL(irq, GIC_TRIG_DUAL_DISABLE);
		is_edge = false;
		break;
	case IRQ_TYPE_LEVEL_HIGH:
	default:
		GIC_SET_POLARITY(irq, GIC_POL_POS);
		GIC_SET_TRIGGER(irq, GIC_TRIG_LEVEL);
		GIC_SET_DUAL(irq, GIC_TRIG_DUAL_DISABLE);
		is_edge = false;
		break;
	}

	if (is_edge) {
		gic_irq_flags[irq] |= GIC_TRIG_EDGE;
		__irq_set_handler_locked(d->irq, handle_edge_irq);
	} else {
		gic_irq_flags[irq] &= ~GIC_TRIG_EDGE;
		__irq_set_handler_locked(d->irq, handle_level_irq);
	}
	spin_unlock_irqrestore(&gic_lock, flags);

	return 0;
}

#ifdef CONFIG_SMP
static int gic_set_affinity(struct irq_data *d, const struct cpumask *cpumask,
			    bool force)
{
	unsigned int irq = (d->irq - gic_irq_base);
	cpumask_t	tmp = CPU_MASK_NONE;
	unsigned long	flags;
	int		cpu;

	if (gic_is_local_irq(irq))
		return -EINVAL;

	cpumask_and(&tmp, cpumask, cpu_online_mask);
	if (cpus_empty(tmp))
		return -EINVAL;

	/* Assumption : cpumask refers to a single CPU */
	spin_lock_irqsave(&gic_lock, flags);

	/* Re-route this IRQ */
	GIC_SH_MAP_TO_VPE_SMASK(irq, first_cpu(tmp));

	/* Update the pcpu_masks */
	for_each_possible_cpu(cpu)
		clear_bit(irq, pcpu_masks[cpu].pcpu_mask);
	set_bit(irq, pcpu_masks[first_cpu(tmp)].pcpu_mask);

	cpumask_copy(d->affinity, cpumask);
	spin_unlock_irqrestore(&gic_lock, flags);

	return IRQ_SET_MASK_OK_NOCOPY;
}
#endif

static struct irq_chip gic_irq_controller = {
	.name			=	"MIPS GIC",
	.irq_enable		=	gic_unmask_irq,
	.irq_disable		=	gic_irq_ack, /* mask, Yield war */
	.irq_ack		=	gic_irq_ack,
	.irq_mask		=	gic_mask_irq,
	.irq_mask_ack		=	gic_mask_irq,
	.irq_unmask		=	gic_unmask_irq,
	.irq_set_type		=	gic_set_type,
	.irq_eoi		=	gic_finish_irq,
#ifdef CONFIG_SMP
	.irq_set_affinity	=	gic_set_affinity,
#endif
};

int gic_yield_setup(unsigned int cpu, unsigned int pin, unsigned int intr)
{
	int cpux;
	int irq;
	unsigned long flags;

	/* Sanity check */
	if ((cpu >= nr_cpu_ids) || (pin > 0xF) || (intr < MIPS_GIC_IRQ_BASE))
		return -EINVAL;

	irq = intr - MIPS_GIC_IRQ_BASE;

	spin_lock_irqsave(&gic_lock, flags);
	GICWRITE(GIC_REG_ADDR(SHARED, GIC_SH_MAP_TO_PIN(irq)),
		GIC_MAP_TO_YQ_MSK | pin);
	/* Setup Intr to CPU mapping */
	GIC_SH_MAP_TO_VPE_SMASK(irq, cpu);
	/* Clear all yield related percpu mask */
	for_each_possible_cpu(cpux)
		clear_bit(irq, pcpu_masks[cpux].pcpu_mask);
	spin_unlock_irqrestore(&gic_lock, flags);
	return 0;
}
EXPORT_SYMBOL_GPL(gic_yield_setup);

static void __init gic_setup_intr(unsigned int intr, unsigned int cpu,
	unsigned int pin, unsigned int polarity, unsigned int trigtype,
	unsigned int flags)
{
	struct gic_shared_intr_map *map_ptr;

	/* 1 is hardcoded Voice FW VPE1/core0 */
	if (!ltq_vpe_run_linux_os(1) && (flags == GIC_FLAG_MUX))
		return;

	/* Setup Intr to Pin mapping */
	if (pin & GIC_MAP_TO_NMI_MSK) {
		int i;

		GICWRITE(GIC_REG_ADDR(SHARED, GIC_SH_MAP_TO_PIN(intr)), pin);
		/* FIXME: hack to route NMI to all cpu's */
		for (i = 0; i < num_possible_cpus(); i += 32) {
			GICWRITE(GIC_REG_ADDR(SHARED,
					  GIC_SH_MAP_TO_VPE_REG_OFF(intr, i)),
				 0xffffffff);
		}
	} else if (pin & GIC_MAP_TO_YQ_MSK) {
		GICWRITE(GIC_REG_ADDR(SHARED, GIC_SH_MAP_TO_PIN(intr)), pin);
		/* Setup Intr to CPU mapping */
		GIC_SH_MAP_TO_VPE_SMASK(intr, cpu);
	} else {
		GICWRITE(GIC_REG_ADDR(SHARED, GIC_SH_MAP_TO_PIN(intr)),
			 GIC_MAP_TO_PIN_MSK | pin);
		/* Setup Intr to CPU mapping */
		GIC_SH_MAP_TO_VPE_SMASK(intr, cpu);
		if (cpu_has_veic) {
			set_vi_handler(pin + GIC_PIN_TO_VEC_OFFSET,
				gic_eic_irq_dispatch);
			map_ptr = &gic_shared_intr_map
				[pin + GIC_PIN_TO_VEC_OFFSET];
			if (map_ptr->num_shared_intr >= GIC_MAX_SHARED_INTR)
				BUG();
			map_ptr->intr_list[map_ptr->num_shared_intr++] = intr;
		}
	}

	/* Setup Intr Polarity */
	GIC_SET_POLARITY(intr, polarity);

	/* Setup Intr Trigger Type */
	GIC_SET_TRIGGER(intr, trigtype);

	/* Init Intr Masks */
	GIC_CLR_INTR_MASK(intr);

	/* Initialise per-cpu Interrupt software masks */
	set_bit(intr, pcpu_masks[cpu].pcpu_mask);

	if ((flags & GIC_FLAG_TRANSPARENT) && (cpu_has_veic == 0))
		GIC_SET_INTR_MASK(intr);

	if (trigtype == GIC_TRIG_EDGE)
		gic_irq_flags[intr] |= GIC_TRIG_EDGE;
}

static unsigned int local_irq_to_map_off(unsigned int intr)
{
	switch (intr) {
	case GIC_LOCAL_INT_PERFCTR:
		return GIC_VPE_PERFCTR_MAP_OFS;
	case GIC_LOCAL_INT_SWINT0:
		return GIC_VPE_SWINT0_MAP_OFS;
	case GIC_LOCAL_INT_SWINT1:
		return GIC_VPE_SWINT1_MAP_OFS;
	case GIC_LOCAL_INT_FDC:
		return GIC_VPE_FDC_MAP_OFS;
	default:
		return GIC_VPE_MAP_OFS + 4 * (intr);
	}
}

static void __init gic_setup_local_intr(unsigned int intr, unsigned int pin,
	unsigned int flags)
{
	int i;
	struct gic_shared_intr_map *map_ptr;
	unsigned int local_irq = gic_hw_to_local_irq(intr);

	/* Setup Intr to Pin mapping */
	for (i = 0; i < nr_cpu_ids; i++) {
		if (ltq_vpe_run_linux_os(i)) {
			GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_OTHER_ADDR), i);
			if (pin & GIC_MAP_TO_NMI_MSK) {
				GICWRITE(GIC_REG_ADDR(VPE_OTHER,
					local_irq_to_map_off(local_irq)), pin);
			} else {
				GICWRITE(GIC_REG_ADDR(VPE_OTHER,
					local_irq_to_map_off(local_irq)),
						GIC_MAP_TO_PIN_MSK | pin);
			}
			if (!gic_local_irq_is_routable(local_irq))
				continue;
			/* Init Intr Masks */
			GICWRITE(GIC_REG(VPE_OTHER, GIC_VPE_RMASK),
				1 << local_irq);
		}
	}

	if (!(pin & GIC_MAP_TO_NMI_MSK) && cpu_has_veic) {
		set_vi_handler(pin + GIC_PIN_TO_VEC_OFFSET,
			gic_eic_irq_dispatch);
		map_ptr = &gic_shared_intr_map[pin + GIC_PIN_TO_VEC_OFFSET];
		if (map_ptr->num_shared_intr >= GIC_MAX_SHARED_INTR)
			BUG();
		map_ptr->intr_list[map_ptr->num_shared_intr++] = intr;
	}
}

static void __init gic_basic_init(int numintrs, int numvpes,
			struct gic_intr_map *intrmap, int mapsize)
{
	unsigned int i, cpu;
	unsigned int pin_offset = 0;

	board_bind_eic_interrupt = &gic_bind_eic_interrupt;

	/* Setup defaults */
	for (i = 0; i < numintrs; i++) {
		/* Bypass VMB/FW IPI initialed by u-boot/IBL */
		if ((i < GIC_NUM_INTRS) && (i < mapsize)) {
			if (intrmap[i].flags == GIC_FLAG_VMB_IPI)
				continue;
		}
		GIC_SET_POLARITY(i, GIC_POL_POS);
		GIC_SET_TRIGGER(i, GIC_TRIG_LEVEL);
		GIC_CLR_INTR_MASK(i);
		if (i < GIC_NUM_INTRS) {
			gic_irq_flags[i] = 0;
			gic_shared_intr_map[i].num_shared_intr = 0;
			gic_shared_intr_map[i].local_intr_mask = 0;
		}
	}
	/*
	 * In EIC mode, the HW_INT# is offset by (2-1). Need to subtract
	 * one because the GIC will add one (since 0=no intr).
	 */
	if (cpu_has_veic)
		pin_offset = (GIC_CPU_TO_VEC_OFFSET - GIC_PIN_TO_VEC_OFFSET);
	/* Setup specifics */
	for (i = 0; i < mapsize; i++) {
		cpu = intrmap[i].cpunum;
		if (cpu == GIC_UNUSED)
			continue;
		if (gic_is_local_irq(i))
			gic_setup_local_intr(i,
				intrmap[i].pin + pin_offset,
				intrmap[i].flags);
		else
			gic_setup_intr(i,
				intrmap[i].cpunum,
				intrmap[i].pin + pin_offset,
				intrmap[i].polarity,
				intrmap[i].trigtype,
				intrmap[i].flags);
	}
}

void __init gic_init(unsigned long gic_base_addr,
		     unsigned long gic_addrspace_size,
		     struct gic_intr_map *intr_map, unsigned int intr_map_size,
		     unsigned int irqbase)
{
	unsigned int gicconfig;
	int numintrs;

	_gic_base = (unsigned long) ioremap_nocache(gic_base_addr,
						    gic_addrspace_size);
	gic_irq_base = irqbase;

	GICREAD(GIC_REG(SHARED, GIC_SH_CONFIG), gicconfig);
	numintrs = (gicconfig & GIC_SH_CONFIG_NUMINTRS_MSK) >>
		   GIC_SH_CONFIG_NUMINTRS_SHF;
	numintrs = ((numintrs + 1) * 8);

	gic_vpes = (gicconfig & GIC_SH_CONFIG_NUMVPES_MSK) >>
		  GIC_SH_CONFIG_NUMVPES_SHF;
	gic_vpes = gic_vpes + 1;
	gic_basic_init(numintrs, gic_vpes, intr_map, intr_map_size);

	gic_platform_init(GIC_NUM_INTRS + GIC_NUM_LOCAL_INTRS,
		&gic_irq_controller);
}
