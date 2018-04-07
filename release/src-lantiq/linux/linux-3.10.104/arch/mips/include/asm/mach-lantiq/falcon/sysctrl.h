/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Copyright (C) 2010 Thomas Langer, Lantiq Deutschland
 */

#ifndef __FALCON_SYSCTRL_H
#define __FALCON_SYSCTRL_H

#include <falcon/lantiq_soc.h>

#ifndef SYSCTRL_USE_EXTENDED
# define SYSCTRL_USE_EXTENDED
#endif

/** Mask definition for SYS1.STATUS block */
#define SYSCTRL_SYS1_STATUS			0x80000000
/** Mask definition for SYS1.SHA1 block */
#define SYSCTRL_SYS1_SHA1			0x40000000
/** Mask definition for SYS1.AES block */
#define SYSCTRL_SYS1_AES 			0x20000000
/** Mask definition for SYS1.PCM block */
#define SYSCTRL_SYS1_PCM 			0x10000000
/** Mask definition for SYS1.FSCT block */
#define SYSCTRL_SYS1_FSCT 			0x08000000
/** Mask definition for SYS1.GPTC block */
#define SYSCTRL_SYS1_GPTC 			0x04000000
/** Mask definition for SYS1.MPS block */
#define SYSCTRL_SYS1_MPS 			0x02000000
/** Mask definition for SYS1.DFEV0 block */
#define SYSCTRL_SYS1_DFEV0 			0x01000000
/** Mask definition for SYS1.PADCTRL4 block */
#define SYSCTRL_SYS1_PADCTRL4 			0x00400000
/** Mask definition for SYS1.PADCTRL3 block */
#define SYSCTRL_SYS1_PADCTRL3 			0x00200000
/** Mask definition for SYS1.PADCTRL1 block */
#define SYSCTRL_SYS1_PADCTRL1 			0x00100000
/** Mask definition for SYS1.P4 block */
#define SYSCTRL_SYS1_P4 			0x00040000
/** Mask definition for SYS1.P3 block */
#define SYSCTRL_SYS1_P3 			0x00020000
/** Mask definition for SYS1.P1 block */
#define SYSCTRL_SYS1_P1 			0x00010000
/** Mask definition for SYS1.HOST block */
#define SYSCTRL_SYS1_HOST 			0x00008000
/** Mask definition for SYS1.I2C block */
#define SYSCTRL_SYS1_I2C 			0x00004000
/** Mask definition for SYS1.SSC0 block */
#define SYSCTRL_SYS1_SSC0 			0x00002000
/** Mask definition for SYS1.ASC0 block */
#define SYSCTRL_SYS1_ASC0 			0x00001000
/** Mask definition for SYS1.ASC1 block */
#define SYSCTRL_SYS1_ASC1 			0x00000800
/** Mask definition for SYS1.DCDCAPD block */
#define SYSCTRL_SYS1_DCDCAPD 			0x00000400
/** Mask definition for SYS1.DCDCDDR block */
#define SYSCTRL_SYS1_DCDCDDR 			0x00000200
/** Mask definition for SYS1.DCDC1V0 block */
#define SYSCTRL_SYS1_DCDC1V0 			0x00000100
/** Mask definition for SYS1.TRC2MEM block */
#define SYSCTRL_SYS1_TRC2MEM 			0x00000040
/** Mask definition for SYS1.DDR block */
#define SYSCTRL_SYS1_DDR 			0x00000020
/** Mask definition for SYS1.EBU block */
#define SYSCTRL_SYS1_EBU 			0x00000010

