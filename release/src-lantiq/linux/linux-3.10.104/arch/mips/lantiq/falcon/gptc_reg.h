/******************************************************************************

                               Copyright (c) 2012
                            Lantiq Deutschland GmbH

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _gptc_reg_h
#define _gptc_reg_h

/** \addtogroup GPTC_REGISTER
   @{
*/
/* access macros */
#define gptc_r32(reg) reg_r32(&gptc->reg)
#define gptc_w32(val, reg) reg_w32(val, &gptc->reg)
#define gptc_w32_mask(clear, set, reg) reg_w32_mask(clear, set, &gptc->reg)
#define gptc_r32_table(reg, idx) reg_r32_table(gptc->reg, idx)
#define gptc_w32_table(val, reg, idx) reg_w32_table(val, gptc->reg, idx)
#define gptc_w32_table_mask(clear, set, reg, idx) reg_w32_table_mask(clear, set, gptc->reg, idx)
#define gptc_adr_table(reg, idx) adr_table(gptc->reg, idx)

#define GPTC_MAX_TIMER_COUNTER	6

/** GPTC register structure */
struct gpon_reg_gptc
{
	/** Clock Control Register */
	unsigned int clc; /* 0x00000000 */
	/** Reserved */
	unsigned int res_0; /* 0x00000004 */
	/** GPTC ID Register */
	unsigned int id; /* 0x00000008 */
	/** Reserved */
	unsigned int res_1; /* 0x0000000C */
	struct {
		/** Control Register CON Timer/Counter */
		unsigned int con[2]; /* 0x00000010 */
		/** Run Register RUN Timer/Counter */
		unsigned int run[2]; /* 0x00000018 */
		/** Reload Register RELOAD Timer/Counter */
		unsigned int reload[2]; /* 0x00000020 */
		/** Count Register COUNT Timer/Counter */
		unsigned int count[2]; /* 0x00000028 */
	} tc[GPTC_MAX_TIMER_COUNTER];

	/** Reserved */
	unsigned int res_2[9]; /* 0x00000070 */
	/** IRN Interrupt Enable Register
	    This register contains the enable (or mask) bits for the interrupts.
	    Disabled interrupts are not visible in the IRNCR register and are not
	    signalled via the interrupt line towards the controller. */
	unsigned int irnen; /* 0x000000F4 */
	/** IRN Interrupt Control Register
	    A write operation directly effects the interrupts.
	    This can be used to trigger events under software control for testing
	    purposes. A read operation returns the unmasked interrupt events. */
	unsigned int irnicr; /* 0x000000F8 */
	/** IRN Capture Register
	    This register shows the currently active interrupt events masked with
	    the corresponding enable bits of the IRNEN register.
	    The interrupts can be acknowledged by a write operation. */
	unsigned int irncr; /* 0x000000FC */
};


/* Fields of "Clock Control Register" */
/** Clock Divider for Sleep Mode
    Depending on the hardware configuration, this dividermay or may not be implemented. If not implemented, it behaves thesame as reserved bits */
#define GPTC_CLC_SMC_MASK 0x00FF0000
/** field offset */
#define GPTC_CLC_SMC_OFFSET 16
/** Clock Divider for Normal Run Mode
    Values larger than 1 divide the bus clock frequencyby the given value to obtain the kernel clock */
#define GPTC_CLC_RMC_MASK 0x0000FF00
/** field offset */
#define GPTC_CLC_RMC_OFFSET 8
/** A value of 0 means, that the peripheral is disabled. */
#define GPTC_CLC_RMC_DISRMC 0x00000000
/** A value of 1 means, that the peripheral kernel runs at the sameclock as the bus. */
#define GPTC_CLC_RMC_ENRMC 0x00000100
/** Fast Shut-Off Enable Bit */
#define GPTC_CLC_FSOE 0x00000020
/** Suspend Bit Write Enable for OCDS
    Reading returns 0 for this bit. */
#define GPTC_CLC_SBWE 0x00000010
/** External Request Disable */
#define GPTC_CLC_EDIS 0x00000008
/** Suspend Enable Bit for OCDS */
#define GPTC_CLC_SPEN 0x00000004
/** Disable Status Bit
    Bit DISS can be modified only by writing to bitDISR */
