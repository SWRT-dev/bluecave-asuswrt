#include<linux/init.h>
#include<linux/module.h>
#include <linux/kernel.h>	/* printk */
#include <linux/types.h>	/* size_t */
#include <linux/version.h>
#include <linux/if_ether.h>
#include <linux/ethtool.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/clk.h>
#include <linux/if_ether.h>
#include <linux/clk.h>
#include <linux/ip.h>
#include <net/ip.h>

#include <lantiq.h>
#include <lantiq_soc.h>
#include <net/lantiq_cbm.h>
#include <net/datapath_api.h>
#include "datapath_pmac.h"
#include "datapath.h"
#include <net/lantiq_cbm_api.h>
#include <xway/switch-api/lantiq_gsw_api.h>
#include <xway/switch-api/lantiq_gsw_flow.h>
#ifdef CONFIG_LTQ_TMU
#include <net/drv_tmu_ll.h>
#endif
#include <linux/ltq_hwmcpy.h>
#if defined(CONFIG_LTQ_PPA_API) || defined(CONFIG_LTQ_PPA_API_MODULE)
#include <net/ppa_api.h>
#endif

#if defined(CONFIG_LTQ_DATAPATH_DBG) && CONFIG_LTQ_DATAPATH_DBG
unsigned int dp_max_print_num = -1, dp_print_num_en = 0;
#endif

GSW_API_HANDLE gswr_r;
u32    dp_rx_test_mode = DP_RX_MODE_NORMAL;
struct dma_rx_desc_1 dma_rx_desc_mask1;
struct dma_rx_desc_3 dma_rx_desc_mask3;
struct dma_rx_desc_0 dma_tx_desc_mask0;
struct dma_rx_desc_1 dma_tx_desc_mask1;
u32 dp_drop_all_tcp_err;
u32 dp_pkt_size_check;

#ifdef CONFIG_LTQ_DP_MPE_FASTHOOK_TEST
int (*ltq_mpe_fasthook_free_fn)(struct sk_buff *) = NULL;
EXPORT_SYMBOL(ltq_mpe_fasthook_free_fn);

int (*ltq_mpe_fasthook_tx_fn)(struct sk_buff *, u32, void *) = NULL;
EXPORT_SYMBOL(ltq_mpe_fasthook_tx_fn);

int (*ltq_mpe_fasthook_rx_fn)(struct sk_buff *, u32, void *) = NULL;
EXPORT_SYMBOL(ltq_mpe_fasthook_rx_fn);
#endif	/*CONFIG_LTQ_DP_MPE_FASTHOOK_TEST */

#if defined(CONFIG_LTQ_HWMCPY) && CONFIG_LTQ_HWMCPY
/*#define dp_memcpy(x, y, z)
* ltq_hwmemcpy(x, y, z, 0, MCPY_IOCU_TO_IOCU, HWMCPY_F_PRIO_HIGH)
*/
#define dp_memcpy(x, y, z)   memcpy(x, y, z)
#else
#define dp_memcpy(x, y, z)   memcpy(x, y, z)
#endif

#undef DP_DBG_ENUM_OR_STRING
#define DP_DBG_ENUM_OR_STRING(name, value, short_name) short_name
char *dp_dbg_flag_str[] = DP_DBG_FLAG_LIST;

#undef DP_DBG_ENUM_OR_STRING

#undef DP_DBG_ENUM_OR_STRING
#define DP_DBG_ENUM_OR_STRING(name, value, short_name) value
u32 dp_dbg_flag_list[] = DP_DBG_FLAG_LIST;

#undef DP_DBG_ENUM_OR_STRING

#undef DP_F_ENUM_OR_STRING
#define DP_F_ENUM_OR_STRING(name, value, short_name) short_name
char *dp_port_type_str[] = DP_F_FLAG_LIST;

#undef DP_F_ENUM_OR_STRING

#undef DP_F_ENUM_OR_STRING
#define DP_F_ENUM_OR_STRING(name, value, short_name) value
u32 dp_port_flag[] = DP_F_FLAG_LIST;

#undef DP_F_ENUM_OR_STRING

char *dp_port_status_str[] = {
	"PORT_FREE",
	"PORT_ALLOCATED",
	"PORT_DEV_REGISTERED",
	"PORT_SUBIF_REGISTERED",
	"Invalid"
};

static int dp_init_ok;
spinlock_t dp_lock; /*datapath spinlock*/
unsigned int dp_dbg_flag;
unsigned int dp_dbg_err = 1; /*print error */
static int32_t dp_rx_one_skb(struct sk_buff *skb, uint32_t flags);
/*port 0 is reserved and never assigned to any one */
struct pmac_port_info dp_port_info[PMAC_MAX_NUM];
struct parser_info pinfo[4];
static int print_len;
static char *parser_flags_str[] = {
	"PASER_FLAGS_NO",
	"PASER_FLAGS_END",
	"PASER_FLAGS_CAPWAP",
	"PASER_FLAGS_GRE",
	"PASER_FLAGS_LEN",
	"PASER_FLAGS_GREK",
	"PASER_FLAGS_NN1",
	"PASER_FLAGS_NN2",

	"PASER_FLAGS_ITAG",
	"PASER_FLAGS_1VLAN",
	"PASER_FLAGS_2VLAN",
	"PASER_FLAGS_3VLAN",
	"PASER_FLAGS_4VLAN",
	"PASER_FLAGS_SNAP",
	"PASER_FLAGS_PPPOES",
	"PASER_FLAGS_1IPV4",

	"PASER_FLAGS_1IPV6",
	"PASER_FLAGS_2IPV4",
	"PASER_FLAGS_2IPV6",
	"PASER_FLAGS_ROUTEXP",
	"PASER_FLAGS_TCP",
	"PASER_FLAGS_1UDP",
	"PASER_FLAGS_IGMP",
	"PASER_FLAGS_IPV4OPT",

	"PASER_FLAGS_IPV6EXT",
	"PASER_FLAGS_TCPACK",
	"PASER_FLAGS_IPFRAG",
	"PASER_FLAGS_EAPOL",
	"PASER_FLAGS_2IPV6EXT",
	"PASER_FLAGS_2UDP",
	"PASER_FLAGS_L2TPNEXP",
	"PASER_FLAGS_LROEXP",

	"PASER_FLAGS_L2TP",
	"PASER_FLAGS_GRE_VLAN1",
	"PASER_FLAGS_GRE_VLAN2",
	"PASER_FLAGS_GRE_PPPOE",
	"PASER_FLAGS_BYTE4_BIT4_UNDEF",
	"PASER_FLAGS_BYTE4_BIT5_UNDEF",
	"PASER_FLAGS_BYTE4_BIT6_UNDEF",
	"PASER_FLAGS_BYTE4_BIT7_UNDEF",

	"PASER_FLAGS_BYTE5_BIT0_UNDEF",
	"PASER_FLAGS_BYTE5_BIT1_UNDEF",
	"PASER_FLAGS_BYTE5_BIT2_UNDEF",
	"PASER_FLAGS_BYTE5_BIT3_UNDEF",
	"PASER_FLAGS_BYTE5_BIT4_UNDEF",
	"PASER_FLAGS_BYTE5_BIT5_UNDEF",
	"PASER_FLAGS_BYTE5_BIT6_UNDEF",
	"PASER_FLAGS_BYTE5_BIT7_UNDEF",

	"PASER_FLAGS_BYTE6_BIT0_UNDEF",
	"PASER_FLAGS_BYTE6_BIT1_UNDEF",
	"PASER_FLAGS_BYTE6_BIT2_UNDEF",
	"PASER_FLAGS_BYTE6_BIT3_UNDEF",
	"PASER_FLAGS_BYTE6_BIT4_UNDEF",
	"PASER_FLAGS_BYTE6_BIT5_UNDEF",
	"PASER_FLAGS_BYTE6_BIT6_UNDEF",
	"PASER_FLAGS_BYTE6_BIT7_UNDEF",

	"PASER_FLAGS_BYTE7_BIT0_UNDEF",
	"PASER_FLAGS_BYTE7_BIT1_UNDEF",
	"PASER_FLAGS_BYTE7_BIT2_UNDEF",
	"PASER_FLAGS_BYTE7_BIT3_UNDEF",
	"PASER_FLAGS_BYTE7_BIT4_UNDEF",
	"PASER_FLAGS_BYTE7_BIT5_UNDEF",
	"PASER_FLAGS_BYTE7_BIT6_UNDEF",
	"PASER_FLAGS_BYTE7_BIT7_UNDEF",

	/*Must be put at the end of the enum */
	"PASER_FLAGS_MAX"
};

struct gsw_itf itf_assign[] = {
		{0, 1, 0, 16},
		{15, 1, 17, 33},
		{1, 1, 34, 50},
		{2, 1, 51, 67},
		{3, 1, 68, 84},
		{4, 1, 85, 101},
		{5, 1, 102, 118},
		{-1, 1, 119, 135},
		{-1, 1, 136, 152},
		{-1, 1, 153, 169},
		{-1, 1, 170, 186},
		{-1, 1, 187, 203},
		{-1, 1, 204, 220},
		{-1, 1, 221, 237},
		{-1, 1, 238, 255}
};

#ifdef CONFIG_LTQ_DP_ACA_CSUM_WORKAROUND
static struct module aca_owner;
static struct net_device aca_dev;
static int aca_portid = -1;
#endif


int get_dp_port_type_str_size(void)
{
	return ARRAY_SIZE(dp_port_type_str);
}

int get_dp_dbg_flag_str_size(void)
{
	return ARRAY_SIZE(dp_dbg_flag_str);
}

int get_dp_port_status_str_size(void)
{
	return ARRAY_SIZE(dp_port_status_str);
}

int parser_size_via_index(u8 index)
{
	if (index >= ARRAY_SIZE(pinfo)) {
		PR_ERR("Wrong index=%d, it should less than %d\n", index,
		       ARRAY_SIZE(pinfo));
		return 0;
	}

	return pinfo[index].size;
}

/*check whether the paser is existing or not in the current packet */
static inline int parser_enabled(int ep, struct dma_rx_desc_1 *desc_1)
{
	u8 index;

	if (!desc_1) {
		PR_ERR("NULL desc_1 is not allowed\n");
		return 0;
	} else if (ep) {
		/*only cpu may have parser, otherwise no parser */
		return 0;
	}
	index = (desc_1->field.mpe2 << 1) + desc_1->field.mpe1;
	return parser_size_via_index(index);
}

struct pmac_port_info *get_port_info(int index)
{
	if (index < PMAC_MAX_NUM)
		return &dp_port_info[index];

	return NULL;
}

struct pmac_port_info *get_port_info_via_dp_port(int dp_port)
{
	int i;

	for (i = 0; i < PMAC_MAX_NUM; i++) {
		if ((dp_port_info[i].status & PORT_DEV_REGISTERED) &&
		    (dp_port_info[i].port_id == dp_port))
			return &dp_port_info[i];
	}

	return NULL;
}

struct pmac_port_info *get_port_info_via_dp_name(struct net_device *dev)
{
	int i;

	for (i = 0; i < PMAC_MAX_NUM; i++) {
		if ((dp_port_info[i].status & PORT_DEV_REGISTERED) &&
		    (dp_port_info[i].dev == dev))
			return &dp_port_info[i];
	}

	return NULL;
}

int8_t parser_size(int8_t v)
{
	if (v == DP_PARSER_F_DISABLE)
		return 0;

	if (v == DP_PARSER_F_HDR_ENABLE)
		return PASAR_OFFSETS_NUM;

	if (v == DP_PARSER_F_HDR_OFFSETS_ENABLE)
		return PASAR_OFFSETS_NUM + PASAR_FLAGS_NUM;

	PR_ERR("Wrong parser setting: %d\n", v);
	/*error */
	return -1;
}

char *parser_str(int index)
{
	if (index == 0)
		return "cpu";

	if (index == 1)
		return "mpe1";

	if (index == 2)
		return "mpe2";

	if (index == 3)
		return "mpe3";

	PR_ERR("Wrong index:%d\n", index);
	return "Wrong index";
}

/*some module may have reconfigure parser configuration in FMDA_PASER.
* It is necessary to refresh the pinfo
*/
void dp_parser_info_refresh(u32 v, u32 verify)
{
	int i;

	pinfo[0].v = (v >> 0) & 0x3;
	pinfo[1].v = (v >> 2) & 0x3;
	pinfo[2].v = (v >> 4) & 0x3;
	pinfo[3].v = (v >> 6) & 0x3;

	for (i = 0; i < ARRAY_SIZE(pinfo); i++) {
		if (verify &&
			(pinfo[i].size != parser_size(pinfo[i].v)))
			PR_ERR
			 ("Lcal parser pinfo[%d](%d) != register cfg(%d)??\n",
			 i, pinfo[i].size,
			 parser_size(pinfo[i].v));

		/*force to update */
		pinfo[i].size = parser_size(pinfo[i].v);

		if ((pinfo[i].size < 0) || (pinfo[i].size > PKT_PMAC_OFFSET)) {
			PR_ERR("Wrong parser setting for %s: %d\n",
			       parser_str(i), pinfo[i].v);
		}
	}
}
EXPORT_SYMBOL(dp_parser_info_refresh);

void print_parser_status(struct seq_file *s)
{
	if (!s)
		return;

	SEQ_PRINTF(s, "REG.cpu  value=%u size=%u\n", pinfo[0].v,
		   pinfo[0].size);
	SEQ_PRINTF(s, "REG.MPE1 value=%u size=%u\n", pinfo[1].v,
		   pinfo[1].size);
	SEQ_PRINTF(s, "REG.MPE2 value=%u size=%u\n", pinfo[2].v,
		   pinfo[2].size);
	SEQ_PRINTF(s, "REG.MPE3 value=%u size=%u\n", pinfo[3].v,
		   pinfo[3].size);
}

