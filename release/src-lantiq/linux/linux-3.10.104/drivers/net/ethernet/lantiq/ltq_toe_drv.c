/******************************************************************************
**
** FILE NAME    : ltq_toe_drv.c
** AUTHOR       : Suresh Nagaraj
** DESCRIPTION  : Lantiq ToE driver for XRX500 series
** COPYRIGHT    :       Copyright (c) 2015
**                      Lantiq Deutschland
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date                $Author                 $Comment
*******************************************************************************/
#include <linux/module.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/seq_file.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/tcp.h>

#include <lantiq.h>
#include <lantiq_soc.h>
#include <lantiq_irq.h>
#include <lantiq_dmax.h>
#include <net/lantiq_cbm.h>
#include <net/lantiq_cbm_api.h>
#include <net/datapath_api.h>

#include "ltq_toe_reg.h"
#include "ltq_toe_drv.h"

#ifdef USE_TIMER_FOR_SESSION_STOP
#include <linux/timer.h>
#include <linux/jiffies.h>
#endif

#define PORT_REQ_CMD_REG0(port)  (PORT_REQ_CMD_REG0_0 + ((port))*0x20)
#define PORT_REQ_CMD_REG1(port)  (PORT_REQ_CMD_REG1_0 + ((port))*0x20)
#define PORT_REQ_CMD_REG2(port)  (PORT_REQ_CMD_REG2_0 + ((port))*0x20)
#define PORT_REQ_CMD_REG3(port)  (PORT_REQ_CMD_REG3_0 + ((port))*0x20)
#define PORT_REQ_CMD_REG4(port)  (PORT_REQ_CMD_REG4_0 + ((port))*0x20)
#define PORT_REQ_CMD_REG5(port)  (PORT_REQ_CMD_REG5_0 + ((port))*0x20)
#define PORT_RES_REG1(port)	(PORT_RES_REG1_0 + ((port))*0x20)
#define PORT_RES_REG0(port)	(PORT_RES_REG0_0 + ((port))*0x20)

#define LRO_FID(port)	   (LRO_FID_0 + ((port)*0x8))
#define LRO_TO_REG(port)   (LRO_TO_REG_0 + ((port)*0x8))
#define LRO_OC_FLAG(port,ctx)  (LRO_OC_FLAG_0 + ((port*2)+ctx)*0x100)
#define LRO_OC_OWNER(port,ctx)  (LRO_OC_OWNER_0 + ((port*2)+ctx)*0x100)

#define LRO_MAX_AGP 20
#define PMAC_HDR_SIZE 8
#define L2_HDR_LEN 14

#define DEFAULT_WAIT_CYCLES 1000

/*unsigned int toe_membase = 0xE2000000;*/
static unsigned char __iomem *ltq_toe_membase; /* Virtual */
/*static const unsigned char __iomem *lro_sram_membase_res0 = (unsigned char *)0xE2013000;*/
static unsigned char __iomem *lro_sram_membase_res0;
/*static unsigned int lro_sram_membase_res1 = 0xE2013100;*/

static struct device *g_toe_dev;

#undef LRO_DEBUG
#define ZERO_SRAM_DBG

#ifdef LRO_DEBUG
#define LRO_MAX_DBG_INFO 20000
struct lro_dbg_info {
	u32 except_reason;
	u32 tcp_seq_no;
	u32 aggr_len;
}lro_dbg_info;
static struct lro_dbg_info dbg_info[LRO_MAX_DBG_INFO];
u32 dbg_head = 0;
#endif
static unsigned int tso_num_tx[LTQ_MAX_TSO_PORTS];

static unsigned char ltq_large_buf[NR_CPUS][65536]__attribute__((aligned(32)));
unsigned char *toe_large_debug_ptr;
static struct proc_dir_entry *g_toe_dir;
#define LRO_MAX_EXCEPTION_COUNT 9
static u32 lro_num_except[LRO_MAX_EXCEPTION_COUNT], lro_num_success;
static u32 lro_budget_left[21];
static u32 lro_num_except_entries[32];
static u32 g_unmatched_entry;

static int ltq_toe_exit(struct platform_device *pdev);
static int tso_configure_dma(void);
static void configure_tso(void);

#ifndef CONFIG_LTQ_TOE_USE_SW_POLLING
static void ltq_tso_tasklet(unsigned long);
static irqreturn_t ltq_tso_tx_int(int irq, void *_port);
static struct tasklet_struct tso_tasklet[NR_CPUS];
#endif

#ifdef USE_TIMER_FOR_SESSION_STOP
static void lro_timer_fn(unsigned long data);
static unsigned long lro_time;
#endif

static void ltq_lro_tasklet(unsigned long);
static struct tasklet_struct lro_tasklet[LTQ_MAX_LRO_PORTS];
static struct tasklet_struct lro_exception_tasklet;

static void ltq_lro_ovflow_tasklet(unsigned long);
static struct tasklet_struct lro_ovflow_tasklet;

static void configure_lro(void);
int lro_stop_flow (int session_id, int timeout, int flags);
int lro_start_flow (int session_id, int timeout, int flags, struct cpumask cpumask);
static void lro_process_output_context(int port, int oc_flag_no);

spinlock_t tso_tx_lock;	/*!< spin lock */
spinlock_t tso_register_lock;

#define skb_tso_size(x)       (skb_shinfo(x)->gso_size)

#define ltq_toe_r32(x)			ltq_r32(ltq_toe_membase + (x))
#define ltq_toe_w32(x, y)		ltq_w32(x, ltq_toe_membase + (y))
#define ltq_toe_w32_mask(x, y, z)	\
	ltq_w32_mask(x, y,  ltq_toe_membase + (z))

#define	ltq_tso_w32_cmd(x, y, z)	\
	ltq_w32(x, (ltq_tso_port[z].membase + (y*4)))

#define	ltq_tso_r32(x, y)	\
	ltq_r32(ltq_tso_port[x].membase + (y*4))

#define ltq_lro_sram_mem(x, y, z) \
	ltq_r32(lro_sram_membase_res0 + (0x150 * x) + (0xA8 * y) + (z))

#define ltq_lro_sram_unmatched_mem(x, y) \
	ltq_r32(lro_sram_membase_res0 + 0xA80 + (0x10*(x)) + (y))

#define ltq_lro_sram_except_mem(x, y) \
	ltq_r32(lro_sram_membase_res0 + 0xB00 + (0x10*x) + (y))

#define toe_fill_cmd0(sphy, dphy, ie, g, chunk, len, others, port) do {   \
  unsigned int reg=(others);                      \
  reg = (sphy) << PORT_REQ_CMD_REG0_0_SPHY_POS |  \
        (dphy) << PORT_REQ_CMD_REG0_0_DPHY_POS | \
        (ie) << PORT_REQ_CMD_REG0_0_IE_POS | \
        (g) << PORT_REQ_CMD_REG0_0_G_POS | \
        (chunk) << PORT_REQ_CMD_REG0_0_CHUNK_SIZ_POS | \
        (len) << PORT_REQ_CMD_REG0_0_LEN_POS | \
        (others) ;                                        \
 /*   printk("REG0 = %X\n", reg); */\
		ltq_toe_w32(reg, PORT_REQ_CMD_REG0(port));  \
}while(0)

#define toe_fill_cmd1_frags(srcbuf, cachewb, port, lenn) do { \
	  	unsigned int physaddr; \
    if (cachewb) {  \
		physaddr = dma_map_single(g_toe_dev, (void *) srcbuf, \
							lenn, DMA_TO_DEVICE); \
			if (dma_mapping_error(g_toe_dev, physaddr)) { \
				pr_err("%s DMA map failed\n", __func__); \
				return -1; \
			} \
    }\
	/* CMD1 */  \
	pr_debug("physical address of the src data: %d = %08x and len = %d\n", \
					(unsigned int)srcbuf, (unsigned int)physaddr, lenn);\
   /* printk("REG1 = %X\n", physaddr); */\
	ltq_toe_w32(physaddr, PORT_REQ_CMD_REG1(port)); \
}while(0) 

#define toe_fill_cmd1(srcbuf, cachewb, port, lenn) do { \
	  	unsigned int physaddr; \
    if (cachewb) {  \
		physaddr = dma_map_single(g_toe_dev, (void *) srcbuf, \
							lenn, DMA_BIDIRECTIONAL); \
			if (dma_mapping_error(g_toe_dev, physaddr)) { \
				pr_err("%s DMA map failed\n", __func__); \
				return -1; \
			} \
    }\
	/* CMD1 */  \
	pr_debug("physical address of the src data: %d = %08x and len = %d\n", \
					(unsigned int)srcbuf, (unsigned int)physaddr, lenn);\
	ltq_r32(srcbuf + lenn - 4); \
   /* printk("REG1 = %X\n", physaddr); */\
	ltq_toe_w32(physaddr, PORT_REQ_CMD_REG1(port)); \
}while(0) 

#define toe_fill_cmd4(buf, port) do { \
			/* CMD4 */ \
	    unsigned int physaddr; \
			physaddr = CPHYSADDR((buf)); \
			pr_debug("address of the large buffer: = %08x \n", (unsigned int)(physaddr)); \
/*      printk("REG4 = %X\n", physaddr); */\
			ltq_toe_w32(physaddr, PORT_REQ_CMD_REG4(port)); \
}while(0) 

#define toe_fill_cmd4_sbk(buff, port) do { \
			/* CMD4 */ \
			pr_debug("address of the large buffer: = %08x \n", (unsigned int)(buff)); \
  /*    printk("REG4_skb = %X\n", buff); */\
			ltq_toe_w32(buff, PORT_REQ_CMD_REG4(port)); \
}while(0) 

#define toe_fill_cmd2(dw0, port) do { \
		/* CMD2 */ \
  /*  printk("REG2 = %X\n", dw0); */\
		ltq_toe_w32((dw0), PORT_REQ_CMD_REG2(port)); \
}while(0)

#define toe_fill_cmd3(dw1, port) do { \
		/* CMD3 */ \
  /*  printk("REG3 = %X\n", dw1); */\
		ltq_toe_w32((dw1), PORT_REQ_CMD_REG3(port)); \
}while(0)

#define toe_fill_cmd5(tso_mss, tirq, port) do { \
    unsigned int reg; \
		/* CMD5 */ \
		/* Set the segment size */ \
		pr_debug("segment size: %d \n", skb_tso_size(skb)); \
    reg = tso_mss << PORT_REQ_CMD_REG5_0_SEG_LEN_POS; \
		/* Enable TIRQ */ \
    reg |= (tirq) << PORT_REQ_CMD_REG5_0_TIRQ_POS; \
    /* Flag TSO to kickstart processing of this CMD */ \
    /* reg |= 1 << PORT_REQ_CMD_REG5_0_OWN_POS; */ \
    /* Write to CMD5 reg */ \
  /*  printk("REG5 = %X\n", reg); */\
		ltq_toe_w32(reg, PORT_REQ_CMD_REG5(port)); \
}while(0)

