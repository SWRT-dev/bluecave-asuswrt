/******************************************************************************
**
** FILE NAME    : ltqmips_usif_spi.c
** PROJECT      : IFX UEIP
** MODULES      : USIF for SPI
**
** DATE         : 03 June 2009
** AUTHOR       : Lei Chuanhua
** DESCRIPTION  : USIF for SPI Master/Slave
** COPYRIGHT    :       Copyright (c) 2009
**                      Infineon Technologies AG
**                      Am Campeon 1-12, 85579 Neubiberg, Germany
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
** HISTORY
** $Date        $Author         $Comment
** 03 Jun,2009  Lei Chuanhua    Initial version
*******************************************************************************/
/*!
  \file ltqmips_usif_spi.c
  \ingroup LANTIQ_USIF_SPI
  \brief usif spi driver source file
*/
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/signal.h>
#include <linux/interrupt.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/sysctl.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <asm/div64.h>

/* Project header */
#include <lantiq_soc.h>
#include <lantiq_dma.h>

#include "lantiq_usif_spi_intern.h"

#undef CONFIG_PROC_FS

#define LANTIQ_USIF_SPI_VER_MAJOR          1
#define LANTIQ_USIF_SPI_VER_MID            3
#define LANTIQ_USIF_SPI_VER_MINOR          2

#define LANTIQ_USIF_SPI_NAME             "lantiq_usif_spi"

#define LANTIQ_USIF_SPI_MAX_BAUD          50000000
#define LANTIQ_USIF_SPI_HALF_BAUD         25000000
#define LANTIQ_USIF_SPI_1MHZ_BAUD         1000000

#ifdef CONFIG_LANTIQ_USIF_SPI_DEBUG
#define INLINE
enum {
	USIF_MSG_TX_FIFO = 0x00000001,
	USIF_MSG_TX_DMA = 0x00000002,
	USIF_MSG_RX_FIFO = 0x00000004,
	USIF_MSG_RX_DMA = 0x00000008,
	USIF_MSG_INT = 0x00000010, /* Interrupt msg */
	USIF_MSG_CFG = 0x00000020,
	USIF_MSG_THREAD = 0x00000040,
	USIF_MSG_TASKLET = 0x00000080,
	USIF_MSG_DEBUG = 0x00000100,
	USIF_MSG_ERROR = 0x00000200,
	USIF_MSG_INIT = 0x00000400, /* Initialization msg */
	USIF_MSG_QUEUE = 0x00000800,
	USIF_MSG_LOCK = 0x00001000,
	USIF_MSG_CALLBACK = 0x00002000,
	USIF_MSG_DUPLEX = 0x00004000,
	USIF_MSG_ANY = 0xffffffff, /* anything */
};
static void lantiq_usif_spi_debug(struct lantiq_usif_port *port, const char *fmt,
		...);

#define LANTIQ_USIF_PRINT(_port, _m, _fmt, args...) do { \
	if ((_port)->usif_debug & (_m)) {              \
		lantiq_usif_spi_debug((_port), (_fmt), ##args);\
	} \
} while (0)

#else
#define INLINE inline
#define LANTIQ_USIF_PRINT(_port, _m, _fmt, ...)
#endif /* CONFIG_LANTIQ_USIF_SPI_DEBUG */

static unsigned int lantiq_get_usif_hz(void);

static struct lantiq_usif_port lantiq_usif_spi_port;

#ifdef CONFIG_PROC_FS
static struct proc_dir_entry *lantiq_usif_spi_proc = NULL;
static const char lantiq_usif_spi_proc_node_name[] = "driver/lantiq_usif_spi";
#endif

/**
 * \fn static void lantiq_usif_spi_start_tasklet(struct lantiq_usif_port *port)
 * \brief Trigger different schedule procedures according to different
 *  context. if caller is already in tasklet, it will be done in caller's
 *  tasklet
 *
 * \param   port       Pointer to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static void lantiq_usif_spi_start_tasklet(struct lantiq_usif_port *port)
{
	struct tasklet_struct *ptasklet;

	/*
	 * Calls the internal process to serve the queue. This routine would
	 * immediately return in case the USIF hardware is currently used to
	 * serve another request.
	 */
	ptasklet = &port->usif_txrxq;
	if (in_irq()) { /* Hardware irq */
		LANTIQ_USIF_PRINT(port, USIF_MSG_INT,
			"%s hardware irq schedule\n", __func__);
		tasklet_hi_schedule(ptasklet);
	} else if (in_softirq()) { /* Softirq or tasklet */
		LANTIQ_USIF_PRINT(port, USIF_MSG_TASKLET,
			"%s softirq schedule\n", __func__);
		if (tasklet_trylock(ptasklet)) { /* tasklet_trylock for SMP */
			ptasklet->func(ptasklet->data);
			tasklet_unlock(ptasklet);
		} else {
			LANTIQ_USIF_PRINT(port, USIF_MSG_TASKLET,
				"%s should never happen\n", __func__);
		}
	} else { /* Process  context */
		LANTIQ_USIF_PRINT(port, USIF_MSG_THREAD,
			"%s process schedule\n", __func__);
		tasklet_schedule(ptasklet);
	}
}


#ifdef CONFIG_LANTIQ_USIF_SPI_DEBUG
/**
 * \fn static void lantiq_usif_spi_debug(struct lantiq_usif_port *port,
 *     const char *fmt, ...)
 * \brief Debug all kinds of level message
 *
 * \param   port    Pointer to structure #lantiq_usif_port
 * \param   fmt     debug output format
 *
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static void
lantiq_usif_spi_debug(struct lantiq_usif_port *port, const char *fmt, ...)
{
	static char buf[256] = { 0 }; /* XXX */
	va_list ap;
#ifdef CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS
	static const char *p = "tasklet";
#else
	static const char *p = "kthread";
#endif /* CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS */
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf), fmt, ap);
	va_end(ap);

	printk(KERN_INFO "%s %s: %s\n", p, port->name, buf);
}
#endif /* CONFIG_LANTIQ_USIF_SPI_DEBUG */

/**
 * \fn static INLINE void lantiq_usif_spi_wait_tx_finished
 *     (struct lantiq_usif_port *port)
 * \brief Wait for SPI bus becomes idle, FIFO empty doesn't means spi bus idle.
 *        to start another transaction, need to make sure bus is idle
 *
 * \param   port    Pointer to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void
lantiq_usif_spi_wait_tx_finished(struct lantiq_usif_port *port)
{
	/* Only TX_FIN can make sure that there is no activity in serial bus */
	while ((LANTIQ_USIF_GET_RAW_INT_STATUS(port)
		& LANTIQ_USIF_INT_TX_FIN) == 0)
	LANTIQ_USIF_PRINT(port, USIF_MSG_TX_FIFO,
				"%s TX FIFO Poll\n", __func__);

	/* Prepare for the next transaction */
	LANTIQ_USIF_CLR_INT(LANTIQ_USIF_INT_TX_FIN, port);
}

#if 0
#define BASIC_INPUT_CLOCK_FREQUENCY     36000000
#define CGU_PLL0_CFG 0x4
#define GET_BITS(x, msb, lsb)               \
    (((x) >> (lsb)) & ((1 << ((msb) + 1 - (lsb))) - 1))
#define CGU_PLL0_CFG_PLLN GET_BITS(ltq_cgu_r32(CGU_PLL0_CFG), 13, 6)
#define CGU_PLL0_CFG_PLLM GET_BITS(ltq_cgu_r32(CGU_PLL0_CFG), 5, 2)

static inline u32 cal_dsm(int pll, u32 num, u32 den)
{
    u64 res;

    res =  BASIC_INPUT_CLOCK_FREQUENCY * num;
    do_div(res, den);

    return res;
}

static unsigned int lantiq_get_usif_hz(void)
{
	unsigned int pll0_fosc = 0;
	unsigned int cgu_pll0_cfg, plln, pllm;

	cgu_pll0_cfg = ltq_cgu_r32(CGU_PLL0_CFG);

	printk ("cgu_pll0_cfg = %x\n", cgu_pll0_cfg);

	if (!(cgu_pll0_cfg & (1 << 0)))
		return 0;
	if (ltq_cgu_r32(CGU_PLL0_CFG) & (1 << 30))
		pll0_fosc = BASIC_INPUT_CLOCK_FREQUENCY;
	else
	{
		plln = CGU_PLL0_CFG_PLLN;
		pllm = CGU_PLL0_CFG_PLLM;
		printk ("plln = %u pllm = %u\n", plln, pllm);
		pll0_fosc = cal_dsm(0, 2*(plln + 1), pllm + 1);
	}

	printk ("pll0_fosc = %u\n", pll0_fosc);
    return (pll0_fosc / 10);
	//return (100000000);
}
#endif

#if 1
static unsigned int lantiq_get_usif_hz(void)
{
	return (100000000);
}
#endif

/**
 * \fn static INLINE unsigned int lantiq_usif_spi_get_kernel_clk
 *     (struct lantiq_usif_port *port)
 * \brief Get USIF SPI clock speed.
 * Returns the current operating speed of the USIF peripheral,
 * depending on chip
 * specific bus speed and RMC setting in CLC register.
 *
 * \param   port     Pointer to structure #lantiq_usif_port
 * \return  >0       Peripheral speed in HZ
 * \return  0        Error
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE unsigned int
lantiq_usif_spi_get_kernel_clk(struct lantiq_usif_port *port)
{
	/* This function assumes that the CLC register is set with the
	 * appropriate value for RMC.
	 */
	unsigned int rmc;

	rmc = MS(LANTIQ_USIF_GET_CLOCK_COUNTER(port), LANTIQ_USIF_CLC_CNT_RMC);
	if (rmc == 0) {
		printk(KERN_ERR "%s rmc==0\n", __func__);
		return 0;
	}
	printk ("USIF: ifx_get_usif_hz returned: %u and rmc: %u\n", (unsigned int) lantiq_get_usif_hz (), rmc);
	return (unsigned int) (lantiq_get_usif_hz() / rmc);
}

#ifdef CONFIG_PROC_FS
/**
 * \fn static int lantiq_usif_spi_stats_read(char *page, char **start,
 *     off_t offset, int count, int *eof, void *data)
 * \brief USIF SPI proc file read.
 * This function creates the proc file output for the USIF SPI statistics.
 *
 * \param   page     Buffer to write the string to
 * \param   start    not used (Linux internal)
 * \param   offset   not used (Linux internal)
 * \param   count    not used (Linux internal)
 * \param   eof      Set to 1 when all data is stored in buffer
 * \param   data     not used (Linux internal)
 * \return  len      Lenght of data in buffer
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int
lantiq_usif_spi_stats_read(char *page, char **start, off_t offset, int count,
			int *eof, void *data)
{
	int off = 0;
	lantiq_usif_port_t *port;
	lantiq_usif_device_t *dev;
	LANTIQ_USIF_SPI_STAT_t *stats;

	port = &lantiq_usif_spi_port;
	stats = &port->stats;

	off += sprintf(page + off, "Statistics for USIF SPI\n\n");
	off += sprintf(page + off, "USIF%d\n", port->port_idx);
	off += sprintf(page + off, "RX overflow errors %d\n",
		stats->rxOvErr);
	off += sprintf(page + off, "RX underflow errors %d\n",
		stats->rxUnErr);
	off += sprintf(page + off, "TX overflow errors %d\n",
		stats->txOvErr);
	off += sprintf(page + off, "TX underflow errors %d\n",
		stats->txUnErr);
	off += sprintf(page + off, "Abort errors %d\n", stats->abortErr);
	off += sprintf(page + off, "Mode errors %d\n", stats->modeErr);
	off += sprintf(page + off, "RX Bytes %llu\n", stats->rxBytes);
	off += sprintf(page + off, "TX Bytes %llu\n", stats->txBytes);
	off += sprintf(page + off, "TX FIFO transaction %llu\n",
		stats->txFifo);
	off += sprintf(page + off, "TX DMA transaction %llu\n",
		stats->txDma);
	off += sprintf(page + off, "TX DMA bytes %llu\n",
		stats->txDmaBytes);
	off += sprintf(page + off, "RX FIFO transaction %llu\n",
		stats->rxFifo);
	off += sprintf(page + off, "RX DMA transaction %llu\n",
		stats->rxDma);
	off += sprintf(page + off, "RX DMA bytes %llu\n",
		stats->rxDmaBytes);
	off += sprintf(page + off, "USIF bus status %s\n",
		port->usif_cs_locked ? "locked" : "unlocked");
	off += sprintf(page + off, "\n");

	/* Per device statistics */
	LANTIQ_USIF_SPI_SEM_LOCK(port->dev_sem);
	TAILQ_FOREACH(dev, &port->usif_devq, dev_entry) {
		off += sprintf(page + off, "Device %s duplex %s:\n",
			dev->dev_name, (dev->duplex ==
			LANTIQ_USIF_SPI_HALF_DUPLEX) ? "Half" : "Full");
		off += sprintf(page + off, "Rx Bytes %llu\n",
			dev->stats.rxBytes);
		off += sprintf(page + off, "Tx Bytes %llu\n",
			dev->stats.txBytes);
		off += sprintf(page + off, "Context errors %d\n",
			dev->stats.context_err);
		off += sprintf(page + off, "Duplicated qentry errors %d\n",
			dev->stats.dup_qentries);
		off += sprintf(page + off, "Fragment errors %d\n",
			dev->stats.frag_err);
		off += sprintf(page + off, "Handler errors %d\n",
			dev->stats.handler_err);
		off += sprintf(page + off, "Duplex errors %d\n",
			dev->stats.dlx_err);
		off += sprintf(page + off, "Enqueue %llu\n",
			       dev->stats.enqueue);
		off += sprintf(page + off, "Dequeue %llu\n",
			dev->stats.dequeue);
	}
	LANTIQ_USIF_SPI_SEM_UNLOCK(port->dev_sem);
	*eof = 1;
	return off;
}

/**
 * \fn static int lantiq_usif_spi_reg_read(char *page, char **start,
 *     off_t offset, int count, int *eof, void *data)
 * \brief USIF SPI proc file read.
 * This function creates the proc file output for the USIF SPI register dump.
 *
 * \param   page     Buffer to write the string to
 * \param   start    not used (Linux internal)
 * \param   offset   not used (Linux internal)
 * \param   count    not used (Linux internal)
 * \param   eof      Set to 1 when all data is stored in buffer
 * \param   data     not used (Linux internal)
 * \return  len      Lenght of data in buffer
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int
lantiq_usif_spi_reg_read(char *page, char **start, off_t offset, int count,
		  int *eof, void *data)
{
	int off = 0;
	lantiq_usif_port_t *port;
#define LANTIQ_USIF_SPI_REG_MAX  24
	u32 stats[LANTIQ_USIF_SPI_REG_MAX] = { 0 };

	port = &lantiq_usif_spi_port;

	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	stats[0] = LANTIQ_USIF_GET_ID(port);
	stats[1] = LANTIQ_USIF_GET_SWC_ID(port);
	stats[2] = LANTIQ_USIF_GET_FIFO_ID(port);
	stats[3] = LANTIQ_USIF_GET_CLOCK_COUNTER(port);
	stats[4] = LANTIQ_USIG_GET_CLOCK_STATUS(port);
	stats[5] = LANTIQ_USIF_GET_MODE_CONFIG(port);
	stats[6] = LANTIQ_USIF_GET_PROTOCOL_CONFIG(port);
	stats[7] = LANTIQ_USIF_GET_PROTOCOL_STATUS(port);
	stats[8] = LANTIQ_USIF_GET_CHIPSELECT_CONFIG(port);
	stats[9] = LANTIQ_USIF_GET_FRAC_DIV(port);
	stats[10] = LANTIQ_USIF_GET_BAUDRATE_COUNTER(port);
	stats[11] = LANTIQ_USIF_GET_INTERCHARACTER_TIMEOUT(port);
	stats[12] = LANTIQ_USIF_GET_FIFO_CONFIG(port);
	stats[13] = LANTIQ_USIF_GET_FIFO_STATUS(port);
	stats[14] = LANTIQ_USIF_GET_TXD_SIDEBAND_CONFIG(port);
	stats[15] = LANTIQ_USIF_GET_DPLUS_STATUS(port);
	stats[16] = LANTIQ_USIF_GET_TX_WORD(port);
	if (LANTIQ_USIG_GET_CLOCK_STATUS(port) & LANTIQ_USIF_CLC_STAT_RUN)
		stats[17] = LANTIQ_USIF_GET_RX_WORD(port);

	stats[18] = LANTIQ_USIF_GET_RX_MRPS(port);
	stats[19] = LANTIQ_USIF_GET_RAW_INT_STATUS(port);
	stats[20] = LANTIQ_USIF_GET_INT_MASK(port);
	stats[21] = LANTIQ_USIF_GET_INT_STATUS(port);

	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);

	off += sprintf(page + off, "Register dump for USIF SPI\n\n");
	off += sprintf(page + off, "USIF_ID           0x%08x\n", stats[0]);
	off += sprintf(page + off, "USIF_SWCID        0x%08x\n", stats[1]);
	off += sprintf(page + off, "USIF_FIFO_ID      0x%08x\n", stats[2]);

	off += sprintf(page + off, "USIF_CLC_CNT      0x%08x\n", stats[3]);
	off += sprintf(page + off, "USIF_CLC_STAT     0x%08x\n", stats[4]);

	off += sprintf(page + off, "USIF_MODE_CFG     0x%08x\n", stats[5]);
	off += sprintf(page + off, "USIF_PRTC_CFG     0x%08x\n", stats[6]);
	off += sprintf(page + off, "USIF_PRTC_STAT    0x%08x\n", stats[7]);

	off += sprintf(page + off, "USIF_CS_CFG       0x%08x\n", stats[8]);
	off += sprintf(page + off, "USIF_FDIV_CFG     0x%08x\n", stats[9]);
	off += sprintf(page + off, "USIF_BC_CFG       0x%08x\n",
		       stats[10]);
	off += sprintf(page + off, "USIF_ICTMO_CFG    0x%08x\n",
		       stats[11]);
	off += sprintf(page + off, "USIF_FIFO_CFG     0x%08x\n",
		       stats[12]);
	off += sprintf(page + off, "USIF_FIFO_STAT    0x%08x\n",
		       stats[13]);
	off += sprintf(page + off, "USIF_TXD_SB       0x%08x\n",
		       stats[14]);
	off += sprintf(page + off, "USIF_DPLUS_STAT   0x%08x\n",
		       stats[15]);
	off += sprintf(page + off, "USIF_TXD          0x%08x\n",
		       stats[16]);
	off += sprintf(page + off, "USIF_RXD          0x%08x\n",
		       stats[17]);
	off += sprintf(page + off, "USIF_MRPS_CTRL    0x%08x\n",
		       stats[18]);
	off += sprintf(page + off, "USIF_RIS          0x%08x\n",
		       stats[19]);
	off += sprintf(page + off, "USIF_IMSC         0x%08x\n",
		       stats[20]);
	off += sprintf(page + off, "USIF_MIS          0x%08x\n",
		       stats[21]);
	off += sprintf(page + off, "\n");

	*eof = 1;
	return off;
#undef LANTIQ_USIF_SPI_REG_MAX
}

/**
 * \fn static inline int lantiq_usif_spi_proc_version(char *page,
 *     char **start, off_t offset, int count, int *eof, void *data)
 * \brief USIF SPI proc version.
 * This function creates the proc file output for the USIF SPI driver.
 *
 * \param   page     Buffer to write the string to
 * \param   start    not used (Linux internal)
 * \param   offset   not used (Linux internal)
 * \param   count    not used (Linux internal)
 * \param   eof      Set to 1 when all data is stored in buffer
 * \param   data     not used (Linux internal)
 * \return  len      Lenght of data in buffer
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static inline int
lantiq_usif_spi_proc_version(char *page, char **start, off_t offset,
		     int count, int *eof, void *data)
{
	int len = 0;

	/* No sanity check because length is smaller than one page */
	len += lantiq_usif_spi_drv_ver(page + len);
	*eof = 1;
	return len;
}