/** Mask definition for SYS_ETH.GPHY1MII2 block */
#define SYSCTRL_SYS_ETH_GPHY1MII2 		0x02000000
/** Mask definition for SYS_ETH.GPHY0MII2 block */
#define SYSCTRL_SYS_ETH_GPHY0MII2 		0x01000000
/** Mask definition for SYS_ETH.PADCTRL2 block */
#define SYSCTRL_SYS_ETH_PADCTRL2 		0x00200000
/** Mask definition for SYS_ETH.PADCTRL0 block */
#define SYSCTRL_SYS_ETH_PADCTRL0 		0x00100000
/** Mask definition for SYS_ETH.P2 block */
#define SYSCTRL_SYS_ETH_P2 			0x00020000
/** Mask definition for SYS_ETH.P0 block */
#define SYSCTRL_SYS_ETH_P0 			0x00010000
/** Mask definition for SYS_ETH.xMII block */
#define SYSCTRL_SYS_ETH_XMII 			0x00000800
/** Mask definition for SYS_ETH.SGMII block */
#define SYSCTRL_SYS_ETH_SGMII 			0x00000400
/** Mask definition for SYS_ETH.GPHY1 block */
#define SYSCTRL_SYS_ETH_GPHY1 			0x00000200
/** Mask definition for SYS_ETH.GPHY0 block */
#define SYSCTRL_SYS_ETH_GPHY0 			0x00000100
/** Mask definition for SYS_ETH.MDIO block */
#define SYSCTRL_SYS_ETH_MDIO 			0x00000080
/** Mask definition for SYS_ETH.GMAC3 block */
#define SYSCTRL_SYS_ETH_GMAC3 			0x00000008
/** Mask definition for SYS_ETH.GMAC2 block */
#define SYSCTRL_SYS_ETH_GMAC2 			0x00000004
/** Mask definition for SYS_ETH.GMAC1 block */
#define SYSCTRL_SYS_ETH_GMAC1 			0x00000002
/** Mask definition for SYS_ETH.GMAC0 block */
#define SYSCTRL_SYS_ETH_GMAC0 			0x00000001

/** Mask definition for SYS_GPE.COP7 block */
#define SYSCTRL_SYS_GPE_COP7 			0x80000000
/** Mask definition for SYS_GPE.COP6 block */
#define SYSCTRL_SYS_GPE_COP6 			0x40000000
/** Mask definition for SYS_GPE.COP5 block */
#define SYSCTRL_SYS_GPE_COP5 			0x20000000
/** Mask definition for SYS_GPE.COP4 block */
#define SYSCTRL_SYS_GPE_COP4 			0x10000000
/** Mask definition for SYS_GPE.COP3 block */
#define SYSCTRL_SYS_GPE_COP3 			0x08000000
/** Mask definition for SYS_GPE.COP2 block */
#define SYSCTRL_SYS_GPE_COP2 			0x04000000
/** Mask definition for SYS_GPE.COP1 block */
#define SYSCTRL_SYS_GPE_COP1 			0x02000000
/** Mask definition for SYS_GPE.COP0 block */
#define SYSCTRL_SYS_GPE_COP0 			0x01000000
/** Mask definition for SYS_GPE.PE5 block */
#define SYSCTRL_SYS_GPE_PE5 			0x00200000
/** Mask definition for SYS_GPE.PE4 block */
#define SYSCTRL_SYS_GPE_PE4 			0x00100000
/** Mask definition for SYS_GPE.PE3 block */
#define SYSCTRL_SYS_GPE_PE3 			0x00080000
/** Mask definition for SYS_GPE.PE2 block */
#define SYSCTRL_SYS_GPE_PE2 			0x00040000
/** Mask definition for SYS_GPE.PE1 block */
#define SYSCTRL_SYS_GPE_PE1 			0x00020000
/** Mask definition for SYS_GPE.PE0 block */
#define SYSCTRL_SYS_GPE_PE0 			0x00010000
/** Mask definition for SYS_GPE.ARB block */
#define SYSCTRL_SYS_GPE_ARB 			0x00002000
/** Mask definition for SYS_GPE.FSQM block */
#define SYSCTRL_SYS_GPE_FSQM 			0x00001000
/** Mask definition for SYS_GPE.TMU block */
#define SYSCTRL_SYS_GPE_TMU 			0x00000800
/** Mask definition for SYS_GPE.MRG block */
#define SYSCTRL_SYS_GPE_MRG 			0x00000400
/** Mask definition for SYS_GPE.DISP block */
#define SYSCTRL_SYS_GPE_DISP 			0x00000200
/** Mask definition for SYS_GPE.IQM block */
#define SYSCTRL_SYS_GPE_IQM 			0x00000100
/** Mask definition for SYS_GPE.CPUE block */
#define SYSCTRL_SYS_GPE_CPUE 			0x00000080
/** Mask definition for SYS_GPE.CPUI block */
#define SYSCTRL_SYS_GPE_CPUI 			0x00000040
/** Mask definition for SYS_GPE.GPONE block */
#define SYSCTRL_SYS_GPE_GPONE 			0x00000020
/** Mask definition for SYS_GPE.GPONI block */
#define SYSCTRL_SYS_GPE_GPONI 			0x00000010
/** Mask definition for SYS_GPE.LAN3 block */
#define SYSCTRL_SYS_GPE_LAN3 			0x00000008
/** Mask definition for SYS_GPE.LAN2 block */
#define SYSCTRL_SYS_GPE_LAN2 			0x00000004
/** Mask definition for SYS_GPE.LAN1 block */
#define SYSCTRL_SYS_GPE_LAN1 			0x00000002
/** Mask definition for SYS_GPE.LAN0 block */
#define SYSCTRL_SYS_GPE_LAN0 			0x00000001

