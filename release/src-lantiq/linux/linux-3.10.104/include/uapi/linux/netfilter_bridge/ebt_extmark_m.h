#ifndef __LINUX_BRIDGE_EBT_EXTMARK_M_H
#define __LINUX_BRIDGE_EBT_EXTMARK_M_H

#include <linux/types.h>

#define EBT_EXTMARK_AND 0x01
#define EBT_EXTMARK_OR 0x02
#define EBT_EXTMARK_MASK (EBT_EXTMARK_AND | EBT_EXTMARK_OR)
struct ebt_extmark_m_info {
	unsigned long extmark, mask;
	__u8 invert;
	__u8 bitmask;
};
#define EBT_EXTMARK_MATCH "extmark_m"

#endif
