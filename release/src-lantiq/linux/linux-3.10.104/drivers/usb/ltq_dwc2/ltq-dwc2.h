/*
 * Copyright (C) 2017 LANTIQ Semiconductor Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 */

#ifndef __LTQ_DWC2_H__
#define __LTQ_DWC2_H__
#include <linux/clk.h>
#include <linux/bitops.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <lantiq_soc.h>

#include "dwc_otg_os_dep.h"


int ltq_dwc2_ctrl_clock(struct platform_device *_dev, int usb_id, int on);
int ltq_dwc2_phy_clock(struct platform_device *_dev, int usb_id, int on);
int ltq_dwc2_rcu_config(struct platform_device *_dev, int usb_id);
int ltq_dwc2_rcu_deconfig(struct platform_device *_dev, int usb_id);
int ltq_dwc2_vbus_init(struct platform_device *_dev, int gpio);
int ltq_dwc2_vbus_deinit(struct platform_device *_dev, int gpio);
#endif

