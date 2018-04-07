/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2009~2015 Lei Chuanhua <chuanhua.lei@lantiq.com>
 *  Copyright(c) 2016 Intel Corporation.
 */
#ifndef PCIE_LANTIQ_H
#define PCIE_LANTIQ_H
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/reset.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>

#include <lantiq.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <lantiq_soc.h>


/*!
 \defgroup PCIE  PCI Express bus driver module
 \brief  PCI Express IP module support VRX200/ARX300/HN
*/

/*!
 \defgroup PCIE_OS OS APIs
 \ingroup PCIE
 \brief PCIe bus driver OS interface functions
*/

/*!
 \file pcie-lantiq.h
 \ingroup PCIE
 \brief header file for PCIe module common header file
*/

#define MS(_v, _f)  (((_v) & (_f)) >> _f##_S)
#define SM(_v, _f)  (((_v) << _f##_S) & (_f))

/* Debug option, more will be coming */

#define PCIE_MSG_MSI		0x00000001
#define PCIE_MSG_ISR		0x00000002
#define PCIE_MSG_FIXUP		0x00000004
#define PCIE_MSG_READ_CFG	0x00000008
#define PCIE_MSG_WRITE_CFG	0x00000010
#define PCIE_MSG_CFG		(PCIE_MSG_READ_CFG | PCIE_MSG_WRITE_CFG)
#define PCIE_MSG_REG		0x00000020
#define PCIE_MSG_INIT		0x00000040
#define PCIE_MSG_ERR		0x00000080
#define PCIE_MSG_PHY		0x00000100
#define PCIE_MSG_ANY		0x000001ff

/* #define LTQ_PCIE_DBG */

/* Reuse kernel stuff, but we need to differentiate baseline
 * error reporting and AEE */
#ifdef CONFIG_PCIEAER
#define LTQ_PCIE_BASIC_ERROR_INT
#endif /* CONFIG_PCIEAER */

/* XXX, should be only enabled after LTQ_PCIE_BASIC_ERROR_INT */
#define LTQ_PCIE_AER_REPORT

/* Always report fatal error */
#define PCIE_KASSERT(exp, msg) do {	\
	if (unlikely(!(exp))) {	\
		printk msg;		\
		BUG();			\
	}				\
} while (0)

/* Port number definition */
enum {
	LTQ_PCIE_PORT0 = 0,
	LTQ_PCIE_PORT1,
	LTQ_PCIE_PORT2,
};

#define MSI_IRQ_PIC_TABLE_SZ		16

/* Interrupt related stuff */
#define PCIE_LEGACY_DISABLE		0
#define PCIE_LEGACY_INTA		1
#define PCIE_LEGACY_INTB		2
#define PCIE_LEGACY_INTC		3
#define PCIE_LEGACY_INTD		4
#define PCIE_LEGACY_INT_MAX		PCIE_LEGACY_INTD

struct msi_irq_idx {
	int irq;
	int idx;
};

struct ltq_msi_pic {
	volatile u32 pic_table[MSI_IRQ_PIC_TABLE_SZ];
	volatile u32 pic_endian; /* 0x40  */
};

struct msi_irq {
	struct ltq_msi_pic *msi_pic_p;
	u32 msi_phy_base;
	struct msi_irq_idx msi_irq_idx[MSI_IRQ_PIC_TABLE_SZ];
	spinlock_t msi_lock;
	/*
	 * Each bit in msi_free_irq_bitmask represents a MSI interrupt that is
	 * in use.
	 */
	u16 msi_free_irq_bitmask;

	/*
	 * Each bit in msi_multiple_irq_bitmask tells that the device using
	 * this bit in msi_free_irq_bitmask is also using the next bit. This
	 * is used so we can disable all of the MSI interrupts when a device
	 * uses multiple.
	 */
	u16 msi_multiple_irq_bitmask;
};

/* used to differentiate between Lantiq  SoC generations */
struct ltq_pcie_soc_data {
	unsigned int num_ports;
	bool has_phy_clk;
	bool has_ahb_clk;
	bool has_bus_clk;
	bool has_pdi_clk;
	bool has_msi_clk;
	bool has_core_rst;
	bool has_clk_out;
	bool has_gen2;
	bool has_ahb2m_endian;
	bool has_ahb4s_endian;
	bool has_ahb3m_endian;
	int ahb2m_endian_ctrl_shift;
	int ahb4s_endian_ctrl_shift;
	int ahb3m_endian_ctrl_shift;
	unsigned int per_rc_max_msi_irqs;
};

/** Structure used to extract physical Root Complex index number,
 * it is shared between RC and EP for the first two fields */
struct ltq_pcie_port {
	/*!< PCI controller information used as system specific information */
	struct pci_controller pcic;
	void __iomem	*app_base;
	int id; /* Physical RC Index */
	struct device	*dev;
	spinlock_t lock; /*!< Per controller lock */
	 /*!< RC specific, per host bus information */
	void __iomem	*mem_base;
	void __iomem	*devcfg_base;
	phys_addr_t	cfg_phy_base;

	void __iomem	*phy_base;
	phys_addr_t	msi_base;
	void __iomem	*msi_pic;
	u32		io_base;
	phys_addr_t	io_phys_base;
	u32		io_size;
	phys_addr_t	mem_phys_base;
	u32		mem_size;
	struct resource		io;
	struct resource		mem;
	struct resource		prefetch;
	int		reset_gpio;
	bool		high_active_rst;
	u32		rst_interval;
	bool		inbound_swap;
	u32		inbound_shift;
	bool		outbound_swap;
	u32		outbound_shift;
	u32		clk_out_pin;
	int		err_irq;
	char		irq_name[16];
	struct msi_irq	msi_irqs;
	int		phy_mode;
	struct clk *phy_clk;
	struct clk *core_clk;
	struct clk *msi_clk;
	struct clk *pdi_clk;
	struct clk *ahb_clk;
	struct clk *bus_clk;
	struct reset_control *phy_rst;
	struct reset_control *core_rst;
	const struct ltq_pcie_soc_data *soc_data;
};

extern int pcie_phy_clock_ppm_enabled(void);
extern int pcie_phy_clock_mode_setup(struct ltq_pcie_port *lpp);
extern void pcie_lcpll_init(struct ltq_pcie_port *lpp);
extern void pcie_msi_pic_init(struct ltq_pcie_port *lpp);
extern struct ltq_pcie_port *bus_to_ltq_pcie_port(struct pci_bus *bus);

extern int (*ltq_pci_plat_dev_init)(struct pci_dev *dev);
extern int (*ltq_pci_plat_arch_init) (struct pci_dev *dev);
extern int (*ltq_pci_map_irq) (const struct pci_dev *dev, u8 slot, u8 pin);
#ifdef LTQ_PCIE_BASIC_ERROR_INT
extern struct pci_dev *aer_dev_to_pci_dev(void *context);
extern irqreturn_t aer_irq(int irq, void *context);
#endif /* LTQ_PCIE_BASIC_ERROR_INT */

/* PCIe Application Logic Register */
/* RC Core Control Register */
#define PCIE_RC_CCR		0x10

/* This should be enabled after initializing configuratin registers
 * Also should check link status retraining bit
 */
/* Enable LTSSM to continue link establishment */
#define PCIE_RC_CCR_LTSSM_ENABLE	0x00000001
/* RC Core Debug Register */
#define PCIE_RC_DR		0x14

#define PCIE_RC_DR_DLL_UP			0x00000001
#define PCIE_RC_DR_CURRENT_POWER_STATE		0x0000000E
#define PCIE_RC_DR_CURRENT_POWER_STATE_S	1
#define PCIE_RC_DR_CURRENT_LTSSM_STATE		0x000001F0
#define PCIE_RC_DR_CURRENT_LTSSM_STATE_S	4

#define PCIE_RC_DR_PM_DEV_STATE			0x00000E00
#define PCIE_RC_DR_PM_DEV_STATE_S		9

#define PCIE_RC_DR_PM_ENABLED			0x00001000
#define PCIE_RC_DR_PME_EVENT_ENABLED		0x00002000
#define PCIE_RC_DR_AUX_POWER_ENABLED		0x00004000

/* Current Power State Definition */
enum {
	PCIE_RC_DR_D0 = 0,
	PCIE_RC_DR_D1, /* Not supported */
	PCIE_RC_DR_D2, /* Not supported */
	PCIE_RC_DR_D3,
	PCIE_RC_DR_UN,
};

/* PHY Link Status Register */
#define PCIE_PHY_SR		0x18

#define PCIE_PHY_SR_PHY_LINK_UP		0x00000001

/* Electromechanical Control Register */
#define PCIE_EM_CR		0x1C

#define PCIE_EM_CR_CARD_IS_PRESENT		0x00000001
#define PCIE_EM_CR_MRL_OPEN			0x00000002
#define PCIE_EM_CR_POWER_FAULT_SET		0x00000004
#define PCIE_EM_CR_MRL_SENSOR_SET		0x00000008
#define PCIE_EM_CR_PRESENT_DETECT_SET		0x00000010
#define PCIE_EM_CR_CMD_CPL_INT_SET		0x00000020
#define PCIE_EM_CR_SYS_INTERLOCK_SET		0x00000040
#define PCIE_EM_CR_ATTENTION_BUTTON_SET		0x00000080

/* Interrupt Status Register */
#define PCIE_IR_SR		0x20

