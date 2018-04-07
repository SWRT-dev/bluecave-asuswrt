/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2012 John Crispin <blogic@openwrt.org>
 *  Copyright (C) 2015 Lei Chuanhua <Chuanhua.lei@lantiq.com>
 */
#include <linux/pci.h>
#include <linux/of_irq.h>
#include <linux/of_pci.h>

int (*ltq_pci_plat_arch_init)(struct pci_dev *dev) = NULL;
int (*ltq_pci_plat_dev_init)(struct pci_dev *dev) = NULL;
int (*ltq_pci_map_irq)(const struct pci_dev *dev, u8 slot, u8 pin);

int pcibios_plat_dev_init(struct pci_dev *dev)
{
	int pos;
	pos = pci_find_capability((struct pci_dev *)dev, PCI_CAP_ID_EXP);
	if (pos > 0) {
		if (ltq_pci_plat_arch_init)
			return ltq_pci_plat_arch_init(dev);
	}

	if (ltq_pci_plat_dev_init)
		return ltq_pci_plat_dev_init(dev);

	return 0;
}

int __init pcibios_map_irq(const struct pci_dev *dev, u8 slot, u8 pin)
{
	int pos;
	struct of_irq dev_irq;
	int irq = -1;

	pos = pci_find_capability((struct pci_dev *)dev, PCI_CAP_ID_EXP);
	if (pos > 0) {
		if (ltq_pci_map_irq)
			return ltq_pci_map_irq(dev, slot, pin);
	} else {
		if (of_irq_map_pci(dev, &dev_irq)) {
			dev_err(&dev->dev,
			"trying to map irq for unknown slot:%d pin:%d\n",
				slot, pin);
			return 0;
		}
		irq = irq_create_of_mapping(dev_irq.controller,
			dev_irq.specifier, dev_irq.size);
	}
	dev_info(&dev->dev, "SLOT:%d PIN:%d IRQ:%d\n", slot, pin, irq);
	return irq;
}
