/*****************************************************************************
 **   FILE NAME       : ltqusb_ctl.c
 **   PROJECT         : Lantiq USB sub-system V3
 **   MODULES         : Lantiq USB sub-system Host and Device driver
 **   SRC VERSION     : 1.0
 **   SRC VERSION     : 3.2
 **   DATE            : 1/Jan/2011
 **   DESCRIPTION     : Implementing the procfs and sysfs for Lantiq USB driver
 **   FUNCTIONS       :
 **   COMPILER        : gcc
 **   REFERENCE       : Synopsys DWC-OTG Driver 2.7
 **   COPYRIGHT       :  Copyright (c) 2010
 **                      LANTIQ DEUTSCHLAND GMBH,
 **                      Am Campeon 3, 85579 Neubiberg, Germany
 **
 **    This program is free software; you can redistribute it and/or modify
 **    it under the terms of the GNU General Public License as published by
 **    the Free Software Foundation; either version 2 of the License, or
 **    (at your option) any later version.
 **
 **  Version Control Section  **
 **   $Author$
 **   $Date$
 **   $Revisions$
 **   $Log$       Revision history
 *****************************************************************************/

/*
 * This file contains code fragments from Synopsys HS OTG Linux Software Driver.
 * For this code the following notice is applicable:
 *
 * ==========================================================================
 *
 * Synopsys HS OTG Linux Software Driver and documentation (hereinafter,
 * "Software") is an Unsupported proprietary work of Synopsys, Inc. unless
 * otherwise expressly agreed to in writing between Synopsys and you.
 *
 * The Software IS NOT an item of Licensed Software or Licensed Product under
 * any End User Software License Agreement or Agreement for Licensed Product
 * with Synopsys or any supplement thereto. You are permitted to use and
 * redistribute this Software in source and binary forms, with or without
 * modification, provided that redistributions of source code must retain this
 * notice. You may not view, use, disclose, copy or distribute this file or
 * any information contained herein except pursuant to this license grant from
 * Synopsys. If you do not agree with this notice, including the disclaimer
 * below, then you are not authorized to use the Software.
 *
 * THIS SOFTWARE IS BEING DISTRIBUTED BY SYNOPSYS SOLELY ON AN "AS IS" BASIS
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE HEREBY DISCLAIMED. IN NO EVENT SHALL SYNOPSYS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 * ========================================================================== */

/*! \file ltqusb_ctl.c
  \ingroup LTQUSB_DRIVER_V3
    \brief Implementing the procfs and sysfs for Lantiq USB driver
*/

#include <linux/version.h>
#include "ltqusb_version.h"


#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/byteorder.h>
#include <asm/unaligned.h>
#include <asm/uaccess.h>

#include "ltqusb_plat.h"
#include "ltqusb_regs.h"
#include "ltqusb_cif.h"

#define __USE_PROCFS__ 1
#define __USE_SYSFS__  1

#include "ltqhcd.h"
#ifdef __HOST_COC__
	#ifdef __NEW_COC__
		#include <ltq_lxfreq.h>
		#include <linux/cpufreq.h>
	#endif
	LTQ_PMCU_REGISTER_t pmcuRegisterUSBHost_1;
	LTQ_PMCU_REGISTER_t pmcuRegisterUSBHost_2;
	unsigned char pmcuPowerFeatureUSBHost_1 = 1;
	unsigned char pmcuPowerFeatureUSBHost_2 = 1;
#endif

#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/gfp.h>

extern char ltqusb_hcd_driver_name[];

extern ltqhcd_hcd_t ltqusb_hcd_1;
extern ltqhcd_hcd_t ltqusb_hcd_2;
extern char ltqusb_hcd_name_1[];
extern char ltqusb_hcd_name_2[];

#if defined(CONFIG_CPU_FREQ) && defined(__NEW_COC__)
	/* Linux CPUFREQ support start */
	#if defined(__HOST_COC__)
		static LTQ_PMCU_RETURN_t ltq_usbhost_stateGet_1(LTQ_PMCU_STATE_t *pmcuModState);
		static LTQ_PMCU_RETURN_t ltq_usbhost_pwrFeatureSwitch_1(LTQ_PMCU_PWR_STATE_ENA_t pmcuPwrStateEna);
		static LTQ_PMCU_RETURN_t ltq_usbhost_preChange_1(LTQ_PMCU_MODULE_t pmcuModule, LTQ_PMCU_STATE_t newState, LTQ_PMCU_STATE_t oldState);
		static LTQ_PMCU_RETURN_t ltq_usbhost_postChange_1(LTQ_PMCU_MODULE_t pmcuModule, LTQ_PMCU_STATE_t newState, LTQ_PMCU_STATE_t oldState);
		static LTQ_PMCU_RETURN_t ltq_usbhost_stateChange_1(LTQ_PMCU_STATE_t newState);
		static LTQ_PMCU_RETURN_t ltq_usbhost_stateGet_2(LTQ_PMCU_STATE_t *pmcuModState);
		static LTQ_PMCU_RETURN_t ltq_usbhost_pwrFeatureSwitch_2(LTQ_PMCU_PWR_STATE_ENA_t pmcuPwrStateEna);
		static LTQ_PMCU_RETURN_t ltq_usbhost_preChange_2(LTQ_PMCU_MODULE_t pmcuModule, LTQ_PMCU_STATE_t newState, LTQ_PMCU_STATE_t oldState);
		static LTQ_PMCU_RETURN_t ltq_usbhost_postChange_2(LTQ_PMCU_MODULE_t pmcuModule, LTQ_PMCU_STATE_t newState, LTQ_PMCU_STATE_t oldState);
		static LTQ_PMCU_RETURN_t ltq_usbhost_stateChange_2(LTQ_PMCU_STATE_t newState);
		struct LTQ_LXFREQ_MODSTRUCT ltq_usbhost1_lxfreq_mod_g = {
			.name							= "USB1 HOST power support",
			.pmcuModule						= LTQ_PMCU_MODULE_USB,
			.pmcuModuleNr					= 1,
			.powerFeatureStat				= LTQ_PMCU_PWR_STATE_ON,
			.ltq_lxfreq_state_get			= ltq_usbhost_stateGet_1,
			.ltq_lxfreq_pwr_feature_switch	= ltq_usbhost_pwrFeatureSwitch_1,
		};

		struct LTQ_LXFREQ_MODSTRUCT ltq_usbhost2_lxfreq_mod_g = {
			.name							= "USB2 HOST power support",
			.pmcuModule						= LTQ_PMCU_MODULE_USB,
			.pmcuModuleNr					= 2,
			.powerFeatureStat				= LTQ_PMCU_PWR_STATE_ON,
			.ltq_lxfreq_state_get			= ltq_usbhost_stateGet_2,
			.ltq_lxfreq_pwr_feature_switch	= ltq_usbhost_pwrFeatureSwitch_2,
		};
	#endif //defined(__HOST_COC__)

	extern struct list_head ltq_lxfreq_head_mod_list_g;

#if 0 /*CPUFREQ*/
	#if defined(__HOST_COC__)
		static int
		ltq_usbhost_1_cpufreq_notifier(struct notifier_block *nb, unsigned long val,
								 void *data);

		static int ltq_usbhost_1_cpufreq_notifier(struct notifier_block *nb, unsigned long val,
										 void *data);
		static struct notifier_block ltq_usbhost_1_cpufreq_notifier_block = {
			.notifier_call  = ltq_usbhost_1_cpufreq_notifier
		};

		/* keep track of frequency transitions */
		static int
		ltq_usbhost_1_cpufreq_notifier(struct notifier_block *nb, unsigned long val, void *data)
		{
			struct cpufreq_freqs *freq = data;
			LTQ_PMCU_STATE_t new_State,old_State;
			LTQ_PMCU_RETURN_t ret = 0;
			//u32 count_start;

			new_State = ltq_lxfreq_get_ps_from_khz(freq->new);
			if(new_State == LTQ_PMCU_STATE_INVALID) {
				return NOTIFY_STOP_MASK | (LTQ_PMCU_MODULE_USB<<4);
			}
			old_State = ltq_lxfreq_get_ps_from_khz(freq->old);
			if(old_State == LTQ_PMCU_STATE_INVALID) {
				return NOTIFY_STOP_MASK | (LTQ_PMCU_MODULE_USB<<4);
			}
			if (val == CPUFREQ_PRECHANGE){
				ret = ltq_usbhost_preChange_1(LTQ_PMCU_MODULE_USB, new_State, old_State);
				if (ret == LTQ_PMCU_RETURN_DENIED) {
					return NOTIFY_STOP_MASK | (LTQ_PMCU_MODULE_USB<<4);
				}
				//count_start = ltq_count0_read();
				//ret = ltq_usbhost_stateChange_1(new_State);
				//ltq_count0_diff(count_start, ltq_count0_read(),"USB_HOST_1_STATECHANGE");
				if (ret == LTQ_PMCU_RETURN_DENIED) {
					return NOTIFY_STOP_MASK | (LTQ_PMCU_MODULE_USB<<4);
				}
			} else if (val == CPUFREQ_POSTCHANGE){
				ret = ltq_usbhost_postChange_1(LTQ_PMCU_MODULE_USB, new_State, old_State);
				if (ret == LTQ_PMCU_RETURN_DENIED) {
					return NOTIFY_STOP_MASK | (LTQ_PMCU_MODULE_USB<<4);
				}
			}else{
				return NOTIFY_OK | (LTQ_PMCU_MODULE_USB<<4);
			}
			return NOTIFY_OK | (LTQ_PMCU_MODULE_USB<<4);
		}

		static int
		ltq_usbhost_2_cpufreq_notifier(struct notifier_block *nb, unsigned long val,
								 void *data);

		static int ltq_usbhost_2_cpufreq_notifier(struct notifier_block *nb, unsigned long val,
										 void *data);
		static struct notifier_block ltq_usbhost_2_cpufreq_notifier_block = {
			.notifier_call  = ltq_usbhost_2_cpufreq_notifier
		};

		/* keep track of frequency transitions */
		static int
		ltq_usbhost_2_cpufreq_notifier(struct notifier_block *nb, unsigned long val, void *data)
		{
			struct cpufreq_freqs *freq = data;
			LTQ_PMCU_STATE_t new_State,old_State;
			LTQ_PMCU_RETURN_t ret = 0;
			//u32 count_start;

			new_State = ltq_lxfreq_get_ps_from_khz(freq->new);
			if(new_State == LTQ_PMCU_STATE_INVALID) {
				return NOTIFY_STOP_MASK | (LTQ_PMCU_MODULE_USB<<4);
			}
			old_State = ltq_lxfreq_get_ps_from_khz(freq->old);
			if(old_State == LTQ_PMCU_STATE_INVALID) {
				return NOTIFY_STOP_MASK | (LTQ_PMCU_MODULE_USB<<4);
			}
			if (val == CPUFREQ_PRECHANGE){
				ret = ltq_usbhost_preChange_2(LTQ_PMCU_MODULE_USB, new_State, old_State);
				if (ret == LTQ_PMCU_RETURN_DENIED) {
					return NOTIFY_STOP_MASK | (LTQ_PMCU_MODULE_USB<<4);
				}
				//count_start = ltq_count0_read();
				//ret = ltq_usbhost_stateChange_2(new_State);
				//ltq_count0_diff(count_start, ltq_count0_read(),"USB_HOST_2_STATECHANGE");
				if (ret == LTQ_PMCU_RETURN_DENIED) {
					return NOTIFY_STOP_MASK | (LTQ_PMCU_MODULE_USB<<4);
				}
			} else if (val == CPUFREQ_POSTCHANGE){
				ret = ltq_usbhost_postChange_2(LTQ_PMCU_MODULE_USB, new_State, old_State);
				if (ret == LTQ_PMCU_RETURN_DENIED) {
					return NOTIFY_STOP_MASK | (LTQ_PMCU_MODULE_USB<<4);
				}
			}else{
				return NOTIFY_OK | (LTQ_PMCU_MODULE_USB<<4);
			}
			return NOTIFY_OK | (LTQ_PMCU_MODULE_USB<<4);
		}
	#endif //defined(__HOST_COC__)
#endif /*CPUFREQ*/
/* Linux CPUFREQ support end */
#endif //defined(CONFIG_CPU_FREQ) && defined(__NEW_COC__)



#ifdef __USE_PROCFS__
	static int procfs_rw_open(struct inode *inode, struct file *filp)
	{
//		try_module_get(THIS_MODULE);
		return 0;
	}
#endif //__USE_PROCFS__



/////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCSYSFS:  VERSION
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __USE_PROCFS__
	static int procfs_version_show(struct seq_file *filp, void *offset){ seq_printf(filp, "%s\n",LTQUSB_VERSION );return 0;}
	static int procfs_version_open(struct inode *inode, struct file *filp){ return single_open(filp, procfs_version_show, NULL);}
	static const struct file_operations procfs_version_fops={
	 	.open		= procfs_version_open,
	 	.read		= seq_read,
	 	.llseek		= seq_lseek,
	};
#endif //__USE_PROCFS__

#ifdef __USE_SYSFS__
	static ssize_t sysfs_version_show( struct device *_dev, struct device_attribute *attr,char *buf) { 	return sprintf( buf, "%s\n",LTQUSB_VERSION ); }
	extern struct device_attribute dev_attr_version_h;
	DEVICE_ATTR(version_h, S_IRUGO, sysfs_version_show, NULL);
#endif //__USE_SYSFS__

/////////////////////////////////////////////////////////////////////////////////////////////////////
// e/PROCSYSFS:  VERSION
/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCSYSFS:  DBGLEVEL
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __USE_PROCFS__
	static ssize_t procfs_dbglevel_show(struct file *filp, char *buf, size_t length, loff_t *offset)
	{
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf( buf, "%08X\n",h_dbg_lvl );
	}

	static ssize_t procfs_dbglevel_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 16);
		h_dbg_lvl =value;
		return length;
	}

	static const struct file_operations procfs_dbglevel_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_dbglevel_show,
		.write 	= procfs_dbglevel_store,
	};

#endif //__USE_PROCFS__

#ifdef __USE_SYSFS__
	static ssize_t sysfs_dbglevel_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return sprintf( buf, "%08X\n",h_dbg_lvl );
	}

	static ssize_t sysfs_dbglevel_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 16);
		h_dbg_lvl =value;
		return count;
	}
	extern struct device_attribute dev_attr_dbglevel_h;
	DEVICE_ATTR(dbglevel_h, S_IRUGO|S_IWUSR, sysfs_dbglevel_show, sysfs_dbglevel_store);
