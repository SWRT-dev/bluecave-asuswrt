/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 * Copyright (C) 2010 Thomas Langer <thomas.langer@lantiq.com>
 * Copyright (C) 2010 John Crispin <blogic@openwrt.org>
 * Copyright (C) 2014~2015 Chuanhua Lei <Chuanhua.lei@lantiq.com>
 */
#include <linux/io.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/err.h>
#include <linux/list.h>
#include <linux/regulator/consumer.h>
#include <asm/time.h>
#include <asm/div64.h>
#include <asm/setup.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <lantiq_soc.h>
#include <lantiq.h>
#include <cgu.h>
#include <ssx0_ssx.h>
#include <ssx1_ssx.h>
#include <ssx2_ssx.h>
#include <ssx6_ssx.h>
#include <ssx7_ssx.h>
#include "clk.h"
#include "prom.h"
#include <cpufreq/ltq_cpufreq.h>

#define MS(_v, _f, _p)  (((_v) & (_f)) >> _p)

/* GRX500 product variants */
#define GRX350			0x1
#define GRX550			0x2
/* local platform identifier */
#define CLK_GRX500		0x3
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

#define ltq_xbar0_w32(y, x)	ltq_w32((x), ltq_xbar0_membase + (y))
#define ltq_xbar0_r32(x)	ltq_r32(ltq_xbar0_membase + (x))
#define ltq_xbar1_w32(y, x)	ltq_w32((x), ltq_xbar1_membase + (y))
#define ltq_xbar1_r32(x)	ltq_r32(ltq_xbar1_membase + (x))
#define ltq_xbar2_w32(y, x)	ltq_w32((x), ltq_xbar2_membase + (y))
#define ltq_xbar2_r32(x)	ltq_r32(ltq_xbar2_membase + (x))
#define ltq_xbar6_w32(y, x)	ltq_w32((x), ltq_xbar6_membase + (y))
#define ltq_xbar6_r32(x)	ltq_r32(ltq_xbar6_membase + (x))
#define ltq_xbar7_w32(y, x)	ltq_w32((x), ltq_xbar7_membase + (y))
#define ltq_xbar7_r32(x)	ltq_r32(ltq_xbar7_membase + (x))

#define TMI2_STATUS		(TMI2_TA_AGENT_STATUS_REQ_WAITING_MASK |\
				TMI2_TA_AGENT_STATUS_RESP_ACTIVE_MASK |\
				TMI2_TA_AGENT_STATUS_BURST_MASK |\
				TMI2_TA_AGENT_STATUS_READEX_MASK)

#define TIC_STATUS		(TIC_TA_AGENT_STATUS_REQ_WAITING_MASK |\
				TMI2_TA_AGENT_STATUS_RESP_ACTIVE_MASK |\
				TMI2_TA_AGENT_STATUS_BURST_MASK |\
				TMI2_TA_AGENT_STATUS_READEX_MASK)

#define IL2_STATUS		(IL2_IA_AGENT_STATUS_RESP_WAITING_MASK |\
				IL2_IA_AGENT_STATUS_REQ_ACTIVE_MASK |\
				IL2_IA_AGENT_STATUS_BURST_MASK |\
				IL2_IA_AGENT_STATUS_READEX_MASK)

#define IDM1R_STATUS		(IDM1R_IA_AGENT_STATUS_RESP_WAITING_MASK |\
				IDM1R_IA_AGENT_STATUS_REQ_ACTIVE_MASK |\
				IDM1R_IA_AGENT_STATUS_BURST_MASK |\
				IDM1R_IA_AGENT_STATUS_READEX_MASK)

#define IDM1T_STATUS		(IDM1T_IA_AGENT_STATUS_RESP_WAITING_MASK |\
				IDM1T_IA_AGENT_STATUS_REQ_ACTIVE_MASK |\
				IDM1T_IA_AGENT_STATUS_BURST_MASK |\
				IDM1T_IA_AGENT_STATUS_READEX_MASK)

#define IDM2R_STATUS		(IDM2R_IA_AGENT_STATUS_RESP_WAITING_MASK |\
				IDM2R_IA_AGENT_STATUS_REQ_ACTIVE_MASK |\
				IDM2R_IA_AGENT_STATUS_BURST_MASK |\
				IDM2R_IA_AGENT_STATUS_READEX_MASK)

#define IDM2T_STATUS		(IDM2T_IA_AGENT_STATUS_RESP_WAITING_MASK |\
				IDM2T_IA_AGENT_STATUS_REQ_ACTIVE_MASK |\
				IDM2T_IA_AGENT_STATUS_BURST_MASK |\
				IDM2T_IA_AGENT_STATUS_READEX_MASK)

#define TCBM1_STATUS		(TCBM1_TA_AGENT_STATUS_REQ_WAITING_MASK |\
				TCBM1_TA_AGENT_STATUS_RESP_ACTIVE_MASK |\
				TCBM1_TA_AGENT_STATUS_BURST_MASK |\
				TCBM1_TA_AGENT_STATUS_READEX_MASK)

#define TCBM2_STATUS		(TCBM2_TA_AGENT_STATUS_REQ_WAITING_MASK |\
				TCBM2_TA_AGENT_STATUS_RESP_ACTIVE_MASK |\
				TCBM2_TA_AGENT_STATUS_BURST_MASK |\
				TCBM2_TA_AGENT_STATUS_READEX_MASK)

#define CFG_CPU_IGP0_ENABLE	(CFG_CPU_IGP_0_BUFREQS_MASK |\
				CFG_CPU_IGP_0_BUFREQJ_MASK |\
				CFG_CPU_IGP_0_RCNTBPEN_MASK)

