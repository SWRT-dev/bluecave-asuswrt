#ifndef _XT_EXTMARK_H
#define _XT_EXTMARK_H

#include <linux/types.h>

struct xt_extmark_tginfo2 {
	__u32 extmark, mask;
};

struct xt_extmark_mtinfo1 {
	__u32 extmark, mask;
	__u8 invert;
};

#endif /*_XT_EXTMARK_H*/
