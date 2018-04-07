/*****************************************************************************
 **   FILE NAME       : ltqhcd.h
 **   PROJECT         : Lantiq USB sub-system V3
 **   MODULES         : Lantiq USB sub-system Host and Device driver
 **   SRC VERSION     : 3.2
 **   DATE            : 1/Jan/2011
 **   AUTHOR          : Chen, Howard
 **   DESCRIPTION     : This file contains the structures, constants, and interfaces for
 **                     the Host Contoller Driver (HCD).
 **
 **                     The Host Controller Driver (HCD) is responsible for translating requests
 **                     from the USB Driver into the appropriate actions on the LTQUSB controller.
 **                     It isolates the USBD from the specifics of the controller by providing an
 **                     API to the USBD.
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
  \defgroup LTQUSB_HCD HCD Interface
  \ingroup LTQUSB_DRIVER_V3
  \brief  The Host Controller Driver (HCD) is responsible for translating requests
 from the USB Driver into the appropriate actions on the LTQUSB controller.
 It isolates the USBD from the specifics of the controller by providing an
 API to the USBD.
 */


/*!
  \file ltqhcd.h
  \ingroup LTQUSB_DRIVER_V3
  \brief This file contains the structures, constants, and interfaces for
 the Host Contoller Driver (HCD).
 */

#if !defined(__LTQHCD_H__)
#define __LTQHCD_H__

#include <linux/list.h>
#include <linux/usb.h>

#include <linux/usb/hcd.h>

#include "ltqusb_cif.h"
#include "ltqusb_plat.h"

#include <linux/hrtimer.h>
#include <linux/ktime.h>

#ifdef __SINGLE_XACT__
	#define XFER_PROT1_USEC  80
	#define XFER_PROT2_USEC  0
#else
	#define XFER_PROT1_USEC  60
	#define XFER_PROT2_USEC  70
#endif

/* Phases for control transfers.*/
typedef enum ltqhcd_epqh_phase {
	EPQH_IDLE=0,
	EPQH_DISABLING,
//	EPQH_COMPLETING,
	EPQH_STDBY,
	EPQH_READY,
	EPQH_ACTIVE
} ltqhcd_epqh_phase_e;

/* Phases for control transfers.*/
typedef enum ltqhcd_urbd_phase {
	URBD_IDLE=0,
	URBD_ACTIVE,
	URBD_RUNNING,
	URBD_FINISHING,    //URB_Complete already scheduled
	URBD_COMPLETING,   //To URB_Complete, it's normal finish
	URBD_DEQUEUEING,   //To URB_Complete, it's abnormal finish
} ltqhcd_urbd_phase_e;

/* Phases for control transfers.*/
typedef enum ltqhcd_hc_phase {
	HC_IDLE=0,
	HC_ASSIGNED,
	HC_WAITING,
	HC_WAITING2,
	HC_STARTING,
	HC_STARTING_HIGH,
	HC_STARTED,
	HC_STOPPING,
	HC_STOPPED,
} ltqhcd_hc_phase_e;

/*!
  \addtogroup LTQUSB_HCD
 */
/*@{*/

/*! \typedef  ltqhcd_control_phase_e
	\brief Phases for control transfers.
*/

typedef enum ltqhcd_control_phase {
	LTQHCD_CONTROL_SETUP,
	LTQHCD_CONTROL_DATA,
	LTQHCD_CONTROL_STATUS
} ltqhcd_control_phase_e;

/*! \typedef  ltqhcd_halt_status_e
	\brief Reasons for halting a host channel.
*/
typedef enum ltqhcd_halt_status
{
	HC_XFER_NO_HALT_STATUS,         // Initial
	HC_XFER_COMPLETE,               // Xact complete without error, upward
	HC_XFER_URB_COMPLETE,           // Xfer complete without error, short upward
	HC_XFER_STALL,                  // HC stopped abnormally, upward/downward
	HC_XFER_XACT_ERR,               // HC stopped abnormally, upward
	HC_XFER_FRAME_OVERRUN,          // HC stopped abnormally, upward
	HC_XFER_BABBLE_ERR,             // HC stopped abnormally, upward
	HC_XFER_AHB_ERR,                // HC stopped abnormally, upward
	HC_XFER_DATA_TOGGLE_ERR,
	HC_XFER_URB_DEQUEUE,            // HC stopper manually, downward
	HC_XFER_NO_URB,                 // HC stopper manually, downward
	HC_XFER_NO_EPQH,                // HC stopper manually, downward
	HC_XFER_STOPPED,                // HC stopped by monitor, downward
	#if defined(__INTRNAKRETRY__) || defined(__INTRINCRETRY__)
		HC_XFER_INTR_NAK_RETRY,     // HC stopped by nak monitor, downward
	#endif
} ltqhcd_halt_status_e;

