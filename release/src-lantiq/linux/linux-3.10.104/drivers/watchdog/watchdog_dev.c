/*
 *	watchdog_dev.c
 *
 *	(c) Copyright 2008-2011 Alan Cox <alan@lxorguk.ukuu.org.uk>,
 *						All Rights Reserved.
 *
 *	(c) Copyright 2008-2011 Wim Van Sebroeck <wim@iguana.be>.
 *
 *
 *	This source code is part of the generic code that can be used
 *	by all the watchdog timer drivers.
 *
 *	This part of the generic code takes care of the following
 *	misc device: /dev/watchdog.
 *
 *	Based on source code of the following authors:
 *	  Matt Domsch <Matt_Domsch@dell.com>,
 *	  Rob Radez <rob@osinvestor.com>,
 *	  Rusty Lynch <rusty@linux.co.intel.com>
 *	  Satyam Sharma <satyam@infradead.org>
 *	  Randy Dunlap <randy.dunlap@oracle.com>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
 *	Neither Alan Cox, CymruNet Ltd., Wim Van Sebroeck nor Iguana vzw.
 *	admit liability nor provide warranty for any of this software.
 *	This material is provided "AS-IS" and at no charge.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>	/* For module stuff/... */
#include <linux/types.h>	/* For standard types (like size_t) */
#include <linux/errno.h>	/* For the -ENODEV/... values */
#include <linux/kernel.h>	/* For printk/panic/... */
#include <linux/fs.h>		/* For file operations */
#include <linux/watchdog.h>	/* For watchdog specific items */
#include <linux/miscdevice.h>	/* For handling misc devices */
#include <linux/init.h>		/* For __init/__exit/... */
#include <linux/uaccess.h>	/* For copy_to_user/put_user/... */

#include "watchdog_core.h"

#ifdef CONFIG_WDT_SUPPORT_WORKQUEUE
#include <linux/workqueue.h>	/* For workqueue */
#include <linux/jiffies.h>	/* For timeout functions */
#include <linux/slab.h>

#endif

/* the dev_t structure to store the dynamically allocated watchdog devices */
static dev_t watchdog_devt;
/* the watchdog device behind /dev/watchdog */
static struct watchdog_device *old_wdd;

#ifdef CONFIG_WDT_SUPPORT_WORKQUEUE
static struct watchdog_core_data *old_wd_data;
static struct workqueue_struct *watchdog_wq;


static inline bool watchdog_need_worker(struct watchdog_device *wdd)
{
	/* All variables in milli-seconds */
	unsigned int hm = wdd->max_hw_heartbeat_ms;
	unsigned int t = wdd->timeout * 1000;

	/*
	 * A worker to generate heartbeat requests is needed if all of the
	 * following conditions are true.
	 * - Userspace activated the watchdog.
	 * - The driver provided a value for the maximum hardware timeout, and
	 *   thus is aware that the framework supports generating heartbeat
	 *   requests.
	 * - Userspace requests a longer timeout than the hardware can handle.
	 *
	 * Alternatively, if userspace has not opened the watchdog
	 * device, we take care of feeding the watchdog if it is
	 * running.
	 */
	return (hm && watchdog_active(wdd) && t > hm) ||
		(t && !watchdog_active(wdd) && watchdog_hw_running(wdd));
}

static long watchdog_next_keepalive(struct watchdog_device *wdd)
{
	struct watchdog_core_data *wd_data = wdd->wd_data;
	unsigned int timeout_ms = wdd->timeout * 1000;
	unsigned long keepalive_interval;
	unsigned long last_heartbeat;
	unsigned long virt_timeout;
	unsigned int hw_heartbeat_ms;

	virt_timeout = wd_data->last_keepalive + msecs_to_jiffies(timeout_ms);
	hw_heartbeat_ms = min_not_zero(timeout_ms, wdd->max_hw_heartbeat_ms);
	keepalive_interval = msecs_to_jiffies(hw_heartbeat_ms / 2);

	if (!watchdog_active(wdd))
		return keepalive_interval;

	/*
	 * To ensure that the watchdog times out wdd->timeout seconds
	 * after the most recent ping from userspace, the last
	 * worker ping has to come in hw_heartbeat_ms before this timeout.
	 */
	last_heartbeat = virt_timeout - msecs_to_jiffies(hw_heartbeat_ms);
	return min_t(long, last_heartbeat - jiffies, keepalive_interval);
}

static inline void watchdog_update_worker(struct watchdog_device *wdd)
{
	struct watchdog_core_data *wd_data = wdd->wd_data;

	if (watchdog_need_worker(wdd)) {
		long t = watchdog_next_keepalive(wdd);

		if (t > 0)
			mod_delayed_work(watchdog_wq, &wd_data->work, t);
	} else {
		cancel_delayed_work(&wd_data->work);
	}
}

