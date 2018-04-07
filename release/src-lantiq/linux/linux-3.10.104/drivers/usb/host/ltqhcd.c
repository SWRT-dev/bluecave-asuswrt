/*****************************************************************************
 **   FILE NAME       : ltqhcd.c
 **   PROJECT         : Lantiq USB sub-system V3
 **   MODULES         : Lantiq USB sub-system Host and Device driver
 **   SRC VERSION     : 3.2
 **   DATE            : 1/Jan/2011
 **   AUTHOR          : Chen, Howard
 **   DESCRIPTION     : This file contains the structures, constants, and
 **                     interfaces for the Host Contoller Driver (HCD).
 **
 **                     The Host Controller Driver (HCD) is responsible for
 **                     translating requests from the USB Driver into the
 **                     appropriate actions on the LTQUSB controller.
 **                     It isolates the USBD from the specifics of the
 **                     controller by providing an API to the USBD.
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
  \file ltqhcd.c
  \ingroup LTQUSB_DRIVER_V3
  \brief This file contains the implementation of the HCD. In Linux,
   the HCD implements the hc_driver API.
*/

#include <linux/version.h>
#include "ltqusb_version.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/device.h>

#include <linux/errno.h>
#include <linux/list.h>
#include <linux/interrupt.h>
#include <linux/string.h>

#include <linux/dma-mapping.h>


#include "ltqusb_plat.h"
#include "ltqusb_regs.h"
#include "ltqusb_cif.h"
#include "ltqhcd.h"

#include <asm/irq.h>


uint8_t ltqusb_chip_family=0      ;

#ifdef __DEBUG__
	static
	void dump_urb_info(struct urb *_urb, char* _fn_name);
#if 0
	static
	void dump_channel_info(ltqhcd_hcd_t *_ltqhcd,
	                               ltqhcd_hc_t *_ltqhc);
#endif
#endif

static
void ltqhcd_complete_urb_sub(ltqhcd_urbd_t *_urbd)
{
	ltqhcd_hcd_t *ltqhcd;
	struct urb *urb=NULL;
	ltqhcd_epqh_t *epqh;

	_urbd->hc = 0;
	if(!_urbd->epqh)
	{
		LTQ_ERROR("%s: urb(%p) invalid epqh\n",
		          __func__,_urbd);
		return;
	}
	epqh=_urbd->epqh;

	LOCK_URBD_RELEASE_LIST(epqh);
	if (!list_empty(&_urbd->ql))
	{
		list_del_init(&_urbd->ql);
		epqh->urbd_count--;
	}
	else
	{
		LTQ_ERROR("%s: urb(%p) not connect to any epqh\n",
		          __func__,_urbd);
	}
	UNLOCK_URBD_RELEASE_LIST(epqh);

	ltqhcd=epqh->ltqhcd;
	urb   =_urbd->urb;
	if(!urb)
		LTQ_ERROR("%s: invalid urb\n",__func__);
	else if(urb->hcpriv)
	{
		if(urb->hcpriv != _urbd)
			LTQ_ERROR("%s: invalid"
			          " urb(%p)->hcpriv(%p) != _urbd(%p)\n",
			          __func__,
			          urb,
			          urb->hcpriv,
			          _urbd);
		#if   defined(__UNALIGNED_BUF_ADJ__)
			if(_urbd->aligned_buf)
			{
				if(_urbd->is_in)
					memcpy(_urbd->xfer_buff,
					       _urbd->aligned_buf,
					       urb->actual_length);
				ltqusb_free_buf_h(_urbd->aligned_buf);
				_urbd->aligned_buf=0;
			}
		#endif
		urb->hcpriv = NULL;
		#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
			urb->status=_urbd->status;
			usb_hcd_giveback_urb(ltqhcd_to_syshcd(ltqhcd), urb);
		#else
			usb_hcd_giveback_urb(ltqhcd_to_syshcd(ltqhcd), urb,
			                     _urbd->status);
		#endif
	}
	kfree(_urbd);
	if(epqh->cancelling)
		if (list_empty(&epqh->urbd_list))
			ltqhcd_epqh_free(epqh);
}

static
void ltqhcd_complete_urb_func(unsigned long data)
{
	unsigned long             flags;
	ltqhcd_urbd_t *urbd;
	ltqhcd_epqh_t *epqh;
	struct list_head *item;

	int count=10;

	epqh=((ltqhcd_epqh_t *)data);

#if 1
	if(epqh->complete_inuse)
		return;
#endif
	epqh->complete_inuse=1;

	while (!list_empty(&epqh->release_list) && count)
	{
		item = epqh->release_list.next;
		urbd = list_entry(item, ltqhcd_urbd_t, ql);
		if (!urbd)
			LTQ_ERROR("%s: invalid urbd\n",__func__);
		else if (!urbd->epqh)
			LTQ_ERROR("%s: invalid epqh\n",__func__);
		else
		{
			local_irq_save(flags);
			ltqhcd_complete_urb_sub(urbd);
			local_irq_restore (flags);
		}
		count--;
	}
	if(!list_empty(&epqh->release_list))
		tasklet_schedule(&epqh->complete_urb_sub);
	epqh->complete_inuse=0;
}

/*!
 \brief Sets the final status of an URB and returns it to the device
  driver. Any required cleanup of the URB is performed.
 */
void ltqhcd_complete_urb(ltqhcd_hcd_t *_ltqhcd,
                         ltqhcd_urbd_t *_urbd,
                         int _status)
{
	unsigned long             flags;

	if(!_urbd)
	{
		LTQ_ERROR("%s: invalid urbd\n",__func__);
		return;
	}
	_urbd->hc=0;
	if (!_urbd->epqh)
	{
		LTQ_ERROR("%s: invalid epqh\n",__func__);
		return;
	}

	if(_urbd->cancelling && _urbd->cancelling_status)
		_urbd->status = _urbd->cancelling_status;
	else
		_urbd->status = _status;

	#ifdef __DEBUG__
		if (CHK_DEBUG_LEVEL(DBG_HCDV)
		     || (_urbd->epqh->ep_type == LTQUSB_EP_TYPE_CTRL && CHK_DEBUG_LEVEL(DBG_HCD_URB_CTRL))
		     || (_urbd->epqh->ep_type == LTQUSB_EP_TYPE_ISOC &&  _urbd->is_in && CHK_DEBUG_LEVEL(DBG_HCD_URB_ISOC_IN))
		     || (_urbd->epqh->ep_type == LTQUSB_EP_TYPE_ISOC && !_urbd->is_in && CHK_DEBUG_LEVEL(DBG_HCD_URB_ISOC_OUT))
		     || (_urbd->epqh->ep_type == LTQUSB_EP_TYPE_BULK &&  _urbd->is_in && CHK_DEBUG_LEVEL(DBG_HCD_URB_BULK_IN))
		     || (_urbd->epqh->ep_type == LTQUSB_EP_TYPE_BULK && !_urbd->is_in && CHK_DEBUG_LEVEL(DBG_HCD_URB_BULK_OUT))
		     || (_urbd->epqh->ep_type == LTQUSB_EP_TYPE_INTR &&  _urbd->is_in && CHK_DEBUG_LEVEL(DBG_HCD_URB_INTR_IN))
		     || (_urbd->epqh->ep_type == LTQUSB_EP_TYPE_INTR && !_urbd->is_in && CHK_DEBUG_LEVEL(DBG_HCD_URB_INTR_OUT))
		    )
		{
			LTQ_PRINT("%s: ehqh %p _urbd %p, urb %p\n"
			          "    device %d, ep %d %s/%s\n"
			          "    actual_transfered=%d status=%d\n",
				  __func__,_urbd->epqh,
				  _urbd,_urbd->urb,
				  (_urbd->urb)?usb_pipedevice(_urbd->urb->pipe):-1,
				  (_urbd->urb)?usb_pipeendpoint(_urbd->urb->pipe):-1,
				  (_urbd->urb)?(usb_pipein(_urbd->urb->pipe) ? "IN" : "OUT"):"--",
				  (_urbd->is_in) ? "IN" : "OUT",
				  (_urbd->urb)?_urbd->urb->actual_length:-1,
				   _urbd->status);
			if ((_urbd->urb)&& _urbd->epqh->ep_type == LTQUSB_EP_TYPE_ISOC)
			{
				int i;
				for (i = 0; i < _urbd->urb->number_of_packets; i++)
					LTQ_PRINT("  ISO Desc %d status: %d\n", i, _urbd->urb->iso_frame_desc[i].status);
			}
		}
	#endif

	local_irq_save(flags);
	if(_urbd->phase!=URBD_FINISHING)
	{
		if(_urbd->phase!=URBD_DEQUEUEING && _urbd->phase!=URBD_COMPLETING)
			printk(KERN_INFO "Warning: %s() Strange URBD PHASE %d\n",__func__,_urbd->phase);
		if(_urbd->urb)
		{
			if((   _urbd->status == 0
			    && _urbd->phase==URBD_COMPLETING
			   )
			   || in_irq())
			{
				_urbd->phase=URBD_FINISHING;
				LOCK_URBD_LIST(_urbd->epqh);
				list_del_init(&_urbd->ql);
				UNLOCK_URBD_LIST(_urbd->epqh);
				LOCK_URBD_RELEASE_LIST(_urbd->epqh);
				list_add_tail(&_urbd->ql,&_urbd->epqh->release_list);
				UNLOCK_URBD_RELEASE_LIST(_urbd->epqh);
				if(!_urbd->epqh->complete_urb_sub.func)
				{
					_urbd->epqh->complete_urb_sub.next = NULL;
					_urbd->epqh->complete_urb_sub.state = 0;
					atomic_set( &_urbd->epqh->complete_urb_sub.count, 0);
					_urbd->epqh->complete_urb_sub.func = ltqhcd_complete_urb_func;
					_urbd->epqh->complete_urb_sub.data = (unsigned long)_urbd->epqh;
				}
				tasklet_schedule(&_urbd->epqh->complete_urb_sub);
			}
			else
			{
				_urbd->phase=URBD_FINISHING;
				LOCK_URBD_LIST(_urbd->epqh);
				list_del_init(&_urbd->ql);
				UNLOCK_URBD_LIST(_urbd->epqh);
				LOCK_URBD_RELEASE_LIST(_urbd->epqh);
				list_add_tail(&_urbd->ql,&_urbd->epqh->release_list);
				UNLOCK_URBD_RELEASE_LIST(_urbd->epqh);
				ltqhcd_complete_urb_sub(_urbd);
			}
		}
		else
		{
			kfree(_urbd);
		}
	}
	else
	{
		printk(KERN_INFO "Warning: %s() Double Completing \n",__func__);
	}

	local_irq_restore (flags);
}

/*!
 \brief Processes all the URBs in a single EPQHs. Completes them with
        status and frees the URBD.
 */
