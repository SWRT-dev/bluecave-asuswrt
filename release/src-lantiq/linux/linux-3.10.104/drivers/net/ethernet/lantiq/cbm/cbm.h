#ifndef __CBM_COMMON_H__
#define __CBM_COMMON_H__
#include <linux/unistd.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/err.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/sysctl.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/clk.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/irq.h>
#include <linux/seq_file.h>
#include <linux/semaphore.h>


#include <lantiq.h>
#include <lantiq_soc.h>
#include <net/lantiq_cbm.h>
#include <linux/netdevice.h>
#include <net/lantiq_cbm_api.h>
#include <net/datapath_api.h>
#include "cbm.h"
#include "reg/cbm_ls.h"
#include "reg/cbm.h"
#include "reg/cbm_dqm.h"
#include "reg/cbm_eqm.h"
#include "reg/cbm_desc64b.h"
#include "reg/fsqm.h"
#include <net/drv_tmu_ll.h>

/*#define CBM_DEBUG 1*/
/*#define CBM_DEBUG_LVL_1 1*/

/************************
* ENUM
***********************/
enum {
	SPREAD_WRR = 0,
	SPREAD_FLOWID = 1,
	SPREAD_MAX,
};

enum {
	CBM1_INT_VPE0 = 174,
	CBM1_INT_VPE1 = 175,
	CBM1_INT_VPE2 = 176,
	CBM1_INT_VPE3 = 177,
	CBM2_INT_VPE0 = 178,
	CBM2_INT_VPE1 = 179,
	CBM2_INT_VPE2 = 180,
	CBM2_INT_VPE3 = 181,
};
enum {
	DQM_DMA = 1,
	EQM_DMA = 2
};
enum {
	SBA0_STD = 1,
	SBA0_JBO = 2,
	SBA1_STD = 3,
	SBA1_JBO = 4,
};
enum CBM_EGP_TYPE {
	CPU_EG_PORT = 0,
	MPE_EG_PORT,
	WAVE500_PORT,
	LRO_PORT,
	GSWIPL_LP_P1,
	GSWIPL_LP_P2,
	GSWIPL_LP_P3,
	GSWIPL_LP_P4,
	GSWIPL_LP_P5,
	GSWIPL_LP_P6,
	GSWIPL_HP_P1,
	GSWIPL_HP_P2,
	GSWIPL_HP_P3,
	GSWIPL_HP_P4,
	GSWIPL_HP_P5,
	GSWIPL_HP_P6,
	/*    GSWIPR_VRX318,*/
	GSWIPR_REDIRECT,
	GSWIPR_ETHWAN,
	VRX318_PORT,
	MAX_DQM_PORT,
};

/*! PMAC port flag */
enum FREE_FLAG {
	P_FREE = 0,         /*! The port is free */
	P_ALLOCATED,      /*! the port is already allocated to some driver, but not registered or no need to register at all.\n
							 for example, LRO/CAPWA, only need to allocate, but no need to register */
	P_REGISTERED,     /*! Registered already. */

	P_FLAG_NO_VALID   /*! Not valid flag */
};

enum DQM_PORT_TYPE {
	DQM_CPU_START_ID = 0,
	DQM_CPU_END_ID = 4,
	DQM_SCPU_START_ID = 5,
	DQM_SCPU_END_ID = 5,
	DQM_DMA_START_ID = 6,
	DQM_DMA_END_ID = 22,
	DQM_LDMA_START_ID = 23,
	DQM_LDMA_END_ID = 23,
	DQM_WAVE_START_ID = 24,
	DQM_WAVE_END_ID = 26,
	DQM_CHECKSUM_ID = 20,
	DQM_DIRECTPATH_RX = 21
};
enum EQM_PORT_TYPE {
	EQM_CPU_START_ID = 0,
	EQM_CPU_END_ID = 3,
	EQM_DMA_START_ID = 5,
	EQM_DMA_END_ID = 8,
};
enum EQM_DQM_PORT_TYPE {
	DQM_CPU_TYPE = 0,
	DQM_SCPU_TYPE = 5,
	DQM_DMA_TYPE = 6,
	DQM_LDMA_TYPE = 23,
	DQM_WAVE_TYPE = 24,
	EQM_CPU_TYPE = 100,
	EQM_DMA_TYPE = 200,
	EQM_TOE_TYPE = 9,
	EQM_DL_TYPE = 12,
	EQM_VRX318_TYPE = 15,
};
#if 0
enum EQM_DQM_TYPE {
	DQM_CPU = 0,
	DQM_SCPU,
	DQM_DMA,
	DQM_LDMA,
	EQM_CPU,
	EQM_DMA,
	EQM_CPU,
};
#endif



