/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Author: Leonid Yegoshin (yegoshin@mips.com)
 * Copyright (C) 2012 MIPS Technologies, Inc.
 * Copyright (C) 2014 Lei Chuanhua <Chuanhua.lei@lantiq.com>
 */

#ifndef _ASM_LANTIQ_SPACES_H
#define _ASM_LANTIQ_SPACES_H

#ifdef CONFIG_EVA

#ifdef CONFIG_EVA_2GB

#define PAGE_OFFSET		_AC(0x20000000, UL)
/* skip first 512MB */
#define PHYS_OFFSET		_AC(0x20000000, UL)
/* No Highmem Support */
#define HIGHMEM_START		_AC(0xffff0000, UL)


#define UNCAC_BASE		_AC(0xa0000000, UL)
#define CAC_BASE		_AC(0x20000000, UL)
#define IO_BASE			UNCAC_BASE

#define KSEG
#define KUSEG			0x00000000
#define KSEG0			0x20000000
#define KSEG1			0xa0000000
#define KSEG2			0xc0000000
#define KSEG3			0xe0000000

#define CKUSEG			0x00000000
#define CKSEG0			0x20000000
#define CKSEG1			0xa0000000
#define CKSEG2			_AC(0xc0000000, UL)
#define CKSEG3			0xe0000000

/* Range from 0x2000.0000 ~ 0x9FFF.FFFF for KSEG0 */
#define KSEGX(a)		(((_ACAST32_(a)) < 0xA0000000) ?	\
					KSEG0 : ((_ACAST32_(a)) & 0xE0000000))

#define INDEX_BASE		CKSEG0
#define MAP_BASE		CKSEG2
#define VMALLOC_START		MAP_BASE
#define VMALLOC_END		(MAP_BASE + _AC(0x20000000, UL) - 2 * PAGE_SIZE)

#define IO_SIZE			_AC(0x10000000, UL)
#define IO_SHIFT		_AC(0x10000000, UL)

#define CPHYSADDR(a)		((_ACAST32_(a)) - PAGE_OFFSET + PHYS_OFFSET)
#define RPHYSADDR(a)		((_ACAST32_(a)) & 0x1fffffff)

#define LEGACY_KSEC0(a)		(RPHYSADDR(a) | 0x80000000)

/* DRAM one */
#define CKSEG0ADDR(a)		((_ACAST32_(a)))

/* IO space one */
#define CKSEG1ADDR(a)		(RPHYSADDR(a) | KSEG1)

/*
 * Map an address to a certain kernel segment
 */
/* DRAM one */
#define KSEG0ADDR(a)		(CKSEG0ADDR(a))

/* IO space one */
#define KSEG1ADDR(a)		(RPHYSADDR(a) | KSEG1)
#define __pa_symbol(x)		__pa(x)

#elif defined(CONFIG_EVA_1GB)

#define PAGE_OFFSET		_AC(0x60000000, UL)
#define PHYS_OFFSET		_AC(0x20000000, UL)

/* No Highmem Support */
#define HIGHMEM_START		_AC(0xffff0000, UL)

#define UNCAC_BASE		(_AC(0xa0000000, UL))
#define CAC_BASE		(_AC(0x60000000, UL))
#define IO_BASE			UNCAC_BASE /* Must be the same */

#define KSEG
#define KUSEG			0x00000000
#define KSEG0			0x60000000
#define KSEG1			0xa0000000
#define KSEG2			0xc0000000
#define KSEG3			0xe0000000

#define CKUSEG			0x00000000
#define CKSEG0			0x60000000
#define CKSEG1			0xa0000000
#define CKSEG2			_AC(0xc0000000, UL)
#define CKSEG3			0xe0000000

/* Range from 0x6000.0000 ~ 0x9FFF.FFFF for KSEG0 */
#define KSEGX(a)		(((_ACAST32_(a)) < 0xA0000000) ?	\
					KSEG0 : ((_ACAST32_(a)) & 0xE0000000))

#define INDEX_BASE		CKSEG0
#define MAP_BASE		CKSEG2
#define VMALLOC_START		MAP_BASE
#define VMALLOC_END		(MAP_BASE + _AC(0x20000000, UL) - 2 * PAGE_SIZE)

#define IO_SIZE			_AC(0x10000000, UL)
#define IO_SHIFT		_AC(0x10000000, UL)

#define CPHYSADDR(a)		((_ACAST32_(a)) - PAGE_OFFSET + PHYS_OFFSET)

#define RPHYSADDR(a)		((_ACAST32_(a)) & 0x1fffffff)

#define LEGACY_KSEC0(a)		(RPHYSADDR(a) | 0x80000000)

/* DRAM one */
#define CKSEG0ADDR(a)		((_ACAST32_(a)))

/* IO space one */
#define CKSEG1ADDR(a)		(RPHYSADDR(a) | KSEG1)

/*
 * Map an address to a certain kernel segment
 */
/* DRAM one */
#define KSEG0ADDR(a)		(CKSEG0ADDR(a))

/* IO space one */
#define KSEG1ADDR(a)		(RPHYSADDR(a) | KSEG1)
#define __pa_symbol(x)		__pa(x)

#else /* EVA legacy */

#define PAGE_OFFSET		_AC(0x80000000, UL)
/* skip first 512MB */
#define PHYS_OFFSET		_AC(0x20000000, UL)
/* No Highmem Support */
#define HIGHMEM_START		_AC(0xffff0000, UL)


#define UNCAC_BASE		_AC(0xa0000000, UL)
#define CAC_BASE		_AC(0x80000000, UL)
#define IO_BASE			UNCAC_BASE

#define KSEG
#define KUSEG			0x00000000
#define KSEG0			0x80000000
#define KSEG1			0xa0000000
#define KSEG2			0xc0000000
#define KSEG3			0xe0000000

#define CKUSEG			0x00000000
#define CKSEG0			0x80000000
#define CKSEG1			0xa0000000
#define CKSEG2			_AC(0xc0000000, UL)
#define CKSEG3			0xe0000000

#define KSEGX(a)		((_ACAST32_ (a)) & 0xe0000000)

#define INDEX_BASE		CKSEG0
#define MAP_BASE		CKSEG2
#define VMALLOC_START		MAP_BASE
#define VMALLOC_END		(MAP_BASE + _AC(0x20000000, UL) - 2 * PAGE_SIZE)

#define IO_SIZE			_AC(0x10000000, UL)
#define IO_SHIFT		_AC(0x10000000, UL)
#define CPHYSADDR(a)		((_ACAST32_(a)) - PAGE_OFFSET + PHYS_OFFSET)
#define RPHYSADDR(a)		((_ACAST32_(a)) & 0x1fffffff)
#define LEGACY_KSEC0(a)		(RPHYSADDR(a) | KSEG0)

/* DRAM one */
#define CKSEG0ADDR(a)		((_ACAST32_(a)))

/* IO space one */
#define CKSEG1ADDR(a)		(RPHYSADDR(a) | KSEG1)

/*
 * Map an address to a certain kernel segment
 */
/* DRAM one */
#define KSEG0ADDR(a)		(CKSEG0ADDR(a))

/* IO space one */
#define KSEG1ADDR(a)		(RPHYSADDR(a) | KSEG1)
#define __pa_symbol(x)		__pa(x)

#endif
#endif /* CONFIG_EVA */

#include <asm/mach-generic/spaces.h>

#endif /* __ASM_MALTA_SPACES_H */
