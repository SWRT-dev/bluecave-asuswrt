/*
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License version 2 as published
 *   by the Free Software Foundation.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Copyright (C) 2011-12 John Crispin <blogic@openwrt.org>
 */

#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/interrupt.h>
#include <linux/uaccess.h>
#include <linux/in.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/phy.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/skbuff.h>
#include <linux/mm.h>
#include <linux/platform_device.h>
#include <linux/ethtool.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/dma-mapping.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/of_net.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>

#include <asm/checksum.h>

#include <lantiq_soc.h>
#include <xway_dma.h>
#include <lantiq_platform.h>

#define LTQ_ETOP_MDIO		0x11804
#define MDIO_REQUEST		0x80000000
#define MDIO_READ		0x40000000
#define MDIO_ADDR_MASK		0x1f
#define MDIO_ADDR_OFFSET	0x15
#define MDIO_REG_MASK		0x1f
#define MDIO_REG_OFFSET		0x10
#define MDIO_VAL_MASK		0xffff

#define PPE32_CGEN		0x800
#define LQ_PPE32_ENET_MAC_CFG	0x1840

#define LTQ_ETOP_ENETS0		0x11850
#define LTQ_ETOP_MAC_DA0	0x1186C
#define LTQ_ETOP_MAC_DA1	0x11870
#define LTQ_ETOP_CFG		0x16020
#define LTQ_ETOP_IGPLEN		0x16080

#define MAX_DMA_CHAN		0x8
#define MAX_DMA_CRC_LEN		0x4
#define MAX_DMA_DATA_LEN	0x600

#define ETOP_FTCU		BIT(28)
#define ETOP_MII_MASK		0xf
#define ETOP_MII_NORMAL		0xd
#define ETOP_MII_REVERSE	0xe
#define ETOP_PLEN_UNDER		0x40
#define ETOP_CGEN		0x800
#define ETOP_CFG_MII0		0x01

#define LTQ_GBIT_MDIO_CTL	0xCC
#define LTQ_GBIT_MDIO_DATA	0xd0
#define LTQ_GBIT_GCTL0		0x68
#define LTQ_GBIT_PMAC_HD_CTL	0x8c
#define LTQ_GBIT_P0_CTL		0x4
#define LTQ_GBIT_PMAC_RX_IPG	0xa8
#define LTQ_GBIT_RGMII_CTL	0x78

#define PMAC_HD_CTL_AS		(1 << 19)
#define PMAC_HD_CTL_RXSH	(1 << 22)

/* Switch Enable (0=disable, 1=enable) */
#define GCTL0_SE		0x80000000
/* Disable MDIO auto polling (0=disable, 1=enable) */
#define PX_CTL_DMDIO		0x00400000

/* MDC clock divider, clock = 25MHz/((MDC_CLOCK + 1) * 2) */
#define MDC_CLOCK_MASK		0xff000000
#define MDC_CLOCK_OFFSET	24

/* register information for the gbit's MDIO bus */
#define MDIO_XR9_REQUEST	0x00008000
#define MDIO_XR9_READ		0x00000800
#define MDIO_XR9_WRITE		0x00000400
#define MDIO_XR9_REG_MASK	0x1f
#define MDIO_XR9_ADDR_MASK	0x1f
#define MDIO_XR9_RD_MASK	0xffff
#define MDIO_XR9_REG_OFFSET	0
#define MDIO_XR9_ADDR_OFFSET	5
#define MDIO_XR9_WR_OFFSET	16

#define LTQ_DMA_ETOP	((of_machine_is_compatible("lantiq,ase")) ? \
			(INT_NUM_IM3_IRL0) : (INT_NUM_IM2_IRL0))

/* the newer xway socks have a embedded 3/7 port gbit multiplexer */
#define ltq_etop_r32(x)		ltq_r32(ltq_etop_membase + (x))
#define ltq_etop_w32(x, y)	ltq_w32(x, ltq_etop_membase + (y))
#define ltq_etop_w32_mask(x, y, z)	\
		ltq_w32_mask(x, y, ltq_etop_membase + (z))

#define ltq_gbit_r32(x)		ltq_r32(ltq_gbit_membase + (x))
#define ltq_gbit_w32(x, y)	ltq_w32(x, ltq_gbit_membase + (y))
#define ltq_gbit_w32_mask(x, y, z)	\
		ltq_w32_mask(x, y, ltq_gbit_membase + (z))

#define DRV_VERSION	"1.2"

static void __iomem *ltq_etop_membase;
static void __iomem *ltq_gbit_membase;