struct ltqhcd_urbd;
struct ltqhcd_hc ;
struct ltqhcd_epqh ;
struct ltqhcd_hcd;

/*! typedef ltqhcd_urbd_t
 \brief A URB Descriptor (URBD) holds the state of a bulk, control,
  interrupt, or isochronous transfer. A single URBD is created for each URB
  (of one of these types) submitted to the HCD. The transfer associated with
  a URBD may require one or multiple transactions.

  A URBD is linked to a EP Queue Head, which is entered in either the
  isoc, intr or non-periodic schedule for execution. When a URBD is chosen for
  execution, some or all of its transactions may be executed. After
  execution, the state of the URBD is updated. The URBD may be retired if all
  its transactions are complete or if an error occurred. Otherwise, it
  remains in the schedule so more transactions can be executed later.
 */
typedef struct ltqhcd_urbd {
	ltqhcd_urbd_phase_e       phase;
	struct list_head          ql;               // Hook for EPQH->urbd_list
	struct urb               *urb;              /*!< URB for this transfer */
	                                            //struct urb {
	                                            //  struct list_head urb_list;
	                                            //  struct list_head anchor_list;
	                                            //  struct usb_anchor * anchor;
	                                            //  struct usb_device * dev;
	                                            //  struct usb_host_endpoint * ep;
	                                            //  unsigned int pipe;
	                                            //  int status;
	                                            //  unsigned int transfer_flags;
	                                            //  void * transfer_buffer;
	                                            //  dma_addr_t transfer_dma;
	                                            //  u32 transfer_buffer_length;
	                                            //  u32 actual_length;
	                                            //  unsigned char * setup_packet;
	                                            //  dma_addr_t setup_dma;
	                                            //  int start_frame;
	                                            //  int number_of_packets;
	                                            //  int interval;
	                                            //  int error_count;
	                                            //  void * context;
	                                            //  usb_complete_t complete;
	                                            //  struct usb_iso_packet_descriptor iso_frame_desc[0];
	                                            //};
	                                            //urb_list         For use by current owner of the URB.
	                                            //anchor_list      membership in the list of an anchor
	                                            //anchor           to anchor URBs to a common mooring
	                                            //dev              Identifies the USB device to perform the request.
	                                            //ep               Points to the endpoint's data structure. Will
	                                            //                 eventually replace pipe.
	                                            //pipe             Holds endpoint number, direction, type, and more.
	                                            //                 Create these values with the eight macros available; u
	                                            //                 sb_{snd,rcv}TYPEpipe(dev,endpoint), where the TYPE is
	                                            //                  "ctrl", "bulk", "int" or "iso". For example
	                                            //                 usb_sndbulkpipe or usb_rcvintpipe. Endpoint numbers
	                                            //                 range from zero to fifteen. Note that "in" endpoint two
	                                            //                 is a different endpoint (and pipe) from "out" endpoint
	                                            //                 two. The current configuration controls the existence,
	                                            //                 type, and maximum packet size of any given endpoint.
	                                            //status           This is read in non-iso completion functions to get
	                                            //                 the status of the particular request. ISO requests
	                                            //                 only use it to tell whether the URB was unlinked;
	                                            //                 detailed status for each frame is in the fields of
	                                            //                 the iso_frame-desc.
	                                            //transfer_flags   A variety of flags may be used to affect how URB
	                                            //                 submission, unlinking, or operation are handled.
	                                            //                 Different kinds of URB can use different flags.
	                                            //                      URB_SHORT_NOT_OK
	                                            //                      URB_ISO_ASAP
	                                            //                      URB_NO_TRANSFER_DMA_MAP
	                                            //                      URB_NO_SETUP_DMA_MAP
	                                            //                      URB_NO_FSBR
	                                            //                      URB_ZERO_PACKET
	                                            //                      URB_NO_INTERRUPT
	                                            //transfer_buffer  This identifies the buffer to (or from) which the I/O
	                                            //                 request will be performed (unless URB_NO_TRANSFER_DMA_MAP
	                                            //                 is set). This buffer must be suitable for DMA; allocate it
	                                            //                 with kmalloc or equivalent. For transfers to "in"
	                                            //                 endpoints, contents of this buffer will be modified. This
	                                            //                 buffer is used for the data stage of control transfers.
	                                            //transfer_dma     When transfer_flags includes URB_NO_TRANSFER_DMA_MAP, the
	                                            //                 device driver is saying that it provided this DMA address,
	                                            //                 which the host controller driver should use in preference
	                                            //                 to the transfer_buffer.
	                                            //transfer_buffer_length How big is transfer_buffer. The transfer may be broken
	                                            //                 up into chunks according to the current maximum packet size
	                                            //                 for the endpoint, which is a function of the configuration
	                                            //                 and is encoded in the pipe. When the length is zero, neither
	                                            //                 transfer_buffer nor transfer_dma is used.
	                                            //actual_length    This is read in non-iso completion functions, and it tells
	                                            //                 how many bytes (out of transfer_buffer_length) were transferred.
	                                            //                 It will normally be the same as requested, unless either an error
	                                            //                 was reported or a short read was performed. The URB_SHORT_NOT_OK
	                                            //                 transfer flag may be used to make such short reads be reported
	                                            //                 as errors.
	                                            //setup_packet     Only used for control transfers, this points to eight bytes of
	                                            //                 setup data. Control transfers always start by sending this data
	                                            //                 to the device. Then transfer_buffer is read or written, if needed.
	                                            //setup_dma        For control transfers with URB_NO_SETUP_DMA_MAP set, the device
	                                            //                 driver has provided this DMA address for the setup packet. The
	                                            //                 host controller driver should use this in preference to setup_packet.
	                                            //start_frame      Returns the initial frame for isochronous transfers.
	                                            //number_of_packets Lists the number of ISO transfer buffers.
	                                            //interval         Specifies the polling interval for interrupt or isochronous transfers.
	                                            //                 The units are frames (milliseconds) for for full and low speed devices,
	                                            //                 and microframes (1/8 millisecond) for highspeed ones.
	                                            //error_count      Returns the number of ISO transfers that reported errors.
	                                            //context          For use in completion functions. This normally points to request-specific
	                                            //                 driver context.
	                                            //complete         Completion handler. This URB is passed as the parameter to the completion
	                                            //                 function. The completion function may then do what it likes with the URB,
	                                            //                 including resubmitting or freeing it.
	                                            //iso_frame_desc[0] Used to provide arrays of ISO transfer buffers and to collect the transfer
	                                            //                 status for each buffer.

	struct ltqhcd_epqh       *epqh;
	struct ltqhcd_hc         *hc;                /*!< Host channel currently processing transfers for this EPQH. */
                                                // Actual data portion, not SETUP or STATUS in case of CTRL XFER
                                                // DMA adjusted
	uint8_t                  *setup_buff;       /*!< Pointer to the entire transfer buffer. (CPU accessable)*/
	uint8_t                  *xfer_buff;        /*!< Pointer to the entire transfer buffer. (CPU accessable)*/
	uint32_t                  xfer_len;         /*!< Total number of bytes to transfer in this xfer. */

	#if   defined(__UNALIGNED_BUF_ADJ__)
		uint8_t *aligned_setup;
		uint8_t *aligned_buf;
	#endif
	#if   defined(__UNALIGNED_BUF_ADJ__) || defined(__UNALIGNED_BUF_CHK__)
		unsigned aligned_checked :  1;
	#endif
	unsigned     is_in           :1;
	unsigned     cancelling      :1;
	int          cancelling_status;
	                          // For ALL XFER
	uint8_t                   error_count;    /*!< Holds the number of bus errors that have occurred for a transaction
	                                               within this transfer.
	                                           */
	                          // For ISOC XFER only
	#ifdef __EN_ISOC__
		int                   isoc_frame_index; /*!< Index of the next frame descriptor for an isochronous transfer. A
		                                             frame descriptor describes the buffer position and length of the
		                                             data to be transferred in the next scheduled (micro)frame of an
		                                             isochronous transfer. It also holds status for that transaction.
		                                             The frame index starts at 0.
		                                         */
	#endif
	int status;
} ltqhcd_urbd_t;