static int __watchdog_ping(struct watchdog_device *wdd)
{
	struct watchdog_core_data *wd_data = wdd->wd_data;
	unsigned long earliest_keepalive = wd_data->last_hw_keepalive +
				msecs_to_jiffies(wdd->min_hw_heartbeat_ms);
	int err;

	if (time_is_after_jiffies(earliest_keepalive)) {
		mod_delayed_work(watchdog_wq, &wd_data->work,
				 earliest_keepalive - jiffies);
		return 0;
	}

	wd_data->last_hw_keepalive = jiffies;

	if (wdd->ops->ping)
		err = wdd->ops->ping(wdd);  /* ping the watchdog */
	else
		err = wdd->ops->start(wdd); /* restart watchdog */

	watchdog_update_worker(wdd);

	return err;
}
/*
 *	watchdog_ping: ping the watchdog.
 *	@wddev: the watchdog device to ping
 *
 *	If the watchdog has no own ping operation then it needs to be
 *	restarted via the start operation. This wrapper function does
 *	exactly that.
 *	We only ping when the watchdog device is running.
 */

 
static int watchdog_ping(struct watchdog_device *wdd)
{
	struct watchdog_core_data *wd_data = wdd->wd_data;

	if (!watchdog_active(wdd) && !watchdog_hw_running(wdd))
		return 0;

	set_bit(_WDOG_KEEPALIVE, &wd_data->status);

	wd_data->last_keepalive = jiffies;
	return __watchdog_ping(wdd);

}

static void watchdog_ping_work(struct work_struct *work)
{
	struct watchdog_core_data *wd_data;
	struct watchdog_device *wdd;

	wd_data = container_of(to_delayed_work(work), struct watchdog_core_data,
			       work);

	mutex_lock(&wd_data->lock);
	wdd = wd_data->wdd;
	if (wdd && (watchdog_active(wdd) || watchdog_hw_running(wdd)))
		__watchdog_ping(wdd);
	mutex_unlock(&wd_data->lock);
}

static int watchdog_start(struct watchdog_device *wdd)
{
	struct watchdog_core_data *wd_data = wdd->wd_data;
	unsigned long started_at;
	int err;

	if (watchdog_active(wdd))
		return 0;

	set_bit(_WDOG_KEEPALIVE, &wd_data->status);

	started_at = jiffies;
	if (watchdog_hw_running(wdd) && wdd->ops->ping)
		err = wdd->ops->ping(wdd);
	else
		err = wdd->ops->start(wdd);
	if (err == 0) {
		set_bit(WDOG_ACTIVE, &wdd->status);
		wd_data->last_keepalive = started_at;
		watchdog_update_worker(wdd);
	}

	return err;
}

static int watchdog_stop(struct watchdog_device *wdd)
{
	int err = 0;

	if (!watchdog_active(wdd))
		return 0;

	if (test_bit(WDOG_NO_WAY_OUT, &wdd->status)) {
		pr_info("watchdog%d: nowayout prevents watchdog being stopped!\n",
			wdd->id);
		return -EBUSY;
	}

	if (wdd->ops->stop) {
		clear_bit(WDOG_HW_RUNNING, &wdd->status);
		err = wdd->ops->stop(wdd);
	} else {
		set_bit(WDOG_HW_RUNNING, &wdd->status);
	}

	if (err == 0) {
		clear_bit(WDOG_ACTIVE, &wdd->status);
		watchdog_update_worker(wdd);
	}

	return err;
}

static unsigned int watchdog_get_status(struct watchdog_device *wdd)
{
	struct watchdog_core_data *wd_data = wdd->wd_data;
	unsigned int status;

	if (wdd->ops->status)
		status = wdd->ops->status(wdd);
	else
		status = wdd->bootstatus & (WDIOF_CARDRESET |
					    WDIOF_OVERHEAT |
					    WDIOF_FANFAULT |
					    WDIOF_EXTERN1 |
					    WDIOF_EXTERN2 |
					    WDIOF_POWERUNDER |
					    WDIOF_POWEROVER);

	if (test_bit(_WDOG_ALLOW_RELEASE, &wd_data->status))
		status |= WDIOF_MAGICCLOSE;

	if (test_and_clear_bit(_WDOG_KEEPALIVE, &wd_data->status))
		status |= WDIOF_KEEPALIVEPING;

	return status;
}

static int watchdog_set_timeout(struct watchdog_device *wdd,
							unsigned int timeout)
{
	int err = 0;

	if (!(wdd->info->options & WDIOF_SETTIMEOUT))
		return -EOPNOTSUPP;

	if (watchdog_timeout_invalid(wdd, timeout))
		return -EINVAL;
	if (wdd->ops->set_timeout) {
		err = wdd->ops->set_timeout(wdd, timeout);
	} else {
		wdd->timeout = timeout;
	}

	watchdog_update_worker(wdd);

	return err;
}

