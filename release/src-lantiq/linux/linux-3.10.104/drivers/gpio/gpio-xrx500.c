/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2012 Thomas Langer <thomas.langer@lantiq.com>
 *  Copyright (C) 2012 John Crispin <blogic@openwrt.org>
 *  Copyright (C) 2014 Kavitha  Subramanian <s.kavitha.EE@lantiq.com>
 */

#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/export.h>
#include <linux/err.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/pinctrl/pinctrl.h>
#include <linux/pinctrl/consumer.h>
#include <linux/platform_device.h>

#include <lantiq_soc.h>
#include <linux/klogging.h>

#define PORTS			2
#define PINS			32
#define PORT(x)                 (x / PINS)
#define PORT_PIN(x)             (x % PINS)

/* ---------  gpio_chip related code --------- */

/* these are the offsets to our registers */
/* Data Output Register */
#define GPIO_OUT		 0x00000000
/* Data Input Register */
#define GPIO_IN			0x00000004
/* Direction Register */
#define GPIO_DIR		0x00000008
/* External Interrupt Control Register 0 */
#define GPIO_EXINTCR0		0x00000018
/* External Interrupt Control Register 1 */
#define GPIO_EXINTCR1		0x0000001C
/* IRN Capture Register */
#define GPIO_IRNCR		0x00000020
/* IRN Interrupt Control Register 0 */
#define GPIO_IRNICR		0x00000024
/* IRN Interrupt Enable Register 0 */
#define GPIO_IRNEN		0x00000028
/* IRN Interrupt Configuration Register */
#define GPIO_IRNCFG		0x0000002C
/* IRN Interrupt Enable Set Register */
#define GPIO_IRNRNSET		0x00000030
/* IRN Interrupt Enable Clear Register */
#define GPIO_IRNENCLR		0x00000034
/* Output Set Register */
#define GPIO_OUTSET		0x00000040
/* Output Cler Register */
#define GPIO_OUTCLR		0x00000044
/* Direction Clear Register */
#define GPIO_DIRSET		0x00000048
/* Direction Set Register */
#define GPIO_DIRCLR		0x0000004C



/* turn a gpio_chip into a xrx500_gpio_port */
#define ctop(c)		container_of(c, struct xrx500_gpio_port, gpio_chip)
/* turn a irq_data into a xrx500_gpio_port */
#define itop(i)		((struct xrx500_gpio_port *) irq_get_chip_data(i->irq))

#define port_r32(p, reg)	ltq_r32(p->port + reg)
#define port_w32(p, val, reg)	ltq_w32(val, p->port + reg)
#define port_w32_mask(p, clear, set, reg) \
		port_w32(p, (port_r32(p, reg) & ~(clear)) | (set), reg)

#define MAX_BANKS		5
#define PINS_PER_PORT		32

struct xrx500_gpio_port {
	struct gpio_chip gpio_chip;
	void __iomem *port;
	unsigned int irq_base;
	unsigned int chained_irq;
	struct clk *clk;
	char name[6];
};

static struct irq_chip xrx500_gpio_irq_chip;

static int xrx500_gpio_direction_input(struct gpio_chip *chip,
					unsigned int offset)
{
	port_w32(ctop(chip), 1 << offset, GPIO_DIRCLR);

	return 0;
}

static void xrx500_gpio_set(struct gpio_chip *chip, unsigned int offset,
					int value)
{
	LOGF_KLOG_DEBUG("%s called with pin: %d and value:%d\n", __func__, offset, value);

	if (value)
		port_w32(ctop(chip), 1 << offset, GPIO_OUTSET);
	else
		port_w32(ctop(chip), 1 << offset, GPIO_OUTCLR);
}

static int xrx500_gpio_direction_output(struct gpio_chip *chip,
					unsigned int offset, int value)
{
	LOGF_KLOG_DEBUG("%s called with pin: %d\n", __func__, offset);
	xrx500_gpio_set(chip, offset, value);
	port_w32(ctop(chip), 1 << offset, GPIO_DIRSET);

	return 0;
}

