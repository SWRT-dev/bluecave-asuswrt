/* Includes */
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/jhash.h>
#include <linux/interrupt.h>

#include <lantiq_dmax.h>
#include <net/lantiq_cbm.h>
#include <net/lantiq_cbm_api.h>
#include <linux/ltq_hwmcpy.h>
#if IS_ENABLED(CONFIG_LTQ_PPA_API)
#include <net/ppa_api.h>
#endif /* #if IS_ENABLED(CONFIG_LTQ_PPA_API) */

#include "directconnect_dp.h"

/* Defines */
#define DRV_MODULE_NAME             "ltq_directconnect_datapath"
#define DRV_MODULE_VERSION          "1.3.1"

#define NEED_PMAC_HDR_REMOVAL 		1

/* Local variables */
spinlock_t g_dc_dp_lock;
static int32_t g_dc_dp_init_ok = 0;
uint32_t g_dc_dp_dbg_flag = 0x1;

int32_t g_size_tx_buffers = DC_DP_DMA_BUF_SIZE_DEFAULT;
dc_dp_priv_dev_info_t g_priv_info[DC_DP_MAX_DEV_NUM];

/* Function prototypes */
/* Callbacks */
#if NEED_PMAC_HDR_REMOVAL
static int32_t
dc_dp_rx_cb(struct net_device *rxif, struct net_device *txif,
				 struct sk_buff *skb, int32_t len);
#endif /* #if NEED_PMAC_HDR_REMOVAL */
static int32_t
dc_dp_check_if_netif_fastpath_cb(struct net_device *netif, char *ifname, uint32_t flags);
static int32_t
dc_dp_get_netif_subifid_cb(struct net_device *netif, struct sk_buff *skb,
								void *subif_data, uint8_t dst_mac[MAX_ETH_ALEN],
								dp_subif_t *subif, uint32_t flags);
#if IS_ENABLED(CONFIG_LTQ_PPA_API)
static int32_t
dc_dp_qos_class2prio_cb(int32_t port_id, struct net_device *netif, uint8_t *class2prio);
#endif /* #if IS_ENABLED(CONFIG_LTQ_PPA_API) */

/* Local */
static int32_t
_dc_dp_alloc_buf(int32_t dev_idx, uint32_t num_bufs_req, int32_t *num_bufpools, dc_dp_buf_pool_t **buflist);
static void _dc_dp_free_buf(int32_t dev_idx);
static int32_t _dc_dp_setup_dma(uint32_t chan, uint32_t desc_num);
static void _dc_dp_cleanup_dma(uint32_t chan);

static inline int32_t
_dc_dp_get_dev_idx_by_module_port(struct module *owner, int32_t port_id);
static int32_t _dc_dp_get_idx_via_dev_private(struct net_device *dev);
static int32_t _dc_dp_get_idx_via_ifname_private(char *ifname);
static inline int32_t _dc_dp_get_dev_idx_by_port(int32_t port_id);
#if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG
static void _dc_dp_dump_raw_data(char *buf, int len, char *prefix_str);
static void _dc_dp_dump_rx_pmac(struct pmac_rx_hdr *pmac);
#endif /* #if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG */
static inline uint8_t
_dc_dp_get_class2wmm(int32_t dev_idx, uint8_t class, bool is_class);

/* Extern */
extern void mcast_helper_register_module(struct net_device *dev, struct module *modName,
										 char *addl_name, dc_dp_mcast_callback_fn_t cb, unsigned int flags);

/*
 * ========================================================================
 * Local Interface API
 * ========================================================================
 */

static int32_t
_dc_dp_alloc_buf(int32_t dev_idx, uint32_t num_bufs_req, int32_t *num_bufpools, dc_dp_buf_pool_t **buflist)
{
	int32_t i;
	uint32_t order;
	uint32_t max_buf_pool_sz;
	uint32_t max_buf_pool_num;
	uint32_t max_bufs_req_sz;
	size_t virt_buflist_sz;
	size_t phys_buflist_sz;
	dc_dp_buf_pool_t *virt_buflist_base = NULL;
	dc_dp_buf_pool_t *phys_buflist_base = NULL;
	uint32_t num_buf_req_rem;
	uint32_t tmp_num_bufs_req;
	uint32_t tmp_buf_pool_sz;
	uint8_t *buf_addr_base = NULL;

	if (num_bufs_req <= 0) {
		return -1;
	}

	max_buf_pool_sz = 0x400000; //4MB : (2 ^ (MAX_ORDER - 1)) * PAGE_SIZE;
	max_buf_pool_num = 2048; //(max_buf_pool_sz / g_size_tx_buffers);
	max_bufs_req_sz = num_bufs_req * g_size_tx_buffers;

	*num_bufpools = (num_bufs_req + (max_buf_pool_num - 1)) / max_buf_pool_num;

	virt_buflist_sz = (*num_bufpools * sizeof(dc_dp_buf_pool_t)); /* virt buflist size */
	phys_buflist_sz = virt_buflist_sz; /* phys buflist size */

	/* Allocate Tx buffers */
	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Allocating %d DMA1-Tx buffer lists.\n", *num_bufpools);
	virt_buflist_base = (dc_dp_buf_pool_t *) kmalloc((virt_buflist_sz + phys_buflist_sz), GFP_KERNEL);
	if (!virt_buflist_base) {
		DC_DP_ERROR("failed to allocate %d buffer lists!!!\n", *num_bufpools);
		return -ENOMEM;
	}
	memset((void *)virt_buflist_base, 0, (virt_buflist_sz + phys_buflist_sz));

	g_priv_info[dev_idx].num_bufpools = *num_bufpools;
	g_priv_info[dev_idx].virt_buflist_base = (void *)virt_buflist_base;

	phys_buflist_base = (dc_dp_buf_pool_t *)((uint8_t *)virt_buflist_base + virt_buflist_sz);
	*buflist = phys_buflist_base;
	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "buflist=0x%p.\n", *buflist);

	num_buf_req_rem = num_bufs_req;
	for (i = 0; i < *num_bufpools; i++) {
		tmp_num_bufs_req = MIN(num_buf_req_rem, max_buf_pool_num);
		tmp_buf_pool_sz = tmp_num_bufs_req * g_size_tx_buffers;
		order = get_order(tmp_buf_pool_sz);

		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "allocating pool %d of size %d KB.\n",
					(i + 1), (tmp_buf_pool_sz >> 10));
		buf_addr_base = (uint8_t *)__get_free_pages(GFP_KERNEL, order);
		if (!buf_addr_base) {
			DC_DP_ERROR("failed to allocate pool %d of size %d bytes!!!\n",
							(i + 1), tmp_buf_pool_sz);
			goto err_out_free_buf;
		}

		/* Virtual buffer pool */
		g_priv_info[dev_idx].virt_buflist_base[i].pool = (void *)buf_addr_base;
		g_priv_info[dev_idx].virt_buflist_base[i].size = tmp_buf_pool_sz;

		/* Physical buffer pool */
		phys_buflist_base[i].pool = (void *)virt_to_phys(buf_addr_base);
		phys_buflist_base[i].size = tmp_buf_pool_sz;

		num_buf_req_rem -= tmp_num_bufs_req;
	}

	return 0;

err_out_free_buf:
	_dc_dp_free_buf(dev_idx);

	return -ENOMEM;
}

