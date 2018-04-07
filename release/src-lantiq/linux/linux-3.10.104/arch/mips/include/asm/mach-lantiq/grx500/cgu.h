//-----------------------------------------------------------------------------
// LSD Generator
//-----------------------------------------------------------------------------
// Perl Package        : LSD::generator::targetC (v1.1)
// LSD Source          : D:/Users/shij/Perforce/l1033.grx500.win.v_shij.priv.dfv.grx500.dfv/ipg_lsd/lsd_sys/source/xml/reg_files/cgu.xml
// Register File Name  : CGU
// Register File Title : Registers Description
// Register Width      : 32
// Note                : Doxygen compliant comments
//-----------------------------------------------------------------------------

#ifndef _CGU_H
#define _CGU_H

//! \defgroup CGU Register File CGU - Registers Description
//! @{

//! Base Address of CGU
#define CGU_MODULE_BASE 0x16200000u

//! \defgroup CGU_PLL0A_CFG0 Register CGU_PLL0A_CFG0 - PLL0 Configuration Register 0
//! @{

//! Register Offset (relative)
#define CGU_PLL0A_CFG0 0x4
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_PLL0A_CFG0 0x16200004u

//! Register Reset Value
#define CGU_PLL0A_CFG0_RST 0x000000F3u

//! Field PLL_RST - PLL Reset
#define CGU_PLL0A_CFG0_PLL_RST_POS 0
//! Field PLL_RST - PLL Reset
#define CGU_PLL0A_CFG0_PLL_RST_MASK 0x1u
//! Constant RESET - Reset request
#define CONST_CGU_PLL0A_CFG0_PLL_RST_RESET 0x0
//! Constant NONE - No action
#define CONST_CGU_PLL0A_CFG0_PLL_RST_NONE 0x1

//! Field PLL_L - PLL Lock Status
#define CGU_PLL0A_CFG0_PLL_L_POS 1
//! Field PLL_L - PLL Lock Status
#define CGU_PLL0A_CFG0_PLL_L_MASK 0x2u
//! Constant FREE - Free running
#define CONST_CGU_PLL0A_CFG0_PLL_L_FREE 0x0
//! Constant LOCK - Lock
#define CONST_CGU_PLL0A_CFG0_PLL_L_LOCK 0x1

//! Field PLL_N - PLL0 N Divider
#define CGU_PLL0A_CFG0_PLL_N_POS 2
//! Field PLL_N - PLL0 N Divider
#define CGU_PLL0A_CFG0_PLL_N_MASK 0x1FCu

//! Field PLL_K - PLL0 Fractional K Divider
#define CGU_PLL0A_CFG0_PLL_K_POS 9
//! Field PLL_K - PLL0 Fractional K Divider
#define CGU_PLL0A_CFG0_PLL_K_MASK 0x3FFFFE00u

//! Field PLL_BP - PLL Bypass
#define CGU_PLL0A_CFG0_PLL_BP_POS 30
//! Field PLL_BP - PLL Bypass
#define CGU_PLL0A_CFG0_PLL_BP_MASK 0x40000000u
//! Constant DISABLE - Disable
#define CONST_CGU_PLL0A_CFG0_PLL_BP_DISABLE 0x0
//! Constant ENABLE - Enable
#define CONST_CGU_PLL0A_CFG0_PLL_BP_ENABLE 0x1

//! Field PLL_BUFF_OUT - PLL Output buffer
#define CGU_PLL0A_CFG0_PLL_BUFF_OUT_POS 31
//! Field PLL_BUFF_OUT - PLL Output buffer
#define CGU_PLL0A_CFG0_PLL_BUFF_OUT_MASK 0x80000000u
//! Constant DIS - Disable
#define CONST_CGU_PLL0A_CFG0_PLL_BUFF_OUT_DIS 0x0
//! Constant EN - Enable
#define CONST_CGU_PLL0A_CFG0_PLL_BUFF_OUT_EN 0x1

//! @}

//! \defgroup CGU_PLL0A_CFG1 Register CGU_PLL0A_CFG1 - PLL0 Configuration Register 1
//! @{

//! Register Offset (relative)
#define CGU_PLL0A_CFG1 0x8
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_PLL0A_CFG1 0x16200008u

//! Register Reset Value
#define CGU_PLL0A_CFG1_RST 0x40409869u

//! Field PLL_CLK1 - PLL Clock Output 1
#define CGU_PLL0A_CFG1_PLL_CLK1_POS 0
//! Field PLL_CLK1 - PLL Clock Output 1
#define CGU_PLL0A_CFG1_PLL_CLK1_MASK 0xFu
//! Constant GND - ref clk
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK1_DIV64 0xF

//! Field PLL_CLK2 - PLL Clock Output 2
#define CGU_PLL0A_CFG1_PLL_CLK2_POS 4
//! Field PLL_CLK2 - PLL Clock Output 2
#define CGU_PLL0A_CFG1_PLL_CLK2_MASK 0xF0u
//! Constant GND - ref clk
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK2_DIV64 0xF

//! Field PLL_CLK3 - PLL Clock Output 3
#define CGU_PLL0A_CFG1_PLL_CLK3_POS 8
//! Field PLL_CLK3 - PLL Clock Output 3
#define CGU_PLL0A_CFG1_PLL_CLK3_MASK 0xF00u
//! Constant GND - ref clk
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK3_DIV64 0xF

//! Field PLL_CLK4 - PLL Clock Output 4
#define CGU_PLL0A_CFG1_PLL_CLK4_POS 12
//! Field PLL_CLK4 - PLL Clock Output 4
#define CGU_PLL0A_CFG1_PLL_CLK4_MASK 0xF000u
//! Constant GND - ref clk
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK4_DIV64 0xF

//! Field PLL_CLK5 - PLL Clock Output 5
#define CGU_PLL0A_CFG1_PLL_CLK5_POS 16
//! Field PLL_CLK5 - PLL Clock Output 5
#define CGU_PLL0A_CFG1_PLL_CLK5_MASK 0x30000u
//! Constant DIV4 - Divide by 4 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK5_DIV4 0x0
//! Constant DIV6 - Divide by 6 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK5_DIV6 0x1
//! Constant DIV8 - Divide by 8 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK5_DIV8 0x2
//! Constant DIV12 - Divide by 12 of VCO 2.4GHz
#define CONST_CGU_PLL0A_CFG1_PLL_CLK5_DIV12 0x3

//! Field PLL_OPD1 - PLL Output buffer power down
#define CGU_PLL0A_CFG1_PLL_OPD1_POS 18
//! Field PLL_OPD1 - PLL Output buffer power down
#define CGU_PLL0A_CFG1_PLL_OPD1_MASK 0x40000u
//! Constant CLK1_EN - Enable
#define CONST_CGU_PLL0A_CFG1_PLL_OPD1_CLK1_EN 0x0
//! Constant CLK1_DIS - Disable
#define CONST_CGU_PLL0A_CFG1_PLL_OPD1_CLK1_DIS 0x1

//! Field PLL_OPD2 - PLL Output buffer power down
#define CGU_PLL0A_CFG1_PLL_OPD2_POS 19
//! Field PLL_OPD2 - PLL Output buffer power down
#define CGU_PLL0A_CFG1_PLL_OPD2_MASK 0x80000u
//! Constant CLK2_EN - Enable
#define CONST_CGU_PLL0A_CFG1_PLL_OPD2_CLK2_EN 0x0
//! Constant CLK2_DIS - Disable
#define CONST_CGU_PLL0A_CFG1_PLL_OPD2_CLK2_DIS 0x1

//! Field PLL_OPD3 - PLL Output buffer power down
#define CGU_PLL0A_CFG1_PLL_OPD3_POS 20
//! Field PLL_OPD3 - PLL Output buffer power down
#define CGU_PLL0A_CFG1_PLL_OPD3_MASK 0x100000u
//! Constant CLK3_EN - Enable
#define CONST_CGU_PLL0A_CFG1_PLL_OPD3_CLK3_EN 0x0
//! Constant CLK3_DIS - Disable
#define CONST_CGU_PLL0A_CFG1_PLL_OPD3_CLK3_DIS 0x1

//! Field PLL_OPD4 - PLL Output buffer power down
#define CGU_PLL0A_CFG1_PLL_OPD4_POS 21
//! Field PLL_OPD4 - PLL Output buffer power down
#define CGU_PLL0A_CFG1_PLL_OPD4_MASK 0x200000u
//! Constant CLK4_EN - Enable
#define CONST_CGU_PLL0A_CFG1_PLL_OPD4_CLK4_EN 0x0
//! Constant CLK4_DIS - Disable
#define CONST_CGU_PLL0A_CFG1_PLL_OPD4_CLK4_DIS 0x1

//! Field PLL_OPD5 - PLL Output buffer power down
#define CGU_PLL0A_CFG1_PLL_OPD5_POS 22
//! Field PLL_OPD5 - PLL Output buffer power down
#define CGU_PLL0A_CFG1_PLL_OPD5_MASK 0x400000u
//! Constant CLK5_EN - Enable
#define CONST_CGU_PLL0A_CFG1_PLL_OPD5_CLK5_EN 0x0
//! Constant CLK5_DIS - Disable
#define CONST_CGU_PLL0A_CFG1_PLL_OPD5_CLK5_DIS 0x1

//! Field PLL_N_MODE - PLL N Mode
#define CGU_PLL0A_CFG1_PLL_N_MODE_POS 23
//! Field PLL_N_MODE - PLL N Mode
#define CGU_PLL0A_CFG1_PLL_N_MODE_MASK 0x800000u
//! Constant FRAC - Fractional PLL mode
#define CONST_CGU_PLL0A_CFG1_PLL_N_MODE_FRAC 0x0
//! Constant INT - Integer PLL mode
#define CONST_CGU_PLL0A_CFG1_PLL_N_MODE_INT 0x1

//! Field PLL_SSC - PLL Spread Spectrum Mode
#define CGU_PLL0A_CFG1_PLL_SSC_POS 24
//! Field PLL_SSC - PLL Spread Spectrum Mode
#define CGU_PLL0A_CFG1_PLL_SSC_MASK 0x1000000u
//! Constant ENABLE - Enable
#define CONST_CGU_PLL0A_CFG1_PLL_SSC_ENABLE 0x0
//! Constant DISABLE - Disable
#define CONST_CGU_PLL0A_CFG1_PLL_SSC_DISABLE 0x1

//! Field GLITCHFRE_DIS - Disable glitch free circuit during power up
#define CGU_PLL0A_CFG1_GLITCHFRE_DIS_POS 28
//! Field GLITCHFRE_DIS - Disable glitch free circuit during power up
#define CGU_PLL0A_CFG1_GLITCHFRE_DIS_MASK 0x10000000u
//! Constant EN - Default. Circuit is enabled.
#define CONST_CGU_PLL0A_CFG1_GLITCHFRE_DIS_EN 0x0
//! Constant DIS - Disable the circuit .may cause issue of clock outputs.
#define CONST_CGU_PLL0A_CFG1_GLITCHFRE_DIS_DIS 0x1

//! Field CPU_CLKM_SEL - CPU Clock Master Select
#define CGU_PLL0A_CFG1_CPU_CLKM_SEL_POS 29
//! Field CPU_CLKM_SEL - CPU Clock Master Select
#define CGU_PLL0A_CFG1_CPU_CLKM_SEL_MASK 0x20000000u
//! Constant PLL0A - PLL0A
#define CONST_CGU_PLL0A_CFG1_CPU_CLKM_SEL_PLL0A 0x0
//! Constant PLL0B - PLL0B
#define CONST_CGU_PLL0A_CFG1_CPU_CLKM_SEL_PLL0B 0x1

//! Field PLL_BW - PLL Bandwidth Select
#define CGU_PLL0A_CFG1_PLL_BW_POS 30
//! Field PLL_BW - PLL Bandwidth Select
#define CGU_PLL0A_CFG1_PLL_BW_MASK 0xC0000000u
//! Constant BW0 - Bandwidth 0
#define CONST_CGU_PLL0A_CFG1_PLL_BW_BW0 0x0
//! Constant BW1 - Bandwidth 1
#define CONST_CGU_PLL0A_CFG1_PLL_BW_BW1 0x1
//! Constant BW2 - Bandwidth 2
#define CONST_CGU_PLL0A_CFG1_PLL_BW_BW2 0x2
//! Constant BW3 - Bandwidth 3
#define CONST_CGU_PLL0A_CFG1_PLL_BW_BW3 0x3

//! @}

//! \defgroup PLL0A_SSC_CTRL Register PLL0A_SSC_CTRL - Spread Spectrum Control
//! @{

//! Register Offset (relative)
#define PLL0A_SSC_CTRL 0xC
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0A_SSC_CTRL 0x1620000Cu

//! Register Reset Value
#define PLL0A_SSC_CTRL_RST 0x00000000u

//! Field EN - Enable
#define PLL0A_SSC_CTRL_EN_POS 0
//! Field EN - Enable
#define PLL0A_SSC_CTRL_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_PLL0A_SSC_CTRL_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_PLL0A_SSC_CTRL_EN_EN 0x1

//! Field INIT - Init
#define PLL0A_SSC_CTRL_INIT_POS 1
//! Field INIT - Init
#define PLL0A_SSC_CTRL_INIT_MASK 0x2u
//! Constant NONE - No action
#define CONST_PLL0A_SSC_CTRL_INIT_NONE 0x0
//! Constant START - Start
#define CONST_PLL0A_SSC_CTRL_INIT_START 0x1

//! Field LEN - SSC Code Size
#define PLL0A_SSC_CTRL_LEN_POS 2
//! Field LEN - SSC Code Size
#define PLL0A_SSC_CTRL_LEN_MASK 0x1Cu

//! Field DIV - SSC Divider
#define PLL0A_SSC_CTRL_DIV_POS 8
//! Field DIV - SSC Divider
#define PLL0A_SSC_CTRL_DIV_MASK 0xFF00u

//! @}

//! \defgroup PLL0A_SSC_SCALE Register PLL0A_SSC_SCALE - SSC Scaling
//! @{

//! Register Offset (relative)
#define PLL0A_SSC_SCALE 0x10
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0A_SSC_SCALE 0x16200010u

//! Register Reset Value
#define PLL0A_SSC_SCALE_RST 0x00000000u

//! Field SCALE - SSC to ROPLL scaling
#define PLL0A_SSC_SCALE_SCALE_POS 0
//! Field SCALE - SSC to ROPLL scaling
#define PLL0A_SSC_SCALE_SCALE_MASK 0x7u

//! Field INTCOEF - Interpolator Coefficient
#define PLL0A_SSC_SCALE_INTCOEF_POS 4
//! Field INTCOEF - Interpolator Coefficient
#define PLL0A_SSC_SCALE_INTCOEF_MASK 0xF0u

//! Field GSCALE - SSC to GPHY IP Scaling
#define PLL0A_SSC_SCALE_GSCALE_POS 8
//! Field GSCALE - SSC to GPHY IP Scaling
#define PLL0A_SSC_SCALE_GSCALE_MASK 0xFF00u

//! @}

//! \defgroup PLL0A_COEF_0 Register PLL0A_COEF_0 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0A_COEF_0 0x14
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0A_COEF_0 0x16200014u

//! Register Reset Value
#define PLL0A_COEF_0_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0A_COEF_0_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0A_COEF_0_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0A_COEF_1 Register PLL0A_COEF_1 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0A_COEF_1 0x18
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0A_COEF_1 0x16200018u

//! Register Reset Value
#define PLL0A_COEF_1_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0A_COEF_1_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0A_COEF_1_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0A_COEF_2 Register PLL0A_COEF_2 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0A_COEF_2 0x1C
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0A_COEF_2 0x1620001Cu

//! Register Reset Value
#define PLL0A_COEF_2_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0A_COEF_2_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0A_COEF_2_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0A_COEF_3 Register PLL0A_COEF_3 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0A_COEF_3 0x20
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0A_COEF_3 0x16200020u

