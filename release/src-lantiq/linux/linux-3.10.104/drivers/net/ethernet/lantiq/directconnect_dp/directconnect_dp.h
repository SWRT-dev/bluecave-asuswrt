#ifndef _DIRECTCONNECT_DP_H_
#define _DIRECTCONNECT_DP_H_

#include <net/ltq_wave500_dp_api.h>

#define DC_DP_MAX_SUBIF_PER_DEV 16
#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
#define DC_DP_MAX_DEV_NUM       4
#else /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */
#define DC_DP_MAX_DEV_NUM       1
#endif /* #else */

#define DC_DP_LOCK    spin_lock_bh
#define DC_DP_UNLOCK  spin_unlock_bh

#define DC_DP_VAPID_OFFSET		8
#define DC_DP_VAPID_MASK		0xF
#define DC_DP_STAID_OFFSET		0
#define DC_DP_STAID_MASK		0xFF

#define DC_WIFI_CLASS_MASK		0x7

#define DC_DP_DBG_FLAG_DBG					0x1
#define DC_DP_DBG_FLAG_DUMP_RX_DATA			0x10
#define DC_DP_DBG_FLAG_DUMP_RX_DESCRIPTOR	0x20
#define DC_DP_DBG_FLAG_DUMP_RX_PMAC			0x40
#define DC_DP_DBG_FLAG_DUMP_RX  (DC_DP_DBG_FLAG_DUMP_RX_DATA |\
			      DC_DP_DBG_FLAG_DUMP_RX_DESCRIPTOR |\
			      DC_DP_DBG_FLAG_DUMP_RX_PMAC)
#define DC_DP_DBG_FLAG_DUMP_TX_DATA			0x100
#define DC_DP_DBG_FLAG_DUMP_TX_DESCRIPTOR	0x200
#define DC_DP_DBG_FLAG_DUMP_TX_PMAC			0x400
#define DC_DP_DBG_FLAG_DUMP_TX  (DC_DP_DBG_FLAG_DUMP_TX_DATA |\
			      DC_DP_DBG_FLAG_DUMP_TX_DESCRIPTOR |\
			      DC_DP_DBG_FLAG_DUMP_TX_PMAC)
#define DC_DP_DBG_FLAG_ERR					0x10000000

#define DC_DP_ERROR(fmt, args...) pr_err("DC DP [%s:%d] " fmt, __func__, __LINE__, ##args)

			//if (unlikely((g_dc_dp_dbg_flag & flags))) {
#if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG
	#define DC_DP_DEBUG(flags, fmt, args...) \
		do { \
			if ((g_dc_dp_dbg_flag & flags)) { \
				pr_info("DC DP [%s:%d] " fmt, __func__, __LINE__, ##args); \
			} \
		} while (0)
#else /* #if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG */
	#define DC_DP_DBG(level, fmt, args...)
#endif /* #else */

#if 0
/*! 
   \brief Macro that specifies network interface data structure
 */
typedef struct net_device NETIF;
#endif

#define STA_HASH_BITS 4
#define STA_HASH_SIZE (1 << STA_HASH_BITS)

#define DC_DP_UMT_PERIOD_DEFAULT     200 /* in micro second */

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

/*! Station detail information */
typedef struct dc_dp_priv_sta_entry {
	struct hlist_node hlist;
	struct rcu_head rcu;
	uint8_t mac_addr[ETH_ALEN];
	uint8_t sta_id;
	struct net_device *netif;
} dc_dp_priv_sta_entry_t;

typedef struct dc_dp_priv_sta_info {
	spinlock_t sta_hash_lock;
	struct hlist_head sta_hash[STA_HASH_SIZE];
} dc_dp_priv_sta_info_t;

/*! Sub interface detail information */
typedef struct dc_dp_priv_subif_info {
	int32_t flags;
#define DC_DP_SUBIF_FREE       0x0
#define DC_DP_SUBIF_REGISTERED 0x1
	uint32_t subif:15;
	struct net_device  *netif;   /*! pointer to  net_device*/
	char device_name[IFNAMSIZ];  /*! devide name, like wlan0, */
} dc_dp_priv_subif_info_t;

typedef struct dc_dp_priv_dev_info {
	int32_t flags;	/*! port flags */
#define DC_DP_DEV_FREE             0x0
#define DC_DP_DEV_PORT_ALLOCATED   0x1
#define DC_DP_DEV_REGISTERED       0x2
#define DC_DP_DEV_SUBIF_REGISTERED 0x3
	int32_t alloc_flags;       /* the flags saved when calling dc_dp_alloc_port */
	struct module *owner;
	struct net_device *dev;
	uint32_t dev_port;
	int32_t port_id;
	int32_t cbm_pid;
	int32_t umt_id;
	int32_t umt_period;
	dc_dp_cb_t cb;         /*! Callback Pointer to DIRECTPATH_CB */
	uint32_t num_subif;
	dc_dp_priv_subif_info_t subif_info[DC_DP_MAX_SUBIF_PER_DEV];
	uint32_t dma_ch;

	/* DMA1-TX ring buffer information */
	uint32_t num_bufpools;
    dc_dp_buf_pool_t *virt_buflist_base;
#ifndef SWAS_3_8
	void *buflist_virt_base;
#endif /* #ifndef SWAS_3_8 */

	/* WMM map information */
	uint8_t prio2wmm[DC_DP_MAX_WIFI_CLASS];
	uint8_t class2prio[16];
} dc_dp_priv_dev_info_t;

/* Extern variables */
extern spinlock_t g_dc_dp_lock;
extern dc_dp_priv_dev_info_t g_priv_info[DC_DP_MAX_DEV_NUM];
extern uint32_t g_dc_dp_dbg_flag;
extern dc_dp_priv_sta_info_t g_sta_info;
extern int32_t g_size_tx_buffers;

/* Extern functions */
extern int32_t
_dc_dp_register_dev_private(struct module *owner, uint32_t port_id,
							struct net_device *dev, dc_dp_cb_t *datapathcb,
							uint32_t num_bufs_req, void *umt_base, int32_t umt_len,
							dc_dp_res_t *resources, uint32_t flags);
#if IS_ENABLED(CONFIG_LTQ_PPA_API)
extern int ppa_br_fdb_delete(struct net_device *dev, const unsigned char *addr);
#endif /* #if IS_ENABLED(CONFIG_LTQ_PPA_API) */

extern int32_t _dc_dp_gsw_pmac_init(int32_t port_id, int32_t dma_cid);

int32_t dc_dp_sta_info_init(void);
void dc_dp_sta_info_exit(void);
dc_dp_priv_sta_entry_t *_dc_dp_find_sta_entry_by_macaddr(const uint8_t *macaddr);
int32_t _dc_dp_find_staid_by_macaddr_rcu(const uint8_t *macaddr, uint32_t *staid);
dc_dp_priv_sta_entry_t *_dc_dp_add_sta_entry(struct net_device *dev,
								const uint8_t *macaddr, const uint32_t sta_id);
void _dc_dp_delete_sta_entry(dc_dp_priv_sta_entry_t *sta_entry);

int32_t dc_dp_proc_init(void);
void dc_dp_proc_exit(void);


#endif /* _DIRECTCONNECT_DP_H_ */