static void
_dc_dp_free_buf(int32_t dev_idx)
{
	int32_t i;
	uint32_t order;

	/* De-allocate Tx buffer pool */
	for (i = 0; i < g_priv_info[dev_idx].num_bufpools; i++) {
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "De-allocating pool %d (0x%p) of size %d bytes.\n",
						(i + 1), g_priv_info[dev_idx].virt_buflist_base[i].pool, g_priv_info[dev_idx].virt_buflist_base[i].size);
		if (g_priv_info[dev_idx].virt_buflist_base[i].pool) {
   			order = get_order(g_priv_info[dev_idx].virt_buflist_base[i].size);
			free_pages((unsigned long)g_priv_info[dev_idx].virt_buflist_base[i].pool, order);
			g_priv_info[dev_idx].virt_buflist_base[i].pool = NULL;
			g_priv_info[dev_idx].virt_buflist_base[i].size = 0;
		}
	}

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "De-allocating %d buffer lists.\n", g_priv_info[dev_idx].num_bufpools);
	if (g_priv_info[dev_idx].virt_buflist_base) {
		kfree(g_priv_info[dev_idx].virt_buflist_base);
		g_priv_info[dev_idx].virt_buflist_base = NULL;
	}

	g_priv_info[dev_idx].num_bufpools = 0;
}

static int32_t
_dc_dp_setup_dma(uint32_t chan, uint32_t desc_num)
{
	int32_t ret;

	/* Allocate DMA1-TX DMA channel */
	ret = ltq_request_dma(chan, "dma1 tx wlan");
	if (ret) {
		DC_DP_ERROR("failed to allocate DMA1-TX DMA channel 0x%x!!!\n", chan);
		return ret;
	}

	/* Initialize DMA1-TX DMA channel descriptors */
	ret = ltq_dma_chan_desc_alloc(chan, desc_num);
	if (ret) {
		DC_DP_ERROR("failed to allocate %d descriptors for DMA1-TX DMA channel 0x%x!!!\n", desc_num, chan);
		ltq_free_dma(chan);
		return ret;
	}

	return 0;
}

static void
_dc_dp_cleanup_dma(uint32_t chan)
{
	/* Free the DMA1-TX DMA channel descriptors */
	if (ltq_dma_chan_desc_free(chan)) {
		DC_DP_ERROR("failed to free descriptors for DMA1-TX DMA channel 0x%x!!!\n", chan);
	}

	/* Free DMA1-TX DMA channel */
	if (ltq_free_dma(chan)) {
		DC_DP_ERROR("failed to free DMA1-TX DMA channel 0x%x!!!\n", chan);
	}
}

static inline int32_t
_dc_dp_get_dev_idx_by_module_port(struct module *owner, int32_t port_id)
{
	int32_t i;

	for (i = 0; i < DC_DP_MAX_DEV_NUM; i++) {
		if (g_priv_info[i].flags == DC_DP_DEV_FREE)
			continue;

		if (g_priv_info[i].owner == owner &&
			g_priv_info[i].port_id == port_id) {
			return i;
		}
	}

	return -1;
}

static int32_t
_dc_dp_get_idx_via_dev_private(struct net_device *dev)
{
	int32_t i, j;

	for (i = 0; i < DC_DP_MAX_DEV_NUM; i++) {
		if (g_priv_info[i].flags != DC_DP_DEV_SUBIF_REGISTERED)
			continue;

		for (j = 0; j < DC_DP_MAX_SUBIF_PER_DEV; j++) {
			if (g_priv_info[i].subif_info[j].flags == DC_DP_SUBIF_FREE)
				continue;

			if (g_priv_info[i].subif_info[j].netif == dev) {
				return i;
			}
		}
	}

	return DP_FAILURE;
}

static int32_t
_dc_dp_get_idx_via_ifname_private(char *ifname)
{
	int32_t i, j;

	for (i = 0; i < DC_DP_MAX_DEV_NUM; i++) {
		if (g_priv_info[i].flags != DC_DP_DEV_SUBIF_REGISTERED)
			continue;

		for (j = 0; j < DC_DP_MAX_SUBIF_PER_DEV; j++) {
			if (g_priv_info[i].subif_info[j].flags == DC_DP_SUBIF_FREE)
				continue;

			if (!strncmp(g_priv_info[i].subif_info[j].device_name, ifname, IFNAMSIZ)) {
				return i;
			}
		}
	}

	return DP_FAILURE;
}

static inline int32_t
_dc_dp_get_dev_idx_by_port(int32_t port_id)
{
	int32_t i;

	for (i = 0; i < DC_DP_MAX_DEV_NUM; i++) {
		if (g_priv_info[i].flags == DC_DP_DEV_FREE)
			continue;

		if (g_priv_info[i].port_id == port_id) {
			return i;
		}
	}

	return -1;
}

#if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG
static void
_dc_dp_dump_raw_data(char *buf, int len, char *prefix_str) 
{
	int i;
	int r;
	int line_num = 32;
	unsigned char *p = (unsigned char *)buf;

	if (!p) {
		pr_err("[%s:%d] p NULL ?\n", __func__, __LINE__);
		return ;
	}

	printk("%s in hex (len = %d) at 0x%p\n", prefix_str ? (char *)prefix_str : "Data", len, p);
	for (i = 0; i < len; i++) {
		r = i % line_num;
		if (r== 0)
			printk(" %04d: ", i);
		else if (r == (line_num/2))
			printk(" "); /*inser seperator*/
		printk("%02x ", p[i]);
		if (r == (line_num - 1))
			printk("\n"); /*insert new line */
	}
	printk("\n");
}

static void
_dc_dp_dump_rx_pmac(struct pmac_rx_hdr *pmac)
{
	int i;
	unsigned char *p = (char *)pmac;

	if (!pmac) {
		pr_err("dump_rx_pmac pmac NULL ??\n");
		return ;
	}
	
	pr_info("PMAC at 0x%p: ", p);
	for (i = 0; i < 8; i++)
		pr_info("0x%02x ", p[i]);
	pr_info("\n");
	
	/*byte 0 */
	pr_info("  byte 0:res=%d ver_done=%d ip_offset=%d\n", pmac->res1,
	       pmac->ver_done, pmac->ip_offset);
	/*byte 1 */
	pr_info("  byte 1:tcp_h_offset=%d tcp_type=%d\n", pmac->tcp_h_offset,
	       pmac->tcp_type);
	/*byte 2 */
	pr_info("  byte 2:ppid=%d class=%d\n", pmac->sppid, pmac->class);
	/*byte 3 */
	pr_info("  byte 3:res=%d pkt_type=%d\n", pmac->res2, pmac->pkt_type);
	/*byte 4 */
	pr_info("  byte 4:res=%d redirect=%d res2=%d src_sub_inf_id=%d\n",
	       pmac->res3, pmac->redirect, pmac->res4, pmac->src_sub_inf_id);
	/*byte 5 */
	pr_info("  byte 5:src_sub_inf_id2=%d\n", pmac->src_sub_inf_id2);
	/*byte 6 */
	pr_info("  byte 6:port_map=%d\n", pmac->port_map);
	/*byte 7 */
	pr_info("  byte 7:port_map2=%d\n", pmac->port_map2);
}
#endif /* #if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG */

static inline uint8_t
_dc_dp_get_class2wmm(int32_t dev_idx, uint8_t class, bool is_class)
{
	uint8_t wmm;
	uint8_t prio;

	if (is_class) {
		class = (class & 0x0F);
		prio = g_priv_info[dev_idx].class2prio[class];
	} else {
		prio = class;
	}

	prio = (prio & DC_WIFI_CLASS_MASK);
	wmm = g_priv_info[dev_idx].prio2wmm[prio];

	return wmm;
}

/*
 * ========================================================================
 * Direct Connect Driver Interface API
 * ========================================================================
 */
