/******************************************************************************
**
** FILE NAME    : ltqmips_usif_spi.h
** PROJECT      : IFX UEIP for VRX200
** MODULES      : USIF for SPI
**
** DATE         : 21 Aug 2008
** AUTHOR       : Lei Chuanhua
** DESCRIPTION  : USIF for SPI Master/Slave
** COPYRIGHT    :       Copyright (c) 2008 ~ 2009
**                      Infineon Technologies AG
**                      Am Campeon 1-12, 85579 Neubiberg, Germany
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
** HISTORY
** $Date         $Author         $Comment
** 21 Aug, 2008  Lei Chuanhua    Initial version
** 20 Jun, 2009  Lei Chuanhua    Code review modification
*******************************************************************************/

#ifndef LTQMIPS_USIF_SPI_H
#define LTQMIPS_USIF_SPI_H

/*!
 \file ltqmips_usif_spi.h
 \ingroup LANTIQ_USIF_SPI
 \brief header file for ifx usif spi driver internal definition
*/

#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

#include <lantiq_usif_spi.h>
#include "lantiq_usif_reg.h"

/* Tail queue declarations */
#define TAILQ_HEAD(name, type)                                     \
struct name {                                                      \
    struct type *tqh_first;     /* first element */                \
    struct type **tqh_last;     /* addr of last next element */    \
}

#define TAILQ_HEAD_INITIALIZER(head)                               \
    { NULL, &(head).tqh_first }

#define TAILQ_ENTRY(type)                                              \
struct {                                                               \
    struct type *tqe_next;      /* next element */                     \
    struct type **tqe_prev;     /* address of previous next element */ \
}
/*
 * Tail queue functions.
 */
#define TAILQ_CONCAT(head1, head2, field) do {                          \
    if (!TAILQ_EMPTY(head2)) {                                          \
        *(head1)->tqh_last = (head2)->tqh_first;                        \
        (head2)->tqh_first->field.tqe_prev = (head1)->tqh_last;         \
        (head1)->tqh_last = (head2)->tqh_last;                          \
        TAILQ_INIT((head2));                                            \
    }                                                                   \
} while (0)

#define TAILQ_EMPTY(head)       ((head)->tqh_first == NULL)

#define TAILQ_FIRST(head)       ((head)->tqh_first)

#define TAILQ_FOREACH(var, head, field)                                  \
     for ((var) = TAILQ_FIRST((head));                                  \
        (var);                                                           \
        (var) = TAILQ_NEXT((var), field))

#define TAILQ_FOREACH_SAFE(var, head, field, tvar)                        \
    for ((var) = TAILQ_FIRST((head));                                   \
        (var) && ((tvar) = TAILQ_NEXT((var), field), 1);                \
        (var) = (tvar))

#define TAILQ_FOREACH_REVERSE(var, head, headname, field)                  \
    for ((var) = TAILQ_LAST((head), headname);                            \
        (var);                                                             \
        (var) = TAILQ_PREV((var), headname, field))

#define TAILQ_FOREACH_REVERSE_SAFE(var, head, headname, field, tvar)        \
    for ((var) = TAILQ_LAST((head), headname);                            \
        (var) && ((tvar) = TAILQ_PREV((var), headname, field), 1);        \
        (var) = (tvar))

#define TAILQ_INIT(head) do {                                               \
    TAILQ_FIRST((head)) = NULL;                                             \
    (head)->tqh_last = &TAILQ_FIRST((head));                                \
} while (0)

#define TAILQ_INSERT_AFTER(head, listelm, elm, field) do {                  \
    if ((TAILQ_NEXT((elm), field) = TAILQ_NEXT((listelm), field)) != NULL)\
        TAILQ_NEXT((elm), field)->field.tqe_prev =                          \
             &TAILQ_NEXT((elm), field);                                     \
    else {                                                                  \
        (head)->tqh_last = &TAILQ_NEXT((elm), field);                       \
    }                                                                       \
    TAILQ_NEXT((listelm), field) = (elm);                                   \
    (elm)->field.tqe_prev = &TAILQ_NEXT((listelm), field);                  \
} while (0)

