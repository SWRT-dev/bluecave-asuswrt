/******************************************************************************
**
** FILE NAME    : ifxmips_atm_ar9.c
** PROJECT      : UEIP
** MODULES      : ATM
**
** DATE         : 7 Jul 2009
** AUTHOR       : Xu Liang
** DESCRIPTION  : ATM driver common source file (core functions)
** COPYRIGHT    :       Copyright (c) 2006
**                      Infineon Technologies AG
**                      Am Campeon 1-12, 85579 Neubiberg, Germany
**
**    This program is free software; you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation; either version 2 of the License, or
**    (at your option) any later version.
**
** HISTORY
** $Date        $Author         $Comment
** 07 JUL 2009  Xu Liang        Init Version
*******************************************************************************/



/*
 * ####################################
 *              Head File
 * ####################################
 */

/*
 *  Common Head File
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/ioctl.h>
#include <asm/delay.h>
#include <asm/irq.h>

#include <linux/clk.h>
#include <lantiq_soc.h>

/*
 *  Chip Specific Head File
 */
#include "ltqmips_atm_core.h"
#if defined(ENABLE_ATM_RETX) && ENABLE_ATM_RETX
  #include "ltqmips_atm_fw_ar9_retx.h"
#else
  #include "ltqmips_atm_fw_ar9.h"
#endif


/*
 * ####################################
 *              Definition
 * ####################################
 */

/*
 *  EMA Settings
 */
#define EMA_CMD_BUF_LEN      0x0040
#define EMA_CMD_BASE_ADDR    (0x00003B80 << 2)
#define EMA_DATA_BUF_LEN     0x0100
#define EMA_DATA_BASE_ADDR   (0x00003C00 << 2)
#define EMA_WRITE_BURST      0x2
#define EMA_READ_BURST       0x2


/*
 * ####################################
 *             Declaration
 * ####################################
 */

/*
 *  Hardware Init/Uninit Functions
 */
static inline void init_pmu(void);
static inline void uninit_pmu(void);
static inline void reset_ppe(void);
static inline void init_ema(void);
static inline void init_mailbox(void);
static inline void init_atm_tc(void);
static inline void clear_share_buffer(void);


/*
 * ####################################
 *            Local Variable
 * ####################################
 */



/*
 * ####################################
 *            Local Function
 * ####################################
 */

static inline void init_pmu(void)
{
/*
    (unsigned long *)0xBF10201C &= ~((1 << 15) | (1 << 13) | (1 << 9));
    PPE_TOP_PMU_SETUP(IFX_PMU_ENABLE);
    PPE_SLL01_PMU_SETUP(IFX_PMU_ENABLE);
    PPE_TC_PMU_SETUP(IFX_PMU_ENABLE);
    PPE_EMA_PMU_SETUP(IFX_PMU_ENABLE);
    PPE_QSB_PMU_SETUP(IFX_PMU_ENABLE);
    PPE_TPE_PMU_SETUP(IFX_PMU_ENABLE);
    DSL_DFE_PMU_SETUP(IFX_PMU_ENABLE);
*/
}

#ifdef CONFIG_LTQ_ATM_XRX300
static inline void set_ppe_250(void)
{
	unsigned int clk_fsr;

	clk_fsr = ltq_cgu_r32(0x0010);
	clk_fsr = (clk_fsr & ~(0x70000)) | (0x10000);
	ltq_cgu_w32(clk_fsr, 0x0010);
	printk("set CGU CLKFSR: 0x%x\n", ltq_cgu_r32(0x0010));
}
#endif

static inline void uninit_pmu(void)
{
/*
    PPE_SLL01_PMU_SETUP(IFX_PMU_DISABLE);
    PPE_TC_PMU_SETUP(IFX_PMU_DISABLE);
    PPE_EMA_PMU_SETUP(IFX_PMU_DISABLE);
    PPE_QSB_PMU_SETUP(IFX_PMU_DISABLE);
    PPE_TPE_PMU_SETUP(IFX_PMU_DISABLE);
    DSL_DFE_PMU_SETUP(IFX_PMU_DISABLE);
    //PPE_TOP_PMU_SETUP(IFX_PMU_DISABLE);
*/
}

static inline void reset_ppe(void)
{
#ifdef MODULE
    //  reset PPE
    ifx_rcu_rst(IFX_RCU_DOMAIN_PPE, IFX_RCU_MODULE_ATM);
#endif
}

static inline void init_ema(void)
{
    IFX_REG_W32((EMA_CMD_BUF_LEN << 16) | (EMA_CMD_BASE_ADDR >> 2), EMA_CMDCFG);
    IFX_REG_W32((EMA_DATA_BUF_LEN << 16) | (EMA_DATA_BASE_ADDR >> 2), EMA_DATACFG);
    IFX_REG_W32(0x000000FF, EMA_IER);
    IFX_REG_W32(EMA_READ_BURST | (EMA_WRITE_BURST << 2), EMA_CFG);
}

