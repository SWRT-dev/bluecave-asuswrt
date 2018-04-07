#ifndef _UGW_WAVE500_DP_API_H_
#define _UGW_WAVE500_DP_API_H_

#include <net/directconnect_dp_api.h>

/** \file ltq_wave500_dp_api.h  This file defines all the APIs and associated data structures for WAVE500 peripheral. */

/** \defgroup WAVE500_Datapath_Driver_Defines WAVE500 Defines
  \brief This section groups all the constant defines used in WAVE500 data structures.
*/

/** \defgroup WAVE500_Datapath_Driver_Enums WAVE500 Enums
  \brief This section groups all the enumeration definitons used in WAVE500 data structures.
*/

/** \defgroup WAVE500_Datapath_Driver_Unions WAVE500 Unions
  \brief This section groups all the Union type data structures definitons used in WAVE500.
*/

/** \defgroup WAVE500_Datapath_Driver_Structs WAVE500 Structures
  \brief This section groups all the Struct type data structures definitons used in WAVE500.
*/

/** \defgroup WAVE500_Datapath_Driver_APIs WAVE500 APIs
  \brief This section groups all the APIs definitons used in WAVE500.
*/

/** \defgroup WAVE500_Datapath_Driver_Power_Management_Wrapper_API  WAVE500 Power Management APIs
  \brief This section provides all the Power Management Wrapper API
*/

/** \addtogroup WAVE500_Datapath_Driver_Defines */
/* @{ */

/**
  \brief WAVE500 DMA1-TX DMA default data buffer size
*/
#define WAVE500_DATA_BUF_MAX_SIZE	DC_DP_DMA_BUF_SIZE_DEFAULT

/**
  \brief WAVE500 Multicast (MC) module register request
*/
#define WAVE500_DP_F_MC_REGISTER	DC_DP_F_MC_REGISTER

/**
  \brief WAVE500 Multicast module de-register request
*/
#define WAVE500_DP_F_MC_DEREGISTER	DC_DP_F_MC_DEREGISTER

/**
  \brief WAVE500 A new multicast group membership add request
*/
#define WAVE500_MC_F_ADD 			DC_DP_MC_F_ADD

/**
  \brief WAVE500 An existing multicast group membership delete request
*/
#define WAVE500_MC_F_DEL			DC_DP_MC_F_DEL

/**
  \brief WLAN Number of WiFi WMM Class/TID
*/
#define WAVE500_DP_MAX_WIFI_CLASS	DC_DP_MAX_WIFI_CLASS

/**
  \brief Power Saving (Cpufreq) callback register request
*/
#define WAVE500_DP_F_PS_REGISTER	DC_DP_F_PS_REGISTER

/**
  \brief Power Saving (PS) callback de-register request
*/
#define WAVE500_DP_F_PS_DEREGISTER	DC_DP_F_PS_DEREGISTER	

/**
  \brief Power Saving (PS) notifier list type - Transition Notifier
*/
#define WAVE500_DP_PS_TRANSITION_NOTIFIER	DC_DP_PS_TRANSITION_NOTIFIER
/**
  \brief Power Saving (PS) notifier list type - Policy Notifier
*/
#define WAVE500_DP_PS_POLICY_NOTIFIER	DC_DP_PS_POLICY_NOTIFIER

/** \brief PS (CPUFreq) Operation Success */
#define WAVE500_DP_PS_SUCCESS	DC_DP_PS_SUCCESS
/** \brief PS (CPUFreq) Operation Denied */
#define WAVE500_DP_PS_DENIED		DC_DP_PS_DENIED
/** \brief Called function just return without doing anything; used only in callback functions */
#define WAVE500_DP_PS_NOACTIVITY	DC_DP_PS_NOACTIVITY
/** \brief It is used if callback function is not defined */
#define WAVE500_DP_PS_NOTDEFINED	DC_DP_PS_NOTDEFINED	

/**
  \brief PS (Cpufreq) notifier list Add Operation
*/
#define WAVE500_DP_PS_LIST_ADD		DC_DP_PS_LIST_ADD
/**
  \brief PS (Cpufreq) notifier list Delete Operation
*/
#define WAVE500_DP_PS_LIST_DEL		DC_DP_PS_LIST_DEL	

