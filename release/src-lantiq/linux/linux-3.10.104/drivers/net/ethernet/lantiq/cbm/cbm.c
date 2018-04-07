#include "cbm.h"
#include <linux/dma-mapping.h>
#include <lantiq_dmax.h>
#include <net/drv_tmu_ll.h>
#include <linux/skbuff.h>
#include <linux/if_ether.h>
#include <linux/klogging.h>

#include <linux/ltq_hwmcpy.h>

#define xrx500_cbm_r32(m_reg)		__raw_readl(m_reg)
#define xrx500_cbm_w32(m_reg, val)	__raw_writel(val, m_reg)
#define xrx500_cbm_w32_mask(m, clear, set, reg) \
		ltq_w32((ltq_r32(m + reg) & ~(clear)) | (set), \
		m + reg)

#define set_val(reg, val, mask, offset) \
do {\
	xrx500_cbm_w32(reg, xrx500_cbm_r32(reg) & ~(mask));\
	xrx500_cbm_w32(reg, xrx500_cbm_r32(reg)\
	| (((val) << (offset)) & (mask)));\
} while (0)

#define get_val(val, mask, offset) (((val) & (mask)) >> (offset))
/*#define DISABLE_OTHER_QUEUE 1*/
#undef ENABLE_LL_DEBUG
#define CBM_NUM_INTERRUPTS 5
#define CBM_NUM_BASEADDR_IN_DTS 12
#define CBM_NUM_ITEMS_DTS_CONFIG 6
#define CBM_PMAC_DYNAMIC 1000
#define CBM_PORT_INVALID 2000
#define CBM_PMAC_NOT_APPL 3000
#define CBM_PORT_NOT_APPL 255
#define CBM_MAX_PHY_PORT_PER_EP 4
#define CBM_DEQ_BUF_SIZE 64
#define CBM_FROM_VRX318_PORT 18
#define CBM_SOC_REV_1 1
#define CBM_SOC_REV_0 0
#define CBM_EQM_DELAY_ENQ 0x10
#define CBM_PDU_TYPE 26
#define CBM_BASE						(g_base_addr.cbm_addr_base)
#define FSQM0_BASE						(g_base_addr.fsqm0_addr_base)
#define FSQM1_BASE						(g_base_addr.fsqm1_addr_base)
#define FSQM_BASE(idx) \
	(idx == 0 ? g_base_addr.fsqm0_addr_base : g_base_addr.fsqm1_addr_base)
#define CBM_EQM_BASE					(g_base_addr.cbm_eqm_addr_base)
#define CBM_DQM_BASE					(g_base_addr.cbm_dqm_addr_base)
#define CBM_DESC_BASE					(g_base_addr.cbm_desc_addr_base)
#define CBM_LS_BASE						(g_base_addr.cbm_ls_addr_base)
#define CBM_QIDT_BASE					(g_base_addr.cbm_qidt_addr_base)
#define CBM_QEQCNT_BASE					(g_base_addr.cbm_qeqcnt_addr_base)
#define CBM_QDQCNT_BASE					(g_base_addr.cbm_qdqcnt_addr_base)
#define CBM_DMADESC_BASE				(g_base_addr.cbm_dmadesc_addr_base)


#define CHECK_WHILE_LOOP 1
extern void *ltq_cbm_std_buf_base;
extern void *ltq_cbm_jbo_buf_base;
static struct cbm_ctrl g_cbm_ctrl;

static struct cbm_buff_info g_cbm_buff = {
	.std_frm_size = CBM_STD_BUF_SIZE,
	.jbo_frm_size = CONFIG_CBM_JBO_PKT_SIZE,
	.std_fsqm_idx = 0,
	.jbo_fsqm_idx = 1
	};
static DEFINE_PER_CPU(struct cbm_jumbo_divide, g_cbm_jbo_divide);
#ifdef CONFIG_CBM_LS_ENABLE
static struct cbm_desc_list g_cbm_dq_dlist[204];
static void __iomem *dqmdesc[2];
static int g_budget;
#endif
static struct cbm_base_addr g_base_addr;
static struct cbm_desc_list g_cbm_dlist[4][CBM_DEQ_BUF_SIZE];
static struct cbm_dqm_port_info dqm_port_info[CBM_PORT_MAX] = { {0} };
static struct cbm_eqm_port_info eqm_port_info[CBM_EQM_PORT_MAX] = { {0} };
static void __iomem *bufreq[2];
static void __iomem *eqmdesc[4];
static struct rcnt_idx rcnt_data[2];
static volatile unsigned long g_cpu_port_alloc;
static uint32_t g_cbm_irq[8];
/*Maintain a shadow queue lookup table for fast processing of queue map get API*/

static struct cbm_qidt_shadow g_cbm_qidt_mirror[0x1000];
static struct cbm_q_info  cbm_qtable[MAX_TMU_QUEUES] = { {0} };
static	struct  cbm_qlink_get queue_flush_buff;
static	uint32_t  tmp_q_buff2[EGRESS_QUEUE_ID_MAX];
static	uint32_t  tmp_q_buff3[EGRESS_QUEUE_ID_MAX];
static struct cbm_cntr_mode cbm_cntr_func[] = {
	{
		.msel_mask = CBM_EQM_CTRL_MSEL_MASK,
		.msel_pos = CBM_EQM_CTRL_MSEL_POS,
		.qen_mask = CBM_EQM_CTRL_EQQCEN_MASK,
		.qen_pos = CBM_EQM_CTRL_EQQCEN_POS,
		.reset_counter = reset_enq_counter_all
	},
	{
		.msel_mask = CBM_DQM_CTRL_MSEL_MASK,
		.msel_pos = CBM_DQM_CTRL_MSEL_POS,
		.qen_mask = CBM_DQM_CTRL_DQQCEN_MASK,
		.qen_pos = CBM_DQM_CTRL_DQQCEN_POS,
		.reset_counter = reset_deq_counter_all
	}
};
spinlock_t cbm_queue_flush_lock;		/*!< spin lock */

static uint32_t g_qidt_help[0x4000];
static u32 cbm_p2p_setup_done;
static u32 cbm_p2p_turned_on;
void (*reset_counter)(void);
static int cbm_rev;
spinlock_t cbm_qidt_lock;	/*!< spin lock */
spinlock_t cbm_buff_lock;		/*!< spin lock */
spinlock_t cbm_buff_free_lock;		/*!< spin lock */
LIST_HEAD(pmac_mapping_list);
spinlock_t cbm_port_mapping;		/*!< spin lock for cbm port mapping list*/



#ifdef CONFIG_CBM_LS_ENABLE
static int do_dq_cbm_poll(struct napi_struct *napi, int budget);
static inline int cbm_dq_deq_list(void);
#endif
static void do_cbm_tasklet(unsigned long);
static void reserved_ports_highest(cbm_tmu_res_t *tmu_res, int flag_set);
static struct tasklet_struct cbm_tasklet[NR_CPUS];
#ifdef ENABLE_LL_DEBUG
static void do_cbm_debug_tasklet(unsigned long cpu);
static struct tasklet_struct cbm_debug_tasklet;
#endif
static void store_rcnt_baseaddr(void);
static void store_bufreq_baseaddr(void);

static int get_is_bit_set(uint32_t flags);

#define BADKEY -1
enum port_value {
	DQM_PORTS_VALUE = 1,
	EQM_PORTS_VALUE
};
#define SBID_START 16
struct t_symstruct {
char *key;
int val;
};

static struct t_symstruct device_table[] = {
	{ "ltq_dqm_ports", DQM_PORTS_VALUE },
	{ "ltq_eqm_ports", EQM_PORTS_VALUE },
};

#define NKEYS (sizeof(device_table)/sizeof(struct t_symstruct))

int keyfromstring(const char *key)
{
	int i;
	for (i = 0; i < NKEYS; i++) {
		struct t_symstruct *sym = device_table + i;
		if (strcmp(sym->key, key) == 0)
			return sym->val;
	}
	return BADKEY;
}
int find_eqm_port_type(int port)
{
	LOGF_KLOG_DEBUG("%s\r\n", __func__);
	if ((port >= EQM_CPU_START_ID) && (port <= EQM_CPU_END_ID))
		return EQM_CPU_TYPE;
	else if ((port >= EQM_DMA_START_ID) && (port <= EQM_DMA_END_ID))
		return EQM_DMA_TYPE;
	else if (port == EQM_TOE_TYPE)
		return EQM_TOE_TYPE;
	else if (port == EQM_VRX318_TYPE)
		return EQM_VRX318_TYPE;
	else if (port == EQM_DL_TYPE)
		return EQM_DL_TYPE;
	else
		return CBM_FAILURE;

}
int find_dqm_port_type(int port)
{
	if ((port >= DQM_CPU_START_ID) && (port <= DQM_CPU_END_ID))
		return DQM_CPU_TYPE;
	if ((port >= DQM_WAVE_START_ID) && (port <= DQM_WAVE_END_ID))
		return DQM_CPU_TYPE;
	else if ((port >= DQM_SCPU_START_ID) && (port <= DQM_SCPU_END_ID))
		return DQM_SCPU_TYPE;
	else if ((port >= DQM_DMA_START_ID) && (port <= DQM_DMA_END_ID))
		return DQM_DMA_TYPE;
	else if ((port >= DQM_LDMA_START_ID) && (port <= DQM_LDMA_END_ID))
		return DQM_LDMA_TYPE;
	else
		return CBM_FAILURE;
}

int find_dqm_port_flush(int port)
{
	if ((port >= DQM_CPU_START_ID) && (port <= (DQM_CPU_END_ID - 1)))
		return DQM_CPU_TYPE;
	if (port == DQM_CPU_END_ID)
		return DQM_WAVE_TYPE;
	if ((port >= DQM_WAVE_START_ID) && (port <= DQM_WAVE_END_ID))
		return DQM_WAVE_TYPE;
	else if ((port >= DQM_SCPU_START_ID) && (port <= DQM_SCPU_END_ID))
		return DQM_SCPU_TYPE;
	else if ((port >= DQM_DMA_START_ID) && (port <= DQM_DMA_END_ID))
		return DQM_DMA_TYPE;
	else if ((port >= DQM_LDMA_START_ID) && (port <= DQM_LDMA_END_ID))
		return DQM_LDMA_TYPE;
	else
		return CBM_FAILURE;
}
static struct cbm_pmac_port_map *cbm_add_to_list(
struct cbm_pmac_port_map *val)
{
	struct cbm_pmac_port_map *ptr = NULL;
	int i = 0;
	unsigned long flags;


	LOGF_KLOG_DEBUG("\n new linked list\n");
	ptr = (struct cbm_pmac_port_map *)kmalloc(sizeof(struct cbm_pmac_port_map), GFP_ATOMIC);
	if (NULL == ptr) {
		LOGF_KLOG_ERROR("\n Node creation failed\n");
		return NULL;
	}
	LOGF_KLOG_DEBUG("\n %s : 1\n", __func__);
	ptr->pmac = val->pmac;
	ptr->egp_port_map = val->egp_port_map;
	for (i = 0; i < val->qid_num; i++) {
		ptr->qids[i] = val->qids[i];
	}
	ptr->qid_num = val->qid_num;
	ptr->egp_type = val->egp_type;
	ptr->owner = 0;
	ptr->dev = 0;
	ptr->dev_port = 0;
	ptr->flags = P_ALLOCATED;
	LOGF_KLOG_DEBUG("\n %s : 2\n", __func__);
	spin_lock_irqsave(&cbm_port_mapping, flags);
	/* Init the list within the struct. */
	INIT_LIST_HEAD(&ptr->list);
	/* Add this struct to the tail of the list. */
	list_add_tail(&ptr->list, &pmac_mapping_list);
	spin_unlock_irqrestore(&cbm_port_mapping, flags);
	return ptr;
}

struct cbm_pmac_port_map *is_cbm_allocated(
int32_t cbm, uint32_t flags)
{
	struct cbm_pmac_port_map *ptr = NULL;
	unsigned long lock_flags;
	int num_deq_ports, i;
	uint32_t port_map, index;

	if ((flags != DP_F_MPE_ACCEL) && (flags != DP_F_DIRECTPATH_RX)  && (flags != DP_F_CHECKSUM))
		flags = DP_F_DONTCARE;
	spin_lock_irqsave(&cbm_port_mapping, lock_flags);
	if (flags == DP_F_DONTCARE) {
		list_for_each_entry(ptr, &pmac_mapping_list, list) {
			num_deq_ports = hweight_long(ptr->egp_port_map);
			port_map = ptr->egp_port_map;
			for (i = 0; i < num_deq_ports; i++) {
				index = get_is_bit_set(port_map);
				if ((index == cbm)
				&& (ptr->egp_type != DP_F_MPE_ACCEL)
				&& (ptr->egp_type != DP_F_DIRECTPATH_RX)
				) {
				spin_unlock_irqrestore(&cbm_port_mapping, lock_flags);
				return ptr;
				}
				port_map &= ~(1 << index);
			}
		}
	} else {
		list_for_each_entry(ptr, &pmac_mapping_list, list) {
			if (/*(ptr->pmac == pmac)
			&&*/ (ptr->egp_type == flags)) {
				spin_unlock_irqrestore(&cbm_port_mapping, lock_flags);
				return ptr;
			}
		}
	}
	spin_unlock_irqrestore(&cbm_port_mapping, lock_flags);
	return NULL;

}
struct cbm_pmac_port_map *is_dp_allocated(
int32_t pmac, uint32_t flags)
{
	struct cbm_pmac_port_map *ptr = NULL;
	unsigned long lock_flags;

	if ((!(flags & DP_F_MPE_ACCEL)) && (!(flags & DP_F_DIRECTPATH_RX)) && (!(flags & DP_F_CHECKSUM)))
		flags = DP_F_DONTCARE;
	LOGF_KLOG_DEBUG("%s: flags 0x%x\r\n", __func__, flags);
	spin_lock_irqsave(&cbm_port_mapping, lock_flags);
	if (flags & DP_F_DONTCARE) {
		list_for_each_entry(ptr, &pmac_mapping_list, list) {
			LOGF_KLOG_DEBUG("11:pmac %d type %d  input %d input %d\r\n", ptr->pmac, ptr->egp_type, pmac, flags);
			if ((ptr->pmac == pmac)
			&& (!(ptr->egp_type & DP_F_MPE_ACCEL))
			&& (!(ptr->egp_type & DP_F_DIRECTPATH_RX))
			#ifdef CONFIG_LTQ_DP_ACA_CSUM_WORKAROUND
			#else
			&& (!(ptr->egp_type & DP_F_CHECKSUM))
			#endif
			) {
				spin_unlock_irqrestore(&cbm_port_mapping, lock_flags);
				return ptr;
			}
		}
	} else {
		list_for_each_entry(ptr, &pmac_mapping_list, list) {
			LOGF_KLOG_DEBUG("22:pmac %d type %d \r\n", ptr->pmac, ptr->egp_type);
			if (ptr->egp_type == flags) {
				spin_unlock_irqrestore(&cbm_port_mapping, lock_flags);
				return ptr;
			}
		}
	}
	spin_unlock_irqrestore(&cbm_port_mapping, lock_flags);
	return NULL;

}
static int cbm_delete_from_list(
int32_t pmac, uint32_t flags)
{

	struct cbm_pmac_port_map *ptr = NULL;
	struct cbm_pmac_port_map *next = NULL;
	int found = 0;
	unsigned long lock_flags;

	if ((flags != DP_F_MPE_ACCEL) && (flags != DP_F_DIRECTPATH_RX) && (flags != DP_F_CHECKSUM))
		flags = DP_F_DONTCARE;
	LOGF_KLOG_DEBUG("%s: flags 0x%x\r\n", __func__, flags);

	spin_lock_irqsave(&cbm_port_mapping, lock_flags);

	list_for_each_entry_safe(ptr, next, &pmac_mapping_list, list) {
		if (ptr->pmac == pmac) {
			if (flags == DP_F_DONTCARE) {
				found = 1;
				break;
			} else if (ptr->egp_type & flags) {
				found = 1;
				break;
			}
		}
	}
	if (found) {
		list_del(&ptr->list);
		kfree(ptr);
		spin_unlock_irqrestore(&cbm_port_mapping, lock_flags);
		return 1;
	}
	spin_unlock_irqrestore(&cbm_port_mapping, lock_flags);
	return 0;
}


struct cbm_egp_map epg_lookup_table[] = {
	{0,	0,					0},
	{0,	CBM_PMAC_DYNAMIC, DP_F_DIRECT},
	{1,	0,					DP_F_MPE_ACCEL},
	{1,	CBM_PMAC_DYNAMIC, DP_F_DIRECTLINK},
	{4,	CBM_PMAC_DYNAMIC, DP_F_FAST_WLAN},
	{5,	CBM_PMAC_NOT_APPL, DP_F_LRO},
	{6,	1,			 DP_F_FAST_ETH_LAN},
	{7,	2,			 DP_F_FAST_ETH_LAN},
	{8,	3,			 DP_F_FAST_ETH_LAN},
	{9,	4,			 DP_F_FAST_ETH_LAN},
	{10,	5,			 DP_F_FAST_ETH_LAN},
	{11,	6,			 DP_F_FAST_ETH_LAN},
	{12,	1,			 DP_F_FAST_ETH_LAN},
	{13,	2,			 DP_F_FAST_ETH_LAN},
	{14,	3,			 DP_F_FAST_ETH_LAN},
	{15,	4,			 DP_F_FAST_ETH_LAN},
	{16,	5,			 DP_F_FAST_ETH_LAN},
	{17,	6,			 DP_F_FAST_ETH_LAN},
	{18,	CBM_PMAC_NOT_APPL, DP_F_FAST_DSL_DOWNSTREAM},
	{19,	15,			 DP_F_FAST_ETH_WAN},
	#ifdef CONFIG_LTQ_DP_ACA_CSUM_WORKAROUND
	{20,	CBM_PMAC_DYNAMIC, DP_F_CHECKSUM},
	#else
	{20,	CBM_PMAC_NOT_APPL, DP_F_CHECKSUM},
	#endif
	{21,	CBM_PMAC_NOT_APPL, DP_F_DIRECTPATH_RX},
	{23,	CBM_PMAC_DYNAMIC, DP_F_FAST_DSL},
	{CBM_PORT_NOT_APPL,	CBM_PMAC_DYNAMIC, DP_F_FAST_WLAN},
	{CBM_PORT_NOT_APPL,	CBM_PMAC_DYNAMIC, DP_F_FAST_WLAN},
	{CBM_PORT_NOT_APPL,	CBM_PMAC_DYNAMIC, DP_F_FAST_WLAN},
	{CBM_PORT_NOT_APPL, CBM_PMAC_DYNAMIC, DP_F_PORT_TUNNEL_DECAP},
};

static uint32_t get_matching_EP(
	uint32_t cbm_port,
	uint32_t flags,
	uint32_t *pmac
)
{
	int i;

	for (i = 0; i < sizeof(epg_lookup_table)/sizeof(epg_lookup_table[0]);
	i++) {
		LOGF_KLOG_DEBUG("%s: %d %d %d\r\n", __func__, epg_lookup_table[i].epg, epg_lookup_table[i].port_type, epg_lookup_table[i].pmac);
		LOGF_KLOG_DEBUG("%s: %d %d \r\n", __func__, cbm_port, flags);
		if ((epg_lookup_table[i].epg == cbm_port) && (epg_lookup_table[i].port_type == flags)) {
				/*flags = epg_lookup_table[i].port_type;*/
				*pmac = epg_lookup_table[i].pmac;
				return 1;
			}
	}
	return 0;
}
static uint32_t assign_port_from_DT(
	uint32_t flags,
	uint32_t cbm_port)
{
	int i, result = CBM_NOTFOUND;

	for (i = 0; i < sizeof(epg_lookup_table)/sizeof(epg_lookup_table[0]);
	i++) {
		if (epg_lookup_table[i].port_type == flags) {
			if ((flags == DP_F_FAST_WLAN) && (epg_lookup_table[i].epg != CBM_PORT_NOT_APPL))
				continue;
			epg_lookup_table[i].epg = cbm_port;
			result = CBM_SUCCESS;
			break;
		}
	}
	return result;
}
static uint32_t get_matching_flag(
	uint32_t *flags,
	uint32_t cbm_port)
{
	int i, result = CBM_NOTFOUND;
	for (i = 0; i < sizeof(epg_lookup_table)/sizeof(epg_lookup_table[0]);
	i++) {
		if (epg_lookup_table[i].epg == cbm_port) {
			*flags = epg_lookup_table[i].port_type;
			result = CBM_SUCCESS;
			break;
		}
	}
	return result;
}
static uint32_t get_matching_pmac(
	uint32_t *ep,
	uint32_t flags,
	uint32_t *cbm_port,
	int pmac,
	int pmac_present)
{
	int i, j = 0, result = CBM_NOTFOUND;
	for (i = 0; i < CBM_MAX_PHY_PORT_PER_EP;
	i++) {
		ep[i] = CBM_PORT_INVALID;
		cbm_port[i] = CBM_PORT_INVALID;
	}
	if (flags & DP_F_FAST_WLAN)
		flags = DP_F_FAST_WLAN;
	for (i = 0; i < sizeof(epg_lookup_table)/sizeof(epg_lookup_table[0]);
	i++) {
		if (epg_lookup_table[i].port_type == flags) {
			if (j < CBM_MAX_PHY_PORT_PER_EP) {
				if (pmac_present
				&& (epg_lookup_table[i].pmac == pmac)) {
					ep[j] = epg_lookup_table[i].pmac;
					cbm_port[j] = epg_lookup_table[i].epg;
					j++;
				}
				if (!pmac_present) {
					ep[j] = epg_lookup_table[i].pmac;
					cbm_port[j] = epg_lookup_table[i].epg;
					j++;
				}
				result = CBM_SUCCESS;
			} else {
				break;
			}
		}
	}
	return result;
}
static uint32_t get_matching_pmac_noflags(
	uint32_t *cbm_port,
	int pmac,
	uint32_t *flags,
	uint32_t *num_ports)
{
	int i, j = 0, result = CBM_NOTFOUND;
	for (i = 0; i < CBM_MAX_PHY_PORT_PER_EP;
	i++) {
		cbm_port[i] = CBM_PORT_INVALID;
	}
	for (i = 0; i < sizeof(epg_lookup_table)/sizeof(epg_lookup_table[0]);
	i++) {
			if (j < CBM_MAX_PHY_PORT_PER_EP) {
				if ((epg_lookup_table[i].pmac == pmac)
				&& (epg_lookup_table[i].port_type != DP_F_MPE_ACCEL)
				&& (epg_lookup_table[i].port_type != DP_F_DIRECTPATH_RX)) {
					cbm_port[j] = epg_lookup_table[i].epg;
					*flags = epg_lookup_table[i].port_type;
					j++;
					*num_ports = j;
					result = CBM_SUCCESS;
				}
			}
	}
	return result;
}

struct sk_buff *cbm_build_skb(void *data, unsigned int frag_size, gfp_t priority)
{
	struct skb_shared_info *shinfo;
	struct sk_buff *skb;
	unsigned int size;
	size = frag_size;
	/*printk("%s data 0x%x size %d\r\n",__func__, data, frag_size);*/
	skb = alloc_skb_head(GFP_ATOMIC);
	if (!skb) {
		LOGF_KLOG_ERROR("%s: SKB head alloc failed\r\n", __func__);
		return NULL;
	}

	size -= SKB_DATA_ALIGN(sizeof(struct skb_shared_info));

	memset(skb, 0, offsetof(struct sk_buff, tail));
	skb->truesize = SKB_TRUESIZE(size);
	skb->head_frag = 0;
	atomic_set(&skb->users, 1);
	skb->head = data;
	skb->data = data;
	skb_reset_tail_pointer(skb);
	skb->end = skb->tail + size;
#ifdef NET_SKBUFF_DATA_USES_OFFSET
	skb->mac_header = ~0U;
	skb->transport_header = ~0U;
#endif

	/* make sure we initialize shinfo sequentially */
	shinfo = skb_shinfo(skb);
	memset(shinfo, 0, offsetof(struct skb_shared_info, dataref));
	atomic_set(&shinfo->dataref, 1);
	kmemcheck_annotate_variable(shinfo->destructor_arg);

	return skb;
}
EXPORT_SYMBOL(cbm_build_skb);
/*
 *	FSQM initialization
 */

void init_fsqm(void)
{

	buf_addr_adjust((uint32_t)ltq_cbm_std_buf_base,
	CONFIG_CBM_STD_BUF_POOL_SIZE,
	&g_cbm_buff.std_buf_addr,
	&g_cbm_buff.std_buf_size,
	g_cbm_buff.std_frm_size);
	buf_addr_adjust((uint32_t)ltq_cbm_jbo_buf_base,
	CONFIG_CBM_JBO_BUF_SIZE,
	&g_cbm_buff.jbo_buf_addr,
	&g_cbm_buff.jbo_buf_size,
	g_cbm_buff.jbo_frm_size);

	g_cbm_buff.std_frm_num = g_cbm_buff.std_buf_size / g_cbm_buff.std_frm_size;
	g_cbm_buff.jbo_frm_num = g_cbm_buff.jbo_buf_size / g_cbm_buff.jbo_frm_size;

	LOGF_KLOG_DEBUG("Jumbo:  buf base: 0x%x, buf size: 0x%x,\
	total frame num: %d, frame size: %d\n",
	g_cbm_buff.jbo_buf_addr, g_cbm_buff.jbo_buf_size,
	g_cbm_buff.jbo_frm_num, g_cbm_buff.jbo_frm_size);

	init_fsqm_buf_standard(g_cbm_buff.std_buf_addr, g_cbm_buff.std_buf_size);
	init_fsqm_buf_jumbo(g_cbm_buff.jbo_buf_addr, g_cbm_buff.jbo_buf_size);
	LOGF_KLOG_INFO("fsqm init successfully\n");
	return;
}

void init_fsqm_by_idx(int idx)
{
	xrx500_cbm_w32(FSQM_BASE(idx) + IO_BUF_RD, 0);
	xrx500_cbm_w32(FSQM_BASE(idx) + IO_BUF_WR, 0);
	xrx500_cbm_w32(FSQM_BASE(idx) + FSQM_CTRL, 0x1);
	return;
}

/*
 *	 FSQM buffer initialization
 */

void buf_addr_adjust(unsigned int buf_base_addr,
unsigned int buf_size,
unsigned int *adjusted_buf_base,
unsigned int *adjusted_buf_size,
unsigned int align)
{
	unsigned int base;
	unsigned int size;

	base = ALIGN(buf_base_addr, align);
	size = buf_base_addr + buf_size - base;

	*adjusted_buf_base = base;
	*adjusted_buf_size = size;
}

void init_fsqm_buf_standard(unsigned int std_base_addr,
unsigned int size)
{
	int i;
	int minlsa, maxlsa;

	for (i = 1; i <= g_cbm_buff.std_frm_num; i++) {
		/*__raw_writel(i,FSQM_LLT_RAM(FSQM_BASE(g_std_fsqm_idx),i)); */
		if (i == g_cbm_buff.std_frm_num - 1) {
			xrx500_cbm_w32((FSQM_LLT_RAM(FSQM_BASE(g_cbm_buff.std_fsqm_idx),
			i-1)),
			0x7FFF);
		} else {
			xrx500_cbm_w32((FSQM_LLT_RAM(FSQM_BASE(g_cbm_buff.std_fsqm_idx),
			i-1)),
			i % g_cbm_buff.std_frm_num);
		}
	#ifdef ENABLE_LL_DEBUG
	xrx500_cbm_w32((FSQM_RCNT(FSQM_BASE(g_cbm_buff.std_fsqm_idx), i-1)), 0);
	#else
	xrx500_cbm_w32((FSQM_RCNT(FSQM_BASE(g_cbm_buff.std_fsqm_idx), i-1)), 1);
	#endif
	}
	minlsa = 0;/* minlas is always zero, it's offset counted into the BA */
	maxlsa = (size / g_cbm_buff.std_frm_size) - 2; /* The last item is invalid inside the FSQM */
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.std_fsqm_idx) + LSARNG),
	((maxlsa << LSARNG_MAXLSA_POS) & LSARNG_MAXLSA_MASK)
	| ((minlsa << LSARNG_MINLSA_POS) & LSARNG_MINLSA_MASK));
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.std_fsqm_idx) + OFSQ),
	((maxlsa << OFSQ_TAIL_POS) & OFSQ_TAIL_MASK)
	|((minlsa << OFSQ_HEAD_POS) & OFSQ_HEAD_MASK));
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.std_fsqm_idx) + OFSC),
	((maxlsa - minlsa + 1) << OFSC_FSC_POS) & OFSC_FSC_MASK);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.std_fsqm_idx) + FSQM_IRNEN), 0x111101F);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.std_fsqm_idx) + FSQT0), g_cbm_buff.std_frm_num/6 * 5);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.std_fsqm_idx) + FSQT1), g_cbm_buff.std_frm_num/6 * 4);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.std_fsqm_idx) + FSQT2), g_cbm_buff.std_frm_num/6 * 3);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.std_fsqm_idx) + FSQT3), g_cbm_buff.std_frm_num/6 * 2);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.std_fsqm_idx) + FSQT4), g_cbm_buff.std_frm_num/6);

	init_fsqm_by_idx(g_cbm_buff.std_fsqm_idx);

	LOGF_KLOG_INFO("init fsqm std successfully\n");

	return;
}