/** Chip Version Number Mask */
#define SYSCTRL_CHIPID_VERSION_MASK 		0xF0000000
/** Chip Version Number Offset */
#define SYSCTRL_CHIPID_VERSION_OFFSET 		28
/** Part Number, Constant Part Mask */
#define SYSCTRL_CHIPID_PARTNR_MASK 		0x0FFFF000
/** Part Number, Constant Part Offset */
#define SYSCTRL_CHIPID_PARTNR_OFFSET 		12
/** Manufacturer ID Mask */
#define SYSCTRL_CHIPID_MANID_MASK 		0x00000FFE
/** Manufacturer ID Offset */
#define SYSCTRL_CHIPID_MANID_OFFSET 		1
/** Constant bit Mask */
#define SYSCTRL_CHIPID_CONST1_MASK 		0x00000001
/** Constant bit Offset */
#define SYSCTRL_CHIPID_CONST1_OFFSET 		0

/** PHY_CLKO pin frequency, 25 MHz */
#define SYSCTRL_EXTPHYC_CLKSEL_F25 		0x00000001
/** PHY_CLKO pin frequency, 125 MHz */
#define SYSCTRL_EXTPHYC_CLKSEL_F125 		0x00000002
/** PHY_CLKO pin frequency, 50 MHz */
#define SYSCTRL_EXTPHYC_CLKSEL_F50 		0x00000005

/** GMAC connects to GPHY0_GMII interface */
#define SYSCTRL_MUX_GPHY0_GMII 			0x00000000
/** GMAC connects to GPHY0_MII2 interface */
#define SYSCTRL_MUX_GPHY0_MII2 			0x00000001
/** GMAC connects to GPHY1_GMII interface */
#define SYSCTRL_MUX_GPHY1_GMII 			0x00000002
/** GMAC connects to GPHY1_MII2 interface */
#define SYSCTRL_MUX_GPHY1_MII2 			0x00000003
/** GMAC connects to SGMII interface */
#define SYSCTRL_MUX_SGMII 			0x00000004
/** GMAC connects to xMII0 interface */
#define SYSCTRL_MUX_XMII0 			0x00000005
/** GMAC connects to xMII1 interface */
#define SYSCTRL_MUX_xMII1 			0x00000006

