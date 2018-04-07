/*****************************************************************************
 **   FILE NAME       : ltqhcd_intr.c
 **   PROJECT         : Lantiq USB sub-system V3
 **   MODULES         : Lantiq USB sub-system Host and Device driver
 **   SRC VERSION     : 3.2
 **   DATE            : 1/Jan/2011
 **   AUTHOR          : Chen, Howard
 **   DESCRIPTION     : This file contains the implementation of the HCD Interrupt handlers.
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
 \file ltqhcd_intr.c
 \ingroup LTQUSB_DRIVER_V3
 \brief This file contains the implementation of the HCD Interrupt handlers.
*/


#include <linux/version.h>
#include "ltqusb_version.h"
#include "ltqusb_plat.h"
#include "ltqusb_regs.h"
#include "ltqusb_cif.h"

#include "ltqhcd.h"


#define ABORT_URBD
#define ABORT_URBD_MAX 5

#define USE_SAFE_URBD


/* Macro used to clear one channel interrupt */
#define clear_hc_int(_hc_regs_,_intr_) \
	do { \
		hcint_data_t hcint_clear = {.d32 = 0}; \
		hcint_clear.b._intr_ = 1; \
		ltqusb_wreg(&((_hc_regs_)->hcint), hcint_clear.d32); \
	} while (0)

/*
 * Macro used to disable one channel interrupt. Channel interrupts are
 * disabled when the channel is halted or released by the interrupt handler.
 * There is no need to handle further interrupts of that type until the
 * channel is re-assigned. In fact, subsequent handling may cause crashes
 * because the channel structures are cleaned up when the channel is released.
 */
#define disable_hc_int(_hc_regs_,_intr_) \
	do { \
		hcint_data_t hcintmsk = {.d32 = 0}; \
		hcintmsk.b._intr_ = 1; \
		ltqusb_mreg(&((_hc_regs_)->hcintmsk), hcintmsk.d32, 0); \
	} while (0)

#define enable_hc_int(_hc_regs_,_intr_) \
	do { \
		hcint_data_t hcintmsk = {.d32 = 0}; \
		hcintmsk.b._intr_ = 1; \
		ltqusb_mreg(&((_hc_regs_)->hcintmsk),0, hcintmsk.d32); \
	} while (0)

/*
 * Save the starting data toggle for the next transfer. The data toggle is
 * saved in the QH for non-control transfers and it's saved in the QTD for
 * control transfers.
 */
uint8_t read_data_toggle(ltqusb_hc_regs_t *_hc_regs)
{
	hctsiz_data_t hctsiz;
	hctsiz.d32 = ltqusb_rreg(&_hc_regs->hctsiz);
	return(hctsiz.b.pid);
}


static
void release_channel_dump(ltqhcd_hc_t      *ltqhc,
                               struct urb       *urb,
                               ltqhcd_epqh_t    *epqh,
                               ltqhcd_urbd_t    *urbd,
                               ltqhcd_halt_status_e  halt_status)
{
	#ifdef __DEBUG__
		printk(KERN_INFO);
		switch (halt_status)
		{
			case HC_XFER_NO_HALT_STATUS:
				printk("HC_XFER_NO_HALT_STATUS");break;
			case HC_XFER_URB_COMPLETE:
				printk("HC_XFER_URB_COMPLETE");break;
			case HC_XFER_AHB_ERR:
				printk("HC_XFER_AHB_ERR");break;
			case HC_XFER_STALL:
				printk("HC_XFER_STALL");break;
			case HC_XFER_BABBLE_ERR:
				printk("HC_XFER_BABBLE_ERR");break;
			case HC_XFER_XACT_ERR:
				printk("HC_XFER_XACT_ERR");break;
			case HC_XFER_URB_DEQUEUE:
				printk("HC_XFER_URB_DEQUEUE");break;
			case HC_XFER_FRAME_OVERRUN:
				printk("HC_XFER_FRAME_OVERRUN");break;
			case HC_XFER_DATA_TOGGLE_ERR:
				printk("HC_XFER_DATA_TOGGLE_ERR");break;
			case HC_XFER_STOPPED:
				printk("HC_XFER_STOPPED");break;
			case HC_XFER_COMPLETE:
				printk("HC_XFER_COMPLETE");break;
			default:
				printk("KNOWN");break;
		}
		if(ltqhc)
			printk("Ch %d %s%s S%d " , ltqhc->hc_num
				,(ltqhc->ep_type == LTQUSB_EP_TYPE_CTRL)?"CTRL-":
				   ((ltqhc->ep_type == LTQUSB_EP_TYPE_BULK)?"BULK-":
				     ((ltqhc->ep_type == LTQUSB_EP_TYPE_INTR)?"INTR-":
				       ((ltqhc->ep_type == LTQUSB_EP_TYPE_ISOC)?"ISOC-":"????"
				       )
				     )
				   )
				,(ltqhc->is_in)?"IN":"OUT"
				,(ltqhc->split)
				);
		else
			printk(" [NULL HC] ");
		printk("urb=%p epqh=%p urbd=%p\n",urb,epqh,urbd);

		if(urb)
		{
			printk(KERN_INFO "  Device address: %d\n", usb_pipedevice(urb->pipe));
			printk(KERN_INFO "  Endpoint: %d, %s\n", usb_pipeendpoint(urb->pipe),
				    (usb_pipein(urb->pipe) ? "IN" : "OUT"));
			printk(KERN_INFO "  Endpoint type: %s\n",
				    ({char *pipetype;
				    switch (usb_pipetype(urb->pipe)) {
					    case PIPE_CONTROL: pipetype = "CTRL"; break;
					    case PIPE_BULK: pipetype = "BULK"; break;
					    case PIPE_INTERRUPT: pipetype = "INTR"; break;
					    case PIPE_ISOCHRONOUS: pipetype = "ISOC"; break;
					    default: pipetype = "????"; break;
				    }; pipetype;}));
			printk(KERN_INFO "  Speed: %s\n",
				    ({char *speed;
				    switch (urb->dev->speed) {
					    case USB_SPEED_HIGH: speed = "HS"; break;
					    case USB_SPEED_FULL: speed = "FS"; break;
					    case USB_SPEED_LOW: speed = "LS"; break;
				    	default: speed = "????"; break;
				    }; speed;}));
			printk(KERN_INFO "  Max packet size: %d\n",
				    usb_maxpacket(urb->dev, urb->pipe, usb_pipeout(urb->pipe)));
			printk(KERN_INFO "  Data buffer length: %d/%d\n",urb->actual_length, urb->transfer_buffer_length);
			printk(KERN_INFO "  Transfer buffer: %p, Transfer DMA: %p\n",
				    urb->transfer_buffer, (void *)urb->transfer_dma);
			printk(KERN_INFO "  Setup buffer: %p, Setup DMA: %p\n",
				    urb->setup_packet, (void *)urb->setup_dma);
			printk(KERN_INFO "  Interval: %d\n", urb->interval);
		}
		if(urbd)
		{
			switch (urbd->status)
			{
				case HC_XFER_NO_HALT_STATUS:
					printk(KERN_INFO "  STATUS:HC_XFER_NO_HALT_STATUS\n");break;
				case HC_XFER_URB_COMPLETE:
					printk(KERN_INFO "  STATUS:HC_XFER_URB_COMPLETE\n");break;
				case HC_XFER_AHB_ERR:
					printk(KERN_INFO "  STATUS:HC_XFER_AHB_ERR\n");break;
				case HC_XFER_STALL:
					printk(KERN_INFO "  STATUS:HC_XFER_STALL\n");break;
				case HC_XFER_BABBLE_ERR:
					printk(KERN_INFO "  STATUS:HC_XFER_BABBLE_ERR\n");break;
				case HC_XFER_XACT_ERR:
					printk(KERN_INFO "  STATUS:HC_XFER_XACT_ERR\n");break;
				case HC_XFER_URB_DEQUEUE:
					printk(KERN_INFO "  STATUS:HC_XFER_URB_DEQUEUE\n");break;
				case HC_XFER_FRAME_OVERRUN:
					printk(KERN_INFO "  STATUS:HC_XFER_FRAME_OVERRUN\n");break;
				case HC_XFER_DATA_TOGGLE_ERR:
					printk(KERN_INFO "  STATUS:HC_XFER_DATA_TOGGLE_ERR\n");break;
				case HC_XFER_COMPLETE:
					printk(KERN_INFO "  STATUS:HC_XFER_COMPLETE\n");break;
				default:
					printk(KERN_INFO "  STATUS:UNKKNOWN %d\n",urbd->status);break;
			}
		}
	#endif
}

/*!
	 \fn    static void release_channel(ltqhcd_hcd_t          *_ltqhcd,
                            ltqhcd_hc_t           *_ltqhc,
                            ltqhcd_halt_status_e  _halt_status)
	 \brief Release the halted channel.
	 \param _ltqhcd Pointer to the sate of HCD structure
	 \param _ltqhc Pointer to host channel descriptor
	 \param _halt_status Halt satus
	 \return None
	 \ingroup  LTQUSB_HCD
 */
static
void release_channel(ltqhcd_hcd_t          *_ltqhcd,
                            ltqhcd_hc_t           *_ltqhc,
                            ltqhcd_halt_status_e  _halt_status)
{
	ltqusb_hc_regs_t *hc_regs = _ltqhcd->core_if.hc_regs[_ltqhc->hc_num];
	struct urb       *urb     = NULL;
	ltqhcd_epqh_t    *epqh    = NULL;
	ltqhcd_urbd_t    *urbd    = NULL;

	LTQ_DEBUGPL(DBG_HCDV, "  %s: channel %d, halt_status %d\n",
		    __func__, _ltqhc->hc_num, _halt_status);

	epqh=_ltqhc->epqh;
	if(!epqh)
	{
		if(_halt_status!=HC_XFER_NO_EPQH)
			LTQ_ERROR("%s epqh=null\n",__func__);
	}
	urbd=_ltqhc->urbd;
	if(!urbd)
	{
		LTQ_ERROR("%s urbd =null\n",__func__);
	}
	else
	{
		urb=urbd->urb;
		if(!urb)
		{
			if(_halt_status!=HC_XFER_NO_URB)
				LTQ_ERROR("%s urb =null\n",__func__);
		}
		else
		{
			if      (read_data_toggle(hc_regs) == LTQUSB_HCTSIZ_DATA0)
				usb_settoggle (urb->dev,usb_pipeendpoint (urb->pipe), (_ltqhc->is_in)?0:1,0);
			else if (read_data_toggle(hc_regs) == LTQUSB_HCTSIZ_DATA1)
				usb_settoggle (urb->dev,usb_pipeendpoint (urb->pipe), (_ltqhc->is_in)?0:1,1);
		}
	}

	switch (_halt_status)
	{
		case HC_XFER_NO_HALT_STATUS:
			LTQ_ERROR("%s: No halt_status, channel %d\n", __func__, _ltqhc->hc_num);
//			return;
			break;
		case HC_XFER_COMPLETE:
			LTQ_ERROR("%s: Inavalid halt_status HC_XFER_COMPLETE, channel %d\n", __func__, _ltqhc->hc_num);
//			return;
			break;
		case HC_XFER_NO_URB:
			break;
		case HC_XFER_NO_EPQH:
			break;
		case HC_XFER_URB_DEQUEUE:
			if(urbd && urb)
			{
				urbd->phase=URBD_DEQUEUEING;
				ltqhcd_complete_urb(_ltqhcd, urbd, 0);
			}
			else
			{
				LTQ_WARN("WARNING %s():%d urbd=%p urb=%p\n",__func__,__LINE__,urbd,urb);
				release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
			}
			break;
		case HC_XFER_AHB_ERR:
			if(urbd && urb)
			{
				urbd->phase=URBD_DEQUEUEING;
#ifdef NEWRET
//				ltqhcd_complete_urb(_ltqhcd, urbd, _halt_status);
				ltqhcd_complete_urb(_ltqhcd, urbd, -ESHUTDOWN);
#else
				ltqhcd_complete_urb(_ltqhcd, urbd, -EREMOTEIO);
#endif
			}
			else
			{
				LTQ_WARN("WARNING %s():%d urbd=%p urb=%p\n",__func__,__LINE__,urbd,urb);
				release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
			}
			break;
		case HC_XFER_XACT_ERR:
			if(urbd && urb)
			{
				urbd->phase=URBD_DEQUEUEING;
#ifdef NEWRET
//				ltqhcd_complete_urb(_ltqhcd, urbd, _halt_status);
				ltqhcd_complete_urb(_ltqhcd, urbd, -EPROTO);
#else
				ltqhcd_complete_urb(_ltqhcd, urbd, -EREMOTEIO);
#endif
			}
			else
			{
				LTQ_WARN("WARNING %s():%d urbd=%p urb=%p\n",__func__,__LINE__,urbd,urb);
				release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
			}
			break;
		case HC_XFER_FRAME_OVERRUN:
			if(urbd && urb)
			{
				urbd->phase=URBD_DEQUEUEING;
#ifdef NEWRET
//				ltqhcd_complete_urb(_ltqhcd, urbd, _halt_status);
				ltqhcd_complete_urb(_ltqhcd, urbd, -EPROTO);
#else
				ltqhcd_complete_urb(_ltqhcd, urbd, -EREMOTEIO);
#endif
			}
			else
			{
				LTQ_WARN("WARNING %s():%d urbd=%p urb=%p\n",__func__,__LINE__,urbd,urb);
				release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
			}
			break;
		case HC_XFER_URB_COMPLETE:
			_ltqhc->cancelling=0;
			if(urbd && urb)
			{
				urbd->phase=URBD_COMPLETING;
				ltqhcd_complete_urb(_ltqhcd, urbd, urbd->status);
			}
			else
			{
				LTQ_WARN("WARNING %s():%d urbd=%p urb=%p\n",__func__,__LINE__,urbd,urb);
				release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
			}
			break;
		case HC_XFER_STALL:
			if(urbd)
			{
				urbd->phase=URBD_DEQUEUEING;
				ltqhcd_complete_urb(_ltqhcd, urbd, -EPIPE);
			}
			else
			{
				LTQ_WARN("WARNING %s():%d urbd=%p urb=%p\n",__func__,__LINE__,urbd,urb);
				release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
			}
			if(epqh && urb && urb->dev && urb->pipe)
				usb_settoggle(urb->dev, usb_pipeendpoint (urb->pipe), !usb_pipein(urb->pipe), LTQUSB_HC_PID_DATA0);
			break;
		case HC_XFER_BABBLE_ERR:
			if(urbd)
			{
				urbd->phase=URBD_DEQUEUEING;
				ltqhcd_complete_urb(_ltqhcd, urbd, -EOVERFLOW);
			}
			else
			{
				LTQ_WARN("WARNING %s():%d urbd=%p urb=%p\n",__func__,__LINE__,urbd,urb);
				release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
			}
			break;
		case HC_XFER_DATA_TOGGLE_ERR:
			if(urbd)
			{
				urbd->phase=URBD_DEQUEUEING;
				ltqhcd_complete_urb(_ltqhcd, urbd, -EOVERFLOW);
			}
			else
			{
				LTQ_WARN("WARNING %s():%d urbd=%p urb=%p\n",__func__,__LINE__,urbd,urb);
				release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
			}
			break;
		case HC_XFER_STOPPED:
			_ltqhc->cancelling=0;
			if (_ltqhc->is_in)
			{
				if(urbd && urb)
				{
					urbd->phase=URBD_COMPLETING;
					ltqhcd_complete_urb(_ltqhcd, urbd, 0);
				}
				else
				{
					LTQ_WARN("WARNING %s():%d urbd=%p urb=%p\n",__func__,__LINE__,urbd,urb);
					release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
				}
			}
			else
			{
				LTQ_WARN("WARNING %s():%d urbd=%p urb=%p\n",__func__,__LINE__,urbd,urb);
				release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
			}
			break;
	#if defined(__INTRNAKRETRY__) || defined(__INTRINCRETRY__)
		case HC_XFER_INTR_NAK_RETRY:
			if(epqh)
				epqh->phase=EPQH_READY;
			if(urbd)
				urbd->phase=URBD_IDLE;
			_ltqhc->cancelling=0;
			urbd->hc=0;
			ltqhcd_hc_cleanup(&_ltqhcd->core_if, _ltqhc);
			select_eps(_ltqhcd);
			return;
			break;

	#endif
	}
	if(epqh)
	{
		ltqhcd_epqh_idle(epqh);
	}
	else if(_halt_status!=HC_XFER_NO_EPQH)
	{
		LTQ_WARN("WARNING %s():%d epqh=%p\n",__func__,__LINE__,epqh);
		release_channel_dump(_ltqhc,urb,epqh,urbd,_halt_status);
	}

	if(_ltqhcd->burst)
	if(epqh)
	if(epqh->ep_type==LTQUSB_EP_TYPE_BULK)
	if(epqh->burst_on && _halt_status== HC_XFER_URB_COMPLETE && !list_empty(&epqh->urbd_list))
	{
		struct list_head *urbd_ptr;
		#ifdef USE_SAFE_URBD
			struct list_head *urbd_ptr_next;
		#endif

		#ifdef __DYN_SOF_INTR__
			_ltqhcd->dyn_sof_count = DYN_SOF_COUNT_DEF;
		#endif

		LOCK_EPQH_LIST(_ltqhcd);
		LOCK_URBD_LIST(epqh);
		#ifdef USE_SAFE_URBD
		list_for_each_safe(urbd_ptr, urbd_ptr_next, &epqh->urbd_list)
		#else //USE_SAFE_URBD
		urbd_ptr       = epqh->urbd_list.next;
		while (urbd_ptr != &epqh->urbd_list)
		#endif //USE_SAFE_URBD
		{
			urbd = list_entry(urbd_ptr, ltqhcd_urbd_t, ql);
			#ifndef USE_SAFE_URBD
				urbd_ptr=urbd_ptr->next;
			#endif //USE_SAFE_URBD

			if(urbd->phase==URBD_ACTIVE || urbd->phase==URBD_RUNNING)
				break;
			if(urbd->phase==URBD_IDLE)
			{
				urbd->hc=_ltqhc;
				_ltqhc->urbd=urbd;
				#ifdef __EPQD_DESTROY_TIMEOUT__
					del_timer(&epqh->destroy_timer);
				#endif

				_ltqhc->halt_status = HC_XFER_NO_HALT_STATUS;

			#if defined(__UNALIGNED_BUF_ADJ__)
				if(urbd->aligned_buf)
					_ltqhc->xfer_buff = urbd->aligned_buf;
				else
			#endif
					_ltqhc->xfer_buff = urbd->xfer_buff;
				_ltqhc->xfer_len       = urbd->xfer_len;
				_ltqhc->xfer_count     = 0;
				_ltqhc->sof_delay      = 0;
				if(urb)
				{
					if(usb_gettoggle (urb->dev,usb_pipeendpoint (urb->pipe), (_ltqhc->is_in)?0:1))
						_ltqhc->data_pid_start = LTQUSB_HCTSIZ_DATA1;
					else
						_ltqhc->data_pid_start = LTQUSB_HCTSIZ_DATA0;
				}
				if(_ltqhc->is_in)
					_ltqhc->short_rw       =0;
				else if(urb)
					_ltqhc->short_rw       =(urb->transfer_flags & URB_ZERO_PACKET)?1:0;
//				epqh->do_ping=1;
				{
					hcint_data_t      hc_intr_mask;

					/* Clear old interrupt conditions for this host channel. */
					hc_intr_mask.d32 = 0xFFFFFFFF;
					hc_intr_mask.b.reserved = 0;
					ltqusb_wreg(&hc_regs->hcint, hc_intr_mask.d32);

					/* Enable channel interrupts required for this transfer. */
					hc_intr_mask.d32 = 0;
					hc_intr_mask.b.chhltd = 1;
					hc_intr_mask.b.ahberr = 1;

					ltqusb_wreg(&hc_regs->hcintmsk, hc_intr_mask.d32);

					/* Enable the top level host channel interrupt. */
					{
						uint32_t          intr_enable;
						intr_enable = (1 << _ltqhc->hc_num);
						ltqusb_mreg(&_ltqhcd->core_if.host_global_regs->haintmsk, 0, intr_enable);
					}

					/* Make sure host channel interrupts are enabled. */
					{
						gint_data_t       gintmsk ={.d32 = 0};
						gintmsk.b.hcintr = 1;
						ltqusb_mreg(&_ltqhcd->core_if.core_global_regs->gintmsk, 0, gintmsk.d32);
					}
				}

				epqh->phase=EPQH_ACTIVE;
//				urbd->phase=URBD_ACTIVE;
				ltqhcd_hc_start(_ltqhcd, _ltqhc,1);
				UNLOCK_URBD_LIST(epqh);
				UNLOCK_EPQH_LIST(_ltqhcd);
				select_eps(_ltqhcd);
				return;
			}
		}
		UNLOCK_URBD_LIST(epqh);
		UNLOCK_EPQH_LIST(_ltqhcd);
	}
	urbd->hc=0;
	ltqhcd_hc_cleanup(&_ltqhcd->core_if, _ltqhc);
	select_eps(_ltqhcd);
}

