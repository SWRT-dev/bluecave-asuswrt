/*****************************************************************************
 **   FILE NAME       : ltqusb_plat.h
 **   PROJECT         : Lantiq USB sub-system V3
 **   MODULES         : Lantiq USB sub-system Host and Device driver
 **   SRC VERSION     : 3.2
 **   DATE            : 1/Jan/2011
 **   AUTHOR          : Chen, Howard
 **   DESCRIPTION     : This file contains the Platform Specific constants, interfaces
 **                     (functions and macros).
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
  \defgroup LTQUSB_PLATEFORM_DEFINITION Platform Specific constants, interfaces (functions and macros).
  \ingroup LTQUSB_DRIVER_V3
  \brief Maintain plateform specific definitions and macros in this file.
         Each plateform has its own definition zone.
 */

/*!
  \defgroup LTQUSB_PLATEFORM_MEM_ADDR Definition of memory address and size and default parameters
  \ingroup LTQUSB_PLATEFORM_DEFINITION
 */

/*!
  \defgroup LTQUSB_DBG_ROUTINE Routines for debug message
  \ingroup LTQUSB_PLATEFORM_DEFINITION
 */


/*! \file ltqusb_plat.h
    \ingroup LTQUSB_DRIVER_V3
    \brief This file contains the Platform Specific constants, interfaces (functions and macros).
*/

#if !defined(__LTQUSB_PLAT_H__)
#define __LTQUSB_PLAT_H__


#include <linux/types.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/delay.h>
#include <asm/io.h>


#define LTQUSB_IOMEM_SIZE   0x00001000
#define LTQUSB_FIFOMEM_SIZE 0x00010000
#define LTQUSB_FIFODBG_SIZE 0x00020000



/*!
  \addtogroup LTQUSB_PLATEFORM_MEM_ADDR
 */
/*@{*/

#ifndef LTQ_MPS
	#define LTQ_MPS								(KSEG1+0x1F107000)
#endif
#ifndef LTQ_MPS_CHIPID
	#define LTQ_MPS_CHIPID						((volatile unsigned long *)(LTQ_MPS + 0x0344))
#endif


//////////////////////////////////////////////////////////////////////////////
////AR9 Section
//////////////////////////////////////////////////////////////////////////////
#define AR9_RCU_BASE_ADDR                (0xBF203000)
#define AR9_CGU                          (0xBF103000)
#define AR9_CGU_IFCCR                    ((volatile unsigned long *)(AR9_CGU+ 0x0018))
#define AR9_RCU_USB1CFG                  ((volatile unsigned long *)(AR9_RCU_BASE_ADDR + 0x18))
#define AR9_RCU_USB2CFG                  ((volatile unsigned long *)(AR9_RCU_BASE_ADDR + 0x34))
#define AR9_RCU_USBRESET                 ((volatile unsigned long *)(AR9_RCU_BASE_ADDR + 0x10))

#define AR9_USBCFG_ARB          7	//
#define AR9_USBCFG_HDSEL_BIT    11	// 0:host, 1:device
#define AR9_USBCFG_HOST_END_BIT 10	// 0:little_end, 1:big_end
#define AR9_USBCFG_SLV_END_BIT  17	// 0:little_end, 1:big_end

#define AR9_param_dma_burst_size      4

#define AR9_param_max_transfer_size   -1  //(Max, hwcfg)
#define AR9_param_max_packet_count    -1  //(Max, hwcfg)
#define AR9_param_phy_utmi_width      16

#define AR9_param_timeout_cal         -1 //(NoChange)

#define AR9_param_data_fifo_size      -1 //(Max, hwcfg)

#define AR9_param_host_channels       -1 //(Max, hwcfg)
#define AR9_param_rx_fifo_size        240
#define AR9_param_nperio_tx_fifo_size 240
#define AR9_param_perio_tx_fifo_size  32

#define AR9_param_ana_disconnect_threshold      -1
#define AR9_param_ana_squelch_threshold         -1
#define AR9_param_ana_transmitter_crossover     -1
#define AR9_param_ana_transmitter_impedance     -1
#define AR9_param_ana_transmitter_dc_voltage    -1
#define AR9_param_ana_transmitter_risefall_time -1
#define AR9_param_ana_transmitter_pre_emphasis  -1
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
////VR9 Section
//////////////////////////////////////////////////////////////////////////////
#define VR9_RCU_BASE_ADDR                    (0xBF203000)
#define VR9_RCU_USB1CFG   ((volatile unsigned long *)(VR9_RCU_BASE_ADDR + 0x18))
#define VR9_RCU_USB2CFG   ((volatile unsigned long *)(VR9_RCU_BASE_ADDR + 0x34))
#define VR9_RCU_USB_ANA_CFG1A  ((volatile unsigned long *)(VR9_RCU_BASE_ADDR + 0x38))
#define VR9_RCU_USB_ANA_CFG1B  ((volatile unsigned long *)(VR9_RCU_BASE_ADDR + 0x3C))
#define VR9_RCU_USBRESET  ((volatile unsigned long *)(VR9_RCU_BASE_ADDR + 0x10))
#define VR9_RCU_USBRESET2 ((volatile unsigned long *)(VR9_RCU_BASE_ADDR + 0x48))

