/*
 * LED Kernel Sequence Trigger
 *
 * Copyright 2011-2012 
 *
 * Author: Ralf Janssen
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/device.h>
//#include <linux/sysdev.h>
#include <linux/timer.h>
#include <linux/ctype.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include "../leds.h"
#include <linux/klogging.h>

struct sequence_setting {
	unsigned long brightness;
	unsigned long delay;
};

struct sequence_trig_data {
	int final_brightness;		/* LED brightness after sequence finished. */
	int repeat;			/* repeat sequence n times (-1 for infinite) */
	int current_iteration;		/* current repetition */
	struct sequence_setting setting[100];
	int num_settings;
	int cur_setting;
	struct timer_list timer;
};

static void led_sequence_function(unsigned long data)
{
	struct led_classdev *led_cdev = (struct led_classdev *) data;
	struct sequence_trig_data *sequence_data = led_cdev->trigger_data;
	unsigned long brightness = sequence_data->final_brightness;
	unsigned long delay = 0;

	if (sequence_data->cur_setting < sequence_data->num_settings) {
		brightness = sequence_data->setting[sequence_data->cur_setting].brightness;
		delay = sequence_data->setting[sequence_data->cur_setting].delay;

		/* repeat sequence ? */
		if (++sequence_data->cur_setting >= sequence_data->num_settings) {
			if ((sequence_data->repeat == -1) || 
			    (++sequence_data->current_iteration < sequence_data->repeat)) {
				sequence_data->cur_setting = 0;
			}
		}
	}

	led_set_brightness(led_cdev, brightness);

	if (delay > 0)
		mod_timer(&sequence_data->timer, jiffies + msecs_to_jiffies(delay));

}

static ssize_t led_final_brightness_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct sequence_trig_data *sequence_data = led_cdev->trigger_data;

	return sprintf(buf, "%i\n", sequence_data->final_brightness);
}

static ssize_t led_final_brightness_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct sequence_trig_data *sequence_data = led_cdev->trigger_data;
	int ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;

	if (*after && isspace(*after))
		count++;

	if (count == size) {
		sequence_data->final_brightness = state;
		ret = count;
	}

	return ret;
}

static ssize_t led_repeat_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct sequence_trig_data *sequence_data = led_cdev->trigger_data;

	return sprintf(buf, "%i/%i\n", 
		sequence_data->current_iteration, sequence_data->repeat);
}

static ssize_t led_repeat_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct sequence_trig_data *sequence_data = led_cdev->trigger_data;
	int ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtol(buf, &after, 10);
	size_t count = after - buf;

	if (*after && isspace(*after))
		count++;

	if (count == size) {
		sequence_data->repeat = state;
		ret = count;
	}

	return ret;
}

static ssize_t led_sequence_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct sequence_trig_data *sequence_data = led_cdev->trigger_data;

	ssize_t count = 0;
	int i;
	/*check the sequence setting*/
	for (i = 0; i < sequence_data->num_settings; i++)
	  count += sprintf(buf + count, "(brightness,delay): %lu,%lu \n", 
			   sequence_data->setting[i].brightness, 
			   sequence_data->setting[i].delay);

	return count;
}

