/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2010 John Crispin <blogic@openwrt.org>
 *  Copyright (C) 2013 Lei Chuanhua <Chuanhua.lei@lantiq.com>
 *  Copyright (C) 2014 Thomas Bartholomä <t.bartholomae@lantiq.com>
 */
#ifdef CONFIG_LTQ_CPUFREQ_DEBUG
	#define DEBUG
#endif

#define pr_fmt(fmt) KBUILD_MODNAME ": " "%s: " ": " fmt,__func__

/* used to get access to mips_hpt_frequency */
#include <asm/time.h>
#include <linux/time.h>
#include <lantiq_soc.h>
#include <cpufreq/ltq_cpufreq.h>
#include "../clk.h"

#define BARRIER __asm__ __volatile__ (".set noreorder\n\t" \
			"nop; nop; nop; nop; nop; nop; nop; nop; nop;\n\t" \
			"nop; nop; nop; nop; nop; nop; nop; nop; nop;\n\t" \
			".set reorder\n\t")
/* MPS SRAM Base Address */
#define MBX_BASEADDR		0xBF200000
#define MPS_MEM_SEG_DATASIZE	512

/* local platform identifier */
#define CLK_VR9			0x0
#define CLK_AR10		0x1
#define CLK_GRX390		0x2

/* frequency transition identifier */
/* frequency transition prohibited */
#define LTQ_NO_TRANS		0x000
/* frequency transition permitted  */
#define LTQ_TRANS_PERMIT	0x001
/* frequency transition critical, need intermediate step */
#define LTQ_TRANS_CRITICAL	0x002
/* no state change */
#define LTQ_STATE_NC		0x100
/* state change up, means step from lower to higher frequency */
#define LTQ_STATE_UP		0x200
/* state change down, means step from higher to lower frequency */
#define LTQ_STATE_DOWN		0x300


static unsigned int ram_clocks[] = {
	CLOCK_167M, CLOCK_133M, CLOCK_111M, CLOCK_83M};
#define DDR_HZ ram_clocks[ltq_cgu_r32(CGU_SYS) & 0x3]

#ifdef CONFIG_LTQ_CPU_FREQ
static int ltq_setclk_hz(unsigned long cpu_freq, unsigned long ddr_freq,
						 unsigned int sel, struct clk_rates *avail_rates);


struct ltq_freq_transition {
	enum ltq_cpufreq_state  old_state; /* oldState */
	enum ltq_cpufreq_state  new_state; /* newState */
	int         permit;	/* permission for this transition */
	enum ltq_cpufreq_state  int_state1;	/* intermediate State1 */
	enum ltq_cpufreq_state  int_state2;	/* intermediate State2 */
	int         state_dir; /* state change direction up,down */
};

static struct clk_rates avail_rates_xrx200[] = {
	/*     cpu        ddr    cgu_sys*/
	{600000000, 300000000, 0x02},
	{500000000, 250000000, 0x13},
	{500000000, 200000000, 0x12},
	{393215332, 196607666, 0x22},
	{333333333, 166666666, 0x32},
	{125000000, 125000000, 0x80},
	{0, 0, 0},
};

/*
 * Frequency transition table for XRX288
 * Intermediate states are only relevant if permit = LTQ_TRANS_CRITICAL
 */
static struct ltq_freq_transition freq_transition_xrx288[] = {
/*            oldState     ,     newState     ,     permit        ,
	  intermediate state1, intermediate stat2     UP/DOWN    */
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_NC},
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D1, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D2, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D3, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D0, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_NC},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D2, LTQ_TRANS_CRITICAL,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D3, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D0, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D1, LTQ_TRANS_CRITICAL,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_STATE_NC},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D3, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D0, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D1, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D1, LTQ_TRANS_CRITICAL,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D2, LTQ_TRANS_CRITICAL,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_STATE_NC}
};

