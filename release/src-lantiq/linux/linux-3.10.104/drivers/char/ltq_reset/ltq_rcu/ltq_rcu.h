/******************************************************************************
**
** FILE NAME	: grx500_rcu.h
** PROJECT		: grx500
** MODULES		: RCU (Reset Control Unit)
**
** DESCRIPTION	: Global RCU driver header file
** COPYRIGHT	:	Copyright (c) 2006
**					Infineon Technologies AG
**	Am Campeon 1-12, 85579 Neubiberg, Germany
**
**	  This program is free software; you can redistribute it and/or modify
**	  it under the terms of the GNU General Public License as published by
**	  the Free Software Foundation; either version 2 of the License, or
**	  (at your option) any later version.
**
** HISTORY
******************************************************************************/

#ifndef LTQ_RCU_H
#define LTQ_RCU_H




/*
 * ########################################################
 *				Definition of offset of RCU register
 * ########################################################
 */

/* reset request register*/
#define LTQ_RCU_RST_REQ		0x0010
/* reset request register 2*/
#define LTQ_RCU_RST_REQ2	0x0048
/* reset status register*/
#define LTQ_RCU_RST_STAT	0x0014
/* reset status register 2*/
#define LTQ_RCU_RST_STAT2	0x0024

#define RCU_REG_RST_OFFSET(x)	((x == RCU_REG_BLK0)\
	? LTQ_RCU_RST_REQ : LTQ_RCU_RST_REQ2)
#define RCU_REG_STAT_OFFSET(x)	((x == RCU_REG_BLK0)\
	? LTQ_RCU_RST_STAT : LTQ_RCU_RST_STAT2)
#define NUM_ENTITY(x)	(sizeof(x) / sizeof(*(x)))

enum {
	RCU_REG_BLK0 = 0,
	RCU_REG_BLK1,
};


enum {
	LTQ_RCU_DOMAIN_HRST  = 0,
	LTQ_RCU_DOMAIN_TEP,/* Boot core, MIPS4kec */
	LTQ_RCU_DOMAIN_CPU_CLUSTER_RST,/* interAptiv cluster reset */
	LTQ_RCU_DOMAIN_CPU_CLUSTER_RLS,/* interAptiv cluster reset release */
	LTQ_RCU_DOMAIN_HRST_CFG,/* HRST config active low enable */
	LTQ_RCU_DOMAIN_CBM,/* CBM & TMU reset */
	LTQ_RCU_DOMAIN_PCIE_PHY3,/* PCIe PHY3 */
	LTQ_RCU_DOMAIN_PCIE_PHY1,/* PCIe PHY1 */
	LTQ_RCU_DOMAIN_PCIE_PHY2,/* PCIe PHY2 */
	LTQ_RCU_DOMAIN_XBAR6,/* Crossbar 6 */
	LTQ_RCU_DOMAIN_GSWIP_L,/* GSWIP_L */
	LTQ_RCU_DOMAIN_WLAN,/* WLAN */
	/* PHY5 reset when set 1, no auto clear by hw, need SW clear */
	LTQ_RCU_DOMAIN_PHY5,
	/* PHY4 reset when set 1, no auto clear by hw, need SW clear */
	LTQ_RCU_DOMAIN_PHY4,
	/* PHY3 reset when set 1, no auto clear by hw, need SW clear */
	LTQ_RCU_DOMAIN_PHY3,
	/* PHY2 reset when set 1, no auto clear by hw, need SW clear */
	LTQ_RCU_DOMAIN_PHY2,
	LTQ_RCU_DOMAIN_GLB_SW,/* Global software reset */
	/* GPHY6 reset when set 1, no auto clear by hw, need SW clear */
	LTQ_RCU_DOMAIN_PHY6F,
	/* can add more domains in the first half
	* for RCU_REG_BLK0's components */
	LTQ_RCU_DOMAIN_HALF = 31,
	LTQ_RCU_DOMAIN_VCODEC,/* Voice Codec */
	LTQ_RCU_DOMAIN_OCP2SRAM,/* OCP2SRAM Bridge */
	LTQ_RCU_DOMAIN_XBAR7,/* Crossbar 7 */
	LTQ_RCU_DOMAIN_SL40,/* SL40 */
	/* USB_PHY0, no auto cleared by hw, need sw to clear */
	LTQ_RCU_DOMAIN_USB_PHY0,
	/* USB_PHY1, no auto cleared by hw, need sw to clear */
	LTQ_RCU_DOMAIN_USB_PHY1,
	/* Soft GPHY6 reset when set 1, no auto clear by hw, need SW clear */
	LTQ_RCU_DOMAIN_SR_PHYF,
	/* Soft PHY2 reset when set 1, no auto clear by hw, need SW clear */
	LTQ_RCU_DOMAIN_SR_PHY2,
	/* Soft PHY3 reset when set 1, no auto clear by hw, need SW clear */
	LTQ_RCU_DOMAIN_SR_PHY3,
	LTQ_RCU_DOMAIN_TEMPS,/* Temperature sensor reset */
	/* Soft PHY5 reset when set 1, no auto clear by hw, need SW clear */
	LTQ_RCU_DOMAIN_SR_PHY5,
	/* Soft PHY4 reset when set 1, but no auto clear by hw, need SW clear */
	LTQ_RCU_DOMAIN_SR_PHY4,
	LTQ_RCU_DOMAIN_XBAR3,/* Crossbar 3 */
	LTQ_RCU_DOMAIN_XBAR2,/* Crossbar 2 */
	LTQ_RCU_DOMAIN_XBAR1,/* Crossbar 1 */
	LTQ_RCU_DOMAIN_XBAR0,/* Crossbar 0 */
	LTQ_RCU_DOMAIN_GSWIP_R,/* GSWIP_R */
	LTQ_RCU_DOMAIN_E123_ABORT_REQ,/* EIP123 reset/abort request */
	LTQ_RCU_DOMAIN_E123_ABORT_ACK,/* Ack Ready for EIP123 to reset/abort */
	LTQ_RCU_DOMAIN_GPHY_CDB,/* GPHY CDB */
	LTQ_RCU_DOMAIN_DDR_CTL,/* DDR Controller */
	LTQ_RCU_DOMAIN_DDR_PUB,/* DDR PUB */
	LTQ_RCU_DOMAIN_DDR_PHY,/* DDR PHY */
	/* can add more domains in the first half for
	* RCU_REG_BLK1's components */
	LTQ_RCU_DOMAIN_MAX,
};