#define TAILQ_INSERT_BEFORE(listelm, elm, field) do {                       \
    (elm)->field.tqe_prev = (listelm)->field.tqe_prev;                      \
    TAILQ_NEXT((elm), field) = (listelm);                                   \
    *(listelm)->field.tqe_prev = (elm);                                     \
    (listelm)->field.tqe_prev = &TAILQ_NEXT((elm), field);                  \
} while (0)

#define TAILQ_INSERT_HEAD(head, elm, field) do {                        \
    if ((TAILQ_NEXT((elm), field) = TAILQ_FIRST((head))) != NULL)     \
        TAILQ_FIRST((head))->field.tqe_prev =                           \
            &TAILQ_NEXT((elm), field);                                  \
    else                                                                \
        (head)->tqh_last = &TAILQ_NEXT((elm), field);                   \
    TAILQ_FIRST((head)) = (elm);                                        \
    (elm)->field.tqe_prev = &TAILQ_FIRST((head));                       \
} while (0)

#define TAILQ_INSERT_TAIL(head, elm, field) do {                           \
    TAILQ_NEXT((elm), field) = NULL;                                       \
    (elm)->field.tqe_prev = (head)->tqh_last;                              \
    *(head)->tqh_last = (elm);                                             \
    (head)->tqh_last = &TAILQ_NEXT((elm), field);                          \
} while (0)

#define TAILQ_LAST(head, headname)                                         \
    (*(((struct headname *)((head)->tqh_last))->tqh_last))

#define TAILQ_NEXT(elm, field) ((elm)->field.tqe_next)

#define TAILQ_PREV(elm, headname, field)                                    \
    (*(((struct headname *)((elm)->field.tqe_prev))->tqh_last))

#define TAILQ_REMOVE(head, elm, field) do {                                 \
    if ((TAILQ_NEXT((elm), field)) != NULL)                               \
         TAILQ_NEXT((elm), field)->field.tqe_prev =                         \
             (elm)->field.tqe_prev;                                         \
    else {                                                                  \
        (head)->tqh_last = (elm)->field.tqe_prev;                           \
    }                                                                       \
    *(elm)->field.tqe_prev = TAILQ_NEXT((elm), field);                      \
} while (0)

#define LANTIQ_USIF_SPI_WAKELIST_INIT(queue) \
	init_waitqueue_head(&queue)

#define LANTIQ_USIF_SPI_WAIT_EVENT_TIMEOUT(info, event, timeout)    \
	do {                                                     \
		wait_event_interruptible_timeout((info)->dma_wait,  \
		test_bit((event), &(info)->event_flags), (timeout)); \
		clear_bit((event), &(info)->event_flags); \
	} while (0)

/* Wait queue has no address symbol in this macro
 * interruptible_sleep_on() possibly cause lost-wakeup issue
 * wait_event_interruptible is robust for multiple events
 */
#define LANTIQ_USIF_SPI_WAIT_EVENT(queue, event, flags)  \
	do {                                              \
		wait_event_interruptible((queue),        \
		test_bit((event), &(flags)));            \
		clear_bit((event), &(flags));            \
		}while (0)

#define LANTIQ_USIF_SPI_WAKEUP_EVENT(queue, event, flags)     \
	do {                                               \
		set_bit((event), &(flags));                \
		wake_up_interruptible(&(queue));           \
	}while (0)


#define LANTIQ_USIF_SPI_IRQ_LOCK_INIT(port, _name) \
	spin_lock_init(&((port)->usif_irq_lock))

