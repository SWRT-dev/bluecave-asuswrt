#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/clockchips.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/slab.h>
#include <linux/clk-provider.h>
#include <asm/time.h>
#include <lantiq_gptc.h>

#define to_gptc_clkevent(x) \
		container_of(x, struct gptc_clockevent, ce)
spinlock_t cevt_gptc_lock;

static void __iomem *gptc0_baseaddr;
static void __iomem *gptc1_baseaddr;
static u32 gptc_timer[NR_CPUS] = {0, 0, 2, 4};
static u32 gptc_irq[NR_CPUS] = {168, 126, 170, 172};

/**
 * struct gptc_timer - This definition defines local timer structure
 *
 * @base_addr:	Base address of timer
 * @clk:	Associated clock source
 * @clk_rate_change_nb	Notifier block for clock rate changes
 */
struct gptc_timer {
	void __iomem *base_addr;
	int timer;
};

struct gptc_clockevent {
	struct gptc_timer gptc;
	struct clock_event_device ce;
};

/**
 * gptc_set_interval - Set the timer interval value
 *
 * @timer:	Pointer to the timer instance
 * @cycles:	Timer interval ticks
 **/
static void gptc_set_interval(struct gptc_timer *timer,
					unsigned long cycles)
{
	__raw_writel(RUN_CLKSRC_STOP, timer->base_addr +  GPTU_CLKSRC_RUN(timer->timer));
	__raw_writel(cycles, timer->base_addr +  GPTU_CLKSRC_RLD(timer->timer));
	__raw_writel(RUN_CLKSRC_SEN | RUN_CLKSRC_RL, timer->base_addr +  GPTU_CLKSRC_RUN(timer->timer));
}

/**
 * gptc_clock_event_interrupt - Clock event timer interrupt handler
 *
 * @irq:	IRQ number of the Timer
 * @dev_id:	void pointer to the gptc_timer instance
 *
 * returns: Always IRQ_HANDLED - success
 **/
static irqreturn_t gptc_clock_event_interrupt(int irq, void *dev_id)
{
	struct gptc_clockevent *gptce = dev_id;
	struct gptc_timer *timer = &gptce->gptc;

	/*spin_lock(&cevt_gptc_lock);*/
	__raw_writel(BIT(timer->timer), timer->base_addr + GPTU_CLKSRC_IRNCR);
	/*spin_unlock(&cevt_gptc_lock);*/
	gptce->ce.event_handler(&gptce->ce);
	return IRQ_HANDLED;
}


/**
 * gptc_set_next_event - Sets the time interval for next event
 *
 * @cycles:	Timer interval ticks
 * @evt:	Address of clock event instance
 *
 * returns: Always 0 - success
 **/
static int gptc_set_next_event(unsigned long cycles,
					struct clock_event_device *evt)
{
	struct gptc_clockevent *gptce = to_gptc_clkevent(evt);
	struct gptc_timer *timer = &gptce->gptc;
	gptc_set_interval(timer, cycles);
	return 0;
}

/**
 * gptc_set_mode - Sets the mode of timer
 *
 * @mode:	Mode to be set
 * @evt:	Address of clock event instance
 **/
static void gptc_set_mode(enum clock_event_mode mode,
					struct clock_event_device *evt)
{
	struct gptc_clockevent *gptce = to_gptc_clkevent(evt);
	struct gptc_timer *timer = &gptce->gptc;
	u32 reg;

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		break;
	case CLOCK_EVT_MODE_ONESHOT:
		/* Make it 32-bit timer using EXT bit */
		reg = __raw_readl(timer->base_addr + GPTU_CLKSRC_CON(timer->timer));
		reg |= (CON_CLKSRC_EXT | CON_CLKSRC_STP);
		__raw_writel(reg, timer->base_addr + GPTU_CLKSRC_CON(timer->timer));
	break;
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
		__raw_writel(RUN_CLKSRC_STOP, timer->base_addr +  GPTU_CLKSRC_RUN(timer->timer));
		__raw_writel(0, timer->base_addr +  GPTU_CLKSRC_RLD(timer->timer));
		while ((__raw_readl(timer->base_addr + GPTU_CLKSRC_CON(timer->timer)) & 1) == 1) {
		}
		break;
	case CLOCK_EVT_MODE_RESUME:
		__raw_writel(RUN_CLKSRC_SEN, timer->base_addr +  GPTU_CLKSRC_RUN(timer->timer));
		break;
	}
}
static DEFINE_PER_CPU(struct gptc_clockevent, gptc_ce);
static DEFINE_PER_CPU(struct irqaction, gptc_clockevent_irqaction);
static DEFINE_PER_CPU(char [32], gptc_clockevent_name);