#define VR9_USBCFG_ARB          7	//
#define VR9_USBCFG_HDSEL_BIT    11	// 0:host, 1:device
#define VR9_USBCFG_HOST_END_BIT 10	// 0:little_end, 1:big_end
#define VR9_USBCFG_SLV_END_BIT  9	// 0:little_end, 1:big_end

#define VR9_param_dma_burst_size   0      //(ALL)
#define VR9_param_dma_burst_size_n 4      //(ALL)

#define VR9_param_max_transfer_size -1  //(Max, hwcfg)
#define VR9_param_max_packet_count  -1  //(Max, hwcfg)
#define VR9_param_phy_utmi_width    16

#define VR9_param_timeout_cal         -1 //(NoChange)

#define VR9_param_data_fifo_size      -1 //(Max, hwcfg)

#define VR9_param_host_channels       -1 //(Max, hwcfg)
#define VR9_param_rx_fifo_size        240
#define VR9_param_nperio_tx_fifo_size 240
#define VR9_param_perio_tx_fifo_size  32

#define VR9_param_ana_disconnect_threshold      7
#define VR9_param_ana_squelch_threshold         -1
#define VR9_param_ana_transmitter_crossover     -1
#define VR9_param_ana_transmitter_impedance     -1
#define VR9_param_ana_transmitter_dc_voltage    -1
#define VR9_param_ana_transmitter_risefall_time -1
#if  defined(__PHY_LONG_PREEMP__)
	#define VR9_param_ana_transmitter_pre_emphasis  1
#else
	#define VR9_param_ana_transmitter_pre_emphasis  -1
#endif
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
////AR10 Section
//////////////////////////////////////////////////////////////////////////////
#define AR10_RCU_BASE_ADDR            (0xBF203000)

#define AR10_RCU_USB1CFG   ((volatile unsigned long *)(AR10_RCU_BASE_ADDR + 0x18))
#define AR10_RCU_USB2CFG   ((volatile unsigned long *)(AR10_RCU_BASE_ADDR + 0x34))
#define AR10_RCU_USB_ANA_CFG1A  ((volatile unsigned long *)(AR10_RCU_BASE_ADDR + 0x38))
#define AR10_RCU_USB_ANA_CFG1B  ((volatile unsigned long *)(AR10_RCU_BASE_ADDR + 0x3C))
#define AR10_RCU_USBRESET  ((volatile unsigned long *)(AR10_RCU_BASE_ADDR + 0x10))

#define AR10_USBCFG_ARB          7	//
#define AR10_USBCFG_HDSEL_BIT    11	// 0:host, 1:device
#define AR10_USBCFG_HOST_END_BIT 10	// 0:little_end, 1:big_end
#define AR10_USBCFG_SLV_END_BIT  9	// 0:little_end, 1:big_end
#define AR10_USBCFG_OC_EN_BIT    0	//
#define AR10_USBCFG_OC_HY_BIT    1	//
#define AR10_USBCFG_OC_HY_MASK   0x3

#define AR10_param_dma_burst_size 4      //(ALL)


#define AR10_param_max_transfer_size -1  //(Max, hwcfg)
#define AR10_param_max_packet_count  -1  //(Max, hwcfg)
#define AR10_param_phy_utmi_width    16

#define AR10_param_timeout_cal         -1 //(NoChange)

#define AR10_param_oc_hy               -1 //(NoChange)

#define AR10_param_data_fifo_size      -1 //(Max, hwcfg)

#define AR10_param_host_channels       -1 //(Max, hwcfg)
#define AR10_param_rx_fifo_size        240
#define AR10_param_nperio_tx_fifo_size 240
#define AR10_param_perio_tx_fifo_size  32

#define AR10_param_ana_disconnect_threshold      7
#define AR10_param_ana_squelch_threshold         -1
#define AR10_param_ana_transmitter_crossover     -1
#define AR10_param_ana_transmitter_impedance     -1
#define AR10_param_ana_transmitter_dc_voltage    -1
#define AR10_param_ana_transmitter_risefall_time -1
#if  defined(__PHY_LONG_PREEMP__)
	#define AR10_param_ana_transmitter_pre_emphasis  1
#else
	#define AR10_param_ana_transmitter_pre_emphasis  -1