#endif //__USE_SYSFS__
/////////////////////////////////////////////////////////////////////////////////////////////////////
// e/PROCSYSFS:  DBGLEVEL
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCSYSFS:  DUMP_PARAMS MODE
/////////////////////////////////////////////////////////////////////////////////////////////////////
static void ltqusb_dump_params(ltqusb_core_if_t *_core_if);

static ssize_t dump_params_1(void)	{ ltqusb_dump_params(&ltqusb_hcd_1.core_if); return 0;}
static ssize_t dump_params_2(void)	{ ltqusb_dump_params(&ltqusb_hcd_2.core_if); return 0;}

#ifdef __USE_PROCFS__
	static int procfs_dump_params_show_1(struct seq_file *filp, void *offset){ dump_params_1();return 0;}
	static int procfs_dump_params_show_2(struct seq_file *filp, void *offset){ dump_params_2();return 0;}
	static int procfs_dump_params_open_1(struct inode *inode, struct file *filp) { return single_open(filp, procfs_dump_params_show_1, NULL);}
	static int procfs_dump_params_open_2(struct inode *inode, struct file *filp) { return single_open(filp, procfs_dump_params_show_2, NULL);}
	static const struct file_operations procfs_dump_params_fops_1={
	 	.open		= procfs_dump_params_open_1,
	 	.read		= seq_read,
	 	.llseek		= seq_lseek,
	};
	static const struct file_operations procfs_dump_params_fops_2={
	 	.open		= procfs_dump_params_open_2,
	 	.read		= seq_read,
	 	.llseek		= seq_lseek,
	};

	static int procfs_mode_show_1(struct seq_file *filp, void *offset)
	{
		if((ltqusb_rreg(&ltqusb_hcd_1.core_if.core_global_regs->gintsts ) & 0x1) == 1)
			seq_printf(filp, "HOST\n" );
		else
			seq_printf(filp, "DEVICE(INCORRECT!)\n" );
		return 0;
	}
	static int procfs_mode_show_2(struct seq_file *filp, void *offset)
	{
		if((ltqusb_rreg(&ltqusb_hcd_2.core_if.core_global_regs->gintsts ) & 0x1) == 1)
			seq_printf(filp, "HOST\n" );
		else
			seq_printf(filp, "DEVICE(INCORRECT!)\n" );
		return 0;
	}
	static int procfs_mode_open_1(struct inode *inode, struct file *filp) { return single_open(filp, procfs_mode_show_1, NULL);}
	static int procfs_mode_open_2(struct inode *inode, struct file *filp) { return single_open(filp, procfs_mode_show_2, NULL);}
	static const struct file_operations procfs_mode_fops_1=
	{
	 	.open		= procfs_mode_open_1,
	 	.read		= seq_read,
	 	.llseek		= seq_lseek,
	};
	static const struct file_operations procfs_mode_fops_2=
	{
	 	.open		= procfs_mode_open_2,
	 	.read		= seq_read,
	 	.llseek		= seq_lseek,
	};
#endif //__USE_PROCFS__

#ifdef __USE_SYSFS__
	static ssize_t sysfs_dump_params_show_1( struct device *_dev, struct device_attribute *attr,char *buf) { return dump_params_1();}
	static ssize_t sysfs_dump_params_show_2( struct device *_dev, struct device_attribute *attr,char *buf) { return dump_params_2();}

	static ssize_t sysfs_mode_show_1( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		if((ltqusb_rreg(&ltqusb_hcd_1.core_if.core_global_regs->gintsts ) & 0x1) == 1)
			return sprintf( buf, "HOST\n" );
		else
			return sprintf( buf, "DEVICE(INCORRECT!)\n" );
	}
	static ssize_t sysfs_mode_show_2( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		if((ltqusb_rreg(&ltqusb_hcd_2.core_if.core_global_regs->gintsts ) & 0x1) == 1)
			return sprintf( buf, "HOST\n" );
		else
			return sprintf( buf, "DEVICE(INCORRECT!)\n" );
	}

	extern struct device_attribute dev_attr_dump_params_h_1;
	extern struct device_attribute dev_attr_dump_params_h_2;
	DEVICE_ATTR(dump_params_h_1, S_IRUGO, sysfs_dump_params_show_1, NULL);
	DEVICE_ATTR(dump_params_h_2, S_IRUGO, sysfs_dump_params_show_2, NULL);

	extern struct device_attribute dev_attr_mode_h_1;
	extern struct device_attribute dev_attr_mode_h_2;
	DEVICE_ATTR(mode_h_1, S_IRUGO, sysfs_mode_show_1, NULL);
	DEVICE_ATTR(mode_h_2, S_IRUGO, sysfs_mode_show_2, NULL);
#endif //__USE_SYSFS__
////////////////////////////////////////////////////////////////////////////////////////////////////
// e/PROCSYSFS:  DUMP_PARAMS MODE
/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCSYSFS:  HOST STATUS
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __USE_PROCFS__
	static int procfs_host_port_status_show_1(struct seq_file *filp, void *offset)
	{
		hprt0_data_t val;
		val.d32 = ltqusb_rreg(ltqusb_hcd_1.core_if.hprt0);
		seq_printf(filp, "Suspended = 0x%x, Connected = 0x%x, Speed = %s(%d)\n"
			, val.b.prtsusp
			, val.b.prtconnsts
			,(val.b.prtspd ==0)?"High":((val.b.prtspd ==1)?"Full":((val.b.prtspd ==2)?"Low":"Unknown"))
			,val.b.prtspd
			);
		return 0;
	}
	static int procfs_host_port_status_show_2(struct seq_file *filp, void *offset)
	{
		hprt0_data_t val;
		val.d32 = ltqusb_rreg(ltqusb_hcd_2.core_if.hprt0);
		seq_printf(filp, "Suspended = 0x%x, Connected = 0x%x, Speed = %s(%d)\n"
			, val.b.prtsusp
			, val.b.prtconnsts
			,(val.b.prtspd ==0)?"High":((val.b.prtspd ==1)?"Full":((val.b.prtspd ==2)?"Low":"Unknown"))
			,val.b.prtspd
			);
		return 0;
	}
	static int procfs_host_port_status_open_1(struct inode *inode, struct file *filp) {	return single_open(filp, procfs_host_port_status_show_1, NULL); }
	static int procfs_host_port_status_open_2(struct inode *inode, struct file *filp) {	return single_open(filp, procfs_host_port_status_show_2, NULL); }
	static const struct file_operations procfs_host_port_status_fops_1= {
 		.open		= procfs_host_port_status_open_1,
 		.read		= seq_read,
 		.llseek		= seq_lseek,
	};
	static const struct file_operations procfs_host_port_status_fops_2= {
 		.open		= procfs_host_port_status_open_2,
 		.read		= seq_read,
 		.llseek		= seq_lseek,
	};
#endif //__USE_PROCFS__
#ifdef __USE_SYSFS__
	static ssize_t sysfs_host_port_status_show_1( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		hprt0_data_t val;
		val.d32 = ltqusb_rreg(ltqusb_hcd_1.core_if.hprt0);
		return sprintf (buf, "Suspended = 0x%x, Connected = 0x%x, Speed = %s(%d)\n"
			, val.b.prtsusp
			, val.b.prtconnsts
			,(val.b.prtspd ==0)?"High":((val.b.prtspd ==1)?"Full":((val.b.prtspd ==2)?"Low":"Unknown"))
			,val.b.prtspd
			);
	}
	static ssize_t sysfs_host_port_status_show_2( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		hprt0_data_t val;
		val.d32 = ltqusb_rreg(ltqusb_hcd_2.core_if.hprt0);
		return sprintf (buf, "Suspended = 0x%x, Connected = 0x%x, Speed = %s(%d)\n"
			, val.b.prtsusp
			, val.b.prtconnsts
			,(val.b.prtspd ==0)?"High":((val.b.prtspd ==1)?"Full":((val.b.prtspd ==2)?"Low":"Unknown"))
			,val.b.prtspd
			);
	}
	extern struct device_attribute dev_attr_host_port_status_1;
	extern struct device_attribute dev_attr_host_port_status_2;
	DEVICE_ATTR(host_port_status_1, S_IRUGO, sysfs_host_port_status_show_1, NULL);
	DEVICE_ATTR(host_port_status_2, S_IRUGO, sysfs_host_port_status_show_2, NULL);
#endif //__USE_SYSFS__
/////////////////////////////////////////////////////////////////////////////////////////////////////
// e/PROCSYSFS:  HOST STATUS
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCSYSFS:  DUMP
/////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __ENABLE_DUMP__
	static void dump_reg_1(void) { ltqusb_dump_registers_h(&ltqusb_hcd_1.core_if); }
	static void dump_reg_2(void) { ltqusb_dump_registers_h(&ltqusb_hcd_2.core_if); }
	static void dump_spram_1(void) { ltqusb_dump_spram_h(&ltqusb_hcd_1.core_if); }
	static void dump_spram_2(void) { ltqusb_dump_spram_h(&ltqusb_hcd_2.core_if); }
	static void dump_host_state_1(void) { ltqhcd_dump_state(&ltqusb_hcd_1); }
	static void dump_host_state_2(void) { ltqhcd_dump_state(&ltqusb_hcd_2); }

	#ifdef __USE_PROCFS__
		static ssize_t procfs_dump_reg_h_show_1(struct seq_file *filp, void *offset){ dump_reg_1();return 0;}
		static ssize_t procfs_dump_reg_h_show_2(struct seq_file *filp, void *offset){ dump_reg_2();return 0;}
		static int procfs_dump_reg_h_open_1(struct inode *inode, struct file *filp) { return single_open(filp, procfs_dump_reg_h_show_1, NULL);}
		static int procfs_dump_reg_h_open_2(struct inode *inode, struct file *filp) { return single_open(filp, procfs_dump_reg_h_show_2, NULL);}
		static const struct file_operations procfs_dump_reg_h_fops_1= {
	 		.open		= procfs_dump_reg_h_open_1,
	 		.read		= seq_read,
	 		.llseek		= seq_lseek,
		};
		static const struct file_operations procfs_dump_reg_h_fops_2= {
	 		.open		= procfs_dump_reg_h_open_2,
	 		.read		= seq_read,
	 		.llseek		= seq_lseek,
		};

		static ssize_t procfs_dump_spram_h_show_1(struct seq_file *filp, void *offset){ dump_spram_1();return 0;}
		static ssize_t procfs_dump_spram_h_show_2(struct seq_file *filp, void *offset){ dump_spram_2();return 0;}
		static int procfs_dump_spram_h_open_1(struct inode *inode, struct file *filp) { return single_open(filp, procfs_dump_spram_h_show_1, NULL);}
		static int procfs_dump_spram_h_open_2(struct inode *inode, struct file *filp) { return single_open(filp, procfs_dump_spram_h_show_2, NULL);}
		static const struct file_operations procfs_dump_spram_h_fops_1= {
	 		.open		= procfs_dump_spram_h_open_1,
	 		.read		= seq_read,
	 		.llseek		= seq_lseek,
		};
		static const struct file_operations procfs_dump_spram_h_fops_2= {
	 		.open		= procfs_dump_spram_h_open_2,
	 		.read		= seq_read,
	 		.llseek		= seq_lseek,
		};

		static ssize_t procfs_dump_host_state_show_1(struct seq_file *filp, void *offset){ dump_host_state_1();return 0;}
		static ssize_t procfs_dump_host_state_show_2(struct seq_file *filp, void *offset){ dump_host_state_2();return 0;}
		static int procfs_dump_host_state_open_1(struct inode *inode, struct file *filp) { return single_open(filp, procfs_dump_host_state_show_1, NULL);}
		static int procfs_dump_host_state_open_2(struct inode *inode, struct file *filp) { return single_open(filp, procfs_dump_host_state_show_2, NULL);}
		static const struct file_operations procfs_dump_host_state_fops_1= {
	 		.open		= procfs_dump_host_state_open_1,
	 		.read		= seq_read,
	 		.llseek		= seq_lseek,
		};
		static const struct file_operations procfs_dump_host_state_fops_2= {
	 		.open		= procfs_dump_host_state_open_2,
	 		.read		= seq_read,
	 		.llseek		= seq_lseek,
		};
	#endif //__USE_PROCFS__
	#ifdef __USE_SYSFS__
		static ssize_t sysfs_dump_reg_h_show_1( struct device *_dev, struct device_attribute *attr,char *buf) { dump_reg_1();return 0;}
		static ssize_t sysfs_dump_reg_h_show_2( struct device *_dev, struct device_attribute *attr,char *buf) { dump_reg_2();return 0;}
		extern struct device_attribute dev_attr_dump_reg_h_1;
		extern struct device_attribute dev_attr_dump_reg_h_2;
		DEVICE_ATTR(dump_reg_h_1, S_IRUGO, sysfs_dump_reg_h_show_1, NULL);
		DEVICE_ATTR(dump_reg_h_2, S_IRUGO, sysfs_dump_reg_h_show_2, NULL);
		static ssize_t sysfs_dump_spram_h_show_1( struct device *_dev, struct device_attribute *attr,char *buf) { dump_spram_1();return 0;}
		static ssize_t sysfs_dump_spram_h_show_2( struct device *_dev, struct device_attribute *attr,char *buf) { dump_spram_2();return 0;}
		extern struct device_attribute dev_attr_dump_spram_h_1;
		extern struct device_attribute dev_attr_dump_spram_h_2;
		DEVICE_ATTR(dump_spram_h_1, S_IRUGO, sysfs_dump_spram_h_show_1, NULL);
		DEVICE_ATTR(dump_spram_h_2, S_IRUGO, sysfs_dump_spram_h_show_2, NULL);

		static ssize_t sysfs_dump_host_state_show_1( struct device *_dev, struct device_attribute *attr,char *buf) { dump_host_state_1();return 0;}
		static ssize_t sysfs_dump_host_state_show_2( struct device *_dev, struct device_attribute *attr,char *buf) { dump_host_state_2();return 0;}
		extern struct device_attribute dev_attr_dump_host_state_1;
		extern struct device_attribute dev_attr_dump_host_state_2;
		DEVICE_ATTR(dump_host_state_1, S_IRUGO, sysfs_dump_host_state_show_1, NULL);
		DEVICE_ATTR(dump_host_state_2, S_IRUGO, sysfs_dump_host_state_show_2, NULL);
	#endif //__USE_SYSFS__
