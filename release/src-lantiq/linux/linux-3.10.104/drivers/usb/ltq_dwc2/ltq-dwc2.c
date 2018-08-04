/*
 * Copyright (C) 2017 LANTIQ Semiconductor Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 */

#include "ltq-dwc2.h"

int ltq_dwc2_ctrl_clock(struct platform_device *_dev, int usb_id, int on)
{
	struct clk *clk = NULL;

	dev_dbg(&_dev->dev, "ltq_dwc2_ctrl_clock, usbid %d, on %d\n", usb_id, on);

	if (usb_id == 0)
		clk = clk_get(&_dev->dev, "ctl");
	else
		clk = clk_get(&_dev->dev, "ctl");
	
	if (IS_ERR(clk)) {
		dev_err(&_dev->dev, "%s:Failed to get ctrl clock\n", __func__);
		return -1;
	}

	if (on)
		clk_enable(clk);
	else
		clk_disable(clk);
	
	mdelay(50);

	return 0;
}

int ltq_dwc2_phy_clock(struct platform_device *_dev, int usb_id, int on)
{
	struct clk *clk = NULL;

	dev_dbg(&_dev->dev, "ltq_dwc2_phy_clock, usbid %d, on %d\n", usb_id, on);

	if (usb_id == 0)
		clk = clk_get(&_dev->dev, "phy");
	else
		clk = clk_get(&_dev->dev, "phy");
	
	if (IS_ERR(clk)) {
		dev_err(&_dev->dev, "%s:Failed to get phy clock\n", __func__);
		return -1;
	}

	if (on)
		clk_enable(clk);
	else
		clk_disable(clk);
	
	mdelay(50);

	return 0;
}

int ltq_dwc2_rcu_config(struct platform_device *_dev, int usb_id)
{
	dev_dbg(&_dev->dev, "ltq_dwc2_rcu_config, usbid %d\n", usb_id);

	ltq_usb_config(usb_id, 1);

	return 0;
}

int ltq_dwc2_rcu_deconfig(struct platform_device *_dev, int usb_id)
{
	dev_dbg(&_dev->dev, "ltq_dwc2_rcu_config, usbid %d\n", usb_id);

	ltq_usb_config(usb_id, 0);

	return 0;
}

int ltq_dwc2_vbus_init(struct platform_device *_dev, int gpio)
{
	int err;

	dev_dbg(&_dev->dev, "ltq_dwc2_vbus_init, gpio %d\n", gpio);

	err = gpio_is_valid(gpio);
	if (!err)
	{
		dev_dbg(&_dev->dev, "%s: gpio pin %d is invalid\n", __func__, gpio);
		return -EINVAL;
	}

	err = gpio_request(gpio, "dwc2_vbus");
	if (err)
	{
		dev_dbg(&_dev->dev, "%s: can't request gpio %d\n", __func__, gpio);
		return -EINVAL;
	}
	gpio_direction_output(gpio, 1);

	return 0;
}

int ltq_dwc2_vbus_deinit(struct platform_device *_dev, int gpio)
{
	dev_dbg(&_dev->dev, "ltq_dwc2_vbus_deinit, gpio %d\n", gpio);

	gpio_free(gpio);

	return 0;
}
