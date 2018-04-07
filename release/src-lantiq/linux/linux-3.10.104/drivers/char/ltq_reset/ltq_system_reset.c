/******************************************************************************
**
** FILE NAME	: ltq_system_reset.c
** PROJECT		: grx500 driver
** MODULES		: System Reset (System Reset Driver)
**
** DATE			: 21 October 2014
** AUTHOR		: None
** DESCRIPTION	: System reset driver
**
** HISTORY
**	$Date	$Author		$Comment
*******************************************************************************/
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/io.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <linux/kallsyms.h>
#include <linux/delay.h>
#include <linux/list.h>
#include <linux/seq_file.h>
#include <asm/cpu-info.h>
#include <linux/cpu.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/completion.h>
#include <linux/list.h>
#include <linux/jiffies.h>


#define LTQ_SYSRST_SUPPORT_SECURE_OS	1


#define SUPPORT_ICC_DRIVER 1



#if defined(CONFIG_SOC_GRX500_BOOTCORE)
#define SUPPORT_NGI_DRIVER 1
#define SUPPORT_RCU_DRIVER 1
#else
#undef SUPPORT_NGI_DRIVER
#undef SUPPORT_RCU_DRIVER
#endif
/*
 *	Chip Specific Head File
 */
#include <lantiq_soc.h>
#ifdef SUPPORT_RCU_DRIVER
#include "./ltq_rcu/ltq_rcu.h"
#endif /* SUPPORT_RCU_DRIVER */
#ifdef SUPPORT_NGI_DRIVER
#include "../../../arch/mips/lantiq/grx500_bootcore/ngi.h"
#include "../../../arch/mips/lantiq/grx500_bootcore/ngi_map.h"
#endif /* SUPPORT_NGI_DRIVER */
#ifdef SUPPORT_ICC_DRIVER
#include "../ltq_mps2/drv_mps.h"
#include "../ltq_icc/drv_icc.h"
#endif /* SUPPORT_ICC_DRIVER */


#include <linux/ltq_system_reset.h>
/*
 * ##########################################
 *	   System Reset Driver	Version No.
 *
 * SYSTEM_RESET_VER   MAJOR:MID:MINOR
 * ##########################################
 */
#define SYSRST_VER_MAJOR				1
#define SYSRST_VER_MID					0
#define SYSRST_VER_MINOR				0
#define SYSRST_MAJOR					242
#define SYSRST_DEVICE_NAME				"sysrst"
#define SYSRST_PROC_DIR					"sysrst"
#define ENABLE_DEBUG_PRINT					  1
#define SYSRST_TIMEOUT_MS               2000
#define NUM_ENTITY(x)	(sizeof(x) / sizeof(*(x)))
/*
 *	Debug Print Mask
 */
#define DBG_ENABLE_MASK_ERR			(1 << 0)
#define DBG_ENABLE_MASK_DEBUG_PRINT	(1 << 1)
#define DBG_ENABLE_MASK_ALL (DBG_ENABLE_MASK_ERR\
					| DBG_ENABLE_MASK_DEBUG_PRINT)
#define sysrst_err(format, arg...)	do {\
	if ((g_dbg_enable & DBG_ENABLE_MASK_ERR))\
		pr_err("%s: " format "\n", __func__, ##arg);\
	} while (0)
#define sysrst_msg(format, arg...)	do {\
	if (1)\
		pr_info("%s: " format, __func__, ##arg);\
	} while (0)

#if defined(ENABLE_DEBUG_PRINT) && ENABLE_DEBUG_PRINT
	#undef  sysrst_dbg
	#define sysrst_dbg(format, arg...)	do {\
		if ((g_dbg_enable & \
			DBG_ENABLE_MASK_DEBUG_PRINT))\
			pr_info(\
			"%s: " format "\n", \
			__func__, ##arg); \
	} while (0)
#else
  #if !defined(sysrst_dbg)
	#define sysrst_dbg(format, arg...)
  #endif
#endif

/* Functions and structure support dbg */
struct sysrst_dgb_info {
	char *cmd;
	char *description;
	uint32_t flag;
};
static struct sysrst_dgb_info sysrst_dbg_enable_mask_str[] = {
	{"err", "error print", DBG_ENABLE_MASK_ERR },
	{"dbg", "debug print", DBG_ENABLE_MASK_DEBUG_PRINT},
	/*the last one */
	{"all", "enable all debug", -1}
};


enum sysrst_cpu_env {
	SYSRST_CPU_4KEC = 0,
	SYSRST_INTERAPTIVE = 1,
};

struct sysrst_async_data_t {
	unsigned int				domain_id;
	unsigned int				module_id;
	unsigned int				flags;
	unsigned int				arg;
	sysrst_rst_async_handler_t	callbackfn;
};
#ifdef SUPPORT_NGI_DRIVER

struct ngi_agent_core {
	unsigned int ngi_inst_id;
	unsigned int core_code;
};

struct ngi_domain_t {
	unsigned int ngi_domain_id;
	struct ngi_agent_core ngi_core;
	char *core_code_name;
	unsigned int flag;
};


#define NGI_F_IA 1
#define NGI_F_TA 2
#define NGI_F_CORE 4

