/*
 * Driver for Texas Instruments INA219, INA226 power monitor chips
 *
 * INA219:
 * Zero Drift Bi-Directional Current/Power Monitor with I2C Interface
 * Datasheet: http://www.ti.com/product/ina219
 *
 * INA220:
 * Bi-Directional Current/Power Monitor with I2C Interface
 * Datasheet: http://www.ti.com/product/ina220
 *
 * INA226:
 * Bi-Directional Current/Power Monitor with I2C Interface
 * Datasheet: http://www.ti.com/product/ina226
 *
 * INA230:
 * Bi-directional Current/Power Monitor with I2C Interface
 * Datasheet: http://www.ti.com/product/ina230
 *
 * Copyright (C) 2012 Lothar Felten <l-felten@ti.com>
 * Thanks to Jan Volkering
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/jiffies.h>
#include <linux/of.h>

#include <linux/platform_data/ina2xx.h>

/* common register definitions */
#define INA2XX_CONFIG			0x00
#define INA2XX_SHUNT_VOLTAGE		0x01 /* readonly */
#define INA2XX_BUS_VOLTAGE		0x02 /* readonly */
#define INA2XX_POWER			0x03 /* readonly */
#define INA2XX_CURRENT			0x04 /* readonly */
#define INA2XX_RAILNAME			0x14 /* readonly */
#define INA2XX_CALIBRATION		0x05

/* INA226 register definitions */
#define INA226_MASK_ENABLE		0x06
#define INA226_ALERT_LIMIT		0x07
#define INA226_DIE_ID			0xFF


/* register count */
#define INA219_REGISTERS		6
#define INA226_REGISTERS		8

#define INA2XX_MAX_REGISTERS		8

/* settings - depend on use case */
#define INA219_CONFIG_DEFAULT		0x399F	/* PGA=8 */
#define INA226_CONFIG_DEFAULT		0x4527	/* averages=16 */

/* worst case is 68.10 ms (~14.6Hz, ina219) */
#define INA2XX_CONVERSION_RATE		15

#define RAIL_NAME_MAX_LENGTH		20

enum ina2xx_ids { ina219, ina226 };

struct ina2xx_config {
	u16 config_default;
	int calibration_factor;
	int registers;
	int shunt_div;
	int bus_voltage_shift;
	int bus_voltage_lsb;	/* uV */
	int current_lsb_def;	/* uA */
	int power_lsb_def;	/* uW */
};

struct ina2xx_data {
	struct device *hwmon_dev;
	char rail_name[RAIL_NAME_MAX_LENGTH];
	const struct ina2xx_config *config;

	int current_lsb;	/* uA */
	int power_lsb;		/* uW */

	struct mutex update_lock;
	bool valid;
	unsigned long last_updated;

	int kind;
	u16 regs[INA2XX_MAX_REGISTERS];
};

static const struct ina2xx_config ina2xx_config[] = {
	[ina219] = {
		.config_default = INA219_CONFIG_DEFAULT,
		.calibration_factor = 40960000,
		.registers = INA219_REGISTERS,
		.shunt_div = 100,
		.bus_voltage_shift = 3,
		.bus_voltage_lsb = 4000,
		.current_lsb_def = 1000,
		.power_lsb_def = 20000,
	},
	[ina226] = {
		.config_default = INA226_CONFIG_DEFAULT,
		.calibration_factor = 5120000,
		.registers = INA226_REGISTERS,
		.shunt_div = 400,
		.bus_voltage_shift = 0,
		.bus_voltage_lsb = 1250,
		.current_lsb_def = 1000,
		.power_lsb_def = 25000,
	},
};

static struct ina2xx_data *ina2xx_update_device(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct ina2xx_data *data = i2c_get_clientdata(client);
	struct ina2xx_data *ret = data;

	mutex_lock(&data->update_lock);

	if (time_after(jiffies, data->last_updated +
		       HZ / INA2XX_CONVERSION_RATE) || !data->valid) {

		int i;

		dev_dbg(&client->dev, "Starting ina2xx update\n");

		/* Read all registers */
		for (i = 0; i < data->config->registers; i++) {
			int rv = i2c_smbus_read_word_swapped(client, i);
			if (rv < 0) {
				ret = ERR_PTR(rv);
				goto abort;
			}
			data->regs[i] = rv;
		}
		data->last_updated = jiffies;
		data->valid = 1;
	}
abort:
	mutex_unlock(&data->update_lock);
	return ret;
}