void init_fsqm_buf_jumbo(unsigned int jbo_base_addr, unsigned int size)
{
	int i;
	int minlsa, maxlsa;

	for (i = 1; i <= g_cbm_buff.jbo_frm_num; i++) {
	/*__raw_writel(i, FSQM_LLT_RAM(FSQM_BASE(g_jbo_fsqm_idx),i));*/
	if (i == g_cbm_buff.jbo_frm_num - 1) {
		xrx500_cbm_w32((FSQM_LLT_RAM(FSQM_BASE(g_cbm_buff.jbo_fsqm_idx), i-1)),
		0x7FFF);
	} else {
		xrx500_cbm_w32((FSQM_LLT_RAM(FSQM_BASE(g_cbm_buff.jbo_fsqm_idx), i-1)),
		i % g_cbm_buff.jbo_frm_num);
	}
	xrx500_cbm_w32((FSQM_RCNT(FSQM_BASE(g_cbm_buff.jbo_fsqm_idx), i-1)),
	1);
	}

	/* minlas is always zero, it's counted into the BA */
	minlsa = 0;
	maxlsa = (size / g_cbm_buff.jbo_frm_size) - 2;  /* The last item is invalid in the FSQM */
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.jbo_fsqm_idx) + LSARNG),
	((maxlsa << LSARNG_MAXLSA_POS) & LSARNG_MAXLSA_MASK)
	| ((minlsa << LSARNG_MINLSA_POS) & LSARNG_MINLSA_MASK));
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.jbo_fsqm_idx) + OFSQ),
	((maxlsa << OFSQ_TAIL_POS) & OFSQ_TAIL_MASK)
	|((minlsa << OFSQ_HEAD_POS) & OFSQ_HEAD_MASK));
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.jbo_fsqm_idx) + OFSC),
	((maxlsa - minlsa + 1) << OFSC_FSC_POS & OFSC_FSC_MASK));

	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.jbo_fsqm_idx) + FSQM_IRNEN), 0x111101F);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.jbo_fsqm_idx) + FSQT0), g_cbm_buff.jbo_frm_num/6 * 5);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.jbo_fsqm_idx) + FSQT1), g_cbm_buff.jbo_frm_num/6 * 4);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.jbo_fsqm_idx) + FSQT2), g_cbm_buff.jbo_frm_num/6 * 3);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.jbo_fsqm_idx) + FSQT3), g_cbm_buff.jbo_frm_num/6 * 2);
	xrx500_cbm_w32((FSQM_BASE(g_cbm_buff.jbo_fsqm_idx) + FSQT4), g_cbm_buff.jbo_frm_num/6);
	init_fsqm_by_idx(g_cbm_buff.jbo_fsqm_idx);
	LOGF_KLOG_INFO("init fsqm jbo successfully\n");
	return;
}

/*
 * CBM interrupt mapping initialization
*/
void cbm_intr_mapping_init(void)
{
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(4, cbm_irnen)), 0x1 << 8);
	#ifdef ENABLE_LL_DEBUG
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(5, cbm_irnen)), 0x1 << 16);
	#else
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(5, cbm_irnen)), 0x2 << 8);
	#endif

	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(6, cbm_irnen)), 0x4 << 8);
#ifdef CONFIG_CBM_LS_ENABLE
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(6, egp_irnen)), 0x4);
#endif
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(7, cbm_irnen)), 0x8 << 8);
	/* Disable the EGP and IGP interrupts at top level */
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(0, egp_irnen)), 0x0);
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(4, egp_irnen)), 0x0);
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(5, egp_irnen)), 0x0);
#ifndef CONFIG_CBM_LS_ENABLE
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(6, egp_irnen)), 0x0);
#endif
	return;
}

void cbm_intr_mapping_uninit(void)
{
	int i;

	for (i = 0; i < CBM_MAX_INT_LINES/2; i++) {
		/*enq intr attach to first 4 lines */
		xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(i, igp_irnen)),
		0 << i);
		/*LS intr attach to last 4 lines */
		xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(i + (CBM_MAX_INT_LINES/2),
		cbm_irnen)),
		0 << (i+8));
	}
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(0, cbm_irnen)), 0x0);
	return;
}


/*
 * CBM basic initialization
*/
void init_cbm_basic(void)
{
	int jsel = 0;

	xrx500_cbm_w32((CBM_BASE + CBM_SBA_0), __pa(g_cbm_buff.std_buf_addr));
	xrx500_cbm_w32((CBM_BASE + CBM_JBA_0), __pa(g_cbm_buff.jbo_buf_addr));
	xrx500_cbm_w32((CBM_BASE + CBM_SBA_1), CBM_IOCU_ADDR(g_cbm_buff.std_buf_addr));
	xrx500_cbm_w32((CBM_BASE + CBM_JBA_1), CBM_IOCU_ADDR(g_cbm_buff.jbo_buf_addr));
	jsel = (g_cbm_buff.jbo_frm_size == 0x2000) ? 0 : 1;
	set_val((CBM_BASE + CBM_CTRL), jsel, 0x20000, 17);
	#ifdef ENABLE_LL_DEBUG
	set_val((CBM_BASE + CBM_CTRL), 1, 0x01000000, 24);
	#endif
	cbm_dw_memset((uint32_t *)CBM_QEQCNT_BASE,
	0, CBM_QEQCNTR_SIZE / sizeof(uint32_t));
	cbm_dw_memset((uint32_t *)CBM_QDQCNT_BASE,
	0, CBM_QDQCNTR_SIZE / sizeof(uint32_t));

	/* Enable all the interrupts */
	cbm_intr_mapping_init();
	cbm_interrupt_init();
	LOGF_KLOG_INFO("init basic CBM successfully\n");

	return;
}

/*
 * CBM Enqueue port Initialization
 */
void init_cbm_eqm_scpu_port(void)
{
	xrx500_cbm_w32((CBM_EQM_BASE + CFG_CPU_IGP_4), 0x303);
	xrx500_cbm_w32((CBM_EQM_BASE + POCC_CPU_IGP_4), 0);
}

void init_cbm_eqm_cpu_port(int idx)
{
	/*struct cbm_eqm_cpu_igp_reg cbm_cpu_igp;*/
	xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_CPU_PORT(idx, cfg)), 0xFC7);
	xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_CPU_PORT(idx, pocc)), 0);
	xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_CPU_PORT(idx, irnen)), 0x3F);
}

/*
 *	form 64bit register from 2 32bit DWORDs, 64-bit register is little endian mode
 */
uint64_t cbm_form_64bit_reg(uint32_t dw0, uint32_t dw1)
{
	uint64_t reg64;
	reg64 = (((uint64_t)dw1) << 32) | (uint64_t)dw0;
	return reg64;
}

int setup_eqm_dma_desc(int pid, uint32_t desc_num, uint32_t flag, uint32_t data_offset)
{
	struct dma_desc desc;
	uint32_t buf;
	u32 reg1;
	u32 reg2;
	int i;
	cbm_assert(pid >= 5, "cbm dma eqm port id is less than 5,pid:%d ", pid);

	/*LOGF_KLOG_INFO("%s:pid:%d,desc_num:%d, flag: %d, offset:%d\n",
	__func__, pid, desc_num, flag, data_offset);*/

	for (i = 0; i < desc_num; i++) {
		buf = (uint32_t)cbm_buffer_alloc(smp_processor_id(), flag);
		if (!buf) {
			cbm_err("Cannot alloc buffer from CBM!!!\n");
			return CBM_FAILURE;
		}
		buf = __pa(buf);
		cbm_dw_memset((uint32_t *)&desc,
		0,
		sizeof(struct dma_desc)/sizeof(uint32_t));
		reg1 = 0;
		reg2 = 0;
		if (flag == CBM_PORT_F_STANDARD_BUF) {
			reg2 = g_cbm_buff.std_frm_size;
		} else {
			reg2 = g_cbm_buff.jbo_frm_size;
	}
	reg1 = (buf + data_offset) & ~0x7;
	reg2 |= ((buf + data_offset) & 0x7) << 23;
	reg2 |= (CBM_DMA_DESC_OWN << 31);
	/*adjust the flag*/
	#ifdef CONFIG_CPU_LITTLE_ENDIAN
	xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag >> 1) + 0x0, 0);
	xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag >> 1) + 0x4, 0);
	xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag >> 1) + 0x8, reg1);
	asm("sync");
	xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag >> 1) + 0xc, reg2);
	asm("sync");
	#else
	xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag >> 1) + 0x4, 0);
	xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag >> 1) + 0x0, 0);
	xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag >> 1) + 0xc, reg1);
	asm("sync");
	xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag >> 1) + 0x8, reg2);
	asm("sync");
	#endif
	#if 0
		LOGF_KLOG_DEBUG("reg 0x%x desc0: 0x%x \r\n",  (unsigned int) ((CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag) + 4)), (unsigned int) (xrx500_cbm_r32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag) + 4)));
		LOGF_KLOG_DEBUG("reg 0x%x desc1:\r\n",  (CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag)), (unsigned int) (xrx500_cbm_r32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag))));
		LOGF_KLOG_DEBUG("reg 0x%x desc2:\r\n", (CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag) + 0xc), xrx500_cbm_r32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag) + 0xc));
		LOGF_KLOG_DEBUG("reg 0x%x desc3:\r\n", (CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag) + 0x8), xrx500_cbm_r32(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(pid, i%2, flag) + 0x8));
	#endif
	}

	return CBM_SUCCESS;
}

void init_cbm_eqm_dma_port(int idx, uint32_t flags)
{
	/*struct cbm_eqm_dma_igp_reg cbm_dma_igp;*/
#if 0
	xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_DMA_PORT(idx, cfg)), 0x301);
#else
	/*enable backpressure*/
	xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_DMA_PORT(idx, cfg)), 0x701);
#endif
	xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_DMA_PORT(idx, pocc)), 0);
	setup_eqm_dma_desc(idx + 5, 2, flags, CBM_DMA_DATA_OFFSET + NET_IP_ALIGN + NET_SKB_PAD);
}

void init_cbm_eqm_dma_toe_port(int idx, uint32_t flags)
{
	/*struct cbm_eqm_dma_igp_reg cbm_dma_igp;*/
	/*enable backpressure*/
	xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_DMA_PORT(idx, cfg)), 0x701);
	xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_DMA_PORT(idx, pocc)), 0);
	setup_eqm_dma_desc(idx + 5, 2, flags, CBM_DMA_DATA_OFFSET + NET_IP_ALIGN + NET_SKB_PAD);

}

void init_cbm_eqm_ldma_port(int flags)
{
	int i;
	u32 buf;

	for (i = 0; i < 32; i++) {
		buf = (u32)cbm_buffer_alloc(smp_processor_id(), flags);
		if (!buf) {
			cbm_err("Cannot alloc buffer from CBM!!!\n");
			return;
		}
		buf = __pa(buf);
		xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_LDMA_DESC(i) + 0x4), buf);
		xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_LDMA_DESC(i)), 0x80000800);
	}
	xrx500_cbm_w32((CBM_EQM_BASE + CFG_DMA_IGP_15), 0x1B01);
	xrx500_cbm_w32((CBM_EQM_BASE + POCC_DMA_IGP_15), 0);
	xrx500_cbm_w32((CBM_EQM_BASE + WM_DMA_IGP_15), (DEFAULT_DMA_HWM << 16) | (DEFAULT_DMA_HWM/3*2));
}

/*
 *	CBM Dequeue Port Initialization
 */
void setup_dqm_dma_desc(uint32_t pid, uint32_t desc_num)
{
	int i;
	cbm_assert(pid >= 5, "cbm dma dqm port id less than 5, pid:%d", pid);

	for (i = 0; i < desc_num; i++) {
		xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_DQM_DMA_DESC(pid, i%2) + 0x0, 0);
		xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_DQM_DMA_DESC(pid, i%2) + 0x4, 0);
		xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_DQM_DMA_DESC(pid, i%2) + 0x8, 0);
		xrx500_cbm_w32(CBM_DMADESC_BASE + CBM_DQM_DMA_DESC(pid, i%2) + 0xc, 0);
	}
	return;

}
void init_cbm_dqm_cpu_port(int idx)
{
	/*struct cbm_dqm_cpu_egp_reg  cbm_dqm_cpu;*/
	xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(idx, cfg)), 0x10F | ((idx << CFG_CPU_EGP_0_EPMAP_POS) & CFG_CPU_EGP_0_EPMAP_MASK));
#ifdef CONFIG_CBM_LS_ENABLE
	xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(2, irnen)), 0x2);
#endif
}

void init_cbm_dqm_wlan_cpu_port(int idx)
{
	/*struct cbm_dqm_cpu_egp_reg  cbm_dqm_cpu;*/
	xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(idx, cfg)), 0x107 | ((idx << CFG_CPU_EGP_0_EPMAP_POS) & CFG_CPU_EGP_0_EPMAP_MASK));
	#if 0
	xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(idx, irnen)), 0x2);
	#endif
}

void init_cbm_dqm_scpu_port(int idx)
{
	xrx500_cbm_w32((CBM_DQM_BASE + CFG_CPU_EGP_5), 0x101 | ((idx << CFG_CPU_EGP_5_EPMAP_POS) & CFG_CPU_EGP_5_EPMAP_MASK));
}

void init_cbm_dqm_dma_port(int dqp_idx, int dmap_idx)
{
	setup_dqm_dma_desc(dqp_idx, 2);
	xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_DMA_PORT(dmap_idx, cfg)), 0x101 | ((dqp_idx << CFG_DMA_EGP_6_EPMAP_POS) & CFG_DMA_EGP_6_EPMAP_MASK));
}

void init_cbm_dqm_ldma_port(int dqp_idx)
{
	xrx500_cbm_w32((CBM_DQM_BASE + CFG_DMA_EGP_23), 0x1901 | ((dqp_idx << CFG_DMA_EGP_23_EPMAP_POS) & CFG_DMA_EGP_23_EPMAP_MASK));
}


/*
 *	Load Spreader Initialization
 */

void init_cbm_ls_port(int idx)
{
	int lsport = CBM_LS_PORT(idx, ctrl);
#ifdef CONFIG_CBM_LS_ENABLE
	if ((1 << idx) & g_cpu_port_alloc) {
	/*Enable spreading only for port 0*/
#else
	if (!idx) {
#endif
		xrx500_cbm_w32((CBM_LS_BASE + lsport), 0xF
			| ((DEFAULT_LS_QTHH << LS_CTRL_PORT0_QUEUE_THRSHLD_POS) & (LS_CTRL_PORT0_QUEUE_THRSHLD_MASK))
			| ((DEFAULT_LS_OVFL_CNT << LS_CTRL_PORT0_CNT_THRSHLD_POS) & LS_CTRL_PORT0_CNT_THRSHLD_MASK));
	} else {
		xrx500_cbm_w32((CBM_LS_BASE + lsport), 0xD
			| ((DEFAULT_LS_QTHH << LS_CTRL_PORT0_QUEUE_THRSHLD_POS) & LS_CTRL_PORT0_QUEUE_THRSHLD_MASK)
			| ((DEFAULT_LS_OVFL_CNT << LS_CTRL_PORT0_CNT_THRSHLD_POS) & LS_CTRL_PORT0_CNT_THRSHLD_MASK));
	}
}

void cbm_add_ls_port(int idx, int flag)
{
	int lsport = CBM_LS_PORT(idx, ctrl);
	if (flag) {
		xrx500_cbm_w32((CBM_LS_BASE + lsport), 0xF
			| ((DEFAULT_LS_QTHH << LS_CTRL_PORT0_QUEUE_THRSHLD_POS) & (LS_CTRL_PORT0_QUEUE_THRSHLD_MASK))
			| ((DEFAULT_LS_OVFL_CNT << LS_CTRL_PORT0_CNT_THRSHLD_POS) & LS_CTRL_PORT0_CNT_THRSHLD_MASK));
	} else {
		xrx500_cbm_w32((CBM_LS_BASE + lsport), 0xD
			| ((DEFAULT_LS_QTHH << LS_CTRL_PORT0_QUEUE_THRSHLD_POS) & LS_CTRL_PORT0_QUEUE_THRSHLD_MASK)
			| ((DEFAULT_LS_OVFL_CNT << LS_CTRL_PORT0_CNT_THRSHLD_POS) & LS_CTRL_PORT0_CNT_THRSHLD_MASK));
	}
}
EXPORT_SYMBOL(cbm_add_ls_port);


int32_t cbm_load_spreader_cfg_get (
	cbm_load_spreader_cfg_t *cfg,
	uint32_t flags)
{
	uint32_t g_reg;
	g_reg = xrx500_cbm_r32(CBM_LS_BASE + LS_GLBL_CTRL);
	cfg->enable = g_reg >> LS_GLBL_CTRL_EN_POS;
	cfg->clk_div = g_reg >> LS_GLBL_CTRL_CLK_DIV_POS;

	g_reg = xrx500_cbm_r32(CBM_LS_BASE + LS_SPR_CTRL);
	cfg->flowid_mode = g_reg >> LS_SPR_CTRL_SPR_SEL_POS;
	cfg->wp0 = cbm_ls_port_weight_get(0);
	cfg->wp1 = cbm_ls_port_weight_get(1);
	cfg->wp2 = cbm_ls_port_weight_get(2);
	cfg->wp3 = cbm_ls_port_weight_get(3);
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_load_spreader_cfg_get);

int32_t cbm_load_spreader_cfg_set(
	cbm_load_spreader_cfg_t *cfg,
	uint32_t flags)
{
	xrx500_cbm_w32((CBM_LS_BASE + LS_GLBL_CTRL), (cfg->enable << LS_GLBL_CTRL_EN_POS));
	xrx500_cbm_w32((CBM_LS_BASE + LS_GLBL_CTRL), (cfg->clk_div << LS_GLBL_CTRL_CLK_DIV_POS) | xrx500_cbm_r32(CBM_LS_BASE + LS_GLBL_CTRL));
	cbm_ls_spread_alg_set(cfg->flowid_mode);
	cbm_ls_port_weight_set(0, cfg->wp0);
	cbm_ls_port_weight_set(1, cfg->wp1);
	cbm_ls_port_weight_set(2, cfg->wp2);
	cbm_ls_port_weight_set(3, cfg->wp3);
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_load_spreader_cfg_set);

int32_t cbm_port_load_spreader_cfg_get (
	uint32_t port_id,
	cbm_port_load_spreader_cfg_t *cfg,
	uint32_t flags)
{
	int lsport = CBM_LS_PORT(port_id, ctrl);
	uint32_t r_cfg = xrx500_cbm_r32(CBM_LS_BASE + lsport);
	cfg->enable = r_cfg >> LS_CTRL_PORT0_PORT_EN_POS;
	cfg->spreading_enable = r_cfg >> LS_CTRL_PORT0_SPR_EN_POS;
	cfg->irq_en = r_cfg >> LS_CTRL_PORT0_INT_EN_POS;
	cfg->counter_en = r_cfg >> LS_CTRL_PORT0_CNT_EN_POS;
	cfg->queue_thresh = r_cfg >> LS_CTRL_PORT0_QUEUE_THRSHLD_POS;
	cfg->counter_thresh = r_cfg >> LS_CTRL_PORT0_CNT_THRSHLD_POS;
	return CBM_SUCCESS;

}
EXPORT_SYMBOL(cbm_port_load_spreader_cfg_get);

int32_t cbm_port_load_spreader_cfg_set (
	uint32_t port_id,
	cbm_port_load_spreader_cfg_t *cfg,
	uint32_t flags)
{
	uint32_t reg;
	int lsport = CBM_LS_PORT(port_id, ctrl);
	reg = (cfg->enable << LS_CTRL_PORT0_PORT_EN_POS)
	| (cfg->spreading_enable << LS_CTRL_PORT0_SPR_EN_POS)
	| (cfg->irq_en << LS_CTRL_PORT0_INT_EN_POS)
	| (cfg->counter_en << LS_CTRL_PORT0_CNT_EN_POS)
	| (cfg->queue_thresh << LS_CTRL_PORT0_QUEUE_THRSHLD_POS)
	| (cfg->counter_thresh << LS_CTRL_PORT0_CNT_THRSHLD_POS);
	xrx500_cbm_w32((CBM_LS_BASE + lsport), reg);
	return CBM_SUCCESS;

}
EXPORT_SYMBOL(cbm_port_load_spreader_cfg_set);

void cbm_ls_spread_alg_set(uint32_t alg)
{
	cbm_assert(alg < SPREAD_MAX, "illegal cbm load spread alg");
	set_val((CBM_LS_BASE + LS_SPR_CTRL), alg, LS_SPR_CTRL_SPR_SEL_MASK, LS_SPR_CTRL_SPR_SEL_POS);
}

void cbm_ls_port_weight_set(uint32_t port_id, uint32_t weight)
{
	uint32_t pos, mask;

	cbm_assert(port_id < LS_PORT_NUM, "illegal cbm ls port id");

	pos  = LS_SPR_CTRL_WP0_POS + 2 * port_id;
	mask = (LS_SPR_CTRL_WP0_MASK >> LS_SPR_CTRL_WP0_POS) << pos;
	set_val((CBM_LS_BASE+LS_SPR_CTRL), weight, mask, pos);
}

void cbm_ls_flowId_map_set(uint32_t col, uint32_t val)
{
	xrx500_cbm_w32((CBM_LS_BASE+LS_FLOWID_MAP_COL0+(col*4)), val);
	return;
}

uint32_t cbm_ls_flowId_map_get(uint32_t col)
{
	return xrx500_cbm_r32(CBM_LS_BASE+LS_FLOWID_MAP_COL0+(col*4));
}

uint32_t cbm_ls_port_weight_get(uint32_t port_id)
{
	uint32_t pos, mask;

	cbm_assert(port_id < LS_PORT_NUM, "illegal cbm ls port id");

	pos  = LS_SPR_CTRL_WP0_POS + 2 * port_id;
	mask = (LS_SPR_CTRL_WP0_MASK >> LS_SPR_CTRL_WP0_POS) << pos;
	return get_val(xrx500_cbm_r32(CBM_LS_BASE + LS_SPR_CTRL), mask, pos);
}



void init_cbm_ls(void)
{
	int i;

	for (i = 0; i < LS_PORT_NUM; i++) {
		init_cbm_ls_port(i);
	}

	cbm_ls_spread_alg_set(SPREAD_WRR);
	for (i = 0; i < LS_PORT_NUM; i++) {
		cbm_ls_port_weight_set(i, DEFAULT_LS_PORT_WEIGHT);
	}

	xrx500_cbm_w32((CBM_LS_BASE + IRNEN_LS), 0xFF0000);
	xrx500_cbm_w32((CBM_LS_BASE + LS_GLBL_CTRL), (0x01 << LS_GLBL_CTRL_EN_POS));

	LOGF_KLOG_INFO("Load spreader init successfully\n");

	return;
}

void cbm_qid_reg_set(struct cbm_qidt_elm *qidt_elm, uint8_t qid_val)
{
	uint32_t qidt;
	uint32_t qidt_idx;
	uint32_t qidt_offset;
	uint32_t offset_factor, value_mask;
	uint8_t prev_qid;
	uint32_t value;

	qidt = (((qidt_elm->flowidh  << 13) & 0x2000)
		| ((qidt_elm->flowidl  << 12) & 0x1000)
		| ((qidt_elm->dec	   << 11) & 0x800)
		| ((qidt_elm->enc	   << 10) & 0x400)
		| ((qidt_elm->mpe2	   << 9) & 0x200)
		| ((qidt_elm->mpe1	   << 8) & 0x100)
		| ((qidt_elm->ep	   << 4) & 0xF0)
		| ((qidt_elm->clsid    << 0) & 0x0F));
	qidt_idx	= qidt >> 2;
	qidt_offset = qidt % 4;

	offset_factor = qidt_offset << 3;
	value_mask = 255 << offset_factor;

	prev_qid = (g_cbm_qidt_mirror[qidt_idx].qidt_shadow & value_mask) >> offset_factor;

	if (qid_val == 255) {
		g_cbm_qidt_mirror[qidt_idx].qidt_drop_flag = (g_cbm_qidt_mirror[qidt_idx].qidt_drop_flag & ~value_mask) | (1 << offset_factor);
		/*g_cbm_qidt_mirror[qidt_idx].qidt_shadow = (g_cbm_qidt_mirror[qidt_idx].qidt_shadow & ~value_mask) | ((qid_val & 0xFF) << ((qidt_offset) * 8));*/
		if (cbm_qtable[prev_qid].refcnt && (prev_qid != 255))
			cbm_qtable[prev_qid].refcnt--;
	} else {
		if (cbm_qtable[prev_qid].refcnt && (prev_qid != 255) && (!((g_cbm_qidt_mirror[qidt_idx].qidt_drop_flag & value_mask) >> offset_factor)))
			cbm_qtable[prev_qid].refcnt--;
		g_cbm_qidt_mirror[qidt_idx].qidt_shadow = (g_cbm_qidt_mirror[qidt_idx].qidt_shadow & ~value_mask) | ((qid_val & 0xFF) << ((qidt_offset) * 8));
		g_cbm_qidt_mirror[qidt_idx].qidt_drop_flag = (g_cbm_qidt_mirror[qidt_idx].qidt_drop_flag & ~value_mask);
		cbm_qtable[qid_val].refcnt++;
	}
	value = (xrx500_cbm_r32(CBM_QIDT_BASE + qidt_idx * 4) & ~value_mask) | ((qid_val & 0xFF) << offset_factor);
	xrx500_cbm_w32((CBM_QIDT_BASE + qidt_idx * 4), value);
}

void cbm_qidt_set(const struct cbm_qidt_elm *qid_set, const struct cbm_qidt_mask *qid_mask, uint8_t qid_val)
{
	int cls_idx, ep_idx, fl_idx, fh_idx, dec_idx, enc_idx, mpe1_idx, mpe2_idx;
	struct cbm_qidt_elm qidt_elm;
	/* Calc class id */
	for (cls_idx = 0; cls_idx < ((qid_mask->classid_mask == 1) ? 16 : 1); cls_idx++) {
		if (qid_mask->classid_mask) {
			qidt_elm.clsid = cls_idx;
		} else {
			qidt_elm.clsid = qid_set->clsid;
		}

		/* Calc EP */
		for (ep_idx = 0; ep_idx < ((qid_mask->ep_mask == 1) ? 16 : 1); ep_idx++) {
			if (qid_mask->ep_mask) {
				qidt_elm.ep = ep_idx;
			} else {
				qidt_elm.ep = qid_set->ep;
			}

			/* Calc MPE1 */
			for (mpe1_idx = 0; mpe1_idx < ((qid_mask->mpe1_mask == 1) ? 2 : 1); mpe1_idx++) {
				if (qid_mask->mpe1_mask) {
					qidt_elm.mpe1 = mpe1_idx;
				} else {
					qidt_elm.mpe1 = qid_set->mpe1;
				}

				/* Calc MPE2 */
				for (mpe2_idx = 0; mpe2_idx < ((qid_mask->mpe2_mask == 1) ? 2 : 1); mpe2_idx++) {
					if (qid_mask->mpe2_mask) {
						qidt_elm.mpe2 = mpe2_idx;
					} else {
						qidt_elm.mpe2 = qid_set->mpe2;
					}

					/* Calc ENC */
					for (enc_idx = 0; enc_idx < ((qid_mask->enc_mask == 1) ? 2 : 1); enc_idx++) {
						if (qid_mask->enc_mask) {
							qidt_elm.enc = enc_idx;
						} else {
							qidt_elm.enc = qid_set->enc;
						}

						/* Calc DEC */
						for (dec_idx = 0; dec_idx < ((qid_mask->dec_mask == 1) ? 2 : 1); dec_idx++) {
							if (qid_mask->dec_mask) {
								qidt_elm.dec = dec_idx;
							} else {
								qidt_elm.dec = qid_set->dec;
							}

							/* Calc flowid lowbit */
							for (fl_idx = 0; fl_idx < ((qid_mask->flowid_lmask == 1) ? 2 : 1); fl_idx++) {
								if (qid_mask->flowid_lmask) {
									qidt_elm.flowidl = fl_idx;
								} else {
									qidt_elm.flowidl = qid_set->flowidl;
								}
								/* Calc flowid lowbit */
								for (fh_idx = 0; fh_idx < ((qid_mask->flowid_hmask == 1) ? 2 : 1); fh_idx++) {
									if (qid_mask->flowid_hmask) {
										qidt_elm.flowidh = fh_idx;
									} else {
										qidt_elm.flowidh = qid_set->flowidh;
									}
									cbm_qid_reg_set(&qidt_elm, qid_val);
								}

							}

						}
					}
				}
			}
		}
	}
}

int32_t
cbm_queue_map_get(
	int32_t tmu_queue_id,
	int32_t *num_entries,
	cbm_queue_map_entry_t **entries,
	uint32_t flags
	)
{
	int i = 0, j = 0, k = 0;
	u32 offset, index;
	unsigned long sys_flag;
	cbm_queue_map_entry_t *temp_entry;
	uint8_t *ptr;
	spin_lock_irqsave(&cbm_qidt_lock, sys_flag);
	if (((tmu_queue_id >= 0) && (tmu_queue_id <= 255))
		&& (cbm_qtable[tmu_queue_id].refcnt)) {
		for (i = 0, k = i; i < 0x1000; i += 1, k += 4) {
		    /*unrolling the loop*/
			ptr = (uint8_t *)&g_cbm_qidt_mirror[i].qidt_shadow;
			if (ptr[3] == tmu_queue_id) {
				g_qidt_help[j] = k + 3;
				j++;
			}
			if (ptr[2] == tmu_queue_id) {
				g_qidt_help[j] = k + 2;
				j++;
			}
			if (ptr[1] == tmu_queue_id) {
				g_qidt_help[j] = k + 1;
				j++;
			}
			if (ptr[0] == tmu_queue_id) {
				g_qidt_help[j] = k;
				j++;
			}
		}
	} else {
		spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);
		return CBM_FAILURE;
	}
	temp_entry = (cbm_queue_map_entry_t *)kmalloc(sizeof(cbm_queue_map_entry_t) * (j), GFP_ATOMIC);
	if (temp_entry) {
		*entries = temp_entry;
		*num_entries = j;
		for (i = 0; i < j; i++) {
			index = (g_qidt_help[i] / 4) << 2;
			offset = (g_qidt_help[i] % 4);
			index = index + 3 - offset;

			temp_entry[i].regval = 0;
			temp_entry[i].flowid = ((index & 0x2000) | (index & 0x1000)) >> 12;
			temp_entry[i].dec = (index & 0x800) >> 11;
			temp_entry[i].enc = (index & 0x400) >> 10;
			temp_entry[i].mpe1 = (index & 0x100) >> 8;
			temp_entry[i].mpe2 = (index & 0x200) >> 9;
			temp_entry[i].ep = (index & 0xF0) >> 4;
			temp_entry[i].tc = (index & 0x0F) >> 0;
		}
	}
	spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);
	return CBM_SUCCESS;
}

EXPORT_SYMBOL(cbm_queue_map_get);