static int watchdog_get_timeleft(struct watchdog_device *wdd,
							unsigned int *timeleft)
{
	*timeleft = 0;

	if (!wdd->ops->get_timeleft)
		return -EOPNOTSUPP;

	*timeleft = wdd->ops->get_timeleft(wdd);

	return 0;
}

static int watchdog_ioctl_op(struct watchdog_device *wdd, unsigned int cmd,
							unsigned long arg)
{
	if (!wdd->ops->ioctl)
		return -ENOIOCTLCMD;

	return wdd->ops->ioctl(wdd, cmd, arg);
}

static ssize_t watchdog_write(struct file *file, const char __user *data,
						size_t len, loff_t *ppos)
{
	struct watchdog_core_data *wd_data = file->private_data;
	struct watchdog_device *wdd;
	int err;
	size_t i;
	char c;

	if (len == 0)
		return 0;

	/*
	 * Note: just in case someone wrote the magic character
	 * five months ago...
	 */
	clear_bit(_WDOG_ALLOW_RELEASE, &wd_data->status);

	/* scan to see whether or not we got the magic character */
	for (i = 0; i != len; i++) {
		if (get_user(c, data + i))
			return -EFAULT;
		if (c == 'V')
			set_bit(_WDOG_ALLOW_RELEASE, &wd_data->status);
	}

	/* someone wrote to us, so we send the watchdog a keepalive ping */

	err = -ENODEV;
	mutex_lock(&wd_data->lock);
	wd_data->user_call = 1;
	wdd = wd_data->wdd;
	if (wdd)
		err = watchdog_ping(wdd);
	mutex_unlock(&wd_data->lock);

	if (err < 0)
		return err;

	return len;
}

static long watchdog_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	struct watchdog_core_data *wd_data = file->private_data;
	void __user *argp = (void __user *)arg;
	struct watchdog_device *wdd;
	int __user *p = argp;
	unsigned int val;
	int err;

	mutex_lock(&wd_data->lock);

	wdd = wd_data->wdd;
	if (!wdd) {
		err = -ENODEV;
		goto out_ioctl;
	}

	err = watchdog_ioctl_op(wdd, cmd, arg);
	if (err != -ENOIOCTLCMD)
		goto out_ioctl;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		err = copy_to_user(argp, wdd->info,
			sizeof(struct watchdog_info)) ? -EFAULT : 0;
		break;
	case WDIOC_GETSTATUS:
		val = watchdog_get_status(wdd);
		err = put_user(val, p);
		break;
	case WDIOC_GETBOOTSTATUS:
		err = put_user(wdd->bootstatus, p);
		break;
	case WDIOC_SETOPTIONS:
		if (get_user(val, p)) {
			err = -EFAULT;
			break;
		}
		if (val & WDIOS_DISABLECARD) {
			err = watchdog_stop(wdd);
			if (err < 0)
				break;
		}
		if (val & WDIOS_ENABLECARD)
			err = watchdog_start(wdd);
		break;
	case WDIOC_KEEPALIVE:
		if (!(wdd->info->options & WDIOF_KEEPALIVEPING)) {
			err = -EOPNOTSUPP;
			break;
		}
		wdd->wd_data->user_call = 1;
		err = watchdog_ping(wdd);
		break;
	case WDIOC_SETTIMEOUT:
		if (get_user(val, p)) {
			err = -EFAULT;
			break;
		}
		err = watchdog_set_timeout(wdd, val);
		if (err < 0)
			break;
		/* If the watchdog is active then we send a keepalive ping
		 * to make sure that the watchdog keep's running (and if
		 * possible that it takes the new timeout) */
		err = watchdog_ping(wdd);
		if (err < 0)
			break;
		/* Fall */
	case WDIOC_GETTIMEOUT:
		/* timeout == 0 means that we don't know the timeout */
		if (wdd->timeout == 0) {
			err = -EOPNOTSUPP;
			break;
		}
		err = put_user(wdd->timeout, p);
		break;
	case WDIOC_GETTIMELEFT:
		err = watchdog_get_timeleft(wdd, &val);
		if (err < 0)
			break;
		err = put_user(val, p);
		break;
	default:
		err = -ENOTTY;
		break;
	}

out_ioctl:
	mutex_unlock(&wd_data->lock);
	return err;
}