static int dp_get_index_via_module_dev_port_priv(struct module *owner,
						 struct net_device *dev,
						 uint32_t dev_port)
{
	int i;

	for (i = 0; i < PMAC_MAX_NUM; i++) {
		if (dp_port_info[i].status) {
			if ((dp_port_info[i].owner == owner) &&
			    (dp_port_info[i].dev_port == dev_port))
				return i;
		}
	}

	return -1;
}

int set_gsw_itf(u8 ep, u8 ena, int start)
{
	union gsw_var tmp;

	if (ep >= PMAC_MAX_NUM)
		return -1;

	/*get this ports itf base */
	tmp.port_cfg.nPortId = ep;
	if (dp_gsw_kioctl(GSWIP_R_DEV_NAME, GSW_PORT_CFG_GET, (u32)&tmp)) {
		DP_DEBUG(DP_DBG_FLAG_MIB,
			 "Why dp_gsw_kioctl return failjure: GSW_PORT_CFG_GET for port_id=%d\n",
			 tmp.port_cfg.nPortId);
		return -1;
	}
	tmp.port_cfg.nIfCountStartIdx = start;
	tmp.port_cfg.bIfCounters = ena ? 1 : 0;
	if (dp_gsw_kioctl(GSWIP_R_DEV_NAME, GSW_PORT_CFG_SET, (u32)&tmp)) {
		DP_DEBUG(DP_DBG_FLAG_MIB,
			 "Why dp_gsw_kioctl return failjure: GSW_PORT_CFG_SET for port_id=%d\n",
			 tmp.port_cfg.nPortId);
		return -1;
	}

	return 0;
}

struct gsw_itf *get_free_itf(u8 ep, u32 flag)
{
	int i;
	int free_id = -1;

	if (ep >= PMAC_MAX_NUM)
		return NULL;

	for (i = 0; i < ARRAY_SIZE(itf_assign); i++) {
		if (ep == itf_assign[i].ep) {
			DP_DEBUG(DP_DBG_FLAG_MIB,
				"Found the free gsw itf for itf_assign[i]=%d\n",
				i);
			set_gsw_itf(ep, 1, itf_assign[i].start);
			return &itf_assign[i];
		}
		if ((itf_assign[i].ep == (u8)-1) && (free_id == -1))
			free_id = i;
	}
	if (free_id != -1) {
		i = free_id;
		DP_DEBUG(DP_DBG_FLAG_MIB,
			"Found the free gsw itf for itf_assign[i]=%d\n",
			i);
		itf_assign[i].ep = ep;
		set_gsw_itf(ep, 1, itf_assign[i].start);
		return &itf_assign[i];
	}

	return NULL;
}

int reset_gsw_itf(u8 ep)
{
	int i;

	if (ep >= PMAC_MAX_NUM)
		return -1;

	/* only matched the fixed assignment one */
	for (i = 0; i < ARRAY_SIZE(itf_assign); i++)
		if (itf_assign[i].ep == ep)
			break;
	if (i >= ARRAY_SIZE(itf_assign)) {
		set_gsw_itf(ep, 0, 0);
	} else if (itf_assign[i].fixed) {
		set_gsw_itf(ep, 1, itf_assign[i].start);
	} else {
		set_gsw_itf(ep, 0, 0);
		itf_assign[i].ep = -1;
	}

	return 0;
}

/*note: dev can be NULL */
static int32_t dp_alloc_port_private(struct module *owner,
				     struct net_device *dev,
				     uint32_t dev_port, int32_t port_id,
				     dp_pmac_cfg_t *pmac_cfg, uint32_t flags)
{
	int i;

	if (!owner) {
		PR_ERR("Allocate port failed for owner NULL\n");
		return DP_FAILURE;
	}

	if (port_id >= PMAC_MAX_NUM || port_id < 0) {
		DP_DEBUG_ASSERT((port_id >= PMAC_MAX_NUM),
				"port_id(%d) >= PMAC_MAX_NUM(%d)", port_id,
				PMAC_MAX_NUM);
		DP_DEBUG_ASSERT((port_id < 0), "port_id(%d) < 0", port_id);
		return DP_FAILURE;
	}

	DP_DEBUG(DP_DBG_FLAG_DBG, "Flags=");

	for (i = 0; i < get_dp_port_type_str_size(); i++) {
		if (flags & dp_port_flag[i])
			DP_DEBUG(DP_DBG_FLAG_DBG, "%s ", dp_port_type_str[i]);
	}

	DP_DEBUG(DP_DBG_FLAG_DBG, "\n");

	if (flags & DP_F_DEREGISTER) {	/*De-register */
		if (dp_port_info[port_id].status != PORT_ALLOCATED) {
			PR_ERR
			    ("No Deallocate for module %s w/o deregistered\n",
			     owner->name);
			return DP_FAILURE;
		}

		cbm_dp_port_dealloc(owner, dev_port, port_id, flags);
		DP_DEBUG(DP_DBG_FLAG_DBG, "de-alloc port %d\n", port_id);
#ifdef CONFIG_LTQ_DATAPATH_MIB
		reset_gsw_itf(port_id);
#endif
		memset(&dp_port_info[port_id], 0,
		       sizeof(dp_port_info[port_id]));
		return DP_SUCCESS;
	}

	/*sanity check here: "onwner + dev_port" should be unique  */
	i = dp_get_index_via_module_dev_port_priv(owner, dev, dev_port);

	if (i > 0) {
		DP_DEBUG_ASSERT(i > 0,
				"module %s(dev_port %d)already alloc %d\n",
				owner->name, dev_port,
				dp_port_info[i].port_id);
		return DP_FAILURE;
	}

	if (port_id) {		/*with specified port_id */
		if (dp_port_info[port_id].status != PORT_FREE) {
			DP_DEBUG_ASSERT(i > 0,
					"module %s (dev_port %d) failed to allocate port %d used by %s %d\n",
					owner->name, dev_port, port_id,
					dp_port_info[i].owner->name,
					dp_port_info[i].dev_port);
			return DP_FAILURE;
		}
	}
	port_id = cbm_dp_port_alloc(owner, dev, dev_port, port_id, flags);
	if (port_id <= 0) {
		PR_ERR
		    ("cbm_dp_port_alloc allocation fail for %s/dev_port %d\n",
		     owner->name, dev_port);
		return DP_FAILURE;
	}
	memset(&dp_port_info[port_id], 0, sizeof(dp_port_info[port_id]));
	dp_port_info[port_id].owner = owner;
	dp_port_info[port_id].dev = dev;
	dp_port_info[port_id].dev_port = dev_port;
	dp_port_info[port_id].alloc_flags = flags;
	dp_port_info[port_id].status = PORT_ALLOCATED;
#ifdef CONFIG_LTQ_DATAPATH_MIB
	dp_port_info[port_id].itf_info = get_free_itf(port_id, flags);
#endif
	if (pmac_cfg)
		dp_pmac_set(port_id, pmac_cfg);

	DP_DEBUG(DP_DBG_FLAG_DBG,
		 "Port %d allocation succeed for module %s with dev_port %d\n",
		 port_id, owner->name, dev_port);
	return port_id;
}

int32_t dp_alloc_port(struct module *owner, struct net_device *dev,
		      uint32_t dev_port, int32_t port_id,
		      dp_pmac_cfg_t *pmac_cfg, uint32_t flags)
{
	int res;

	if (unlikely(!dp_init_ok)) {
		PR_ERR("dp_alloc_port failed for datapath not init yet\n");
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);
	res =
	    dp_alloc_port_private(owner, dev, dev_port, port_id, NULL, flags);
	DP_LIB_UNLOCK(&dp_lock);

#ifdef CONFIG_LTQ_DP_ACA_CSUM_WORKAROUND
	/*
	For VRX518, it will always carry DP_F_FAST_WLAN flag for
	* ACA HW resource purpose in CBM
	*/
	if ((res > 0) &&
		(flags & DP_F_FAST_WLAN) &&
		(aca_portid < 0)) {
		dp_subif_t subif_id;
		#define ACA_CSUM_NAME "aca_csum"

		strcpy(aca_owner.name, ACA_CSUM_NAME);
		strcpy(aca_dev.name, ACA_CSUM_NAME);
		aca_portid = dp_alloc_port(&aca_owner, &aca_dev,
				0, 0, NULL, DP_F_CHECKSUM);
		if (aca_portid <= 0) {
			PR_ERR("dp_alloc_port failed for %s\n", ACA_CSUM_NAME);
			return res;
		}
		if (dp_register_dev(&aca_owner, aca_portid,
			NULL, DP_F_CHECKSUM)) {
			PR_ERR("dp_register_dev fail for %s\n", ACA_CSUM_NAME);
			return res;
		}
		subif_id.port_id = aca_portid;
		subif_id.subif = -1;
		if (dp_register_subif(&aca_owner, &aca_dev,
			ACA_CSUM_NAME, &subif_id,
			DP_F_CHECKSUM)) {
			PR_ERR("dp_register_subif fail for %s\n",
				ACA_CSUM_NAME);
			return res;
		}
	}
#endif
	return res;
}
EXPORT_SYMBOL(dp_alloc_port);


int32_t dp_register_dev(struct module *owner, uint32_t port_id,
			dp_cb_t *dp_cb, uint32_t flags)
{
	int res = DP_FAILURE;

	if (unlikely(!dp_init_ok)) {
		PR_ERR("dp_register_dev failed for datapath not init yet\n");
		return DP_FAILURE;
	}

	if (!port_id || !owner || port_id >= PMAC_MAX_NUM) {
		if (!owner)
			DP_DEBUG(DP_DBG_FLAG_DBG, "owner NULL\n");
		else
			DP_DEBUG(DP_DBG_FLAG_DBG, "Wrong port_id:%d\n",
				 port_id);

		return DP_FAILURE;
	}

	if (flags & DP_F_DEREGISTER) {	/*de-register */
		DP_LIB_LOCK(&dp_lock);

		if (dp_port_info[port_id].status != PORT_DEV_REGISTERED) {
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "No or %s to de-register for num_subif=%d\n",
				 owner->name,
				 dp_port_info[port_id].num_subif);
		} else if (dp_port_info[port_id].status ==
			   PORT_DEV_REGISTERED) {
			dp_port_info[port_id].status = PORT_ALLOCATED;
			res = DP_SUCCESS;
		} else {
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "No for %s to de-register for unknown status:%d\n",
				 owner->name, dp_port_info[port_id].status);
		}

		DP_LIB_UNLOCK(&dp_lock);
		return res;
	}

	DP_LIB_LOCK(&dp_lock);

	/*register a device */
	if (dp_port_info[port_id].status != PORT_ALLOCATED) {
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "No de-register for %s for unknown status:%d\n",
			 owner->name, dp_port_info[port_id].status);
		return DP_FAILURE;
	}

	if (dp_port_info[port_id].owner != owner) {
		DP_DEBUG(DP_DBG_FLAG_DBG, "No matched owner(%s):%p->%p\n",
			 owner->name, owner, dp_port_info[port_id].owner);
		DP_LIB_UNLOCK(&dp_lock);
		return res;
	}

	dp_port_info[port_id].status = PORT_DEV_REGISTERED;

	if (dp_cb)
		dp_port_info[port_id].cb = *dp_cb;

	DP_LIB_UNLOCK(&dp_lock);
	return DP_SUCCESS;
}
EXPORT_SYMBOL(dp_register_dev);

