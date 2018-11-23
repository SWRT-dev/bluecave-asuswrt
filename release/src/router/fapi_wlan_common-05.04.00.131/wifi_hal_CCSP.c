/******************************************************************************

						 Copyright (c) 2015
						Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/*  *****************************************************************************
 *         File Name    : wifi_hal_Technicolor.c                               	*
 *         Description  : adaption layer between FAPI RPC and CCSP				*
 *                                                                              *
 *  *****************************************************************************/

#include <stdio.h>
#include <string.h>

#include "help_objlist.h"
#include "wlan_config_api.h"
#include "wifi_hal_CCSP.h"
#include "fapi_wlan_private.h"

#define STR_ARRAY_LEN 256

 /* General Remark: This module is API LEVEL WRAPPER,
	All functions should not have internal logic instead call relevant RPC function.
	it is NOT allowed to use external helper functions with this module by definition!
*/
static init_helper(int apIndex) {
	char command[256];
	sprintf(command, "iwconfig wlan%d", apIndex);
	if (system("lsmod | grep mtlk") == UGW_SUCCESS)
	{
		printf("WAVE WLAN driver already insmod, doing Up\n");
		if (system(command) == UGW_FAILURE)
		{
			printf("WAVE WLAN driver not ready - Ignores Up\n");
		}
		else {
			return wlan_ifconfigUp(apIndex);
		}
	}
	else {
		return wlan_init();
	}
}

INT wifi_init(void)
{
	return init_helper(0);
}

INT wifi_initRadio(INT radioIndex)
{
	return init_helper((int)radioIndex);
}

INT wifi_factoryReset()
{
	return wlan_factoryReset();
}

INT wifi_setChannel(INT radioIndex, ULONG channel)
{
	return wlan_setChannel(radioIndex, (u_int_32)channel);
}

