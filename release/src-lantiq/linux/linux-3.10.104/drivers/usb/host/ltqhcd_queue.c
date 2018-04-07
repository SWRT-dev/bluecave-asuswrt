/*****************************************************************************
 **   FILE NAME       : ltqhcd_queue.c
 **   PROJECT         : Lantiq USB sub-system V3
 **   MODULES         : Lantiq USB sub-system Host and Device driver
 **   SRC VERSION     : 3.2
 **   DATE            : 1/Jan/2011
 **   AUTHOR          : Chen, Howard
 **   DESCRIPTION     : This file contains the functions to manage Queue Heads and Queue
 **                     Transfer Descriptors.
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
 \file ltqhcd_queue.c
 \ingroup LTQUSB_DRIVER_V3
  \brief This file contains the functions to manage Queue Heads and Queue
  Transfer Descriptors.
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

#include "ltqusb_plat.h"
#include "ltqusb_regs.h"
#include "ltqusb_cif.h"
#include "ltqhcd.h"

#ifdef __EPQD_DESTROY_TIMEOUT__
	#define epqh_self_destroy_timeout 300
	static
	void eqph_destroy_func(unsigned long _ptr)
	{
		ltqhcd_epqh_t *epqh=(ltqhcd_epqh_t *)_ptr;
		if(epqh)
		{
			if(epqh->sysep)
			{
				epqh->sysep->hcpriv=NULL;
			}
			ltqhcd_epqh_free (epqh);
		}
	}
#endif

/*!
  \brief This function allocates and initializes a EPQH.

  \param _ltqhcd The HCD state structure for the USB Host controller.
  \param[in] _urb Holds the information about the device/endpoint that we need
  to initialize the EPQH.

  \return Returns pointer to the newly allocated EPQH, or NULL on error.
 */
