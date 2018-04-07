#include <linux/init.h>
#include <linux/clocksource.h>
#include <linux/time.h>
#include <linux/clk.h>
#include <lantiq_gptc.h>

#define GPTC_CLKSRC_TIMER3_A 4

#define to_gptc_timer_clksrc(x) \
		container_of(x, struct gptc_clocksource, cs)
/**
 * struct gptu_timer - This definition defines local timer structure
 *
 * @base_addr:	Base address of timer
 * @clk:	Associated clock source
 * @clk_rate_change_nb	Notifier block for clock rate changes
 */
struct gptc_timer {
	void __iomem *base_addr;
	int timer;
};

struct gptc_clocksource {
	struct gptc_timer	gptc;
	struct clocksource	cs;
};

static struct gptc_clocksource gptc_clksrc;
/**

 *
 * returns: Current timer counter register value
 **/
static cycle_t gptc_clocksource_read(struct clocksource *cs)
{
	struct gptc_timer *timer = &to_gptc_timer_clksrc(cs)->gptc;
/*	pr_info("reading 0x%x\r\n", __raw_readl(timer->base_addr +
                              GPTU_CLKSRC_CNT(0)));*/
	return (cycle_t)__raw_readl(timer->base_addr + GPTU_CLKSRC_CNT(timer->timer));
}

static void __init gptu_setup_clocksource(u32 freq, void __iomem *base)
{
	int err;
	u32 reg;

	gptc_clksrc.gptc.base_addr = base;
	gptc_clksrc.gptc.timer = GPTC_CLKSRC_TIMER3_A;
	gptc_clksrc.cs.name = "gptc-timer";
	gptc_clksrc.cs.rating = 200;
	gptc_clksrc.cs.read = gptc_clocksource_read;
	gptc_clksrc.cs.mask = CLOCKSOURCE_MASK(32);
	gptc_clksrc.cs.flags = CLOCK_SOURCE_IS_CONTINUOUS;
	/*
	 * Setup the clock source counter to be an incrementing counter
	 * with no interrupt and it rolls over at 0xFFFFFFFF. Let it start running now.
	 */
	pr_debug("%s  0x%x\r\n", __func__,(unsigned int) base+ GPTU_CLKSRC_IRNEN);
	reg = __raw_readl( base + GPTU_CLKSRC_CON(GPTC_CLKSRC_TIMER3_A));
	reg |= CON_CLKSRC_DIR | CON_CLKSRC_EXT;
	__raw_writel(reg, base + GPTU_CLKSRC_CON(GPTC_CLKSRC_TIMER3_A));
	
	__raw_writel(0, base + GPTU_CLKSRC_RLD(GPTC_CLKSRC_TIMER3_A));
	__raw_writel(RUN_CLKSRC_SEN | RUN_CLKSRC_RL, base + GPTU_CLKSRC_RUN(GPTC_CLKSRC_TIMER3_A));

	/*wait for status bit*/
	while ((__raw_readl( base + GPTU_CLKSRC_CON(GPTC_CLKSRC_TIMER3_A)) & 1) == 0) {
	}

	err = clocksource_register_hz(&gptc_clksrc.cs, freq);
	if (WARN_ON(err)) {
		return;
	}
	pr_info("%s: done ! \n", __func__);
}

int __init gptc_clocksource_init()
{
	void __iomem *gptc0_baseaddr;

	gptc0_baseaddr = (void __iomem *) (IO_BASE | GPTC0_BASE_ADDRESS);
	gptu_setup_clocksource(GPTC_TIMER_FREQ, gptc0_baseaddr);
	return 0;
}