static inline void init_mailbox(void)
{
    IFX_REG_W32(0xFFFFFFFF, MBOX_IGU1_ISRC);
    IFX_REG_W32(0x00000000, MBOX_IGU1_IER);
    IFX_REG_W32(0xFFFFFFFF, MBOX_IGU3_ISRC);
    IFX_REG_W32(0x00000000, MBOX_IGU3_IER);
}

static inline void init_atm_tc(void)
{
}

static inline void clear_share_buffer(void)
{
    volatile u32 *p = SB_RAM0_ADDR(0);
    unsigned int i;

    for ( i = 0; i < SB_RAM0_DWLEN + SB_RAM1_DWLEN + SB_RAM2_DWLEN + SB_RAM3_DWLEN + SB_RAM4_DWLEN; i++ )
        IFX_REG_W32(0, p++);
}

/*
 *  Description:
 *    Download PPE firmware binary code.
 *  Input:
 *    src       --- u32 *, binary code buffer
 *    dword_len --- unsigned int, binary code length in DWORD (32-bit)
 *  Output:
 *    int       --- 0:    Success
 *                  else:           Error Code
 */
static inline int pp32_download_code(u32 *code_src, unsigned int code_dword_len, u32 *data_src, unsigned int data_dword_len)
{
    volatile u32 *dest;

    if ( code_src == 0 || ((unsigned long)code_src & 0x03) != 0
        || data_src == 0 || ((unsigned long)data_src & 0x03) != 0 )
        return -EINVAL;

    if ( code_dword_len <= CDM_CODE_MEMORYn_DWLEN(0) )
        IFX_REG_W32(0x00, CDM_CFG);
    else
        IFX_REG_W32(0x04, CDM_CFG);

    /*  copy code   */
    dest = CDM_CODE_MEMORY(0, 0);
    while ( code_dword_len-- > 0 )
        IFX_REG_W32(*code_src++, dest++);

    /*  copy data   */
    dest = CDM_DATA_MEMORY(0, 0);
    while ( data_dword_len-- > 0 )
        IFX_REG_W32(*data_src++, dest++);

    return 0;
}



/*
 * ####################################
 *           Global Function
 * ####################################
 */

extern void ifx_atm_get_fw_ver(unsigned int *major, unsigned int *minor)
{
    ASSERT(major != NULL, "pointer is NULL");
    ASSERT(minor != NULL, "pointer is NULL");

#if (defined(ENABLE_ATM_RETX) && ENABLE_ATM_RETX) || defined(VER_IN_FIRMWARE)
    *major = FW_VER_ID->major;
    *minor = FW_VER_ID->minor;
#else
    *major = ATM_FW_VER_MAJOR;
    *minor = ATM_FW_VER_MINOR;
#endif
}

void ifx_atm_init_chip(void)
{
    init_pmu();

#ifdef CONFIG_LTQ_ATM_XRX300
	set_ppe_250();
#endif

    reset_ppe();

    init_ema();

    init_mailbox();

    init_atm_tc();

    clear_share_buffer();
}

void ifx_atm_uninit_chip(void)
{
    uninit_pmu();
}

/*
 *  Description:
 *    Initialize and start up PP32.
 *  Input:
 *    none
 *  Output:
 *    int  --- 0: Success
 *             else:        Error Code
 */
int ifx_pp32_start(int pp32)
{
    int ret;

    /*  download firmware   */
    ret = pp32_download_code(firmware_binary_code, sizeof(firmware_binary_code) / sizeof(*firmware_binary_code), firmware_binary_data, sizeof(firmware_binary_data) / sizeof(*firmware_binary_data));
    if ( ret != 0 )
        return ret;

    /*  run PP32    */
    IFX_REG_W32(DBG_CTRL_RESTART, PP32_DBG_CTRL(0));

    /*  idle for a while to let PP32 init itself    */
    udelay(10);

    return 0;
}

/*
 *  Description:
 *    Halt PP32.
 *  Input:
 *    none
 *  Output:
 *    none
 */
void ifx_pp32_stop(int pp32)
{
    /*  halt PP32   */
    IFX_REG_W32(DBG_CTRL_STOP, PP32_DBG_CTRL(0));
}

unsigned int ifx_atm_get_qsb_clk(void)
{
    unsigned int shift = ((ltq_cgu_r32(0x0010)) >> 28) & 0x03;
	unsigned int fpi_clk_rate;

	struct clk *fpi_clk = clk_get_fpi();
	fpi_clk_rate = clk_get_rate(fpi_clk);
	return (fpi_clk_rate >> shift);
}

#ifdef CONFIG_LTQ_ATM_XRX300
uint32_t get_mailbox1_irq_num(void)
{
    volatile struct ar10_chipid *chipid;
    static uint32_t MAILBOX1_IRQ_NUM = 0;

    if(!MAILBOX1_IRQ_NUM){
        chipid = (volatile struct ar10_chipid *)AR10_MIPS_CHIPID;
        if(chipid->version > 1){
            MAILBOX1_IRQ_NUM = INT_NUM_IM0_IRL24;
        }else{
            MAILBOX1_IRQ_NUM = INT_NUM_IM1_IRL29;
        }
    }

    return MAILBOX1_IRQ_NUM; 
}
#endif