static
ltqhcd_epqh_t *ltqhcd_epqh_create (ltqhcd_hcd_t *_ltqhcd, struct urb *_urb)
{
	ltqhcd_epqh_t *epqh;

	hprt0_data_t   hprt0;
	struct usb_host_endpoint *sysep = ltqhcd_urb_to_endpoint(_urb);

	/* Allocate memory */
	epqh=(ltqhcd_epqh_t *) kmalloc (sizeof(ltqhcd_epqh_t), GFP_ATOMIC);

	if(epqh == NULL)
		return NULL;

	memset (epqh, 0, sizeof (ltqhcd_epqh_t));

	epqh->sysep=sysep;

	epqh->devno=_urb->dev->devnum;

	epqh->ltqhcd=_ltqhcd;
	epqh->phase=EPQH_IDLE;

	/* Initialize EPQH */
	switch (usb_pipetype(_urb->pipe))
	{
		case PIPE_CONTROL    : epqh->ep_type = LTQUSB_EP_TYPE_CTRL; epqh->burst_on=0;break;
		case PIPE_BULK       : epqh->ep_type = LTQUSB_EP_TYPE_BULK; epqh->burst_on=1;break;
		case PIPE_ISOCHRONOUS: epqh->ep_type = LTQUSB_EP_TYPE_ISOC; epqh->burst_on=0;break;
		case PIPE_INTERRUPT  : epqh->ep_type = LTQUSB_EP_TYPE_INTR; epqh->burst_on=0;break;
	}

	usb_settoggle(_urb->dev, usb_pipeendpoint (_urb->pipe), !usb_pipein(_urb->pipe), LTQUSB_HC_PID_DATA0);
	epqh->mps = usb_maxpacket(_urb->dev, _urb->pipe, !(usb_pipein(_urb->pipe)));

	INIT_LIST_HEAD(&epqh->urbd_list);
	INIT_LIST_HEAD(&epqh->release_list);
	INIT_LIST_HEAD(&epqh->ql);
	INIT_LIST_HEAD(&epqh->ql_all);
	INIT_URBD_LIST(epqh);
	INIT_URBD_RELEASE_LIST(epqh);

	/* FS/LS Enpoint on HS Hub
	 * NOT virtual root hub */
	epqh->need_split = 0;
	hprt0.d32 = ltqusb_read_hprt0 (&_ltqhcd->core_if);
	if (hprt0.b.prtspd == LTQUSB_HPRT0_PRTSPD_HIGH_SPEED)
		_ltqhcd->use_hrtimer=1;
	else
		_ltqhcd->use_hrtimer=0;
	if (hprt0.b.prtspd == LTQUSB_HPRT0_PRTSPD_HIGH_SPEED &&
	    ((_urb->dev->speed == USB_SPEED_LOW) ||
	     (_urb->dev->speed == USB_SPEED_FULL)) &&
	     (_urb->dev->tt) && (_urb->dev->tt->hub->devnum != 1))
	{
		LTQ_DEBUGPL(DBG_HCD, "QH init: EP %d: TT found at hub addr %d, for port %d\n",
		       usb_pipeendpoint(_urb->pipe), _urb->dev->tt->hub->devnum,
		       _urb->dev->ttport);
		epqh->need_split = 1;
		epqh->burst_on=0;
	}

	if (epqh->ep_type == LTQUSB_EP_TYPE_INTR ||
	    epqh->ep_type == LTQUSB_EP_TYPE_ISOC)
	{
		/* Compute scheduling parameters once and save them. */
		epqh->interval    = _urb->interval+1;
		if(epqh->need_split)
			epqh->interval *= 8;
	}

	#ifdef __EN_ISOC__
		if (epqh->ep_type == LTQUSB_EP_TYPE_ISOC)
			_ltqhcd->isoc_ep_count++;
	#endif

	epqh->period_counter=0;

	#ifdef __EPQD_DESTROY_TIMEOUT__
		/* Start a timer for this transfer. */
		init_timer(&epqh->destroy_timer);
		epqh->destroy_timer.function = eqph_destroy_func;
		epqh->destroy_timer.data = (unsigned long)(epqh);
	#endif

	#ifdef __DEBUG__
		LTQ_DEBUGPL(DBG_HCD , "LTQUSB HCD EPQH Initialized\n");
		LTQ_DEBUGPL(DBG_HCDV, "LTQUSB HCD EPQH  - epqh = %p\n", epqh);
		LTQ_DEBUGPL(DBG_HCDV, "LTQUSB HCD EPQH  - Device Address = %d EP %d, %s\n",
			    _urb->dev->devnum,
			    usb_pipeendpoint(_urb->pipe),
			    usb_pipein(_urb->pipe) == USB_DIR_IN ? "IN" : "OUT");
		LTQ_DEBUGPL(DBG_HCDV, "LTQUSB HCD EPQH  - Speed = %s\n",
			    ({ char *speed; switch (_urb->dev->speed) {
			    case USB_SPEED_LOW: speed  = "low" ; break;
			    case USB_SPEED_FULL: speed = "full"; break;
			    case USB_SPEED_HIGH: speed = "high"; break;
			    default: speed = "?";	break;
			    }; speed;}));
		LTQ_DEBUGPL(DBG_HCDV, "LTQUSB HCD EPQH  - Type = %s\n",
			({
				char *type; switch (epqh->ep_type)
				{
				    case LTQUSB_EP_TYPE_ISOC: type = "isochronous"; break;
				    case LTQUSB_EP_TYPE_INTR: type = "interrupt"  ; break;
				    case LTQUSB_EP_TYPE_CTRL: type = "control"    ; break;
				    case LTQUSB_EP_TYPE_BULK: type = "bulk"       ; break;
				    default: type = "?";	break;
				};
				type;
			}));
		if (epqh->ep_type == LTQUSB_EP_TYPE_INTR)
			LTQ_DEBUGPL(DBG_HCDV, "LTQUSB HCD EPQH - interval = %d\n", epqh->interval);
	#endif

	list_add_tail(&epqh->ql_all, &_ltqhcd->epqh_list_all);

	LOCK_EPQH_LIST(_ltqhcd);
	switch (epqh->ep_type)
	{
		case LTQUSB_EP_TYPE_CTRL:
			list_add_tail(&epqh->ql, &_ltqhcd->epqh_list_ctrl);
			break;
		case LTQUSB_EP_TYPE_BULK:
			list_add_tail(&epqh->ql, &_ltqhcd->epqh_list_bulk);
			break;
		case LTQUSB_EP_TYPE_INTR:
			list_add_tail(&epqh->ql, &_ltqhcd->epqh_list_intr);
			break;
	#ifdef __EN_ISOC__
		case LTQUSB_EP_TYPE_ISOC:
			list_add_tail(&epqh->ql, &_ltqhcd->epqh_list_isoc);
			break;
	#endif
	}
	UNLOCK_EPQH_LIST(_ltqhcd);
	return epqh;
}






