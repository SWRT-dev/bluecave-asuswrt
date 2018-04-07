/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C)2009~2015 Lei Chuanhua <Chuanhua.lei@lantiq.com>
 */
 /*!
 \file fixup-lantiq-pcie.c
 \ingroup PCIE
 \brief PCIe Fixup functions source file
*/
#include <linux/pci.h>
#include <linux/pci_regs.h>
#include <linux/pci_ids.h>

#include <lantiq_soc.h>

#include "pcie-lantiq.h"

#define PCI_VENDOR_ID_INFINEON		0x15D1
#define PCI_VENDOR_ID_LANTIQ		0x1BEF

static void pcie_fixup_bridge(struct pci_dev *dev)
{
	struct ltq_pcie_port *lpp;
	lpp = bus_to_ltq_pcie_port(dev->bus);

	if ((dev->class >> 16) == PCI_BASE_CLASS_BRIDGE) {
		u16 reg;
		pci_read_config_word(dev, PCI_COMMAND, &reg);
		/* Setup COMMAND register */
		reg = PCI_COMMAND_IO | PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER
		| PCI_COMMAND_SERR;
		pci_write_config_word(dev, PCI_COMMAND, reg);
		dev_info(lpp->dev,
			"%s port %d : fixup host controller %s (%04x:%04x)\n",
			__func__, lpp->id, pci_name(dev),
			dev->vendor, dev->device);
	}
}
DECLARE_PCI_FIXUP_FINAL(PCI_VENDOR_ID_INFINEON, PCI_ANY_ID, pcie_fixup_bridge);
DECLARE_PCI_FIXUP_FINAL(PCI_VENDOR_ID_LANTIQ, PCI_ANY_ID, pcie_fixup_bridge);

/*
 * The root complex has a hardwired class of PCI_CLASS_NETWORK_OTHER or
 * PCI_CLASS_BRIDGE_HOST, when it is operating as a root complex this
 * needs to be switched to * PCI_CLASS_BRIDGE_PCI
 */
static void pcie_rc_class_fixup(struct pci_dev *dev)
{
	struct ltq_pcie_port *lpp;
	lpp = bus_to_ltq_pcie_port(dev->bus);

	if ((dev->bus->parent == NULL) && (dev->devfn == 0)) {
		dev->class = (PCI_CLASS_BRIDGE_PCI << 8) | (dev->class & 0xff);
		dev_info(lpp->dev,
			"%s port %d: fixed pci host bridge to pci-pci bridge\n",
			__func__, lpp->id);
	}
}

DECLARE_PCI_FIXUP_EARLY(PCI_VENDOR_ID_INFINEON, PCI_ANY_ID,
	pcie_rc_class_fixup);
DECLARE_PCI_FIXUP_EARLY(PCI_VENDOR_ID_LANTIQ, PCI_ANY_ID,
	pcie_rc_class_fixup);