struct ltq_etop_chan {
	int tx_free;
	int irq;
	struct net_device *netdev;
	struct napi_struct napi;
	struct ltq_dma_channel dma;
	struct sk_buff *skb[LTQ_DESC_NUM];
};

struct ltq_etop_priv {
	struct net_device *netdev;
	struct platform_device *pdev;
	struct resource *res;

	struct mii_bus *mii_bus;
	struct phy_device *phydev;

	struct ltq_etop_chan txch;
	struct ltq_etop_chan rxch;

	int tx_irq;
	int rx_irq;

	const void *mac;
	int mii_mode;

	spinlock_t lock;

	struct clk *clk_ppe;
	struct clk *clk_switch;
	struct clk *clk_ephy;
	struct clk *clk_ephycgu;
};

static int ltq_etop_mdio_wr(struct mii_bus *bus, int phy_addr,
				int phy_reg, u16 phy_data);

static int
ltq_etop_alloc_skb(struct ltq_etop_chan *ch)
{
	ch->skb[ch->dma.desc] = dev_alloc_skb(MAX_DMA_DATA_LEN);
	if (!ch->skb[ch->dma.desc])
		return -ENOMEM;
	ch->dma.desc_base[ch->dma.desc].addr = dma_map_single(NULL,
		ch->skb[ch->dma.desc]->data, MAX_DMA_DATA_LEN,
		DMA_FROM_DEVICE);
	ch->dma.desc_base[ch->dma.desc].addr =
		CPHYSADDR(ch->skb[ch->dma.desc]->data);
	ch->dma.desc_base[ch->dma.desc].ctl =
		LTQ_DMA_OWN | LTQ_DMA_RX_OFFSET(NET_IP_ALIGN) |
		MAX_DMA_DATA_LEN;
	skb_reserve(ch->skb[ch->dma.desc], NET_IP_ALIGN);
	return 0;
}

static void
ltq_etop_hw_receive(struct ltq_etop_chan *ch)
{
	struct ltq_etop_priv *priv = netdev_priv(ch->netdev);
	struct ltq_dma_desc *desc = &ch->dma.desc_base[ch->dma.desc];
	struct sk_buff *skb = ch->skb[ch->dma.desc];
	int len = (desc->ctl & LTQ_DMA_SIZE_MASK) - MAX_DMA_CRC_LEN;
	unsigned long flags;

	spin_lock_irqsave(&priv->lock, flags);
	if (ltq_etop_alloc_skb(ch)) {
		netdev_err(ch->netdev,
			"failed to allocate new rx buffer, stopping DMA\n");
		ltq_dma_close(&ch->dma);
	}
	ch->dma.desc++;
	ch->dma.desc %= LTQ_DESC_NUM;
	spin_unlock_irqrestore(&priv->lock, flags);

	skb_put(skb, len);
	skb->dev = ch->netdev;
	skb->protocol = eth_type_trans(skb, ch->netdev);
	netif_receive_skb(skb);
	ch->netdev->stats.rx_packets++;
	ch->netdev->stats.rx_bytes += len;
}

static int
ltq_etop_poll_rx(struct napi_struct *napi, int budget)
{
	struct ltq_etop_chan *ch = container_of(napi,
				struct ltq_etop_chan, napi);
	struct ltq_etop_priv *priv = netdev_priv(ch->netdev);
	int rx = 0;
	int complete = 0;
	unsigned long flags;

	while ((rx < budget) && !complete) {
		struct ltq_dma_desc *desc = &ch->dma.desc_base[ch->dma.desc];

		if ((desc->ctl & (LTQ_DMA_OWN | LTQ_DMA_C)) == LTQ_DMA_C) {
			ltq_etop_hw_receive(ch);
			rx++;
		} else {
			complete = 1;
		}
	}
	if (complete || !rx) {
		napi_complete(&ch->napi);
		spin_lock_irqsave(&priv->lock, flags);
		ltq_dma_ack_irq(&ch->dma);
		spin_unlock_irqrestore(&priv->lock, flags);
	}
	return rx;
}

