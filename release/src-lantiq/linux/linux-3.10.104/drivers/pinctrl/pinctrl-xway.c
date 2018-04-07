/*
 *  linux/drivers/pinctrl/pinmux-xway.c
 *  based on linux/drivers/pinctrl/pinmux-pxa910.c
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  publishhed by the Free Software Foundation.
 *
 *  Copyright (C) 2012 John Crispin <blogic@openwrt.org>
 */

#include <linux/err.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/ioport.h>
#include <linux/io.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#ifdef CONFIG_PINCTRL_SYSFS
#include <linux/device.h>
#include <linux/kdev_t.h>
#endif

#include "pinctrl-lantiq.h"

#include <lantiq_soc.h>

/* we have 3 1/2 banks of 16 bit each */
#define PINS			16
#define PORT3			3
#define PORT(x)			(x / PINS)
#define PORT_PIN(x)		(x % PINS)

/* we have 2 mux bits that can be set for each pin */
#define MUX_ALT0	0x1
#define MUX_ALT1	0x2

/*
 * each bank has this offset apart from the 1/2 bank that is mixed into the
 * other 3 ranges
 */
#define REG_OFF			0x30

/* these are the offsets to our registers */
#define GPIO_BASE(p)		(REG_OFF * PORT(p))
#define GPIO_OUT(p)		GPIO_BASE(p)
#define GPIO_IN(p)		(GPIO_BASE(p) + 0x04)
#define GPIO_DIR(p)		(GPIO_BASE(p) + 0x08)
#define GPIO_ALT0(p)		(GPIO_BASE(p) + 0x0C)
#define GPIO_ALT1(p)		(GPIO_BASE(p) + 0x10)
#define GPIO_OD(p)		(GPIO_BASE(p) + 0x14)
#define GPIO_PUDSEL(p)		(GPIO_BASE(p) + 0x1c)
#define GPIO_PUDEN(p)		(GPIO_BASE(p) + 0x20)

/* the 1/2 port needs special offsets for some registers */
#define GPIO3_OD		(GPIO_BASE(0) + 0x24)
#define GPIO3_PUDSEL		(GPIO_BASE(0) + 0x28)
#define GPIO3_PUDEN		(GPIO_BASE(0) + 0x2C)
#define GPIO3_ALT1		(GPIO_BASE(PINS) + 0x24)

/* macros to help us access the registers */
#define gpio_getbit(m, r, p)	(!!(ltq_r32(m + r) & BIT(p)))
#define gpio_setbit(m, r, p)	ltq_w32_mask(0, BIT(p), m + r)
#define gpio_clearbit(m, r, p)	ltq_w32_mask(BIT(p), 0, m + r)

#define MFP_XWAY(a, f0, f1, f2, f3)	\
	{				\
		.name = #a,		\
		.pin = a,		\
		.func = {		\
			XWAY_MUX_##f0,	\
			XWAY_MUX_##f1,	\
			XWAY_MUX_##f2,	\
			XWAY_MUX_##f3,	\
		},			\
	}