int32_t
dc_dp_alloc_port(struct module *owner, uint32_t dev_port,
				 struct net_device *dev, int32_t port_id, uint32_t flags)
{
	int32_t ret;
	int32_t i;
	int32_t dev_idx;
	int32_t ep_id;
	uint32_t cbm_pid = 0;
	uint32_t dma_ctrlid = 0;
	uint32_t dma_cid = 0;
	uint32_t umt_id = 0;

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "owner=%p, dev_port=%u, dev=%p, port_id=%d, flags=0x%08X\n",
					owner, dev_port, dev, port_id, flags);

	/* Validate input arguments */
	if (!owner) {
		DC_DP_ERROR("failed to allocate/de-allocate port as owner=%p!!!\n", owner);
		return DP_FAILURE;
	}

	/* De-register */
	if (flags & DP_F_DEREGISTER) {
		DC_DP_LOCK(&g_dc_dp_lock);

		/* Find the registered device index */
		dev_idx = _dc_dp_get_dev_idx_by_module_port(owner, port_id);
		if (dev_idx < 0) {
			DC_DP_ERROR("failed to de-allocate port_id=%d as port is not allocated yet!!!\n", port_id);
			DC_DP_UNLOCK(&g_dc_dp_lock);
			return DP_FAILURE;
		}

		if (g_priv_info[dev_idx].flags != DC_DP_DEV_PORT_ALLOCATED) {
			DC_DP_ERROR("failed to de-allocate port_id=%d as it is in unknown state!!!\n", port_id);
			DC_DP_UNLOCK(&g_dc_dp_lock);
			return DP_FAILURE;
		}

#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
		/* Release UMT port */
		ret = ltq_umt_release(g_priv_info[dev_idx].umt_id, port_id);
		if (ret < 0) {
			DC_DP_ERROR("failed to release umt_id=%d, port_id=%d!!!\n", g_priv_info[dev_idx].umt_id, port_id);
			DC_DP_UNLOCK(&g_dc_dp_lock);
			return DP_FAILURE;
		}
#endif /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */

		ret = dp_alloc_port(owner, dev, dev_port, port_id, NULL, flags);
		if (ret != DP_SUCCESS) {
		    DC_DP_ERROR("failed to de-allocate port_id=%d from datapath library/core!!!\n", port_id);
			DC_DP_UNLOCK(&g_dc_dp_lock);
			return DP_FAILURE;
		}

		memset(&g_priv_info[dev_idx], 0, sizeof(dc_dp_priv_dev_info_t));

		DC_DP_UNLOCK(&g_dc_dp_lock);

		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Success, returned %d\n", DP_SUCCESS);
		return DP_SUCCESS;
	}

	DC_DP_LOCK(&g_dc_dp_lock);

	/* Find a free device index */
	for (i = 0; i < DC_DP_MAX_DEV_NUM; i++) {
		if (g_priv_info[i].flags == DC_DP_DEV_FREE) {
			break;
		}
	}

	if (i >= DC_DP_MAX_DEV_NUM) {
        DC_DP_ERROR("failed to allocate port as it reaches maximum directconnect device limit - %d!!!\n", DC_DP_MAX_DEV_NUM);
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return DP_FAILURE;
	}
	dev_idx = i;

	ret = dp_alloc_port(owner, dev, dev_port, port_id, NULL, flags);
	if (ret == DP_FAILURE) {
		DC_DP_ERROR("failed to allocate port in datapath library/core!!!\n");
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return DP_FAILURE;
	}
	ep_id = ret;

	ret = cbm_get_wlan_umt_pid(ep_id, &cbm_pid);
	if (ret < 0) {
		DC_DP_ERROR("failed to acquire CBM pid!!!\n");
		goto err_out_free_port;
	}

#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
	/* Request for UMT port */
	ret = ltq_umt_request(ep_id, cbm_pid, &dma_ctrlid, &dma_cid, &umt_id);
	if (ret < 0) {
		DC_DP_ERROR("failed to allocate umt port for ep_id=%d, cbm_pid=%d!!!\n", ep_id, cbm_pid);
		goto err_out_free_port;
	}
#else /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */
	dma_ctrlid = DMA1TX;
	dma_cid = DMA_CHANNEL_7;
	umt_id = 0;
#endif /* #else */

	/* Configure GSWIP-R PMAC port */
    if (_dc_dp_gsw_pmac_init(ep_id, dma_cid) < 0) {
		DC_DP_ERROR("failed to configure GSWIP-R PMAC port %d!!!\n", ep_id);
		goto err_out_release_umt;
	}

	memset(&g_priv_info[dev_idx], 0, sizeof(dc_dp_priv_dev_info_t));
	g_priv_info[dev_idx].owner = owner;
	g_priv_info[dev_idx].dev = dev;
	g_priv_info[dev_idx].dev_port = dev_port;
	g_priv_info[dev_idx].port_id = ep_id;
	g_priv_info[dev_idx].cbm_pid = cbm_pid;
	g_priv_info[dev_idx].dma_ch = _DMA_C(dma_ctrlid, DMA1TX_PORT, dma_cid);
	g_priv_info[dev_idx].umt_id = umt_id;
	g_priv_info[dev_idx].alloc_flags = flags;
	g_priv_info[dev_idx].flags = DC_DP_DEV_PORT_ALLOCATED;

	DC_DP_UNLOCK(&g_dc_dp_lock);

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Success, returned port_id=%d.\n", ep_id);
	return ep_id;

err_out_release_umt:
#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
	ltq_umt_release(umt_id, ep_id);
#endif /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */

err_out_free_port:
	dp_alloc_port(owner, dev, dev_port, ep_id, NULL, DP_F_DEREGISTER);

	DC_DP_UNLOCK(&g_dc_dp_lock);

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Failure, returned %d.\n", DP_FAILURE);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_alloc_port);

int32_t
_dc_dp_register_dev_private(struct module *owner, uint32_t port_id,
							struct net_device *dev, dc_dp_cb_t *datapathcb,
							uint32_t num_bufs_req, void *umt_base, int32_t umt_len,
							dc_dp_res_t *resources, uint32_t flags)
{
	int32_t ret;
	int32_t i;
	int32_t dev_idx;
	dp_cb_t dp_cb = {0};
	int32_t num_bufpools = 0;
	dc_dp_buf_pool_t *buflist = NULL;
	cbm_dq_port_res_t cbm_res = {0};

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "owner=%p, port_id=%u, dev=%p, datapathcb=%p, \
					num_bufs_req=%d, umt_base=%p, umt_len=%d, resources=%p, flags=0x%08X\n",
					owner, port_id, dev, datapathcb, num_bufs_req, umt_base, umt_len, resources, flags);

	/* Find the device index */
	dev_idx = _dc_dp_get_dev_idx_by_module_port(owner, port_id);
	if (dev_idx < 0) {
		DC_DP_ERROR("failed to register/de-register dev, as port_id=%d is not allocated yet!!!\n", port_id);
		return DP_FAILURE;
	}

	/* De-register */
	if (flags & DP_F_DEREGISTER) {
		if (g_priv_info[dev_idx].flags < DC_DP_DEV_REGISTERED) {
			DC_DP_ERROR("failed to de-register dev as port_id=%d is in unknown state!!!\n", port_id);
			return DP_FAILURE;
		}

		if (g_priv_info[dev_idx].num_subif > 0) {
			/* De-register subif from Datapath Library/Core, if any */
			dp_subif_t subif_id = {0};
			for (i = 0; i < DC_DP_MAX_SUBIF_PER_DEV; i++) {
				if (g_priv_info[dev_idx].subif_info[i].flags == DC_DP_SUBIF_REGISTERED) {
					subif_id.port_id = port_id;
					subif_id.subif = g_priv_info[dev_idx].subif_info[i].subif;

					ret = dp_register_subif(owner, dev, dev->name, &subif_id, flags);
					if (ret != DP_SUCCESS) {
						DC_DP_ERROR("failed to de-register subif=0x%x!!!\n", subif_id.subif);
					}

					memset(&g_priv_info[dev_idx].subif_info[i], 0, sizeof(dc_dp_priv_subif_info_t));
					g_priv_info[dev_idx].num_subif--;
				}
			}

#if IS_ENABLED(CONFIG_LTQ_PPA_API)
			if (ppa_register_qos_class2prio_hook_fn) {
				ret = ppa_register_qos_class2prio_hook_fn(port_id, dev, NULL, WMM_QOS_DEV_F_DREG);
				if (ret) {
					DC_DP_ERROR("failed to de-register class2prio callback from PPA!!!\n");
				}
			}
#endif /* #if IS_ENABLED(CONFIG_LTQ_PPA_API) */

			/* Disable DMA1-TX DMA channel */
			ret = ltq_dma_chan_close(g_priv_info[dev_idx].dma_ch);
			if (ret != DP_SUCCESS) {
				DC_DP_ERROR("failed to close DMA channel 0x%x!!!\n", g_priv_info[dev_idx].dma_ch);
			}

			/* Disable UMT HW */
			DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Disabling UMT HW.\n");
#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
			ret = ltq_umt_enable(g_priv_info[dev_idx].umt_id, port_id, 0);
#else /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */
			ret = ltq_umt_enable(0);
#endif /* #else */

			g_priv_info[dev_idx].flags = DC_DP_DEV_REGISTERED;
		}

		ret = dp_register_dev(owner, port_id, &dp_cb, flags);
		if (ret != DP_SUCCESS) {
			DC_DP_ERROR("failed to de-register device for the port_id %d!!!\n", port_id);
		}

		/* Cleanup DMA1-TX DMA Channel */
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "De-configuring DMA1-Tx channel 0x%x.\n",
					g_priv_info[dev_idx].dma_ch);
		_dc_dp_cleanup_dma(g_priv_info[dev_idx].dma_ch);

		/* De-allocate Tx buffer pool */
		_dc_dp_free_buf(dev_idx);

		g_priv_info[dev_idx].flags = DC_DP_DEV_PORT_ALLOCATED;

		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Success, returned %d.\n", DP_SUCCESS);
		return DP_SUCCESS;
	}

	if (!datapathcb && !resources) {
		DC_DP_ERROR("failed to register dev as datapathcb=%p, resources=%p!!!\n", datapathcb, resources);
		return DP_FAILURE;
	}

	if (g_priv_info[dev_idx].flags != DC_DP_DEV_PORT_ALLOCATED) {
		DC_DP_ERROR("failed to register dev as port_id=%d is in unknown state!!!\n", port_id);
		return DP_FAILURE;
	}

	/* Datapath Library callback registration */
