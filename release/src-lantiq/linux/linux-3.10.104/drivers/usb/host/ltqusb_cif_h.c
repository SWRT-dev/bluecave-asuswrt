/*****************************************************************************
 **   FILE NAME       : ltqusb_cif_h.c
 **   PROJECT         : Lantiq USB sub-system V3
 **   MODULES         : Lantiq USB sub-system Host and Device driver
 **   SRC VERSION     : 3.2
 **   DATE            : 1/Jan/2011
 **   AUTHOR          : Chen, Howard
 **   DESCRIPTION     : The Core Interface provides basic services for accessing and
 **                     managing the Lantiq USB hardware. These services are used by the
 **                     Host Controller Driver only.
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
 \file ltqusb_cif_h.c
 \ingroup LTQUSB_DRIVER_V3
 \brief This file contains the interface to the Lantiq USB Core.
*/
#include <linux/version.h>
#include "ltqusb_version.h"

#include <asm/byteorder.h>
#include <asm/unaligned.h>

#ifdef __DEBUG__
	#include <linux/jiffies.h>
#endif
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/mm.h>
#include <linux/gfp.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include "ltqusb_plat.h"
#include "ltqusb_regs.h"
#include "ltqusb_cif.h"

#include "ltqhcd.h"

extern ltqhcd_hcd_t ltqusb_hcd_1;
extern ltqhcd_hcd_t ltqusb_hcd_2;
extern int ltqusb_oc_shared;
extern int ltqusb_vbus_shared;
extern int ltqusb_led_shared;

/*!
 \brief This function is called to allocate buffer of specified size.
        The allocated buffer is mapped into DMA accessable address.
 \param size Size in BYTE to be allocated
 \param clear 0: don't do clear after buffer allocated, other: do clear to zero
 \return 0/NULL: Fail; uncached pointer of allocated buffer
 */
void *ltqusb_alloc_buf_h(size_t size, int clear)
{
	uint32_t *cached,*uncached;
	uint32_t totalsize,page;

	if(!size)
		return 0;

	size=(size+3)&0xFFFFFFFC;
	totalsize=size + 12;
	page=get_order(totalsize);

	cached = (void *) __get_free_pages(( GFP_ATOMIC | GFP_DMA), page);

	if(!cached)
	{
		LTQ_PRINT("%s Allocation Failed size:%d\n",__func__,size);
		return NULL;
	}

	uncached = (uint32_t *)(KSEG1ADDR(cached));
	if(clear)
		memset(uncached, 0, totalsize);

	*(uncached+0)=totalsize;
	*(uncached+1)=page;
	*(uncached+2)=(uint32_t)cached;
	return (void *)(uncached+3);
}


/*!
 \brief This function is called to free allocated buffer.
 \param vaddr the uncached pointer of the buffer
 */
void ltqusb_free_buf_h(void *vaddr)
{
	uint32_t totalsize,page;
	uint32_t *cached,*uncached;

	if(vaddr != NULL)
	{
		uncached=vaddr;
		uncached-=3;
		totalsize=*(uncached+0);
		page=*(uncached+1);
		cached=(uint32_t *)(*(uncached+2));
		if(totalsize && page==get_order(totalsize) && cached==(uint32_t *)(KSEG0ADDR(uncached)))
		{
			free_pages((unsigned long)cached, page);
			return;
		}
		// the memory is not allocated by ltqusb_alloc_buf. Allowed but must be careful.
		return;
	}
}



/*!
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
   \return 	0: success;
 */
int ltqusb_core_if_init_h(ltqusb_core_if_t *_core_if,
                        int               _irq,
                        uint32_t          _reg_base_addr,
                        uint32_t          _fifo_base_addr,
                        uint32_t          _fifo_dbg_addr)
{
	int retval = 0;
	uint32_t *reg_base  =NULL;
    uint32_t *fifo_base =NULL;
    uint32_t *fifo_dbg  =NULL;

    int i;

	LTQ_DEBUGPL(DBG_CILV, "%s(%p,%d,0x%08X,0x%08X,0x%08X)\n", __func__,
	                                             _core_if,
	                                             _irq,
	                                             _reg_base_addr,
	                                             _fifo_base_addr,
	                                             _fifo_dbg_addr);

	if( _core_if == NULL)
	{
		LTQ_ERROR("%s() invalid _core_if\n", __func__);
		retval = -ENOMEM;
		goto fail;
	}

	//memset(_core_if, 0, sizeof(ltqusb_core_if_t));

	_core_if->irq=_irq;

	reg_base  =ioremap_nocache(_reg_base_addr , LTQUSB_IOMEM_SIZE  );
	fifo_base =ioremap_nocache(_fifo_base_addr, LTQUSB_FIFOMEM_SIZE);
	fifo_dbg  =ioremap_nocache(_fifo_dbg_addr , LTQUSB_FIFODBG_SIZE);
	if( reg_base == NULL || fifo_base == NULL || fifo_dbg == NULL)
	{
		LTQ_ERROR("%s() usb ioremap() failed\n", __func__);
		retval = -ENOMEM;
		goto fail;
	}

	_core_if->core_global_regs = (ltqusb_core_global_regs_t *)reg_base;

	/*
	 * Attempt to ensure this device is really a LTQUSB Controller.
	 * Read and verify the SNPSID register contents. The value should be
	 * 0x45F42XXX
	 */
	{
		int32_t snpsid;
		snpsid = ltqusb_rreg(&_core_if->core_global_regs->gsnpsid);
		if ((snpsid & 0xFFFFF000) != 0x4F542000)
		{
			LTQ_ERROR("%s() snpsid error(0x%08x) failed\n", __func__,snpsid);
			retval = -EINVAL;
			goto fail;
		}
		_core_if->snpsid=snpsid;
	}

	_core_if->host_global_regs = (ltqusb_host_global_regs_t *)
	    ((uint32_t)reg_base + LTQUSB_HOST_GLOBAL_REG_OFFSET);
	_core_if->hprt0 = (uint32_t*)((uint32_t)reg_base + LTQUSB_HOST_PORT_REGS_OFFSET);

	for (i=0; i<MAX_EPS_CHANNELS; i++)
	{
		_core_if->hc_regs[i] = (ltqusb_hc_regs_t *)
		    ((uint32_t)reg_base + LTQUSB_HOST_CHAN_REGS_OFFSET +
		    (i * LTQUSB_CHAN_REGS_OFFSET));
		LTQ_DEBUGPL(DBG_CILV, "hc_reg[%d]->hcchar=%p\n",
		    i, &_core_if->hc_regs[i]->hcchar);
	}

	/* Setting the FIFO and other Address. */
	for (i=0; i<MAX_EPS_CHANNELS; i++)
	{
		_core_if->data_fifo[i] = fifo_base + (i * LTQUSB_DATA_FIFO_SIZE);
		LTQ_DEBUGPL(DBG_CILV, "data_fifo[%d]=0x%08x\n",
		    i, (unsigned)_core_if->data_fifo[i]);
	}

	_core_if->data_fifo_dbg = fifo_dbg;
	_core_if->pcgcctl = (uint32_t*)(((uint32_t)reg_base) + LTQUSB_PCGCCTL_OFFSET);

	/*
	 * Store the contents of the hardware configuration registers here for
	 * easy access later.
	 */
	_core_if->hwcfg1.d32 = ltqusb_rreg(&_core_if->core_global_regs->ghwcfg1);
	_core_if->hwcfg2.d32 = ltqusb_rreg(&_core_if->core_global_regs->ghwcfg2);
	_core_if->hwcfg3.d32 = ltqusb_rreg(&_core_if->core_global_regs->ghwcfg3);
	_core_if->hwcfg4.d32 = ltqusb_rreg(&_core_if->core_global_regs->ghwcfg4);

	LTQ_DEBUGPL(DBG_CILV,"hwcfg1=%08x\n",_core_if->hwcfg1.d32);
	LTQ_DEBUGPL(DBG_CILV,"hwcfg2=%08x\n",_core_if->hwcfg2.d32);
	LTQ_DEBUGPL(DBG_CILV,"hwcfg3=%08x\n",_core_if->hwcfg3.d32);
	LTQ_DEBUGPL(DBG_CILV,"hwcfg4=%08x\n",_core_if->hwcfg4.d32);


	#ifdef __DED_FIFO__
	{
		unsigned int countdown=0xFFFF;
		LTQ_PRINT("Waiting for PHY Clock Lock!\n");
		while(--countdown && !( ltqusb_rreg(&_core_if->core_global_regs->grxfsiz) & (1<<9)))
		{
			UDELAY(1);
		}
		if(countdown)
			LTQ_PRINT("PHY Clock Locked!\n");
		else
			LTQ_PRINT("PHY Clock Not Locked! %08X\n",ltqusb_rreg(&_core_if->core_global_regs->grxfsiz));
	}
	#endif

	/* Create new workqueue and init works */
#if 0
	_core_if->wq_usb = create_singlethread_workqueue(_core_if->core_name);

	if(_core_if->wq_usb == 0)
	{
		LTQ_DEBUGPL(DBG_CIL, "Creation of wq_usb failed\n");
		retval = -EINVAL;
		goto fail;
	}

	#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
		INIT_WORK(&core_if->w_conn_id, w_conn_id_status_change, core_if);
		INIT_WORK(&core_if->w_wkp, w_wakeup_detected, core_if);
	#else
		INIT_WORK(&core_if->w_conn_id, w_conn_id_status_change);
		INIT_DELAYED_WORK(&core_if->w_wkp, w_wakeup_detected);
	#endif
#endif
	return 0;

fail:
	if( reg_base  != NULL) iounmap(reg_base );
	if( fifo_base != NULL) iounmap(fifo_base);
	if( fifo_dbg  != NULL) iounmap(fifo_dbg );
	return retval;
}

/*!
 \brief This function free the mapped address in the LTQUSB CSR data structures.
 \param _core_if Pointer of core_if structure
 */
void ltqusb_core_if_remove_h(ltqusb_core_if_t *_core_if)
{
	/* Disable all interrupts */
	if( _core_if->core_global_regs  != NULL)
	{
		gusbcfg_data_t usbcfg   ={.d32 = 0};
		usbcfg.d32 = ltqusb_rreg( &_core_if->core_global_regs->gusbcfg);
		usbcfg.b.ForceDevMode=0;
		usbcfg.b.ForceHstMode=0;
		ltqusb_wreg( &_core_if->core_global_regs->gusbcfg,usbcfg.d32);
		ltqusb_mreg( &_core_if->core_global_regs->gahbcfg, 1, 0);
		ltqusb_wreg( &_core_if->core_global_regs->gintmsk, 0);
	}

	if( _core_if->core_global_regs  != NULL) iounmap(_core_if->core_global_regs );
	if( _core_if->data_fifo[0]      != NULL) iounmap(_core_if->data_fifo[0]     );
	if( _core_if->data_fifo_dbg     != NULL) iounmap(_core_if->data_fifo_dbg    );

#if 0
	if (_core_if->wq_usb)
		destroy_workqueue(_core_if->wq_usb);
#endif
	memset(_core_if, 0, sizeof(ltqusb_core_if_t));
}




/*!
 \brief This function enbles the controller's Global Interrupt in the AHB Config register.
 \param _core_if Pointer of core_if structure
 */
void ltqusb_enable_global_interrupts_h( ltqusb_core_if_t *_core_if )
{
	gahbcfg_data_t ahbcfg ={ .d32 = 0};
	ahbcfg.b.glblintrmsk = 1; /* Enable interrupts */
	ltqusb_mreg(&_core_if->core_global_regs->gahbcfg, 0, ahbcfg.d32);
}

/*!
 \brief This function disables the controller's Global Interrupt in the AHB Config register.
 \param _core_if Pointer of core_if structure
 */
void ltqusb_disable_global_interrupts_h( ltqusb_core_if_t *_core_if )
{
	gahbcfg_data_t ahbcfg ={ .d32 = 0};
	ahbcfg.b.glblintrmsk = 1; /* Enable interrupts */
	ltqusb_mreg(&_core_if->core_global_regs->gahbcfg, ahbcfg.d32, 0);
}




/*!
 \brief Flush Tx and Rx FIFO.
 \param _core_if Pointer of core_if structure
 */
void ltqusb_flush_both_fifo_h( ltqusb_core_if_t *_core_if )
{
	ltqusb_core_global_regs_t *global_regs = _core_if->core_global_regs;
	volatile grstctl_t greset ={ .d32 = 0};
	int count = 0;

	LTQ_DEBUGPL((DBG_CIL|DBG_PCDV), "%s\n", __func__);
	greset.b.rxfflsh = 1;
	greset.b.txfflsh = 1;
	greset.b.txfnum = 0x10;
	greset.b.intknqflsh=1;
	greset.b.hstfrm=1;
	ltqusb_wreg( &global_regs->grstctl, greset.d32 );

	do
	{
		greset.d32 = ltqusb_rreg( &global_regs->grstctl);
		if (++count > 10000)
		{
			LTQ_WARN("%s() HANG! GRSTCTL=%0x\n", __func__, greset.d32);
			break;
		}
	} while (greset.b.rxfflsh == 1 || greset.b.txfflsh == 1);
	/* Wait for 3 PHY Clocks*/
	UDELAY(1);
}

/*!
 \brief Flush a Tx FIFO.
 \param _core_if Pointer of core_if structure
 \param _num Tx FIFO to flush. ( 0x10 for ALL TX FIFO )
 */