static int
ltq_etop_poll_tx(struct napi_struct *napi, int budget)
{
	struct ltq_etop_chan *ch =
		container_of(napi, struct ltq_etop_chan, napi);
	struct ltq_etop_priv *priv = netdev_priv(ch->netdev);
	struct netdev_queue *txq =
		netdev_get_tx_queue(ch->netdev, ch->dma.nr >> 1);
	unsigned long flags;

	spin_lock_irqsave(&priv->lock, flags);
	while ((ch->dma.desc_base[ch->tx_free].ctl &
			(LTQ_DMA_OWN | LTQ_DMA_C)) == LTQ_DMA_C) {
		ch->netdev->stats.tx_packets++;
		ch->netdev->stats.tx_bytes += ch->skb[ch->tx_free]->len;
		dev_kfree_skb_any(ch->skb[ch->tx_free]);
		ch->skb[ch->tx_free] = NULL;
		memset(&ch->dma.desc_base[ch->tx_free], 0,
			sizeof(struct ltq_dma_desc));
		ch->tx_free++;
		ch->tx_free %= LTQ_DESC_NUM;
	}
	spin_unlock_irqrestore(&priv->lock, flags);

	if (netif_tx_queue_stopped(txq))
		netif_tx_start_queue(txq);
	napi_complete(&ch->napi);
	spin_lock_irqsave(&priv->lock, flags);
	ltq_dma_ack_irq(&ch->dma);
	spin_unlock_irqrestore(&priv->lock, flags);
	return 1;
}

static irqreturn_t
ltq_etop_dma_irq(int irq, void *_priv)
{
	struct ltq_etop_priv *priv = _priv;
	if (irq == priv->txch.dma.irq)
		napi_schedule(&priv->txch.napi);
	else
		napi_schedule(&priv->rxch.napi);
	return IRQ_HANDLED;
}

static void
ltq_etop_free_channel(struct net_device *dev, struct ltq_etop_chan *ch)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);

	ltq_dma_free(&ch->dma);
	if (ch->dma.irq)
		free_irq(ch->dma.irq, priv);
	if (ch == &priv->txch) {
		int desc;
		for (desc = 0; desc < LTQ_DESC_NUM; desc++)
			dev_kfree_skb_any(ch->skb[ch->dma.desc]);
	}
}

static void
ltq_etop_hw_exit(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);

	clk_disable(priv->clk_ppe);

	if (of_machine_is_compatible("lantiq,ar9"))
		clk_disable(priv->clk_switch);

	if (of_machine_is_compatible("lantiq,ase")) {
		clk_disable(priv->clk_ephy);
		clk_disable(priv->clk_ephycgu);
	}

	ltq_etop_free_channel(dev, &priv->txch);
	ltq_etop_free_channel(dev, &priv->rxch);
}

static void
ltq_etop_gbit_init(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);

	clk_enable(priv->clk_switch);

	/* enable gbit port0 on the SoC */
	ltq_gbit_w32_mask((1 << 17), (1 << 18), LTQ_GBIT_P0_CTL);

	ltq_gbit_w32_mask(0, GCTL0_SE, LTQ_GBIT_GCTL0);
	/* disable MDIO auto polling mode */
	ltq_gbit_w32_mask(0, PX_CTL_DMDIO, LTQ_GBIT_P0_CTL);
	/* set 1522 packet size */
	ltq_gbit_w32_mask(0x300, 0, LTQ_GBIT_GCTL0);
	/* disable pmac & dmac headers */
	ltq_gbit_w32_mask(PMAC_HD_CTL_AS | PMAC_HD_CTL_RXSH, 0,
		LTQ_GBIT_PMAC_HD_CTL);
	/* Due to traffic halt when burst length 8,
		replace default IPG value with 0x3B */
	ltq_gbit_w32(0x3B, LTQ_GBIT_PMAC_RX_IPG);
	/* set mdc clock to 2.5 MHz */
	ltq_gbit_w32_mask(MDC_CLOCK_MASK, 4 << MDC_CLOCK_OFFSET,
		LTQ_GBIT_RGMII_CTL);
}

static int
ltq_etop_hw_init(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);
	int mii_mode = priv->mii_mode;

	clk_enable(priv->clk_ppe);

	if (of_machine_is_compatible("lantiq,ar9")) {
		ltq_etop_gbit_init(dev);
		/* force the etops link to the gbit to MII */
		mii_mode = PHY_INTERFACE_MODE_MII;
	}

	switch (mii_mode) {
	case PHY_INTERFACE_MODE_RMII:
		ltq_etop_w32_mask(ETOP_MII_MASK,
			ETOP_MII_REVERSE, LTQ_ETOP_CFG);
		break;

	case PHY_INTERFACE_MODE_MII:
		ltq_etop_w32_mask(ETOP_MII_MASK,
			ETOP_MII_NORMAL, LTQ_ETOP_CFG);
		break;

	default:
		if (of_machine_is_compatible("lantiq,ase")) {
			clk_enable(priv->clk_ephy);
			/* disable external MII */
			ltq_etop_w32_mask(0, ETOP_CFG_MII0, LTQ_ETOP_CFG);
			/* enable clock for internal PHY */
			clk_enable(priv->clk_ephycgu);
			/* we need to write this magic to the internal phy to
			   make it work */
			ltq_etop_mdio_wr(NULL, 0x8, 0x12, 0xC020);
			pr_info("Selected EPHY mode\n");
			break;
		}
		netdev_err(dev, "unknown mii mode %d\n",
			mii_mode);
		return -ENOTSUPP;
	}

	/* enable crc generation */
	ltq_etop_w32(PPE32_CGEN, LQ_PPE32_ENET_MAC_CFG);

	return 0;
}

