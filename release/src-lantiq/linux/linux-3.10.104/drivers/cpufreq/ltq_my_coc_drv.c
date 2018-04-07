/*
 *
 *			 Copyright (c) 2012, 2014, 2015
 *       		Lantiq Beteiligungs-GmbH & Co. KG
 *
 *  For licensing information, see the file 'LICENSE' in the root folder of
 *  this software module.
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/timer.h>
#include <linux/cpufreq.h>
#include <cpufreq/ltq_cpufreq.h>

#define LTQ_MYCOCDRV_BUSY	1
#define LTQ_MYCOCDRV_IDLE	0

/* this ID should represent the PCIE interface No. (0, 1, 2, 3, ...) */
#define LTQ_PCIE_ID		0


/*
static struct timer_list mycocdrv_timer;
static int polling_period;              
                                        
*/

/* threshold data for D0:D3 */
struct ltq_cpufreq_threshold *th_data = NULL;

/* driver is busy and needs highest performance */
int mycocdrv_busy = LTQ_MYCOCDRV_IDLE;
/* current power state of the driver */
enum ltq_cpufreq_state mycocdrv_ps = LTQ_CPUFREQ_PS_D0;


/*
static void                                                                                 
ltq_mycocdrv_timer_callback( unsigned long data )                                           
{                                                                                           
        int ret;                                                                            
                                                                                            
        polling_period = ltq_cpufreq_get_poll_period(LTQ_CPUFREQ_MODULE_PCIE, LTQ_PCIE_ID); 
        ret = mod_timer( &mycocdrv_timer, jiffies + msecs_to_jiffies(polling_period*1000) );
        if (ret)                                                                            
                pr_err("Error in mod_timer\n");                                             
        pr_info( "mycocdrv_timer_callback called.\n");                                      
}                                                                                           
                                                                                            
*/