int32_t dp_register_subif(struct module *owner, struct net_device *dev,
			  char *subif_name, dp_subif_t *subif_id,
			  uint32_t flags)
{
	int res = DP_FAILURE;
	int i, port_id, start, end;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "dp_register_subif failed for datapath not init yet\n");
		return DP_FAILURE;
	}

	DP_DEBUG(DP_DBG_FLAG_DBG,
		 "%s:owner=%s dev=%s subif_name=%s port_id=%d subif=%d(%s)\n",
		 (flags & DP_F_DEREGISTER) ? "unregister subif:" :
		 "register subif", owner ? owner->name : "NULL",
		 dev ? dev->name : "NULL", subif_name, subif_id->port_id,
		 subif_id->subif,
		 (subif_id->subif < 0) ? "dynamic" : "fixed");

	if ((!subif_id) || (!subif_id->port_id) || (!owner) ||
	    (subif_id->port_id >= PMAC_MAX_NUM) ||
	    (subif_id->port_id <= 0)) {
		if (!owner)
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "Unregister subif failed for owner NULL\n");
		else if (!subif_id)
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "Unregister subif failed for NULL subif_id\n");
		else
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "Unregister subif failed port_id=%d or others\n",
				 subif_id->port_id);

		return DP_FAILURE;
	}

	port_id = subif_id->port_id;

	if (((!dev) && !(dp_port_info[port_id].alloc_flags & DP_F_FAST_DSL)) ||
	    !subif_name) {
		DP_DEBUG(DP_DBG_FLAG_DBG, "Wrong dev=%p, subif_name=%p\n",
			 dev, subif_name);
		return DP_FAILURE;
	}

	DP_LIB_LOCK(&dp_lock);

	/*register a device */
	if (dp_port_info[port_id].owner != owner) {
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "Unregister subif failed:Not matching:%p(%s)->%p(%s)\n",
			 owner, owner->name, dp_port_info[port_id].owner,
			 dp_port_info[port_id].owner->name);
		DP_LIB_UNLOCK(&dp_lock);
		return res;
	}

	if (flags & DP_F_DEREGISTER) {	/*de-register */
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "Try to unregister subif=%s with dp_port=%d subif=%d\n",
			 subif_name, subif_id->port_id, subif_id->subif);

		if (dp_port_info[port_id].status != PORT_SUBIF_REGISTERED) {
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "Unregister failed:%s not registered subif!\n",
				 subif_name);
			DP_LIB_UNLOCK(&dp_lock);
			return DP_FAILURE;
		}

		for (i = 0; i < MAX_SUBIF_PER_PORT; i++) {
			if (dp_port_info[port_id].subif_info[i].subif ==
			    subif_id->subif) {
				memset(&dp_port_info[port_id].subif_info[i],
				       0,
				       sizeof(dp_port_info[port_id].subif_info
					      [i]));
				dp_port_info[port_id].num_subif--;

				if (dp_port_info[port_id].num_subif == 0) {
					dp_port_info[port_id].status =
					    PORT_DEV_REGISTERED;
					/*last subif deregistered,
					*   so disable cbm port
					*/
#if defined(NEW_CBM_API) && NEW_CBM_API

					if (cbm_dp_enable
					    (owner, port_id,
					     CBM_PORT_F_DISABLE,
					     dp_port_info[port_id].
					     alloc_flags)) {
						DP_LIB_UNLOCK(&dp_lock);
						DP_DEBUG(DP_DBG_FLAG_DBG,
							 "cbm_dp_disable for port %d\n",
							 port_id);

						return res;
					}
#else

					if (cbm_dp_enable
					    (owner, port_id,
					     CBM_PORT_F_DISABLE)) {
						DP_LIB_UNLOCK(&dp_lock);
						DP_DEBUG(DP_DBG_FLAG_DBG,
							 "cbm_dp_disable for port %d\n",
							 port_id);
						return res;
					}
#endif
				}

				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "Found matched subif: %d_%d!\n",
					 subif_id->port_id, subif_id->subif);
				break;
			}
		}

		if (i < MAX_SUBIF_PER_PORT) {
			DP_DEBUG(DP_DBG_FLAG_DBG, "  dp_port=%d subif=%d\n",
				 subif_id->port_id, subif_id->subif);
			res = DP_SUCCESS;
		}

		DP_LIB_UNLOCK(&dp_lock);
		return res;
	}

	/*Register subif */
	if (dp_port_info[port_id].status < PORT_DEV_REGISTERED) {
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "Unregister subif failed:%s is not a registered dev!\n",
			 subif_name);
		DP_LIB_UNLOCK(&dp_lock);
		return res;
	}

	if (subif_id->subif < 0) {	/*dynamic mode */
		start = 0;
		end = MAX_SUBIF_PER_PORT;
	} else {
		/*caller provided the vap. Try to get its vap value as start */
		start = get_vap(subif_id->subif);
		end = start + 1;
	}

	/*PRINTK("search range: start=%d end=%d\n",start, end);*/
	/*allocate a free subif */
	for (i = start; i < end; i++) {
		if (dp_port_info[port_id].subif_info[i].flags)
			continue;

		/*free */
		dp_port_info[port_id].subif_info[i].flags = 1;
		dp_port_info[port_id].subif_info[i].netif = dev;
		dp_port_info[port_id].port_id = port_id;

		if (subif_id->subif < 0)	/*dynamic */
			dp_port_info[port_id].subif_info[i].subif =
			    set_vap(i);
		else		/*provided by caller */
			dp_port_info[port_id].subif_info[i].subif =
			    subif_id->subif;

		strcpy(dp_port_info[port_id].subif_info[i].device_name,
		       subif_name);
		dp_port_info[port_id].subif_info[i].flags =
		    PORT_SUBIF_REGISTERED;
		dp_port_info[port_id].status = PORT_SUBIF_REGISTERED;
		subif_id->port_id = port_id;
		subif_id->subif = dp_port_info[port_id].subif_info[i].subif;
		dp_port_info[port_id].num_subif++;
#if defined(NEW_CBM_API) && NEW_CBM_API

		if (dp_port_info[port_id].num_subif == 1) {
			if (cbm_dp_enable
			    (owner, port_id, 0,
			     dp_port_info[port_id].alloc_flags)) {
				DP_LIB_UNLOCK(&dp_lock);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "cbm_dp_disable for port %d\n",
					 port_id);
				return res;
			}
		}
#else

		/*if this is the first subif for this port id */
		if (dp_port_info[port_id].num_subif == 1) {
			if (cbm_dp_enable(owner, port_id, 0)) {
				res = DP_FAILURE;
				DP_LIB_UNLOCK(&dp_lock);
				DP_DEBUG(DP_DBG_FLAG_DBG,
					 "cbm_dp_disable for port %d\n",
					 port_id);
				return res;
			}
		}
#endif
		break;
	}

	if (i < end)
		res = DP_SUCCESS;
	else
		DP_DEBUG(DP_DBG_FLAG_DBG,
			"register subif failed for no matched vap\n");

	DP_LIB_UNLOCK(&dp_lock);
	return res;
}
EXPORT_SYMBOL(dp_register_subif);

/*Note:
** try to get subif according to netif, skb,vcc,dst_mac.
** For DLS nas interface, must provide valid subif_data, otherwise set to NULL.
** Note: subif_data is mainly used for DSL WAN mode, esp ATM.
*/

int32_t dp_get_netif_subifid(struct net_device *netif, struct sk_buff *skb,
			     void *subif_data, uint8_t dst_mac[MAX_ETH_ALEN],
			     dp_subif_t *subif, uint32_t flags)
{
	int res = -1;
	int i, k;
	int port_id = -1;
	int dst_subifid = -1;
	dp_get_netif_subifid_fn_t subifid_fn_t;

	/*DP_DEBUG(DP_DBG_FLAG_DBG,
	  "get_netif_subif:dev=%s skb=0x%p data=0x%p port=%d(%s) flag=0x%x\n",
	  netif ? netif->name : "NULL", skb, subif_data,
	  subif->port_id,
	  (subif->port_id <= 0) ? "not portid" : "specified portid", flags);
	*/
	if (!netif && !subif_data) {
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "dp_get_netif_subifid failed: netif=%p subif_data=%p\n",
			 netif, subif_data);
		return res;
	}

	if (!subif) {
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "dp_get_netif_subifid failed:subif=%p\n", subif);
		return res;
	}

	if (subif->port_id >= PMAC_MAX_NUM) {
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "dp_get_netif_subifid wrong port_id: %d\n",
			 subif->port_id);
		return res;
	}

	DP_LIB_LOCK(&dp_lock);
	for (k = 0; k < PMAC_MAX_NUM; k++) {
		if (dp_port_info[k].status != PORT_SUBIF_REGISTERED)
			continue;

		/*Workaround for VRX318 */
		if (subif_data &&
		    (dp_port_info[k].alloc_flags & DP_F_FAST_DSL)) {
			/*VRX318 should overwritten them later if necessary */
			port_id = k;
			dst_subifid = 0;
			break;
		}

		/*search sub-interfaces */
		for (i = 0; i < MAX_SUBIF_PER_PORT; i++) {
			if (!dp_port_info[k].subif_info[i].flags)
				continue;

			if (dp_port_info[k].subif_info[i].netif == netif) {
				if ((subif->port_id > 0) &&
				    (subif->port_id != k)) {
					DP_DEBUG(DP_DBG_FLAG_DBG,
						"dp_get_netif_subifid portid not match\n");
				} else {
					port_id = k;
					dst_subifid =
					    dp_port_info[k].subif_info[i].
					    subif;
				}
				break;
			}
		}
	}
	DP_LIB_UNLOCK(&dp_lock);

	if ((dst_subifid < 0) || (port_id < 0)) {
		if (subif_data)
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "dp_get_netif_subifid failed with subif_data %p\n",
				 subif_data);
		else		/*netif must should be valid */
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "dp_get_netif_subifid failed: %s\n",
				 netif->name);

		return res;
	}

	subif->port_id = port_id;
	subifid_fn_t = dp_port_info[port_id].cb.get_subifid_fn;

	if (subifid_fn_t && !(flags & DP_F_SUBIF_LOGICAL)) {
		/*subif->subif will be set by callback api itself */
		res =
		    subifid_fn_t(netif, skb, subif_data, dst_mac, subif,
				 flags);

		if (res != 0)
			DP_DEBUG(DP_DBG_FLAG_DBG,
				 "get_netif_subifid callback failed\n");

		return res;
	}
	subif->subif = dst_subifid;
	res = 0;
	return res;
}
EXPORT_SYMBOL(dp_get_netif_subifid);

int update_coc_up_sub_module(enum ltq_cpufreq_state new_state,
			     enum ltq_cpufreq_state old_state, uint32_t flag)
{
	int i;
	dp_coc_confirm_stat fn;

	for (i = 0; i < PMAC_MAX_NUM; i++) {
		fn = dp_port_info[i].cb.dp_coc_confirm_stat_fn;

		if (fn)
			fn(new_state, old_state, flag);
	}

	return 0;
}

/* return DP_SUCESS -- found
*  return DP_FAILURE -- not found
*/
int dp_get_port_subitf_via_dev_private(struct net_device *dev,
				       dp_subif_t *subif)
{
	int i, j;

	for (i = 0; i < PMAC_MAX_NUM; i++)
		for (j = 0; j < MAX_SUBIF_PER_PORT; j++) {
			if (dp_port_info[i].subif_info[j].netif == dev) {
				subif->port_id = i;
				subif->subif = j << VAP_OFFSET;
				return DP_SUCCESS;
			}
		}

	return DP_FAILURE;
}

int dp_get_port_subitf_via_dev(struct net_device *dev, dp_subif_t *subif)
{
	int res;

	DP_LIB_LOCK(&dp_lock);
	res = dp_get_port_subitf_via_dev_private(dev, subif);
	DP_LIB_UNLOCK(&dp_lock);
	return res;
}
EXPORT_SYMBOL(dp_get_port_subitf_via_dev);

int dp_get_port_subitf_via_ifname_private(char *ifname, dp_subif_t *subif)
{
	int i, j;

	for (i = 0; i < PMAC_MAX_NUM; i++) {
		for (j = 0; j < MAX_SUBIF_PER_PORT; j++) {
			if (strcmp
			    (dp_port_info[i].subif_info[j].device_name,
			     ifname) == 0) {
				subif->port_id = i;
				subif->subif = j << VAP_OFFSET;
				return DP_SUCCESS;
			}
		}
	}

	return DP_FAILURE;
}

int dp_get_port_subitf_via_ifname(char *ifname, dp_subif_t *subif)
{
	int res;

	DP_LIB_LOCK(&dp_lock);
	res = dp_get_port_subitf_via_ifname_private(ifname, subif);
	DP_LIB_UNLOCK(&dp_lock);
	return res;
}
EXPORT_SYMBOL(dp_get_port_subitf_via_ifname);

int32_t dp_check_if_netif_fastpath_fn(struct net_device *netif,
				      dp_subif_t *subif, char *ifname,
				      uint32_t flags)
{
	int res = 1;
	dp_subif_t tmp_subif = { 0 };

	DP_LIB_LOCK(&dp_lock);
	if (unlikely(!dp_init_ok)) {
		PR_ERR("dp_check_if_netif_fastpath_fn fail: dp not ready\n");
		return DP_FAILURE;
	}
	if (subif)
		tmp_subif = *subif;
	else if (netif)
		dp_get_port_subitf_via_dev_private(netif, &tmp_subif);
	else if (ifname)
		dp_get_port_subitf_via_ifname_private(ifname, &tmp_subif);

	if (tmp_subif.port_id <= 0 && tmp_subif.port_id >= PMAC_MAX_NUM)
		res = 0;
	else if (!
		 (dp_port_info[tmp_subif.port_id].
		  alloc_flags & (DP_F_FAST_DSL || DP_F_FAST_ETH_LAN ||
				 DP_F_FAST_ETH_WAN || DP_F_FAST_WLAN)))
		res = 0;

	DP_LIB_UNLOCK(&dp_lock);
	return res;
}
EXPORT_SYMBOL(dp_check_if_netif_fastpath_fn);

struct module *dp_get_module_owner(int ep)
{
	if (unlikely(!dp_init_ok)) {
		PR_ERR
		    ("dp_get_module_owner failed for datapath not init yet\n");
		return NULL;
	}

	if ((ep >= 0) && (ep < PMAC_MAX_NUM))
		return dp_port_info[ep].owner;

	return NULL;
}
EXPORT_SYMBOL(dp_get_module_owner);

/*if subif->vap == -1, it means all vap */
void dp_clear_mib(dp_subif_t *subif, uint32_t flag)
{
	int i, j, start_vap, end_vap;
	dp_reset_mib_fn_t reset_mib_fn;

	if (!subif || (subif->port_id >= PMAC_MAX_NUM) ||
	    (subif->port_id < 0)) {
		DP_DEBUG(DP_DBG_FLAG_DBG, "dp_clear_mib Wrong subif\n");
		return;
	}

	i = subif->port_id;

	if (subif->subif == -1) {
		start_vap = 0;
		end_vap = MAX_SUBIF_PER_PORT;
	} else {
		start_vap = get_vap(subif->subif);
		end_vap = start_vap + 1;
	}

	for (j = start_vap; j < end_vap; j++) {
		dp_port_info[i].tx_err_drop = 0;
		dp_port_info[i].rx_err_drop = 0;
		memset(&dp_port_info[i].subif_info[j].mib, 0,
		       sizeof(dp_port_info[i].subif_info[j].mib));
		reset_mib_fn = dp_port_info[i].cb.reset_mib_fn;

		if (reset_mib_fn)
			reset_mib_fn(subif, 0);
	}
}

void dp_clear_all_mib_inside(uint32_t flag)
{
	dp_subif_t subif;
	int i;

	for (i = 0; i < PMAC_MAX_NUM; i++) {
		subif.port_id = i;
		subif.subif = -1;
		dp_clear_mib(&subif, flag);
	}
}