/*! typedef ltqhcd_epqh_t
 \brief A EP Queue Head (EPQH) holds the static characteristics of an endpoint and
 maintains a list of transfers (URBDs) for that endpoint. A EPQH structure may
 be entered in either the isoc, intr or non-periodic schedule.
 */

typedef struct ltqhcd_epqh {
	struct ltqhcd_hcd    *ltqhcd;
	struct usb_host_endpoint *sysep;
	uint8_t devno;

	ltqhcd_epqh_phase_e  phase;
	struct list_head     ql_all;
	struct list_head     ql;                // Hook for EP Queues
	struct list_head     urbd_list;         /*!< List of URBDs for this EPQH. */
	struct list_head     release_list;
	struct tasklet_struct  complete_urb_sub;
	uint8_t              ep_type;           /*!< Endpoint type. One of the following values:
	                                             - LTQUSB_EP_TYPE_CTRL
	                                             - LTQUSB_EP_TYPE_ISOC
	                                             - LTQUSB_EP_TYPE_BULK
	                                             - LTQUSB_EP_TYPE_INTR
	                                         */
	uint16_t             mps;               /*!< wMaxPacketSize Field of Endpoint Descriptor. */
	#ifdef __EPQD_DESTROY_TIMEOUT__
		struct timer_list destroy_timer;
	#endif

	unsigned burst_on       : 1 ;
	unsigned need_split     : 1 ;
	unsigned do_ping        : 1 ; /*!< Set to 1 to indicate that a PING request should be issued on this
	                                  channel. If 0, process normally.
	                              */
	unsigned period_do      : 1;
	unsigned cancelling     : 1;
	uint16_t             interval;          /*!< Interval between transfers in (micro)frames. (for INTR)*/
	uint16_t             period_counter;    /*!< Interval between transfers in (micro)frames. */

	#ifdef __EN_ISOC__
		struct tasklet_struct  tasklet_next_isoc;
		uint8_t               isoc_now;
		uint32_t              isoc_start_frame;
	                          // For SPLITed ISOC XFER only
		#ifdef __EN_ISOC_SPLIT__
			uint8_t           isoc_split_pos;   /*!< Position of the ISOC split on full/low speed */
			uint16_t          isoc_split_offset;/*!< Position of the ISOC split in the buffer for the current frame */
		#endif
	#endif
	spinlock_t	urbd_list_lock;
	spinlock_t	urbd_release_list_lock;
	int urbd_count;

	uint8_t complete_inuse;
} ltqhcd_epqh_t;