//! Register Reset Value
#define PLL0A_COEF_3_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0A_COEF_3_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0A_COEF_3_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0A_COEF_4 Register PLL0A_COEF_4 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0A_COEF_4 0x24
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0A_COEF_4 0x16200024u

//! Register Reset Value
#define PLL0A_COEF_4_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0A_COEF_4_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0A_COEF_4_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0A_COEF_5 Register PLL0A_COEF_5 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0A_COEF_5 0x28
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0A_COEF_5 0x16200028u

//! Register Reset Value
#define PLL0A_COEF_5_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0A_COEF_5_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0A_COEF_5_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0A_COEF_6 Register PLL0A_COEF_6 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0A_COEF_6 0x2C
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0A_COEF_6 0x1620002Cu

//! Register Reset Value
#define PLL0A_COEF_6_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0A_COEF_6_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0A_COEF_6_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0A_COEF_7 Register PLL0A_COEF_7 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0A_COEF_7 0x30
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0A_COEF_7 0x16200030u

//! Register Reset Value
#define PLL0A_COEF_7_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0A_COEF_7_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0A_COEF_7_DATA_MASK 0xFFFFu

//! @}

//! \defgroup CGU_PLL0B_CFG0 Register CGU_PLL0B_CFG0 - PLL0b Configuration Register 0
//! @{

//! Register Offset (relative)
#define CGU_PLL0B_CFG0 0x34
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_PLL0B_CFG0 0x16200034u

//! Register Reset Value
#define CGU_PLL0B_CFG0_RST 0x000000CBu

//! Field PLL_RST - PLL Reset
#define CGU_PLL0B_CFG0_PLL_RST_POS 0
//! Field PLL_RST - PLL Reset
#define CGU_PLL0B_CFG0_PLL_RST_MASK 0x1u
//! Constant RESET - Reset request
#define CONST_CGU_PLL0B_CFG0_PLL_RST_RESET 0x0
//! Constant NONE - No action
#define CONST_CGU_PLL0B_CFG0_PLL_RST_NONE 0x1

//! Field PLL_L - PLL Lock Status
#define CGU_PLL0B_CFG0_PLL_L_POS 1
//! Field PLL_L - PLL Lock Status
#define CGU_PLL0B_CFG0_PLL_L_MASK 0x2u
//! Constant FREE - Free running
#define CONST_CGU_PLL0B_CFG0_PLL_L_FREE 0x0
//! Constant LOCK - Lock
#define CONST_CGU_PLL0B_CFG0_PLL_L_LOCK 0x1

//! Field PLL_N - PLL0 N Divider
#define CGU_PLL0B_CFG0_PLL_N_POS 2
//! Field PLL_N - PLL0 N Divider
#define CGU_PLL0B_CFG0_PLL_N_MASK 0x1FCu

//! Field PLL_K - PLL0 Fractional K Divider
#define CGU_PLL0B_CFG0_PLL_K_POS 9
//! Field PLL_K - PLL0 Fractional K Divider
#define CGU_PLL0B_CFG0_PLL_K_MASK 0x3FFFFE00u

//! Field PLL_BP - PLL Bypass
#define CGU_PLL0B_CFG0_PLL_BP_POS 30
//! Field PLL_BP - PLL Bypass
#define CGU_PLL0B_CFG0_PLL_BP_MASK 0x40000000u
//! Constant DISABLE - Disable
#define CONST_CGU_PLL0B_CFG0_PLL_BP_DISABLE 0x0
//! Constant ENABLE - Enable
#define CONST_CGU_PLL0B_CFG0_PLL_BP_ENABLE 0x1

//! Field PLL_BUFF_OUT - PLL Output buffer
#define CGU_PLL0B_CFG0_PLL_BUFF_OUT_POS 31
//! Field PLL_BUFF_OUT - PLL Output buffer
#define CGU_PLL0B_CFG0_PLL_BUFF_OUT_MASK 0x80000000u
//! Constant DIS - Disable
#define CONST_CGU_PLL0B_CFG0_PLL_BUFF_OUT_DIS 0x0
//! Constant EN - Enable
#define CONST_CGU_PLL0B_CFG0_PLL_BUFF_OUT_EN 0x1

//! @}

//! \defgroup CGU_PLL0B_CFG1 Register CGU_PLL0B_CFG1 - PLL0B Configuration Register 1
//! @{

//! Register Offset (relative)
#define CGU_PLL0B_CFG1 0x38
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_PLL0B_CFG1 0x16200038u

//! Register Reset Value
#define CGU_PLL0B_CFG1_RST 0x40401856u

//! Field PLL_CLK1 - PLL Clock Output 1
#define CGU_PLL0B_CFG1_PLL_CLK1_POS 0
//! Field PLL_CLK1 - PLL Clock Output 1
#define CGU_PLL0B_CFG1_PLL_CLK1_MASK 0xFu
//! Constant GND - ref clk
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK1_DIV64 0xF

//! Field PLL_CLK2 - PLL Clock Output 2
#define CGU_PLL0B_CFG1_PLL_CLK2_POS 4
//! Field PLL_CLK2 - PLL Clock Output 2
#define CGU_PLL0B_CFG1_PLL_CLK2_MASK 0xF0u
//! Constant GND - ref clk
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK2_DIV64 0xF

//! Field PLL_CLK3 - PLL Clock Output 3
#define CGU_PLL0B_CFG1_PLL_CLK3_POS 8
//! Field PLL_CLK3 - PLL Clock Output 3
#define CGU_PLL0B_CFG1_PLL_CLK3_MASK 0xF00u
//! Constant GND - ref clk
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK3_DIV64 0xF

//! Field PLL_CLK4 - PLL Clock Output 4
#define CGU_PLL0B_CFG1_PLL_CLK4_POS 12
//! Field PLL_CLK4 - PLL Clock Output 4
#define CGU_PLL0B_CFG1_PLL_CLK4_MASK 0xF000u
//! Constant GND - ref clk
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK4_DIV64 0xF

//! Field PLL_CLK5 - PLL Clock Output 4
#define CGU_PLL0B_CFG1_PLL_CLK5_POS 16
//! Field PLL_CLK5 - PLL Clock Output 4
#define CGU_PLL0B_CFG1_PLL_CLK5_MASK 0x30000u
//! Constant DIV4 - Divide by 4 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK5_DIV4 0x0
//! Constant DIV6 - Divide by 6 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK5_DIV6 0x1
//! Constant DIV8 - Divide by 8 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK5_DIV8 0x2
//! Constant DIV12 - Divide by 12 of VCO 2GHz
#define CONST_CGU_PLL0B_CFG1_PLL_CLK5_DIV12 0x3

//! Field PLL_OPD1 - PLL Output buffer power down
#define CGU_PLL0B_CFG1_PLL_OPD1_POS 18
//! Field PLL_OPD1 - PLL Output buffer power down
#define CGU_PLL0B_CFG1_PLL_OPD1_MASK 0x40000u
//! Constant CLK1_EN - Enable
#define CONST_CGU_PLL0B_CFG1_PLL_OPD1_CLK1_EN 0x0
//! Constant CLK1_DIS - Disable
#define CONST_CGU_PLL0B_CFG1_PLL_OPD1_CLK1_DIS 0x1

//! Field PLL_OPD2 - PLL Output buffer power down
#define CGU_PLL0B_CFG1_PLL_OPD2_POS 19
//! Field PLL_OPD2 - PLL Output buffer power down
#define CGU_PLL0B_CFG1_PLL_OPD2_MASK 0x80000u
//! Constant CLK2_EN - Enable
#define CONST_CGU_PLL0B_CFG1_PLL_OPD2_CLK2_EN 0x0
//! Constant CLK2_DIS - Disable
#define CONST_CGU_PLL0B_CFG1_PLL_OPD2_CLK2_DIS 0x1

//! Field PLL_OPD3 - PLL Output buffer power down
#define CGU_PLL0B_CFG1_PLL_OPD3_POS 20
//! Field PLL_OPD3 - PLL Output buffer power down
#define CGU_PLL0B_CFG1_PLL_OPD3_MASK 0x100000u
//! Constant CLK3_EN - Enable
#define CONST_CGU_PLL0B_CFG1_PLL_OPD3_CLK3_EN 0x0
//! Constant CLK3_DIS - Disable
#define CONST_CGU_PLL0B_CFG1_PLL_OPD3_CLK3_DIS 0x1

//! Field PLL_OPD4 - PLL Output buffer power down
#define CGU_PLL0B_CFG1_PLL_OPD4_POS 21
//! Field PLL_OPD4 - PLL Output buffer power down
#define CGU_PLL0B_CFG1_PLL_OPD4_MASK 0x200000u
//! Constant CLK4_EN - Enable
#define CONST_CGU_PLL0B_CFG1_PLL_OPD4_CLK4_EN 0x0
//! Constant CLK4_DIS - Disable
#define CONST_CGU_PLL0B_CFG1_PLL_OPD4_CLK4_DIS 0x1

//! Field PLL_OPD5 - PLL Output buffer power down
#define CGU_PLL0B_CFG1_PLL_OPD5_POS 22
//! Field PLL_OPD5 - PLL Output buffer power down
#define CGU_PLL0B_CFG1_PLL_OPD5_MASK 0x400000u
//! Constant CLK5_EN - Enable
#define CONST_CGU_PLL0B_CFG1_PLL_OPD5_CLK5_EN 0x0
//! Constant CLK5_DIS - Disable
#define CONST_CGU_PLL0B_CFG1_PLL_OPD5_CLK5_DIS 0x1

//! Field PLL_N_MODE - PLL N Mode
#define CGU_PLL0B_CFG1_PLL_N_MODE_POS 23
//! Field PLL_N_MODE - PLL N Mode
#define CGU_PLL0B_CFG1_PLL_N_MODE_MASK 0x800000u
//! Constant FRAC - Fractional PLL mode
#define CONST_CGU_PLL0B_CFG1_PLL_N_MODE_FRAC 0x0
//! Constant INT - Integer PLL mode
#define CONST_CGU_PLL0B_CFG1_PLL_N_MODE_INT 0x1

//! Field PLL_SSC - PLL Spread Spectrum Mode
#define CGU_PLL0B_CFG1_PLL_SSC_POS 24
//! Field PLL_SSC - PLL Spread Spectrum Mode
#define CGU_PLL0B_CFG1_PLL_SSC_MASK 0x1000000u
//! Constant ENABLE - Enable
#define CONST_CGU_PLL0B_CFG1_PLL_SSC_ENABLE 0x0
//! Constant DISABLE - Disable
#define CONST_CGU_PLL0B_CFG1_PLL_SSC_DISABLE 0x1

//! Field GLITCHFRE_DIS - Disable glitch free circuit during power up
#define CGU_PLL0B_CFG1_GLITCHFRE_DIS_POS 28
//! Field GLITCHFRE_DIS - Disable glitch free circuit during power up
#define CGU_PLL0B_CFG1_GLITCHFRE_DIS_MASK 0x10000000u
//! Constant EN - Default. Circuit is enabled.
#define CONST_CGU_PLL0B_CFG1_GLITCHFRE_DIS_EN 0x0
//! Constant DIS - Disable the circuit .may cause issue of clock outputs.
#define CONST_CGU_PLL0B_CFG1_GLITCHFRE_DIS_DIS 0x1

//! Field PLL_BW - PLL Bandwidth Select
#define CGU_PLL0B_CFG1_PLL_BW_POS 30
//! Field PLL_BW - PLL Bandwidth Select
#define CGU_PLL0B_CFG1_PLL_BW_MASK 0xC0000000u
//! Constant BW0 - Bandwidth 0
#define CONST_CGU_PLL0B_CFG1_PLL_BW_BW0 0x0
//! Constant BW1 - Bandwidth 1
#define CONST_CGU_PLL0B_CFG1_PLL_BW_BW1 0x1
//! Constant BW2 - Bandwidth 2
#define CONST_CGU_PLL0B_CFG1_PLL_BW_BW2 0x2
//! Constant BW3 - Bandwidth 3
#define CONST_CGU_PLL0B_CFG1_PLL_BW_BW3 0x3

//! @}

//! \defgroup PLL0B_SSC_CTRL Register PLL0B_SSC_CTRL - Spread Spectrum Control
//! @{

//! Register Offset (relative)
#define PLL0B_SSC_CTRL 0x3C
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0B_SSC_CTRL 0x1620003Cu

//! Register Reset Value
#define PLL0B_SSC_CTRL_RST 0x00000000u

//! Field EN - Enable
#define PLL0B_SSC_CTRL_EN_POS 0
//! Field EN - Enable
#define PLL0B_SSC_CTRL_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_PLL0B_SSC_CTRL_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_PLL0B_SSC_CTRL_EN_EN 0x1

//! Field INIT - Init
#define PLL0B_SSC_CTRL_INIT_POS 1
//! Field INIT - Init
#define PLL0B_SSC_CTRL_INIT_MASK 0x2u
//! Constant NONE - No action
#define CONST_PLL0B_SSC_CTRL_INIT_NONE 0x0
//! Constant START - Start
#define CONST_PLL0B_SSC_CTRL_INIT_START 0x1

//! Field LEN - SSC Code Size
#define PLL0B_SSC_CTRL_LEN_POS 2
//! Field LEN - SSC Code Size
#define PLL0B_SSC_CTRL_LEN_MASK 0x1Cu

//! Field DIV - SSC Divider
#define PLL0B_SSC_CTRL_DIV_POS 8
//! Field DIV - SSC Divider
#define PLL0B_SSC_CTRL_DIV_MASK 0xFF00u

//! @}

//! \defgroup PLL0B_SSC_SCALE Register PLL0B_SSC_SCALE - SSC Scaling
//! @{

//! Register Offset (relative)
#define PLL0B_SSC_SCALE 0x40
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0B_SSC_SCALE 0x16200040u

//! Register Reset Value
#define PLL0B_SSC_SCALE_RST 0x00000000u

//! Field SCALE - SSC to ROPLL scaling
#define PLL0B_SSC_SCALE_SCALE_POS 0
//! Field SCALE - SSC to ROPLL scaling
#define PLL0B_SSC_SCALE_SCALE_MASK 0x7u

//! Field INTCOEF - Interpolator Coefficient
#define PLL0B_SSC_SCALE_INTCOEF_POS 3
//! Field INTCOEF - Interpolator Coefficient
#define PLL0B_SSC_SCALE_INTCOEF_MASK 0x78u

//! Field GSCALE - SSC to GPHY IP Scaling
#define PLL0B_SSC_SCALE_GSCALE_POS 8
//! Field GSCALE - SSC to GPHY IP Scaling
#define PLL0B_SSC_SCALE_GSCALE_MASK 0xFF00u

//! @}

//! \defgroup PLL0B_COEF_0 Register PLL0B_COEF_0 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0B_COEF_0 0x44
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0B_COEF_0 0x16200044u

//! Register Reset Value
#define PLL0B_COEF_0_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0B_COEF_0_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0B_COEF_0_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0B_COEF_1 Register PLL0B_COEF_1 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0B_COEF_1 0x48
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0B_COEF_1 0x16200048u

//! Register Reset Value
#define PLL0B_COEF_1_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0B_COEF_1_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0B_COEF_1_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0B_COEF_2 Register PLL0B_COEF_2 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0B_COEF_2 0x4C
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0B_COEF_2 0x1620004Cu

//! Register Reset Value
#define PLL0B_COEF_2_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0B_COEF_2_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0B_COEF_2_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0B_COEF_3 Register PLL0B_COEF_3 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0B_COEF_3 0x50
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0B_COEF_3 0x16200050u

//! Register Reset Value
#define PLL0B_COEF_3_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0B_COEF_3_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0B_COEF_3_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0B_COEF_4 Register PLL0B_COEF_4 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0B_COEF_4 0x54
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0B_COEF_4 0x16200054u

//! Register Reset Value
#define PLL0B_COEF_4_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0B_COEF_4_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0B_COEF_4_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0B_COEF_5 Register PLL0B_COEF_5 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0B_COEF_5 0x58
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0B_COEF_5 0x16200058u

