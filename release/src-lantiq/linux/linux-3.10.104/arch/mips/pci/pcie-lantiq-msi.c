/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2009~2015 Lei Chuanhua <chuanhua.lei@lantiq.com>
 *  Copyright(c) 2016 Intel Corporation.
 */

/*!
 * \file pcie-lantiq-msi.c
 * \ingroup PCIE
 * \brief PCIe MSI OS interface file
 */
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/pci.h>
#include <linux/msi.h>
#include <linux/of_irq.h>
#include <lantiq_irq.h>

#include "pcie-lantiq.h"

/* MSI PIC */
#define MSI_PIC_BIG_ENDIAN		1
#define MSI_PIC_LITTLE_ENDIAN		0

#define MSI_PCI_INT_DISABLE		0x80000000
#define MSI_PIC_INT_LINE		0x70000000
#define MSI_PIC_INT_LINE_S		28
#define MSI_PIC_MSG_ADDR		0x0FFF0000
#define MSI_PIC_MSG_ADDR_S		16
#define MSI_PIC_MSG_DATA		0x0000FFFF
#define MSI_PIC_MSG_DATA_S		0x0

#define PCIE_MSI_MAX_IRQ_NUM_PER_RC	8

/* Keep base data lower bits as zero since MSI has maximum 32 vectors */
#define PCIE_MSI_BASE_DATA  0x4AE0

void pcie_msi_pic_init(struct ltq_pcie_port *lpp)
{
	int i;
	int ret;
	int irq_num;
	struct resource  msi_irqs[PCIE_MSI_MAX_IRQ_NUM_PER_RC];
	struct device *dev = lpp->dev;
	struct device_node *np = dev->of_node;
	struct msi_irq *lmsi = &lpp->msi_irqs;
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	spin_lock_init(&lmsi->msi_lock);
	lmsi->msi_pic_p = (struct ltq_msi_pic *)lpp->msi_pic;
	lmsi->msi_phy_base = lpp->msi_base;
	lmsi->msi_free_irq_bitmask = 0;
	lmsi->msi_multiple_irq_bitmask = 0;
	irq_num = of_irq_count(np);
	irq_num--; /* last one is ir interrupt */
	ret = of_irq_to_resource_table(np,
				msi_irqs, irq_num);
	if (ret != irq_num)
		panic("failed to load msi irq resources\n");

	if (ret > soc->per_rc_max_msi_irqs) {
		dev_err(lpp->dev, "Beyong the supported interrupts\n");
		return;
	}

	for (i = 0; i < soc->per_rc_max_msi_irqs; i++) {
		lmsi->msi_irq_idx[i].irq = msi_irqs[i].start;
		lmsi->msi_irq_idx[i].idx = i;
	}

	spin_lock(&lmsi->msi_lock);
	if (lpp->inbound_swap)
		lmsi->msi_pic_p->pic_endian = MSI_PIC_BIG_ENDIAN;
	else
		lmsi->msi_pic_p->pic_endian = MSI_PIC_LITTLE_ENDIAN;

	spin_unlock(&lmsi->msi_lock);
}

static void ltq_msi_unmask_irq(struct msi_desc *desc, u32 mask)
{
	u32 mask_bits = desc->masked;

	if (!desc->msi_attrib.maskbit)
		return;
	mask_bits &= ~mask;
	pci_write_config_dword(desc->dev, desc->mask_pos, mask_bits);
}

/**
 * \fn int ltq_setup_msi_irq(struct pci_dev *pdev, struct msi_desc *desc)
 * \brief Called when a driver request MSI interrupts instead of the
 * legacy INT A-D. This routine will allocate multiple interrupts
 * for MSI devices that support them. A device can override this by
 * programming the MSI control bits [6:4] before calling
 * pci_enable_msi().
 *
 * \param[in] pdev   Device requesting MSI interrupts
 * \param[in] desc   MSI descriptor
 *
 * \return   -EINVAL Invalid pcie root port or invalid msi bit
 * \return    0        OK
 * \ingroup PCIE_MSI
 */