void ltqusb_flush_tx_fifo_h( ltqusb_core_if_t *_core_if, const int _num )
{
	ltqusb_core_global_regs_t *global_regs = _core_if->core_global_regs;
	volatile grstctl_t greset ={ .d32 = 0};
	int count = 0;

	LTQ_DEBUGPL((DBG_CIL|DBG_PCDV), "Flush Tx FIFO %d\n", _num);

	greset.b.intknqflsh=1;
	greset.b.txfflsh = 1;
	greset.b.txfnum = _num;
	ltqusb_wreg( &global_regs->grstctl, greset.d32 );

	do
	{
		greset.d32 = ltqusb_rreg( &global_regs->grstctl);
		if (++count > 10000&&(_num==0 ||_num==0x10))
		{
			LTQ_WARN("%s() HANG! GRSTCTL=%0x GNPTXSTS=0x%08x\n",
			    __func__, greset.d32,
			ltqusb_rreg( &global_regs->gnptxsts));
			break;
		}
	} while (greset.b.txfflsh == 1);
	/* Wait for 3 PHY Clocks*/
	UDELAY(1);
}


/*!
 \brief Flush Rx FIFO.
 \param _core_if Pointer of core_if structure
 */
void ltqusb_flush_rx_fifo_h( ltqusb_core_if_t *_core_if )
{
	ltqusb_core_global_regs_t *global_regs = _core_if->core_global_regs;
	volatile grstctl_t greset ={ .d32 = 0};
	int count = 0;

	LTQ_DEBUGPL((DBG_CIL|DBG_PCDV), "%s\n", __func__);
	greset.b.rxfflsh = 1;
	ltqusb_wreg( &global_regs->grstctl, greset.d32 );

	do
	{
		greset.d32 = ltqusb_rreg( &global_regs->grstctl);
		if (++count > 10000)
		{
			LTQ_WARN("%s() HANG! GRSTCTL=%0x\n", __func__, greset.d32);
			break;
		}
	} while (greset.b.rxfflsh == 1);
	/* Wait for 3 PHY Clocks*/
	UDELAY(1);
}


#define SOFT_RESET_DELAY 100 /*!< Delay in msec of detection after soft-reset of usb core */

/*!
 \brief Do a soft reset of the core.  Be careful with this because it
        resets all the internal state machines of the core.
 \param _core_if Pointer of core_if structure
 */
int ltqusb_core_soft_reset_h(ltqusb_core_if_t *_core_if)
{
	ltqusb_core_global_regs_t *global_regs = _core_if->core_global_regs;
	volatile grstctl_t greset ={ .d32 = 0};
	int count = 0;

	LTQ_DEBUGPL(DBG_CILV, "%s\n", __func__);
	/* Wait for AHB master IDLE state. */
	do
	{
		UDELAY(10);
		greset.d32 = ltqusb_rreg( &global_regs->grstctl);
		if (++count > 100000)
		{
			LTQ_WARN("%s() HANG! AHB Idle GRSTCTL=%0x %x\n", __func__,
			greset.d32, greset.b.ahbidle);
			break;
		}
	} while (greset.b.ahbidle == 0);

	UDELAY(1);

	/* Core Soft Reset */
	count = 0;
	greset.b.csftrst = 1;
	ltqusb_wreg( &global_regs->grstctl, greset.d32 );

	#ifdef SOFT_RESET_DELAY
		MDELAY(SOFT_RESET_DELAY);
	#endif

	do
	{
		UDELAY(10);
		greset.d32 = ltqusb_rreg( &global_regs->grstctl);
		if (++count > 100000)
		{
			LTQ_WARN("%s() HANG! Soft Reset GRSTCTL=%0x\n", __func__, greset.d32);
			return -1;
		}
	} while (greset.b.csftrst == 1);

	#ifdef SOFT_RESET_DELAY
		MDELAY(SOFT_RESET_DELAY);
	#endif

	// This is to reset the PHY of VR9
	if(ltqusb_chip_family== LTQUSB_CHIP_VR9)
	{
		if(_core_if->core_no==0)
		{
			set_bit (4, VR9_RCU_USBRESET2);
			MDELAY(50);
			clear_bit (4, VR9_RCU_USBRESET2);
		}
		else
		{
			set_bit (5, VR9_RCU_USBRESET2);
			MDELAY(50);
			clear_bit (5, VR9_RCU_USBRESET2);
		}
		MDELAY(50);
	}

	LTQ_PRINT("USB core #%d soft-reset\n",_core_if->core_no);

	return 0;
}

void USB_AHB_PMU_SETUP(int OnOff)
{
	struct clk *clk = NULL;
	if     (ltqusb_chip_family== LTQUSB_CHIP_DANUBE)
	{
//		clk = clk_get_sys(NULL, "ahb");
	}
	else if(ltqusb_chip_family== LTQUSB_CHIP_ASE)
	{
//		clk = clk_get_sys(NULL, "ahb");
	}
	else if(ltqusb_chip_family== LTQUSB_CHIP_AR9)
	{
//		clk = clk_get_sys(NULL, "ahb");
	}
	else if(ltqusb_chip_family== LTQUSB_CHIP_VR9)
	{
		clk = clk_get_sys(NULL, "ahb");
	}
	else if(ltqusb_chip_family== LTQUSB_CHIP_AR10)
	{
//		clk = clk_get_sys(NULL, "ahb");
	}
	if (clk && !IS_ERR(clk))
		clk_enable(clk);// Enable only
}

void USB_CTRL_PMU_SETUP(int core,int OnOff)
{
	struct clk *clk = NULL;
	switch(ltqusb_chip_family)
	{
		case LTQUSB_CHIP_DANUBE:
		case LTQUSB_CHIP_ASE:
			clk = clk_get_sys("1e101000.usb", "ctl");
			if (!clk || IS_ERR(clk))
				clk = clk_get_sys("1e101000.ltqhcd", "ctl");
			if (!clk || IS_ERR(clk))
				clk = clk_get_sys("1e101000.ifxhcd", "ctl");
			break;
		case LTQUSB_CHIP_AR9:
		case LTQUSB_CHIP_VR9:
		case LTQUSB_CHIP_AR10:
			if(core==0)
			{
				clk = clk_get_sys("1e101000.usb", "ctl");
				if (!clk || IS_ERR(clk))
					clk = clk_get_sys("1e101000.ltqhcd", "ctl");
				if (!clk || IS_ERR(clk))
					clk = clk_get_sys("1e101000.ifxhcd", "ctl");
			}
			else
			{
				clk = clk_get_sys("1e106000.usb", "ctl");
				if (!clk || IS_ERR(clk))
					clk = clk_get_sys("1e106000.ltqhcd", "ctl");
				if (!clk || IS_ERR(clk))
					clk = clk_get_sys("1e106000.ifxhcd1", "ctl");
			}
			break;
		default:
			clk=NULL;
	}
	if (clk && !IS_ERR(clk))
	{
		if(OnOff)
			clk_enable(clk);
		else
			clk_disable(clk);
	}
}
void USB_PHY_PMU_SETUP(int core,int OnOff)
{
	struct clk *clk = NULL;
	switch(ltqusb_chip_family)
	{
		case LTQUSB_CHIP_DANUBE:
		case LTQUSB_CHIP_ASE:
			clk = clk_get_sys("1e101000.usb", "phy");
			if (!clk || IS_ERR(clk))
				clk = clk_get_sys("1e101000.ltqhcd", "phy");
			if (!clk || IS_ERR(clk))
				clk = clk_get_sys("1e101000.ifxhcd", "phy");
			break;
		case LTQUSB_CHIP_AR9:
		case LTQUSB_CHIP_VR9:
		case LTQUSB_CHIP_AR10:
			if(core==0)
			{
				clk = clk_get_sys("1e101000.usb", "phy");
				if (!clk || IS_ERR(clk))
					clk = clk_get_sys("1e101000.ltqhcd", "phy");
				if (!clk || IS_ERR(clk))
					clk = clk_get_sys("1e101000.ifxhcd", "phy");
			}
			else
			{
				clk = clk_get_sys("1e106000.usb", "phy");
				if (!clk || IS_ERR(clk))
					clk = clk_get_sys("1e106000.ltqhcd", "phy");
				if (!clk || IS_ERR(clk))
					clk = clk_get_sys("1e106000.ifxhcd1", "phy");
			}
			break;
		default:
			clk=NULL;
	}
	if (clk && !IS_ERR(clk))
	{
		if(OnOff)
			clk_enable(clk);
		else
			clk_disable(clk);
	}
}


/*!
 \brief Turn on the USB Core Power
 \param _core_if Pointer of core_if structure
*/
void ltqusb_power_on_h (ltqusb_core_if_t *_core_if)
{
	LTQ_DEBUGPL(DBG_ENTRY, "%s() %d\n", __func__, __LINE__ );

	// set clock gating
	if(ltqusb_chip_family== LTQUSB_CHIP_AR9)
	{
		set_bit (0, (volatile unsigned long *)AR9_CGU_IFCCR);
		set_bit (1, (volatile unsigned long *)AR9_CGU_IFCCR);
	}

	MDELAY(50);

	// set power
	USB_AHB_PMU_SETUP(1);
	USB_CTRL_PMU_SETUP(_core_if->core_no,1);
	MDELAY(50);

	if(_core_if->pcgcctl)
	{
		pcgcctl_data_t pcgcctl = {.d32=0};
		pcgcctl.b.gatehclk = 1;
		ltqusb_mreg(_core_if->pcgcctl, pcgcctl.d32, 0);
	}


	if(_core_if->core_global_regs)
	{
		// PHY configurations.
		if(ltqusb_chip_family== LTQUSB_CHIP_AR9)
			ltqusb_wreg (&_core_if->core_global_regs->guid,0x14014);
	}
}

/*!
 \brief Turn off the USB Core Power
 \param _core_if Pointer of core_if structure
*/
void ltqusb_power_off_h (ltqusb_core_if_t *_core_if)
{
	ltqusb_phy_power_off_h (_core_if);

	// set power
	if(_core_if->pcgcctl)
	{
		pcgcctl_data_t pcgcctl = {.d32=0};
		pcgcctl.b.gatehclk = 1;
		pcgcctl.b.stoppclk = 1;
		ltqusb_mreg(_core_if->pcgcctl, 0, pcgcctl.d32);
	}
	USB_CTRL_PMU_SETUP(_core_if->core_no,0);
}

/*!
 \brief Turn on the USB PHY Power
 \param _core_if Pointer of core_if structure
*/
void ltqusb_phy_power_on_h (ltqusb_core_if_t *_core_if)
{
	if(_core_if->core_global_regs)
	{
		if(ltqusb_chip_family== LTQUSB_CHIP_AR9)
			ltqusb_wreg (&_core_if->core_global_regs->guid,0x14014);

		if     (ltqusb_chip_family== LTQUSB_CHIP_VR9
		      ||ltqusb_chip_family== LTQUSB_CHIP_AR10
		 )
		{
			volatile rcu_ana_cfg1_data_t rcu_ana_cfg1 = {.d32=0};
			if(ltqusb_chip_family== LTQUSB_CHIP_VR9)
			{
				if(_core_if->core_no==0)
					rcu_ana_cfg1.d32=ltqusb_rreg(((volatile uint32_t *)VR9_RCU_USB_ANA_CFG1A));
				else
					rcu_ana_cfg1.d32=ltqusb_rreg(((volatile uint32_t *)VR9_RCU_USB_ANA_CFG1B));
			}
			if(ltqusb_chip_family== LTQUSB_CHIP_AR10)
			{
				if(_core_if->core_no==0)
					rcu_ana_cfg1.d32=ltqusb_rreg(((volatile uint32_t *)AR10_RCU_USB_ANA_CFG1A));
				else
					rcu_ana_cfg1.d32=ltqusb_rreg(((volatile uint32_t *)AR10_RCU_USB_ANA_CFG1B));
			}

			if(_core_if->params.ana_disconnect_threshold >=0 && _core_if->params.ana_disconnect_threshold <=7)
				rcu_ana_cfg1.b.dis_thr = _core_if->params.ana_disconnect_threshold ;
			if(_core_if->params.ana_squelch_threshold >=0 && _core_if->params.ana_squelch_threshold <=7)
				rcu_ana_cfg1.b.squs_thr = _core_if->params.ana_squelch_threshold ;
			if(_core_if->params.ana_transmitter_crossover >=0 && _core_if->params.ana_transmitter_crossover <=3)
				rcu_ana_cfg1.b.txhs_xv = _core_if->params.ana_transmitter_crossover ;
			if(_core_if->params.ana_transmitter_impedance >=0 && _core_if->params.ana_transmitter_impedance <=15)
				rcu_ana_cfg1.b.txsrci_xv = _core_if->params.ana_transmitter_impedance ;
			if(_core_if->params.ana_transmitter_dc_voltage >=0 && _core_if->params.ana_transmitter_dc_voltage <=15)
				rcu_ana_cfg1.b.txhs_dc = _core_if->params.ana_transmitter_dc_voltage ;
			if(_core_if->params.ana_transmitter_risefall_time >=0 && _core_if->params.ana_transmitter_risefall_time <=1)
				rcu_ana_cfg1.b.tx_edge = _core_if->params.ana_transmitter_risefall_time ;
			if(_core_if->params.ana_transmitter_pre_emphasis >=0 && _core_if->params.ana_transmitter_pre_emphasis <=1)
				rcu_ana_cfg1.b.tx_pee = _core_if->params.ana_transmitter_pre_emphasis ;

			if(ltqusb_chip_family== LTQUSB_CHIP_VR9)
			{
				if(_core_if->core_no==0)
					ltqusb_wreg(((volatile uint32_t *)VR9_RCU_USB_ANA_CFG1A),rcu_ana_cfg1.d32);
				else
					ltqusb_wreg(((volatile uint32_t *)VR9_RCU_USB_ANA_CFG1B),rcu_ana_cfg1.d32);
			}
			if(ltqusb_chip_family== LTQUSB_CHIP_AR10)
			{
				if(_core_if->core_no==0)
					ltqusb_wreg(((volatile uint32_t *)AR10_RCU_USB_ANA_CFG1A),rcu_ana_cfg1.d32);
				else
					ltqusb_wreg(((volatile uint32_t *)AR10_RCU_USB_ANA_CFG1B),rcu_ana_cfg1.d32);
			}
		}

		if(_core_if->pcgcctl)
		{
			pcgcctl_data_t pcgcctl = {.d32=0};
			pcgcctl.b.stoppclk = 1;
			ltqusb_mreg(_core_if->pcgcctl, pcgcctl.d32, 0);
		}
	}

	USB_PHY_PMU_SETUP(_core_if->core_no,1);

	if(ltqusb_chip_family== LTQUSB_CHIP_AR9)
		if(_core_if->core_global_regs)
			ltqusb_wreg (&_core_if->core_global_regs->guid,0x14014);
}