/* @} */


/** \addtogroup WAVE500_Datapath_Driver_Enums */
/* @{ */

/**
  \brief WAVE500 IP Type structure
*/
typedef dc_dp_iptype_t wave500_dp_iptype_t;

/**
  \brief WAVE500 Classification Location - LAN2LAN (GSWIP-L) or All traffic (PAE)
*/
typedef dc_dp_class_devingress_t wave500_dp_class_devingress_t;

/**
  \brief WAVE500 Classification Rule Category
*/
typedef dc_dp_class_category_t wave500_dp_class_category_t;

/**
  \brief WAVE500 Classification Rule Sub-Category
*/
typedef dc_dp_class_sub_category_t wave500_dp_class_sub_category_t;

/**
  \brief WAVE500 Classification Learning Action Configuration
*/
typedef dc_dp_class_action_learning_t wave500_dp_class_action_learning_t;

/**
  \brief WAVE500 Classification Metering Action Configuration
*/
typedef dc_dp_class_action_meter_t wave500_dp_class_action_meter_t;

/**
  \brief WAVE500 Classification Action Traffic Class Configuration
*/
typedef dc_dp_class_action_trafficclass_t wave500_dp_class_action_trafficclass_t;

/**
  \brief WAVE500 Classification Interrupt action Configuration
*/
typedef dc_dp_class_action_irq_t wave500_dp_class_action_irq_t;

/**
  \brief WAVE500 Classification Cross State Action Configuration
*/
typedef dc_dp_class_action_crossstate_t wave500_dp_class_action_crossstate_t;

/**
  \brief WAVE500 Set subifid = subinterface[12:8]  routine
*/
typedef dc_dp_class_action_criticalframe_t wave500_dp_class_action_criticalframe_t;

/**
  \brief WAVE500 Classification Timestamp Action Configuration
*/
typedef dc_dp_class_action_timestamp_t wave500_dp_class_action_timestamp_t;

/**
  \brief WAVE500 Classification PortMap Action Configuration
*/
typedef dc_dp_class_action_portmap_t wave500_dp_class_action_portmap_t;

/**
  \brief WAVE500 Classification VLAN Action Configuration
*/
typedef dc_dp_class_action_vlan_t wave500_dp_class_action_vlan_t;

/**
  \brief WAVE500 Classification Port Filter Action Type
*/
typedef dc_dp_class_action_portfilter_t wave500_dp_class_action_portfilter_t;

/**
  \brief WAVE500 Classification Action Interface Type
*/
typedef dc_dp_class_action_iftype_t wave500_dp_class_action_iftype_t;

/**
  \brief WAVE500 Processing Path Selectors
*/
typedef dc_dp_class_action_processingpath_t wave500_dp_class_action_processingpath_t;

/**
  \brief WAVE500 Cross VLAN action
*/
typedef dc_dp_class_action_crossvlan_t wave500_dp_class_action_crossvlan_t;

/**
  \brief WAVE500 Rule selection for IPv4/IPv6
*/
typedef dc_dp_pce_ip_t wave500_dp_pce_ip_t;

/**
  \brief WAVE500 Power module identifier
*/
typedef dc_dp_power_module_t wave500_dp_power_module_t;

/**
  \brief WAVE500 Power State structure
*/
typedef dc_dp_power_state_t wave500_dp_power_state_t;

/* @} */

/** \addtogroup WAVE500_Datapath_Driver_Unions */
/* @{ */

/** \brief WAVE500 IPv4 and IPv6 address type in numeric representation */
typedef dc_dp_ip_t wave500_dp_ip_t;

/* @} */


/** \addtogroup WAVE500_Datapath_Driver_Structs */
/* @{ */

/**
  \brief WAVE500 Rx function callback
*/
typedef dc_dp_rx_fn_t wave500_dp_rx_fn_t;