#endif //__ENABLE_DUMP__
/////////////////////////////////////////////////////////////////////////////////////////////////////
// e/PROCSYSFS:  DUMP
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCSYSFS:  ANA
/////////////////////////////////////////////////////////////////////////////////////////////////////
static uint32_t ana_read(ltqusb_core_if_t *_core_if)
{
	if(ltqusb_chip_family== LTQUSB_CHIP_VR9)
	{
		if(_core_if->core_no==0) return (ltqusb_rreg(((volatile uint32_t *)VR9_RCU_USB_ANA_CFG1A)));
		else                     return (ltqusb_rreg(((volatile uint32_t *)VR9_RCU_USB_ANA_CFG1B)));
	}
	if(ltqusb_chip_family== LTQUSB_CHIP_AR10)
	{
		if(_core_if->core_no==0) return (ltqusb_rreg(((volatile uint32_t *)AR10_RCU_USB_ANA_CFG1A)));
		else                     return (ltqusb_rreg(((volatile uint32_t *)AR10_RCU_USB_ANA_CFG1B)));
	}
	return 0xFFFFFFFF;
}
static void ana_write(ltqusb_core_if_t *_core_if,uint32_t data)
{
	if(ltqusb_chip_family== LTQUSB_CHIP_VR9)
	{
		if(_core_if->core_no==0) ltqusb_wreg(((volatile uint32_t *)VR9_RCU_USB_ANA_CFG1A),data);
		else                     ltqusb_wreg(((volatile uint32_t *)VR9_RCU_USB_ANA_CFG1B),data);
	}
	if(ltqusb_chip_family== LTQUSB_CHIP_AR10)
	{
		if(_core_if->core_no==0) ltqusb_wreg(((volatile uint32_t *)AR10_RCU_USB_ANA_CFG1A),data);
		else                     ltqusb_wreg(((volatile uint32_t *)AR10_RCU_USB_ANA_CFG1B),data);
	}
}
static int ana_read_disconnect_threshold(ltqusb_core_if_t *_core_if)
{
	volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
	rcu_ana_cfg1.d32=ana_read(_core_if);
	return (int)(rcu_ana_cfg1.b.dis_thr);
}
static void ana_write_disconnect_threshold(ltqusb_core_if_t *_core_if,int value)
{
	if( value>=0 && value <= 7 )
	{
		volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
		rcu_ana_cfg1.d32=ana_read(_core_if);
		rcu_ana_cfg1.b.dis_thr=value;
		ana_write(_core_if,rcu_ana_cfg1.d32);
		_core_if->params.ana_disconnect_threshold=(int)value;
	}
}
static int ana_read_squelch_threshold(ltqusb_core_if_t *_core_if)
{
	volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
	rcu_ana_cfg1.d32=ana_read(_core_if);
	return (int)(rcu_ana_cfg1.b.squs_thr);
}
static void ana_write_squelch_threshold(ltqusb_core_if_t *_core_if,int value)
{
	if( value>=0 && value <= 7 )
	{
		volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
		rcu_ana_cfg1.d32=ana_read(_core_if);
		rcu_ana_cfg1.b.squs_thr=value;
		ana_write(_core_if,rcu_ana_cfg1.d32);
		_core_if->params.ana_squelch_threshold=(int)value;
	}
}
static int ana_read_transmitter_crossover(ltqusb_core_if_t *_core_if)
{
	volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
	rcu_ana_cfg1.d32=ana_read(_core_if);
	return (int)(rcu_ana_cfg1.b.txhs_xv);
}
static void ana_write_transmitter_crossover(ltqusb_core_if_t *_core_if,int value)
{
	if( value>=0 && value <= 3 )
	{
		volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
		rcu_ana_cfg1.d32=ana_read(_core_if);
		rcu_ana_cfg1.b.txhs_xv=value;
		ana_write(_core_if,rcu_ana_cfg1.d32);
		_core_if->params.ana_transmitter_crossover=(int)value;
	}
}
static int ana_read_transmitter_impedance(ltqusb_core_if_t *_core_if)
{
	volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
	rcu_ana_cfg1.d32=ana_read(_core_if);
	return (int)(rcu_ana_cfg1.b.txsrci_xv);
}
static void ana_write_transmitter_impedance(ltqusb_core_if_t *_core_if,int value)
{
	if( value>=0 && value <= 15 )
	{
		volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
		rcu_ana_cfg1.d32=ana_read(_core_if);
		rcu_ana_cfg1.b.txsrci_xv=value;
		ana_write(_core_if,rcu_ana_cfg1.d32);
		_core_if->params.ana_transmitter_impedance=(int)value;
	}
}
static int ana_read_transmitter_dc_voltage(ltqusb_core_if_t *_core_if)
{
	volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
	rcu_ana_cfg1.d32=ana_read(_core_if);
	return (int)(rcu_ana_cfg1.b.txhs_dc);
}
static void ana_write_transmitter_dc_voltage(ltqusb_core_if_t *_core_if,int value)
{
	if( value>=0 && value <= 15 )
	{
		volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
		rcu_ana_cfg1.d32=ana_read(_core_if);
		rcu_ana_cfg1.b.txhs_dc=value;
		ana_write(_core_if,rcu_ana_cfg1.d32);
		_core_if->params.ana_transmitter_dc_voltage=(int)value;
	}
}
static int ana_read_transmitter_risefall_time(ltqusb_core_if_t *_core_if)
{
	volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
	rcu_ana_cfg1.d32=ana_read(_core_if);
	return (int)(rcu_ana_cfg1.b.tx_edge);
}
static void ana_write_transmitter_risefall_time(ltqusb_core_if_t *_core_if,int value)
{
	if( value>=0 && value <= 1 )
	{
		volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
		rcu_ana_cfg1.d32=ana_read(_core_if);
		rcu_ana_cfg1.b.tx_edge=value;
		ana_write(_core_if,rcu_ana_cfg1.d32);
		_core_if->params.ana_transmitter_risefall_time=(int)value;
	}
}
static int ana_read_transmitter_pre_emphasis(ltqusb_core_if_t *_core_if)
{
	volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
	rcu_ana_cfg1.d32=ana_read(_core_if);
	return (int)(rcu_ana_cfg1.b.tx_pee);
}
static void ana_write_transmitter_pre_emphasis(ltqusb_core_if_t *_core_if,int value)
{
	if( value>=0 && value <= 1 )
	{
		volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
		rcu_ana_cfg1.d32=ana_read(_core_if);
		rcu_ana_cfg1.b.tx_pee=value;
		ana_write(_core_if,rcu_ana_cfg1.d32);
		_core_if->params.ana_transmitter_pre_emphasis=(int)value;
	}
}
#ifdef __USE_PROCFS__
	static ssize_t procfs_ana_disconnect_threshold_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 1 Disconnect Threshold %d\n",ana_read_disconnect_threshold(&ltqusb_hcd_1.core_if));
	}
	static ssize_t procfs_ana_disconnect_threshold_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 2 Disconnect Threshold %d\n",ana_read_disconnect_threshold(&ltqusb_hcd_2.core_if));
	}
	static ssize_t procfs_ana_disconnect_threshold_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_disconnect_threshold(&ltqusb_hcd_1.core_if,value);
		return length;
	}
	static ssize_t procfs_ana_disconnect_threshold_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_disconnect_threshold(&ltqusb_hcd_2.core_if,value);
		return length;
	}
	static const struct file_operations procfs_ana_disconnect_threshold_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_disconnect_threshold_1_show,
		.write 	= procfs_ana_disconnect_threshold_1_store,
	};
	static const struct file_operations procfs_ana_disconnect_threshold_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_disconnect_threshold_2_show,
		.write 	= procfs_ana_disconnect_threshold_2_store,
	};

	static ssize_t procfs_ana_squelch_threshold_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 1 Squelch Threshold %d\n",ana_read_squelch_threshold(&ltqusb_hcd_1.core_if));
	}
	static ssize_t procfs_ana_squelch_threshold_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 2 Squelch Threshold %d\n",ana_read_squelch_threshold(&ltqusb_hcd_2.core_if));
	}
	static ssize_t procfs_ana_squelch_threshold_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_squelch_threshold(&ltqusb_hcd_1.core_if,value);
		return length;
	}
	static ssize_t procfs_ana_squelch_threshold_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
				return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_squelch_threshold(&ltqusb_hcd_2.core_if,value);
		return length;
	}
	static const struct file_operations procfs_ana_squelch_threshold_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_squelch_threshold_1_show,
		.write 	= procfs_ana_squelch_threshold_1_store,
	};
	static const struct file_operations procfs_ana_squelch_threshold_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_squelch_threshold_2_show,
		.write 	= procfs_ana_squelch_threshold_2_store,
	};

	static ssize_t procfs_ana_transmitter_crossover_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 1 Transmitter High Speed Crossover %d\n",ana_read_transmitter_crossover(&ltqusb_hcd_1.core_if));
	}
	static ssize_t procfs_ana_transmitter_crossover_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 2 Transmitter High Speed Crossover %d\n",ana_read_transmitter_crossover(&ltqusb_hcd_2.core_if));
	}
	static ssize_t procfs_ana_transmitter_crossover_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
				return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_transmitter_crossover(&ltqusb_hcd_1.core_if,value);
		return length;
	}
	static ssize_t procfs_ana_transmitter_crossover_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_transmitter_crossover(&ltqusb_hcd_2.core_if,value);
		return length;
	}
	static const struct file_operations procfs_ana_transmitter_crossover_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_transmitter_crossover_1_show,
		.write 	= procfs_ana_transmitter_crossover_1_store,
	};
	static const struct file_operations procfs_ana_transmitter_crossover_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_transmitter_crossover_2_show,
		.write 	= procfs_ana_transmitter_crossover_2_store,
	};

	static ssize_t procfs_ana_transmitter_impedance_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 1 Transmitter Source Impedance %d\n",ana_read_transmitter_impedance(&ltqusb_hcd_1.core_if));
	}
	static ssize_t procfs_ana_transmitter_impedance_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 2 Transmitter Source Impedance %d\n",ana_read_transmitter_impedance(&ltqusb_hcd_2.core_if));
	}
	static ssize_t procfs_ana_transmitter_impedance_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_transmitter_impedance(&ltqusb_hcd_1.core_if,value);
		return length;
	}
	static ssize_t procfs_ana_transmitter_impedance_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
				return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_transmitter_impedance(&ltqusb_hcd_2.core_if,value);
		return length;
	}
	static const struct file_operations procfs_ana_transmitter_impedance_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_transmitter_impedance_1_show,
		.write 	= procfs_ana_transmitter_impedance_1_store,
	};
	static const struct file_operations procfs_ana_transmitter_impedance_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_transmitter_impedance_2_show,
		.write 	= procfs_ana_transmitter_impedance_2_store,
	};

	static ssize_t procfs_ana_transmitter_dc_voltage_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 1 Transmitter HS DC Voltage Level %d\n",ana_read_transmitter_dc_voltage(&ltqusb_hcd_1.core_if));
	}
	static ssize_t procfs_ana_transmitter_dc_voltage_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 2 Transmitter HS DC Voltage Level %d\n",ana_read_transmitter_dc_voltage(&ltqusb_hcd_2.core_if));
	}
	static ssize_t procfs_ana_transmitter_dc_voltage_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_transmitter_dc_voltage(&ltqusb_hcd_1.core_if,value);
		return length;
	}
	static ssize_t procfs_ana_transmitter_dc_voltage_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_transmitter_dc_voltage(&ltqusb_hcd_2.core_if,value);
		return length;
	}
	static const struct file_operations procfs_ana_transmitter_dc_voltage_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_transmitter_dc_voltage_1_show,
		.write 	= procfs_ana_transmitter_dc_voltage_1_store,
	};
	static const struct file_operations procfs_ana_transmitter_dc_voltage_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_transmitter_dc_voltage_2_show,
		.write 	= procfs_ana_transmitter_dc_voltage_2_store,
	};
	static ssize_t procfs_ana_transmitter_risefall_time_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 1 Transmitter HS Rise/Fall Time %d\n",ana_read_transmitter_risefall_time(&ltqusb_hcd_1.core_if));
	}
	static ssize_t procfs_ana_transmitter_risefall_time_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 2 Transmitter HS Rise/Fall Time %d\n",ana_read_transmitter_risefall_time(&ltqusb_hcd_2.core_if));
	}
	static ssize_t procfs_ana_transmitter_risefall_time_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_transmitter_risefall_time(&ltqusb_hcd_1.core_if,value);
		return length;
	}
	static ssize_t procfs_ana_transmitter_risefall_time_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_transmitter_risefall_time(&ltqusb_hcd_2.core_if,value);
		return length;
	}
	static const struct file_operations procfs_ana_transmitter_risefall_time_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_transmitter_risefall_time_1_show,
		.write 	= procfs_ana_transmitter_risefall_time_1_store,
	};
	static const struct file_operations procfs_ana_transmitter_risefall_time_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_transmitter_risefall_time_2_show,
		.write 	= procfs_ana_transmitter_risefall_time_2_store,
	};

	static ssize_t procfs_ana_transmitter_pre_emphasis_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 1 Transmitter HS Pre-Emphasis %d\n",ana_read_transmitter_pre_emphasis(&ltqusb_hcd_1.core_if));
	}
	static ssize_t procfs_ana_transmitter_pre_emphasis_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 2 Transmitter HS Pre-Emphasis %d\n",ana_read_transmitter_pre_emphasis(&ltqusb_hcd_2.core_if));
	}
	static ssize_t procfs_ana_transmitter_pre_emphasis_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_transmitter_pre_emphasis(&ltqusb_hcd_1.core_if,value);
		return length;
	}
	static ssize_t procfs_ana_transmitter_pre_emphasis_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int value;
		if (copy_from_user(buffer, buf, 7))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		ana_write_transmitter_pre_emphasis(&ltqusb_hcd_2.core_if,value);
		return length;
	}
	static const struct file_operations procfs_ana_transmitter_pre_emphasis_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_transmitter_pre_emphasis_1_show,
		.write 	= procfs_ana_transmitter_pre_emphasis_1_store,
	};
	static const struct file_operations procfs_ana_transmitter_pre_emphasis_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_ana_transmitter_pre_emphasis_2_show,
		.write 	= procfs_ana_transmitter_pre_emphasis_2_store,
	};