static int
ltq_etop_dma_init(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);
	int tx = priv->tx_irq - LTQ_DMA_ETOP;
	int rx = priv->rx_irq - LTQ_DMA_ETOP;
	int err;

	ltq_dma_init_port(DMA_PORT_ETOP);

	priv->txch.dma.nr = tx;
	ltq_dma_alloc_tx(&priv->txch.dma);
	err = request_irq(priv->tx_irq, ltq_etop_dma_irq, IRQF_DISABLED,
		"eth_tx", priv);
	if (err) {
		netdev_err(dev, "failed to allocate tx irq\n");
		goto err_out;
	}
	priv->txch.dma.irq = priv->tx_irq;

	priv->rxch.dma.nr = rx;
	ltq_dma_alloc_rx(&priv->rxch.dma);
	for (priv->rxch.dma.desc = 0; priv->rxch.dma.desc < LTQ_DESC_NUM;
			priv->rxch.dma.desc++) {
		if (ltq_etop_alloc_skb(&priv->rxch)) {
			netdev_err(dev, "failed to allocate skbs\n");
			err = -ENOMEM;
			goto err_out;
		}
	}
	priv->rxch.dma.desc = 0;
	err = request_irq(priv->rx_irq, ltq_etop_dma_irq, IRQF_DISABLED,
		"eth_rx", priv);
	if (err)
		netdev_err(dev, "failed to allocate rx irq\n");
	else
		priv->rxch.dma.irq = priv->rx_irq;
err_out:
	return err;
}

static void
ltq_etop_get_drvinfo(struct net_device *dev, struct ethtool_drvinfo *info)
{
	strlcpy(info->driver, "Lantiq ETOP", sizeof(info->driver));
	strlcpy(info->bus_info, "internal", sizeof(info->bus_info));
	strlcpy(info->version, DRV_VERSION, sizeof(info->version));
}

static int
ltq_etop_get_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);

	if (priv->phydev)
		return phy_ethtool_gset(priv->phydev, cmd);
	else
		return 0;
}

static int
ltq_etop_set_settings(struct net_device *dev, struct ethtool_cmd *cmd)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);

	if (priv->phydev)
		return phy_ethtool_sset(priv->phydev, cmd);
	else
		return 0;
}

static int
ltq_etop_nway_reset(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);

	if (priv->phydev)
		return phy_start_aneg(priv->phydev);
	else
		return 0;
}

static const struct ethtool_ops ltq_etop_ethtool_ops = {
	.get_drvinfo = ltq_etop_get_drvinfo,
	.get_settings = ltq_etop_get_settings,
	.set_settings = ltq_etop_set_settings,
	.nway_reset = ltq_etop_nway_reset,
};

static int
ltq_etop_mdio_wr_xr9(struct mii_bus *bus, int phy_addr,
		int phy_reg, u16 phy_data)
{
	u32 val = MDIO_XR9_REQUEST | MDIO_XR9_WRITE |
		(phy_data << MDIO_XR9_WR_OFFSET) |
		((phy_addr & MDIO_XR9_ADDR_MASK) << MDIO_XR9_ADDR_OFFSET) |
		((phy_reg & MDIO_XR9_REG_MASK) << MDIO_XR9_REG_OFFSET);

	while (ltq_gbit_r32(LTQ_GBIT_MDIO_CTL) & MDIO_XR9_REQUEST)
		;
	ltq_gbit_w32(val, LTQ_GBIT_MDIO_CTL);
	while (ltq_gbit_r32(LTQ_GBIT_MDIO_CTL) & MDIO_XR9_REQUEST)
		;
	return 0;
}