static int watchdog_open(struct inode *inode, struct file *file)
{
	struct watchdog_core_data *wd_data;
	struct watchdog_device *wdd;
	int err;

	/* Get the corresponding watchdog device */
	if (imajor(inode) == MISC_MAJOR)
		wd_data = old_wd_data;
	else
		wd_data = container_of(inode->i_cdev, struct watchdog_core_data,
				       cdev);

	/* the watchdog is single open! */
	if (test_and_set_bit(_WDOG_DEV_OPEN, &wd_data->status))
		return -EBUSY;

	wdd = wd_data->wdd;

	/*
	 * If the /dev/watchdog device is open, we don't want the module
	 * to be unloaded.
	 */
	if (!watchdog_hw_running(wdd) && !try_module_get(wdd->ops->owner)) {
		err = -EBUSY;
		goto out_clear;
	}

	err = watchdog_start(wdd);
	if (err < 0)
		goto out_mod;

	file->private_data = wd_data;

	if (!watchdog_hw_running(wdd))
		kref_get(&wd_data->kref);

	/* dev/watchdog is a virtual (and thus non-seekable) filesystem */
	return nonseekable_open(inode, file);

out_mod:
	module_put(wd_data->wdd->ops->owner);
out_clear:
	clear_bit(_WDOG_DEV_OPEN, &wd_data->status);
	return err;
}

static void watchdog_core_data_release(struct kref *kref)
{
	struct watchdog_core_data *wd_data;

	wd_data = container_of(kref, struct watchdog_core_data, kref);

	kfree(wd_data);
}

static int watchdog_release(struct inode *inode, struct file *file)
{
	struct watchdog_core_data *wd_data = file->private_data;
	struct watchdog_device *wdd;
	int err = -EBUSY;
	bool running;

	mutex_lock(&wd_data->lock);

	wdd = wd_data->wdd;
	if (!wdd)
		goto done;

	/*
	 * We only stop the watchdog if we received the magic character
	 * or if WDIOF_MAGICCLOSE is not set. If nowayout was set then
	 * watchdog_stop will fail.
	 */
	if (!test_bit(WDOG_ACTIVE, &wdd->status))
		err = 0;
	else if (test_and_clear_bit(_WDOG_ALLOW_RELEASE, &wd_data->status) ||
		 !(wdd->info->options & WDIOF_MAGICCLOSE))
		err = watchdog_stop(wdd);

	/* If the watchdog was not stopped, send a keepalive ping */
	if (err < 0) {
		pr_crit("watchdog%d: watchdog did not stop!\n", wdd->id);
		watchdog_ping(wdd);
	}

	watchdog_update_worker(wdd);

	/* make sure that /dev/watchdog can be re-opened */
	clear_bit(_WDOG_DEV_OPEN, &wd_data->status);

done:
	running = wdd && watchdog_hw_running(wdd);
	mutex_unlock(&wd_data->lock);
	/*
	 * Allow the owner module to be unloaded again unless the watchdog
	 * is still running. If the watchdog is still running, it can not
	 * be stopped, and its driver must not be unloaded.
	 */
	if (!running) {
		module_put(wd_data->cdev.owner);
		kref_put(&wd_data->kref, watchdog_core_data_release);
	}
	return 0;
}
static const struct file_operations watchdog_fops = {
	.owner		= THIS_MODULE,
	.write		= watchdog_write,
	.unlocked_ioctl	= watchdog_ioctl,
	.open		= watchdog_open,
	.release	= watchdog_release,
};

static struct miscdevice watchdog_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &watchdog_fops,
};


/*
 *	watchdog_cdev_register: register watchdog character device
 *	@wdd: watchdog device
 *	@devno: character device number
 *
 *	Register a watchdog character device including handling the legacy
 *	/dev/watchdog node. /dev/watchdog is actually a miscdevice and
 *	thus we set it up like that.
 */

static int watchdog_cdev_register(struct watchdog_device *wdd, dev_t devno)
{
	struct watchdog_core_data *wd_data;
	int err;

	wd_data = kzalloc(sizeof(struct watchdog_core_data), GFP_KERNEL);
	if (!wd_data)
		return -ENOMEM;
	kref_init(&wd_data->kref);
	mutex_init(&wd_data->lock);

	wd_data->wdd = wdd;
	wdd->wd_data = wd_data;

	if (!watchdog_wq)
		return -ENODEV;

	INIT_DELAYED_WORK(&wd_data->work, watchdog_ping_work);

	if (wdd->id == 0) {
		old_wd_data = wd_data;
		watchdog_miscdev.parent = wdd->parent;
		err = misc_register(&watchdog_miscdev);
		if (err != 0) {
			pr_err("%s: cannot register miscdev on minor=%d (err=%d).\n",
				wdd->info->identity, WATCHDOG_MINOR, err);
			if (err == -EBUSY)
				pr_err("%s: a legacy watchdog module is probably present.\n",
					wdd->info->identity);
			old_wd_data = NULL;
			kfree(wd_data);
			return err;
		}
	}

	/* Fill in the data structures */
	cdev_init(&wd_data->cdev, &watchdog_fops);
	wd_data->cdev.owner = wdd->ops->owner;

	/* Add the device */
	err = cdev_add(&wd_data->cdev, devno, 1);
	if (err) {
		pr_err("watchdog%d unable to add device %d:%d\n",
			wdd->id,  MAJOR(watchdog_devt), wdd->id);
		if (wdd->id == 0) {
			misc_deregister(&watchdog_miscdev);
			old_wd_data = NULL;
			kref_put(&wd_data->kref, watchdog_core_data_release);
		}
		return err;
	}
	/*copy core data cdev to main dev*/
	memcpy(&wdd->cdev,&wd_data->cdev,sizeof(wd_data->cdev));

	/* Record time of most recent heartbeat as 'just before now'. */
	wd_data->last_hw_keepalive = jiffies - 1;

	/*
	 * If the watchdog is running, prevent its driver from being unloaded,
	 * and schedule an immediate ping.
	 */
	if (watchdog_hw_running(wdd)) {
		__module_get(wdd->ops->owner);
		kref_get(&wd_data->kref);
		queue_delayed_work(watchdog_wq, &wd_data->work, 0);
	}

	return 0;
}