/*
 * Updates the state of the URB after a Transfer Complete interrupt on the
 * host channel. Updates the actual_length field of the URB based on the
 * number of bytes transferred via the host channel. Sets the URB status
 * if the data transfer is finished.
 *
 * @return 1 if the data transfer specified by the URB is completely finished,
 * 0 otherwise.
 */
static
int update_urb_state_xfer_comp(ltqhcd_hc_t       *_ltqhc,
                                      ltqusb_hc_regs_t  *_hc_regs,
                                      struct urb        *_urb,
                                      ltqhcd_urbd_t      *_urbd)
{
	int xfer_done  = 0;

	#ifdef __EN_ISOC__
	if(_urbd->epqh->ep_type==LTQUSB_EP_TYPE_ISOC)
	{
		struct usb_iso_packet_descriptor *frame_desc;
		frame_desc            = &_urb->iso_frame_desc[_urbd->isoc_frame_index];
		if (_ltqhc->is_in)
		{
			hctsiz_data_t hctsiz;
			hctsiz.d32 = ltqusb_rreg(&_hc_regs->hctsiz);
			frame_desc->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
			if ((hctsiz.b.xfersize != 0) || (frame_desc->actual_length >= _urbd->xfer_len))
			{
				xfer_done = 1;
				frame_desc->status = 0;
				#if 0
					if (frame_desc->actual_length < frame_desc->length && _urb->transfer_flags & URB_SHORT_NOT_OK)
						frame_desc->status = -EREMOTEIO;
				#endif
			}
		}
		else
		{
			if (_ltqhc->split)
				frame_desc->actual_length +=  _ltqhc->ssplit_out_xfer_count;
			else
				frame_desc->actual_length +=  _ltqhc->xfer_len;
			if (frame_desc->actual_length >= _urbd->xfer_len)
			{
				xfer_done = 1;
				frame_desc->status = 0;
			}
		}
	}
	else
	#endif
	if (_ltqhc->is_in)
	{
		hctsiz_data_t hctsiz;
		hctsiz.d32 = ltqusb_rreg(&_hc_regs->hctsiz);
		_urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
#ifdef __INTRINCRETRY__
		if(_urbd->epqh->ep_type==LTQUSB_EP_TYPE_INTR)
		{
			if(_ltqhc->xfer_len != hctsiz.b.xfersize)
			{
				xfer_done = 1;
				_urbd->status = 0;
			}
		}
		else
#endif
		if (((hctsiz.b.xfersize% _ltqhc->mps) != 0) || (_urb->actual_length >= _urb->transfer_buffer_length))
		{
			xfer_done = 1;
			_urbd->status = 0;
			if(_urb->transfer_flags & URB_SHORT_NOT_OK)
			{
				if (_urb->actual_length < _urb->transfer_buffer_length)
					_urbd->status = -EREMOTEIO;
			}
		}
		else if(_ltqhc->start_pkt_count>hctsiz.b.pktcnt) // hctsiz.b.xfersize% _ltqhc->mps) == 0
		{
			int pkt_xfer1,pkt_xfer2;
			pkt_xfer1=_ltqhc->start_pkt_count-hctsiz.b.pktcnt;
			pkt_xfer2=(_ltqhc->xfer_len - hctsiz.b.xfersize)/ _ltqhc->mps;
			if(pkt_xfer1>pkt_xfer2)
			{
				xfer_done = 1;
				_urbd->status = 0;
				if(_urb->transfer_flags & URB_SHORT_NOT_OK)
				{
					if (_urb->actual_length < _urb->transfer_buffer_length)
						_urbd->status = -EREMOTEIO;
				}
			}
		}
	}
	else if(_urb->transfer_buffer_length%_ltqhc->mps) // OUT without ZLP
	{
		if (_ltqhc->split)
			_urb->actual_length +=  _ltqhc->ssplit_out_xfer_count;
		else
			_urb->actual_length +=  _ltqhc->xfer_len;
		if (_urb->actual_length >= _urb->transfer_buffer_length)
		{
			xfer_done = 1;
			_urbd->status = 0;
		}
	}
	else if (_urb->actual_length >= _urb->transfer_buffer_length) //OUT with ZLP
	{
		xfer_done = 1;
		_urbd->status = 0;
	}
	else //OUT without ZLP, unfinished
	{
		if (_ltqhc->split)
			_urb->actual_length +=  _ltqhc->ssplit_out_xfer_count;
		else
			_urb->actual_length +=  _ltqhc->xfer_len;
		if (!_ltqhc->short_rw && _urb->actual_length >= _urb->transfer_buffer_length)
		{
			xfer_done = 1;
			_urbd->status = 0;
		}
	}

	#ifdef __DEBUG__
		{
			hctsiz_data_t 	hctsiz;
			hctsiz.d32 = ltqusb_rreg(&_hc_regs->hctsiz);
			LTQ_DEBUGPL(DBG_HCDV, "LTQUSB: %s: %s, channel %d\n",
				    __func__, (_ltqhc->is_in ? "IN" : "OUT"), _ltqhc->hc_num);
			LTQ_DEBUGPL(DBG_HCDV, "  hc->xfer_len %d\n", _ltqhc->xfer_len);
			LTQ_DEBUGPL(DBG_HCDV, "  hctsiz.xfersize %d\n", hctsiz.b.xfersize);
			#ifdef __EN_ISOC__
			if(_urbd->epqh->ep_type==LTQUSB_EP_TYPE_ISOC)
			{
				LTQ_DEBUGPL(DBG_HCDV, "  descritor # %d\n", _urbd->isoc_frame_index);
				LTQ_DEBUGPL(DBG_HCDV, "  buffer_length %d\n",
					_urb->iso_frame_desc[_urbd->isoc_frame_index].length);
				LTQ_DEBUGPL(DBG_HCDV, "  actual_length %d\n", _urb->iso_frame_desc[_urbd->isoc_frame_index].actual_length);
			}
			else
			#endif
			{
				LTQ_DEBUGPL(DBG_HCDV, "  urb->transfer_buffer_length %d\n",
					    _urb->transfer_buffer_length);
				LTQ_DEBUGPL(DBG_HCDV, "  urb->actual_length %d\n", _urb->actual_length);
			}
		}
	#endif

	return xfer_done;
}

#ifdef __EN_ISOC__
	static
	void next_isoc_sub(unsigned long data)
	{
		ltqhcd_urbd_t *urbd;
		ltqhcd_hcd_t *ltqhcd;

		urbd=((ltqhcd_urbd_t *)data);
		ltqhcd=urbd->epqh->ltqhcd;

		if (!urbd->epqh)
			LTQ_ERROR("%s: invalid epqd\n",__func__);
		#if   defined(__UNALIGNED_BUF_ADJ__)
		else
		{
			if( urbd->aligned_checked   &&
			    urbd->xfer_buff &&
			    urbd->is_in)
			{
				uint8_t *buf;

				buf=urbd->xfer_buff;
				buf+=urbd->urb->iso_frame_desc[urbd->isoc_frame_index].offset;
				memcpy(buf,urbd->aligned_buf,urbd->urb->iso_frame_desc[urbd->isoc_frame_index].length);
			}
		}
		#endif

		urbd->isoc_frame_index++;
		if(urbd->isoc_frame_index>=urbd->urb->number_of_packets)
			release_channel(ltqhcd,urbd->hc,HC_XFER_URB_COMPLETE);
		else
			init_hc(urbd->epqh,urbd->hc);
	}
#endif

/*!
	 \fn    static void complete_channel(ltqhcd_hcd_t        *_ltqhcd,
                            ltqhcd_hc_t          *_ltqhc,
                            ltqhcd_urbd_t        *_urbd)
	 \brief Complete the transaction on the channel.
	 \param _ltqhcd Pointer to the sate of HCD structure
	 \param _ltqhc Pointer to host channel descriptor
	 \param _urbd Pointer to URB descriptor
	 \return None
	 \ingroup  LTQUSB_HCD
 */