static int
ltq_etop_mdio_rd_xr9(struct mii_bus *bus, int phy_addr, int phy_reg)
{
	u32 val = MDIO_XR9_REQUEST | MDIO_XR9_READ |
		((phy_addr & MDIO_XR9_ADDR_MASK) << MDIO_XR9_ADDR_OFFSET) |
		((phy_reg & MDIO_XR9_REG_MASK) << MDIO_XR9_REG_OFFSET);

	while (ltq_gbit_r32(LTQ_GBIT_MDIO_CTL) & MDIO_XR9_REQUEST)
		;
	ltq_gbit_w32(val, LTQ_GBIT_MDIO_CTL);
	while (ltq_gbit_r32(LTQ_GBIT_MDIO_CTL) & MDIO_XR9_REQUEST)
		;
	val = ltq_gbit_r32(LTQ_GBIT_MDIO_DATA) & MDIO_XR9_RD_MASK;
	return val;
}

static int
ltq_etop_mdio_wr(struct mii_bus *bus, int phy_addr, int phy_reg, u16 phy_data)
{
	u32 val = MDIO_REQUEST |
		((phy_addr & MDIO_ADDR_MASK) << MDIO_ADDR_OFFSET) |
		((phy_reg & MDIO_REG_MASK) << MDIO_REG_OFFSET) |
		phy_data;

	while (ltq_etop_r32(LTQ_ETOP_MDIO) & MDIO_REQUEST)
		;
	ltq_etop_w32(val, LTQ_ETOP_MDIO);
	return 0;
}

static int
ltq_etop_mdio_rd(struct mii_bus *bus, int phy_addr, int phy_reg)
{
	u32 val = MDIO_REQUEST | MDIO_READ |
		((phy_addr & MDIO_ADDR_MASK) << MDIO_ADDR_OFFSET) |
		((phy_reg & MDIO_REG_MASK) << MDIO_REG_OFFSET);

	while (ltq_etop_r32(LTQ_ETOP_MDIO) & MDIO_REQUEST)
		;
	ltq_etop_w32(val, LTQ_ETOP_MDIO);
	while (ltq_etop_r32(LTQ_ETOP_MDIO) & MDIO_REQUEST)
		;
	val = ltq_etop_r32(LTQ_ETOP_MDIO) & MDIO_VAL_MASK;
	return val;
}

static void
ltq_etop_mdio_link(struct net_device *dev)
{
	/* nothing to do  */
}

static int
ltq_etop_mdio_probe(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);
	struct phy_device *phydev = NULL;
	u32 phy_supported =  (SUPPORTED_10baseT_Half
			| SUPPORTED_10baseT_Full
			| SUPPORTED_100baseT_Half
			| SUPPORTED_100baseT_Full
			| SUPPORTED_Autoneg
			| SUPPORTED_MII
			| SUPPORTED_TP);

	if (of_machine_is_compatible("lantiq,ase"))
		phydev = priv->mii_bus->phy_map[8];
	else
		phydev = priv->mii_bus->phy_map[0];

	if (!phydev) {
		netdev_err(dev, "no PHY found\n");
		return -ENODEV;
	}

	phydev = phy_connect(dev, dev_name(&phydev->dev),
			&ltq_etop_mdio_link, priv->mii_mode);

	if (IS_ERR(phydev)) {
		netdev_err(dev, "Could not attach to PHY\n");
		return PTR_ERR(phydev);
	}

	if (of_machine_is_compatible("lantiq,ar9"))
		phy_supported |= SUPPORTED_1000baseT_Half
			| SUPPORTED_1000baseT_Full;

	phydev->supported &= phy_supported;
	phydev->advertising = phydev->supported;
	priv->phydev = phydev;
	pr_info("%s: attached PHY [%s] (phy_addr=%s, irq=%d)\n",
	       dev->name, phydev->drv->name,
	       dev_name(&phydev->dev), phydev->irq);

	return 0;
}

static int
ltq_etop_mdio_init(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);
	int i;
	int err;

	priv->mii_bus = mdiobus_alloc();
	if (!priv->mii_bus) {
		netdev_err(dev, "failed to allocate mii bus\n");
		err = -ENOMEM;
		goto err_out;
	}

	priv->mii_bus->priv = dev;
	if (of_machine_is_compatible("lantiq,ar9")) {
		priv->mii_bus->read = ltq_etop_mdio_rd_xr9;
		priv->mii_bus->write = ltq_etop_mdio_wr_xr9;
	} else {
		priv->mii_bus->read = ltq_etop_mdio_rd;
		priv->mii_bus->write = ltq_etop_mdio_wr;
	}
	priv->mii_bus->name = "ltq_mii";
	snprintf(priv->mii_bus->id, MII_BUS_ID_SIZE, "%s-%x",
		priv->pdev->name, priv->pdev->id);
	priv->mii_bus->irq = kmalloc(sizeof(int) * PHY_MAX_ADDR, GFP_KERNEL);
	if (!priv->mii_bus->irq) {
		err = -ENOMEM;
		goto err_out_free_mdiobus;
	}

	for (i = 0; i < PHY_MAX_ADDR; ++i)
		priv->mii_bus->irq[i] = PHY_POLL;

	if (mdiobus_register(priv->mii_bus)) {
		err = -ENXIO;
		goto err_out_free_mdio_irq;
	}

	if (ltq_etop_mdio_probe(dev)) {
		err = -ENXIO;
		goto err_out_unregister_bus;
	}
	return 0;

