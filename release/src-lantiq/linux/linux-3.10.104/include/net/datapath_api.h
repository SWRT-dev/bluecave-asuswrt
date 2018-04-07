#ifndef DATAPATH_API_H
#define DATAPATH_API_H

#include <linux/skbuff.h>
#include <linux/etherdevice.h>	/* eth_type_trans */
#include <linux/atmdev.h>  /*atm_vcc*/
#include <xway/switch-api/lantiq_gsw_api.h> /*Switch related structures */
#include <cpufreq/ltq_cpufreq.h>
#ifdef CONFIG_LTQ_DATAPATH_CPUFREQ
#include <linux/cpufreq.h>
#include <cpufreq/ltq_cpufreq.h>
#endif /* CONFIG_LTQ_DATAPATH_CPUFREQ*/

/*! \file datapath_api.h
\brief This file contains all the API for datapath library on the GRX500 system
This will actually be split into different header files,
but collected together for understanding here.
*/

/** \defgroup GRX500_Datapath_Library GRX500 Datapath Library
\brief All API and defines exported by Datapath Library of GRX500
*/
/* @{ */
/** \defgroup Datapath_Driver_Defines Datapath Driver Defines
\brief Defines used in the Datapath Driver
*/
/** \defgroup Datapath_Driver_Structures Datapath Driver Structures
\brief Datapath Configuration Structures
*/
/** \defgroup Datapath_Driver_API Datapath Driver Library API
\brief  Datapath Driver Library API
*/
/** \defgroup PPA_Accel_API PPA Acceleration Driver API
\brief PPA Acceleration Driver API used for learning and getting
the information necessary to accelerate a flow
*/
/* @} */
#define UNUSED(x) ((void)(x))

#define DP_TX_CAL_CHKSUM     1	/*! Need calculate PMAC. \n
			  Note, make sure pmac place holder already have \n
			   or set flag DP_TX_INSERT_PMAC to insert it
			 */
#define DP_TX_DSL_FCS        2	/*! Only for DSL FCS Checksum calculation */
#define DP_TX_INSERT_PMAC    4	/*! It only for special test purpose so far */
#define DP_TX_OAM            8	/*! OAM packet */
#define DP_TX_TO_DL_MPEFW    0x10/*! Send Pkt directly to DL FW */

/*Default, here define each enum member's value.
  Later it will be overridden to define its mapped string array
*/
#define DP_DBG_ENUM_OR_STRING(name, value, short_name) name = value

/*Note: per bit one variable */
#define DP_DBG_FLAG_LIST {\
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DBG, 0x1, "dbg"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_RX_DATA,\
		0x10, "rx_data"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_RX_DESCRIPTOR, \
		0x20, "rx_desc"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_RX_PASER, \
		0x40, "rx_parse"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_RX_PMAC, \
		0x80, "rx_pmac"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_RX, \
		(0x10 | 0x20 | 0x40 | 0x80), "rx"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_TX_DATA, \
		0x100, "tx_data"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_TX_DESCRIPTOR, \
		0x200, "tx_desc"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_TX_PMAC, \
		0x400, "tx_pmac"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_TX_SUM, \
		0x800, "tx_sum"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_DUMP_TX, \
		(0x100 | 0x200 | 0x400 | 0x800), "tx"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_COC, \
		0x1000, "coc"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_MIB, \
		0x2000, "mib"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_MIB_ALGO, \
		0x4000, "mib_algo"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_CBM_BUF, \
		0x8000, "cbm_buf"), \
	DP_DBG_ENUM_OR_STRING(DP_DBG_FLAG_PAE, \
		0x10000, "pae") \
}

enum DP_DBG_FLAG DP_DBG_FLAG_LIST;


#define MAX_ETH_ALEN 6
#define PMAC_LEN     8

enum PMAC_TCP_TYPE {
	TCP_OVER_IPV4 = 0,
	UDP_OVER_IPV4,
	TCP_OVER_IPV6,
	UDP_OVER_IPV6,
	TCP_OVER_IPV6_IPV4,
	UDP_OVER_IPV6_IPV4,
	TCP_OVER_IPV4_IPV6,
	UDP_OVER_IPV4_IPV6
};

/** \addtogroup Datapath_Driver_Structures */
/* @{ */
/*! \brief  PPA Sub-interface Data structure
\param port_id  Datapath Port Id corresponds to PMAC Port Id
\param subif    Sub-interface Id info. In GRX500, this 15 bits,
		only 13 bits in PAE are handled [14, 11:0]
\note
*/