static
void kill_all_urbs_in_epqh(ltqhcd_hcd_t *_ltqhcd, ltqhcd_epqh_t *_epqh, int _status)
{
	struct list_head *item;
	struct list_head *next;
	ltqhcd_urbd_t    *urbd;

	if(!_epqh)
		return;

	LTQ_DEBUGPL(DBG_HCDV, "%s %p\n",__func__,_epqh);
	LOCK_URBD_LIST(_epqh);
	_epqh->cancelling=1;
#if 0
	list_for_each(item, &_epqh->urbd_list)
	{
		urbd = list_entry(item, ltqhcd_urbd_t, ql);
		if(   urbd->phase==URBD_IDLE
		   || urbd->phase==URBD_ACTIVE
		  )
			urbd->phase=URBD_DEQUEUEING;
	}
	list_for_each_safe(item, next, &_epqh->urbd_list)
	{
		urbd = list_entry(item, ltqhcd_urbd_t, ql);
		urbd->cancelling=1;
		if(urbd->hc) urbd->hc->cancelling=1;
		if(urbd->phase==URBD_DEQUEUEING)
		{
			urbd->urb->status = _status;
			urbd->phase = URBD_FINISHING;
			ltqhcd_complete_urb_sub(urbd);
			ltqhcd_epqh_idle(_epqh);
		}
		else if(   urbd->phase==URBD_RUNNING )
		{
			if(ltqhcd_hc_halt(&_ltqhcd->core_if, urbd->hc, HC_XFER_URB_DEQUEUE))
			{
				urbd->urb->status = _status;
				urbd->phase = URBD_FINISHING;
				ltqhcd_complete_urb_sub(urbd);
				ltqhcd_epqh_idle(_epqh);
			}
		}
		else if(   urbd->phase==URBD_FINISHING )
		{
		}
		else
			LTQ_ERROR("%s: invalid urb phase:%d \n",__func__,urbd->phase);
	}
#else
	list_for_each_safe(item, next, &_epqh->urbd_list)
	{
		urbd = list_entry(item, ltqhcd_urbd_t, ql);
		urbd->cancelling=1;
		if(urbd->hc) urbd->hc->cancelling=1;
		if(   urbd->phase==URBD_IDLE
		   || urbd->phase==URBD_ACTIVE
		  )
		{
			urbd->urb->status = _status;
			urbd->phase = URBD_DEQUEUEING;
			UNLOCK_URBD_LIST(_epqh);
			ltqhcd_complete_urb(_ltqhcd, urbd, _status);
			ltqhcd_epqh_idle(_epqh);
			LOCK_URBD_LIST(_epqh);
		}
		else if(   urbd->phase==URBD_RUNNING )
		{
			if(ltqhcd_hc_halt(&_ltqhcd->core_if, urbd->hc, HC_XFER_URB_DEQUEUE))
			{
				urbd->urb->status = _status;
				urbd->phase = URBD_DEQUEUEING;
				UNLOCK_URBD_LIST(_epqh);
				ltqhcd_complete_urb(_ltqhcd, urbd, _status);
				ltqhcd_epqh_idle(_epqh);
				LOCK_URBD_LIST(_epqh);
			}
		}
		else if(   urbd->phase==URBD_FINISHING )
		{
		}
		else
			LTQ_ERROR("%s: invalid urb phase:%d \n",__func__,urbd->phase);
	}
#endif
	UNLOCK_URBD_LIST(_epqh);
	LTQ_DEBUGPL(DBG_HCDV, "%s %p finish\n",__func__,_epqh);
}


/*!
 \brief Free all EPS in one Processes all the URBs in a single list of EPQHs. Completes them with
        -ETIMEDOUT and frees the URBD.
 */
static
void epqh_list_free(ltqhcd_hcd_t *_ltqhcd, struct list_head *_epqh_list)
{
	ltqhcd_epqh_t    *epqh;
	struct list_head *item;
	if (!_ltqhcd)
		return;
	if (!_epqh_list)
		return;

	LTQ_DEBUGPL(DBG_HCDV, "%s %p\n",__func__,_epqh_list);

	item = _epqh_list->next;
	while(item != _epqh_list && item != item->next)
	{
		epqh = list_entry(item, ltqhcd_epqh_t, ql);
		epqh->phase=EPQH_DISABLING;
		item = item->next;
#ifdef NEWRET
		kill_all_urbs_in_epqh(_ltqhcd, epqh, -ESHUTDOWN);
#else
		kill_all_urbs_in_epqh(_ltqhcd, epqh, -ETIMEDOUT);
#endif
	}

	item = _epqh_list->next;
	while(item != _epqh_list && item != item->next)
	{
		epqh = list_entry(item, ltqhcd_epqh_t, ql);
		epqh->phase=EPQH_DISABLING;
		item = item->next;
		if(list_empty(&epqh->urbd_list) && list_empty(&epqh->release_list))
			ltqhcd_epqh_free(epqh);
	}

	LTQ_DEBUGPL(DBG_HCDV, "%s %p finish\n",__func__,_epqh_list);
	/* Ensure there are no URBDs or URBs left. */
}

static
void epqh_list_free_2(ltqhcd_hcd_t *_ltqhcd, struct list_head *_epqh_list)
{
	ltqhcd_epqh_t    *epqh;
	struct list_head *item;
	struct list_head *next;
	if (!_ltqhcd)
		return;
	if (!_epqh_list)
		return;

	LTQ_DEBUGPL(DBG_HCDV, "%s %p\n",__func__,_epqh_list);
	list_for_each_safe(item, next, _epqh_list)
	{
		epqh = list_entry(item, ltqhcd_epqh_t, ql);
		if(item == item->next)
		{
			ltqhcd_epqh_free(epqh);
		}
		else
		{
			uint32_t count=0x80000;
			for(;(!list_empty(&epqh->urbd_list) || !list_empty(&epqh->release_list))&& count> 0; count--) udelay(1);
			if(!count)
			{
//				LTQ_ERROR("%s: unable to clear urbd in epqh \n",__func__);
			}
			else
				ltqhcd_epqh_free(epqh);
		}
	}
	LTQ_DEBUGPL(DBG_HCDV, "%s %p finish\n",__func__,_epqh_list);
	/* Ensure there are no URBDs or URBs left. */
}

static
void epqh_list_free_all_sub(unsigned long data)
{
	ltqhcd_hcd_t *ltqhcd;

	ltqhcd=(ltqhcd_hcd_t *)data;
	epqh_list_free(ltqhcd, &ltqhcd->epqh_list_ctrl);
	epqh_list_free(ltqhcd, &ltqhcd->epqh_list_bulk);
	epqh_list_free(ltqhcd, &ltqhcd->epqh_list_intr);
	#ifdef __EN_ISOC__
		epqh_list_free(ltqhcd, &ltqhcd->epqh_list_isoc);
	#endif

	epqh_list_free_2(ltqhcd, &ltqhcd->epqh_list_ctrl);
	epqh_list_free_2(ltqhcd, &ltqhcd->epqh_list_bulk);
	epqh_list_free_2(ltqhcd, &ltqhcd->epqh_list_intr);
	#ifdef __EN_ISOC__
		epqh_list_free_2(ltqhcd, &ltqhcd->epqh_list_isoc);
	#endif
}

static
void epqh_list_free_all(ltqhcd_hcd_t *_ltqhcd)
{
#if 1
	_ltqhcd->tasklet_free_epqh_list.next = NULL;
	_ltqhcd->tasklet_free_epqh_list.state = 0;
	atomic_set( &_ltqhcd->tasklet_free_epqh_list.count, 0);
	_ltqhcd->tasklet_free_epqh_list.func=epqh_list_free_all_sub;
	_ltqhcd->tasklet_free_epqh_list.data = (unsigned long)_ltqhcd;
	tasklet_schedule(&_ltqhcd->tasklet_free_epqh_list);
#else
	epqh_list_free_all_sub((unsigned long)_ltqhcd);
#endif
}


/*!
   \brief This function is called to handle the disconnection of host port.
 */
int32_t ltqhcd_disconnect(ltqhcd_hcd_t *_ltqhcd)
{
	LTQ_DEBUGPL(DBG_HCDV, "%s(%p)\n", __func__, _ltqhcd);

	_ltqhcd->disconnecting=1;
	/* Set status flags for the hub driver. */
	_ltqhcd->flags.b.port_connect_status_change = 1;
	_ltqhcd->flags.b.port_connect_status = 0;

	/*
	 * Shutdown any transfers in process by clearing the Tx FIFO Empty
	 * interrupt mask and status bits and disabling subsequent host
	 * channel interrupts.
	 */
	 {
		gint_data_t intr = { .d32 = 0 };
		intr.b.nptxfempty = 1;
		intr.b.ptxfempty  = 1;
		intr.b.hcintr     = 1;
		ltqusb_mreg (&_ltqhcd->core_if.core_global_regs->gintmsk, intr.d32, 0);
		ltqusb_mreg (&_ltqhcd->core_if.core_global_regs->gintsts, intr.d32, 0);
	}

	/* Respond with an error status to all URBs in the schedule. */
	epqh_list_free_all(_ltqhcd);

	/* Clean up any host channels that were in use. */
	{
		int               num_channels;
		ltqhcd_hc_t      *channel;
		ltqusb_hc_regs_t *hc_regs;
		hcchar_data_t     hcchar;
		int	              i;

		num_channels = _ltqhcd->core_if.params.host_channels;

		for (i = 0; i < num_channels; i++)
		{
			channel = &_ltqhcd->ltqhc[i];
			hc_regs = _ltqhcd->core_if.hc_regs[i];
			hcchar.d32 = ltqusb_rreg(&hc_regs->hcchar);
			if (hcchar.b.chen)
				printk(KERN_INFO "Warning: %s() HC still enabled\n",__func__);
			ltqhcd_hc_cleanup(&_ltqhcd->core_if, channel);
		}
	}
	//UNLOCK_ENQUEUE(_ltqhcd);
	LTQ_DEBUGPL(DBG_HCDV, "%s(%p) finish\n", __func__, _ltqhcd);

	usb_hcd_poll_rh_status(_ltqhcd->syshcd);

	return 1;
}


/*!
   \brief Frees secondary storage associated with the ltqhcd_hcd structure contained
          in the struct usb_hcd field.
 */
static
void ltqhcd_freeextra(struct usb_hcd *_syshcd)
{
	ltqhcd_hcd_t 	*ltqhcd = syshcd_to_ltqhcd(_syshcd);

	LTQ_DEBUGPL(DBG_HCD, "LTQUSB HCD FREE\n");

	/* Free memory for EPQH/URBD lists */
	epqh_list_free_all(ltqhcd);

	/* Free memory for the host channels. */
	ltqusb_free_buf_h(ltqhcd->status_buf);
	return;
}

/*!
   \brief Initializes the HCD. This function allocates memory for and initializes the
  static parts of the usb_hcd and ltqhcd_hcd structures. It also registers the
  USB bus with the core and calls the hc_driver->start() function. It returns
  a negative error on failure.
 */
