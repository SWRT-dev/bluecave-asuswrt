#ifndef	__SMARTPHY_ADDR_DEF_INC
#define	__SMARTPHY_ADDR_DEF_INC

/*==========================================================================
 Defines for "PPE	DSL	Notifications" Module
============================================================================*/
#define	__DREG_BC0_LINK_STATUS							0x7DC0
#define	__DREG_BC1_LINK_STATUS							0x7DD0
#define	__DREG_MEI_STATE								0x7DC1
#define	__DREG_PDBRAM_S_44K_OWN							0x7DC2
#define	__DREG_SIGNATURE								0x7DC3
#define	__DREG_FRAMER_REQUEST_STOPPED					0x7DC7
#define	__DREG_CW_FILL_LEVEL        					0x7DCA
#define	__SFSM_PGCNT0									0x741C
#define	__SFSM_PGCNT1									0x741D

/*==========================================================================
 General Configuraton:	0x2000 - 0x202F
 Need	to be	refined	further
============================================================================*/

#define	__FW_VER_ID													0x2000
#define	__FW_FEATURE												0x2001

#define	__PP32_0_TIME												0x2002	/* New */
#define	__PP32_1_TIME												0x2003	/* New */
#define	__PP32_2_TIME												0x2004	/* New */

#define	__DSL_LINK_DOWN											0x2007	/* New */

/*	#define	__TFA_MAILBOX_PDMA_ADDR0					0x2004	(Deleted)
		#define	__TFA_MAILBOX_PDMA_ADDR1					0x2005	(Deleted)
		#define	__LINK_UP_NOTIFY_ADDR						0x2006	(Deleted)
		#define	__LINK_DOWN_NOTIFY_ADDR					0x2007	(Deleted)*/
#define	__BOND_CONF											0x2008
#define	__US_BG_QMAP										0x2009
#define	__US_BG_GMAP										0x200A
#define	__DS_BG_GMAP										0x200B

#define	__PEER_GIF_LINK_STATE_TMP					0x200C
#define	__ISR_CHECK_CNT_NORMAL_LINK_DOWN	0x200D
#define	__ISRS_SET_CNT_NORMAL_LINK_DOWN		0x200E
#define	__ISR_CHECK_CNT_FORCE_LINK_DOWN		0x2018
#define	__ISRS_SET_CNT_FORCE_LINK_DOWN		0x2019
#define	__LINK_STATE_INTERMEDIATE					0x201C
#define	__LINK_STABLE_CNT									0x201D

/*#define	__CURR_TIME_STAMP							0x200C	(Deleted)
	#define	__E1_DES_PDMA_BAR							0x200D	(Deleted)
	#define	__B1_DES_PDMA_BAR							0x200E	(Deleted)
	#define	__DATA_PTR_PDMA_PREFIX_CFG		0x200F	(Deleted)
																				0x2010	(Reused)*/

/*Only for DEBUG!!*/
#define	_PDMA_RX_CMD_ERR_FLAG						0x200F
#define	_PDMA_TX_CMD_ERR_FLAG						0x2010

#define	__STD_DES_CFG										0x2011
#define	__QOS_CFG												0x2012
#define	__QOSQ_FLOW_CTRL_CFG						0x2013

#define	__CFG_STD_DATA_LEN							__STD_DES_CFG
#define	__TX_QOS_CFG										__QOS_CFG
#define	__CFG_TX_QOSQ_BANDWIDTH_CTRL		__QOSQ_FLOW_CTRL_CFG


#define	__PSAVE_CFG_TX									0x2014
#define	__PSAVE_CFG_BOND								0x2015
#define	__PSAVE_CFG_RX									0x2016

/*																			0x2018
	#define __GPIO_ADDR									0x2019	(Deleted)*/


/*Added	by Naveen	(31/01/2012) --	START*/
#define	__SHAPING_CFG										0x201A	/* New */
#define	__WFQ_CFG												0x201B	/* New */
/*Added	by Naveen	(31/01/2012) --	END*/

/*	#define __GPIO_MODE									0x201C	(Deleted,	Reused)
		#define __GPIO_WM_CFG								0x201D	(Deleted,	Reused)*/

#define	__QOS_SHADOW_TTICK							0x201E
#define	__TX_QOS_CFG_SHADOW							__QOS_SHADOW_TTICK

#define	__TEST_MODE											0x201F
/*#define	__US_CDMA_RX_DES_PDMA_BAR			0x2020	(Deleted)
	#define	__US_CDMA_RX_DES_BASE					0x2021	(Deleted)
	#define	__US_FRAG_READY_NOTICE_ADDR		0x2022	(Deleted)
																				0x2023
																				0x2024
																				0x2025
																				0x2026
																				0x2027
																				0x2028
																				0x2029
	#define	__US_E1_FRAG_DESBA_CFG				0x202A	(Deleted)
	#define	__DS_E1_FRAG_DESBA_CFG				0x202B	(Deleted)
																				0x202C
																				0x202D
																				0x202E
																				0x202F*/

#define	__PP32_0_TASK_CFG								0x2020	/* new */
#define	__PP32_1_TASK_CFG								0x2021	/* new */
#define	__PP32_2_TASK_CFG								0x2027	/* new */
/*must 4 Dword aligned*/
#define	__PP32_0_MODULE_ACTIVITY				0x2024
#define	__PP32_1_MODULE_ACTIVITY				0x2025

/*valid below for PP32	address	range:
		sb_addr range:			0x2000-0x5FFF
	 hw_reg	range	:			0x7000-0x7FFF
 a vrx218	can	access any <pp32_addr> in	above	range	in its peer	vrx218 with
	an effective external	address	of	*__BOND_PEER_SB_BASE + (pp32_addr	<< 4)*/
#define	__BOND_PEER_SB_BASE								0xA800	/* New */

#define	PCIE_EP_MSI_SR_ADDR_HIGH					0x1E10
#define	PCIE_EP_MSI_SR_ADDR_LOW						0x2484

/*																			0x2019*/

#define	__DS_PKT_PMAC_HEADER						0x2022	/* new */
#define	__VRX218_FLOW_CTRL_CFG					 0x2023	/*	new	*/

/*==========================================================================
Unified QoS Part
============================================================================*/
#define	__INQ_QOS_CFG_BASE							0x2508	/* 0x2508-0x2509	2	x	1	=	2*/

#define	__US_FAST_PATH_DES_LIST_BASE		0xA070	/*0xA070-0xA0EF	64	x	2	=	128*/
#define	__US_FAST_PATH_DES_LIST_SIZE		128
#define	__US_FAST_PATH_DES_LIST_NUM			64
#define	__US_FAST_PATH_DES_LIST_GUARD		63

#define	__US_CPU_PATH_DEST_LIST_BASE		0x3D00		/* 0x3D00-0x3D7F	64x2=	128*/
#define	__QOSQ_OVERFLOW_STATE_BMAP			0x250c		/* 0x250c-0x250c	1x1=1*/

#define	__QOSQ_CFG_CTXT_BASE						0x2FE0			/* 0x2FE0-0x2FFF	16x2=32*/

#define	__US_QOSQ_DES_LIST_BASE					0xA9F0	/* 0xA9F0-0xADEF	16x32x2=1024*/
																								/*(VRX318:	0x2800)*/
#define	__US_QOSQ_DES_LIST_SIZE					256
#define	__US_QOSQ_DES_LIST_NUM_Q				2		/* Number	of QOSQ	-*/
																						/*one for preemption*/
																						/*one for non-preemption*/
#define	__US_QOSQ_DES_LIST_PERQ_NUM			64	/*Number of descriptors per QOSQ*/


#define	__QOSQ_MIB_BASE									0x2030	/* 0x2030-0x20AF	16x8=128*/
#define	__QOSQ_EVENT_MBOX_INT_CFG				0x3F10	/* 0x3F10-0x3F13	1x4=4*/
/*#define	__QOSQ_PKT_CNT									0x2680	(0x2680-0x268F	16x1=16)*/

#define	__SHAPING_WFQ_CFG_BASE					0x3420	/* 0x3420-0x3473	21x4=84*/

#define	__SWAPQ_CFG_CTXT_BASE						0x2500	/* 0x2500-0x2501	1*2=2*/
#define	__SWAPQ_DES_LIST_BASE						0x3F60	/* 0x3F60-0x3F9F	32*2=64*/

#define	__OUTQ_QOS_CFG_CTXT_BASE				0x2550	/* 0x2550-0x255F	4x4=16*/


/*#define	__US_OUTQ_DES_LIST_BASE					0x2100*/	/*0x2100-0x217F	64x2=128*/
#define	__US_OUTQ_DES_LIST_BASE					0xA1F0	/* 0xA1F0-0xA26F	2x32x2=128*/
#define	__US_OUTQ_DES_LIST_SIZE					128
#define	__US_OUTQ0_DES_LIST_NUM					 32
#define	__US_OUTQ1_DES_LIST_NUM					 32


#define	__DESQ_MBOX_INT_CFG							0x3F14	/* 0x3F14-0x3F1F	6x2=12*/

#define	__US_FP_INQ_DES_CFG_CTXT				0x20B0	/* 0x20B0-0x20B7	1x8=8*/
#define	__US_CPU_INQ_DES_CFG_CTXT				0x20B8	/* 0x20B8-0x20BF	1x8=8*/
#define	__US_QOS_OUTQ_DES_CFG_CTXT_BASE	0x20C0	/* 0x20C0-0x20DF	4x8=32*/

