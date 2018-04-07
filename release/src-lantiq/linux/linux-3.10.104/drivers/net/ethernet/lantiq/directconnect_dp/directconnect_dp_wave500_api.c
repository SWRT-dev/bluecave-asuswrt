#include "directconnect_dp.h"

#ifndef SWAS_3_8
static inline int32_t
_wave500_dp_get_dev_idx_by_module_port(struct module *owner, int32_t port_id)
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

int32_t
wave500_dp_register_dev(struct module *owner, uint32_t port_id,
						struct net_device *dev, wave500_dp_cb_t *datapathcb,
						uint32_t num_bufs_req, void *umt_base, int32_t umt_len,
						wave500_dp_res_t *resources, uint32_t flags)
{
	int32_t ret;
	int32_t dev_idx;
	dc_dp_res_t dc_dp_res = {0};
	int32_t i, j;
	size_t buflist_virt_sz;
	struct list_head *buflist_virt_base = NULL;
	uint32_t num_buf_req_rem;
	uint32_t tmp_num_bufs_req;
	bufptr_t *tmp_buf_node = NULL;
	uint8_t *tmp_buf = NULL;

	DC_DP_LOCK(&g_dc_dp_lock);

	/* Convert resource structure */
	if (resources) {
		dc_dp_res.num_dma_desc = resources->num_dma_desc;
		dc_dp_res.dmatx_ring_base = resources->dmatx_ring_base;
		dc_dp_res.cbm_dequeue_base = resources->cbm_dequeue_base;
		dc_dp_res.num_dequeue_bufs = resources->num_dequeue_bufs;
		dc_dp_res.cbm_buf_free_base = resources->cbm_buf_free_base;
		dc_dp_res.num_free_bufs = resources->num_free_bufs;
	}

	ret = _dc_dp_register_dev_private(owner, port_id, dev, datapathcb, num_bufs_req, umt_base, umt_len, &dc_dp_res, flags);
	if (ret != DP_SUCCESS) {
		DC_DP_ERROR("failed to register/de-register dev for the port_id=%d!!!\n", port_id);
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return ret;
	}

	/* Find the device index */
	dev_idx = _wave500_dp_get_dev_idx_by_module_port(owner, port_id);
	if (dev_idx < 0) {
		DC_DP_ERROR("failed to register/de-register dev, as port_id=%d is not allocated yet!!!\n", port_id);
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return DP_FAILURE;
	}

	/* De-register */
	if (flags & DP_F_DEREGISTER) {
		if (g_priv_info[dev_idx].buflist_virt_base) {
			kfree(g_priv_info[dev_idx].buflist_virt_base);
			g_priv_info[dev_idx].buflist_virt_base = NULL;
		}

		DC_DP_UNLOCK(&g_dc_dp_lock);
		return ret;
	}

	/* Convert resource structure */
	if (resources) {
		resources->num_dma_desc = dc_dp_res.num_dma_desc;
		resources->dmatx_ring_base = dc_dp_res.dmatx_ring_base;
		resources->cbm_dequeue_base = dc_dp_res.cbm_dequeue_base;
		resources->num_dequeue_bufs = dc_dp_res.num_dequeue_bufs;
		resources->cbm_buf_free_base = dc_dp_res.cbm_buf_free_base;
		resources->num_free_bufs = dc_dp_res.num_free_bufs;
	}

	buflist_virt_sz = (sizeof(struct list_head) + (num_bufs_req * sizeof(bufptr_t)));

	/* Allocate Tx buffers */
	DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Allocating %d DMA1-Tx buffer lists.\n", dc_dp_res.num_bufpools);
	buflist_virt_base = (struct list_head *) kmalloc(buflist_virt_sz, GFP_KERNEL);
	if (!buflist_virt_base) {
		DC_DP_ERROR("failed to allocate %d buffer lists!!!\n", dc_dp_res.num_bufpools);
		_dc_dp_register_dev_private(owner, port_id, dev, datapathcb, num_bufs_req, umt_base, umt_len, &dc_dp_res, DP_F_DEREGISTER);
		DC_DP_UNLOCK(&g_dc_dp_lock);
		return DP_FAILURE;
	}
	memset((void *)buflist_virt_base, 0, buflist_virt_sz);
	g_priv_info[dev_idx].buflist_virt_base = buflist_virt_base;

	INIT_LIST_HEAD(buflist_virt_base);

	tmp_buf_node = (bufptr_t *)((uint8_t *)buflist_virt_base + sizeof(struct list_head));
	num_buf_req_rem = num_bufs_req;
	for (i = 0; i < dc_dp_res.num_bufpools; i++) {
		tmp_buf = (uint8_t *)dc_dp_res.buflist[i].pool;
		tmp_num_bufs_req = MIN(num_buf_req_rem, (dc_dp_res.buflist[i].size / g_size_tx_buffers));
		for (j = 0; j < tmp_num_bufs_req; j++) {
			tmp_buf_node->pktbuf = tmp_buf;

			/* Populate it into the list returned 'buflist' */
			list_add_tail(&tmp_buf_node->list, buflist_virt_base);

			tmp_buf_node = (bufptr_t *)((uint8_t *)tmp_buf_node + sizeof(bufptr_t));
			tmp_buf += g_size_tx_buffers;
		}

		num_buf_req_rem -= tmp_num_bufs_req;
	}

	resources->buflist = (struct list_head *) buflist_virt_base;
#if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG
	if ( !(flags & DP_F_DEREGISTER) ) {
		int32_t i;

		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->buflist = %p\n", resources->buflist);
		i = 0;
		list_for_each_entry(tmp_buf_node, resources->buflist, list) {
			DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "Buffer %d : 0x%p.\n", (i + 1), (void *)tmp_buf_node->pktbuf);
			if(i >= 5) {
				break;
			} else {
				i++;
			}
		}
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->num_dma_desc = %d\n", resources->num_dma_desc);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->dmatx_ring_base = 0x%p\n", resources->dmatx_ring_base);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->cbm_dequeue_base = 0x%p.\n", resources->cbm_dequeue_base);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->num_dequeue_bufs = %d.\n", resources->num_dequeue_bufs);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->cbm_buf_free_base = 0x%p.\n", resources->cbm_buf_free_base);
		DC_DP_DEBUG(DC_DP_DBG_FLAG_DBG, "resources->num_free_bufs = %d.\n", resources->num_free_bufs);
	}
