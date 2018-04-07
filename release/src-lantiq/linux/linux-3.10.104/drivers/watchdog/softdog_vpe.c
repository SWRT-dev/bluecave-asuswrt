/*
** =============================================================================
** FILE NAME     : softdog_vpe.c
** MODULES       : LXDB
** DATE          : 24-03-2008
** AUTHOR        : LXDB Team
** DESCRIPTION   : This header file contains the code for the watchdog
**                 implentation on vpe1 side.
** REFERENCES    :
** COPYRIGHT     : Copyright (c) 2008
**                 Am Campeon 1-12, 85579 Neubiberg, Germany
** Any use of this software is subject to the conclusion of a respective
** License agreement. Without such a License agreement no rights to the
** software are granted
**
** HISTORY       :
** $Date   $Author    $Comment
** 24-03-2008   LXDB    Initial version
** ============================================================================
*/

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <asm/ltq_vpe.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/jiffies.h>

static uint32_t last_wdog_value;
static uint32_t vpe1_wdog_cleared;

static uint32_t vpe1_wdog_dead;
void watchdog_vpe0_fire(unsigned long); /* Called when vpe0 timer expires */
static void keep_alive_vpe0(uint32_t);
VPE_SW_WDOG_RESET reset_local_fn;


static struct timer_list watchdog_vpe0_ticktock =
                TIMER_INITIALIZER(watchdog_vpe0_fire, 0, 0);

void watchdog_vpe0_fire (unsigned long flags) 
{
	volatile uint32_t *wdog_ctr_value;
	wdog_ctr_value = (uint32_t *) vpe1_wdog_ctr;
	if (*wdog_ctr_value == last_wdog_value) { /* VPE1 watchdog expiry handling */
		vpe1_sw_wdog_stop(flags);
		vpe1_wdog_dead++;
		printk(KERN_DEBUG "VPE1 watchdog reset handler called\n");
	/* Call the reset handler function */
		reset_local_fn(flags);
	} else { /* Everything is OK on vpe1 side. Continue. */
		last_wdog_value = *wdog_ctr_value;
		vpe1_wdog_cleared++;
		keep_alive_vpe0(flags);
	}
}

int32_t vpe1_sw_wdog_register_reset_handler (VPE_SW_WDOG_RESET reset_fn) 
{
	reset_local_fn = (VPE_SW_WDOG_RESET)reset_fn;
	return 0;
}

static void keep_alive_vpe0(uint32_t flags)
{
	mod_timer(&watchdog_vpe0_ticktock, jiffies+ vpe1_wdog_timeout );	
}

uint32_t vpe1_sw_wdog_start(uint32_t flags)
{
	volatile uint32_t *wdog_ctr_value;
	wdog_ctr_value = (uint32_t *) vpe1_wdog_ctr;
	*wdog_ctr_value = 0;
	last_wdog_value = 0;
	keep_alive_vpe0(flags);
	return 0;
}

uint32_t vpe1_sw_wdog_stop(uint32_t flags) 
{
	del_timer(&watchdog_vpe0_ticktock);
	return 0;
}

static int __init watchdog_vpe1_init(void) 
{
	/* Nothing to be done here */
	return 0;
}

static void __exit watchdog_vpe1_exit(void) 
{
	uint32_t flags = 0;
	vpe1_sw_wdog_stop(flags);
}

module_init(watchdog_vpe1_init);
module_exit(watchdog_vpe1_exit);

EXPORT_SYMBOL(vpe1_sw_wdog_register_reset_handler);
EXPORT_SYMBOL(vpe1_sw_wdog_start);
EXPORT_SYMBOL(vpe1_sw_wdog_stop);

MODULE_AUTHOR("LXDB");
MODULE_DESCRIPTION("Software Watchdog For VPE1");
MODULE_LICENSE("GPL");
