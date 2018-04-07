/******************************************************************************
**
** FILE NAME    : ifxmips_ptm_vdsl.c
** PROJECT      : UEIP
** MODULES      : PTM
**
** DATE         : 7 Jul 2009
** AUTHOR       : Xu Liang
** DESCRIPTION  : PTM driver common source file (core functions for VR9)
** COPYRIGHT    :       Copyright (c) 2006
**                      Infineon Technologies AG
**                      Am Campeon 1-12, 85579 Neubiberg, Germany
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date        $Author         $Comment
** 07 JUL 2009  Xu Liang        Init Version
*******************************************************************************/



/*
 * ####################################
 *              Head File
 * ####################################
 */

/*
 *  Common Head File
 */
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/etherdevice.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <lantiq_soc.h>

/*
 *  Chip Specific Head File
 */
#include "ltqmips_ptm_vdsl.h"


/*
 * ####################################
 *        Kernel Version Adaption
 * ####################################
 */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,11)
  #define MODULE_PARM_ARRAY(a, b)   module_param_array(a, int, NULL, 0)
  #define MODULE_PARM(a, b)         module_param(a, int, 0)
#else
  #define MODULE_PARM_ARRAY(a, b)   MODULE_PARM(a, b)
#endif



/*
 * ####################################
 *   Parameters to Configure PPE
 * ####################################
 */

static int wanqos_en = 0;

static int queue_gamma_map[4] = {0xFE, 0x01, 0x00, 0x00};

MODULE_PARM(wanqos_en, "i");
MODULE_PARM_DESC(wanqos_en, "WAN QoS support, 1 - enabled, 0 - disabled.");

MODULE_PARM_ARRAY(queue_gamma_map, "4-4i");
MODULE_PARM_DESC(queue_gamma_map, "TX QoS queues mapping to 4 TX Gamma interfaces.");



/*
 * ####################################
 *              Definition
 * ####################################
 */

#define ENABLE_TMP_DBG                          0

#define ENABLE_LED_FRAMEWORK                    0

#define DUMP_SKB_LEN                            ~0



/*
 * ####################################
 *             Declaration
 * ####################################
 */

/*
 *  Network Operations
 */
static void ptm_setup(struct net_device *, int);
static struct net_device_stats *ptm_get_stats(struct net_device *);
static int ptm_open(struct net_device *);
static int ptm_stop(struct net_device *);
#ifdef CONFIG_LTQ_PTM_RX_NAPI
  static unsigned int ptm_poll(int, unsigned int);
 #if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
  static int ptm_napi_poll(struct net_device *, int *);
 #else
  static int ptm_napi_poll(struct napi_struct *, int);
 #endif
#endif
static int ptm_hard_start_xmit(struct sk_buff *, struct net_device *);
static int ptm_ioctl(struct net_device *, struct ifreq *, int);
static void ptm_tx_timeout(struct net_device *);

/*
 *  DSL Data LED
 */
static INLINE void adsl_led_flash(void);

/*
 *  buffer manage functions
 */
static INLINE struct sk_buff* alloc_skb_rx(void);
static INLINE struct sk_buff* alloc_skb_tx(unsigned int);
static INLINE struct sk_buff *get_skb_pointer(unsigned int);
static INLINE int get_tx_desc(unsigned int, unsigned int *);

/*
 *  Mailbox handler and signal function
 */
static irqreturn_t mailbox_irq_handler(int, void *);

/*
 *  Tasklet to Handle Swap Descriptors
 */
static void do_swap_desc_tasklet(unsigned long);

/*
 *  Debug Functions
 */
#if defined(DEBUG_DUMP_SKB) && DEBUG_DUMP_SKB
  static void dump_skb(struct sk_buff *, u32, char *, int, int, int);
#else
  #define dump_skb(skb, len, title, port, ch, is_tx)    do {} while (0)
#endif
#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
  static void skb_swap(struct sk_buff *);
#else
  #define skb_swap(skb)                                 do {} while (0)
#endif

/*
 *  Proc File Functions
 */
static INLINE void proc_file_create(void);
static INLINE void proc_file_delete(void);
static int proc_read_version(char *, char **, off_t, int, int *, void *);
static int proc_read_wanmib(char *, char **, off_t, int, int *, void *);
static int proc_write_wanmib(struct file *, const char *, unsigned long, void *);
static int proc_read_prio(char *, char **, off_t, int, int *, void *);
static int proc_write_prio(struct file *, const char *, unsigned long, void *);
static int proc_read_qos(char *, char **, off_t, int, int *, void *);
static int proc_read_gamma(char *, char **, off_t, int, int *, void *);
#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
  static int proc_read_dbg(char *, char **, off_t, int, int *, void *);
  static int proc_write_dbg(struct file *, const char *, unsigned long, void *);
#endif
#if defined(ENABLE_TMP_DBG) && ENABLE_TMP_DBG
  static int proc_read_rdptr(char *, char **, off_t, int, int *, void *);
#endif

/*
 *  Proc Help Functions
 */
static INLINE int stricmp(const char *, const char *);
#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
  static INLINE int strincmp(const char *, const char *, int);
#endif
static INLINE int get_token(char **, char **, int *, int *);
static INLINE int get_number(char **, int *, int);
static INLINE void ignore_space(char **, int *);
static INLINE int ifx_ptm_version(char *);

/*
 *  Init & clean-up functions
 */
static INLINE int init_priv_data(void);
static INLINE void clear_priv_data(void);
static INLINE int init_tables(void);
static INLINE void clear_tables(void);

/*
 *  Exteranl Function
 */
#if defined(CONFIG_IFXMIPS_DSL_CPE_MEI) || defined(CONFIG_IFXMIPS_DSL_CPE_MEI_MODULE)
 #if !defined(ENABLE_LED_FRAMEWORK) || !ENABLE_LED_FRAMEWORK
  extern int ifx_mei_atm_led_blink(void);
 #endif
  extern int ifx_mei_atm_showtime_check(int *is_showtime, struct port_cell_info *port_cell, void **xdata_addr);
#else
 #if !defined(ENABLE_LED_FRAMEWORK) || !ENABLE_LED_FRAMEWORK
  static inline int ifx_mei_atm_led_blink(void) { return 0; }
 #endif
  static inline int ifx_mei_atm_showtime_check(int *is_showtime, struct port_cell_info *port_cell, void **xdata_addr)
  {
    if ( is_showtime != NULL )
        *is_showtime = 0;
    return 0;
  }
#endif

/*
 *  External variable
 */
#if defined(CONFIG_IFXMIPS_DSL_CPE_MEI) || defined(CONFIG_IFXMIPS_DSL_CPE_MEI_MODULE)
  extern int (*ifx_mei_atm_showtime_enter)(struct port_cell_info *, void *);
  extern int (*ifx_mei_atm_showtime_exit)(void);
#else
  int (*ifx_mei_atm_showtime_enter)(struct port_cell_info *, void *) = NULL;
  EXPORT_SYMBOL(ifx_mei_atm_showtime_enter);
  int (*ifx_mei_atm_showtime_exit)(void) = NULL;
  EXPORT_SYMBOL(ifx_mei_atm_showtime_exit);
#endif



/*
 * ####################################
 *            Local Variable
 * ####################################
 */

static int g_wanqos_en = 0;

static int g_queue_gamma_map[4];

static struct ptm_priv_data g_ptm_priv_data;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32)
static struct net_device_ops g_ptm_netdev_ops = {
    .ndo_get_stats       = ptm_get_stats,
    .ndo_open            = ptm_open,
    .ndo_stop            = ptm_stop,
    .ndo_start_xmit      = ptm_hard_start_xmit,
    .ndo_validate_addr   = eth_validate_addr,
    .ndo_set_mac_address = eth_mac_addr,
    .ndo_change_mtu      = eth_change_mtu,
    .ndo_do_ioctl        = ptm_ioctl,
    .ndo_tx_timeout      = ptm_tx_timeout,
};
#endif

static struct net_device *g_net_dev[1] = {0};
static char *g_net_dev_name[1] = {"ptm0"};

static int g_ptm_prio_queue_map[8];

static DECLARE_TASKLET(g_swap_desc_tasklet, do_swap_desc_tasklet, 0);

static int g_showtime = 0;
static void *g_xdata_addr = NULL;

#if defined(ENABLE_LED_FRAMEWORK) && ENABLE_LED_FRAMEWORK
  static void *g_data_led_trigger = NULL;
#endif

unsigned int ifx_ptm_dbg_enable = DBG_ENABLE_MASK_ERR;

static struct proc_dir_entry* g_ptm_dir = NULL;

static unsigned int g_wrx_total_pdu[4] = {0};
static unsigned int g_wrx_err_pdu[4] = {0};



/*
 * ####################################
 *            Local Function
 * ####################################
 */

static void ptm_setup(struct net_device *dev, int ndev)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
    /*  hook network operations */
    dev->get_stats       = ptm_get_stats;
    dev->open            = ptm_open;
    dev->stop            = ptm_stop;
  #ifdef CONFIG_LTQ_PTM_RX_NAPI
    dev->poll            = ptm_napi_poll;
    dev->weight          = 16;
  #endif
    dev->hard_start_xmit = ptm_hard_start_xmit;
    dev->do_ioctl        = ptm_ioctl;
    dev->tx_timeout      = ptm_tx_timeout;
#else
    dev->netdev_ops      = &g_ptm_netdev_ops;
  #ifdef CONFIG_LTQ_PTM_RX_NAPI
    netif_napi_add(dev, &g_ptm_priv_data.itf[ndev].napi, ptm_napi_poll, 16);
  #endif
#endif
    dev->watchdog_timeo  = ETH_WATCHDOG_TIMEOUT;

    dev->dev_addr[0] = 0x00;
    dev->dev_addr[1] = 0x20;
	dev->dev_addr[2] = 0xda;
	dev->dev_addr[3] = 0x86;
	dev->dev_addr[4] = 0x23;
	dev->dev_addr[5] = 0x75 + ndev;
}

static struct net_device_stats *ptm_get_stats(struct net_device *dev)
{
    if ( dev != g_net_dev[0] )
        return NULL;

    return &g_ptm_priv_data.itf[0].stats;
}

static int ptm_open(struct net_device *dev)
{
    ASSERT(dev == g_net_dev[0], "incorrect device");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32) && defined(CONFIG_LTQ_PTM_RX_NAPI)
    napi_enable(&g_ptm_priv_data.itf[0].napi);
#endif

    IFX_REG_W32_MASK(0, 1, MBOX_IGU1_IER);

    netif_start_queue(dev);

    return 0;
}

static int ptm_stop(struct net_device *dev)
{
    ASSERT(dev == g_net_dev[0], "incorrect device");

    IFX_REG_W32_MASK(1 | (1 << 17), 0, MBOX_IGU1_IER);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,32) && defined(CONFIG_LTQ_PTM_RX_NAPI)
    napi_disable(&g_ptm_priv_data.itf[0].napi);
#endif

    netif_stop_queue(dev);

    return 0;
}

