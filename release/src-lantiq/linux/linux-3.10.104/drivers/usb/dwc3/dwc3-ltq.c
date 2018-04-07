/*
 * Copyright (C) 2015 LANTIQ Semiconductor Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 */
 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/clk.h>

#include <linux/usb/otg.h>
#include <linux/usb/ch9.h>
#include <linux/usb/gadget.h>

#include "core.h"
#include "gadget.h"
#include "io.h"

#include "debug.h"

static u64 dwc3_otg_dmamask = 0xffffffff;

#include <lantiq_soc.h>

#define LTQ_GRX500_RCU_USB_PHY0 BIT(4) /* 0:NO_REST, 1:REST*/
#define LTQ_GRX500_RCU_USB_PHY1 BIT(5)

#define LTQ_GRX500_CHIP_TOP_AXI_DEV_END BIT(9) /* 0: Little, 1: Big Endian */
#define LTQ_GRX500_CHIP_TOP_AXI_HOST_END BIT(10)


/* -------------------------------------------------------------------------- */

int ltq_vbus_gpio_init(struct dwc3 *dwc)
{
	int err;
	int vbus_pin = dwc->vbus_pin;

	if (vbus_pin < 0)
		return -EINVAL;

	err = gpio_is_valid(vbus_pin);
	if (!err)
	{
		printk("?%s: gpio pin %d is invalid\n", __func__, vbus_pin);
		return -EINVAL;
	}

	err = gpio_request(vbus_pin, "dwc3_vbus");
	if (err)
	{
		printk("?%s: can't request gpio %d\n", __func__, vbus_pin);
		return -EINVAL;
	}

	gpio_direction_output(vbus_pin, 1);

	return 0;
}

void ltq_vbus_gpio_free(struct dwc3 *dwc)
{
	int vbus_pin = dwc->vbus_pin;

	if (vbus_pin)
		gpio_free(vbus_pin);
}

int ltq_usb_release_phy_reset(int usb_id)
{

	if (usb_id > 1)
		return -EINVAL;

	ltq_usb_phy_reset(usb_id == 0 ? LTQ_GRX500_RCU_USB_PHY0 : 
		LTQ_GRX500_RCU_USB_PHY1);

	return 0;
}

int ltq_usb_set_axi_host_to_be(int usb_id)
{

	if (usb_id > 1)
		return -EINVAL;

	/*
	 * Select AXI Host Port Endian Mode.
	 * 0: Little 1: Big
	 */

	if (usb_id == 0)
		ltq_usb0_port_endian_set(LTQ_GRX500_CHIP_TOP_AXI_DEV_END,
			LTQ_GRX500_CHIP_TOP_AXI_HOST_END);
	else
		ltq_usb1_port_endian_set(LTQ_GRX500_CHIP_TOP_AXI_DEV_END,
			LTQ_GRX500_CHIP_TOP_AXI_HOST_END);

	return 0;
}


void dwc3_ltq_set_mode(struct dwc3 *dwc, u32 mode)
{
	u32 reg;

	reg = dwc3_readl(dwc->regs, DWC3_GCTL);
	reg &= ~(DWC3_GCTL_PRTCAPDIR(DWC3_GCTL_PRTCAP_OTG));
	reg |= DWC3_GCTL_PRTCAPDIR(mode);
	dwc3_writel(dwc->regs, DWC3_GCTL, reg);
}

/**
 * dwc3_ltq_core_soft_reset - Issues core soft reset and PHY reset
 * @dwc: pointer to our context structure
 */
