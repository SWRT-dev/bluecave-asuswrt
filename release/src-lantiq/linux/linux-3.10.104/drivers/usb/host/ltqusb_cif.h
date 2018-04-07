/*****************************************************************************
 **   FILE NAME       : ltqusb_cif.h
 **   PROJECT         : Lantiq USB sub-system V3
 **   MODULES         : Lantiq USB sub-system Host and Device driver
 **   SRC VERSION     : 3.2
 **   DATE            : 1/Jan/2011
 **   AUTHOR          : Chen, Howard
 **   DESCRIPTION     : The Core Interface provides basic services for accessing and
 **                     managing the Lantiq USB hardware. These services are used by both the
 **                     Host Controller Driver and the Peripheral Controller Driver.
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
 \defgroup LTQUSB_DRIVER_V3 Lantiq USB SS Project
 \brief Lantiq USB subsystem V3.x
 */

/*!
 \defgroup LTQUSB_CIF Core Interface APIs
 \ingroup LTQUSB_DRIVER_V3
 \brief The Core Interface provides basic services for accessing and
        managing the LTQUSB hardware. These services are used by both the
        Host Controller Driver and the Peripheral Controller Driver.
 */


/*!
 \file ltqusb_cif.h
 \ingroup LTQUSB_DRIVER_V3
 \brief This file contains the interface to the Lantiq USB Core.
 */

#if !defined(__LTQUSB_CIF_H__)
#define __LTQUSB_CIF_H__

#include <linux/workqueue.h>

#include <linux/version.h>
#include <asm/param.h>

#include "ltqusb_plat.h"
#include "ltqusb_regs.h"

#ifdef __DEBUG__
	#include "linux/timer.h"
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LTQUSB_PARAM_SPEED_HIGH 0 /*!< Build stage parameter: High Speed */
#define LTQUSB_PARAM_SPEED_FULL 1 /*!< Build stage parameter: Full Speed */

#define LTQUSB_EP_SPEED_LOW     0 /*!< Run-Time Status: High Speed */
#define LTQUSB_EP_SPEED_FULL    1 /*!< Run-Time Status: Full Speed */
#define LTQUSB_EP_SPEED_HIGH    2 /*!< Run-Time Status: Low Speed */

#define LTQUSB_EP_TYPE_CTRL     0 /*!< Run-Time Status: CTRL */
#define LTQUSB_EP_TYPE_ISOC     1 /*!< Run-Time Status: ISOC */
#define LTQUSB_EP_TYPE_BULK     2 /*!< Run-Time Status: BULK */
#define LTQUSB_EP_TYPE_INTR     3 /*!< Run-Time Status: INTR */

#define LTQUSB_HC_PID_DATA0     0     /*!< Run-Time Data Toggle: Data 0 */
#define LTQUSB_HC_PID_DATA2     1     /*!< Run-Time Data Toggle: Data 2 */
#define LTQUSB_HC_PID_DATA1     2     /*!< Run-Time Data Toggle: Data 1 */
#define LTQUSB_HC_PID_MDATA     3     /*!< Run-Time Data Toggle: MData */
#define LTQUSB_HC_PID_SETUP     3     /*!< Run-Time Data Toggle: Setup */


/*!
 \addtogroup LTQUSB_CIF
 */
/*@{*/

/*! typedef ltqusb_params_t
 \brief LTQUSB Parameters structure.
       This structure is used for both importing from insmod stage and run-time storage.
       These parameters define how the LTQUSB controller should be configured.
 */
typedef struct ltqusb_params
{
	int32_t dma_burst_size;     /*!< The DMA Burst size (applicable only for Internal DMA
	                                  Mode). 0(for single), 1(incr), 4(incr4), 8(incr8) 16(incr16)
	                             */
	                            /* Translate this to GAHBCFG values */
	int32_t speed;              /*!< Specifies the maximum speed of operation in host and device mode.
	                               The actual speed depends on the speed of the attached device and
	                               the value of phy_type. The actual speed depends on the speed of the
	                               attached device.
	                               0 - High Speed (default)
	                               1 - Full Speed
                                */

	int32_t data_fifo_size;     /*!< Total number of dwords in the data FIFO memory. This
	                               memory includes the Rx FIFO, non-periodic Tx FIFO, and periodic
	                               Tx FIFOs.
	                               32 to 32768
	                            */
	int32_t host_channels;      /*!< The number of host channel registers to use.
	                                 1 to 16
	                             */

	int32_t rx_fifo_size;       /*!< Number of dwords in the Rx FIFO in host mode.
	                                16 to 32768
	                             */

	int32_t nperio_tx_fifo_size;    /*!< Number of dwords in the non-periodic Tx FIFO in host mode.
	                                     16 to 32768
	                                 */

	int32_t perio_tx_fifo_size;     /*!< Number of dwords in the host periodic Tx FIFO.
	                                     16 to 32768
	                                 */

	int32_t max_transfer_size;      /*!< The maximum transfer size supported in bytes.
	                                     2047 to 65,535
	                                 */

	int32_t max_packet_count;       /*!< The maximum number of packets in a transfer.
	                                     15 to 511  (default 511)
	                                 */
	int32_t phy_utmi_width;         /*!< Specifies the UTMI+ Data Width.
	                                     8 or 16 bits (default 16)
	                                 */

	int32_t timeout_cal;            /*!< Specifies the Timeout_Calibration*/

	int32_t oc_hy;

	int32_t ana_disconnect_threshold      ;
	int32_t ana_squelch_threshold         ;
	int32_t ana_transmitter_crossover     ;
	int32_t ana_transmitter_impedance     ;
	int32_t ana_transmitter_dc_voltage    ;
	int32_t ana_transmitter_risefall_time ;
	int32_t ana_transmitter_pre_emphasis  ;
} ltqusb_params_t;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*! typedef ltqusb_core_if_t
 \brief The ltq_core_if structure contains information needed to manage
       the Lantiq USB controller acting in either host or device mode. It
       represents the programming view of the controller as a whole.
 */