static struct clk_rates avail_rates_xrx300[] = {
	/*     cpu        ddr    cgu_sys*/
	{600000000, 300000000, 0x101},
	{500000000, 250000000, 0x001},
	{300000000, 300000000, 0x111},
	{300000000, 150000000, 0x112},
	{250000000, 250000000, 0x011},
	{250000000, 125000000, 0x012},
	{150000000, 150000000, 0x122},
	{125000000, 125000000, 0x022},
	{0, 0, 0},
};

/*
 * Frequency transition table for XRX300
 * Intermediate states are only relevant if permit = LTQ_TRANS_CRITICAL
 */
static struct ltq_freq_transition freq_transition_xrx300[] = {
/*            oldState     ,     newState     ,     permit        ,
 intermediate state1, intermediate stat2     UP/DOWN    */
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_NC},
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D1, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D2, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D3, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D0, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_NC},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D2, LTQ_TRANS_CRITICAL,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D3, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D0, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D1, LTQ_TRANS_CRITICAL,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_STATE_NC},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D3, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D0, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D1, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D1, LTQ_TRANS_CRITICAL,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D2, LTQ_TRANS_CRITICAL,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_STATE_NC}
};

static struct clk_rates avail_rates_xrx390[] = {
	/*     cpu        ddr    cgu_sys*/
	{600000000, 300000000, 0x001},
	{300000000, 300000000, 0x011},
	{300000000, 150000000, 0x012},
	{150000000, 150000000, 0x022},
	{666666666, 333333333, 0x201},
	{333333333, 333333333, 0x211},
	{333333333, 166666666, 0x212},
	{166666666, 166666666, 0x222},
	{720000000, 360000000, 0x401},
	{360000000, 360000000, 0x411},
	{360000000, 180000000, 0x412},
	{180000000, 180000000, 0x422},
	{0, 0, 0},
};

#endif

/* legacy xway clock */
#define CGU_SYS			0x10

/* vr9, ar10/grx390 register offsets*/
#define CGU_SYS_XRX		0x0c
#define CGU_UPDATE_XRX		0x20
#define CGU_IF_CLK_AR10		0x24

/* DDR Combo Controller Base Address */
#define DDR_COMBO_BASE		0xBF801000
#define DDRCC_R32(reg) (*(volatile u32 *)(DDR_COMBO_BASE+reg))
#define DDRCC_W32(reg, val) (*(volatile u32 *)(DDR_COMBO_BASE+reg) = val)
#define DDR_CCR00_XRX		0x0000
#define DDR_CCR15_XRX		0x00F0
#define DDR_CCR28_XRX		0x01C0
#define DDR_PHYR11_XRX		0x04B0
/* CGU Base Address */
#define CGU_BASE		0xBF103000
#define CGU_R32(reg) (*(volatile u32 *)(CGU_BASE+reg))
#define CGU_W32(reg, val) (*(volatile u32 *)(CGU_BASE+reg) = val)

DEFINE_SPINLOCK(ltq_setclk_lock);
DECLARE_PER_CPU(struct clock_event_device, mips_clockevent_device);

static inline void ddrcc_w32(int reg, int val)
{
	*(volatile u32 *)(DDR_COMBO_BASE+reg) = val;
}


unsigned long ltq_ase_cpu_hz(void)
{
	if (ltq_cgu_r32(CGU_SYS) & (1 << 5))
		return CLOCK_266M;
	else
		return CLOCK_133M;
}

unsigned long ltq_ase_fpi_hz(void)
{
	if (ltq_cgu_r32(CGU_SYS) & (1 << 5))
		return CLOCK_133M;
	else
		return CLOCK_133M;
}

unsigned long ltq_ase_pp32_hz(void)
{
	if (ltq_cgu_r32(CGU_SYS) & (1 << 5))
		return CLOCK_266M;
	else
		return CLOCK_133M;
}

unsigned long ltq_danube_fpi_hz(void)
{
	unsigned long ddr_clock = DDR_HZ;

	if (ltq_cgu_r32(CGU_SYS) & 0x40)
		return ddr_clock >> 1;
	return ddr_clock;
}