/*!
 \brief Turn off the USB PHY Power
 \param _core_if Pointer of core_if structure
*/
void ltqusb_phy_power_off_h (ltqusb_core_if_t *_core_if)
{
	if(_core_if->pcgcctl)
	{
		pcgcctl_data_t pcgcctl = {.d32=0};
		pcgcctl.b.stoppclk = 1;
		ltqusb_mreg(_core_if->pcgcctl, 0, pcgcctl.d32);
	}
	USB_PHY_PMU_SETUP(_core_if->core_no,0);
}


/*!
 \brief Reset on the USB Core RCU
 \param _core_if Pointer of core_if structure
 */
static int CheckAlready(void)
{
	gusbcfg_data_t usbcfg   ={.d32 = 0};

	if     (ltqusb_chip_family== LTQUSB_CHIP_VR9
	      ||ltqusb_chip_family== LTQUSB_CHIP_AR10
	 )
	{
		usbcfg.d32 = ltqusb_rreg((volatile uint32_t *)0xBE10100C);
		if(usbcfg.b.ForceDevMode)
			return 1;
		if(usbcfg.b.ForceHstMode)
			return 1;
		usbcfg.d32 = ltqusb_rreg((volatile uint32_t *)0xBE10600C);
		if(usbcfg.b.ForceDevMode)
			return 1;
		if(usbcfg.b.ForceHstMode)
			return 1;
		return 0;
	}
	return 1;
}

void ltqusb_hard_reset_h(ltqusb_core_if_t *_core_if)
{
	if     (ltqusb_chip_family== LTQUSB_CHIP_AR9 )
	{
		if(_core_if->core_no==0)
		{
			clear_bit (AR9_USBCFG_HDSEL_BIT, (volatile unsigned long *)AR9_RCU_USB1CFG);
		}
		else
		{
			clear_bit (AR9_USBCFG_HDSEL_BIT, (volatile unsigned long *)AR9_RCU_USB2CFG);
		}
	}
	else if(ltqusb_chip_family== LTQUSB_CHIP_VR9 )
	{
//		if(!CheckAlready())
		{
			if      (ltqusb_hcd_1.core_if.irq && ltqusb_hcd_2.core_if.irq)
			{
				clear_bit (VR9_USBCFG_HDSEL_BIT, (volatile unsigned long *)VR9_RCU_USB1CFG);
				clear_bit (VR9_USBCFG_HDSEL_BIT, (volatile unsigned long *)VR9_RCU_USB2CFG);
			}
			else if(ltqusb_hcd_1.core_if.irq)
			{
				clear_bit (VR9_USBCFG_HDSEL_BIT, (volatile unsigned long *)VR9_RCU_USB1CFG);
				set_bit (VR9_USBCFG_HDSEL_BIT, (volatile unsigned long *)VR9_RCU_USB2CFG);
			}
			else if(ltqusb_hcd_2.core_if.irq)
			{
				set_bit (VR9_USBCFG_HDSEL_BIT, (volatile unsigned long *)VR9_RCU_USB1CFG);
				clear_bit (VR9_USBCFG_HDSEL_BIT, (volatile unsigned long *)VR9_RCU_USB2CFG);
			}
		}
	}
	else if(ltqusb_chip_family== LTQUSB_CHIP_AR10 )
	{
//		if(!CheckAlready())
		{
			if      (ltqusb_hcd_1.core_if.irq && ltqusb_hcd_2.core_if.irq)
			{
				clear_bit (AR10_USBCFG_HDSEL_BIT, (volatile unsigned long *)AR10_RCU_USB1CFG);
				clear_bit (AR10_USBCFG_HDSEL_BIT, (volatile unsigned long *)AR10_RCU_USB2CFG);
			}
			else if(ltqusb_hcd_1.core_if.irq)
			{
				clear_bit (AR10_USBCFG_HDSEL_BIT, (volatile unsigned long *)AR10_RCU_USB1CFG);
				set_bit (AR10_USBCFG_HDSEL_BIT, (volatile unsigned long *)AR10_RCU_USB2CFG);
			}
			else if(ltqusb_hcd_2.core_if.irq)
			{
				set_bit (AR10_USBCFG_HDSEL_BIT, (volatile unsigned long *)AR10_RCU_USB1CFG);
				clear_bit (AR10_USBCFG_HDSEL_BIT, (volatile unsigned long *)AR10_RCU_USB2CFG);
			}
		}
	}

	// set the HC's byte-order to big-endian
	if     (ltqusb_chip_family== LTQUSB_CHIP_AR9 )
	{
		if(_core_if->core_no==0)
		{
			set_bit   (AR9_USBCFG_HOST_END_BIT, (volatile unsigned long *)AR9_RCU_USB1CFG);
			clear_bit (AR9_USBCFG_SLV_END_BIT, (volatile unsigned long *)AR9_RCU_USB1CFG);
		}
		else
		{
			set_bit   (AR9_USBCFG_HOST_END_BIT, (volatile unsigned long *)AR9_RCU_USB2CFG);
			clear_bit (AR9_USBCFG_SLV_END_BIT, (volatile unsigned long *)AR9_RCU_USB2CFG);
		}
	}
	else if(ltqusb_chip_family== LTQUSB_CHIP_VR9 )
	{
		if(_core_if->core_no==0)
		{
			set_bit   (VR9_USBCFG_HOST_END_BIT, (volatile unsigned long *)VR9_RCU_USB1CFG);
			clear_bit (VR9_USBCFG_SLV_END_BIT, (volatile unsigned long *)VR9_RCU_USB1CFG);
		}
		else
		{
			set_bit   (VR9_USBCFG_HOST_END_BIT, (volatile unsigned long *)VR9_RCU_USB2CFG);
			clear_bit (VR9_USBCFG_SLV_END_BIT, (volatile unsigned long *)VR9_RCU_USB2CFG);
		}
	}
	else if(ltqusb_chip_family== LTQUSB_CHIP_AR10 )
	{
		if(_core_if->core_no==0)
		{
			set_bit   (AR10_USBCFG_HOST_END_BIT, (volatile unsigned long *)AR10_RCU_USB1CFG);
			clear_bit (AR10_USBCFG_SLV_END_BIT, (volatile unsigned long *)AR10_RCU_USB1CFG);
		}
		else
		{
			set_bit   (AR10_USBCFG_HOST_END_BIT, (volatile unsigned long *)AR10_RCU_USB2CFG);
			clear_bit (AR10_USBCFG_SLV_END_BIT, (volatile unsigned long *)AR10_RCU_USB2CFG);
		}
	}

	if(ltqusb_chip_family== LTQUSB_CHIP_AR9 )
	{
		if(_core_if->core_no==0)
		{
			set_bit (4, AR9_RCU_USBRESET);
			MDELAY(50);
			clear_bit (4, AR9_RCU_USBRESET);
		}
		else
		{
			set_bit (28, AR9_RCU_USBRESET);
			MDELAY(50);
			clear_bit (28, AR9_RCU_USBRESET);
		}
		MDELAY(50);
	}
	else if(ltqusb_chip_family== LTQUSB_CHIP_VR9 )
	{
		if(!CheckAlready())
		{
			set_bit (4, VR9_RCU_USBRESET);
			MDELAY(50);
			clear_bit (4, VR9_RCU_USBRESET);
			MDELAY(50);
		}
	}
	else if(ltqusb_chip_family== LTQUSB_CHIP_AR10 )
	{
		if(!CheckAlready())
		{
			set_bit (4, AR10_RCU_USBRESET);
			MDELAY(50);
			clear_bit (4, AR10_RCU_USBRESET);
			MDELAY(50);
		}
	}

	if(_core_if->core_global_regs)
	{
		// PHY configurations.
		if(ltqusb_chip_family== LTQUSB_CHIP_AR9 )
			ltqusb_wreg (&_core_if->core_global_regs->guid,0x14014);
	}
}

/////////////////////////////////////////////////////////////////////////
void ltqusb_led_init(ltqhcd_hcd_t *_ltqhcd)
{
	if(_ltqhcd->ledgpioid<0)
		return;
	if(ltqusb_led_shared)
	{
		if(ltqusb_hcd_1.ledgpioinstalled||ltqusb_hcd_2.ledgpioinstalled)
		{
			ltqusb_hcd_1.ledgpioinstalled=ltqusb_hcd_2.ledgpioinstalled=1;
		}
		else
		{
			if(devm_gpio_request(_ltqhcd->dev, _ltqhcd->ledgpioid,"USBLED")==0)
			{
				gpio_direction_output(_ltqhcd->ledgpioid,1);
				ltqusb_hcd_1.ledgpioinstalled=ltqusb_hcd_2.ledgpioinstalled=1;
			}
		}
	}
	else if(_ltqhcd->core_if.core_no==0)
	{
		if(!_ltqhcd->ledgpioinstalled)
		{
			if(devm_gpio_request(_ltqhcd->dev, _ltqhcd->ledgpioid,"USBLED1")==0)
			{
				gpio_direction_output(_ltqhcd->ledgpioid,1);
				_ltqhcd->ledgpioinstalled=1;
			}
		}
	}
	else
	{
		if(!_ltqhcd->ledgpioinstalled)
		{
			if(devm_gpio_request(_ltqhcd->dev, _ltqhcd->ledgpioid,"USBLED2")==0)
			{
				gpio_direction_output(_ltqhcd->ledgpioid,1);
				_ltqhcd->ledgpioinstalled=1;
			}
		}
	}
}


void ltqusb_led_free(ltqhcd_hcd_t *_ltqhcd)
{
	if(_ltqhcd->ledgpioid<0)
		return;
	if(ltqusb_led_shared)
	{
		if(ltqusb_hcd_1.ledgpioinstalled||ltqusb_hcd_2.ledgpioinstalled)
		{
			devm_gpio_free(_ltqhcd->dev, _ltqhcd->ledgpioid);
			ltqusb_hcd_1.ledgpioinstalled=ltqusb_hcd_2.ledgpioinstalled=0;
		}
	}
	else
	{
		if(_ltqhcd->ledgpioinstalled)
		{
			devm_gpio_free(_ltqhcd->dev, _ltqhcd->ledgpioid);
			_ltqhcd->ledgpioinstalled=0;
		}
	}
}

/*!
   \brief Turn off the USB LED
   \param _core_if        Pointer of core_if structure
 */
void ltqusb_led(ltqhcd_hcd_t *_ltqhcd)
{
	if(_ltqhcd->ledgpioid<0)
		return;
	if(_ltqhcd->ledgpioinstalled)
	{
		if(_ltqhcd->ledgpiostatus)
		{
			__gpio_set_value(_ltqhcd->ledgpioid,1);
			if(ltqusb_led_shared)
				ltqusb_hcd_1.ledgpiostatus=ltqusb_hcd_2.ledgpiostatus=0;
			else
				_ltqhcd->ledgpiostatus=0;
		}
		else
		{
			__gpio_set_value(_ltqhcd->ledgpioid,0);
			if(ltqusb_led_shared)
				ltqusb_hcd_1.ledgpiostatus=ltqusb_hcd_2.ledgpiostatus=1;
			else
				_ltqhcd->ledgpiostatus=1;
		}
	}
}
/////////////////////////////////////////////////////////////////////////


//#define __VERBOSE_DUMP__
/*!
 \brief internal routines for debugging
 */
void ltqusb_dump_msg_h(const u8 *buf, unsigned int length)
{
#ifdef __DEBUG__
	unsigned int	start, num, i;
	char		line[52], *p;

	if (length >= 512)
		return;
	start = 0;
	while (length > 0)
	{
		num = min(length, 16u);
		p = line;
		for (i = 0; i < num; ++i)
		{
			if (i == 8)
				*p++ = ' ';
			sprintf(p, " %02x", buf[i]);
			p += 3;
		}
		*p = 0;
		LTQ_PRINT( "%6x: %s\n", start, line);
		buf += num;
		start += num;
		length -= num;
	}
#endif
}

/*!
 \brief internal routines for debugging, reads the SPRAM and prints its content
 */
void ltqusb_dump_spram_h(ltqusb_core_if_t *_core_if)
{
#ifdef __ENABLE_DUMP__
	volatile uint8_t *addr, *start_addr, *end_addr;
	uint32_t size;
	LTQ_PRINT("SPRAM Data:\n");
	start_addr = (void*)_core_if->core_global_regs;
	LTQ_PRINT("Base Address: 0x%8X\n", (uint32_t)start_addr);

	start_addr = (void*)_core_if->data_fifo_dbg;
	LTQ_PRINT("Starting Address: 0x%8X\n", (uint32_t)start_addr);

	size=_core_if->hwcfg3.b.dfifo_depth;
	size<<=2;
	size+=0x200;
	size&=0x0003FFFC;

	end_addr = (void*)_core_if->data_fifo_dbg;
	end_addr += size;

	for(addr = start_addr; addr < end_addr; addr+=16)
	{
		LTQ_PRINT("0x%8X:  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X  %02X \n", (uint32_t)addr,
			addr[ 0], addr[ 1], addr[ 2], addr[ 3],
			addr[ 4], addr[ 5], addr[ 6], addr[ 7],
			addr[ 8], addr[ 9], addr[10], addr[11],
			addr[12], addr[13], addr[14], addr[15]
			);
	}
	return;
#endif //__ENABLE_DUMP__
}