/*==========================================================================
 Des Queues, CDMA	 copy	channel	mgm, des_sync	(SmartPhy	Part)
============================================================================*/
#define	__US_FRAGQ_CFG_CTXT_BASE				0xA810	/* 0xA810-0xA84F	8x8=64*/

/*#define	__US_FRAGQ_DES_LIST_BASE				0x21C0*/	/*0x21C0-0x233F	8x48=384*/
#define	__US_FRAGQ_DES_LIST_BASE				0xA270	/* 0xA270-A3EF	8x16x3=384*/
#define	__US_FRAGQ_DES_LIST_SIZE				384
#define	__US_FRAGQ_DES_LIST_NUM_Q				8	 /*	Number of	fragment queues*/
#define	__US_FRAGQ_DES_LIST_PERQ_NUM		16 /*	Number of	descriptors	per	FRAGQ*/

#define	__US_TC_LOCAL_Q_CFG_CTXT_BASE		0x2E00	/*0x2E00-0x2E7F	16x8=128*/

#define	__US_BP_TC_LOCAL_Q_CFG_CTXT_BASE		0x2390	/* 0x2390-0x23A2	1*19=19*/


/*#define	__US_TC_LOCAL_Q_DES_LIST_BASE		0x2360*/	/* 0x2360-0x2385	1x48=48*/

#define	__US_TC_LOCAL_Q_DES_LIST_BASE		0xA000	/* 0xA000-0xA023	18x2=36*/
#define	__US_TC_LOCAL_Q_DES_LIST_SIZE		36
#define	__US_TC_LOCAL_Q0_DES_LIST_NUM		16
#define	__US_TC_LOCAL_Q1_DES_LIST_NUM		2

#define	__US_TC_LOCAL_Q_DES_LIST_BP_BASE	 0xA030	 /*	0xA030-0xA053	 18x2=36*/
#define	__US_TC_LOCAL_Q_DES_LIST_BP_SIZE	 36
#define	__US_TC_LOCAL_Q0_DES_LIST_BP_NUM	 16
#define	__US_TC_LOCAL_Q1_DES_LIST_BP_NUM	 2

#define	__DS_TC_LOCAL_Q_CFG_CTXT				0x20E0	/* 0x20E0-0x20EF	2x8=16*/

#define __DS_TC_LOCAL_Q_BASE_SRAM_HI               0x0028
#define __DS_TC_LOCAL_Q_BASE_SRAM_LOW              0x4000
#define	__DS_TC_LOCAL_Q_DES_LIST_BASE		0xB000	/* 0xB000-0xB03B	30x2=60*/
																								/*(VRX318:0x24D0)*/
#define	__DS_TC_LOCAL_Q_DES_LIST_SIZE		60
#define	__DS_TC_LOCAL_Q_DES_LIST_NUM		30

#define	__DS_TC_LOCAL_Q_DES_LIST_BP_BASE	 0xB040	 /*0xB040-0xB05D	 30x1=30*/
																									/*(VRX318: 0x24F0)*/
#define	__DS_TC_LOCAL_Q_DES_LIST_BP_SIZE	 30
#define	__DS_TC_LOCAL_Q_DES_LIST_BP_NUM		 30


#define	__DS_FRAGQ_CFG_CTXT_BASE				0xB550	/* 0xB550-0xB55F	2x8=16*/

/*FRAGQ	includes non-bonding and bonding part*/
#define	__DS_FRAGQ_DES_LIST_BASE				0xB300	/* 0xB300-0xB47F	2x96x3=576*/
																								/*(VRX318: 0x26D0)*/
#define	__DS_FRAGQ_DES_LIST_SIZE				576	/*Total size	in number	of dwords*/
#define	__DS_FRAGQ_DES_LIST_NUM_Q				2		/*Number	of fragment	queues*/
#define	__DS_FRAGQ_DES_LIST_PERQ_NUM		96	/*Number of descriptors per FRAGQ*/

#define	__DS_PKT_DESQ_CFG_CTXT					0x24C0	/* 0x24C0-0x24CF	2	 x 8 = 16*/

#define	__DS_PKT_DES_LIST_BASE					0xB100	/* 0xB100-0xB2FF	256x2=512*/
																								/*(VRX318: 0x2600)*/
#define	__DS_PKT_DES_LIST_SIZE					510
#define	__DS_PKT_DES_LIST_NUM						255
#define	__DS_PKT_DES_LIST_GUARD					254

#define	__MAINLOOP_CFG_ADDR						0x27A0
#define	__MAINLOOP_CNT_ADDR						0X27A1

#define	__DES_SYNC_CFG_CTXT							0x2F00	/* 0x2F00-0x2FBF	6x32=192*/
/*In Bonding,	Upstream Bonding Master	only synchronizes	2
	Upstream Queues	(FP	&	CPU) Downstream Bonding Master	only synchronizes
	1	Downstream Queue Since	Peer-to-Peer writes	are	not	allowed, we	need to
	synchronize	Bonding	Descriptors!
		Re-using the space reserved	for	"Descriptor	Sync"
		__DES_SYNC_CFG_CTXT	==>	0x2F00 - 0x2F3F	(Upstream	Bonding	Master)
		__DES_SYNC_CFG_CTXT	==>	0x2F00 - 0x2F1F	(Downstream	Bonding	Master)*/
#define	__BOND_US_DES_SYNC_CFG_CTXT						0x2F48	/*0x2F48-0x2F67;4*8=32*/

#define	__BOND_US_DES_SYNC_QUEUE_ID						0x2F68	/*0x2F68-0x2F68;1*1=1*/

#define	__US_BOND_SOC_SYNC_ADDR_GIF4					0x2F6C	/*0x2F6C-0x2F6C;1*1=1*/
#define	__US_BOND_SOC_SYNC_ADDR_GIF5					0x2F6D	/*0x2F6D-0x2F6D;1*1=1*/
#define	__US_BOND_SOC_SYNC_ADDR_GIF6					0x2F6E	/*0x2F6E-0x2F6E;1*1=1*/
#define	__US_BOND_SOC_SYNC_ADDR_GIF7					0x2F6F	/*0x2F6F-0x2F6F;1*1=1*/

#define	__BOND_US_DES_SYNC_TX_DES_DW0					0x2F78	/*0x2F78-0x2F78;1*1=1*/
#define	__BOND_US_DES_SYNC_TX_DES_DW1					0x2F79	/*0x2F79-0x2F79;1*1=1*/
#define	__BOND_US_DES_SYNC_RX_DES_DW0					0x2F7A	/*0x2F7A-0x2F7A;1*1=1*/
#define	__BOND_US_DES_SYNC_RX_DES_DW1					0x2F7B	/*0x2F7B-0x2F7B;1*1=1*/

#define	__BOND_US_DES_SYNC_BUF_BASE					0x2F80	/*0x2F80-0x2FBF;64 DWORDS*/
#define	__BOND_US_DES_SYNC_ENQ_PKT_CNT			0x2F80	/*0x2F80-0x2F80;1*1=1*/
#define	__BOND_US_DES_SYNC_DEQ_PKT_CNT			0x2F81	/*0x2F81-0x2F81;1*1=1*/
#define	__BOND_US_DES_SYNC_DES_BASE					0x2F88	/*0x2F88-0x2FA7;32 DWORDs*/
#define	__BOND_US_DES_SYNC_DES_BP_BASE			0x2FA8	/*0x2FA8-0x2FB7;16 DWORDs*/

#define	__BOND_DS_DES_SYNC_CFG_CTXT						0x2FC0	/*0x2FC0-0x2FC7;1*8=8*/
#define	__BOND_DS_DES_SYNC_QUEUE_ID						0x2FC8	/*0x2FC8-0x2FC8;1*1=1*/
#define	__DS_BOND_SOC_SYNC_ADDR								0x2FC9	/*0x2FC9-0x2FC9;1*1=1*/
#define	__DS_BOND_SOC_SYNC_ENQ_CNT_ADDR				0x2FCA	/*0x2FCA-0x2FCA;1*1=1*/
#define	__DS_BOND_SOC_SYNC_DEQ_CNT_ADDR				0x2FCB	/*0x2FCB-0x2FCB;1*1=1*/

#define	__BOND_DS_DES_SYNC_BUF_BASE					0x2458	/*0x2458-0x24BF;104DWORDS*/
#define	__BOND_DS_DES_SYNC_ENQ_PKT_CNT			0x2458	/*0x2458-0x2458;1*1=1*/
#define	__BOND_DS_DES_SYNC_DEQ_PKT_CNT			0x2459	/*0x2459-0x2459;1*1=1*/
#define	__BOND_DS_DES_SYNC_DES_BASE					0x2460	/*0x2460-0x249F; 64DWORDS*/
#define	__BOND_DS_DES_SYNC_DES_BP_BASE			0x24A0	/*0x24A0-0x24BF; 32DWORDs*/

#define	__BOND_DES_SYNC_CFG_CTXT_SIZE						8
#define	__BOND_DES_SYNC_ENQ_PKT_CNT_OFFSET			0
#define	__BOND_DES_SYNC_DEQ_PKT_CNT_OFFSET			1
#define	__BOND_DES_SYNC_DES_BASE_OFFSET					2