unsigned long ltq_danube_cpu_hz(void)
{
	switch (ltq_cgu_r32(CGU_SYS) & 0xc) {
	case 0:
		return CLOCK_333M;
	case 4:
		return DDR_HZ;
	case 8:
		return DDR_HZ << 1;
	default:
		return DDR_HZ >> 1;
	}
}

unsigned long ltq_danube_pp32_hz(void)
{
	unsigned int clksys = (ltq_cgu_r32(CGU_SYS) >> 7) & 3;
	unsigned long clk;

	switch (clksys) {
	case 1:
		clk = CLOCK_240M;
		break;
	case 2:
		clk = CLOCK_222M;
		break;
	case 3:
		clk = CLOCK_133M;
		break;
	default:
		clk = CLOCK_266M;
		break;
	}
	return clk;
}

unsigned long ltq_ar9_sys_hz(void)
{
	if (((ltq_cgu_r32(CGU_SYS) >> 3) & 0x3) == 0x2)
		return CLOCK_393M;
	return CLOCK_333M;
}

unsigned long ltq_ar9_fpi_hz(void)
{
	unsigned long sys = ltq_ar9_sys_hz();

	if (ltq_cgu_r32(CGU_SYS) & BIT(0))
		return sys;
	return sys >> 1;
}

unsigned long ltq_ar9_cpu_hz(void)
{
	if (ltq_cgu_r32(CGU_SYS) & BIT(2))
		return ltq_ar9_fpi_hz();
	else
		return ltq_ar9_sys_hz();
}

unsigned long ltq_ar9_pp32_hz(void)
{
	return CLOCK_250M;
}

unsigned long ltq_vr9_cpu_hz(void)
{
	unsigned int cpu_sel;
	unsigned long clk;

	cpu_sel = (ltq_cgu_r32(CGU_SYS_XRX) >> 4) & 0xf;

	switch (cpu_sel) {
	case 0:
		clk = CLOCK_600M;
		break;
	case 1:
		clk = CLOCK_500M;
		break;
	case 2:
		clk = CLOCK_393M;
		break;
	case 3:
		clk = CLOCK_333M;
		break;
	case 5:
	case 6:
		clk = CLOCK_196_608M;
		break;
	case 7:
		clk = CLOCK_167M;
		break;
	case 4:
	case 8:
	case 9:
		clk = CLOCK_125M;
		break;
	default:
		clk = 0;
		break;
	}

	return clk;
}

unsigned long ltq_vr9_fpi_hz(void)
{
	unsigned int ocp_sel, cpu_clk;
	unsigned long clk;

	cpu_clk = ltq_vr9_cpu_hz();
	ocp_sel = ltq_cgu_r32(CGU_SYS_XRX) & 0x3;

	switch (ocp_sel) {
	case 0:
		/* OCP ratio 1 */
		clk = cpu_clk;
		break;
	case 2:
		/* OCP ratio 2 */
		clk = cpu_clk / 2;
		break;
	case 3:
		/* OCP ratio 2.5 */
		clk = (cpu_clk * 2) / 5;
		break;
	case 4:
		/* OCP ratio 3 */
		clk = cpu_clk / 3;
		break;
	default:
		clk = 0;
		break;
	}

	return clk;
}

unsigned long ltq_vr9_pp32_hz(void)
{
	unsigned int clksys = (ltq_cgu_r32(CGU_SYS) >> 16) & 0x7;
	unsigned long clk;

	switch (clksys) {
	case 0:
		clk = CLOCK_500M;
		break;
	case 1:
		clk = CLOCK_432M;
		break;
	case 2:
		clk = CLOCK_288M;
		break;
	default:
		clk = CLOCK_500M;
		break;
	}
	return clk;
}