/*! typedef ltqhcd_hc_t
 \brief Host channel descriptor. This structure represents the state of a single
 host channel when acting in host mode. It contains the data items needed to
 transfer packets to an endpoint via a host channel.
 */
typedef struct ltqhcd_hc
{
	ltqusb_hc_regs_t *hc_regs;
	struct ltqhcd_epqh *epqh        ; /*!< EP Queue Head for the transfer being processed by this channel. */
	struct ltqhcd_urbd *urbd;              /*!< URBD currently assigned to a host channel for this EPQH. */
	uint8_t  hc_num                 ; /*!< Host channel number used for register address lookup */
	uint8_t *xfer_buff              ; /*!< Pointer to the entire transfer buffer. */
	uint32_t xfer_count             ; /*!< Number of bytes transferred so far. The offset of the begin of the buf */
	uint32_t xfer_len               ; /*!< Total number of bytes to transfer in this xfer. */
	uint16_t start_pkt_count        ; /*!< Packet count at start of transfer. Used to calculate the actual xfer size*/
	ltqhcd_halt_status_e halt_status; /*!< Reason for halting the host channel. */
	ltqhcd_hc_phase_e  phase;

	unsigned dev_addr       : 7; /*!< Device to access */
	unsigned is_in          : 1; /*!< EP direction. 0: OUT, 1: IN */
	unsigned ep_num         : 4; /*!< EP to access */
	unsigned speed          : 2; /*!< EP speed. */
	unsigned ep_type        : 2; /*!< Endpoint type. */
	unsigned mps            :11; /*!< Max packet size in bytes */
	unsigned data_pid_start : 2; /*!< PID for initial transaction. */
	unsigned short_rw       : 1; /*!< When Tx, means termination needed.
	                                  When Rx, indicate Short Read  */
	/* Split settings for the host channel */
	unsigned split          : 2; /*!< Split: 0-Non Split, 1-SSPLIT, 2&3 CSPLIT */

	unsigned sof_delay      :16;
	unsigned erron          : 1;
	unsigned cancelling     : 1;
	unsigned stopping       : 1;

	ltqhcd_control_phase_e    control_phase;  /*!< Current phase for control transfers (Setup, Data, or Status). */
	uint32_t ssplit_out_xfer_count; /*!< How many bytes transferred during SSPLIT OUT */
	#ifdef __DEBUG__
		uint32_t          start_hcchar_val;
	#endif
	uint32_t hcchar;

	/* Split settings for the host channel */
	uint8_t hub_addr;          /*!< Address of high speed hub */
	uint8_t port_addr;         /*!< Port of the low/full speed device */
	#if defined(__EN_ISOC__) && defined(__EN_ISOC_SPLIT__)
		uint8_t isoc_xact_pos;          /*!< Split transaction position */
	#endif
} ltqhcd_hc_t;