#endif /* #if defined(CONFIG_LTQ_DIRECTCONNECT_DP_DBG) && CONFIG_LTQ_DIRECTCONNECT_DP_DBG */

	DC_DP_UNLOCK(&g_dc_dp_lock);
	return ret;
}
EXPORT_SYMBOL(wave500_dp_register_dev);
#endif /* #ifndef SWAS_3_8 */

int32_t
wave500_dp_disconn_staid(uint8_t sta_id, uint8_t macaddr[MAX_ETH_ALEN])
{
	int32_t ret = 0;
	struct net_device *dev;
	dc_dp_priv_sta_entry_t *sta_entry;

	DC_DP_DEBUG((DC_DP_DBG_FLAG_DUMP_TX | DC_DP_DBG_FLAG_DUMP_RX),
					"sta_id=%d, macaddr=%pM\n", sta_id, macaddr);

	spin_lock_bh(&g_sta_info.sta_hash_lock);

	sta_entry = _dc_dp_find_sta_entry_by_macaddr(macaddr);
	if (!sta_entry) {
		DC_DP_DEBUG((DC_DP_DBG_FLAG_DUMP_TX|DC_DP_DBG_FLAG_DUMP_RX), "No entry found for staid %d and macaddr %pM!!!\n", sta_id, macaddr);
		spin_unlock_bh(&g_sta_info.sta_hash_lock);
		return ret;
	}

	dev = sta_entry->netif;

	/* Delete from <macaddr-staid> mapping table */
	_dc_dp_delete_sta_entry(sta_entry);

	spin_unlock_bh(&g_sta_info.sta_hash_lock);

	/* Remove all the sessions from PPA */
	ret = dc_dp_disconn_if(NULL, NULL, macaddr, 0);

	/* Remove the Linux bridge entry */
	ppa_br_fdb_delete(dev, macaddr);

	DC_DP_DEBUG((DC_DP_DBG_FLAG_DUMP_TX|DC_DP_DBG_FLAG_DUMP_RX),
					"Exit, returned=%d.\n", ret);
	return ret;
}
EXPORT_SYMBOL(wave500_dp_disconn_staid);
