#ifndef _UGW_DIRECTCONNECT_DP_API_LEGACY_H_
#define _UGW_DIRECTCONNECT_DP_API_LEGACY_H_

#include <net/datapath_api.h>

/** \file directconnect_dp_api.h  This file defines all the APIs and associated data structures for DirectConnect (DC) compliant peripherals. */

/** \defgroup DirectConnect_Datapath_Driver_Defines Direct Connect Defines
  \brief This section groups all the constant defines used in DirectConnect data structures.
*/

/** \defgroup DirectConnect_Datapath_Driver_Enums Direct Connect Enums
  \brief This section groups all the enumeration definitons used in DirectConnect data structures.
*/

/** \defgroup DirectConnect_Datapath_Driver_Unions Direct Connect Unions
  \brief This section groups all the Union type data structures definitons used in DirectConnect.
*/

/** \defgroup DirectConnect_Datapath_Driver_Structs Direct Connect Structures
  \brief This section groups all the Struct type data structures definitons used in DirectConnect.
*/

/** \defgroup DirectConnect_Datapath_Driver_APIs Direct Connect APIs
  \brief This section groups all the APIs definitons used in DirectConnect.
*/

/** \defgroup DirectConnect_Datapath_Driver_Power_Management_Wrapper_APIs  Direct Connect Power Management APIs
  \brief This section provides all the Power Management Wrapper API
*/

/** \addtogroup DirectConnect_Datapath_Driver_Defines */
/* @{ */

/**
  \brief DMA1-TX DMA default data buffer size
*/
#define DC_DP_DMA_BUF_SIZE_DEFAULT		2048

/**
  \brief Multicast (MC) module register request
*/
#define DC_DP_F_MC_REGISTER				0x01

/**
  \brief Multicast module de-register request
*/
#define DC_DP_F_MC_DEREGISTER			0x02

/**
  \brief A new multicast group membership add request
*/
#define DC_DP_MC_F_ADD					0x01

/**
  \brief An existing multicast group membership delete request
*/
#define DC_DP_MC_F_DEL					0x02

/**
  \brief WLAN Number of WiFi WMM Class/TID
*/
#define DC_DP_MAX_WIFI_CLASS			8

/**
  \brief Power Saving (Cpufreq) callback register request
*/
#define DC_DP_F_PS_REGISTER				0x01

/**
  \brief Power Saving (PS) callback de-register request
*/
#define DC_DP_F_PS_DEREGISTER			0x02

/**
  \brief Power Saving (PS) notifier list type - Transition Notifier
*/
#define DC_DP_PS_TRANSITION_NOTIFIER	(0)
/**
  \brief Power Saving (PS) notifier list type - Policy Notifier
*/
#define DC_DP_PS_POLICY_NOTIFIER		(1)

/** \brief PS (CPUFreq) Operation Success */
#define DC_DP_PS_SUCCESS				0
/** \brief PS (CPUFreq) Operation Denied */
#define DC_DP_PS_DENIED					1
/** \brief Called function just return without doing anything; used only in callback functions */
#define DC_DP_PS_NOACTIVITY				3
/** \brief It is used if callback function is not defined */
#define DC_DP_PS_NOTDEFINED				4

/**
  \brief PS (Cpufreq) notifier list Add Operation
*/
#define DC_DP_PS_LIST_ADD				1
/**
  \brief PS (Cpufreq) notifier list Delete Operation
*/
#define DC_DP_PS_LIST_DEL				0

/* @} */


/** \addtogroup DirectConnect_Datapath_Driver_Enums */
/* @{ */

/** \brief Definition of the IP Type used.
*/
typedef enum {
  /** IPv4 Type selector */
	IPV4 = 0,
  /** IPv6 Type selector */
	IPV6 = 1,
  /** None IP selector */
	INVALID,
} dc_dp_iptype_t;

/** \brief Classification Location - GSW-L (LAN2LAN) or GSW-R (All traffic) device.
    Used by \ref dc_dp_class_rule_t. */
typedef enum {
	DC_DP_GSWR_INGRESS=0, 	/*!< GSWIP-R ingress traffic*/
	DC_DP_GSWL_INGRESS=1	/*!< GSWIP-L ingress traffic*/
} dc_dp_class_devingress_t;

/** \brief Classification Rule Category.
    Used by \ref dc_dp_class_rule_t. */
typedef enum {
	DC_DP_CAT_NONE=0,	/*!< DP category none */
	DC_DP_CAT_FILTER,	/*!< DP category filter */
	DC_DP_CAT_VLAN,	/*!< DP category VLAN */
	DC_DP_CAT_FWD,		/*!< DP category forward */
	DC_DP_CAT_USQOS,	/*!< DP category US QOS */
	DC_DP_CAT_DSQOS,	/*!< DP category DS QOS */
	DC_DP_CAT_MGMT,	/*!< DP category Management */
	DC_DP_CAT_LRO,		/*!< DP category LRO */
	DC_DP_CAT_TUN,		/*!< DP category TUNNEL */
	DC_DP_CAT_MAX		/*!< DP category MAX*/
} dc_dp_class_category_t;

/** \brief Classification Rule Sub-Category.
    Used by \ref dc_dp_class_rule_t. */
typedef enum {
	DC_DP_SUBCAT_NONE=0,		/*!< DP sub category none */
	DC_DP_SUBCAT_WLAN_FILTER,	/*!< DP category wlan filter */
	DC_DP_SUBCAT_MAX			/*!< DP category max */
} dc_dp_class_sub_category_t;

/** \brief Classification Learning Action Configuration.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_LEARNING_DISABLE=0,		/*!< DP class action learning disable */
	DC_DP_LEARNING_REGULAR=1,		/*!< DP class action learning regular (enable) */
	DC_DP_LEARNING_FORCE_NOT = 2,	/*!< DP class action learning force not*/
	DC_DP_LEARNING_FORCE = 3		/*!< DP class action Forced Learning */
} dc_dp_class_action_learning_t;

/** \brief Classification Metering Action Configuration.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_METER_DISABLE = 0,	/*!< DP class action meter disable */
	DC_DP_METER_REGULAR = 1,	/*!< DP class action meter regular (enable) */
	DC_DP_METER_1 = 2,			/*!< DP class action single meter */
	DC_DP_METER_1_2 = 3		/*!< DP class action pair of cascaded meters */
} dc_dp_class_action_meter_t;

/** \brief Classification TrafficClass (TC) Action Configuration.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_TRAFFIC_CLASS_DISABLE = 0,		/*!< DP class action trafficclass disable */
	DC_DP_TRAFFIC_CLASS_REGULAR = 1,		/*!< DP class action trafficclass regular */
	DC_DP_TRAFFIC_CLASS_ALTERNATIVE = 2	/*!< DP class action trafficclassalternative */
} dc_dp_class_action_trafficclass_t;

/** \brief Classification Interrupt Action Configuration.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_IRQ_DISABLE = 0,		/*!< DP class action irq diasble */
	DC_DP_IRQ_REGULAR = 1,		/*!< DP class action irq regular */
	DC_DP_IRQ_EVENT = 2		/*!< DP class action irq event */
} dc_dp_class_action_irq_t;

/** \brief Classification Cross State Action Configuration.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_CROSS_STATE_DISABLE = 0,	/*!< DP class action crossstate disable*/
	DC_DP_CROSS_STATE_REGULAR = 1,	/*!< DP class action crossstate regular */
	DC_DP_CROSS_STATE_CROSS = 2	/*!< DP class action crossstate cross*/
} dc_dp_class_action_crossstate_t;

