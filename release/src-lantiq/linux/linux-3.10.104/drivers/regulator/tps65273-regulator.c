/*
 * tps65273.c - Voltage and current regulation for the TPS65273
 *
 * Copyright (C) 2015 Lantiq
 * Ho Nghia Duc <Ho Nghia Duc @ lantiq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*#define DEBUG*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/regulator/of_regulator.h>
#include <linux/slab.h>

#define DRV_NAME	"DCDC Regulator TPS65273"
#define DRV_VERSION	"1.1.0"

#define TPS65273_MIN_VOLTAGE 680000
#define TPS65273_MAX_VOLTAGE 1950000
#define TPS65273_STEP_VOLTAGE 10000


/* Registers */
enum {
	TPS65273_VOUT1_SEL,
	TPS65273_VOUT2_SEL,
	TPS65273_VOUT1_COM,
	TPS65273_VOUT2_COM,
	TPS65273_SYS_STATUS,
};

enum TPS_STATUS {
	TPS65273_DISABLE,
	TPS65273_ENABLE,
};

/* Register bit definition */
/* VOUT - Voltage control */
#define TPS65273_REG_VOUT_GO	BIT(7)
#define TPS65273_REG_VOUT_GO_OFFSET 7

/* COM - Command */
#define TPS65273_REG_COM_ENABLE	BIT(0)
#define TPS65273_REG_OFFSET	0

/* STATUS - Status */
#define TPS65273_REG_STATUS_TEMP BIT(2)
#define TPS65273_REG_STATUS_PGOOD2 BIT(1)
#define TPS65273_REG_STATUS_PGOOD1 BIT(0)



#define TPS65273_NUM_REGULATOR 2

struct tps65273_platform_data {
	u32 default_mode;
	u32 slew_rate[TPS65273_NUM_REGULATOR];
	u32 psm_mode[TPS65273_NUM_REGULATOR];
	u32 irdrop_offset[TPS65273_NUM_REGULATOR]; /*microvolt*/
	struct regulator_init_data *reg_data[TPS65273_NUM_REGULATOR];
};

struct tps65273_data {
	struct i2c_client	*client;
	struct device		*dev;
	struct tps65273_platform_data *pdata;
	struct regulator_desc desc[TPS65273_NUM_REGULATOR];
	struct regulator_dev *rdev[TPS65273_NUM_REGULATOR];
};

/* Regulator specific details */
struct tps_info {
	const char *name;
	u8 table_len;
	const unsigned int *table;
};

/* Struct passed as driver data */
struct tps_driver_data {
	const struct tps_info *info;
};
static int tps65273_is_enabled(struct regulator_dev *rdev);
static int tps65273_enable(struct regulator_dev *rdev);
static int tps65273_disable(struct regulator_dev *rdev);
static int tps65273_status(struct regulator_dev *rdev);
static int tps65273_get_voltage_sel(struct regulator_dev *rdev);
static int tps65273_set_voltage_sel(struct regulator_dev *rdev,
				   unsigned selector);
static int tps65273_enable_go(struct regulator_dev *rdev);
static int tps65273_disable_go(struct regulator_dev *rdev);
static int tps65273_get_go(struct regulator_dev *rdev);

static struct regulator_ops tps65273_ops = {
	.is_enabled	 = tps65273_is_enabled,
	.enable		 = tps65273_enable,
	.disable	 = tps65273_disable,
	.get_status	 = tps65273_status,
	.list_voltage	 = regulator_list_voltage_table,
	.map_voltage	 = regulator_map_voltage_ascend,
	.get_voltage_sel = tps65273_get_voltage_sel,
	.set_voltage_sel = tps65273_set_voltage_sel,
	.enable_ext_ctrl = tps65273_enable_go,
	.disable_ext_ctrl = tps65273_disable_go,
	.get_ext_ctrl	 = tps65273_get_go,
};