int32_t
cbm_queue_map_set(
int32_t tmu_queue_id,
cbm_queue_map_entry_t *entry,
uint32_t flags
)
{

	unsigned long sys_flag;
	struct cbm_qidt_elm qidt_elm;
	struct cbm_qidt_mask qidt_mask = {0};
	if (!entry)
		return CBM_FAILURE;
	/*(pr_info("q %d\r\n", tmu_queue_id);*/
	qidt_elm.clsid = entry->tc;
	qidt_elm.ep = entry->ep;
	qidt_elm.mpe1 = entry->mpe1;
	qidt_elm.mpe2 = entry->mpe2;
	qidt_elm.enc = entry->enc;
	qidt_elm.dec = entry->dec;
	qidt_elm.flowidl = entry->flowid & 0x1;
	qidt_elm.flowidh = entry->flowid >> 1;

	if (flags & CBM_QUEUE_MAP_F_FLOWID_L_DONTCARE) {
		qidt_mask.flowid_lmask = 1;
	}
    if (flags & CBM_QUEUE_MAP_F_FLOWID_H_DONTCARE) {
		qidt_mask.flowid_hmask = 1;
	}
	if (flags & CBM_QUEUE_MAP_F_EN_DONTCARE) {
		qidt_mask.enc_mask = 1;
	};
	if (flags & CBM_QUEUE_MAP_F_DE_DONTCARE) {
		qidt_mask.dec_mask = 1;
	};
	if (flags & CBM_QUEUE_MAP_F_MPE1_DONTCARE) {
		qidt_mask.mpe1_mask = 1;
	};
	if (flags & CBM_QUEUE_MAP_F_MPE2_DONTCARE) {
		qidt_mask.mpe2_mask = 1;
	};
	if (flags & CBM_QUEUE_MAP_F_EP_DONTCARE) {
		qidt_mask.ep_mask = 1;
	};
	if (flags & CBM_QUEUE_MAP_F_TC_DONTCARE) {
		qidt_mask.classid_mask = 1;
	};

	spin_lock_irqsave(&cbm_qidt_lock, sys_flag);
	cbm_qidt_set(&qidt_elm, &qidt_mask, tmu_queue_id);
	spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);

	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_queue_map_set);

/*
 *	CBM CPU Enqueue API
 */
static int cbm_setup_desc(struct cbm_desc *desc, uint32_t data_ptr, uint32_t data_len,
uint32_t DW0, uint32_t DW1)
{

	struct dma_desc temp_desc;
	struct cbm_desc *ptr;
	if (!desc) {
		return CBM_FAILURE;
	}

	memset(&temp_desc, 0, sizeof(struct dma_desc));
	temp_desc.data_ptr		= data_ptr & (~0x7);
	temp_desc.data_len		= data_len;
	temp_desc.byte_offset	= data_ptr & 0x7;
	temp_desc.sop			= 1;
	temp_desc.eop			= 1;
	temp_desc.own			= 0;
	ptr = (struct cbm_desc *)&temp_desc;
	ptr->desc0 = DW0;
	ptr->desc1 = DW1;
	#ifdef CONFIG_LITTLE_ENDIAN
	desc->desc0 = ptr->desc0;
	desc->desc1 = ptr->desc1;
	desc->desc2 = ptr->desc2;
	desc->desc3 = ptr->desc3;
	#else
	desc->desc0 = ptr->desc1;
	desc->desc1 = ptr->desc0;
	desc->desc2 = ptr->desc3;
	desc->desc3 = ptr->desc2;
	#endif
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_setup_desc);

void dump_cbm_desc(struct cbm_desc *desc, int detail)
{
	struct dma_desc *des = (struct dma_desc *)desc;
	LOGF_KLOG_INFO("The RAW value of the CBM desc:\n");

	LOGF_KLOG_INFO("desc 0: 0x%x\n", desc->desc0);
	LOGF_KLOG_INFO("desc 1: 0x%x\n", desc->desc1);
	LOGF_KLOG_INFO("desc 2: 0x%x\n", desc->desc2);
	LOGF_KLOG_INFO("desc 3: 0x%x\n", desc->desc3);

	if (!detail)
		return;

	LOGF_KLOG_INFO("The detail of the DMA desc:\n");

	LOGF_KLOG_INFO("dest sub interface id: %d\n",   des->dest_sub_if_id);
	LOGF_KLOG_INFO("eth type:              %d\n",   des->eth_type);
	LOGF_KLOG_INFO("flow id:               %d n",   des->flow_id);
	LOGF_KLOG_INFO("tunnel id:             %d\n\n", des->tunnel_id);

	LOGF_KLOG_INFO("class id:              %d\n",   des->classid);
	LOGF_KLOG_INFO("ep:                    %d\n",   des->ep);
	LOGF_KLOG_INFO("color:                 %d\n",   des->color);
	LOGF_KLOG_INFO("mpe1:                  %d\n",   des->mpe1);
	LOGF_KLOG_INFO("mpe2:                  %d\n",   des->mpe2);
	LOGF_KLOG_INFO("enc:                   %d\n",   des->enc);
	LOGF_KLOG_INFO("dec:                   %d\n",   des->dec);
	LOGF_KLOG_INFO("nat:                   %d\n",   des->nat);
	LOGF_KLOG_INFO("tcp_err:               %d\n",   des->tcp_err);
	LOGF_KLOG_INFO("session id:            %d\n\n", des->session_id);

	LOGF_KLOG_INFO("data ptr:              %d\n\n", des->data_ptr);

	LOGF_KLOG_INFO("data len:              %d\n",   des->data_len);
	LOGF_KLOG_INFO("mpoa mode:             %d\n",   des->mpoa_mode);
	LOGF_KLOG_INFO("mpoa pt:               %d\n",   des->mpoa_pt);
	LOGF_KLOG_INFO("qid:                   %d\n",   des->qid);
	LOGF_KLOG_INFO("byte offset:           %d\n",   des->byte_offset);
	LOGF_KLOG_INFO("pdu type:              %d\n",   des->pdu_type);
	LOGF_KLOG_INFO("dic:                   %d\n",   des->dic);
	LOGF_KLOG_INFO("eop:                   %d\n",   des->eop);
	LOGF_KLOG_INFO("sop:                   %d\n",   des->sop);
	LOGF_KLOG_INFO("complete:              %d\n",   des->c);
	LOGF_KLOG_INFO("own:                   %d\n",   des->own);

	return;
}

static int cbm_cpu_enqueue(uint32_t pid, struct cbm_desc *desc)
{
	uint32_t data_pointer, size_to_wb, pointer_to_wb;
	if (!desc || pid >= CPU_EQM_PORT_NUM /*|| ((1 << pid) & g_cpu_port_alloc) == 0*/)
		return CBM_FAILURE;


	size_to_wb = ((desc->desc2 >= g_cbm_buff.std_buf_addr) && (desc->desc2 <= (g_cbm_buff.std_buf_addr + g_cbm_buff.std_buf_size))) ?
		CBM_STD_BUF_SIZE : CONFIG_CBM_JBO_PKT_SIZE;
	pointer_to_wb = (size_to_wb == CBM_STD_BUF_SIZE) ? (desc->desc2 & 0xfffff800) : (desc->desc2 & g_cbm_ctrl.jumbo_size_mask);

	if ((size_to_wb == CONFIG_CBM_JBO_PKT_SIZE) && (get_cnt_of_ptr(desc->desc2) > 1)) {
		size_to_wb = desc->desc3 & 0x0000FFFF;
		pointer_to_wb = desc->desc2;
	}

	data_pointer = dma_map_single(g_cbm_ctrl.dev, (void *) pointer_to_wb,
		size_to_wb, DMA_TO_DEVICE);
	if (dma_mapping_error(g_cbm_ctrl.dev, data_pointer)) {
		LOGF_KLOG_DEV_ERROR(g_cbm_ctrl.dev, "%s DMA map failed\n", __func__);
		return CBM_FAILURE;
	}
	data_pointer += desc->desc2 - pointer_to_wb;
	cbm_cpu_enqueue_hw(pid, desc, (void *)data_pointer, 0);
	return CBM_SUCCESS;
}
/* flags : STD Buffer or Jumbo buffer using existing defines
Caller must have checked the parameter validity etc before calling this API  */
int cbm_cpu_enqueue_hw(uint32_t pid, struct cbm_desc *desc, void *data_pointer,  int flags)
{
	unsigned long sys_flag;
	local_irq_save(sys_flag);
	xrx500_cbm_w32(eqmdesc[pid], desc->desc0);
	xrx500_cbm_w32(eqmdesc[pid] + 4, desc->desc1);
	xrx500_cbm_w32(eqmdesc[pid] + 8, (uint32_t)data_pointer);
	asm("sync");
	xrx500_cbm_w32(eqmdesc[pid] + 12, desc->desc3);
	asm("sync");
	local_irq_restore(sys_flag);
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_cpu_enqueue_hw);

static void store_bufreq_baseaddr(void)
{
	bufreq[0] = CBM_EQM_BASE + CBM_EQM_CPU_PORT(0, new_sptr);
	bufreq[1] = CBM_EQM_BASE + CBM_EQM_CPU_PORT(0, new_jptr);
#ifdef CONFIG_CBM_LS_ENABLE
	dqmdesc[0] = CBM_DQM_BASE + CBM_DQM_CPU_PORT(2, desc0.desc0);
	dqmdesc[1] = CBM_DQM_BASE + CBM_DQM_CPU_PORT(2, desc1.desc0);
#endif
	eqmdesc[0] = CBM_EQM_BASE + CBM_EQM_CPU_PORT(0, desc0.desc0);
	eqmdesc[1] = CBM_EQM_BASE + CBM_EQM_CPU_PORT(1, desc0.desc0);
	eqmdesc[2] = CBM_EQM_BASE + CBM_EQM_CPU_PORT(2, desc0.desc0);
	eqmdesc[3] = CBM_EQM_BASE + CBM_EQM_CPU_PORT(3, desc0.desc0);
	cbm_cntr_func[0].reg = CBM_EQM_BASE + CBM_EQM_CTRL;
	cbm_cntr_func[1].reg = CBM_DQM_BASE + CBM_DQM_CTRL;

}
void *cbm_alloc_buf_by_size(uint32_t size)
{
	void *ptr = NULL;
	unsigned int cpu = smp_processor_id();
	int32_t *jumbo_buf_used = &per_cpu(g_cbm_jbo_divide.jumbo_buf_used, cpu);
	void **curr_jumbo_buf = &per_cpu(g_cbm_jbo_divide.curr_jumbo_buf, cpu);
	int32_t *curr_jumbo_buf_bytes_used = &per_cpu(g_cbm_jbo_divide.curr_jumbo_buf_bytes_used, cpu);
	int32_t *curr_jumbo_buf_alloc_no = &per_cpu(g_cbm_jbo_divide.curr_jumbo_buf_alloc_no, cpu);
	if (*curr_jumbo_buf == NULL
	|| get_cnt_of_ptr((uint32_t)*curr_jumbo_buf)  == CBM_MAX_RCNT
	|| (CONFIG_CBM_JBO_PKT_SIZE - *curr_jumbo_buf_bytes_used) < size) {
		if (*curr_jumbo_buf) {
		/* Adjust RCNT in HW */
			cbm_buffer_free(cpu, (uint32_t)*curr_jumbo_buf, 0);
		}
		/* Get new Jumbo buffer from CBM */
		*curr_jumbo_buf = cbm_buffer_alloc(cpu, CBM_PORT_F_JUMBO_BUF);
		(*jumbo_buf_used)++;
		*curr_jumbo_buf_alloc_no = 0;
		*curr_jumbo_buf_bytes_used = 0;
	}

	ptr = *curr_jumbo_buf + *curr_jumbo_buf_bytes_used;
	*curr_jumbo_buf_bytes_used += size + CBM_JUMBO_BUF_SPLIT_GUARD_BAND;
	(*curr_jumbo_buf_alloc_no)++;
	if (*curr_jumbo_buf_alloc_no == CBM_MAX_RCNT) {
		*curr_jumbo_buf = NULL;
	} else {
		cbm_buffer_upd_rcnt(cpu, *curr_jumbo_buf,
		CBM_CPU_BUF_INC_RCNT);
	}
	return ptr;
}

struct sk_buff *cbm_alloc_skb_by_size(
	unsigned int size,
	gfp_t priority
	)
{
	void *buf = NULL;
	struct sk_buff *skbuf = NULL;
	size = SKB_DATA_ALIGN(size + CBM_DMA_DATA_OFFSET + NET_SKB_PAD)
			 + SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
	buf = cbm_alloc_buf_by_size(size);
	if (buf) {
		skbuf = cbm_build_skb((void *)buf,
			size, priority);
		if (skbuf) {
			skb_reserve(skbuf, CBM_DMA_DATA_OFFSET);
		} else {
			cbm_buffer_free(smp_processor_id(), (uint32_t) buf, 0);
		}
	}
	return skbuf;
}
EXPORT_SYMBOL(cbm_alloc_skb_by_size);

void *cbm_buffer_alloc(uint32_t pid, uint32_t flag)
{
	uint32_t buf_addr = 0;
	uint32_t i = 0;
	unsigned long sys_flag;


	if (pid >= CPU_EQM_PORT_NUM /*|| (((1 << pid) & g_cpu_port_alloc) == 0)*/) {
		cbm_err("illegal pid: %d, legal cpu bitmap: 0x%x\n", pid, (unsigned int) (g_cpu_port_alloc));
		return NULL;
	}
	local_irq_save(sys_flag);
	do {
		buf_addr = xrx500_cbm_r32(bufreq[flag >> 1] + (pid * 0x1000));
	} while ((buf_addr & 0xFFFFF800) == 0xFFFFF800 && (i++) < DEFAULT_WAIT_CYCLES);

	if ((buf_addr & 0xFFFFF800) == 0xFFFFF800) {
		cbm_err("alloc buffer fail for portid: %d type %d ofsc %d", pid, flag, cbm_get_std_free_count());
		local_irq_restore(sys_flag);
		return NULL;
	}
	local_irq_restore(sys_flag);

	return (void *)__va(buf_addr);
}
EXPORT_SYMBOL(cbm_buffer_alloc);

struct sk_buff *cbm_copy_skb(const struct sk_buff *skb, gfp_t gfp_mask)
{
	int headerlen = skb_headroom(skb);
	unsigned int size = skb_end_offset(skb) + skb->data_len;
	struct sk_buff *n = cbm_alloc_skb(size, gfp_mask);

	if (!n)
		return NULL;

	/* Set the data pointer */
	skb_reserve(n, headerlen);
	/* Set the tail pointer and length */
	skb_put(n, skb->len);

	if (skb_copy_bits(skb, -headerlen, n->head, headerlen + skb->len))
		BUG();

	ltq_copy_skb_header(n, skb);
	return n;
}
EXPORT_SYMBOL(cbm_copy_skb);

struct sk_buff *cbm_alloc_skb(
	unsigned int size,
	gfp_t priority
	)
{
	uint32_t flag = CBM_PORT_F_STANDARD_BUF;
	void *buf = NULL;
	struct sk_buff *skbuf = NULL;
	size = SKB_DATA_ALIGN(size + CBM_DMA_DATA_OFFSET + NET_SKB_PAD)
			 + SKB_DATA_ALIGN(sizeof(struct skb_shared_info));

	if (size > CBM_STD_BUF_SIZE) {
		flag = CBM_PORT_F_JUMBO_BUF;
	}
	buf = cbm_buffer_alloc(smp_processor_id(), flag);
	if (buf) {
		skbuf = cbm_build_skb((void *)buf,
			size, priority);
		if (skbuf) {
			skb_reserve(skbuf, CBM_DMA_DATA_OFFSET);
		} else {
			cbm_buffer_free(smp_processor_id(), (uint32_t) buf, 0);
		}
	}
	return skbuf;
}
EXPORT_SYMBOL(cbm_alloc_skb);

int cbm_buffer_free(uint32_t pid, uint32_t v_buf, uint32_t flag)
{
	uint32_t buf;
	unsigned long sys_flag, cnt;
	uint32_t size_to_wb, pointer_to_wb;

	if (pid >= CPU_DQM_PORT_NUM /*|| (((1 << pid) & g_cpu_port_alloc) == 0)*/) {
		cbm_err("illegal pid: %d, legal cpu bitmap: 0x%x", pid, (unsigned int)g_cpu_port_alloc);
		return CBM_FAILURE;
	}
	if (flag)
		v_buf = (uint32_t)__va(v_buf);
	size_to_wb = ((v_buf >= g_cbm_buff.std_buf_addr) && (v_buf <= (g_cbm_buff.std_buf_addr + g_cbm_buff.std_buf_size))) ?
		CBM_STD_BUF_SIZE : CONFIG_CBM_JBO_PKT_SIZE;
	pointer_to_wb = (size_to_wb == CBM_STD_BUF_SIZE) ? (v_buf & 0xfffff800) : (v_buf & g_cbm_ctrl.jumbo_size_mask);

	cnt = get_cnt_of_ptr(v_buf);
	#ifdef ENABLE_LL_DEBUG
	if ((cnt == 1) || (cnt == 0)) {
	#else
	WARN_ON(!cnt);
	if (cnt == 1) {
	#endif
		buf = dma_map_single(
		g_cbm_ctrl.dev,
		(void *) pointer_to_wb,
		size_to_wb,
		DMA_FROM_DEVICE);
		if (dma_mapping_error(g_cbm_ctrl.dev, buf)) {
			LOGF_KLOG_DEV_ERROR(g_cbm_ctrl.dev, "%s DMA map failed\n", __func__);
			return CBM_FAILURE;
		}
	} else {
		buf = __pa(pointer_to_wb);
	}
	local_irq_save(sys_flag);
	xrx500_cbm_w32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(pid, ptr_rtn), buf);
	local_irq_restore(sys_flag);
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_buffer_free);



#ifdef CONFIG_CBM_LS_ENABLE
static inline int cbm_dq_deq_list()
{
	int i = 0;
	static int to_read;
	int l_read = to_read;
	int rx = 0;
		g_cbm_dq_dlist[i].desc.desc3 = xrx500_cbm_r32(dqmdesc[l_read] + 12);
		if (!(g_cbm_dq_dlist[i].desc.desc3 & 0x80000000)) {
			goto DONE;
		}
		g_cbm_dq_dlist[i].desc.desc0 = xrx500_cbm_r32(dqmdesc[l_read]);
		g_cbm_dq_dlist[i].desc.desc1 = xrx500_cbm_r32(dqmdesc[l_read] + 4);
		g_cbm_dq_dlist[i].desc.desc2 = xrx500_cbm_r32(dqmdesc[l_read] + 8);
		xrx500_cbm_w32(dqmdesc[l_read] + 12, 0);
		i++;
		rx++;
		to_read = !l_read;
DONE:
	LOGF_KLOG_DEBUG("rx %d\r\n", rx);
	return rx;
}

int get_budget()
{
	return g_budget;
}
#endif
int cbm_ls_deq_list(uint32_t pid)
{
	int budget = 50, i = 0;
	int ret = CBM_SUCCESS;
	while (((xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pid, status)) >> 7) & 0xF) != 0) {
		if (budget-- < 0) {
			ret = CBM_RESCHEDULE;
			break;
		}
			g_cbm_dlist[pid][i].desc.desc0 = xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pid, desc.desc0));
			g_cbm_dlist[pid][i].desc.desc1 = xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pid, desc.desc1));
			g_cbm_dlist[pid][i].desc.desc2 = xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pid, desc.desc2));
			g_cbm_dlist[pid][i].desc.desc3 = xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pid, desc.desc3));
			i++;
	}
	g_cbm_dlist[pid][i].desc.desc2 = 0;
	return ret;
}

int cbm_cpu_dequeue_list(uint32_t pid)
{
	if (pid >= CPU_DQM_PORT_NUM || (((1 << pid) & g_cpu_port_alloc) == 0))
	return CBM_FAILURE;
	return cbm_ls_deq_list(pid);
}
EXPORT_SYMBOL(cbm_cpu_dequeue_list);

#if 0
int cbm_ls_deq(uint32_t pid, struct cbm_desc *desc)
{
	if (((xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pid, status)) >> 7) & 0xF) != 0) {
		desc->desc0 = xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pid, desc.desc0));
		desc->desc1 = xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pid, desc.desc1));
		desc->desc2 = xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pid, desc.desc2));
		desc->desc3 = xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pid, desc.desc3));

		desc->desc2 = phy2vrtl(desc->desc2);
		return CBM_SUCCESS;
	}
	return CBM_FAILURE;
}

int cbm_cpu_dequeue(uint32_t pid, struct cbm_desc *desc, int *quota, uint32_t *buf_per_port)
{
	int i;
	if (pid >= CPU_DQM_PORT_NUM)
	return CBM_FAILURE;

	for (i = 0; i < *quota; i++) {
		if (cbm_ls_deq(pid, desc) == CBM_SUCCESS) {
			(*buf_per_port)++;
			desc += 1;
		} else {
			break;
		}
	}

	*quota = *quota - i;
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_cpu_dequeue);
#endif

/*
 *	return 1: pointer is valid
 *
 *		 0: pointer is invalid
*/
inline int check_ptr_validation(uint32_t buf)
{
	if (((buf >= g_cbm_buff.std_buf_addr) && (buf <= (g_cbm_buff.std_buf_addr + g_cbm_buff.std_buf_size)))
		|| ((buf >= g_cbm_buff.jbo_buf_addr) && (buf <= (g_cbm_buff.jbo_buf_addr + g_cbm_buff.jbo_buf_size)))) {
		return 1;
	}
	return 0;
}
EXPORT_SYMBOL(check_ptr_validation);
inline int check_ptr_valid_type(uint32_t buf)
{

	if ((buf >= g_cbm_buff.std_buf_addr) && (buf <= (g_cbm_buff.std_buf_addr + g_cbm_buff.std_buf_size))) {
		return SBA0_STD;
	} else if ((buf >= g_cbm_buff.jbo_buf_addr) && (buf <= (g_cbm_buff.jbo_buf_addr + g_cbm_buff.jbo_buf_size))) {
		return SBA0_JBO;
	}
	return 0;
}

u32 cbm_linearise_buf(struct sk_buff *skb)
{
	u32 new_buf, tmp_buf;
	void *frag_addr;
	const skb_frag_t *frag;
	int i, len = 0, tot_len = 0, copy_len = 0;
	int buf_size;

	SKB_FRAG_ASSERT(skb);
    /*This check is not needed at the moment*/
	/*WARN_ON(skb_shared(skb));*/

	tot_len = skb->len;
	if (tot_len <= CBM_STD_BUF_SIZE) {
		new_buf = (u32) cbm_buffer_alloc(smp_processor_id(), CBM_PORT_F_STANDARD_BUF);
		buf_size = CBM_STD_BUF_SIZE;
	} else if (tot_len <= CONFIG_CBM_JBO_PKT_SIZE) {
		new_buf = (u32) cbm_buffer_alloc(smp_processor_id(), CBM_PORT_F_JUMBO_BUF);
		buf_size = CONFIG_CBM_JBO_PKT_SIZE;
	} else {
		LOGF_KLOG_ERROR("%s:11:length exceeds buf size tot_len %d \n", __func__, tot_len);
		return 0;
	}
	if (new_buf) {
		/*reserve the headroom for the HW */
		new_buf += CBM_DMA_DATA_OFFSET + NET_IP_ALIGN + NET_SKB_PAD;
		tmp_buf = new_buf;
		if (skb_is_nonlinear(skb)) {
			LOGF_KLOG_DEBUG("skb is non-linear in cbm\n");
			copy_len = skb_headlen(skb);
		} else
			copy_len = skb->len;
		memcpy((u8 *)tmp_buf, skb->data, copy_len);
		tmp_buf += copy_len;
		/* If the packet has fragments, copy that also */
		for (i = 0; i < (skb_shinfo(skb)->nr_frags); i++) {
			frag = &skb_shinfo(skb)->frags[i];
			len = skb_frag_size(frag);
			frag_addr = skb_frag_address(frag);
			if (len < (buf_size - copy_len)) {
				 memcpy((u8 *)tmp_buf, (u8 *)frag_addr, len);
				 tmp_buf += len;
				 copy_len += len;
			} else {
				 LOGF_KLOG_ERROR("%s:22:copied = %d and remaining = %d and skb->head is %x \n", __func__, copy_len, len, (u32)(skb->head));
				 cbm_buffer_free(smp_processor_id(), new_buf, 0);
				 return 0;
			}
		}
	} else { /* no buffer */
		LOGF_KLOG_ERROR("%s:33:no buffer in CBM HW ! \n", __func__);
		return 0;
	}
	return new_buf;
}
#if 0
static void dump_raw_data(unsigned char *p, int len, char *prefix_str)
{
	int i;
	int line_num = 32;

	printk("%s:\n", prefix_str ? prefix_str : "Data");
	for (i = 0; i < len; i++) {
		if ((i % line_num) == 0)
			printk("  ");
		printk("%02x ", p[i]);
		if ((i % line_num) == (line_num - 1))
			printk("\n");
	}
	printk("\n");
}
#endif

/* Transmit the packet sent by the upper layer */

int32_t
cbm_cpu_pkt_tx(
	struct sk_buff *skb,
	int32_t pmac_port_id,
	uint32_t flags
	)
{
	struct cbm_desc desc;
	uint32_t tmp_data_ptr;
	struct dma_tx_desc_2 *desc_2 = (struct dma_tx_desc_2 *) &skb->DW2;
	u32 new_buf;


	if (!check_ptr_validation((u32)(skb->head)) || skb_cloned(skb)) {
	/*	LOGF_KLOG_INFO("11:buffer status: %d and cloned status: %d\n", check_ptr_validation((u32) (skb->head)),  skb_cloned(skb));*/
		new_buf = cbm_linearise_buf(skb);
		if (!new_buf) {
			LOGF_KLOG_ERROR("%s: cbm buffer alloc failed ..\n", __func__);
			dev_kfree_skb_any(skb);
			return CBM_FAILURE;
		}
		tmp_data_ptr = new_buf;
	} else {
/*		LOGF_KLOG_INFO("22:buffer status: %d and cloned status: %d\n", check_ptr_validation((u32) (skb->head)),  skb_cloned(skb));*/
		tmp_data_ptr = desc_2->field.data_ptr;
		/* Detach th skb */
		skb->head = NULL;
	}

	/*dump_raw_data(new_buf, 96, "Tx Data after linearise");*/

	if (cbm_setup_desc((struct cbm_desc *) &desc, tmp_data_ptr,
			(skb->len < (ETH_ZLEN + 8)) ? (ETH_ZLEN + 8) : skb->len,
			skb->DW1, skb->DW0)){
		LOGF_KLOG_ERROR("cbm setup desc failed..\n");
		dev_kfree_skb_any(skb);
		cbm_buffer_free(smp_processor_id(), tmp_data_ptr, 0);
		/*spin_unlock_irqrestore(&cbm_tx_lock, sys_flag);*/
		return CBM_FAILURE;
	}
	/*copy the pdu type bit for DSL*/
	if (skb->DW3 & BIT(CBM_PDU_TYPE))
		desc.desc3 |= BIT(CBM_PDU_TYPE);
	/*spin_lock_irqsave(&cbm_tx_lock, sys_flag);*/

	dev_kfree_skb_any(skb);

	if (cbm_cpu_enqueue(smp_processor_id(), &desc) == CBM_FAILURE) {
		LOGF_KLOG_ERROR("cpu enqueue failed..\n");
		cbm_buffer_free(smp_processor_id(), tmp_data_ptr, 0);
		return CBM_FAILURE;
	}
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_cpu_pkt_tx);


void cbm_hw_init(void)
{
	init_fsqm();
	init_cbm_basic();
	LOGF_KLOG_INFO("CBM HW init\n");
	return;
}


int cbm_set_cpu_alloc_offset(uint32_t pid,
uint32_t jbo,
unsigned int offset)
{
	if (pid > 4) {
		cbm_err("pid:[%d]  > 4", pid);
		return CBM_FAILURE;
	}

	if (!jbo) {
		xrx500_cbm_w32(CBM_EQM_BASE +  CBM_EQM_CPU_PORT(pid, new_sptr),
		offset);
	} else {
		xrx500_cbm_w32(CBM_EQM_BASE +  CBM_EQM_CPU_PORT(pid, new_jptr),
		offset);
	}

	return CBM_SUCCESS;
}

uint32_t cbm_get_total_buf_num(uint32_t flag)
{
	if (flag == 0) {/*STD*/
		return g_cbm_buff.std_frm_num - 2; /* !!!should change to -1 after HW updated*/
	} else {
		return g_cbm_buff.jbo_frm_num - 2;
	}
}


/*
* CBM assistant function
*/
void cbm_dw_memset(uint32_t *base, int val, uint32_t size)
{
	int i;
	for (i = 0; i < size; i++) {
		base[i] = val;
	}

	return;
}

uint32_t cbm_get_std_free_count (void) {
	return xrx500_cbm_r32(FSQM_BASE(g_cbm_buff.std_fsqm_idx) + OFSC);
}
EXPORT_SYMBOL(cbm_get_std_free_count);

/*
 *	CBM Interrupt handler
 */

void fsqm_intr_handler(uint32_t fsqm_idx)
{
	uint32_t fsqm_intr_stat;
	int i;

	fsqm_intr_stat = xrx500_cbm_r32(FSQM_BASE(fsqm_idx) + FSQM_IRNCR);

	LOGF_KLOG_INFO("fsqm[%d] status: 0x%x\n",
	fsqm_idx, fsqm_intr_stat);
	for (i = 0; i < 5; i++) {
		if (fsqm_intr_stat & (1 << i)) {
			LOGF_KLOG_ERR_ONCE("fsqm[%d] watermark[%d] interrupt!\n",
			fsqm_idx, i);
		}
	}
	if (fsqm_intr_stat & (1 << 12)) {
		LOGF_KLOG_ERR_ONCE("RAM Access Violation interrupt\n");
	}
	if (fsqm_intr_stat & (1 << 16)) {
		LOGF_KLOG_ERR_ONCE("Free Command RAM Access Violation\n");
	}
	if (fsqm_intr_stat & (1 << 20)) {
		LOGF_KLOG_ERR_ONCE("Free Command OverFlow\n");
	}
	if (fsqm_intr_stat & (1 << 24)) {
		LOGF_KLOG_ERR_ONCE("Alloc Command Nil Response\n");
	}

	LOGF_KLOG_DEBUG("Clear FSQM interrupt status\n");
	xrx500_cbm_w32(FSQM_BASE(fsqm_idx) + FSQM_IRNCR, fsqm_intr_stat);
	LOGF_KLOG_DEBUG("Double check the fsqm intr status:0x%x\n",
	xrx500_cbm_r32(FSQM_BASE(fsqm_idx) + FSQM_IRNCR));

}