/*
 *	watchdog_cdev_unregister: unregister watchdog character device
 *	@watchdog: watchdog device
 *
 *	Unregister watchdog character device and if needed the legacy
 *	/dev/watchdog device.
 */

static void watchdog_cdev_unregister(struct watchdog_device *wdd)
{
	struct watchdog_core_data *wd_data = wdd->wd_data;

	cdev_del(&wd_data->cdev);
	if (wdd->id == 0) {
		misc_deregister(&watchdog_miscdev);
		old_wd_data = NULL;
	}

	mutex_lock(&wd_data->lock);
	wd_data->wdd = NULL;
	wdd->wd_data = NULL;
	mutex_unlock(&wd_data->lock);

	cancel_delayed_work_sync(&wd_data->work);

	kref_put(&wd_data->kref, watchdog_core_data_release);
}


/*
 *	watchdog_dev_register: register a watchdog device
 *	@wdd: watchdog device
 *
 *	Register a watchdog device including handling the legacy
 *	/dev/watchdog node. /dev/watchdog is actually a miscdevice and
 *	thus we set it up like that.
 */

int watchdog_dev_register(struct watchdog_device *wdd)
{
//	struct device *dev;
	dev_t devno;
	int ret;

	devno = MKDEV(MAJOR(watchdog_devt), wdd->id);
	ret = watchdog_cdev_register(wdd, devno);

	return ret;
}

/*
 *	watchdog_dev_unregister: unregister a watchdog device
 *	@watchdog: watchdog device
 *
 *	Unregister watchdog device and if needed the legacy
 *	/dev/watchdog device.
 */

int watchdog_dev_unregister(struct watchdog_device *wdd)
{
	watchdog_cdev_unregister(wdd);
	return 0;
}

/*
 *	watchdog_dev_init: init dev part of watchdog core
 *
 *	Allocate a range of chardev nodes to use for watchdog devices
 */

int __init watchdog_dev_init(void)
{
	int err;

	watchdog_wq = alloc_workqueue("watchdogd",
				      WQ_HIGHPRI | WQ_MEM_RECLAIM, 0);
	if (!watchdog_wq) {
		pr_err("Failed to create watchdog workqueue\n");
		return -ENOMEM;
	}


	err = alloc_chrdev_region(&watchdog_devt, 0, MAX_DOGS, "watchdog");
	if (err < 0) {
		pr_err("watchdog: unable to allocate char dev region\n");
		goto err_alloc;
	}

	return 0;

err_alloc:
	destroy_workqueue(watchdog_wq);
	return err;
}

/*
 *	watchdog_dev_exit: exit dev part of watchdog core
 *
 *	Release the range of chardev nodes used for watchdog devices
 */

void __exit watchdog_dev_exit(void)
{
	unregister_chrdev_region(watchdog_devt, MAX_DOGS);
	destroy_workqueue(watchdog_wq);
}

#else
/*
 *	watchdog_ping: ping the watchdog.
 *	@wddev: the watchdog device to ping
 *
 *	If the watchdog has no own ping operation then it needs to be
 *	restarted via the start operation. This wrapper function does
 *	exactly that.
 *	We only ping when the watchdog device is running.
 */

static int watchdog_ping(struct watchdog_device *wddev)
{
	int err = 0;

	mutex_lock(&wddev->lock);

	if (test_bit(WDOG_UNREGISTERED, &wddev->status)) {
		err = -ENODEV;
		goto out_ping;
	}

	if (!watchdog_active(wddev))
		goto out_ping;

	if (wddev->ops->ping)
		err = wddev->ops->ping(wddev);  /* ping the watchdog */
	else
		err = wddev->ops->start(wddev); /* restart watchdog */

out_ping:
	mutex_unlock(&wddev->lock);
	return err;
}

/*
 *	watchdog_start: wrapper to start the watchdog.
 *	@wddev: the watchdog device to start
 *
 *	Start the watchdog if it is not active and mark it active.
 *	This function returns zero on success or a negative errno code for
 *	failure.
 */
