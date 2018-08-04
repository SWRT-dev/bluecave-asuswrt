/******************************************************************************

                         Copyright (c) 2012, 2014, 2015
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/clk.h>
#include <asm/gic.h>

/* WDT MACROs */
#define WD_RESET	(1 << 7)
#define WD_INTR		(1 << 6)
#define WD_NWAIT	(1 << 5)

/* Second Countdown mode */
#define WD_TYPE_SCD	(1 << 1)
#define WD_START	(0x1)

/* RCU MACROs */
void __iomem *rcu_membase = (void *)0xb6000000;
#define RCU_IAP_WDT_RST_EN            0x0050
#define	RCU_WDTx_RESET		      0xf 			

#define TIMER_MARGIN	300		/* Default is 300 seconds > MAX_TIMEOUT so probe reads from DT file timeout-sec */

static unsigned int timeout = TIMER_MARGIN;	/* in seconds */
module_param(timeout, uint, 0);
MODULE_PARM_DESC(timeout, 
	"Watchdog timeout in seconds "
        "(default=" __MODULE_STRING(TIMER_MARGIN) ")");

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout,
	"Watchdog cannot be stopped once started "
	"(default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");


extern unsigned long ltq_grx500_cpu_hz(void);
extern void gic_watchdog_irq(uint32_t irq);
DEFINE_PER_CPU(struct watchdog_device, grx500wdt);
static unsigned long cpu_clk;

static inline void Enable_WDT_intr(void* wdtirq) {
	printk("[%s]:[%d] irq= %d cpu = %d \n", __FUNCTION__, __LINE__, (*(u32 *)wdtirq), smp_processor_id());

	enable_percpu_irq((*(u32 *)wdtirq), 0);
}

static int grx500wdt_start_other(struct watchdog_device *wdt_dev)
{
	uint32_t config0;
	GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_OTHER_ADDR), wdt_dev->id);
	wmb();	
	GICREAD(GIC_REG(VPE_OTHER, GIC_VPE_WD_CONFIG0), config0);
	rmb();
	GICWRITE(GIC_REG(VPE_OTHER, GIC_VPE_WD_CONFIG0), (config0 | WD_START));
  wmb();

	return 0;
}

static int grx500wdt_stop_other(struct watchdog_device *wdt_dev)
{
	uint32_t config0;
	GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_OTHER_ADDR), wdt_dev->id);
	wmb();
	GICREAD(GIC_REG(VPE_OTHER, GIC_VPE_WD_CONFIG0), config0);
	rmb();
	GICWRITE(GIC_REG(VPE_OTHER, GIC_VPE_WD_CONFIG0), (config0 & ~WD_START));
	wmb();

	return 0;
}

static int grx500wdt_start(struct watchdog_device *wdt_dev)
{
	uint32_t config0;
	if(smp_processor_id() != wdt_dev->id){
		printk(KERN_ERR "cant start on cpu [%d] rather than cpu [%d]\n", smp_processor_id(), wdt_dev->id);
		panic("Please set affinity of this process to cpu %d\n",wdt_dev->id);
	}
	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_WD_CONFIG0), config0);
	rmb();
	GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_WD_CONFIG0), (config0 | WD_START));
  wmb();

	return 0;
}

static int grx500wdt_stop(struct watchdog_device *wdt_dev)
{
	uint32_t config0;
	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_WD_CONFIG0), config0);
	rmb();
	GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_WD_CONFIG0), (config0 & ~WD_START));
	wmb();

	return 0;
}

static int grx500wdt_set_timeout(struct watchdog_device *wdt_dev,
			       unsigned int new_timeout)
{
	int timeout_cal;

	if(wdt_dev->id != smp_processor_id()){
		printk(KERN_ERR "cant set timeout on cpu [%d] rather than cpu [%d]\n", smp_processor_id(), wdt_dev->id);
		panic("Please set affinity of this process to cpu %d\n",wdt_dev->id);
	}
	wdt_dev->timeout = new_timeout;
	timeout_cal = (new_timeout > wdt_dev->max_timeout)?wdt_dev->max_timeout:wdt_dev->timeout;
	printk("%s: timeout = %d, cpu = %d, id = %d \n", __func__, new_timeout, smp_processor_id(), wdt_dev->id);

	grx500wdt_stop(wdt_dev);

	GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_WD_INITIAL0), (cpu_clk * timeout_cal));
	wmb();

	grx500wdt_start(wdt_dev);

	return 0;
}