/*!
   \brief   Get Station Id callback
   \param[in] port_id  Port Id of port through which dst MAC address mac_addr can be reached [optional]
   \param[in] netif  Network interface through which packet to dst MAC address mac_addr will be transmitted
   \param[in] mac_addr  MAC Address of Station
   \param[out] sta_id  Station Id returned
   \return 0 if OK, -1 on ERROR
*/
typedef int32_t (*wave500_get_staid_fn_t)(int32_t port_id, struct net_device *netif, char *mac_addr, uint32_t *sta_id);

/**
  \brief WAVE500 Get Ring/Buffer recovery stats callback
*/
typedef dc_dp_get_recovery_stats_fn_t wave500_get_recovery_stats_fn_t;

#if 0
/**
   \brief WAVE500 Datapath Driver Registration Callback.
   \param rx_fn  Rx function callback
   \param stop_fn  Stop Tx function callback for flow control
   \param restart_fn  Start Tx function callback for flow control
   \param get_staid_fn  Get Station Id callback for MAC address
   \note
*/
typedef struct wave500_dp_cb {
	wave500_dp_rx_fn_t rx_fn;   /*!< Rx function callback */
	dp_stop_tx_fn_t stop_fn;  /*!< Stop Tx function callback for Tx flow control */
	dp_restart_tx_fn_t restart_fn;  /*!< Start Tx function callback for Tx flow control */
	wave500_get_staid_fn_t get_staid_fn;  /*!< Get Station Id callback for MAC address */
	wave500_get_recovery_stats_fn_t recovery_fn; /*!< Get Recovery related stats */
} wave500_dp_cb_t;
#else /* #if 0 */
typedef dc_dp_cb_t wave500_dp_cb_t;
#endif /* #else */

#ifdef SWAS_3_8
/**
  \brief WAVE500 Buffer Pool Data structure
*/
typedef dc_dp_buf_pool_t wave500_dp_buf_pool_t;
#else /* #ifdef SWAS_3_8 */
/**
   \brief Buffer list node structure.
   \param list  Pointer to next buffer list node
   \param pktbuf  Pointer to buffer
   \note
*/
typedef struct bufptr {
	struct list_head list; /*!< Pointer to next buffer list node */
	uint8_t *pktbuf; /*!< Pointer to buffer */
} bufptr_t;
#endif /* #else */

/**
  \brief WAVE500 Datapath Resources structure
*/
typedef struct wave500_dp_res {
#ifdef SWAS_3_8
	int32_t num_bufpools; /*!< Number of buffer pools/chunks allocated for the desired no of buffers */
	dc_dp_buf_pool_t *buflist; /*!< Allocated list of buffer chunks from which packet buffers are carved out.
									Caller needs to free the memory given by buflist pointer. */
#else /* #ifdef SWAS_3_8 */
	struct list_head *buflist; /*!< Allocate list of buffers */
#endif /* #else */
	uint32_t num_dma_desc; /*!< Number of DMA descriptors to be programmed to Tx DMA channel of PAE */
	void *dmatx_ring_base; /*!< Physical Base address of the DMA ring */
	void *cbm_dequeue_base; /*!< CBM Dequeue Port Base address for DirectConnect Dequeue Port */
	uint32_t num_dequeue_bufs; /*!< Number of Dequeue descriptors in ring. 32 in DirectConnect Dequeue port, even more for FW based
								    additional DirectConnect support */
	void *cbm_buf_free_base; /*!< CBM Buffer Free port Base address for DirectConnect Buffer Free Port */
	uint32_t num_free_bufs; /*!< Number of free buffer descriptors in ring. 32 in DirectConnect Buffer Free port, even more for FW
								 based additional DirectConnect support. */
} wave500_dp_res_t;

/**
  \brief WAVE500 multicast module callback
*/
typedef dc_dp_mcast_callback_fn_t wave500_dp_mcast_callback_fn_t;

/**
  \brief WAVE500 IP address data structure
*/
typedef dc_dp_ip_addr_t ip_addr_t;

/**
  \brief WAVE500 Multicast stream structure
*/
typedef dc_dp_mcast_stream_t mcast_stream_t;