#define LTQ_RCU_DECLARE_DOMAIN_NAME(var)\
	char *var[] = {\
	"HRST",\
	"TEP",\
	"CPU_CLUSTER_RST",\
	"CPU_CLUSTER_RLS",\
	"HRST_CFG",\
	"CBM",\
	"PCIE_PHY3",\
	"PCIE_PHY1",\
	"PCIE_PHY2",\
	"XBAR6",\
	"GSWIP_L",\
	"WLAN",\
	"PHY5",\
	"PHY4",\
	"PHY3",\
	"PHY2",\
	"GLB_SW",\
	"PHY6F",\
	"VCODEC",\
	"OCP2SRAM",\
	"XBAR7",\
	"SL40",\
	"USB_PHY0",\
	"USB_PHY1",\
	"SR_PHYF",\
	"SR_PHY2",\
	"SR_PHY3",\
	"TEMPS",\
	"SR_PHY5",\
	"SR_PHY4",\
	"XBAR3",\
	"XBAR2",\
	"XBAR1",\
	"XBAR0",\
	"GSWIP_R",\
	"E123_ABORT_REQ",\
	"E123_ABORT_ACK",\
	"GPHY_CDB",\
	"DDR_CTL",\
	"DDR_PUB",\
	"DDR_PHY",\
}


enum {
	LTQ_RCU_MODULE_USB,
	LTQ_RCU_MODULE_ETH,
	LTQ_RCU_MODULE_ATM,
	LTQ_RCU_MODULE_PTM,
	LTQ_RCU_MODULE_PPA,
	LTQ_RCU_MODULE_DMA,
	LTQ_RCU_MODULE_SDIO,
	LTQ_RCU_MODULE_MEI,
	LTQ_RCU_MODULE_TAPI,
	LTQ_RCU_MODULE_PCI,
	LTQ_RCU_MODULE_NAND,
	LTQ_RCU_STANDALONE,
	/* add more component in the future */
	LTQ_RCU_MODULE_MAX,
};

#define LTQ_RCU_DECLARE_MODULE_NAME(var)\
	char *var[] = {\
		"USB",\
		"ETH",\
		"ATM",\
		"PTM",\
		"PPA",\
		"DMA",\
		"SDIO",\
		"MEI",\
		"TAPI",\
		"PCI",\
		"NAND",\
		"STANDALONE",\
	}

