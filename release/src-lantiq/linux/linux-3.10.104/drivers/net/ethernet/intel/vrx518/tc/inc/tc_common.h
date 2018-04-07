/*******************************************************************************

  Intel SmartPHY DSL PCIe TC driver
  Copyright(c) 2016 Intel Corporation.

  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.

  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.

  The full GNU General Public License is included in this distribution in
  the file called "COPYING".

*******************************************************************************/
#ifndef __TC_COMMON_H__
#define __TC_COMMON_H__

#define HD_RSRV_SZ		32
#define UMT_DEF_PERIOD		200	/* microseconds */

#define MAX_MTU			(DMA_PACKET_SZ - ETH_HLEN - HD_RSRV_SZ)
#define QOSQ_NUM		2
#define FW_STOP_TIMEOUT		20	/* millisecond */
#define QOS_DISPATCH_OWN	0
#define ACA_TXIN_POLL_INTVAL	10	/* millisecond */
#define ACA_TXIN_HD_DESC_NUM	32
#define ACA_TXOUT_HD_DESC_NUM	32
#define ACA_RXIN_HD_DESC_NUM	32
#define ACA_RXOUT_HD_DESC_NUM	32

enum {
	LOC_IN_SRAM = 0,
	LOC_IN_FPI,
	LOC_IN_PDBRAM,
};

enum {
	TC_SRAM_ADDR = 0,
	TC_FPI_ADDR,
};

/* Mailbox Interrupt Generate Unit */
enum {
	MBOX_IGU0 = 0,
	MBOX_IGU1,
	MBOX_IGU2,
	MBOX_IGU3,
	MBOX_IGU4,
	MBOX_IGU5,
	MBOX_IGU6,
};

/* Powersaving flags */
enum {
	PPE_PS_SAVE = 0,
	PPE_PS_RESTORE,
};

struct ppe_fw_data {
	const u32 *data[PP32_MAX];
	size_t size[PP32_MAX];
};

struct ppe_fw {
	struct fw_ver_id ptm_ver;
	struct fw_ver_id atm_ver;

	struct ppe_fw_data ptm_fw;
	struct ppe_fw_data atm_fw;
};

struct psave_cfg { /*@2014*/
	unsigned int res1:15;
	/*1: start from partial PPE reset, 0: start from full PPE reset */
	unsigned int start_state:1;
	unsigned int res2:15;
	/*1: enable sleep mode, 0: disable sleep mode */
	unsigned int sleep_en:1;
} __packed;

struct rx_bc_cfg {
	unsigned int res1:14;
	/**
	* 0: local receiver is "Looking",
	* 1: local receiver is "Freewheel Sync False",
	* 2: local receiver is "Synced",
	* 3: local receiver is "Freewheel Sync Truee"
	*/
	unsigned int local_state:2;
	unsigned int res2:15;
	/**
	 * 0: remote receiver is "Out-of-Sync",
	 * 1: remote receiver is "Synced"
	 */
	unsigned int remote_state:1;

	/* the number of consecutive "Miss Sync" for
	 * leaving "Freewheel Sync False" to "Looking" (default 3)
	 */
	unsigned int to_false_th:16;
	/**
	 * the number of consecutive "Miss Sync" for
	 * leaving "Freewheel Sync True" to "Freewheel Sync False" (default 7)
	 */
	unsigned int to_looking_th:16;
	/* firmware use only (total 30 dwords) */
	unsigned int rx_cw_rdptr;
	unsigned int cw_cnt;
	unsigned int missed_sync_cnt;
	unsigned int bitmap_last_cw[3]; /* Bitmap of the Last Codeword */
	/* Bitmap of the Second Last Codeword */
	unsigned int bitmap_second_last_cw[3];
	/*Bitmap of the Third Last Codeword */
	unsigned int bitmap_third_last_cw[3];
	unsigned int looking_cw_cnt;
	unsigned int looking_cw_th;
	unsigned int byte_shift_cnt;
	unsigned int byte_shift_val;
	unsigned int res_word1[14];
};

struct rx_gamma_itf_cfg {
	unsigned int res1:31;
	/* 0: "Out-of-Fragment", 1: "In-Fragment" */
	unsigned int receive_state:1;
	unsigned int res2:16;
	/* min length of packet,
	 * padding if packet length is smaller than this value
	 */
	unsigned int rx_min_len:8;
	unsigned int rx_pad_en:1; /*0:  padding disabled, 1: padding enabled */
	unsigned int res3:2;
	/* 0: ETH FCS verification is enabled, 1: disabled */
	unsigned int rx_eth_fcs_ver_dis:1;
	/* 0: ETH FCS field is not removed, 1: ETH FCS field is removed*/
	unsigned int rx_rm_eth_fcs:1;
	/* 0: TC CRC verification enabled, 1: disabled */
	unsigned int rx_tc_crc_ver_dis:1;
	/* 0: 0-bit, 1: 16-bit, 2: 32-bit */
	unsigned int rx_tc_crc_size:2;
	/* if the ETH FCS result matches this magic number,
	 then the packet is valid packet
	 */
	unsigned int rx_eth_fcs_result;
	/* if the TC CRC result matches this magic number,
	 then the packet is valid packet
	 */
	unsigned int rx_tc_crc_result;
	/* TC CRC config, please check the description of
	 SAR context data structure in the hardware spec
	 */
	unsigned int rx_crc_cfg:16;
	unsigned int res4:16;
	/* ETH FCS initialization value */
	unsigned int rx_eth_fcs_init_value;
	/* TC CRC initialization value */
	unsigned int rx_tc_crc_init_value;
	unsigned int res_word1;
	/* 0: normal, the max length is given by MAX_LEN_NORMAL,
	 1: fragment, the max length is given by MAX_LEN_FRAG
	 */
	unsigned int rx_max_len_sel:1;
	unsigned int res5:2;
	/* number of bytes to be inserted/removed */
	unsigned int rx_edit_num2:4;
	/* first byte position to be edited */
	unsigned int rx_edit_pos2:7;
	/* 0: remove, 1: insert */
	unsigned int rx_edit_type2:1;
	/* 0: disable insertion or removal of data, 1: enable */
	unsigned int rx_edit_en2:1;
	unsigned int res6:3;
	/* number of bytes to be inserted/removed */
	unsigned int rx_edit_num1:4;
	/* first byte position to be edited */
	unsigned int rx_edit_pos1:7;
	/* 0: remove, 1: insert */
	unsigned int rx_edit_type1:1;
	/* 0: disable insertion or removal of data, 1: enable */
	unsigned int rx_edit_en1:1;
	unsigned int res_word2[2];
	unsigned int rx_inserted_bytes_1l;
	unsigned int rx_inserted_bytes_1h;
	unsigned int rx_inserted_bytes_2l;
	unsigned int rx_inserted_bytes_2h;
	/* the packet length adjustment, it is sign integer */
	int rx_len_adj;
	unsigned int res_word3[16];
};