int dp_get_drv_mib(dp_subif_t *subif, dp_drv_mib_t *mib, uint32_t flag)
{
	dp_get_mib_fn_t get_mib_fn;
	dp_drv_mib_t tmp;
	int i, vap;

	if (unlikely(!dp_init_ok)) {
		DP_DEBUG(DP_DBG_FLAG_DBG,
			 "dp_get_drv_mib failed for datapath not init yet\n");
		return DP_FAILURE;
	}

	if (!subif || !mib)
		return -1;

	vap = get_vap(subif->subif);

	memset(mib, 0, sizeof(*mib));
	get_mib_fn = dp_port_info[subif->port_id].cb.get_mib_fn;

	if (!get_mib_fn)
		return -1;

	if (!(flag & DP_F_STATS_SUBIF)) {
		/*get all VAP's  mib counters if it is -1 */
		for (i = 0; i < MAX_SUBIF_PER_PORT; i++) {
			if (!dp_port_info[subif->port_id].subif_info[i].flags)
				continue;

			subif->subif =
			    dp_port_info[subif->port_id].subif_info[i].subif;
			memset(&tmp, 0, sizeof(tmp));
			get_mib_fn(subif, &tmp, flag);
			mib->rx_drop_pkts += tmp.rx_drop_pkts;
			mib->rx_error_pkts += tmp.rx_error_pkts;
			mib->tx_drop_pkts += tmp.tx_drop_pkts;
			mib->tx_error_pkts += tmp.tx_error_pkts;
		}
	} else {
		if (dp_port_info[subif->port_id].subif_info[vap].flags)
			get_mib_fn(subif, mib, flag);
	}

	return 0;
}

int32_t dp_api_init(void)
{
	spin_lock_init(&dp_lock);
	DP_LIB_LOCK(&dp_lock);
	memset(dp_port_info, 0, sizeof(dp_port_info));
	dp_init_ok = 1;
	DP_LIB_UNLOCK(&dp_lock);
	return 0;
}

void dump_parser_flag(char *buf)
{
	int i, j;
	unsigned char *pflags = buf + PKT_PMAC_OFFSET - 1;
	unsigned char *poffset = buf;

	if (!buf) {
		PR_ERR("dump_parser_flag buf NULL\n");
		return;
	}

	/* one TCP example: offset
	   offset 0
	   00 3a 00 00 00 00 00 00 00 00 00 00 00 00 00 0e
	   00 00 00 16 22 00 00 00 00 00 00 00 00 00 00 2e
	   00 00 00 00 00 00 00 00
	   flags: FLAG_L2TPFLAG_NO
	   00 00 00 00 80 18 80 00
	 */
	PRINTK("paser flag at 0x%p: ", buf);

	for (i = 0; i < 8; i++)
		PRINTK("%02x ", *(pflags - 7 + i));

	PRINTK("\n");
#if 1
	PRINTK("paser flag: ");

	for (i = 0; i < 8; i++)
		PRINTK("%02x ", *(pflags - i));

	PRINTK("(reverse)\n");
#endif

	for (i = 0; i < PASAR_FLAGS_NUM; i++) {	/*8 flags per byte */
		for (j = 0; j < 8; j++) {	/*8 bits per byte */
			if ((i * 8 + j) >= PASER_FLAGS_MAX)
				break;

			if ((*(pflags - i)) & (1 << j)) {	/*flag is set */
				if ((i * 8 + j) < PASAR_OFFSETS_NUM)
					PRINTK
					    ("  Flag %02d offset=%02d: %s\n",
					     i * 8 + j,
					     *(poffset + i * 8 + j),
					     parser_flags_str[i * 8 + j]);
				else
					PRINTK("  Flag %02d %s (No offset)\n",
					       i * 8 + j,
					       parser_flags_str[i * 8 + j]);
			}
		}
	}
}

void dump_rx_dma_desc(struct dma_rx_desc_0 *desc_0,
		      struct dma_rx_desc_1 *desc_1,
		      struct dma_rx_desc_2 *desc_2,
		      struct dma_rx_desc_3 *desc_3)
{
	if (!desc_0 || !desc_1 || !desc_2 || !desc_3) {
		PR_ERR("rx desc_0/1/2/3 NULL\n");
		return;
	}

	PRINTK(" DMA Descripotr:D0=0x%08x D1=0x%08x D2=0x%08x D3=0x%08x\n",
	       *(u32 *)desc_0, *(u32 *)desc_1,
	       *(u32 *)desc_2, *(u32 *)desc_3);
	PRINTK
	    ("  DW0:resv0=%d tunl_id=%d flow_id=%d eth_type=%d subif=0x%04x\n",
	     desc_0->field.resv0, desc_0->field.tunnel_id,
	     desc_0->field.flow_id, desc_0->field.eth_type,
	     desc_0->field.dest_sub_if_id);
	PRINTK
	    ("  DW1:sess_id=%d tcp_err=%d nat=%d dec=%d enc=%d mpe2/1=%d/%d\n",
	     desc_1->field.session_id, desc_1->field.tcp_err,
	     desc_1->field.nat, desc_1->field.dec, desc_1->field.enc,
	     desc_1->field.mpe2, desc_1->field.mpe1);
	PRINTK("      color=%02d ep=%02d resv1=%d classid=%02d\n",
	       desc_1->field.color, desc_1->field.ep, desc_1->field.resv1,
	       desc_1->field.classid);
	PRINTK("  DW2:data_ptr=0x%08x\n", desc_2->field.data_ptr);
	PRINTK("  DW3:own=%d c=%d sop=%d eop=%d dic=%d pdu_type=%d\n",
	       desc_3->field.own, desc_3->field.c, desc_3->field.sop,
	       desc_3->field.eop, desc_3->field.dic, desc_3->field.pdu_type);
	PRINTK
	    ("      offset=%d atm_q=%d mpoa_pt=%d mpoa_mode=%d len=%d\n",
	     desc_3->field.byte_offset, desc_3->field.qid,
	     desc_3->field.mpoa_pt, desc_3->field.mpoa_mode,
	     desc_3->field.data_len);
}

void dump_tx_dma_desc(struct dma_tx_desc_0 *desc_0,
		      struct dma_tx_desc_1 *desc_1,
		      struct dma_tx_desc_2 *desc_2,
		      struct dma_tx_desc_3 *desc_3)
{
	int lookup;

	if (!desc_0 || !desc_1 || !desc_2 || !desc_3) {
		PR_ERR("tx desc_0/1/2/3 NULL\n");
		return;
	}
	PRINTK(" DMA Descripotr:D0=0x%08x D1=0x%08x D2=0x%08x D3=0x%08x\n",
		*(u32 *)desc_0, *(u32 *)desc_1,
		*(u32 *)desc_2, *(u32 *)desc_3);
	PRINTK
	  ("  DW0:resv0=%d tunl_id=%d flow_id=%d eth_type=%d subif=0x%04x\n",
		desc_0->field.resv0, desc_0->field.tunnel_id,
		desc_0->field.flow_id, desc_0->field.eth_type,
		desc_0->field.dest_sub_if_id);
	PRINTK
	  ("  DW1:sess_id=%d tcp_err=%d nat=%d dec=%d enc=%d mpe2/1=%d/%d\n",
		desc_1->field.session_id, desc_1->field.tcp_err,
		desc_1->field.nat, desc_1->field.dec, desc_1->field.enc,
		desc_1->field.mpe2, desc_1->field.mpe1);
	PRINTK("  color=%02d ep=%02d resv1=%d classid=%02d\n",
		desc_1->field.color, desc_1->field.ep, desc_1->field.resv1,
		desc_1->field.classid);
	PRINTK("  DW2:data_ptr=0x%08x\n", desc_2->field.data_ptr);
	PRINTK("  DW3:own=%d c=%d sop=%d eop=%d dic=%d pdu_type=%d\n",
		desc_3->field.own, desc_3->field.c, desc_3->field.sop,
		desc_3->field.eop, desc_3->field.dic, desc_3->field.pdu_type);
	PRINTK("  offset=%d atm_qid=%d mpoa_pt=%d mpoa_mode=%d len=%d\n",
		desc_3->field.byte_offset, desc_3->field.qid,
		desc_3->field.mpoa_pt, desc_3->field.mpoa_mode,
		desc_3->field.data_len);
	lookup =
	    ((desc_0->field.flow_id >> 6) << 12) | ((desc_1->field.
						     dec) << 11) | ((desc_1->
								     field.
								     enc) <<
								    10) |
	    ((desc_1->field.mpe2) << 9) | ((desc_1->field.
					    mpe1) << 8) | ((desc_1->field.
							    ep) << 4) |
	    ((desc_1->field.classid) << 0);
	PRINTK("  lookup index=0x%x qid=%d\n", lookup,
	       get_lookup_qid_via_index(lookup));
}

void dump_rx_pmac(struct pmac_rx_hdr *pmac)
{
	int i;
	unsigned char *p = (char *)pmac;

	if (!pmac) {
		PR_ERR("dump_rx_pmac pmac NULL ??\n");
		return;
	}

	PRINTK("PMAC at 0x%p: ", p);

	for (i = 0; i < 8; i++)
		PRINTK("0x%02x ", p[i]);

	PRINTK("\n");
	/*byte 0 */
	PRINTK("  byte 0:res=%d ver_done=%d ip_offset=%d\n", pmac->res1,
	       pmac->ver_done, pmac->ip_offset);
	/*byte 1 */
	PRINTK("  byte 1:tcp_h_offset=%d tcp_type=%d\n", pmac->tcp_h_offset,
	       pmac->tcp_type);
	/*byte 2 */
	PRINTK("  byte 2:ppid=%d class=%d\n", pmac->sppid, pmac->class);
	/*byte 3 */
	PRINTK("  byte 3:res=%d pkt_type=%d\n", pmac->res2, pmac->pkt_type);
	/*byte 4 */
	PRINTK("  byte 4:res=%d redirect=%d res2=%d src_sub_inf_id=%d\n",
	       pmac->res3, pmac->redirect, pmac->res4, pmac->src_sub_inf_id);
	/*byte 5 */
	PRINTK("  byte 5:src_sub_inf_id2=%d\n", pmac->src_sub_inf_id2);
	/*byte 6 */
	PRINTK("  byte 6:port_map=%d\n", pmac->port_map);
	/*byte 7 */
	PRINTK("  byte 7:port_map2=%d\n", pmac->port_map2);
}

void dump_tx_pmac(struct pmac_tx_hdr *pmac)
{
	int i;
	unsigned char *p = (char *)pmac;

	if (!pmac) {
		PR_ERR("dump_tx_pmac pmac NULL ??\n");
		return;
	}

	PRINTK(" PMAC at 0x%p:", p);

	for (i = 0; i < 8; i++)
		PRINTK("0x%02x ", p[i]);
	PRINTK("\n");
	/*byte 0 */
	PRINTK("  byte 0:tcp_chksum=%d res=%d ip_offset=%d\n",
	       pmac->tcp_chksum, pmac->res1, pmac->ip_offset);
	/*byte 1 */
	PRINTK("  byte 1:tcp_h_offset=%d tcp_type=%d\n", pmac->tcp_h_offset,
	       pmac->tcp_type);
	/*byte 2 */
	PRINTK("  byte 2:ppid=%d res=%d\n", pmac->sppid, pmac->res);
	/*byte 3 */
	PRINTK
	 ("  byte 3:map_en=%d res=%d/%d time_dis=%d class_en=%d pkt_type=%d\n",
	     pmac->port_map_en, pmac->res2, pmac->res3,
	     pmac->time_dis, pmac->class_en,
	     pmac->pkt_type);
	/*byte 4 */
	PRINTK
	  ("  byte 4:fcs_ins_dis=%d redirect=%d time_stmp=%d subif=%d\n",
	     pmac->fcs_ins_dis, pmac->redirect, pmac->time_stmp,
	     pmac->src_sub_inf_id);
	/*byte 5 */
	PRINTK("  byte 5:src_sub_inf_id2=%d\n", pmac->src_sub_inf_id2);
	/*byte 6 */
	PRINTK("  byte 6:port_map=%d\n", pmac->port_map);
	/*byte 7 */
	PRINTK("  byte 7:port_map2=%d\n", pmac->port_map2);
}

void dp_dump_raw_data(char *buf, int len, char *prefix_str)
{
	int i;
	int r;
	int line_num = 32;
	unsigned char *p = (unsigned char *)buf;

	if (!p) {
		PR_ERR("dp_dump_raw_data: p NULL ?\n");
		return;
	}

	PR_CONT("%s in hex at 0x%p\n",
		prefix_str ? (char *)prefix_str : "Data", p);

	for (i = 0; i < len; i++) {
		r = i % line_num;

		if (r == 0)
			PR_CONT(" %04d: ", i);
		else if (r == (line_num / 2))
			PR_CONT(" ");	/*inser seperator */

		PR_CONT("%02x ", p[i]);

		if (r == (line_num - 1))
			PR_CONT("\n");	/*insert new line */
	}

	PRINTK("\n");
}
EXPORT_SYMBOL(dp_dump_raw_data);

int32_t dp_rx(struct sk_buff *skb, uint32_t flags)
{
	struct sk_buff *next;
	int res = -1;

	if (unlikely(!dp_init_ok)) {
		while (skb) {
			next = skb->next;
			skb->next = 0;
			dev_kfree_skb_any(skb);
			skb = next;
		}
	}

	while (skb) {
		next = skb->next;
		skb->next = 0;
		res = dp_rx_one_skb(skb, flags);
		skb = next;
	}

	return res;
}
EXPORT_SYMBOL(dp_rx);

int dp_lan_wan_bridging(int port_id, struct sk_buff *skb)
{
	dp_subif_t subif;
	struct net_device *dev;
	static int lan_port = 4;

	if (!skb)
		return DP_FAILURE;

	skb_pull(skb, 8);	/*remove pmac */

	if (port_id == 15) {
		/*recv from WAN and forward to LAN via lan_port */
		subif.port_id = lan_port;	/*send to last lan port */
		subif.subif = 0;
	} else if (port_id <= 6) {	/*recv from LAN and forward to WAN */
		subif.port_id = 15;
		subif.subif = 0;
		lan_port = port_id;	/*save lan port id */
	} else {
		dev_kfree_skb_any(skb);
		return DP_FAILURE;
	}

	dev = dp_port_info[subif.port_id].subif_info[0].netif;

	if (!dp_port_info[subif.port_id].subif_info[0].flags || !dev) {
		dev_kfree_skb_any(skb);
		return DP_FAILURE;
	}

	((struct dma_tx_desc_1 *)&skb->DW1)->field.ep = subif.port_id;
	((struct dma_tx_desc_0 *)&skb->DW0)->field.dest_sub_if_id =
	    subif.subif;

	dp_xmit(dev, &subif, skb, skb->len, 0);
	return DP_SUCCESS;
}