void ls_intr_handler(uint32_t pid)
{
	uint32_t ls_intr_stat;

	ls_intr_stat = xrx500_cbm_r32(CBM_LS_BASE + IRNICR_LS);
	LOGF_KLOG_DEBUG("ls interrupt status: 0x%x\n", ls_intr_stat);

	if (ls_intr_stat & (3 << (pid * 2 + 16))) {
		LOGF_KLOG_DEBUG("pid:%d, interrupt\n", pid);
	}

	LOGF_KLOG_DEBUG("Clear ls intr status on pid:%d\n", pid);
	ls_intr_stat &= (3 << (pid * 2 + 16));
	xrx500_cbm_w32(CBM_LS_BASE + IRNCR_LS, ls_intr_stat);

	LOGF_KLOG_DEBUG("Double check the ls status after clear: 0x%x\n",
	xrx500_cbm_r32(CBM_LS_BASE + IRNCR_LS));

}

void igp_cpu_intr_handler(uint32_t pid)
{
	uint32_t eqm_intr_stat;

	eqm_intr_stat = xrx500_cbm_r32(CBM_EQM_BASE
	+ CBM_EQM_CPU_PORT(pid, irncr));
	LOGF_KLOG_ERR_ONCE("igp interrupt for pid: %d, stat: 0x%x\n", pid, eqm_intr_stat);

	if (eqm_intr_stat & (1 << 0)) {
		LOGF_KLOG_ERR_ONCE("Packet Not Accept\n");
	}
	if (eqm_intr_stat & (1 << 1)) {
		LOGF_KLOG_ERR_ONCE("Low WaterMark Interrupt\n");
	}
	if (eqm_intr_stat & (1 << 2)) {
		LOGF_KLOG_ERR_ONCE("High WaterMark Interrupt\n");
	}
	if (eqm_intr_stat & (1 << 3)) {
		LOGF_KLOG_ERR_ONCE("Descriptor Ready for Enqueue\n");
	}
	if (eqm_intr_stat & (1 << 4)) {
		LOGF_KLOG_ERR_ONCE("Standard Pointer Ready\n");
	}
	if (eqm_intr_stat & (1 << 5)) {
		LOGF_KLOG_ERR_ONCE("Jumbo Pointer Ready\n");
	}

	LOGF_KLOG_DEBUG("Clear IGP intr status on pid:%d\n", pid);
	xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, irncr), eqm_intr_stat);

	LOGF_KLOG_DEBUG("Double check the IGP status after clear: 0x%x\n",
	xrx500_cbm_r32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, irncr)));
}

void egp_cpu_intr_handler(uint32_t pid)
{
	uint32_t dqm_intr_stat;

	dqm_intr_stat = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(pid, irncr));
	LOGF_KLOG_ERR_ONCE("dqm interrupt for pid: %d, stat: 0x%x\n", pid, dqm_intr_stat);

	if (dqm_intr_stat & 0x01) {
		LOGF_KLOG_ERR_ONCE("Register ready for pointer free on port: %d\n", pid);
	}

	LOGF_KLOG_DEBUG("Clear EGP intr status on pid:%d\n", pid);
	xrx500_cbm_w32(CBM_DQM_BASE + CBM_EQM_CPU_PORT(pid, irncr), dqm_intr_stat);

	LOGF_KLOG_DEBUG("Double check the EGP status after clear: 0x%x\n",
	xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(pid, irncr)));
}

static irqreturn_t cbm_isr_0(int irq, void *dev_id)
{
	uint32_t cbm_intr_status;
	uint32_t igp_intr_status;
	uint32_t egp_intr_status;

	cbm_intr_status = xrx500_cbm_r32(CBM_BASE
	+ CBM_INT_LINE(0, cbm_irncr));
	igp_intr_status = xrx500_cbm_r32(CBM_BASE
	+ CBM_INT_LINE(0, igp_irncr));
	egp_intr_status = xrx500_cbm_r32(CBM_BASE
	+ CBM_INT_LINE(0, egp_irncr));

	LOGF_KLOG_DEBUG("isr0: cbm_intr_status: 0x%x\n", cbm_intr_status);
	LOGF_KLOG_DEBUG("isr0: igp_intr_status: 0x%x\n", igp_intr_status);
	LOGF_KLOG_DEBUG("isr0: egp_intr_status: 0x%x\n", egp_intr_status);

	/* isr 0 by default bind FSQM interrupt and TMU interrupt */
	if (cbm_intr_status != 0) {
	if (cbm_intr_status & 3) {/*TMU interrupt*/
		LOGF_KLOG_DEBUG("TMU interrupt triggered!!! Impossible!!!\n");
		LOGF_KLOG_ERROR("TMU interrupt triggered!!! Impossible!!!\n");
	}
	if (cbm_intr_status & 0xF0) {/*FSQM interrupt */
		if (cbm_intr_status & 0x30) {
			LOGF_KLOG_DEBUG("FSQM0 interrupt!\n");
			LOGF_KLOG_ERROR("FSQM0 interrupt!\n");
			fsqm_intr_handler(0);
		} else {
			LOGF_KLOG_DEBUG("FSQM1 interrupt!\n");
			LOGF_KLOG_ERROR("FSQM1 interrupt!\n");
			fsqm_intr_handler(1);
		}
	}
	if (cbm_intr_status & 0x0100) {
		LOGF_KLOG_DEBUG("Load Spreader interrupt on pid 0\n");
		LOGF_KLOG_ERROR("Load Spreader interrupt on pid 0\n");
	}
	}

	if (igp_intr_status & 0x1) {
	igp_cpu_intr_handler(0);
	}

	LOGF_KLOG_DEBUG("Clear CBM intr source \n");
	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(0, cbm_irncr),
	cbm_intr_status);
	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(0, igp_irncr),
	igp_intr_status);
	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(0, egp_irncr),
	egp_intr_status);

	LOGF_KLOG_DEBUG("The status after clear\n");

	LOGF_KLOG_DEBUG("isr0: cbm_intr_status: 0x%x\n",
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(0, cbm_irncr)));
	LOGF_KLOG_DEBUG("isr0: igp_intr_status: 0x%x\n",
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(0, igp_irncr)));
	LOGF_KLOG_DEBUG("isr0: egp_intr_status: 0x%x\n",
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(0, egp_irncr)));

	return IRQ_HANDLED;
}

void cbm_isr_1(void)
{
	uint32_t cbm_intr_status;
	uint32_t igp_intr_status;
	uint32_t egp_intr_status;

	cbm_intr_status = xrx500_cbm_r32(CBM_BASE
	+ CBM_INT_LINE(1, cbm_irncr));
	igp_intr_status = xrx500_cbm_r32(CBM_BASE
	+ CBM_INT_LINE(1, igp_irncr));
	egp_intr_status = xrx500_cbm_r32(CBM_BASE
	+ CBM_INT_LINE(1, egp_irncr));

	LOGF_KLOG_DEBUG("isr1: cbm_intr_status: 0x%x\n", cbm_intr_status);
	LOGF_KLOG_DEBUG("isr1: igp_intr_status: 0x%x\n", igp_intr_status);
	LOGF_KLOG_DEBUG("isr1: egp_intr_status: 0x%x\n", egp_intr_status);

	if (igp_intr_status & 0x2) {
		igp_cpu_intr_handler(1);
	}

	LOGF_KLOG_DEBUG("Clear intr source \n");
	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(1, cbm_irncr),
	cbm_intr_status);
	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(1, igp_irncr),
	igp_intr_status);
	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(1, egp_irncr),
	egp_intr_status);

	LOGF_KLOG_DEBUG("The status after clear\n");
	LOGF_KLOG_DEBUG("isr1: cbm_intr_status: 0x%x\n",
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(1, cbm_irncr)));
	LOGF_KLOG_DEBUG("isr1: igp_intr_status: 0x%x\n",
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(1, igp_irncr)));
	LOGF_KLOG_DEBUG("isr1: egp_intr_status: 0x%x\n",
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(1, egp_irncr)));

}

void cbm_isr_2(void)
{
	uint32_t cbm_intr_status;
	uint32_t igp_intr_status;
	uint32_t egp_intr_status;

	cbm_intr_status = xrx500_cbm_r32(CBM_BASE
	+ CBM_INT_LINE(2, cbm_irncr));
	igp_intr_status = xrx500_cbm_r32(CBM_BASE
	+ CBM_INT_LINE(2, igp_irncr));
	egp_intr_status = xrx500_cbm_r32(CBM_BASE
	+ CBM_INT_LINE(2, egp_irncr));

	LOGF_KLOG_DEBUG("isr2: cbm_intr_status: 0x%x\n", cbm_intr_status);
	LOGF_KLOG_DEBUG("isr2: igp_intr_status: 0x%x\n", igp_intr_status);
	LOGF_KLOG_DEBUG("isr2: egp_intr_status: 0x%x\n", egp_intr_status);

	if (igp_intr_status & 0x4) {
		igp_cpu_intr_handler(2);
	}

	LOGF_KLOG_DEBUG("Clear intr source \n");

	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(2, cbm_irncr),
	cbm_intr_status);
	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(2, igp_irncr),
	igp_intr_status);
	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(2, egp_irncr),
	egp_intr_status);

	LOGF_KLOG_DEBUG("The status after clear\n");
	LOGF_KLOG_DEBUG("isr2: cbm_intr_status: 0x%x\n",
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(2, cbm_irncr)));
	LOGF_KLOG_DEBUG("isr2: igp_intr_status: 0x%x\n",
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(2, igp_irncr)));
	LOGF_KLOG_DEBUG("isr2: egp_intr_status: 0x%x\n",
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(2, egp_irncr)));
}

void cbm_isr_3(void)
{
	uint32_t cbm_intr_status;
	uint32_t igp_intr_status;
	uint32_t egp_intr_status;

	cbm_intr_status = xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(3, cbm_irncr));
	igp_intr_status = xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(3, igp_irncr));
	egp_intr_status = xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(3, egp_irncr));

	LOGF_KLOG_DEBUG("isr3: cbm_intr_status: 0x%x\n", cbm_intr_status);
	LOGF_KLOG_DEBUG("isr3: igp_intr_status: 0x%x\n", igp_intr_status);
	LOGF_KLOG_DEBUG("isr3: egp_intr_status: 0x%x\n", egp_intr_status);

	if (igp_intr_status & 0x8) {
		igp_cpu_intr_handler(3);
	}

	LOGF_KLOG_DEBUG("Clear intr source \n");

	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(3, cbm_irncr),
	cbm_intr_status);
	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(3, igp_irncr),
	igp_intr_status);
	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(3, egp_irncr),
	egp_intr_status);

	LOGF_KLOG_DEBUG("The status after clear\n");

}
#ifdef CONFIG_CBM_LS_ENABLE
static int do_dq_cbm_poll(struct napi_struct *napi, int budget)
{
	struct cbm_desc_list *desc_list;
	struct sk_buff *skb;
	int data_len = 0;
	uint32_t data_ptr;
	int real_len = 0, temp_len = 0;
	int deq_ret = 0;
	static int tasklet_loop;
	int complete = 0;

	while ((deq_ret < budget) && !complete) {
		if (cbm_dq_deq_list()) {
			LOGF_KLOG_DEBUG("deq_ret %d\r\n", deq_ret);
			deq_ret++;
			desc_list = &g_cbm_dq_dlist[0];
			/* Build the SKB */
			data_len = desc_list->desc.desc3 & 0x0000FFFF;
			data_ptr = (unsigned int) __va(desc_list->desc.desc2);
			dma_map_single(g_cbm_ctrl.dev, (void *) (data_ptr + ((desc_list->desc.desc3 & 0x3800000) >> 23)),
			data_len, DMA_FROM_DEVICE);
			if (dma_mapping_error(g_cbm_ctrl.dev, desc_list->desc.desc2)) {
				LOGF_KLOG_DEV_ERROR(g_cbm_ctrl.dev, "%s DMA map failed\n", __func__);
				break;
			}

			temp_len = data_len + CBM_DMA_DATA_OFFSET + NET_IP_ALIGN + NET_SKB_PAD;
			real_len = SKB_DATA_ALIGN(temp_len)
			+ SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
			skb = cbm_build_skb((void *) (data_ptr +
			((desc_list->desc.desc3 & 0x3800000) >> 23)
			- CBM_DMA_DATA_OFFSET - NET_IP_ALIGN - NET_SKB_PAD),
			real_len, GFP_ATOMIC);
			if (skb) {
				skb_reserve(skb,  CBM_DMA_DATA_OFFSET + NET_IP_ALIGN + NET_SKB_PAD);
				/* Call the datapath library Rx function */
				skb->DW0 = desc_list->desc.desc0;
				skb->DW1 = desc_list->desc.desc1;
				skb->DW2 = data_ptr;
				skb->DW3 = desc_list->desc.desc3;
				skb_put(skb, data_len);
				dp_rx(skb, 0);
				LOGF_KLOG_DEBUG ("%s: rx to datapath library\n", __func__);
			} else {
				LOGF_KLOG_ERROR("%s:failure in allocating skb, free cbm data pointer\r\n", __func__);
				cbm_buffer_free(smp_processor_id(), desc_list->desc.desc2, 0);
			}
		} else {
				tasklet_loop = (tasklet_loop >= 2) ? 0 : tasklet_loop + 1;
				if (tasklet_loop >= 2)
					complete = 1;
		}
	}
	g_budget++;
	LOGF_KLOG_DEBUG(" complete %d deq_ret %d\r\n", complete, deq_ret);
	/*tasklet_loop += 1;*/
	if ((complete || !deq_ret)) {
		LOGF_KLOG_DEBUG("complete\r\n");
		/*tasklet_loop = 0;*/
		napi_complete(napi);
		xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(6, egp_irnen)), 4);
	}
	return CBM_SUCCESS;
}
#endif

#ifdef ENABLE_LL_DEBUG
/** Trigger when taklet schedule calls*/
static void do_cbm_debug_tasklet(unsigned long cpu)
{
	uint32_t err_port = 0;
	uint32_t err_egpid = 0;
	uint32_t err_igpid = 0;
	uint32_t err_lsa = 0;


	err_port = get_val(xrx500_cbm_r32(CBM_BASE + CBM_LL_DBG_0),
	CBM_LL_DBG_0_ERR_PORT_MASK,
	CBM_LL_DBG_0_ERR_PORT_POS);

	err_egpid = get_val(xrx500_cbm_r32(CBM_BASE + CBM_LL_DBG_0),
	CBM_LL_DBG_0_ERR_EGPID_MASK,
	CBM_LL_DBG_0_ERR_EGPID_POS);

	err_igpid = get_val(xrx500_cbm_r32(CBM_BASE + CBM_LL_DBG_0),
	CBM_LL_DBG_0_ERR_IGPID_MASK,
	CBM_LL_DBG_0_ERR_IGPID_POS);

	err_lsa = get_val(xrx500_cbm_r32(CBM_BASE + CBM_LL_DBG_0),
	CBM_LL_DBG_0_ERR_LSA_MASK,
	CBM_LL_DBG_0_ERR_LSA_POS);
	if (err_port)
		LOGF_KLOG_ERROR("Double free egp %u Buff addr 0x%x\n", err_egpid, xrx500_cbm_r32(CBM_BASE + CBM_SBA_0) + (err_lsa * 0x800));
	else
		LOGF_KLOG_ERROR("Double free igp %u Buff addr 0x%x\n", err_igpid, xrx500_cbm_r32(CBM_BASE + CBM_SBA_0) + (err_lsa * 0x800));

	xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(5, cbm_irncr), 1 << 16);
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(5, cbm_irnicr));
	/*Dummy read for write flush*/
	xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(5, cbm_irncr));
	rmb();
	/* Enable all interrupts */
	xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(5, cbm_irnen)),
	(1) << 16);

}
#endif
/** Trigger when taklet schedule calls*/
static void do_cbm_tasklet(unsigned long cpu)
{
	struct cbm_desc_list *desc_list;
	struct sk_buff *skb;
	uint32_t pkt_recvd = 0;
	int data_len = 0, deq_ret = 0, j = 0;
	uint32_t data_ptr;
	int real_len = 0, temp_len = 0;

	/* Dequeue the packet */
	LOGF_KLOG_DEBUG("am running in cpu %lu\r\n", cpu);
	LOGF_KLOG_DEBUG("tasklet:cpu dequeue\n");
	pkt_recvd = get_val(xrx500_cbm_r32(CBM_LS_BASE +
	CBM_LS_PORT(cpu, status)),
	LS_STATUS_PORT0_QUEUE_LEN_MASK,
	LS_STATUS_PORT0_QUEUE_LEN_POS);

	LOGF_KLOG_DEBUG("Qlen Port %lu recv %d pkts\n", cpu, pkt_recvd);
	if (pkt_recvd > 0) {
		deq_ret = cbm_cpu_dequeue_list(cpu);
		desc_list = &g_cbm_dlist[cpu][0];
		while (desc_list->desc.desc2) {
			/* Build the SKB */
			data_len = desc_list->desc.desc3 & 0x0000FFFF;
			data_ptr = (unsigned int) __va(desc_list->desc.desc2);
			dma_map_single(g_cbm_ctrl.dev, (void *) (data_ptr + ((desc_list->desc.desc3 & 0x3800000) >> 23)),
			data_len, DMA_FROM_DEVICE);
			if (dma_mapping_error(g_cbm_ctrl.dev, desc_list->desc.desc2)) {
				LOGF_KLOG_DEV_ERROR(g_cbm_ctrl.dev, "%s DMA map failed\n", __func__);
				break;
			}

			temp_len = data_len + CBM_DMA_DATA_OFFSET + NET_IP_ALIGN + NET_SKB_PAD;
			real_len = SKB_DATA_ALIGN(temp_len)
			+ SKB_DATA_ALIGN(sizeof(struct skb_shared_info));
			skb = cbm_build_skb((void *) (data_ptr +
			((desc_list->desc.desc3 & 0x3800000) >> 23)
			- CBM_DMA_DATA_OFFSET - NET_IP_ALIGN - NET_SKB_PAD),
			real_len, GFP_ATOMIC);
			if (skb) {
				skb_reserve(skb,  CBM_DMA_DATA_OFFSET + NET_IP_ALIGN + NET_SKB_PAD);
				/* Call the datapath library Rx function */
				skb->DW0 = desc_list->desc.desc0;
				skb->DW1 = desc_list->desc.desc1;
				skb->DW2 = data_ptr;
				skb->DW3 = desc_list->desc.desc3;
				skb_put(skb, data_len);
				dp_rx(skb, 0);
				LOGF_KLOG_DEBUG("%s: rx to datapath library\n", __func__);
			} else {
				LOGF_KLOG_ERROR("%s:failure in allocating skb, free cbm data pointer\r\n", __func__);
				cbm_buffer_free(smp_processor_id(), desc_list->desc.desc2, 0);
			}
			desc_list = &g_cbm_dlist[cpu][++j];
		}
	}
	if ((deq_ret == CBM_RESCHEDULE) || !(xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(cpu, status)) & BIT(13))) {
		tasklet_schedule(&cbm_tasklet[smp_processor_id()]);
	} else {
		ls_intr_handler(cpu);
		wmb();
		xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(4 + cpu, cbm_irncr), BIT(cpu) << 8);
		xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(4 + cpu, cbm_irnicr));
		/*Dummy read for write flush*/
		xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(4 + cpu, cbm_irncr));
		rmb();
		/* Enable LS */
		xrx500_cbm_w32(CBM_LS_BASE + IRNEN_LS, xrx500_cbm_r32(CBM_LS_BASE + IRNEN_LS) | (3 << (cpu * 2 + 16)));
		/* Enable all interrupts */
		xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(4 + cpu, cbm_irnen)),
		(1 << cpu) << 8);
	}
}

static irqreturn_t cbm_isr_4(int irq, void *dev_id)
{
	int cpu = smp_processor_id();
	uint32_t cbm_intr_status;

	/*try disable, clear and then run the tasklet
	*/
	cbm_intr_status = xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(4, cbm_irncr));
	if (cbm_intr_status & 0x0100) {
		/* Disable all interrupts */
		xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(4, cbm_irnen)), 0);
		/*Dummy Read, for write flush*/
		xrx500_cbm_r32((CBM_BASE + CBM_INT_LINE(4, cbm_irnen)));
		/* Disable  LS interrupt */
		xrx500_cbm_w32(CBM_LS_BASE + IRNEN_LS, xrx500_cbm_r32(CBM_LS_BASE + IRNEN_LS) & (~(3 << (cpu * 2 + 16))));
		xrx500_cbm_r32(CBM_LS_BASE + IRNEN_LS);
		LOGF_KLOG_DEBUG("Load Spreader interrupt on pid 0\n");
		/* Schedule the tasklet */
		tasklet_schedule(&cbm_tasklet[cpu]);
	} else {
		ls_intr_handler(cpu);
		xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(4 + cpu, cbm_irncr), BIT(cpu) << 8);
		return IRQ_NONE;
	}
	return IRQ_HANDLED;
}

static irqreturn_t cbm_isr_5(int irq, void *dev_id)
{
	int cpu = smp_processor_id();
	uint32_t cbm_intr_status;

	cbm_intr_status = xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(5, cbm_irncr));
	if (cbm_intr_status & 0x0200) {
		/* Disable all interrupts */
		xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(5, cbm_irnen)), 0);
		/*Dummy Read, for write flush*/
		xrx500_cbm_r32((CBM_BASE + CBM_INT_LINE(5, cbm_irnen)));
		/* Disable  LS interrupt */
		xrx500_cbm_w32(CBM_LS_BASE + IRNEN_LS, xrx500_cbm_r32(CBM_LS_BASE + IRNEN_LS) & (~(3 << (cpu * 2 + 16))));
		xrx500_cbm_r32(CBM_LS_BASE + IRNEN_LS);
		/* Schedule the tasklet */
		tasklet_schedule(&cbm_tasklet[cpu]);
	#ifdef ENABLE_LL_DEBUG
	} else if (cbm_intr_status & 0x10000) {
		/* Disable all interrupts */
		xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(5, cbm_irnen)), 0);
		/*Dummy Read, for write flush*/
		xrx500_cbm_r32((CBM_BASE + CBM_INT_LINE(5, cbm_irnen)));
		/* Schedule the tasklet */
		tasklet_schedule(&cbm_debug_tasklet);
	#endif
	} else {
		ls_intr_handler(cpu);
		xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(4 + cpu, cbm_irncr), BIT(cpu) << 8);
		return IRQ_NONE;
	}
	return IRQ_HANDLED;
}

static irqreturn_t cbm_isr_6(int irq, void *dev_id)
{
	uint32_t cbm_intr_status = 0;
	int cpu = smp_processor_id();
	#ifdef CONFIG_CBM_LS_ENABLE
	uint32_t dqm_intr_status = 0;
	uint32_t pkt_recvd, pkt_recvd1;
	dqm_intr_status = xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(6, egp_irncr));

	if (dqm_intr_status) {
		xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(2, irncr)), 0x2);
		xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(6, egp_irncr), dqm_intr_status);
		LOGF_KLOG_DEBUG("schedule napi 0x%x\r\n", dqm_intr_status);
		pkt_recvd = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(2, desc0.desc3));
		pkt_recvd1 = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(2, desc1.desc3));
		if ((pkt_recvd & 0x80000000) || ((pkt_recvd1 & 0x80000000))) {
			napi_schedule(&g_cbm_ctrl.napi);
			xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(6, egp_irnen)), 0);
			/*Dummy Read, for write flush*/
			xrx500_cbm_r32((CBM_BASE + CBM_INT_LINE(6, egp_irnen)));
		}
	}
	#endif
	cbm_intr_status = xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(6, cbm_irncr));
	if (cbm_intr_status) {
		/* Disable all interrupts */
		xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(6, cbm_irnen)), 0);
		/*Dummy Read, for write flush*/
		xrx500_cbm_r32((CBM_BASE + CBM_INT_LINE(6, cbm_irnen)));
		/* Disable  LS interrupt */
		xrx500_cbm_w32(CBM_LS_BASE + IRNEN_LS, xrx500_cbm_r32(CBM_LS_BASE + IRNEN_LS) & (~(3 << (cpu * 2 + 16))));
		xrx500_cbm_r32(CBM_LS_BASE + IRNEN_LS);
		/* Schedule the tasklet */
		tasklet_schedule(&cbm_tasklet[cpu]);
#ifdef CONFIG_CBM_LS_ENABLE
	}
	if (!dqm_intr_status && !cbm_intr_status) {
		LOGF_KLOG_DEBUG("######spurious ls int###########\r\n");
		ls_intr_handler(cpu);
		xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(4 + cpu, cbm_irncr), BIT(cpu) << 8);
		return IRQ_NONE;
	}
#else
	} else {
		ls_intr_handler(cpu);
		xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(4 + cpu, cbm_irncr), BIT(cpu) << 8);
		return IRQ_NONE;
	}
#endif
	return IRQ_HANDLED;
}

static irqreturn_t cbm_isr_7(int irq, void *dev_id)
{
	int cpu = smp_processor_id();
	uint32_t cbm_intr_status;

	cbm_intr_status = xrx500_cbm_r32(CBM_BASE + CBM_INT_LINE(7, cbm_irncr));
	if (cbm_intr_status & 0x0800) {
		/* Disable all interrupts */
		xrx500_cbm_w32((CBM_BASE + CBM_INT_LINE(7, cbm_irnen)), 0);
		/*Dummy Read, for write flush*/
		xrx500_cbm_r32((CBM_BASE + CBM_INT_LINE(7, cbm_irnen)));
		/* Disable  LS interrupt */
		xrx500_cbm_w32(CBM_LS_BASE + IRNEN_LS, xrx500_cbm_r32(CBM_LS_BASE + IRNEN_LS) & (~(3 << (cpu * 2 + 16))));
		xrx500_cbm_r32(CBM_LS_BASE + IRNEN_LS);
		/* Schedule the tasklet */
		tasklet_schedule(&cbm_tasklet[cpu]);
	} else {
		ls_intr_handler(cpu);
		xrx500_cbm_w32(CBM_BASE + CBM_INT_LINE(4 + cpu, cbm_irncr), BIT(cpu) << 8);
		return IRQ_NONE;
	}
	return IRQ_HANDLED;
}


/*
 *	CBM Interrupt init
 *	Register to handle all the 8 lines , need to be adapted to work with MPE FW
 */
int cbm_interrupt_init(void)
{
	int ret = 0;
	struct cpumask cpumask;

	ret = request_irq(g_cbm_irq[0], cbm_isr_0, IRQF_DISABLED, "cbm_eqm", NULL);
	if (ret) {
		LOGF_KLOG_ERROR("Can not get IRQ - %d", ret);
		return ret;
	}

	if (g_cbm_irq[1]) {
		ret = request_irq(g_cbm_irq[1], cbm_isr_4, IRQF_DISABLED, "cbm_dqm", NULL);
		if (ret) {
			LOGF_KLOG_ERROR("Can not get IRQ - %d", ret);
			return ret;
		}
		/* Set the Affinity to VPE0 */
		cpumask.bits[0] = 0x1;
		ret = irq_set_affinity(g_cbm_irq[1], &cpumask);
		if (ret)
			LOGF_KLOG_ERROR("Can not set affinity for IRQ - %d", g_cbm_irq[1]);
	}

	if (g_cbm_irq[2]) {
		ret = request_irq(g_cbm_irq[2], cbm_isr_5, IRQF_DISABLED, "cbm_dqm", NULL);
		if (ret) {
			LOGF_KLOG_ERROR("Can not get IRQ - %d", ret);
			return ret;
		}
		/* Set the Affinity to VPE1 */
		#ifdef ENABLE_LL_DEBUG
		cpumask.bits[0] = 0x4;
		#else
		cpumask.bits[0] = 0x2;
		#endif
		ret = irq_set_affinity(g_cbm_irq[2], &cpumask);
		if (ret)
			LOGF_KLOG_ERROR("Can not set affinity for IRQ - %d", g_cbm_irq[2]);
	}

	if (g_cbm_irq[3]) {
		ret = request_irq(g_cbm_irq[3], cbm_isr_6, IRQF_DISABLED, "cbm_dqm", NULL);
		if (ret) {
			LOGF_KLOG_ERROR("Can not get IRQ - %d", ret);
			return ret;
		}
		/* Set the Affinity to VPE2 */
		cpumask.bits[0] = 0x4;
		ret = irq_set_affinity(g_cbm_irq[3], &cpumask);
		if (ret)
			LOGF_KLOG_ERROR("Can not set affinity for IRQ - %d", g_cbm_irq[3]);
	}
	if (g_cbm_irq[4]) {
		ret = request_irq(g_cbm_irq[4], cbm_isr_7, IRQF_DISABLED, "cbm_dqm", NULL);
		if (ret) {
			LOGF_KLOG_ERROR("Can not get IRQ - %d", ret);
			return ret;
		}
		/* Set the Affinity to VPE3 */
		cpumask.bits[0] = 0x8;
		ret = irq_set_affinity(g_cbm_irq[4], &cpumask);
		if (ret)
			LOGF_KLOG_ERROR("Can not set affinity for IRQ - %d", g_cbm_irq[4]);
	}

	return ret;
}



/* violation ptr start from 8K*2k offset for STD pointer
 *	and 0.5K * (8K/16K) offset for JBO pointer
 */
uint32_t get_violation_ptr(uint32_t fsqm_idx)
{
	static uint32_t delta;
	uint32_t buf_addr;
	const uint32_t std_base		= 8 * 1024 * 2	* 1024;		 /* (8k * 2k)*/
	const uint32_t jbo_base_8k	= 1 * 1024 * 8	* 1024 / 2;  /* (0.5k * 8k)*/
	const uint32_t jbo_base_16k = 1 * 1024 * 16 * 1024 / 2;  /* (0.5k * 16k)*/

	if (fsqm_idx == g_cbm_buff.std_fsqm_idx) {
		buf_addr = xrx500_cbm_r32(CBM_BASE + CBM_SBA_0);
		buf_addr += (delta * 2 * 1024) + std_base;
	} else {
		buf_addr = xrx500_cbm_r32(CBM_BASE + CBM_JBA_0);
	#if defined(CONFIG_BRONZE_RTL307)
		if (xrx500_cbm_r32(CBM_BASE + CBM_CTRL) & (1 << CBM_CTRL_JSEL_POS)) {/*(16k)*/
	#else
		if (xrx500_cbm_r32(CBM_BASE + CBM_STAT) & (1 << CBM_STAT_JSEL_POS)) {/*(16k)*/
	#endif
			buf_addr += (delta * 16 * 1024) + jbo_base_16k;
		} else {/*(8k)*/
			buf_addr += (delta * 8 * 1024) + jbo_base_8k;
		}
	}

	delta += 1;

	LOGF_KLOG_DEBUG("The illegal pointer addr: 0x%x\n", buf_addr);

	return buf_addr;
}

void fsqm_intr_ctrl(uint32_t idx, uint32_t val)
{
	xrx500_cbm_w32(FSQM_BASE(idx) + FSQM_IRNEN, val); /* 0x111101F;*/
}

void ls_intr_ctrl(uint32_t val)
{
	xrx500_cbm_w32(CBM_LS_BASE + IRNEN_LS, val); /* 0xFF0000;*/
}

void eqm_intr_ctrl(uint32_t val)
{
	int i;

	for (i = 0; i < CPU_EQM_PORT_NUM; i++) {
		xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(i, irnen), val);/* 0x3F;*/
	}
}

