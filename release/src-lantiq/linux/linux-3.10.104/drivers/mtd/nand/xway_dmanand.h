/******************************************************************************
**
** FILE NAME	: xway_dmanand.h
** PROJECT		: UEIP
** MODULES		: NAND Flash
**
** DATE			: 05 Aug 2008
** AUTHOR		: Lei Chuanhua
** DESCRIPTION	: NAND Flash MTD Driver
** COPYRIGHT	:		Copyright (c) 2008
**						Infineon Technologies AG
**						Am Campeon 1-12, 85579 Neubiberg, Germany
**
**	  This program is free software; you can redistribute it and/or modify
**	  it under the terms of the GNU General Public License as published by
**	  the Free Software Foundation; either version 2 of the License, or
**	  (at your option) any later version.
**
** HISTORY
** $Date		$Author			 $Version	  $Comment
** 05 Aug 2008	Lei Chuanhua	   1.0		  initial version
** 08 Nov 2011	Mohammad Firdaus   1.1		  Added Support for MLC NAND driver
**				B Alias Thani
** 03 Sept 2013 Mohammad Firdaus   2.0		  Migrate drivers to OpenWRT
**				B Alias Thani 0
*******************************************************************************/
#ifndef XWAY_DMANAND_H
#define XWAY_DMANAND_H

#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>

#define BBT_D (struct nand_bbt_descr)
#define ECC_L (struct nand_ecclayout)

#define ltq_dma_nand_w32(x, y)	  ltq_w32(x, ltq_dmanand_membase + (y))
#define ltq_dma_nand_r32(x)		  ltq_r32(ltq_dmanand_membase + (x))

#ifndef CONFIG_SOC_GRX500
/* calculate respective offset - x: page size, y: ECC Length */
#define BBT_OFFS(x,y) (8 + (x / 128) * y)
#define VER_OFFS(x,y) (12 + (x / 128) * y)
#else
/* BBT Marker @ OOB[3] to OOB[6], Version @ OOB[7] */
#define BBT_OFFS(x,y) 3
#define VER_OFFS(x,y) 7
#endif

extern __iomem void *ltq_dmanand_membase;

/* bad block table descriptor which defines where in the
 * NAND device the bad block indicator is stored. This information
 * can be found in the NAND device data sheet (pattern, location, etc)
 * Be careful not to write over the bad block indicators
 */

uint8_t ltq_nand_2048_bbt_pattern[] = { 0xff, 0xff };
uint8_t ltq_nand_512_bbt_pattern[] = {0xff, 0xff};
uint8_t generic_bbt_pattern[] = {'B', 'b', 't', '0' };
uint8_t generic_mirror_pattern[] = {'1', 't', 'b', 'B' };

struct nand_bbt_descr ltq_nand_main_desc_8192 = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_LASTBLOCK,
	.offs = BBT_OFFS(8192, 4),
	.len = 4,
	.veroffs = VER_OFFS(8192, 4),
	.maxblocks = 4,
	.pattern = generic_bbt_pattern,
};

struct nand_bbt_descr ltq_nand_mirror_desc_8192 = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_LASTBLOCK,
	.offs = BBT_OFFS(8192, 4),
	.len = 4,
	.veroffs = VER_OFFS(8192, 4),
	.maxblocks = 4,
	.pattern = generic_mirror_pattern,
};

struct nand_bbt_descr ltq_nand_main_desc_8192_3BECC = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
		| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_LASTBLOCK,
	.offs = BBT_OFFS(8192, 3),
	.len = 4,
	.veroffs = VER_OFFS(8192, 3),
	.maxblocks = 4,
	.pattern = generic_mirror_pattern,
};

struct nand_bbt_descr ltq_nand_mirror_desc_8192_3BECC = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
		| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_LASTBLOCK,
	.offs = BBT_OFFS(8192, 3),
	.len = 4,
	.veroffs = VER_OFFS(8192, 3),
	.maxblocks = 4,
	.pattern = generic_mirror_pattern,
};

struct nand_bbt_descr ltq_nand_main_desc_4096 = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_LASTBLOCK,
	.offs = BBT_OFFS(4096, 4),
	.len = 4,
	.veroffs = VER_OFFS(4096, 4),
	.maxblocks = 4,
	.pattern = generic_bbt_pattern,
};

struct nand_bbt_descr ltq_nand_mirror_desc_4096 = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_LASTBLOCK,
	.offs = BBT_OFFS(4096, 4),
	.len = 4,
	.veroffs = VER_OFFS(4096, 4),
	.maxblocks = 4,
	.pattern = generic_mirror_pattern,
};

struct nand_bbt_descr ltq_nand_main_desc_4096_3BECC = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_LASTBLOCK,
	.offs = BBT_OFFS(4096, 3),
	.len = 4,
	.veroffs = VER_OFFS(4096, 3),
	.maxblocks = 4,
	.pattern = generic_bbt_pattern,
};

