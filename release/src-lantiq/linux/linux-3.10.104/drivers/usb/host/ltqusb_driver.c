/*****************************************************************************
 **   FILE NAME       : ltqusb_driver.c
 **   PROJECT         : Lantiq USB sub-system V3
 **   MODULES         : Lantiq USB sub-system Host and Device driver
 **   SRC VERSION     : 3.2
 **   DATE            : 1/Jan/2011
 **   AUTHOR          : Chen, Howard
 **   DESCRIPTION     : The provides the initialization and cleanup entry
 **                     points for the Lantiq USB driver. This module can be
 **                     dynamically loaded with insmod command or built-in
 **                     with kernel. When loaded or executed the ltqusb_driver_init
 **                     function is called. When the module is removed (using rmmod),
 **                     the ltqusb_driver_cleanup function is called.
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

/*!
 \file ltqusb_driver.c
 \brief This file contains the loading/unloading interface to the Linux driver.
*/

#include <linux/version.h>
#include "ltqusb_version.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_gpio.h>

#include <linux/errno.h>
#include <linux/types.h>
#include <linux/stat.h>  /* permission constants */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
	#include <linux/irq.h>
#endif

#include <asm/io.h>
//#include <asm/mach-ltqcpe/ltq_regs.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
	#include <asm/irq.h>
#endif

#include "ltqusb_plat.h"

#include "ltqusb_cif.h"

#include "ltqhcd.h"

#define    USB_DRIVER_DESC		"Lantiq USB HCD driver"
const char ltqusb_hcd_driver_name[]    = "ltqusb_hcd";
ltqhcd_hcd_t ltqusb_hcd_1;
ltqhcd_hcd_t ltqusb_hcd_2;
const char ltqusb_hcd_name_1[] = "ltqusb_hcd_1";
const char ltqusb_hcd_name_2[] = "ltqusb_hcd_2";

int ltqusb_oc_shared  =0;
int ltqusb_vbus_shared=0;
int ltqusb_led_shared =0;

ltqhcd_hcd_t *oc1_int_target=NULL;
ltqhcd_hcd_t *oc2_int_target=NULL;
extern unsigned int  oc1_int_no;
extern unsigned int  oc2_int_no;

/* Global Debug Level Mask. */
uint32_t h_dbg_lvl = 0x00;

ltqusb_params_t ltqusb_module_params_h;

static void parse_parms(void);

/* Function to setup the structures to control one usb core running as host*/
/*!
   \brief inlined by ltqusb_driver_probe(), handling host mode probing. Run at each host core.
*/
static inline int ltqusb_driver_probe_h(ltqhcd_hcd_t *_hcd,
                                        int           _irq,
                                        uint32_t      _iobase,
                                        uint32_t      _fifomem,
                                        uint32_t      _fifodbg
                                        )
{
	int retval = 0;
	LTQ_DEBUGPL(DBG_ENTRY, "%s() %d\n", __func__, __LINE__ );

	ltqusb_power_on_h (&_hcd->core_if);
	mdelay(50);
	ltqusb_phy_power_on_h  (&_hcd->core_if); // Test
	mdelay(50);
	ltqusb_hard_reset_h(&_hcd->core_if);

	retval =ltqusb_core_if_init_h(&_hcd->core_if,
	                             _irq,
	                             _iobase,
	                             _fifomem,
	                             _fifodbg);
	if(retval)
		return retval;

	ltqusb_host_core_init(&_hcd->core_if,&ltqusb_module_params_h);

	ltqusb_disable_global_interrupts_h( &_hcd->core_if);

	/* The driver is now initialized and need to be registered into Linux USB sub-system */

	retval = ltqhcd_init(_hcd); // hook the hcd into usb ss

	if (retval != 0)
	{
		LTQ_ERROR("_hcd_init failed\n");
		return retval;
	}

	//ltqusb_enable_global_interrupts_h( _hcd->core_if ); // this should be done at hcd_start , including hcd_interrupt
	return 0;
}