/**
 * \fn static int lantiq_usif_spi_proc_create(void)
 * \brief Create USIF SPI proc/drivers node
 *
 * \return  -EFAULT Failed to create proc node
 * \return  0       Create proc node successfully
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int lantiq_usif_spi_proc_create(void)
{
	int ret = 0;
	struct proc_dir_entry *entry;

	lantiq_usif_spi_proc = proc_mkdir(lantiq_usif_spi_proc_node_name, NULL);
	if (lantiq_usif_spi_proc == NULL) {
		printk(KERN_ERR
			"%s:  Unable to create proc entry for %s\n",
			__func__, lantiq_usif_spi_proc_node_name);
		ret = -EFAULT;
		goto done;
	}
	entry = create_proc_read_entry("version",
		0, lantiq_usif_spi_proc, lantiq_usif_spi_proc_version, NULL);
	if (entry == NULL) {
		ret = -EFAULT;
		goto err1;
	}

	entry = create_proc_read_entry("stats",
		0, lantiq_usif_spi_proc, lantiq_usif_spi_stats_read, NULL);
	if (entry == NULL) {
		ret = -EFAULT;
		goto err2;
	}
	entry = create_proc_read_entry("reg",
		0, lantiq_usif_spi_proc, lantiq_usif_spi_reg_read, NULL);
	if (entry == NULL) {
		ret = -EFAULT;
		goto err3;
	}
	return ret;

err3:
	remove_proc_entry("stats", lantiq_usif_spi_proc);
err2:
	remove_proc_entry("version", lantiq_usif_spi_proc);
err1:
	remove_proc_entry(lantiq_usif_spi_proc_node_name, NULL);
done:
	return ret;
}

/**
 * \fn static void lantiq_usif_spi_proc_delete(void)
 * \brief Delete USIF SPI proc/drivers node
 *
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static void lantiq_usif_spi_proc_delete(void)
{
	remove_proc_entry("version", lantiq_usif_spi_proc);

	remove_proc_entry("stats", lantiq_usif_spi_proc);

	remove_proc_entry("reg", lantiq_usif_spi_proc);

	remove_proc_entry(lantiq_usif_spi_proc_node_name, NULL);
}
#endif

static inline int lantiq_drv_ver(char *buf, char *module, int major, int mid, int minor)
{
    return sprintf(buf, "Lantiq %s driver, version %d.%d.%d, (c) 2001-2013 Lantiq Deutschland GmbH\n",
	        module, major, mid, minor);
}

/**
 * \fn static INLINE int lantiq_usif_spi_drv_ver(char *buf)
 * \brief Display USPI SPI driver version after initilazation succeeds
 *
 * \return number of bytes will be printed
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE int lantiq_usif_spi_drv_ver(char *buf)
{

	return lantiq_drv_ver(buf, "USIF-SPI", LANTIQ_USIF_SPI_VER_MAJOR,
		LANTIQ_USIF_SPI_VER_MID, LANTIQ_USIF_SPI_VER_MINOR);
}

/**
 * \fn static void lantiq_usif_spi_dma_setup(struct lantiq_usif_port *port,
 *     int dir, int enabled)
 *
 * \brief Setup dma direction and enable/disable
 *
 * \param   port        Pointer to structure #lantiq_usif_port
 * \param   dir         DMA Direction, tx/rx, tx&rx
 * \param   enabled     DMA enable/disable
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static void
lantiq_usif_spi_dma_setup(struct lantiq_usif_port *port, int dir, int enabled)
{
	u32 reg = 0;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg;
	lantiq_usif_device_t *current_dev = port->current_dev;
	struct dma_device_info *dma_dev = port->dma_dev;
	static const u32 dplus_def_setting = LANTIQ_USIF_DPLUS_CTRL_TX_DIS
		| LANTIQ_USIF_DPLUS_CTRL_RX_MASK |
		LANTIQ_USIF_DPLUS_CTRL_SET_MASK;

	usif_cfg = &current_dev->conn_id;

	reg = dplus_def_setting;

	if (dir == LANTIQ_USIF_SPI_DIR_RX) { /* RX */
		if (enabled) {
			/* CSX<0~2> mapping to RX Class, this class must
			  go to D+ interface */
			reg &= ~(1 << (usif_cfg->cs_data +
				 LANTIQ_USIF_DPLUS_CTRL_RX_MASK_OFFSET));
			/* Change USIF SPI RX DMA channel class according
			   to CSX<0~2> */
			dma_device_setup_class_val(
				dma_dev->rx_chan[dma_dev->current_rx_chan],
					usif_cfg->cs_data);
		} else { /* BPI interface */
			reg |= (1 << (usif_cfg->cs_data +
				 LANTIQ_USIF_DPLUS_CTRL_RX_MASK_OFFSET));
			/* Change USIF SPI RX DMA channel class according
			   to CSX<0~2> */
			dma_device_setup_class_val(
				dma_dev->rx_chan[dma_dev->current_rx_chan], 0);
		}
	} else if (dir == LANTIQ_USIF_SPI_DIR_TX) {/* TX */
		if (enabled) {
			reg &= ~LANTIQ_USIF_DPLUS_CTRL_TX_DIS;
			reg |= LANTIQ_USIF_DPLUS_CTRL_TX_EN;
		} else {
			reg &= ~LANTIQ_USIF_DPLUS_CTRL_TX_EN;
			reg |= LANTIQ_USIF_DPLUS_CTRL_TX_DIS;
		}
		/*
		 * XXX, auto chipselect, tx class information
		 * At present, auto chipselection disabled,
		 * TX Class will always use CLASS0
		 */
	} else { /* TXRX */
		if (enabled) {
			/* CSX<0~2> mapping to RX Class, this class must
			   go to D+ interface */
			reg &= ~(1 << (usif_cfg->cs_data +
				LANTIQ_USIF_DPLUS_CTRL_RX_MASK_OFFSET));
			/* Change USIF SPI RX DMA channel class
			   according to CSX<0~2> */
			dma_device_setup_class_val(
				dma_dev->rx_chan[dma_dev->current_rx_chan],
				usif_cfg->cs_data);
			reg &= ~LANTIQ_USIF_DPLUS_CTRL_TX_DIS;
			reg |= LANTIQ_USIF_DPLUS_CTRL_TX_EN;
		} else {
			reg |= (1 << (usif_cfg->cs_data +
				 LANTIQ_USIF_DPLUS_CTRL_RX_MASK_OFFSET));
			/* Change USIF SPI RX DMA channel class according
			*  to CSX<0~2> */
			dma_device_setup_class_val(
				dma_dev->rx_chan[dma_dev->current_rx_chan], 0);
			reg &= ~LANTIQ_USIF_DPLUS_CTRL_TX_EN;
			reg |= LANTIQ_USIF_DPLUS_CTRL_TX_DIS;
		}
	}
	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	LANTIQ_USIF_CONFIG_DPLUS_CONTROL(reg, port);
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	/* Ensure D+ TX activity finished */
	if ((dir == LANTIQ_USIF_SPI_DIR_TX)
		&& (enabled == LANTIQ_USIF_SPI_DMA_DISABLE)) {
		while ((LANTIQ_USIF_GET_DPLUS_STATUS(port) &
			LANTIQ_USIF_DPLUS_STAT_TX_ACT)) {
			;
		}
	}
}

/**
 * \fn static INLINE void lantiq_usif_spi_dma_irq_finished(struct lantiq_usif_port
 *     *port)
 * \brief DMA<TX/RX> interrupt received, this function calls to reschedule
 * or wake up sleep kernel thread
 *
 * \param   port        Pointer to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void
lantiq_usif_spi_dma_irq_finished(struct lantiq_usif_port *port)
{
	/*
	 * Reset the flag that we are waiting for the DMA to complete
	 * This flag should be reset before the following stuff, otherwise
	 * start_tasklet will stop
	 */
	atomic_set(&port->dma_wait_state, 0);
	smp_wmb();
#ifdef CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS
	lantiq_usif_spi_start_tasklet(port);
#else
	LANTIQ_USIF_SPI_WAKEUP_EVENT(port->usif_thread_wait,
		LANTIQ_USIF_SPI_THREAD_EVENT, port->event_flags);
#endif /* CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS */
}

/**
 * \fn static int lantiq_usif_spi_dma_int_handler(struct dma_device_info* dma_dev,
 *     int status)
 *
 * \brief Pseudo Interrupt handler for DMA.
 * This function processes DMA interrupts notified to the USIF device driver.
 * It is installed at the DMA core as interrupt handler for the USIF SPI DMA
 * device and handles the following DMA interrupts:
 * - In case of a DMA receive interrupt the received data is passed to the
 *   upper layer.
 *
 * \param   dma_dev pointer to DMA device structure
 * \param   status  type of interrupt being notified (RCV_INT: DMA receive
 *                  interrupt, TX_BUF_FULL_INT: transmit buffer full interrupt,
 *                  TRANSMIT_CPT_INT: transmission complete interrupt)
 * \return  OK      In case of successful data reception from DMA
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int
lantiq_usif_spi_dma_int_handler(struct dma_device_info *dma_dev, int status)
{
	int i;
	struct lantiq_usif_port *port;
	u32 dma_burst_len;
	u32 dma_burst_len_mask;

	port = (struct lantiq_usif_port *) dma_dev->priv;
	LANTIQ_USIF_PRINT(port, USIF_MSG_INT, "Interrupt status %d\n", status);
	switch (status) {
	case RCV_INT:
		LANTIQ_USIF_PRINT(port, USIF_MSG_INT, "RCV_INT\n");
		dma_burst_len = (dma_dev->rx_burst_len << 2);
		dma_burst_len_mask = dma_burst_len - 1;
		lantiq_usif_spi_dma_setup(port, LANTIQ_USIF_SPI_DIR_TXRX,
				LANTIQ_USIF_SPI_DMA_DISABLE);

		/* Reset USIF-SPI DMA RX channel */
		(dma_dev->rx_chan[dma_dev->current_rx_chan])->reset(
			dma_dev->rx_chan[dma_dev->current_rx_chan]);

		/* HW WAR, if packet length is indivisible by burst length,
		   memory copy must be used */
		if ((port->rx_len & dma_burst_len_mask)) {
			memcpy((char *) port->rxbuf_ptr, port->dma_rxbuf,
				port->rx_len);
			/* Reset these for FIFO usage */
			port->rxbuf_ptr = NULL;
			port->rx_len = 0;
		}

		lantiq_usif_spi_dma_irq_finished(port);
		break;

	case TX_BUF_FULL_INT:
		for (i = 0; i < dma_dev->max_tx_chan_num; i++) {
			if ((dma_dev->tx_chan[i])->control ==
				IFX_DMA_CH_ON) {
				dma_dev->tx_chan[i]->enable_irq(
					dma_dev->tx_chan[i]);
			}
		}
		LANTIQ_USIF_PRINT(port, USIF_MSG_INT, "TX_BUF_FULL_INT\n");
		break;

	case TRANSMIT_CPT_INT:
		for (i = 0; i < dma_dev->max_tx_chan_num; i++)
			dma_dev->tx_chan[i]->disable_irq(dma_dev->tx_chan[i]);

		LANTIQ_USIF_PRINT(port, USIF_MSG_INT, "TRANSMIT_CPT_INT\n");

		/*
		 * 1. NB, Make sure data has been sent out. TX_FIN interrupt
		 * may take longer time to receive. Polling is better here
		 * 2. If FDX is used, only RCV_INT will schedule the proccess
		 * /tasklet.
		 * HDX device may use FDX mode for RX operation. However,
		 * for TX, only HDX can be used for HDX device.
		 */
		lantiq_usif_spi_wait_tx_finished(port); /* Still needed? */
		if (port->dma_is_in_half_duplex == 1)
			lantiq_usif_spi_dma_irq_finished(port);

		break;

	default:
		printk(KERN_ERR "%s unknow interrupt %d\n", __func__,
			status);
		break;
	}
	return 0;
}

/**
 * \fn static u8* lantiq_usif_spi_dma_buffer_alloc(int len, int* byte_offset,
 *     void** opt)
 * \brief Allocates buffer for USIF SPI DMA.
 * This function is installed as DMA callback function to be called when
 * the DMA needs to allocate a new buffer.
 *
 * \param   len          Length of packet
 * \param   *byte_offset Pointer to byte offset
 * \param   **opt        unused
 * \return  NULL         In case of buffer allocation fails
 * \return  buffer       Pointer to allocated memory
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static u8 *lantiq_usif_spi_dma_buffer_alloc(int len, int *byte_offset,
	void **opt)
{
	return NULL;
}

/**
 * \fn static int lantiq_usif_spi_dma_buffer_free(u8 *dataptr, void *opt)
 * \brief Free DMA buffer.
 * This function frees a buffer previously allocated by the DMA.
 *
 * \param   *dataptr Pointer to data buffer
 * \param   *opt     unused
 * \return  0        OK
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int lantiq_usif_spi_dma_buffer_free(u8 *dataptr, void *opt)
{
	/* SPI will be in charge of memory free if necessary */
	return 0;
}

/**
 * \fn static int lantiq_usif_spi_init_dma_device(struct dma_device_info *dma_dev)
 * \brief Initialize USIF SPI DMA device.
 * This function initializes the passed DMA device structure for usage as
 * USIF SPI DMA device.
 *
 * \param   *dma_dev Pointer to dma device structure to be initialized
 * \return  0        OK
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int lantiq_usif_spi_init_dma_device(struct dma_device_info *dma_dev)
{
	int i;

	dma_dev->priv = &lantiq_usif_spi_port;
	dma_dev->num_tx_chan = DEFAULT_USIF_SPI_TX_CHANNEL_NUM;
	dma_dev->num_rx_chan = DEFAULT_USIF_SPI_RX_CHANNEL_NUM;
	dma_dev->tx_burst_len = DEFAULT_USIF_SPI_TX_BURST_LEN;
	dma_dev->rx_burst_len = DEFAULT_USIF_SPI_RX_BURST_LEN;
	dma_dev->port_tx_weight = 1;
	/* Combined with FIFO TX/RX SWAP */
	dma_dev->tx_endianness_mode = IFX_DMA_ENDIAN_TYPE3;
	dma_dev->rx_endianness_mode = IFX_DMA_ENDIAN_TYPE3;
	dma_dev->port_packet_drop_enable = 0;
	for (i = 0; i < dma_dev->num_tx_chan; i++) {
		dma_dev->tx_chan[i]->desc_len =
			DEFAULT_USIF_SPI_TX_CHANNEL_DESC_NUM;
		dma_dev->tx_chan[i]->control = IFX_DMA_CH_ON;
		dma_dev->tx_chan[i]->packet_size =
			DEFAULT_USIF_SPI_FRAGMENT_SIZE;
	}
	for (i = 0; i < dma_dev->num_rx_chan; i++) {
		dma_dev->rx_chan[i]->desc_len =
			DEFAULT_USIF_SPI_RX_CHANNEL_DESC_NUM;
		dma_dev->rx_chan[i]->packet_size =
			DEFAULT_USIF_SPI_FRAGMENT_SIZE;
		dma_dev->rx_chan[i]->control = IFX_DMA_CH_ON;
		dma_dev->rx_chan[i]->byte_offset = 0;
	}
	dma_dev->current_tx_chan = 0;
	dma_dev->current_rx_chan = 0;
	/*
	 * set DMA handler functions for rx-interrupts,
	 * buffer allocation and release
	 */
	dma_dev->intr_handler = lantiq_usif_spi_dma_int_handler;
	dma_dev->buffer_alloc = lantiq_usif_spi_dma_buffer_alloc;
	dma_dev->buffer_free = lantiq_usif_spi_dma_buffer_free;
	dma_device_register(dma_dev);
	return 0;
}

/**
 * \fn static INLINE void lantiq_usif_spi_gpio_init(void)
 * \brief Reserve and initialize GPIO for USIF-SPI.
 *
 * Initialize MUX settings to enable SPI interface
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void lantiq_usif_spi_gpio_init(void)
{
	//lantiq_gpio_register(LANTIQ_GPIO_MODULE_USIF_SPI);
}

/**
 * \fn static INLINE void lantiq_usif_spi_gpio_release(void)
 * \brief GPIO release.
 *
 * Release reserverd gpio resource so that other module could use it
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void lantiq_usif_spi_gpio_release(void)
{
	//lantiq_gpio_deregister(LANTIQ_GPIO_MODULE_USIF_SPI);
}

/**
 * \fn static INLINE void lantiq_usif_spi_clc_cfg_mode(struct lantiq_usif_port *port)
 * \brief enter configuration mode.
 *
 * \param   port    Pointer to structure #lantiq_usif_port
 *
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void lantiq_usif_spi_clc_cfg_mode(struct lantiq_usif_port *port)
{
	int i;
#define LANTIQ_USIF_SPI_CLC_CFG_TIMEOUT  10
	/*
	 * WAR, read back to make sure the previous write/read to take effect
	 * before we enter config mode.
	 * Please note, must be a config register, instead of status register
	 * Unfortunately, it will not work on emulation platform
	 */
#ifndef CONFIG_USE_EMULATOR
	LANTIQ_USIF_GET_MODE_CONFIG(port);
#endif /* CONFIG_USE_EMULATOR */
	LANTIQ_USIF_CLC_CONFIG_MODE(port);
	for (i = 0; i < LANTIQ_USIF_SPI_CLC_CFG_TIMEOUT; i++) {
		if ((LANTIQ_USIG_GET_CLOCK_STATUS(port) &
			LANTIQ_USIF_CLC_STAT_CFG_UPDATE_DONE)
			== LANTIQ_USIF_CLC_STAT_CFG_UPDATE_DONE) {
			break;
		}
	}
	if (i >= LANTIQ_USIF_SPI_CLC_CFG_TIMEOUT)
		printk(KERN_ERR "%s timeout\n", __func__);

#undef LANTIQ_USIF_SPI_CLC_CFG_TIMEOUT
}

/**
 * \fn static INLINE void lantiq_usif_spi_clc_run_mode(struct lantiq_usif_port *port)
 * \brief enter running mode.
 *
 * \param   port    Pointer to structure #lantiq_usif_port
 *
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void lantiq_usif_spi_clc_run_mode(struct lantiq_usif_port *port)
{
	int i;
#define LANTIQ_USIF_SPI_CLC_RUN_TIMEOUT  10

	/*
	 * WAR, dummy read back to make sure the previous write/read to take
	 * effect  before we enter run mode.
	 * Please note, must be a config register, instead of status register.
	 * Unfortunately, it will not work on emulation platform
	 */
#ifndef CONFIG_USE_EMULATOR
	LANTIQ_USIF_GET_MODE_CONFIG(port);
#endif /* CONFIG_USE_EMULATOR */
	LANTIQ_USIF_CLC_RUN_MODE(port);
	for (i = 0; i < LANTIQ_USIF_SPI_CLC_RUN_TIMEOUT; i++) {
		if ((LANTIQ_USIG_GET_CLOCK_STATUS(port) &
			LANTIQ_USIF_CLC_STAT_RUN_UPDATE_DONE)
			== LANTIQ_USIF_CLC_STAT_RUN_UPDATE_DONE) {
			break;
		}
	}
	if (i >= LANTIQ_USIF_SPI_CLC_RUN_TIMEOUT)
		printk(KERN_ERR "%s timeout\n", __func__);

	/* WAR, for lower baudrate, it still takes some time to
	*  make configuration to take effect */
	if (port->baudrate < 1000000)
		udelay(1000000 / port->baudrate);

#undef LANTIQ_USIF_SPI_CLC_RUN_TIMEOUT
}