//! Register Reset Value
#define PLL0B_COEF_5_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0B_COEF_5_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0B_COEF_5_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0B_COEF_6 Register PLL0B_COEF_6 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0B_COEF_6 0x5C
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0B_COEF_6 0x1620005Cu

//! Register Reset Value
#define PLL0B_COEF_6_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0B_COEF_6_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0B_COEF_6_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL0B_COEF_7 Register PLL0B_COEF_7 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL0B_COEF_7 0x60
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL0B_COEF_7 0x16200060u

//! Register Reset Value
#define PLL0B_COEF_7_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL0B_COEF_7_DATA_POS 0
//! Field DATA - Coefficient
#define PLL0B_COEF_7_DATA_MASK 0xFFFFu

//! @}

//! \defgroup CGU_PLL1_CFG0 Register CGU_PLL1_CFG0 - PLL1 Configuration Register 0
//! @{

//! Register Offset (relative)
#define CGU_PLL1_CFG0 0x64
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_PLL1_CFG0 0x16200064u

//! Register Reset Value
#define CGU_PLL1_CFG0_RST 0x1F6FD2EBu

//! Field PLL_RST - PLL Reset
#define CGU_PLL1_CFG0_PLL_RST_POS 0
//! Field PLL_RST - PLL Reset
#define CGU_PLL1_CFG0_PLL_RST_MASK 0x1u
//! Constant RESET - Reset request
#define CONST_CGU_PLL1_CFG0_PLL_RST_RESET 0x0
//! Constant NONE - No action
#define CONST_CGU_PLL1_CFG0_PLL_RST_NONE 0x1

//! Field PLL_L - PLL Lock Status
#define CGU_PLL1_CFG0_PLL_L_POS 1
//! Field PLL_L - PLL Lock Status
#define CGU_PLL1_CFG0_PLL_L_MASK 0x2u
//! Constant FREE - Free running
#define CONST_CGU_PLL1_CFG0_PLL_L_FREE 0x0
//! Constant LOCK - Lock
#define CONST_CGU_PLL1_CFG0_PLL_L_LOCK 0x1

//! Field PLL_N - PLL0 N Divider
#define CGU_PLL1_CFG0_PLL_N_POS 2
//! Field PLL_N - PLL0 N Divider
#define CGU_PLL1_CFG0_PLL_N_MASK 0x1FCu

//! Field PLL_K - PLL0 Fractional K Divider
#define CGU_PLL1_CFG0_PLL_K_POS 9
//! Field PLL_K - PLL0 Fractional K Divider
#define CGU_PLL1_CFG0_PLL_K_MASK 0x3FFFFE00u

//! Field PLL_BP - PLL Bypass
#define CGU_PLL1_CFG0_PLL_BP_POS 30
//! Field PLL_BP - PLL Bypass
#define CGU_PLL1_CFG0_PLL_BP_MASK 0x40000000u
//! Constant DISABLE - Disable
#define CONST_CGU_PLL1_CFG0_PLL_BP_DISABLE 0x0
//! Constant ENABLE - Enable
#define CONST_CGU_PLL1_CFG0_PLL_BP_ENABLE 0x1

//! Field PLL_BUFF_OUT - PLL Output buffer
#define CGU_PLL1_CFG0_PLL_BUFF_OUT_POS 31
//! Field PLL_BUFF_OUT - PLL Output buffer
#define CGU_PLL1_CFG0_PLL_BUFF_OUT_MASK 0x80000000u
//! Constant DIS - Disable
#define CONST_CGU_PLL1_CFG0_PLL_BUFF_OUT_DIS 0x0
//! Constant EN - Enable
#define CONST_CGU_PLL1_CFG0_PLL_BUFF_OUT_EN 0x1

//! @}

//! \defgroup CGU_PLL1_CFG1 Register CGU_PLL1_CFG1 - PLL1 Configuration Register 1
//! @{

//! Register Offset (relative)
#define CGU_PLL1_CFG1 0x68
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_PLL1_CFG1 0x16200068u

//! Register Reset Value
#define CGU_PLL1_CFG1_RST 0x407000E5u

//! Field PLL_CLK1 - PLL Clock Output 1
#define CGU_PLL1_CFG1_PLL_CLK1_POS 0
//! Field PLL_CLK1 - PLL Clock Output 1
#define CGU_PLL1_CFG1_PLL_CLK1_MASK 0xFu
//! Constant GND - ref clk
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK1_DIV64 0xF

//! Field PLL_CLK2 - PLL Clock Output 2
#define CGU_PLL1_CFG1_PLL_CLK2_POS 4
//! Field PLL_CLK2 - PLL Clock Output 2
#define CGU_PLL1_CFG1_PLL_CLK2_MASK 0xF0u
//! Constant GND - ref clk
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK2_DIV64 0xF

//! Field PLL_CLK3 - PLL Clock Output 3
#define CGU_PLL1_CFG1_PLL_CLK3_POS 8
//! Field PLL_CLK3 - PLL Clock Output 3
#define CGU_PLL1_CFG1_PLL_CLK3_MASK 0xF00u
//! Constant GND - ref clk
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK3_DIV64 0xF

//! Field PLL_CLK4 - PLL Clock Output 4
#define CGU_PLL1_CFG1_PLL_CLK4_POS 12
//! Field PLL_CLK4 - PLL Clock Output 4
#define CGU_PLL1_CFG1_PLL_CLK4_MASK 0xF000u
//! Constant GND - ref clk
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_GND 0x0
//! Constant DIV2 - Divide by 2 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV2 0x1
//! Constant DIV3 - Divide by 3 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV3 0x2
//! Constant DIV4 - Divide by 4 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV4 0x3
//! Constant DIV5 - Divide by 5 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV5 0x4
//! Constant DIV6 - Divide by 6 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV6 0x5
//! Constant DIV8 - Divide by 8 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV8 0x6
//! Constant DIV10 - Divide by 10 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV10 0x7
//! Constant DIV12 - Divide by 12 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV12 0x8
//! Constant DIV16 - Divide by 16 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV16 0x9
//! Constant DIV20 - Divide by 20 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV20 0xA
//! Constant DIV24 - Divide by 24 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV24 0xB
//! Constant DIV32 - Divide by 32 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV32 0xC
//! Constant DIV40 - Divide by 40 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV40 0xD
//! Constant DIV48 - Divide by 48 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV48 0xE
//! Constant DIV64 - Divide by 64 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK4_DIV64 0xF

//! Field PLL_CLK5 - PLL Clock Output 5
#define CGU_PLL1_CFG1_PLL_CLK5_POS 16
//! Field PLL_CLK5 - PLL Clock Output 5
#define CGU_PLL1_CFG1_PLL_CLK5_MASK 0x30000u
//! Constant DIV4 - Divide by 4 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK5_DIV4 0x0
//! Constant DIV6 - Divide by 6 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK5_DIV6 0x1
//! Constant DIV8 - Divide by 8 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK5_DIV8 0x2
//! Constant DIV12 - Divide by 12 of VCO 2.359296GHz
#define CONST_CGU_PLL1_CFG1_PLL_CLK5_DIV12 0x3

//! Field PLL_OPD1 - PLL Output buffer power down
#define CGU_PLL1_CFG1_PLL_OPD1_POS 18
//! Field PLL_OPD1 - PLL Output buffer power down
#define CGU_PLL1_CFG1_PLL_OPD1_MASK 0x40000u
//! Constant CLK1_EN - Enable
#define CONST_CGU_PLL1_CFG1_PLL_OPD1_CLK1_EN 0x0
//! Constant CLK1_DIS - Disable
#define CONST_CGU_PLL1_CFG1_PLL_OPD1_CLK1_DIS 0x1

//! Field PLL_OPD2 - PLL Output buffer power down
#define CGU_PLL1_CFG1_PLL_OPD2_POS 19
//! Field PLL_OPD2 - PLL Output buffer power down
#define CGU_PLL1_CFG1_PLL_OPD2_MASK 0x80000u
//! Constant CLK2_EN - Enable
#define CONST_CGU_PLL1_CFG1_PLL_OPD2_CLK2_EN 0x0
//! Constant CLK2_DIS - Disable
#define CONST_CGU_PLL1_CFG1_PLL_OPD2_CLK2_DIS 0x1

//! Field PLL_OPD3 - PLL Output buffer power down
#define CGU_PLL1_CFG1_PLL_OPD3_POS 20
//! Field PLL_OPD3 - PLL Output buffer power down
#define CGU_PLL1_CFG1_PLL_OPD3_MASK 0x100000u
//! Constant CLK3_EN - Enable
#define CONST_CGU_PLL1_CFG1_PLL_OPD3_CLK3_EN 0x0
//! Constant CLK3_DIS - Disable
#define CONST_CGU_PLL1_CFG1_PLL_OPD3_CLK3_DIS 0x1

//! Field PLL_OPD4 - PLL Output buffer power down
#define CGU_PLL1_CFG1_PLL_OPD4_POS 21
//! Field PLL_OPD4 - PLL Output buffer power down
#define CGU_PLL1_CFG1_PLL_OPD4_MASK 0x200000u
//! Constant CLK4_EN - Enable
#define CONST_CGU_PLL1_CFG1_PLL_OPD4_CLK4_EN 0x0
//! Constant CLK4_DIS - Disable
#define CONST_CGU_PLL1_CFG1_PLL_OPD4_CLK4_DIS 0x1

//! Field PLL_OPD5 - PLL Output buffer power down
#define CGU_PLL1_CFG1_PLL_OPD5_POS 22
//! Field PLL_OPD5 - PLL Output buffer power down
#define CGU_PLL1_CFG1_PLL_OPD5_MASK 0x400000u
//! Constant CLK5_EN - Enable
#define CONST_CGU_PLL1_CFG1_PLL_OPD5_CLK5_EN 0x0
//! Constant CLK5_DIS - Disable
#define CONST_CGU_PLL1_CFG1_PLL_OPD5_CLK5_DIS 0x1

//! Field PLL_N_MODE - PLL N Mode
#define CGU_PLL1_CFG1_PLL_N_MODE_POS 23
//! Field PLL_N_MODE - PLL N Mode
#define CGU_PLL1_CFG1_PLL_N_MODE_MASK 0x800000u
//! Constant FRAC - Fractional PLL mode
#define CONST_CGU_PLL1_CFG1_PLL_N_MODE_FRAC 0x0
//! Constant INT - Integer PLL mode
#define CONST_CGU_PLL1_CFG1_PLL_N_MODE_INT 0x1

//! Field PLL_SSC - PLL Spread Spectrum Mode
#define CGU_PLL1_CFG1_PLL_SSC_POS 24
//! Field PLL_SSC - PLL Spread Spectrum Mode
#define CGU_PLL1_CFG1_PLL_SSC_MASK 0x1000000u
//! Constant ENABLE - Enable
#define CONST_CGU_PLL1_CFG1_PLL_SSC_ENABLE 0x0
//! Constant DISABLE - Disable
#define CONST_CGU_PLL1_CFG1_PLL_SSC_DISABLE 0x1

//! Field GLITCHFRE_DIS - Disable glitch free circuit during power up
#define CGU_PLL1_CFG1_GLITCHFRE_DIS_POS 28
//! Field GLITCHFRE_DIS - Disable glitch free circuit during power up
#define CGU_PLL1_CFG1_GLITCHFRE_DIS_MASK 0x10000000u
//! Constant EN - Default. Circuit is enabled.
#define CONST_CGU_PLL1_CFG1_GLITCHFRE_DIS_EN 0x0
//! Constant DIS - Disable the circuit .may cause issue of clock outputs.
#define CONST_CGU_PLL1_CFG1_GLITCHFRE_DIS_DIS 0x1

//! Field PLL_BW - PLL Bandwidth Select
#define CGU_PLL1_CFG1_PLL_BW_POS 30
//! Field PLL_BW - PLL Bandwidth Select
#define CGU_PLL1_CFG1_PLL_BW_MASK 0xC0000000u
//! Constant BW0 - Bandwidth 0
#define CONST_CGU_PLL1_CFG1_PLL_BW_BW0 0x0
//! Constant BW1 - Bandwidth 1
#define CONST_CGU_PLL1_CFG1_PLL_BW_BW1 0x1
//! Constant BW2 - Bandwidth 2
#define CONST_CGU_PLL1_CFG1_PLL_BW_BW2 0x2
//! Constant BW3 - Bandwidth 3
#define CONST_CGU_PLL1_CFG1_PLL_BW_BW3 0x3

//! @}

//! \defgroup PLL1_SSC_CTRL Register PLL1_SSC_CTRL - Spread Spectrum Control
//! @{

//! Register Offset (relative)
#define PLL1_SSC_CTRL 0x6C
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL1_SSC_CTRL 0x1620006Cu

//! Register Reset Value
#define PLL1_SSC_CTRL_RST 0x00000000u

//! Field EN - Enable
#define PLL1_SSC_CTRL_EN_POS 0
//! Field EN - Enable
#define PLL1_SSC_CTRL_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_PLL1_SSC_CTRL_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_PLL1_SSC_CTRL_EN_EN 0x1

//! Field INIT - Init
#define PLL1_SSC_CTRL_INIT_POS 1
//! Field INIT - Init
#define PLL1_SSC_CTRL_INIT_MASK 0x2u
//! Constant NONE - No action
#define CONST_PLL1_SSC_CTRL_INIT_NONE 0x0
//! Constant START - Start
#define CONST_PLL1_SSC_CTRL_INIT_START 0x1

//! Field LEN - SSC Code Size
#define PLL1_SSC_CTRL_LEN_POS 2
//! Field LEN - SSC Code Size
#define PLL1_SSC_CTRL_LEN_MASK 0x1Cu

//! Field DIV - SSC Divider
#define PLL1_SSC_CTRL_DIV_POS 8
//! Field DIV - SSC Divider
#define PLL1_SSC_CTRL_DIV_MASK 0xFF00u

//! @}

//! \defgroup PLL1_SSC_SCALE Register PLL1_SSC_SCALE - SSC Scaling
//! @{

//! Register Offset (relative)
#define PLL1_SSC_SCALE 0x70
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL1_SSC_SCALE 0x16200070u

//! Register Reset Value
#define PLL1_SSC_SCALE_RST 0x00000000u

//! Field SCALE - SSC to ROPLL scaling
#define PLL1_SSC_SCALE_SCALE_POS 0
//! Field SCALE - SSC to ROPLL scaling
#define PLL1_SSC_SCALE_SCALE_MASK 0x7u

//! Field INTCOEF - Interpolator Coefficient
#define PLL1_SSC_SCALE_INTCOEF_POS 4
//! Field INTCOEF - Interpolator Coefficient
#define PLL1_SSC_SCALE_INTCOEF_MASK 0xF0u

//! Field GSCALE - SSC to GPHY IP Scaling
#define PLL1_SSC_SCALE_GSCALE_POS 8
//! Field GSCALE - SSC to GPHY IP Scaling
#define PLL1_SSC_SCALE_GSCALE_MASK 0xFF00u

//! @}

//! \defgroup PLL1_COEF_0 Register PLL1_COEF_0 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL1_COEF_0 0x74
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL1_COEF_0 0x16200074u

//! Register Reset Value
#define PLL1_COEF_0_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL1_COEF_0_DATA_POS 0
//! Field DATA - Coefficient
#define PLL1_COEF_0_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL1_COEF_1 Register PLL1_COEF_1 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL1_COEF_1 0x78
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL1_COEF_1 0x16200078u

//! Register Reset Value
#define PLL1_COEF_1_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL1_COEF_1_DATA_POS 0
//! Field DATA - Coefficient
#define PLL1_COEF_1_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL1_COEF_2 Register PLL1_COEF_2 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL1_COEF_2 0x7C
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL1_COEF_2 0x1620007Cu

//! Register Reset Value
#define PLL1_COEF_2_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL1_COEF_2_DATA_POS 0
//! Field DATA - Coefficient
#define PLL1_COEF_2_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL1_COEF_3 Register PLL1_COEF_3 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL1_COEF_3 0x80
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL1_COEF_3 0x16200080u