/*!
   \brief This function is called when a driver is unregistered. This happens when
  the rmmod command is executed. The device may or may not be electrically
  present. If it is present, the driver stops device processing. Any resources
  used on behalf of this device are freed.
*/
static int ltqusb_driver_remove(struct platform_device *_pdev)
{
	LTQ_DEBUGPL(DBG_ENTRY, "%s() %d\n", __func__, __LINE__ );
	ltqhcd_remove(&ltqusb_hcd_1);
	ltqusb_core_if_remove_h(&ltqusb_hcd_1.core_if );
	ltqhcd_remove(&ltqusb_hcd_2);
	ltqusb_core_if_remove_h(&ltqusb_hcd_2.core_if );
	/* Remove the device attributes */
	ltqusb_attr_remove_h(&_pdev->dev);
	return 0;
}




/*!
   \brief This function is called by module management in 2.6 kernel or by ltqusb_driver_init with 2.4 kernel
  It is to probe and setup LTQUSB core(s).
*/
static int ltqusb_driver_probe(struct platform_device *_pdev)
{
	int retval = 0;
	struct device_node *np;
	uint32_t plt;
	LTQ_DEBUGPL(DBG_ENTRY, "%s() %d\n", __func__, __LINE__ );
	LTQ_PRINT("%s: version %s\n", ltqusb_hcd_driver_name, LTQUSB_VERSION);

	#if 1
		#ifdef __PHY_LONG_PREEMP__
			LTQ_PRINT("   OPTION: __PHY_LONG_PREEMP__\n");
		#endif
		#ifdef __FORCE_USB11__
			LTQ_PRINT("   OPTION: __FORCE_USB11__\n");
		#endif
		#ifdef __UNALIGNED_BUF_ADJ__
			LTQ_PRINT("   OPTION: __UNALIGNED_BUF_ADJ__\n");
		#endif
		#ifdef __UNALIGNED_BUF_CHK__
			LTQ_PRINT("   OPTION: __UNALIGNED_BUF_CHK__\n");
		#endif
		#ifdef __UNALIGNED_BUF_BURST__
			LTQ_PRINT("   OPTION: __UNALIGNED_BUF_BURST__\n");
		#endif
		#ifdef __DEBUG__
			LTQ_PRINT("   OPTION: __DEBUG__\n");
		#endif
		#ifdef __ENABLE_DUMP__
			LTQ_PRINT("   OPTION: __ENABLE_DUMP__\n");
		#endif
		#ifdef __NEW_COC__
			LTQ_PRINT("   OPTION: __NEW_COC__\n");
		#endif
		#ifdef __WITH_HS_ELECT_TST__
			LTQ_PRINT("           __WITH_HS_ELECT_TST__\n");
		#endif
		#ifdef __EN_ISOC__
			LTQ_PRINT("           __EN_ISOC__\n");
		#endif
		#ifdef __EN_ISOC_SPLIT__
			LTQ_PRINT("           __EN_ISOC_SPLIT__\n");
		#endif
		#ifdef __EPQD_DESTROY_TIMEOUT__
			LTQ_PRINT("           __EPQD_DESTROY_TIMEOUT__\n");
		#endif
		#ifdef __DYN_SOF_INTR__
			LTQ_PRINT("           __DYN_SOF_INTR__\n");
		#endif
		#ifdef __HOST_COC__
			LTQ_PRINT("           __HOST_COC__\n");
		#endif
		#ifdef __INTRNAKRETRY__
			LTQ_PRINT("           __INTRNAKRETRY__\n");
		#endif
		#ifdef __INTRINCRETRY__
			LTQ_PRINT("           __INTRINCRETRY__\n");
		#endif
	#endif

	memset(&ltqusb_hcd_1, 0, sizeof(ltqhcd_hcd_t));
	memset(&ltqusb_hcd_2, 0, sizeof(ltqhcd_hcd_t));
	ltqusb_hcd_1.core_if.core_no=0;
	ltqusb_hcd_2.core_if.core_no=1;
	ltqusb_hcd_1.core_if.core_name=(char *)ltqusb_hcd_name_1;
	ltqusb_hcd_2.core_if.core_name=(char *)ltqusb_hcd_name_2;

	ltqusb_hcd_1.vbusgpioid=-1;
	ltqusb_hcd_2.vbusgpioid=-1;
	ltqusb_hcd_1.ledgpioid=-1;
	ltqusb_hcd_2.ledgpioid=-1;

	for_each_compatible_node(np,NULL,"lantiq,ltqusb")
	{
		if(of_property_match_string(np,"status","okay")>=0)
			break;
		if(of_property_match_string(np,"status","ok")>=0)
			break;
	}
	if(!np)
	{
		LTQ_PRINT("No matching in DTS\n");
		return -1;
	}

	ltqusb_chip_family=0;
	if(of_machine_is_compatible("lantiq,danube")) ltqusb_chip_family= LTQUSB_CHIP_DANUBE;
	if(of_machine_is_compatible("lantiq,ase")   ) ltqusb_chip_family= LTQUSB_CHIP_ASE;
	if(of_machine_is_compatible("lantiq,ar9")   ) ltqusb_chip_family= LTQUSB_CHIP_AR9;
	if(of_machine_is_compatible("lantiq,vr9")   ) ltqusb_chip_family= LTQUSB_CHIP_VR9;
	if(of_machine_is_compatible("lantiq,ar10")  ) ltqusb_chip_family= LTQUSB_CHIP_AR10;
	if(of_machine_is_compatible("lantiq,grx390")) ltqusb_chip_family= LTQUSB_CHIP_AR10;

	if(!ltqusb_chip_family)
	{
		LTQ_PRINT("No matching in SoC\n");
		return -1;
	}

	if(_pdev->num_resources >=8) //dual
	{
		ltqusb_hcd_1.core_if.mem1=_pdev->resource[ 0].start;
		ltqusb_hcd_1.core_if.mem2=_pdev->resource[ 1].start;
		ltqusb_hcd_1.core_if.mem3=_pdev->resource[ 2].start;
		ltqusb_hcd_2.core_if.mem1=_pdev->resource[ 3].start;
		ltqusb_hcd_2.core_if.mem2=_pdev->resource[ 4].start;
		ltqusb_hcd_2.core_if.mem3=_pdev->resource[ 5].start;
		ltqusb_hcd_1.core_if.irq =_pdev->resource[ 6].start;
		ltqusb_hcd_2.core_if.irq =_pdev->resource[ 7].start;
		if(_pdev->num_resources > 8)
			ltqusb_hcd_1.core_if.ocirq=
			ltqusb_hcd_2.core_if.ocirq=_pdev->resource[ 8].start;
		if(_pdev->num_resources > 9)
			ltqusb_hcd_2.core_if.ocirq=_pdev->resource[ 9].start;

		ltqusb_hcd_1.vbusgpioid=of_get_named_gpio(np,"vbus",0);
		ltqusb_hcd_2.vbusgpioid=of_get_named_gpio(np,"vbus",1);
		ltqusb_hcd_1.ledgpioid =of_get_named_gpio(np,"led",0);
		ltqusb_hcd_2.ledgpioid =of_get_named_gpio(np,"led",1);
		if(!gpio_is_valid(ltqusb_hcd_1.vbusgpioid))
			ltqusb_hcd_1.vbusgpioid=-1;
		if(!gpio_is_valid(ltqusb_hcd_2.vbusgpioid))
			ltqusb_hcd_2.vbusgpioid=-1;
		if(!gpio_is_valid(ltqusb_hcd_1.ledgpioid))
			ltqusb_hcd_1.ledgpioid=-1;
		if(!gpio_is_valid(ltqusb_hcd_2.ledgpioid))
			ltqusb_hcd_2.ledgpioid=-1;

		plt=0;of_property_read_u32_index(np,"oc_off",0,&plt);
		if(plt)
			ltqusb_hcd_1.core_if.ocirq=0;
		plt=0;of_property_read_u32_index(np,"oc_off",1,&plt);
		if(plt)
			ltqusb_hcd_2.core_if.ocirq=0;

		plt=0;of_property_read_u32_index(np,"as_host",0,&plt);
		if(!plt)
			ltqusb_hcd_1.core_if.irq=0;
		plt=0;of_property_read_u32_index(np,"as_host",1,&plt);
		if(!plt)
			ltqusb_hcd_2.core_if.irq=0;
	}
	else
	{
		ltqusb_hcd_1.core_if.mem1=_pdev->resource[ 0].start;
		ltqusb_hcd_1.core_if.mem2=_pdev->resource[ 1].start;
		ltqusb_hcd_1.core_if.mem3=_pdev->resource[ 2].start;
		ltqusb_hcd_1.core_if.irq =_pdev->resource[ 3].start;
		if(_pdev->num_resources > 4)
			ltqusb_hcd_1.core_if.ocirq=_pdev->resource[ 4].start;
		ltqusb_hcd_1.vbusgpioid=of_get_named_gpio(np,"vbus",0);
		ltqusb_hcd_1.ledgpioid=of_get_named_gpio(np,"led",0);
		plt=0;of_property_read_u32(np,"oc_off",&plt);
		if(plt)
			ltqusb_hcd_1.core_if.ocirq=0;
		plt=0;of_property_read_u32(np,"as_host",&plt);
		if(!plt)
			ltqusb_hcd_1.core_if.irq=0;
	}

	if(!ltqusb_hcd_1.core_if.irq && !ltqusb_hcd_2.core_if.irq)
	{
		LTQ_PRINT( "None of the Host Port is enabled\n");
		return -1;
	}

	if(!ltqusb_hcd_1.core_if.irq)
	{
		ltqusb_hcd_1.core_if.ocirq=0;
		ltqusb_hcd_1.vbusgpioid=ltqusb_hcd_1.ledgpioid=-1;
	}
	if(!ltqusb_hcd_2.core_if.irq)
	{
		ltqusb_hcd_2.core_if.ocirq=0;
		ltqusb_hcd_2.vbusgpioid=ltqusb_hcd_1.ledgpioid=-1;
	}

	ltqusb_oc_shared=0;
	ltqusb_vbus_shared=0;
	ltqusb_led_shared=0;

	if(ltqusb_hcd_1.core_if.ocirq>0 && ltqusb_hcd_2.core_if.ocirq>0 && ltqusb_hcd_1.core_if.ocirq==ltqusb_hcd_2.core_if.ocirq)
		ltqusb_oc_shared=1;
	if(ltqusb_hcd_1.vbusgpioid>=0 && ltqusb_hcd_2.vbusgpioid>=0 && ltqusb_hcd_1.vbusgpioid==ltqusb_hcd_2.vbusgpioid)
		ltqusb_vbus_shared=1;
	if(ltqusb_hcd_1.ledgpioid>=0 && ltqusb_hcd_2.ledgpioid>=0 && ltqusb_hcd_1.ledgpioid==ltqusb_hcd_2.ledgpioid)
		ltqusb_led_shared=1;
	if(ltqusb_oc_shared && !ltqusb_vbus_shared)
		LTQ_PRINT("Wanring:  OverCurrent Detection shared with both port, but VBus control is not!!\n");
	if(!ltqusb_oc_shared && ltqusb_vbus_shared)
		LTQ_PRINT("Wanring:  OverCurrent Detection is not shared with both port, but VBus control is!!\n");

	oc1_int_no    = ltqusb_hcd_1.core_if.ocirq;
	oc2_int_no    = ltqusb_hcd_2.core_if.ocirq;
	oc1_int_target=&ltqusb_hcd_1;
	oc2_int_target=&ltqusb_hcd_2;

	// Parsing and store the parameters
	LTQ_DEBUGPL(DBG_ENTRY, "%s() %d\n", __func__, __LINE__ );
	parse_parms();

	if(ltqusb_hcd_1.core_if.irq)
	{
		ltqusb_hcd_1.dev=&_pdev->dev;

		retval = ltqusb_driver_probe_h(&ltqusb_hcd_1,
								ltqusb_hcd_1.core_if.irq,
								ltqusb_hcd_1.core_if.mem1,
								ltqusb_hcd_1.core_if.mem2,
								ltqusb_hcd_1.core_if.mem3);
		if(retval)
		{
			LTQ_ERROR("%s() usb probe() failed\n", __func__);
			ltqusb_hcd_1.dev=NULL;
			return (-ENOMEM);
		}
	}

	if(ltqusb_hcd_2.core_if.irq)
	{
		ltqusb_hcd_2.dev=&_pdev->dev;
		retval = ltqusb_driver_probe_h(&ltqusb_hcd_2,
								ltqusb_hcd_2.core_if.irq,
								ltqusb_hcd_2.core_if.mem1,
								ltqusb_hcd_2.core_if.mem2,
								ltqusb_hcd_2.core_if.mem3);
		if(retval)
		{
			LTQ_ERROR("%s() usb probe() failed\n", __func__);
			ltqusb_hcd_2.dev=NULL;
			if(ltqusb_hcd_1.dev)
			{
				ltqusb_hcd_1.dev=NULL;
			}
			return (-ENOMEM);
		}
	}
	ltqusb_attr_create_h(&_pdev->dev);
	return 0;
}


