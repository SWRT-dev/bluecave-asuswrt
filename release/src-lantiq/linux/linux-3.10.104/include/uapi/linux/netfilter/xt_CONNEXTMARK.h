#ifndef _XT_CONNEXTMARK_H_target
#define _XT_CONNEXTMARK_H_target

#include <linux/types.h>

enum {
       CONNEXTMARK_SAVE = 1,
       CONNEXTMARK_RESTORE,
};

struct xt_connextmark_target_info {
       __u8 mode;
};

#endif /*_XT_CONNEXTMARK_H_target */