static const unsigned int vcore_vsel_table[] = {
	680000, 690000, 700000, 710000, 720000, 730000, 740000, 750000,
	760000, 770000, 780000, 790000, 800000, 810000, 820000, 830000,
	840000, 850000, 860000, 870000, 880000, 890000, 900000, 910000,
	920000, 930000, 940000, 950000, 960000, 970000, 980000, 990000,
	1000000, 1010000, 1020000, 1030000, 1040000, 1050000, 1060000, 1070000,
	1080000, 1090000, 1100000, 1110000, 1120000, 1130000, 1140000, 1150000,
	1160000, 1170000, 1180000, 1190000, 1200000, 1210000, 1220000, 1230000,
	1240000, 1250000, 1260000, 1270000, 1280000, 1290000, 1300000, 1310000,
	1320000, 1330000, 1340000, 1350000, 1360000, 1370000, 1380000, 1390000,
	1400000, 1410000, 1420000, 1430000, 1440000, 1450000, 1460000, 1470000,
	1480000, 1490000, 1500000, 1510000, 1520000, 1530000, 1540000, 1550000,
	1560000, 1570000, 1580000, 1590000, 1600000, 1610000, 1620000, 1630000,
	1640000, 1650000, 1660000, 1670000, 1680000, 1690000, 1700000, 1710000,
	1720000, 1730000, 1740000, 1750000, 1760000, 1770000, 1780000, 1790000,
	1800000, 1810000, 1820000, 1830000, 1840000, 1850000, 1860000, 1870000,
	1880000, 1890000, 1900000, 1910000, 1920000, 1930000, 1940000, 1950000,
};

static const struct tps_info tps65273_regs[] = {
	{
		.name = "VDCDC1",
		.table_len = ARRAY_SIZE(vcore_vsel_table),
		.table = vcore_vsel_table,
	},
	{
		.name = "VDCDC2",
		.table_len = ARRAY_SIZE(vcore_vsel_table),
		.table = vcore_vsel_table,
	},
};

static struct tps_driver_data tps65273_drv_data = {
	.info = tps65273_regs,
};


static const struct i2c_device_id tps65273_ids[] = {
	{.name = "tps65273",
	.driver_data = (unsigned long) &tps65273_drv_data},
	{ },
};

static int tps65273_read_reg(struct tps65273_data *tps65273, u8 reg)
{
	int ret = i2c_smbus_read_byte_data(tps65273->client, reg);
	if (ret > 0)
		ret &= 0xff;

	return ret;
}

static int tps65273_write_reg(struct tps65273_data *tps65273,
		u8 reg, u8 value)
{
	return i2c_smbus_write_byte_data(tps65273->client, reg, value);
}

static int tps65273_reg_is_enabled(struct tps65273_data *tps, int dcdc_id)
{
	int ret_com;

	if (dcdc_id == 0)
		ret_com = tps65273_read_reg(tps, TPS65273_VOUT1_COM);
	else if (dcdc_id == 1)
		ret_com = tps65273_read_reg(tps, TPS65273_VOUT2_COM);
	else
		return -1;

	if (((ret_com & TPS65273_REG_COM_ENABLE) == 0))
		return 1; /* enabled */

	return 0; /* disabled */
}

static int tps65273_reg_enable(struct tps65273_data *tps, int dcdc_id,
				int status)
{
	u8 ret = 0;
	u8 reg = TPS65273_VOUT1_COM;

	if (dcdc_id == 0)
		reg = TPS65273_VOUT1_COM;
	else if (dcdc_id == 1)
		reg = TPS65273_VOUT2_COM;
	else
		return -1;

	if (status == TPS65273_ENABLE) {
		ret = tps65273_read_reg(tps, reg);
		ret &= ~TPS65273_REG_COM_ENABLE;
	} else if (status == TPS65273_DISABLE) {
		ret = tps65273_read_reg(tps, reg);
		ret = ret | TPS65273_REG_COM_ENABLE;
	} else
		return -1;
	/* write to register */
	pr_debug("%s: write %x to reg %d\n", __func__, ret, reg);
	tps65273_write_reg(tps, reg, ret);
	return status;
}

