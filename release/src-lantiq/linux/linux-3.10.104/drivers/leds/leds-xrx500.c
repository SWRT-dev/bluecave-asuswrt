/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2014 Kavitha  Subramanian <s.kavitha.EE@lantiq.com>
 *
 */


#include "leds-xrx500-config.h"

static struct xrx500_sso_freq_table fpid_table[] = {
	{0, 251, 500},
	{1, 126, 250},
	{2, 101, 125},
	{3, 1, 100}
};

static struct xrx500_sso_freq_table gptd_table[] = {
	{0, 11, 20},
	{1, 6, 10},
	{2, 5, 5},
	{3, 1, 4}
};
static struct xrx500_sso_config_param g_board_xrx500_sso_hw_config = {
	.operation_mask         = /*XRX500_SSO_CFG_OP_UPDATE_SHADOW | XRX500_SSO_CFG_OP_BLINK_MASK 
				| XRX500_SSO_CFG_OP_UPDATE_CLOCK | XRX500_SSO_CFG_OP_UPDATE_FPID | XRX500_SSO_CFG_OP_UPDATE_FCDSC | XRX500_SSO_CFG_OP_UPDATE_GPTD
				| XRX500_SSO_CFG_OP_BLINK_RATE | XRX500_SSO_CFG_OP_INDIVIDUAL_BLINK_SOURCE | XRX500_SSO_CFG_OP_BLINK_SOURCE_0_23*/ 0,
	.shadow		    = 0x0,
	.blink_mask             = 0x0,
	.update_clock           = LED_CON1_UPDATE_SRC_GPTD2,
	.fpid                   = 0,
	.fcdsc                  = 1,
	.gptd					= 0,
	.is_blink_rate_diff     = 1,
	.blink_rate_source_0     = {
				LED_H8_UPDATE_SRC_FPID,
				LED_H8_UPDATE_SRC_FPID,
					LED_H8_UPDATE_SRC_FPID,
					LED_H8_UPDATE_SRC_FPID,
					LED_H8_UPDATE_SRC_FPID,
					LED_H8_UPDATE_SRC_FPID,
					LED_H8_UPDATE_SRC_FPID,
					LED_H8_UPDATE_SRC_FPID,
			      },
	.blink_rate_source_1 = LED_H8_UPDATE_SRC_FPID
};



static struct xrx500_sso *g_chip;
static struct xrx500_sso_led g_xrx500_sso_led_config[NUM_LEDS];

/**
 * xrx500_sso_set() - gpio_chip->set - set gpios.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 * @val:    Value to be written to specified signal.
 *
 * Set the shadow value and call ltq_ebu_apply.
 */
static void xrx500_sso_set(struct gpio_chip *gc, unsigned gpio, int val)
{
	struct xrx500_sso *chip =
		container_of(gc, struct xrx500_sso, gc);
	/*normal LED ON/OFF*/
	LOGF_KLOG_DEBUG(" %s shadow 0x%x gpio %d val %d\r\n", __func__,
		chip->shadow, gpio, val);
	if (val)
		chip->shadow |= BIT(gpio);
	else
		chip->shadow &= ~BIT(gpio);
	xrx500_sso_w32(chip->virt, chip->shadow, (get_cpu_select()? XRX500_SSO_CPU1:XRX500_SSO_CPU0));
	xrx500_sso_w32(chip->virt, val, XRX500_SSO_DUTYCYCLE(gpio));
	xrx500_sso_w32_mask(chip->virt, 0, XRX500_SSO_CON_SWU, XRX500_SSO_CON0);

}
/**
 * xrx_sso_dir_out() - gpio_chip->dir_out - set gpio direction.
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 * @val:    Value to be written to specified signal.
 *
 * Same as xrx500_sso_set, always returns 0.
 */
static int xrx500_sso_dir_out(struct gpio_chip *gc, unsigned gpio, int val)
{
	xrx500_sso_set(gc, gpio, val);
	return 0;
}