#endif //__USE_PROCFS__
#ifdef __USE_SYSFS__
	static ssize_t sysfs_ana_disconnect_threshold_1_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 1 Disconnect Threshold %d\n",ana_read_disconnect_threshold(&ltqusb_hcd_1.core_if));
	}
	static ssize_t sysfs_ana_disconnect_threshold_2_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 2 Disconnect Threshold %d\n",ana_read_disconnect_threshold(&ltqusb_hcd_2.core_if));
	}
	static ssize_t sysfs_ana_disconnect_threshold_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_disconnect_threshold(&ltqusb_hcd_1.core_if,value);
		return count;
	}
	static ssize_t sysfs_ana_disconnect_threshold_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_disconnect_threshold(&ltqusb_hcd_2.core_if,value);
		return count;
	}
	extern struct device_attribute dev_attr_ana_disconnect_threshold_1;
	extern struct device_attribute dev_attr_ana_disconnect_threshold_2;
	DEVICE_ATTR(ana_disconnect_threshold_1, S_IRUGO|S_IWUSR, sysfs_ana_disconnect_threshold_1_show, sysfs_ana_disconnect_threshold_1_store);
	DEVICE_ATTR(ana_disconnect_threshold_2, S_IRUGO|S_IWUSR, sysfs_ana_disconnect_threshold_2_show, sysfs_ana_disconnect_threshold_2_store);

	static ssize_t sysfs_ana_squelch_threshold_1_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 1 Squelch Threshold %d\n",ana_read_squelch_threshold(&ltqusb_hcd_1.core_if));
	}
	static ssize_t sysfs_ana_squelch_threshold_2_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 2 Squelch Threshold %d\n",ana_read_squelch_threshold(&ltqusb_hcd_2.core_if));
	}
	static ssize_t sysfs_ana_squelch_threshold_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
		return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_squelch_threshold(&ltqusb_hcd_1.core_if,value);
		return count;
	}
	static ssize_t sysfs_ana_squelch_threshold_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_squelch_threshold(&ltqusb_hcd_2.core_if,value);
		return count;
	}
	extern struct device_attribute dev_attr_ana_squelch_threshold_1;
	extern struct device_attribute dev_attr_ana_squelch_threshold_2;
	DEVICE_ATTR(ana_squelch_threshold_1, S_IRUGO|S_IWUSR, sysfs_ana_squelch_threshold_1_show, sysfs_ana_squelch_threshold_1_store);
	DEVICE_ATTR(ana_squelch_threshold_2, S_IRUGO|S_IWUSR, sysfs_ana_squelch_threshold_2_show, sysfs_ana_squelch_threshold_2_store);

	static ssize_t sysfs_ana_transmitter_crossover_1_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 1 Transmitter High Speed Crossover %d\n",ana_read_transmitter_crossover(&ltqusb_hcd_1.core_if));
	}
	static ssize_t sysfs_ana_transmitter_crossover_2_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 2 Transmitter High Speed Crossover %d\n",ana_read_transmitter_crossover(&ltqusb_hcd_2.core_if));
	}
	static ssize_t sysfs_ana_transmitter_crossover_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_transmitter_crossover(&ltqusb_hcd_1.core_if,value);
		return count;
	}
	static ssize_t sysfs_ana_transmitter_crossover_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_transmitter_crossover(&ltqusb_hcd_2.core_if,value);
		return count;
	}
	extern struct device_attribute dev_attr_ana_transmitter_crossover_1;
	extern struct device_attribute dev_attr_ana_transmitter_crossover_2;
	DEVICE_ATTR(ana_transmitter_crossover_1, S_IRUGO|S_IWUSR, sysfs_ana_transmitter_crossover_1_show, sysfs_ana_transmitter_crossover_1_store);
	DEVICE_ATTR(ana_transmitter_crossover_2, S_IRUGO|S_IWUSR, sysfs_ana_transmitter_crossover_2_show, sysfs_ana_transmitter_crossover_2_store);

	static ssize_t sysfs_ana_transmitter_impedance_1_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 1 Transmitter Source Impedance %d\n",ana_read_transmitter_impedance(&ltqusb_hcd_1.core_if));
	}
	static ssize_t sysfs_ana_transmitter_impedance_2_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 2 Transmitter Source Impedance %d\n",ana_read_transmitter_impedance(&ltqusb_hcd_2.core_if));
	}
	static ssize_t sysfs_ana_transmitter_impedance_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_transmitter_impedance(&ltqusb_hcd_1.core_if,value);
		return count;
	}
	static ssize_t sysfs_ana_transmitter_impedance_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_transmitter_impedance(&ltqusb_hcd_2.core_if,value);
		return count;
	}
	extern struct device_attribute dev_attr_ana_transmitter_impedance_1;
	extern struct device_attribute dev_attr_ana_transmitter_impedance_2;
	DEVICE_ATTR(ana_transmitter_impedance_1, S_IRUGO|S_IWUSR, sysfs_ana_transmitter_impedance_1_show, sysfs_ana_transmitter_impedance_1_store);
	DEVICE_ATTR(ana_transmitter_impedance_2, S_IRUGO|S_IWUSR, sysfs_ana_transmitter_impedance_2_show, sysfs_ana_transmitter_impedance_2_store);

	static ssize_t sysfs_ana_transmitter_dc_voltage_1_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 1 Transmitter HS DC Voltage Level %d\n",ana_read_transmitter_dc_voltage(&ltqusb_hcd_1.core_if));
	}
	static ssize_t sysfs_ana_transmitter_dc_voltage_2_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 2 Transmitter HS DC Voltage Level %d\n",ana_read_transmitter_dc_voltage(&ltqusb_hcd_2.core_if));
	}
	static ssize_t sysfs_ana_transmitter_dc_voltage_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_transmitter_dc_voltage(&ltqusb_hcd_1.core_if,value);
		return count;
	}
	static ssize_t sysfs_ana_transmitter_dc_voltage_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_transmitter_dc_voltage(&ltqusb_hcd_2.core_if,value);
		return count;
	}
	extern struct device_attribute dev_attr_ana_transmitter_dc_voltage_1;
	extern struct device_attribute dev_attr_ana_transmitter_dc_voltage_2;
	DEVICE_ATTR(ana_transmitter_dc_voltage_1, S_IRUGO|S_IWUSR, sysfs_ana_transmitter_dc_voltage_1_show, sysfs_ana_transmitter_dc_voltage_1_store);
	DEVICE_ATTR(ana_transmitter_dc_voltage_2, S_IRUGO|S_IWUSR, sysfs_ana_transmitter_dc_voltage_2_show, sysfs_ana_transmitter_dc_voltage_2_store);

	static ssize_t sysfs_ana_transmitter_risefall_time_1_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 1 Transmitter HS Rise/Fall Time %d\n",ana_read_transmitter_risefall_time(&ltqusb_hcd_1.core_if));
	}
	static ssize_t sysfs_ana_transmitter_risefall_time_2_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 2 Transmitter HS Rise/Fall Time %d\n",ana_read_transmitter_risefall_time(&ltqusb_hcd_2.core_if));
	}
	static ssize_t sysfs_ana_transmitter_risefall_time_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_transmitter_risefall_time(&ltqusb_hcd_1.core_if,value);
		return count;
	}
	static ssize_t sysfs_ana_transmitter_risefall_time_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_transmitter_risefall_time(&ltqusb_hcd_2.core_if,value);
		return count;
	}
	extern struct device_attribute dev_attr_ana_transmitter_risefall_time_1;
	extern struct device_attribute dev_attr_ana_transmitter_risefall_time_2;
	DEVICE_ATTR(ana_transmitter_risefall_time_1, S_IRUGO|S_IWUSR, sysfs_ana_transmitter_risefall_time_1_show, sysfs_ana_transmitter_risefall_time_1_store);
	DEVICE_ATTR(ana_transmitter_risefall_time_2, S_IRUGO|S_IWUSR, sysfs_ana_transmitter_risefall_time_2_show, sysfs_ana_transmitter_risefall_time_2_store);

	static ssize_t sysfs_ana_transmitter_pre_emphasis_1_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 1 Transmitter HS Pre-Emphasis %d\n",ana_read_transmitter_pre_emphasis(&ltqusb_hcd_1.core_if));
	}
	static ssize_t sysfs_ana_transmitter_pre_emphasis_2_show( struct device *_dev, struct device_attribute *attr,char *buf){
		return sprintf (buf,"Host 2 Transmitter HS Pre-Emphasis %d\n",ana_read_transmitter_pre_emphasis(&ltqusb_hcd_2.core_if));
	}
	static ssize_t sysfs_ana_transmitter_pre_emphasis_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_transmitter_pre_emphasis(&ltqusb_hcd_1.core_if,value);
		return count;
	}
	static ssize_t sysfs_ana_transmitter_pre_emphasis_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int value;
		if (copy_from_user(buf, buffer, 7))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		ana_write_transmitter_pre_emphasis(&ltqusb_hcd_2.core_if,value);
		return count;
	}
	extern struct device_attribute dev_attr_ana_transmitter_pre_emphasis_1;
	extern struct device_attribute dev_attr_ana_transmitter_pre_emphasis_2;
	DEVICE_ATTR(ana_transmitter_pre_emphasis_1, S_IRUGO|S_IWUSR, sysfs_ana_transmitter_pre_emphasis_1_show, sysfs_ana_transmitter_pre_emphasis_1_store);
	DEVICE_ATTR(ana_transmitter_pre_emphasis_2, S_IRUGO|S_IWUSR, sysfs_ana_transmitter_pre_emphasis_2_show, sysfs_ana_transmitter_pre_emphasis_2_store);
#endif //__USE_SYSFS__
/////////////////////////////////////////////////////////////////////////////////////////////////////
// e/PROCSYSFS:  ANA
/////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCSYSFS:  HOST CTRL
/////////////////////////////////////////////////////////////////////////////////////////////////////
static uint32_t buspower_show(ltqhcd_hcd_t *_ltqhcd,char *buf)
{
	if(ltqusb_vbus (_ltqhcd)==1) return sprintf( buf, "1\n" );
	if(ltqusb_vbus (_ltqhcd)==0) return sprintf( buf, "0\n" );
	return sprintf( buf, "unknown\n" );
}
static void buspower_store(ltqhcd_hcd_t *_ltqhcd,uint32_t value)
{
	if     (value==1)  ltqusb_vbus_on (_ltqhcd);
	else if(value==0)  ltqusb_vbus_off(_ltqhcd);
}

static ssize_t protect1_show(ltqhcd_hcd_t *_ltqhcd, char *buf)
{
	return sprintf( buf, "%d %04x %d\n",_ltqhcd->xfer_prot1_usec,_ltqhcd->xfer_prot1_value,_ltqhcd->xfer_prot1_value);
}

static void protect1_store(ltqhcd_hcd_t *_ltqhcd, uint32_t value)
{
	if(value==0)
	{
		_ltqhcd->xfer_prot1_usec =0;
		_ltqhcd->xfer_prot1_value= 0;
	}
	else if(value<120)
	{
		_ltqhcd->xfer_prot1_usec=value;
		_ltqhcd->xfer_prot1_value= XFER_PROT1_CALC(_ltqhcd->xfer_prot1_usec);
	}
}
static ssize_t protect2_show(ltqhcd_hcd_t *_ltqhcd, char *buf)
{
	return sprintf( buf, "%d %lu\n",_ltqhcd->xfer_prot2_usec,_ltqhcd->xfer_prot2_value);
}
static void protect2_store(ltqhcd_hcd_t *_ltqhcd, uint32_t value)
{
	if(value==0)
	{
		_ltqhcd->xfer_prot2_usec =0;
		_ltqhcd->xfer_prot2_value= 0;
	}
	else if(value<120)
	{
		_ltqhcd->xfer_prot2_usec=value;
		_ltqhcd->xfer_prot2_value= XFER_PROT2_CALC(_ltqhcd->xfer_prot2_usec);
	}
}
static ssize_t singlexact_show(ltqhcd_hcd_t *_ltqhcd, char *buf)
{
	return sprintf( buf, "%d\n",_ltqhcd->singlexact);
}
static void singlexact_store(ltqhcd_hcd_t *_ltqhcd, uint32_t value)
{
	if(value==1) _ltqhcd->singlexact=1;
	if(value==0) _ltqhcd->singlexact=0;
}
static ssize_t burst_show(ltqhcd_hcd_t *_ltqhcd, char *buf)
{
	return sprintf( buf, "%d\n",_ltqhcd->burst);
}
static void burst_store(ltqhcd_hcd_t *_ltqhcd, uint32_t value)
{
	if(value==1) _ltqhcd->burst=1;
	if(value==0) _ltqhcd->burst=0;
}

#ifdef __USE_PROCFS__
	static ssize_t procfs_buspower_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return buspower_show(&ltqusb_hcd_1,buf);
	}
	static ssize_t procfs_buspower_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return buspower_show(&ltqusb_hcd_2,buf);
	}
	static ssize_t procfs_protect1_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return protect1_show(&ltqusb_hcd_1,buf);
	}
	static ssize_t procfs_protect1_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return protect1_show(&ltqusb_hcd_2,buf);
	}
	static ssize_t procfs_protect2_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return protect2_show(&ltqusb_hcd_1,buf);
	}
	static ssize_t procfs_protect2_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return protect2_show(&ltqusb_hcd_2,buf);
	}
	static ssize_t procfs_singlexact_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return singlexact_show(&ltqusb_hcd_1,buf);
	}
	static ssize_t procfs_singlexact_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return singlexact_show(&ltqusb_hcd_2,buf);
	}

	static ssize_t procfs_burst_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return burst_show(&ltqusb_hcd_1,buf);
	}
	static ssize_t procfs_burst_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return burst_show(&ltqusb_hcd_2,buf);
	}

	static ssize_t procfs_buspower_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		buspower_store(&ltqusb_hcd_1,value);
		return length;
	}
	static ssize_t procfs_buspower_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		buspower_store(&ltqusb_hcd_2,value);
		return length;
	}
	static ssize_t procfs_protect1_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		protect1_store(&ltqusb_hcd_1,value);
		return length;
	}
	static ssize_t procfs_protect1_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		protect1_store(&ltqusb_hcd_2,value);
		return length;
	}
	static ssize_t procfs_protect2_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		protect2_store(&ltqusb_hcd_1,value);
		return length;
	}
	static ssize_t procfs_protect2_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		protect2_store(&ltqusb_hcd_2,value);
		return length;
	}
	static ssize_t procfs_singlexact_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		singlexact_store(&ltqusb_hcd_1,value);
		return length;
	}
	static ssize_t procfs_singlexact_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		singlexact_store(&ltqusb_hcd_2,value);
		return length;
	}
	static ssize_t procfs_burst_1_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		burst_store(&ltqusb_hcd_1,value);
		return length;
	}
	static ssize_t procfs_burst_2_store(struct file *filp, const char *buf, size_t length, loff_t *offset)
	{
		char buffer[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buffer, &buf[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buffer, NULL, 10);
		burst_store(&ltqusb_hcd_2,value);
		return length;
	}

	static const struct file_operations procfs_buspower_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_buspower_1_show,
		.write 	= procfs_buspower_1_store,
	};
	static const struct file_operations procfs_buspower_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_buspower_2_show,
		.write 	= procfs_buspower_2_store,
	};
	static const struct file_operations procfs_protect1_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_protect1_1_show,
		.write 	= procfs_protect1_1_store,
	};
	static const struct file_operations procfs_protect1_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_protect1_2_show,
		.write 	= procfs_protect1_2_store,
	};
	static const struct file_operations procfs_protect2_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_protect2_1_show,
		.write 	= procfs_protect2_1_store,
	};
	static const struct file_operations procfs_protect2_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_protect2_2_show,
		.write 	= procfs_protect2_2_store,
	};
	static const struct file_operations procfs_singlexact_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_singlexact_1_show,
		.write 	= procfs_singlexact_1_store,
	};
	static const struct file_operations procfs_singlexact_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_singlexact_2_show,
		.write 	= procfs_singlexact_2_store,
	};
	static const struct file_operations procfs_burst_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_burst_1_show,
		.write 	= procfs_burst_1_store,
	};
	static const struct file_operations procfs_burst_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_burst_2_show,
		.write 	= procfs_burst_2_store,
	};