#define PRINT_INTERVAL  (5 * HZ) /* 5 seconds */
unsigned long dp_err_interval = PRINT_INTERVAL;
static inline int32_t dp_rx_one_skb(struct sk_buff *skb, uint32_t flags)
{
	int res = DP_SUCCESS;
	struct dma_rx_desc_0 *desc_0 = (struct dma_rx_desc_0 *)&skb->DW0;
	struct dma_rx_desc_1 *desc_1 = (struct dma_rx_desc_1 *)&skb->DW1;
	struct dma_rx_desc_2 *desc_2 = (struct dma_rx_desc_2 *)&skb->DW2;
	struct dma_rx_desc_3 *desc_3 = (struct dma_rx_desc_3 *)&skb->DW3;
	struct pmac_rx_hdr *pmac;
	unsigned char *parser = NULL;
	int rx_tx_flag = 0;	/*0-rx, 1-tx */
	u32 ep = desc_1->field.ep;	/* ep: 0 -15 */
	int vap;		/*vap: 0-15 */
	int paser_exist;
	u32 port_id = ep; /*same with ep now, later set to sspid if ep is 0 */
	struct net_device *dev;
	dp_rx_fn_t rx_fn;
	static unsigned long last_jiffies;
	char decryp = 0;
#define K (1024)
#define M (K * K)

	if (unlikely(!skb)) {
		PR_ERR("why skb NULL\n");
		return DP_FAILURE;
	}
	if (unlikely(!skb->data)) {
		PR_ERR("skb->data NULL\n");
		dev_kfree_skb_any(skb);
		return DP_FAILURE;
	}
	paser_exist = parser_enabled(port_id, desc_1);
	if (paser_exist)
		parser = skb->data;
	pmac = (struct pmac_rx_hdr *)(skb->data + paser_exist);

	if (unlikely(dp_dbg_flag)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX,
			 "\ndp_rx:skb->data=%p Loc=%x offset=%d skb->len=%d\n",
			 skb->data, desc_2->field.data_ptr,
			 desc_3->field.byte_offset, skb->len);
		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_RX_DATA)
			dp_dump_raw_data(skb->data,
					 (skb->len >
					  print_len) ? skb->len : print_len,
					 "Original Data");
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX, "parse hdr size = %d\n",
			 paser_exist);
		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_RX_DESCRIPTOR)
			dump_rx_dma_desc(desc_0, desc_1, desc_2, desc_3);
		if (paser_exist && (dp_dbg_flag & DP_DBG_FLAG_DUMP_RX_PASER))
			dump_parser_flag(parser);
		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_RX_PMAC)
			dump_rx_pmac(pmac);
	}
	if (paser_exist) {
		skb_pull(skb, paser_exist);	/*remove parser */
#if defined(CONFIG_LTQ_PPA_API) || defined(CONFIG_LTQ_PPA_API_MODULE)
#if defined(CONFIG_LTQ_PPA_API_SW_FASTPATH)
		skb->mark |= FLG_PPA_PROCESSED;
#endif
#endif
	}
	/*Sanity check */
	if (unlikely(((ep >= 1) && (ep <= 6)) || (ep == 15))) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX, "Wrong: why ep=%d??", ep);
		dump_rx_dma_desc(desc_0, desc_1, desc_2, desc_3);
		dp_dump_raw_data(skb->data,
				 (skb->len >
				  print_len) ? skb->len : print_len,
				 "Recv Data");
		dev_kfree_skb_any(skb);
		return DP_FAILURE;
	}
	if (unlikely(dp_drop_all_tcp_err && desc_1->field.tcp_err)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX, "\n----dp_rx why tcp_err ???\n");
		dump_rx_dma_desc(desc_0, desc_1, desc_2, desc_3);
		if (paser_exist)
			dump_parser_flag(parser);
		dump_rx_pmac(pmac);
		dp_dump_raw_data(skb->data,
				 (skb->len >
				  print_len) ? skb->len : print_len,
				 "Recv Data");
		dev_kfree_skb_any(skb);
		return DP_FAILURE;
	}

#ifdef LTQ_TSO_SW_WORKAROUND
	if (desc_1->field.mpe2 && desc_1->field.enc && desc_1->field.dec) {
		DP_DEBUG(DP_DBG_FLAG_DBG, "TSO LB\n");
		desc_1->field.mpe2 = 0;
		desc_1->field.dec = 0;
		desc_1->field.enc = 0;
		desc_0->field.flow_id = 0;
		pmac->ip_offset += PMAC_SIZE;
		res = cbm_cpu_pkt_tx(skb, desc_1->field.ep, 0);
		return res;
	}
#endif

	if (port_id == PMAC_CPU_ID) {	/*To CPU and need check src pmac port */
		port_id = pmac->sppid;	/*get the port_id from pmac's sppid */
		if (dp_port_info[port_id].alloc_flags & DP_F_LOOPBACK) {
			/*get the real source port from VAP for ipsec */
			/* related tunnel decap case */
			port_id =
			    get_vap((uint32_t)pmac->src_sub_inf_id2 +
				    (uint32_t)(pmac->src_sub_inf_id << 8));
			vap = 0;
			decryp = 1;
		} else
			vap =
			    get_vap((uint32_t)pmac->src_sub_inf_id2 +
				    (uint32_t)(pmac->src_sub_inf_id << 8));
	} else {		/*GSWIP-R already know the destination */
		rx_tx_flag = 1;
		vap = get_vap(desc_0->field.dest_sub_if_id);
	}

	if (unlikely(!port_id)) { /*Normally shouldnot go to here */
		/*Note: Wrong if Ep and pmac src port both zero*/
		if (jiffies < last_jiffies) /* wraparuond */
			last_jiffies = 0;
		if (jiffies <= last_jiffies + dp_err_interval)
			/* not print in order to keep console not busy */
			goto RX_DROP;
		last_jiffies = jiffies;
		if (!dp_dbg_err) /*bypass dump */
			goto RX_DROP;
		DP_DEBUG(-1, "%s=%d vap=%d\n",
			 (ep) ? "ep" : "port_id", port_id, vap);
		PR_ERR("\nDrop for ep and source port id both zero ??\n");
		dump_rx_dma_desc(desc_0, desc_1, desc_2, desc_3);

		if (paser_exist)
			dump_parser_flag(parser);
		if (pmac)
			dump_rx_pmac(pmac);
		dp_dump_raw_data((char *)(skb->data),
				 (skb->len >
				  print_len) ? skb->len : print_len,
				 "Recv Data");
		goto RX_DROP;
	}

	rx_fn = dp_port_info[port_id].cb.rx_fn;
	if (unlikely(port_id >= PMAC_END_ID)) {
		PR_ERR("Drop for wrong ep or src port id=%u ??\n",
		       port_id);
		goto RX_DROP;
	} else if (unlikely(dp_port_info[port_id].status == PORT_FREE)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX, "Drop for port %u free\n",
			 port_id);
		goto RX_DROP;
	} else if (unlikely(!rx_fn)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX,
			 "Drop for subif of port %u not registered yet\n",
			 port_id);
		DP_LIB_LOCK(&dp_lock);
		dp_port_info[port_id].subif_info[vap].mib.rx_fn_dropped++;
		DP_LIB_UNLOCK(&dp_lock);
		goto RX_DROP2;
	}

	/*Clear some fields as SWAS V3.7 required */
	desc_1->all &= dma_rx_desc_mask1.all;
	desc_3->all &= dma_rx_desc_mask3.all;
	skb->priority = desc_1->field.classid;
	skb->dev = dp_port_info[port_id].subif_info[vap].netif;
	dev = dp_port_info[port_id].subif_info[vap].netif;
	if (decryp) { /*for bypass xfrm policy check since it is decryped*/
		desc_1->field.dec = 1;
		desc_1->field.enc = 1;
	}
	if (!dev &&
	    ((dp_port_info[port_id].alloc_flags & DP_F_FAST_DSL) == 0)) {
		DP_LIB_LOCK(&dp_lock);
		dp_port_info[port_id].subif_info[vap].mib.rx_fn_dropped++;
		DP_LIB_UNLOCK(&dp_lock);
		goto RX_DROP;
	}

	if (unlikely(dp_dbg_flag)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_RX, "%s=%d vap=%d\n",
			 (ep) ? "ep" : "port_id", port_id, vap);

		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_RX_DATA)
			dp_dump_raw_data(skb->data,
					 (skb->len >
					  print_len) ? skb->len : print_len,
					 "Data to top drivers");
		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_RX_DESCRIPTOR)
			dump_rx_dma_desc(desc_0, desc_1, desc_2, desc_3);
	}
#ifdef CONFIG_LTQ_DP_MPE_FASTHOOK_TEST
	if (unlikely(ltq_mpe_fasthook_rx_fn))
		ltq_mpe_fasthook_rx_fn(skb, 1, NULL);	/*with pmac */
#endif
	if (unlikely((enum TEST_MODE)dp_rx_test_mode ==
		DP_RX_MODE_LAN_WAN_BRIDGE))
		/*for datapath performance test only */
		dp_lan_wan_bridging(port_id, skb);
		/*return DP_SUCCESS;*/
	else
		rx_fn((rx_tx_flag == 0) ? dev : NULL,
		      (rx_tx_flag == 1) ? dev : NULL, skb, skb->len);

	DP_LIB_LOCK(&dp_lock);
	if (rx_tx_flag)
		dp_port_info[port_id].subif_info[vap].mib.rx_fn_txif_pkt++;
	else
		dp_port_info[port_id].subif_info[vap].mib.rx_fn_rxif_pkt++;
	DP_LIB_UNLOCK(&dp_lock);
	return DP_SUCCESS;
RX_DROP:
	DP_LIB_LOCK(&dp_lock);
	dp_port_info[port_id].rx_err_drop++;
	DP_LIB_UNLOCK(&dp_lock);
RX_DROP2:
	dev_kfree_skb_any(skb);
	return res;
}

#define PROTOCOL_IPIP 4
#define PROTOCOL_TCP 6
#define PROTOCOL_UDP 17
#define PROTOCOL_ENCAPSULATED_IPV6 41
#define PROTOCOL_ROUTING 43
#define PROTOCOL_NONE 59
#define PROTOCOL_IPV6_FRAGMENT 44

#define TWO_MAC_SIZE 12
#define VLAN_HDR_SIZE  4
#define PPPOE_HDR_SIZE  8
#define IPV6_HDR_SIZE  40
#define IPV6_EXTENSION_SIZE 8

#define IP_CHKSUM_OFFSET_IPV4 10
#define UDP_CHKSUM_OFFSET 6
#define TCP_CHKSUM_OFFSET 16
/*Workaround: Currently need to includes PMAC
**although spec said it starts from mac address. ?
*/

struct ip_hdr_info {
	u8 ip_ver;
	u8 proto;		/*udp/tcp */
	u16 ip_offset;		/*this offset is based on L2 MAC header */
	u16 udp_tcp_offset;	/*this offset is based on ip header */
	u16 next_ip_hdr_offset;	/*0 - means no next valid ip header.*/
				/* Based on current IP header */
	u8 is_fragment;		/*0 means non fragmented packet */
};

/*input p: pointers to ip header
* output info:
* return: 0:  it is UDP/TCP packet
* -1: not UDP/TCP
*/
#define DP_IP_VER4 4
#define DP_IP_VER6 6
int get_ip_hdr_info(u8 *pdata, int len, struct ip_hdr_info *info)
{
	int ip_hdr_size;
	u8 *p = pdata;
	struct iphdr *iphdr = (struct iphdr *)pdata;

	memset((void *)info, 0, sizeof(*info));
	info->ip_ver = p[0] >> 4;

	if (info->ip_ver == DP_IP_VER4) {	/*ipv4 */
		ip_hdr_size = (p[0] & 0xf) << 2;
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
			 "IPV4 pkt with protocol 0x%x with ip hdr size %d\n",
			 p[9], ip_hdr_size);
#endif
		info->proto = p[9];

		if ((info->proto == PROTOCOL_UDP) ||
			(info->proto == PROTOCOL_TCP)) {
			if ((iphdr->frag_off & IP_MF) ||
			    (iphdr->frag_off & IP_OFFSET)) {
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
				DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				  "frag pkt:off=%x,IP_MF=%x,IP_OFFSET=%x\n",
					iphdr->frag_off, IP_MF, IP_OFFSET);
#endif
				info->udp_tcp_offset = (p[0] & 0x0f) << 2;
				info->is_fragment = 1;
				return -1;
			}
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				 "%s packet with src/dst port:%u/%u\n",
				 (p[9] ==
				  PROTOCOL_UDP) ? "UDP" : "TCP",
				 *(unsigned short *)(pdata +
						     ip_hdr_size),
				 *(unsigned short *)(pdata +
						     ip_hdr_size +
						     2));
#endif
			info->udp_tcp_offset = (p[0] & 0x0f) << 2;
			return 0;
		} else if (p[9] == PROTOCOL_ENCAPSULATED_IPV6) {
			/*6RD */
			info->next_ip_hdr_offset = (p[0] & 0x0f) << 2;
			return 0;
		}
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
			 "Not supported extension hdr:0x%x\n", p[9]);