#define PCIE_IR_SR_PME_CAUSE_MSI		0x00000002
#define PCIE_IR_SR_HP_PME_WAKE_GEN		0x00000004
#define PCIE_IR_SR_HP_MSI			0x00000008
#define PCIE_IR_SR_AHB_LU_ERR			0x00000030
#define PCIE_IR_SR_AHB_LU_ERR_S			4
#define PCIE_IR_SR_INT_MSG_NUM			0x00003E00
#define PCIE_IR_SR_INT_MSG_NUM_S		9
#define PCIE_IR_SR_AER_INT_MSG_NUM		0xF8000000
#define PCIE_IR_SR_AER_INT_MSG_NUM_S		27

/* Message Control Register */
#define PCIE_MSG_CR		0x30

#define PCIE_MSG_CR_GEN_PME_TURN_OFF_MSG	0x00000001
#define PCIE_MSG_CR_GEN_UNLOCK_MSG		0x00000002

#define PCIE_VDM_DR		0x34

/* Vendor-Defined Message Requester ID Register */
#define PCIE_VDM_RID		0x38

#define PCIE_VDM_RID_VENROR_MSG_REQ_ID		0x0000FFFF
#define PCIE_VDM_RID_VDMRID_S			0

/* ASPM Control Register */
#define PCIE_ASPM_CR		0x40

#define PCIE_ASPM_CR_HOT_RST			0x00000001
#define PCIE_ASPM_CR_REQ_EXIT_L1		0x00000002
#define PCIE_ASPM_CR_REQ_ENTER_L1		0x00000004

/* Vendor Message DW0 Register */
#define PCIE_VM_MSG_DW0		0x50

#define PCIE_VM_MSG_DW0_TYPE		0x0000001F /* Message type */
#define PCIE_VM_MSG_DW0_TYPE_S		0
#define PCIE_VM_MSG_DW0_FORMAT		0x00000060 /* Format */
#define PCIE_VM_MSG_DW0_FORMAT_S	5
#define PCIE_VM_MSG_DW0_TC		0x00007000 /* Traffic Class */
#define PCIE_VM_MSG_DW0_TC_S		12
#define PCIE_VM_MSG_DW0_ATTR		0x000C0000 /* Atrributes */
#define PCIE_VM_MSG_DW0_ATTR_S		18
#define PCIE_VM_MSG_DW0_EP_TLP		0x00100000 /* Poisoned TLP */
#define PCIE_VM_MSG_DW0_TD		0x00200000 /* TLP Digest */
#define PCIE_VM_MSG_DW0_LEN		0xFFC00000 /* Length */
#define PCIE_VM_MSG_DW0_LEN_S		22

/* Format Definition */
enum {
	PCIE_VM_MSG_FORMAT_00 = 0, /* 3DW Hdr, no data */
	PCIE_VM_MSG_FORMAT_01, /* 4DW Hdr, no data */
	PCIE_VM_MSG_FORMAT_10, /* 3DW Hdr, with data */
	PCIE_VM_MSG_FORMAT_11, /* 4DW Hdr, with data */
};

/* Traffic Class Definition */
enum {
	PCIE_VM_MSG_TC0 = 0,
	PCIE_VM_MSG_TC1,
	PCIE_VM_MSG_TC2,
	PCIE_VM_MSG_TC3,
	PCIE_VM_MSG_TC4,
	PCIE_VM_MSG_TC5,
	PCIE_VM_MSG_TC6,
	PCIE_VM_MSG_TC7,
};

/* Attributes Definition */
enum {
	PCIE_VM_MSG_ATTR_00 = 0, /* RO and No Snoop cleared */
	PCIE_VM_MSG_ATTR_01, /* RO cleared , No Snoop set */
	PCIE_VM_MSG_ATTR_10, /* RO set, No Snoop cleared */
	PCIE_VM_MSG_ATTR_11, /* RO and No Snoop set */
};

/* Payload Size Definition */
#define PCIE_VM_MSG_LEN_MIN		0
#define PCIE_VM_MSG_LEN_MAX		1024

/* Vendor Message DW1 Register */
#define PCIE_VM_MSG_DW1		0x54

#define PCIE_VM_MSG_DW1_FUNC_NUM	0x00000070 /* Function Number */
#define PCIE_VM_MSG_DW1_FUNC_NUM_S	8
#define PCIE_VM_MSG_DW1_CODE		0x00FF0000 /* Message Code */
#define PCIE_VM_MSG_DW1_CODE_S		16
#define PCIE_VM_MSG_DW1_TAG		0xFF000000 /* Tag */
#define PCIE_VM_MSG_DW1_TAG_S		24

#define PCIE_VM_MSG_DW2		0x58

#define PCIE_VM_MSG_DW3		0x5C

/* Vendor Message Request Register */
#define PCIE_VM_MSG_REQR	0x60

#define PCIE_VM_MSG_REQR_REQ		0x00000001

/* AHB Slave Side Band Control Register */
#define PCIE_AHB_SSB		0x70

#define PCIE_AHB_SSB_REQ_BCM		0x00000001
#define PCIE_AHB_SSB_REQ_EP		0x00000002
#define PCIE_AHB_SSB_REQ_TD		0x00000004
#define PCIE_AHB_SSB_REQ_ATTR		0x00000018
#define PCIE_AHB_SSB_REQ_ATTR_S		3
#define PCIE_AHB_SSB_REQ_TC		0x000000E0
#define PCIE_AHB_SSB_REQ_TC_S		5

/* AHB Master SideBand Ctrl Register */
#define PCIE_AHB_MSB			0x74

#define PCIE_AHB_MSB_RESP_ATTR		0x00000003
#define PCIE_AHB_MSB_RESP_ATTR_S	0
#define PCIE_AHB_MSB_RESP_BAD_EOT	0x00000004
#define PCIE_AHB_MSB_RESP_BCM		0x00000008
#define PCIE_AHB_MSB_RESP_EP		0x00000010
#define PCIE_AHB_MSB_RESP_TD		0x00000020
#define PCIE_AHB_MSB_RESP_FUN_NUM	0x000003C0
#define PCIE_AHB_MSB_RESP_FUN_NUM_S	6

/* AHB Control Register, fixed bus enumeration exception
 * It happens to the same as PCIE_CLK_CTRL
 */
#define PCIE_AHB_CTRL			0x78

#define PCIE_AHB_CTRL_BUS_ERROR_SUPPRESS	0x00000001

/* PCIe 2.0 */
#define PCIE_DBG_EI			0x80
#define PCIE_DBG_INFO_L			0x84
#define PCIE_DBG_INFO_H			0x88

/* Interrupt Enalbe Register */
#define PCIE_IRNEN			0xF4

#define PCIE_IRNCR			0xF8

#define PCIE_IRNICR			0xFC

/* PCIe interrupt enable/control/capture register definition */
#define PCIE_IRN_AER_REPORT		0x00000001
#define PCIE_IRN_AER_MSIX		0x00000002
#define PCIE_IRN_PME			0x00000004
#define PCIE_IRN_HOTPLUG		0x00000008
#define PCIE_IRN_RX_VDM_MSG		0x00000010
#define PCIE_IRN_RX_CORRECTABLE_ERR_MSG	0x00000020
#define PCIE_IRN_RX_NON_FATAL_ERR_MSG	0x00000040
#define PCIE_IRN_RX_FATAL_ERR_MSG	0x00000080
#define PCIE_IRN_RX_PME_MSG		0x00000100
#define PCIE_IRN_RX_PME_TURNOFF_ACK	0x00000200
#define PCIE_IRN_AHB_BR_FATAL_ERR	0x00000400
#define PCIE_IRN_LINK_AUTO_BW_STATUS	0x00000800
#define PCIE_IRN_BW_MGT			0x00001000
#define PCIE_IRN_INTA			0x00002000 /* INTA */
#define PCIE_IRN_INTB			0x00004000 /* INTB */
#define PCIE_IRN_INTC			0x00008000 /* INTC */
#define PCIE_IRN_INTD			0x00010000 /* INTD */
#define PCIE_IRN_WAKEUP			0x00020000 /* Wake up Interrupt */
#define PCIE_IRN_LINK_BW_MGT		0x00080000 /* PCIe 2.0 BW status */
#define PCIE_IRN_LINK_AUTO_BW		0x00100000 /* PCIe 2.0 BW request */
#define PCIE_INTX_OFFSET		12

#define PCIE_RC_CORE_COMBINED_INT  (PCIE_IRN_AER_REPORT | PCIE_IRN_AER_MSIX \
		| PCIE_IRN_PME | PCIE_IRN_HOTPLUG | PCIE_IRN_RX_VDM_MSG \
		| PCIE_IRN_RX_CORRECTABLE_ERR_MSG \
		| PCIE_IRN_RX_NON_FATAL_ERR_MSG | PCIE_IRN_RX_FATAL_ERR_MSG \
		| PCIE_IRN_RX_PME_MSG | PCIE_IRN_RX_PME_TURNOFF_ACK \
		| PCIE_IRN_AHB_BR_FATAL_ERR | PCIE_IRN_LINK_AUTO_BW_STATUS\
		| PCIE_IRN_BW_MGT)

/* PCIe RC Configuration Register */
#define PCIE_VDID		0x00

/* Bit definition from pci_reg.h */
#define PCIE_PCICMDSTS		0x04
#define PCIE_CCRID		0x08

#define PCIE_CLSLTHTBR		0x0C

/* BAR0, BAR1,Only necessary if the bridges implements a device-specific
   register set or memory buffer */