#endif //__USE_PROCFS__
#ifdef __USE_SYSFS__
	static ssize_t sysfs_buspower_1_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return buspower_show(&ltqusb_hcd_1,buf);
	}
	static ssize_t sysfs_buspower_2_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return buspower_show(&ltqusb_hcd_2,buf);
	}
	static ssize_t sysfs_protect1_1_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return protect1_show(&ltqusb_hcd_1,buf);
	}
	static ssize_t sysfs_protect1_2_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return protect1_show(&ltqusb_hcd_2,buf);
	}
	static ssize_t sysfs_protect2_1_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return protect2_show(&ltqusb_hcd_1,buf);
	}
	static ssize_t sysfs_protect2_2_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return protect2_show(&ltqusb_hcd_2,buf);
	}
	static ssize_t sysfs_singlexact_1_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return singlexact_show(&ltqusb_hcd_1,buf);
	}
	static ssize_t sysfs_singlexact_2_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return singlexact_show(&ltqusb_hcd_2,buf);
	}
	static ssize_t sysfs_burst_1_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return burst_show(&ltqusb_hcd_1,buf);
	}
	static ssize_t sysfs_burst_2_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return burst_show(&ltqusb_hcd_2,buf);
	}

	static ssize_t sysfs_buspower_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		buspower_store(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t sysfs_buspower_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		buspower_store(&ltqusb_hcd_2,value);
		return count;
	}
	static ssize_t sysfs_protect1_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		protect1_store(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t sysfs_protect1_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		protect1_store(&ltqusb_hcd_2,value);
		return count;
	}
	static ssize_t sysfs_protect2_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		protect2_store(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t sysfs_protect2_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		protect2_store(&ltqusb_hcd_2,value);
		return count;
	}
	static ssize_t sysfs_singlexact_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		singlexact_store(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t sysfs_singlexact_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		singlexact_store(&ltqusb_hcd_2,value);
		return count;
	}
	static ssize_t sysfs_burst_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		burst_store(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t sysfs_burst_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		burst_store(&ltqusb_hcd_2,value);
		return count;
	}
	extern struct device_attribute dev_attr_buspower_1;
	DEVICE_ATTR(buspower_1, S_IRUGO|S_IWUSR, sysfs_buspower_1_show, sysfs_buspower_1_store);
	extern struct device_attribute dev_attr_buspower_2;
	DEVICE_ATTR(buspower_2, S_IRUGO|S_IWUSR, sysfs_buspower_2_show, sysfs_buspower_2_store);
	extern struct device_attribute dev_attr_protect1_1;
	DEVICE_ATTR(protect1_1, S_IRUGO|S_IWUSR, sysfs_protect1_1_show, sysfs_protect1_1_store);
	extern struct device_attribute dev_attr_protect1_2;
	DEVICE_ATTR(protect1_2, S_IRUGO|S_IWUSR, sysfs_protect1_2_show, sysfs_protect1_2_store);
	extern struct device_attribute dev_attr_protect2_1;
	DEVICE_ATTR(protect2_1, S_IRUGO|S_IWUSR, sysfs_protect2_1_show, sysfs_protect2_1_store);
	extern struct device_attribute dev_attr_protect2_2;
	DEVICE_ATTR(protect2_2, S_IRUGO|S_IWUSR, sysfs_protect2_2_show, sysfs_protect2_2_store);
	extern struct device_attribute dev_attr_singlexact_1;
	DEVICE_ATTR(singlexact_1, S_IRUGO|S_IWUSR, sysfs_singlexact_1_show, sysfs_singlexact_1_store);
	extern struct device_attribute dev_attr_singlexact_2;
	DEVICE_ATTR(singlexact_2, S_IRUGO|S_IWUSR, sysfs_singlexact_2_show, sysfs_singlexact_2_store);
	extern struct device_attribute dev_attr_burst_1;
	DEVICE_ATTR(burst_1, S_IRUGO|S_IWUSR, sysfs_burst_1_show, sysfs_burst_1_store);
	extern struct device_attribute dev_attr_burst_2;
	DEVICE_ATTR(burst_2, S_IRUGO|S_IWUSR, sysfs_burst_2_show, sysfs_burst_2_store);
#endif //__USE_SYSFS__
/////////////////////////////////////////////////////////////////////////////////////////////////////
// e/PROCSYSFS:  HOST CTRL
/////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////
// PROCSYSFS:  HOST POWER CONTROL
/////////////////////////////////////////////////////////////////////////////////////////////////////

static void host_probing(unsigned long _ptr)
{
	ltqhcd_hcd_t *ltqhcd = (ltqhcd_hcd_t *)_ptr;

	if(ltqhcd->flags.b.port_connect_status)
	{
		del_timer(&ltqhcd->host_probe_timer);
		del_timer(&ltqhcd->autoprobe_timer);
		ltqhcd->power_status = 0;
	}
	else
	{
		del_timer(&ltqhcd->autoprobe_timer);
		ltqhcd->autoprobe_timer.expires = jiffies + (HZ*ltqhcd->autoprobe_sec);
		add_timer(&ltqhcd->autoprobe_timer);
		ltqhcd->power_status = 2;
		del_timer(&ltqhcd->host_probe_timer);
		do_suspend_h(ltqhcd);
	}
}

static void host_autoprobe(unsigned long _ptr)
{
	ltqhcd_hcd_t *ltqhcd = (ltqhcd_hcd_t *)_ptr;
	del_timer(&ltqhcd->host_probe_timer);
	ltqhcd->host_probe_timer.function = host_probing;
	ltqhcd->host_probe_timer.expires = jiffies + (HZ*ltqhcd->probe_sec);
	ltqhcd->host_probe_timer.data = (unsigned long)ltqhcd;
	add_timer(&ltqhcd->host_probe_timer);
	do_resume_h(ltqhcd);
}

static void host_single_probe(ltqhcd_hcd_t *ltqhcd, uint32_t value)
{
	if(ltqhcd->power_status == 1)
		host_autoprobe((unsigned long)ltqhcd);
}


static void suspend_host_write(ltqhcd_hcd_t *ltqhcd , uint32_t value)
{
	if(value==2)
	{
		del_timer(&ltqhcd->autoprobe_timer);
		ltqhcd->autoprobe_timer.function = host_autoprobe;
		ltqhcd->autoprobe_timer.expires = jiffies + (HZ*ltqhcd->autoprobe_sec);
		ltqhcd->autoprobe_timer.data = (unsigned long)ltqhcd;
		add_timer(&ltqhcd->autoprobe_timer);
		ltqhcd->power_status = 2;
	}
	else if(value==1)
	{
		do_suspend_h(ltqhcd);
		ltqhcd->power_status = 1;
		del_timer(&ltqhcd->host_probe_timer);
		del_timer(&ltqhcd->autoprobe_timer);
	}
	else if(value==0)
	{
		do_resume_h(ltqhcd);
		ltqhcd->power_status = 0;
		del_timer(&ltqhcd->host_probe_timer);
		del_timer(&ltqhcd->autoprobe_timer);
	}
}
static ssize_t suspend_host_read(ltqhcd_hcd_t *ltqhcd , char *buf)
{
	if(ltqhcd->power_status == 0)
		return sprintf (buf,"Host %d power status is ON\n",ltqhcd->core_if.core_no+1);
	else if(ltqhcd->power_status == 1)
		return sprintf (buf,"Host %d power status is Suspend\n",ltqhcd->core_if.core_no+1);
	else
		return sprintf (buf,"Host %d power status is Auto-probing\n",ltqhcd->core_if.core_no+1);
}

static void bussuspend_write(ltqhcd_hcd_t *ltqhcd , uint32_t value)
{
	if(value==1)
		do_bussuspend_h(ltqhcd);
	else if(value==0)
		do_busresume_h(ltqhcd);
}
static ssize_t bussuspend_read(ltqhcd_hcd_t *ltqhcd , char *buf)
{
	hprt0_data_t val;
	val.d32 = ltqusb_rreg(ltqhcd->core_if.hprt0);
	return sprintf (buf,"Suspended = 0x%x\n", val.b.prtsusp);
}

static void host_probe_timer_write(ltqhcd_hcd_t *ltqhcd , uint32_t value)
{
	if(value>0 && value<=10) ltqhcd->probe_sec=value;
}
static void host_autoprobe_timer_write(ltqhcd_hcd_t *ltqhcd , uint32_t value)
{
	if(value>0 && value<=300) ltqhcd->autoprobe_sec=value;
}
static ssize_t host_probe_timer_read(ltqhcd_hcd_t *ltqhcd , char *buf)
{
	return sprintf (buf,"Host %d probing timer is %d seconds long , probe every %d seconds\n",ltqhcd->core_if.core_no+1,ltqhcd->probe_sec,ltqhcd->autoprobe_sec);
}

#ifdef __USE_PROCFS__
	static ssize_t procfs_host_suspend_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return suspend_host_read(&ltqusb_hcd_1,buf);
	}
	static ssize_t procfs_host_suspend_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return suspend_host_read(&ltqusb_hcd_2,buf);
	}
	static ssize_t procfs_host_suspend_1_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		suspend_host_write(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t procfs_host_suspend_2_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		suspend_host_write(&ltqusb_hcd_2,value);
		return count;
	}
	static const struct file_operations procfs_host_suspend_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_host_suspend_1_show,
		.write 	= procfs_host_suspend_1_store,
	};
	static const struct file_operations procfs_host_suspend_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_host_suspend_2_show,
		.write 	= procfs_host_suspend_2_store,
	};

	static ssize_t procfs_bussuspend_1_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return bussuspend_read(&ltqusb_hcd_1,buf);
	}
	static ssize_t procfs_bussuspend_2_show(struct file *filp, char *buf, size_t length, loff_t *offset){
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return bussuspend_read(&ltqusb_hcd_2,buf);
	}
	static ssize_t procfs_bussuspend_1_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		bussuspend_write(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t procfs_bussuspend_2_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		bussuspend_write(&ltqusb_hcd_2,value);
		return count;
	}
	static const struct file_operations procfs_bussuspend_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_bussuspend_1_show,
		.write 	= procfs_bussuspend_1_store,
	};
	static const struct file_operations procfs_bussuspend_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_bussuspend_2_show,
		.write 	= procfs_bussuspend_2_store,
	};


	static ssize_t procfs_probe_host_1_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_single_probe(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t procfs_probe_host_2_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_single_probe(&ltqusb_hcd_2,value);
		return count;
	}
	static const struct file_operations procfs_probe_host_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_host_suspend_1_show,
		.write 	= procfs_probe_host_1_store,
	};
	static const struct file_operations procfs_probe_host_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_host_suspend_2_show,
		.write 	= procfs_probe_host_2_store,
	};

	static ssize_t procfs_host_probe_timer_1_show(struct file *filp, char *buf, size_t length, loff_t *offset)
	{
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return host_probe_timer_read(&ltqusb_hcd_1,buf);
	}
	static ssize_t procfs_host_probe_timer_2_show(struct file *filp, char *buf, size_t length, loff_t *offset)
	{
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
			return host_probe_timer_read(&ltqusb_hcd_2,buf);
	}

	static ssize_t procfs_host_probe_timer_1_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_probe_timer_write(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t procfs_host_probe_timer_2_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_probe_timer_write(&ltqusb_hcd_2,value);
		return count;
	}
	static const struct file_operations procfs_host_probe_timer_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_host_probe_timer_1_show,
		.write 	= procfs_host_probe_timer_1_store,
	};
	static const struct file_operations procfs_host_probe_timer_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_host_probe_timer_2_show,
		.write 	= procfs_host_probe_timer_2_store,
	};

	static ssize_t procfs_host_autoprobe_timer_1_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_autoprobe_timer_write(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t procfs_host_autoprobe_timer_2_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_autoprobe_timer_write(&ltqusb_hcd_2,value);
		return count;
	}
	static const struct file_operations procfs_host_autoprobe_timer_1_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_host_probe_timer_1_show,
		.write 	= procfs_host_autoprobe_timer_1_store,
	};
	static const struct file_operations procfs_host_autoprobe_timer_2_fops = {
	 	.open	= procfs_rw_open,
		.read 	= procfs_host_probe_timer_2_show,
		.write 	= procfs_host_autoprobe_timer_2_store,
	};

	static ssize_t procfs_oc_hy_1_show(struct file *filp, char *buf, size_t length, loff_t *offset)
	{
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 1 Over-Current Hysteresis is %d\n",(int)(ltqusb_oc_get_hy(1)));
	}
	static ssize_t procfs_oc_hy_2_show(struct file *filp, char *buf, size_t length, loff_t *offset)
	{
		static int done=0;
		if(done)
		{
			done=0;
			return 0;
		}
		done=1;
		return sprintf (buf,"Host 2 Over-Current Hysteresis is %d\n",(int)(ltqusb_oc_get_hy(1)));
	}
	static ssize_t procfs_oc_hy_1_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		if( value>=0 && value < 4 )
		ltqusb_oc_set_hy(1,value);
		return count;
	}
	static ssize_t procfs_oc_hy_2_store(struct file *filp, const char *buffer, size_t count, loff_t *offset)
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		if( value>=0 && value < 4 )
			ltqusb_oc_set_hy(2,value);
		return count;
	}
	static const struct file_operations procfs_oc_hy_1_fops = {
		.open	= procfs_rw_open,
		.read 	= procfs_oc_hy_1_show,
		.write 	= procfs_oc_hy_1_store,
	};
	static const struct file_operations procfs_oc_hy_2_fops = {
		.open	= procfs_rw_open,
		.read 	= procfs_oc_hy_2_show,
		.write 	= procfs_oc_hy_2_store,
	};