#define CFG_CPU_IGP1_ENABLE	(CFG_CPU_IGP_1_BUFREQS_MASK |\
				CFG_CPU_IGP_1_BUFREQJ_MASK |\
				CFG_CPU_IGP_1_RCNTBPEN_MASK)

#define CFG_CPU_IGP2_ENABLE	(CFG_CPU_IGP_2_BUFREQS_MASK |\
				CFG_CPU_IGP_2_BUFREQJ_MASK |\
				CFG_CPU_IGP_2_RCNTBPEN_MASK)

#define CFG_CPU_IGP3_ENABLE	(CFG_CPU_IGP_3_BUFREQS_MASK |\
				CFG_CPU_IGP_3_BUFREQJ_MASK |\
				CFG_CPU_IGP_3_RCNTBPEN_MASK)

static void __iomem *ltq_xbar0_membase;
static void __iomem *ltq_xbar1_membase;
static void __iomem *ltq_xbar2_membase;
static void __iomem *ltq_xbar6_membase;
static void __iomem *ltq_xbar7_membase;

#ifdef CONFIG_LTQ_CPU_FREQ
DEFINE_SPINLOCK(ltq_setclk_lock);

#ifdef CONFIG_CEVT_GIC
DECLARE_PER_CPU(struct clock_event_device, gic_clockevent_device);
#endif

struct ltq_freq_transition {
	enum ltq_cpufreq_state old_state; /* oldState */
	enum ltq_cpufreq_state new_state; /* newState */
	int permit; /* permission for this transition */
	enum ltq_cpufreq_state int_state1; /* intermediate State1 */
	enum ltq_cpufreq_state int_state2; /* intermediate State2 */
	int state_dir; /* state change direction up,down */
};

static int ltq_setclk_hz(unsigned long cpu_freq, unsigned long ddr_freq,
			 unsigned int sel, struct clk_rates *avail_rates);
static int ltq_set_cpu_hz(unsigned int sel,
			struct clk_rates *avail_rates,
			struct ltq_freq_transition *freq_tt);

static int ltq_speed_grade = MEDIUM0 | 0x100; /* bit8 indicates default value */