#define GRP_MUX(a, m, p)		\
	{ .name = a, .mux = XWAY_MUX_##m, .pins = p, .npins = ARRAY_SIZE(p), }

#define FUNC_MUX(f, m)		\
	{ .func = f, .mux = XWAY_MUX_##m, }

#define XWAY_MAX_PIN		32
#define XR9_MAX_PIN		56
#define XR10_MAX_PIN	62
#define XRX3XX_MAX_PIN	 62

enum xway_mux {
	XWAY_MUX_GPIO = 0,
	XWAY_MUX_SPI,
	XWAY_MUX_USIF,
	XWAY_MUX_ASC,
	XWAY_MUX_PCI,
	XWAY_MUX_CGU,
	XWAY_MUX_EBU,
	XWAY_MUX_JTAG,
	XWAY_MUX_EXIN,
	XWAY_MUX_TDM,
	XWAY_MUX_STP,
	XWAY_MUX_SIN,
	XWAY_MUX_GPT,
	XWAY_MUX_NMI,
	XWAY_MUX_MDIO,
	XWAY_MUX_MII,
	XWAY_MUX_EPHY,
	XWAY_MUX_DFE,
	XWAY_MUX_SDIO,
	XWAY_MUX_GPHY,
	XWAY_MUX_WLAN,
	XWAY_MUX_MCD,
	XWAY_MUX_SSI0,
	XWAY_MUX_LED,
	XWAY_MUX_DSP,
	XWAY_MUX_ARC,
	XWAY_MUX_NONE = 0xffff,
};

static const struct ltq_mfp_pin xway_mfp[] = {
	/*       pin    f0	f1	f2	f3   */
	MFP_XWAY(GPIO0, GPIO,	EXIN,	NONE,	TDM),
	MFP_XWAY(GPIO1, GPIO,	EXIN,	NONE,	NONE),
	MFP_XWAY(GPIO2, GPIO,	CGU,	EXIN,	NONE),
	MFP_XWAY(GPIO3, GPIO,	CGU,	NONE,	PCI),
	MFP_XWAY(GPIO4, GPIO,	STP,	NONE,	ASC),
	MFP_XWAY(GPIO5, GPIO,	STP,	NONE,	NONE),
	MFP_XWAY(GPIO6, GPIO,	STP,	GPT,	ASC),
	MFP_XWAY(GPIO7, GPIO,	CGU,	PCI,	NONE),
	MFP_XWAY(GPIO8, GPIO,	CGU,	NMI,	NONE),
	MFP_XWAY(GPIO9, GPIO,	ASC,	SPI,	EXIN),
	MFP_XWAY(GPIO10, GPIO,	USIF,	SPI,	NONE),
	MFP_XWAY(GPIO11, GPIO,	USIF,	PCI,	SPI),
	MFP_XWAY(GPIO12, GPIO,	USIF,	NONE,	NONE),
	MFP_XWAY(GPIO13, GPIO,	EBU,	SPI,	NONE),
	MFP_XWAY(GPIO14, GPIO,	CGU,	PCI,	NONE),
	MFP_XWAY(GPIO15, GPIO,	SPI,	JTAG,	NONE),
	MFP_XWAY(GPIO16, GPIO,	SPI,	NONE,	JTAG),
	MFP_XWAY(GPIO17, GPIO,	SPI,	NONE,	JTAG),
	MFP_XWAY(GPIO18, GPIO,	SPI,	NONE,	JTAG),
	MFP_XWAY(GPIO19, GPIO,	USIF,	NONE,	NONE),
	MFP_XWAY(GPIO20, GPIO,	JTAG,	NONE,	NONE),
	MFP_XWAY(GPIO21, GPIO,	PCI,	EBU,	GPT),
	MFP_XWAY(GPIO22, GPIO,	SPI,	NONE,	NONE),
	MFP_XWAY(GPIO23, GPIO,	EBU,	PCI,	STP),
	MFP_XWAY(GPIO24, GPIO,	EBU,	TDM,	PCI),
	MFP_XWAY(GPIO25, GPIO,	TDM,	NONE,	ASC),
	MFP_XWAY(GPIO26, GPIO,	EBU,	NONE,	TDM),
	MFP_XWAY(GPIO27, GPIO,	TDM,	NONE,	ASC),
	MFP_XWAY(GPIO28, GPIO,	GPT,	NONE,	NONE),
	MFP_XWAY(GPIO29, GPIO,	PCI,	NONE,	NONE),
	MFP_XWAY(GPIO30, GPIO,	PCI,	NONE,	NONE),
	MFP_XWAY(GPIO31, GPIO,	EBU,	PCI,	NONE),
	MFP_XWAY(GPIO32, GPIO,	NONE,	NONE,	EBU),
	MFP_XWAY(GPIO33, GPIO,	NONE,	NONE,	EBU),
	MFP_XWAY(GPIO34, GPIO,	NONE,	NONE,	EBU),
	MFP_XWAY(GPIO35, GPIO,	NONE,	NONE,	EBU),
	MFP_XWAY(GPIO36, GPIO,	SIN,	NONE,	EBU),
	MFP_XWAY(GPIO37, GPIO,	PCI,	NONE,	NONE),
	MFP_XWAY(GPIO38, GPIO,	PCI,	NONE,	NONE),
	MFP_XWAY(GPIO39, GPIO,	EXIN,	NONE,	NONE),
	MFP_XWAY(GPIO40, GPIO,	NONE,	NONE,	NONE),
	MFP_XWAY(GPIO41, GPIO,	NONE,	NONE,	NONE),
	MFP_XWAY(GPIO42, GPIO,	MDIO,	NONE,	NONE),
	MFP_XWAY(GPIO43, GPIO,	MDIO,	NONE,	NONE),
	MFP_XWAY(GPIO44, GPIO,	NONE,	NONE,	SIN),
	MFP_XWAY(GPIO45, GPIO,	NONE,	NONE,	SIN),
	MFP_XWAY(GPIO46, GPIO,	NONE,	NONE,	EXIN),
	MFP_XWAY(GPIO47, GPIO,	NONE,	NONE,	SIN),
	MFP_XWAY(GPIO48, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO49, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO50, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO51, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO52, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO53, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO54, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO55, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO56, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO57, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO58, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO59, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO60, GPIO,	EBU,	NONE,	NONE),
	MFP_XWAY(GPIO61, GPIO,	EBU,	NONE,	NONE),

};

static const struct ltq_mfp_pin xrx3xx_mfp[] = {
        MFP_XWAY(GPIO0, GPIO,   EXIN,   EPHY,   NONE),
        MFP_XWAY(GPIO1, GPIO,   NONE,   EXIN,   NONE),
        MFP_XWAY(GPIO2, NONE,   NONE,   NONE,   NONE),
        MFP_XWAY(GPIO3, GPIO,   CGU,    NONE,   NONE),
        MFP_XWAY(GPIO4, GPIO,   STP,	DSP,    NONE),
        MFP_XWAY(GPIO5, GPIO,   STP,    EPHY,   NONE),
        MFP_XWAY(GPIO6, GPIO,   STP,    NONE,   ARC),
        MFP_XWAY(GPIO7, NONE,   NONE,   NONE,   NONE),
        MFP_XWAY(GPIO8, GPIO,   CGU,    GPHY,   EPHY),
        MFP_XWAY(GPIO9, GPIO,   WLAN,   NONE,   EXIN),
        MFP_XWAY(GPIO10, GPIO,  USIF,   SPI,    EXIN),
        MFP_XWAY(GPIO11, GPIO,  USIF,   WLAN,    SPI),
        MFP_XWAY(GPIO12, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO13, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO14, GPIO,  CGU,    USIF,   EPHY),
        MFP_XWAY(GPIO15, GPIO,  SPI,    NONE,   MCD),
        MFP_XWAY(GPIO16, GPIO,  SPI,    EXIN,   NONE),
        MFP_XWAY(GPIO17, GPIO,  SPI,    NONE,   NONE),
        MFP_XWAY(GPIO18, GPIO,  SPI,    NONE,   NONE),
        MFP_XWAY(GPIO19, GPIO,  USIF,   NONE,   EPHY),
        MFP_XWAY(GPIO20, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO21, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO22, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO23, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO24, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO25, GPIO,  TDM,    NONE,   NONE),
        MFP_XWAY(GPIO26, GPIO,  NONE,   TDM,    NONE),
        MFP_XWAY(GPIO27, GPIO,  TDM,    NONE,   NONE),
        MFP_XWAY(GPIO28, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO29, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO30, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO31, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO32, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO33, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO34, GPIO,  NONE,   SSI0,   NONE),
        MFP_XWAY(GPIO35, GPIO,  NONE,   SSI0,   NONE),
        MFP_XWAY(GPIO36, GPIO,  NONE,   SSI0,   NONE),
        MFP_XWAY(GPIO37, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO38, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO39, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO40, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO41, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO42, GPIO,  MDIO,   NONE,   NONE),
        MFP_XWAY(GPIO43, GPIO,  MDIO,   NONE,   NONE),
        MFP_XWAY(GPIO44, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO45, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO46, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO47, NONE,  NONE,   NONE,   NONE),
        MFP_XWAY(GPIO48, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO49, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO50, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO51, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO52, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO53, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO54, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO55, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO56, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO57, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO58, GPIO,  EBU,    TDM,    NONE),
        MFP_XWAY(GPIO59, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO60, GPIO,  EBU,    NONE,   NONE),
        MFP_XWAY(GPIO61, GPIO,  EBU,    NONE,   NONE),
};
static const struct ltq_mfp_pin ase_mfp[] = {
	/*       pin    f0	f1	f2	f3   */
	MFP_XWAY(GPIO0, GPIO,	EXIN,	MII,	TDM),
	MFP_XWAY(GPIO1, GPIO,	STP,	DFE,	EBU),
	MFP_XWAY(GPIO2, GPIO,	STP,	DFE,	EPHY),
	MFP_XWAY(GPIO3, GPIO,	STP,	EPHY,	EBU),
	MFP_XWAY(GPIO4, GPIO,	GPT,	EPHY,	MII),
	MFP_XWAY(GPIO5, GPIO,	MII,	ASC,	GPT),
	MFP_XWAY(GPIO6, GPIO,	MII,	ASC,	EXIN),
	MFP_XWAY(GPIO7, GPIO,	SPI,	MII,	JTAG),
	MFP_XWAY(GPIO8, GPIO,	SPI,	MII,	JTAG),
	MFP_XWAY(GPIO9, GPIO,	SPI,	MII,	JTAG),
	MFP_XWAY(GPIO10, GPIO,	SPI,	MII,	JTAG),
	MFP_XWAY(GPIO11, GPIO,	EBU,	CGU,	JTAG),
	MFP_XWAY(GPIO12, GPIO,	EBU,	MII,	SDIO),
	MFP_XWAY(GPIO13, GPIO,	EBU,	MII,	CGU),
	MFP_XWAY(GPIO14, GPIO,	EBU,	SPI,	CGU),
	MFP_XWAY(GPIO15, GPIO,	EBU,	SPI,	SDIO),
	MFP_XWAY(GPIO16, GPIO,	NONE,	NONE,	NONE),
	MFP_XWAY(GPIO17, GPIO,	NONE,	NONE,	NONE),
	MFP_XWAY(GPIO18, GPIO,	NONE,	NONE,	NONE),
	MFP_XWAY(GPIO19, GPIO,	EBU,	MII,	SDIO),
	MFP_XWAY(GPIO20, GPIO,	EBU,	MII,	SDIO),
	MFP_XWAY(GPIO21, GPIO,	EBU,	MII,	SDIO),
	MFP_XWAY(GPIO22, GPIO,	EBU,	MII,	CGU),
	MFP_XWAY(GPIO23, GPIO,	EBU,	MII,	CGU),
	MFP_XWAY(GPIO24, GPIO,	EBU,	NONE,	MII),
	MFP_XWAY(GPIO25, GPIO,	EBU,	MII,	GPT),
	MFP_XWAY(GPIO26, GPIO,	EBU,	MII,	SDIO),
	MFP_XWAY(GPIO27, GPIO,	EBU,	NONE,	MII),
	MFP_XWAY(GPIO28, GPIO,	MII,	EBU,	SDIO),
	MFP_XWAY(GPIO29, GPIO,	EBU,	MII,	EXIN),
	MFP_XWAY(GPIO30, GPIO,	NONE,	NONE,	NONE),
	MFP_XWAY(GPIO31, GPIO,	NONE,	NONE,	NONE),
};
static const unsigned xrx3xx_pins_stp[] = {GPIO4, GPIO5, GPIO6};
static const unsigned xrx3xx_pins_mdio[] = {GPIO42, GPIO43};
static const unsigned xrx3xx_pins_nand_ale[] = {GPIO13};
static const unsigned xrx3xx_pins_nand_cs1[] = {GPIO23};
static const unsigned xrx3xx_pins_nand_cle[] = {GPIO24};
static const unsigned xrx3xx_pins_nand_rdy[] = {GPIO48};
static const unsigned xrx3xx_pins_nand_rd[] = {GPIO49};
static const unsigned xrx3xx_pins_nand_d1[] = {GPIO50};
static const unsigned xrx3xx_pins_nand_d0[] = {GPIO51};
static const unsigned xrx3xx_pins_nand_d2p1[] = {GPIO52};
static const unsigned xrx3xx_pins_nand_d2p2[] = {GPIO53};
static const unsigned xrx3xx_pins_nand_d6 [] = {GPIO54};
static const unsigned xrx3xx_pins_nand_d5p1 [] = {GPIO55};
static const unsigned xrx3xx_pins_nand_d5p2 [] = {GPIO56};
static const unsigned xrx3xx_pins_nand_d3 [] = {GPIO57};
static const unsigned xrx3xx_pins_nand_cs0 [] = {GPIO58};
static const unsigned xrx3xx_pins_nand_wr [] = {GPIO59};
static const unsigned xrx3xx_pins_nand_wp [] = {GPIO60};
static const unsigned xrx3xx_pins_nand_se [] = {GPIO61};
static const unsigned xrx3xx_pins_exin0[] = {GPIO0};
static const unsigned xrx3xx_pins_exin1[] = {GPIO1};
static const unsigned xrx3xx_pins_exin2[] = {GPIO16};
static const unsigned xrx3xx_pins_exin4[] = {GPIO10};
static const unsigned xrx3xx_pins_exin5[] = {GPIO9};
static const unsigned xrx3xx_pins_spi[] = {GPIO16, GPIO17, GPIO18};
static const unsigned xrx3xx_pins_spi_cs1[] = {GPIO15};
static const unsigned xrx3xx_pins_spi_cs4[] = {GPIO10};
static const unsigned xrx3xx_pins_spi_cs6[] = {GPIO11};
static const unsigned xrx3xx_pins_usif[] = {GPIO10, GPIO11, GPIO14, GPIO19};
static const unsigned xrx3xx_pins_clkout0[] = {GPIO3};
static const unsigned xrx3xx_pins_clkout1[] = {GPIO8};
static const unsigned xrx3xx_pins_clkout2[] = {GPIO14};
static const unsigned xrx3xx_pins_ephy_ready[] = {GPIO0};
static const unsigned xrx3xx_pins_ephy0_led0[] = {GPIO5};
static const unsigned xrx3xx_pins_ephy0_led1[] = {GPIO8};
static const unsigned xrx3xx_pins_ephy1_led0[] = {GPIO14};
static const unsigned xrx3xx_pins_ephy1_led1[] = {GPIO19};
static const unsigned xrx3xx_pins_gphy[] = {GPIO8};
static const unsigned xrx3xx_pins_wlan[] = {GPIO9, GPIO11};
static const unsigned xrx3xx_pins_mcd[] = {GPIO15};
static const unsigned xrx3xx_pins_tdm[] = {GPIO25, GPIO26, GPIO27, GPIO58};
static const unsigned xrx3xx_pins_ssi0[] = {GPIO34, GPIO35, GPIO36};
static const unsigned xrx3xx_pins_dsp[] = {GPIO4};
static const unsigned xrx3xx_pins_arc[] = {GPIO5};

static const unsigned pins_jtag[] = {GPIO15, GPIO16, GPIO17, GPIO19, GPIO35};
static const unsigned pins_asc0[] = {GPIO11, GPIO12};
static const unsigned pins_asc0_cts_rts[] = {GPIO9, GPIO10};
static const unsigned pins_stp[] = {GPIO4, GPIO5, GPIO6};
static const unsigned pins_nmi[] = {GPIO8};
static const unsigned pins_mdio[] = {GPIO42, GPIO43};
static const unsigned pins_arc_jtag[] = {GPIO9, GPIO10, GPIO11, GPIO14, GPIO19};

static const unsigned pins_ebu_a24[] = {GPIO13};
static const unsigned pins_ebu_clk[] = {GPIO21};
static const unsigned pins_ebu_cs1[] = {GPIO23};
static const unsigned pins_ebu_a23[] = {GPIO24};
static const unsigned pins_ebu_wait[] = {GPIO26};
static const unsigned pins_ebu_a25[] = {GPIO31};
static const unsigned pins_ebu_rdy[] = {GPIO48};
static const unsigned pins_ebu_rd[] = {GPIO49};

static const unsigned pins_nand_ale[] = {GPIO13};
static const unsigned pins_nand_cs1[] = {GPIO23};
static const unsigned pins_nand_cle[] = {GPIO24};
static const unsigned pins_nand_rdy[] = {GPIO48};
static const unsigned pins_nand_rd[] = {GPIO49};
static const unsigned pins_nand_d1[] = {GPIO50};
static const unsigned pins_nand_d0[] = {GPIO51};
static const unsigned pins_nand_d2p1[] = {GPIO52};
static const unsigned pins_nand_d2p2[] = {GPIO53};
static const unsigned pins_nand_d6 [] = {GPIO54};
static const unsigned pins_nand_d5p1 [] = {GPIO55};
static const unsigned pins_nand_d5p2 [] = {GPIO56};
static const unsigned pins_nand_d3 [] = {GPIO57};
static const unsigned pins_nand_cs0 [] = {GPIO58};
static const unsigned pins_nand_wr [] = {GPIO59};
static const unsigned pins_nand_wp [] = {GPIO60};
static const unsigned pins_nand_se [] = {GPIO61};

static const unsigned pins_exin0[] = {GPIO0};
static const unsigned pins_exin1[] = {GPIO1};
static const unsigned pins_exin2[] = {GPIO2};
static const unsigned pins_exin3[] = {GPIO39};
static const unsigned pins_exin4[] = {GPIO46};
static const unsigned pins_exin5[] = {GPIO9};

static const unsigned pins_spi[] = {GPIO16, GPIO17, GPIO18};
static const unsigned pins_spi_cs1[] = {GPIO15};
static const unsigned pins_spi_cs2[] = {GPIO22};
static const unsigned pins_spi_cs3[] = {GPIO13};
static const unsigned pins_spi_cs4[] = {GPIO10};
static const unsigned pins_spi_cs5[] = {GPIO9};
static const unsigned pins_spi_cs6[] = {GPIO11};

static const unsigned pins_usif[] = {GPIO10, GPIO11, GPIO19};
static const unsigned pins_usif_uart[] = {GPIO11, GPIO12};
static const unsigned pins_gpt1[] = {GPIO28};
static const unsigned pins_gpt2[] = {GPIO21};
static const unsigned pins_gpt3[] = {GPIO6};

static const unsigned pins_clkout0[] = {GPIO8};
static const unsigned pins_clkout1[] = {GPIO7};
static const unsigned pins_clkout2[] = {GPIO3};
static const unsigned pins_clkout3[] = {GPIO2};

static const unsigned pins_pci_gnt1[] = {GPIO30};
static const unsigned pins_pci_gnt2[] = {GPIO23};
static const unsigned pins_pci_gnt3[] = {GPIO19};
static const unsigned pins_pci_gnt4[] = {GPIO38};
static const unsigned pins_pci_req1[] = {GPIO29};
static const unsigned pins_pci_req2[] = {GPIO31};
static const unsigned pins_pci_req3[] = {GPIO3};
static const unsigned pins_pci_req4[] = {GPIO37};
static const unsigned pins_led[] = {GPIO1, GPIO9, GPIO5, GPIO33, GPIO11, GPIO12, GPIO15};

static const unsigned ase_pins_jtag[] = {GPIO7, GPIO8, GPIO9, GPIO10, GPIO11};
static const unsigned ase_pins_asc[] = {GPIO5, GPIO6};
static const unsigned ase_pins_stp[] = {GPIO1, GPIO2, GPIO3};
static const unsigned ase_pins_ephy[] = {GPIO2, GPIO3, GPIO4};
static const unsigned ase_pins_dfe[] = {GPIO1, GPIO2};

static const unsigned ase_pins_spi[] = {GPIO8, GPIO9, GPIO10};
static const unsigned ase_pins_spi_cs1[] = {GPIO7};
static const unsigned ase_pins_spi_cs2[] = {GPIO15};
static const unsigned ase_pins_spi_cs3[] = {GPIO14};

static const unsigned ase_pins_exin0[] = {GPIO6};
static const unsigned ase_pins_exin1[] = {GPIO29};
static const unsigned ase_pins_exin2[] = {GPIO0};

static const unsigned ase_pins_gpt1[] = {GPIO5};
static const unsigned ase_pins_gpt2[] = {GPIO4};
static const unsigned ase_pins_gpt3[] = {GPIO25};

static const struct ltq_pin_group xway_grps[] = {
	GRP_MUX("exin0", EXIN, pins_exin0),
	GRP_MUX("exin1", EXIN, pins_exin1),
	GRP_MUX("exin2", EXIN, pins_exin2),
	GRP_MUX("jtag", JTAG, pins_jtag),
	GRP_MUX("ebu a23", EBU, pins_ebu_a23),
	GRP_MUX("ebu a24", EBU, pins_ebu_a24),
	GRP_MUX("ebu a25", EBU, pins_ebu_a25),
	GRP_MUX("ebu clk", EBU, pins_ebu_clk),
	GRP_MUX("ebu cs1", EBU, pins_ebu_cs1),
	GRP_MUX("ebu wait", EBU, pins_ebu_wait),
	GRP_MUX("nand ale", EBU, pins_nand_ale),
	GRP_MUX("nand cs1", EBU, pins_nand_cs1),
	GRP_MUX("nand cle", EBU, pins_nand_cle),
	GRP_MUX("spi", SPI, pins_spi),
	GRP_MUX("spi_cs1", SPI, pins_spi_cs1),
	GRP_MUX("spi_cs2", SPI, pins_spi_cs2),
	GRP_MUX("spi_cs3", SPI, pins_spi_cs3),
	GRP_MUX("spi_cs4", SPI, pins_spi_cs4),
	GRP_MUX("spi_cs5", SPI, pins_spi_cs5),
	GRP_MUX("spi_cs6", SPI, pins_spi_cs6),
	GRP_MUX("usif", USIF, pins_usif),
	GRP_MUX("usif_uart", USIF, pins_usif_uart),
	GRP_MUX("asc0", ASC, pins_asc0),
	GRP_MUX("asc0 cts rts", ASC, pins_asc0_cts_rts),
	GRP_MUX("stp", STP, pins_stp),
	GRP_MUX("nmi", NMI, pins_nmi),
	GRP_MUX("gpt1", GPT, pins_gpt1),
	GRP_MUX("gpt2", GPT, pins_gpt2),
	GRP_MUX("gpt3", GPT, pins_gpt3),
	GRP_MUX("clkout0", CGU, pins_clkout0),
	GRP_MUX("clkout1", CGU, pins_clkout1),
	GRP_MUX("clkout2", CGU, pins_clkout2),
	GRP_MUX("clkout3", CGU, pins_clkout3),
	GRP_MUX("gnt1", PCI, pins_pci_gnt1),
	GRP_MUX("gnt2", PCI, pins_pci_gnt2),
	GRP_MUX("gnt3", PCI, pins_pci_gnt3),
	GRP_MUX("req1", PCI, pins_pci_req1),
	GRP_MUX("req2", PCI, pins_pci_req2),
	GRP_MUX("req3", PCI, pins_pci_req3),
/* xrx only */
	GRP_MUX("nand rdy", EBU, pins_nand_rdy),
	GRP_MUX("nand rd", EBU, pins_nand_rd),
	GRP_MUX("exin3", EXIN, pins_exin3),
	GRP_MUX("exin4", EXIN, pins_exin4),
	GRP_MUX("exin5", EXIN, pins_exin5),
	GRP_MUX("gnt4", PCI, pins_pci_gnt4),
	GRP_MUX("req4", PCI, pins_pci_gnt4),
	GRP_MUX("mdio", MDIO, pins_mdio),
	/* xrx388 additional pins */
    GRP_MUX("nand d1", EBU, pins_nand_d1),
	GRP_MUX("nand d0", EBU, pins_nand_d0),
	GRP_MUX("nand d2p1", EBU, pins_nand_d2p1),
	GRP_MUX("nand d2p2", EBU, pins_nand_d2p2),
	GRP_MUX("nand d6", EBU, pins_nand_d6),
	GRP_MUX("nand d5p1", EBU, pins_nand_d5p1),
	GRP_MUX("nand d5p2", EBU, pins_nand_d5p2),
	GRP_MUX("nand d3", EBU, pins_nand_d3),
	GRP_MUX("nand cs0", EBU, pins_nand_cs0),
	GRP_MUX("nand wr", EBU, pins_nand_wr),
	GRP_MUX("nand wp", EBU, pins_nand_wp),
	GRP_MUX("nand se", EBU, pins_nand_se),
	GRP_MUX("arc_jtag", GPIO, pins_arc_jtag),
	GRP_MUX("led", GPIO, pins_led),
};
static const struct ltq_pin_group xrx3xx_grps[] = {
	GRP_MUX("exin0", EXIN, xrx3xx_pins_exin0),
	GRP_MUX("exin1", EXIN, xrx3xx_pins_exin1),
	GRP_MUX("exin2", EXIN, xrx3xx_pins_exin2),
	GRP_MUX("exin4", EXIN, xrx3xx_pins_exin4),
	GRP_MUX("exin5", EXIN, xrx3xx_pins_exin5),
	GRP_MUX("stp", STP, xrx3xx_pins_stp),
	GRP_MUX("mdio", MDIO, xrx3xx_pins_mdio),
	GRP_MUX("nand ale", EBU, xrx3xx_pins_nand_ale),
	GRP_MUX("nand cs1", EBU, xrx3xx_pins_nand_cs1),
	GRP_MUX("nand cle", EBU, xrx3xx_pins_nand_cle),
	GRP_MUX("nand rdy", EBU, xrx3xx_pins_nand_rdy),
    GRP_MUX("nand d1", EBU, xrx3xx_pins_nand_d1),
	GRP_MUX("nand d0", EBU, xrx3xx_pins_nand_d0),
	GRP_MUX("nand d2p1", EBU, xrx3xx_pins_nand_d2p1),
	GRP_MUX("nand d2p2", EBU, xrx3xx_pins_nand_d2p2),
	GRP_MUX("nand d6", EBU, xrx3xx_pins_nand_d6),
	GRP_MUX("nand d5p1", EBU, xrx3xx_pins_nand_d5p1),
	GRP_MUX("nand d5p2", EBU, xrx3xx_pins_nand_d5p2),
	GRP_MUX("nand d3", EBU, xrx3xx_pins_nand_d3),
	GRP_MUX("nand cs0", EBU, xrx3xx_pins_nand_cs0),
	GRP_MUX("nand wr", EBU, xrx3xx_pins_nand_wr),
	GRP_MUX("nand wp", EBU, xrx3xx_pins_nand_wp),
	GRP_MUX("nand rd", EBU, xrx3xx_pins_nand_rd),
	GRP_MUX("nand se", EBU, xrx3xx_pins_nand_se),
	GRP_MUX("spi", SPI, xrx3xx_pins_spi),
	GRP_MUX("spi_cs1", SPI, xrx3xx_pins_spi_cs1),
	GRP_MUX("spi_cs4", SPI, xrx3xx_pins_spi_cs4),
	GRP_MUX("spi_cs6", SPI, xrx3xx_pins_spi_cs6),
	GRP_MUX("usif", USIF, xrx3xx_pins_usif),
	GRP_MUX("clkout0", CGU, xrx3xx_pins_clkout0),
	GRP_MUX("clkout1", CGU, xrx3xx_pins_clkout1),
	GRP_MUX("clkout2", CGU, xrx3xx_pins_clkout2),
	GRP_MUX("ephy_ready", EPHY, xrx3xx_pins_ephy_ready),
	GRP_MUX("ephy0_led0", EPHY, xrx3xx_pins_ephy0_led0),
	GRP_MUX("ephy0_led1", EPHY, xrx3xx_pins_ephy0_led1),
	GRP_MUX("ephy1_led0", EPHY, xrx3xx_pins_ephy1_led0),
	GRP_MUX("ephy1_led1", EPHY, xrx3xx_pins_ephy1_led1),
	GRP_MUX("gphy", GPHY, xrx3xx_pins_gphy),
	GRP_MUX("wlan", WLAN, xrx3xx_pins_wlan),
	GRP_MUX("mcd", MCD, xrx3xx_pins_mcd),
	GRP_MUX("tdm", TDM, xrx3xx_pins_tdm),
	GRP_MUX("ssi0", SSI0, xrx3xx_pins_ssi0),
	GRP_MUX("dsp", SSI0, xrx3xx_pins_dsp),
	GRP_MUX("arc", SSI0, xrx3xx_pins_arc),
};

static const struct ltq_pin_group ase_grps[] = {
	GRP_MUX("exin0", EXIN, ase_pins_exin0),
	GRP_MUX("exin1", EXIN, ase_pins_exin1),
	GRP_MUX("exin2", EXIN, ase_pins_exin2),
	GRP_MUX("jtag", JTAG, ase_pins_jtag),
	GRP_MUX("stp", STP, ase_pins_stp),
	GRP_MUX("asc", ASC, ase_pins_asc),
	GRP_MUX("gpt1", GPT, ase_pins_gpt1),
	GRP_MUX("gpt2", GPT, ase_pins_gpt2),
	GRP_MUX("gpt3", GPT, ase_pins_gpt3),
	GRP_MUX("ephy", EPHY, ase_pins_ephy),
	GRP_MUX("dfe", DFE, ase_pins_dfe),
	GRP_MUX("spi", SPI, ase_pins_spi),
	GRP_MUX("spi_cs1", SPI, ase_pins_spi_cs1),
	GRP_MUX("spi_cs2", SPI, ase_pins_spi_cs2),
	GRP_MUX("spi_cs3", SPI, ase_pins_spi_cs3),
};

static const char * const xway_pci_grps[] = {"gnt1", "gnt2",
						"gnt3", "req1",
						"req2", "req3"};
static const char * const xway_spi_grps[] = {"spi", "spi_cs1",
						"spi_cs2", "spi_cs3",
						"spi_cs4", "spi_cs5",
						"spi_cs6"};
static const char * const xway_usif_grps[] = {"usif"};
static const char * const xway_usif_uart_grps[] = {"usif_uart"};
static const char * const xway_cgu_grps[] = {"clkout0", "clkout1",
						"clkout2", "clkout3"};
static const char * const xway_ebu_grps[] = {"ebu a23", "ebu a24",
						"ebu a25", "ebu cs1",
						"ebu wait", "ebu clk",
						"nand ale", "nand cs1",
						"nand cle"};
static const char * const xway_exin_grps[] = {"exin0", "exin1", "exin2"};
static const char * const xway_gpt_grps[] = {"gpt1", "gpt2", "gpt3"};
static const char * const xway_asc_grps[] = {"asc0", "asc0 cts rts"};
static const char * const xway_jtag_grps[] = {"jtag"};
static const char * const xway_stp_grps[] = {"stp"};
static const char * const xway_nmi_grps[] = {"nmi"};
static const char * const xway_led_grps[] = {"led"};

/* ar9/vr9/gr9 */
static const char * const xrx_mdio_grps[] = {"mdio"};
static const char * const xrx_ebu_grps[] = {"ebu a23", "ebu a24",
						"ebu a25", "ebu cs1",
						"ebu wait", "ebu clk",
						"nand ale", "nand cs1",
						"nand cle", "nand rdy",
						"nand rd",	"nand d1",
						"nand d0", "nand d2p1",
						"nand d2p2", "nand d6",
						"nand d5p1", "nand d5p2",
						"nand d3", "nand cs0",
						"nand wr",	"nand wp",
						"nand se"};
static const char * const xrx_exin_grps[] = {"exin0", "exin1", "exin2",
						"exin3", "exin4", "exin5"};
static const char * const xrx_pci_grps[] = {"gnt1", "gnt2",
						"gnt3", "gnt4",
						"req1", "req2",
						"req3", "req4"};
static const char * const xrx_arc_jtag_grps[] = {"arc_jtag"};

static const char * const xrx3xx_spi_grps[] = {"spi", "spi_cs1",
						"spi_cs4",
						"spi_cs6"};
static const char * const xrx3xx_usif_grps[] = {"usif"};
static const char * const xrx3xx_cgu_grps[] = {"clkout0", "clkout1",
						"clkout2"};
static const char * const xrx3xx_exin_grps[] = {"exin0", "exin1", "exin2",
							"exin4", "exin5"};
static const char * const xrx3xx_stp_grps[] = {"stp"};
static const char * const xrx3xx_mdio_grps[] = {"mdio"};
static const char * const xrx3xx_ebu_grps[] = { 
						"nand ale", "nand cs1",
						"nand cle", "nand rdy",
						"nand rd",	"nand d1",
						"nand d0", "nand d2p1",
						"nand d2p2", "nand d6",
						"nand d5p1", "nand d5p2",
						"nand d3", "nand cs0",
						"nand wr",	"nand wp",
						"nand se"};
static const char * const xrx3xx_ephy_grps[] = {"ephy_ready",
						"ephy0_led0", "ephy0_led1",
						 "ephy1_led0", "ephy1_led1"};
static const char * const xrx3xx_gphy_grps[] = {"gphy"};
static const char * const xrx3xx_wlan_grps[] = {"wlan"};
static const char * const xrx3xx_mcd_grps[] = {"mcd"};
static const char * const xrx3xx_tdm_grps[] = {"tdm"};
static const char * const xrx3xx_ssi0_grps[] = {"ssi0"};
static const char * const xrx3xx_dsp_grps[] = {"dsp"};
static const char * const xrx3xx_arc_grps[] = {"arc"};
/* ase */
static const char * const ase_exin_grps[] = {"exin0", "exin1", "exin2"};
static const char * const ase_gpt_grps[] = {"gpt1", "gpt2", "gpt3"};
static const char * const ase_dfe_grps[] = {"dfe"};
static const char * const ase_ephy_grps[] = {"ephy"};
static const char * const ase_asc_grps[] = {"asc"};
static const char * const ase_jtag_grps[] = {"jtag"};
static const char * const ase_stp_grps[] = {"stp"};
static const char * const ase_spi_grps[] = {"spi", "spi_cs1",
						"spi_cs2", "spi_cs3"};

static const struct ltq_pmx_func danube_funcs[] = {
	{"spi",		ARRAY_AND_SIZE(xway_spi_grps)},
	{"asc",		ARRAY_AND_SIZE(xway_asc_grps)},
	{"cgu",		ARRAY_AND_SIZE(xway_cgu_grps)},
	{"jtag",	ARRAY_AND_SIZE(xway_jtag_grps)},
	{"exin",	ARRAY_AND_SIZE(xway_exin_grps)},
	{"stp",		ARRAY_AND_SIZE(xway_stp_grps)},
	{"gpt",		ARRAY_AND_SIZE(xway_gpt_grps)},
	{"nmi",		ARRAY_AND_SIZE(xway_nmi_grps)},
	{"pci",		ARRAY_AND_SIZE(xway_pci_grps)},
	{"ebu",		ARRAY_AND_SIZE(xway_ebu_grps)},
};

static const struct ltq_pmx_func xrx_funcs[] = {
	{"spi",		ARRAY_AND_SIZE(xway_spi_grps)},
	{"usif",	ARRAY_AND_SIZE(xway_usif_grps)},
	{"usif_uart",	ARRAY_AND_SIZE(xway_usif_uart_grps)},
	{"asc",		ARRAY_AND_SIZE(xway_asc_grps)},
	{"cgu",		ARRAY_AND_SIZE(xway_cgu_grps)},
	{"jtag",	ARRAY_AND_SIZE(xway_jtag_grps)},
	{"exin",	ARRAY_AND_SIZE(xrx_exin_grps)},
	{"stp",		ARRAY_AND_SIZE(xway_stp_grps)},
	{"gpt",		ARRAY_AND_SIZE(xway_gpt_grps)},
	{"nmi",		ARRAY_AND_SIZE(xway_nmi_grps)},
	{"pci",		ARRAY_AND_SIZE(xrx_pci_grps)},
	{"ebu",		ARRAY_AND_SIZE(xrx_ebu_grps)},
	{"mdio",	ARRAY_AND_SIZE(xrx_mdio_grps)},
	{"arc_jtag",	ARRAY_AND_SIZE(xrx_arc_jtag_grps)},
	{"led",		ARRAY_AND_SIZE(xway_led_grps)},
};
static const struct ltq_pmx_func xrx3xx_funcs[] = {
	{"spi",		ARRAY_AND_SIZE(xrx3xx_spi_grps)},
	{"usif",	ARRAY_AND_SIZE(xrx3xx_usif_grps)},
	{"cgu",		ARRAY_AND_SIZE(xrx3xx_cgu_grps)},
	{"exin",	ARRAY_AND_SIZE(xrx3xx_exin_grps)},
	{"ebu",		ARRAY_AND_SIZE(xrx3xx_ebu_grps)},
	{"mdio",	ARRAY_AND_SIZE(xrx3xx_mdio_grps)},
	{"stp",		ARRAY_AND_SIZE(xrx3xx_stp_grps)},
	{"ephy",	ARRAY_AND_SIZE(xrx3xx_ephy_grps)},
	{"gphy",	ARRAY_AND_SIZE(xrx3xx_gphy_grps)},
	{"wlan",	ARRAY_AND_SIZE(xrx3xx_wlan_grps)},
	{"mcd",		ARRAY_AND_SIZE(xrx3xx_mcd_grps)},
	{"tdm",		ARRAY_AND_SIZE(xrx3xx_tdm_grps)},
	{"ssi0",	ARRAY_AND_SIZE(xrx3xx_ssi0_grps)},
	{"dsp",		ARRAY_AND_SIZE(xrx3xx_dsp_grps)},
	{"arc",		ARRAY_AND_SIZE(xrx3xx_arc_grps)},
};
static const struct ltq_pmx_func ase_funcs[] = {
	{"spi",		ARRAY_AND_SIZE(ase_spi_grps)},
	{"asc",		ARRAY_AND_SIZE(ase_asc_grps)},
	{"jtag",	ARRAY_AND_SIZE(ase_jtag_grps)},
	{"exin",	ARRAY_AND_SIZE(ase_exin_grps)},
	{"stp",		ARRAY_AND_SIZE(ase_stp_grps)},
	{"gpt",		ARRAY_AND_SIZE(ase_gpt_grps)},
	{"ephy",	ARRAY_AND_SIZE(ase_ephy_grps)},
	{"dfe",		ARRAY_AND_SIZE(ase_dfe_grps)},
};

#ifdef CONFIG_PINCTRL_SYSFS
#define NUM_PORTS 4
static struct platform_device *pinctrl_platform_dev;

struct sys_lantiq_pin_desc {
		unsigned long		flags;
			/* flag symbols are bit numbers */
#define FLAG_REQUESTED	0
#define FLAG_IS_OUT	1
#define FLAG_EXPORT	2	/* protected by sysfs_lock */
#define FLAG_SYSFS	3	/* exported via /sys/class/gpio/control */
#define FLAG_TRIG_FALL	4	/* trigger on falling edge */
#define FLAG_PULL_DOWN	5	
#define FLAG_PULL_UP	6	
#define FLAG_OPEN_DRAIN	7	/* Gpio is open drain type */
#define FLAG_OPEN_SOURCE 8	/* Gpio is open source type */

#define ID_SHIFT	16	/* add new flags before this one */

#define GPIO_FLAGS_MASK		((1 << ID_SHIFT) - 1)
#define GPIO_TRIGGER_MASK	(BIT(FLAG_TRIG_FALL) | BIT(FLAG_TRIG_RISE))

};
static struct sys_lantiq_pin_desc pin_desc_array[NUM_PORTS * PINS];
#endif

/* ---------  pinconf related code --------- */
static int xway_pinconf_get(struct pinctrl_dev *pctldev,
				unsigned pin,
				unsigned long *config)
{
	struct ltq_pinmux_info *info = pinctrl_dev_get_drvdata(pctldev);
	enum ltq_pinconf_param param = LTQ_PINCONF_UNPACK_PARAM(*config);
	int port = PORT(pin);
	u32 reg;

	switch (param) {
	case LTQ_PINCONF_PARAM_OPEN_DRAIN:
		if (port == PORT3)
			reg = GPIO3_OD;
		else
			reg = GPIO_OD(pin);
		*config = LTQ_PINCONF_PACK(param,
			!gpio_getbit(info->membase[0], reg, PORT_PIN(pin)));
		break;

	case LTQ_PINCONF_PARAM_PULL:
		if (port == PORT3)
			reg = GPIO3_PUDEN;
		else
			reg = GPIO_PUDEN(pin);
		if (!gpio_getbit(info->membase[0], reg, PORT_PIN(pin))) {
			*config = LTQ_PINCONF_PACK(param, 0);
			break;
		}

		if (port == PORT3)
			reg = GPIO3_PUDSEL;
		else
			reg = GPIO_PUDSEL(pin);
		if (!gpio_getbit(info->membase[0], reg, PORT_PIN(pin)))
			*config = LTQ_PINCONF_PACK(param, 2);
		else
			*config = LTQ_PINCONF_PACK(param, 1);
		break;

	case LTQ_PINCONF_PARAM_OUTPUT:
		reg = GPIO_DIR(pin);
		*config = LTQ_PINCONF_PACK(param,
			gpio_getbit(info->membase[0], reg, PORT_PIN(pin)));
		break;
	default:
		dev_err(pctldev->dev, "Invalid config param %04x\n", param);
		return -ENOTSUPP;
	}
	return 0;
}

static int xway_pinconf_set(struct pinctrl_dev *pctldev,
				unsigned pin,
				unsigned long config)
{
	struct ltq_pinmux_info *info = pinctrl_dev_get_drvdata(pctldev);
	enum ltq_pinconf_param param = LTQ_PINCONF_UNPACK_PARAM(config);
	int arg = LTQ_PINCONF_UNPACK_ARG(config);
	int port = PORT(pin);
	u32 reg;

	switch (param) {
	case LTQ_PINCONF_PARAM_OPEN_DRAIN:
		if (port == PORT3)
			reg = GPIO3_OD;
		else
			reg = GPIO_OD(pin);
		if (arg == 0)
			gpio_setbit(info->membase[0], reg, PORT_PIN(pin));
		else
			gpio_clearbit(info->membase[0], reg, PORT_PIN(pin));
		break;

	case LTQ_PINCONF_PARAM_PULL:
		if (port == PORT3)
			reg = GPIO3_PUDEN;
		else
			reg = GPIO_PUDEN(pin);
		if (arg == 0) {
			gpio_clearbit(info->membase[0], reg, PORT_PIN(pin));
			break;
		}
		gpio_setbit(info->membase[0], reg, PORT_PIN(pin));

		if (port == PORT3)
			reg = GPIO3_PUDSEL;
		else
			reg = GPIO_PUDSEL(pin);
		if (arg == 1)
			gpio_clearbit(info->membase[0], reg, PORT_PIN(pin));
		else if (arg == 2)
			gpio_setbit(info->membase[0], reg, PORT_PIN(pin));
		else
			dev_err(pctldev->dev, "Invalid pull value %d\n", arg);
		break;

	case LTQ_PINCONF_PARAM_OUTPUT:
		reg = GPIO_DIR(pin);
		if (arg == 0)
			gpio_clearbit(info->membase[0], reg, PORT_PIN(pin));
		else
			gpio_setbit(info->membase[0], reg, PORT_PIN(pin));
		break;

	default:
		dev_err(pctldev->dev, "Invalid config param %04x\n", param);
		return -ENOTSUPP;
	}
	return 0;
}

int xway_pinconf_group_set(struct pinctrl_dev *pctldev,
			unsigned selector,
			unsigned long config)
{
	struct ltq_pinmux_info *info = pinctrl_dev_get_drvdata(pctldev);
	int i, ret = 0;

	for (i = 0; i < info->grps[selector].npins && !ret; i++)
		ret = xway_pinconf_set(pctldev,
				info->grps[selector].pins[i], config);

	return ret;
}

static const struct pinconf_ops xway_pinconf_ops = {
	.pin_config_get	= xway_pinconf_get,
	.pin_config_set	= xway_pinconf_set,
	.pin_config_group_set = xway_pinconf_group_set,
};

static struct pinctrl_desc xway_pctrl_desc = {
	.owner		= THIS_MODULE,
	.confops	= &xway_pinconf_ops,
};

static int mux_apply(struct ltq_pinmux_info *info,
				int pin, int mux)
{
	int port = PORT(pin);
	u32 alt1_reg = GPIO_ALT1(pin);

	if (port == PORT3)
		alt1_reg = GPIO3_ALT1;

	if (mux & MUX_ALT0)
		gpio_setbit(info->membase[0], GPIO_ALT0(pin), PORT_PIN(pin));
	else
		gpio_clearbit(info->membase[0], GPIO_ALT0(pin), PORT_PIN(pin));

	if (mux & MUX_ALT1)
		gpio_setbit(info->membase[0], alt1_reg, PORT_PIN(pin));
	else
		gpio_clearbit(info->membase[0], alt1_reg, PORT_PIN(pin));

	return 0;
}

static inline int xway_mux_apply(struct pinctrl_dev *pctrldev,
				int pin, int mux)
{
	struct ltq_pinmux_info *info = pinctrl_dev_get_drvdata(pctrldev);

	return mux_apply(info, pin, mux);
}

static const struct ltq_cfg_param xway_cfg_params[] = {
	{"lantiq,pull",		LTQ_PINCONF_PARAM_PULL},
	{"lantiq,open-drain",	LTQ_PINCONF_PARAM_OPEN_DRAIN},
	{"lantiq,output",	LTQ_PINCONF_PARAM_OUTPUT},
};

static struct ltq_pinmux_info xway_info = {
	.desc		= &xway_pctrl_desc,
	.apply_mux	= xway_mux_apply,
	.params		= xway_cfg_params,
	.num_params	= ARRAY_SIZE(xway_cfg_params),
};

/* ---------  gpio_chip related code --------- */
static void xway_gpio_set(struct gpio_chip *chip, unsigned int pin, int val)
{
	struct ltq_pinmux_info *info = dev_get_drvdata(chip->dev);

	if (val)
		gpio_setbit(info->membase[0], GPIO_OUT(pin), PORT_PIN(pin));
	else
		gpio_clearbit(info->membase[0], GPIO_OUT(pin), PORT_PIN(pin));
}

static int xway_gpio_get(struct gpio_chip *chip, unsigned int pin)
{
	struct ltq_pinmux_info *info = dev_get_drvdata(chip->dev);

	return gpio_getbit(info->membase[0], GPIO_IN(pin), PORT_PIN(pin));
}

static int xway_gpio_dir_in(struct gpio_chip *chip, unsigned int pin)
{
	struct ltq_pinmux_info *info = dev_get_drvdata(chip->dev);

	gpio_clearbit(info->membase[0], GPIO_DIR(pin), PORT_PIN(pin));

	return 0;
}

static int xway_gpio_dir_out(struct gpio_chip *chip, unsigned int pin, int val)
{
	struct ltq_pinmux_info *info = dev_get_drvdata(chip->dev);

	if (PORT(pin) == PORT3)
		gpio_setbit(info->membase[0], GPIO3_OD, PORT_PIN(pin));
	else
		gpio_setbit(info->membase[0], GPIO_OD(pin), PORT_PIN(pin));
	gpio_setbit(info->membase[0], GPIO_DIR(pin), PORT_PIN(pin));
	xway_gpio_set(chip, pin, val);

	return 0;
}

static int xway_gpio_req(struct gpio_chip *chip, unsigned offset)
{
	int gpio = chip->base + offset;

	return pinctrl_request_gpio(gpio);
}

static void xway_gpio_free(struct gpio_chip *chip, unsigned offset)
{
	int gpio = chip->base + offset;

	pinctrl_free_gpio(gpio);
}

static int xway_gpio_to_irq(struct gpio_chip *chip, unsigned offset)
{
	struct ltq_pinmux_info *info = dev_get_drvdata(chip->dev);
	int i;

	for (i = 0; i < info->num_exin; i++)
		if (info->exin[i] == offset)
			return ltq_eiu_get_irq(i);

	return -1;
}

static struct gpio_chip xway_chip = {
	.label = "gpio-xway",
	.direction_input = xway_gpio_dir_in,
	.direction_output = xway_gpio_dir_out,
	.get = xway_gpio_get,
	.set = xway_gpio_set,
	.request = xway_gpio_req,
	.free = xway_gpio_free,
	.to_irq = xway_gpio_to_irq,
	.base = -1,
};


/* --------- register the pinctrl layer --------- */
static const unsigned xway_exin_pin_map[] = {GPIO0, GPIO1, GPIO2, GPIO39, GPIO46, GPIO9};
static const unsigned ase_exin_pins_map[] = {GPIO6, GPIO29, GPIO0};
static const unsigned xrx3xx_exin_pins_map[] = {GPIO0, GPIO1, GPIO16, GPIO10, GPIO9};

static struct pinctrl_xway_soc {
	int pin_count;
	const struct ltq_mfp_pin *mfp;
	const struct ltq_pin_group *grps;
	unsigned int num_grps;
	const struct ltq_pmx_func *funcs;
	unsigned int num_funcs;
	const unsigned *exin;
	unsigned int num_exin;
} soc_cfg[] = {
	/* legacy xway */
	{XWAY_MAX_PIN, xway_mfp,
		xway_grps, ARRAY_SIZE(xway_grps),
		danube_funcs, ARRAY_SIZE(danube_funcs),
		xway_exin_pin_map, 3},
	/* xway xr9 series */
	{XR9_MAX_PIN, xway_mfp,
		xway_grps, ARRAY_SIZE(xway_grps),
		xrx_funcs, ARRAY_SIZE(xrx_funcs),
		xway_exin_pin_map, 6},
	{XR10_MAX_PIN, xway_mfp,
		xway_grps, ARRAY_SIZE(xway_grps),
		xrx_funcs, ARRAY_SIZE(xrx_funcs),
		xway_exin_pin_map, 6},
	/* xway ase series */
	{XWAY_MAX_PIN, ase_mfp,
		ase_grps, ARRAY_SIZE(ase_grps),
		ase_funcs, ARRAY_SIZE(ase_funcs),
		ase_exin_pins_map, 3},
	{XRX3XX_MAX_PIN, xrx3xx_mfp,
		xrx3xx_grps, ARRAY_SIZE(xrx3xx_grps),
		xrx3xx_funcs, ARRAY_SIZE(xrx3xx_funcs),
		xrx3xx_exin_pins_map, 5},
};

static struct pinctrl_gpio_range xway_gpio_range = {
	.name	= "XWAY GPIO",
	.gc	= &xway_chip,
};

static const struct of_device_id xway_match[] = {
	{ .compatible = "lantiq,pinctrl-xway", .data = &soc_cfg[0]},
	{ .compatible = "lantiq,pinctrl-xr9", .data = &soc_cfg[1]},
	{ .compatible = "lantiq,pinctrl-ar10", .data = &soc_cfg[4]},
	{ .compatible = "lantiq,pinctrl-ase", .data = &soc_cfg[3]},
	{},
};
MODULE_DEVICE_TABLE(of, xway_match);

#ifdef CONFIG_PINCTRL_SYSFS
/* lock protects against unexport_pin() being called while
 * sysfs files are active.
 */
static DEFINE_MUTEX(sysfs_lock);

static ssize_t export_store(struct class *class,
				struct class_attribute *attr,
				const char *buf, size_t len);
static ssize_t unexport_store(struct class *class,
				struct class_attribute *attr,
				const char *buf, size_t len);
static struct class_attribute pinctrl_class_attrs[] = {
        __ATTR(export, 0200, NULL, export_store),
        __ATTR(unexport, 0200, NULL, unexport_store),
        __ATTR_NULL,
};

static struct class pinctrl_class = {
        .name =         "pinctrl",
        .owner =        THIS_MODULE,

        .class_attrs =  pinctrl_class_attrs,
};

static ssize_t pad_ctrl_avail_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	const struct sys_lantiq_pin_desc *desc = dev_get_drvdata(dev);
	ssize_t			status;
	u32 pin;
    unsigned long value = 0xFFFFFFFF;
	pin = desc - pin_desc_array;
	mutex_lock(&sysfs_lock);
	if (!test_bit((FLAG_EXPORT), &(desc->flags)))
		status = -EIO;
	else
		status = sprintf(buf, "%d\n",
				test_bit(pin, &value));

	mutex_unlock(&sysfs_lock);

	return status;
}


static const DEVICE_ATTR(padctrl_availability, 0644,
		pad_ctrl_avail_show, NULL);

static ssize_t pin_pullup_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	const struct sys_lantiq_pin_desc *desc = dev_get_drvdata(dev);
	struct ltq_pinmux_info *info = platform_get_drvdata(pinctrl_platform_dev);
	ssize_t status = 0;
	u32 pin;
	int port;
	u32 reg;

	pin = desc - pin_desc_array;
	port  = PORT(pin);
	
	mutex_lock(&sysfs_lock);
	if (!test_bit((FLAG_EXPORT), &(desc->flags))) {
		status = -EIO;
	} else {
		if (port == PORT3)
			reg = GPIO3_PUDSEL;
		else
			reg = GPIO_PUDSEL(pin);
			
		if(gpio_getbit(info->membase[0], reg, PORT_PIN(pin))) {
			if (port == PORT3)
				reg = GPIO3_PUDEN;
			else
				reg = GPIO_PUDEN(pin);
			if(gpio_getbit(info->membase[0], reg, PORT_PIN(pin)))
				status = sprintf(buf, "%s\n", "EN");
			else 
				status = sprintf(buf, "%s\n", "DIS");
		} else {
		}

	}
	mutex_unlock(&sysfs_lock);

	return status;
}