/**
 * xrx500_sso_request() - gpio_chip->request
 * @gc:     Pointer to gpio_chip device structure.
 * @gpio:   GPIO signal number.
 *
 * We mask out the HW driven pins
 */
static int xrx500_sso_request(struct gpio_chip *gc, unsigned gpio)
{
	struct xrx500_sso *chip =
		container_of(gc, struct xrx500_sso, gc);

	if (/*(gpio < 15) && */(chip->reserved & BIT(gpio))) {
		LOGF_KLOG_DEV_ERROR(gc->dev, "GPIO %d is driven by hardware\n", gpio);
		return -ENODEV;
	}
	if ((BIT(gpio) & chip->blink_mask) == BIT(gpio)) {
		/*Its a blink masked gpio, so check the US*/
		if (chip->upd == LED_CON1_UPDATE_SRC_SW) {
			/*throw a warning, the update source is sw*/
			LOGF_KLOG_DEV_ERROR(gc->dev,
			"GPIO %d configure the update source to be FPID or GPTD\n",
			gpio);
		}
	}
	return 0;
}

#define find_range(delay_on, delay_off, table, value) \
for (j = 0; j < 4; j++) { \
	if ( \
	(delay_on >= table[j].range_start \
	&& delay_on <= table[j].range_end) \
	|| \
	(delay_off >= table[j].range_start \
	&& delay_off <= table[j].range_end)) { \
		value = j; \
		break;\
	} \
} \

/* LED Related Code */
static void xrx500_sso_program_delay(
	int phys_id,
	unsigned long delay_on,
	unsigned long delay_off) {

	int j = 0, k = 0;
	int value = 0;
	/*program the individual programmable rate if
	proper range is found or fallback to default values*/
	if (!g_chip->blink_rate) {
		if (g_chip->upd == LED_CON1_UPDATE_SRC_FPID) {
			find_range(delay_on, delay_off, fpid_table, value);
			LOGF_KLOG_DEBUG("delay %lu %lu value from table %d %d\r\n",
			delay_on, delay_off, value, fpid_table[value].value);
			xrx500_sso_w32_mask(g_chip->virt,
			XRX500_SSO_FPID_MASK << XRX500_SSO_FPID_SHIFT,
			fpid_table[value].value << XRX500_SSO_FPID_SHIFT,
			XRX500_SSO_CON1);
		} else if (g_chip->upd == LED_CON1_UPDATE_SRC_GPTD2) {
			find_range(delay_on, delay_off, gptd_table, value);
			LOGF_KLOG_DEBUG("delay %lu %lu value from table %d %d\r\n",
			delay_on, delay_off,
			value, gptd_table[value].value);
			xrx500_sso_w32_mask(g_chip->virt,
			XRX500_SSO_GPTD_MASK << XRX500_SSO_GPTD_SHIFT,
			gptd_table[value].value << XRX500_SSO_GPTD_SHIFT,
			XRX500_SSO_CON1);
		}
		return;
	}
	if ((phys_id >= 24) && (phys_id <= 31)) {
		k = phys_id - 24;
		if (g_chip->blink_source_24_31[k] == LED_H8_UPDATE_SRC_FPID) {
			find_range(delay_on, delay_off, fpid_table, value);
			xrx500_sso_w32_mask(g_chip->virt,
			0x3 << ((k%5) * 6),
			fpid_table[value].value << ((k%5) * 6),
			((phys_id < 29) ?
			XRX500_SSO_BLINK_H8_0 : XRX500_SSO_BLINK_H8_1));
		} else if (g_chip->blink_source_24_31[k]
			== LED_H8_UPDATE_SRC_GPTD) {
			find_range(delay_on, delay_off, gptd_table, value);
			xrx500_sso_w32_mask(g_chip->virt,
			0x3 << (((k%5) * 6) + 2),
			gptd_table[value].value << (((k%5) * 6) + 2),
			((phys_id < 29) ?
			XRX500_SSO_BLINK_H8_0 : XRX500_SSO_BLINK_H8_1));
		}
	} else if ((phys_id >= 0) && (phys_id <= 23)) {
		k = 3;
		if (g_chip->blink_source_0_23 == LED_H8_UPDATE_SRC_FPID) {
			find_range(delay_on, delay_off, fpid_table, value);
			xrx500_sso_w32_mask(g_chip->virt,
			0x3 << (k * 6),
			fpid_table[value].value << (k * 6),
			XRX500_SSO_BLINK_H8_1);
		} else if (g_chip->blink_source_0_23
			== LED_H8_UPDATE_SRC_GPTD) {
			find_range(delay_on, delay_off, gptd_table, value);
			xrx500_sso_w32_mask(g_chip->virt,
			0x3 << ((k * 6) + 2),
			gptd_table[value].value << ((k * 6) + 2),
			XRX500_SSO_BLINK_H8_1);
		}
	}
}
static int xrx500_sso_led_blink_set(struct led_classdev *device,
				unsigned long *delay_on,
				unsigned long *delay_off)