/**************************************************
*
* Macros
*
***************************************************/

#define CONFIG_BRONZE_RTL307 1
#if 0
#define CONFIG_CBM_JBO_BUF_BASE 0x21F00000
#define CONFIG_CBM_JBO_BUF_SIZE 0x100000
#endif
/**************************
*Address Calculation
 **************************/
#define FSQM_LLT_RAM(fsqm_base, idx)    ((volatile uint32_t *)((fsqm_base) + RAM + ((idx) << 2)))
#define FSQM_RCNT(fsqm_base, idx)       ((volatile uint32_t *)((fsqm_base) + RCNT + ((idx) << 2)))
#define CBM_EQM_CPU_PORT(idx, reg)      (CFG_CPU_IGP_0 + (idx)*0x1000 + (offsetof(struct cbm_eqm_cpu_igp_reg, reg)))
#define CBM_EQM_DMA_PORT(idx, reg)       (CFG_DMA_IGP_5 + (idx)*0x1000 + (offsetof(struct cbm_eqm_dma_igp_reg, reg)))
#define CBM_DQM_CPU_PORT(idx, reg)       (CFG_CPU_EGP_0 + (idx)*0x1000 + (offsetof(struct cbm_dqm_cpu_egp_reg, reg)))
#define CBM_DQM_SCPU_PORT(idx, reg)       (CFG_CPU_EGP_0 + (idx)*0x1000 + (offsetof(struct cbm_dqm_scpu_egp_reg, reg)))
#define CBM_DQM_DMA_PORT(idx, reg)       (CFG_DMA_EGP_6 + (idx)*0x1000 + (offsetof(struct cbm_dqm_dma_egp_reg, reg)))
#define CBM_LS_PORT(idx, reg)            (LS_DESC_DW0_PORT0 + (idx)*0x100 + (offsetof(struct cbm_ls_reg, reg)))
#define CBM_IOCU_ADDR(addr)             (((addr) & 0x1FFFFFFF) | 0xC0000000)
#define CBM_INT_LINE(idx, reg)           ((idx)*0x40 + offsetof(struct cbm_int_reg, reg))
#define CBM_INT_LINE_DQM(idx, reg)           ((idx)*0x40 + offsetof(struct cbm_int_reg, reg))
#define CBM_EQM_DMA_DESC(pid, des_idx, jbo_flag)  (SDESC0_0_IGP_5 + ((pid)-5)*0x1000 + (des_idx)*16 + (jbo_flag)*0x800)
#define CBM_DQM_DMA_DESC(pid, des_idx)            (DESC0_0_EGP_5  + ((pid)-5)*0x1000 + (des_idx)*16)
#define CBM_EQM_LDMA_DESC(des_idx)	(SDESC0_0_IGP_15 + (des_idx) * 8)


/***********************
  * Default  Value Definition
  **********************/
#define DEFAULT_CPU_HWM         256
#define DEFAULT_DMA_HWM         128
#define DEFAULT_LS_QTHH         7
#define DEFAULT_LS_OVFL_CNT     0x2000
#define DEFAULT_LS_PORT_WEIGHT  2