#endif
		return -1;
	} else if (info->ip_ver == DP_IP_VER6) {	/*ipv6 */
		int i;
		int ip_hdr_size;
		u8 next_hdr;
		u8 udp_tcp_h_offset;
		u8 first_extension = 1;

		ip_hdr_size = IPV6_HDR_SIZE;
		udp_tcp_h_offset = IPV6_HDR_SIZE;
		next_hdr = p[6];
		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX) {
			PRINTK("IPV6 packet with next hdr:0x%x\n", next_hdr);
			PRINTK(" src IP: ");

			for (i = 0; i < 16; i++)
				PRINTK("%02x%s", pdata[8 + i],
				       (i != 15) ? ":" : " ");

			PRINTK("\n");

			PRINTK(" Dst IP: ");

			for (i = 0; i < 16; i++)
				PRINTK("%02x%s", pdata[24 + i],
				       (i != 15) ? ":" : " ");

			PRINTK("\n");
		}

		while (1) {
			/*Next Header: UDP/TCP */
			if ((next_hdr == PROTOCOL_UDP) ||
			    (next_hdr == PROTOCOL_TCP)) {
				info->proto = next_hdr;

				if (!first_extension)
					udp_tcp_h_offset +=
					    IPV6_EXTENSION_SIZE + p[1];

				info->udp_tcp_offset = udp_tcp_h_offset;
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
				DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				 "IP6 UDP:src/dst port:%u/%u udp_tcp_off=%d\n",
					 *(unsigned short *)(pdata +
							     udp_tcp_h_offset),
					 *(unsigned short *)(pdata +
							     udp_tcp_h_offset
							     + 2),
					 udp_tcp_h_offset);
#endif
				return 0;
			} else if (next_hdr == PROTOCOL_IPIP) {	/*dslite */
				if (!first_extension)
					udp_tcp_h_offset +=
					    IPV6_EXTENSION_SIZE + p[1];

				info->next_ip_hdr_offset = udp_tcp_h_offset;
				return 0;
			} else if (next_hdr == PROTOCOL_IPV6_FRAGMENT) {
				pr_info_once("fragmented IPV6 packet !\n");
				info->is_fragment = 1;
				return -1;
			}
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				 "Skip extension hdr:0x%x\n", next_hdr);
#endif
			if ((next_hdr == PROTOCOL_NONE) ||
			    (next_hdr == PROTOCOL_ENCAPSULATED_IPV6))
				break;

			if (first_extension) {
				/*skip ip header */
				p += IPV6_HDR_SIZE;
				first_extension = 0;
			} else {
				/*TO NEXT */
				udp_tcp_h_offset +=
				    IPV6_EXTENSION_SIZE + p[1];
				p += IPV6_EXTENSION_SIZE + p[1];
			}
			next_hdr = p[0];
			if (udp_tcp_h_offset > len) {
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
				DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				 "\n- Wrong IPV6 packet header ?\n");
#endif
				break;
			}
		}
	}

	/*not supported protocol */
	return -1;
}

#ifdef CONFIG_LTQ_DATAPATH_MANUAL_PARSE
#define IP_OFFSET_HW_ADJUST 8

/*parse protol and get the ip_offset/tcp_h_offset and its type:
* return: 0-found udp/tcp header, -1 - not found  udp/tcp header
* Note: skb->data points to pmac header, not L2 MAC header;
*/
int get_offset_clear_chksum(struct sk_buff *skb, u32 *ip_offset,
			    u32 *tcp_h_offset, u32 *tcp_type)
{
	u8 *p_l2_mac = skb->data + sizeof(struct pmac_tx_hdr);
	u8 *p = p_l2_mac + TWO_MAC_SIZE;
	struct ip_hdr_info pkt_info[2];
	u8 ip_num = 0;
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
	int i;
#endif
	int len;

	if (skb->ip_summed != CHECKSUM_PARTIAL)
		return -1;

	*ip_offset = 0;
	*tcp_h_offset = 0;

#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
	if (dp_dbg_flag)
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
			 "flags DP_TX_CAL_CHKSUM is set\n");
#endif

	while ((p[0] == 0x81) && (p[1] == 0x00))	/*skip vlan header */
		p += VLAN_HDR_SIZE;

	if ((p[0] == 0x88) && (p[1] == 0x64))	/*skip pppoe header */
		p += PPPOE_HDR_SIZE;

#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
	DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
		 "To find ip header:%02x %02x %02x %02x %02x %02x %02x %02x\n",
		 p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
#endif
	if (((p[0] != 0x08) || (p[1] != 0x00)) &&
	    ((p[0] != 0x86) && (p[1] != 0xdd))) {
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "None IP type:%02x%02x\n", p[0],
			 p[1]);
#endif
	}

	p += 2;			/* jump to ip header */
	len = skb->len - TWO_MAC_SIZE - 2;

	while (1) {
		if (get_ip_hdr_info(p, len, &pkt_info[ip_num]) == 0) {
			pkt_info[ip_num].ip_offset = (u32)p - (u32)p_l2_mac;

			if (pkt_info[ip_num].next_ip_hdr_offset) {
				p += pkt_info[ip_num].next_ip_hdr_offset;
				ip_num++;

				if (ip_num >= ARRAY_SIZE(pkt_info))
					return -1;

				len -= pkt_info[ip_num].next_ip_hdr_offset;
				continue;

			} else {
				ip_num++;

				if (ip_num >= ARRAY_SIZE(pkt_info))
					return -1;

				break;
			}
		} else {
			/*Not UDP/TCP and cannot do checksum calculation */
			pr_info_once
			    ("Not UDP/TCP and cannot do checksum cal!\n");
			return -1;
		}
	}
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
	if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX) {
		for (i = 0; i < ip_num; i++) {
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
			 "Parse:ip[%d]:v=%d prot=%d ip_off=%d udp_tcp_off=%d, n_hdr_off=%d\n",
				 i, pkt_info[i].ip_ver, pkt_info[i].proto,
				 pkt_info[i].ip_offset,
				 pkt_info[i].udp_tcp_offset,
				 pkt_info[i].next_ip_hdr_offset);
		}
	}
#endif
	if (ip_num == 1) {
		if (pkt_info[0].ip_ver == DP_IP_VER4) {
			*ip_offset = pkt_info[0].ip_offset;
			*tcp_h_offset = pkt_info[0].udp_tcp_offset;

			if (pkt_info[0].proto == PROTOCOL_UDP) {
				*tcp_type = UDP_OVER_IPV4;
				/*clear original udp checksum */
				if (!pkt_info[0].is_fragment)
					*(uint16_t *)(p_l2_mac + *ip_offset +
						       *tcp_h_offset +
						       UDP_CHKSUM_OFFSET) = 0;
			} else {
				*tcp_type = TCP_OVER_IPV4;
				/*clear original TCP checksum */
				*(uint16_t *)(p_l2_mac + *ip_offset +
					       *tcp_h_offset +
					       TCP_CHKSUM_OFFSET) = 0;
			}

			if (!pkt_info[0].is_fragment) {
				/*clear original ip4 checksum */
				*(uint16_t *)(p_l2_mac + *ip_offset +
					       IP_CHKSUM_OFFSET_IPV4) = 0;
			} else {
				return 1;
			}

			return 0;
		} else if (pkt_info[0].ip_ver == DP_IP_VER6) {
			*ip_offset = pkt_info[0].ip_offset;
			*tcp_h_offset = pkt_info[0].udp_tcp_offset;

			if (pkt_info[0].proto == PROTOCOL_UDP) {
				*tcp_type = UDP_OVER_IPV6;
				if (!pkt_info[0].is_fragment)
					/*clear original udp checksum */
					*(uint16_t *)(p_l2_mac + *ip_offset +
						       *tcp_h_offset +
						       UDP_CHKSUM_OFFSET) = 0;
			} else {
				*tcp_type = TCP_OVER_IPV6;
				/*clear original TCP checksum */
				if (!pkt_info[0].is_fragment) {
					*(uint16_t *)(p_l2_mac + *ip_offset +
						       *tcp_h_offset +
						       TCP_CHKSUM_OFFSET) = 0;
				} else {
					return 1;
				}
			}

			return 0;
		}
	} else if (ip_num == 2) {
		/*for tunnels:current for 6rd/dslite only */
		if ((pkt_info[0].ip_ver == DP_IP_VER4) &&
		   (pkt_info[1].ip_ver == DP_IP_VER6)) {
			/*6rd */
			*ip_offset = pkt_info[0].ip_offset;
			*tcp_h_offset =
			    (pkt_info[0].next_ip_hdr_offset +
			     pkt_info[1].udp_tcp_offset);

			if (pkt_info[1].proto == PROTOCOL_UDP) {
				*tcp_type = UDP_OVER_IPV6_IPV4;
				/*clear original udp checksum */
				if (!pkt_info[0].is_fragment)
					*(uint16_t *)(p_l2_mac + *ip_offset +
						       *tcp_h_offset +
						       UDP_CHKSUM_OFFSET) = 0;
			} else {
				*tcp_type = TCP_OVER_IPV6_IPV4;
				/*clear original udp checksum */
				*(uint16_t *)(p_l2_mac + *ip_offset +
					       *tcp_h_offset +
					       TCP_CHKSUM_OFFSET) = 0;
			}

			if (!pkt_info[0].is_fragment) {
				/*clear original ip4 checksum */
				*(uint16_t *)(p_l2_mac + *ip_offset +
					       IP_CHKSUM_OFFSET_IPV4) = 0;
			} else {
				return 1;
			}

			return 0;

		} else if ((pkt_info[0].ip_ver == DP_IP_VER6) &&
			(pkt_info[1].ip_ver == DP_IP_VER4)) {	/*dslite */
			*ip_offset = pkt_info[0].ip_offset;
			*tcp_h_offset =
			    (pkt_info[0].next_ip_hdr_offset +
			     pkt_info[1].udp_tcp_offset);

			if (pkt_info[1].proto == PROTOCOL_UDP) {
				*tcp_type = UDP_OVER_IPV4_IPV6;
				if (!pkt_info[0].is_fragment)
					/*clear original udp checksum */
					*(uint16_t *)(p_l2_mac +
						       pkt_info[1].ip_offset +
						       *tcp_h_offset +
						       UDP_CHKSUM_OFFSET) = 0;
			} else {
				*tcp_type = TCP_OVER_IPV4_IPV6;
				/*clear original udp checksum */
				*(uint16_t *)(p_l2_mac +
					       pkt_info[1].ip_offset +
					       pkt_info[1].udp_tcp_offset +
					       TCP_CHKSUM_OFFSET) = 0;
			}

			if (!pkt_info[0].is_fragment) {
				/*clear original ip4 checksum */
				*(uint16_t *)(p_l2_mac +
					       pkt_info[1].ip_offset +
					       IP_CHKSUM_OFFSET_IPV4) = 0;
			} else {
				return 1;
			}

			return 0;
		}
	}

	return -1;
}
#else				/* CONFIG_LTQ_DATAPATH_MANUAL_PARSE */
/*parse protol and get the ip_offset/tcp_h_offset and its type
* based on skb_inner_network_header/skb_network_header/
*           skb_inner_transport_header/skb_transport_header
* return: 0-found udp/tcp header, -1 - not found  udp/tcp header
*  Note: skb->data points to pmac header, not L2 MAC header;
*/
#define IP_OFFSET_HW_ADJUST  0

int get_offset_clear_chksum(struct sk_buff *skb, u32 *ip_offset,
			    u32 *tcp_h_offset, u32 *tcp_type)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	struct udphdr *udph;
	unsigned char *l4_p;

	if (skb->ip_summed != CHECKSUM_PARTIAL) {
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
			 "No need HW checksum Support\n");
#endif
		return -1;
	}

	if (skb->encapsulation) {
		iph = (struct iphdr *)skb_inner_network_header(skb);
		*ip_offset =
		    (uint32_t)(skb_inner_network_header(skb) - skb->data);
		*tcp_h_offset =
		    (uint32_t)(skb_inner_transport_header(skb) -
				skb_inner_network_header(skb));
		l4_p = skb_inner_transport_header(skb);
	} else {
		iph = (struct iphdr *)skb_network_header(skb);
		*ip_offset = (uint32_t)(skb_network_header(skb) - skb->data);
		*tcp_h_offset =
		    (uint32_t)(skb_transport_header(skb) -
				skb_network_header(skb));
		l4_p = skb_transport_header(skb);
	}
	if (((int)(ip_offset) <= 0) || ((int)(tcp_h_offset) <= 0)) {
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
			 "Wrong IP offset(%d) or TCP/UDP offset(%d)\n",
			 ((int)(ip_offset) <= 0), ((int)(tcp_h_offset) <= 0));
#endif
		return -1;
	}

	if (iph->protocol == IPPROTO_UDP) {
		if (iph->version == DP_IP_VER4) {
			*tcp_type = UDP_OVER_IPV4;
			iph->check = 0;	/*clear original ip checksum */
		} else if (iph->version == DP_IP_VER6) {
			*tcp_type = UDP_OVER_IPV6;
		} else { /*wrong ver*/
			return -1;
		}
		udph = (struct udphdr *)l4_p;
		udph->check = 0; /*clear original UDP checksum */
	} else if (iph->protocol == IPPROTO_TCP) {
		if (iph->version == DP_IP_VER4) {
			*tcp_type = TCP_OVER_IPV4;
			iph->check = 0;	/*clear original ip checksum */
		} else if (iph->version == DP_IP_VER6) {
			*tcp_type = TCP_OVER_IPV6;
		} else {
			return -1;
		}
		tcph = (struct tcphdr *)l4_p;
		tcph->check = 0;	/*clear original UDP checksum */
	}
#ifdef CONFIG_LTQ_DATAPATH_DBG_PROTOCOL_PARSE
	DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM, "Found tcp_type=%u ip_offset=%u\n",
		 *tcp_type, *ip_offset);
#endif
	return 0;
}
#endif				/* CONFIG_LTQ_DATAPATH_MANUAL_PARSE */

/*Don't know why kernel does not define skb_inner_transport_offset */
static inline int skb_inner_transport_offset(const struct sk_buff *skb)
{
	return skb_inner_transport_header(skb) - skb->data;
}