err_out_unregister_bus:
	mdiobus_unregister(priv->mii_bus);
err_out_free_mdio_irq:
	kfree(priv->mii_bus->irq);
err_out_free_mdiobus:
	mdiobus_free(priv->mii_bus);
err_out:
	return err;
}

static void
ltq_etop_mdio_cleanup(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);

	phy_disconnect(priv->phydev);
	mdiobus_unregister(priv->mii_bus);
	kfree(priv->mii_bus->irq);
	mdiobus_free(priv->mii_bus);
}

static int
ltq_etop_open(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);
	unsigned long flags;

	napi_enable(&priv->txch.napi);
	napi_enable(&priv->rxch.napi);

	spin_lock_irqsave(&priv->lock, flags);
	ltq_dma_open(&priv->txch.dma);
	ltq_dma_open(&priv->rxch.dma);
	spin_unlock_irqrestore(&priv->lock, flags);

	if (priv->phydev)
		phy_start(priv->phydev);

	netif_tx_start_all_queues(dev);
	return 0;
}

static int
ltq_etop_stop(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);
	unsigned long flags;

	netif_tx_stop_all_queues(dev);
	if (priv->phydev)
		phy_stop(priv->phydev);
	napi_disable(&priv->txch.napi);
	napi_disable(&priv->rxch.napi);

	spin_lock_irqsave(&priv->lock, flags);
	ltq_dma_close(&priv->txch.dma);
	ltq_dma_close(&priv->rxch.dma);
	spin_unlock_irqrestore(&priv->lock, flags);

	return 0;
}

static int
ltq_etop_tx(struct sk_buff *skb, struct net_device *dev)
{
	int queue = skb_get_queue_mapping(skb);
	struct netdev_queue *txq = netdev_get_tx_queue(dev, queue);
	struct ltq_etop_priv *priv = netdev_priv(dev);
	struct ltq_dma_desc *desc =
		&priv->txch.dma.desc_base[priv->txch.dma.desc];
	unsigned long flags;
	u32 byte_offset;
	int len;

	len = skb->len < ETH_ZLEN ? ETH_ZLEN : skb->len;

	if ((desc->ctl & (LTQ_DMA_OWN | LTQ_DMA_C)) ||
			priv->txch.skb[priv->txch.dma.desc]) {
		netdev_err(dev, "tx ring full\n");
		netif_tx_stop_queue(txq);
		return NETDEV_TX_BUSY;
	}

	/* dma needs to start on a 16 byte aligned address */
	byte_offset = CPHYSADDR(skb->data) % 16;
	priv->txch.skb[priv->txch.dma.desc] = skb;

	dev->trans_start = jiffies;

	spin_lock_irqsave(&priv->lock, flags);
	desc->addr = ((unsigned int) dma_map_single(NULL, skb->data, len,
						DMA_TO_DEVICE)) - byte_offset;
	wmb();
	desc->ctl = LTQ_DMA_OWN | LTQ_DMA_SOP | LTQ_DMA_EOP |
		LTQ_DMA_TX_OFFSET(byte_offset) | (len & LTQ_DMA_SIZE_MASK);
	priv->txch.dma.desc++;
	priv->txch.dma.desc %= LTQ_DESC_NUM;
	spin_unlock_irqrestore(&priv->lock, flags);

	if (priv->txch.dma.desc_base[priv->txch.dma.desc].ctl & LTQ_DMA_OWN)
		netif_tx_stop_queue(txq);

	return NETDEV_TX_OK;
}

static int
ltq_etop_change_mtu(struct net_device *dev, int new_mtu)
{
	int ret = eth_change_mtu(dev, new_mtu);

	if (!ret) {
		struct ltq_etop_priv *priv = netdev_priv(dev);
		unsigned long flags;

		spin_lock_irqsave(&priv->lock, flags);
		ltq_etop_w32((ETOP_PLEN_UNDER << 16) | new_mtu,
			LTQ_ETOP_IGPLEN);
		spin_unlock_irqrestore(&priv->lock, flags);
	}
	return ret;
}