#define CPU_EQM_PORT_NUM        4
#define SCPU_EQM_PORT_NUM       1
#define DMA_EQM_PORT_NUM        10
#define CPU_DQM_PORT_NUM        4
#define SCPU_DQM_PORT_NUM       1
#define DMA_DQM_PORT_NUM        17
#define LS_PORT_NUM             4
#define CBM_MAX_INT_LINES       8
#define CBM_DMA_DESC_OWN        1/*belong to DMA*/
#define CBM_DMA_DATA_OFFSET     128
#define DEFAULT_WAIT_CYCLES     20
#if 0
#define CBM_QIDT_MODULE_BASE    0x1E720000
#if defined(BRONZE_RTL307)
#define CBM_QEQCNTR_BASE        0x1E750000/*0x1E781000  //0x1E750000*/
#define CBM_QDQCNTR_BASE        0x1E750800/*0x1E7C1000  //0x1E750800*/
#else
#define CBM_QEQCNTR_BASE        0x1E781000/*0x1E750000*/
#define CBM_QDQCNTR_BASE        0x1E7C1000/*0x1E750800*/
#endif
#endif
#define CBM_QEQCNTR_SIZE        0x400
#define CBM_QDQCNTR_SIZE        0x400
#define CBM_BASIC_JBO_NUM       100
#define CBM_BASIC_STD_NUM       600
#define SBID_START 16
#define CBM_SUCCESS             0
#define CBM_FAILURE             -1
#define CBM_NOTFOUND             2
#define CBM_RESCHEDULE          4
#define CBM_PORT_MAX			64
#define CBM_EQM_PORT_MAX		16
#define CBM_DQM_DMA		1
#define PMAC_MAX_NUM 16
#define CBM_JUMBO_BUF_SPLIT_GUARD_BAND	16
#define CBM_MAX_RCNT 7
#define MAX_TMU_QUEUES 256

/*************************
  * Structure definition
 *************************/
#ifdef CONFIG_CPU_BIG_ENDIAN
struct flowid_desc {
	uint8_t row:5;
	uint8_t col:3;
};
#else
struct flowid_desc {
	uint8_t col:3;
	uint8_t row:5;
};
#endif
struct cbm_scpu_ptr_rtn {
	uint32_t ptr_rtn;
};

#ifdef CONFIG_CPU_LITTLE_ENDIAN
struct dma_desc{
	/* DWORD 0 */
	uint32_t dest_sub_if_id:15;
	uint32_t eth_type:2;
	uint32_t flow_id:8;
	uint32_t tunnel_id:4;
	uint32_t resv0:3;

	/* DWORD 1 */
	uint32_t classid:4;
	uint32_t resv1:4;
	uint32_t ep:4;
	uint32_t color:2;
	uint32_t mpe1:1;
	uint32_t mpe2:1;
	uint32_t enc:1;
	uint32_t dec:1;
	uint32_t nat:1;
	uint32_t tcp_err:1;
	uint32_t session_id:12;

	/*DWORD 2 */
	uint32_t data_ptr;

	/*DWORD 3*/
	uint32_t data_len:16;
	uint32_t mpoa_mode:2;
	uint32_t mpoa_pt:1;
	uint32_t qid:4;
	uint32_t byte_offset:3;
	uint32_t pdu_type:1;
	uint32_t dic:1;
	uint32_t eop:1;
	uint32_t sop:1;
	uint32_t c:1;
	uint32_t own:1;

};

#else
struct dma_desc{
	/* DWORD 1 */
	uint32_t session_id:12;
	uint32_t tcp_err:1;
	uint32_t nat:1;
	uint32_t dec:1;
	uint32_t enc:1;
	uint32_t mpe2:1;
	uint32_t mpe1:1;
	uint32_t color:2;
	uint32_t ep:4;
	uint32_t resv1:4;
	uint32_t classid:4;

	/* DWORD 0 */
	uint32_t resv0:3;
	uint32_t tunnel_id:4;
	uint32_t flow_id:8;
	uint32_t eth_type:2;
	uint32_t dest_sub_if_id:15;



	/*DWORD 3*/
	uint32_t own:1;
	uint32_t c:1;
	uint32_t sop:1;
	uint32_t eop:1;
	uint32_t dic:1;
	uint32_t pdu_type:1;
	uint32_t byte_offset:3;
	uint32_t qid:4;
	uint32_t mpoa_pt:1;
	uint32_t mpoa_mode:2;
	uint32_t data_len:16;

	/*DWORD 2 */
	uint32_t data_ptr;

};

struct cbm_ctrl {
	int id;
	struct device *dev;
	struct net_device dummy_dev;
	struct napi_struct napi;
	u32 jumbo_size_mask;
};

#endif


struct cbm_desc_list{
	struct cbm_desc desc;
	struct cbm_desc_list *next;
};

struct cbm_qidt_mask {
	uint32_t classid_mask;
	uint32_t ep_mask;
	uint32_t mpe1_mask;
	uint32_t mpe2_mask;
	uint32_t enc_mask;
	uint32_t dec_mask;
	uint32_t flowid_lmask;
	uint32_t flowid_hmask;
};

