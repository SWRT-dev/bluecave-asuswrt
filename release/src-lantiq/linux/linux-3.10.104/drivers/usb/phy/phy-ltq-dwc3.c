#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/dma-mapping.h>
#include <linux/usb/otg.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_address.h>

#define LANE0_TX_OVRD_IN_LO 0x4000
#define LANE0_RX_OVRD_IN_LO 0x4014

#define DWC3_PHY_TX_INVERT_OVRD (1<<3)
#define DWC3_PHY_RX_INVERT_OVRD (1<<1)

struct ltq_dwc3_phy {

	struct usb_phy	phy;
	spinlock_t		lock;
	struct device	*dev;
	void __iomem	*base;
	bool is_tx_pol_inv;
	bool is_rx_pol_inv;
		
};

static inline void ltq_dwc3_writel(void __iomem *base, u32 offset, u32 value)
{
	writel(value, base + offset);
}

static int ltq_dwc3_phy_init(struct usb_phy *phy)
{
	struct ltq_dwc3_phy *dwc3_phy = dev_get_drvdata(phy->dev);
	u32 reg;

	if (dwc3_phy->is_tx_pol_inv) {
		reg = readl(dwc3_phy->base + LANE0_TX_OVRD_IN_LO);
		reg |= DWC3_PHY_TX_INVERT_OVRD;
		ltq_dwc3_writel(dwc3_phy->base, LANE0_TX_OVRD_IN_LO, reg);
	}
	if (dwc3_phy->is_rx_pol_inv) {
		reg = readl(dwc3_phy->base + LANE0_RX_OVRD_IN_LO);
		reg |= DWC3_PHY_RX_INVERT_OVRD;;
		ltq_dwc3_writel(dwc3_phy->base, LANE0_RX_OVRD_IN_LO, reg);
	}

	return 0;
}

static void ltq_dwc3_phy_shutdown(struct usb_phy *phy)
{
	/*struct ltq_dwc3_phy *dwc3_phy = dev_get_drvdata(phy->dev);*/
}

static int ltq_dwc3_phy_probe(struct platform_device *pdev)
{
	struct ltq_dwc3_phy *dwc3_phy;
	struct device *dev = &pdev->dev;
	struct resource		*res;
	void __iomem		*base;

	dwc3_phy = devm_kzalloc(dev, sizeof(*dwc3_phy), GFP_KERNEL);
	if (!dwc3_phy)
		return -ENOMEM;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "missing memory base resource\n");
		return -EINVAL;
	}

	base = devm_ioremap_nocache(dev, res->start, resource_size(res));
	if (!base) {
		dev_err(dev, "devm_ioremap_nocache failed\n");
		return -ENOMEM;
	}

	dwc3_phy->is_rx_pol_inv = of_property_read_bool(dev->of_node, "invert-rx-polarity");
	dwc3_phy->is_tx_pol_inv = of_property_read_bool(dev->of_node, "invert-tx-polarity");
	dwc3_phy->base = base;
	dwc3_phy->dev = dev;

	dwc3_phy->phy.dev = dev;
	dwc3_phy->phy.label = "ltq-dwc3-phy";
	dwc3_phy->phy.init = ltq_dwc3_phy_init;
	dwc3_phy->phy.shutdown = ltq_dwc3_phy_shutdown;
	dwc3_phy->phy.type = USB_PHY_TYPE_USB3;

	spin_lock_init(&dwc3_phy->lock);
	platform_set_drvdata(pdev, dwc3_phy);
	usb_add_phy_dev(&dwc3_phy->phy);

	return 0;
}

static int ltq_dwc3_phy_remove(struct platform_device *pdev)
{
	struct ltq_dwc3_phy *dwc3_phy = platform_get_drvdata(pdev);

	usb_remove_phy(&dwc3_phy->phy);
	return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id of_ltq_dwc3_phy_match[] = {
	{
		.compatible = "lantiq,dwc3-phy"
	},
	{ },
};
MODULE_DEVICE_TABLE(of, of_ltq_dwc3_phy_match);
#endif

static struct platform_driver ltq_dwc3_phy_driver = {
	.probe          = ltq_dwc3_phy_probe,
	.remove         = ltq_dwc3_phy_remove,
	.driver         = {
		.name   = "ltq-dwc3-phy",
		.owner  = THIS_MODULE,
		.pm = NULL,
		.of_match_table	= of_match_ptr(of_ltq_dwc3_phy_match),
	},
};

module_platform_driver(ltq_dwc3_phy_driver);
MODULE_LICENSE("GPL v2");