extern enum hrtimer_restart xfer_stopper_timer_callback(struct hrtimer *pt);
int ltqhcd_init(ltqhcd_hcd_t *_ltqhcd)
{
	int retval = 0;
	struct usb_hcd *syshcd = NULL;

	LTQ_DEBUGPL(DBG_HCD, "Lantiq USB HCD INIT\n");

	INIT_EPQH_LIST_ALL(_ltqhcd);
	INIT_EPQH_LIST(_ltqhcd);
	INIT_INTR(_ltqhcd);
	INIT_ENQUEUE(_ltqhcd);
	INIT_SELECT(_ltqhcd);

	init_timer(&_ltqhcd->autoprobe_timer);
	init_timer(&_ltqhcd->host_probe_timer);
	_ltqhcd->probe_sec = 5;
	_ltqhcd->autoprobe_sec = 30;

	_ltqhcd->hc_driver.description      = _ltqhcd->core_if.core_name;
	_ltqhcd->hc_driver.product_desc     = "Lantiq USB Controller";
	//_ltqhcd->hc_driver.hcd_priv_size    = sizeof(ltqhcd_hcd_t);
	_ltqhcd->hc_driver.hcd_priv_size    = sizeof(unsigned long);
	_ltqhcd->hc_driver.irq              = ltqhcd_irq;
	_ltqhcd->hc_driver.flags            = HCD_MEMORY | HCD_USB2;
	_ltqhcd->hc_driver.start            = ltqhcd_start;
	_ltqhcd->hc_driver.stop             = ltqhcd_stop;
	//_ltqhcd->hc_driver.reset          =
	//_ltqhcd->hc_driver.suspend        =
	//_ltqhcd->hc_driver.resume         =
	_ltqhcd->hc_driver.urb_enqueue      = ltqhcd_urb_enqueue;
	_ltqhcd->hc_driver.urb_dequeue      = ltqhcd_urb_dequeue;
	_ltqhcd->hc_driver.endpoint_disable = ltqhcd_endpoint_disable;
	_ltqhcd->hc_driver.get_frame_number = ltqhcd_get_frame_number;
	_ltqhcd->hc_driver.hub_status_data  = ltqhcd_hub_status_data;
	_ltqhcd->hc_driver.hub_control      = ltqhcd_hub_control;
	//_ltqhcd->hc_driver.hub_suspend    =
	//_ltqhcd->hc_driver.hub_resume     =

	_ltqhcd->xfer_prot1_usec= XFER_PROT1_USEC;
	_ltqhcd->xfer_prot2_usec= XFER_PROT2_USEC;
	_ltqhcd->xfer_prot1_value= XFER_PROT1_CALC(_ltqhcd->xfer_prot1_usec);
	_ltqhcd->xfer_prot2_value= XFER_PROT2_CALC(_ltqhcd->xfer_prot2_usec);

	#ifdef __BURST_XFER__
		_ltqhcd->burst =1;
	#endif
	#ifdef __SINGLE_XACT__
		_ltqhcd->singlexact =1;
	#endif

	printk(KERN_INFO "hcd Prot1:%d %04x %u   Prot2:%d %lu SingleXact:%d\n",_ltqhcd->xfer_prot1_usec,_ltqhcd->xfer_prot1_value,_ltqhcd->xfer_prot1_value,_ltqhcd->xfer_prot2_usec,_ltqhcd->xfer_prot2_value,_ltqhcd->singlexact);

	/* Allocate memory for and initialize the base HCD and  */
	//syshcd = usb_create_hcd(&_ltqhcd->hc_driver, _ltqhcd->dev, _ltqhcd->dev->bus_id);
	syshcd = usb_create_hcd(&_ltqhcd->hc_driver, _ltqhcd->dev, _ltqhcd->core_if.core_name);

	if (syshcd == NULL)
	{
		retval = -ENOMEM;
		goto error1;
	}

	syshcd->rsrc_start = (unsigned long)_ltqhcd->core_if.core_global_regs;
	syshcd->regs       = (void *)_ltqhcd->core_if.core_global_regs;
	syshcd->self.otg_port = 0;

	//*((unsigned long *)(&(syshcd->hcd_priv)))=(unsigned long)_ltqhcd;
	//*((unsigned long *)(&(syshcd->hcd_priv[0])))=(unsigned long)_ltqhcd;
	syshcd->hcd_priv[0]=(unsigned long)_ltqhcd;
	_ltqhcd->syshcd=syshcd;
	INIT_LIST_HEAD(&_ltqhcd->epqh_list_all   );
	INIT_LIST_HEAD(&_ltqhcd->epqh_list_ctrl  );
	INIT_LIST_HEAD(&_ltqhcd->epqh_list_bulk  );
	INIT_LIST_HEAD(&_ltqhcd->epqh_list_intr  );
	#ifdef __EN_ISOC__
		INIT_LIST_HEAD(&_ltqhcd->epqh_list_isoc);
	#endif

	/*
	 * Create a host channel descriptor for each host channel implemented
	 * in the controller. Initialize the channel descriptor array.
	 */
	{
		int          num_channels = _ltqhcd->core_if.params.host_channels;
		int i;
		for (i = 0; i < num_channels; i++)
		{
			_ltqhcd->ltqhc[i].hc_num = i;
			_ltqhcd->ltqhc[i].hc_regs = _ltqhcd->core_if.hc_regs[i];
			LTQ_DEBUGPL(DBG_HCDV, "HCD Added channel #%d\n", i);
		}
	}
	hrtimer_init(&(_ltqhcd->xfer_stopper_timer),XFER_PROT2_OPTIONS,HRTIMER_MODE_REL);
	_ltqhcd->xfer_stopper_timer.function=xfer_stopper_timer_callback;

	/* Set device flags indicating whether the HCD supports DMA. */
	if(_ltqhcd->dev->dma_mask)
		*(_ltqhcd->dev->dma_mask) = ~0;
	_ltqhcd->dev->coherent_dma_mask = ~0;

	/*
	 * Finish generic HCD initialization and start the HCD. This function
	 * allocates the DMA buffer pool, registers the USB bus, requests the
	 * IRQ line, and calls ltqusb_hcd_start method.
	 */
	retval = usb_add_hcd(syshcd, _ltqhcd->core_if.irq, 0
			                                   |IRQF_DISABLED
			                                   |IRQF_SHARED
							   );
	if (retval < 0)
		goto error2;

	/*
	 * Allocate space for storing data on status transactions. Normally no
	 * data is sent, but this space acts as a bit bucket. This must be
	 * done after usb_add_hcd since that function allocates the DMA buffer
	 * pool.
	 */
	_ltqhcd->status_buf = ltqusb_alloc_buf_h(LTQHCD_STATUS_BUF_SIZE, 64);

	if (_ltqhcd->status_buf)
	{
		LTQ_DEBUGPL(DBG_HCD, "Lantiq USB HCD Initialized, bus=%s, usbbus=%d\n", _ltqhcd->core_if.core_name, syshcd->self.busnum);
		return 0;
	}
	LTQ_ERROR("%s: status_buf allocation failed\n", __func__);

	/* Error conditions */
	usb_remove_hcd(syshcd);
error2:
	ltqhcd_freeextra(syshcd);
	usb_put_hcd(syshcd);
error1:
	return retval;
}

/*!
   \brief Removes the HCD.
  Frees memory and resources associated with the HCD and deregisters the bus.
 */
void ltqhcd_remove(ltqhcd_hcd_t *_ltqhcd)
{
	struct usb_hcd *syshcd = ltqhcd_to_syshcd(_ltqhcd);

	LTQ_DEBUGPL(DBG_HCD, "Lantiq USB HCD REMOVE\n");

	/* Turn off all interrupts */
	ltqusb_wreg (&_ltqhcd->core_if.core_global_regs->gintmsk, 0);
	ltqusb_mreg (&_ltqhcd->core_if.core_global_regs->gahbcfg, 1, 0);

	ltqusb_phy_power_off_h  (&_ltqhcd->core_if); // Test
	ltqusb_power_off_h (&_ltqhcd->core_if);

	usb_remove_hcd(syshcd);
	ltqhcd_freeextra(syshcd);
	usb_put_hcd(syshcd);
	return;
}


/* =========================================================================
 *  Linux HC Driver Functions
 * ========================================================================= */

/*!
   \brief Initializes the LTQUSB controller and its root hub and prepares it for host
 mode operation. Activates the root port. Returns 0 on success and a negative
 error code on failure.
 Called by USB stack.
 */
int ltqhcd_start(struct usb_hcd *_syshcd)
{
	ltqhcd_hcd_t *ltqhcd = syshcd_to_ltqhcd (_syshcd);
	ltqusb_core_if_t *core_if = &ltqhcd->core_if;
	struct usb_bus *bus;

	LTQ_DEBUGPL(DBG_HCD, "Lantiq USB HCD START\n");

	bus = hcd_to_bus(_syshcd);

	/* Initialize the bus state.  */
	_syshcd->state = HC_STATE_RUNNING;

	/* Initialize and connect root hub if one is not already attached */
	if (bus->root_hub)
	{
		LTQ_DEBUGPL(DBG_HCD, "Lantiq USB HCD Has Root Hub\n");
		/* Inform the HUB driver to resume. */
		usb_hcd_resume_root_hub(_syshcd);
	}

	ltqhcd->flags.d32 = 0;

	/* Put all channels in the free channel list and clean up channel states.*/
	{
		int num_channels = ltqhcd->core_if.params.host_channels;
		int i;
		for (i = 0; i < num_channels; i++)
		{
			ltqhcd_hc_t      *channel;
			channel = &ltqhcd->ltqhc[i];
			ltqhcd_hc_cleanup(&ltqhcd->core_if, channel);
		}
	}
	/* Initialize the USB core for host mode operation. */

	ltqusb_host_enable_interrupts(core_if);
	ltqusb_enable_global_interrupts_h(core_if);
	ltqusb_phy_power_on_h (core_if);

	ltqusb_vbus_init(ltqhcd);

	/* Turn on the vbus power. */
	{
		hprt0_data_t hprt0;
		hprt0.d32 = ltqusb_read_hprt0(core_if);

		LTQ_PRINT("Init: Power Port (%d)\n", hprt0.b.prtpwr);
		if (hprt0.b.prtpwr == 0 )
		{
			hprt0.b.prtpwr = 1;
			ltqusb_wreg(core_if->hprt0, hprt0.d32);
			ltqusb_vbus_on(ltqhcd);
		}
	}
	return 0;
}

/*!
   \brief Halts the LTQUSB  host mode operations in a clean manner. USB transfers are
 stopped.
 */
void		ltqusb_oc_int_free(int port);

	extern struct timer_list oc1_retry_timer;
	extern struct timer_list oc2_retry_timer;

void ltqhcd_stop(struct usb_hcd *_syshcd)
{
	ltqhcd_hcd_t *ltqhcd = syshcd_to_ltqhcd(_syshcd);
	hprt0_data_t  hprt0 = { .d32=0 };

	LTQ_DEBUGPL(DBG_HCD, "Lantiq USB HCD STOP\n");
	#ifdef __EPQD_DESTROY_TIMEOUT__
		del_timer(&ltqhcd->destroy_timer);
	#endif
	del_timer(&ltqhcd->host_probe_timer);
	del_timer(&ltqhcd->autoprobe_timer);


	del_timer(&oc1_retry_timer);
	del_timer(&oc2_retry_timer);

	/* Turn off all interrupts. */
	ltqusb_disable_global_interrupts_h(&ltqhcd->core_if );
	ltqusb_host_disable_interrupts(&ltqhcd->core_if );

	/*
	 * The root hub should be disconnected before this function is called.
	 * The disconnect will clear the URBD lists (via ..._hcd_urb_dequeue)
	 * and the EPQH lists (via ..._hcd_endpoint_disable).
	 */

	/* Turn off the vbus power */
	LTQ_PRINT("PortPower off\n");

	ltqusb_vbus_off(ltqhcd);


	ltqusb_oc_int_free(ltqhcd->core_if.core_no);


	ltqusb_vbus_free(ltqhcd);
	hprt0.b.prtpwr = 0;
	ltqusb_wreg(ltqhcd->core_if.hprt0, hprt0.d32);
	return;
}

/*!
   \brief Returns the current frame number
 */
