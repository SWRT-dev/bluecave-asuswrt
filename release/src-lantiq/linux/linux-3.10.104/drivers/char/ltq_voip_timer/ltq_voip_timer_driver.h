/******************************************************************************

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.

*******************************************************************************/
#ifndef _VOIP_TIMER_DRIVER_H_
#define _VOIP_TIMER_DRIVER_H_

#define TIMER_MAX_PROCESS 10
#define TIMER_DRV_MAJOR             229
#define TIMER_DRV_MINOR_0           0
#define TIMER_DRV_MINOR_1           1
#define TIMER_DRV_MODE_CHANGE _IOR(TIMER_DRV_MAJOR, 0, char *)

#define TIMER_DEVICE_NAME "voip_timer_driver"

#define EXPIRE_TIME_DEVIDE  (1000/timer_interval)
#define EXPIRE_TIME_MULTY   (HZ*timer_interval/1000)

struct Timer_CntrlBlock{
   struct timer_list timer;
   struct task_struct *pxtask;
   wait_queue_head_t Timer_WakeupList;
   int Timer_excpFlag;
 }Timer_CntrlBlock;

#endif /* _VOIP_TIMER_DRIVER_H_ */