#define toe_get_cmd_own(port) { \
  unsigned long OwnReg;  \
  do { \
			OwnReg = ltq_toe_r32(PORT_REQ_CMD_REG5(port));  \
   /*   printk("Own = %X\n", OwnReg); */\
  }while (!test_bit(31, &OwnReg)); \
}


enum tso_desc_base {
	/* TSO Port 0 */
	DMA3_TOE_MEMCPY_CHAN_4_DESC_BASE = 0xA2010600,
	DMA3_TOE_MEMCPY_CHAN_5_DESC_BASE = 0xA2010620,
	DMA3_TOE_MEMCPY_CHAN_0_DESC_BASE = 0xA2014800,
	DMA3_TOE_MEMCPY_CHAN_1_DESC_BASE = 0xA2014820,
	DMA3_TOE_MEMCPY_CHAN_2_DESC_BASE = 0xA2014840,
	DMA3_TOE_MEMCPY_CHAN_3_DESC_BASE = 0xA2014860,
	DMA3_TOE_MEMCPY_CHAN_8_DESC_BASE = 0xA2015800,
	DMA3_TOE_MEMCPY_CHAN_9_DESC_BASE = 0xA2015810,
	DMA3_TOE_MEMCPY_CHAN_10_DESC_BASE = 0xA2015820,
	DMA3_TOE_MEMCPY_CHAN_11_DESC_BASE = 0xA2015830,

	/* TSO Port 1 */
	DMA3_TOE_MEMCPY_CHAN_6_DESC_BASE = 0xA2010700,
	DMA3_TOE_MEMCPY_CHAN_7_DESC_BASE = 0xA2010720,
	DMA3_TOE_MEMCPY_CHAN_30_DESC_BASE = 0xA2014900,
	DMA3_TOE_MEMCPY_CHAN_31_DESC_BASE = 0xA2014920,
	DMA3_TOE_MEMCPY_CHAN_32_DESC_BASE = 0xA2014940,
	DMA3_TOE_MEMCPY_CHAN_33_DESC_BASE = 0xA2014960,
	DMA3_TOE_MEMCPY_CHAN_38_DESC_BASE = 0xA2015900,
	DMA3_TOE_MEMCPY_CHAN_39_DESC_BASE = 0xA2015910,
	DMA3_TOE_MEMCPY_CHAN_40_DESC_BASE = 0xA2015920,
	DMA3_TOE_MEMCPY_CHAN_41_DESC_BASE = 0xA2015930,

	/* TSO Port 2 */
	DMA3_TOE_MEMCPY_CHAN_34_DESC_BASE = 0xA2010800,
	DMA3_TOE_MEMCPY_CHAN_35_DESC_BASE = 0xA2010820,
	DMA3_TOE_MEMCPY_CHAN_54_DESC_BASE = 0xA2014a00,
	DMA3_TOE_MEMCPY_CHAN_55_DESC_BASE = 0xA2014a20,
	DMA3_TOE_MEMCPY_CHAN_42_DESC_BASE = 0xA2014a40,
	DMA3_TOE_MEMCPY_CHAN_43_DESC_BASE = 0xA2014a60,
	DMA3_TOE_MEMCPY_CHAN_48_DESC_BASE = 0xA2015a00,
	DMA3_TOE_MEMCPY_CHAN_49_DESC_BASE = 0xA2015a10,
	DMA3_TOE_MEMCPY_CHAN_50_DESC_BASE = 0xA2015a20,
	DMA3_TOE_MEMCPY_CHAN_51_DESC_BASE = 0xA2015a30,

	/* TSO Port 3 */
	DMA3_TOE_MEMCPY_CHAN_36_DESC_BASE = 0xA2010900,
	DMA3_TOE_MEMCPY_CHAN_37_DESC_BASE = 0xA2010920,
	DMA3_TOE_MEMCPY_CHAN_62_DESC_BASE = 0xA2014b00,
	DMA3_TOE_MEMCPY_CHAN_63_DESC_BASE = 0xA2014b20,
	DMA3_TOE_MEMCPY_CHAN_52_DESC_BASE = 0xA2014b40,
	DMA3_TOE_MEMCPY_CHAN_53_DESC_BASE = 0xA2014b60,
	DMA3_TOE_MEMCPY_CHAN_58_DESC_BASE = 0xA2015b00,
	DMA3_TOE_MEMCPY_CHAN_59_DESC_BASE = 0xA2015b10,
	DMA3_TOE_MEMCPY_CHAN_60_DESC_BASE = 0xA2015b20,
	DMA3_TOE_MEMCPY_CHAN_61_DESC_BASE = 0xA2015b30
};

#if 0
static irqreturn_t lro_port_except_isr (int irq, void *priv)
{
	//u32 int_status,except_flag, oc_flag_0, oc_flag_1;
	u32 int_status,except_flag;

	pr_debug ("%s called with irq: %d\n", __func__, irq);

	/* Mask the interrupt */
	ltq_toe_w32_mask(0, (1 << TOE_INT_MASK_LRO_EXP_POS), TOE_INT_MASK);

	/* Read the Exception flag */
	except_flag = ltq_toe_r32(LRO_EXP_EFLAG);

#if 0
	oc_flag_0 = ltq_toe_r32(LRO_OC_FLAG(0));
	oc_flag_1 = ltq_toe_r32(LRO_OC_FLAG(1));

	if (!(oc_flag_0 & LRO_OC_FLAG_0_EXCPT_MASK) || 
			!(oc_flag_1 & LRO_OC_FLAG_0_EXCPT_MASK)) {
		pr_info("BUG ! got exception, but OC_FLAG 2nd bit not set\n");
		/* Give the exception ownership back to LRO */
		ltq_toe_w32(except_flag, LRO_EXP_EFLAG);
	}

    if (except_flag & 0x1)
		ltq_toe_w32(1, LRO_EXP_EFLAG);

	if (except_flag & 0x2)
		ltq_toe_w32(2, LRO_EXP_EFLAG);
#endif

	/* Read the interrupt status */
	int_status = ltq_toe_r32(TOE_INT_STAT);

	/* Clear the exception and interrupt if there is any */
	if (int_status & TOE_INT_STAT_LRO_EXP_MASK)
		ltq_toe_w32(TOE_INT_STAT_LRO_EXP_MASK, TOE_INT_STAT);

	/* Unmask the interrupt */
	ltq_toe_w32_mask((1 << TOE_INT_MASK_LRO_EXP_POS), 0 , TOE_INT_MASK);

	return IRQ_HANDLED;
}
#endif

static irqreturn_t lro_port_except_isr (int irq, void *priv)
{
	uint32_t except_entries, int_status;
	int i;
	unsigned long tso_rl_flags;

	//printk("except_isr\n");

	/* Mask the interrupt */
	ltq_toe_w32_mask((1 << TOE_INT_MASK_LRO_EXP_POS), 0, TOE_INT_EN);

	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);

	/* Clear the exception and interrupt if there is any */
	int_status = ltq_toe_r32(TOE_INT_STAT);
	if(int_status & TOE_INT_STAT_LRO_EXP_MASK) {
		ltq_toe_w32(TOE_INT_STAT_LRO_EXP_MASK, TOE_INT_STAT);
	} else {
		/* Unmask the interrupt */
		ltq_toe_w32_mask(0, (1 << TOE_INT_MASK_LRO_EXP_POS), TOE_INT_EN);
		spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);
		return IRQ_HANDLED;
	}
	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);

	except_entries = ltq_toe_r32(LRO_EXP_EFLAG);
	for(i=0; i<32; i++) {
		if(except_entries&(1<<i)) lro_num_except_entries[i]++;
	}

	tasklet_schedule(&lro_exception_tasklet);

	return IRQ_HANDLED;
}

static void ltq_lro_ovflow_tasklet(unsigned long dev)
{
	u32 int_status, i, pos;
	volatile unsigned long except_flag, flag_wr = 0;
	unsigned long tso_rl_flags;

	/* Read the interrupt status */
	int_status = ltq_toe_r32(TOE_INT_STAT);

	/* Read the Exception flag */
	except_flag = ltq_toe_r32(LRO_EXP_EFLAG);

	/* Find the first bit set in the flag */
	for(i=0; i<(2*LTQ_MAX_LRO_PORTS); i++) {
		pos = ffs(except_flag);
		if (!pos) {
			break;
		} else {
			clear_bit(pos-1, &except_flag);
			flag_wr |= 1 << (pos-1);
		}
	}
	
	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);
	if (flag_wr) {
		pr_info("ecovfl writing: %x bcos except_flag = %x at pos =%d !\n", (unsigned int)flag_wr, (unsigned int)except_flag, pos);
		ltq_toe_w32(flag_wr, LRO_EXP_EFLAG);
	}

	/* unmask the interrupt */
	//ltq_toe_w32_mask((1 << TOE_INT_MASK_S22_POS), 0 , TOE_INT_MASK);
	ltq_toe_w32_mask(0, (1 << TOE_INT_EN_S22_POS), TOE_INT_EN);
	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);
}

static irqreturn_t lro_port_overflow_isr (int irq, void *priv)
{
#if 1
	unsigned long tso_rl_flags;

	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);
	ltq_toe_w32_mask((1 << TOE_INT_EN_S22_POS), 0, TOE_INT_EN);
	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);
	return IRQ_HANDLED;
#else
	unsigned long tso_rl_flags;

	/* mask the interrupt */
	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);
	ltq_toe_w32_mask((1 << TOE_INT_EN_S22_POS), 0, TOE_INT_EN);
	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);
	//ltq_toe_w32_mask(0, (1 << TOE_INT_MASK_S22_POS), TOE_INT_MASK);

	/* clear the interrupt */
	ltq_toe_w32((1 << TOE_INT_MASK_S22_POS), TOE_INT_STAT);

	/* schedule the tasklet for housekeeping */
	tasklet_schedule(&lro_ovflow_tasklet);
	//tasklet_schedule(&lro_tasklet[0]);

	return IRQ_HANDLED;
#endif
}

