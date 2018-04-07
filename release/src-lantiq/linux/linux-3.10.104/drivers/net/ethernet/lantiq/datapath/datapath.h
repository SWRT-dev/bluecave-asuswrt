#ifndef DATAPATH_H
#define DATAPATH_H		/*! PMAC port informatin data */

#include <linux/klogging.h>
#include <linux/skbuff.h>	/*skb */
#define SEQ_PRINTF  seq_printf

#define PRINTK  LOGF_KLOG_CONT
#define PR_ERR  LOGF_KLOG_ERROR
#define PR_INFO LOGF_KLOG_INFO
#define PR_CONT LOGF_KLOG_CONT
#define PR_INFO_ONCE    LOGF_KLOG_INFO_ONCE
#define PR_RATELIMITED LOGF_KLOG_RATELIMITED

#define GSWIP_L_DEV_NAME "/dev/switch_api/0"
#define GSWIP_R_DEV_NAME "/dev/switch_api/1"
#define SEQ_PRINTF seq_printf

#define dp_set_val(reg, val, mask, offset) do \
	{(reg) &= ~(mask);\
	(reg) |= (((val) << (offset)) & (mask));\
	} while (0)

#define dp_get_val(val, mask, offset) (((val) & (mask)) >> (offset))

#define DP_DEBUG_ASSERT(expr, fmt, arg...)  do { if (expr) \
	PR_ERR(fmt, ##arg); }	\
	while (0)

#if defined(CONFIG_LTQ_DATAPATH_DBG) && CONFIG_LTQ_DATAPATH_DBG
#define DP_DEBUG(flags, fmt, arg...)  do { if (unlikely((dp_dbg_flag & flags) \
	&& (((dp_print_num_en) && (dp_max_print_num)) || (!dp_print_num_en))))\
	{ PRINTK(fmt, ##arg); if ((dp_print_num_en) &&\
	(dp_max_print_num)) dp_max_print_num--; } } \
	while (0)

#define DP_ASSERT_SCOPE __func__

#else
#define DP_DEBUG(flags, fmt, arg...)
#endif				/* end of CONFIG_LTQ_DATAPATH_DBG */

#define SET_PMAC_PORTMAP(pmac, port_id) do { if (port_id <= 7)\
	pmac->port_map2 = 1 << (port_id); \
	else \
		pmac->port_map = (1 << (port_id-8)); } \
	while (0)

#define SET_PMAC_SUBIF(pmac, subif) do {;\
	pmac->src_sub_inf_id2 = (subif) & 0xff; \
	pmac->src_sub_inf_id =  ((subif) >> 8) & 0x1f; } \
	while (0)

#define IFNAMSIZ 16
#define PMAC_MAX_NUM  16
#define PAMC_LAN_MAX_NUM 7

#define DP_LIB_LOCK    spin_lock_bh
#define DP_LIB_UNLOCK  spin_unlock_bh

#define PARSER_FLAG_SIZE   40
#define PARSER_OFFSET_SIZE 8
#define PMAC_SIZE          8

#define MAX_SUBIF_PER_PORT 16

#define PKT_PASER_FLAG_OFFSET   0
#define PKT_PASER_OFFSET_OFFSET (PARSER_FLAG_SIZE)
#define PKT_PMAC_OFFSET         ((PARSER_FLAG_SIZE) + (PARSER_OFFSET_SIZE))
#define PKT_DATA_OFFSET         ((PKT_PMAC_OFFSET) + (PMAC_SIZE))

#define CHECK_BIT(var, pos) (((var) & (1<<(pos))) ? 1 : 0)

#define VAP_OFFSET 8
#define VAP_MASK  0xF
#define VAP_DSL_OFFSET 3
#define NEW_CBM_API 1

#define PASAR_OFFSETS_NUM 40	/*40 bytes offset */
#define PASAR_FLAGS_NUM 8	/*8 bytes */

enum PARSER_FLAGS {
	PASER_FLAGS_NO = 0,
	PASER_FLAGS_END,
	PASER_FLAGS_CAPWAP,
	PASER_FLAGS_GRE,
	PASER_FLAGS_LEN,
	PASER_FLAGS_GREK,
	PASER_FLAGS_NN1,
	PASER_FLAGS_NN2,

	PASER_FLAGS_ITAG,
	PASER_FLAGS_1VLAN,
	PASER_FLAGS_2VLAN,
	PASER_FLAGS_3VLAN,
	PASER_FLAGS_4VLAN,
	PASER_FLAGS_SNAP,
	PASER_FLAGS_PPPOES,
	PASER_FLAGS_1IPV4,