#define GPTC_CLC_DISS 0x00000002
/** Disable Request Bit */
#define GPTC_CLC_DISR 0x00000001

/* Fields of "GPTC ID Register" */
/** Module ID
    Module Identification Number (59H for GPTC). */
#define GPTC_ID_ID_MASK 0x0000FF00
/** field offset */
#define GPTC_ID_ID_OFFSET 8
/** Configuration
    Gives the number of timer/counter blocks in the GPTC */
#define GPTC_ID_CFG_MASK 0x000000E0
/** field offset */
#define GPTC_ID_CFG_OFFSET 5
/** Revision
    Hardware revision number (01H .. 1FH). */
#define GPTC_ID_REV_MASK 0x0000001F
/** field offset */
#define GPTC_ID_REV_OFFSET 0

/* Fields of "Control Register CON Timer/Counter" */
/** Input Synchronization Specification */
#define GPTC_CON_SYNC 0x00000100
/** Count Edge Specification */
#define GPTC_CON_EDGE_MASK 0x000000C0
/** field offset */
#define GPTC_CON_EDGE_OFFSET 6
/** Input Inversion Specification */
#define GPTC_CON_INV 0x00000020
/** Extended Mode Specification (only Timer_A) */
#define GPTC_CON_A_EXT 0x00000010
/** Stopping Specification */
#define GPTC_CON_STP 0x00000008
/** Working Mode Specification */
#define GPTC_CON_CNT 0x00000004
/** Counting Direction Specification */
#define GPTC_CON_DIR 0x00000002
/** Count Enable Specification */
#define GPTC_CON_EN 0x00000001

/* Fields of "Run Register RUN Timer/Counter" */
/** Reload when starting */
#define GPTC_RUN_RL 0x00000004
/** Clear Enable */
#define GPTC_RUN_CEN 0x00000002
/** Set enable */
#define GPTC_RUN_SEN 0x00000001

/* Fields of "Reload Register RELOAD Timer/Counter" */
/** Upper Reload part for Timer/Counter */
#define GPTC_RELOAD_HIGH_MASK 0xFFFF0000
/** field offset */
#define GPTC_RELOAD_HIGH_OFFSET 16
/** Lower Reload part for Timer/Counter */
#define GPTC_RELOAD_LOW_MASK 0x0000FFFF
/** field offset */
#define GPTC_RELOAD_LOW_OFFSET 0

/* Fields of "Count Register COUNT Timer/Counter" */
/** Upper Reload part for Timer/Counter */
#define GPTC_COUNT_HIGH_MASK 0xFFFF0000
/** field offset */
#define GPTC_COUNT_HIGH_OFFSET 16
/** Lower Reload part for Timer/Counter */
#define GPTC_COUNT_LOW_MASK 0x0000FFFF
/** field offset */
#define GPTC_COUNT_LOW_OFFSET 0

/* Fields of "IRN Interrupt Enable Register" */
/** Timer/Counter 3B Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNEN_TC3B 0x00000020
/* Disable
#define GPTC_IRNEN_TC3B_DIS 0x00000000 */
/** Enable */
#define GPTC_IRNEN_TC3B_EN 0x00000020
/** Timer/Counter 3A Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNEN_TC3A 0x00000010
/* Disable
#define GPTC_IRNEN_TC3A_DIS 0x00000000 */
/** Enable */
#define GPTC_IRNEN_TC3A_EN 0x00000010
/** Timer/Counter 2B Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNEN_TC2B 0x00000008
/* Disable
#define GPTC_IRNEN_TC2B_DIS 0x00000000 */
/** Enable */
#define GPTC_IRNEN_TC2B_EN 0x00000008
/** Timer/Counter 2A Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNEN_TC2A 0x00000004
/* Disable
#define GPTC_IRNEN_TC2A_DIS 0x00000000 */
/** Enable */
#define GPTC_IRNEN_TC2A_EN 0x00000004
/** Timer/Counter 1B Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNEN_TC1B 0x00000002
/* Disable
#define GPTC_IRNEN_TC1B_DIS 0x00000000 */
/** Enable */
#define GPTC_IRNEN_TC1B_EN 0x00000002
/** Timer/Counter 1A Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNEN_TC1A 0x00000001
/* Disable
#define GPTC_IRNEN_TC1A_DIS 0x00000000 */
/** Enable */
#define GPTC_IRNEN_TC1A_EN 0x00000001