static irqreturn_t lro_port_context_isr (int irq, void *priv)
{
	struct ltq_lro_port *pport = (struct ltq_lro_port *)priv;
	unsigned long tso_rl_flags, int_status;

	pr_info_once("%s called with irq: %d\n", __func__, irq);


	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);
	int_status = ltq_toe_r32(TOE_INT_STAT);
	if (!(int_status & (1 << (pport->port_num + TOE_INT_MASK_LRO0_POS)))) {
#if 0
		printk("spurious interrupt for port %d (OC_OWNER %x/%x), (OC_FLAG %x/%x), EFLAG = %x, INT_STAT = %x, INT_MASK = %x DBG_INFO = %d\n", pport->port_num, ltq_toe_r32(LRO_OC_OWNER(pport->port_num, 0)), ltq_toe_r32(LRO_OC_OWNER(pport->port_num, 1)),
				ltq_toe_r32(LRO_OC_FLAG(pport->port_num, 0)),
				ltq_toe_r32(LRO_OC_FLAG(pport->port_num, 1)),
				ltq_toe_r32(LRO_EXP_EFLAG),
				int_status,
				ltq_toe_r32(TOE_INT_MASK),
				ltq_toe_r32(LRO_DBG_INFO));
#endif
		spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);
	    return IRQ_NONE;
	}

	/* Mask the interrupt */
	ltq_toe_w32_mask((1 << (pport->port_num + TOE_INT_MASK_LRO0_POS)), 0, TOE_INT_EN);
	//ltq_toe_w32_mask(0, (1 << (pport->port_num + TOE_INT_MASK_LRO0_POS)), TOE_INT_MASK);

	wmb();
	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);

	/* Schedule the tasklet for housekeeping */
	tasklet_schedule(&lro_tasklet[pport->port_num]);
	return IRQ_HANDLED;
}

#if 1
static inline void dump_data(u32 len, char *pData){
	int i;
	u32 *data = (u32 *) pData;
	for(i=0;i<len/4;i++){
		printk("%8.8x ",(u32)(data[i]));
		if (i % 4 == 3)
			printk("\n");
	}
	printk("\n");
}
#endif

#if 0
static inline void dump_data(u32 len, char *pData){
	int i;
	for(i=0;i<len;i++){
		printk("%2.2x ",(u8)(pData[i]));
		if (i % 16 == 15)
			printk("\n");
	}
	printk("\n");
}
#endif

static void lro_process_output_context(int port, int oc_flag_no)
{
	u32 oc_flag;
	u8 *data_ptr;
	int i, j, data_len, temp_len, real_len, no_segs;
	u16 frag_len, offset;
	u8 *frag_addr;
	struct sk_buff *skb;
	u32 desc0, desc1, desc2, desc3, out_of_sequence = 0;
	u32 except_flag = 0, except_reason = LRO_MAX_EXCEPTION_COUNT + 1;
#ifdef LRO_DEBUG
	struct tcphdr *tcp_hdr;
#endif

#ifdef CONFIG_USE_SKB_FRAGS_ARRAY
	struct page *page;
#else
	struct sk_buff *frag_skb = NULL, *last_frag_skb = NULL;
#endif
	unsigned long tso_rl_flags;

	oc_flag = ltq_toe_r32(LRO_OC_FLAG(port, oc_flag_no));
	pr_debug("LRO done and OC_FLAG: %x\n", oc_flag);
	no_segs = (oc_flag & LRO_OC_FLAG_0_SEG_NR_MASK) >> LRO_OC_FLAG_0_SEG_NR_POS;
	pr_debug("First look into LRO success and later exception.. \n");

	if (oc_flag & LRO_OC_FLAG_0_EXCPT_MASK) {
			except_flag = ltq_toe_r32(LRO_EXP_EFLAG);

			if (except_flag & (1 << ((port*2) + oc_flag_no)))
				except_flag = 1;
			else
				except_flag = 0;
			pr_debug("LRO exception with EXP_EFLAG = %x ..\n", except_flag);
			except_reason = (oc_flag & LRO_OC_FLAG_0_EXP_CASE_MASK) >> LRO_OC_FLAG_0_EXP_CASE_POS;
			if (except_flag && except_reason != 6)
				out_of_sequence = 1;
			if (except_reason < LRO_MAX_EXCEPTION_COUNT)
				lro_num_except[except_reason]++;
			else 
				pr_err("spurious exception !!\n");
			if ((except_reason == 0) && (no_segs == 0))
				pr_info("flush exception with no_segs = %d for OC..%d\n", no_segs, oc_flag_no);
	}

	if (no_segs > 0) {

		if (no_segs == LRO_MAX_AGP) {
			lro_num_success++;
		} else if (except_reason == (LRO_MAX_EXCEPTION_COUNT + 1)) {
			pr_info("BUG! no exception no_segs = %d and OC_FLAG = %x!\n", no_segs, oc_flag);
		}
				desc0 = ltq_lro_sram_mem(port, oc_flag_no, 0);
				desc1 = ltq_lro_sram_mem(port, oc_flag_no, 4);
				desc2 = ltq_lro_sram_mem(port, oc_flag_no, 8);
				desc3 = ltq_lro_sram_mem(port, oc_flag_no, 0xc);

#ifdef ZERO_SRAM_DBG
				ltq_w32(0xffffffff, lro_sram_membase_res0 + (0x150 * (port)) + (0xA8 * (oc_flag_no)) + (8));
#endif
				/* Build the SKB */
				data_len = desc3 & 0x0000FFFF;
				data_ptr = (unsigned char *) __va(desc2);
				offset = (desc3 & 0x3800000) >> 23;
				dma_cache_inv((unsigned long) data_ptr+offset, data_len);

				temp_len = data_len + 128 + NET_IP_ALIGN + NET_SKB_PAD;
				real_len = SKB_DATA_ALIGN(temp_len) + SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
				/*first_len = ltq_toe_r32(lro_sram_membase_res0 + 0xc) & 0x0000FFFF;*/
				if (port == 0xff)
					pr_info("buffer pointer of the first packet: %x and length:%d no_segs = %d\n", (unsigned int)data_ptr, real_len, no_segs);
				//skb = build_skb((void *) (data_ptr + ((desc3 & 0x3800000) >> 23) - 128 - NET_IP_ALIGN), real_len);
				skb = cbm_build_skb((void *) (data_ptr + (offset - 128 - NET_IP_ALIGN - NET_SKB_PAD)), real_len, GFP_ATOMIC);

				/* Get the Descriptor words */
				if (skb) {
					skb_reserve(skb,  128 + NET_IP_ALIGN + NET_SKB_PAD);
					skb->DW0 = desc0;
					skb->DW1 = desc1;
					/*skb->DW2 = desc2;*/
					skb->DW2 = (u32)data_ptr;
					skb->DW3 = desc3;
					skb_put(skb, data_len);
				} else {
					pr_err("failure in allocating skb\r\n");
					return;
				}

				for (i = 1, j = 0; i < no_segs; i++, j++) {
					frag_addr = (uint8_t *) __va((ltq_lro_sram_mem(port, oc_flag_no , 0x14 + (j*8))));
#ifdef ZERO_SRAM_DBG
					ltq_w32(0xffffffff, lro_sram_membase_res0 + (0x150 * (port)) + (0xA8 * (oc_flag_no)) + (0x14 + (j*8)));
#endif
					if (port == 0xff)
						pr_info("Buffer pointer of the %i packet: %x\n", i, (unsigned int)frag_addr);
					frag_len = ltq_lro_sram_mem(port, oc_flag_no, 0x10 + (j*8)) & 0x0000FFFF;
					offset = (ltq_lro_sram_mem(port, oc_flag_no, 0x10 + (j*8)) & 0xFFFF0000) >> 16;
					if (port == 0xff)
						pr_info("Offset: %d Fragment Length: %d\n", offset, frag_len);
					dma_cache_inv((unsigned long) frag_addr+offset, frag_len);

#ifdef CONFIG_USE_SKB_FRAGS_ARRAY
					/* Append to the first skb */
					page = alloc_page(GFP_ATOMIC);
					memcpy(page_address(page), frag_addr + offset, frag_len);
					pr_debug("adding page: %d address: %x\n", j, (unsigned int)page_address(page));
					skb_add_rx_frag(skb, j, page, 0, frag_len, frag_len);
					/*skb_fill_page_desc(skb, j, page, 0, frag_len);*/

					/* Free the CBM buffer */
					cbm_buffer_free(0, (u32)frag_addr, 0);
#else
					real_len =  frag_len + SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
					frag_skb = cbm_build_skb((void *) (frag_addr + offset), real_len, GFP_ATOMIC);
					if (frag_skb) {
						skb_put(frag_skb, frag_len);
						if (last_frag_skb) 
							last_frag_skb->next = frag_skb;
						else
							skb_shinfo(skb)->frag_list = frag_skb;

						last_frag_skb = frag_skb;	
						skb->len += frag_len;
						skb->data_len += frag_len;
						skb->truesize += frag_skb->truesize;
						if (except_reason == 88)
							dump_data(100, (u8 *) frag_skb->data);
					} else {
							pr_err("%s: cbm_build_skb failed !!\n", __func__);
						}
#endif
				}
#ifdef LRO_DEBUG
				tcp_hdr = (struct tcphdr *) (skb->data + 42);
				dbg_info[dbg_head].tcp_seq_no = ntohl(tcp_hdr->seq);
				dbg_info[dbg_head].except_reason = except_reason;
				dbg_info[dbg_head].aggr_len = skb->len - 62;
				dbg_head = (dbg_head + 1) % LRO_MAX_DBG_INFO;
#endif
				/* Send it to datapath library */
				dp_rx(skb, 0);
	} 

	if (out_of_sequence) {

			desc0 = ltq_lro_sram_except_mem(port, 0);
			desc1 = ltq_lro_sram_except_mem(port, 4);
			desc2 = ltq_lro_sram_except_mem(port, 8);
			desc3 = ltq_lro_sram_except_mem(port, 0xc);

			/* Build the SKB */
			data_len = desc3 & 0x0000FFFF;
			data_ptr = (unsigned char *) __va(desc2);
			offset = (desc3 & 0x3800000) >> 23;
			dma_cache_inv((unsigned long) data_ptr+offset, data_len);

			temp_len = data_len + 128 + NET_IP_ALIGN + NET_SKB_PAD;
			real_len = SKB_DATA_ALIGN(temp_len) + SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
			/*first_len = ltq_toe_r32(lro_sram_membase_res0 + 0xc) & 0x0000FFFF;*/
			
			if (port == 0xff)
				pr_debug("buffer pointer of the exception packet: %x and length:%d \n", (unsigned int)data_ptr, data_len);
			//skb = build_skb((void *) (data_ptr + ((desc3 & 0x3800000) >> 23) - 128 - NET_IP_ALIGN), real_len);
			skb = cbm_build_skb((void *) (data_ptr + (offset - 128 - NET_IP_ALIGN - NET_SKB_PAD)), real_len, GFP_ATOMIC);

			/* Get the Descriptor words */
			if (skb) {
				skb_reserve(skb,  128 + NET_IP_ALIGN + NET_SKB_PAD);
				skb->DW0 = desc0;
				skb->DW1 = desc1;
				/*skb->DW2 = desc2;*/
				skb->DW2 = (u32)data_ptr;
				skb->DW3 = desc3;
				skb_put(skb, data_len);
			} else {
				pr_err("failure in allocating skb\r\n");
				return;
			}

			if (except_reason == 88)
				dump_data(100, (u8 *) skb->data);
#ifdef LRO_DEBUG
			dbg_info[dbg_head].except_reason = except_reason;
			tcp_hdr = (struct tcphdr *) (skb->data + 42);
			dbg_info[dbg_head].tcp_seq_no = ntohl(tcp_hdr->seq);
			dbg_info[dbg_head].aggr_len = skb->len - 62;
			dbg_head = (dbg_head + 1) % LRO_MAX_DBG_INFO;
#endif

			dp_rx(skb, 0);

			asm("sync");
			if (except_reason == 0)
				pr_debug("eflag = %x\n", ltq_toe_r32(LRO_EXP_EFLAG));
	}

	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);

	/* Give the ownership back to LRO */
	ltq_toe_w32(LRO_OC_OWNER_0_OWNER_MASK, LRO_OC_OWNER(port,oc_flag_no));
	wmb();

	/* Give the exception ownership back to LRO */
	if (except_flag) {
		ltq_toe_w32(1 << ((port*2) + oc_flag_no), LRO_EXP_EFLAG);
	}
	asm("sync");

	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);