static int ltq_setup_msi_irq(struct pci_dev *pdev,
	struct msi_desc *desc, int nvec)
{
	int i;
	int irq_base;
	int pos;
	u16 control;
	int irq_idx;
	int irq_step;
	int configured_private_bits;
	int request_private_bits;
	struct msi_msg msg;
	u16 search_mask;
	u16 msi_base_data;
	struct msi_irq *lmsi;
	u32 msi_mask;
	struct ltq_pcie_port *lpp = bus_to_ltq_pcie_port(pdev->bus);
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;
	lmsi = &lpp->msi_irqs;
	BUG_ON(nvec <= 0);
	dev_dbg(lpp->dev, "%s %s port %d enter\n",
		__func__, pci_name(pdev), lpp->id);

	/* Skip RC and switch ports since we have limited
	 * interrupt resource available */
	if (pci_pcie_type(pdev) != PCI_EXP_TYPE_ENDPOINT) {
		dev_dbg(lpp->dev,
			"%s RC %d or Switch Port doesn't use MSI interrupt\n",
			__func__, lpp->id);
		return -EINVAL;
	}
	/*
	 * Read the MSI config to figure out how many IRQs this device
	 * wants.  Most devices only want 1, which will give
	 * configured_private_bits and request_private_bits equal 0.
	 */
	pci_read_config_word(pdev, desc->msi_attrib.pos + PCI_MSI_FLAGS,
				&control);

	/*
	 * If the number of private bits has been configured then use
	 * that value instead of the requested number. This gives the
	 * driver the chance to override the number of interrupts
	 * before calling pci_enable_msi().
	 */
	configured_private_bits = (control & PCI_MSI_FLAGS_QSIZE) >> 4;

	if (configured_private_bits == 0) {
		/* Nothing is configured, so use the hardware requested size */
		request_private_bits =
			(control & PCI_MSI_FLAGS_QMASK) >> 1;
	} else {
		/*
		 * Use the number of configured bits, assuming the
		 * driver wanted to override the hardware request
		 * value.
		 */
		request_private_bits = configured_private_bits;
	}

	/*
	 * The PCI 2.3 spec mandates that there are at most 32
	 * interrupts. If this device asks for more, only give it one.
	 */
	if (request_private_bits > 5)
		request_private_bits = 0;
again:
	/*
	 * The IRQs have to be aligned on a power of two based on the
	 * number being requested.
	 */
	irq_step = (1 << request_private_bits);

	/* NB: Pick up the minimun one for usage */
	irq_step = min(irq_step, nvec);

	msi_mask = (1 << (1 << request_private_bits)) - 1;
	/* If device supports more than RC supported, fall back to single MSI */
	if (irq_step > soc->per_rc_max_msi_irqs) {
		irq_step = 1;
		request_private_bits = 0;
		dev_info(lpp->dev, "Requested interrupts more than RC supported, fall back to single MSI!!!\n");
	}

	/* Mask with one bit for each IRQ */
	search_mask = (1 << irq_step) - 1;

	/*
	 * We're going to search msi_free_irq_bitmask_lock for zero
	 * bits. This represents an MSI interrupt number that isn't in
	 * use.
	 */
	spin_lock(&lmsi->msi_lock);
	for (pos = 0; pos < MSI_IRQ_PIC_TABLE_SZ; pos += irq_step) {
		if ((lmsi->msi_free_irq_bitmask & (search_mask << pos)) == 0) {
			lmsi->msi_free_irq_bitmask |= search_mask << pos;
			lmsi->msi_multiple_irq_bitmask |=
				(search_mask >> 1) << pos;
			break;
		}
	}
	spin_unlock(&lmsi->msi_lock);