#if NEED_PMAC_HDR_REMOVAL
	dp_cb.rx_fn = dc_dp_rx_cb;
#else /* #if NEED_PMAC_HDR_REMOVAL */
	dp_cb.rx_fn = datapathcb->rx_fn;
#endif /* #else */
	dp_cb.stop_fn = datapathcb->stop_fn;
	dp_cb.restart_fn = datapathcb->restart_fn;
	dp_cb.get_subifid_fn = dc_dp_get_netif_subifid_cb;
	dp_cb.reset_mib_fn = datapathcb->reset_mib_fn;
	dp_cb.get_mib_fn = datapathcb->get_mib_fn;

	ret = dp_register_dev(owner, port_id, &dp_cb, flags);
	if (ret != DP_SUCCESS) {
		DC_DP_ERROR("failed to register dev to Datapath Library/Core!!!\n");
		return DP_FAILURE;
	}

	/* Allocate Tx buffers */
	ret = _dc_dp_alloc_buf(dev_idx, num_bufs_req, &num_bufpools, &buflist);
    if (ret) {
		DC_DP_ERROR("failed to register dev as tx buffer allocation failure!!!\n");
	    dp_register_dev(owner, port_id, &dp_cb, DP_F_DEREGISTER);
		return DP_FAILURE;
	}

	resources->num_bufpools = num_bufpools;
	resources->buflist = buflist;

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Configuring DMA1-Tx channel 0x%x.\n",
				g_priv_info[dev_idx].dma_ch);
	/* Configure DMA1-TX DMA Channel */
	ret = _dc_dp_setup_dma(g_priv_info[dev_idx].dma_ch, num_bufs_req);
	if (ret != DP_SUCCESS) {
		DC_DP_ERROR("failed to configure DMA1-TX DMA channel 0x%x!!!\n",
						g_priv_info[dev_idx].dma_ch);
		goto err_out_free_buf;
	}

	/* Update returned 'resource' structure */
	resources->num_dma_desc = num_bufs_req;
	resources->dmatx_ring_base = (void *)ltq_dma_chan_get_desc_phys_base(g_priv_info[dev_idx].dma_ch);

	/* UMT */
	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Configuring UMT HW with dest address 0x%p.\n",
				(uint32_t *)umt_base);
#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
	ret = ltq_umt_set_mode(g_priv_info[dev_idx].umt_id, port_id, 0, 3, (uint32_t)umt_base, DC_DP_UMT_PERIOD_DEFAULT, 0);
#else /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */
	ret = ltq_umt_set_mode(0, 3, (uint32_t)umt_base, DC_DP_UMT_PERIOD_DEFAULT, 0);
#endif /* #else */
	if (ret) {
		DC_DP_ERROR("failed to configure UMT transfer!!!\n");
		goto err_out_free_dma;
	}

	/* CBM Resources */
	ret = cbm_dequeue_port_resources_get(port_id, &cbm_res, 0);
	if (ret != CBM_OK) {
		DC_DP_ERROR("failed to get CBM dequeue port resources for the port %d!!!", port_id);
		goto err_out_free_dma;
	}

#if 0
#if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG
	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "cbm_res.cbm_buf_free_base = 0x%p.\n", cbm_res.cbm_buf_free_base);
	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "cbm_res.num_free_entries = %d.\n", cbm_res.num_free_entries);
	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "cbm_res.num_deq_ports = %d.\n", cbm_res.num_deq_ports);
#endif /* #if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG */
#endif

	resources->cbm_buf_free_base = (void *)RPHYSADDR((uint32_t)cbm_res.cbm_buf_free_base);
	resources->num_free_bufs = 32; // FIXME : cbm_res.num_free_entries;
	if (cbm_res.num_deq_ports) {
#if 0
#if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "cbm_res.deq_info[0].port_no = %d.\n",
					cbm_res.deq_info[0].port_no);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "cbm_res.deq_info[0].cbm_dq_port_base = 0x%p.\n",
					cbm_res.deq_info[0].cbm_dq_port_base);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "cbm_res.deq_info[0].dma_tx_chan = %d.\n",
					cbm_res.deq_info[0].dma_tx_chan);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "cbm_res.deq_info[0].num_desc = %d.\n",
					cbm_res.deq_info[0].num_desc);
#endif /* #if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG */
#endif

		resources->cbm_dequeue_base = (void *)RPHYSADDR((uint32_t)cbm_res.deq_info[0].cbm_dq_port_base);
		resources->num_dequeue_bufs = cbm_res.deq_info[0].num_desc;

		kfree(cbm_res.deq_info);
	}

#if IS_ENABLED(CONFIG_LTQ_PPA_API)
	if (ppa_register_qos_class2prio_hook_fn) {
		DC_DP_UNLOCK(&g_dc_dp_lock);
		ret = ppa_register_qos_class2prio_hook_fn(port_id, dev, dc_dp_qos_class2prio_cb, WMM_QOS_DEV_F_REG);
		if (ret) {
			DC_DP_ERROR("failed to register class2prio callback to PPA!!!\n");
			DC_DP_LOCK(&g_dc_dp_lock);
			goto err_out_free_dma;
		}
		DC_DP_LOCK(&g_dc_dp_lock);
	} else {
		for (i = 0; i < 16; i++) {
			g_priv_info[dev_idx].class2prio[i] = (i >> 1);
		}
	}
#endif /* #if IS_ENABLED(CONFIG_LTQ_PPA_API) */

	/* Keep the callback registered by WLAN driver */
	g_priv_info[dev_idx].cb = *datapathcb;
	g_priv_info[dev_idx].umt_period = DC_DP_UMT_PERIOD_DEFAULT;
	g_priv_info[dev_idx].flags = DC_DP_DEV_REGISTERED;

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Success, returned %d.\n", ret);
	return ret;

