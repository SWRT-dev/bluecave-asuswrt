/******************************************************************************

			Copyright (c) 2015
			Lantiq Deutschland GmbH

	For licensing information, see the file 'LICENSE' in the root folder
	of this software module.

******************************************************************************/
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/module.h>

#if defined(CONFIG_DEBUG_FS)
#include <linux/debugfs.h>
#include <linux/seq_file.h>
#include <linux/interrupt.h>
#include <linux/regulator/consumer.h>
#include <cpufreq/ltq_cpufreq.h>

#define BUF_SIZE 24

static struct dentry		*debugfs_root;

static int rc_ena_seq_write(struct file *file, const char __user *buf,
					size_t count, loff_t *dat)
{
	struct reg_cpufreq *data = file->f_inode->i_private;
	char str[BUF_SIZE];
	int err;

	pr_debug("%s is called\n", __func__);
	memset(str, 0, sizeof(str));
	if ((count <= 1) || (count > BUF_SIZE))
		return count;

	copy_from_user(str, buf, count - 1);

	if (sysfs_streq(str, "disable") || sysfs_streq(str, "0")) {
		err = regulator_disable(data->regulator);
		if (err < 0) {
			pr_err("regulator %d disable failed\n",
							data->reg_idx + 1);
			return count;
		}
		return count;
	} else if (sysfs_streq(str, "enable") || sysfs_streq(str, "1")) {
		err = regulator_enable(data->regulator);
		if (err < 0) {
			pr_err("regulator %d enable failed\n",
							data->reg_idx + 1);
			return count;
		}
		return count;
	}
	pr_err("invalid value! Please use: 0 | disable | 1 | enable\n");
	return count;
}

static int rc_rd_ena(struct seq_file *seq, void *v)
{
	struct reg_cpufreq *data = seq->private;
	int ret;

	pr_debug("%s is called\n", __func__);
	ret = regulator_is_enabled(data->regulator);
	if (ret < 0)
		pr_err("regulator %d read enable failed\n", data->reg_idx + 1);
	else
		seq_printf(seq, "regulator %d output: %s\n", data->reg_idx + 1,
					(ret == 1) ? "enabled" : "disabled");
	return 0;
}

static int rc_ena_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, rc_rd_ena, inode->i_private);
}

static const struct file_operations rc_ena_fops = {
	.owner		= THIS_MODULE,
	.open		= rc_ena_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= rc_ena_seq_write,
	.release	= single_release
};

static int rc_i2c_ctrl_seq_write(struct file *file, const char __user *buf,
					size_t count, loff_t *dat)
{
	struct reg_cpufreq *data = file->f_inode->i_private;
	char str[BUF_SIZE];
	int err;

	pr_debug("%s is called\n", __func__);
	memset(str, 0, sizeof(str));
	if ((count <= 1) || (count > BUF_SIZE))
		return count;

	copy_from_user(str, buf, count - 1);
	if (sysfs_streq(str, "off") || sysfs_streq(str, "0")) {
		err = regulator_disable_ext_ctrl(data->regulator);
		if (err < 0)
			pr_err("regulator %d set i2c mode failed\n",
							data->reg_idx + 1);
		return count;
	} else if (sysfs_streq(str, "on") || sysfs_streq(str, "1")) {
		err = regulator_enable_ext_ctrl(data->regulator);
		if (err < 0)
			pr_err("regulator %d set i2c mode failed\n",
							data->reg_idx + 1);
		return count;
	}
	pr_err("invalid value! Please use: 0 | off | 1 | on\n");
	return count;
}

static int rc_rd_i2c_ctrl(struct seq_file *seq, void *v)
{
	struct reg_cpufreq *data = seq->private;
	int ret;

	pr_debug("%s is called\n", __func__);
	ret = regulator_get_ext_ctrl(data->regulator);
	seq_printf(seq, "i2c ctrl %d: %s\n", data->reg_idx + 1,
					(ret == 1) ? "enabled" : "disabled");
	return 0;
}

static int rc_i2c_ctrl_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, rc_rd_i2c_ctrl, inode->i_private);
}

static const struct file_operations rc_i2c_ctrl_fops = {
	.owner		= THIS_MODULE,
	.open		= rc_i2c_ctrl_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= rc_i2c_ctrl_seq_write,
	.release	= single_release
};

static int rc_vol_seq_write(struct file *file, const char __user *buf,
					size_t count, loff_t *dat)
{
	struct reg_cpufreq *data = file->f_inode->i_private;
	char str[BUF_SIZE];
	signed long value;

	pr_debug("%s is called\n", __func__);
	memset(str, 0, sizeof(str));
	if ((count <= 1) || (count > BUF_SIZE))
		return count;

	copy_from_user(str, buf, count - 1);
	if (kstrtol(str, 10, &value) == 0)
		regulator_set_voltage(data->regulator, value, value);
	return count;
}

static int rc_rd_vol(struct seq_file *seq, void *v)
{
	struct reg_cpufreq *data = seq->private;

	pr_debug("%s is called\n", __func__);
	seq_printf(seq, "Current voltage %d: %d\n", data->reg_idx + 1,
	regulator_get_voltage(data->regulator));
	return 0;
}

static int rc_vol_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, rc_rd_vol, inode->i_private);
}

static const struct file_operations rc_vol_fops = {
	.owner		= THIS_MODULE,
	.open		= rc_vol_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write		= rc_vol_seq_write,
	.release	= single_release
};

void rcf_debugfs_init(struct reg_cpufreq *data)
{
	int i;
	char str[12];

//	pr_debug("%s is called\n", __func__);
	pr_err("%s is called\n", __func__);
	debugfs_root = debugfs_create_dir("ltq_regulator_cpufreq", NULL);
	if (IS_ERR(debugfs_root)) {
		debugfs_root = NULL;
		pr_err("ERROR: no debugfs_root\n");
		return;
	}

	for (i = 0; i < NUM_SUPPLY; i++) {
		if (IS_ERR(data->regulator)) {
			pr_debug("regulator %d not defined\n", i + 1);
			data++;
			continue;
		}
		sprintf(str, "vol%d", i+1);
		(void)debugfs_create_file(str, S_IFREG | S_IRUGO, debugfs_root,
					data, &rc_vol_fops);
		sprintf(str, "enable%d", i+1);
		(void)debugfs_create_file(str, S_IFREG | S_IRUGO, debugfs_root,
					data, &rc_ena_fops);
		sprintf(str, "i2c_ctrl%d", i+1);
		(void)debugfs_create_file(str, S_IFREG | S_IRUGO, debugfs_root,
					data, &rc_i2c_ctrl_fops);
		data++;
	}
}

void rcf_debugfs_cleanup(struct reg_cpufreq *data)
{
	pr_debug("%s is called\n", __func__);
	if (!debugfs_root)
		return;

	debugfs_remove_recursive(debugfs_root);
	debugfs_root = NULL;
}
#endif /* CONFIG_DEBUG_FS */