{
	struct xrx500_sso_led *led = (struct xrx500_sso_led *) device;
	u32 reg;
	if ((BIT(led->phys_id) & g_chip->blink_mask) == BIT(led->phys_id)) {
		reg = xrx500_sso_r32(g_chip->virt, XRX500_SSO_CON2);
		reg |= BIT(led->phys_id);
		xrx500_sso_program_delay(led->phys_id, *delay_on, *delay_off);
		xrx500_sso_w32(g_chip->virt, reg, XRX500_SSO_CON2);
		led->blinking = 1;
		return 0;
	} else{
		/*LOGF_KLOG_ERROR("blink mask not enabled\r\n");*/
		return -1;
	}
}

static void ledc_brightness_set(struct led_classdev *device,
	enum led_brightness value)
{
	u32 reg;
	struct xrx500_sso_led *led = (struct xrx500_sso_led *) device;

	xrx500_sso_w32(g_chip->virt, value, XRX500_SSO_DUTYCYCLE(led->phys_id));
	/*normal LED ON/OFF*/
	if (led->blinking && (value == 0)) {
			reg = xrx500_sso_r32(g_chip->virt, XRX500_SSO_CON2);
			reg &= ~BIT(led->phys_id);
			xrx500_sso_w32(g_chip->virt, reg, XRX500_SSO_CON2);
			led->blinking = 0;
	}
	if (!led->blinking) {
		if (value)
			g_chip->shadow |= BIT(led->phys_id);
		else
			g_chip->shadow &= ~BIT(led->phys_id);

		xrx500_sso_w32(g_chip->virt, g_chip->shadow, (get_cpu_select()? XRX500_SSO_CPU1:XRX500_SSO_CPU0));
		if (g_chip->upd == LED_CON1_UPDATE_SRC_SW)
			xrx500_sso_w32_mask(g_chip->virt,
			0, XRX500_SSO_CON_SWU, XRX500_SSO_CON0);
	}
}
static char* blink_src_get_24_31(int phys_id) {
	int i = phys_id % 24;
	u32 reg;
	reg = xrx500_sso_r32(g_chip->virt, ((i < 5) ?	XRX500_SSO_BLINK_H8_0 : XRX500_SSO_BLINK_H8_1));
	reg = (reg >> (((i%5) * 6) + 4)) & 0x3;
	switch (reg) {
		case 0:
			return "FPID";
		break;
		case 1:
			return "GPTD";
		break;
		case 2:
			return "V1M";
		break;
		default:
			return "Error";
	}
}
static void blink_src_set_24_31(char *source, int phys_id) {
	int i = phys_id % 24;
	int src = 0;
	if (!strcmp(source, "FPID")) {
		src = 0;
	} else if (!strcmp(source, "GPTD")) {
		src = 1;
	} else if (!strcmp(source, "V1M")) {
		src = 2;
	}
	xrx500_sso_w32_mask(g_chip->virt,
	0x3 << (((i%5) * 6) + 4),
	src << (((i%5) * 6) + 4),
	((i < 5) ?
	XRX500_SSO_BLINK_H8_0 : XRX500_SSO_BLINK_H8_1));		

}
static void blink_src_set(struct led_classdev *device, char *source)
{
	struct xrx500_sso_led *led = (struct xrx500_sso_led *) device;
	if(led->phys_id <= 23) {
		LOGF_KLOG_ERROR("blink_src is not supported for LEDS 0-23 instead use blink_src_0_23 in the top directory\r\n");
	} else {
		blink_src_set_24_31(source, led->phys_id);
	}
	
}
static char *blink_src_get(struct led_classdev *device)
{
	struct xrx500_sso_led *led = (struct xrx500_sso_led *) device;
	if(led->phys_id <= 23)
		return "blink_src is not supported for LEDS 0-23 instead use blink_src_0_23 in the top directory\r\n";
	else
		return blink_src_get_24_31(led->phys_id);
}