#define PCIE_BAR0		0x10

#define PCIE_BAR1		0x14

#define PCIE_BNR		0x18
/* Bus Number Register bits */
#define PCIE_BNR_PRIMARY_BUS_NUM	0x000000FF
#define PCIE_BNR_PRIMARY_BUS_NUM_S	0
#define PCIE_PNR_SECONDARY_BUS_NUM	0x0000FF00
#define PCIE_PNR_SECONDARY_BUS_NUM_S	8
#define PCIE_PNR_SUB_BUS_NUM	0x00FF0000
#define PCIE_PNR_SUB_BUS_NUM_S	16

/* IO Base/Limit Register bits */
#define PCIE_IOBLSECS			0x1C

#define PCIE_IOBLSECS_32BIT_IO_ADDR	0x00000001
#define PCIE_IOBLSECS_IO_BASE_ADDR	0x000000F0
#define PCIE_IOBLSECS_IO_BASE_ADDR_S	4
#define PCIE_IOBLSECS_32BIT_IOLIMT	0x00000100
#define PCIE_IOBLSECS_IO_LIMIT_ADDR	0x0000F000
#define PCIE_IOBLSECS_IO_LIMIT_ADDR_S	12

/* Non-prefetchable Memory Base/Limit Register bit */
#define PCIE_MBML			0x20

#define PCIE_MBML_MEM_BASE_ADDR		0x0000FFF0
#define PCIE_MBML_MEM_BASE_ADDR_S	4
#define PCIE_MBML_MEM_LIMIT_ADDR	0xFFF00000
#define PCIE_MBML_MEM_LIMIT_ADDR_S	20

/* Prefetchable Memory Base/Limit Register bit */
#define PCIE_PMBL			0x24
#define PCIE_PMBL_64BIT_ADDR		0x00000001
#define PCIE_PMBL_UPPER_12BIT		0x0000FFF0
#define PCIE_PMBL_UPPER_12BIT_S		4
#define PCIE_PMBL_E64MA			0x00010000
#define PCIE_PMBL_END_ADDR		0xFFF00000
#define PCIE_PMBL_END_ADDR_S		20

#define PCIE_PMBU32			0x28

#define PCIE_PMLU32			0x2C

/* I/O Base/Limit Upper 16 bits register */
#define PCIE_IO_BANDL			0x30

#define PCIE_IO_BANDL_UPPER_16BIT_IO_BASE	0x0000FFFF
#define PCIE_IO_BANDL_UPPER_16BIT_IO_BASE_S	0
#define PCIE_IO_BANDL_UPPER_16BIT_IO_LIMIT	0xFFFF0000
#define PCIE_IO_BANDL_UPPER_16BIT_IO_LIMIT_S	16

#define PCIE_CPR			0x34

#define PCIE_EBBAR			0x38

/* Interrupt and Secondary Bridge Control Register */
#define PCIE_INTRBCTRL			0x3C

#define PCIE_INTRBCTRL_INT_LINE			0x000000FF
#define PCIE_INTRBCTRL_INT_LINE_S		0
#define PCIE_INTRBCTRL_INT_PIN			0x0000FF00
#define PCIE_INTRBCTRL_INT_PIN_S		8
#define PCIE_INTRBCTRL_PARITY_ERR_RESP_ENABLE	0x00010000
#define PCIE_INTRBCTRL_SERR_ENABLE		0x00020000
#define PCIE_INTRBCTRL_ISA_ENABLE		0x00040000
#define PCIE_INTRBCTRL_VGA_ENABLE		0x00080000
#define PCIE_INTRBCTRL_VGA_16BIT_DECODE		0x00100000
#define PCIE_INTRBCTRL_RST_SECONDARY_BUS	0x00400000
/* Others are read only */
enum {
	PCIE_INTRBCTRL_INT_NON = 0,
	PCIE_INTRBCTRL_INTA,
	PCIE_INTRBCTRL_INTB,
	PCIE_INTRBCTRL_INTC,
	PCIE_INTRBCTRL_INTD,
};

#define PCIE_PM_CAPR			0x40

/* Power Management Control and Status Register */
#define PCIE_PM_CSR			0x44

#define PCIE_PM_CSR_POWER_STATE		0x00000003 /* Power State */
#define PCIE_PM_CSR_POWER_STATE_S	0
#define PCIE_PM_CSR_SW_RST		0x00000008 /* Soft Reset Enabled */
#define PCIE_PM_CSR_PME_ENABLE		0x00000100 /* PME Enable */
#define PCIE_PM_CSR_PME_STATUS		0x00008000 /* PME status */

/* MSI Capability Register for EP */
#define PCIE_MCAPR			0x50

#define PCIE_MCAPR_MSI_CAP_ID		0x000000FF
#define PCIE_MCAPR_MSI_CAP_ID_S		0
#define PCIE_MCAPR_MSI_NEXT_CAP_PTR	0x0000FF00
#define PCIE_MCAPR_MSI_NEXT_CAP_PTR_S	8
#define PCIE_MCAPR_MSI_ENABLE		0x00010000
#define PCIE_MCAPR_MULTI_MSG_CAP	0x000E0000
#define PCIE_MCAPR_MULTI_MSG_CAP_S	17
#define PCIE_MCAPR_MULTI_MSG_ENABLE	0x00700000
#define PCIE_MCAPR_MULTI_MSG_ENABLE_S	20
#define PCIE_MCAPR_ADDR64_CAP		0X00800000

/* MSI Message Address Register */
#define PCIE_MA				0x54

#define PCIE_MA_ADDR_MASK		0xFFFFFFFC /* Message Address */

/* MSI Message Upper Address Register */
#define PCIE_MUA			0x58

/* MSI Message Data Register */
#define PCIE_MD				0x5C

#define PCIE_MD_DATA			0x0000FFFF /* Message Data */
#define PCIE_MD_DATA_S			0

/* PCI Express Capability Register */
#define PCIE_XCAP			0x70

#define PCIE_XCAP_ID			0x000000FF
#define PCIE_XCAP_ID_S			0
#define PCIE_XCAP_NEXT_CAP		0x0000FF00
#define PCIE_XCAP_NEXT_CAP_S		8
#define PCIE_XCAP_VER			0x000F0000
#define PCIE_XCAP_VER_S			16
#define PCIE_XCAP_DEV_PORT_TYPE		0x00F00000
#define PCIE_XCAP_DEV_PORT_TYPE_S	20
#define PCIE_XCAP_SLOT_IMPLEMENTED	0x01000000
#define PCIE_XCAP_MSG_INT_NUM		0x3E000000
#define PCIE_XCAP_MSG_INT_NUM_S		25

/* Device Capability Register */
#define PCIE_DCAP			0x74

#define PCIE_DCAP_MAX_PAYLOAD_SIZE	0x00000007
#define PCIE_DCAP_MAX_PAYLOAD_SIZE_S	0
#define PCIE_DCAP_PHANTOM_FUNC		0x00000018
#define PCIE_DCAP_PHANTOM_FUNC_S	3
#define PCIE_DCAP_EXT_TAG		0x00000020
#define PCIE_DCAP_EP_L0S_LATENCY	0x000001C0
#define PCIE_DCAP_EP_L0S_LATENCY_S	6
#define PCIE_DCAP_EP_L1_LATENCY		0x00000E00
#define PCIE_DCAP_EP_L1_LATENCY_S	9
#define PCIE_DCAP_ROLE_BASE_ERR_REPORT	0x00008000

/* Maximum payload size supported */
enum {
	PCIE_MAX_PAYLOAD_128 = 0,
	PCIE_MAX_PAYLOAD_256,
	PCIE_MAX_PAYLOAD_512,
	PCIE_MAX_PAYLOAD_1024,
	PCIE_MAX_PAYLOAD_2048,
	PCIE_MAX_PAYLOAD_4096,
};

/* Device Control and Status Register */
#define PCIE_DCTLSTS				0x78

#define PCIE_DCTLSTS_CORRECTABLE_ERR_EN		0x00000001
#define PCIE_DCTLSTS_NONFATAL_ERR_EN		0x00000002
#define PCIE_DCTLSTS_FATAL_ERR_EN		0x00000004
#define PCIE_DCTLSYS_UR_REQ_EN			0x00000008
#define PCIE_DCTLSTS_RELAXED_ORDERING_EN	0x00000010
#define PCIE_DCTLSTS_MAX_PAYLOAD_SIZE		0x000000E0
#define PCIE_DCTLSTS_MAX_PAYLOAD_SIZE_S		5
#define PCIE_DCTLSTS_EXT_TAG_EN			0x00000100
#define PCIE_DCTLSTS_PHANTOM_FUNC_EN		0x00000200
#define PCIE_DCTLSTS_AUX_PM_EN			0x00000400
#define PCIE_DCTLSTS_NO_SNOOP_EN		0x00000800
#define PCIE_DCTLSTS_MAX_READ_SIZE		0x00007000
#define PCIE_DCTLSTS_MAX_READ_SIZE_S		12
#define PCIE_DCTLSTS_CORRECTABLE_ERR		0x00010000
#define PCIE_DCTLSTS_NONFATAL_ERR		0x00020000
#define PCIE_DCTLSTS_FATAL_ER			0x00040000
#define PCIE_DCTLSTS_UNSUPPORTED_REQ		0x00080000
#define PCIE_DCTLSTS_AUX_POWER			0x00100000
#define PCIE_DCTLSTS_TRANSACT_PENDING	0x00200000