struct nand_bbt_descr ltq_nand_mirror_desc_4096_3BECC = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_LASTBLOCK,
	.offs = BBT_OFFS(4096, 3),
	.len = 4,
	.veroffs = VER_OFFS(4096, 3),
	.maxblocks = 4,
	.pattern = generic_mirror_pattern,
};

struct nand_bbt_descr ltq_nand_main_desc_2048 = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_LASTBLOCK,
	.offs = BBT_OFFS(2048, 3),
	.len = 4,
	.veroffs = VER_OFFS(2048, 3),
	.maxblocks = 4,
	.pattern = generic_bbt_pattern,
};

struct nand_bbt_descr ltq_nand_mirror_desc_2048 = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_LASTBLOCK,
	.offs = BBT_OFFS(2048, 3),
	.len = 4,
	.veroffs = VER_OFFS(2048, 3),
	.maxblocks = 4,
	.pattern = generic_mirror_pattern,
};

struct nand_bbt_descr ltq_nand_main_desc_512 = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_ABSPAGE | NAND_BBT_LASTBLOCK,
	.offs = 15,
	.len = 1,
	.maxblocks = 4,
	.pattern = generic_bbt_pattern,

};

struct nand_bbt_descr ltq_nand_mirror_desc_512 = {
	.options = NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_ABSPAGE | NAND_BBT_LASTBLOCK,
	.offs = 15,
	.len = 1,
	.maxblocks = 4,
	.pattern = generic_mirror_pattern,

};

struct nand_bbt_descr factory_default = {
	.options = NAND_BBT_SCAN2NDPAGE,
	.offs = 0,
	.len = 2,
	.pattern = ltq_nand_2048_bbt_pattern,
};


/* Generic flash bbt decriptors. BBT layout that are predefined
 * in chips can be defined here.
*/

struct nand_bbt_descr generic_bbt_main_descr = {
		.options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
		.offs = 8,
		.len = 4,
		.veroffs = 12,
		.maxblocks = 4,
		.pattern = generic_bbt_pattern
};

struct nand_bbt_descr generic_bbt_mirror_descr = {
		.options = NAND_BBT_LASTBLOCK | NAND_BBT_CREATE | NAND_BBT_WRITE
			| NAND_BBT_2BIT | NAND_BBT_VERSION | NAND_BBT_PERCHIP,
		.offs = 8,
		.len = 4,
		.veroffs = 12,
		.maxblocks = 4,
		.pattern = generic_mirror_pattern
};

/* --> 512 page size for Samsung chips, bad block info
 * is located @ the last byte of the oob (16th byte)
 * area of the 1st two pages of every block.
 * --> 2048 page size for Samsung chips, bad block info
 * is located @ the first byte of the oob area of
 * the first two pages of every block
 */

struct nand_ecclayout ltq_oobinfo_512 = {
	.eccbytes = 3,
	.eccpos = {0, 1, 2},
	.oobavail = 9,
	.oobfree = { {4, 13} }
};

struct nand_ecclayout ltq_oobinfo_2048 = {
	.eccbytes = 12,
	.eccpos = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
	.oobavail = 50,
	.oobfree = { {13, 63} }
};

/* MLC driver ecc layout */
struct nand_ecclayout B4_byte_ecc_oobinfo_2048 = {
	.eccbytes = 64,
	.eccpos = {0, 1, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
			   13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
			   23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
			   33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
			   43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
			   53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
			   63},
	.oobavail = 0,
	.oobfree = { {0, 0} }
};

#ifndef CONFIG_SOC_GRX500
struct nand_ecclayout B4_byte_ecc_oobinfo_4096 = {
	.eccbytes = 128,
	.eccpos = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
		103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
		113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
		123, 124, 125, 126, 127, 128  },
	.oobavail = 96,
	.oobfree = { {129, 96} }
};

struct nand_ecclayout B4_byte_ecc_oobinfo_4096_other = {
	.eccbytes = 128,
	.eccpos = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
		103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
		113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
		123, 124, 125, 126, 127, 128 },
	.oobavail = 90,
	.oobfree = { {129, 90} }
};

struct nand_ecclayout B4_byte_ecc_oobinfo_8192 = {
	.eccbytes = 256,
	.eccpos = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
		103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
		113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
		123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
		133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
		143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
		153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
		163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
		173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
		183, 184, 185, 186, 187, 188, 189, 190, 191, 192,
		193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
		203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
		213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
		223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
		233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
		243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
		253, 254, 255, 256 },
	.oobavail = 176,
	.oobfree = { {257, 176} }
};

struct nand_ecclayout B3_byte_ecc_oobinfo_2048 = {
	.eccbytes = 48,
	.eccpos = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47 },
	.oobfree = { {48, 16} }
};

struct nand_ecclayout B3_byte_ecc_oobinfo_4096 = {
	.eccbytes = 96,
	.eccpos = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96	},
	.oobavail = 128,
	.oobfree = { {97, 128} }
};