static void dwc3_ltq_core_soft_reset(struct dwc3 *dwc)
{
	u32		reg;

	/* Before Resetting PHY, put Core in Reset */
	reg = dwc3_readl(dwc->regs, DWC3_GCTL);
	reg |= DWC3_GCTL_CORESOFTRESET;
	dwc3_writel(dwc->regs, DWC3_GCTL, reg);

	/* Assert USB3 PHY reset */
	reg = dwc3_readl(dwc->regs, DWC3_GUSB3PIPECTL(0));
	reg |= DWC3_GUSB3PIPECTL_PHYSOFTRST;
	dwc3_writel(dwc->regs, DWC3_GUSB3PIPECTL(0), reg);

	/* Assert USB2 PHY reset */
	reg = dwc3_readl(dwc->regs, DWC3_GUSB2PHYCFG(0));
	reg |= DWC3_GUSB2PHYCFG_PHYSOFTRST;
	dwc3_writel(dwc->regs, DWC3_GUSB2PHYCFG(0), reg);

	/* Arec: Change core power state to P2 and then perform receiver detection */
	reg = dwc3_readl(dwc->regs, DWC3_GUSB3PIPECTL(0));
	reg |= DWC3_GUSB3PIPECTL_DISRXDETP3;
	dwc3_writel(dwc->regs, DWC3_GUSB3PIPECTL(0), reg);

	mdelay(100);

	usb_phy_init(dwc->usb3_phy);
	mdelay(100);
	/* Clear USB3 PHY reset */
	reg = dwc3_readl(dwc->regs, DWC3_GUSB3PIPECTL(0));
	reg &= ~DWC3_GUSB3PIPECTL_PHYSOFTRST;
	dwc3_writel(dwc->regs, DWC3_GUSB3PIPECTL(0), reg);

	/* Clear USB2 PHY reset */
	reg = dwc3_readl(dwc->regs, DWC3_GUSB2PHYCFG(0));
	reg &= ~DWC3_GUSB2PHYCFG_PHYSOFTRST;
	dwc3_writel(dwc->regs, DWC3_GUSB2PHYCFG(0), reg);

	mdelay(100);

	/* After PHYs are stable we can take Core out of reset state */
	reg = dwc3_readl(dwc->regs, DWC3_GCTL);
	reg &= ~DWC3_GCTL_CORESOFTRESET;
	dwc3_writel(dwc->regs, DWC3_GCTL, reg);
}

/**
 * dwc3_ltq_free_one_event_buffer - Frees one event buffer
 * @dwc: Pointer to our controller context structure
 * @evt: Pointer to event buffer to be freed
 */
static void dwc3_ltq_free_one_event_buffer(struct dwc3 *dwc,
		struct dwc3_event_buffer *evt)
{
	dma_free_coherent(dwc->dev, evt->length, evt->buf, evt->dma);
}

/**
 * dwc3_ltq_alloc_one_event_buffer - Allocates one event buffer structure
 * @dwc: Pointer to our controller context structure
 * @length: size of the event buffer
 *
 * Returns a pointer to the allocated event buffer structure on success
 * otherwise ERR_PTR(errno).
 */
static struct dwc3_event_buffer *dwc3_ltq_alloc_one_event_buffer(struct dwc3 *dwc,
		unsigned length)
{
	struct dwc3_event_buffer	*evt;

	evt = devm_kzalloc(dwc->dev, sizeof(*evt), GFP_KERNEL);
	if (!evt)
		return ERR_PTR(-ENOMEM);

	evt->dwc	= dwc;
	evt->length	= length;
	evt->buf	= dma_alloc_coherent(dwc->dev, length,
			&evt->dma, GFP_KERNEL);
	if (!evt->buf)
		return ERR_PTR(-ENOMEM);

	return evt;
}

/**
 * dwc3_ltq_free_event_buffers - frees all allocated event buffers
 * @dwc: Pointer to our controller context structure
 */
static void dwc3_ltq_free_event_buffers(struct dwc3 *dwc)
{
	struct dwc3_event_buffer	*evt;
	int i;

	for (i = 0; i < dwc->num_event_buffers; i++) {
		evt = dwc->ev_buffs[i];
		if (evt)
			dwc3_ltq_free_one_event_buffer(dwc, evt);
	}
}