#define LANTIQ_USIF_SPI_IRQ_LOCK_DESTROY(port)
#define LANTIQ_USIF_SPI_IRQ_LOCK(port)  do {            \
	unsigned long __ilockflags;                     \
	spin_lock_irqsave(&((port)->usif_irq_lock), __ilockflags);
#define LANTIQ_USIF_SPI_IRQ_UNLOCK(port)               \
	spin_unlock_irqrestore(&((port)->usif_irq_lock), __ilockflags);\
} while (0)


#define LANTIQ_USIF_SPI_SEM_INIT(sem)   \
	sema_init(&sem, 1)

#define LANTIQ_USIF_SPI_SEM_LOCK(sem)   \
	down(&(sem))

#define LANTIQ_USIF_SPI_SEM_UNLOCK(sem) \
	up(&(sem))

/* Data may run in kernel thread, tasklet at the same time */
#define LANTIQ_USIF_SPI_Q_LOCK_INIT(port) \
	spin_lock_init(&(port)->q_lock)

#define LANTIQ_USIF_SPI_Q_LOCK_DESTROY(port)

#define LANTIQ_USIF_SPI_Q_LOCK_BH(port)  do {   \
	unsigned long __ilockflags;         \
	spin_lock_irqsave(&((port)->q_lock), __ilockflags);
#define LANTIQ_USIF_SPI_Q_UNLOCK_BH(port)       \
	spin_unlock_irqrestore(&((port)->q_lock), __ilockflags);\
} while (0)

#define CTL_AUTO CTL_UNNUMBERED

#define LANTIQ_INIT_CTL_NAME(val)
#define LANTIQ_SET_CTL_NAME(ctl, val)

#define LANTIQ_REGISTER_SYSCTL_TABLE(t) register_sysctl_table(t)

#define LANTIQ_USIF_SPI_MAX_PORT_NUM    1 /* assume default value */

#define LANTIQ_USIF_SPI_MAX_DEVNAME     16
#define LANTIQ_USIF_SPI_MAX_DEVICE      8 /* XXX */

/* Tx/Rx mode, FDF, HDF 2 bits */
#define LANTIQ_USIF_SPI_MODE_OFF         0
#define LANTIQ_USIF_SPI_MODE_RX          0x1
#define LANTIQ_USIF_SPI_MODE_TX          0x2
#define LANTIQ_USIF_SPI_MODE_RXTX        0x3
#define LANTIQ_USIF_SPI_MODE_RXTX_MASK   (LANTIQ_USIF_SPI_MODE_RX\
	| LANTIQ_USIF_SPI_MODE_TX)

/* Heading Control */
#define LANTIQ_USIF_SPI_LSB_FIRST            0
#define LANTIQ_USIF_SPI_MSB_FIRST            1

#define LANTIQ_USIF_SPI_CS_MAX               7

/* Parameters for USIF_SPI DMA device */
#define DEFAULT_USIF_SPI_TX_CHANNEL_CLASS   0
#define DEFAULT_USIF_SPI_RX_CHANNEL_CLASS   0

#define DEFAULT_USIF_SPI_TX_BURST_LEN       2 /* 2 words, 4 words, 8 words */
#define DEFAULT_USIF_SPI_RX_BURST_LEN       2 /* 2 words, 4 words, 8 words */

#define DEFAULT_USIF_SPI_TX_CHANNEL_NUM      1
#define DEFAULT_USIF_SPI_RX_CHANNEL_NUM      1

#define DEFAULT_USIF_SPI_TX_CHANNEL_DESC_NUM 1
#define DEFAULT_USIF_SPI_RX_CHANNEL_DESC_NUM 1