static int xrx500_gpio_get(struct gpio_chip *chip, unsigned int offset)
{
	if ((port_r32(ctop(chip), GPIO_DIR) >> offset) & 1)
		return (port_r32(ctop(chip), GPIO_OUT) >> offset) & 1;
	else
		return (port_r32(ctop(chip), GPIO_IN) >> offset) & 1;
}

static int xrx500_gpio_request(struct gpio_chip *chip, unsigned offset)
{
	int gpio = chip->base + offset;

	LOGF_KLOG_DEBUG("%s called with pin: %d\n", __func__, offset);
	return pinctrl_request_gpio(gpio);
}

static void xrx500_gpio_free(struct gpio_chip *chip, unsigned offset)
{
	int gpio = chip->base + offset;

	pinctrl_free_gpio(gpio);
}

static int xrx500_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	return ctop(chip)->irq_base + offset;
}

static void xrx500_gpio_disable_irq(struct irq_data *d)
{
	unsigned int offset = d->irq - itop(d)->irq_base;

	port_w32(itop(d), 1 << offset, GPIO_IRNENCLR);
}

static void xrx500_gpio_enable_irq(struct irq_data *d)
{
	unsigned int offset = d->irq - itop(d)->irq_base;

	port_w32(itop(d), 1 << offset, GPIO_IRNRNSET);
}

static void xrx500_gpio_ack_irq(struct irq_data *d)
{
	unsigned int offset = d->irq - itop(d)->irq_base;

	port_w32(itop(d), 1 << offset, GPIO_IRNCR);
}

static void xrx500_gpio_mask_and_ack_irq(struct irq_data *d)
{
	unsigned int offset = d->irq - itop(d)->irq_base;

	port_w32(itop(d), 1 << offset, GPIO_IRNENCLR);
	port_w32(itop(d), 1 << offset, GPIO_IRNCR);
}

static int xrx500_gpio_irq_type(struct irq_data *d, unsigned int type)
{
	unsigned int offset = d->irq - itop(d)->irq_base;
	unsigned int mask = 1 << offset;

	if ((type & IRQ_TYPE_SENSE_MASK) == IRQ_TYPE_NONE)
		return 0;

	if ((type & (IRQ_TYPE_LEVEL_HIGH | IRQ_TYPE_LEVEL_LOW)) != 0) {
		/* level triggered */
		port_w32_mask(itop(d), 0, mask, GPIO_IRNCFG);
		__irq_set_handler_locked(d->irq, handle_level_irq);
	} else {
		/* edge triggered */
		port_w32_mask(itop(d), mask, 0, GPIO_IRNCFG);
		__irq_set_handler_locked(d->irq, handle_simple_irq);
	}

	if ((type & IRQ_TYPE_EDGE_BOTH) == IRQ_TYPE_EDGE_BOTH) {
		port_w32_mask(itop(d), mask, 0, GPIO_EXINTCR0);
		port_w32_mask(itop(d), 0, mask, GPIO_EXINTCR1);
	} else {
		if ((type & (IRQ_TYPE_EDGE_RISING | IRQ_TYPE_LEVEL_HIGH)) != 0)
			/* positive logic: rising edge, high level */
			port_w32_mask(itop(d), mask, 0, GPIO_EXINTCR0);
		else
			/* negative logic: falling edge, low level */
			port_w32_mask(itop(d), 0, mask, GPIO_EXINTCR0);
		port_w32_mask(itop(d), mask, 0, GPIO_EXINTCR1);
	}
	return xrx500_gpio_direction_input(&itop(d)->gpio_chip, offset);
}

static void xrx500_gpio_irq_handler(unsigned int irq, struct irq_desc *desc)
{
	struct xrx500_gpio_port *gpio_port = irq_desc_get_handler_data(desc);
	unsigned long irncr;
	int offset;

	/* acknowledge interrupt */
	irncr = port_r32(gpio_port, GPIO_IRNCR);
	port_w32(gpio_port, irncr, GPIO_IRNCR);

	/*desc->irq_data.chip->irq_ack(&desc->irq_data);*/

	for_each_set_bit(offset, &irncr, gpio_port->gpio_chip.ngpio)
		generic_handle_irq(gpio_port->irq_base + offset);
}