static int
ltq_etop_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);

	/* TODO: mii-toll reports "No MII transceiver present!." ?!*/
	return phy_mii_ioctl(priv->phydev, rq, cmd);
}

static int
ltq_etop_set_mac_address(struct net_device *dev, void *p)
{
	int ret = eth_mac_addr(dev, p);

	if (!ret) {
		struct ltq_etop_priv *priv = netdev_priv(dev);
		unsigned long flags;

		/* store the mac for the unicast filter */
		spin_lock_irqsave(&priv->lock, flags);
		ltq_etop_w32(*((u32 *)dev->dev_addr), LTQ_ETOP_MAC_DA0);
		ltq_etop_w32(*((u16 *)&dev->dev_addr[4]) << 16,
			LTQ_ETOP_MAC_DA1);
		spin_unlock_irqrestore(&priv->lock, flags);
	}
	return ret;
}

static void
ltq_etop_set_multicast_list(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);
	unsigned long flags;

	/* ensure that the unicast filter is not enabled in promiscious mode */
	spin_lock_irqsave(&priv->lock, flags);
	if ((dev->flags & IFF_PROMISC) || (dev->flags & IFF_ALLMULTI))
		ltq_etop_w32_mask(ETOP_FTCU, 0, LTQ_ETOP_ENETS0);
	else
		ltq_etop_w32_mask(0, ETOP_FTCU, LTQ_ETOP_ENETS0);
	spin_unlock_irqrestore(&priv->lock, flags);
}

static u16
ltq_etop_select_queue(struct net_device *dev, struct sk_buff *skb)
{
	/* we are currently only using the first queue */
	return 0;
}

static int
ltq_etop_init(struct net_device *dev)
{
	struct ltq_etop_priv *priv = netdev_priv(dev);
	struct sockaddr mac;
	int err;

	ether_setup(dev);
	dev->watchdog_timeo = 10 * HZ;
	err = ltq_etop_hw_init(dev);
	if (err)
		goto err_hw;
	err = ltq_etop_dma_init(dev);
	if (err)
		goto err_hw;

	ltq_etop_change_mtu(dev, 1500);

	if (priv->mac)
		memcpy(&mac.sa_data, priv->mac, ETH_ALEN);
	if (!is_valid_ether_addr(mac.sa_data)) {
		pr_warn("etop: invalid MAC, using random\n");
		random_ether_addr(mac.sa_data);
	}

	err = ltq_etop_set_mac_address(dev, &mac);
	if (err)
		goto err_netdev;
	ltq_etop_set_multicast_list(dev);
	if (!ltq_etop_mdio_init(dev))
		dev->ethtool_ops = &ltq_etop_ethtool_ops;
	else
		pr_warn("etop: mdio probe failed\n");;
	return 0;

err_netdev:
	unregister_netdev(dev);
	free_netdev(dev);
err_hw:
	ltq_etop_hw_exit(dev);
	return err;
}

static void
ltq_etop_tx_timeout(struct net_device *dev)
{
	int err;

	ltq_etop_hw_exit(dev);
	err = ltq_etop_hw_init(dev);
	if (err)
		goto err_hw;
	err = ltq_etop_dma_init(dev);
	if (err)
		goto err_hw;
	dev->trans_start = jiffies;
	netif_wake_queue(dev);
	return;

err_hw:
	ltq_etop_hw_exit(dev);
	netdev_err(dev, "failed to restart etop after TX timeout\n");
}

static const struct net_device_ops ltq_eth_netdev_ops = {
	.ndo_open = ltq_etop_open,
	.ndo_stop = ltq_etop_stop,
	.ndo_start_xmit = ltq_etop_tx,
	.ndo_change_mtu = ltq_etop_change_mtu,
	.ndo_do_ioctl = ltq_etop_ioctl,
	.ndo_set_mac_address = ltq_etop_set_mac_address,
	.ndo_validate_addr = eth_validate_addr,
	.ndo_set_rx_mode = ltq_etop_set_multicast_list,
	.ndo_select_queue = ltq_etop_select_queue,
	.ndo_init = ltq_etop_init,
	.ndo_tx_timeout = ltq_etop_tx_timeout,
};