/*!
  \brief Free the EPQH.  EPQH should already be removed from a list.
  URBD list should already be empty if called from URB Dequeue.

  \param[in] _epqh The EPQH to free.
 */
void ltqhcd_epqh_free (ltqhcd_epqh_t *_epqh)
{
	unsigned long     flags;
	if(!_epqh)
		return;

	if(_epqh->sysep) _epqh->sysep->hcpriv=NULL;
	_epqh->sysep=NULL;

	local_irq_save (flags);
	if(!list_empty(&_epqh->urbd_list) || !list_empty(&_epqh->release_list))
		LTQ_WARN("%s() %p epqh not empty\n",__func__,_epqh);
	else
	{
		LOCK_EPQH_LIST_ALL(_epqh->ltqhcd);
		if (!list_empty(&_epqh->ql_all))
			list_del_init (&_epqh->ql_all);
		UNLOCK_EPQH_LIST_ALL(_epqh->ltqhcd);

		LOCK_EPQH_LIST(_epqh->ltqhcd);
		if (!list_empty(&_epqh->ql))
			list_del_init (&_epqh->ql);
		UNLOCK_EPQH_LIST(_epqh->ltqhcd);

		#ifdef __EPQD_DESTROY_TIMEOUT__
			del_timer(&_epqh->destroy_timer);
		#endif
		kfree (_epqh);
	}
	local_irq_restore (flags);
}


void ltqhcd_epqh_idle(ltqhcd_epqh_t *_epqh)
{
	unsigned long flags;
	local_irq_save(flags);
	LOCK_URBD_LIST(_epqh);
	if(_epqh->phase==EPQH_DISABLING ){
	}else
	if (list_empty(&_epqh->urbd_list))
	{
		if(_epqh->ep_type == LTQUSB_EP_TYPE_ISOC || _epqh->ep_type == LTQUSB_EP_TYPE_INTR)
			_epqh->phase=EPQH_STDBY;
		else
		{
			_epqh->phase=EPQH_IDLE;
			#ifdef __EPQD_DESTROY_TIMEOUT__
				del_timer(&_epqh->destroy_timer);
				_epqh->destroy_timer.expires = jiffies + (HZ*epqh_self_destroy_timeout);
				add_timer(&_epqh->destroy_timer );
			#endif
		}
	}
	else
	{
		_epqh->phase=EPQH_READY;
		#ifdef __EPQD_DESTROY_TIMEOUT__
			del_timer(&_epqh->destroy_timer);
		#endif
	}
	UNLOCK_URBD_LIST(_epqh);
	local_irq_restore(flags);
}


void ltqhcd_epqh_idle_periodic(ltqhcd_epqh_t *_epqh)
{
	unsigned long flags;
	if(_epqh->ep_type != LTQUSB_EP_TYPE_ISOC && _epqh->ep_type != LTQUSB_EP_TYPE_INTR && _epqh->phase!=EPQH_STDBY)
		return;

	local_irq_save(flags);
	LOCK_URBD_LIST(_epqh);
	if (!list_empty(&_epqh->urbd_list))
		LTQ_WARN("%s() invalid epqh state(not empty)\n",__func__);

	if(_epqh->phase==EPQH_DISABLING ){
	}else
	{
		_epqh->phase=EPQH_IDLE;

		#ifdef __EPQD_DESTROY_TIMEOUT__
			del_timer(&_epqh->destroy_timer);
			_epqh->destroy_timer.expires = jiffies + (HZ*epqh_self_destroy_timeout);
			add_timer(&_epqh->destroy_timer );
		#endif

		#ifdef __EN_ISOC__
			if (_epqh->ep_type == LTQUSB_EP_TYPE_ISOC)
				_epqh->ltqhcd->isoc_ep_count--;
		#endif
	}
	UNLOCK_URBD_LIST(_epqh);
	local_irq_restore(flags);
}



