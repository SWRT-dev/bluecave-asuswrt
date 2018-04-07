/******************************************************************************
**
** FILE NAME    : ltqmips_usif_uart.h
** PROJECT      : IFX UEIP
** MODULES      : Serial driver for USIF
**
** DATE         : 06 Aug, 2008
** AUTHOR       : Lei Chuanhua
** DESCRIPTION  : USIF module for uart driver
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
** 06 Aug, 2008  Lei Chuanhua    Initial version
** 26 Jun, 2009  Lei Chuanhua    Code Review modification
*******************************************************************************/

#ifndef LTQMIPS_USIF_UART_H
#define LTQMIPS_USIF_UART_H

#include "lantiq_usif_reg.h"
#include <lantiq_soc.h>

/*!
 \file ltqmips_usif_uart.h
 \ingroup LANTIQ_USIF_UART
 \brief header file for ifx usif uart driver internal definition
*/
/*!
 \addtogroup LANTIQ_USIF_UART_DEFINITIONS
*/
/* @{ */
#if defined(CONFIG_SERIAL_LANTIQ)

/*! \def LANTIQ_USIF_UART_MINOR
    \brief define the USIF UART minor number

    2nd device, can't use ttyS0/1. Use ttyUSIF instead
    Major number is the same, USIF minor device number
    minor 64/65 has been used by ASC
 */
#define LANTIQ_USIF_UART_MINOR       66
/*! \def LANTIQ_USIF_UART_DEV_NAME
    \brief define the USIF UART device name in /dev/

    2nd device, can't use ttyS0/1. Use ttyUSIF instead
 */
#define LANTIQ_USIF_UART_DEV_NAME    "ttyUSIF"
#else
/* 1st device, ttyS0, as UART ports are wholy used by USIF */
#define LANTIQ_USIF_UART_MINOR       64
#define LANTIQ_USIF_UART_DEV_NAME    "ttyS"
#endif /* CONFIG_SERIAL_IFX_ASC */

/*! \def LANTIQ_USIF_UART_MAJOR
    \brief USIF major device number
 */
#define LANTIQ_USIF_UART_MAJOR      4

/*! \def LANTIQ_USIF_UART_NR
    \brief Number of USIF devices in USIF-UART
 */
#define LANTIQ_USIF_UART_NR              1

/*! \def LANTIQ_USIF_UART_DEFAULT_RMC
    \brief Default RMC
 */
#define LANTIQ_USIF_UART_DEFAULT_RMC     1

/*! \def LANTIQ_USIF_UART_DEFAULT_ORMC
    \brief Default ORMC
 */
#define LANTIQ_USIF_UART_DEFAULT_ORMC    1

/*! \def LANTIQ_USIF_UART_TX_FIFO_LENGTH
    \brief  TX FIFO size, update accordingly if changed 32 bytes, 8DWs
 */
#define LANTIQ_USIF_UART_TX_FIFO_LENGTH       0x20

/*! \def LANTIQ_USIF_UART_TX_BUSRT_SIZE
    \brief  TX FIFO burst size is set to 4 words, update accordingly
    when 'LANTIQ_USIF_UART_TXFIFO_SETUP' changed
*/
#define LANTIQ_USIF_UART_TX_BUSRT_SIZE        0x04

/*! \def LANTIQ_USIF_UART_TX_FIFO_CHARS_PER_WORD
    \brief TX FIFO is byte aligned, so one word contains 4 characters
     update accordingly when 'LANTIQ_USIF_UART_TXFIFO_SETUP' changed
*/
#define LANTIQ_USIF_UART_TX_FIFO_CHARS_PER_WORD   0x04

/*! \def LANTIQ_USIF_UART_RX_BUSRT_SIZE
    \brief RX FIFO burst size is set to 4 words, update accordingly
     when 'LANTIQ_USIF_UART_RXFIFO_SETUP' changed
*/
#define LANTIQ_USIF_UART_RX_BUSRT_SIZE           0x04

/*! \def LANTIQ_USIF_UART_RX_FIFO_CHARS_PER_WORD
    \brief  RX FIFO is byte aligned, so one word contains 4 characters,
     update accordingly when 'LANTIQ_USIF_UART_RXFIFO_SETUP' changed
*/
#define LANTIQ_USIF_UART_RX_FIFO_CHARS_PER_WORD   0x04


/* The following is used while checking the error for received characters */
#define LANTIQ_USIF_UART_RX_GOOD_CHAR              0x00
#define LANTIQ_USIF_UART_RX_IGNORE_CHAR            0x01

/* Macro defintation used while transmit in progress */
#define LANTIQ_USIF_UART_TX_PROGRESS_ACTIVE        0x01
#define LANTIQ_USIF_UART_TX_PROGRESS_INACTIVE      0x00