/* Code to create from OpenFirmware platform devices */
static int gpio_leds_create_of(struct platform_device *pdev)
{
	struct device_node *np = pdev->dev.of_node, *child;
	int count, i = 0;

	/* count LEDs in this device, so we know how much to allocate */
	count = of_get_child_count(np);
	if (!count)
		return -ENODEV;

	for_each_child_of_node(np, child) {
		struct xrx500_sso_led *led_dat = &g_xrx500_sso_led_config[i];
		led_dat->device.name = of_get_property(child, "label",
						     NULL) ? : child->name;
		LOGF_KLOG_DEBUG("name %s\r\n", led_dat->device.name);
		led_dat->device.default_trigger = of_get_property(child,
						"linux,default-trigger", NULL);
		of_property_read_u32(child, "max-brightness",
				&led_dat->device.max_brightness);

		of_property_read_u32(child, "reg", &led_dat->phys_id);
		led_dat->device.brightness_set = ledc_brightness_set;
		/*led_dat->device.led_source_set = ledc_source_set;
		led_dat->device.led_source_get = ledc_source_get;*/
		led_dat->device.blink_src_set = blink_src_set;
		led_dat->device.blink_src_get = blink_src_get;
		
		led_dat->device.blink_set = xrx500_sso_led_blink_set;
		led_dat->device.brightness = LED_OFF;
		led_dat->device.flags |= LED_CORE_SUSPENDRESUME;
		if ((led_classdev_register(&pdev->dev, &led_dat->device)) != 0)
			return -ENODEV;
		else
			LOGF_KLOG_DEBUG("classdev registered successfully\r\n");
		
		i++;
	}

	return 0;
}

static int sso_led_init(struct platform_device *pdev)
{
	struct pinctrl *pinctrl;

	pinctrl = devm_pinctrl_get_select_default(&pdev->dev);
	if (IS_ERR(pinctrl))
		LOGF_KLOG_DEV_WARNING(&pdev->dev,
			"pins are not configured from the driver\n");
	gpio_leds_create_of(pdev);

	return 0;
}

/**
 * xrx500_sso_hw_init() - Configure the SSO unit and enable the clock gate
 * @virt: pointer to the remapped register range
 */
static int xrx500_sso_hw_init(struct xrx500_sso *chip,
struct xrx500_sso_config_param *param)
{
	u32 i  = 0;
	struct clk *test_clk;
	int ret = 0;
	/* sane defaults */
	xrx500_sso_w32(chip->virt, 0, XRX500_SSO_AR);
	xrx500_sso_w32(chip->virt, 0, XRX500_SSO_CPU0);
	xrx500_sso_w32(chip->virt, 0, XRX500_SSO_CPU1);
	xrx500_sso_w32(chip->virt, 0, XRX500_SSO_CON1);