typedef struct ltqusb_core_if
{
	ltqusb_params_t      params;  /*!< Run-time Parameters */

	uint8_t  core_no;             /*!< core number (used as id when multi-core case */
	char    *core_name;           /*!< core name used for registration and informative purpose*/
	int      irq;                 /*!< irq number this core is hooked */
	int      ocirq;
	uint32_t mem1,mem2,mem3;
	/*****************************************************************
	 * Structures and pointers to physical register interface.
	 *****************************************************************/
	/** Core Global registers starting at offset 000h. */
	ltqusb_core_global_regs_t *core_global_regs;  /*!< pointer to Core Global Registers, offset at 000h */

	/** Host-specific registers */
		/** Host Global Registers starting at offset 400h.*/
	ltqusb_host_global_regs_t *host_global_regs; /*!< pointer to Host Global Registers, offset at 400h */
		#define LTQUSB_HOST_GLOBAL_REG_OFFSET 0x400
	/** Host Port 0 Control and Status Register */
	volatile uint32_t *hprt0;                    /*!< pointer to HPRT0 Registers, offset at 440h */
		#define LTQUSB_HOST_PORT_REGS_OFFSET 0x440
	/** Host Channel Specific Registers at offsets 500h-5FCh. */
	ltqusb_hc_regs_t *hc_regs[MAX_EPS_CHANNELS]; /*!< pointer to Host-Channel n Registers, offset at 500h */
		#define LTQUSB_HOST_CHAN_REGS_OFFSET 0x500
		#define LTQUSB_CHAN_REGS_OFFSET 0x20

	/** Power and Clock Gating Control Register */
	volatile uint32_t *pcgcctl;                                    /*!< pointer to Power and Clock Gating Control Registers, offset at E00h */
		#define LTQUSB_PCGCCTL_OFFSET 0xE00

	/** Push/pop addresses for endpoints or host channels.*/
	uint32_t *data_fifo[MAX_EPS_CHANNELS];    /*!< pointer to FIFO access windows, offset at 1000h */
		#define LTQUSB_DATA_FIFO_OFFSET 0x1000
		#define LTQUSB_DATA_FIFO_SIZE   0x1000

	uint32_t *data_fifo_dbg;                 /*!< pointer to FIFO debug windows, offset at 1000h */

	/** Hardware Configuration -- stored here for convenience.*/
	hwcfg1_data_t hwcfg1;  /*!< preserved Hardware Configuration 1 */
	hwcfg2_data_t hwcfg2;  /*!< preserved Hardware Configuration 2 */
	hwcfg3_data_t hwcfg3;  /*!< preserved Hardware Configuration 3 */
	hwcfg4_data_t hwcfg4;  /*!< preserved Hardware Configuration 3 */
	uint32_t      snpsid;  /*!< preserved SNPSID */

	/*****************************************************************
	 * Run-time informations.
	 *****************************************************************/
	/* Set to 1 if the core PHY interface bits in USBCFG have been  initialized. */
	unsigned phy_init_done     : 1 ;/*!< indicated PHY is initialized. */
	unsigned issuspended       : 1 ;

	uint8_t queuing_high_bandwidth; /*!< Host mode, Queueing High Bandwidth. */

	#if defined(__UNALIGNED_BUF_ADJ__) || defined(__UNALIGNED_BUF_CHK__)
		uint32_t unaligned_mask;
	#endif
} ltqusb_core_if_t;

/*@}*//*LTQUSB_CIF*/


