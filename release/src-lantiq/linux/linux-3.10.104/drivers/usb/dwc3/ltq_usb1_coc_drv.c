/*
 *
 *			 Copyright (c) 2012, 2014, 2015
 *       		Lantiq Beteiligungs-GmbH & Co. KG
 *
 *  For licensing information, see the file 'LICENSE' in the root folder of
 *  this software module.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/timer.h>
#include <linux/cpufreq.h>
#include <cpufreq/ltq_cpufreq.h>

#include <linux/gpio.h>
#include <linux/delay.h>

#define LTQ_USB1COCDRV_BUSY	1
#define LTQ_USB1COCDRV_IDLE	0

/* this ID represent the USB interface No. (0, 1, 2, 3, ...) */
#define LTQ_USB_ID		1

/* threshold data for D0:D3 */
static struct ltq_cpufreq_threshold *th_data = NULL;

/* driver is busy and needs highest performance */
int usb1_cocdrv_busy = LTQ_USB1COCDRV_IDLE;

/* current power state of the driver */
enum ltq_cpufreq_state usb1_cocdrv_ps = LTQ_CPUFREQ_PS_D0;

#define PORT_CONNECT  (1 << 0)
#define PORT_PE		  (1 << 1)

#define PORT_PLS_MASK (0xf << 5)
#define XDEV_U0       (0x0 << 5)
#define XDEV_U2       (0x2 << 5)
#define XDEV_U3       (0x3 << 5)
#define XDEV_DISABLED (0x4 << 5)
#define XDEV_RXDETECT (0x5 << 5)

#define PORT_POWER	  (1 << 9)
/* Port Link State Write Strobe - set this when changing link state */
#define PORT_LINK_STROBE	(1 << 16)

static u32 __iomem *port_array[] = {0xba500420, 0xba500430};