	PASER_FLAGS_1IPV6,
	PASER_FLAGS_2IPV4,
	PASER_FLAGS_2IPV6,
	PASER_FLAGS_ROUTEXP,
	PASER_FLAGS_TCP,
	PASER_FLAGS_1UDP,
	PASER_FLAGS_IGMP,
	PASER_FLAGS_IPV4OPT,

	PASER_FLAGS_IPV6EXT,
	PASER_FLAGS_TCPACK,
	PASER_FLAGS_IPFRAG,
	PASER_FLAGS_EAPOL,
	PASER_FLAGS_2IPV6EXT,
	PASER_FLAGS_2UDP,
	PASER_FLAGS_L2TPNEXP,
	PASER_FLAGS_LROEXP,

	PASER_FLAGS_L2TP,
	PASER_FLAGS_GRE_VLAN1,
	PASER_FLAGS_GRE_VLAN2,
	PASER_FLAGS_GRE_PPPOE,
	PASER_FLAGS_BYTE4_BIT4,
	PASER_FLAGS_BYTE4_BIT5,
	PASER_FLAGS_BYTE4_BIT6,
	PASER_FLAGS_BYTE4_BIT7,

	PASER_FLAGS_BYTE5_BIT0,
	PASER_FLAGS_BYTE5_BIT1,
	PASER_FLAGS_BYTE5_BIT2,
	PASER_FLAGS_BYTE5_BIT3,
	PASER_FLAGS_BYTE5_BIT4,
	PASER_FLAGS_BYTE5_BIT5,
	PASER_FLAGS_BYTE5_BIT6,
	PASER_FLAGS_BYTE5_BIT7,

	PASER_FLAGS_BYTE6_BIT0,
	PASER_FLAGS_BYTE6_BIT1,
	PASER_FLAGS_BYTE6_BIT2,
	PASER_FLAGS_BYTE6_BIT3,
	PASER_FLAGS_BYTE6_BIT4,
	PASER_FLAGS_BYTE6_BIT5,
	PASER_FLAGS_BYTE6_BIT6,
	PASER_FLAGS_BYTE6_BIT7,

	PASER_FLAGS_BYTE7_BIT0,
	PASER_FLAGS_BYTE7_BIT1,
	PASER_FLAGS_BYTE7_BIT2,
	PASER_FLAGS_BYTE7_BIT3,
	PASER_FLAGS_BYTE7_BIT4,
	PASER_FLAGS_BYTE7_BIT5,
	PASER_FLAGS_BYTE7_BIT6,
	PASER_FLAGS_BYTE7_BIT7,

	/*Must be put at the end of the enum */
	PASER_FLAGS_MAX
};

/*! PMAC port flag */
enum PORT_FLAG {
	PORT_FREE = 0,		/*! The port is free */
	PORT_ALLOCATED,		/*! the port is already allocated to some driver,
				   but not registered or no need to register at all.\n
				   for example, LRO/CAPWA, only need to allocate,
				   but no need to register
				 */
	PORT_DEV_REGISTERED,	/*! dev Registered already. */
	PORT_SUBIF_REGISTERED,	/*! subif Registered already. */

	PORT_FLAG_NO_VALID	/*! Not valid flag */
};

struct dev_mib {
	uint32_t rx_fn_rxif_pkt;	/*! received packet counter */
	uint32_t rx_fn_txif_pkt;	/*! transmitted packet coutner */
	uint32_t rx_fn_dropped;	/*! transmitted packet coutner */
	uint32_t tx_cbm_pkt;	/*! transmitted packet counter */
	uint32_t tx_clone_pkt;	/*! duplicate unicast packet for cloned flag */
	uint32_t tx_hdr_room_pkt;	/*! duplicate packet for no enough skb header room */
	uint32_t tx_tso_pkt;	/*! transmitted packet counter */
	uint32_t tx_pkt_dropped;	/*! dropped packet counter */

};

/*! Sub interface detail information */
struct dp_subif_info {
	int32_t flags;
	uint32_t subif:15;
	struct net_device *netif;	/*! pointer to  net_device */
	char device_name[IFNAMSIZ]; /*! devide name, like wlan0, */
	struct dev_mib mib; /*! mib */
};

struct gsw_itf {
	u8 ep; /*-1 means no assigned yet for dynamic case */
	u8 fixed; /*fixed (1) or dynamically allocate (0)*/
	u16 start;
	u16 end;
};