/**
 * dwc3_ltq_alloc_event_buffers - Allocates @num event buffers of size @length
 * @dwc: pointer to our controller context structure
 * @length: size of event buffer
 *
 * Returns 0 on success otherwise negative errno. In the error case, dwc
 * may contain some buffers allocated but not all which were requested.
 */
static int dwc3_ltq_alloc_event_buffers(struct dwc3 *dwc, unsigned length)
{
	int			num;
	int			i;

	num = DWC3_NUM_INT(dwc->hwparams.hwparams1);
	dwc->num_event_buffers = num;

	dwc->ev_buffs = devm_kzalloc(dwc->dev, sizeof(*dwc->ev_buffs) * num,
			GFP_KERNEL);
	if (!dwc->ev_buffs) {
		dev_err(dwc->dev, "can't allocate event buffers array\n");
		return -ENOMEM;
	}

	for (i = 0; i < num; i++) {
		struct dwc3_event_buffer	*evt;

		evt = dwc3_ltq_alloc_one_event_buffer(dwc, length);
		if (IS_ERR(evt)) {
			dev_err(dwc->dev, "can't allocate event buffer\n");
			return PTR_ERR(evt);
		}
		dwc->ev_buffs[i] = evt;
	}

	return 0;
}

/**
 * dwc3_ltq_event_buffers_setup - setup our allocated event buffers
 * @dwc: pointer to our controller context structure
 *
 * Returns 0 on success otherwise negative errno.
 */
static int dwc3_ltq_event_buffers_setup(struct dwc3 *dwc)
{
	struct dwc3_event_buffer	*evt;
	int				n;

	for (n = 0; n < dwc->num_event_buffers; n++) {
		evt = dwc->ev_buffs[n];
		dev_dbg(dwc->dev, "Event buf %p dma %08llx length %d\n",
				evt->buf, (unsigned long long) evt->dma,
				evt->length);

		evt->lpos = 0;

		dwc3_writel(dwc->regs, DWC3_GEVNTADRLO(n),
				lower_32_bits(evt->dma));
		dwc3_writel(dwc->regs, DWC3_GEVNTADRHI(n),
				upper_32_bits(evt->dma));
		dwc3_writel(dwc->regs, DWC3_GEVNTSIZ(n),
				evt->length & 0xffff);
		dwc3_writel(dwc->regs, DWC3_GEVNTCOUNT(n), 0);
	}

	return 0;
}

static void dwc3_ltq_event_buffers_cleanup(struct dwc3 *dwc)
{
	struct dwc3_event_buffer	*evt;
	int				n;

	for (n = 0; n < dwc->num_event_buffers; n++) {
		evt = dwc->ev_buffs[n];

		evt->lpos = 0;

		dwc3_writel(dwc->regs, DWC3_GEVNTADRLO(n), 0);
		dwc3_writel(dwc->regs, DWC3_GEVNTADRHI(n), 0);
		dwc3_writel(dwc->regs, DWC3_GEVNTSIZ(n), 0);
		dwc3_writel(dwc->regs, DWC3_GEVNTCOUNT(n), 0);
	}
}

static void dwc3_ltq_core_num_eps(struct dwc3 *dwc)
{
	struct dwc3_hwparams	*parms = &dwc->hwparams;

	dwc->num_in_eps = DWC3_NUM_IN_EPS(parms);
	dwc->num_out_eps = DWC3_NUM_EPS(parms) - dwc->num_in_eps;

	dev_vdbg(dwc->dev, "found %d IN and %d OUT endpoints\n",
			dwc->num_in_eps, dwc->num_out_eps);
}

static void dwc3_ltq_cache_hwparams(struct dwc3 *dwc)
{
	struct dwc3_hwparams	*parms = &dwc->hwparams;

	parms->hwparams1 = dwc3_readl(dwc->regs, DWC3_GHWPARAMS1);
}