#define	__CDMA_DES_SYNC_DEST_LIST_BASE	0x27A0	/* 0x27A0-0x27FF	6x4x2x2=96*/

/*----------------------------------------------------------
	Detailed	Memory Allocation
	----------------------------------------------------------*/

/***********************************************
	"Unified QoS"	Module Memory	Allocation - START
	***********************************************/

/*2	INQs;	CPU	Path & Fast	Path*/
#define	__INQ_QOS_CFG_BASE							0x2508
#define	__INQ_QOS_CFG_END								0x2508
#define	__INQ_QOS_CFG_SIZE							1

/*16 QoSQs*/
#define	__QOSQ_CFG_CTXT_BASE						0x2FE0
#define	__QOSQ_CFG_CTXT_END							0x2FFF
#define	__QOSQ_CFG_CTXT_SIZE						2

/*16 QoSQs;	4	L2 Shapers;	1	L3 Shaper*/
#define	__SHAPING_WFQ_CFG_BASE					0x3420
#define	__SHAPING_WFQ_CFG_END						0x3473
#define	__SHAPING_WFQ_CFG_SIZE					4

/*16 QoSQs*/
#define	__QOSQ_MIB_BASE									0x2030
#define	__QOSQ_MIB_END									0x20AF
#define	__QOSQ_MIB_SIZE									8

#define	__QOSQ_PKT_CNT									0x2680

/*#define	__SWAPQ_CFG_CTXT_BASE						0x2500*/
#define	__SWAPQ_CFG_CTXT_DW1_ADDR				0x2501
#define	__SWAPQ_CFG_CTXT_END						0x2501
#define	__SWAPQ_CFG_CTXT_SIZE						2

#define	__OUTQ_QOS_CFG_CTXT_BASE				0x2550
#define	__OUTQ_QOS_CFG_CTXT_END					0x255F
#define	__OUTQ_QOS_CFG_CTXT_SIZE				4

/*==========================================================================
 Defines for ERB Module
============================================================================*/

#define	__OUTQ_ERB0_BASE								0xA3F0	/*	0xA3F0-0xA40F	 16x2=32*/
#define	__OUTQ_ERB0_SIZE								18
#define	__OUTQ_ERB0_NUM									9

#define	__OUTQ_ERB_PEER_BASE						0xA410	/*	0xA410-0xA42F	 16x2=32*/
#define	__OUTQ_ERB_PEER_SIZE						18
#define	__OUTQ_ERB_PEER_NUM							9

#define	__FRAGQ_ERB0_BASE								0xA430	/*	0xA430-0xA44F	 16x2=32*/
#define	__FRAGQ_ERB0_SIZE								18
#define	__FRAGQ_ERB0_NUM								9

#define	__FRAGQ_ERB0_BP_BASE						0xA450	/*	0xA450-0xA45F	 16x1=16*/
#define	__FRAGQ_ERB0_BP_SIZE						9
#define	__FRAGQ_ERB0_BP_NUM							9

#define	__FRAGQ_ERB_PEER_BASE						0xA460	/*	0xA460-0xA47F	 16x2=32*/
#define	__FRAGQ_ERB_PEER_SIZE						18
#define	__FRAGQ_ERB_PEER_NUM						9

#define	__FRAGQ_ERB_PEER_BP_BASE				0xA480	/*	0xA480-0xA48F	 16x1=16*/
#define	__FRAGQ_ERB_PEER_BP_SIZE				9
#define	__FRAGQ_ERB_PEER_BP_NUM					9

#define	__OUTQ_ERB0_CFG_CTXT_BASE				0xA490	/*	0xA490-0xA497	 1x8=8*/
#define	__OUTQ_ERB0_CFG_CTXT_SIZE				8
#define	__OUTQ_ERB0_CFG_CTXT_NUM				1

#define	__OUTQ_ERB_PEER_CFG_CTXT_BASE		0xA4A0	/*	0xA4A0-0xA4A7	 1x8=8*/
#define	__OUTQ_ERB_PEER_CFG_CTXT_SIZE		8
#define	__OUTQ_ERB_PEER_CFG_CTXT_NUM		1

#define	__FRAGQ_ERB0_CFG_CTXT_BASE			0xA4B0	/*	0xA4B0-0xA4B7	 1x8=8*/
#define	__FRAGQ_ERB0_CFG_CTXT_SIZE			8
#define	__FRAGQ_ERB0_CFG_CTXT_NUM				1

#define	__FRAGQ_ERB_PEER_CFG_CTXT_BASE	0xA4C0	/*	0xA4C0-0xA4C7	 1x8=8*/
#define	__FRAGQ_ERB_PEER_CFG_CTXT_SIZE	8
#define	__FRAGQ_ERB_PEER_CFG_CTXT_NUM		1

#define	__BG_ERB_CFG_CTXT_BASE				0xA4D0	/*	0xA4D0-0xA4D3	 4x4=16*/
#define	__BG_ERB_CFG_CTXT_SIZE				16
#define	__BG_ERB_CFG_CTXT_NUM				4

#define	__FRAGQ_ERB0_FREE_FRAG_CNT		0xA4E0
#define	__FRAGQ_ERB_PEER_FREE_FRAG_CNT	0xA4E1

#define	__FRAGQ_ERB0_REL_FRAG_MIB		0xA4F0
#define	__FRAGQ_ERB_PEER_REL_FRAG_MIB	0xA4F1

#define	__OUTQ_ERB0_REL_PKT_MIB			0xA4F2
#define	__OUTQ_ERB_PEER_REL_PKT_MIB		0xA4F3

#define	__ERB_FRAG_MIB					0xA4F4
#define	__ERB_PEER_FRAG_MIB				0xA4F5

#define	__FRAGQ_ERB0_REL_IDX			0xA4F6
#define	__FRAGQ_ERB_PEER_REL_IDX		0xA4F7

#define	__NUM_ERB						2

#define	__ERB_STD_DATA_LEN				0x2354

#define	__ERB_REG_STACK					0x2355 /*	0x2355-0x2357	*/

#define __ERB_TIMER_ON                  0x2358

#define __US_DISPATCH_CNT_CFG           0x24D0
#define __US_DISPATCH_CNT               0x24D1

#define __US_BOND_CNT_CFG               0x24D2
#define __US_BOND_CNT                   0x24D3

/***********************************************
	"Unified QoS"	Module Memory	Allocation - END
	**********************************************/

#define	__DES_SYNC_CFG_CTXT_BASE				__DES_SYNC_CFG_CTXT
#define	__DES_SYNC_CFG_CTXT_BASE_END		0x2FBF

#define	__DES_READ_SYNC_CFG_CTXT_BASE	 __DES_SYNC_CFG_CTXT_BASE
#define	__DES_READ_SYNC_CFG_CTXT_END	 __DES_SYNC_CFG_CTXT_BASE_END

#define	__DES_WRITE_SYNC_CFG_CTXT_BASE	__DES_SYNC_CFG_CTXT_BASE
#define	__DES_WRITE_SYNC_CFG_CTXT_END		__DES_SYNC_CFG_CTXT_BASE_END

/*temp defined US_CDMA_COPY_CH related structure from	 0x2340to0x4f
	(reuse__US_EDMA_COPY_CH_CFG)*/
#define	__US_CDMA_COPY_CH_CFG						0x2340	/* 0x2340-0x2347	1x8=8*/
#define	__US_CDMA_READ_CH_CFG_CTXT			0x2798	/* 0x2798-0x279F	1x8=8*/
/*					VRX518 specific	definition */
#define	__US_CDMA_CHAN_NO												8

#define	__CDMA_READ_COPY_CH_CFG_BASE		__US_CDMA_COPY_CH_CFG
#define	__CDMA_READ_COPY_CH_CFG_END			0x2350
#define	__SIZE_OF_CDMA_COPY_CH_CFG		1
#define	__CDMA_READ_CH_CTXT				__US_CDMA_READ_CH_CFG_CTXT
#define	__CDMA_READ_CH_CTXT_END					0x27A0
#define	__SIZE_OF_CDMA_READ_CH_CTXT			8
#define __US_CDMA_EXT_SB_BASE						0xBA80	/* 0xBA80-0xBC7F	128x4=512*/

#define	__US_LOCAL_Q_SHARED_CDMA_BASE		0xB980
#define	__US_LOCAL_Q_SHARED_CDMA_NUM		128
#define	__US_QOS_Q_SHARED_CDMA_BASE			0xB880
#define	__US_QOS_Q_SHARED_CDMA_NUM			128

/*PPA2.16	PI*/
#define	__DS_CDMA_COPY_CH_CFG						0x2350	/* 0x2350-0x2351	1x2=1*/

#define	__REG_STACK_PI									0x2351	/* 0x2351-0x2352	2x1=2*/

#define	__DES_SIDEBAND_WR_PTR						0x2353	/*0x2352-0x2353	*/

#define	__DES_SIDEBAND_LIST_BASE				0xB780	/* 0xB780-0xB87F	256x1=256*/

/*Match	the	DS_PKT_DES_LIST	SIZE for Single	Line */
#define	__DES_SIDEBAND_LIST_SIZE				__DS_PKT_DES_LIST_NUM

/*Match	the	DS_FRAG_Q_LIST SIZE for Bonding */
#define	__DES_SIDEBAND_BOND_LIST_SIZE			__DS_FRAGQ_DES_LIST_PERQ_NUM