struct nand_ecclayout B3_byte_ecc_oobinfo_4096_other = {
	.eccbytes = 96,
	.eccpos = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96 },
	.oobavail = 122,
	.oobfree = { {97, 122} }
};

struct nand_ecclayout B3_byte_ecc_oobinfo_8192 = {
	.eccbytes = 192,
	.eccpos = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
		103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
		113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
		123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
		133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
		143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
		153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
		163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
		173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
		183, 184, 185, 186, 187, 188, 189, 190, 191 },
	.oobavail = 241,
	.oobfree = { {192, 241} }
};
#else
struct nand_ecclayout B4_byte_ecc_oobinfo_4096 = {
	.eccbytes = 128,
	.eccpos = { 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
		103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
		113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
		123, 124, 125, 126, 127, 128, 129, 130, 131,
		132, 133, 134, 135 },
	.oobavail = 88,
	.oobfree = { {136, 88} }
};

struct nand_ecclayout B4_byte_ecc_oobinfo_4096_other = {
	.eccbytes = 128,
	.eccpos = { 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
		103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
		113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
		123, 124, 125, 126, 127, 128, 129, 130, 131,
		        132, 133, 134, 135 },
	.oobavail = 82,
	.oobfree = { {136, 82} }
};

struct nand_ecclayout B4_byte_ecc_oobinfo_8192 = {
	.eccbytes = 256,
	.eccpos = { 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
		103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
		113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
		123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
		133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
		143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
		153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
		163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
		173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
		183, 184, 185, 186, 187, 188, 189, 190, 191, 192,
		193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
		203, 204, 205, 206, 207, 208, 209, 210, 211, 212,
		213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
		223, 224, 225, 226, 227, 228, 229, 230, 231, 232,
		233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
		243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
		253, 254, 255, 256, 257, 258, 259, 260, 261, 262,
		263, 264, },
	.oobavail = 168,
	.oobfree = { {265, 168} }
};

struct nand_ecclayout B3_byte_ecc_oobinfo_2048 = {
	.eccbytes = 48,
	.eccpos = { 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54 },
	.oobfree = { {55, 8} }
};

struct nand_ecclayout B3_byte_ecc_oobinfo_4096 = {
	.eccbytes = 96,
	.eccpos = { 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
		103, 104 },
	.oobavail = 116,
	.oobfree = { {105, 116} }
};

struct nand_ecclayout B3_byte_ecc_oobinfo_4096_other = {
	.eccbytes = 96,
	.eccpos = { 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
		103, 104 },
	.oobavail = 114,
	.oobfree = { {105, 114} }
};

struct nand_ecclayout B3_byte_ecc_oobinfo_8192 = {
	.eccbytes = 192,
	.eccpos = { 8, 9, 10, 11, 12,
		13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
		23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
		33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
		43, 44, 45, 46, 47, 48, 49, 50, 51, 52,
		53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
		63, 64, 65, 66, 67, 68, 69, 70, 71, 72,
		73, 74, 75, 76, 77, 78, 79, 80, 81, 82,
		83, 84, 85, 86, 87, 88, 89, 90, 91, 92,
		93, 94, 95, 96, 97, 98, 99, 100, 101, 102,
		103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
		113, 114, 115, 116, 117, 118, 119, 120, 121, 122,
		123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
		133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
		143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
		153, 154, 155, 156, 157, 158, 159, 160, 161, 162,
		163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
		173, 174, 175, 176, 177, 178, 179, 180, 181, 182,
		183, 184, 185, 186, 187, 188, 189, 190, 191, 192,
		193, 194, 195, 196, 197, 198, 199 },
	.oobavail = 232,
	.oobfree = { {200, 232} }
};
#endif /* CONFIG_SOC_GRX500 */

struct nand_ecclayout embedded_ecc_generic = {
	.eccbytes = 0,
	.eccpos = { 0 },
};

/* OOB info for generic chips. For specific chips with
 * predefined factry default OOB info, create a structure
 * with the specifics of the chip's OOB layout
 */

struct nand_ecclayout oobinfo_512_generic  = {
		.eccbytes = 6,
		.eccpos = {0, 1, 2, 3, 6, 7},
		.oobfree = {
				{ .offset = 8,
				  . length = 8} }
};

struct nand_ecclayout oobinfo_2048_generic = {
		.eccbytes = 24,
		.eccpos = {
				   40, 41, 42, 43, 44, 45, 46, 47,
				   48, 49, 50, 51, 52, 53, 54, 55,
				   56, 57, 58, 59, 60, 61, 62, 63},
	.oobavail = 38,
		.oobfree = {
				{ .offset = 2,
				 .length = 38} }
};

struct nand_extra_info {
	u32 chip_id;
	int addr_cycle;
	struct nand_ecclayout *chip_ecclayout;
	struct nand_bbt_descr *chip_bbt_main_descr;
	struct nand_bbt_descr *chip_bbt_mirror_descr;
};

#endif /* XWAY_DMANAND_H */