/** FUSE0 SFC field Mask */
#define SYSCTRL_FUSE0_SFC_MASK 			0x80000000
/** FUSE0 SFC field Offset */
#define SYSCTRL_FUSE0_SFC_OFFSET 		31
/** FUSE0 SFC field Select */
#define SYSCTRL_FUSE0_SFC_SEL 			0x80000000
/** FUSE0 SC_MBCFG field Mask */
#define SYSCTRL_FUSE0_SC_MBCFG_MASK		0x40000000
/** FUSE0 SC_MBCFG field Offset */
#define SYSCTRL_FUSE0_SC_MBCFG_OFFSET		30
/** FUSE0 SC_MBCFG field Select */
#define SYSCTRL_FUSE0_SC_MBCFG_SEL 		0x40000000
/** FUSE0 Device Type field Mask */
#define SYSCTRL_FUSE0_F0_MASK 			0x3C000000
/** FUSE0 Device Type field Offset */
#define SYSCTRL_FUSE0_F0_OFFSET 		26
/** FUSE0 Device Type field value: FALCOND */
#define SYSCTRL_FUSE0_F0_FALCOND 		0x00000000
/** FUSE0 Device Type field value: FALCONV */
#define SYSCTRL_FUSE0_F0_FALCONV 		0x04000000
/** FUSE0 Device Type field value: FALCONM */
#define SYSCTRL_FUSE0_F0_FALCONM 		0x08000000
/** FUSE0 Device Type field value: SFP */
#define SYSCTRL_FUSE0_F0_FALCONS 		0x0C000000
/** FUSE0 Device Type field value: D2 */
#define SYSCTRL_FUSE0_F0_FALCOND2 		0x10000000
/** FUSE0 Device Type field value: SR */
#define SYSCTRL_FUSE0_F0_FALCONSR 		0x14000000
/** FUSE0 VCALMM20 Voltage Reference Mask */
#define SYSCTRL_FUSE0_VCALMM20_MASK 		0x03F00000
/** FUSE0 VCALMM20 Voltage Reference Offset */
#define SYSCTRL_FUSE0_VCALMM20_OFFSET 		20
/** FUSE0 VCALMM100 Voltage Reference Mask */
#define SYSCTRL_FUSE0_VCALMM100_MASK 		0x000FC000
/** FUSE0 VCALMM100 Voltage Reference Offset */
#define SYSCTRL_FUSE0_VCALMM100_OFFSET 		14
/** FUSE0 VCALMM400 Voltage Reference Mask */
#define SYSCTRL_FUSE0_VCALMM400_MASK 		0x00003F00
/** FUSE0 VCALMM400 Voltage Reference Offset */
#define SYSCTRL_FUSE0_VCALMM400_OFFSET 		8
/** FUSE0 RCALMM R error correction Mask */
#define SYSCTRL_FUSE0_RCALMM_MASK 		0x000000FF
/** FUSE0 RCALMM R error correction Offset */
#define SYSCTRL_FUSE0_RCALMM_OFFSET 		0

/** FUSE1 Spare fuse1 Mask */
#define SYSCTRL_FUSE1_F1_MASK 			0xFFF00000
/** FUSE1 Spare fuse1 Offset */
#define SYSCTRL_FUSE1_F1_OFFSET 		20
/** FUSE1 FREE_1 Mask */
#define SYSCTRL_FUSE1_FR1_MASK 			0x000F0000
/** FUSE1 FREE_1 Offset */
#define SYSCTRL_FUSE1_FR1_OFFSET 		16
/** FUSE1 DCDC 1V0 OFFSET Mask */
#define SYSCTRL_FUSE1_OFFSET1V0DCDC_MASK   	0x0000F800
/** FUSE1 DCDC 1V0 OFFSET Offset */
#define SYSCTRL_FUSE1_OFFSET1V0DCDC_OFFSET	11
/** FUSE1 DCDC APD OFFSET Mask */
#define SYSCTRL_FUSE1_OFFSETAPDDCDC_MASK 	0x000007C0
/** FUSE1 DCDC APD OFFSET Offset */
#define SYSCTRL_FUSE1_OFFSETAPDDCDC_OFFSET 	6
/** FUSE1 DCDC APD GAIN Mask */
#define SYSCTRL_FUSE1_GAINAPDDCDC_MASK 		0x0000003F
/** FUSE1 DCDC APD GAIN Offset */
#define SYSCTRL_FUSE1_GAINAPDDCDC_OFFSET 	0

