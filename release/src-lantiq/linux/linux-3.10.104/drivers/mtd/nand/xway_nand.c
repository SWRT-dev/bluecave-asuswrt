/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright © 2012 John Crispin <blogic@openwrt.org>
 *  Copyright (c) 2013 Mohammad Firdaus B Alias Thani  <m.aliasthani@lantiq.com>
 *	Bug-fix to allow write to flash possible
 *	including minor hardware behaviour adaptation fixes
 */

#include <linux/mtd/nand.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <lantiq_soc.h>

/* nand registers */
#define EBU_ADDSEL0		0x20
#define EBU_ADDSEL1		0x24
#define EBU_NAND_CON		0xB0
#define EBU_NAND_WAIT		0xB4
#define EBU_NAND_ECC0		0xB8
#define EBU_NAND_ECC_AC		0xBC

/* nand commands */
#define NAND_CMD_ALE		(1 << 2)
#define NAND_CMD_CLE		(1 << 3)
#define NAND_CMD_CS		(1 << 4)
#define NAND_WRITE_CMD_RESET	0xff
#define NAND_WRITE_CMD		(NAND_CMD_CS | NAND_CMD_CLE)
#define NAND_WRITE_ADDR		(NAND_CMD_CS | NAND_CMD_ALE)
#define NAND_WRITE_DATA		(NAND_CMD_CS)
#define NAND_READ_DATA		(NAND_CMD_CS)
#define NAND_WAIT_WR_C		(1 << 3)
#define NAND_WAIT_RD		(0x1)

/* we need to tel the ebu which addr we mapped the nand to */
#define ADDSEL1_MASK(x)		(x << 4)
#define ADDSEL0_REGEN		1
#define ADDSEL1_REGEN		1

/* we need to tell the EBU that we have nand attached and set it up properly */
#define BUSCON1_SETUP		(1 << 22)
#define BUSCON1_BCGEN_RES	(0x3 << 12)
#define BUSCON1_WAITWRC2	(2 << 8)
#define BUSCON1_WAITRDC2	(2 << 6)
#define BUSCON1_HOLDC1		(1 << 4)
#define BUSCON1_RECOVC1		(1 << 2)
#define BUSCON1_CMULT4		1

#define BUSCON0_SETUP		(1 << 22)
#define BUSCON0_ALEC		(2 << 14)
#define BUSCON0_BCGEN_RES	(0x3 << 12)
#define BUSCON0_WAITWRC2	(7 << 8)
#define BUSCON0_WAITRDC2	(3 << 6)
#define BUSCON0_HOLDC1		(3 << 4)
#define BUSCON0_RECOVC1		(3 << 2)
#define BUSCON0_CMULT4		2

#define NAND_CON_CE		(1 << 20)
#define NAND_CON_OUT_CS1	(1 << 10)
#define NAND_CON_IN_CS1		(1 << 8)
#define NAND_CON_PRE_P		(1 << 7)
#define NAND_CON_WP_P		(1 << 6)
#define NAND_CON_SE_P		(1 << 5)
#define NAND_CON_CS_P		(1 << 4)
#define NAND_CON_CSMUX		(1 << 1)
#define NAND_CON_NANDM		1

#define NAND_ALE_SET		ltq_ebu_w32(ltq_ebu_r32(EBU_NAND_CON) | \
							(1 << 18), EBU_NAND_CON);
#define NAND_ALE_CLEAR		ltq_ebu_w32(ltq_ebu_r32(EBU_NAND_CON) & \
							~(1 << 18), EBU_NAND_CON);

#ifndef CONFIG_EVA
#define NANDPHYSADDR(x)    CPHYSADDR(x)
#else
#define NANDPHYSADDR(x) RPHYSADDR(x)
#endif /* CONFIG_EVA */

#ifdef CONFIG_LTQ_BCH_4BITS
#define ECC_BYTES	7 
#else
#define ECC_BYTES	13
#endif /* CONFIG_LTQ_BCH_4BITS */

static u32 xway_latchcmd;