#define PCIE_DCTLSTS_ERR_EN	(PCIE_DCTLSTS_CORRECTABLE_ERR_EN | \
		PCIE_DCTLSTS_NONFATAL_ERR_EN | PCIE_DCTLSTS_FATAL_ERR_EN \
		| PCIE_DCTLSYS_UR_REQ_EN)

/* Link Capability Register */
#define PCIE_LCAP				0x7C
#define PCIE_LCAP_MAX_LINK_SPEED		0x0000000F
#define PCIE_LCAP_MAX_LINK_SPEED_S		0
#define PCIE_LCAP_MAX_LENGTH_WIDTH		0x000003F0
#define PCIE_LCAP_MAX_LENGTH_WIDTH_S		4
#define PCIE_LCAP_ASPM_LEVEL			0x00000C00
#define PCIE_LCAP_ASPM_LEVEL_S			10
#define PCIE_LCAP_L0S_EIXT_LATENCY		0x00007000
#define PCIE_LCAP_L0S_EIXT_LATENCY_S		12
#define PCIE_LCAP_L1_EXIT_LATENCY		0x00038000
#define PCIE_LCAP_L1_EXIT_LATENCY_S		15
#define PCIE_LCAP_CLK_PM			0x00040000
#define PCIE_LCAP_SDER				0x00080000
#define PCIE_LCAP_DLL_ACTIVE_REPROT		0x00100000
#define PCIE_LCAP_PORT_NUM			0xFF000000
#define PCIE_LCAP_PORT_NUM_S			24

/* Maximum Length width definition */
#define PCIE_MAX_LENGTH_WIDTH_RES		0x00
#define PCIE_MAX_LENGTH_WIDTH_X1		0x01 /* Default */
#define PCIE_MAX_LENGTH_WIDTH_X2		0x02
#define PCIE_MAX_LENGTH_WIDTH_X4		0x04
#define PCIE_MAX_LENGTH_WIDTH_X8		0x08
#define PCIE_MAX_LENGTH_WIDTH_X12		0x0C
#define PCIE_MAX_LENGTH_WIDTH_X16		0x10
#define PCIE_MAX_LENGTH_WIDTH_X32		0x20

/* Active State Link PM definition */
enum {
	PCIE_ASPM_RES0 = 0,
	PCIE_ASPM_L0S_ENTRY_SUPPORT, /* L0s */
	PCIE_ASPM_RES1,
	PCIE_ASPM_L0S_L1_ENTRY_SUPPORT, /* L0s and L1, default */
};

/* L0s Exit Latency definition */
enum {
	PCIE_L0S_EIXT_LATENCY_L64NS = 0, /* < 64 ns */
	PCIE_L0S_EIXT_LATENCY_B64A128,  /* > 64 ns < 128 ns */
	PCIE_L0S_EIXT_LATENCY_B128A256, /* > 128 ns < 256 ns */
	PCIE_L0S_EIXT_LATENCY_B256A512, /* > 256 ns < 512 ns */
	PCIE_L0S_EIXT_LATENCY_B512TO1U, /* > 512 ns < 1 us */
	PCIE_L0S_EIXT_LATENCY_B1A2U, /* > 1 us < 2 us */
	PCIE_L0S_EIXT_LATENCY_B2A4U, /* > 2 us < 4 us */
	PCIE_L0S_EIXT_LATENCY_M4US, /* > 4 us  */
};

/* L1 Exit Latency definition */
enum {
	PCIE_L1_EXIT_LATENCY_L1US = 0, /* < 1 us */
	PCIE_L1_EXIT_LATENCY_B1A2,     /* > 1 us < 2 us */
	PCIE_L1_EXIT_LATENCY_B2A4,     /* > 2 us < 4 us */
	PCIE_L1_EXIT_LATENCY_B4A8,     /* > 4 us < 8 us */
	PCIE_L1_EXIT_LATENCY_B8A16,    /* > 8 us < 16 us */
	PCIE_L1_EXIT_LATENCY_B16A32,   /* > 16 us < 32 us */
	PCIE_L1_EXIT_LATENCY_B32A64,   /* > 32 us < 64 us */
	PCIE_L1_EXIT_LATENCY_M64US,    /* > 64 us */
};

/* Link Control and Status Register */
#define PCIE_LCTLSTS				0x80
#define PCIE_LCTLSTS_ASPM_ENABLE		0x00000003
#define PCIE_LCTLSTS_ASPM_ENABLE_S		0
#define PCIE_LCTLSTS_RCB128			0x00000008
#define PCIE_LCTLSTS_LINK_DISABLE		0x00000010
#define PCIE_LCTLSTS_RETRIAN_LINK		0x00000020
#define PCIE_LCTLSTS_COM_CLK_CFG		0x00000040
#define PCIE_LCTLSTS_EXT_SYNC			0x00000080
#define PCIE_LCTLSTS_CLK_PM_EN			0x00000100
/* PCIe 2.0 */
#define PCIE_LCTLSTS_HW_AW_DIS			0x00000200
#define PCIE_LCTLSTS_LINK_ABM_IRQ_EN		0x00000400
#define PCIE_LCTLSTS_LINK_AB_IRQ_EN		0x00000800

#define PCIE_LCTLSTS_LINK_SPEED			0x000F0000
#define PCIE_LCTLSTS_LINK_SPEED_S		16
#define PCIE_LCTLSTS_NEGOTIATED_LINK_WIDTH	0x03F00000
#define PCIE_LCTLSTS_NEGOTIATED_LINK_WIDTH_S	20
#define PCIE_LCTLSTS_RETRAIN_PENDING		0x08000000
#define PCIE_LCTLSTS_SLOT_CLK_CFG		0x10000000
#define PCIE_LCTLSTS_DLL_ACTIVE			0x20000000
/* PCIe 2.0 */
#define PCIE_LCTLSTS_LINK_BMS			0x40000000
#define PCIE_LCTLSTS_LINK_ABS			0x80000000

/* Slot Capabilities Register */
#define PCIE_SLCAP				0x84

/* Slot Capabilities */
#define PCIE_SLCTLSTS				0x88

/* Root Control and Capability Register */
#define PCIE_RCTLCAP				0x8C

#define PCIE_RCTLCAP_SERR_ON_CORRECTABLE_ERR	0x00000001
#define PCIE_RCTLCAP_SERR_ON_NONFATAL_ERR	0x00000002
#define PCIE_RCTLCAP_SERR_ON_FATAL_ERR		0x00000004
#define PCIE_RCTLCAP_PME_INT_EN			0x00000008
#define PCIE_RCTLCAP_SERR_ENABLE	(PCIE_RCTLCAP_SERR_ON_CORRECTABLE_ERR \
		| PCIE_RCTLCAP_SERR_ON_NONFATAL_ERR \
		| PCIE_RCTLCAP_SERR_ON_FATAL_ERR)
/* Root Status Register */
#define PCIE_RSTS			0x90

#define PCIE_RSTS_PME_REQ_ID		0x0000FFFF
#define PCIE_RSTS_PME_REQ_ID_S		0
#define PCIE_RSTS_PME_STATUS		0x00010000
#define PCIE_RSTS_PME_PENDING		0x00020000

/* PCIe 2.0 specific stuff */
#define PCIE_DCAP2			0x94

#define PCIE_DCTLSTS2			0x98

#define PCIE_CPL_TIMEOUT_RANGE		0x0000000F
#define PCIE_CPL_TIMEOUT_RANGE_S	0
#define PCIE_CPL_TIMEOUT_DIS		0x00000010 /* Root Port optional */

/*
 * Range A: 50us to 10ms
 * Range B: 10ms to 250ms
 * Range C: 250ms to 4s
 * Range D: 4s to 64 s
*/
#define PCIE_CPL_TIMEOUT_RANGE_S	0
#define PCIE_CPL_TIMEOUT_RANGE_A	1
#define PCIE_CPL_TIMEOUT_RANGE_B	2
#define PCIE_CPL_TIMEOUT_RANGE_AB	3
#define PCIE_CPL_TIMEOUT_RANGE_BC	6
#define PCIE_CPL_TIMEOUT_RANGE_ABC	7
#define PCIE_CPL_TIMEOUT_RANGE_BCD	14
#define PCIE_CPL_TIMEOUT_RANGE_ABCD	15

#define PCIE_LCAP2			0x9C
#define PCIE_LCTLSTS2			0xA0

#define PCIE_LCTLSTS2_TGT_LINK_SPEED	0x0000000F
#define PCIE_LCTLSTS2_TGT_LINK_SPEED_S	0
#define PCIE_LCTLSTS2_TGT_LINK_SPEED_25GT	1
#define PCIE_LCTLSTS2_TGT_LINK_SPEED_5GT	2
#define PCIE_LCTLSTS2_ENTER_COMPLIACE	0x00000010
#define PCIE_LCTLSTS2_HW_AUTO_DIS	0x00000020
#define PCIE_LCTLSTS2_DE_EMPHASIS_35DB	0x00000040 /* 0 -6DB */
#define PCIE_LCTLSTS2_TX_MARGIN		0x00000380
#define PCIE_LCTLSTS2_TX_MARGIN_S	7

#define PCIE_LCTLSTS2_MF_COMP_ENTER	0x00000400 /* LTSSM substate */

#define PCIE_LCTLSTS2_COMPLIANCE_SOS	0x00000800
#define PCIE_LCTLSTS2_COMP_DE_EM35DB	0x00001000