/* Default parameter */
#define LANTIQ_USIF_SPI_DEF_SYNC           1 /* Synchronous */
#define LANTIQ_USIF_SPI_DEF_MASTERSLAVE    1 /* Master */
#define LANTIQ_USIF_SPI_DEF_FRMCTL         0 /* Disable framing control */
#define LANTIQ_USIF_SPI_DEF_LOOP_BACK      0 /* Disable loopback */
#define LANTIQ_USIF_SPI_DEF_ECHO_MODE      0 /* Disable echo mode */
#define LANTIQ_USIF_SPI_DEF_TX_INVERT      0 /* Don't invert */
#define LANTIQ_USIF_SPI_DEF_RX_INVERT      0 /* Don't invert */
#define LANTIQ_USIF_SPI_DEF_SHIFT_CLK      0 /* No shift clock */
#define LANTIQ_USIF_SPI_DEF_CLK_PO         0 /* Idle Low */
/* 0: shift on leading edge, latch on trailling edge, 1, otherwise*/
#define LANTIQ_USIF_SPI_DEF_CLK_PH         1
#define LANTIQ_USIF_SPI_DEF_TX_IDLE_STATE  0 /* Low */
#define LANTIQ_USIF_SPI_DEF_MODE_RXTX     LANTIQ_USIF_SPI_MODE_RXTX /* TX/RX modes */
#define LANTIQ_USIF_SPI_DEF_CHAR_LEN       LANTIQ_USIF_PRTC_CFG_CLEN8
#define LANTIQ_USIF_SPI_DEF_HDR_CTRL       LANTIQ_USIF_SPI_MSB_FIRST
#define LANTIQ_USIF_SPI_DEF_WAKEUP         0

#ifdef CONFIG_USE_EMULATOR
#define LANTIQ_USIF_SPI_DEF_BAUDRATE        10000
#else
#define LANTIQ_USIF_SPI_DEF_BAUDRATE        2000000
#endif
#define LANTIQ_USIF_SPI_DEF_RMC             0x01 /* To use it up to max value */
#define LANTIQ_USIF_SPI_DEF_ORMC            0x01

/*!
 \addtogroup LANTIQ_USIF_SPI_DEFINITIONS
*/
/* @{ */

/*! \def DEFAULT_USIF_SPI_FRAGMENT_SIZE
    \brief a maximum fragment size parameter is defined for all connections.

    This threshold is given in number of bytes. It has to be configured for
    each connection and be less or equal. If the size of the packet is greater
    than this threshold the application has to take care of the fragmentation
    of the packet. The default global value is given with 1056 bytes. This is
    the recommended value to use, but each connection is free configure this
    parameter to a lesser value individually
  */
#define DEFAULT_USIF_SPI_FRAGMENT_SIZE    1056

/*! \def LANTIQ_USIF_SPI_MAX_FRAGSIZE
    \brief maximum fragment size in theory.
  */
#define LANTIQ_USIF_SPI_MAX_FRAGSIZE             (32 * 1024)

/*! \def LANTIQ_USIF_SPI_MIN_FRAGSIZE
    \brief Mimimum fragment size. Smaller than this size has no meaning .
  */
#define LANTIQ_USIF_SPI_MIN_FRAGSIZE             32

/*! \def DEFAULT_USIF_SPI_FIFO_THRESHOULD
    \brief For enabling the USIF_SPI driver to choose between FIFO and DMA mode
    in HDX communication a threshold parameter is set for all connections

    This threshold is given in number of bytes. If the size of the packet is
    lesser than this threshold the FIFO mode is used, else the DMA mode. The
    global default value of 148 bytes is recommended. This value has to be
    optimized/tuned accoring to application
  */
#define DEFAULT_USIF_SPI_FIFO_THRESHOULD  148

/*! \def LANTIQ_USIF_SPI_FIFO_MAX_THRESHOULD
    \brief Maximum FIFO/DMA threshould
  */
#define LANTIQ_USIF_SPI_FIFO_MAX_THRESHOULD      600

/*! \def LANTIQ_USIF_SPI_FIFO_MIN_THRESHOULD
    \brief Mimimum FIFO/DMA threshould.
    \note  This thresould must be more than DMA burst length
  */