static int ina2xx_get_value(struct ina2xx_data *data, u8 reg)
{
	int val;

	switch (reg) {
	case INA2XX_SHUNT_VOLTAGE:
		/* signed register */
		val = DIV_ROUND_CLOSEST((s16)data->regs[reg],
					data->config->shunt_div);
		break;
	case INA2XX_BUS_VOLTAGE:
		val = (data->regs[reg] >> data->config->bus_voltage_shift)
		  * data->config->bus_voltage_lsb;
		val = DIV_ROUND_CLOSEST(val, 1000);
		break;
	case INA2XX_POWER:
		val = data->regs[reg] * data->power_lsb;
		break;
	case INA2XX_CURRENT:
		val = (s16)data->regs[reg] * data->current_lsb;
		break;
	case INA2XX_CALIBRATION:
		val = data->regs[reg];
		break;
	case INA2XX_CONFIG:
		val = data->regs[reg];
		break;
	default:
		/* programmer goofed */
		WARN_ON_ONCE(1);
		val = 0;
		break;
	}

	return val;
}

static int ina2xx_set_value(struct i2c_client *client, struct ina2xx_data *data,
				u8 reg, int val)
{
	switch (reg) {
	case INA2XX_CALIBRATION:
		data->regs[reg] = val;
		i2c_smbus_write_word_swapped(client, INA2XX_CALIBRATION, val);
		break;
	case INA2XX_CONFIG:
		data->regs[reg] = val;
		i2c_smbus_write_word_swapped(client, INA2XX_CONFIG, val);
		break;
	default:
		/* programmer goofed */
		WARN_ON_ONCE(1);
		val = 0;
		break;
	}
	return 0;
}

static ssize_t ina2xx_show_value(struct device *dev,
				 struct device_attribute *da, char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct ina2xx_data *data = ina2xx_update_device(dev);

	if (IS_ERR(data))
		return PTR_ERR(data);

	if (attr->index == INA2XX_RAILNAME) {
		return snprintf(buf, PAGE_SIZE, "%s\n", data->rail_name);
	} else if (attr->index == INA2XX_CONFIG) {
		return snprintf(buf, PAGE_SIZE, "0x%x\n",
			ina2xx_get_value(data, attr->index));
	} else {
		return snprintf(buf, PAGE_SIZE, "%d\n",
			ina2xx_get_value(data, attr->index));
	}
}

static ssize_t ina2xx_store_value(struct device *dev,
				struct device_attribute *da, const char *buf,
				size_t count)
{
	long val;
	int err;
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct i2c_client *client = to_i2c_client(dev);
	struct ina2xx_data *data = i2c_get_clientdata(client);

	err = kstrtol(buf, 0, &val);
	if (err)
		return err;

	if (attr->index == INA2XX_CALIBRATION)
		ina2xx_set_value(client, data, attr->index, (int)val);

	if (attr->index == INA2XX_CONFIG)
		ina2xx_set_value(client, data, attr->index, (int)val);

	return count;
}

/* shunt voltage */
static SENSOR_DEVICE_ATTR(in0_input, S_IRUGO, ina2xx_show_value, NULL,
			  INA2XX_SHUNT_VOLTAGE);

/* bus voltage */
static SENSOR_DEVICE_ATTR(in1_input, S_IRUGO, ina2xx_show_value, NULL,
			  INA2XX_BUS_VOLTAGE);

/* calculated current */
static SENSOR_DEVICE_ATTR(curr1_input, S_IRUGO, ina2xx_show_value, NULL,
			  INA2XX_CURRENT);

/* calculated power */
static SENSOR_DEVICE_ATTR(power1_input, S_IRUGO, ina2xx_show_value, NULL,
			  INA2XX_POWER);

/* rail name */
static SENSOR_DEVICE_ATTR(rail_name, S_IRUGO, ina2xx_show_value, NULL,
			  INA2XX_RAILNAME);

/* calibration register */
static SENSOR_DEVICE_ATTR(cal_reg, S_IWUSR | S_IRUGO, ina2xx_show_value,
			  ina2xx_store_value, INA2XX_CALIBRATION);

