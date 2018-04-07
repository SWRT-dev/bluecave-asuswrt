/*******************************************************************************

  Intel SmartPHY DSL PCIe TC Linux driver
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
#ifndef __DSL_TC_H__
#define __DSL_TC_H__

#include <lantiq_atm.h>

typedef enum {
	LTQ_MEI_UNKNOWN = 0,
	/** To register function for getting showtime status. */
	LTQ_MEI_SHOWTIME_CHECK = 1,
	/** To register function for showtime entry signalling. */
	LTQ_MEI_SHOWTIME_ENTER = 2,
	/** To register function for showtime exit signalling. */
	LTQ_MEI_SHOWTIME_EXIT = 3,
	/** To register function for TC-Layer selection.
	For DSL/PP switchover concept (also refer to DSLCPE_SW-858)
	*/
	LTQ_MEI_TC_REQUEST = 4,
	/** To register function for performing reset of TC-Layer.
	(also refer to PPA_SYS-353)
	*/
	LTQ_MEI_TC_RESET = 5,
	LTQ_MEI_ERB_ADDR_GET = 6,
	/** To get ERB PDBRAM and PPE SB BAR base address */
} e_ltq_mei_cb_type;

/**
 * This definition is used by the MEI Driver within TC request trigger to
 * inform the PPA about the TC-Layer that has been negotiated during handshake
 * and therefore needs to be loaded/configured on the PPA side.
 */
typedef enum {
	/** Request to disable the TC */
	MEI_TC_REQUEST_OFF = 0,
	/** Request to load/configure the PTM (EFM) TC */
	MEI_TC_REQUEST_PTM = 1,
	/** Request to load/configure the ATM TC */
	MEI_TC_REQUEST_ATM = 2,
	/** Delimiter only! */
	MEI_TC_REQUEST_LAST = 3
} mei_tc_request_type;

/**
 * This bit-field definition is used by the MEI Driver to trigger reset
 * handling(s) within TC-Layer of the PPA.
 */
typedef enum {
	/** This value is defined for initialization purpose only.
	 At least one of the defined bits of the bitmask shall be set.
	 */
	MEI_TC_RESET_CLEAN = 0x00000000,
	/** Request to perform a reset of the TC codeword buffer */
	MEI_TC_RESET_CW_BUFFER = 0x00000001
} mei_tc_reset_type;

struct ltq_mei_atm_showtime_info {
	void *check_ptr;
	void *enter_ptr;
	void *exit_ptr;
	void *req_tc_ptr;
	void *tc_reset_ptr;
	void *erb_addr_ptr;
};

/** Function pointer for new DSL/PP switchover concept
 * (also refer to DSLCPE_SW-858)
 * \param line            Line number
 * \param tc_reqest_type  TC-Layer to be used/configured
 * \param is_bonding      Defines whether bonding is used (>=1) or not (0)
 * \return                0 if successful
 */
typedef int (*mei_tc_request_t)(
	const unsigned char line,
	mei_tc_request_type tc_reqest_type,
	int is_bonding);

/** Function pointer for triggering a reset of the TC
 * (also refer to PPA_SYS-353)
 * \param line            Line number
 * \param tc_reset_type   TC Reset type to be performed
 * \return                0 if successful
 */
typedef int (*mei_tc_reset_t)(
	const unsigned char line,
	mei_tc_reset_type tc_reset_type);

/** Function pointer to get ERB data base address and
 * descriptor base address
 * \param line            Line number
 * \param data_addr       ERB data base address
 * \param desc_addr       ERB descriptor base address
 * \return                0 if successful
 * data_addr and desc_addr is 64KB aligned address
 */
typedef int (*mei_erb_addr_get_t) (const unsigned char line,
	unsigned int *data_addr, unsigned int *desc_addr);

extern void *ppa_callback_get(e_ltq_mei_cb_type);
extern int ppa_callback_set(e_ltq_mei_cb_type, void *);
typedef int (*ltq_mei_atm_showtime_check_t)(const unsigned char,
		int *, struct port_cell_info *, void **);
typedef int (*ltq_mei_atm_showtime_enter_t)(const unsigned char,
		struct port_cell_info *, void *);
typedef int (*ltq_mei_atm_showtime_exit_t)(const unsigned char);