/*!
 \fn    void *ltqusb_alloc_buf(size_t size, int clear)
 \brief This function is called to allocate buffer of specified size.
        The allocated buffer is mapped into DMA accessable address.
 \param    size Size in BYTE to be allocated
 \param    clear 0: don't do clear after buffer allocated, other: do clear to zero
 \return   0/NULL: Fail; uncached pointer of allocated buffer
 \ingroup  LTQUSB_CIF
 */
extern void *ltqusb_alloc_buf_h(size_t size, int clear);


/*!
 \fn    void ltqusb_free_buf(void *vaddr)
 \brief This function is called to free allocated buffer.
 \param vaddr the uncached pointer of the buffer
 \ingroup  LTQUSB_CIF
 */
extern void ltqusb_free_buf_h(void *vaddr);

/*!
 \fn    int ltqusb_core_if_init(ltqusb_core_if_t *_core_if,
                        int               _irq,
                        uint32_t          _reg_base_addr,
                        uint32_t          _fifo_base_addr,
                        uint32_t          _fifo_dbg_addr)
 \brief This function is called to initialize the LTQUSB CSR data
        structures.  The register addresses in the device and host
        structures are initialized from the base address supplied by the
        caller.  The calling function must make the OS calls to get the
        base address of the LTQUSB controller registers.
 \param _core_if        Pointer of core_if structure
 \param _irq            irq number
 \param _reg_base_addr  Base address of LTQUSB core registers
 \param _fifo_base_addr Fifo base address
 \param _fifo_dbg_addr  Fifo debug address
 \return 0: success;
 \ingroup  LTQUSB_CIF
 */
extern int ltqusb_core_if_init_h(ltqusb_core_if_t *_core_if,
                        int               _irq,
                        uint32_t          _reg_base_addr,
                        uint32_t          _fifo_base_addr,
                        uint32_t          _fifo_dbg_addr);


/*!
 \fn    void ltqusb_core_if_remove(ltqusb_core_if_t *_core_if)
 \brief This function free the mapped address in the LTQUSB CSR data structures.
 \param _core_if Pointer of core_if structure
 \ingroup  LTQUSB_CIF
 */
extern void ltqusb_core_if_remove_h(ltqusb_core_if_t *_core_if);

/*!
 \fn    void ltqusb_enable_global_interrupts( ltqusb_core_if_t *_core_if )
 \brief This function enbles the controller's Global Interrupt in the AHB Config register.
 \param _core_if Pointer of core_if structure
 */
extern void ltqusb_enable_global_interrupts_h( ltqusb_core_if_t *_core_if );

/*!
 \fn    void ltqusb_disable_global_interrupts( ltqusb_core_if_t *_core_if )
 \brief This function disables the controller's Global Interrupt in the AHB Config register.
 \param _core_if Pointer of core_if structure
 \ingroup  LTQUSB_CIF
 */
extern void ltqusb_disable_global_interrupts_h( ltqusb_core_if_t *_core_if );

/*!
 \fn    void ltqusb_flush_tx_fifo( ltqusb_core_if_t *_core_if, const int _num )
 \brief Flush a Tx FIFO.
 \param _core_if Pointer of core_if structure
 \param _num Tx FIFO to flush. ( 0x10 for ALL TX FIFO )
 \ingroup  LTQUSB_CIF
 */
extern void ltqusb_flush_tx_fifo_h( ltqusb_core_if_t *_core_if, const int _num );

/*!
 \fn    void ltqusb_flush_rx_fifo( ltqusb_core_if_t *_core_if )
 \brief Flush Rx FIFO.
 \param _core_if Pointer of core_if structure
 \ingroup  LTQUSB_CIF
 */
extern void ltqusb_flush_rx_fifo_h( ltqusb_core_if_t *_core_if );

/*!
 \fn    void ltqusb_flush_both_fifo( ltqusb_core_if_t *_core_if )
 \brief Flush ALL Rx and Tx FIFO.
 \param _core_if Pointer of core_if structure
 \ingroup  LTQUSB_CIF
 */
extern void ltqusb_flush_both_fifo_h( ltqusb_core_if_t *_core_if );


/*!
 \fn    int ltqusb_core_soft_reset(ltqusb_core_if_t *_core_if)
 \brief Do core a soft reset of the core.  Be careful with this because it
        resets all the internal state machines of the core.
 \param    _core_if Pointer of core_if structure
 \ingroup  LTQUSB_CIF
 */
extern int ltqusb_core_soft_reset_h(ltqusb_core_if_t *_core_if);


/*!
 \brief Turn on the USB Core Power
 \param _core_if Pointer of core_if structure
 \ingroup  LTQUSB_CIF
*/
extern void ltqusb_power_on_h (ltqusb_core_if_t *_core_if);