static int
ltq_usb1_cocdrv_stateget(enum ltq_cpufreq_state *state)
{
	/*pr_info("%s is called\n", __func__);*/
	*state = usb1_cocdrv_ps;
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
ltq_usb1_cocdrv_fss_ena(int ena)
{
	/*pr_info("%s is called\n", __func__);*/
	if (ena) {
		pr_debug("enable frequency scaling\n");
	} else {
		pr_debug("disable frequency scaling\n");
	}
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
ltq_usb1_cocdrv_prechange(enum ltq_cpufreq_module module,
			enum ltq_cpufreq_state new_state,
			enum ltq_cpufreq_state old_state,
		        u8 flags)
{
	u32 value;
	int usb3_index = 1;

	/*pr_info("%s is called\n", __func__);*/
	if (usb1_cocdrv_busy == LTQ_USB1COCDRV_IDLE) {
		/* do what ever is necessary to prepare the drv
		for a frequency change. */
		if(new_state == LTQ_CPUFREQ_PS_D3) {
			value = readl(port_array[usb3_index]);
			if (value & PORT_CONNECT)
				return LTQ_CPUFREQ_RETURN_DENIED;
		}
		return LTQ_CPUFREQ_RETURN_SUCCESS;
	} else if (flags & CPUFREQ_PM_NO_DENY) {
		/* this flag is set if we need thermal reduction and
		   frequency down scaling is a must to avoid thermal problems.*/
		return LTQ_CPUFREQ_RETURN_SUCCESS;
	} else {
		if (new_state != LTQ_CPUFREQ_PS_D0)
			return LTQ_CPUFREQ_RETURN_DENIED; /*avoid down scaling*/
	}
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
ltq_usb1_cocdrv_statechange(enum ltq_cpufreq_state new_state)
{
	/*pr_info("%s is called\n",__func__);*/
	/* do what ever is necessary to make changes in the drv
	to support the new frequency. */
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
ltq_usb1_cocdrv_postchange(enum ltq_cpufreq_module module,
			enum ltq_cpufreq_state new_state,
			enum ltq_cpufreq_state old_state,
			u8 flags)
{
	u32 value;
	int usb2_index = 0, usb3_index = 1;

	/*pr_info("%s is called\n", __func__);*/
	/* do what ever is necessary to cleanup things in the drv
	after a frequency change. */

	if (new_state == LTQ_CPUFREQ_PS_D0) {
		value = readl(port_array[usb3_index]);
		if ((value & PORT_PLS_MASK) == XDEV_DISABLED) {
			/*Power-off USB2 PORT*/
			value = readl(port_array[usb2_index]);
			value &= ~PORT_POWER;
			writel(value, port_array[usb2_index]);

			/*Transition USB3 PORT state to RXDETECT*/
			value = readl(port_array[usb3_index]);
			value &= ~PORT_PLS_MASK;
			value |= XDEV_RXDETECT | PORT_LINK_STROBE;
			writel(value, port_array[usb3_index]);

			/*Power-on USB2 PORT*/
			value = readl(port_array[usb2_index]);
			value |= PORT_POWER;
			writel(value, port_array[usb2_index]);
		}
	}
	else if(new_state == LTQ_CPUFREQ_PS_D3) {
		value = readl(port_array[usb3_index]);
		if((value & PORT_PLS_MASK) != XDEV_DISABLED) {
			/*Disable USB3 PORT*/
			value |= PORT_PE;
			writel(value, port_array[usb3_index]);
		}
	}

	usb1_cocdrv_ps = new_state;
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

/* This function should be called if the driver becomes BUSY */
void
ltq_usb1_cocdrv_busy(void)
{
	int ret = LTQ_CPUFREQ_RETURN_SUCCESS;

	usb1_cocdrv_busy = LTQ_USB1COCDRV_BUSY;
	ret = ltq_cpufreq_state_req(LTQ_CPUFREQ_MODULE_USB, LTQ_USB_ID,
							LTQ_CPUFREQ_PS_D0);
	if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
		pr_debug("Power state request D0 failed");
}

/* This function should be called if the driver becomes IDLE */
void
ltq_usb1_cocdrv_idle(void)
{
	int ret = LTQ_CPUFREQ_RETURN_SUCCESS;

	usb1_cocdrv_busy = LTQ_USB1COCDRV_IDLE;
	ret = ltq_cpufreq_state_req(LTQ_CPUFREQ_MODULE_USB, LTQ_USB_ID,
							LTQ_CPUFREQ_PS_D0D3);
	if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
		pr_err("Power state request D0D3 failed");
}

/* keep track of frequency transitions */
static int
usb1_cocdrv_cpufreq_notifier(struct notifier_block *nb, unsigned long val,
							void *data)
{
	struct cpufreq_freqs *freq = data;
	enum ltq_cpufreq_state new_state, old_state;
	int ret;
	pr_debug("%s is called\n", __func__);

	new_state = ltq_cpufreq_get_ps_from_khz(freq->new);
	if (new_state == LTQ_CPUFREQ_PS_UNDEF)
		return NOTIFY_STOP_MASK | (LTQ_CPUFREQ_MODULE_PCIE<<4);
	old_state = ltq_cpufreq_get_ps_from_khz(freq->old);
	if (old_state == LTQ_CPUFREQ_PS_UNDEF)
		return NOTIFY_STOP_MASK | (LTQ_CPUFREQ_MODULE_PCIE<<4);
	if (val == CPUFREQ_PRECHANGE) {
		ret = ltq_usb1_cocdrv_prechange(LTQ_CPUFREQ_MODULE_USB,
					new_state, old_state, freq->flags);
		if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
			return NOTIFY_STOP_MASK | (LTQ_CPUFREQ_MODULE_USB<<4);
		ret = ltq_usb1_cocdrv_statechange(new_state);
		if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
			return NOTIFY_STOP_MASK | (LTQ_CPUFREQ_MODULE_USB<<4);
	} else if (val == CPUFREQ_POSTCHANGE) {
		ret = ltq_usb1_cocdrv_postchange(LTQ_CPUFREQ_MODULE_USB,
					new_state, old_state, freq->flags);
		if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
			return NOTIFY_STOP_MASK | (LTQ_CPUFREQ_MODULE_USB<<4);
	} else {
		return NOTIFY_OK | (LTQ_CPUFREQ_MODULE_USB<<4);
	}
	return NOTIFY_OK | (LTQ_CPUFREQ_MODULE_USB<<4);
}

static struct notifier_block usb1_cocdrv_cpufreq_notifier_block = {
	.notifier_call = usb1_cocdrv_cpufreq_notifier
};

static struct ltq_cpufreq_module_info usb1_cocdrv_feature_fss = {
.name                           = "USB1 frequency scaling support",
.pmcuModule                     = LTQ_CPUFREQ_MODULE_USB,
.pmcuModuleNr                   = LTQ_USB_ID,
.powerFeatureStat               = 1,
.ltq_cpufreq_state_get          = ltq_usb1_cocdrv_stateget,
.ltq_cpufreq_pwr_feature_switch = ltq_usb1_cocdrv_fss_ena,
};

static int
ltq_usb1_cocdrv_cpufreq_init(void)
{
        int ret;
                
	pr_debug("%s is called\n", __func__);
	if (cpufreq_register_notifier(&usb1_cocdrv_cpufreq_notifier_block,
						CPUFREQ_TRANSITION_NOTIFIER)) {
		pr_err("Fail in registering USB1_COCDRV to CPUFreq\n");
		return -1;
	}
	ltq_cpufreq_mod_list(&usb1_cocdrv_feature_fss.list, LTQ_CPUFREQ_LIST_ADD);

	th_data = ltq_cpufreq_get_threshold(LTQ_CPUFREQ_MODULE_USB,
							LTQ_USB_ID);
	if (th_data == NULL)
		pr_debug("No PS related threshold values are defined ");


	ret = ltq_cpufreq_state_req(LTQ_CPUFREQ_MODULE_USB, LTQ_USB_ID,
							LTQ_CPUFREQ_PS_D0);
	if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
		pr_debug("Power state request D0 failed");

	pr_debug("Register USB1_COCDRV to CPUFREQ.\n");
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
ltq_usb1_cocdrv_cpufreq_exit(void)
{

	/* set status of usb1_cocdrv inside CPUFreq to don't care */
	ltq_usb1_cocdrv_idle();

	if (cpufreq_unregister_notifier(&usb1_cocdrv_cpufreq_notifier_block,
						CPUFREQ_TRANSITION_NOTIFIER)) {
		pr_err("CPUFREQ unregistration failed.");
		return -1;
	}
	ltq_cpufreq_mod_list(&usb1_cocdrv_feature_fss.list, LTQ_CPUFREQ_LIST_DEL);
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
__init ltq_usb1_cocdrv_init(void)
{
	ltq_usb1_cocdrv_cpufreq_init();
	return 0;
}

static void
__exit ltq_usb1_cocdrv_exit(void)
{
	ltq_usb1_cocdrv_cpufreq_exit();
	return;
}

module_init(ltq_usb1_cocdrv_init);
module_exit(ltq_usb1_cocdrv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Bartholomae, t.bartholomae@lantiq.com");
MODULE_DESCRIPTION("LANTIQ USB1_COCDRV driver");
MODULE_SUPPORTED_DEVICE("LANTIQ in general ;-)");