static uint32_t grx500wdt_get_timeleft(struct watchdog_device *wdt_dev)
{
        uint32_t count0, initial0, config0;

	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_WD_INITIAL0), initial0);
	GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_WD_CONFIG0), config0);
        GICREAD(GIC_REG(VPE_LOCAL, GIC_VPE_WD_COUNT0), count0);

	printk("%s cpu = %d , initial0 = %x, count0 = %x, config0 = %x timeleftsecs = %d timeout = %d , timeleftsecs_clk = %d \n", __func__, smp_processor_id(), initial0, count0, config0, count0 / ltq_grx500_cpu_hz(), wdt_dev->timeout, count0 / cpu_clk);

        return (count0 / cpu_clk);
}

static int grx500wdt_ping(struct watchdog_device *wdt_dev)
{
	/*ping from anothe cpu is allowed only in case if its coming from wq,
		for user space call detect it and throw panic
		For wq it can get scheduled on any CPU and its not a good idea to set affinity
		for WQ as it looses the granularity of increasing the timeout when the 
		CPU is really stuck*/
	if(smp_processor_id() != wdt_dev->id){
		if(wdt_dev->wd_data->user_call){
			printk(KERN_ERR "User space ping received from cpu[%d] but  expected from cpu [%d] \n",
							smp_processor_id(),wdt_dev->id);
			panic("Please set affinity of this process to cpu %d\n",wdt_dev->id);
		}else{/*has to work upon another cpu in case of wq from another cpu*/
   		grx500wdt_stop_other(wdt_dev);
  		grx500wdt_start_other(wdt_dev);
		}
	}else{/*same cpu*/
   	grx500wdt_stop(wdt_dev);
  	grx500wdt_start(wdt_dev);
	}
	/*reset the user space call flag irrespective of WQ/user call*/
	wdt_dev->wd_data->user_call = 0;
	return 0;
}

static irqreturn_t grx500wdt_irq(int irqno, void *param)
{
	struct watchdog_device *grx500_wdt;
	grx500_wdt = &per_cpu(grx500wdt, smp_processor_id());
	/*stop and start the timer to have longer duration after pre warning ISR*/	
	grx500wdt_stop(grx500_wdt);
	WARN_ONCE(1, " IRQ %d triggered as WDT%d Timer Overflow on CPU %d !!!.. \n", irqno, grx500_wdt->id, smp_processor_id());
	grx500wdt_start(grx500_wdt);

  return IRQ_HANDLED;
}

struct irqaction grx500wdt_irqaction = {
        .handler = grx500wdt_irq,
        .flags = IRQF_PERCPU | IRQF_NO_SUSPEND,
        .name = "watchdog",
};

static const struct watchdog_info grx500wdt_info = {
        .options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
        .identity = "Hardware Watchdog for GRX500",
};

static const struct watchdog_ops grx500wdt_ops = {
        .owner = THIS_MODULE,
        .start = grx500wdt_start,
        .stop = grx500wdt_stop,
        .ping = grx500wdt_ping,
        .set_timeout = grx500wdt_set_timeout,
        .get_timeleft = grx500wdt_get_timeleft,
};

