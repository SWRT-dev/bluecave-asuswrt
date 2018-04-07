#ifndef _IPT_SYNFLOOD_H
#define _IPT_SYNFLOOD_H

#include <linux/param.h>
#include <linux/types.h>
#include <linux/netfilter/xt_limit.h>
#define IPT_LIMIT_SCALE XT_LIMIT_SCALE
#define ipt_rateinfo xt_rateinfo

/* 1/10,000 sec period => max of 10,000/sec.  Min rate is then 429490
   seconds, or one every 59 hours. */
/*
 * Keep track of up to LIST_SIZE source addresses, using a hash table of
 * HASH_SIZE entries for faster lookups, but limiting hash collisions to
 * HASH_MAX source addresses per the same hash value.
 */
#define LIST_SIZE                      0x40
#define HASH_LOG                       7
#define HASH_SIZE                      (1 << HASH_LOG)
#define HASH_MAX                       0x10

enum addr_type_t { DESTINATION=0, SOURCE };
#if 0
struct ipt_rateinfo {
       u_int32_t avg;    /* Average secs between packets * scale */
       u_int32_t burst;  /* Period multiplier for upper limit. */
       u_int8_t  invert; /* Invert */

       /* Used internally by the kernel */
       u_int32_t credit;
       u_int32_t credit_cap, cost;
       enum addr_type_t addr_type;
};
#endif
#endif /*_IPT_SYNFLOOD_H*/