struct cbm_qidt_elm {
	uint32_t clsid;
	uint32_t ep;
	uint32_t mpe1;
	uint32_t mpe2;
	uint32_t enc;
	uint32_t dec;
	uint32_t flowidl;
	uint32_t flowidh;
};
struct cbm_eqm_cpu_igp_reg{
	uint32_t cfg;
	uint32_t wm;
	uint32_t pocc;
	uint32_t eqpc;
	struct cbm_desc disc;
	uint32_t irncr;  /*Interrupt Capture :  Status + Clear */
	uint32_t irnicr;
	uint32_t irnen;  /*Interrupt Enable Mask */
	uint32_t resv0[2];
	uint32_t rcnt;
	uint32_t dicc;
	uint32_t rcntc;
	uint32_t nsbpc;
	uint32_t njbpc;
	uint32_t resv1[1];
	uint32_t dcntr;
	uint32_t resv2[12];
	uint32_t new_sptr;
	uint32_t resv3[3];
	uint32_t new_jptr;
	uint32_t resv4[27];

	struct cbm_desc desc0;
	struct cbm_desc desc1;
};

struct cbm_eqm_dma_igp_reg{
	uint32_t cfg;
	uint32_t wm;
	uint32_t pocc;
	uint32_t eqpc;
	struct cbm_desc disc;
	uint32_t irncr;
	uint32_t irnicr;
	uint32_t irnen;
	uint32_t resv0;
	uint32_t dptr;
	uint32_t resv1;
	uint32_t dicc;
};

struct cbm_dqm_cpu_egp_reg{
	uint32_t cfg;
	uint32_t dqpc;
	uint32_t resv0[6];
	uint32_t irncr;
	uint32_t irnicr;
	uint32_t irnen;
	uint32_t resv1;
	uint32_t dptr;
	uint32_t bprc;
	uint32_t resv2[18];
	uint32_t ptr_rtn;
	uint32_t resv3[31];
	struct cbm_desc desc0;
	struct cbm_desc desc1;
};

struct cbm_dqm_scpu_egp_reg{
	uint32_t cfg;
	uint32_t dqpc;
	uint32_t resv0[6];
	uint32_t irncr;
	uint32_t irnicr;
	uint32_t irnen;
	uint32_t resv1;
	uint32_t dptr;
	uint32_t bprc;
	uint32_t brptr;
	uint32_t resv2[17];
	struct cbm_scpu_ptr_rtn scpu_ptr_rtn[32];
	struct cbm_desc desc[32];
};

struct cbm_dqm_dma_egp_reg{
	uint32_t cfg;
	uint32_t dqpc;
	uint32_t resv0[6];
	uint32_t irncr;
	uint32_t irnicr;
	uint32_t irnen;
	uint32_t resv1;
	uint32_t dptr;
};

struct cbm_ls_reg{
	struct cbm_desc desc;
	uint32_t ctrl;
	uint32_t status;
	uint32_t resv0[2];
	struct cbm_desc qdesc[7];
};

struct cbm_int_reg{
	uint32_t cbm_irncr;
	uint32_t cbm_irnicr;
	uint32_t cbm_irnen;
	uint32_t resv0[1];
	uint32_t igp_irncr;
	uint32_t igp_irnicr;
	uint32_t igp_irnen;
	uint32_t resv1[1];
	uint32_t egp_irncr;
	uint32_t egp_irnicr;
	uint32_t egp_irnen;
};

struct flowID {
#ifdef CONFIG_CPU_LITTLE_ENDIAN

	uint32_t q0:4;
	uint32_t q1:4;
	uint32_t q2:4;
	uint32_t q3:4;
	uint32_t q4:4;
	uint32_t q5:4;
	uint32_t q6:4;
	uint32_t q7:4;

#else