static
void process_unaligned( ltqhcd_urbd_t *_urbd, ltqhcd_epqh_t *_epqh, ltqusb_core_if_t *_core_if)
{
	#if   defined(__UNALIGNED_BUF_ADJ__) || defined(__UNALIGNED_BUF_CHK__)
	if(!_urbd->aligned_checked)
	{
		#if   defined(__UNALIGNED_BUF_ADJ__)
			uint32_t xfer_len;

			if(_urbd->aligned_buf)
			{
				ltqusb_free_buf_h(_urbd->aligned_buf);
				_urbd->aligned_buf=NULL;
			}
			if(_urbd->aligned_setup)
			{
				ltqusb_free_buf_h(_urbd->aligned_setup);
				_urbd->aligned_setup=NULL;
			}

			xfer_len=_urbd->xfer_len;
			if(_urbd->is_in && xfer_len<_epqh->mps)
				xfer_len = _epqh->mps;

			if(xfer_len > 0 && ((unsigned long)_urbd->xfer_buff) & _core_if->unaligned_mask)
			{
				_urbd->aligned_buf = ltqusb_alloc_buf_h(xfer_len, _urbd->is_in);
				if(!_urbd->aligned_buf)
					LTQ_ERROR("Unaligned buf alloc error\n");
				else if(!_urbd->is_in)
					memcpy(_urbd->aligned_buf, _urbd->xfer_buff, xfer_len);
			}
			if(_epqh->ep_type==LTQUSB_EP_TYPE_CTRL)
			{
				if(((unsigned long)_urbd->setup_buff) & _core_if->unaligned_mask)
				{
					_urbd->aligned_setup = ltqusb_alloc_buf_h(8,0);
					if(!_urbd->aligned_setup)
						LTQ_ERROR("Unaligned setup buf alloc error\n");
					else
						memcpy(_urbd->aligned_setup, _urbd->setup_buff, 8);
				}
			}
		#elif defined(__UNALIGNED_BUF_CHK__)
			if(_urbd->is_in)
			{
				if(_urbd->xfer_len==0)
					LTQ_WARN("%s():%d IN xfer while length is zero \n",__func__,__LINE__);
				else{
					if(((unsigned long)_urbd->xfer_buff) & _core_if->unaligned_mask)
						LTQ_WARN("%s():%d IN xfer Buffer UNALIGNED\n",__func__,__LINE__);
				}
			}
			else
			{
				if(_urbd->xfer_len > 0 && (((unsigned long)_urbd->xfer_buff) & _core_if->unaligned_mask))
					LTQ_WARN("%s():%d OUT xfer Buffer UNALIGNED\n",__func__,__LINE__);
			}
			if(_epqh->ep_type==LTQUSB_EP_TYPE_CTRL)
			{
				if(((unsigned long)_urbd->setup_buff) & _core_if->unaligned_mask)
					LTQ_WARN("%s():%d SETUP xfer Buffer UNALIGNED\n",__func__,__LINE__);
			}
		#endif
	}
	_urbd->aligned_checked=1;
	#endif
}


