/*
 * Copyright (C) 2015 LANTIQ Semiconductor Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/usb/otg.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/gpio.h>
#include <linux/of_address.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>

#define SECOND_TIME_DELAY 30

#define LTQ_OC_PORT_MUX(n)		((n) << 0)
#define LTQ_OC_PORT_MUX_MASK	LTQ_OC_PORT_MUX(7)

struct ltq_usb_oc {
	struct workqueue_struct *workq;
	struct delayed_work oc_recover_w;
	struct device	*dev;
	void __iomem	*base;
	unsigned int irqnum;
	int oc_gpio;
};

static void ltq_usb_oc_set_gpio_alt(int pin, int alt, void __iomem *base)
{
	u32 value;

	value = readl(base + (pin*4));
	value &= ~LTQ_OC_PORT_MUX_MASK;
	value |= alt;
	writel(value, base + (pin*4));

}

void ltq_usb_oc_recovery_worker(struct work_struct *work)  
{
	struct ltq_usb_oc *usb_oc = container_of(work, \
		struct ltq_usb_oc, oc_recover_w.work);
	printk("+%s: Turn on the power again to recover\n", __func__);
	/* The power need to be turned on again to recover*/
	ltq_usb_oc_set_gpio_alt(usb_oc->oc_gpio, 0, usb_oc->base);
	msleep(10);
	/* Enable OC protection again*/
	ltq_usb_oc_set_gpio_alt(usb_oc->oc_gpio, 3, usb_oc->base);
	msleep(10);
	
	irq_set_irq_type(usb_oc->irqnum, IRQ_TYPE_LEVEL_LOW);
}

static irqreturn_t ltq_usb_oc_irq (int irq, void * _usb_oc)
{
	struct ltq_usb_oc *usb_oc = _usb_oc;

	printk("+%s: USB OC protection is triggered\n", __func__);
	irq_set_irq_type(irq, IRQ_TYPE_EDGE_FALLING);
	/*ltq_usb_oc_clear_edge(irq);*/
	queue_delayed_work(usb_oc->workq, \
		&usb_oc->oc_recover_w, SECOND_TIME_DELAY*HZ);
	return IRQ_HANDLED;
}

static int ltq_usb_oc_probe(struct platform_device *pdev)
{
	const char *name;
	struct ltq_usb_oc *usb_oc;
	struct device *dev = &pdev->dev;
	void __iomem		*base;
	int ret, irq;
	int oc_gpio = 0;
	struct device_node	*pad_node, *oc_node = pdev->dev.of_node;

	usb_oc = devm_kzalloc(dev, sizeof(*usb_oc), GFP_KERNEL);
	if (!usb_oc)
		return -ENOMEM;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(dev, "missing IRQ resource\n");
		return -EINVAL;
	}

	if(of_property_read_u32(oc_node, "overcurrent-gpio", &oc_gpio)) {
		dev_err(dev, "missing pin-num OF property\n");
		return -EINVAL;
	}

	if(of_property_read_string(oc_node, "node-name", &name)) {
		dev_err(dev, "missing node-name OF property\n");
		return -EINVAL;
	}
	
	pad_node = of_find_node_by_name(NULL, name);
	if (!pad_node) {
		dev_err(dev, "Failed to find device-tree node: %s\n", name);
		return -ENODEV;
	}

	base = of_iomap(pad_node, 0);
	if (!base) {
		dev_err(dev, "of_iomap failed\n");
		return -ENOMEM;
	}	
	of_node_put(pad_node);

	usb_oc->dev = dev;
	usb_oc->irqnum = irq;
	usb_oc->base = base;
	usb_oc->oc_gpio = oc_gpio;
	platform_set_drvdata(pdev, usb_oc);

	/* Enable OC protection*/
	ltq_usb_oc_set_gpio_alt(oc_gpio, 3, base);
	msleep(10);

	usb_oc->workq = create_singlethread_workqueue("dwc3 usb oc wq");
	if (!usb_oc->workq) {
		dev_err(dev, "create_singlethread_workqueue() failed\n");
		return -ENOMEM;
	}
	INIT_DELAYED_WORK(&usb_oc->oc_recover_w, ltq_usb_oc_recovery_worker);

	ret = devm_request_irq(dev, irq, &ltq_usb_oc_irq, \
		IRQ_TYPE_LEVEL_LOW, "usb oc", usb_oc);
	if (ret) {
		dev_err(dev, "failed to request IRQ #%d --> %d\n",
			irq, ret);
		return ret;
	}

	return 0;
}

static int ltq_usb_oc_remove(struct platform_device *pdev)
{
	struct ltq_usb_oc *usb_oc = platform_get_drvdata(pdev);

	flush_workqueue(usb_oc->workq);
	destroy_workqueue(usb_oc->workq);
	ltq_usb_oc_set_gpio_alt(usb_oc->oc_gpio, 0, usb_oc->base);
	devm_free_irq(usb_oc->dev, usb_oc->irqnum, usb_oc);
	devm_kfree(usb_oc->dev, usb_oc);
	return 0;
}

static const struct of_device_id of_ltq_usb_oc_match[] = {
	{
		.compatible = "lantiq,usb-oc"
	},
	{ },
};
MODULE_DEVICE_TABLE(of, of_ltq_usb_oc_match);

static struct platform_driver ltq_usb_oc_driver = {
	.probe          = ltq_usb_oc_probe,
	.remove         = ltq_usb_oc_remove,
	.driver         = {
		.name   = "ltq-usb-oc",
		.owner  = THIS_MODULE,
		.pm = NULL,
		.of_match_table	= of_match_ptr(of_ltq_usb_oc_match),
	},
};

module_platform_driver(ltq_usb_oc_driver);
MODULE_LICENSE("GPL v2");