err_out_free_dma:
	/* Cleanup DMA Tx Channel */
	_dc_dp_cleanup_dma(g_priv_info[dev_idx].dma_ch);

err_out_free_buf:
	/* De-allocate Tx buffers */
	_dc_dp_free_buf(dev_idx);

	dp_register_dev(owner, port_id, &dp_cb, DP_F_DEREGISTER);

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Failure, returned %d.\n", DP_FAILURE);
	return DP_FAILURE;
}

int32_t
dc_dp_register_dev(struct module *owner, uint32_t port_id,
				   struct net_device *dev, dc_dp_cb_t *datapathcb,
				   uint32_t num_bufs_req, void *umt_base, int32_t umt_len,
				   dc_dp_res_t *resources, uint32_t flags)
{
	int32_t ret;

	DC_DP_LOCK(&g_dc_dp_lock);
	ret = _dc_dp_register_dev_private(owner, port_id, dev, datapathcb, num_bufs_req, umt_base, umt_len, resources, flags);
	DC_DP_UNLOCK(&g_dc_dp_lock);

#if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG
	if ( !(flags & DP_F_DEREGISTER) ) {
		int32_t i;

		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->num_bufpools = %d\n", resources->num_bufpools);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->buflist = %p\n", resources->buflist);
		for (i = 0; i < resources->num_bufpools; i++) {
			DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Bufpool %d : pool = %p of size %d\n",
							(i + 1), resources->buflist[i].pool, resources->buflist[i].size);
		}
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->num_dma_desc = %d\n", resources->num_dma_desc);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->dmatx_ring_base = 0x%p\n", resources->dmatx_ring_base);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->cbm_dequeue_base = 0x%p.\n", resources->cbm_dequeue_base);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->num_dequeue_bufs = %d.\n", resources->num_dequeue_bufs);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->cbm_buf_free_base = 0x%p.\n", resources->cbm_buf_free_base);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->num_free_bufs = %d.\n", resources->num_free_bufs);
	}
#endif /* #if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG */

	return ret;
}
EXPORT_SYMBOL(dc_dp_register_dev);

int32_t
dc_dp_register_subif(struct module *owner, struct net_device *dev,
					 dp_subif_t *subif_id, uint32_t flags)
{
	int32_t ret;
	int32_t dev_idx;
	int32_t subif_idx;

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "owner=%p, dev=%p, subif_id=%p, flags=0x%08X\n",
					owner, dev, subif_id, flags);

	if (!owner) {
		DC_DP_ERROR("failed to register/de-register subif as owner=%p!!!\n", owner);
		return DP_FAILURE;
	}

	if (!dev || !subif_id) {
		DC_DP_ERROR("failed to register/de-register subif as dev=%p or subif_id=%p!!!\n", dev, subif_id);
		return DP_FAILURE;
	}

	DC_DP_LOCK(&g_dc_dp_lock);

	/* Find the registered device index */
	dev_idx = _dc_dp_get_dev_idx_by_module_port(owner, subif_id->port_id);
	if (dev_idx < 0) {
		DC_DP_ERROR("failed to register/de-register subif as port_id=%d is not allocated yet!!!\n", subif_id->port_id);
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return DP_FAILURE;
	}

	/* De-register */
	if (flags & DP_F_DEREGISTER) {

		if (g_priv_info[dev_idx].flags != DC_DP_DEV_SUBIF_REGISTERED) {
		    DC_DP_ERROR("failed to de-register subif as port_id=%d is in unknown state!!!\n", subif_id->port_id);
			DC_DP_UNLOCK(&g_dc_dp_lock);
			return DP_FAILURE;
		}

		/* Find the registered subif index (for multiple vap) */
		subif_idx = ((subif_id->subif >> DC_DP_VAPID_OFFSET) & DC_DP_VAPID_MASK);
		if (g_priv_info[dev_idx].subif_info[subif_idx].flags != DC_DP_SUBIF_REGISTERED) {
		    DC_DP_ERROR("failed to de-register subif as subif_id=%d is in unknown state!!!\n", subif_idx);
			DC_DP_UNLOCK(&g_dc_dp_lock);
			return DP_FAILURE;
		}

		/* De-register subif from Datapath Library/Core */
		ret = dp_register_subif(owner, dev, dev->name, subif_id, flags);
		if (ret != DP_SUCCESS) {
		    DC_DP_ERROR("failed to de-register subif from Datapath Library/Core!!!\n");
			DC_DP_UNLOCK(&g_dc_dp_lock);
			return DP_FAILURE;
		}

		/* Reset private structure corresponding to the subif */
		memset(&g_priv_info[dev_idx].subif_info[subif_idx], 0, sizeof(dc_dp_priv_subif_info_t));

		g_priv_info[dev_idx].num_subif--;

		/* For the last subif, disable DMA1-TX DMA channel */
		if (g_priv_info[dev_idx].num_subif == 0) {
			if (ltq_dma_chan_close(g_priv_info[dev_idx].dma_ch)) {
				DC_DP_ERROR("failed to close dma1-tx dma channel 0x%x!!!\n", g_priv_info[dev_idx].dma_ch);
			}

			/* Disable UMT hw */
			DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Disabling UMT.\n");
#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
			if (ltq_umt_enable(g_priv_info[dev_idx].umt_id, g_priv_info[dev_idx].port_id, 0)) {
				DC_DP_ERROR("failed to disable umt_id=%d, port_id=%d!!!\n",
								g_priv_info[dev_idx].umt_id, g_priv_info[dev_idx].port_id);
			}
#else /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */
			if (ltq_umt_enable(0)) {
				DC_DP_ERROR("failed to disable UMT!!!\n");
			}
#endif /* #else */

			g_priv_info[dev_idx].flags = DC_DP_DEV_REGISTERED;
		}

		DC_DP_UNLOCK(&g_dc_dp_lock);

		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Success, returned %d.\n", DP_SUCCESS);
		return DP_SUCCESS;
	}

	if (g_priv_info[dev_idx].flags < DC_DP_DEV_REGISTERED) {
		DC_DP_ERROR("failed to register subif as port_id=%d is in unknown state!!!\n", subif_id->port_id);
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return DP_FAILURE;
	}

	/* Register subif to Datapath Library/Core */
	ret = dp_register_subif(owner, dev, dev->name, subif_id, flags);
	if (ret != DP_SUCCESS) {
		DC_DP_ERROR("failed to register subif to Datapath Library/Core!!!\n");
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return DP_FAILURE;
	}

	/* Initialize private structure with device and subif information */
	subif_idx = ((subif_id->subif >> DC_DP_VAPID_OFFSET) & DC_DP_VAPID_MASK);
	if (g_priv_info[dev_idx].subif_info[subif_idx].flags != DC_DP_SUBIF_FREE) {
		DC_DP_ERROR("failed to find free subif entry for the subif_id=%d!!!\n", subif_idx);
		goto err_out_deregister_subif;
	}

	/* For the first subif, enable DMA1-TX DMA channel */
	if (g_priv_info[dev_idx].num_subif == 0) {
		/* Enable UMT hw */
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Enabling UMT HW.\n");
#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
		ret = ltq_umt_enable(g_priv_info[dev_idx].umt_id, g_priv_info[dev_idx].port_id, 1);
#else /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */
		ret = ltq_umt_enable(1);
#endif /* #else */
		if (ret) {
	        DC_DP_ERROR("failed to enable umt_id=%d, port_id=%d!!!\n",
							g_priv_info[dev_idx].umt_id, g_priv_info[dev_idx].port_id);
			goto err_out_deregister_subif;
		}

		/* Enable DMA1-TX DMA channel */
		if (ltq_dma_chan_open(g_priv_info[dev_idx].dma_ch)) {
			DC_DP_ERROR("failed to open dma1-tx dma channel=0x%x!!!\n", g_priv_info[dev_idx].dma_ch);
			goto err_out_disable_umt;
		}
	}

	g_priv_info[dev_idx].subif_info[subif_idx].flags = DC_DP_SUBIF_REGISTERED;
	g_priv_info[dev_idx].subif_info[subif_idx].netif = dev;
	g_priv_info[dev_idx].subif_info[subif_idx].subif = subif_id->subif;
	strcpy(g_priv_info[dev_idx].subif_info[subif_idx].device_name, dev->name);

	g_priv_info[dev_idx].flags = DC_DP_DEV_SUBIF_REGISTERED;
	g_priv_info[dev_idx].num_subif++;

	DC_DP_UNLOCK(&g_dc_dp_lock);

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Success, returned %d.\n", ret);
	return ret;

