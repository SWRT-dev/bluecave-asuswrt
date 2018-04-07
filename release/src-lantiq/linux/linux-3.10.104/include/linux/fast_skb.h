#ifndef __FAST_SKB_H_
#define __FAST_SKB_H_

#ifdef CONFIG_LTQ_SKB_RECYCLE 
#include <linux/percpu.h>
#include <linux/smp.h>
#include <linux/netdevice.h>

#define FAST_DMA_PACKET_SIZE 1664
#define SKB_RECYCLE_LEN 1600
#define NET_SKB_PAD_ALLOC 64
	extern struct sk_buff *alloc_recycled_skb(uint32_t len, gfp_t gfp_mask); 
	#define FAST_ALLOC_SKB()  fast_alloc_skb()
	#define SKB_POOL_ENQUEUE(skb)	fast_free_skb(skb)
	#define skb_recycle_free(skb)	SKB_POOL_ENQUEUE(skb)
void __init_skb(struct sk_buff *skb);
inline static struct sk_buff* fast_alloc_skb(void)
{
	struct sk_buff *skb=NULL;
	unsigned long flags;
	uint32_t c1;
	local_irq_save(flags);
	c1 = per_cpu(pool_handler,get_cpu()).count;
	put_cpu();

	if (c1) {
			skb = per_cpu(pool_handler,get_cpu()).skb_dma_pool[c1-1];
			put_cpu();
	        per_cpu(pool_handler,get_cpu()).count--;	
			put_cpu();
			}
	local_irq_restore(flags);
	return skb;
}

inline static int fast_free_skb(struct sk_buff *skb)
{  
	int ret = -1;
	unsigned long flags;
	
	uint32_t c2;

	if ((skb->end - skb->head) < SKB_RECYCLE_LEN + 128) {		
    	goto lbl_devfree_ret;
   		 }
	
	local_irq_save(flags);
	c2 = per_cpu(pool_handler,get_cpu()).count;
	put_cpu();

	local_irq_restore(flags);
	if( (c2) >= MAX_SKB_DMA_POOL_LEN ) 
	{
	goto lbl_devfree_ret;
	} else {
			if(likely(skb))
			{
		    __init_skb(skb); /*XXX: The reset of skb happens */
			}

			local_irq_save(flags);
            per_cpu(pool_handler,get_cpu()).skb_dma_pool[per_cpu(pool_handler,get_cpu()).count] = skb;
  			put_cpu();
 			per_cpu(pool_handler,get_cpu()).count++;
			put_cpu();
		    local_irq_restore(flags);
			skb->recycle = 1; // XXX:added back because of init_skb
			}
    goto lbl_free_ret;

lbl_devfree_ret:
	skb->recycle = 0;
	kfree(skb->head);
	
lbl_free_ret:
	return ret;
}


#define DMA_ALLOC_SKB(skb, len,mask)	do {\
 				uint32_t c3; \
				unsigned long flags;\
				local_irq_save(flags);\
				c3 = per_cpu(pool_handler,get_cpu()).count; \
					local_irq_restore(flags);\
					if((len == (FAST_DMA_PACKET_SIZE + NET_SKB_PAD_ALLOC))  && (c3)){\
					skb = FAST_ALLOC_SKB();\
					}\
					if(!skb){\
					skb = alloc_skb(len,mask);\
					if(skb){\
					skb->recycle=1; }	}\
					}while(0)
	
								  

#endif
#endif /* __FAST_SKB_H_ */

