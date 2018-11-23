/******************************************************************************

                         Copyright (c) 2015
                        Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/*  ***************************************************************************** 
 *         File Name    : fapi_wlan_cli.c                                    	*
 *         Description  : cli utility to test FAPI wlan common layer     		* 
 *                                                                              *
 *  *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "fapi_wlan_private.h"
#include "fapi_wlan.h"
#include "help_objlist.h"
#include "wlan_config_api.h"

#ifndef LOG_LEVEL
uint16_t   LOGLEVEL = SYS_LOG_DEBUG + 1;
#else
uint16_t   LOGLEVEL = LOG_LEVEL + 1;
#endif
#ifndef LOG_TYPE
uint16_t   LOGTYPE = SYS_LOG_TYPE_FILE;
#else
uint16_t   LOGTYPE = LOG_TYPE;
#endif

#define DB_CLI_FILE_NAME "input_cli"

enum
{
	BADAPI = (-1),
	ERROR_VALUE = (-1),
	FLAG = 0,
	LOAD,
	INIT,
	UNINIT,
	RESET,
	DISABLE_RPC_MODE,
	CREATE_CONFIG,
	FACTORY_DEAFULTS,
	CREATE_VAP,
	DELETE_VAP,
	SET_RADIO_TR181,
	SET_SSID_TR181,
	SET_AP_TR181,
	SET_SECURITY_TR181,
	SET_WPS_TR181,
	SET_WMM_STA_TR181,
	SET_WMM_AP_TR181,
	SET_WDS_TR181,
	SET_HOTSPOT_TR181,
	GET_INTERFACE_NAME,
	GET_CHANNEL,
	SET_CHANNEL,
	GET_AUTO_CHANNEL_ENABLE,
	SET_AUTO_CHANNEL_ENABLE,
	GET_RADIO_ENABLE,
	SET_RADIO_ENABLE,
	GET_ENABLE,
	SET_ENABLE,
	GET_SSID,
	SET_SSID,
	GET_SSID_ADVERTISEMENT_ENABLED, 
	SET_SSID_ADVERTISEMENT_ENABLED,
	GET_MAC_ADD_CONTROL_ENABLED,
	SET_MAC_ADD_CONTROL_ENABLED,
	GET_BEACON_TYPE,
	SET_BEACON_TYPE,
	GET_KEY_PASS_PHRASE,
	SET_KEY_PASS_PHRASE,
	GET_WEP_KEY_PASSPHRASE,
	SET_WEP_KEY_PASSPHRASE,
	GET_BASIC_ENC_MODE,
	SET_BASIC_ENC_MODE,
	GET_BASIC_AUTH_MODE,
	SET_BASIC_AUTH_MODE,
	GET_WPA_ENC_MODE,
	SET_WPA_ENC_MODE, 
	GET_WPA_AUTH_MODE,
	SET_WPA_AUTH_MODE,
	GET_SUPPORTED_FREQUENCY_BANDS,
	GET_PRE_SHARED_KEY,
	SET_PRE_SHARED_KEY,
	GET_AP_SECURITY_MODE_ENABLED,
	IF_CONFIG_UP,
	IF_CONFIG_DOWN,
	GET_BASE_BSSID,
	GET_STATUS,
	GET_MAX_BIT_RATE,
	GET_STANDARD,
	GET_WMM_ENABLE,
	NOTIFY,
	GET_CHANNEL_MODE,
	SET_CHANNEL_MODE,
	GET_POSSIBLE_CHANNELS,
	ADD_ACL_DEV,
	DEL_ACL_DEV,
	GET_ACL_DEV_NUM,
	SET_MAC_ADD_CONTROL_MODE,
	GET_WIFI_ENABLE_STATUS_BOOL,
	GET_WPA_REKEY_INTERVAL,
	SET_WPA_REKEY_INTERVAL,
	GET_INDEX_FROM_NAME,
	GET_ASSOC_DEVICES_NUM,
	SET_AP_BRIDGING,
	GET_RADIO_INDEX,
	GET_RADIO_ACTIVE,
	GET_WPS_ENABLE,
	SET_WPS_ENABLE,
	GET_WPS_DEVICE_PIN,
	SET_WPS_DEVICE_PIN,
	GET_WPS_CONFIG_METHOD_ENABLED,
	SET_WPS_CONFIG_METHOD_ENABLED,
	GET_WPS_CONFIGURATION_STATE,
	SET_WPS_PBC_TRIGGER,
	CANCEL_WPS,
	GET_MAX_STATIONS,
	SET_MAX_STATIONS,
	KICK_ACL_ASSOCIATED_DEVICES,
	GET_AP_WMM_CAPABILITY,
	GET_AP_UAPSD_CAPABILITY,
	GET_AP_ISOLATION_ENABLE,
	SET_AP_ISOLATION_ENABLE,
	GET_AP_WMM_UAPSD_ENABLE,
	SET_AP_WMM_UAPSD_ENABLE,
	GET_AP_ACL_DEVICES,
	GET_AP_SECURITY_RADIUS_SERVER_IP,
	SET_AP_SECURITY_RADIUS_SERVER_IP,
	GET_AP_SECURITY_RADIUS_SERVER_PORT,
	SET_AP_SECURITY_RADIUS_SERVER_PORT,
	GET_AP_SECURITY_RADIUS_SERVER,
	SET_AP_SECURITY_RADIUS_SERVER,
	GET_RADIUS_SECRET,
	SET_RADIUS_SECRET,
	GET_WEP_KEY,
	SET_WEP_KEY,
	GET_WEP_KEY_INDEX,
	SET_WEP_KEY_INDEX,
	START_MONITORING,
	STATION_STEER_TR181,
	STATION_STEER,
	STATION_ALLOW_TR181,
	STATION_ALLOW,
	CHANNEL_SWITCH_TR181,
	CHANNEL_SWITCH,
	UNASSOCIATED_DEVICES_INFO_REQ_TR181,
	UNASSOCIATED_DEVICES_INFO_REQ,
	RESTRICTED_CHANNEL_SET_TR181,
	RESTRICTED_CHANNEL_SET,
	FAILSAFE_CHANNEL_SET_TR181,
	FAILSAFE_CHANNEL_SET,
	STATION_MEASUREMENT_GET,
	VAP_MEASUREMENT_GET,
	RADIO_INFO_GET,
	ACS_REPORT_GET,
	FAILSAFE_CHANNEL_GET,
	RESTRICTED_CHANNEL_GET,
	WLAN_DOWN,
	FACTORY_RESET_RADIOS,
	GET_TRANSMIT_POWER,
	SET_TRANSMIT_POWER,
	SET_STBC_ENABLE,
	GET_AMSDU_ENABLE,
	SET_AMSDU_ENABLE,
	SET_SHORT_GUARD_INTERVAL,
	GET_TX_CHAIN_MASK,
	SET_TX_CHAIN_MASK,
	GET_RX_CHAIN_MASK,
	SET_RX_CHAIN_MASK,
	GET_RADIO_IEEE_ENABLED,
	GET_RADIO_IEEE_SUPPORTED,
	GET_RADIO_AUTO_CH_REFRESH_PERIOD_SUPPORTED,
	GET_RADIO_AUTO_CH_REFRESH_PERIOD,
	GET_RADIO_DCS_SUPPORTED,
	GET_RADIO_DCS_ENABLE,
	SET_RADIO_DCS_ENABLE,
	SET_WMM_ENABLE,
	SET_WMM_UAPSD_ENABLE,
	SET_WMM_OG_AIFSN,
	GET_WMM_OG_AIFSN,
	SET_WMM_OG_ECW_MIN,
	GET_WMM_OG_ECW_MIN,
	SET_WMM_OG_ECW_MAX,
	GET_WMM_OG_ECW_MAX,
	SET_WMM_OG_TX_OP,
	GET_WMM_OG_TX_OP,
	SET_WMM_IC_AIFSN,
	GET_WMM_IC_AIFSN,
	SET_WMM_IC_ECW_MIN,
	GET_WMM_IC_ECW_MIN,
	SET_WMM_IC_ECW_MAX,
	GET_WMM_IC_ECW_MAX,
	SET_WMM_IC_TX_OP,
	GET_WMM_IC_TX_OP,
	SET_BEACON_INTERVAL,
	GET_RADIO_BASIC_DATA_TRANSMIT_RATES,
	SET_RADIO_BASIC_DATA_TRANSMIT_RATES,
	GET_RADIO_BEACON_PERIOD,
	SET_RADIO_BEACON_PERIOD,
	GET_RADIO_GUARD_INTERVAL,
	SET_RADIO_GUARD_INTERVAL,
	GET_RADIO_UP_TIME,
	GET_AP_RETRY_LIMIT,
	SET_AP_RETRY_LIMIT,
	GET_BRIDGE_NAME,
	SET_BRIDGE_NAME,
	SET_WPS_ENROLLEE_PIN,
	GET_COUNTRY_CODE,
	SET_COUNTRY_CODE,
	GET_NUMBER_OF_RADIOS,
	SET_ACCELERATION,
	GET_ACCELERATION,
	GET_WPS_STATUS,
	SET_RADIO_ATF_ENABLE,
	GET_RADIO_ATF_ENABLE,
	SET_RADIO_ATF_DIST_TYPE,
	GET_RADIO_ATF_DIST_TYPE,
	SET_RADIO_ATF_ALGO_TYPE,
	GET_RADIO_ATF_ALGO_TYPE,
	SET_RADIO_ATF_INTERVAL,
	GET_RADIO_ATF_INTERVAL,
	SET_RADIO_ATF_FREE_TIME,
	GET_RADIO_ATF_FREE_TIME,
	SET_RADIO_ATF_VAP_ENABLED,
	GET_RADIO_ATF_VAP_ENABLED,
	SET_RADIO_ATF_STA_ENABLED,
	GET_RADIO_ATF_STA_ENABLED,
	SET_RADIO_ATF_RADIO_STAS_AND_WEIGHTS,
	GET_RADIO_ATF_RADIO_STAS_AND_WEIGHTS,
	SET_RADIO_ATF_VAP_WEIGHT,
	GET_RADIO_ATF_VAP_WEIGHT,
	SET_RADIO_ATF_VAP_STAS_AND_WEIGHTS,
	GET_RADIO_ATF_VAP_STAS_AND_WEIGHTS,
	GET_WDS_ENABLED,
	SET_WDS_ENABLED,
	GET_WDS_SECURITY_MODE,
	SET_WDS_SECURITY_MODE,
	GET_WDS_KEY,
	SET_WDS_KEY,
	GET_WDS_PEERS,
	SET_WDS_PEERS,
	GET_UDMA_ENABLED,
    SET_UDMA_ENABLED,
    GET_VLAN_ID,
    SET_VLAN_ID,
    GET_BSSID_PORT,
	FACTORY_COMPLETE,
    CLEAR_WAVE_NVRAM,
	GET_WDS_MODE,
	SET_WDS_MODE,
	APPLY_ALL,
	GET_BF_MODE,
	SET_BF_MODE,
	VERSION
};

typedef int(*FapiWlanGenericSetNativeFunc)(int index, ObjList *wlObj, unsigned int flags);

typedef struct
{
	char *key;
	int   val;
} lookupstruct_t;

static lookupstruct_t lookuptable[] = {
	{ "load" , LOAD },
	{ "init" , INIT },
	{ "unInit" , UNINIT },
	{ "reset" , RESET },
	{ "disableRPC" , DISABLE_RPC_MODE },	
	{ "createConfig" , CREATE_CONFIG },
	{ "factory" , FACTORY_DEAFULTS },
	{ "createVap" , CREATE_VAP },
	{ "deleteVap" , DELETE_VAP },
	{ "setRadioTR181" , SET_RADIO_TR181 },
	{ "setSsidTR181" ,  SET_SSID_TR181 },
	{ "setApTR181" ,  SET_AP_TR181 },
	{ "setSecurityTR181" ,  SET_SECURITY_TR181 },
	{ "setWpsTR181" , SET_WPS_TR181 },
	{ "setWmmStationTR181" , SET_WMM_STA_TR181 },
	{ "setWmmApTR181" , SET_WMM_AP_TR181 },
	{ "setWdsTR181" , SET_WDS_TR181 },
	{ "setHotspotTR181" , SET_HOTSPOT_TR181 },
	{ "getInterfaceName" , GET_INTERFACE_NAME },
	{ "setChannel" , SET_CHANNEL},
	{ "getChannel" , GET_CHANNEL},
	{ "getAutoChannelEnable" , GET_AUTO_CHANNEL_ENABLE},
	{ "setAutoChannelEnable" , SET_AUTO_CHANNEL_ENABLE},
	{ "getRadioEnabled" , GET_RADIO_ENABLE},
	{ "setRadioEnabled" , SET_RADIO_ENABLE},
	{ "getEnable" , GET_ENABLE},
	{ "setEnable" , SET_ENABLE},
	{ "getSsid" , GET_SSID},
	{ "setSsid" , SET_SSID},
	{ "getSsidAdvertisementEnabled" , GET_SSID_ADVERTISEMENT_ENABLED},
	{ "setSsidAdvertisementEnabled" , SET_SSID_ADVERTISEMENT_ENABLED},	
	{ "getMacControlEnabled" , GET_MAC_ADD_CONTROL_ENABLED},
	{ "setMacControlEnabled" , SET_MAC_ADD_CONTROL_ENABLED},
	{ "getBeaconType" , GET_BEACON_TYPE},
	{ "setBeaconType" , SET_BEACON_TYPE},
	{ "getKeyPassphrase" , GET_KEY_PASS_PHRASE},
	{ "setKeyPassphrase" , SET_KEY_PASS_PHRASE},
	{ "getWepKeyPassphrase" , GET_WEP_KEY_PASSPHRASE},
	{ "setWepKeyPassphrase" , SET_WEP_KEY_PASSPHRASE},
	{ "getEncMode", GET_BASIC_ENC_MODE},
	{ "setEncMode", SET_BASIC_ENC_MODE}, 
	{ "getAuthMode", GET_BASIC_AUTH_MODE},
	{ "setAuthMode", SET_BASIC_AUTH_MODE},
	{ "getWpaEncMode" , GET_WPA_ENC_MODE},
	{ "setWpaEncMode" , SET_WPA_ENC_MODE},
	{ "getWpaAuthMode" , GET_WPA_AUTH_MODE},
	{ "setWpaAuthMode" , SET_WPA_AUTH_MODE},
	{ "getSupportedFrequencyBands" , GET_SUPPORTED_FREQUENCY_BANDS},
	{ "getPreSharedKey" , GET_PRE_SHARED_KEY},
	{ "setPreSharedKey" , SET_PRE_SHARED_KEY},
	{ "getApSecurityModeEnabled" , GET_AP_SECURITY_MODE_ENABLED},
	{ "ifconfigUp" , IF_CONFIG_UP},
	{ "ifconfigDown" , IF_CONFIG_DOWN},
	{ "getBaseBssid" , GET_BASE_BSSID},
	{ "getStatus" , GET_STATUS},
	{ "getMaxBitRate" , GET_MAX_BIT_RATE},
	{ "getStandard" , GET_STANDARD},
	{ "getWmmEnable" , GET_WMM_ENABLE},
	{ "notify" , NOTIFY},
	{ "getChannelMode" , GET_CHANNEL_MODE},
	{ "setChannelMode" , SET_CHANNEL_MODE},
	{ "getPossibleChannels" , GET_POSSIBLE_CHANNELS},
	{ "addAclDevice" , ADD_ACL_DEV},
	{ "delAclDevice" , DEL_ACL_DEV},
	{ "getAclDeviceNum" , GET_ACL_DEV_NUM},
	{ "setMacAddControlMode" , SET_MAC_ADD_CONTROL_MODE},
	{ "getWifiEnableStatusBool" , GET_WIFI_ENABLE_STATUS_BOOL},
	{ "getWpaRekeyInterval" , GET_WPA_REKEY_INTERVAL},
	{ "setWpaRekeyInterval" , SET_WPA_REKEY_INTERVAL},
	{ "getIndexFromName" , GET_INDEX_FROM_NAME},
	{ "getAssocDevicesNum" , GET_ASSOC_DEVICES_NUM},
	{ "setApBridging" , SET_AP_BRIDGING},
	{ "getRadioIndex" , GET_RADIO_INDEX},
	{ "getRadioActive" , GET_RADIO_ACTIVE},
	{ "getWpsEnable" , GET_WPS_ENABLE},
	{ "setWpsEnable" , SET_WPS_ENABLE},
	{ "getWpsDevicePIN" , GET_WPS_DEVICE_PIN},
	{ "setWpsDevicePIN" , SET_WPS_DEVICE_PIN},
	{ "getWpsConfigMethodsEnabled" , GET_WPS_CONFIG_METHOD_ENABLED},
	{ "setWpsConfigMethodsEnabled" , SET_WPS_CONFIG_METHOD_ENABLED},
	{ "getWpsConfigurationState" , GET_WPS_CONFIGURATION_STATE},
	{ "setWpsPbcTrigger" , SET_WPS_PBC_TRIGGER},
	{ "cancelWps" , CANCEL_WPS},
	{ "getMaxStations" , GET_MAX_STATIONS},
	{ "setMaxStations" , SET_MAX_STATIONS},
	{ "kickAclAssociatedDevices" , KICK_ACL_ASSOCIATED_DEVICES},
	{ "getApWMMCapability" , GET_AP_WMM_CAPABILITY},
	{ "getApUAPSDCapability" , GET_AP_UAPSD_CAPABILITY},
	{ "getApIsolationEnable" , GET_AP_ISOLATION_ENABLE},
	{ "setApIsolationEnable" , SET_AP_ISOLATION_ENABLE},	
	{ "getApWmmUapsdEnable" , GET_AP_WMM_UAPSD_ENABLE},
	{ "setApWmmUapsdEnable" , SET_AP_WMM_UAPSD_ENABLE},
	{ "getApAclDevices" , GET_AP_ACL_DEVICES},
	{ "getApSecurityRadiusServerIP" , GET_AP_SECURITY_RADIUS_SERVER_IP},
	{ "setApSecurityRadiusServerIP" , SET_AP_SECURITY_RADIUS_SERVER_IP},
	{ "getApSecurityRadiusServerPort" , GET_AP_SECURITY_RADIUS_SERVER_PORT},
	{ "setApSecurityRadiusServerPort" , SET_AP_SECURITY_RADIUS_SERVER_PORT},
	{ "getApSecurityRadiusServer" , GET_AP_SECURITY_RADIUS_SERVER},
	{ "setApSecurityRadiusServer" , SET_AP_SECURITY_RADIUS_SERVER},
	{ "getRadiusSecret" , GET_RADIUS_SECRET},
	{ "setRadiusSecret" , SET_RADIUS_SECRET},
	{ "getWepKey" , GET_WEP_KEY},
	{ "setWepKey" , SET_WEP_KEY},
	{ "getWepKeyIndex" , GET_WEP_KEY_INDEX},
	{ "setWepKeyIndex" , SET_WEP_KEY_INDEX},
	{ "startMonitoring", START_MONITORING },
	{ "stationSteerTR181", STATION_STEER_TR181 },
	{ "stationSteer", STATION_STEER },
	{ "stationAllowTR181", STATION_ALLOW_TR181 },
	{ "stationAllow", STATION_ALLOW },
	{ "channelSwitchTR181", CHANNEL_SWITCH_TR181 },
	{ "channelSwitch", CHANNEL_SWITCH },
	{ "unassociatedDevicesInfoReqTR181", UNASSOCIATED_DEVICES_INFO_REQ_TR181 },
	{ "unassociatedDevicesInfoReq", UNASSOCIATED_DEVICES_INFO_REQ },
	{ "restrictedChannelSetTR181", RESTRICTED_CHANNEL_SET_TR181 },
	{ "restrictedChannelSet", RESTRICTED_CHANNEL_SET },
	{ "failsafeChannelSetTR181", FAILSAFE_CHANNEL_SET_TR181 },
	{ "failsafeChannelSet", FAILSAFE_CHANNEL_SET },
	{ "stationMeasurementGet", STATION_MEASUREMENT_GET },
	{ "VapMeasurementGet", VAP_MEASUREMENT_GET },
	{ "radioInfoGet", RADIO_INFO_GET },
	{ "acsReportGet", ACS_REPORT_GET },
	{ "failsafeChannelGet", FAILSAFE_CHANNEL_GET },
	{ "restrictedChannelGet", RESTRICTED_CHANNEL_GET },
	{ "wlanDown" , WLAN_DOWN},
	{ "factoryResetRadios" , FACTORY_RESET_RADIOS},
	{ "getTransmitPower" , GET_TRANSMIT_POWER},
	{ "setTransmitPower" , SET_TRANSMIT_POWER}, 
	{ "setSTBCEnable" , SET_STBC_ENABLE},
	{ "getAMSDUEnable" , GET_AMSDU_ENABLE}, 
	{ "setAMSDUEnable" , SET_AMSDU_ENABLE},
	{ "setShortGuardInterval" , SET_SHORT_GUARD_INTERVAL}, 
	{ "getTxChainMask" , GET_TX_CHAIN_MASK},
	{ "setTxChainMask" , SET_TX_CHAIN_MASK},
	{ "getRxChainMask" , GET_RX_CHAIN_MASK},
	{ "setRxChainMask" , SET_RX_CHAIN_MASK},
	{ "getRadioIEEEEnabled" , GET_RADIO_IEEE_ENABLED},
	{ "getRadioIEEESupported" , GET_RADIO_IEEE_SUPPORTED},
	{ "getRadioAutoChRefreshPeriodSupported" , GET_RADIO_AUTO_CH_REFRESH_PERIOD_SUPPORTED},
	{ "getRadioAutoChRefreshPeriod" , GET_RADIO_AUTO_CH_REFRESH_PERIOD},
	{ "getRadioDCSSupported" , GET_RADIO_DCS_SUPPORTED},
	{ "getRadioDCSEnable" , GET_RADIO_DCS_ENABLE},
	{ "setRadioDCSEnable" , SET_RADIO_DCS_ENABLE},
	{ "setWmmEnable" , SET_WMM_ENABLE},
	{ "setWmmUapsdEnable" , SET_WMM_UAPSD_ENABLE},
	{ "setWmmOgAifsn" , SET_WMM_OG_AIFSN},
	{ "getWmmOgAifsn" , GET_WMM_OG_AIFSN},
	{ "setWmmOgEcwMin" , SET_WMM_OG_ECW_MIN},
	{ "getWmmOgEcwMin" , GET_WMM_OG_ECW_MIN},
	{ "setWmmOgEcwMax" , SET_WMM_OG_ECW_MAX},
	{ "getWmmOgEcwMax" , GET_WMM_OG_ECW_MAX},
	{ "setWmmOgTxOp" , SET_WMM_OG_TX_OP},
	{ "getWmmOgTxOp" , GET_WMM_OG_TX_OP},
	{ "setWmmIcAifsn" , SET_WMM_IC_AIFSN},
	{ "getWmmIcAifsn" , GET_WMM_IC_AIFSN},
	{ "setWmmIcEcwMin" , SET_WMM_IC_ECW_MIN},
	{ "getWmmIcEcwMin" , GET_WMM_IC_ECW_MIN},
	{ "setWmmIcEcwMax" , SET_WMM_IC_ECW_MAX},
	{ "getWmmIcEcwMax" , GET_WMM_IC_ECW_MAX},
	{ "setWmmIcTxOp" , SET_WMM_IC_TX_OP},
	{ "getWmmIcTxOp" , GET_WMM_IC_TX_OP},
	{ "setBeaconInterval" , SET_BEACON_INTERVAL},
	{ "getRadioBasicDataTransmitRates" , GET_RADIO_BASIC_DATA_TRANSMIT_RATES},
	{ "setRadioBasicDataTransmitRates" , SET_RADIO_BASIC_DATA_TRANSMIT_RATES},
	{ "getRadioBeaconPeriod" , GET_RADIO_BEACON_PERIOD},
	{ "setRadioBeaconPeriod" , SET_RADIO_BEACON_PERIOD},
	{ "getRadioGuardInterval" , GET_RADIO_GUARD_INTERVAL},
	{ "setRadioGuardInterval" , SET_RADIO_GUARD_INTERVAL},
	{ "getRadioUptime" , GET_RADIO_UP_TIME},
	{ "getApRetryLimit" , GET_AP_RETRY_LIMIT},
	{ "setApRetryLimit" , SET_AP_RETRY_LIMIT},
	{ "getBridgeName" , GET_BRIDGE_NAME},
	{ "setBridgeName" , SET_BRIDGE_NAME},
	{ "setWpsEnrolleePin" , SET_WPS_ENROLLEE_PIN},
	{ "getCountryCode" , GET_COUNTRY_CODE},
	{ "setCountryCode" , SET_COUNTRY_CODE},
	{ "getNumberOfRadios" ,GET_NUMBER_OF_RADIOS},
	{ "setAcceleration" ,SET_ACCELERATION},
	{ "getAcceleration" ,GET_ACCELERATION},
	{ "getWpsStatus" , GET_WPS_STATUS},
	{ "getWpsStatus" , GET_WPS_STATUS},
	{ "setAtfEnable" , SET_RADIO_ATF_ENABLE},
	{ "getAtfEnable" , GET_RADIO_ATF_ENABLE},
	{ "setAtfDistributionType" , SET_RADIO_ATF_DIST_TYPE},
	{ "getAtfDistributionType" , GET_RADIO_ATF_DIST_TYPE},
	{ "setAtfAlgoType" , SET_RADIO_ATF_ALGO_TYPE},
	{ "getAtfAlgoType" , GET_RADIO_ATF_ALGO_TYPE},
	{ "setAtfInterval" , SET_RADIO_ATF_INTERVAL},
	{ "getAtfInterval" , GET_RADIO_ATF_INTERVAL},
	{ "setAtfFreeTime" , SET_RADIO_ATF_FREE_TIME},
	{ "getAtfFreeTime" , GET_RADIO_ATF_FREE_TIME},
	{ "setAtfVapEnabled" , SET_RADIO_ATF_VAP_ENABLED},
	{ "getAtfVapEnabled" , GET_RADIO_ATF_VAP_ENABLED},
	{ "setAtfStationEnabled" , SET_RADIO_ATF_STA_ENABLED},
	{ "getAtfStationEnabled" , GET_RADIO_ATF_STA_ENABLED},
	{ "setAtfRadioStationsAndWeights" , SET_RADIO_ATF_RADIO_STAS_AND_WEIGHTS},
	{ "getAtfRadioStationsAndWeights" , GET_RADIO_ATF_RADIO_STAS_AND_WEIGHTS},
	{ "setAtfVapWeight" , SET_RADIO_ATF_VAP_WEIGHT},
	{ "getAtfVapWeight" , GET_RADIO_ATF_VAP_WEIGHT},
	{ "setAtfVapStationsAndWeights" , SET_RADIO_ATF_VAP_STAS_AND_WEIGHTS},
	{ "getAtfVapStationsAndWeights" , GET_RADIO_ATF_VAP_STAS_AND_WEIGHTS},
	{ "getWdsEnabled" , GET_WDS_ENABLED},
	{ "setWdsEnabled" , SET_WDS_ENABLED},
	{ "getWdsSecurityMode" , GET_WDS_SECURITY_MODE},
	{ "setWdsSecurityMode" , SET_WDS_SECURITY_MODE},
	{ "getWdsKey" , GET_WDS_KEY},
	{ "setWdsKey" , SET_WDS_KEY},
	{ "getWdsPeers" , GET_WDS_PEERS},
	{ "setWdsPeers" , SET_WDS_PEERS},
	{ "getUdmaEnabled" , GET_UDMA_ENABLED},
	{ "setUdmaEnabled" , SET_UDMA_ENABLED},
	{ "getVlanId" , GET_VLAN_ID},
	{ "setVlanId" , SET_VLAN_ID},
	{ "getBssIdPort" , GET_BSSID_PORT},
	{ "factoryReboot" , FACTORY_COMPLETE},
	{ "clearWaveNvram" , CLEAR_WAVE_NVRAM},
	{ "getWdsMode" , GET_WDS_MODE},
	{ "setWdsMode" , SET_WDS_MODE},
	{ "apply" , APPLY_ALL},
	{ "version" , VERSION},
	{ "getBfMode" , GET_BF_MODE},
	{ "setBfMode" , SET_BF_MODE}
};

#define NKEYS ((int)(sizeof(lookuptable)/sizeof(lookupstruct_t)))

static int fapi_wlan_generic_set_native(FapiWlanGenericSetNativeFunc fapiWlanGenericSetNativeFunc,
	int index,
	ObjList *wlObj,
	char *dbCliFileName,
	unsigned int flags)
{
	if (wlanLoadFromDB(dbCliFileName, "", wlObj) == UGW_SUCCESS)
	{
		setLog("wlan0", wlObj, 0);
		if (fapiWlanGenericSetNativeFunc(index, wlObj, flags) == UGW_SUCCESS)
		{
			return UGW_SUCCESS;
		}
		else
		{
			printf("FAPI_WLAN_CLI, fapiWlanGenericSetNativeFunc return with error\n");
			return UGW_FAILURE;
		}
	}
	else
	{
		printf("FAPI_WLAN_CLI, wlanLoadFromDB return with error\n");
		return UGW_FAILURE;
	}
}

static int keyFromString(char *key)
{
    int i;

    for (i=0; i < NKEYS; i++)
    {
        if (strcmp(lookuptable[i].key, key) == 0)
	{
            return lookuptable[i].val;
	}
    }

	return UGW_FAILURE;
}

static int printGetopts(void)
{
	int i;

	printf("\tNAME\n \
		\t\tfapi_wlan_cli - interface to FAPI API\n \
		\tSYNOPSIS\n \
		\t\tfapi_wlan_cli[API]...[OPTION]...\n \
		\tDESCRIPTION\n \
		\t\tList  information  about  the  options  for  FAPI  CLI  APIs\n \
		\t\t - i <interface>\n \
		\t\t\tinterface index for APIs that requires it\n \
		\t\t - f <file name>\n \
		\t\t\toptional option, default name is input_cli\n \
		\t\t\tfor example in YOCTO, file should be copied to : / nvarm / etc / wave / db / instance / \n \
		\t\t - v <vap index>\n \
		\t\t\tvap index for createVap and deleteVap APIs\n \
		\t\t - s <ssid>\n \
		\t\t\tssid for createVap\n \
		\t\t - c <channel>\n \
		\t\t\tchannel for setChannel API\n \
		\t\t - e <enable>\n \
		\t\t\tenable for setAutoChannelEnable and setRadioEnabled APIs\n \
		\t\t - p <parameter>\n \
		\t\t\tparameter\n");

	printf("\tcurrently the following APIs are supported:\n");
	for (i = 0; i < NKEYS; i++)
	{
		printf("\t\t%s\n", lookuptable[i].key);
	}
	printf("\tfor more information: you can write -h <api_name>\n");

	return 0;
}

static int printHelpGetopts(char *api)
{

	switch (keyFromString(api)) {
	case LOAD:
	case INIT:
	case FACTORY_DEAFULTS:
	case UNINIT:
		printf("\tAPI doesn't require arguments");
		break;

	case CREATE_VAP:
		printf("\targuments are:\n \
			\t - i <interface>, -v <vap index>, -s <ssid>\n \
			\tvap index should be unique, not used before.\n \
			\tfor example : fapi_wlan_cli createVap - i 0 - v 5 - s test_ssid \n");
		break;

	case DELETE_VAP:
		printf("\targuments are:\n \
			\t - v <vap index>\n \
			\tfor example : fapi_wlan_cli deleteVap - v 5 \n");
		break;

	case SET_RADIO_TR181:
	case SET_SSID_TR181:
	case SET_AP_TR181:
	case SET_SECURITY_TR181:
	case SET_WDS_TR181:
	case SET_WPS_TR181:
	case SET_WMM_STA_TR181:
	case SET_WMM_AP_TR181:
	case SET_HOTSPOT_TR181:
		printf("\targuments are:\n \
			\t - i <interface>, -f <file name>\n \
			\t - f is an optional option, default name is input_cli\n \
			\tExample\n \
			\t\techo Object_0=Device.WiFi.SSID > /opt/lantiq/wave/db/instance/input_cli \n \
			\t\techo SSID_0=NEW_SSID >> /opt/lantiq/wave/db/instance/input_cli \n \
			\t\tfapi_wlan_cli setSsidTR181 -i0 \n \			
			\t\tfapi_wlan_cli ifconfigUp -i0 \n");
		break;

	case GET_INTERFACE_NAME:
		printf("\targument is:\n \
			\t - i <interface>\n \
			\tfor example : fapi_wlan_cli getInterfaceName - i 0 \n");
		break;
	
	case GET_BRIDGE_NAME:
		printf("\targument is:\n \
			\t - i <interface>\n \
			\tfor example : fapi_wlan_cli getBridgeName - i 0 \n");
		break;
	case SET_BRIDGE_NAME:
		printf("\targuments are:\n \
			\t - i <interface>\n \
			\t - p <parameter - bridgeName>\n \
			\tfor example : fapi_wlan_cli setBridgeName - i 0 - p Br-lan \n");
		break;
	case SET_CHANNEL:
		printf("\targuments are:\n \
			\t - i <interface>\n \
			\t - c <channel>\n \
			\tfor example : fapi_wlan_cli setChannel - i 0 - c 6\n");
		break;
	
	case SET_AUTO_CHANNEL_ENABLE:
	case SET_RADIO_ENABLE:
		printf("\targuments are:\n \
		\t - i <interface>\n \
		\t - e <enable_bool>\n \
		\tfor example : fapi_wlan_cli setAutoChannelEnable - i 0 - e false\n");
		break;
	
	case SET_SSID:
		printf("\targument is:\n \
			\t - s <ssid>\n \
			\tfor example : fapi_wlan_cli setSsid - s \"ssid_test_1\" \n");
		break;
		
	case SET_BEACON_TYPE:
		printf("\targuments are:\n \
			\t - i <interface>\n \
			\t - p <parameter - beaconTypeString>\n \
			\tfor example : fapi_wlan_cli setBeaconType - i 0 - p Basic \n"); //should change to beacon type
		break;
			
	case GET_CHANNEL:
	case GET_KEY_PASS_PHRASE:
	case GET_AP_SECURITY_MODE_ENABLED:
	case GET_AUTO_CHANNEL_ENABLE:
	case GET_RADIO_ENABLE:
	case GET_SSID:
	case GET_PRE_SHARED_KEY:
	case GET_WPA_ENC_MODE:
	case GET_SUPPORTED_FREQUENCY_BANDS:
	case GET_BEACON_TYPE:
		printf("\tget methods, argument is:\n \
		\t - i <interface>\n \
		\tfor example : fapi_wlan_cli getChannel - i 0 \n");
		break;
		
	default:
		printGetopts();
		break;
	}

	return 0;

}

static int changeCharToBoolean(char *str, bool *enable_bool)
{
	
	if (!strcmp(str, "true"))
	{
		*enable_bool = true;
		//printf("enable bool is true\n");
	}
	else if (!strcmp(str, "false"))
	{
		*enable_bool = false;
		//printf("enable bool is false\n");
	}
	else
	{
		return UGW_FAILURE;
	}
	printf("enable value is %s\n", *enable_bool?"true":"false");
	return 0;
}

static int changeBooleanToChar(char **str, bool *enable_bool)
{
	
	if (*enable_bool == true)
	{
		*str = "true";
	}
	else if (*enable_bool == false)
	{
		*str = "false";
	}
	else
	{
		return UGW_FAILURE;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	char *apiName = NULL;
	ObjList *dbObjPtr = NULL;

	int index = -1, vap_index = -1;
	char *ssid = NULL;
	char *name = NULL;
	char *dbCliFileName = "input_cli";
	int retVal = UGW_FAILURE;
	char interfaceName[MAX_LEN_PARAM_VALUE] = {'\0'};
	unsigned int channel = 0;
	unsigned int numberOfRadios = 0;
	
	char *enable = NULL;
	bool enable_bool = NULL;

	char *mode = NULL;
	bool mode_bool = NULL;

	char output_string[MAX_LEN_PARAM_VALUE];
	char *parameter = NULL;
	char *parameter1 = NULL;
	char *parameter2 = NULL;
	char *parameter3 = NULL;
	char *output = NULL;
	unsigned int output_int;
	unsigned int output_int2;
	unsigned long output_long; 
	int acl_mode = -1;
	char ***macArray = NULL;
	unsigned int buf_size;
	int i = 0;
	
	dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	
	int option = 0;
	extern char *optarg;

	if (argv[1] != NULL)
	{
		apiName = argv[1];
	}
	else
	{
		printf("%s: ERROR - NO API ENTERED!\n", __FUNCTION__);
		printGetopts();
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);  // free memory
		return UGW_FAILURE;
	}
	//Specifying the expected options
	//The two options i and v expect numbers as argument
	while ((option = getopt(argc, argv, "i:v:n:s:f:h:c:e:p:q:r:t:m:o:")) != -1) {
		switch (option) {
		case 'i':
			index = atoi(optarg);
			break;
		case 'v':
			vap_index = atoi(optarg);
			break;
		case 'm':
			mode = optarg;
			break;
		case 's':
			ssid = optarg;
			break;
		case 'n':
			name = optarg;
			break;
		case 'f':
			dbCliFileName = optarg;
			break;
		case 'h':
			printHelpGetopts(optarg);
			break;
		case 'c':
			channel = atoi(optarg);
			break;
		case 'e':
			enable = optarg;
			break;
		case 'p':
			parameter = optarg;
			break;
		case 'q':
			parameter1 = optarg;
			break;
		case 'r':
			parameter2 = optarg;
			break;
		case 't':
			parameter3 = optarg;
			break;
		case 'o':
			output = optarg;
			break;

		default: //Unknown Argument
			return UGW_FAILURE;
		}
	}
	

	printf("%s Entry command = %s\n", __FUNCTION__ ,apiName);
	
	switch (keyFromString(apiName))
	{
		/* APIs that common to development */
		
		case LOAD:
			retVal = wlan_load();
			break;	

		case INIT:
			retVal = wlan_init();
			break;

		case APPLY_ALL:
			retVal = wlan_apply();
			break;

		case VERSION:
			retVal = wlan_version();
			break;
			
		case UNINIT:
			retVal = wlan_uninit();
			break;

	    case RESET:
			retVal = wlan_reset();
			break;
		
		case CREATE_CONFIG:
			retVal = wlan_createInitialConfigFiles(parameter);
			break;

		case DISABLE_RPC_MODE:
			retVal = wlan_disableRpcMode();
			break;
			
		case FACTORY_DEAFULTS:
			retVal = wlan_factoryReset();
			break;

		case CREATE_VAP:
			retVal = wlan_createVap(vap_index, index, ssid, 0);
			if (!retVal) {
			     retVal = wlan_ifconfigUp(vap_index);
			}
			break;
		
		case DELETE_VAP:
			retVal = wlan_deleteVap(index);
			break;	
		
		/* APIs for TR-181 */
			
		case SET_RADIO_TR181:
			retVal = fapi_wlan_generic_set_native(fapi_wlan_radio_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;

		case SET_SSID_TR181:
			retVal = fapi_wlan_generic_set_native(fapi_wlan_ssid_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;

		case SET_AP_TR181:
			retVal = fapi_wlan_generic_set_native(fapi_wlan_ap_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;

		case SET_SECURITY_TR181:
			retVal = fapi_wlan_generic_set_native(fapi_wlan_security_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;

		case SET_WPS_TR181:
			retVal = fapi_wlan_generic_set_native(fapi_wlan_wps_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;

		case SET_WMM_STA_TR181:
			retVal = fapi_wlan_generic_set_native(fapi_wlan_wmm_sta_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;
	
		case SET_WMM_AP_TR181:
			retVal = fapi_wlan_generic_set_native(fapi_wlan_wmm_ap_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;
			
		case SET_WDS_TR181:
			retVal = fapi_wlan_generic_set_native(fapi_wlan_wds_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;
		
		case NOTIFY:
			retVal = fapi_wlan_notify(name, dbObjPtr, dbCliFileName);
			break;
			
		case SET_HOTSPOT_TR181:
			retVal = fapi_wlan_generic_set_native(fapi_wlan_hotspot_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;

		/*case SET_ENDPOINT_AP:  /* set endpoint *\/
			retVal = fapi_wlan_generic_set_native(fapi_wlan_endpoint_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;*/

		/*case SET_WPS:  /* set wps *\/
			retVal = fapi_wlan_generic_set_native(fapi_wlan_wps_set_native, index, dbObjPtr, dbCliFileName, FLAG);
			break;*/

		case GET_INTERFACE_NAME:  /* get interface name */
			retVal = getInterfaceName(index, interfaceName);
			printf("cli_return=%s\n", interfaceName);
			break;

		case GET_CHANNEL:
			retVal = wlan_getChannel(index, &channel);
			printf("cli_return=%d\n", channel);
			break;
	
		case SET_CHANNEL:
			retVal = wlan_setChannel(index, channel);
			break;
	
		case GET_AUTO_CHANNEL_ENABLE:
			retVal = wlan_getAutoChannelEnable(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
	
		case SET_AUTO_CHANNEL_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setAutoChannelEnable(index, enable_bool);
			}
			break;
	
		case GET_RADIO_ENABLE:
			retVal = wlan_getRadioEnabled(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;		
	
		case SET_RADIO_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setRadioEnabled(index, enable_bool);
			}
			break;	
		
		case GET_ENABLE:
			retVal = wlan_getEnable(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;

		case SET_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setEnable(index, enable_bool);
			}
			break;

		case GET_NUMBER_OF_RADIOS:
			retVal = wlan_getNumberOfRadios(&numberOfRadios);
			printf("cli_return=%d\n", numberOfRadios);
			break;

		case GET_ACCELERATION:
			retVal = wlan_getAccelerationMode(index, &mode_bool);
			changeBooleanToChar(&mode, &mode_bool);
			printf("cli_return=%s\n", mode);
			break;

		case SET_ACCELERATION:
			if (changeCharToBoolean(mode, &mode_bool) != UGW_FAILURE)
			{
				retVal = wlan_setAccelerationMode(index, mode_bool);
			}
			break;

		case GET_SSID:
			retVal = wlan_getSSID(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;
	
		case SET_SSID:
			retVal = wlan_setSSID(index, ssid);
			break;
		
		case GET_SSID_ADVERTISEMENT_ENABLED:
			retVal = wlan_getSsidAdvertisementEnabled(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;

		case SET_SSID_ADVERTISEMENT_ENABLED:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setSsidAdvertisementEnabled(index, enable_bool);
			}
			break;
	
		case GET_MAC_ADD_CONTROL_ENABLED:
			retVal = wlan_getMacAddressControlEnabled(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;

		case SET_MAC_ADD_CONTROL_ENABLED:	
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setMacAddressControlEnabled(index, enable_bool);
			}
			break;

		case GET_BEACON_TYPE:
			retVal = wlan_getBeaconType(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;
		
		case SET_BEACON_TYPE:
			retVal = wlan_setBeaconType(index, parameter);
			break;

		case GET_KEY_PASS_PHRASE:
			retVal = wlan_getPassphrase(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_KEY_PASS_PHRASE:
			retVal = wlan_setKeyPassphrase(index, 0, parameter);
			break;

		case GET_WEP_KEY_PASSPHRASE:
			retVal = wlan_getWepKeyPassphrase(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_WEP_KEY_PASSPHRASE:
			retVal = wlan_setWepKeyPassphrase(index, parameter);
			break;
	
		case GET_BASIC_ENC_MODE:
			printf("get basic enc mode\n");
			retVal = wlan_getBasicEncryptionModes(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_BASIC_ENC_MODE:
			printf("encMode is %s\n", parameter);
			retVal = wlan_setBasicEncryptionModes(index, parameter);
			break;

		case GET_BASIC_AUTH_MODE:
			retVal = wlan_getBasicAuthenticationModes(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_BASIC_AUTH_MODE:
			printf("authMode is %s\n", parameter);
			retVal = wlan_setBasicAuthenticationModes(index, parameter);
			break;

		case GET_WPA_ENC_MODE:
			retVal = wlan_getWpaEncryptionModes(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_WPA_ENC_MODE:
			printf("encMode is %s\n", parameter);	
			retVal = wlan_setWpaEncryptionModes(index, parameter);
			break;

		case GET_WPA_AUTH_MODE:
			retVal = wlan_getWpaAuthenticationModes(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_WPA_AUTH_MODE:
			printf("authMode is %s\n", parameter);
			retVal = wlan_setWpaBasicAuthenticationModes(index, parameter);
			break;

		case GET_SUPPORTED_FREQUENCY_BANDS:
			retVal = wlan_getSupportedFrequencyBands(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;
	
		case GET_PRE_SHARED_KEY:
			retVal = wlan_getPreSharedKey(index, 0, output_string);
			printf("cli_return=%s\n", output_string);
			break;
			
		case SET_PRE_SHARED_KEY:
			retVal = wlan_setPreSharedKey(index, 0, parameter);
			break;

		case GET_AP_SECURITY_MODE_ENABLED:
			retVal = wlan_getApSecurityModeEnabled(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;
		
		case IF_CONFIG_UP:
			retVal = wlan_ifconfigUp(index);
			break;

		case IF_CONFIG_DOWN:
			retVal = wlan_ifconfigDown(index);
			break;

		case GET_BASE_BSSID:
			retVal = wlan_getBaseBSSID(index, output_string);	
			printf("cli_return=%s\n", output_string);		
			break;

		case GET_STATUS:
			retVal = wlan_getStatus(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case GET_MAX_BIT_RATE:
			retVal = wlan_getMaxBitRate(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;				
		
		case GET_STANDARD:
			retVal = wlan_getStandard(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case GET_WMM_ENABLE:
			retVal = wlan_getWmmEnable(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;

		case GET_CHANNEL_MODE:
			retVal = wlan_getChannelMode(index, output_string);	
			printf("cli_return=%s\n", output_string);		
			break;
			
		case SET_CHANNEL_MODE:
			retVal = wlan_setChannelMode(index, parameter, false, false, false);		
			break;
		
		case GET_POSSIBLE_CHANNELS:
			retVal = wlan_getPossibleChannels(index, output_string);	
			printf("cli_return=%s\n", output_string);		
			break;
			
		case ADD_ACL_DEV:
			printf("MAC Address %s will add\n", parameter);
			retVal = wlan_addAclDevice(index, parameter);
			break;
			
	    case DEL_ACL_DEV:
			printf("MAC Address %s will delete\n", parameter);
			retVal = wlan_delAclDevice(index, parameter);
			break;
			
	    case GET_ACL_DEV_NUM:
			retVal = wlan_getAclDeviceNum(index, &output_int);
			printf("cli_return=%d\n", output_int);
			break;
			
		case SET_MAC_ADD_CONTROL_MODE:
			printf("parameter is %s\n", parameter);
			if (!strcmp(parameter, "Disabled"))
			{
				acl_mode=0;
			}
			else if (!strcmp(parameter, "Allow")) {
				acl_mode=1;
			}
			else if (!strcmp(parameter, "Deny")) {
				acl_mode=2;
			}
			else
			{
				return UGW_FAILURE;
			}
			
			
			printf("acl_mode is %d\n", acl_mode);
			
			retVal = wlan_setMacAddressControlMode(index, acl_mode);
			break;
		
		case GET_WIFI_ENABLE_STATUS_BOOL:
			retVal = wlan_getWifiEnableStatusBool(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
			
		case GET_WPA_REKEY_INTERVAL:
			retVal = wlan_getWpaRekeyInterval(index, (int *)&output_int);
			printf("cli_return=%d\n", output_int);
			break;
			
		case SET_WPA_REKEY_INTERVAL:
			retVal = wlan_setWpaRekeyInterval(index, atoi(parameter));
			break;
			
		case GET_INDEX_FROM_NAME:
			retVal = wlan_getIndexFromName(parameter, (int *)&output_int);
			printf("cli_return=%d\n", output_int);
			break;
			
		case GET_ASSOC_DEVICES_NUM:
			retVal = wlan_getAssocDevicesNum(index, &output_int);
			printf("cli_return=%d\n", output_int);
			break;
		
		case SET_AP_BRIDGING:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setApBridging(index, enable_bool);
			}
			break;
		
		case GET_RADIO_INDEX:
			retVal = wlan_getRadioIndex(index, (int *)&output_int);
			printf("cli_return=%d\n", output_int);		
			break;
		
		case GET_RADIO_ACTIVE:
			retVal = wlan_getRadioActive(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
			
		case GET_WPS_ENABLE:
			retVal = wlan_getWpsEnable(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
			
		case SET_WPS_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setWpsEnable(index, enable_bool);
			}
			break;
			
		case GET_WPS_DEVICE_PIN:
			retVal = wlan_getWpsDevicePIN(index, &output_long);
			printf("cli_return=%lu\n", output_long);	
			break;
			
		case SET_WPS_DEVICE_PIN:
			retVal = wlan_setWpsDevicePIN(index, strtol(parameter,NULL,0));
			break;
		
		case GET_WPS_CONFIG_METHOD_ENABLED:
			retVal = wlan_getWpsConfigMethodsEnabled(index, output_string);	
			printf("cli_return=%s\n", output_string);	
			break;
			
		case SET_WPS_CONFIG_METHOD_ENABLED:
			retVal = wlan_setWpsConfigMethodsEnabled(index, parameter);
			break;
			
		case GET_WPS_CONFIGURATION_STATE:
			retVal = wlan_getWpsConfigurationState(index, output_string);	
			printf("cli_return=%s\n", output_string);	
			break;
			
		case SET_WPS_PBC_TRIGGER:
			retVal = wlan_setWpsPbcTrigger(index);
			break;
			
		case CANCEL_WPS:
			retVal = wlan_cancelWps(index);
			break;
			
		case SET_MAX_STATIONS:
			retVal = wlan_setMaxStations(index, atoi(parameter));
			break;
			
		case GET_MAX_STATIONS:
			retVal = wlan_getMaxStations(index, &output_int);
			printf("cli_return=%d\n", output_int);	
			break;
			
		case KICK_ACL_ASSOCIATED_DEVICES:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_kickAclAssociatedDevices(index, enable_bool);
			}
			break;
		
		case GET_AP_WMM_CAPABILITY:
			retVal = wlan_getApWMMCapability(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
			
		case GET_AP_UAPSD_CAPABILITY:
			retVal = wlan_getApUAPSDCapability(index,&enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
			
		case GET_AP_ISOLATION_ENABLE:
			retVal = wlan_getApIsolationEnable(index,&enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
		
		case SET_AP_ISOLATION_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setApIsolationEnable(index, enable_bool);
			}
			break;
			
		case GET_AP_WMM_UAPSD_ENABLE:
			retVal = wlan_getApWmmUapsdEnable(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
			
		case SET_AP_WMM_UAPSD_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setApWmmUapsdEnable(index, enable_bool);
			}
			
			break;
		
		case GET_AP_ACL_DEVICES:
			printf("esti_dbg_1\n");
			retVal = wlan_getApAclDevices(index, macArray, &buf_size);
			printf("esti_dbg_2\n");
			printf("Checking return values...  macArray:%p\n", macArray);
			for (i = 0; i < (int)buf_size; i++)
			{
				printf("*macArray[%d] = %s\n", i, *macArray[i]);
				free(macArray[i]);				
			}	
			free(macArray);	
			break;
			
		case GET_AP_SECURITY_RADIUS_SERVER_IP:
			retVal = wlan_getApSecurityRadiusServerIPAddr(index, output_string);	
			printf("cli_return=%s\n", output_string);	
			break;
			
		case SET_AP_SECURITY_RADIUS_SERVER_IP:
			retVal = wlan_setApSecurityRadiusServerIPAddr(index, parameter);
			break;
			
		case GET_AP_SECURITY_RADIUS_SERVER_PORT:
			retVal = wlan_getApSecurityRadiusServerPort(index, &output_int);
			printf("cli_return=%d\n", output_int);		
			break;
			
		case SET_AP_SECURITY_RADIUS_SERVER_PORT:
			retVal = wlan_setApSecurityRadiusServerPort(index, atoi(parameter));
			break;
			
		case GET_AP_SECURITY_RADIUS_SERVER:
			retVal = wlan_getApSecurityRadiusServer(index, output_string, &output_int);
			printf("cli_return_1=%s , cli_return_2=%d\n", output_string, output_int);
			break;
			
		case SET_AP_SECURITY_RADIUS_SERVER:
			retVal = wlan_setApSecurityRadiusServer(index, output, atoi(parameter));
			break;
		
		case SET_RADIUS_SECRET:
			retVal = wlan_setRadiusSecret(index, parameter);
			break;
		case GET_RADIUS_SECRET:
			retVal = wlan_getRadiusSecret(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;
		case GET_WEP_KEY:
			printf("Used default key Index 1 - TR181 allow only one password field\n");
			retVal = wlan_getWepKey(index, 1, output_string);
			printf("cli_return=%s\n", output_string);	
			break;
		
		case SET_WEP_KEY:
			printf("Used default key Index 1 - TR181 allow only one password field\n");
			retVal = wlan_setWepKey(index, 1, parameter);
			break;
		
		case GET_WEP_KEY_INDEX:
			printf("Used default key Index 1 - TR181 allow only one password field\n");
			retVal = wlan_getWepKeyIndex(index, &output_int);
			printf("cli_return=%d\n", output_int);
			break;
			
		case SET_WEP_KEY_INDEX:
			printf("Used default key Index 1 - TR181 allow only one password field\n");
			retVal = wlan_setWepKeyIndex(index, atoi(parameter));
			break;

		case START_MONITORING:
			retVal = fapi_wlan_start_monitoring_native(index, NULL, FLAG);
			//Format:  fapi_wlan_cli startMonitoring -i index
			//Example: fapi_wlan_cli startMonitoring -i 1
			break;

		case STATION_STEER_TR181:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_sta_steer_native(index, dbObjPtr, FLAG);
			break;

		case STATION_STEER:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = wlan_staSteer(index, parameter, parameter1);
			//Format:  fapi_wlan_cli stationSteer -i index MACAddress BSSID-Dest.
			//Example: fapi_wlan_cli stationSteer -i 1 -p 8c:3a:e3:45:d1:ae -q 00:0A:1B:0E:04:70
			break;

		case STATION_ALLOW_TR181:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_sta_allow_native(index, dbObjPtr, FLAG);
			break;

		case STATION_ALLOW:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = wlan_staAllow(index, parameter);
			//Format:  fapi_wlan_cli stationAllow -i index MACAddress
			//Example: fapi_wlan_cli stationAllow -i 0 -p 8c:3a:e3:45:d1:ae
			break;

		case CHANNEL_SWITCH_TR181:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_channel_switch_native(index, dbObjPtr, FLAG);
			break;

		case CHANNEL_SWITCH:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = wlan_channelSwitch(index, parameter, parameter1, parameter2);
			//Format:   fapi_wlan_cli channelSwitchTR181 -i index -p CenterFrequency -q WaveVhtCenterFrequency -r OperatingChannelBandwidth
			//Examples: set channel 149: fapi_wlan_cli channelSwitch -i 1 -p 5745 -q 5775 -r 80
			//set channel 36:  fapi_wlan_cli channelSwitch -i 1 -p 5180 -q 5210 -r 80
			break;

		case UNASSOCIATED_DEVICES_INFO_REQ_TR181:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_unassociated_devices_info_req_native(index, dbObjPtr, FLAG);
			break;

		case UNASSOCIATED_DEVICES_INFO_REQ:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = wlan_unassociatedDevicesInfoReq(index, parameter, parameter1, parameter2, parameter3);
			//Format:  fapi_wlan_cli unassociatedDevicesInfoReq -i index -p MACAddress -q OperatingChannelBandwidth -r CenterFrequency -t WaveVhtCenterFrequency
			//Example: fapi_wlan_cli unassociatedDevicesInfoReq -i 1 -p 8c:3a:e3:45:d1:ae -q 80 -r 5220 -t 5210
			break;

		case RESTRICTED_CHANNEL_SET_TR181:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_restricted_channels_set_native(index, dbObjPtr, FLAG);
			break;

		case RESTRICTED_CHANNEL_SET:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = wlan_restrictedChannelsSet(index, parameter);
			/*
			Format:   fapi_wlan_cli restrictedChannelSet -i index -p Channel Channel Channel-Channel Channel...
			Examples: Set channels 1, 6, 11, 12, 13:            fapi_wlan_cli restrictedChannelSet -i index 0 -p "6 11-13"
					  Set channels 7:                           fapi_wlan_cli restrictedChannelSet -i index 0 -p 7
					  Remove all channels from restricted list: fapi_wlan_cli restrictedChannelSet -i index 0
			*/
			break;

		case FAILSAFE_CHANNEL_SET_TR181:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_failsafe_channel_set_native(index, dbObjPtr, FLAG);
			break;

		case FAILSAFE_CHANNEL_SET:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = wlan_failsafeChannelSet(index, parameter, parameter1, parameter2);
			/*
			Format:   fapi_wlan_cli failsafeChannelSet -i index -p Channel -q CenterFrequency -r OperatingChannelBandwidth
			Examples: fapi_wlan_cli failsafeChannelSet -i 1 -p 5745 -q 5775 -r 80
					  fapi_wlan_cli failsafeChannelSet -i 1 -p 0
			*/
			break;

		case STATION_MEASUREMENT_GET:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_sta_measurement_get_native(index, dbObjPtr, FLAG);
			//Format:  fapi_wlan_cli stationMeasurementGet -i index -p MACAddress
			//Example: fapi_wlan_cli stationMeasurementGet -i 1 -p 8c:3a:e3:45:d1:ae
			break;

		case VAP_MEASUREMENT_GET:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_vap_measurement_get_native(index, dbObjPtr, FLAG);
			//Format:  fapi_wlan_cli VapMeasurementGet -i index
			//Example: fapi_wlan_cli VapMeasurementGet -i 1
			break;

		case RADIO_INFO_GET:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_radio_info_get_native(index, dbObjPtr, FLAG);
			//Format:  fapi_wlan_cli radioInfoGet -i index
			//Example: fapi_wlan_cli radioInfoGet -i 1
			break;

		case ACS_REPORT_GET:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_acs_report_get_native(index, dbObjPtr, FLAG);
			//Format:  fapi_wlan_cli acsReportGet -i index
			//Example: fapi_wlan_cli acsReportGet -i 1
			break;

		case FAILSAFE_CHANNEL_GET:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_failsafe_channel_get_native(index, dbObjPtr, FLAG);
			//Format:  fapi_wlan_cli failsafeChannelGet -i index
			//Example: fapi_wlan_cli failsafeChannelGet -i 1
			break;

		case RESTRICTED_CHANNEL_GET:
			if (! fapi_wlan_start_monitoring_native(index, NULL, FLAG) )
				retVal = fapi_wlan_restricted_channels_get_native(index, dbObjPtr, FLAG);
			//Format:  fapi_wlan_cli restrictedChannelGet -i index
			//Example: fapi_wlan_cli restrictedChannelGet -i 1
			break;

		case WLAN_DOWN:
			retVal = wlan_down();
			break;

		case FACTORY_RESET_RADIOS:
			retVal = wlan_factoryResetRadios();
			break;

		case FACTORY_COMPLETE:
			retVal = wlan_factoryComplete();
			break;
		case CLEAR_WAVE_NVRAM:
			retVal = wlan_clearWaveNvram();
			break;
		case GET_TRANSMIT_POWER:
			retVal = wlan_getTransmitPower(index, &output_int);
			printf("cli_return=%d\n", output_int);
			break;
				
		case GET_BSSID_PORT:
			retVal = wlan_getBssIdPort(parameter, &output_int, &output_int2);
			printf("cli_return=%d\n", output_int);
			printf("cli_return=%d\n", output_int2);
			break;	
			
		case SET_TRANSMIT_POWER:
			retVal = wlan_setTransmitPower(index, atoi(parameter));
			break;	

		case SET_STBC_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setSTBCEnable(index, enable_bool);
			}
			break;	
			
		case GET_AMSDU_ENABLE:
			retVal = wlan_getAMSDUEnable(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
			
		case SET_AMSDU_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setAMSDUEnable(index, enable_bool);
			}
			break;
			
		case SET_SHORT_GUARD_INTERVAL:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setShortGuardInterval(index, enable_bool);
			}
			break;
			
		case GET_TX_CHAIN_MASK:
			retVal = wlan_getTxChainMask(index, (int *)&output_int);
			printf("cli_return=%d\n", output_int);
			break;
			
		case SET_TX_CHAIN_MASK:
			retVal = wlan_setTxChainMask(index, atoi(parameter));
			break;
			
		case GET_RX_CHAIN_MASK:
			retVal = wlan_getRxChainMask(index, (int *)&output_int);
			printf("cli_return=%d\n", output_int);
			break;
			
		case SET_RX_CHAIN_MASK:
			retVal = wlan_setRxChainMask(index, atoi(parameter));
			break;
			
		case GET_RADIO_IEEE_ENABLED:
			retVal = wlan_getRadioIEEE80211hEnabled(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
			
		case GET_RADIO_IEEE_SUPPORTED:
			retVal = wlan_getRadioIEEE80211hSupported(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;
	
		case GET_RADIO_AUTO_CH_REFRESH_PERIOD:
			retVal = wlan_getRadioAutoChannelRefreshPeriod(index, &output_int);
			printf("cli_return=%d\n", output_int);
			break;
		
		case SET_WMM_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setWmmEnable(index,enable_bool);
			}
			break;
			 
		case SET_WMM_UAPSD_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setWmmUaspdEnable(index, enable_bool);
			}
			break;
			
		case SET_WMM_OG_AIFSN:
			retVal = wlan_setWmmOgAifsn(index, atoi(output), atoi(parameter));
			break;
			
		case GET_WMM_OG_AIFSN:
			retVal = wlan_getWmmOgAifsn(index, atoi(output), &output_int);
			printf("cli_return=%d\n", output_int);
			break;
			
		case SET_WMM_OG_ECW_MIN:
			retVal = wlan_setWmmOgEcwMin(index, atoi(output), atoi(parameter));
			break;
			
		case GET_WMM_OG_ECW_MIN:
			retVal = wlan_getWmmOgEcwMin(index, atoi(output), &output_int);
			printf("cli_return=%d\n", output_int);
			break;
			
		case SET_WMM_OG_ECW_MAX:
			retVal = wlan_setWmmOgEcwMax(index, atoi(output), atoi(parameter));
			break;
			
		case GET_WMM_OG_ECW_MAX:
			retVal = wlan_getWmmOgEcwMax(index, atoi(output), &output_int);
			printf("cli_return=%d\n", output_int);
			break;
			
		case SET_WMM_OG_TX_OP:
			retVal = wlan_setWmmOgTxOp(index, atoi(output), atoi(parameter));
			break;
			
		case GET_WMM_OG_TX_OP:
			retVal = wlan_getWmmOgTxOp(index, atoi(output), &output_int);
			printf("cli_return=%d\n", output_int);
			break;
		case GET_RADIO_BEACON_PERIOD:
			retVal = wlan_getRadioBeaconPeriod(index, (int *)&output_int);
			printf("cli_return=%d\n", output_int);
			break;
			
		case SET_RADIO_BEACON_PERIOD:
			retVal = wlan_setRadioBeaconPeriod(index, atoi(parameter));
			break;
			
		case GET_RADIO_GUARD_INTERVAL:
			retVal = wlan_getGuardInterval(index, output_string);	
			printf("cli_return=%s\n", output_string);	
			break;
			
		case SET_RADIO_GUARD_INTERVAL:
			retVal = wlan_setGuardInterval(index, parameter);
			break;
		
		case GET_BRIDGE_NAME:
			retVal = wlan_getBridgeName(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_BRIDGE_NAME:
			retVal = wlan_setBridgeName(index, parameter);
			break;
		
		case SET_WPS_ENROLLEE_PIN:
			retVal = wlan_setWpsEnrolleePin(index, parameter);
			break;
			
		case SET_COUNTRY_CODE:
			retVal = wlan_setCountryCode(index, parameter);
			break;
			
		case GET_COUNTRY_CODE:
			retVal = wlan_getCountryCode(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;
			
		case GET_WPS_STATUS:
			retVal = wlan_getWpsStatus(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_RADIO_ATF_ENABLE:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setAtfEnable(index, enable_bool);
			}
			break;

		case GET_RADIO_ATF_ENABLE:
			retVal = wlan_getAtfEnable(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s retVal=%d\n", enable, retVal);
			break;
		
		case SET_RADIO_ATF_DIST_TYPE:
			retVal = wlan_setAtfDistributionType(index, parameter);
			printf("cli_return retVal=%d\n", retVal);
			break;

		case GET_RADIO_ATF_DIST_TYPE:
			retVal = wlan_getAtfDistributionType(index, output_string);
			printf("cli_return=%s retVal=%d options:%s\n", output_string, retVal, "Static,Dynmic,Disable");
			break;
			
		case SET_RADIO_ATF_ALGO_TYPE:
			retVal = wlan_setAtfAlgoType(index, parameter);
			printf("cli_return retVal=%d\n", retVal);
			break;

		case GET_RADIO_ATF_ALGO_TYPE:
			retVal = wlan_getAtfAlgoType(index, output_string);
			printf("cli_return=%s retVal=%d options:%s\n", output_string, retVal, "Weighted,Global");
			break;
			
		case SET_RADIO_ATF_INTERVAL:
			retVal = wlan_setAtfInterval(index, atoi(parameter));
			printf("cli_return retVal=%d\n", retVal);
			break;

		case GET_RADIO_ATF_INTERVAL:
			retVal = wlan_getAtfInterval(index, (int *)&output_int);
			printf("cli_return=%d retVal=%d options:%s\n", output_int, retVal, "interval in msec");
			break;
			
		case SET_RADIO_ATF_FREE_TIME:
			retVal = wlan_setAtfFreeTime(index, atoi(parameter));
			printf("cli_return retVal=%d\n", retVal);
			break;

		case GET_RADIO_ATF_FREE_TIME:
			retVal = wlan_getAtfFreeTime(index, (int *)&output_int);
			printf("cli_return=%d retVal=%d options:%s\n", output_int, retVal, "free time in msec");
			break;

		case SET_RADIO_ATF_VAP_ENABLED:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setAtfVapEnabled(index, enable_bool);
			}
			break;

		case GET_RADIO_ATF_VAP_ENABLED:
			retVal = wlan_getAtfVapEnabled(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s retVal=%d\n", enable, retVal);
			break;

		case SET_RADIO_ATF_STA_ENABLED:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setAtfStationEnabled(index, enable_bool);
			}
			break;

		case GET_RADIO_ATF_STA_ENABLED:
			retVal = wlan_getAtfStationEnabled(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s retVal=%d\n", enable, retVal);
			break;

		case SET_RADIO_ATF_RADIO_STAS_AND_WEIGHTS:
			retVal = wlan_setAtfRadioStationsAndWeights(index, parameter);
			printf("cli_return retVal=%d\n", retVal);
			break;

		case GET_RADIO_ATF_RADIO_STAS_AND_WEIGHTS:
			retVal = wlan_getAtfRadioStationsAndWeights(index, output_string);
			printf("cli_return=%s retVal=%d options:%s\n", output_string, retVal, "Radio MAC,weight...N sations");
			break;
			
		case SET_RADIO_ATF_VAP_WEIGHT:
			retVal = wlan_setAtfVapWeight(index, atoi(parameter));
			printf("cli_return retVal=%d\n", retVal);
			break;

		case GET_RADIO_ATF_VAP_WEIGHT:
			retVal = wlan_getAtfVapWeight(index, (int *)&output_int);
			printf("cli_return=%d retVal=%d options:%s\n", output_int, retVal, "Weight of the VAP 1 - 100");
			break;
			
		case SET_RADIO_ATF_VAP_STAS_AND_WEIGHTS:
			retVal = wlan_setAtfVapStationsAndWeights(index, parameter);
			printf("cli_return retVal=%d\n", retVal);
			break;

		case GET_RADIO_ATF_VAP_STAS_AND_WEIGHTS:
			retVal = wlan_getAtfVapStationsAndWeights(index, output_string);
			printf("cli_return=%s retVal=%d options:%s\n", output_string, retVal, "VAP MAC,weight...N sations");
			break;

		case GET_WDS_ENABLED:
			retVal = wlan_getWdsEnabled(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;

		case SET_WDS_ENABLED:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setWdsEnabled(index, enable_bool);
			}
			break;

		case SET_WDS_SECURITY_MODE:
			retVal = wlan_setWdsSecurityMode(index, parameter);
			break;

		case GET_WDS_SECURITY_MODE:
			retVal = wlan_getWdsSecurityMode(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_WDS_KEY:
			retVal = wlan_setWdsKey(index, parameter);
			break;

		case GET_WDS_KEY:
			retVal = wlan_getWdsKey(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_WDS_PEERS:
			retVal = wlan_setWdsPeers(index, parameter);
			break;

		case GET_WDS_PEERS:
			retVal = wlan_getWdsPeers(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;

		case SET_WDS_MODE:
			retVal = wlan_setWdsMode(index, parameter);
			break;

		case GET_WDS_MODE:
			retVal = wlan_getWdsMode(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;
						
		case GET_UDMA_ENABLED:
			retVal = wlan_getUdmaEnabled(index, &enable_bool);
			changeBooleanToChar(&enable, &enable_bool);
			printf("cli_return=%s\n", enable);
			break;

		case SET_BF_MODE:
			retVal = wlan_setBfMode(index, parameter);
			break;

		case GET_BF_MODE:
			retVal = wlan_getBfMode(index, output_string);
			printf("cli_return=%s\n", output_string);
			break;
					
		case SET_UDMA_ENABLED:
			if (changeCharToBoolean(enable, &enable_bool) != UGW_FAILURE)
			{
				retVal = wlan_setUdmaEnabled(index, enable_bool);
			}
			break;
		
		case GET_VLAN_ID:
			retVal = wlan_getVlanId(index, (int *)&output_int);
			printf("cli_return=%d\n", output_int);
			break;
		
		case SET_VLAN_ID:
			retVal = wlan_setVlanId(index, atoi(parameter));
			break;
			
		case SET_WMM_IC_AIFSN: //Waiting
			//retVal = 
			break;
			
		case GET_WMM_IC_AIFSN: //Waiting
			//retVal = 
			break;
			
		case SET_WMM_IC_ECW_MIN: //Waiting
			//retVal = 
			break;
			
		case GET_WMM_IC_ECW_MIN: //Waiting
			//retVal = 
			break;
			
		case SET_WMM_IC_ECW_MAX: //Waiting
			//retVal = 
			break;
			
		case GET_WMM_IC_ECW_MAX: //Waiting
			//retVal = 
			break;
			
		case SET_WMM_IC_TX_OP: //Waiting
			//retVal = 
			break;
			
		case GET_WMM_IC_TX_OP: //Waiting
			//retVal = 
			break;
			
		case SET_BEACON_INTERVAL: //Waiting
			//retVal = 
			break;
			
		case GET_RADIO_BASIC_DATA_TRANSMIT_RATES: //Waiting
			//retVal = 
			break;
			
		case SET_RADIO_BASIC_DATA_TRANSMIT_RATES: //Waiting
			//retVal = 
			break;
		case GET_RADIO_DCS_SUPPORTED: //Waiting
			//retVal = 
			break;
		
		case GET_RADIO_DCS_ENABLE: //Waiting
			//retVal = 
			break;
			
		case SET_RADIO_DCS_ENABLE: //Waiting
			//retVal = 
			break;

		case GET_RADIO_AUTO_CH_REFRESH_PERIOD_SUPPORTED: //Waiting
			//retVal =
			break;
			
		case GET_RADIO_UP_TIME: //Waiting
			//retVal = 
			break;
			
		case GET_AP_RETRY_LIMIT: //Waiting
			//retVal = 
			break;
			
		case SET_AP_RETRY_LIMIT: //Waiting
			//retVal = 
			break;
		
		case BADAPI: /* handle failed lookup */
			printf("Wrong command, please try again !\n");
			return UGW_FAILURE;
			break;
	}

	printf("retVal is %d\n", retVal);
	if (retVal == 0)
	{
		printf("FAPI Command Success\n");
	} else {
		printf("FAPI Command Failed\n");
	}
	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);  // free memory

	return 0;
}