	/* apply edge trigger settings for the shift register */
	xrx500_sso_w32_mask(chip->virt, XRX500_SSO_EDGE_MASK,
				chip->edge, XRX500_SSO_CON0);

	/* apply led group settings */
	xrx500_sso_w32_mask(chip->virt, XRX500_SSO_GROUP_MASK,
				chip->groups, XRX500_SSO_CON1);

	/* tell the hardware which pins are controlled by the PHY*/
	xrx500_sso_w32_mask(chip->virt,
	XRX500_SSO_LD0_MASK,
	XRX500_SSO_LD0_PATTERN(chip->phy6, chip->phy2,
	chip->phy3, chip->phy4, chip->phy5),
	XRX500_SSO_CON3);
	xrx500_sso_w32_mask(chip->virt,
	XRX500_SSO_LD1_MASK,
	XRX500_SSO_LD1_PATTERN(chip->phy6, chip->phy2,
	chip->phy3, chip->phy4, chip->phy5),
	XRX500_SSO_CON3);
	xrx500_sso_w32_mask(chip->virt,
	XRX500_SSO_LD2_MASK,
	XRX500_SSO_LD2_PATTERN(chip->phy6, chip->phy2,
	chip->phy3, chip->phy4, chip->phy5),
	XRX500_SSO_CON3);
	/* tell the hardware which pins are controlled by the WLAN*/
	xrx500_sso_w32_mask(chip->virt,
	XRX500_SSO_W1_W2_W3_MASK,
	XRX500_SSO_W1_W2_W3_PATTERN(chip->wlan),
	XRX500_SSO_CON3);
	xrx500_sso_w32_mask(chip->virt,
	XRX500_SSO_W4_W5_W6_MASK,
	XRX500_SSO_W4_W5_W6_PATTERN(chip->wlan),
	XRX500_SSO_CON3);


	/* mask out the hw driven bits in gpio_request */
	chip->reserved =
	XRX500_SSO_LD0_PATTERN(chip->phy6, chip->phy2,
	chip->phy3, chip->phy4, chip->phy5)
	| XRX500_SSO_LD1_PATTERN(chip->phy6, chip->phy2,
	chip->phy3, chip->phy4, chip->phy5)
	| XRX500_SSO_LD2_PATTERN(chip->phy6, chip->phy2,
	chip->phy3, chip->phy4, chip->phy5);
	/*WLAN pins are mapped to SOUT4 as per the registers specification*/
	if (chip->wlan)
		chip->reserved |= (1 << 4);
	if (param == NULL)
		return 0;