/** \brief Classification Critical Frame Action Configuration.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_FRAME_DISABLE = 0,		/*!< DP class action criticalframe disable */
	DC_DP_FRAME_REGULAR = 1,		/*!< DP class action criticalframe regular */
	DC_DP_FRAME_CRITICAL = 2		/*!< DP class action criticalframe critical*/
} dc_dp_class_action_criticalframe_t;

/** \brief Classification Timestamp Action Configuration.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_TIMESTAMP_DISABLE = 0,	/*!< DP class action timestamp disable */
	DC_DP_TIMESTAMP_REGULAR = 1,	/*!< DP class action timestamp regular */
	DC_DP_TIMESTAMP_STORED = 2		/*!< DP class action timestamp stored */
} dc_dp_class_action_timestamp_t;

/** \brief Classification PortMap Action Configuration.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_PORTMAP_DISABLE = 0,					/*!< DP class action portmap disable */
	DC_DP_PORTMAP_REGULAR = 1,					/*!< DP class action portmap  regular*/
	DC_DP_PORTMAP_DISCARD = 2,					/*!< DP class action portmap discard*/
	DC_DP_PORTMAP_CPU = 3,						/*!< DP class action portmap CPU*/
	DC_DP_PORTMAP_ALTERNATIVE = 4,				/*!< DP class action portmap alternative*/
	DC_DP_PORTMAP_MULTICAST_ROUTER = 5,		/*!< DP class action portmap router*/
	DC_DP_PORTMAP_MULTICAST_HW_TABLE = 6,		/*!< DP class action portmap hw table*/
	DC_DP_PORTMAP_ALTERNATIVE_VLAN = 7,		/*!< DP class action portmap VLAN*/
	DC_DP_PORTMAP_ALTERNATIVE_STAG_VLAN = 8	/*!< DP class action portmap STAG VLAN*/
} dc_dp_class_action_portmap_t;

/** \brief Classification VLAN Action Configuration.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_VLAN_DISABLE = 0,		/*!< DP class action vlan disable*/
	DC_DP_VLAN_REGULAR = 1,		/*!< DP class action vlan regular*/
	DC_DP_VLAN_ALTERNATIVE = 2		/*!< DP class action vlan alternative*/
} dc_dp_class_action_vlan_t;

/** \brief Classification Port Filter Action Type.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_PORT_FILTER_ACTION_UNUSED = 0,	/*!< DP class action portfilter disabled*/
	DC_DP_PORT_FILTER_ACTION_1 = 1,		/*!< DP class action portfilter action 1*/
	DC_DP_PORT_FILTER_ACTION_2 = 2,		/*!< DP class action portfilter action 2*/
	DC_DP_PORT_FILTER_ACTION_3 = 3,		/*!< DP class action portfilter action 3*/
	DC_DP_PORT_FILTER_ACTION_4 = 4,		/*!< DP class action portfilter action 4*/
	DC_DP_PORT_FILTER_ACTION_5 = 5,		/*!< DP class action portfilter action 5*/
	DC_DP_PORT_FILTER_ACTION_6 = 6			/*!< DP class action portfilter action 6*/
} dc_dp_class_action_portfilter_t;

/** \brief Classification Action Interface Type.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_IF_CATEGORY_ETHLAN, /*!< Ethernet LAN interface category. */
	DC_DP_IF_CATEGORY_ETHWAN, /*!< Ethernet WAN interface category. */
	DC_DP_IF_CATEGORY_PTMWAN, /*!< xDSL PTM WAN interface category. */
	DC_DP_IF_CATEGORY_ATMWAN, /*!< xDSL ATM WAN interface category. */
	DC_DP_IF_CATEGORY_LTEWAN, /*!< LTE WAN interface category. */
	DC_DP_IF_CATEGORY_WLANDP, /*!< WLAN in Directpath interface category. */
	DC_DP_IF_CATEGORY_WLANNDP, /*!< WLAN in Non-directpath interface category. */
	DC_DP_IF_CATEGORY_LOCAL, /*!< Local interface category. */
	DC_DP_IF_CATEGORY_MAX /*!< Max interface category count. */
} dc_dp_class_action_iftype_t;

/** \brief Processing Path Selectors. (None, 1, 2, or Both)
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_PROCESSING_PATH_UNUSED = 0, 	/*!< DP class action processing path disabled*/
	DC_DP_PROCESSING_PATH_1 = 1,		/*!< DP class action processing path-1*/
	DC_DP_PROCESSING_PATH_2 = 2, 		/*!< DP class action processing path-2*/
	DC_DP_PROCESSING_PATH_1_N_2 = 3 		/*!< DP class action processing path-1 & -2*/
} dc_dp_class_action_processingpath_t;

/** \brief Cross VLAN action in classification rule.
    Used by \ref dc_dp_class_action_t. */
typedef enum {
	DC_DP_CROSS_VLAN_DISABLE = 0,	/*!< DP class action crossvlan disabled*/
	DC_DP_CROSS_VLAN_REGULAR = 1,	/*!< DP class action crossvlan regular*/
	DC_DP_CROSS_VLAN_CROSS = 2		/*!< DP class action crossvlan cross*/
} dc_dp_class_action_crossvlan_t;

/** \brief Rule selection for IPv4/IPv6.
    Used by \ref dc_dp_class_pattern_t. */
typedef enum
{
	/** Rule Pattern for IP selection disabled. */
	DC_DP_PCE_IP_DISABLED  = 0,
	/** Rule Pattern for IPv4. */
	DC_DP_PCE_IP_V4    = 1,
	/** Rule Pattern for IPv6. */
	DC_DP_PCE_IP_V6    = 2
} dc_dp_pce_ip_t;

/** \brief Definition of power management module identifier
*/
typedef enum {
	DC_DP_PS_MODULE_CPU	,
	DC_DP_PS_MODULE_WLAN	,
	DC_DP_PS_MODULE_VE	,
	DC_DP_PS_MODULE_PPE	,
	DC_DP_PS_MODULE_SWITCH,
	DC_DP_PS_MODULE_UART	,
	DC_DP_PS_MODULE_GPTC	,
	DC_DP_PS_MODULE_PCIE	,
	DC_DP_PS_MODULE_USB	,
	DC_DP_PS_MODULE_DEU	,
	DC_DP_PS_MODULE_DP	,	/*Direct Path driver*/
	DC_DP_PS_MODULE_DPL	,	/*Direct Path Lib*/
	DC_DP_PS_MODULE_ETH	,	/*Ethernet driver*/
	DC_DP_PS_MODULE_PATM	,	/*PTM/ATM driver*/
	DC_DP_PS_MODULE_I2C	,
	DC_DP_PS_MODULE_ID_MAX,
} dc_dp_power_module_t;

/** \brief Definition of power management state
*/
typedef enum dc_dp_ps_state {
	/** Power State Invalid. */
	DC_DP_PS_UNDEF,
	/** Power State D0. normal operation freq */
	DC_DP_PS_D0,
	/** Power State D1.  intermediate freq */
	DC_DP_PS_D1,
	/** Power State D2.  intermediate freq */
	DC_DP_PS_D2,
	/** Power State D3. lowest freq */
	DC_DP_PS_D3,
	/** Power State don't care */
	DC_DP_PS_D0D3,
	/** Power State BOOST highest freq, time limited because of thermal aspects */
	DC_DP_PS_BOOST,
} dc_dp_power_state_t;