/**
 * \fn static INLINE int lantiq_usif_spi_rxtx_mode_set(struct lantiq_usif_port *port,
 *     unsigned int val)
 * \brief Rx/Tx mode set.
 * Set the transmission mode while USIF SPI is idle
 *
 * \param   port    Pointer to structure #lantiq_usif_port
 * \param   val     Rx/Tx mode
 * \return  0       OK
 * \return  -EINVAL Invalid parameters supplied
 * \return  -EBUSY  Transmission or reception ongoing
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE int
lantiq_usif_spi_rxtx_mode_set(struct lantiq_usif_port *port, unsigned int val)
{
	u32 bits = 0;
	u32 reg;
	int enabled = 0;
	int force_clk;

	LANTIQ_USIF_PRINT(port, USIF_MSG_DUPLEX,
				"%s rxtx mode set..\n", __func__);
	if (unlikely(port == NULL)
		|| unlikely((val & (LANTIQ_USIF_SPI_MODE_RXTX_MASK)) ==
			LANTIQ_USIF_SPI_MODE_OFF)) {
		return -EINVAL;
	}

	/* Check if there is activity in USIF bus */
	if (((LANTIQ_USIF_GET_FIFO_STATUS(port) &
		LANTIQ_USIF_FIFO_STAT_TXRX_FFS_MASK) != 0x00)) {
		printk(KERN_ERR "%s busy fifo status 0x%08x\n", __func__,
			LANTIQ_USIF_GET_FIFO_STATUS(port));
		return -EBUSY;
	}

	switch (val) {
	case LANTIQ_USIF_SPI_MODE_RX:
		bits = LANTIQ_USIF_MODE_CFG_RX_EN;
		force_clk = 1; /* Force shift clock for RX only */
		break;

	case LANTIQ_USIF_SPI_MODE_TX:
		bits = LANTIQ_USIF_MODE_CFG_TX_EN;
		force_clk = 0;
		break;

	case LANTIQ_USIF_SPI_MODE_RXTX: /* Fall through */
	default:
		bits = LANTIQ_USIF_MODE_CFG_TXRX_EN;
		force_clk = 0;
		break;
	}

	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	enabled = LANTIQ_USIG_GET_CLOCK_STATUS(port) & LANTIQ_USIF_CLC_STAT_RUN;
	if (enabled)
		lantiq_usif_spi_clc_cfg_mode(port);

	reg = LANTIQ_USIF_GET_MODE_CONFIG(port);
	reg &= ~LANTIQ_USIF_MODE_CFG_TXRX_MASK;

	reg |= bits;

	if (force_clk == 1)
		reg |= LANTIQ_USIF_MODE_CFG_SC_FRC;
	else
		reg &= ~LANTIQ_USIF_MODE_CFG_SC_FRC;
	LANTIQ_USIF_SET_MODE_CONFIG(reg, port);
	if (enabled)
		lantiq_usif_spi_clc_run_mode(port);

	port->opts.modeRxTx = val;
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);

	LANTIQ_USIF_PRINT(port, USIF_MSG_DUPLEX,
				"%s rxtx mode set finished..\n", __func__);
	return 0;
}

/**
 * \fn static int lantiq_usif_spi_sethwopts(struct lantiq_usif_port *port)
 * \brief USIF SPI set hardware options.
 * This routine intializes the USIF SPI appropriately depending on
 * slave/master and full-/half-duplex mode. It assumes that the USIF SPI
 * is disabled and the fifo's and buffers are flushed later on.
 *
 * \param   port    Pointer to structure #lantiq_usif_port
 * \return  0       OK
 * \return  -EINVAL Invalid hardware options supplied
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int lantiq_usif_spi_sethwopts(struct lantiq_usif_port *port)
{
	u32 reg;
	u32 mode = 0;
	u32 prtc = 0;
	struct lantiq_usif_hwopts *opts = &port->opts;

	if (opts->sync)
		mode |= LANTIQ_USIF_MODE_CFG_SYNC;

	if (opts->master)
		mode |= LANTIQ_USIF_MODE_CFG_MA;

	if (opts->frmctl)
		mode |= LANTIQ_USIF_MODE_CFG_FC_EN;

	if (opts->loopBack)
		mode |= LANTIQ_USIF_MODE_CFG_LB;

	if (opts->echoMode)
		mode |= LANTIQ_USIF_MODE_CFG_ECH;

	if (opts->txInvert)
		mode |= LANTIQ_USIF_MODE_CFG_TX_INV;

	if (opts->rxInvert)
		mode |= LANTIQ_USIF_MODE_CFG_RX_INV;

	if (opts->shiftClk)
		mode |= LANTIQ_USIF_MODE_CFG_SC_FRC;

	if (opts->clkPO)
		mode |= LANTIQ_USIF_MODE_CFG_SC_POL;

	if (opts->clkPH)
		mode |= LANTIQ_USIF_MODE_CFG_SC_PH;

	if (opts->txIdleValue)
		mode |= LANTIQ_USIF_MODE_CFG_TX_IDLE;

	switch (opts->modeRxTx) {
	case LANTIQ_USIF_SPI_MODE_TX:
		mode |= LANTIQ_USIF_MODE_CFG_TX_EN;
		break;
	case LANTIQ_USIF_SPI_MODE_RX:
		mode |= LANTIQ_USIF_MODE_CFG_RX_EN;
		break;
	case LANTIQ_USIF_SPI_MODE_RXTX:
		mode |= LANTIQ_USIF_MODE_CFG_TXRX_EN;
		break;
	default:
		mode |= LANTIQ_USIF_MODE_CFG_TXRX_EN;
		break;
	}

	switch (opts->charLen) {
	case LANTIQ_USIF_PRTC_CFG_CLEN8:
		prtc |= LANTIQ_USIF_PRTC_CFG_CLEN8;
		break;
	case LANTIQ_USIF_PRTC_CFG_CLEN16:
		prtc |= LANTIQ_USIF_PRTC_CFG_CLEN16;
		break;
	case LANTIQ_USIF_PRTC_CFG_CLEN32:
		prtc |= LANTIQ_USIF_PRTC_CFG_CLEN32;
		break;
	default:
		prtc |= LANTIQ_USIF_PRTC_CFG_CLEN32;
		break;
	}

	if (opts->hdrCtrl)
		prtc |= LANTIQ_USIF_PRTC_CFG_HD_MSB;

	if (opts->wakeUp)
		prtc |= LANTIQ_USIF_PRTC_CFG_WK_UP;

	LANTIQ_USIF_SPI_IRQ_LOCK(port);

	/* Mode configration register, master, sync, mode 0, tx/rx enable  */
	LANTIQ_USIF_SET_MODE_CONFIG(mode, port);

	/* Protocol configuration register, 4 bytes per character, MSB first */
	LANTIQ_USIF_SET_PROTOCOL_CONFIG(prtc, port);

	/* Default CS, no auto chip select. Initial value as high */
	reg = LANTIQ_USIF_CS_CFG_EN;
	reg |= LANTIQ_USIF_CS_CFG_CSX_DEFAULT;
	LANTIQ_USIF_SET_CHIPSELECT_CONFIG(reg, port);

	/* Class 0 only, disable D+ TX */
	LANTIQ_USIF_CONFIG_DPLUS_CONTROL(LANTIQ_USIF_DPLUS_CTRL_TX_DIS |
		LANTIQ_USIF_DPLUS_CTRL_RX_MASK |
		LANTIQ_USIF_DPLUS_CTRL_SET_MASK, port);

	/* TX Class information */
	LANTIQ_USIF_CONFIG_TXD_SIDEBAND(SM(LANTIQ_USIF_TXD_CLASS0,
		LANTIQ_USIF_TXD_SB_TX_CLS), port);

	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	return 0;
}

/**
 * \fn int lantiq_usif_spi_cs_low(u32 cs)
 * \brief Chip select enable.
 * This function sets the given chip select for USIF SPI to low.
 *
 * \param   cs       Selected CS pin
 * \return  0        OK
 * \return  -EINVAL  Invalid GPIO pin provided
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int lantiq_usif_spi_cs_low(u32 cs)
{
	int ret;
	u32 reg;
	int enabled;
	struct lantiq_usif_port *port = &lantiq_usif_spi_port;

	if (cs > LANTIQ_USIF_SPI_CS_MAX)
		ret = -EINVAL;
	else {
		/* Check if there is activity in USIF bus */
		if (((LANTIQ_USIF_GET_FIFO_STATUS(port) &
			LANTIQ_USIF_FIFO_STAT_TXRX_FFS_MASK) != 0x00)) {
			printk(KERN_ERR "%s is busy\n", __func__);
			return -EBUSY;
		}
		LANTIQ_USIF_SPI_IRQ_LOCK(port);
		enabled =
			LANTIQ_USIG_GET_CLOCK_STATUS(port) &
			LANTIQ_USIF_CLC_STAT_RUN;
		if (enabled)
			lantiq_usif_spi_clc_cfg_mode(port);

		reg = LANTIQ_USIF_GET_CHIPSELECT_CONFIG(port);
		/* Chipboudary CSO = CSO XOR CSOINV, CSOINV is
		   initialized to 0xff */
		reg |= (1 << (cs + LANTIQ_USIF_CS_OFFSET));
		LANTIQ_USIF_SET_CHIPSELECT_CONFIG(reg, port);

		if (enabled)
			lantiq_usif_spi_clc_run_mode(port);
		LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
		ret = 0;
	}
	return ret;
}
EXPORT_SYMBOL(lantiq_usif_spi_cs_low);

/**
 * \fn int lantiq_usif_spi_cs_high(u32 cs)
 * \brief Chip select disable.
 * This function sets the given chip select for USIF SPI to high.
 *
 * \param   cs       Selected CS pin
 * \return  0        OK
 * \return  -EINVAL  Invalid GPIO pin provided
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int lantiq_usif_spi_cs_high(u32 cs)
{
	int ret;
	int enabled;
	u32 reg;
	struct lantiq_usif_port *port = &lantiq_usif_spi_port;

	if (cs > LANTIQ_USIF_SPI_CS_MAX)
		ret = -EINVAL;
	else {
		/* Check if there is activity in USIF bus */
		if (((LANTIQ_USIF_GET_FIFO_STATUS(port) &
			LANTIQ_USIF_FIFO_STAT_TXRX_FFS_MASK) != 0x00)) {
			printk(KERN_ERR "%s is busy\n", __func__);
			return -EBUSY;
		}
		LANTIQ_USIF_SPI_IRQ_LOCK(port);
		enabled =
			LANTIQ_USIG_GET_CLOCK_STATUS(port) &
			LANTIQ_USIF_CLC_STAT_RUN;
		if (enabled)
			lantiq_usif_spi_clc_cfg_mode(port);

		reg = LANTIQ_USIF_GET_CHIPSELECT_CONFIG(port);
		/* Chipboudary CSO = CSO XOR CSOINV, CSOINV
		*  is initialized to 0xff */
		reg &= ~(1 << (cs + LANTIQ_USIF_CS_OFFSET));
		LANTIQ_USIF_SET_CHIPSELECT_CONFIG(reg, port);

		if (enabled)
			lantiq_usif_spi_clc_run_mode(port);

		LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
		ret = 0;
	}
	return ret;
}
EXPORT_SYMBOL(lantiq_usif_spi_cs_high);

/**
 * \fn static void lantiq_usif_spi_start_txfifo(struct lantiq_usif_port *port,
 *     char *txbuf, u32 txsize)
 * \brief Start FIFO data transmisssion .
 * This function copies remaining data in the transmit buffer into the FIFO
 *
 * \param   port     Pointer to structure #lantiq_usif_port
 * \param   txbuf    Pointer to the data packet to transmit
 * \param   txsize   Amount of Bytes to transmit
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static void
lantiq_usif_spi_start_txfifo(struct lantiq_usif_port *port, char *txbuf,
		    u32 txsize)
{
	int i;
	char *p;
	u32 eff_bytes, eff_word;
	u32 tx_cnt;
	u32 *ptxbuf;
	LANTIQ_USIF_TXD_t txd_data;
	int total = txsize;

	while (1) {

		/* Wait for TX FIFO empty */
		while (LANTIQ_USIF_GET_TX_FILL_FIFO(port) != 0x00)
			;

		/* Optimized the frequest two cases */
		/* Last one incomlete or complete word */
		if (total <= LANTIQ_USIF_SPI_TX_FIFO_CHARS_PER_WORD) {
			eff_bytes = total;
			for (i = 0, p = txbuf; i < eff_bytes; i++, p++)
				txd_data.txd_byte.byte[i] = *(u8 *) p;

			LANTIQ_USIF_PUT_TX_LAST_WORD(((eff_bytes ==
				LANTIQ_USIF_SPI_TX_FIFO_CHARS_PER_WORD) ?
				LANTIQ_USIF_TXD_BE4 : eff_bytes),
				txd_data.txd_word, port);
		/* More than fifo bytes */
		} else if (total > port->tx_fifo_size_bytes) {
			eff_bytes = port->tx_fifo_size_bytes;
			eff_word = port->tx_fifo_size_words;
			for (i = 0, ptxbuf = (u32 *) txbuf; i < eff_word;
			     i++, ptxbuf++) {
				LANTIQ_USIF_PUT_TX_WORD(*ptxbuf, port);
			}
		} else { /* 5 ~ 32 bytes */
			eff_bytes = total;
			eff_word = (eff_bytes >> 2)
				+ ((eff_bytes & 0x3) > 0 ? 1 : 0);

			/* Transmit the complete word */
			for (i = 0, ptxbuf = (u32 *) txbuf;
			     i < eff_word - 1; i++, ptxbuf++) {
				LANTIQ_USIF_PUT_TX_WORD(*ptxbuf, port);
			}
			tx_cnt = (eff_bytes & 0x3);
			/* Still one complete word */
			if (tx_cnt == 0) {
				/* 4 bytes for one complete word */
				tx_cnt = LANTIQ_USIF_SPI_TX_FIFO_CHARS_PER_WORD;
			}

			/* Transmit the last complete or incomplete word */
			for (i = 0, p = (char *) ptxbuf; i < tx_cnt;
			     i++, p++) {
				txd_data.txd_byte.byte[i] = *(u8 *) p;
			}
			LANTIQ_USIF_PUT_TX_LAST_WORD((tx_cnt ==
				LANTIQ_USIF_SPI_TX_FIFO_CHARS_PER_WORD ?
				LANTIQ_USIF_TXD_BE4 : tx_cnt),
				txd_data.txd_word, port);
		}

		txbuf += eff_bytes;
		total -= eff_bytes;
		if (total == 0)
			break;

	}
}

/**
 * \fn static void lantiq_usif_spi_start_rxfifo(struct lantiq_usif_port *port,
 *     char *rxbuf, u32 rxsize)
 * \brief Start FIFO data reception.
 * This function processes received data. It will read data from the FIFO and
 * copy it to the receive buffer.
 *
 * \param   port     Pointer to structure #lantiq_usif_port
 * \param   rxbuf    Pointer to store the received data packet
 * \param   rxsize   Amount of Bytes to receive.
 * XXX, RME or calculate the length from the total length
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static void
lantiq_usif_spi_start_rxfifo(struct lantiq_usif_port *port, char *rxbuf,
		    u32 rxsize)
{
	int i;
	char *p;
	u32 eff_bytes, eff_word;
	int rx_cnt;
	u32 *prxbuf;
	LANTIQ_USIF_RXD_t rxd_data;
	int total = rxsize;

	while (1) {
		/* In this case, no need to check last complete or
		   incomplete word */
		if (total >= port->rx_fifo_size_bytes) {
			eff_bytes = port->rx_fifo_size_bytes;
			eff_word = port->rx_fifo_size_words;

			LANTIQ_USIF_CONFIG_RX_MRPS(SM(eff_bytes,
				LANTIQ_USIF_MRPS_CTRL_MRPS), port);
			/* Wait for data whose length is equal to FIFO size */
			while (LANTIQ_USIF_GET_RX_FILL_FIFO(port) != eff_word)
				LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
					"%s trying to receive eff_words..\n",
					__func__);

			LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
				"%s received eff_words..\n",
				__func__);
			for (i = 0, prxbuf = (u32 *) rxbuf; i < eff_word;
			     i++, prxbuf++) {
				*prxbuf = LANTIQ_USIF_GET_RX_WORD(port);
			}
		} else {
			eff_bytes = total;
			eff_word = (eff_bytes >> 2)
				+ ((eff_bytes & 0x3) > 0 ? 1 : 0);

			LANTIQ_USIF_CONFIG_RX_MRPS(SM(eff_bytes,
				LANTIQ_USIF_MRPS_CTRL_MRPS), port);
			while (LANTIQ_USIF_GET_RX_FILL_FIFO(port) != eff_word)
				;

			/* Receive complete word */
			for (i = 0, prxbuf = (u32 *) rxbuf;
			     i < eff_word - 1; i++, prxbuf++) {
				*prxbuf = LANTIQ_USIF_GET_RX_WORD(port);
			}

			/*
			 * SPI Master know how many bytes to be received
			 * XXX, a better way to use RME and DPLUS status
			 * register, but it involves interrupt handling.
			 */
			rx_cnt = total - ((eff_word - 1) << 2);
			/* Last complete word or in complete word */
			rxd_data.rxd_word = LANTIQ_USIF_GET_RX_WORD(port);
			for (i = 0, p = (char *) prxbuf; i < rx_cnt;
			     i++, p++) {
				*(u8 *) p = rxd_data.rxd_byte.byte[i];
			}
		}
		rxbuf += eff_bytes;
		total -= eff_bytes;
		if (total == 0)
			break;
	}
}