#define LANTIQ_USIF_SPI_FIFO_MIN_THRESHOULD      32

/**
 * TX FIFO burst size is set to 2 words, update accordingly
 * when 'LANTIQ_USIF_SPI_TXFIFO_SETUP' changed
 * NB, Must be the same as DMA burst length
 */
#define LANTIQ_USIF_SPI_TX_BUSRT_SIZE            0x02
#define LANTIQ_USIF_SPI_TX_BUSRT_SIZE_BYTES      0x08 /* 8 bytes */

/**
 * TX FIFO is byte aligned, so one word contains 4 characters,
 *   update accordingly when 'LANTIQ_USIF_SPI_TXFIFO_SETUP' changed
 */
#define LANTIQ_USIF_SPI_TX_FIFO_CHARS_PER_WORD   0x04

/**
 * RX FIFO burst size is set to 2 words, update accordingly
 * when 'LANTIQ_USIF_SPI_RXFIFO_SETUP' changed
 * NB, Must be the same as DMA burst length
 */
#define LANTIQ_USIF_SPI_RX_BUSRT_SIZE            0x02
#define LANTIQ_USIF_SPI_RX_BUSRT_SIZE_BYTES      0x08 /* 8 bytes */
/**
 * RX FIFO is byte aligned, so one word contains 4 characters,
 *   update accordingly when 'LANTIQ_USIF_SPI_RXFIFO_SETUP' changed
 */
#define LANTIQ_USIF_SPI_RX_FIFO_CHARS_PER_WORD   0x04

/**
 * TX FIFO is setup as flow control mode, with byte alignemnet and
 *  busrt size as 2 words
 */
#define LANTIQ_USIF_SPI_TXFIFO_SETUP           \
	(/*LANTIQ_USIF_FIFO_CFG_TXFC |*/\
	SM(LANTIQ_USIF_TXFIFO_ALIGN_BYTE, LANTIQ_USIF_FIFO_CFG_RXFA) \
	| SM(LANTIQ_USIF_TXFIFO_BURST_WORD2, LANTIQ_USIF_FIFO_CFG_TXBS))

/*
 * RX FIFO is setup as flow control mode, with byte alignemnet and
 * busrt size as 2 words
 */
#define LANTIQ_USIF_SPI_RXFIFO_SETUP            \
	(LANTIQ_USIF_FIFO_CFG_RXFC |\
	SM(LANTIQ_USIF_RXFIFO_ALIGN_BYTE, LANTIQ_USIF_FIFO_CFG_RXFA) \
	| SM(LANTIQ_USIF_RXFIFO_BURST_WORD2, LANTIQ_USIF_FIFO_CFG_RXBS))

#define LANTIQ_USIF_SPI_FIFO_SETUP  (LANTIQ_USIF_SPI_TXFIFO_SETUP |\
	LANTIQ_USIF_SPI_RXFIFO_SETUP)

/*!
  \brief USIF_SPI Statistics.
  Symbolic constants to be used in USIF_SPI routines
 */
typedef struct lantiq_usif_spi_statistics {
	u32 abortErr; /*!< abort error */
	u32 modeErr; /*!< master/slave mode error */
	u32 txOvErr; /*!< TX Overflow error */
	u32 txUnErr; /*!< TX Underrun error */
	u32 rxOvErr; /*!< RX Overflow error */
	u32 rxUnErr; /*!< RX Underrun error */
	u64 rxBytes; /*!< Received bytes */
	u64 txBytes; /*!< Transmitted bytes */
	u64 txFifo; /*!< Transmit transactions in FIFO */
	u64 txDma; /*!< Transmit transactions in DMA */
	u64 txDmaBytes; /*!< Transmit bytes in DMA mode */
	u64 rxFifo; /*!< Receive transactions in FIFO */
	u64 rxDma; /*!< Receive transactions in FIFO */
	u64 rxDmaBytes; /*!< Receive bytes in DMA mode */
} LANTIQ_USIF_SPI_STAT_t;

