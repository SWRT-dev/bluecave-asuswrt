/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2014~2015 Lei Chuanhua <chuanhua.lei@lantiq.com>
 *  Copyright(c) 2016 Intel Corporation.
 */

#ifndef DMA_H
#define DMA_H
#include <lantiq_dmax.h>

#define DMA_CHAN_MAGIC		-1

enum dma_endian {
	DMA_ENDIAN_TYPE0 = 0,
	DMA_ENDIAN_TYPE1,	/*!< Byte Swap(B0B1B2B3 => B1B0B3B2) */
	DMA_ENDIAN_TYPE2,	/*!< Word Swap (B0B1B2B3 => B2B3B0B1) */
	DMA_ENDIAN_TYPE3,	/*!< DWord Swap (B0B1B2B3 => B3B2B1B0) */
	DMA_ENDIAN_MAX,
};

enum dma_burst {
	DMA_BURSTL_2DW = 1,	/*!< 2 DWORD DMA burst length */
	DMA_BURSTL_4DW = 2,	/*!< 4 DWORD DMA burst length */
	DMA_BURSTL_8DW = 3,	/*!< 8 DWORD DMA burst length */
	DMA_BURSTL_16DW = 16,
};

enum dma_chan_on_off {
	DMA_CH_OFF = 0,	/*!< DMA channel is OFF */
	DMA_CH_ON = 1,	/*!< DMA channel is ON */
};

enum dma_pkt_drop {
	DMA_PKT_DROP_DISABLE = 0,
	DMA_PKT_DROP_ENABLE,
};

#define DMA_FLUSH_MEMCPY		1
#define DMA_CHAN_RST			1

enum dma_global_poll {
	DMA_CHAN_GLOBAL_POLLING_DIS = 0,
	DMA_CHAN_GLOBAL_POLLING_EN,
};

enum dma_port_txwgt {
	DMA_PORT_TXWGT0 = 0,
	DMA_PORT_TXWGT1,
	DMA_PORT_TXWGT2,
	DMA_PORT_TXWGT3,
	DMA_PORT_TXWGT4,
	DMA_PORT_TXWGT5,
	DMA_PORT_TXWGT6,
	DMA_PORT_TXWGT7,
	DMA_PORT_TXWGTMAX,
};

enum dma_chan_txwgt {
	DMA_CHAN_TXWGT0 = 0,
	DMA_CHAN_TXWGT1,
	DMA_CHAN_TXWGT2,
	DMA_CHAN_TXWGT3,
	DMA_CHAN_TXWGTMAX,
};

enum dma_arb {
	DMA_ARB_BURST = 0,
	DMA_ARB_MUL_BURST,
	DMA_ARB_PKT,
	DMA_ARB_MAX,
};
#define DMA_ARB_MUL_BURST_DEFAULT	4


#define DMA_IRQ_BUDGET		20

#define DMA_GLOBAL_POLLING_DEFAULT_INTERVAL	4

#define DMA_MAX_DESC_NUM	4095

#define DMA_MAX_PKT_SIZE	65535

#define DMA_PKT_SIZE_DEFAULT	2048

#define DMA_TX_PORT_DEFAULT_WEIGHT	1
/** Default Port Transmit weight value */
#define DMA_TX_CHAN_DEFAULT_WEIGHT	1

enum {
	DMA_CTRL_UNINITIALIZED = 0,
	DMA_CTRL_INITIALIZED,
};

#ifdef CONFIG_CPU_BIG_ENDIAN
#define DMA_DEFAULT_ENDIAN	DMA_ENDIAN_TYPE3
#else
#define DMA_DEFAULT_ENDIAN	DMA_ENDIAN_TYPE0
#endif

#define DMA_DEFAULT_BURST	DMA_BURSTL_16DW
#define DMA_DEFAULT_PKDROP	DMA_PKT_DROP_DISABLE
#define DMA_DEFAULT_DESC_LEN	1

#define DMA_OWN		1
#define CPU_OWN		0
#define DESC_OWN_DMA	0x80000000
#define DESC_CPT_SET	0x40000000
#define DESC_SOP_SET	0x20000000

struct dma_ctrl;
struct dma_port;
struct dmax_chan {
	struct dma_ctrl *controller;
	struct dma_port *port; /* back pointer */
	int nr;			/* Channel number */
#define DMA_TX_CH		0x1
#define DMA_RX_CH		0x2
#define DEVICE_ALLOC_DESC	0x4 /* Client allocate descriptor itself */
#define CHAN_IN_USE		0x8 /* Channel is in use already */
#define DEVICE_CTRL_CHAN	0x10 /* Device handle all dma actions itself */
#define DMA_HW_DESC		0x20 /* CBM or other hardware descriptor */
	u32 flags; /* central way or channel based way */
	enum dma_chan_on_off onoff;
	int rst;
	int irq;	/*!< DMA channel IRQ number */
	void *data;
	int lpbk_en;
	int lpbk_ch_nr;
	int pden;
	int curr_desc;
	int prev_desc;
	dma_addr_t desc_phys;	/* Descriptor base physical address */
	u32 desc_base;
	int desc_len;
	enum dma_chan_txwgt txwgt;
	int pkt_size;
	int p2pcpy;
	int global_buffer_len; /* Used for peri_to_peri */
	struct mutex ch_lock;
	const char *device_id;
	spinlock_t irq_lock;
	void **opt; /* Dynamic allocate */
	buffer_alloc_t alloc;
	buffer_free_t  free;
	intr_handler_t intr_handler;
	u32  cis;  /* Record channel interrupt status for later usage */
	int  lnr;   /* Logical channel number */
	bool desc_configured;
	void *priv; /* Client specific info */
};

struct dma_port {
	struct dma_ctrl *controller; /* back pointer */
	int pid;
	const char *name;
	enum dma_endian rxendi;
	enum dma_endian txendi;
	enum dma_burst rxbl;
	enum dma_burst txbl;
	enum dma_port_txwgt txwgt;
	enum dma_pkt_drop pkt_drop;
	int flush_memcpy; /* change to flags */
	int chan_nrs; /* For the current port */
	struct dmax_chan *chans; /* Dynamic allocate */
	spinlock_t port_lock;
};

struct dma_ctrl {
	int cid;
	const char *name;
#define DMA_CTL_64BIT		0x1
#define DMA_FLCTL		0x2
#define DMA_FTOD		0x4
#define DMA_DESC_IN_SRAM	0x8
#define DMA_DRB			0x10
#define DMA_EN_BYTE_EN		0x20
	u32 flags;
	void __iomem *membase;
	u32 chained_irq;
	u32 irq_base;
	u32 port_nrs; /* for the current controller */
	u32 chans;
	struct dma_port *ports; /* Dynamic allocate */
	u32 arb_type;
	u32 labcnt; /* Look ahead buffer counter */
	u32 pollcnt;
	spinlock_t ctrl_lock;
	struct tasklet_struct dma_tasklet;
	DECLARE_BITMAP(dma_int_status, MAX_DMA_CHAN_PER_PORT);
	atomic_t dma_in_process;
	u32 burst_mask;
	u32 desc_size;
	struct device *dev;
	struct proc_dir_entry *proc;
	u32 budget;
};
#endif /* DMA_H */