static struct clk_rates avail_rates_xrx500_a[] = {
/* cpu        ddr      cpu_clkM_sel, pll_clk1 pll_clk2 pll_clk4   microvolt(slow0  , slow1  , slow2  , slow3  ,  typ0    , typ1   , typ2   , typ3   ,  fast0   , fast1  , fast2  , fast3)*/
{1200000000, 400000000, 0x00000000, 0x0003,  0x0060,  0x1000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1150000, 1150000, 1100000, 1150000}, {1150000, 1150000, 1100000, 1150000}} },
{1000000000, 333000000, 0x20000000, 0x0003,  0x0060,  0x1000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1150000, 1150000, 1100000, 1150000}, {1150000, 1150000, 1100000, 1150000}} },
{ 800000000, 333000000, 0x00000000, 0x0003,  0x0060,  0x2000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 666666666, 333000000, 0x20000000, 0x0008,  0x0060,  0x2000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 600000000, 333000000, 0x00000000, 0x0008,  0x0060,  0x3000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 500000000, 333000000, 0x20000000, 0x0008,  0x0060,  0x3000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 480000000, 333000000, 0x00000000, 0x0008,  0x0060,  0x4000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 400000000, 333000000, 0x20000000, 0x0008,  0x0060,  0x4000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 400000000, 333000000, 0x00000000, 0x0008,  0x0060,  0x5000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 333333333, 333000000, 0x20000000, 0x0008,  0x0060,  0x5000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 300000000, 333000000, 0x00000000, 0x0008,  0x0060,  0x6000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 250000000, 333000000, 0x20000000, 0x0008,  0x0060,  0x6000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 240000000, 333000000, 0x00000000, 0x0008,  0x0060,  0x7000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 200000000, 333000000, 0x20000000, 0x0008,  0x0060,  0x7000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 200000000, 333000000, 0x00000000, 0x0008,  0x0060,  0x8000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 166666666, 333000000, 0x20000000, 0x0009,  0x0060,  0x8000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 150000000, 333000000, 0x00000000, 0x0009,  0x0060,  0x9000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 125000000, 333000000, 0x20000000, 0x0009,  0x0060,  0x9000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 120000000, 333000000, 0x00000000, 0x0009,  0x0060,  0xa000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 100000000, 333000000, 0x20000000, 0x0009,  0x0060,  0xa000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{ 100000000, 333000000, 0x00000000, 0x0009,  0x0060,  0xb000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{  83333333, 333000000, 0x20000000, 0x0009,  0x0060,  0xb000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{  75000000, 166000000, 0x00000000, 0x0009,  0x0060,  0xc000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{  62500000, 166000000, 0x20000000, 0x0009,  0x0060,  0xc000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{  60000000, 166000000, 0x00000000, 0x0009,  0x0060,  0xd000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{  50000000, 166000000, 0x20000000, 0x000E,  0x00E0,  0xd000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{  50000000, 166000000, 0x00000000, 0x000E,  0x00E0,  0xe000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{  41666666, 166000000, 0x20000000, 0x000E,  0x00E0,  0xe000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{  37500000, 166000000, 0x00000000, 0x000E,  0x00E0,  0xf000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{  31250000, 166000000, 0x20000000, 0x000E,  0x00E0,  0xf000,	/*grx350*/{{1100000, 1100000, 1100000, 1150000}, {1050000, 1100000, 1050000, 1050000}, {1000000, 1050000, 1000000, 1150000}},
								/*grx550*/{{1150000, 1150000, 1150000, 1150000}, {1050000, 1050000, 1050000, 1150000}, {1050000, 1050000, 1000000, 1150000}} },
{0, 0, 0, 0, 0, 0,	/*grx350*/{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
			/*grx550*/{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}} },
};


/*
 * Frequency transition table for XRX300
 * Intermediate states are only relevant if permit = LTQ_TRANS_CRITICAL
 */
static struct ltq_freq_transition freq_transition_xrx500[] = {
/*		oldState,     newState     ,     permit,
	intermediate state1, intermediate stat2     UP/DOWN*/
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_NC},
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D1, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D2, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D3, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D0, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D0, LTQ_CPUFREQ_PS_D0, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_NC},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D2, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D3, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D0, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D1, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D1, LTQ_CPUFREQ_PS_D1, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_STATE_NC},
	{ LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D3, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_STATE_DOWN},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D0, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D1, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D1, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D2, LTQ_TRANS_PERMIT,
		LTQ_CPUFREQ_PS_D2, LTQ_CPUFREQ_PS_D2, LTQ_STATE_UP},
	{ LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_NO_TRANS,
		LTQ_CPUFREQ_PS_D3, LTQ_CPUFREQ_PS_D3, LTQ_STATE_NC}
};
#endif


static int xrx500_cpu_clk = CLOCK_16M;
static int __init cpu_clk(char *str)
{
	int clk;

	if (get_option(&str, &clk)) {
		xrx500_cpu_clk = clk;
		return 0;
	}

	return -EINVAL;
}
early_param("cpuclk", cpu_clk);

#ifdef CONFIG_LTQ_CPUFREQ_DVS
static unsigned long ltq_grx500_get_core_vol(unsigned long rate)
{
	struct clk_rates *avail_rates = avail_rates_xrx500_a;
	unsigned int chip_id = ltq_get_cpu_id();
	unsigned int chip_rev = ltq_get_soc_rev();
	unsigned int chip_type;

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

	while (avail_rates->cpu_freq != 0) {
		if (avail_rates->cpu_freq == rate) {
			switch ((ltq_speed_grade & 0xFF)) {
			case SLOW0:
			case SLOW_DEFAULT:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.slow[0];
				else
					return avail_rates->core_vol550.slow[0];
			case SLOW1:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.slow[1];
				else
					return avail_rates->core_vol550.slow[1];
			case SLOW2:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.slow[2];
				else
					return avail_rates->core_vol550.slow[2];
			case SLOW3:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.slow[3];
				else
					return avail_rates->core_vol550.slow[3];
			case MEDIUM0:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.typ[0];
				else
					return avail_rates->core_vol550.typ[0];
			case MEDIUM1:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.typ[1];
				else
					return avail_rates->core_vol550.typ[1];
			case MEDIUM2:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.typ[2];
				else
					return avail_rates->core_vol550.typ[2];
			case MEDIUM3:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.typ[3];
				else
					return avail_rates->core_vol550.typ[3];
			case FAST0:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.fast[0];
				else
					return avail_rates->core_vol550.fast[0];
			case FAST1:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.fast[1];
				else
					return avail_rates->core_vol550.fast[1];
			case FAST2:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.fast[2];
				else
					return avail_rates->core_vol550.fast[2];
			case FAST3:
				if (chip_type == GRX350)
					return avail_rates->core_vol350.fast[3];
				else
					return avail_rates->core_vol550.fast[3];
			default:
				return avail_rates->core_vol350.typ[0];
			}
		}
		avail_rates++;
	}
	return 0;
}
#endif

static u32 ltq_grx500_clk_div_to_div(u32 clk_div)
{
	u32 div = 2;

	if (clk_div < 6)
		div = clk_div + 1;
	else {
		switch (clk_div) {
		case 6:
			div = 8;
			break;
		case 7:
			div = 10;
			break;
		case 8:
			div = 12;
			break;
		case 9:
			div = 16;
			break;
		case 10:
			div = 20;
			break;
		case 11:
			div = 24;
			break;
		case 12:
			div = 32;
			break;
		case 13:
			div = 40;
			break;
		case 14:
			div = 48;
			break;
		case 15:
			div = 64;
			break;
		default:
			div = 2;
			break;
		}
	}
	return div;
}

void ltq_grx500_set_speed_grade(int spg)
{
	if (spg != UNDEF)
		ltq_speed_grade = spg;

}
EXPORT_SYMBOL(ltq_grx500_set_speed_grade);

int ltq_grx500_get_speed_grade(void)
{
	return ltq_speed_grade;
}
EXPORT_SYMBOL(ltq_grx500_get_speed_grade);

int ltq_grx500_set_cpu_hz(unsigned long cpu_freq)
{
#ifdef CONFIG_LTQ_CPU_FREQ

	return ltq_set_cpu_hz(CLK_GRX500, avail_rates_xrx500_a,
					  freq_transition_xrx500);
#else
	return -1;
#endif /* CONFIG_LTQ_CPU_FREQ */
}

unsigned long ltq_grx500_cpu_hz(void)
{
#ifdef CONFIG_USE_EMULATOR
	return xrx500_cpu_clk;
#else
	unsigned long pllclk;
	u32 val;
	u32 clk4_div;
	u32 div;

	val = ltq_cgu_r32(CGU_PLL0A_CFG1);

	if (val & CGU_PLL0A_CFG1_CPU_CLKM_SEL_MASK) {
		pllclk = CLOCK_2000M;
		clk4_div = MS(ltq_cgu_r32(CGU_PLL0B_CFG1),
				CGU_PLL0B_CFG1_PLL_CLK4_MASK,
				CGU_PLL0B_CFG1_PLL_CLK4_POS);
	} else {
		pllclk = CLOCK_2400M;
		clk4_div = MS(ltq_cgu_r32(CGU_PLL0A_CFG1),
				CGU_PLL0A_CFG1_PLL_CLK4_MASK,
				CGU_PLL0A_CFG1_PLL_CLK4_POS);
	}
	div = ltq_grx500_clk_div_to_div(clk4_div);
	return (unsigned long)(pllclk / div);
#endif
}

/* if *rate==NULL return the core voltage of the current cpu rate.*/
unsigned long ltq_grx500_cpu_vol(unsigned long *rate)
{
	int ret = 0;
#ifdef CONFIG_LTQ_CPUFREQ_DVS
	if (rate == NULL)
		ret = ltq_grx500_get_core_vol(ltq_grx500_cpu_hz());
	else
		ret = ltq_grx500_get_core_vol(*rate);

	if (ret == 0) {
		pr_err("Failed to get core voltage\n");
		return 0;
	}
	return ret;
#else
	return ret; /*no voltage value available*/
#endif
}
EXPORT_SYMBOL_GPL(ltq_grx500_cpu_vol);

int ltq_grx500_set_fpi_hz(unsigned long fpi_freq)
{
#ifdef CONFIG_LTQ_CPU_FREQ
	return -1;
#else
	return -1;
#endif /* CONFIG_LTQ_CPU_FREQ */
}

unsigned long ltq_grx500_fpi_hz(void)
{
#ifdef CONFIG_USE_EMULATOR
	return ltq_grx500_cpu_hz() / 4;
#else
	return CLOCK_200M;
#endif
}

unsigned long ltq_grx500_cbm_hz(void)
{
	u32 div;
	u32 clk1_div;
	unsigned long pllclk = CLOCK_2400M;

	clk1_div = MS(ltq_cgu_r32(CGU_PLL0A_CFG1),
			CGU_PLL0A_CFG1_PLL_CLK1_MASK,
			CGU_PLL0A_CFG1_PLL_CLK1_POS);
	div = ltq_grx500_clk_div_to_div(clk1_div);
	return (unsigned long)(pllclk / div);
}

int ltq_grx500_set_cbm_hz(unsigned long cbm_freq)
{
#ifdef CONFIG_LTQ_CPU_FREQ
	return -1;
#else
	return -1;
#endif /* CONFIG_LTQ_CPU_FREQ */
}

unsigned long ltq_grx500_ngi_hz(void)
{
	u32 div;
	u32 clk2_div;
	unsigned long pllclk = CLOCK_2400M;

	clk2_div = MS(ltq_cgu_r32(CGU_PLL0A_CFG1),
			CGU_PLL0A_CFG1_PLL_CLK2_MASK,
			CGU_PLL0A_CFG1_PLL_CLK2_POS);
	div = ltq_grx500_clk_div_to_div(clk2_div);
	return (unsigned long)(pllclk / div);
}

int ltq_grx500_set_ngi_hz(unsigned long ngi_freq)
{
#ifdef CONFIG_LTQ_CPU_FREQ
	return -1;
#else
	return -1;
#endif /* CONFIG_LTQ_CPU_FREQ */
}

unsigned long ltq_grx500_ddr_hz(void)
{
	u32 div;
	u32 clk3_div;
	unsigned long pllclk = CLOCK_2000M;

	clk3_div = MS(ltq_cgu_r32(CGU_PLL0B_CFG1),
			CGU_PLL0B_CFG1_PLL_CLK3_MASK,
			CGU_PLL0B_CFG1_PLL_CLK3_POS);
	div = ltq_grx500_clk_div_to_div(clk3_div);
	return (unsigned long)(2 * (pllclk / div));
}

#ifdef CONFIG_LTQ_CPU_FREQ
extern unsigned long loops_per_jiffy;

static
inline unsigned long cpufreq_scale(unsigned long old, u_int div, u_int mult)
{
#if BITS_PER_LONG == 32
	u64 result = ((u64) old) * ((u64) mult);
	do_div(result, div);
	return (unsigned long) result;
#elif BITS_PER_LONG == 64
	unsigned long result = old * ((u64) mult);
	result /= div;
	return result;
#endif
};

/* Precondition: all CPUS running on the same frequency
	update clock source
	update clock event
	adjust loops_per_jiffy for udelay
*/
static
int update_sysclock(unsigned long cpu_freq_prev, unsigned long cpu_freq,
						   struct clocksource *cs)
{
	int cpu;
#ifdef CONFIG_CEVT_GIC
	struct clocksource cs_old;
	struct clock_event_device *ce = NULL;
	if (0 == strcmp(cs->name, "GIC")) {
		gic_frequency = cpu_freq;
		memcpy(&cs_old, cs, sizeof(cs_old));
		__clocksource_updatefreq_scale(cs, 1, gic_frequency);
		timekeeping_clocksource_update(cs, &cs_old);
		for_each_possible_cpu(cpu) {
			ce = &per_cpu(gic_clockevent_device, cpu);
			if (ce == NULL)
				return -EINVAL;

			ce->mult = div_sc((unsigned long)gic_frequency,
							NSEC_PER_SEC, 32);
			ce->max_delta_ns = clockevent_delta2ns(0x7fffffff, ce);
			ce->min_delta_ns = clockevent_delta2ns(0x300, ce);
		}
	}
#endif

	/* adjust loops_per_jiffy and udelay here */
	loops_per_jiffy = cpufreq_scale(loops_per_jiffy, cpu_freq_prev,
								cpu_freq);
	/* adjust udelay for each cpu */
	for_each_possible_cpu(cpu) {
		cpu_data[cpu].udelay_val = loops_per_jiffy;
	}

	return 0;
}

static void ltq_grx500_change_ngi_clk(unsigned int ena)
{
	register unsigned int value;

	if (ena) {
		/*ssx2 agent control*/
		value = ltq_xbar2_r32(IDM1R_IA_AGENT_CONTROL);
		value &= ~IDM1R_IA_AGENT_CONTROL_REJECT_MASK; /*clear rej bit*/
		ltq_xbar2_w32(value, IDM1R_IA_AGENT_CONTROL);

		value = ltq_xbar2_r32(IDM1T_IA_AGENT_CONTROL);
		value &= ~IDM1T_IA_AGENT_CONTROL_REJECT_MASK; /*clear rej bit*/
		ltq_xbar2_w32(value, IDM1T_IA_AGENT_CONTROL);

		/*ssx1 agent control*/
		value = ltq_xbar1_r32(IDM2R_IA_AGENT_CONTROL);
		value &= ~IDM2R_IA_AGENT_CONTROL_REJECT_MASK; /*clear rej bit*/
		ltq_xbar1_w32(value, IDM2R_IA_AGENT_CONTROL);

		value = ltq_xbar1_r32(IDM2T_IA_AGENT_CONTROL);
		value &= ~IDM2T_IA_AGENT_CONTROL_REJECT_MASK; /*clear rej bit*/
		ltq_xbar1_w32(value, IDM2T_IA_AGENT_CONTROL);
	} else {
		/*ssx2 agent control*/
		value = ltq_xbar2_r32(IDM1R_IA_AGENT_CONTROL);
		value |= IDM1R_IA_AGENT_CONTROL_REJECT_MASK; /*set reject bit*/
		ltq_xbar2_w32(value, IDM1R_IA_AGENT_CONTROL);
		/*wait until status bits BURTS, ERADEX and REQ_WAITING are '0'*/
		while ((ltq_xbar2_r32(IDM1R_IA_AGENT_STATUS) & IDM1R_STATUS) != 0)
			;

		value = ltq_xbar2_r32(IDM1T_IA_AGENT_CONTROL);
		value |= IDM1T_IA_AGENT_CONTROL_REJECT_MASK; /*set reject bit*/
		ltq_xbar2_w32(value, IDM1T_IA_AGENT_CONTROL);
		/*wait until status bits BURTS, ERADEX and REQ_WAITING are '0'*/
		while ((ltq_xbar2_r32(IDM1T_IA_AGENT_STATUS) & IDM1T_STATUS) != 0)
			;

		/*ssx1 agent control*/
		value = ltq_xbar1_r32(IDM2R_IA_AGENT_CONTROL);
		value |= IDM2R_IA_AGENT_CONTROL_REJECT_MASK; /*set reject bit*/
		ltq_xbar1_w32(value, IDM2R_IA_AGENT_CONTROL);
		/*wait until status bits BURTS, ERADEX and REQ_WAITING are '0'*/
		while ((ltq_xbar1_r32(IDM2R_IA_AGENT_STATUS) & IDM2R_STATUS) != 0)
			;

		value = ltq_xbar1_r32(IDM2T_IA_AGENT_CONTROL);
		value |= IDM2T_IA_AGENT_CONTROL_REJECT_MASK; /*set reject bit*/
		ltq_xbar1_w32(value, IDM2T_IA_AGENT_CONTROL);
		/*wait until status bits BURTS, ERADEX and REQ_WAITING are '0'*/
		while ((ltq_xbar1_r32(IDM2T_IA_AGENT_STATUS) & IDM2T_STATUS) != 0)
			;
	}
}

static int ltq_setclk_hz(unsigned long cpu_freq, unsigned long ddr_freq,
			 unsigned int sel, struct clk_rates *avail_rates)
{
	int retval = 0;
	unsigned long cpu_freq_prev;
	unsigned long sys_flag;
	struct clk *clk;
	unsigned int pll_clk4 = 0; /* cpu */
	unsigned int pll_clk2 = 0; /* ngi */
	unsigned int pll_clk1 = 0; /* cbm */
	unsigned int cpu_clkm_sel = 0; /* cpu clock master select */
	unsigned int value, value2;
	struct clocksource *cs = clocksource_get_current();

	if (!cs) {
		pr_err("current clocksource isn't defined\n");
		return -EINVAL;
	}

	if ((0 != strcmp(cs->name, "GIC")) &&
		(0 != strcmp(cs->name, "gptc-timer"))) {
		pr_err("unknown clocksource detected: %s\n", cs->name);
		return -EINVAL;
	}

	clk = clk_get_cpu();
	if (IS_ERR(clk)) {
		pr_err("CPU clk not found.\n");
		return -EINVAL;
	}
	cpu_freq_prev = clk_get_rate(clk);

	while (avail_rates->cpu_freq != 0) {
		if (avail_rates->cpu_freq == cpu_freq) {
			pll_clk1 = avail_rates->pll_clk1;
			pll_clk2 = avail_rates->pll_clk2;
			pll_clk4 = avail_rates->pll_clk4;
			cpu_clkm_sel = avail_rates->cpu_clkm_sel;
			break;
		}
		avail_rates++;
	}
	if (pll_clk4 == 0) {
		pr_err("No valid cpu clock rate found\n");
		return -EINVAL;
	}

	spin_lock_irqsave(&ltq_setclk_lock, sys_flag);
	ltq_grx500_change_ngi_clk(0);
	/*ssx7 agent control*/
	value = ltq_xbar7_r32(TMI2_TA_AGENT_CONTROL);
	value |= TMI2_TA_AGENT_CONTROL_REJECT_MASK; /*set reject bit*/
	ltq_xbar7_w32(value, TMI2_TA_AGENT_CONTROL);
	/*wait until status bits BURTS, ERADEX and REQ_WAITING are '0'*/
	while ((ltq_xbar7_r32(TMI2_TA_AGENT_STATUS) & TMI2_STATUS) != 0)
		;

	/*ssx6 agent control*/
	value = ltq_xbar6_r32(TIC_TA_AGENT_CONTROL);
	value |= TIC_TA_AGENT_CONTROL_REJECT_MASK; /*set reject bit*/
	ltq_xbar6_w32(value, TIC_TA_AGENT_CONTROL);
	/*wait until status bits BURTS, ERADEX and REQ_WAITING are '0'*/
	while ((ltq_xbar6_r32(TIC_TA_AGENT_STATUS) & TIC_STATUS) != 0)
		;

	value = ltq_cgu_r32(CGU_PLL0A_CFG1);
	value &= ~CGU_PLL0A_CFG1_CPU_CLKM_SEL_MASK;
	value |= cpu_clkm_sel;
	if (value & CGU_PLL0A_CFG1_CPU_CLKM_SEL_MASK) {
		/* PLL0B 2000MHz */
		value &= ~(CGU_PLL0A_CFG1_PLL_CLK2_MASK);/* |*/
			 /*CGU_PLL0A_CFG1_PLL_CLK1_MASK);*/
		value |= (pll_clk2);/* | pll_clk1);*/

		value2 = ltq_cgu_r32(CGU_PLL0B_CFG1);
		value2 &= ~(CGU_PLL0B_CFG1_PLL_CLK4_MASK);
		value2 |= (pll_clk4);
		ltq_cgu_w32(value2, CGU_PLL0B_CFG1);
		ltq_cgu_w32(value, CGU_PLL0A_CFG1);
	} else {
		/* PLL0A 2400MHz */
		value &= ~(CGU_PLL0A_CFG1_PLL_CLK4_MASK |
			CGU_PLL0A_CFG1_PLL_CLK2_MASK);/* |*/
			/*CGU_PLL0A_CFG1_PLL_CLK1_MASK);*/
		value |= (pll_clk4 | pll_clk2);/* | pll_clk1);*/
		ltq_cgu_w32(value, CGU_PLL0A_CFG1);
	}

	/*ssx7 agent control*/
	value = ltq_xbar7_r32(TMI2_TA_AGENT_CONTROL);
	value &= ~TMI2_TA_AGENT_CONTROL_REJECT_MASK; /*clear reject bit*/
	ltq_xbar7_w32(value, TMI2_TA_AGENT_CONTROL);

	/*ssx6 agent control*/
	value = ltq_xbar6_r32(TIC_TA_AGENT_CONTROL);
	value &= ~TIC_TA_AGENT_CONTROL_REJECT_MASK; /*clear reject bit*/
	ltq_xbar6_w32(value, TIC_TA_AGENT_CONTROL);

	ltq_grx500_change_ngi_clk(1);
	retval = update_sysclock(cpu_freq_prev, cpu_freq, cs);
	spin_unlock_irqrestore(&ltq_setclk_lock, sys_flag);

	return retval;
}

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

static int ltq_set_cpu_hz(unsigned int sel,
				   struct clk_rates *avail_rates,
				   struct ltq_freq_transition *freq_tt)
{
	int ret, retclk;
	struct ltq_cpufreq *clk_cpufreq_p;
	unsigned long *ddr_freq_p;
	unsigned long *cpu_freq_p;
	enum ltq_cpufreq_state intState1 = 0;
	enum ltq_cpufreq_state intState2 = 0;
	#ifdef CONFIG_LTQ_CPUFREQ_DVS
	unsigned long vol;
	#endif

	clk_cpufreq_p = ltq_cpufreq_get();
	if (clk_cpufreq_p == NULL)
		return -1;

	ddr_freq_p = clk_cpufreq_p->ddr_scaling_rates;
	if (ddr_freq_p == NULL)
		return -1;

	cpu_freq_p = clk_cpufreq_p->cpu_scaling_rates;
	if (cpu_freq_p == NULL)
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
		#ifdef CONFIG_LTQ_CPUFREQ_DVS
		if (((ret & 0xF00) == LTQ_STATE_UP)) {
			pr_debug("UP step, voltage scaling is called\n");
			vol = ltq_grx500_cpu_vol(cpu_freq_p);
			if (vol > 0)
				regulator_set_voltage(clk_cpufreq_p->regulator,
								vol, vol);
		}
		#endif
		retclk = ltq_setclk_hz(*cpu_freq_p, *ddr_freq_p, sel,
								avail_rates);
		#ifdef CONFIG_LTQ_CPUFREQ_DVS
		if (((ret & 0xF00) == LTQ_STATE_DOWN) || (retclk < 0)) {
			pr_debug("DOWN step, voltage scaling is called\n");
			vol = ltq_grx500_cpu_vol(NULL);
			if (vol > 0)
				regulator_set_voltage(clk_cpufreq_p->regulator,
								vol, vol);
		}
		#endif
		if (retclk < 0)
			return -1;

		if (intState1 != intState2) {
			ddr_freq_p += intState2 - LTQ_CPUFREQ_PS_D0;
			cpu_freq_p += intState2 - LTQ_CPUFREQ_PS_D0;
			#ifdef CONFIG_LTQ_CPUFREQ_DVS
			if (((ret & 0xF00) == LTQ_STATE_UP)) {
				pr_debug("UP step, dvs is called\n");
				vol = ltq_grx500_cpu_vol(cpu_freq_p);
				if (vol > 0)
					regulator_set_voltage(
						clk_cpufreq_p->regulator,
						vol, vol);
			}
			#endif
			retclk = ltq_setclk_hz(*cpu_freq_p, *ddr_freq_p, sel,
								avail_rates);
			#ifdef CONFIG_LTQ_CPUFREQ_DVS
			if (((ret & 0xF00) == LTQ_STATE_DOWN) || (retclk < 0)) {
				pr_debug("DOWN step, dvs is called\n");
				vol = ltq_grx500_cpu_vol(NULL);
				if (vol > 0)
					regulator_set_voltage(
						clk_cpufreq_p->regulator,
						vol, vol);
			}
			#endif
			if (retclk < 0)
				return -1;
		}
	}
	ddr_freq_p += clk_cpufreq_p->cpufreq_new_state - LTQ_CPUFREQ_PS_D0;
	cpu_freq_p += clk_cpufreq_p->cpufreq_new_state - LTQ_CPUFREQ_PS_D0;
	#ifdef CONFIG_LTQ_CPUFREQ_DVS
	if (((ret & 0xF00) == LTQ_STATE_UP)) {
		pr_debug("UP step, voltage scaling is called\n");
		vol = ltq_grx500_cpu_vol(cpu_freq_p);
		if (vol > 0)
			regulator_set_voltage(clk_cpufreq_p->regulator, vol,
									vol);
	}
	#endif

	retclk = ltq_setclk_hz(*cpu_freq_p, *ddr_freq_p, sel, avail_rates);

	#ifdef CONFIG_LTQ_CPUFREQ_DVS
	if (((ret & 0xF00) == LTQ_STATE_DOWN) || (retclk < 0)) {
		pr_debug("DOWN step, voltage scaling is called\n");
		vol = ltq_grx500_cpu_vol(NULL); /*take vol from cur cpu rate*/
		if (vol > 0)
			regulator_set_voltage(clk_cpufreq_p->regulator, vol,
									vol);
	}
	#endif

	return retclk;
}
#endif /* CONFIG_LTQ_CPU_FREQ */



struct clk *clk_get_cpu(void)
{
	return clk_get_sys("cpu", "cpu");
}
EXPORT_SYMBOL_GPL(clk_get_cpu);

struct clk *clk_get_ddr(void)
{
	return clk_get_sys("ddr", "ddr");
}
EXPORT_SYMBOL_GPL(clk_get_ddr);

struct clk *clk_get_fpi(void)
{
	return clk_get_sys("fpi", "fpi");
}
EXPORT_SYMBOL_GPL(clk_get_fpi);

struct clk *clk_get_io(void)
{
	return clk_get_sys("io", "io");
}
EXPORT_SYMBOL_GPL(clk_get_io);

struct clk *clk_get_ppe(void)
{
	return clk_get_sys("ppe", "ppe");
}
EXPORT_SYMBOL_GPL(clk_get_ppe);

struct clk *clk_get_xbar(void)
{
	return clk_get_sys("xbar", "xbar");
}
EXPORT_SYMBOL_GPL(clk_get_xbar);

static inline int clk_good(struct clk *clk)
{
	return clk && !IS_ERR(clk);
}

unsigned long clk_get_rate(struct clk *clk)
{
	if (unlikely(!clk_good(clk)))
		return 0;

#ifndef CONFIG_CPU_FREQ
	if (clk->rate != 0)
		return clk->rate;
#endif

	if (clk->get_rate != NULL) {
		clk->rate = clk->get_rate();
		return clk->rate;
	}
#if 0
	if (clk->parent) {
		pr_info("derive real clock speed for %s from clock %s ",
			clk->cl.dev_id,
			cpu_clk_generic[clk->parent - 1].cl.dev_id);
		pr_info("with scale data 0x%08X\n", clk->scale_data);
		return cpu_clk_generic[clk->parent - 1].rate;
	}
#endif
	return 0;
}
EXPORT_SYMBOL(clk_get_rate);

#if 0
unsigned long clk_get_vol(struct clk *clk, unsigned long *rate)
{
	if (clk->get_vol != NULL)
		return clk->get_vol(rate);

	return 0;
}
EXPORT_SYMBOL(clk_get_vol);
#endif
int clk_set_rate(struct clk *clk, unsigned long rate)
{
	int i = 0;

	if (unlikely(!clk_good(clk)))
		return 0;

	if (clk->rates && *clk->rates) {
		unsigned long *r = clk->rates;

		while (*r && (*r != rate)) {
			r++;
			i++;
		}
		if (!*r) {
			pr_err("clk %s.%s: trying to set invalid rate %ld\n",
				clk->cl.dev_id, clk->cl.con_id, rate);
			return -1;
		}
	}

	if (clk->set_rate != NULL) {
		if (clk->set_rate(rate) == 0) {
			clk->rate = rate;
			return 0;
		}
	}

	pr_err("trying to set invalid clk %s.%s: rate %ld.\n",
			clk->cl.dev_id, clk->cl.con_id, rate);
	pr_err("clk_set_rate not supported for this clk.\n");
	return -EINVAL;
}
EXPORT_SYMBOL(clk_set_rate);

int clk_enable(struct clk *clk)
{
	if (unlikely(!clk_good(clk)))
		return -1;

	if (clk->enable)
		return clk->enable(clk);

	return -1;
}
EXPORT_SYMBOL(clk_enable);

void clk_disable(struct clk *clk)
{
	if (unlikely(!clk_good(clk)))
		return;

	if (clk->disable)
		clk->disable(clk);
}
EXPORT_SYMBOL(clk_disable);

int clk_activate(struct clk *clk)
{
	if (unlikely(!clk_good(clk)))
		return -1;

	if (clk->activate)
		return clk->activate(clk);

	return -1;
}
EXPORT_SYMBOL(clk_activate);

void clk_deactivate(struct clk *clk)
{
	if (unlikely(!clk_good(clk)))
		return;

	if (clk->deactivate)
		clk->deactivate(clk);
}
EXPORT_SYMBOL(clk_deactivate);

struct clk *of_clk_get_from_provider(struct of_phandle_args *clkspec)
{
	return NULL;
}

static inline u32 get_counter_resolution(void)
{
	u32 res;

	__asm__ __volatile__(
		".set	push\n"
		".set	mips32r2\n"
		"rdhwr	%0, $3\n"
		".set pop\n"
		: "=&r" (res)
		: /* no input */
		: "memory");

	return res;
}

void clk_change_init(void)
{
	int id;
	struct resource res_xbar0, res_xbar1, res_xbar2, res_xbar6, res_xbar7;
	struct device_node *np;
	struct device_node *np_xbar0 = NULL;
	struct device_node *np_xbar1 = NULL;
	struct device_node *np_xbar2 = NULL;
	struct device_node *np_xbar6 = NULL;
	struct device_node *np_xbar7 = NULL;

	for_each_compatible_node(np, NULL, "lantiq,ngi-ssx-grx500") {
		id = of_alias_get_id(np, "xbar");
		if (id == 0)
			np_xbar0 = np;
		if (id == 1)
			np_xbar1 = np;
		if (id == 2)
			np_xbar2 = np;
		if (id == 6)
			np_xbar6 = np;
		if (id == 7)
			np_xbar7 = np;
	}

	/* check if all the core register ranges are available */
	if (!np_xbar0 || !np_xbar1 || !np_xbar2 || !np_xbar6 || !np_xbar7)
		panic("Failed to load xbar nodes from devicetree");

	if (of_address_to_resource(np_xbar0, 0, &res_xbar0) ||
	    of_address_to_resource(np_xbar1, 0, &res_xbar1) ||
	    of_address_to_resource(np_xbar2, 0, &res_xbar2) ||
	    of_address_to_resource(np_xbar6, 0, &res_xbar6) ||
	    of_address_to_resource(np_xbar7, 0, &res_xbar7))
		panic("Failed to get core resources");

	if (!(request_mem_region(res_xbar0.start, resource_size(&res_xbar0),
		res_xbar0.name)) ||
	    !(request_mem_region(res_xbar1.start, resource_size(&res_xbar1),
		res_xbar1.name)) ||
	    !(request_mem_region(res_xbar2.start, resource_size(&res_xbar2),
		res_xbar2.name)) ||
	    !(request_mem_region(res_xbar6.start, resource_size(&res_xbar6),
		res_xbar6.name)) ||
	    !(request_mem_region(res_xbar7.start, resource_size(&res_xbar7),
		res_xbar7.name)))
		pr_err("Failed to request core reources");

	ltq_xbar0_membase = ioremap_nocache(res_xbar0.start,
		resource_size(&res_xbar0));
	ltq_xbar1_membase = ioremap_nocache(res_xbar1.start,
		resource_size(&res_xbar1));
	ltq_xbar2_membase = ioremap_nocache(res_xbar2.start,
		resource_size(&res_xbar2));
	ltq_xbar6_membase = ioremap_nocache(res_xbar6.start,
		resource_size(&res_xbar6));
	ltq_xbar7_membase = ioremap_nocache(res_xbar7.start,
		resource_size(&res_xbar7));

	if (!ltq_xbar0_membase || !ltq_xbar1_membase || !ltq_xbar2_membase ||
		!ltq_xbar6_membase || !ltq_xbar7_membase)
		panic("Failed to remap core resources");
}

void __init plat_time_init(void)
{
	struct clk *clk;
	unsigned long cpuclk;

	ltq_soc_init();
	clk_change_init();

	clk = clk_get_cpu();
	if (clk == NULL) {
		pr_err("CPU clock structure not found\n");
		return;
	}
	cpuclk = clk_get_rate(clk);
	mips_hpt_frequency = cpuclk / get_counter_resolution();
#ifdef CONFIG_USE_EMULATOR
	/* we do some hacking here to give illusion we have a faster counter
	 * frequency so that the time interrupt happends less frequently
	 */
	mips_hpt_frequency *= 25;
#endif
	write_c0_compare(read_c0_count());
	pr_info("CPU Clock: %ldHz  mips_hpt_frequency %dHz\n",
		cpuclk, mips_hpt_frequency);
	clk_put(clk);
	{
		struct clk *ppe_clk;
		struct clk *ddr_clk;
		struct clk *ngi_clk;

		ppe_clk = clk_get_ppe();
		ddr_clk = clk_get_ddr();
		ngi_clk = clk_get_xbar();
		pr_info("CBM Clock: %ldHz DDR Clock: %ldHz NGI Clock: %ldHz\n",
			clk_get_rate(ppe_clk), clk_get_rate(ddr_clk),
			clk_get_rate(ngi_clk));

	}

#ifdef CONFIG_IRQ_GIC
	gic_frequency = cpuclk;
#ifdef CONFIG_USE_EMULATOR
	/* we do some hacking here to give illusion we have a faster counter
	 * frequency so that the time interrupt happends less frequently
	 * Make sure that GIC has higher priority than R4K timer
	 */
	gic_frequency *= 15;
#endif
#ifdef CONFIG_CSRC_GIC
	if (gic_present) {
		pr_info("GIC frequency %d.%02d MHz\n", gic_frequency / 1000000,
			(gic_frequency % 1000000) * 100 / 1000000);
		gic_clocksource_init(gic_frequency);
	}
#endif
#endif

#if (defined CONFIG_CEVT_GPTC) || (defined CONFIG_CSRC_GPTC)
	gptc_module_init();
#endif

#ifdef CONFIG_CEVT_GPTC
	gptc_clockevent_init();
#endif
#ifdef CONFIG_CSRC_GPTC
	gptc_clocksource_init();
#endif
}