static ssize_t pin_pullup_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	const struct sys_lantiq_pin_desc *desc = dev_get_drvdata(dev);
	struct ltq_pinmux_info *info = platform_get_drvdata(pinctrl_platform_dev);
	ssize_t			status = 0;
	u32 pin;
	int port;
	u32 reg;
	long value;
	pin = desc - pin_desc_array;
	port  = PORT(pin); 
	mutex_lock(&sysfs_lock);

	if (!test_bit(FLAG_EXPORT, &desc->flags))
		status = -EIO;
		
	
	if (sysfs_streq(buf, "DIS")) {
		value = 0;
	} else if (sysfs_streq(buf, "EN")) {
		value = 1;
	}else
		status = -EINVAL;
	
	if (status == 0) {
		if (port == PORT3)
			reg = GPIO3_PUDSEL;
		else
			reg = GPIO_PUDSEL(pin);
		gpio_setbit(info->membase[0], reg, PORT_PIN(pin));
		
		if (port == PORT3)
			reg = GPIO3_PUDEN;
		else
			reg = GPIO_PUDEN(pin);
			
		if (value == 0) {
			gpio_clearbit(info->membase[0], reg, PORT_PIN(pin));
		} else {
			gpio_setbit(info->membase[0], reg, PORT_PIN(pin));
		}
	}
	mutex_unlock(&sysfs_lock);
	return status ? : size;
}