/**
 * dwc3_ltq_core_init - Low-level initialization of DWC3 Core
 * @dwc: Pointer to our controller context structure
 *
 * Returns 0 on success otherwise negative errno.
 */
static int dwc3_ltq_core_init(struct dwc3 *dwc)
{
	unsigned long		timeout;
	u32			reg;
	int			ret;

	reg = dwc3_readl(dwc->regs, DWC3_GSNPSID);
	/* This should read as U3 followed by revision number */
	if ((reg & DWC3_GSNPSID_MASK) != 0x55330000) {
		dev_err(dwc->dev, "this is not a DesignWare USB3 DRD Core\n");
		ret = -ENODEV;
		goto err0;
	}
	dwc->revision = reg;

	/* issue device SoftReset too */
	timeout = jiffies + msecs_to_jiffies(500);
	dwc3_writel(dwc->regs, DWC3_DCTL, DWC3_DCTL_CSFTRST);
	do {
		reg = dwc3_readl(dwc->regs, DWC3_DCTL);
		if (!(reg & DWC3_DCTL_CSFTRST))
			break;

		if (time_after(jiffies, timeout)) {
			dev_err(dwc->dev, "Reset Timed Out\n");
			ret = -ETIMEDOUT;
			goto err0;
		}

		cpu_relax();
	} while (true);

	dwc3_ltq_core_soft_reset(dwc);

	reg = dwc3_readl(dwc->regs, DWC3_GCTL);
	reg &= ~DWC3_GCTL_SCALEDOWN_MASK;
	reg &= ~DWC3_GCTL_DISSCRAMBLE;

	switch (DWC3_GHWPARAMS1_EN_PWROPT(dwc->hwparams.hwparams1)) {
	case DWC3_GHWPARAMS1_EN_PWROPT_CLK:
		reg &= ~DWC3_GCTL_DSBLCLKGTNG;
		break;
	default:
		dev_dbg(dwc->dev, "No power optimization available\n");
	}

	/*
	 * WORKAROUND: DWC3 revisions <1.90a have a bug
	 * where the device can fail to connect at SuperSpeed
	 * and falls back to high-speed mode which causes
	 * the device to enter a Connect/Disconnect loop
	 */
	if (dwc->revision < DWC3_REVISION_190A)
		reg |= DWC3_GCTL_U2RSTECN;

	dwc3_ltq_core_num_eps(dwc);

	dwc3_writel(dwc->regs, DWC3_GCTL, reg);

	return 0;

err0:
	return ret;
}

#define DWC3_ALIGN_MASK		(16 - 1)