/*!
 \fn    void ltqusb_power_off (ltqusb_core_if_t *_core_if)
 \brief Turn off the USB Core Power
 \param _core_if Pointer of core_if structure
 \ingroup  LTQUSB_CIF
*/
extern void ltqusb_power_off_h (ltqusb_core_if_t *_core_if);

/*!
 \fn    void ltqusb_phy_power_on (ltqusb_core_if_t *_core_if)
 \brief Turn on the USB PHY Power
 \param _core_if Pointer of core_if structure
 \ingroup  LTQUSB_CIF
*/
extern void ltqusb_phy_power_on_h (ltqusb_core_if_t *_core_if);


/*!
 \fn    void ltqusb_phy_power_off (ltqusb_core_if_t *_core_if)
 \brief Turn off the USB PHY Power
 \param _core_if Pointer of core_if structure
 \ingroup  LTQUSB_CIF
*/
extern void ltqusb_phy_power_off_h (ltqusb_core_if_t *_core_if);

/*!
 \fn    void ltqusb_hard_reset(ltqusb_core_if_t *_core_if)
 \brief Reset on the USB Core RCU
 \param _core_if Pointer of core_if structure
 \ingroup  LTQUSB_CIF
 */
extern void ltqusb_hard_reset_h(ltqusb_core_if_t *_core_if);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	/*!
	 \fn    void ltqusb_host_core_init(ltqusb_core_if_t *_core_if, ltqusb_params_t  *_params)
	 \brief This function initializes the LTQUSB controller registers for  Host mode.
	        This function flushes the Tx and Rx FIFOs and it flushes any entries in the
	        request queues.
	 \param _core_if        Pointer of core_if structure
	 \param _params         parameters to be set
	 \ingroup  LTQUSB_CIF
	 */
	extern void ltqusb_host_core_init(ltqusb_core_if_t *_core_if, ltqusb_params_t  *_params);

	/*!
	 \fn    void ltqusb_host_enable_interrupts(ltqusb_core_if_t *_core_if)
	 \brief This function enables the Host mode interrupts.
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
	extern void ltqusb_host_enable_interrupts(ltqusb_core_if_t *_core_if);

	/*!
	 \fn    void ltqusb_host_disable_interrupts(ltqusb_core_if_t *_core_if)
	 \brief This function disables the Host mode interrupts.
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
	extern void ltqusb_host_disable_interrupts(ltqusb_core_if_t *_core_if);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* internal routines for debugging */
	extern void ltqusb_dump_msg_h(const u8 *buf, unsigned int length);
	extern void ltqusb_dump_spram_h(ltqusb_core_if_t *_core_if);
	extern void ltqusb_dump_registers_h(ltqusb_core_if_t *_core_if);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static inline uint32_t ltqusb_read_core_intr(ltqusb_core_if_t *_core_if)
{
	return (ltqusb_rreg(&_core_if->core_global_regs->gintsts) &
	        ltqusb_rreg(&_core_if->core_global_regs->gintmsk));
}

static inline uint32_t ltqusb_read_otg_intr (ltqusb_core_if_t *_core_if)
{
	return (ltqusb_rreg (&_core_if->core_global_regs->gotgint));
}

static inline uint32_t ltqusb_mode(ltqusb_core_if_t *_core_if)
{
	return (ltqusb_rreg( &_core_if->core_global_regs->gintsts ) & 0x1);
}
static inline uint8_t ltqusb_is_device_mode(ltqusb_core_if_t *_core_if)
{
	return (ltqusb_mode(_core_if) != 1);
}
static inline uint8_t ltqusb_is_host_mode(ltqusb_core_if_t *_core_if)
{
	return (ltqusb_mode(_core_if) == 1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static inline uint32_t ltqusb_read_hprt0(ltqusb_core_if_t *_core_if)
{
	hprt0_data_t hprt0;
	hprt0.d32 = ltqusb_rreg(_core_if->hprt0);
	hprt0.b.prtena = 0;
	hprt0.b.prtconndet = 0;
	hprt0.b.prtenchng = 0;
	hprt0.b.prtovrcurrchng = 0;
	return hprt0.d32;
}

static inline uint32_t ltqusb_read_host_all_channels_intr (ltqusb_core_if_t *_core_if)
{
	return (ltqusb_rreg (&_core_if->host_global_regs->haint));
}

static inline uint32_t ltqusb_read_host_channel_intr (ltqusb_core_if_t *_core_if, int hc_num)
{
	return (ltqusb_rreg (&_core_if->hc_regs[hc_num]->hcint));
}

extern void ltqusb_attr_create_h (void *_dev);
extern void ltqusb_attr_remove_h (void *_dev);

extern uint32_t ltqusb_oc_get_hy(int port);
extern void ltqusb_oc_set_hy(int port,uint32_t setting);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // !defined(__LTQUSB_CIF_H__)