void dqm_intr_ctrl(uint32_t val)
{
	int i;
	for (i = 0; i < CPU_EQM_PORT_NUM; i++)
		xrx500_cbm_w32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(i, irnen), val); /* 0x02*/
}

void cbm_eqm_enable_bkpr(uint32_t pid, uint32_t enable)
{
	if (enable) {
		xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, cfg),
		xrx500_cbm_r32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, cfg))
		& (~(1 << 5)));
		xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, cfg),
		xrx500_cbm_r32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, cfg))
		& (~(1 << 5)));
	} else {
		xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, cfg),
		xrx500_cbm_r32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, cfg))
		| (1 << 5));
	}
}

int cbm_ptr_refcnt_incr(uint32_t pid, uint32_t buf_ptr)
{
	uint32_t i;

	if (pid >= CPU_EQM_PORT_NUM /*|| ((1 << pid) & g_cpu_port_alloc) == 0*/) {
		LOGF_KLOG_ERROR("illegal port id: %d\n", pid);
		return CBM_FAILURE;
	}

	if (!check_ptr_validation(buf_ptr)) {
		LOGF_KLOG_ERROR("buf is not valid: 0x%x\n", buf_ptr);
		return CBM_FAILURE;
	}

	i = 0;
	/*xrx500_cbm_w32(0xbf800020) = 0x1234; */
	xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, rcnt), buf_ptr);
	LOGF_KLOG_DEBUG("pid:%d, offset: 0x%x\n", pid, CBM_EQM_CPU_PORT(pid, rcnt));
	while (xrx500_cbm_r32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, rcnt))
	!= 0xFFFFF800 && (i++) < DEFAULT_WAIT_CYCLES) {
	};
	if (i >= DEFAULT_WAIT_CYCLES) {
		LOGF_KLOG_ERROR("Increase buf_ptr 0x%x refcnt on pid:\
		%d fail!!!\n", buf_ptr, pid);
	}

	return CBM_SUCCESS;
}

int cbm_clear_cpu_eqm_cnt(uint32_t pid)
{
	if (pid >= CPU_EQM_PORT_NUM)
		return CBM_FAILURE;

	xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, eqpc), 0);
	xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, dicc), 0);
	xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, rcntc), 0);
	xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, nsbpc), 0);
	xrx500_cbm_w32(CBM_EQM_BASE + CBM_EQM_CPU_PORT(pid, njbpc), 0);

	return CBM_SUCCESS;
}

static void store_rcnt_baseaddr()
{
	rcnt_data[0].fsqm_idx = g_cbm_buff.std_fsqm_idx;
	rcnt_data[0].frm_size = g_cbm_buff.std_frm_size;
	rcnt_data[0].rcnt_base = (uint32_t)FSQM_BASE(g_cbm_buff.std_fsqm_idx) + RCNT;
	rcnt_data[0].buf_addr = g_cbm_buff.std_buf_addr;
	rcnt_data[1].fsqm_idx = g_cbm_buff.jbo_fsqm_idx;
	rcnt_data[1].frm_size = g_cbm_buff.jbo_frm_size;
	rcnt_data[1].rcnt_base = (uint32_t)FSQM_BASE(g_cbm_buff.jbo_fsqm_idx) + RCNT;
	rcnt_data[1].buf_addr = g_cbm_buff.jbo_buf_addr;
}

int32_t get_cnt_of_ptr(uint32_t buf_addr)
{
	uint32_t buf_base, idx, arr_idx;
	uint32_t buf_type, fsqm_idx = 0;
	uint32_t buf_cnt = 0;

	buf_base = __pa(buf_addr);
	buf_type = check_ptr_valid_type(buf_addr);
	if ((buf_type != SBA0_STD) && (buf_type != SBA0_JBO))
		return -1;
	arr_idx = buf_type >> 1;
	fsqm_idx = rcnt_data[arr_idx].fsqm_idx;
	buf_base &= ~(rcnt_data[arr_idx].frm_size - 1);
	idx		 = (buf_addr - rcnt_data[arr_idx].buf_addr)/rcnt_data[arr_idx].frm_size;

	buf_cnt = xrx500_cbm_r32((void __iomem *)rcnt_data[arr_idx].rcnt_base + (idx * 4));
	LOGF_KLOG_DEBUG("Buf:[0x%x],PHY base:[0x%x] fsqm:%d, offset idx: %d,\
	refcnt:%d\n", buf_addr, buf_base, fsqm_idx, idx, buf_cnt);
	return buf_cnt;
}

int32_t cbm_buffer_upd_rcnt(int32_t vpe_id, void *buffer, uint32_t flags)
{
	uint32_t i;
	int count;
	if (vpe_id >= CPU_EQM_PORT_NUM /*|| ((1 << pid) & g_cpu_port_alloc) == 0*/) {
		LOGF_KLOG_ERROR("illegal port id: %d\n", vpe_id);
		return CBM_FAILURE;
	}

	count = get_cnt_of_ptr((uint32_t)buffer);
	LOGF_KLOG_DEBUG("Repeat count %d\r\n", count);
	if ((count == CBM_MAX_RCNT) || (count == -1) || (count == 0))
		return CBM_FAILURE;
	i = 0;
	xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_CPU_PORT(vpe_id, rcnt)),
	__pa(buffer));
	LOGF_KLOG_DEBUG("clone done\r\n");
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_buffer_upd_rcnt);

#ifdef CONFIG_LTQ_UMT_SW_MODE
static uint32_t cbm_ctrl_reg_shadow = 0;
#endif

inline void set_wlan_ep(int ep, int cbm_port)
{
	uint32_t* addr = 0;
#ifdef CONFIG_LTQ_UMT_SW_MODE
	addr = &cbm_ctrl_reg_shadow;
#else
	addr = CBM_BASE + CBM_CTRL;
#endif
	switch (cbm_port) {
	case 4:
		set_val(addr, ep, 0xF00000, 20);
		break;
	case 24:
		set_val(addr, ep, 0xF000, 12);
		break;
	case 25:
		set_val(addr, ep, 0xF00, 8);
		break;
	case 26:
		set_val(addr, ep, 0xF0, 4);
		break;
	default:
		break;
	}
}
inline void cbm_populate_entry(
struct cbm_pmac_port_map *local_entry,
int *phys_port,
uint32_t cbm_port,
uint32_t flags,
int *found)
{
	*phys_port = cbm_port;
	local_entry->egp_port_map |= BIT(cbm_port);
	local_entry->qids[local_entry->qid_num]
		= dqm_port_info[cbm_port].def_qid;
	local_entry->qid_num++;
	local_entry->egp_type = flags;
	*found = 1;
}

/*CBM Dequeue port reset.
This API expects that no traffic is enqueued for this port while calling this port reset API.
This API resets the Dequeue Port Current Desc Index and Buffer Return Register Current Index
(only applicable to CPU Dequeue Ports) to zero, and resets statistics for the port!*/
int32_t cbm_port_quick_reset(
int32_t cbm_port_id,
uint32_t flags)
{
	int result = cbm_port_id;
	int i = 0;
	uint32_t reg;
	uint32_t data_pointer = 0;
	uint32_t cfg;

	if (flags & CBM_PORT_F_DEQUEUE_PORT) {
		if ((cbm_port_id >= 5) && (cbm_port_id <= 22))
			result = 5;
		switch (result) {
		case 4:
		case 24:
		case 25:
		case 26:
			cfg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(result, cfg));
			rmb();
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(result, cfg)), 0);
			wmb();
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, dptr)), 0x1f);
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, resv2[0])), 0x0);
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, dqpc)), 0x0);
			wmb();
#ifdef CONFIG_LTQ_UMT_SW_MODE
			umt_reset_port_dq_idx(cbm_port_id);
#endif
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, bprc)), 0x0);
			wmb();
			for (i = 0; i < dqm_port_info[cbm_port_id].deq_info.num_desc; i++) {
				reg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, desc[i].desc0) + 12);
				rmb();
				if (reg & 0x80000000) {
					data_pointer = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, desc[i].desc0) + 8);
					cbm_buffer_free(smp_processor_id(), data_pointer, 1);
				}
				xrx500_cbm_w32(CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, desc[i].desc0) + 0, 0);
				xrx500_cbm_w32(CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, desc[i].desc0) + 4, 0);
				xrx500_cbm_w32(CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, desc[i].desc0) + 8, 0);
				xrx500_cbm_w32(CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, desc[i].desc0) + 12, 0);
			}
                        xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(result, cfg)), cfg);
			wmb();

		break;
		case 0:
		case 1:
		case 2:
		case 3:
			cfg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(result, cfg));
			rmb();
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(result, cfg)), 0);
			wmb();
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, dptr)), 0x1);
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, dqpc)), 0x0);
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, bprc)), 0x0);
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(result, cfg)), cfg);
			wmb();
		break;
		case 5:
			cfg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, cfg));
			rmb();
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, cfg)), 0);
			wmb();
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, dptr)), 0x1);
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, dqpc)), 0x0);
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, cfg)), cfg);
			wmb();
		break;
		case 23:
			cfg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(result, cfg));
			rmb();
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(result, cfg)), 0);
			wmb();
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, dptr)), 0x1f);
			xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, dqpc)), 0x0);
			xrx500_cbm_w32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(result, cfg), cfg);
			wmb();
		}
	} else {
		return CBM_FAILURE;
	}
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_port_quick_reset);

uint32_t 
cbm_get_dptr_scpu_egp_count (uint32_t cbm_port_id, uint32_t flags)
{
	if (cbm_port_id == 4 || cbm_port_id == 24 || cbm_port_id == 25 || cbm_port_id == 26)
	{
		uint32_t reg = 0;
		reg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, dptr));
		reg  = (reg >> 16) & 0x1F;
	 	return reg;	
	}
	else
		return 0;
	
}
EXPORT_SYMBOL(cbm_get_dptr_scpu_egp_count);

int32_t
cbm_dp_port_alloc(
	struct module *owner,
	struct net_device *dev,
	uint32_t dev_port,
	int32_t port_id,
/*	cbm_dp_callback_t *cbm_dp_cb,*/
	uint32_t flags
	)
{
	uint32_t port_start;
	uint32_t port_end;
	uint32_t temp_flags, exit = 0;
	int i, result, phys_port = 0, phys_port_1 = 0;
	int param_pmac = 0, pmac_present = 0, pmac_found = 0;
	uint32_t cbm_port[CBM_MAX_PHY_PORT_PER_EP],
	pmac[CBM_MAX_PHY_PORT_PER_EP];
	struct cbm_pmac_port_map *test_ptr = NULL;
	struct cbm_pmac_port_map local_entry = {0};
	struct cbm_pmac_port_map *dp_local_entry = NULL;
	uint32_t num;
	struct cbm_qidt_elm qidt_set = {0};
	struct cbm_qidt_mask qidt_mask = {1, 1, 1, 1, 1, 1, 1, 1};
	cbm_tmu_res_t *res = NULL;
	if (flags & DP_F_DEREGISTER) {	/*De-register */
		if (port_id >= PMAC_MAX_NUM || port_id < 0)
			return DP_FAILURE;
		cbm_delete_from_list(port_id, flags);
		return DP_SUCCESS;
	}
	pr_info("flags 0x%x\n", flags);
	/*to allocate port*/
	if (flags & DP_F_FAST_ETH_LAN) {
		port_start = PMAC_ETH_LAN_START_ID;
		port_end = PMAC_ETH_LAN_END_ID;
	} else if (flags & DP_F_FAST_ETH_WAN) {
		port_start = PMAC_ETH_WAN_ID;
		port_end = PMAC_ETH_WAN_ID;
	} else {
		port_start = PMAC_ALLOC_START_ID;
		port_end = PMAC_ALLOC_END_ID;
	}
	/*alloc port */
	if (port_id) {	/*allocate with specified port id */
		if ((port_id > port_end) || (port_id < port_start))
			return DP_FAILURE;

		if (is_dp_allocated(port_id, flags)) {
			LOGF_KLOG_ERROR("Failed to alloc a pmac port %d\
			since it is allocated already\n", port_id);
			return DP_FAILURE; /*not free*/
		}
		param_pmac = port_id;
		pmac_present = 1;
		goto ALLOC_OK;
	} else { /* dynamic alloc a free port */
		for (i = port_start; i <= port_end; i++) {
			if (flags == DP_F_CHECKSUM) {
				dp_local_entry = is_dp_allocated(i, 0);
			} else {
				dp_local_entry = is_dp_allocated(i, flags);
			}
			if (!dp_local_entry) {
				port_id = i;
				goto ALLOC_OK;
			}
		}
	}
	LOGF_KLOG_ERROR("Failed to get a free port for module %p\n", owner);
	return DP_FAILURE;
ALLOC_OK:
	result = get_matching_pmac(pmac, flags, cbm_port, param_pmac, pmac_present);
	if (result == CBM_FAILURE) {
		return DP_FAILURE;
	} else if (result == CBM_NOTFOUND) {
		/*Physical port allocation failure, means this is just virtual pmac required*/
	} else {
		if ((pmac[0] != CBM_PMAC_DYNAMIC) && (pmac[0] != port_id)) {
			LOGF_KLOG_ERROR("%s Error in the mapping table\r\n", __func__);
			return DP_FAILURE;
		}
		local_entry.pmac = port_id;
		local_entry.owner = owner;
		local_entry.dev = dev;
		local_entry.dev_port = dev_port;
		local_entry.flags = P_ALLOCATED;
		for (i = 0; i < CBM_MAX_PHY_PORT_PER_EP; i++) {
			if (pmac[i] != CBM_PORT_INVALID) {
				temp_flags = (flags & DP_F_FAST_WLAN) ? DP_F_FAST_WLAN : flags;
				temp_flags = (flags & DP_F_DSL_BONDING) ? DP_F_DSL_BONDING : temp_flags;
				switch (temp_flags) {
				case DP_F_FAST_WLAN:
					if (cbm_port[i] != CBM_PORT_NOT_APPL) {
						test_ptr = is_cbm_allocated(cbm_port[i], flags);
						if (!test_ptr) {
							cbm_populate_entry(&local_entry, &phys_port, cbm_port[i], flags, &pmac_found);
							exit = 1;
						}
					}
				break;
				case DP_F_DSL_BONDING:
					if (cbm_port[i] != CBM_PORT_NOT_APPL) {
						test_ptr = NULL;
						test_ptr = is_cbm_allocated(cbm_port[i], flags);
						if (!test_ptr) {
							if (i == (CBM_MAX_PHY_PORT_PER_EP - 1)) {
								LOGF_KLOG_ERROR("%s Error allocating 2 ports for DSL BONDING\r\n", __func__);
								return DP_FAILURE;
							}
							cbm_populate_entry(&local_entry, &phys_port, cbm_port[i], flags, &pmac_found);
							test_ptr = NULL;
							test_ptr = is_cbm_allocated(cbm_port[i+1], flags);
							if (!test_ptr) {
								cbm_populate_entry(&local_entry, &phys_port_1, cbm_port[i+1], flags, &pmac_found);
								exit = 1;
							} else {
								return DP_FAILURE;
							}
						}
					}
				break;
				case DP_F_CHECKSUM:
					if (cbm_port[i] != CBM_PORT_NOT_APPL) {
						test_ptr = NULL;
						test_ptr = is_cbm_allocated(cbm_port[i], flags);
						if (test_ptr) {
							cbm_delete_from_list(CBM_PMAC_NOT_APPL, flags);
							cbm_populate_entry(&local_entry, &phys_port, cbm_port[i], flags, &pmac_found);
							exit = 1;
						}
					}
				break;
				case DP_F_PORT_TUNNEL_DECAP:
					pr_info("0x%x 0x%x\n", port_id, flags);
					local_entry.egp_type = flags;
					cbm_add_to_list(&local_entry);
					return port_id;
				break;
				case DP_F_DIRECT:
					if (cbm_port[i] != CBM_PORT_NOT_APPL) {
						pr_info("0x%x 0x%x\n", cbm_port[i], flags);
						cbm_populate_entry(&local_entry, &phys_port, cbm_port[i], flags, &pmac_found);
						cbm_add_to_list(&local_entry);
						return port_id;
					}
				break;
				default:
					if (cbm_port[i] != CBM_PORT_NOT_APPL) {
						pr_info("0x%x 0x%x\n", cbm_port[i], flags);
						cbm_populate_entry(&local_entry, &phys_port, cbm_port[i], flags, &pmac_found);
					}
				break;
				}
				if (exit)
					break;
			}
		}
		if (!pmac_found)
			return DP_FAILURE;
	}


	cbm_add_to_list(&local_entry);

	if (flags & DP_F_FAST_WLAN) {
		/*special mapping for direct path*/
		set_wlan_ep(port_id, phys_port);
		if (flags & DP_F_DSL_BONDING) {
			set_wlan_ep(0, phys_port_1);
		}
	}
	if ((cbm_dp_port_resources_get(&port_id, &num, &res,
						  flags) != 0) && (!res)) {

			LOGF_KLOG_ERROR("Error getting resources for port_id %d\n", port_id);
			return CBM_FAILURE;
	}
	if (num && res) {
		pr_info("disabling q %d port %d\n", res[0].tmu_q, res[0].tmu_port);
		tmu_equeue_enable(res[0].tmu_q, 0);
		tmu_egress_port_enable(res[0].tmu_port, 0);
		qidt_set.ep = port_id;
		qidt_mask.ep_mask = 0;
		if ((!cbm_qtable[res[0].tmu_q].refcnt)) {
			LOGF_KLOG_INFO("first time LOOKUP prgrm\n");
			cbm_qidt_set(&qidt_set,
			&qidt_mask,
			res[0].tmu_q);
	}
	
	LOGF_KLOG_INFO("%d\n", cbm_qtable[res[0].tmu_q].refcnt);
		kfree(res);
	}
	return port_id;
}
EXPORT_SYMBOL(cbm_dp_port_alloc);
int32_t
cbm_dp_port_dealloc(
	struct module *owner,
	uint32_t dev_port,
	int32_t cbm_port_id,
	uint32_t flags
	)
{
	cbm_delete_from_list(cbm_port_id, flags);
	return CBM_SUCCESS;
}

/**
*  Return CBM Port ID of the given WLAN EP
*  input:
*   @ep_id:   ep_id (7-14) allocated from datapath library.
*  output:
*   @cbm_pid:  (0 - 3)
* return:  Fail < 0,  Success: 0
 */
int cbm_get_wlan_umt_pid(u32 ep_id,  u32 *cbm_pid)
{
	uint32_t reg;
#ifdef CONFIG_LTQ_UMT_SW_MODE
	reg = cbm_ctrl_reg_shadow;
#else
	reg = xrx500_cbm_r32(CBM_BASE + CBM_CTRL);
	LOGF_KLOG_INFO("0x%x 0x%x 0x%x 0x%x %d\r\n",
	((reg & 0xF00000) >> 20),
	((reg & 0xF000) >> 12),
	((reg & 0xF00) >> 8),
	((reg & 0xF0) >> 4),
	ep_id);
#endif

	if (((reg & 0xF00000) >> 20) == ep_id) {
		*cbm_pid = 0;
		return CBM_SUCCESS;
	}
	if (((reg & 0xF000) >> 12) == ep_id) {
		*cbm_pid = 1;
		return CBM_SUCCESS;
	}
	if (((reg & 0xF00) >> 8) == ep_id) {
		*cbm_pid = 2;
		return CBM_SUCCESS;
	}
	if (((reg & 0xF0) >> 4) == ep_id) {
		*cbm_pid = 3;
		return CBM_SUCCESS;
	}
	return CBM_FAILURE;
}
EXPORT_SYMBOL(cbm_get_wlan_umt_pid);

int32_t direct_dp_enable(uint32_t port_id, uint32_t flags, uint32_t type)
{
	int j, queue_number;
	struct cbm_qidt_elm qidt_set = {0};
	struct cbm_qidt_mask qidt_mask = {1, 1, 1, 1, 1, 1, 1, 1};
	struct cbm_pmac_port_map *local_entry = NULL;
	qidt_set.ep = port_id;
	qidt_mask.ep_mask = 0;
	if (type != DP_F_FAST_DSL) {
		local_entry = is_dp_allocated(CBM_PMAC_NOT_APPL, DP_F_DIRECTPATH_RX);
		if (local_entry) {
			for (j = 0; j < local_entry->qid_num; j++) {
				if (flags & CBM_PORT_F_DISABLE)
					queue_number = 255;
				else
					queue_number = local_entry->qids[j];
				qidt_set.flowidl = 0;
				qidt_mask.flowid_lmask = 0;
				qidt_set.flowidh = 0;
				qidt_mask.flowid_hmask = 0;
				qidt_set.mpe2 = 0;
				qidt_mask.mpe2_mask = 0;
				qidt_set.mpe1 = 0;
				qidt_mask.mpe1_mask = 0;
				qidt_set.dec = 1;
				qidt_mask.dec_mask = 0;
				qidt_set.enc = 1;
				qidt_mask.enc_mask = 0;
				cbm_qidt_set(&qidt_set,
				&qidt_mask,
				queue_number);
			}
		}
	}

	local_entry = is_dp_allocated(CBM_PMAC_NOT_APPL, DP_F_CHECKSUM);
	if (local_entry) {
		for (j = 0; j < local_entry->qid_num; j++) {
			if (flags & CBM_PORT_F_DISABLE)
				queue_number = 255;
			else
				queue_number = local_entry->qids[j];
			qidt_set.flowidl = 0;
			qidt_mask.flowid_lmask = 0;
			qidt_set.flowidh = 0;
			qidt_mask.flowid_lmask = 0;
			qidt_set.mpe2 = 1;
			qidt_mask.mpe2_mask = 0;
			qidt_mask.mpe1_mask = 1;
			qidt_set.dec = 1;
			qidt_mask.dec_mask = 0;
			qidt_set.enc = 1;
			qidt_mask.enc_mask = 0;
			cbm_qidt_set(&qidt_set,
			&qidt_mask,
			queue_number);
		}
	}
	return CBM_SUCCESS;
}
#ifdef CONFIG_LTQ_DP_ACA_CSUM_WORKAROUND
int32_t checksum_dp_enable(uint32_t port_id, uint32_t flags, uint32_t type)
{
	int j, queue_number;
	struct cbm_qidt_elm qidt_set = {0};
	struct cbm_qidt_mask qidt_mask = {1, 1, 1, 1, 1, 1, 1, 1};
	struct cbm_pmac_port_map *local_entry = NULL;
	qidt_set.ep = port_id;
	qidt_mask.ep_mask = 0;
	local_entry = is_dp_allocated(port_id, DP_F_CHECKSUM);
	if (local_entry) {
		for (j = 0; j < local_entry->qid_num; j++) {
			if (flags & CBM_PORT_F_DISABLE)
				queue_number = 255;
			else
				queue_number = local_entry->qids[j];
			qidt_set.flowidl = 0;
			qidt_mask.flowid_lmask = 0;
			qidt_set.flowidh = 0;
			qidt_mask.flowid_lmask = 0;
			qidt_set.mpe2 = 1;
			qidt_mask.mpe2_mask = 0;
			qidt_mask.mpe1_mask = 1;
			qidt_set.dec = 1;
			qidt_mask.dec_mask = 0;
			qidt_set.enc = 1;
			qidt_mask.enc_mask = 0;
			cbm_qidt_set(&qidt_set,
			&qidt_mask,
			queue_number);
		}
	}
	return CBM_SUCCESS;
}
#endif
void cbm_restore_qmap(int enable, int qid)
{
	uint8_t *ptr, *ptr1;
	uint32_t i, found = 0, count = 0;
	uint32_t tmp_drop_flag;
	for (i = 0; i < 0x1000; i++) {
		tmp_drop_flag = g_cbm_qidt_mirror[i].qidt_drop_flag;
		ptr = (uint8_t *)&g_cbm_qidt_mirror[i].qidt_drop_flag;
		ptr1 = (uint8_t *)&g_cbm_qidt_mirror[i].qidt_shadow;

		if (enable) {
			if (ptr[3] && (ptr1[3] == qid)) {
				cbm_qtable[ptr1[3]].refcnt++;
				found++;
				count++;
				tmp_drop_flag &= 0x01010100;
			}
			if (ptr[2] && (ptr1[2] == qid)) {
				cbm_qtable[ptr1[2]].refcnt++;
				found++;
				count++;
				tmp_drop_flag &= 0x01010001;
			}
			if (ptr[1]&& (ptr1[1] == qid)) {
				cbm_qtable[ptr1[1]].refcnt++;
				found++;
				count++;
				tmp_drop_flag &= 0x01000101;
			}
			if (ptr[0] && (ptr1[0] == qid)) {
				cbm_qtable[ptr1[0]].refcnt++;
				found++;
				count++;
				tmp_drop_flag &= 0x00010101;
			}
		} else {
			if (ptr[3] || ptr[2] || ptr[1] || ptr[0])
				found = 1;
			if (ptr[3]) {
				count++;
			}
			if (ptr[2]) {
				count++;
			}
			if (ptr[1]) {
				count++;
			}
			if (ptr[0]) {
				count++;
			}
		}
		if (found) {
			g_cbm_qidt_mirror[i].qidt_drop_flag = tmp_drop_flag;
			if (enable)
				xrx500_cbm_w32((CBM_QIDT_BASE + i * 4), g_cbm_qidt_mirror[i].qidt_shadow);
		}
		found = 0;
	}
	LOGF_KLOG_DEBUG("%s:count %d\n", __func__, count);
}

int32_t
cbm_dp_enable(
	struct module *owner,
	uint32_t port_id,
	uint32_t flags,
	uint32_t alloc_flags
	)
{
	int j;
	struct cbm_qidt_elm qidt_set = {0};
	struct cbm_qidt_mask qidt_mask = {1, 1, 1, 1, 1, 1, 1, 1};
	struct cbm_pmac_port_map *local_entry = NULL;
	uint32_t num;
	cbm_tmu_res_t *res = NULL;
	int q_buff_num;
	unsigned long sys_flag;
	qidt_set.ep = port_id;
	qidt_mask.ep_mask = 0;

	if ((cbm_dp_port_resources_get(&port_id, &num, &res,
					  alloc_flags) != 0) && (!res)) {
		LOGF_KLOG_ERROR("Error getting resources for port_id %d\n", port_id);
		return CBM_FAILURE;
	}
	LOGF_KLOG_INFO("******************ep=%d tmu_port=%d queue=%d sid=%d\n", port_id,
				   res[0].tmu_port,
				   res[0].tmu_q,
				   res[0].tmu_sched);
	local_entry = is_dp_allocated(port_id, alloc_flags);
	spin_lock_irqsave(&cbm_qidt_lock, sys_flag);
	if (local_entry && ((local_entry->egp_type == DP_F_DIRECT)
	|| (local_entry->egp_type == DP_F_FAST_DSL)
	|| (local_entry->egp_type == DP_F_DIRECTLINK))) {
		/*cpu 0 ,*/
		pr_info("egp_type 0x%x\n", local_entry->egp_type);
		cbm_qidt_set(&qidt_set,
		&qidt_mask,
		(flags & CBM_PORT_F_DISABLE) ? 255 : res[0].tmu_q);
		/*Hard coded 2 physical ports mapped,
		 port 20, port 21
		*/
		/*enable the queue for direcpath rx and checksum ports when a directpath port is enabled*/
		direct_dp_enable(port_id, flags, local_entry->egp_type);
		if((local_entry->egp_type == DP_F_DIRECT) && (flags & CBM_PORT_F_DISABLE))
			goto UNLOCK;
	}
	#ifdef CONFIG_LTQ_DP_ACA_CSUM_WORKAROUND
	else if (local_entry && (local_entry->egp_type == DP_F_CHECKSUM))	{
			checksum_dp_enable(port_id, flags, local_entry->egp_type);
	}
	#endif
	/* For each queue_id in the CBM Queue map table matched entries (by EP), disable/enable the TMU queue by calling
	the API of the TMU driver*/

	tmu_queue_list(res[0].tmu_port, tmp_q_buff3, ARRAY_SIZE(tmp_q_buff3), &q_buff_num, NULL);
	for (j = 0; j < q_buff_num; j++) {
		tmu_equeue_enable(tmp_q_buff3[j], (flags & CBM_PORT_F_DISABLE) ? 0 : 1);
		LOGF_KLOG_INFO("enable queue %d\n", tmp_q_buff3[j]);
		LOGF_KLOG_INFO("flag %d refcnt %d\n", ((flags & CBM_PORT_F_DISABLE) ? 0 : 1), cbm_qtable[tmp_q_buff3[j]].refcnt);
	}
	/*Enable the TMU port, i*/
	tmu_egress_port_enable(res[0].tmu_port, (flags & CBM_PORT_F_DISABLE) ? 0 : 1);
	UNLOCK:
	if (res)
		kfree(res);
	spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_dp_enable);


#define CHECK_BIT(var, pos) ((var) & (1<<(pos)))
/*cbm_qid_schedule_out:
Input: TMU port id, tmu queue id
output:void
This API disables/enables the scheduling of packets from queues other than tmu_queue_id(disassociation of queues).
Expects the sequence of disable before enable*/
void cbm_qid_schedule_out(int32_t cbm_port_id, int32_t tmu_queue_id, int32_t enable)
{
	struct tmu_equeue_link qlink;
	static int q_buff_num;
	int	j;
	/*LOGF_KLOG_INFO("enable %d\n", enable);*/
	if (!enable) {
		tmu_queue_list(cbm_port_id, &queue_flush_buff.qid[0], EGRESS_QUEUE_ID_MAX, &q_buff_num, NULL);
		/*LOGF_KLOG_INFO("qbuff_num %d\n", q_buff_num);*/
		for (j = 0; j < q_buff_num; j++) {
			if (queue_flush_buff.qid[j] == tmu_queue_id) {
				/*tmu_equeue_link_get(queue_flush_buff.qid[j], &qlink);
				udelay(20000);
				LOGF_KLOG_INFO("queue def %d sbit def %d\r\n",  queue_flush_buff.qid[j], qlink.sbin);*/
				continue;
			} else {
				#ifdef DISABLE_OTHER_QUEUE
				/*Disable the queue*/
				tmu_equeue_enable(queue_flush_buff.qid[j], 0);
				#else
				tmu_equeue_link_get(queue_flush_buff.qid[j], &qlink);
				/*Disable the scheduler block input*/
				tmu_sched_blk_in_enable(qlink.sbin, enable);
				queue_flush_buff.qlink_sbin[j] = qlink.sbin;
				#endif
				/*LOGF_KLOG_INFO("queue %d sbit %d\r\n",  queue_flush_buff.qid[j], qlink.sbin);*/
				udelay(10000);
			}
		}
	} else {
		for (j = 0; j < q_buff_num; j++) {
			if (queue_flush_buff.qid[j] == tmu_queue_id)
				continue;
			else {
				#ifdef DISABLE_OTHER_QUEUE
				/*Enable the queue*/
				tmu_equeue_enable(queue_flush_buff.qid[j], 1);
				#else
				/*Disable the scheduler block input*/
				tmu_sched_blk_in_enable(queue_flush_buff.qlink_sbin[j] , enable);
				#endif
				udelay(10000);
				/*LOGF_KLOG_INFO("enable back queue %d sbit %d\r\n", queue_flush_buff.qid[j], queue_flush_buff.qlink_sbin[j]);*/
			}
		}
	}
	/*tmu_equeue_link_get(tmu_queue_id, &qlink);
	pr_info("sched blk in status %d\n",tmu_is_sched_blk_in_enabled(qlink.sbin));*/
}