unsigned long ltq_ar10_cpu_hz(void)
{
	unsigned int clksys;
	int cpu_fs = (ltq_cgu_r32(CGU_SYS_XRX) >> 8) & 0x1;
	int freq_div = (ltq_cgu_r32(CGU_SYS_XRX) >> 4) & 0x7;

	switch (cpu_fs) {
	case 0:
		clksys = CLOCK_500M;
		break;
	case 1:
		clksys = CLOCK_600M;
		break;
	default:
		clksys = CLOCK_500M;
		break;
	}

	switch (freq_div) {
	case 0:
		return clksys;
	case 1:
		return clksys >> 1;
	case 2:
		return clksys >> 2;
	default:
		return clksys;
	}
}

unsigned long ltq_ar10_fpi_hz(void)
{
	int freq_fpi = (ltq_cgu_r32(CGU_IF_CLK_AR10) >> 25) & 0xf;

	switch (freq_fpi) {
	case 1:
		return CLOCK_300M;
	case 5:
		return CLOCK_250M;
	case 2:
		return CLOCK_150M;
	case 6:
		return CLOCK_125M;

	default:
		return CLOCK_125M;
	}
}

unsigned long ltq_ar10_pp32_hz(void)
{
	unsigned int clksys = (ltq_cgu_r32(CGU_SYS) >> 16) & 0x7;
	unsigned long clk;

	switch (clksys) {
	case 1:
		clk = CLOCK_250M;
		break;
	case 4:
		clk = CLOCK_400M;
		break;
	default:
		clk = CLOCK_250M;
		break;
	}

	return clk;
}

unsigned long ltq_grx390_cpu_hz(void)
{
	unsigned int clksys;
	int cpu_fs = ((ltq_cgu_r32(CGU_SYS_XRX) >> 9) & 0x3);
	int freq_div = ((ltq_cgu_r32(CGU_SYS_XRX) >> 4) & 0x7);

	switch (cpu_fs) {
	case 0:
		clksys = CLOCK_600M;
		break;
	case 1:
		clksys = CLOCK_666M;
		break;
	case 2:
		clksys = CLOCK_720M;
		break;
	default:
		clksys = CLOCK_600M;
		break;
	}

	switch (freq_div) {
	case 0:
		return clksys;
	case 1:
		return clksys >> 1;
	case 2:
		return clksys >> 2;
	default:
		return clksys;
	}
}

unsigned long ltq_grx390_fpi_hz(void)
{
	/* fpi clock is derived from ddr_clk */
	unsigned int clksys;
	int cpu_fs = ((ltq_cgu_r32(CGU_SYS_XRX) >> 9) & 0x3);
	int freq_div = ((ltq_cgu_r32(CGU_SYS_XRX)) & 0x7);
	switch (cpu_fs) {
	case 0:
		clksys = CLOCK_600M;
		break;
	case 1:
		clksys = CLOCK_666M;
		break;
	case 2:
		clksys = CLOCK_720M;
		break;
	default:
		clksys = CLOCK_600M;
		break;
	}

	switch (freq_div) {
	case 1:
		return clksys >> 1;
	case 2:
		return clksys >> 2;
	default:
		return clksys >> 1;
	}
}

unsigned long ltq_grx390_pp32_hz(void)
{
	unsigned int clksys = (ltq_cgu_r32(CGU_SYS) >> 16) & 0x7;
	unsigned long clk;

	switch (clksys) {
	case 1:
		clk = CLOCK_250M;
		break;
	case 2:
		clk = CLOCK_432M;
		break;
	case 4:
		clk = CLOCK_400M;
		break;
	default:
		clk = CLOCK_250M;
		break;
	}
	return clk;
}

int ltq_ase_set_cpu_hz(unsigned long cpu_freq)
{
	return 0;
}
int ltq_danube_set_cpu_hz(unsigned long cpu_freq)
{
	return 0;
}
int ltq_ar9_set_cpu_hz(unsigned long cpu_freq)
{
	return 0;
}