static void xway_reset_chip(struct nand_chip *chip)
{
	unsigned long nandaddr = (unsigned long) chip->IO_ADDR_W;
	unsigned long timeout;
	unsigned long flags;

	nandaddr &= ~NAND_WRITE_ADDR;
	nandaddr |= NAND_WRITE_CMD;

	/* finish with a reset */
	timeout = jiffies + msecs_to_jiffies(20);

	spin_lock_irqsave(&ebu_lock, flags);
	writeb(NAND_WRITE_CMD_RESET, (void __iomem *) nandaddr);
	spin_unlock_irqrestore(&ebu_lock, flags);
	do {
		if ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			break;
		cond_resched();
	} while (!time_after_eq(jiffies, timeout));
}

static void xway_select_chip(struct mtd_info *mtd, int chip)
{

	switch (chip) {
	case -1:
		ltq_ebu_w32_mask(NAND_CON_CE, 0, EBU_NAND_CON);
		ltq_ebu_w32_mask(NAND_CON_NANDM, 0, EBU_NAND_CON);
		break;
	case 0:
		ltq_ebu_w32_mask(0, NAND_CON_NANDM, EBU_NAND_CON);
		ltq_ebu_w32_mask(0, NAND_CON_CE, EBU_NAND_CON);
		break;
	default:
		BUG();
	}
}

static void xway_cmd_ctrl(struct mtd_info *mtd, int cmd, unsigned int ctrl)
{
	struct nand_chip *this = mtd->priv;
	unsigned long nandaddr = (unsigned long) this->IO_ADDR_W;
	unsigned long flags;

	if (ctrl & NAND_CTRL_CHANGE) {
		if (ctrl & NAND_CLE) {
			NAND_ALE_CLEAR;
			xway_latchcmd = NAND_WRITE_CMD;
		} else if (ctrl & NAND_ALE) {
			NAND_ALE_SET;
			xway_latchcmd = NAND_WRITE_ADDR;
		} else {
			if (xway_latchcmd == NAND_WRITE_ADDR) {
				NAND_ALE_CLEAR;
				xway_latchcmd = NAND_WRITE_DATA;
			}
		}
	}

	if (cmd != NAND_CMD_NONE) {
		spin_lock_irqsave(&ebu_lock, flags);
		writeb(cmd, (void __iomem *) (nandaddr | xway_latchcmd));
		while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			;
		spin_unlock_irqrestore(&ebu_lock, flags);
	}
}

static int xway_dev_ready(struct mtd_info *mtd)
{
	return ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_RD;
}

static unsigned char xway_read_byte(struct mtd_info *mtd)
{
	struct nand_chip *this = mtd->priv;
	unsigned long nandaddr = (unsigned long) this->IO_ADDR_R;
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&ebu_lock, flags);
	ret = ltq_r8((void __iomem *)(nandaddr | NAND_READ_DATA));
	while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
	;
	spin_unlock_irqrestore(&ebu_lock, flags);

	return ret;
}

static void xway_read_buf(struct mtd_info *mtd, u_char *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	unsigned long nandaddr = (unsigned long) this->IO_ADDR_R;
	unsigned long flags;
	int i;

	spin_lock_irqsave(&ebu_lock, flags);
	for (i = 0; i < len; i++) {
		buf[i] = ltq_r8((void __iomem *)(nandaddr | NAND_READ_DATA));
		while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			;
	}
	spin_unlock_irqrestore(&ebu_lock, flags);
}

static void xway_write_buf(struct mtd_info *mtd, const u_char *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	unsigned long nandaddr = (unsigned long) this->IO_ADDR_W;
	unsigned long flags;
	int i;

	spin_lock_irqsave(&ebu_lock, flags);
	for (i = 0; i < len; i++) {
		ltq_w8(buf[i], (void __iomem *) (nandaddr | NAND_WRITE_DATA));
		while ((ltq_ebu_r32(EBU_NAND_WAIT) & NAND_WAIT_WR_C) == 0)
			;
	}
	spin_unlock_irqrestore(&ebu_lock, flags);
}