/*! typedef ltqhcd_hcd_t
 \brief This structure holds the state of the HCD, including the non-periodic and
 periodic schedules.
 */
typedef struct ltqhcd_hcd
{
	struct device *dev;
	struct hc_driver hc_driver;
	ltqusb_core_if_t core_if;   /*!< Pointer to the core interface structure. */
	struct usb_hcd *syshcd;
	int             vbusgpioid,vbusgpioinstalled,vbusgpiostatus;
	int             ledgpioid ,ledgpioinstalled ,ledgpiostatus;

	volatile union
	{
		uint32_t d32;
		struct
		{
			unsigned port_connect_status_change : 1;
			unsigned port_connect_status        : 1;
			unsigned port_reset_change          : 1;
			unsigned port_enable_change         : 1;
			unsigned port_suspend_change        : 1;
			unsigned port_over_current_change   : 1;
			unsigned reserved                   : 27;
		} b;
	} flags; /*!< Internal HCD Flags */

	struct ltqhcd_hc ltqhc[MAX_EPS_CHANNELS];         /*!< Array of pointers to the host channel descriptors. Allows accessing
	                                                       a host channel descriptor given the host channel number. This is
	                                                       useful in interrupt handlers.
	                                                   */
	uint8_t   *status_buf;                            /*!< Buffer to use for any data received during the status phase of a
	                                                       control transfer. Normally no data is transferred during the status
	                                                       phase. This buffer is used as a bit bucket.
	                                                   */
		#define LTQHCD_STATUS_BUF_SIZE 64             /*!< buffer size of status phase in CTRL xfer */

	struct list_head epqh_list_all;
	struct list_head epqh_list_ctrl;
	struct list_head epqh_list_bulk;
	struct list_head epqh_list_intr;
	#ifdef __EN_ISOC__
		struct list_head epqh_list_isoc;
	#endif

	uint32_t lastframe;

	#ifdef __EN_ISOC__
		uint8_t isoc_ep_count;
	#endif

	spinlock_t	epqh_list_lock;
	spinlock_t	epqh_list_all_lock;
	spinlock_t	enqueue_lock;
	spinlock_t	intr_lock;
	spinlock_t	select_lock;

	struct timer_list host_probe_timer;
	struct timer_list autoprobe_timer;

	unsigned	power_status;
	int		probe_sec;
	int		autoprobe_sec;
	#ifdef __DYN_SOF_INTR__
		uint32_t dyn_sof_count;
		#define DYN_SOF_COUNT_DEF 40000
	#endif
	struct tasklet_struct  tasklet_select_eps;        /*!<  Tasket to do a reset */
	struct tasklet_struct  tasklet_free_epqh_list ;        /*!<  Tasket to do a reset */
	unsigned disconnecting         : 1 ;
	unsigned singlexact            : 1 ;
	unsigned burst                 : 1 ;

	unsigned use_hrtimer           : 1 ;
	unsigned xfer_stopper_timer_on : 1 ;

	struct  hrtimer xfer_stopper_timer;

	int             xfer_prot1_usec;
	int             xfer_prot2_usec;
	uint16_t        xfer_prot1_value;
	unsigned long   xfer_prot2_value;

} ltqhcd_hcd_t;

/* Gets the ltqhcd_hcd from a struct usb_hcd */
static inline ltqhcd_hcd_t *syshcd_to_ltqhcd(struct usb_hcd *syshcd)
{
	return (ltqhcd_hcd_t *)(syshcd->hcd_priv[0]);
}

/* Gets the struct usb_hcd that contains a ltqhcd_hcd_t. */
static inline struct usb_hcd *ltqhcd_to_syshcd(ltqhcd_hcd_t *ltqhcd)
{
	return (struct usb_hcd *)(ltqhcd->syshcd);
}


extern ltqhcd_epqh_t * sysep_to_epqh(ltqhcd_hcd_t *_ltqhcd, struct usb_host_endpoint *_sysep, int skipcancel);

/* HCD Create/Destroy Functions */
	extern int  ltqhcd_init  (ltqhcd_hcd_t *_ltqhcd);
	extern void ltqhcd_remove(ltqhcd_hcd_t *_ltqhcd);

/*Linux HC Driver API Functions */

extern int  ltqhcd_start(struct usb_hcd *hcd);
extern void ltqhcd_stop (struct usb_hcd *hcd);
extern int  ltqhcd_get_frame_number(struct usb_hcd *hcd);