typedef int (*ltq_rcu_callbackfn)(unsigned int reset_domain_id,
	unsigned int module_id,
	int f_after_reset,
	unsigned long arg
	);


/*
 * ####################################
 *				Data Type
 * ####################################
 */

struct ltq_rcu_handler_t {
	struct ltq_rcu_handler_t   *next;
	ltq_rcu_callbackfn			fn;
	unsigned long				arg;
	unsigned int				module_id;
};

struct ltq_rcu_domain_t {
	/* 0 for RST_REQ & RST_STAT,1 for RST_REQ2 & RST_STAT2 */
	unsigned int		regblock;
	/* domains affected by reset to this domain (inclusive) */
	unsigned int		affected_domains;
	/* value write to RST_REQ register */
	unsigned int		rst_req_value;
	/* mask of RST_REQ register */
	unsigned int		rst_req_mask;
	/* mask of RST_STAT register */
	unsigned int		rst_stat_mask;
	/* latch, hold the signal for some while */
	unsigned int		latch;
	unsigned int		udelay;
	struct ltq_rcu_handler_t	*handlers;
};

/*
 * ####################################
 *				 Global RCU Domains
 * ####################################
 */







/*
 * ####################################
 *				  IOCTL
 * ####################################
 */


struct ltq_rcu_ioctl_version {
	unsigned int	major;	/*!< output, major number of driver */
	unsigned int	mid;	/*!< output, mid number of driver */
	unsigned int	minor;	/*!< output, minor number of driver */
};

/*!
  \struct ltq_rcu_ioctl_query_rst_domain
  \brief Structure used to get reset status of given hardware module.
 */
struct ltq_rcu_ioctl_query_rst_domain {
	unsigned int	domain_id;/*!< input, hardware module ID */
	int				f_reset;/*!< output, reset status */
};


#define LTQ_RCU_IOC_MAGIC				0xe0
/*!
\def LTQ_RCU_IOC_VERSION
\brief RCU IOCTL Command - Get driver version number.
 This command uses struct "ltq_rcu_ioctl_version" as parameter
 to RCU driver version number.
*/
#define LTQ_RCU_IOC_VERSION	_IOR(LTQ_RCU_IOC_MAGIC,\
	0,\
	struct ltq_rcu_ioctl_version)
/*!
\def LTQ_RCU_IOC_QUERY_RST_DOMAIN
\brief RCU IOCTL Command - Get reset status of given hardware module.
\This command uses struct "ltq_rcu_ioctl_query_rst_domain" as parameter
\to get reset status of given hardware module.
*/
#define LTQ_RCU_IOC_QUERY_RST_DOMAIN	_IOWR(LTQ_RCU_IOC_MAGIC,\
	1,\
	struct ltq_rcu_ioctl_query_rst_domain)


/*
 * ####################################
 *				   API
 * ####################################
 */


#ifdef __KERNEL__
/*
* RST_REQ/RST_STAT Register Access
* For accessing to special bits in these two registers,
* such as fusing, endianess
*/
unsigned int ltq_rcu_rst_req_read(unsigned int regblk);
void ltq_rcu_rst_req_write(unsigned int value,
	unsigned int regblk,
	unsigned int mask)
	;
unsigned int ltq_rcu_rst_stat_read(unsigned int regblk);

/*
*	Reset Operation
*/
int ltq_rcu_request(unsigned int reset_domain_id,
	unsigned int module_id,
	ltq_rcu_callbackfn callbackfn,
	unsigned long arg
	);
int ltq_rcu_free(unsigned int reset_domain_id,
	unsigned int module_id);
int ltq_rcu_stat_get(unsigned int reset_domain_id);
void ltq_rcu_stat_clear(unsigned int reset_domain_id);
int ltq_rcu_rst(unsigned int reset_domain_id,
	unsigned int module_id);
#endif

/*
 *	find first 1 from MSB in a 32-bit word
 *	if all ZERO, return -1
 *	e.g. 0x10000000 => 28
 */
static inline uint32_t clz(uint32_t x)
{
	__asm__ (
	"		.set	push									\n"
	"		.set	mips32									\n"
	"		clz 	%0, %1									\n"
	"		.set	pop										\n"
	: "=r" (x)
	: "r" (x));

	return 31 - (uint32_t)x;
}


#endif	/* LTQ_RCU_H */