/*!
   \brief This function is called when the driver is removed from the kernel
  with the rmmod command. The driver unregisters itself with its bus
  driver.
*/
static const struct of_device_id ltqhcd_match[] = {
	{ .compatible = "lantiq,ltqusb"},
	{},
};

static struct platform_driver ltqhcd_driver = {
	.probe = ltqusb_driver_probe,
	.remove = ltqusb_driver_remove,
	.driver = {
		.name = "lantiq,ltqusb",
		.owner = THIS_MODULE,
		.of_match_table = ltqhcd_match,
	},
};

module_platform_driver(ltqhcd_driver);
MODULE_DESCRIPTION(USB_DRIVER_DESC);
MODULE_AUTHOR("Lantiq");
MODULE_LICENSE("GPL");






















// Parameters set when loaded
//static long  dbg_lvl =0xFFFFFFFF;
static long  dbg_lvl =0;
static short dma_burst_size =-1;
static short speed =-1;
static short  host_channels =-1;
static long  data_fifo_size =-1;
static long   rx_fifo_size =-1;
static long   nperio_tx_fifo_size =-1;
static long   perio_tx_fifo_size =-1;

static long   max_transfer_size =-1;
static long   max_packet_count =-1;
static long   phy_utmi_width =-1;
static long   timeout_cal  =-1;
static long   oc_hy =-1;