/**
  \brief WAVE500 Classifier Pattern structure
*/
typedef dc_dp_class_pattern_t wave500_dp_class_pattern_t;

/**
  \brief WAVE500 Classification Action Structure
*/
typedef dc_dp_class_filter_action_t wave500_dp_class_filter_action_t;

/**
  \brief WAVE500 Classifier VLAN action structure
*/
typedef dc_dp_class_vlan_action_t wave500_dp_class_vlan_action_t;

/**
  \brief WAVE500 Classifier Action Forwarding struct
*/
typedef dc_dp_class_action_fwd_t wave500_dp_class_action_fwd_t;

/**
  \brief WAVE500 Classifier action QoS struct
*/
typedef dc_dp_class_action_qos_t wave500_dp_class_action_qos_t;

/**
  \brief WAVE500 Classification action IRQ/Timestamp struct for mgmt
*/
typedef dc_dp_class_action_mgmt_t wave500_dp_class_action_mgmt_t;

/**
  \brief WAVE500 Classification Overall Action struct
*/
typedef dc_dp_class_action_t wave500_dp_class_action_t;

/**
  \brief WAVE500 Classification Rule Overall Configuration structure
*/
typedef dc_dp_class_rule_t wave500_dp_class_rule_t;

/**
  \brief WAVE500 Power Saving Thresholds values for different power states
*/
typedef dc_dp_ps_threshold_t wave500_dc_dp_ps_threshold_t;

/**
  \brief WAVE500 Power State Module Info for registration
*/
typedef dc_dp_ps_module_info_t wave500_dc_dp_ps_module_info_t;

/**
  \brief WAVE500 Wake-on-LAN Structure
*/
typedef dc_dp_wol_cfg_t wave500_dp_wol_cfg_t;

/**
  \brief WAVE500 Ring Recovery stats Structure
*/
typedef dc_dp_recovery_stats_t wave500_dp_recovery_stats_t;

/* @} */


/** \addtogroup WAVE500_Datapath_Driver_APIs */
/* @{ */

/**
  \brief WAVE500 DP Alloc port routine
*/
#define wave500_dp_alloc_port dc_dp_alloc_port

#ifdef SWAS_3_8
/**
  \brief WAVE500 DP Register dev routine
*/
#define wave500_dp_register_dev dc_dp_register_dev
#else /* #ifdef SWAS_3_8 */

/*! \brief  Higher layer Driver Datapath registration API
   \param[in] owner  Kernel module pointer which owns the port
   \param[in] port_id  Datapath Port Id (PMAC Port No) on which to register
   \param[in] dev  Pointer to Linux netdevice structure
   \param[in] datapathcb  Callback registration structure
   \param[in] num_bufs_req  Number of Tx buffers to allocate (WLAN -> GRX500 SoC PAE direction)
   \param[in] umt_base  Base address of UMT write location (in WAVE500 HW memory)
   \param[in] umt_len  Number of bytes of UMT message to write to umt_base
   \param[out] resources  Buffer, DMA ring and CBM port resource addresses
   \param[in] flags  : Special input flags to alloc routine
   		- DP_F_DEREGISTER : Deregister the device
   \return 0 if OK / -1 if error
   \note
*/
int32_t
wave500_dp_register_dev (
	struct module *owner,
	uint32_t port_id,
	struct net_device *dev,
	wave500_dp_cb_t *datapathcb,
	uint32_t num_bufs_req,
	void *umt_base,
	int32_t umt_len,
	wave500_dp_res_t *resources,
	uint32_t flags
);
#endif /* #else */

/**
  \brief WAVE500 DP Register subif routine
*/
#define wave500_dp_register_subif dc_dp_register_subif

/**
  \brief WAVE500 DP xmit routine calling dc_dp_xmit
*/
#define wave500_dp_xmit dc_dp_xmit

/**
  \brief WAVE500 Set Egress Port  in packet
*/
#define wave500_dp_set_ep_pkt dc_dp_set_ep_pkt

/**
  \brief WAVE500 Get Egress port in packet
*/
#define wave500_dp_get_ep_pkt dc_dp_get_ep_pkt