static struct ngi_domain_t g_ngi_domains[LTQ_NGI_MAX] = {
{
	/* TOE TA SSX7 */
	LTQ_NGI_DOMAIN_TA_TOE,
	{
		7,
		0x73
	},
	"TA_TOE",
	NGI_F_TA
},
{
	/* MPE TA SSX7 */
	LTQ_NGI_DOMAIN_TA_MPE,
	{
		7,
		0x65
	},
	"TA_MPE",
	NGI_F_TA
},
{
	/* MemMax - DDR TA SSX0 */
	LTQ_NGI_DOMAIN_TA_DDR,
	{
		0,
		0x65
	},
	"TA_DDR",
	NGI_F_TA
},
{
	/* CBM1 TA SSX0 */
	LTQ_NGI_DOMAIN_TA_CMB1,
	{
		0,
		0x9
	},
	"TA_CBM1",
	NGI_F_TA
},
{
	/* CBM2 TA SSX0 */
	LTQ_NGI_DOMAIN_TA_CMB2,
	{
		0,
		0x109
	},
	"TA_CBM2",
	NGI_F_TA
},
{
	/* DMA4 TA SSX0 */
	LTQ_NGI_DOMAIN_TA_DMA4,
	{
		0,
		0xd
	},
	"TA_DMA4",
	NGI_F_TA
},
{
	/* DMA4 IA SSX0 */
	LTQ_NGI_DOMAIN_IA_DMA4,
	{
		0,
		0xd
	},
	"IA_DMA4",
	NGI_F_IA
},
{
	/* DMA3 TA SSX0 */
	LTQ_NGI_DOMAIN_TA_DMA3,
	{
		0,
		0xc
	},
	"TA_DMA3",
	NGI_F_TA
},
{
	/* DMA3 IA SSX0 */
	LTQ_NGI_DOMAIN_IA_DMA3,
	{
		0,
		0xc
	},
	"IA_DMA3",
	NGI_F_IA
},
{
	/* EIP-123 TA SSX0 */
	LTQ_NGI_DOMAIN_TA_EIP_123,
	{
		0,
		0xf
	},
	"TA_EIP123",
	NGI_F_TA
},
{
	/* EIP-97 TA SSX0 */
	LTQ_NGI_DOMAIN_TA_EIP_97,
	{
		0,
		0xe
	},
	"TA_EIP97",
	NGI_F_TA
},
{
	/* EX05 TA SSX0 */
	LTQ_NGI_DOMAIN_TA_EX05,
	{
		0,
		0xf05
	},
	"TA_EX05",
	NGI_F_TA
},
{
	/* OCP2OTP TA SSX0 */
	LTQ_NGI_DOMAIN_TA_OCP2OTP,
	{
		0,
		0x7
	},
	"TA_OTP",
	NGI_F_TA
},
{
	/* I2C TA SSX4 */
	LTQ_NGI_DOMAIN_TA_I2C,
	{
		4,
		0x47
	},
	"TA_I2C",
	NGI_F_TA
},
{
	/* UART0 - ASC0 TA SSX4 */
	LTQ_NGI_DOMAIN_TA_UART0,
	{
		4,
		0x48
	},
	"TA_ASC0",
	NGI_F_TA
},
{
	/* UART1 - ASC1 TA SSX4 */
	LTQ_NGI_DOMAIN_TA_UART1,
	{
		4,
		0x49
	},
	"TA_ASC1",
	NGI_F_TA
},
{
	/* GPT0 - GPTC0 TA SSX4 */
	LTQ_NGI_DOMAIN_TA_GPTC0,
	{
		4,
		0x4A
	},
	"TA_GPT0",
	NGI_F_TA
},
{
	/* GPT1 - GPTC1 TA SSX4 */
	LTQ_NGI_DOMAIN_TA_GPTC1,
	{
		4,
		0x4B
	},
	"TA_GPT1",
	NGI_F_TA
},
{
	/* GPT2 - GPTC2 TA SSX4 */
	LTQ_NGI_DOMAIN_TA_GPTC2,
	{
		4,
		0x4C
	},
	"TA_GPT2",
	NGI_F_TA
},
{
	/* SSC0 - SPI0 TA SSX4 */
	LTQ_NGI_DOMAIN_TA_SPI0,
	{
		4,
		0x4d
	},
	"TA_SSC0",
	NGI_F_TA
},
{
	/* SSC1 - SPI1 TA SSX4 */
	LTQ_NGI_DOMAIN_TA_SPI1,
	{
		4,
		0x4e
	},
	"TA_SSC1",
	NGI_F_TA
},
{
	/* DMA0 TA SSX4 */
	LTQ_NGI_DOMAIN_TA_DMA0,
	{
		4,
		0x4F
	},
	"TA_DMA0",
	NGI_F_TA
},
{
	/* DMA0 IA SSX4 */
	LTQ_NGI_DOMAIN_IA_DMA0,
	{
		0,
		0x4F
	},
	"IA_DMA0",
	NGI_F_IA
},
{
	/* EBU TA SSX4 */
	LTQ_NGI_DOMAIN_TA_EBU,
	{
		4,
		0x50
	},
	"TA_EBU",
	NGI_F_TA
},
{
	/* HSNAND TA SSX4 */
	LTQ_NGI_DOMAIN_TA_HSNAND,
	{
		4,
		0x00
	},
	"unknown",
	NGI_F_TA
},
{
	/* DMA2 TX TA SSX1 */
	LTQ_NGI_DOMAIN_TA_DMA2_TX,
	{
		1,
		0x13
	},
	"TA_DMA2T",
	NGI_F_TA
},
{
	/* DMA2 TX IA SSX1 */
	LTQ_NGI_DOMAIN_IA_DMA2_TX,
	{
		1,
		0x13
	},
	"IA_DMA2T",
	NGI_F_IA
},
{
	/* DMA2 RX TA SSX1 */
	LTQ_NGI_DOMAIN_TA_DMA2_RX,
	{
		1,
		0x14
	},
	"TA_DMA2R",
	NGI_F_TA
},
{
	/* DMA2 RX IA SSX2 */
	LTQ_NGI_DOMAIN_IA_DMA2_RX,
	{
		1,
		0x14
	},
	"IA_DMA2R",
	NGI_F_IA
},
{
	/* GSWIP TA SSX1 */
	LTQ_NGI_DOMAIN_TA_GSWIP_L,
	{
		1,
		0x12
	},
	"TA_GSWIP_L",
	NGI_F_TA
},
{
	/* DMA1 TX TA SSX2 */
	LTQ_NGI_DOMAIN_TA_DMA1_TX,
	{
		2,
		0x23
	},
	"TA_DMA1T",
	NGI_F_TA
},
{
	/* DMA1 TX IA SSX2 */
	LTQ_NGI_DOMAIN_IA_DMA1_TX,
	{
		2,
		0x23
	},
	"IA_DMA1_T",
	NGI_F_IA
},
{
	/* DMA1 RX TA SSX2 */
	LTQ_NGI_DOMAIN_TA_DMA1_RX,
	{
		2,
		0x24
	},
	"TA_DMA1R",
	NGI_F_TA
},
{
	/* DMA1 RX IA SSX2 */
	LTQ_NGI_DOMAIN_IA_DMA1_RX,
	{
		2,
		0x24
	},
	"IA_DMA1_R",
	NGI_F_IA
},
{
	/* GSWIP TA SSX2 */
	LTQ_NGI_DOMAIN_TA_GSWIP_R,
	{
		2,
		0x22
	},
	"TA_GSWIP_R",
	NGI_F_TA
},
{
	/* PCIE1 TA, SSX3 */
	LTQ_NGI_DOMAIN_TA_PCIE1,
	{
		3,
		0x36
	},
	"TA_PCIE1",
	NGI_F_TA
},
{
	/* PCIE1 TA A, SSX3 */
	LTQ_NGI_DOMAIN_TA_PCIE1_A,
	{
		3,
		0x136
	},
	"TA_PCIE1_A",
	NGI_F_TA
},
{
	/* PCIE1 TA C, SSX3 */
	LTQ_NGI_DOMAIN_TA_PCIE1_C,
	{
		3,
		0x38
	},
	"TA_PCIE1_C",
	NGI_F_TA
},
{
	/* PCIE1 IA  W, SSX3 */
	LTQ_NGI_DOMAIN_IA_PCIE1_W,
	{
		3,
		0x136
	},
	"IA_PCIE1_W",
	NGI_F_IA
},
{
	/* PCIE1 IA  R, SSX3 */
	LTQ_NGI_DOMAIN_IA_PCIE1_R,
	{
		3,
		0x36
	},
	"IA_PCIE1_R",
	NGI_F_IA
},
{
	/* PCIE2 TA , SSX3 */
	LTQ_NGI_DOMAIN_TA_PCIE2,
	{
		3,
		0x32
	},
	"TA_PCIE2",
	NGI_F_TA
},
{
	/* PCIE2 TA App , SSX3 */
	LTQ_NGI_DOMAIN_TA_PCIE2_A,
	{
		3,
		0x132
	},
	"TA_PCIE2_A",
	NGI_F_TA
},
{
	/* PCIE2 TA CR Bridge, SSX3 */
	LTQ_NGI_DOMAIN_TA_PCIE2_C,
	{
		3,
		0x34
	},
	"TA_PCIE2_C",
	NGI_F_TA
},
{
	/* PCIE2 IA W, SSX3 */
	LTQ_NGI_DOMAIN_IA_PCIE2_W,
	{
		3,
		0x132
	},
	"IA_PCIE2_W",
	NGI_F_IA
},
{
	/* PCIE2 IA R, SSX3 */
	LTQ_NGI_DOMAIN_IA_PCIE2_R,
	{
		3,
		0x32
	},
	"IA_PCIE2_R",
	NGI_F_IA
},
{
	/* PCIE3 TA, SSX3 */
	LTQ_NGI_DOMAIN_TA_PCIE3,
	{
		3,
		0x3a
	},
	"TA_PCIE3",
	NGI_F_TA
},
{
	/* PCIE3 TA A, SSX3 */
	LTQ_NGI_DOMAIN_TA_PCIE3_A,
	{
		3,
		0x13a
	},
	"TA_PCIE3_A",
	NGI_F_TA
},
{
	/* PCIE3 TA C, SSX3 */
	LTQ_NGI_DOMAIN_TA_PCIE3_C,
	{
		3,
		0x3c
	},
	"TA_PCIE3_C",
	NGI_F_TA
},
{
	/* PCIE3 IA  W, SSX3 */
	LTQ_NGI_DOMAIN_IA_PCIE3_W,
	{
		3,
		0x13a
	},
	"IA_PCIE3_W",
	NGI_F_IA
},
{
	/* PCIE3 IA  R, SSX3 */
	LTQ_NGI_DOMAIN_IA_PCIE3_R,
	{
		3,
		0x3a
	},
	"IA_PCIE3_R",
	NGI_F_IA
}
};
#endif /* SUPPORT_NGI_DRIVER */

/*
 * ####################################
 *			  Global Variables
 * ####################################
*/
/* global spinlock to protect synchronous system reset function */
static struct semaphore g_sysrst_lock;
/* spinlock for calling to rcu driver */
static spinlock_t g_sysrst_rcu_lock;
/* spinlock for calling to ngi driver */
static spinlock_t g_sysrst_ngi_lock;
static enum sysrst_cpu_env g_current_cpu_env = SYSRST_CPU_4KEC;
static int g_dbg_enable = DBG_ENABLE_MASK_ERR;

#ifdef SUPPORT_ICC_DRIVER

#define FASTBUF_FW_OWNED 0x00000002

#define ICC_RET_OK 0
#define ICC_RET_ERROR 1

#define ICC_COMMAND_RESET_INVALID -1
#define ICC_COMMAND_RESET_REQ 0
#define ICC_COMMAND_CLEAR_REQ 1
#define ICC_COMMAND_GET_REQ 2


#define ICC_COMMAND_RESET_REPLY (0 | 0x80)
#define ICC_COMMAND_CLEAR_REPLY (1 | 0x80)
#define ICC_COMMAND_GET_REPLY (2 | 0x80)

icc_msg_t g_icc_rw;
icc_msg_t g_icc_write;

struct icc_param_t {
	int transaction_id;
	int command;
	int result;
	int reset_domain_id;
	int module_id;
};

struct icc_transaction_t {
	struct list_head list;
	int	   transaction_id;
	int result;
	struct completion comp;
};

static unsigned int g_transaction_id;
struct semaphore sem_transaction;
LIST_HEAD(g_icc_transaction);

#endif

/* declare system domain name */
static	SYSRST_DECLARE_DOMAIN_NAME(g_system_domain_name);
static	SYSRST_DECLARE_MODULE_NAME(g_system_module_name);
#ifdef SUPPORT_NGI_DRIVER
static LTQ_NGI_DECLARE_NAME(g_system_ngi_name);
#endif /* SUPPORT_NGI_DRIVER */

/*
 * ####################################
 *				 Global SysRST Domains
 * ####################################
 */