#if 0
	i=0;
	do {
		owner = ltq_toe_r32(LRO_OC_OWNER(port + oc_flag_no));
	} while (((owner & LRO_OC_OWNER_0_OWNER_MASK) == LRO_OC_OWNER_0_OWNER_MASK) && ((i++) < DEFAULT_WAIT_CYCLES));
	owner = ltq_toe_r32(LRO_OC_OWNER(port,oc_flag_no));
	if ((owner & LRO_OC_OWNER_0_OWNER_MASK) == LRO_OC_OWNER_0_OWNER_MASK)
		pr_err("couldn't give the ownership back to LRO..\n");
#endif
}

#if 0
static void ltq_lro_tasklet (unsigned long dev)
{
	u32 lro_done_0, port;
	struct ltq_lro_port *pport = (struct ltq_lro_port *)dev;
	int l_read = pport->to_read;
	u32 lro_done = 0;

	port = pport->port_num;

	/* Keep checking the ownership bit till its not HW */
	do {
		lro_done_0 = ltq_toe_r32(LRO_OC_OWNER(port,l_read));
		if ((lro_done_0 & LRO_OC_OWNER_0_OWNER_MASK)) {
			pr_debug("lro status: %x and current OC is %d \n", lro_done_0, l_read);
			lro_process_output_context(port, l_read);
			l_read = !l_read;
		}

		lro_done = ltq_toe_r32(LRO_OC_OWNER(port,l_read));
	} while(lro_done); 

	pport->to_read = l_read;
	/* Unmask the interrupt for other output context */
	ltq_toe_w32_mask((1 << (pport->port_num + TOE_INT_MASK_LRO0_POS)), 0, TOE_INT_MASK);

	return;
}

static void ltq_lro_tasklet (unsigned long dev)
{
	u32 lro_done_0, lro_done_1, port, oc_flag_no;
	struct ltq_lro_port *pport = (struct ltq_lro_port *)dev;
	//u16 lro_tasklet_buget = 20;
	unsigned long tso_rl_flags;

	port = pport->port_num;

	lro_done_0 = ltq_toe_r32(LRO_OC_OWNER(port, 0));
	lro_done_1 = ltq_toe_r32(LRO_OC_OWNER(port, 1));

	/* Keep checking the ownership bit till its not HW */
	do {
		if ((lro_done_0 >> LRO_OC_OWNER_0_OWNER_POS) & 0x1) {
			oc_flag_no = 0;
			pr_debug("lro status: %x and current OC is %d \n", lro_done_0, oc_flag_no);
			lro_process_output_context(port, oc_flag_no);
		}

		if ((lro_done_1 >> LRO_OC_OWNER_0_OWNER_POS) & 0x1) {
			oc_flag_no = 1;
			pr_debug("lro status: %x and current OC is %d \n", lro_done_1, oc_flag_no);
			lro_process_output_context(port, oc_flag_no);
		}
#if 0
		lro_tasklet_buget--;
#endif
		lro_done_0 = ltq_toe_r32(LRO_OC_OWNER(port, 0));
		lro_done_1 = ltq_toe_r32(LRO_OC_OWNER(port, 1));
	//} while(lro_done_0 && lro_done_1 && lro_tasklet_buget);
	} while(lro_done_0 || lro_done_1);

#if 0
	lro_done_0 = ltq_toe_r32(LRO_OC_OWNER(port, 0));
	lro_done_1 = ltq_toe_r32(LRO_OC_OWNER(port, 1));

	if ((lro_done_0 & LRO_OC_OWNER_0_OWNER_MASK) || (lro_done_1 & LRO_OC_OWNER_0_OWNER_MASK))	
		tasklet_schedule(&lro_tasklet[port]);

	/* Check if there was an exception overflow */
	spin_lock_irqsave(&tso_tx_lock, sys_flag);
	if (g_ecovfl) {
		for (i=port; i<2; i++) {
			if (g_except_flag & ((port*2) + i)) {
				clear_bit((port*2) + i, &g_except_flag);
				ltq_toe_w32(g_except_flag, LRO_EXP_EFLAG);
			}
		}
		g_ecovfl = 0;
	}
	spin_unlock_irqrestore(&tso_tx_lock, sys_flag);
#endif

	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);
	/* Unmask the interrupt for other output context */
	ltq_toe_w32_mask(0, (1 << (pport->port_num + TOE_INT_MASK_LRO0_POS)), TOE_INT_EN);
	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);
	return;
}
#endif

static void ltq_lro_exception_tasklet(unsigned long dev __maybe_unused)
{
	uint32_t entries, clear_entries=0;
	uint32_t desc0, desc1, desc2, desc3;
	int i, temp_len, real_len, data_len;
	unsigned long tso_rl_flags;
	static int read_pos = 0;
	u8 *data_ptr;
	struct sk_buff *skb;
	u16 offset;

	entries = ltq_toe_r32(LRO_EXP_EFLAG);

	if(entries & ~(LRO_EXP_EFLAG_UNMATCH_MASK | LRO_EXP_EFLAG_EXP_MASK)) {
		pr_err("flush entries found (EFLAG: %x)\n", entries);
	}

	for(i = read_pos; entries & (1 << (i + LRO_EXP_EFLAG_UNMATCH_POS)); i=(i+1)%8) {
		//if(i>23) printk("PHU: unexpected flush entries\n");
		desc0 = ltq_lro_sram_unmatched_mem(i, 0);
		desc1 = ltq_lro_sram_unmatched_mem(i, 4);
		desc2 = ltq_lro_sram_unmatched_mem(i, 8);
		desc3 = ltq_lro_sram_unmatched_mem(i, 0xc);
		printk("read unmatched buffer %#x from index %d because: %x\n", desc2, i, entries & (1 << (i + LRO_EXP_EFLAG_UNMATCH_POS)));
#ifdef ZERO_SRAM_DBG
		ltq_w32(0xffffffff, lro_sram_membase_res0 + 0xA80 + (0x10*i) + (8));
#endif
		spin_lock_irqsave(&tso_register_lock, tso_rl_flags);

		clear_entries = 1 << (i + LRO_EXP_EFLAG_UNMATCH_POS);
		ltq_toe_w32(clear_entries, LRO_EXP_EFLAG);

		asm("sync");

		spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);

		/* Build the SKB */
		data_len = desc3 & 0x0000FFFF;
		data_ptr = (unsigned char *) __va(desc2);
		offset = (desc3 & 0x3800000) >> 23;
		dma_cache_inv((unsigned long) data_ptr+offset, data_len);

		temp_len = data_len + 128 + NET_IP_ALIGN + NET_SKB_PAD;
		real_len = SKB_DATA_ALIGN(temp_len) + SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
			
		skb = cbm_build_skb((void *) (data_ptr + (offset - 128 - NET_IP_ALIGN - NET_SKB_PAD)), real_len, GFP_ATOMIC);

		/* Get the Descriptor words */
		if (skb) {
			skb_reserve(skb,  128 + NET_IP_ALIGN + NET_SKB_PAD);
			skb->DW0 = desc0;
			skb->DW1 = desc1;
			/*skb->DW2 = desc2;*/
			skb->DW2 = (u32)data_ptr;
			skb->DW3 = desc3;
			skb_put(skb, data_len);
		} else {
			pr_err("failure in allocating skb\r\n");
			return;
		}

		dp_rx(skb, 0);
		g_unmatched_entry++;
		/* Read the EEFLAG back */
		entries = ltq_toe_r32(LRO_EXP_EFLAG);
	}

	read_pos = i;

#if 0
	if(entries & LRO_EXP_EFLAG_UNMATCH_MASK )  {
		static int i=16;
		int j;
		printk("TOE_INT_STAT: %x\n", ltq_toe_r32(TOE_INT_STAT));
		for(j=0; j<0x10; j+=4) {
			desc0 = ltq_lro_sram_unmatched_mem(i-16, j);
			printk("PHU: exception context entry %d: %#x %s %s\n",
			       i,
			       desc0,
			       (u32)__va(desc0) ? "<- cbm!" : "",
			       (entries & (1 << i)) ? "<- set!" : ""
			      );
		}
		if(entries & (1 << i) ) {
			ltq_toe_w32(1 << i, LRO_EXP_EFLAG);
			if(i>23) i=16;
		}

	}
#endif
	/* Unmask the interrupt */
	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);
	ltq_toe_w32_mask(0, (1 << TOE_INT_MASK_LRO_EXP_POS), TOE_INT_EN);
	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);
	return;
}