//! Register Reset Value
#define PLL1_COEF_3_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL1_COEF_3_DATA_POS 0
//! Field DATA - Coefficient
#define PLL1_COEF_3_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL1_COEF_4 Register PLL1_COEF_4 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL1_COEF_4 0x84
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL1_COEF_4 0x16200084u

//! Register Reset Value
#define PLL1_COEF_4_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL1_COEF_4_DATA_POS 0
//! Field DATA - Coefficient
#define PLL1_COEF_4_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL1_COEF_5 Register PLL1_COEF_5 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL1_COEF_5 0x88
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL1_COEF_5 0x16200088u

//! Register Reset Value
#define PLL1_COEF_5_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL1_COEF_5_DATA_POS 0
//! Field DATA - Coefficient
#define PLL1_COEF_5_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL1_COEF_6 Register PLL1_COEF_6 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL1_COEF_6 0x8C
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL1_COEF_6 0x1620008Cu

//! Register Reset Value
#define PLL1_COEF_6_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL1_COEF_6_DATA_POS 0
//! Field DATA - Coefficient
#define PLL1_COEF_6_DATA_MASK 0xFFFFu

//! @}

//! \defgroup PLL1_COEF_7 Register PLL1_COEF_7 - Coefficient
//! @{

//! Register Offset (relative)
#define PLL1_COEF_7 0x90
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_PLL1_COEF_7 0x16200090u

//! Register Reset Value
#define PLL1_COEF_7_RST 0x00000000u

//! Field DATA - Coefficient
#define PLL1_COEF_7_DATA_POS 0
//! Field DATA - Coefficient
#define PLL1_COEF_7_DATA_MASK 0xFFFFu

//! @}

//! \defgroup LCPLL_CFG0 Register LCPLL_CFG0 - LC PLL Configuration Register 0
//! @{

//! Register Offset (relative)
#define LCPLL_CFG0 0x94
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_CFG0 0x16200094u

//! Register Reset Value
#define LCPLL_CFG0_RST 0x00000193u

//! Field PLL_RST - PLL Reset
#define LCPLL_CFG0_PLL_RST_POS 0
//! Field PLL_RST - PLL Reset
#define LCPLL_CFG0_PLL_RST_MASK 0x1u
//! Constant RESET - Reset request
#define CONST_LCPLL_CFG0_PLL_RST_RESET 0x0
//! Constant NONE - No action
#define CONST_LCPLL_CFG0_PLL_RST_NONE 0x1

//! Field PLL_L - PLL Lock Status
#define LCPLL_CFG0_PLL_L_POS 1
//! Field PLL_L - PLL Lock Status
#define LCPLL_CFG0_PLL_L_MASK 0x2u
//! Constant FREE - Free running
#define CONST_LCPLL_CFG0_PLL_L_FREE 0x0
//! Constant LOCK - Locked
#define CONST_LCPLL_CFG0_PLL_L_LOCK 0x1

//! Field PLL_N - LCPLL N Divider
#define LCPLL_CFG0_PLL_N_POS 2
//! Field PLL_N - LCPLL N Divider
#define LCPLL_CFG0_PLL_N_MASK 0x3FCu

//! Field PLL_K - LCLL Fractional K Divider
#define LCPLL_CFG0_PLL_K_POS 10
//! Field PLL_K - LCLL Fractional K Divider
#define LCPLL_CFG0_PLL_K_MASK 0x7FFFFC00u

//! Field PLL_BP - PLL Bypass
#define LCPLL_CFG0_PLL_BP_POS 31
//! Field PLL_BP - PLL Bypass
#define LCPLL_CFG0_PLL_BP_MASK 0x80000000u
//! Constant DISABLE - Disable
#define CONST_LCPLL_CFG0_PLL_BP_DISABLE 0x0
//! Constant ENABLE - Enable
#define CONST_LCPLL_CFG0_PLL_BP_ENABLE 0x1

//! @}

//! \defgroup LCPLL_CFG1 Register LCPLL_CFG1 - LC PLL Configuration Register 1
//! @{

//! Register Offset (relative)
#define LCPLL_CFG1 0x98
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_CFG1 0x16200098u

//! Register Reset Value
#define LCPLL_CFG1_RST 0x01003004u

//! Field PLL_BUFF_OUT - PLL Output buffer
#define LCPLL_CFG1_PLL_BUFF_OUT_POS 0
//! Field PLL_BUFF_OUT - PLL Output buffer
#define LCPLL_CFG1_PLL_BUFF_OUT_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LCPLL_CFG1_PLL_BUFF_OUT_DIS 0x0
//! Constant EN - Enable
#define CONST_LCPLL_CFG1_PLL_BUFF_OUT_EN 0x1

//! Field PLL_MODE - PLL MODE
#define LCPLL_CFG1_PLL_MODE_POS 1
//! Field PLL_MODE - PLL MODE
#define LCPLL_CFG1_PLL_MODE_MASK 0x6u
//! Constant GND - Ground
#define CONST_LCPLL_CFG1_PLL_MODE_GND 0x0
//! Constant DIV2 - mode
#define CONST_LCPLL_CFG1_PLL_MODE_DIV2 0x1
//! Constant DIV3 - mode
#define CONST_LCPLL_CFG1_PLL_MODE_DIV3 0x2
//! Constant DIV4 - mode
#define CONST_LCPLL_CFG1_PLL_MODE_DIV4 0x3

//! Field PLL_INPUT_CLK - PLL INPUT Clock Selection
#define LCPLL_CFG1_PLL_INPUT_CLK_POS 3
//! Field PLL_INPUT_CLK - PLL INPUT Clock Selection
#define LCPLL_CFG1_PLL_INPUT_CLK_MASK 0x18u
//! Constant M40 - Input Clock
#define CONST_LCPLL_CFG1_PLL_INPUT_CLK_M40 0x0
//! Constant DIV2 - Input clock
#define CONST_LCPLL_CFG1_PLL_INPUT_CLK_DIV2 0x1
//! Constant DIV3 - Input clock
#define CONST_LCPLL_CFG1_PLL_INPUT_CLK_DIV3 0x2
//! Constant DIV4 - Input clock
#define CONST_LCPLL_CFG1_PLL_INPUT_CLK_DIV4 0x3

//! Field PLL_CLK4 - PLL Clock Output 4
#define LCPLL_CFG1_PLL_CLK4_POS 12
//! Field PLL_CLK4 - PLL Clock Output 4
#define LCPLL_CFG1_PLL_CLK4_MASK 0x3000u
//! Constant GND - Ground
#define CONST_LCPLL_CFG1_PLL_CLK4_GND 0x0
//! Constant DIV2 - Divide clock
#define CONST_LCPLL_CFG1_PLL_CLK4_DIV2 0x1
//! Constant DIV3 - Divide clock
#define CONST_LCPLL_CFG1_PLL_CLK4_DIV3 0x2
//! Constant DIV4 - Divide clock
#define CONST_LCPLL_CFG1_PLL_CLK4_DIV4 0x3

//! Field PLL_NK_SRC - PLL N K divider Source
#define LCPLL_CFG1_PLL_NK_SRC_POS 18
//! Field PLL_NK_SRC - PLL N K divider Source
#define LCPLL_CFG1_PLL_NK_SRC_MASK 0x40000u
//! Constant CLK1_EN - user programming
#define CONST_LCPLL_CFG1_PLL_NK_SRC_CLK1_EN 0x0
//! Constant CLK1_DIS - Internal Table
#define CONST_LCPLL_CFG1_PLL_NK_SRC_CLK1_DIS 0x1

//! Field PLL_LOCK_OV - PLL Lock override
#define LCPLL_CFG1_PLL_LOCK_OV_POS 19
//! Field PLL_LOCK_OV - PLL Lock override
#define LCPLL_CFG1_PLL_LOCK_OV_MASK 0x80000u
//! Constant OV_DIS - auto detection
#define CONST_LCPLL_CFG1_PLL_LOCK_OV_OV_DIS 0x0
//! Constant OV_EN - override
#define CONST_LCPLL_CFG1_PLL_LOCK_OV_OV_EN 0x1

//! Field PLL_POK_OV - PLL POK OVER RIDE
#define LCPLL_CFG1_PLL_POK_OV_POS 20
//! Field PLL_POK_OV - PLL POK OVER RIDE
#define LCPLL_CFG1_PLL_POK_OV_MASK 0x100000u
//! Constant POK_NOV - no override
#define CONST_LCPLL_CFG1_PLL_POK_OV_POK_NOV 0x0
//! Constant POK_OV - over ride
#define CONST_LCPLL_CFG1_PLL_POK_OV_POK_OV 0x1

//! Field PLL_OPD4 - PLL Output buffer power down
#define LCPLL_CFG1_PLL_OPD4_POS 21
//! Field PLL_OPD4 - PLL Output buffer power down
#define LCPLL_CFG1_PLL_OPD4_MASK 0x200000u
//! Constant CLK4_EN - Enable
#define CONST_LCPLL_CFG1_PLL_OPD4_CLK4_EN 0x0
//! Constant CLK4_DIS - Disable
#define CONST_LCPLL_CFG1_PLL_OPD4_CLK4_DIS 0x1

//! Field PLL_INPUT_MD - PLL input mode
#define LCPLL_CFG1_PLL_INPUT_MD_POS 22
//! Field PLL_INPUT_MD - PLL input mode
#define LCPLL_CFG1_PLL_INPUT_MD_MASK 0x400000u
//! Constant CML - CML
#define CONST_LCPLL_CFG1_PLL_INPUT_MD_CML 0x0
//! Constant CMOS - CMOS
#define CONST_LCPLL_CFG1_PLL_INPUT_MD_CMOS 0x1

//! Field PLL_N_MODE - PLL N Mode
#define LCPLL_CFG1_PLL_N_MODE_POS 23
//! Field PLL_N_MODE - PLL N Mode
#define LCPLL_CFG1_PLL_N_MODE_MASK 0x800000u
//! Constant FRAC - Fractional PLL mode
#define CONST_LCPLL_CFG1_PLL_N_MODE_FRAC 0x0
//! Constant INT - Integer PLL mode
#define CONST_LCPLL_CFG1_PLL_N_MODE_INT 0x1

//! Field PLL_SSC - PLL Spread Spectrum Mode
#define LCPLL_CFG1_PLL_SSC_POS 24
//! Field PLL_SSC - PLL Spread Spectrum Mode
#define LCPLL_CFG1_PLL_SSC_MASK 0x1000000u
//! Constant ENABLE - Enable
#define CONST_LCPLL_CFG1_PLL_SSC_ENABLE 0x0
//! Constant DISABLE - Disable
#define CONST_LCPLL_CFG1_PLL_SSC_DISABLE 0x1

//! Field GLITCHFRE_DIS - Disable glitch free circuit during power up
#define LCPLL_CFG1_GLITCHFRE_DIS_POS 28
//! Field GLITCHFRE_DIS - Disable glitch free circuit during power up
#define LCPLL_CFG1_GLITCHFRE_DIS_MASK 0x10000000u
//! Constant EN - Default. Circuit is enabled.
#define CONST_LCPLL_CFG1_GLITCHFRE_DIS_EN 0x0
//! Constant DIS - Disable the circuit .may cause issue of clock outputs.
#define CONST_LCPLL_CFG1_GLITCHFRE_DIS_DIS 0x1

//! Field IP_OKB_OVR - Internal Power On Bias Okay Override
#define LCPLL_CFG1_IP_OKB_OVR_POS 31
//! Field IP_OKB_OVR - Internal Power On Bias Okay Override
#define LCPLL_CFG1_IP_OKB_OVR_MASK 0x80000000u
//! Constant NO - No Override , POK Bias check is always enabled.
#define CONST_LCPLL_CFG1_IP_OKB_OVR_NO 0x0
//! Constant YES - Override internal POK , internal bias checking is disabled.
#define CONST_LCPLL_CFG1_IP_OKB_OVR_YES 0x1

//! @}

//! \defgroup LCPLL_SSC_CTRL Register LCPLL_SSC_CTRL - Spread Spectrum Control
//! @{

//! Register Offset (relative)
#define LCPLL_SSC_CTRL 0x9C
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_SSC_CTRL 0x1620009Cu

//! Register Reset Value
#define LCPLL_SSC_CTRL_RST 0x00000000u

//! Field EN - Enable
#define LCPLL_SSC_CTRL_EN_POS 0
//! Field EN - Enable
#define LCPLL_SSC_CTRL_EN_MASK 0x1u
//! Constant DIS - Disable
#define CONST_LCPLL_SSC_CTRL_EN_DIS 0x0
//! Constant EN - Enable
#define CONST_LCPLL_SSC_CTRL_EN_EN 0x1

//! Field INIT - Init
#define LCPLL_SSC_CTRL_INIT_POS 1
//! Field INIT - Init
#define LCPLL_SSC_CTRL_INIT_MASK 0x2u
//! Constant NONE - No action
#define CONST_LCPLL_SSC_CTRL_INIT_NONE 0x0
//! Constant START - Start
#define CONST_LCPLL_SSC_CTRL_INIT_START 0x1

//! Field LEN - SSC Code Size
#define LCPLL_SSC_CTRL_LEN_POS 2
//! Field LEN - SSC Code Size
#define LCPLL_SSC_CTRL_LEN_MASK 0x1Cu

//! Field DIV - SSC Divider
#define LCPLL_SSC_CTRL_DIV_POS 8
//! Field DIV - SSC Divider
#define LCPLL_SSC_CTRL_DIV_MASK 0xFF00u

//! @}

//! \defgroup LCPLL_SSC_SCALE Register LCPLL_SSC_SCALE - SSC Scaling of LCPLL
//! @{

//! Register Offset (relative)
#define LCPLL_SSC_SCALE 0xA0
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_SSC_SCALE 0x162000A0u

//! Register Reset Value
#define LCPLL_SSC_SCALE_RST 0x00000000u

//! Field SCALE - SSC to LCPLL scaling
#define LCPLL_SSC_SCALE_SCALE_POS 0
//! Field SCALE - SSC to LCPLL scaling
#define LCPLL_SSC_SCALE_SCALE_MASK 0x7u

//! Field INTCOEF - Interpolator Coefficient
#define LCPLL_SSC_SCALE_INTCOEF_POS 4
//! Field INTCOEF - Interpolator Coefficient
#define LCPLL_SSC_SCALE_INTCOEF_MASK 0xF0u

//! Field GSCALE - SSC to GPHY IP Scaling
#define LCPLL_SSC_SCALE_GSCALE_POS 8
//! Field GSCALE - SSC to GPHY IP Scaling
#define LCPLL_SSC_SCALE_GSCALE_MASK 0xFF00u

//! @}

//! \defgroup LCPLL_COEF_0 Register LCPLL_COEF_0 - Coefficient of LCPLL
//! @{

//! Register Offset (relative)
#define LCPLL_COEF_0 0xA4
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_COEF_0 0x162000A4u

//! Register Reset Value
#define LCPLL_COEF_0_RST 0x00000000u

//! Field DATA - Coefficient
#define LCPLL_COEF_0_DATA_POS 0
//! Field DATA - Coefficient
#define LCPLL_COEF_0_DATA_MASK 0xFFFFu

//! @}

//! \defgroup LCPLL_COEF_1 Register LCPLL_COEF_1 - Coefficient of LCPLL
//! @{

//! Register Offset (relative)
#define LCPLL_COEF_1 0xA8
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_COEF_1 0x162000A8u

//! Register Reset Value
#define LCPLL_COEF_1_RST 0x00000000u

//! Field DATA - Coefficient
#define LCPLL_COEF_1_DATA_POS 0
//! Field DATA - Coefficient
#define LCPLL_COEF_1_DATA_MASK 0xFFFFu

//! @}

//! \defgroup LCPLL_COEF_2 Register LCPLL_COEF_2 - Coefficient of LCPLL
//! @{

//! Register Offset (relative)
#define LCPLL_COEF_2 0xAC
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_COEF_2 0x162000ACu

//! Register Reset Value
#define LCPLL_COEF_2_RST 0x00000000u

