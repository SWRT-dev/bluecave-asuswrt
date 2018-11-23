/*##################################################################################################
# "Copyright (c) 2013 Intel Corporation                                                            #
# DISTRIBUTABLE AS SAMPLE SOURCE SOFTWARE                                                          #
# This Distributable As Sample Source Software is subject to the terms and conditions              #
# of the Intel Software License Agreement for the Intel(R) Cable and GW Software Development Kit"  #
##################################################################################################*/

/*! \file 	wlan_wlan_beerock.c
    \brief 	This file implements the interface between the HostAPD and the FAPI.
    \todo 	Add license header
	\todo   Move all system() function uses to clean C functions
	\todo   merge all db load\store functions to one
	\todo   Solve all mac addresses workarounds PUMA \ UBOOT  \ UGW
	\todo   Create script that align DB sources to UGW version and converts to RPC format, a must!!!!!
	\todo   Add null pointer verifications and clean-up 
 */

#include <stdio.h>
#include <stdlib.h>
#include "fapi_wlan_beerock.h"
#include "help_objlist.h"


/************************************/
/* Global Functions for general use */
/************************************/
int bssidGet(const char *ifname, char *BSSID)
{
	char    *localBSSID;
	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	if (wlObj == NULL)
	{
		printf("%s; create object ERROR ==> Abort!\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if (fapi_wlan_mac_address_get(ifname, wlObj, 0) == UGW_FAILURE)
	{
		printf("%s; fapi_wlan_mac_address_get returned ERROR ==> Abort!\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	localBSSID = paramValueFromObjGet(wlObj, "Device.WiFi.SSID", "MACAddress");
	printf("%s; (ifname= '%s') localBSSID= '%s'\n", __FUNCTION__, ifname, localBSSID);
	if (localBSSID == NULL)
	{
		printf("%s; localBSSID is NULL ==> Abort!\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	strncpy(BSSID, localBSSID, 17);

	return UGW_SUCCESS;
}


int channelNumberGet(const char *ifname, char *ChannelNumber)
{
	char    *localChannelNumber = NULL;
	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	if (wlObj == NULL)
	{
		printf("%s; create object ERROR ==> Abort!\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if (fapi_wlan_channel_get(ifname, wlObj, 0) == UGW_FAILURE)
	{
		printf("%s; fapi_wlan_channel_get returned ERROR ==> Abort!\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	localChannelNumber = paramValueFromObjGet(wlObj, "Device.WiFi.Radio", "Channel");
	printf("%s; localChannelNumber= '%s'\n", __FUNCTION__, localChannelNumber);
	if ( (localChannelNumber == NULL) || (strlen(localChannelNumber) > 3) )
	{
		printf("%s; Illegal ChannelNumber ('%s') ==> Abort!\n", __FUNCTION__, localChannelNumber);
		return UGW_FAILURE;
	}

	strcpy(ChannelNumber, localChannelNumber);

	return UGW_SUCCESS;
}


ObjList *subObjForTwoSameFieldsGet(ObjList *wlObjDB, char *BSSID, char *nr_BSSID)
{
	ObjList *subObjDB;
	char    *BSSID_db, *nr_BSSID_db;

	FOR_EACH_OBJ(wlObjDB, subObjDB)
	{
		BSSID_db    = paramValueFromSubObjGet(subObjDB, "BSSID");
		nr_BSSID_db = paramValueFromSubObjGet(subObjDB, "nr_BSSID");

		if ( (BSSID_db != NULL) &&
		     (nr_BSSID_db != NULL) &&
			 (!strcmp(BSSID, BSSID_db)) &&
			 (!strcmp(nr_BSSID, nr_BSSID_db)) )
		{
			printf("%s; Found object with the same BSSID ('%s') and the same nr_BSSID ('%s')\n", __FUNCTION__, BSSID, nr_BSSID);
			return subObjDB;
		}
	}

	printf("%s; There is no object with the same BSSID ('%s') and the same nr_BSSID ('%s')\n", __FUNCTION__, BSSID, nr_BSSID);

	return NULL;
}


/*****************************************/
/* 1) AP Manager to FAPI - API Functions */
/*****************************************/

/*************/
/* 1.1) Init */
/*************/
int fapi_wlan_hostapd_callback_register(const char *ifname, FapiWlanCallBack fapiWlanCallBackFunc, void *context)
{
	int ret = UGW_SUCCESS;
	fapi_wlan_vendors_t vendor;

	fapi_wlan_trace_in(__FUNCTION__, ifname, NULL);

	vendor = fapi_wlan_get_vendor(ifname);
	if ( (int)vendor > 0 ) {
		FAPI_WLAN_ASSERT(vendor < FAPI_WLAN_MAX_VENDORS && fapi_wlan_vendor_ops[vendor] && fapi_wlan_vendor_ops[vendor]->fapi_wlan_hostapd_callback_register);
		ret = fapi_wlan_vendor_ops[vendor]->fapi_wlan_hostapd_callback_register(ifname, fapiWlanCallBackFunc, context);
	}
	else {
		ret = UGW_FAILURE;
	}

	fapi_wlan_trace_out(__FUNCTION__, ifname, NULL, ret);

	return ret;
}

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_start_monitoring);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_stop_monitoring);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_interface_info_get);


int fapi_wlan_hostapd_socket_get(const char *ifname, void **ctrlInterface)
{
	int ret = UGW_SUCCESS;
	fapi_wlan_vendors_t vendor;

	fapi_wlan_trace_in(__FUNCTION__, ifname, NULL);

	vendor = fapi_wlan_get_vendor(ifname);
	if ( (int)vendor > 0 ) {
		FAPI_WLAN_ASSERT(vendor < FAPI_WLAN_MAX_VENDORS && fapi_wlan_vendor_ops[vendor] && fapi_wlan_vendor_ops[vendor]->fapi_wlan_hostapd_socket_get);
		ret = fapi_wlan_vendor_ops[vendor]->fapi_wlan_hostapd_socket_get(ifname, ctrlInterface);
	}
	else {
		ret = UGW_FAILURE;
	}

	fapi_wlan_trace_out(__FUNCTION__, ifname, NULL, ret);

	return ret;
}


int fapi_wlan_hostapd_report_process(const char *ifname, ObjList *wlObj, char *opCode)
{
	int ret = UGW_SUCCESS;
	fapi_wlan_vendors_t vendor;

	fapi_wlan_trace_in(__FUNCTION__, ifname, wlObj);

	vendor = fapi_wlan_get_vendor(ifname);
	if ( (int)vendor > 0 ) {
		FAPI_WLAN_ASSERT(vendor < FAPI_WLAN_MAX_VENDORS && fapi_wlan_vendor_ops[vendor] && fapi_wlan_vendor_ops[vendor]->fapi_wlan_hostapd_report_process);
		ret = fapi_wlan_vendor_ops[vendor]->fapi_wlan_hostapd_report_process(ifname, wlObj, opCode);
	}
	else {
		ret = UGW_FAILURE;
	}

	fapi_wlan_trace_out(__FUNCTION__, ifname, wlObj, ret);

	return ret;
}


DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_radio_state_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_mac_address_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_channel_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_radio_ctrl_a_filter_bank_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_radio_ctrl_a_filter_bank_get);


/*****************/
/* 1.2) Controls */
/*****************/
int fapi_wlan_general_command_set(const char *ifname, char *command, char *report)
{
	int ret = UGW_SUCCESS;
	fapi_wlan_vendors_t vendor;

	fapi_wlan_trace_in(__FUNCTION__, ifname, NULL);

	vendor = fapi_wlan_get_vendor(ifname);
	if ( (int)vendor > 0 ) {
		FAPI_WLAN_ASSERT(vendor < FAPI_WLAN_MAX_VENDORS && fapi_wlan_vendor_ops[vendor] && fapi_wlan_vendor_ops[vendor]->fapi_wlan_general_command_set);
		ret = fapi_wlan_vendor_ops[vendor]->fapi_wlan_general_command_set(ifname, command, report);
	}
	else {
		ret = UGW_FAILURE;
	}

	fapi_wlan_trace_out(__FUNCTION__, ifname, NULL, ret);

	return ret;
}


DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_iface_enable);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_iface_disable);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_sta_steer);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_bss_transition_management_req);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_sta_allow);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_sta_deny);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_sta_disassociate);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_sta_deauthenticate);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_channel_switch);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_unassociated_devices_info_req);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_restricted_channels_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_failsafe_channel_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_mbo_sta_allow);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_mbo_sta_deny);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_neighbor_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_neighbor_remove);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_cellular_pref_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_cellular_aware_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_set_command_send);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_channel_load_request);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_channel_load_response);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_beacon_request);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_beacon_report_response);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_self_beacon_request);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_sta_statistics_request);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_sta_statistics_response);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_sta_link_measurements_request);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_sta_link_measurements_response);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_noise_histigram_request);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_noise_histogram_response);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_oce_wan_metrics_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_oce_assoc_reject_set);


/*********************/
/* 1.3) Measurements */
/*********************/
DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_sta_measurement_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_vap_measurement_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_radio_info_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_acs_report_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_failsafe_channel_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_restricted_channels_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_beacon_report_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_connection_status_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_metrics_get);


/********************/
/* 1.4) Client Mode */
/********************/
DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_network_add);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_network_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_network_enable);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_network_select);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_network_disconnect);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_network_remove);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_scan_start);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_scan_results_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_connection_status_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_data_routing_mode_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_data_routing_mode_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_client_mode_radio_info_get);