/** ANALOG Status of written FUSES Mask */
#define SYSCTRL_ANALOG_FS_MASK 			0xE0000000
/** ANALOG Status of written FUSES Offset */
#define SYSCTRL_ANALOG_FS_OFFSET 		29
/** ANALOG fuse A11 format */
#define SYSCTRL_ANALOG_FS_FS0 			0x00000000
/** ANALOG fuse new format */
#define SYSCTRL_ANALOG_FS_FS1 			0x20000000
/** ANALOG fuse reserved format  */
#define SYSCTRL_ANALOG_FS_FS2 			0x40000000
/** Reserved Analog eFuses Mask */
#define SYSCTRL_ANALOG_A0_MASK 			0x1C000000
/** Reserved Analog eFuses Offset */
#define SYSCTRL_ANALOG_A0_OFFSET 		26
/** ANALOG fuse Absolut Temperature Mask */
#define SYSCTRL_ANALOG_TEMPMM_MASK 		0x03F00000
/** ANALOG fuse Absolut Temperature Offset */
#define SYSCTRL_ANALOG_TEMPMM_OFFSET 		20
/** ANALOG fuse Drive DAC Gain Mask */
#define SYSCTRL_ANALOG_GAINDRIVEDAC_MASK 	0x000F8000
/** ANALOG fuse Drive DAC Gain Offset */
#define SYSCTRL_ANALOG_GAINDRIVEDAC_OFFSET 	15
/** ANALOG fuse Bias Voltage Generation Mask */
#define SYSCTRL_ANALOG_VBGP_MASK 		0x00007000
/** ANALOG fuse Bias Voltage Generation Offset */
#define SYSCTRL_ANALOG_VBGP_OFFSET 		12
/** ANALOG fuse Bias Current Generation Mask */
#define SYSCTRL_ANALOG_IREFBGP_MASK 		0x00000F00
/** ANALOG fuse Bias Current Generation Offset */
#define SYSCTRL_ANALOG_IREFBGP_OFFSET 		8
/** ANALOG fuse Bias Voltage Generation Mask */
#define SYSCTRL_ANALOG_TBGP_MASK 		0x000000E0
/** ANALOG fuse Bias Voltage Generation Offset */
#define SYSCTRL_ANALOG_TBGP_OFFSET 		5
/** ANALOG fuse BIAS DAC Gain Mask */
#define SYSCTRL_ANALOG_GAINBIASDAC_MASK 	0x0000001F
/** ANALOG fuse BIAS DAC Gain Offset */
#define SYSCTRL_ANALOG_GAINBIASDAC_OFFSET 	0

/** CONFIG fuse ddr PU driver Mask */
#define SYSCTRL_CONFIG_DDRPU_MASK 		0xC0000000
/** CONFIG fuse ddr PU driver Offset */
#define SYSCTRL_CONFIG_DDRPU_OFFSET 		30
/** CONFIG fuse ddr PD driver Mask */
#define SYSCTRL_CONFIG_DDRPD_MASK 		0x30000000
/** CONFIG fuse ddr PD driver Mask */
#define SYSCTRL_CONFIG_DDRPD_OFFSET 		28
/** CONFIG fuse Authentification Unit enable Mask */
#define SYSCTRL_CONFIG_SHA1EN_MASK 		0x08000000
/** CONFIG fuse Authentification Unit enable Offset */
#define SYSCTRL_CONFIG_SHA1EN_OFFSET 		27
/** CONFIG fuse Encryption Unit enable Mask */
#define SYSCTRL_CONFIG_AESEN_MASK 		0x04000000
/** CONFIG fuse Encryption Unit enable Offset */
#define SYSCTRL_CONFIG_AESEN_OFFSET 		26
/** CONFIG fuse Subversion Number Mask */
#define SYSCTRL_CONFIG_SUBVERS_MASK 		0x03C00000
/** CONFIG fuse Subversion Number Offset  */
#define SYSCTRL_CONFIG_SUBVERS_OFFSET 		22
/** CONFIG fuse Subversion Number: AV1 */
#define SYSCTRL_CONFIG_SUBVERS_AV1 		0x00000000
/** CONFIG fuse Subversion Number: AV2 */
#define SYSCTRL_CONFIG_SUBVERS_AV2 		0x00400000
/** CONFIG fuse Subversion Number: AV3 */
#define SYSCTRL_CONFIG_SUBVERS_AV3 		0x00800000
/** CONFIG fuse Subversion Number: AV4 */
#define SYSCTRL_CONFIG_SUBVERS_AV4 		0x00C00000
/** CONFIG fuse Subversion Number: BV1 */
#define SYSCTRL_CONFIG_SUBVERS_BV1 		0x01000000
/** CONFIG fuse PLL settings Mask */
#define SYSCTRL_CONFIG_PLLINFRA_MASK 		0x003FF000
/** CONFIG fuse PLL settings Offset */
#define SYSCTRL_CONFIG_PLLINFRA_OFFSET 		12
/** CONFIG fuse GPE frequency selection Mask */
#define SYSCTRL_CONFIG_GPEFREQ_MASK 		0x00000C00
/** CONFIG fuse GPE frequency selection Offset */
#define SYSCTRL_CONFIG_GPEFREQ_OFFSET		10
/** CONFIG fuse RM enable Mask */
#define SYSCTRL_CONFIG_RME_MASK 		0x00000200
/** CONFIG fuse RM enable Offset */
#define SYSCTRL_CONFIG_RME_OFFSET 		9
/** CONFIG fuse RM settings Mask */
#define SYSCTRL_CONFIG_RM_MASK 			0x000001E0
/** CONFIG fuse RM settings Offset */
#define SYSCTRL_CONFIG_RM_OFFSET 		5
/** CONFIG fuse RM enable for GPE Memories Mask */
#define SYSCTRL_CONFIG_RMEGPE_MASK 		0x00000010
/** CONFIG fuse RM enable for GPE Memories Offset */
#define SYSCTRL_CONFIG_RMEGPE_OFFSET 		4
/** CONFIG fuse RM settings for GPE Memories Mask */
#define SYSCTRL_CONFIG_RMGPE_MASK 		0x0000000F
/** CONFIG fuse RM settings for GPE Memories Offset */
#define SYSCTRL_CONFIG_RMGPE_OFFSET 		0