#ifdef CONFIG_LTQ_PTM_RX_NAPI
static unsigned int ptm_poll(int ndev, unsigned int work_to_do)
{
    unsigned int work_done = 0;
    volatile struct rx_descriptor *desc;
    struct rx_descriptor reg_desc;
    struct sk_buff *skb, *new_skb;

    ASSERT(ndev >= 0 && ndev < NUM_ENTITY(g_net_dev), "ndev = %d (wrong value)", ndev);

    while ( work_done < work_to_do ) {
        desc = &WAN_RX_DESC_BASE[g_ptm_priv_data.itf[0].rx_desc_pos];
        if ( desc->own /* || !desc->c */ )  //  if PP32 hold descriptor or descriptor not completed
            break;
        if ( ++g_ptm_priv_data.itf[0].rx_desc_pos == WAN_RX_DESC_NUM )
            g_ptm_priv_data.itf[0].rx_desc_pos = 0;

        reg_desc = *desc;
        skb = get_skb_pointer(reg_desc.dataptr);
        ASSERT(skb != NULL, "invalid pointer skb == NULL");

        new_skb = alloc_skb_rx();
        if ( new_skb != NULL ) {
            ASSERT(reg_desc.byteoff == RX_HEAD_MAC_ADDR_ALIGNMENT, "invalid byteoff (%d) != RX_HEAD_MAC_ADDR_ALIGNMENT (%d)", reg_desc.byteoff, RX_HEAD_MAC_ADDR_ALIGNMENT);
            skb_reserve(skb, reg_desc.byteoff);
            skb_put(skb, reg_desc.datalen);

            dump_skb(skb, DUMP_SKB_LEN, (char *)__func__, 0, 0, 0);

            //  parse protocol header
            skb->dev = g_net_dev[0];
            skb->protocol = eth_type_trans(skb, skb->dev);

            g_net_dev[0]->last_rx = jiffies;

            netif_receive_skb(skb);

            g_ptm_priv_data.itf[0].stats.rx_packets++;
            g_ptm_priv_data.itf[0].stats.rx_bytes += reg_desc.datalen;

            reg_desc.dataptr = (unsigned int)new_skb->data & 0x0FFFFFFF;
            reg_desc.byteoff = RX_HEAD_MAC_ADDR_ALIGNMENT;
        }

        reg_desc.datalen = RX_MAX_BUFFER_SIZE - RX_HEAD_MAC_ADDR_ALIGNMENT;
        reg_desc.own     = 1;
        reg_desc.c       = 0;

        /*  write discriptor to memory  */
        *((volatile unsigned int *)desc + 1) = *((unsigned int *)&reg_desc + 1);
        wmb();
        *(volatile unsigned int *)desc = *(unsigned int *)&reg_desc;

        adsl_led_flash();

        work_done++;
    }

    return work_done;
}
  #if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
static int ptm_napi_poll(struct net_device *dev, int *quota)
{
    int ndev = 0;
    unsigned int work_to_do = min(dev->quota, *quota);
    unsigned int work_done;

    ASSERT(dev == g_net_dev[0], "incorrect device");

    work_done = ptm_poll(ndev, work_to_do);

    *quota -= work_done;

    //  interface down
    if ( !netif_running(dev) ) {
        netif_rx_complete(dev);
        return 0;
    }

    //  clear interrupt
    IFX_REG_W32_MASK(0, 1, MBOX_IGU1_ISRC);
    //  no more traffic
    if ( WAN_RX_DESC_BASE[g_ptm_priv_data.itf[0].rx_desc_pos].own ) {   //  if PP32 hold descriptor
        netif_rx_complete(dev);
        IFX_REG_W32_MASK(0, 1, MBOX_IGU1_IER);
        return 0;
    }

    //  next round
    dev->quota -= work_done;
    return 1;
}
  #else
static int ptm_napi_poll(struct napi_struct *napi, int budget)
{
    int ndev = 0;
    unsigned int work_done;

    work_done = ptm_poll(ndev, budget);

    //  interface down
    if ( !netif_running(napi->dev) ) {
        napi_complete(napi);
        return work_done;
    }

    //  clear interrupt
    IFX_REG_W32_MASK(0, 1, MBOX_IGU1_ISRC);
    //  no more traffic
    if ( WAN_RX_DESC_BASE[g_ptm_priv_data.itf[0].rx_desc_pos].own ) {   //  if PP32 hold descriptor
        napi_complete(napi);
        IFX_REG_W32_MASK(0, 1, MBOX_IGU1_IER);
        return work_done;
    }

    //  next round
    return work_done;
}
  #endif
#endif

static int ptm_hard_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
    int qid;
    unsigned int f_full;
    int desc_base;
    volatile struct tx_descriptor *desc;
    struct tx_descriptor reg_desc = {0};
    struct sk_buff *skb_to_free;
    unsigned int byteoff;

    ASSERT(dev == g_net_dev[0], "incorrect device");

    if ( !g_showtime ) {
        err("not in showtime");
        goto PTM_HARD_START_XMIT_FAIL;
    }

    /*  allocate descriptor */
    desc_base = get_tx_desc(0, &f_full);
    if ( f_full ) {
        dev->trans_start = jiffies;
        netif_stop_queue(dev);

        IFX_REG_W32_MASK(0, 1 << 17, MBOX_IGU1_ISRC);
        IFX_REG_W32_MASK(0, 1 << 17, MBOX_IGU1_IER);
    }
    if ( desc_base < 0 )
        goto PTM_HARD_START_XMIT_FAIL;
    desc = &CPU_TO_WAN_TX_DESC_BASE[desc_base];

    if ( skb->cb[13] == 0x5A )  //  magic number indicating forcing QId (e.g. directpath)
        qid = skb->cb[15];
    else
        qid = g_ptm_prio_queue_map[skb->priority > 7 ? 7 : skb->priority];

    byteoff = (unsigned int)skb->data & (DATA_BUFFER_ALIGNMENT - 1);
    if ( skb_headroom(skb) < sizeof(struct sk_buff *) + byteoff || skb_cloned(skb) ) {
        struct sk_buff *new_skb;

        ASSERT(skb_headroom(skb) >= sizeof(struct sk_buff *) + byteoff, "skb_headroom(skb) < sizeof(struct sk_buff *) + byteoff");
        ASSERT(!skb_cloned(skb), "skb is cloned");

        new_skb = alloc_skb_tx(skb->len);
        if ( new_skb == NULL ) {
            dbg("no memory");
            goto ALLOC_SKB_TX_FAIL;
        }
        skb_put(new_skb, skb->len);
        memcpy(new_skb->data, skb->data, skb->len);
        dev_kfree_skb_any(skb);
        skb = new_skb;
        byteoff = (unsigned int)skb->data & (DATA_BUFFER_ALIGNMENT - 1);
        /*  write back to physical memory   */
        dma_cache_sync(NULL, skb->data, skb->len, DMA_TO_DEVICE);
    }

    *(struct sk_buff **)((unsigned int)skb->data - byteoff - sizeof(struct sk_buff *)) = skb;
    /*  write back to physical memory   */
    dma_cache_sync(NULL, (void *)((unsigned long)skb->data - byteoff - sizeof(struct sk_buff *)), skb->len + byteoff + sizeof(struct sk_buff *), DMA_TO_DEVICE);

    /*  free previous skb   */
    skb_to_free = get_skb_pointer(desc->dataptr);
    if ( skb_to_free != NULL )
        dev_kfree_skb_any(skb_to_free);

    /*  update descriptor   */
    reg_desc.small   = 0;
    reg_desc.dataptr = (unsigned int)skb->data & (0x0FFFFFFF ^ (DATA_BUFFER_ALIGNMENT - 1));
    reg_desc.datalen = skb->len < ETH_ZLEN ? ETH_ZLEN : skb->len;
    reg_desc.qid     = qid;
    reg_desc.byteoff = byteoff;
    reg_desc.own     = 1;
    reg_desc.c       = 1;
    reg_desc.sop = reg_desc.eop = 1;

    /*  update MIB  */
    g_ptm_priv_data.itf[0].stats.tx_packets++;
    g_ptm_priv_data.itf[0].stats.tx_bytes += reg_desc.datalen;

    dump_skb(skb, DUMP_SKB_LEN, (char *)__func__, 0, reg_desc.qid, 1);

    if ( (ifx_ptm_dbg_enable & DBG_ENABLE_MASK_MAC_SWAP) ) {
        skb_swap(skb);
    }

    /*  write discriptor to memory  */
    *((volatile unsigned int *)desc + 1) = *((unsigned int *)&reg_desc + 1);
    wmb();
    *(volatile unsigned int *)desc = *(unsigned int *)&reg_desc;

    dev->trans_start = jiffies;

    adsl_led_flash();

    return 0;

ALLOC_SKB_TX_FAIL:
PTM_HARD_START_XMIT_FAIL:
    dev_kfree_skb_any(skb);
    g_ptm_priv_data.itf[0].stats.tx_dropped++;
    return 0;
}

static int ptm_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
    ASSERT(dev == g_net_dev[0], "incorrect device");

    switch ( cmd )
    {
    case IFX_PTM_MIB_CW_GET:
        ((PTM_CW_IF_ENTRY_T *)ifr->ifr_data)->ifRxNoIdleCodewords   = IFX_REG_R32(DREG_AR_CELL0) + IFX_REG_R32(DREG_AR_CELL1);
        ((PTM_CW_IF_ENTRY_T *)ifr->ifr_data)->ifRxIdleCodewords     = IFX_REG_R32(DREG_AR_IDLE_CNT0) + IFX_REG_R32(DREG_AR_IDLE_CNT1);
        ((PTM_CW_IF_ENTRY_T *)ifr->ifr_data)->ifRxCodingViolation   = IFX_REG_R32(DREG_AR_CVN_CNT0) + IFX_REG_R32(DREG_AR_CVN_CNT1) + IFX_REG_R32(DREG_AR_CVNP_CNT0) + IFX_REG_R32(DREG_AR_CVNP_CNT1);
        ((PTM_CW_IF_ENTRY_T *)ifr->ifr_data)->ifTxNoIdleCodewords   = IFX_REG_R32(DREG_AT_CELL0) + IFX_REG_R32(DREG_AT_CELL1);
        ((PTM_CW_IF_ENTRY_T *)ifr->ifr_data)->ifTxIdleCodewords     = IFX_REG_R32(DREG_AT_IDLE_CNT0) + IFX_REG_R32(DREG_AT_IDLE_CNT1);
        break;
    case IFX_PTM_MIB_FRAME_GET:
        {
            PTM_FRAME_MIB_T data = {0};
            int i;

            data.RxCorrect = IFX_REG_R32(DREG_AR_HEC_CNT0) + IFX_REG_R32(DREG_AR_HEC_CNT1) + IFX_REG_R32(DREG_AR_AIIDLE_CNT0) + IFX_REG_R32(DREG_AR_AIIDLE_CNT1);
            for ( i = 0; i < 4; i++ )
                data.RxDropped += WAN_RX_MIB_TABLE(i)->wrx_dropdes_pdu;
            for ( i = 0; i < 8; i++ )
                data.TxSend    += WAN_TX_MIB_TABLE(i)->wtx_total_pdu;

            *((PTM_FRAME_MIB_T *)ifr->ifr_data) = data;
        }
        break;
    case IFX_PTM_CFG_GET:
        //  use bear channel 0 preemption gamma interface settings
        ((IFX_PTM_CFG_T *)ifr->ifr_data)->RxEthCrcPresent = 1;
        ((IFX_PTM_CFG_T *)ifr->ifr_data)->RxEthCrcCheck   = RX_GAMMA_ITF_CFG(0)->rx_eth_fcs_ver_dis == 0 ? 1 : 0;
        ((IFX_PTM_CFG_T *)ifr->ifr_data)->RxTcCrcCheck    = RX_GAMMA_ITF_CFG(0)->rx_tc_crc_ver_dis == 0 ? 1 : 0;;
        ((IFX_PTM_CFG_T *)ifr->ifr_data)->RxTcCrcLen      = RX_GAMMA_ITF_CFG(0)->rx_tc_crc_size == 0 ? 0 : (RX_GAMMA_ITF_CFG(0)->rx_tc_crc_size * 16);
        ((IFX_PTM_CFG_T *)ifr->ifr_data)->TxEthCrcGen     = TX_GAMMA_ITF_CFG(0)->tx_eth_fcs_gen_dis == 0 ? 1 : 0;
        ((IFX_PTM_CFG_T *)ifr->ifr_data)->TxTcCrcGen      = TX_GAMMA_ITF_CFG(0)->tx_tc_crc_size == 0 ? 0 : 1;
        ((IFX_PTM_CFG_T *)ifr->ifr_data)->TxTcCrcLen      = TX_GAMMA_ITF_CFG(0)->tx_tc_crc_size == 0 ? 0 : (TX_GAMMA_ITF_CFG(0)->tx_tc_crc_size * 16);
        break;
    case IFX_PTM_CFG_SET:
        {
            int i;

            for ( i = 0; i < 4; i++ ) {
                RX_GAMMA_ITF_CFG(i)->rx_eth_fcs_ver_dis = ((IFX_PTM_CFG_T *)ifr->ifr_data)->RxEthCrcCheck ? 0 : 1;

                RX_GAMMA_ITF_CFG(0)->rx_tc_crc_ver_dis = ((IFX_PTM_CFG_T *)ifr->ifr_data)->RxTcCrcCheck ? 0 : 1;

                switch ( ((IFX_PTM_CFG_T *)ifr->ifr_data)->RxTcCrcLen ) {
                    case 16: RX_GAMMA_ITF_CFG(0)->rx_tc_crc_size = 1; break;
                    case 32: RX_GAMMA_ITF_CFG(0)->rx_tc_crc_size = 2; break;
                    default: RX_GAMMA_ITF_CFG(0)->rx_tc_crc_size = 0;
                }

                TX_GAMMA_ITF_CFG(0)->tx_eth_fcs_gen_dis = ((IFX_PTM_CFG_T *)ifr->ifr_data)->TxEthCrcGen ? 0 : 1;

                if ( ((IFX_PTM_CFG_T *)ifr->ifr_data)->TxTcCrcGen ) {
                    switch ( ((IFX_PTM_CFG_T *)ifr->ifr_data)->TxTcCrcLen ) {
                        case 16: TX_GAMMA_ITF_CFG(0)->tx_tc_crc_size = 1; break;
                        case 32: TX_GAMMA_ITF_CFG(0)->tx_tc_crc_size = 2; break;
                        default: TX_GAMMA_ITF_CFG(0)->tx_tc_crc_size = 0;
                    }
                }
                else
                    TX_GAMMA_ITF_CFG(0)->tx_tc_crc_size = 0;
            }
        }
        break;
    case IFX_PTM_MAP_PKT_PRIO_TO_Q:
        {
            struct ppe_prio_q_map cmd;

            if ( copy_from_user(&cmd, ifr->ifr_data, sizeof(cmd)) )
                return -EFAULT;

            if ( cmd.pkt_prio < 0 || cmd.pkt_prio >= NUM_ENTITY(g_ptm_prio_queue_map) )
                return -EINVAL;

            if ( cmd.qid < 0 || cmd.qid >= g_wanqos_en )
                return -EINVAL;

            g_ptm_prio_queue_map[cmd.pkt_prio] = cmd.qid;
        }
        break;
    default:
        return -EOPNOTSUPP;
    }

    return 0;
}