#ifdef CONFIG_LTQ_CPU_FREQ
static int ltq_get_transition_permit(struct ltq_freq_transition *freq_tt,
									 enum ltq_cpufreq_state old_state,
									 enum ltq_cpufreq_state new_state,
									 enum ltq_cpufreq_state *int_state1,
									 enum ltq_cpufreq_state *int_state2)
{
	int i;
	struct ltq_freq_transition freq_trans;

	for (i = 0; i < 16; i++) {
		freq_trans = *(freq_tt + i);
		if (freq_trans.old_state == old_state) {
			if (freq_trans.new_state == new_state) {
				*int_state1 = freq_trans.int_state1;
				*int_state2 = freq_trans.int_state2;
				return freq_trans.permit | freq_trans.state_dir;
			}
		}
	}
	return LTQ_NO_TRANS | LTQ_STATE_NC;
}

int ltq_set_cpu_hz(unsigned int sel,
				   struct clk_rates *avail_rates,
				   struct ltq_freq_transition *freq_tt)
{
	int ret;
	struct ltq_cpufreq *clk_cpufreq_p;
	unsigned long *ddr_freq_p;
	unsigned long *cpu_freq_p;
	enum ltq_cpufreq_state intState1 = 0;
	enum ltq_cpufreq_state intState2 = 0;

	clk_cpufreq_p = ltq_cpufreq_get();
	if (clk_cpufreq_p == NULL)
		return -1;

	ddr_freq_p = clk_cpufreq_p->ddr_scaling_rates;
	if (ddr_freq_p == NULL)
		return -1;

	cpu_freq_p = clk_cpufreq_p->cpu_scaling_rates;
	if (ddr_freq_p == NULL)
		return -1;


	/*  Check if we have a critical frequency transition */
	ret = ltq_get_transition_permit(freq_tt,
									clk_cpufreq_p->cpufreq_cur_state,
									clk_cpufreq_p->cpufreq_new_state,
									&intState1, &intState2);
	if ((ret & 0xFF) == LTQ_NO_TRANS)
		return 0; /* nothing to do */

	if ((ret & 0xFF) == LTQ_TRANS_CRITICAL) {
		ddr_freq_p += intState1 - LTQ_CPUFREQ_PS_D0;
		cpu_freq_p += intState1 - LTQ_CPUFREQ_PS_D0;
		ret = ltq_setclk_hz(*cpu_freq_p, *ddr_freq_p, sel,
							avail_rates);
		if (ret < 0)
			return -1;

		if (intState1 != intState2) {
			ddr_freq_p += intState2 - LTQ_CPUFREQ_PS_D0;
			cpu_freq_p += intState2 - LTQ_CPUFREQ_PS_D0;
			ret = ltq_setclk_hz(*cpu_freq_p, *ddr_freq_p, sel,
								avail_rates);
			if (ret < 0)
				return -1;
		}
	}
	ddr_freq_p += clk_cpufreq_p->cpufreq_new_state - LTQ_CPUFREQ_PS_D0;
	cpu_freq_p += clk_cpufreq_p->cpufreq_new_state - LTQ_CPUFREQ_PS_D0;
	ret = ltq_setclk_hz(*cpu_freq_p, *ddr_freq_p, sel, avail_rates);
	return ret;
}
#endif /* CONFIG_LTQ_CPU_FREQ */

int ltq_vr9_set_cpu_hz(unsigned long cpu_freq)
{
#ifdef CONFIG_LTQ_CPU_FREQ
	return ltq_set_cpu_hz(CLK_VR9, avail_rates_xrx200,
						  freq_transition_xrx288);
#else
	return -1;
#endif /* CONFIG_LTQ_CPU_FREQ */
}

int ltq_ar10_set_cpu_hz(unsigned long cpu_freq)
{
#ifdef CONFIG_LTQ_CPU_FREQ
	return ltq_set_cpu_hz(CLK_AR10, avail_rates_xrx300,
						  freq_transition_xrx300);
#else
	return -1;
#endif /* CONFIG_LTQ_CPU_FREQ */
}