#define PCIE_LCTLSTS2_CUR_DE_EM35DB	0x00010000



/* PCI Express Enhanced Capability Header */
#define PCIE_ENHANCED_CAP		0x100

#define PCIE_ENHANCED_CAP_ID			0x0000FFFF
#define PCIE_ENHANCED_CAP_ID_S			0
#define PCIE_ENHANCED_CAP_VER			0x000F0000
#define PCIE_ENHANCED_CAP_VER_S			16
#define PCIE_ENHANCED_CAP_NEXT_OFFSET		0xFFF00000
#define PCIE_ENHANCED_CAP_NEXT_OFFSET_S		20

/* Uncorrectable Error Status Register */
#define PCIE_UES_R			0x104

#define PCIE_DATA_LINK_PROTOCOL_ERR		0x00000010
#define PCIE_SURPRISE_DOWN_ERROR		0x00000020
#define PCIE_POISONED_TLP			0x00001000
#define PCIE_FC_PROTOCOL_ERR			0x00002000
#define PCIE_COMPLETION_TIMEOUT			0x00004000
#define PCIE_COMPLETOR_ABORT			0x00008000
#define PCIE_UNEXPECTED_COMPLETION		0x00010000
#define PCIE_RECEIVER_OVERFLOW			0x00020000
#define PCIE_MALFORNED_TLP			0x00040000
#define PCIE_ECRC_ERR				0x00080000
#define PCIE_UR_REQ				0x00100000
#define PCIE_ALL_UNCORRECTABLE_ERR	(PCIE_DATA_LINK_PROTOCOL_ERR |\
		PCIE_SURPRISE_DOWN_ERROR | PCIE_POISONED_TLP |\
		PCIE_FC_PROTOCOL_ERR | PCIE_COMPLETION_TIMEOUT | \
		PCIE_COMPLETOR_ABORT | PCIE_UNEXPECTED_COMPLETION |\
		PCIE_RECEIVER_OVERFLOW | PCIE_MALFORNED_TLP | \
		PCIE_ECRC_ERR | PCIE_UR_REQ)

/* Uncorrectable Error Mask Register, Mask means no report */
#define PCIE_UEMR			0x108

/* Uncorrectable Error Severity Register */
#define PCIE_UESR			0x10C

/* Correctable Error Status Register */
#define PCIE_CESR			0x110
#define PCIE_RX_ERR			0x00000001
#define PCIE_BAD_TLP			0x00000040
#define PCIE_BAD_DLLP			0x00000080
#define PCIE_REPLAY_NUM_ROLLOVER	0x00000100
#define PCIE_REPLAY_TIMER_TIMEOUT_ERR	0x00001000
#define PCIE_ADVISORY_NONFTAL_ERR	0x00002000
#define PCIE_CORRECTABLE_ERR	(PCIE_RX_ERR | PCIE_BAD_TLP | PCIE_BAD_DLLP \
		| PCIE_REPLAY_NUM_ROLLOVER | PCIE_REPLAY_TIMER_TIMEOUT_ERR\
		| PCIE_ADVISORY_NONFTAL_ERR)

/* Correctable Error Mask Register */
#define PCIE_CEMR			0x114

/* Advanced Error Capabilities and Control Register */
#define PCIE_AECCR			0x118

#define PCIE_AECCR_FIRST_ERR_PTR		0x0000001F
#define PCIE_AECCR_FIRST_ERR_PTR_S		0
#define PCIE_AECCR_ECRC_GEN_CAP			0x00000020
#define PCIE_AECCR_ECRC_GEN_EN			0x00000040
#define PCIE_AECCR_ECRC_CHECK_CAP		0x00000080
#define PCIE_AECCR_ECRC_CHECK_EN		0x00000100

/* Header Log Register 1 */
#define PCIE_HLR1			0x11C

/* Header Log Register 2 */
#define PCIE_HLR2			0x120

/* Header Log Register 3 */
#define PCIE_HLR3			0x124

/* Header Log Register 4 */
#define PCIE_HLR4			0x128

/* Root Error Command Register */
#define PCIE_RECR			0x12C

#define PCIE_RECR_CORRECTABLE_ERR_REPORT_EN	0x00000001 /* COR-ERR */
#define PCIE_RECR_NONFATAL_ERR_REPORT_EN	0x00000002 /* Non-Fatal ERR */
#define PCIE_RECR_FATAL_ERR_REPORT_EN		0x00000004 /* Fatal ERR */
#define PCIE_RECR_ERR_REPORT_EN	(PCIE_RECR_CORRECTABLE_ERR_REPORT_EN\
		| PCIE_RECR_NONFATAL_ERR_REPORT_EN |\
		PCIE_RECR_FATAL_ERR_REPORT_EN)

/* Root Error Status Register */
#define PCIE_RESR			0x130

#define PCIE_RESR_CORRECTABLE_ERR		0x00000001
#define PCIE_RESR_MULTI_CORRECTABLE_ERR		0x00000002
#define PCIE_RESR_FATAL_NOFATAL_ERR		0x00000004
#define PCIE_RESR_MULTI_FATAL_NOFATAL_ERR	0x00000008
#define PCIE_RESR_FIRST_UNCORRECTABLE_FATAL_ERR	0x00000010
#define PCIR_RESR_NON_FATAL_ERR			0x00000020
#define PCIE_RESR_FATAL_ERR			0x00000040
#define PCIE_RESR_AER_INT_MSG_NUM		0xF8000000
#define PCIE_RESR_AER_INT_MSG_NUM_S		27

/* Error Source Indentification Register */
#define PCIE_ESIR			0x134

#define PCIE_ESIR_CORRECTABLE_ERR_SRC_ID	0x0000FFFF
#define PCIE_ESIR_CORRECTABLE_ERR_SRC_ID_S	0
#define PCIE_ESIR_FATAL_NON_FATAL_SRC_ID	0xFFFF0000
#define PCIE_ESIR_FATAL_NON_FATAL_SRC_ID_S	16

/* VC Enhanced Capability Header */
#define PCIE_VC_ECH			0x140

/* Port VC Capability Register */
#define PCIE_PVC1			0x144

#define PCIE_PVC1_EXT_VC_CNT			0x00000007
#define PCIE_PVC1_EXT_VC_CNT_S			0
#define PCIE_PVC1_LOW_PRI_EXT_VC_CNT		0x00000070
#define PCIE_PVC1_LOW_PRI_EXT_VC_CNT_S		4
#define PCIE_PVC1_REF_CLK			0x00000300
#define PCIE_PVC1_REF_CLK_S			8
#define PCIE_PVC1_PORT_ARB_TAB_ENTRY_SIZE	0x00000C00
#define PCIE_PVC1_PORT_ARB_TAB_ENTRY_SIZE_S	10

/* Extended Virtual Channel Count Defintion */
#define PCIE_EXT_VC_CNT_MIN		0
#define PCIE_EXT_VC_CNT_MAX		7

/* Port Arbitration Table Entry Size Definition */
enum {
	PCIE_PORT_ARB_TAB_ENTRY_SIZE_S1BIT = 0,
	PCIE_PORT_ARB_TAB_ENTRY_SIZE_S2BIT,
	PCIE_PORT_ARB_TAB_ENTRY_SIZE_S4BIT,
	PCIE_PORT_ARB_TAB_ENTRY_SIZE_S8BIT,
};

/* Port VC Capability Register 2 */
#define PCIE_PVC2			0x148

#define PCIE_PVC2_VC_ARB_16P_FIXED_WRR		0x00000001
#define PCIE_PVC2_VC_ARB_32P_WRR		0x00000002
#define PCIE_PVC2_VC_ARB_64P_WRR		0x00000004
#define PCIE_PVC2_VC_ARB_128P_WRR		0x00000008
#define PCIE_PVC2_VC_ARB_WRR			0x0000000F
#define PCIE_PVC2_VC_ARB_TAB_OFFSET		0xFF000000
#define PCIE_PVC2_VC_ARB_TAB_OFFSET_S		24

/* Port VC Control and Status Register */
#define PCIE_PVCCRSR			0x14C

#define PCIE_PVCCRSR_LOAD_VC_ARB_TAB		0x00000001
#define PCIE_PVCCRSR_VC_ARB_SEL			0x0000000E
#define PCIE_PVCCRSR_VC_ARB_SEL_S		1
#define PCIE_PVCCRSR_VC_ARB_TAB_STATUS		0x00010000

/* VC0 Resource Capability Register */
#define PCIE_VC0_RC			0x150

#define PCIE_VC0_RC_PORT_ARB_HW_FIXED		0x00000001
#define PCIE_VC0_RC_PORT_ARB_32P_WRR		0x00000002
#define PCIE_VC0_RC_PORT_ARB_64P_WRR		0x00000004
#define PCIE_VC0_RC_PORT_ARB_128P_WRR		0x00000008
#define PCIE_VC0_RC_PORT_ARB_TM_128P_WRR	0x00000010
#define PCIE_VC0_RC_PORT_ARB_TM_256P_WRR	0x00000020
#define PCIE_VC0_RC_PORT_ARB	(PCIE_VC0_RC_PORT_ARB_HW_FIXED |\
		PCIE_VC0_RC_PORT_ARB_32P_WRR | PCIE_VC0_RC_PORT_ARB_64P_WRR |\
		PCIE_VC0_RC_PORT_ARB_128P_WRR |\
		PCIE_VC0_RC_PORT_ARB_TM_128P_WRR |\
		PCIE_VC0_RC_PORT_ARB_TM_256P_WRR)