#define	__SOC_FAMILY										0x2507

/*SB	=	0x7000*/
#define	__WRITE_CH_DOORBELL_EXT_ADDR		0x1E700980
#define	__WRITE_CH_DOORBELL_EXT_ADDR_HI	0x1E70
#define	__WRITE_CH_DOORBELL_EXT_ADDR_LO	0x0980

/*SB	=	0x7001*/
#define	__READ_CH_DOORBELL_EXT_ADDR			0x1E709A0
#define	__READ_CH_DOORBELL_EXT_ADDR_HI	0x1E70
#define	__READ_CH_DOORBELL_EXT_ADDR_LO	0x09A0


/*==========================================================================
	TC	Part (E1 part)
	==========================================================================*/
#define	__RX_BC_CFG_STATS_CFG_BASE			0x3E80	/* 0x3E80-0x3EBF	2x32=64*/
#define	__TX_BC_CFG_STATS_CFG_BASE			0x3EC0	/* 0x3EC0-0x3EFF	2x32=64*/
#define	__RX_GIF_CFG_STATS_CFG_BASE			0x3D80	/* 0x3D80-0x3DFF	4x32=128*/
#define	__TX_GIF_CFG_STATS_CFG_BASE			0x3E00	/* 0x3E00-0x3E7F	4x32=128*/
#define	__RX_GIF_MIB_BASE								0x3400	/* 0x3400-0x341F	4x8=32*/

/*---------------------------------------------------
	detailed	SB allocation
	---------------------------------------------------*/

/*---------------------------------------------------
	WAN RX	BC Config/Status/Context Table
	 32	*	2	=	64
	---------------------------------------------------*/
#define	__RX_BC0_CFG_STATS_CFG							__RX_BC_CFG_STATS_CFG_BASE
/* #define __RX_BC1_CFG_STATS_CFG							 0x3EA0*/

/*---------------------------------------------------
	WAN TX	BC Config/Status/Context Table
	32	*	2	=	64
	---------------------------------------------------*/
#define	__TX_BC0_CFG_STATS_CFG							__TX_BC_CFG_STATS_CFG_BASE
/*#define	__TX_BC1_CFG_STATS_CFG							0x3EE0*/

/*---------------------------------------------------
	WAN RX	Path Config/Status/Context Table
	i.e.	Gamma	Interface	Config/Status/Context
	32	*	4	=	128
	---------------------------------------------------*/
#define	__RX_GIF0_CFG_STATS_CFG							__RX_GIF_CFG_STATS_CFG_BASE
/*	#define __RX_GIF1_CFG_STATS_CFG						0x3DA0
		#define __RX_GIF2_CFG_STATS_CFG						0x3DC0
		#define __RX_GIF3_CFG_STATS_CFG						0x3DE0*/

/*---------------------------------------------------
	WAN TX	Path Config/Status/Context Table
	i.e.	Gamma	Interface	Config/Status/Context
	32	*	4	=	128
	---------------------------------------------------*/
#define	__TX_GIF0_CFG_STATS_CFG							__TX_GIF_CFG_STATS_CFG_BASE
/* #define __TX_GIF1_CFG_STATS_CFG						 0x3E20
	#define __TX_GIF2_CFG_STATS_CFG						 0x3E40
	#define __TX_GIF3_CFG_STATS_CFG						 0x3E60*/

/*---------------------------------------------------
	WAN RX	Gamma	Interface	MIB	Table
	4 * 8 = 32
	---------------------------------------------------*/
#define	__RX_GIF0_MIB_BASE									__RX_GIF_MIB_BASE
#define	__RX_GIF1_MIB_BASE									0x3408
#define	__RX_GIF2_MIB_BASE									0x3410
#define	__RX_GIF3_MIB_BASE									0x3418


/*==========================================================================
	Bonding Part	(B1	part)
	==========================================================================*/
#define	__DS_BOND_GIF_LL_DES_BA					0x9300 	/* 0x9300-0x9EFF	1536x2=3072*/

#define	__US_BG_CTXT_BASE								0x3F00	/* 0x3F00-0x3F0F	4x4=16*/
#define	__DS_BOND_LL_CTXT_BASE					0x26A4	/* 0x26A4-0x26C7	9x4=36*/
#define	__DS_BOND_GIF_MIB_BASE					0x2E80	/* 0x2E80-0x2EFF	8x16=128*/
#define	__DS_BG_MIB_BASE								0x3F20	/* 0x3F20-0x3F5F	4x16=64*/
#define	__DS_BG_CTXT_BASE								0x3FE0	/* 0x3FE0-0x3FFF	4x8=32*/
#define	__DS_BOND_GIF_LL_DESBP_BA				0xB900 /* 0xB900-0xBEFF	1536x1=1536*/

#define __TO_BUF_FULL_CNT						0x2456


/*-------------------------------------
	SB	allocation in	detail
	-------------------------------------*/
/*US Bonding Group Context
	Section	5.7	in Spec*/
#define	__US_BG_CTXT												__US_BG_CTXT_BASE
#define	__US_BG_CTXT_SIZE										16
#define	__US_BG_CTXT_SIZE_PER_BG						4

/*DS Bonding Gamma Interface Linked-List Context
	Section	5.8	in Spec*/
#define	__DS_BOND_FREE_LL_CTXT							__DS_BOND_LL_CTXT_BASE
#define	__DS_BOND_FREE_LL_CTXT_SIZE					4

#define	__DS_BOND_GIF_LL_CTXT								0x26A8
#define	__DS_BOND_GIF_LL_CTXT_SIZE					32
#define	__DS_BOND_GIF_LL_CTXT_SIZE_PER_GIF	4

/*DS Bonding Gamma Interface MIB
	Section	5.9	in Spec*/
#define	__DS_BOND_GIF_MIB										__DS_BOND_GIF_MIB_BASE
#define	__DS_BOND_GIF_MIB_SIZE							128
#define	__DS_BOND_GIF_MIB_SIZE_PER_BG				16

/*DS Bonding Group MIB
	Section	5.10 in	Spec*/
#define	__DS_BG_MIB													__DS_BG_MIB_BASE
#define	__DS_BG_MIB_SIZE										64
#define	__DS_BG_MIB_SIZE_PER_BG							16

/*DS Bonding Group Context
	Section	5.11 in	Spec*/
#define	__DS_BG_CTXT												__DS_BG_CTXT_BASE
#define	__DS_BG_CTXT_SIZE										32
#define	__DS_BG_CTXT_SIZE_PER_BG						8

#define	__MAX_PKT_SIZE_CFG					0x2502


/*	Link	UP/DOWN	info
		E1	forwards link	up/down	info generated by	DSL	firmware
		to	B1 with	 __MBOX_IGU4_ISRS	and	__MBOX_IGU4_ISRC
		to	indicate link	UP and DOWN*/
#define	__LINK_UP_DOWN_STATUS								__MBOX_IGU4_ISR


/*==========================================================================
	General Context
	==========================================================================*/
#define	__GENERAL_CONTEXT_BASE							0x3000	/*0x3000-0x31FF	512x1=512*/

/*------------------------------------------------
	detailed	SB allocation
	------------------------------------------------*/
/*US Internal	Variables*/
#define	__US_E1_FRAG_Q_FRAG_MIB							0x3100
#define	__US_E1_FRAG_Q_NON_EMPTY_FLAG				0x3108

#define	__US_E1_FRAG_Q_DES_WRIDX						0x3110
#define	__US_E1_FRAG_Q_DES_RDIDX						0x3118

/*free	des	per	gif	0x3120-0x3127*/
#define	__US_E1_FRAG_Q_FREE_FRAG_CNT				0x3120
/*number	of fragment	released by	release	proc per gif
	0x3128-0x312F*/
#define	__US_E1_FRAG_Q_REL_FAG_MIB					0x3128

/*===========================================================
	Debug Variable
	===========================================================*/
#define	__MAX_XPCI_WDES_NUM									0x3130
#define	__FLUSH_NO_DES_NUM									0x3131

/*===========================================================
	XPCI	desc read	data buffer: 0x3140-0x314F
	===========================================================*/
#define	__XPCI_RDES_DES_BASE								0x3140
#define	__XPCI_RDES_DES_BASE_HW							0x1140
#define	__XPCI_RDES_DES_SIZE								16


/*	0x3150	-	0x31F0*/
#define	__XPCI_DES_READ_OWN_CHECK_CNT				0x3150


/*DS Internal	Variables*/
#define	__DS_SM_E1_FRAG_DES_RDIDX						0x3180
#define	__DS_BM_E1_FRAG_DES_RDIDX						0x3181
#define	__RX_FRAG_DISP_RDIDX								__DS_SM_E1_FRAG_DES_RDIDX

#define	__DS_BOND_GIF_LL_NONEMPTY_FLAG			0x3182

#define	__IS_PKT_FLUSHING										0x3183
#define	__DMA_PKT_FLUSHING_CURR_BG					0x3184
#define	__DMA_TX_CH1_DES_WRPTR							0x3185

/*	task	activity*/
#define	__PP32_0_TASK_ACTV_FLAG							0x3188
#define	__PP32_1_TASK_ACTV_FLAG							0x3189

/*=====================================================================
	debug counter,	to counter cycles	needed to	read/write cross pci
	=====================================================================*/
