
/* Base address of GPTC0 module */
#define GPTC0_BASE_ADDRESS 0x16b00000
#define GPTC1_BASE_ADDRESS 0x16300000

#define GPTC_TIMER_FREQ 200000000
#define GPTC1_IRQ_BASE 168

/* ID byte of the core */
#define GPTU_CLKSRC_MAGIC	0x59
/* clock control register */
#define GPTU_CLKSRC_CLC	0x00
/* id register */
#define GPTU_CLKSRC_ID		0x08
/* interrupt node enable */
#define GPTU_CLKSRC_IRNEN	0xf4
/* interrupt control register */
#define GPTU_CLKSRC_IRCR	0xf8
/* interrupt capture register */
#define GPTU_CLKSRC_IRNCR	0xfc
#if 1
/* there are 3 identical blocks of 2 timers. calculate register offsets */
#define GPTU_CLKSRC_SHIFT(x)	(x % 2 ? 4 : 0)
#define GPTU_CLKSRC_BASE(x)	(((x >> 1) * 0x20) + 0x10)
/* timer control register */
#define GPTU_CLKSRC_CON(x)	(GPTU_CLKSRC_BASE(x) + GPTU_CLKSRC_SHIFT(x) + 0x00)
/* timer auto reload register */
#define GPTU_CLKSRC_RUN(x)	(GPTU_CLKSRC_BASE(x) + GPTU_CLKSRC_SHIFT(x) + 0x08)
/* timer manual reload register */
#define GPTU_CLKSRC_RLD(x)	(GPTU_CLKSRC_BASE(x) + GPTU_CLKSRC_SHIFT(x) + 0x10)
/* timer count register */
#define GPTU_CLKSRC_CNT(x)	(GPTU_CLKSRC_BASE(x) + GPTU_CLKSRC_SHIFT(x) + 0x18)

#endif

/* GPTU_CON(x) */
#define CON_CLKSRC_DIR		BIT(1)
#define CON_CLKSRC_CNT		BIT(2)
#define CON_CLKSRC_STP		BIT(3)
#define CON_CLKSRC_EXT		BIT(4)
#define CON_CLKSRC_EDGE_ANY	(BIT(7) | BIT(6))
#define CON_CLKSRC_SYNC	BIT(8)
#define CON_CLKSRC_CLK_INT	BIT(10)
#define CON_CLKSRC_CLK_EXT	BIT(9)

/* GPTU_RUN(x) */
#define RUN_CLKSRC_SEN		BIT(0)
#define RUN_CLKSRC_RL		BIT(2)
#define RUN_CLKSRC_STOP    BIT(1)
/* set clock to runmode */
#define CLC_CLKSRC_RMC		BIT(8)
#define CLC_CLKSRC_SMC     BIT(16)

/* bring core out of suspend */
#define CLC_SUSPEND	BIT(4)
/* the disable bit */
#define CLC_DISABLE	BIT(0)
int __init gptc_clocksource_init(void);