static struct sysrst_domain_t g_sysrst_domains[SYSRST_DOMAIN_MAX] = {
{
/* SYSRST_DOMAIN_ONBOARD_PERIPHERAL
* Goes as HRST to outside SoC peripheral reset on the
* mainboard.
*/
.affected_domains = {SYSRST_DOMAIN_ONBOARD_PERIPHERAL,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains = {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains = {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_HRST,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers = NULL,
},
{
/* SYSRST_DOMAIN_TEP
* Reset goes to MIPS4kec for Self-clean Reset
*/
.affected_domains = {SYSRST_DOMAIN_TEP,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains = {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains = {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_TEP,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers = NULL,
},
{
/* SYSRST_DOMAIN_CPS_SUBSYSTEM
* Reset goes to SI_CPCReset. CPC handles sub system reset
* sequence.
*/
.affected_domains = {SYSRST_DOMAIN_CPS_SUBSYSTEM,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains = {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_CPU_CLUSTER_RST,
	LTQ_RCU_DOMAIN_CPU_CLUSTER_RLS,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_TOE
* Interconnect: NGI
*/
.affected_domains		= {SYSRST_DOMAIN_TOE,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_TOE,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_TMU
* TMU Controller reset through RST_REQ of the CBM which
* will cause a CBM reset including a TMU reset. One needs to
* ensure CBM EQM and DQM are disabled before TMU is reset.
*/
.affected_domains		= {SYSRST_DOMAIN_TMU,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_CBM,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_MPE
* MPE reset through NGI.
*/
.affected_domains		= {SYSRST_DOMAIN_MPE,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_MPE,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_PCIE_PHY3 */
.affected_domains		= {SYSRST_DOMAIN_PCIE_PHY3,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_PCIE_PHY3,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_PCIE_PHY1 */
.affected_domains		= {SYSRST_DOMAIN_PCIE_PHY1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_PCIE_PHY1,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_PCIE_PHY2 */
.affected_domains		= {SYSRST_DOMAIN_PCIE_PHY2,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_PCIE_PHY2,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/*	SYSRST_DOMAIN_XBAR6 */
.affected_domains		= {SYSRST_DOMAIN_XBAR6,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_XBAR6,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_GSWIP_L */
.affected_domains		= {SYSRST_DOMAIN_GSWIP_L,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_GSWIP_L,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_GSWIP_L,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_WLAN */
.affected_domains		= {SYSRST_DOMAIN_WLAN,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_WLAN,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_PHY5 */
.affected_domains		= {SYSRST_DOMAIN_PHY5,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_PHY5,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_PHY4 */
.affected_domains		= {SYSRST_DOMAIN_PHY4,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_PHY4,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_PHY3 */
.affected_domains		= {SYSRST_DOMAIN_PHY3,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_PHY3,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_PHY2 */
.affected_domains		= {SYSRST_DOMAIN_PHY2,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_PHY2,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/*	SYSRST_DOMAIN_GLB_SW */
.affected_domains		= {SYSRST_DOMAIN_GLB_SW,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_GLB_SW,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/*	SYSRST_DOMAIN_PHY6F */
.affected_domains		= {SYSRST_DOMAIN_PHY6F,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_PHY6F,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_VCODEC */
.affected_domains		= {SYSRST_DOMAIN_VCODEC,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_VCODEC,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_OCP2SRAM */
.affected_domains		= {SYSRST_DOMAIN_OCP2SRAM,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_OCP2SRAM,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_XBAR7 */
.affected_domains		= {SYSRST_DOMAIN_XBAR7,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_XBAR7,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_SL40 */
.affected_domains		= {SYSRST_DOMAIN_SL40,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_SL40,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_USB_PHY0 */
.affected_domains		= {SYSRST_DOMAIN_USB_PHY0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_USB_PHY0,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_USB_PHY1 */
.affected_domains		= {SYSRST_DOMAIN_USB_PHY1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_USB_PHY1,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_SR_PHYF */
.affected_domains		= {SYSRST_DOMAIN_SR_PHYF,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_SR_PHYF,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_SR_PHY2 */
.affected_domains		= {SYSRST_DOMAIN_SR_PHY2,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_SR_PHY2,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_SR_PHY3 */
.affected_domains		= {SYSRST_DOMAIN_SR_PHY3,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_SR_PHY3,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_TEMPS */
.affected_domains		= {SYSRST_DOMAIN_TEMPS,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_TEMPS,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_SR_PHY5 */
.affected_domains		= {SYSRST_DOMAIN_SR_PHY5,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_SR_PHY5,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_SR_PHY4 */
.affected_domains		= {SYSRST_DOMAIN_SR_PHY4,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_SR_PHY4,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_XBAR3 */
.affected_domains		= {SYSRST_DOMAIN_XBAR3,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_XBAR3,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_XBAR2 */
.affected_domains		= {SYSRST_DOMAIN_XBAR2,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_XBAR2,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_XBAR1 */
.affected_domains		= {SYSRST_DOMAIN_XBAR1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_XBAR1,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_XBAR0 */
.affected_domains		= {SYSRST_DOMAIN_XBAR0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_XBAR0,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_GSWIP_R */
.affected_domains		= {SYSRST_DOMAIN_GSWIP_R,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_GSWIP_R,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_GSWIP_R,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_E123_ABORT_REQ */
.affected_domains	= {SYSRST_DOMAIN_E123_ABORT_REQ,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_E123_ABORT_REQ,
#endif
	SYSRST_DOMAIN_INVALID_ID} ,
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_E123_ABORT_ACK */
.affected_domains		= {SYSRST_DOMAIN_E123_ABORT_ACK,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_E123_ABORT_ACK,
#endif
	SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_GPHY_CDB */
.affected_domains		= {SYSRST_DOMAIN_GPHY_CDB,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_GPHY_CDB,
#endif
	SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_DDR_CTL */
.affected_domains		= {SYSRST_DOMAIN_DDR_CTL,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_DDR_CTL,
#endif
	SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_DDR_PUB */
.affected_domains		= {SYSRST_DOMAIN_DDR_PUB,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_DDR_PUB,
#endif
	SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_DDR_PHY */
.affected_domains		= {SYSRST_DOMAIN_DDR_PHY,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_DDR_PHY,
#endif
	SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_MEMMAX */
.affected_domains		= {SYSRST_DOMAIN_MEMMAX,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_DDR,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_CBM
* two TA ports(CBM1 and CBM2) to be reset at the same time
* followed by RCU reset.
*/
.affected_domains		= {SYSRST_DOMAIN_CBM,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_CMB1,
	LTQ_NGI_DOMAIN_TA_CMB2,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_CBM,
#endif
	SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_DMA4
* Separate bit to reset IA(SSX0) and TA(SSX1) ports.
*/
.affected_domains		= {SYSRST_DOMAIN_DMA4,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_IA_DMA4,
	LTQ_NGI_DOMAIN_TA_DMA4,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_DMA3
* Separate bit to reset IA(SSX0) and TA(SSX0) ports.
*/
.affected_domains		= {SYSRST_DOMAIN_DMA3,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_IA_DMA3,
	LTQ_NGI_DOMAIN_TA_DMA3,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_EIP123
* For Reset of EIP123, set the E123_ABORT_REQ bit, and
* then poll for RST_REQ2.E123_ABORT_ACK bit to be set. At this point,
* ngi reset for EIP123 can be called to carry out the EIP123 reset safely.
*/
.affected_domains		= {SYSRST_DOMAIN_EIP123,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_EIP_123,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_E123_ABORT_REQ,
#endif
	SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_EIP97
* Separate bit to reset IA(SSX0) and TA(SSX0) ports.
*/
.affected_domains		= {SYSRST_DOMAIN_EIP97,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_EIP_97,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_EX05
* Separate bit to reset IA(SSX7) and TA(SSX0) ports.
*/
.affected_domains		= {SYSRST_DOMAIN_EX05,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_EX05,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_OCP2OTP
* Separate bit to reset  TA(SSX0) port.
*/
.affected_domains		= {SYSRST_DOMAIN_OCP2OTP,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_OCP2OTP,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {
#ifdef SUPPORT_RCU_DRIVER
	LTQ_RCU_DOMAIN_OCP2SRAM,
#endif
	SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_I2C
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_I2C,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_UART0
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_UART0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_UART0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_UART1
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_UART1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_UART1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_GPTC0
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_GPTC0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_GPTC0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_GPTC1
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_GPTC1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_GPTC1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_GPTC2
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_GPTC2,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_GPTC2,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_SPI0
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_SPI0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_SPI0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_SPI1
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_SPI1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_SPI1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_DMA0
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_DMA0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_IA_DMA0,
	LTQ_NGI_DOMAIN_TA_DMA0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_EBU
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_EBU,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_EBU,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_HSNAND
* Separate bit to reset  TA(SSX4) port.
*/
.affected_domains		= {SYSRST_DOMAIN_HSNAND,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_TA_HSNAND,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_DMA2TX
* Separate bit to reset  TA(SSX1) and IQ(SSX1) port.
*/
.affected_domains		= {SYSRST_DOMAIN_DMA2TX,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_IA_DMA2_TX,
	LTQ_NGI_DOMAIN_TA_DMA2_TX,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_DMA2RX
* Separate bit to reset  TA(SSX1) and IQ(SSX1) port.
*/
.affected_domains		= {SYSRST_DOMAIN_DMA2RX,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_IA_DMA2_RX,
	LTQ_NGI_DOMAIN_TA_DMA2_RX,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_DMA1TX
* Separate bit to reset  TA(SSX2) and IQ(SSX2) port.
*/
.affected_domains		= {SYSRST_DOMAIN_DMA1TX,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_IA_DMA1_TX,
	LTQ_NGI_DOMAIN_TA_DMA1_TX,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_DMA1RX
* Separate bit to reset  TA(SSX2) and IQ(SSX2) port.
*/
.affected_domains		= {SYSRST_DOMAIN_DMA1RX,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_IA_DMA1_RX,
	LTQ_NGI_DOMAIN_TA_DMA1_RX,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_PCIE0
* Separate bit to reset  TA(SSX3) and IQ(SSX3) port.
*/
.affected_domains		= {SYSRST_DOMAIN_PCIE0,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_IA_PCIE1_R,
	LTQ_NGI_DOMAIN_IA_PCIE1_W,
	LTQ_NGI_DOMAIN_TA_PCIE1_A,
	LTQ_NGI_DOMAIN_TA_PCIE1_C,
	LTQ_NGI_DOMAIN_TA_PCIE1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_PCIE1
* Separate bit to reset  TA(SSX3) and IQ(SSX3) port.
*/
.affected_domains		= {SYSRST_DOMAIN_PCIE1,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_IA_PCIE2_R,
	LTQ_NGI_DOMAIN_IA_PCIE2_W,
	LTQ_NGI_DOMAIN_TA_PCIE2_A,
	LTQ_NGI_DOMAIN_TA_PCIE2_C,
	LTQ_NGI_DOMAIN_TA_PCIE2,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},
{
/* SYSRST_DOMAIN_PCIE2
* Separate bit to reset  TA(SSX3) and IQ(SSX3) port.
*/
.affected_domains		= {SYSRST_DOMAIN_PCIE2,
	SYSRST_DOMAIN_INVALID_ID},
.affected_ngi_domains	= {LTQ_NGI_DOMAIN_IA_PCIE3_R,
	LTQ_NGI_DOMAIN_IA_PCIE3_W,
	LTQ_NGI_DOMAIN_TA_PCIE3_A,
	LTQ_NGI_DOMAIN_TA_PCIE3_C,
	LTQ_NGI_DOMAIN_TA_PCIE3,
	SYSRST_DOMAIN_INVALID_ID},
.affected_rcu_domains	= {SYSRST_DOMAIN_INVALID_ID},
.handlers				= NULL,
},

};

/*
 *	Declare local functions
 */
static inline int sysrst_version(char *);
static int __init sysrst_pre_init(void);
static struct task_struct *task;
static struct sysrst_async_data_t data;
static int execute_reset(unsigned int reset_domain_id,
	unsigned int module_id);
#ifdef SUPPORT_NGI_DRIVER
static int sysrst_execute_ngi_reset(unsigned int reset_domain_id,
	unsigned int module_id);
#endif
#ifdef SUPPORT_RCU_DRIVER
static int sysrst_execute_rcu_reset(unsigned int reset_domain_id,
	unsigned int module_id);
#endif

/*
 * ####################################
 *			 RCU Kernel imported APIs
 * ####################################
 */
#ifdef SUPPORT_RCU_DRIVER

#endif /* SUPPORT_RCU_DRIVER */
/*
 * ####################################
 *			 NGI Kernel imported APIs
 * ####################################
 */
#ifdef SUPPORT_NGI_DRIVER
extern int ltq_ngi_reset_core_agent(struct ngi_agent_core *agent, u32 flags);
static int ltq_ngi_get_core_code_id(char *core_code_name, unsigned int flag);
#endif /* SUPPORT_NGI_DRIVER */
/*
 * ####################################
 *			 ICC Kernel imported APIs
 * ####################################
 */
/*
 * ####################################
 *			  Local functions
 * ####################################
 */
static inline int sysrst_version(char *buf)
{
	return sprintf(buf,
		"Lantiq CPE GRX500 System driver version %d.%d.%d\n",
		SYSRST_VER_MAJOR,
		SYSRST_VER_MID,
		SYSRST_VER_MINOR
		);
}

static int __init sysrst_pre_init(void)
{
	int i;
	struct cpuinfo_mips *c = &current_cpu_data;

	/* init semaphore */
	sema_init(&g_sysrst_lock, 1);
	/* init spinlocks */
	spin_lock_init(&g_sysrst_rcu_lock);
	spin_lock_init(&g_sysrst_ngi_lock);

	/* init resources */
	for (i = 0; i < NUM_ENTITY(g_sysrst_domains); i++)
		g_sysrst_domains[i].handlers = NULL;

	/* detect current cpu type */
	if (c->cputype	== CPU_INTERAPTIV) {
		sysrst_msg("Detect InterAptiv CPU ...\n");
		g_current_cpu_env = SYSRST_INTERAPTIVE;
	} else {
		sysrst_msg("Detect 4KEC CPU ...\n");
		g_current_cpu_env = SYSRST_CPU_4KEC;
	}

	return 0;
}
/*
 * ##############################################
 *			  Support proc file system file
 * ##############################################
 */
static int strincmp(const char *p1, const char *p2, int n)
{
	int c1 = 0, c2;
	while (n && *p1 && *p2) {
		c1 = *p1 >= 'A' && *p1 <= 'Z' ? *p1 + 'a' - 'A' : *p1;
		c2 = *p2 >= 'A' && *p2 <= 'Z' ? *p2 + 'a' - 'A' : *p2;
		c1 -= c2;
		if (c1)
			return c1;
		p1++;
		p2++;
		n--;
	}
	return n ? *p1 - *p2 : c1;
}
/*
 * Function: get_next_argument.
 * Description: Get the next valid argument from given string, ignore space
 * Argument: p_src [IN]: source pointer
 * Return: p_src[OUT]: new pointer points to the starting of valid argument
 *			  len[OUT]: the len of ignorance space.
*/
static char *get_next_argument(char *p_src, int *len)
{
	char *p_temp = p_src;
	if (p_temp == NULL) {
		*len = 0;
		return NULL;
	}

	while (p_temp != NULL && *p_temp == ' ')
		p_temp++;


	return p_temp;

}

static struct proc_dir_entry *g_proc_sysrst;
static int proc_env_sysrst(struct seq_file *seq, void *v)
{
	int idx = 0;
	if (g_current_cpu_env == SYSRST_CPU_4KEC)
		seq_puts(seq, "The current CPU is MIPS 4KEC\n");
	else
		seq_puts(seq, "The current CPU is MIPS InterAptiv\n");

	seq_puts(seq, "List of domain name and id:\n");
	for (idx = 0; idx < NUM_ENTITY(g_system_domain_name); idx++) {
		seq_printf(seq, "   %d>: %s - %d", idx,
			(char *)g_system_domain_name[idx], idx);
		if (idx % 3 == 0)
			seq_puts(seq, "\n");
	}

	seq_puts(seq, "\nList of module name and id:\n");
	for (idx = 0; idx < NUM_ENTITY(g_system_module_name); idx++) {
		seq_printf(seq, "   %d>: %s - %d", idx,
			(char *)g_system_module_name[idx], idx);
		if (idx % 3 == 0)
			seq_puts(seq, "\n");
	}

#ifdef SUPPORT_NGI_DRIVER
	seq_puts(seq, "\nList of NGI module name and id:\n");
	for (idx = 0; idx < NUM_ENTITY(g_system_ngi_name); idx++) {
		seq_printf(seq, "   %d>: %s - %d", idx,
			(char *)g_system_ngi_name[idx], idx);
		if (idx % 3 == 0)
			seq_puts(seq, "\n");
	}

#if 0
	/* Print all NGI's TA name and its core code */
	seq_puts(seq, "List of NGI TA:\n");
	for (idx = 0; idx < LTQ_NUM_TARGET_AGENTS; idx++) {
		seq_printf(seq, "   %d>: %s - %d", idx,
			(char *)ngi_ta_core_table[idx].core_name,
			(char *)ngi_ta_core_table[idx].core_code
			);
		if (idx % 3 == 0)
			seq_puts(seq, "\n");
	}
	/* Print all NGI's IA name and its core code */
	seq_puts(seq, "List of NGI IA:\n");
	for (idx = 0; idx < LTQ_NUM_TARGET_AGENTS; idx++) {
		seq_printf(seq, "   %d>: %s - %d", idx,
			(char *)ngi_ia_core_table[idx].core_name,
			(char *)ngi_ia_core_table[idx].core_code
			);
		if (idx % 3 == 0)
			seq_puts(seq, "\n");
	}
#endif
#endif

	return 0;
}
static int proc_read_env_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_env_sysrst, NULL);
}
static const struct file_operations g_proc_file_env_seq_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_env_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release
};

static int proc_read_reset(struct seq_file *seq, void *v)
{
	seq_puts(seq, "Reset information\n");
	return 0;
}

static int proc_read_reset_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, proc_read_reset, NULL);
}

static ssize_t proc_write_reset_seq_write(struct file *file,
			const char __user *buf,
			size_t count,
			loff_t *data
			)
{
	int len;
	char str[64];
	char *p;
	int f_reset = 0;
	int f_status = 0;
	int space_len = 0;

	memset(str, 0, 64);
	len = min(count, (size_t)sizeof(str) - 1);
	len -= copy_from_user(str, buf, len);
	while (len && str[len - 1] <= ' ')
		len--;
	str[len] = 0;
	for (p = str; *p && *p <= ' '; p++, len--)
		;
	if (!*p)
		return count;

	if (strincmp(p, "reset", 5) == 0) {
		p += 5;
		len -= 5;
		f_reset = 1;
		sysrst_dbg("User reset");
	} else if (strincmp(p, "help", 4) == 0 || *p == '?') {
		sysrst_msg("echo reset [domain] [module]> /proc/%s/reset\n",
			SYSRST_PROC_DIR
			);
		sysrst_msg("echo status [domain name] > /proc/%s/reset\n",
			SYSRST_PROC_DIR
			);
		 return count;
	} else if (strincmp(p, "status", 6) == 0) {
		p += 6;
		len -= 6;
		f_status = 1;
		sysrst_dbg("User get status");
	} else if (strincmp(p, "areset", 6) == 0) {
		p += 6;
		len -= 6;
		f_reset = 2;
		sysrst_dbg("User get status");
	}

	p = get_next_argument(p, &space_len);
	len -= space_len;
	if (p == NULL || len <= 0) {
		sysrst_dbg("No more data %s %d",
						p, len);
		return count;
	}
	/* reset or get status of given domain id */
	if (f_reset || f_status) {
		int domain_id = SYSRST_DOMAIN_INVALID_ID;
		int module_id = SYSRST_DOMAIN_INVALID_ID;
		int idx = 0;
		/* get the domain id */
		for (idx = 0; idx < SYSRST_DOMAIN_MAX; idx++) {
			if (strncmp(p, g_system_domain_name[idx],
				strlen(g_system_domain_name[idx])) == 0) {
				domain_id = idx;
				break;
			}
		}
		if (domain_id != SYSRST_DOMAIN_INVALID_ID) {
			if (f_reset) {
				p += strlen(g_system_domain_name[domain_id]);
				len -= strlen(g_system_domain_name[domain_id]);
				p = get_next_argument(p, &space_len);
				len -= space_len;
				if (p == NULL || len <= 0)
					return count;
				/* get the module id */
				for (idx = 0; idx < SYSRST_MODULE_MAX;
					idx++) {
					if (strncmp(p,
						g_system_module_name[idx],
						strlen(
						g_system_module_name[idx]
						)
						)
						== 0) {
						module_id = idx;
						break;
					}
				}
				if (module_id
					!= SYSRST_DOMAIN_INVALID_ID) {
					/* execute the reset */
					if (f_reset == 1) {
						sysrst_dbg(
							"Sync reset %d %d",
							domain_id,
							module_id
							);
						sysrst_rst(
							domain_id,
							module_id,
							0
							);
					} else {
						sysrst_dbg(
							"Async reset %d %d",
							domain_id,
							module_id
							);
						sysrst_rst_async(
							domain_id,
							module_id,
							NULL,
							0,
							0);
					}
				}
			} else if (f_status) {
				int rst_status;
				sysrst_dbg("Going to get status %d",
						domain_id
						);
				rst_status = sysrst_rststatus(domain_id);
				sysrst_msg("Reset status of module[%d] is %d\n",
						domain_id, rst_status);
			}
		}
	}

	return count;

}

static const struct file_operations g_proc_file_reset_seq_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_reset_seq_open,
	.read		= seq_read,
	.write		= proc_write_reset_seq_write,
	.llseek		= seq_lseek,
	.release	= single_release
};

static int proc_read_sysrst_dbg(struct seq_file *seq, void *v)
{
	int i;
	/* skip - 1 */
	for (i = 0; i < NUM_ENTITY(sysrst_dbg_enable_mask_str) - 1; i++)
		seq_printf(seq, "<%d> - %-10s(%-40s): %-5s\n", i,
			sysrst_dbg_enable_mask_str[i].cmd,
			sysrst_dbg_enable_mask_str[i].description,
			(g_dbg_enable & sysrst_dbg_enable_mask_str[i].flag) ?
				"enabled" : "disabled");

	return 0;

}

static int proc_read_sysrst_dbg_seq_open(struct inode *inode,
			struct file *file
			)
{
	return single_open(file, proc_read_sysrst_dbg, NULL);
}

static ssize_t proc_write_sysrst_dbg_seq(struct file *file,
					const char __user *buf,
					size_t count,
					loff_t *data
					)
{
	int len;
	char str[64];
	char *p;
	int f_enable = 0;
	int i;
	int space_len = 0;

	memset(str, 0, 64);
	len = min((unsigned long)count, (unsigned long)sizeof(str) - 1);
	len -= copy_from_user(str, buf, len);
	while (len && str[len - 1] <= ' ')
		len--;
	str[len] = 0;
	for (p = str; *p && *p <= ' '; p++, len--)
		;
	if (!*p)
		return count;

	if (strincmp(p, "enable", 6) == 0) {
		p += 6;
		len -= 6;
		f_enable = 1;
	} else if (strincmp(p, "disable", 7) == 0) {
		p += 7;
		len -= 7;
		f_enable = -1;
	} else if (strincmp(p, "help", 4) == 0 || *p == '?') {
		sysrst_msg("echo <enable/disable> [");
		for (i = 0; i < NUM_ENTITY(sysrst_dbg_enable_mask_str); i++)
			sysrst_msg("%s/", sysrst_dbg_enable_mask_str[i].cmd);
		sysrst_msg("] > /proc/%s/dbg\n", SYSRST_PROC_DIR);
		return count;
	}

	p = get_next_argument(p, &space_len);
	if (p == NULL)
		return count;
	len -= space_len;
	if (f_enable) {
		if ((len <= 0) || (p[0] >= '0' && p[1] <= '9')) {
			if (f_enable > 0)
				g_dbg_enable |= DBG_ENABLE_MASK_ALL;
			else
				g_dbg_enable &= ~DBG_ENABLE_MASK_ALL;
		} else {
			struct sysrst_dgb_info *a =
				sysrst_dbg_enable_mask_str;
			for (i = 0;
				i < NUM_ENTITY(sysrst_dbg_enable_mask_str);
				i++) {
				if (strincmp(p,
					a[i].cmd,
					strlen(a[i].cmd)
					) == 0) {
					char *ptr_cmd = a[i].cmd;
					uint32_t temp_flag = a[i].flag;
					if (f_enable > 0)
						g_dbg_enable |=
							temp_flag;
					else
						g_dbg_enable &=
							~temp_flag;
					/* skip one blank */
					p += strlen(ptr_cmd);
					p = get_next_argument(p,
						&space_len
						);
					/* skip one blank. len maybe
					* negative now if there is
					* no other parameters */
					len -= strlen(ptr_cmd) +
						space_len;
					break;
				}
			}
		}
	}

	return count;

}
static const struct file_operations g_proc_file_dbg_seq_fops = {
	.owner		= THIS_MODULE,
	.open		= proc_read_sysrst_dbg_seq_open,
	.read		= seq_read,
	.write		= proc_write_sysrst_dbg_seq,
	.llseek		= seq_lseek,
	.release	= single_release
};

static int proc_file_create(void)
{
	struct proc_dir_entry *ret;
	if (g_proc_sysrst) {
		sysrst_err("More than one System Reset driver found!");
		return -EEXIST;
	}

	/* create parent proc directory */
	g_proc_sysrst = proc_mkdir(SYSRST_PROC_DIR, NULL);

	/* create env */
	ret  = proc_create("env", S_IRUGO|S_IWUSR,
			g_proc_sysrst, &g_proc_file_env_seq_fops);

	if (!ret) {
		sysrst_err("Failed to create env proc file");
		return -ENODEV;
	}

	/* create dbg */
	ret = proc_create("dbg", S_IRUGO|S_IWUSR,
			g_proc_sysrst, &g_proc_file_dbg_seq_fops);

	if (!ret) {
		sysrst_err("Failed to create dbg proc file");
		return -ENODEV;
	}

	/* create reset */
	ret = proc_create("reset", S_IRUGO|S_IWUSR,
			g_proc_sysrst, &g_proc_file_reset_seq_fops);

	if (!ret) {
		sysrst_err("Failed to create reset proc file");
		return -ENODEV;
	}

	return 0;
}

static void proc_file_delete(void)
{
	/* delete env  entry */
	remove_proc_entry("env", g_proc_sysrst);

	/* delete dbg  entry */
	remove_proc_entry("dbg", g_proc_sysrst);

	/* delete reset  entry */
	remove_proc_entry("reset", g_proc_sysrst);

	/* delete debug dir */
	remove_proc_entry(SYSRST_PROC_DIR, NULL);
}

/*
 * ####################################
 *			  File operations
 * ####################################
 */

static int sysrst_open(struct inode *inode, struct file *filep);
static int sysrst_release(struct inode *inode, struct file *filelp);
static long sysrst_ioctl(struct file *filp,
				unsigned int cmd, unsigned long arg);

static const struct file_operations g_sysrst_fops = {
	.open			= sysrst_open,
	.release		= sysrst_release,
	.unlocked_ioctl = sysrst_ioctl
};
static int sysrst_open(struct inode *inode, struct file *filep)
{
	sysrst_dbg("%s: start", __func__);
	return 0;
}

static int sysrst_release(struct inode *inode, struct file *filelp)
{
	sysrst_dbg("%s: start", __func__);
	return 0;
}

static long sysrst_ioctl(struct file *filp,
							unsigned int cmd,
							unsigned long arg)
{
	int ret;

	/* check magic number */
	if (_IOC_TYPE(cmd) != SYSRST_IOC_MAGIC)
		return -EINVAL;

	/* check read/write right */
	if (((_IOC_DIR(cmd) & _IOC_WRITE)
		&& !access_ok(VERIFY_READ, arg, _IOC_SIZE(cmd)))
		|| ((_IOC_DIR(cmd) & _IOC_READ)
		&& !access_ok(VERIFY_WRITE, arg, _IOC_SIZE(cmd))))
		return -EINVAL;

	switch (cmd) {
	case SYSRST_IOC_VERSION:
		{
			struct sysrst_ioctl_version version = {
				.major = SYSRST_VER_MAJOR,
				.mid   = SYSRST_VER_MID,
				.minor = SYSRST_VER_MINOR
			};
			ret = copy_to_user((void *)arg,
				(void *)&version, sizeof(version));
		}
		break;
	case SYSRST_IOC_QUERY_RST_DOMAIN:
		{
			struct sysrst_ioctl_query_rst_domain query = {0};

			ret = copy_from_user((void *)&query,
				(void *)arg, sizeof(query));
			if (ret == 0) {
				if (query.domain_id
					< NUM_ENTITY(g_sysrst_domains)) {
					query.f_reset =
						sysrst_rststatus(
						query.domain_id);
					ret = copy_to_user((void *)arg,
						(void *)&query, sizeof(query));
				} else
					ret = -EIO;
			}
		}
		break;
	case SYSRST_IOC_DO_RST_DOMAIN:
		{
			struct sysrst_ioctl_query_rst_domain query = {0};

			ret = copy_from_user((void *)&query,
				(void *)arg, sizeof(query));
			if (ret == 0) {
				if (query.domain_id
					< NUM_ENTITY(g_sysrst_domains)) {
					if (sysrst_rst(
							query.domain_id,
							query.module_id,
							query.flags
							) == 0) {
						sysrst_dbg("Execute reset successfully!");
						ret = 0;
					} else {
						sysrst_dbg("Execute reset failed!");
						ret = -EIO;
					}
				} else
					ret = -EIO;
			}
		}
		break;
	default:
		ret = -ENOTSUPP;
	}

	return ret;
}

/*
 * ####################################
 *			  Supported functions
 * ####################################
 */
 /*!
\fn		execute_callback_event(unsigned int reset_domain_id,
						unsigned int module_id,
						sysrst_rst_event_t event)
\brief	Execute callback handlers
		Internally used to execute callback handlers .

\param	reset_domain_id - unsigned int, domain module ID
\param	module_id		- unsigned int, driver/component module ID
\param	event			- sysrst_rst_event_t, pre or post event
\return	int	- 0 :	   execute callback handlers successfully.
					< 0 :	   error
*/
static int execute_callback_event(
			unsigned int reset_domain_id,
			unsigned int module_id,
			enum sysrst_rst_event_t event
			)
{
	unsigned char domain_id;
	struct sysrst_handler_t *p_cur;
	int index = 0;
	unsigned char *domain_id_array = NULL;
	if (reset_domain_id >= SYSRST_DOMAIN_MAX
		|| module_id >= SYSRST_MODULE_MAX) {
		sysrst_err("Invalid reset id[%d] module[%d] event[%d]",
			reset_domain_id, module_id, event);
		return -EINVAL;
	}
	domain_id_array = g_sysrst_domains[reset_domain_id].affected_domains;
	sysrst_dbg("Prepare reset[%d],module_id[%d] event[%d]",
		reset_domain_id, module_id, event);
	if (domain_id_array != NULL) {
		while (index < SYSRST_AFFTECTED_DOMAIN_MAX
			&& (domain_id_array[index]
				!= SYSRST_DOMAIN_INVALID_ID)) {
			domain_id = domain_id_array[index];
			sysrst_dbg("Just b4 issue reset domain_id = %d",
				(int)domain_id);
			if (domain_id < SYSRST_DOMAIN_MAX) {
				p_cur = g_sysrst_domains[domain_id].handlers;
				for (;
					p_cur != NULL;
					p_cur = p_cur->next
					)
					if (p_cur->module_id == module_id)
						p_cur->fn(domain_id,
								module_id,
								event,
								p_cur->arg);
			}
			index++;
		}
	}
	return 0;
}
/*!
\fn		sysrst_rst_thread_function(void *data)
\brief	Execute asynchronous reset requested by user or other modules.

\param	data - sysrst_async_data_t, reset arguments
\return	int	- 0 :	   execute callback handlers successfully.
			< 0 :	   error
*/
static int sysrst_rst_thread_function(void *data)
{
	struct sysrst_async_data_t rst_data;
	int reset_result = -1;

	sysrst_dbg("Start execute sysrst_rst");
	rst_data = *(struct sysrst_async_data_t *)data;
	sysrst_dbg(
		"Reset information: domain[%d] module[%d] arg[%x]",
		rst_data.domain_id,
		rst_data.module_id,
		rst_data.arg
		);
	if (!kthread_should_stop()) {
		if (sysrst_rst(
				rst_data.domain_id,
				rst_data.module_id,
				rst_data.flags
				)
				== 0) {
			sysrst_dbg("Execute reset successfully!");
			reset_result = 0;
		} else {
			sysrst_dbg("Execute reset failed!");
			reset_result = 0;
		}
	}
	if (rst_data.callbackfn) {
		sysrst_dbg("Call callback function");
		rst_data.callbackfn(reset_result, rst_data.arg);
	} else {
		sysrst_err("Callback function is NULL");
	}
	sysrst_dbg("Finish execute sysrst_rst");
	return reset_result;
}
#ifdef SUPPORT_ICC_DRIVER

static struct icc_transaction_t *icc_new_transaction(void)
{
	struct icc_transaction_t *new_trans;

	new_trans = kzalloc(sizeof(struct icc_transaction_t), GFP_KERNEL);
	if (!new_trans) {
		sysrst_err("Can not malloc\n");
		return NULL;
	}
	if (down_interruptible(&sem_transaction)) {
		/* free new transaction */
		kfree(new_trans);
		return NULL;
	}
	/* Get the new transaction id */
	g_transaction_id++;
	new_trans->transaction_id = g_transaction_id;
	init_completion(&(new_trans->comp));
	list_add((struct list_head *)new_trans, &g_icc_transaction);
	up(&sem_transaction);
	return new_trans;
}

static void icc_complete_transaction(int transaction_id, int result)
{
	struct list_head *pos, *q;
	struct icc_transaction_t *trans = NULL;
	sysrst_dbg("%s\n", __func__);
	if (down_interruptible(&sem_transaction))
		return;
	list_for_each_safe(pos, q, &g_icc_transaction) {
		if (pos) {
			struct icc_transaction_t *temp_trans =
				(struct icc_transaction_t *)pos;
			if (temp_trans->transaction_id == transaction_id) {
				trans = temp_trans;
				break;
			}
		}
	}
	up(&sem_transaction);
	if (trans != NULL) {
		trans->result = result;
		complete(&(trans->comp));
	}
}

static int icc_wait_and_exit_transaction(struct icc_transaction_t *trans)
{
	struct list_head *pos, *q;
	int result, error;
	if (!trans)
		return -1;
	error = 0;
	if (!wait_for_completion_timeout(&trans->comp,
			msecs_to_jiffies(SYSRST_TIMEOUT_MS))) {
		sysrst_err("ICC timeout");
		error = -1;
		result = -1;
	}
	/* remove the transaction from list */
	if (down_interruptible(&sem_transaction))
		return -1;
	list_for_each_safe(pos, q, &g_icc_transaction) {
		if (pos) {
			struct icc_transaction_t *temp_trans =
				(struct icc_transaction_t *)pos;
			if (temp_trans->transaction_id
					== trans->transaction_id) {
				list_del(pos);
				break;
			}
		}
	}
	up(&sem_transaction);
	/* destroy the completion */
	if (!error)
		result = trans->result;
	kfree(trans);
	return result;
}

static int icc_send_command(int command, int reset_domain_id, int module_id)
{
	struct icc_param_t param;
	struct icc_transaction_t *new_trans;
	if (g_current_cpu_env == SYSRST_CPU_4KEC)
		sysrst_err("Cannot call this function in 4KEC\n");
	param.command = command;
	param.reset_domain_id = reset_domain_id;
	param.module_id = module_id;
	new_trans = icc_new_transaction();
	if (!new_trans)
		return -1;
	param.transaction_id = new_trans->transaction_id;
	param.result = 0;
	memcpy((void *)g_icc_write.param[0],
		&param,
		sizeof(struct icc_param_t));
	g_icc_write.param[1] = sizeof(struct icc_param_t);
	cache_wb_inv(g_icc_write.param[0], g_icc_write.param[1]);
	/* send to 4Kec */
	icc_write(SR, &g_icc_write);
	/* wait for result */
	return icc_wait_and_exit_transaction(new_trans);
}

static int icc_communication_callback(void)
{
	int ret;
	struct icc_param_t param;
	ret = -1;
	sysrst_dbg("%s: g_icc_rw.param[0][0x0x%x] length[%d]\n",
		__func__, g_icc_rw.param[0], g_icc_rw.param[1]);
	param = *(struct icc_param_t *)(g_icc_rw.param[0]);
	sysrst_dbg("%s: trans[%d] cmd[%d] reset[%d] domain[%d]",
		__func__,
		param.transaction_id,
		param.command,
		param.reset_domain_id,
		param.module_id
		);
	/* prepare for reply */
	if (g_current_cpu_env == SYSRST_CPU_4KEC) {
		switch (param.command) {
		case ICC_COMMAND_RESET_REQ:
			sysrst_dbg("%s:RESET REQ from InterAptiv\n", __func__);
			ret = execute_reset(param.reset_domain_id,
				param.module_id);
			param.result = ret;
			param.command = ICC_COMMAND_RESET_REPLY;
			break;
		case ICC_COMMAND_CLEAR_REQ:
			sysrst_dbg("%s:CLEAR REQ from InterAptiv\n", __func__);
			sysrst_clrstatus(param.reset_domain_id);
			ret = 0;
			param.result = 0;
			param.command = ICC_COMMAND_CLEAR_REPLY;
			break;
		case ICC_COMMAND_GET_REQ:
			sysrst_dbg("%s:GET REQ from InterAptiv\n", __func__);
			ret = sysrst_rststatus(param.reset_domain_id);
			param.result = ret;
			param.command = ICC_COMMAND_GET_REPLY;
			break;
		default:
			sysrst_err("Unknown command from ICC communication");
			ret = -1;
			break;
		}
	} else {
		/* received reply from 4Kec, then continue to execute
		* post-event calback functions
		*/
		switch (param.command) {
		case ICC_COMMAND_RESET_REPLY:
		case ICC_COMMAND_CLEAR_REPLY:
		case ICC_COMMAND_GET_REPLY:
			sysrst_dbg("%s:Received REPLY from 4KEC\n", __func__);
			icc_complete_transaction(param.transaction_id,
				param.result);
			ret = 0;
			break;
		default:
			sysrst_err("Unknown command from ICC communication");
			icc_complete_transaction(param.transaction_id,
				param.result);
			ret = -1;
			break;
		}
	}
	/*
	* Only 4Kec needs to send back respond to InterAptiv
	*/
	if (g_current_cpu_env == SYSRST_CPU_4KEC) {
		/*
		* 4Kec sends result back using the same pointer
		* received in callback/request from IAP
		*/
		g_icc_write.param[0] = g_icc_rw.param[0];
		memcpy((void *)g_icc_write.param[0],
			&param, sizeof(struct icc_param_t));
		g_icc_write.param[1] = sizeof(struct icc_param_t);
		cache_wb_inv(g_icc_write.param[0], g_icc_write.param[1]);
		icc_write(SR, &g_icc_write);
	}
	return ret;
}

void icc_callback_fn(icc_wake_type wake_type)
{
	memset(&g_icc_rw, 0, sizeof(icc_msg_t));
	if (wake_type & ICC_READ) {
		icc_read(SR, &g_icc_rw);
		cache_inv(g_icc_rw.param[0], g_icc_rw.param[1]);
		icc_communication_callback();
	}
}

int sysrst_icc_init(void)
{
	int result;

	/* init resources for transaction */
	sema_init(&sem_transaction, 1);
	if (g_current_cpu_env == SYSRST_INTERAPTIVE) {
		/*
		* InterAptiv uses DMA buffer to send command to 4KEC
		*/
		g_icc_write.param[0] = (uint32_t)kzalloc(
				sizeof(struct icc_param_t),
				GFP_KERNEL | GFP_DMA
				);
		if (g_icc_write.param[0] == 0)
			return -ENOMEM;
	}
	result = icc_open((struct inode *)SR, NULL);
	if (result < 0) {
		sysrst_err("Open ICC failed\n");
		return result;
	}
	result = icc_register_callback(SR, &icc_callback_fn);
	if (result < 0) {
		sysrst_err("CallBack Register with ICC Failed\n");
		return result;
	}
	/*source and dest client id to be filled*/
	g_icc_write.src_client_id = SR;
	g_icc_write.dst_client_id = SR;
	g_icc_write.param_attr = ICC_PARAM_PTR;
	return 0;
}

void sysrst_icc_exit(void)
{
	/* Unregister ICC callback function */
	icc_unregister_callback(SR);
	/* Free memory */
	if (g_icc_write.param[0])
		kfree((void *)g_icc_write.param[0]);
}
#endif

/*!
\fn		execute_reset(void *data)
\brief	Execute reset command to NGI and RCU.
		If System Driver is running on InterAptiv, it will
		use ICC to communicate with 4Kec.

\param	reset_domain_id - reset domain id
		module_id - domain id
\return	int	- 0 :	   execute callback handlers successfully.
			< 0 :	   error
*/
static int execute_reset(
		unsigned int reset_domain_id,
		unsigned int module_id
		)
{
	int rst_result;
	/*
	* if driver is running on InterAptiv Core,
	* it should redirect the command to
	* it peer which is running on 4KEC core.
	*/
	rst_result = -1;
	if (g_current_cpu_env == SYSRST_INTERAPTIVE) {
#ifdef SUPPORT_ICC_DRIVER
		/* re-direct command using ICC interface */
		rst_result = icc_send_command(ICC_COMMAND_RESET_REQ,
			reset_domain_id,
			module_id
			);
		sysrst_dbg("%s:Finish ICC\n", __func__);
#else
		sysrst_err("Need to support ICC in secured OS system\n");
#endif
	} else {
		/*
		* Execute reset command to NGI and RCU directly
		*/
#ifdef SUPPORT_NGI_DRIVER
		sysrst_execute_ngi_reset(reset_domain_id, module_id);
#endif
#ifdef SUPPORT_RCU_DRIVER
		rst_result = sysrst_execute_rcu_reset(
					reset_domain_id,
					module_id);
#endif
	}
	return rst_result;
}
#ifdef SUPPORT_NGI_DRIVER

/*!
 \fn		ltq_ngi_get_core_code_id(char *core_code_name,
					unsigned int flag)
 \brief	Execute ngi hardware reset

			Internally used to execute RCU reset.

\param	core_code_name - NGI core code name, defined by NGI module
\param	flag - IA/TA or core
\return	int	- 0 :	   RCU reset successfully.
				< 0 :	   error
*/
static int ltq_ngi_get_core_code_id(
		char *core_code_name,
		unsigned int flag
		)
{
	unsigned int core_code_id;
	int i = 0;
	sysrst_dbg("%s: Find core code if for[%s][%d]\n", __func__
		, core_code_name
		, flag
		);
	core_code_id = -1;
	if (flag == NGI_F_IA) {
		/* Looking for Initiator Agent */
		for (i = 0; i < LTQ_NUM_INIT_AGENTS; i++) {
			sysrst_dbg("%s: search IA [%s]\n",
				__func__,
				ngi_ia_core_table[i].core_name
				);
			if (strincmp(core_code_name,
				ngi_ia_core_table[i].core_name,
				strlen(ngi_ia_core_table[i].core_name)
				) == 0) {
				core_code_id = ngi_ia_core_table[i].core_code;
				break;
			}
		}
	} else if (flag == NGI_F_TA) {
		/* Looking for Target Agent */
		for (i = 0; i < LTQ_NUM_TARGET_AGENTS; i++) {
			sysrst_dbg("%s: search TA [%s]\n",
				__func__,
				ngi_ta_core_table[i].core_name
				);
			if (strincmp(core_code_name,
				ngi_ta_core_table[i].core_name,
				strlen(ngi_ta_core_table[i].core_name))
				== 0) {
				core_code_id = ngi_ta_core_table[i].core_code;
				break;
			}
		}
	} else {
		sysrst_err("%s:Not supported\n", __func__);
	}
	return core_code_id;
}
/*!
 \fn		sysrst_execute_ngi_reset(unsigned int reset_domain_id,
					unsigned int module_id)
 \brief	Execute ngi hardware reset


\param	reset_domain_id - system reset domain id
\param	module_id		- module id
\return	int	- 0 :	   NGI reset successfully.
				< 0 :	   error
*/
static int sysrst_execute_ngi_reset(
		unsigned int reset_domain_id,
		unsigned int module_id
		)
{
	int index = 0;
	unsigned char *array;
	unsigned long sys_flags;
	/* only support in 4kec */
	if (g_current_cpu_env != SYSRST_CPU_4KEC) {
		sysrst_dbg("Cannot execute NGI reset in InterAptiv");
		return -EINVAL;
	}
	/* sanity check */
	if (reset_domain_id > SYSRST_DOMAIN_MAX
		|| module_id > SYSRST_MODULE_MAX) {
		sysrst_err("Invalid domain id[%d] and module id[%d]\n",
			reset_domain_id, module_id);
		return -EINVAL;
	}
	spin_lock_irqsave(&g_sysrst_ngi_lock, sys_flags);
	array = g_sysrst_domains[reset_domain_id].affected_ngi_domains;
	if (array != NULL) {
		while (array[index]
			!= SYSRST_DOMAIN_INVALID_ID) {
			int core_code_id;
			sysrst_dbg("Going to reset NGI domain[%d]",
				array[index]
				);
			/* Get the core code id from its name */
			core_code_id = ltq_ngi_get_core_code_id(
				g_ngi_domains[array[index]].core_code_name,
				g_ngi_domains[array[index]].flag
				);
			if (core_code_id == -1) {
				sysrst_err("%s: Error name[%s][%d]\n",
				__func__,
				g_ngi_domains[array[index]].core_code_name,
				g_ngi_domains[array[index]].flag
				);
				index++;
				continue;/* while */
			}
			g_ngi_domains[array[index]].ngi_core.core_code =
				core_code_id;
			/* reset NGI hardware */
			if (ltq_ngi_reset_core_agent(
				&g_ngi_domains[array[index]].ngi_core,
				g_ngi_domains[array[index]].flag
				) != 0) {
				sysrst_err("Failed to reset NGI[%d]\n",
					array[index]
					);
			}

			index++;
		}
	}
	spin_unlock_irqrestore(&g_sysrst_ngi_lock, sys_flags);
	return 0;
}
#endif
#ifdef SUPPORT_RCU_DRIVER
 /*!
  \fn		static int sysrst_execute_rcu_reset(
			unsigned char *rcu_array_domain)
  \brief	Execute rcu hardware reset

		Internally used to execute RCU reset .

  \param	rst_id - system reset domain id
  \param	module_id		- module id
  \return	int	- 0 :	   RCU reset successfully.
					< 0 :	   error
*/
static int sysrst_execute_rcu_reset(unsigned int rst_id,
				unsigned int module_id)
{
	int index = 0;
	/* sanity check */
	if (rst_id > SYSRST_DOMAIN_MAX
		|| module_id > SYSRST_MODULE_MAX) {
		sysrst_err("Invalid domain[%d] and module[%d]\n",
			rst_id, module_id);
		return -EINVAL;
	}
	sysrst_dbg("Starting reset for RCU");
	/*
	* if driver is running on InterAptiv Core,
	* it should redirect the command to
	* it peer which is running on 4KEC core.
	*/
	if (g_current_cpu_env == SYSRST_INTERAPTIVE) {
		sysrst_err("Cannot run in InterAptiv\n");
	} else {
		unsigned long sys_flags;
		unsigned char *affected_rcu = NULL;
		spin_lock_irqsave(&g_sysrst_rcu_lock, sys_flags);
		affected_rcu = g_sysrst_domains[rst_id].affected_rcu_domains;
		if (affected_rcu != NULL) {
			while (affected_rcu[index] !=
				SYSRST_DOMAIN_INVALID_ID) {
				sysrst_dbg("Reset RCU [%d] module[%d]",
					affected_rcu[index],
					module_id
					);
				/* reset RCU hardware */
				ltq_rcu_rst(affected_rcu[index], module_id);
				index++;
			}
		}
		spin_unlock_irqrestore(&g_sysrst_rcu_lock, sys_flags);
	}
	return 0;
}
#endif
/*
 * ####################################
 *			  Exported functions
 * ####################################
 */
#ifdef __KERNEL__
/*!
  \fn		int sysrst_register(unsigned int reset_domain_id,
						unsigned int module_id,
						sysrst_rst_handler_t callbackfn,
						unsigned long arg)
  \brief	Register ownership of one hardware module.
			User uses this function to register ownership of
			one hardware module and register
			callback function to receive reset event.
			When reset is triggered by other component,
			one pre-reset event is received through callback
			function before reset, and one post-reset event
			is received through callback function after reset.
  \param	reset_domain_id - unsigned int, domain module ID
  \param	module_id	- unsigned int, driver/component module ID
  \param	callbackfn	- sysrst_rst_handler_t,
					callback function to receive event
  \param	arg - unsigned long, parameter of callback function
  \return	0	Register successfully.
  \return	negative   Register fail.
*/
int sysrst_register(unsigned int reset_domain_id,
						unsigned int module_id,
						sysrst_rst_handler_t callbackfn,
						unsigned long arg
						)
{
	struct sysrst_handler_t *p_handler;
	struct sysrst_handler_t *p_cur;

	if (reset_domain_id >= NUM_ENTITY(g_sysrst_domains)
		|| module_id >= SYSRST_MODULE_MAX
		|| callbackfn == NULL) {
		sysrst_err("Invalid argument [%d] [%d]\n",
					reset_domain_id,
					module_id
					);
		return -EINVAL;
	}

	p_handler = kmalloc(
		sizeof(struct sysrst_handler_t),
		GFP_KERNEL
		);
	if (p_handler == NULL) {
		sysrst_err("Cannot alloc memory\n");
		return -ENOMEM;
	}
	sysrst_dbg("Start register callback function [%d][%d]",
				reset_domain_id,
				module_id);
	p_handler->fn			= callbackfn;
	p_handler->arg			= arg;
	p_handler->module_id	= module_id;
	p_handler->next	= NULL;

	if (down_interruptible(&g_sysrst_lock)) {
		if (p_handler != NULL)
			kfree(p_handler);
		return -EBUSY;
	}

	for (p_cur = g_sysrst_domains[reset_domain_id].handlers;
			p_cur != NULL; p_cur = p_cur->next)
		if (p_cur->module_id == module_id)
			break;
	if (p_cur != NULL) {
		sysrst_dbg("Replace old registration");
		p_cur->fn	= callbackfn;
		p_cur->arg	= arg;
	} else {
		sysrst_dbg("New registration");
		p_handler->next =
			g_sysrst_domains[reset_domain_id].handlers;
		g_sysrst_domains[reset_domain_id].handlers = p_handler;
	}
	up(&g_sysrst_lock);

	if (p_cur != NULL)
		kfree(p_handler);

	return 0;
}
EXPORT_SYMBOL(sysrst_register);

/*!
  \fn		int sysrst_free(unsigned int reset_domain_id,
					unsigned int module_id)
  \brief	Release ownership of one hardware module.
			User uses this function to release
			ownership of one hardware module.
			Driver will not notify this component
			for reset event if this function return
			successfully.

  \param	rst_id - unsigned int, hardware module ID
  \param	module_id - unsigned int, driver/component module ID
  \return	0			   Release successfully.
  \return	negative	   Release fail.
 */
int sysrst_free(unsigned int rst_id, unsigned int module_id)
{
	struct sysrst_handler_t *p_cur, *p_prev;

	if (rst_id >= NUM_ENTITY(g_sysrst_domains)
		|| module_id >= SYSRST_MODULE_MAX) {
		sysrst_err("Invalid argument [%d] [%d]\n",
					rst_id,
					module_id);
		return -EINVAL;
	}
	sysrst_dbg("Start free callback function [%d][%d]",
				rst_id,
				module_id);

	if (down_interruptible(&g_sysrst_lock))
		return -EBUSY;
	for (p_prev = NULL, p_cur = g_sysrst_domains[rst_id].handlers;
			p_cur != NULL;
			p_prev = p_cur, p_cur = p_cur->next
		)
		if (p_cur->module_id == module_id) {
			if (p_prev != NULL)
				p_prev->next = p_cur->next;
			else
				g_sysrst_domains[rst_id].handlers =
					p_cur->next;
			break;
		}
	up(&g_sysrst_lock);

	if (p_cur != NULL) {
		kfree(p_cur);
		return 0;
	} else
		return -EIO;
}
EXPORT_SYMBOL(sysrst_free);

/*!
  \fn		int sysrst_rst(unsigned int reset_domain_id,
					unsigned int module_id,
					unsigned int flags)
  \brief	Trigger reset of one hardware module.
		User uses this function to trigger reset of one hardware module.
		Driver will give notification to all drivers/components
		registering for reset of this hardware module.

  \param	reset_domain_id - unsigned int, hardware module ID
  \param	module_id	- unsigned int, driver/component module ID
  \param	flags			- flags, not use now.
  \return	0			   Reset successfully.
  \return	negative	   Reset fail.
 */
int sysrst_rst(unsigned int reset_domain_id,
					unsigned int module_id,
					unsigned int flags)
{
	int rst_result = -1;
	sysrst_dbg("Reset on reset[%d] and module[%d] with flags[%d]",
		reset_domain_id,
		module_id,
		flags
		);
	if (down_interruptible(&g_sysrst_lock))
		return -EBUSY;

	/* trigger pre-reset event, each handler
	* must be as fast as possible, because interrupt is disabled */
	/* issue reset */
	execute_callback_event(
		reset_domain_id,
		module_id,
		SYSRST_EVENT_PRE_RESET
		);
	/* execute and wait for result of reset */
	rst_result = execute_reset(reset_domain_id, module_id);
	/* trigger post-reset event, each handler must be
	* as fast as possible, because interrupt is disabled */
	execute_callback_event(
			reset_domain_id,
			module_id,
			SYSRST_EVENT_POST_RESET
			);
	up(&g_sysrst_lock);
	return rst_result;
}
EXPORT_SYMBOL(sysrst_rst);

/*!
  \fn		int sysrst_rststatus(unsigned int reset_domain_id)
  \brief	Get reset status - For the RCU reset devices, this is valid.

  \param	reset_domain_id - unsigned int, hardware module ID
  \return	1	in reset status
  \return	0	normal status
 */
int sysrst_rststatus(unsigned int reset_domain_id)
{
	int result = 0;
#ifdef SUPPORT_RCU_DRIVER
	unsigned long sys_flags;
	int tmp_result = 0;
	int index = 0;
	unsigned char *array;
#endif

	if (g_current_cpu_env == SYSRST_INTERAPTIVE) {
		if (down_interruptible(&g_sysrst_lock))
				return -EBUSY;
		result = icc_send_command(ICC_COMMAND_GET_REQ,
			reset_domain_id,
			0);
		up(&g_sysrst_lock);
	} else {
	/* the reset domain should relate to RCU only. */
	sysrst_dbg("Start reset on reset_domain_id[%d]", reset_domain_id);
#ifdef SUPPORT_RCU_DRIVER
		spin_lock_irqsave(&g_sysrst_rcu_lock, sys_flags);
		array = g_sysrst_domains[reset_domain_id].affected_rcu_domains;
		if (array != NULL) {
			while (array[index]
				!= SYSRST_DOMAIN_INVALID_ID) {
				tmp_result = ltq_rcu_stat_get(
				array[index]
				)
				;
			sysrst_dbg("Get RCU domain[%d] status[%d]",
				array[index],
				tmp_result
				);
			/* reset RCU hardware */
			result |= tmp_result;
			index++;
			}
		}
		spin_unlock_irqrestore(&g_sysrst_rcu_lock, sys_flags);
#endif
	}

	return result;
}
EXPORT_SYMBOL(sysrst_rststatus);
/*!
\fn		int sysrst_clrstatus(unsigned int reset_domain_id)
\brief	Clear reset status - For the RCU reset devices, this is valid.

\param	reset_domain_id - unsigned int, hardware module ID.
\return void
*/
void sysrst_clrstatus(unsigned int reset_domain_id)
{
#ifdef SUPPORT_RCU_DRIVER
	unsigned long sys_flags;
	int index = 0;
	unsigned char *array;
#endif
    /* the reset domain should relate to RCU only. */
	sysrst_dbg("Start clear status domain[%d]", reset_domain_id);
	if (g_current_cpu_env == SYSRST_INTERAPTIVE) {
		if (down_interruptible(&g_sysrst_lock))
			return;
		icc_send_command(ICC_COMMAND_CLEAR_REQ, reset_domain_id, 0);
		up(&g_sysrst_lock);
	} else {
#ifdef SUPPORT_RCU_DRIVER
		spin_lock_irqsave(&g_sysrst_rcu_lock, sys_flags);
		array = g_sysrst_domains[reset_domain_id].affected_rcu_domains;
		if (array != NULL) {
			while (array[index]
				!= SYSRST_DOMAIN_INVALID_ID) {
				ltq_rcu_stat_clear(
					array[index]
					);
				sysrst_dbg("Clear RCU domain[%d]",
					array[index]
					);
				index++;
			}
		}
		spin_unlock_irqrestore(&g_sysrst_rcu_lock, sys_flags);
#endif
	}
}
EXPORT_SYMBOL(sysrst_clrstatus);
/*!
\fn		int sysrst_rst_async(unsigned int reset_domain_id,
					unsigned int module_id,
					sysrst_rst_async_handler_t *rst_done,
					unsigned int arg,
					unsigned int flags)

\brief	Execute asynchronous reset.
		This function is mainly called by NGI driver.

\param	reset_domain_id - unsigned int, hardware module ID.
		module_id - module ID.
		rst_done - callback function
		arg - argument passed back to callback function
		flags - currently not use

\return int 0 - successfully
			-1 - fail
*/
int sysrst_rst_async(unsigned int reset_domain_id,
					unsigned int module_id,
					sysrst_rst_async_handler_t rst_done,
					unsigned int arg,
					unsigned int flags)
{
	data.domain_id = reset_domain_id;
	data.module_id = module_id;
	data.arg = arg;
	data.flags = flags;
	data.callbackfn = rst_done;
	task = kthread_run(&sysrst_rst_thread_function,
					(void *)&data,
					"sysrst_rst_async"
					);
	sysrst_dbg("Kernel Thread : Start %s", task->comm);
	return 0;
}
EXPORT_SYMBOL(sysrst_rst_async);
#endif /* __KERNEL__ */

static int __init sysrst_init(void)
{
	int ret = 0;
	char ver_str[64];

	sysrst_msg("Trying to register System Reset Driver ...\n");

	/* register character device */
	ret = register_chrdev(SYSRST_MAJOR, SYSRST_DEVICE_NAME,
							&g_sysrst_fops);
	if (ret != 0) {
		sysrst_msg("Can not register System Reset Driver - %d",
			ret);
		return ret;
	}
    /* Clear the default callback to NULL for each reset domains */
	ret = sysrst_pre_init();
#ifdef SUPPORT_ICC_DRIVER
	/* Init ICC Driver */
	ret = sysrst_icc_init();
	if (ret < 0)
		sysrst_msg("Can not communicate with ICC module");
#endif
	/* driver version */
	sysrst_version(ver_str);
	/* create proc file system */
	proc_file_create();

	sysrst_msg("%s", ver_str);
	sysrst_msg("GRX500 System Reset initialized successfully!\n");
	return 0;
}
static void __exit sysrst_exit(void)
{
	struct sysrst_handler_t *p_cur, *p_free;
	int i;
	/* un-register device */
	unregister_chrdev(SYSRST_MAJOR, SYSRST_DEVICE_NAME);

	for (i = 0; i < NUM_ENTITY(g_sysrst_domains); i++) {
		/* free all pre reset callback functions */
		for (p_free = p_cur = g_sysrst_domains[i].handlers;
						p_cur != NULL; p_free = p_cur) {
			p_cur = p_cur->next;
			kfree(p_free);
		}
	}
	/* delete proc file system */
	proc_file_delete();
#ifdef SUPPORT_ICC_DRIVER
	sysrst_icc_exit();
#endif
	sysrst_msg("GRX500 RCU Driver removed successfully!\n");

}

module_init(sysrst_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GRX500 System Reset Driver Module");
MODULE_AUTHOR("Ho Nghia Duc");