/*!
   \brief This function does the setup for a data transfer for a host channel and
  starts the transfer. May be called in either Slave mode or DMA mode. In
  Slave mode, the caller must ensure that there is sufficient space in the
  request queue and Tx Data FIFO.

  For an OUT transfer in Slave mode, it loads a data packet into the
  appropriate FIFO. If necessary, additional data packets will be loaded in
  the Host ISR.

  For an IN transfer in Slave mode, a data packet is requested. The data
  packets are unloaded from the Rx FIFO in the Host ISR. If necessary,
  additional data packets are requested in the Host ISR.

  For a PING transfer in Slave mode, the Do Ping bit is set in the HCTSIZ
  register along with a packet count of 1 and the channel is enabled. This
  causes a single PING transaction to occur. Other fields in HCTSIZ are
  simply set to 0 since no data transfer occurs in this case.

  For a PING transfer in DMA mode, the HCTSIZ register is initialized with
  all the information required to perform the subsequent data transfer. In
  addition, the Do Ping bit is set in the HCTSIZ register. In this case, the
  controller performs the entire PING protocol, then starts the data
  transfer.

  @param _ltqhc Information needed to initialize the host channel. The xfer_len
  value may be reduced to accommodate the max widths of the XferSize and
  PktCnt fields in the HCTSIZn register. The multi_count value may be changed
  to reflect the final xfer_len value.
 */
extern void ltqhcd_hc_start(ltqhcd_hcd_t *_ltqhcd, ltqhcd_hc_t *_ltqhc, uint8_t do_now);
extern void ltqhcd_xfer_stopper(ltqhcd_hcd_t *_ltqhcd);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
extern int ltqhcd_urb_enqueue(struct usb_hcd *_syshcd, struct usb_host_endpoint *_sysep, struct urb *_urb, gfp_t mem_flags);
extern int ltqhcd_urb_dequeue(struct usb_hcd *_syshcd, struct urb *_urb);
#else
extern int ltqhcd_urb_enqueue(struct usb_hcd *_syshcd, struct urb *_urb, gfp_t mem_flags);
extern int ltqhcd_urb_dequeue(struct usb_hcd *_syshcd, struct urb *_urb, int status);
#endif
extern irqreturn_t ltqhcd_irq(struct usb_hcd *_syshcd);

extern void ltqhcd_endpoint_disable(struct usb_hcd *_syshcd, struct usb_host_endpoint *_sysep);

extern int ltqhcd_hub_status_data(struct usb_hcd *_syshcd, char *_buf);
extern int ltqhcd_hub_control( struct usb_hcd *_syshcd,
                               u16             _typeReq,
                               u16             _wValue,
                               u16             _wIndex,
                               char           *_buf,
                               u16             _wLength);

/*@}*/

/*! \brief Transaction Execution Functions */
/*@{*/
extern void                      ltqhcd_complete_urb       (ltqhcd_hcd_t *_ltqhcd, ltqhcd_urbd_t *_urbd,  int _status);

/*!
   \brief Clears the transfer state for a host channel. This function is normally
  called after a transfer is done and the host channel is being released.
 */
extern void ltqhcd_hc_cleanup(ltqusb_core_if_t *_core_if, ltqhcd_hc_t *_ltqhc);

/*!
   \brief Attempts to halt a host channel. This function should only be called in
  Slave mode or to abort a transfer in either Slave mode or DMA mode. Under
  normal circumstances in DMA mode, the controller halts the channel when the
  transfer is complete or a condition occurs that requires application
  intervention.

  In DMA mode, always sets the Channel Enable and Channel Disable bits of the
  HCCHARn register. The controller ensures there is space in the request
  queue before submitting the halt request.

  Some time may elapse before the core flushes any posted requests for this
  host channel and halts. The Channel Halted interrupt handler completes the
  deactivation of the host channel.
 */
extern int ltqhcd_hc_halt(ltqusb_core_if_t *_core_if,
                    ltqhcd_hc_t *_ltqhc,
                    ltqhcd_halt_status_e _halt_status);

/*!
   \brief This function is called to handle the disconnection of host port.
 */
int32_t ltqhcd_disconnect(ltqhcd_hcd_t *_ltqhcd);
/*@}*/

/*!  \brief Interrupt Handler Functions */
/*@{*/
extern irqreturn_t ltqhcd_oc_irq(int _irq, void *_dev);

extern int32_t ltqhcd_handle_oc_intr(ltqhcd_hcd_t *_ltqhcd);
extern int32_t ltqhcd_handle_intr   (ltqhcd_hcd_t *_ltqhcd);
/*@}*/