struct tx_bc_cfg {
	unsigned int fill_wm:16; /* default 2 */
	unsigned int uflw_wm:16; /* default 2 */
	/* firmware use only (total 31 dwords) */
	unsigned int res0;
	/* FW Internal Use */
	unsigned int holding_pages;
	unsigned int ready_pages;
	unsigned int pending_pages;
	unsigned int cw_wrptr; /* TX codeword write pointer for e*/
	unsigned int res_word[26];
};

struct tx_gamma_itf_cfg {
	unsigned int res_word1;
	unsigned int res1:8;
	/* 4 * (not TX_ETH_FCS_GEN_DIS) + TX_TC_CRC_SIZE */
	unsigned int tx_len_adj:4;
	/* 4 + TX_TC_CRC_SIZE */
	unsigned int tx_crc_off_adj:4;
	/* min length of packet, if length
	 is less than this value, packet is padded
	 */
	unsigned int tx_min_len:8;
	unsigned int res2:3;
	/* 0: ETH FCS generation enabled, 1: disabled */
	unsigned int tx_eth_fcs_gen_dis:1;
	unsigned int res3:2;
	/* 0: 0-bit, 1: 16-bit, 2: 32-bit */
	unsigned int tx_tc_crc_size:2;
	unsigned int res4:24;
	/* TX queue attached to this Gamma interface */
	unsigned int queue_mapping:8;
	unsigned int res_word2;
	/* TC CRC config, please check the
	 description of SAR context data structure in the hardware spec
	 */
	unsigned int tx_crc_cfg:16;
	unsigned int res5:16;
	unsigned int tx_eth_fcs_init_value; /* ETH FCS initialization value */
	unsigned int tx_tc_crc_init_value; /* TC CRC initialization value */
	unsigned int res_word3[9];
	/* firmware use only (total 25 dwords) */
	/* FW Internal Use */
	unsigned int curr_qid;
	unsigned int fill_pkt_state;
	unsigned int post_pkt_state;
	unsigned int curr_pdma_context_ptr;
	unsigned int curr_sar_context_ptr;
	unsigned int des_addr;
	unsigned int des_qid;
	unsigned int rem_data;
	unsigned int rem_crc;
	/* bonding fields */
	unsigned int rem_fh_len;
	unsigned int des_dw0;
	unsigned int des_dw1;
	unsigned int des_bp_dw;
	/* MIB field */
	unsigned int tx_pkt_cnt;
	unsigned int tx_byte_cnt;
	/* Reserved */
	unsigned int res_word4;
};

struct wan_rx_mib_table {
	unsigned int res1[2];
	unsigned int wrx_dropdes_pdu;
	unsigned int wrx_total_bytes;
	unsigned int res2[4];
};

struct sfsm_cfg {
	unsigned int res:14;
	unsigned int rlsync:1;
	unsigned int endian:1;
	unsigned int idlekeep:1;
	unsigned int sen:1;
	unsigned int res1:6;
	unsigned int pnum:8;
};

struct sfsm_dba {
	unsigned int res:17;
	unsigned int dbase:15;
};

struct sfsm_cba {
	unsigned int res:15;
	unsigned int cbase:17;
};

struct ffsm_dba {
	unsigned int res:17;
	unsigned int dbase:15;
};

struct ffsm_cfg  {
	unsigned int en:1;
	unsigned int cf:1;
	unsigned int res:10;
	unsigned int rstptr:1;
	unsigned int clvpage:1;
	unsigned int fidle:1;
	unsigned int endian:1;
	unsigned int res1:8;
	unsigned int pnum:8;
};

enum tc_attr {
	TC_A_UNSPEC = 0,
	TC_A_TC_MODE,
	TC_A_TC_ACTION,
	TC_A_BOND,
	TC_A_LINENO,
	TC_A_MAX,
};

enum tc_cmd {
	TC_C_UNSPEC = 0,
	TC_C_NOTIFY,
	TC_C_MAX,
};

#define TC_MCAST_GRP_NAME	"vrx_mcgrp"
#define TC_FAMILY_NAME		"vrx_tc_notify"

enum tc_type {
	ATM_TC = 1,
	PTM_TC,
};

enum tc_action {
	TC_UNLOADED = 0,
	TC_LOADED,
};

enum bonding_stat {
	NON_BONDING_MODE = 0,
	BONDING_MODE,
};

#define DW_SZ(a)		(sizeof(a) / sizeof(u32))
#define DESC_SZ			8
#define DESC_DWSZ		(DESC_SZ >> 2)

#endif /*__TC_COMMON_H__ */