INT wifi_setAutoChannelEnable(INT radioIndex, BOOL enable)
{
	if (wlan_setAutoChannelEnable(radioIndex, enable) == UGW_SUCCESS)
	{
		return wlan_ifconfigUp(radioIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_pushDefaultValues(INT radioIndex)
{
	return UGW_SUCCESS;
}

INT wifi_getBaseBSSID(INT radioIndex, CHAR *output_string)
{
	return wlan_getBaseBSSID(radioIndex, output_string);
}

INT wifi_getChannel(INT radioIndex, ULONG *output_ulong)
{
	wlan_getChannel(radioIndex, (u_int_32*)output_ulong);
}

INT wifi_pushChannel(INT radioIndex, UINT channel)
{
	if (wlan_setChannel(radioIndex, channel) == UGW_SUCCESS)
	{
		return wlan_ifconfigUp(radioIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_createAp(INT apIndex, INT radioIndex, CHAR *essid, BOOL hideSsid)
{
	printf("__FUNCTION__ INPUT: apIndex[%d] radioIndex[%d] essid[%s] hideSsid[%u]\n", apIndex, radioIndex, essid, hideSsid);
	return wlan_createVap(apIndex, radioIndex, essid, hideSsid);
}

INT wifi_deleteAp(INT apIndex)
{
	//TCH requirement to delete 
	//return wlan_deleteVap(apIndex);
	return wlan_ifconfigDown(apIndex);
}

INT wifi_setSSID(INT apIndex, CHAR *ssid_string)
{
	return wlan_setSSID(apIndex, ssid_string);
}

INT wifi_setSSIDName(INT apIndex, CHAR *ssid_string)
{
	return wlan_setSSID(apIndex, ssid_string);
}

INT wifi_pushSSID(INT apIndex, CHAR *ssid)
{
	if (wlan_setSSID(apIndex, ssid) == RETURN_OK)
	{
		return wlan_ifconfigUp(apIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_getKeyPassphrase(INT apIndex, CHAR *output_string)
{
	return wlan_getPassphrase(apIndex, output_string);
}

INT wifi_getApSecurityModeEnabled(INT apIndex, CHAR *output)
{
	return wlan_getApSecurityModeEnabled(apIndex, output);
}

INT wifi_getAutoChannelEnable(INT radioIndex, BOOL *output_bool)
{
	return wlan_AutoChannelEnable(radioIndex, (bool*)output_bool);
}

INT wifi_getRadioActive(INT radioIndex, BOOL *output_bool)
{
	return wlan_getRadioActive(radioIndex, (bool*)output_bool);
}

INT wifi_getRadioEnable(INT radioIndex, BOOL *output_bool)
{
	return wlan_getRadioEnabled(radioIndex, (bool*)output_bool);
}

INT wifi_getEnable(INT apIndex, BOOL *output_bool)
{
	return wlan_getEnable(apIndex, (bool*)output_bool);
}

INT wifi_getSSIDEnable(INT apIndex, BOOL *output_bool)
{
	return wlan_getEnable(apIndex, (bool*)output_bool);
}

INT wifi_getApEnable(INT apIndex, BOOL *enableFlag)
{
	return wlan_getEnable(apIndex, (bool*)enableFlag);
}

INT wifi_getSSIDStatus(INT apIndex, CHAR *output_string)
{
	return wlan_getStatus(apIndex, output_string);
}

INT wifi_getName(INT apIndex, CHAR *output_string)
{
	if (getInterfaceName(apIndex, &output_string) == UGW_SUCCESS)
	{
		return UGW_SUCCESS;
	}
}

INT wifi_getSSID(INT apIndex, CHAR *output_string)
{
	return wlan_getSSID(apIndex, output_string);
}

INT wifi_getSSIDName(INT apIndex, CHAR *output_string)
{
	return wlan_getSSID(apIndex, output_string);
}

INT wifi_getPreSharedKey(INT apIndex, CHAR *output_string)
{
	return wlan_getPreSharedKey(apIndex, 0, output_string);
}

INT wifi_setBasicAuthenticationMode(INT apIndex, CHAR *authMode)
{
	return wlan_setBasicAuthenticationModes(apIndex, authMode);
}

INT wifi_getWpaEncryptoinMode(INT apIndex, CHAR *output_string)
{
	return wlan_getWpaEncryptionModes(apIndex, output_string);
}

INT wifi_setWpaEncryptionMode(INT apIndex, CHAR *encMode)
{
	return wlan_setWpaEncryptionModes(apIndex, encMode);
}

INT wifi_setBasicEncryptionMode(INT apIndex, CHAR *encMode)
{
	return wlan_setBasicEncryptionModes(apIndex, encMode);
}

INT wifi_setApSecurityModeEnabled(INT apIndex, CHAR *encMode)
{
	//TODO, currently the CCSP doesn't support this, once will be ready maybe WEP ALSO will be required 
	return RETURN_OK;
}

INT wifi_setAuthMode(INT apIndex, INT mode)
{
	return RETURN_OK;
}

INT wifi_getStandard(INT radioIndex, CHAR *output_string, BOOL *gOnly, BOOL *nOnly, BOOL *acOnly)
{
	if (wlan_getStandard(radioIndex, output_string) == UGW_SUCCESS)
	{
		*acOnly = (BOOL)FALSE;
		*gOnly = (BOOL)FALSE;
		*nOnly = (BOOL)FALSE;
		if (!strcmp(output_string, "ac"))
		{
			*acOnly = (BOOL)TRUE;
		}
		else if (!strcmp(output_string, "g")) {
			*gOnly = (BOOL)TRUE;
		}
		else if (!strcmp(output_string, "n")) {
			*nOnly = (BOOL)TRUE;
		}
	}
	else {
		return RETURN_ERR;
	}

	return RETURN_OK;
}

INT wifi_getWifiEnableStatus(INT radioIndex, BOOL *output_bool)
{
	return wlan_getWifiEnableStatusBool(radioIndex, (bool*)output_bool);
}

INT wifi_setWifiEnableStatus(INT radioIndex, BOOL enabled)
{
	if (wlan_setRadioEnabled(radioIndex, (bool)enabled) == UGW_SUCCESS)
	{
		return wlan_ifconfigUp(radioIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_setRadioEnable(INT radioIndex, BOOL enable)
{
	if (wlan_setRadioEnabled(radioIndex, (bool)enable) == UGW_SUCCESS)
	{
		return wlan_ifconfigUp(radioIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_setEnable(INT apIndex, BOOL enable)
{
	return wlan_setEnable(apIndex, (bool)enable);
}

INT wifi_setSSIDEnable(INT apIndex, BOOL enable)
{
	return wlan_setEnable(apIndex, enable);
}

INT wifi_getSupportedFrequencyBands(INT radioIndex, CHAR *output_string)
{
	return wlan_getSupportedFrequencyBands(radioIndex, output_string);
}

INT wifi_getBeaconType(INT apIndex, CHAR *output_string)
{
	return wlan_getBeaconType(apIndex, output_string);
}

INT wifi_ifConfigUp(INT apIndex)
{
	return wlan_ifconfigUp(apIndex);
}

INT wifi_setBeaconType(INT apIndex, CHAR *beaconTypeString)
{
	return wlan_setBeaconType(apIndex, beaconTypeString);
}

INT wifi_getWpaRekeyInterval(INT apIndex, INT *output_int)
{
	return wlan_getWpaRekeyInterval(apIndex, output_int);
}

INT wifi_setWpaRekeyInterval(INT apIndex, INT rekeyInterval)
{
	return wlan_setWpaRekeyInterval(apIndex, rekeyInterval);
}

INT wifi_getIndexFromName(CHAR *inputSsidString, INT *output_int)
{
	return wlan_getIndexFromName(inputSsidString, output_int);
}

INT wifi_getRadioIndex(INT apIndex, INT *output_int)
{
	return wlan_getRadioIndex(apIndex, output_int);
}

wifi_getSSIDRadioIndex(INT apIndex, INT *output_int)
{
	return wlan_getRadioIndex(apIndex, output_int);
}

INT wifi_setPreSharedKey(INT apIndex, CHAR *preSharedKey)
{
	return wlan_setPreSharedKey(apIndex, 0, preSharedKey);
}

INT wifi_setKeyPassphrase(INT apIndex, CHAR *passPhrase)
{
	return wlan_setKeyPassphrase(apIndex, 0, passPhrase);
}

INT wifi_getPossibleChannels(INT radioIndex, CHAR *output_string)
{
	return wlan_getPossibleChannels(radioIndex, output_string);
}

INT wifi_getStatus(INT apIndex, CHAR *output_string)
{
	return wlan_getStatus(apIndex, output_string);
}

INT wifi_getEnableOnLine(INT apIndex, BOOL *output_bool)
{
	return wlan_getEnableOnline(apIndex, (bool*)output_bool);
}

INT wifi_setEnableOnLine(INT apIndex, BOOL enabled)
{
	return wlan_setEnableOnline(apIndex, (bool)enabled);
}

INT wifi_ifConfigDown(INT apIndex)
{
	return wlan_ifconfigDown(apIndex);
}

INT wifi_getNumDevicesAssociated(INT apIndex, ULONG *output_ulong)
{
	return wlan_getAssocDevicesNum(apIndex, (u_int_32*)output_ulong);
}

INT wifi_setApBridging(INT apIndex, BOOL bridgeEnable)
{
	if (wlan_setApBridging(apIndex, (bool)bridgeEnable) == UGW_SUCCESS)
	{
		return wlan_ifconfigUp(apIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_pushChannelMode(INT radioIndex)
{
	return wlan_ifconfigUp(radioIndex);
}


INT wifi_getChannelMode(INT radioIndex, CHAR *output_string)
{
	return wlan_getChannelMode(radioIndex, output_string);
}


INT wifi_setChannelMode(INT radioIndex, CHAR *channelMode, BOOL gOnlyFlag, BOOL nOnlyFlag, BOOL acOnlyFlag)
{
	return wlan_setChannelMode(radioIndex, channelMode, (bool)gOnlyFlag, (bool)nOnlyFlag, (bool)acOnlyFlag);
}

INT wifi_getRouterEnable(INT apIndex, BOOL *output_bool)
{
	return wlan_getRouterEnable(apIndex, (bool*)output_bool);
}

INT wifi_setRouterEnable(INT apIndex, BOOL enabled)
{
	return wlan_setRouterEnable(apIndex, (bool)enabled);
}

INT wifi_removeSecVaribles(INT apIndex)
{
	return UGW_SUCCESS;
}

INT wifi_disableEncryption(INT apIndex)
{
	return UGW_SUCCESS;
}

INT wifi_createHostApdConfig(INT apIndex, BOOL createWpsCfg)
{
	return UGW_SUCCESS;
}

INT wifi_startHostApd()
{
	return UGW_SUCCESS;
}

INT wifi_stopHostApd()
{
	return UGW_SUCCESS;
}

INT wifi_getSsidAdvertisementEnable(INT apIndex, BOOL *output_bool)
{
	return wlan_getSsidAdvertisementEnabled(apIndex, (bool*)output_bool);
}

INT wifi_setSsidAdvertisementEnable(INT apIndex, BOOL enable)
{
	return wlan_setSsidAdvertisementEnabled(apIndex, enable);
}

INT wifi_pushSsidAdvertisementEnable(INT apIndex, BOOL enable)
{
	if (wlan_setSsidAdvertisementEnabled(apIndex, enable) == UGW_SUCCESS)
	{
		return wlan_ifconfigUp(apIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_getWpsEnable(INT apIndex, BOOL *output_bool)
{
	return wlan_getWpsEnable(apIndex, (bool*)output_bool);
}

INT wifi_setWpsEnable(INT apIndex, BOOL enableValue)
{
	return wlan_setWpsEnable(apIndex, (bool)enableValue);
}

INT wifi_getWpsConfigMethodsEnabled(INT apIndex, CHAR *output_string)
{
	return wlan_getWpsConfigMethodsEnabled(apIndex, output_string);
}

INT wifi_setWpsConfigMethodsEnabled(INT apIndex, CHAR *methodString)
{
	return wlan_setWpsConfigMethodsEnabled(apIndex, methodString);
}

INT wifi_getApWepKeyIndex(INT apIndex, ULONG *output_ulong)
{
	//in TR181 supported only one key index use
	//it agreed with TCH that we always return index=1 
	return wlan_getWepKeyIndex(apIndex, (u_int_32*)output_ulong);
}

INT wifi_pushApSsidAdvertisementEnable(INT apIndex, BOOL enable)
{
	if (wlan_setSsidAdvertisementEnabled(apIndex, enable) == UGW_SUCCESS)
	{
		return wlan_ifconfigUp(apIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_setMaxStations(INT apIndex, INT maxStations)
{
	return wlan_setMaxStations(apIndex, maxStations);
}

INT wifi_setApMaxAssociatedDevices(INT apIndex, UINT maxStations)
{
	return wlan_setMaxStations(apIndex, maxStations);
}

INT wifi_getApMaxAssociatedDevices(INT apIndex, UINT *output)
{
	return wlan_getMaxStations(apIndex, (unsigned int *)output);
}

INT wifi_getWpsDevicePIN(INT apIndex, ULONG *output_ulong)
{
	return wlan_getWpsDevicePIN(apIndex, (u_int_32*)output_ulong);
}

INT wifi_setWpsDevicePIN(INT apIndex, ULONG pin)
{
	return wlan_setWpsDevicePIN(apIndex, (u_int_32)pin);
}

INT wifi_getWpsConfigurationState(INT apIndex, CHAR *output_string)
{
	return wlan_getWpsConfigurationState(apIndex, output_string);
}

INT wifi_getApWMMCapability(INT apIndex, BOOL *output)
{
	return wlan_getApWMMCapability(apIndex, (bool*)output);
}

INT wifi_setWmmUapsdEnable(INT apIndex, BOOL enable)
{
	return wlan_setWmmUaspdEnable(apIndex, (bool)enable);
}

INT wifi_getApUAPSDCapability(INT apIndex, BOOL *output)
{
	return wlan_getApUAPSDCapability(apIndex, (bool*)output);
}

INT wifi_getApIsolationEnable(INT apIndex, BOOL *output)
{
	return wlan_getApIsolationEnable(apIndex, (bool*)output);
}

INT wifi_setApIsolationEnable(INT apIndex, BOOL enable)
{
	return wlan_setApBridging(apIndex, (bool)enable);
}

INT wifi_getApWmmUapsdEnable(INT apIndex, BOOL * output)
{
	return wlan_getApWmmUapsdEnable(apIndex, (bool*)output);
}

INT wifi_setApWmmUapsdEnable(INT apIndex, BOOL enable)
{
	return wlan_setApWmmUapsdEnable(apIndex, (bool)enable);
}

INT wifi_getApSecurityRadiusServerIPAddr(INT apIndex, CHAR *output)
{
	return wlan_getApSecurityRadiusServerIPAddr(apIndex, output);
}

INT wifi_setApSecurityRadiusServerIPAddr(INT apIndex, CHAR *IPAddress)
{
	return wlan_setApSecurityRadiusServerIPAddr(apIndex, IPAddress);
}

INT wifi_getApSecurityRadiusServerPort(INT apIndex, UINT *output)
{
	return wlan_getApSecurityRadiusServerPort(apIndex, output);
}

INT wifi_setApSecurityRadiusServerPort(INT apIndex, UINT port)
{
	printf("INPUT: apIndex[%d] port[%d]\n", apIndex, port);
	return wlan_setApSecurityRadiusServerPort(apIndex, port);
}

INT wifi_getApSecurityRadiusServer(INT apIndex, CHAR *IP_output, UINT *Port_output)
{
	return wlan_getApSecurityRadiusServer(apIndex, IP_output, Port_output);
}

INT wifi_setApSecurityRadiusServer(INT apIndex, CHAR *IPAddress, UINT port)
{
	printf("INPUT: apIndex[%d] IPAddress[%s] port[%d]\n", apIndex, IPAddress, port);
	return wlan_setApSecurityRadiusServer(apIndex, IPAddress, port);
}

INT wifi_cancelWPS(INT apIndex)
{
	return wlan_cancelWps(apIndex);
}

INT wifi_kickAclAssociatedDevices(int apIndex, BOOL enable)
{
	if (wlan_kickAclAssociatedDevices(apIndex, (bool)enable) == UGW_SUCCESS)
	{
		return wlan_ifconfigUp(apIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_setMacAddressControlMode(INT apIndex, INT filterMode)
{
	if (wlan_setMacAddressControlMode(apIndex, filterMode) == UGW_SUCCESS)
	{
		return wlan_ifconfigUp(apIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_addAclDevice(INT apIndex, CHAR *DeviceMacAddress)
{

	if (wlan_addAclDevice(apIndex, DeviceMacAddress) == UGW_SUCCESS)
	{
		return wlan_ifconfigUp(apIndex);
	}
	else {
		return RETURN_ERR;
	}
}

INT wifi_delAclDevice(INT apIndex, CHAR *DeviceMacAddress)
{
	return wlan_delAclDevice(apIndex, DeviceMacAddress);
}

INT wifi_getAclDeviceNum(INT apIndex, UINT *output_uint)
{
	return wlan_getAclDeviceNum(apIndex, output_uint);
}

INT wifi_getWepKey(INT apIndex, INT keyIndex, CHAR *output_string)
{
	int retVal = 0;
	printf("INPUT: apIndex[%d] keyIndex[%d]\n", apIndex, keyIndex);
	retVal = wlan_getWepKey(apIndex, 1, output_string);
	printf("OUTPUT: output_string[%s] ReturnValue[%d]\n", output_string, retVal);
	return retVal;
}

INT wifi_setWepKey(INT apIndex, INT keyIndex, CHAR *WepKey)
{
	int retVal = 0;
	printf("INPUT: apIndex[%d] keyIndex[%d] WepKey[%p]\n", apIndex, keyIndex, WepKey);
	retVal = wlan_setWepKey(apIndex, keyIndex, WepKey);
	printf("OUTPUT: apIndex[%d] keyIndex[%d] WepKey[%s] ReturnValue[%d]\n", apIndex, keyIndex, WepKey, retVal);
	return retVal;
}

INT wifi_pushWepKey(INT apIndex, INT keyIndex)
{
	return wlan_ifconfigUp(apIndex);
}

INT wifi_setApWepKeyIndex(INT apIndex, ULONG output_ulong)
{
	return wlan_setWepKeyIndex(apIndex, (u_int_32)output_ulong);
}

INT wifi_pushWepKeyIndex(INT apIndex, UINT keyIndex)
{
	return wlan_ifconfigUp(apIndex);
}

INT wifi_getApAclDevices(INT apIndex, CHAR ***macArray, UINT *buf_size)
{
	int i = 0;
	int ret_val = wlan_getApAclDevices(apIndex, macArray, (unsigned int *)buf_size);

	printf("Checking return values...  macArray:%p\n", macArray);
	for (i = 0; i < *buf_size; i++)
	{
		printf("*macArray[%d] = %s\n", i, *macArray[i]);
	}
	return ret_val;
}

INT wifi_setWpsButtonPush(INT apIndex)
{
	return wlan_setWpsPbcTrigger(apIndex);
}

INT wifi_getAssociatedDeviceDetail(INT apIndex, INT devIndex, wifi_device_t *output_struct)
{
	printf("INPUT: apIndex[%d] devIndex[%d] output_struct[%p]\n", apIndex, devIndex, output_struct);
	return wlan_getAssociatedDeviceDetail(apIndex, devIndex, (wifi_device_fapi_t*)output_struct);
}

INT wifi_getAllAssociatedDeviceDetail(INT apIndex, ULONG *output_ulong, wifi_device_t ***output_struct)
{
	printf("INPUT: apIndex[%d] output_ulong[%p] output_struct[%p]\n", apIndex, output_ulong, output_struct);
	return wlan_getAllAssociatedDeviceDetail(apIndex, (u_int_32*)output_ulong, (wifi_device_fapi_t***)output_struct);
}

INT wifi_setWpsEnrolleePin(INT apIndex, CHAR *pin)
{
	unsigned long ul;
	char * stopstring;
	ul = strtoul(pin, &stopstring, 10);
	return wlan_setWpsDevicePIN(apIndex, ul);
}

INT wifi_getTransmitPower(INT radioIndex, ULONG *output_ulong)
{
	return wlan_getTransmitPower(radioIndex, (u_int_32*)output_ulong);
}

INT wifi_setTransmitPower(INT radioIndex, ULONG TransmitPower)
{
	return wlan_setTransmitPower(radioIndex, (u_int_32)TransmitPower);
}

INT wifi_setSTBCEnable(INT radioIndex, BOOL STBC_Enable)
{
	printf("This API updates the value of HT STBC, VHT will not change.\n");
	return wlan_setSTBCEnable(radioIndex, (bool)STBC_Enable);
}

INT wifi_getAMSDUEnable(INT radioIndex, BOOL *output_bool)
{
	return 0;
}
INT wifi_setAMSDUEnable(INT radioIndex, BOOL amsduEnable)
{
	return 0;
}

/*
check if parameters aligned to TCH
INT wifi_setShortGuardInterval(INT radioIndex, BOOL enable)
{
	return wlan_setGuardInterval(radioIndex, (bool)enable);
}
*/

INT wifi_getRadioIEEE80211hEnabled(INT radioIndex, BOOL *enable)
{
	return wlan_getRadioIEEE80211hEnabled(radioIndex, (bool*)enable);
}

INT wifi_getRadioIEEE80211hSupported(INT radioIndex, BOOL *Supported)
{
	return wlan_getRadioIEEE80211hSupported(radioIndex, (bool*)Supported);
}

INT wifi_getRadioDCSSupported(INT radioIndex, BOOL *output_bool)
{
	*output_bool = false;
	return RETURN_OK;
}

INT wifi_getRadioAutoChannelRefreshPeriodSupported(INT radioIndex, BOOL *output_bool)
{
	return wlan_getRadioAutoChannelRefreshPeriodSupported(radioIndex, (bool*)output_bool);
}

INT wifi_getRadioAutoChannelRefreshPeriod(INT radioIndex, ULONG *output_ulong)
{
	return wlan_getRadioAutoChannelRefreshPeriod(radioIndex, (u_int_32*)output_ulong);
}

INT wifi_setRadioAutoChannelRefreshPeriod(INT radioIndex, ULONG seconds)
{
	return wlan_setRadioAutoChannelRefreshPeriod(radioIndex, (u_int_32)seconds);
}

INT wifi_getRadioDCSEnable(INT radioIndex, BOOL *output_bool)
{
	return RETURN_ERR;
}

INT wifi_setRadioDCSEnable(INT radioIndex, BOOL enable)
{
	return RETURN_ERR;
}

INT wifi_setWmmOgAifsn(INT apIndex, INT accessCat, UINT aifsn)
{
	return wlan_setWmmOgAifsn(apIndex, accessCat, (u_int_32)aifsn);
}

INT wifi_getWmmOgAifsn(INT apIndex, INT accessCat, UINT *aifsn)
{
	return wlan_getWmmOgAifsn(apIndex, accessCat, (u_int_32*)aifsn);
}

INT wifi_setWmmOgEcwMin(INT apIndex, INT accessCat, UINT ecwMin)
{
	return wlan_setWmmOgEcwMin(apIndex, accessCat, (u_int_32)ecwMin);
}

INT wifi_getWmmOgEcwMin(INT apIndex, INT accessCat, UINT *ecwMin)
{
	return wlan_getWmmOgEcwMin(apIndex, accessCat, (u_int_32*)ecwMin);
}

INT wifi_setWmmOgEcwMax(INT apIndex, INT accessCat, UINT ecwMax)
{
	return wlan_setWmmOgEcwMax(apIndex, accessCat, (u_int_32)ecwMax);
}

INT wifi_getWmmOgEcwMax(INT apIndex, INT accessCat, UINT *ecwMax)
{
	return wlan_getWmmOgEcwMax(apIndex, accessCat, (u_int_32*)ecwMax);
}

INT wifi_setWmmOgTxOp(INT apIndex, INT accessCat, UINT txOp)
{
	return wlan_setWmmOgTxOp(apIndex, accessCat, (u_int_32)txOp);
}

INT wifi_getWmmOgTxOp(INT apIndex, INT accessCat, UINT *txOp)
{
	return wlan_getWmmOgTxOp(apIndex, accessCat, (u_int_32*)txOp);
}

INT wifi_setWmmOgAckPolicy(INT apIndex, INT accessCat, UINT ackPolicy)
{
	return RETURN_ERR;
}

INT wifi_getWmmOgAckPolicy(INT apIndex, INT accessCat, UINT *ackPolicy)
{
	return RETURN_ERR;
}

INT wifi_setWmmIcAifsn(INT apIndex, INT accessCat, UINT aifsn)
{
	return RETURN_ERR;
}

INT wifi_getWmmIcAifsn(INT apIndex, INT accessCat, UINT *aifsn)
{
	return RETURN_ERR;
}

INT wifi_setWmmIcEcwMin(INT apIndex, INT accessCat, UINT ecwMin)
{
	return RETURN_ERR;
}

INT wifi_getWmmIcEcwMin(INT apIndex, INT accessCat, UINT *ecwMin)
{
	return RETURN_ERR;
}

INT wifi_setWmmIcEcwMax(INT apIndex, INT accessCat, UINT ecwMax)
{
	return RETURN_ERR;
}

INT wifi_getWmmIcEcwMax(INT apIndex, INT accessCat, UINT *ecwMax)
{
	return RETURN_ERR;
}

INT wifi_setWmmIcTxOp(INT apIndex, INT accessCat, UINT txOp)
{
	return RETURN_ERR;
}

INT wifi_getWmmIcTxOp(INT apIndex, INT accessCat, UINT *txOp)
{
	return RETURN_ERR;
}

INT wifi_getRadioGuardInterval(INT radioIndex, CHAR *output_string)
{
	return wlan_getGuardInterval(radioIndex, output_string);
}

INT wifi_setRadioGuardInterval(INT radioIndex, CHAR *string)
{
	return wlan_setGuardInterval(radioIndex, string);
}

INT wifi_createInitialConfigFiles()
{
	return RETURN_OK;
}

INT wifi_getRadioIGMPSnoopingEnable(INT radioIndex, BOOL *output)
{
	return RETURN_OK;
}

INT wifi_setRadioIGMPSnoopingEnable(INT radioIndex, BOOL enable)
{
	return RETURN_OK;
}

INT wifi_getRadioDeclineBARequestEnable(INT radioIndex, BOOL *output)
{
	return RETURN_OK;
}

INT wifi_setRadioDeclineBARequestEnable(INT radioIndex, BOOL enable)
{
	return RETURN_OK;
}

INT wifi_getRadioAutoBlockAckEnable(INT radioIndex, BOOL *output)
{
	return RETURN_OK;
}

INT wifi_setRadioAutoBlockAckEnable(INT radioIndex, BOOL output)
{
	return RETURN_OK;
}

INT wifi_getTxChainMask(INT radioIndex, INT *output_int)
{
	return wlan_getTxChainMask(radioIndex, output_int);
}

INT wifi_setTxChainMask(INT radioIndex, INT numStreams)
{
	return wlan_setTxChainMask(radioIndex, numStreams);
}

INT wifi_getRxChainMask(INT radioIndex, INT *output_int)
{
	return wlan_getRxChainMask(radioIndex, output_int);
}

INT wifi_setRxChainMask(INT radioIndex, INT numStreams)
{
	return wlan_setRxChainMask(radioIndex, numStreams);
}

INT wifi_pushTxChainMask(INT radioIndex)
{
	return wlan_ifconfigUp(radioIndex);
}

INT wifi_pushRxChainMask(INT radioIndex)
{
	return wlan_ifconfigUp(radioIndex);
}

INT wifi_setWmmEnable(INT apIndex, BOOL enable)
{
	return wlan_setWmmEnable(apIndex, (bool)enable);
}

INT wifi_getAccessNetworkType(INT apIndex, ULONG *output)
{
	printf("Not supported, Part Of HS2.0\n");
	return UGW_FAILURE;
}

INT wifi_setAccessNetworkType(INT apIndex, ULONG ant)
{
	printf("Not supported, Part Of HS2.0\n");
	return UGW_FAILURE;
}

wifi_down()
{
	return wlan_down();
}

INT wifi_reset()
{
	return wlan_reset();
}

INT wifi_factoryResetRadios()
{
	return wlan_factoryResetRadios();
}

INT wifi_getRadioBeaconPeriod(INT radioIndex, UINT *output)
{
	return wlan_getRadioBeaconPeriod(radioIndex, (unsigned int*)output);
}

INT wifi_setRadioBeaconPeriod(INT radioIndex, UINT beaconPeriod)
{
	return wlan_setRadioBeaconPeriod(radioIndex, (unsigned int)beaconPeriod);
}

INT wifi_setRtsThreshold(INT apIndex, UINT threshold)
{

}

INT wifi_setBeaconInterval(INT apIndex, INT beaconInterval)
{
	//return wlan_setBeaconInterval(apIndex, beaconInterval);
}