	/* Make sure the search for available interrupts didn't fail */
	if (pos >= MSI_IRQ_PIC_TABLE_SZ) {
		if (request_private_bits) {
			dev_dbg(lpp->dev,
				"%s: Unable to find %d free interrupts, trying just one",
				__func__,
				1 << request_private_bits);
			request_private_bits = 0;
			goto again;
		} else {
			dev_err(lpp->dev,
				"%s: Unable to find a free MSI interrupt\n",
				__func__);
			return -EINVAL;
		}
	}

	/* Only assign the base irq to msi entry */
	irq_base = lmsi->msi_irq_idx[pos].irq;
	irq_idx = lmsi->msi_irq_idx[pos].idx;

	dev_dbg(lpp->dev, "pos %d, irq %d irq_idx %d\n",
		pos, irq_base, irq_idx);

	/*
	 * Initialize MSI. This has to match the memory-write endianess
	 * from the device
	 * Address bits [23:12]
	 * For multiple MSI, we have to assign and enable sequence MSI data
	 * Make sure that base data lower bits as zero since multiple MSI
	 * just modify lower several bits to generate different interrupts
	 * pos is used for multiple instanaces of device and single MSI 
	 */
	msi_base_data = ((1 << pos) + PCIE_MSI_BASE_DATA) & (~search_mask);
	spin_lock(&lmsi->msi_lock);
	for (i = 0; i < irq_step; i++) {
		lmsi->msi_pic_p->pic_table[pos + i] =
			SM((irq_idx + i) % soc->per_rc_max_msi_irqs,
				MSI_PIC_INT_LINE)
			| SM((lmsi->msi_phy_base >> 12), MSI_PIC_MSG_ADDR)
			| SM((msi_base_data + i), MSI_PIC_MSG_DATA);
		/* Enable this entry */
		lmsi->msi_pic_p->pic_table[pos + i] &= ~MSI_PCI_INT_DISABLE;
		dev_dbg(lpp->dev, "pic_table[%d]: 0x%08x\n",
			(pos + i), lmsi->msi_pic_p->pic_table[pos + i]);
	}
	spin_unlock(&lmsi->msi_lock);

	/* Update multiple MSI property */
	desc->msi_attrib.multiple = request_private_bits;
	/* Assign base irq and base data to the first MSI entry */
	irq_set_msi_desc(irq_base, desc);
	msg.address_hi = 0x0;
	msg.address_lo = lmsi->msi_phy_base;
	msg.data = SM(msi_base_data, MSI_PIC_MSG_DATA);
	dev_dbg(lpp->dev,
		"base msi_data: pos %d data 0x%08x irq %d-%d\n",
		pos, msg.data, irq_base, irq_base + irq_step - 1);

	write_msi_msg(irq_base, &msg);

	/* Unmask optional per-vector MSI, some advanced card supported */
	ltq_msi_unmask_irq(desc, msi_mask);

	dev_dbg(lpp->dev, "%s port %d exit\n", __func__, lpp->id);
	return 0;
}

static int ltq_setup_msi_irqs(struct msi_chip *chip, struct pci_dev *dev,
		int nvec, int type)
{
	struct msi_desc *entry;
	int ret;

	/* MSI-X is not supported */
	if (type == PCI_CAP_ID_MSIX)
		return -EINVAL;

	list_for_each_entry(entry, &dev->msi_list, list) {
		ret = ltq_setup_msi_irq(dev, entry, nvec);
		if (ret < 0)
			return ret;
		if (ret > 0)
			return -ENOSPC;
	}
	return 0;
}

/**
 * \fn void ltq_teardown_msi_irqs(struct msi_chip *chip, struct pci_dev *pdev)
 * \brief Called when a device no longer needs its MSI interrupts. All
 * MSI interrupts for the device are freed.
 *
 * \param irq   The devices first irq number. There may be multple in sequence.
 * \return none
 * \ingroup PCIE_MSI
 */