/*  Make a copy of both an &sk_buff and part of its data, located
 * in header. Fragmented data remain shared. This is used since
 * datapath need to modify only header of &sk_buff and needs
 * private copy of the header to alter.
 *  Returns NULL on failure, or the pointer to the buffer on success
 *  Note, this API will used in dp_xmit when there is no enough room
 *        to insert pmac header or the packet is cloned but we need
 *        to insert pmac header or reset udp/tcp checksum
 *  This logic is mainly copied from API __pskb_copy(...)
*/
struct sk_buff *dp_create_new_skb(struct sk_buff *skb)
{
	struct sk_buff *new_skb;
#ifndef CONFIG_LTQ_DATAPATH_COPY_LINEAR_BUF_ONLY
	/* seems CBM driver does not support it yet */
	void *p;
	const skb_frag_t *frag;
	int len;
#else
	int linear_len;
#endif
	int i;

	if (unlikely(skb->data_len >= skb->len)) {
		PR_ERR("why skb->data_len(%d) >= skb->len(%d)\n",
		       skb->data_len, skb->len);
		dev_kfree_skb_any(skb);
		return NULL;
	}

	if (skb_shinfo(skb)->frag_list) {
		PR_ERR("DP Not support skb_shinfo(skb)->frag_list yet !!\n");
		dev_kfree_skb_any(skb);
		return NULL;
	}
#ifndef CONFIG_LTQ_DATAPATH_COPY_LINEAR_BUF_ONLY
	new_skb = cbm_alloc_skb(skb->len + 8, GFP_ATOMIC);
#else
	linear_len = skb->len - skb->data_len;
	/*cbm_alloc_skb will reserve enough header room */
	new_skb = cbm_alloc_skb(linear_len, GFP_ATOMIC);
#endif

	if (unlikely(!new_skb)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "allocate cbm buffer fail\n");
		dev_kfree_skb_any(skb);
		return NULL;
	}
#ifndef CONFIG_LTQ_DATAPATH_COPY_LINEAR_BUF_ONLY
	p = new_skb->data;
	dp_memcpy(p, skb->data, skb->len - skb->data_len);
	p += skb->len - skb->data_len;

	if (skb->data_len) {
		for (i = 0; i < (skb_shinfo(skb)->nr_frags); i++) {
			frag = &skb_shinfo(skb)->frags[i];
			len = skb_frag_size(frag);
			dp_memcpy(p, skb_frag_address(frag), len);
			p += len;
		}
	}
	skb_put(new_skb, skb->len);
#else
	/* Copy the linear data part only */
	memcpy(new_skb->data, skb->data, linear_len);
	skb_put(new_skb, linear_len);

	/*Share the Fragmented data */
	if (skb_shinfo(skb)->nr_frags) {
		for (i = 0; i < skb_shinfo(skb)->nr_frags; i++) {
			skb_shinfo(new_skb)->frags[i] =
			    skb_shinfo(skb)->frags[i];
			skb_frag_ref(skb, i);	/*increase counter */
		}
		skb_shinfo(new_skb)->nr_frags = i;
		skb_shinfo(new_skb)->gso_size = skb_shinfo(skb)->gso_size;
		skb_shinfo(new_skb)->gso_segs = skb_shinfo(skb)->gso_segs;
		skb_shinfo(new_skb)->gso_type = skb_shinfo(skb)->gso_type;
		new_skb->data_len = skb->data_len;
		new_skb->len += skb->data_len;
	}
#endif
	new_skb->dev = skb->dev;
	new_skb->priority = skb->priority;
	new_skb->truesize += skb->data_len;
	new_skb->DW0 = skb->DW0;
	new_skb->DW1 = skb->DW1;
	new_skb->DW2 = skb->DW2;
	new_skb->DW3 = skb->DW3;

	/*copy other necessary fields for checksum calculation case */
	new_skb->ip_summed = skb->ip_summed;
	new_skb->encapsulation = skb->encapsulation;
	new_skb->inner_mac_header = skb->inner_mac_header;
	new_skb->protocol = skb->protocol;

	if (skb->encapsulation) {
		skb_reset_inner_network_header(new_skb);
		skb_set_inner_network_header(new_skb,
					     skb_inner_network_offset(skb));
		skb_reset_transport_header(new_skb);
		skb_set_inner_transport_header(new_skb,
					       skb_inner_transport_offset
					       (skb));
	} else {
		skb_reset_network_header(new_skb);
		skb_set_network_header(new_skb, skb_network_offset(skb));
		skb_reset_transport_header(new_skb);
		skb_set_transport_header(new_skb, skb_transport_offset(skb));
	}

	/*DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "alloc new buffer succeed\n");*/
	/*free old skb */
	dev_kfree_skb_any(skb);
	return new_skb;
}

char *dp_skb_csum_str(struct sk_buff *skb)
{
	if (!skb)
		return "NULL";
	if (skb->ip_summed == CHECKSUM_PARTIAL)
		return "HW Checksum";
	if (skb->ip_summed == CHECKSUM_NONE)
		return "SW Checksum";
	return "Unknown";
}

int32_t dp_xmit(struct net_device *rx_if, dp_subif_t *rx_subif,
		struct sk_buff *skb, int32_t len, uint32_t flags)
{
	struct dma_tx_desc_0 *desc_0;
	struct dma_tx_desc_1 *desc_1;
	struct dma_tx_desc_2 *desc_2;
	struct dma_tx_desc_3 *desc_3;
	struct pmac_tx_hdr *pmac = NULL;
	u32 ip_offset, tcp_h_offset, tcp_type;
	int tx_chksum_flag = 0;	/*check checksum cal can be supported or not */
	int insert_pmac_f = 1;	/*flag to insert one pmac */
	int res = DP_SUCCESS;
	int ep, vap;
	int clone_f, no_hdr_room_f;

	if (unlikely(!skb)) {
		PR_RATELIMITED("skb NULL");
		return DP_FAILURE;
	}
	if (unlikely(!dp_init_ok)) {
		dev_kfree_skb_any(skb);
		PR_RATELIMITED("dp_xmit failed for datapath not init yet\n");
		return DP_FAILURE;
	}
	if (unlikely(!rx_subif)) {
		dev_kfree_skb_any(skb);
		PR_RATELIMITED("dp_xmit failed for rx_subif null\n");
		DP_LIB_LOCK(&dp_lock);
		dp_port_info[0].tx_err_drop++;
		DP_LIB_UNLOCK(&dp_lock);
		return DP_FAILURE;
	}
	ep = rx_subif->port_id;
	vap = get_vap(rx_subif->subif);
	if (unlikely(ep >= PMAC_MAX_NUM)) {
		dev_kfree_skb_any(skb);
		DP_LIB_LOCK(&dp_lock);
		dp_port_info[0].tx_err_drop++;
		DP_LIB_UNLOCK(&dp_lock);
		PR_RATELIMITED("rx_subif->port_id >= PMAC_MAX_NUM");
		return DP_FAILURE;
	}
	if (unlikely(in_irq())) {
		PR_RATELIMITED
		    ("Not allowed to call dp_xmit in interrupt context\n");
		dev_kfree_skb_any(skb);
		DP_LIB_LOCK(&dp_lock);
		dp_port_info[ep].tx_err_drop++;
		dp_port_info[ep].subif_info[vap].mib.tx_pkt_dropped++;
		DP_LIB_UNLOCK(&dp_lock);
		return DP_FAILURE;
	}
	if (unlikely(!rx_if &&	/*For atm pppoa case, rx_if is NULL now */
		     !(dp_port_info[ep].alloc_flags & DP_F_FAST_DSL))) {
		dev_kfree_skb_any(skb);
		DP_LIB_LOCK(&dp_lock);
		dp_port_info[ep].subif_info[vap].mib.tx_pkt_dropped++;
		DP_LIB_UNLOCK(&dp_lock);
		PR_RATELIMITED("rx_if NULL");
		return DP_FAILURE;
	}
#ifdef CONFIG_LTQ_DP_MPE_FASTHOOK_TEST
	if (unlikely(ltq_mpe_fasthook_tx_fn))
		ltq_mpe_fasthook_tx_fn(skb, 0, NULL);
#endif

	/*No PMAC for WAVE500 and DSL by default except bonding case */
	if ((dp_port_info[ep].alloc_flags &
		(DP_F_FAST_WLAN | DP_F_FAST_DSL)) &&
		!(flags & (DP_TX_CAL_CHKSUM | DP_TX_DSL_FCS)))
		insert_pmac_f = 0;

	if (unlikely(dp_dbg_flag)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
			 "dp_xmit:skb->data/len=0x%p/%d data_ptr=%x from port=%d and subitf=%d\n",
			 skb->data, len,
			 ((struct dma_tx_desc_2 *)&skb->DW2)->field.data_ptr,
			 ep, rx_subif->subif);

		DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
			 "dp_xmit:skb->data/len=0x%p/%d data_ptr=%x from port=%d and subitf=%d\n",
			 skb->data, len,
			 ((struct dma_tx_desc_2 *)&skb->DW2)->field.data_ptr,
			 ep, rx_subif->subif);

		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX_DATA)
			dp_dump_raw_data(skb->data,
					 (skb->len >
					  print_len) ? skb->len : print_len,
					 "Tx Orig Data");

		DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
			 "ip_summed=%s(%d) encapsulation=%s\n",
			 dp_skb_csum_str(skb), skb->ip_summed,
			 skb->encapsulation ? "Yes" : "No");
		if (skb->encapsulation)
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
				 "inner ip start=0x%x(%d), transport=0x%x(%d)\n",
				 (unsigned int)skb_inner_network_header(skb),
				 (int)(skb_inner_network_header(skb) -
				       skb->data),
				 (unsigned int)
				 skb_inner_transport_header(skb),
				 (int)(skb_inner_transport_header(skb) -
				       skb_inner_network_header(skb)));
		else
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX_SUM,
				 "ip start=0x%x(%d), transport=0x%x(%d)\n",
				 (unsigned int)(unsigned int)
				 skb_network_header(skb),
				 (int)(skb_network_header(skb) - skb->data),
				 (unsigned int)skb_transport_header(skb),
				 (int)(skb_transport_header(skb) -
				       skb_network_header(skb)));

		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX_DESCRIPTOR)
			dump_tx_dma_desc((struct dma_tx_desc_0 *)&skb->DW0,
					 (struct dma_tx_desc_1 *)&skb->DW1,
					 (struct dma_tx_desc_2 *)&skb->DW2,
					 (struct dma_tx_desc_3 *)&skb->DW3);

		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "flags=0x%x skb->len=%d\n",
			 flags, skb->len);
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
			 "skb->data=0x%p with pmac hdr size=%u\n", skb->data,
			 sizeof(struct pmac_tx_hdr));
		if (insert_pmac_f) /*insert one pmac header */
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				"flags DP_TX_INSERT_PMAC is set\n");
	}

	if (insert_pmac_f) {	/*insert one pmac header */
		clone_f = skb_cloned(skb);
		no_hdr_room_f =
			skb_headroom(skb) < sizeof(struct pmac_tx_hdr) ? 1 : 0;

		if (clone_f && !skb_is_gso(skb)) {
			if (clone_f && (skb->data[0] & 1)) {
				/*not boradcast/mc */
				DP_LIB_LOCK(&dp_lock);
				dp_port_info[ep].subif_info[vap].mib.
				    tx_clone_pkt++;
				DP_LIB_UNLOCK(&dp_lock);
			}
			/*make sure dp_create_new_skb copied correct */
			/* flags like ip_summed, encapsulation and so on */
			skb = dp_create_new_skb(skb);

			if (unlikely(!skb)) {
				PR_INFO_ONCE("dp_create_new_skb failed\n");
				return DP_FAILURE;
			}
		} else if (no_hdr_room_f) {
			PR_INFO_ONCE
			    ("dp_xmit:no header room to insert pmac:%s ?\n",
			     rx_if ? rx_if->name : "NULL");
			DP_LIB_LOCK(&dp_lock);
			dp_port_info[ep].subif_info[vap].mib.
			    tx_hdr_room_pkt++;
			DP_LIB_UNLOCK(&dp_lock);
			/*make sure dp_create_new_skb copied correct flags */
			/*like ip_summed encapsulation and so on */
			skb = dp_create_new_skb(skb);
			if (unlikely(!skb)) {
				PR_INFO_ONCE("dp_create_new_skb failed\n");
				return DP_FAILURE;
			}
		}

		skb_push(skb, sizeof(struct pmac_tx_hdr));
		pmac = (struct pmac_tx_hdr *)(skb->data);
		memset(pmac, 0, sizeof(struct pmac_tx_hdr));
	}

	desc_0 = (struct dma_tx_desc_0 *)&skb->DW0;
	desc_1 = (struct dma_tx_desc_1 *)&skb->DW1;
	desc_2 = (struct dma_tx_desc_2 *)&skb->DW2;
	desc_3 = (struct dma_tx_desc_3 *)&skb->DW3;

	if (flags & DP_TX_CAL_CHKSUM) {
		int ret_flg;

		ret_flg =
		    get_offset_clear_chksum(skb, &ip_offset, &tcp_h_offset,
					    &tcp_type);
		if (ret_flg == 0) {
			pr_debug("packet CAN do hw checksum\n");
			tx_chksum_flag = 1;
		} else if (ret_flg == -1) {
			pr_info_once("packet cant do hw checksum\n");
		}
	}

	/*reset all descriptors as SWAS required since SWAS 3.7 */
	/*As new SWAS 3.7 required, MPE1/Color/FlowID is set by applications */
	desc_0->all &= dma_tx_desc_mask0.all;
	desc_1->all &= dma_tx_desc_mask1.all;
	desc_2->all = 0;
	desc_3->all = 0;

	if (flags & DP_TX_OAM)
		desc_3->field.pdu_type = 1;

	desc_1->field.classid = (skb->priority >= 15) ? 15 : skb->priority;
	desc_2->field.data_ptr = (uint32_t)skb->data;

	/*for ETH LAN/WAN */
	if (dp_port_info[ep].
	    alloc_flags & (DP_F_FAST_ETH_LAN | DP_F_FAST_ETH_WAN)) {
		if (pmac) {
			pmac->port_map_en = 1;
			SET_PMAC_PORTMAP(pmac, ep);
			SET_PMAC_SUBIF(pmac, rx_subif->subif);
			pmac->sppid = PMAC_CPU_ID;	/*must set cpu port */

			if (dp_port_info[ep].alloc_flags & DP_F_FAST_ETH_WAN)
				pmac->redirect = 1;
			else
				pmac->redirect = 0;
		} else
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				 "Why !pmac for ETH Port?\n");
	} else if (dp_port_info[ep].
		   alloc_flags & (DP_F_DIRECT | DP_F_FAST_WLAN | DP_F_FAST_DSL
				  | DP_F_DIRECTLINK)) {
		if (flags & DP_TX_TO_DL_MPEFW) {
			if (pmac) {
				pmac->port_map_en = 1;
				SET_PMAC_PORTMAP(pmac, ep);
				pmac->sppid = PMAC_CPU_ID;
				SET_PMAC_SUBIF(pmac, rx_subif->subif);
			}
#if defined(CONFIG_ACCL_11AC_CS2) || defined(CONFIG_ACCL_11AC_CS2_MODULE)
			/* CPU traffic to PAE via cbm to apply PCE rule */
			desc_1->field.enc = 1;
			desc_1->field.dec = 1;
			desc_1->field.mpe2 = 0;
#else
			desc_1->field.enc = 0;
			desc_1->field.dec = 0;
			desc_1->field.mpe2 = 0;
#endif
		} else if (flags & (DP_TX_CAL_CHKSUM | DP_TX_DSL_FCS)) {
			/*checksum/fcs(bonding) queue */
			if (pmac) {
				pmac->port_map_en = 1;
				/*specify destination */
				SET_PMAC_PORTMAP(pmac, ep);
				pmac->sppid = PMAC_CPU_ID;
				pmac->redirect = 1;
				SET_PMAC_SUBIF(pmac, rx_subif->subif);
			} else
				DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
					 "Why !pmac for Checksum/Redirect?\n");

			desc_1->field.enc = 1;
			desc_1->field.dec = 1;
			desc_1->field.mpe2 = 1;
#ifdef CONFIG_LTQ_DP_ACA_CSUM_WORKAROUND
			if (aca_portid > 0)
				desc_1->field.ep = aca_portid;
#endif
		} else if (dp_port_info[ep].alloc_flags & DP_F_FAST_DSL) {
			/* VRX318 queue without bonding
			 If it is bondig, it should with DP_TX_DSL_FCS flag
			*/
			desc_1->field.enc = 0;
			desc_1->field.dec = 0;
			desc_1->field.mpe2 = 0;
		} else {	/*normal directpath queue */
			if (pmac) {
				pmac->port_map_en = 0;
				pmac->port_map = 0xff;
				pmac->port_map2 = 0xff;
				pmac->sppid = ep;
				SET_PMAC_SUBIF(pmac, rx_subif->subif);
			} else
				DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
					 "Why !pmac for normal directpath ?\n");

			desc_1->field.enc = 1;
			desc_1->field.dec = 1;
			desc_1->field.mpe2 = 0;
		}
	} else {
		dev_kfree_skb_any(skb);
		dp_port_info[ep].subif_info[vap].mib.tx_pkt_dropped++;
		PR_INFO_ONCE("Why come to here:%x\n",
			     dp_port_info[ep].status);
		return DP_FAILURE;
	}

	if (tx_chksum_flag) {	/*definately have pmac if tx_chksum_flag == 1 */
		if (pmac) {
			pmac->tcp_chksum = 1;
			pmac->tcp_type = tcp_type;
			pmac->ip_offset = ip_offset + IP_OFFSET_HW_ADJUST;
			pmac->tcp_h_offset = tcp_h_offset >> 2;
		} else
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				 "Why !pmac for Checksum?\n");
	}

	desc_3->field.data_len = skb->len;