/** FUSE0 Fuse Selector */
#define SYSCTRL_FUSE_0				0
/** FUSE1 Fuse Selector */
#define SYSCTRL_FUSE_1				1
/** ANALOG Fuse Selector */
#define SYSCTRL_FUSE_ANALOG			2
/** CONFIG Fuse Selector */
#define SYSCTRL_FUSE_CONFIG			3

/** xMII0 Interface Selector */
#define SYSCTRL_INTERFACE_XMII0			0
/** xMII1 Interface Selector */
#define SYSCTRL_INTERFACE_XMII1			1
/** SGMII Interface Selector */
#define SYSCTRL_INTERFACE_SGMII			2

/** Data Rate 10 MBit/s */
#define SYSCTRL_DRC_10				0
/** Data Rate 100 MBit/s */
#define SYSCTRL_DRC_100				1
/** Data Rate 200 MBit/s */
#define SYSCTRL_DRC_200				5
/** Data Rate 1000 MBit/s */
#define SYSCTRL_DRC_1000			2
/** Data Rate 2500 MBit/s */
#define SYSCTRL_DRC_2500			4

/** Linear 1.5 V Regulator set to 1.34V */
#define SYSCTRL_LDO1V5_1V34		        0
/** Linear 1.5 V Regulator set to 1.39V */
#define SYSCTRL_LDO1V5_1V39		        1
/** Linear 1.5 V Regulator set to 1.44V */
#define SYSCTRL_LDO1V5_1V44		        2
/** Linear 1.5 V Regulator set to 1.50V */
#define SYSCTRL_LDO1V5_1V50		        3
/** Linear 1.5 V Regulator set to 1.56V */
#define SYSCTRL_LDO1V5_1V56		        4
/** Linear 1.5 V Regulator set to 1.63V */
#define SYSCTRL_LDO1V5_1V63		        5
/** Linear 1.5 V Regulator set to 1.70V */
#define SYSCTRL_LDO1V5_1V70		        6
/** Linear 1.5 V Regulator set to 1.78V */
#define SYSCTRL_LDO1V5_1V78		        7

unsigned int ltq_sysctl_is_activated(const int module, const unsigned int mask);

unsigned int ltq_sysctl_is_clocked(const int module, const unsigned int mask);

static inline void ltq_sysctl_sys1_activate(unsigned int mask)
{ ltq_sysctl_activate(SYSCTL_SYS1, mask); }
static inline void ltq_sysctl_sys1_deactivate(unsigned int mask)
{ ltq_sysctl_deactivate(SYSCTL_SYS1, mask); }
static inline void ltq_sysctl_sys1_clk_enable(unsigned int mask)
{ ltq_sysctl_clken(SYSCTL_SYS1, mask); }
static inline void ltq_sysctl_sys1_clk_disable(unsigned int mask)
{ ltq_sysctl_clkdis(SYSCTL_SYS1, mask); }
static inline void ltq_sysctl_sys1_reboot(unsigned int mask)
{ ltq_sysctl_reboot(SYSCTL_SYS1, mask); }
static inline
unsigned int ltq_sysctl_sys1_is_activated(const unsigned int mask)
{ return ltq_sysctl_is_activated(SYSCTL_SYS1, mask); }
static inline
unsigned int ltq_sysctl_sys1_is_clocked(const unsigned int mask)
{ return ltq_sysctl_is_clocked(SYSCTL_SYS1, mask); }