#define	__READ_NUM													0x3190
#define	__READ_MAX_CYCLES										0x3191
#define	__READ_MIN_CYCLES										0x3192
#define	__TOTAL_READ_CYCLES_HI							0x3194
#define	__TOTAL_READ_CYCLES_LO							0x3195

#define	__WRITE_NUM													0x3198
#define	__WRITE_MAX_CYCLES									0x3199
#define	__WRITE_MIN_CYCLES									0x319A
#define	__TOTAL_WRITE_CYCLES_HI							0x319C
#define	__TOTAL_WRITE_CYCLES_LO							0x319D

/*=====================================================================
	cross PCI optimization	variables
	=====================================================================*/
#define	__IS_XPCI_DES_READ_STATUS						0x31A0
#define	__XPCI_DES_READ_SPTR								0x31A1
#define	__XPCI_DES_READ_EPTR								0x31A2
#define	__XPCI_DES_READ_PDMA_CMD0						0x31A3
#define	__XPCI_DES_READ_PDMA_CMD1						0x31A4

#define	__IS_XPCI_DES_WRITE_STATUS					0x31A5
#define	__XPCI_DES_WRITE_SPTR								0x31A6
#define	__XPCI_DES_WRITE_EPTR								0x31A7
#define	__XPCI_DES_WRITE_PDMA_CMD0					0x31A8
#define	__XPCI_DES_WRITE_PDMA_CMD1					0x31A9


#define	__TOTAL_SUCC_CNT										0x31AA
#define	__TOTAL_FAIL_CNT										0x31AB

#define	__FAILD_DATA_PTR0										0x31AC
#define	__FAILD_DATA_PTR1										0x31AD
#define	__FAILD_DATA_PTR2										0x31AE
#define	__FAILD_DATA_PTR3										0x31AF

/*	0x31B0-0x31BF for 4 consecutive des own bit combinnation*/
#define	__XPCI_DES_READ_OWN_FAIL_CNT				0x31B0

/*0x31C0-0x31CF for 4 data_ptr	violation	combinaton count*/
#define	__XPCI_DES_READ_DPTR_FAIL_CNT				0x31C0



/*==========================================================================
	TC	Part (E1 Part)
	==========================================================================*/
#define	__TC_GENERAL_CONTEXT_BASE				0x3480	/* 0x3480-0x35FF	1x384=384*/

#define	__SFSM0_DATA_BUF_BASE						0x8000	/* 0x8000-0x90FF	256x17=4352*/
																								/*(VRX318: 0x5000)*/
#define	__SFSM0_CTRL_BUF_BASE						0x9100	/* 0x9100-0x91FF	256x1= 256*/
																								/*(VRX318: 0x5EE0)*/


#define	__FFSM0_DATA_BUF_BASE						0x4000	/* 0x4000-0x454F	80x17=1360*/
/*#define	__SFSM1_DATA_BUF_BASE					0x5770*/	/*0x5770-0x5EDF	112x17=1904*/
/*#define	__SFSM1_CTRL_BUF_BASE					0x5F50*/	/* 0x5F50-0x5FBF	112x1=112*/

#define	__FFSM1_DATA_BUF_BASE						0x4550	/*0x4550-0x4A9F	80x17=1360*/
#define	__CTRL_K_TBL_BASE								0x4AA0	/*0x4AA0-0x4AAF	1x16=16*/
#define	__RX_SARPDMA_CMD_BUF_BASE				0x3900	/*0x3900-0x39FF	128x 2=256*/
#define	__TX_SARPDMA_CMD_BUF_BASE				0x3A00	/*0x3A00-0x3AFF	128x 2=256*/
#define	__RX_SAR_CTXT_BUF_BASE					0x3200	/*0x3200-0x33FF	64 x 8=512*/
#define	__RX_PDMA_CTXT_BUF_BASE					0x4C00	/*0x4C00-0x4DFF	64 x 8=512*/
#define	__TX_SAR_CTXT_BUF_BASE					0x4E00	/*0x4E00-0x4FFF	64 x 8=512*/
#define	__TX_PDMA_CTXT_BUF_BASE					0x3B00	/*0x3B00-0x3CFF	64 x 8=512*/
#define	__PDMA_RX_DATA_BUFFER						0x5FC0	/*0x5FC0-0x5FDF	32 x 1=32*/
#define	__PDMA_TX_DATA_BUFFER						0x5FE0	/*0x5FE0-0x5FFF	64 x 1=32*/

/*---------------------------------------------------
	detailed	SB allocation
	---------------------------------------------------*/

/*---------------------------------------------------
	VR9 EFM-TC	general	context	[0x3480..0x35FF]
	---------------------------------------------------*/
#define	__BC_ID															0x3480

/*---------------------------------------------------
	RX	&	TX SAR PDMA	CMD	Write	Pointers
	---------------------------------------------------*/
#define	__TX_SARPDMA_CMD_RDPTR							0x3481
#define	__RX_SARPDMA_CMD_WRPTR							0x3482
#define	__TX_SARPDMA_CMD_WRPTR							0x3483

/*---------------------------------------------------
	RX	DMA	Descriptor - Read	&	Write	Pointers
	---------------------------------------------------*/
#define	__RX_DMA_DESC_RDPTR									0x3484
#define	__RX_DMA_DESC_WRPTR									0x3485

/*---------------------------------------------------
	RX	Context	Use	Flag
	---------------------------------------------------*/
#define	__RX_CTX_USE_FLAG_0									0x3486
#define	__RX_CTX_USE_FLAG_1									0x3487

#define	__RX_CTXT_QID										0x2100
#define	__RX_CTX_REUSE_FLAG_0								0x250E
#define	__RX_CTX_REUSE_FLAG_1								0x250F

/*---------------------------------------------------
	TX	Context	Use	Flag
	---------------------------------------------------*/
#define	__TX_CTX_USE_FLAG_0									0x3488
#define	__TX_CTX_USE_FLAG_1									0x3489

/*---------------------------------------------------
	TX	QoS	SWAP read	pointer
	---------------------------------------------------*/
#define	__FP_IN_DESC_RDPTR									0x348A
#define	__CPU_IN_DESC_RDPTR									0x348B
#define	__SWAP_DESC_RDPTR										0x348C

/*---------------------------------------------------
	Upstream	backpressure GPIO	control	value
	---------------------------------------------------*/
#define	__GPIO_BP_CURR_VAL									0x348D

/*---------------------------------------------------
	Task	pending	flag for power save
	---------------------------------------------------*/
#define	__TASK_PENDING_FLAG_TC_TX									0x3490
#define	__TASK_PENDING_FLAG_TC_RX									0x3491
#define	__TASK_PENDING_FLAG_TC_BOND								0x3492



/*---------------------------------------------------
	Downstream	Pending	Receive	packet counter
	to	avoid	read pointer and write pointer overlap issue
	---------------------------------------------------*/
#define	__DS_PENDING_RECV_PKT_CNT						0x348F
#define	DS_RW_PTR_GUARD											2

/*---------------------------------------------------
	Task	pending	flag for power save	in PP32	1
	---------------------------------------------------*/
/*#define	__TASK_PENDING_FLAG_VRX318_PPA						0x3490*/

#define	__WAKEUP_MIB_VRX318_TX											0x3494
#define	__WAKEUP_MIB_VRX318_RX											0x3495
#define	__WAKEUP_MIB_VRX318_BOND										0x3496

#define	__RTHA_INT_CNT															0x3497	/*NEW*/
#define	__WAKEUP_RTHA_INT_CNT												0x3498	/*NEW*/
#define	__TOTAL_TX_WAKEUP_CNT												0x3499	/*NEW*/
#define	__TOTAL_RX_WAKEUP_CNT												0x349A	/*NEW*/
#define	__TOTAL_BOND_WAKEUP_CNT											0x349B

#define	__RTHA_INT_TIMEOUT													0x349C

/*---------------------------------------------------
	Receive Non Idle	Cell Count
	---------------------------------------------------*/
#define	__RECEIVE_NON_IDLE_CELL_CNT_0				0x34A0
#define	__RECEIVE_NON_IDLE_CELL_CNT_1				0x34A1

/*---------------------------------------------------
	Receive Idle	Cell Count
	---------------------------------------------------*/
#define	__RECEIVE_IDLE_CELL_CNT_0						0x34A2
#define	__RECEIVE_IDLE_CELL_CNT_1						0x34A3

/*---------------------------------------------------
	Transmit	Cell Count
	---------------------------------------------------*/
#define	__TRANSMIT_CELL_CNT_0								0x34A4
#define	__TRANSMIT_CELL_CNT_1								0x34A5

/*---------------------------------------------------
	Receive Packet	Count	-	From Switch
	---------------------------------------------------*/
#define	__FP_RECEIVE_PKT_CNT								0x34A6
#define	__CPU_PATH_RECEIVE_PKT_CNT					0x34A7

/*---------------------------------------------------
	Receive Packet	Count	-	From Switch
	---------------------------------------------------*/
#define	__TIMER_INT_COUNT										0x34A8

/*---------------------------------------------------
	address of	pkt_cnt	that is	used to	control	GPIO
	for VR9,	it is	initialized	to 0x7E08	in tc_init.asm
	for debug/test	purpose, it	should be	changed	to 0x3490
	since 0x7e08	is not writable, note	that the address
	should	be 8 dword aligned
	---------------------------------------------------*/
