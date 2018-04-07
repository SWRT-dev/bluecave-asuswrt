/******************************************************************************
**
** FILE NAME    : stat_helper.c
** AUTHOR       : 
** DESCRIPTION  : Counter and Statistics Helper module
** COPYRIGHT    :      Copyright (c) 2014
**              Lantiq Deutschland GmbH 
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date         $Author                $Comment
**                                   
*******************************************************************************/

/******************************************************************************
 *				Includes				      *
 ******************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#ifdef CONFIG_PROC_FS
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

/* Root directory node for module proc entries */
static struct proc_dir_entry *proc_root = NULL;
#endif /* CONFIG_PROC_FS */

#include <linux/netdevice.h>
#include <linux/rcupdate.h>

/* Kernel helper include */
#include <linux/string.h>

#include <net/ppa_api.h>

#include "stat_helper.h"
#include "platform/stat_helper_platform.h"

#define STAT_HELPER_DEBUG 0

/******************************************************************************
 *				Local Functions				      *
 ******************************************************************************/
static struct rtnl_link_stats64*
stat_helper_dev_get_extended_stats64(struct net_device *dev,
									 struct rtnl_link_stats64 *storage)
{
#if (STAT_HELPER_DEBUG == 0)
	stat_helper_platform_dev_get_extended_stats64(dev, storage);
#endif

	return storage;
}

#if STAT_HELPER_DEBUG
static struct rtnl_link_stats64 *stat_helper_dev_get_stats(struct net_device *dev,
                    struct rtnl_link_stats64 *storage)
{
    const struct net_device_ops *ops = dev->netdev_ops;

    if (ops->ndo_get_stats64) {
        memset(storage, 0, sizeof(*storage));
        ops->ndo_get_stats64(dev, storage);
    } else if (ops->ndo_get_stats) {
        netdev_stats_to_stats64(storage, ops->ndo_get_stats(dev));
    } else {
        netdev_stats_to_stats64(storage, &dev->stats);
    }
	stat_helper_platform_dev_get_extended_stats64(dev, storage);
    storage->rx_dropped += atomic_long_read(&dev->rx_dropped);
    return storage;
}
#endif

#ifdef CONFIG_PROC_FS
static status stat_helper_get_ndo_stats(struct seq_file *seq)
{
	struct net *net = &init_net;
	struct net_device *dev;
	struct rtnl_link_stats64 temp;
	const struct rtnl_link_stats64 *stats = NULL;
	PPA_NETIF_ACCEL_STATS ppa_stats;

	rcu_read_lock();
	for_each_netdev_rcu(net, dev) {
		//printk("Device name : %s  - Device type : %u, Device priv_flags : 0x%04X\n", dev->name, dev->type, dev->priv_flags);
		memset(&ppa_stats, 0, sizeof(PPA_NETIF_ACCEL_STATS));

#if STAT_HELPER_DEBUG
		stats = stat_helper_dev_get_stats(dev, &temp);
#else
		stats = dev_get_stats(dev, &temp);
#endif

#if (defined(CONFIG_LTQ_PPA_API) || defined(CONFIG_LTQ_PPA_API_MODULE))
		/* Get PPA HW and SW accelerated session stats */
		if (ppa_hook_get_netif_accel_stats_fn) {
			ppa_hook_get_netif_accel_stats_fn(dev->name, &ppa_stats, (PPA_F_NETIF_HW_ACCEL | PPA_F_NETIF_SW_ACCEL));
		}
#endif

		seq_printf(seq, "%6s: %7llu %7llu %4llu %4llu %7llu %7llu  "
			   "%8llu %7llu %4llu %4llu %7llu %7llu\n",
			   dev->name, stats->rx_bytes, stats->rx_packets,
			   stats->rx_errors, stats->rx_dropped + stats->rx_missed_errors,
			   ppa_stats.hw_accel_stats.rx_bytes, ppa_stats.sw_accel_stats.rx_bytes,
			   stats->tx_bytes, stats->tx_packets,
			   stats->tx_errors, stats->tx_dropped,
			   ppa_stats.hw_accel_stats.tx_bytes, ppa_stats.sw_accel_stats.tx_bytes);
	}
	rcu_read_unlock();

	return STAT_HELPER_SUCCESS;
}

static int stat_helper_proc_read(struct seq_file *seq, void *v) 
{
	int ret = STAT_HELPER_SUCCESS;
	int len = 0;

	if (!MODULE_GET)
		return ret;

	len = seq->size - seq->count;

	seq_puts(seq, "Inter-|   Receive                                 "
		      " |  Transmit\n"
		      " face |bytes    packets errs drop hw-accl sw-accl "
		      " |bytes    packets errs drop hw-accl sw-accl\n");

	/* Get NDO status */
	stat_helper_get_ndo_stats(seq);

	ret = 0;

	MODULE_PUT;
	return ret;
}

static int stat_helper_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, stat_helper_proc_read, NULL);
}

static const struct file_operations stat_helper_proc_fops = {
	.owner = THIS_MODULE,
	.open = stat_helper_proc_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int _stat_helper_proc_init(void)
{
#if STAT_HELPER_DEBUG
	printk(KERN_INFO "STAT_HELPER: creating proc interfaces\n");
#endif
	proc_root = proc_mkdir("stat_helper", init_net.proc_net);
	if (!proc_create("dev", 0, proc_root, &stat_helper_proc_fops)) {
		printk(KERN_ERR "STAT_HELPER: failed to create proc entry 'dev'\n");
		return STAT_HELPER_FAILURE;
	}

	return STAT_HELPER_SUCCESS;
}

static int _stat_helper_proc_deinit(void)
{
#if STAT_HELPER_DEBUG
	printk(KERN_INFO "STAT_HELPER: removing proc interfaces\n");
#endif
	remove_proc_entry("dev", proc_root);
	proc_remove(proc_root);
	proc_root = NULL;
	return STAT_HELPER_SUCCESS;
}
#endif /* CONFIG_PROC_FS */

/******************************************************************************
 *				Global Functions			      *
 ******************************************************************************/
static int __init stat_helper_init_module(void)
{
	rcu_assign_pointer(dev_get_extended_stats64_fn, stat_helper_dev_get_extended_stats64);
#ifdef CONFIG_PROC_FS
	_stat_helper_proc_init();
#endif
#if STAT_HELPER_DEBUG
	printk("Successfully loaded stat helper module.\n");
#endif
	return 0;
}

static void __exit stat_helper_exit_module(void)
{
#ifdef CONFIG_PROC_FS
	_stat_helper_proc_deinit();
#endif
	rcu_assign_pointer(dev_get_extended_stats64_fn, NULL);
#if STAT_HELPER_DEBUG
	printk("Successfully unloaded stat helper module.\n");
#endif
}

module_init(stat_helper_init_module);
module_exit(stat_helper_exit_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Copyright © 2015 Lantiq");
MODULE_DESCRIPTION("Lantiq Stat Helper Module");