static int32_t cbm_empty_queue(int32_t cbm_port_id, uint32_t qocc, uint32_t port_type)
{
	int i;
	int no_packet = 0;
	uint32_t reg = 0;
	#ifdef CHECK_WHILE_LOOP
	int iter = 0;
	#endif
	i = 0;
	while (qocc) {
		#ifdef CHECK_WHILE_LOOP
		iter++;
		if (iter > 30000) {
			pr_info_once("%s >30k iter qocc %d", __func__, qocc);
		} else if (iter > 10000) {
			pr_info_once("%s >10k iter qocc %d", __func__, qocc);
		} else if (iter > 5000) {			
			pr_info_once("%s >5k iter qocc %d", __func__, qocc);
		} else if (iter > 500) {			
			pr_info_once("%s >500 iter qocc %d", __func__, qocc);
		}
		#endif

		
		/*pr_info("read_dptr_idx=%d num_desc=%d\n", i, dqm_port_info[cbm_port_id].deq_info.num_desc);*/
		for (i = 0; i < dqm_port_info[cbm_port_id].deq_info.num_desc; ) {
			/*pr_info("read %d\n", i);*/
			switch(port_type) {
			case DQM_DMA_TYPE:
				reg = xrx500_cbm_r32(CBM_DMADESC_BASE + CBM_DQM_DMA_DESC(cbm_port_id, i) + 0x8);
				break;
			default:
				LOGF_KLOG_ERROR("Unknown port type\n");			
			}
			//LOGF_KLOG_ERROR("0x%x 0x%x %d\n", CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, desc[i].desc0) + 12, reg, qocc);
			//LOGF_KLOG_ERROR("0x%x %d\n", xrx500_cbm_r32(CBM_DMADESC_BASE + CBM_DQM_DMA_DESC(cbm_port_id, i) + 0xc), cbm_port_id);
			rmb();
			if (reg & 0x80000000) {
				switch(port_type) {
				case DQM_DMA_TYPE:
					xrx500_cbm_w32((CBM_DMADESC_BASE + CBM_DQM_DMA_DESC(cbm_port_id, i) + 0x8), (reg & 0x7fffffff) | 0x40000000);
					break;
				default:
					LOGF_KLOG_ERROR("Unknown port type\n");
				}
				wmb();
				no_packet = 0;
				qocc--;
				i++;
				i %= dqm_port_info[cbm_port_id].deq_info.num_desc;
			} else {
				no_packet++;
				i++;
				i %= dqm_port_info[cbm_port_id].deq_info.num_desc;
				//LOGF_KLOG_ERROR("NP 0x%x 0x%x %d %d\n", CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, desc[i].desc0) + 12, reg, qocc, i);
				if(!i)
					goto EXIT;
			}
			//usleep_range(2000, 2500);
			
		}
		EXIT:
		//usleep_range(2000, 2500);
		if ((qocc <= 33) && no_packet) {
			/*pr_info("inter qocc %d\n", qocc);*/
			return qocc;
		}
	}
	return 0;
}

static int32_t cbm_wave_empty(int32_t cbm_port_id, int32_t tmu_queue_id)
{
	uint32_t wq, qrth, qocc_temp, qavg;
	int count = 0, pkt_deq = 0;
	int dqpc = 0, i;
	uint32_t reg = 0;
	uint32_t data_pointer = 0;

	#ifdef CHECK_WHILE_LOOP
	int iter = 0;
	#endif
	tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
	dqpc = xrx500_cbm_r32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, dqpc)));
	if ((qocc_temp == 0) || (dqpc != 0x20)) {
		pr_info("qocc is %d, dont flush desc fifo %d\n", qocc_temp, dqpc);
		return CBM_FAILURE;
	}
	if ((qocc_temp != 0) && (dqpc == 0x20)) {
		//pr_info("sane fifo status\n");
		//pr_info("Disable Backpressure\n");
		xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, cfg)), 0x107 | ((cbm_port_id << CFG_CPU_EGP_0_EPMAP_POS) & CFG_CPU_EGP_0_EPMAP_MASK));
		count = qocc_temp + dqpc;
		while (pkt_deq < count) {
			#ifdef CHECK_WHILE_LOOP
			iter++;
			if (iter > 30000) {
				pr_info_once("%s >30k iter qocc %d", __func__, count);
			} else if (iter > 10000) {
				pr_info_once("%s >10k iter qocc %d", __func__, count);
			} else if (iter > 5000) {			
				pr_info_once("%s >5k iter qocc %d", __func__, count);
			} else if (iter > 500) {			
				pr_info_once("%s >500 iter qocc %d", __func__, count);
			}
			#endif
			for (i = 0; i < dqm_port_info[cbm_port_id].deq_info.num_desc;) {
				reg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, desc[i].desc0) + 12);
				rmb();
				if (reg & 0x80000000) {
					//pr_info("own set %d\n", i);
					data_pointer = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, desc[i].desc0) + 8);
					xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_SCPU_PORT(cbm_port_id, scpu_ptr_rtn[i])), data_pointer);
					wmb();
					pkt_deq++;
					i++;
				}
				if (pkt_deq >= count)
					break;
			}
			//usleep_range(2000, 2500);
		}
		//pr_info("Reenable Backpressure\n");
		xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, cfg)), 0x10F | ((cbm_port_id << CFG_CPU_EGP_0_EPMAP_POS) & CFG_CPU_EGP_0_EPMAP_MASK));
	}
	return CBM_SUCCESS;
}
static int32_t cbm_queue_empty_process(uint32_t port_type, int32_t cbm_port_id, int32_t tmu_queue_id)
{
	uint32_t wq, qrth, qocc_temp, qavg;
	int result = 0;
	int no_packet = 0;
	#ifdef CHECK_WHILE_LOOP
	int iter = 0;
	#endif
	tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
	qocc_temp += 33;
	while (1) {
		#ifdef CHECK_WHILE_LOOP
		iter++;
		if (iter > 30000) {
			tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
			pr_info_once("%s >30k iter qocc %d", __func__, qocc_temp);
		} else if (iter > 10000) {
			tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
			pr_info_once("%s >10k iter qocc %d", __func__, qocc_temp);
		} else if (iter > 5000) {
			tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
			pr_info_once("%s >5k iter qocc %d", __func__, qocc_temp);
		} else if (iter > 500) {
			tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
			pr_info_once("%s >500 iter qocc %d", __func__, qocc_temp);
		}
		#endif
		//pr_info("inter1 qocc %d\n", qocc_temp);
		result = cbm_empty_queue(cbm_port_id, qocc_temp, port_type);
		if (!result) {
			no_packet = 0;
			tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
			/*LOGF_KLOG_ERROR("QOCC %d\n", qocc_temp);*/
			if (qocc_temp && (qocc_temp != 32767)) {
				continue;
			} else {
				return result;
			}
		} else {
			no_packet++;
			/*pr_info("no packet count %d\n", no_packet);*/
			qocc_temp = result;
			if (no_packet > 10) {
				tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
				return result;
			}
		}
	}
	return result;
}
int32_t cbm_queue_flush(
int32_t cbm_port_id,
int32_t tmu_queue_id,
uint32_t timeout,
uint32_t flags)
{
	uint32_t port_type = find_dqm_port_flush(cbm_port_id);
	uint32_t reg, reg1;
	#ifdef CONFIG_CBM_LS_ENABLE
	int32_t pos = 3;
	int count1 = 0;
	#endif
	#ifdef CHECK_WHILE_LOOP
	int iter = 0;
	#endif
	int chan;
	uint32_t wq, qrth, qocc, qavg;
	uint32_t qocc_temp;
	cbm_debug_1("%s\r\n", __func__);
	if ((port_type == DQM_DMA_TYPE) || (port_type == DQM_LDMA_TYPE) || (port_type == DQM_WAVE_TYPE)) {
		chan = dqm_port_info[cbm_port_id].dma_ch;
		if (tmu_queue_id != -1) {
			qocc = 0;
			cbm_qid_schedule_out(cbm_port_id, tmu_queue_id, 0);
			
			/* Disable the DMA channels associated with CBM Dequeue Port */
			if (chan)
				ltq_dma_chan_off(chan);
		}
		tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
		/*LOGF_KLOG_INFO("QOCC %d\n", qocc_temp);*/
		if ((port_type == DQM_LDMA_TYPE) || (port_type == DQM_WAVE_TYPE)) {
			if (flags & CBM_Q_F_FORCE_FLUSH) {
				/*LOGF_KLOG_INFO("Force flush\n");*/
				if (port_type == DQM_WAVE_TYPE) {
					cbm_wave_empty(cbm_port_id, tmu_queue_id);
				} else {
					cbm_queue_empty_process(port_type, cbm_port_id, tmu_queue_id);
				}
			} else {
				/*LOGF_KLOG_INFO("Auto flush\n");*/
				while (qocc_temp) {
					#ifdef CHECK_WHILE_LOOP
					iter++;
					if (iter > 30000) {
						tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
						pr_info_once("%s >30k iter qocc %d", __func__, qocc_temp);
					} else if (iter > 10000) {
						tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
						pr_info_once("%s >10k iter qocc %d", __func__, qocc_temp);
					} else if (iter > 5000) {
						tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
						pr_info_once("%s >5k iter qocc %d", __func__, qocc_temp);
					} else if (iter > 500) {
						tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
						pr_info_once("%s >500 iter qocc %d", __func__, qocc_temp);
					}
					#endif
					usleep_range(10000, 10100);
					tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
					/*LOGF_KLOG_INFO("----->qocc_temp =%d Q %d\n", qocc_temp, tmu_queue_id);*/
				}
				usleep_range(10000, 10100);
			}
		} else {
			cbm_queue_empty_process(port_type, cbm_port_id, tmu_queue_id);
		}	
		if (tmu_queue_id != -1) {
			tmu_qoct_read(tmu_queue_id, &wq, &qrth, &qocc_temp, &qavg);
				LOGF_KLOG_ERROR("out QOCC %d\n", qocc_temp);
				/*LOGF_KLOG_INFO("enq %d deq %d qid %d\r\n", get_enq_counter(tmu_queue_id), get_deq_counter(tmu_queue_id), tmu_queue_id);
				tmu_egress_port_seg_occupancy_get(cbm_port_id, &epoc[0]);
				LOGF_KLOG_INFO(" epoc0 %u epoc1 %u epoc2 %u epoc3 %u\r\n", epoc[0], epoc[1], epoc[2], epoc[3]);*/

			cbm_qid_schedule_out(cbm_port_id, tmu_queue_id, 1);
		}
		if (chan)
			ltq_dma_chan_on(chan);
	} else if ((port_type == DQM_CPU_TYPE)) {
		if (tmu_queue_id != -1) {
		/*Disassociate other queues and stop scheduling them*/
			cbm_qid_schedule_out(cbm_port_id, tmu_queue_id, 0);
		}
		#ifdef CONFIG_CBM_LS_ENABLE
			if (!cbm_port_id) {
			/*port 0, so need to read all the LS ports involved in spreading*/
				while (pos >= 0) {
					if ((1 << pos) & g_cpu_port_alloc) {
						/*LS port*/
						DEQ_PKTS:
						while (((xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pos, status)) >> 7) & 0xF) != 0) {
							/*return the cbm buffer to CBM pool*/
							cbm_buffer_free(smp_processor_id(), xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pos, desc.desc2)), 1);
							reg = xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(pos, desc.desc3));
						}
						reg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, desc0.desc0) + 12);
						reg1 = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, desc1.desc0) + 12);
						if ((reg & 0x80000000) || (reg1 & 0x80000000)) {
							udelay(10);
							goto DEQ_PKTS;
						}
					}
					pos--;
				}
			} else {
				while (!(count0 && count1)) {
					count0 = count1 = 0;
					reg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, desc0.desc0) + 12);
					if (!(reg & 0x80000000)) {
						count0++;
					} else {
						cbm_buffer_free(smp_processor_id(), xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, desc0.desc0) + 8), 1);
						xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, desc0.desc0) + 12), 0);
					}
					reg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, desc1.desc0) + 12);
					if (!(reg & 0x80000000)) {
						count1++;
					} else {
						cbm_buffer_free(smp_processor_id(), xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, desc1.desc0) + 8), 1);
						xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, desc1.desc0) + 12), 0);
					}
				}
			}
		#else
			/*Loop until LS port packet count becomes 0*/
			LS_DEQ_PKTS:
			while (((xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(cbm_port_id, status)) >> 7) & 0xF) != 0) {
				/*return the cbm buffer to CBM pool*/
				cbm_buffer_free(smp_processor_id(), xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(cbm_port_id, desc.desc2)), 1);
				/*read the desc3 to clear*/
				reg = xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(cbm_port_id, desc.desc3));
				cbm_debug("ls queue len %d", ((xrx500_cbm_r32(CBM_LS_BASE + CBM_LS_PORT(cbm_port_id, status)) >> 7) & 0xF));
			}
			reg = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, desc0.desc0) + 12);
			reg1 = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CPU_PORT(cbm_port_id, desc1.desc0) + 12);
			if ((reg & 0x80000000) || (reg1 & 0x80000000)) {
				udelay(10);
				goto LS_DEQ_PKTS;
			}
		#endif
		/*Reassociate other queues whose scheduling were stopped*/
		if (tmu_queue_id != -1) {
			cbm_qid_schedule_out(cbm_port_id, tmu_queue_id, 1);
		}
	} else {
		LOGF_KLOG_ERROR("Unsupported port type\n");
	}
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_queue_flush);

int32_t cbm_queue_sched_mgmt(int32_t tmu_port, uint32_t qid, uint32_t flags)
{
	struct tmu_equeue_link qlink;

	/*tmu_equeue_link_get(qid, &qlink);
	pr_info("QUEUE status %d\n", tmu_is_equeue_enabled(qid));
	pr_info("Sched blk out status %d\n", tmu_is_sched_blk_out_enabled(qlink.sbin >> 3));*/
	if (flags & CBM_Q_F_DISABLE) {
		if (flags & CBM_Q_F_FLUSH) {
			cbm_queue_flush(tmu_port, qid, 0, flags);
		} else if (!(flags & CBM_Q_F_NODEQUEUE)) {
			LOGF_KLOG_ERROR("Queue %d is not empty after timeout\n", qid);
			return CBM_FAILURE;
		}
	}
	tmu_equeue_link_get(qid, &qlink);
	/*Disable the scheduler block input*/
	tmu_equeue_enable(qid, (flags & CBM_Q_F_DISABLE) ? 0 : 1);
	/*LOGF_KLOG_INFO("qid %d status %d\n", qid, (flags & CBM_Q_F_DISABLE) ? 0 : 1);*/
	if (!(flags & CBM_Q_F_DISABLE))
		tmu_sched_blk_out_enable(qlink.sbin >> 3, 1);
	return CBM_SUCCESS;
}

int32_t cbm_dp_q_enable (
int32_t dp_port_id,
int32_t qnum,
int32_t tmu_port_id,
int32_t remap_to_qid,
uint32_t timeout,
int32_t qidt_valid,
/*struct cbm_queue_map_entry_t *qidt_set_cfg,*/
uint32_t flags)
{
	uint32_t op_type = 0;
	int j;
	struct cbm_qidt_elm qidt_set = {0};
	struct cbm_qidt_mask qidt_mask = {1, 1, 1, 1, 1, 1, 1, 1};
	struct cbm_pmac_port_map *local_entry = NULL;
	uint32_t num;
	cbm_tmu_res_t *res = NULL;
	int q_buff_num, result = CBM_SUCCESS;
	unsigned long sys_flag;
	int32_t num_entries = 0, i = 0, tmu_port = -1;
	struct tmu_equeue_link qlink;
	cbm_queue_map_entry_t *q_map_get = NULL;
	qidt_set.ep = dp_port_id;
	qidt_mask.ep_mask = 0;


	if (dp_port_id >= 0) {
		op_type = 1;
		tmu_port = tmu_port_id;
	}
	if (qnum >= 0) {
		op_type = 2;
		tmu_equeue_link_get(qnum, &qlink);
		tmu_port = qlink.epn;
	}
	if (flags & CBM_Q_F_CKSUM)
		op_type = 3;
	cbm_debug("optype %d\n", op_type);
	if (cbm_dp_port_resources_get(&dp_port_id, &num, &res,
						(op_type == 3) ? DP_F_CHECKSUM : 0) != 0) {
		LOGF_KLOG_ERROR("Error getting resources for port_id %d\n", dp_port_id);
		return CBM_FAILURE;
	}

	switch (op_type) {
	case 1:
		/*LOGF_KLOG_INFO("******************ep=%d tmu_port=%d queue=%d\n", dp_port_id,
					tmu_port,
					qnum);*/
		tmu_queue_list(tmu_port, tmp_q_buff2, ARRAY_SIZE(tmp_q_buff2), &q_buff_num, NULL);
		LOGF_KLOG_INFO(" q_buff_num: %d\n", q_buff_num);
		if (flags & CBM_Q_F_RESTORE_ONLY) {
			#if 1
			/*restore all entries in the queue map table for that
			EP to map to original queues, and remove the CBM_F_QUEUE_DROP flag from the DDR table*/
			spin_lock_irqsave(&cbm_qidt_lock, sys_flag);
			for (j = 0; j < q_buff_num; j++) {
				cbm_restore_qmap(1, tmp_q_buff2[j]);
				/*LOGF_KLOG_INFO(" j: %d\n", j);*/
			}
			spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);
			#endif
		}
		

		/*spin_lock_irqsave(&cbm_qidt_lock, sys_flag);*/
		/*For the Datapath port (EP i), go through all entries in the CBM Queue Map table in DDR, and mark with
		CBM_F_QUEUE_DROP flag. For each such entry, program the CBM Queue Map table entry in CBM to map
		to the Global Drop Queue*/
		if (flags & CBM_Q_F_DISABLE) {
			spin_lock_irqsave(&cbm_qidt_lock, sys_flag);
			cbm_qidt_set(&qidt_set,
						&qidt_mask,
						255);
			spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);
			/*LOGF_KLOG_INFO("global drop queue\n");*/
			//if (!(flags & CBM_Q_F_NODEQUEUE))
				//usleep_range(timeout, timeout+100);
		}
		/*spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);*/
		/* For each queue_id in the CBM Queue map table matched entries (by EP), disable/enable the TMU queue by calling
		the API of the TMU driver*/
		
		for (j = 0; j < q_buff_num; j++) {
			cbm_queue_sched_mgmt(tmu_port, tmp_q_buff2[j], flags);
			/*LOGF_KLOG_INFO(" j: %d\n", j);*/
		}
		/*restore all entries in the queue map table for that
		EP to map to original queues, and remove the CBM_F_QUEUE_DROP flag from the DDR table*/
		spin_lock_irqsave(&cbm_qidt_lock, sys_flag);
		for (j = 0; j < q_buff_num; j++) {
			(flags & CBM_Q_F_DISABLE) ?  : cbm_restore_qmap(1, tmp_q_buff2[j]);
			/*LOGF_KLOG_INFO(" j: %d\n", j);*/
		}
		spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);
	break;
	case 3:
		local_entry = is_dp_allocated(CBM_PMAC_NOT_APPL, DP_F_CHECKSUM);
		if (!local_entry) {
			LOGF_KLOG_ERROR("ret failure\n");
			kfree(res);
			return CBM_FAILURE;
		}
		qnum = local_entry->qids[0];
		tmu_port = res[0].tmu_port;
	case 2:
		cbm_debug("case 2 %d sbin %d\n", qnum, qlink.sbin);

		/*For the Datapath port (EP i), go through all entries in the CBM Queue Map table in DDR, and mark with
		CBM_F_QUEUE_DROP flag. For each such entry, program the CBM Queue Map table entry in CBM to map
		to the Global Drop Queue*/
		/*cbm_queue_map_get_hw(qnum, &num_entries);*/
		/*pr_info("map get hw %d %d\n", num_entries, qnum);*/
		num_entries = 0;
		cbm_queue_map_get(
				qnum,
				&num_entries,
				&q_map_get, 0);
		/*pr_info("map get %d %d %d\n", num_entries, remap_to_qid, (flags & CBM_Q_F_DISABLE)? 0 : 1);*/
		if (num_entries > 0) {
			for (i = 0; i < num_entries; i++) {
				cbm_queue_map_set((remap_to_qid == -1) ? 255 : remap_to_qid, &q_map_get[i], 0);
			}
			cbm_debug("map set\n");
		}

		//if (!(flags & CBM_Q_F_NODEQUEUE))
			//usleep_range(timeout, timeout+100);
		/* For each queue_id in the CBM Queue map table matched entries (by EP), disable/enable the TMU queue by calling
		the API of the TMU driver*/
		pr_info("cbm_queue_sched_mgmt\n");
		if (cbm_queue_sched_mgmt(tmu_port, qnum, flags) == CBM_FAILURE) {
			LOGF_KLOG_ERROR("ret failure\n");
			result = CBM_FAILURE;
			goto DQ_FAIL;
		}
		/*restore all entries in the queue map table for that
		EP to map to original queues, and remove the CBM_F_QUEUE_DROP flag from the DDR table*/
		if ((num_entries > 0) && (remap_to_qid == -1) && (!(flags & CBM_Q_F_DISABLE))) {
			/*cbm_restore_qmap((flags & CBM_Q_F_DISABLE) ? 0 : 1);*/
			LOGF_KLOG_INFO("%s***********FIX ME***********\n", __func__);
		}

		if (qidt_valid == 0) {
			if ((!cbm_qtable[qnum].refcnt) && !(flags & CBM_Q_F_DISABLE)) {
				pr_info("guess first time qidt_valid==0\n");
				cbm_qidt_set(&qidt_set,
				&qidt_mask,
				qnum);
			}
		}
		/*pr_info("%d\n", cbm_qtable[qnum].refcnt);*/
	break;
	default:
	break;
	}
	DQ_FAIL:
	if (res)
		kfree(res);
	if (q_map_get)
		kfree(q_map_get);
	return result;
}
EXPORT_SYMBOL(cbm_dp_q_enable);

static int get_is_bit_set(uint32_t flags)
{
	int i;

	for (i = 0; i <= 31; i++) {
		if ((flags & (1 << i)) != 0)
			return i;
	}
	return -1;
}

int32_t cbm_enqueue_port_resources_get (
cbm_eq_port_res_t *res,
uint32_t flags)
{
	int count = 0, i, j;
	switch (flags) {
	case DP_F_ENQ_GSWIPL:
	case DP_F_ENQ_PAE:
	case DP_F_ENQ_TSO:
	case DP_F_ENQ_VRX318:
	case DP_F_ENQ_CPU_0:
	case DP_F_ENQ_CPU_1:
	case DP_F_ENQ_CPU_2:
	case DP_F_ENQ_CPU_3:
	case DP_F_ENQ_WAVE:
	case DP_F_ENQ_DL:
	break;
	default:
		LOGF_KLOG_ERROR("%s: Invalid flag for the port\r\n", __func__);
		return CBM_FAILURE;
	}
	for (i = 0; i < CBM_EQM_PORT_MAX; i++) {
		if (eqm_port_info[i].port_type == flags) {
			count++;
		}
	}
	if (count) {
		res->num_eq_ports = count;
		res->eq_info = (cbm_eq_info_t *)kmalloc(sizeof(cbm_eq_info_t) * (res->num_eq_ports), GFP_ATOMIC);
		if (res->eq_info) {
			j = 0;
			for (i = 0; i < CBM_EQM_PORT_MAX; i++) {
				if (eqm_port_info[i].port_type == flags) {
					res->eq_info[j].port_no = i;
					res->eq_info[j].cbm_eq_port_base
					= eqm_port_info[i].eq_info.
					cbm_eq_port_base;
					res->eq_info[j].dma_rx_chan_std
					= eqm_port_info[i].eq_info.dma_rx_chan_std;
					if (eqm_port_info[i].num_eq_ports > 1) {
						res->eq_info[j].dma_rx_chan_jumbo
						= eqm_port_info[i].eq_info.dma_rx_chan_jumbo;
					}
					res->eq_info[j].num_desc
					= eqm_port_info[i].eq_info.num_desc;
					j++;
				}

				if (j > res->num_eq_ports)
					BUG_ON("exceeds the number of resources\r\n");
			}
		}
	}
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_enqueue_port_resources_get);

int32_t
cbm_dequeue_port_resources_get(
	uint32_t	dp_port,
	cbm_dq_port_res_t *res,
	uint32_t flags
	)
{
	int i = 0;
	uint32_t port_map, port;
	struct cbm_pmac_port_map *local_entry;

	local_entry = is_dp_allocated(dp_port, flags);
	if (local_entry) {
		res->num_deq_ports = hweight_long(local_entry->egp_port_map);
		res->deq_info = (cbm_dq_info_t *)kmalloc(sizeof(cbm_dq_info_t) * (res->num_deq_ports), GFP_ATOMIC);
		if (res->deq_info) {
			port_map = local_entry->egp_port_map;
			port = get_is_bit_set(port_map);
			res->cbm_buf_free_base
			= dqm_port_info[port]
			.cbm_buf_free_base;
			res->num_free_entries
			= dqm_port_info[port]
			.num_free_entries;

			for (i = 0; i < res->num_deq_ports; i++) {
				port = get_is_bit_set(port_map);
				res->deq_info[i].cbm_dq_port_base
				= dqm_port_info[port]
				.deq_info.cbm_dq_port_base;
				res->deq_info[i].num_desc
				= dqm_port_info[port]
				.deq_info.num_desc;
				res->deq_info[i].dma_tx_chan
				= dqm_port_info[port]
				.deq_info.dma_tx_chan;
				res->deq_info[i].port_no = port;
				/*clear the flag for the current bitpos*/
				port_map &= ~(1 << port);
			}
		} else {
			LOGF_KLOG_ERROR("%s error in allocating memory", __func__);
		}
	} else {
		LOGF_KLOG_ERROR("%s: unallocated pmac port\r\n", __func__);
		return -1;
	}
	return 0;
}
EXPORT_SYMBOL(cbm_dequeue_port_resources_get);

int32_t cbm_dp_port_resources_get (
	uint32_t *dp_port,
	uint32_t *num_tmu_ports,
	cbm_tmu_res_t **res_pp,
	uint32_t flags)
{
	int i = 0;
	uint32_t port_map;
	cbm_tmu_res_t *res;
	struct cbm_pmac_port_map *local_entry = NULL;
	uint32_t cbm_port[CBM_MAX_PHY_PORT_PER_EP];
	uint32_t local_flags = 0, local_num_ports = 0, result;

	LOGF_KLOG_DEBUG("%s: flags 0x%x dp %d\r\n", __func__, flags, *dp_port);
	if (dp_port) {
		LOGF_KLOG_DEBUG("%s: dp %d\r\n", __func__, *dp_port);
		local_entry = is_dp_allocated(*dp_port, flags);
	} else {
		local_entry = is_dp_allocated(0, flags);
	}
	if (local_entry) {
		*num_tmu_ports = hweight_long(local_entry->egp_port_map);
		if ((*num_tmu_ports > 16) || (*num_tmu_ports == 0))
			return -1;
		res = (cbm_tmu_res_t *)kmalloc(sizeof(cbm_tmu_res_t) * (*num_tmu_ports), GFP_ATOMIC);
		if (res) {
			*res_pp = res;
			port_map = local_entry->egp_port_map;
			LOGF_KLOG_DEBUG("port_map %d\r\n", port_map);
			for (i = 0; i < *num_tmu_ports; i++) {
				res[i].tmu_port = get_is_bit_set(port_map);
				res[i].cbm_deq_port = get_is_bit_set(port_map);
				LOGF_KLOG_DEBUG("%d tmu_port\r\n", res[i].tmu_port);
				/*clear the flag for the current bitpos*/
				port_map &= ~(1 << res[i].tmu_port);
				res[i].tmu_q = local_entry->qids[i];
				LOGF_KLOG_DEBUG("%d tmu_q\r\n", res[i].tmu_q);
				res[i].tmu_sched = res[i].tmu_q - SBID_START;
			}
		} else {
			LOGF_KLOG_ERROR("%s error in allocating memory", __func__);
		}
	} else if ((!local_entry) && ((flags & DP_F_MPE_ACCEL) || (flags & DP_F_DIRECTPATH_RX) || (flags & DP_F_CHECKSUM))) {
		return -1;
	} else if (dp_port && ((*dp_port <= 6) || (*dp_port == 15))) {
		/*check if it is static pmac*/
			result = get_matching_pmac_noflags(cbm_port, *dp_port, &local_flags, &local_num_ports);
			if (result == CBM_SUCCESS) {
				res = (cbm_tmu_res_t *)kmalloc(sizeof(cbm_tmu_res_t) * (local_num_ports), GFP_ATOMIC);
				if (res) {
					*res_pp = res;
					LOGF_KLOG_DEBUG("local_num_ports %d\r\n", local_num_ports);
					*num_tmu_ports = local_num_ports;
					for (i = 0; i < local_num_ports; i++) {
						LOGF_KLOG_DEBUG("cbm_port %d\r\n", cbm_port[i]);
						res[i].tmu_port = cbm_port[i];
						res[i].cbm_deq_port = cbm_port[i];
						/*clear the flag for the current bitpos*/
						res[i].tmu_q = dqm_port_info[cbm_port[i]]
									.def_qid;
						LOGF_KLOG_DEBUG("%d tmu_\r\n", res[i].tmu_q);
						res[i].tmu_sched = res[i].tmu_q - SBID_START;
					}
				} else {
					LOGF_KLOG_ERROR("%s: kmalloc failed in getting static pmac port\r\n", __func__);
				}
			} else {
			LOGF_KLOG_ERROR("%s: Error in getting static pmac port\r\n", __func__);
			return -1;
		}
	} else {
		/*LOGF_KLOG_ERROR("%s: unallocated pmac port\r\n", __func__);*/
		return -1;
	}
	return 0;
}
EXPORT_SYMBOL(cbm_dp_port_resources_get);