/**
  \brief WAVE500 Set subifid = subinterface[12:8]  routine
*/
#define wave500_dp_set_subifid_pkt dc_dp_set_subifid_pkt

/**
  \brief WAVE500 Get subifid = subinterface[12:8]  routine
*/
#define wave500_dp_get_subifid_pkt dc_dp_get_subifid_pkt

/** \brief  Disconnect a particular MAC addr - remove all MAC table entries
    and/or routing sessions which use the specified MAC address.
   \param[in] sta_id  Station ID to remove
   \param[out] macaddr  MAC address to remove
   \return 0 if OK / -1 if error
   \note  Calls dc_dp_disconn_if() internally.
*/
/* static inline */int32_t
wave500_dp_disconn_staid (
	uint8_t sta_id,
	uint8_t macaddr[MAX_ETH_ALEN]
);

/**
  \brief WAVE500 Regiser Mcast Module
*/
#define wave500_dp_register_mcast_module dc_dp_register_mcast_module

/**
  \brief  Provide a Priority (802.1D Priority) to WMM Class/TID map for the
  given WiFi Radio/net_device
*/
#define wave500_dp_map_prio_wmm_class dc_dp_map_prio_wmm_class

/**
  \brief  Provide WMM AC/TID mapping for a given packet
*/
#define wave500_dp_mark_pkt_wmm dc_dp_mark_pkt_wmm

/**
  \brief WAVE500 Get network interface stats
*/
#define wave500_dp_get_netif_stats dc_dp_get_netif_stats

/**
  \brief WAVE500 DP Clear Netif Stats
*/
#define wave500_dp_clear_netif_stats dc_dp_clear_netif_stats

/**
  \brief WAVE500 Add Classifier Rule
*/
#define wave500_dp_add_class_rule dc_dp_add_class_rule

/**
  \brief WAVE500 Modify Classifier Rule
*/
#define wave500_dp_mod_class_rule dc_dp_mod_class_rule

/**
  \brief WAVE500 Delete Classifier Rule
*/
#define wave500_dp_del_class_rule dc_dp_del_class_rule

/**
  \brief WAVE500 Get Classifier Rule
*/
#define wave500_dp_get_class_rule dc_dp_get_class_rule

/**
  \brief WAVE500 Get Wake on LAN config
*/
#define wave500_dp_get_wol_cfg dc_dp_get_wol_cfg

/**
  \brief WAVE500 Set Wake on LAN configuration
*/
#define wave500_dp_set_wol_cfg dc_dp_set_wol_cfg

/**
  \brief WAVE500 Set Wake on LAN enable/disable
*/
#define wave500_dp_set_wol_ctrl dc_dp_set_wol_ctrl

/**
  \brief WAVE500 Get Wake on LAN enable/disable
*/
#define wave500_dp_get_wol_ctrl_status dc_dp_get_wol_ctrl_status

/* @} */

/** \addtogroup WAVE500_Datapath_Driver_Power_Management_Wrapper_API */
/* @{ */

/**
  \brief WAVE500 Register Power Mgmt Notifier (with cpufreq driver)
*/
#define wave500_dp_register_power_notifier dc_dp_register_power_notifier

/**
  \brief WAVE500 Request new power state
*/
#define wave500_dp_req_power_state dc_dp_req_power_state

/**
  \brief WAVE500 Get Power Mgmt polling period = subinterface[12:8]  routine
*/
#define wave500_dp_get_ps_poll_period dc_dp_get_ps_poll_period

/**
  \brief WAVE500 Get Power State thresholds
*/
#define wave500_dp_get_ps_threshold dc_dp_get_ps_threshold

/**
  \brief WAVE500 Add/Delete module from Power State list
*/
#define wave500_dp_mod_ps_list dc_dp_mod_ps_list

/**
  \brief WAVE500 Convert CPU frequency to Power state
*/
#define wave500_dp_get_ps_from_khz dc_dp_get_ps_from_khz
/* @} */

#endif /* _UGW_WAVE500_DP_API_H_ */