static int
ltq_mycocdrv_stateget(enum ltq_cpufreq_state *state)
{
	/*pr_info("%s is called\n", __func__);*/
	*state = mycocdrv_ps;
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
ltq_mycocdrv_fss_ena(int ena)
{
	/*pr_info("%s is called\n", __func__);*/
	if (ena) {
		pr_debug("enable frequency scaling\n");
	} else {
		pr_debug("disable frequency scaling\n");
	}
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
ltq_mycocdrv_prechange(enum ltq_cpufreq_module module,
			enum ltq_cpufreq_state new_state,
			enum ltq_cpufreq_state old_state,
		        u8 flags)
{
	/*pr_info("%s is called\n", __func__);*/
	if (mycocdrv_busy == LTQ_MYCOCDRV_IDLE) {
		/* do what ever is necessary to prepare the drv
		for a frequency change. */
		return LTQ_CPUFREQ_RETURN_SUCCESS;
	} else if (flags & CPUFREQ_PM_NO_DENY) {
		/* this flag is set if we need thermal reduction and
		   frequency down scaling is a must to avoid thermal problems.*/
		return LTQ_CPUFREQ_RETURN_SUCCESS;
	} else {
		if (new_state != LTQ_CPUFREQ_PS_D0)
			return LTQ_CPUFREQ_RETURN_DENIED; /*avoid down scaling*/
	}
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
ltq_mycocdrv_statechange(enum ltq_cpufreq_state new_state)
{
	/*pr_info("%s is called\n",__func__);*/
	/* do what ever is necessary to make changes in the drv
	to support the new frequency. */
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
ltq_mycocdrv_postchange(enum ltq_cpufreq_module module,
			enum ltq_cpufreq_state new_state,
			enum ltq_cpufreq_state old_state,
			u8 flags)
{
	/*pr_info("%s is called\n", __func__);*/
	/* do what ever is necessary to cleanup things in the drv
	after a frequency change. */
	mycocdrv_ps = new_state;
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

/* This function should be called if the driver becomes BUSY */
void
ltq_mycocdrv_busy(void)
{
	int ret = LTQ_CPUFREQ_RETURN_SUCCESS;

	mycocdrv_busy = LTQ_MYCOCDRV_BUSY;
	ret = ltq_cpufreq_state_req(LTQ_CPUFREQ_MODULE_PCIE, LTQ_PCIE_ID,
							LTQ_CPUFREQ_PS_D0);
	if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
		pr_debug("Power state request D0 failed");
}

/* This function should be called if the driver becomes IDLE */
void
ltq_mycocdrv_idle(void)
{
	int ret = LTQ_CPUFREQ_RETURN_SUCCESS;

	mycocdrv_busy = LTQ_MYCOCDRV_IDLE;
	ret = ltq_cpufreq_state_req(LTQ_CPUFREQ_MODULE_PCIE, LTQ_PCIE_ID,
							LTQ_CPUFREQ_PS_D0D3);
	if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
		pr_err("Power state request D0D3 failed");
}

/* keep track of frequency transitions */
static int
mycocdrv_cpufreq_notifier(struct notifier_block *nb, unsigned long val,
							void *data)
{
	struct cpufreq_freqs *freq = data;
	enum ltq_cpufreq_state new_state, old_state;
	int ret;
	pr_debug("%s is called\n", __func__);

	new_state = ltq_cpufreq_get_ps_from_khz(freq->new);
	if (new_state == LTQ_CPUFREQ_PS_UNDEF)
		return NOTIFY_STOP_MASK | (LTQ_CPUFREQ_MODULE_PCIE<<4);
	old_state = ltq_cpufreq_get_ps_from_khz(freq->old);
	if (old_state == LTQ_CPUFREQ_PS_UNDEF)
		return NOTIFY_STOP_MASK | (LTQ_CPUFREQ_MODULE_PCIE<<4);
	if (val == CPUFREQ_PRECHANGE) {
		ret = ltq_mycocdrv_prechange(LTQ_CPUFREQ_MODULE_PCIE,
					new_state, old_state, freq->flags);
		if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
			return NOTIFY_STOP_MASK | (LTQ_CPUFREQ_MODULE_PCIE<<4);
		ret = ltq_mycocdrv_statechange(new_state);
		if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
			return NOTIFY_STOP_MASK | (LTQ_CPUFREQ_MODULE_PCIE<<4);
	} else if (val == CPUFREQ_POSTCHANGE) {
		ret = ltq_mycocdrv_postchange(LTQ_CPUFREQ_MODULE_PCIE,
					new_state, old_state, freq->flags);
		if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
			return NOTIFY_STOP_MASK | (LTQ_CPUFREQ_MODULE_PCIE<<4);
	} else {
		return NOTIFY_OK | (LTQ_CPUFREQ_MODULE_PCIE<<4);
	}
	return NOTIFY_OK | (LTQ_CPUFREQ_MODULE_PCIE<<4);
}

static struct notifier_block mycocdrv_cpufreq_notifier_block = {
	.notifier_call = mycocdrv_cpufreq_notifier
};

static struct ltq_cpufreq_module_info mycocdrv_feature_fss = {
.name                           = "PCIe frequency scaling support",
.pmcuModule                     = LTQ_CPUFREQ_MODULE_PCIE,
.pmcuModuleNr                   = LTQ_PCIE_ID,
.powerFeatureStat               = 1,
.ltq_cpufreq_state_get          = ltq_mycocdrv_stateget,
.ltq_cpufreq_pwr_feature_switch = ltq_mycocdrv_fss_ena,
};

static int
ltq_mycocdrv_cpufreq_init(void)
{
        int ret;
                
	pr_debug("%s is called\n", __func__);
	if (cpufreq_register_notifier(&mycocdrv_cpufreq_notifier_block,
						CPUFREQ_TRANSITION_NOTIFIER)) {
		pr_err("Fail in registering MYCOCDRV to CPUFreq\n");
		return -1;
	}
	ltq_cpufreq_mod_list(&mycocdrv_feature_fss.list, LTQ_CPUFREQ_LIST_ADD);

	th_data = ltq_cpufreq_get_threshold(LTQ_CPUFREQ_MODULE_PCIE,
							LTQ_PCIE_ID);
	if (th_data == NULL)
		pr_err("No PS related threshold values are defined ");


/*
        polling_period = ltq_cpufreq_get_poll_period(LTQ_CPUFREQ_MODULE_PCIE, LTQ_PCIE_ID); 
        setup_timer( &mycocdrv_timer, ltq_mycocdrv_timer_callback, 0 );                     
        pr_debug( "Starting timer to fire in 3000ms (%ld)\n", jiffies );                    
        ret = mod_timer( &mycocdrv_timer, jiffies + msecs_to_jiffies(polling_period*1000) );
        if (ret)                                                                            
                pr_err("Error in mod_timer\n");                                             
                                                                                            
*/

	ret = ltq_cpufreq_state_req(LTQ_CPUFREQ_MODULE_PCIE, LTQ_PCIE_ID,
							LTQ_CPUFREQ_PS_D0);
	if (ret != LTQ_CPUFREQ_RETURN_SUCCESS)
		pr_debug("Power state request D0 failed");

	pr_debug("Register MYCOCDRV to CPUFREQ.\n");
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
ltq_mycocdrv_cpufreq_exit(void)
{

/*
        int ret;                                         
                                                         
        ret = del_timer( &mycocdrv_timer );              
        if (ret)                                         
                pr_err("The timer is still in use...\n");
                                                         
*/

	/* set status of mycocdrv inside CPUFreq to don't care */
	ltq_mycocdrv_idle();

	if (cpufreq_unregister_notifier(&mycocdrv_cpufreq_notifier_block,
						CPUFREQ_TRANSITION_NOTIFIER)) {
		pr_err("CPUFREQ unregistration failed.");
		return -1;
	}
	ltq_cpufreq_mod_list(&mycocdrv_feature_fss.list, LTQ_CPUFREQ_LIST_DEL);
	return LTQ_CPUFREQ_RETURN_SUCCESS;
}

static int
__init ltq_mycocdrv_init(void)
{
	ltq_mycocdrv_cpufreq_init();
	return 0;
}

static void
__exit ltq_mycocdrv_exit(void)
{
	ltq_mycocdrv_cpufreq_exit();
	return;
}

module_init(ltq_mycocdrv_init);
module_exit(ltq_mycocdrv_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Thomas Bartholomae, t.bartholomae@lantiq.com");
MODULE_DESCRIPTION("LANTIQ MYCOCDRV driver");
MODULE_SUPPORTED_DEVICE("LANTIQ in general ;-)");