#endif //__USE_PROCFS__
#ifdef __USE_SYSFS__
	static ssize_t sysfs_host_suspend_1_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return suspend_host_read(&ltqusb_hcd_1,buf);
	}
	static ssize_t sysfs_host_suspend_2_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return suspend_host_read(&ltqusb_hcd_2,buf);
	}
	static ssize_t sysfs_host_suspend_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		suspend_host_write(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t sysfs_host_suspend_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		suspend_host_write(&ltqusb_hcd_2,value);
		return count;
	}
	extern struct device_attribute dev_attr_host_suspend_1;
	DEVICE_ATTR(host_suspend_1, S_IRUGO|S_IWUSR, sysfs_host_suspend_1_show, sysfs_host_suspend_1_store);
	extern struct device_attribute dev_attr_host_suspend_2;
	DEVICE_ATTR(host_suspend_2, S_IRUGO|S_IWUSR, sysfs_host_suspend_2_show, sysfs_host_suspend_2_store);

	static ssize_t sysfs_bussuspend_1_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return bussuspend_read(&ltqusb_hcd_1,buf);
	}
	static ssize_t sysfs_bussuspend_2_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return bussuspend_read(&ltqusb_hcd_2,buf);
	}
	static ssize_t sysfs_bussuspend_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		bussuspend_write(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t sysfs_bussuspend_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		bussuspend_write(&ltqusb_hcd_2,value);
		return count;
	}
	extern struct device_attribute dev_attr_bussuspend_1;
	DEVICE_ATTR(bussuspend_1, S_IRUGO|S_IWUSR, sysfs_bussuspend_1_show, sysfs_bussuspend_1_store);
	extern struct device_attribute dev_attr_bussuspend_2;
	DEVICE_ATTR(bussuspend_2, S_IRUGO|S_IWUSR, sysfs_bussuspend_2_show, sysfs_bussuspend_2_store);

	static ssize_t sysfs_probe_host_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_single_probe(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t sysfs_probe_host_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_single_probe(&ltqusb_hcd_2,value);
		return count;
	}
	extern struct device_attribute dev_attr_probe_host_1;
	DEVICE_ATTR(probe_host_1, S_IRUGO|S_IWUSR, sysfs_host_suspend_1_show, sysfs_probe_host_1_store);
	extern struct device_attribute dev_attr_probe_host_2;
	DEVICE_ATTR(probe_host_2, S_IRUGO|S_IWUSR, sysfs_host_suspend_2_show, sysfs_probe_host_2_store);

	static ssize_t sysfs_host_probe_timer_1_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return host_probe_timer_read(&ltqusb_hcd_1,buf);
	}
	static ssize_t sysfs_host_probe_timer_2_show( struct device *_dev, struct device_attribute *attr,char *buf)
	{
		return host_probe_timer_read(&ltqusb_hcd_2,buf);
	}

	static ssize_t sysfs_host_probe_timer_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_probe_timer_write(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t sysfs_host_probe_timer_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_probe_timer_write(&ltqusb_hcd_2,value);
		return count;
	}
	extern struct device_attribute dev_attr_host_probe_timer_1;
	DEVICE_ATTR(host_probe_timer_1, S_IRUGO|S_IWUSR, sysfs_host_probe_timer_1_show, sysfs_host_probe_timer_1_store);
	extern struct device_attribute dev_attr_host_probe_timer_2;
	DEVICE_ATTR(host_probe_timer_2, S_IRUGO|S_IWUSR, sysfs_host_probe_timer_2_show, sysfs_host_probe_timer_2_store);

	static ssize_t sysfs_host_autoprobe_timer_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_autoprobe_timer_write(&ltqusb_hcd_1,value);
		return count;
	}
	static ssize_t sysfs_host_autoprobe_timer_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
	{
		char buf[10];
		int i = 0;
		uint32_t value;
		if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
			return -EFAULT;
		value = simple_strtoul(buf, NULL, 10);
		host_autoprobe_timer_write(&ltqusb_hcd_2,value);
		return count;
	}
	extern struct device_attribute dev_attr_host_autoprobe_timer_1;
	DEVICE_ATTR(host_autoprobe_timer_1, S_IRUGO|S_IWUSR, sysfs_host_probe_timer_1_show, sysfs_host_autoprobe_timer_1_store);
	extern struct device_attribute dev_attr_host_autoprobe_timer_2;
	DEVICE_ATTR(host_autoprobe_timer_2, S_IRUGO|S_IWUSR, sysfs_host_probe_timer_2_show, sysfs_host_autoprobe_timer_2_store);

		static ssize_t sysfs_oc_hy_1_show( struct device *_dev, struct device_attribute *attr,char *buf)
		{
			return sprintf (buf,"Host 1 Over-Current Hysteresis is %d\n",(int)(ltqusb_oc_get_hy(1)));
		}
		static ssize_t sysfs_oc_hy_2_show( struct device *_dev, struct device_attribute *attr,char *buf)
		{
			return sprintf (buf,"Host 2 Over-Current Hysteresis is %d\n",(int)(ltqusb_oc_get_hy(1)));
		}
		static ssize_t sysfs_oc_hy_1_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
		{
			char buf[10];
			int i = 0;
			uint32_t value;
			if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
				return -EFAULT;
			value = simple_strtoul(buf, NULL, 10);
			if( value>=0 && value < 4 )
				ltqusb_oc_set_hy(1,value);
			return count;
		}
		static ssize_t sysfs_oc_hy_2_store( struct device *_dev, struct device_attribute *attr,const char *buffer, size_t count )
		{
			char buf[10];
			int i = 0;
			uint32_t value;
			if (copy_from_user(buf, &buffer[i], sizeof("0xFFFFFFFF\n")+1))
				return -EFAULT;
			value = simple_strtoul(buf, NULL, 10);
			if( value>=0 && value < 4 )
				ltqusb_oc_set_hy(2,value);
			return count;
		}
		extern struct device_attribute dev_attr_oc_hy_1;
		DEVICE_ATTR(oc_hy_1, S_IRUGO|S_IWUSR, sysfs_oc_hy_1_show, sysfs_oc_hy_1_store);
		extern struct device_attribute dev_attr_oc_hy_2;
		DEVICE_ATTR(oc_hy_2, S_IRUGO|S_IWUSR, sysfs_oc_hy_2_show, sysfs_oc_hy_2_store);
#endif //__USE_PROCFS__
/////////////////////////////////////////////////////////////////////////////////////////////////////
// e/PROCSYSFS:  HOST POWER CONTROL
/////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#ifdef __USE_PROCFS__
	static struct proc_dir_entry * proc_ltq_root = NULL;
	static void ltq_proc_init(void)
	{
		proc_ltq_root = proc_mkdir(ltqusb_hcd_driver_name, (void *)0);
		if (!proc_ltq_root){
			LTQ_PRINT("%s proc initialization failed! \n", ltqusb_hcd_driver_name);
			return;
		}
	}
	static int ltq_proc_addproc(const char *funcname, umode_t mode, const struct file_operations *proc_fops)
	{
		if (!proc_ltq_root)
			ltq_proc_init();
		if(!proc_create(funcname, mode, proc_ltq_root, proc_fops))
		{
			printk(KERN_INFO "PROCFS: Failed to create (%s) entry!!\n", funcname);
			return -1;
		}
		return 0;
	}

	/* proc file system del function for removing module. */
	static void ltq_proc_delproc(const char *funcname)
	{
		char pname[30];
		sprintf(pname, "%s", funcname);
		remove_proc_entry(pname, proc_ltq_root);
	}
#endif //__USE_PROCFS__
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////


#if defined(__HOST_COC__)
	static LTQ_PMCU_MODULE_DEP_t depListUSBHost_1=
	{
	     1,
	     {
#ifdef __NEW_COC__
	         {LTQ_PMCU_MODULE_CPU, 0, LTQ_PMCU_STATE_D0, LTQ_PMCU_STATE_D0D3, LTQ_PMCU_STATE_D0D3, LTQ_PMCU_STATE_D0D3}
#else
	         {LTQ_PMCU_MODULE_CPU, LTQ_PMCU_STATE_D0, LTQ_PMCU_STATE_D0D3, LTQ_PMCU_STATE_D0D3, LTQ_PMCU_STATE_D0D3}
#endif
	     }
	};
	static LTQ_PMCU_MODULE_DEP_t depListUSBHost_2=
	{
	     1,
	     {
#ifdef __NEW_COC__
	         {LTQ_PMCU_MODULE_CPU, 0, LTQ_PMCU_STATE_D0, LTQ_PMCU_STATE_D0D3, LTQ_PMCU_STATE_D0D3, LTQ_PMCU_STATE_D0D3}
#else
	         {LTQ_PMCU_MODULE_CPU, LTQ_PMCU_STATE_D0, LTQ_PMCU_STATE_D0D3, LTQ_PMCU_STATE_D0D3, LTQ_PMCU_STATE_D0D3}
#endif
	     }
	};
	// This functions returns the current power state of the module
	static LTQ_PMCU_RETURN_t
	ltq_usbhost_stateGet_1(LTQ_PMCU_STATE_t *pmcuModState) {
	    //printk(KERN_DEBUG "ltq_usbhost_stateGet_1 is called\n");
	    if(ltqusb_hcd_1.power_status == 0){
	        printk(KERN_DEBUG "current power state of USB Host #1 is D0\n");
	        *pmcuModState = LTQ_PMCU_STATE_D0; // set here the right value
	    }
	    else if(ltqusb_hcd_1.power_status == 1){
	        printk(KERN_DEBUG "current power state of USB Host #1 is D3 (Suspend)\n");
	        *pmcuModState = LTQ_PMCU_STATE_D3; // set here the right value
	    }
	    else if(ltqusb_hcd_1.power_status == 2){
	        printk(KERN_DEBUG "current power state of USB Host #1 is D3 (Auto-Probing)\n");
	        *pmcuModState = LTQ_PMCU_STATE_D3; // set here the right value
	    }
	    else{
	        printk(KERN_DEBUG "current power state of USB Host #1 is unknown (%d)\n",ltqusb_hcd_1.power_status);
	        *pmcuModState = LTQ_PMCU_STATE_INVALID; // must be set to INVALID
	    }
	    return LTQ_PMCU_RETURN_SUCCESS;
	}
	static LTQ_PMCU_RETURN_t
	ltq_usbhost_stateGet_2(LTQ_PMCU_STATE_t *pmcuModState) {
	    //printk(KERN_DEBUG "ltq_usbhost_stateGet_2 is called\n");
	    if(ltqusb_hcd_2.power_status == 0){
	        printk(KERN_DEBUG "current power state of USB Host #2 is D0\n");
	        *pmcuModState = LTQ_PMCU_STATE_D0; // set here the right value
	    }
	    else if(ltqusb_hcd_2.power_status == 1){
	        printk(KERN_DEBUG "current power state of USB Host #2 is D3 (Suspend)\n");
	        *pmcuModState = LTQ_PMCU_STATE_D3; // set here the right value
	    }
	    else if(ltqusb_hcd_2.power_status == 2){
	        printk(KERN_DEBUG "current power state of USB Host #2 is D3 (Auto-Probing)\n");
	        *pmcuModState = LTQ_PMCU_STATE_D3; // set here the right value
	    }
	    else{
	        printk(KERN_DEBUG "current power state of USB Host #2 is unknown (%d)\n",ltqusb_hcd_2.power_status);
	        *pmcuModState = LTQ_PMCU_STATE_INVALID; // must be set to INVALID
	    }
	    return LTQ_PMCU_RETURN_SUCCESS;
	}

	// The function should be used to enable/disable the module specific power saving methods
	static LTQ_PMCU_RETURN_t
	ltq_usbhost_pwrFeatureSwitch_1(LTQ_PMCU_PWR_STATE_ENA_t pmcuPwrStateEna)
	{
	    if (pmcuPwrStateEna == LTQ_PMCU_PWR_STATE_ON) {
		    pmcuPowerFeatureUSBHost_1=1;
	        return LTQ_PMCU_RETURN_SUCCESS;
	    }
	    if (pmcuPwrStateEna == LTQ_PMCU_PWR_STATE_OFF) {
		    pmcuPowerFeatureUSBHost_1=0;
		    suspend_host_write(&ltqusb_hcd_1, 0);
	        return LTQ_PMCU_RETURN_SUCCESS;
        }
	    return LTQ_PMCU_RETURN_DENIED;
	}
	static LTQ_PMCU_RETURN_t
	ltq_usbhost_pwrFeatureSwitch_2(LTQ_PMCU_PWR_STATE_ENA_t pmcuPwrStateEna)
	{
	    if (pmcuPwrStateEna == LTQ_PMCU_PWR_STATE_ON) {
		    pmcuPowerFeatureUSBHost_2=1;
	        return LTQ_PMCU_RETURN_SUCCESS;
	    }
	    if (pmcuPwrStateEna == LTQ_PMCU_PWR_STATE_OFF) {
		    pmcuPowerFeatureUSBHost_2=0;
		    suspend_host_write(&ltqusb_hcd_2, 0);
	        return LTQ_PMCU_RETURN_SUCCESS;
        }
	    return LTQ_PMCU_RETURN_DENIED;
	}

	// This function should be used to do all the necessary clean-up's before a the real
	// power state change is initiated; e.g. flush all serial buffers inside the UART  before
	// the frequency will be changed.
	static LTQ_PMCU_RETURN_t
	ltq_usbhost_preChange_1(LTQ_PMCU_MODULE_t pmcuModule, LTQ_PMCU_STATE_t newState, LTQ_PMCU_STATE_t oldState)
	{
	    //printk(KERN_DEBUG "ltq_usbhost_preChange_1 is called\n");
	    return LTQ_PMCU_RETURN_SUCCESS;
	}
	static LTQ_PMCU_RETURN_t
	ltq_usbhost_preChange_2(LTQ_PMCU_MODULE_t pmcuModule, LTQ_PMCU_STATE_t newState, LTQ_PMCU_STATE_t oldState)
	{
	    //printk(KERN_DEBUG "ltq_usbhost_preChange_2 is called\n");
	    return LTQ_PMCU_RETURN_SUCCESS;
	}

	// This function initiate the real power state change. The module should do all the necessary
	//   adpations to the new state.
	static LTQ_PMCU_RETURN_t
	ltq_usbhost_stateChange_1(LTQ_PMCU_STATE_t newState)
	{
	    //printk(KERN_DEBUG "ltq_usbhost_stateChange_1 is called\n");
	    if(pmcuPowerFeatureUSBHost_1)
	    {
		    if (newState == LTQ_PMCU_STATE_D0) {
			    suspend_host_write(&ltqusb_hcd_1, 0);
		        return LTQ_PMCU_RETURN_SUCCESS;
		    }
		    if (newState == LTQ_PMCU_STATE_D1) {
			    suspend_host_write(&ltqusb_hcd_1, 1);
		        return LTQ_PMCU_RETURN_SUCCESS;
		    }
		    if (newState == LTQ_PMCU_STATE_D2) {
			    suspend_host_write(&ltqusb_hcd_1, 1);
		        return LTQ_PMCU_RETURN_SUCCESS;
		    }
		    if (newState == LTQ_PMCU_STATE_D3) {
			    suspend_host_write(&ltqusb_hcd_1, 1);
		        return LTQ_PMCU_RETURN_SUCCESS;
		    }
		}
	    return LTQ_PMCU_RETURN_DENIED;
	}
	static LTQ_PMCU_RETURN_t
	ltq_usbhost_stateChange_2(LTQ_PMCU_STATE_t newState)
	{
	    //printk(KERN_DEBUG "ltq_usbhost_stateChange_2 is called\n");
	    if(pmcuPowerFeatureUSBHost_2)
	    {
		    if (newState == LTQ_PMCU_STATE_D0) {
			    suspend_host_write(&ltqusb_hcd_2, 0);
		        return LTQ_PMCU_RETURN_SUCCESS;
		    }
		    if (newState == LTQ_PMCU_STATE_D1) {
			    suspend_host_write(&ltqusb_hcd_2, 1);
		        return LTQ_PMCU_RETURN_SUCCESS;
		    }
		    if (newState == LTQ_PMCU_STATE_D2) {
			    suspend_host_write(&ltqusb_hcd_2, 1);
		        return LTQ_PMCU_RETURN_SUCCESS;
		    }
		    if (newState == LTQ_PMCU_STATE_D3) {
			    suspend_host_write(&ltqusb_hcd_2, 1);
		        return LTQ_PMCU_RETURN_SUCCESS;
		    }
	    }
	    return LTQ_PMCU_RETURN_DENIED;
	}

	// This function should be used to do all the necessary post processing after a the real
	//   power state change was initiated.
	static LTQ_PMCU_RETURN_t
	ltq_usbhost_postChange_1(LTQ_PMCU_MODULE_t pmcuModule, LTQ_PMCU_STATE_t newState, LTQ_PMCU_STATE_t oldState)
	{
	    //printk(KERN_DEBUG "ltq_usbhost_postChange_1 is called\n");
	    return LTQ_PMCU_RETURN_SUCCESS;
	}
	static LTQ_PMCU_RETURN_t
	ltq_usbhost_postChange_2(LTQ_PMCU_MODULE_t pmcuModule, LTQ_PMCU_STATE_t newState, LTQ_PMCU_STATE_t oldState)
	{
	    //printk(KERN_DEBUG "ltq_usbhost_postChange_2 is called\n");
	    return LTQ_PMCU_RETURN_SUCCESS;
	}
