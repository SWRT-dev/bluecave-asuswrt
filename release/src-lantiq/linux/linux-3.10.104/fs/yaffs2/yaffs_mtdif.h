/*
 * YAFFS: Yet another Flash File System . A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2010 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1 as
 * published by the Free Software Foundation.
 *
 * Note: Only YAFFS headers are LGPL, YAFFS C code is covered by GPL.
 */

#ifndef __YAFFS_MTDIF_H__
#define __YAFFS_MTDIF_H__

#include "yaffs_guts.h"

#if (MTD_VERSION_CODE < MTD_VERSION(2, 6, 18))
extern struct nand_oobinfo yaffs_oobinfo;
extern struct nand_oobinfo yaffs_noeccinfo;
#endif
int nandmtd_EraseBlockInNAND(yaffs_dev_t *dev, int blockNumber);
int nandmtd_InitialiseNAND(yaffs_dev_t *dev);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0))
#include <mtd/mtd-abi.h>
#else
#define MTD_OPS_AUTO_OOB 	MTD_OOB_AUTO
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 3, 0))
static inline int mtd_erase(struct mdt_info *mtd, struct erase_info *ei)
{
	return mtd->erase(mtd, ei);
}

static inline int mtd_block_markbad(struct mtd_info *mtd, loff_t ofs)
{
	return mtd->block_mark_bad(mtd, ofs);
}

static inline int mtd_block_isbad(struct mtd_info *mtd, loff_t ofs)
{
	return mtd->block_is_bad(mtd, ofs);
}

static inline int mtd_read_oob(struct mtd_info *mtd, loff_t from,
			       struct mtd_oob_ops *ops)
{
	return mtd->read_oob(mtd, from, ops);
}

static inline int mtd_write_oob(struct mtd_info *mtd, loff_t to,
				struct mtd_oob_ops *ops)
{
	return mtd->write_oob(mtd, to, ops);
}

static inline void mtd_sync(struct mtd_info *mtd)
{
	if (mtd->sync)
		mtd->sync(mtd);
}
#endif

#endif
