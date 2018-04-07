/******************************************************************************
**
** FILE NAME    : ltqmips_usif_sflash.h
** PROJECT      : IFX UEIP
** MODULES      : Serial Flash
**
** DATE         : 16 Oct 2009
** AUTHOR       : Lei Chuanhua
** DESCRIPTION  : SPI Flash MTD Driver
** COPYRIGHT    :       Copyright (c) 2009
**                      Infineon Technologies AG
**                      Am Campeon 1-12, 85579 Neubiberg, Germany
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date        $Author         $Comment
**  16,Oct,2009 Lei Chuanhua   Initial UEIP release
*******************************************************************************/
/*!
  \file ltqmips_usif_sflash.h
  \ingroup LANTIQ_USIF_SFLASH
  \brief ifx usif serial flash driver header
*/
#ifndef LTQMIPS_USIF_SFLASH_H
#define LTQMIPS_USIF_SFLASH_H

#include <lantiq_usif_spi.h>

/* Flash related definition */
#define LANTIQ_FLASH_128KB   0
#define LANTIQ_FLASH_256KB   1
#define LANTIQ_FLASH_512KB   2
#define LANTIQ_FLASH_1MB     3
#define LANTIQ_FLASH_2MB     4
#define LANTIQ_FLASH_4MB     5
#define LANTIQ_FLASH_8MB     6
#define LANTIQ_FLASH_16MB    7
#define LANTIQ_SPI_MAX_FLASH 8
#define LANTIQ_FLASH_INVALID_SIZE -1

#define LANTIQ_FLASH_PAGESIZE      256

/* Flash opcodes. */
#define LANTIQ_OPCODE_WREN         0x06 /* Write enable */
#define LANTIQ_OPCODE_RDSR         0x05 /* Read status register */
#define LANTIQ_OPCODE_NORM_READ    0x03 /* Read data bytes (low frequency) */
#define LANTIQ_OPCODE_FAST_READ    0x0b /* Read data bytes (high frequency) */
#define LANTIQ_OPCODE_PP           0x02 /* Page program (up to 256 bytes) */
#define LANTIQ_OPCODE_BE_4K        0x20 /* Erase 4KiB block */
#define LANTIQ_OPCODE_BE_32K       0x52 /* Erase 32KiB block */
#define LANTIQ_OPCODE_CHIP_ERASE   0xc7 /* Erase whole flash chip */
#define LANTIQ_OPCODE_SE           0xd8 /* Sector erase (usually 64KiB) */
#define LANTIQ_OPCODE_RDID         0x9f /* Read JEDEC ID */

#define LANTIQ_MAX_ADDRESS_NUM     5
#define LANTIQ_MAX_DUMMY_CYCLES    10

/* Status Register bits. */
#define LANTIQ_SR_WIP          1 /* Write in progress */
#define LANTIQ_SR_WEL          2 /* Write enable latch */
/* meaning of other SR_* bits may differ between vendors */
#define LANTIQ_SR_BP0          4 /* Block protect 0 */
#define LANTIQ_SR_BP1          8 /* Block protect 1 */
#define LANTIQ_SR_BP2          0x10 /* Block protect 2 */
#define LANTIQ_SR_SRWD         0x80 /* SR write protect */

#define LANTIQ_SFLASH_DETECT_COUNTER   100000

/* Define max times to check status register before we give up. */
#define	LANTIQ_MAX_READY_WAIT_JIFFIES (10 * HZ)

/*
 * 32 should be maximum requirement for DMA alignment at the low level,
 * keep it here for future change
 */
#define LANTIQ_SFLASH_DMA_MAX_BURST_LEN    32
#define LANTIQ_SFLASH_MAX_WRITE_SIZE      (LANTIQ_FLASH_PAGESIZE * 2)
#define LANTIQ_SFLASH_MAX_READ_SIZE       (LANTIQ_FLASH_PAGESIZE * 8)

#define LANTIQ_SFLASH_MAX_FLASH_TYPE 15 /* Per Vendor */

/* Define fast read/normal read, fast read for higher frequence*/
#define CONFIG_SPI_FLASH_SLOW_READ


#ifdef CONFIG_SPI_FLASH_SLOW_READ
#define LANTIQ_OPCODE_READ  LANTIQ_OPCODE_NORM_READ
#define LANTIQ_FAST_READ_DUMMY_BYTE  0
#else
#define LANTIQ_OPCODE_READ  LANTIQ_OPCODE_FAST_READ
#define LANTIQ_FAST_READ_DUMMY_BYTE  1
#endif

struct lantiq_usif_sflash_info {
	char *name;

	/* JEDEC id zero means "no ID" (most older chips); otherwise it has
	 * a high byte of zero plus three data bytes: the manufacturer id,
	 * then a two byte device id.
	 */
	u16 id;
	/* The size listed here is what works with LANTIQ_OPCODE_SE, which isn't
	 * necessarily called a "sector" by the vendor.
	 */
	unsigned int sector_size;
	u16 num_sectors;
	u16 flags;
#define SECT_4K     0x01 /* LANTIQ_OPCODE_BE_4K works uniformly */
};

struct lantiq_usif_sflash_manufacturer_info {
	char *name;
	u8 id;
	struct lantiq_usif_sflash_info flashes[LANTIQ_SFLASH_MAX_FLASH_TYPE];
};

enum {
	JED_MANU_SPANSION = 0x01,
	JED_MANU_ST = 0x20,
	JED_MANU_SST = 0xBF,
	JED_MANU_ATMEL = 0x1F,
	JED_MANU_WINBOND = 0xEF,
	JED_MANU_MX = 0xC2,
};

/* Driver private data */
typedef struct {
	struct mtd_info *mtd;
	struct mtd_partition *parsed_parts;	/* parsed partitions */
	char *flash_tx_org_buf;	/* Original write buffer */
	char *flash_tx_buf;	/* Aligned write buffer */
	char *flash_rx_org_buf;	/* Orignal read buffer */
	char *flash_rx_buf;	/* Aligned read buffer */
	u8 addr_cycles;
	LANTIQ_USIF_SPI_HANDLE_t sflash_handler;
	struct lantiq_usif_sflash_manufacturer_info *manufacturer;
	struct lantiq_usif_sflash_info *flash;
	u8 erase_opcode;
	u8 manufacturer_id, device_id1, device_id2;
	unsigned int write_length;
	unsigned long sector_size, num_sectors, size;
	int dummy_cycles;
} lantiq_usif_spi_dev_t;

#ifdef CONFIG_USIF_SPI_XRX200
#include "lantiq_usif_sflash_vr9.h"
#elif defined CONFIG_USIF_SPI_XRX300
#include "lantiq_usif_sflash_ar10.h"
#else
#error "Platform not specified!"
#endif

#endif /* LTQMIPS_USIF_SFLASH_H */