int ltqhcd_get_frame_number(struct usb_hcd *_syshcd)
{
	ltqhcd_hcd_t 	*ltqhcd = syshcd_to_ltqhcd(_syshcd);
	hfnum_data_t hfnum;

	hfnum.d32 = ltqusb_rreg(&ltqhcd->core_if.host_global_regs->hfnum);

	return hfnum.b.frnum;
}

/*!
   \brief Starts processing a USB transfer request specified by a USB Request Block
  (URB). mem_flags indicates the type of memory allocation to use while
  processing this URB.
 */
int ltqhcd_urb_enqueue( struct usb_hcd           *_syshcd,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
                        struct usb_host_endpoint *_sysep,
#endif
                        struct urb               *_urb,
                        gfp_t                     _mem_flags)
{
	ltqhcd_hcd_t *ltqhcd = syshcd_to_ltqhcd (_syshcd);
	ltqhcd_epqh_t *epqh = NULL;

	LOCK_ENQUEUE(ltqhcd);

	#ifdef __DEBUG__
		if (CHK_DEBUG_LEVEL(DBG_HCDV)
		     || (usb_pipetype(_urb->pipe) == PIPE_CONTROL     && CHK_DEBUG_LEVEL(DBG_HCD_URB_CTRL))
		     || (usb_pipetype(_urb->pipe) == PIPE_ISOCHRONOUS &&  usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_ISOC_IN))
		     || (usb_pipetype(_urb->pipe) == PIPE_ISOCHRONOUS && !usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_ISOC_OUT))
		     || (usb_pipetype(_urb->pipe) == PIPE_BULK        &&  usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_BULK_IN))
		     || (usb_pipetype(_urb->pipe) == PIPE_BULK        && !usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_BULK_OUT))
		     || (usb_pipetype(_urb->pipe) == PIPE_INTERRUPT   &&  usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_INTR_IN))
		     || (usb_pipetype(_urb->pipe) == PIPE_INTERRUPT   && !usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_INTR_OUT))
		    )
			dump_urb_info(_urb, "ltqusb_hcd_urb_enqueue");
	#endif //__DEBUG__
	LTQ_DEBUGPL(DBG_HCD, "LTQUSB HCD URB Enqueue\n");

	if (!ltqhcd->flags.b.port_connect_status)  /* No longer connected. */
	{
		UNLOCK_ENQUEUE(ltqhcd);
		return -ENODEV;
	}

	#if !defined(__EN_ISOC__)
		if(usb_pipetype(_urb->pipe) == PIPE_ISOCHRONOUS)
		{
			UNLOCK_ENQUEUE(ltqhcd);
			LTQ_ERROR("ISOC transfer not supported!!!\n");
#ifdef NEWRET
			return -ENXIO;
			//return -EINVAL;	//a) Invalid transfer type specified (or not supported)
#else
			return -ENODEV;
#endif
		}
	#endif

	if(_urb->hcpriv)
	{
		UNLOCK_ENQUEUE(ltqhcd);
		LTQ_WARN("%s() Previous urb->hcpriv exist %p\n",__func__,_urb->hcpriv);
	#if 1
#ifdef NEWRET
		return -EBUSY;//		The URB is already active.
#else
		return -ENOSPC;
#endif
	#endif
	}

	epqh=ltqhcd_urbd_create (ltqhcd,_urb);
	UNLOCK_ENQUEUE(ltqhcd);
	if (!epqh)
	{
		LTQ_ERROR("LTQUSB HCD URB Enqueue failed creating URBD\n");
#ifdef NEWRET
		return -ENOMEM;//		no memory for allocation of internal structures
#else
		return -ENOSPC;
#endif
	}
	if(epqh->phase==EPQH_DISABLING )
	{
		LTQ_ERROR("Enqueue to a DISABLING EP!!!\n");
#ifdef NEWRET
		return -ENOENT	;//	specified interface or endpoint does not exist or is not enabled
#else
		return -ENODEV;
#endif
	}

	_urb->status= -EINPROGRESS;
	#ifdef __DYN_SOF_INTR__
		ltqhcd->dyn_sof_count = DYN_SOF_COUNT_DEF;
	#endif
	//enable_sof(ltqhcd);
	{
		gint_data_t gintsts;
		gintsts.d32=0;
		gintsts.b.sofintr = 1;
		ltqusb_mreg(&ltqhcd->core_if.core_global_regs->gintmsk, 0,gintsts.d32);
	}

	if(epqh->phase==EPQH_IDLE || epqh->phase==EPQH_STDBY )
	{
		epqh->phase=EPQH_READY;
		#ifdef __EPQD_DESTROY_TIMEOUT__
			del_timer(&epqh->destroy_timer);
		#endif
	}
	select_eps(ltqhcd);
	return 0;
}

/*!
   \brief Aborts/cancels a USB transfer request. Always returns 0 to indicate
  success.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
int ltqhcd_urb_dequeue(struct usb_hcd *_syshcd, struct urb *_urb)
#else
int ltqhcd_urb_dequeue(struct usb_hcd *_syshcd, struct urb *_urb, int status)
#endif
{
	ltqhcd_hcd_t  *ltqhcd;
	struct usb_host_endpoint *sysep;
	ltqhcd_urbd_t *urbd;
	ltqhcd_epqh_t *epqh;

	#ifdef __DEBUG__
		if (CHK_DEBUG_LEVEL(DBG_HCDV)
		     || (usb_pipetype(_urb->pipe) == PIPE_CONTROL     && CHK_DEBUG_LEVEL(DBG_HCD_URB_CTRL))
		     || (usb_pipetype(_urb->pipe) == PIPE_ISOCHRONOUS &&  usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_ISOC_IN))
		     || (usb_pipetype(_urb->pipe) == PIPE_ISOCHRONOUS && !usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_ISOC_OUT))
		     || (usb_pipetype(_urb->pipe) == PIPE_BULK        &&  usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_BULK_IN))
		     || (usb_pipetype(_urb->pipe) == PIPE_BULK        && !usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_BULK_OUT))
		     || (usb_pipetype(_urb->pipe) == PIPE_INTERRUPT   &&  usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_INTR_IN))
		     || (usb_pipetype(_urb->pipe) == PIPE_INTERRUPT   && !usb_pipein(_urb->pipe) && CHK_DEBUG_LEVEL(DBG_HCD_URB_INTR_OUT))
		    )
			dump_urb_info(_urb, "ltqusb_hcd_urb_dequeue");
	#endif //__DEBUG__
	LTQ_DEBUGPL(DBG_HCD, "LTQUSB HCD URB Dequeue\n");
	#if !defined(__EN_ISOC__)
		if(usb_pipetype(_urb->pipe) == PIPE_ISOCHRONOUS)
			return 0;
	#endif

	ltqhcd = syshcd_to_ltqhcd(_syshcd);

	urbd = (ltqhcd_urbd_t *) _urb->hcpriv;
	if(!urbd)
	{
		#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
#ifdef NEWRET
//			_urb->status=-ESHUTDOWN;//From xhci
			_urb->status=-ECONNRESET;//From Linux URB doc
#else
			_urb->status=-ETIMEDOUT;
#endif
			usb_hcd_giveback_urb(_syshcd, _urb);
		#else
			usb_hcd_giveback_urb(_syshcd, _urb,status);
		#endif
		return 0;
	}

	urbd->cancelling=1;
	sysep = ltqhcd_urb_to_endpoint(_urb);
	if(sysep)
	{
		LOCK_EPQH_LIST_ALL(ltqhcd);
		epqh = sysep_to_epqh(ltqhcd,sysep,0);
		UNLOCK_EPQH_LIST_ALL(ltqhcd);
		if(epqh!=urbd->epqh)
		{
			LTQ_ERROR("%s inconsistant epqh %p %p\n",__func__,epqh,urbd->epqh);
			LTQ_ERROR("       sysep     : %p    %p\n",epqh->sysep, urbd->epqh->sysep);
			LTQ_ERROR("       devno     : %8d    %8d\n",epqh->devno, urbd->epqh->devno);
			LTQ_ERROR("       phase     : %8d    %8d\n",epqh->phase, urbd->epqh->phase);
			LTQ_ERROR("       ep_type   : %8d    %8d\n",epqh->ep_type, urbd->epqh->ep_type);
			LTQ_ERROR("       mps       : %8d    %8d\n",epqh->mps, urbd->epqh->mps);
			LTQ_ERROR("       urbd_count: %8d    %8d\n",epqh->urbd_count, urbd->epqh->urbd_count);
		}
	}
	else
		epqh = (ltqhcd_epqh_t *) urbd->epqh;
	if(!ltqhcd->flags.b.port_connect_status || !epqh)
	{
		urbd->phase=URBD_DEQUEUEING;
		ltqhcd_complete_urb(ltqhcd, urbd, -ENODEV);
	}
	else if(  urbd->phase==URBD_IDLE
		   || urbd->phase==URBD_ACTIVE
		   )
	{
		urbd->phase=URBD_DEQUEUEING;
		#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
#ifdef NEWRET
//				ltqhcd_complete_urb(ltqhcd, urbd, -ESHUTDOWN);//From xhci
				ltqhcd_complete_urb(ltqhcd, urbd, -ECONNRESET);//From Linux URB doc
#else
				ltqhcd_complete_urb(ltqhcd, urbd, -ETIMEDOUT);
#endif
		#else
			ltqhcd_complete_urb(ltqhcd, urbd, status);
		#endif
		ltqhcd_epqh_idle(epqh);
	}
	else if(   urbd->phase==URBD_RUNNING )
	{
		if(ltqhcd_hc_halt(&ltqhcd->core_if, urbd->hc, HC_XFER_URB_DEQUEUE))
		{
			urbd->phase=URBD_DEQUEUEING;
			#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
#ifdef NEWRET
//				ltqhcd_complete_urb(ltqhcd, urbd, -ESHUTDOWN);//From xhci
				ltqhcd_complete_urb(ltqhcd, urbd, -ECONNRESET);//From Linux URB doc
#else
				ltqhcd_complete_urb(ltqhcd, urbd, -ETIMEDOUT);
#endif
			#else
				ltqhcd_complete_urb(ltqhcd, urbd, status);
			#endif
			ltqhcd_epqh_idle(epqh);
		}
		else
		{
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
				urbd->cancelling_status=status;
			#endif
		}
	}
	return 0;
}

/*!
   \brief Frees resources in the LTQUSB controller related to a given endpoint. Also
  clears state in the HCD related to the endpoint. Any URBs for the endpoint
  must already be dequeued.
 */
void ltqhcd_endpoint_disable( struct usb_hcd *_syshcd,
                              struct usb_host_endpoint *_sysep)
{
	ltqhcd_hcd_t  *ltqhcd;
	ltqhcd_epqh_t *epqh;

	LTQ_DEBUGPL(DBG_HCD, "LTQUSB HCD EP DISABLE: _bEndpointAddress=0x%02x, "
	    "endpoint=%d\n", _sysep->desc.bEndpointAddress,
		    ltqhcd_ep_addr_to_endpoint(_sysep->desc.bEndpointAddress));

	ltqhcd = syshcd_to_ltqhcd(_syshcd);

	LOCK_EPQH_LIST_ALL(ltqhcd);
	epqh = sysep_to_epqh(ltqhcd,_sysep,0);
	UNLOCK_EPQH_LIST_ALL(ltqhcd);