static void ltq_lro_tasklet (unsigned long dev)
{
	uint32_t port, context_ready;
	uint32_t current_context;
	struct ltq_lro_port *pport = (struct ltq_lro_port *)dev;
	uint8_t lro_tasklet_budget = 20;
	unsigned long tso_rl_flags, oc_flag;
	static atomic_t scheduled[8] = {{0}};
	static uint32_t last_context[8] = {0};

	if(atomic_add_return(1, &scheduled[pport->port_num]) > 1) {
		printk("lro_tasklet concurrency detected on port %d\n", pport->port_num);
	}

	port = pport->port_num;

	current_context = last_context[port];
	BUG_ON(current_context > 1);
	context_ready = ltq_toe_r32(LRO_OC_OWNER(port, current_context));
	oc_flag = ltq_toe_r32(LRO_OC_FLAG(port, current_context));
	if(!context_ready) {
		printk("no context ready :(\n");
		printk("context status for other OC: %x\n", ltq_toe_r32(LRO_OC_OWNER(port, !current_context)));
		last_context[port] = !current_context;
		goto leave;
	}

	/* Keep checking the ownership bit till its not HW */
	do {
		if ((ltq_toe_r32(TOE_INT_STAT) & (1 << (port + TOE_INT_MASK_LRO0_POS)))) {
			/* Clear the interrupt */
			ltq_toe_w32((1 << (port + TOE_INT_MASK_LRO0_POS)), TOE_INT_STAT);
		}
		lro_process_output_context(port, current_context);
		current_context = !current_context;
		context_ready = ltq_toe_r32(LRO_OC_OWNER(port, current_context));
		oc_flag = ltq_toe_r32(LRO_OC_FLAG(port, current_context));
	} while(context_ready && --lro_tasklet_budget);

	lro_budget_left[lro_tasklet_budget]++;
	last_context[port] = current_context;

	if((ltq_toe_r32(TOE_INT_STAT) & (1 << (port + TOE_INT_MASK_LRO0_POS))) || context_ready) {
		tasklet_schedule(&lro_tasklet[port]);
		atomic_dec(&scheduled[port]);
		return;
	}

leave:
	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);
	/* Unmask the interrupt for other output context */
	ltq_toe_w32_mask(0, (1 << (port + TOE_INT_MASK_LRO0_POS)), TOE_INT_EN);
	//ltq_toe_w32_mask((1 << (port + TOE_INT_MASK_LRO0_POS)), 0, TOE_INT_MASK);
	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);
	if((ltq_toe_r32(TOE_INT_STAT) & (1 << (port + TOE_INT_MASK_LRO0_POS))))
		tasklet_schedule(&lro_tasklet[port]);
	atomic_dec(&scheduled[port]);
	return;
}

static struct ltq_lro_port *ltq_allocate_lro_port (void)
{
	int i;

	for (i = 0; i < LTQ_MAX_LRO_PORTS; i++) {
		if (!ltq_lro_port[i].in_use) {
			ltq_lro_port[i].in_use = 1;
			return &ltq_lro_port[i];
		}
	}
	return NULL;
}

int lro_start_flow (int session_id, int timeout, int flags, struct cpumask cpumask)
{
	struct ltq_lro_port *pport;
	u32 port;
	int ret;
	unsigned long tso_rl_flags;

	/* Allocate a LRO port for the session */
	pport = ltq_allocate_lro_port();

	if (!pport) {
		pr_debug("no more LRO ports \n");
		return -EBUSY;
	}

	port = pport->port_num;

	if((ltq_toe_r32(LRO_FID(port)) & LRO_FID_0_OWNER_MASK) == 0) {
		pr_debug("FID ownership still with the HW !\n");
		pport->in_use = 0;
		return -EBUSY;
	}

	pport->session_id = session_id & LRO_FID_0_LRO_FID_MASK;

	/*pr_info("%s called with session_id = %x and port is: %d \n", __func__, session_id & LRO_FID_0_LRO_FID_MASK, port);*/

	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);
	ltq_toe_w32(timeout, LRO_TO_REG(port));

	/* Test values */
	/* Set the timeout to large value */
	//ltq_toe_w32(0xffffffff, LRO_TO_REG(port));
	//ltq_toe_w32(0x7270e00, LRO_TO_REG(port));
	//ltq_toe_w32(0x4c25A00, LRO_TO_REG(port));
	//ltq_toe_w32(0xa270e00, LRO_TO_REG(port));
	//ltq_toe_w32(0x19fd94, LRO_TO_REG(port)); /*5ms*/
	//ltq_toe_w32(0x90000, LRO_TO_REG(port)); /*2ms*/
	//ltq_toe_w32(0xe666, LRO_TO_REG(port)); /* 200 us */

	/* Set the Flow ID */
	ltq_toe_w32((session_id & LRO_FID_0_LRO_FID_MASK) << LRO_FID_0_LRO_FID_POS, LRO_FID(port));
	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);

	wmb ();

	pr_info("started flow %u for session id = %x\n", port, ltq_toe_r32(LRO_FID(port)));

	/* Set the IRQ affinity */
	cpumask.bits[0] = 0x1;
	ret = irq_set_affinity(pport->irq_num, &cpumask);
	if (ret) {
		pr_err("%s: can not set affinity for IRQ - %d", __func__, pport->irq_num);
		return ret;
	}

	ret = irq_set_affinity(190, &cpumask);
	if (ret) {
		pr_err("%s: can not set affinity for IRQ - %d", __func__, 190);
		return ret;
	}

	//enable_irq(pport->irq_num);
	return ret;
}
EXPORT_SYMBOL(lro_start_flow);

static struct ltq_lro_port *ltq_lookup_lro_port(int session_id)
{
	int i;

	for (i = 0; i < LTQ_MAX_LRO_PORTS; i++) {
		if (ltq_lro_port[i].session_id == session_id) {
			return &ltq_lro_port[i];
		}
	}
	return NULL;
}

#ifdef USE_TIMER_FOR_SESSION_STOP
int lro_real_stop_flow (int session_id, int timeout, int flags)
{
	struct ltq_lro_port *pport;
	u32 port;
	unsigned long tso_rl_flags;

	/* Allocate a LRO port for the session */
	pport = ltq_lookup_lro_port(session_id & LRO_FID_0_LRO_FID_MASK);

	if (!pport)
		return -EINVAL;

	/*pr_info("%s called\n", __func__);*/

	/* Disable the interrupt */
	//disable_irq(pport->irq_num);

	port = pport->port_num;

	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);

	/* Set the S_END */
	ltq_toe_w32_mask(0, 1 << LRO_FID_0_S_END_POS, LRO_FID(port));

	/* Clear the OWNER */
	ltq_toe_w32_mask(LRO_FID_0_OWNER_MASK, 0,  LRO_FID(port));

	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);
	wmb();
	mdelay(5);

	pr_info("stopped flow %u for session_id = %x\n", port, ltq_toe_r32(LRO_FID(port)));

#if 0
	while((ltq_toe_r32(LRO_FID(port)) & LRO_FID_0_OWNER_MASK) == 0) {
		pr_err_ratelimited("not owner of LRO_FID(%d): %x! (INT_STAT: %x DBG_INFO: %x EFLAG: %x OC_OWNER: %x/%x OC_FLAG: %x/%x)\n",
				   port,
				   ltq_toe_r32(LRO_FID(port)),
				   ltq_toe_r32(TOE_INT_STAT),
				   ltq_toe_r32(LRO_DBG_INFO),
				   ltq_toe_r32(LRO_EXP_EFLAG),
				   ltq_toe_r32(LRO_OC_OWNER(port, 0)),
				   ltq_toe_r32(LRO_OC_OWNER(port, 1)),
				   ltq_toe_r32(LRO_OC_FLAG(port, 0)),
				   ltq_toe_r32(LRO_OC_FLAG(port, 1))
				  );
		mdelay(200);
	}
#endif

	/* Make the port available */
	pport->in_use = 0;

	return 0;
}

static void lro_timer_fn (unsigned long data)
{
	struct ltq_lro_port *pport = (struct ltq_lro_port *)data;

	WARN_ON(!pport->in_use);

	pr_debug("LRO timer fired, session to be stopped !\n");
	lro_real_stop_flow(pport->session_id, 0, 0);
}

int lro_stop_flow (int session_id, int timeout, int flags)
{
	struct ltq_lro_port *pport;

	/* Lookup the LRO port */ 
	pport = ltq_lookup_lro_port(session_id & LRO_FID_0_LRO_FID_MASK);

	if (!pport) {
		pr_err("couldn't find the LRO port for session id: %d\n", session_id);
		return -EINVAL;
	}

	pr_info("triggering the stop the flow for %d\n", session_id);

	/* Flag the session should be stopped */
	mod_timer(&pport->lro_timer, jiffies + lro_time);

	return 0;
}
EXPORT_SYMBOL(lro_stop_flow);

#else

int lro_stop_flow (int session_id, int timeout, int flags)
{
	struct ltq_lro_port *pport;
	u32 port;
	unsigned long tso_rl_flags;

	/* Allocate a LRO port for the session */
	pport = ltq_lookup_lro_port(session_id & LRO_FID_0_LRO_FID_MASK);

	if (!pport)
		return -EINVAL;

	/*pr_info("%s called\n", __func__);*/

	/* Disable the interrupt */
	//disable_irq(pport->irq_num);

	port = pport->port_num;

#if 1
	while((ltq_toe_r32(LRO_FID(port)) & LRO_FID_0_OWNER_MASK) == 0) {
		pr_err_once("not owner of LRO_FID in stop !\n");
	}
#endif

	spin_lock_irqsave(&tso_register_lock, tso_rl_flags);

	/* Set the S_END */
	ltq_toe_w32_mask(0, 1 << LRO_FID_0_S_END_POS, LRO_FID(port));

	/* Clear the OWNER */
	ltq_toe_w32_mask(LRO_FID_0_OWNER_MASK, 0,  LRO_FID(port));

	spin_unlock_irqrestore(&tso_register_lock, tso_rl_flags);
	wmb();

	pr_debug("stopped flow %u for session_id = %x\n", port, ltq_toe_r32(LRO_FID(port)));
	mdelay(5);

	/* Make the port available */
	pport->in_use = 0;

	return 0;
}
EXPORT_SYMBOL(lro_stop_flow);
#endif

static int tso_configure_dma (void)
{
	int rxchan;
	int txchan;

	/* ----------------------------*/
	/*      TSO Port 0 channels   */
	/* ----------------------------*/

	pr_debug ("Initializing TSO Port 0 channels..\n");

	rxchan = DMA3_TOE_MEMCOPY_CLASS2_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS2_TX;

	/* Configure Pre-Processing Channels (4,5) */
	if ((ltq_request_dma(rxchan, "dma3 rx 4")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 4 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 5")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 5 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_4_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 4\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_5_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 5\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);

#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Configure Seg0 channels (0,1) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS0_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS0_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 0")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 0 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 1")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 1 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_0_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 0\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_1_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 1\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);

#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Configure Seg1 channels (2,3) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS1_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS1_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 2")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 2 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 3")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 3 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_2_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 0\r\n", __func__);
    }

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_3_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 1\r\n", __func__);
    }

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);

#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Enqueue 0 channels (8,9) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS4_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS4_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 8")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 8 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 9")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 9 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_8_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 8\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_9_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 9\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);

#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Enqueue 1 channels (10,11) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS5_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS5_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 10")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 10 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 11")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 11 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_10_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 10\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_11_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 11\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);

#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	pr_debug ("Initializing TSO Port 0 channels: Done !\n");