static void ptm_tx_timeout(struct net_device *dev)
{
    ASSERT(dev == g_net_dev[0], "incorrect device");

    /*  disable TX irq, release skb when sending new packet */
    IFX_REG_W32_MASK(1 << 17, 0, MBOX_IGU1_IER);

    /*  wake up TX queue    */
    netif_wake_queue(dev);

    return;
}

static INLINE void adsl_led_flash(void)
{
#if defined(ENABLE_LED_FRAMEWORK) && ENABLE_LED_FRAMEWORK
    if ( g_data_led_trigger != NULL )
        ifx_led_trigger_activate(g_data_led_trigger);
#else
    return ifx_mei_atm_led_blink();
#endif
}

static INLINE struct sk_buff* alloc_skb_rx(void)
{
    struct sk_buff *skb;

    /*  allocate memroy including trailer and padding   */
    skb = dev_alloc_skb(RX_MAX_BUFFER_SIZE + DATA_BUFFER_ALIGNMENT);
    if ( skb != NULL ) {
        /*  must be burst length alignment and reserve two more bytes for MAC address alignment  */
        if ( ((unsigned int)skb->data & (DATA_BUFFER_ALIGNMENT - 1)) != 0 )
            skb_reserve(skb, ~((unsigned int)skb->data + (DATA_BUFFER_ALIGNMENT - 1)) & (DATA_BUFFER_ALIGNMENT - 1));
        /*  pub skb in reserved area "skb->data - 4"    */
        *((struct sk_buff **)skb->data - 1) = skb;
        wmb();
        /*  write back and invalidate cache    */
        dma_cache_sync(NULL, (void *)((unsigned long)skb->data - sizeof(skb)), sizeof(skb), DMA_BIDIRECTIONAL);
        /*  invalidate cache    */
        dma_cache_sync(NULL, skb->data, (unsigned int)skb->end - (unsigned int)skb->data, DMA_FROM_DEVICE);
    }

    return skb;
}

static INLINE struct sk_buff* alloc_skb_tx(unsigned int size)
{
    struct sk_buff *skb;

    /*  allocate memory including padding   */
    size = RX_MAX_BUFFER_SIZE;
    size = (size + DATA_BUFFER_ALIGNMENT - 1) & ~(DATA_BUFFER_ALIGNMENT - 1);
    skb = dev_alloc_skb(size + DATA_BUFFER_ALIGNMENT);
    /*  must be burst length alignment  */
    if ( skb != NULL )
        skb_reserve(skb, ~((unsigned int)skb->data + (DATA_BUFFER_ALIGNMENT - 1)) & (DATA_BUFFER_ALIGNMENT - 1));
    return skb;
}

static INLINE struct sk_buff *get_skb_pointer(unsigned int dataptr)
{
    unsigned int skb_dataptr;
    struct sk_buff *skb;

    //  usually, CPE memory is less than 256M bytes
    //  so NULL means invalid pointer
    if ( dataptr == 0 ) {
        dbg("dataptr is 0, it's supposed to be invalid pointer");
        return NULL;
    }

    skb_dataptr = (dataptr - 4) | KSEG1;
    skb = *(struct sk_buff **)skb_dataptr;

    ASSERT((unsigned int)skb >= KSEG0, "invalid skb - skb = %#08x, dataptr = %#08x", (unsigned int)skb, dataptr);
    ASSERT((((unsigned int)skb->data & (0x0FFFFFFF ^ (DATA_BUFFER_ALIGNMENT - 1))) | KSEG1) == (dataptr | KSEG1), "invalid skb - skb = %#08x, skb->data = %#08x, dataptr = %#08x", (unsigned int)skb, (unsigned int)skb->data, dataptr);

    return skb;
}

static INLINE int get_tx_desc(unsigned int itf, unsigned int *f_full)
{
    int desc_base = -1;
    struct ptm_itf *p_itf = &g_ptm_priv_data.itf[0];

    //  assume TX is serial operation
    //  no protection provided

    *f_full = 1;

    if ( CPU_TO_WAN_TX_DESC_BASE[p_itf->tx_desc_pos].own == 0 ) {
        desc_base = p_itf->tx_desc_pos;
        if ( ++(p_itf->tx_desc_pos) == CPU_TO_WAN_TX_DESC_NUM )
            p_itf->tx_desc_pos = 0;
        if ( CPU_TO_WAN_TX_DESC_BASE[p_itf->tx_desc_pos].own == 0 )
            *f_full = 0;
    }

    return desc_base;
}

static irqreturn_t mailbox_irq_handler(int irq, void *dev_id)
{
    unsigned int isr;
    int i;

    isr = IFX_REG_R32(MBOX_IGU1_ISR);
    IFX_REG_W32(isr, MBOX_IGU1_ISRC);
    isr &= IFX_REG_R32(MBOX_IGU1_IER);

    while ( (i = clz(isr)) >= 0 ) {
        isr ^= 1 << i;

        switch ( i ) {
            case 0:     //  RX
#if defined(CONFIG_LTQ_PTM_RX_NAPI)
                IFX_REG_W32_MASK(1, 0, MBOX_IGU1_IER);
  #if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
                netif_rx_schedule(g_net_dev[0]);
  #else
                napi_schedule(&g_ptm_priv_data.itf[0].napi);
  #endif
#endif
#if defined(ENABLE_TMP_DBG) && ENABLE_TMP_DBG
                {
                    volatile struct rx_descriptor *desc = &WAN_RX_DESC_BASE[g_ptm_priv_data.itf[0].rx_desc_pos];

                    if ( desc->own ) {  //  PP32 hold
                        err("invalid interrupt");
                    }
                }
#endif
                break;
            case 16:    //  SWAP
                IFX_REG_W32_MASK(1 << 16, 0, MBOX_IGU1_IER);
                tasklet_hi_schedule(&g_swap_desc_tasklet);
                break;
            case 17:    //  TX
                IFX_REG_W32_MASK(1 << 17, 0, MBOX_IGU1_IER);
                netif_wake_queue(g_net_dev[0]);
                break;
        }
    }

    return IRQ_HANDLED;
}

static void do_swap_desc_tasklet(unsigned long arg)
{
    int budget = 32;
    volatile struct tx_descriptor *desc;
    struct sk_buff *skb;
    unsigned int byteoff;

    while ( budget-- > 0 ) {
        if ( WAN_SWAP_DESC_BASE[g_ptm_priv_data.itf[0].tx_swap_desc_pos].own )  //  if PP32 hold descriptor
            break;

        desc = &WAN_SWAP_DESC_BASE[g_ptm_priv_data.itf[0].tx_swap_desc_pos];
        if ( ++g_ptm_priv_data.itf[0].tx_swap_desc_pos == WAN_SWAP_DESC_NUM )
            g_ptm_priv_data.itf[0].tx_swap_desc_pos = 0;

        skb = get_skb_pointer(desc->dataptr);
        if ( skb != NULL )
            dev_kfree_skb_any(skb);

        skb = alloc_skb_tx(RX_MAX_BUFFER_SIZE);
        if ( skb == NULL )
            panic("can't allocate swap buffer for PPE firmware use\n");
        byteoff = (unsigned int)skb->data & (DATA_BUFFER_ALIGNMENT - 1);
        *(struct sk_buff **)((unsigned int)skb->data - byteoff - sizeof(struct sk_buff *)) = skb;

        desc->dataptr = (unsigned int)skb->data & 0x0FFFFFFF;
        desc->own = 1;
    }

    //  clear interrupt
    IFX_REG_W32_MASK(0, 16, MBOX_IGU1_ISRC);
    //  no more skb to be replaced
    if ( WAN_SWAP_DESC_BASE[g_ptm_priv_data.itf[0].tx_swap_desc_pos].own ) {    //  if PP32 hold descriptor
        IFX_REG_W32_MASK(0, 1 << 16, MBOX_IGU1_IER);
        return;
    }

    tasklet_hi_schedule(&g_swap_desc_tasklet);
    return;
}

#if defined(DEBUG_DUMP_SKB) && DEBUG_DUMP_SKB
static void dump_skb(struct sk_buff *skb, u32 len, char *title, int port, int ch, int is_tx)
{
    int i;

    if ( !(ifx_ptm_dbg_enable & (is_tx ? DBG_ENABLE_MASK_DUMP_SKB_TX : DBG_ENABLE_MASK_DUMP_SKB_RX)) )
        return;

    if ( skb->len < len )
        len = skb->len;

    if ( len > ETH_MAX_FRAME_LENGTH ) {
        printk("too big data length: skb = %08x, skb->data = %08x, skb->len = %d\n", (u32)skb, (u32)skb->data, skb->len);
        return;
    }

    if ( ch >= 0 )
        printk("%s (port %d, ch %d)\n", title, port, ch);
    else
        printk("%s\n", title);
    printk("  skb->data = %08X, skb->tail = %08X, skb->len = %d\n", (u32)skb->data, (u32)skb->tail, (int)skb->len);
    for ( i = 1; i <= len; i++ ) {
        if ( i % 16 == 1 )
            printk("  %4d:", i - 1);
        printk(" %02X", (int)(*((char*)skb->data + i - 1) & 0xFF));
        if ( i % 16 == 0 )
            printk("\n");
    }
    if ( (i - 1) % 16 != 0 )
        printk("\n");
}
#endif