	if (!epqh)
	{
		return;
	}
	else
	{
		if (epqh->sysep!=_sysep)
		{
			LTQ_ERROR("%s inconsistant sysep %p %p %p\n",__func__,epqh,epqh->sysep,_sysep);
			return;
		}

		epqh->phase=EPQH_DISABLING;
#ifdef NEWRET
//		kill_all_urbs_in_epqh(ltqhcd, epqh, -ESHUTDOWN);//From xhci
		kill_all_urbs_in_epqh(ltqhcd, epqh, -ECONNRESET);//From Linux URB doc
#else
		kill_all_urbs_in_epqh(ltqhcd, epqh, -ETIMEDOUT);
#endif
		{
			uint32_t count=0x80000;
			for(;!list_empty(&epqh->urbd_list) && count> 0; count--) udelay(1);
			if(!count)
				LTQ_ERROR("%s: unable to clear urbd in epqh \n",__func__);
		}
		if(list_empty(&epqh->urbd_list) && list_empty(&epqh->release_list))
			ltqhcd_epqh_free(epqh);
	}
	LTQ_DEBUGPL(DBG_HCD, "LTQUSB HCD EP DISABLE: done\n");
}


/*!
  \brief Handles host mode interrupts for the LTQUSB controller. Returns IRQ_NONE if
 there was no interrupt to handle. Returns IRQ_HANDLED if there was a valid
 interrupt.

 This function is called by the USB core when an interrupt occurs
 */
irqreturn_t ltqhcd_irq(struct usb_hcd *_syshcd)
{
	ltqhcd_hcd_t *ltqhcd = syshcd_to_ltqhcd (_syshcd);
	int32_t retval=0;

	//mask_and_ack_ltq_irq (ltqhcd->core_if.irq);
	retval = ltqhcd_handle_intr(ltqhcd);
	return  IRQ_HANDLED ;
}



/*!
 \brief Creates Status Change bitmap for the root hub and root port. The bitmap is
  returned in buf. Bit 0 is the status change indicator for the root hub. Bit 1
  is the status change indicator for the single root port. Returns 1 if either
  change indicator is 1, otherwise returns 0.
 */
int ltqhcd_hub_status_data(struct usb_hcd *_syshcd, char *_buf)
{
	ltqhcd_hcd_t *ltqhcd = syshcd_to_ltqhcd (_syshcd);

	_buf[0] = 0;
	_buf[0] |= (ltqhcd->flags.b.port_connect_status_change ||
	            ltqhcd->flags.b.port_reset_change ||
	            ltqhcd->flags.b.port_enable_change ||
	            ltqhcd->flags.b.port_suspend_change ||
	            ltqhcd->flags.b.port_over_current_change) << 1;

	#ifdef __DEBUG__
		if (_buf[0])
		{
			LTQ_DEBUGPL(DBG_HCD, "LTQUSB HCD HUB STATUS DATA:"
				    " Root port status changed\n");
			LTQ_DEBUGPL(DBG_HCDV, "  port_connect_status_change: %d\n",
				    ltqhcd->flags.b.port_connect_status_change);
			LTQ_DEBUGPL(DBG_HCDV, "  port_reset_change: %d\n",
				    ltqhcd->flags.b.port_reset_change);
			LTQ_DEBUGPL(DBG_HCDV, "  port_enable_change: %d\n",
				    ltqhcd->flags.b.port_enable_change);
			LTQ_DEBUGPL(DBG_HCDV, "  port_suspend_change: %d\n",
				    ltqhcd->flags.b.port_suspend_change);
			LTQ_DEBUGPL(DBG_HCDV, "  port_over_current_change: %d\n",
				    ltqhcd->flags.b.port_over_current_change);
			{
				hprt0_data_t hprt0;
				hprt0.d32 = ltqusb_rreg(ltqhcd->core_if.hprt0);
				LTQ_DEBUGPL(DBG_HCDV, "  port reg :%08X\n",hprt0.d32);
				LTQ_DEBUGPL(DBG_HCDV, "  port reg :connect: %d/%d\n",hprt0.b.prtconnsts,hprt0.b.prtconndet);
				LTQ_DEBUGPL(DBG_HCDV, "  port reg :enable: %d/%d\n",hprt0.b.prtena,hprt0.b.prtenchng);
				LTQ_DEBUGPL(DBG_HCDV, "  port reg :OC: %d/%d\n",hprt0.b.prtovrcurract,hprt0.b.prtovrcurrchng);
				LTQ_DEBUGPL(DBG_HCDV, "  port reg :rsume/suspend(flag)/reset: %d/%d(%d)/%d\n",hprt0.b.prtres,hprt0.b.prtsusp,ltqhcd->core_if.issuspended,hprt0.b.prtrst);
				LTQ_DEBUGPL(DBG_HCDV, "  port reg :port power: %d/\n",hprt0.b.prtpwr);
				LTQ_DEBUGPL(DBG_HCDV, "  port reg :speed: %d/\n",hprt0.b.prtspd);
			}
		}
	#endif //__DEBUG__
	return (_buf[0] != 0);
}

#ifdef __WITH_HS_ELECT_TST__
	extern void do_setup(ltqusb_core_if_t *_core_if) ;
	extern void do_in_ack(ltqusb_core_if_t *_core_if);
#endif //__WITH_HS_ELECT_TST__

/*!
 \brief Handles hub class-specific requests.
 */
int ltqhcd_hub_control( struct usb_hcd *_syshcd,
                        u16             _typeReq,
                        u16             _wValue,
                        u16             _wIndex,
                        char           *_buf,
                        u16             _wLength)
{
	int retval = 0;

	ltqhcd_hcd_t              *ltqhcd  = syshcd_to_ltqhcd (_syshcd);
	ltqusb_core_if_t          *core_if = &ltqhcd->core_if;
	struct usb_hub_descriptor *desc;
	hprt0_data_t               hprt0 = {.d32 = 0};

	uint32_t port_status;

	switch (_typeReq)
	{
		case ClearHubFeature:
			LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
			         "ClearHubFeature 0x%x\n", _wValue);
			switch (_wValue)
			{
				case C_HUB_LOCAL_POWER:
				case C_HUB_OVER_CURRENT:
					/* Nothing required here */
					break;
				default:
					retval = -EINVAL;
					LTQ_ERROR ("LTQUSB HCD - "
						   "ClearHubFeature request %xh unknown\n", _wValue);
			}
			break;
		case ClearPortFeature:
			if (!_wIndex || _wIndex > 1)
				goto error;

			switch (_wValue)
			{
				case USB_PORT_FEAT_ENABLE:
					LTQ_DEBUGPL (DBG_ANY, "LTQUSB HCD HUB CONTROL - "
						     "ClearPortFeature USB_PORT_FEAT_ENABLE\n");
					hprt0.d32 = ltqusb_read_hprt0 (core_if);
					hprt0.b.prtena = 1;
					ltqusb_wreg(core_if->hprt0, hprt0.d32);
					break;
				case USB_PORT_FEAT_SUSPEND:
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "ClearPortFeature USB_PORT_FEAT_SUSPEND\n");
					hprt0.d32 = ltqusb_read_hprt0 (core_if);
					hprt0.b.prtres = 1;
					ltqusb_wreg(core_if->hprt0, hprt0.d32);
					/* Clear Resume bit */
					mdelay (100);
					hprt0.b.prtres = 0;
					ltqusb_wreg(core_if->hprt0, hprt0.d32);
					core_if->issuspended=0;
					break;
				case USB_PORT_FEAT_POWER:
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "ClearPortFeature USB_PORT_FEAT_POWER\n");
					ltqusb_vbus_off(ltqhcd);
					hprt0.d32 = ltqusb_read_hprt0 (core_if);
					hprt0.b.prtpwr = 0;
					ltqusb_wreg(core_if->hprt0, hprt0.d32);
					break;
				case USB_PORT_FEAT_INDICATOR:
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "ClearPortFeature USB_PORT_FEAT_INDICATOR\n");
					/* Port inidicator not supported */
					break;
				case USB_PORT_FEAT_C_CONNECTION:
					/* Clears drivers internal connect status change
					 * flag */
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "ClearPortFeature USB_PORT_FEAT_C_CONNECTION\n");
					ltqhcd->flags.b.port_connect_status_change = 0;
					break;
				case USB_PORT_FEAT_C_RESET:
					/* Clears the driver's internal Port Reset Change
					 * flag */
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "ClearPortFeature USB_PORT_FEAT_C_RESET\n");
					ltqhcd->flags.b.port_reset_change = 0;
					break;
				case USB_PORT_FEAT_C_ENABLE:
					/* Clears the driver's internal Port
					 * Enable/Disable Change flag */
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "ClearPortFeature USB_PORT_FEAT_C_ENABLE\n");
					ltqhcd->flags.b.port_enable_change = 0;
					break;
				case USB_PORT_FEAT_C_SUSPEND:
					/* Clears the driver's internal Port Suspend
					 * Change flag, which is set when resume signaling on
					 * the host port is complete */
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "ClearPortFeature USB_PORT_FEAT_C_SUSPEND\n");
					ltqhcd->flags.b.port_suspend_change = 0;
					break;
				case USB_PORT_FEAT_C_OVER_CURRENT:
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "ClearPortFeature USB_PORT_FEAT_C_OVER_CURRENT\n");
					ltqhcd->flags.b.port_over_current_change = 0;
					break;
				default:
					retval = -EINVAL;
					LTQ_ERROR ("LTQUSB HCD - "
					         "ClearPortFeature request %xh "
					         "unknown or unsupported\n", _wValue);
			}
			break;
		case GetHubDescriptor:
			LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
			         "GetHubDescriptor\n");
			desc = (struct usb_hub_descriptor *)_buf;
			desc->bDescLength = 9;
			desc->bDescriptorType = 0x29;
			desc->bNbrPorts = 1;
			desc->wHubCharacteristics = 0x08;
			desc->bPwrOn2PwrGood = 1;
			desc->bHubContrCurrent = 0;
			desc->u.hs.DeviceRemovable[0] = 0;
			desc->u.hs.DeviceRemovable[1] = 0xff;
			break;
		case GetHubStatus:
			LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
			         "GetHubStatus\n");
			memset (_buf, 0, 4);
			break;
		case GetPortStatus:
			LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
			         "GetPortStatus\n");
			if (!_wIndex || _wIndex > 1)
				goto error;
			port_status = 0;
			if (ltqhcd->flags.b.port_connect_status_change)
				port_status |= (1 << USB_PORT_FEAT_C_CONNECTION);
			if (ltqhcd->flags.b.port_enable_change)
				port_status |= (1 << USB_PORT_FEAT_C_ENABLE);
			if (ltqhcd->flags.b.port_suspend_change)
				port_status |= (1 << USB_PORT_FEAT_C_SUSPEND);
			if (ltqhcd->flags.b.port_reset_change)
				port_status |= (1 << USB_PORT_FEAT_C_RESET);
			if (ltqhcd->flags.b.port_over_current_change)
			{
				LTQ_ERROR("Device Not Supported\n");
				port_status |= (1 << USB_PORT_FEAT_C_OVER_CURRENT);
			}
			if (!ltqhcd->flags.b.port_connect_status)
			{
				/*
				 * The port is disconnected, which means the core is
				 * either in device mode or it soon will be. Just
				 * return 0's for the remainder of the port status
				 * since the port register can't be read if the core
				 * is in device mode.
				 */
				*((u32 *) _buf) = cpu_to_le32(port_status);
				break;
			}

			hprt0.d32 = ltqusb_rreg(core_if->hprt0);
			LTQ_DEBUGPL(DBG_HCDV, "  HPRT0: 0x%08x\n", hprt0.d32);
			if (hprt0.b.prtconnsts)
				port_status |= (1 << USB_PORT_FEAT_CONNECTION);
			if (hprt0.b.prtena)
			{
				ltqhcd->disconnecting=0;
				port_status |= (1 << USB_PORT_FEAT_ENABLE);
			}