static const DEVICE_ATTR(pullup, 0644,
		pin_pullup_show, pin_pullup_store);
		
static ssize_t pin_pulldown_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	const struct sys_lantiq_pin_desc *desc = dev_get_drvdata(dev);
	struct ltq_pinmux_info *info = platform_get_drvdata(pinctrl_platform_dev);
	ssize_t status = 0;
	u32 pin;
	int port;
	u32 reg;

	pin = desc - pin_desc_array;
	port  = PORT(pin);
	
	mutex_lock(&sysfs_lock);
	if (!test_bit((FLAG_EXPORT), &(desc->flags))) {
		status = -EIO;
	} else {
		if (port == PORT3)
			reg = GPIO3_PUDSEL;
		else
			reg = GPIO_PUDSEL(pin);
			
		if(!(gpio_getbit(info->membase[0], reg, PORT_PIN(pin)))){
			if (port == PORT3)
				reg = GPIO3_PUDEN;
			else
				reg = GPIO_PUDEN(pin);
			if(gpio_getbit(info->membase[0], reg, PORT_PIN(pin)))
				status = sprintf(buf, "%s\n", "EN");
			else 
				status = sprintf(buf, "%s\n", "DIS");
		} else {
		}

	}
	mutex_unlock(&sysfs_lock);

	return status;
}