	if (param->operation_mask & XRX500_SSO_CFG_OP_UPDATE_SHADOW) {
		/* store the shadow value if one was passed by the config */
		chip->shadow = param->shadow;
	}
	if (param->operation_mask & XRX500_SSO_CFG_OP_BLINK_MASK) {
		/* store the blink mask value if one was passed by the config */
		chip->blink_mask = param->blink_mask;
	}
	if (param->operation_mask & XRX500_SSO_CFG_OP_UPDATE_CLOCK) {
		chip->upd = param->update_clock;
		xrx500_sso_w32_mask(chip->virt,
			XRX500_SSO_UPD_MASK << XRX500_SSO_UPD_SHIFT,
			chip->upd << XRX500_SSO_UPD_SHIFT, XRX500_SSO_CON1);
	}
	if (param->operation_mask & XRX500_SSO_CFG_OP_UPDATE_FCDSC) {
		xrx500_sso_w32_mask(chip->virt,
		XRX500_SSO_FCDSC_MASK << XRX500_SSO_FCDSC_SHIFT,
		param->fcdsc << XRX500_SSO_FCDSC_SHIFT, XRX500_SSO_CON1);
	}
	if (param->operation_mask & XRX500_SSO_CFG_OP_UPDATE_FPID) {
		xrx500_sso_w32_mask(chip->virt,
		XRX500_SSO_FPID_MASK << XRX500_SSO_FPID_SHIFT,
			param->fpid << XRX500_SSO_FPID_SHIFT, XRX500_SSO_CON1);
	}
	if (param->operation_mask & XRX500_SSO_CFG_OP_UPDATE_GPTD) {
		xrx500_sso_w32_mask(chip->virt,
		XRX500_SSO_GPTD_MASK << XRX500_SSO_GPTD_SHIFT,
			param->gptd << XRX500_SSO_GPTD_SHIFT, XRX500_SSO_CON1);
	}
	if (param->operation_mask & XRX500_SSO_CFG_OP_BLINK_RATE) {
		chip->blink_rate = param->is_blink_rate_diff;
		xrx500_sso_w32_mask(chip->virt,
		XRX500_SSO_BLINK_RATE_MASK << XRX500_SSO_BLINK_RATE_SHIFT, chip->blink_rate << XRX500_SSO_BLINK_RATE_SHIFT, XRX500_SSO_CON0);
	}
	/*program the individual blinking source for leds 24:31*/
	if (param->operation_mask & XRX500_SSO_CFG_OP_INDIVIDUAL_BLINK_SOURCE) {
		for (i = 0; i < 8; i++) {
			chip->blink_source_24_31[i] =
			param->blink_rate_source_0[i];
			xrx500_sso_w32_mask(chip->virt,
			0x3 << (((i%5) * 6) + 4),
			chip->blink_source_24_31[i] << (((i%5) * 6) + 4),
			((i < 5) ?
			XRX500_SSO_BLINK_H8_0 : XRX500_SSO_BLINK_H8_1));
		}
	}
	/*program the blinking source for leds 0:23*/
	if (param->operation_mask & XRX500_SSO_CFG_OP_BLINK_SOURCE_0_23) {
			chip->blink_source_0_23 =
			param->blink_rate_source_1;
			xrx500_sso_w32_mask(chip->virt,
			0x3 << XRX500_SSO_BLINK_SOURCE_SHIFT,
			chip->blink_source_0_23
			<< XRX500_SSO_BLINK_SOURCE_SHIFT,
			XRX500_SSO_BLINK_H8_1);
	}
	#if 1
	test_clk = clk_get_sys("16b00000.gptu", "timer2a");
	if (IS_ERR(test_clk)) {
		LOGF_KLOG_ERROR("Failed to get clock timer2a\r\n");
		return -ENOENT;
	} else {
#ifdef CONFIG_USE_EMULATOR
			clk_set_rate(test_clk, 32258);
#else
			clk_set_rate(test_clk, 200000000);
#endif
			ret = clk_enable(test_clk);
			if (ret) {
				LOGF_KLOG_ERROR("%s enable failed\r\n", __func__);
				clk_set_rate(test_clk, 0);
			}
	}
	#else
	ret = lq_request_timer(TIMER2A,
	TIMER_FLAG_16BIT|TIMER_FLAG_TIMER|TIMER_FLAG_CYCLIC|TIMER_FLAG_DOWN,
	32258, 0, 0);
	if (!ret) {
		LOGF_KLOG_INFO("timer request successful\r\n");
		if (!lq_start_timer(TIMER2A, 1))
			LOGF_KLOG_INFO("timer started successfully\r\n");
	}
	#endif
	return 0;
}
static int xrx500_sso_probe(struct platform_device *pdev)
{
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	const __be32 *groups, *phy, *wlan;
	struct clk *clk;

	int ret = 0;
	struct xrx500_sso_config_param config = g_board_xrx500_sso_hw_config;

	if (!res) {
		LOGF_KLOG_DEV_ERROR(&pdev->dev, "failed to request SSO resource\n");
		return -ENOENT;
	}

	g_chip = devm_kzalloc(&pdev->dev, sizeof(*g_chip), GFP_KERNEL);
	if (!g_chip)
		return -ENOMEM;

	g_chip->virt = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(g_chip->virt))
		return PTR_ERR(g_chip->virt);

	g_chip->gc.dev = &pdev->dev;
	g_chip->gc.label = "sso-xrx500";
	g_chip->gc.direction_output = xrx500_sso_dir_out;
	g_chip->gc.set = xrx500_sso_set;
	g_chip->gc.request = xrx500_sso_request;
	g_chip->gc.base = -1;
	g_chip->gc.owner = THIS_MODULE;



	/* find out which gpio groups should be enabled */
	groups = of_get_property(pdev->dev.of_node, "lantiq,groups", NULL);
	if (groups)
		g_chip->groups = be32_to_cpu(*groups) & XRX500_SSO_GROUP_MASK;
	else
		g_chip->groups = XRX500_SSO_GROUP0;
	g_chip->gc.ngpio = fls(g_chip->groups) * 8;
		phy = of_get_property(pdev->dev.of_node, "lantiq,phy2", NULL);
		if (phy)
			g_chip->phy2 = be32_to_cpu(*phy) & XRX500_SSO_PHY_MASK;
		phy = of_get_property(pdev->dev.of_node, "lantiq,phy3", NULL);
		if (phy)
			g_chip->phy3 = be32_to_cpu(*phy) & XRX500_SSO_PHY_MASK;
		phy = of_get_property(pdev->dev.of_node, "lantiq,phy4", NULL);
		if (phy)
			g_chip->phy4 = be32_to_cpu(*phy) & XRX500_SSO_PHY_MASK;
		phy = of_get_property(pdev->dev.of_node, "lantiq,phy5", NULL);
		if (phy)
			g_chip->phy5 = be32_to_cpu(*phy) & XRX500_SSO_PHY_MASK;
		phy = of_get_property(pdev->dev.of_node, "lantiq,phy6", NULL);
		if (phy)
			g_chip->phy6 = be32_to_cpu(*phy) & XRX500_SSO_PHY_MASK;
		wlan = of_get_property(pdev->dev.of_node, "lantiq,wlan", NULL);
		if (wlan)
			g_chip->wlan = be32_to_cpu(*wlan)
			& XRX500_SSO_WLAN_MASK;

	/* check which edge trigger we should use, default to a falling edge */
	if (!of_find_property(pdev->dev.of_node, "lantiq,rising", NULL))
		g_chip->edge = XRX500_SSO_FALLING;