int32_t cbm_reserved_dp_resources_get(
	uint32_t *tmu_port,
	uint32_t *tmu_sched,
	uint32_t *tmu_q)
{
	cbm_tmu_res_t tmu_res;
	reserved_ports_highest(&tmu_res, 0);
	*tmu_port = tmu_res.tmu_port;
	*tmu_sched = tmu_res.tmu_sched;
	*tmu_q = tmu_res.tmu_q;
	return 0;
}
EXPORT_SYMBOL(cbm_reserved_dp_resources_get);

int32_t cbm_get_egress_port_info(uint32_t cbm_port, uint32_t *tx_ch,
	uint32_t *flags)
{
	if (cbm_port < ARRAY_SIZE(dqm_port_info)) {
		if (tx_ch)
			*tx_ch = dqm_port_info[cbm_port].deq_info.dma_tx_chan;
		if (flags)
			*flags = dqm_port_info[cbm_port].egp_type;
	} else {
		if (tx_ch)
			*tx_ch = 0;
		if (flags)
			*flags = 0;

	}
	return 0;
}
EXPORT_SYMBOL(cbm_get_egress_port_info);

int32_t cbm_enqueue_mgr_ctrl_get(
	cbm_eqm_ctrl_t *ctrl, /*!< EQM Control data structure */
	uint32_t flags /*!< Flags */
	)
{
	ctrl->regval = xrx500_cbm_r32(CBM_BASE + CBM_CTRL);
	return CBM_SUCCESS;
}

int32_t
cbm_enqueue_mgr_ctrl_set(
	cbm_eqm_ctrl_t *ctrl, /*!< EQM Control data structure */
	uint32_t flags /*!< Flags */
	)
{
	xrx500_cbm_w32(CBM_BASE + CBM_CTRL, ctrl->regval);
	asm("sync");
	return CBM_SUCCESS;
}

int32_t
cbm_dequeue_mgr_ctrl_get(
	cbm_dqm_ctrl_t *ctrl, /*!< EQM Control data structure */
	uint32_t flags /*!< Flags */
	)
{
	ctrl->regval = xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CTRL);
	return CBM_SUCCESS;
}

int32_t
cbm_dequeue_mgr_ctrl_set(
	cbm_dqm_ctrl_t *ctrl, /*!< EQM Control data structure */
	uint32_t flags /*!< Flags */
	)
{
	xrx500_cbm_w32(CBM_DQM_BASE + CBM_DQM_CTRL, ctrl->regval);
	asm("sync");
	return CBM_SUCCESS;
}

#define DMA_DQM_START_PORT 6
#define DMA_EQM_START_PORT 5

static int get_controller(char *ctrl)
{
	if (!strcmp(ctrl, "DMA1TX"))
		return DMA1TX;
	else if (!strcmp(ctrl, "DMA1RX"))
		return DMA1RX;
	else if (!strcmp(ctrl, "DMA2TX"))
		return DMA2TX;
	else if (!strcmp(ctrl, "DMA2RX"))
		return DMA2RX;
	else
		return DMAMAX;
}

static int32_t setup_DMA_channel(int chan, int cbm_port, int type, int buf_type)
{
	LOGF_KLOG_DEBUG("port %d type %d buf_type %d\r\n",
	cbm_port, type, buf_type);
	if ((ltq_request_dma(chan, "test")) < 0) {
		LOGF_KLOG_ERROR(" %s failed to open chan for chan%d\r\n",
		__func__, chan);
		return CBM_FAILURE;
	}
	if (type & DQM_DMA) {
		/* For CBM */
		if ((ltq_dma_chan_desc_cfg(chan,
		RPHYSADDR(
		(uint32_t)(CBM_DMADESC_BASE + CBM_DQM_DMA_DESC(cbm_port
		, 0))),
		2)) < 0) {
			LOGF_KLOG_ERROR(" %s failed to setup chan\
			desc for chan%d\r\n", __func__, chan);
			return CBM_FAILURE;
		}
	} else {
		LOGF_KLOG_DEBUG("EQM DMA reg 0x%x\r\n",
		(unsigned int)(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(cbm_port,
		0, (buf_type >> 1))));
		if ((ltq_dma_chan_desc_cfg(chan,
		RPHYSADDR(
		(uint32_t)(CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(cbm_port,
		0,
		(buf_type >> 1)))), 2)) < 0) {
			LOGF_KLOG_ERROR(" %s failed to setup chan desc for chan%d\r\n", __func__, chan);
		}
	}
	if (ltq_dma_chan_irq_disable(chan) < 0) {
		LOGF_KLOG_ERROR(" %s failed to disable IRQ for chan%d\r\n",
		__func__, chan);
		return CBM_FAILURE;
	}
	/* Channel on */
	if (ltq_dma_chan_on(chan) < 0) {
		LOGF_KLOG_ERROR(" %s failed to ON chan%d\r\n", __func__, chan);
		return CBM_FAILURE;
	}
	LOGF_KLOG_DEBUG("%s executed\n", __func__);
	return CBM_SUCCESS;
}


int32_t
cbm_enqueue_dma_port_init(
	int32_t cbm_port_id,
	int32_t dma_hw_num,
	uint32_t std_dma_chan_num,
	uint32_t jumbo_dma_chan_num,
	uint32_t flags
	)
{
	int dma_controller, chan;
	unsigned char dma_ctrl[16];
	if (flags & CBM_PORT_F_STANDARD_BUF) {
		/*config the dma channel*/
		sprintf(dma_ctrl, "DMA%dRX", dma_hw_num);
		dma_controller = get_controller(dma_ctrl);
		chan = _DMA_C(dma_controller, 0, std_dma_chan_num);
		eqm_port_info[cbm_port_id].eq_info.dma_rx_chan_std = chan;
		if (setup_DMA_channel(chan,
		cbm_port_id,
		EQM_DMA,
		CBM_PORT_F_STANDARD_BUF))
			return CBM_FAILURE;
	}
	if (flags & CBM_PORT_F_JUMBO_BUF) {
		/*config the dma channel*/
		sprintf(dma_ctrl, "DMA%dRX", dma_hw_num);
		dma_controller = get_controller(dma_ctrl);
		chan = _DMA_C(dma_controller, 0, jumbo_dma_chan_num);
		eqm_port_info[cbm_port_id].eq_info.dma_rx_chan_jumbo = chan;
		if (setup_DMA_channel(chan,
		cbm_port_id,
		EQM_DMA,
		CBM_PORT_F_JUMBO_BUF))
			return CBM_FAILURE;
	}
	return CBM_SUCCESS;
}

int32_t
cbm_enqueue_dma_port_uninit(
	int32_t cbm_port_id,
	uint32_t flags
	)
{
	int chan;
	chan = dqm_port_info[cbm_port_id].dma_ch;
	if (ltq_dma_chan_off(chan))
		return CBM_FAILURE;

	if (ltq_free_dma(chan))
		return CBM_FAILURE;
	return CBM_SUCCESS;
}

int32_t
cbm_dequeue_dma_port_init(
	int32_t cbm_port_id,
	int32_t dma_hw_num,
	uint32_t dma_chan_num,
	uint32_t flags
	)
{
	int32_t ret = CBM_SUCCESS;
	int dma_controller, chan;
	unsigned char dma_ctrl[16];
	/*config the dma channel*/
	sprintf(dma_ctrl, "DMA%dTX", dma_hw_num);
	dma_controller = get_controller(dma_ctrl);
	LOGF_KLOG_DEBUG("dma %d \r\n", dma_controller);
	chan = _DMA_C(dma_controller, 0, dma_chan_num);
	LOGF_KLOG_DEBUG("chan %d \r\n", chan);
	dqm_port_info[cbm_port_id].dma_ch = chan;
	if (setup_DMA_channel(chan,
	cbm_port_id,
	DQM_DMA,
	CBM_PORT_F_STANDARD_BUF) < 0)
		ret = CBM_FAILURE;
	return ret;
}


int32_t
cbm_dequeue_dma_port_uninit(
	int32_t cbm_port_id,
	uint32_t flags
	)
{
	int chan;
	chan = dqm_port_info[cbm_port_id].dma_ch;
	if (ltq_dma_chan_off(chan))
		return CBM_FAILURE;
	if (ltq_free_dma(chan))
		return CBM_FAILURE;
	return CBM_SUCCESS;
}

int32_t
cbm_dma_port_enable(
	int32_t cbm_port_id,
	uint32_t flags
	)
{
	if (flags & CBM_PORT_F_DEQUEUE_PORT) {
		/*DMA dequeue port*/
		if (flags & CBM_PORT_F_DISABLE)
			xrx500_cbm_w32((CBM_DQM_BASE +
			CBM_DQM_DMA_PORT(cbm_port_id - 6, cfg)),
			0x0);
		else
			init_cbm_dqm_dma_port(cbm_port_id,
			cbm_port_id - 6);
	} else {
		/*DMA Enqueue port*/
		if (flags & CBM_PORT_F_DISABLE)
			xrx500_cbm_w32((CBM_EQM_BASE
			+ CBM_EQM_DMA_PORT(cbm_port_id - 5, cfg)),
			0x0);
		else
			init_cbm_eqm_dma_port(cbm_port_id - 5, flags);
	}
	return CBM_SUCCESS;
}

int32_t
cbm_dma_toe_port_enable(
	int32_t cbm_port_id,
	int flags)
{
		/*DMA Enqueue port*/
	if (flags & CBM_PORT_F_DISABLE)
		xrx500_cbm_w32((CBM_EQM_BASE
		+ CBM_EQM_DMA_PORT(cbm_port_id - 5, cfg)), 0x0);
	else
		init_cbm_eqm_dma_toe_port(cbm_port_id - 5, flags);
	return CBM_SUCCESS;
}

int32_t
cbm_dma_vrx318_port_enable(int32_t cbm_port_id, int flags)
{
	/*DMA Enqueue port*/
	if (flags & CBM_PORT_F_DISABLE)
		xrx500_cbm_w32((CBM_EQM_BASE
		+ CBM_EQM_DMA_PORT(cbm_port_id - 5, cfg)), 0x0);
	else
		init_cbm_eqm_ldma_port(flags);
	return CBM_SUCCESS;
}


int32_t
cbm_dma_port_config_get(
	int32_t cbm_port_id,
	cbm_dma_port_config_t *cfg,
	uint32_t flags
	)
{
	if (flags == CBM_PORT_F_DEQUEUE_PORT)
		cfg->deq_cfg.regval =  xrx500_cbm_r32(CBM_DQM_BASE
		+ CBM_DQM_DMA_PORT(cbm_port_id - DMA_DQM_START_PORT,
		cfg));
	else
		cfg->enq_cfg.regval = xrx500_cbm_r32(CBM_EQM_BASE
		+ CBM_EQM_DMA_PORT(cbm_port_id - DMA_EQM_START_PORT,
		cfg));
	return CBM_SUCCESS;
}

int32_t
cbm_dma_port_config_set(
	int32_t cbm_port_id,
	cbm_dma_port_config_t *cfg,
	uint32_t flags
	)
{
	if (flags == CBM_PORT_F_DEQUEUE_PORT)
		xrx500_cbm_w32(CBM_DQM_BASE
		+ CBM_DQM_DMA_PORT(cbm_port_id - DMA_DQM_START_PORT, cfg),
		cfg->deq_cfg.regval);
	else
		xrx500_cbm_w32(CBM_EQM_BASE
		+ CBM_EQM_DMA_PORT(cbm_port_id - DMA_EQM_START_PORT, cfg),
		cfg->enq_cfg.regval);
	return CBM_SUCCESS;
}
int32_t
cbm_enqueue_port_overhead_set(
int32_t port_id,
int8_t ovh
)
{
	BUG_ON(!((port_id >= 0) && (port_id <= 15)));
	if (cbm_rev <= CBM_SOC_REV_1) {
		set_val(CBM_EQM_BASE
		+ CBM_EQM_CPU_PORT(port_id, cfg),
		ovh,
		CFG_CPU_IGP_0_OVH_MASK,
		CFG_CPU_IGP_0_OVH_POS);
	} else {
		xrx500_cbm_w32(CBM_EQM_BASE + OVH_0 + (4 * port_id), ovh);
	}
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_enqueue_port_overhead_set);

int32_t
cbm_enqueue_port_overhead_get(
int32_t port_id,
int8_t *ovh
)
{
	BUG_ON(!((port_id >= 0) && (port_id <= 15)));
	if (cbm_rev <= CBM_SOC_REV_1) {
		*ovh = (xrx500_cbm_r32(CBM_EQM_BASE
		+ CBM_EQM_CPU_PORT(port_id, cfg)) & CFG_CPU_IGP_0_OVH_MASK) >> CFG_CPU_IGP_0_OVH_POS;
	} else {
		*ovh = xrx500_cbm_r32(CBM_EQM_BASE + OVH_0 + (4 * port_id)) & 0xff;
	}
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_enqueue_port_overhead_get);

int32_t
cbm_igp_delay_set(
	int32_t cbm_port_id,
	int32_t delay
	)
{

	BUG_ON(!((cbm_port_id >= 0) && (cbm_port_id <= 15)));

	if (cbm_rev > CBM_SOC_REV_1) {
		xrx500_cbm_w32(CBM_EQM_BASE
		+ CBM_EQM_CPU_PORT(cbm_port_id, dcntr)
		, delay & 0x3f);
	}
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_igp_delay_set);

int32_t
cbm_igp_delay_get(
	int32_t cbm_port_id,
	int32_t *delay
	)
{
	BUG_ON(!((cbm_port_id >= 0) && (cbm_port_id <= 15)));
	if (cbm_rev > CBM_SOC_REV_1) {
		*delay = xrx500_cbm_r32(CBM_EQM_BASE
		+ CBM_EQM_CPU_PORT(cbm_port_id, dcntr)
		) & 0x3f;
	}
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_igp_delay_get);
int32_t
cbm_queue_delay_enable_set(
	int32_t enable,
	int32_t queue
	)
{
	uint32_t regval, i;
	BUG_ON(!((queue >= 0) && (queue <= 256)));
#if 0
	if (ltq_get_soc_rev() != CBM_SOC_REV_1) {
		regval = xrx500_cbm_r32(CBM_EQM_BASE
		+ DEN_0 + ((queue >> 5) * 4));
		if (enable & CBM_QUEUE_DELAY_ENABLE)
			regval |= BIT(queue & 0x1F);
		else
			regval &= ~BIT(queue & 0x1F);
		xrx500_cbm_w32(CBM_EQM_BASE
		+ DEN_0 + ((queue >> 5) * 4), regval);
	}
#endif
	regval = xrx500_cbm_r32(CBM_EQM_BASE + CBM_EQM_CTRL);
	regval |= BIT(8) | BIT(6);
	regval |= 3 << CBM_EQM_CTRL_L_POS;
	xrx500_cbm_w32(CBM_EQM_BASE
		+ CBM_EQM_CTRL, regval);
	for (i = 0; i <= 15; i++) {
		cbm_igp_delay_set(i, CBM_EQM_DELAY_ENQ);
	}
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_queue_delay_enable_set);

int32_t
cbm_enqueue_port_thresh_get(
	int32_t cbm_port_id,
	cbm_port_thresh_t *thresh,
	uint32_t flags
	)
{
	WARN_ON(!((cbm_port_id >= 0) && (cbm_port_id <= 15)));
	if ((cbm_port_id >= 0) && (cbm_port_id <= 15)) {
		thresh->regval = xrx500_cbm_r32(CBM_EQM_BASE
		+ CBM_EQM_CPU_PORT(cbm_port_id, wm));
		return CBM_SUCCESS;
	}
	return CBM_FAILURE;
}
EXPORT_SYMBOL(cbm_enqueue_port_thresh_get);

int32_t cbm_enqueue_port_thresh_set(
	int32_t cbm_port_id,
	cbm_port_thresh_t *thresh,
	uint32_t flags)
{
	WARN_ON(!((cbm_port_id >= 0) && (cbm_port_id <= 15)));
	if ((cbm_port_id >= 0) && (cbm_port_id <= 15)) {
		 xrx500_cbm_w32(CBM_EQM_BASE
		+ CBM_EQM_CPU_PORT(cbm_port_id,
		wm), thresh->regval);
		return CBM_SUCCESS;
	}
	return CBM_FAILURE;
}
EXPORT_SYMBOL(cbm_enqueue_port_thresh_set);

int32_t cbm_dequeue_cpu_port_stats_get(
	int32_t cbm_port_id,
	uint32_t *deq_ctr,
	uint32_t flags)
{
	WARN_ON(!((cbm_port_id >= 0) && (cbm_port_id <= 4)));
	if ((cbm_port_id >= 0) && (cbm_port_id <= 4)) {
		*deq_ctr = xrx500_cbm_r32(CBM_DQM_BASE
		+ CBM_DQM_CPU_PORT(cbm_port_id,
		dqpc));
		return CBM_SUCCESS;
	}
	return CBM_FAILURE;
}
EXPORT_SYMBOL(cbm_dequeue_cpu_port_stats_get);

int32_t cbm_enqueue_cpu_port_stats_get(
	int32_t cbm_port_id,
	uint32_t *occupancy_ctr,
	uint32_t *enq_ctr,
	uint32_t flags)
{
	WARN_ON(!((cbm_port_id >= 0) && (cbm_port_id <= 4)));
	if ((cbm_port_id >= 0) && (cbm_port_id <= 4)) {
		*enq_ctr = xrx500_cbm_r32(CBM_EQM_BASE
		+ CBM_EQM_CPU_PORT(cbm_port_id, eqpc));
		*occupancy_ctr = xrx500_cbm_r32(CBM_EQM_BASE
		+ CBM_EQM_CPU_PORT(cbm_port_id,
		pocc));
		return CBM_SUCCESS;
	}
	return CBM_FAILURE;
}
EXPORT_SYMBOL(cbm_enqueue_cpu_port_stats_get);

int32_t cbm_dequeue_dma_port_stats_get(
	int32_t cbm_port_id,
	uint32_t *deq_ctr,
	uint32_t flags)
{
	WARN_ON(!((cbm_port_id >= 4) && (cbm_port_id <= 26)));
	if ((cbm_port_id >= 4) && (cbm_port_id <= 26)) {
		*deq_ctr = xrx500_cbm_r32(CBM_DQM_BASE
		+ CBM_DQM_CPU_PORT(cbm_port_id,
		dqpc));
		return CBM_SUCCESS;
	}
	return CBM_FAILURE;
}
EXPORT_SYMBOL(cbm_dequeue_dma_port_stats_get);

int32_t cbm_enqueue_dma_port_stats_get(
	int32_t cbm_port_id,
	uint32_t *occupancy_ctr,
	uint32_t *enq_ctr,
	uint32_t flags)
{

	WARN_ON(!((cbm_port_id >= 5) && (cbm_port_id <= 15)));
	if ((cbm_port_id >= 5) && (cbm_port_id <= 15)) {
		 *enq_ctr = xrx500_cbm_r32(CBM_EQM_BASE
		+ CBM_EQM_CPU_PORT(cbm_port_id,
		eqpc));
		*occupancy_ctr = xrx500_cbm_r32(CBM_EQM_BASE
		+ CBM_EQM_CPU_PORT(cbm_port_id,
		pocc));
	return CBM_SUCCESS;
	}
	return CBM_FAILURE;
}
EXPORT_SYMBOL(cbm_enqueue_dma_port_stats_get);

uint32_t get_enq_counter(uint32_t index)
{
	return xrx500_cbm_r32(CBM_QEQCNT_BASE + index * 4);
}
EXPORT_SYMBOL(get_enq_counter);

uint32_t get_deq_counter(uint32_t index)
{
	return xrx500_cbm_r32(CBM_QDQCNT_BASE + index * 4);
}
EXPORT_SYMBOL(get_deq_counter);

void reset_enq_counter(uint32_t index)
{
	xrx500_cbm_w32(CBM_QEQCNT_BASE + index * 4, 0);
}
EXPORT_SYMBOL(reset_enq_counter);

void reset_deq_counter(uint32_t index)
{
	xrx500_cbm_w32(CBM_QDQCNT_BASE + index * 4, 0);
}
EXPORT_SYMBOL(reset_deq_counter);

void reset_enq_counter_all()
{
	int i;
	for (i = 0; i < CBM_QEQCNTR_SIZE / sizeof(uint32_t); i++)
		xrx500_cbm_w32(CBM_QEQCNT_BASE+i*4, 0);
}
EXPORT_SYMBOL(reset_enq_counter_all);

void reset_deq_counter_all()
{
	int i;
	for (i = 0; i < CBM_QDQCNTR_SIZE / sizeof(uint32_t); i++)
		xrx500_cbm_w32(CBM_QDQCNT_BASE+i*4, 0);
}
EXPORT_SYMBOL(reset_deq_counter_all);

int cbm_counter_mode_set(int enq, int mode)
{
	int curr_mode;
	curr_mode = get_val(xrx500_cbm_r32(cbm_cntr_func[enq].reg), cbm_cntr_func[enq].msel_mask, cbm_cntr_func[enq].msel_pos);
	if (mode != curr_mode) {
		set_val(cbm_cntr_func[enq].reg, 0, cbm_cntr_func[enq].qen_mask, cbm_cntr_func[enq].qen_pos);
		cbm_cntr_func[enq].reset_counter();
		set_val(cbm_cntr_func[enq].reg, mode, cbm_cntr_func[enq].msel_mask, cbm_cntr_func[enq].msel_pos);
		set_val(cbm_cntr_func[enq].reg, 1, cbm_cntr_func[enq].qen_mask, cbm_cntr_func[enq].qen_pos);
	}
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_counter_mode_set);

int cbm_q_thres_get(uint32_t *length)
{
	*length = get_val (xrx500_cbm_r32(CBM_EQM_BASE + CBM_EQM_CTRL), CBM_EQM_CTRL_L_MASK, CBM_EQM_CTRL_L_POS);
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_q_thres_get);
int cbm_q_thres_set(uint32_t length)
{
	set_val((CBM_EQM_BASE + CBM_EQM_CTRL), length, CBM_EQM_CTRL_L_MASK, CBM_EQM_CTRL_L_POS);
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_q_thres_set);
int cbm_counter_mode_get(int enq, int *mode)
{
	*mode = get_val(xrx500_cbm_r32(cbm_cntr_func[enq].reg), cbm_cntr_func[enq].msel_mask, cbm_cntr_func[enq].msel_pos);
	return CBM_SUCCESS;
}
EXPORT_SYMBOL(cbm_counter_mode_get);

void set_lookup_qid_via_index(uint32_t index, uint32_t qid)
{
	uint32_t offset = (index / 4) * 4;
	uint32_t shift = (index % 4) * 8;
	unsigned long sys_flag;
	uint32_t tmp;
	spin_lock_irqsave(&cbm_qidt_lock, sys_flag);
	tmp = xrx500_cbm_r32(CBM_QIDT_BASE + offset);
	tmp = (tmp & (~(0xFF << shift))) | (qid << shift);
	xrx500_cbm_w32(CBM_QIDT_BASE + offset, tmp);
	spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);
}
EXPORT_SYMBOL(set_lookup_qid_via_index);


uint8_t get_lookup_qid_via_index(uint32_t index)
{
	uint32_t offset = (index / 4) * 4;
	uint32_t shift = (index % 4) * 8;
	unsigned long sys_flag;
	uint8_t value = 0;
	spin_lock_irqsave(&cbm_qidt_lock, sys_flag);
	value =  ((xrx500_cbm_r32(CBM_QIDT_BASE + offset)) >> shift) & 0xFF;
	spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);
	return value;
}
EXPORT_SYMBOL(get_lookup_qid_via_index);

uint8_t get_lookup_qid_via_bits(uint32_t ep,
uint32_t classid,
uint32_t mpe1,
uint32_t mpe2,
uint32_t enc,
uint32_t dec,
uint8_t flow_id,
uint32_t dic)
{
	uint32_t index;
	index = ((flow_id  << 12) & 0x3000)
		| ((dec   << 11) & 0x800)
		| ((enc   << 10) & 0x400)
		| ((mpe2  << 9) & 0x200)
		| ((mpe1  << 8) & 0x100)
		| ((ep  << 4) & 0xF0)
		| ((classid) & 0x0F);
	return get_lookup_qid_via_index(index);
}
EXPORT_SYMBOL(get_lookup_qid_via_bits);

void setup_DMA_p2p()
{
/*do only p2p here.commented out other dma channel setup*/
	int chan1 = DMA1TX_LAN_SWITCH_CLASS0;
	int chan2 = DMA2RX_GSWIP_R_CLASS0;
#if !defined(SINGLE_RX_CH0_ONLY) || !SINGLE_RX_CH0_ONLY
	int chan1_1 = DMA1TX_LAN_SWITCH_CLASS1;
	int chan2_1 = DMA2RX_GSWIP_R_CLASS1;
	int chan1_2 = DMA1TX_LAN_SWITCH_CLASS2;
	int chan2_2 = DMA2RX_GSWIP_R_CLASS2;
	int chan1_3 = DMA1TX_LAN_SWITCH_CLASS3;
	int chan2_3 = DMA2RX_GSWIP_R_CLASS3;
#endif

	if ((ltq_request_dma(chan1, "dma1 tx lan")) < 0)
		LOGF_KLOG_ERROR(" %s failed to open chan for chan1\r\n", __func__);

	if ((ltq_request_dma(chan2, "dma2 rx gswpr")) < 0)
		LOGF_KLOG_ERROR(" %s failed to open chan for chan2\r\n", __func__);

	/* rx decriptor setup */
	if (ltq_dma_chan_desc_alloc(chan2, 32) == 0)
		ltq_dma_chan_data_buf_alloc(chan2);

#if !defined(SINGLE_RX_CH0_ONLY) || !SINGLE_RX_CH0_ONLY
	if ((ltq_request_dma(chan1_1, "dma1 tx lan")) < 0)
		LOGF_KLOG_ERROR(" %s failed to open chan for chan1_1\r\n", __func__);

	if ((ltq_request_dma(chan2_1, "dma2 rx gswpr")) < 0)
		LOGF_KLOG_ERROR(" %s failed to open chan for chan2_1\r\n", __func__);

	/* rx decriptor setup */
	if (ltq_dma_chan_desc_alloc(chan2_1, 32) == 0)
		ltq_dma_chan_data_buf_alloc(chan2_1);

	if ((ltq_request_dma(chan1_2, "dma1 tx lan")) < 0)
		LOGF_KLOG_ERROR(" %s failed to open chan for chan1_2\r\n", __func__);

	if ((ltq_request_dma(chan2_2, "dma2 rx gswpr")) < 0)
		LOGF_KLOG_ERROR(" %s failed to open chan for chan2_2\r\n", __func__);

	/* rx decriptor setup */
	if (ltq_dma_chan_desc_alloc(chan2_2, 32) == 0)
		ltq_dma_chan_data_buf_alloc(chan2_2);

	if ((ltq_request_dma(chan1_3, "dma1 tx lan")) < 0)
		LOGF_KLOG_ERROR(" %s failed to open chan for chan1_3\r\n", __func__);

	if ((ltq_request_dma(chan2_3, "dma2 rx gswpr")) < 0)
		LOGF_KLOG_ERROR(" %s failed to open chan for chan2_3\r\n", __func__);

		/* rx decriptor setup */
	if (ltq_dma_chan_desc_alloc(chan2_3, 32) == 0)
		ltq_dma_chan_data_buf_alloc(chan2_3);
#endif

	/* Any other configuration ???? */
	ltq_dma_p2p_cfg(chan2, chan1);
#if !defined(SINGLE_RX_CH0_ONLY) || !SINGLE_RX_CH0_ONLY
	ltq_dma_p2p_cfg(chan2_3, chan1_3);
	ltq_dma_p2p_cfg(chan2_1, chan1_1);
	ltq_dma_p2p_cfg(chan2_2, chan1_2);
#endif
	ltq_dma_chan_irq_disable(chan2);
	ltq_dma_chan_irq_disable(chan1);
#if !defined(SINGLE_RX_CH0_ONLY) || !SINGLE_RX_CH0_ONLY
	ltq_dma_chan_irq_disable(chan2_3);
	ltq_dma_chan_irq_disable(chan1_3);
	ltq_dma_chan_irq_disable(chan2_1);
	ltq_dma_chan_irq_disable(chan1_1);
	ltq_dma_chan_irq_disable(chan2_2);
	ltq_dma_chan_irq_disable(chan1_2);
#endif
	cbm_p2p_setup_done = 1;
	LOGF_KLOG_INFO("%s executed\n", __func__);
}
EXPORT_SYMBOL(setup_DMA_p2p);

int turn_on_DMA_p2p(void)
{
	int chan1 = DMA1TX_LAN_SWITCH_CLASS0;
	int chan2 = DMA2RX_GSWIP_R_CLASS0;
#if !defined(SINGLE_RX_CH0_ONLY) || !SINGLE_RX_CH0_ONLY
	int chan1_1 = DMA1TX_LAN_SWITCH_CLASS1;
	int chan2_1 = DMA2RX_GSWIP_R_CLASS1;
	int chan1_2 = DMA1TX_LAN_SWITCH_CLASS2;
	int chan2_2 = DMA2RX_GSWIP_R_CLASS2;
	int chan1_3 = DMA1TX_LAN_SWITCH_CLASS3;
	int chan2_3 = DMA2RX_GSWIP_R_CLASS3;
#endif

	if (!cbm_p2p_setup_done || cbm_p2p_turned_on)
		return -EPERM;

	if (!cbm_p2p_turned_on) {
		ltq_dma_chan_on(chan1);
		ltq_dma_chan_on(chan2);
#if !defined(SINGLE_RX_CH0_ONLY) || !SINGLE_RX_CH0_ONLY
		ltq_dma_chan_on(chan1_1);
		ltq_dma_chan_on(chan2_1);

		ltq_dma_chan_on(chan1_2);
		ltq_dma_chan_on(chan2_2);

		ltq_dma_chan_on(chan1_3);
		ltq_dma_chan_on(chan2_3);
#endif
		cbm_p2p_turned_on = 1;

	}
	return 0;
}
EXPORT_SYMBOL(turn_on_DMA_p2p);


