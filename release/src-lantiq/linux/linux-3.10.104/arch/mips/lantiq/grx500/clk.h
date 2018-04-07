/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 * Copyright (C) 2010 John Crispin <blogic@openwrt.org>
 * Copyright (C) 2013 Lei Chuanhua <Chuanhua.lei@lantiq.com>
 */

#ifndef _LTQ_CLK_H__
#define _LTQ_CLK_H__

#include <linux/clkdev.h>

/* clock speeds */
#define CLOCK_16M	16000000
#define CLOCK_33M	33333333
#define CLOCK_50M	50000000
#define CLOCK_100M	100000000
#define CLOCK_133M	133333333
#define CLOCK_150M	150000000
#define CLOCK_166M	166666666
#define CLOCK_200M	200000000
#define CLOCK_300M	300000000
#define CLOCK_333M	333333333
#define CLOCK_400M	400000000
#define CLOCK_500M	500000000
#define CLOCK_600M	600000000
#define CLOCK_666M	666666666
#define CLOCK_720M	720000000
#define CLOCK_800M	800000000
#define CLOCK_1000M	1000000000
#define CLOCK_2000M	2000000000UL
#define CLOCK_2400M	2400000000UL

/* clock out speeds */
#define CLOCK_8M_192K	8192000
#define CLOCK_25M	25000000
#define CLOCK_40M	40000000

enum {
	STATIC_CPU_CLK = 1,
	STATIC_FPI_CLK,
	STATIC_IO_CLK,
	STATIC_PPE_CLK,
	STATIC_NO_PARENT = 0xff,
};

struct vol_sg {/*voltage speed-grade*/
	unsigned int slow[4];
	unsigned int typ[4];
	unsigned int fast[4];
};

struct clk_rates {
	unsigned int cpu_freq;
	unsigned int ddr_freq;
	unsigned int cpu_clkm_sel;
	unsigned int pll_clk1;
	unsigned int pll_clk2;
	unsigned int pll_clk4;
	struct vol_sg core_vol350;
	struct vol_sg core_vol550;
};

struct clk {
	struct clk_lookup cl;
	unsigned long rate;
	unsigned long *rates;
	unsigned int module;
	unsigned int bits;
	unsigned int parent;
	unsigned int scale_data;
	atomic_t refcount;
	bool always_on; /* 0 -- always on or not used, 1 -- configurable */
	unsigned long (*get_rate) (void);
	int (*set_rate) (unsigned long cpu_freq);
	int (*enable) (struct clk *clk);
	void (*disable) (struct clk *clk);
	int (*activate) (struct clk *clk);
	void (*deactivate) (struct clk *clk);
	void (*reboot) (struct clk *clk);
};

enum spd_class_t {
	FAST0 = 0,
	FAST1,
	FAST2,
	FAST3,
	MEDIUM0,
	MEDIUM1,
	MEDIUM2,
	MEDIUM3,
	SLOW0,
	SLOW1,
	SLOW2,
	SLOW3,
	SLOW_DEFAULT,
	UNDEF
};

extern unsigned long ltq_grx500_cpu_hz(void);
extern int ltq_grx500_set_cpu_hz(unsigned long cpu_freq);
extern unsigned long ltq_grx500_cpu_vol(unsigned long *rate);

extern unsigned long ltq_grx500_fpi_hz(void);
extern int ltq_grx500_set_fpi_hz(unsigned long fpi_freq);

extern unsigned long ltq_grx500_cbm_hz(void);
extern int ltq_grx500_set_cbm_hz(unsigned long cbm_freq);

extern unsigned long ltq_grx500_ngi_hz(void);
extern int ltq_grx500_set_ngi_hz(unsigned long ngi_freq);

extern unsigned long ltq_grx500_ddr_hz(void);

extern unsigned long *ltq_get_avail_scaling_rates(int sel);
extern unsigned long ltq_grx500_cpu_vol(unsigned long *rate);
extern int ltq_grx500_get_speed_grade(void);
extern void ltq_grx500_set_speed_grade(int spg);
#endif