#define INTEL_PTM_TC_IOCTL_STATS (SIOCDEVPRIVATE + 1)
#define INTEL_ATM_TC_IOCTL_STATS  _IOR('a', ATMIOC_PHYTYP, struct atmif_sioc)

struct intel_tc_ptm_sl_stats {
	__u64 wrx_bc_overdrop;
	__u64 wrx_bc_user_cw;
	__u64 wrx_bc_idle_cw;
	__u64 wtx_bc_cw;
	__u64 wrx_gif_total_pdu;
	__u64 wrx_gif_drop_pdu;
	__u64 wrx_gif_crc_err_pdu;
	__u64 wrx_gif_violated_cw;
	__u64 wrx_gif_total_bytes;
	__u64 wtx_gif_total_pdu;
	__u64 wtx_gif_total_bytes;
	__u64 rx_pkts;
	__u64 rx_bytes;
	__u64 rx_errs;
	__u64 rx_drops;
	__u64 tx_pkts;
	__u64 tx_bytes;
	__u64 tx_errs;
	__u64 tx_drops;
};

struct intel_tc_ptm_bonding_ds_mib {
	__u32 rx_frag_byte_cnt;
	__u32 rx_byte_cnt;
	__u32 rx_of_frag_byte_cnt;
	__u32 rx_of_byte_cnt;
	__u32 rx_oor_frag_byte_cnt;
	__u32 rx_miss_frag_byte_cnt;
	__u32 rx_to_frag_byte_cnt;
};
struct intel_tc_ptm_bonding_ds_bg_mib {
	__u32 conform_pkt_cnt;
	__u32 conform_frag_cnt;
	__u32 conform_byte_cnt;
	__u32 no_sop_pkt_cnt;
	__u32 no_sop_frag_cnt;
	__u32 no_sop_byte_cnt;
	__u32 no_eop_pkt_cnt;
	__u32 no_eop_frag_cnt;
	__u32 no_eop_byte_cnt;
	__u32 oversize_pkt_cnt;
	__u32 oversize_frag_cnt;
	__u32 oversize_byte_cnt;
	__u32 noncosec_pkt_cnt;
	__u32 noncosec_frag_cnt;
	__u32 noncosec_byte_cnt;
};
struct intel_tc_ptm_bonding_stats {
	__u32 us_gif_mib[8];
	struct intel_tc_ptm_bonding_ds_mib ds_gif_mib[8];
	struct intel_tc_ptm_bonding_ds_bg_mib ds_bg_mib[4];
};

struct intel_tc_ptm_stats {
	struct intel_tc_ptm_sl_stats ptm_sl_stats;
	struct intel_tc_ptm_bonding_stats pmt_bonding_stats;
};
struct intel_tc_atm_queue_stats {
	__u32 valid;
	__u32 vpi;
	__u32 vci;
	__u32 idx;
	__u32 pdu;
	__u32 bytes;
};
struct intel_tc_atm_stats {
	struct intel_tc_atm_queue_stats rx_mib[16];
	struct intel_tc_atm_queue_stats tx_mib[16];
	__u32 wrx_drophtu_cell;
	__u32 wrx_dropdes_pdu;
	__u32 wrx_correct_pdu;
	__u32 wrx_err_pdu;
	__u32 wrx_dropdes_cell;
	__u32 wrx_correct_cell;
	__u32 wrx_err_cell;
	__u32 wrx_total_byte;
	__u32 wtx_total_pdu;
	__u32 wtx_total_cell;
	__u32 wtx_total_byte;
	__u32 aal5_rx_pkts;
	__u32 aal5_rx_bytes;
	__u32 aal5_rx_errors;
	__u32 aal5_rx_dropped;

	__u32 oam_rx_pkts;
	__u32 oam_rx_bytes;
	__u32 oam_rx_errors;
	__u32 oam_rx_dropped;

	__u32 aal5_tx_pkts;
	__u32 aal5_tx_bytes;
	__u32 aal5_tx_errors;
	__u32 aal5_tx_dropped;

	__u32 oam_tx_pkts;
	__u32 oam_tx_bytes;
	__u32 oam_tx_errors;
	__u32 oam_tx_dropped;

};

struct intel_tc_stats {
	__u32 tc_info:16;
	__u32 res:16;
	union {
		struct intel_tc_ptm_stats ptm_tc_stats;
		struct intel_tc_atm_stats atm_tc_stats;
	} stats;
};
#endif /* __DSL_TC_H__ */