static int watchdog_start(struct watchdog_device *wddev)
{
	int err = 0;

	mutex_lock(&wddev->lock);

	if (test_bit(WDOG_UNREGISTERED, &wddev->status)) {
		err = -ENODEV;
		goto out_start;
	}

	if (watchdog_active(wddev))
		goto out_start;

	err = wddev->ops->start(wddev);
	if (err == 0)
		set_bit(WDOG_ACTIVE, &wddev->status);

out_start:
	mutex_unlock(&wddev->lock);
	return err;
}

/*
 *	watchdog_stop: wrapper to stop the watchdog.
 *	@wddev: the watchdog device to stop
 *
 *	Stop the watchdog if it is still active and unmark it active.
 *	This function returns zero on success or a negative errno code for
 *	failure.
 *	If the 'nowayout' feature was set, the watchdog cannot be stopped.
 */
static int watchdog_stop(struct watchdog_device *wddev)
{
	int err = 0;

	mutex_lock(&wddev->lock);

	if (test_bit(WDOG_UNREGISTERED, &wddev->status)) {
		err = -ENODEV;
		goto out_stop;
	}

	if (!watchdog_active(wddev))
		goto out_stop;

	if (test_bit(WDOG_NO_WAY_OUT, &wddev->status)) {
		dev_info(wddev->dev, "nowayout prevents watchdog being stopped!\n");
		err = -EBUSY;
		goto out_stop;
	}

	err = wddev->ops->stop(wddev);
	if (err == 0)
		clear_bit(WDOG_ACTIVE, &wddev->status);

out_stop:
	mutex_unlock(&wddev->lock);
	return err;
}

/*
 *	watchdog_get_status: wrapper to get the watchdog status
 *	@wddev: the watchdog device to get the status from
 *	@status: the status of the watchdog device
 *
 *	Get the watchdog's status flags.
 */

static int watchdog_get_status(struct watchdog_device *wddev,
							unsigned int *status)
{
	int err = 0;

	*status = 0;
	if (!wddev->ops->status)
		return -EOPNOTSUPP;

	mutex_lock(&wddev->lock);

	if (test_bit(WDOG_UNREGISTERED, &wddev->status)) {
		err = -ENODEV;
		goto out_status;
	}

	*status = wddev->ops->status(wddev);

out_status:
	mutex_unlock(&wddev->lock);
	return err;
}

/*
 *	watchdog_set_timeout: set the watchdog timer timeout
 *	@wddev: the watchdog device to set the timeout for
 *	@timeout: timeout to set in seconds
 */

static int watchdog_set_timeout(struct watchdog_device *wddev,
							unsigned int timeout)
{
	int err;

	if ((wddev->ops->set_timeout == NULL) ||
	    !(wddev->info->options & WDIOF_SETTIMEOUT))
		return -EOPNOTSUPP;

	if (watchdog_timeout_invalid(wddev, timeout))
		return -EINVAL;

	mutex_lock(&wddev->lock);

	if (test_bit(WDOG_UNREGISTERED, &wddev->status)) {
		err = -ENODEV;
		goto out_timeout;
	}

	err = wddev->ops->set_timeout(wddev, timeout);

out_timeout:
	mutex_unlock(&wddev->lock);
	return err;
}

/*
 *	watchdog_get_timeleft: wrapper to get the time left before a reboot
 *	@wddev: the watchdog device to get the remaining time from
 *	@timeleft: the time that's left
 *
 *	Get the time before a watchdog will reboot (if not pinged).
 */

static int watchdog_get_timeleft(struct watchdog_device *wddev,
							unsigned int *timeleft)
{
	int err = 0;

	*timeleft = 0;
	if (!wddev->ops->get_timeleft)
		return -EOPNOTSUPP;

	mutex_lock(&wddev->lock);

	if (test_bit(WDOG_UNREGISTERED, &wddev->status)) {
		err = -ENODEV;
		goto out_timeleft;
	}

	*timeleft = wddev->ops->get_timeleft(wddev);

out_timeleft:
	mutex_unlock(&wddev->lock);
	return err;
}

/*
 *	watchdog_ioctl_op: call the watchdog drivers ioctl op if defined
 *	@wddev: the watchdog device to do the ioctl on
 *	@cmd: watchdog command
 *	@arg: argument pointer
 */

static int watchdog_ioctl_op(struct watchdog_device *wddev, unsigned int cmd,
							unsigned long arg)
{
	int err;

	if (!wddev->ops->ioctl)
		return -ENOIOCTLCMD;

	mutex_lock(&wddev->lock);

	if (test_bit(WDOG_UNREGISTERED, &wddev->status)) {
		err = -ENODEV;
		goto out_ioctl;
	}

	err = wddev->ops->ioctl(wddev, cmd, arg);

out_ioctl:
	mutex_unlock(&wddev->lock);
	return err;
}