ltqhcd_epqh_t *ltqhcd_urbd_create (ltqhcd_hcd_t *_ltqhcd,struct urb *_urb)
{
	ltqhcd_urbd_t            *urbd;
	struct usb_host_endpoint *sysep;
	ltqhcd_epqh_t            *epqh=NULL;
	unsigned long             flags;

	local_irq_save(flags);

	sysep = ltqhcd_urb_to_endpoint(_urb);

	LOCK_EPQH_LIST_ALL(_ltqhcd);
	epqh = sysep_to_epqh(_ltqhcd, sysep,1);

	if (!epqh)
	{
		sysep->hcpriv = NULL;
		epqh = ltqhcd_epqh_create (_ltqhcd, _urb);
	}
	UNLOCK_EPQH_LIST_ALL(_ltqhcd);

	if (!epqh)
	{
		LTQ_ERROR("EPQH Error alloc\n");
		local_irq_restore (flags);
		return (ltqhcd_epqh_t *)NULL;
	}
	if(epqh->phase==EPQH_DISABLING)
	{
		LTQ_ERROR("EPQH Error alloc while disabling\n");
		local_irq_restore (flags);
		return (ltqhcd_epqh_t *)NULL;
	}
	sysep->hcpriv = epqh;

	if(_urb->hcpriv)
	{
		LTQ_WARN("%s() Previous urb->hcpriv exist %p\n",__func__,_urb->hcpriv);
	#if 1
		local_irq_restore (flags);
		return (ltqhcd_epqh_t *)NULL;
	#else
		urbd = _urb->hcpriv;
		if(urbd->epqh!=epqh)
			LTQ_WARN("%s() Previous urb->hcpriv exist %p and epqh not the same %p %p\n",__func__,_urb->hcpriv,urbd->epqh,epqh);
	#endif
	}
	else
	{
		urbd =  (ltqhcd_urbd_t *) kmalloc (sizeof(ltqhcd_urbd_t), GFP_ATOMIC);
		if (!urbd)
		{
			local_irq_restore (flags);
			return (ltqhcd_epqh_t *)NULL;
		}
		memset (urbd, 0, sizeof (ltqhcd_urbd_t));
		INIT_LIST_HEAD(&urbd->ql);
	}

	_urb->hcpriv = urbd;
	urbd->urb   = _urb;
	urbd->epqh  =  epqh;
	urbd->status= -EINPROGRESS;

	urbd->is_in=usb_pipein(_urb->pipe) ? 1 : 0;

	#ifdef __EN_ISOC__
	if(epqh->ep_type == LTQUSB_EP_TYPE_ISOC)
	{
		if((_urb->transfer_flags & URB_NO_TRANSFER_DMA_MAP) && _urb->transfer_dma && (_urb->transfer_dma &3)==0)
			urbd->xfer_buff = (uint8_t *) (KSEG1ADDR((uint32_t *)_urb->transfer_dma));
		else
			urbd->xfer_buff = (uint8_t *) _urb->transfer_buffer;
	}
	else
	#endif
	{
		urbd->xfer_len=_urb->transfer_buffer_length;
		if(urbd->xfer_len>0)
		{
			if((_urb->transfer_flags & URB_NO_TRANSFER_DMA_MAP) && _urb->transfer_dma && (_urb->transfer_dma &3)==0)
				urbd->xfer_buff = (uint8_t *) (KSEG1ADDR((uint32_t *)_urb->transfer_dma));
			else
				urbd->xfer_buff = (uint8_t *) _urb->transfer_buffer;
		}
	}

	#if 1 // cache write-back, so DMA engine can get correct content. Precaution
		if(urbd->xfer_len)
			dma_cache_wback_inv((unsigned long)urbd->xfer_buff, urbd->xfer_len);
	#endif

	if(epqh->ep_type == LTQUSB_EP_TYPE_CTRL)
	{
		if(_urb->setup_dma && (_urb->setup_dma &3)==0 )
			urbd->setup_buff = (uint8_t *) (KSEG1ADDR((uint32_t *)_urb->setup_dma));
		else
			urbd->setup_buff = (uint8_t *) _urb->setup_packet;
		#if 1 // cache write-back, so DMA engine can get correct content. Precaution
			dma_cache_wback_inv((unsigned long)urbd->setup_buff, 16);
		#endif
	}

	LOCK_URBD_LIST(epqh);
	if (!list_empty(&urbd->ql))
		list_del_init(&urbd->ql);
	list_add_tail(&urbd->ql, &epqh->urbd_list);
	epqh->urbd_count++;
	UNLOCK_URBD_LIST(epqh);

	process_unaligned( urbd, epqh, (&_ltqhcd->core_if));

	local_irq_restore (flags);
	return epqh;
}



ltqhcd_epqh_t * sysep_to_epqh(ltqhcd_hcd_t *_ltqhcd, struct usb_host_endpoint *_sysep, int _skipcancel)
{
	ltqhcd_epqh_t *epqh;

	list_for_each_entry( epqh, &_ltqhcd->epqh_list_all, ql_all)
	{
		if(!(_skipcancel && epqh->cancelling))
		{
			if( epqh->sysep==_sysep)
				return epqh;
		}
	}
	return NULL;
}