err_out_disable_umt:
#if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE
	ltq_umt_enable(g_priv_info[dev_idx].umt_id, g_priv_info[dev_idx].port_id, 0);
#else /* #if defined(CONFIG_LTQ_UMT_EXPAND_MODE) && CONFIG_LTQ_UMT_EXPAND_MODE */
	ltq_umt_enable(0);
#endif /* #else */

err_out_deregister_subif:
	dp_register_subif(owner, dev, dev->name, subif_id, DP_F_DEREGISTER);

	DC_DP_UNLOCK(&g_dc_dp_lock);

	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Failure, returned %d.\n", DP_FAILURE);
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_register_subif);

int32_t
dc_dp_xmit(struct net_device *rx_if, dp_subif_t *rx_subif,
		   struct sk_buff *buf, int32_t len, uint32_t flags)
{
	int32_t ret;

	if (!buf) {
		DC_DP_ERROR("failed to xmit as buf=%p!!!", buf);
		return DP_FAILURE;
	}

	/* Send it to Datapath library for transmit */
	ret = dp_xmit(rx_if, rx_subif, buf, len, flags);

	return ret;
}
EXPORT_SYMBOL(dc_dp_xmit);

int32_t
dc_dp_disconn_if(struct net_device *netif, dp_subif_t *subif_id,
				 uint8_t mac_addr[MAX_ETH_ALEN], uint32_t flags)
{
	int32_t ret = DP_SUCCESS;

#if IS_ENABLED(CONFIG_LTQ_PPA_API)
	/* Remove all the sessions from PPA */
	ret = ppa_hook_disconn_if_fn(netif, subif_id, mac_addr, flags);
#endif /* #if IS_ENABLED(CONFIG_LTQ_PPA_API) */

	return ret;
}
EXPORT_SYMBOL(dc_dp_disconn_if);

/*
 * ========================================================================
 * Callbacks Registered to Datapath Library/Core
 * ========================================================================
 */
#if NEED_PMAC_HDR_REMOVAL
static int32_t
dc_dp_rx_cb(struct net_device *rxif, struct net_device *txif,
			struct sk_buff *skb, int32_t len)
{
	int32_t dev_idx;
	struct pmac_rx_hdr *pmac;
	dp_subif_t rx_subif = {0};

    if (!skb) {
        DC_DP_ERROR("failed to receive as skb=%p!!!\n", skb);
        return -1;
    }

    if (!rxif) {
        DC_DP_ERROR("failed to receive as rxif=%p!!!\n", rxif);
		dev_kfree_skb_any(skb);
        return -1;
    }

	pmac = (struct pmac_rx_hdr *)(skb->data);
	rx_subif.port_id = pmac->sppid;
	rx_subif.subif = (pmac->src_sub_inf_id << 8);
	rx_subif.subif |= pmac->src_sub_inf_id2;

	DC_DP_LOCK(&g_dc_dp_lock);
#if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG
	if (g_dc_dp_dbg_flag & DC_DP_DBG_FLAG_DUMP_RX_PMAC)
		_dc_dp_dump_rx_pmac(pmac);
	if (g_dc_dp_dbg_flag & DC_DP_DBG_FLAG_DUMP_RX_DATA)
		_dc_dp_dump_raw_data(skb->data, skb->len, "Recv Data");
#endif /* #if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG */
	dev_idx = _dc_dp_get_dev_idx_by_port(rx_subif.port_id);
	DC_DP_UNLOCK(&g_dc_dp_lock);
	if (dev_idx < 0) {
        DC_DP_ERROR("port_id=%d does not exist for the rxif=%s!!!\n",
						rx_subif.port_id, rxif->name);
		dev_kfree_skb_any(skb);
		return -1;
	}

	len -= sizeof(struct pmac_rx_hdr);
	skb_pull(skb, sizeof(struct pmac_rx_hdr));
	if (g_priv_info[dev_idx].cb.rx_fn) {
		g_priv_info[dev_idx].cb.rx_fn(rxif, txif, &rx_subif, skb, skb->len);
	} else {
        DC_DP_ERROR("rx_fn callback is not registered on rxif->name=%s!!!\n",
						rxif->name);
		dev_kfree_skb_any(skb);
		return -1;
	}

	return 0;
}
#endif /* #if NEED_PMAC_HDR_REMOVAL */

static int32_t
dc_dp_check_if_netif_fastpath_cb(struct net_device *netif, char *ifname, uint32_t flags)
{
	int32_t ret = 1;
	int32_t idx;

	if (!netif && !ifname) {
		DC_DP_ERROR("failed as netif=%p and ifname=%p!!!", netif, ifname);
		return 0;
	}

	DC_DP_LOCK(&g_dc_dp_lock);

	if (netif)
		idx = _dc_dp_get_idx_via_dev_private(netif);
	else if (ifname)
		idx = _dc_dp_get_idx_via_ifname_private(ifname);

	DC_DP_UNLOCK(&g_dc_dp_lock);

	if (idx < 0) {
		ret = 0;
	}

    DC_DP_DEBUG((DC_DP_DBG_FLAG_DUMP_TX|DC_DP_DBG_FLAG_DUMP_RX), "ifname=%s, fastpath(directconnect)=%d.\n",
					(netif? netif->name : ifname), ret);
	return ret;
}

static int32_t
dc_dp_get_netif_subifid_cb(struct net_device *netif, struct sk_buff *skb,
						   void *subif_data, uint8_t dst_mac[MAX_ETH_ALEN],
						   dp_subif_t *subif, uint32_t flags)
{
	int32_t ret = -1;
	int32_t i, j;
	int32_t (*subif_fn_t)(int32_t port_id, struct net_device *netif, char *mac_addr, uint32_t *sta_id);
	int32_t subifid = -1;
	uint32_t staid = 0;
	uint8_t macaddr[ETH_ALEN] = {0};

	if (!netif) {
		DC_DP_ERROR("failed to get subifid as netif=%p!!!\n", netif);
		return ret;
	}

	if (!subif) {
		DC_DP_ERROR("failed to get subifid as subif=%p!!!\n", subif);
		return ret;
	}

	/* Find Port ID and DC VAP ID */
	DC_DP_LOCK(&g_dc_dp_lock);
	for (i = 0; i < DC_DP_MAX_DEV_NUM; i++) {
		if (g_priv_info[i].port_id != subif->port_id)
			continue;

		for (j = 0; j < DC_DP_MAX_SUBIF_PER_DEV; j++) {
			if (g_priv_info[i].subif_info[j].flags == DC_DP_SUBIF_FREE)
				continue;

			if (g_priv_info[i].subif_info[j].netif == netif) {
				subifid = g_priv_info[i].subif_info[j].subif;
				subif_fn_t = g_priv_info[i].cb.get_subif_fn;

				break;
			}
		}
		if (j < DC_DP_MAX_SUBIF_PER_DEV) {
			break;
		}
	}
	DC_DP_UNLOCK(&g_dc_dp_lock);

	if (subifid < 0) {
		DC_DP_ERROR("failed to get subifid for netif->name=%s!!!\n", netif->name);
		return ret;
	}

	/* Optionally, find WAVE500 Station ID */
	if ((dst_mac || skb) && subif_fn_t) {

		if (dst_mac) {
			memcpy(macaddr, dst_mac, ETH_ALEN);
		} else if (skb) {
			memcpy(macaddr, eth_hdr(skb)->h_dest, ETH_ALEN);
		}

		ret = _dc_dp_find_staid_by_macaddr_rcu(macaddr, &staid);

		if (!ret) {
			ret = subif_fn_t(subif->port_id, netif, macaddr, &staid);
			if (ret) {
				DC_DP_ERROR("failed to get subifid for netif->name=%s!!!\n", netif->name);
				return ret;
			}

			/* Add to <macaddr-staid> mapping table */
			_dc_dp_add_sta_entry(netif, macaddr, staid);
		}

		subifid |= (staid & DC_DP_STAID_MASK);
	}

	subif->subif = subifid;

	ret = 0; /*according to SWAS, return 1 means subifid found */
    DC_DP_DEBUG((DC_DP_DBG_FLAG_DUMP_TX|DC_DP_DBG_FLAG_DUMP_RX), "subifid=0x%04x for netif->name=%s.\n",
					subif->subif, netif->name);
	return ret;
}