static int tps65273_enable_go(struct regulator_dev *rdev)
{
	u8 value = 0;
	u8 ret = 0;
	u8 reg = TPS65273_VOUT1_SEL;
	struct tps65273_data *tps = rdev_get_drvdata(rdev);
	int dcdc = rdev_get_id(rdev);

	value = 1 << TPS65273_REG_VOUT_GO_OFFSET;
	if (dcdc == 0)
		reg = TPS65273_VOUT1_SEL;
	else if (dcdc == 1)
		reg = TPS65273_VOUT2_SEL;
	else
		return -1;

	ret = tps65273_read_reg(tps, reg);
	ret |= value;

	/* write to register */
	pr_debug("%s: write %x to reg %d\n", __func__, ret, reg);
	tps65273_write_reg(tps, reg, ret);
	return 0;
}

static int tps65273_disable_go(struct regulator_dev *rdev)
{
	u8 value = 0;
	u8 ret = 0;
	u8 reg = TPS65273_VOUT1_SEL;
	struct tps65273_data *tps = rdev_get_drvdata(rdev);
	int dcdc = rdev_get_id(rdev);

	value = 1 << TPS65273_REG_VOUT_GO_OFFSET;
	if (dcdc == 0)
		reg = TPS65273_VOUT1_SEL;
	else if (dcdc == 1)
		reg = TPS65273_VOUT2_SEL;
	else
		return -1;

	ret = tps65273_read_reg(tps, reg);
	ret &= ~value;

	/* write to register */
	pr_debug("%s: write %x to reg %d\n", __func__, ret, reg);
	tps65273_write_reg(tps, reg, ret);
	return 0;
}

static int tps65273_get_go(struct regulator_dev *rdev)
{
	u8 mask = 0;
	u8 ret = 0;
	u8 reg = TPS65273_VOUT1_SEL;
	struct tps65273_data *tps = rdev_get_drvdata(rdev);
	int dcdc = rdev_get_id(rdev);

	mask = 1 << TPS65273_REG_VOUT_GO_OFFSET;

	if (dcdc == 0)
		reg = TPS65273_VOUT1_SEL;
	else if (dcdc == 1)
		reg = TPS65273_VOUT2_SEL;
	else
		return -1;

	ret = tps65273_read_reg(tps, reg);
	if ((ret & mask) > 0)
		return 1;
	else
		return 0;
}

static int tps65273_set_slew(struct tps65273_data *tps, int dcdc_id, int slew)
{
	u8 ret = 0;
	u8 reg = TPS65273_VOUT1_COM;

	if (dcdc_id == 0)
		reg = TPS65273_VOUT1_COM;
	else if (dcdc_id == 1)
		reg = TPS65273_VOUT2_COM;
	else
		return -1;
	ret = tps65273_read_reg(tps, reg);
	ret = ret | 0x70;
	slew = slew & 0x7;
	ret = ret & (slew << 4);
	/* write to register */
	pr_debug("%s: write %x to reg %d\n", __func__, ret, reg);
	tps65273_write_reg(tps, reg, ret);
	return 0;
}

static int tps65273_set_vout(struct tps65273_data *tps,
	int dcdc_id,
	int voltage)
{
	u8 ret = 0;
	u8 reg = TPS65273_VOUT1_SEL;

	voltage += (tps->pdata->irdrop_offset[dcdc_id]) / 10000; /*10mV steps*/

	if (dcdc_id == 0)
		reg = TPS65273_VOUT1_SEL;
	else if (dcdc_id == 1)
		reg = TPS65273_VOUT2_SEL;
	else
		return -1;

	ret = tps65273_read_reg(tps, reg);
	ret = ret & 0x80;
	voltage = voltage & 0x7f;
	ret = ret | voltage;
	/* write to register */
	pr_debug("%s: write %x to reg %d\n", __func__, ret, reg);
	tps65273_write_reg(tps, reg, ret);
	return 0;
}