static inline void ltq_sysctl_sys_eth_activate(unsigned int mask)
{ ltq_sysctl_activate(SYSCTL_SYSETH, mask); }
static inline void ltq_sysctl_sys_eth_deactivate(unsigned int mask)
{ ltq_sysctl_deactivate(SYSCTL_SYSETH, mask); }
static inline void ltq_sysctl_sys_eth_clk_enable(unsigned int mask)
{ ltq_sysctl_clken(SYSCTL_SYSETH, mask); }
static inline void ltq_sysctl_sys_eth_clk_disable(unsigned int mask)
{ ltq_sysctl_clkdis(SYSCTL_SYSETH, mask); }
static inline void ltq_sysctl_sys_eth_reboot(unsigned int mask)
{ ltq_sysctl_reboot(SYSCTL_SYSETH, mask); }
static inline
unsigned int ltq_sysctl_sys_eth_is_activated(const unsigned int mask)
{ return ltq_sysctl_is_activated(SYSCTL_SYSETH, mask); }
static inline
unsigned int ltq_sysctl_sys_eth_is_clocked(const unsigned int mask)
{ return ltq_sysctl_is_clocked(SYSCTL_SYSETH, mask); }

static inline void ltq_sysctl_sys_gpe_activate(unsigned int mask)
{ ltq_sysctl_activate(SYSCTL_SYSGPE, mask); }
static inline void ltq_sysctl_sys_gpe_deactivate(unsigned int mask)
{ ltq_sysctl_deactivate(SYSCTL_SYSGPE, mask); }
static inline void ltq_sysctl_sys_gpe_clk_enable(unsigned int mask)
{ ltq_sysctl_clken(SYSCTL_SYSGPE, mask); }
static inline void ltq_sysctl_sys_gpe_clk_disable(unsigned int mask)
{ ltq_sysctl_clkdis(SYSCTL_SYSGPE, mask); }
static inline void ltq_sysctl_sys_gpe_reboot(unsigned int mask)
{ ltq_sysctl_reboot(SYSCTL_SYSGPE, mask); }
static inline
unsigned int ltq_sysctl_sys_gpe_is_activated(const unsigned int mask)
{ return ltq_sysctl_is_activated(SYSCTL_SYSGPE, mask); }
static inline
unsigned int ltq_sysctl_sys_gpe_is_clocked(const unsigned int mask)
{ return ltq_sysctl_is_clocked(SYSCTL_SYSGPE, mask); }

unsigned int ltq_sysctl_sys_gpe_hw_is_activated(unsigned int mask);

void ltq_sysctl_sleep_enable(int cpu0, int xbar);

void ltq_sysctl_dgasp_cfg_set(const unsigned int enable,
			      const unsigned int hyst);

void ltq_sysctl_dgasp_cfg_get(unsigned int *enable, unsigned int *hyst);

void ltq_sysctl_clko_enable(const unsigned int enable);

void ltq_sysctl_ldo1v5_cfg_set(const unsigned int enable,
			       const unsigned int control);

void ltq_sysctl_ldo1v5_cfg_get(unsigned int *enable,
			       unsigned int *control);

int ltq_sysctl_fuse_get(const unsigned int fuse, unsigned int *val);

void ltq_sysctl_chipid_get(unsigned int *chipid);

void ltq_sysctl_ephy_clko_set(const unsigned int enable,
			      const unsigned int clk);

int ltq_sysctl_drc_set(const unsigned int interface, const unsigned int drc);

int ltq_sysctl_mac_mux_set(const unsigned int mac, const unsigned int mux);

int ltq_sysctl_mac_mux_get(const unsigned int mac, unsigned int *mux);

int ltq_sysctl_mac_drs_get(const unsigned int mac, unsigned int *drs);

#endif /* __FALCON_SYSCTRL_H */