/*! \brief Schedule Queue Functions */
/*@{*/
extern void           ltqhcd_epqh_free   (ltqhcd_epqh_t *_epqh);
extern void           select_eps      (ltqhcd_hcd_t *_ltqhcd);
extern void           ltqhcd_epqh_idle(ltqhcd_epqh_t *_epqh);
extern void           ltqhcd_epqh_idle_periodic(ltqhcd_epqh_t *_epqh);
extern ltqhcd_epqh_t *ltqhcd_urbd_create (ltqhcd_hcd_t *_ltqhcd,struct urb *_urb);
/*@}*/

/*! \brief Gets the usb_host_endpoint associated with an URB. */
static inline struct usb_host_endpoint *ltqhcd_urb_to_endpoint(struct urb *_urb)
{
	struct usb_device *dev = _urb->dev;
	int    ep_num = usb_pipeendpoint(_urb->pipe);

	return (usb_pipein(_urb->pipe))?(dev->ep_in[ep_num]):(dev->ep_out[ep_num]);
}

/*!
 * \brief Gets the endpoint number from a _bEndpointAddress argument. The endpoint is
 * qualified with its direction (possible 32 endpoints per device).
 */
#define ltqhcd_ep_addr_to_endpoint(_bEndpointAddress_) ((_bEndpointAddress_ & USB_ENDPOINT_NUMBER_MASK) | \
                                                       ((_bEndpointAddress_ & USB_DIR_IN) != 0) << 4)



/*! Internal debug function */
void         ltqhcd_dump_state(ltqhcd_hcd_t *_ltqhcd);

/*@}*//*LTQUSB_HCD*/

extern struct usb_device *usb_alloc_dev  (struct usb_device *parent, struct usb_bus *, unsigned port);
extern int                usb_add_hcd    (struct usb_hcd *syshcd, unsigned int irqnum, unsigned long irqflags);
extern void               usb_remove_hcd (struct usb_hcd *syshcd);
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
extern struct usb_hcd    *usb_create_hcd (const struct hc_driver *driver, struct device *dev, char *bus_name);
#else
extern struct usb_hcd    *usb_create_hcd (const struct hc_driver *driver, struct device *dev, const char *bus_name);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
extern void               usb_hcd_giveback_urb (struct usb_hcd *syshcd, struct urb *urb);
#else
extern void               usb_hcd_giveback_urb (struct usb_hcd *syshcd, struct urb *urb,int status);
#endif

extern void               usb_put_hcd       (struct usb_hcd *syshcd);
extern long               usb_calc_bus_time (int speed, int is_input, int isoc, int bytecount);
extern char *syserr(int errno);

extern void do_suspend_h(ltqhcd_hcd_t *_ltqhcd);
extern void do_resume_h(ltqhcd_hcd_t *_ltqhcd);
extern void do_bussuspend_h(ltqhcd_hcd_t *_ltqhcd);
extern void do_busresume_h(ltqhcd_hcd_t *_ltqhcd);


static inline void INIT_EPQH_LIST_ALL(ltqhcd_hcd_t   *_ltqhcd)
{
	spin_lock_init(&_ltqhcd->epqh_list_all_lock);
}
static inline void LOCK_EPQH_LIST_ALL(ltqhcd_hcd_t   *_ltqhcd)
{
	spin_lock(&_ltqhcd->epqh_list_all_lock);
}
static inline void UNLOCK_EPQH_LIST_ALL(ltqhcd_hcd_t *_ltqhcd)
{
	spin_unlock(&_ltqhcd->epqh_list_all_lock);
}

static inline void INIT_EPQH_LIST(ltqhcd_hcd_t   *_ltqhcd)
{
	spin_lock_init(&_ltqhcd->epqh_list_lock);
}
static inline void LOCK_EPQH_LIST(ltqhcd_hcd_t   *_ltqhcd)
{
	spin_lock(&_ltqhcd->epqh_list_lock);
}
static inline void UNLOCK_EPQH_LIST(ltqhcd_hcd_t *_ltqhcd)
{
	spin_unlock(&_ltqhcd->epqh_list_lock);
}

static inline void INIT_URBD_LIST(ltqhcd_epqh_t    *_epqh)
{
	spin_lock_init(&_epqh->urbd_list_lock);
}
static inline void LOCK_URBD_LIST(ltqhcd_epqh_t    *_epqh)
{
	spin_lock(&_epqh->urbd_list_lock);
}
static inline void UNLOCK_URBD_LIST(ltqhcd_epqh_t  *_epqh)
{
	spin_unlock(&_epqh->urbd_list_lock);
}