#endif
//////////////////////////////////////////////////////////////////////////////

/*@}*//*LTQUSB_PLATEFORM_MEM_ADDR*/

/////////////////////////////////////////////////////////////////////////

#if defined(CONFIG_USB_HOST_LTQ_FORCE_USB11) || defined(__FORCE_USB11__)
	#define default_param_speed               LTQUSB_PARAM_SPEED_FULL
#else
	#define default_param_speed               LTQUSB_PARAM_SPEED_HIGH
#endif


/////////////////////////////////////////////////////////////////////////

static __inline__ void UDELAY( const uint32_t _usecs )
{
	udelay( _usecs );
}

static __inline__ void MDELAY( const uint32_t _msecs )
{
	mdelay( _msecs );
}

static __inline__ void SPIN_LOCK( spinlock_t *_lock )
{
	spin_lock(_lock);
}

static __inline__ void SPIN_UNLOCK( spinlock_t *_lock )
{
	spin_unlock(_lock);
}

#define SPIN_LOCK_IRQSAVE( _l, _f )  \
	{ \
	spin_lock_irqsave(_l,_f); \
	}

#define SPIN_UNLOCK_IRQRESTORE( _l,_f ) \
	{ \
	spin_unlock_irqrestore(_l,_f); \
	}

/////////////////////////////////////////////////////////////////////////
/*!
  \addtogroup LTQUSB_DBG_ROUTINE
 */
/*@{*/
extern uint32_t h_dbg_lvl;

/*! \brief When debug level has the DBG_CIL bit set, display CIL Debug messages. */
#define DBG_CIL		(0x2)
/*! \brief When debug level has the DBG_CILV bit set, display CIL Verbose debug messages */
#define DBG_CILV	(0x20)
/*! \brief When debug level has the DBG_PCD bit set, display PCD (Device) debug messages */
#define DBG_PCD		(0x4)
/*! \brief When debug level has the DBG_PCDV set, display PCD (Device) Verbose debug messages */
#define DBG_PCDV	(0x40)
/*! \brief When debug level has the DBG_HCD bit set, display Host debug messages */
#define DBG_HCD		(0x8)
/*! \brief When debug level has the DBG_HCDV bit set, display Verbose Host debug messages */
#define DBG_HCDV	(0x80)
/*! \brief When debug level has the DBG_HCD_URB bit set, display enqueued URBs in host mode. */
#define DBG_HCD_URB_CTRL     (0x0200)
#define DBG_HCD_URB_BULK_IN  (0x0400)
#define DBG_HCD_URB_BULK_OUT (0x0800)
#define DBG_HCD_URB_INTR_IN  (0x1000)
#define DBG_HCD_URB_INTR_OUT (0x2000)
#define DBG_HCD_URB_ISOC_IN  (0x4000)
#define DBG_HCD_URB_ISOC_OUT (0x8000)

/*! \brief When debug level has any bit set, display debug messages */
#define DBG_ANY		(0xFF)
/*! \brief All debug messages off */
#define DBG_OFF		0

#define DBG_ENTRY	(0x0100)

#define LTQUSB "LTQUSB: "

/*!
   \fn    inline uint32_t SET_DEBUG_LEVEL( const uint32_t _new )
   \brief Set the Debug Level variable.
   \param _new 32-bit mask of debug level.
   \return previous debug level
 */
static inline uint32_t SET_DEBUG_LEVEL( const uint32_t _new )
{
	uint32_t old = h_dbg_lvl;
	h_dbg_lvl = _new;
	return old;
}

#ifdef __DEBUG__
	# define LTQ_DEBUGPL(lvl, x...) do{ if ((lvl)&h_dbg_lvl)printk( KERN_DEBUG LTQUSB x ); }while(0)
	# define CHK_DEBUG_LEVEL(level) ((level) & h_dbg_lvl)

	# define LTQ_DEBUGP(x...)	LTQ_DEBUGPL(DBG_ANY, x )
#else
	# define LTQ_DEBUGPL(lvl, x...) do{}while(0)
	# define LTQ_DEBUGP(x...)
	# define CHK_DEBUG_LEVEL(level) (0)
#endif //__DEBUG__

/* Print an Error message. */
#define LTQ_ERROR(x...) printk( KERN_ERR LTQUSB x )
/* Print a Warning message. */
#define LTQ_WARN(x...) printk( KERN_WARNING LTQUSB x )
/* Print a notice (normal but significant message). */
#define LTQ_NOTICE(x...) printk( KERN_NOTICE LTQUSB x )
/*  Basic message printing. */
#define LTQ_PRINT(x...) printk( KERN_INFO LTQUSB x )

/*@}*//*LTQUSB_DBG_ROUTINE*/


#endif //__LTQUSB_PLAT_H__