int ltq_grx390_set_cpu_hz(unsigned long cpu_freq)
{
#ifdef CONFIG_LTQ_CPU_FREQ
	return ltq_set_cpu_hz(CLK_AR10, avail_rates_xrx390,
						  freq_transition_xrx300);

#else
	return -1;
#endif /* CONFIG_LTQ_CPU_FREQ */
}

#ifdef CONFIG_LTQ_CPU_FREQ
extern unsigned long loops_per_jiffy;

static
inline unsigned long cpufreq_scale(unsigned long old, u_int div, u_int mult)
{
#if BITS_PER_LONG == 32
	u64 result = ((u64) old) * ((u64) mult);
	do_div(result, div);
	return(unsigned long) result;
#elif BITS_PER_LONG == 64
	unsigned long result = old * ((u64) mult);
	result /= div;
	return result;
#endif
};

/* Precondition: all CPUS running on the same frequency */
static int update_sysclock(unsigned long cpu_freq_prev, unsigned long cpu_freq,
						   struct clocksource *cs)
{
	int i, cpu;
	struct clocksource cs_old;
	struct clock_event_device *ce = NULL;

	mips_hpt_frequency = cpu_freq / 2;/*half of the CPU frequency*/
	memcpy(&cs_old, cs, sizeof(cs_old));
	__clocksource_updatefreq_scale(cs, 1, mips_hpt_frequency);
	timekeeping_clocksource_update(cs, &cs_old);
	for_each_cpu(cpu, cpu_possible_mask) {
		ce = &per_cpu(mips_clockevent_device, cpu);
		if (ce == NULL) {
			return -EINVAL;
		}
		ce->mult = div_sc((unsigned long)mips_hpt_frequency, 
						  NSEC_PER_SEC, 32);
		ce->max_delta_ns    = clockevent_delta2ns(0x7fffffff, ce);
		ce->min_delta_ns    = clockevent_delta2ns(0x300, ce);
	}

	/* START: update udelay because of frequency change */
	/* adjust loops_per_jiffy and udelay here */
	loops_per_jiffy = cpufreq_scale(loops_per_jiffy, cpu_freq_prev,
									cpu_freq);
	/* adjust udelay for each cpu */
	for (i=0;i<NR_CPUS;i++) {
		cpu_data[i].udelay_val = loops_per_jiffy;
	}
	/* END: update udelay because of frequency change */

	return 0;
}