/*!
  \brief USIF_SPI Hardware Options.
 */
struct lantiq_usif_hwopts {
	unsigned int sync:1;  /*!< Synchronous or Asynchronous */
	unsigned int master:1; /*!< Master or slave for sync mode */
	unsigned int frmctl:1; /*!< Framing control enable or disable */
	unsigned int loopBack:1; /*!< Loopback mode */
	unsigned int echoMode:1; /*!< Echo mode */
	unsigned int txInvert:1; /*!< Transmissin line invert */
	unsigned int rxInvert:1; /*!< Receive line invert */
	unsigned int shiftClk:1; /*!< Shift clock force */
	unsigned int clkPO:1; /*!< Shift clock polarity configuration */
	unsigned int clkPH:1; /*!< Shift clock phase configuration */
	unsigned int txIdleValue:1; /*!< Tx idle state */
	unsigned int modeRxTx:2; /*!< Rx/tx mode */
	unsigned int charLen:5; /*!< Character length */
	unsigned int hdrCtrl:1; /*!< Heading of data bits */
	unsigned int wakeUp:1; /*!< Wake-up bit detection */
};

struct lantiq_usif_device;

/*! typedef LANTIQ_USIF_QUEUE
    \brief queue structure for sync or async API
*/
typedef struct LANTIQ_USIF_QUEUE {
	struct lantiq_usif_device *dev; /*!< Back pointer to SSC client device */
	bool request_lock; /*!< Indicates this queue locked or not */
	atomic_t isqueued; /*!< Only one entry is allowed per device
				shared between tasklet and kernel thread
				 */
	char *txbuf; /*!< Transmission buffer snapshot */
	int txsize; /*!< Transmission size snaphot */
	char *rxbuf; /*!< Reception buffer snapshot */
	int rxsize; /*!< Receptin size snapshot */
	int exchange_bytes; /*!< TX/RX bytes for callback function */
	/*!< Sync/Async handle type */
	LANTIQ_USIF_SPI_HANDL_TYPE_t handle_type;
	/*!< Async API callback function */
	LANTIQ_USIF_SPI_ASYNC_CALLBACK_t callback;
	/*!< Used if it works as queue header */
	 TAILQ_ENTRY(LANTIQ_USIF_QUEUE) q_next;
} LANTIQ_USIF_QUEUE_t;

enum {
	LANTIQ_USIF_SPI_DIR_RX = 0,
	LANTIQ_USIF_SPI_DIR_TX,
	LANTIQ_USIF_SPI_DIR_TXRX,
};

enum {
	LANTIQ_USIF_SPI_DMA_DISABLE = 0,
	LANTIQ_USIF_SPI_DMA_ENABLE,
};

/*!
  \brief USIF SPI bus device statistics
 */
struct usif_device_stats {
	u64 rxBytes;  /*!< Received bytes */
	u64 txBytes;  /*!< Transmitted bytes */
	u32 dup_qentries; /*!< Duplicated qentry error */
	u32 context_err;  /*!< Context error */
	u32 frag_err;     /*!< Fragment error */
	u32 handler_err; /*!< Handler error */
	u32 dlx_err; /*!< Duplex error */
	u64 enqueue; /*!< enqueue times */
	u64 dequeue; /*!< dequeue times */
	/* More ... */
};