#define PCIE_VC0_RC_REJECT_SNOOP		0x00008000
#define PCIE_VC0_RC_MAX_TIMESLOTS		0x007F0000
#define PCIE_VC0_RC_MAX_TIMESLOTS_S		16
#define PCIE_VC0_RC_PORT_ARB_TAB_OFFSET		0xFF000000
#define PCIE_VC0_RC_PORT_ARB_TAB_OFFSET_S	24

/* VC0 Resource Control Register */
#define PCIE_VC0_RC0			0x154

#define PCIE_VC0_RC0_TVM0			0x00000001
#define PCIE_VC0_RC0_TVM1			0x00000002
#define PCIE_VC0_RC0_TVM2			0x00000004
#define PCIE_VC0_RC0_TVM3			0x00000008
#define PCIE_VC0_RC0_TVM4			0x00000010
#define PCIE_VC0_RC0_TVM5			0x00000020
#define PCIE_VC0_RC0_TVM6			0x00000040
#define PCIE_VC0_RC0_TVM7			0x00000080
#define PCIE_VC0_RC0_TC_VC			0x000000FF

#define PCIE_VC0_RC0_LOAD_PORT_ARB_TAB		0x00010000
#define PCIE_VC0_RC0_PORT_ARB_SEL		0x000E0000
#define PCIE_VC0_RC0_PORT_ARB_SEL_S		17
#define PCIE_VC0_RC0_VC_ID			0x07000000
#define PCIE_VC0_RC0_VC_ID_S			24
#define PCIE_VC0_RC0_VC_EN			0x80000000

/* VC0 Resource Status Register */
#define PCIE_VC0_RSR0			0x158

#define PCIE_VC0_RSR0_PORT_ARB_TAB_STATUS	0x00010000
#define PCIE_VC0_RSR0_VC_NEG_PENDING		0x00020000

/* Ack Latency Timer and Replay Timer Register */
#define PCIE_ALTRT			0x700

#define PCIE_ALTRT_ROUND_TRIP_LATENCY_LIMIT	0x0000FFFF
#define PCIE_ALTRT_ROUND_TRIP_LATENCY_LIMIT_S	0
#define PCIE_ALTRT_REPLAY_TIME_LIMIT		0xFFFF0000
#define PCIE_ALTRT_REPLAY_TIME_LIMIT_S		16

/* Other Message Register */
#define PCIE_OMR			0x704

/* Port Force Link Register */
#define PCIE_PFLR			0x708

#define PCIE_PFLR_LINK_NUM			0x000000FF
#define PCIE_PFLR_LINK_NUM_S			0
#define PCIE_PFLR_FORCE_LINK			0x00008000
#define PCIE_PFLR_LINK_STATE			0x003F0000
#define PCIE_PFLR_LINK_STATE_S			16
#define PCIE_PFLR_LOW_POWER_ENTRY_CNT		0xFF000000
#define PCIE_PFLR_LOW_POWER_ENTRY_CNT_S		24

/* Ack Frequency Register */
#define PCIE_AFR			0x70C

#define PCIE_AFR_AF			0x000000FF
#define PCIE_AFR_AF_S			0
#define PCIE_AFR_FTS_NUM		0x0000FF00
#define PCIE_AFR_FTS_NUM_S		8
#define PCIE_AFR_COM_FTS_NUM		0x00FF0000
#define PCIE_AFR_COM_FTS_NUM_S		16
#define PCIE_AFR_L0S_ENTRY_LATENCY	0x07000000
#define PCIE_AFR_L0S_ENTRY_LATENCY_S	24
#define PCIE_AFR_L1_ENTRY_LATENCY	0x38000000
#define PCIE_AFR_L1_ENTRY_LATENCY_S	27
#define PCIE_AFR_FTS_NUM_DEFAULT	127
#define PCIE_AFR_L0S_ENTRY_LATENCY_DEFAULT	7
#define PCIE_AFR_L1_ENTRY_LATENCY_DEFAULT	5

/* Port Link Control Register */
#define PCIE_PLCR			0x710

#define PCIE_PLCR_OTHER_MSG_REQ		0x00000001
#define PCIE_PLCR_SCRAMBLE_DISABLE	0x00000002
#define PCIE_PLCR_LOOPBACK_EN		0x00000004
#define PCIE_PLCR_LTSSM_HOT_RST		0x00000008
#define PCIE_PLCR_DLL_LINK_EN		0x00000020
#define PCIE_PLCR_FAST_LINK_SIM_EN	0x00000080
#define PCIE_PLCR_LINK_MODE		0x003F0000
#define PCIE_PLCR_LINK_MODE_S		16
#define PCIE_PLCR_CORRUPTED_CRC_EN	0x02000000

/* Lane Skew Register */
#define PCIE_LSR			0x714

#define PCIE_LSR_LANE_SKEW_NUM		0x00FFFFFF
#define PCIE_LSR_LANE_SKEW_NUM_S	0
#define PCIE_LSR_FC_DISABLE		0x01000000
#define PCIE_LSR_ACKNAK_DISABLE		0x02000000
#define PCIE_LSR_LANE_DESKEW_DISABLE	0x80000000

/* Symbol Number Register */
#define PCIE_SNR			0x718

#define PCIE_SNR_TS			0x0000000F
#define PCIE_SNR_TS_S			0
#define PCIE_SNR_SKP			0x00000700
#define PCIE_SNR_SKP_S			8
#define PCIE_SNR_REPLAY_TIMER		0x0007C000
#define PCIE_SNR_REPLAY_TIMER_S		14
#define PCIE_SNR_ACKNAK_LATENCY_TIMER	0x00F80000
#define PCIE_SNR_ACKNAK_LATENCY_TIMER_S	19
#define PCIE_SNR_FC_TIMER		0x1F000000
#define PCIE_SNR_FC_TIMER_S		28

/* Symbol Timer Register and Filter Mask Register 1 */
#define PCIE_STRFMR			0x71C

#define PCIE_STRFMR_SKP_INTERVAL		0x000007FF
#define PCIE_STRFMR_SKP_INTERVAL_S		0
#define PCIE_STRFMR_FC_WDT_DISABLE		0x00008000
#define PCIE_STRFMR_TLP_FUNC_MISMATCH_OK	0x00010000
#define PCIE_STRFMR_POISONED_TLP_OK		0x00020000
#define PCIE_STRFMR_BAR_MATCH_OK		0x00040000
#define PCIE_STRFMR_TYPE1_CFG_REQ_OK		0x00080000
#define PCIE_STRFMR_LOCKED_REQ_OK		0x00100000
#define PCIE_STRFMR_CPL_TAG_ERR_RULES_OK	0x00200000
#define PCIE_STRFMR_CPL_REQUESTOR_ID_MISMATCH_OK	0x00400000
#define PCIE_STRFMR_CPL_FUNC_MISMATCH_OK	0x00800000
#define PCIE_STRFMR_CPL_TC_MISMATCH_OK		0x01000000
#define PCIE_STRFMR_CPL_ATTR_MISMATCH_OK	0x02000000
#define PCIE_STRFMR_CPL_LENGTH_MISMATCH_OK	0x04000000
#define PCIE_STRFMR_TLP_ECRC_ERR_OK		0x08000000
#define PCIE_STRFMR_CPL_TLP_ECRC_OK		0x10000000
#define PCIE_STRFMR_RX_TLP_MSG_NO_DROP		0x20000000
#define PCIE_STRFMR_RX_IO_TRANS_ENABLE		0x40000000
#define PCIE_STRFMR_RX_CFG_TRANS_ENABLE		0x80000000

#define PCIE_DEF_SKP_INTERVAL	700 /* 1180 ~1538 , 125MHz * 2, 250MHz * 1 */

/* Filter Masker Register 2 */
#define PCIE_FMR2			0x720

#define PCIE_FMR2_VENDOR_MSG0_PASSED_TO_TRGT1	0x00000001
#define PCIE_FMR2_VENDOR_MSG1_PASSED_TO_TRGT1	0x00000002

/* PCIe 2.0 AXI */
#define PCIE_AMBA_MUTLI_REQ		0x724

/* Debug Register 0 */
#define PCIE_DBR0			0x728

/* Debug Register 1 */
#define PCIE_DBR1			0x72C

/* Transmit Posted FC Credit Status Register */
#define PCIE_TPFCS			0x730

#define PCIE_TPFCS_TX_P_DATA_FC_CREDITS		0x00000FFF
#define PCIE_TPFCS_TX_P_DATA_FC_CREDITS_S	0
#define PCIE_TPFCS_TX_P_HDR_FC_CREDITS		0x000FF000
#define PCIE_TPFCS_TX_P_HDR_FC_CREDITS_S	12

/* Transmit Non-Posted FC Credit Status */
#define PCIE_TNPFCS			0x734

#define PCIE_TNPFCS_TX_NP_DATA_FC_CREDITS	0x00000FFF
#define PCIE_TNPFCS_TX_NP_DATA_FC_CREDITS_S	0
#define PCIE_TNPFCS_TX_NP_HDR_FC_CREDITS	0x000FF000
#define PCIE_TNPFCS_TX_NP_HDR_FC_CREDITS_S	12

/* Transmit Complete FC Credit Status Register */
#define PCIE_TCFCS			0x738

