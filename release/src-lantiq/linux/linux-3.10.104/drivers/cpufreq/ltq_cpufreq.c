/*
 *
 *			 Copyright (c) 2012, 2014, 2015
 *			Lantiq Beteiligungs-GmbH & Co. KG
 *
 *  For licensing information, see the file 'LICENSE' in the root folder of
 *  this software module.
 */

#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
	#define DEBUG
#endif

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/cpu.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/sysfs.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/clk.h>
#include <lantiq.h>
#include <linux/regulator/consumer.h>
#include <linux/cpufreq.h>
#include <cpufreq/ltq_cpufreq.h>
#ifdef CONFIG_LTQ_CPUFREQ_DVS
	#include <../arch/mips/lantiq/grx500/clk.h>
#endif
#include <../arch/mips/lantiq/clk.h>
#include <../arch/mips/include/asm/idle.h>

#define VERSION "3.0.0"
#define RED     "\033[31;1m"
#define NORMAL  "\033[0m"
#define LTQ_PR_DBG(fmt, arg...)				\
({							\
	if (alert_msg > 0) {				\
		pr_alert(RED pr_fmt(fmt) NORMAL, ##arg);\
	} else {					\
		pr_debug(pr_fmt(fmt), ##arg);		\
	}						\
})

#define LTQ_CPUFREQ_TRANSITION_LATENCY 500000 /* ns */

/* GRX500 product variants */
#define GRX350			0x1
#define GRX550			0x2

int alert_msg;

enum ltq_cpufreq_req_id {
	/** no pending powerState request pending in the ringBuffer */
	LTQ_CPUFREQ_NO_PENDING_REQ      = 0,
	/** powerState request pending in the ringBuffer */
	LTQ_CPUFREQ_PENDING_REQ         = 1,
	/** unknown powerState request in buffer or buffer overflow */
	LTQ_CPUFREQ_PENDING_REQ_ERROR   = -1,
};

/**
	struct used in REQUEST ringBuffer to keep all relevant info's for one
	powerState request
*/
struct ltq_cpufreq_req_state {
	struct ltq_cpufreq_module_state moduleState;
	enum ltq_cpufreq_req_id         reqId;
};


/*======================================================================*/
/* LOCAL FUNCTIONS PROTOTYPES						*/
/*======================================================================*/
static int ltq_cpufreq_init(struct cpufreq_policy *policy);
static int ltq_cpufreq_verify(struct cpufreq_policy *policy);
static int ltq_cpufreq_target(struct cpufreq_policy *policy,
				unsigned int target_freq,
				unsigned int relation);
static int ltq_cpufreq_exit(struct cpufreq_policy *policy);
static int ltq_cpufreq_resume(struct cpufreq_policy *policy);
static struct ltq_cpufreq_req_state ltq_cpufreq_get_req(void);
static int ltq_cpufreq_put_req(struct ltq_cpufreq_req_state req);
static void ltq_cpufreq_process_req(struct work_struct *work);
static int ltq_cpucg_pwrFeatureSwitch(int pwrStateEna);
static int ltq_cpufreq_state_get(enum ltq_cpufreq_state *pmcuState);

/*======================================================================*/
/* WORKQUEUE DECLARATION						*/
/*======================================================================*/
static DECLARE_WORK(work_obj, ltq_cpufreq_process_req);

/*======================================================================*/
/* module DEFINES							*/
/*======================================================================*/
/* size of request ringbuffer */
#define LTQ_REQ_BUFFER_SIZE 5

/*======================================================================*/
/* Internal module global variable DECLARATIONS				*/
/*======================================================================*/
#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
struct ltq_cpufreq_latency ltq_cpufreq_latency_g[3] = {
	/* max,  min   , cur, freq, comment */
	{ -1, 999999999, -1,   0,   "Not Defined"},
	{ -1, 999999999, -1,   0,   "Not Defined"},
	{ -1, 999999999, -1,   0,   "Not Defined"},
};
#endif

struct reg_cpufreq	drvdata;

struct ltq_cpufreq_module_info cpuddr_feature_fss = {
	.name                      = "CPU/DDR frequency scaling support",
	.pmcuModule                = LTQ_CPUFREQ_MODULE_CPU,
	.pmcuModuleNr              = 0,
	.powerFeatureStat          = 1,
	.ltq_cpufreq_state_get     = ltq_cpufreq_state_get,
	.ltq_cpufreq_pwr_feature_switch = NULL,
};

struct ltq_cpufreq_module_info cpuwait_feature_cgs = {
	.name                   = "CPU clock gating support (sleep opcode)",
	.pmcuModule             = LTQ_CPUFREQ_MODULE_CPU,
	.pmcuModuleNr           = 0,
	.powerFeatureStat       = 1,
	.ltq_cpufreq_state_get  = NULL,
	.ltq_cpufreq_pwr_feature_switch = ltq_cpucg_pwrFeatureSwitch,
};

/** \ingroup LQ_CPUFREQ
\brief  Container to save cpu_wait pointer
	Used to enable/disable the mips_core clock gating feature
*/
static void (*cpu_wait_sav)(void);	/* container to save mips_wait */

static struct ltq_cpufreq ltq_cpufreq_obj = {
	.list_head_module = LIST_HEAD_INIT(ltq_cpufreq_obj.list_head_module),
	.cpufreq_cur_state  = LTQ_CPUFREQ_PS_D0,
	.cpufreq_new_state  = LTQ_CPUFREQ_PS_D0,
	.cpu_scaling_rates = {
		500000000, /* D0 */
		500000000, /* D1 */
		500000000, /* D2 */
		500000000, /* D3 */
		0xA}, /* eol */
	.ddr_scaling_rates = {
		250000000, /* D0 */
		250000000, /* D1 */
		250000000, /* D2 */
		250000000, /* D3 */
		0xA}, /* eol */
	.ltq_state_change_control = 0,
	.ltq_clk_change_cb = NULL,
	.regulator = NULL,
	.polling_period = 10, /*10 seconds*/
	.ps_threshold_dp = {
		.th_d0 = 1,
		.th_d1 = 1,
		.th_d2 = 1,
		.th_d3 = 100,
		},
	.frequency_up_threshold = 60,
	.frequency_down_threshold = 20,
	.sampling_down_factor = 8
};

static int ltq_state_change_enable = 1;

/** \ingroup LTQ_CPUFREQ
\brief  The powerState request ringBuffer collect the incoming power state
	requests. The ringBuffer has a size of LTQ_REQ_BUFFER_SIZE,
	and will continuously start from the beginning with each overflow.
*/
static struct ltq_cpufreq_req_state ltq_cpufreq_reqBuffer[LTQ_REQ_BUFFER_SIZE];

/** \ingroup LTQ_CPUFREQ
\brief  The reqGetIndex points to the next request entry to be processed.
*/
static unsigned int ltq_cpufreq_reqGetIndex;

/** \ingroup LTQ_CPUFREQ
\brief  The reqPutIndex points to the next free entry inside the ringBuffer
			to place the next incoming request.
*/
static unsigned int ltq_cpufreq_reqPutIndex;

/** \ingroup LTQ_CPUFREQ
\brief  The reqBufferSize is the ringBuffer watchdog to signalize a real
	overflow of the ringBuffer. A real overflow means that the
	reqbuffer contains already LTQ_REQ_BUFFER_SIZE request entries
	and a new incoming request can not be placed and will be discarded.
*/
static unsigned int ltq_cpufreq_reqBufferSize;


/* module names; array correspond to enum enum ltq_cpufreq_module */
char *ltq_cpufreq_mod[LTQ_CPUFREQ_MODULE_ID_MAX+1] = {
	"CPU" ,
	"WLAN" ,
	"VE" ,
	"PPE" ,
	"SWITCH" ,
	"UART" ,
	"GPTC" ,
	"PCIE" ,
	"USB" ,
	"DEU" ,
	"DP" ,
	"DPL" ,
	"ETH" ,
	"PATM" ,
	"I2C" ,
	"INVALID" ,
};

/* CPUFREQ specific power state translation */
const char *ltq_cpufreq_pst[] = {
	"UNDEF",
	"D0",
	"D1",
	"D2",
	"D3",
	"D0D3",
	"BOOST"
};

/** \ingroup LTQ_CPUFREQ
\brief  hold the current status if a sub system is busy and will deny
	a freq change. Each sub system is represented by one bit position.
	'1' sub system is busy and will deny a frequency change request
	'0' sub system is idle and can accept a frequency change request
*/
static int ltq_subsys_busy;

struct cpufreq_frequency_table ltq_freq_tab[] = {
	{.index = LTQ_CPUFREQ_PS_D0, .frequency = CPUFREQ_ENTRY_INVALID,},
	{.index = LTQ_CPUFREQ_PS_D1, .frequency = CPUFREQ_ENTRY_INVALID,},
	{.index = LTQ_CPUFREQ_PS_D2, .frequency = CPUFREQ_ENTRY_INVALID,},
	{.index = LTQ_CPUFREQ_PS_D3, .frequency = CPUFREQ_ENTRY_INVALID,},
	{.index = 0, .frequency = CPUFREQ_TABLE_END,}
};


static struct freq_attr *ltq_freq_attr[] = {
	&cpufreq_freq_attr_scaling_available_freqs,
	NULL,
};

struct cpufreq_driver ltq_cpufreq_driver = {

	.name       = "ltq_cpufreq_drv "VERSION"\n",
	.owner      = THIS_MODULE,
	.init       = ltq_cpufreq_init,
	.flags      = 0,
	.verify     = ltq_cpufreq_verify,
	.setpolicy  = NULL,
	.target     = ltq_cpufreq_target,
	.get        = ltq_cpufreq_getfreq_khz,
	.getavg     = NULL,
	.exit       = ltq_cpufreq_exit,
	.suspend    = NULL,
	.resume     = ltq_cpufreq_resume,
	.attr       = ltq_freq_attr,
};

static int ltq_cpufreq_state_set(enum ltq_cpufreq_state ps)
{
	int retval = 0;
	unsigned long *cpu_freq;
	unsigned long freq;
	struct clk *clk = clk_get_sys("cpu", "cpu");

	if (clk == NULL) {
		pr_err("CPU clock structure not found\n");
		return -1;
	}
	cpu_freq = ltq_cpufreq_obj.cpu_scaling_rates;
	if (cpu_freq == NULL)
		return -1;

	switch (ps) {
	case LTQ_CPUFREQ_PS_D0:
		{
			freq = *(cpu_freq +
				(LTQ_CPUFREQ_PS_D0 - LTQ_CPUFREQ_PS_D0));
			ltq_cpufreq_obj.cpufreq_new_state = LTQ_CPUFREQ_PS_D0;
			retval = clk_set_rate(clk, freq);
			if (retval == 0) {
				ltq_cpufreq_obj.cpufreq_cur_state =
				ltq_cpufreq_obj.cpufreq_new_state;
			}
			break;
		}
	case LTQ_CPUFREQ_PS_D1:
		{
			freq = *(cpu_freq +
				(LTQ_CPUFREQ_PS_D1 - LTQ_CPUFREQ_PS_D0));
			ltq_cpufreq_obj.cpufreq_new_state = LTQ_CPUFREQ_PS_D1;
			retval = clk_set_rate(clk, freq);
			if (retval == 0) {
				ltq_cpufreq_obj.cpufreq_cur_state =
				ltq_cpufreq_obj.cpufreq_new_state;
			}
			break;
		}
	case LTQ_CPUFREQ_PS_D2:
		{
			freq = *(cpu_freq +
				(LTQ_CPUFREQ_PS_D2 - LTQ_CPUFREQ_PS_D0));
			ltq_cpufreq_obj.cpufreq_new_state = LTQ_CPUFREQ_PS_D2;
			retval = clk_set_rate(clk, freq);
			if (retval == 0) {
				ltq_cpufreq_obj.cpufreq_cur_state =
				ltq_cpufreq_obj.cpufreq_new_state;
			}
			break;
		}
	case LTQ_CPUFREQ_PS_D3:
		{
			freq = *(cpu_freq +
				(LTQ_CPUFREQ_PS_D3 - LTQ_CPUFREQ_PS_D0));
			ltq_cpufreq_obj.cpufreq_new_state = LTQ_CPUFREQ_PS_D3;
			retval = clk_set_rate(clk, freq);
			if (retval == 0) {
				ltq_cpufreq_obj.cpufreq_cur_state =
				ltq_cpufreq_obj.cpufreq_new_state;
			}
			break;
		}
	default:
		{
			break;
		}
	}
	return retval;
}


/**
	Callback function registered at the CPUFREQ.
	Enable/Disable of the mips_core clock gating (mips wait instruction).

	\param[in]  pwrStateEna
		- 1 enabled
		- 0 disabled

	\return Returns value as follows:
		- 0: if successful
		- < 0: in case of an error
*/
static int ltq_cpucg_pwrFeatureSwitch(int pwrStateEna)
{
	if (pwrStateEna == 1) {
		cpu_wait = cpu_wait_sav;
		cpuwait_feature_cgs.powerFeatureStat = 1;
		return 0;
	}
	if (pwrStateEna == 0) {
		cpu_wait = 0;
		cpuwait_feature_cgs.powerFeatureStat = 0;
		return 0;
	}
	return -1;
}

/**
	Callback function registered at the PMCU.
	This function is called by the PMCU to get the current power state
	status of the cpu frequency driver.

	\param[out]   pmcuState  current power state

	\return Returns value as follows:
		- IFX_PMCU_RETURN_SUCCESS: if successful
		- IFX_PMCU_RETURN_ERROR: in case of an error
*/
static int ltq_cpufreq_state_get(enum ltq_cpufreq_state *pmcuState)
{
	*pmcuState = ltq_cpufreq_obj.cpufreq_cur_state;
	return 0;
}

struct ltq_cpufreq *ltq_cpufreq_get(void)
{
	return &ltq_cpufreq_obj;
}
EXPORT_SYMBOL(ltq_cpufreq_get);

int ltq_cpufreq_state_change_disable(void)
{

	ltq_state_change_enable = 0;
	ltq_cpufreq_state_req(LTQ_CPUFREQ_MODULE_CPU, 0, LTQ_CPUFREQ_PS_D0);
	return 0;
}

int ltq_cpufreq_state_change_enable(void)
{

	ltq_state_change_enable = 1;
	ltq_cpufreq_state_req(LTQ_CPUFREQ_MODULE_CPU, 0, LTQ_CPUFREQ_PS_D3);
	return 0;
}

static void ltq_cpufreq_process_req(struct work_struct *work)
{
	struct cpufreq_policy *policy = NULL;
	struct ltq_cpufreq_req_state reqState;

	pr_debug("%s is called\n", __func__);

	while (ltq_cpufreq_reqBufferSize > 0) {
		reqState = ltq_cpufreq_get_req();
		if (reqState.reqId == LTQ_CPUFREQ_PENDING_REQ_ERROR) {
			pr_err("CPUFREQ Request Buffer underflow\n");
			return;
		}
		if (reqState.reqId == LTQ_CPUFREQ_NO_PENDING_REQ) {
			pr_err("No valid CPUFREQ Request in buffer!!!!!\n");
			return;
		}

		if (!ltq_state_change_enable) {
			pr_debug("Frequency down scaling disabled.\n");
			reqState.moduleState.pmcuState = LTQ_CPUFREQ_PS_D0;
		}
		policy = cpufreq_cpu_get(smp_processor_id());
		if (!policy)
			continue;

		if (reqState.moduleState.pmcuState == LTQ_CPUFREQ_PS_D0) {
			__cpufreq_driver_target(policy, policy->max,
							CPUFREQ_RELATION_L);
		} else {
			__cpufreq_driver_target(policy, policy->min,
							CPUFREQ_RELATION_L);
		}
	}
	return;
}

static void ltq_set_subsys_busy(unsigned int busy,
				enum ltq_cpufreq_module module)
{
	unsigned long iflags;

	spin_lock_irqsave(&ltq_cpufreq_obj.ltq_cpufreq_lock, iflags);

	if (busy == 0)
		ltq_subsys_busy &= ~(0x1<<module);
	else if (busy == 1)
		ltq_subsys_busy |= 0x1<<module;
	else
		ltq_subsys_busy = 0;

	spin_unlock_irqrestore(&ltq_cpufreq_obj.ltq_cpufreq_lock, iflags);
}

int ltq_cpufreq_state_req(enum ltq_cpufreq_module module,
				unsigned char moduleNr,
				enum ltq_cpufreq_state newState)
{
	struct cpufreq_policy *policy = NULL;
	struct ltq_cpufreq_req_state reqState;

	LTQ_PR_DBG("%s is called: Module=%s, ModuleNr=%d, State=%s\n",
			  __func__, ltq_cpufreq_mod[module], moduleNr,
			 ltq_cpufreq_pst[newState]);
	/* check if the acception of the powerstate request is enabled */
	if (ltq_cpufreq_obj.ltq_state_change_control == 0) {
		pr_debug("StateChange disabled. Request will be rejected\n");
		return LTQ_CPUFREQ_RETURN_DENIED;
	}

	if (ltq_cpufreq_driver.flags & CPUFREQ_PM_NO_DENY) {
		/*we are in special thermal mode where we deny any up-scaling*/
		return LTQ_CPUFREQ_RETURN_DENIED;
	}

	/*only D0 request will be processed. Down scaling will be triggered by
	  dyn. governor*/
	if (newState != LTQ_CPUFREQ_PS_D0) {
		/*special handling for PPE. DS request handled by DP metering*/
		if ((module & LTQ_CPUFREQ_MODULE_DP) == LTQ_CPUFREQ_MODULE_DP)
			ltq_set_subsys_busy(0, LTQ_CPUFREQ_MODULE_PPE);

		ltq_set_subsys_busy(0, module);
		return 0;
	} else {
		ltq_set_subsys_busy(1, module);
	}

	/*check if the new request is already active*/
	policy = cpufreq_cpu_get(smp_processor_id());
	if (policy) {
		if (ltq_cpufreq_get_ps_from_khz(policy->cur) == newState)
			return 0;
	}

	reqState.moduleState.pmcuModule = module;
	reqState.moduleState.pmcuModuleNr = moduleNr;
	reqState.moduleState.pmcuState = newState;
	reqState.reqId = LTQ_CPUFREQ_NO_PENDING_REQ;

	/* put new powerState request into request buffer */
	if (ltq_cpufreq_put_req(reqState) < 0)
		pr_err("CPUFREQ RequestBuffer overflow !!!\n");

	if (system_wq == NULL) {
		pr_debug("No workqueue available\n");
		return -EFAULT;
	}
	/* feed the system_wq workqueue.
	   fetch powerState request from requestBuffer by the wq */
	if (!queue_work(system_wq, &work_obj))
		pr_debug("workqueue successfully loaded\n");

	return 0;
}
EXPORT_SYMBOL(ltq_cpufreq_state_req);

int ltq_cpufreq_get_poll_period(enum ltq_cpufreq_module module,
				unsigned char moduleNr)
{
	/*currently we define same polling period for all modules*/
	return ltq_cpufreq_obj.polling_period;
}
EXPORT_SYMBOL(ltq_cpufreq_get_poll_period);


struct ltq_cpufreq_threshold *ltq_cpufreq_get_threshold(
				enum ltq_cpufreq_module module,
				unsigned char moduleNr)
{
	return &ltq_cpufreq_obj.ps_threshold_dp;
}
EXPORT_SYMBOL(ltq_cpufreq_get_threshold);

static int ltq_cpufreq_put_req(struct ltq_cpufreq_req_state req)
{
	int i;
	unsigned long iflags;

	pr_debug("%s is called\n", __func__);
	if (ltq_cpufreq_reqBufferSize >= LTQ_REQ_BUFFER_SIZE)
		return -1;

	spin_lock_irqsave(&ltq_cpufreq_obj.ltq_cpufreq_lock, iflags);

	/* first check if there is already a request for this module pending.
	   If this is the case, reject the actual one. */
	for (i = 0; i < LTQ_REQ_BUFFER_SIZE; i++) {
		if (ltq_cpufreq_reqBuffer[i].reqId ==
			LTQ_CPUFREQ_NO_PENDING_REQ) {
			continue;
		}
		if ((ltq_cpufreq_reqBuffer[i].moduleState.pmcuModule ==
			 req.moduleState.pmcuModule) &&
			(ltq_cpufreq_reqBuffer[i].moduleState.pmcuModuleNr ==
			 req.moduleState.pmcuModuleNr) &&
			(ltq_cpufreq_reqBuffer[i].moduleState.pmcuState ==
			 req.moduleState.pmcuState)) {
			spin_unlock_irqrestore(
				&ltq_cpufreq_obj.ltq_cpufreq_lock, iflags);
			pr_debug("%s multiple requests from same module.\n",
				 __func__);
			return 0;
		}
	}

	req.reqId = LTQ_CPUFREQ_PENDING_REQ;
	memcpy(&ltq_cpufreq_reqBuffer[ltq_cpufreq_reqPutIndex],
		   &req, sizeof(req));
	ltq_cpufreq_reqPutIndex++;
	if (ltq_cpufreq_reqPutIndex >= LTQ_REQ_BUFFER_SIZE)
		ltq_cpufreq_reqPutIndex = 0;

	ltq_cpufreq_reqBufferSize++;
	spin_unlock_irqrestore(&ltq_cpufreq_obj.ltq_cpufreq_lock, iflags);
	return 0;
}

static struct ltq_cpufreq_req_state ltq_cpufreq_get_req(void)
{
	struct ltq_cpufreq_req_state req;

	pr_debug("%s is called\n", __func__);
	memset(&req, 0, sizeof(req));
	req.reqId = LTQ_CPUFREQ_PENDING_REQ_ERROR;

	if (!ltq_cpufreq_reqBufferSize)
		return req;

	ltq_cpufreq_reqBufferSize--;
	memcpy(&req, &ltq_cpufreq_reqBuffer[ltq_cpufreq_reqGetIndex],
		   sizeof(req));
	ltq_cpufreq_reqBuffer[ltq_cpufreq_reqGetIndex].reqId =
	LTQ_CPUFREQ_NO_PENDING_REQ;
	ltq_cpufreq_reqGetIndex++;
	if (ltq_cpufreq_reqGetIndex >= LTQ_REQ_BUFFER_SIZE)
		ltq_cpufreq_reqGetIndex = 0;

	return req;
}

static unsigned long ltq_get_min_freq(unsigned long *freq_list)
{
	int i;
	unsigned long freq = 0xdeadbeef;

	for (i = 0; i < 4; i++) {
		if ((*(freq_list + i) != 0) &&
			(*(freq_list + i) < freq)) {
			freq = *(freq_list + i);
		}
	}
	return freq;
}

/* convert given frequency in kHz to corresponding power state */
enum ltq_cpufreq_state ltq_cpufreq_get_ps_from_khz(unsigned int freqkhz)
{
	struct cpufreq_frequency_table *ltq_freq_tab_p;
	ltq_freq_tab_p = ltq_freq_tab;

	while (ltq_freq_tab_p->frequency != CPUFREQ_TABLE_END) {
		if (ltq_freq_tab_p->frequency == freqkhz)
			return (enum ltq_cpufreq_state)ltq_freq_tab_p->index;

		ltq_freq_tab_p++;
	}
	return (enum ltq_cpufreq_state)ltq_freq_tab_p->index;
}
EXPORT_SYMBOL(ltq_cpufreq_get_ps_from_khz);

/* convert given power state to corresponding frequency in kHz */
unsigned int ltq_cpufreq_get_khz_from_ps(enum ltq_cpufreq_state ps)
{
	unsigned long *cpu_freq;

	cpu_freq = ltq_cpufreq_obj.cpu_scaling_rates;
	if (cpu_freq == NULL)
		return -1;

	return (*(cpu_freq + (ps - LTQ_CPUFREQ_PS_D0)))/1000;
}
EXPORT_SYMBOL(ltq_cpufreq_get_khz_from_ps);

unsigned int ltq_cpufreq_getfreq_khz(unsigned int cpu)
{
	struct clk *clk = clk_get_sys("cpu", "cpu");

	if (clk == NULL) {
		pr_err("CPU clock structure not found\n");
		return -1;
	}

	return clk_get_rate(clk) / 1000;
}
EXPORT_SYMBOL(ltq_cpufreq_getfreq_khz);

int ltq_get_subsys_busy(void)
{
	return ltq_subsys_busy;
}
EXPORT_SYMBOL(ltq_get_subsys_busy);

int ltq_register_clk_change_cb(int (*callback) (void *))
{
	pr_debug("%s is called\n", __func__);
	ltq_cpufreq_obj.ltq_clk_change_cb = callback;
	return 0;
}
EXPORT_SYMBOL(ltq_register_clk_change_cb);

int ltq_cpufreq_mod_list(struct list_head *head, int add)
{
	unsigned long iflags;

	pr_debug("%s is called\n", __func__);
	spin_lock_irqsave(&ltq_cpufreq_obj.ltq_cpufreq_lock, iflags);
	if (add == 1)
		list_add_tail(head, &ltq_cpufreq_obj.list_head_module);
	else
		list_del(head);

	spin_unlock_irqrestore(&ltq_cpufreq_obj.ltq_cpufreq_lock, iflags);
	return 0;
}
EXPORT_SYMBOL(ltq_cpufreq_mod_list);

struct cpufreq_policy *policy_sav;

static int ltq_cpufreq_init(struct cpufreq_policy *policy)
{
	unsigned long freq;
	struct clk *clk = clk_get_sys("cpu", "cpu");

	pr_debug("%s is called\n", __func__);
	if (clk == NULL) {
		pr_err("CPU clock structure not found\n");
		return -EINVAL;
	}
	freq = ltq_get_min_freq(ltq_cpufreq_obj.cpu_scaling_rates);
	if (freq == 0xdeadbeef) {
		pr_err("no valid scaling frequencies defined\n");
		return -EINVAL;
	}
	policy_sav = policy;
	cpumask_copy(policy->cpus, cpu_possible_mask);
	policy->cpuinfo.min_freq = freq / 1000;
	policy->cpuinfo.max_freq = ltq_cpufreq_obj.cpu_scaling_rates[0]/1000;
	policy->cpuinfo.transition_latency = LTQ_CPUFREQ_TRANSITION_LATENCY;
	policy->cur      = (clk_get_rate(clk) / 1000);
	policy->min      = freq / 1000;
	policy->max      = ltq_cpufreq_obj.cpu_scaling_rates[0]/1000;
	policy->policy   = 0;

	cpufreq_frequency_table_cpuinfo(policy, ltq_freq_tab);
	cpufreq_frequency_table_get_attr(ltq_freq_tab, policy->cpu);
	return 0;
}

static int ltq_cpufreq_verify(struct cpufreq_policy *policy)
{
	pr_debug("%s is called\n", __func__);
	cpufreq_frequency_table_verify(policy, ltq_freq_tab);
	return 0;
}

static int ltq_cpufreq_target(struct cpufreq_policy *policy,
				unsigned int target_freq,
				unsigned int relation)
{
	static struct cpufreq_policy *policy_sav;
	static unsigned int target_freq_sav;
	static unsigned int tab_index, ret;
	int freqOk = -1;
	struct cpufreq_freqs freqs;
#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
	unsigned int count_start;
#endif
	struct clk *clk = clk_get_sys("cpu", "cpu");

	pr_debug("%s is called\n", __func__);
	if (clk == NULL) {
		pr_err("CPU clock structure not found\n");
		return -EINVAL;
	}
	policy_sav = policy;
	target_freq_sav = target_freq;
	if (cpufreq_frequency_table_target(policy, ltq_freq_tab,
				target_freq, relation, &tab_index)) {
		return -EINVAL;
	}
	freqs.new = ltq_freq_tab[tab_index].frequency;
	freqs.old = (clk_get_rate(clk) / 1000);	/* get kHz */
	freqs.cpu = policy->cpu;
	/* interrupt the process here if frequency doesn't change */
	if (freqs.new == freqs.old)
		return 0; /* nothing to do */

	pr_debug("cpufreq_notify_transition_once, CPUFREQ_PRECHANGE\n");
#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
	count_start = ltq_count0_read();
#endif
	ret = cpufreq_notify_transition_once(policy, &freqs, CPUFREQ_PRECHANGE);
#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
	ltq_count0_diff(count_start, ltq_count0_read(),
					"prechange notification ", 0);
#endif
	if ((ret & NOTIFY_STOP_MASK) == NOTIFY_STOP_MASK) {
		ret = (ret >> 4) & 0x1F; /*mask module id*/
		LTQ_PR_DBG("Frequency scaling was denied by module %s\n",
				 ltq_cpufreq_mod[ret]);
	} else {

#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
		count_start = ltq_count0_read();
#endif
		if (ltq_cpufreq_state_set(ltq_freq_tab[tab_index].index))
			freqOk = -1;
		else
			freqOk = 0;

#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
		ltq_count0_diff(count_start, ltq_count0_read(),
						"change clock rate      ", 1);
#endif
	}

	if (freqOk < 0) {
		/* if freq change is denied, call post_change with new=old */
		freqs.new = freqs.old;
	}
	pr_debug("cpufreq_notify_transition_once, CPUFREQ_POSTCHANGE\n");
#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
	count_start = ltq_count0_read();
#endif
	cpufreq_notify_transition_once(policy, &freqs, CPUFREQ_POSTCHANGE);
#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
	ltq_count0_diff(count_start, ltq_count0_read(),
					"postchange notification", 2);
#endif
	if ((freqOk >= 0) && (ltq_cpufreq_obj.ltq_clk_change_cb != NULL))
		(void)ltq_cpufreq_obj.ltq_clk_change_cb(NULL);

	/* copy loops_per_jiffy to cpu_data */
	/*cpu_data[smp_processor_id()].udelay_val = loops_per_jiffy;*/
	return freqOk;
}

static int ltq_cpufreq_exit(struct cpufreq_policy *policy)
{
	pr_debug("%s is called\n", __func__);
	return 0;
}

static int ltq_cpufreq_resume(struct cpufreq_policy *policy)
{
	pr_debug("%s is called\n", __func__);
	return 0;
}


#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
/* time measurement routines for debug purpose only */
	#define COUNT0_MAX 0xFFFFFFFF

static unsigned int cpu_freq_MHz;

int ltq_count0_diff(u32 count_start, u32 count_end, char *ident, int index)
{
	u64 diff, time64;
	int time;

	if (count_start >= count_end)
		diff = (COUNT0_MAX - count_start) + count_end;
	else
		diff = count_end - count_start;

	time64 = 2 * diff; /* count runs with cpu/2 */
	do_div(time64, cpu_freq_MHz);

	/* check for realistic time */
	if (time64 > 0x7FFFFFFF) {
		pr_debug("timer overflow in freq change measurement\n");
		time = 0x7FFFFFFF;
	}
	time = (int)time64;

	switch (index) {
	case 0:/*PRE*/
		{
			ltq_cpufreq_latency_g[0].comment = ident;
			ltq_cpufreq_latency_g[0].cpu_frequency = cpu_freq_MHz;
			if (time < ltq_cpufreq_latency_g[0].min_time)
				ltq_cpufreq_latency_g[0].min_time = time;

			ltq_cpufreq_latency_g[0].cur_time = time;
			if (time > ltq_cpufreq_latency_g[0].max_time)
				ltq_cpufreq_latency_g[0].max_time = time;
			break;
		}
	case 1:/*STATE CHANGE*/
		{
			ltq_cpufreq_latency_g[1].comment = ident;
			ltq_cpufreq_latency_g[1].cpu_frequency = cpu_freq_MHz;
			if (time < ltq_cpufreq_latency_g[1].min_time)
				ltq_cpufreq_latency_g[1].min_time = time;

			ltq_cpufreq_latency_g[1].cur_time = time;
			if (time > ltq_cpufreq_latency_g[1].max_time)
				ltq_cpufreq_latency_g[1].max_time = time;
			break;
		}
	case 2:/*POST*/
		{
			ltq_cpufreq_latency_g[2].comment = ident;
			ltq_cpufreq_latency_g[2].cpu_frequency = cpu_freq_MHz;
			if (time < ltq_cpufreq_latency_g[2].min_time)
				ltq_cpufreq_latency_g[2].min_time = time;

			ltq_cpufreq_latency_g[2].cur_time = time;
			if (time > ltq_cpufreq_latency_g[2].max_time)
				ltq_cpufreq_latency_g[2].max_time = time;
			break;
		}
	default:
			break;
	}

	return time;
}
EXPORT_SYMBOL(ltq_count0_diff);

u32 ltq_count0_read(void)
{
	struct clk *clk = clk_get_sys("cpu", "cpu");

	if (clk == NULL) {
		pr_err("CPU clock structure not found\n");
		return -1;
	}
	cpu_freq_MHz = clk_get_rate(clk)/1000000;
	return read_c0_count();
}
EXPORT_SYMBOL(ltq_count0_read);

#endif /* CONFIG_LTQ_CPUFREQ_DEBUG */

/*======================================================================*/
/* START: SYSFS HELPER FUNCTION CALLED FROM LINUX CPUFREQ		*/
/*======================================================================*/
static int ltq_cpufreq_read_scaling_clks(char *buf)
{
	int len = 0;
	unsigned long ngi, cbm, ddr;

	ngi = clk_get_rate(clk_get_sys("xbar", "xbar"))/1000;
	if (ngi) {
		cbm = clk_get_rate(clk_get_sys("ppe", "ppe"))/1000;
		ddr = clk_get_rate(clk_get_sys("ddr", "ddr"))/1000;

		len += sprintf(buf+len, " NGI frequency: %lu\n", ngi);
		len += sprintf(buf+len, " CBM frequency: %lu\n", cbm);
		len += sprintf(buf+len, " DDR frequency: %lu\n", ddr);
	}
	return len;
}

static int ltq_cpufreq_read_dvs(char *buf)
{
	int len = 0;

	len += sprintf(buf+len, "  ltq_dvs_support = %s\n\n",
	(ltq_cpufreq_obj.regulator != NULL) ? "AVAILABLE" : "NOT AVAILABLE");
	return len;
}

static int ltq_cpufreq_read_control(char *buf)
{
	int len = 0;

	len += sprintf(buf+len, "  ltq_state_change_control = %s\n\n",
	(ltq_cpufreq_obj.ltq_state_change_control == 1) ? "ON" : "OFF");
	return len;
}

static int ltq_cpufreq_write_control(const char *buf, size_t count)
{
	if (buf == NULL)
		return 0;

	if (sysfs_streq(buf, "on\n") || sysfs_streq(buf, "1")) {
		ltq_cpufreq_obj.ltq_state_change_control = 1;
	} else if (sysfs_streq(buf, "off\n") || sysfs_streq(buf, "0")) {
		ltq_cpufreq_obj.ltq_state_change_control = 0;
		ltq_set_subsys_busy(0xFF, 0);
	} else {
		pr_err("wrong input parameter. Use on | off | 1 | 0\n");
	}
	return count;
}

static int ltq_cpufreq_read_force_ds(char *buf)
{
	int len = 0;
	u8 no_deny = ltq_cpufreq_driver.flags & CPUFREQ_PM_NO_DENY;

	len += sprintf(buf+len, "  ltq_force_down_scaling = %s\n\n",
	(no_deny == CPUFREQ_PM_NO_DENY) ? "ON" : "OFF");
	return len;
}

static int ltq_cpufreq_write_force_ds(const char *buf, size_t count)
{
	if (buf == NULL)
		return 0;

	if (sysfs_streq(buf, "on\n") || sysfs_streq(buf, "1")) {
		ltq_cpufreq_driver.flags |= CPUFREQ_PM_NO_DENY;
		ltq_set_subsys_busy(0xFF, 0);
	} else if (sysfs_streq(buf, "off\n") || sysfs_streq(buf, "0")) {
		ltq_cpufreq_driver.flags &= ~CPUFREQ_PM_NO_DENY;
	} else {
		pr_err("wrong input parameter. Use on | off | 1 | 0\n");
	}
	return count;
}

static int ltq_cpufreq_read_alert(char *buf)
{
	int len = 0;
	len += sprintf(buf+len, "  alert_msg = %s\n\n",
	(alert_msg == 1) ? "ON" : "OFF");
	return len;
}

static int ltq_cpufreq_write_alert(const char *buf, size_t count)
{
	if (buf == NULL)
		return 0;

	if (sysfs_streq(buf, "on\n") || sysfs_streq(buf, "1"))
		alert_msg = 1;
	else if (sysfs_streq(buf, "off\n") || sysfs_streq(buf, "0"))
		alert_msg = 0;
	else
		pr_err("wrong input parameter. Use on | off | 1 | 0\n");

	return count;
}

static int ltq_cpufreq_read_poll_period(char *buf)
{
	int len = 0;

	len += sprintf(buf+len, "%d [sec]\n",
		       ltq_cpufreq_obj.polling_period);
	return len;
}

static int ltq_cpufreq_write_poll_period(const char *buf, size_t count)
{
	unsigned int ret;

	if (buf == NULL)
		return 0;
	ret = sscanf(buf, "%d", &ltq_cpufreq_obj.polling_period);
	if (ret != 1)
		return -EINVAL;

	return count;
}

static int ltq_cpufreq_read_subsys_busy(char *buf)
{
	int len = 0;
	int i;

	len += sprintf(buf+len, "ltq_subsys_busy=0x%x\n", ltq_subsys_busy);
	for (i = 0; i < LTQ_CPUFREQ_MODULE_ID_MAX; i++) {
		if ((ltq_subsys_busy >> i) & 0x1)
			len += sprintf(buf+len, "%s busy\n",
							ltq_cpufreq_mod[i]);
	}
	return len;
}

static int ltq_cpufreq_write_subsys_busy(const char *buf, size_t count)
{
	unsigned int ret;

	if (buf == NULL)
		return 0;
	ret = sscanf(buf, "%d", &ltq_subsys_busy);
	if (ret != 1)
		return -EINVAL;

	return count;
}

static int ltq_cpufreq_read_frequency_up_threshold(char *buf)
{
	int len = 0;

	len += sprintf(buf+len, "%d\n",
				ltq_cpufreq_obj.frequency_up_threshold);
	return len;
}

static int ltq_cpufreq_write_frequency_up_threshold(const char *buf,
								size_t count)
{
	unsigned int ret;

	if (buf == NULL)
		return 0;
	ret = sscanf(buf, "%d", &ltq_cpufreq_obj.frequency_up_threshold);
	if (ret != 1)
		return -EINVAL;

	return count;
}

static int ltq_cpufreq_read_frequency_down_threshold(char *buf)
{
	int len = 0;

	len += sprintf(buf+len, "%d\n",
				ltq_cpufreq_obj.frequency_down_threshold);
	return len;
}

static int ltq_cpufreq_write_frequency_down_threshold(const char *buf,
								size_t count)
{
	unsigned int ret;

	if (buf == NULL)
		return 0;
	ret = sscanf(buf, "%d", &ltq_cpufreq_obj.frequency_down_threshold);
	if (ret != 1)
		return -EINVAL;

	return count;
}

static int ltq_cpufreq_read_sampling_down_factor(char *buf)
{
	int len = 0;

	len += sprintf(buf+len, "%d\n",
				ltq_cpufreq_obj.sampling_down_factor);
	return len;
}

static int ltq_cpufreq_write_sampling_down_factor(const char *buf,
								size_t count)
{
	unsigned int ret;

	if (buf == NULL)
		return 0;
	ret = sscanf(buf, "%d", &ltq_cpufreq_obj.sampling_down_factor);
	if (ret != 1)
		return -EINVAL;

	return count;
}

static int ltq_cpufreq_read_module_info(char *buf)
{
	int len = 0;
	struct ltq_cpufreq_module_info *cur_list_pos;
	enum ltq_cpufreq_state pState;

	len += sprintf(buf+len, " sub system state change = %s\n\n",
	(ltq_cpufreq_obj.ltq_state_change_control == 1) ? "ON" : "OFF");
	list_for_each_entry(cur_list_pos,
				&ltq_cpufreq_obj.list_head_module, list) {
		len += sprintf(buf+len, " Comment = %s\n", cur_list_pos->name);
		if (cur_list_pos->pmcuModule < LTQ_CPUFREQ_MODULE_ID_MAX) {
			len += sprintf(buf+len, " Mod = %s\t",
				ltq_cpufreq_mod[cur_list_pos->pmcuModule]);
			if (cur_list_pos->pmcuModuleNr != 0) {
				len += sprintf(buf+len, "  SubNo = %d",
						cur_list_pos->pmcuModuleNr);
			}
		}
		if (cur_list_pos->ltq_cpufreq_state_get != NULL) {
			cur_list_pos->ltq_cpufreq_state_get(&pState);
			len += sprintf(buf+len, "\tPS = %s\t",
						   ltq_cpufreq_pst[pState]);
		}
		if ((cur_list_pos->powerFeatureStat == 1)
			|| (cur_list_pos->powerFeatureStat == 0)) {
			len += sprintf(buf+len, "\tPowerFeature = %s\n\n",
			(cur_list_pos->powerFeatureStat == 1 ? "ENABLED" :
							"DISABLED"));
		} else {
			len += sprintf(buf+len, "\n\n");
		}
	}
	len += sprintf(buf+len, "\n");
	return len;
}

int ltq_cpufreq_write_module_info(const char *buf, size_t count)
{
	int par1 = 0;
	char *s = (char *)buf;
	char *p0;
	char *p1;
	struct ltq_cpufreq_module_info *c;

	if (s == NULL)
		return 0;

	p0 = strsep(&s, "/");
	p1 = strsep(&s, "/");
	if ((p0 == NULL) || (p1 == NULL))
		return 0;

	*(p1 + strlen(p1) - 1) = '\0'; /* remove LF */
	if (strcmp(p1, "on") == 0)
		par1 = 1;

	list_for_each_entry(c, &ltq_cpufreq_obj.list_head_module, list) {
		if (strcmp(c->name, p0) == 0) {
			/*pr_debug("power feature found, %s\n",p0);*/
			if (c->ltq_cpufreq_pwr_feature_switch != NULL) {
				if (c->
				ltq_cpufreq_pwr_feature_switch(par1) >= 0) {
					c->powerFeatureStat = par1;
					pr_debug("pwr feature set, %s, to %d\n",
							 p0, par1);
				}
			}
		}
	}
	return count;
}

#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
static int ltq_cpufreq_read_freqchangelatency(char *buf)
{
	int len = 0;

	len += sprintf(buf+len, "\n\n");
	len += sprintf(buf+len, "%s: cur_time measured = %d[us], @%dHz,\n",
				   ltq_cpufreq_latency_g[0].comment,
				   ltq_cpufreq_latency_g[0].cur_time,
				   ltq_cpufreq_latency_g[0].cpu_frequency);
	len += sprintf(buf+len, "%s: min_time measured = %d[us]\n",
				   ltq_cpufreq_latency_g[0].comment,
				   ltq_cpufreq_latency_g[0].min_time);
	len += sprintf(buf+len, "%s: max_time measured = %d[us]\n",
				   ltq_cpufreq_latency_g[0].comment,
				   ltq_cpufreq_latency_g[0].max_time);
	len += sprintf(buf+len, "%s: cur_time measured = %d[us], @%dHz,\n",
				   ltq_cpufreq_latency_g[1].comment,
				   ltq_cpufreq_latency_g[1].cur_time,
				   ltq_cpufreq_latency_g[1].cpu_frequency);
	len += sprintf(buf+len, "%s: min_time measured = %d[us]\n",
				   ltq_cpufreq_latency_g[1].comment,
				   ltq_cpufreq_latency_g[1].min_time);
	len += sprintf(buf+len, "%s: max_time measured = %d[us]\n",
				   ltq_cpufreq_latency_g[1].comment,
				   ltq_cpufreq_latency_g[1].max_time);
	len += sprintf(buf+len, "%s: cur_time measured = %d[us], @%dHz,\n",
				   ltq_cpufreq_latency_g[2].comment,
				   ltq_cpufreq_latency_g[2].cur_time,
				   ltq_cpufreq_latency_g[2].cpu_frequency);
	len += sprintf(buf+len, "%s: min_time measured = %d[us]\n",
				   ltq_cpufreq_latency_g[2].comment,
				   ltq_cpufreq_latency_g[2].min_time);
	len += sprintf(buf+len, "%s: max_time measured = %d[us]\n",
				   ltq_cpufreq_latency_g[2].comment,
				   ltq_cpufreq_latency_g[2].max_time);

	len += sprintf(buf+len, "\n\n");
	return len;
}

static int ltq_cpufreq_write_freqchangelatency(const char *buf,
						unsigned long count)
{
	int i;

	if (sysfs_streq(buf, "reset\n") || sysfs_streq(buf, "0")) {
		/* reset the freq change latency buffer */
		for (i = 0; i < 3; i++) {
			ltq_cpufreq_latency_g[i].max_time = -1;
			ltq_cpufreq_latency_g[i].min_time = 999999999;
			ltq_cpufreq_latency_g[i].cur_time = -1;
			ltq_cpufreq_latency_g[i].cpu_frequency = 0;
			ltq_cpufreq_latency_g[i].comment  = "Not Defined";
		}
	} else {
		pr_err("wrong input parameter. Use reset | 0\n");
	}

	return count;
}

/* test function to check the correct setting for loops_per_jiffy */
static int ltq_cpufreq_read_udelay1000us(char *buf)
{
	int len = 0;
	u32 count_start;
	int time;

	count_start = ltq_count0_read();
	udelay(1000); /* 1000us */
	time = ltq_count0_diff(count_start, ltq_count0_read(), NULL, 0xFF);
	len += sprintf(buf+len, "\n\n");
	len += sprintf(buf+len, "This is a udelay/jiffies test to prove that\n"
				"the kernel timing settings are correct\n\n");
	len += sprintf(buf+len, "udelay measured = %d[us] for udelay 1000[us]",
				  time);
	len += sprintf(buf+len, "\n\n");
	return len;
}

static ssize_t show_ltq_freqchangelatency(struct cpufreq_policy *policy,
						char *buf)
{
	return ltq_cpufreq_read_freqchangelatency(buf);
}

static ssize_t store_ltq_freqchangelatency(struct cpufreq_policy *policy,
						const char *buf, size_t count)
{
	return ltq_cpufreq_write_freqchangelatency(buf, count);
}

static ssize_t show_ltq_udelay1000us(struct cpufreq_policy *policy, char *buf)
{
	return ltq_cpufreq_read_udelay1000us(buf);
}

cpufreq_freq_attr_rw(ltq_freqchangelatency);
cpufreq_freq_attr_ro(ltq_udelay1000us);
#endif /* CONFIG_LTQ_CPUFREQ_DEBUG */

static ssize_t show_ltq_moduleinfo(struct cpufreq_policy *policy, char *buf)
{
	return ltq_cpufreq_read_module_info(buf);
}

static ssize_t store_ltq_moduleinfo(struct cpufreq_policy *policy,
					const char *buf, size_t count)
{
	return ltq_cpufreq_write_module_info(buf, count);
}

static ssize_t show_ltq_control(struct cpufreq_policy *policy, char *buf)
{
	return ltq_cpufreq_read_control(buf);
}

static ssize_t store_ltq_control(struct cpufreq_policy *policy,
					 const char *buf, size_t count)
{
	return ltq_cpufreq_write_control(buf, count);
}

static ssize_t show_ltq_force_ds(struct cpufreq_policy *policy, char *buf)
{
	return ltq_cpufreq_read_force_ds(buf);
}

static ssize_t store_ltq_force_ds(struct cpufreq_policy *policy,
					 const char *buf, size_t count)
{
	return ltq_cpufreq_write_force_ds(buf, count);
}

static ssize_t show_ltq_alert(struct cpufreq_policy *policy, char *buf)
{
	return ltq_cpufreq_read_alert(buf);
}

static ssize_t store_ltq_alert(struct cpufreq_policy *policy,
					 const char *buf, size_t count)
{
	return ltq_cpufreq_write_alert(buf, count);
}

static ssize_t show_ltq_dvs(struct cpufreq_policy *policy, char *buf)
{
	return ltq_cpufreq_read_dvs(buf);
}

static ssize_t show_ltq_scaling_clks(struct cpufreq_policy *policy, char *buf)
{
	return ltq_cpufreq_read_scaling_clks(buf);
}

static ssize_t show_ltq_poll_period(struct cpufreq_policy *policy, char *buf)
{
	return ltq_cpufreq_read_poll_period(buf);
}

static ssize_t store_ltq_poll_period(struct cpufreq_policy *policy,
					 const char *buf, size_t count)
{
	return ltq_cpufreq_write_poll_period(buf, count);
}

static ssize_t show_ltq_subsysbusy(struct cpufreq_policy *policy, char *buf)
{
	return ltq_cpufreq_read_subsys_busy(buf);
}

static ssize_t store_ltq_subsysbusy(struct cpufreq_policy *policy,
					 const char *buf, size_t count)
{
	return ltq_cpufreq_write_subsys_busy(buf, count);
}

static ssize_t show_ltq_frequency_up_threshold(struct cpufreq_policy *policy,
								char *buf)
{
	return ltq_cpufreq_read_frequency_up_threshold(buf);
}

static ssize_t store_ltq_frequency_up_threshold(struct cpufreq_policy *policy,
					 const char *buf, size_t count)
{
	return ltq_cpufreq_write_frequency_up_threshold(buf, count);
}

static ssize_t show_ltq_frequency_down_threshold(struct cpufreq_policy *policy,
								char *buf)
{
	return ltq_cpufreq_read_frequency_down_threshold(buf);
}

static ssize_t store_ltq_frequency_down_threshold(struct cpufreq_policy *policy,
					 const char *buf, size_t count)
{
	return ltq_cpufreq_write_frequency_down_threshold(buf, count);
}

static ssize_t show_ltq_sampling_down_factor(struct cpufreq_policy *policy,
								char *buf)
{
	return ltq_cpufreq_read_sampling_down_factor(buf);
}

static ssize_t store_ltq_sampling_down_factor(struct cpufreq_policy *policy,
					 const char *buf, size_t count)
{
	return ltq_cpufreq_write_sampling_down_factor(buf, count);
}

cpufreq_freq_attr_rw(ltq_moduleinfo);
cpufreq_freq_attr_rw(ltq_control);
cpufreq_freq_attr_rw(ltq_alert);
cpufreq_freq_attr_rw(ltq_force_ds);
cpufreq_freq_attr_rw(ltq_poll_period);
cpufreq_freq_attr_rw(ltq_subsysbusy);
cpufreq_freq_attr_rw(ltq_frequency_up_threshold);
cpufreq_freq_attr_rw(ltq_frequency_down_threshold);
cpufreq_freq_attr_rw(ltq_sampling_down_factor);
cpufreq_freq_attr_ro(ltq_dvs);
cpufreq_freq_attr_ro(ltq_scaling_clks);

static struct attribute *default_attrs[] = {
	&ltq_moduleinfo.attr,
	&ltq_control.attr,
	&ltq_alert.attr,
	&ltq_force_ds.attr,
	&ltq_dvs.attr,
	&ltq_scaling_clks.attr,
	&ltq_poll_period.attr,
	&ltq_subsysbusy.attr,
	&ltq_frequency_up_threshold.attr,
	&ltq_frequency_down_threshold.attr,
	&ltq_sampling_down_factor.attr,
#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
	&ltq_freqchangelatency.attr,
	&ltq_udelay1000us.attr,
#endif
	NULL
};

static struct attribute_group ltq_stats_attr_group = {
	.attrs = default_attrs,
	.name = "ltq_stats",
};

static int ltq_cpufreq_stats_create(struct cpufreq_policy *policy)
{
	unsigned int ret = 0;
	struct cpufreq_policy *data;

	data = cpufreq_cpu_get(policy->cpu);
	if (data == NULL) {
		ret = -EINVAL;
		return ret;
	}
	ret = sysfs_create_group(&data->kobj, &ltq_stats_attr_group);
	cpufreq_cpu_put(data);
	return ret;
}
/*======================================================================*/
/* END: SYSFS HELPER FUNCTION CALLED FROM LANTIQ CPUFREQ		*/
/*======================================================================*/


static void get_frequencies(const u32 *addr, unsigned long *freqs, int len)
{
	int i;

	if (len > 4*sizeof(u32))
		len = 4*sizeof(u32); /* we allow currently only 4 u32 values */

	for (i = 0; i < (len/4); i++)
		*(freqs+i) = (unsigned long)*(addr+i);
}

static void of_get_cpuddr_scaling_rates(void)
{
	struct device_node *np;
	struct device_node *nc;
	int len;
	const u32 *addr;

	np = of_find_compatible_node(NULL, NULL, "lantiq,scaling-frequencies");
	if (!np) {
		pr_debug("No Scaling frequencies found");
		return;
	}

	for_each_available_child_of_node(np, nc) {
		addr = of_get_property(nc, "lantiq,cpuclocks", &len);
		if (addr != NULL) {
			get_frequencies(addr,
					&ltq_cpufreq_obj.cpu_scaling_rates[0],
					len);
		}
		addr = of_get_property(nc, "lantiq,ddrclocks", &len);
		if (addr != NULL) {
			get_frequencies(addr,
					&ltq_cpufreq_obj.ddr_scaling_rates[0],
					len);
		}
		addr = of_get_property(nc, "lantiq,threshold_dp", &len);
		if (addr != NULL) {
			if (len == 16) {
				ltq_cpufreq_obj.ps_threshold_dp.th_d0 =
								(int)*(addr+0);
				ltq_cpufreq_obj.ps_threshold_dp.th_d1 =
								(int)*(addr+1);
				ltq_cpufreq_obj.ps_threshold_dp.th_d2 =
								(int)*(addr+2);
				ltq_cpufreq_obj.ps_threshold_dp.th_d3 =
								(int)*(addr+3);
			} else {
				pr_err("DTerr: threshold_dp needs 4 values\n");
			}
		}
		addr = of_get_property(nc, "lantiq,poll_period", &len);
		if (addr != NULL)
			ltq_cpufreq_obj.polling_period = *((int *)addr);
	}

	return;
}

static void ltq_set_default_scaling_rates(void)
{
	struct clk *clk = clk_get_sys("cpu", "cpu");
	int cpu_clk = clk_get_rate(clk);

	if (clk == NULL) {
		pr_err("CPU clock structure not found\n");
		return;
	}
	ltq_cpufreq_obj.cpu_scaling_rates[0] = cpu_clk;
	ltq_cpufreq_obj.cpu_scaling_rates[1] = cpu_clk;
	ltq_cpufreq_obj.cpu_scaling_rates[2] = cpu_clk;
	ltq_cpufreq_obj.cpu_scaling_rates[3] = cpu_clk;
}

static int __init ltq_cpufreq_modinit(void)
{
	int i, ret = 0;
	unsigned int freq;
	struct cpufreq_policy *policy = NULL;
#if CONFIG_LTQ_CPUFREQ_DVS
	unsigned long vol;
	unsigned int chip_type;
	unsigned int chip_id = ltq_get_cpu_id();
	unsigned int chip_rev = ltq_get_soc_rev();
	struct clk *clk = clk_get_sys("cpu", "cpu");
#endif

	pr_debug("LTQ_CPUFREQ_Version: 0x%x\n", LTQ_CPUFREQ_VERSION_CODE);
	spin_lock_init(&ltq_cpufreq_obj.ltq_cpufreq_lock);
	ltq_set_default_scaling_rates();
	of_get_cpuddr_scaling_rates();
	for (i = 0; i < 4; i++) {
		freq = ltq_cpufreq_obj.cpu_scaling_rates[i];
		if (freq == 0)
			ltq_freq_tab[i].frequency = CPUFREQ_ENTRY_INVALID;
		else
			ltq_freq_tab[i].frequency = freq / 1000;
	}

#if CONFIG_LTQ_CPUFREQ_DVS
	ltq_cpufreq_obj.regulator = regulator_get(NULL, "1V15VDD");
	if (IS_ERR(ltq_cpufreq_obj.regulator)) {
		pr_err("regulator %s not found.\n", "1V15VDD");
		ltq_cpufreq_obj.regulator = NULL;
		return -EINVAL;
	}
	drvdata.regulator = ltq_cpufreq_obj.regulator;
	drvdata.reg_idx = 0;

	vol = ltq_grx500_cpu_vol(NULL);
	if (vol == 0) {
		pr_err("core voltage returned is 0V\n");
		return -EINVAL;
	}

	ret = regulator_set_voltage(ltq_cpufreq_obj.regulator, vol, vol);
	if (ret < 0) {
		pr_err("regulator_set_voltage failed\n");
		return ret;
	}

	ret = regulator_enable(ltq_cpufreq_obj.regulator);
	if (ret < 0) {
		pr_err("regulator CPUFreq enable failed\n");
		return ret;
	}

	if (chip_rev == 1) { /*V1.1*/
		chip_type = GRX350;
	} else {
		switch (chip_id) {
		case 0x20:
		case 0x26:
		       chip_type = GRX350;
		       break;
		case 0x24:
		case 0x25:
		       chip_type = GRX550;
		       break;
		default:
		       chip_type = GRX350;
		       break;
		}
	}
	if (chip_type == GRX550) {
		/*enable I2C control*/
		pr_info("GRX550 detected. cpu_freq = %d\n",
						ltq_cpufreq_getfreq_khz(0));
		ret = regulator_enable_ext_ctrl(ltq_cpufreq_obj.regulator);
		if (ret < 0) {
			pr_err("regulator CPUFreq enable I2C Control failed\n");
			return ret;
		}

		ltq_cpufreq_obj.cpufreq_cur_state  = LTQ_CPUFREQ_PS_D1;
		ltq_cpufreq_obj.cpufreq_new_state  = LTQ_CPUFREQ_PS_D0;
		ret = clk_set_rate(clk, ltq_cpufreq_obj.cpu_scaling_rates[0]);
		if (ret < 0) {
			pr_err("clk_set_rate failed for %lu\n",
					ltq_cpufreq_obj.cpu_scaling_rates[0]);
		}
		ltq_cpufreq_obj.cpufreq_cur_state  = LTQ_CPUFREQ_PS_D0;
		pr_info("new cpu_freq for GRX550 = %d\n",
						ltq_cpufreq_getfreq_khz(0));
	}

	rcf_debugfs_init(&drvdata);
#endif

	/* register driver to the linux cpufreq driver */
	ret = cpufreq_register_driver(&ltq_cpufreq_driver);
	if (ret < 0) {
		pr_err("Error in %s, cpufreq_register_driver\n", __func__);
		return ret;
	}
	/* define the usage of mips_wait instruction inside linux idle loop */
	cpu_wait_sav = cpu_wait; /* save cpu_wait pointer */
	ltq_cpufreq_mod_list(&cpuddr_feature_fss.list, LTQ_CPUFREQ_LIST_ADD);
	ltq_cpufreq_mod_list(&cpuwait_feature_cgs.list, LTQ_CPUFREQ_LIST_ADD);

	policy = cpufreq_cpu_get(smp_processor_id());
	if (policy) {
		ret = ltq_cpufreq_stats_create(policy);
		if (ret < 0) {
			pr_err("Error in %s,ltq_cpufreq_stats_create_table\n",
				   __func__);
			return ret;
		}
	}
	return ret;
}
/*needs to be fs_initcall and not subsys_initcall because we need to have
  the clocksource initialized first because it is used in ltq_setclk_hz().
  clocksource is initialized also during fs_initcall but before ltq_cpufreq.*/
fs_initcall(ltq_cpufreq_modinit);