/*!
 \brief internal routines for debugging, reads the core global registers and prints them
 */
void ltqusb_dump_registers_h(ltqusb_core_if_t *_core_if)
{
#ifdef __ENABLE_DUMP__
	int i;
	volatile uint32_t *addr;

	LTQ_PRINT("Core #%d\n",_core_if->core_no);
	LTQ_PRINT("========================================\n");
	LTQ_PRINT("Core Global Registers\n");
	{
		gotgctl_data_t gotgctl;
		addr=&_core_if->core_global_regs->gotgctl;
		gotgctl.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GOTGCTL   @0x%08X : 0x%08X\n",(uint32_t)addr,gotgctl.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            currmod       : %x\n",gotgctl.b.currmod);
		LTQ_PRINT("            bsesvld       : %x\n",gotgctl.b.bsesvld);
		LTQ_PRINT("            asesvld       : %x\n",gotgctl.b.asesvld);
		LTQ_PRINT("            conidsts      : %x\n",gotgctl.b.conidsts);
		LTQ_PRINT("            devhnpen      : %x\n",gotgctl.b.devhnpen);
		LTQ_PRINT("            hstsethnpen   : %x\n",gotgctl.b.hstsethnpen);
		LTQ_PRINT("            hnpreq        : %x\n",gotgctl.b.hnpreq);
		LTQ_PRINT("            hstnegscs     : %x\n",gotgctl.b.hstnegscs);
		LTQ_PRINT("            sesreq        : %x\n",gotgctl.b.sesreq);
		LTQ_PRINT("            sesreqscs     : %x\n",gotgctl.b.sesreqscs);
		#endif // __VERBOSE_DUMP__
	}
	{
		gotgint_data_t gotgint;
		addr=&_core_if->core_global_regs->gotgint;
		gotgint.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GOTGINT   @0x%08X : 0x%08X\n",(uint32_t)addr,gotgint.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            debdone           : %x\n",gotgint.b.debdone);
		LTQ_PRINT("            adevtoutchng      : %x\n",gotgint.b.adevtoutchng);
		LTQ_PRINT("            hstnegdet         : %x\n",gotgint.b.hstnegdet);
		LTQ_PRINT("            hstnegsucstschng  : %x\n",gotgint.b.hstnegsucstschng);
		LTQ_PRINT("            sesreqsucstschng  : %x\n",gotgint.b.sesreqsucstschng);
		LTQ_PRINT("            sesenddet         : %x\n",gotgint.b.sesenddet);
		#endif // __VERBOSE_DUMP__
	}
	{
		gahbcfg_data_t gahbcfg;
		addr=&_core_if->core_global_regs->gahbcfg;
		gahbcfg.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GAHBCFG   @0x%08X : 0x%08X\n",(uint32_t)addr,gahbcfg.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            ptxfemplvl        : %x\n",gahbcfg.b.ptxfemplvl);
		LTQ_PRINT("            nptxfemplvl       : %x\n",gahbcfg.b.nptxfemplvl);
		LTQ_PRINT("            dmaenable         : %x\n",gahbcfg.b.dmaenable);
		LTQ_PRINT("            hburstlen         : %x\n",gahbcfg.b.hburstlen);
		LTQ_PRINT("            glblintrmsk       : %x\n",gahbcfg.b.glblintrmsk);
		#endif // __VERBOSE_DUMP__
	}
	{
		gusbcfg_data_t gusbcfg;
		addr=&_core_if->core_global_regs->gusbcfg;
		gusbcfg.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GUSBCFG   @0x%08X : 0x%08X\n",(uint32_t)addr,gusbcfg.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            ForceDevMode            : %x\n",gusbcfg.b.ForceDevMode);
		LTQ_PRINT("            ForceHstMode            : %x\n",gusbcfg.b.ForceHstMode);
		LTQ_PRINT("            TxEndDelay              : %x\n",gusbcfg.b.TxEndDelay);
		LTQ_PRINT("            term_sel_dl_pulse       : %x\n",gusbcfg.b.term_sel_dl_pulse);
		LTQ_PRINT("            otgutmifssel            : %x\n",gusbcfg.b.otgutmifssel);
		LTQ_PRINT("            phylpwrclksel           : %x\n",gusbcfg.b.phylpwrclksel);
		LTQ_PRINT("            usbtrdtim               : %x\n",gusbcfg.b.usbtrdtim);
		LTQ_PRINT("            hnpcap                  : %x\n",gusbcfg.b.hnpcap);
		LTQ_PRINT("            srpcap                  : %x\n",gusbcfg.b.srpcap);
		LTQ_PRINT("            physel                  : %x\n",gusbcfg.b.physel);
		LTQ_PRINT("            fsintf                  : %x\n",gusbcfg.b.fsintf);
		LTQ_PRINT("            ulpi_utmi_sel           : %x\n",gusbcfg.b.ulpi_utmi_sel);
		LTQ_PRINT("            phyif                   : %x\n",gusbcfg.b.phyif);
		LTQ_PRINT("            toutcal                 : %x\n",gusbcfg.b.toutcal);
		#endif // __VERBOSE_DUMP__
	}
	{
		grstctl_t grstctl;
		addr=&_core_if->core_global_regs->grstctl;
		grstctl.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GRSTCTL   @0x%08X : 0x%08X\n",(uint32_t)addr,grstctl.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            ahbidle         : %x\n",grstctl.b.ahbidle);
		LTQ_PRINT("            dmareq          : %x\n",grstctl.b.dmareq);
		LTQ_PRINT("            txfnum          : %x\n",grstctl.b.txfnum);
		LTQ_PRINT("            txfflsh         : %x\n",grstctl.b.txfflsh);
		LTQ_PRINT("            rxfflsh         : %x\n",grstctl.b.rxfflsh);
		LTQ_PRINT("            intknqflsh      : %x\n",grstctl.b.intknqflsh);
		LTQ_PRINT("            hstfrm          : %x\n",grstctl.b.hstfrm);
		LTQ_PRINT("            hsftrst         : %x\n",grstctl.b.hsftrst);
		LTQ_PRINT("            csftrst         : %x\n",grstctl.b.csftrst);
		#endif // __VERBOSE_DUMP__
	}
	{
		gint_data_t gint;
		addr=&_core_if->core_global_regs->gintsts;
		gint.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GINTSTS   @0x%08X : 0x%08X\n",(uint32_t)addr,gint.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            wkupintr      : %x\n",gint.b.wkupintr);
		LTQ_PRINT("            sessreqintr   : %x\n",gint.b.sessreqintr);
		LTQ_PRINT("            disconnect    : %x\n",gint.b.disconnect);
		LTQ_PRINT("            conidstschng  : %x\n",gint.b.conidstschng);
		LTQ_PRINT("            ptxfempty     : %x\n",gint.b.ptxfempty);
		LTQ_PRINT("            hcintr        : %x\n",gint.b.hcintr);
		LTQ_PRINT("            portintr      : %x\n",gint.b.portintr);
		LTQ_PRINT("            fetsuspmsk    : %x\n",gint.b.fetsuspmsk);
		LTQ_PRINT("            incomplisoout : %x\n",gint.b.incomplisoout);
		LTQ_PRINT("            incomplisoin  : %x\n",gint.b.incomplisoin);
		LTQ_PRINT("            outepintr     : %x\n",gint.b.outepintr);
		LTQ_PRINT("            inepintr      : %x\n",gint.b.inepintr);
		LTQ_PRINT("            epmismatch    : %x\n",gint.b.epmismatch);
		LTQ_PRINT("            eopframe      : %x\n",gint.b.eopframe);
		LTQ_PRINT("            isooutdrop    : %x\n",gint.b.isooutdrop);
		LTQ_PRINT("            enumdone      : %x\n",gint.b.enumdone);
		LTQ_PRINT("            usbreset      : %x\n",gint.b.usbreset);
		LTQ_PRINT("            usbsuspend    : %x\n",gint.b.usbsuspend);
		LTQ_PRINT("            erlysuspend   : %x\n",gint.b.erlysuspend);
		LTQ_PRINT("            i2cintr       : %x\n",gint.b.i2cintr);
		LTQ_PRINT("            goutnakeff    : %x\n",gint.b.goutnakeff);
		LTQ_PRINT("            ginnakeff     : %x\n",gint.b.ginnakeff);
		LTQ_PRINT("            nptxfempty    : %x\n",gint.b.nptxfempty);
		LTQ_PRINT("            rxstsqlvl     : %x\n",gint.b.rxstsqlvl);
		LTQ_PRINT("            sofintr       : %x\n",gint.b.sofintr);
		LTQ_PRINT("            otgintr       : %x\n",gint.b.otgintr);
		LTQ_PRINT("            modemismatch  : %x\n",gint.b.modemismatch);
		#endif // __VERBOSE_DUMP__
		addr=&_core_if->core_global_regs->gintmsk;
		gint.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GINTMSK   @0x%08X : 0x%08X\n",(uint32_t)addr,gint.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            wkupintr      : %x\n",gint.b.wkupintr);
		LTQ_PRINT("            sessreqintr   : %x\n",gint.b.sessreqintr);
		LTQ_PRINT("            disconnect    : %x\n",gint.b.disconnect);
		LTQ_PRINT("            conidstschng  : %x\n",gint.b.conidstschng);
		LTQ_PRINT("            ptxfempty     : %x\n",gint.b.ptxfempty);
		LTQ_PRINT("            hcintr        : %x\n",gint.b.hcintr);
		LTQ_PRINT("            portintr      : %x\n",gint.b.portintr);
		LTQ_PRINT("            fetsuspmsk    : %x\n",gint.b.fetsuspmsk);
		LTQ_PRINT("            incomplisoout : %x\n",gint.b.incomplisoout);
		LTQ_PRINT("            incomplisoin  : %x\n",gint.b.incomplisoin);
		LTQ_PRINT("            outepintr     : %x\n",gint.b.outepintr);
		LTQ_PRINT("            inepintr      : %x\n",gint.b.inepintr);
		LTQ_PRINT("            epmismatch    : %x\n",gint.b.epmismatch);
		LTQ_PRINT("            eopframe      : %x\n",gint.b.eopframe);
		LTQ_PRINT("            isooutdrop    : %x\n",gint.b.isooutdrop);
		LTQ_PRINT("            enumdone      : %x\n",gint.b.enumdone);
		LTQ_PRINT("            usbreset      : %x\n",gint.b.usbreset);
		LTQ_PRINT("            usbsuspend    : %x\n",gint.b.usbsuspend);
		LTQ_PRINT("            erlysuspend   : %x\n",gint.b.erlysuspend);
		LTQ_PRINT("            i2cintr       : %x\n",gint.b.i2cintr);
		LTQ_PRINT("            goutnakeff    : %x\n",gint.b.goutnakeff);
		LTQ_PRINT("            ginnakeff     : %x\n",gint.b.ginnakeff);
		LTQ_PRINT("            nptxfempty    : %x\n",gint.b.nptxfempty);
		LTQ_PRINT("            rxstsqlvl     : %x\n",gint.b.rxstsqlvl);
		LTQ_PRINT("            sofintr       : %x\n",gint.b.sofintr);
		LTQ_PRINT("            otgintr       : %x\n",gint.b.otgintr);
		LTQ_PRINT("            modemismatch  : %x\n",gint.b.modemismatch);
		#endif // __VERBOSE_DUMP__
	}
	{
		gi2cctl_data_t gi2cctl;
		addr=&_core_if->core_global_regs->gi2cctl;
		gi2cctl.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GI2CCTL   @0x%08X : 0x%08X\n",(uint32_t)addr,gi2cctl.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            bsydne     : %x\n",gi2cctl.b.bsydne);
		LTQ_PRINT("            rw         : %x\n",gi2cctl.b.rw);
		LTQ_PRINT("            i2cdevaddr : %x\n",gi2cctl.b.i2cdevaddr);
		LTQ_PRINT("            i2csuspctl : %x\n",gi2cctl.b.i2csuspctl);
		LTQ_PRINT("            ack        : %x\n",gi2cctl.b.ack);
		LTQ_PRINT("            i2cen      : %x\n",gi2cctl.b.i2cen);
		LTQ_PRINT("            addr       : %x\n",gi2cctl.b.addr);
		LTQ_PRINT("            regaddr    : %x\n",gi2cctl.b.regaddr);
		LTQ_PRINT("            rwdata     : %x\n",gi2cctl.b.rwdata);
		#endif // __VERBOSE_DUMP__
	}
	addr=&_core_if->core_global_regs->gpvndctl;
	LTQ_PRINT("  GPVNDCTL  @0x%08X : 0x%08X\n",(uint32_t)addr,ltqusb_rreg(addr));
	addr=&_core_if->core_global_regs->ggpio;
	LTQ_PRINT("  GGPIO     @0x%08X : 0x%08X\n",(uint32_t)addr,ltqusb_rreg(addr));
	addr=&_core_if->core_global_regs->guid;
	LTQ_PRINT("  GUID      @0x%08X : 0x%08X\n",(uint32_t)addr,ltqusb_rreg(addr));
	addr=&_core_if->core_global_regs->gsnpsid;
	LTQ_PRINT("  GSNPSID   @0x%08X : 0x%08X\n",(uint32_t)addr,ltqusb_rreg(addr));
	{
		hwcfg1_data_t hwcfg1;
		addr=&_core_if->core_global_regs->ghwcfg1;
		hwcfg1.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GHWCFG1   @0x%08X : 0x%08X\n",(uint32_t)addr,hwcfg1.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            ep_dir15 : %x\n",hwcfg1.b.ep_dir15);
		LTQ_PRINT("            ep_dir14 : %x\n",hwcfg1.b.ep_dir14);
		LTQ_PRINT("            ep_dir13 : %x\n",hwcfg1.b.ep_dir13);
		LTQ_PRINT("            ep_dir12 : %x\n",hwcfg1.b.ep_dir12);
		LTQ_PRINT("            ep_dir11 : %x\n",hwcfg1.b.ep_dir11);
		LTQ_PRINT("            ep_dir10 : %x\n",hwcfg1.b.ep_dir10);
		LTQ_PRINT("            ep_dir09 : %x\n",hwcfg1.b.ep_dir09);
		LTQ_PRINT("            ep_dir08 : %x\n",hwcfg1.b.ep_dir08);
		LTQ_PRINT("            ep_dir07 : %x\n",hwcfg1.b.ep_dir07);
		LTQ_PRINT("            ep_dir06 : %x\n",hwcfg1.b.ep_dir06);
		LTQ_PRINT("            ep_dir05 : %x\n",hwcfg1.b.ep_dir05);
		LTQ_PRINT("            ep_dir04 : %x\n",hwcfg1.b.ep_dir04);
		LTQ_PRINT("            ep_dir03 : %x\n",hwcfg1.b.ep_dir03);
		LTQ_PRINT("            ep_dir02 : %x\n",hwcfg1.b.ep_dir02);
		LTQ_PRINT("            ep_dir01 : %x\n",hwcfg1.b.ep_dir01);
		LTQ_PRINT("            ep_dir00 : %x\n",hwcfg1.b.ep_dir00);
		#endif // __VERBOSE_DUMP__
	}
	{
		hwcfg2_data_t hwcfg2;
		addr=&_core_if->core_global_regs->ghwcfg2;
		hwcfg2.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GHWCFG2   @0x%08X : 0x%08X\n",(uint32_t)addr,hwcfg2.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            dev_token_q_depth      : %x\n",hwcfg2.b.dev_token_q_depth);
		LTQ_PRINT("            host_perio_tx_q_depth  : %x\n",hwcfg2.b.host_perio_tx_q_depth);
		LTQ_PRINT("            nonperio_tx_q_depth    : %x\n",hwcfg2.b.nonperio_tx_q_depth);
		LTQ_PRINT("            rx_status_q_depth      : %x\n",hwcfg2.b.rx_status_q_depth);
		LTQ_PRINT("            dynamic_fifo           : %x\n",hwcfg2.b.dynamic_fifo);
		LTQ_PRINT("            perio_ep_supported     : %x\n",hwcfg2.b.perio_ep_supported);
		LTQ_PRINT("            num_host_chan          : %x\n",hwcfg2.b.num_host_chan);
		LTQ_PRINT("            num_dev_ep             : %x\n",hwcfg2.b.num_dev_ep);
		LTQ_PRINT("            fs_phy_type            : %x\n",hwcfg2.b.fs_phy_type);
		LTQ_PRINT("            hs_phy_type            : %x\n",hwcfg2.b.hs_phy_type);
		LTQ_PRINT("            point2point            : %x\n",hwcfg2.b.point2point);
		LTQ_PRINT("            architecture           : %x\n",hwcfg2.b.architecture);
		LTQ_PRINT("            op_mode                : %x\n",hwcfg2.b.op_mode);
		#endif // __VERBOSE_DUMP__
	}
	{
		hwcfg3_data_t hwcfg3;
		addr=&_core_if->core_global_regs->ghwcfg3;
		hwcfg3.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GHWCFG3   @0x%08X : 0x%08X\n",(uint32_t)addr,hwcfg3.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            dfifo_depth            : %x\n",hwcfg3.b.dfifo_depth);
		LTQ_PRINT("            synch_reset_type       : %x\n",hwcfg3.b.synch_reset_type);
		LTQ_PRINT("            optional_features      : %x\n",hwcfg3.b.optional_features);
		LTQ_PRINT("            vendor_ctrl_if         : %x\n",hwcfg3.b.vendor_ctrl_if);
		LTQ_PRINT("            i2c                    : %x\n",hwcfg3.b.otg_func);
		LTQ_PRINT("            otg_func               : %x\n",hwcfg3.b.otg_func);
		LTQ_PRINT("            packet_size_cntr_width : %x\n",hwcfg3.b.packet_size_cntr_width);
		LTQ_PRINT("            xfer_size_cntr_width   : %x\n",hwcfg3.b.xfer_size_cntr_width);
		#endif // __VERBOSE_DUMP__
	}
	{
		hwcfg4_data_t hwcfg4;
		addr=&_core_if->core_global_regs->ghwcfg4;
		hwcfg4.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GHWCFG4   @0x%08X : 0x%08X\n",(uint32_t)addr,hwcfg4.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            desc_dma_dyn         : %x\n",hwcfg4.b.desc_dma_dyn);
		LTQ_PRINT("            desc_dma             : %x\n",hwcfg4.b.desc_dma);
		LTQ_PRINT("            num_in_eps           : %x\n",hwcfg4.b.num_in_eps);
		LTQ_PRINT("            ded_fifo_en          : %x\n",hwcfg4.b.ded_fifo_en);
		LTQ_PRINT("            session_end_filt_en  : %x\n",hwcfg4.b.session_end_filt_en);
		LTQ_PRINT("            b_valid_filt_en      : %x\n",hwcfg4.b.b_valid_filt_en);
		LTQ_PRINT("            a_valid_filt_en      : %x\n",hwcfg4.b.a_valid_filt_en);
		LTQ_PRINT("            vbus_valid_filt_en   : %x\n",hwcfg4.b.vbus_valid_filt_en);
		LTQ_PRINT("            iddig_filt_en        : %x\n",hwcfg4.b.iddig_filt_en);
		LTQ_PRINT("            num_dev_mode_ctrl_ep : %x\n",hwcfg4.b.num_dev_mode_ctrl_ep);
		LTQ_PRINT("            utmi_phy_data_width  : %x\n",hwcfg4.b.utmi_phy_data_width);
		LTQ_PRINT("            min_ahb_freq         : %x\n",hwcfg4.b.min_ahb_freq);
		LTQ_PRINT("            power_optimiz        : %x\n",hwcfg4.b.power_optimiz);
		LTQ_PRINT("            num_dev_perio_in_ep  : %x\n",hwcfg4.b.num_dev_perio_in_ep);
		#endif // __VERBOSE_DUMP__
	}

	{
		pcgcctl_data_t pcgcctl;
		addr=_core_if->pcgcctl;
		pcgcctl.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  PCGCCTL   @0x%08X : 0x%08X\n",(uint32_t)addr,pcgcctl.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            physuspended  : %x\n",pcgcctl.b.physuspended);
		LTQ_PRINT("            rstpdwnmodule : %x\n",pcgcctl.b.rstpdwnmodule);
		LTQ_PRINT("            pwrclmp       : %x\n",pcgcctl.b.pwrclmp);
		LTQ_PRINT("            gatehclk      : %x\n",pcgcctl.b.gatehclk);
		LTQ_PRINT("            stoppclk      : %x\n",pcgcctl.b.stoppclk);
		#endif // __VERBOSE_DUMP__
	}

	addr=&_core_if->core_global_regs->grxfsiz;
	LTQ_PRINT("  GRXFSIZ   @0x%08X : 0x%08X\n",(uint32_t)addr,ltqusb_rreg(addr));
	{
		fifosize_data_t fifosize;
		addr=&_core_if->core_global_regs->gnptxfsiz;
		fifosize.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  GNPTXFSIZ @0x%08X : 0x%08X\n",(uint32_t)addr,fifosize.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            depth     : %x\n",fifosize.b.depth);
		LTQ_PRINT("            startaddr : %x\n",fifosize.b.startaddr);
		#endif // __VERBOSE_DUMP__
		addr=&_core_if->core_global_regs->hptxfsiz;
		fifosize.d32=ltqusb_rreg(addr);
		LTQ_PRINT("  HPTXFSIZ  @0x%08X : 0x%08X\n",(uint32_t)addr,fifosize.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            depth     : %x\n",fifosize.b.depth);
		LTQ_PRINT("            startaddr : %x\n",fifosize.b.startaddr);
		#endif // __VERBOSE_DUMP__
	}

	LTQ_PRINT("  Host Global Registers\n");
	{
		hcfg_data_t hcfg;
		addr=&_core_if->host_global_regs->hcfg;
		hcfg.d32=ltqusb_rreg(addr);
		LTQ_PRINT("    HCFG      @0x%08X : 0x%08X\n",(uint32_t)addr,hcfg.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            fslssupp      : %x\n",hcfg.b.fslssupp);
		LTQ_PRINT("            fslspclksel   : %x\n",hcfg.b.fslspclksel);
		#endif // __VERBOSE_DUMP__
	}
	addr=&_core_if->host_global_regs->hfir;
	LTQ_PRINT("    HFIR      @0x%08X : 0x%08X\n",(uint32_t)addr,ltqusb_rreg(addr));
	{
		hfnum_data_t hfnum;
		addr=&_core_if->host_global_regs->hfnum;
		hfnum.d32=ltqusb_rreg(addr);
		LTQ_PRINT("    HFNUM     @0x%08X : 0x%08X\n",(uint32_t)addr,hfnum.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            frrem : %x\n",hfnum.b.frrem);
		LTQ_PRINT("            frnum : %x\n",hfnum.b.frnum);
		#endif // __VERBOSE_DUMP__
	}
	{
		hptxsts_data_t hptxsts;
		addr=&_core_if->host_global_regs->hptxsts;
		hptxsts.d32=ltqusb_rreg(addr);
		LTQ_PRINT("    HPTXSTS   @0x%08X : 0x%08X\n",(uint32_t)addr,hptxsts.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            ptxqtop_odd       : %x\n",hptxsts.b.ptxqtop_odd);
		LTQ_PRINT("            ptxqtop_chnum     : %x\n",hptxsts.b.ptxqtop_chnum);
		LTQ_PRINT("            ptxqtop_token     : %x\n",hptxsts.b.ptxqtop_token);
		LTQ_PRINT("            ptxqtop_terminate : %x\n",hptxsts.b.ptxqtop_terminate);
		LTQ_PRINT("            ptxqspcavail      : %x\n",hptxsts.b.ptxqspcavail);
		LTQ_PRINT("            ptxfspcavail      : %x\n",hptxsts.b.ptxfspcavail );
		#endif // __VERBOSE_DUMP__
	}
	addr=&_core_if->host_global_regs->haint;
	LTQ_PRINT("    HAINT     @0x%08X : 0x%08X\n",(uint32_t)addr,ltqusb_rreg(addr));
	addr=&_core_if->host_global_regs->haintmsk;
	LTQ_PRINT("    HAINTMSK  @0x%08X : 0x%08X\n",(uint32_t)addr,ltqusb_rreg(addr));
	{
		hprt0_data_t hprt0;
		addr= _core_if->hprt0;
		hprt0.d32=ltqusb_rreg(addr);
		LTQ_PRINT("    HPRT0     @0x%08X : 0x%08X\n",(uint32_t)addr,hprt0.d32);
		#ifdef __VERBOSE_DUMP__
		LTQ_PRINT("            prtspd          : %x\n",hprt0.b.prtspd);
		LTQ_PRINT("            prttstctl       : %x\n",hprt0.b.prttstctl);
		LTQ_PRINT("            prtpwr          : %x\n",hprt0.b.prtpwr);
		LTQ_PRINT("            prtlnsts        : %x\n",hprt0.b.prtlnsts);
		LTQ_PRINT("            prtrst          : %x\n",hprt0.b.prtrst);
		LTQ_PRINT("            prtsusp         : %x\n",hprt0.b.prtsusp);
		LTQ_PRINT("            prtres          : %x\n",hprt0.b.prtres);
		LTQ_PRINT("            prtovrcurrchng  : %x\n",hprt0.b.prtovrcurrchng);
		LTQ_PRINT("            prtovrcurract   : %x\n",hprt0.b.prtovrcurract);
		LTQ_PRINT("            prtenchng       : %x\n",hprt0.b.prtenchng);
		LTQ_PRINT("            prtena          : %x\n",hprt0.b.prtena);
		LTQ_PRINT("            prtconndet      : %x\n",hprt0.b.prtconndet );
		LTQ_PRINT("            prtconnsts      : %x\n",hprt0.b.prtconnsts);
		#endif // __VERBOSE_DUMP__
	}

	for (i=0; i<MAX_EPS_CHANNELS; i++)
	{
		LTQ_PRINT("  Host Channel %d Specific Registers\n", i);
		{
			hcchar_data_t hcchar;
			addr=&_core_if->hc_regs[i]->hcchar;
			hcchar.d32=ltqusb_rreg(addr);
			LTQ_PRINT("    HCCHAR    @0x%08X : 0x%08X\n",(uint32_t)addr,hcchar.d32);
			#ifdef __VERBOSE_DUMP__
			LTQ_PRINT("            chen      : %x\n",hcchar.b.chen);
			LTQ_PRINT("            chdis     : %x\n",hcchar.b.chdis);
			LTQ_PRINT("            oddfrm    : %x\n",hcchar.b.oddfrm);
			LTQ_PRINT("            devaddr   : %x\n",hcchar.b.devaddr);
			LTQ_PRINT("            multicnt  : %x\n",hcchar.b.multicnt);
			LTQ_PRINT("            eptype    : %x\n",hcchar.b.eptype);
			LTQ_PRINT("            lspddev   : %x\n",hcchar.b.lspddev);
			LTQ_PRINT("            epdir     : %x\n",hcchar.b.epdir);
			LTQ_PRINT("            epnum     : %x\n",hcchar.b.epnum);
			LTQ_PRINT("            mps       : %x\n",hcchar.b.mps);
			#endif // __VERBOSE_DUMP__
		}
		{
			hcsplt_data_t hcsplt;
			addr=&_core_if->hc_regs[i]->hcsplt;
			hcsplt.d32=ltqusb_rreg(addr);
			LTQ_PRINT("    HCSPLT    @0x%08X : 0x%08X\n",(uint32_t)addr,hcsplt.d32);
			#ifdef __VERBOSE_DUMP__
			LTQ_PRINT("            spltena  : %x\n",hcsplt.b.spltena);
			LTQ_PRINT("            compsplt : %x\n",hcsplt.b.compsplt);
			LTQ_PRINT("            xactpos  : %x\n",hcsplt.b.xactpos);
			LTQ_PRINT("            hubaddr  : %x\n",hcsplt.b.hubaddr);
			LTQ_PRINT("            prtaddr  : %x\n",hcsplt.b.prtaddr);
			#endif // __VERBOSE_DUMP__
		}
		{
			hcint_data_t hcint;
			addr=&_core_if->hc_regs[i]->hcint;
			hcint.d32=ltqusb_rreg(addr);
			LTQ_PRINT("    HCINT     @0x%08X : 0x%08X\n",(uint32_t)addr,hcint.d32);
			#ifdef __VERBOSE_DUMP__
			LTQ_PRINT("            datatglerr : %x\n",hcint.b.datatglerr);
			LTQ_PRINT("            frmovrun   : %x\n",hcint.b.frmovrun);
			LTQ_PRINT("            bblerr     : %x\n",hcint.b.bblerr);
			LTQ_PRINT("            xacterr    : %x\n",hcint.b.xacterr);
			LTQ_PRINT("            nyet       : %x\n",hcint.b.nyet);
			LTQ_PRINT("            ack        : %x\n",hcint.b.ack);
			LTQ_PRINT("            nak        : %x\n",hcint.b.nak);
			LTQ_PRINT("            stall      : %x\n",hcint.b.stall);
			LTQ_PRINT("            ahberr     : %x\n",hcint.b.ahberr);
			LTQ_PRINT("            chhltd     : %x\n",hcint.b.chhltd);
			LTQ_PRINT("            xfercomp   : %x\n",hcint.b.xfercomp);
			#endif // __VERBOSE_DUMP__
			addr=&_core_if->hc_regs[i]->hcintmsk;
			hcint.d32=ltqusb_rreg(addr);
			LTQ_PRINT("    HCINTMSK  @0x%08X : 0x%08X\n",(uint32_t)addr,hcint.d32);
			#ifdef __VERBOSE_DUMP__
			LTQ_PRINT("            datatglerr : %x\n",hcint.b.datatglerr);
			LTQ_PRINT("            frmovrun   : %x\n",hcint.b.frmovrun);
			LTQ_PRINT("            bblerr     : %x\n",hcint.b.bblerr);
			LTQ_PRINT("            xacterr    : %x\n",hcint.b.xacterr);
			LTQ_PRINT("            nyet       : %x\n",hcint.b.nyet);
			LTQ_PRINT("            ack        : %x\n",hcint.b.ack);
			LTQ_PRINT("            nak        : %x\n",hcint.b.nak);
			LTQ_PRINT("            stall      : %x\n",hcint.b.stall);
			LTQ_PRINT("            ahberr     : %x\n",hcint.b.ahberr);
			LTQ_PRINT("            chhltd     : %x\n",hcint.b.chhltd);
			LTQ_PRINT("            xfercomp   : %x\n",hcint.b.xfercomp);
			#endif // __VERBOSE_DUMP__
		}
		{
			hctsiz_data_t hctsiz;
			addr=&_core_if->hc_regs[i]->hctsiz;
			hctsiz.d32=ltqusb_rreg(addr);
			LTQ_PRINT("    HCTSIZ    @0x%08X : 0x%08X\n",(uint32_t)addr,hctsiz.d32);
			#ifdef __VERBOSE_DUMP__
			LTQ_PRINT("            dopng     : %x\n",hctsiz.b.dopng);
			LTQ_PRINT("            pid       : %x\n",hctsiz.b.pid);
			LTQ_PRINT("            pktcnt    : %x\n",hctsiz.b.pktcnt);
			LTQ_PRINT("            xfersize  : %x\n",hctsiz.b.xfersize);
			#endif // __VERBOSE_DUMP__
		}
		addr=&_core_if->hc_regs[i]->hcdma;
		LTQ_PRINT("    HCDMA     @0x%08X : 0x%08X\n",(uint32_t)addr,ltqusb_rreg(addr));
	}
#endif //__ENABLE_DUMP__
}



void do_suspend_h(ltqhcd_hcd_t *_ltqhcd)
{
	hprt0_data_t               hprt0 = {.d32 = 0};
	ltqusb_core_if_t *core_if=&_ltqhcd->core_if;

	core_if->issuspended=1;
#if 1
	hprt0.d32 = ltqusb_read_hprt0 (core_if);
	hprt0.b.prtsusp = 1;
	hprt0.b.prtres = 0;
	hprt0.b.prtpwr = 0;
	ltqusb_wreg(core_if->hprt0, hprt0.d32);
	ltqusb_vbus_off(_ltqhcd);
	mdelay(100);
#endif
	ltqusb_phy_power_off_h(core_if);
	ltqusb_power_off_h(core_if);
}
void do_resume_h(ltqhcd_hcd_t *_ltqhcd)
{
	hprt0_data_t               hprt0 = {.d32 = 0};
	ltqusb_core_if_t *core_if=&_ltqhcd->core_if;
	ltqusb_vbus_on(_ltqhcd);
	mdelay(100);
	ltqusb_power_on_h(core_if);
	ltqusb_phy_power_on_h(core_if);

	hprt0.d32 = ltqusb_read_hprt0 (core_if);
	hprt0.b.prtsusp = 0;
#if 1
	hprt0.b.prtres = 1;
#endif
	hprt0.b.prtpwr = 1;
	ltqusb_wreg(core_if->hprt0, hprt0.d32);
	mdelay(100);
	hprt0.d32 = ltqusb_read_hprt0 (core_if);
	hprt0.b.prtsusp = 0;
	hprt0.b.prtres = 0;
	ltqusb_wreg(core_if->hprt0, hprt0.d32);
	core_if->issuspended=0;
}
void do_bussuspend_h(ltqhcd_hcd_t *_ltqhcd)
{
	hprt0_data_t               hprt0 = {.d32 = 0};
	ltqusb_core_if_t *core_if=&_ltqhcd->core_if;

	hprt0.d32 = ltqusb_read_hprt0 (core_if);
	hprt0.b.prtsusp  =1;
	hprt0.b.prtres   =0;
	hprt0.b.prttstctl=0; //Test mode disabled
	ltqusb_wreg(core_if->hprt0, hprt0.d32);
}
void do_busresume_h(ltqhcd_hcd_t *_ltqhcd)
{
	hprt0_data_t               hprt0 = {.d32 = 0};

	ltqusb_core_if_t *core_if=&_ltqhcd->core_if;

	hprt0.d32 = ltqusb_read_hprt0 (core_if);
	hprt0.b.prtsusp  =0;
	hprt0.b.prtres   =1;
	hprt0.b.prttstctl=0; //Test mode disabled
	ltqusb_wreg(core_if->hprt0, hprt0.d32);
	mdelay(20);

	hprt0.d32 = ltqusb_read_hprt0 (core_if);
	hprt0.b.prtsusp  =0;
	hprt0.b.prtres   =0;
	hprt0.b.prttstctl=0; //Test mode disabled
	ltqusb_wreg(core_if->hprt0, hprt0.d32);
}


/*!
 \brief This function enables the Host mode interrupts.
 \param _core_if        Pointer of core_if structure
 */
void ltqusb_host_enable_interrupts(ltqusb_core_if_t *_core_if)
{
	gint_data_t intr_mask ={ .d32 = 0};
	ltqusb_core_global_regs_t *global_regs = _core_if->core_global_regs;

	LTQ_DEBUGPL(DBG_CIL, "%s()\n", __func__);

	/* Clear any pending OTG Interrupts */
	ltqusb_wreg( &global_regs->gotgint, 0xFFFFFFFF);

	/* Clear any pending interrupts */
	ltqusb_wreg( &global_regs->gintsts, 0xFFFFFFFF);

	/* Enable the interrupts in the GINTMSK.*/

	/* Common interrupts */
	intr_mask.b.modemismatch = 1;
	intr_mask.b.conidstschng = 1;
	intr_mask.b.wkupintr = 1;
	intr_mask.b.disconnect = 1;
	intr_mask.b.usbsuspend = 1;

	/* Host interrupts */
	intr_mask.b.sofintr = 1;
	intr_mask.b.portintr = 1;
	intr_mask.b.hcintr = 1;

	ltqusb_mreg( &global_regs->gintmsk, intr_mask.d32, intr_mask.d32);
	LTQ_DEBUGPL(DBG_CIL, "%s() gintmsk=%0x\n", __func__, ltqusb_rreg( &global_regs->gintmsk));
}

/*!
 \brief This function disables the Host mode interrupts.
 \param _core_if        Pointer of core_if structure
 */
void ltqusb_host_disable_interrupts(ltqusb_core_if_t *_core_if)
{
	ltqusb_core_global_regs_t *global_regs = _core_if->core_global_regs;

	LTQ_DEBUGPL(DBG_CILV, "%s()\n", __func__);

	#if 1
		ltqusb_wreg( &global_regs->gintmsk, 0);
	#else
		/* Common interrupts */
		{
			gint_data_t intr_mask ={.d32 = 0};
			intr_mask.b.modemismatch = 1;
			intr_mask.b.rxstsqlvl = 1;
			intr_mask.b.conidstschng = 1;
			intr_mask.b.wkupintr = 1;
			intr_mask.b.disconnect = 1;
			intr_mask.b.usbsuspend = 1;

			/* Host interrupts */
			intr_mask.b.sofintr = 1;
			intr_mask.b.portintr = 1;
			intr_mask.b.hcintr = 1;
			intr_mask.b.ptxfempty = 1;
			intr_mask.b.nptxfempty = 1;
			ltqusb_mreg(&global_regs->gintmsk, intr_mask.d32, 0);
		}
	#endif
}

/*!
 \brief This function initializes the LTQUSB controller registers for  Host mode.
        This function flushes the Tx and Rx FIFOs and it flushes any entries in the
        request queues.
 \param _core_if        Pointer of core_if structure
 \param _params         parameters to be set
 */
void ltqusb_host_core_init(ltqusb_core_if_t *_core_if, ltqusb_params_t  *_params)
{
	ltqusb_core_global_regs_t *global_regs =  _core_if->core_global_regs;

	gusbcfg_data_t usbcfg   ={.d32 = 0};
	gahbcfg_data_t ahbcfg   ={.d32 = 0};
	gotgctl_data_t gotgctl  ={.d32 = 0};

	int i;

	LTQ_DEBUGPL(DBG_CILV, "%s(%p)\n",__func__,_core_if);

	/* Copy Params */

	_core_if->params.dma_burst_size      =  _params->dma_burst_size;
	_core_if->params.speed               =  _params->speed;
	if(_params->max_transfer_size < 2048 || _params->max_transfer_size > ((1 << (_core_if->hwcfg3.b.xfer_size_cntr_width + 11)) - 1) )
		_core_if->params.max_transfer_size = ((1 << (_core_if->hwcfg3.b.xfer_size_cntr_width + 11)) - 1);
	else
		_core_if->params.max_transfer_size = _params->max_transfer_size;

	if(_params->max_packet_count < 16 || _params->max_packet_count > ((1 << (_core_if->hwcfg3.b.packet_size_cntr_width + 4)) - 1) )
		_core_if->params.max_packet_count= ((1 << (_core_if->hwcfg3.b.packet_size_cntr_width + 4)) - 1);
	else
		_core_if->params.max_packet_count=  _params->max_packet_count;
	_core_if->params.phy_utmi_width      =  _params->phy_utmi_width;
	_core_if->params.timeout_cal         =  _params->timeout_cal;
	#if defined(__WITH_OC_HY__)
		_core_if->params.oc_hy           =  _params->oc_hy;
	#endif

	_core_if->params.ana_disconnect_threshold        =  _params->ana_disconnect_threshold;
	_core_if->params.ana_squelch_threshold           =  _params->ana_squelch_threshold;
	_core_if->params.ana_transmitter_crossover       =  _params->ana_transmitter_crossover;
	_core_if->params.ana_transmitter_impedance       =  _params->ana_transmitter_impedance;
	_core_if->params.ana_transmitter_dc_voltage      =  _params->ana_transmitter_dc_voltage;
	_core_if->params.ana_transmitter_risefall_time   =  _params->ana_transmitter_risefall_time;
	_core_if->params.ana_transmitter_pre_emphasis    =  _params->ana_transmitter_pre_emphasis;

	usbcfg.d32 = ltqusb_rreg(&global_regs->gusbcfg);
//	usbcfg.b.ulpi_ext_vbus_drv = 1;
	usbcfg.b.term_sel_dl_pulse = 0;
	usbcfg.b.ForceDevMode = 0;
	usbcfg.b.ForceHstMode = 1;
	usbcfg.b.toutcal      = _core_if->params.timeout_cal;
	ltqusb_wreg (&global_regs->gusbcfg, usbcfg.d32);
	/* Reset the Controller */
	do
	{
		while(ltqusb_core_soft_reset_h( _core_if ))
			ltqusb_hard_reset_h(_core_if);
	} while (ltqusb_is_device_mode(_core_if));

	usbcfg.d32 = ltqusb_rreg(&global_regs->gusbcfg);
//	usbcfg.b.ulpi_ext_vbus_drv = 1;
	usbcfg.b.term_sel_dl_pulse = 0;
	usbcfg.b.toutcal           = _core_if->params.timeout_cal;
	ltqusb_wreg (&global_regs->gusbcfg, usbcfg.d32);

	/* This programming sequence needs to happen in FS mode before any other
	 * programming occurs */
	/* High speed PHY. */
	if (!_core_if->phy_init_done)
	{
		_core_if->phy_init_done = 1;
		/* HS PHY parameters.  These parameters are preserved
		 * during soft reset so only program the first time.  Do
		 * a soft reset immediately after setting phyif.  */
		usbcfg.b.ulpi_utmi_sel = 0; //UTMI+
		usbcfg.b.phyif = ( _core_if->params.phy_utmi_width == 16)?1:0;
		ltqusb_wreg( &global_regs->gusbcfg, usbcfg.d32);
		/* Reset after setting the PHY parameters */
		ltqusb_core_soft_reset_h( _core_if );
	}

	usbcfg.d32 = ltqusb_rreg(&global_regs->gusbcfg);
//	usbcfg.b.ulpi_fsls = 0;
//	usbcfg.b.ulpi_clk_sus_m = 0;
	usbcfg.b.term_sel_dl_pulse = 0;
	usbcfg.b.ForceDevMode = 0;
	usbcfg.b.ForceHstMode = 1;
	ltqusb_wreg(&global_regs->gusbcfg, usbcfg.d32);

	/* Program the GAHBCFG Register.*/
	switch (_core_if->params.dma_burst_size)
	{
		case 0 :
			ahbcfg.b.hburstlen = LTQUSB_GAHBCFG_INT_DMA_BURST_SINGLE;
			break;
		case 1 :
			ahbcfg.b.hburstlen = LTQUSB_GAHBCFG_INT_DMA_BURST_INCR;
			break;
		case 4 :
			ahbcfg.b.hburstlen = LTQUSB_GAHBCFG_INT_DMA_BURST_INCR4;
			break;
		case 8 :
			ahbcfg.b.hburstlen = LTQUSB_GAHBCFG_INT_DMA_BURST_INCR8;
			break;
		case 16:
			ahbcfg.b.hburstlen = LTQUSB_GAHBCFG_INT_DMA_BURST_INCR16;
			break;
	}
	#if defined(__UNALIGNED_BUF_ADJ__) || defined(__UNALIGNED_BUF_CHK__)
		_core_if->unaligned_mask=3;
		#if defined(__UNALIGNED_BUF_BURST__)
			switch(_core_if->params.dma_burst_size)
			{
				case 4 :
					_core_if->unaligned_mask=15;
					break;
				case 8 :
					_core_if->unaligned_mask=31;
					break;
				case 16:
					_core_if->unaligned_mask=63;
					break;
				case 0 :
				case 1 :
					break;
				default:
					break;
			}
		#endif //defined(__UNALIGNED_BUF_BURST__)
	#endif //defined(__UNALIGNED_BUF_ADJ__) || defined(__UNALIGNED_BUF_CHK__)
	ahbcfg.b.dmaenable = 1;
	ltqusb_wreg(&global_regs->gahbcfg, ahbcfg.d32);

	/* Program the GUSBCFG register. */
	usbcfg.d32 = ltqusb_rreg( &global_regs->gusbcfg );
	usbcfg.b.hnpcap = 0;
	usbcfg.b.srpcap = 0;
	ltqusb_wreg( &global_regs->gusbcfg, usbcfg.d32);

	/* Restart the Phy Clock */
	ltqusb_wreg(_core_if->pcgcctl, 0);

	/* Initialize Host Configuration Register */
	{
		hcfg_data_t 	hcfg;
		hcfg.d32 = ltqusb_rreg(&_core_if->host_global_regs->hcfg);
		hcfg.b.fslspclksel = LTQUSB_HCFG_30_60_MHZ;
		if (_params->speed == LTQUSB_PARAM_SPEED_FULL)
			hcfg.b.fslssupp = 1;
		ltqusb_wreg(&_core_if->host_global_regs->hcfg, hcfg.d32);
	}

	_core_if->params.host_channels=(_core_if->hwcfg2.b.num_host_chan + 1);

	if(_params->host_channels>0 && _params->host_channels < _core_if->params.host_channels)
		_core_if->params.host_channels = _params->host_channels;

	/* Configure data FIFO sizes */
	_core_if->params.data_fifo_size     = _core_if->hwcfg3.b.dfifo_depth;
	_core_if->params.rx_fifo_size       = ltqusb_rreg(&global_regs->grxfsiz);
	_core_if->params.nperio_tx_fifo_size= ltqusb_rreg(&global_regs->gnptxfsiz) >> 16;
	_core_if->params.perio_tx_fifo_size = ltqusb_rreg(&global_regs->hptxfsiz) >> 16;
	LTQ_DEBUGPL(DBG_CIL, "Initial: FIFO Size=0x%06X\n"   , _core_if->params.data_fifo_size);
	LTQ_DEBUGPL(DBG_CIL, "           Rx FIFO Size=0x%06X\n", _core_if->params.rx_fifo_size);
	LTQ_DEBUGPL(DBG_CIL, "         NPTx FIFO Size=0x%06X\n", _core_if->params.nperio_tx_fifo_size);
	LTQ_DEBUGPL(DBG_CIL, "          PTx FIFO Size=0x%06X\n", _core_if->params.perio_tx_fifo_size);

	{
		fifosize_data_t txfifosize;
		if(_params->data_fifo_size >=0 && _params->data_fifo_size < _core_if->params.data_fifo_size)
			_core_if->params.data_fifo_size = _params->data_fifo_size;

		if( _params->rx_fifo_size >= 0 && _params->rx_fifo_size < _core_if->params.rx_fifo_size)
			_core_if->params.rx_fifo_size = _params->rx_fifo_size;
		if( _params->nperio_tx_fifo_size >=0 && _params->nperio_tx_fifo_size < _core_if->params.nperio_tx_fifo_size)
			_core_if->params.nperio_tx_fifo_size = _params->nperio_tx_fifo_size;
		if( _params->perio_tx_fifo_size >=0 && _params->perio_tx_fifo_size < _core_if->params.perio_tx_fifo_size)
			_core_if->params.perio_tx_fifo_size = _params->perio_tx_fifo_size;

		if(_core_if->params.data_fifo_size < _core_if->params.rx_fifo_size)
			_core_if->params.rx_fifo_size = _core_if->params.data_fifo_size;
		ltqusb_wreg( &global_regs->grxfsiz, _core_if->params.rx_fifo_size);
		txfifosize.b.startaddr = _core_if->params.rx_fifo_size;

		if(txfifosize.b.startaddr + _core_if->params.nperio_tx_fifo_size > _core_if->params.data_fifo_size)
			_core_if->params.nperio_tx_fifo_size = _core_if->params.data_fifo_size - txfifosize.b.startaddr;
		txfifosize.b.depth=_core_if->params.nperio_tx_fifo_size;
		ltqusb_wreg( &global_regs->gnptxfsiz, txfifosize.d32);
		txfifosize.b.startaddr += _core_if->params.nperio_tx_fifo_size;

		if(txfifosize.b.startaddr + _core_if->params.perio_tx_fifo_size > _core_if->params.data_fifo_size)
			_core_if->params.perio_tx_fifo_size = _core_if->params.data_fifo_size - txfifosize.b.startaddr;
		txfifosize.b.depth=_core_if->params.perio_tx_fifo_size;
		ltqusb_wreg( &global_regs->hptxfsiz, txfifosize.d32);
		txfifosize.b.startaddr += _core_if->params.perio_tx_fifo_size;
	}

	#ifdef __DEBUG__
	{
		fifosize_data_t fifosize;
		LTQ_DEBUGPL(DBG_CIL, "Result : FIFO Size=0x%06X\n"   , _core_if->params.data_fifo_size);

		fifosize.d32=ltqusb_rreg(&global_regs->grxfsiz);
		LTQ_DEBUGPL(DBG_CIL, "         Rx FIFO =0x%06X 0x%06X\n", fifosize.b.startaddr,fifosize.b.depth);
		fifosize.d32=ltqusb_rreg(&global_regs->gnptxfsiz);
		LTQ_DEBUGPL(DBG_CIL, "         NPTx FIFO =0x%06X 0x%06X\n", fifosize.b.startaddr,fifosize.b.depth);
		fifosize.d32=ltqusb_rreg(&global_regs->hptxfsiz);
		LTQ_DEBUGPL(DBG_CIL, "          PTx FIFO =0x%06X 0x%06X\n", fifosize.b.startaddr,fifosize.b.depth);
	}
	#endif

	/* Clear Host Set HNP Enable in the OTG Control Register */
	gotgctl.b.hstsethnpen = 1;
	ltqusb_mreg( &global_regs->gotgctl, gotgctl.d32, 0);

	/* Flush the FIFOs */
	ltqusb_flush_tx_fifo_h(_core_if, 0x10);  /* all Tx FIFOs */
	ltqusb_flush_rx_fifo_h(_core_if);

	for (i = 0; i < _core_if->hwcfg2.b.num_host_chan + 1; i++)
	{
		hcchar_data_t    hcchar;
		hcchar.d32 = ltqusb_rreg(&_core_if->hc_regs[i]->hcchar);
		hcchar.b.chen  = 0;
		hcchar.b.chdis = 1;
		hcchar.b.epdir = 0;
		ltqusb_wreg(&_core_if->hc_regs[i]->hcchar, hcchar.d32);
	}
	/* Halt all channels to put them into a known state. */
	for (i = 0; i < _core_if->hwcfg2.b.num_host_chan + 1; i++)
	{
		hcchar_data_t    hcchar;
		int count = 0;

		hcchar.d32 = ltqusb_rreg(&_core_if->hc_regs[i]->hcchar);
		hcchar.b.chen  = 1;
		hcchar.b.chdis = 1;
		hcchar.b.epdir = 0;
		ltqusb_wreg(&_core_if->hc_regs[i]->hcchar, hcchar.d32);

		LTQ_DEBUGPL(DBG_HCDV, "%s: Halt channel %d\n", __func__, i);
		do{
			hcchar.d32 = ltqusb_rreg(&_core_if->hc_regs[i]->hcchar);
			if (++count > 1000)
			{
				LTQ_ERROR("%s: Unable to clear halt on channel %d\n", __func__, i);
				break;
			}
		} while (hcchar.b.chen);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void ltqusb_oc_int_on(int port);
void ltqusb_oc_int_off(int port);
/*!
	 \fn    void ltqusb_vbus_init(ltqusb_core_if_t *_core_if)
	 \brief This function init the VBUS control.
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
void ltqusb_vbus_init(ltqhcd_hcd_t *_ltqhcd)
{
	if(_ltqhcd->vbusgpioid<0)
		return;
	if(ltqusb_vbus_shared)
	{
		if(ltqusb_hcd_1.vbusgpioinstalled||ltqusb_hcd_2.vbusgpioinstalled)
		{
			ltqusb_hcd_1.vbusgpioinstalled=ltqusb_hcd_2.vbusgpioinstalled=1;
		}
		else
		{
			if(devm_gpio_request(_ltqhcd->dev, _ltqhcd->vbusgpioid,"USBVBus")==0)
			{
				gpio_direction_output(_ltqhcd->vbusgpioid,1);
				ltqusb_hcd_1.vbusgpioinstalled=ltqusb_hcd_2.vbusgpioinstalled=1;
				LTQ_PRINT("VBus1 and VBus2 registered(Shared) IO#%d\n",_ltqhcd->vbusgpioid);
			}
		}
	}
	else if(_ltqhcd->core_if.core_no==0)
	{
		if(!_ltqhcd->vbusgpioinstalled)
		{
			if(devm_gpio_request(_ltqhcd->dev, _ltqhcd->vbusgpioid,"USBVBus1")==0)
			{
				gpio_direction_output(_ltqhcd->vbusgpioid,1);
				_ltqhcd->vbusgpioinstalled=1;
				LTQ_PRINT("VBus1 registered IO#%d\n",_ltqhcd->vbusgpioid);
			}
		}
	}
	else
	{
		if(!_ltqhcd->vbusgpioinstalled)
		{
			if(devm_gpio_request(_ltqhcd->dev, _ltqhcd->vbusgpioid,"USBVBus2")==0)
			{
				gpio_direction_output(_ltqhcd->vbusgpioid,1);
				_ltqhcd->vbusgpioinstalled=1;
				LTQ_PRINT("VBus2 registered IO#%d\n",_ltqhcd->vbusgpioid);
			}
		}
	}
}


/*!
	 \fn    void ltqusb_vbus_free(ltqusb_core_if_t *_core_if)
	 \brief This function free the VBUS control.
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
void ltqusb_vbus_free(ltqhcd_hcd_t *_ltqhcd)
{
	if(_ltqhcd->vbusgpioid<0)
		return;
	if(ltqusb_vbus_shared)
	{
		if(ltqusb_hcd_1.vbusgpioinstalled||ltqusb_hcd_2.vbusgpioinstalled)
		{
			devm_gpio_free(_ltqhcd->dev, _ltqhcd->vbusgpioid);
			ltqusb_hcd_1.vbusgpioinstalled=ltqusb_hcd_2.vbusgpioinstalled=0;
		}
	}
	else
	{
		if(_ltqhcd->vbusgpioinstalled)
		{
			devm_gpio_free(_ltqhcd->dev, _ltqhcd->vbusgpioid);
			_ltqhcd->vbusgpioinstalled=0;
		}
	}
}

/*!
	 \fn    void ltqusb_vbus_on(ltqusb_core_if_t *_core_if)
	 \brief Turn on the USB 5V VBus Power
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
 */
void ltqusb_vbus_on(ltqhcd_hcd_t *_ltqhcd)
{
	if(_ltqhcd->vbusgpioid<0)
		return;
	if(_ltqhcd->vbusgpioinstalled)
	{
		__gpio_set_value(_ltqhcd->vbusgpioid,1);
		if(ltqusb_vbus_shared)
				ltqusb_hcd_1.vbusgpiostatus=ltqusb_hcd_2.vbusgpiostatus=1;
		else
			_ltqhcd->vbusgpiostatus=1;
	}
	if(_ltqhcd->core_if.core_no==0)
		ltqusb_oc_int_on(1);
	else
		ltqusb_oc_int_on(2);
}


/*!
	 \fn    void ltqusb_vbus_off(ltqusb_core_if_t *_core_if)
	 \brief Turn off the USB 5V VBus Power
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
void ltqusb_vbus_off(ltqhcd_hcd_t *_ltqhcd)
{
	if(_ltqhcd->vbusgpioid<0)
		return;
	if(_ltqhcd->vbusgpioinstalled)
	{
		__gpio_set_value(_ltqhcd->vbusgpioid,0);
		if(ltqusb_vbus_shared)
				ltqusb_hcd_1.vbusgpiostatus=ltqusb_hcd_2.vbusgpiostatus=0;
		else
			_ltqhcd->vbusgpiostatus=0;
	}
	if(_ltqhcd->core_if.core_no==0)
		ltqusb_oc_int_off(1);
	else
		ltqusb_oc_int_off(2);
}


/*!
	 \fn    int ltqusb_vbus(ltqusb_core_if_t *_core_if)
	 \brief Read Current VBus status
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
int ltqusb_vbus(ltqhcd_hcd_t *_ltqhcd)
{
	if(_ltqhcd->vbusgpioid<0)
		return -1;
	if(_ltqhcd->vbusgpioinstalled)
			return (_ltqhcd->vbusgpiostatus);
		return -1;

}


/////////////////////////////////////////////////////////////////////////









//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define OC_Timer_Stable 3
#define OC_Timer_Sleep 200
#define OC_Timer_Max 3

extern ltqhcd_hcd_t *oc1_int_target;
extern ltqhcd_hcd_t *oc2_int_target;
unsigned int  oc1_int_installed=0;
unsigned int  oc2_int_installed=0;
unsigned int  oc1_int_count=0;
unsigned int  oc2_int_count=0;
struct timer_list oc1_retry_timer;
struct timer_list oc2_retry_timer;
unsigned int  oc1_int_no=0;
unsigned int  oc2_int_no=0;

void oc_retry_timer_func(unsigned long arg)
{
	if(arg==1)
	{
		if(oc1_int_installed==0) //not installed
		{
		}
		else if(oc1_int_installed==1) //disabled
		{
		}
		else if(oc1_int_installed==2) //stablizing
		{
			oc1_int_installed=4;
			oc1_int_count=0;
		}
		else if(oc1_int_installed==3) // sleeping
		{
			mod_timer(&oc1_retry_timer,jiffies +  HZ*OC_Timer_Stable);
			oc1_int_installed=2;
			enable_irq(oc1_int_no);
		}
		else if(oc1_int_installed==4) //
		{
			oc1_int_count=0;
		}
		else if(oc1_int_installed==5) // Stable sleeping
		{
			mod_timer(&oc1_retry_timer,jiffies +  HZ*OC_Timer_Stable);
			oc1_int_installed=4;
			enable_irq(oc1_int_no);
		}
		else
		{
		}
	}
	else
	{
		if(oc2_int_installed==0) //not installed
		{
		}
		else if(oc2_int_installed==1) //disabled
		{
		}
		else if(oc2_int_installed==2) //stablizing
		{
			oc2_int_installed=4;
			oc2_int_count=0;
		}
		else if(oc2_int_installed==3) // sleeping
		{
			mod_timer(&oc2_retry_timer,jiffies +  HZ*OC_Timer_Stable);
			oc2_int_installed=2;
			enable_irq(oc2_int_no);
		}
		else if(oc2_int_installed==4) //
		{
			oc2_int_count=0;
		}
		else if(oc2_int_installed==5) // Stable sleeping
		{
			mod_timer(&oc2_retry_timer,jiffies +  HZ*OC_Timer_Stable);
			oc2_int_installed=4;
			enable_irq(oc2_int_no);
		}
		else
		{
		}
	}
}

irqreturn_t ltqhcd_oc_irq(int _irq , void *_dev)
{
	int32_t retval=1;
	//ltqhcd_hcd_t *ltqhcd= _dev;
	if(_irq==oc1_int_no)
	{
		disable_irq_nosync(oc1_int_no);
		if(oc1_int_installed==0)      //not installed
		{
		}
		else if(oc1_int_installed==1) //disabled
		{
		}
		else if(oc1_int_installed==2) //stablizing
		{
			mod_timer(&oc1_retry_timer,jiffies +  HZ/OC_Timer_Sleep);
			oc1_int_installed=3;
		}
		else if(oc1_int_installed==3) // sleeping
		{
		}
		else if(oc1_int_installed==4) //
		{
			oc1_int_count++;
			if(oc1_int_count>=OC_Timer_Max)
			{
				LTQ_DEBUGP("OC INTERRUPT port #1\n");
				oc1_int_target->flags.b.port_over_current_change = 1;
				if(ltqusb_oc_shared)
					oc2_int_target->flags.b.port_over_current_change = 1;
				ltqusb_vbus_off(oc1_int_target);
				if(ltqusb_oc_shared)
					ltqusb_vbus_off(oc2_int_target);
				LTQ_DEBUGP("Turning off port #1\n");
			}
			else
			{
				mod_timer(&oc1_retry_timer,jiffies +  HZ/OC_Timer_Sleep);
				oc1_int_installed=5;
			}
		}
		else if(oc1_int_installed==5) // Stable sleeping
		{
		}
	}
	else
	{
		disable_irq_nosync(oc2_int_no);
		if(oc2_int_installed==0)      //not installed
		{
		}
		else if(oc2_int_installed==1) //disabled
		{
		}
		else if(oc2_int_installed==2) //stablizing
		{
			mod_timer(&oc2_retry_timer,jiffies +  HZ/OC_Timer_Sleep);
			oc2_int_installed=3;
		}
		else if(oc2_int_installed==3) // sleeping
		{
		}
		else if(oc2_int_installed==4) //
		{
			oc2_int_count++;
			if(oc2_int_count>=OC_Timer_Max)
			{
				LTQ_DEBUGP("OC INTERRUPT port #2\n");
				oc2_int_target->flags.b.port_over_current_change = 1;
				ltqusb_vbus_off(oc2_int_target);
				LTQ_DEBUGP("Turning off port #2\n");
			}
			else
			{
				mod_timer(&oc2_retry_timer,jiffies +  HZ/OC_Timer_Sleep);
				oc2_int_installed=5;
			}
		}
		else if(oc2_int_installed==5) // Stable sleeping
		{
		}
	}
	return IRQ_RETVAL(retval);
}

void ltqusb_oc_int_on(int port)
{
	if     (ltqusb_oc_shared || (port==1&&oc1_int_target && oc1_int_no>0))
	{
		if(oc1_int_installed==0)
		{
			//LTQ_DEBUGPL( DBG_CIL, "registering (overcurrent) handler for port #1 irq#%d\n", oc1_int_no);
			LTQ_PRINT("registering (overcurrent) handler for port #1 irq#%d\n", oc1_int_no);
			oc1_int_installed=2;
			init_timer(&oc1_retry_timer);
			oc1_retry_timer.function = oc_retry_timer_func;
			oc1_retry_timer.data=1;
			if(request_irq(oc1_int_no, &ltqhcd_oc_irq,
					       IRQF_TRIGGER_NONE
//					     | IRQF_TRIGGER_RISING
//					     | IRQF_TRIGGER_FALLING
//					     | IRQF_TRIGGER_HIGH
//					     | IRQF_TRIGGER_LOW
//					     | IRQF_TRIGGER_PROBE
					     | IRQF_DISABLED
//					     | IRQF_SAMPLE_RANDOM
//					     | IRQF_SHARED
//					     | IRQF_PROBE_SHARED
//					     | IRQF_TIMER
//					     | IRQF_PERCPU
//					     | IRQF_NOBALANCING
//					     | IRQF_IRQPOLL
//					     | IRQF_ONESHOT
				    ,
				     "ltqusb1_oc", (void *)oc1_int_target))
				oc1_int_installed=0;
			else
				mod_timer(&oc1_retry_timer,jiffies +  HZ*OC_Timer_Stable);
		}
		else if(oc1_int_installed!=2 && oc1_int_installed!=4 )
		{
			oc1_int_installed=2;
			enable_irq(oc1_int_no);
			mod_timer(&oc1_retry_timer,jiffies +  HZ*OC_Timer_Stable);
		}
	}
	else if((port==2&&oc2_int_target && oc2_int_no>0))
	{
		if(oc2_int_installed==0)
		{
			//LTQ_DEBUGPL( DBG_CIL, "registering (overcurrent) handler for port #2 irq#%d\n", oc2_int_no);
			LTQ_PRINT("registering (overcurrent) handler for port #2 irq#%d\n", oc2_int_no);
			oc2_int_installed=2;
			init_timer(&oc2_retry_timer);
			oc2_retry_timer.function = oc_retry_timer_func;
			oc2_retry_timer.data=2;
			if(request_irq(oc2_int_no, &ltqhcd_oc_irq,
					       IRQF_TRIGGER_NONE
//					     | IRQF_TRIGGER_RISING
//					     | IRQF_TRIGGER_FALLING
//					     | IRQF_TRIGGER_HIGH
//					     | IRQF_TRIGGER_LOW
//					     | IRQF_TRIGGER_PROBE
				     | IRQF_DISABLED
//					     | IRQF_SAMPLE_RANDOM
//					     | IRQF_SHARED
//					     | IRQF_PROBE_SHARED
//					     | IRQF_TIMER
//					     | IRQF_PERCPU
//					     | IRQF_NOBALANCING
//					     | IRQF_IRQPOLL
//					     | IRQF_ONESHOT
				    ,
			     "ltqusb2_oc", (void *)oc2_int_target))
				oc2_int_installed=0;
			else
				mod_timer(&oc2_retry_timer,jiffies +  HZ*OC_Timer_Stable);
		}
		else if(oc2_int_installed!=2 && oc2_int_installed!=4 )
		{
			oc2_int_installed=2;
			enable_irq(oc2_int_no);
			mod_timer(&oc2_retry_timer,jiffies +  HZ*OC_Timer_Stable);
		}
	}
}


void ltqusb_oc_int_off(int port)
{
	if(port==1)
	{
		disable_irq_nosync(oc1_int_no);
		if(oc1_int_installed)
			oc1_int_installed=1;
	}
	else
	{
		disable_irq_nosync(oc2_int_no);
		if(oc2_int_installed)
			oc2_int_installed=1;
	}
}

uint32_t ltqusb_oc_get_hy(int port)
{
	uint32_t data=0;
	if(ltqusb_chip_family== LTQUSB_CHIP_AR10)
	{
		if(port==1)
			data = ltqusb_rreg((volatile uint32_t *)AR10_RCU_USB1CFG);
		else
			data = ltqusb_rreg((volatile uint32_t *)AR10_RCU_USB2CFG);
		data = (data >> AR10_USBCFG_OC_HY_BIT ) & AR10_USBCFG_OC_HY_MASK;
	}
	return data;
}

void ltqusb_oc_set_hy(int port,uint32_t setting)
{
	if(ltqusb_chip_family== LTQUSB_CHIP_AR10)
	{
		uint32_t data;
		if(port==1)
			data = ltqusb_rreg((volatile uint32_t *)AR10_RCU_USB1CFG);
		else
			data = ltqusb_rreg((volatile uint32_t *)AR10_RCU_USB2CFG);

		data &= (~(AR10_USBCFG_OC_HY_MASK << AR10_USBCFG_OC_HY_BIT));
		data |= ((setting & AR10_USBCFG_OC_HY_MASK)<< AR10_USBCFG_OC_HY_BIT);

		if(port==1)
			ltqusb_wreg((volatile uint32_t *)AR10_RCU_USB1CFG,data);
		else
			ltqusb_wreg((volatile uint32_t *)AR10_RCU_USB2CFG, data);
	}
}

void ltqusb_oc_int_free(int port)
{
	if(ltqusb_chip_family== LTQUSB_CHIP_AR10) // ar10
	{
		if(port==1)
		{
			del_timer(&oc1_retry_timer);
			if(oc1_int_no>0)
			{
				disable_irq_nosync(oc1_int_no);
				free_irq(oc1_int_no, (void *)oc1_int_target);
			}
			oc1_int_installed=0;
		}
		else
		{
			del_timer(&oc2_retry_timer);
			if(oc2_int_no>0)
			{
				disable_irq_nosync(oc2_int_no);
				free_irq(oc2_int_no, (void *)oc2_int_target);
			}
			oc2_int_installed=0;
		}
	}
	else
	{
		del_timer(&oc1_retry_timer);
		if(oc1_int_no>0)
		{
			disable_irq_nosync(oc1_int_no);
			free_irq(oc1_int_no, (void *)oc1_int_target);
		}
		oc1_int_installed=0;
	}
}