//! Field DATA - Coefficient
#define LCPLL_COEF_2_DATA_POS 0
//! Field DATA - Coefficient
#define LCPLL_COEF_2_DATA_MASK 0xFFFFu

//! @}

//! \defgroup LCPLL_COEF_3 Register LCPLL_COEF_3 - Coefficient of LCPLL
//! @{

//! Register Offset (relative)
#define LCPLL_COEF_3 0xB0
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_COEF_3 0x162000B0u

//! Register Reset Value
#define LCPLL_COEF_3_RST 0x00000000u

//! Field DATA - Coefficient
#define LCPLL_COEF_3_DATA_POS 0
//! Field DATA - Coefficient
#define LCPLL_COEF_3_DATA_MASK 0xFFFFu

//! @}

//! \defgroup LCPLL_COEF_4 Register LCPLL_COEF_4 - Coefficient of LCPLL
//! @{

//! Register Offset (relative)
#define LCPLL_COEF_4 0xB4
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_COEF_4 0x162000B4u

//! Register Reset Value
#define LCPLL_COEF_4_RST 0x00000000u

//! Field DATA - Coefficient
#define LCPLL_COEF_4_DATA_POS 0
//! Field DATA - Coefficient
#define LCPLL_COEF_4_DATA_MASK 0xFFFFu

//! @}

//! \defgroup LCPLL_COEF_5 Register LCPLL_COEF_5 - Coefficient of LCPLL
//! @{

//! Register Offset (relative)
#define LCPLL_COEF_5 0xB8
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_COEF_5 0x162000B8u

//! Register Reset Value
#define LCPLL_COEF_5_RST 0x00000000u

//! Field DATA - Coefficient
#define LCPLL_COEF_5_DATA_POS 0
//! Field DATA - Coefficient
#define LCPLL_COEF_5_DATA_MASK 0xFFFFu

//! @}

//! \defgroup LCPLL_COEF_6 Register LCPLL_COEF_6 - Coefficient of LCPLL
//! @{

//! Register Offset (relative)
#define LCPLL_COEF_6 0xBC
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_COEF_6 0x162000BCu

//! Register Reset Value
#define LCPLL_COEF_6_RST 0x00000000u

//! Field DATA - Coefficient
#define LCPLL_COEF_6_DATA_POS 0
//! Field DATA - Coefficient
#define LCPLL_COEF_6_DATA_MASK 0xFFFFu

//! @}

//! \defgroup LCPLL_COEF_7 Register LCPLL_COEF_7 - Coefficient of LCPLL
//! @{

//! Register Offset (relative)
#define LCPLL_COEF_7 0xC0
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_LCPLL_COEF_7 0x162000C0u

//! Register Reset Value
#define LCPLL_COEF_7_RST 0x00000000u

//! Field DATA - Coefficient
#define LCPLL_COEF_7_DATA_POS 0
//! Field DATA - Coefficient
#define LCPLL_COEF_7_DATA_MASK 0xFFFFu

//! @}

//! \defgroup CGU_IF_CLK Register CGU_IF_CLK - Interface Clock Register
//! @{

//! Register Offset (relative)
#define CGU_IF_CLK 0xC4
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_IF_CLK 0x162000C4u

//! Register Reset Value
#define CGU_IF_CLK_RST 0x00000084u

//! Field GPHY_CLK_M - Clock MODE to GPHY CDB
#define CGU_IF_CLK_GPHY_CLK_M_POS 0
//! Field GPHY_CLK_M - Clock MODE to GPHY CDB
#define CGU_IF_CLK_GPHY_CLK_M_MASK 0x1u
//! Constant CML - CML mode
#define CONST_CGU_IF_CLK_GPHY_CLK_M_CML 0x0
//! Constant CMOS - CMOS Single Ended Mode
#define CONST_CGU_IF_CLK_GPHY_CLK_M_CMOS 0x1

//! Field GPHY_SEL - GPHY Clock Source
#define CGU_IF_CLK_GPHY_SEL_POS 2
//! Field GPHY_SEL - GPHY Clock Source
#define CGU_IF_CLK_GPHY_SEL_MASK 0x1Cu
//! Constant XTAL - XTAL
#define CONST_CGU_IF_CLK_GPHY_SEL_XTAL 0x1
//! Constant EXT - External GPIO3
#define CONST_CGU_IF_CLK_GPHY_SEL_EXT 0x2

//! Field SI_CLK_EN - SI_CLK to CPU sub system Enable
#define CGU_IF_CLK_SI_CLK_EN_POS 7
//! Field SI_CLK_EN - SI_CLK to CPU sub system Enable
#define CGU_IF_CLK_SI_CLK_EN_MASK 0x80u
//! Constant DIS - not Gated
#define CONST_CGU_IF_CLK_SI_CLK_EN_DIS 0x0
//! Constant ENB - Gated
#define CONST_CGU_IF_CLK_SI_CLK_EN_ENB 0x1

//! Field CLKOD2 - Clock Output CLK_OUT2 Frequency Select
#define CGU_IF_CLK_CLKOD2_POS 10
//! Field CLKOD2 - Clock Output CLK_OUT2 Frequency Select
#define CGU_IF_CLK_CLKOD2_MASK 0xC00u
//! Constant V25_MHZ - 25 MHz
#define CONST_CGU_IF_CLK_CLKOD2_V25_MHZ 0x0
//! Constant RES1 - RES1
#define CONST_CGU_IF_CLK_CLKOD2_RES1 0x1
//! Constant RES2 - RES2
#define CONST_CGU_IF_CLK_CLKOD2_RES2 0x2
//! Constant RES3 - RES3
#define CONST_CGU_IF_CLK_CLKOD2_RES3 0x3

//! Field CLKOD1 - Clock Output CLK_OUT1 Frequency Select
#define CGU_IF_CLK_CLKOD1_POS 12
//! Field CLKOD1 - Clock Output CLK_OUT1 Frequency Select
#define CGU_IF_CLK_CLKOD1_MASK 0x3000u
//! Constant XTAL - XTAL
#define CONST_CGU_IF_CLK_CLKOD1_XTAL 0x0
//! Constant RES1 - RES1
#define CONST_CGU_IF_CLK_CLKOD1_RES1 0x1
//! Constant RES2 - RES2
#define CONST_CGU_IF_CLK_CLKOD1_RES2 0x2
//! Constant RES3 - RES3
#define CONST_CGU_IF_CLK_CLKOD1_RES3 0x3

//! Field CLKOD0 - Clock Output CLK_OUT0 Frequency Select
#define CGU_IF_CLK_CLKOD0_POS 14
//! Field CLKOD0 - Clock Output CLK_OUT0 Frequency Select
#define CGU_IF_CLK_CLKOD0_MASK 0xC000u
//! Constant RES0 - RES0
#define CONST_CGU_IF_CLK_CLKOD0_RES0 0x0
//! Constant RES1 - RES1
#define CONST_CGU_IF_CLK_CLKOD0_RES1 0x1
//! Constant V8192 - 8.192 MHz
#define CONST_CGU_IF_CLK_CLKOD0_V8192 0x2
//! Constant RES3 - RES3
#define CONST_CGU_IF_CLK_CLKOD0_RES3 0x3

//! @}

//! \defgroup CGU_PCMCR Register CGU_PCMCR - CGU PCM Control Register
//! @{

//! Register Offset (relative)
#define CGU_PCMCR 0xC8
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_PCMCR 0x162000C8u

//! Register Reset Value
#define CGU_PCMCR_RST 0x04000000u

//! Field FSC_INT - FSC Interrupt Type
#define CGU_PCMCR_FSC_INT_POS 1
//! Field FSC_INT - FSC Interrupt Type
#define CGU_PCMCR_FSC_INT_MASK 0x2u
//! Constant SHORT - short
#define CONST_CGU_PCMCR_FSC_INT_SHORT 0x0
//! Constant STICKY - sticky
#define CONST_CGU_PCMCR_FSC_INT_STICKY 0x1

//! Field CTM_SEL - CT Mode
#define CGU_PCMCR_CTM_SEL_POS 4
//! Field CTM_SEL - CT Mode
#define CGU_PCMCR_CTM_SEL_MASK 0x10u
//! Constant M131 - only option for taking 131.072Mhz
#define CONST_CGU_PCMCR_CTM_SEL_M131 0x0

//! Field FSC_DUTY - FSC Duty Cycle Configuration
#define CGU_PCMCR_FSC_DUTY_POS 5
//! Field FSC_DUTY - FSC Duty Cycle Configuration
#define CGU_PCMCR_FSC_DUTY_MASK 0x20u
//! Constant V5050 - 50:50
#define CONST_CGU_PCMCR_FSC_DUTY_V5050 0x0
//! Constant DCL - DCL
#define CONST_CGU_PCMCR_FSC_DUTY_DCL 0x1

//! Field CT_MUX_SEL - Select input ref clk for clock tracking
#define CGU_PCMCR_CT_MUX_SEL_POS 6
//! Field CT_MUX_SEL - Select input ref clk for clock tracking
#define CGU_PCMCR_CT_MUX_SEL_MASK 0xC0u
//! Constant PCM - FSC from PCM interface
#define CONST_CGU_PCMCR_CT_MUX_SEL_PCM 0x0
//! Constant GPIO - option for taking NTR source from GPIO
#define CONST_CGU_PCMCR_CT_MUX_SEL_GPIO 0x2

//! Field CT_TEST_MUX - CT Test Mux
#define CGU_PCMCR_CT_TEST_MUX_POS 8
//! Field CT_TEST_MUX - CT Test Mux
#define CGU_PCMCR_CT_TEST_MUX_MASK 0x100u
//! Constant CT_REF - use CT_MUX_SEL to select source
#define CONST_CGU_PCMCR_CT_TEST_MUX_CT_REF 0x0
//! Constant DCL - use DCL clock
#define CONST_CGU_PCMCR_CT_TEST_MUX_DCL 0x1

//! Field DNTR - NTR Dividers
#define CGU_PCMCR_DNTR_POS 11
//! Field DNTR - NTR Dividers
#define CGU_PCMCR_DNTR_MASK 0x1800u
//! Constant D192 - 1.536Mhz divided by 192
#define CONST_CGU_PCMCR_DNTR_D192 0x0
//! Constant D96 - 0.768Mhz divided by 96
#define CONST_CGU_PCMCR_DNTR_D96 0x1
//! Constant D64 - 0.512Mhz divided by 64
#define CONST_CGU_PCMCR_DNTR_D64 0x2
//! Constant D1 - no division, 8Khz
#define CONST_CGU_PCMCR_DNTR_D1 0x3

//! Field PCM_SL - PCM Slave Mode
#define CGU_PCMCR_PCM_SL_POS 13
//! Field PCM_SL - PCM Slave Mode
#define CGU_PCMCR_PCM_SL_MASK 0x2000u
//! Constant MASTER - Master
#define CONST_CGU_PCMCR_PCM_SL_MASTER 0x0
//! Constant SLAVE - Slave
#define CONST_CGU_PCMCR_PCM_SL_SLAVE 0x1

//! Field MUXFSC - Multiplexing Select FSC
#define CGU_PCMCR_MUXFSC_POS 18
//! Field MUXFSC - Multiplexing Select FSC
#define CGU_PCMCR_MUXFSC_MASK 0x40000u
//! Constant MASTER - Master
#define CONST_CGU_PCMCR_MUXFSC_MASTER 0x0
//! Constant SLAVE - Slave
#define CONST_CGU_PCMCR_MUXFSC_SLAVE 0x1

//! Field MUXDCL - Multiplexing Select DCL
#define CGU_PCMCR_MUXDCL_POS 22
//! Field MUXDCL - Multiplexing Select DCL
#define CGU_PCMCR_MUXDCL_MASK 0x400000u
//! Constant MASTER - Master
#define CONST_CGU_PCMCR_MUXDCL_MASTER 0x0
//! Constant SLAVE - Slave
#define CONST_CGU_PCMCR_MUXDCL_SLAVE 0x1

//! Field DCL_SEL - Frequency selection of DCL
#define CGU_PCMCR_DCL_SEL_POS 25
//! Field DCL_SEL - Frequency selection of DCL
#define CGU_PCMCR_DCL_SEL_MASK 0xE000000u
//! Constant V8192_MHZ - 8.192 MHz
#define CONST_CGU_PCMCR_DCL_SEL_V8192_MHZ 0x0
//! Constant V4096_MHZ - 4.096 MHz
#define CONST_CGU_PCMCR_DCL_SEL_V4096_MHZ 0x1
//! Constant V2048_MHZ - 2.048 MHz
#define CONST_CGU_PCMCR_DCL_SEL_V2048_MHZ 0x2
//! Constant V1536_MHZ - 1.536 MHz
#define CONST_CGU_PCMCR_DCL_SEL_V1536_MHZ 0x3
//! Constant V1024_MHZ - 1.024 MHz
#define CONST_CGU_PCMCR_DCL_SEL_V1024_MHZ 0x4
//! Constant V0512_MHZ - 0.512 MHz
#define CONST_CGU_PCMCR_DCL_SEL_V0512_MHZ 0x5
//! Constant RES - res
#define CONST_CGU_PCMCR_DCL_SEL_RES 0x6

//! Field INT_SEL - Select FSC Edge for interrupt generation
#define CGU_PCMCR_INT_SEL_POS 28
//! Field INT_SEL - Select FSC Edge for interrupt generation
#define CGU_PCMCR_INT_SEL_MASK 0x30000000u
//! Constant NINT - no interrupt generation
#define CONST_CGU_PCMCR_INT_SEL_NINT 0x0
//! Constant R - FSC rising edge to generate
#define CONST_CGU_PCMCR_INT_SEL_R 0x1
//! Constant F - FSC falling edge to generate
#define CONST_CGU_PCMCR_INT_SEL_F 0x2
//! Constant RF - Both edge to generate
#define CONST_CGU_PCMCR_INT_SEL_RF 0x3

//! Field CT_EN - Clock Track Enable
#define CGU_PCMCR_CT_EN_POS 31
//! Field CT_EN - Clock Track Enable
#define CGU_PCMCR_CT_EN_MASK 0x80000000u
//! Constant DISABLE - Disable
#define CONST_CGU_PCMCR_CT_EN_DISABLE 0x0
//! Constant ENABLE - Enable
#define CONST_CGU_PCMCR_CT_EN_ENABLE 0x1

//! @}

//! \defgroup CGU_CT_STAT Register CGU_CT_STAT - CGU Clock Tracking Status Register
//! @{

//! Register Offset (relative)
#define CGU_CT_STAT 0xCC
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_CT_STAT 0x162000CCu

//! Register Reset Value
#define CGU_CT_STAT_RST 0x00000000u

//! Field PDOUT - Signed Integer Value of phase discriminator output
#define CGU_CT_STAT_PDOUT_POS 0
//! Field PDOUT - Signed Integer Value of phase discriminator output
#define CGU_CT_STAT_PDOUT_MASK 0x7FFFu

//! @}

//! \defgroup CGU_CT_KVAL Register CGU_CT_KVAL - CGU Clock Tracking K Value Register
//! @{

//! Register Offset (relative)
#define CGU_CT_KVAL 0xD0
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_CT_KVAL 0x162000D0u

//! Register Reset Value
#define CGU_CT_KVAL_RST 0x00000000u

//! Field PLL1K - Positive Integer Value of the output of the CT block
#define CGU_CT_KVAL_PLL1K_POS 0
//! Field PLL1K - Positive Integer Value of the output of the CT block
#define CGU_CT_KVAL_PLL1K_MASK 0xFFFFFu

//! @}

//! \defgroup CGU_CLKGSR0 Register CGU_CLKGSR0 - CGU Clock Gating Status Register 0
//! @{

//! Register Offset (relative)
#define CGU_CLKGSR0 0x114
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_CLKGSR0 0x16200114u

//! Register Reset Value
#define CGU_CLKGSR0_RST 0x000000CFu