//			if (hprt0.b.prtsusp)
			if (core_if->issuspended)
				port_status |= (1 << USB_PORT_FEAT_SUSPEND);
			if (hprt0.b.prtovrcurract)
				port_status |= (1 << USB_PORT_FEAT_OVER_CURRENT);
			if (hprt0.b.prtrst)
				port_status |= (1 << USB_PORT_FEAT_RESET);
			if (hprt0.b.prtpwr)
				port_status |= (1 << USB_PORT_FEAT_POWER);
#if LINUX_VERSION_CODE > KERNEL_VERSION(2, 6, 32)
			if      (hprt0.b.prtspd == LTQUSB_HPRT0_PRTSPD_HIGH_SPEED)
				port_status |= USB_PORT_STAT_HIGH_SPEED;
			else if (hprt0.b.prtspd == LTQUSB_HPRT0_PRTSPD_LOW_SPEED)
				port_status |= USB_PORT_STAT_LOW_SPEED;
#else
			if      (hprt0.b.prtspd == LTQUSB_HPRT0_PRTSPD_HIGH_SPEED)
				port_status |= (1 << USB_PORT_FEAT_HIGHSPEED);
			else if (hprt0.b.prtspd == LTQUSB_HPRT0_PRTSPD_LOW_SPEED)
				port_status |= (1 << USB_PORT_FEAT_LOWSPEED);
#endif
			if (hprt0.b.prttstctl)
				port_status |= (1 << USB_PORT_FEAT_TEST);
			/* USB_PORT_FEAT_INDICATOR unsupported always 0 */
			*((u32 *) _buf) = cpu_to_le32(port_status);
			break;
		case SetHubFeature:
			LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
			         "SetHubFeature\n");
			/* No HUB features supported */
			break;
		case SetPortFeature:
			if (_wValue != USB_PORT_FEAT_TEST && (!_wIndex || _wIndex > 1))
				goto error;
			/*
			 * The port is disconnected, which means the core is
			 * either in device mode or it soon will be. Just
			 * return without doing anything since the port
			 * register can't be written if the core is in device
			 * mode.
			 */
			 if(_wValue==USB_PORT_FEAT_POWER && !core_if->issuspended)
			 {
				LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
				     "SetPortFeature - USB_PORT_FEAT_POWER\n");
				ltqusb_vbus_on (ltqhcd);
				hprt0.d32 = ltqusb_read_hprt0 (core_if);
				hprt0.b.prtpwr = 1;
				ltqusb_wreg(core_if->hprt0, hprt0.d32);
				break;
			}
			if (!ltqhcd->flags.b.port_connect_status)
				break;
			switch (_wValue)
			{
				case USB_PORT_FEAT_SUSPEND:
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "SetPortFeature - USB_PORT_FEAT_SUSPEND\n");
					hprt0.d32 = ltqusb_read_hprt0 (core_if);
					hprt0.b.prtsusp = 1;
					ltqusb_wreg(core_if->hprt0, hprt0.d32);
					//LTQ_PRINT( "SUSPEND: HPRT0=%0x\n", hprt0.d32);
					/* Suspend the Phy Clock */
					{
						pcgcctl_data_t pcgcctl = {.d32=0};
						pcgcctl.b.stoppclk = 1;
						ltqusb_wreg(core_if->pcgcctl, pcgcctl.d32);
					}
					core_if->issuspended=1;
					break;
#if 0
				case USB_PORT_FEAT_POWER:
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
					     "SetPortFeature - USB_PORT_FEAT_POWER\n");
					ltqusb_vbus_on (ltqhcd);
					hprt0.d32 = ltqusb_read_hprt0 (core_if);
					hprt0.b.prtpwr = 1;
					ltqusb_wreg(core_if->hprt0, hprt0.d32);
					break;
#endif
				case USB_PORT_FEAT_RESET:
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "SetPortFeature - USB_PORT_FEAT_RESET\n");
					hprt0.d32 = ltqusb_read_hprt0 (core_if);
					hprt0.b.prtrst = 1;
					ltqusb_wreg(core_if->hprt0, hprt0.d32);
					/* Clear reset bit in 10ms (FS/LS) or 50ms (HS) */
					MDELAY (60);
					hprt0.b.prtrst = 0;
					ltqusb_wreg(core_if->hprt0, hprt0.d32);
					break;
			#ifdef __WITH_HS_ELECT_TST__
				case USB_PORT_FEAT_TEST:
					{
						uint32_t t;
						gint_data_t gintmsk;
						t = (_wIndex >> 8); /* MSB wIndex USB */
						LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
							     "SetPortFeature - USB_PORT_FEAT_TEST %d\n", t);
						warn("USB_PORT_FEAT_TEST\n");
						if (t < 6)
						{
							hprt0.d32 = ltqusb_read_hprt0 (core_if);
							hprt0.b.prttstctl = t;
							ltqusb_wreg(core_if->hprt0, hprt0.d32);
						}
						else if (t == 6)  /* HS_HOST_PORT_SUSPEND_RESUME */
						{
							/* Save current interrupt mask */
							gintmsk.d32 = ltqusb_rreg(&core_if->core_global_regs->gintmsk);

							/* Disable all interrupts while we muck with
							 * the hardware directly
							 */
							ltqusb_wreg(&core_if->core_global_regs->gintmsk, 0);

							/* 15 second delay per the test spec */
							mdelay(15000);

							/* Drive suspend on the root port */
							hprt0.d32 = ltqusb_read_hprt0 (core_if);
							hprt0.b.prtsusp = 1;
							hprt0.b.prtres = 0;
							ltqusb_wreg(core_if->hprt0, hprt0.d32);

							/* 15 second delay per the test spec */
							mdelay(15000);

							/* Drive resume on the root port */
							hprt0.d32 = ltqusb_read_hprt0 (core_if);
							hprt0.b.prtsusp = 0;
							hprt0.b.prtres = 1;
							ltqusb_wreg(core_if->hprt0, hprt0.d32);
							mdelay(100);

							/* Clear the resume bit */
							hprt0.b.prtres = 0;
							ltqusb_wreg(core_if->hprt0, hprt0.d32);

							/* Restore interrupts */
							ltqusb_wreg(&core_if->core_global_regs->gintmsk, gintmsk.d32);
						}
						else if (t == 7)  /* SINGLE_STEP_GET_DEVICE_DESCRIPTOR setup */
						{
							/* Save current interrupt mask */
							gintmsk.d32 = ltqusb_rreg(&core_if->core_global_regs->gintmsk);

							/* Disable all interrupts while we muck with
							 * the hardware directly
							 */
							ltqusb_wreg(&core_if->core_global_regs->gintmsk, 0);

							/* 15 second delay per the test spec */
							mdelay(15000);

							/* Send the Setup packet */
							do_setup(core_if);

							/* 15 second delay so nothing else happens for awhile */
							mdelay(15000);

							/* Restore interrupts */
							ltqusb_wreg(&core_if->core_global_regs->gintmsk, gintmsk.d32);
						}

						else if (t == 8)  /* SINGLE_STEP_GET_DEVICE_DESCRIPTOR execute */
						{
							/* Save current interrupt mask */
							gintmsk.d32 = ltqusb_rreg(&core_if->core_global_regs->gintmsk);

							/* Disable all interrupts while we muck with
							 * the hardware directly
							 */
							ltqusb_wreg(&core_if->core_global_regs->gintmsk, 0);

							/* Send the Setup packet */
							do_setup(core_if);

							/* 15 second delay so nothing else happens for awhile */
							mdelay(15000);

							/* Send the In and Ack packets */
							do_in_ack(core_if);

							/* 15 second delay so nothing else happens for awhile */
							mdelay(15000);

							/* Restore interrupts */
							ltqusb_wreg(&core_if->core_global_regs->gintmsk, gintmsk.d32);
						}
					}
					break;
			#endif //__WITH_HS_ELECT_TST__
				case USB_PORT_FEAT_INDICATOR:
					LTQ_DEBUGPL (DBG_HCD, "LTQUSB HCD HUB CONTROL - "
						     "SetPortFeature - USB_PORT_FEAT_INDICATOR\n");
					/* Not supported */
					break;
				default:
					retval = -EINVAL;
					LTQ_ERROR ("LTQUSB HCD - "
						   "SetPortFeature request %xh "
						   "unknown or unsupported\n", _wValue);
			}
			break;
		default:
		error:
			retval = -EINVAL;
			LTQ_WARN ("LTQUSB HCD - "
			          "Unknown hub control request type or invalid typeReq: %xh wIndex: %xh wValue: %xh\n",
			          _typeReq, _wIndex, _wValue);
	}
	return retval;
}




/*!
   \brief This function trigger a data transfer for a host channel and
  starts the transfer.

  For a PING transfer in Slave mode, the Do Ping bit is set in the HCTSIZ
  register along with a packet count of 1 and the channel is enabled. This
  causes a single PING transaction to occur. Other fields in HCTSIZ are
  simply set to 0 since no data transfer occurs in this case.

  For a PING transfer in DMA mode, the HCTSIZ register is initialized with
  all the information required to perform the subsequent data transfer. In
  addition, the Do Ping bit is set in the HCTSIZ register. In this case, the
  controller performs the entire PING protocol, then starts the data
  transfer.
  \param _core_if        Pointer of core_if structure
  \param _ltqhc Information needed to initialize the host channel. The xfer_len
  value may be reduced to accommodate the max widths of the XferSize and
  PktCnt fields in the HCTSIZn register. The multi_count value may be changed
  to reflect the final xfer_len value.
 */