/*
 *	watchdog_write: writes to the watchdog.
 *	@file: file from VFS
 *	@data: user address of data
 *	@len: length of data
 *	@ppos: pointer to the file offset
 *
 *	A write to a watchdog device is defined as a keepalive ping.
 *	Writing the magic 'V' sequence allows the next close to turn
 *	off the watchdog (if 'nowayout' is not set).
 */

static ssize_t watchdog_write(struct file *file, const char __user *data,
						size_t len, loff_t *ppos)
{
	struct watchdog_device *wdd = file->private_data;
	size_t i;
	char c;

	if (len == 0)
		return 0;

	/*
	 * Note: just in case someone wrote the magic character
	 * five months ago...
	 */
	clear_bit(WDOG_ALLOW_RELEASE, &wdd->status);

	/* scan to see whether or not we got the magic character */
	for (i = 0; i != len; i++) {
		if (get_user(c, data + i))
			return -EFAULT;
		if (c == 'V')
			set_bit(WDOG_ALLOW_RELEASE, &wdd->status);
	}

	/* someone wrote to us, so we send the watchdog a keepalive ping */
	watchdog_ping(wdd);

	return len;
}

/*
 *	watchdog_ioctl: handle the different ioctl's for the watchdog device.
 *	@file: file handle to the device
 *	@cmd: watchdog command
 *	@arg: argument pointer
 *
 *	The watchdog API defines a common set of functions for all watchdogs
 *	according to their available features.
 */

static long watchdog_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	struct watchdog_device *wdd = file->private_data;
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	unsigned int val;
	int err;

	err = watchdog_ioctl_op(wdd, cmd, arg);
	if (err != -ENOIOCTLCMD)
		return err;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, wdd->info,
			sizeof(struct watchdog_info)) ? -EFAULT : 0;
	case WDIOC_GETSTATUS:
		err = watchdog_get_status(wdd, &val);
		if (err == -ENODEV)
			return err;
		return put_user(val, p);
	case WDIOC_GETBOOTSTATUS:
		return put_user(wdd->bootstatus, p);
	case WDIOC_SETOPTIONS:
		if (get_user(val, p))
			return -EFAULT;
		if (val & WDIOS_DISABLECARD) {
			err = watchdog_stop(wdd);
			if (err < 0)
				return err;
		}
		if (val & WDIOS_ENABLECARD) {
			err = watchdog_start(wdd);
			if (err < 0)
				return err;
		}
		return 0;
	case WDIOC_KEEPALIVE:
		if (!(wdd->info->options & WDIOF_KEEPALIVEPING))
			return -EOPNOTSUPP;
		watchdog_ping(wdd);
		return 0;
	case WDIOC_SETTIMEOUT:
		if (get_user(val, p))
			return -EFAULT;
		err = watchdog_set_timeout(wdd, val);
		if (err < 0)
			return err;
		/* If the watchdog is active then we send a keepalive ping
		 * to make sure that the watchdog keep's running (and if
		 * possible that it takes the new timeout) */
		watchdog_ping(wdd);
		/* Fall */
	case WDIOC_GETTIMEOUT:
		/* timeout == 0 means that we don't know the timeout */
		if (wdd->timeout == 0)
			return -EOPNOTSUPP;
		return put_user(wdd->timeout, p);
	case WDIOC_GETTIMELEFT:
		err = watchdog_get_timeleft(wdd, &val);
		if (err)
			return err;
		return put_user(val, p);
	default:
		return -ENOTTY;
	}
}

/*
 *	watchdog_open: open the /dev/watchdog* devices.
 *	@inode: inode of device
 *	@file: file handle to device
 *
 *	When the /dev/watchdog* device gets opened, we start the watchdog.
 *	Watch out: the /dev/watchdog device is single open, so we make sure
 *	it can only be opened once.
 */

static int watchdog_open(struct inode *inode, struct file *file)
{
	int err = -EBUSY;
	struct watchdog_device *wdd;

	/* Get the corresponding watchdog device */
	if (imajor(inode) == MISC_MAJOR)
		wdd = old_wdd;
	else
		wdd = container_of(inode->i_cdev, struct watchdog_device, cdev);

	/* the watchdog is single open! */
	if (test_and_set_bit(WDOG_DEV_OPEN, &wdd->status))
		return -EBUSY;

	/*
	 * If the /dev/watchdog device is open, we don't want the module
	 * to be unloaded.
	 */
	if (!try_module_get(wdd->ops->owner))
		goto out;

	err = watchdog_start(wdd);
	if (err < 0)
		goto out_mod;

	file->private_data = wdd;

	if (wdd->ops->ref)
		wdd->ops->ref(wdd);

	/* dev/watchdog is a virtual (and thus non-seekable) filesystem */
	return nonseekable_open(inode, file);

out_mod:
	module_put(wdd->ops->owner);
out:
	clear_bit(WDOG_DEV_OPEN, &wdd->status);
	return err;
}