/*! \def LANTIQ_USIF_UART_TXFIFO_SETUP
    \brief  TX FIFO is setup as flow control mode, with byte alignemnet and
     busrt size as 4 (half of the FIFO size) words
*/
#define LANTIQ_USIF_UART_TXFIFO_SETUP           \
	(LANTIQ_USIF_FIFO_CFG_TXFC | SM(LANTIQ_USIF_TXFIFO_ALIGN_BYTE,\
	LANTIQ_USIF_FIFO_CFG_TXFA) | SM(LANTIQ_USIF_TXFIFO_BURST_WORD4,\
	LANTIQ_USIF_FIFO_CFG_TXBS))

/*! \def LANTIQ_USIF_UART_RXFIFO_SETUP
    \brief  RX FIFO is setup as flow control mode, with byte alignemnet and
    busrt size as 4 (half of the FIFO size) words
*/
#define LANTIQ_USIF_UART_RXFIFO_SETUP                 \
	(LANTIQ_USIF_FIFO_CFG_RXFC | SM(LANTIQ_USIF_RXFIFO_ALIGN_BYTE,\
	LANTIQ_USIF_FIFO_CFG_RXFA) | SM(LANTIQ_USIF_RXFIFO_BURST_WORD4,\
	LANTIQ_USIF_FIFO_CFG_RXBS))


#define LANTIQ_USIF_UART_FIFO_SETUP  (LANTIQ_USIF_UART_TXFIFO_SETUP |\
	LANTIQ_USIF_UART_RXFIFO_SETUP)

/* Baudrate supported */
#define LANTIQ_USIF_BAUD_115200    115200
#define LANTIQ_USIF_BAUD_57600     57600
#define LANTIQ_USIF_BAUD_38400     38400
#define LANTIQ_USIF_BAUD_19200     19200
#define LANTIQ_USIF_BAUD_9600      9600
#define LANTIQ_USIF_BAUD_4800      4800
#define LANTIQ_USIF_BAUD_2400      2400
#define LANTIQ_USIF_BAUD_1200      1200
#define USIF_BAUD_300       300
#define LANTIQ_USIF_BAUD_MIN       (USIF_BAUD_300)

/*! \def IFX_PORT_USIF_UART
    \brief  Identification number for USIF driver in serial core, it has been
    used till 62 Remember to move the following two lines to
    linux/serial_core.h
*/
#define IFX_PORT_USIF_UART   112

/* baudrates */
typedef enum {
	LANTIQ_USIF_HW_BPS_300,
	LANTIQ_USIF_HW_BPS_1200,
	LANTIQ_USIF_HW_BPS_2400,
	LANTIQ_USIF_HW_BPS_4800,
	LANTIQ_USIF_HW_BPS_9600,
	LANTIQ_USIF_HW_BPS_19200,
	LANTIQ_USIF_HW_BPS_38400,
	LANTIQ_USIF_HW_BPS_57600,
	LANTIQ_USIF_HW_BPS_115200,
	LANTIQ_USIF_HW_BPS_NUM_BAUDRATES
} LANTIQ_USIF_HW_BAUDRATE_IDX_t;

typedef struct lantiq_usif_baudrate_desc {
	u32 inc;
	u32 dec;
	u32 bc;
	u32 clc_rmc;
	u8 rtstl; /* flow control ttl level */
	u32 tmo; /* timeout in shift clock cycles */
	u32 tm;  /* Configure timer value */
	u32 tsp;
} LANTIQ_USIF_BAUDRATE_DESC_t;

typedef struct lantiq_usif_hw_baudtable {
	unsigned long max_baudrate;
	LANTIQ_USIF_BAUDRATE_DESC_t fpi[LANTIQ_USIF_HW_BPS_NUM_BAUDRATES];
} LANTIQ_USIF_HW_BAUDTABLE_t;

#define LANTIQ_USIF_UART_DMA_DB_SHORT_CONST    71
#define LANTIQ_USIF_HW_SHORT_TIMEOUT_300 (2036 * LANTIQ_USIF_UART_DMA_DB_SHORT_CONST)
#define LANTIQ_USIF_HW_SHORT_TIMEOUT_1200 (768 * LANTIQ_USIF_UART_DMA_DB_SHORT_CONST)
#define LANTIQ_USIF_HW_SHORT_TIMEOUT_2400 (384 * LANTIQ_USIF_UART_DMA_DB_SHORT_CONST)
#define LANTIQ_USIF_HW_SHORT_TIMEOUT_4800 (192 * LANTIQ_USIF_UART_DMA_DB_SHORT_CONST)
#define LANTIQ_USIF_HW_SHORT_TIMEOUT_9600  (96 * LANTIQ_USIF_UART_DMA_DB_SHORT_CONST)
#define LANTIQ_USIF_HW_SHORT_TIMEOUT_19200 (48 * LANTIQ_USIF_UART_DMA_DB_SHORT_CONST)
#define LANTIQ_USIF_HW_SHORT_TIMEOUT_38400 (24 * LANTIQ_USIF_UART_DMA_DB_SHORT_CONST)
#define LANTIQ_USIF_HW_SHORT_TIMEOUT_57600 (16 * LANTIQ_USIF_UART_DMA_DB_SHORT_CONST)
#define LANTIQ_USIF_HW_SHORT_TIMEOUT_115200 (8 * LANTIQ_USIF_UART_DMA_DB_SHORT_CONST)