/* @} */


/** \addtogroup DirectConnect_Datapath_Driver_Unions */
/* @{ */

/** \brief This is a union to describe the IPv4 and IPv6 Address in numeric representation.
		   Used by multiple Structures and APIs. The member selection would be based upon \ref dc_dp_pce_ip_t */
typedef union
{
   /** Describe the IPv4 address.
       Only used if the IPv4 address should be read or configured.
       Cannot be used together with the IPv6 address fields. */
   uint32_t  ipv4;
   /** Describe the IPv6 address.
       Only used if the IPv6 address should be read or configured.
       Cannot be used together with the IPv4 address fields. */
   uint16_t  ipv6[8];
} dc_dp_ip_t;

/* @} */


/** \addtogroup DirectConnect_Datapath_Driver_Structs */
/* @{ */

/** \brief  Rx function callback - basic data struct for ACA Peripherals.
   \param[in] rx_if  Rx If netdevice pointer
   \param[in] tx_if  Tx If netdevice pointer - optional
   \param[in] rx_subif  Rx SubIf pointer
   \param[in] skb  Pointer to packet buffer, like sk_buff
   \param[in] len  Length of the packet (optional as also present in skb->len)
   \return 0 if OK / -1 if error
   \note The receive callback is must to register and is invoked by DirectConnect datapath driver to pass the packets to the peripheral driver
*/
typedef int32_t (*dc_dp_rx_fn_t)(struct net_device *rxif, struct net_device *txif, dp_subif_t *rx_subif, struct sk_buff *skb, int32_t len); /*!< Rx function callback */

/**
   \brief   Get Meta-SubInterface Integer Indentifier (e.g. Station Id) callback
   \param[in] port_id  Port Id of port through which dst MAC address mac_addr can be reached [optional]
   \param[in] netif  Network interface through which packet to dst MAC address mac_addr will be transmitted
   \param[in] mac_addr  MAC Address of Station
   \param[out] meta_sub_id  Meta Sub Interface Id (e.g. STA Id) returned
   \return 0 if OK, -1 on ERROR
*/
typedef int32_t (*dc_dp_get_netif_subinterface_fn_t)(int32_t port_id, struct net_device *netif, char *mac_addr, uint32_t *subinterface); /*!< Get Meta SubInterface Id callback. */

typedef struct dc_dp_recovery_stats dc_dp_recovery_stats_t; /* forward declaration */
/**
   \brief   Get ring/Buffer recovery stats callback
   \param[in] netif  Pointer to Linux netdevice structure
   \param[in] port_id  PMAC port number
   \param[out] stats  Pointer to dc_dp_recovery_stats_t structure
   \param[in] flags  Reserved for future use
   \return 0 if OK, -1 on ERROR
   \note Optional recovery stats callback. The DC datapath driver can tally up the buffer and
   ring stats on the DC peripheral and on the host SoC side, and trigger recovery - for exampe,
   through rebooting system if significant buffrs lost
*/
typedef int32_t (*dc_dp_get_recovery_stats_fn_t)(struct net_device *netif, int32_t port_id, dc_dp_recovery_stats_t *stats, uint32_t flags);

/**
   \brief DirectConnect Datapath Driver Registration Callback.
*/
typedef struct dc_dp_cb {
	dc_dp_rx_fn_t rx_fn;   /*!< Rx function callback */
	dp_stop_tx_fn_t stop_fn;  /*!< Stop Tx function callback for Tx flow control - Optional (NULL) */
	dp_restart_tx_fn_t restart_fn;  /*!< Start Tx function callback for Tx flow control - Optional (NULL)*/
	dc_dp_get_netif_subinterface_fn_t get_subif_fn;  /*!< Get Subinterface metaid callback */
#define get_staid_fn get_subif_fn
	dp_reset_mib_fn_t reset_mib_fn;  /*!< reset registered device's network mib counters */
	dp_get_mib_fn_t get_mib_fn;  /*!< reset registered device's network mib counters */
	dc_dp_get_recovery_stats_fn_t recovery_fn; /*!< Get Recovery related stats */
} dc_dp_cb_t;

/**
   \brief DirectConnect Buffer pools/Chunks Data structure.
*/
typedef struct dc_dp_buf_pool {
	void *pool; /*!< Pointer to pool */
	uint32_t size; /*!< Size of pool in bytes */
} dc_dp_buf_pool_t;

/**
   \brief DirectConnect Datapath Driver Resource structure
*/
typedef struct dc_dp_res {
	int32_t num_bufpools; /*!< Number of buffer pools/chunks allocated for the desired no of buffers */
	dc_dp_buf_pool_t *buflist; /*!< Allocated list of buffer chunks from which packet buffers are carved out.
									Caller needs to free the memory given by buflist pointer. */
	uint32_t num_dma_desc; /*!< Number of DMA descriptors to be programmed to Tx DMA channel of PAE */
	void *dmatx_ring_base; /*!< Physical Base address of the DMA ring */
	void *cbm_dequeue_base; /*!< CBM Dequeue Port Base address for DirectConnect Dequeue Port */
	uint32_t num_dequeue_bufs; /*!< Number of Dequeue descriptors in ring. 32 in DirectConnect Dequeue port, even more for FW based
								    additional DirectConnect support */
	void *cbm_buf_free_base; /*!< CBM Buffer Free port Base address for DirectConnect Buffer Free Port */
	uint32_t num_free_bufs; /*!< Number of free buffer descriptors in ring. 32 in DirectConnect Buffer Free port, even more for FW
								 based additional DirectConnect support. */
} dc_dp_res_t;

/** \brief  Multicast module callback to add/delete a mcast group membership to/from a DirectConnect interface.
   \param[in] grp_id  Multicast group id.
   \param[in] dev  Registered net device.
   \param[in] mc_stream  Multicast stream information.
   \param[in] flags  :
       DC_DP_MC_F_ADD - Add a new mcast group membership to a DirectConnect interface.
       DC_DP_MC_F_DEL - Delete an existing mcast group membership from a DirectConnect interface.
   \return none
   \note Group Identifier is allocated and managed by Multicast Subsystem.
*/
typedef void (*dc_dp_mcast_callback_fn_t)(uint32_t grp_id, struct net_device *dev, void *mc_stream, uint32_t flags);

/**
   \brief IP address data structure - used in Multicast registration.
*/
typedef struct dc_dp_ip_addr {
	dc_dp_iptype_t ip_type; /*!< IPv4 or IPv6 Type */
	union {
		struct in_addr ip4_addr; /*!< IPv4 address */
		struct in6_addr ip6_addr; /*!< IPv6 address */
	} u;  /*!< Union name u */
} dc_dp_ip_addr_t;

/**
   \brief Mutlicast stream (5-tuple) structure
*/
typedef struct dc_dp_mcast_stream {
	struct net_device *mem_dev;	/*!< Member Netdevice */
	dc_dp_ip_addr_t src_ip; /*!< Source ip : can be ipv4 or ipv6 */
	dc_dp_ip_addr_t dst_ip; /*!< Destination ip - GA : can be ipv4 or ipv6 */
	uint32_t proto;	/*!< Protocol type : Mostly UDP for Multicast */
	uint32_t src_port; /*!< Source port */
	uint32_t dst_port; /*!< Destination port */
	uint8_t mac_addr[MAX_ETH_ALEN]; /*!< Member MAC address */
} dc_dp_mcast_stream_t;