	uint32_t q7:4;
	uint32_t q6:4;
	uint32_t q5:4;
	uint32_t q4:4;
	uint32_t q3:4;
	uint32_t q2:4;
	uint32_t q1:4;
	uint32_t q0:4;
};
#endif
struct cbm_egp_tmu_map {
	uint32_t egp_type;
	uint32_t qid;/* qid value*/
	/*uint32_t q_num;*//*total queue number*/
	uint32_t inc;/*increase qid with class id*/
	struct cbm_qidt_elm qidt_elm;
	struct cbm_qidt_mask qidt_mask;
};
struct cbm_egp_map {
	uint32_t epg;
	uint32_t pmac;
	uint32_t port_type;
};
/*! CBM port information*/
struct cbm_dqm_port_info {
	uint32_t dma_ch;
	uint32_t dma_dt_ch;
	uint32_t dma_dt_ctrl;
	uint32_t dma_dt_init_type;
	uint32_t def_qid;
	uint32_t def_schd;
	uint32_t def_tmu_pid;
	void		*cbm_buf_free_base;
	uint32_t	num_free_entries;/*!< Number of Free Port entries */
	cbm_dq_info_t	deq_info;
	uint32_t egp_type;
};

struct cbm_eqm_port_info {
	uint32_t port_type;
	int		num_eq_ports;/*!< Number of Enqueue Ports */
	cbm_eq_info_t	eq_info;/*!<  EQ port info */
	uint32_t dma_dt_std_ch;
	uint32_t dma_dt_jum_ch;
	uint32_t dma_dt_ctrl;
	uint32_t dma_dt_init_type;
};

struct cbm_pmac_port_map{
	enum FREE_FLAG  flags;/*! port flag */
	struct module *owner;
	struct net_device *dev;
	uint32_t dev_port;
	uint32_t pmac;
	uint32_t egp_port_map;/*bit map to egp port*/
	uint32_t qid_num;/*queue numbers allocated to that pmac port*/
	uint32_t qids[16];/*qid array*/
	uint32_t egp_type;/* e.g. DP_F_FAST_ETH_LAN/DP_F_FAST_ETH_WAN/DP_F_DIRECT/High priority/Low priority*/
	struct list_head list;

};
struct rcnt_idx {
	uint32_t fsqm_idx;
	uint32_t frm_size;
	uint32_t rcnt_base;
	uint32_t buf_addr;
};

struct cbm_cntr_mode {
void __iomem *reg;
uint32_t msel_mask;
uint32_t msel_pos;
uint32_t qen_mask;
uint32_t qen_pos;
void (*reset_counter)(void);
};

struct cbm_base_addr {
	void __iomem *tmu_addr_base;
	void __iomem *cbm_addr_base;
	void __iomem *fsqm0_addr_base;
	void __iomem *fsqm1_addr_base;
	void __iomem *cbm_eqm_addr_base;
	void __iomem *cbm_dqm_addr_base;
	void __iomem *cbm_ls_addr_base;
	void __iomem *cbm_qidt_addr_base;
	void __iomem *cbm_qeqcnt_addr_base;
	void __iomem *cbm_qdqcnt_addr_base;
	void __iomem *cbm_dmadesc_addr_base;
};

struct cbm_buff_info {
	unsigned int std_buf_addr;
	unsigned int jbo_buf_addr;
	unsigned int std_buf_size;
	unsigned int jbo_buf_size;
	unsigned int std_frm_num;
	unsigned int jbo_frm_num;
	unsigned int std_frm_size;
	unsigned int jbo_frm_size;
	unsigned int std_fsqm_idx;
	unsigned int jbo_fsqm_idx;
};

struct cbm_jumbo_divide {
int32_t jumbo_buf_used;
void *curr_jumbo_buf;
int32_t curr_jumbo_buf_bytes_used;
int32_t curr_jumbo_buf_alloc_no;
};

struct cbm_qidt_shadow {
uint32_t qidt_shadow;
uint32_t qidt_drop_flag;
};

struct cbm_qlink_get {
uint32_t qid[EGRESS_QUEUE_ID_MAX];
uint32_t qlink_sbin[EGRESS_QUEUE_ID_MAX];
};

struct cbm_q_info {
uint16_t      refcnt; /* No of Queue Map table entries pointing to this q */
uint16_t       qmap_idx_start; /* First index of Queue Map table pointing to q */
uint16_t       qmap_idx_end; /* last index of Queue Map table pointing to q */
};

/************************
 * Function prototypes
 ************************/