#if IS_ENABLED(CONFIG_LTQ_PPA_API)
static int32_t
dc_dp_qos_class2prio_cb(int32_t port_id, struct net_device *netif, uint8_t *class2prio)
{
	int32_t dev_idx;

	if (!class2prio) {
		DC_DP_ERROR("failed to update class2prio map, as port_id=%d, netif=%p, class2prio=%p!!!\n",
						port_id, netif, class2prio);
		return DP_FAILURE;
	}

	DC_DP_LOCK(&g_dc_dp_lock);

	/* Find the registered device index */
	dev_idx = _dc_dp_get_dev_idx_by_port(port_id);
	if (dev_idx < 0) {
		DC_DP_ERROR("failed to find port=%d in device table!!!\n", port_id);
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return DP_FAILURE;
	}

	/* Update local QoS Class2Priority map table */
	memcpy(g_priv_info[dev_idx].class2prio, class2prio, sizeof(g_priv_info[dev_idx].class2prio));

	/* Configure the egress PMAC table to mark the WMM/TID in the descriptor DW1[7:4] */
#if 0 // FIXME
	dp_pmac_cfg_t pmac_cfg;

	ret = dp_pmac_set(port_id, &pmac_cfg);
#else
{
	uint8_t i = 0, j = 0;
	GSW_PMAC_Glbl_Cfg_t pmac_glb;
	GSW_PMAC_Eg_Cfg_t egcfg;
	GSW_API_HANDLE gswr;
	uint8_t wmm;

	/* Do the GSW-R configuration */
	gswr = gsw_api_kopen("/dev/switch_api/1");
	if (gswr == 0) {
		DC_DP_ERROR("failed to open SWAPI device!!!\n");
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return -EIO;
	}

	memset(&pmac_glb, 0, sizeof(pmac_glb));
	gsw_api_kioctl(gswr, GSW_PMAC_GLBL_CFG_GET,
		       (u32) &pmac_glb);

	/* GSWIP-R PMAC Egress Configuration Table */
	for (i = 0; i <= 15; i++) {
		wmm = _dc_dp_get_class2wmm(dev_idx, i, 1);

		for (j = 0; j <= 3; j++) {
			memset((void *)&egcfg, 0x00, sizeof(egcfg));

			egcfg.nRxDmaChanId  = 0;
			egcfg.bPmacEna      = 0;
			egcfg.bFcsEna       = 0;
			egcfg.bRemL2Hdr     = 0;
			egcfg.numBytesRem   = 0;
			egcfg.nResDW1       = wmm;
			egcfg.nRes1DW0      = 0;
			egcfg.nRes2DW0      = 0;
			egcfg.nDestPortId   = port_id;
			egcfg.nTrafficClass = i;
			egcfg.bMpe1Flag     = 0;
			egcfg.bMpe2Flag     = 0;
			egcfg.bEncFlag      = 0;
			egcfg.bDecFlag      = 0;
			egcfg.nFlowIDMsb    = j;
			egcfg.bTCEnable	    = 1;
			egcfg.bProcFlagsSelect = pmac_glb.bProcFlagsEgCfgEna;

			gsw_api_kioctl(gswr, GSW_PMAC_EG_CFG_SET, (unsigned int)&egcfg);		
		}
	}

	gsw_api_kclose(gswr);
}
#endif

	DC_DP_UNLOCK(&g_dc_dp_lock);

	return DP_SUCCESS;
}
#endif /* #if IS_ENABLED(CONFIG_LTQ_PPA_API) */

/*
 * ========================================================================
 * Misclleneous API
 * ========================================================================
 */
int32_t
dc_dp_register_mcast_module(struct net_device *dev, struct module *owner,
							dc_dp_mcast_callback_fn_t cb, unsigned int flags)
{
#if defined(CONFIG_LANTIQ_MCAST_HELPER_MODULE) || defined(CONFIG_LANTIQ_MCAST_HELPER)
	mcast_helper_register_module(dev, owner, NULL, cb, flags);
	return DP_SUCCESS;
#else /* #if defined(CONFIG_LANTIQ_MCAST_HELPER_MODULE) || defined(CONFIG_LANTIQ_MCAST_HELPER) */
	return DP_FAILURE;
#endif /* #else */
}
EXPORT_SYMBOL(dc_dp_register_mcast_module);

int32_t
dc_dp_map_prio_wmm_class(int32_t port_id, struct net_device *netif, uint8_t prio2wmm[DC_DP_MAX_WIFI_CLASS])
{
	//int32_t ret;
	int32_t dev_idx;
	uint8_t wmm;

	if (!netif) {
		DC_DP_ERROR("failed to set prio2wmm map: port_id=%d, netif=%p, prio2wmm=%p!!!\n",
						port_id, netif, prio2wmm);
		return DP_FAILURE;
	}

	DC_DP_LOCK(&g_dc_dp_lock);

	/* Find the registered device index */
	dev_idx = _dc_dp_get_dev_idx_by_port(port_id);
	if (dev_idx < 0) {
		DC_DP_ERROR("failed to find port=%d in device table!!!\n", port_id);
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return DP_FAILURE;
	}

	/* Update local prio2wmm map table */
	if (prio2wmm)
		memcpy(g_priv_info[dev_idx].prio2wmm, prio2wmm, sizeof(g_priv_info[dev_idx].prio2wmm));
	else
		memset(g_priv_info[dev_idx].prio2wmm, 0, sizeof(g_priv_info[dev_idx].prio2wmm));

	/* Configure the egress PMAC table to mark the WMM/TID in the descriptor DW1[7:4] */
#if 0 // FIXME
	dp_pmac_cfg_t pmac_cfg;

	ret = dp_pmac_set(port_id, &pmac_cfg);
#else
{
	uint8_t i = 0, j = 0;
	GSW_PMAC_Glbl_Cfg_t pmac_glb;
	GSW_PMAC_Eg_Cfg_t egcfg;
	GSW_API_HANDLE gswr;

	/* Do the GSW-R configuration */
	gswr = gsw_api_kopen("/dev/switch_api/1");
	if (gswr == 0) {
		DC_DP_ERROR("failed to open SWAPI device!!!\n");
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return -EIO;
	}

	memset(&pmac_glb, 0, sizeof(pmac_glb));
	gsw_api_kioctl(gswr, GSW_PMAC_GLBL_CFG_GET,
		       (u32) &pmac_glb);

	/* GSWIP-R PMAC Egress Configuration Table */
	for (i = 0; i <= 15; i++) {
		if (prio2wmm)
			wmm = _dc_dp_get_class2wmm(dev_idx, i, 1);
		else
			wmm = 0;

		for (j = 0; j <= 3; j++) {
			memset((void *)&egcfg, 0x00, sizeof(egcfg));

			egcfg.nRxDmaChanId  = 0;
			egcfg.bPmacEna      = 0;
			egcfg.bFcsEna       = 0;
			egcfg.bRemL2Hdr     = 0;
			egcfg.numBytesRem   = 0;
			egcfg.nResDW1       = wmm;
			egcfg.nRes1DW0      = 0;
			egcfg.nRes2DW0      = 0;
			egcfg.nDestPortId   = port_id;
			egcfg.nTrafficClass = i;
			egcfg.bMpe1Flag     = 0;
			egcfg.bMpe2Flag     = 0;
			egcfg.bEncFlag      = 0;
			egcfg.bDecFlag      = 0;
			egcfg.nFlowIDMsb    = j;
			egcfg.bTCEnable	    = 1;
			egcfg.bProcFlagsSelect = pmac_glb.bProcFlagsEgCfgEna;

			gsw_api_kioctl(gswr, GSW_PMAC_EG_CFG_SET, (unsigned int)&egcfg);		
		}
	}

	gsw_api_kclose(gswr);
}
#endif

	DC_DP_UNLOCK(&g_dc_dp_lock);

	return DP_SUCCESS;
}
EXPORT_SYMBOL(dc_dp_map_prio_wmm_class);