static
void complete_channel(ltqhcd_hcd_t        *_ltqhcd,
                            ltqhcd_hc_t          *_ltqhc,
                            ltqhcd_urbd_t        *_urbd)
{
	ltqusb_hc_regs_t *hc_regs = _ltqhcd->core_if.hc_regs[_ltqhc->hc_num];
	struct urb    *urb  = NULL;
	ltqhcd_epqh_t *epqh = NULL;
	int urb_xfer_done;

	LTQ_DEBUGPL(DBG_HCD, "--Complete Channel %d : \n", _ltqhc->hc_num);

	if(!_urbd)
	{
		LTQ_ERROR("ERROR %s():%d urbd=%p\n",__func__,__LINE__,_urbd);
		return;
	}

	urb  = _urbd->urb;
	epqh = _urbd->epqh;

	if(!epqh)
	{
		release_channel(_ltqhcd,_ltqhc,HC_XFER_NO_EPQH);
		return;
	}
	if(!urb || (unsigned long)urb->hcpriv!=(unsigned long)_urbd)
	{
		release_channel(_ltqhcd,_ltqhc,HC_XFER_NO_URB);
		return;
	}

	if (_ltqhc->split)
		_ltqhc->split = 1;

	switch (epqh->ep_type)
	{
		case LTQUSB_EP_TYPE_CTRL:
			switch (_ltqhc->control_phase)
			{
				case LTQHCD_CONTROL_SETUP:
					if (_urbd->xfer_len > 0)
					{
						_ltqhc->control_phase = LTQHCD_CONTROL_DATA;
						LTQ_DEBUGPL(DBG_HCDV, "  Control setup transaction done Data Stage now\n");
						_ltqhc->is_in         = _urbd->is_in;
						_ltqhc->xfer_len      = _urbd->xfer_len;
						#if   defined(__UNALIGNED_BUF_ADJ__)
							if(_urbd->aligned_buf)
								_ltqhc->xfer_buff      = _urbd->aligned_buf;
							else
						#endif
								_ltqhc->xfer_buff      = _urbd->xfer_buff;
					}
					else
					{
						LTQ_DEBUGPL(DBG_HCDV, "  Control setup transaction done Status Stage now\n");
						_ltqhc->control_phase = LTQHCD_CONTROL_STATUS;
						_ltqhc->is_in          = 1;
						_ltqhc->xfer_len       = 0;
						_ltqhc->xfer_buff      = _ltqhcd->status_buf;
					}
					if(_ltqhc->is_in)
						_ltqhc->short_rw       =0;
					else
						_ltqhc->short_rw       =(urb->transfer_flags & URB_ZERO_PACKET)?1:0;
					_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA1;
					_ltqhc->xfer_count     = 0;
					_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
					ltqhcd_hc_start(_ltqhcd, _ltqhc, 1);
					break;
				case LTQHCD_CONTROL_DATA:
					urb_xfer_done = update_urb_state_xfer_comp(_ltqhc, hc_regs, urb, _urbd);
					if (urb_xfer_done)
					{
#if 1
if(_ltqhc->is_in)
if(_urbd->urb->actual_length==18)
{
	__le16 vid,pid;
	vid=le16_to_cpup((__le16 *)(_urbd->xfer_buff+8 ));
	pid=le16_to_cpup((__le16 *)(_urbd->xfer_buff+10));

	switch(vid)
	{
		case 0X1C9E:
			switch(pid)
			{
				case 0X9B03:
				case 0X9B05:
					_ltqhcd->singlexact=1;
					_ltqhcd->xfer_prot1_usec= 100;
					_ltqhcd->xfer_prot2_usec= 0;
					_ltqhcd->xfer_prot1_value= XFER_PROT1_CALC(_ltqhcd->xfer_prot1_usec);
					_ltqhcd->xfer_prot2_value= XFER_PROT2_CALC(_ltqhcd->xfer_prot2_usec);
					printk(KERN_INFO "ID:%04x/%04x LC\n",vid,pid);
					break;
				default:
					printk(KERN_INFO "ID:%04x/%04X\n",vid,pid);
			}
			break;
		default:
			printk(KERN_INFO "ID:%04x/%04X\n",vid,pid);
	}
}
#endif
						_ltqhc->control_phase  = LTQHCD_CONTROL_STATUS;
						LTQ_DEBUGPL(DBG_HCDV, "  Control data transaction done Status Stage now\n");
						_ltqhc->is_in          = (_urbd->is_in)?0:1;
						_ltqhc->xfer_len       = 0;
						_ltqhc->xfer_count     = 0;
						_ltqhc->xfer_buff      = _ltqhcd->status_buf;
						_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
						_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA1;
						if(_ltqhc->is_in)
							_ltqhc->short_rw       =0;
						else
							_ltqhc->short_rw       =1;
					}
					else // continue
					{
						LTQ_DEBUGPL(DBG_HCDV, "  Control data transaction continue\n");
						_ltqhc->xfer_len       = _urbd->xfer_len - urb->actual_length;
						_ltqhc->xfer_count     = urb->actual_length;
						_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
						_ltqhc->data_pid_start = read_data_toggle(hc_regs);
					}
					ltqhcd_hc_start(_ltqhcd, _ltqhc, 1);
					break;
				case LTQHCD_CONTROL_STATUS:
					LTQ_DEBUGPL(DBG_HCDV, "  Control status transaction done\n");
					if (_urbd->status == -EINPROGRESS)
						_urbd->status = 0;
					release_channel(_ltqhcd,_ltqhc,HC_XFER_URB_COMPLETE);
					break;
			}
			break;
		case LTQUSB_EP_TYPE_BULK:
			LTQ_DEBUGPL(DBG_HCDV, "  Bulk transfer complete\n");
			urb_xfer_done = update_urb_state_xfer_comp(_ltqhc, hc_regs, urb, _urbd);
			if (urb_xfer_done)
			{
				release_channel(_ltqhcd,_ltqhc,HC_XFER_URB_COMPLETE);
			}
			else
			{
				_ltqhc->xfer_len       = _urbd->xfer_len - urb->actual_length;
				_ltqhc->xfer_count     = urb->actual_length;
				_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
				_ltqhc->data_pid_start = read_data_toggle(hc_regs);
				ltqhcd_hc_start(_ltqhcd, _ltqhc, 1);
			}
			break;
		case LTQUSB_EP_TYPE_INTR:
			urb_xfer_done = update_urb_state_xfer_comp(_ltqhc, hc_regs, urb, _urbd);

			#ifdef __INTRINCRETRY__
			if(!urb_xfer_done)
			{
				release_channel(_ltqhcd,_ltqhc,HC_XFER_INTR_NAK_RETRY);
			}
			else
			#endif
			{
				release_channel(_ltqhcd,_ltqhc,HC_XFER_URB_COMPLETE);
			}
			break;
		case LTQUSB_EP_TYPE_ISOC:
			#ifdef __EN_ISOC__
				urb_xfer_done = update_urb_state_xfer_comp(_ltqhc, hc_regs, urb, _urbd);
				if (urb_xfer_done)
				{
					#if   defined(__UNALIGNED_BUF_ADJ__)
					if(in_irq())
					{
						if(!epqh->tasklet_next_isoc.func)
						{
							epqh->tasklet_next_isoc.next = NULL;
							epqh->tasklet_next_isoc.state = 0;
							atomic_set( &epqh->tasklet_next_isoc.count, 0);
							epqh->tasklet_next_isoc.func = next_isoc_sub;
							epqh->tasklet_next_isoc.data = (unsigned long)_urbd;
						}
						tasklet_schedule(&epqh->tasklet_next_isoc);
					}
					else
					#endif
					{
						next_isoc_sub((unsigned long)_urbd);
					}
				}
				else
				{
					struct usb_iso_packet_descriptor *frame_desc;
					frame_desc            = &urb->iso_frame_desc[_urbd->isoc_frame_index];
					_ltqhc->xfer_len       = _urbd->xfer_len - frame_desc->actual_length;
					_ltqhc->xfer_count     = frame_desc->actual_length;
					_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
					_ltqhc->data_pid_start = read_data_toggle(hc_regs);
					ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
				}
			#endif
			break;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_ctrl_rx_nonsplit(ltqhcd_hcd_t      *_ltqhcd,
                                        ltqhcd_hc_t       *_ltqhc,
                                        ltqusb_hc_regs_t  *_hc_regs,
                                        ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);

	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if (_ltqhc->halt_status == HC_XFER_STOPPED)
	{
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
		{
			u32 actual_length;
			actual_length = _urbd->urb->actual_length + (_ltqhc->xfer_len - hctsiz.b.xfersize);
			if(
			   (_urbd->xfer_len && actual_length>=_urbd->xfer_len)
			   || (hctsiz.b.xfersize % _ltqhc->mps)>0
			  )
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else if(_ltqhc->start_pkt_count>hctsiz.b.pktcnt) // hctsiz.b.xfersize% _ltqhc->mps) == 0
			{
				int pkt_xfer1,pkt_xfer2;
				pkt_xfer1=_ltqhc->start_pkt_count-hctsiz.b.pktcnt;
				pkt_xfer2=(_ltqhc->xfer_len - hctsiz.b.xfersize)/ _ltqhc->mps;
				if(pkt_xfer1>pkt_xfer2)
				{
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				}
				else
				{
					_ltqhc->xfer_count        =
					_urbd->urb->actual_length = actual_length;
					_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
					_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
					_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
					ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
				}
			}
			else
			{
				_ltqhc->xfer_count        =
				_urbd->urb->actual_length = actual_length;
				_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
				_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
				_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
				ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
			}
		}
		else if(_ltqhc->control_phase == LTQHCD_CONTROL_STATUS)
		{
			if(hctsiz.b.pktcnt==0)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else
			{
				_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
				ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
			}
		}
		else
		{
			printk(KERN_INFO "Warning: %s() %d Invalid CTRL Phase:%d\n",__func__,__LINE__,_ltqhc->control_phase);
			release_channel(_ltqhcd, _ltqhc, _ltqhc->halt_status);
		}
		return 1;
	}

	if (hcint.b.xfercomp || hcint.d32 == 0x02 || hcint.d32 == 0x22)
	{
		_urbd->error_count     =0;
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else if (hcint.b.stall)
	{
		_urbd->error_count     =0;
		// ZLP shortcut
		#if 0
		if(hctsiz.b.pktcnt==0)
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		else
		#endif
		#if 0
		if(_ltqhc->control_phase == LTQHCD_CONTROL_STATUS)
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		else
		#endif
		{
			if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
				_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
//			if( _urbd->urb->actual_length > _ltqhc->xfer_len) _urbd->urb->actual_length = _urbd->xfer_len;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		}
		return 1;
	}
	else if (hcint.b.bblerr)
	{
		_urbd->error_count     =0;

		// ZLP shortcut
		#if 0
		if(hctsiz.b.pktcnt==0)
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		else
		#endif
		#if 0
		if(_ltqhc->control_phase == LTQHCD_CONTROL_STATUS)
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		else
		#endif
		{
			if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
				_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
//			if( _urbd->urb->actual_length > _ltqhc->xfer_len) _urbd->urb->actual_length = _urbd->xfer_len;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		}
		return 1;
	}
	else if (hcint.b.xacterr)
	{
		// ZLP shortcut
		#if 1
		if(hctsiz.b.pktcnt==0)
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		#endif
		#if 1
		if(_ltqhc->control_phase == LTQHCD_CONTROL_STATUS)
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		#endif
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
		{
			#if 1
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			#else
				u32 actual_length;
				actual_length = _urbd->urb->actual_length + (_ltqhc->xfer_len - hctsiz.b.xfersize);
				if(actual_length >= _urbd->xfer_len)
				{
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				}
				else
				{
					_urbd->error_count++;
					_ltqhc->xfer_count        =
					_urbd->urb->actual_length = actual_length;
					_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
					_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
					if (_urbd->error_count >= 3)
					{
						_urbd->error_count     =0;
						release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
					}
					else
					{
						_ltqhc->erron=1;
						ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
					}
				}
			#endif
		}
		else
		{
			_urbd->error_count     =0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		#if 0
			#if 1
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			#else
				u32 actual_length;
				actual_length = _urbd->urb->actual_length + (_ltqhc->xfer_len - hctsiz.b.xfersize);
				if(actual_length>=_urbd->xfer_len)
				{
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				}
				else
				{
					_urbd->urb->actual_length = actual_length;
					_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
					_urbd->error_count     =0;
					release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
				}
			#endif
		#else
			if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
				_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
//			if( _urbd->urb->actual_length > _ltqhc->xfer_len) _urbd->urb->actual_length = _urbd->xfer_len;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		#endif
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
			_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
//		if( _urbd->urb->actual_length > _ltqhc->xfer_len) _urbd->urb->actual_length = _urbd->xfer_len;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else
	{
		_urbd->error_count     =0;
		LTQ_ERROR("ERROR %s():%d invalid chhlt condition %08X/%08X  %d\n",__func__,__LINE__,hcint.d32,hcintmsk.d32,_ltqhc->halt_status);
		release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		return 1;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_ctrl_tx_nonsplit(ltqhcd_hcd_t      *_ltqhcd,
                                        ltqhcd_hc_t       *_ltqhc,
                                        ltqusb_hc_regs_t  *_hc_regs,
                                        ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);

	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if (_ltqhc->halt_status == HC_XFER_STOPPED)
	{
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
		{
			u32 actual_length;
			actual_length = _urbd->urb->actual_length + ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);

			if(_urbd->xfer_len && actual_length >= _urbd->xfer_len)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else
			{
				_ltqhc->xfer_count        =
				_urbd->urb->actual_length = actual_length;
				_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
				_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
				_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
				_ltqhc->epqh->do_ping=1;
				ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
			}
		}
		else if(_ltqhc->control_phase == LTQHCD_CONTROL_STATUS)
		{
			if(hctsiz.b.pktcnt==0)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else
			{
				_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
				ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
			}
		}
		else
		{
			printk(KERN_INFO "Warning: %s() %d Invalid CTRL Phase:%d\n",__func__,__LINE__,_ltqhc->control_phase);
			release_channel(_ltqhcd, _ltqhc, _ltqhc->halt_status);
		}
		return 1;
	}


	if (hcint.b.xfercomp || hcint.d32 == 0x02 || hcint.d32 == 0x22)
	{
		_urbd->error_count     =0;
		if(_ltqhc->xfer_len==0 && !hcint.b.ack && hcint.b.nak)
		{
			// Walkaround: When sending ZLP and receive NAK but also issue CMPT intr
			// Solution:   NoSplit: Resend at next SOF
			//             Split  : Resend at next SOF with SSPLIT
			if(hcint.b.nyet)
				_ltqhc->epqh->do_ping=1;

			_ltqhc->xfer_len       = 0;
			_ltqhc->xfer_count     = 0;
			_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		else
		{
			if(hcint.b.nyet)
				_ltqhc->epqh->do_ping=1;
			#if 1
			if(hctsiz.b.pktcnt==0)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else
			#endif
			if(_ltqhc->control_phase == LTQHCD_CONTROL_STATUS)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else if(_ltqhc->control_phase == LTQHCD_CONTROL_SETUP)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
			{
				#if 0
					_ltqhc->epqh->do_ping=1;
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				#else
					u32 actual_length;
					_ltqhc->epqh->do_ping=1;
					actual_length = _urbd->urb->actual_length + ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
					if(actual_length>=_urbd->xfer_len)
					{
						_urbd->error_count     =0;
						complete_channel(_ltqhcd, _ltqhc, _urbd);
					}
					else
					{
						_ltqhc->xfer_count        =
						_urbd->urb->actual_length = actual_length;
						_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
						_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
						_ltqhc->erron=1;
						_ltqhc->epqh->do_ping=1;
						ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
					}
				#endif
			}
		}
		return 1;
	}
	else if (hcint.b.stall)
	{
		_urbd->error_count     =0;

		// ZLP shortcut
		#if 1
		if(hctsiz.b.pktcnt==0)
		{
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		#endif
		{
			if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
				_urbd->urb->actual_length += ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
			release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		}
		return 1;
	}
	else if (hcint.b.xacterr)
	{
		// ZLP shortcut
		#if 1
		if(hctsiz.b.pktcnt==0)
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		#endif
		if(_ltqhc->control_phase == LTQHCD_CONTROL_STATUS)
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else if(_ltqhc->control_phase == LTQHCD_CONTROL_SETUP)
		{
			_urbd->error_count     =0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
		{
			#if 0
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			#else
				u32 actual_length;
				actual_length = _urbd->urb->actual_length + ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
				if(actual_length>=_urbd->xfer_len)
				{
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				}
				else
				{
					_urbd->error_count++;
					_ltqhc->xfer_count        =
					_urbd->urb->actual_length = actual_length;
					_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
					_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
					if (_urbd->error_count >= 3)
					{
						_urbd->error_count     =0;
						release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
					}
					else
					{
						_ltqhc->erron=1;
						_ltqhc->epqh->do_ping=1;
						ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
					}
				}
			#endif
		}
		else
		{
			_urbd->error_count     =0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
			_urbd->urb->actual_length += ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.nak || hcint.b.nyet)
	{
		#if 1
			_urbd->error_count     =0;
			LTQ_ERROR("ERROR %s():%d invalid chhlt condition\n",__func__,__LINE__);
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		#else
			// ZLP shortcut
			#if 1
			if(hctsiz.b.pktcnt==0)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else
			#endif
			if(_ltqhc->control_phase == LTQHCD_CONTROL_STATUS)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else if(_ltqhc->control_phase == LTQHCD_CONTROL_SETUP)
			{
				_urbd->error_count     =0;
				LTQ_ERROR("ERROR %s():%d invalid chhlt condition\n",__func__,__LINE__);
				release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
			}
			else if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
			{
				#if 0
					_ltqhc->epqh->do_ping=1;
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				#else
					u32 actual_length;
					_ltqhc->epqh->do_ping=1;
					actual_length = _urbd->urb->actual_length + ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
					if(actual_length>=_urbd->xfer_len)
					{
						_urbd->error_count     =0;
						complete_channel(_ltqhcd, _ltqhc, _urbd);
					}
					else
					{
						_ltqhc->xfer_count        =
						_urbd->urb->actual_length = actual_length;
						_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
						_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
						_ltqhc->erron=1;
						_ltqhc->epqh->do_ping=1;
						ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
					}
				#endif
			}
		#endif
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
		{
			_urbd->urb->actual_length += ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
//			if( _urbd->urb->actual_length > _ltqhc->xfer_len) _urbd->urb->actual_length = _urbd->xfer_len;
		}
		_urbd->error_count     =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
		{
			_urbd->urb->actual_length += ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
//			if( _urbd->urb->actual_length > _ltqhc->xfer_len) _urbd->urb->actual_length = _urbd->xfer_len;
		}
		_urbd->error_count     =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else
	{
		_urbd->error_count     =0;
		LTQ_ERROR("ERROR %s():%d invalid chhlt condition %08X/%08X  %d\n",__func__,__LINE__,hcint.d32,hcintmsk.d32,_ltqhc->halt_status);
		release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_bulk_rx_nonsplit(ltqhcd_hcd_t      *_ltqhcd,
                                        ltqhcd_hc_t       *_ltqhc,
                                        ltqusb_hc_regs_t  *_hc_regs,
                                        ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);

	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if(_ltqhc->halt_status == HC_XFER_STOPPED)
	{
		u32 actual_length;
		actual_length = _urbd->urb->actual_length + (_ltqhc->xfer_len - hctsiz.b.xfersize);

		if(
		   (_urbd->xfer_len && actual_length>=_urbd->xfer_len)
		   || hctsiz.b.pktcnt==0
		   || (hctsiz.b.xfersize % _ltqhc->mps)>0
		  )
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else if(_ltqhc->start_pkt_count>hctsiz.b.pktcnt) // hctsiz.b.xfersize% _ltqhc->mps) == 0
		{
			int pkt_xfer1,pkt_xfer2;
			pkt_xfer1=_ltqhc->start_pkt_count-hctsiz.b.pktcnt;
			pkt_xfer2=(_ltqhc->xfer_len - hctsiz.b.xfersize)/ _ltqhc->mps;
			if(pkt_xfer1>pkt_xfer2)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else
			{
				_urbd->urb->actual_length = actual_length;
				_ltqhc->xfer_len          = _urbd->xfer_len - _urbd->urb->actual_length;
				_ltqhc->xfer_count        = _urbd->urb->actual_length;
				_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
				_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
				ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
			}
		}
		else
		{
			_urbd->urb->actual_length = actual_length;
			_ltqhc->xfer_len          = _urbd->xfer_len - _urbd->urb->actual_length;
			_ltqhc->xfer_count        = _urbd->urb->actual_length;
			_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
			_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}

	hcint.b.ack=0;
	hcint.b.nak=0;

	if (hcint.b.xfercomp || hcint.d32 == 0x02 || hcint.d32 == 0x22)
	{
		_urbd->error_count     =0;
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else if (hcint.b.stall)
	{
		_urbd->error_count     =0;
		// ZLP shortcut
		#if 0
		if(hctsiz.b.pktcnt==0)
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		else
		#endif
		{
			_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
			release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		}
		return 1;
	}
	else if (hcint.b.bblerr)
	{
		_urbd->error_count     =0;

		// ZLP shortcut
		#if 0
		if(hctsiz.b.pktcnt==0)
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		else
		#endif
		{
			_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
			release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		}
		return 1;
	}
	else if (hcint.b.xacterr)
	{
		// ZLP shortcut
		#if 1
		if(hctsiz.b.pktcnt==0)
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		#endif
		{
			#if 0
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			#else
				u32 actual_length;
				actual_length = _urbd->urb->actual_length + (_ltqhc->xfer_len - hctsiz.b.xfersize);
				if(actual_length >= _urbd->xfer_len)
				{
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				}
				else
				{
					_urbd->error_count++;
					_ltqhc->xfer_count        =
					_urbd->urb->actual_length = actual_length;
					_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
					_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
					if (_urbd->error_count >= 3)
					{
						_urbd->error_count     =0;
						release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
					}
					else
					{
						_ltqhc->erron=1;
						ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
					}
				}
			#endif
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		#if 0
			#if 1
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			#else
				u32 actual_length;
				actual_length = _urbd->urb->actual_length + (_ltqhc->xfer_len - hctsiz.b.xfersize);
				if(actual_length >= _urbd->xfer_len)
				{
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				}
				else
				{
					_urbd->urb->actual_length = actual_length;
					_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
					_urbd->error_count     =0;
					release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
				}
			#endif
		#else
			_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
//			if( _urbd->urb->actual_length > _ltqhc->xfer_len) _urbd->urb->actual_length = _urbd->xfer_len;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		#endif
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
//		if( _urbd->urb->actual_length > _ltqhc->xfer_len) _urbd->urb->actual_length = _urbd->xfer_len;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else
	{
		_urbd->error_count     =0;
		LTQ_ERROR("ERROR %s():%d invalid chhlt condition %08X/%08X %d sz:%d/%d/%d/%d\n",__func__,__LINE__,hcint.d32,hcintmsk.d32,_ltqhc->halt_status , hctsiz.b.xfersize, _ltqhc->xfer_len-_ltqhc->xfer_len,_ltqhc->xfer_len,_urbd->xfer_len);
		release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_bulk_tx_nonsplit(ltqhcd_hcd_t      *_ltqhcd,
                                        ltqhcd_hc_t       *_ltqhc,
                                        ltqusb_hc_regs_t  *_hc_regs,
                                        ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;
	int out_nak_enh = 0;

	if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
		out_nak_enh = 1;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if (_ltqhc->halt_status == HC_XFER_STOPPED)
	{
		u32 actual_length;
		actual_length = _urbd->urb->actual_length + ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);

		if(_urbd->xfer_len && actual_length >= _urbd->xfer_len)
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		{
			_ltqhc->xfer_count        =
			_urbd->urb->actual_length = actual_length;
			_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
			_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
			_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
			_ltqhc->epqh->do_ping=1;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}

	if (hcint.b.xfercomp || hcint.d32 == 0x02 || hcint.d32 == 0x22 )
	{
		_urbd->error_count     =0;
		if(_ltqhc->xfer_len==0 && !hcint.b.ack && hcint.b.nak)
		{
			// Walkaround: When sending ZLP and receive NAK but also issue CMPT intr
			// Solution:   NoSplit: Resend at next SOF
			//             Split  : Resend at next SOF with SSPLIT
			if(hcint.b.nyet)
				_ltqhc->epqh->do_ping=1;

			_ltqhc->xfer_len       = 0;
			_ltqhc->xfer_count     = 0;
			_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		else
		{
			if(hcint.b.nyet)
				_ltqhc->epqh->do_ping=1;
			#if 1
			if(hctsiz.b.pktcnt==0)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else
			#endif
			{
				#if 0
					_ltqhc->epqh->do_ping=1;
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				#else
					u32 actual_length;
					_ltqhc->epqh->do_ping=1;
					actual_length = _urbd->urb->actual_length + ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
					if(actual_length>=_urbd->xfer_len)
					{
						_urbd->error_count     =0;
						complete_channel(_ltqhcd, _ltqhc, _urbd);
					}
					else
					{
						_ltqhc->xfer_count        =
						_urbd->urb->actual_length = actual_length;
						_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
						_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
						_ltqhc->erron=1;
						_ltqhc->epqh->do_ping=1;
						ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
					}
				#endif
			}
		}
		return 1;
	}
	else if (hcint.b.stall)
	{
		_urbd->error_count     =0;

		// ZLP shortcut
		#if 1
		if(hctsiz.b.pktcnt==0)
		{
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		#endif
		{
			_urbd->urb->actual_length += ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
			if(_urbd->urb->actual_length>_urbd->xfer_len) _urbd->urb->actual_length=_urbd->xfer_len;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		}
		return 1;
	}
	else if (hcint.b.xacterr)
	{
		// ZLP shortcut
		#if 1
		if(hctsiz.b.pktcnt==0)
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		#endif
		{
			#if 0
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			#else
				u32 actual_length;
				actual_length = _urbd->urb->actual_length + ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
				if(actual_length >= _urbd->xfer_len)
				{
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				}
				else
				{
					_urbd->error_count++;
					_ltqhc->xfer_count        =
					_urbd->urb->actual_length = actual_length;
					_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
					_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
					if (_urbd->error_count >= 3)
					{
						_urbd->error_count     =0;
						release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
					}
					else
					{
						_ltqhc->erron=1;
						_ltqhc->epqh->do_ping=1;
						ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
					}
				}
			#endif
		}
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->urb->actual_length += ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
		if(_urbd->urb->actual_length>_urbd->xfer_len) _urbd->urb->actual_length=_urbd->xfer_len;
		LTQ_ERROR("ERROR %s():%d invalid packet babble\n",__func__,__LINE__);
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.nak || hcint.b.nyet)
	{
		#if 1
			_urbd->error_count     =0;
			LTQ_ERROR("ERROR %s():%d invalid chhlt condition\n",__func__,__LINE__);
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		#else
			// ZLP shortcut
			#if 1
			if(hctsiz.b.pktcnt==0)
			{
				_urbd->error_count     =0;
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			}
			else
			#endif
			{
				#if 0
					_ltqhc->epqh->do_ping=1;
					_urbd->error_count     =0;
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				#else
					u32 actual_length;
					_ltqhc->epqh->do_ping=1;
					actual_length = _urbd->urb->actual_length + ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
					if(actual_length>=_urbd->xfer_len)
					{
						_urbd->error_count     =0;
						complete_channel(_ltqhcd, _ltqhc, _urbd);
					}
					else
					{
						_ltqhc->xfer_count        =
						_urbd->urb->actual_length = actual_length;
						_ltqhc->xfer_len          = _urbd->xfer_len - actual_length;
						_ltqhc->data_pid_start    = read_data_toggle(_hc_regs);
						_ltqhc->erron=1;
						_ltqhc->epqh->do_ping=1;
						ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
					}
				#endif
			}
		#endif
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		_urbd->urb->actual_length += ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
//		if( _urbd->urb->actual_length > _ltqhc->xfer_len) _urbd->urb->actual_length = _urbd->xfer_len;
		_urbd->error_count     =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		_urbd->urb->actual_length += ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
//		if( _urbd->urb->actual_length > _ltqhc->xfer_len) _urbd->urb->actual_length = _urbd->xfer_len;
		_urbd->error_count     =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else
	{
		_urbd->error_count     =0;
		LTQ_ERROR("ERROR %s():%d invalid chhlt condition %08X/%08X  %d\n",__func__,__LINE__,hcint.d32,hcintmsk.d32,_ltqhc->halt_status);
		release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_intr_rx_nonsplit(ltqhcd_hcd_t      *_ltqhcd,
                                    ltqhcd_hc_t       *_ltqhc,
                                    ltqusb_hc_regs_t  *_hc_regs,
                                    ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if (hcint.b.xfercomp || hcint.d32 == 0x02 || hcint.d32 == 0x22)
	{
		_urbd->error_count   =0;
		//restart INTR immediately
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else if (hcint.b.stall)
	{
		_urbd->error_count   =0;

		// Don't care shortcut
		#if 0
		if(hctsiz.b.pktcnt==0)
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		else
		#endif
		{
			_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
			release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		}
		return 1;
	}
	else if (hcint.b.bblerr)
	{
		_urbd->error_count   =0;

		// Don't care shortcut
		#if 0
		if(hctsiz.b.pktcnt==0)
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		else
		#endif
		{
			_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
			release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		}
		return 1;
	}
	else if (hcint.b.datatglerr)
	{
		_urbd->error_count   =0;
		//restart INTR immediately
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else if (hcint.b.frmovrun)
	{
#if 0
		_urbd->error_count   =0;
		//restart INTR immediately
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
#else
		#if 1
		if(hctsiz.b.pktcnt==0)
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		#endif
		{
			_urbd->error_count++;
			if(_urbd->error_count>=3)
			{
				_urbd->error_count     =0;
			}
	//		complete_channel(_ltqhcd, _ltqhc, _urbd);
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
#endif
	}
	else if (hcint.b.xacterr)
	{
		// ZLP shortcut
		#if 1
		if(hctsiz.b.pktcnt==0)
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		#endif
		{
			_urbd->error_count++;
			if(_urbd->error_count>=3)
			{
				_urbd->error_count     =0;
				release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
			}
			else
			{
				ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
			}
		}
		return 1;
	}
	else if(hcint.b.nyet   )
	{
		return 1;
	}
	else if (hcint.b.nak)
	{
		#ifdef __INTRNAKRETRY__
		if(hctsiz.b.pktcnt)
		{
			release_channel(_ltqhcd, _ltqhc, HC_XFER_INTR_NAK_RETRY);
			return 1;
		}
		#endif
		_urbd->error_count   =0;
		//restart INTR immediately
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else
	{
		_urbd->error_count   =0;
		//restart INTR immediately
		#if 0
		if(hctsiz.b.pktcnt>0)
		{
			// TODO Re-initialize Channel (in next b_interval - 1 uF/F)
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		else
		#endif
		{
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		return 1;
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_intr_tx_nonsplit(ltqhcd_hcd_t      *_ltqhcd,
                                    ltqhcd_hc_t       *_ltqhc,
                                    ltqusb_hc_regs_t  *_hc_regs,
                                    ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;
	int out_nak_enh = 0;

	if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
		out_nak_enh = 1;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);

	if (hcint.b.xfercomp || hcint.d32 == 0x02 || hcint.d32 == 0x22)
	{
		disable_hc_int(_hc_regs,ack);
		disable_hc_int(_hc_regs,nak);
		disable_hc_int(_hc_regs,nyet);
		_urbd->error_count   =0;
		//restart INTR immediately
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else if (hcint.b.stall)
	{
		disable_hc_int(_hc_regs,ack);
		disable_hc_int(_hc_regs,nyet);
		disable_hc_int(_hc_regs,nak);
		_urbd->error_count   =0;

		// Don't care shortcut
		#if 0
		if(hctsiz.b.pktcnt==0)
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		else
		#endif
		{
			if(_ltqhc->xfer_len!=0)// !_ltqhc->is_in
				_urbd->urb->actual_length += ((_ltqhc->start_pkt_count - hctsiz.b.pktcnt ) * _ltqhc->mps);
			release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		}
		return 1;
	}
	else if(hcint.b.nak || hcint.b.frmovrun )
	{
		disable_hc_int(_hc_regs,ack);
		disable_hc_int(_hc_regs,nyet);
		disable_hc_int(_hc_regs,nak);
		_urbd->error_count   =0;
		//restart INTR immediately
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else if(hcint.b.xacterr    )
	{
		// ZLP shortcut
		#if 1
		if(hctsiz.b.pktcnt==0)
		{
			_urbd->error_count     =0;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		else
		#endif
		{
			_urbd->error_count++;
			if(_urbd->error_count>=3)
			{
				_urbd->error_count     =0;
				release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
			}
			else
			{
				ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
			}
		}
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->error_count     =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		_urbd->error_count     =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_isoc_rx_nonsplit(ltqhcd_hcd_t      *_ltqhcd,
                                    ltqhcd_hc_t       *_ltqhc,
                                    ltqusb_hc_regs_t  *_hc_regs,
                                    ltqhcd_urbd_t     *_urbd)
{
	#ifdef __EN_ISOC__
		hcint_data_t  hcint;
		hcint_data_t  hcintmsk;
		hctsiz_data_t hctsiz;

		hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
		hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
		hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);

		if (hcint.b.xfercomp || hcint.b.frmovrun || hcint.d32 == 0x02)
		{
			_urbd->error_count=0;
			disable_hc_int(_hc_regs,ack);
			disable_hc_int(_hc_regs,nak);
			disable_hc_int(_hc_regs,nyet);
			if (hcint.b.xfercomp)
				complete_channel(_ltqhcd, _ltqhc, _urbd);
			else
				release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		}
		else if (hcint.b.xacterr || hcint.b.bblerr)
		{
			#ifndef VR9Skip
				if(hctsiz.b.pktcnt==0)
				{
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				}
				else
				{
					_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
					_ltqhc->xfer_len           = _urbd->xfer_len - _urbd->urb->actual_length;
					_ltqhc->xfer_count         = _urbd->urb->actual_length;
					_ltqhc->data_pid_start = read_data_toggle(_hc_regs);
					_urbd->error_count++;
					if(_urbd->error_count>=3)
					{
						_urbd->error_count=0;
						if (hcint.b.bblerr)
							release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
						else if (hcint.b.xacterr)
							release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
					}
					else
					{
						enable_hc_int(_hc_regs,ack);
						enable_hc_int(_hc_regs,nak);
						enable_hc_int(_hc_regs,nyet);
						ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
					}
				}
			#endif
		}
		else if(hcint.b.datatglerr )
		{
			return 1;
		}
		else if(hcint.b.stall      )
		{
			return 1;
		}
	#endif
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_isoc_tx_nonsplit(ltqhcd_hcd_t      *_ltqhcd,
                                    ltqhcd_hc_t       *_ltqhc,
                                    ltqusb_hc_regs_t  *_hc_regs,
                                    ltqhcd_urbd_t     *_urbd)
{
	#ifdef __EN_ISOC__
		hcint_data_t  hcint;
		hcint_data_t  hcintmsk;
		hctsiz_data_t hctsiz;
		int out_nak_enh = 0;

		if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
			out_nak_enh = 1;

		hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
		hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
		hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);

		if (hcint.b.xfercomp || hcint.d32 == 0x02)
		{
			_urbd->error_count=0;
			disable_hc_int(_hc_regs,ack);
			disable_hc_int(_hc_regs,nak);
			disable_hc_int(_hc_regs,nyet);
			complete_channel(_ltqhcd, _ltqhc, _urbd);
			return 1;
		}
		else if (hcint.b.frmovrun)
		{
			#ifndef VR9Skip
				_urbd->error_count=0;
				disable_hc_int(_hc_regs,ack);
				disable_hc_int(_hc_regs,nak);
				disable_hc_int(_hc_regs,nyet);
				release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
			#endif
		}
		else if(hcint.b.datatglerr )
		{
			return 1;
		}
		else if(hcint.b.bblerr     )
		{
			#ifndef VR9Skip
				if(hctsiz.b.pktcnt==0)
				{
					complete_channel(_ltqhcd, _ltqhc, _urbd);
				}
				else
				{
					_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
					_ltqhc->xfer_len           = _urbd->xfer_len - _urbd->urb->actual_length;
					_ltqhc->xfer_count         = _urbd->urb->actual_length;
					_ltqhc->data_pid_start = read_data_toggle(_hc_regs);
					_urbd->error_count++;
					if(_urbd->error_count>=3)
					{
						_urbd->error_count=0;
						release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
					}
					else
					{
						enable_hc_int(_hc_regs,ack);
						enable_hc_int(_hc_regs,nak);
						enable_hc_int(_hc_regs,nyet);
						ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
					}
				}
			#endif
		}
		else if(hcint.b.xacterr    )
		{
			if(hctsiz.b.pktcnt==0)
			{
				complete_channel(_ltqhcd, _ltqhc, _urbd);
				return 1;
			}
			_urbd->error_count++;
			if(_urbd->error_count>=3)
			{
				_urbd->error_count=0;
				release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
			}
			else
			{
				ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
			}
			return 1;
		}
		else if(hcint.b.stall      )
		{
			return 1;
		}
	#endif
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_ctrl_rx_ssplit(ltqhcd_hcd_t      *_ltqhcd,
                                      ltqhcd_hc_t       *_ltqhc,
                                      ltqusb_hc_regs_t  *_hc_regs,
                                      ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);

	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if (hcint.b.ack)
	{
		_urbd->error_count=0;
		_ltqhc->split=2;
		_ltqhc->data_pid_start = read_data_toggle(_hc_regs);
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if (hcint.b.nak)
	{
		_urbd->error_count     = 0;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if (hcint.b.xacterr)
	{
		_urbd->error_count++;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.stall      )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else if(hcint.b.nyet   )
	{
	}
	else if(hcint.b.xfercomp   )
	{
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_ctrl_tx_ssplit(ltqhcd_hcd_t      *_ltqhcd,
                                      ltqhcd_hc_t       *_ltqhc,
                                      ltqusb_hc_regs_t  *_hc_regs,
                                      ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;
	int out_nak_enh = 0;

	if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
		out_nak_enh = 1;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if     (hcint.b.ack )
	{
		_urbd->error_count=0;
		if (_ltqhc->control_phase != LTQHCD_CONTROL_SETUP)
			_ltqhc->ssplit_out_xfer_count = _ltqhc->xfer_len;
		_ltqhc->split=2;
		_ltqhc->data_pid_start =read_data_toggle(_hc_regs);
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nyet)
	{
		_urbd->error_count=0;
		if (_ltqhc->control_phase != LTQHCD_CONTROL_SETUP)
			_ltqhc->ssplit_out_xfer_count = _ltqhc->xfer_len;
		_ltqhc->split=2;
		_ltqhc->data_pid_start =read_data_toggle(_hc_regs);
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nak        )
	{
		_urbd->error_count    =0;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.xacterr    )
	{
		_urbd->error_count++;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.stall      )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else if(hcint.b.xfercomp   )
	{
		printk(KERN_INFO "Warning: %s() %d CTRL OUT SPLIT1 COMPLETE\n",__func__,__LINE__);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_bulk_rx_ssplit(ltqhcd_hcd_t              *_ltqhcd,
                                      ltqhcd_hc_t       *_ltqhc,
                                      ltqusb_hc_regs_t  *_hc_regs,
                                      ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);

	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if (hcint.b.ack)
	{
		_urbd->error_count=0;
		_ltqhc->split=2;
		_ltqhc->data_pid_start = read_data_toggle(_hc_regs);
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if (hcint.b.nak)
	{
		_urbd->error_count     = 0;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if (hcint.b.xacterr)
	{
		_urbd->error_count++;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.stall      )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else if(hcint.b.nyet   )
	{
	}
	else if(hcint.b.xfercomp   )
	{
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_bulk_tx_ssplit(ltqhcd_hcd_t      *_ltqhcd,
                                      ltqhcd_hc_t       *_ltqhc,
                                      ltqusb_hc_regs_t  *_hc_regs,
                                      ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;
	int out_nak_enh = 0;

	if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
		out_nak_enh = 1;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if     (hcint.b.ack )
	{
		_urbd->error_count=0;
		_ltqhc->ssplit_out_xfer_count = _ltqhc->xfer_len;
		_ltqhc->split=2;
		_ltqhc->data_pid_start =read_data_toggle(_hc_regs);
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nyet)
	{
		_urbd->error_count=0;
		_ltqhc->ssplit_out_xfer_count = _ltqhc->xfer_len;
		_ltqhc->split=2;
		_ltqhc->data_pid_start =read_data_toggle(_hc_regs);
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nak        )
	{
		_urbd->error_count    =0;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.xacterr    )
	{
		_urbd->error_count++;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.stall      )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else if(hcint.b.xfercomp   )
	{
		printk(KERN_INFO "Warning: %s() %d BULK OUT SPLIT1 COMPLETE\n",__func__,__LINE__);
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_intr_rx_ssplit(ltqhcd_hcd_t      *_ltqhcd,
                                  ltqhcd_hc_t       *_ltqhc,
                                  ltqusb_hc_regs_t  *_hc_regs,
                                  ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);

	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if     (hcint.b.ack)
	{
		_urbd->error_count=0;
		_ltqhc->split=2;
		_ltqhc->data_pid_start = read_data_toggle(_hc_regs);
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nak)
	{
		_urbd->error_count=0;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.xacterr)
	{
		hcchar_data_t 	hcchar;
		hcchar.d32 = ltqusb_rreg(&_hc_regs->hcchar);
		_urbd->error_count=hcchar.b.multicnt;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.stall      )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		return 1;
	}
	else if(hcint.b.xfercomp   )
	{
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_intr_tx_ssplit(ltqhcd_hcd_t      *_ltqhcd,
                                  ltqhcd_hc_t       *_ltqhc,
                                  ltqusb_hc_regs_t  *_hc_regs,
                                  ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;
	int out_nak_enh = 0;

	if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
		out_nak_enh = 1;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);

	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if     (hcint.b.ack )
	{
		_urbd->error_count=0;
		_ltqhc->ssplit_out_xfer_count = _ltqhc->xfer_len;
		_ltqhc->split=2;
		_ltqhc->data_pid_start = read_data_toggle(_hc_regs);
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nyet)
	{
		_urbd->error_count=0;
		_ltqhc->ssplit_out_xfer_count = _ltqhc->xfer_len;
		_ltqhc->split=2;
		_ltqhc->data_pid_start = read_data_toggle(_hc_regs);
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nak        )
	{
		_urbd->error_count   =0;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		_urbd->error_count   =0;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.xacterr    )
	{
		hcchar_data_t 	hcchar;
		hcchar.d32 = ltqusb_rreg(&_hc_regs->hcchar);
		_urbd->error_count=hcchar.b.multicnt;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			enable_hc_int(_hc_regs,ack);
			enable_hc_int(_hc_regs,nak);
			enable_hc_int(_hc_regs,nyet);
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_DATA_TOGGLE_ERR);
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.stall      )
	{
		_urbd->error_count   =0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		return 1;
	}
	else if(hcint.b.xfercomp   )
	{
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_isoc_rx_ssplit(ltqhcd_hcd_t      *_ltqhcd,
                                   ltqhcd_hc_t       *_ltqhc,
                                   ltqusb_hc_regs_t  *_hc_regs,
                                   ltqhcd_urbd_t     *_urbd)
{
	#if defined(__EN_ISOC__) && defined(__EN_ISOC_SPLIT__)
		hcint_data_t  hcint;
		hcint_data_t  hcintmsk;
		hctsiz_data_t hctsiz;

		hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
		hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
		hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
		if     (hcint.b.ack )
		{
			Do Complete Split
		}
		else if(hcint.b.frmovrun   )
		{
			Rewind Buffer Pointers
			Retry Start Split (in next b_interval ¡V 1 uF)
		}
		else if(hcint.b.datatglerr )
		{
			//warning
		}
		else if(hcint.b.bblerr     )
		{
			//warning
		}
		else if(hcint.b.xacterr    )
		{
			//warning
		}
		else if(hcint.b.stall      )
		{
			//warning
		}
		else if(hcint.b.nak        )
		{
			//warning
		}
		else if(hcint.b.xfercomp   )
		{
			//warning
		}
		else if(hcint.b.nyet)
		{
			//warning
		}
	#endif
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_isoc_tx_ssplit(ltqhcd_hcd_t      *_ltqhcd,
                                   ltqhcd_hc_t       *_ltqhc,
                                   ltqusb_hc_regs_t  *_hc_regs,
                                   ltqhcd_urbd_t     *_urbd)
{
	#if defined(__EN_ISOC__) && defined(__EN_ISOC_SPLIT__)
		hcint_data_t  hcint;
		hcint_data_t  hcintmsk;
		hctsiz_data_t hctsiz;
		int out_nak_enh = 0;

		if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
			out_nak_enh = 1;

		hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
		hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
		hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
		if     (hcint.b.ack )
		{
			//Do Next Start Split (in next b_interval ¡V 1 uF)
		}
		else if(hcint.b.frmovrun   )
		{
			//Do Next Transaction in next frame.
		}
		else if(hcint.b.datatglerr )
		{
			//warning
		}
		else if(hcint.b.bblerr     )
		{
			//warning
		}
		else if(hcint.b.xacterr    )
		{
			//warning
		}
		else if(hcint.b.stall      )
		{
			//warning
		}
		else if(hcint.b.nak        )
		{
			//warning
		}
		else if(hcint.b.xfercomp   )
		{
			//warning
		}
		else if(hcint.b.nyet)
		{
			//warning
		}
	#endif
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_ctrl_rx_csplit(ltqhcd_hcd_t      *_ltqhcd,
                                      ltqhcd_hc_t       *_ltqhc,
                                      ltqusb_hc_regs_t  *_hc_regs,
                                      ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if (hcint.b.xfercomp)
	{
		_urbd->error_count   =0;
		_ltqhc->split=1;
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else if (hcint.b.nak)
	{
		_ltqhc->split          = 1;
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
		{
			_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
			_ltqhc->xfer_count     = _urbd->urb->actual_length;
		}
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nyet)
	{
		_urbd->error_count=0;
		_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.stall || hcint.b.bblerr )
	{
		_urbd->error_count=0;
		if     (hcint.b.stall)
			release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		else if(hcint.b.bblerr )
			release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.xacterr    )
	{
		_urbd->error_count++;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			_ltqhc->split=1;
			if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
			{
				_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
				_ltqhc->xfer_count     = _urbd->urb->actual_length;
			}
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		if(_ltqhc->data_pid_start == LTQUSB_HC_PID_DATA0)
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA1;
		else
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA0;
		_ltqhc->split=1;
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
		{
			_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
			_ltqhc->xfer_count     = _urbd->urb->actual_length;
		}
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		_urbd->error_count=0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_ctrl_tx_csplit(ltqhcd_hcd_t      *_ltqhcd,
                                      ltqhcd_hc_t       *_ltqhc,
                                      ltqusb_hc_regs_t  *_hc_regs,
                                      ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;
	int out_nak_enh = 0;

	if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
		out_nak_enh = 1;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if(hcint.b.xfercomp   )
	{
		_urbd->error_count=0;
		_ltqhc->split=1;
		#if 0
		if(_ltqhc->xfer_len==0 && !hcint.b.ack && (hcint.b.nak || hcint.b.nyet))
		{
			// Walkaround: When sending ZLP and receive NYEY or NAK but also issue CMPT intr
			// Solution:   NoSplit: Resend at next SOF
			//             Split  : Resend at next SOF with SSPLIT
			_ltqhc->xfer_len       = 0;
			_ltqhc->xfer_count     = 0;
			_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		else
		#endif
		{
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		return 1;
	}
	else if(hcint.b.nak        )
	{
		_ltqhc->split          = 1;
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
		{
			_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
			_ltqhc->xfer_count     = _urbd->urb->actual_length;
		}
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nyet)
	{
		//Retry Complete Split
		// Issue Retry instantly on next SOF, without gothrough process_channels
		_urbd->error_count=0;
		_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.stall      )
	{
		_urbd->error_count=0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		return 1;
	}
	else if(hcint.b.xacterr    )
	{
		_urbd->error_count++;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			_ltqhc->split=1;
			if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
			{
				_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
				_ltqhc->xfer_count     = _urbd->urb->actual_length;
			}
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		if(_ltqhc->data_pid_start == LTQUSB_HC_PID_DATA0)
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA1;
		else
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA0;
		_ltqhc->split=1;
		if(_ltqhc->control_phase == LTQHCD_CONTROL_DATA)
		{
			_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
			_ltqhc->xfer_count     = _urbd->urb->actual_length;
		}
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		_urbd->error_count=0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->error_count=0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_bulk_rx_csplit(ltqhcd_hcd_t      *_ltqhcd,
                                      ltqhcd_hc_t       *_ltqhc,
                                      ltqusb_hc_regs_t  *_hc_regs,
                                      ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if (hcint.b.xfercomp)
	{
		_urbd->error_count   =0;
		_ltqhc->split=1;
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else if (hcint.b.nak)
	{
		_ltqhc->split          = 1;
		_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
		_ltqhc->xfer_count     = _urbd->urb->actual_length;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nyet)
	{
		_urbd->error_count=0;
		_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.stall || hcint.b.bblerr )
	{
		_urbd->error_count=0;
		if     (hcint.b.stall)
			release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		else if(hcint.b.bblerr )
			release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	else if(hcint.b.xacterr    )
	{
		_urbd->error_count++;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			_ltqhc->split=1;
			_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
			_ltqhc->xfer_count     = _urbd->urb->actual_length;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		if(_ltqhc->data_pid_start == LTQUSB_HC_PID_DATA0)
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA1;
		else
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA0;
		_ltqhc->split=1;
		_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
		_ltqhc->xfer_count     = _urbd->urb->actual_length;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		_urbd->error_count=0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_bulk_tx_csplit(ltqhcd_hcd_t      *_ltqhcd,
                                      ltqhcd_hc_t       *_ltqhc,
                                      ltqusb_hc_regs_t  *_hc_regs,
                                      ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;
	int out_nak_enh = 0;

	if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
		out_nak_enh = 1;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if(hcint.b.xfercomp   )
	{
		_urbd->error_count=0;
		_ltqhc->split=1;
		#if 0
		if(_ltqhc->xfer_len==0 && !hcint.b.ack && (hcint.b.nak || hcint.b.nyet))
		{
			// Walkaround: When sending ZLP and receive NYEY or NAK but also issue CMPT intr
			// Solution:   NoSplit: Resend at next SOF
			//             Split  : Resend at next SOF with SSPLIT
			_ltqhc->xfer_len       = 0;
			_ltqhc->xfer_count     = 0;
			_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		else
		#endif
		{
			complete_channel(_ltqhcd, _ltqhc, _urbd);
		}
		return 1;
	}
	else if(hcint.b.nak        )
	{
		_ltqhc->split          = 1;
		_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
		_ltqhc->xfer_count     = _urbd->urb->actual_length;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nyet)
	{
		//Retry Complete Split
		// Issue Retry instantly on next SOF, without gothrough process_channels
		_urbd->error_count=0;
		_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.stall      )
	{
		_urbd->error_count=0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		return 1;
	}
	else if(hcint.b.xacterr    )
	{
		_urbd->error_count++;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			_ltqhc->split=1;
			_ltqhc->epqh->do_ping=1;
			_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
			_ltqhc->xfer_count     = _urbd->urb->actual_length;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		if(_ltqhc->data_pid_start == LTQUSB_HC_PID_DATA0)
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA1;
		else
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA0;
		_ltqhc->split=1;
		_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
		_ltqhc->xfer_count     = _urbd->urb->actual_length;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.frmovrun   )
	{
		_urbd->error_count=0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->error_count=0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_intr_rx_csplit(ltqhcd_hcd_t      *_ltqhcd,
                                  ltqhcd_hc_t       *_ltqhc,
                                  ltqusb_hc_regs_t  *_hc_regs,
                                  ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if (hcint.b.xfercomp   )
	{
		_urbd->error_count=0;
		_ltqhc->split=1;
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else if(hcint.b.nak        )
	{
		_ltqhc->split          = 1;
		_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
		_ltqhc->xfer_count     = _urbd->urb->actual_length;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nyet)
	{
		_urbd->error_count=0;
		_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.frmovrun || hcint.b.bblerr || hcint.b.stall )
	{
		_urbd->error_count=0;
		if     (hcint.b.stall)
			release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		else if(hcint.b.bblerr )
			release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		else if(hcint.b.frmovrun )
			release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else if(hcint.b.xacterr    )
	{
		hcchar_data_t 	hcchar;
		hcchar.d32 = ltqusb_rreg(&_hc_regs->hcchar);
		_urbd->error_count=hcchar.b.multicnt;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			_ltqhc->split=1;
			_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
			_ltqhc->xfer_count     = _urbd->urb->actual_length;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		if(_ltqhc->data_pid_start == LTQUSB_HC_PID_DATA0)
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA1;
		else
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA0;
		_ltqhc->split=1;
		_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
		_ltqhc->xfer_count     = _urbd->urb->actual_length;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_intr_tx_csplit(ltqhcd_hcd_t      *_ltqhcd,
                                  ltqhcd_hc_t       *_ltqhc,
                                  ltqusb_hc_regs_t  *_hc_regs,
                                  ltqhcd_urbd_t     *_urbd)
{
	hcint_data_t  hcint;
	hcint_data_t  hcintmsk;
	hctsiz_data_t hctsiz;
	int out_nak_enh = 0;

	if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
		out_nak_enh = 1;

	hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
	hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
	hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);

	if(hcint.b.xfercomp   )
	{
		_urbd->error_count=0;
		_ltqhc->split=1;
		complete_channel(_ltqhcd, _ltqhc, _urbd);
		return 1;
	}
	else if(hcint.b.nak        )
	{
		_ltqhc->split          = 1;
		_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
		_ltqhc->xfer_count     = _urbd->urb->actual_length;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.nyet)
	{
		_urbd->error_count=0;
		_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.stall || hcint.b.frmovrun)
	{
		_urbd->error_count=0;
		if     (hcint.b.stall)
			release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
		else if(hcint.b.frmovrun )
			release_channel(_ltqhcd, _ltqhc, HC_XFER_FRAME_OVERRUN);
		return 1;
	}
	else if(hcint.b.xacterr    )
	{
		hcchar_data_t 	hcchar;
		hcchar.d32 = ltqusb_rreg(&_hc_regs->hcchar);
		_urbd->error_count=hcchar.b.multicnt;
		if(_urbd->error_count>=3)
		{
			_urbd->error_count=0;
			release_channel(_ltqhcd, _ltqhc, HC_XFER_XACT_ERR);
		}
		else
		{
			_ltqhc->split=1;
			_ltqhc->epqh->do_ping=1;
			_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
			_ltqhc->xfer_count     = _urbd->urb->actual_length;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		}
		return 1;
	}
	else if(hcint.b.datatglerr )
	{
		if(_ltqhc->data_pid_start == LTQUSB_HC_PID_DATA0)
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA1;
		else
			_ltqhc->data_pid_start = LTQUSB_HC_PID_DATA0;
		_ltqhc->split=1;
		_ltqhc->epqh->do_ping=1;
		_ltqhc->xfer_len       = _urbd->xfer_len - _urbd->urb->actual_length;
		_ltqhc->xfer_count     = _urbd->urb->actual_length;
		ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
		return 1;
	}
	else if(hcint.b.bblerr     )
	{
		_urbd->error_count=0;
		release_channel(_ltqhcd, _ltqhc, HC_XFER_BABBLE_ERR);
		return 1;
	}
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_isoc_rx_csplit(ltqhcd_hcd_t      *_ltqhcd,
                                   ltqhcd_hc_t       *_ltqhc,
                                   ltqusb_hc_regs_t  *_hc_regs,
                                   ltqhcd_urbd_t     *_urbd)
{
	#if defined(__EN_ISOC__) && defined(__EN_ISOC_SPLIT__)
		hcint_data_t  hcint;
		hcint_data_t  hcintmsk;
		hctsiz_data_t hctsiz;

		hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
		hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
		hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
		if(hcint.b.xfercomp   )
		{
			disable_hc_int(_hc_regs,ack);
			disable_hc_int(_hc_regs,nak);
			disable_hc_int(_hc_regs,nyet);
			_urbd->error_count=0;
			_ltqhc->split=1;
			complete_channel(_ltqhcd, _ltqhc, _urbd);
			return 1;
		}
		else if(hcint.b.nak        )
		{
			Retry Start Split (in next b_interval ¡V 1 uF)
		}
		else if(hcint.b.nyet)
		{
			//Do Next Complete Split
			// Issue Retry instantly on next SOF, without gothrough process_channels
			_urbd->error_count=0;
			//disable_hc_int(_hc_regs,ack);
			//disable_hc_int(_hc_regs,nak);
			//disable_hc_int(_hc_regs,datatglerr);
			_ltqhc->halt_status    = HC_XFER_NO_HALT_STATUS;
			ltqhcd_hc_start(_ltqhcd, _ltqhc, 0);
			return 1;
		}
		else if(hcint.b.frmovrun || hcint.b.stall || hcint.b.bblerr)
		{
			_urbd->error_count=0;
			disable_hc_int(_hc_regs,ack);
			disable_hc_int(_hc_regs,nyet);
			disable_hc_int(_hc_regs,nak);
			_ltqhc->wait_for_sof   = 0;

			//if(hctsiz.b.pktcnt==0)
			//{
			//	complete_channel(_ltqhcd, _ltqhc, _urbd);
			//	return 1;
			//}
			//else
			//	_urbd->urb->actual_length += (_ltqhc->xfer_len - hctsiz.b.xfersize);
			if     (hcint.b.stall)
				release_channel(_ltqhcd, _ltqhc, HC_XFER_STALL);
			else if(hcint.b.frmovrun )
			else if(hcint.b.bblerr )
			return 1;
		}
		else if(hcint.b.xacterr    )
		{
			Rewind Buffer Pointers
			if (HCCHARn.EC = = 3) // ERR response received
			{
				Record ERR error
				Do Next Start Split (in next frame)
			}
			else
			{
				De-allocate Channel
			}
		}
		else if(hcint.b.datatglerr )
		{
			warning
		}
		else if(hcint.b.ack )
		{
			warning
		}
	#endif
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static
int32_t chhltd_isoc_tx_csplit(ltqhcd_hcd_t      *_ltqhcd,
                                   ltqhcd_hc_t       *_ltqhc,
                                   ltqusb_hc_regs_t  *_hc_regs,
                                   ltqhcd_urbd_t     *_urbd)
{
	#if defined(__EN_ISOC__) && defined(__EN_ISOC_SPLIT__)
		hcint_data_t  hcint;
		hcint_data_t  hcintmsk;
		hctsiz_data_t hctsiz;
		int out_nak_enh = 0;

		if (_ltqhcd->core_if.snpsid >= 0x4f54271a && _ltqhc->speed == LTQUSB_EP_SPEED_HIGH)
			out_nak_enh = 1;

		hcint.d32    = ltqusb_rreg(&_hc_regs->hcint);
		hcintmsk.d32 = ltqusb_rreg(&_hc_regs->hcintmsk);
		hctsiz.d32   = ltqusb_rreg(&_hc_regs->hctsiz);
		warning
	#endif
	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*!
	 \fn    static int32_t handle_hc_chhltd_intr(ltqhcd_hcd_t      *_ltqhcd,
                                     ltqhcd_hc_t       *_ltqhc,
                                     ltqusb_hc_regs_t  *_hc_regs,
                                     ltqhcd_urbd_t      *_urbd)
	 \brief This function handles halted interrupts of host channels.
	 \param  _ltqhcd Pointer to the sate of HCD structure
	 \param  _ltqhc Pointer to host channel descriptor
	 \param  _hc_regs Pointer to host channel registers
	 \param  _urbd Pointer to URB descriptor
	 \return  0 OK
	 \ingroup  LTQUSB_HCD
 */
static
int32_t handle_hc_chhltd_intr(ltqhcd_hcd_t      *_ltqhcd,
                                     ltqhcd_hc_t       *_ltqhc,
                                     ltqusb_hc_regs_t  *_hc_regs,
                                     ltqhcd_urbd_t      *_urbd)
{
	LTQ_DEBUGPL(DBG_HCD, "--Host Channel %d Interrupt: Channel Halted--\n", _ltqhc->hc_num);

	_ltqhc->phase = HC_STOPPED;

	if (_ltqhc->halt_status == HC_XFER_URB_DEQUEUE ||
	    _ltqhc->halt_status == HC_XFER_AHB_ERR) {
		/*
		 * Just release the channel. A dequeue can happen on a
		 * transfer timeout. In the case of an AHB Error, the channel
		 * was forced to halt because there's no way to gracefully
		 * recover.
		 */
		if(_ltqhc->urbd)
			_ltqhc->urbd->phase=URBD_DEQUEUEING;
		release_channel(_ltqhcd, _ltqhc, _ltqhc->halt_status);
		return 1;
	}

	if(_ltqhc->urbd)
		_ltqhc->urbd->phase=URBD_ACTIVE;

	if     (_ltqhc->ep_type == LTQUSB_EP_TYPE_CTRL)
	{
		if     (_ltqhc->split==0)
		{
			if(_ltqhc->is_in)
				return (chhltd_ctrl_rx_nonsplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_ctrl_tx_nonsplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
		else if(_ltqhc->split==1)
		{
			if(_ltqhc->is_in)
				return (chhltd_ctrl_rx_ssplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_ctrl_tx_ssplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
		else if(_ltqhc->split==2)
		{
			if(_ltqhc->is_in)
				return (chhltd_ctrl_rx_csplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_ctrl_tx_csplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
	}
	else if(_ltqhc->ep_type == LTQUSB_EP_TYPE_BULK)
	{
		if     (_ltqhc->split==0)
		{
			if(_ltqhc->is_in)
				return (chhltd_bulk_rx_nonsplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_bulk_tx_nonsplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
		else if(_ltqhc->split==1)
		{
			if(_ltqhc->is_in)
				return (chhltd_bulk_rx_ssplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_bulk_tx_ssplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
		else if(_ltqhc->split==2)
		{
			if(_ltqhc->is_in)
				return (chhltd_bulk_rx_csplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_bulk_tx_csplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
	}
	else if(_ltqhc->ep_type == LTQUSB_EP_TYPE_INTR)
	{
		if     (_ltqhc->split==0)
		{
			if(_ltqhc->is_in)
				return (chhltd_intr_rx_nonsplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_intr_tx_nonsplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
		else if(_ltqhc->split==1)
		{
			if(_ltqhc->is_in)
				return (chhltd_intr_rx_ssplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_intr_tx_ssplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
		else if(_ltqhc->split==2)
		{
			if(_ltqhc->is_in)
				return (chhltd_intr_rx_csplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_intr_tx_csplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
	}
	else if(_ltqhc->ep_type == LTQUSB_EP_TYPE_ISOC)
	{
		if     (_ltqhc->split==0)
		{
			if(_ltqhc->is_in)
				return (chhltd_isoc_rx_nonsplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_isoc_tx_nonsplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
		else if(_ltqhc->split==1)
		{
			if(_ltqhc->is_in)
				return (chhltd_isoc_rx_ssplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_isoc_tx_ssplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
		else if(_ltqhc->split==2)
		{
			if(_ltqhc->is_in)
				return (chhltd_isoc_rx_csplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
			else
				return (chhltd_isoc_tx_csplit(_ltqhcd,_ltqhc,_hc_regs,_urbd));
		}
	}
	return 0;
}

/*
 * Handles a host channel AHB error interrupt. This handler is only called in
 * DMA mode.
 */
static
void hc_other_intr_dump(ltqhcd_hcd_t      *_ltqhcd,
                               ltqhcd_hc_t       *_ltqhc,
                               ltqusb_hc_regs_t  *_hc_regs,
                               ltqhcd_urbd_t      *_urbd)
{
	#ifdef __DEBUG__
		hcchar_data_t hcchar;
		hcsplt_data_t hcsplt;
		hctsiz_data_t hctsiz;
		uint32_t      hcdma;
		struct urb   *urb = _urbd->urb;
		hcchar.d32 = ltqusb_rreg(&_hc_regs->hcchar);
		hcsplt.d32 = ltqusb_rreg(&_hc_regs->hcsplt);
		hctsiz.d32 = ltqusb_rreg(&_hc_regs->hctsiz);
		hcdma = ltqusb_rreg(&_hc_regs->hcdma);

		LTQ_ERROR("Channel %d\n", _ltqhc->hc_num);
		LTQ_ERROR("  hcchar 0x%08x, hcsplt 0x%08x\n", hcchar.d32, hcsplt.d32);
		LTQ_ERROR("  hctsiz 0x%08x, hcdma 0x%08x\n", hctsiz.d32, hcdma);
		LTQ_ERROR("  Device address: %d\n", usb_pipedevice(urb->pipe));
		LTQ_ERROR("  Endpoint: %d, %s\n", usb_pipeendpoint(urb->pipe),
			    (usb_pipein(urb->pipe) ? "IN" : "OUT"));
		LTQ_ERROR("  Endpoint type: %s\n",
			    ({char *pipetype;
			    switch (usb_pipetype(urb->pipe)) {
				    case PIPE_CONTROL: pipetype = "CTRL"; break;
				    case PIPE_BULK: pipetype = "BULK"; break;
				    case PIPE_INTERRUPT: pipetype = "INTR"; break;
				    case PIPE_ISOCHRONOUS: pipetype = "ISOC"; break;
				    default: pipetype = "????"; break;
			    }; pipetype;}));
		LTQ_ERROR("  Speed: %s\n",
			    ({char *speed;
			    switch (urb->dev->speed) {
				    case USB_SPEED_HIGH: speed = "HS"; break;
				    case USB_SPEED_FULL: speed = "FS"; break;
				    case USB_SPEED_LOW: speed = "LS"; break;
			    	default: speed = "????"; break;
			    }; speed;}));
		LTQ_ERROR("  Max packet size: %d\n",
			    usb_maxpacket(urb->dev, urb->pipe, usb_pipeout(urb->pipe)));
		LTQ_ERROR("  Data buffer length: %d\n", urb->transfer_buffer_length);
		LTQ_ERROR("  Transfer buffer: %p, Transfer DMA: %p\n",
			    urb->transfer_buffer, (void *)urb->transfer_dma);
		LTQ_ERROR("  Setup buffer: %p, Setup DMA: %p\n",
			    urb->setup_packet, (void *)urb->setup_dma);
		LTQ_ERROR("  Interval: %d\n", urb->interval);
	#endif //__DEBUG__
}

/*
 * Handles a host channel ACK interrupt. This interrupt is enabled when
 *  errors occur, and during Start Split transactions.
 */
static
int32_t handle_hc_ack_intr(ltqhcd_hcd_t      *_ltqhcd,
                                  ltqhcd_hc_t      *_ltqhc,
                                  ltqusb_hc_regs_t *_hc_regs,
                                  ltqhcd_urbd_t     *_urbd)
{
	_urbd->error_count=0;
	_ltqhc->erron = 0;
	clear_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);
	return 1;
}

/*
 * Handles a host channel ACK interrupt. This interrupt is enabled when
 *  errors occur, and during Start Split transactions.
 */
static
int32_t handle_hc_nak_intr(ltqhcd_hcd_t      *_ltqhcd,
                                  ltqhcd_hc_t      *_ltqhc,
                                  ltqusb_hc_regs_t *_hc_regs,
                                  ltqhcd_urbd_t     *_urbd)
{
	_urbd->error_count=0;
	_ltqhc->erron=0;
	disable_hc_int(_hc_regs,nyet);
	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	clear_hc_int(_hc_regs,nak);
	return 1;
}

static
int32_t handle_hc_nyet_intr(ltqhcd_hcd_t      *_ltqhcd,
                                   ltqhcd_hc_t      *_ltqhc,
                                   ltqusb_hc_regs_t *_hc_regs,
                                   ltqhcd_urbd_t     *_urbd)
{
	_urbd->error_count=0;
	_ltqhc->erron = 0;

	disable_hc_int(_hc_regs,ack);
	disable_hc_int(_hc_regs,nak);
	disable_hc_int(_hc_regs,nyet);
	clear_hc_int(_hc_regs,nyet);
	return 1;
}

/*
 * Handles a host channel AHB error interrupt. This handler is only called in
 * DMA mode.
 */
static
int32_t handle_hc_ahberr_intr(ltqhcd_hcd_t      *_ltqhcd,
                                     ltqhcd_hc_t       *_ltqhc,
                                     ltqusb_hc_regs_t  *_hc_regs,
                                     ltqhcd_urbd_t      *_urbd)
{
	LTQ_DEBUGPL(DBG_HCD, "--Host Channel %d Interrupt: "
		    "AHB Error--\n", _ltqhc->hc_num);
	hc_other_intr_dump(_ltqhcd,_ltqhc,_hc_regs,_urbd);

	ltqhcd_hc_halt(&_ltqhcd->core_if, _ltqhc, HC_XFER_AHB_ERR);
	return 1;
}

/*
 * Datatoggle
 */
static int32_t handle_hc_datatglerr_intr(ltqhcd_hcd_t      *_ltqhcd,
                                         ltqhcd_hc_t      *_ltqhc,
                                         ltqusb_hc_regs_t *_hc_regs,
                                         ltqhcd_urbd_t     *_urbd)
{
	LTQ_ERROR( "--Host Channel %d Interrupt: "
		    "DATATOGGLE Error--\n", _ltqhc->hc_num);
	hc_other_intr_dump(_ltqhcd,_ltqhc,_hc_regs,_urbd);
	disable_hc_int(_hc_regs,datatglerr);
	return 1;
}


/*
 * Interrupts which should not been triggered
 */
static
int32_t handle_hc_frmovrun_intr(ltqhcd_hcd_t      *_ltqhcd,
                                       ltqhcd_hc_t      *_ltqhc,
                                       ltqusb_hc_regs_t *_hc_regs,
                                       ltqhcd_urbd_t     *_urbd)
{
	LTQ_ERROR( "--Host Channel %d Interrupt: "
		    "FrameOverRun Error--\n", _ltqhc->hc_num);
	hc_other_intr_dump(_ltqhcd,_ltqhc,_hc_regs,_urbd);
	disable_hc_int(_hc_regs,frmovrun);
	return 1;
}

static
int32_t handle_hc_bblerr_intr(ltqhcd_hcd_t      *_ltqhcd,
                                     ltqhcd_hc_t      *_ltqhc,
                                     ltqusb_hc_regs_t *_hc_regs,
                                     ltqhcd_urbd_t     *_urbd)
{
	LTQ_ERROR( "--Host Channel %d Interrupt: "
		    "BBL Error--\n", _ltqhc->hc_num);
	hc_other_intr_dump(_ltqhcd,_ltqhc,_hc_regs,_urbd);
	disable_hc_int(_hc_regs,bblerr);
	return 1;
}

static
int32_t handle_hc_xacterr_intr(ltqhcd_hcd_t      *_ltqhcd,
                                      ltqhcd_hc_t      *_ltqhc,
                                      ltqusb_hc_regs_t *_hc_regs,
                                      ltqhcd_urbd_t     *_urbd)
{
	LTQ_ERROR( "--Host Channel %d Interrupt: "
		    "XACT Error--\n", _ltqhc->hc_num);
	hc_other_intr_dump(_ltqhcd,_ltqhc,_hc_regs,_urbd);
	disable_hc_int(_hc_regs,xacterr);
	return 1;
}


static
int32_t handle_hc_stall_intr(ltqhcd_hcd_t      *_ltqhcd,
                                    ltqhcd_hc_t      *_ltqhc,
                                    ltqusb_hc_regs_t *_hc_regs,
                                    ltqhcd_urbd_t     *_urbd)
{
	LTQ_ERROR( "--Host Channel %d Interrupt: "
		    "STALL--\n", _ltqhc->hc_num);
	hc_other_intr_dump(_ltqhcd,_ltqhc,_hc_regs,_urbd);
	disable_hc_int(_hc_regs,stall);
	return 1;
}

static
int32_t handle_hc_xfercomp_intr(ltqhcd_hcd_t      *_ltqhcd,
                                       ltqhcd_hc_t      *_ltqhc,
                                       ltqusb_hc_regs_t *_hc_regs,
                                       ltqhcd_urbd_t     *_urbd)
{
	LTQ_ERROR( "--Host Channel %d Interrupt: "
		    "XFERCOMP--\n", _ltqhc->hc_num);
	hc_other_intr_dump(_ltqhcd,_ltqhc,_hc_regs,_urbd);
	disable_hc_int(_hc_regs,xfercomp);
	return 1;
}

/* This interrupt indicates that the specified host channels has a pending
 * interrupt. There are multiple conditions that can cause each host channel
 * interrupt. This function determines which conditions have occurred for this
 * host channel interrupt and handles them appropriately. */
static
int32_t handle_hc_n_intr (ltqhcd_hcd_t *_ltqhcd, uint32_t _num)
{
	uint32_t          hcintval,hcintmsk;
	hcint_data_t      hcint;
	ltqhcd_hc_t      *ltqhc;
	ltqusb_hc_regs_t *hc_regs;
	ltqhcd_urbd_t     *urbd;

	int retval = 0;

	LTQ_DEBUGPL(DBG_HCDV, "--Host Channel Interrupt--, Channel %d\n", _num);

	ltqhc = &_ltqhcd->ltqhc[_num];
	hc_regs = _ltqhcd->core_if.hc_regs[_num];

	ltqhc->stopping=0;

	hcintval  = ltqusb_rreg(&hc_regs->hcint);
	hcintmsk  = ltqusb_rreg(&hc_regs->hcintmsk);
	hcint.d32 = hcintval & hcintmsk;
	LTQ_DEBUGPL(DBG_HCDV, "  0x%08x & 0x%08x = 0x%08x\n",
		    hcintval, hcintmsk, hcint.d32);

	urbd = ltqhc->urbd;

	if (hcint.b.ahberr)
		retval |= handle_hc_ahberr_intr(_ltqhcd, ltqhc, hc_regs, urbd);
	else if (hcint.b.chhltd)
		retval |= handle_hc_chhltd_intr(_ltqhcd, ltqhc, hc_regs, urbd);
	else
	{
		if (hcint.b.datatglerr)
			retval |= handle_hc_datatglerr_intr(_ltqhcd, ltqhc, hc_regs, urbd);
		if (hcint.b.frmovrun)
			retval |= handle_hc_frmovrun_intr(_ltqhcd, ltqhc, hc_regs, urbd);
		if (hcint.b.bblerr)
			retval |= handle_hc_bblerr_intr(_ltqhcd, ltqhc, hc_regs, urbd);
		if (hcint.b.xacterr)
			retval |= handle_hc_xacterr_intr(_ltqhcd, ltqhc, hc_regs, urbd);
		if (hcint.b.nyet)
			retval |= handle_hc_nyet_intr(_ltqhcd, ltqhc, hc_regs, urbd);
		if (hcint.b.ack)
			retval |= handle_hc_ack_intr(_ltqhcd, ltqhc, hc_regs, urbd);
		if (hcint.b.nak)
			retval |= handle_hc_nak_intr(_ltqhcd, ltqhc, hc_regs, urbd);
		if (hcint.b.stall)
			retval |= handle_hc_stall_intr(_ltqhcd, ltqhc, hc_regs, urbd);
		if (hcint.b.xfercomp)
			retval |= handle_hc_xfercomp_intr(_ltqhcd, ltqhc, hc_regs, urbd);
	}

	ltqusb_wreg(&hc_regs->hcint,hcintval);

	return retval;
}

static
uint8_t update_interval_counter(ltqhcd_epqh_t *_epqh,uint32_t _diff)
{
	if(_diff>=_epqh->period_counter)
	{
		_epqh->period_do=1;
		if(_diff>_epqh->interval)
			_epqh->period_counter=1;
		else
			_epqh->period_counter=_epqh->period_counter+_epqh->interval-_diff;
		return 1;
	}
	_epqh->period_counter=_epqh->period_counter-_diff;
	return 0;
}

/*!
 \brief Assigns transactions from a URBD to a free host channel and initializes the
 host channel to perform the transactions. The host channel is removed from
 the free list.
 \param _ltqhcd The HCD state structure.
 \param _epqh Transactions from the first URBD for this EPQH are selected and assigned to a free host channel.
 */
static
int assign_hc(ltqhcd_hcd_t *_ltqhcd, ltqhcd_epqh_t *_epqh,ltqhcd_urbd_t *_urbd)
{
	ltqhcd_hc_t   *ltqhc;
	struct urb    *urb;

	LTQ_DEBUGPL(DBG_HCDV, "%s(%p,%p)\n", __func__, _ltqhcd, _epqh);

	if(_ltqhcd->disconnecting)
	{
		printk(KERN_INFO "Warning: %s() Port is in discoonection\n",__func__);
		return 0;
	}

	if(!_epqh)       return 0;
	if(!_urbd)       return 0;
	if(!_urbd->urb)  return 0;
	if(_urbd->cancelling) return 0;

	{
		int i;
		int num_channels = _ltqhcd->core_if.params.host_channels;
		for(i=0;i<num_channels ; i++)
		{
			hcchar_data_t hcchar;
			ltqusb_hc_regs_t *hc_regs;
			hc_regs = _ltqhcd->core_if.hc_regs[i];
			if(_ltqhcd->ltqhc[i].phase!=HC_IDLE)
			{
				continue;
			}
			hcchar.d32 = ltqusb_rreg(&hc_regs->hcchar);
			if(hcchar.b.chen || hcchar.b.chdis)
			{
				continue;
			}
			break;
		}

		if(!_urbd->cancelling&&i<num_channels)
		{
			ltqhc=&_ltqhcd->ltqhc[i];
			ltqhc->phase=HC_ASSIGNED;
		}
		else
			return 0;
	}
	ltqhc->cancelling=0;

	urb         = _urbd->urb;

	ltqhc->epqh = _epqh;
	ltqhc->urbd = _urbd;
	_urbd->hc   = ltqhc;
	_urbd->epqh = _epqh;

	/*
	 * Use usb_pipedevice to determine device address. This address is
	 * 0 before the SET_ADDRESS command and the correct address afterward.
	 */
	ltqhc->dev_addr = usb_pipedevice(urb->pipe);
	ltqhc->ep_num   = usb_pipeendpoint(urb->pipe);

	if      (urb->dev->speed == USB_SPEED_LOW)  ltqhc->speed = LTQUSB_EP_SPEED_LOW;
	else if (urb->dev->speed == USB_SPEED_FULL) ltqhc->speed = LTQUSB_EP_SPEED_FULL;
	else                                        ltqhc->speed = LTQUSB_EP_SPEED_HIGH;

	ltqhc->mps         = _epqh->mps;
	ltqhc->halt_status = HC_XFER_NO_HALT_STATUS;
	ltqhc->ep_type = _epqh->ep_type;

	ltqhc->split = 0;
	if (_epqh->need_split)
	{
		ltqhc->split = 1;
		ltqhc->hub_addr       = urb->dev->tt->hub->devnum;
		ltqhc->port_addr      = urb->dev->ttport;
	}
	return 1;
}

/*!
 \brief Assigns transactions from a URBD to a free host channel and initializes the
 host channel to perform the transactions. The host channel is removed from
 the free list.
 \param _ltqhcd The HCD state structure.
 \param _epqh Transactions from the first URBD for this EPQH are selected and assigned to a free host channel.
 */
static
void init_hc(ltqhcd_epqh_t *_epqh, ltqhcd_hc_t   *_ltqhc)
{
	ltqhcd_urbd_t *urbd;
	struct urb    *urb;
	ltqhcd_hcd_t  *ltqhcd;

	LTQ_DEBUGPL(DBG_HCDV, "%s(%p)\n", __func__, _epqh);

	if(_ltqhc->cancelling)
		return;

	urbd  =_ltqhc->urbd;
	ltqhcd=_epqh->ltqhcd;
	urb  = urbd->urb;

	_ltqhc->halt_status = HC_XFER_NO_HALT_STATUS;

	if(_epqh->ep_type==LTQUSB_EP_TYPE_CTRL)
	{
		_ltqhc->control_phase  =LTQHCD_CONTROL_SETUP;
		_ltqhc->is_in          = 0;
		_ltqhc->data_pid_start = LTQUSB_HC_PID_SETUP;
		_ltqhc->xfer_buff      = urbd->setup_buff;
		#if defined(__UNALIGNED_BUF_ADJ__)
			if(urbd->aligned_setup)
				_ltqhc->xfer_buff = urbd->aligned_setup;
			else
		#endif
				_ltqhc->xfer_buff      = urbd->setup_buff;
		_ltqhc->xfer_len       = 8;
		_ltqhc->xfer_count     = 0;
		_ltqhc->short_rw       =(urb->transfer_flags & URB_ZERO_PACKET)?1:0;
		_ltqhc->sof_delay      = 0;
		_epqh->do_ping=0;
		if(!_ltqhc->is_in && _ltqhc->split==0)
			_epqh->do_ping=1;
	}
	else if(_epqh->ep_type==LTQUSB_EP_TYPE_ISOC)
	{
		#ifdef __EN_ISOC__
			struct usb_iso_packet_descriptor *frame_desc;
			_ltqhc->is_in          = urbd->is_in;
			frame_desc            = &urb->iso_frame_desc[urbd->isoc_frame_index];
			urbd->xfer_len        = _ltqhc->xfer_len       = frame_desc->length;
		#if defined(__UNALIGNED_BUF_ADJ__)
			if(urbd->aligned_buf)
				_ltqhc->xfer_buff = urbd->aligned_buf;
			else
		#endif
				_ltqhc->xfer_buff      = urbd->xfer_buff;
			_ltqhc->xfer_buff     += frame_desc->offset;
			_ltqhc->xfer_count     = 0;
			_ltqhc->sof_delay      = 0;
			if(usb_gettoggle (urb->dev,usb_pipeendpoint (urb->pipe), (_ltqhc->is_in)?0:1))
				_ltqhc->data_pid_start = LTQUSB_HCTSIZ_DATA1;
			else
				_ltqhc->data_pid_start = LTQUSB_HCTSIZ_DATA0;

			if(_ltqhc->is_in)
				_ltqhc->short_rw       =0;
			else
				_ltqhc->short_rw       =(urb->transfer_flags & URB_ZERO_PACKET)?1:0;
			#ifdef __EN_ISOC_SPLIT__
				_ltqhc->isoc_xact_pos = LTQUSB_HCSPLIT_XACTPOS_ALL;
			#endif

			_epqh->isoc_frame_index=0;
			_epqh->isoc_now=0;
			_epqh->isoc_start_frame=0;
			if(_urb->transfer_flags && URB_ISO_ASAP)
				_epqh->isoc_now=1;
			else
				_epqh->isoc_start_frame=_urb->start_frame;
			#ifdef __EN_ISOC_SPLIT__
				_epqh->isoc_split_pos   =0;
				_epqh->isoc_split_offset=0;
			#endif
			_epqh->do_ping=0;
		#endif
	}
	else
	{
		_ltqhc->is_in          = urbd->is_in;
		#if defined(__UNALIGNED_BUF_ADJ__)
			if(urbd->aligned_buf)
				_ltqhc->xfer_buff = urbd->aligned_buf;
			else
		#endif
				_ltqhc->xfer_buff      = urbd->xfer_buff;
		_ltqhc->xfer_len       = urbd->xfer_len;
		_ltqhc->xfer_count     = 0;
		_ltqhc->sof_delay      = 0;
//		if(_ltqhc->xfer_len==13 &&  _ltqhc->is_in && _epqh->ep_type==LTQUSB_EP_TYPE_BULK && _ltqhc->split==0)
//			_ltqhc->sof_delay      = 8;
		if(usb_gettoggle (urb->dev,usb_pipeendpoint (urb->pipe), (_ltqhc->is_in)?0:1))
			_ltqhc->data_pid_start = LTQUSB_HCTSIZ_DATA1;
		else
			_ltqhc->data_pid_start = LTQUSB_HCTSIZ_DATA0;
		if(_ltqhc->is_in)
			_ltqhc->short_rw       =0;
		else
			_ltqhc->short_rw       =(urb->transfer_flags & URB_ZERO_PACKET)?1:0;
		_epqh->do_ping=0;
		if(!_ltqhc->is_in && _ltqhc->split==0)
		{
			if(_epqh->ep_type==LTQUSB_EP_TYPE_BULK) _epqh->do_ping=1;
		}
	}

	{
		hcint_data_t      hc_intr_mask;
		uint8_t           hc_num = _ltqhc->hc_num;
		ltqusb_hc_regs_t *hc_regs = ltqhcd->core_if.hc_regs[hc_num];

		/* Clear old interrupt conditions for this host channel. */
		hc_intr_mask.d32 = 0xFFFFFFFF;
		hc_intr_mask.b.reserved = 0;
		ltqusb_wreg(&hc_regs->hcint, hc_intr_mask.d32);

		/* Enable channel interrupts required for this transfer. */
		hc_intr_mask.d32 = 0;
		hc_intr_mask.b.chhltd = 1;
		hc_intr_mask.b.ahberr = 1;

		ltqusb_wreg(&hc_regs->hcintmsk, hc_intr_mask.d32);

		/* Enable the top level host channel interrupt. */
		{
			uint32_t          intr_enable;
			intr_enable = (1 << hc_num);
			ltqusb_mreg(&ltqhcd->core_if.host_global_regs->haintmsk, 0, intr_enable);
		}

		/* Make sure host channel interrupts are enabled. */
		{
			gint_data_t       gintmsk ={.d32 = 0};
			gintmsk.b.hcintr = 1;
			ltqusb_mreg(&ltqhcd->core_if.core_global_regs->gintmsk, 0, gintmsk.d32);
		}

		/*
		 * Program the HCCHARn register with the endpoint characteristics for
		 * the current transfer.
		 */
		{
			hcchar_data_t     hcchar;

			hcchar.d32 = 0;
			hcchar.b.devaddr   =  _ltqhc->dev_addr;
			hcchar.b.epnum     =  _ltqhc->ep_num;
			hcchar.b.lspddev   = (_ltqhc->speed == LTQUSB_EP_SPEED_LOW);
			hcchar.b.eptype    =  _ltqhc->ep_type;
			hcchar.b.mps       =  _ltqhc->mps;
			ltqusb_wreg(&hc_regs->hcchar, hcchar.d32);

			LTQ_DEBUGPL(DBG_HCDV, "%s: Channel %d\n", __func__, _ltqhc->hc_num);
			LTQ_DEBUGPL(DBG_HCDV, "  Dev Addr: %d\n"    , hcchar.b.devaddr);
			LTQ_DEBUGPL(DBG_HCDV, "  Ep Num: %d\n"      , hcchar.b.epnum);
			LTQ_DEBUGPL(DBG_HCDV, "  Is Low Speed: %d\n", hcchar.b.lspddev);
			LTQ_DEBUGPL(DBG_HCDV, "  Ep Type: %d\n"     , hcchar.b.eptype);
			LTQ_DEBUGPL(DBG_HCDV, "  Max Pkt: %d\n"     , hcchar.b.mps);
			LTQ_DEBUGPL(DBG_HCDV, "  Multi Cnt: %d\n"   , hcchar.b.multicnt);
		}
		/* Program the HCSPLIT register for SPLITs */
		{
			hcsplt_data_t     hcsplt;

			hcsplt.d32 = 0;
			if (_ltqhc->split)
			{
				LTQ_DEBUGPL(DBG_HCDV, "Programming HC %d with split --> %s\n", _ltqhc->hc_num,
					   (_ltqhc->split==2) ? "CSPLIT" : "SSPLIT");
				hcsplt.b.spltena  = 1;
				hcsplt.b.compsplt = (_ltqhc->split==2);
				#if defined(__EN_ISOC__) && defined(__EN_ISOC_SPLIT__)
					if(_epqh->ep_type==LTQUSB_EP_TYPE_ISOC)
						hcsplt.b.xactpos  = _ltqhc->isoc_xact_pos;
					else
				#endif
					hcsplt.b.xactpos  = LTQUSB_HCSPLIT_XACTPOS_ALL;
				hcsplt.b.hubaddr  = _ltqhc->hub_addr;
				hcsplt.b.prtaddr  = _ltqhc->port_addr;
				LTQ_DEBUGPL(DBG_HCDV, "   comp split %d\n" , hcsplt.b.compsplt);
				LTQ_DEBUGPL(DBG_HCDV, "   xact pos %d\n"   , hcsplt.b.xactpos);
				LTQ_DEBUGPL(DBG_HCDV, "   hub addr %d\n"   , hcsplt.b.hubaddr);
				LTQ_DEBUGPL(DBG_HCDV, "   port addr %d\n"  , hcsplt.b.prtaddr);
				LTQ_DEBUGPL(DBG_HCDV, "   is_in %d\n"      , _ltqhc->is_in);
				LTQ_DEBUGPL(DBG_HCDV, "   Max Pkt: %d\n"   , _ltqhc->mps);
				LTQ_DEBUGPL(DBG_HCDV, "   xferlen: %d\n"   , _ltqhc->xfer_len);
			}
			ltqusb_wreg(&hc_regs->hcsplt, hcsplt.d32);
		}
	}
}


static
void select_eps_func(unsigned long data);

static
void select_eps_sub(ltqhcd_hcd_t *_ltqhcd)
{
	struct list_head *epqh_ptr;
	ltqhcd_epqh_t    *epqh;
	struct list_head *urbd_ptr;
#ifdef USE_SAFE_URBD
	struct list_head *urbd_ptr_next;
#endif
	unsigned long     flags;
	ltqhcd_urbd_t    *urbd;
	struct list_head *urbd_ptr_ori;
	unsigned int urbd_ptr_cnt;
	hfnum_data_t hfnum;
	uint32_t fndiff;

	if(_ltqhcd->disconnecting)
		return ;
	LOCK_SELECT(_ltqhcd);

	local_irq_save(flags);
	LOCK_EPQH_LIST(_ltqhcd);

	hfnum.d32 = ltqusb_rreg(&_ltqhcd->core_if.host_global_regs->hfnum);
	fndiff = hfnum.b.frnum;
	fndiff+= 0x00004000;
	fndiff-= _ltqhcd->lastframe ;
	fndiff&= 0x00003FFF;
	if(!fndiff) fndiff =1;

	#ifdef __EN_ISOC__
		epqh_ptr       = _ltqhcd->epqh_list_isoc.next;
		while (epqh_ptr != &_ltqhcd->epqh_list_isoc)
		{
			epqh = list_entry(epqh_ptr, ltqhcd_epqh_t, ql);
			epqh_ptr = epqh_ptr->next;

			#ifdef __DYN_SOF_INTR__
			if (!list_empty(&epqh->urbd_list))
				_ltqhcd->dyn_sof_count = DYN_SOF_COUNT_DEF;
			#endif

			if(epqh->phase==EPQH_READY)
			{
				if(update_interval_counter(epqh,fndiff) || epqh->isoc_now)
				{
					LOCK_URBD_LIST(epqh);
					urbd_ptr_ori = epqh->urbd_list.next;
					urbd_ptr_cnt = 0;
				#ifdef USE_SAFE_URBD
					list_for_each_safe(urbd_ptr, urbd_ptr_next, &epqh->urbd_list)
				#else //USE_SAFE_URBD
					urbd_ptr       = epqh->urbd_list.next;
					while (urbd_ptr != &epqh->urbd_list)
				#endif //USE_SAFE_URBD
					{
						urbd = list_entry(urbd_ptr, ltqhcd_urbd_t, ql);
					#ifndef USE_SAFE_URBD
						urbd_ptr=urbd_ptr->next;
					#endif //USE_SAFE_URBD
						if(urbd->cancelling) break;
						if(urbd->phase==URBD_ACTIVE || urbd->phase==URBD_RUNNING)
							break;
						if(urbd->phase==URBD_IDLE)
						{
							if(assign_hc(_ltqhcd, epqh,urbd))
							{
								LTQ_DEBUGPL(DBG_HCD, "  select_eps ISOC\n");
								#ifdef __EPQD_DESTROY_TIMEOUT__
									del_timer(&epqh->destroy_timer);
								#endif
								epqh->isoc_now=0;
								list_del_init (&epqh->ql);
								list_add_tail(&epqh->ql, &_ltqhcd->epqh_list_isoc);
								init_hc(epqh,urbd->hc);
								epqh->phase=EPQH_ACTIVE;
								urbd->phase=URBD_ACTIVE;
								urbd->hc.phase=HC_WAITING;
								ltqhcd_hc_start(_ltqhcd, urbd->hc,0);
							}
							break;
						}
						if(urbd_ptr_ori == urbd_ptr)
						{
							if(++urbd_ptr_cnt > ABORT_URBD_MAX)
							{
								printk(KERN_INFO "NEVER END URBD ISOC %d\n",urbd_ptr_cnt);
								urbd_ptr_cnt = 0;
							#ifdef ABORT_URBD
								break;
							#endif
							}
						}
						else
						{
							urbd_ptr_ori = urbd_ptr;
							urbd_ptr_cnt = 0;
						}
					}
					UNLOCK_URBD_LIST(epqh);
				}
			}
		}
	#endif //__EN_ISOC__

	epqh_ptr       = _ltqhcd->epqh_list_intr.next;
	while (epqh_ptr != &_ltqhcd->epqh_list_intr)
	{
		epqh = list_entry(epqh_ptr, ltqhcd_epqh_t, ql);
		epqh_ptr = epqh_ptr->next;
		#ifdef __DYN_SOF_INTR__
		if (!list_empty(&epqh->urbd_list))
			_ltqhcd->dyn_sof_count = DYN_SOF_COUNT_DEF;
		#endif
		if(epqh->phase==EPQH_READY)
		{
			if(update_interval_counter(epqh,fndiff))
			{
				LOCK_URBD_LIST(epqh);

				urbd_ptr_ori = epqh->urbd_list.next;
				urbd_ptr_cnt = 0;
			#ifdef USE_SAFE_URBD
				list_for_each_safe(urbd_ptr, urbd_ptr_next, &epqh->urbd_list)
			#else //USE_SAFE_URBD
				urbd_ptr       = epqh->urbd_list.next;
				while (urbd_ptr != &epqh->urbd_list)
			#endif //USE_SAFE_URBD
				{
					urbd = list_entry(urbd_ptr, ltqhcd_urbd_t, ql);
					#ifndef USE_SAFE_URBD
						urbd_ptr=urbd_ptr->next;
					#endif //USE_SAFE_URBD
					if(urbd->cancelling) break;
					if(urbd->phase==URBD_ACTIVE || urbd->phase==URBD_RUNNING)
						break;
					if(urbd->phase==URBD_IDLE)
					{
						if(assign_hc(_ltqhcd, epqh,urbd))
						{
							LTQ_DEBUGPL(DBG_HCD, "  select_eps INTR\n");
							#ifdef __EPQD_DESTROY_TIMEOUT__
								del_timer(&epqh->destroy_timer);
							#endif
							list_del_init (&epqh->ql);
							list_add_tail(&epqh->ql, &_ltqhcd->epqh_list_intr);
							init_hc(epqh,urbd->hc);
							epqh->phase=EPQH_ACTIVE;
							urbd->phase=URBD_ACTIVE;
							urbd->hc->phase=HC_WAITING;
							ltqhcd_hc_start(_ltqhcd, urbd->hc,0);
						}
						break;
					}
					if(urbd_ptr_ori == urbd_ptr)
					{
						if(++urbd_ptr_cnt > ABORT_URBD_MAX)
						{
							printk(KERN_INFO "NEVER END URBD INTR ep %02X\n",epqh->sysep->desc.bEndpointAddress);
							urbd_ptr_cnt = 0;
							#ifdef ABORT_URBD
								break;
							#endif
						}
					}
					else
					{
						urbd_ptr_ori = urbd_ptr;
						urbd_ptr_cnt = 0;
					}
				}
				UNLOCK_URBD_LIST(epqh);
			}
		}
		else if(epqh->phase==EPQH_STDBY)
		{
			if(epqh->period_counter > 0 )
				epqh->period_counter --;
			if(epqh->period_counter == 0)
				ltqhcd_epqh_idle_periodic(epqh);
			update_interval_counter(epqh,fndiff);
		}
		else
			update_interval_counter(epqh,fndiff);
	}

	epqh_ptr       = _ltqhcd->epqh_list_ctrl.next;
	while (epqh_ptr != &_ltqhcd->epqh_list_ctrl)  // may need to preserve at lease one for period
	{
		epqh = list_entry(epqh_ptr, ltqhcd_epqh_t, ql);
		epqh_ptr = epqh_ptr->next;
		#ifdef __DYN_SOF_INTR__
		if (!list_empty(&epqh->urbd_list))
			_ltqhcd->dyn_sof_count = DYN_SOF_COUNT_DEF;
		#endif
		if(epqh->phase==EPQH_READY)
		{
			LOCK_URBD_LIST(epqh);

#ifdef USE_SAFE_URBD
			list_for_each_safe(urbd_ptr, urbd_ptr_next, &epqh->urbd_list)
#else //USE_SAFE_URBD
			urbd_ptr       = epqh->urbd_list.next;
			while (urbd_ptr != &epqh->urbd_list)
#endif //USE_SAFE_URBD
			{
				urbd = list_entry(urbd_ptr, ltqhcd_urbd_t, ql);
#ifndef USE_SAFE_URBD
				urbd_ptr=urbd_ptr->next;
#endif //USE_SAFE_URBD
				if(urbd->cancelling)
					break;
				if(urbd->phase==URBD_ACTIVE || urbd->phase==URBD_RUNNING)
					break;
				if(urbd->phase==URBD_IDLE)
				{
					if(assign_hc(_ltqhcd, epqh,urbd))
					{
						LTQ_DEBUGPL(DBG_HCD, "  select_eps Non-Period\n");
						#ifdef __EPQD_DESTROY_TIMEOUT__
							del_timer(&epqh->destroy_timer);
						#endif
						list_del_init (&epqh->ql);
						list_add_tail(&epqh->ql, &_ltqhcd->epqh_list_ctrl);
						init_hc(epqh,urbd->hc);
						epqh->phase=EPQH_ACTIVE;
						urbd->phase=URBD_ACTIVE;
						urbd->hc->phase=HC_WAITING;
//						ltqhcd_hc_start(_ltqhcd, urbd->hc,epqh->burst_on);
						ltqhcd_hc_start(_ltqhcd, urbd->hc,0);
					}
					break;
				}
			}
			UNLOCK_URBD_LIST(epqh);
		}
	}

	epqh_ptr       = _ltqhcd->epqh_list_bulk.next;
	while (epqh_ptr != &_ltqhcd->epqh_list_bulk)  // may need to preserve at lease one for period
	{
		epqh = list_entry(epqh_ptr, ltqhcd_epqh_t, ql);
		epqh_ptr = epqh_ptr->next;
		#ifdef __DYN_SOF_INTR__
		if (!list_empty(&epqh->urbd_list))
			_ltqhcd->dyn_sof_count = DYN_SOF_COUNT_DEF;
		#endif
		if(epqh->phase==EPQH_READY)
		{
			LOCK_URBD_LIST(epqh);

#ifdef USE_SAFE_URBD
			list_for_each_safe(urbd_ptr, urbd_ptr_next, &epqh->urbd_list)
#else //USE_SAFE_URBD
			urbd_ptr       = epqh->urbd_list.next;
			while (urbd_ptr != &epqh->urbd_list)
#endif //USE_SAFE_URBD
			{
				urbd = list_entry(urbd_ptr, ltqhcd_urbd_t, ql);
#ifndef USE_SAFE_URBD
				urbd_ptr=urbd_ptr->next;
#endif //USE_SAFE_URBD
				if(urbd->cancelling)
					break;
				if(urbd->phase==URBD_ACTIVE || urbd->phase==URBD_RUNNING)
					break;
				if(urbd->phase==URBD_IDLE)
				{
					if(assign_hc(_ltqhcd, epqh,urbd))
					{
						LTQ_DEBUGPL(DBG_HCD, "  select_eps Non-Period\n");
						#ifdef __EPQD_DESTROY_TIMEOUT__
							del_timer(&epqh->destroy_timer);
						#endif
						list_del_init (&epqh->ql);
						list_add_tail(&epqh->ql, &_ltqhcd->epqh_list_bulk);
						init_hc(epqh,urbd->hc);
						epqh->phase=EPQH_ACTIVE;
						urbd->phase=URBD_ACTIVE;
						urbd->hc->phase=HC_WAITING;
//						ltqhcd_hc_start(_ltqhcd, urbd->hc,epqh->burst_on);
						ltqhcd_hc_start(_ltqhcd, urbd->hc,0);
					}
					break;
				}
			}
			UNLOCK_URBD_LIST(epqh);
		}
	}

	_ltqhcd->lastframe=hfnum.b.frnum;

	UNLOCK_EPQH_LIST(_ltqhcd);
	local_irq_restore(flags);
	UNLOCK_SELECT(_ltqhcd);
}

static
void select_eps_func(unsigned long data)
{
	ltqhcd_hcd_t *ltqhcd;
	ltqhcd=((ltqhcd_hcd_t *)data);

	select_eps_sub(ltqhcd);
}

/*!
	 \fn    void select_eps(ltqhcd_hcd_t *_ltqhcd)
	 \brief This function selects transactions from the HCD transfer schedule and assigns them to available host channels.
	 \param _ltqhcd Pointer to the sate of HCD structure
	 \ingroup  LTQUSB_HCD
 */
void select_eps(ltqhcd_hcd_t *_ltqhcd)
{
	if(!_ltqhcd->tasklet_select_eps.func)
	{
		_ltqhcd->tasklet_select_eps.next = NULL;
		_ltqhcd->tasklet_select_eps.state = 0;
		atomic_set( &_ltqhcd->tasklet_select_eps.count, 0);
		_ltqhcd->tasklet_select_eps.func = select_eps_func;
		_ltqhcd->tasklet_select_eps.data = (unsigned long)_ltqhcd;
	}
	tasklet_schedule(&_ltqhcd->tasklet_select_eps);
}


void ltqhcd_xfer_stopper(ltqhcd_hcd_t *_ltqhcd)
{
	ltqhcd_hc_t      *ltqhc;
	int               num_channels;
	ltqusb_hc_regs_t *hc_regs;
	int               i;

	num_channels = _ltqhcd->core_if.params.host_channels;
	for (i = 0; i < num_channels; i++)
	{
		ltqhc=&_ltqhcd->ltqhc[i];
		if(ltqhc->stopping)
			continue;
		if(ltqhc->ep_type == LTQUSB_EP_TYPE_BULK ||
		   (
			ltqhc->ep_type == LTQUSB_EP_TYPE_CTRL && ltqhc->control_phase ==  LTQHCD_CONTROL_DATA
		   )
		)
		{
			hcchar_data_t 	hcchar;
			hc_regs = _ltqhcd->core_if.hc_regs[i];
			hcchar.d32=ltqusb_rreg(&hc_regs->hcchar);
			if(hcchar.b.chen)
			{
				ltqhc->stopping=1;
				hcchar.b.chdis = 1;
				ltqhc->halt_status = HC_XFER_STOPPED;
				ltqusb_wreg(&hc_regs->hcchar, hcchar.d32);
			}
		}
	}
}



inline
int ltqhcd_hc_kickstart_check(ltqhcd_hcd_t *_ltqhcd)
{
	if(_ltqhcd->xfer_prot1_value)
	{
		hfnum_data_t hfnum;
		hfnum.d32 = ltqusb_rreg(&_ltqhcd->core_if.host_global_regs->hfnum);

		if(hfnum.b.frrem < _ltqhcd->xfer_prot1_value)
			return 1;
	}
	return 0;
}

static
void ltqhcd_hc_kickstart(ltqhcd_hcd_t *_ltqhcd)
{
	int               num_channels;
	ltqusb_hc_regs_t *hc_regs;
	int               i;
	ltqhcd_hc_t      *ltqhc;
	num_channels = _ltqhcd->core_if.params.host_channels;

	if(ltqhcd_hc_kickstart_check(_ltqhcd))
	{
		if(hrtimer_try_to_cancel(&_ltqhcd->xfer_stopper_timer)!=-1)
			ltqhcd_xfer_stopper(_ltqhcd);
	}
	else
	{
		for (i = 0; i < num_channels; i++)
		{
			ltqhc=&_ltqhcd->ltqhc[i];

			if(ltqhc->cancelling) continue;
			if(ltqhc->stopping)   continue;
			if(ltqhc->phase==HC_STARTING_HIGH)
			{
				if(ltqhc->sof_delay) ltqhc->sof_delay--;
				if(!ltqhc->sof_delay)
				{
					hcint_data_t hcint;
					hcchar_data_t 	hcchar;
//					ltqhc->erron=0;
					hc_regs = _ltqhcd->core_if.hc_regs[i];

					hcchar.d32=ltqusb_rreg(&hc_regs->hcchar);
					if(hcchar.b.chen || hcchar.b.chdis)
					{
						if(ltqhc->phase==HC_STARTING_HIGH)
						{
							printk(KERN_INFO "%s:%s() %d PERIOD CH STILL ENABLED %08x \n",__FILE__,__func__,__LINE__,hcchar.d32);
							ltqhc->sof_delay=1;
						}
					}
					else
					{
						hcint.d32 =0xFFFFFFFF;
						ltqusb_wreg(&hc_regs->hcint, hcint.d32);
						hcint.d32 =ltqusb_rreg(&hc_regs->hcintmsk);
						hcint.b.nak =0;
						hcint.b.ack =0;
						hcint.b.nyet=0;
						if(ltqhc->erron)
						{
							hcint.b.ack  =1;
							hcint.b.nak  =1;
							hcint.b.nyet =1;
						}
//						if(!ltqhcd_hc_kickstart_check(_ltqhcd))
//							break;
						if(!ltqhc->cancelling)
						{
							ltqusb_wreg(&hc_regs->hcintmsk, hcint.d32);
							ltqusb_wreg(&hc_regs->hcchar, ltqhc->hcchar);
							ltqhc->phase=HC_STARTED;
						}
					}
				}
			}
		}

		for (i = 0; i < num_channels; i++)
		{
			ltqhc=&_ltqhcd->ltqhc[i];
			if(ltqhc->cancelling) continue;
			if(ltqhc->stopping)   continue;
			if(ltqhc->phase==HC_STARTING)
			{
				if(ltqhc->sof_delay) ltqhc->sof_delay--;
				if(!ltqhc->sof_delay)
				{
					hcint_data_t hcint;
					hcchar_data_t 	hcchar;
//					ltqhc->erron=0;
					hc_regs = _ltqhcd->core_if.hc_regs[i];
					hcchar.d32=ltqusb_rreg(&hc_regs->hcchar);
					if(hcchar.b.chen || hcchar.b.chdis)
					{
						if(ltqhc->phase==HC_STARTING)
						{
							printk(KERN_INFO "%s:%s() %d NON-PERIOD CH %d STILL ENABLED %p %08x  \n",__FILE__,__func__,__LINE__,i,&hc_regs->hcchar,hcchar.d32);
							ltqhc->sof_delay=1;
							hcchar.b.chdis=1;
							ltqusb_wreg(&hc_regs->hcchar,hcchar.d32);
						}
					}
					else
					{
						hcint.d32 =0xFFFFFFFF;
						ltqusb_wreg(&hc_regs->hcint, hcint.d32);
						hcint.d32 =ltqusb_rreg(&hc_regs->hcintmsk);
						hcint.b.nak =0;
						hcint.b.ack =0;
						hcint.b.nyet=0;
						if(ltqhc->erron)
						{
							hcint.b.ack  =1;
							hcint.b.nak  =1;
							hcint.b.nyet =1;
						}
//						if(!ltqhcd_hc_kickstart_check(_ltqhcd))
//							break;
						if(!ltqhc->cancelling)
						{
							ltqusb_wreg(&hc_regs->hcintmsk, hcint.d32);
							ltqusb_wreg(&hc_regs->hcchar, ltqhc->hcchar);
							ltqhc->phase=HC_STARTED;
						}
					}
				}
			}
		}
		for (i = 0; i < num_channels; i++)
		{
			ltqhc=&_ltqhcd->ltqhc[i];
			if(ltqhc->cancelling) continue;
			if(ltqhc->phase==HC_WAITING &&
			    (ltqhc->ep_type == LTQUSB_EP_TYPE_INTR || ltqhc->ep_type == LTQUSB_EP_TYPE_ISOC)
			  )
				ltqhcd_hc_start(_ltqhcd, ltqhc,0);
		}
		for (i = 0; i < num_channels; i++)
		{
			ltqhc=&_ltqhcd->ltqhc[i];
			if(ltqhc->cancelling) continue;
			if(ltqhc->phase==HC_WAITING)
				ltqhcd_hc_start(_ltqhcd, ltqhc,0);
		}
	}
}


/*
 * Handles the start-of-frame interrupt in host mode. Non-periodic
 * transactions may be queued to the DWC_otg controller for the current
 * (micro)frame. Periodic transactions may be queued to the controller for the
 * next (micro)frame.
 */
enum hrtimer_restart xfer_stopper_timer_callback(struct hrtimer *pt)
{
	ltqhcd_hcd_t     *ltqhcd=container_of(pt,ltqhcd_hcd_t,xfer_stopper_timer);
	ltqhcd->xfer_stopper_timer_on=0;
	if(ltqhcd->use_hrtimer)
		ltqhcd_xfer_stopper(ltqhcd);
	return HRTIMER_NORESTART;
}

static
int32_t handle_sof_intr (ltqhcd_hcd_t *_ltqhcd)
{
	if(_ltqhcd->use_hrtimer && _ltqhcd->xfer_prot2_value)
	{
		ktime_t ktime;
		ktime=ktime_set(0,_ltqhcd->xfer_prot2_value);
		hrtimer_start(&_ltqhcd->xfer_stopper_timer,ktime,HRTIMER_MODE_REL);
		_ltqhcd->xfer_stopper_timer_on=1;
	}

	ltqhcd_hc_kickstart(_ltqhcd);
	select_eps(_ltqhcd);

	/* Clear interrupt */
	{
		gint_data_t gintsts;
		gintsts.d32=0;
		gintsts.b.sofintr = 1;
		ltqusb_wreg(&_ltqhcd->core_if.core_global_regs->gintsts, gintsts.d32);

		#ifdef __DYN_SOF_INTR__
			if(_ltqhcd->dyn_sof_count)
				_ltqhcd->dyn_sof_count--;
			if(!_ltqhcd->dyn_sof_count)
				ltqusb_mreg(&_ltqhcd->core_if.core_global_regs->gintmsk, gintsts.d32,0);
		#endif
	}
	return 1;
}



/* There are multiple conditions that can cause a port interrupt. This function
 * determines which interrupt conditions have occurred and handles them
 * appropriately. */
static
int32_t handle_port_intr (ltqhcd_hcd_t *_ltqhcd)
{
	int retval = 0;
	hprt0_data_t hprt0;
	hprt0_data_t hprt0_modify;

	hprt0.d32        =
	hprt0_modify.d32 = ltqusb_rreg(_ltqhcd->core_if.hprt0);

	/* Clear appropriate bits in HPRT0 to clear the interrupt bit in
	 * GINTSTS */

	hprt0_modify.b.prtena = 0;
	hprt0_modify.b.prtconndet = 0;
	hprt0_modify.b.prtenchng = 0;
	hprt0_modify.b.prtovrcurrchng = 0;

	/* Port Connect Detected
	 * Set flag and clear if detected */
	if (hprt0.b.prtconndet) {
		LTQ_DEBUGPL(DBG_HCD, "--Port Interrupt HPRT0=0x%08x "
			    "Port Connect Detected--\n", hprt0.d32);
		_ltqhcd->flags.b.port_connect_status_change = 1;
		_ltqhcd->flags.b.port_connect_status = 1;
		hprt0_modify.b.prtconndet = 1;

		/* The Hub driver asserts a reset when it sees port connect
		 * status change flag */
		retval |= 1;
	}

	/* Port Enable Changed
	 * Clear if detected - Set internal flag if disabled */
	if (hprt0.b.prtenchng) {
		LTQ_DEBUGPL(DBG_HCD, "  --Port Interrupt HPRT0=0x%08x "
			    "Port Enable Changed--\n", hprt0.d32);
		hprt0_modify.b.prtenchng = 1;
		if (hprt0.b.prtena == 1)
		{
			/* Port has been enabled set the reset change flag */
			_ltqhcd->flags.b.port_reset_change = 1;
		}
		else
			_ltqhcd->flags.b.port_enable_change = 1;
		retval |= 1;
	}

	/* Overcurrent Change Interrupt */

	if (hprt0.b.prtovrcurrchng) {
#if 0 // OC is handled by AFE, not USB PHY
		LTQ_DEBUGPL(DBG_HCD, "  --Port Interrupt HPRT0=0x%08x "
			    "Port Overcurrent Changed--\n", hprt0.d32);
		_ltqhcd->flags.b.port_over_current_change = 1;
#else
		LTQ_WARN("Core Overcurrent Change triggered : HPRT0=0x%08x  chg:%d sts:%d\n",hprt0.d32,hprt0.b.prtovrcurrchng,hprt0.b.prtovrcurract);
#endif
		hprt0_modify.b.prtovrcurrchng = 1;
		retval |= 1;
	}
	/* Clear Port Interrupts */
	ltqusb_wreg(_ltqhcd->core_if.hprt0, hprt0_modify.d32);

	if(retval)
		usb_hcd_poll_rh_status(_ltqhcd->syshcd);
	return retval;
}

/*
 * This interrupt indicates that SUSPEND state has been detected on
 * the USB.
 * No Functioning in Host Mode
 */
static
int32_t handle_usb_suspend_intr(ltqhcd_hcd_t *_ltqhcd)
{
	gint_data_t gintsts;
	LTQ_DEBUGP("USB SUSPEND RECEIVED!\n");
	/* Clear interrupt */
	gintsts.d32 = 0;
	gintsts.b.usbsuspend = 1;
	ltqusb_wreg(&_ltqhcd->core_if.core_global_regs->gintsts, gintsts.d32);
	return 1;
}

/*
 * This interrupt indicates that the LTQUSB controller has detected a
 * resume or remote wakeup sequence. If the LTQUSB controller is in
 * low power mode, the handler must brings the controller out of low
 * power mode. The controller automatically begins resume
 * signaling. The handler schedules a time to stop resume signaling.
 */
static
int32_t handle_wakeup_detected_intr(ltqhcd_hcd_t *_ltqhcd)
{
	gint_data_t gintsts;
	hprt0_data_t hprt0 = {.d32=0};
	pcgcctl_data_t pcgcctl = {.d32=0};
	ltqusb_core_if_t *core_if = &_ltqhcd->core_if;

	LTQ_DEBUGPL(DBG_ANY, "++Resume and Remote Wakeup Detected Interrupt++\n");

	/*
	 * Clear the Resume after 70ms. (Need 20 ms minimum. Use 70 ms
	 * so that OPT tests pass with all PHYs).
	 */
	/* Restart the Phy Clock */
	pcgcctl.b.stoppclk = 1;
	ltqusb_mreg(core_if->pcgcctl, pcgcctl.d32, 0);
	UDELAY(10);

	/* Now wait for 70 ms. */
	hprt0.d32 = ltqusb_read_hprt0( core_if );
	LTQ_DEBUGPL(DBG_ANY,"Resume: HPRT0=%0x\n", hprt0.d32);
	MDELAY(70);
	hprt0.b.prtres = 0; /* Resume */
	ltqusb_wreg(core_if->hprt0, hprt0.d32);
	LTQ_DEBUGPL(DBG_ANY,"Clear Resume: HPRT0=%0x\n", ltqusb_rreg(core_if->hprt0));

	/* Clear interrupt */
	gintsts.d32 = 0;
	gintsts.b.wkupintr = 1;
	ltqusb_wreg(&core_if->core_global_regs->gintsts, gintsts.d32);
	return 1;
}

/*
 * This interrupt indicates that a device is initiating the Session
 * Request Protocol to request the host to turn on bus power so a new
 * session can begin. The handler responds by turning on bus power. If
 * the DWC_otg controller is in low power mode, the handler brings the
 * controller out of low power mode before turning on bus power.
 */
static
int32_t handle_session_req_intr(ltqhcd_hcd_t *_ltqhcd)
{
	/* Clear interrupt */
	gint_data_t gintsts = { .d32 = 0 };
	gintsts.b.sessreqintr = 1;
	ltqusb_wreg(&_ltqhcd->core_if.core_global_regs->gintsts, gintsts.d32);
	return 1;
}

/*
 * This interrupt indicates that a device has been disconnected from
 * the root port.
 */
static
int32_t handle_disconnect_intr(ltqhcd_hcd_t *_ltqhcd)
{
	gint_data_t gintsts;
	ltqhcd_disconnect(_ltqhcd);

	gintsts.d32 = 0;
	gintsts.b.disconnect = 1;
	ltqusb_wreg(&_ltqhcd->core_if.core_global_regs->gintsts, gintsts.d32);
	return 1;
}

/*
 * This function handles the Connector ID Status Change Interrupt.  It
 * reads the OTG Interrupt Register (GOTCTL) to determine whether this
 * is a Device to Host Mode transition or a Host Mode to Device
 * Transition.
 * This only occurs when the cable is connected/removed from the PHY
 * connector.
 */
static
int32_t handle_conn_id_status_change_intr(ltqhcd_hcd_t *_ltqhcd)
{
	gint_data_t gintsts;

	LTQ_WARN("ID Status Change Interrupt: currently in %s mode\n",
	     ltqusb_mode(&_ltqhcd->core_if) ? "Host" : "Device");

	gintsts.d32 = 0;
	gintsts.b.conidstschng = 1;
	ltqusb_wreg(&_ltqhcd->core_if.core_global_regs->gintsts, gintsts.d32);
	return 1;
}

static
int32_t handle_otg_intr(ltqhcd_hcd_t *_ltqhcd)
{
	ltqusb_core_global_regs_t *global_regs = _ltqhcd->core_if.core_global_regs;
	gotgint_data_t gotgint;
	gotgint.d32 = ltqusb_rreg( &global_regs->gotgint);
	/* Clear GOTGINT */
	ltqusb_wreg (&global_regs->gotgint, gotgint.d32);
	return 1;
}

/** This function will log a debug message */
static
int32_t handle_mode_mismatch_intr(ltqhcd_hcd_t *_ltqhcd)
{
	gint_data_t gintsts;

	LTQ_WARN("Mode Mismatch Interrupt: currently in %s mode\n",
	     ltqusb_mode(&_ltqhcd->core_if) ? "Host" : "Device");
	gintsts.d32 = 0;
	gintsts.b.modemismatch = 1;
	ltqusb_wreg(&_ltqhcd->core_if.core_global_regs->gintsts, gintsts.d32);
	return 1;
}

/** This function handles interrupts for the HCD. */
int32_t ltqhcd_handle_intr (ltqhcd_hcd_t *_ltqhcd)
{
	int retval = 0;

	ltqusb_core_if_t *core_if = &_ltqhcd->core_if;
	gint_data_t gintsts;

	/* Check if HOST Mode */
	if (ltqusb_is_device_mode(core_if))
	{
		LTQ_ERROR("%s() CRITICAL!  IN DEVICE MODE\n", __func__);
		return 0;
	}

	gintsts.d32 = ltqusb_read_core_intr(core_if);

	if (!gintsts.d32)
		return 0;

	LOCK_INTR(_ltqhcd);

	//Common INT
	if (gintsts.b.modemismatch)
	{
		retval |= handle_mode_mismatch_intr(_ltqhcd);
	}
	if (gintsts.b.otgintr)
	{
		retval |= handle_otg_intr(_ltqhcd);
	}
	if (gintsts.b.conidstschng)
	{
		retval |= handle_conn_id_status_change_intr(_ltqhcd);
	}
	if (gintsts.b.disconnect)
	{
		retval |= handle_disconnect_intr(_ltqhcd);
	}
	if (gintsts.b.sessreqintr)
	{
		retval |= handle_session_req_intr(_ltqhcd);
	}
	if (gintsts.b.wkupintr)
	{
		retval |= handle_wakeup_detected_intr(_ltqhcd);
	}
	if (gintsts.b.usbsuspend)
	{
		retval |= handle_usb_suspend_intr(_ltqhcd);
	}

	//Host Int
	if (gintsts.b.sofintr)
	{
		retval |= handle_sof_intr (_ltqhcd);
	}
	if (gintsts.b.portintr)
	{
		retval |= handle_port_intr (_ltqhcd);
	}
	if (gintsts.b.hcintr)
	{
		int i;
		haint_data_t haint;
		haint.d32 = ltqusb_read_host_all_channels_intr(core_if);
		for (i=0; i<MAX_EPS_CHANNELS && i< core_if->params.host_channels; i++)
			if (haint.b2.chint & (1 << i))
				retval |= handle_hc_n_intr (_ltqhcd, i);
	}
	UNLOCK_INTR(_ltqhcd);
	return retval;
}

