/*  ***************************************************************************** 
 *         File Name    : fapi_wlan_band_steering.c                            	*
 *         Description  : band steering (according to RSSI) functions	 		* 
 *                                                                              *
 *  *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "fapi_wlan_band_steering.h"
#include "fapi_wlan_private.h"
#include "fapi_wlan_beerock.h"
#include "fapi_wlan_tools.h"
#include "help_objlist.h"
#include "wpa_ctrl.h"
#include <errno.h>  //added for 'errno'


typedef struct StationsInfo
{
	char MACAddress[18];
	char connectedTo[6];
	int  connectionTime;
	int is_5G_supported;
	char ifnameCheckIfConnected[6];
	int  numOfTicks;
	bool btm_supported;
	bool isBandSteeringPossible;
	struct StationsInfo *nextStation;
} StationsInfo_t;

typedef struct
{
	char *name;
	char operationMode[8];
	char wpaCtrlName[32];
	int  fd;
} RadioInterface;

typedef struct
{
	char *ifname;
	char supportedFrequencyBands[16];
	char ifnameToSteerTo[8];
	char BSSID_ToSteerTo[24];
} InterfaceBandSteerInfo;

static StationsInfo_t *firstStationInfo = NULL;
static int numOfActiveApInterfaces = 0;
static RadioInterface radioInterface[] = { { "wlan0", "NONE", "NONE", -1 },
                                           { "wlan1", "NONE", "NONE", -1 },
                                           { "wlan2", "NONE", "NONE", -1 },
                                           { "wlan3", "NONE", "NONE", -1 },
                                           { "wlan4", "NONE", "NONE", -1 },
                                           { "wlan5", "NONE", "NONE", -1 } };

static InterfaceBandSteerInfo interfaceBandSteerInfo[] =
{
	{ "wlan0", "\0", "\0", "\0" },
	{ "wlan1", "\0", "\0", "\0" },
	{ "wlan2", "\0", "\0", "\0" },
	{ "wlan3", "\0", "\0", "\0" },
	{ "wlan4", "\0", "\0", "\0" },
	{ "wlan5", "\0", "\0", "\0" },
	{ NULL,    "\0", "\0", "\0" }
};


/*********************************************/
/* static Functions for general internal use */
/*********************************************/
static void stationsInfoListPrint(void)
{
	bool           isPrintEmptyLine = false;
	StationsInfo_t *stationsInfo = firstStationInfo;

	if (stationsInfo != NULL)
	{
		isPrintEmptyLine = true;
	}

	while (stationsInfo != NULL)
	{
		printf("=====================================================\n");
		printf("MACAddress= '%s'\n", stationsInfo->MACAddress);
		printf("-----------------------------------------------------\n");
		printf("connectedTo= '%s'\n", stationsInfo->connectedTo);
		printf("connectionTime= %d\n", stationsInfo->connectionTime);
		printf("is_5G_supported= %d\n", stationsInfo->is_5G_supported);
		printf("ifnameCheckIfConnected= '%s'\n", stationsInfo->ifnameCheckIfConnected);
		printf("numOfTicks= %d\n", stationsInfo->numOfTicks);
		printf("btm_supported= %d\n", stationsInfo->btm_supported);
		printf("isBandSteeringPossible= %d\n", stationsInfo->isBandSteeringPossible);
		printf("nextStation= 0x%x\n", (unsigned int)stationsInfo->nextStation);
		printf("-----------------------------------------------------\n");

		stationsInfo = stationsInfo->nextStation;
	}

	if (isPrintEmptyLine)
	{
		printf("\n");
	}
}


static void stationsInfoListClear(void)
{
	StationsInfo_t *stationsInfo = firstStationInfo, *stationsInfoToClear = NULL;

	printf("%s Entry\n", __FUNCTION__);

	while (stationsInfo != NULL)
	{
		stationsInfoToClear = stationsInfo;
		stationsInfo = stationsInfo->nextStation;

		printf("%s; Clear Record; MACAddress= '%s'; stationsInfoToClear= 0x%x\n", __FUNCTION__, stationsInfo->MACAddress, (unsigned int)stationsInfoToClear);
		free((void *)stationsInfoToClear);
	}

	firstStationInfo = NULL;
}


static StationsInfo_t *stationsInfoGet(char *MACAddress)
{
	StationsInfo_t *stationsInfo = firstStationInfo;

	while (stationsInfo != NULL)
	{
		if (!strcmp(stationsInfo->MACAddress, MACAddress))
		{
			printf("%s; MACAddress ('%s') record found!\n", __FUNCTION__, stationsInfo->MACAddress);
			return stationsInfo;
		}

		stationsInfo = stationsInfo->nextStation;
	}

	return stationsInfo;
}


static int interfaceIndexGet(const char *ifname)
{
	int i = 0;

	while (interfaceBandSteerInfo[i].ifname != NULL)
	{
		if (!strcmp(interfaceBandSteerInfo[i].ifname, ifname))
		{
			return i;
		}

		i++;
	}

	return -1;
}