static ssize_t pin_pulldown_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	const struct sys_lantiq_pin_desc *desc = dev_get_drvdata(dev);
	struct ltq_pinmux_info *info = platform_get_drvdata(pinctrl_platform_dev);
	ssize_t			status = 0;
	u32 pin;
	int port;
	u32 reg;
	long value;
	pin = desc - pin_desc_array;
	port  = PORT(pin); 
	mutex_lock(&sysfs_lock);

	if (!test_bit(FLAG_EXPORT, &desc->flags))
		status = -EIO;
		
	
	if (sysfs_streq(buf, "DIS")) {
		value = 0;
	} else if (sysfs_streq(buf, "EN")) {
		value = 1;
	}else
		status = -EINVAL;
	
	if (status == 0) {
		if (port == PORT3)
			reg = GPIO3_PUDSEL;
		else
			reg = GPIO_PUDSEL(pin);
		gpio_clearbit(info->membase[0], reg, PORT_PIN(pin));
		
		if (port == PORT3)
			reg = GPIO3_PUDEN;
		else
			reg = GPIO_PUDEN(pin);
			
		if (value == 0) {
			gpio_clearbit(info->membase[0], reg, PORT_PIN(pin));
		} else {
			gpio_setbit(info->membase[0], reg, PORT_PIN(pin));
		}
	}
	mutex_unlock(&sysfs_lock);
	return status ? : size;
}