struct pmac_port_info {
	enum PORT_FLAG status;	/*! port status */
	int alloc_flags;	/* the flags saved when calling dp_port_alloc */
	struct dp_cb cb;	/*! Callback Pointer to DIRECTPATH_CB */
	struct module *owner;
	struct net_device *dev;
	uint32_t dev_port;
	dp_pmac_cfg_t pmac_cfg;
	uint32_t num_subif;
	int32_t port_id;
	struct dp_subif_info subif_info[MAX_SUBIF_PER_PORT];
	uint32_t tx_err_drop;
	uint32_t rx_err_drop;
	struct gsw_itf *itf_info;  /*point to switch interface configuration */
};

struct parser_info {
	uint8_t v;
	int8_t size;
};

#if defined(CONFIG_LTQ_DATAPATH_DBG) && CONFIG_LTQ_DATAPATH_DBG
extern unsigned int dp_dbg_flag;
extern unsigned int dp_dbg_err;
extern unsigned int dp_max_print_num;
extern unsigned int dp_print_num_en;
#endif
extern struct pmac_port_info dp_port_info[PMAC_MAX_NUM];	/*port 0 is reserved */
extern int dp_loop_eth_dev_init(struct proc_dir_entry *parent);
extern void dp_loop_eth_dev_exit(void);
extern struct proc_dir_entry *dp_proc_install(void);
extern char *dp_dbg_flag_str[];
extern unsigned int dp_dbg_flag_list[];
extern u32 dp_port_flag[];
extern char *dp_port_type_str[];
extern char *dp_port_status_str[];
extern struct parser_info pinfo[];
extern int get_dp_dbg_flag_str_size(void);
extern int get_dp_port_status_str_size(void);
extern int get_dp_port_type_str_size(void);
extern GSW_API_HANDLE gswr_r;
enum TEST_MODE {
	DP_RX_MODE_NORMAL = 0,
	DP_RX_MODE_LAN_WAN_BRIDGE,
	DPR_RX_MODE_MAX
};
extern u32 dp_rx_test_mode;

extern int32_t dp_pmac_set(uint32_t port, dp_pmac_cfg_t *pmac_cfg);
char *parser_flag_str(uint8_t f);
ssize_t proc_print_mode_write(struct file *file, const char *buf,
			      size_t count, loff_t *ppos);
void proc_print_mode_read(struct seq_file *s);
int parser_size_via_index(u8 index);
struct pmac_port_info *get_port_info_via_dp_name(struct net_device *dev);
void dp_clear_mib(dp_subif_t *subif, uint32_t flag);
extern u32 dp_drop_all_tcp_err;
extern u32 dp_pkt_size_check;
int dp_mib_init(void);
void dp_mib_exit(void);
int gsw_mib_reset(int dev, u32 flag);
void print_parser_status(struct seq_file *s);
int dp_get_drv_mib(dp_subif_t *subif, dp_drv_mib_t *mib, uint32_t flag);
void proc_mib_timer_read(struct seq_file *s);
ssize_t proc_mib_timer_write(struct file *file, const char *buf, size_t count,
			     loff_t *ppos);
int proc_mib_inside_dump(struct seq_file *s, int pos);
int proc_mib_inside_start(void);
ssize_t proc_mib_inside_write(struct file *file, const char *buf,
			      size_t count, loff_t *ppos);
int proc_mib_port_start(void);
int proc_mib_port_dump(struct seq_file *s, int pos);
int proc_mib_vap_start(void);
int proc_mib_vap_dump(struct seq_file *s, int pos);
ssize_t proc_mib_vap_write(struct file *, const char *, size_t, loff_t *);
ssize_t proc_mib_port_write(struct file *file, const char *buf, size_t count,
		      loff_t *ppos);
int mpe_fh_netfiler_install(void);
int dp_coc_cpufreq_exit(void);
int dp_coc_cpufreq_init(void);
int update_coc_up_sub_module(enum ltq_cpufreq_state new_state,
			     enum ltq_cpufreq_state old_state, uint32_t flag);
int get_gsw_port_rmon(u32 ep, char *gsw_drv_name, GSW_RMON_Port_cnt_t *mib);
void proc_coc_read(struct seq_file *s);
ssize_t proc_coc_write(struct file *file, const char *buf, size_t count,
		       loff_t *ppos);
void dp_sys_mib_reset(uint32_t flag);
int dp_reset_sys_mib(u32 flag);

void dp_clear_all_mib_inside(uint32_t flag);

static inline unsigned int get_vap(uint32_t subif)
{
	return (subif >> VAP_OFFSET) & VAP_MASK;
}

static inline unsigned int set_vap(int vap)
{
	return (vap & VAP_MASK) << VAP_OFFSET;
}

#endif				/*DATAPATH_H */