/**
 * Interrupt type. The SA_INTERRUPT will block all other interrupts till the
 * calling interrupt is serviced. This is required, as the receive interrupt
 * will call the 'tty_flip_buffer_push' utility and does not want to service
 * the transmit interrupt in-between.
 */

#define LANTIQ_USIF_UART_IRQ_LOCK_INIT(port, _name)
#define LANTIQ_USIF_UART_IRQ_LOCK_DESTROY(port)

#define LANTIQ_USIF_UART_IRQ_LOCK(port) do {                  \
	unsigned long __ilockflags;                        \
	spin_lock_irqsave(&((port)->lock), __ilockflags);

#define LANTIQ_USIF_UART_IRQ_UNLOCK(port)                     \
	spin_unlock_irqrestore(&((port)->lock), __ilockflags); \
} while (0)

typedef struct lantiq_usif_uart_proc_stat {
	__u32 tx_intr_breq;
	__u32 tx_intr_lbreq;
	__u32 tx_intr_sreq;
	__u32 tx_intr_lsreq;
	__u32 tx_intr_txfin;
	__u32 tx_start_req;
	__u32 tx_stop_req;
	__u32 tx_write_fifo;
	__u32 tx_bytes;
	__u32 tx_stopped;
	__u32 tx_req_fifo_full;

	__u32 rx_intr_breq;
	__u32 rx_intr_lbreq;
	__u32 rx_intr_sreq;
	__u32 rx_intr_lsreq;
	__u32 rx_read_fifo;
	__u32 rx_last_rps;
	__u32 rx_low_latency;
	__u32 rx_bytes;

	__u32 err_int_rxur;
	__u32 err_int_txur;
	__u32 err_int_txof;
	__u32 err_int_phe;
	__u32 err_int_bre;
	__u32 err_int_crc;
} LANTIQ_USIF_UART_PROC_STAT_t;

/* Wrap usif uart port structure around the generic uart_port */
typedef struct lantiq_usif_uart_port {
	struct uart_port port;	/* Must be the first */
	u32 tx_progress;
	u32 rx_rps_cnt:13; /* Record the count of data read already */
	u32 prev_baud;   /* Record the previous baud rate to speed up config */
	u32 prev_mode_cfg; /* Record the previous mode configration to speed
			      up config */
	u32 prev_prtc_cfg; /* Record the previous protocol to speed up config*/
	u8 tx_irq;   /* TX interrupt */
	u8 rx_irq;   /* RX interrupt */
	u8 wakeup_irq; /* Wakeup interrupt */
	u8 autobaud_irq; /* Autoband interrupt */
	u8 status_irq; /* Status interrupt for modem */
	u8 err_irq;    /* Error interrupt */
	u32 debug;
	LANTIQ_USIF_UART_PROC_STAT_t usif_proc_data;
} LANTIQ_USIF_UART_PORT_t;

/* Structure for FDIV_CFG, fractional divider configuration register */
typedef union lantiq_usif_baudrate_fdiv {
	struct {
#if defined(__BIG_ENDIAN)
		u32 dec:16;
		u32 dummy:7;
		u32 inc:9;
#else
		u32 inc:9;
		u32 dummy:7;
		u32 dec:16;
#endif /* __BIG_ENDIAN */
	} fdiv_cfg_bits;
	u32 fdiv_cfg_word;
} LANTIQ_USIF_BAUDRATE_FDIV_t;

#define INT_NUM_IM3_IRL3 		   (INT_NUM_IM3_IRL0 + 3)
#define INT_NUM_IM3_IRL4 		   (INT_NUM_IM3_IRL0 + 4)
#define INT_NUM_IM3_IRL5 		   (INT_NUM_IM3_IRL0 + 5)
#define INT_NUM_IM3_IRL6 		   (INT_NUM_IM3_IRL0 + 6)
#define INT_NUM_IM3_IRL21 		   (INT_NUM_IM3_IRL0 + 21)
#define INT_NUM_IM0_IRL21 		   (INT_NUM_IM0_IRL0 + 21)

#define LANTIQ_USIF_EIR_INT                INT_NUM_IM3_IRL3
#define LANTIQ_USIF_STA_INT                INT_NUM_IM3_IRL4
#define LANTIQ_USIF_AB_INT                 INT_NUM_IM3_IRL5
#define LANTIQ_USIF_WKP_INT                INT_NUM_IM3_IRL6
#define LANTIQ_USIF_TX_INT                 INT_NUM_IM0_IRL21
#define LANTIQ_USIF_RX_INT                 INT_NUM_IM3_IRL21

/* @} */
#endif /* LTQMIPS_USIF_UART_H */