#if 1
	/* ----------------------------*/
	/*      TSO Port 1 channels   */
	/* ----------------------------*/

	pr_debug ("Initializing TSO Port 1 channels !\n");

	rxchan = DMA3_TOE_MEMCOPY_CLASS3_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS3_TX;

	/* Configure Pre-Processing Channels (6,7) */
	if ((ltq_request_dma(rxchan, "dma3 rx 6")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 6 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 7")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 7 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_6_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 6\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_7_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 7\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);

#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Configure Seg0 channels (30,31) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS15_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS15_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 30")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 30 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 31")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 31 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_30_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 0\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_31_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 1\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);

#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Configure Seg1 channels (32,33) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS16_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS16_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 32")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 32 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 33")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 33 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_32_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 32\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_33_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 33\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);

#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Enqueue 0 channels (38,39) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS19_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS19_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 38")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 38 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 39")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 39 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_38_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 38\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_39_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 39\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);

#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Enqueue 1 channels (40,41) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS20_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS20_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 40")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 40 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 41")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 41 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_40_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 40\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_41_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 41\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	pr_debug ("Initializing TSO Port 1 channels: Done !\n");

	/* ----------------------------*/
	/*      TSO Port 2 channels   */
	/* ----------------------------*/
	pr_debug ("Initializing TSO Port 2 channels ! \n");

	rxchan = DMA3_TOE_MEMCOPY_CLASS17_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS17_TX;

	/* Configure Pre-Processing Channels (34,35) */
	if ((ltq_request_dma(rxchan, "dma3 rx 34")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 34 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 35")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 35 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_34_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 34\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_35_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 35\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Configure Seg0 channels (54,55) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS27_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS27_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 54")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 54 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 55")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 55 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_54_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 54\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_55_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 55\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Configure Seg1 channels (42,43) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS21_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS21_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 42")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 42 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 43")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 43 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_42_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 42\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_43_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 43\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Enqueue 0 channels (48,49) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS24_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS24_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 48")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 48 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 49")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 49 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_48_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 48\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t) DMA3_TOE_MEMCPY_CHAN_49_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 49\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Enqueue 1 channels (50,51) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS25_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS25_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 50")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 50 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 51")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 51 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_50_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 50\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_51_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 51\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	pr_debug ("Initializing TSO Port 2 channels: Done! \n");

	/* ----------------------------*/
	/*      TSO Port 3 channels   */
	/* ----------------------------*/

	pr_debug ("Initializing TSO Port 3 channels ! \n");

	rxchan = DMA3_TOE_MEMCOPY_CLASS18_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS18_TX;

	/* Configure Pre-Processing Channels (36,37) */
	if ((ltq_request_dma(rxchan, "dma3 rx 36")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 36 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 37")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 37 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_36_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 36\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_37_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 37\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Configure Seg0 channels (62,63) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS31_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS31_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 62")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 62 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 63")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 63 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_62_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 62\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_63_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 63\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Configure Seg1 channels (52,53) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS26_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS26_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 52")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 52 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 53")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 53 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_52_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 52\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_53_DESC_BASE, 2)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 53\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);

	/* Enqueue 0 channels (58,59) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS29_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS29_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 58")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 58 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 49")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 59 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_58_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 58\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_59_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 59\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);


	/* Enqueue 1 channels (60,61) */
	rxchan = DMA3_TOE_MEMCOPY_CLASS30_RX;
	txchan = DMA3_TOE_MEMCOPY_CLASS30_TX;

	if ((ltq_request_dma(rxchan, "dma3 rx 60")) < 0)
		pr_debug(" %s failed to open chan for dma3 rx chan 60 \n", __func__);

	if ((ltq_request_dma(txchan, "dma3 tx 61")) < 0)
		pr_debug(" %s failed to open chan for dma3 tx chan 61 \n", __func__);

	if ((ltq_dma_chan_desc_cfg(rxchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_60_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for rxchan 60\r\n", __func__);
	}

	if ((ltq_dma_chan_desc_cfg(txchan, (dma_addr_t)DMA3_TOE_MEMCPY_CHAN_61_DESC_BASE, 1)) < 0) {
		pr_debug(" %s failed to setup chan desc for txchan 61\r\n", __func__);
	}

	ltq_dma_chan_irq_enable(rxchan);
	ltq_dma_chan_irq_enable(txchan);

#if 0
	ltq_dma_chan_enable_polling(rxchan);
	ltq_dma_chan_enable_polling(txchan);
	ltq_dma_chan_desc_info(rxchan);
	ltq_dma_chan_desc_info(txchan);
#endif

	ltq_dma_chan_on(rxchan);
	ltq_dma_chan_on(txchan);


	pr_debug ("Initializing TSO Port 3 channels: Done! \n");
#endif
	return 0;
}

static void configure_lro(void)
{
	int i;
	u32 lro_en = 0;

	/* Maximum aggregation size to 2 */
	ltq_toe_w32_mask(TSO_GCTRL_LRO_MAX_AGP_MASK, (LRO_MAX_AGP << TSO_GCTRL_LRO_MAX_AGP_POS), TSO_GCTRL);

	/* Enable all the LRO ports */
	for (i = 0; i < LTQ_MAX_LRO_PORTS; i++)
		lro_en |= (1 << (TSO_GCTRL_LRO0_EN_POS + i));
	ltq_toe_w32_mask(0, lro_en, TSO_GCTRL);

	/* Enable the interrupts */
	for (i = 0; i < LTQ_MAX_LRO_PORTS; i++)
		lro_en |= (1 << (TOE_INT_EN_LRO0_POS + i));
	ltq_toe_w32_mask(0, lro_en, TOE_INT_EN);

	ltq_toe_w32_mask(0, (1 << TOE_INT_EN_LRO_EXP_POS), TOE_INT_EN);
#if 0
	ltq_toe_w32_mask(0, (1 << TOE_INT_EN_S20_POS), TOE_INT_EN);
	ltq_toe_w32_mask(0, (1 << TOE_INT_EN_S21_POS), TOE_INT_EN);
	ltq_toe_w32_mask(0, (1 << TOE_INT_EN_S22_POS), TOE_INT_EN);
	ltq_toe_w32_mask(0, (1 << TOE_INT_EN_S23_POS), TOE_INT_EN);
#endif

	/* Unmask LRO interrupts */
	lro_en = 0;
	for (i = 0; i < LTQ_MAX_LRO_PORTS; i++)
		lro_en |= (1 << (TOE_INT_MASK_LRO0_POS + i));
	ltq_toe_w32_mask(lro_en, 0, TOE_INT_MASK);

	ltq_toe_w32_mask((1 << TOE_INT_MASK_LRO_EXP_POS), 0, TOE_INT_MASK);
#if 0
	ltq_toe_w32_mask((1 << TOE_INT_MASK_S20_POS), 0, TOE_INT_MASK);
	ltq_toe_w32_mask((1 << TOE_INT_MASK_S21_POS), 0, TOE_INT_MASK);
	ltq_toe_w32_mask((1 << TOE_INT_MASK_S22_POS), 0, TOE_INT_MASK);
	ltq_toe_w32_mask((1 << TOE_INT_MASK_S23_POS), 0, TOE_INT_MASK);
#endif
}

static void configure_tso (void)
{
	pr_debug ("Enabling the TSO..\n");

	/* Enable the TSO */
	ltq_toe_w32_mask(0, (1 << TSO_GCTRL_ENA_P0_POS), TSO_GCTRL);
	ltq_toe_w32_mask(0, (1 << TSO_GCTRL_ENA_P1_POS), TSO_GCTRL);
	ltq_toe_w32_mask(0, (1 << TSO_GCTRL_ENA_P2_POS), TSO_GCTRL);
	ltq_toe_w32_mask(0, (1 << TSO_GCTRL_ENA_P3_POS), TSO_GCTRL);

	pr_debug ("Setup the SRAM address as scratchpad.. \n");
	/* Setup the SRAM addresses for scratchpad */
	ltq_toe_w32(0x1F806000, HDR_BASE_SEG0);
	/*ltq_toe_w32(0x1F806800, toe_membase + HDR_BASE_SEG1);*/
	ltq_toe_w32(0x1F807000, HDR_BASE_SEG1);

	/* Disable the swap */
	/*ltq_toe_w32_mask(TSO_GCTRL_OCP_CTRL_ENDI_B_MASK, 0, TSO_GCTRL);*/
	/* Enable the word swap */
	ltq_toe_w32_mask(0, (1 << TSO_GCTRL_OCP_CTRL_ENDI_W_POS), TSO_GCTRL);

	/* Enable the DMA MCOPY Interrupts */
	ltq_toe_w32_mask(0, (1 << TSO_INTL_INT_EN_MCPY0_DONE_POS), TSO_INTL_INT_EN);
	ltq_toe_w32_mask(0, (1 << TSO_INTL_INT_EN_MCPY1_DONE_POS), TSO_INTL_INT_EN);
	ltq_toe_w32_mask(0, (1 << TSO_INTL_INT_EN_MCPY2_DONE_POS), TSO_INTL_INT_EN);
	ltq_toe_w32_mask(0, (1 << TSO_INTL_INT_EN_MCPY3_DONE_POS), TSO_INTL_INT_EN);

	/* Enable the interrupts */
	ltq_toe_w32_mask(0, (1 << TOE_INT_EN_TOE0_POS), TOE_INT_EN);
	ltq_toe_w32_mask(0, (1 << TOE_INT_EN_TOE1_POS), TOE_INT_EN);
	ltq_toe_w32_mask(0, (1 << TOE_INT_EN_TOE2_POS), TOE_INT_EN);
	ltq_toe_w32_mask(0, (1 << TOE_INT_EN_TOE3_POS), TOE_INT_EN);

	/* Unmask only TSO interrupts */
#ifndef CONFIG_LTQ_TOE_USE_SW_POLLING
	ltq_toe_w32_mask((1 << TOE_INT_MASK_TOE0_POS), 0, TOE_INT_MASK);
	ltq_toe_w32_mask((1 << TOE_INT_MASK_TOE1_POS), 0, TOE_INT_MASK);
	ltq_toe_w32_mask((1 << TOE_INT_MASK_TOE2_POS), 0, TOE_INT_MASK);
	ltq_toe_w32_mask((1 << TOE_INT_MASK_TOE3_POS), 0, TOE_INT_MASK);
#endif
}

#ifndef CONFIG_LTQ_TOE_USE_SW_POLLING
static void ltq_tso_tasklet(unsigned long cpu_id)
{
	u32 tso_done;
	/*ltq_tso_port_t *port = &(ltq_tso_port[cpu_id]);*/
	struct sk_buff *skb;

    tso_done = ltq_toe_r32(PORT_RES_REG1(cpu_id));
    tso_done = 1 & (tso_done >> PORT_RES_REG1_0_DONE_POS);

    if (tso_done) {
			/* free the skb */
			skb = (struct sk_buff *) ltq_toe_r32(PORT_RES_REG0(cpu_id));
			pr_debug("skb = %x\n", (unsigned int)skb);
			dev_kfree_skb_any(skb);

			/* Unmask the interrupt */
			ltq_toe_w32_mask(1<<cpu_id, 0, TOE_INT_MASK);
	} else {
		pr_err("TSO not done ! but got the interrupt \n");
	}
	
	return;
}

static irqreturn_t ltq_tso_tx_int(int irq, void *_port)
{
	int cpu = smp_processor_id();
	struct sk_buff *skb;
	u32 tso_done;

	/* Mask the interrupt */
	ltq_toe_w32_mask(0, (1<<cpu), TOE_INT_MASK);

	/* Clear the interrupt */
	ltq_toe_w32_mask(0, (1<<cpu), TOE_INT_STAT);

	skb = (struct sk_buff *) ltq_toe_r32(PORT_RES_REG0(cpu));
	pr_debug("skb = %x\n", (unsigned int)skb);
	dev_kfree_skb_any(skb);

    tso_done = ltq_toe_r32(PORT_RES_REG1(cpu));

	/* Unmask the interrupt */
	ltq_toe_w32_mask(1<<cpu, 0, TOE_INT_MASK);

	/* Schedule the tasklet for housekeeping */
	//tasklet_schedule(&tso_tasklet[cpu]);
	return IRQ_HANDLED;
}
#endif

int ltq_tso_xmit (struct sk_buff *skb, int egress_port, int flags)
{
	int i, len;
#ifndef CONFIG_LTQ_TOE_USE_SW_POLLING
	unsigned long cmd5_reg, sys_flag;
#endif
	unsigned long tso_done = 0;
	void *frag_addr;
	const skb_frag_t *frag;
#ifndef CONFIG_LTQ_TOE_USE_SW_POLLING
	ltq_tso_port_t *p_tso_port;
#endif
	int port;
	struct skb_shared_info *shinfo=NULL;
  	unsigned long toe_g = 1;
  	unsigned long toe_sioc = 0;
  	unsigned long toe_last = 0;
  	unsigned char *cmd4_buf;

	/*spin_lock_irqsave(&tso_tx_lock, sys_flag);*/

#ifndef CONFIG_LTQ_TOE_USE_SW_POLLING
	p_tso_port = &ltq_tso_port[port];

	/* Wait till the Ownership belongs to CPU */
	do {
		cmd5_reg = ltq_toe_r32(PORT_REQ_CMD_REG5(port));
		pr_debug("cmd5_reg = %x\n", cmd5_reg);
	} while (!test_bit(31, &cmd5_reg));

	local_irq_save(sys_flag);
#else
	port = smp_processor_id();
#endif

	pr_debug("%s: called.. with len:%d on port: %d\n", 
					__FUNCTION__, skb->len, port);

  	cmd4_buf = (unsigned char *)ltq_large_buf[port];

  	shinfo = skb_shinfo(skb);

  	if (skb->data) {
		 if (shinfo->nr_frags == 0) {
		   toe_g = 0;
		   cmd4_buf = (unsigned char *)skb;
		   /* toe_last |= (1 << PORT_REQ_CMD_REG0_0_LAST_POS); 20170601 : fix issue IPS#176617 */
		 }
    } else if (shinfo->nr_frags == 1) {
		 toe_g = 0;
   	 	 cmd4_buf = (unsigned char *)skb;
   	     /* toe_last |= (1 << PORT_REQ_CMD_REG0_0_LAST_POS); 20170601 : fix issue IPS#176617 */
  	}
	
 	/* Setup 1st command of gather in cmd registers */
 	/* Check that CMD port is available */
 	toe_get_cmd_own(port);
 	len = skb->len - skb->data_len;
 	toe_fill_cmd0(1, 1, 1, toe_g, 2, len, toe_last, port);
 	toe_fill_cmd1(skb->data, ~toe_sioc, port, len);
 	toe_fill_cmd2(skb->DW0, port);
 	toe_fill_cmd3(skb->DW1, port);
 	toe_fill_cmd4(cmd4_buf, port);
	/*udelay(10);*/
	asm("sync");
	if (skb_tso_size(skb) > 1546)
 		toe_fill_cmd5(1546, 1, port);
	else
 		toe_fill_cmd5(skb_tso_size(skb), 1, port);

	/* Write the command registers to start the gathering*/
	for (i = 0; i < shinfo->nr_frags ; i++) {
			/* Check if last segment of Gather */
			if (i == (shinfo->nr_frags - 1)) {
				toe_last = (1 << PORT_REQ_CMD_REG0_0_LAST_POS) | (1 << PORT_REQ_CMD_REG0_0_IE_POS);
			}
			frag = &shinfo->frags[i];

			/* Check that CMD port is available */
			toe_get_cmd_own(port);
			/* CMD0 - Fill frag length */
			toe_fill_cmd0(1, 1, 1, toe_g, 2, skb_frag_size(frag), toe_last, port);

			/* CMD1 - Fill frag i */
			frag_addr = skb_frag_address(frag);
			toe_fill_cmd1_frags(frag_addr, ~toe_sioc, port, skb_frag_size(frag));

			/* */
			toe_fill_cmd4_sbk((u32)skb, port);

			/* CMD5 */
		if (skb_tso_size(skb) > 1546)
 			toe_fill_cmd5(1546, 1, port);
		else
			toe_fill_cmd5(skb_tso_size(skb), 1, port);

			pr_debug ("start for packet:%d with G.. \n", i);
	}

#ifdef CONFIG_LTQ_TOE_USE_SW_POLLING
	do {
		tso_done = ltq_toe_r32(PORT_RES_REG1(port));
		pr_debug ("checking tso status: %x\n", (unsigned int)tso_done);
		tso_done = 1 & (tso_done >> PORT_RES_REG1_0_DONE_POS);
		if (tso_done)
			break;
		/*udelay (1000);*/
	} while (1);

	if (skb_is_nonlinear(skb))
		pr_debug("!");
	pr_debug("tso_done !!!\n");

	/* Free the SKB */
	dev_kfree_skb_any(skb);

	/*spin_unlock_irqrestore(&tso_tx_lock, sys_flag);*/
#endif

#ifndef CONFIG_LTQ_TOE_USE_SW_POLLING
	local_irq_restore(sys_flag);
#endif
	tso_num_tx[port]++;
	return 0;
}
EXPORT_SYMBOL(ltq_tso_xmit);

static int toe_reg_read_proc(struct seq_file *s, void *v)
{
	seq_puts(s, "===============Global Regs ==================\n");
	seq_printf(s, "ToE base address 0x%08x\n", (unsigned int) ltq_toe_membase);
	seq_printf(s, "TSO_GCTRL: addr 0x%08x value 0x%08x\t\n", (unsigned int)(ltq_toe_membase + TSO_GCTRL), ltq_toe_r32(TSO_GCTRL));
	seq_printf(s, "HDR_BASE_SEG0: addr 0x%08x value 0x%08x\t\n", (unsigned int) (ltq_toe_membase + HDR_BASE_SEG0), ltq_toe_r32(HDR_BASE_SEG0));
	seq_printf(s, "HDR_BASE_SEG1: addr 0x%08x value 0x%08x\t\n", (unsigned int) (ltq_toe_membase + HDR_BASE_SEG1), ltq_toe_r32(HDR_BASE_SEG1));
	seq_printf(s, "TOE_INT_MASK: addr 0x%08x value 0x%08x\t\n", (unsigned int)(ltq_toe_membase + TOE_INT_MASK), ltq_toe_r32(TOE_INT_MASK));
	seq_printf(s, "TOE_INT_STAT: addr 0x%08x value 0x%08x\t\n", (unsigned int)(ltq_toe_membase + TOE_INT_STAT), ltq_toe_r32(TOE_INT_STAT));
	seq_printf(s, "=============== Port 0 TSO CMD Regs ============\n");
	seq_printf(s, "PORT_REQ_CMD_REG0_0: addr 0x%08x value 0x%08x\t\n", (unsigned int)(ltq_toe_membase + TSO_GCTRL), ltq_toe_r32(PORT_REQ_CMD_REG0_0));
	seq_printf(s, "PORT_REQ_CMD_REG1_0: addr 0x%08x value 0x%08x\t\n", (unsigned int)(ltq_toe_membase + TSO_GCTRL), ltq_toe_r32(PORT_REQ_CMD_REG1_0));
	seq_printf(s, "PORT_REQ_CMD_REG2_0: addr 0x%08x value 0x%08x\t\n", (unsigned int)(ltq_toe_membase + TSO_GCTRL), ltq_toe_r32(PORT_REQ_CMD_REG2_0));
	seq_printf(s, "PORT_REQ_CMD_REG3_0: addr 0x%08x value 0x%08x\t\n", (unsigned int)(ltq_toe_membase + TSO_GCTRL), ltq_toe_r32(PORT_REQ_CMD_REG3_0));
	seq_printf(s, "PORT_REQ_CMD_REG4_0: addr 0x%08x value 0x%08x\t\n", (unsigned int)(ltq_toe_membase + TSO_GCTRL), ltq_toe_r32(PORT_REQ_CMD_REG4_0));
	seq_printf(s, "PORT_REQ_CMD_REG5_0: addr 0x%08x value 0x%08x\t\n", (unsigned int)(ltq_toe_membase + TSO_GCTRL), ltq_toe_r32(PORT_REQ_CMD_REG5_0));
	seq_printf(s, "=============== Port 0 LRO Regs ============\n");
	seq_printf(s, "LRO_FID_0: addr 0x%08x value 0x%08x\t\n", (unsigned int) (ltq_toe_membase + LRO_FID_0), ltq_toe_r32(LRO_FID_0));
	seq_printf(s, "LRO_TO_REG_0: addr 0x%08x value 0x%08x\t\n", (unsigned int) (ltq_toe_membase + LRO_TO_REG_0), ltq_toe_r32(LRO_TO_REG_0));
	seq_printf(s, "LRO_OC_FLAG_0: addr 0x%08x value 0x%08x\t\n", (unsigned int) (ltq_toe_membase + LRO_OC_FLAG_0), ltq_toe_r32(LRO_OC_FLAG_0));
	return 0;
}

static int ltq_reg_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, toe_reg_read_proc, NULL);
}