static int xrx500_gpio_irq_map(struct irq_domain *d, unsigned int irq,
				irq_hw_number_t hw)
{
	struct xrx500_gpio_port *port = d->host_data;

	irq_set_chip_and_handler_name(irq, &xrx500_gpio_irq_chip,
			handle_level_irq, "mux");
	irq_set_chip_data(irq, port);

	/* set to negative logic (falling edge, low level) */
	port_w32_mask(port, 0, 1 << hw, GPIO_EXINTCR0);
	/* set to single edge */
	port_w32_mask(port, 1 << hw, 0, GPIO_EXINTCR1);
	return 0;
}

static unsigned int xrx500_gpio_irq_startup(struct irq_data *d)
 {
 		unsigned int offset = d->irq - itop(d)->irq_base;
 		xrx500_gpio_request(&itop(d)->gpio_chip, offset);
		xrx500_gpio_direction_input(&itop(d)->gpio_chip, offset);
		xrx500_gpio_enable_irq(d);
		return 0;
 }
static struct irq_chip xrx500_gpio_irq_chip = {
	.name = "gpio_irq",
	.irq_mask = xrx500_gpio_disable_irq,
	.irq_unmask = xrx500_gpio_enable_irq,
	.irq_ack = xrx500_gpio_ack_irq,
	.irq_mask_ack = xrx500_gpio_mask_and_ack_irq,
	.irq_set_type = xrx500_gpio_irq_type,
	.irq_startup = xrx500_gpio_irq_startup,
};

static const struct irq_domain_ops irq_domain_ops = {
	.xlate = irq_domain_xlate_onetwocell,
	.map = xrx500_gpio_irq_map,
};

static struct irqaction gpio_cascade = {
	.handler = no_action,
	.flags = IRQF_DISABLED,
	.name = "gpio_cascade",
};

static int xrx500_gpio_probe(struct platform_device *pdev)
{
	struct pinctrl_gpio_range *gpio_range;
	struct device_node *node = pdev->dev.of_node, *child;
	const __be32 *bank = of_get_property(node, "lantiq,bank", NULL);
	struct xrx500_gpio_port *gpio_port;
	struct resource *gpiores, irqres;
	int ret, size, count;
/*	static int sso_init_done = 0;*/
	unsigned int gpio_num, offset;

	if (!bank || *bank >= MAX_BANKS)
		return -ENODEV;

	size = pinctrl_xrx500_get_range_size(*bank);
	if (size < 1) {
		LOGF_KLOG_DEV_ERROR(&pdev->dev, "pad not loaded for bank %d\n", *bank);
		return size;
	}

	gpio_range = devm_kzalloc(&pdev->dev, sizeof(struct pinctrl_gpio_range),
				GFP_KERNEL);
	if (!gpio_range)
		return -ENOMEM;

	gpio_port = devm_kzalloc(&pdev->dev, sizeof(struct xrx500_gpio_port),
				GFP_KERNEL);
	if (!gpio_port)
		return -ENOMEM;

	snprintf(gpio_port->name, 6, "gpio%d", *bank);
	gpio_port->gpio_chip.label = gpio_port->name;
	gpio_port->gpio_chip.direction_input = xrx500_gpio_direction_input;
	gpio_port->gpio_chip.direction_output = xrx500_gpio_direction_output;
	gpio_port->gpio_chip.get = xrx500_gpio_get;
	gpio_port->gpio_chip.set = xrx500_gpio_set;
	gpio_port->gpio_chip.request = xrx500_gpio_request;
	gpio_port->gpio_chip.free = xrx500_gpio_free;
	gpio_port->gpio_chip.base = *bank * PINS_PER_PORT;
	gpio_port->gpio_chip.ngpio = size;
	gpio_port->gpio_chip.dev = &pdev->dev;

	gpiores = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	gpio_port->port = devm_request_and_ioremap(&pdev->dev, gpiores);
	if (IS_ERR(gpio_port->port))
		return PTR_ERR(gpio_port->port);

#if 0
	gpio_port->clk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(gpio_port->clk))
		return PTR_ERR(gpio_port->clk);
	clk_activate(gpio_port->clk);
