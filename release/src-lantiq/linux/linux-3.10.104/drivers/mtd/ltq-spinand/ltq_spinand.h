/*
 * (C) Copyright 2016 Intel Corporation
 * Author: William Widjaja <w.widjaja@intel.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
 
#ifndef __LTQ_MTD_SPI_NAND_H__
#define __LTQ_MTD_SPI_NAND_H__

#include <linux/wait.h>
#include <linux/spinlock.h>
#include <linux/mtd/mtd.h>

/*============================================================================*/

/*
 * Standard SPI-NAND flash commands
 */
#define CMD_READ                        0x13
#define CMD_READ_RDM                    0x03
#define CMD_PROG_PAGE_CLRCACHE          0x02
#define CMD_PROG_PAGE                   0x84
#define CMD_PROG_PAGE_EXC               0x10
#define CMD_ERASE_BLK                   0xd8
#define CMD_WR_ENABLE                   0x06
#define CMD_WR_DISABLE                  0x04
#define CMD_READ_ID                     0x9f
#define CMD_RESET                       0xff
#define CMD_READ_REG                    0x0f
#define CMD_WRITE_REG                   0x1f
#define CMD_READ_CACHE_X2               0x3b
#define CMD_READ_CACHE_X4               0x6b

/* feature registers */
#define REG_BLOCK_LOCK                  0xa0
#define REG_OTP                         0xb0
#define REG_STATUS                      0xc0 /* timing */

/* status */
#define STATUS_OIP_MASK                 0x01
#define STATUS_READY                    (0 << 0)
#define STATUS_BUSY                     (1 << 0)

#define STATUS_E_FAIL_MASK              0x04
#define STATUS_E_FAIL                   (1 << 2)

#define STATUS_P_FAIL_MASK              0x08
#define STATUS_P_FAIL                   (1 << 3)

/* OTP register defines*/
#define OTP_ECC_MASK                    0X10
#define OTP_ECC_ENABLE                  0x10
#define OTP_ENABLE                      0x40

/* block lock */
#define BL_ALL_LOCKED                   0x38
#define BL_1_2_LOCKED                   0x30
#define BL_1_4_LOCKED                   0x28
#define BL_1_8_LOCKED                   0x20
#define BL_1_16_LOCKED                  0x18
#define BL_1_32_LOCKED                  0x10
#define BL_1_64_LOCKED                  0x08
#define BL_ALL_UNLOCKED                 0x00

#define SPI_NAND_MT29F_ECC_MASK         3
#define SPI_NAND_MT29F_ECC_CORRECTED    1
#define SPI_NAND_MT29F_ECC_UNCORR       2
#define SPI_NAND_MT29F_ECC_RESERVED     3
#define SPI_NAND_MT29F_ECC_SHIFT        4

#define SPI_NAND_GD5F_ECC_MASK          7
#define SPI_NAND_GD5F_ECC_UNCORR        7
#define SPI_NAND_GD5F_ECC_SHIFT         4

/*============================================================================*/

struct spinand_info {
	struct nand_ecclayout *ecclayout;
	struct spi_device *spi;
	void *priv;
	int options;
};

struct spinand_privbuf {
	uint32_t	col;
	uint32_t	row;
	int		buf_ptr;
	u8		*buf;
};

struct spinand_cmd {
	u8		cmd;
	u32		n_addr;		/* Number of address */
	u8		addr[3];	/* Reg Offset */
	u32		n_dummy;	/* Dummy use */
	u32		n_tx;		/* Number of tx bytes */
	u8		*tx_buf;	/* Tx buf */
	u32		n_rx;		/* Number of rx bytes */
	u8		*rx_buf;	/* Rx buf */
};

/*============================================================================*/

/*
 * this is to diferentiate gigadevice dummy byte with the rest of manufacturer
 *  - CMD_READ_RDM : extra dummy byte before column address
 */
#define GIGADEVICE_DUMMY_TYPE 0x1

#endif /* __LTQ_MTD_SPI_NAND_H__ */