static const DEVICE_ATTR(pulldown, 0644,
		pin_pulldown_show, pin_pulldown_store);

static ssize_t pin_opendrain_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	const struct sys_lantiq_pin_desc *desc = dev_get_drvdata(dev);
	struct ltq_pinmux_info *info = platform_get_drvdata(pinctrl_platform_dev);
	ssize_t status = 0;
	u32 pin;
	int port;
	u32 reg;

	pin = desc - pin_desc_array;
	port  = PORT(pin);
	
	mutex_lock(&sysfs_lock);
	if (!test_bit((FLAG_EXPORT), &(desc->flags))) {
		status = -EIO;
	} else {
		if (port == PORT3)
			reg = GPIO3_OD;
		else
			reg = GPIO_OD(pin);
			
		if((gpio_getbit(info->membase[0], reg, PORT_PIN(pin)))){
			status = sprintf(buf, "%s\n", "NOP");
		} else { 
				status = sprintf(buf, "%s\n", "EN");
		} 
	}
	mutex_unlock(&sysfs_lock);
	return status;
}

static ssize_t pin_opendrain_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	const struct sys_lantiq_pin_desc *desc = dev_get_drvdata(dev);
	struct ltq_pinmux_info *info = platform_get_drvdata(pinctrl_platform_dev);
	ssize_t			status = 0;
	u32 pin;
	int port;
	u32 reg;
	long value;
	pin = desc - pin_desc_array;
	port  = PORT(pin); 
	mutex_lock(&sysfs_lock);

	if (!test_bit(FLAG_EXPORT, &desc->flags))
		status = -EIO;
		
	
	if (sysfs_streq(buf, "NOP")) {
		value = 1;
	} else if (sysfs_streq(buf, "EN")) {
		value = 0;
	}else
		status = -EINVAL;
	
	if (status == 0) {
		if (port == PORT3)
			reg = GPIO3_OD;
		else
			reg = GPIO_OD(pin);
			
		if (value == 0)
			gpio_clearbit(info->membase[0], reg, PORT_PIN(pin));
		else
			gpio_setbit(info->membase[0], reg, PORT_PIN(pin));
	}
	mutex_unlock(&sysfs_lock);
	return status ? : size;
}