static ssize_t led_sequence_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	struct sequence_trig_data *sequence_data = led_cdev->trigger_data;
	char *copybuf;
	char *p, *token;
	char *pt;
	unsigned long brightness, delay;
	char *after;
	int ret = -EINVAL;
	int num_settings = 0;

	copybuf = kmalloc(1024, GFP_KERNEL);
	if(!copybuf) {
		LOGF_KLOG_ERROR("mem Alloc failed\r\n");
		return ret;
	}
	/* take a copy since we don't want to trash the inbound buffer when using strsep */
	strncpy(copybuf, buf, 1024);
	copybuf[1023] = 0;
	p = copybuf;
	pt = p;
	
	del_timer_sync(&sequence_data->timer);
	/* TODO: lock accesses to data structure */
	sequence_data->current_iteration = 0;
	sequence_data->num_settings = 0;
	sequence_data->cur_setting = 0;

	/*Debug on sequence*/
	LOGF_KLOG_CONT("Start to read sequence setting: %s \r\n", pt);
	while (p != NULL) {
		while((char)*p==' ')p++;
		token = strsep(&p, ",");
		brightness = simple_strtoul(token, &after, 10);
		while((char)*p==' ')p++;
		token = strsep(&p, ",");
		delay = simple_strtoul(token, &after, 10);

		LOGF_KLOG_CONT("setting%d brightness = %d delay = %d \r\n",num_settings, brightness, delay);
		sequence_data->setting[num_settings].brightness = brightness;
		sequence_data->setting[num_settings].delay = delay;
	
		if (++num_settings >= (sizeof(sequence_data->setting) / sizeof(sequence_data->setting[0])))
			break;
		}
	if ((p == NULL) && (num_settings <= (sizeof(sequence_data->setting) / sizeof(sequence_data->setting[0])))) {

		LOGF_KLOG_CONT("End of sequence setting, num = %d!\r\n",num_settings);		
		ret = size;
		sequence_data->num_settings = num_settings;
		led_sequence_function((unsigned long)led_cdev);
		
	}
	return ret;
}

static DEVICE_ATTR(final_brightness, 0644, led_final_brightness_show, led_final_brightness_store);
static DEVICE_ATTR(repeat, 0644, led_repeat_show, led_repeat_store);
static DEVICE_ATTR(sequence, 0644, led_sequence_show, led_sequence_store);

static void sequence_trig_activate(struct led_classdev *led_cdev)
{
	struct sequence_trig_data *sequence_data;
	int rc;

	sequence_data = kzalloc(sizeof(struct sequence_trig_data), GFP_KERNEL);
	if (!sequence_data)
		return;

	sequence_data->final_brightness = led_get_brightness(led_cdev);
	led_cdev->trigger_data = sequence_data;

	init_timer(&sequence_data->timer);
	sequence_data->timer.function = led_sequence_function;
	sequence_data->timer.data = (unsigned long) led_cdev;

	rc = device_create_file(led_cdev->dev, &dev_attr_final_brightness);
	if (rc)
		goto err_out;
	rc = device_create_file(led_cdev->dev, &dev_attr_repeat);
	if (rc)
		goto err_out_finalbrightness;
	rc = device_create_file(led_cdev->dev, &dev_attr_sequence);
	if (rc)
		goto err_out_repeat;

	return;

err_out_repeat:
	device_remove_file(led_cdev->dev, &dev_attr_repeat);
err_out_finalbrightness:
	device_remove_file(led_cdev->dev, &dev_attr_final_brightness);
err_out:
	led_cdev->trigger_data = NULL;
	kfree(sequence_data);
}

static void sequence_trig_deactivate(struct led_classdev *led_cdev)
{
	struct sequence_trig_data *sequence_data = led_cdev->trigger_data;

	if (sequence_data) {
		device_remove_file(led_cdev->dev, &dev_attr_sequence);
		device_remove_file(led_cdev->dev, &dev_attr_repeat);
		device_remove_file(led_cdev->dev, &dev_attr_final_brightness);
		del_timer_sync(&sequence_data->timer);
		kfree(sequence_data);
	}
}

static struct led_trigger sequence_led_trigger = {
	.name     = "sequence",
	.activate = sequence_trig_activate,
	.deactivate = sequence_trig_deactivate,
};

static int __init sequence_trig_init(void)
{
	return led_trigger_register(&sequence_led_trigger);
}

static void __exit sequence_trig_exit(void)
{
	led_trigger_unregister(&sequence_led_trigger);
}

module_init(sequence_trig_init);
module_exit(sequence_trig_exit);

MODULE_AUTHOR("Ralf Janssen");
MODULE_DESCRIPTION("Sequence LED trigger");
MODULE_LICENSE("GPL");