#if 0
	if (flags & DP_TX_DSL_FCS) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX, "flags DP_TX_DSL_FCS is set\n");
		if (likely(pmac))
			pmac->fcs_ins_dis = 1;
		else
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				 "Why !pmac for DP_TX_DSL_FCS\n");
	}
#endif

	if (unlikely(dp_dbg_flag)) {
		DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
			 "pmac=0x%p data_len=%u skb->len=%u\n", pmac,
			 desc_3->field.data_len, skb->len);

		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX_DESCRIPTOR) {
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX_DESCRIPTOR,
				 "Final DMA descriptor after modification:\n");
			dump_tx_dma_desc((struct dma_tx_desc_0 *)&skb->DW0,
					 (struct dma_tx_desc_1 *)&skb->DW1,
					 (struct dma_tx_desc_2 *)&skb->DW2,
					 (struct dma_tx_desc_3 *)&skb->DW3);
		}
		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX_PMAC && pmac) {
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX_PMAC,
				 "Final PMAC header after modification:\n");
			dump_tx_pmac(pmac);
		}
		if (dp_dbg_flag & DP_DBG_FLAG_DUMP_TX_DATA)
			dp_dump_raw_data(skb->data,
					 (skb->len >
					  print_len) ? skb->len : print_len,
					 "Final Data");
	}
#ifdef CONFIG_LTQ_TOE_DRIVER
	if (skb_is_gso(skb)) {
#ifdef LTQ_TSO_SW_WORKAROUND
		/*Need TSO segmentation */
		/* Hack in the PMAC header to make the ip_offset to 14 */
		if (pmac)
			pmac->ip_offset = ip_offset - 8;
		else
			DP_DEBUG(DP_DBG_FLAG_DUMP_TX,
				 "Why !pmac for TSO?\n");

		desc_0->field.flow_id = 1 << 6;
		desc_1->field.enc = 1;
		desc_1->field.dec = 1;
		desc_1->field.mpe2 = 1;
#else
		res = ltq_tso_xmit(skb, desc_1->field.ep, 0);
		DP_LIB_LOCK(&dp_lock);
		dp_port_info[ep].subif_info[vap].mib.tx_tso_pkt++;
		DP_LIB_UNLOCK(&dp_lock);
		return res;
#endif /* LTQ_TSO_SW_WORKAROUND */
	}
#endif /* CONFIG_LTQ_TOE_DRIVER */

	if (pmac)
		pmac->class_en = 1;
	if (unlikely(!desc_1->field.ep)) {
		/*Not allowed to send out pkt with ep zero */
		PR_ERR("Why ep zero in dp_xmit for device name %s\n",
		       skb->dev ? skb->dev->name : "NULL");
		dev_kfree_skb_any(skb);
		return DP_FAILURE;
	}
	res = cbm_cpu_pkt_tx(skb, desc_1->field.ep, 0);
	DP_LIB_LOCK(&dp_lock);
	dp_port_info[ep].subif_info[vap].mib.tx_cbm_pkt++;
	DP_LIB_UNLOCK(&dp_lock);
	return res;
}
EXPORT_SYMBOL(dp_xmit);

void set_dp_dbg_flag(uint32_t flags)
{
	dp_dbg_flag = flags;
}

uint32_t get_dp_dbg_flag(void)
{
	return dp_dbg_flag;
}

#ifdef DP_TEST_EXAMPLE
void test(void)
{
	/* Base on below example data, it should print like below log
	*DMA Descripotr:D0=0x00004000 D1=0x00001000 D2=0xa0c02080 D3=0xb0000074
	*DW0:resv0=0 tunnel_id=00 flow_id=0 eth_type=0 dest_sub_if_id=0x4000
	*DW1:session_id=0x000 tcp_err=0 nat=0 dec=0 enc=0 mpe2=0 mpe1=0
	*color=01 ep=00 resv1=0 classid=00
	*DW2:data_ptr=0xa0c02080
	*DW3:own=1 c=0 sop=1 eop=1 dic=0 pdu_type=0
	*byte_offset=0 atm_qid=0 mpoa_pt=0 mpoa_mode=0 data_len= 116
	*paser flags: 00 00 00 00 80 18 80 00
	*paser flags: 00 80 18 80 00 00 00 00 (reverse)
	*flags 15 offset=14: PASER_FLAGS_1IPV4
	*flags 19 offset=22: PASER_FLAGS_ROUTEXP
	*flags 20 offset=34: PASER_FLAGS_TCP
	*flags 31 offset=46: PASER_FLAGS_LROEXP
	*pmac:0x4e 0x28 0xf0 0x00 0x00 0x00 0x00 0x01
	*byte 0:res=0 ver_done =1 ip_offset=14
	*byte 1:tcp_h_offset=5 tcp_type=0
	*byte 2:ppid=15 class=0
	*byte 3:res=0 pkt_type=0
	*byte 4:res=0 redirect=0 res2=0 src_sub_inf_id=0
	*byte 5:src_sub_inf_id2=0
	*byte 6:port_map=0
	*byte 7:port_map2=1
	*/
#ifdef CONFIG_LITTLE_ENDIAN
	char example_data[] = {
		0x00, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e,
		0x00, 0x00, 0x00, 0x16, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x2e,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x80, 0x18, 0x80, 0x00,
		0x00, 0xf0, 0x28, 0x4e, 0x01, 0x00, 0x00, 0x00, 0xaa, 0x00,
		0x00, 0x00, 0x04, 0x03, 0xbb, 0x00,
		0x00, 0x00, 0x04, 0x02, 0x08, 0x00, 0x45, 0x00, 0x00, 0x2e,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x06,
		0xb9, 0xcb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x04, 0x00, 0xb2, 0x9a, 0x03, 0xde
	};
#else
	char example_data[] = {
		0x00, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e,
		0x00, 0x00, 0x00, 0x16, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x2e,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x80, 0x18, 0x80, 0x00,
		0x4e, 0x28, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xaa, 0x00,
		0x00, 0x00, 0x04, 0x03, 0xbb, 0x00,
		0x00, 0x00, 0x04, 0x02, 0x08, 0x00, 0x45, 0x00, 0x00, 0x2e,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x06,
		0xb9, 0xcb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x04, 0x00, 0xb2, 0x9a, 0x03, 0xde
	};
#endif
	struct sk_buff skb;

	skb.DW0 = 0x4000;
	skb.DW1 = 0x1000;
	skb.DW2 = 0xa0c02080;
	skb.DW3 = 0xb0000074;
	skb.data = example_data;
	skb.len = sizeof(example_data);
	dp_rx(&skb, 0);
}
#endif				/* DP_TEST_EXAMPLE */

static __init int dp_init_module(void)
{
	int res;
	struct proc_dir_entry *proc_node = dp_proc_install();

	/*mask to reset some field as SWAS required  all others try to keep */
	dma_rx_desc_mask1.all = 0xFFFFFFFF;
	dma_rx_desc_mask3.all = 0xFFFFFFFF;
	dma_rx_desc_mask3.field.own = 0;
	dma_rx_desc_mask3.field.c = 0;
	dma_rx_desc_mask3.field.sop = 0;
	dma_rx_desc_mask3.field.eop = 0;
	dma_rx_desc_mask3.field.dic = 0;
	dma_rx_desc_mask3.field.byte_offset = 0;
	dma_rx_desc_mask1.field.dec = 0;
	dma_rx_desc_mask1.field.enc = 0;
	dma_rx_desc_mask1.field.mpe2 = 0;
	dma_rx_desc_mask1.field.mpe1 = 0;
	/*mask to keep some value set by top application all others set to 0*/
	dma_tx_desc_mask0.all = 0;
	dma_tx_desc_mask1.all = 0;
	dma_tx_desc_mask0.field.flow_id = 0xFF;
	dma_tx_desc_mask0.field.dest_sub_if_id = 0x7FFF;
	dma_tx_desc_mask1.field.mpe1 = 0x1;
	dma_tx_desc_mask1.field.color = 0x3;
	dma_tx_desc_mask1.field.ep = 0xF;
#ifdef CONFIG_LTQ_DATAPATH_LOOPETH
	dp_loop_eth_dev_init(proc_node);
#endif
#ifdef CONFIG_LTQ_TMU
	tmu_proc_install(proc_node);
#endif
	/*dp_set_gsw_parser(3, 2, 2, 0, 0);*/
	dp_get_gsw_parser(NULL, NULL, NULL, NULL);
#ifdef CONFIG_LTQ_DATAPATH_MIB
	dp_mib_init();
	gsw_mib_reset(0, 0); /* GSW L */
	gsw_mib_reset(1, 0); /* GSW R */
	cbm_counter_mode_set(0, 1); /*enqueue to byte */
	cbm_counter_mode_set(1, 1); /*dequeue to byte */
#endif
	res = dp_api_init();
#ifdef CONFIG_LTQ_DATAPATH_FILTER
	mpe_fh_netfiler_install();
#endif
#ifdef CONFIG_LTQ_DATAPATH_CPUFREQ
	dp_coc_cpufreq_init();
#endif

#ifdef CONFIG_LTQ_DATAPATH_LOCAL_SESSION
	dp_local_session_fast_init(0);
#endif
	PRINTK("dp init done\n");
	return res;
}

static __exit void dp_cleanup_module(void)
{
	if (dp_init_ok) {
		DP_LIB_LOCK(&dp_lock);
		memset(dp_port_info, 0, sizeof(dp_port_info));
#ifdef CONFIG_LTQ_DATAPATH_MIB
		dp_mib_exit();
#endif
		dp_init_ok = 0;
		DP_LIB_UNLOCK(&dp_lock);
#ifdef CONFIG_LTQ_DATAPATH_LOOPETH
		dp_loop_eth_dev_exit();
#endif
#ifdef CONFIG_LTQ_DATAPATH_CPUFREQ
		dp_coc_cpufreq_exit();
#endif
	}
}

module_init(dp_init_module);
module_exit(dp_cleanup_module);

MODULE_LICENSE("GPL");