//! Field XBAR0 - XBAR0 clock status
#define CGU_CLKGSR0_XBAR0_POS 0
//! Field XBAR0 - XBAR0 clock status
#define CGU_CLKGSR0_XBAR0_MASK 0x1u
//! Constant GATED - gated
#define CONST_CGU_CLKGSR0_XBAR0_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGSR0_XBAR0_ENABLED 0x1

//! Field XBAR1 - XBAR1 clock status
#define CGU_CLKGSR0_XBAR1_POS 1
//! Field XBAR1 - XBAR1 clock status
#define CGU_CLKGSR0_XBAR1_MASK 0x2u
//! Constant GATED - gated
#define CONST_CGU_CLKGSR0_XBAR1_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGSR0_XBAR1_ENABLED 0x1

//! Field XBAR2 - XBAR2 clock status
#define CGU_CLKGSR0_XBAR2_POS 2
//! Field XBAR2 - XBAR2 clock status
#define CGU_CLKGSR0_XBAR2_MASK 0x4u
//! Constant GATED - gated
#define CONST_CGU_CLKGSR0_XBAR2_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGSR0_XBAR2_ENABLED 0x1

//! Field XBAR3 - XBAR3 clock status
#define CGU_CLKGSR0_XBAR3_POS 3
//! Field XBAR3 - XBAR3 clock status
#define CGU_CLKGSR0_XBAR3_MASK 0x8u
//! Constant GATED - gated
#define CONST_CGU_CLKGSR0_XBAR3_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGSR0_XBAR3_ENABLED 0x1

//! Field XBAR6 - XBAR6 clock status
#define CGU_CLKGSR0_XBAR6_POS 6
//! Field XBAR6 - XBAR6 clock status
#define CGU_CLKGSR0_XBAR6_MASK 0x40u
//! Constant GATED - gated
#define CONST_CGU_CLKGSR0_XBAR6_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGSR0_XBAR6_ENABLED 0x1

//! Field XBAR7 - XBAR7 clock status
#define CGU_CLKGSR0_XBAR7_POS 7
//! Field XBAR7 - XBAR7 clock status
#define CGU_CLKGSR0_XBAR7_MASK 0x80u
//! Constant GATED - gated
#define CONST_CGU_CLKGSR0_XBAR7_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGSR0_XBAR7_ENABLED 0x1

//! @}

//! \defgroup CGU_CLKGCR0_A Register CGU_CLKGCR0_A - CGU Clock Gating Control Register 0_A
//! @{

//! Register Offset (relative)
#define CGU_CLKGCR0_A 0x118
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_CLKGCR0_A 0x16200118u

//! Register Reset Value
#define CGU_CLKGCR0_A_RST 0x00000000u

//! Field XBAR0 - XBAR0 clock Enable Request
#define CGU_CLKGCR0_A_XBAR0_POS 0
//! Field XBAR0 - XBAR0 clock Enable Request
#define CGU_CLKGCR0_A_XBAR0_MASK 0x1u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_A_XBAR0_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_A_XBAR0_ENABLED 0x1

//! Field XBAR1 - XBAR1 clock Enable Request
#define CGU_CLKGCR0_A_XBAR1_POS 1
//! Field XBAR1 - XBAR1 clock Enable Request
#define CGU_CLKGCR0_A_XBAR1_MASK 0x2u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_A_XBAR1_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_A_XBAR1_ENABLED 0x1

//! Field XBAR2 - XBAR2 clock Enable Request
#define CGU_CLKGCR0_A_XBAR2_POS 2
//! Field XBAR2 - XBAR2 clock Enable Request
#define CGU_CLKGCR0_A_XBAR2_MASK 0x4u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_A_XBAR2_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_A_XBAR2_ENABLED 0x1

//! Field XBAR3 - XBAR3 clock Enable Request
#define CGU_CLKGCR0_A_XBAR3_POS 3
//! Field XBAR3 - XBAR3 clock Enable Request
#define CGU_CLKGCR0_A_XBAR3_MASK 0x8u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_A_XBAR3_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_A_XBAR3_ENABLED 0x1

//! Field XBAR6 - XBAR6 clock Enable Request
#define CGU_CLKGCR0_A_XBAR6_POS 6
//! Field XBAR6 - XBAR6 clock Enable Request
#define CGU_CLKGCR0_A_XBAR6_MASK 0x40u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_A_XBAR6_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_A_XBAR6_ENABLED 0x1

//! Field XBAR7 - XBAR7 clock Enable Request
#define CGU_CLKGCR0_A_XBAR7_POS 7
//! Field XBAR7 - XBAR7 clock Enable Request
#define CGU_CLKGCR0_A_XBAR7_MASK 0x80u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_A_XBAR7_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_A_XBAR7_ENABLED 0x1

//! @}

//! \defgroup CGU_CLKGCR0_B Register CGU_CLKGCR0_B - CGU Clock Gating Control Register 0_B
//! @{

//! Register Offset (relative)
#define CGU_CLKGCR0_B 0x11C
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_CLKGCR0_B 0x1620011Cu

//! Register Reset Value
#define CGU_CLKGCR0_B_RST 0x00000000u

//! Field XBAR0 - XBAR0 clock Disable Request
#define CGU_CLKGCR0_B_XBAR0_POS 0
//! Field XBAR0 - XBAR0 clock Disable Request
#define CGU_CLKGCR0_B_XBAR0_MASK 0x1u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_B_XBAR0_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_B_XBAR0_ENABLED 0x1

//! Field XBAR1 - XBAR1 clock Disable Request
#define CGU_CLKGCR0_B_XBAR1_POS 1
//! Field XBAR1 - XBAR1 clock Disable Request
#define CGU_CLKGCR0_B_XBAR1_MASK 0x2u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_B_XBAR1_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_B_XBAR1_ENABLED 0x1

//! Field XBAR2 - XBAR2 clock Disable Request
#define CGU_CLKGCR0_B_XBAR2_POS 2
//! Field XBAR2 - XBAR2 clock Disable Request
#define CGU_CLKGCR0_B_XBAR2_MASK 0x4u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_B_XBAR2_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_B_XBAR2_ENABLED 0x1

//! Field XBAR3 - XBAR3 clock Disable Request
#define CGU_CLKGCR0_B_XBAR3_POS 3
//! Field XBAR3 - XBAR3 clock Disable Request
#define CGU_CLKGCR0_B_XBAR3_MASK 0x8u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_B_XBAR3_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_B_XBAR3_ENABLED 0x1

//! Field XBAR6 - XBAR6 clock Disable Request
#define CGU_CLKGCR0_B_XBAR6_POS 6
//! Field XBAR6 - XBAR6 clock Disable Request
#define CGU_CLKGCR0_B_XBAR6_MASK 0x40u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_B_XBAR6_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_B_XBAR6_ENABLED 0x1

//! Field XBAR7 - XBAR7 clock Disable Request
#define CGU_CLKGCR0_B_XBAR7_POS 7
//! Field XBAR7 - XBAR7 clock Disable Request
#define CGU_CLKGCR0_B_XBAR7_MASK 0x80u
//! Constant GATED - gated
#define CONST_CGU_CLKGCR0_B_XBAR7_GATED 0x0
//! Constant ENABLED - enabled
#define CONST_CGU_CLKGCR0_B_XBAR7_ENABLED 0x1

//! @}

//! \defgroup GGU_CLKGSR1 Register GGU_CLKGSR1 - CGU Clock Gating Status Register 1
//! @{

//! Register Offset (relative)
#define GGU_CLKGSR1 0x120
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_GGU_CLKGSR1 0x16200120u

//! Register Reset Value
#define GGU_CLKGSR1_RST 0x1EF27FE4u

//! Field V_CODEC - Voice Codec Clock Status
#define GGU_CLKGSR1_V_CODEC_POS 2
//! Field V_CODEC - Voice Codec Clock Status
#define GGU_CLKGSR1_V_CODEC_MASK 0x4u
//! Constant V_0 - OFF
#define CONST_GGU_CLKGSR1_V_CODEC_V_0 0x0
//! Constant V_1 - ON
#define CONST_GGU_CLKGSR1_V_CODEC_V_1 0x1

//! Field DMA0 - DMA0 Clock Status
#define GGU_CLKGSR1_DMA0_POS 5
//! Field DMA0 - DMA0 Clock Status
#define GGU_CLKGSR1_DMA0_MASK 0x20u
//! Constant DMA0_0 - OFF
#define CONST_GGU_CLKGSR1_DMA0_DMA0_0 0x0
//! Constant DMA0_1 - ON
#define CONST_GGU_CLKGSR1_DMA0_DMA0_1 0x1

//! Field USB0 - USB0 Controller and PHY Clock Status
#define GGU_CLKGSR1_USB0_POS 6
//! Field USB0 - USB0 Controller and PHY Clock Status
#define GGU_CLKGSR1_USB0_MASK 0x40u
//! Constant USB0_0 - OFF
#define CONST_GGU_CLKGSR1_USB0_USB0_0 0x0
//! Constant USB0_1 - ON
#define CONST_GGU_CLKGSR1_USB0_USB0_1 0x1

//! Field SPI1 - SPI1 Clock Status
#define GGU_CLKGSR1_SPI1_POS 7
//! Field SPI1 - SPI1 Clock Status
#define GGU_CLKGSR1_SPI1_MASK 0x80u
//! Constant SPI1_0 - OFF
#define CONST_GGU_CLKGSR1_SPI1_SPI1_0 0x0
//! Constant SPI1_1 - ON
#define CONST_GGU_CLKGSR1_SPI1_SPI1_1 0x1

//! Field SPI0 - SPI0 Clock Status
#define GGU_CLKGSR1_SPI0_POS 8
//! Field SPI0 - SPI0 Clock Status
#define GGU_CLKGSR1_SPI0_MASK 0x100u
//! Constant SPI0_0 - OFF
#define CONST_GGU_CLKGSR1_SPI0_SPI0_0 0x0
//! Constant SPI0_1 - ON
#define CONST_GGU_CLKGSR1_SPI0_SPI0_1 0x1

//! Field CBM - CBM Clock Status
#define GGU_CLKGSR1_CBM_POS 9
//! Field CBM - CBM Clock Status
#define GGU_CLKGSR1_CBM_MASK 0x200u
//! Constant V0 - OFF
#define CONST_GGU_CLKGSR1_CBM_V0 0x0
//! Constant V1 - ON
#define CONST_GGU_CLKGSR1_CBM_V1 0x1

//! Field EBU_NAND - EBU and NAND Controller Clock Status
#define GGU_CLKGSR1_EBU_NAND_POS 10
//! Field EBU_NAND - EBU and NAND Controller Clock Status
#define GGU_CLKGSR1_EBU_NAND_MASK 0x400u
//! Constant EBU_0 - OFF
#define CONST_GGU_CLKGSR1_EBU_NAND_EBU_0 0x0
//! Constant EBU_1 - ON
#define CONST_GGU_CLKGSR1_EBU_NAND_EBU_1 0x1

//! Field SSO - Serial Shift Output Controller Clock Status
#define GGU_CLKGSR1_SSO_POS 11
//! Field SSO - Serial Shift Output Controller Clock Status
#define GGU_CLKGSR1_SSO_MASK 0x800u
//! Constant SSO_0 - OFF
#define CONST_GGU_CLKGSR1_SSO_SSO_0 0x0
//! Constant SSO_1 - ON
#define CONST_GGU_CLKGSR1_SSO_SSO_1 0x1

//! Field GPTC0 - GPTC 0 Clock Status
#define GGU_CLKGSR1_GPTC0_POS 12
//! Field GPTC0 - GPTC 0 Clock Status
#define GGU_CLKGSR1_GPTC0_MASK 0x1000u
//! Constant GPTC0_0 - OFF
#define CONST_GGU_CLKGSR1_GPTC0_GPTC0_0 0x0
//! Constant GPTC0_1 - ON
#define CONST_GGU_CLKGSR1_GPTC0_GPTC0_1 0x1

//! Field GPTC1 - GPTC 1 Clock Status
#define GGU_CLKGSR1_GPTC1_POS 13
//! Field GPTC1 - GPTC 1 Clock Status
#define GGU_CLKGSR1_GPTC1_MASK 0x2000u
//! Constant GPTC1_0 - OFF
#define CONST_GGU_CLKGSR1_GPTC1_GPTC1_0 0x0
//! Constant GPTC1_1 - ON
#define CONST_GGU_CLKGSR1_GPTC1_GPTC1_1 0x1

//! Field GPTC2 - GPTC 2 Clock Status
#define GGU_CLKGSR1_GPTC2_POS 14
//! Field GPTC2 - GPTC 2 Clock Status
#define GGU_CLKGSR1_GPTC2_MASK 0x4000u
//! Constant GPTC2_0 - OFF
#define CONST_GGU_CLKGSR1_GPTC2_GPTC2_0 0x0
//! Constant GPTC2_1 - ON
#define CONST_GGU_CLKGSR1_GPTC2_GPTC2_1 0x1

//! Field UART - UART 0 Clock Status
#define GGU_CLKGSR1_UART_POS 17
//! Field UART - UART 0 Clock Status
#define GGU_CLKGSR1_UART_MASK 0x20000u
//! Constant UART_0 - OFF
#define CONST_GGU_CLKGSR1_UART_UART_0 0x0
//! Constant UART_1 - ON
#define CONST_GGU_CLKGSR1_UART_UART_1 0x1

//! Field CRYPTO - Crypto Engine Clock Status
#define GGU_CLKGSR1_CRYPTO_POS 20
//! Field CRYPTO - Crypto Engine Clock Status
#define GGU_CLKGSR1_CRYPTO_MASK 0x100000u
//! Constant CRYP_0 - OFF
#define CONST_GGU_CLKGSR1_CRYPTO_CRYP_0 0x0
//! Constant CRYP_1 - ON
#define CONST_GGU_CLKGSR1_CRYPTO_CRYP_1 0x1

//! Field SECPT - Secure Platform Engine Clock Status
#define GGU_CLKGSR1_SECPT_POS 21
//! Field SECPT - Secure Platform Engine Clock Status
#define GGU_CLKGSR1_SECPT_MASK 0x200000u
//! Constant SRYP_0 - OFF
#define CONST_GGU_CLKGSR1_SECPT_SRYP_0 0x0
//! Constant SRYP_1 - ON
#define CONST_GGU_CLKGSR1_SECPT_SRYP_1 0x1

//! Field TOE - TCP Offload Engine Clock Status
#define GGU_CLKGSR1_TOE_POS 22
//! Field TOE - TCP Offload Engine Clock Status
#define GGU_CLKGSR1_TOE_MASK 0x400000u
//! Constant V0 - OFF
#define CONST_GGU_CLKGSR1_TOE_V0 0x0
//! Constant V1 - ON
#define CONST_GGU_CLKGSR1_TOE_V1 0x1

//! Field MPE - MPE Clock Status
#define GGU_CLKGSR1_MPE_POS 23
//! Field MPE - MPE Clock Status
#define GGU_CLKGSR1_MPE_MASK 0x800000u
//! Constant V0 - OFF
#define CONST_GGU_CLKGSR1_MPE_V0 0x0
//! Constant V1 - ON
#define CONST_GGU_CLKGSR1_MPE_V1 0x1

//! Field TDM - TDM Module Clock Status
#define GGU_CLKGSR1_TDM_POS 25
//! Field TDM - TDM Module Clock Status
#define GGU_CLKGSR1_TDM_MASK 0x2000000u
//! Constant TDM_0 - OFF
#define CONST_GGU_CLKGSR1_TDM_TDM_0 0x0
//! Constant TDM_1 - ON
#define CONST_GGU_CLKGSR1_TDM_TDM_1 0x1

//! Field PAE - Packet Acceleration Engine Clock Status
#define GGU_CLKGSR1_PAE_POS 26
//! Field PAE - Packet Acceleration Engine Clock Status
#define GGU_CLKGSR1_PAE_MASK 0x4000000u
//! Constant PAE_0 - OFF
#define CONST_GGU_CLKGSR1_PAE_PAE_0 0x0
//! Constant PAE_1 - ON
#define CONST_GGU_CLKGSR1_PAE_PAE_1 0x1