#define	__GPIO_CTRL_PKT_CNT_ADDR_CFG				0x34A9

/*---------------------------------------------------
	bonding variables
	---------------------------------------------------*/

/*	polling control
		MBOX_IGU6_ISR is	used to	as signal	of new fragment	ready*/
#define	__GIF_TX_POLLING_CTRL								0x34AA
#define	__GIF_TX_FRAG_AVAILABLE							0x34AB


/*	read	index	for	TX descritpor	0x34AC-0X34AF*/
#define	__GIF_TX_DES_RDIDX									0x34AC
#define	__GIF_TX_DES_SIZE										32
#define	__GIF_TX_DES_SIZE_BIT_SHIFT					5

#define	__GIF_TX_DES_BP_SIZE								16
#define	__GIF_TX_DES_BP_SIZE_BIT_SHIFT			4

/*	bit0-3	indicate if	GIF0-GIF3	is up	or down
		0:link is	down,	means	local_state	!= SYNC	or SYNC_TRUE or	remote_state = 0
		1:link is	on,	means	locase_state = SYNC	or SYNC_TRUE and remote_state	=	1*/
#define	__PREV_GIF_LINK_STATE								0xA802
#define	__PEER_GIF_LINK_STATE								0xA803

/* Bonding debug information */
#define	__US_FRAG_OWN_DEBUG									0xA805
#define	__US_FRAG_PEER_PTR_DEBUG							0xAE00

#define	__PREV_LINK_UP_DOWN_STATUS					0x34B2

/*	busy	polling	couter for each	GIF	when link	is down*/
#define	__GIF_BUSY_POLLING_CNT							0x34B3

/*	waiting for RX	descriptor*/
#define	__RX_WAIT_FOR_DES										0x34B4
#define	__CONTEXT_DESC_INIT_RET_ADDR				0x34B5
#define	__RX_NO_DES_CNT											0x34B6

/*Number of	times	DREG_PDBRAM_S_44K_OWN	changes	from
	S_44K_OWN_PPE to	S_44K_OWN_DSL*/
#define	__PDBRAM_OWN_CHG_CNT								0x34B7


/*---------------------------------------------------
	SFSM0 (Pages) - Data
	112 * 17	=	1904
	---------------------------------------------------*/
/*#define	__SFSM0_DATA_BUF_BASE*/
#define	__SFSM0_DATA_BUF_SIZE								4335
#define	__SFSM0_DATA_PAGE_NUM								255

/*---------------------------------------------------
	SFSM0 (Pages) - Ctrl
	112 * 1 = 64
	---------------------------------------------------*/
/*#define	__SFSM0_CTRL_BUF_BASE*/
#define	__SFSM0_CTRL_BUF_SIZE								__SFSM0_DATA_PAGE_NUM
#define	__SFSM0_CTRL_PAGE_NUM								__SFSM0_DATA_PAGE_NUM

/*---------------------------------------------------
	FFSM0 (Pages) - Data
	80	*	17 = 1360
	---------------------------------------------------*/
/*#define	__FFSM0_DATA_BUF_BASE*/
#define	__FFSM0_DATA_BUF_SIZE								1360
#define	__FFSM0_DATA_PAGE_NUM								80

/*---------------------------------------------------
	SFSM1 (Pages) - Data
	112 * 17	=	1904
	---------------------------------------------------*/
/*#define	__SFSM1_DATA_BUF_BASE*/
#define	__SFSM1_DATA_BUF_SIZE								0
#define	__SFSM1_DATA_PAGE_NUM								0

/*---------------------------------------------------
	SFSM1 (Pages) - Ctrl
	112 * 1 = 112
	---------------------------------------------------*/
/*#define	__SFSM1_CTRL_BUF_BASE*/
#define	__SFSM1_CTRL_BUF_SIZE								__SFSM1_DATA_PAGE_NUM
#define	__SFSM1_CTRL_PAGE_NUM								__SFSM1_DATA_PAGE_NUM

/*---------------------------------------------------
	FFSM1 (Pages) - Data
	89	*	17 = 1360
	---------------------------------------------------*/
/*#define	__FFSM1_DATA_BUF_BASE*/
#define	__FFSM1_DATA_BUF_SIZE								1360
#define	__FFSM1_DATA_PAGE_NUM								80

/*---------------------------------------------------
	Control K Table
	1 * 16	=	16
	---------------------------------------------------*/
/*#define	__CTRL_K_TBL_BASE*/
#define	__CTRL_K_TBL_SIZE										16
#define	__CTRL_K_TBL_NUM										1

/*---------------------------------------------------
	RX	SARPDMA	Command	Buffer
	128 * 2 = 256
	---------------------------------------------------*/
/*#define	__RX_SARPDMA_CMD_BUF_BASE*/
#define	__RX_SARPDMA_CMD_BUF_SIZE						254
#define	__RX_SARPDMA_CMD_NUM								127

/*---------------------------------------------------
	TX	SARPDMA	Command	Buffer
	128 * 2 = 256
	---------------------------------------------------*/
/*#define	__TX_SARPDMA_CMD_BUF_BASE*/
#define	__TX_SARPDMA_CMD_BUF_SIZE						254
#define	__TX_SARPDMA_CMD_NUM								127

/*---------------------------------------------------
	RX	SAR	Context	Buffer
	64	*	8	=	512
	---------------------------------------------------*/
/*#define	__RX_SAR_CTXT_BUF_BASE*/
#define	__RX_SAR_CTXT_BUF_SIZE							512
#define	__RX_SAR_CTXT_NUM										64

/*---------------------------------------------------
	RX	PDMA Context Buffer
	64	*	8	=	512
	---------------------------------------------------*/
/*#define	__RX_PDMA_CTXT_BUF_BASE*/
#define	__RX_PDMA_CTXT_BUF_SIZE							512
#define	__RX_PDMA_CTXT_NUM									64

/*---------------------------------------------------
	TX	SAR	Context	Buffer
	64	*	8	=	512
	---------------------------------------------------*/
/*#define	__TX_SAR_CTXT_BUF_BASE*/
#define	__TX_SAR_CTXT_BUF_SIZE							512
#define	__TX_SAR_CTXT_NUM										64

/*---------------------------------------------------
	TX	PDMA Context Buffer
	64	*	8	=	512
	---------------------------------------------------*/
/*#define	__TX_PDMA_CTXT_BUF_BASE*/
#define	__TX_PDMA_CTXT_BUF_SIZE							512
#define	__TX_PDMA_CTXT_NUM									64


/*==========================================================================
	Performance Monitor And Event Trace
	==========================================================================*/
#define	__PP32_0_INIT_CNT								0x385C
#define	__PP32_1_INIT_CNT								0x385D
#define	__PP32_0_MAIN_LOOP_CNT					0x385E
#define	__PP32_1_MAIN_LOOP_CNT					0x385F
#define	__EVENT_TRACE_INDEX_BASE				0x2570	/*0x2570-0x257F 1x16=16*/

/*use SFSM	RX buffer	to store more	event, start_cmd_process will
	disable store for BC1*/
#define	__EVENT_TRACE_USE_BC1_SFSM_BASE	0x5800	/*0x5800-0x5BFF : 16x64=1024*/

#define	__MODULE_PERF_STATISTICS				0x3860	/*0x3860-0x38FF 20x8=160*/
#define	__PROFILING_START_TIME_HI			0x3474
#define	__PROFILING_START_TIME_LO			0x3475
#define	__PROFILING_END_TIME_HI					0x3476
#define	__PROFILING_END_TIME_LO					0x3477
#define	__RROF_EVENT_CMD							0x3478
#define	__TIMER_SYNC_STATE							0x3479
#define	__TC_RX_FIFO_PKT_CNT						0x347A	/*number of correct packet*/
																					/*received in	RX_FIFO_INT_POLLING	*/

#define	PROF_EVENT_CMD_START						0x1
#define	PROF_EVENT_STATUS_STARTED				0x8001
#define	PROF_EVENT_CMD_STOP							0x2
#define	PROF_EVENT_STATUS_STOPPED				0x8002
#define	TIMER_SYNC_END_STATE						2
#define	EM_TICK_WIDTH										19
#define	EM_CLK_WIDTH										13


/*	frag_header tracing*/
#define	__FRAG_HEADER_TRACE_IDX					0x26C8

#define	FH_TRACE_NUM										128
#define	FH_TRACE_NUM_BIT_WIDTH					7

#define	__EP0_US_SEG_IDX								0x26C8
#define	__EP1_US_SEG_IDX								0x26C9
#define	__EP0_DS_TC_RECV_IDX						0x26CA
#define	__EP1_DS_TC_RECV_IDX						0x26CB
/*#define	__EP0_DS_EDMA_PP_IDX						0x26CC*/
/*#define	__EP1_DS_EDMA_PP_IDX						0x26CD*/
#define	__EP0_DS_BONDING_DISP_IDX				0x26CE
#define	__EP1_DS_BONDING_DISP_IDX				0x26CF