static const struct file_operations ltq_toe_reg_proc_fops = {
	.open = ltq_reg_seq_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

static int lro_stats_read_proc(struct seq_file *s, void *v)
{
	int i,port;

	seq_puts(s, "===============LRO Stats==================\n");
	for (i=0; i<LRO_MAX_EXCEPTION_COUNT; i++)
		seq_printf(s, "Exceptions %i: %d\t\n", i, (unsigned int) lro_num_except[i]);
	seq_printf(s, "Number of success %d\t\n", (unsigned int) lro_num_success);
	seq_printf(s, "=============== Port 0 LRO Regs ============\n");
	seq_printf(s, "LRO_OC_FLAG_0: addr 0x%08x value 0x%08x\t\n", (unsigned int) (ltq_toe_membase + LRO_OC_FLAG_0), ltq_toe_r32(LRO_OC_FLAG_0));
	seq_printf(s, "LRO_OC_FLAG_1: addr 0x%08x value 0x%08x\t\n", (unsigned int) (ltq_toe_membase + LRO_OC_FLAG_1), ltq_toe_r32(LRO_OC_FLAG_1));
	seq_printf(s, "LRO_OC_OWNER_0: addr 0x%08x value 0x%08x\t\n", (unsigned int) (ltq_toe_membase + LRO_OC_OWNER_0), ltq_toe_r32(LRO_OC_OWNER_0));
	seq_printf(s, "LRO_OC_OWNER_1: addr 0x%08x value 0x%08x\t\n", (unsigned int) (ltq_toe_membase + LRO_OC_OWNER_1), ltq_toe_r32(LRO_OC_OWNER_1));
	seq_puts(s, "===============TSO Stats==================\n");
	for (port=0; port < LTQ_MAX_TSO_PORTS; port++)
		seq_printf(s, "Number of tx %d\t\n", (unsigned int) tso_num_tx[port]);
	return 0;
}

static int ltq_lro_stats_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, lro_stats_read_proc, NULL);
}