/** \brief Classification Rule's matching Pattern Configuration data structure. */
typedef struct dc_dp_class_pattern {
   /** Rule Index */
   int32_t	index;

   /** Index is used (enabled) or set to unused (disabled) */
   bool	enable;

   /** Port ID used  for ingress packet classification */
   bool	port_id_enable;
   /** Port ID value of incoming packets used for classification */
   uint8_t	port_id;
   /** Exclude Port Id Value - When set exclusion of specified port_id takes effect. */
   bool	port_id_exclude;

   /** Incoming Sub-Interface ID Enable */
   bool	subif_id_enable;
   /** Incoming Sub-Interface ID value */
   uint16_t	subif_id;
   /** Exclude of specified Sub-Interface Id value in subif_id */
   bool	subif_id_exclude;

   /** DSCP value used */
   bool	dscp_enable;
   /** DSCP value */
   uint8_t		dscp;
   /** Exclude (Outer) DSCP value */
   bool	dscp_exclude;

   /** Inner DSCP value */
   bool	inner_dscp_enable;
   /** Inner DSCP value */
   uint8_t		inner_dscp;
   /** Exclude of Inner DSCP (inner_dscp) value */
   bool	inner_dscp_exclude;

   /** CTAG VLAN PCP value used */
   bool	pcp_enable;
   /** CTAG VLAN PCP value */
   uint8_t		pcp;
   /* Exclude CTAG  value */
//   bool	ctags_exclude;
   /** Exclude CTAG PCP & DEI value */
   bool	ctag_pcp_dei_exclude;

   /** STAG VLAN PCP/DEI value used */
   bool	stag_pcp_dei_enable;
   /** STAG VLAN PCP value */
   uint8_t		stag_pcp_dei;
   /* Exclude STAG  value */
//   bool	stag_exclude;
   /** Exclude STAG PCP & DEI value */
   bool	stag_pcp_dei_exclude;

   /** Packet length used for classification */
   bool	pkt_lng_enable;
   /** Packet length in bytes */
   uint16_t		pkt_lng;
   /** Packet length Range (from pkt_lng to pkt_lng_range) */
   uint16_t		pkt_lng_range;
   /** Exclude of Packet Length or range value */
   bool	pkt_lng_exclude;

   /** Destination MAC address used */
   bool	mac_dst_enable;
   /** Destination MAC address */
   uint8_t		mac_dst[6];
   /** Destination MAC address nibble mask.
       Please clear the bits of the nibbles that are not marked out and set all other bits.
       The LSB bit represents the lowest data nibble, the next bit the next nibble,
       and so on. */
   uint16_t	mac_dst_mask;
   /** Exclude Destination MAC Address */
   bool	dst_mac_exclude;

   /** Source MAC address used */
   bool	mac_src_enable;
   /** Source MAC address */
   uint8_t	mac_src[6];
   /** Source MAC address nibble mask.
       Please clear the bits of the nibbles that are not marked out and set all other bits.
       The LSB bit represents the lowest data nibble, the next bit the next nibble,
       and so on. */
   uint16_t	mac_src_mask;
   /** Exclude Source MAC Address */
   bool	src_mac_exclude;

   /** MSB Application field used */
   bool	app_data_msb_enable;
   /** MSB Application field.
       The first 2 bytes of the packet content following the IP header
       for TCP/UDP packets (source port field), or the first 2 bytes of packet content
       following the Ethertype for non-IP packets. Any part of this
       content can be masked-out by a programmable bit
       mask 'app_mask_range_msb'. */
   uint16_t	app_data_msb;
   /** MSB Application mask/range selection.
       If set to TRUE, the field 'app_mask_range_msb' is used as a
       range parameter, otherwise it is used as a nibble mask field. */
   bool	app_mask_range_msb_select;
   /** MSB Application mask/range. When used as a range parameter,
       1 bit represents 1 nibble mask of the 'app_data_msb' field.
       Please clear the bits of the nibbles that are not marked out and set all other bits.
       The LSB bit represents the lowest data nibble, the next bit the next nibble,
       and so on. */
   uint16_t	app_mask_range_msb;
   /** MSB Application Data Exclude */
   bool	app_msb_exclude;

   /** LSB Application used */
   bool	app_data_lsb_enable;
   /** LSB Application field.
       The following 2 bytes of the packet behind the 'app_data_msb' field.
       This is the destination port field for TCP/UDP packets,
       or byte 3 and byte 4 of the packet content following the Ethertype
       for non-IP packets. Any part of this content can be masked-out
       by a programmable bit mask 'app_mask_range_lsb'. */
   uint16_t	app_data_lsb;
   /** LSB Application mask/range selection.
       If set to TRUE, the field 'app_mask_range_lsb' is used as
       a range parameter, otherwise it is used as a nibble mask field. */
   bool	app_mask_range_lsb_select;
   /** LSB Application mask/range. When used as a range parameter,
       1 bit represents 1 nibble mask of the 'app_data_lsb' field.
       Please clear the bits of the nibbles that are not marked out and set all other bits.
       The LSB bit represents the lowest data nibble, the next bit the next nibble,
       and so on. */
   uint16_t	app_mask_range_lsb;
   /** LSB Application Data Exclude */
   bool	app_lsb_exclude;

   /** Destination IP Selection. */
   dc_dp_pce_ip_t	dst_ip_select;
   /** Destination IP */
   dc_dp_ip_t	dst_ip;
   /** Destination IP Nibble Mask.
       1 bit represents 1 nibble mask of the 'dst_ip' field.
       Please clear the bits of the nibbles that are not marked out and set all other bits.
       The LSB bit represents the lowest data nibble, the next bit the next nibble,
       and so on. */
   uint32_t	dst_ip_mask;
   /** Exclude Destination IP Value */
   bool	dst_ip_exclude;

   /** Inner Destination IP Selection. */
   dc_dp_pce_ip_t	inner_dst_ip_select;
   /** Inner Destination IP*/
   dc_dp_ip_t	inner_dst_ip;
   /** Inner Destination IP Nibble Mask.
       1 bit represents 1 nibble mask of the 'inner_dst_iP' field.
       Please clear the bits of the nibbles that are not marked out and set all other bits.
       The LSB bit represents the lowest data nibble, the next bit the next nibble,
       and so on. */
   uint32_t	inner_dst_ip_mask;
   /** Exclude Inner Destination IP Value */
   bool	inner_dst_ip_exclude;

   /** Source IP Selection. */
   dc_dp_pce_ip_t	src_ip_select;
   /** Source IP */
   dc_dp_ip_t	src_ip;
   /** Source IP Nibble Mask.
       1 bit represents 1 nibble mask of the 'src_ip' field.
       Please clear the bits of the nibbles that are not marked out and set all other bits.
       The LSB bit represents the lowest data nibble, the next bit the next nibble,
       and so on. */
   uint32_t	src_ip_mask;
   /** Exclude Source IP Value */
   bool	src_ip_exclude;

   /** Inner Source IP Selection. */
   dc_dp_pce_ip_t	inner_src_ip_select;
   /** Inner Source IP */
   dc_dp_ip_t	inner_src_ip;
   /** Inner Src IP Nibble Mask.
       1 bit represents 1 nibble mask of the 'inner_src_ip' field.
       Please clear the bits of the nibbles that are not marked out and set all other bits.
       The LSB bit represents the lowest data nibble, the next bit the next nibble,
       and so on. */
   uint32_t	inner_src_ip_mask;
   /** Exclude Inner Source IP Value */
   bool	inner_src_ip_exclude;

   /** Ethertype used. */
   bool	ether_type_enable;
   /** Ethertype */
   uint16_t	ether_type;
   /** Ethertype Mask.
       1 bit represents 1 nibble mask of the 'ether_type' field.
       Please clear the bits of the nibbles that are not marked out and set all other bits.
       The LSB bit represents the lowest data nibble, the next bit the next nibble,
       and so on. */
   uint16_t	ether_type_mask;
   /** Exclude for Ether Type Value. */
   bool	ether_type_exclude;

   /** IP protocol used */
   bool	protocol_enable;
   /** IP protocol Value */
   uint8_t	protocol;
   /** IP protocol Mask.
       1 bit represents 1 nibble mask of the 'protocol' field.
       Please clear the bits of the nibbles that are not marked out and set all other bits i.e. a set bit 1 indicates that bit is masked out (not compared).
       The LSB bit represents the lowest data nibble, the next bit the next nibble,
       and so on. */
   uint8_t	protocol_mask;
   /** Exclude for IP Protocol Value. */
   bool	protocol_exclude;

   /** Inner IP protocol used. */
   bool	inner_protocol_enable;
   /** Inner IP protocol Value. */
   uint8_t	inner_protocol;
   /** Inner IP protocol Bit Mask. */
   uint8_t	inner_protocol_mask;
   /** Exclude for Inner IP Protocol Value. */
   bool	inner_protocol_exclude;

   /** PPPoE used. */
   bool	session_id_enable;
   /** PPPoE Session Id */
   uint16_t	session_id;
   /** Exclude for PPPoE Session Value. */
   bool	session_id_exclude;

   /** PPP Protocol used */
   bool	ppp_protocol_enable;
   /** PPP Protocol Value */
   uint16_t	ppp_protocol;
   /** PPP protocol Bit Mask (Positional bit 1 signifies masking of corresponding bit value in ppp_protocol). */
   uint16_t	ppp_protocol_mask;
   /** Exclude for PPP Protocol Value. */
   bool	ppp_protocol_exclude;

   /** VLAN ID (CVID) used. */
   bool	vid_enable;
   /** VLAN ID (CVID) */
   uint16_t	vid;
   /** VID mask/range selection.
       If set to 1, the field 'vid_range' is used as
       a range parameter, otherwise it is used as a mask field. */
   bool	vid_range_select;
   /** VLAN ID Range (CVID). Gets used as mask to nVid in case vid_range_select is set to 0 */
   uint16_t	vid_range;
   /** Exclude for VLAN Id (CVLAN). */
   bool	vid_exclude;

   /** STAG VLAN ID used. */
   bool	slan_vid_enable;
   /** STAG VLAN ID */
   uint16_t	slan_vid;
   /** Exclude for SVLAN Id (SVLAN). */
   bool	slan_vid_exclude;

   /** Payload-1 used */
   bool	payload1_src_enable;
   /** Payload-1 Value (16-bits) */
   uint16_t	payload1;
   /** Payload1 mask/range selection.
       If set to TRUE, the field 'payload1' is used as
       a range parameter, otherwise it is used as a bit mask field. */
   bool	payload1_mask_range_select;
   /** Payload-1 Bit mask */
   uint16_t	payload1_mask;
   /** Exclude Payload-1 */
   bool	payload1_exclude;

   /** Payload-2 */
   bool	payload2_src_enable;
   /** Payload-2 Value (16-bits) */
   uint16_t	payload2;
   /** Payload2 mask/range selection.
       If set to TRUE, the field 'payload2' is used as
       a range parameter, otherwise it is used as a bit mask field. */
   bool	payload2_mask_range_select;
   /** Payload-2 Bit mask */
   uint16_t	payload2_mask;
   /** Exclude Payload-2 */
   bool	payload2_exclude;

   /** Parser Flag LSW (Bit position 15 to 0) is used */
   bool	parser_flag_lsb_enable;
   /** Parser Flag LSW Value - each bit indicates specific parsed result */
   uint16_t	parser_flag_lsb;
   /** Corresponding LSW Parser Flag Mask - when the bit is set to 1 corresponding flag gets masked out (ignored). */
   uint16_t	parser_flag_lsb_mask;
   /** Exclude for Parser Flag LSW specified in parser_flag_lsb */
   bool	parser_flag_lsb_exclude;

   /** Parser Flag MSW (Bit 31 to 16) is used */
   bool	parser_flag_msb_enable;
   /** Parser Flag MSW Value - each bit indicates specific parsed result */
   uint16_t	parser_flag_msb;
   /** Corresponding Parser Flag MSW Mask - when the bit is set to 1 corresponding flag gets masked out (ignored). */
   uint16_t	parser_flag_msb_mask;
   /** Exclude for Parser Flag MSW specified in parser_flag_msb */
   bool	parser_flag_msb_exclude;
} dc_dp_class_pattern_t;

