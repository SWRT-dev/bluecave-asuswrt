#define LTQ_MAX_TSO_PORTS 4
#define MAX_NUMBER_OF_CMDS 100
#define LTQ_MAX_LRO_PORTS 8

#define USE_TIMER_FOR_SESSION_STOP
#undef CONFIG_USE_SKB_FRAGS_ARRAY
#define CONFIG_LTQ_TOE_USE_SW_POLLING

typedef struct ltq_tso_port {
	unsigned char __iomem *membase; /* Virtual */
} ltq_tso_port_t;

typedef struct ltq_lro_port {
	unsigned char __iomem *membase; /* Virtual */
	u32 port_num;
	unsigned int irq_num;
	bool in_use;
	int session_id;
	int to_read;
#ifdef USE_TIMER_FOR_SESSION_STOP
	struct timer_list lro_timer;
#endif
} ltq_lro_port_t;

static ltq_tso_port_t ltq_tso_port[NR_CPUS];

static ltq_lro_port_t ltq_lro_port[LTQ_MAX_LRO_PORTS];