/**
 * \fn static int lantiq_usif_spi_txrx_fifo_fdx(struct lantiq_usif_port *port,
 *     char *txbuf, u32 txsize, char *rxbuf, u32 rxsize)
 * \brief Called to transmit/receive to/from USIF SPI in one step using
 *  FIFO mode.
 *
 * \param   port      Pointer to structure #lantiq_usif_port
 * \param   txbuf     Pointer to the data packet to transmit
 * \param   txsize    Amount of Bytes to transmit
 * \param   rxbuf     Pointer to store the received data packet
 * \param   rxsize    Amount of Bytes to receive.
 * \return  >= 0      Number of bytes received (if rxbuf != 0) or
 *                    transmitted
 * \return  < 0       error number
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int
lantiq_usif_spi_txrx_fifo_fdx(struct lantiq_usif_port *port, char *txbuf,
		     u32 txsize, char *rxbuf, u32 rxsize)
{
	int i;
	char *p;
	int tx_fifo_space;
	int rx_fill_space;
	u32 txtotal = txsize;
	u32 rxtotal = rxsize;
	u32 eff_bytes, eff_word;
	int tx_cnt, rx_cnt;
	u32 *ptxbuf;
	u32 *prxbuf;
	LANTIQ_USIF_TXD_t txd_data;
	LANTIQ_USIF_RXD_t rxd_data;

	lantiq_usif_spi_dma_setup(port, LANTIQ_USIF_SPI_DIR_TXRX,
		LANTIQ_USIF_SPI_DMA_DISABLE);

	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	while (1) {
		tx_fifo_space = port->tx_fifo_size_words -
			LANTIQ_USIF_GET_TX_FILL_FIFO(port);
		/* No space, wait */
		if (tx_fifo_space == 0)
			continue;

		/* Still have more data to transmit, whole words */
		if (txtotal > (tx_fifo_space << 2)) {
			eff_bytes = (tx_fifo_space << 2);
			eff_word = tx_fifo_space;

			LANTIQ_USIF_CONFIG_RX_MRPS(SM(eff_bytes,
				LANTIQ_USIF_MRPS_CTRL_MRPS), port);

			for (i = 0, ptxbuf = (u32 *) txbuf; i < eff_word;
			     i++, ptxbuf++) {
				LANTIQ_USIF_PUT_TX_WORD(*ptxbuf, port);
			}
		} else {
			eff_bytes = txtotal;
			eff_word = (eff_bytes >> 2)
				+ ((eff_bytes & 0x3) > 0 ? 1 : 0);

			LANTIQ_USIF_CONFIG_RX_MRPS(SM(eff_bytes,
				LANTIQ_USIF_MRPS_CTRL_MRPS), port);

			/* Transmit the complete word */
			for (i = 0, ptxbuf = (u32 *) txbuf;
			     i < eff_word - 1; i++, ptxbuf++) {
				LANTIQ_USIF_PUT_TX_WORD(*ptxbuf, port);
			}

			tx_cnt = (eff_bytes & 0x3);

			/* Still one complete word */
			if (tx_cnt == 0)
				tx_cnt = LANTIQ_USIF_SPI_TX_FIFO_CHARS_PER_WORD;

			/* Transmit the last complete or incomplete word */
			for (i = 0, p = (char *) ptxbuf; i < tx_cnt;
			     i++, p++) {
				txd_data.txd_byte.byte[i] = *(u8 *) p;
			}
			LANTIQ_USIF_PUT_TX_LAST_WORD((tx_cnt ==
				LANTIQ_USIF_SPI_TX_FIFO_CHARS_PER_WORD
				? LANTIQ_USIF_TXD_BE4 : tx_cnt),
				txd_data.txd_word, port);
		}
		txbuf += eff_bytes;
		txtotal -= eff_bytes;

		/* NB, wait for data to come in, not efficient,
		   but no RX overrun issue */
		while (LANTIQ_USIF_GET_RX_FILL_FIFO(port) != eff_word)
			;

		rx_fill_space = LANTIQ_USIF_GET_RX_FILL_FIFO(port);

		if (rxtotal > (rx_fill_space << 2)) {
			eff_bytes = (rx_fill_space << 2);
			eff_word = rx_fill_space;
			for (i = 0, prxbuf = (u32 *) rxbuf; i < eff_word;
			     i++, prxbuf++) {
				*prxbuf = LANTIQ_USIF_GET_RX_WORD(port);
			}
		} else { /* At least one word */
			eff_bytes = rxtotal;
			eff_word = (eff_bytes >> 2)
				+ ((eff_bytes & 0x3) > 0 ? 1 : 0);
			/* Receive complete word */
			for (i = 0, prxbuf = (u32 *) rxbuf;
			     i < eff_word - 1; i++, prxbuf++) {
				*prxbuf = LANTIQ_USIF_GET_RX_WORD(port);
			}
			/* SPI Master know how many bytes to be received */
			rx_cnt = rxtotal - ((eff_word - 1) << 2);
			rxd_data.rxd_word = LANTIQ_USIF_GET_RX_WORD(port);
			for (i = 0, p = (char *) prxbuf; i < rx_cnt;
			     i++, p++) {
				*(u8 *) p = rxd_data.rxd_byte.byte[i];
			}
		}
		rxbuf += eff_bytes;
		rxtotal -= eff_bytes;

		/* Both RX/TX finished, break */
		if ((txtotal == 0) && (rxtotal == 0))
			break;
	}
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	return txsize;
}

/**
 * \fn static int lantiq_usif_spi_set_baudrate(struct lantiq_usif_port *port,
 *     unsigned int baudrate)
 * \brief USIF SPI set baudrate.
 * Sets the baudrate of the corresponding port according to the passed
 * rate after reading out the current module speed.
 *
 * \param   port      Pointer to structure #lantiq_usif_port
 * \param   baudrate  Desired baudrate
 * \return  0         OK
 * \return  -EINVAL   Could not retrieve system clock or invalid
 *                    baudrate setting
 * \return  -EBUSY    Serial bus still busy
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int
lantiq_usif_spi_set_baudrate(struct lantiq_usif_port *port,
		     unsigned int baudrate)
{
	int enabled;
	unsigned int spi_clock;
	unsigned int bc = 0;

	if (port->prev_baudrate == baudrate)
		return 0;

	if ((baudrate > LANTIQ_USIF_SPI_HALF_BAUD)
		&& (baudrate < LANTIQ_USIF_SPI_MAX_BAUD)) {
		printk(KERN_ERR "%s: clock < %d ~ %d >not supported\n",
			__func__, LANTIQ_USIF_SPI_HALF_BAUD,
			LANTIQ_USIF_SPI_MAX_BAUD);
		return -EINVAL;
	}

	spi_clock = lantiq_usif_spi_get_kernel_clk(port);
	printk ("USIF: spi_clock : %u\n", spi_clock);
	if (spi_clock == 0)
		return -EINVAL;

	if ((baudrate != LANTIQ_USIF_SPI_MAX_BAUD)
		&& (spi_clock >> 2) < baudrate) {
		printk(KERN_ERR "%s: illegal baudrate %u\n", __func__,
			baudrate);
		return -EINVAL;
	}

	if (baudrate != LANTIQ_USIF_SPI_MAX_BAUD) {
		/*
		 * Compute the baud counter
		 * bc = fkernel/(4 * fbaud) -1 ; not so accurate
		 * Only half kernel clock is different.
		 */
		bc = (((spi_clock >> 2) + baudrate / 2) / baudrate) - 1;
	}
	if (bc > 0x1fff || ((bc == 0) &&
		((LANTIQ_USIF_GET_MODE_CONFIG(port) &
		LANTIQ_USIF_MODE_CFG_MA) == 0))) {
		printk(KERN_ERR "%s: illegal baudrate %u br %d\n",
			__func__, baudrate, bc);
		return -EINVAL;
	}

	/* Check if there is activity in USIF bus */
	if ((LANTIQ_USIF_GET_FIFO_STATUS(port) &
		LANTIQ_USIF_FIFO_STAT_TXRX_FFS_MASK) != 0x00) {
		printk(KERN_ERR "%s is busy\n", __func__);
		return -EBUSY;
	}
	port->baudrate = baudrate;
	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	/* Has to be in cfg mode */
	enabled = LANTIQ_USIG_GET_CLOCK_STATUS(port) & LANTIQ_USIF_CLC_STAT_RUN;
	if (enabled)
		lantiq_usif_spi_clc_cfg_mode(port);


	/* NB, SPI mode should not use fraction divider */
	LANTIQ_USIF_FRAC_DIV_CONFIG(0, port);

	/* Half kernel clock, SCDIV = 1, BCRV = 0, fraction divider
	   must not be used */
	if (baudrate == LANTIQ_USIF_SPI_MAX_BAUD) {
		bc = 0;
		bc |= LANTIQ_USIF_BC_CFG_SCDIV_HALF;
	}
	LANTIQ_USIF_BAUDRATE_COUNTER_CONFIG(bc, port);

	if (enabled)
		lantiq_usif_spi_clc_run_mode(port);

	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);

	printk ("USIF: setting baud rate to: %u\n", baudrate);
	port->prev_baudrate = baudrate;
	return 0;
}

/**
 * \fn static int lantiq_usif_spi_hwinit(struct lantiq_usif_port *port)
 * \brief USIF SPI hardware initialization.
 * Initializes the USIF SPI port hardware with the desired baudrate and
 * transmission options.
 *
 * \param   port    Pointer to structure #lantiq_usif_port
 * \return  0       OK
 * \return  -EINVAL Error during initialization
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int lantiq_usif_spi_hwinit(struct lantiq_usif_port *port)
{
	u32 reg = 0;

	lantiq_usif_spi_gpio_init();

	/* Enable USIF /AHBS */
	lantiq_usif_spi_pm_enable();

	/* RMC first */
	LANTIQ_USIF_SET_CLOCK_COUNTER(
		SM(LANTIQ_USIF_SPI_DEF_RMC, LANTIQ_USIF_CLC_CNT_RMC) |
		SM(LANTIQ_USIF_SPI_DEF_ORMC, LANTIQ_USIF_CLC_CNT_ORMC), port);

	/* Switch to USIF configuration mode, module enabled onece  */
	reg = LANTIQ_USIF_CLC_MOD_EN;
	LANTIQ_USIF_SET_RUN_CONTROL(reg, port);

	lantiq_usif_spi_clc_cfg_mode(port);

	/* Disable all interrupts. No interrupts at all except DMA */
	LANTIQ_USIF_SET_INT_MASK(0, port);
	LANTIQ_USIF_CLR_INT(LANTIQ_USIF_INT_ALL, port);

	if (lantiq_usif_spi_sethwopts(port) < 0) {
		printk(KERN_ERR
			"%s: setting the hardware options failed\n",
		__func__);
		return -EINVAL;
	}

	if (lantiq_usif_spi_set_baudrate(port, port->baudrate) < 0) {
		printk(KERN_ERR "%s: setting the baud rate failed\n",
			__func__);
		return -EINVAL;
	}

	LANTIQ_USIF_SPI_IRQ_LOCK(port);

	reg = LANTIQ_USIF_SPI_FIFO_SETUP;

	/*
	 * Enable TX swap and RX swap, must be set so that it is
	 * compatible with old SSC module.
	 */
	reg |= LANTIQ_USIF_FIFO_CFG_TX_SWAP | LANTIQ_USIF_FIFO_CFG_RX_SWAP;
	/* Configure the TX/RX FIFO */
	LANTIQ_USIF_SET_FIFO_CONFIG(reg, port);

	/* Switch to USIF working mode */
	lantiq_usif_spi_clc_run_mode(port);

	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);

	/* FIFO information for later use */
	port->tx_fifo_size_words = LANTIQ_USIF_GET_TX_FIFO_SIZE(port);
	port->tx_fifo_size_bytes = (port->tx_fifo_size_words << 2);
	port->rx_fifo_size_words = LANTIQ_USIF_GET_RX_FIFO_SIZE(port);
	port->rx_fifo_size_bytes = (port->rx_fifo_size_words << 2);
	return 0;
#undef LANTIQ_USIF_CLC_MAX_CNT
}

/**
 * \fn static INLINE int lantiq_usif_spi_txfifo(struct lantiq_usif_port *port,
 *     char *txbuf, u32 txsize)
 * \brief Called to transmit data to USIF SPI using FIFO mode .
 * \param   port       Pointer to structure #lantiq_usif_port
 * \param   txbuf      Pointer to the data packet to transmit
 * \param   txsize     Amount of Bytes to transmit
 * \return  >= 0       Number of bytes transmitted
 *          < 0        error number
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE int
lantiq_usif_spi_txfifo(struct lantiq_usif_port *port, char *txbuf, u32 txsize)
{
	LANTIQ_KASSERT(port->opts.modeRxTx == LANTIQ_USIF_SPI_MODE_TX,
		("%s invalid txrx mode\n", __func__));

	lantiq_usif_spi_dma_setup(port, LANTIQ_USIF_SPI_DIR_TXRX,
		LANTIQ_USIF_SPI_DMA_DISABLE);
	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	/* Start the transmission */
	lantiq_usif_spi_start_txfifo(port, txbuf, txsize);
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);

	return txsize;
}

/**
 * \fn static INLINE int lantiq_usif_spi_rxfifo(struct lantiq_usif_port *port,
 *     char *rxbuf, u32 rxsize)
 * \brief Called to receive from USIF SPI using FIFO mode .
 * \param   port       Pointer to structure #lantiq_usif_port
 * \param   rxbuf      Pointer to store the received data packet
 * \param   rxsize     Amount of Bytes to receive.
 * \return  >= 0       Number of bytes received
 *          < 0        error number
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE int
lantiq_usif_spi_rxfifo(struct lantiq_usif_port *port, char *rxbuf, u32 rxsize)
{
	LANTIQ_KASSERT(port->opts.modeRxTx == LANTIQ_USIF_SPI_MODE_RX,
		("%s invalid txrx mode\n", __func__));

	lantiq_usif_spi_dma_setup(port, LANTIQ_USIF_SPI_DIR_TXRX,
		LANTIQ_USIF_SPI_DMA_DISABLE);
	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	lantiq_usif_spi_start_rxfifo(port, rxbuf, rxsize);
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);

	return rxsize;
}

/**
 * \fn static INLINE int lantiq_usif_spi_set_spi_mode(lantiq_usif_device_t *dev)
 * \brief Sets the spi mode of the corresponding device. USIF SPI mode is
 * per device parameter. It is initialized during registeration
 *
 * \param   dev     Pointer to device
 * \return  0       OK
 * \return  -EBUSY  could not set usif spi mode because serial bus is busy
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE int lantiq_usif_spi_set_spi_mode(lantiq_usif_device_t *dev)
{
	u32 reg;
	int val = 0;
	int enabled = 0;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg;
	struct lantiq_usif_port *port;

	usif_cfg = &dev->conn_id;
	port = dev->port;

	if (port->prev_spi_mode == usif_cfg->spi_mode)
		return 0;


	/* Check if there is activity in USIF bus */
	if ((LANTIQ_USIF_GET_FIFO_STATUS(port) &
		LANTIQ_USIF_FIFO_STAT_TXRX_FFS_MASK) != 0x00) {
		printk(KERN_ERR "%s is busy\n", __func__);
		return -EBUSY;
	}

	switch (usif_cfg->spi_mode) {
	case LANTIQ_USIF_SPI_MODE_0:
		val = LANTIQ_USIF_MODE_CFG_SC_PH;
		break;
	case LANTIQ_USIF_SPI_MODE_1:
		val = 0;
		break;
	case LANTIQ_USIF_SPI_MODE_2:
		val = LANTIQ_USIF_MODE_CFG_SC_POL | LANTIQ_USIF_MODE_CFG_SC_PH;
		break;
	case LANTIQ_USIF_SPI_MODE_3:
		val = LANTIQ_USIF_MODE_CFG_SC_POL;
		break;
	default:		/* Mode 0 */
		val = LANTIQ_USIF_MODE_CFG_SC_PH;
		break;
	}
	LANTIQ_USIF_SPI_IRQ_LOCK(port);

	enabled = LANTIQ_USIG_GET_CLOCK_STATUS(port) & LANTIQ_USIF_CLC_STAT_RUN;
	if (enabled)
		lantiq_usif_spi_clc_cfg_mode(port);

	reg = LANTIQ_USIF_GET_MODE_CONFIG(port);

	reg &= ~LANTIQ_USIF_MODE_CFG_SC_MODE_MASK;

	reg |= val;
	LANTIQ_USIF_SET_MODE_CONFIG(reg, port);

	if (enabled)
		lantiq_usif_spi_clc_run_mode(port);

	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	port->prev_spi_mode = usif_cfg->spi_mode;
	return 0;
}

/**
 * \fn static INLINE void lantiq_usif_spi_tx_setup(struct lantiq_usif_port *port)
 * \brief USIF SPI set TX mode
 *
 * \param   port    Pointer to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void lantiq_usif_spi_tx_setup(struct lantiq_usif_port *port)
{
	if (port->opts.modeRxTx != LANTIQ_USIF_SPI_MODE_TX)
		lantiq_usif_spi_rxtx_mode_set(port, LANTIQ_USIF_SPI_MODE_TX);
}

/**
 * \fn static INLINE void lantiq_usif_spi_rx_setup(struct lantiq_usif_port *port)
 * \brief  USIF SPI set RX mode
 *
 * \param   port    Pointer to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void lantiq_usif_spi_rx_setup(struct lantiq_usif_port *port)
{
	if (port->opts.modeRxTx != LANTIQ_USIF_SPI_MODE_RX)
		lantiq_usif_spi_rxtx_mode_set(port, LANTIQ_USIF_SPI_MODE_RX);
}

/**
 * \fn static INLINE void lantiq_usif_spi_txrx_setup(struct lantiq_usif_port *port)
 * \brief  USIF SPI set TXRX mode/full duplex
 *
 * \param   port    Pointer to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void lantiq_usif_spi_txrx_setup(struct lantiq_usif_port *port)
{
	if (port->opts.modeRxTx != LANTIQ_USIF_SPI_MODE_RXTX)
		lantiq_usif_spi_rxtx_mode_set(port, LANTIQ_USIF_SPI_MODE_RXTX);
}

/**
 * \fn static void lantiq_usif_spi_enqueue(LANTIQ_USIF_QUEUE_t *queue)
 * \brief USIF add queue entry to priority queue
 *
 * \param   queue    Pointer to structure #LANTIQ_USIF_QUEUE_t
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static void lantiq_usif_spi_enqueue(LANTIQ_USIF_QUEUE_t *queue)
{
	lantiq_usif_device_t *dev;
	struct lantiq_usif_port *port;

	LANTIQ_KASSERT((queue != NULL),
		("%s should never happen\n", __func__));
	dev = queue->dev;
	port = dev->port;
	LANTIQ_USIF_PRINT(port, USIF_MSG_QUEUE,
		"%s dev %s prio %d enqueued\n", __func__,
		dev->dev_name, dev->dev_prio);

	LANTIQ_USIF_SPI_Q_LOCK_BH(port);
	if (dev->dev_prio == LANTIQ_USIF_SPI_PRIO_ASYNC) {
		TAILQ_INSERT_TAIL(&port->usif_asyncq, queue, q_next);
	} else {
		TAILQ_INSERT_TAIL(&port->usif_syncq[dev->dev_prio],
			queue, q_next);
	}
	dev->stats.enqueue++;
	LANTIQ_USIF_SPI_Q_UNLOCK_BH(port);
}

/**
 * \fn static void lantiq_usif_spi_dequeue(LANTIQ_USIF_QUEUE_t *queue)
 * \brief USIF remove queue entry from priority queue
 *
 * \param   queue    Pointer to structure #LANTIQ_USIF_QUEUE_t
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static void lantiq_usif_spi_dequeue(LANTIQ_USIF_QUEUE_t *queue)
{
	lantiq_usif_device_t *dev;
	struct lantiq_usif_port *port;

	LANTIQ_KASSERT((queue != NULL),
		("%s should never happen\n", __func__));
	dev = queue->dev;
	port = dev->port;
	LANTIQ_USIF_PRINT(port, USIF_MSG_QUEUE,
		"%s dev %s prio %d dequeued\n", __func__,
		dev->dev_name, dev->dev_prio);

	LANTIQ_USIF_SPI_Q_LOCK_BH(port);
	if (dev->dev_prio == LANTIQ_USIF_SPI_PRIO_ASYNC) {
		if (!TAILQ_EMPTY(&port->usif_asyncq))
			TAILQ_REMOVE(&port->usif_asyncq, queue, q_next);

	} else {
		if (!TAILQ_EMPTY(&port->usif_syncq[dev->dev_prio])) {
			TAILQ_REMOVE(&port->usif_syncq[dev->dev_prio],
				queue, q_next);
		}
	}
	dev->stats.dequeue++;
	LANTIQ_USIF_SPI_Q_UNLOCK_BH(port);
}

/**
 * \fn static void lantiq_usif_spi_cs_lock(lantiq_usif_device_t *dev)
 * \brief USIF chip select function, set spi mode, baudrate, call
 *  registered device-specific cs set function.
 *
 * \param   dev    Pointer to structure #lantiq_usif_device_t
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static void lantiq_usif_spi_cs_lock(lantiq_usif_device_t *dev)
{
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg;
	struct lantiq_usif_port *port;

	port = dev->port;
	if (port->usif_cs_locked == true) {
		printk(KERN_ERR "Fatal error: %s locked already before\n",
			__func__);
		return;
	}
	LANTIQ_USIF_PRINT(dev->port, USIF_MSG_LOCK, "%s %s enter\n", __func__,
		dev->dev_name);

	port->usif_cs_locked = true;

	usif_cfg = &dev->conn_id;
	lantiq_usif_spi_set_spi_mode(dev);
	lantiq_usif_spi_set_baudrate(port, usif_cfg->baudrate);
	if (usif_cfg->csset_cb != NULL)
		usif_cfg->csset_cb(LANTIQ_USIF_SPI_CS_ON, usif_cfg->cs_data);

}

/**
 * \fn static INLINE void lantiq_usif_spi_cs_unlock(lantiq_usif_device_t *dev)
 * \brief USIF chip unselect function,  call registered device-specific
 *  cs reset function.
 *
 * \param   dev    Pointer to structure #lantiq_usif_device_t
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void lantiq_usif_spi_cs_unlock(lantiq_usif_device_t *dev)
{
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg;
	struct lantiq_usif_port *port;

	port = dev->port;
	if (port->usif_cs_locked == false) {
		printk(KERN_ERR
			"Fatal error: %s unlocked already before\n",
			__func__);
		return;
	}
	usif_cfg = &dev->conn_id;
	if (usif_cfg->csset_cb != NULL)
		usif_cfg->csset_cb(LANTIQ_USIF_SPI_CS_OFF, usif_cfg->cs_data);

	port->usif_cs_locked = false;
	LANTIQ_USIF_PRINT(port, USIF_MSG_LOCK, "%s %s exit\n", __func__,
		dev->dev_name);
}

/**
 *\fn int lantiq_usif_spiLock(LANTIQ_USIF_SPI_HANDLE_t handler)
 *\brief  Called to lock and reserve the whole USIF SPI interface
 * for the given 'handler'
 *
 * The chipselect, belonging to this USIF SPI session is already
 * activated. This means the chipselect callback is called.
 * After complete data transmission and reception, lantiq_usif_spiUnLock
 * has to be called to release the USIF SPI interface again for
 * other clients.
 *
 * \param   handler    Handle of the connection where to make the
 *                     configuration on
 * \return  = 0
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int lantiq_usif_spiLock(LANTIQ_USIF_SPI_HANDLE_t handler)
{
	lantiq_usif_device_t *dev;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg;
	struct lantiq_usif_port *port;
	LANTIQ_USIF_QUEUE_t *pqueue;

	LANTIQ_KASSERT((handler != NULL),
		("%s Invalid parameter\n", __func__));

	dev = (lantiq_usif_device_t *) handler;
	pqueue = &dev->queue;
	usif_cfg = &dev->conn_id;
	port = dev->port;

	if (port->lock_qentry == pqueue) {
		/* We hold the lock already -> nothing to request here! */
		return -1;
	}

	/*
	 * Check if the queue entry of the ConnId is already queued with
	 * a request but this request is not served yet. Every ConnId can
	 * only queue up one request at one time.
	 */
	if (atomic_read(&pqueue->isqueued) == 1)
		return -1;

	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	/* Initialize unused variable */
	pqueue->txbuf = NULL;
	pqueue->txsize = 0;
	pqueue->rxbuf = NULL;
	pqueue->rxsize = 0;

	/* Place a lock request in the queue */
	pqueue->request_lock = true;
	
	atomic_set(&pqueue->isqueued, 1);
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	/* Add queue entry to priority queue */
	lantiq_usif_spi_enqueue(pqueue);

	/*
	 * If no async support is avaiable, trigger the USIF kernel thread and
	 * wait pending till job is done.
	 */