static int tps65273_get_vout(struct tps65273_data *tps, int dcdc_id)
{
	u8 ret = 0;
	u8 reg = TPS65273_VOUT1_SEL;

	if (dcdc_id == 0)
		reg = TPS65273_VOUT1_SEL;
	else if (dcdc_id == 1)
		reg = TPS65273_VOUT2_SEL;
	else
		return -1;
	ret = tps65273_read_reg(tps, reg);
	ret = ret & 0x7f;
	return ret;
}

static int tps65273_set_psm(struct tps65273_data *tps, int dcdc_id, int psm)
{
	u8 ret = 0;
	u8 reg = TPS65273_VOUT1_COM;

	if (dcdc_id == 0)
		reg = TPS65273_VOUT1_COM;
	else if (dcdc_id == 1)
		reg = TPS65273_VOUT2_COM;
	else
		return -1;
	ret = tps65273_read_reg(tps, reg);
	ret = ret | 0x06;
	psm = psm & 0x3;
	ret = ret & (psm << 1);
	/* write to register */
	pr_debug("%s: write %x to reg %d\n", __func__, ret, reg);
	tps65273_write_reg(tps, reg, ret);
	return 0;
}

static int tps65273_get_voltage_sel(struct regulator_dev *rdev)
{
	struct tps65273_data *tps65273 = rdev_get_drvdata(rdev);
	int dcdc = rdev_get_id(rdev);
	int selector;

	pr_debug("%s dcdc[%d]\n", __func__, dcdc);
	selector = tps65273_get_vout(tps65273, dcdc);

	return selector;
}

static int tps65273_set_voltage_sel(struct regulator_dev *rdev,
				   unsigned selector)
{
	struct tps65273_data *tps65273 = rdev_get_drvdata(rdev);
	int dcdc = rdev_get_id(rdev);

	pr_debug("%s dcdc[%d] %d\n", __func__, dcdc, selector);

	tps65273_set_vout(tps65273, dcdc, selector);
	/* send notify */
	regulator_notifier_call_chain(rdev,
		REGULATOR_EVENT_VOLTAGE_CHANGE,
		tps65273);

	return selector;
}

static int tps65273_enable(struct regulator_dev *rdev)
{
	int ret = 0;
	struct tps65273_data *tps = rdev_get_drvdata(rdev);
	int dcdc = rdev_get_id(rdev);

	pr_debug("%s dcdc[%d]\n", __func__, dcdc);
	ret = tps65273_reg_enable(tps, dcdc, TPS65273_ENABLE);
	return ret;
}

static int tps65273_disable(struct regulator_dev *rdev)
{
	int ret = 0;
	struct tps65273_data *tps = rdev_get_drvdata(rdev);
	int dcdc = rdev_get_id(rdev);

	pr_debug("%s dcdc[%d]\n", __func__, dcdc);
	tps65273_reg_enable(tps, dcdc, TPS65273_DISABLE);
	return ret;
}

static int tps65273_is_enabled(struct regulator_dev *rdev)
{
	struct tps65273_data *tps = rdev_get_drvdata(rdev);
	int dcdc = rdev_get_id(rdev);

	pr_debug("%s dcdc id[%d]\n", __func__, dcdc);
	if (!tps) {
		pr_err("%s: Can not get user resource\n", __func__);
		return -1;
	}
	return tps65273_reg_is_enabled(tps, dcdc);
}

static int tps65273_status(struct regulator_dev *rdev)
{
	int ret = 0;
	struct tps65273_data *tps = rdev_get_drvdata(rdev);
	int dcdc = rdev_get_id(rdev);

	pr_debug("%s dcdc[%d]\n", __func__, dcdc);
	ret = tps65273_read_reg(tps, TPS65273_SYS_STATUS);
	if (dcdc == 0)
		ret = ret & TPS65273_REG_STATUS_PGOOD1;
	else if (dcdc == 1)
		ret = ret & TPS65273_REG_STATUS_PGOOD2;
	else
		ret = -1;

	if (ret >= 0) {
		switch (ret) {
		case 0:
			ret = REGULATOR_STATUS_OFF;
			break;
		case 1:
			ret = REGULATOR_STATUS_ON;
			break;
		default:
			ret = REGULATOR_STATUS_UNDEFINED;
			break;
		}
	}
	return ret;
}