#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
static void skb_swap(struct sk_buff *skb)
{
    unsigned char tmp[8];
    unsigned char *p = skb->data;

    if ( p[12] == 0x08 && p[13] == 0x06 ) { //  arp
        if ( p[14] == 0x00 && p[15] == 0x01 && p[16] == 0x08 && p[17] == 0x00 && p[20] == 0x00 && p[21] == 0x01 ) {
            //  dest mac
            memcpy(p, p + 6, 6);
            //  src mac
            p[6] = p[7] = 0;
            memcpy(p + 8, p + 38, 4);
            //  arp reply
            p[21] = 0x02;
            //  sender mac
            memcpy(p + 22, p + 6, 6);
            //  sender IP
            memcpy(tmp, p + 28, 4);
            memcpy(p + 28, p + 38, 4);
            //  target mac
            memcpy(p + 32, p, 6);
            //  target IP
            memcpy(p + 38, tmp, 4);

            dma_cache_sync(NULL, skb->data, 42, DMA_TO_DEVICE);
        }
    }
    else if ( !(p[0] & 0x01) ) { //  bypass broadcast/multicast
        //  swap MAC
        memcpy(tmp, p, 6);
        memcpy(p, p + 6, 6);
        memcpy(p + 6, tmp, 6);
        p += 12;

        //  bypass VLAN
        while ( p[0] == 0x81 && p[1] == 0x00 )
            p += 4;

        //  IP
        if ( p[0] == 0x08 && p[1] == 0x00 ) {
            p += 14;
            memcpy(tmp, p, 4);
            memcpy(p, p + 4, 4);
            memcpy(p + 4, tmp, 4);
            p += 8;
        }

        dma_cache_sync(NULL, skb->data, (unsigned long)p - (unsigned long)skb->data, DMA_TO_DEVICE);
    }
}
#endif

static INLINE void proc_file_create(void)
{
#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
    struct proc_dir_entry *res;
#endif

    g_ptm_dir = proc_mkdir("driver/ifx_ptm", NULL);

    create_proc_read_entry("version",
                            0,
                            g_ptm_dir,
                            proc_read_version,
                            NULL);

    res = create_proc_entry("wanmib",
                            0,
                            g_ptm_dir);
    if ( res )
    {
        res->read_proc  = proc_read_wanmib;
        res->write_proc = proc_write_wanmib;
    }

    res = create_proc_entry("prio",
                            0,
                            g_ptm_dir);
    if ( res )
    {
        res->read_proc  = proc_read_prio;
        res->write_proc = proc_write_prio;
    }

    res = create_proc_entry("qos",
                            0,
                            g_ptm_dir);
    if ( res )
        res->read_proc  = proc_read_qos;

    res = create_proc_entry("gamma",
                            0,
                            g_ptm_dir);
    if ( res )
        res->read_proc  = proc_read_gamma;

#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
    res = create_proc_entry("dbg",
                            0,
                            g_ptm_dir);
    if ( res )
    {
        res->read_proc  = proc_read_dbg;
        res->write_proc = proc_write_dbg;
    }
#endif

#if defined(ENABLE_TMP_DBG) && ENABLE_TMP_DBG
    res = create_proc_entry("rdptr",
                            0,
                            g_ptm_dir);
    if ( res )
        res->read_proc  = proc_read_rdptr;
#endif
}

static INLINE void proc_file_delete(void)
{
#if defined(ENABLE_TMP_DBG) && ENABLE_TMP_DBG
    remove_proc_entry("rdptr", g_ptm_dir);
#endif

#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
    remove_proc_entry("dbg", g_ptm_dir);
#endif

    remove_proc_entry("gamma", g_ptm_dir);

    remove_proc_entry("qos", g_ptm_dir);

    remove_proc_entry("prio", g_ptm_dir);

    remove_proc_entry("wanmib", g_ptm_dir);

    remove_proc_entry("version", g_ptm_dir);

    remove_proc_entry("driver/ifx_ptm", NULL);
}

static int proc_read_version(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
    int len = 0;

    len += ifx_ptm_version(buf + len);

    if ( offset >= len ) {
        *start = buf;
        *eof = 1;
        return 0;
    }
    *start = buf + offset;
    if ( (len -= offset) > count )
        return count;
    *eof = 1;
    return len;
}

static int proc_read_wanmib(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    volatile unsigned int *wrx_total_pdu[4] = {DREG_AR_AIIDLE_CNT0, DREG_AR_HEC_CNT0, DREG_AR_AIIDLE_CNT1, DREG_AR_HEC_CNT1};
    volatile unsigned int *wrx_err_pdu[4] = {DREG_AR_CERRN_CNT0, DREG_AR_CERRN_CNT1, DREG_AR_CERRNP_CNT0, DREG_AR_CERRNP_CNT1};
    int i;

    len += sprintf(page + off + len, "RX:\n");
    len += sprintf(page + off + len, "  wrx_total_pdu:   ");
    for ( i = 0; i < 4; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", IFX_REG_R32(wrx_total_pdu[i]) - g_wrx_total_pdu[i]);
    }
    len += sprintf(page + off + len, "\n");
    len += sprintf(page + off + len, "  wrx_dropdes_pdu: ");
    for ( i = 0; i < 4; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", WAN_RX_MIB_TABLE(i)->wrx_dropdes_pdu);
    }
    len += sprintf(page + off + len, "\n");
    len += sprintf(page + off + len, "  wrx_err_pdu:     ");
    for ( i = 0; i < 4; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", IFX_REG_R32(wrx_err_pdu[i]) - g_wrx_err_pdu[i]);
    }
    len += sprintf(page + off + len, "\n");
    len += sprintf(page + off + len, "  wrx_total_bytes: ");
    for ( i = 0; i < 4; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", WAN_RX_MIB_TABLE(i)->wrx_total_bytes);
    }
    len += sprintf(page + off + len, "\n");

    len += sprintf(page + off + len, "TX:\n");
    len += sprintf(page + off + len, "  wrx_total_pdu:           ");
    for ( i = 0; i < 8; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", WAN_TX_MIB_TABLE(i)->wrx_total_pdu);
    }
    len += sprintf(page + off + len, "\n");
    len += sprintf(page + off + len, "  wrx_total_bytes:         ");
    for ( i = 0; i < 8; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", WAN_TX_MIB_TABLE(i)->wrx_total_bytes);
    }
    len += sprintf(page + off + len, "\n");
    len += sprintf(page + off + len, "  wtx_total_pdu:           ");
    for ( i = 0; i < 8; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", WAN_TX_MIB_TABLE(i)->wtx_total_pdu);
    }
    len += sprintf(page + off + len, "\n");
    len += sprintf(page + off + len, "  wtx_total_bytes:         ");
    for ( i = 0; i < 8; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", WAN_TX_MIB_TABLE(i)->wtx_total_bytes);
    }
    len += sprintf(page + off + len, "\n");
    len += sprintf(page + off + len, "  wtx_cpu_dropdes_pdu:     ");
    for ( i = 0; i < 8; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", WAN_TX_MIB_TABLE(i)->wtx_cpu_dropdes_pdu);
    }
    len += sprintf(page + off + len, "\n");
    len += sprintf(page + off + len, "  wtx_cpu_dropsmall_pdu:   ");
    for ( i = 0; i < 8; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", WAN_TX_MIB_TABLE(i)->wtx_cpu_dropsmall_pdu);
    }
    len += sprintf(page + off + len, "\n");
    len += sprintf(page + off + len, "  wtx_fast_dropdes_pdu:    ");
    for ( i = 0; i < 8; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", WAN_TX_MIB_TABLE(i)->wtx_fast_dropdes_pdu);
    }
    len += sprintf(page + off + len, "\n");
    len += sprintf(page + off + len, "  wtx_fast_dropsmall_pdu:  ");
    for ( i = 0; i < 8; i++ ) {
        if ( i != 0 )
            len += sprintf(page + off + len, ", ");
        len += sprintf(page + off + len, "%10u", WAN_TX_MIB_TABLE(i)->wtx_fast_dropsmall_pdu);
    }
    len += sprintf(page + off + len, "\n");

    *eof = 1;

    return len;
}

static int proc_write_wanmib(struct file *file, const char *buf, unsigned long count, void *data)
{
    char str[2048];
    char *p;
    int len, rlen;

    volatile unsigned int *wrx_total_pdu[4] = {DREG_AR_AIIDLE_CNT0, DREG_AR_HEC_CNT0, DREG_AR_AIIDLE_CNT1, DREG_AR_HEC_CNT1};
    volatile unsigned int *wrx_err_pdu[4] = {DREG_AR_CERRN_CNT0, DREG_AR_CERRN_CNT1, DREG_AR_CERRNP_CNT0, DREG_AR_CERRNP_CNT1};
    int i;

    len = count < sizeof(str) ? count : sizeof(str) - 1;
    rlen = len - copy_from_user(str, buf, len);
    while ( rlen && str[rlen - 1] <= ' ' )
        rlen--;
    str[rlen] = 0;
    for ( p = str; *p && *p <= ' '; p++, rlen-- );
    if ( !*p )
        return count;

    if ( stricmp(p, "clear") == 0 || stricmp(p, "clean") == 0 ) {
        void *tmp;
        for ( i = 0; i < 4; i++ ) {
            tmp = (void*)WAN_RX_MIB_TABLE(i);
            memset(tmp, 0, sizeof(*WAN_RX_MIB_TABLE(i)));
            g_wrx_total_pdu[i] = IFX_REG_R32(wrx_total_pdu[i]);
            g_wrx_err_pdu[i] = IFX_REG_R32(wrx_err_pdu[i]);
        }
        for ( i = 0; i < 8; i++ ) {
            tmp = (void*)WAN_TX_MIB_TABLE(i);
            memset(tmp, 0, sizeof(*WAN_TX_MIB_TABLE(i)));
        }
    }

    return count;
}

static int proc_read_prio(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    int j;

    len += sprintf(page + off + len,        "Priority to Queue Map:\n");
    len += sprintf(page + off + len,        "  prio     :  0  1  2  3  4  5  6  7\n");
    len += sprintf(page + off + len,    "  ptm0     :");
    for ( j = 0; j < NUM_ENTITY(g_ptm_prio_queue_map); j++ )
        len += sprintf(page + off + len,"  %d", g_ptm_prio_queue_map[j]);
    len += sprintf(page + off + len,    "\n");

    *eof = 1;

    return len;
}

static int proc_write_prio(struct file *file, const char *buf, unsigned long count, void *data)
{
    char local_buf[2048];
    int len;
    char *p1, *p2;
    int colon = 1;
    unsigned int port = ~0;
    int prio = -1;
    int queue = -1;

    len = sizeof(local_buf) < count ? sizeof(local_buf) - 1 : count;
    len = len - copy_from_user(local_buf, buf, len);
    local_buf[len] = 0;

    p1 = local_buf;
    while ( get_token(&p1, &p2, &len, &colon) )
    {
        if ( stricmp(p1, "help") == 0 )
        {
            printk("echo <ptm0> prio xx queue xx [prio xx queue xx] > /proc/driver/ifx_ptm/prio\n");
            break;
        }
        else if ( stricmp(p1, "eth0") == 0 )
        {
            printk("eth0 is not valid\n");
            prio = queue = -1;
        }
        else if ( stricmp(p1, "eth1") == 0 )
        {
            printk("eth1 is not valid\n");
            prio = queue = -1;
        }
        else if ( stricmp(p1, "ptm0") == 0 )
        {
            port = 2;
            prio = queue = -1;
            printk("port = 2 (PTM0)\n");
        }
        else if ( stricmp(p1, "pvc") == 0 )
        {
            printk("atm is not valid\n");
            prio = queue = -1;
        }
        else if ( port != ~0 )
        {
            if ( stricmp(p1, "p") == 0 || stricmp(p1, "prio") == 0 )
            {
                ignore_space(&p2, &len);
                prio = get_number(&p2, &len, 0);
                printk("prio = %d\n", prio);
                if ( port == 2 )
                {
                    if ( prio >= 0 && prio <  NUM_ENTITY(g_ptm_prio_queue_map) )
                    {
                        if ( queue >= 0 )
                            g_ptm_prio_queue_map[prio] = queue;
                    }
                    else
                        printk("prio (%d) is out of range 0 - %d\n", prio, NUM_ENTITY(g_ptm_prio_queue_map) - 1);
                }
            }
            else if ( stricmp(p1, "q") == 0 || stricmp(p1, "queue") == 0 )
            {
                ignore_space(&p2, &len);
                queue = get_number(&p2, &len, 0);
                printk("queue = %d\n", queue);
                if ( port == 2 )
                {
                    if ( queue >= 0 && queue < g_wanqos_en )
                    {
                        if ( prio >= 0 )
                            g_ptm_prio_queue_map[prio] = queue;
                    }
                    else
                        printk("queue (%d) is out of range 0 - %d\n", queue, g_wanqos_en - 1);
                }
            }
            else
                printk("unknown command (%s)\n", p1);
        }
        else
            printk("unknown command (%s)\n", p1);

        p1 = p2;
        colon = 1;
    }

    return count;
}