static int ltq_etop_probe(struct platform_device *pdev)
{
	struct net_device *dev;
	struct ltq_etop_priv *priv;
	struct resource *res, *gbit_res, irqres[2];
	int err;

	err = of_irq_to_resource_table(pdev->dev.of_node, irqres, 2);
	if (err != 2) {
		dev_err(&pdev->dev, "failed to get etop irqs\n");
		return -EINVAL;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "failed to get etop resource\n");
		err = -ENOENT;
		goto err_out;
	}

	res = devm_request_mem_region(&pdev->dev, res->start,
		resource_size(res), dev_name(&pdev->dev));
	if (!res) {
		dev_err(&pdev->dev, "failed to request etop resource\n");
		err = -EBUSY;
		goto err_out;
	}

	ltq_etop_membase = devm_ioremap_nocache(&pdev->dev,
		res->start, resource_size(res));
	if (!ltq_etop_membase) {
		dev_err(&pdev->dev, "failed to remap etop engine %d\n",
			pdev->id);
		err = -ENOMEM;
		goto err_out;
	}

	if (of_machine_is_compatible("lantiq,ar9")) {
		gbit_res = platform_get_resource(pdev, IORESOURCE_MEM, 1);
		if (!gbit_res) {
			dev_err(&pdev->dev, "failed to get gbit resource\n");
			err = -ENOENT;
			goto err_out;
		}
		ltq_gbit_membase = devm_ioremap_nocache(&pdev->dev,
			gbit_res->start, resource_size(gbit_res));
		if (!ltq_gbit_membase) {
			dev_err(&pdev->dev, "failed to remap gigabit switch %d\n",
				pdev->id);
			err = -ENOMEM;
			goto err_out;
		}
	}

	dev = alloc_etherdev_mq(sizeof(struct ltq_etop_priv), 4);
	strcpy(dev->name, "eth%d");
	dev->netdev_ops = &ltq_eth_netdev_ops;
	priv = netdev_priv(dev);
	priv->res = res;
	priv->pdev = pdev;
	priv->netdev = dev;
	priv->tx_irq = irqres[0].start;
	priv->rx_irq = irqres[1].start;
	priv->mii_mode = of_get_phy_mode(pdev->dev.of_node);
	of_get_mac_address_mtd(pdev->dev.of_node, priv->mac);

	priv->clk_ppe = clk_get(&pdev->dev, NULL);
	if (IS_ERR(priv->clk_ppe))
		return PTR_ERR(priv->clk_ppe);
	if (of_machine_is_compatible("lantiq,ar9")) {
		priv->clk_switch = clk_get(&pdev->dev, "switch");
		if (IS_ERR(priv->clk_switch))
			return PTR_ERR(priv->clk_switch);
	}
	if (of_machine_is_compatible("lantiq,ase")) {
		priv->clk_ephy = clk_get(&pdev->dev, "ephy");
		if (IS_ERR(priv->clk_ephy))
			return PTR_ERR(priv->clk_ephy);
		priv->clk_ephycgu = clk_get(&pdev->dev, "ephycgu");
		if (IS_ERR(priv->clk_ephycgu))
			return PTR_ERR(priv->clk_ephycgu);
	}

	spin_lock_init(&priv->lock);

	netif_napi_add(dev, &priv->txch.napi, ltq_etop_poll_tx, 8);
	netif_napi_add(dev, &priv->rxch.napi, ltq_etop_poll_rx, 32);
	priv->txch.netdev = dev;
	priv->rxch.netdev = dev;

	err = register_netdev(dev);
	if (err)
		goto err_free;

	platform_set_drvdata(pdev, dev);
	return 0;

err_free:
	free_netdev(dev);
err_out:
	return err;
}

static int
ltq_etop_remove(struct platform_device *pdev)
{
	struct net_device *dev = platform_get_drvdata(pdev);

	if (dev) {
		netif_tx_stop_all_queues(dev);
		ltq_etop_hw_exit(dev);
		ltq_etop_mdio_cleanup(dev);
		unregister_netdev(dev);
	}
	return 0;
}

static const struct of_device_id ltq_etop_match[] = {
	{ .compatible = "lantiq,etop-xway" },
	{},
};
MODULE_DEVICE_TABLE(of, ltq_etop_match);

static struct platform_driver ltq_mii_driver = {
	.probe = ltq_etop_probe,
	.remove = ltq_etop_remove,
	.driver = {
		.name = "ltq_etop",
		.owner = THIS_MODULE,
		.of_match_table = ltq_etop_match,
	},
};

module_platform_driver(ltq_mii_driver);

MODULE_AUTHOR("John Crispin <blogic@openwrt.org>");
MODULE_DESCRIPTION("Lantiq SoC ETOP");
MODULE_LICENSE("GPL");