#define	__EP0_US_SEG_FH_BASE						0x5800
#define	__EP1_US_SEG_FH_BASE						0x5880
#define	__EP0_DS_TC_RECV_FH_BASE				0x5900
#define	__EP1_DS_TC_RECV_FH_BASE				0x5980
/*#define	__EP0_DS_EDMA_PP_FH_BASE				0x5A00*/
/*#define	__EP1_DS_EDMA_PP_FH_BASE				0x5A80*/
#define	__EP0_DS_BONDING_DISP_FH_BASE		0x5B00
#define	__EP1_DS_BONDING_DISP_FH_BASE		0x5B80


/*==========================================================================
	Free	Spaces
	==========================================================================*/
/*--------------------------------------------------------------------------
		From		To			 Size
----------------------------------------------------------------------------*/


/*==========================================================================
	FROM	vr9	bonding
	--------------
	PDMA Registers
	--------------*/
#define	__DMAL_BAR0_FIFO_STATUS							0x7600
#define	__DMAL_BAR1_FIFO_STATUS							0x7601
#define	__PDMA_BAR0													0x7602
#define	__PDMA_BAR1													0x7603

#define	__BAR0_WINDOW_SB_ADDR								0x6000
#define	__BAR0_WINDOW_SB_ADDR_PLUS_1				0x6001
#define	__BAR1_WINDOW_SB_ADDR								0x6800
#define	__BAR1_WINDOW_SB_ADDR_PLUS_1				0x6801


/*	hardware	registers*/
#define	__SAR_PDMA_RX_FW_DATABUF_CFG				0x7F02
#define	__SAR_PDMA_TX_FW_DATABUF_CFG				0x7F03
#define	__PDMA_IER													0x7A0B

/*--------------------
	Constant Definitions
	--------------------*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//---------------------
//Descriptors Addresses
//---------------------
//E1 TX Fragment Descriptor Base Address (Non-Bonding Part)
//Section 5.2.4 (Non-Bonding Part)
#define	__US_E1_FRAG_DESBA											0x3800
#define	__US_E1_FRAG_DES_NUM										128
#define	__US_E1_FRAG_DES_SIZE										2
#define	__US_E1_FRAG_DES_TOTAL_SIZE             256
//__US_E1_FRAG_DES_NUM_PER_GIF = __US_E1_FRAG_DES_NUM/__NUM_US_GIFS
#define	__US_E1_FRAG_DES_NUM_PER_GIF						16
//__US_E1_FRAG_DES_TOTAL_SIZE_PER_GIF=__US_E1_FRAG_DES_TOTAL_SIZE/__NUM_US_GIFS
#define	__US_E1_FRAG_DES_TOTAL_SIZE_PER_GIF			32

//E1 TX Fragment Descriptor Base Address	(Bonding Part)
//Section 5.2.4 (Bonding Part)
#define	__US_E1_FRAG_BP_DESBA										0x3F60
#define	__US_E1_FRAG_BP_DES_NUM									128
#define	__US_E1_FRAG_BP_DES_SIZE								1
#define	__US_E1_FRAG_BP_DES_TOTAL_SIZE					128
//__US_E1_FRAG_BP_DES_NUM_PER_GIF = __US_E1_FRAG_BP_DES_NUM/__NUM_US_GIFS
#define	__US_E1_FRAG_BP_DES_NUM_PER_GIF					16
//__US_E1_FRAG_BP_DES_TOTAL_SIZE_PER_GIF = \
		__US_E1_FRAG_BP_DES_TOTAL_SIZE/__NUM_US_GIFS
#define	__US_E1_FRAG_BP_DES_TOTAL_SIZE_PER_GIF	16

//System Master E1 RX Fragment Descriptor Base Address
#define	__DS_SM_E1_FRAG_DESBA										0x3D00
#define	__DS_SM_E1_FRAG_DES_NUM									32
#define	__DS_SM_E1_FRAG_DES_SIZE								2
#define	__DS_SM_E1_FRAG_DES_TOTAL_SIZE					64

//Bonding Master E1 RX Fragment Descriptor Base Address
#define	__DS_BM_E1_FRAG_DESBA										0x3D40
#define	__DS_BM_E1_FRAG_DES_NUM									32
#define	__DS_SM_E1_FRAG_DES_SIZE								2
#define	__DS_SM_E1_FRAG_DES_TOTAL_SIZE					64

//DS Bonding GIF Link List Descriptor Base Address
#define	__DS_BOND_GIF_LL_DESBA									0x5210
#define	__DS_BOND_GIF_LL_DES_NUM								512

//DMA TX Channel 1 Descriptor List
#define	__DMA_TX_CH1_DESBA											0xB070
#define	__DMA_TX_CH1_DESBA_HW										0x9070
#define	__DMA_TX_CH1_DES_NUM										256
#define	__DMA_TX_CH1_DES_TOTAL_SIZE							512
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*----------------
	FW Context & MIB
	----------------*/

/*US Bonding Priority	Queue	Descriptor Configuraton	Table
	Section	5.4	in Spec	-	Already	defined	in E1*/

/*US Bonding Priority	Queue	Shaping	&	Scheduling Configuration Table
	Section	5.5	in Spec	-	Already	defined	in E1*/

/*US Bonding Priority	Queue	MIB	Counters
	Section	5.6	in Spec	-	Already	defined	in E1*/






/*---------------------
	FW Internal	Variables

	============================================================================
	 FROM	e1


	==========================================================================
	 EFM-TC	Address	definition
	==========================================================================*/

/*
//---------------------------------------------------
// WAN CPU TX channel Descriptor List
// 64 * 2 = 128
//---------------------------------------------------
#define __CPU_TX_DESC_LIST                  0x3D00
#define __CPU_TX_DESC_LIST_SIZE             128
#define __CPU_TX_DESC_NUM                   64

//---------------------------------------------------
// WAN SWAP TX channel Descriptor List Length
// 64 * 2 = 128
//---------------------------------------------------
#define __SWAP_TX_DESC_LIST                 0x2E80
#define __SWAP_TX_DESC_LIST_SIZE            64
#define __SWAP_TX_DESC_NUM                  32

//---------------------------------------------------
// DSL Fastpath TX Channel Descriptor
// 64 * 2 = 128
//---------------------------------------------------
#define __DSL_FASTPATH_TX_DESC_LIST         0x2580
#define __DSL_FASTPATH_TX_DESC_LIST_SIZE    128
#define __DSL_FASTPATH_TX_DESC_NUM          64



//---------------------------------------------------
// DMA RX Channel 1 Descriptor
// 64 * 2 = 128
//---------------------------------------------------
#define	__DMA_RX_CH1_DESC_LIST						__DSL_FASTPATH_TX_DESC_LIST
#define	__DMA_RX_CH1_DESC_LIST_SIZE					__DSL_FASTPATH_TX_DESC_LIST_SIZE
#define	__DMA_RX_CH1_DESC_NUM						__DSL_FASTPATH_TX_DESC_NUM

//---------------------------------------------------
// DSL RX Channel Descriptor
// 64 * 2 = 128
//---------------------------------------------------
#define	__DSL_RX_DESC_LIST							0x2600
#define	__DSL_RX_DESC_LIST_SIZE						128
#define	__DSL_RX_DESC_NUM							64

//---------------------------------------------------
// DMA TX Channel 1 Descriptor List
// 64 * 2 = 128
//---------------------------------------------------
#define	__DMA_TX_CH1_DESC_LIST						__DSL_RX_DESC_LIST
#define	__DMA_TX_CH1_DESC_LIST_SIZE					__DSL_RX_DESC_LIST_SIZE
#define	__DMA_TX_CH1_DESC_NUM						__DSL_RX_DESC_NUM

//---------------------------------------------------
// DSL Shaping and Scheduling Configure
// 8 * 4 = 32
//---------------------------------------------------
#define	__DSL_SHAPING_SCHEDULING_CFG			0x2680
#define	__DSL_SHAPING_SCHEDULING_CFG_SIZE		32
#define	__DSL_SHAPING_SCHEDULING_CFG_NUM		8

//---------------------------------------------------
// ETH0 RX (Pages) - Data
// 20 * 32 = 640
//---------------------------------------------------
#define	__ETH0_RX_DATA_BUF_BASE					0x4550
#define	__ETH0_RX_DATA_BUF_SIZE					640
#define	__ETH0_RX_DATA_PAGE_NUM					20

//---------------------------------------------------
// ETH0 RX (Pages) - Ctrl
// 20 * 1 = 20
//---------------------------------------------------
#define	__ETH0_RX_CTRL_BUF_BASE					0x47D0
#define	__ETH0_RX_CTRL_BUF_SIZE					__ETH0_RX_DATA_PAGE_NUM
#define	__ETH0_RX_CTRL_PAGE_NUM					__ETH0_RX_DATA_PAGE_NUM


*/

/*---------------------------------------------------
	ACA FW	Context
	2 * 8 = 16
	---------------------------------------------------*/
#define	__US_ACA_CFG_CTXT_BASE					0x4BB0	/* 0x4BB0--0x4BB7	1x8=8*/
#define	__DS_ACA_CFG_CTXT_BASE					0x4BB8	/* 0x4BB8--0x4BBF	1x8=8*/


/*----------------------------------------------------------
	ACA Shadow	Registers
	3 * 4 = 12
	*_STATUS	need to	be initialized to	nonzero	by PPE driver
	----------------------------------------------------------*/

#define	__ACA_SHADOW_REG_BASE					0xADF0

#define	__TX_IN_ACA_ACCUM_COUNT					0xADF0