static int fapiWlanCallBackBandSteeringFunc(char *opCode, const char *ifname, ObjList *wlObj, unsigned int flags, void *context)
{
	char           *field;
	time_t         rawtime;
	int            idx;
	StationsInfo_t *stationsInfo = NULL;

	(void)flags;
	(void)context;

	printf("%s; ifname= '%s', opCode= '%s'\n", __FUNCTION__, ifname, opCode);

	printf("\n%s; database BEFORE event process:\n", __FUNCTION__);
	stationsInfoListPrint();

	if (wlObj != NULL)
	{
		printf("\n%s; print the input Obj\n", __FUNCTION__);
		setLog(ifname, wlObj, 0);
		printf("\n");
	}

	if (!strcmp(opCode, "AP-STA-CONNECTED"))
	{
		/* <3>AP-STA-CONNECTED wlan0 24:77:03:80:5d:90 SignalStrength=-49 SupportedRates=2 4 11 22 12 18 24 36 48 72 96 108 HT_CAP=107E HT_MCS=FF FF FF 00 00 00 00 00 00 00 C2 01 01 00 00 00
		   VHT_CAP=03807122 VHT_MCS=FFFA 0000 FFFA 0000 btm_supported=1 nr_enabled=0 non_pref_chan=81:200:1:7 non_pref_chan=81:100:2:9 cell_capa=1 */
		/* non_pref_chan = <opClass : channelNumber : pref : reason> */

		field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress");

		stationsInfo = stationsInfoGet(field);

		if (stationsInfo == NULL)
		{
			printf("%s; AP-STA-CONNECTED; HELP_GETOBJPTR ==> 'MACAddress' (%s) not found! ==> create new record\n", __FUNCTION__, field);

			/* add a record at the beginning of the list */
			stationsInfo = (StationsInfo_t *)malloc(sizeof(StationsInfo_t));
			stationsInfo->nextStation = firstStationInfo;
			firstStationInfo = stationsInfo;

			/* New object - update */
			idx = interfaceIndexGet(ifname);
			if (idx == (-1))
			{
				printf("%s; ERROR: interfaceIdx of '%s' is %d ==> Abort!\n", __FUNCTION__, ifname, idx);
				return UGW_SUCCESS;
			}

			printf("%s; MACAddress= '%s'\n", __FUNCTION__, field);
			strcpy(stationsInfo->MACAddress, field);

			if (!strcmp(interfaceBandSteerInfo[idx].supportedFrequencyBands, "5GHz"))
			{
				printf("%s; set 'is_5G_supported' to 'true'\n", __FUNCTION__);
				stationsInfo->is_5G_supported = 1;  /*true*/
			}
			else
			{
				printf("%s; set 'is_5G_supported' to 'NON_VALID'\n", __FUNCTION__);
				stationsInfo->is_5G_supported = 2;  /* NON_VALID */
			}

			strcpy(stationsInfo->ifnameCheckIfConnected, "NONE");
			stationsInfo->numOfTicks = 0;
			stationsInfo->isBandSteeringPossible = true;
		}
		else
		{
			printf("%s; MACAddress ('%s') found! ==> check if it is already set as connected\n", __FUNCTION__, field);
			printf("%s; 'connectedTo' is '%s'\n", __FUNCTION__, stationsInfo->connectedTo);
			if ( (stationsInfo->connectedTo != NULL) && (!strncmp(stationsInfo->connectedTo, ifname, 5)) )
			{
				printf("%s; Station ('%s') already connected to the same i/f ('%s') ==> do NOT update the data-base\n", __FUNCTION__, stationsInfo->MACAddress, ifname);
				return UGW_SUCCESS;
			}

			/* Getting here means that the station record is present, and it is NOT connected to the same ifname (or no connected at all) */
			if ( strcmp(stationsInfo->connectedTo, "NONE") && strcmp(stationsInfo->connectedTo, "") )
			{  /* The station record is present, and it is NOT connected to the same ifname */
				ObjList *wlObjLocal = HELP_CREATE_OBJ(SOPT_OBJVALUE);

				printf("%s; Station ('%s') is connected to a DIFFERENT i/f ('%s') ==> call disassociate!\n", __FUNCTION__, stationsInfo->MACAddress, stationsInfo->connectedTo);

				if (wlObjLocal == NULL)
				{
					printf("%s; create DB (wlObjLocal) object ERROR\n", __FUNCTION__);
					return UGW_FAILURE;
				}

				if (!strcmp(stationsInfo->connectedTo, stationsInfo->ifnameCheckIfConnected))
				{
					printf("%s; Steering occurred! (MACAddress= '%s' to '%s')\n", __FUNCTION__, stationsInfo->MACAddress, stationsInfo->connectedTo);
					stationsInfo->numOfTicks = 0;
					strcpy(stationsInfo->ifnameCheckIfConnected, "NONE");
					stationsInfoListPrint();
				}

				help_addObjList(wlObjLocal, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
				HELP_EDIT_NODE(wlObjLocal, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddress", stationsInfo->MACAddress, 0, 0);

				/* Due to the fact that some stations does NOT send DISASSOCIATE when disconnecting, STA-DISCONNECTED won't arrive; the below will force it! */
				if (fapi_wlan_sta_disassociate(stationsInfo->connectedTo, wlObjLocal, 0) == UGW_FAILURE)
					printf("fapi_wlan_sta_disassociate ERROR\n");

				HELP_DELETE_OBJ(wlObjLocal, SOPT_OBJVALUE, FREE_OBJLIST);
			}

			/* update 'is_5G_supported' ONLY if it is NOT 'true' */
			if (stationsInfo->is_5G_supported != 1 /*true*/)
			{
				idx = interfaceIndexGet(ifname);
				if (idx == (-1))
				{
					printf("%s; ERROR: interfaceIdx of '%s' is %d ==> Abort!\n", __FUNCTION__, ifname, idx);
					return UGW_FAILURE;
				}

				if (!strcmp(interfaceBandSteerInfo[idx].supportedFrequencyBands, "5GHz"))
				{  /* it means that the band we are connected to is 5 GHz */
					printf("%s; connected to 5 GHz ==> update 'is_5G_supported' to 'true'\n", __FUNCTION__);
					stationsInfo->is_5G_supported = 1;  /*true*/
				}
			}
		}

		printf("%s; Update 'connectedTo' ('%s')\n", __FUNCTION__, (char *)ifname);
		strncpy(stationsInfo->connectedTo, (char *)ifname, 6);

		time(&rawtime);
		printf("%s; rawtime= %ld\n", __FUNCTION__, rawtime);

		stationsInfo->connectionTime = rawtime;

		field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "btm_supported");
		stationsInfo->btm_supported = false;
		if (field != NULL)
		{
			printf("%s; btm_supported is '%s'\n", __FUNCTION__, field);
			if (!strcmp(field, "true"))
			{
				printf("%s; btm_supported is 'true' ==> update 'btm_supported' and 'is_5G_supported' to 'true'\n", __FUNCTION__);
				stationsInfo->btm_supported = true;
				stationsInfo->is_5G_supported = 1;  /*true*/
			}
		}
	}
	else if (!strcmp(opCode, "AP-STA-DISCONNECTED"))
	{
		/* <3>AP-STA-DISCONNECTED wlan0 14:d6:4d:ac:36:70 */

		field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress");

		stationsInfo = stationsInfoGet(field);

		if (stationsInfo == NULL)
		{
			printf("%s; AP-STA-DISCONNECTED; HELP_GETOBJPTR ==> 'MACAddress' (%s) not found! ==> do NOT update the database\n", __FUNCTION__, field);
		}
		else
		{
			field = paramValueFromObjGet(wlObj, "Device.WiFi.SSID", "Name");
			if (!strncmp(field, stationsInfo->connectedTo, 5))
			{
				printf("%s; AP-STA-DISCONNECTED for the ifname ('%s') it is connected to ==> set 'connectedTo' to 'NONE'\n", __FUNCTION__, field);
				strcpy(stationsInfo->connectedTo, "NONE");
				stationsInfo->connectionTime = 0;

			}
			else
			{
				printf("%s; AP-STA-DISCONNECTED for the ifname ('%s') it is NOT connected to ('%s') ==> do NOT update the database\n", __FUNCTION__, field, stationsInfo->connectedTo);
			}
		}
	}
	else if (!strcmp(opCode, "BSS-TM-RESP"))
	{
		/* <3>BSS-TM-RESP wlan2 e4:9a:79:d2:6b:0b dialog_token=5 status_code=6 bss_termination_delay=0 target_bssid=12:ab:34:cd:56:10 */

		char *MACAddress = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress");
		char *target_bssid = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "target_bssid");
		char *status_code = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "status_code");

		printf("%s; Steering of MACAddress= '%s' to target_bssid= '%s' returned status_code= '%s'\n", __FUNCTION__, MACAddress, target_bssid, status_code);
		switch (atoi(status_code))
		{
			case 0:
				printf("%s; status_code= '%s' ==> WNM_BSS_TM_ACCEPT\n", __FUNCTION__, status_code);
				break;

			case 1:
				printf("%s; status_code= '%s' ==> WNM_BSS_TM_REJECT_UNSPECIFIED\n", __FUNCTION__, status_code);
				break;

			case 2:
				printf("%s; status_code= '%s' ==> WNM_BSS_TM_REJECT_INSUFFICIENT_BEACON\n", __FUNCTION__, status_code);
				break;

			case 3:
				printf("%s; status_code= '%s' ==> WNM_BSS_TM_REJECT_INSUFFICIENT_CAPABITY\n", __FUNCTION__, status_code);
				break;

			case 4:
				printf("%s; status_code= '%s' ==> WNM_BSS_TM_REJECT_UNDESIRED\n", __FUNCTION__, status_code);
				break;

			case 5:
				printf("%s; status_code= '%s' ==> WNM_BSS_TM_REJECT_DELAY_REQUEST\n", __FUNCTION__, status_code);
				break;

			case 6:
				printf("%s; status_code= '%s' ==> WNM_BSS_TM_REJECT_STA_CANDIDATE_LIST_PROVIDED\n", __FUNCTION__, status_code);
				break;

			case 7:
				printf("%s; status_code= '%s' ==> WNM_BSS_TM_REJECT_NO_SUITABLE_CANDIDATES\n", __FUNCTION__, status_code);
				break;

			case 8:
				printf("%s; status_code= '%s' ==> WNM_BSS_TM_REJECT_LEAVING_ESS\n", __FUNCTION__, status_code);
				break;

			default:
				printf("%s; Illegal status_code ('%s')\n", __FUNCTION__, status_code);
				break;
		}
	}	

	printf("\n%s; database AFTER event process:\n", __FUNCTION__);
	stationsInfoListPrint();

	return UGW_SUCCESS;
}


