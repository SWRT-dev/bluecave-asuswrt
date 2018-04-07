/*
 *  Copyright (C) 2011 Luca Olivetti <luca@ventoso.org>
 *  Copyright (C) 2011 John Crispin <blogic@openwrt.org>
 *  Copyright (C) 2011 Andrej Vlašić <andrej.vlasic0@gmail.com>
 *  Copyright (C) 2013 Álvaro Fernández Rojas <noltari@gmail.com>
 *  Copyright (C) 2013 Daniel Gimpelevich <daniel@gimpelevich.san-francisco.ca.us>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/etherdevice.h>
#include <linux/ath5k_platform.h>
#include <linux/ath9k_platform.h>
#include <linux/pci.h>
#include <linux/err.h>
#include <linux/mtd/mtd.h>
#include <pci-ath-fixup.h>
#include <lantiq_soc.h>
#include <linux/of_net.h>

extern int (*ltq_pci_plat_dev_init)(struct pci_dev *dev);
struct ath5k_platform_data ath5k_pdata;
struct ath9k_platform_data ath9k_pdata = {
	.led_pin = -1,
};
static u8 athxk_eeprom_mac[6];

static int ath9k_pci_plat_dev_init(struct pci_dev *dev)
{
	dev->dev.platform_data = &ath9k_pdata;
	return 0;
}

int __init of_ath9k_eeprom_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node, *mtd_np;
	int mac_offset;
	u32 mac_inc = 0, pci_slot = 0;
	int i;
	struct mtd_info *the_mtd;
	size_t flash_readlen;
	const __be32 *list;
	const char *part;
	phandle phandle;

	list = of_get_property(np, "ath,eep-flash", &i);
	if (!list || (i !=  (2 * sizeof(*list)))) {
		dev_err(&pdev->dev, "failed to find ath,eep-flash\n");
		return -ENODEV;
	}

	phandle = be32_to_cpup(list++);
	if (!phandle) {
		dev_err(&pdev->dev, "failed to find phandle\n");
		return -ENODEV;
	}

	mtd_np = of_find_node_by_phandle(phandle);
	if (!mtd_np) {
		dev_err(&pdev->dev, "failed to find mtd node\n");
		return -ENODEV;
	}

	part = of_get_property(mtd_np, "label", NULL);
	if (!part)
		part = mtd_np->name;

	the_mtd = get_mtd_device_nm(part);
	if (the_mtd == ERR_PTR(-ENODEV)) {
		dev_err(&pdev->dev, "failed to find mtd device\n");
		return -ENODEV;
	}

	i = mtd_read(the_mtd, be32_to_cpup(list),
			ATH9K_PLAT_EEP_MAX_WORDS << 1, &flash_readlen,
			(void *) ath9k_pdata.eeprom_data);
	put_mtd_device(the_mtd);
	if ((sizeof(ath9k_pdata.eeprom_data) != flash_readlen) || i) {
		dev_err(&pdev->dev, "failed to load eeprom from mtd\n");
		return -ENODEV;
	}

	if (of_find_property(np, "ath,eep-swap", NULL))
		for (i = 0; i < ATH9K_PLAT_EEP_MAX_WORDS; i++)
			ath9k_pdata.eeprom_data[i] = swab16(ath9k_pdata.eeprom_data[i]);

	if (of_find_property(np, "ath,eep-endian", NULL)) {
		ath9k_pdata.endian_check = true;

		dev_info(&pdev->dev, "endian check enabled.\n");
	}

	if (!of_property_read_u32(np, "ath,mac-offset", &mac_offset)) {
		memcpy_fromio(athxk_eeprom_mac, (void*) ath9k_pdata.eeprom_data + mac_offset, 6);
	} else {
		random_ether_addr(athxk_eeprom_mac);
		if (of_get_mac_address_mtd(np, athxk_eeprom_mac))
			dev_warn(&pdev->dev, "using random mac\n");
	}

	if (!of_property_read_u32(np, "ath,mac-increment", &mac_inc))
		athxk_eeprom_mac[5] += mac_inc;

	ath9k_pdata.macaddr = athxk_eeprom_mac;
	ltq_pci_plat_dev_init = ath9k_pci_plat_dev_init;

	if (!of_property_read_u32(np, "ath,pci-slot", &pci_slot)) {
		ltq_pci_ath_fixup(pci_slot, ath9k_pdata.eeprom_data);
		dev_info(&pdev->dev, "pci slot: %u\n", pci_slot);
	}

	dev_info(&pdev->dev, "loaded ath9k eeprom\n");

	return 0;
}

static struct of_device_id ath9k_eeprom_ids[] = {
	{ .compatible = "ath9k,eeprom" },
	{ }
};

static struct platform_driver ath9k_eeprom_driver = {
	.driver		= {
		.name		= "ath9k,eeprom",
		.owner	= THIS_MODULE,
		.of_match_table	= of_match_ptr(ath9k_eeprom_ids),
	},
};

static int ath9k_eep_loaded;
static int __init of_ath9k_eeprom_init(void)
{
	int ret = platform_driver_probe(&ath9k_eeprom_driver, of_ath9k_eeprom_probe);

	if (!ret)
		ath9k_eep_loaded = 1;

	return ret;
}

static int __init of_ath9k_eeprom_init_late(void)
{
	if (ath9k_eep_loaded)
		return 0;
	return platform_driver_probe(&ath9k_eeprom_driver, of_ath9k_eeprom_probe);
}
late_initcall(of_ath9k_eeprom_init_late);
subsys_initcall(of_ath9k_eeprom_init);

static int ath5k_pci_plat_dev_init(struct pci_dev *dev)
{
	dev->dev.platform_data = &ath5k_pdata;
	return 0;
}

int __init of_ath5k_eeprom_probe(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node, *mtd_np;
	int mac_offset;
	u32 mac_inc = 0;
	int i;
	struct mtd_info *the_mtd;
	size_t flash_readlen;
	const __be32 *list;
	const char *part;
	phandle phandle;

	list = of_get_property(np, "ath,eep-flash", &i);
	if (!list || (i !=  (2 * sizeof(*list)))) {
		dev_err(&pdev->dev, "failed to find ath,eep-flash\n");
		return -ENODEV;
	}

	phandle = be32_to_cpup(list++);
	if (!phandle) {
		dev_err(&pdev->dev, "failed to find phandle\n");
		return -ENODEV;
	}

	mtd_np = of_find_node_by_phandle(phandle);
	if (!mtd_np) {
		dev_err(&pdev->dev, "failed to find mtd node\n");
		return -ENODEV;
	}

	part = of_get_property(mtd_np, "label", NULL);
	if (!part)
		part = mtd_np->name;

	the_mtd = get_mtd_device_nm(part);
	if (the_mtd == ERR_PTR(-ENODEV)) {
		dev_err(&pdev->dev, "failed to find mtd device\n");
		return -ENODEV;
	}

	i = mtd_read(the_mtd, be32_to_cpup(list),
			ATH5K_PLAT_EEP_MAX_WORDS << 1, &flash_readlen,
			(void *) ath5k_pdata.eeprom_data);
	put_mtd_device(the_mtd);
	if ((sizeof(ath5k_pdata.eeprom_data) != flash_readlen) || i) {
		dev_err(&pdev->dev, "failed to load eeprom from mtd\n");
		return -ENODEV;
	}

	if (of_find_property(np, "ath,eep-swap", NULL))
		for (i = 0; i < ATH5K_PLAT_EEP_MAX_WORDS; i++)
			ath5k_pdata.eeprom_data[i] = swab16(ath9k_pdata.eeprom_data[i]);

	if (!of_property_read_u32(np, "ath,mac-offset", &mac_offset)) {
		memcpy_fromio(athxk_eeprom_mac, (void*) ath5k_pdata.eeprom_data + mac_offset, 6);
	} else {
		random_ether_addr(athxk_eeprom_mac);
		if (of_get_mac_address_mtd(np, athxk_eeprom_mac))
			dev_warn(&pdev->dev, "using random mac\n");
	}

	if (!of_property_read_u32(np, "ath,mac-increment", &mac_inc))
		athxk_eeprom_mac[5] += mac_inc;

	ath5k_pdata.macaddr = athxk_eeprom_mac;
	ltq_pci_plat_dev_init = ath5k_pci_plat_dev_init;

	dev_info(&pdev->dev, "loaded ath5k eeprom\n");

	return 0;
}

static struct of_device_id ath5k_eeprom_ids[] = {
	{ .compatible = "ath5k,eeprom" },
	{ }
};

static struct platform_driver ath5k_eeprom_driver = {
	.driver		= {
		.name		= "ath5k,eeprom",
		.owner	= THIS_MODULE,
		.of_match_table	= of_match_ptr(ath5k_eeprom_ids),
	},
};

static int __init of_ath5k_eeprom_init(void)
{
	return platform_driver_probe(&ath5k_eeprom_driver, of_ath5k_eeprom_probe);
}
device_initcall(of_ath5k_eeprom_init);