static long   ana_disconnect_threshold=-1;
static long   ana_squelch_threshold=-1;
static long   ana_transmitter_crossover=-1;
static long   ana_transmitter_impedance=-1;
static long   ana_transmitter_dc_voltage=-1;
static long   ana_transmitter_risefall_time=-1;
static long   ana_transmitter_pre_emphasis=-1;

/*!
   \brief Parsing the parameters taken when module load
*/
static void parse_parms(void)
{
	ltqusb_params_t *params;
	LTQ_DEBUGPL(DBG_ENTRY, "%s() %d\n", __func__, __LINE__ );
	h_dbg_lvl=dbg_lvl;
	params=&ltqusb_module_params_h;

	//Setting Default
	params->speed=default_param_speed;
	if(ltqusb_chip_family==LTQUSB_CHIP_DANUBE)
	{
	}
	if(ltqusb_chip_family==LTQUSB_CHIP_ASE)
	{
	}
	if(ltqusb_chip_family==LTQUSB_CHIP_AR9)
	{
		params->dma_burst_size     =AR9_param_dma_burst_size;
		params->max_transfer_size  =AR9_param_max_transfer_size;
		params->max_packet_count   =AR9_param_max_packet_count;
		params->phy_utmi_width     =AR9_param_phy_utmi_width;
		params->timeout_cal        =AR9_param_timeout_cal;
		params->host_channels      =AR9_param_host_channels;
		params->data_fifo_size     =AR9_param_data_fifo_size;
		params->rx_fifo_size       =AR9_param_rx_fifo_size;
		params->nperio_tx_fifo_size=AR9_param_nperio_tx_fifo_size;
		params->perio_tx_fifo_size =AR9_param_perio_tx_fifo_size;
		params->ana_disconnect_threshold=AR9_param_ana_disconnect_threshold;
		params->ana_squelch_threshold   =AR9_param_ana_squelch_threshold;
		params->ana_transmitter_crossover=AR9_param_ana_transmitter_crossover;
		params->ana_transmitter_impedance=AR9_param_ana_transmitter_impedance;
		params->ana_transmitter_dc_voltage=AR9_param_ana_transmitter_dc_voltage;
		params->ana_transmitter_risefall_time=AR9_param_ana_transmitter_risefall_time;
		params->ana_transmitter_pre_emphasis=AR9_param_ana_transmitter_pre_emphasis;
	}
	if(ltqusb_chip_family==LTQUSB_CHIP_VR9)
	{
		params->dma_burst_size     =VR9_param_dma_burst_size;
		{
			unsigned int chipid;
			unsigned int partnum;
			chipid=*((volatile uint32_t *)LTQ_MPS_CHIPID);
			partnum=(chipid&0x0FFFF000)>>12;
			switch(partnum)
			{
				case 0x000B: //VRX288_A2x
				case 0x000E: //VRX282_A2x
				case 0x000C: //VRX268_A2x
				case 0x000D: //GRX288_A2x
					params->dma_burst_size=VR9_param_dma_burst_size_n;
					break;
			}
			printk(KERN_INFO "Chip Version :%04x BurstSize=%d\n",partnum,params->dma_burst_size);
		}
		params->max_transfer_size  =VR9_param_max_transfer_size;
		params->max_packet_count   =VR9_param_max_packet_count;
		params->phy_utmi_width     =VR9_param_phy_utmi_width;
		params->timeout_cal        =VR9_param_timeout_cal;
		params->host_channels      =VR9_param_host_channels;
		params->data_fifo_size     =VR9_param_data_fifo_size;
		params->rx_fifo_size       =VR9_param_rx_fifo_size;
		params->nperio_tx_fifo_size=VR9_param_nperio_tx_fifo_size;
		params->perio_tx_fifo_size =VR9_param_perio_tx_fifo_size;
		params->ana_disconnect_threshold=VR9_param_ana_disconnect_threshold;
		params->ana_squelch_threshold   =VR9_param_ana_squelch_threshold;
		params->ana_transmitter_crossover=VR9_param_ana_transmitter_crossover;
		params->ana_transmitter_impedance=VR9_param_ana_transmitter_impedance;
		params->ana_transmitter_dc_voltage=VR9_param_ana_transmitter_dc_voltage;
		params->ana_transmitter_risefall_time=VR9_param_ana_transmitter_risefall_time;
		params->ana_transmitter_pre_emphasis=VR9_param_ana_transmitter_pre_emphasis;
	}
	if(ltqusb_chip_family==LTQUSB_CHIP_AR10)
	{
		params->dma_burst_size     =AR10_param_dma_burst_size;
		params->max_transfer_size  =AR10_param_max_transfer_size;
		params->max_packet_count   =AR10_param_max_packet_count;
		params->phy_utmi_width     =AR10_param_phy_utmi_width;
		params->oc_hy=AR10_param_oc_hy;
		params->timeout_cal        =AR10_param_timeout_cal;
		params->host_channels      =AR10_param_host_channels;
		params->data_fifo_size     =AR10_param_data_fifo_size;
		params->rx_fifo_size       =AR10_param_rx_fifo_size;
		params->nperio_tx_fifo_size=AR10_param_nperio_tx_fifo_size;
		params->perio_tx_fifo_size =AR10_param_perio_tx_fifo_size;
		params->ana_disconnect_threshold=AR10_param_ana_disconnect_threshold;
		params->ana_squelch_threshold   =AR10_param_ana_squelch_threshold;
		params->ana_transmitter_crossover=AR10_param_ana_transmitter_crossover;
		params->ana_transmitter_impedance=AR10_param_ana_transmitter_impedance;
		params->ana_transmitter_dc_voltage=AR10_param_ana_transmitter_dc_voltage;
		params->ana_transmitter_risefall_time=AR10_param_ana_transmitter_risefall_time;
		params->ana_transmitter_pre_emphasis=AR10_param_ana_transmitter_pre_emphasis;
	}

//////////////////////////////////////////////////////////////////////////////
	//Setting New
	switch(dma_burst_size)
	{
		case 0:
		case 1:
		case 4:
		case 8:
		case 16:
			params->dma_burst_size=dma_burst_size;
			break;
	}

	if(speed==0 || speed==1)
		params->speed=speed;
	if(max_transfer_size>=2048 && max_transfer_size<=65535)
		params->max_transfer_size=max_transfer_size;
	if(max_packet_count>=15 && max_packet_count<=511)
		params->max_packet_count=max_packet_count;
	switch(phy_utmi_width)
	{
		case 8:
		case 16:
			params->phy_utmi_width=phy_utmi_width;
			break;
	}

	if(ltqusb_chip_family==LTQUSB_CHIP_AR10)
	{
		if(oc_hy>=0 && oc_hy<=3)
			params->oc_hy=oc_hy;
		if(params->oc_hy>=0 && params->oc_hy<=3)
		{
			ltqusb_oc_set_hy(1,params->oc_hy);
			ltqusb_oc_set_hy(2,params->oc_hy);
		}
	}

	if(timeout_cal>=0 && timeout_cal<=7)
		params->timeout_cal=timeout_cal;
	if(host_channels>=1 && host_channels<=16)
		params->host_channels=host_channels;
	if(data_fifo_size>=32 && data_fifo_size<=32768)
		params->data_fifo_size=data_fifo_size;
	if(rx_fifo_size>=16 && rx_fifo_size<=32768)
		params->rx_fifo_size=rx_fifo_size;
	if(nperio_tx_fifo_size>=16 && nperio_tx_fifo_size<=32768)
		params->nperio_tx_fifo_size=nperio_tx_fifo_size;
	if(perio_tx_fifo_size>=16 && perio_tx_fifo_size<=32768)
		params->perio_tx_fifo_size=perio_tx_fifo_size;
	if(ana_disconnect_threshold>=0 && ana_disconnect_threshold<=7)
		params->ana_disconnect_threshold=ana_disconnect_threshold;
	if(ana_squelch_threshold>=0 && ana_squelch_threshold<=7)
		params->ana_squelch_threshold=ana_squelch_threshold;
	if(ana_transmitter_crossover>=0 && ana_transmitter_crossover<=3)
		params->ana_transmitter_crossover=ana_transmitter_crossover;
	if(ana_transmitter_impedance>=0 && ana_transmitter_impedance<=15)
		params->ana_transmitter_impedance=ana_transmitter_impedance;
	if(ana_transmitter_dc_voltage>=0 && ana_transmitter_dc_voltage<=15)
		params->ana_transmitter_dc_voltage=ana_transmitter_dc_voltage;
	if(ana_transmitter_risefall_time>=0 && ana_transmitter_risefall_time<=1)
		params->ana_transmitter_risefall_time=ana_transmitter_risefall_time;
	if(ana_transmitter_pre_emphasis>=0 && ana_transmitter_pre_emphasis<=1)
		params->ana_transmitter_pre_emphasis=ana_transmitter_pre_emphasis;
}