static int dwc3_ltq_probe(struct platform_device *pdev)
{
	struct device_node	*node = pdev->dev.of_node;
	struct resource		*res;
	struct dwc3		*dwc;
	struct device		*dev = &pdev->dev;

	int			ret = -ENOMEM;

	void __iomem		*regs;
	void			*mem;

	int usb_id;
	int vbus_pin;
	struct clk *clk;

	if (!node) {
		dev_err(dev, "device node not found\n");
		return -EINVAL;
	}

	usb_id = of_alias_get_id(node, "usb");
	if (usb_id < 0)
	{
		dev_err(dev, "Failed to get alias id, errno %d\n", usb_id);
		return -ENODEV;
	}

	clk = clk_get(dev, NULL);
	if (IS_ERR(clk))
	{
		dev_err(dev, "Failed to get clk\n");
		return PTR_ERR(clk);
	}

	mem = devm_kzalloc(dev, sizeof(*dwc) + DWC3_ALIGN_MASK, GFP_KERNEL);
	if (!mem) {
		dev_err(dev, "not enough memory\n");
		return -ENOMEM;
	}
	dwc = PTR_ALIGN(mem, DWC3_ALIGN_MASK + 1);
	dwc->mem = mem;

	vbus_pin = of_get_named_gpio(node, "vbus-gpio", 0);
	if (vbus_pin < 0)
	{
		dev_err(dev, "Failed to get vbus-gpio, errno %d\n", vbus_pin);
		return -ENODEV;
	}

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (!res) {
		dev_err(dev, "missing IRQ\n");
		return -ENODEV;
	}
	dwc->xhci_resources[1].start = res->start;
	dwc->xhci_resources[1].end = res->end;
	dwc->xhci_resources[1].flags = res->flags;
	dwc->xhci_resources[1].name = res->name;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "missing memory resource\n");
		return -ENODEV;
	}
	dwc->xhci_resources[0].start = res->start;
	dwc->xhci_resources[0].end = dwc->xhci_resources[0].start +
					DWC3_XHCI_REGS_END;
	dwc->xhci_resources[0].flags = res->flags;
	dwc->xhci_resources[0].name = res->name;


	 /*
	  * Request memory region but exclude xHCI regs,
	  * since it will be requested by the xhci-plat driver.
	  */
	res = devm_request_mem_region(dev, res->start + DWC3_GLOBALS_REGS_START,
			resource_size(res) - DWC3_GLOBALS_REGS_START,
			dev_name(dev));
	if (!res) {
		dev_err(dev, "can't request mem region\n");
		return -ENOMEM;
	}

	regs = devm_ioremap_nocache(dev, res->start, resource_size(res));
	if (!regs) {
		dev_err(dev, "ioremap failed\n");
		return -ENOMEM;
	}

	if (node)
		dwc->usb3_phy = devm_usb_get_phy_by_phandle(dev, "phys", 0);
	if (IS_ERR(dwc->usb3_phy)) {
		ret = PTR_ERR(dwc->usb3_phy);

		/*
		 * if -ENXIO is returned, it means PHY layer wasn't
		 * enabled, so it makes no sense to return -EPROBE_DEFER
		 * in that case, since no PHY driver will ever probe.
		 */
		if (ret == -ENXIO)
			return ret;

		dev_err(dev, "no usb3 phy configured\n");
		return -EPROBE_DEFER;
	}
	spin_lock_init(&dwc->lock);
	platform_set_drvdata(pdev, dwc);

	dwc->regs	= regs;
	dwc->regs_size	= resource_size(res);
	dwc->dev	= dev;
	dwc->vbus_pin = vbus_pin;

	dev->dma_mask	= &dwc3_otg_dmamask;
	dev->coherent_dma_mask = DMA_BIT_MASK(32);

	/*
	 *Request Clock Enable
	 */
	clk_enable(clk);
	mdelay(100);

	ret = ltq_vbus_gpio_init(dwc);
	if (ret) {
		dev_err(dwc->dev, "?%s: can't init usb%d vbus gpio %d\n", __func__, usb_id, dwc->vbus_pin);
		return -EINVAL;
	}

	/*
	 * CHIP_TOP Set AXI to Big Endian Mode
	 */
	ret = ltq_usb_set_axi_host_to_be(usb_id);
	if (ret) {
		dev_err(dwc->dev, "?%s: can't set usb%d host port endian\n", __func__, usb_id);
		return -EINVAL;
	}
	mdelay(100);

	dwc3_ltq_cache_hwparams(dwc);

	ret = dwc3_ltq_alloc_event_buffers(dwc, DWC3_EVENT_BUFFERS_SIZE);
	if (ret) {
		dev_err(dwc->dev, "failed to allocate event buffers\n");
		ret = -ENOMEM;
		goto err0;
	}

	/*
	 * RCU RST_REQ2 - USB PHY0/1 released from reset;
	 */
	ltq_usb_release_phy_reset(usb_id);
	mdelay(100);

	ret = dwc3_ltq_core_init(dwc);
	if (ret) {
		dev_err(dev, "failed to initialize core\n");
		goto err0;
	}

	ret = dwc3_ltq_event_buffers_setup(dwc);
	if (ret) {
		dev_err(dwc->dev, "failed to setup event buffers\n");
		goto err1;
	}

	dwc3_ltq_set_mode(dwc, DWC3_GCTL_PRTCAP_HOST);
	ret = dwc3_host_init(dwc);
	if (ret) {
		dev_err(dev, "failed to initialize host\n");
		goto err1;
	}

	ret = dwc3_debugfs_init(dwc);
	if (ret) {
		dev_err(dev, "failed to initialize debugfs\n");
		goto err2;
	}

	return 0;