static const DEVICE_ATTR(opendrain, 0644,
		pin_opendrain_show, pin_opendrain_store);

static ssize_t pinmux_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	const struct sys_lantiq_pin_desc *desc = dev_get_drvdata(dev);
	struct ltq_pinmux_info *info = platform_get_drvdata(pinctrl_platform_dev);
	ssize_t status = 0;
	u32 pin;
	int port;
	u32 mux;

	pin = desc - pin_desc_array;
	port  = PORT(pin);
	
	mutex_lock(&sysfs_lock);
	if (!test_bit((FLAG_EXPORT), &(desc->flags))) {
		status = -EIO;
	} else {
		u32 alt1_reg = GPIO_ALT1(pin);
		if (port == PORT3)
			alt1_reg = GPIO3_ALT1;
		mux = gpio_getbit(info->membase[0], GPIO_ALT0(pin), PORT_PIN(pin));
		mux |= gpio_getbit(info->membase[0], alt1_reg, PORT_PIN(pin)) << 1;
		status = sprintf(buf, "%d\n", mux);
	}

	
	mutex_unlock(&sysfs_lock);

	return status;
}

static ssize_t pinmux_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	const struct sys_lantiq_pin_desc *desc = dev_get_drvdata(dev);
	struct ltq_pinmux_info *info = platform_get_drvdata(pinctrl_platform_dev);
	ssize_t			status = 0;
	u32 pin;
	int port;
	u32 reg;
	long value;
	pin = desc - pin_desc_array;
	port  = PORT(pin); 
	mutex_lock(&sysfs_lock);

	if (!test_bit(FLAG_EXPORT, &desc->flags)) {
		status = -EIO;
	} else {
		status = strict_strtol(buf, 0, &value);
		if (status == 0) {
			u32 alt1_reg = GPIO_ALT1(pin);

			if (port == PORT3)
				alt1_reg = GPIO3_ALT1;

			if (value & MUX_ALT0)
				gpio_setbit(info->membase[0], GPIO_ALT0(pin), PORT_PIN(pin));
			else
				gpio_clearbit(info->membase[0], GPIO_ALT0(pin), PORT_PIN(pin));

			if (value & MUX_ALT1)
				gpio_setbit(info->membase[0], alt1_reg, PORT_PIN(pin));
			else
				gpio_clearbit(info->membase[0], alt1_reg, PORT_PIN(pin));
		}
	}


	mutex_unlock(&sysfs_lock);
	return status ? : size;
}