static int grx500wdt_probe(struct platform_device *pdev)
{
	struct resource *wdt_irq;
	struct clk *clk;
	int ret, cpu, irq;
	struct device_node *node = pdev->dev.of_node;
	int pirq = MIPS_GIC_LOCAL_IRQ_BASE + GIC_LOCAL_TO_HWIRQ(GIC_LOCAL_INT_WD);

	/* 
	   I/O memory need not be taken from Device Tree as WDT is part of GIC and can be accessed
	   through GIC_[READ/WRITE] and VPE_LOCAL APIs 
	*/

        wdt_irq = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
        if (wdt_irq == NULL) {
		irq = pirq; 
		printk("no irq resource specified so setting to default irq = %d\n", irq);
        } else {
		printk("irq= %d\n", wdt_irq->start);
		irq = wdt_irq->start;	
	}

	WARN_ONCE((irq != pirq),"Wrong WDT Local IRQ %d configured in DT !!. WDT Local IRQ is %d \n", irq, pirq);

	/* set up per-cpu IRQ */
	setup_percpu_irq(irq, &grx500wdt_irqaction);

	/* Get the Clock frequency */
	clk = clk_get_sys("cpu", "cpu");
	if (clk == NULL) {
                pr_err("CPU clock structure not found\n");
		ret = -1;
                goto exit;
        }

	cpu_clk = clk_get_rate(clk);

	for_each_possible_cpu(cpu) {
		struct watchdog_device *grx500_wdt;
		grx500_wdt = &per_cpu(grx500wdt, cpu);

		grx500_wdt->id = cpu;
		grx500_wdt->info = &grx500wdt_info;
		grx500_wdt->ops = &grx500wdt_ops; 
		/*we dont want kernel to override this based on no of cpus present
			as we want to detect the cpu from the id rather than other means
			setting this bit will ensure ID is not changed
			w/o this the behaviour is 
			core0 Linux - WDT0 
			core1 legacy- ?
			core2 Linux -WDT1 instead of WDT2, as kernel is allocating in sequence
			this check will avoid that */
		set_bit(WDOG_ID_NOT_REQ, &grx500_wdt->status);
	}
	
	/* Enable WDT reset to RCU for VPEx */  
	__raw_writel(RCU_WDTx_RESET, rcu_membase + RCU_IAP_WDT_RST_EN);
	wmb();

	for_each_online_cpu(cpu) {
		uint32_t config0;
		struct irq_desc *desc;
		struct watchdog_device *grx500_wdt;
		grx500_wdt = &per_cpu(grx500wdt, cpu);
		
		printk("cpu = %d cpu_clock (ltq_grx500_cpu_hz) = %ld cpu_clk = %ld \n", cpu, ltq_grx500_cpu_hz(), cpu_clk);
		printk("grx500_wdt->id = %d\n", grx500_wdt->id);

        	grx500_wdt->min_timeout = 1;
	        grx500_wdt->max_timeout = (0xffffffff / cpu_clk);
					/*filling in heartbeat for WQ to kick in in case if its enabled*/
					grx500_wdt->max_hw_heartbeat_ms = grx500_wdt->max_timeout *1000;
					grx500_wdt->min_hw_heartbeat_ms = grx500_wdt->min_timeout *1000;

		watchdog_init_timeout(grx500_wdt, timeout , &pdev->dev);
	        watchdog_set_nowayout(grx500_wdt, nowayout);

		GICWRITE(GIC_REG(VPE_LOCAL, GIC_VPE_OTHER_ADDR), cpu);
		wmb();	
        	GICWRITE(GIC_REG(VPE_OTHER, GIC_VPE_WD_CONFIG0), 0x0); /* Reset CONFIG0 to 0x0 */
        	wmb();
        	GICREAD(GIC_REG(VPE_OTHER, GIC_VPE_WD_CONFIG0), config0);
        	rmb();
        	GICWRITE(GIC_REG(VPE_OTHER, GIC_VPE_WD_CONFIG0), (config0 | WD_TYPE_SCD | WD_NWAIT));
        	wmb();
		GICWRITE(GIC_REG(VPE_OTHER, GIC_VPE_WD_INITIAL0), (cpu_clk * (grx500_wdt->timeout)));
		wmb();

		/* Map to PIN is configured by GIC */

		/* 
		   Use enable_percpu_irq() for each Online CPU.
		*/

		preempt_disable();
		if (cpu != smp_processor_id())
			smp_call_function_single(cpu, (smp_call_func_t)Enable_WDT_intr, (void *)&irq, 1);
		else
			Enable_WDT_intr((void *)&irq);
		preempt_enable();

		printk ("[%s]:[%d] grx500_wdt = %p \n", __FUNCTION__, __LINE__, grx500_wdt);
	
		ret = watchdog_register_device(grx500_wdt);
		if (ret) {
			printk ("[%s]:[%d] ret = %d \n", __FUNCTION__, __LINE__, ret);	
			goto exit;
		}
	}

	pr_info("Hardware Watchdog Timer: (max %ld) (nowayout= %d)\n", (0xffffffff / cpu_clk), nowayout);

	return 0;

exit:
	return ret;
}

static int __exit grx500wdt_remove(struct platform_device *dev)
{
	int cpu;

	for_each_online_cpu(cpu) {
		struct watchdog_device *grx500_wdt;
		
		grx500_wdt = &per_cpu(grx500wdt, cpu);
		grx500_wdt->id = cpu;
                printk("cpu = %d\n", cpu);
		watchdog_unregister_device(grx500_wdt);
	}
	return 0;
}

static void grx500wdt_shutdown(struct platform_device *dev)
{
	struct watchdog_device *grx500_wdt;
	
	grx500_wdt = &per_cpu(grx500wdt, smp_processor_id());

	grx500wdt_stop(grx500_wdt);
}

static const struct of_device_id grx500wdt_match[] = {
        { .compatible = "lantiq,grx500wdt" },
        {},
};

static struct platform_driver grx500wdt_driver = {
	.probe = grx500wdt_probe,
	.remove = __exit_p(grx500wdt_remove),
	.shutdown = grx500wdt_shutdown,
	.driver = {
		.name = "grx500wdt",
		.owner = THIS_MODULE,
		.of_match_table = grx500wdt_match,
	},
};

module_platform_driver(grx500wdt_driver);

MODULE_DESCRIPTION("GRX500 Watchdog Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:grx500wdt");