err2:
	dwc3_host_exit(dwc);
err1:
	dwc3_ltq_event_buffers_cleanup(dwc);
err0:
	dwc3_ltq_free_event_buffers(dwc);

	return ret;
}

static int dwc3_ltq_remove(struct platform_device *pdev)
{
	struct dwc3	*dwc = platform_get_drvdata(pdev);

	dwc3_debugfs_exit(dwc);
	dwc3_host_exit(dwc);

	dwc3_ltq_event_buffers_cleanup(dwc);
	dwc3_ltq_free_event_buffers(dwc);
	ltq_vbus_gpio_free(dwc);
	usb_phy_shutdown(dwc->usb3_phy);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int dwc3_ltq_prepare(struct device *dev)
{
	struct dwc3	*dwc = dev_get_drvdata(dev);
	unsigned long	flags;

	spin_lock_irqsave(&dwc->lock, flags);
	dwc3_ltq_event_buffers_cleanup(dwc);
	spin_unlock_irqrestore(&dwc->lock, flags);

	return 0;
}

static void dwc3_ltq_complete(struct device *dev)
{
	struct dwc3	*dwc = dev_get_drvdata(dev);
	unsigned long	flags;

	spin_lock_irqsave(&dwc->lock, flags);
	dwc3_ltq_event_buffers_setup(dwc);
	spin_unlock_irqrestore(&dwc->lock, flags);
}

static int dwc3_ltq_suspend(struct device *dev)
{
	struct dwc3	*dwc = dev_get_drvdata(dev);
	unsigned long	flags;

	spin_lock_irqsave(&dwc->lock, flags);
	dwc->gctl = dwc3_readl(dwc->regs, DWC3_GCTL);
	spin_unlock_irqrestore(&dwc->lock, flags);

	return 0;
}

static int dwc3_ltq_resume(struct device *dev)
{
	struct dwc3	*dwc = dev_get_drvdata(dev);
	unsigned long	flags;

	spin_lock_irqsave(&dwc->lock, flags);
	dwc3_writel(dwc->regs, DWC3_GCTL, dwc->gctl);
	spin_unlock_irqrestore(&dwc->lock, flags);

	return 0;
}

static const struct dev_pm_ops dwc3_ltq_dev_pm_ops = {
	.prepare	= dwc3_ltq_prepare,
	.complete	= dwc3_ltq_complete,

	SET_SYSTEM_SLEEP_PM_OPS(dwc3_ltq_suspend, dwc3_ltq_resume)
};

#define DWC3_PM_OPS	&(dwc3_ltq_dev_pm_ops)
#else
#define DWC3_PM_OPS	NULL
#endif

#ifdef CONFIG_OF
static const struct of_device_id of_dwc3_core_ltq_match[] = {
	{
		.compatible = "lantiq,dwc3-core"
	},
	{ },
};

MODULE_DEVICE_TABLE(of, of_dwc3_core_ltq_match);
#endif

static struct platform_driver dwc3_core_driver = {
	.probe		= dwc3_ltq_probe,
	.remove		= dwc3_ltq_remove,
	.driver		= {
		.name	= "ltq-dwc3-core",
		.of_match_table	= of_match_ptr(of_dwc3_core_ltq_match),
		.pm	= DWC3_PM_OPS,
	},
};

module_platform_driver(dwc3_core_driver);

MODULE_ALIAS("platform:ltq-dwc3");
MODULE_AUTHOR("Arec Kao <arec.kao@lantiq.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("DesignWare USB3 Lantiq Glue Layer");