//! Field USB1 - USB1 Controller and PHY Clock Status
#define GGU_CLKGSR1_USB1_POS 27
//! Field USB1 - USB1 Controller and PHY Clock Status
#define GGU_CLKGSR1_USB1_MASK 0x8000000u
//! Constant USB1_0 - OFF
#define CONST_GGU_CLKGSR1_USB1_USB1_0 0x0
//! Constant USB1_1 - ON
#define CONST_GGU_CLKGSR1_USB1_USB1_1 0x1

//! Field LAN_SWITCH - LAN Switch Clock Status
#define GGU_CLKGSR1_LAN_SWITCH_POS 28
//! Field LAN_SWITCH - LAN Switch Clock Status
#define GGU_CLKGSR1_LAN_SWITCH_MASK 0x10000000u
//! Constant SWITCH_0 - OFF
#define CONST_GGU_CLKGSR1_LAN_SWITCH_SWITCH_0 0x0
//! Constant SWITCH_1 - ON
#define CONST_GGU_CLKGSR1_LAN_SWITCH_SWITCH_1 0x1

//! @}

//! \defgroup CGU_CLKGCR1_A Register CGU_CLKGCR1_A - CGU Clock Gating Control Register 1_A
//! @{

//! Register Offset (relative)
#define CGU_CLKGCR1_A 0x124
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_CLKGCR1_A 0x16200124u

//! Register Reset Value
#define CGU_CLKGCR1_A_RST 0x00000000u

//! Field V_CODEC - Voice Codec Clock Enable Request
#define CGU_CLKGCR1_A_V_CODEC_POS 2
//! Field V_CODEC - Voice Codec Clock Enable Request
#define CGU_CLKGCR1_A_V_CODEC_MASK 0x4u
//! Constant V_0 - no action
#define CONST_CGU_CLKGCR1_A_V_CODEC_V_0 0x0
//! Constant V_1 - Enable
#define CONST_CGU_CLKGCR1_A_V_CODEC_V_1 0x1

//! Field DMA0 - DMA0 Clock Enable Request
#define CGU_CLKGCR1_A_DMA0_POS 5
//! Field DMA0 - DMA0 Clock Enable Request
#define CGU_CLKGCR1_A_DMA0_MASK 0x20u
//! Constant DMA_0 - no action
#define CONST_CGU_CLKGCR1_A_DMA0_DMA_0 0x0
//! Constant DMA_1 - Enable
#define CONST_CGU_CLKGCR1_A_DMA0_DMA_1 0x1

//! Field USB0 - USB0 Controller and PHY Clock Enable Request
#define CGU_CLKGCR1_A_USB0_POS 6
//! Field USB0 - USB0 Controller and PHY Clock Enable Request
#define CGU_CLKGCR1_A_USB0_MASK 0x40u
//! Constant USB0_0 - no action
#define CONST_CGU_CLKGCR1_A_USB0_USB0_0 0x0
//! Constant USB0_1 - Enable
#define CONST_CGU_CLKGCR1_A_USB0_USB0_1 0x1

//! Field SPI1 - SPI1 Clock Enable Request
#define CGU_CLKGCR1_A_SPI1_POS 7
//! Field SPI1 - SPI1 Clock Enable Request
#define CGU_CLKGCR1_A_SPI1_MASK 0x80u
//! Constant V0 - no action
#define CONST_CGU_CLKGCR1_A_SPI1_V0 0x0
//! Constant V1 - Enable
#define CONST_CGU_CLKGCR1_A_SPI1_V1 0x1

//! Field SPI0 - SPI0 Clock Enable Request
#define CGU_CLKGCR1_A_SPI0_POS 8
//! Field SPI0 - SPI0 Clock Enable Request
#define CGU_CLKGCR1_A_SPI0_MASK 0x100u
//! Constant SPI0_0 - no action
#define CONST_CGU_CLKGCR1_A_SPI0_SPI0_0 0x0
//! Constant SPI0_1 - Enable
#define CONST_CGU_CLKGCR1_A_SPI0_SPI0_1 0x1

//! Field CBM - CBM Clock Enable Request
#define CGU_CLKGCR1_A_CBM_POS 9
//! Field CBM - CBM Clock Enable Request
#define CGU_CLKGCR1_A_CBM_MASK 0x200u
//! Constant V0 - no action
#define CONST_CGU_CLKGCR1_A_CBM_V0 0x0
//! Constant V1 - Enable
#define CONST_CGU_CLKGCR1_A_CBM_V1 0x1

//! Field EBU - EBU Clock Enable Request
#define CGU_CLKGCR1_A_EBU_POS 10
//! Field EBU - EBU Clock Enable Request
#define CGU_CLKGCR1_A_EBU_MASK 0x400u
//! Constant EBU_0 - no action
#define CONST_CGU_CLKGCR1_A_EBU_EBU_0 0x0
//! Constant EBU_1 - Enable
#define CONST_CGU_CLKGCR1_A_EBU_EBU_1 0x1

//! Field SSO - Serial Shift Output Controller Clock Enable Request
#define CGU_CLKGCR1_A_SSO_POS 11
//! Field SSO - Serial Shift Output Controller Clock Enable Request
#define CGU_CLKGCR1_A_SSO_MASK 0x800u
//! Constant SSO_0 - no action
#define CONST_CGU_CLKGCR1_A_SSO_SSO_0 0x0
//! Constant SSO_1 - Enable
#define CONST_CGU_CLKGCR1_A_SSO_SSO_1 0x1

//! Field GPTC0 - GPTC 0 Clock Enable Request
#define CGU_CLKGCR1_A_GPTC0_POS 12
//! Field GPTC0 - GPTC 0 Clock Enable Request
#define CGU_CLKGCR1_A_GPTC0_MASK 0x1000u
//! Constant GPTC0_0 - no action
#define CONST_CGU_CLKGCR1_A_GPTC0_GPTC0_0 0x0
//! Constant GPTC0_1 - Enable
#define CONST_CGU_CLKGCR1_A_GPTC0_GPTC0_1 0x1

//! Field GPTC1 - GPTC 1 Clock Enable Request
#define CGU_CLKGCR1_A_GPTC1_POS 13
//! Field GPTC1 - GPTC 1 Clock Enable Request
#define CGU_CLKGCR1_A_GPTC1_MASK 0x2000u
//! Constant GPTC1_0 - no action
#define CONST_CGU_CLKGCR1_A_GPTC1_GPTC1_0 0x0
//! Constant GPTC1_1 - Enable
#define CONST_CGU_CLKGCR1_A_GPTC1_GPTC1_1 0x1

//! Field GPTC2 - GPTC 2 Clock Enable Request
#define CGU_CLKGCR1_A_GPTC2_POS 14
//! Field GPTC2 - GPTC 2 Clock Enable Request
#define CGU_CLKGCR1_A_GPTC2_MASK 0x4000u
//! Constant GPTC2_0 - no action
#define CONST_CGU_CLKGCR1_A_GPTC2_GPTC2_0 0x0
//! Constant GPTC2_1 - Enable
#define CONST_CGU_CLKGCR1_A_GPTC2_GPTC2_1 0x1

//! Field UART - UART Clock Enable Request
#define CGU_CLKGCR1_A_UART_POS 17
//! Field UART - UART Clock Enable Request
#define CGU_CLKGCR1_A_UART_MASK 0x20000u
//! Constant UART_0 - no action
#define CONST_CGU_CLKGCR1_A_UART_UART_0 0x0
//! Constant UART_1 - Enable
#define CONST_CGU_CLKGCR1_A_UART_UART_1 0x1

//! Field CRYPTO - Crypto Engine Clock Enable Request
#define CGU_CLKGCR1_A_CRYPTO_POS 20
//! Field CRYPTO - Crypto Engine Clock Enable Request
#define CGU_CLKGCR1_A_CRYPTO_MASK 0x100000u
//! Constant CRYP_0 - no action
#define CONST_CGU_CLKGCR1_A_CRYPTO_CRYP_0 0x0
//! Constant CRYP_1 - Enable
#define CONST_CGU_CLKGCR1_A_CRYPTO_CRYP_1 0x1

//! Field SECPT - Secure Platform Engine Clock Enable Request
#define CGU_CLKGCR1_A_SECPT_POS 21
//! Field SECPT - Secure Platform Engine Clock Enable Request
#define CGU_CLKGCR1_A_SECPT_MASK 0x200000u
//! Constant SRYP_0 - no action
#define CONST_CGU_CLKGCR1_A_SECPT_SRYP_0 0x0
//! Constant SRYP_1 - Enable
#define CONST_CGU_CLKGCR1_A_SECPT_SRYP_1 0x1

//! Field TOE - TOE Clock Enable Request
#define CGU_CLKGCR1_A_TOE_POS 22
//! Field TOE - TOE Clock Enable Request
#define CGU_CLKGCR1_A_TOE_MASK 0x400000u
//! Constant V0 - no action
#define CONST_CGU_CLKGCR1_A_TOE_V0 0x0
//! Constant V1 - Enable
#define CONST_CGU_CLKGCR1_A_TOE_V1 0x1

//! Field MPE - MPE Clock Enable Request
#define CGU_CLKGCR1_A_MPE_POS 23
//! Field MPE - MPE Clock Enable Request
#define CGU_CLKGCR1_A_MPE_MASK 0x800000u
//! Constant V0 - no action
#define CONST_CGU_CLKGCR1_A_MPE_V0 0x0
//! Constant V1 - Enable
#define CONST_CGU_CLKGCR1_A_MPE_V1 0x1

//! Field TDM - TDM Module Clock Enable Request
#define CGU_CLKGCR1_A_TDM_POS 25
//! Field TDM - TDM Module Clock Enable Request
#define CGU_CLKGCR1_A_TDM_MASK 0x2000000u
//! Constant TDM_0 - no action
#define CONST_CGU_CLKGCR1_A_TDM_TDM_0 0x0
//! Constant TDM_1 - Enable
#define CONST_CGU_CLKGCR1_A_TDM_TDM_1 0x1

//! Field PAE - Packet Acceleration Engine Clock Enable Request
#define CGU_CLKGCR1_A_PAE_POS 26
//! Field PAE - Packet Acceleration Engine Clock Enable Request
#define CGU_CLKGCR1_A_PAE_MASK 0x4000000u
//! Constant PAE_0 - no action
#define CONST_CGU_CLKGCR1_A_PAE_PAE_0 0x0
//! Constant PAE_1 - Enable
#define CONST_CGU_CLKGCR1_A_PAE_PAE_1 0x1

//! Field USB1 - USB1 Controller and PHY Clock Enable Request
#define CGU_CLKGCR1_A_USB1_POS 27
//! Field USB1 - USB1 Controller and PHY Clock Enable Request
#define CGU_CLKGCR1_A_USB1_MASK 0x8000000u
//! Constant USB1_0 - no action
#define CONST_CGU_CLKGCR1_A_USB1_USB1_0 0x0
//! Constant USB1_1 - Enable
#define CONST_CGU_CLKGCR1_A_USB1_USB1_1 0x1

//! Field LAN_SWITCH - LAN Switch Clock Enable Request
#define CGU_CLKGCR1_A_LAN_SWITCH_POS 28
//! Field LAN_SWITCH - LAN Switch Clock Enable Request
#define CGU_CLKGCR1_A_LAN_SWITCH_MASK 0x10000000u
//! Constant SWITCH_0 - no action
#define CONST_CGU_CLKGCR1_A_LAN_SWITCH_SWITCH_0 0x0
//! Constant SWITCH_1 - Enable
#define CONST_CGU_CLKGCR1_A_LAN_SWITCH_SWITCH_1 0x1

//! @}

//! \defgroup CGU_CLKGCR1_B Register CGU_CLKGCR1_B - CGU Clock Gating Control Register 1_B
//! @{

//! Register Offset (relative)
#define CGU_CLKGCR1_B 0x128
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_CLKGCR1_B 0x16200128u

//! Register Reset Value
#define CGU_CLKGCR1_B_RST 0x00000000u

//! Field V_CODEC - Voice Codec Clock Disable Request
#define CGU_CLKGCR1_B_V_CODEC_POS 2
//! Field V_CODEC - Voice Codec Clock Disable Request
#define CGU_CLKGCR1_B_V_CODEC_MASK 0x4u
//! Constant V_0 - no action
#define CONST_CGU_CLKGCR1_B_V_CODEC_V_0 0x0
//! Constant V_1 - Disable
#define CONST_CGU_CLKGCR1_B_V_CODEC_V_1 0x1

//! Field DMA0 - DMA0 Clock Disable Request
#define CGU_CLKGCR1_B_DMA0_POS 5
//! Field DMA0 - DMA0 Clock Disable Request
#define CGU_CLKGCR1_B_DMA0_MASK 0x20u
//! Constant DMA_0 - no action
#define CONST_CGU_CLKGCR1_B_DMA0_DMA_0 0x0
//! Constant DMA_1 - Disable
#define CONST_CGU_CLKGCR1_B_DMA0_DMA_1 0x1

//! Field USB0 - USB0 Controller and PHY Clock Disable Request
#define CGU_CLKGCR1_B_USB0_POS 6
//! Field USB0 - USB0 Controller and PHY Clock Disable Request
#define CGU_CLKGCR1_B_USB0_MASK 0x40u
//! Constant USB0_0 - no action
#define CONST_CGU_CLKGCR1_B_USB0_USB0_0 0x0
//! Constant USB0_1 - Disable
#define CONST_CGU_CLKGCR1_B_USB0_USB0_1 0x1

//! Field SPI1 - SPI1 Clock Disable Request
#define CGU_CLKGCR1_B_SPI1_POS 7
//! Field SPI1 - SPI1 Clock Disable Request
#define CGU_CLKGCR1_B_SPI1_MASK 0x80u
//! Constant SPI1_0 - no action
#define CONST_CGU_CLKGCR1_B_SPI1_SPI1_0 0x0
//! Constant SPI1_1 - Disable
#define CONST_CGU_CLKGCR1_B_SPI1_SPI1_1 0x1

//! Field SPI0 - SPI0 Clock Disable Request
#define CGU_CLKGCR1_B_SPI0_POS 8
//! Field SPI0 - SPI0 Clock Disable Request
#define CGU_CLKGCR1_B_SPI0_MASK 0x100u
//! Constant SPI0_0 - no action
#define CONST_CGU_CLKGCR1_B_SPI0_SPI0_0 0x0
//! Constant SPI0_1 - Disable
#define CONST_CGU_CLKGCR1_B_SPI0_SPI0_1 0x1

//! Field CBM - CBM Clock Disable Request
#define CGU_CLKGCR1_B_CBM_POS 9
//! Field CBM - CBM Clock Disable Request
#define CGU_CLKGCR1_B_CBM_MASK 0x200u
//! Constant V0 - no action
#define CONST_CGU_CLKGCR1_B_CBM_V0 0x0
//! Constant V1 - Disable
#define CONST_CGU_CLKGCR1_B_CBM_V1 0x1

//! Field EBU - EBU Clock Disable Request
#define CGU_CLKGCR1_B_EBU_POS 10
//! Field EBU - EBU Clock Disable Request
#define CGU_CLKGCR1_B_EBU_MASK 0x400u
//! Constant EBU_0 - no action
#define CONST_CGU_CLKGCR1_B_EBU_EBU_0 0x0
//! Constant EBU_1 - Enable
#define CONST_CGU_CLKGCR1_B_EBU_EBU_1 0x1

//! Field SSO - Serial Shift Output Controller Clock Disable Request
#define CGU_CLKGCR1_B_SSO_POS 11
//! Field SSO - Serial Shift Output Controller Clock Disable Request
#define CGU_CLKGCR1_B_SSO_MASK 0x800u
//! Constant LEDC_0 - no action
#define CONST_CGU_CLKGCR1_B_SSO_LEDC_0 0x0
//! Constant LEDC_1 - Disable
#define CONST_CGU_CLKGCR1_B_SSO_LEDC_1 0x1