#define PCIE_TCFCS_TX_CPL_DATA_FC_CREDITS	0x00000FFF
#define PCIE_TCFCS_TX_CPL_DATA_FC_CREDITS_S	0
#define PCIE_TCFCS_TX_CPL_HDR_FC_CREDITS	0x000FF000
#define PCIE_TCFCS_TX_CPL_HDR_FC_CREDITS_S	12

/* Queue Status Register */
#define PCIE_QSR			0x73C

#define PCIE_QSR_WAIT_UPDATE_FC_DLL		0x00000001
#define PCIE_QSR_TX_RETRY_BUF_NOT_EMPTY		0x00000002
#define PCIE_QSR_RX_QUEUE_NOT_EMPTY		0x00000004

/* VC Transmit Arbitration Register 1 */
#define PCIE_VCTAR1			0x740

#define PCIE_VCTAR1_WRR_WEIGHT_VC0		0x000000FF
#define PCIE_VCTAR1_WRR_WEIGHT_VC1		0x0000FF00
#define PCIE_VCTAR1_WRR_WEIGHT_VC2		0x00FF0000
#define PCIE_VCTAR1_WRR_WEIGHT_VC3		0xFF000000

/* VC Transmit Arbitration Register 2 */
#define PCIE_VCTAR2			0x744

#define PCIE_VCTAR2_WRR_WEIGHT_VC4		0x000000FF
#define PCIE_VCTAR2_WRR_WEIGHT_VC5		0x0000FF00
#define PCIE_VCTAR2_WRR_WEIGHT_VC6		0x00FF0000
#define PCIE_VCTAR2_WRR_WEIGHT_VC7		0xFF000000

/* VC0 Posted Receive Queue Control Register */
#define PCIE_VC0_PRQCR			0x748

#define PCIE_VC0_PRQCR_P_DATA_CREDITS		0x00000FFF
#define PCIE_VC0_PRQCR_P_DATA_CREDITS_S		0
#define PCIE_VC0_PRQCR_P_HDR_CREDITS		0x000FF000
#define PCIE_VC0_PRQCR_P_HDR_CREDITS_S		12
#define PCIE_VC0_PRQCR_P_TLP_QUEUE_MODE		0x00E00000
#define PCIE_VC0_PRQCR_P_TLP_QUEUE_MODE_S	20
#define PCIE_VC0_PRQCR_TLP_RELAX_ORDER		0x40000000
#define PCIE_VC0_PRQCR_VC_STRICT_ORDER		0x80000000

/* VC0 Non-Posted Receive Queue Control */
#define PCIE_VC0_NPRQCR			0x74C

#define PCIE_VC0_NPRQCR_NP_DATA_CREDITS		0x00000FFF
#define PCIE_VC0_NPRQCR_NP_DATA_CREDITS_S	0
#define PCIE_VC0_NPRQCR_NP_HDR_CREDITS		0x000FF000
#define PCIE_VC0_NPRQCR_NP_HDR_CREDITS_S	12
#define PCIE_VC0_NPRQCR_NP_TLP_QUEUE_MODE	0x00E00000
#define PCIE_VC0_NPRQCR_NP_TLP_QUEUE_MODE_S	20

/* VC0 Completion Receive Queue Control */
#define PCIE_VC0_CRQCR			0x750

#define PCIE_VC0_CRQCR_CPL_DATA_CREDITS		0x00000FFF
#define PCIE_VC0_CRQCR_CPL_DATA_CREDITS_S	0
#define PCIE_VC0_CRQCR_CPL_HDR_CREDITS		0x000FF000
#define PCIE_VC0_CRQCR_CPL_HDR_CREDITS_S	12
#define PCIE_VC0_CRQCR_CPL_TLP_QUEUE_MODE	0x00E00000
#define PCIE_VC0_CRQCR_CPL_TLP_QUEUE_MODE_S	21

/* Applicable to the above three registers */
enum {
	PCIE_VC0_TLP_QUEUE_MODE_STORE_FORWARD = 1,
	PCIE_VC0_TLP_QUEUE_MODE_CUT_THROUGH = 2,
	PCIE_VC0_TLP_QUEUE_MODE_BYPASS = 4,
};

/* VC0 Posted Buffer Depth Register */
#define PCIE_VC0_PBD			0x7A8

#define PCIE_VC0_PBD_P_DATA_QUEUE_ENTRIES	0x00003FFF
#define PCIE_VC0_PBD_P_DATA_QUEUE_ENTRIES_S	0
#define PCIE_VC0_PBD_P_HDR_QUEUE_ENTRIES	0x03FF0000
#define PCIE_VC0_PBD_P_HDR_QUEUE_ENTRIES_S	16

/* VC0 Non-Posted Buffer Depth Register */
#define PCIE_VC0_NPBD			0x7AC

#define PCIE_VC0_NPBD_NP_DATA_QUEUE_ENTRIES	0x00003FFF
#define PCIE_VC0_NPBD_NP_DATA_QUEUE_ENTRIES_S	0
#define PCIE_VC0_NPBD_NP_HDR_QUEUE_ENTRIES	0x03FF0000
#define PCIE_VC0_NPBD_NP_HDR_QUEUE_ENTRIES_S	16

/* VC0 Completion Buffer Depth Register */
#define PCIE_VC0_CBD			0x7B0

#define PCIE_VC0_CBD_CPL_DATA_QUEUE_ENTRIES	0x00003FFF
#define PCIE_VC0_CBD_CPL_DATA_QUEUE_ENTRIES_S	0
#define PCIE_VC0_CBD_CPL_HDR_QUEUE_ENTRIES	0x03FF0000
#define PCIE_VC0_CBD_CPL_HDR_QUEUE_ENTRIES_S	16

/* PCIE 2.0 important configuration register */
#define PCIE_GEN2_CTRL			0x80C
#define PCIE_GEN2_CTRL_FTS		0x000000FF
#define PCIE_GEN2_CTRL_FTS_S		0
#define PCIE_GEN2_DEFAULT_FTS_NUM	127
#define PCIE_GEN2_CTRL_LANES_NR		0x0001FF00
#define PCIE_GEN2_CTRL_LANES_NR_S	8
#define PCIE_GEN2_CTRL_SPEED_CHANGE	BIT(17)
#define PCIE_GEN2_CTRL_TX_PHY_CHANGE	BIT(18)
#define PCIE_GEN2_CTRL_TX_COMP_RX	BIT(19)
#define PCIE_GEN2_CTRL_SEL_DEEMP_LOW	BIT(20)
#define PCIE_GEN2_CTRL_GEN1_EI_INFER	BIT(21)

/* PHY Status Register,*/
#define PCIE_PHYSR			0x810

/* PHY Control Register */
#define PCIE_PHYCR			0x814

/*
 * legacy PCIe PDI PHY register definition, suppose all the following
 * stuff is confidential.
 * XXX, detailed bit definition
 */
#define	PCIE_PHY_PLL_CTRL1		(0x22 << 1)
#define	PCIE_PHY_PLL_CTRL2		(0x23 << 1)
#define	PCIE_PHY_PLL_CTRL3		(0x24 << 1)
#define	PCIE_PHY_PLL_CTRL4		(0x25 << 1)
#define	PCIE_PHY_PLL_CTRL5		(0x26 << 1)
#define	PCIE_PHY_PLL_CTRL6		(0x27 << 1)
#define	PCIE_PHY_PLL_CTRL7		(0x28 << 1)
#define	PCIE_PHY_PLL_A_CTRL1		(0x29 << 1)
#define	PCIE_PHY_PLL_A_CTRL2		(0x2A << 1)
#define	PCIE_PHY_PLL_A_CTRL3		(0x2B << 1)
#define	PCIE_PHY_PLL_STATUS		(0x2C << 1)

#define PCIE_PHY_TX1_CTRL1		(0x30 << 1)
#define PCIE_PHY_TX1_CTRL2		(0x31 << 1)
#define PCIE_PHY_TX1_CTRL3		(0x32 << 1)
#define PCIE_PHY_TX1_A_CTRL1		(0x33 << 1)
#define PCIE_PHY_TX1_A_CTRL2		(0x34 << 1)
#define PCIE_PHY_TX1_MOD1		(0x35 << 1)
#define PCIE_PHY_TX1_MOD2		(0x36 << 1)
#define PCIE_PHY_TX1_MOD3		(0x37 << 1)

#define PCIE_PHY_TX2_CTRL1		(0x38 << 1)
#define PCIE_PHY_TX2_CTRL2		(0x39 << 1)
#define PCIE_PHY_TX2_A_CTRL1		(0x3B << 1)
#define PCIE_PHY_TX2_A_CTRL2		(0x3C << 1)
#define PCIE_PHY_TX2_MOD1		(0x3D << 1)
#define PCIE_PHY_TX2_MOD2		(0x3E << 1)
#define PCIE_PHY_TX2_MOD3		(0x3F << 1)

#define PCIE_PHY_RX1_CTRL1		(0x50 << 1)
#define PCIE_PHY_RX1_CTRL2		(0x51 << 1)
#define PCIE_PHY_RX1_CDR		(0x52 << 1)
#define PCIE_PHY_RX1_EI			(0x53 << 1)
#define PCIE_PHY_RX1_A_CTRL		(0x55 << 1)

/* PCIe 2.0 PDI PHY register definition */
#define PCIE_PHY_CFG0			0xc004
#define PCIE_PHY_CFG1			0xc008
#define PCIE_PHY_CFG2			0xc00c
#define PCIE_PHY_CFG3			0xc010
#define PCIE_PHY_CFG4			0xc014