#ifdef CONFIG_LITTLE_ENDIAN
struct dma_rx_desc_0 {
	/* DWORD 0 */
	union {
		struct {
			uint32_t dest_sub_if_id:15;
			uint32_t eth_type:2;
			uint32_t flow_id:8;
			uint32_t tunnel_id:4;
			uint32_t resv0:3;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_rx_desc_1 {
	/* DWORD 1 */
	union {
		struct {
			uint32_t classid:4;
			uint32_t resv1:4;
			uint32_t ep:4;
			uint32_t color:2;
			uint32_t mpe1:1;
			uint32_t mpe2:1;
			uint32_t enc:1;
			uint32_t dec:1;
			uint32_t nat:1;
			uint32_t tcp_err:1;
			uint32_t session_id:12;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_rx_desc_2 {
	/*DWORD 2 */
	union {
		struct {
			uint32_t data_ptr;
		} __packed field;
		uint32_t all;
	};

} __packed;

struct dma_rx_desc_3 {
	/*DWORD 3 */
	union {
		struct {
			uint32_t data_len:16;
			uint32_t mpoa_mode:2;
			uint32_t mpoa_pt:1;
			uint32_t qid:4;
			uint32_t byte_offset:3;
			uint32_t pdu_type:1;
			uint32_t dic:1;
			uint32_t eop:1;
			uint32_t sop:1;
			uint32_t c:1;
			uint32_t own:1;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_tx_desc_0 {
	/* DWORD 0 */
	union {
		struct {
			uint32_t dest_sub_if_id:15;
			uint32_t eth_type:2;
			uint32_t flow_id:8;
			uint32_t tunnel_id:4;
			uint32_t resv0:3;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_tx_desc_1 {
	/* DWORD 1 */
	union {
		struct {
			uint32_t classid:4;
			uint32_t resv1:4;
			uint32_t ep:4;
			uint32_t color:2;
			uint32_t mpe1:1;
			uint32_t mpe2:1;
			uint32_t enc:1;
			uint32_t dec:1;
			uint32_t nat:1;
			uint32_t tcp_err:1;
			uint32_t session_id:12;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_tx_desc_2 {
	/*DWORD 2 */
	union {
		struct {
			uint32_t data_ptr;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_tx_desc_3 {
	/*DWORD 3 */
	union {
		struct {
			uint32_t data_len:16;
			uint32_t mpoa_mode:2;
			uint32_t mpoa_pt:1;
			uint32_t qid:4;
			uint32_t byte_offset:3;
			uint32_t pdu_type:1;
			uint32_t dic:1;
			uint32_t eop:1;
			uint32_t sop:1;
			uint32_t c:1;
			uint32_t own:1;
		} __packed field;
		uint32_t all;
	};
} __packed;

#else				/*big endian */

struct dma_rx_desc_0 {
	/* DWORD 0 */
	union {
		struct {
			uint32_t resv0:3;
			uint32_t tunnel_id:4;
			uint32_t flow_id:8;
			uint32_t eth_type:2;
			uint32_t dest_sub_if_id:15;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_rx_desc_1 {
	/* DWORD 1 */
	union {
		struct {
			uint32_t session_id:12;
			uint32_t tcp_err:1;
			uint32_t nat:1;
			uint32_t dec:1;
			uint32_t enc:1;
			uint32_t mpe2:1;
			uint32_t mpe1:1;
			uint32_t color:2;
			uint32_t ep:4;
			uint32_t resv1:4;
			uint32_t classid:4;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_rx_desc_2 {
	/*DWORD 2 */
	union {
		struct {
			uint32_t data_ptr;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_rx_desc_3 {
	/*DWORD 3 */
	union {
		struct {
			uint32_t own:1;
			uint32_t c:1;
			uint32_t sop:1;
			uint32_t eop:1;
			uint32_t dic:1;
			uint32_t pdu_type:1;
			uint32_t byte_offset:3;
			uint32_t qid:4;
			uint32_t mpoa_pt:1;
			uint32_t mpoa_mode:2;
			uint32_t data_len:16;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_tx_desc_0 {
	union {
		struct {
			/* DWORD 0 */
			uint32_t resv0:3;
			uint32_t tunnel_id:4;
			uint32_t flow_id:8;
			uint32_t eth_type:2;
			uint32_t dest_sub_if_id:15;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_tx_desc_1 {
	/* DWORD 1 */
	union {
		struct {
			uint32_t session_id:12;
			uint32_t tcp_err:1;
			uint32_t nat:1;
			uint32_t dec:1;
			uint32_t enc:1;
			uint32_t mpe2:1;
			uint32_t mpe1:1;
			uint32_t color:2;
			uint32_t ep:4;
			uint32_t resv1:4;
			uint32_t classid:4;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_tx_desc_2 {
	/*DWORD 2 */
	union {
		struct {
			uint32_t data_ptr;
		} __packed field;
		uint32_t all;
	};
} __packed;

struct dma_tx_desc_3 {
	/*DWORD 3 */
	union {
		struct {
			uint32_t own:1;
			uint32_t c:1;
			uint32_t sop:1;
			uint32_t eop:1;
			uint32_t dic:1;
			uint32_t pdu_type:1;
			uint32_t byte_offset:3;
			uint32_t qid:4;
			uint32_t mpoa_pt:1;
			uint32_t mpoa_mode:2;
			uint32_t data_len:16;
		} __packed field;
		uint32_t all;
	};
} __packed;

#endif
#ifdef CONFIG_CPU_BIG_ENDIAN
/*Note:pmac normally not DWORD aligned. Most time 2 bytes aligment */
struct pmac_rx_hdr {
	/*byte 0 */
	uint8_t res1:1;
	uint8_t ver_done:1;
	uint8_t ip_offset:6;

	/*byte 1 */
	uint8_t tcp_h_offset:5;
	uint8_t tcp_type:3;

	/*byte 2 */
	uint8_t sppid:4;
	uint8_t class:4;

	/*byte 3 */
	uint8_t res2:6;
	uint8_t pkt_type:2;

	/*byte 4 */
	uint8_t res3:1;
	uint8_t redirect:1;
	uint8_t res4:1;
	uint8_t src_sub_inf_id:5;	/*high:mc:1 + vap:4 */

	/*byte 5 */
	uint8_t src_sub_inf_id2:8;	/*low:mc:1 + vap:4 */

	/*byte 6 */
	uint8_t port_map:8;	/*high:port map */

	/*byte 7 */
	uint8_t port_map2:8;	/*low:port map */
} __packed;

struct pmac_tx_hdr {
	/*byte 0 */
	uint8_t tcp_chksum:1;
	uint8_t res1:1;
	uint8_t ip_offset:6;

	/*byte 1 */
	uint8_t tcp_h_offset:5;
	uint8_t tcp_type:3;

	/*byte 2 */
	uint8_t sppid:4;
	uint8_t res:4;

	/*byte 3 */
	uint8_t port_map_en:1;
	uint8_t res2:1;
	uint8_t time_dis:1;
	uint8_t class_en:1;
	uint8_t res3:2;
	uint8_t pkt_type:2;

	/*byte 4 */
	uint8_t fcs_ins_dis:1;
	uint8_t redirect:1;
	uint8_t time_stmp:1;
	uint8_t src_sub_inf_id:5;	/*high:mc:1 + vap:4 */

	/*byte 5 */
	uint8_t src_sub_inf_id2:8;	/*low:mc:1 + vap:4 */

	/*byte 6 */
	uint8_t port_map:8;	/*high:port map */

	/*byte 7 */
	uint8_t port_map2:8;	/*low:port map */
} __packed;

#else

/*Note:pmac normally not DWORD aligned. Most time 2 bytes aligment */
struct pmac_rx_hdr {
	/*byte 0 */
	uint8_t ip_offset:6;
	uint8_t ver_done:1;
	uint8_t res1:1;

	/*byte 1 */
	uint8_t tcp_type:3;
	uint8_t tcp_h_offset:5;

	/*byte 2 */
	uint8_t class:4;
	uint8_t sppid:4;

	/*byte 3 */
	uint8_t pkt_type:2; /* refer to PMAC_TCP_TYPE */
	uint8_t res2:6;

	/*byte 4 */
	uint8_t src_sub_inf_id:5;	/*high:mc:1 + vap:4 */
	uint8_t res4:1;
	uint8_t redirect:1;
	uint8_t res3:1;

	/*byte 5 */
	uint8_t src_sub_inf_id2:8;	/*low:mc:1 + vap:4 */

	/*byte 6 */
	uint8_t port_map:8;	/*high:port map */

	/*byte 7 */
	uint8_t port_map2:8;	/*low:port map */
} __packed;

struct pmac_tx_hdr {
	/*byte 0 */
	uint8_t ip_offset:6;
	uint8_t res1:1;
	uint8_t tcp_chksum:1;

	/*byte 1 */
	uint8_t tcp_type:3;
	uint8_t tcp_h_offset:5;

	/*byte 2 */
	uint8_t res:4;
	uint8_t sppid:4;

	/*byte 3 */
	uint8_t pkt_type:2; /* refer to PMAC_TCP_TYPE */
	uint8_t res3:2;
	uint8_t class_en:1;
	uint8_t time_dis:1;
	uint8_t res2:1;
	uint8_t port_map_en:1;

	/*byte 4 */
	uint8_t src_sub_inf_id:5;	/*high:mc:1 + vap:4 */
	uint8_t time_stmp:1;
	uint8_t redirect:1;
	uint8_t fcs_ins_dis:1;

	/*byte 5 */
	uint8_t src_sub_inf_id2:8;	/*low:mc:1 + vap:4 */

	/*byte 6 */
	uint8_t port_map:8;	/*high:port map */

	/*byte 7 */
	uint8_t port_map2:8;	/*low:port map */
} __packed;

#endif

enum DP_DEV_TYPE {
	PMAC_CPU_ID = 0,
	PMAC_ETH_LAN_START_ID = 1,
	PMAC_ETH_LAN_END_ID = 6,
	PMAC_ALLOC_START_ID = 7,
	PMAC_ALLOC_END_ID = 14,
	PMAC_TUNNEL_DECAP_ID = 14,
	PMAC_ETH_WAN_ID = 15,
	PMAC_END_ID = 16,
};

enum DP_API_STATUS {
	DP_FAILURE = -1,
	DP_SUCCESS = 0,
};

#define DP_F_ENUM_OR_STRING(name, value, short_name) name = value

/*Note:per bit one variable */
#define DP_F_FLAG_LIST  { \
	DP_F_ENUM_OR_STRING(DP_F_DEREGISTER, 0x00000001, "De-Register"), \
	DP_F_ENUM_OR_STRING(DP_F_FAST_ETH_LAN,   0x00000002, "ETH_LAN"), \
	DP_F_ENUM_OR_STRING(DP_F_FAST_ETH_WAN,   0x00000004, "ETH_WAN"),\
	DP_F_ENUM_OR_STRING(DP_F_FAST_WLAN,      0x00000008, "FAST_WLAN"),\
	DP_F_ENUM_OR_STRING(DP_F_FAST_DSL,       0x00000010, "DSL"),\
	DP_F_ENUM_OR_STRING(DP_F_DIRECT,         0x00000020, "DirectPath"), \
	DP_F_ENUM_OR_STRING(DP_F_LOOPBACK,       0x00000040, "Tunne_loop"),\
	DP_F_ENUM_OR_STRING(DP_F_DIRECTPATH_RX,  0x00000080, "Directpath_RX"),\
	DP_F_ENUM_OR_STRING(DP_F_MPE_ACCEL,      0x00000100, "MPE_FW"), \
	DP_F_ENUM_OR_STRING(DP_F_CHECKSUM,       0x00000200, "Checksum"),\
	DP_F_ENUM_OR_STRING(DP_F_DONTCARE,       0x00000400, "DontCare"),\
	DP_F_ENUM_OR_STRING(DP_F_DIRECTLINK,     0x00000800, "DirectLink"),\
	DP_F_ENUM_OR_STRING(DP_F_SUBIF_LOGICAL, 0x00001000, "LogicalIf"), \
	DP_F_ENUM_OR_STRING(DP_F_LRO,           0x00002000, "LRO"), \
	DP_F_ENUM_OR_STRING(DP_F_FAST_DSL_DOWNSTREAM, 0x00004000, "DSL_Down"),\
	DP_F_ENUM_OR_STRING(DP_F_DSL_BONDING, 0x00008000, "DSL_Bonding") \
}

enum DP_F_FLAG DP_F_FLAG_LIST;

/*Note:CBM is using old macro DP_F_PORT_TUNNEL_DECAP */
#define DP_F_PORT_TUNNEL_DECAP  DP_F_LOOPBACK


#define DP_COC_REQ_DP	1 /*COC request from Datapath itself */
#define DP_COC_REQ_ETHERNET	2 /*COC request from ethernet */
#define DP_COC_REQ_VRX318	4 /*COC request from vrx318 */

typedef struct dp_subif {
	int32_t port_id; /*!< Datapath Port Id corresponds to PMAC Port Id */
	int32_t subif:15; /*!< Sub-interface Id info. In GRX500,
			   15 bits, only 13 bits in PAE handled [14, 11:0].\n
			   subif format is mc_flag[14:14]  Res[13:12] VAP[11:8]
			   GRP[7:7] Index/StationID[6:0] \n
			 */
} dp_subif_t;

typedef dp_subif_t PPA_SUBIF;

typedef struct dp_drv_mib {
	u64 rx_drop_pkts;
	u64 rx_error_pkts;
	u64 tx_drop_pkts;
	u64 tx_error_pkts;
	u64 tx_pkts; /* for VRX318 case. \n
			Note, for bonding tx, also difficult to get its
			mib counter from its shared queue mapping
			*/
	u64 tx_bytes; /* for for VRX318 case */
} dp_drv_mib_t;

typedef int32_t(*dp_rx_fn_t)(struct net_device *rxif, struct net_device *txif,
	struct sk_buff *skb, int32_t len);	/*! Device Receive
					   Function callback for packets
					 */
typedef int32_t(*dp_stop_tx_fn_t)(struct net_device *dev);/*! The Driver Stop
							   Tx function callback
							*/
typedef int32_t(*dp_restart_tx_fn_t)(struct net_device *dev);	/*! Driver
						   Restart Tx function callback
							 */
typedef int32_t(*dp_reset_mib_fn_t)(dp_subif_t *subif, int32_t flag);/*! Driver
						reset its mib counter callback
						*/
typedef int32_t(*dp_get_mib_fn_t)(dp_subif_t *subif, dp_drv_mib_t *,
	int32_t flag); /*! Driver get mib counter of the
			specified subif interface.
			*/
typedef int32_t(*dp_get_netif_subifid_fn_t)(struct net_device *netif,
	struct sk_buff *skb, void *subif_data, uint8_t dst_mac[MAX_ETH_ALEN],
	dp_subif_t *subif, uint32_t flags);	/*! get subifid */
typedef int32_t(*dp_coc_confirm_stat)(enum ltq_cpufreq_state new_state,
	enum ltq_cpufreq_state old_st, uint32_t f); /*! Confirm state by COC */

/*!
\brief Datapath Library Registration Callback
\param rx_fn  Rx function callback
\param stop_fn    Stop Tx function callback for flow control
\param restart_fn    Start Tx function callback for flow control
\param get_subifid_fn    Get Sub Interface Id of netif
\note
*/
typedef struct dp_cb {
	dp_rx_fn_t rx_fn;	/*!< Rx function callback */
	dp_stop_tx_fn_t stop_fn;/*!< Stop Tx function callback for
				   flow control
				 */
	dp_restart_tx_fn_t restart_fn;	/*!< Start Tx function callback
					   for flow control
					 */
	dp_get_netif_subifid_fn_t get_subifid_fn; /*!< Get Sub Interface Id
						   of netif/netdevice
						 */
	dp_reset_mib_fn_t reset_mib_fn;  /*!< reset registered device's network
					mib counters
					*/
	dp_get_mib_fn_t get_mib_fn; /*!< reset registered device's
					network mib counters
					*/
	dp_coc_confirm_stat dp_coc_confirm_stat_fn; /*!< once COC confirm the
			state changed, Datatpath will notify Ethernet/VRX318
			driver. and Ethernet/VRX318 driver need to enable/
			disable interrupt or change threshold accordingly
			*/
} dp_cb_t;

/*!
\brief Ingress PMAC port configuration from Datapath Library
\param tx_dma_chan  Tx DMA channel Number for which PMAC
		configuration is to be done
\param err_disc     Is Discard Error Enable
\param pmac    Is Ingress PMAC Hdr Present
\param def_pmac   Is Default PMAC Header configured for Tx DMA Channel
\param def_pmac_pmap Is PortMap to be used from Default PMAC hdr (else use
		Ingress PMAC hdr)
\param def_pmac_en_pmap Is PortMap Enable to be used from Default PMAC hrd
		(else use Ingress PMAC hdr)
\param def_pmac_tc  Is TC (class) to be used from Default PMAC hdr
		(else use Ingress PMAC hdr)
\param def_pmac_en_tc  Are TC bits to be used for TC from Default PMAC hdr
		(else use Ingress PMAC hdr)
		Alternately, EN/DE/MPE1/MPE2 bits can be used for TC
\param def_pmac_subifid  Is Sub-interfaceId to be taken from Default PMAC hdr
		(else use Ingress PMAC hdr)
\param def_pmac_src_port Packet Source Port determined from Default PMAC hdr
		(else use Ingress PMAC hdr)
\param res_ing	Reserved bits
\param def_pmac_hdr Default PMAC header configuration for the Tx DMA channel
		Useful if Src Port does not send PMAC header with packet
\note
*/
typedef struct ingress_pmac {
	uint32_t tx_dma_chan:8;	/*!< Tx DMA channel Number for which PMAC
				   configuration is to be done
				 */
	uint32_t err_disc:1;	/*!< Is Discard Error Enable */
	uint32_t pmac:1;	/*!< Is Ingress PMAC Hdr Present */
	uint32_t def_pmac:1;	/*!< Is Ingress PMAC Hdr Present */
	uint32_t def_pmac_pmap:1;	/*!< Is Default PMAC Header configured
					   for Tx DMA Channel
					 */
	uint32_t def_pmac_en_pmap:1;	/*!< Is PortMap Enable to be used from
				   Default PMAC hrd (else use Ingress PMAC hdr)
				 */
	uint32_t def_pmac_tc:1;	/*!< Is TC (class) to be used from Default PMAC
				   hdr (else use Ingress PMAC hdr)
				 */
	uint32_t def_pmac_en_tc:1;	/*!< Are TC bits to be used for TC from
				   Default PMAC hdr (else use Ingress PMAC hdr)
				   Alternately, EN/DE/MPE1/MPE2 bits can
				   be used for TC
				 */
	uint32_t def_pmac_subifid:1; /*!< Is Sub-interfaceId to be taken from
				   Default PMAC hdr (else use Ingress PMAC hdr)
				 */
	uint32_t def_pmac_src_port:1;	/*!< Packet Source Port determined from
				   Default PMAC hdr (else use Ingress PMAC hdr)
				 */
	uint32_t res_ing:15;	/*!< Reserved bits */
	uint8_t def_pmac_hdr[PMAC_LEN];	/*!< Default PMAC header configuration
				   for the Tx DMA channel. Useful if Src Port
				   does not send PMAC header with packet
					 */
} ingress_pmac_t;

/*!
\brief Egress PMAC port configuration from Datapath Library
\param rx_dma_chan  Rx DMA channel Number for which PMAC configuration
		is to be done
\param rm_l2hdr	If Layer 2 Header is to be removed before Egress
		(for eg. for IP interfaces like LTE)
\param num_l2hdr_bytes_rm If rm_l2hdr=1,then number of L2 hdr bytes to be
		removed
\param fcs	If FCS is enabled on the port
\param pmac If PMAC header is enabled on the port
\param dst_port  Destination Port Identifier
\param res_eg  Reserved bits
\note
*/
typedef struct egress_pmac {
	uint32_t rx_dma_chan:8;	/*!< Rx DMA channel Number for which PMAC
				   configuration is to be done
				 */
	uint32_t rm_l2hdr:1;	/*!< If Layer 2 Header is to be removed
			   before Egress (for eg. for IP interfaces like LTE)
			 */

	uint32_t num_l2hdr_bytes_rm:8;	/*!< If rm_l2hdr=1,
				   then number of L2 hdr bytes to be removed
					 */
	uint32_t fcs:1;		/*!< If FCS is enabled on the port */
	uint32_t pmac:1;	/*!< If PMAC header is enabled on the port */
	uint32_t dst_port:8;	/*!< Destination Port Identifier */
	uint32_t res_dw1:4;	/*!< reserved field in DMA descriptor - DW1*/
	uint32_t res1_dw0:3;	/*!< reserved field in DMA descriptor - DW0*/
	uint32_t res2_dw0:2;	/*!< reserved field in DMA descriptor - DW0*/
	uint32_t tc_enable:1;	/*!<Selector for traffic class bits */
	uint32_t traffic_class:8;/*!< If tc_enable=true,sets egress queue
					traffic class
					*/
	uint32_t flow_id:8; /*!< flow id msb*/
	uint32_t dec_flag:1; /*!< If tc_enable=false,sets decryption flag*/
	uint32_t enc_flag:1; /*!< If tc_enable=false,sets encryption flag*/
	uint32_t mpe1_flag:1; /*!< If tc_enable=false,mpe1 marked flag valid*/
	uint32_t mpe2_flag:1; /*!< If tc_enable=false,mpe1 marked flag valid*/
	uint32_t res_eg:5; /*!< Reserved bits */
} egress_pmac_t;

typedef struct dp_subif_stats_t {
	u64 rx_bytes;
	u64 rx_pkts;
	u64 rx_disc_pkts;
	u64 rx_err_pkts;
	u64 tx_bytes;
	u64 tx_pkts;
	u64 tx_disc_pkts;
	u64 tx_err_pkts;
} dp_subif_stats_t;

enum EG_PMAC_F {
	/*1 bit one flag */
	EG_PMAC_F_L2HDR_RM = 0x1, /*!< eg_pmac.numBytesRem/bRemL2Hdr valid*/
	EG_PMAC_F_FCS = 0x2, /*!< mean eg_pmac.bFcsEna valid*/
	EG_PMAC_F_PMAC = 0x4, /*!< mean eg_pmac.bPmacEna valid */
	EG_PMAC_F_RXID = 0x8, /*!< mean eg_pmac.nRxDmaChanId valid */
	EG_PMAC_F_RESDW1 = 0x10, /*!< mean eg_pmac.nResDW1 valid */
	EG_PMAC_F_RES1DW0 = 0x20, /*!< mean eg_pmac.nRes1DW0 valid */
	EG_PMAC_F_RES2DW0 = 0x40, /*!< mean eg_pmac.nRes2DW0 valid */
	EG_PMAC_F_TCENA = 0x80, /*!< mean eg_pmac.bTCEnable valid */
	EG_PMAC_F_DECFLG = 0x100, /*!< mean eg_pmac.bDecFlag valid */
	EG_PMAC_F_ENCFLG = 0x200, /*!< mean eg_pmac.bEncFlag valid */
	EG_PMAC_F_MPE1FLG = 0x400, /*!< mean eg_pmac.bMpe1Flag valid */
	EG_PMAC_F_MPE2FLG = 0x800, /*!< mean eg_pmac.bMpe2Flag valid */
};

/*! EG_PMAC Flags */
enum IG_PMAC_F {
	/*1 bit one flag */
	IG_PMAC_F_ERR_DISC = 0x1, /*!< mean ig_pmac.bErrPktsDisc valid */
	IG_PMAC_F_PRESENT = 0x2,  /*!< mean ig_pmac.bPmacPresent valid */
	IG_PMAC_F_SUBIF = 0x4,  /*!< mean ig_pmac.bSubIdDefault valid */
	IG_PMAC_F_SPID = 0x8,  /*!< mean ig_pmac.bSpIdDefault valid */
	IG_PMAC_F_CLASSENA = 0x10,  /*!< mean ig_pmac.bClassEna valid */
	IG_PMAC_F_CLASS = 0x20,  /*!< mean ig_pmac.bClassDefault valid */
	IG_PMAC_F_PMAPENA = 0x40,  /*!< mean ig_pmac.bPmapEna valid */
	IG_PMAC_F_PMAP = 0x80,  /*!< mean ig_pmac.bPmapDefault valid */
	IG_PMAC_F_PMACHDR1 = 0x100,  /*!< mean ig_pmac.defPmacHdr[1] valid */
	IG_PMAC_F_PMACHDR2 = 0x200,  /*!< mean ig_pmac.defPmacHdr[2] valid */
	IG_PMAC_F_PMACHDR3 = 0x400,  /*!< mean ig_pmac.defPmacHdr[3] valid */
	IG_PMAC_F_PMACHDR4 = 0x800,  /*!< mean ig_pmac.defPmacHdr[4] valid */
	IG_PMAC_F_PMACHDR5 = 0x1000,  /*!< mean ig_pmac.defPmacHdr[5] valid */
	IG_PMAC_F_PMACHDR6 = 0x2000,  /*!< mean ig_pmac.defPmacHdr[6] valid */
	IG_PMAC_F_PMACHDR7 = 0x4000,  /*!< mean ig_pmac.defPmacHdr[7] valid */
	IG_PMAC_F_PMACHDR8 = 0x8000,  /*!< mean ig_pmac.defPmacHdr[8] valid */
};

enum PASER_FLAG {
	F_MPE_NONE = 0x1, /*Need set MPE1=0 and MPE2=0*/
	F_MPE1_ONLY = 0x2, /*Need set MPE1=1 and MPE2=0 */
	F_MPE2_ONLY = 0x4, /*Need set MPE1=0 and MPE2=1 */
	F_MPE1_MPE2 = 0x8, /*Need set MPE1=1 and MPE2=1 */
};
enum PASER_VALUE {
	DP_PARSER_F_DISABLE = 0,  /*Without Paser Header and Offset */
	DP_PARSER_F_HDR_ENABLE = 1,/*With Paser Header, but without Offset */
	DP_PARSER_F_HDR_OFFSETS_ENABLE = 2,  /*with Paser Header and Offset */
};
/*!
\brief Datapath Library Port PMAC configuration structure
\param ig_pmac  Ingress PMAC configuration
\param eg_pmac  Egress PMAC configuration
\note GSW_PMAC_Ig_Cfg_t/GSW_PMAC_Eg_Cfg_t defined in GSWIP driver:
	<xway/switch-api/lantiq_gsw_api.h>
*/
typedef struct dp_pmac_cfg {
	uint32_t ig_pmac_flags;	/*!< one bit for one ingress_pmac_t fields */
	uint32_t eg_pmac_flags;	/*!< one bit for one egress_pmac_t fields */
	ingress_pmac_t ig_pmac;	/*!< Ingress PMAC configuration */
	egress_pmac_t eg_pmac;	/*!< Egress PMAC configuration */
} dp_pmac_cfg_t;
/* @} */

/** \addtogroup Datapath_Driver_API */
/* @{ */
/*! \brief  Datapath Allocate Datapath Port aka PMAC port
	port may map to an exclusive netdevice like in the case of
	ethernet LAN ports. In other cases like WLAN, the physical port is a
	Radio port, while netdevices are Virtual Access Points (VAPs)
	In this case, the  AP netdevice can be passed
Alternately, driver_port & driver_id will be used to identify this port
\param[in] owner  Kernel module pointer which owns the port
\param[in] dev pointer to Linux netdevice structure (optional), can be NULL
\param[in] dev_port Physical Port Number of this device managed by the driver
\param[in] port_id Optional port_id number requested. Usually, 0 and
	allocated by driver
\param[in] pmac_cfg PMAC related configuration parameters
\param[in] flags :Various special Port flags like WAVE500, VRX318 etc ...
	-  DP_F_DEALLOC_PORT :Deallocate the already allocated port
\return  Returns PMAC Port number, -1 on ERROR
*/
int32_t dp_alloc_port(struct module *owner, struct net_device *dev,
		      uint32_t dev_port, int32_t port_id,
		      dp_pmac_cfg_t *pmac_cfg, uint32_t flags);

/*! \brief  Higher layer Driver Datapath registration API
\param[in] owner  Kernel module pointer which owns the port
\param[in] port_id Port Id returned by alloc() function
\param[in] dp_cb  Datapath driver callback structure
\param[in] flags :Special input flags to alloc routine
		- F_DEREGISTER :Deregister the device
\return 0 - OK / -1 - Correct Return Value
\note
*/

int32_t dp_register_dev(struct module *owner, uint32_t port_id,
			dp_cb_t *dp_cb, uint32_t flags);

/*! \brief  Allocates datapath subif number to a sub-interface netdevice
* Sub-interface value must be passed to the driver
port may map to an exclusive netdevice like in the case of ethernet LAN ports
\param[in] owner  Kernel module pointer which owns the port
\param[in] dev pointer to Linux netdevice structure, only for VRX318 driver,\n
it can be NULL. All other driver's, must provide valid dev pointer.
\param[in] port_id Optional port_id number requested. Usually, 0 and
		allocated by driver
\param[in,out] subif_id pointer to subif_id structure including port_id
\param[in] flags :
	DP_F_DEREGISTER - De-register already registered subif/vap
\return Port Id  / IFX_FAILURE
\note
*/
int32_t dp_register_subif(struct module *owner, struct net_device *dev,
			  char *subif_name, dp_subif_t *subif_id,
			  uint32_t flags);

/*! \brief  Transmit packet to low-level Datapath driver
\param[in] rx_if  Rx If netdevice pointer - optional
\param[in] buf   pointer to packet buffer like sk_buff
\param[in] len    Length of packet to transmit
\param[in] flags :Reserved
\return 0 if OK  / -1 if error
\note
*/
int32_t dp_xmit(struct net_device *rx_if, dp_subif_t *rx_subif,
		struct sk_buff *skb, int32_t len, uint32_t flags);

/*! \brief  Check if network interface like WLAN is a fastpath interface
Sub-interface value must be passed to the driver
	port may map to an exclusive netdevice like in the case of
	ethernet LAN ports.
\param[in] netif  pointer to stack network interface structure
\param[in] ifname  Interface Name
\param[in] flags :Reserved
\return 1 if WLAN fastpath interface like WAVE500 / 0 otherwise
\note  Prototype of PPA_DP_CHECK_IF_NETIF_FASTPATH part of the callback
		structure. Such a function needs to be defined by client driver
		like the WAVE500 WLAN driver. This API is used by the PPA Stack
		AL to check during acceleration learning and configuration
*/

int32_t dp_check_if_netif_fastpath_fn(struct net_device *netif,
				      dp_subif_t *subif, char *ifname,
				      uint32_t flags);

/*! \brief  Get Pkt dst-if Sub-if value
	Sub-interface value must be passed to the driver
	port may map to an exclusive netdevice like in the case of ethernet
	LAN ports.
\param[in] port_id Datapath/PMAC Port Id on which to search for subifid
	(optional)
\param[in] netif  pointer to stack network interface structure through
		which packet to be Tx
\param[in] skb pointer to sk_buff structure that carries packet destination
		info
\param[in] dst_mac  Destiantion MAC address to which packet is addressed
\param[in] flags :Reserved
\return 0 if subifid found; -1 otherwise
\note  Prototype of PPA_DP_GET_NETIF_SUBIF function. Not implemented in PPA
	Datapath, but in client driver like WAVE500 WLAN driver
\note  Either skbuff parameters to be used  or dst_mac to determine subifid
	For WAVE500 driver, this will be the StationId + VAP on the basis of
	the dst mac. This function is only to be used by the PPA to program
	acceleration entries. The client driver is still expected to fill
	in Sub-interface id when transmitting to the underlying datapath driver
*/
int32_t dp_get_netif_subifid(struct net_device *netif, struct sk_buff *skb,
			     void *subif_data, uint8_t dst_mac[MAX_ETH_ALEN],
			     dp_subif_t *subif, uint32_t flags);
/*!
\brief  The API is for CBM to send received packets(skb) to dp lib. Datapath lib
	will do basic packet parsing and forwards it to related drivers,\n
	like ethernet driver, wifi and lte and so on. Noted.
	It is a chained skb and dp lib will split it before send it to
	related drivers
\param[in] skb  pointer to packet buffer like sk_buffer
\param[in] flag  reserved for futures
\return 0 if OK / -1 if error
*/
int32_t dp_rx(struct sk_buff *skb, uint32_t flags);
/*!
\brief  The API is for configuing PMAC based on deque port
\param[in] port  Egress Port
\param[in] pmac_cfg Structure of ingress/egress parameters for setting PMAC
	   configuration
\return 0 if OK / -1 if error
*/
int32_t dp_pmac_set(uint32_t port, dp_pmac_cfg_t *pmac_cfg);

struct pmac_port_info *get_port_info(int index);
struct pmac_port_info *get_port_info_via_dp_port(int dp_port);

void set_dp_dbg_flag(uint32_t flags);
uint32_t get_dp_dbg_flag(void);
struct module *dp_get_module_owner(int ep);
void dp_dump_raw_data(char *buf, int len, char *prefix_str);

#ifdef CONFIG_LTQ_TOE_DRIVER
#undef LTQ_TSO_SW_WORKAROUND
extern int ltq_tso_xmit(struct sk_buff *skb, int egress_port, int flags);
#endif

enum DP_F_STATS_ENUM {
	DP_F_STATS_SUBIF = 1 << 0,
	DP_F_STATS_PAE_CPU = 1 << 1
};
extern void dp_set_gsw_parser(uint8_t flag, uint8_t cpu, uint8_t mpe1,
		uint8_t mpe2, uint8_t mpe3);
/*backcompatible since PPA is already calling it */
#define get_gsw_parser  dp_get_gsw_parser
extern void dp_get_gsw_parser(uint8_t *cpu, uint8_t *mpe1, uint8_t *mpe2,
	uint8_t *mpe3);
extern void dp_parser_info_refresh(u32 v, u32 verify);
extern int dp_reset_itf_mib(u32 flag);
extern int dp_get_netif_stats(struct net_device *dev, dp_subif_t *subif_id,
		struct rtnl_link_stats64 *path_stats, uint32_t flags);
int32_t dp_clear_netif_stats(struct net_device *dev, dp_subif_t *subif_id,
		uint32_t flag);
extern int dp_get_port_subitf_via_ifname(char *ifname, dp_subif_t *subif);
extern int dp_get_port_subitf_via_dev(struct net_device *dev,
		dp_subif_t *subif);
#ifdef CONFIG_LTQ_DATAPATH_CPUFREQ
int dp_coc_new_stat_req(enum ltq_cpufreq_state new_state, uint32_t flag);
/*DP's submodule to call it */
extern int dp_set_rmon_threshold(struct ltq_cpufreq_threshold *threshold,
	uint32_t flags);
#endif /* CONFIG_LTQ_DATAPATH_CPUFREQ*/

enum ltq_cpufreq_state;
int dp_set_meter_rate(enum ltq_cpufreq_state stat, unsigned int rate);
char *dp_skb_csum_str(struct sk_buff *skb);


/* @} */
#endif				/*DATAPATH_API_H */