#ifdef CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS
	LANTIQ_USIF_PRINT(port, USIF_MSG_TASKLET, "%s raise fake interrupt\n",
		__func__);
	lantiq_usif_spi_start_tasklet(port);
#else
	LANTIQ_USIF_PRINT(port, USIF_MSG_THREAD,
		"%s wake up usif kernel thread\n", __func__);
	/* Kick off USIF kernel thread */
	LANTIQ_USIF_SPI_WAKEUP_EVENT(port->usif_thread_wait,
		LANTIQ_USIF_SPI_THREAD_EVENT, port->event_flags);
#endif /* CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS */
	/* Wait till wake up from USIF kernel thread */
	LANTIQ_USIF_SPI_WAIT_EVENT(dev->dev_thread_wait,
		LANTIQ_USIF_SPI_DEV_THREAD_EVENT, dev->event_flags);
	LANTIQ_USIF_PRINT(port, USIF_MSG_THREAD,
		"%s wakeup received from usif kernel thread\n", __func__);
	return 0;
}
EXPORT_SYMBOL(lantiq_usif_spiLock);

/**
 *\fn int lantiq_usif_spiUnlock(LANTIQ_USIF_SPI_HANDLE_t handler)
 * \brief  This function releases the USIF SPI lock that was placed before by
 * calling lantiq_usif_spiLock.
 *
 * This function also inactivate the chipselect signal, which was set in
 * lantiq_usif_spiLock.
 *
 * \param   handler    Handle of the connection where to make the
 *                     configuration on
 * \return  = 0
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int lantiq_usif_spiUnlock(LANTIQ_USIF_SPI_HANDLE_t handler)
{
	lantiq_usif_device_t *dev;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg = NULL;
	struct lantiq_usif_port *port = NULL;
	LANTIQ_USIF_QUEUE_t *pqueue;

	LANTIQ_KASSERT((handler != NULL),
		("%s Invalid parameter\n", __func__));

	dev = (lantiq_usif_device_t *) handler;
	pqueue = &dev->queue;
	usif_cfg = &dev->conn_id;
	port = dev->port;

	if (port->lock_qentry != &dev->queue) {
		/* We do not hold the lock, therefore we can not release it! */
		return -1;
	}

	/* Just forget about the lock, then the USIF driver would just take it
	 * as a normel queue entry
	 */
	lantiq_usif_spi_cs_unlock(dev);
	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	port->lock_qentry = NULL;
	pqueue->txbuf = NULL;
	pqueue->txsize = 0;
	pqueue->rxbuf = NULL;
	pqueue->rxsize = 0;
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	return 0;
}
EXPORT_SYMBOL(lantiq_usif_spiUnlock);

/**
 * \fn int lantiq_usif_spiSetBaud(LANTIQ_USIF_SPI_HANDLE_t handler,
 *     unsigned int baud)
 * \brief Configures the Baudrate of a given connection.
 *
 * The baudrate can also be change multiple times
 * for a single connection. The baudrate change
 * will take place for the next call of lantiq_usif_spiTx,
 * lantiq_usif_spiRx or lantiq_usif_spiTxRx.
 *
 * \param   handler    Handle of the connection where to make the
 *                     configuration on
 * \param   baud       Baudrate to configure. This value can be rounded
 *                     during the calculation of the USIF SPI clock divider
 *
 * \return  = 0        OK
 *          < 0        error number
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int lantiq_usif_spiSetBaud(LANTIQ_USIF_SPI_HANDLE_t handler, unsigned int baud)
{
	lantiq_usif_device_t *dev;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg;

	/* Sanity check */
	LANTIQ_KASSERT((handler != NULL),
		("%s Invalid parameter\n", __func__));

	dev = (lantiq_usif_device_t *) handler;
	usif_cfg = &dev->conn_id;
	/* XXX, protection because of being used in other places */
	usif_cfg->baudrate = baud;
	return 0;
}
EXPORT_SYMBOL(lantiq_usif_spiSetBaud);

/**
 * \fn static int lantiq_usif_spi_txdma(struct lantiq_usif_port *port,
 *     char *txbuf, int txsize)
 * \brief Called to transmit the data using DMA mode .
 *
 * \param   port       Pointer to structure #lantiq_usif_port
 * \param   txbuf     Pointer to the data packet to transmit
 * \param   txsize     Amount of Bytes to transmit
 * \return  >= 0       Number of bytes transmitted
 *          < 0        error number
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int
lantiq_usif_spi_txdma(struct lantiq_usif_port *port, char *txbuf, int txsize)
{
	int retval = 0;
	struct dma_device_info *dma_dev;

	dma_dev = port->dma_dev;

	/**
	* Set a flag that we are waiting for the DMA to complete. This flag
	* will be reseted again in the DMA interrupt.
	* NB, it must be ahead of the following stuff, because once descriptor
	* is prepared, interrupt may come back immediately
	*/
	atomic_set(&port->dma_wait_state, 1);

	lantiq_usif_spi_dma_setup(port, LANTIQ_USIF_SPI_DIR_TX,
		LANTIQ_USIF_SPI_DMA_ENABLE);
	/* TX DMA only half duplex is used */
	port->dma_is_in_half_duplex = 1;
	/**
	* Run in tasklet or kernel thread, DMA tasklet may run the same
	* function Lock must be used.
	* Although descriptor length field requires multiple of dma burst
	* length, The real size should be set up, otherwise, more clocks
	* than expected will be transmitted to the serial line.
	*/
	retval = dma_device_write(dma_dev, txbuf, txsize, NULL);

	LANTIQ_KASSERT(retval == txsize, ("%s retval %d != txsize %d\n",
		__func__, retval, txsize));

	return retval;
}

/**
 * \fn static int lantiq_usif_spi_dma_fdx(struct lantiq_usif_port *port,
 *     char *txbuf, int txsize, char *rxbuf, int rxsize)
 * \brief Called to transmit/receive the data using DMA mode .
 *
 * \param   port       Pointer to structure #lantiq_usif_port
 * \param   txbuf      Pointer to the data packet to transmit
 * \param   txsize     Amount of Bytes to transmit
 * \param   rxbuf      Pointer to the data packet to be received
 * \param   rxsize     Amount of Bytes to be received
 * \return  >= 0       Number of bytes transmitted/received
 *          < 0        error number
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int
lantiq_usif_spi_dma_fdx(struct lantiq_usif_port *port, char *txbuf, int txsize,
		  char *rxbuf, int rxsize)
{
	int retval = 0;
	struct dma_device_info *dma_dev;
	dma_dev = port->dma_dev;

	/*
	 * FDX required TX/RX has the same amount of data and they must
	 * be a multiple of  of DMA burst length
	 */
	if (((rxsize & ((dma_dev->rx_burst_len << 2) - 1)) != 0) ||
	    ((txsize & ((dma_dev->tx_burst_len << 2) - 1)) != 0)) {
		printk(KERN_ERR "%s data length is not a multiple of DMA"
			" burst length %d\n", __func__, rxsize);
		return -EINVAL;
	}

	/* Backup original buffer, so that later we can find it
	   in dma handler */
	port->rxbuf_ptr = rxbuf;
	port->rx_len = rxsize;

	/* Disable interrupt */
	LANTIQ_USIF_SET_INT_MASK(0, port);

	dma_device_desc_setup(dma_dev, rxbuf, rxsize);

	/* D+ RX always enabled, RX Class MASK */
	lantiq_usif_spi_dma_setup(port, LANTIQ_USIF_SPI_DIR_TXRX,
		LANTIQ_USIF_SPI_DMA_ENABLE);

	/* Enable SPI DMA channel */
	(dma_dev->rx_chan[dma_dev->current_rx_chan])
		->open(dma_dev->rx_chan[dma_dev->current_rx_chan]);

	/*
	 * Set a flag that we are waiting for the DMA to complete. This flag
	 * will be reseted again in the DMA interrupt.
	 */
	atomic_set(&port->dma_wait_state, 1);

	port->dma_is_in_half_duplex = 0;

	/* TX/RX mode also should program MRPS to trigger RX operation */
	LANTIQ_USIF_CONFIG_RX_MRPS(SM(rxsize, LANTIQ_USIF_MRPS_CTRL_MRPS), port);

	/* TX real data sent and it will drive clock out at the same time */

	retval = dma_device_write(dma_dev, txbuf, txsize, NULL);

	LANTIQ_KASSERT(retval == txsize, ("%s retval %d != txsize %d\n",
		__func__, retval, txsize));

	return retval;
}

/**
 * \fn static int lantiq_usif_spi_rxdma(struct lantiq_usif_port *port,
 *     char *rxbuf, int rxsize)
 * \brief Called to receive the data using DMA mode .
 *
 * \param   port       Pointer to structure #lantiq_usif_port
 * \param   rxbuf      Pointer to the data packet to be received
 * \param   rxsize     Amount of Bytes to be received
 * \return  >= 0       Number of bytes received
 *          < 0        error number
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int
lantiq_usif_spi_rxdma(struct lantiq_usif_port *port, char *rxbuf, int rxsize)
{
	char *pbuf;
	int dma_rxsize;
	int retval = 0;
	u32 dma_burst_len;
	u32 dma_burst_len_mask;
	struct dma_device_info *dma_dev;

	dma_dev = port->dma_dev;
	dma_burst_len = (dma_dev->rx_burst_len << 2);
	dma_burst_len_mask = dma_burst_len - 1;

	/* Backup original buffer, so that later we can find it
	   in dma handler */
	port->rxbuf_ptr = rxbuf;
	port->rx_len = rxsize;
	/* Can't be divisible by burst length  */
	if ((rxsize & dma_burst_len_mask)) {
		pbuf = port->dma_rxbuf;
		 /* Round up burst length to make sure enough space */
		dma_rxsize = (rxsize & ~dma_burst_len_mask) + dma_burst_len;
		LANTIQ_KASSERT(dma_rxsize <= DEFAULT_USIF_SPI_FRAGMENT_SIZE,
			("%s fragment %d out of range\n", __func__,
			dma_rxsize));
	} else {
		pbuf = rxbuf;
		dma_rxsize = rxsize;
	}

	/* Disable interrupt */
	LANTIQ_USIF_SET_INT_MASK(0, port);

	dma_device_desc_setup(dma_dev, pbuf, dma_rxsize);

	/* D+ RX always enabled, RX Class MASK */
	lantiq_usif_spi_dma_setup(port, LANTIQ_USIF_SPI_DIR_TXRX,
		LANTIQ_USIF_SPI_DMA_ENABLE);

	/* Enable SPI DMA channel */
	(dma_dev->rx_chan[dma_dev->current_rx_chan])
		->open(dma_dev->rx_chan[dma_dev->current_rx_chan]);

	/*
	 * Set a flag that we are waiting for the DMA to complete. This flag
	 * will be reseted again in the DMA interrupt.
	 */
	atomic_set(&port->dma_wait_state, 1);

	port->dma_is_in_half_duplex = 0;

	/* TX/RX mode also should program MRPS to trigger RX operation */
	LANTIQ_USIF_CONFIG_RX_MRPS(SM(dma_rxsize, LANTIQ_USIF_MRPS_CTRL_MRPS), port);

	/* TX dummy data sent */

	retval = dma_device_write(dma_dev, port->dma_txbuf, dma_rxsize, NULL);

	LANTIQ_KASSERT(retval == dma_rxsize, ("%s retval %d != txsize %d\n",
		__func__, retval, dma_rxsize));

	return rxsize;
}

/**
 *\fn int lantiq_usif_spiTxRx (LANTIQ_USIF_SPI_HANDLE_t handler, char *txbuf,
 *        u32 txsize, char *rxbuf, u32 rxsize)
 *\brief Called to transmit/receive to/from USIF SPI in one step.

 * This means that the data transmission and reception is done in parallel.
 * No DMA is possible here. The USIF SPI driver sets the chipselect when the
 * data transmission starts and resets it when the transmission is
 * completed. The transmit and receive buffer memory allocation and
 * de-allocation is done by the USIF SPI client.
 *
 * \param   handler    Handle of the connection where to make the
 *                     configuration on
 * \param   txbuf     Pointer to the data packet to transmit
 * \param   txsize    Amount of Bytes to transmit
 * \param   rxbuf     Pointer to store the received data packet
 * \param   rxsize    Amount of Bytes to receive.
 * \return  >= 0      Number of bytes received (if rx_buf != 0) or transmitted
 * \return  < 0       error number
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int
lantiq_usif_spiTxRx(LANTIQ_USIF_SPI_HANDLE_t handler, char *txbuf, u32 txsize,
		 char *rxbuf, u32 rxsize)
{
	lantiq_usif_device_t *dev;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg;
	LANTIQ_USIF_QUEUE_t *pqueue;
	struct lantiq_usif_port *port;
	struct usif_device_stats *stats;

	/* Sanity check */
	LANTIQ_KASSERT((handler != NULL),
		("%s Invalid parameter\n", __func__));

	LANTIQ_KASSERT(!((rxbuf == NULL) && (rxsize == 0)
		&& (txbuf == NULL) && (txsize == 0)),
		("%s Invalid parameter\n", __func__));

	dev = (lantiq_usif_device_t *) handler;
	stats = &dev->stats;
	port = dev->port;

	if (in_interrupt()) {
		stats->context_err++;
		printk(KERN_ERR "%s can't be called in interupt context"
			"< irq, softirq, tasklet>\n", __func__);
		return 0;
	}
	usif_cfg = &dev->conn_id;
	pqueue = &dev->queue;

	if ((txsize > usif_cfg->fragSize) || (rxsize > usif_cfg->fragSize)) {
		stats->frag_err++;
		printk(KERN_ERR "%s Device driver must do its own fragmentation"
			" tx %d, rx %d > %d\n", __func__, txsize, rxsize,
			usif_cfg->fragSize);
		return 0;
	}

	if ((txsize != 0) && (rxsize != 0)) {
		if (usif_cfg->duplex_mode == LANTIQ_USIF_SPI_FULL_DUPLEX) {
			if (txsize != rxsize) {
				stats->dlx_err++;
				printk(KERN_ERR "%s full duplex required TX/RX"
					" has the same amount of data tx %d"
					" rx %d\n", __func__, txsize, rxsize);
				return 0;
			}
		}
	}
	/*
	 * Ensure that only asynchronous USIF Handles could enqueue a
	 * synchronous request. The parameter 'handle_type' is set during the
	 * ConnId allocation process.
	 */
	if (pqueue->handle_type != LANTIQ_USIF_SPI_HANDL_TYPE_SYNC) {
		stats->handler_err++;
		printk(KERN_ERR "%s must use sync handler\n", __func__);
		return 0;
	}

	/*
	 * Check if the queue entry of the ConnId is already queued with
	 * a request but this request is not served yet. Every ConnId can
	 * only queue up one request at one time.
	 */
	if (atomic_read(&pqueue->isqueued) == 1) {
		stats->dup_qentries++;
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s same queue has queued more than once\n",
			__func__);
		return 0;
	}
	/* Add pointer and sizes to the queue entry of this USIF handle. */
	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	pqueue->txbuf = txbuf;
	pqueue->txsize = txsize;
	pqueue->rxbuf = rxbuf;
	pqueue->rxsize = rxsize;
	atomic_set(&pqueue->isqueued, 1);
	pqueue->exchange_bytes = 0;
	pqueue->request_lock = false;
	memset(&pqueue->callback, 0,
		sizeof(LANTIQ_USIF_SPI_ASYNC_CALLBACK_t));
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	/* Add queue entry to priority queue */
	lantiq_usif_spi_enqueue(pqueue);

	/* If no async support is avaiable, trigger the USIF kernel thread and
	 * wait pending till job is done.
	 */
#ifdef CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS
	LANTIQ_USIF_PRINT(port, USIF_MSG_TASKLET, "%s raise fake interrupt\n",
		__func__);
	lantiq_usif_spi_start_tasklet(port);
#else
	LANTIQ_USIF_PRINT(port, USIF_MSG_THREAD,
		"%s wake up usif kernel thread\n", __func__);
	/* Kick off USIF kernel thread */
	LANTIQ_USIF_SPI_WAKEUP_EVENT(port->usif_thread_wait,
		LANTIQ_USIF_SPI_THREAD_EVENT, port->event_flags);
#endif /* CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS */
	/* Wait till wakeup from USIF kernel thread */
	LANTIQ_USIF_SPI_WAIT_EVENT(dev->dev_thread_wait,
		LANTIQ_USIF_SPI_DEV_THREAD_EVENT, dev->event_flags);
	LANTIQ_USIF_PRINT(port, USIF_MSG_THREAD,
		"%s wakeup event received from usif kernel thread\n",
		__func__);
	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	/* Reset queue pointer */
	pqueue->txbuf = NULL;
	pqueue->rxbuf = NULL;
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	return pqueue->txsize + pqueue->rxsize;
}
EXPORT_SYMBOL(lantiq_usif_spiTxRx);