static int ltq_ddr_clk_magic_vr9(char *argv, unsigned int cgu_sys)
{
	void *start, *end;

	/* Cache the reset code of this function */
	__asm__ __volatile__ (
						 "       .set    push\n"
						 "       .set    mips3\n"
						 "       la      %0,startpoint_vr9\n"
						 "       la      %1,endpoint_vr9\n"
						 "       .set    pop\n"
						 : "=r" (start), "=r" (end)
						 :
						 );
	memcpy((u8 *)argv, (u8 *)MBX_BASEADDR, MPS_MEM_SEG_DATASIZE);

	/* check if end-start exceeds 512 Byte */
	if ((end-start) >= MPS_MEM_SEG_DATASIZE)
		return -EINVAL;

	memcpy((u8 *)MBX_BASEADDR, (u8 *)start, (end - start));

	/* save cgu_sys value to MPS memory for later use */
	*(volatile u32 *)MBX_BASEADDR = cgu_sys;

	/* jump to MPS memory; skip first 4 byte because of cgu_sys value */
	__asm__("jr     %0" : : "r"(MBX_BASEADDR+4));
	__asm__ __volatile__ ("startpoint_vr9:\n");
	BARRIER;
	*(volatile u32 *)0xbf401070 |= 0x1;	/* Put DDR into self Refresh mode*/
	BARRIER;
	/* wait until DDR controller acknowledge self refresh */
	while ((*(volatile u32 *)0xbf400400 & 0x2) == 0) {
		/* check CKE bit */
	}
	/* Stop the DDR controller by writing a 0 to the START parameter. */
	*(volatile u32 *)0xbf401070 &= ~(1<<8);
	BARRIER;
	*(volatile u32 *)0xbf103020 = 1;
	BARRIER;
	/*
	  this wait is necessary, otherwise system becomes instable !
	  CGU_SYS is split into a master register and a shadow register.
	  write -> master-register; read <- shadow-register.
	  Takeover from master into shadow triggered by IFX_CGU_UPDATE.
	  Therefore we have to wait until cgu_sys value is transfered into
	  shadow register.
	*/
	while (*(volatile u32 *)0xbf10300C != *(volatile u32 *)MBX_BASEADDR) {
	}
	/*
	  restart the DDR SDRAM controller by writing a 1 to the START
	  parameter. This forces the DLL to lock to the new frequency.
	*/
	*(volatile u32 *)0xbf401070 |= 0x100;
	/* check for DLL relock */
	while ((*(volatile u32 *)0xbf4012f0 & 0x1) == 0) {
	}
	/* check for DLL relock */
	while ((*(volatile u32 *)0xbf401300 & 0x1) == 0) {
	}
	/* Put DDR out of self Refresh mode */
	*(volatile u32 *)0xbf401070 &= ~(1<<0);
	__asm__("jr     %0" : : "r"(end + 16));
	__asm__ __volatile__ ("endpoint_vr9:\n");
	BARRIER;
	return 0;
}

static int ltq_ddr_clk_magic_ar10(char *argv, unsigned int cgu_sys)
{
	void *start, *end;

	/* check if DDR clk must be changed too. */
	if ((cgu_sys & 0x7) == (DDRCC_R32(CGU_SYS_XRX) & 0x7)) {
		/* no DDR clk change; set new CPU clk and trigger update */
		CGU_W32(CGU_SYS_XRX, (0x80 | cgu_sys));
		BARRIER;
		return 0;
	}
	/* Cache the reset code of this function */
	__asm__ __volatile__ (
						 "       .set    push\n"
						 "       .set    mips3\n"
						 "       la      %0,startpoint_ar10\n"
						 "       la      %1,endpoint_ar10\n"
						 "       .set    pop\n"
						 : "=r" (start), "=r" (end)
						 :
						 );
	memcpy((u8 *)argv, (u8 *)MBX_BASEADDR, MPS_MEM_SEG_DATASIZE);

	/* check if end-start exceeds 512 Byte */
	if ((end-start) >= MPS_MEM_SEG_DATASIZE)
		return -EINVAL;

	memcpy((u8 *)MBX_BASEADDR, (u8 *)start, (end - start));

	/* save cgu_sys value to MPS memory for later use */
	*(volatile u32 *)MBX_BASEADDR = cgu_sys;

	/* jump to MPS memory; skip first 4 byte because of cgu_sys value */
	__asm__("jr     %0" : : "r"(MBX_BASEADDR+4));
	__asm__ __volatile__ ("startpoint_ar10:\n");
	BARRIER;
	/* quick refresh + self refresh mode */
	DDRCC_W32(DDR_CCR15_XRX, (DDRCC_R32(DDR_CCR15_XRX) | 0x01000001));
	BARRIER;
	/* wait until DDR controller acknowledge self refresh. Check CKE bit */
	while ((DDRCC_R32(DDR_CCR28_XRX) & 0x2) == 0x2) {
	}

	/* Stop the DDR controller by writing a 0 to the START parameter. */
	DDRCC_W32(DDR_CCR00_XRX, (DDRCC_R32(DDR_CCR00_XRX) & ~0x1));
	BARRIER;
	/* set new frequency and trigger update */
	CGU_W32(CGU_SYS_XRX, (0x80 | cgu_sys));
	BARRIER;
	/*
	  Restart the DDR controller by writing a 1 to the START parameter.
	  This forces the DLL to lock to the new frequency.
	*/
	DDRCC_W32(DDR_CCR00_XRX, (DDRCC_R32(DDR_CCR00_XRX) | 0x1));

	/* check for DLL relock */
	while ((DDRCC_R32(DDR_PHYR11_XRX) & 0x1) == 0) {
	}

	/* Put DDR out of self Refresh mode */
	DDRCC_W32(DDR_CCR15_XRX, (DDRCC_R32(DDR_CCR15_XRX) & ~0x1));
	__asm__("jr     %0" : : "r"(end + 16));
	__asm__ __volatile__ ("endpoint_ar10:\n");
	BARRIER;
	return 0;
}