//! Field GPTC0 - GPTC 0 Clock Disable Request
#define CGU_CLKGCR1_B_GPTC0_POS 12
//! Field GPTC0 - GPTC 0 Clock Disable Request
#define CGU_CLKGCR1_B_GPTC0_MASK 0x1000u
//! Constant GPTC0_0 - no action
#define CONST_CGU_CLKGCR1_B_GPTC0_GPTC0_0 0x0
//! Constant GPTC0_1 - Disable
#define CONST_CGU_CLKGCR1_B_GPTC0_GPTC0_1 0x1

//! Field GPTC1 - GPTC 1 Clock Disable Request
#define CGU_CLKGCR1_B_GPTC1_POS 13
//! Field GPTC1 - GPTC 1 Clock Disable Request
#define CGU_CLKGCR1_B_GPTC1_MASK 0x2000u
//! Constant GPTC1_0 - no action
#define CONST_CGU_CLKGCR1_B_GPTC1_GPTC1_0 0x0
//! Constant GPTC1_1 - Disable
#define CONST_CGU_CLKGCR1_B_GPTC1_GPTC1_1 0x1

//! Field GPTC2 - GPTC 2 Clock Disable Request
#define CGU_CLKGCR1_B_GPTC2_POS 14
//! Field GPTC2 - GPTC 2 Clock Disable Request
#define CGU_CLKGCR1_B_GPTC2_MASK 0x4000u
//! Constant GPTC2_0 - no action
#define CONST_CGU_CLKGCR1_B_GPTC2_GPTC2_0 0x0
//! Constant GPTC2_1 - Disable
#define CONST_CGU_CLKGCR1_B_GPTC2_GPTC2_1 0x1

//! Field UART - UART Clock Disable Request
#define CGU_CLKGCR1_B_UART_POS 17
//! Field UART - UART Clock Disable Request
#define CGU_CLKGCR1_B_UART_MASK 0x20000u
//! Constant UART_0 - no action
#define CONST_CGU_CLKGCR1_B_UART_UART_0 0x0
//! Constant UART_1 - Disable
#define CONST_CGU_CLKGCR1_B_UART_UART_1 0x1

//! Field CRYPTO - Crypto Engine Clock Disable Request
#define CGU_CLKGCR1_B_CRYPTO_POS 20
//! Field CRYPTO - Crypto Engine Clock Disable Request
#define CGU_CLKGCR1_B_CRYPTO_MASK 0x100000u
//! Constant CRYP_0 - no action
#define CONST_CGU_CLKGCR1_B_CRYPTO_CRYP_0 0x0
//! Constant CRYP_1 - Disable
#define CONST_CGU_CLKGCR1_B_CRYPTO_CRYP_1 0x1

//! Field SECPT - Secure Platform Engine Clock Disable Request
#define CGU_CLKGCR1_B_SECPT_POS 21
//! Field SECPT - Secure Platform Engine Clock Disable Request
#define CGU_CLKGCR1_B_SECPT_MASK 0x200000u
//! Constant SRYP_0 - no action
#define CONST_CGU_CLKGCR1_B_SECPT_SRYP_0 0x0
//! Constant SRYP_1 - Disable
#define CONST_CGU_CLKGCR1_B_SECPT_SRYP_1 0x1

//! Field TOE - TOE Clock Disable Request
#define CGU_CLKGCR1_B_TOE_POS 22
//! Field TOE - TOE Clock Disable Request
#define CGU_CLKGCR1_B_TOE_MASK 0x400000u
//! Constant V0 - no action
#define CONST_CGU_CLKGCR1_B_TOE_V0 0x0
//! Constant V1 - Disable
#define CONST_CGU_CLKGCR1_B_TOE_V1 0x1

//! Field MPE - MPE Clock Disable Request
#define CGU_CLKGCR1_B_MPE_POS 23
//! Field MPE - MPE Clock Disable Request
#define CGU_CLKGCR1_B_MPE_MASK 0x800000u
//! Constant V0 - no action
#define CONST_CGU_CLKGCR1_B_MPE_V0 0x0
//! Constant V1 - Disable
#define CONST_CGU_CLKGCR1_B_MPE_V1 0x1

//! Field TDM - TDM Module Clock Disable Request
#define CGU_CLKGCR1_B_TDM_POS 25
//! Field TDM - TDM Module Clock Disable Request
#define CGU_CLKGCR1_B_TDM_MASK 0x2000000u
//! Constant TDM_0 - no action
#define CONST_CGU_CLKGCR1_B_TDM_TDM_0 0x0
//! Constant TDM_1 - Disable
#define CONST_CGU_CLKGCR1_B_TDM_TDM_1 0x1

//! Field PAE - PAE Module Clock Disable Request
#define CGU_CLKGCR1_B_PAE_POS 26
//! Field PAE - PAE Module Clock Disable Request
#define CGU_CLKGCR1_B_PAE_MASK 0x4000000u
//! Constant PAE_0 - no action
#define CONST_CGU_CLKGCR1_B_PAE_PAE_0 0x0
//! Constant PAE_1 - Disable
#define CONST_CGU_CLKGCR1_B_PAE_PAE_1 0x1

//! Field USB1_CTR - USB1 Controller Clock Disable Request
#define CGU_CLKGCR1_B_USB1_CTR_POS 27
//! Field USB1_CTR - USB1 Controller Clock Disable Request
#define CGU_CLKGCR1_B_USB1_CTR_MASK 0x8000000u
//! Constant USB1CTR_0 - no action
#define CONST_CGU_CLKGCR1_B_USB1_CTR_USB1CTR_0 0x0
//! Constant USB1CTR_1 - Disable
#define CONST_CGU_CLKGCR1_B_USB1_CTR_USB1CTR_1 0x1

//! Field LAN_SWITCH - LAN Switch Clock Disable Request
#define CGU_CLKGCR1_B_LAN_SWITCH_POS 28
//! Field LAN_SWITCH - LAN Switch Clock Disable Request
#define CGU_CLKGCR1_B_LAN_SWITCH_MASK 0x10000000u
//! Constant SWITCH_0 - no action
#define CONST_CGU_CLKGCR1_B_LAN_SWITCH_SWITCH_0 0x0
//! Constant SWITCH_1 - Disable
#define CONST_CGU_CLKGCR1_B_LAN_SWITCH_SWITCH_1 0x1

//! @}

//! \defgroup CGU_CLKGSR2 Register CGU_CLKGSR2 - CGU Clock Gating Status Register 2
//! @{

//! Register Offset (relative)
#define CGU_CLKGSR2 0x130
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_CLKGSR2 0x16200130u

//! Register Reset Value
#define CGU_CLKGSR2_RST 0x02020002u

//! Field PCIE_CTR0 - PCIe Controller #0 Clock Status
#define CGU_CLKGSR2_PCIE_CTR0_POS 1
//! Field PCIE_CTR0 - PCIe Controller #0 Clock Status
#define CGU_CLKGSR2_PCIE_CTR0_MASK 0x2u
//! Constant PCIE_CTR0_0 - Disabled
#define CONST_CGU_CLKGSR2_PCIE_CTR0_PCIE_CTR0_0 0x0
//! Constant PCIE_CTR0_1 - Enabled
#define CONST_CGU_CLKGSR2_PCIE_CTR0_PCIE_CTR0_1 0x1

//! Field PCIE_PD0 - PCIe #0 Power Status PD Value
#define CGU_CLKGSR2_PCIE_PD0_POS 7
//! Field PCIE_PD0 - PCIe #0 Power Status PD Value
#define CGU_CLKGSR2_PCIE_PD0_MASK 0x180u
//! Constant PCIE_NOR - PCIe_NOR
#define CONST_CGU_CLKGSR2_PCIE_PD0_PCIE_NOR 0x0
//! Constant PCIE_L0S - PCIe_LOS
#define CONST_CGU_CLKGSR2_PCIE_PD0_PCIE_L0S 0x1
//! Constant PCIE_L1 - PCIe_L1
#define CONST_CGU_CLKGSR2_PCIE_PD0_PCIE_L1 0x2
//! Constant PCIE_L3 - PCIe_L3
#define CONST_CGU_CLKGSR2_PCIE_PD0_PCIE_L3 0x3

//! Field PCIE_CTR1 - PCIe Controller/PHY #1 Clock Status
#define CGU_CLKGSR2_PCIE_CTR1_POS 17
//! Field PCIE_CTR1 - PCIe Controller/PHY #1 Clock Status
#define CGU_CLKGSR2_PCIE_CTR1_MASK 0x20000u
//! Constant PCIE_CTR1_0 - Enabled
#define CONST_CGU_CLKGSR2_PCIE_CTR1_PCIE_CTR1_0 0x0
//! Constant PCIE_CTR1_1 - Disabled
#define CONST_CGU_CLKGSR2_PCIE_CTR1_PCIE_CTR1_1 0x1

//! Field PCIE_PD1 - PCIe #1 Power Status PD Value
#define CGU_CLKGSR2_PCIE_PD1_POS 23
//! Field PCIE_PD1 - PCIe #1 Power Status PD Value
#define CGU_CLKGSR2_PCIE_PD1_MASK 0x1800000u
//! Constant PCIE_NOR - PCIe_NOR
#define CONST_CGU_CLKGSR2_PCIE_PD1_PCIE_NOR 0x0
//! Constant PCIE_L0S - PCIe_L0S
#define CONST_CGU_CLKGSR2_PCIE_PD1_PCIE_L0S 0x1
//! Constant PCIE_L1_1 - PCIe_L1
#define CONST_CGU_CLKGSR2_PCIE_PD1_PCIE_L1_1 0x2
//! Constant PCIE_L3_1 - PCIe_L3
#define CONST_CGU_CLKGSR2_PCIE_PD1_PCIE_L3_1 0x3

//! Field PCIE_CTR2 - PCIe Controller/PHY #2 Clock Status
#define CGU_CLKGSR2_PCIE_CTR2_POS 25
//! Field PCIE_CTR2 - PCIe Controller/PHY #2 Clock Status
#define CGU_CLKGSR2_PCIE_CTR2_MASK 0x2000000u
//! Constant PCIE_CTR1_0 - Disabled
#define CONST_CGU_CLKGSR2_PCIE_CTR2_PCIE_CTR1_0 0x0
//! Constant PCIE_CTR1_1 - Enabled
#define CONST_CGU_CLKGSR2_PCIE_CTR2_PCIE_CTR1_1 0x1

//! Field PCIE_PD2 - PCIe #2 Power Status PD Value
#define CGU_CLKGSR2_PCIE_PD2_POS 30
//! Field PCIE_PD2 - PCIe #2 Power Status PD Value
#define CGU_CLKGSR2_PCIE_PD2_MASK 0xC0000000u
//! Constant PCIE_NOR - PCIe_NOR
#define CONST_CGU_CLKGSR2_PCIE_PD2_PCIE_NOR 0x0
//! Constant PCIE_L0S - PCIe_L0S
#define CONST_CGU_CLKGSR2_PCIE_PD2_PCIE_L0S 0x1
//! Constant PCIE_L1 - PCIe_L1
#define CONST_CGU_CLKGSR2_PCIE_PD2_PCIE_L1 0x2
//! Constant PCIE_L3 - PCIe_L3
#define CONST_CGU_CLKGSR2_PCIE_PD2_PCIE_L3 0x3

//! @}

//! \defgroup CGU_CLKGCR2_A Register CGU_CLKGCR2_A - CGU Clock Gating Control Register 2_A
//! @{

//! Register Offset (relative)
#define CGU_CLKGCR2_A 0x134
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_CLKGCR2_A 0x16200134u

//! Register Reset Value
#define CGU_CLKGCR2_A_RST 0x00000000u

//! Field PCIE_CTR0 - PCIe Controller/PHY Power Down
#define CGU_CLKGCR2_A_PCIE_CTR0_POS 1
//! Field PCIE_CTR0 - PCIe Controller/PHY Power Down
#define CGU_CLKGCR2_A_PCIE_CTR0_MASK 0x2u
//! Constant PCIE_CTR0 - no action
#define CONST_CGU_CLKGCR2_A_PCIE_CTR0_PCIE_CTR0 0x0
//! Constant PCIE_CTR0_1 - Enable
#define CONST_CGU_CLKGCR2_A_PCIE_CTR0_PCIE_CTR0_1 0x1

//! Field PCIE_CTR1 - PCIe Controller/PHY 1 Power Down
#define CGU_CLKGCR2_A_PCIE_CTR1_POS 17
//! Field PCIE_CTR1 - PCIe Controller/PHY 1 Power Down
#define CGU_CLKGCR2_A_PCIE_CTR1_MASK 0x20000u
//! Constant PCIE_CTR1_0 - no action
#define CONST_CGU_CLKGCR2_A_PCIE_CTR1_PCIE_CTR1_0 0x0
//! Constant PCIE_CTR1_1 - Enable
#define CONST_CGU_CLKGCR2_A_PCIE_CTR1_PCIE_CTR1_1 0x1

//! Field PCIE_CTR2 - PCIe Controller/PHY 2 Power Down
#define CGU_CLKGCR2_A_PCIE_CTR2_POS 25
//! Field PCIE_CTR2 - PCIe Controller/PHY 2 Power Down
#define CGU_CLKGCR2_A_PCIE_CTR2_MASK 0x2000000u
//! Constant PCIE_CTR1_0 - no action
#define CONST_CGU_CLKGCR2_A_PCIE_CTR2_PCIE_CTR1_0 0x0
//! Constant PCIE_CTR1_1 - Enable
#define CONST_CGU_CLKGCR2_A_PCIE_CTR2_PCIE_CTR1_1 0x1

//! @}

//! \defgroup CGU_CLKGCR2_B Register CGU_CLKGCR2_B - CGU Clock Gating Control Register 2_B
//! @{

//! Register Offset (relative)
#define CGU_CLKGCR2_B 0x138
//! Register Offset (absolute) for 1st Instance CGU
#define CGU_CGU_CLKGCR2_B 0x16200138u

//! Register Reset Value
#define CGU_CLKGCR2_B_RST 0x00000000u

//! Field PCIE_CTR0 - PCIe Controller/PHY Power Down
#define CGU_CLKGCR2_B_PCIE_CTR0_POS 1
//! Field PCIE_CTR0 - PCIe Controller/PHY Power Down
#define CGU_CLKGCR2_B_PCIE_CTR0_MASK 0x2u
//! Constant PCIE_CTR0_0 - no action
#define CONST_CGU_CLKGCR2_B_PCIE_CTR0_PCIE_CTR0_0 0x0
//! Constant PCIE_CTR0_1 - Disable
#define CONST_CGU_CLKGCR2_B_PCIE_CTR0_PCIE_CTR0_1 0x1

//! Field PCIE_CTR1 - PCIe Controller/PHY 1 Power Down
#define CGU_CLKGCR2_B_PCIE_CTR1_POS 17
//! Field PCIE_CTR1 - PCIe Controller/PHY 1 Power Down
#define CGU_CLKGCR2_B_PCIE_CTR1_MASK 0x20000u
//! Constant PCIE_CTR1_0 - no action
#define CONST_CGU_CLKGCR2_B_PCIE_CTR1_PCIE_CTR1_0 0x0
//! Constant PCIE_CTR1_1 - Disable
#define CONST_CGU_CLKGCR2_B_PCIE_CTR1_PCIE_CTR1_1 0x1

//! Field PCIE_CTR2 - PCIe Controller/PHY 2 Power Down
#define CGU_CLKGCR2_B_PCIE_CTR2_POS 25
//! Field PCIE_CTR2 - PCIe Controller/PHY 2 Power Down
#define CGU_CLKGCR2_B_PCIE_CTR2_MASK 0x2000000u
//! Constant PCIE_CTR1_0 - no action
#define CONST_CGU_CLKGCR2_B_PCIE_CTR2_PCIE_CTR1_0 0x0
//! Constant PCIE_CTR1_1 - Disable
#define CONST_CGU_CLKGCR2_B_PCIE_CTR2_PCIE_CTR1_1 0x1

//! @}

//! @}

#endif