/**
 *\fn int lantiq_usif_spiTx(LANTIQ_USIF_SPI_HANDLE_t handler, char *txbuf, u32 txsize)
 *\brief Called to transmit the data.

 * transmission starts and resets it when the transmission
 * the transmit buffer is done by the USIF SPI client.
 *
 * \param   handler    Handle of the connection where to make the
 *                     configuration on
 * \param   txbuf      Pointer to the data packet to transmit
 * \param   txsize     Amount of Bytes to transmit
 * \return  >= 0       Number of bytes transmitted
 * \return  < 0        error number
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int lantiq_usif_spiTx(LANTIQ_USIF_SPI_HANDLE_t handler, char *txbuf, u32 txsize)
{
	return lantiq_usif_spiTxRx(handler, txbuf, txsize, NULL, 0);
}
EXPORT_SYMBOL(lantiq_usif_spiTx);

/**
 *\fn int lantiq_usif_spiRx(LANTIQ_USIF_SPI_HANDLE_t handler, char *rxbuf, u32 rxsize)
 *\brief Called to receive the data.

 * The USIF SPI driver sets the chipselect when the data reception starts and
 * resets it when the reception is completed. The memory allocation and
 * de-allocation of the receive buffer is done by the USIF SPI client.
 *
 * \param   handler    Handle of the connection where to make the
 *                     configuration on
 * \param   rxbuf      Pointer to the data packet to be received
 * \param   rxsize     Amount of Bytes to be received
 * \return  >= 0       Number of bytes received
 * \return  < 0        error number
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int lantiq_usif_spiRx(LANTIQ_USIF_SPI_HANDLE_t handler, char *rxbuf, u32 rxsize)
{
	return lantiq_usif_spiTxRx(handler, NULL, 0, rxbuf, rxsize);
}
EXPORT_SYMBOL(lantiq_usif_spiRx);

/**
 * \fn static int lantiq_usif_spi_serve_qentry(struct lantiq_usif_port *port)
 * \brief Called to serve every queue entry and it is a common function for
 * USIF kernel thread and tasklet
 *
 * \param   port       Pointer to structure #lantiq_usif_port
 * \return  1          Continue to loop this function until return 0
 * \return  0          Immediately exit this function. For kernel thread,
 *                     it will sleep, for tasklet, it will exit dynamically
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int lantiq_usif_spi_serve_qentry(struct lantiq_usif_port *port)
{
	LANTIQ_USIF_QUEUE_t *qentry = NULL;
	lantiq_usif_device_t *dev;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg = NULL;
	struct dma_device_info *dma_dev;

	/*
	 * Don't serve the current or next queue entry in case we are currently
	 * waiting for the DMA interrupt to report the transmission completion.
	 */
	if (atomic_read(&port->dma_wait_state) == 1) {
		/*
		 * DMA interrupt will spawn tasklet or wake up kernel thread
		 * in order to continue.
		 */
		return 0;
	}

	/* Identify the queue entry to serve */
	if (port->serve_qentry != NULL) {
		/* Continues serving the queue that was served before */
		qentry = port->serve_qentry;
	} else if (port->lock_qentry != NULL) {
		/*
		 * If one queue holds the lock, only serve this one element and
		 * ignore all others.
		 */
		qentry = port->lock_qentry;
		port->serve_qentry = qentry;
	} else {
		LANTIQ_USIF_PRINT(port, USIF_MSG_DUPLEX,
				"%s Locking SPI Q BH..\n", __func__);
		LANTIQ_USIF_SPI_Q_LOCK_BH(port);
		if (!TAILQ_EMPTY(&port->usif_asyncq)) {
			qentry = TAILQ_FIRST(&port->usif_asyncq);
		} else {
			int i;

			/* Choose the highest queue entry first */
			for (i = LANTIQ_USIF_SPI_PRIO_HIGH;
				i >= LANTIQ_USIF_SPI_PRIO_LOW; i--) {
				if (!TAILQ_EMPTY(&port->usif_syncq[i])) {
					qentry = TAILQ_FIRST
						(&port->usif_syncq[i]);
					break;
				}
			}
		}
		LANTIQ_USIF_SPI_Q_UNLOCK_BH(port);

		/* Remember that we are working on this queue now */
		port->serve_qentry = qentry;

		/*
		 * NB, Don't remove the qentry here since we may never come
		 * here once callers called sequence. It may cause
		 * enqueue/dequeue mismatch
		 */
	}

	/* No queue found that should be served */
	if (qentry == NULL)
		return 0;

	/* Get connection handle */
	dev = qentry->dev;
	usif_cfg = &dev->conn_id;
	dma_dev = port->dma_dev;
	/* This will pass cs to port for class information per queue entry */
	port->current_dev = dev;

	if ((usif_cfg->duplex_mode == LANTIQ_USIF_SPI_FULL_DUPLEX)
	    && (qentry->txbuf != NULL)
	    && (qentry->rxbuf != NULL)) {
		int tx_dma_aligned =
			((((u32) qentry->txbuf)
			& ((dma_dev->tx_burst_len << 2) - 1)) == 0) ? 1 : 0;
		int rx_dma_aligned =
			((((u32) qentry->rxbuf)
			& ((dma_dev->rx_burst_len << 2) - 1)) == 0) ? 1 : 0;
		if ((port->usif_cs_locked == false)
			&& (port->lock_qentry == NULL)) {
			LANTIQ_USIF_PRINT(port, USIF_MSG_DUPLEX,
				"%s Locking SPI CS..\n", __func__);
			lantiq_usif_spi_cs_lock(dev);
		}
		lantiq_usif_spi_txrx_setup(port);

		/* If buffer not aligned on DMA burst length,
		fall back to FIFO */
		if ((qentry->txsize > usif_cfg->maxFIFOSize)
		    && (tx_dma_aligned == 1)
		    && (qentry->rxsize > usif_cfg->maxFIFOSize)
		    && (rx_dma_aligned == 1)) {

			LANTIQ_USIF_PRINT(port, USIF_MSG_DUPLEX,
				"%s TX/RX DMA enter\n", __func__);
			lantiq_usif_spi_dma_fdx(port, qentry->txbuf,
				qentry->txsize, qentry->rxbuf,
				qentry->rxsize);

			port->stats.txDma++;
			port->stats.rxDma++;
			qentry->txbuf = NULL;
			qentry->exchange_bytes = qentry->txsize;

			qentry->rxbuf = NULL;
			qentry->exchange_bytes += qentry->rxsize;

			dev->stats.txBytes += qentry->txsize;
			port->stats.txBytes += qentry->txsize;
			port->stats.txDmaBytes += qentry->txsize;
			dev->stats.rxBytes += qentry->rxsize;
			port->stats.rxBytes += qentry->rxsize;
			port->stats.rxDmaBytes += qentry->rxsize;
			return 0;
		} else {
			LANTIQ_USIF_PRINT(port, USIF_MSG_DUPLEX,
				"%s TX/RX FIFO enter\n", __func__);
			lantiq_usif_spi_txrx_fifo_fdx(port, qentry->txbuf,
				qentry->txsize, qentry->rxbuf, qentry->rxsize);
			port->stats.txFifo++;
			port->stats.rxFifo++;

			qentry->txbuf = NULL;
			qentry->exchange_bytes = qentry->txsize;

			qentry->rxbuf = NULL;
			qentry->exchange_bytes += qentry->rxsize;

			dev->stats.txBytes += qentry->txsize;
			port->stats.txBytes += qentry->txsize;
			dev->stats.rxBytes += qentry->rxsize;
			port->stats.rxBytes += qentry->rxsize;
			return 1;
		}

	} else if (qentry->txbuf != NULL) {
		int tx_dma_aligned =
			((((u32) qentry->txbuf)
			& ((dma_dev->tx_burst_len << 2) - 1)) == 0) ? 1 : 0;

		LANTIQ_USIF_PRINT(port, USIF_MSG_TX_DMA,
				       "%s TX DMA enter\n", __func__);

		if ((port->usif_cs_locked == false)
			&& (port->lock_qentry == NULL)) {
			lantiq_usif_spi_cs_lock(dev);
		}

		lantiq_usif_spi_tx_setup(port);
		/* If buffer not aligned on DMA burst length,
		   fall back to FIFO */
		if ((qentry->txsize > usif_cfg->maxFIFOSize)
		    && (tx_dma_aligned == 1)) {
			lantiq_usif_spi_txdma(port, qentry->txbuf,
					   qentry->txsize);
			port->stats.txDma++;

			qentry->txbuf = NULL;

			qentry->exchange_bytes = qentry->txsize;
			dev->stats.txBytes += qentry->txsize;
			port->stats.txBytes += qentry->txsize;
			port->stats.txDmaBytes += qentry->txsize;
			return 0;
		} else {
			lantiq_usif_spi_txfifo(port, qentry->txbuf,
				qentry->txsize);
			port->stats.txFifo++;
			LANTIQ_USIF_PRINT(port, USIF_MSG_TX_FIFO,
				"%s TX FIFO enter\n", __func__);

			qentry->txbuf = NULL;

			qentry->exchange_bytes = qentry->txsize;
			dev->stats.txBytes += qentry->txsize;
			port->stats.txBytes += qentry->txsize;
			/*
			 * NB, Make sure data has been sent out
			 * XXX, remove polling by checking TX_FIN
			 */
			lantiq_usif_spi_wait_tx_finished(port);
			return 1;
		}
	} else if (qentry->rxbuf != NULL) {
		int rx_dma_aligned =
			((((u32) qentry->rxbuf)
			& ((dma_dev->rx_burst_len << 2) - 1)) == 0) ? 1 : 0;

		LANTIQ_USIF_PRINT(port, USIF_MSG_RX_DMA,
				"%s RX DMA enter\n", __func__);

		if ((port->usif_cs_locked == false)
		    && (port->lock_qentry == NULL)) {
			lantiq_usif_spi_cs_lock(dev);
		}

		/* If buffer not aligned on DMA burst length,
		   fall back to FIFO */
		if ((qentry->rxsize > usif_cfg->maxFIFOSize)
		    && (rx_dma_aligned == 1)) {
			lantiq_usif_spi_txrx_setup(port);
			lantiq_usif_spi_rxdma(port, qentry->rxbuf,
				qentry->rxsize);
			port->stats.rxDma++;
			port->stats.txDma++;

			qentry->rxbuf = NULL;
			qentry->exchange_bytes += qentry->rxsize;
			dev->stats.rxBytes += qentry->rxsize;
			port->stats.rxBytes += qentry->rxsize;
			port->stats.rxDmaBytes += qentry->rxsize;
			dev->stats.txBytes += qentry->rxsize;
			port->stats.txBytes += qentry->rxsize;
			port->stats.txDmaBytes += qentry->rxsize;
			return 0;
		} else {
			LANTIQ_USIF_PRINT(port, USIF_MSG_RX_FIFO,
				"%s RX FIFO enter\n", __func__);
			lantiq_usif_spi_rx_setup(port);
			lantiq_usif_spi_rxfifo(port, qentry->rxbuf,
				qentry->rxsize);
			port->stats.rxFifo++;

			qentry->rxbuf = NULL;
			qentry->exchange_bytes += qentry->rxsize;
			dev->stats.rxBytes += qentry->rxsize;
			port->stats.rxBytes += qentry->rxsize;
			return 1;
		}
	} else if (qentry->request_lock) {
		/* A lock request found */
		LANTIQ_USIF_PRINT(port, USIF_MSG_LOCK,
			"%s request lock enter\n", __func__);
		port->lock_qentry = qentry;
		qentry->request_lock = false;

		/* Serving the current queue entry is done */
		port->serve_qentry = NULL;

		/* Reset current device */
		port->current_dev = NULL;
		/*
		 * Reset the flag in the queue element that this one is queued
		 * with a request to be served.
		 */
		atomic_set(&qentry->isqueued, 0);

		/* syncAPI has the last watchdog branch but nested
		   callback has to dequeue it */
		if (qentry->handle_type == LANTIQ_USIF_SPI_HANDL_TYPE_ASYNC)
			lantiq_usif_spi_dequeue(qentry);

		/* Call the Chipselect set callback of the USIF-Handle */
		lantiq_usif_spi_cs_lock(dev);

		if (qentry->callback.pFunction) {
			/*
			 * Store the callback parameter local to cleanup
			 * the queue entry before calling the callback.
			 */
			LANTIQ_USIF_SPI_ASYNC_CALLBACK_t callback =
				qentry->callback;

			qentry->callback.pFunction = NULL;
			LANTIQ_USIF_PRINT(port, USIF_MSG_CALLBACK,
				"%s line%d request callback\n",
				__func__, __LINE__);
			/* Callback to call */
			callback.pFunction(callback.functionHandle, 0);
		}
		return 1;
	} else if (qentry->callback.pFunction != NULL) {

		LANTIQ_USIF_PRINT(port, USIF_MSG_CALLBACK,
			"%s line%d direct callback\n", __func__, __LINE__);
		/*
		 * Store the callback parameter local to cleanup the queue
		 * entry before calling the callback.
		 */
		LANTIQ_USIF_SPI_ASYNC_CALLBACK_t callback = qentry->callback;

		qentry->callback.pFunction = NULL;

		/* Serving the current queue entry is done */
		port->serve_qentry = NULL;

		/* Reset current device */
		port->current_dev = NULL;
		/*
		 * Reset the flag in the queue element that this one is queued
		 * with a request to be served.
		 */
		atomic_set(&qentry->isqueued, 0);
		lantiq_usif_spi_dequeue(qentry);

		if ((port->usif_cs_locked == true)
			&& (port->lock_qentry == NULL)) {

			lantiq_usif_spi_cs_unlock(dev);
		}

		/* Callback to call */
		callback.pFunction(callback.functionHandle,
			qentry->exchange_bytes);

		return 1;
	} else if (qentry->handle_type == LANTIQ_USIF_SPI_HANDL_TYPE_SYNC) {
		LANTIQ_USIF_PRINT(port, USIF_MSG_CALLBACK,
				"%s SPI_HANDLTYPE_SYNC enter\n", __func__);
		if ((port->usif_cs_locked == true)
		    && (port->lock_qentry == NULL)) {
			LANTIQ_USIF_PRINT(port, USIF_MSG_CALLBACK,
				"%s cs_unlock \n", __func__);
			lantiq_usif_spi_cs_unlock(dev);
		}
		/* Serving the current queue entry is done */
		port->serve_qentry = NULL;
		/* Reset current device */
		port->current_dev = NULL;
		/*
		 * Reset the flag in the queue element that this one is queued
		 * with a request to be served.
		 */
		atomic_set(&qentry->isqueued, 0);

		LANTIQ_USIF_PRINT(port, USIF_MSG_THREAD,
			"%s spi dequeue.. \n", __func__);
		lantiq_usif_spi_dequeue(qentry);

		/* Wake up the pending thread */
		LANTIQ_USIF_SPI_WAKEUP_EVENT(dev->dev_thread_wait,
			LANTIQ_USIF_SPI_DEV_THREAD_EVENT, dev->event_flags);
		LANTIQ_USIF_PRINT(port, USIF_MSG_THREAD,
			"%s wakeup USIF client kernel thread\n", __func__);
		/* syncAPI has no callback, we have to differentiate it */
		if (port->usif_cs_locked == true)
			return 0;
		else
			return 1;
	} else if ((qentry->handle_type == LANTIQ_USIF_SPI_HANDL_TYPE_ASYNC)
		   && (port->usif_cs_locked == true)) {
		/* Serving the current queue entry is done */
		port->serve_qentry = NULL;
		/* Don't let it continue,  but must restart tasklet
		   in somewhere else */
		return 0;
	}
	return 0;
}

#ifdef CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS
/**
 * \fn static INLINE void lantiq_usif_spi_tasklet_serve_queue
 * (struct lantiq_usif_port *port)
 * \brief Called to serve every queue entry in tasklet
 *
 * \param   port       Pointer to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void
lantiq_usif_spi_tasklet_serve_queue(struct lantiq_usif_port *port)
{
	/*
	 * Serve queue entries till no queue entry anymore to serve,
	 * we wait for DMA or the lock entry is not in the queue.
	 */
	while (lantiq_usif_spi_serve_qentry(port))
		;
}

/**
 * \fn static INLINE void lantiq_usif_spi_tasklet(unsigned long arg)
 * \brief USIF tasklet implementation
 *
 * \param   arg       cast to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void lantiq_usif_spi_tasklet(unsigned long arg)
{
	struct lantiq_usif_port *port = (struct lantiq_usif_port *) arg;

	LANTIQ_USIF_PRINT(port, USIF_MSG_TASKLET, "%s enter\n", __func__);
	lantiq_usif_spi_tasklet_serve_queue(port);
}

/**
 * \fn static INLINE void lantiq_usif_spi_tasklet_init(struct lantiq_usif_port *port)
 * \brief USIF tasklet initialization
 *
 * \param   port       Pointer to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE void lantiq_usif_spi_tasklet_init(struct lantiq_usif_port *port)
{
	tasklet_init(&port->usif_txrxq, lantiq_usif_spi_tasklet,
		(unsigned long) port);
}

#else
#define LANTIQ_USIF_THREAD_OPTIONS   (CLONE_FS | CLONE_FILES | CLONE_SIGHAND)

/**
 * \fn static int lantiq_usif_spi_kthread(void *arg)
 * \brief USIF kernel thread implementation function
 *
 * \param   arg       cast to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int lantiq_usif_spi_kthread(void *arg)
{
	struct lantiq_usif_port *port = (struct lantiq_usif_port *) arg;

	set_current_state(TASK_INTERRUPTIBLE);
	while (!kthread_should_stop()) {
		/* Wait for DMA interrupt or sync queue to wakes us up */
		LANTIQ_USIF_SPI_WAIT_EVENT(port->usif_thread_wait,
			LANTIQ_USIF_SPI_THREAD_EVENT, port->event_flags);
		LANTIQ_USIF_PRINT(port, USIF_MSG_THREAD,
			"%s DMA or sync queue event received\n",
			__func__);
		__set_current_state(TASK_RUNNING);

		/* Serve queue entries till no more queue entry to serve */
		while (lantiq_usif_spi_serve_qentry(port))
			;
		set_current_state(TASK_INTERRUPTIBLE);
	}

	return 0;
}

/**
 * \fn static INLINE int lantiq_usif_spi_thread_init(struct lantiq_usif_port *port)
 * \brief USIF kernel thread initialization
 *
 * \param   port       Pointer to structure #lantiq_usif_port
 * \return  none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static INLINE int lantiq_usif_spi_thread_init(struct lantiq_usif_port *port)
{
	struct task_struct *p;
	int error = 0;
	
	p = kthread_run(lantiq_usif_spi_kthread, (void *) port, "usif_kthread");
	if (IS_ERR(p))
		error = PTR_ERR(p);
	else
		port->usif_tsk = p;
	return error;
}
#endif /* CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS */

/**
 * \fn int lantiq_usif_spiAsyncTxRx(LANTIQ_USIF_SPI_HANDLE_t handler,
 *  LANTIQ_USIF_SPI_ASYNC_CALLBACK_t *pCallback, char *txbuf,
 *  int txsize, char *rxbuf, int rxsize)
 * \brief Transmit/receive to/from USIF in one step. It performs the data
 *  transmission and then the data reception.

 * The USIF driver sets the chipselect when the data transmission starts
 * and resets it when the transmission is completed.
 * This routine is called to start an asynchronous data transmission.
 * The provided callback routine is called after the transmission is done.
 *
 * \param   handler    Handle of the connection where to make the
 *                     configuration on
 * \param   pCallback  Function callback that is called after the request
 *                     is performed.
 * \param   txbuf      Pointer to the data packet to transmit.
 * \param   txsize     Amount of Bytes to transmit.
 * \param   rxbuf      Pointer to store the received data packet.
 * \param   rxsize     Amount of Bytes to receive.

 * \return Returns the amount of bytes that are transmitted in case of
 *  successful transmission. In case of an error the function returns -1
 *
 * \remarks This function activates the chipselect before data transmission
 *  and deactivates it after transmission (when function returns). This is
 * done with the callback function. The chipselect is not modified in case
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int
lantiq_usif_spiAsyncTxRx(LANTIQ_USIF_SPI_HANDLE_t handler,
		LANTIQ_USIF_SPI_ASYNC_CALLBACK_t *pCallback,
		char *txbuf, int txsize, char *rxbuf, int rxsize)
{
	lantiq_usif_device_t *dev;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg;
	LANTIQ_USIF_QUEUE_t *pqueue;
	struct lantiq_usif_port *port;
	struct usif_device_stats *stats;

	/* Sanity check */
	LANTIQ_KASSERT((handler != NULL),
		("%s Invalid parameter\n", __func__));

	LANTIQ_KASSERT(!((rxbuf == NULL) && (rxsize == 0)
		&& (txbuf == NULL) && (txsize == 0)),
		("%s Invalid parameter\n", __func__));

	dev = (lantiq_usif_device_t *) handler;
	stats = &dev->stats;
	port = dev->port;

	if (!in_interrupt()) {
		stats->context_err++;
		printk(KERN_ERR
			"%s must be called in interrupt context<irq, softirq, tasklet>\n",
			__func__);
		return -1;
	}

	usif_cfg = &dev->conn_id;
	pqueue = &dev->queue;

	if ((txsize > usif_cfg->fragSize) || (rxsize > usif_cfg->fragSize)) {
		stats->frag_err++;
		printk(KERN_ERR
			"%s Device driver must do its own fragmentation\n",
			__func__);
		return -1;
	}

	if ((txsize != 0) && (rxsize != 0)) {
		if (usif_cfg->duplex_mode == LANTIQ_USIF_SPI_FULL_DUPLEX) {
			if (txsize != rxsize) {
				stats->dlx_err++;
				printk(KERN_ERR
					"%s full duplex required TX/RX has"
					" the same amount of data tx %d rx %d\n",
					__func__, txsize, rxsize);
				return -1;
			}
		}
	}

	/*
	 * Ensure that only asynchronous USIF Handles could enqueue an
	 * asynchronous request. The parameter 'usifHandleType' is set
	 * during the ConnId allocation process.
	 */
	if (pqueue->handle_type != LANTIQ_USIF_SPI_HANDL_TYPE_ASYNC) {
		stats->handler_err++;
		printk(KERN_ERR "%s must use async handler\n", __func__);
		return -1;
	}

	/*
	 * Check if the queue entry of the ConnId is already queued with
	 * a request but this request is not served yet. Every ConnId can
	 * only queue up one request at the time.
	 */
	if (atomic_read(&pqueue->isqueued) == 1) {
		stats->dup_qentries++;
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s same queue has queued more than once\n", __func__);
		return -1;
	}

	/* Add pointer and sizes to the queue entry of this USIF handle. */
	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	pqueue->txbuf = txbuf;
	pqueue->txsize = txsize;
	pqueue->rxbuf = rxbuf;
	pqueue->rxsize = rxsize;
	if (pCallback != NULL)
		pqueue->callback = *pCallback;

	pqueue->request_lock = false;
	pqueue->exchange_bytes = 0;
	atomic_set(&pqueue->isqueued, 1);
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	/* Add queue entry to priority synchronous queue */
	lantiq_usif_spi_enqueue(pqueue);
	/*
	 * Trigger schedule or tasklet or fake interrupt according to different
	 * cases.
	 */
	lantiq_usif_spi_start_tasklet(port);
	return 0;
}
EXPORT_SYMBOL(lantiq_usif_spiAsyncTxRx);

/**
 * \fn int lantiq_usif_spiAsyncTx(LANTIQ_USIF_SPI_HANDLE_t handler,
 *	LANTIQ_USIF_SPI_ASYNC_CALLBACK_t *pCallback, char *txbuf, int txsize)
 * \brief transmit the data, located at "txbuf". The "txsize" amount of
 * bytes is transmitted over USIF.

 * The USIF driver sets the chipselect when the data transmission starts and
 * resets it when the transmission is completed.
 * This routine is called to start an asynchronous data transmission.
 * The provided callback routine is called after the transmission is done.
 *
 * \param   handler    Handle of the connection where to make the configuration
 *                     on
 * \param   pCallback  Function callback that is called after the request is
 *                     performed.
 * \param   txbuf      Pointer to the data packet to transmit.
 * \param   txsize     Amount of Bytes to transmit.

 * \return Returns the amount of bytes that are transmitted in case of
 *  successful transmission. In case of an error the function returns (-1).
 *
 * \remarks This function activates the chipselect before data transmission and
 *  deactivates it after transmission (when function returns). This is done
 *  with the callback function. The chipselect is not modified in case
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int
lantiq_usif_spiAsyncTx(LANTIQ_USIF_SPI_HANDLE_t handler,
		LANTIQ_USIF_SPI_ASYNC_CALLBACK_t *pCallback, char *txbuf,
		int txsize)
{
	return lantiq_usif_spiAsyncTxRx(handler,
		pCallback, txbuf, txsize, NULL, 0);
}
EXPORT_SYMBOL(lantiq_usif_spiAsyncTx);

/**
 * \fn int lantiq_usif_spiAsyncRx(LANTIQ_USIF_SPI_HANDLE_t handler,
 * LANTIQ_USIF_SPI_ASYNC_CALLBACK_t *pCallback, char *rxbuf, int rxsize)
 * \brief Receive from USIF. The received data are stored at "rxbuf".
 * The "rxsize" describes the amount of bytes to receive from USIF.

 * The USIF driver sets the chipselect when the data reception starts and
 * resets it when the reception is completed.
 *
 * \param   handler    Handle of the connection where to make the
 *                     configuration on
 * \param   pCallback  Function callback that is called after the request
 *                     is performed.
 * \param   rxbuf      Pointer to store the received data packet.
 * \param   rxsize     Amount of Bytes to receive.

 * \return  Returns the amount of bytes that are received in case of
 * successful transmission. In case of an error the function returns (-1).
 *
 * \remarks This function activates the chipselect before data transmission
 * and deactivates it after transmission (when function returns). This is
 * done with the callback function. The chipselect is not modified in case
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int
lantiq_usif_spiAsyncRx(LANTIQ_USIF_SPI_HANDLE_t handler,
	LANTIQ_USIF_SPI_ASYNC_CALLBACK_t *pCallback, char *rxbuf,
	int rxsize)
{
	return lantiq_usif_spiAsyncTxRx(handler,
		pCallback, NULL, 0, rxbuf, rxsize);
}
EXPORT_SYMBOL(lantiq_usif_spiAsyncRx);

/**
 * \fn int lantiq_usif_spiAsyncLock(LANTIQ_USIF_SPI_HANDLE_t handler,
 * LANTIQ_USIF_SPI_ASYNC_CALLBACK_t *pCallback)
 *
 * \brief This function locks and reserves the whole USIF interface
 * for the given 'handler'.

 *  The chipselect, belonging to this USIF session is already
 *  activated. This means the chipselect callback is called.
 *  After complete data transmission and reception,
 *  This routine is called to get the lock on tasklet level.
 *  The provided callback routine is called after the lock is set.
 *
 * \param handler Handle of the connection.
 * \param pCallback Function callback that is called after the request is
 *  performed.
 *
 * \return Return (0) in case of success, otherwise (-1) in case of errors.
 *
 * \remarks This function actives the USIF chipselect of this 'handler'
 * by calling the callback function.
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int
lantiq_usif_spiAsyncLock(LANTIQ_USIF_SPI_HANDLE_t handler,
		   LANTIQ_USIF_SPI_ASYNC_CALLBACK_t *pCallback)
{
	lantiq_usif_device_t *dev;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg;
	struct lantiq_usif_port *port;
	LANTIQ_USIF_QUEUE_t *pqueue;

	LANTIQ_KASSERT((handler != NULL),
		("%s Invalid parameter\n", __func__));
	LANTIQ_KASSERT((pCallback != NULL),
		("%s Invalid parameter\n", __func__));

	dev = (lantiq_usif_device_t *) handler;
	pqueue = &dev->queue;
	usif_cfg = &dev->conn_id;
	port = dev->port;

	if (port->lock_qentry == pqueue) {
		/* We hold the lock already -> nothing to request here! */
		return -1;
	}

	/*
	 * Check if the queue entry of the ConnId is already queued with
	 * a request  but this request is not served yet. Every ConnId can
	 * only queue up one  request at the time.
	 */
	if (atomic_read(&pqueue->isqueued) == 1)
		return -1;

	/* Initialize unused variable */
	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	pqueue->txbuf = NULL;
	pqueue->txsize = 0;
	pqueue->rxbuf = NULL;
	pqueue->rxsize = 0;

	/* Place a lock request in the queue */
	pqueue->request_lock = true;
	pqueue->callback = *pCallback;

	pqueue->exchange_bytes = 0;
	atomic_set(&pqueue->isqueued, 1);
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	/* Add queue entry to priority synchronous queue */
	lantiq_usif_spi_enqueue(pqueue);

	/*
	 * Calls the internal process to serve the queue. This routine would
	 * immediately return in case  the USIF hardware is currently used
	 * to serve another request.
	 */
	lantiq_usif_spi_start_tasklet(port);
	return 0;
}
EXPORT_SYMBOL(lantiq_usif_spiAsyncLock);
/**
 * \fn int lantiq_usif_spiAsyncUnLock(LANTIQ_USIF_SPI_HANDLE_t handler)
 * \brief This function releases the USIF lock that was placed before
 * by calling
 * \ref lantiq_usif_spiAsyncLock. This function also inactivate the chipselect
 * signal, which was set in \ref lantiq_usif_spiAsyncLock.

 * \param handler Handle of the connection.
 *
 * \return Return (0) in case of success, otherwise (-1) in case of errors.
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int lantiq_usif_spiAsyncUnLock(LANTIQ_USIF_SPI_HANDLE_t handler)
{
	lantiq_usif_device_t *dev;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg = NULL;
	struct lantiq_usif_port *port = NULL;
	LANTIQ_USIF_QUEUE_t *pqueue;

	LANTIQ_KASSERT((handler != NULL),
		("%s Invalid parameter\n", __func__));

	dev = (lantiq_usif_device_t *) handler;
	pqueue = &dev->queue;
	usif_cfg = &dev->conn_id;
	port = dev->port;

	if (port->lock_qentry != pqueue) {
		/* We do not hold the lock, therefore we can not release it! */
		return -1;
	}

	/* Just forget about the lock, then the USIF driver would just take it
	 * as a normel queue entry
	 */
	lantiq_usif_spi_cs_unlock(dev);

	/* Reset to the default value */
	LANTIQ_USIF_SPI_IRQ_LOCK(port);
	pqueue->txbuf = NULL;
	pqueue->txsize = 0;
	pqueue->rxbuf = NULL;
	pqueue->rxsize = 0;
	pqueue->exchange_bytes = 0;
	memset(&pqueue->callback, 0,
		sizeof(LANTIQ_USIF_SPI_ASYNC_CALLBACK_t));

	port->lock_qentry = NULL;
	LANTIQ_USIF_SPI_IRQ_UNLOCK(port);
	/* Restart tasklet since we are async release SPI bus */
	lantiq_usif_spi_start_tasklet(port);
	return 0;
}
EXPORT_SYMBOL(lantiq_usif_spiAsyncUnLock);

#ifdef CONFIG_SYSCTL
/*
 * Deal with the sysctl handler api changing.
 */

#define USIF_SYSCTL_DECL(f, ctl, write, filp, buffer, lenp, ppos) \
	f(ctl_table * ctl, int write, \
	void __user *buffer, size_t *lenp, loff_t *ppos)

#define USIF_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
	proc_dointvec(ctl, write, buffer, lenp, ppos)

enum {
	USIF_SPI_PRIV_FRAGMENT_SIZE = 1,
	USIF_SPI_PRIV_FIFO_SIZE = 2,
	USIF_SPI_PRIV_BAUDRATE = 3,
	USIF_SPI_PRIV_MODE = 4,
};

static int
USIF_SYSCTL_DECL(usif_sysctl_private, ctl, write, filp, buffer, lenp, ppos)
{
	lantiq_usif_device_t *dev = ctl->extra1;
	LANTIQ_USIF_SPI_CONFIGURE_t *usif_cfg = &dev->conn_id;
	struct lantiq_usif_port *port;
	u32 val;
	int ret;

	port = dev->port;
	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = USIF_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0) {
			switch ((long) ctl->extra2) {
			case USIF_SPI_PRIV_FRAGMENT_SIZE:
				if (val < LANTIQ_USIF_SPI_MIN_FRAGSIZE
				    || val > LANTIQ_USIF_SPI_MAX_FRAGSIZE)
					return -EINVAL;
				usif_cfg->fragSize = val;
				port->usif_frag_size = val;
				break;

			case USIF_SPI_PRIV_FIFO_SIZE:
				if (val < LANTIQ_USIF_SPI_FIFO_MIN_THRESHOULD
				    || val >
				    LANTIQ_USIF_SPI_FIFO_MAX_THRESHOULD)
					return -EINVAL;
				usif_cfg->maxFIFOSize = val;
				break;

			case USIF_SPI_PRIV_BAUDRATE:
				/* XXX, sanity check */
				usif_cfg->baudrate = val;
				break;

			case USIF_SPI_PRIV_MODE:
				ret = -EINVAL;
				break;

			default:
				return -EINVAL;
			}
		}
	} else {
		switch ((long) ctl->extra2) {
		case USIF_SPI_PRIV_FRAGMENT_SIZE:
			val = usif_cfg->fragSize;
			break;

		case USIF_SPI_PRIV_FIFO_SIZE:
			val = usif_cfg->maxFIFOSize;
			break;

		case USIF_SPI_PRIV_BAUDRATE:
			val = usif_cfg->baudrate;
			break;

		case USIF_SPI_PRIV_MODE:
			val = usif_cfg->spi_mode;
			break;

		default:
			return -EINVAL;
		}
		ret = USIF_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
	}
	return ret;
}

static const ctl_table usif_sysctl_template[] = {
	/* NB: must be last entry before NULL */
	{LANTIQ_INIT_CTL_NAME(CTL_AUTO)
	 .procname = "fragment_size",
	 .mode = 0644,
	 .proc_handler = usif_sysctl_private,
	 .extra2 = (void *) USIF_SPI_PRIV_FRAGMENT_SIZE,
	 },
	{LANTIQ_INIT_CTL_NAME(CTL_AUTO)
	 .procname = "fifosize",
	 .mode = 0644,
	 .proc_handler = usif_sysctl_private,
	 .extra2 = (void *) USIF_SPI_PRIV_FIFO_SIZE,
	 },
	{LANTIQ_INIT_CTL_NAME(CTL_AUTO)
	 .procname = "baudrate",
	 .mode = 0644,
	 .proc_handler = usif_sysctl_private,
	 .extra2 = (void *) USIF_SPI_PRIV_BAUDRATE,
	 },
	{LANTIQ_INIT_CTL_NAME(CTL_AUTO)
	 .procname = "spimode",
	 .mode = 0644,
	 .proc_handler = usif_sysctl_private,
	 .extra2 = (void *) USIF_SPI_PRIV_MODE,
	 },
	{0}
};

static void lantiq_usif_spi_sysctl_attach(lantiq_usif_device_t *dev)
{
	int i, space;

	space = 5 * sizeof(struct ctl_table) +
		sizeof(usif_sysctl_template);
	dev->usif_sysctls = kmalloc(space, GFP_KERNEL);
	if (dev->usif_sysctls == NULL) {
		printk(KERN_ERR "%s: no memory for sysctl table!\n", __func__);
		return;
	}

	/* setup the table */
	memset(dev->usif_sysctls, 0, space);
	LANTIQ_SET_CTL_NAME(dev->usif_sysctls[0], CTL_DEV);
	dev->usif_sysctls[0].procname = "dev";
	dev->usif_sysctls[0].mode = 0555;
	dev->usif_sysctls[0].child = &dev->usif_sysctls[2];
	/* [1] is NULL terminator */
	LANTIQ_SET_CTL_NAME(dev->usif_sysctls[2], CTL_AUTO);
	dev->usif_sysctls[2].procname = dev->dev_name;
	dev->usif_sysctls[2].mode = 0555;
	dev->usif_sysctls[2].child = &dev->usif_sysctls[4];
	/* [3] is NULL terminator */
	/* copy in pre-defined data */
	memcpy(&dev->usif_sysctls[4], usif_sysctl_template,
		sizeof(usif_sysctl_template));

	/* add in dynamic data references */
	for (i = 4; dev->usif_sysctls[i].procname; i++) {
		if (dev->usif_sysctls[i].extra1 == NULL)
			dev->usif_sysctls[i].extra1 = dev;

	}

	/* tack on back-pointer to parent device */
	dev->usif_sysctls[i - 1].data = dev->dev_name;

	/* and register everything */
	dev->usif_sysctl_header =
		LANTIQ_REGISTER_SYSCTL_TABLE(dev->usif_sysctls);
	if (!dev->usif_sysctl_header) {
		printk(KERN_WARNING "%s: failed to register sysctls!\n",
			dev->dev_name);
		kfree(dev->usif_sysctls);
		dev->usif_sysctls = NULL;
	}
}

static void lantiq_usif_spi_sysctl_detach(lantiq_usif_device_t *dev)
{
	if (dev->usif_sysctl_header) {
		unregister_sysctl_table(dev->usif_sysctl_header);
		dev->usif_sysctl_header = NULL;
	}
	if (dev->usif_sysctls) {
		kfree(dev->usif_sysctls);
		dev->usif_sysctls = NULL;
	}
}

/* USIF Driver itself proc support for debug and future configuration */
enum {
	USIF_SPI_PRIV_DEBUG = 1,
};

static int
USIF_SYSCTL_DECL(port_sysctl_private, ctl, write, filp, buffer, lenp, ppos)
{
	struct lantiq_usif_port *port = ctl->extra1;
	u32 val;
	int ret;

	ctl->data = &val;
	ctl->maxlen = sizeof(val);
	if (write) {
		ret = USIF_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
		if (ret == 0) {
			switch ((long) ctl->extra2) {
			case USIF_SPI_PRIV_DEBUG:
				port->usif_debug = val;
				break;

			default:
				return -EINVAL;
			}
		}
	} else {
		switch ((long) ctl->extra2) {
		case USIF_SPI_PRIV_DEBUG:
			val = port->usif_debug;
			break;

		default:
			return -EINVAL;
		}
		ret = USIF_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer,
			lenp, ppos);
	}
	return ret;
}

static const ctl_table port_sysctl_template[] = {
	/* NB: must be last entry before NULL */
	{LANTIQ_INIT_CTL_NAME(CTL_AUTO)
	 .procname = "debug",
	 .mode = 0644,
	 .proc_handler = port_sysctl_private,
	 .extra2 = (void *) USIF_SPI_PRIV_DEBUG,
	 },
	{0}
};

static void lantiq_usif_spi_port_sysctl_attach(struct lantiq_usif_port *port)
{
	int i, space;

	space = 5 * sizeof(struct ctl_table) +
		sizeof(port_sysctl_template);
	port->port_sysctls = kmalloc(space, GFP_KERNEL);
	if (port->port_sysctls == NULL) {
		printk(KERN_ERR "%s: no memory for sysctl table!\n", __func__);
		return;
	}

	/* setup the table */
	memset(port->port_sysctls, 0, space);
	LANTIQ_SET_CTL_NAME(port->port_sysctls[0], CTL_DEV);
	port->port_sysctls[0].procname = "dev";
	port->port_sysctls[0].mode = 0555;
	port->port_sysctls[0].child = &port->port_sysctls[2];
	/* [1] is NULL terminator */
	LANTIQ_SET_CTL_NAME(port->port_sysctls[2], CTL_AUTO);
	port->port_sysctls[2].procname = port->name;
	port->port_sysctls[2].mode = 0555;
	port->port_sysctls[2].child = &port->port_sysctls[4];
	/* [3] is NULL terminator */
	/* copy in pre-defined data */
	memcpy(&port->port_sysctls[4], port_sysctl_template,
		sizeof(port_sysctl_template));

	/* add in dynamic data references */
	for (i = 4; port->port_sysctls[i].procname; i++) {
		if (port->port_sysctls[i].extra1 == NULL)
			port->port_sysctls[i].extra1 = port;
	}

	/* tack on back-pointer to parent device */
	port->port_sysctls[i - 1].data = port->name;

	/* and register everything */
	port->port_sysctl_header =
		LANTIQ_REGISTER_SYSCTL_TABLE(port->port_sysctls);
	if (port->port_sysctl_header == NULL) {
		printk(KERN_WARNING "%s: failed to register sysctls!\n",
			port->name);
		kfree(port->port_sysctls);
		port->port_sysctls = NULL;
	}
}