/** \brief Classification Rule's filter action data structure. */
typedef struct dc_dp_class_filter_action {
	dc_dp_class_action_portfilter_t portfilter;	/*!< DP class filter action portfilter*/
	dc_dp_class_action_crossstate_t crossstate;	/*!< DP class filter action crossstate*/
} dc_dp_class_filter_action_t;

/** \brief Classification Rule's VLAN action data structure. */
typedef struct dc_dp_class_vlan_action {
	dc_dp_class_action_vlan_t cvlan;			/*!< DP class vlan_action cvlan*/
	uint16_t vlan_id;					/*!< DP class vlan_action vlanid*/
	uint8_t fid;						/*!< DP class vlan_action fid*/
	dc_dp_class_action_vlan_t svlan;			/*!< DP class vlan_action svlan*/
	uint16_t svlan_id;					/*!< DP class vlan_action svlanid*/
	dc_dp_class_action_crossvlan_t cross_vlan;		/*!< DP class vlan_action crossvlan*/
	uint8_t cvlan_ignore;					/*!< DP class vlan_action cvlan ignore*/
} dc_dp_class_vlan_action_t;

/** \brief Classification Rule's Forwarding action data structure. */
typedef struct dc_dp_class_action_fwd {
	dc_dp_class_action_learning_t learning;		/*!< DP class action_fwd learning*/
	uint8_t port_trunk;				/*!< DP class action_fwd port trunk*/
	dc_dp_class_action_portmap_t portmap;		/*!< DP class action_fwd portmap*/
	uint32_t forward_portmap;			/*!< DP class action_fwd forward_portmap*/
	uint16_t forward_subifid;			/*!< DP class action_fwd forward_subifid*/
	uint8_t routextid_enable;			/*!< DP class action_fwd routextid_enable*/
	uint8_t routextid;				/*!< DP class action_fwd routextid*/
	uint8_t rtdestportmaskcmp;			/*!< DP class action_fwd rt dest port compare*/
	uint8_t rtsrcportmaskcmp;			/*!< DP class action_fwd rt src port compare*/
	uint8_t rtdstipmaskcmp;				/*!< DP class action_fwd rt dest ip compare*/
	uint8_t rtsrcipmaskcmp;				/*!< DP class action_fwd rt src ip compare*/
	uint8_t rtinneripaskey;				/*!< DP class action_fwd rt inner ip compare enable*/
	uint8_t rtaccelenable;				/*!< DP class action_fwd rt acceleration enable*/
	uint8_t rtctrlenable;				/*!< DP class action_fwd rt ctrl enable*/
	dc_dp_class_action_processingpath_t processpath;  /*!< DP class action_fwd processing path*/
} dc_dp_class_action_fwd_t;