static int proc_read_qos(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;
    int len_max = off + count;
    char *pstr;
    char str[200];
    int llen;
    int i;
    struct wan_tx_mib_table qos_queue_mib;
    volatile struct tx_qos_cfg tx_qos_cfg = *TX_QOS_CFG;
    volatile struct wtx_qos_q_desc_cfg qos_q_desc_cfg;

    pstr = *start = page;

    __sync();

    llen = sprintf(pstr, "\n  qos         : %s\n  wfq         : %s\n  Rate shaping: %s\n\n",
                    tx_qos_cfg.eth1_qss ?"enabled":"disabled",
                    tx_qos_cfg.wfq_en?"enabled":"disabled",
                    tx_qos_cfg.shape_en ?"enabled":"disabled");
    pstr += llen;
    len += llen;

    llen = sprintf(pstr, "  Ticks  =%u,    overhd    =%u,       qnum=%u  @%p\n", (u32)tx_qos_cfg.time_tick, (u32)tx_qos_cfg.overhd_bytes, (u32)tx_qos_cfg.eth1_eg_qnum, TX_QOS_CFG );
    pstr += llen;
    len += llen;

    //llen = sprintf(pstr, "  PPE clk=%u MHz, basic tick=%u\n", (u32)cgu_get_pp32_clock() / 1000000, TX_QOS_CFG->time_tick / (cgu_get_pp32_clock() / 1000000));
    pstr += llen;
    len += llen;

    if ( tx_qos_cfg.eth1_eg_qnum )
    {
        llen = sprintf(pstr, "\n  MIB : rx_pkt/rx_bytes         tx_pkt/tx_bytes       cpu_small_drop/cpu_drop  fast_small_drop/fast_drop_cnt\n");
        pstr += llen;
        len += llen;
        for ( i = 0; i < 8; i++ )
        {
            qos_queue_mib = *WAN_TX_MIB_TABLE(i);

            llen = sprintf(str, "    %2u: %010u/%010u  %010u/%010u  %010u/%010u  %010u/%010u  @0x%p\n", i,
                qos_queue_mib.wrx_total_pdu, qos_queue_mib.wrx_total_bytes,
                qos_queue_mib.wtx_total_pdu, qos_queue_mib.wtx_total_bytes,
                qos_queue_mib.wtx_cpu_dropsmall_pdu, qos_queue_mib.wtx_cpu_dropdes_pdu,
                qos_queue_mib.wtx_fast_dropsmall_pdu, qos_queue_mib.wtx_fast_dropdes_pdu,
                WAN_TX_MIB_TABLE(i));

            if ( len <= off && len + llen > off )
            {
                memcpy(pstr, str + off - len, len + llen - off);
                pstr += len + llen - off;
            }
            else if ( len > off )
            {
                memcpy(pstr, str, llen);
                pstr += llen;
            }
            len += llen;
            if ( len >= len_max )
                goto PROC_READ_MAC_OVERRUN_END;
        }


        //QOS queue descriptor
        llen = sprintf(pstr, "\n  Desc: threshold  num    base_addr  rd_ptr   wr_ptr\n");
        pstr += llen;
        len += llen;
        for ( i = 0; i < 8; i++ )
        {
            qos_q_desc_cfg = *WTX_QOS_Q_DESC_CFG(i);

            llen = sprintf(pstr, "    %2u: 0x%02x       0x%02x   0x%04x     0x%04x   0x%04x  @0x%p\n", i,
                qos_q_desc_cfg.threshold,
                qos_q_desc_cfg.length,
                qos_q_desc_cfg.addr,
                qos_q_desc_cfg.rd_ptr,
                qos_q_desc_cfg.wr_ptr,
                WTX_QOS_Q_DESC_CFG(i) );

            pstr += llen;
            len += llen;
        }
    }

    *eof = 1;

    return len - off;

PROC_READ_MAC_OVERRUN_END:
    return len - llen - off;
}