int32_t
dc_dp_mark_pkt_wmm(int32_t port_id, struct net_device *dst_netif, struct sk_buff *skb)
{
	int32_t dev_idx;
	struct dma_tx_desc_1 *desc_1 = (struct dma_tx_desc_1 *) &skb->DW1;
	int32_t class_prio;
	int32_t wmm;
	bool is_class = 1;

	DC_DP_LOCK(&g_dc_dp_lock);

	/* Find the registered device index */
	dev_idx = _dc_dp_get_dev_idx_by_port(port_id);
	if (dev_idx < 0) {
		DC_DP_ERROR("failed to find port=%d in device table!!!\n", port_id);
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return DP_FAILURE;
	}

#ifdef CONFIG_NETWORK_EXTMARK
	GET_DATA_FROM_MARK_OPT(skb->extmark, QUEPRIO_MASK, QUEPRIO_START_BIT_POS, class_prio);
	class_prio -= 1; /* extmark value is in <1-8>, where corresponding class value is expected in <0-7> */
#else /* #ifdef CONFIG_NETWORK_EXTMARK */
	class_prio = skb->priority;
	is_class = 0;
#endif /* #else */

	wmm = _dc_dp_get_class2wmm(dev_idx, class_prio, is_class);

	DC_DP_UNLOCK(&g_dc_dp_lock);

	desc_1->field.resv1 = wmm;

	return wmm;
}
EXPORT_SYMBOL(dc_dp_mark_pkt_wmm);

int32_t
dc_dp_get_netif_stats(struct net_device *netif, dp_subif_t *subif_id,
					  struct rtnl_link_stats64 *if_stats, uint32_t flags)
{
	return dp_get_netif_stats(netif, subif_id, if_stats, flags);
}
EXPORT_SYMBOL(dc_dp_get_netif_stats);

int32_t
dc_dp_clear_netif_stats(struct net_device *netif, dp_subif_t *subif_id, uint32_t flags)
{
	return dp_clear_netif_stats(netif, subif_id, flags);
}
EXPORT_SYMBOL(dc_dp_clear_netif_stats);

int32_t
dc_dp_get_wol_cfg(dc_dp_wol_cfg_t *cfg, uint32_t flags)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_get_wol_cfg);

int32_t
dc_dp_set_wol_cfg(dc_dp_wol_cfg_t *cfg, uint32_t flags)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_set_wol_cfg);

int32_t
dc_dp_set_wol_ctrl(int32_t port_id, uint32_t enable)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_set_wol_ctrl);

int32_t
dc_dp_get_wol_ctrl_status(int32_t port_id)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_get_wol_ctrl_status);

int32_t
dc_dp_add_class_rule(dc_dp_class_rule_t *rule, uint32_t flags)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_add_class_rule);

int32_t
dc_dp_mod_class_rule(dc_dp_class_rule_t *rule, uint32_t flags)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_mod_class_rule);

int32_t
dc_dp_del_class_rule(dc_dp_class_rule_t *rule, uint32_t flags)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_del_class_rule);

int32_t
dc_dp_get_class_rule(dc_dp_class_rule_t *rule, uint32_t flags)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_get_class_rule);

int32_t
dc_dp_register_power_notifier(struct notifier_block *nb, uint32_t notify_type, uint32_t flags)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_register_power_notifier);

int32_t
dc_dp_req_power_state(dc_dp_power_module_t module, uint8_t module_nr, dc_dp_power_state_t new_state)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_req_power_state);

int32_t
dc_dp_get_ps_poll_period(dc_dp_power_module_t module, uint8_t module_nr)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_get_ps_poll_period);

dc_dp_ps_threshold_t *
dc_dp_get_ps_threshold(dc_dp_power_module_t module, uint8_t module_nr)
{
    /* TODO */
	return NULL;
}
EXPORT_SYMBOL(dc_dp_get_ps_threshold);

int32_t
dc_dp_mod_ps_list(struct list_head *head, int32_t add)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_mod_ps_list);

dc_dp_power_state_t
dc_dp_get_ps_from_khz(uint32_t freq_khz)
{
    /* TODO */
	return DP_FAILURE;
}
EXPORT_SYMBOL(dc_dp_get_ps_from_khz);

/*
 * ========================================================================
 * Module Init/Exit API
 * ========================================================================
 */
static __init int dc_dp_init_module(void)
{
	int32_t ret = 0;

	spin_lock_init(&g_dc_dp_lock);

	DC_DP_LOCK(&g_dc_dp_lock);

	if (!g_dc_dp_init_ok) {
		memset(g_priv_info, 0, sizeof(g_priv_info));

		/* Initialize <MAC, StaId> map table */
		ret = dc_dp_sta_info_init();

#if IS_ENABLED(CONFIG_LTQ_PPA_API)
		ppa_check_if_netif_fastpath_fn = dc_dp_check_if_netif_fastpath_cb;
#endif /* #if IS_ENABLED(CONFIG_LTQ_PPA_API) */

#ifdef CONFIG_PROC_FS
		dc_dp_proc_init();
#endif /* #ifdef CONFIG_PROC_FS */

		g_dc_dp_init_ok = 1;
	}

	DC_DP_UNLOCK(&g_dc_dp_lock);

	return ret;
}

static __exit void dc_dp_exit_module(void)
{
	if (g_dc_dp_init_ok) {

		DC_DP_LOCK(&g_dc_dp_lock);

#ifdef CONFIG_PROC_FS
		dc_dp_proc_exit();
#endif /* #ifdef CONFIG_PROC_FS */

#if IS_ENABLED(CONFIG_LTQ_PPA_API)
		ppa_check_if_netif_fastpath_fn = NULL;
#endif /* #if IS_ENABLED(CONFIG_LTQ_PPA_API) */

		dc_dp_sta_info_exit();

		/* Reset private data structure */
		memset(g_priv_info, 0, sizeof(g_priv_info));
		g_dc_dp_init_ok = 0;

		DC_DP_UNLOCK(&g_dc_dp_lock);
	}
}

module_init(dc_dp_init_module);
module_exit(dc_dp_exit_module);

MODULE_AUTHOR("Anath Bandhu Garai");
MODULE_DESCRIPTION("Lantiq DirectConnect datapath driver (Supported XRX500)");
MODULE_LICENSE("GPL");
MODULE_VERSION(DRV_MODULE_VERSION);