/** \brief Classification Rule's QoS action data structure. */
typedef struct dc_dp_class_action_qos {
	dc_dp_class_action_trafficclass_t trafficclass;	/*!< DP class action_qos trafficclass*/
	uint8_t alt_trafficclass;				/*!< DP class action_qos alternate traffic class*/
	dc_dp_class_action_meter_t meter;			/*!< DP class action_qos meter*/
	uint8_t meterid;					/*!< DP class action_qos meter id*/
	dc_dp_class_action_criticalframe_t criticalframe;	/*!< DP class action_qos critical frame*/
	uint8_t remark;						/*!< DP class action_qos remark*/
	uint8_t remarkpcp;					/*!< DP class action_qos remark pcp*/
	uint8_t new_pcp;					/*!< DP class action_qos new pcp*/
	uint8_t remark_stagpcp;					/*!< DP class action_qos remark stag pcp*/
	uint8_t remark_stagdei;					/*!< DP class action_qos remark stag dei*/
	uint8_t remark_dscp;					/*!< DP class action_qos remark dscp*/
	uint8_t remark_class;					/*!< DP class action_qos remark class*/
	uint8_t flowid_enabled;					/*!< DP class action_qos flowid enable*/
	uint16_t flowid; 					/*!< DP class action_qos flowid*/
} dc_dp_class_action_qos_t;

/** \brief Classification Rule's Management (such as IRQ & Timestamp) action data structure. */
typedef struct dc_dp_class_action_mgmt {
	dc_dp_class_action_irq_t irq;					/*!< DP class action_mgmt irq action*/
	dc_dp_class_action_timestamp_t timestamp;		/*!< DP class action_mgmt timestamp action*/
} dc_dp_class_action_mgmt_t;

/** \brief Classification Rule's Overall grouped action data structure. */
typedef struct dc_dp_class_action {
	dc_dp_class_filter_action_t filter_action;	/*!< DP filtering actions */
	dc_dp_class_vlan_action_t vlan_action;		/*!< DP VLAN actions */
	dc_dp_class_action_fwd_t fwd_action;		/*!< DP forwarding actions */
	dc_dp_class_action_qos_t qos_action;		/*!< DP QOS actions */
	dc_dp_class_action_mgmt_t mgmt_action;		/*!< DP Management actions */
	dc_dp_class_action_iftype_t iftype;		/*!< DP Interface type actions */
	uint8_t rmon_action;				/*!< DP RMON actions */
	uint8_t rmon_id;				/*!< DP RMON id to be used */
} dc_dp_class_action_t;

/** \brief Classification Rule Overall Configuration data structure. */
typedef struct dc_dp_class_rule {
	dc_dp_class_devingress_t in_dev; 		/*!< DP ingress device id*/
	uint8_t order;					/*!< DP order within the category selected */
	dc_dp_class_category_t category;		/*!< DP category of the classifier rule */
	dc_dp_class_sub_category_t subcategory;	/*!< DP subcategory of the classifier rule */
	char owner[32];							/*!< DP owner of the rule to be configures */
	dc_dp_class_pattern_t pattern;			/*!< DP pattern of the rule to be configured */
	dc_dp_class_action_t action;			/*!< DP action of the rule to be confiured */
	uint16_t uidx;							/*!< DP unique ID returned by the classification API upon successful addition of rule */
} dc_dp_class_rule_t;

/**
   \brief DirectConnect Power Saving (CoC) Thresholds values for different power states.
*/
typedef struct dc_dp_ps_threshold {
	int32_t th_d0; /*!< Power State D0 (Highest Power) Threshold Level */
	int32_t th_d1; /*!< Power State D1 Threshold Level */
	int32_t th_d2; /*!< Power State D2 Threshold Level */
	int32_t th_d3; /*!< Power State D3 (Lowest Power) Threshold Level */
} dc_dp_ps_threshold_t;

/**
   \brief DirectConnect Power State Module Information to be used for registration.
*/
typedef struct dc_dp_ps_module_info {
    struct list_head        list;  /*!< List Head - TBC */
    char                *module_name;  /*!< Power State registeirng module Name */
    dc_dp_power_module_t     pmcu_module; /*!< Enum of Modules */
    uint32_t            pmcu_module_nr; /*!< PMCU Module NR number */
    int32_t             power_feature_stat; /*!< Current Power State */
    int32_t (*dc_dp_ps_state_get) (dc_dp_power_state_t *pmcu_state);  /*!< Callback to query current power state */
    int32_t (*dc_dp_ps_state_switch) (int32_t pmcu_pwr_state_ena); /*!< Callback to enable power state */
} dc_dp_ps_module_info_t;

/**
   \brief Wake-on-LAN Config Structure
*/
typedef struct dc_dp_wol_cfg {
	uint8_t wol_mac[MAX_ETH_ALEN]; /*!< Wake-on-LAN MAC address - part of Magic packet (16 times repeat)*/
	bool wol_passwd_enable; /*!< Wake-on-LAN password enable */
	uint8_t wol_passwd[MAX_ETH_ALEN]; /*!< Wake-on-LAN password */
} dc_dp_wol_cfg_t;

/**
   \brief Ring recovery stats Structure
   \note If stats difference exceeds pre-configured threshold the system would be rebooted to recover from loss of descriptiors.
*/
typedef struct dc_dp_recovery_stats {
	uint32_t cbm_announced_desc_cum; /*!< Announced cumulative desc count by UMT*/
	uint32_t cbm_to_be_pulled_counter; /*!< desc count to be pulled from DC CBM Dequeue port */
	uint32_t cbm_to_be_freed_buffer; /*!< buffer count to be freed to DC CBM Dequeue Free port */
	uint32_t dc_rx_outstanding_desc; /*!< DC client driver/fw HD ring outstanding decsriptors-
					 Add num_desc when written to HD ring and subtract num_desc based on UMT message received. */
} dc_dp_recovery_stats_t;

/* @} */


/** \addtogroup DirectConnect_Datapath_Driver_APIs */
/* @{ */

/** \brief  DirectConnect Datapath Allocate Data structure port may map to an exclusive netdevice
    like in the case of ethernet LAN ports.	In other cases like WLAN, the physical port is a Radio port,
    while netdevices are Virtual Access Points (VAPs). In this case, the AP netdevice can be passed.
    Alternately, driver_port & driver_id will be used to identify this port.
   \param[in] owner  Kernel module pointer which owns the port
   \param[in] dev_port  Physical Port Number of this device managed by the driver
   \param[in] dev  Pointer to Linux netdevice structure (optional)
   \param[in] port_id  Optional port_id number requested. Usually, 0 and allocated by driver
   \param[in] flags  : Use Datapath driver flags for Datapath Port Alloc
   	-  DC_DP_F_FAST_WLAN : Allocate the port
   	-  DC_DP_F_DEREGISTER : Deallocate the already allocated port
   \return  Returns PMAC Port number / -1 if error
*/
int32_t
dc_dp_alloc_port (
	struct module *owner,
	uint32_t dev_port,
	struct net_device *dev,
	int32_t port_id,
	uint32_t flags
);