static void allBandsStationAllowSet(char *MACAddress)
{
	int i = 0;

	while (interfaceBandSteerInfo[i].ifname != NULL)
	{
		if ( (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, "2.4GHz")) ||
		     (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, "5GHz")) )
		{
			ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);

			if (wlObj == NULL)
			{
				printf("%s; create object ERROR\n", __FUNCTION__);
				continue;
			}

			help_addObjList(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddressControlList", MACAddress, 0, 0);

			printf("%s; Allowing back station (MACAddress= '%s') for interface '%s'\n",
			       __FUNCTION__, MACAddress, interfaceBandSteerInfo[i].ifname);

			if (fapi_wlan_sta_allow(interfaceBandSteerInfo[i].ifname, wlObj, 0) == UGW_FAILURE)
				printf("%s; fapi_wlan_sta_allow ERROR\n", __FUNCTION__);

			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		}

		i++;
	}
}


static void bandSteeringPerform(char *ifname, char *MACAddress, char *ifnameToSteerTo, char *BSSID_ToSteerTo, bool btm_supported)
{
	ObjList *wlObjSteer = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	printf("%s Entry; ifname= '%s', MACAddress= '%s', ifnameToSteerTo= '%s', BSSID_ToSteerTo= '%s', btm_supported= %d\n", __FUNCTION__, ifname, MACAddress, ifnameToSteerTo, BSSID_ToSteerTo, btm_supported);

	if (wlObjSteer == NULL)
	{
		printf("%s; create object (entry) ERROR ('wlObjSteer')\n", __FUNCTION__);
		return;
	}

	if (btm_supported == true)
	{
		char ChannelNumberToSteerTo[8];
		char neighbor[64];

		printf("%s; handle a BTM station (btm_supported= %d)\n", __FUNCTION__, btm_supported);

		help_addObjList(wlObjSteer, "Device.WiFi.AccessPoint.AssociatedDevice", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObjSteer, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress", MACAddress, 0, 0);  // Station's MAC Address

		help_addObjList(wlObjSteer, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObjSteer, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "pref", "1", 0, 0);
		HELP_EDIT_NODE(wlObjSteer, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "disassoc_imminent", "1", 0, 0);
		HELP_EDIT_NODE(wlObjSteer, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "disassoc_timer", "10", 0, 0);

		if (channelNumberGet(ifnameToSteerTo, ChannelNumberToSteerTo) == UGW_FAILURE)
			printf("%s; channelNumberGet returned error ==> use '1' for ChannelNumberToSteerTo\n", __FUNCTION__);
		else
			printf("%s; ChannelNumberToSteerTo= '%s'\n", __FUNCTION__, ChannelNumberToSteerTo);

		sprintf(neighbor, "%s,0,0,%s,0,255", BSSID_ToSteerTo, ChannelNumberToSteerTo);
		HELP_EDIT_NODE(wlObjSteer, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "0_neighbor", neighbor, 0, 0);

		printf("%s; send BSS_TM_REQ command; MACAddress= '%s', pref=1, disassoc_imminent=1, disassoc_timer=10, neighbor= '%s'\n", __FUNCTION__, MACAddress, neighbor);

		if (fapi_wlan_bss_transition_management_req(ifname, wlObjSteer, 0) == UGW_FAILURE)
		{
			printf("fapi_wlan_bss_transition_management_req ERROR\n");
			HELP_DELETE_OBJ(wlObjSteer, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}
	}
	else
	{
		printf("%s; handle a non-BTM station (btm_supported= %d)\n", __FUNCTION__, btm_supported);

		help_addObjList(wlObjSteer, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObjSteer, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddressControlList", MACAddress, 0, 0);

		if (fapi_wlan_sta_allow(ifnameToSteerTo, wlObjSteer, 0) == UGW_FAILURE)
		{
			printf("fapi_wlan_sta_allow ERROR\n");
			HELP_DELETE_OBJ(wlObjSteer, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Empty the ObjList */
		HELP_DELETE_OBJ(wlObjSteer, SOPT_OBJVALUE, EMPTY_OBJLIST);

		help_addObjList(wlObjSteer, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObjSteer, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddressControlList", MACAddress, 0, 0);

		if (fapi_wlan_sta_deny(ifname, wlObjSteer, 0) == UGW_FAILURE)
		{
			printf("fapi_wlan_sta_deny ERROR\n");
			HELP_DELETE_OBJ(wlObjSteer, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Empty the ObjList */
		HELP_DELETE_OBJ(wlObjSteer, SOPT_OBJVALUE, EMPTY_OBJLIST);

		help_addObjList(wlObjSteer, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObjSteer, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddress", MACAddress, 0, 0);

		if (fapi_wlan_sta_disassociate(ifname, wlObjSteer, 0) == UGW_FAILURE)
		{
			printf("fapi_wlan_sta_disassociate ERROR\n");
			HELP_DELETE_OBJ(wlObjSteer, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}
	}

	HELP_DELETE_OBJ(wlObjSteer, SOPT_OBJVALUE, FREE_OBJLIST);
}


static void bandSteeringIfNeededPerform(char *ifname, char *MACAddress, int signalStrengthThreshold_2_4, int signalStrengthThreshold_5, char *ifnameSteeredTo, bool btm_supported)
{
	ObjList *wlObj = NULL;
	char    *SignalStrength = NULL;
	char    *OperatingStandard = NULL;
	char    ifnameToSteerTo[8] = "\0";
	char    BSSID_ToSteerTo[32] = "\0";
	int     idx;
#if defined BAND_STEERING_TEST_MODE
	static int tempOffset = 0;  // for 'tweaking' the RSSI reports
#endif

	printf("%s; ifname= '%s', MACAddress= '%s', btm_supported= %d\n", __FUNCTION__, ifname, MACAddress, btm_supported);

	strcpy(ifnameSteeredTo, "NONE");

	wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObj == NULL)
	{
		printf("%s; create object ERROR ('wlObj')\n", __FUNCTION__);
		return;
	}

	help_addObjList(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress", MACAddress, 0, 0);

	if (fapi_wlan_sta_measurement_get(ifname, wlObj, 0) == UGW_FAILURE)
	{
		printf("%s; fapi_wlan_sta_measurement_get ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		strcpy(ifnameSteeredTo, "NON_VALID");
		return;
	}

	SignalStrength = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "SignalStrength");
	OperatingStandard = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "OperatingStandard");

	printf("%s; SignalStrength= '%s', OperatingStandard= '%s'\n", __FUNCTION__, SignalStrength, OperatingStandard);

	if ( (SignalStrength != NULL) && (OperatingStandard != NULL) )
	{
		printf("%s; atoi(SignalStrength)= %d, signalStrengthThreshold_2_4= %d, signalStrengthThreshold_5= %d\n",
		       __FUNCTION__, atoi(SignalStrength), signalStrengthThreshold_2_4, signalStrengthThreshold_5);

		idx = interfaceIndexGet(ifname);
		if (idx == (-1))
		{
			printf("%s; ERROR: interfaceIdx of '%s' is %d ==> Abort!\n", __FUNCTION__, ifname, idx);
			return;
		}

		printf("%s; supportedFrequencyBands= %s\n", __FUNCTION__, interfaceBandSteerInfo[idx].supportedFrequencyBands);

#if defined BAND_STEERING_TEST_MODE
		tempOffset += 8;
		printf("%s; atoi(SignalStrength)= %d, signalStrengthThreshold_2_4= %d, signalStrengthThreshold_5= %d\n",
		       __FUNCTION__, atoi(SignalStrength), signalStrengthThreshold_2_4, signalStrengthThreshold_5);
		printf("%s; supportedFrequencyBands= '%s' ==>\n", __FUNCTION__, interfaceBandSteerInfo[idx].supportedFrequencyBands);
		printf("%s; atoi(SignalStrength) + %d)= %d, signalStrengthThreshold_2_4= %d\n",
		       __FUNCTION__, tempOffset, (atoi(SignalStrength) + tempOffset), signalStrengthThreshold_2_4);
		printf("%s; atoi(SignalStrength) - %d)= %d, signalStrengthThreshold_5= %d\n",
		       __FUNCTION__, tempOffset, (atoi(SignalStrength) - tempOffset), signalStrengthThreshold_5);
		if ( ( (!strcmp(interfaceBandSteerInfo[idx].supportedFrequencyBands, "2.4GHz")) &&
		       ((atoi(SignalStrength) + tempOffset) > signalStrengthThreshold_2_4) ) ||
		     ( (!strcmp(interfaceBandSteerInfo[idx].supportedFrequencyBands, "5GHz")) &&
		       ((atoi(SignalStrength) - tempOffset) < signalStrengthThreshold_5) ) )
#else
		if ( ( (!strcmp(interfaceBandSteerInfo[idx].supportedFrequencyBands, "2.4GHz")) &&
		       (atoi(SignalStrength) > signalStrengthThreshold_2_4) ) ||
		     ( (!strcmp(interfaceBandSteerInfo[idx].supportedFrequencyBands, "5GHz")) &&
		       (atoi(SignalStrength) < signalStrengthThreshold_5) ) )
#endif
		{
#if defined BAND_STEERING_TEST_MODE
			tempOffset = 0;
#endif

			strcpy(ifnameToSteerTo, interfaceBandSteerInfo[idx].ifnameToSteerTo);
			strcpy(BSSID_ToSteerTo, interfaceBandSteerInfo[idx].BSSID_ToSteerTo);

			printf("%s; Perform Band-Steering (MACAddress= '%s' from '%s' to '%s')\n", __FUNCTION__, MACAddress, ifname, ifnameToSteerTo);
			printf("%s; ifname= '%s', ifnameToSteerTo= '%s', BSSID_ToSteerTo= '%s'\n", __FUNCTION__, ifname, ifnameToSteerTo, BSSID_ToSteerTo);

			strcpy(ifnameSteeredTo, ifnameToSteerTo);

			bandSteeringPerform(ifname, MACAddress, ifnameToSteerTo, BSSID_ToSteerTo, btm_supported);
		}
		else
		{
			printf("%s; (%s) Signal threshold: SignalStrength= %d, signalStrengthThreshold_2_4= %d, signalStrengthThreshold_5= %d ==> cont...\n",
			       __FUNCTION__, ifname, atoi(SignalStrength), signalStrengthThreshold_2_4, signalStrengthThreshold_5);
		}
	}

	HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
}


static int ap_manager_lite_band_steering_perform(int signalStrengthThreshold_2_4, int signalStrengthThreshold_5, int intervalInSeconds, int toleranceInSeconds, int numOfTicksAllowedForSteering)
{
	ObjList   *wlObj = NULL;
	ObjList   *wlObjInfo = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char      *paramValue = NULL;
	char      OperatingStandard[64] = "\0";
	char      ifnameSteeredTo[64] = "\0";
#if defined NEED_TO_BE_TESTED
	char      isLegacyBandSteeringTriedOnce[64] = "\0";
#endif
	int       secondsFromStaConnection, idx;
	time_t    rawtime;
	StationsInfo_t *stationsInfo = NULL;

	printf("\n%s Entry; signalStrengthThreshold_2_4= %d, signalStrengthThreshold_5= %d, intervalInSeconds= %d, toleranceInSeconds= %d, numOfTicksAllowedForSteering= %d\n",
	       __FUNCTION__, signalStrengthThreshold_2_4, signalStrengthThreshold_5, intervalInSeconds, toleranceInSeconds, numOfTicksAllowedForSteering);

	stationsInfo = firstStationInfo;
	while (stationsInfo != NULL)
	{
		printf("%s; MACAddress= '%s', connectedTo= '%s', connectionTime= %d, is_5G_supported= %d, ifnameCheckIfConnected= '%s', numOfTicks= %d, btm_supported= %d, isBandSteeringPossible= %d\n",
			   __FUNCTION__, stationsInfo->MACAddress, stationsInfo->connectedTo, stationsInfo->connectionTime, stationsInfo->is_5G_supported, stationsInfo->ifnameCheckIfConnected,
			   stationsInfo->numOfTicks, stationsInfo->btm_supported, stationsInfo->isBandSteeringPossible);

		if (stationsInfo->isBandSteeringPossible == false)
		{
			/* Band-Steering is impossible for this station */
			printf("%s; Station (MACAddress= '%s') can NOT be steered, it failed to do so many times before ==> do NOT check for band-steering. cont...\n",
				   __FUNCTION__, stationsInfo->MACAddress);

			stationsInfo = stationsInfo->nextStation;
			continue;
		}

		/* update 'is_5G_supported' ONLY if it is NOT 'true', and that the station is connected */
		if ( (stationsInfo->is_5G_supported != 1 /*true*/) && (strcmp(stationsInfo->connectedTo, "NONE")) )
		{
			wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
			if (wlObj == NULL)
			{
				printf("%s; create object ERROR ('wlObj')\n", __FUNCTION__);
				HELP_DELETE_OBJ(wlObjInfo, SOPT_OBJVALUE, FREE_OBJLIST);
				stationsInfo = stationsInfo->nextStation;
				continue;
			}

			help_addObjList(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress", stationsInfo->MACAddress, 0, 0);

			if (fapi_wlan_sta_measurement_get(stationsInfo->connectedTo, wlObj, 0) == UGW_FAILURE)
			{
				printf("%s; fapi_wlan_sta_measurement_get ERROR\n", __FUNCTION__);
				HELP_DELETE_OBJ(wlObjInfo, SOPT_OBJVALUE, FREE_OBJLIST);
				stationsInfo = stationsInfo->nextStation;
				continue;
			}

			paramValue = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "OperatingStandard");
			strcpy(OperatingStandard, paramValue);

			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);

			printf("%s; OperatingStandard= '%s'\n", __FUNCTION__, OperatingStandard);
			if (OperatingStandard == NULL)
			{
				printf("%s; OperatingStandard ('%s') not present ==> Abort!\n", __FUNCTION__, OperatingStandard);
				HELP_DELETE_OBJ(wlObjInfo, SOPT_OBJVALUE, FREE_OBJLIST);
				stationsInfo = stationsInfo->nextStation;
				continue;
			}

			printf("%s; *** is_5G_supported= %d ***\n", __FUNCTION__, stationsInfo->is_5G_supported);
			if (strchr(OperatingStandard, 'a') != NULL)
			{
				if (stationsInfo->is_5G_supported != 1 /*true*/)
				{
					stationsInfo->is_5G_supported = 1; /*true*/
					printf("%s; OperatingStandard ('%s') includes 'a' ==> supports 5 GHz; database:\n", __FUNCTION__, OperatingStandard);
					stationsInfoListPrint();
				}
				else
				{
					printf("%s; *** is_5G_supported= %d ==> do NOT check for update ***\n", __FUNCTION__, stationsInfo->is_5G_supported);
				}
			}
			else
			{
				printf("%s; OperatingStandard ('%s') does NOT include 'a' ==> NOT supporting 5 GHz\n", __FUNCTION__, OperatingStandard);
			}
		}

		printf("%s; connectedTo= '%s', is_5G_supported= %d, ifnameCheckIfConnected= '%s'\n",
			   __FUNCTION__, stationsInfo->connectedTo, stationsInfo->is_5G_supported, stationsInfo->ifnameCheckIfConnected);
		if ( (stationsInfo->ifnameCheckIfConnected != NULL) && (strcmp(stationsInfo->ifnameCheckIfConnected, "NONE")) && (strcmp(stationsInfo->ifnameCheckIfConnected, "")))
		{
			printf("%s; ifnameCheckIfConnected= '%s' ==> do NOT check for band steering; cont...\n", __FUNCTION__, stationsInfo->ifnameCheckIfConnected);
		}
		else
		{
			printf("%s; ifnameCheckIfConnected= '%s' ==> check for band steering\n", __FUNCTION__, stationsInfo->ifnameCheckIfConnected);

			if ( ( (stationsInfo->connectedTo != NULL) && (!strcmp(stationsInfo->connectedTo, "NONE")) ) ||
				 (stationsInfo->connectedTo == NULL) ||
				 (stationsInfo->is_5G_supported == 0 /*false*/) )
			{
				/* There is no connection, or 5 GHz is not supported */
				printf("%s; connectedTo= '%s', is_5G_supported= %d ==> do NOT check for band-steering. cont...\n",
					   __FUNCTION__, stationsInfo->connectedTo, stationsInfo->is_5G_supported);
			}
			else if ( (strcmp(stationsInfo->connectedTo, "NONE")) && (stationsInfo->is_5G_supported == 2 /*NON_VALID*/) )
			{
				/* There is a connection, and 5 GHz supported is unknown yet! */
				printf("%s; connectedTo= '%s', is_5G_supported= %d ==> try to switch to 5 GHz band\n",
					   __FUNCTION__, stationsInfo->connectedTo, stationsInfo->is_5G_supported);

				idx = interfaceIndexGet(stationsInfo->connectedTo);
				if (idx == (-1))
				{
					printf("%s; ERROR: interfaceIdx of '%s' is %d ==> Abort!\n", __FUNCTION__, stationsInfo->connectedTo, idx);
					stationsInfo = stationsInfo->nextStation;
					continue;
				}

				printf("%s; ifnameToSteerTo= '%s', BSSID_ToSteerTo= '%s'\n", __FUNCTION__, interfaceBandSteerInfo[idx].ifnameToSteerTo, interfaceBandSteerInfo[idx].BSSID_ToSteerTo);

				bandSteeringPerform(stationsInfo->connectedTo, stationsInfo->MACAddress, interfaceBandSteerInfo[idx].ifnameToSteerTo, interfaceBandSteerInfo[idx].BSSID_ToSteerTo, stationsInfo->btm_supported);

				stationsInfo->numOfTicks = 0;
				strcpy(stationsInfo->ifnameCheckIfConnected, interfaceBandSteerInfo[idx].ifnameToSteerTo);  /* Update ifnameCheckIfConnected */
			}
			else
			{
				time(&rawtime);
				secondsFromStaConnection = rawtime - stationsInfo->connectionTime;
				printf("%s; rawtime= %ld, connectionTime= %d ==> secondsFromStaConnection= %d\n", __FUNCTION__, rawtime, stationsInfo->connectionTime, secondsFromStaConnection);

				if (secondsFromStaConnection > toleranceInSeconds)
				{
					printf("%s; If needed, perform Band-Steering: Connected to '%s', MACAddress= '%s'\n", __FUNCTION__, stationsInfo->connectedTo, stationsInfo->MACAddress);

					bandSteeringIfNeededPerform(stationsInfo->connectedTo, stationsInfo->MACAddress, signalStrengthThreshold_2_4, signalStrengthThreshold_5, ifnameSteeredTo, stationsInfo->btm_supported);

					printf("%s; *** MACAddress= '%s', ifnameSteeredTo= '%s' ***\n", __FUNCTION__, stationsInfo->MACAddress, ifnameSteeredTo);
					if (strcmp(ifnameSteeredTo, "NON_VALID"))
					{
						strcpy(stationsInfo->ifnameCheckIfConnected, ifnameSteeredTo);  /* Update ifnameCheckIfConnected */
					}
				}
				else
				{
					printf("%s; secondsFromStaConnection (%d) <= toleranceInSeconds (%d) ==> do NOT check for band-steering. cont...\n", __FUNCTION__, secondsFromStaConnection, toleranceInSeconds);
				}
			}
		}

		printf("%s; *** MACAddress= '%s', is_5G_supported= %d, connectedTo= '%s', ifnameCheckIfConnected= '%s', btm_supported= %d ***\n",
			   __FUNCTION__, stationsInfo->MACAddress, stationsInfo->is_5G_supported, stationsInfo->connectedTo, stationsInfo->ifnameCheckIfConnected, stationsInfo->btm_supported);

		if ( (stationsInfo->is_5G_supported != 0 /*false*/) /* 'true' or 'NON_VALID' */ &&
			 (stationsInfo->connectedTo != NULL) &&
			 (strcmp(stationsInfo->ifnameCheckIfConnected, "NONE")) &&
			 (strcmp(stationsInfo->ifnameCheckIfConnected, "")) )
		{  /* The ifnameCheckIfConnected is NOT "NONE", meaning, there was a band-steering on this station */
			printf("%s; *** numOfTicks= %d; (connectedTo= '%s', ifnameCheckIfConnected= '%s') ***\n", __FUNCTION__, stationsInfo->numOfTicks, stationsInfo->connectedTo, stationsInfo->ifnameCheckIfConnected);

			if (!strcmp(stationsInfo->connectedTo, stationsInfo->ifnameCheckIfConnected))
			{
				printf("%s; Steering occurred! (MACAddress= '%s' to '%s')\n", __FUNCTION__, stationsInfo->MACAddress, stationsInfo->connectedTo);
				stationsInfo->numOfTicks = 0;
				strcpy(stationsInfo->ifnameCheckIfConnected, "NONE");
				stationsInfoListPrint();
#if defined NEED_TO_BE_TESTED
				HELP_EDIT_SELF_NODE(tmpObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "isLegacyBandSteeringTriedOnce", "false", 0, 0);
#endif
			}
			else
			{
				printf("%s; Steering (MACAddress= '%s' from '%s' to '%s') did NOT occur! (check %d out of %d)\n",
				       __FUNCTION__, stationsInfo->MACAddress, stationsInfo->connectedTo, stationsInfo->ifnameCheckIfConnected, stationsInfo->numOfTicks+1, numOfTicksAllowedForSteering);

				if (stationsInfo->numOfTicks >= (numOfTicksAllowedForSteering - 1))
				{
#if defined NEED_TO_BE_TESTED
					bool isSpecialCase = false;
#endif
					printf("%s; PROBLEM!!! Steering did NOT work!!! (MACAddress= '%s' from '%s' to '%s')\n",
					       __FUNCTION__, stationsInfo->MACAddress, stationsInfo->connectedTo, stationsInfo->ifnameCheckIfConnected);

					stationsInfo->numOfTicks = 0;
					strcpy(stationsInfo->ifnameCheckIfConnected, "NONE");

					if (stationsInfo->btm_supported == true)
					{
						printf("%s; btm_supported= %d ==> do not set 'isBandSteeringPossible' to 'false'\n", __FUNCTION__, stationsInfo->btm_supported);
						printf("%s; Steering did NOT occur! MACAddress= '%s' from '%s' to '%s' (BTM supported station)\n",
						       __FUNCTION__, stationsInfo->MACAddress, stationsInfo->connectedTo, stationsInfo->ifnameCheckIfConnected);
						printf("%s; btm_supported= %d ==> it should NOT try to steer for %d seconds period of time\n", __FUNCTION__, stationsInfo->btm_supported, toleranceInSeconds);
#if defined NEED_TO_BE_TESTED
						printf("%s; isLegacyBandSteeringTriedOnce= '%s'\n", __FUNCTION__, isLegacyBandSteeringTriedOnce);

						if ( (!strcmp(isLegacyBandSteeringTriedOnce, "")) || (!strcmp(isLegacyBandSteeringTriedOnce, "false")) )  // if legacy steering did NOT occured once
						{
							isSpecialCase = true;

							/* perform legacy steering; update that legacy steeering occured once */
							printf("%s; Special case: Steering of btm_supported station failed ==> try ONE TIME Legacy-Steering (black/white list)\n", __FUNCTION__);
							HELP_EDIT_SELF_NODE(tmpObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "isLegacyBandSteeringTriedOnce", "true", 0, 0);

							stationsInfo->numOfTicks = 0;
							HELP_EDIT_SELF_NODE(tmpObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "numOfTicks", "0", 0, 0);

							idx = interfaceIndexGet(stationsInfo->connectedTo);
							if (idx == (-1))
							{
								printf("%s; ERROR: interfaceIdx of '%s' is %d ==> Abort!\n", __FUNCTION__, connectedTo, idx);
								return UGW_FAILURE;
							}

							printf("%s; Special case; connectedTo= '%s', idx= %d\n", __FUNCTION__, stationsInfo->connectedTo, idx);
							bandSteeringPerform(stationsInfo->connectedTo, stationsInfo->MACAddress, interfaceBandSteerInfo[idx].ifnameToSteerTo, interfaceBandSteerInfo[idx].BSSID_ToSteerTo, "false");
						}
						else
#endif
						{
							time(&rawtime);
							printf("%s; Reset station's connection time (rawtime= %ld)\n", __FUNCTION__, rawtime);
							stationsInfo->connectionTime = rawtime;
						}
					}
					else
					{
						stationsInfo->isBandSteeringPossible = false;
					}

#if defined NEED_TO_BE_TESTED
					printf("%s; isSpecialCase= %d\n", __FUNCTION__, isSpecialCase);

					/* Allow back the station for ALL active bands */
					if (isSpecialCase == false)
#endif
					{
						if (stationsInfo->btm_supported == false)
						{
							allBandsStationAllowSet(stationsInfo->MACAddress);
						}
					}

					/* In case that band steering failed, and 'is_5G_supported' is 'NON_VALID', it means that steering to 5 GHz failed ==> mark 'is_5G_supported' to 'false' */
					if (stationsInfo->is_5G_supported == 2 /*NON_VALID*/)
					{
						printf("%s; Steering to 5 GHz band for a station which is unknown if supporting dual-band failed ==> set is_5G_supported to 'false'\n", __FUNCTION__);
						stationsInfo->is_5G_supported = 0;  /*false*/
					}
				}
				else
				{
					stationsInfo->numOfTicks++;
					printf("%s; increment the counter ==> numOfTicks= %d\n", __FUNCTION__, stationsInfo->numOfTicks);
				}

				printf("%s; Steering did NOT occur; MACAddress= '%s' from '%s' to '%s' ==> database:\n",
					   __FUNCTION__, stationsInfo->MACAddress, stationsInfo->connectedTo, stationsInfo->ifnameCheckIfConnected);
				stationsInfoListPrint();
			}
		}

		stationsInfo = stationsInfo->nextStation;
	}

	HELP_DELETE_OBJ(wlObjInfo, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}


static int ifnameBssidToSteerToSet(int recordIdx, char *FrequencyBandToSteerTo)
{
	int  i = 0;
	char BSSID[] = "00:11:22:33:44:55";

	printf("%s; recordIdx= %d, FrequencyBandToSteerTo= '%s'\n", __FUNCTION__, recordIdx, FrequencyBandToSteerTo);

	while (interfaceBandSteerInfo[i].ifname != NULL)
	{
		if (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, FrequencyBandToSteerTo))
		{
			strcpy(interfaceBandSteerInfo[recordIdx].ifnameToSteerTo, interfaceBandSteerInfo[i].ifname);

			if (bssidGet(interfaceBandSteerInfo[i].ifname, BSSID) == UGW_FAILURE)
			{
				printf("%s; bssidGet() ERROR\n", __FUNCTION__);
				return UGW_FAILURE;
			}
			strcpy(interfaceBandSteerInfo[recordIdx].BSSID_ToSteerTo, BSSID);
		}

		i++;
	}

	printf("%s; [idx= %d] ifname= '%s', ifnameToSteerTo= '%s', BSSID= '%s'\n",
	       __FUNCTION__, recordIdx, interfaceBandSteerInfo[recordIdx].ifname, interfaceBandSteerInfo[recordIdx].ifnameToSteerTo, interfaceBandSteerInfo[recordIdx].BSSID_ToSteerTo);

	return UGW_SUCCESS;
}


static int interfaceBandSteerInfoSet(void)
{
	int       i = 0;
	ObjList   *wlObjInfo = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char      *operationMode;
	char      *supportedFrequencyBands;

	if (wlObjInfo == NULL)
	{
		printf("%s; create DB Info object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	while (interfaceBandSteerInfo[i].ifname != NULL)
	{
		if (fapi_wlan_interface_info_get(interfaceBandSteerInfo[i].ifname, wlObjInfo, 0) == UGW_SUCCESS)
		{
			printf("\n%s; output obj from fapi_wlan_interface_info_get:\n", __FUNCTION__);
			setLog(interfaceBandSteerInfo[i].ifname, wlObjInfo, 0);
			printf("\n");

			operationMode = paramValueFromObjGet(wlObjInfo, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "OperationMode");
			supportedFrequencyBands = paramValueFromObjGet(wlObjInfo, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "SupportedFrequencyBands");
			printf("%s; interfaceBandSteerInfo[%d].ifname= '%s'; operationMode= '%s', supportedFrequencyBands= '%s'\n",
				   __FUNCTION__, i, interfaceBandSteerInfo[i].ifname, operationMode, supportedFrequencyBands);

			if ( (operationMode != NULL) && (supportedFrequencyBands != NULL) )
			{
				if (!strcmp(operationMode, "AP"))
				{
					if (!strcmp(supportedFrequencyBands, "2.4GHz"))
					{
						printf("%s; set support_2_4 to 'true'\n", __FUNCTION__);
						strcpy(interfaceBandSteerInfo[i].supportedFrequencyBands, "2.4GHz");
					}

					if (!strcmp(supportedFrequencyBands, "5GHz"))
					{
						printf("%s; set support_5 to 'true'\n", __FUNCTION__);
						strcpy(interfaceBandSteerInfo[i].supportedFrequencyBands, "5GHz");
					}
				}
			}
		}

		i++;
	}

	return UGW_SUCCESS;
}


static int ap_manager_lite_band_steering_init(void)
{
	ObjList   *wlObjInfo = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	bool      support_2_4 = false, support_5 = false;
	char      command[64] = "\0";
	int       i=0;

	printf("%s Entry\n", __FUNCTION__);

	if (wlObjInfo == NULL)
	{
		printf("%s; create DB Info object ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObjInfo, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	printf("%s; numOfActiveApInterfaces= %d\n", __FUNCTION__, numOfActiveApInterfaces);
	if (numOfActiveApInterfaces < 2)
	{
		printf("%s; Less than two APs (%d) are present ==> do NOT check for band-steering. Quit!\n\n", __FUNCTION__, numOfActiveApInterfaces);
		HELP_DELETE_OBJ(wlObjInfo, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	/* Set the band-steering data-base */
	if (interfaceBandSteerInfoSet() == UGW_FAILURE)
	{
		printf("%s; create Band-Steering DB Info object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	i = 0;
	while (interfaceBandSteerInfo[i].ifname != NULL)
	{
		if (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, "2.4GHz"))
			support_2_4 = true;
		else if (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, "5GHz"))
			support_5 = true;

		i++;
	}

	if ( (support_2_4 == false) || (support_5 == false) )
	{
		printf("%s; one (or more) of the bands (2.4 & 5 GHz) are not available ==> do NOT check for band-steering. Quit!\n\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObjInfo, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	i = 0;
	while (interfaceBandSteerInfo[i].ifname != NULL)
	{
		if ( (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, "2.4GHz")) ||
		     (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, "5GHz")) )
		{
			/* Turn all radios off - in case there are connected stations, it'll be disconnected */
			sprintf(command, "iwpriv %s sEnableRadio 0", interfaceBandSteerInfo[i].ifname);
			printf("%s; turn radios off; command= '%s'\n", __FUNCTION__, command);
			system(command);

			if (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, "2.4GHz"))
			{
				if (ifnameBssidToSteerToSet(i, "5GHz") == UGW_FAILURE)
				{
					printf("%s; ifnameBssidToSteerToSet returned error ==> Quit!\n\n", __FUNCTION__);
					HELP_DELETE_OBJ(wlObjInfo, SOPT_OBJVALUE, FREE_OBJLIST);
					return UGW_FAILURE;
				}
			}

			if (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, "5GHz"))
			{
				if (ifnameBssidToSteerToSet(i, "2.4GHz") == UGW_FAILURE)
				{
					printf("%s; ifnameBssidToSteerToSet returned error ==> Quit!\n\n", __FUNCTION__);
					HELP_DELETE_OBJ(wlObjInfo, SOPT_OBJVALUE, FREE_OBJLIST);
					return UGW_FAILURE;
				}
			}
		}

		i++;
	}

	/* Turn all radios back on */
	sleep(1);  /* wait 1 second in order to make sure that the listener is already available */
	i = 0;
	while (interfaceBandSteerInfo[i].ifname != NULL)
	{
		if ( (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, "2.4GHz")) ||
		     (!strcmp(interfaceBandSteerInfo[i].supportedFrequencyBands, "5GHz")) )
		{
			sprintf(command, "iwpriv %s sEnableRadio 1", interfaceBandSteerInfo[i].ifname);
			printf("%s; turn radios on; command= '%s'\n", __FUNCTION__, command);
			system(command);
		}

		i++;
	}

	HELP_DELETE_OBJ(wlObjInfo, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}


static void radioInterfaceDataBaseUpdate(void)
{
	size_t  numOfInterfaces = sizeof(radioInterface) / sizeof(RadioInterface);
	int     i;
	char    wpaCtrlName[32];

	for (i=0; i < (int)numOfInterfaces; i++)
	{
		/* check if '/var/run/hostapd/wlanX' or '/var/run/wpa_supplicant/wlanX' exists */
		sprintf(wpaCtrlName, "%s%s", "/var/run/hostapd/", radioInterface[i].name);
		if (access(wpaCtrlName, F_OK) == 0)
		{
			printf("%s; Radio '%s' exists - AP Mode\n", __FUNCTION__, radioInterface[i].name);
			strcpy(radioInterface[i].operationMode, "AP");
			strcpy(radioInterface[i].wpaCtrlName, wpaCtrlName);
			numOfActiveApInterfaces++;
		}
		else
		{
			sprintf(wpaCtrlName, "%s%s", "/var/run/wpa_supplicant/", radioInterface[i].name);
			if (access(wpaCtrlName, F_OK) == 0)
			{
				printf("%s; Radio '%s' exists - STA Mode\n", __FUNCTION__, radioInterface[i].name);
				strcpy(radioInterface[i].operationMode, "STA");
				strcpy(radioInterface[i].wpaCtrlName, wpaCtrlName);
			}
		}
	}
}


static int wpaCtrlReconnectHandle(const char *ifname, struct wpa_ctrl *wpaSocket, int highestValFD)
{
	int fd, count = 0;

	/* Notify the client that the WLAN has disconnected */
	fapiWlanCallBackBandSteeringFunc("WLAN-DISCONNECTED", ifname, NULL, 0, NULL);

	/* wait for the hostapd - it removes the fd */
	sleep(1);

	fapi_wlan_stop_monitoring(ifname, NULL, 0);

	while (1)
	{
		if (fapi_wlan_start_monitoring(ifname, NULL, 1) == UGW_SUCCESS)
		{
			printf("%s; connection i/f is back! (ifname= '%s')\n", __FUNCTION__, ifname);
			break;
		}

		usleep (350);

		if (count++ >= 5000)
		{
			printf("%s; trying to reconnect (ifname= '%s')\n", __FUNCTION__, ifname);
			count = 0;
		}
	}

	if (fapi_wlan_hostapd_socket_get(ifname, (void**)&wpaSocket) == UGW_FAILURE)
		printf("%s; ERROR: fapi_wlan_hostapd_socket_get on '%s' failed!\n", __FUNCTION__, ifname);
	else if (wpa_ctrl_attach(wpaSocket) != 0)
	{
		printf("%s; ERROR: wpa_ctrl_attach for '%s' failed!\n", __FUNCTION__, ifname);
	}
	else
	{
		fd = wpa_ctrl_get_fd(wpaSocket);
		highestValFD = (fd > highestValFD)? fd : highestValFD;  /* find the highest value fd */
	}

	if (fapi_wlan_hostapd_callback_register(ifname, fapiWlanCallBackBandSteeringFunc, NULL) == UGW_FAILURE)
		printf("fapi_wlan_hostapd_callback_register Band-Steering ERROR (ifname= '%s')\n", ifname);

	/* Notify the client that the WLAN has reconnected */
	printf("%s; call fapiWlanCallBackBandSteeringFunc with 'WLAN-RECONNECTED' (ifname= '%s')\n", __FUNCTION__, ifname);
	fapiWlanCallBackBandSteeringFunc("WLAN-RECONNECTED", ifname, NULL, 0, NULL);
	
	return highestValFD;
}


/**********************************/
/* API Functions for external use */
/**********************************/
void fapiBandSteeringAppStart(int signalStrengthThreshold_2_4, int signalStrengthThreshold_5, int intervalInSeconds, int toleranceInSeconds, int numOfTicksAllowedForSteering)
{
	size_t  numOfInterfaces = sizeof(radioInterface) / sizeof(RadioInterface);
	int     i, interfaceIdx, highestValFD = 0, fd[numOfInterfaces], res, numOfActiveInterfaces = 0, res_wpa_ctrl;
	char    *interfaces[numOfInterfaces];
	fd_set  rfds;
	struct  timeval tv;
	bool    isReportReceived = false, isCliRunning = true;
	struct  wpa_ctrl *wpaSocket[numOfInterfaces];

	printf("%s; START; signalStrengthThreshold_2_4= %d, signalStrengthThreshold_5= %d, intervalInSeconds= %d, toleranceInSeconds= %d\n",
	       __FUNCTION__, signalStrengthThreshold_2_4, signalStrengthThreshold_5, intervalInSeconds, toleranceInSeconds);

	/* Update the data-base with the info regarding of the active radio interfaces */
	radioInterfaceDataBaseUpdate();

	for (i=0; i < (int)numOfInterfaces; i++)
	{
		if (!strcmp(radioInterface[i].operationMode, "NONE"))
		{
			printf("%s; radio interface '%s' not present ==> Continue\n", __FUNCTION__, radioInterface[i].name);
			continue;
		}

		printf("%s; supportedInterfaces[%d]= '%s'\n", __FUNCTION__, i, radioInterface[i].name);

		printf("%s; register to Band-Steering call-back (ifname= '%s')\n", __FUNCTION__, radioInterface[i].name);
		if (fapi_wlan_hostapd_callback_register(radioInterface[i].name, fapiWlanCallBackBandSteeringFunc, NULL) == UGW_FAILURE)
			printf("fapi_wlan_hostapd_callback_register ERROR (ifname= '%s')\n", radioInterface[i].name);

		if (fapi_wlan_start_monitoring(radioInterface[i].name, NULL, 0) == UGW_FAILURE)
			printf("fapi_wlan_start_monitoring ERROR (ifname= '%s')\n", radioInterface[i].name);

		/* Read the hostapd socket fd */
		wpaSocket[i] = NULL;  /* reset the Socket */
		if (fapi_wlan_hostapd_socket_get(radioInterface[i].name, (void**)&wpaSocket[i]) == UGW_FAILURE)
		{
			printf("%s; ERROR: fapi_wlan_hostapd_socket_get on '%s' failed!\n", __FUNCTION__, radioInterface[i].name);
		}
		else if (wpa_ctrl_attach(wpaSocket[i]) != 0)
		{
			printf("%s; ERROR: wpa_ctrl_attach for '%s' failed!\n", __FUNCTION__, radioInterface[i].name);
		}
		else
		{
			fd[numOfActiveInterfaces] = wpa_ctrl_get_fd(wpaSocket[i]);
			highestValFD = (fd[numOfActiveInterfaces] > highestValFD)? fd[numOfActiveInterfaces] : highestValFD;  /* find the highest value fd */
			interfaces[numOfActiveInterfaces++] = radioInterface[i].name;  /* store the ifname vs. its index */
		}
	}

	if (numOfActiveInterfaces == 0)
	{
		printf("\nBeeRock CLI: No active interfaces! ==> Quit Band Steering app.\n");
		isCliRunning = false;
	}

	ap_manager_lite_band_steering_init();

	/* Main event loop */
	while (isCliRunning)
	{
		for (i=0; i < numOfActiveInterfaces; i++)
		{
			FD_SET(fd[i], &rfds);
		}

		/* Wait up to "intervalInSeconds" seconds. */
		tv.tv_sec = intervalInSeconds;
		tv.tv_usec = 0;

		res = select(highestValFD + 1, &rfds, NULL, NULL, &tv);
		if (res < 0)
		{
			printf("%s; select() return value= %d ==> cont...; errno= %d ('%s')\n", __FUNCTION__, res, errno, strerror(errno));
			continue;
		}

		isReportReceived = false;

		for (i=0; i < numOfActiveInterfaces; i++)
		{
			if (FD_ISSET(fd[i], &rfds))
			{
				//printf("%s; *** [%s] FAPI RECEIVED FROM HOSTAPD SOMETHING!!! ***\n", __FUNCTION__, interfaces[i]);
				interfaceIdx = interfaceIndexGet(interfaces[i]);
				if (interfaceIdx == (-1))
				{
					printf("ERROR: interfaceIdx of '%s' is %d ==> Skip this interface\n", interfaces[i], interfaceIdx);
					continue;
				}

				while (1)
				{
					res_wpa_ctrl = wpa_ctrl_pending(wpaSocket[interfaceIdx]);
					if (res_wpa_ctrl != 1)
						break;  /* quit the 'while' loop */

					res = fapi_wlan_hostapd_report_process(interfaces[i], NULL, NULL);
					if (res == UGW_FAILURE)
						printf("ERROR: fapi_wlan_hostapd_report_process failed!\n");
					else
					{
						isReportReceived = true;
						if (res == WPA_CTRL_DISCONNECT)
						{  /* handle hostapd recover mechanism */
							printf("wpa-ctrl disconnected - try reconnecting...\n");
							highestValFD = wpaCtrlReconnectHandle(interfaces[i], wpaSocket[interfaceIdx], highestValFD);
							i = numOfActiveInterfaces;  /* quit the 'for' loop */
							break;  /* quit the 'while' loop */
						}
					}
				}

				if ( res_wpa_ctrl == (-1) )
				{  /* ERROR - issue a trace */
					printf("wpa_ctrl_pending() returned ERROR\n");
				}
			}
		}

#if defined BAND_STEERING_TEST_MODE
		printf("\n===================================================================\n");
		printf("=======================  T E S T   M O D E  =======================\n");
		printf("===================================================================\n\n");
#endif

		if (!isReportReceived)
		{
			printf("%s; Report does NOT received ==> check if band steering is needed\n", __FUNCTION__);
			ap_manager_lite_band_steering_perform(signalStrengthThreshold_2_4, signalStrengthThreshold_5, intervalInSeconds, toleranceInSeconds, numOfTicksAllowedForSteering);
			printf("%s; sleep %d seconds...\n\n", __FUNCTION__, intervalInSeconds);  /* Performed by the above select() */
			sleep(intervalInSeconds);
		}
		else
		{
			printf("%s; Report received ==> handle it...\n", __FUNCTION__);
		}
	}

	/* Cleanup */
	printf("\nFAPI BeeRock CLI cleanup...\n");

	for (i=0; i < numOfActiveInterfaces; i++)
	{
		interfaceIdx = interfaceIndexGet(interfaces[i]);
		if (interfaceIdx == (-1))
		{
			printf("ERROR: interfaceIdx of '%s' is %d ==> Skip this interface\n", interfaces[i], interfaceIdx);
			continue;
		}

		if (wpaSocket[interfaceIdx] != NULL)
			wpa_ctrl_detach(wpaSocket[interfaceIdx]);

		fapi_wlan_stop_monitoring(interfaces[i], NULL, 0);
	}

	stationsInfoListClear();

	printf("Bye!\n");
}