#define	__TX_IN_ACA_ACCUM_STATUS				0xADF1

#define	__TX_IN_QUEUE_PD_BASE_ADDR_OFFSET		0xADF2

#define	__TX_OUT_ACA_ACCUM_COUNT				0xADF3

#define	__TX_OUT_ACA_ACCUM_STATUS				0xADF4

#define	__TX_OUT_QUEUE_PD_BASE_ADDR_OFFSET		0xADF5

#define	__RX_IN_ACA_ACCUM_COUNT					0xADF6

#define	__RX_IN_ACA_ACCUM_STATUS				0xADF7	/*0x1E22AFDC for VRX318*/

#define	__RX_IN_QUEUE_PD_BASE_ADDR_OFFSET		0xADF8

#define	__RX_OUT_ACA_ACCUM_COUNT				0xADF9

#define	__RX_OUT_ACA_ACCUM_STATUS				0xADFA

#define	__RX_OUT_QUEUE_PD_BASE_ADDR_OFFSET		0xADFB


/*dummy	ACA	registers, FPI addresses to	be configured	in *_ACA_CFG_CTXT, used
	for	testing	without	ACA	HW*/

/*#define TX_IN_ACA_ACCUM_SUB					0x4BFC*/	/*0x1E22AFF0 for VRX318	*/
/*#define TX_OUT_ACA_ACCUM_ADD					0x4BFD*/	/*0x1E22AFF4 for VRX318*/


/*#define RX_IN_ACA_ACCUM_SUB					0x4BE0*/	/*0x1E22AF80	for	VRX318 */
/*#define RX_OUT_ACA_ACCUM_ADD					0x4BE1*/	/*0x1E22AF84 for	VRX318*/

#define	_CHK_PREEMP_MAP							0x4BFC

#define	_QID2PREEMP_MAP							0x4BFD

#define	__QOSQ_NO_DROP							0x4BFE

#define	__TOTAL_DS_ACA_DEQ_CNT					0x4BFF

#define	__TOTAL_RX_IN_ACA_ACCUM_CNT				0x347F


/*---------------------------------------------------------------
	Programmable	Address	Mapping	(To	be initialized by	PPE	driver)
	---------------------------------------------------------------*/
#define	__EP_ADDR_BAR							0x2028


/*----------------------------------------------------------------
	ACA TX_IN PD	List
	----------------------------------------------------------------*/
#define	__ACA_TX_IN_PD_LIST_BASE				__US_FAST_PATH_DES_LIST_BASE
#define	__ACA_TX_IN_PD_LIST_SIZE				__US_FAST_PATH_DES_LIST_SIZE
#define	__ACA_TX_IN_PD_LIST_NUM					__US_FAST_PATH_DES_LIST_NUM

/*__ACA_TX_IN_PD_LIST_GUARD	<= __ACA_TX_IN_PD_LIST_NUM - 1*/
#define	__ACA_TX_IN_PD_LIST_GUARD				__US_FAST_PATH_DES_LIST_GUARD

/*----------------------------------------------------------------
	ACA TX_OUT	PD List
	----------------------------------------------------------------*/

#define	__ACA_TX_OUT_PD_LIST_BASE				0xA960	/* 0xA960-0xA9DF	128	 */
#define	__ACA_TX_OUT_PD_LIST_SIZE				128
#define	__ACA_TX_OUT_PD_LIST_NUM				64

/*__ACA_TX_OUT_PD_LIST_GUARD <=	__ACA_TX_OUT_PD_LIST_NUM - 1*/
#define	__ACA_TX_OUT_PD_LIST_GUARD				32


/*----------------------------------------------------------------
	ACA RX_IN PD	List
	----------------------------------------------------------------*/
#define	__ACA_RX_IN_PD_LIST_BASE				__DS_PKT_DES_LIST_BASE
#define	__ACA_RX_IN_PD_LIST_SIZE				__DS_PKT_DES_LIST_SIZE
#define	__ACA_RX_IN_PD_LIST_NUM					__DS_PKT_DES_LIST_NUM

/*__ACA_RX_IN_PD_LIST_GUARD	<= __ACA_RX_IN_PD_LIST_NUM - 1*/
#define	__ACA_RX_IN_PD_LIST_GUARD				__DS_PKT_DES_LIST_GUARD

/*----------------------------------------------------------------
	ACA RX_OUT	PD List
	----------------------------------------------------------------*/
#define	__ACA_RX_OUT_PD_LIST_BASE				__DS_PKT_DES_LIST_BASE
#define	__ACA_RX_OUT_PD_LIST_SIZE				__DS_PKT_DES_LIST_SIZE
#define	__ACA_RX_OUT_PD_LIST_NUM				__DS_PKT_DES_LIST_NUM

/*__ACA_RX_OUT_PD_LIST_GUARD <=	__ACA_RX_OUT_PD_LIST_NUM - 1*/
#define	__ACA_RX_OUT_PD_LIST_GUARD				__DS_PKT_DES_LIST_GUARD


#define	__INT_ACA_TX_IN_CNT							0x3D00
#define	__INT_ACA_TX_IN_TIMEOUT						0x3D01
#define	__INT_ACA_TX_IN_WAKEUP_CNT					0x3D02

#define	__INT_ACA_RX_IN_CNT							0x3D04
#define	__INT_ACA_RX_IN_TIMEOUT						0x3D05
#define	__INT_ACA_RX_IN_WAKEUP_CNT					0x3D06


/*RX interrupt from	PP32_2(TC_RX core) to	PP32_0 (TC_TX	core)
	or PP32_1	(TC_BOND core)*/
#define	__MBOX_PPM_IER								__MBOX_IGU3_IER
#define	__MBOX_PPM_ISR								__MBOX_IGU3_ISR
#define	__MBOX_PPM_ISRS								__MBOX_IGU3_ISRS
#define	__MBOX_PPM_ISRC								__MBOX_IGU3_ISRC

#define	BIT_MBOX_PPM								BIT0
#define	__ISRS_SET_CNT_MBOX_PPM						0x3D08
#define	__ISR_CHECK_CNT_MBOX_PPM					0x3D09

#define	__INT_MBOX0_PPM_CNT							0x3D0A
#define	__INT_MBOX0_PPM_TIMEOUT						0x3D0B
#define	__INT_MBOX0_PPM_WAKEUP_CNT					0x3D0C


/*cdma_read_data_pp	interrupt	from PP32_1(Bonding	Core)	to
	PP32_0	(TC_TX core)*/
#define	__MBOX1_PPM_IER							__MBOX_IGU4_IER
#define	__MBOX1_PPM_ISR							__MBOX_IGU4_ISR
#define	__MBOX1_PPM_ISRS						__MBOX_IGU4_ISRS
#define	__MBOX1_PPM_ISRC						__MBOX_IGU4_ISRC

#define	BIT_MBOX1_PPM							BIT0
#define	__ISRS_SET_CNT_MBOX1_PPM				0x3D10
#define	__ISR_CHECK_CNT_MBOX1_PPM				0x3D11

#define	__INT_MBOX1_PPM_CNT						0x3D12
#define	__INT_MBOX1_PPM_TIMEOUT					0x3D13
#define	__INT_MBOX1_PPM_WAKEUP_CNT				0x3D14

/*DSL showtime/leave showtime interrupt from MIPS to PP32_2 (TC_RX core) */
#define	__MBOX2_PPM_IER						__MBOX_IGU5_IER
#define	__MBOX2_PPM_ISR						__MBOX_IGU5_ISR
#define	__MBOX2_PPM_ISRS					__MBOX_IGU5_ISRS
#define	__MBOX2_PPM_ISRC					__MBOX_IGU5_ISRC

#define	BIT_MBOX2_PPM						BIT0
#define	__ISRS_SET_CNT_MBOX2_PPM			0x3D15
#define	__ISR_CHECK_CNT_MBOX2_PPM			0x3D16

#define	__INT_MBOX2_PPM_CNT					0x3D17
#define	__INT_MBOX2_PPM_WAKEUP_CNT			0x3D18

#define	__SLEEP_UPAGE_CNT					0x3D19

/* 0x2FC0--0x2FDF 1 x 32 = 32  */
#define	__INT_PP32_2_CNT					0x2FC0 


#define	__MODE_SW_CFG						__DREG_MEI_STATE
#define	__MODE_SW_INT						0x250D

/* 0x3FC0--0x3FDF 1 x 32 = 32 */
#define	__MODE_SW_CNT						0x3FC0

#define	__MODE_SW_CNT_CFG					0x4BC0

#define	__DS_ACA_DELAY						0x4BC1

#define	__DS_ACA_TIMEOUT_CNT				0x4BC2

#define	__PCIE_SWITCH_PRESENT				0x4BC3

#define	__TX_IN_OUT_TOTAL_CNT				0x4BC4

#define	__PD_SHADOW_PRESENT					0x4BC5

#define	__TX_OUT_SHADOW_PTR					0x4BC6

#define	__RX_OUT_SHADOW_PTR					0x4BC7

/*PDBRAM addresses used for manual testing */
#define	__TX_OUT_PDBRAM_BASE_HI				0x0023
#define	__TX_OUT_PDBRAM_BASE_LOW			0x6000

#define	__RX_OUT_PDBRAM_BASE_HI				0x0023
#define	__RX_OUT_PDBRAM_BASE_LOW			0x5000


#endif