static void ltq_teardown_msi_irqs(struct msi_chip *chip, struct pci_dev *pdev)
{
	int i;
	int pos;
	int number_irqs;
	u16 bitmask;
	struct msi_desc *desc;
	int irq;
	struct ltq_pcie_port *lpp;
	struct msi_irq *lmsi;

	lpp = bus_to_ltq_pcie_port(pdev->bus);
	lmsi = &lpp->msi_irqs;

	/*
	 * Skip RC and switch ports since we have limited
	 * interrupt resource available
	 */
	if (pci_pcie_type(pdev) != PCI_EXP_TYPE_ENDPOINT) {
		dev_dbg(lpp->dev,
			"%s RC %d or Switch Port doesn't use MSI interrupt\n",
			__func__, lpp->id);
		return;
	}

	BUG_ON(list_empty(&pdev->msi_list));
	desc = list_first_entry(&pdev->msi_list, struct msi_desc, list);

	/* NB pdev->irq has been restored, retrieve msi irq number */
	irq = desc->irq;

	dev_dbg(lpp->dev, "%s port %d irq %d enter\n",
		__func__, lpp->id, irq);

	if (irq == 0)
		return;
	/*
	 * Shift the mask to the correct bit location, not always correct
	 * Probally, the first match will be chosen.
	 */
	for (pos = 0; pos < MSI_IRQ_PIC_TABLE_SZ; pos++) {
		if ((lmsi->msi_irq_idx[pos].irq == irq)
			&& (lmsi->msi_free_irq_bitmask & (1 << pos)))
			break;
	}

	if (pos >= MSI_IRQ_PIC_TABLE_SZ) {
		dev_err(lpp->dev,
			"%s: Unable to find a matched MSI interrupt %d\n",
			__func__, irq);
		return;
	}
	/*
	 * Count the number of IRQs we need to free by looking at the
	 * msi_multiple_irq_bitmask. Each bit set means that the next
	 * IRQ is also owned by this device.
	 */
	number_irqs = 0;
	while (((pos + number_irqs) < MSI_IRQ_PIC_TABLE_SZ)
		&& (lmsi->msi_multiple_irq_bitmask
		& (1 << (pos + number_irqs))))
		number_irqs++;

	number_irqs++;

	/* Disable entries if multiple MSI  */
	spin_lock(&lmsi->msi_lock);
	for (i = 0; i < number_irqs; i++) {
		lmsi->msi_pic_p->pic_table[pos + i] |= MSI_PCI_INT_DISABLE;
		lmsi->msi_pic_p->pic_table[pos + i] &=
			~(MSI_PIC_INT_LINE | MSI_PIC_MSG_ADDR |
			MSI_PIC_MSG_DATA);
	}
	spin_unlock(&lmsi->msi_lock);

	/* Mask with one bit for each IRQ */
	bitmask = (1 << number_irqs) - 1;

	bitmask <<= pos;
	if ((lmsi->msi_free_irq_bitmask & bitmask) != bitmask) {
		dev_err(lpp->dev,
			"%s: Attempted to teardown MSI interrupt %d not in use\n",
			__func__, irq);
		return;
	}

	/* Checks are done, update the in use bitmask */
	spin_lock(&lmsi->msi_lock);
	lmsi->msi_free_irq_bitmask &= ~bitmask;
	lmsi->msi_multiple_irq_bitmask &= ~(bitmask >> 1);
	spin_unlock(&lmsi->msi_lock);

	dev_dbg(lpp->dev, "%s port %d exit\n", __func__, lpp->id);
}

struct msi_chip ltq_msi_chip = {
	.setup_irqs = ltq_setup_msi_irqs,
	.teardown_irqs = ltq_teardown_msi_irqs,
};

MODULE_LICENSE("GPL V2");
MODULE_AUTHOR("Lei Chuanhua <Chuanhua.Lei@lantiq.com>");
MODULE_SUPPORTED_DEVICE("Lqntiq PCIe IP builtin MSI PIC module");
MODULE_DESCRIPTION("Lantiq PCIe IP builtin MSI PIC driver");
