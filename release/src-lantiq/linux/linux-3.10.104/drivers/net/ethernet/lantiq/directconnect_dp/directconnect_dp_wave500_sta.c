#include <linux/kernel.h>
#include <linux/jhash.h>

#include "directconnect_dp.h"

dc_dp_priv_sta_info_t g_sta_info;
static struct kmem_cache *g_sta_info_cache __read_mostly;
static uint32_t g_sta_info_salt __read_mostly;

static inline int _dc_dp_mac_hash(const uint8_t *macaddr);
//static dc_dp_priv_sta_entry_t *_dc_dp_find_sta_entry_by_macaddr(const uint8_t *macaddr);
//static int32_t _dc_dp_find_staid_by_macaddr_rcu(const uint8_t *macaddr, uint32_t *staid);
static void _dc_dp_rcu_free_sta_entry(struct rcu_head *head);
///static dc_dp_priv_sta_entry_t *_dc_dp_add_sta_entry(struct net_device *dev,
///			const uint8_t *macaddr, const uint32_t sta_id);
///static void _dc_dp_delete_sta_entry(dc_dp_priv_sta_entry_t *sta_entry);

static inline int _dc_dp_mac_hash(const uint8_t *macaddr)
{
	/* use 1 byte of OUI and 3 bytes of NIC */
	uint32_t key = get_unaligned((uint32_t *)(macaddr + 2));

	return jhash_1word(key, g_sta_info_salt) & (STA_HASH_SIZE - 1);
}

int32_t
_dc_dp_find_staid_by_macaddr_rcu(const uint8_t *macaddr, uint32_t *staid)
{
	dc_dp_priv_sta_entry_t *sta_entry;
	struct hlist_head *head = &g_sta_info.sta_hash[_dc_dp_mac_hash(macaddr)];

	rcu_read_lock();
	hlist_for_each_entry_rcu(sta_entry, head, hlist) {
		if (!memcmp(sta_entry->mac_addr, macaddr, ETH_ALEN)) {
			*staid = sta_entry->sta_id;
			return 1;
		}
	}
	rcu_read_unlock();

	return 0;
}

dc_dp_priv_sta_entry_t *
_dc_dp_find_sta_entry_by_macaddr(const uint8_t *macaddr)
{
	dc_dp_priv_sta_entry_t *sta_entry;
	struct hlist_head *head = &g_sta_info.sta_hash[_dc_dp_mac_hash(macaddr)];

	hlist_for_each_entry(sta_entry, head, hlist) {
		if (!memcmp(sta_entry->mac_addr, macaddr, ETH_ALEN)) {
			return sta_entry;
		}
	}

	return NULL;
}

dc_dp_priv_sta_entry_t *
_dc_dp_add_sta_entry(struct net_device *dev, const uint8_t *macaddr, const uint32_t sta_id)
{
	dc_dp_priv_sta_entry_t *sta_entry;
	struct hlist_head *head = &g_sta_info.sta_hash[_dc_dp_mac_hash(macaddr)];

	spin_lock_bh(&g_sta_info.sta_hash_lock);

	sta_entry = _dc_dp_find_sta_entry_by_macaddr(macaddr);
	if (sta_entry) {
		return sta_entry;
	}

	sta_entry = kmem_cache_alloc(g_sta_info_cache, GFP_ATOMIC);
	if (sta_entry) {
		INIT_HLIST_NODE(&sta_entry->hlist);
		memcpy(sta_entry->mac_addr, macaddr, ETH_ALEN);
		sta_entry->sta_id = sta_id;
		sta_entry->netif = dev;
		hlist_add_head_rcu(&sta_entry->hlist, head);
	} else {
		DC_DP_ERROR("failed to allocate station entry for station id %d!!!\n", sta_id);
	}

	spin_unlock_bh(&g_sta_info.sta_hash_lock);

	return sta_entry;
}

static void
_dc_dp_rcu_free_sta_entry(struct rcu_head *head)
{
	dc_dp_priv_sta_entry_t *sta_entry = container_of(head, dc_dp_priv_sta_entry_t, rcu);
	kmem_cache_free(g_sta_info_cache, sta_entry);
}

void
_dc_dp_delete_sta_entry(dc_dp_priv_sta_entry_t *sta_entry)
{
	hlist_del_rcu(&sta_entry->hlist);
	call_rcu(&sta_entry->rcu, _dc_dp_rcu_free_sta_entry);
}

int32_t
dc_dp_sta_info_init(void)
{
	int32_t i;

	g_sta_info_cache = kmem_cache_create("g_sta_info_cache",
					 sizeof(dc_dp_priv_sta_entry_t),
					 0,
					 SLAB_HWCACHE_ALIGN, NULL);
	if (!g_sta_info_cache)
		return -ENOMEM;

	get_random_bytes(&g_sta_info_salt, sizeof(g_sta_info_salt));

	spin_lock_init(&g_sta_info.sta_hash_lock);
	for (i = 0; i < STA_HASH_SIZE; i++) {
		INIT_HLIST_HEAD(&g_sta_info.sta_hash[i]);
	}

	return 0;
}

void
dc_dp_sta_info_exit(void)
{
	kmem_cache_destroy(g_sta_info_cache);
}