/** \brief  Higher layer Driver Datapath registration API
   \param[in] owner  Kernel module pointer which owns the port
   \param[in] port_id  Datapath Port Id (PMAC Port No) on which to register
   \param[in] dev  Pointer to Linux netdevice structure
   \param[in] datapathcb  Callback registration structure
   \param[in] num_bufs_req  Number of Tx buffers to allocate (DirectConnect Peripheral -> GRX500 SoC PAE direction)
   \param[in] umt_base  Base address of UMT write location (in DirectConnect HW memory)
   \param[in] umt_len  Number of bytes of UMT message to write to umt_base
   \param[out] resources  Buffer, DMA ring and CBM port resource addresses
   \param[in] flags  : Special input flags to alloc routine
   		- DP_F_DEREGISTER : Deregister the device
   \return 0 if OK / -1 if error
   \note This is the first registration to be invoked by any ACA peripheral. Subsequently additional registrations like Multicast, Ring-Recovery or Power Saving (PS) to be done.
*/
int32_t
dc_dp_register_dev (
	struct module *owner,
	uint32_t port_id,
	struct net_device *dev,
	dc_dp_cb_t *datapathcb,
	uint32_t num_bufs_req,
	void *umt_base,
	int32_t umt_len,
	dc_dp_res_t *resources,
	uint32_t flags
);

/** \brief  Allocates datapath subif number to a sub-interface netdevice.
    Sub-interface value must be passed to the driver.
    The port may map to an exclusive netdevice like in the case of ethernet LAN ports.
   \param[in] owner  Kernel module pointer which owns the port
   \param[in] dev  Pointer to Linux netdevice structure
   \param[in,out] subif_id  Pointer to subif_id structure including port_id
   \param[in] flags :
       DP_F_DEREGISTER - De-register already registered subif/vap
   \return 0 if OK / -1 if error
   \note Sub-Interface is applicable for logical or virtual interfaces liek VAP (SSID) or VLAN.
*/
int32_t
dc_dp_register_subif (
	struct module *owner,
	struct net_device *dev,
	dp_subif_t *subif_id,
	uint32_t flags
);

/** \brief  Transmit packet to low-level Datapath driver
   \param[in] rx_if  Rx If netdevice pointer - optional
   \param[in] rx_subif  Rx SubIf pointer - optional
   \param[in] skb  Pointer to packet buffer like sk_buff
   \param[in] len  Length of packet to transmit
   \param[in] flags : Reserved for future use.
   \return 0 if OK / -1 if error
   \note Either rx_if or rx_subif would be passed in this routine.
*/
int32_t
dc_dp_xmit (
	struct net_device *rx_if,
	dp_subif_t *rx_subif,
	struct sk_buff *skb,
	int32_t len,
	uint32_t flags
);

/** \brief  Set port id in DMA Desc DWORD 1 (DW1).
   \param[in,out] desc_dw  Pointer to descriptor DWORD
   \param[in] port_id  port identifier value
   \param[in] flags : Reserved
   \return  0 if OK / -1 if error
   \note  This utility function will set and return DMA Decriptor's DWord-1 from given port value.
*/
static inline int32_t
dc_dp_set_ep_pkt (
	uint32_t *desc_dw,
	int32_t port_id,
	uint32_t flags
)
{
	struct dma_tx_desc_1 *desc_1 = (struct dma_tx_desc_1 *) desc_dw;

	if (!desc_1) {
		return DP_FAILURE;
	}

	desc_1->field.ep = port_id;

	return DP_SUCCESS;
}

/** \brief  Get port id from DMA Desc DWORD 1 (DW1).
   \param[in] desc_dw  Pointer to descriptor DWORD
   \param[out] port_id  Port identifier value
   \param[in] flags  : Reserved
   \return  0 if OK / -1 if error
   \note This utility funciton will extract port identifier fromgiven DMA Descriptor's DWord-1.
*/
static inline int32_t
dc_dp_get_ep_pkt (
	uint32_t *desc_dw,
	int32_t *port_id,
	uint32_t flags
)
{
	struct dma_tx_desc_1 *desc_1 = (struct dma_tx_desc_1 *) desc_dw;

	if (!desc_1 || !port_id) {
		return DP_FAILURE;
	}

	*port_id = desc_1->field.ep;

	return DP_SUCCESS;
}

/** \brief  Set sub-interface id in DMA Desc DWORD 0 (DW0).
   \param[in,out] desc_dw  Pointer to descriptor DWORD
   \param[in] subif_id  subinterface id value
   \param[in] flags : Reserved
   \return  0 if OK / -1 if error
   \note This funciton will return DMA Descriptior's DWord-0 by setting the given sub-interface id value.
*/
static inline int32_t
dc_dp_set_subifid_pkt (
	uint32_t *desc_dw,
	int32_t subif_id,
	uint32_t flags
)
{
	struct dma_tx_desc_0 *desc_0 = (struct dma_tx_desc_0 *) desc_dw;

	if (!desc_0) {
		return DP_FAILURE;
	}

	desc_0->field.dest_sub_if_id = subif_id;

	return DP_SUCCESS;
}

/** \brief  Get sub-interface id from DMA Desc DWORD 0 (DW0).
   \param[in] desc_dw  Pointer to descriptor DWORD
   \param[out] subif_id  subinterface id value
   \param[in] flags  : Reserved
   \return  0 if OK / -1 if error
   \note This utility function will extract sub-interface id from given DMA Descriptor DWord-0.
*/
static inline int32_t
dc_dp_get_subifid_pkt (
	uint32_t *desc_dw,
	int32_t *subif_id,
	uint32_t flags
)
{
	struct dma_tx_desc_0 *desc_0 = (struct dma_tx_desc_0 *) desc_dw;

	if (!desc_0 || !subif_id) {
		return DP_FAILURE;
	}

	*subif_id = desc_0->field.dest_sub_if_id;

	return DP_SUCCESS;
}

/** \brief  Disconnect a particular MAC addr or an network device - remove all MAC table entries
    and/or routing sessions which use the specified MAC address.
   \param[in] netif  netdevice pointer through which all entries must be removed from acceleration - optional
   \param[in] subif_id  Sub-interface identifier to remove on netif
   \param[in] mac_addr  MAC address to remove
   \param[in] flags  Reserved for future use
   \return 0 if OK / -1 if error
   \note One of subif_id, mac_addr or netif must be specified
*/
int32_t
dc_dp_disconn_if (
	struct net_device *netif,
	dp_subif_t *subif_id,
	uint8_t mac_addr[MAX_ETH_ALEN],
	uint32_t flags
);

/** \brief  Register/De-register a DirectConnect interface to MCAST helper module
   \param[in] dev  Net device to be registered, e.g., wlan0_0.
   \param[in] owner  Kernel module pointer which owns the port.
   \param[in] cb  Multicast callback function.
   \param[in] flags  :
       DC_DP_F_MC_REGISTER - Register a DirectConnect interface.
       DC_DP_F_MC_DEREGISTER - De-register already registered DirectConnect interface.
   \return 0 if OK / -1 if error
   \note
*/
int32_t
dc_dp_register_mcast_module (
	struct net_device *dev,
	struct module *owner,
	dc_dp_mcast_callback_fn_t cb,
	uint32_t flags
);

/** \brief  Provide a Priority (802.1D Priority) to WMM Class/TID map for the
 * given WiFi Radio/net_device
   \param[in] port_id  Port Id on which mapping is to be updated
   \param[in] netif  Pointer to stack network interface structure on which mapping is to be updated
   \param[in] prio2wmm Array of priority to WMM Class/TID mapping values
   \return 0 if OK / -1 if error
   \note  One of port_id or netif must be specified
   \note The DC driver must  configure the Egress PMAC table to mark the WMM Class/TID in the descriptor DW1[7:4]
*/
int32_t
dc_dp_map_prio_wmm_class (
	int32_t	port_id,
	struct net_device *netif,
	uint8_t prio2wmm[DC_DP_MAX_WIFI_CLASS]
);