/*
 *	watchdog_release: release the watchdog device.
 *	@inode: inode of device
 *	@file: file handle to device
 *
 *	This is the code for when /dev/watchdog gets closed. We will only
 *	stop the watchdog when we have received the magic char (and nowayout
 *	was not set), else the watchdog will keep running.
 */

static int watchdog_release(struct inode *inode, struct file *file)
{
	struct watchdog_device *wdd = file->private_data;
	int err = -EBUSY;

	/*
	 * We only stop the watchdog if we received the magic character
	 * or if WDIOF_MAGICCLOSE is not set. If nowayout was set then
	 * watchdog_stop will fail.
	 */
	if (test_and_clear_bit(WDOG_ALLOW_RELEASE, &wdd->status) ||
	    !(wdd->info->options & WDIOF_MAGICCLOSE))
		err = watchdog_stop(wdd);

	/* If the watchdog was not stopped, send a keepalive ping */
	if (err < 0) {
		mutex_lock(&wdd->lock);
		if (!test_bit(WDOG_UNREGISTERED, &wdd->status))
			dev_crit(wdd->dev, "watchdog did not stop!\n");
		mutex_unlock(&wdd->lock);
		watchdog_ping(wdd);
	}

	/* Allow the owner module to be unloaded again */
	module_put(wdd->ops->owner);

	/* make sure that /dev/watchdog can be re-opened */
	clear_bit(WDOG_DEV_OPEN, &wdd->status);

	/* Note wdd may be gone after this, do not use after this! */
	if (wdd->ops->unref)
		wdd->ops->unref(wdd);

	return 0;
}

static const struct file_operations watchdog_fops = {
	.owner		= THIS_MODULE,
	.write		= watchdog_write,
	.unlocked_ioctl	= watchdog_ioctl,
	.open		= watchdog_open,
	.release	= watchdog_release,
};

static struct miscdevice watchdog_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &watchdog_fops,
};

/*
 *	watchdog_dev_register: register a watchdog device
 *	@watchdog: watchdog device
 *
 *	Register a watchdog device including handling the legacy
 *	/dev/watchdog node. /dev/watchdog is actually a miscdevice and
 *	thus we set it up like that.
 */

int watchdog_dev_register(struct watchdog_device *watchdog)
{
	int err, devno;

	if (watchdog->id == 0) {
		old_wdd = watchdog;
		watchdog_miscdev.parent = watchdog->parent;
		err = misc_register(&watchdog_miscdev);
		if (err != 0) {
			pr_err("%s: cannot register miscdev on minor=%d (err=%d).\n",
				watchdog->info->identity, WATCHDOG_MINOR, err);
			if (err == -EBUSY)
				pr_err("%s: a legacy watchdog module is probably present.\n",
					watchdog->info->identity);
			old_wdd = NULL;
			return err;
		}
	}

	/* Fill in the data structures */
	devno = MKDEV(MAJOR(watchdog_devt), watchdog->id);
	cdev_init(&watchdog->cdev, &watchdog_fops);
	watchdog->cdev.owner = watchdog->ops->owner;

	/* Add the device */
	err  = cdev_add(&watchdog->cdev, devno, 1);
	if (err) {
		pr_err("watchdog%d unable to add device %d:%d\n",
			watchdog->id,  MAJOR(watchdog_devt), watchdog->id);
		if (watchdog->id == 0) {
			misc_deregister(&watchdog_miscdev);
			old_wdd = NULL;
		}
	}
	return err;
}

/*
 *	watchdog_dev_unregister: unregister a watchdog device
 *	@watchdog: watchdog device
 *
 *	Unregister the watchdog and if needed the legacy /dev/watchdog device.
 */

int watchdog_dev_unregister(struct watchdog_device *watchdog)
{
	mutex_lock(&watchdog->lock);
	set_bit(WDOG_UNREGISTERED, &watchdog->status);
	mutex_unlock(&watchdog->lock);

	cdev_del(&watchdog->cdev);
	if (watchdog->id == 0) {
		misc_deregister(&watchdog_miscdev);
		old_wdd = NULL;
	}
	return 0;
}

/*
 *	watchdog_dev_init: init dev part of watchdog core
 *
 *	Allocate a range of chardev nodes to use for watchdog devices
 */

int __init watchdog_dev_init(void)
{
	int err = alloc_chrdev_region(&watchdog_devt, 0, MAX_DOGS, "watchdog");
	if (err < 0)
		pr_err("watchdog: unable to allocate char dev region\n");
	return err;
}

/*
 *	watchdog_dev_exit: exit dev part of watchdog core
 *
 *	Release the range of chardev nodes used for watchdog devices
 */

void __exit watchdog_dev_exit(void)
{
	unregister_chrdev_region(watchdog_devt, MAX_DOGS);
}
#endif