static int proc_read_gamma(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;

    len += sprintf(page + off + len, "PSAVE_CFG     (0x%08X): start_state - %s, sleep_en - %s\n", (unsigned int)PSAVE_CFG, PSAVE_CFG->start_state ? "partial reset" : "full reset", PSAVE_CFG->sleep_en ? "on" : "off");
    len += sprintf(page + off + len, "EG_BWCTRL_CFG (0x%08X): fdesc_wm - %d, class_len - %d\n", (unsigned int)EG_BWCTRL_CFG, EG_BWCTRL_CFG->fdesc_wm, EG_BWCTRL_CFG->class_len);
    len += sprintf(page + off + len, "TEST_MODE     (0x%08X): mib_clear_mode - %s, test_mode - %s\n", (unsigned int)TEST_MODE, TEST_MODE->mib_clear_mode ? "on" : "off", TEST_MODE->test_mode ? "on" : "off");
    len += sprintf(page + off + len, "RX_BC_CFG:\t0x%08x\t0x%08x\n",   (unsigned int)RX_BC_CFG(0),                (unsigned int)RX_BC_CFG(1));
    len += sprintf(page + off + len, "  local_state:    %8d\t  %8d\n", (unsigned int)RX_BC_CFG(0)->local_state,   (unsigned int)RX_BC_CFG(1)->local_state);
    len += sprintf(page + off + len, "  remote_state:   %8d\t  %8d\n", (unsigned int)RX_BC_CFG(0)->remote_state,  (unsigned int)RX_BC_CFG(1)->remote_state);
    len += sprintf(page + off + len, "  to_false_th:    %8d\t  %8d\n", (unsigned int)RX_BC_CFG(0)->to_false_th,   (unsigned int)RX_BC_CFG(1)->to_false_th);
    len += sprintf(page + off + len, "  to_looking_th:  %8d\t  %8d\n", (unsigned int)RX_BC_CFG(0)->to_looking_th, (unsigned int)RX_BC_CFG(1)->to_looking_th);
    len += sprintf(page + off + len, "TX_BC_CFG:\t0x%08x\t0x%08x\n",   (unsigned int)TX_BC_CFG(0),                (unsigned int)TX_BC_CFG(1));
    len += sprintf(page + off + len, "  fill_wm:\t  %8d\t  %8d\n", (unsigned int)TX_BC_CFG(0)->fill_wm,   (unsigned int)TX_BC_CFG(1)->fill_wm);
    len += sprintf(page + off + len, "  uflw_wm:\t  %8d\t  %8d\n", (unsigned int)TX_BC_CFG(0)->uflw_wm,   (unsigned int)TX_BC_CFG(1)->uflw_wm);
    len += sprintf(page + off + len, "RX_GAMMA_ITF_CFG:\t0x%08x\t0x%08x\t0x%08x\t0x%08x\n",       (unsigned int)RX_GAMMA_ITF_CFG(0),                        (unsigned int)RX_GAMMA_ITF_CFG(1),                        (unsigned int)RX_GAMMA_ITF_CFG(2),                        (unsigned int)RX_GAMMA_ITF_CFG(3));
    len += sprintf(page + off + len, "  receive_state: \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->receive_state,         (unsigned int)RX_GAMMA_ITF_CFG(1)->receive_state,         (unsigned int)RX_GAMMA_ITF_CFG(2)->receive_state,         (unsigned int)RX_GAMMA_ITF_CFG(3)->receive_state);
    len += sprintf(page + off + len, "  rx_min_len:    \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_min_len,            (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_min_len,            (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_min_len,            (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_min_len);
    len += sprintf(page + off + len, "  rx_pad_en:     \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_pad_en,             (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_pad_en,             (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_pad_en,             (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_pad_en);
    len += sprintf(page + off + len, "  rx_eth_fcs_ver_dis:\t  %8d\t  %8d\t  %8d\t  %8d\n",       (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_eth_fcs_ver_dis,    (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_eth_fcs_ver_dis,    (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_eth_fcs_ver_dis,    (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_eth_fcs_ver_dis);
    len += sprintf(page + off + len, "  rx_rm_eth_fcs: \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_rm_eth_fcs,         (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_rm_eth_fcs,         (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_rm_eth_fcs,         (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_rm_eth_fcs);
    len += sprintf(page + off + len, "  rx_tc_crc_ver_dis:\t  %8d\t  %8d\t  %8d\t  %8d\n",        (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_tc_crc_ver_dis,     (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_tc_crc_ver_dis,     (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_tc_crc_ver_dis,     (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_tc_crc_ver_dis);
    len += sprintf(page + off + len, "  rx_tc_crc_size:\t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_tc_crc_size,        (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_tc_crc_size,        (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_tc_crc_size,        (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_tc_crc_size);
    len += sprintf(page + off + len, "  rx_eth_fcs_result:\t0x%8X\t0x%8X\t0x%8X\t0x%8X\n",        (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_eth_fcs_result,     (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_eth_fcs_result,     (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_eth_fcs_result,     (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_eth_fcs_result);
    len += sprintf(page + off + len, "  rx_tc_crc_result:\t0x%8X\t0x%8X\t0x%8X\t0x%8X\n",         (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_tc_crc_result,      (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_tc_crc_result,      (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_tc_crc_result,      (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_tc_crc_result);
    len += sprintf(page + off + len, "  rx_crc_cfg:    \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_crc_cfg,            (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_crc_cfg,            (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_crc_cfg,            (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_crc_cfg);
    len += sprintf(page + off + len, "  rx_eth_fcs_init_value:0x%08X\t0x%08X\t0x%08X\t0x%08X\n",  (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_eth_fcs_init_value, (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_eth_fcs_init_value, (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_eth_fcs_init_value, (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_eth_fcs_init_value);
    len += sprintf(page + off + len, "  rx_tc_crc_init_value:\t0x%08X\t0x%08X\t0x%08X\t0x%08X\n", (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_tc_crc_init_value,  (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_tc_crc_init_value,  (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_tc_crc_init_value,  (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_tc_crc_init_value);
    len += sprintf(page + off + len, "  rx_max_len_sel:\t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_max_len_sel,        (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_max_len_sel,        (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_max_len_sel,        (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_max_len_sel);
    len += sprintf(page + off + len, "  rx_edit_num2:  \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_edit_num2,          (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_edit_num2,          (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_edit_num2,          (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_edit_num2);
    len += sprintf(page + off + len, "  rx_edit_pos2:  \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_edit_pos2,          (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_edit_pos2,          (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_edit_pos2,          (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_edit_pos2);
    len += sprintf(page + off + len, "  rx_edit_type2: \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_edit_type2,         (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_edit_type2,         (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_edit_type2,         (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_edit_type2);
    len += sprintf(page + off + len, "  rx_edit_en2:   \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_edit_en2,           (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_edit_en2,           (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_edit_en2,           (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_edit_en2);
    len += sprintf(page + off + len, "  rx_edit_num1:  \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_edit_num1,          (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_edit_num1,          (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_edit_num1,          (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_edit_num1);
    len += sprintf(page + off + len, "  rx_edit_pos1:  \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_edit_pos1,          (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_edit_pos1,          (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_edit_pos1,          (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_edit_pos1);
    len += sprintf(page + off + len, "  rx_edit_type1: \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_edit_type1,         (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_edit_type1,         (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_edit_type1,         (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_edit_type1);
    len += sprintf(page + off + len, "  rx_edit_en1:   \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_edit_en1,           (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_edit_en1,           (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_edit_en1,           (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_edit_en1);
    len += sprintf(page + off + len, "  rx_inserted_bytes_1l:\t0x%08X\t0x%08X\t0x%08X\t0x%08X\n", (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_inserted_bytes_1l,  (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_inserted_bytes_1l,  (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_inserted_bytes_1l,  (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_inserted_bytes_1l);
    len += sprintf(page + off + len, "  rx_inserted_bytes_1h:\t0x%08X\t0x%08X\t0x%08X\t0x%08X\n", (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_inserted_bytes_1h,  (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_inserted_bytes_1h,  (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_inserted_bytes_1h,  (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_inserted_bytes_1h);
    len += sprintf(page + off + len, "  rx_inserted_bytes_2l:\t0x%08X\t0x%08X\t0x%08X\t0x%08X\n", (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_inserted_bytes_2l,  (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_inserted_bytes_2l,  (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_inserted_bytes_2l,  (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_inserted_bytes_2l);
    len += sprintf(page + off + len, "  rx_inserted_bytes_2h:\t0x%08X\t0x%08X\t0x%08X\t0x%08X\n", (unsigned int)RX_GAMMA_ITF_CFG(0)->rx_inserted_bytes_2h,  (unsigned int)RX_GAMMA_ITF_CFG(1)->rx_inserted_bytes_2h,  (unsigned int)RX_GAMMA_ITF_CFG(2)->rx_inserted_bytes_2h,  (unsigned int)RX_GAMMA_ITF_CFG(3)->rx_inserted_bytes_2h);
    len += sprintf(page + off + len, "RX_GAMMA_ITF_CFG:\t0x%08x\t0x%08x\t0x%08x\t0x%08x\n",       (unsigned int)TX_GAMMA_ITF_CFG(0),                        (unsigned int)TX_GAMMA_ITF_CFG(1),                        (unsigned int)TX_GAMMA_ITF_CFG(2),                        (unsigned int)TX_GAMMA_ITF_CFG(3));
    len += sprintf(page + off + len, "  tx_len_adj:    \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)TX_GAMMA_ITF_CFG(0)->tx_len_adj,            (unsigned int)TX_GAMMA_ITF_CFG(1)->tx_len_adj,            (unsigned int)TX_GAMMA_ITF_CFG(2)->tx_len_adj,            (unsigned int)TX_GAMMA_ITF_CFG(3)->tx_len_adj);
    len += sprintf(page + off + len, "  tx_crc_off_adj:\t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)TX_GAMMA_ITF_CFG(0)->tx_crc_off_adj,        (unsigned int)TX_GAMMA_ITF_CFG(1)->tx_crc_off_adj,        (unsigned int)TX_GAMMA_ITF_CFG(2)->tx_crc_off_adj,        (unsigned int)TX_GAMMA_ITF_CFG(3)->tx_crc_off_adj);
    len += sprintf(page + off + len, "  tx_min_len:    \t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)TX_GAMMA_ITF_CFG(0)->tx_min_len,            (unsigned int)TX_GAMMA_ITF_CFG(1)->tx_min_len,            (unsigned int)TX_GAMMA_ITF_CFG(2)->tx_min_len,            (unsigned int)TX_GAMMA_ITF_CFG(3)->tx_min_len);
    len += sprintf(page + off + len, "  tx_eth_fcs_gen_dis:\t  %8d\t  %8d\t  %8d\t  %8d\n",       (unsigned int)TX_GAMMA_ITF_CFG(0)->tx_eth_fcs_gen_dis,    (unsigned int)TX_GAMMA_ITF_CFG(1)->tx_eth_fcs_gen_dis,    (unsigned int)TX_GAMMA_ITF_CFG(2)->tx_eth_fcs_gen_dis,    (unsigned int)TX_GAMMA_ITF_CFG(3)->tx_eth_fcs_gen_dis);
    len += sprintf(page + off + len, "  tx_tc_crc_size:\t  %8d\t  %8d\t  %8d\t  %8d\n",           (unsigned int)TX_GAMMA_ITF_CFG(0)->tx_tc_crc_size,        (unsigned int)TX_GAMMA_ITF_CFG(1)->tx_tc_crc_size,        (unsigned int)TX_GAMMA_ITF_CFG(2)->tx_tc_crc_size,        (unsigned int)TX_GAMMA_ITF_CFG(3)->tx_tc_crc_size);
    len += sprintf(page + off + len, "  tx_crc_cfg:    \t0x%08X\t0x%08X\t0x%08X\t0x%08X\n",       (unsigned int)TX_GAMMA_ITF_CFG(0)->tx_crc_cfg,            (unsigned int)TX_GAMMA_ITF_CFG(1)->tx_crc_cfg,            (unsigned int)TX_GAMMA_ITF_CFG(2)->tx_crc_cfg,            (unsigned int)TX_GAMMA_ITF_CFG(3)->tx_crc_cfg);
    len += sprintf(page + off + len, "  tx_eth_fcs_init_value:0x%08X\t0x%08X\t0x%08X\t0x%08X\n",  (unsigned int)TX_GAMMA_ITF_CFG(0)->tx_eth_fcs_init_value, (unsigned int)TX_GAMMA_ITF_CFG(1)->tx_eth_fcs_init_value, (unsigned int)TX_GAMMA_ITF_CFG(2)->tx_eth_fcs_init_value, (unsigned int)TX_GAMMA_ITF_CFG(3)->tx_eth_fcs_init_value);
    len += sprintf(page + off + len, "  tx_tc_crc_init_value:\t0x%08X\t0x%08X\t0x%08X\t0x%08X\n", (unsigned int)TX_GAMMA_ITF_CFG(0)->tx_tc_crc_init_value,  (unsigned int)TX_GAMMA_ITF_CFG(1)->tx_tc_crc_init_value,  (unsigned int)TX_GAMMA_ITF_CFG(2)->tx_tc_crc_init_value,  (unsigned int)TX_GAMMA_ITF_CFG(3)->tx_tc_crc_init_value);
    len += sprintf(page + off + len, "  queue_mapping: \t0x%08X\t0x%08X\t0x%08X\t0x%08X\n",       (unsigned int)TX_GAMMA_ITF_CFG(0)->queue_mapping,         (unsigned int)TX_GAMMA_ITF_CFG(1)->queue_mapping,         (unsigned int)TX_GAMMA_ITF_CFG(2)->queue_mapping,         (unsigned int)TX_GAMMA_ITF_CFG(3)->queue_mapping);

    *eof = 1;

    return len;
}

#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
static int proc_read_dbg(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;

    len += sprintf(page + off + len, "error print      - %s\n", (ifx_ptm_dbg_enable & DBG_ENABLE_MASK_ERR)              ? "enabled" : "disabled");
    len += sprintf(page + off + len, "debug print      - %s\n", (ifx_ptm_dbg_enable & DBG_ENABLE_MASK_DEBUG_PRINT)      ? "enabled" : "disabled");
    len += sprintf(page + off + len, "assert           - %s\n", (ifx_ptm_dbg_enable & DBG_ENABLE_MASK_ASSERT)           ? "enabled" : "disabled");
    len += sprintf(page + off + len, "dump rx skb      - %s\n", (ifx_ptm_dbg_enable & DBG_ENABLE_MASK_DUMP_SKB_RX)      ? "enabled" : "disabled");
    len += sprintf(page + off + len, "dump tx skb      - %s\n", (ifx_ptm_dbg_enable & DBG_ENABLE_MASK_DUMP_SKB_TX)      ? "enabled" : "disabled");
    len += sprintf(page + off + len, "mac swap         - %s\n", (ifx_ptm_dbg_enable & DBG_ENABLE_MASK_MAC_SWAP)         ? "enabled" : "disabled");

    *eof = 1;

    return len;
}

static int proc_write_dbg(struct file *file, const char *buf, unsigned long count, void *data)
{
    static const char *dbg_enable_mask_str[] = {
        " error print",
        " err",
        " debug print",
        " dbg",
        " assert",
        " assert",
        " dump rx skb",
        " rx",
        " dump tx skb",
        " tx",
        " dump init",
        " init",
        " dump qos",
        " qos",
        " mac swap",
        " swap",
        " all"
    };
    static const int dbg_enable_mask_str_len[] = {
        12, 4,
        12, 4,
        7,  7,
        12, 3,
        12, 3,
        10, 5,
        9,  4,
        9,  5,
        4
    };
    unsigned int dbg_enable_mask[] = {
        DBG_ENABLE_MASK_ERR,
        DBG_ENABLE_MASK_DEBUG_PRINT,
        DBG_ENABLE_MASK_ASSERT,
        DBG_ENABLE_MASK_DUMP_SKB_RX,
        DBG_ENABLE_MASK_DUMP_SKB_TX,
        DBG_ENABLE_MASK_DUMP_INIT,
        DBG_ENABLE_MASK_DUMP_QOS,
        DBG_ENABLE_MASK_MAC_SWAP,
        DBG_ENABLE_MASK_ALL
    };

    char str[2048];
    char *p;

    int len, rlen;

    int f_enable = 0;
    int i;

    len = count < sizeof(str) ? count : sizeof(str) - 1;
    rlen = len - copy_from_user(str, buf, len);
    while ( rlen && str[rlen - 1] <= ' ' )
        rlen--;
    str[rlen] = 0;
    for ( p = str; *p && *p <= ' '; p++, rlen-- );
    if ( !*p )
        return 0;

    //  debugging feature for enter/leave showtime
    if ( strincmp(p, "enter", 5) == 0 && ifx_mei_atm_showtime_enter != NULL )
        ifx_mei_atm_showtime_enter(NULL, NULL);
    else if ( strincmp(p, "leave", 5) == 0 && ifx_mei_atm_showtime_exit != NULL )
        ifx_mei_atm_showtime_exit();

    if ( strincmp(p, "enable", 6) == 0 ) {
        p += 6;
        f_enable = 1;
    }
    else if ( strincmp(p, "disable", 7) == 0 ) {
        p += 7;
        f_enable = -1;
    }
    else if ( strincmp(p, "help", 4) == 0 || *p == '?' ) {
        printk("echo <enable/disable> [err/dbg/assert/rx/tx/init/qos/swap/all] > /proc/driver/ifx_ptm/dbg\n");
    }

    if ( f_enable ) {
        if ( *p == 0 ) {
            if ( f_enable > 0 )
                ifx_ptm_dbg_enable |= DBG_ENABLE_MASK_ALL & ~DBG_ENABLE_MASK_MAC_SWAP;
            else
                ifx_ptm_dbg_enable &= ~DBG_ENABLE_MASK_ALL | DBG_ENABLE_MASK_MAC_SWAP;
        }
        else {
            do {
                for ( i = 0; i < NUM_ENTITY(dbg_enable_mask_str); i++ )
                    if ( strincmp(p, dbg_enable_mask_str[i], dbg_enable_mask_str_len[i]) == 0 ) {
                        if ( f_enable > 0 )
                            ifx_ptm_dbg_enable |= dbg_enable_mask[i >> 1];
                        else
                            ifx_ptm_dbg_enable &= ~dbg_enable_mask[i >> 1];
                        p += dbg_enable_mask_str_len[i];
                        break;
                    }
            } while ( i < NUM_ENTITY(dbg_enable_mask_str) );
        }
    }

    return count;
}
#endif

#if defined(ENABLE_TMP_DBG) && ENABLE_TMP_DBG
static int proc_read_rdptr(char *page, char **start, off_t off, int count, int *eof, void *data)
{
    int len = 0;

    len += sprintf(page + off + len, "rx_desc_pos - %d\n", g_ptm_priv_data.itf[0].rx_desc_pos);

    *eof = 1;

    return len;
}
#endif

static INLINE int stricmp(const char *p1, const char *p2)
{
    int c1, c2;

    while ( *p1 && *p2 )
    {
        c1 = *p1 >= 'A' && *p1 <= 'Z' ? *p1 + 'a' - 'A' : *p1;
        c2 = *p2 >= 'A' && *p2 <= 'Z' ? *p2 + 'a' - 'A' : *p2;
        if ( (c1 -= c2) )
            return c1;
        p1++;
        p2++;
    }

    return *p1 - *p2;
}

#if defined(ENABLE_DBG_PROC) && ENABLE_DBG_PROC
static INLINE int strincmp(const char *p1, const char *p2, int n)
{
    int c1 = 0, c2;

    while ( n && *p1 && *p2 )
    {
        c1 = *p1 >= 'A' && *p1 <= 'Z' ? *p1 + 'a' - 'A' : *p1;
        c2 = *p2 >= 'A' && *p2 <= 'Z' ? *p2 + 'a' - 'A' : *p2;
        if ( (c1 -= c2) )
            return c1;
        p1++;
        p2++;
        n--;
    }

    return n ? *p1 - *p2 : c1;
}
#endif

static INLINE int get_token(char **p1, char **p2, int *len, int *colon)
{
    int tlen = 0;

    while ( *len && !((**p1 >= 'A' && **p1 <= 'Z') || (**p1 >= 'a' && **p1<= 'z') || (**p1 >= '0' && **p1<= '9')) )
    {
        (*p1)++;
        (*len)--;
    }
    if ( !*len )
        return 0;

    if ( *colon )
    {
        *colon = 0;
        *p2 = *p1;
        while ( *len && **p2 > ' ' && **p2 != ',' )
        {
            if ( **p2 == ':' )
            {
                *colon = 1;
                break;
            }
            (*p2)++;
            (*len)--;
            tlen++;
        }
        **p2 = 0;
    }
    else
    {
        *p2 = *p1;
        while ( *len && **p2 > ' ' && **p2 != ',' )
        {
            (*p2)++;
            (*len)--;
            tlen++;
        }
        **p2 = 0;
    }

    return tlen;
}

static INLINE int get_number(char **p, int *len, int is_hex)
{
    int ret = 0;
    int n = 0;

    if ( (*p)[0] == '0' && (*p)[1] == 'x' )
    {
        is_hex = 1;
        (*p) += 2;
        (*len) -= 2;
    }

    if ( is_hex )
    {
        while ( *len && ((**p >= '0' && **p <= '9') || (**p >= 'a' && **p <= 'f') || (**p >= 'A' && **p <= 'F')) )
        {
            if ( **p >= '0' && **p <= '9' )
                n = **p - '0';
            else if ( **p >= 'a' && **p <= 'f' )
               n = **p - 'a' + 10;
            else if ( **p >= 'A' && **p <= 'F' )
                n = **p - 'A' + 10;
            ret = (ret << 4) | n;
            (*p)++;
            (*len)--;
        }
    }
    else
    {
        while ( *len && **p >= '0' && **p <= '9' )
        {
            n = **p - '0';
            ret = ret * 10 + n;
            (*p)++;
            (*len)--;
        }
    }

    return ret;
}

static INLINE void ignore_space(char **p, int *len)
{
    while ( *len && (**p <= ' ' || **p == ':' || **p == '.' || **p == ',') )
    {
        (*p)++;
        (*len)--;
    }
}

static INLINE int ifx_ptm_version(char *buf)
{
    int len = 0;
    unsigned int major, mid, minor;

    ifx_ptm_get_fw_ver(&major, &mid, &minor);

    len += ifx_drv_ver(buf + len, "PTM", IFX_PTM_VER_MAJOR, IFX_PTM_VER_MID, IFX_PTM_VER_MINOR);
    if ( mid == ~0 )
        len += sprintf(buf + len, "    PTM (E1) firmware version %u.%u\n", major, minor);
    else
        len += sprintf(buf + len, "    PTM (E1) firmware version %u.%u.%u\n", major, mid, minor);

    return len;
}

static INLINE int init_priv_data(void)
{
    int i, j;

    g_wanqos_en = wanqos_en ? wanqos_en : 8;
    if ( g_wanqos_en > 8 )
        g_wanqos_en = 8;

    for ( i = 0; i < NUM_ENTITY(g_queue_gamma_map); i++ )
    {
        g_queue_gamma_map[i] = queue_gamma_map[i] & ((1 << g_wanqos_en) - 1);
        for ( j = 0; j < i; j++ )
            g_queue_gamma_map[i] &= ~g_queue_gamma_map[j];
    }

    memset(&g_ptm_priv_data, 0, sizeof(g_ptm_priv_data));

    {
        int max_packet_priority = NUM_ENTITY(g_ptm_prio_queue_map);
        int tx_num_q;
        int q_step, q_accum, p_step;

        tx_num_q = __ETH_WAN_TX_QUEUE_NUM;
        q_step = tx_num_q - 1;
        p_step = max_packet_priority - 1;
        for ( j = 0, q_accum = 0; j < max_packet_priority; j++, q_accum += q_step )
            g_ptm_prio_queue_map[j] = q_step - (q_accum + (p_step >> 1)) / p_step;
    }

    return 0;
}

static INLINE void clear_priv_data(void)
{
}

static INLINE int init_tables(void)
{
    struct sk_buff *skb_pool[WAN_RX_DESC_NUM] = {0};
    struct cfg_std_data_len cfg_std_data_len = {0};
    struct tx_qos_cfg tx_qos_cfg = {0};
    struct psave_cfg psave_cfg = {0};
    struct eg_bwctrl_cfg eg_bwctrl_cfg = {0};
    struct test_mode test_mode = {0};
    struct rx_bc_cfg rx_bc_cfg = {0};
    struct tx_bc_cfg tx_bc_cfg = {0};
    struct gpio_mode gpio_mode = {0};
    struct gpio_wm_cfg gpio_wm_cfg = {0};
    struct rx_gamma_itf_cfg rx_gamma_itf_cfg = {0};
    struct tx_gamma_itf_cfg tx_gamma_itf_cfg = {0};
    struct wtx_qos_q_desc_cfg wtx_qos_q_desc_cfg = {0};
    struct rx_descriptor rx_desc = {0};
    struct tx_descriptor tx_desc = {0};
    int i;

    for ( i = 0; i < WAN_RX_DESC_NUM; i++ ) {
        skb_pool[i] = alloc_skb_rx();
        if ( skb_pool[i] == NULL )
            goto ALLOC_SKB_RX_FAIL;
    }

    cfg_std_data_len.byte_off = RX_HEAD_MAC_ADDR_ALIGNMENT; //  this field replaces byte_off in rx descriptor of VDSL ingress
    cfg_std_data_len.data_len = 1600;
    *CFG_STD_DATA_LEN = cfg_std_data_len;

    //tx_qos_cfg.time_tick    = cgu_get_pp32_clock() / 62500; //  16 * (cgu_get_pp32_clock() / 1000000)
    tx_qos_cfg.overhd_bytes = 0;
    tx_qos_cfg.eth1_eg_qnum = __ETH_WAN_TX_QUEUE_NUM;
    tx_qos_cfg.eth1_burst_chk = 1;
    tx_qos_cfg.eth1_qss     = 0;
    tx_qos_cfg.shape_en     = 0;    //  disable
    tx_qos_cfg.wfq_en       = 0;    //  strict priority
    *TX_QOS_CFG = tx_qos_cfg;

    psave_cfg.start_state   = 0;
    psave_cfg.sleep_en      = 1;    //  enable sleep mode
    *PSAVE_CFG = psave_cfg;

    eg_bwctrl_cfg.fdesc_wm  = 16;
    eg_bwctrl_cfg.class_len = 128;
    *EG_BWCTRL_CFG = eg_bwctrl_cfg;

    //*GPIO_ADDR = (unsigned int)IFX_GPIO_P0_OUT;
    *GPIO_ADDR = (unsigned int)0x00000000;  //  disabled by default

    gpio_mode.gpio_bit_bc1 = 2;
    gpio_mode.gpio_bit_bc0 = 1;
    gpio_mode.gpio_bc1_en  = 0;
    gpio_mode.gpio_bc0_en  = 0;
    *GPIO_MODE = gpio_mode;

    gpio_wm_cfg.stop_wm_bc1  = 2;
    gpio_wm_cfg.start_wm_bc1 = 4;
    gpio_wm_cfg.stop_wm_bc0  = 2;
    gpio_wm_cfg.start_wm_bc0 = 4;
    *GPIO_WM_CFG = gpio_wm_cfg;

    test_mode.mib_clear_mode    = 0;
    test_mode.test_mode         = 0;
    *TEST_MODE = test_mode;

    rx_bc_cfg.local_state   = 0;
    rx_bc_cfg.remote_state  = 0;
    rx_bc_cfg.to_false_th   = 7;
    rx_bc_cfg.to_looking_th = 3;
    *RX_BC_CFG(0) = rx_bc_cfg;
    *RX_BC_CFG(1) = rx_bc_cfg;

    tx_bc_cfg.fill_wm   = 2;
    tx_bc_cfg.uflw_wm   = 2;
    *TX_BC_CFG(0) = tx_bc_cfg;
    *TX_BC_CFG(1) = tx_bc_cfg;

    rx_gamma_itf_cfg.receive_state      = 0;
    rx_gamma_itf_cfg.rx_min_len         = 60;
    rx_gamma_itf_cfg.rx_pad_en          = 1;
    rx_gamma_itf_cfg.rx_eth_fcs_ver_dis = 0;
    rx_gamma_itf_cfg.rx_rm_eth_fcs      = 1;
    rx_gamma_itf_cfg.rx_tc_crc_ver_dis  = 0;
    rx_gamma_itf_cfg.rx_tc_crc_size     = 1;
    rx_gamma_itf_cfg.rx_eth_fcs_result  = 0xC704DD7B;
    rx_gamma_itf_cfg.rx_tc_crc_result   = 0x1D0F1D0F;
    rx_gamma_itf_cfg.rx_crc_cfg         = 0x2500;
    rx_gamma_itf_cfg.rx_eth_fcs_init_value  = 0xFFFFFFFF;
    rx_gamma_itf_cfg.rx_tc_crc_init_value   = 0x0000FFFF;
    rx_gamma_itf_cfg.rx_max_len_sel     = 0;
    rx_gamma_itf_cfg.rx_edit_num2       = 0;
    rx_gamma_itf_cfg.rx_edit_pos2       = 0;
    rx_gamma_itf_cfg.rx_edit_type2      = 0;
    rx_gamma_itf_cfg.rx_edit_en2        = 0;
    rx_gamma_itf_cfg.rx_edit_num1       = 0;
    rx_gamma_itf_cfg.rx_edit_pos1       = 0;
    rx_gamma_itf_cfg.rx_edit_type1      = 0;
    rx_gamma_itf_cfg.rx_edit_en1        = 0;
    rx_gamma_itf_cfg.rx_inserted_bytes_1l   = 0;
    rx_gamma_itf_cfg.rx_inserted_bytes_1h   = 0;
    rx_gamma_itf_cfg.rx_inserted_bytes_2l   = 0;
    rx_gamma_itf_cfg.rx_inserted_bytes_2h   = 0;
    rx_gamma_itf_cfg.rx_len_adj         = -6;
    for ( i = 0; i < 4; i++ )
        *RX_GAMMA_ITF_CFG(i) = rx_gamma_itf_cfg;

    tx_gamma_itf_cfg.tx_len_adj         = 6;
    tx_gamma_itf_cfg.tx_crc_off_adj     = 6;
    tx_gamma_itf_cfg.tx_min_len         = 0;
    tx_gamma_itf_cfg.tx_eth_fcs_gen_dis = 0;
    tx_gamma_itf_cfg.tx_tc_crc_size     = 1;
    tx_gamma_itf_cfg.tx_crc_cfg         = 0x2F00;
    tx_gamma_itf_cfg.tx_eth_fcs_init_value  = 0xFFFFFFFF;
    tx_gamma_itf_cfg.tx_tc_crc_init_value   = 0x0000FFFF;
    for ( i = 0; i < NUM_ENTITY(g_queue_gamma_map); i++ ) {
        tx_gamma_itf_cfg.queue_mapping = g_queue_gamma_map[i];
        *TX_GAMMA_ITF_CFG(i) = tx_gamma_itf_cfg;
    }

    for ( i = 0; i < __ETH_WAN_TX_QUEUE_NUM; i++ ) {
        wtx_qos_q_desc_cfg.length = WAN_TX_DESC_NUM;
        wtx_qos_q_desc_cfg.addr   = __ETH_WAN_TX_DESC_BASE(i);
        *WTX_QOS_Q_DESC_CFG(i) = wtx_qos_q_desc_cfg;
    }

    //  default TX queue QoS config is all ZERO

    //  TX Ctrl K Table
    IFX_REG_W32(0x90111293, TX_CTRL_K_TABLE(0));
    IFX_REG_W32(0x14959617, TX_CTRL_K_TABLE(1));
    IFX_REG_W32(0x18999A1B, TX_CTRL_K_TABLE(2));
    IFX_REG_W32(0x9C1D1E9F, TX_CTRL_K_TABLE(3));
    IFX_REG_W32(0xA02122A3, TX_CTRL_K_TABLE(4));
    IFX_REG_W32(0x24A5A627, TX_CTRL_K_TABLE(5));
    IFX_REG_W32(0x28A9AA2B, TX_CTRL_K_TABLE(6));
    IFX_REG_W32(0xAC2D2EAF, TX_CTRL_K_TABLE(7));
    IFX_REG_W32(0x30B1B233, TX_CTRL_K_TABLE(8));
    IFX_REG_W32(0xB43536B7, TX_CTRL_K_TABLE(9));
    IFX_REG_W32(0xB8393ABB, TX_CTRL_K_TABLE(10));
    IFX_REG_W32(0x3CBDBE3F, TX_CTRL_K_TABLE(11));
    IFX_REG_W32(0xC04142C3, TX_CTRL_K_TABLE(12));
    IFX_REG_W32(0x44C5C647, TX_CTRL_K_TABLE(13));
    IFX_REG_W32(0x48C9CA4B, TX_CTRL_K_TABLE(14));
    IFX_REG_W32(0xCC4D4ECF, TX_CTRL_K_TABLE(15));

    //  init RX descriptor
    rx_desc.own     = 1;
    rx_desc.c       = 0;
    rx_desc.sop     = 1;
    rx_desc.eop     = 1;
    rx_desc.byteoff = RX_HEAD_MAC_ADDR_ALIGNMENT;
    rx_desc.datalen = RX_MAX_BUFFER_SIZE - RX_HEAD_MAC_ADDR_ALIGNMENT;
    for ( i = 0; i < WAN_RX_DESC_NUM; i++ ) {
        rx_desc.dataptr = (unsigned int)skb_pool[i]->data & 0x0FFFFFFF;
        WAN_RX_DESC_BASE[i] = rx_desc;
    }

    //  init TX descriptor
    tx_desc.own     = 0;
    tx_desc.c       = 0;
    tx_desc.sop     = 1;
    tx_desc.eop     = 1;
    tx_desc.byteoff = 0;
    tx_desc.qid     = 0;
    tx_desc.datalen = 0;
    tx_desc.small   = 0;
    tx_desc.dataptr = 0;
    for ( i = 0; i < CPU_TO_WAN_TX_DESC_NUM; i++ )
        CPU_TO_WAN_TX_DESC_BASE[i] = tx_desc;
    for ( i = 0; i < WAN_TX_DESC_NUM_TOTAL; i++ )
        WAN_TX_DESC_BASE(0)[i] = tx_desc;

    //  init Swap descriptor
    for ( i = 0; i < WAN_SWAP_DESC_NUM; i++ )
        WAN_SWAP_DESC_BASE[i] = tx_desc;

    //  init fastpath TX descriptor
    tx_desc.own     = 1;
    for ( i = 0; i < FASTPATH_TO_WAN_TX_DESC_NUM; i++ )
        FASTPATH_TO_WAN_TX_DESC_BASE[i] = tx_desc;

    return 0;

ALLOC_SKB_RX_FAIL:
    while ( i-- > 0 )
        dev_kfree_skb_any(skb_pool[i]);
    return -ENOMEM;
}

static INLINE void clear_tables(void)
{
    struct sk_buff *skb;
    int i, j;

    for ( i = 0; i < WAN_RX_DESC_NUM; i++ ) {
        skb = get_skb_pointer(WAN_RX_DESC_BASE[i].dataptr);
        if ( skb != NULL )
            dev_kfree_skb_any(skb);
    }

    for ( i = 0; i < CPU_TO_WAN_TX_DESC_NUM; i++ ) {
        skb = get_skb_pointer(CPU_TO_WAN_TX_DESC_BASE[i].dataptr);
        if ( skb != NULL )
            dev_kfree_skb_any(skb);
    }

    for ( j = 0; j < 8; j++ )
        for ( i = 0; i < WAN_TX_DESC_NUM; i++ ) {
            skb = get_skb_pointer(WAN_TX_DESC_BASE(j)[i].dataptr);
            if ( skb != NULL )
                dev_kfree_skb_any(skb);
        }

    for ( i = 0; i < WAN_SWAP_DESC_NUM; i++ ) {
        skb = get_skb_pointer(WAN_SWAP_DESC_BASE[i].dataptr);
        if ( skb != NULL )
            dev_kfree_skb_any(skb);
    }

    for ( i = 0; i < FASTPATH_TO_WAN_TX_DESC_NUM; i++ ) {
        skb = get_skb_pointer(FASTPATH_TO_WAN_TX_DESC_BASE[i].dataptr);
        if ( skb != NULL )
            dev_kfree_skb_any(skb);
    }
}



/*
 * ####################################
 *           Global Function
 * ####################################
 */

static int ptm_showtime_enter(struct port_cell_info *port_cell, void *xdata_addr)
{
    ASSERT(port_cell != NULL, "port_cell is NULL");
    ASSERT(xdata_addr != NULL, "xdata_addr is NULL");

    //  TODO: ReTX set xdata_addr
    g_xdata_addr = xdata_addr;

    g_showtime = 1;

    IFX_REG_W32(0x0F, UTP_CFG);

//#ifdef CONFIG_VR9
//    IFX_REG_W32_MASK(1 << 17, 0, FFSM_CFG0);
//#endif

    printk("enter showtime\n");

    return 0;
}

static int ptm_showtime_exit(void)
{
    if ( !g_showtime )
        return -EIO;

//#ifdef CONFIG_VR9
//    IFX_REG_W32_MASK(0, 1 << 17, FFSM_CFG0);
//#endif

    IFX_REG_W32(0x00, UTP_CFG);

    g_showtime = 0;

    //  TODO: ReTX clean state
    g_xdata_addr = NULL;

    printk("leave showtime\n");

    return 0;
}



/*
 * ####################################
 *           Init/Cleanup API
 * ####################################
 */

/*
 *  Description:
 *    Initialize global variables, PP32, comunication structures, register IRQ
 *    and register device.
 *  Input:
 *    none
 *  Output:
 *    0    --- successful
 *    else --- failure, usually it is negative value of error code
 */
static int __init ifx_ptm_init(void)
{
    int ret;
    struct port_cell_info port_cell = {0};
    int i;
    char ver_str[256];

    ret = init_priv_data();
    if ( ret != 0 ) {
        err("INIT_PRIV_DATA_FAIL");
        goto INIT_PRIV_DATA_FAIL;
    }

    ifx_ptm_init_chip();
    ret = init_tables();
    if ( ret != 0 ) {
        err("INIT_TABLES_FAIL");
        goto INIT_TABLES_FAIL;
    }

    for ( i = 0; i < NUM_ENTITY(g_net_dev); i++ ) {
        g_net_dev[i] = alloc_netdev(0, g_net_dev_name[i], ether_setup);
        if ( g_net_dev[i] == NULL )
            goto ALLOC_NETDEV_FAIL;
        ptm_setup(g_net_dev[i], i);
    }

    for ( i = 0; i < NUM_ENTITY(g_net_dev); i++ ) {
        ret = register_netdev(g_net_dev[i]);
        if ( ret != 0 )
            goto REGISTER_NETDEV_FAIL;
    }

    /*  register interrupt handler  */
    ret = request_irq(PPE_MAILBOX_IGU1_INT, mailbox_irq_handler, IRQF_DISABLED, "ptm_mailbox_isr", &g_ptm_priv_data);
    if ( ret ) {
        if ( ret == -EBUSY ) {
            err("IRQ may be occupied by other driver, please reconfig to disable it.");
        }
        else {
            err("request_irq fail");
        }
        goto REQUEST_IRQ_PPE_MAILBOX_IGU1_INT_FAIL;
    }
    disable_irq(PPE_MAILBOX_IGU1_INT);

    ret = ifx_pp32_start(0);
    if ( ret ) {
        err("ifx_pp32_start fail!");
        goto PP32_START_FAIL;
    }
    IFX_REG_W32(1 << 16, MBOX_IGU1_IER);    //  enable SWAP interrupt
    IFX_REG_W32(~0, MBOX_IGU1_ISRC);

    enable_irq(PPE_MAILBOX_IGU1_INT);

    ifx_mei_atm_showtime_check(&g_showtime, &port_cell, &g_xdata_addr);

#if defined(ENABLE_LED_FRAMEWORK) && ENABLE_LED_FRAMEWORK
    ifx_led_trigger_register("dsl_data", &g_data_led_trigger);
#endif

    proc_file_create();

    ifx_mei_atm_showtime_enter = ptm_showtime_enter;
    ifx_mei_atm_showtime_exit  = ptm_showtime_exit;

    ifx_ptm_version(ver_str);
    printk(KERN_INFO "%s", ver_str);

    printk("ifxmips_ptm: PTM init succeed\n");

    return 0;

PP32_START_FAIL:
    free_irq(PPE_MAILBOX_IGU1_INT, &g_ptm_priv_data);
REQUEST_IRQ_PPE_MAILBOX_IGU1_INT_FAIL:
    i = NUM_ENTITY(g_net_dev);
REGISTER_NETDEV_FAIL:
    while ( i-- )
        unregister_netdev(g_net_dev[i]);
    i = NUM_ENTITY(g_net_dev);
ALLOC_NETDEV_FAIL:
    while ( i-- ) {
        free_netdev(g_net_dev[i]);
        g_net_dev[i] = NULL;
    }
INIT_TABLES_FAIL:
INIT_PRIV_DATA_FAIL:
    clear_priv_data();
    printk("ifxmips_ptm: PTM init failed\n");
    return ret;
}

/*
 *  Description:
 *    Release memory, free IRQ, and deregister device.
 *  Input:
 *    none
 *  Output:
 *   none
 */
static void __exit ifx_ptm_exit(void)
{
    int i;

    ifx_mei_atm_showtime_enter = NULL;
    ifx_mei_atm_showtime_exit  = NULL;

    proc_file_delete();

#if defined(ENABLE_LED_FRAMEWORK) && ENABLE_LED_FRAMEWORK
    ifx_led_trigger_deregister(g_data_led_trigger);
    g_data_led_trigger = NULL;
#endif

    ifx_pp32_stop(0);

    free_irq(PPE_MAILBOX_IGU1_INT, &g_ptm_priv_data);

    for ( i = 0; i < NUM_ENTITY(g_net_dev); i++ )
        unregister_netdev(g_net_dev[i]);

    for ( i = 0; i < NUM_ENTITY(g_net_dev); i++ ) {
        free_netdev(g_net_dev[i]);
        g_net_dev[i] = NULL;
    }

    clear_tables();

    ifx_ptm_uninit_chip();

    clear_priv_data();
}

#ifndef MODULE
static int __init wanqos_en_setup(char *line)
{
    wanqos_en = simple_strtoul(line, NULL, 0);

    if ( wanqos_en < 1 || wanqos_en > 8 )
        wanqos_en = 0;

    return 0;
}

static int __init queue_gamma_map_setup(char *line)
{
    char *p;
    int i;

    for ( i = 0, p = line; i < NUM_ENTITY(queue_gamma_map) && isxdigit(*p); i++ )
    {
        queue_gamma_map[i] = simple_strtoul(p, &p, 0);
        if ( *p == ',' || *p == ';' || *p == ':' )
            p++;
    }

    return 0;
}
#endif

module_init(ifx_ptm_init);
module_exit(ifx_ptm_exit);
#ifndef MODULE
  __setup("wanqos_en=", wanqos_en_setup);
  __setup("queue_gamma_map=", queue_gamma_map_setup);
#endif

MODULE_LICENSE("GPL");
