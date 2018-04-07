#include <linux/skbuff.h>
#include <linux/fast_skb.h>

#ifdef CONFIG_LTQ_SKB_RECYCLE
DEFINE_PER_CPU(struct skb_pool_handler,pool_handler)={{NULL,0}};
#define LEN_MODIFIED 1664

struct sk_buff *alloc_recycled_skb(uint32_t len , gfp_t gfp_mask)
{
	struct sk_buff *skb = NULL;
	
	if (len <= LEN_MODIFIED && len > 64) {
		len = LEN_MODIFIED;
	}
	else {
    	len = len; /* If len more than LEN_MODIFIED,cant allocate from pool*/
	}

	DMA_ALLOC_SKB(skb, len + NET_SKB_PAD_ALLOC, gfp_mask);

	if (likely(skb)) {
		skb->recycle=1;
		skb_reserve(skb, NET_SKB_PAD_ALLOC);
		}	
	
	return skb;
}
EXPORT_SYMBOL(alloc_recycled_skb);
#endif