/* Fields of "IRN Interrupt Control Register" */
/** Timer/Counter 3B Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNICR_TC3B 0x00000020
/** Timer/Counter 3A Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNICR_TC3A 0x00000010
/** Timer/Counter 2B Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNICR_TC2B 0x00000008
/** Timer/Counter 2A Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNICR_TC2A 0x00000004
/** Timer/Counter 1B Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNICR_TC1B 0x00000002
/** Timer/Counter 1A Interrupt Request
    Description of the bit: see IRNCR register */
#define GPTC_IRNICR_TC1A 0x00000001

/* Fields of "IRN Capture Register" */
/** Timer/Counter 3B Interrupt Request
    This bit is edge-sensitive. This bit is a direct interrupt. */
#define GPTC_IRNCR_TC3B 0x00000020
/* Nothing
#define GPTC_IRNCR_TC3B_NULL 0x00000000 */
/** Write: Acknowledge the interrupt. */
#define GPTC_IRNCR_TC3B_INTACK 0x00000020
/** Read: Interrupt occurred. */
#define GPTC_IRNCR_TC3B_INTOCC 0x00000020
/** Timer/Counter 3A Interrupt Request
    This bit is edge-sensitive. This bit is a direct interrupt. */
#define GPTC_IRNCR_TC3A 0x00000010
/* Nothing
#define GPTC_IRNCR_TC3A_NULL 0x00000000 */
/** Write: Acknowledge the interrupt. */
#define GPTC_IRNCR_TC3A_INTACK 0x00000010
/** Read: Interrupt occurred. */
#define GPTC_IRNCR_TC3A_INTOCC 0x00000010
/** Timer/Counter 2B Interrupt Request
    This bit is edge-sensitive. This bit is a direct interrupt. */
#define GPTC_IRNCR_TC2B 0x00000008
/* Nothing
#define GPTC_IRNCR_TC2B_NULL 0x00000000 */
/** Write: Acknowledge the interrupt. */
#define GPTC_IRNCR_TC2B_INTACK 0x00000008
/** Read: Interrupt occurred. */
#define GPTC_IRNCR_TC2B_INTOCC 0x00000008
/** Timer/Counter 2A Interrupt Request
    This bit is edge-sensitive. This bit is a direct interrupt. */
#define GPTC_IRNCR_TC2A 0x00000004
/* Nothing
#define GPTC_IRNCR_TC2A_NULL 0x00000000 */
/** Write: Acknowledge the interrupt. */
#define GPTC_IRNCR_TC2A_INTACK 0x00000004
/** Read: Interrupt occurred. */
#define GPTC_IRNCR_TC2A_INTOCC 0x00000004
/** Timer/Counter 1B Interrupt Request
    This bit is edge-sensitive. This bit is a direct interrupt. */
#define GPTC_IRNCR_TC1B 0x00000002
/* Nothing
#define GPTC_IRNCR_TC1B_NULL 0x00000000 */
/** Write: Acknowledge the interrupt. */
#define GPTC_IRNCR_TC1B_INTACK 0x00000002
/** Read: Interrupt occurred. */
#define GPTC_IRNCR_TC1B_INTOCC 0x00000002
/** Timer/Counter 1A Interrupt Request
    This bit is edge-sensitive. This bit is a direct interrupt. */
#define GPTC_IRNCR_TC1A 0x00000001
/* Nothing
#define GPTC_IRNCR_TC1A_NULL 0x00000000 */
/** Write: Acknowledge the interrupt. */
#define GPTC_IRNCR_TC1A_INTACK 0x00000001
/** Read: Interrupt occurred. */
#define GPTC_IRNCR_TC1A_INTOCC 0x00000001

/*! @} */ /* GPTC_REGISTER */

#endif /* _gptc_reg_h */