static void reserved_ports_highest(cbm_tmu_res_t *tmu_res, int flag_set)
{
	static cbm_tmu_res_t high_tmu_res = {0};
	if (flag_set) {
		if (high_tmu_res.tmu_port < tmu_res->tmu_port)
			high_tmu_res.tmu_port = tmu_res->tmu_port;
		if (high_tmu_res.cbm_deq_port < tmu_res->cbm_deq_port)
			high_tmu_res.cbm_deq_port = tmu_res->cbm_deq_port;

		if ((high_tmu_res.tmu_q < tmu_res->tmu_q) && (tmu_res->tmu_q != 255))
			high_tmu_res.tmu_q = tmu_res->tmu_q;
		if ((high_tmu_res.tmu_sched < tmu_res->tmu_sched) && (tmu_res->tmu_q != 255))
			high_tmu_res.tmu_sched = tmu_res->tmu_sched;
	} else {
		tmu_res->cbm_deq_port = high_tmu_res.cbm_deq_port;
		tmu_res->tmu_port = high_tmu_res.tmu_port;
		tmu_res->tmu_sched = high_tmu_res.tmu_sched;
		tmu_res->tmu_q = high_tmu_res.tmu_q;
	}

}

void init_special_ports(uint32_t tmu_port)
{
	struct cbm_pmac_port_map local_entry = {0};
	local_entry.pmac = CBM_PMAC_NOT_APPL;
	local_entry.qid_num = 0;
	/*special mapping for direct path*/
	local_entry.egp_port_map = BIT(tmu_port);
	local_entry.qids[local_entry.qid_num]
	= dqm_port_info[tmu_port].def_qid;
	local_entry.qid_num++;
	local_entry.egp_type = (tmu_port == DQM_CHECKSUM_ID) ? DP_F_CHECKSUM : DP_F_DIRECTPATH_RX;
	cbm_add_to_list(&local_entry);
}

static int configure_ports(struct device_node *node)
{
	int count = 0;
	struct device_node *child;
	unsigned int config[CBM_NUM_ITEMS_DTS_CONFIG];
	int result, res = 0;
	unsigned int ep, flags = 0;
	int type, num_queue;
	int (*fp) (int);
	struct cbm_qidt_elm qidt_set = {0};
	struct cbm_qidt_mask qidt_mask = {0};
	cbm_tmu_res_t tmp_res;
	uint32_t tmu_port, tmu_queue;
	uint32_t dma_ctrl, dma_chan;
	uint32_t port_enable, num_desc;

	struct cbm_pmac_port_map local_entry = {0};
	count = of_get_child_count(node);

	LOGF_KLOG_DEBUG("%s\r\n", node->name);
	for_each_child_of_node(node, child) {
		memset(&local_entry, 0, sizeof(local_entry));
		type = keyfromstring(node->name);
		LOGF_KLOG_DEBUG("%d\r\n", type);
		if (type == DQM_PORTS_VALUE)
			fp = &find_dqm_port_type;
		else
			fp = &find_eqm_port_type;
		of_property_read_u32_array(child,
		"lantiq,cbm-config", config, 1);
		result = fp(config[0]);
		if (result == CBM_FAILURE) {
			LOGF_KLOG_ERROR("Invalid port number dts\r\n");
			return CBM_FAILURE;
		}
		of_property_read_u32_array(child,
		"lantiq,cbm-config",
		config,
		CBM_NUM_ITEMS_DTS_CONFIG);
		LOGF_KLOG_DEBUG("%d %d %d %d %d %d\r\n",
		config[0], config[1], config[2],
		config[3], config[4], config[5]);
		tmu_port = config[0];
		tmu_queue = config[1];
		dma_ctrl = config[2];
		dma_chan = config[3];
		port_enable = config[4];
		num_desc = config[5];

		if (!port_enable)
			continue;
		if (type == DQM_PORTS_VALUE) {
			/*As of now tmu port and cbm port ar emapped  1 to 1*/
			tmp_res.cbm_deq_port = tmu_port;
			tmp_res.tmu_port = tmu_port;
			tmp_res.tmu_sched = tmu_queue - SBID_START;
			tmp_res.tmu_q = tmu_queue;
			reserved_ports_highest(&tmp_res, 1);
		}
		memset(&qidt_set, 0, sizeof(struct cbm_qidt_elm));
		memset(&qidt_mask, 0, sizeof(struct cbm_qidt_mask));
		switch (result) {
		case DQM_DMA_TYPE:
			num_queue = 1;
			/*check if the EP is enabled and do the config*/
			if ((tmu_port == 22) && (cbm_rev <= CBM_SOC_REV_1)) {
				LOGF_KLOG_INFO("skip dq22 if a11\r\n");
				break;
			}
			if (tmu_port == 18)
				num_queue = 4;
			dqm_port_info[tmu_port]
			.def_qid = tmu_queue;
			dqm_port_info[tmu_port]
			.def_schd = tmu_queue - SBID_START;
			dqm_port_info[tmu_port]
			.deq_info.cbm_dq_port_base
			= CBM_DMADESC_BASE + CBM_DQM_DMA_DESC(tmu_port, 0);
			dqm_port_info[tmu_port]
			.deq_info.num_desc
			= num_desc;
			dqm_port_info[tmu_port]
			.deq_info.port_no
			= tmu_port;
			dqm_port_info[tmu_port]
			.deq_info.dma_tx_chan
			= dma_chan;
			dqm_port_info[tmu_port]
			.cbm_buf_free_base = NULL;
			dqm_port_info[tmu_port]
			.num_free_entries = 0;
			if (!get_matching_flag(&flags, tmu_port)) {
				dqm_port_info[tmu_port].egp_type
				= flags;
			}
			dqm_port_info[tmu_port]
			.dma_dt_ch
			= dma_chan;
			dqm_port_info[tmu_port]
			.dma_dt_ctrl
			= dma_ctrl;
			dqm_port_info[tmu_port]
			.dma_dt_init_type
			= 1;
			cbm_dma_port_enable(tmu_port,
			CBM_PORT_F_DEQUEUE_PORT);
			/*config the tmu queue and scheduler*/
			tmu_create_flat_egress_path(1,
			tmu_port,
			tmu_queue - SBID_START,
			tmu_queue,
			num_queue);
			if ((tmu_port == DQM_CHECKSUM_ID) || (tmu_port == DQM_DIRECTPATH_RX)) {
				init_special_ports(tmu_port);
			}
			break;
		case DQM_LDMA_TYPE:
			dqm_port_info[tmu_port]
			.def_qid = tmu_queue;
			dqm_port_info[tmu_port]
			.def_schd = tmu_queue - SBID_START;
			dqm_port_info[tmu_port]
			.deq_info.cbm_dq_port_base
			= CBM_DQM_BASE + LDESC0_0_DMA_EGP_23;
			dqm_port_info[tmu_port]
			.deq_info.num_desc
			 = num_desc;
			 dqm_port_info[tmu_port]
			.deq_info.port_no
			= tmu_port;
			dqm_port_info[tmu_port]
			.deq_info.dma_tx_chan
			= dqm_port_info[CBM_FROM_VRX318_PORT]
			.deq_info.dma_tx_chan;
			 dqm_port_info[tmu_port]
			.cbm_buf_free_base = NULL;
			dqm_port_info[tmu_port]
			.num_free_entries = 0;
			if (!get_matching_flag(&flags, tmu_port)) {
				dqm_port_info[tmu_port].egp_type
				= flags;
			}
			init_cbm_dqm_ldma_port(tmu_port);
			/*config the tmu queue and scheduler*/
			tmu_create_flat_egress_path(1,
			tmu_port,
			tmu_queue - SBID_START,
			tmu_queue,
			1);
			break;
		case DQM_CPU_TYPE:
			dqm_port_info[tmu_port]
			.def_qid = tmu_queue;
			dqm_port_info[tmu_port]
			.def_schd = tmu_queue - SBID_START;
			dqm_port_info[tmu_port]
			.deq_info.cbm_dq_port_base
			 = CBM_DQM_BASE + CBM_DQM_CPU_PORT(tmu_port, desc0);
			dqm_port_info[tmu_port]
			.deq_info.num_desc
			 = num_desc;
			dqm_port_info[tmu_port]
			.deq_info.port_no
			= tmu_port;
			dqm_port_info[tmu_port]
			.deq_info.dma_tx_chan
			= dma_chan;
			dqm_port_info[tmu_port]
			.cbm_buf_free_base
			= CBM_DQM_BASE + CBM_DQM_CPU_PORT(tmu_port, ptr_rtn);
			dqm_port_info[tmu_port]
			.num_free_entries
			= (tmu_port > 3) ? 32 : 1;
			/*config cbm/dma port*/
			init_cbm_dqm_cpu_port(tmu_port);
			/*config the tmu queue and scheduler*/
			tmu_create_flat_egress_path(1,
			tmu_port,
			tmu_queue - SBID_START,
			tmu_queue, 1);


			if (tmu_port <= 3) {
				if (port_enable == DP_F_DEQ_CPU) {
					flags = 0;
					assign_port_from_DT(0, tmu_port);
					assign_port_from_DT(DP_F_DIRECT, tmu_port);
				} else if (port_enable == DP_F_DEQ_CPU1) {
					flags = 0;
					/*assign_port_from_DT(0, tmu_port);
					assign_port_from_DT(DP_F_DIRECT, tmu_port);*/
				} else if (port_enable == DP_F_DEQ_MPE) {
					flags = DP_F_MPE_ACCEL;
					assign_port_from_DT(DP_F_MPE_ACCEL, tmu_port);
				} else if (port_enable == DP_F_DEQ_DL) {
					flags = DP_F_DIRECTLINK;
					assign_port_from_DT(DP_F_DIRECTLINK, tmu_port);
				}
				/*if(tmu_port != 0) {*/
				/*TMU/CBM port  with pmac 0 is fixed for cpu port*/
				#ifdef CONFIG_CBM_LS_ENABLE
				if (port_enable == DP_F_DEQ_CPU1) {
					local_entry.pmac = 0;
					local_entry.egp_type = 0;
					qidt_set.mpe2 = 0;
					qidt_set.mpe1 = 0;
					qidt_set.flowidl = 1;
					qidt_set.flowidh = 0;
					/*to receive packets with any classid*/
					qidt_mask.classid_mask = 1;
					qidt_mask.mpe1_mask = 1;
					qidt_mask.mpe2_mask = 1;
					qidt_mask.enc_mask = 1;
					qidt_mask.dec_mask = 1;
					cbm_qidt_set(&qidt_set,
					&qidt_mask,
					tmu_queue);
				} else
				#endif
				if (port_enable == DP_F_DEQ_CPU) {
					local_entry.pmac = 0;
					local_entry.egp_type = 0;
					qidt_set.mpe2 = 0;
					qidt_set.mpe1 = 0;
					qidt_set.flowidh = 0;
					#ifdef CONFIG_CBM_LS_ENABLE
					qidt_set.flowidl = 0;
					#else
					qidt_mask.flowid_lmask = 1;
					#endif
					/*to receive packets with any classid*/
					qidt_mask.classid_mask = 1;
					qidt_mask.mpe1_mask = 1;
					qidt_mask.mpe2_mask = 1;
					qidt_mask.enc_mask = 1;
					qidt_mask.dec_mask = 1;
					cbm_qidt_set(&qidt_set,
					&qidt_mask,
					tmu_queue);
				} else {
					res = get_matching_EP(tmu_port, flags, &ep);
					if (res) {
						local_entry.egp_type = flags;
						local_entry.pmac = ep;
						if (ep == CBM_PMAC_DYNAMIC) {
							LOGF_KLOG_DEBUG("ep is dyanmic %d\r\n", tmu_port);
							/*break;*/
							goto ASSIGN_FLAGS;
						}
					} else {
						LOGF_KLOG_ERROR("mapping missing for phys port %d\r\n", tmu_port);
						return CBM_FAILURE;
					}
				}
				local_entry.egp_port_map |= BIT(tmu_port);
				local_entry.qids[local_entry.qid_num] = dqm_port_info[tmu_port].def_qid;
				local_entry.qid_num++;
				local_entry.owner = 0;
				local_entry.dev = 0;
				local_entry.dev_port = 0;
				local_entry.flags = P_ALLOCATED;
				cbm_add_to_list(&local_entry);
			} else if ((cbm_rev > CBM_SOC_REV_1) && (tmu_port > 4)) {
				flags = DP_F_FAST_WLAN;
				assign_port_from_DT(DP_F_FAST_WLAN, tmu_port);
			}
ASSIGN_FLAGS:
			dqm_port_info[tmu_port].egp_type
			= flags;
			break;
		case DQM_SCPU_TYPE:
			dqm_port_info[tmu_port]
			.def_qid = tmu_queue;
			dqm_port_info[tmu_port]
			.def_schd = tmu_queue - SBID_START;
			dqm_port_info[tmu_port]
			.deq_info.cbm_dq_port_base
			= CBM_DMADESC_BASE + CBM_DQM_DMA_DESC(tmu_port, 0);
			dqm_port_info[tmu_port]
			.deq_info.num_desc
			 = num_desc;
			dqm_port_info[tmu_port]
			.deq_info.port_no
			= tmu_port;
			dqm_port_info[tmu_port]
			.deq_info.dma_tx_chan
			= dma_chan;
			dqm_port_info[tmu_port]
			.cbm_buf_free_base
			= NULL;
			dqm_port_info[tmu_port]
			.num_free_entries
			= 0;
			if (!get_matching_flag(&flags, tmu_port)) {
				dqm_port_info[tmu_port].egp_type
				= flags;
			}
			/*config cbm/dma port*/
			init_cbm_dqm_scpu_port(tmu_port);
			/*config the tmu queue and scheduler*/
			tmu_create_flat_egress_path(
			1,
			tmu_port,
			tmu_queue - SBID_START,
			tmu_queue,
			1);
			/*LRO*/
			qidt_set.flowidh = 1;
			qidt_mask.mpe1_mask = 1;
			qidt_mask.mpe2_mask = 1;
			qidt_mask.enc_mask = 1;
			qidt_mask.dec_mask = 1;
			qidt_mask.flowid_lmask = 1;
			/*to receive packets with any classid*/
			qidt_mask.classid_mask = 1;
			cbm_qidt_set(
			&qidt_set,
			&qidt_mask,
			tmu_queue);
			break;
		case EQM_DMA_TYPE:
			/*check if the EP is enabled and do the config*/
			if (dma_chan) {
				eqm_port_info[tmu_port]
				.num_eq_ports = 2;
				eqm_port_info[tmu_port].eq_info
				.cbm_eq_port_base
				= CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(tmu_port, 0, 1);
				eqm_port_info[tmu_port].eq_info
				.port_no
				= tmu_port;
				eqm_port_info[tmu_port]
				.port_type = port_enable;
				eqm_port_info[tmu_port].eq_info
				.num_desc
				= num_desc;
				/*if a21, change the jumbo dma channel number.*/
				if ((cbm_rev > CBM_SOC_REV_1) && (tmu_queue == 1)) {
					LOGF_KLOG_INFO("%s %d %d %d %d %d %d\r\n", __func__,
					config[0], config[1], config[2],
					config[3], config[4], config[5]);
					dma_ctrl = (dma_ctrl == 16) ? 6 : 11;
				}
				eqm_port_info[tmu_port]
				.dma_dt_jum_ch
				= dma_ctrl;
				eqm_port_info[tmu_port]
				.dma_dt_ctrl
				= tmu_queue;
				eqm_port_info[tmu_port]
				.dma_dt_init_type
				|= 2;
				cbm_dma_port_enable(
				tmu_port,
				CBM_PORT_F_JUMBO_BUF);
			} else {
				eqm_port_info[tmu_port]
				.num_eq_ports = 1;
				eqm_port_info[tmu_port].eq_info
				.cbm_eq_port_base
				= CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(tmu_port, 0, 0);
				eqm_port_info[tmu_port].eq_info
				.port_no
				= tmu_port;
				eqm_port_info[tmu_port]
				.port_type = port_enable;
				eqm_port_info[tmu_port].eq_info
				.num_desc
				= num_desc;
				/*if a21, change the standard dma channel number.*/
				if ((cbm_rev > CBM_SOC_REV_1) && (tmu_queue == 1)) {
					LOGF_KLOG_INFO("%s %d %d %d %d %d %d\r\n", __func__,
					config[0], config[1], config[2],
					config[3], config[4], config[5]);
					dma_ctrl = (dma_ctrl == 0) ? 0 : 5;
				}
				eqm_port_info[tmu_port]
				.dma_dt_std_ch
				= dma_ctrl;
				eqm_port_info[tmu_port]
				.dma_dt_ctrl
				= tmu_queue;
				eqm_port_info[tmu_port]
				.dma_dt_init_type
				|= 4;
				cbm_dma_port_enable(
				tmu_port,
				CBM_PORT_F_STANDARD_BUF);
			}
			break;
		case EQM_CPU_TYPE:
			eqm_port_info[tmu_port]
			.num_eq_ports = 1;
			eqm_port_info[tmu_port].eq_info
			.cbm_eq_port_base
			= CBM_EQM_BASE + CBM_EQM_CPU_PORT(tmu_port, desc0);
			eqm_port_info[tmu_port].eq_info
			.port_no
			= tmu_port;
			eqm_port_info[tmu_port]
			.port_type = port_enable;
			eqm_port_info[tmu_port].eq_info
			.num_desc
			= num_desc;
			init_cbm_eqm_cpu_port(tmu_port);
			break;
		case EQM_TOE_TYPE:
			eqm_port_info[tmu_port]
			.num_eq_ports = 1;
			eqm_port_info[tmu_port].eq_info
			.cbm_eq_port_base
			= CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(tmu_port, 0, 0);
			eqm_port_info[tmu_port].eq_info
			.port_no
			= tmu_port;
			eqm_port_info[tmu_port]
			.port_type = port_enable;
			cbm_dma_toe_port_enable(tmu_port, CBM_PORT_F_STANDARD_BUF);
			break;
		case EQM_VRX318_TYPE:
			/*VRX318 supports only std size packets at this time*/
			eqm_port_info[tmu_port]
			.num_eq_ports = 1;
			eqm_port_info[tmu_port].eq_info
			.cbm_eq_port_base
			= CBM_EQM_BASE + CBM_EQM_CPU_PORT(tmu_port, desc0);
			eqm_port_info[tmu_port].eq_info
			.port_no
			= tmu_port;
			eqm_port_info[tmu_port].eq_info
			.num_desc
			= 32;
			eqm_port_info[tmu_port]
			.port_type = port_enable;
			cbm_dma_vrx318_port_enable(tmu_port, CBM_PORT_F_STANDARD_BUF);
			break;
		case EQM_DL_TYPE:
			eqm_port_info[tmu_port]
				.num_eq_ports = 1;
			eqm_port_info[tmu_port].eq_info
				.cbm_eq_port_base
				= CBM_DMADESC_BASE + CBM_EQM_DMA_DESC(tmu_port, 0, 0);
			eqm_port_info[tmu_port].eq_info
				.port_no
				= tmu_port;
			eqm_port_info[tmu_port]
				.port_type = port_enable;
			eqm_port_info[tmu_port].eq_info
				.num_desc
				= num_desc;
			eqm_port_info[tmu_port].eq_info
				.dma_rx_chan_std
				= dma_ctrl;
			eqm_port_info[tmu_port]
				.dma_dt_ctrl
				= tmu_queue;
			eqm_port_info[tmu_port]
				.dma_dt_init_type
				|= 255;
		default:
			LOGF_KLOG_ERROR("%s wrong key", __func__);
		}
	}
	return CBM_SUCCESS;
}
static int cbm_xrx500_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct resource irqres[CBM_NUM_INTERRUPTS];
	struct device_node *node = pdev->dev.of_node, *child;
	struct clk *clk;
	struct resource *res[CBM_NUM_BASEADDR_IN_DTS];
	int i;
	int count;
	unsigned long sys_flag;
	struct clk *ppe_clk;


	/** init spin lock */
	spin_lock_init(&cbm_qidt_lock);
	spin_lock_init(&cbm_buff_lock);
	spin_lock_init(&cbm_queue_flush_lock);
	spin_lock_init(&cbm_port_mapping);


	#if 1
	/* load the memory ranges */
	for (i = 0; i < CBM_NUM_BASEADDR_IN_DTS; i++) {
		res[i] = platform_get_resource(pdev, IORESOURCE_MEM, i);
		if (!res[i]) {
			LOGF_KLOG_DEV_ERROR(&pdev->dev, "failed to get resources %d\n", i);
			return -ENOENT;
		}
	}
	#endif
	g_cbm_ctrl.id = pdev->id;
	g_cbm_ctrl.dev = &pdev->dev;
	g_cbm_ctrl.jumbo_size_mask = (CONFIG_CBM_JBO_PKT_SIZE == 0x2000) ? 0xffffe000 : 0xffffc000;

	g_base_addr.tmu_addr_base = devm_ioremap_resource(&pdev->dev, res[0]);
	g_base_addr.cbm_addr_base = devm_ioremap_resource(&pdev->dev, res[1]);
	g_base_addr.cbm_qidt_addr_base = devm_ioremap_resource(&pdev->dev, res[2]);
	g_base_addr.cbm_qeqcnt_addr_base = devm_ioremap_resource(&pdev->dev, res[4]);
	g_base_addr.cbm_qdqcnt_addr_base = devm_ioremap_resource(&pdev->dev, res[5]);
	g_base_addr.cbm_ls_addr_base = devm_ioremap_resource(&pdev->dev, res[6]);
	g_base_addr.cbm_eqm_addr_base = devm_ioremap_resource(&pdev->dev, res[7]);
	g_base_addr.cbm_dqm_addr_base = devm_ioremap_resource(&pdev->dev, res[8]);
	g_base_addr.fsqm0_addr_base = devm_ioremap_resource(&pdev->dev, res[9]);
	g_base_addr.fsqm1_addr_base = devm_ioremap_resource(&pdev->dev, res[10]);
	g_base_addr.cbm_dmadesc_addr_base = devm_ioremap_resource(&pdev->dev, res[11]);

	if (!g_base_addr.tmu_addr_base || !g_base_addr.cbm_addr_base  || !g_base_addr.cbm_qidt_addr_base ||
			!g_base_addr.cbm_qeqcnt_addr_base  || !g_base_addr.cbm_qdqcnt_addr_base ||
			!g_base_addr.cbm_ls_addr_base || !g_base_addr.cbm_eqm_addr_base ||
			!g_base_addr.cbm_dqm_addr_base || !g_base_addr.fsqm0_addr_base ||
			!g_base_addr.fsqm1_addr_base || !g_base_addr.cbm_dmadesc_addr_base)  {
		LOGF_KLOG_DEV_ERROR(&pdev->dev, "failed to request and remap io ranges\n");
		return -ENOMEM;
	}
	store_bufreq_baseaddr();
	ret = of_irq_to_resource_table(node, irqres, CBM_NUM_INTERRUPTS);
	if (ret != CBM_NUM_INTERRUPTS) {
				LOGF_KLOG_DEV_ERROR(&pdev->dev,
						"failed to get irq for CBM");
				return -ENODEV;
	}
	g_cbm_irq[0] = irqres[0].start;
	#ifdef ENABLE_LL_DEBUG
	g_cbm_irq[2] = irqres[2].start;
	#endif
	for_each_present_cpu(i) {
		if (cpu_online(i)) {
			set_bit(i, &g_cpu_port_alloc);
			g_cbm_irq[i+1] = irqres[i+1].start;
		}
	}
#ifndef CONFIG_USE_EMULATOR
	/* get the clock */
	clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(clk)) {
		LOGF_KLOG_DEV_ERROR(&pdev->dev, "Failed to get clock\n");
		return PTR_ERR(clk);
	}
	clk_enable(clk);
#endif

	spin_lock_irqsave(&cbm_qidt_lock, sys_flag);
	for (i = 0; i < 0x1000; i++) {
		g_cbm_qidt_mirror[i].qidt_shadow = 0xFFFFFFFF;
		xrx500_cbm_w32((CBM_QIDT_BASE + i * 4), 0xFFFFFFFF);
	}
	spin_unlock_irqrestore(&cbm_qidt_lock, sys_flag);
#ifdef CONFIG_SOC_GRX500_A21
	cbm_rev = 2;
#endif
	LOGF_KLOG_INFO("%d\r\n", cbm_rev);	/* Init the TMU first*/
	tmu_ll_init((long unsigned int)g_base_addr.tmu_addr_base);

	cbm_hw_init();
/*configure p2p channels*/
	setup_DMA_p2p();
	#if 1
	count = of_get_child_count(node);
	if (count) {
		LOGF_KLOG_DEBUG("node count %d\r\n", count);
		for_each_child_of_node(node, child) {
			configure_ports(child);
		}
	}
	#endif
	/*Initialise the DQ port 0 as the cbm buffer free API uses DQ port 0 for freeing,
	but do not initialise the TMU queue or look up table for this.
	The CPU traffic id redirected to DQ2 via device tree entry*/
	for_each_online_cpu(i)
		init_cbm_dqm_cpu_port(i);
	/*Enable the EQM and DQM contollers*/
	xrx500_cbm_w32((CBM_DQM_BASE + CBM_DQM_CTRL), 0x11);
	for (i = 0; i < DEFAULT_WAIT_CYCLES; i++) {
		if ((xrx500_cbm_r32(CBM_DQM_BASE + CBM_DQM_CTRL)
		& CBM_DQM_CTRL_DQM_ACT_MASK) != 0)
			LOGF_KLOG_DEBUG("cbm dqm init successfully\n");
	}
	init_cbm_ls();
	xrx500_cbm_w32((CBM_EQM_BASE + CBM_EQM_CTRL), 0x11);
	for (i = 0; i < DEFAULT_WAIT_CYCLES; i++) {
		if ((xrx500_cbm_r32(CBM_EQM_BASE + CBM_EQM_CTRL)
		& CBM_EQM_CTRL_EQM_ACT_MASK) != 0) {
			LOGF_KLOG_DEBUG("cbm eqm init successfully\n");
			break;
		}
	}
#if 1
	for_each_online_cpu(i)
		tasklet_init(&cbm_tasklet[i],
			do_cbm_tasklet, (unsigned long) i);
#endif
#ifdef ENABLE_LL_DEBUG
		tasklet_init(&cbm_debug_tasklet,
			do_cbm_debug_tasklet, (unsigned long) i);
#endif
#ifdef CONFIG_CBM_LS_ENABLE
	init_dummy_netdev(&g_cbm_ctrl.dummy_dev);
	netif_napi_add(&g_cbm_ctrl.dummy_dev,
				&g_cbm_ctrl.napi, do_dq_cbm_poll, 20);
	napi_enable(&g_cbm_ctrl.napi);
#endif
	/* Disable all the EQM and DQM interrupts */
	eqm_intr_ctrl(0);
#ifndef CONFIG_CBM_LS_ENABLE
	dqm_intr_ctrl(0);
#endif
	/* Enable all the LS interrupts */
	ls_intr_ctrl(0xFF0000);
	cbm_proc_install(NULL);
	store_rcnt_baseaddr();
	/*set the Ingress port delay before enqueue*/
	ppe_clk = clk_get_ppe();
	pr_info("CBM Clock: %ldHz \n",
	clk_get_rate(ppe_clk));
	if ((cbm_rev > CBM_SOC_REV_1) /*&& (clk_get_rate(ppe_clk) > 300000000)*/) {
		pr_info("EQM delay Enabled\n");
		cbm_queue_delay_enable_set(1, 0);
	}
	/*setup the DMA channels*/
	#if 1
	for (i = 0; i < CBM_PORT_MAX; i++) {
		switch (dqm_port_info[i].dma_dt_init_type) {
		case 1:
		cbm_dequeue_dma_port_init(i,
		dqm_port_info[i].dma_dt_ctrl,
		dqm_port_info[i].dma_dt_ch,
		0);
		break;
		default:
		break;
		}
	}
	for (i = 0; i < CBM_EQM_PORT_MAX; i++) {
		switch (eqm_port_info[i].dma_dt_init_type) {
		case 2:
		cbm_enqueue_dma_port_init(
			i,
			eqm_port_info[i].dma_dt_ctrl,
			0,
			eqm_port_info[i].dma_dt_jum_ch,
			CBM_PORT_F_JUMBO_BUF);
			/*cbm_dma_port_enable(
			i,
			CBM_PORT_F_JUMBO_BUF);*/
		break;
		case 4:
		cbm_enqueue_dma_port_init(
			i,
			eqm_port_info[i].dma_dt_ctrl,
			eqm_port_info[i].dma_dt_std_ch,
			0,
			CBM_PORT_F_STANDARD_BUF);
			/*cbm_dma_port_enable(
			i,
			CBM_PORT_F_STANDARD_BUF);*/
		break;
		case 6:
		cbm_enqueue_dma_port_init(
			i,
			eqm_port_info[i].dma_dt_ctrl,
			0,
			eqm_port_info[i].dma_dt_jum_ch,
			CBM_PORT_F_JUMBO_BUF);
			/*cbm_dma_port_enable(
			i,
			CBM_PORT_F_JUMBO_BUF);*/
		cbm_enqueue_dma_port_init(
			i,
			eqm_port_info[i].dma_dt_ctrl,
			eqm_port_info[i].dma_dt_std_ch,
			0,
			CBM_PORT_F_STANDARD_BUF);
			/*cbm_dma_port_enable(
			i,
			CBM_PORT_F_STANDARD_BUF);*/
		break;
		default:
		break;
		}
	}
	#endif
	LOGF_KLOG_INFO("%s", "CBM: Init Done !!");
	return 0;


}

static int	cbm_xrx500_release(struct platform_device *pdev)
{
	return 0;
}
static const struct of_device_id cbm_xrx500_match[] = {
	{ .compatible = "lantiq,cbm-xrx500" },
	{},
};

static struct platform_driver cbm_xrx500_driver = {
	.probe = cbm_xrx500_probe,
	.remove = cbm_xrx500_release,
	.driver = {
				.name = "cbm-xrx500",
				.owner = THIS_MODULE,
				.of_match_table = cbm_xrx500_match,
		},
};
#if 0
module_platform_driver(cbm_xrx500_driver);
#else

int __init cbm_xrx500_init(void)
{
	return platform_driver_register(&cbm_xrx500_driver);
}

arch_initcall(cbm_xrx500_init);
#endif