static inline void INIT_URBD_RELEASE_LIST(ltqhcd_epqh_t    *_epqh)
{
	spin_lock_init(&_epqh->urbd_release_list_lock);
}
static inline void LOCK_URBD_RELEASE_LIST(ltqhcd_epqh_t    *_epqh)
{
	spin_lock(&_epqh->urbd_release_list_lock);
}
static inline void UNLOCK_URBD_RELEASE_LIST(ltqhcd_epqh_t  *_epqh)
{
	spin_unlock(&_epqh->urbd_release_list_lock);
}
static inline void INIT_ENQUEUE(ltqhcd_hcd_t *_ltqhcd)
{
	spin_lock_init(&_ltqhcd->enqueue_lock);
}
static inline void LOCK_ENQUEUE(ltqhcd_hcd_t *_ltqhcd)
{
	spin_lock(&_ltqhcd->enqueue_lock);
}
static inline void UNLOCK_ENQUEUE(ltqhcd_hcd_t *_ltqhcd)
{
	spin_unlock(&_ltqhcd->enqueue_lock);
}
static inline void INIT_INTR(ltqhcd_hcd_t *_ltqhcd)
{
	spin_lock_init(&_ltqhcd->intr_lock);
}
static inline void LOCK_INTR(ltqhcd_hcd_t *_ltqhcd)
{
	spin_lock(&_ltqhcd->intr_lock);
}
static inline void UNLOCK_INTR(ltqhcd_hcd_t *_ltqhcd)
{
	spin_unlock(&_ltqhcd->intr_lock);
}
static inline void INIT_SELECT(ltqhcd_hcd_t *_ltqhcd)
{
	spin_lock_init(&_ltqhcd->select_lock);
}
static inline void LOCK_SELECT(ltqhcd_hcd_t *_ltqhcd)
{
	spin_lock(&_ltqhcd->select_lock);
}
static inline void UNLOCK_SELECT(ltqhcd_hcd_t *_ltqhcd)
{
	spin_unlock(&_ltqhcd->select_lock);
}



#define NEWRET 1

#define XFER_PROT1_CALC(n)  (3750L/125L*(125L-(n)))
#define XFER_PROT2_CALC(n)  ((n)*1000)
#define XFER_PROT2_OPTIONS HIGH_RES_NSEC

extern uint8_t ltqusb_chip_family        ;
	#define LTQUSB_CHIP_DANUBE 1
	#define LTQUSB_CHIP_ASE    2
	#define LTQUSB_CHIP_AR9    3
	#define LTQUSB_CHIP_VR9    4
	#define LTQUSB_CHIP_AR10   5


	/*!
	 \fn    void ltqusb_vbus_init(ltqhcd_hcd_t *_ltqhcd)
	 \brief This function init the VBUS control.
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
	extern void ltqusb_vbus_init(ltqhcd_hcd_t *_ltqhcd);

	/*!
	 \fn    void ltqusb_vbus_free(ltqhcd_hcd_t *_ltqhcd)
	 \brief This function free the VBUS control.
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
	extern void ltqusb_vbus_free(ltqhcd_hcd_t *_ltqhcd);

	/*!
	 \fn    void ltqusb_vbus_on(ltqusb_core_if_t *_core_if)
	 \brief Turn on the USB 5V VBus Power
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
	extern void ltqusb_vbus_on(ltqhcd_hcd_t *_ltqhcd);

	/*!
	 \fn    void ltqusb_vbus_off(ltqusb_core_if_t *_core_if)
	 \brief Turn off the USB 5V VBus Power
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
	extern void ltqusb_vbus_off(ltqhcd_hcd_t *_ltqhcd);

	/*!
	 \fn    int ltqusb_vbus(ltqusb_core_if_t *_core_if)
	 \brief Read Current VBus status
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
	extern int ltqusb_vbus(ltqhcd_hcd_t *_ltqhcd);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(__HOST_LED__)
	/*!
	 \fn    void ltqusb_led_init(ltqusb_core_if_t *_core_if)
	 \brief This function init the LED control.
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
	extern void ltqusb_led_init(ltqhcd_hcd_t *_ltqhcd);

	/*!
	 \fn    void ltqusb_led_free(ltqusb_core_if_t *_core_if)
	 \brief This function free the LED control.
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
	extern void ltqusb_led_free(ltqhcd_hcd_t *_ltqhcd);

	/*!
	 \fn    void ltqusb_led(ltqusb_core_if_t *_core_if)
	 \brief This function trigger the LED access.
	 \param _core_if        Pointer of core_if structure
	 \ingroup  LTQUSB_CIF
	 */
	extern void ltqusb_led(ltqhcd_hcd_t *_ltqhcd);
#endif


#endif // __LTQHCD_H__