static const DEVICE_ATTR(pinmux, 0644,
		pinmux_show, pinmux_store);


static const struct attribute *pin_attrs[] = {

	&dev_attr_pullup.attr,
	&dev_attr_pulldown.attr,
	&dev_attr_opendrain.attr,
	&dev_attr_pinmux.attr,
#if 0
	&dev_attr_slewrate.attr,
	&dev_attr_currentcontrol.attr,
	&dev_attr_padctrl_availability.attr,
#endif
	NULL,
};

static const struct attribute_group pin_attr_group = {
	.attrs = (struct attribute **) pin_attrs,
};

static int pin_export(unsigned int pin)
{
	int		status;
	struct device	*dev;

	/* Many systems register gpio chips for SOC support very early,
	 * before driver model support is available.  In those cases we
	 * export this later, in gpiolib_sysfs_init() ... here we just
	 * verify that _some_ field of gpio_class got initialized.
	 */
	if (!pinctrl_class.p)
		return 0;

	/* use chip->base for the ID; it's already known to be unique */
	mutex_lock(&sysfs_lock);
	//if((pin_desc_array[pin].flags & FLAG_EXPORT) == FLAG_EXPORT){
	if (pin >= (NUM_PORTS*PINS)) {
		pr_warn("%s: invalid pin\n", __func__);
		status = -ENODEV;
		goto fail_unlock;
	}
	
	if(test_bit((FLAG_EXPORT), &pin_desc_array[pin].flags)){
		pr_err("Pin %d already exported\r\n", pin);
		status = -EPERM;
		goto fail_unlock;
	}
	dev = device_create(&pinctrl_class, &pinctrl_platform_dev->dev, MKDEV(0, 0), &pin_desc_array[pin], "pin_%d", pin);
    if (IS_ERR(dev)) {
		status = PTR_ERR(dev);
		goto fail_unlock;
	}
    set_bit((FLAG_EXPORT), &pin_desc_array[pin].flags);
	//pin_desc_array[pin].flags |= FLAG_EXPORT;
    status = sysfs_create_group(&dev->kobj, &pin_attr_group);
	if (status)
		goto fail_unregister_device;
	mutex_unlock(&sysfs_lock);
	return 0;

fail_unregister_device:
	device_unregister(dev);
fail_unlock:
	mutex_unlock(&sysfs_lock);
	pr_debug("%s:  status %d\n", __func__, status);
	return status;
}
static int match_export(struct device *dev, const void *data)
{   
	    return dev_get_drvdata(dev) == data;
}

static int pin_unexport(unsigned int pin)
{
	int			status = 0;
	struct device		*dev = NULL;

	if (pin >= (NUM_PORTS*PINS)) {
		pr_warn("%s: invalid pin\n", __func__);
		status = -ENODEV;
		goto fail_unlock;
	}

	mutex_lock(&sysfs_lock);
	if(!(test_bit((FLAG_EXPORT), &pin_desc_array[pin].flags))){
		status = -ENODEV;
		goto fail_unlock;
	}


		dev = class_find_device(&pinctrl_class, NULL, &pin_desc_array[pin], match_export);
		if (dev) {
            clear_bit((FLAG_EXPORT), &pin_desc_array[pin].flags);
			//pin_desc_array[pin].flags &= ~FLAG_EXPORT;	
		} else {
			status = -ENODEV;
			goto fail_unlock;
		}	
	if (dev) {
		device_unregister(dev);
		put_device(dev);
	}
	mutex_unlock(&sysfs_lock);
	return 0;
fail_unlock:
	mutex_unlock(&sysfs_lock);
	pr_debug("%s:  status %d\n", __func__, status);
	return status;
}

static ssize_t export_store(struct class *class,
				struct class_attribute *attr,
				const char *buf, size_t len)
{
	long			pin;
	int			status;

	status = strict_strtol(buf, 0, &pin);
	if (status < 0)
		goto done;

	
	/* No extra locking here; FLAG_SYSFS just signifies that the
	 * request and export were done by on behalf of userspace, so
	 * they may be undone on its behalf too.
	 */

	status = pin_export((unsigned int)pin);
	if (status < 0) {
		if (status == -EPROBE_DEFER)
			status = -ENODEV;
		goto done;
	}
	
	

done:
	if (status)
		pr_debug("%s: status %d\n", __func__, status);
	return status ? : len;
}

static ssize_t unexport_store(struct class *class,
				struct class_attribute *attr,
				const char *buf, size_t len)
{
	long	pin;
	int			status;

	status = strict_strtol(buf, 0, &pin);
	if (status < 0)
		goto done;
	status = pin_unexport(pin);
	/* No extra locking here; FLAG_SYSFS just signifies that the
	 * request and export were done by on behalf of userspace, so
	 * they may be undone on its behalf too.
	 */
done:
	if (status)
		pr_debug("%s: status %d\n", __func__, status);
	return status ? : len;
}


static int pinctrl_sysfs_init(struct device *dev)
{
	int	status;
	status = class_register(&pinctrl_class);
	return status;

}
#endif

static int pinmux_xway_probe(struct platform_device *pdev)
{
	const struct of_device_id *match;
	const struct pinctrl_xway_soc *xway_soc;
	struct resource *res;
	int ret, i;

	/* get and remap our register range */
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	xway_info.membase[0] = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(xway_info.membase[0]))
		return PTR_ERR(xway_info.membase[0]);

	match = of_match_device(xway_match, &pdev->dev);
	if (match)
		xway_soc = (const struct pinctrl_xway_soc *) match->data;
	else
		xway_soc = &soc_cfg[0];

	/* find out how many pads we have */
	xway_chip.ngpio = xway_soc->pin_count;

	/* load our pad descriptors */
	xway_info.pads = devm_kzalloc(&pdev->dev,
			sizeof(struct pinctrl_pin_desc) * xway_chip.ngpio,
			GFP_KERNEL);
	if (!xway_info.pads) {
		dev_err(&pdev->dev, "Failed to allocate pads\n");
		return -ENOMEM;
	}
	for (i = 0; i < xway_chip.ngpio; i++) {
		/* strlen("ioXY") + 1 = 5 */
		char *name = devm_kzalloc(&pdev->dev, 5, GFP_KERNEL);

		if (!name) {
			dev_err(&pdev->dev, "Failed to allocate pad name\n");
			return -ENOMEM;
		}
		snprintf(name, 5, "io%d", i);
		xway_info.pads[i].number = GPIO0 + i;
		xway_info.pads[i].name = name;
	}
	xway_pctrl_desc.pins = xway_info.pads;

	/* load the gpio chip */
	xway_chip.dev = &pdev->dev;
	of_gpiochip_add(&xway_chip);
	ret = gpiochip_add(&xway_chip);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register gpio chip\n");
		return ret;
	}

	/* setup the data needed by pinctrl */
	xway_pctrl_desc.name	= dev_name(&pdev->dev);
	xway_pctrl_desc.npins	= xway_chip.ngpio;

	xway_info.num_pads	= xway_chip.ngpio;
	xway_info.num_mfp	= xway_chip.ngpio;
	xway_info.mfp		= xway_soc->mfp;
	xway_info.grps		= xway_soc->grps;
	xway_info.num_grps	= xway_soc->num_grps;
	xway_info.funcs		= xway_soc->funcs;
	xway_info.num_funcs	= xway_soc->num_funcs;
	xway_info.exin		= xway_soc->exin;
	xway_info.num_exin	= xway_soc->num_exin;

	/* register with the generic lantiq layer */
	ret = ltq_pinctrl_register(pdev, &xway_info);
	if (ret) {
		dev_err(&pdev->dev, "Failed to register pinctrl driver\n");
		return ret;
	}

	/* finish with registering the gpio range in pinctrl */
	xway_gpio_range.npins = xway_chip.ngpio;
	xway_gpio_range.base = xway_chip.base;
	pinctrl_add_gpio_range(xway_info.pctrl, &xway_gpio_range);
	dev_info(&pdev->dev, "Init done\n");

	if (!ret) {
#ifdef CONFIG_PINCTRL_SYSFS
	    pinctrl_platform_dev = pdev;
		pinctrl_sysfs_init(&pdev->dev);	
#endif
		pr_info("Init done\n");
	}
	return 0;
}

static struct platform_driver pinmux_xway_driver = {
	.probe	= pinmux_xway_probe,
	.driver = {
		.name	= "pinctrl-xway",
		.owner	= THIS_MODULE,
		.of_match_table = xway_match,
	},
};

static int __init pinmux_xway_init(void)
{
	return platform_driver_register(&pinmux_xway_driver);
}

core_initcall_sync(pinmux_xway_init);
