/******************************************************************************
 *
 *                        Copyright (c) 2012, 2014, 2015
 *                           Lantiq Deutschland GmbH
 *
 *  For licensing information, see the file 'LICENSE' in the root folder of
 *  this software module.
 *
 ******************************************************************************/
#define DEBUG
#include <linux/ctype.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/seq_file.h>
#include <linux/printk.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <net/datapath_api.h>
#include <net/ppa_stack_al.h>

#include <lantiq_atm.h>
#include <lantiq_pcie.h>

#include "include/vrx318_common.h"

static struct ltq_mei_atm_showtime_info g_mei_showtime;

void init_dsl_callback(void)
{
	memset(&g_mei_showtime, 0, sizeof(g_mei_showtime));
}
EXPORT_SYMBOL(init_dsl_callback);

void *ppa_callback_get(int type)
{
	switch (type) {
	case LTQ_MEI_SHOWTIME_CHECK:
		return g_mei_showtime.check_ptr;
	case LTQ_MEI_SHOWTIME_ENTER:
		return g_mei_showtime.enter_ptr;
	case LTQ_MEI_SHOWTIME_EXIT:
		return g_mei_showtime.exit_ptr;
	case LTQ_MEI_TC_REQUEST:
		return g_mei_showtime.req_tc_ptr;
	case LTQ_MEI_TC_RESET:
		return g_mei_showtime.tc_reset_ptr;
	default:
		pr_err("mei unknown function type");
		return NULL;
	}
}
EXPORT_SYMBOL(ppa_callback_get);

int ppa_callback_set(int type, void *func)
{
	int ret = 0;
	switch (type) {
	/* save func address within global struct */
	case LTQ_MEI_SHOWTIME_CHECK:
		g_mei_showtime.check_ptr = func;
		break;
	case LTQ_MEI_SHOWTIME_ENTER:
		g_mei_showtime.enter_ptr = func;
		break;
	case LTQ_MEI_SHOWTIME_EXIT:
		g_mei_showtime.exit_ptr = func;
		break;
	case LTQ_MEI_TC_REQUEST:
		g_mei_showtime.req_tc_ptr = func;
		break;
	case LTQ_MEI_TC_RESET:
		g_mei_showtime.tc_reset_ptr = func;
		break;
	default:
		pr_err("mei unknown function type");
		ret = -1;
		break;
	}

	return ret;
}
EXPORT_SYMBOL(ppa_callback_set);

int get_num_from_str(char **p, int *len, int is_hex)
{
	int ret = 0;
	int n = 0;

	if ((*p)[0] == '0' && (*p)[1] == 'x') {
		is_hex = 1;
		(*p) += 2;
		(*len) -= 2;
	}

	if (is_hex) {
		while (*len && ((**p >= '0' && **p <= '9')
			|| (**p >= 'a' && **p <= 'f')
			|| (**p >= 'A' && **p <= 'F'))) {

			if (**p >= '0' && **p <= '9')
				n = **p - '0';
			else if (**p >= 'a' && **p <= 'f')
				n = **p - 'a' + 10;
			else if (**p >= 'A' && **p <= 'F')
				n = **p - 'A' + 10;
			ret = (ret << 4) | n;
			(*p)++;
			(*len)--;
		}
	} else {
		while (*len && **p >= '0' && **p <= '9') {
			n = **p - '0';
			ret = ret * 10 + n;
			(*p)++;
			(*len)--;
		}
	}

	return ret;
}
EXPORT_SYMBOL(get_num_from_str);

void ignore_space(char **p, int *len)
{
	while (*len && (**p <= ' ' || **p == ':' || **p == '.' || **p == ',')) {
		(*p)++;
		(*len)--;
	}
}
EXPORT_SYMBOL(ignore_space);


/**
* Note: Soc have to initialize the descriptor and allocate the buffer if needed
* This API should be moved to SoC part
*/
int soc_vrx318_cfg_init(struct vrx318_soc_cfg *cfg)
{
	cfg->soc_cpu_us_phybase	= 0;
	cfg->soc_cpu_us_dnum	= 0;
	/* TODO: Should get the address from CBM driver */
	/*CBM_DQM_LDESC0_0_DMA_EGP_23*/
	cfg->soc_fp_us_phybase	= 0x1E7E7100u;
	cfg->soc_fp_us_dnum	= 32;
	/* TODO: Should get the address from CBM driver */
	/* CBM_EQM_SDESC0_0_IGP_15 */
	cfg->soc_ds_phybase	= 0x1E79F100u;
	cfg->soc_ds_dbase	= 0xBE79F100u;
	cfg->soc_ds_dnum	= 32;
	cfg->pmac_en		= 0; /* Disable PMAC */
	cfg->soc_us_desc_own	= 1;
	cfg->soc_ds_desc_own	= 0;

	return 0;
}
EXPORT_SYMBOL(soc_vrx318_cfg_init);

void soc_vrx318_cfg_reset(void)
{
	void __iomem *addr;
	u32 val;

	addr = (void __iomem *)0xBE7E7030;
	val = ltq_r32(addr) & 0x1F;
	ltq_w32(val, addr);

	addr = (void __iomem *)0xBE79F030;
	val = ltq_r32(addr) & 0x71F;
	ltq_w32(val, addr);
}
EXPORT_SYMBOL(soc_vrx318_cfg_reset);