ssize_t lro_proc_write(struct file *file, const char *buf, size_t count,
			     loff_t *ppos)
{
	u32 flow_id;

	/* Read the Flow ID */
	flow_id = ltq_toe_r32(LRO_FID(0)) & LRO_FID_0_LRO_FID_MASK;
	pr_info("current flow id = %d\n", flow_id);

	/* Stop the flow */
	if (lro_stop_flow(flow_id, 0, 0) < 0)
		pr_err("session couldn't be deleted..\n");
	else
		pr_info("session %d deleted..\n", flow_id);
	return count;
}

static const struct file_operations ltq_lro_stats_proc_fops = {
	.open = ltq_lro_stats_seq_open,
	.read = seq_read,
	.write = lro_proc_write,
	.llseek = seq_lseek,
	.release = seq_release,
};

/* Debug info related proc */
#ifdef LRO_DEBUG
static void *lro_dbg_info_seq_start(struct seq_file *s, loff_t *pos)
{
	if (*pos > dbg_head)
		return NULL;
	return &dbg_info[*pos];
}

static void *lro_dbg_info_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	if (++*pos > dbg_head)
		return NULL;
	return &dbg_info[*pos];
}

static void lro_dbg_info_seq_stop(struct seq_file *s, void *v)
{

}

static int lro_dbg_info_seq_show(struct seq_file *s, void *v)
{
	struct lro_dbg_info *pinfo = (struct lro_dbg_info *)v;

	seq_puts(s, "===============LRO Debug info==================\n");
	seq_printf(s, "Except reason: %d \t\n", pinfo->except_reason);
	seq_printf(s, "TCP sequence no: %x \t\n", pinfo->tcp_seq_no);
	seq_printf(s, "Total aggr len: %d \t\n", pinfo->aggr_len);
	seq_puts(s, "<===============>\n");
	return 0;
}

static const struct seq_operations lro_debug_info_seq_ops = {
	.start = lro_dbg_info_seq_start,
	.next = lro_dbg_info_seq_next,
	.stop = lro_dbg_info_seq_stop,
	.show = lro_dbg_info_seq_show,
};

static int ltq_lro_dbg_info_seq_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &lro_debug_info_seq_ops);
}

static const struct file_operations ltq_lro_debug_info_proc_fops = {
	.open = ltq_lro_dbg_info_seq_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};
#endif

/** create proc for debug  info, used ifx_dma_init_module */
static int ltq_toe_proc_init (void)
{
	struct proc_dir_entry *entry;

	g_toe_dir = proc_mkdir("driver/ltq_toe", NULL);
	if (!g_toe_dir)
		return -ENOMEM;

	entry = proc_create("register", 0,
			g_toe_dir, &ltq_toe_reg_proc_fops);
	if (!entry)
		goto err1;

	entry = proc_create("lro_stats", 0,
			g_toe_dir, &ltq_lro_stats_proc_fops);
	if (!entry)
		goto err1;

#ifdef LRO_DEBUG
	entry = proc_create("lro_dbg_info", 0,
			g_toe_dir, &ltq_lro_debug_info_proc_fops);
	if (!entry)
		goto err1;
#endif

	return 0;
err1:
	remove_proc_entry("driver/ltq_toe", NULL);
	return -ENOMEM;
}

static int ltq_toe_init(struct platform_device *pdev)
{
	struct resource *r;
	struct resource irqres[15];
#ifndef CONFIG_LTQ_TOE_USE_SW_POLLING
	int tso_irq;
	struct cpumask cpumask;
#endif
	struct device_node *node = pdev->dev.of_node;
	int ret_val, i;

	/* Get the TOE base address */
	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (r == NULL) {
		dev_err(&pdev->dev, "platform_get_resource\n");
		ret_val = -ENOENT;
		return ret_val;
	}

	r = devm_request_mem_region(&pdev->dev, r->start, resource_size(r),
			pdev->name);
	if (!r) {
		dev_err(&pdev->dev, "failed to request memory region\n");
		ret_val = -ENXIO;
		return ret_val;
	}

	pr_debug ("ltq_toe_membase from DT: %x \n", (unsigned int) r->start);
	ltq_toe_membase = devm_ioremap_nocache(&pdev->dev,
		r->start, resource_size(r));

	if (!ltq_toe_membase) {
		dev_err(&pdev->dev, "failed to remap memory region\n");
		ret_val = -ENXIO;
		return ret_val;
	}

	/* Get the LRO output context base address */
	r = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (r == NULL) {
		dev_err(&pdev->dev, "platform_get_resource failed for LRO\n");
		ret_val = -ENOENT;
		return ret_val;
	}

	r = devm_request_mem_region(&pdev->dev, r->start, resource_size(r),
			pdev->name);
	if (!r) {
		dev_err(&pdev->dev, "failed to request LRO memory region\n");
		ret_val = -ENXIO;
		return ret_val;
	}

	pr_info ("lro_sram_membase_res0 from DT: %x \n", (unsigned int) r->start);
	lro_sram_membase_res0 = devm_ioremap_nocache(&pdev->dev,
		r->start, resource_size(r));

	if (!lro_sram_membase_res0) {
		dev_err(&pdev->dev, "failed to remap memory region for LRO\n");
		ret_val = -ENXIO;
		return ret_val;
	}

	/* Get the interrupt numbers */
	ret_val = of_irq_to_resource_table(node, irqres, 15);
	if (ret_val != 15) {
		dev_err(&pdev->dev,
			"failed to get irq for toe port\n");
		return -ENODEV;
	}

	pr_info("ltq_toe_membase: %x and lro_sram_membase_res0: %x\n", (unsigned int)ltq_toe_membase, (unsigned int)lro_sram_membase_res0);

	/* Initialise the 4 ports */
	for_each_online_cpu(i) {
		ltq_tso_port[i].membase = ltq_toe_membase + (i*0x20);

#ifndef CONFIG_LTQ_TOE_USE_SW_POLLING
		/* Register the interrupt handler for TSO */
		tso_irq = irqres[i+1].start;
		ret_val = request_irq(tso_irq, ltq_tso_tx_int,
									0, "tso_irq", NULL);
		if (ret_val) {
			pr_err("failed to request tso_tx_irq \n");
			return ret_val;
		}

		/* Set the affinity */
		cpumask.bits[0] = (1 << i);
		if (irq_set_affinity(tso_irq, &cpumask))
			pr_err("can not set affinity for IRQ - %d", tso_irq);

		tasklet_init(&tso_tasklet[i],
			ltq_tso_tasklet, (unsigned long) i);
#endif
	}

	/* Register the interrupt handlers for the LRO */
	for (i = 7; i < (7 + LTQ_MAX_LRO_PORTS); i++) {
		ltq_lro_port[i-7].port_num = i-7;
		ltq_lro_port[i-7].irq_num = irqres[i].start;
		ret_val = request_irq(irqres[i].start, lro_port_context_isr,
					0, "lro_irq", &ltq_lro_port[i-7]);
		if (ret_val) {
			pr_err("failed to request lro_irq \n");
			return ret_val;
		}
		tasklet_init(&lro_tasklet[i-7],
			ltq_lro_tasklet, (unsigned long) &ltq_lro_port[i-7]);
#ifdef USE_TIMER_FOR_SESSION_STOP
		init_timer(&ltq_lro_port[i-7].lro_timer);
		lro_time = msecs_to_jiffies(5000);
		ltq_lro_port[i-7].lro_timer.function = lro_timer_fn;
		ltq_lro_port[i-7].lro_timer.expires = jiffies + lro_time;
		ltq_lro_port[i-7].lro_timer.data = (unsigned int)&ltq_lro_port[i-7];
#endif
		//disable_irq(irqres[i].start);
	}

#if 1
	ret_val = request_irq(191, lro_port_except_isr,
					0, "lro_except_irq", NULL);
	if (ret_val) {
		pr_err("failed to request lro exception irq \n");
		return ret_val;
	}
	tasklet_init(&lro_exception_tasklet, ltq_lro_exception_tasklet, 0l);
#endif
	ret_val = request_irq(190, lro_port_overflow_isr,
					0, "lro_ovflow_irq", NULL);

	if (ret_val) {
		pr_err("failed to request lro overflow irq \n");
		return ret_val;
	}
	tasklet_init(&lro_ovflow_tasklet, ltq_lro_ovflow_tasklet, 0);

	/* Set up the DMA3 for ToE */
	tso_configure_dma();

	/* Enable the ToE at top level */
	ltq_toe_w32_mask(0, (CONST_TSO_GCTRL_ENABLE_EN << TSO_GCTRL_ENABLE_POS), TSO_GCTRL);

	/* Mask all the interrupts */
	ltq_toe_w32(0xffffffff, TOE_INT_MASK);

	/* Configure and enable the TSO */
	configure_tso();

	/* Configure and enable the LRO */
	configure_lro();

	/* Initialise the proc filesystem */
	ltq_toe_proc_init();

	spin_lock_init(&tso_register_lock);
	spin_lock_init(&tso_tx_lock);

	g_toe_dev = &pdev->dev;

	pr_info("TOE Init Done !!\n");
	return 0;
}

static int ltq_toe_exit(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id ltq_toe_match[] = {
	{ .compatible = "lantiq,toe-xrx500" },
	{},
};
MODULE_DEVICE_TABLE(of, ltq_toe_match);

static struct platform_driver ltq_toe_driver = {
	.probe = ltq_toe_init,
	.remove = ltq_toe_exit,
	.driver = {
		.name = "toe-xrx500",
		.owner = THIS_MODULE,
		.of_match_table = ltq_toe_match,
	},
};

module_platform_driver(ltq_toe_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Suresh Nagaraj, suresh.nagaraj@lantiq.com");
MODULE_DESCRIPTION("Lantiq TOE driver");
MODULE_SUPPORTED_DEVICE("Lantiq TOE IP module");