#endif
	if (of_irq_to_resource_table(node, &irqres, 1) == 1) {
		gpio_port->irq_base = GPIO0_IRQ_BASE + (32 * *bank);
		gpio_port->gpio_chip.to_irq = xrx500_gpio_to_irq;
		gpio_port->chained_irq = irqres.start;
		irq_domain_add_legacy(node, size, gpio_port->irq_base, 0,
					&irq_domain_ops, gpio_port);
		setup_irq(irqres.start, &gpio_cascade);
		irq_set_handler_data(irqres.start, gpio_port);
		irq_set_chained_handler(irqres.start, xrx500_gpio_irq_handler);
	}
	LOGF_KLOG_DEBUG("%s: gpiochip_add with : %s\n", __func__, gpio_port->name);
	ret = gpiochip_add(&gpio_port->gpio_chip);
	if (ret)
		return ret;

	platform_set_drvdata(pdev, gpio_port);

	gpio_range->name = "xrx500 GPIO";
	gpio_range->id = *bank;
	gpio_range->base = gpio_port->gpio_chip.base;
	gpio_range->pin_base = gpio_port->gpio_chip.base;
	gpio_range->npins = gpio_port->gpio_chip.ngpio;
	gpio_range->gc = &gpio_port->gpio_chip;

	pinctrl_xrx500_add_gpio_range(gpio_range);

#if 0
	if (!sso_init_done) {
		xrx500_gpio_direction_output(&gpio_port->gpio_chip, 4, 0);
		xrx500_gpio_direction_output(&gpio_port->gpio_chip, 5, 0);
		xrx500_gpio_direction_output(&gpio_port->gpio_chip, 6, 0);
		sso_init_done++;
	}
#else
	count = of_get_child_count(node);
	if (count) {
		int output = -1;
		struct property *prop;
		const __be32 *p;
		LOGF_KLOG_DEBUG("node count %d\r\n", count);
		for_each_child_of_node(node, child) {
			if (of_property_read_bool(child, "hog-as-input"))
				output = 0;
			else if (of_property_read_bool(child, "hog-as-output"))
				output = 1;

			of_property_for_each_u32(child, "gpio", prop, p, gpio_num) {
				offset = gpio_num - gpio_port->gpio_chip.base;
				if (offset >= PINS_PER_PORT) {
					dev_err(&pdev->dev, "gpio: %u is out of the gpio chip range:(%u, %u)\n",
						gpio_num, gpio_port->gpio_chip.base,
						gpio_port->gpio_chip.base + PINS_PER_PORT);
					continue;
				}
				if (output == 1){
					if(gpio_num==6){
						xrx500_gpio_direction_output(&gpio_port->gpio_chip, offset, 1);
					}else if(gpio_num==4){
						xrx500_gpio_direction_output(&gpio_port->gpio_chip, offset, 0);
					}else if(gpio_num==1 || gpio_num==8 ||gpio_num==42){
						xrx500_gpio_direction_output(&gpio_port->gpio_chip, offset, 0);
						xrx500_gpio_direction_output(&gpio_port->gpio_chip, 1 /* gpio1 */ - gpio_port->gpio_chip.base, 0);
					}else{
						xrx500_gpio_direction_output(&gpio_port->gpio_chip, offset, 0);
					}
				}else if (output == 0){
					xrx500_gpio_direction_input(&gpio_port->gpio_chip, offset);
				}
			}
		}
	}

#endif

	return 0;
}

static const struct of_device_id xrx500_gpio_match[] = {
	{ .compatible = "lantiq,xrx500-gpio" },
	{ .compatible = "lantiq,gpio-xrx500" },
	{},
};
MODULE_DEVICE_TABLE(of, xrx500_gpio_match);

static struct platform_driver xrx500_gpio_driver = {
	.probe = xrx500_gpio_probe,
	.driver = {
	.name = "gpio-xrx500",
	.owner = THIS_MODULE,
	.of_match_table = xrx500_gpio_match,
	},
};

int __init xrx500_gpio_init(void)
{
	int ret;

	LOGF_KLOG_INFO("XRX500 GPIO Driver, (C) 2014 Lantiq Deutschland Gmbh\n");
	ret = platform_driver_register(&xrx500_gpio_driver);
	if (ret)
		LOGF_KLOG_ERROR("xrx500_gpio: Error registering platform driver!");
	return ret;
}

core_initcall_sync(xrx500_gpio_init);