#endif //defined(__HOST_COC__)

/*!
  \brief This function create the sysfs and procfs entries
  \param[in] _dev Pointer of device structure, if applied
 */
void ltqusb_attr_create_h (void *_dev)
{
	struct device *dev = (struct device *) _dev;

	LTQ_DEBUGPL(DBG_ENTRY, "%s() %d\n", __func__, __LINE__ );

#ifdef __USE_PROCFS__
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  VERSION
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ltq_proc_addproc("version_h",0, &procfs_version_fops);
	ltq_proc_addproc("dbglevel_h",0, &procfs_dbglevel_fops);


	if(ltqusb_hcd_1.dev)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  DUMP_PARAMS MODE
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		ltq_proc_addproc("dump_params_h_1",0, &procfs_dump_params_fops_1);
		ltq_proc_addproc("mode_h_1",0, &procfs_mode_fops_1);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST STATUS
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		ltq_proc_addproc("host_port_status_1",0, &procfs_host_port_status_fops_1);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  DUMP
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		#ifdef __ENABLE_DUMP__
			ltq_proc_addproc("dump_reg_h_1",0, &procfs_dump_reg_h_fops_1);
			ltq_proc_addproc("dump_spram_h_1",0, &procfs_dump_spram_h_fops_1);
			ltq_proc_addproc("dump_host_state_1",0, &procfs_dump_host_state_fops_1);
		#endif //__ENABLE_DUMP__

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  ANA
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		ltq_proc_addproc("ana_disconnect_threshold_1",0, &procfs_ana_disconnect_threshold_1_fops);
		ltq_proc_addproc("ana_squelch_threshold_1",0, &procfs_ana_squelch_threshold_1_fops);
		ltq_proc_addproc("ana_transmitter_crossover_1",0, &procfs_ana_transmitter_crossover_1_fops);
		ltq_proc_addproc("ana_transmitter_impedance_1",0, &procfs_ana_transmitter_impedance_1_fops);
		ltq_proc_addproc("ana_transmitter_dc_voltage_1",0, &procfs_ana_transmitter_dc_voltage_1_fops);
		ltq_proc_addproc("ana_transmitter_risefall_time_1",0, &procfs_ana_transmitter_risefall_time_1_fops);
		ltq_proc_addproc("ana_transmitter_pre_emphasis_1",0, &procfs_ana_transmitter_pre_emphasis_1_fops);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST CTRL
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		ltq_proc_addproc("buspower_1",0, &procfs_buspower_1_fops);
		ltq_proc_addproc("protect1_1",0, &procfs_protect1_1_fops);
		ltq_proc_addproc("protect2_1",0, &procfs_protect2_1_fops);
		ltq_proc_addproc("singlexact_1",0, &procfs_singlexact_1_fops);
		ltq_proc_addproc("burst_1",0, &procfs_burst_1_fops);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST POWER CONTROL
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		ltq_proc_addproc("host_suspend_1",0, &procfs_host_suspend_1_fops);
		ltq_proc_addproc("bussuspend_1",0, &procfs_bussuspend_1_fops);
		ltq_proc_addproc("probe_host_1",0, &procfs_probe_host_1_fops);
		ltq_proc_addproc("host_probe_timer_1",0, &procfs_host_probe_timer_1_fops);
		ltq_proc_addproc("host_autoprobe_timer_1",0, &procfs_host_autoprobe_timer_1_fops);
		ltq_proc_addproc("oc_hy_1",0, &procfs_oc_hy_1_fops);
	}
	if(ltqusb_hcd_2.dev)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  DUMP_PARAMS MODE
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		ltq_proc_addproc("dump_params_h_2",0, &procfs_dump_params_fops_2);
		ltq_proc_addproc("mode_h_2",0, &procfs_mode_fops_2);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST STATUS
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		ltq_proc_addproc("host_port_status_2",0, &procfs_host_port_status_fops_2);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  DUMP
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		#ifdef __ENABLE_DUMP__
			ltq_proc_addproc("dump_reg_h_2",0, &procfs_dump_reg_h_fops_2);
			ltq_proc_addproc("dump_spram_h_2",0, &procfs_dump_spram_h_fops_2);
			ltq_proc_addproc("dump_host_state_2",0, &procfs_dump_host_state_fops_2);
		#endif //__ENABLE_DUMP__

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  ANA
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		ltq_proc_addproc("ana_disconnect_threshold_2",0, &procfs_ana_disconnect_threshold_2_fops);
		ltq_proc_addproc("ana_squelch_threshold_2",0, &procfs_ana_squelch_threshold_2_fops);
		ltq_proc_addproc("ana_transmitter_crossover_2",0, &procfs_ana_transmitter_crossover_2_fops);
		ltq_proc_addproc("ana_transmitter_impedance_2",0, &procfs_ana_transmitter_impedance_2_fops);
		ltq_proc_addproc("ana_transmitter_dc_voltage_2",0, &procfs_ana_transmitter_dc_voltage_2_fops);
		ltq_proc_addproc("ana_transmitter_risefall_time_2",0, &procfs_ana_transmitter_risefall_time_2_fops);
		ltq_proc_addproc("ana_transmitter_pre_emphasis_2",0, &procfs_ana_transmitter_pre_emphasis_2_fops);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST CTRL
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		ltq_proc_addproc("buspower_2",0, &procfs_buspower_2_fops);
		ltq_proc_addproc("protect1_2",0, &procfs_protect1_2_fops);
		ltq_proc_addproc("protect2_2",0, &procfs_protect2_2_fops);
		ltq_proc_addproc("singlexact_2",0, &procfs_singlexact_2_fops);
		ltq_proc_addproc("burst_2",0, &procfs_burst_2_fops);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST POWER CONTROL
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		ltq_proc_addproc("host_suspend_2",0, &procfs_host_suspend_2_fops);
		ltq_proc_addproc("bussuspend_2",0, &procfs_bussuspend_2_fops);
		ltq_proc_addproc("probe_host_2",0, &procfs_probe_host_2_fops);
		ltq_proc_addproc("host_probe_timer_2",0, &procfs_host_probe_timer_2_fops);
		ltq_proc_addproc("host_autoprobe_timer_2",0, &procfs_host_autoprobe_timer_2_fops);
		ltq_proc_addproc("oc_hy_2",0, &procfs_oc_hy_2_fops);
	}

#endif //__USE_PROCFS__

#ifdef __USE_SYSFS__
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  VERSION
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	device_create_file(dev, &dev_attr_version_h);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  DBGLEVEL
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	device_create_file(dev, &dev_attr_dbglevel_h);

	if(ltqusb_hcd_1.dev)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  DUMP_PARAMS MODE
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		device_create_file(dev, &dev_attr_dump_params_h_1);
		device_create_file(dev, &dev_attr_mode_h_1);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST STATUS
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		device_create_file(dev, &dev_attr_host_port_status_1);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  DUMP
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		#ifdef __ENABLE_DUMP__
			device_create_file(dev, &dev_attr_dump_reg_h_1);
			device_create_file(dev, &dev_attr_dump_spram_h_1);
			device_create_file(dev, &dev_attr_dump_host_state_1);
		#endif //__ENABLE_DUMP__

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  ANA
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		device_create_file(dev, &dev_attr_ana_disconnect_threshold_1);
		device_create_file(dev, &dev_attr_ana_squelch_threshold_1);
		device_create_file(dev, &dev_attr_ana_transmitter_crossover_1);
		device_create_file(dev, &dev_attr_ana_transmitter_impedance_1);
		device_create_file(dev, &dev_attr_ana_transmitter_dc_voltage_1);
		device_create_file(dev, &dev_attr_ana_transmitter_risefall_time_1);
		device_create_file(dev, &dev_attr_ana_transmitter_pre_emphasis_1);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST CTRL
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		device_create_file(dev, &dev_attr_buspower_1);
		device_create_file(dev, &dev_attr_protect1_1);
		device_create_file(dev, &dev_attr_protect2_1);
		device_create_file(dev, &dev_attr_singlexact_1);
		device_create_file(dev, &dev_attr_burst_1);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST POWER CONTROL
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		device_create_file(dev, &dev_attr_host_suspend_1);
		device_create_file(dev, &dev_attr_bussuspend_1);
		device_create_file(dev, &dev_attr_probe_host_1);
		device_create_file(dev, &dev_attr_host_probe_timer_1);
		device_create_file(dev, &dev_attr_host_autoprobe_timer_1);

		device_create_file(dev, &dev_attr_oc_hy_1);
	}
	if(ltqusb_hcd_2.dev)
	{
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  DUMP_PARAMS MODE
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		device_create_file(dev, &dev_attr_dump_params_h_2);
		device_create_file(dev, &dev_attr_mode_h_2);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST STATUS
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		device_create_file(dev, &dev_attr_host_port_status_2);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  DUMP
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		#ifdef __ENABLE_DUMP__
			device_create_file(dev, &dev_attr_dump_reg_h_2);
			device_create_file(dev, &dev_attr_dump_spram_h_2);
			device_create_file(dev, &dev_attr_dump_host_state_2);
		#endif //__ENABLE_DUMP__

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  ANA
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		device_create_file(dev, &dev_attr_ana_disconnect_threshold_2);
		device_create_file(dev, &dev_attr_ana_squelch_threshold_2);
		device_create_file(dev, &dev_attr_ana_transmitter_crossover_2);
		device_create_file(dev, &dev_attr_ana_transmitter_impedance_2);
		device_create_file(dev, &dev_attr_ana_transmitter_dc_voltage_2);
		device_create_file(dev, &dev_attr_ana_transmitter_risefall_time_2);
		device_create_file(dev, &dev_attr_ana_transmitter_pre_emphasis_2);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST CTRL
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		device_create_file(dev, &dev_attr_buspower_2);
		device_create_file(dev, &dev_attr_protect1_2);
		device_create_file(dev, &dev_attr_protect2_2);
		device_create_file(dev, &dev_attr_singlexact_2);
		device_create_file(dev, &dev_attr_burst_2);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		// PROCSYSFS:  HOST POWER CONTROL
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		device_create_file(dev, &dev_attr_host_suspend_2);
		device_create_file(dev, &dev_attr_bussuspend_2);
		device_create_file(dev, &dev_attr_probe_host_2);
		device_create_file(dev, &dev_attr_host_probe_timer_2);
		device_create_file(dev, &dev_attr_host_autoprobe_timer_2);

		device_create_file(dev, &dev_attr_oc_hy_2);
	}
#endif //__USE_SYSFS__