#ifdef CONFIG_OF
static struct of_regulator_match tps65273_matches[] = {
	{ .name = "dcdc1", },
	{ .name = "dcdc2", },
};

static struct of_device_id tps65273_dt_match[] = {
	{ .compatible	 = "ti,tps65273" },
	{},
};
MODULE_DEVICE_TABLE(of, tps65273_dt_match);

static struct tps65273_platform_data *tps65273_parse_dt(struct device *dev)
{
	struct tps65273_platform_data *pd;
	struct device_node *np = dev->of_node;
	int i, ret;
	struct device_node *regulators;

	pd = devm_kzalloc(dev, sizeof(*pd), GFP_KERNEL);
	if (!pd) {
		dev_err(dev, "Failed to allocate platform data\n");
		return NULL;
	}

	regulators = of_find_node_by_name(np, "regulators");
	if (!regulators) {
		dev_err(dev, "regulator node not found\n");
		return ERR_PTR(-ENODEV);
	}

	ret = of_regulator_match(dev, regulators, tps65273_matches,
			ARRAY_SIZE(tps65273_matches));
	if (ret < 0) {
		dev_err(dev, "Error parsing regulator init data: %d\n", ret);
		return ERR_PTR(ret);
	}
	for (i = 0; i < ARRAY_SIZE(tps65273_matches); i++) {
		struct regulator_init_data *reg_data;

		reg_data = tps65273_matches[i].init_data;
		if (!reg_data || !tps65273_matches[i].of_node)
			continue;

		pd->reg_data[i] = reg_data;
		if (of_property_read_u32(tps65273_matches[i].of_node,
						"vout-slew-rate",
						&pd->slew_rate[i])) {
			dev_warn(dev, "Slew rate not specified\n");
			pd->slew_rate[i] = 0;
		}

		if (of_property_read_u32(tps65273_matches[i].of_node,
						"vout-psm-mode",
						&pd->psm_mode[i])) {
			dev_warn(dev, "PSM mode not specified\n");
			pd->psm_mode[i] = 0;
		}

		if (of_property_read_u32(tps65273_matches[i].of_node,
						"vout-irdrop-offset",
						&pd->irdrop_offset[i])) {
			dev_warn(dev, "IR-Drop offset not specified. Def=0\n");
			pd->irdrop_offset[i] = 0;
		}

		if (of_property_read_u32(tps65273_matches[i].of_node,
					"regulator-min-microvolt",
					&reg_data->constraints.min_uV)) {
			dev_warn(dev, "min-microvolt not specified.\n");
			reg_data->constraints.min_uV = TPS65273_MIN_VOLTAGE;
		}

		if (of_property_read_u32(tps65273_matches[i].of_node,
					"regulator-max-microvolt",
					&reg_data->constraints.max_uV)) {
			dev_warn(dev, "max-microvolt not specified.\n");
			reg_data->constraints.max_uV = TPS65273_MAX_VOLTAGE;
		}

		reg_data->constraints.valid_modes_mask = REGULATOR_MODE_NORMAL
						| REGULATOR_MODE_STANDBY;
		reg_data->constraints.valid_ops_mask = REGULATOR_CHANGE_VOLTAGE
						| REGULATOR_CHANGE_STATUS;
	}
	return pd;
}
#else
static struct tps65273_platform_data *tps65273_parse_dt(struct device *dev)
{
	return NULL;
}
#endif