/** \brief  Provide WMM AC/TID mapping for a given packet
   \param[in] port_id  Port Id on which mapping is to be updated
   \param[in] dst_netif  Pointer to stack network interface structure
   \param[in] skb  : pointer to network packet/sk_buff structure
   \return  WMM value marked if successful, -1 on error
   \note The WAVE/ DC Datapath driver needs to use skb->extmark/skb->priority to mark and return WMM Class/TID
*/
/* static inline */int32_t
dc_dp_mark_pkt_wmm (
	int32_t	port_id,
	struct net_device *dst_netif,
	struct sk_buff *skb
);

/** \brief  Get DirectConnect interface statistics. Either netdevice or subif_id has to be passed to this API.
   \param[in] netif  Pointer to Linux netdevice structure
   \param[in] subif_id  Datapath Port Number and Sub-Interface (if applicable else -1).
   \param[out] if_stats  Pointer to Linux rtnl_link_stats64 structure
   \param[in] flags  :
       DP_F_SUBIF_LOGICAL - to be used when the interface is subif type
   \return 0 if OK / -1 if error
*/
int32_t
dc_dp_get_netif_stats (
	struct net_device *netif,
	dp_subif_t *subif_id,
	struct rtnl_link_stats64 *if_stats,
	uint32_t flags
);

/** \brief  Clear DirectConnect interface statistics. Either netdevice or subif_id has to be passed.
   \param[in] netif  Pointer to Linux netdevice structure
   \param[in] subif_id  Datapath Port Number and Sub-Interface (if applicable else -1).
   \param[in] flags  Flag Type to pass additional info such as
       DP_F_SUBIF_LOGICAL - to be used when the interface is subif type
   \return 0 if OK / -1 if error
*/
int32_t
dc_dp_clear_netif_stats (
	struct net_device *netif,
	dp_subif_t *subif_id,
	uint32_t flags
);

/** \brief  Add a classifier rule entry - pattern and action to the table of the packet classification engine.
   The pattern part describes the pattern to identify an incoming packet to which the dedicated actions should be applied.
   \param[in] rule  Pointer to Classifier
   \param[in] flags  Reserved for future use
   \return 0 if OK / -1 if error
*/
int32_t
dc_dp_add_class_rule (
	dc_dp_class_rule_t *rule,
	uint32_t flags
);

/** \brief  Modify a classifier rule entry - pattern and action to the table of the packet classification engine.
   The pattern part describes the parameter to identify an incoming packet to which the dedicated actions should be applied.
   \param[in] rule  Pointer to Classifier
   \param[in] flags  Reserved for future use
   \return 0 if OK / -1 if error
*/
int32_t
dc_dp_mod_class_rule (
	dc_dp_class_rule_t *rule,
	uint32_t flags
);

/** \brief  Delete a classifier rule entry for given reference.
   \param[in] rule  Pointer to Classifier with key of (Category & Order values) or Switch PCE Rule Index
   \param[in] flags  Reserved for future use
   \return 0 if OK / -1 if error
*/
int32_t
dc_dp_del_class_rule (
	dc_dp_class_rule_t *rule,
	uint32_t flags
);

/** \brief  Read a classifier rule entry for given reference.
   \param[in,out] rule  Pointer to Classifier
   \param[in] flags  Reserved for future use
   \return 0 if OK / -1 if error
*/
int32_t
dc_dp_get_class_rule (
	dc_dp_class_rule_t *rule,
	uint32_t flags
);

/** \brief  Get Wake-on-LAN configuration.
   \param[out] cfg  Wake-on-LAN Configuration
   \param[in] flags  : Reserved
   \return 0 if OK / -1 if error
*/
int32_t
dc_dp_get_wol_cfg (
	dc_dp_wol_cfg_t *cfg,
	uint32_t flags
);

/** \brief  Set Wake-on-LAN config
   \param[in] cfg  Wake-on-LAN Configuration
   \param[in] flags  : Reserved
   \return  0 if OK / -1 if error
*/
int32_t
dc_dp_set_wol_cfg (
	dc_dp_wol_cfg_t *cfg,
	uint32_t flags
);

/** \brief  Enable/Disable Wake-on-LAN functionality
   \param[in] port_id  PMAC port number
   \param[in] enable  Enable Wake-on-LAN if 1, disable Wake-on-LAN if 0
   \return  0 if OK / -1 if error
*/
int32_t
dc_dp_set_wol_ctrl (
	int32_t port_id,
	uint32_t enable
);

/** \brief  Get Wake-on-LAN status
   \param[in] port_id  PMAC port number
   \return  1 if Enabled/ 0 if Disabled / -1 if error
*/
int32_t
dc_dp_get_wol_ctrl_status (
	int32_t port_id
);

/* @} */


/** \addtogroup DirectConnect_Datapath_Driver_Power_Management_Wrapper_API */
/* @{ */

/** \brief  Register/De-register to/from the CPUFreq framework
   \param[in] nb  Notifier function to register
   \param[in] notify_type  Notifier list type (DC_DP_PS_TRANSITION_NOTIFIER / DC_DP_PS_POLICY_NOTIFIER)
   \param[in] flags  :
   	-  DC_DP_F_PS_REGISTER : Register to CPUFreq framework
   	-  DC_DP_F_PS_DEREGISTER : De-register from CPUFreq framework
   \return  0 if OK / < 0 if error
*/
int32_t
dc_dp_register_power_notifier (
	struct notifier_block *nb,
	uint32_t notify_type,
	uint32_t flags
);

/** \brief  Request new system power state
   \param[in] module  Module identifier
   \param[in] module_nr Module Number
   \param[in] new_state  Power state as defined in dc_dp_power_state_t
   \return  DC_DP_PS_SUCCESS / DC_DP_PS_DENIED / DC_DP_PS_NOACTIVITY / DC_DP_PS_NOTDEFINED
*/
int32_t
dc_dp_req_power_state (
	dc_dp_power_module_t module,
	uint8_t module_nr,
	dc_dp_power_state_t new_state
);

/** \brief  Get Power Mgmt polling period per module
   \param[in] module  Module identifier
   \param[in] module_nr
   \return  Polling period
*/
int32_t
dc_dp_get_ps_poll_period (
	dc_dp_power_module_t module,
	uint8_t module_nr
);

/** \brief  Get power state related threshold values per module
   \param[in] module  Module identifier
   \param[in] module_nr
   \return  dc_dp_ps_threshold_t pointer if OK / NULL if error
*/
dc_dp_ps_threshold_t *
dc_dp_get_ps_threshold (
	dc_dp_power_module_t module,
	uint8_t module_nr
);

/** \brief  Add/Delete module from CPUFReq module list
   \param[in] head  Module list head pointer
   \param[in] add  DC_DP_PS_LIST_ADD / DC_DP_PS_LIST_DEL
   \return  0 if OK / < 0 if error
*/
int32_t
dc_dp_mod_ps_list (
	struct list_head *head,
	int32_t add
);

/** \brief  Convert frequency to power state
   \param[in] freq_khz frequency (in khz)
   \return  power state if OK / < 0 if error
*/
dc_dp_power_state_t
dc_dp_get_ps_from_khz (
	uint32_t freq_khz
);

/* @} */

#endif /* _UGW_DIRECTCONNECT_DP_API_LEGACY_H_ */