void ltqhcd_hc_start(ltqhcd_hcd_t *_ltqhcd, ltqhcd_hc_t *_ltqhc, uint8_t do_now)
{
	ltqusb_core_if_t *core_if = &_ltqhcd->core_if;
	uint32_t max_hc_xfer_size = core_if->params.max_transfer_size;
	uint16_t max_hc_pkt_count = core_if->params.max_packet_count;
	ltqusb_hc_regs_t *hc_regs = core_if->hc_regs[_ltqhc->hc_num];
	hfnum_data_t hfnum;
	int new_phase=HC_WAITING;

	_ltqhc->phase=HC_WAITING2;
	if(_ltqhc->cancelling || _ltqhc->urbd->cancelling || _ltqhc->urbd->phase==URBD_DEQUEUEING)
	{
		_ltqhc->phase=new_phase;
		return;
	}

	_ltqhc->urbd->phase=URBD_RUNNING;

	if(max_hc_pkt_count * _ltqhc->mps <  max_hc_xfer_size)
		max_hc_xfer_size = max_hc_pkt_count * _ltqhc->mps;

	if(_ltqhc->is_in || _ltqhc->speed != LTQUSB_EP_SPEED_HIGH || _ltqhc->xfer_len==0)
		_ltqhc->epqh->do_ping=0;
	if(_ltqhc->ep_type == LTQUSB_EP_TYPE_INTR || _ltqhc->ep_type == LTQUSB_EP_TYPE_ISOC)
		_ltqhc->epqh->do_ping=0;
	if(_ltqhc->ep_type == LTQUSB_EP_TYPE_CTRL && _ltqhc->control_phase != LTQHCD_CONTROL_DATA  )
		_ltqhc->epqh->do_ping=0;

#ifdef __NO_BULK_PING__
	if(_ltqhc->ep_type == LTQUSB_EP_TYPE_BULK  )
		_ltqhc->epqh->do_ping=0;
#endif

	if (_ltqhc->split > 0)
	{
		_ltqhc->start_pkt_count = 1;
		if(!_ltqhc->is_in && _ltqhc->split>1) // OUT CSPLIT
			_ltqhc->xfer_len = 0;
		if (_ltqhc->xfer_len > _ltqhc->mps)
			_ltqhc->xfer_len = _ltqhc->mps;
		if (_ltqhc->xfer_len > 188)
			_ltqhc->xfer_len = 188;
	}
	else if(_ltqhc->is_in)
	{
		_ltqhc->short_rw = 0;
		if(_ltqhcd->singlexact)
			_ltqhc->start_pkt_count = 1;
		else if (_ltqhc->xfer_len > 0)
		{
			if (_ltqhc->xfer_len > max_hc_xfer_size)
				_ltqhc->xfer_len = max_hc_xfer_size - _ltqhc->mps + 1;
			_ltqhc->start_pkt_count = (_ltqhc->xfer_len + _ltqhc->mps - 1) / _ltqhc->mps;
			if (_ltqhc->start_pkt_count > max_hc_pkt_count)
				_ltqhc->start_pkt_count = max_hc_pkt_count;
		}
		else /* Need 1 packet for transfer length of 0. */
			_ltqhc->start_pkt_count = 1;
		_ltqhc->xfer_len = _ltqhc->start_pkt_count * _ltqhc->mps;
	}
	else //non-split out
	{
		if (_ltqhc->xfer_len == 0)
		{
			if(_ltqhc->short_rw==0)
				printk(KERN_INFO "Info: %s() line %d: ZLP write without short_rw set! xfer_count:%d/%d \n",__func__,__LINE__,
					_ltqhc->xfer_count,
					_ltqhc->urbd->xfer_len);
			_ltqhc->start_pkt_count = 1;
		}
		else
		{
			if(_ltqhcd->singlexact)
			{
				_ltqhc->start_pkt_count = 1;
				if (_ltqhc->xfer_len > _ltqhc->mps)
					_ltqhc->xfer_len = _ltqhc->mps;
			}
			else if (_ltqhc->xfer_len > max_hc_xfer_size)
			{
				_ltqhc->start_pkt_count = (max_hc_xfer_size / _ltqhc->mps);
				_ltqhc->xfer_len = _ltqhc->start_pkt_count * _ltqhc->mps;
			}
			else
			{
				_ltqhc->start_pkt_count = (_ltqhc->xfer_len+_ltqhc->mps-1)  / _ltqhc->mps;
//				if(_ltqhc->start_pkt_count * _ltqhc->mps == _ltqhc->xfer_len )
//					_ltqhc->start_pkt_count += _ltqhc->short_rw;
			}
		}
	}

	#ifdef __EN_ISOC__
		if (_ltqhc->ep_type == LTQUSB_EP_TYPE_ISOC)
		{
			/* Set up the initial PID for the transfer. */
			#if 1
				_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA0;
			#else
				if (_ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
				{
					if (_ltqhc->is_in)
					{
						if      (_ltqhc->multi_count == 1)
							_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA0;
						else if (_ltqhc->multi_count == 2)
							_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA1;
						else
							_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA2;
					}
					else
					{
						if (_ltqhc->multi_count == 1)
							_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA0;
						else
							_ltqhc->data_pid_start = LTQUSB_HC_PID_MDATA;
					}
				}
				else
					_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA0;
			#endif
		}
	#endif

	LTQ_DEBUGPL(DBG_HCDV, "%s: Channel %d\n", __func__, _ltqhc->hc_num);
	{
		hctsiz_data_t hctsiz= { .d32=0 };

		hctsiz.b.dopng = _ltqhc->epqh->do_ping;
		_ltqhc->epqh->do_ping=0;

		hctsiz.b.xfersize = _ltqhc->xfer_len;
		hctsiz.b.pktcnt   = _ltqhc->start_pkt_count;
		hctsiz.b.pid      = _ltqhc->data_pid_start;
		ltqusb_wreg(&hc_regs->hctsiz, hctsiz.d32);

		LTQ_DEBUGPL(DBG_HCDV, "  Xfer Size: %d\n", hctsiz.b.xfersize);
		LTQ_DEBUGPL(DBG_HCDV, "  Num Pkts: %d\n" , hctsiz.b.pktcnt);
		LTQ_DEBUGPL(DBG_HCDV, "  Start PID: %d\n", hctsiz.b.pid);

		LTQ_DEBUGPL(DBG_HCDV, "  Start PID: %d\n", hctsiz.b.pid);
	}
	LTQ_DEBUGPL(DBG_HCDV, "  DMA: 0x%08x\n", (uint32_t)(CPHYSADDR( ((uint32_t)(_ltqhc->xfer_buff))+ _ltqhc->xfer_count )));
	ltqusb_wreg(&hc_regs->hcdma, (uint32_t)(CPHYSADDR( ((uint32_t)(_ltqhc->xfer_buff))+ _ltqhc->xfer_count )));

	/* Start the split */
	if (_ltqhc->split>0)
	{
		hcsplt_data_t hcsplt;
		hcsplt.d32 = ltqusb_rreg (&hc_regs->hcsplt);
		hcsplt.b.spltena = 1;
		if (_ltqhc->split>1)
			hcsplt.b.compsplt = 1;
		else
			hcsplt.b.compsplt = 0;

		#if defined(__EN_ISOC__) && defined(__EN_ISOC_SPLIT__)
			if (_ltqhc->ep_type == LTQUSB_EP_TYPE_ISOC)
				hcsplt.b.xactpos = _ltqhc->isoc_xact_pos;
			else
		#endif
		hcsplt.b.xactpos = LTQUSB_HCSPLIT_XACTPOS_ALL;// if not ISO
		ltqusb_wreg(&hc_regs->hcsplt, hcsplt.d32);
		LTQ_DEBUGPL(DBG_HCDV, "  SPLIT: XACT_POS:0x%08x\n", hcsplt.d32);
	}

	{
		hcchar_data_t hcchar;
		hcchar.d32 = ltqusb_rreg(&hc_regs->hcchar);
//		hcchar.b.multicnt = _ltqhc->multi_count;
		hcchar.b.multicnt = 1;

		if (_ltqhc->ep_type == LTQUSB_EP_TYPE_INTR || _ltqhc->ep_type == LTQUSB_EP_TYPE_ISOC)
		{
			hfnum.d32 = ltqusb_rreg(&core_if->host_global_regs->hfnum);
			/* 1 if _next_ frame is odd, 0 if it's even */
			hcchar.b.oddfrm = (hfnum.b.frnum & 0x1) ? 0 : 1;
		}

		#ifdef __DEBUG__
			_ltqhc->start_hcchar_val = hcchar.d32;
			if (hcchar.b.chdis)
				LTQ_WARN("%s: chdis set, channel %d, hcchar 0x%08x\n",
					 __func__, _ltqhc->hc_num, hcchar.d32);
		#endif

		/* Set host channel enable after all other setup is complete. */
		hcchar.b.chen  = 1;
		hcchar.b.chdis = 0;
		hcchar.b.epdir =  _ltqhc->is_in;
		_ltqhc->hcchar=hcchar.d32;
	}

	LTQ_DEBUGPL(DBG_HCDV, "  HCCHART: 0x%08x\n", _ltqhc->hcchar);

	if (_ltqhc->ep_type == LTQUSB_EP_TYPE_ISOC || _ltqhc->ep_type == LTQUSB_EP_TYPE_INTR )
		new_phase=HC_STARTING_HIGH;
	else
		new_phase=HC_STARTING;

	if(do_now)
	{
		hcint_data_t      hcint;
		hfnum_data_t hfnum;
		hfnum.d32 = ltqusb_rreg(&_ltqhcd->core_if.host_global_regs->hfnum);
		if(hfnum.b.frrem>_ltqhcd->xfer_prot1_value)
		{
			_ltqhc->erron=0;
			hcint.d32 =0xFFFFFFFF;
			ltqusb_wreg(&hc_regs->hcint, hcint.d32);
			hcint.d32 =ltqusb_rreg(&hc_regs->hcintmsk);
			hcint.b.nak =0;
			hcint.b.ack =0;
			hcint.b.nyet=0;
			if(_ltqhc->erron)
			{
				hcint.b.ack  =1;
				hcint.b.nak  =1;
				hcint.b.nyet =1;
			}
			if(!_ltqhc->cancelling)
			{
				ltqusb_wreg(&hc_regs->hcintmsk, hcint.d32);
				ltqusb_wreg(&hc_regs->hcchar, _ltqhc->hcchar);
				_ltqhc->phase=HC_STARTED;
				return;
			}
		}
		else if(hrtimer_try_to_cancel(&_ltqhcd->xfer_stopper_timer)!=-1)
			ltqhcd_xfer_stopper(_ltqhcd);
	}
	_ltqhc->phase=new_phase;
}

/*!
   \brief Attempts to halt a host channel. This function should only be called
  to abort a transfer in DMA mode. Under normal circumstances in DMA mode, the
  controller halts the channel when the transfer is complete or a condition
  occurs that requires application intervention.

  In DMA mode, always sets the Channel Enable and Channel Disable bits of the
  HCCHARn register. The controller ensures there is space in the request
  queue before submitting the halt request.

  Some time may elapse before the core flushes any posted requests for this
  host channel and halts. The Channel Halted interrupt handler completes the
  deactivation of the host channel.
 */

int ltqhcd_hc_halt(ltqusb_core_if_t *_core_if,
                    ltqhcd_hc_t *_ltqhc,
                    ltqhcd_halt_status_e _halt_status)
{
	hcchar_data_t   hcchar;
	ltqusb_hc_regs_t           *hc_regs;
	if(!_ltqhc)   return -1;
	if(!_core_if) return -1;
	hc_regs          = _core_if->hc_regs[_ltqhc->hc_num];

	WARN_ON(_halt_status == HC_XFER_NO_HALT_STATUS);

	_ltqhc->cancelling=1;
	{
		hprt0_data_t hprt0;
		hprt0.d32 = ltqusb_rreg(_core_if->hprt0);
		if(hprt0.b.prtena == 0)
			return -1;
	}

	if (_halt_status == HC_XFER_URB_DEQUEUE ||
	    _halt_status == HC_XFER_AHB_ERR)
	{
		/*
		 * Disable all channel interrupts except Ch Halted. The URBD
		 * and EPQH state associated with this transfer has been cleared
		 * (in the case of URB_DEQUEUE), so the channel needs to be
		 * shut down carefully to prevent crashes.
		 */
		hcint_data_t hcintmsk;
		hcintmsk.d32 = 0;
		hcintmsk.b.chhltd = 1;
		ltqusb_wreg(&hc_regs->hcintmsk, hcintmsk.d32);

		/*
		 * Make sure no other interrupts besides halt are currently
		 * pending. Handling another interrupt could cause a crash due
		 * to the URBD and EPQH state.
		 */
		ltqusb_wreg(&hc_regs->hcint, ~hcintmsk.d32);

		/*
		 * Make sure the halt status is set to URB_DEQUEUE or AHB_ERR
		 * even if the channel was already halted for some other
		 * reason.
		 */
		_ltqhc->halt_status = _halt_status;
	}
	if (_ltqhc->phase==HC_STOPPING)
	{
		/*
		 * A halt has already been issued for this channel. This might
		 * happen when a transfer is aborted by a higher level in
		 * the stack.
		 */
		#ifdef __DEBUG__
			LTQ_PRINT("*** %s: Channel %d, double halt a channel***\n",
				  __func__, _ltqhc->hc_num);
		#endif
		return 0;
	}

	hcchar.d32 = ltqusb_rreg(&hc_regs->hcchar);
	if (hcchar.b.chen == 0)
	{
		_ltqhc->cancelling=0;
		/*
		 * The channel is either already halted or it hasn't
		 * started yet. In DMA mode, the transfer may halt if
		 * it finishes normally or a condition occurs that
		 * requires driver intervention. Don't want to halt
		 * the channel again. In either Slave or DMA mode,
		 * it's possible that the transfer has been assigned
		 * to a channel, but not started yet when an URB is
		 * dequeued. Don't want to halt a channel that hasn't
		 * started yet.
		 */
		if (_ltqhc->phase==HC_STARTED)
			return 0;
		if (_ltqhc->phase==HC_STOPPED)
			return 0;
		_ltqhc->phase=HC_IDLE;
		return -1;
	}
	_ltqhc->phase=HC_STOPPING;

//	hcchar.d32 = ltqusb_rreg(&hc_regs->hcchar);
//	hcchar.b.chen = 1;
	hcchar.b.chdis = 1;
	ltqusb_wreg(&hc_regs->hcchar, hcchar.d32);

	_ltqhc->halt_status = _halt_status;

	LTQ_DEBUGPL(DBG_HCDV, "%s: Channel %d\n" , __func__, _ltqhc->hc_num);
	LTQ_DEBUGPL(DBG_HCDV, "  hcchar: 0x%08x\n"   , hcchar.d32);
	LTQ_DEBUGPL(DBG_HCDV, "  halt_status: %d\n"  , _ltqhc->halt_status);
	return 0;
}

/*!
   \brief Clears a host channel.
 */
void ltqhcd_hc_cleanup(ltqusb_core_if_t *_core_if, ltqhcd_hc_t *_ltqhc)
{
	ltqusb_hc_regs_t *hc_regs;

	_ltqhc->phase=HC_IDLE;
	_ltqhc->epqh=0;
	_ltqhc->urbd=0;
	_ltqhc->cancelling=0;
	_ltqhc->stopping=0;

	/*
	 * Clear channel interrupt enables and any unhandled channel interrupt
	 * conditions.
	 */
	hc_regs = _core_if->hc_regs[_ltqhc->hc_num];
	ltqusb_wreg(&hc_regs->hcintmsk, 0);
	ltqusb_wreg(&hc_regs->hcint, 0xFFFFFFFF);

	#ifdef __DEBUG__
		{
			hcchar_data_t hcchar;
			hcchar.d32 = ltqusb_rreg(&hc_regs->hcchar);
			if (hcchar.b.chdis)
				LTQ_WARN("%s: chdis set, channel %d, hcchar 0x%08x\n", __func__, _ltqhc->hc_num, hcchar.d32);
		}
	#endif
}





#ifdef __DEBUG__
	static
	void dump_urb_info(struct urb *_urb, char* _fn_name)
	{
		LTQ_PRINT("%s, urb %p\n"          , _fn_name, _urb);
		LTQ_PRINT("  Device address: %d\n", usb_pipedevice(_urb->pipe));
		LTQ_PRINT("  Endpoint: %d, %s\n"  , usb_pipeendpoint(_urb->pipe),
		                                    (usb_pipein(_urb->pipe) ? "IN" : "OUT"));
		LTQ_PRINT("  Endpoint type: %s\n",
		    ({	char *pipetype;
		    	switch (usb_pipetype(_urb->pipe)) {
		    		case PIPE_CONTROL:     pipetype = "CONTROL"; break;
		    		case PIPE_BULK:        pipetype = "BULK"; break;
		    		case PIPE_INTERRUPT:   pipetype = "INTERRUPT"; break;
		    		case PIPE_ISOCHRONOUS: pipetype = "ISOCHRONOUS"; break;
		    		default:               pipetype = "UNKNOWN"; break;
		    	};
		    	pipetype;
		    }));
		LTQ_PRINT("  Speed: %s\n",
		    ({	char *speed;
		    	switch (_urb->dev->speed) {
		    		case USB_SPEED_HIGH: speed = "HIGH"; break;
		    		case USB_SPEED_FULL: speed = "FULL"; break;
		    		case USB_SPEED_LOW:  speed = "LOW"; break;
		    		default:             speed = "UNKNOWN"; break;
		    	};
		    	speed;
		    }));
		LTQ_PRINT("  Max packet size: %d\n",
			  usb_maxpacket(_urb->dev, _urb->pipe, usb_pipeout(_urb->pipe)));
		LTQ_PRINT("  Data buffer length: %d\n", _urb->transfer_buffer_length);
		LTQ_PRINT("  Transfer buffer: %p, Transfer DMA: %p\n",
			  _urb->transfer_buffer, (void *)_urb->transfer_dma);
		LTQ_PRINT("  Setup buffer: %p, Setup DMA: %p\n",
			  _urb->setup_packet, (void *)_urb->setup_dma);
		LTQ_PRINT("  Interval: %d\n", _urb->interval);
		if (usb_pipetype(_urb->pipe) == PIPE_ISOCHRONOUS)
		{
			int i;
			for (i = 0; i < _urb->number_of_packets;  i++)
			{
				LTQ_PRINT("  ISO Desc %d:\n", i);
				LTQ_PRINT("    offset: %d, length %d\n",
				    _urb->iso_frame_desc[i].offset,
				    _urb->iso_frame_desc[i].length);
			}
		}
	}

#if 0
	static
	void dump_channel_info(ltqhcd_hcd_t *_ltqhcd, ltqhcd_hc_t *_ltqhc)
	{
		if (_ltqhc != NULL)
		{
			ltqusb_hc_regs_t *hc_regs;

			hcchar_data_t  hcchar;
			hcsplt_data_t  hcsplt;
			hctsiz_data_t  hctsiz;
			uint32_t       hcdma;

			hc_regs = _ltqhcd->core_if.hc_regs[_ltqhc->hc_num];
			hcchar.d32 = ltqusb_rreg(&hc_regs->hcchar);
			hcsplt.d32 = ltqusb_rreg(&hc_regs->hcsplt);
			hctsiz.d32 = ltqusb_rreg(&hc_regs->hctsiz);
			hcdma      = ltqusb_rreg(&hc_regs->hcdma);

			LTQ_PRINT("  Assigned to channel %d:\n"       , _ltqhc->hc_num);
			LTQ_PRINT("    hcchar 0x%08x, hcsplt 0x%08x\n", hcchar.d32, hcsplt.d32);
			LTQ_PRINT("    hctsiz 0x%08x, hcdma 0x%08x\n" , hctsiz.d32, hcdma);
			LTQ_PRINT("    dev_addr: %d, ep_num: %d, is_in: %d\n",
			   _ltqhc->dev_addr, _ltqhc->ep_num, _ltqhc->is_in);
			LTQ_PRINT("    ep_type: %d\n"        , _ltqhc->ep_type);
			LTQ_PRINT("    max_packet_size: %d\n", _ltqhc->mps);
			LTQ_PRINT("    data_pid_start: %d\n" , _ltqhc->data_pid_start);
			LTQ_PRINT("    halt_status: %d\n"    , _ltqhc->halt_status);
			LTQ_PRINT("    xfer_buff: %p\n"      , _ltqhc->xfer_buff);
			LTQ_PRINT("    xfer_len: %d\n"       , _ltqhc->xfer_len);
			LTQ_PRINT("    epqh: %p\n"           , _ltqhc->epqh);
			LTQ_PRINT("    urbd: %p\n"           , _ltqhc->urbd);
			LTQ_PRINT("  NP :\n");
		}
	}
#endif
#endif //__DEBUG__


/*!
   \brief This function writes a packet into the Tx FIFO associated with the Host
  Channel. For a channel associated with a non-periodic EP, the non-periodic
  Tx FIFO is written. For a channel associated with a periodic EP, the
  periodic Tx FIFO is written. This function should only be called in Slave
  mode.

  Upon return the xfer_buff and xfer_count fields in _hc are incremented by
  then number of bytes written to the Tx FIFO.
 */

#ifdef __ENABLE_DUMP__
	void ltqhcd_dump_state(ltqhcd_hcd_t *_ltqhcd)
	{
		int num_channels;
		int i;
		num_channels = _ltqhcd->core_if.params.host_channels;
		LTQ_PRINT("\n");
		LTQ_PRINT("************************************************************\n");
		LTQ_PRINT("HCD State:\n");
		LTQ_PRINT("  Num channels: %d\n", num_channels);
		for (i = 0; i < num_channels; i++) {
			ltqhcd_hc_t *hc = &_ltqhcd->ltqhc[i];
			LTQ_PRINT("  Channel %d:\n", hc->hc_num);
			LTQ_PRINT("    dev_addr: %d, ep_num: %d, ep_is_in: %d\n",
				  hc->dev_addr, hc->ep_num, hc->is_in);
			LTQ_PRINT("    speed: %d\n"          , hc->speed);
			LTQ_PRINT("    ep_type: %d\n"        , hc->ep_type);
			LTQ_PRINT("    mps: %d\n", hc->mps);
			LTQ_PRINT("    data_pid_start: %d\n" , hc->data_pid_start);
			LTQ_PRINT("    xfer_buff: %p\n"      , hc->xfer_buff);
			LTQ_PRINT("    xfer_len: %d\n"       , hc->xfer_len);
			LTQ_PRINT("    xfer_count: %d\n"     , hc->xfer_count);
			LTQ_PRINT("    halt_status: %d\n"    , hc->halt_status);
			LTQ_PRINT("    split: %d\n"          , hc->split);
			LTQ_PRINT("    hub_addr: %d\n"       , hc->hub_addr);
			LTQ_PRINT("    port_addr: %d\n"      , hc->port_addr);
			#if defined(__EN_ISOC__) && defined(__EN_ISOC_SPLIT__)
				LTQ_PRINT("    isoc_xact_pos: %d\n"       , hc->isoc_xact_pos);
			#endif

			LTQ_PRINT("    epqh: %p\n"           , hc->epqh);
			LTQ_PRINT("    short_rw: %d\n"       , hc->short_rw);
			LTQ_PRINT("    control_phase: %d\n"  , hc->control_phase);
			if(hc->epqh)
			{
				LTQ_PRINT("    do_ping: %d\n"        , hc->epqh->do_ping);
			}
			LTQ_PRINT("    start_pkt_count: %d\n"       , hc->start_pkt_count);
		}
		LTQ_PRINT("************************************************************\n");
		LTQ_PRINT("\n");
	}
#endif //__ENABLE_DUMP__