static int tps_65273_i2c_probe(struct i2c_client *client,
		const struct i2c_device_id *i2c_id)
{
	struct i2c_adapter *adapter = to_i2c_adapter(client->dev.parent);
	struct tps65273_platform_data *pdata = client->dev.platform_data;
	const struct tps_driver_data *drv_data = (void *)i2c_id->driver_data;
	const struct tps_info *info = drv_data->info;
	struct regulator_config config = { };
	struct tps65273_data *tps65273;
	int i;
	int ret = 0;

	if (client->dev.of_node)
		pdata = tps65273_parse_dt(&client->dev);

	if (!pdata) {
		dev_err(&client->dev, "Require the platform data\n");
		return -EINVAL;
	}

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE))
		return -EIO;

	tps65273 = devm_kzalloc(&client->dev, sizeof(struct tps65273_data),
			       GFP_KERNEL);
	if (!tps65273)
		return -ENOMEM;

	/* save general information */
	tps65273->client = client;
	tps65273->dev = &client->dev;
	tps65273->pdata = pdata;

	/* register regulator to system */
	for (i = 0; i < TPS65273_NUM_REGULATOR; i++, info++) {
		if (!tps65273->pdata->reg_data[i])
			continue;

		tps65273->desc[i].name =
			tps65273->pdata->reg_data[i]->constraints.name;
		tps65273->desc[i].id		= i;
		tps65273->desc[i].n_voltages	= info->table_len;
		tps65273->desc[i].min_uV	=
			tps65273->pdata->reg_data[i]->constraints.min_uV;
		tps65273->desc[i].uV_step	= TPS65273_STEP_VOLTAGE;
		tps65273->desc[i].volt_table	= info->table;
		tps65273->desc[i].ops		= &tps65273_ops;
		tps65273->desc[i].type		= REGULATOR_VOLTAGE;
		tps65273->desc[i].owner		= THIS_MODULE;
		if (i == 0)
			tps65273->desc[i].apply_reg = TPS65273_VOUT1_SEL;
		else if (i == 1)
			tps65273->desc[i].apply_reg = TPS65273_VOUT2_SEL;

		tps65273->desc[i].apply_bit = TPS65273_REG_VOUT_GO;

		config.dev = tps65273->dev;
		config.init_data = pdata->reg_data[i];
		config.driver_data = tps65273;
		config.of_node = client->dev.of_node;
		tps65273->rdev[i] =
			regulator_register(&tps65273->desc[i], &config);
		if (IS_ERR(tps65273->rdev[i])) {
			ret = PTR_ERR(tps65273->rdev[i]);
			dev_err(tps65273->dev,
				"regulator init failed (%d)\n", ret);
			return -EINVAL;
		}
		/* other settings from device tree */
		tps65273_set_slew(tps65273, i, pdata->slew_rate[i]);
		tps65273_set_psm(tps65273, i, pdata->psm_mode[i]);
	}

	i2c_set_clientdata(client, tps65273);
	return 0;
}

static int tps_65273_i2c_remove(struct i2c_client *client)
{
	struct tps65273_data *tps65273 = i2c_get_clientdata(client);
	int i;

	for (i = 0; i < TPS65273_NUM_REGULATOR; i++) {
		struct regulator_dev *rdev = tps65273->rdev[i];
		regulator_unregister(rdev);
	}
	return 0;
}

MODULE_DEVICE_TABLE(i2c, tps65273_ids);

static struct i2c_driver tps_65273_i2c_driver = {
	.probe		= tps_65273_i2c_probe,
	.remove		= tps_65273_i2c_remove,
	.driver		= {
		.name		= "tps65273",
#ifdef CONFIG_OF
		.of_match_table	= of_match_ptr(tps65273_dt_match),
#endif
	},
	.id_table	= tps65273_ids,
};

static int __init tps_65273_init(void)
{
	pr_debug(DRV_NAME ", Version " DRV_VERSION
		" (c) Copyright 2015, Lantiq Deutschland GmbH\n");
	return i2c_add_driver(&tps_65273_i2c_driver);
}

static void __exit tps_65273_exit(void)
{
	pr_debug("Module " DRV_NAME "is removed\n");
	i2c_del_driver(&tps_65273_i2c_driver);
}

subsys_initcall(tps_65273_init);
module_exit(tps_65273_exit);

MODULE_DESCRIPTION("TPS65273 voltage regulator driver");
MODULE_AUTHOR("Ho Nghia Duc <Ho Nghia Duc at lantiq.com>");
MODULE_LICENSE("GPL v2");
