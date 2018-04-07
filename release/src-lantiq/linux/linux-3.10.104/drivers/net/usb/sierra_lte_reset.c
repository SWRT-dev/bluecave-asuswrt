/******************************************************************************
**
** FILE NAME    : sw_reset.c
** PROJECT      : UGW-6.x
** MODULES      : GPIO for sw reset
**
** DATE         : 16 May 2014
** AUTHOR       :
** DESCRIPTION  : SW Reset Push Button Driver
** COPYRIGHT    : Copyright (c) 2014
**                Lantiq Communications
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date        $Author         $Comment
** 16 May 2014
*******************************************************************************/

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/interrupt.h>
#include <linux/major.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/fcntl.h>
#include <linux/ptrace.h>
#include <linux/mm.h>
#include <linux/ioport.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <lantiq_soc.h>
#include <lantiq_ssc.h>
#include <lantiq_irq.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>

#define DEBUG  printk

struct timer_list pwr_on_timer;
unsigned int pwr_on_sierra_lte=0;
int lte_pwr_on_ngpio=-1;

static void pwr_on_timer_func(unsigned long arg)
{
	if(pwr_on_sierra_lte== 0) //init
	{
		printk(KERN_DEBUG "******************** %s: pull GPIO %d (output 1)low for 2 seconds. ********************\n", __func__, lte_pwr_on_ngpio);
		gpio_direction_output( lte_pwr_on_ngpio, 1); //pull low for 2000ms, output 1 to pull low POWER_ON pin
		pwr_on_sierra_lte = 1;
		mod_timer(&pwr_on_timer, jiffies+ HZ*2); 
	} else {
		printk(KERN_DEBUG "******************* %s: power on done, pull high (output 0) GPIO %d again and del itself   ********************\n", __func__, lte_pwr_on_ngpio);
		gpio_direction_output( lte_pwr_on_ngpio, 0); //pull high back, output 0 to pull high
		del_timer(&pwr_on_timer);
	}
}


int ltq_swreset_init(void)
{

	int ret=0;

	if(pwr_on_sierra_lte==0){
		printk("[SHLEE] bring up sierra lte module in 10 seconds after when USB core ready........\n");
		ret = gpio_request(lte_pwr_on_ngpio, "LTE_PWR_ON");
		if(ret<0)
			printk("%s: Failed to request GPIO %d as LTE_PWR_ON PIN and return with %d\n", __func__,lte_pwr_on_ngpio, ret);
		else {
//		gpio_direction_output(lte_pwr_on_ngpio, 1);
		#if 1
			init_timer(&pwr_on_timer);
			pwr_on_timer.function = pwr_on_timer_func;
			pwr_on_timer.data = 1;
			pwr_on_timer.expires = jiffies + 10* HZ;
			add_timer(&pwr_on_timer);
		#endif
		}	
	}
  return 0;

}

void ltq_swreset_cleanup_module(void)
{

}

static int ltq_sierra_lte_reset_driver_probe(struct platform_device *pdev)
{
	
	struct device_node *node = pdev->dev.of_node;
	DEBUG ("ltq_sierra_lte_reset_driver_probe !!\n");

	lte_pwr_on_ngpio = of_get_named_gpio(node,"lte_pwr_on",0);
	if(!gpio_is_valid(lte_pwr_on_ngpio)){
		printk ("Could not read 'lte_pwr_on' from device tree!\n");
		return -ENODEV;
	}

	DEBUG ("ltq_sierra_lte_reset_driver: lte_pwr_on_ngpio = %u\n", lte_pwr_on_ngpio);

	ltq_swreset_init ();

	return 0;
}

static int ltq_sierra_lte_reset_driver_remove(struct platform_device *pdev)
{
	DEBUG ("ltq_swreset_driver_remove!\n");
	ltq_swreset_cleanup_module ();
	return 0;
}

static const struct of_device_id ltq_sierra_lte_match[] = {
	{ .compatible = "lantiq,sierra_lte" },
	{},
};

static struct platform_driver ltq_sierra_lte_reset_driver = {
	.probe = ltq_sierra_lte_reset_driver_probe,
	.remove = ltq_sierra_lte_reset_driver_remove,
	.driver = {
		.name = "lantiq,sierra_lte",
		.owner = THIS_MODULE,
		.of_match_table = ltq_sierra_lte_match,
	},
};

module_platform_driver(ltq_sierra_lte_reset_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LTQ Sierra HL7548 LTE Reset driver");