/* configuration register */
static SENSOR_DEVICE_ATTR(config_reg, S_IWUSR | S_IRUGO, ina2xx_show_value,
			  ina2xx_store_value, INA2XX_CONFIG);

/* pointers to created device attributes */
static struct attribute *ina2xx_attributes[] = {
	&sensor_dev_attr_in0_input.dev_attr.attr,
	&sensor_dev_attr_in1_input.dev_attr.attr,
	&sensor_dev_attr_curr1_input.dev_attr.attr,
	&sensor_dev_attr_power1_input.dev_attr.attr,
	&sensor_dev_attr_rail_name.dev_attr.attr,
	&sensor_dev_attr_cal_reg.dev_attr.attr,
	&sensor_dev_attr_config_reg.dev_attr.attr,
	NULL,
};

static const struct attribute_group ina2xx_group = {
	.attrs = ina2xx_attributes,
};

static int ina2xx_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	struct i2c_adapter *adapter = client->adapter;
	struct ina2xx_data *data;
	struct ina2xx_platform_data *pdata;
	int len;
	const u32 *addr;
	int ret;
	u32 val, cal_factor;
	const char *railname = NULL;
	long shunt = 10000; /* default shunt value 10mOhms */

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_WORD_DATA))
		return -ENODEV;

	data = devm_kzalloc(&client->dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	if (client->dev.platform_data) {
		pdata =
		  (struct ina2xx_platform_data *)client->dev.platform_data;
		shunt = pdata->shunt_uohms;
	} else if (!of_property_read_u32(client->dev.of_node,
						"shunt-resistor", &val)) {
		shunt = val;
	}

	if (shunt <= 0)
		return -ENODEV;

	data->rail_name[0] = '\0'; /* init rail_name */
	if (!of_property_read_string(client->dev.of_node,
						"rail-name", &railname)) {
		strncpy(data->rail_name, railname, RAIL_NAME_MAX_LENGTH);
		data->rail_name[RAIL_NAME_MAX_LENGTH - 1] = '\0';
	}

	/* set the device type */
	data->kind = id->driver_data;
	data->config = &ina2xx_config[data->kind];


	addr = of_get_property(client->dev.of_node, "user-calibration", &len);
	if (addr != NULL) {
		i2c_smbus_write_word_swapped(client, INA2XX_CALIBRATION,
								*(addr+0));
		i2c_smbus_write_word_swapped(client, INA2XX_CONFIG, *(addr+1));
		data->current_lsb = *(addr+2);
		data->power_lsb = *(addr+3);
	} else {
		i2c_smbus_write_word_swapped(client, INA2XX_CONFIG,
					     data->config->config_default);
		cal_factor = data->config->calibration_factor;
		/* set current LSB to 1mA, shunt is in uOhms */
		/* (equation 13 in datasheet) */
		i2c_smbus_write_word_swapped(client, INA2XX_CALIBRATION,
						cal_factor / shunt);
		data->current_lsb = data->config->current_lsb_def;
		data->power_lsb = data->config->power_lsb_def;
	}

	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);

	ret = sysfs_create_group(&client->dev.kobj, &ina2xx_group);
	if (ret)
		return ret;

	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		ret = PTR_ERR(data->hwmon_dev);
		goto out_err_hwmon;
	}

	dev_info(&client->dev, "pm: %s, railname:  %s, Rshunt: %li uOhm\n",
						id->name, railname, shunt);

	return 0;

out_err_hwmon:
	sysfs_remove_group(&client->dev.kobj, &ina2xx_group);
	return ret;
}

static int ina2xx_remove(struct i2c_client *client)
{
	struct ina2xx_data *data = i2c_get_clientdata(client);

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &ina2xx_group);

	return 0;
}

static const struct i2c_device_id ina2xx_id[] = {
	{ "ina219", ina219 },
	{ "ina220", ina219 },
	{ "ina226", ina226 },
	{ "ina230", ina226 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, ina2xx_id);

static struct i2c_driver ina2xx_driver = {
	.driver = {
		.name	= "ina2xx",
	},
	.probe		= ina2xx_probe,
	.remove		= ina2xx_remove,
	.id_table	= ina2xx_id,
};

module_i2c_driver(ina2xx_driver);

MODULE_AUTHOR("Lothar Felten <l-felten@ti.com>");
MODULE_DESCRIPTION("ina2xx driver");
MODULE_LICENSE("GPL");