/* PCIe PHY CFG0 */
#define PCIE_PHY_RX_EQ			0x00000007
#define PCIE_PHY_RX_EQ_S		0

#define PCIE_PHY_TX_DE_6DB		0x000001F8
#define PCIE_PHY_TX_DE_6DB_S		3

#define PCIE_PHY_TX_SWING_LOW		0x0000FE00
#define PCIE_PHY_TX_SWING_LOW_S		9

/* PCIe PHY CFG1 */
#define PCIE_PHY_TX_DE_3DB		0x0000003F
#define PCIE_PHY_TX_DE_3DB_S		0

#define PCIE_PHY_TX_TERM		0x000007C0
#define PCIE_PHY_TX_TERM_S		6

/* PCIe PHY CFG2 */
#define PCIE_PHY_TX_DE_GEN1		0x0000003F
#define PCIE_PHY_TX_DE_GEN1_S		0

#define PCIE_PHY_TX_SWING_FULL		0x00001FC0
#define PCIE_PHY_TX_SWING_FULL_S	6

/* PCIe PHY CFG3 */
#define PCIE_PHY_PIPE_EN		BIT(0)
#define PCIE_PHY_CM_CLK_SEL		BIT(1)
#define PCIE_PHY_MPLL_M			0x00003F80
#define PCIE_PHY_MPLL_M_S		7
#define PCIE_PHY_REF_CLK_DIV2		0x0000C000
#define PCIE_PHY_REF_CLK_DIV2_S		14

/* PCIe PHY CFG4 */
#define PCIE_PHY_REF_CLK_PAD		BIT(0)
#define PCIE_PHY_LOS_BIAS		0x0000000E
#define PCIE_PHY_LOS_BIAS_S		1
#define PCIE_PHY_LOS_LEVEL		0x000001F0
#define PCIE_PHY_IOS_LEVEL_S		4
#define PCIE_PHY_TX_VBOOST		0x00000E00
#define PCIE_PHY_TX_VBOOST_S		9
#define PCIE_PHY_VREG_BYPASS		BIT(12)
#define PCIE_PHY_PIPE_PD		BIT(14)
#define PCIE_PHY_PIPE_PD_O		BIT(15)

/* xRX500 LCPLL SSC */
#define PCIE_LCPLL_CFG0		0x0094
#define LCPLL_CFG0_LOCKED	BIT(1)
#define PCIE_LCPLL_CFG1		0x0098
#define PCIE_LCPLL_SSC_CTRL	0x009C
#define PCIE_LCPLL_SSC_SCALE	0x00A0
#define PCIE_LCPLL_COEF_0	0x00A4
#define PCIE_LCPLL_COEF_1	0x00A8
#define PCIE_LCPLL_COEF_2	0x00AC
#define PCIE_LCPLL_COEF_3	0x00B0
#define PCIE_LCPLL_COEF_4	0x00B4
#define PCIE_LCPLL_COEF_5	0x00B8
#define PCIE_LCPLL_COEF_6	0x00BC
#define PCIE_LCPLL_COEF_7	0x00C0

#ifdef CONFIG_PCI_MSI
extern struct msi_chip ltq_msi_chip;
#endif

static inline int pcie_ep_rst_init(struct ltq_pcie_port *lpp)
{
	struct device *dev = lpp->dev;
	struct device_node *np = dev->of_node;

	lpp->reset_gpio = of_get_named_gpio(np, "reset-gpio", 0);
	dev_dbg(lpp->dev, "gpio %d\n", lpp->reset_gpio);
	if (gpio_is_valid(lpp->reset_gpio)) {
		int ret = devm_gpio_request(lpp->dev,
				lpp->reset_gpio, "pcie-reset");
		if (ret) {
			dev_err(lpp->dev, "%s failed to request gpio %d\n",
				__func__, lpp->reset_gpio);
			return -EINVAL;
		}
		gpio_direction_output(lpp->reset_gpio, 1);
	}

	if (lpp->high_active_rst)
		gpio_set_value(lpp->reset_gpio, 0);
	else
		gpio_set_value(lpp->reset_gpio, 1);

	mdelay(100);
	return 0;
}

static inline int pcie_ahb_pmu_setup(struct ltq_pcie_port *lpp)
{
	int err;
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	if (soc->has_ahb_clk) {
		err = clk_prepare_enable(lpp->ahb_clk);
		if (err < 0) {
			dev_err(lpp->dev,
				"failed to enable ahb clock: %d\n", err);
			return err;
		}
	}
	return 0;
}

static inline void pcie_rcu_endian_setup(struct ltq_pcie_port *lpp)
{
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;
#ifdef CONFIG_CPU_BIG_ENDIAN
	if (soc->has_ahb2m_endian)
		ltq_endian_clr(soc->ahb2m_endian_ctrl_shift);

	if (soc->has_ahb4s_endian)
		ltq_endian_set(soc->ahb4s_endian_ctrl_shift);

	if (soc->has_ahb3m_endian)
		ltq_endian_clr(soc->ahb3m_endian_ctrl_shift);

	if (lpp->outbound_swap)
		ltq_endian_set(lpp->outbound_shift);
	else
		ltq_endian_clr(lpp->outbound_shift);
	if (lpp->inbound_swap)
		ltq_endian_set(lpp->inbound_shift);
	else
		ltq_endian_clr(lpp->inbound_shift);
#else
	if (soc->has_ahb2m_endian)
		ltq_endian_clr(soc->ahb2m_endian_ctrl_shift);

	if (soc->has_ahb4s_endian)
		ltq_endian_clr(soc->ahb4s_endian_ctrl_shift);

	if (soc->has_ahb3m_endian)
		ltq_endian_clr(soc->ahb3m_endian_ctrl_shift);
	ltq_endian_clr(lpp->outbound_shift);
	ltq_endian_clr(lpp->inbound_shift);
#endif /* CONFIG_CPU_BIG_ENDIAN */
}

static inline int pcie_phy_pmu_enable(struct ltq_pcie_port *lpp)
{
	int err;
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	if (soc->has_phy_clk) {
		err = clk_prepare_enable(lpp->phy_clk);
		if (err < 0) {
			dev_err(lpp->dev,
				"failed to enable PHY clock: %d\n", err);
			return err;
		}
	}
	return 0;
}

static inline void pcie_phy_pmu_disable(struct ltq_pcie_port *lpp)
{
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	if (soc->has_phy_clk)
		clk_disable_unprepare(lpp->phy_clk);
}

static inline int pcie_pdi_pmu_enable(struct ltq_pcie_port *lpp)
{
	int err;
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	if (soc->has_pdi_clk) {
		err = clk_prepare_enable(lpp->pdi_clk);
		if (err < 0) {
			dev_err(lpp->dev,
				"failed to enable PDI clock: %d\n", err);
			return err;
		}
	}
	return 0;
}

static inline void pcie_phy_clk_out(struct ltq_pcie_port *lpp)
{
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	if (soc->has_clk_out)
		ltq_pcie_clk_out(lpp->clk_out_pin);
}

static inline void pcie_core_rst_assert(struct ltq_pcie_port *lpp)
{
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	if (soc->has_core_rst)
		reset_control_assert(lpp->core_rst);
}

static inline void pcie_core_rst_deassert(struct ltq_pcie_port *lpp)
{
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	if (soc->has_core_rst) {
		/* Make sure one micro-second delay */
		udelay(1);
		reset_control_deassert(lpp->core_rst);
	}
}

static inline void pcie_phy_rst_assert(struct ltq_pcie_port *lpp)
{
	reset_control_assert(lpp->phy_rst);
}

static inline void pcie_phy_rst_deassert(struct ltq_pcie_port *lpp)
{
	/* Make sure one micro-second delay */
	udelay(1);
	reset_control_deassert(lpp->phy_rst);
}

static inline void pcie_device_rst_assert(struct ltq_pcie_port *lpp)
{
	if (lpp->high_active_rst)
		gpio_set_value(lpp->reset_gpio, 1);
	else
		gpio_set_value(lpp->reset_gpio, 0);
}

static inline void pcie_device_rst_deassert(struct ltq_pcie_port *lpp)
{
	mdelay(lpp->rst_interval);
	if (lpp->high_active_rst)
		gpio_set_value(lpp->reset_gpio, 0);
	else
		gpio_set_value(lpp->reset_gpio, 1);
}

static inline int pcie_core_pmu_setup(struct ltq_pcie_port *lpp)
{
	int err;
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	if (soc->has_bus_clk) {
		err = clk_prepare_enable(lpp->bus_clk);
		if (err < 0) {
			dev_err(lpp->dev, "failed to enable bus clock: %d\n",
				err);
			return err;
		}
	}

	err = clk_prepare_enable(lpp->core_clk);
	if (err < 0) {
		dev_err(lpp->dev, "failed to enable core clock: %d\n", err);
		return err;
	}
	return 0;
}

static inline int pcie_msi_init(struct ltq_pcie_port *lpp)
{
	int err;
	const struct ltq_pcie_soc_data *soc = lpp->soc_data;

	if (soc->has_msi_clk) {
		err = clk_prepare_enable(lpp->msi_clk);
		if (err < 0) {
			dev_err(lpp->dev,
				"failed to enable MSI clock: %d\n", err);
			return err;
		}
	}
	pcie_msi_pic_init(lpp);
	return 0;
}

#endif /* PCIE_LANTIQ_H */