#define cbm_assert(cond, fmt, arg...)  do { if (!(cond)) printk("%d:%s:" fmt "\n", __LINE__, __func__, ##arg); } while (0)
#define cbm_err(fmt, arg...)          do { printk("%d:%s:"fmt "\n", __LINE__, __func__, ##arg); } while (0)

#ifdef CBM_DEBUG
#define cbm_debug(fmt, arg...)        do { pr_info(fmt, ##arg); } while (0)
#else
#define cbm_debug(fmt, arg...)
#endif

#ifdef CBM_DEBUG_LVL_1
#define cbm_debug_1(fmt, arg...)        do { pr_info(fmt, ##arg); } while (0)
#else
#define cbm_debug_1(fmt, arg...)
#endif


int cbm_set_cpu_alloc_offset(uint32_t pid, uint32_t jbo, unsigned int offset);
void dump_fsqm_regs(int idx);
void dump_cbm_basic_regs(uint32_t flag);
uint32_t cbm_get_total_buf_num(uint32_t flag);
extern void dump_fsqm_regs(int);
extern uint32_t cbm_get_total_buf_num(uint32_t);
extern void dump_fsqm_llt(uint32_t);
extern void cbm_hw_init(void);

void cbm_eqm_enable_bkpr(uint32_t pid, uint32_t enable);
void ls_intr_ctrl(uint32_t val);
void fsqm_intr_ctrl(uint32_t idx, uint32_t val);
void eqm_intr_ctrl(uint32_t val);
void dqm_intr_ctrl(uint32_t val);
uint32_t get_violation_ptr(uint32_t fsqm_idx);
void enable_rcnt_bkpr(uint32_t pid, uint32_t enable);
void cbm_ls_spread_alg_set(uint32_t alg);
void cbm_ls_port_weight_set(uint32_t port_id, uint32_t weight);
uint32_t cbm_ls_port_weight_get(uint32_t port_id);
void cbm_ls_flowId_map_set(uint32_t col, uint32_t val);
uint32_t cbm_ls_flowId_map_get(uint32_t col);
void dump_cbm_ls_regs(void);
void dump_cbm_desc(struct cbm_desc *desc, int detail);
int cbm_interrupt_init(void);
void cbm_fsqm_check_wm(uint32_t flag);
int cbm_ptr_refcnt_incr(uint32_t pid, uint32_t buf_ptr);

void dump_cbm_iqm_cpu_regs(int pid);
int32_t get_cnt_of_ptr(uint32_t buf_addr);

void init_fsqm_buf_jumbo(unsigned int jbo_base_addr, unsigned int size);
void init_fsqm_buf_standard(unsigned int std_base_addr, unsigned int size);
void buf_addr_adjust(unsigned int buf_base_addr, unsigned int buf_size, unsigned int *adjusted_buf_base, unsigned int *adjusted_buf_size, unsigned int align);
void cbm_intr_mapping_init(void);
void cbm_intr_mapping_uninit(void);
int cbm_clear_cpu_eqm_cnt(uint32_t pid);
void dump_cbm_cpu_eqm_cnt(uint32_t pid);
void cbm_dw_memset(uint32_t *base, int val, uint32_t size);
void cbm_qidt_set(const struct cbm_qidt_elm *qid_set, const struct cbm_qidt_mask *qid_mask, uint8_t qid_val);
int cbm_ptr_refcnt_incr(uint32_t pid, uint32_t buf_ptr);
/*int cbm_setup_desc(struct cbm_desc *desc, uint32_t data_ptr, uint32_t data_len, uint32_t ep,
				uint32_t classid, uint32_t mpe1, uint32_t mpe2, uint32_t enc, uint32_t dec, uint8_t flow_id, uint32_t dic,
						uint32_t color, uint32_t tunnel_id);*/
extern void cbm_enable_port(int pid);
extern int check_ptr_validation(uint32_t);

struct cbm_pmac_port_map *add_to_list(
struct cbm_pmac_port_map *value
);


struct cbm_pmac_port_map *is_port_allocated(
int32_t pmac, uint32_t flags);

struct cbm_pmac_port_map *search_in_list(
int32_t pmac, uint32_t flags,
struct cbm_pmac_port_map **prev);

int delete_from_list(
int32_t pmac, uint32_t flags);
int get_budget(void);
void cbm_restore_qmap(int enable, int qid);

#endif /* __CBM_COMMON_H__ */