#if defined(__HOST_COC__)
	if(ltqusb_hcd_1.dev)
	{
	   memset (&pmcuRegisterUSBHost_1, 0, sizeof(pmcuRegisterUSBHost_1));
	   pmcuRegisterUSBHost_1.pmcuModule=LTQ_PMCU_MODULE_USB;
	   pmcuRegisterUSBHost_1.pmcuModuleNr=1;
	   pmcuRegisterUSBHost_1.pmcuModuleDep = &depListUSBHost_1;
	   pmcuRegisterUSBHost_1.pre = ltq_usbhost_preChange_1;
	   pmcuRegisterUSBHost_1.post = ltq_usbhost_postChange_1;
	   pmcuRegisterUSBHost_1.ltq_pmcu_state_change = ltq_usbhost_stateChange_1;
	   pmcuRegisterUSBHost_1.ltq_pmcu_state_get = ltq_usbhost_stateGet_1;
	   pmcuRegisterUSBHost_1.ltq_pmcu_pwr_feature_switch = ltq_usbhost_pwrFeatureSwitch_1;
	   #ifdef CONFIG_LTQ_PMCU
		   ltq_pmcu_register ( &pmcuRegisterUSBHost_1 );
	   #endif

		#ifdef __NEW_COC__
			   #ifdef CONFIG_CPU_FREQ
			/*
		       if ( cpufreq_register_notifier(&ltq_usbhost_1_cpufreq_notifier_block,CPUFREQ_TRANSITION_NOTIFIER) ) {
		            printk(KERN_ERR "Fail in registering USB_HOST_1 to CPUFREQ\n");
		            return;
		       }
		       if ( cpufreq_register_notifier(&ltq_usbhost_2_cpufreq_notifier_block,CPUFREQ_TRANSITION_NOTIFIER) ) {
		            printk(KERN_ERR "Fail in registering USB_HOST_2 to CPUFREQ\n");
		            return;
		       }
			*/
			   /* add two entrys to the module status linked list */
			   list_add_tail(&ltq_usbhost1_lxfreq_mod_g.list, &ltq_lxfreq_head_mod_list_g);
			   #endif //CONFIG_CPU_FREQ
		#endif //__NEW_COC__
	}
	if(ltqusb_hcd_2.dev)
	{
	   memset (&pmcuRegisterUSBHost_2, 0, sizeof(pmcuRegisterUSBHost_2));
	   pmcuRegisterUSBHost_2.pmcuModule=LTQ_PMCU_MODULE_USB;
	   pmcuRegisterUSBHost_2.pmcuModuleNr=2;
	   pmcuRegisterUSBHost_2.pmcuModuleDep = &depListUSBHost_2;
	   pmcuRegisterUSBHost_2.pre = ltq_usbhost_preChange_2;
	   pmcuRegisterUSBHost_2.post = ltq_usbhost_postChange_2;
	   pmcuRegisterUSBHost_2.ltq_pmcu_state_change = ltq_usbhost_stateChange_2;
	   pmcuRegisterUSBHost_2.ltq_pmcu_state_get = ltq_usbhost_stateGet_2;
	   pmcuRegisterUSBHost_2.ltq_pmcu_pwr_feature_switch = ltq_usbhost_pwrFeatureSwitch_2;
	   #ifdef CONFIG_LTQ_PMCU
		   ltq_pmcu_register ( &pmcuRegisterUSBHost_2 );
	   #endif

		#ifdef __NEW_COC__
			   #ifdef CONFIG_CPU_FREQ
			/*
		       if ( cpufreq_register_notifier(&ltq_usbhost_1_cpufreq_notifier_block,CPUFREQ_TRANSITION_NOTIFIER) ) {
		            printk(KERN_ERR "Fail in registering USB_HOST_1 to CPUFREQ\n");
		            return;
		       }
		       if ( cpufreq_register_notifier(&ltq_usbhost_2_cpufreq_notifier_block,CPUFREQ_TRANSITION_NOTIFIER) ) {
		            printk(KERN_ERR "Fail in registering USB_HOST_2 to CPUFREQ\n");
		            return;
		       }
			*/
			   /* add two entrys to the module status linked list */
			   list_add_tail(&ltq_usbhost2_lxfreq_mod_g.list, &ltq_lxfreq_head_mod_list_g);
			   #endif //CONFIG_CPU_FREQ
		#endif //__NEW_COC__
	}
#endif //defined(__HOST_COC__)
}

/*!
  \brief This function remove the sysfs and procfs entries
  \param[in] _dev Pointer of device structure, if applied
 */
void ltqusb_attr_remove_h (void *_dev)
{
	struct device *dev = (struct device *) _dev;

	LTQ_DEBUGPL(DBG_ENTRY, "%s() %d\n", __func__, __LINE__ );

#ifdef __USE_PROCFS__
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  VERSION
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ltq_proc_delproc("version_h");
	ltq_proc_delproc("dbglevel_h");

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  DUMP_PARAMS MODE
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ltq_proc_delproc("dump_params_h_1");
	ltq_proc_delproc("dump_params_h_2");
	ltq_proc_delproc("mode_h_1");
	ltq_proc_delproc("mode_h_2");

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  HOST STATUS
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ltq_proc_delproc("host_port_status_1");
	ltq_proc_delproc("host_port_status_2");

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  DUMP
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef __ENABLE_DUMP__
		ltq_proc_delproc("dump_reg_h_1");
		ltq_proc_delproc("dump_reg_h_2");
		ltq_proc_delproc("dump_spram_h_1");
		ltq_proc_delproc("dump_spram_h_2");
		ltq_proc_delproc("dump_host_state_1");
		ltq_proc_delproc("dump_host_state_2");
	#endif //__ENABLE_DUMP__

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  ANA
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ltq_proc_delproc("ana_disconnect_threshold_1");
	ltq_proc_delproc("ana_disconnect_threshold_2");
	ltq_proc_delproc("ana_squelch_threshold_1");
	ltq_proc_delproc("ana_squelch_threshold_2");
	ltq_proc_delproc("ana_transmitter_crossover_1");
	ltq_proc_delproc("ana_transmitter_crossover_2");
	ltq_proc_delproc("ana_transmitter_impedance_1");
	ltq_proc_delproc("ana_transmitter_impedance_2");
	ltq_proc_delproc("ana_transmitter_dc_voltage_1");
	ltq_proc_delproc("ana_transmitter_dc_voltage_2");
	ltq_proc_delproc("ana_transmitter_risefall_time_1");
	ltq_proc_delproc("ana_transmitter_risefall_time_2");
	ltq_proc_delproc("ana_transmitter_pre_emphasis_1");
	ltq_proc_delproc("ana_transmitter_pre_emphasis_2");

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  HOST CTRL
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ltq_proc_delproc("buspower_1");
	ltq_proc_delproc("buspower_2");
	ltq_proc_delproc("protect1_1");
	ltq_proc_delproc("protect1_2");
	ltq_proc_delproc("protect2_1");
	ltq_proc_delproc("protect2_2");
	ltq_proc_delproc("singlexact_1");
	ltq_proc_delproc("singlexact_2");
	ltq_proc_delproc("burst_1");
	ltq_proc_delproc("burst_2");

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  HOST POWER CONTROL
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	ltq_proc_delproc("host_suspend_1");
	ltq_proc_delproc("host_suspend_2");
	ltq_proc_delproc("bussuspend_1");
	ltq_proc_delproc("bussuspend_2");
	ltq_proc_delproc("probe_host_1");
	ltq_proc_delproc("probe_host_2");
	ltq_proc_delproc("host_probe_timer_1");
	ltq_proc_delproc("host_probe_timer_2");
	ltq_proc_delproc("host_autoprobe_timer_1");
	ltq_proc_delproc("host_autoprobe_timer_2");
	ltq_proc_delproc("oc_hy_1");
	ltq_proc_delproc("oc_hy_2");

	remove_proc_entry(ltqusb_hcd_driver_name, (void *)0);
#endif //__USE_PROCFS__

#ifdef __USE_SYSFS__
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  VERSION
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	device_remove_file(dev, &dev_attr_version_h);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  DBGLEVEL
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	device_remove_file(dev, &dev_attr_dbglevel_h);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  DUMP_PARAMS MODE
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	device_remove_file(dev, &dev_attr_dump_params_h_1);
	device_remove_file(dev, &dev_attr_dump_params_h_2);
	device_remove_file(dev, &dev_attr_mode_h_1);
	device_remove_file(dev, &dev_attr_mode_h_2);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  HOST STATUS
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	device_remove_file(dev, &dev_attr_host_port_status_1);
	device_remove_file(dev, &dev_attr_host_port_status_2);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  DUMP
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	#ifdef __ENABLE_DUMP__
		device_remove_file(dev, &dev_attr_dump_reg_h_1);
		device_remove_file(dev, &dev_attr_dump_reg_h_2);
		device_remove_file(dev, &dev_attr_dump_spram_h_1);
		device_remove_file(dev, &dev_attr_dump_spram_h_2);
		device_remove_file(dev, &dev_attr_dump_host_state_1);
		device_remove_file(dev, &dev_attr_dump_host_state_2);
	#endif //__ENABLE_DUMP__

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  ANA
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	device_remove_file(dev, &dev_attr_ana_disconnect_threshold_1);
	device_remove_file(dev, &dev_attr_ana_disconnect_threshold_2);
	device_remove_file(dev, &dev_attr_ana_squelch_threshold_1);
	device_remove_file(dev, &dev_attr_ana_squelch_threshold_2);
	device_remove_file(dev, &dev_attr_ana_transmitter_crossover_1);
	device_remove_file(dev, &dev_attr_ana_transmitter_crossover_2);
	device_remove_file(dev, &dev_attr_ana_transmitter_impedance_1);
	device_remove_file(dev, &dev_attr_ana_transmitter_impedance_2);
	device_remove_file(dev, &dev_attr_ana_transmitter_dc_voltage_1);
	device_remove_file(dev, &dev_attr_ana_transmitter_dc_voltage_2);
	device_remove_file(dev, &dev_attr_ana_transmitter_risefall_time_1);
	device_remove_file(dev, &dev_attr_ana_transmitter_risefall_time_2);
	device_remove_file(dev, &dev_attr_ana_transmitter_pre_emphasis_1);
	device_remove_file(dev, &dev_attr_ana_transmitter_pre_emphasis_2);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  HOST CTRL
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	device_remove_file(dev, &dev_attr_buspower_1);
	device_remove_file(dev, &dev_attr_buspower_2);
	device_remove_file(dev, &dev_attr_protect1_1);
	device_remove_file(dev, &dev_attr_protect1_2);
	device_remove_file(dev, &dev_attr_protect2_1);
	device_remove_file(dev, &dev_attr_protect2_2);
	device_remove_file(dev, &dev_attr_singlexact_1);
	device_remove_file(dev, &dev_attr_singlexact_2);
	device_remove_file(dev, &dev_attr_burst_1);
	device_remove_file(dev, &dev_attr_burst_2);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// PROCSYSFS:  HOST POWER CONTROL
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	device_remove_file(dev, &dev_attr_host_suspend_1);
	device_remove_file(dev, &dev_attr_host_suspend_2);
	device_remove_file(dev, &dev_attr_bussuspend_1);
	device_remove_file(dev, &dev_attr_bussuspend_2);
	device_remove_file(dev, &dev_attr_probe_host_1);
	device_remove_file(dev, &dev_attr_probe_host_2);
	device_remove_file(dev, &dev_attr_host_probe_timer_1);
	device_remove_file(dev, &dev_attr_host_probe_timer_2);
	device_remove_file(dev, &dev_attr_host_autoprobe_timer_1);
	device_remove_file(dev, &dev_attr_host_autoprobe_timer_2);

	device_remove_file(dev, &dev_attr_oc_hy_1);
	device_remove_file(dev, &dev_attr_oc_hy_2);
#endif //__USE_SYSFS__

#if defined(__HOST_COC__)
   ltq_pmcu_unregister ( &pmcuRegisterUSBHost_1 );
   ltq_pmcu_unregister ( &pmcuRegisterUSBHost_2 );
#endif
}


/* initialize the proc file system and make a dir named /proc/[name] */


static void ltqusb_dump_params(ltqusb_core_if_t *_core_if)
{
	ltqusb_params_t *params=&_core_if->params;

	LTQ_PRINT("LTQUSB Dump Parameters ( Host Mode) \n");
	LTQ_PRINT("     Burst size: %d\n",params->dma_burst_size);

	if     (params->speed==1)
		LTQ_PRINT("Full Speed only\n");
	else if(params->speed==0)
		LTQ_PRINT("Full/Hign Speed\n");
	else
		LTQ_PRINT("Unkonwn setting (%d) for Speed\n",params->speed);

	LTQ_PRINT("Total Data FIFO size: %d(0x%06X) DWord, %d(0x%06X) Bytes\n",
		params->data_fifo_size,params->data_fifo_size,
		params->data_fifo_size*4, params->data_fifo_size*4
	);

	LTQ_PRINT("Host Channels: %d\n",params->host_channels);

	LTQ_PRINT("Rx FIFO size: %d(0x%06X) DWord, %d(0x%06X) Bytes\n",
		params->data_fifo_size,params->data_fifo_size,
		params->data_fifo_size*4, params->data_fifo_size*4
	);

	LTQ_PRINT("NP Tx FIFO size: %d(0x%06X) DWord, %d(0x%06X) Bytes\n",
		params->nperio_tx_fifo_size,params->nperio_tx_fifo_size,
		params->nperio_tx_fifo_size*4, params->nperio_tx_fifo_size*4
	);

	LTQ_PRINT(" P Tx FIFO size: %d(0x%06X) DWord, %d(0x%06X) Bytes\n",
		params->perio_tx_fifo_size,params->perio_tx_fifo_size,
		params->perio_tx_fifo_size*4, params->perio_tx_fifo_size*4
	);

	LTQ_PRINT("Max Transfer size: %d(0x%06X) Bytes\n",
		params->max_transfer_size,params->max_transfer_size
	);
	LTQ_PRINT("Max Packet Count: %d(0x%06X)\n",
		params->max_packet_count,params->max_packet_count
	);

	LTQ_PRINT("PHY UTMI Width: %d\n",params->phy_utmi_width);

	LTQ_PRINT("Timeout Calibration: %d\n",params->timeout_cal);


	LTQ_PRINT("==================================================\n");
	LTQ_PRINT("End of Parameters Dump\n");
	LTQ_PRINT("==================================================\n");
}