static int ltq_setclk_hz(unsigned long cpu_freq, unsigned long ddr_freq,
						 unsigned int sel, struct clk_rates *avail_rates)
{
	unsigned long cpu_freq_prev;
	unsigned long sys_flag;
	unsigned long dmt_flag;
	unsigned int vpe_flag, cur_tc;
	struct clk *clk;
	char   *argv;
	unsigned int cgu_sys = 0;
	int ret_magic = 0;
	int retval = 0;
	struct clocksource *cs = clocksource_get_current();

	/* 
	* First check for MIPS clock source because the following
	* implementation is only for MIPS cs.
	*/
	if (cs == NULL) {
		pr_err("current clocksource is NULL\n");
		return -EINVAL;
	}

	if (0 != strcmp(cs->name, "MIPS")) {
		pr_err("current clocksource isn't MIPS.\n");
		return -EINVAL;
	}

	clk = clk_get_sys("cpu", "cpu");
	if (IS_ERR(clk)) {
		pr_err("CPU clk not found.\n");
		return -EINVAL;
	}

	cpu_freq_prev = clk_get_rate(clk);
	while (avail_rates->cpu_freq != 0) {
		if (avail_rates->cpu_freq == cpu_freq) {
			if (avail_rates->ddr_freq == ddr_freq) {
				cgu_sys = avail_rates->cgu_sys;
				break;
			}
		}
		avail_rates++;
	}
	if (cgu_sys == 0) {
		pr_err("cgu_sys is NULL.\n");
		return -EINVAL;
	}

	cur_tc = smp_processor_id();
	spin_lock_irqsave(&ltq_setclk_lock, sys_flag);
	/* malloc MPS backup mem */
	argv = kmalloc(MPS_MEM_SEG_DATASIZE, GFP_KERNEL);
	if (argv == NULL) {
		spin_unlock_irqrestore(&ltq_setclk_lock, sys_flag);
		return -EINVAL;
	}
	/* Put MVPE's into 'configuration state' */
	set_c0_mvpcontrol(MVPCONTROL_VPC);
	dmt_flag = dmt();
	vpe_flag = dvpe();
	/* take system out of configuration state */

	*(volatile u32 *)0xbf10300C = cgu_sys;
	if (sel == CLK_VR9)
		ret_magic = ltq_ddr_clk_magic_vr9(argv, cgu_sys);
	else if (sel == CLK_AR10)
		ret_magic = ltq_ddr_clk_magic_ar10(argv, cgu_sys);

	BARRIER;
	memcpy((u8 *)MBX_BASEADDR, (u8 *)argv, MPS_MEM_SEG_DATASIZE);
	kfree(argv); /* free MPS backup mem */

	emt(dmt_flag);
	evpe(vpe_flag);
	/* take system out of configuration state */
	clear_c0_mvpcontrol(MVPCONTROL_VPC);

	/* update clock source */
	/* update clock event */
	/* adjust loops_per_jiffy for udelay */
	retval = update_sysclock(cpu_freq_prev, cpu_freq, cs);
	spin_unlock_irqrestore(&ltq_setclk_lock, sys_flag);

	if (ret_magic < 0)
		pr_err("magic code size overflow\n");

	return retval;
}
#endif