static int xway_nand_probe(struct platform_device *pdev)
{
	struct nand_chip *this = platform_get_drvdata(pdev);
	unsigned long nandaddr = (unsigned long) this->IO_ADDR_W;
	const __be32 *cs = of_get_property(pdev->dev.of_node,
					"lantiq,cs", NULL);
	u32 cs_flag = 0;

	/* load our CS from the DT. Either we find a valid 1 or default to 0 */
	if (cs && (*cs == 1)) {
		cs_flag = NAND_CON_IN_CS1 | NAND_CON_OUT_CS1;
	}

#if defined(CONFIG_LTQ_BCH_MODE)
	this->ecc.mode = NAND_ECC_SOFT_BCH;
	this->ecc.size = 512;
	this->ecc.bytes = ECC_BYTES;
	this->options |= NAND_NO_SUBPAGE_WRITE;
#elif defined (CONFIG_MTD_NAND_BENAND)
	this->ecc.mode = NAND_ECC_BENAND;
#else
	this->ecc.mode = NAND_ECC_SOFT;
#endif

	/* setup the EBU to run in NAND mode on our base addr for different CS */
	if (cs && (*cs  == 1)) {
		if (of_machine_is_compatible("lantiq,vr9")) {
			ltq_ebu_w32(NANDPHYSADDR(nandaddr)
				| ADDSEL1_MASK(3) | ADDSEL1_REGEN, EBU_ADDSEL1);
		} else if (of_machine_is_compatible("lantiq,grx500")) {
			ltq_ebu_w32(NANDPHYSADDR(nandaddr)
                | ADDSEL1_MASK(5) | ADDSEL1_REGEN, EBU_ADDSEL1);
		} else {
			ltq_ebu_w32(NANDPHYSADDR(nandaddr)
				| ADDSEL1_MASK(2) | ADDSEL1_REGEN, EBU_ADDSEL1);
		}

		ltq_ebu_w32(BUSCON1_SETUP | BUSCON1_BCGEN_RES | BUSCON1_WAITWRC2
			| BUSCON1_WAITRDC2 | BUSCON1_HOLDC1 | BUSCON1_RECOVC1
			| BUSCON1_CMULT4, LTQ_EBU_BUSCON1);

		ltq_ebu_w32(NAND_CON_NANDM | NAND_CON_CSMUX | NAND_CON_CS_P
			| NAND_CON_SE_P | NAND_CON_WP_P | NAND_CON_PRE_P
			| cs_flag, EBU_NAND_CON);	
	} else if (cs && (*cs == 0)) {
		ltq_ebu_w32(NANDPHYSADDR(nandaddr)
			|  ADDSEL1_MASK(1) | ADDSEL0_REGEN, EBU_ADDSEL0);

		ltq_ebu_w32(BUSCON0_SETUP | BUSCON0_ALEC | BUSCON0_BCGEN_RES
			| BUSCON0_WAITWRC2 | BUSCON0_WAITRDC2
			| BUSCON0_HOLDC1 | BUSCON0_RECOVC1
			| BUSCON0_CMULT4, LTQ_EBU_BUSCON0);

		ltq_ebu_w32(NAND_CON_CSMUX | NAND_CON_CS_P
			| NAND_CON_SE_P | NAND_CON_WP_P
			| NAND_CON_PRE_P, EBU_NAND_CON);
	} else {
		pr_err("Platform does not support chip select %d\n", cs_flag);
	}

	/* finish with a reset */
	xway_reset_chip(this);

	return 0;
}

/* allow users to override the partition in DT using the cmdline */
static const char const *part_probes[] = { "cmdlinepart", "ofpart", NULL };

static struct platform_nand_data xway_nand_data = {
	.chip = {
		.nr_chips		= 1,
		.chip_delay		= 30,
		.part_probe_types	= part_probes,
	},
	.ctrl = {
		.probe		= xway_nand_probe,
		.cmd_ctrl	= xway_cmd_ctrl,
		.dev_ready	= xway_dev_ready,
		.select_chip	= xway_select_chip,
		.read_byte	= xway_read_byte,
		.read_buf	= xway_read_buf,
		.write_buf	= xway_write_buf,
	}
};

/*
 * Try to find the node inside the DT. If it is available attach out
 * platform_nand_data
 */
static int __init xway_register_nand(void)
{
	struct device_node *node;
	struct platform_device *pdev;

	node = of_find_compatible_node(NULL, NULL, "lantiq,nand-xway");
	if (!node) {
		pr_err("Cannot find compatible string\n");
		return -ENOENT;
	}
	pdev = of_find_device_by_node(node);
	if (!pdev) {
		pr_err("Cannot find device in tree\n");
		return -EINVAL;
	}
	pdev->dev.platform_data = &xway_nand_data;
	of_node_put(node);

	pr_err("Register xway nand done!\n");
	return 0;
}

subsys_initcall(xway_register_nand);