static void gptc_setup_clockevent(void);
static void __init gptc_setup_clockevent()
{
	u32 reg;
	unsigned int cpu = smp_processor_id();
	unsigned int irq; 
	unsigned int timer;
	struct gptc_clockevent *gptcce = &per_cpu(gptc_ce, cpu);
	unsigned char *name = per_cpu(gptc_clockevent_name, cpu);
	unsigned long sys_flag;
	struct irqaction *action = &per_cpu(gptc_clockevent_irqaction, cpu);
	/*struct cpumask cpumask;*/
	int err;

	BUG_ON(cpu > 3);

	timer = gptc_timer[cpu];
	irq = gptc_irq[cpu];

	sprintf(name, "gptc_clockevent_%d", cpu);
	/* Special case where voice fw is not running on VPE1 */
	if (cpu == 1)
		gptcce->gptc.base_addr = (void *) gptc0_baseaddr; 
	else
		gptcce->gptc.base_addr = (void *) gptc1_baseaddr; 
	gptcce->ce.name = name;
	gptcce->ce.features = CLOCK_EVT_FEAT_ONESHOT;
	clockevent_set_clock(&gptcce->ce, GPTC_TIMER_FREQ);
	gptcce->ce.set_next_event = gptc_set_next_event;
	gptcce->ce.set_mode = gptc_set_mode;
	gptcce->ce.rating = 400;
	gptcce->ce.irq = irq;
	gptcce->ce.cpumask = cpumask_of(cpu);
	gptcce->ce.max_delta_ns	= clockevent_delta2ns(0xffffffff, &gptcce->ce);
	gptcce->ce.min_delta_ns	= clockevent_delta2ns(0x1, &gptcce->ce);
	gptcce->gptc.timer = timer;
	clockevents_register_device(&gptcce->ce);

	/* set up the interrupts here */
	action->handler = gptc_clock_event_interrupt;
	action->flags	= IRQF_PERCPU | IRQF_TIMER;
	action->name	= name;
	action->dev_id	= gptcce;

	setup_irq(irq, action);

	/*cpumask.bits[0] = (0x1 << cpu);*/
	/*pr_info("setting the affinity of irq: %d to %x\n", irq, (unsigned int)(cpumask.bits[0]));*/
	err = irq_set_affinity(irq, cpumask_of(cpu));
	/*err = irq_set_affinity(irq, &cpumask);*/

	if (err < 0)
		pr_err("couldn't set the affinity for the irq: %d\n", irq);

	/* Enable the interrupt at the source */
	spin_lock_irqsave(&cevt_gptc_lock, sys_flag);
	reg = __raw_readl(gptcce->gptc.base_addr + GPTU_CLKSRC_IRNEN);
	reg |= BIT(timer);
	__raw_writel(reg, gptcce->gptc.base_addr + GPTU_CLKSRC_IRNEN);
	spin_unlock_irqrestore(&cevt_gptc_lock, sys_flag);

	pr_info ("clock event init done !\n");
}

/**
 * gptc_timer_init - Initialize the timer
 *
 * Initializes the timer hardware and register the clock source and clock event
 * timers with Linux kernel timer framework
 */
int __cpuinit gptc_clockevent_init()
{
	spin_lock_init(&cevt_gptc_lock);

	gptc_setup_clockevent();
	return 0;
}

int __cpuinit gptc_module_init()
{
	struct clk *clk;

	/* Enable the GPTC1 module in CGU */
	clk = clk_get_sys("16300000.gptu", NULL);
	if (IS_ERR(clk)) {
		pr_err("ERROR: timer input clock not found\n");
		BUG();
	}
	clk_enable(clk);

	/* Enable the GPTC1 module */
	gptc1_baseaddr = (void __iomem *) (IO_BASE | GPTC1_BASE_ADDRESS);

	__raw_writel(0x00, gptc1_baseaddr + GPTU_CLKSRC_IRNEN);
	__raw_writel(0xff, gptc1_baseaddr + GPTU_CLKSRC_IRNCR);
	__raw_writel(CLC_CLKSRC_SMC|CLC_CLKSRC_RMC, gptc1_baseaddr + GPTU_CLKSRC_CLC);


	/* Enable the GPTC0 module in CGU */
	clk = clk_get_sys("16b00000.gptu", NULL);
	if (IS_ERR(clk)) {
		pr_err("ERROR: timer input clock not found\n");
		BUG();
	}
	clk_enable(clk);

	/* Enable the GPTC0 module */
	gptc0_baseaddr = (void __iomem *) (IO_BASE | GPTC0_BASE_ADDRESS);
	__raw_writel(0x00, gptc0_baseaddr + GPTU_CLKSRC_IRNEN);
	__raw_writel(0xff, gptc0_baseaddr + GPTU_CLKSRC_IRNCR);
	__raw_writel(CLC_CLKSRC_SMC|CLC_CLKSRC_RMC, gptc0_baseaddr + GPTU_CLKSRC_CLC);

	return 0;
}
