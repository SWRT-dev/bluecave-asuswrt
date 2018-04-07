/*
 *  Copyright (C) 2011 John Crispin <blogic@openwrt.org>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/rt2x00_platform.h>

extern int (*ltq_pci_plat_dev_init)(struct pci_dev *dev);
static struct rt2x00_platform_data rt2x00_pdata;

static int rt2x00_pci_plat_dev_init(struct pci_dev *dev)
{
	dev->dev.platform_data = &rt2x00_pdata;
	return 0;
}

int __init of_ralink_eeprom_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node;
	const char *eeprom;

	if (of_property_read_string(np, "ralink,eeprom", &eeprom)) {
		dev_err(&pdev->dev, "failed to load eeprom filename\n");
		return 0;
	}

	rt2x00_pdata.eeprom_file_name = kstrdup(eeprom, GFP_KERNEL);
//	rt2x00_pdata.mac_address = mac;
	ltq_pci_plat_dev_init = rt2x00_pci_plat_dev_init;

	dev_info(&pdev->dev, "using %s as eeprom\n", eeprom);

	return 0;
}

static struct of_device_id ralink_eeprom_ids[] = {
	{ .compatible = "ralink,eeprom" },
	{ }
};

static struct platform_driver ralink_eeprom_driver = {
	.driver		= {
		.name		= "ralink,eeprom",
		.owner	= THIS_MODULE,
		.of_match_table	= of_match_ptr(ralink_eeprom_ids),
	},
};

static int __init of_ralink_eeprom_init(void)
{
	return platform_driver_probe(&ralink_eeprom_driver, of_ralink_eeprom_probe);
}
device_initcall(of_ralink_eeprom_init);