/*! typedef lantiq_usif_device_t
    \brief USIF SPI device structure defintion
*/
typedef struct lantiq_usif_device {
	char dev_name[LANTIQ_USIF_SPI_MAX_DEVNAME];
	int duplex;
	struct lantiq_usif_port *port; /* Back pointer */
	 TAILQ_ENTRY(lantiq_usif_device) dev_entry;
	LANTIQ_USIF_SPI_CONFIGURE_t conn_id;
	struct usif_device_stats stats;
#ifdef CONFIG_SYSCTL
	struct ctl_table_header *usif_sysctl_header;
	struct ctl_table *usif_sysctls;
#endif /* CONFIG_SYSCTL */
	LANTIQ_USIF_QUEUE_t queue;
	LANTIQ_USIF_SPI_PRIO_t dev_prio;

	/* Kernel thread based per device <ssc client> */
	wait_queue_head_t dev_thread_wait;
	volatile long event_flags;
#define LANTIQ_USIF_SPI_DEV_THREAD_EVENT            1
} lantiq_usif_device_t;

typedef TAILQ_HEAD(, lantiq_usif_device) usif_devq_t;
typedef TAILQ_HEAD(, LANTIQ_USIF_QUEUE) usif_dataq_t;

/*! typedef lantiq_usif_port_t
    \brief USIF SPI logical port definition
*/
typedef struct lantiq_usif_port {
	unsigned char __iomem *membase; /* Virtual */
	unsigned long mapbase; /* Physical */
	struct lantiq_usif_hwopts opts;
	LANTIQ_USIF_SPI_STAT_t stats;
	u32 usif_debug; /* debug msg flags */
	u32 tx_fifo_size_words;
	u32 tx_fifo_size_bytes;
	u32 rx_fifo_size_words;
	u32 rx_fifo_size_bytes;
	/* USIF_SPI port index, actually, it means
	   how many spi ports we have. */
	int port_idx;
	usif_devq_t usif_devq;
	lantiq_usif_device_t *current_dev; /* Current calling dev in the port */
	int usif_ndevs;
	char name[LANTIQ_USIF_SPI_MAX_DEVNAME];
	struct semaphore dev_sem;
	unsigned int prev_baudrate;
	unsigned int baudrate;
	/* Pointers to rx position */
	volatile char *rxbuf_ptr;
	int rx_len;

	volatile long event_flags;
	struct dma_device_info *dma_dev; /* DMA device structure */
	LANTIQ_USIF_SPI_MODE_t prev_spi_mode;
	int usif_frag_size; /* Possible configuration change, void malloc
			     * big memory in reception
			     */
	LANTIQ_USIF_QUEUE_t *lock_qentry; /* XXX, protection? */
	/* Only in its own tasklet or kernel thread */
	LANTIQ_USIF_QUEUE_t *serve_qentry;
	usif_dataq_t usif_asyncq; /* Async queue header */
	usif_dataq_t usif_syncq[LANTIQ_USIF_SPI_PRIO_MAX]; /* Sync queue header */
	spinlock_t q_lock; /* on three queues */
	int usif_thread;
	int usif_fake_irq;
	spinlock_t usif_irq_lock;
	atomic_t dma_wait_state;
	wait_queue_head_t usif_thread_wait;
#define LANTIQ_USIF_SPI_THREAD_EVENT       1

	struct task_struct *usif_tsk;
	bool usif_cs_locked; /* Done in the same tasklet or thread only */
	struct tasklet_struct usif_txrxq;
	/* HW WAR DMA buffer */
	char *dma_txbuf; /* Aligned buffer */
	char *dma_orig_txbuf; /* Original buffer */
	char *dma_rxbuf;  /* Aligned buffer */
	char *dma_orig_rxbuf; /* Original buffer */
#ifdef CONFIG_SYSCTL
	struct ctl_table_header *port_sysctl_header;
	struct ctl_table *port_sysctls;
#endif /* CONFIG_SYSCTL */
	u32 dma_is_in_half_duplex;
} lantiq_usif_port_t;
/* @} */

#ifdef CONFIG_USIF_SPI_XRX200
#include "lantiq_usif_spi_vr9.h"
#elif defined CONFIG_USIF_SPI_XRX300
#include "lantiq_usif_spi_ar10.h"
#else
#error "USIF platform not defined!!!!"
#endif
#endif /* LTQMIPS_USIF_SPI_H */

