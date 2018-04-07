/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2009~2013 Lei Chuanhua <chuanhua.lei@lantiq.com>
 */

/*!
  \file ltq_sflash.h
  \ingroup IFX_SFLASH
  \brief ifx serial flash driver header file
*/
#ifndef LTQ_SFLASH_H
#define LTQ_SFLASH_H

/* Flash related definition */
#define IFX_FLASH_128KB   0
#define IFX_FLASH_256KB   1
#define IFX_FLASH_512KB   2
#define IFX_FLASH_1MB	  3
#define IFX_FLASH_2MB	  4
#define IFX_FLASH_4MB	  5
#define IFX_FLASH_8MB	  6
#define IFX_FLASH_16MB	  7
#define IFX_SPI_MAX_FLASH 8
#define IFX_FLASH_INVALID_SIZE -1

#define IFX_FLASH_PAGESIZE    256

/* Flash opcodes. */
#define IFX_OPCODE_WREN         0x06 /* Write enable */
#define IFX_OPCODE_RDSR         0x05 /* Read status register */
#define IFX_OPCODE_NORM_READ    0x03 /* Read data bytes (low frequency) */
#define IFX_OPCODE_FAST_READ    0x0b /* Read data bytes (high frequency) */
#define IFX_OPCODE_PP           0x02 /* Page program (up to 256 bytes) */
#define IFX_OPCODE_BE_4K        0x20 /* Erase 4KiB block */
#define IFX_OPCODE_BE_32K       0x52 /* Erase 32KiB block */
#define IFX_OPCODE_CHIP_ERASE   0xc7 /* Erase whole flash chip */
#define IFX_OPCODE_SE           0xd8 /* Sector erase (usually 64KiB) */
#define IFX_OPCODE_RDID         0x9f /* Read JEDEC ID */

#define IFX_MAX_ADDRESS_NUM     5
#define IFX_MAX_DUMMY_CYCLES    10

/* Status Register bits. */
#define IFX_SR_WIP   1 /* Write in progress */
#define IFX_SR_WEL   2 /* Write enable latch */
/* meaning of other SR_* bits may differ between vendors */
#define IFX_SR_BP0   4 /* Block protect 0 */
#define IFX_SR_BP1   8 /* Block protect 1 */
#define IFX_SR_BP2   0x10 /* Block protect 2 */
#define IFX_SR_SRWD  0x80 /* SR write protect */

#define IFX_SFLASH_DETECT_COUNTER 100000 /* XXX, check if flash mounted */

/* Define max times to check status register before we give up. */
#define	IFX_MAX_READY_WAIT_JIFFIES (10 * HZ)

/*
 * 32 should be maximum requirement for DMA alignment at the low level,
 * keep it here for future change
 */
#define IFX_SFLASH_DMA_MAX_BURST_LEN    32
#define IFX_SFLASH_MAX_WRITE_SIZE      (IFX_FLASH_PAGESIZE * 2)
#define IFX_SFLASH_MAX_READ_SIZE       (IFX_FLASH_PAGESIZE * 8) /* Tunable */

#define IFX_SFLASH_MAX_FLASH_TYPE 15  /* Per Vendor */

/* Define fast read/normal read, fast read for higher frequence*/
#define CONFIG_SPI_FLASH_SLOW_READ

#ifdef CONFIG_SPI_FLASH_SLOW_READ
#define IFX_OPCODE_READ  IFX_OPCODE_NORM_READ
#define IFX_FAST_READ_DUMMY_BYTE  0
#else
#define IFX_OPCODE_READ  IFX_OPCODE_FAST_READ
#define IFX_FAST_READ_DUMMY_BYTE  1
#endif

struct ifx_sflash_info {
	char  *name;

	/* JEDEC id zero means "no ID" (most older chips); otherwise it has
	 * a high byte of zero plus three data bytes: the manufacturer id,
	 * then a two byte device id.
	 */
	u16 id;
	/* The size listed here is what works with IFX_OPCODE_SE, which isn't
	 * necessarily called a "sector" by the vendor.
	 */
	unsigned int sector_size;
	u16 num_sectors;
	u16 flags;
#define SECT_4K    0x01 /* IFX_OPCODE_BE_4K works uniformly */
};

struct ifx_sflash_manufacturer_info {
	char  *name;
	u8     id;
	struct ifx_sflash_info flashes[IFX_SFLASH_MAX_FLASH_TYPE];
};

enum {
	JED_MANU_SPANSION = 0x01,
	JED_MANU_ST       = 0x20,
	JED_MANU_SST      = 0xBF,
	JED_MANU_ATMEL    = 0x1F,
	JED_MANU_WINBOND  = 0xEF,
	JED_MANU_MX       = 0xC2,
};

/* Driver private data */
typedef struct {
	struct mtd_info       *mtd;
	struct mtd_partition  *parsed_parts; /* parsed partitions */
	char                  *flash_tx_org_buf; /* Original write buffer */
	char                  *flash_tx_buf; /* Aligned write buffer */
	char                  *flash_rx_org_buf; /* Orignal read buffer */
	char                  *flash_rx_buf;  /* Aligned read buffer */
	u8                     addr_cycles;
	IFX_SSC_HANDLE         sflash_handler;
	struct ifx_sflash_manufacturer_info *manufacturer;
	struct ifx_sflash_info     *flash;
	u8                          erase_opcode;
	u8 manufacturer_id, device_id1, device_id2;
	unsigned int write_length;
	unsigned long sector_size, num_sectors, size;
	int  dummy_cycles;
} ifx_spi_dev_t;
#endif /* LTQ_SFLASH_H */