static void lantiq_usif_spi_port_sysctl_detach(struct lantiq_usif_port *port)
{
	if (port->port_sysctl_header != NULL) {
		unregister_sysctl_table(port->port_sysctl_header);
		port->port_sysctl_header = NULL;
	}
	if (port->port_sysctls != NULL) {
		kfree(port->port_sysctls);
		port->port_sysctls = NULL;
	}
}
#endif /* CONFIG_SYSCTL */

/**
 *\fn LANTIQ_USIF_SPI_HANDLE_t lantiq_usif_spiAllocConnection
 * (char *dev_name, LANTIQ_USIF_SPI_CONFIGURE_t *connid)
 *\brief Allocate and create a Connection ID "ConnId"
 *
 * Allocate and create a Connection ID "ConnId" to communicate over USIF SPI.
 * This ConnId is needed for all remaining USIF SPI driver API calls. This
 * ConnId is a handle that helps the USIF SPI driver to find the configuration
 * that belongs to the connection. ConnId specific parameters are e.g.
 * Baudrate, Priority, Chipselect Callback, etc.
 *
 * \param   dev_name    unique name for this connection. If null, will alloc
 *                      one unique name automatically
 * \param   connid      Connectin id
 * \return  a handle "LANTIQ_USIF_SPI_HANDLE_t" in case the allocation was
 * successful. In case of an error, the return handle is zero (NULL).
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
LANTIQ_USIF_SPI_HANDLE_t
lantiq_usif_spiAllocConnection(char *dev_name, LANTIQ_USIF_SPI_CONFIGURE_t *connid)
{
	struct lantiq_usif_port *port;
	lantiq_usif_device_t *p;
	lantiq_usif_device_t *q;
	LANTIQ_USIF_QUEUE_t *queue;
	char buf[LANTIQ_USIF_SPI_MAX_DEVNAME] = { 0 };
	char *pname;

	port = &lantiq_usif_spi_port;

	if (port->usif_ndevs >= LANTIQ_USIF_SPI_MAX_DEVICE) {
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s device number out of range\n",
			__func__);
		return NULL;
	}

	if (connid == NULL) {
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s must provide connection portrmation!\n",
			__func__);
		return NULL;
	}

	if ((connid->spi_mode < LANTIQ_USIF_SPI_MODE_0)
	    || (connid->spi_mode > LANTIQ_USIF_SPI_MODE_3)) {
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s invalid spi mode <%d~%d>!\n", __func__,
			LANTIQ_USIF_SPI_MODE_0, LANTIQ_USIF_SPI_MODE_3);
		return NULL;
	}

	if (connid->spi_prio < LANTIQ_USIF_SPI_PRIO_LOW
	    || (connid->spi_prio > LANTIQ_USIF_SPI_PRIO_MAX)) {
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s invalid priority <%d~%d>!\n", __func__,
			LANTIQ_USIF_SPI_PRIO_LOW,
			LANTIQ_USIF_SPI_PRIO_MAX);
	}

	if (connid->csset_cb == NULL) {
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s must provide cs function\n", __func__);
		return NULL;
	}

	if (connid->fragSize < LANTIQ_USIF_SPI_MIN_FRAGSIZE
	    || connid->fragSize > LANTIQ_USIF_SPI_MAX_FRAGSIZE) {
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s invalid fragment size <%d~%d>!\n",
			__func__, LANTIQ_USIF_SPI_MIN_FRAGSIZE,
			LANTIQ_USIF_SPI_MAX_FRAGSIZE);
		return NULL;
	}

	if (connid->maxFIFOSize < LANTIQ_USIF_SPI_FIFO_MIN_THRESHOULD
	    || connid->maxFIFOSize > LANTIQ_USIF_SPI_FIFO_MAX_THRESHOULD) {
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s invalid fifo size <%d~%d>!\n", __func__,
			LANTIQ_USIF_SPI_FIFO_MIN_THRESHOULD,
			LANTIQ_USIF_SPI_FIFO_MAX_THRESHOULD);
		return NULL;
	}

	if (connid->duplex_mode != LANTIQ_USIF_SPI_FULL_DUPLEX
	    && connid->duplex_mode != LANTIQ_USIF_SPI_HALF_DUPLEX) {
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s %d invalid duplex mode <%d~%d>!\n",
			__func__, connid->duplex_mode,
			LANTIQ_USIF_SPI_FULL_DUPLEX,
			LANTIQ_USIF_SPI_HALF_DUPLEX);
		return NULL;
	}

	/* If no name specified, will assign one name for identification */
	if (dev_name == NULL) {
		sprintf(buf, "usif%d", port->usif_ndevs);
		pname = buf;
	} else {
		if (strlen(dev_name) > (LANTIQ_USIF_SPI_MAX_DEVNAME - 1)) {
			LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
				"%s device name is too long\n", __func__);
			return NULL;
		}
		pname = dev_name;
	}

	p = kmalloc(sizeof(lantiq_usif_device_t), GFP_KERNEL);
	if (p == NULL) {
		LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
			"%s failed to allocate memory\n", __func__);
		return NULL;
	}
	memset(p, 0, sizeof(lantiq_usif_device_t));

	LANTIQ_USIF_SPI_SEM_LOCK(port->dev_sem);
	TAILQ_FOREACH(q, &port->usif_devq, dev_entry) {
		if (strcmp(q->dev_name, pname) == 0) {
			kfree(p);
			LANTIQ_USIF_SPI_SEM_UNLOCK(port->dev_sem);
			LANTIQ_USIF_PRINT(port, USIF_MSG_ERROR,
				"%s device registered already!\n", __func__);
			return NULL;
		}
	}
	LANTIQ_USIF_SPI_SEM_UNLOCK(port->dev_sem);

	/* Follow net device driver name rule */
	memcpy(p->dev_name, pname, LANTIQ_USIF_SPI_MAX_DEVNAME);
	p->duplex = connid->duplex_mode;
	memcpy((char *) &p->conn_id, (char *) connid,
		sizeof(LANTIQ_USIF_SPI_CONFIGURE_t));

	queue = &p->queue;
	/* Queue handler type converted from priority */
	if (connid->spi_prio == LANTIQ_USIF_SPI_PRIO_ASYNC)
		queue->handle_type = LANTIQ_USIF_SPI_HANDL_TYPE_ASYNC;
	else
		queue->handle_type = LANTIQ_USIF_SPI_HANDL_TYPE_SYNC;

	/* Back pointer to later usage */
	queue->dev = p;
	atomic_set(&queue->isqueued, 0);
	queue->request_lock = false;
	/*
	 * Just for fast access, priority based on device, instead of packet
	 * Still keep per packet priority there for future change.
	 */
	p->dev_prio = connid->spi_prio;

	LANTIQ_USIF_SPI_WAKELIST_INIT(p->dev_thread_wait);

	p->port = port; /* back pointer to port for easy reference later */
	port->usif_ndevs++;
#ifdef CONFIG_SYSCTL
	lantiq_usif_spi_sysctl_attach(p);
#endif /* CONFIG_SYSCTL */
	LANTIQ_USIF_SPI_SEM_LOCK(port->dev_sem);
	TAILQ_INSERT_TAIL(&port->usif_devq, p, dev_entry);
	LANTIQ_USIF_SPI_SEM_UNLOCK(port->dev_sem);

	LANTIQ_USIF_PRINT(port, USIF_MSG_INIT,
		"%s: device %s register sucessfully!\n", __func__,
		p->dev_name);
	return (LANTIQ_USIF_SPI_HANDLE_t) p;
}
EXPORT_SYMBOL(lantiq_usif_spiAllocConnection);

/*!
 *\fn int lantiq_usif_spiFreeConnection(LANTIQ_USIF_SPI_HANDLE_t handler)
 *\brief Release usif spi connnection
 *
 * Release a ConnId handle that was allocated by the function
 * lantiq_usif_spiAllocConnection before. An allocated ConnId has
 * to be released by the client driver module when the USIF SPI
 * driver is not used anymore. Note that all allocated ConnId's should
 * be released before the USIF SPI driver is unloaded from the kernel.
 *
 * \param   handler    ConnId handle allocated by lantiq_usif_spiAllocConnection
 * \returns (0) in case of success, otherwise (-1) in case of errors.
 * \ingroup LANTIQ_USIF_SPI_FUNCTIONS
 */
int lantiq_usif_spiFreeConnection(LANTIQ_USIF_SPI_HANDLE_t handler)
{
	lantiq_usif_device_t *p;
	struct lantiq_usif_port *port;
	lantiq_usif_device_t *q, *next;

	LANTIQ_KASSERT((handler != NULL),
		("%s Invalid parameter\n", __func__));

	p = (lantiq_usif_device_t *) handler;
	port = p->port;
	LANTIQ_USIF_SPI_SEM_LOCK(port->dev_sem);
	TAILQ_FOREACH_SAFE(q, &port->usif_devq, dev_entry, next) {
		if (strcmp(q->dev_name, p->dev_name) == 0) {
			TAILQ_REMOVE(&port->usif_devq, q, dev_entry);
		#ifdef CONFIG_SYSCTL
			lantiq_usif_spi_sysctl_detach(q);
		#endif /* CONFIG_SYSCTL */
			kfree(q);
			port->usif_ndevs--;
			LANTIQ_USIF_SPI_SEM_UNLOCK(port->dev_sem);
			LANTIQ_USIF_PRINT(port, USIF_MSG_INIT,
				"%s device %s unregistered\n",
				__func__, p->dev_name);
			return 0;
		}
	}
	LANTIQ_USIF_SPI_SEM_UNLOCK(port->dev_sem);
	return -1;
}
EXPORT_SYMBOL(lantiq_usif_spiFreeConnection);

/**
 * \fn static int __init lantiq_usif_spi_init (void)
 * \brief USIF-SPI module Initialization.
 *
 * \return -ENOMEM  Failed to allocate memory
 *         -EBUSY   Failed to iomap register space
 *         0        OK
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static int __init lantiq_usif_spi_init(void)
{
	struct lantiq_usif_port *port;
	int i;
	int ret_val = -ENOMEM;
	char ver_str[128] = { 0 };
	static int lantiq_usif_spi_initialized;

	if (lantiq_usif_spi_initialized == 1)
		return 0;
	else
		lantiq_usif_spi_initialized = 1;

	memset(&lantiq_usif_spi_port, 0, sizeof(struct lantiq_usif_port));

	/* Set default values in lantiq_usif_port */
	port = &lantiq_usif_spi_port;

	port->port_idx = 0;

	/* default values for the HwOpts */
	port->opts.sync = LANTIQ_USIF_SPI_DEF_SYNC;
	port->opts.master = LANTIQ_USIF_SPI_DEF_MASTERSLAVE;
	port->opts.frmctl = LANTIQ_USIF_SPI_DEF_FRMCTL;
	port->opts.loopBack = LANTIQ_USIF_SPI_DEF_LOOP_BACK;
	port->opts.echoMode = LANTIQ_USIF_SPI_DEF_ECHO_MODE;
	port->opts.txInvert = LANTIQ_USIF_SPI_DEF_TX_INVERT;
	port->opts.rxInvert = LANTIQ_USIF_SPI_DEF_RX_INVERT;
	port->opts.shiftClk = LANTIQ_USIF_SPI_DEF_SHIFT_CLK;
	port->opts.clkPO = LANTIQ_USIF_SPI_DEF_CLK_PO;
	port->opts.clkPH = LANTIQ_USIF_SPI_DEF_CLK_PH;
	port->opts.txIdleValue = LANTIQ_USIF_SPI_DEF_TX_IDLE_STATE;
	port->opts.modeRxTx = LANTIQ_USIF_SPI_DEF_MODE_RXTX;

	port->opts.charLen = LANTIQ_USIF_SPI_DEF_CHAR_LEN;
	port->opts.hdrCtrl = LANTIQ_USIF_SPI_DEF_HDR_CTRL;
	port->opts.wakeUp = LANTIQ_USIF_SPI_DEF_WAKEUP;

	port->baudrate = LANTIQ_USIF_SPI_DEF_BAUDRATE;
	port->prev_baudrate = 0;
	port->prev_spi_mode = LANTIQ_USIF_SPI_MODE_UNKNOWN;
	port->usif_ndevs = 0;
	port->usif_frag_size = DEFAULT_USIF_SPI_FRAGMENT_SIZE;

	/* Values specific to USIF SPI */
	port->mapbase = LANTIQ_USIF_PHY_BASE;
	port->membase = ioremap_nocache(port->mapbase, LANTIQ_USIF_SIZE);
	if (!port->membase) {
		printk(KERN_ERR "%s: Failed during io remap\n", __func__);
		ret_val = -EBUSY;
		goto errout1;
	}
	LANTIQ_USIF_PRINT(port, USIF_MSG_INIT,
		"%s: mapbase 0x%08lx membase 0x%08x\n", __func__,
		port->mapbase, (u32) port->membase);

	/* Register with DMA engine */
	port->dma_dev = dma_device_reserve("USIF");
	if (port->dma_dev == NULL) {
		printk(KERN_ERR "%s: Failed to reserve dma device!\n",
			__func__);
		goto errout2;
	}

	lantiq_usif_spi_init_dma_device(port->dma_dev);
	strcpy(port->name, LANTIQ_USIF_SPI_NAME);
	port->usif_cs_locked = false;
	port->lock_qentry = NULL;
	port->serve_qentry = NULL;
#ifdef CONFIG_LANTIQ_USIF_SPI_DEBUG
	port->usif_debug = USIF_MSG_INIT | USIF_MSG_ERROR; 
#else
	port->usif_debug = 0;
#endif /* CONFIG_LANTIQ_USIF_SPI_DEBUG */

	atomic_set(&port->dma_wait_state, 0);
	port->dma_is_in_half_duplex = 1;
	/* Either USIF tasklet or USIF kernel thread support, not both */
#ifdef CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS
	lantiq_usif_spi_tasklet_init(port);
#else
	if (lantiq_usif_spi_thread_init(port)) {
		printk(KERN_ERR "%s: unable to start usif_kthread\n",
			__func__);
		goto errout3;
	}
#endif /* CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS */

	/* The following buffer allocation for HW WAR, last 1~3 bytes in DMA
	 * It will make sure buffer will align on dma burst length
	 */
	port->dma_orig_txbuf = kmalloc(DEFAULT_USIF_SPI_FRAGMENT_SIZE
		+ ((port->dma_dev->tx_burst_len << 2) - 1), GFP_KERNEL);
	if (port->dma_orig_txbuf == NULL) {
		printk(KERN_ERR "%s: no memory for dma_orig_txbuf\n",
			__func__);
		goto errout4;
	}
	port->dma_txbuf = (char *) (((u32) (port->dma_orig_txbuf +
		((port->dma_dev->tx_burst_len << 2) - 1)))
		& ~((port->dma_dev->tx_burst_len << 2) - 1));


	port->dma_orig_rxbuf = kmalloc(DEFAULT_USIF_SPI_FRAGMENT_SIZE
		+ ((port->dma_dev->rx_burst_len << 2) - 1), GFP_KERNEL);
	if (port->dma_orig_rxbuf == NULL) {
		printk(KERN_ERR "%s: no memory for dma_orig_rxbuf\n",
			__func__);
		goto errout5;
	}
	port->dma_rxbuf = (char *) (((u32) (port->dma_orig_rxbuf +
		((port->dma_dev->rx_burst_len << 2) - 1)))
		& ~((port->dma_dev->rx_burst_len << 2) - 1));

	TAILQ_INIT(&port->usif_devq);
	TAILQ_INIT(&port->usif_asyncq);

	for (i = 0; i < LANTIQ_USIF_SPI_PRIO_MAX; i++)
		TAILQ_INIT(&port->usif_syncq[i]);

	LANTIQ_USIF_SPI_Q_LOCK_INIT(port);
	LANTIQ_USIF_SPI_SEM_INIT(port->dev_sem);
	LANTIQ_USIF_SPI_WAKELIST_INIT(port->usif_thread_wait);
	LANTIQ_USIF_SPI_IRQ_LOCK_INIT(port, "lantiq_usif_spi_lock");

	if (lantiq_usif_spi_hwinit(port) < 0) {
		printk(KERN_ERR "%s: hardware init failed for USIF SPI\n",
			__func__);
		goto errout6;
	}
#ifdef CONFIG_SYSCTL
	lantiq_usif_spi_port_sysctl_attach(port);
#endif /* CONFIG_SYSCTL */

#ifdef CONFIG_PROC_FS
	lantiq_usif_spi_proc_create();
#endif
	lantiq_usif_spi_drv_ver(ver_str);
	printk(KERN_INFO "%s", ver_str);
	return 0;

errout6:
	lantiq_usif_spi_gpio_release();
	kfree(port->dma_orig_rxbuf);
errout5:
	kfree(port->dma_orig_txbuf);
errout4:
	kthread_stop(port->usif_tsk);
#ifndef CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS
errout3:
	dma_device_unregister(port->dma_dev);
	dma_device_release(port->dma_dev);
#endif
errout2:
	iounmap(port->membase);
errout1:
	return ret_val;
}

/**
 * \fn static void __exit lantiq_usif_spi_exit (void)
 * \brief USIF SPI Module Cleanup.
 *
 * Upon removal of the USIF-SPI module this function will free all allocated
 * resources and unregister devices.
 * \return none
 * \ingroup LANTIQ_USIF_SPI_INTERNAL
 */
static void __exit lantiq_usif_spi_exit(void)
{
	struct dma_device_info *dma_dev;
	struct lantiq_usif_port *port = &lantiq_usif_spi_port;

	/* Exit running mode */
	lantiq_usif_spi_clc_cfg_mode(port);

	LANTIQ_USIF_SPI_SEM_LOCK(port->dev_sem);
	if (!TAILQ_EMPTY(&port->usif_devq)) {
		LANTIQ_USIF_SPI_SEM_UNLOCK(port->dev_sem);
		printk(KERN_ERR "%s USIF devices still attached,"
			" please release them first\n", __func__);
		return;
	}
	LANTIQ_USIF_SPI_SEM_UNLOCK(port->dev_sem);

#ifdef CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS
	tasklet_kill(&port->usif_txrxq);
#else
	if (port->usif_tsk) {
		kthread_stop(port->usif_tsk);
		port->usif_tsk = NULL;
	}
#endif /* CONFIG_LANTIQ_USIF_SPI_ASYNCHRONOUS */
	LANTIQ_USIF_SPI_IRQ_LOCK_DESTROY(port);
	LANTIQ_USIF_SPI_Q_LOCK_DESTROY(port);
	dma_dev = port->dma_dev;
	if (dma_dev != NULL) {
		dma_device_unregister(dma_dev);
		dma_device_release(dma_dev);
	}
#ifdef CONFIG_SYSCTL
	lantiq_usif_spi_port_sysctl_detach(port);
#endif /* CONFIG_SYSCTL */

	kfree(port->dma_orig_rxbuf);
	kfree(port->dma_orig_txbuf);
#ifdef CONFIG_PROC_FS
	lantiq_usif_spi_proc_delete();
#endif
	iounmap(port->membase);
	lantiq_usif_spi_pm_disable();
	lantiq_usif_spi_gpio_release();
}

module_init(lantiq_usif_spi_init);
module_exit(lantiq_usif_spi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Lei Chuanhua, Chuanhua.Lei@infineon.com");
MODULE_DESCRIPTION("IFX USIF SPI driver for VRX200 reference board");
MODULE_SUPPORTED_DEVICE("VRX200 builtin USIF-SPI module");