#ifndef CONFIG_USE_EMULATOR
	clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(clk)) {
		LOGF_KLOG_DEV_ERROR(&pdev->dev, "Failed to get clock\n");
		return PTR_ERR(clk);
	}
	clk_enable(clk);
#endif

	ret = xrx500_sso_hw_init(g_chip, &config);
	if (!ret)
		ret = gpiochip_add(&g_chip->gc);

	sso_led_init(pdev);
	platform_set_drvdata(pdev, g_chip);
	if (!ret) {
		ret = grx500_register_sysfs(pdev);
	if (ret) {
			LOGF_KLOG_DEV_ERROR(&pdev->dev, "registering config sysfs failed\n");
			return ret;
	}

		LOGF_KLOG_DEV_INFO(&pdev->dev, "Init done\n");
	}
	return ret;
}

static const struct of_device_id xrx500_sso_match[] = {
	{ .compatible = "lantiq,gpio-sso-xrx500" },
	{},
};

static struct platform_driver xrx500_sso_driver = {
	.probe = xrx500_sso_probe,
	.driver = {
		.name = "gpio-sso-xrx500",
		.owner = THIS_MODULE,
		.of_match_table = xrx500_sso_match,
	},
};

int __init xrx500_sso_init(void)
{
	return platform_driver_register(&xrx500_sso_driver);
}

#if 0
int __init xrx500_sso_init(void)
{
	return platform_driver_register(&xrx500_sso_driver);
}

core_initcall_sync(xrx500_sso_init);
#else
module_platform_driver(xrx500_sso_driver);
#endif
