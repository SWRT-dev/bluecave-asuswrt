/*
 * (C) Copyright TOSHIBA CORPORATION 2013
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This file is the header for the NAND BENAND implementation.
 */

#ifndef __MTD_NAND_BENAND_H__
#define __MTD_NAND_BENAND_H__

#if defined(CONFIG_MTD_NAND_BENAND)

static inline int mtd_nand_has_benand(void) { return 1; }

void nand_benand_setupecc(struct mtd_info *mtd, nand_ecc_modes_t *mode);

int nand_read_subpage_benand(struct mtd_info *mtd, struct nand_chip *chip,
			uint32_t data_offs, uint32_t readlen, uint8_t *bufpoi);

int nand_read_page_benand(struct mtd_info *mtd, struct nand_chip *chip,
			  uint8_t *buf, int oob_required, int page);

void nand_benand_init(struct mtd_info *mtd);

#else /* !CONFIG_MTD_NAND_BENAND */

static inline int mtd_nand_has_benand(void) { return 0; }

static inline void nand_benand_setupecc(struct mtd_info *mtd,
					nand_ecc_modes_t *mode) {}

static inline int nand_read_page_benand(struct mtd_info *mtd,
					struct nand_chip *chip,
					uint8_t *buf, int oob_required,
					int page)
{
	return -1;
}

static inline void nand_benand_init(struct mtd_info *mtd)
{
	return -1;
}

int nand_read_subpage_benand(struct mtd_info *mtd, struct nand_chip *chip,
			uint32_t data_offs, uint32_t readlen, uint8_t *bufpoi)
{
	return -1;
}

#endif /* CONFIG_MTD_NAND_BENAND */
#endif /* __MTD_NAND_BENAND_H__ */
