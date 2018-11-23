/*##################################################################################################
# "Copyright (c) 2013 Intel Corporation                                                            #
# DISTRIBUTABLE AS SAMPLE SOURCE SOFTWARE                                                          #
# This Distributable As Sample Source Software is subject to the terms and conditions              #
# of the Intel Software License Agreement for the Intel(R) Cable and GW Software Development Kit"  #
##################################################################################################*/

/*! \file 	wlan_wlan_beerock_aml.c
    \brief 	This file implements the interface between the HostAPD and the FAPI.
    \todo 	Add license header
	\todo   Move all system() function uses to clean C functions
	\todo   merge all db load\store functions to one
	\todo   Solve all mac addresses workarounds PUMA \ UBOOT  \ UGW
	\todo   Create script that align DB sources to UGW version and converts to RPC format, a must!!!!!
	\todo   Add null pointer verifications and clean-up 
 */

#include "fapi_wlan_beerock.h"
#include "fapi_wlan_beerock_aml.h"
#include "help_objlist.h"

#include <unistd.h>
#include <signal.h>
#include <time.h>

#define PRIORITY_INTERNAL_SET
//#define BAND_STEERING_TEST_MODE


typedef struct
{
	char *ifname;
	char supportedFrequencyBands[16];
	char ifnameToSteerTo[8];
	char BSSID_ToSteerTo[24];
} InterfaceBandSteerInfo;

typedef struct
{
	char    *ifname;
	timer_t timerid;
} TimerIdPerIfname;

int numOfActiveApInterfaces = 0;
static timer_t timerid;

static TimerIdPerIfname timerIdPerIfnameInfo[] =
{
	{ "wlan0", 0 },
	{ "wlan1", 0 },
	{ "wlan2", 0 },
	{ "wlan3", 0 },
	{ "wlan4", 0 },
	{ "wlan5", 0 },
	{ NULL,    0 }
};


/*********************************************/
/* static Functions for general internal use */
/*********************************************/
static int ownBssNeighborDataBaseUpdate(const char *ifname, ObjList *wlObj, char *BSSID)
{
	char      *channel;
	char      channelNumber[] = "1234";
	char      fullPath[MAX_LEN_PARAM_VALUE];
	ObjList   *subObjDB;

	subObjDB = subObjForTwoSameFieldsGet(wlObj, BSSID, BSSID);
	if (subObjDB == NULL)
	{  /* There is no object with this BSSID & nr_BSSID ==> exit */
		printf("%s; NOT FOUND RECORD, nothing to update ==> exit\n", __FUNCTION__);
		return UGW_SUCCESS;
	}

	channel = paramValueFromSubObjGet(subObjDB, "channelNumber");
	printf("%s; channelNumber= '%s'\n", __FUNCTION__, channel);
	if (channel == NULL)
	{  /* 'channelNumber' is not present ==> exit */
		printf("%s; 'channelNumber' record NOT FOUND, nothing to update ==> exit\n", __FUNCTION__);
		return UGW_SUCCESS;
	}

	/* Get the current channel number */
	if (channelNumberGet(ifname, channelNumber) == UGW_FAILURE)
	{
		printf("%s; channelNumberGet ERROR ==> use the original one!\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if (!strcmp(channel, channelNumber))
	{
		printf("%s; current channel ('%s') and stored channel ('%s') are the same ==> nothing to update\n", __FUNCTION__, channelNumber, channel);
		return UGW_SUCCESS;
	}

	printf("%s; current channel ('%s') is DIFFERENT than stored channel ('%s') ==> UPDATE the neighbor data-base\n", __FUNCTION__, channelNumber, channel);

	sprintf(fullPath, "/tmp/wlan_wave/%s_%s", "BeeRock.BSS.Neighbor", ifname);
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);

	printf("%s; Stored Data Base Object ('%s'):\n", __FUNCTION__, fullPath);
	setLog(ifname, wlObj, 0);

	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "channelNumber", channelNumber, 0, 0);

	printf("%s; Data Base Object to be stored ('%s'):\n", __FUNCTION__, fullPath);
	setLog(ifname, wlObj, 0);

	if (HELP_STORELOCALDB(wlObj, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_STORELOCALDB Error!\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	return UGW_SUCCESS;
}


static int ifnameOfTimerIdGet(timer_t timerId, char *ifname)
{
	int i = 0;

	while (timerIdPerIfnameInfo[i].ifname != NULL)
	{
		if (timerIdPerIfnameInfo[i].timerid == timerId)
		{
			printf("%s; ifname ('%s') found! timerId= %d\n", __FUNCTION__, timerIdPerIfnameInfo[i].ifname, (int)timerId);
			strcpy(ifname, timerIdPerIfnameInfo[i].ifname);
			timerIdPerIfnameInfo[i].timerid = 0;  /* reset the timerid */

			return UGW_SUCCESS;
		}

		i++;
	}

	return UGW_FAILURE;
}


static int ifnameOfTimerIdSet(timer_t timerId, const char *ifname)
{
	int i = 0;

	while (timerIdPerIfnameInfo[i].ifname != NULL)
	{
		if (!strcmp(timerIdPerIfnameInfo[i].ifname, ifname))
		{
			printf("%s; ifname ('%s') found! timerId= %d\n", __FUNCTION__, timerIdPerIfnameInfo[i].ifname, (int)timerId);
			timerIdPerIfnameInfo[i].timerid = timerId;

			return UGW_SUCCESS;
		}

		i++;
	}

	return UGW_FAILURE;
}


static void timerExpireCB(int sig, siginfo_t *si, void *uc)
{
	(void)uc;

	if(si->si_value.sival_ptr != &timerid)
	{
		printf("Stray signal\n");
	}
	else
	{
		char ifname[8];
		char command[32];

		printf("%s; Caught signal %d from timer ==> disable the radio & fapi_wlan_beerock_cli\n", __FUNCTION__, sig);
		system("killall fapi_wlan_beerock_cli");  /* Kill the listener */

		if (ifnameOfTimerIdGet(timerid, ifname) == UGW_SUCCESS)
		{
			sprintf(command, "iwpriv %s sEnableRadio 0", ifname);
			printf("%s; command= '%s'\n", __FUNCTION__, command);
			system(command);

			/* In case of 2.4 GHz, even when the tx is disabled, the interference detection is still enabled; it can cause channel switch which causes some beacons to be sent */
			if (!strcmp(ifname, "wlan0"))
			{
				printf("%s; send command: '%s'\n", __FUNCTION__, "iwpriv wlan0 sInterfDetThresh 0 0 0 0 0 0");
				system("iwpriv wlan0 sInterfDetThresh 0 0 0 0 0 0");
			}
		}
	}
}


static int fapiWlanBssTmQueryReportHandle(const char *ifname, ObjList *wlObj)
{
	char      fullPath[MAX_LEN_PARAM_VALUE];
	char      fullPathGeneral[MAX_LEN_PARAM_VALUE];
	char      fullPathBTM[MAX_LEN_PARAM_VALUE];
	ObjList   *wlObjDB, *subObjDB;
	ObjList   *wlObjGeneralDB, *wlObjBtmDB;
	char      *field, *dialog_token, *reason, *MACAddress, *nr_BSSID = NULL, *pref = NULL, *disassocTimer = NULL;
	char      fieldName[] = "99_candidate";
	int       idx, res = UGW_SUCCESS;
	char      BSSID[] = "00:11:22:33:44:55";

	/* <3>BSS-TM-QUERY wlan2 8c:70:5a:ed:55:40 dialog_token=10
	   reason=19 candidate=d8:fe:e3:3e:bd:14,2178,83,5,7,255 candidate=13:cd:45:ef:67:21,2178,83,5,7,154 candidate=11:22:33:44:55:10,2178,83,5,7,244 */
	//candidate=<nr_BSSID>,<BSSID Information>,<Operating Class>,<Channel Number>,<PHY Type>,<priority for this BSS, 1-255>
	//candidate=12:ab:34:cd:56:10,2178,83,5,7,255

	printf("%s; print the input Obj\n", __FUNCTION__);
	setLog(ifname, wlObj, 0);

	/* Read the BSSID from the internal data-base */
	if (bssidGet(ifname, BSSID) == UGW_FAILURE)
	{
		printf("%s; bssidGet() ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}
	printf("%s; ifname= '%s', BSSID= '%s'\n", __FUNCTION__, ifname, BSSID);

	/* Get the input params of this function */
	MACAddress = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress");
	dialog_token = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "dialog_token");
	printf("%s; MACAddress= '%s', dialog_token= '%s'\n", __FUNCTION__, MACAddress, dialog_token);

	/* read the data from the data-base */
	wlObjDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjDB == NULL)
	{
		printf("%s; create DB object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	sprintf(fullPath, "/tmp/wlan_wave/%s_%s", "BeeRock.BSS.Neighbor", ifname);
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	if (HELP_LOADLOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPath);
	}

	wlObjGeneralDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjGeneralDB == NULL)
	{
		printf("%s; create General-DB object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	sprintf(fullPathGeneral, "/usr/lib/%s_%s", "BeeRock.General.db", ifname);
	printf("%s; fullPathGeneral= '%s'\n", __FUNCTION__, fullPathGeneral);
	if (HELP_LOADLOCALDB(wlObjGeneralDB, fullPathGeneral) == UGW_FAILURE)
	{
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPathGeneral);
	}

	reason = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "reason");
	printf("%s; reason= '%s'\n", __FUNCTION__, reason);
	if ( (reason != NULL) && !strcmp(reason, "19") )
	{  /* Only when the 'reason' is '19', there is a candidate list */
		idx = 0;
		while (1)
		{
			sprintf(fieldName, "%d_candidate", idx++);
			field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", fieldName);
			if ( (field == NULL) || (idx > 99) )
			{
				printf("%s; %s= '%s', idx= %d ==> exit\n", __FUNCTION__, fieldName, field, idx);
				break;
			}
			printf("%s; %s= '%s'\n", __FUNCTION__, fieldName, field);
			nr_BSSID = strtok(field, ",");
			printf("%s; [0] nr_BSSID= '%s'\n", __FUNCTION__, field);

			subObjDB = subObjForTwoSameFieldsGet(wlObjDB, BSSID, nr_BSSID);
			if (subObjDB == NULL)
			{  /* There is no object with this BSSID & nr_BSSID ==> create a new one */
				printf("%s; NOT FOUND RECORD ==> Create a new one\n", __FUNCTION__);
				subObjDB = help_addObjList(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
				HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSID", BSSID, 0, 0);
			}

			HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "nr_BSSID", nr_BSSID, 0, 0);

			/* Update the data-base with the input params of this function */
			//HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ssid", ssid, 0, 0);
			HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "MACAddress", MACAddress, 0, 0);

			field = strtok(NULL, ",");
			printf("%s; [1] BSSIDInfo= '%s'\n", __FUNCTION__, field);
			if (field != NULL)
			{
				HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSIDInfo", field, 0, 0);
			}

			field = strtok(NULL, ",");
			printf("%s; [2] opClass= '%s'\n", __FUNCTION__, field);
			if (field != NULL)
			{
				HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "opClass", field, 0, 0);
			}

			field = strtok(NULL, ",");
			printf("%s; [3] channelNumber= '%s'\n", __FUNCTION__, field);
			if (field != NULL)
			{
				HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "channelNumber", field, 0, 0);
			}

			field = strtok(NULL, ",");
			printf("%s; [4] PhyType= '%s'\n", __FUNCTION__, field);
			if (field != NULL)
			{
				HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "PhyType", field, 0, 0);
			}

			field = strtok(NULL, " ");
			printf("%s; [5] priority= '%s'\n", __FUNCTION__, field);
			if (field != NULL)
			{
				HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "priority", field, 0, 0);
			}

			HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "reason", reason, 0, 0);
		}
	}

	printf("%s; Data Base Object to be stored ('%s'):\n", __FUNCTION__, fullPath);
	setLog(ifname, wlObjDB, 0);

	/* write the data into the data-base */
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	if (HELP_STORELOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_STORELOCALDB Error!\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	/*
	HERE call ap_manager_lite_btm_req() with the stored params
	*/
	sprintf(fullPathBTM, "/tmp/wlan_wave/%s_%s", "BeeRock.BTM.Params", ifname);
	printf("%s; fullPathBTM= '%s'\n", __FUNCTION__, fullPathBTM);
	wlObjBtmDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjBtmDB == NULL)
	{
		printf("%s; create BTM-DB object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if (HELP_LOADLOCALDB(wlObjBtmDB, fullPathBTM) == UGW_FAILURE)
	{
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPathBTM);
	}

	/* Get 'pref' from "BeeRock.General.db" */
	pref = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "pref");
	printf("%s; 'pref' is not present in '%s' ==> get it from '%s'; pref= '%s'\n", __FUNCTION__, fullPath, fullPathGeneral, pref);

	/* Try getting disassocTimer from BeeRock.BTM.Params */
	disassocTimer = paramValueFromObjGet(wlObjBtmDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "disassocTimer");
	printf("%s; [0.1] disassocTimer= '%s'\n", __FUNCTION__, disassocTimer);
	if (disassocTimer == NULL)
	{    /* In case it is not present in "BeeRock.BTM.Params" get it from "BeeRock.General.db" */
		printf("%s; 'disassocTimer' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPath, fullPathGeneral);
		disassocTimer = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "disassocTimer");
		printf("%s; [0.2] disassocTimer= '%s'\n", __FUNCTION__, disassocTimer);
	}

	printf("%s; ifname= '%s', MACAddress= '%s', dialog_token= '%s', pref= '%s', reason= '%s', disassocTimer= '%s'\n",
		   __FUNCTION__, ifname, MACAddress, dialog_token, pref, reason, disassocTimer);
	if (ap_manager_lite_btm_req(ifname, MACAddress, dialog_token, pref, reason, disassocTimer, "1", NULL) == UGW_FAILURE)
	{
		printf("%s; ap_manager_lite_btm_req() ERROR\n", __FUNCTION__);
		res = UGW_FAILURE;
	}

	HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);

	return res;
}


static int fapiWlanBeaconReportHandle(const char *ifname, ObjList *wlObj)
{
	char    fullPath[MAX_LEN_PARAM_VALUE];
	ObjList *wlObjDB, *subObjDB;
	char    *nr_BSSID, *field;
	char    BSSID[] = "00:11:22:33:44:55";

	printf("%s; print the input Obj\n", __FUNCTION__);
	setLog(ifname, wlObj, 0);

	wlObjDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjDB == NULL)
	{
		printf("%s; create DB object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if (bssidGet(ifname, BSSID) == UGW_FAILURE)
	{
		printf("%s; bssidGet() ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	/* read the data from the data-base */
	sprintf(fullPath, "/tmp/wlan_wave/%s_%s", "BeeRock.BSS.Neighbor", ifname);
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	if (HELP_LOADLOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPath);
	}

	if (ownBssNeighborDataBaseUpdate(ifname, wlObjDB, BSSID) == UGW_FAILURE)
	{
		printf("%s; ownBssNeighborDataBaseUpdate returned ERROR\n", __FUNCTION__);
	}

	nr_BSSID = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssid");
	printf("%s; [0] nr_BSSID= '%s'\n", __FUNCTION__, nr_BSSID);

	/* Find a record with the same BSSID and nr_BSSID; if not present, create it! */
	printf("%s; BSSID= '%s', nr_BSSID= '%s'\n", __FUNCTION__, BSSID, nr_BSSID);

	subObjDB = subObjForTwoSameFieldsGet(wlObjDB, BSSID, nr_BSSID);
	if (subObjDB == NULL)
	{  /* There is no object with this BSSID & nr_BSSID ==> create a new one */
		printf("%s; NOT FOUND RECORD ==> Create a new one\n", __FUNCTION__);
		subObjDB = help_addObjList(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSID", BSSID, 0, 0);
	}

	printf("%s; [1] BSSID= '%s', nr_BSSID= '%s'\n", __FUNCTION__, BSSID, nr_BSSID);
	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "nr_BSSID", nr_BSSID, 0, 0);

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "MACAddress", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dialog_token");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dialog_token", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_rep_mode");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_rep_mode", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "op_class");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "opClass", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio", "Channel");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "channelNumber", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "start_time");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "start_time", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "frame_info");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "frame_info", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rcpi");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rcpi", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rsni");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rsni", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "antenna_id");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "antenna_id", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "parent_tsf");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "parent_tsf", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "wide_band_ch_switch");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "wide_band_ch_switch", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "timestamp");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "timestamp", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "beacon_int");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "beacon_int", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "capab_info");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "capab_info", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ssid");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ssid", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rm_capa");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rm_capa", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "vendor_specific");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "vendor_specific", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rsn_info");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rsn_info", field, 0, 0);
	}

	printf("%s; Data Base Object to be stored ('%s'):\n", __FUNCTION__, fullPath);
	setLog(ifname, wlObjDB, 0);

	/* write the data into the data-base */
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	if (HELP_STORELOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_STORELOCALDB Error!\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	return UGW_SUCCESS;
}


static int fapiWlanStaWnmNotifHandle(const char *ifname, ObjList *wlObj)
{
	char    fullPath[MAX_LEN_PARAM_VALUE];
	ObjList *wlObjDB, *subObjDB;
	char    *field;
	int     idx;
	char    fieldName[32];

	/* <3>AP-STA-WNM-NOTIF wlan0 00:11:22:33:44:55 cell_capa=1 non_pref_chan=5:1:36:50 non_pref_chan=5:9:36:50 */
	/* non_pref_chan = <opClass : channelNumber : pref : reason> */

	printf("%s; print the input Obj\n", __FUNCTION__);
	setLog(ifname, wlObj, 0);

	wlObjDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjDB == NULL)
	{
		printf("%s; create DB object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	/* read the data from the data-base */
	sprintf(fullPath, "/tmp/wlan_wave/%s_%s", "BeeRock.STA.Params", ifname);
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	if (HELP_LOADLOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPath);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress");
	printf("%s; [0] BSSID= '%s'\n", __FUNCTION__, field);
	subObjDB = HELP_GETOBJPTR(wlObjDB, "MACAddress", field);
	if (subObjDB == NULL)
	{
		printf("%s; HELP_GETOBJPTR ==> 'MACAddress' not found! ==> create new object\n", __FUNCTION__);
		subObjDB = help_addObjList(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	}

	printf("%s; [1] MACAddress= '%s'\n", __FUNCTION__, field);
	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "MACAddress", field, 0, 0);

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio", "Name");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "Name", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "cell_capa");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "cell_capa", field, 0, 0);
	}

	idx = 0;
	while (1)
	{
		sprintf(fieldName, "%d_non_pref_chan", idx++);
		field = paramValueFromObjGet(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", fieldName);
		if ( (field == NULL) || (idx > 99) )
		{
			printf("%s; %s= '%s', idx= %d ==> exit\n", __FUNCTION__, fieldName, field, idx);
			break;
		}
		
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", fieldName, field, 0, 0);
	}

	printf("%s; Data Base Object to be stored ('%s'):\n", __FUNCTION__, fullPath);
	setLog(ifname, wlObjDB, 0);

	/* write the data into the data-base */
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	if (HELP_STORELOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_STORELOCALDB Error!\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	return UGW_SUCCESS;
}


static int fapiWlanStaConnectReportHandle(const char *ifname, ObjList *wlObj)
{
	char    fullPath[MAX_LEN_PARAM_VALUE];
	ObjList *wlObjDB, *subObjDB;
	char    *field;
	int     idx;
	char    fieldName[32];

	/* <3>AP-STA-CONNECTED wlan0 24:77:03:80:5d:90 SignalStrength=-49 SupportedRates=2 4 11 22 12 18 24 36 48 72 96 108 HT_CAP=107E HT_MCS=FF FF FF 00 00 00 00 00 00 00 C2 01 01 00 00 00
	   VHT_CAP=03807122 VHT_MCS=FFFA 0000 FFFA 0000 btm_supported=1 nr_enabled=0 non_pref_chan=81:200:1:7 non_pref_chan=81:100:2:9 cell_capa=1 */
	/* non_pref_chan = <opClass : channelNumber : pref : reason> */

	printf("%s; print the input Obj\n", __FUNCTION__);
	setLog(ifname, wlObj, 0);

	wlObjDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjDB == NULL)
	{
		printf("%s; create DB object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	/* read the data from the data-base */
	sprintf(fullPath, "/tmp/wlan_wave/%s_%s", "BeeRock.STA.Params", ifname);
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	if (HELP_LOADLOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPath);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress");
	subObjDB = HELP_GETOBJPTR(wlObjDB, "MACAddress", field);
	if (subObjDB == NULL)
	{
		printf("%s; HELP_GETOBJPTR ==> 'MACAddress' not found! ==> create new object\n", __FUNCTION__);
		subObjDB = help_addObjList(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	}

	printf("%s; MACAddress= '%s'\n", __FUNCTION__, field);
	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "MACAddress", field, 0, 0);

	field = paramValueFromObjGet(wlObj, "Device.WiFi.SSID", "Name");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "Name", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "SignalStrength");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "SignalStrength", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "SupportedRates");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "SupportedRates", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "SupportedChannels");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "SupportedChannels", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "HT_CAP");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "HT_CAP", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "HT_MCS");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "HT_MCS", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "VHT_CAP");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "VHT_CAP", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "VHT_MCS");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "VHT_MCS", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "btm_supported");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "btm_supported", field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "nr_enabled");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "nr_enabled", field, 0, 0);
	}

	idx = 0;
	while (1)
	{
		sprintf(fieldName, "%d_non_pref_chan", idx);
		field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", fieldName);
		if ( (field == NULL) || (idx > 99) )
		{
			printf("%s; %s= '%s', idx= %d ==> exit\n", __FUNCTION__, fieldName, field, idx);
			break;
		}
		
		idx++;
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", fieldName, field, 0, 0);
	}

	field = paramValueFromObjGet(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor.AssociatedDevice", "cell_capa");
	if (field != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "cell_capa", field, 0, 0);
	}

	printf("%s; Data Base Object to be stored ('%s'):\n", __FUNCTION__, fullPath);
	setLog(ifname, wlObjDB, 0);

	/* write the data into the data-base */
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	if (HELP_STORELOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_STORELOCALDB Error!\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}


/*************************************************/
/* Global Functions for general use - prototypes */
/*************************************************/
char *paramValueFromObjGet(ObjList *objPtr, char *objName, char *paramName);
char *paramValueFromSubObjGet(ObjList *subObjDB, char *paramName);
int bssidGet(const char *ifname, char *BSSID);
ObjList *subObjForTwoSameFieldsGet(ObjList *wlObjDB, char *BSSID, char *nr_BSSID);


/***********************************/
/* AP Manager Lite - API Functions */
/***********************************/
int ap_manager_lite_cellular_config_set(const char *ifname, char *MBOCellAware, char *CellPref)
{
	int     res = UGW_SUCCESS;
	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	ObjList *wlObjDB, *tmpObjDB;
	char    fullPath[MAX_LEN_PARAM_VALUE];

	printf("%s; ifname= '%s', MBOCellAware= '%s', CellPref= '%s'\n", __FUNCTION__, ifname, MBOCellAware, CellPref);

	if (wlObj == NULL)
	{
		printf("%s; create object ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	if (strcmp(MBOCellAware, "NULL"))
	{
		if ( (!strcmp(MBOCellAware, "0")) || (!strcmp(MBOCellAware, "1")) )
		{
			printf("%s; MBOCellAware= '%s'\n", __FUNCTION__, MBOCellAware);

			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "MBOCellAware", MBOCellAware, 0, 0);

			printf("%s; Data Base Object to be sent to cellular_aware_set():\n", __FUNCTION__);
			setLog(ifname, wlObj, 0);

			if (fapi_wlan_cellular_aware_set(ifname, wlObj, 0) == UGW_FAILURE)
			{
				res = UGW_FAILURE;
				printf("%s; fapi_wlan_cellular_aware_set ERROR\n", __FUNCTION__);
			}

			/* Delete "Device.WiFi.Radio.X_LANTIQ_COM_Vendor" Object */
			FOR_EACH_OBJ(wlObj, tmpObjDB)
			{
				if ( ! strcmp(GET_OBJ_NAME(tmpObjDB), "Device.WiFi.Radio.X_LANTIQ_COM_Vendor") )
				{
					HELP_DELETE_CURRENT_OBJ(wlObj, GET_OBJ_NAME(tmpObjDB), tmpObjDB, SOPT_OBJVALUE);
				}
			}
		}
		else
		{
			printf("%s; The ONLY supported 'MBOCellAware' values are '0' and '1' ==> Abort!\n", __FUNCTION__);
		}
	}

	if (strcmp(CellPref, "NULL"))
	{
		if ( (!strcmp(CellPref, "0")) || (!strcmp(CellPref, "1")) || (!strcmp(CellPref, "255")) )
		{
			printf("%s; CellPref= '%s'\n", __FUNCTION__, CellPref);

			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "CellPref", CellPref, 0, 0);

			/* write "cellPref" into the data-base */
			wlObjDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
			if (wlObjDB == NULL)
			{
				printf("%s; create DB object ERROR\n", __FUNCTION__);
			}
			else
			{
				/* read the data from the data-base */
				sprintf(fullPath, "/tmp/wlan_wave/%s_%s", "BeeRock.BTM.Params", ifname);
				printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
				
				if (HELP_LOADLOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
				{
					printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPath);
					help_addObjList(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
				}
				printf("%s; wlObjDB= 0x%x\n", __FUNCTION__, (unsigned int)wlObjDB);

				HELP_EDIT_NODE(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "cellPref", CellPref, 0, 0);

				printf("%s; Data Base Object to be stored ('%s'):\n", __FUNCTION__, fullPath);
				setLog(ifname, wlObjDB, 0);

				/* write the data into the data-base */
				if (HELP_STORELOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
				{
					printf("%s; HELP_STORELOCALDB Error!\n", __FUNCTION__);
				}
				
				HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
			}

			printf("%s; Data Base Object to be sent to cellular_pref_set():\n", __FUNCTION__);
			setLog(ifname, wlObj, 0);

			if (fapi_wlan_cellular_pref_set(ifname, wlObj, 0) == UGW_FAILURE)
			{
				res = UGW_FAILURE;
				printf("%s; fapi_wlan_cellular_pref_set ERROR\n", __FUNCTION__);
			}
		}
		else
		{
			printf("%s; The ONLY supported 'cellPref' values are '0', '1' and '255' ==> Abort!\n", __FUNCTION__);
		}
	}

	HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);

	return res;
}


int ap_manager_lite_bss_neighbor_set(const char *ifname, char *nr_BSSID, char *opClass, char *channelNumber, char *priority, char *oce, char *tbtt_offset)
{
	int     res = UGW_SUCCESS, decChannelNumber, decOpClass;
	char    *ssid = NULL, *BSSIDInfo = NULL, *PhyType = NULL;
	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	ObjList *wlObjDB, *wlObjGeneralDB, *subObjDB;
	char    NeighborReport[32] = "00112233445599999999620205";  /* default should be "00112233445500000000620207" - it'll get this value at the end of the below sequence */
	char    fullPath[MAX_LEN_PARAM_VALUE];
	char    fullPathGeneral[MAX_LEN_PARAM_VALUE];
	char    hexOpClass[2], hexChannelNumber[2];
	char    BSSID[] = "00:11:22:33:44:55";

	//nr=<hexdump of neighbor element report: BSSID (6 octets) + BSSID information (4 octets) Operating class (1 byte) Channel number (1 byte) PHY type (1 byte) optional subelements >
	//nr = "001122334455 00000000 51 01 07"

	printf("%s; ifname= '%s', nr_BSSID= '%s', opClass= '%s', channelNumber= '%s', priority= '%s'\n",
	       __FUNCTION__, ifname, nr_BSSID, opClass, channelNumber, priority);

	if (wlObj == NULL)
	{
		printf("%s; create object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if ( (nr_BSSID == NULL) || (opClass == NULL) || (channelNumber == NULL) || (priority == NULL) )
	{
		printf("%s; parameters ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	if (atoi(channelNumber) > 255)
	{
		printf("%s; channelNumber= '%s' ==> parameters ERROR\n", __FUNCTION__, channelNumber);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	if (bssidGet(ifname, BSSID) == UGW_FAILURE)
	{
		printf("%s; bssidGet() ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	wlObjGeneralDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjGeneralDB == NULL)
	{
		printf("%s; create General-DB object ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}
	
	/* Set the General db full-path */
	sprintf(fullPathGeneral, "/usr/lib/%s_%s", "BeeRock.General.db", ifname);
	printf("%s; fullPathGeneral= '%s'\n", __FUNCTION__, fullPathGeneral);
	if (HELP_LOADLOCALDB(wlObjGeneralDB, fullPathGeneral) == UGW_FAILURE)
	{
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found! ==> Abort!\n", __FUNCTION__, fullPathGeneral);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		HELP_DELETE_OBJ(wlObjGeneralDB, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}
	
	/* read the data from the data-base - get ssid */
	wlObjDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjDB == NULL)
	{
		printf("%s; create DB object ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	sprintf(fullPath, "/tmp/wlan_wave/%s_%s", "BeeRock.BSS.Neighbor", ifname);
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	if (HELP_LOADLOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		/* BeeRock.General.db does NOT exist; Do not abort! Continue and try using BeeRock.General.db */
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPath);
	}

	if (ownBssNeighborDataBaseUpdate(ifname, wlObjDB, BSSID) == UGW_FAILURE)
	{
		printf("%s; ownBssNeighborDataBaseUpdate returned ERROR\n", __FUNCTION__);
	}

	ssid = paramValueFromObjGet(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ssid");
	printf("%s; ssid= '%s'\n", __FUNCTION__, ssid);
	if (ssid == NULL)
	{  /* In case it is not present in "BeeRock.BSS.Neighbor" get it from "BeeRock.General.db" */
		printf("%s; 'ssid' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPath, fullPathGeneral);
		ssid = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ssid");
		printf("%s; ssid ('%s') found in '%s'\n", __FUNCTION__, ssid, fullPathGeneral);
	}

	BSSIDInfo = paramValueFromObjGet(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSIDInfo");
	printf("%s; BSSIDInfo= '%s'\n", __FUNCTION__, BSSIDInfo);
	if (BSSIDInfo == NULL)
	{  /* In case it is not present in "BeeRock.BSS.Neighbor" get it from "BeeRock.General.db" */
		printf("%s; 'BSSIDInfo' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPath, fullPathGeneral);
		if (!strcmp(BSSID, nr_BSSID))
		{
			printf("%s; BSSID= '%s', nr_BSSID= '%s' ==> this is Own-BSS ==> get 'BSSIDInfoOwnBSS' from '%s'\n", __FUNCTION__, BSSID, nr_BSSID, fullPathGeneral);
			BSSIDInfo = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSIDInfoOwnBSS");
		}
		else
		{
			BSSIDInfo = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSIDInfo");
		}

		printf("%s; BSSIDInfo ('%s') found in '%s'\n", __FUNCTION__, BSSIDInfo, fullPathGeneral);
	}

	PhyType = paramValueFromObjGet(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "PhyType");
	printf("%s; PhyType= '%s'\n", __FUNCTION__, PhyType);
	if (PhyType == NULL)
	{  /* In case it is not present in "BeeRock.BSS.Neighbor" get it from "BeeRock.General.db" */
		printf("%s; 'PhyType' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPath, fullPathGeneral);
		PhyType = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "PhyType");
		printf("%s; PhyType ('%s') found in '%s'\n", __FUNCTION__, PhyType, fullPathGeneral);
	}

	decOpClass = atoi(opClass);
	if (decOpClass <= 0xFF)
	{
		sprintf(hexOpClass, "%2x", atoi(opClass));
	}

	decChannelNumber = atoi(channelNumber);
	if (decChannelNumber <= 0xFF)
	{
		sprintf(hexChannelNumber, "%2x", decChannelNumber);
	}

	printf("%s; opClass= '%s', channelNumber= '%s'\n", __FUNCTION__, opClass, channelNumber);
	printf("%s; [0] hexOpClass= '%s', hexChannelNumber= '%s'\n", __FUNCTION__, hexOpClass, hexChannelNumber);

	if (hexOpClass[0] == ' ')
		hexOpClass[0] = '0';

	if (hexChannelNumber[0] == ' ')
		hexChannelNumber[0] = '0';

	printf("%s; [1] hexOpClass= '%s', hexChannelNumber= '%s'\n", __FUNCTION__, hexOpClass, hexChannelNumber);

	printf("%s; [0] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	strncpy(&NeighborReport[0], &nr_BSSID[0], 2);
	printf("%s; [0.1] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	strncpy(&NeighborReport[2], &nr_BSSID[3], 2);
	printf("%s; [0.2] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	strncpy(&NeighborReport[4], &nr_BSSID[6], 2);
	printf("%s; [0.3] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	strncpy(&NeighborReport[6], &nr_BSSID[9], 2);
	printf("%s; [0.4] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	strncpy(&NeighborReport[8], &nr_BSSID[12], 2);
	printf("%s; [0.5] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	strncpy(&NeighborReport[10], &nr_BSSID[15], 2);
	printf("%s; [0.6] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	printf("%s; [1] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	strncpy(&NeighborReport[12], BSSIDInfo, 8);
	printf("%s; [2] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	strncpy(&NeighborReport[20], hexOpClass, 2);
	printf("%s; [3] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	strncpy(&NeighborReport[22], hexChannelNumber, 2);
	printf("%s; [4] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	strncpy(&NeighborReport[24], PhyType, 2);
	printf("%s; [5] NeighborReport= '%s'\n", __FUNCTION__, NeighborReport);
	printf("%s; [6] BSSIDInfo= '%s'\n", __FUNCTION__, BSSIDInfo);

	help_addObjList(wlObj, "Device.WiFi.SSID", 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID", "BSSID", BSSID, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID", "SSID", ssid, 0, 0);

	help_addObjList(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "NeighborReport", NeighborReport, 0, 0);

	/* Handle optional OCE parameters */
	if (oce != NULL)
	{
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "oce", oce, 0, 0);
	}

	if (tbtt_offset != NULL)
	{
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "tbtt_offset", tbtt_offset, 0, 0);
	}

	printf("%s; Data Base Object to be sent to neighbor_set():\n", __FUNCTION__);
	setLog(ifname, wlObj, 0);

	if (!strcmp(BSSID, nr_BSSID))
	{
		printf("%s; BSSID= '%s', nr_BSSID= '%s' ==> this is Own-BSS ==> do NOT call fapi_wlan_neighbor_set()\n", __FUNCTION__, BSSID, nr_BSSID);
	}
	else if (fapi_wlan_neighbor_set(ifname, wlObj, 0) == UGW_FAILURE)
	{
		printf("fapi_wlan_neighbor_set ERROR\n");
		res = UGW_FAILURE;
	}

	/* Find a record with the same BSSID and nr_BSSID; if not present, create it! */
	printf("%s; BSSID= '%s', nr_BSSID= '%s'\n", __FUNCTION__, BSSID, nr_BSSID);

	subObjDB = subObjForTwoSameFieldsGet(wlObjDB, BSSID, nr_BSSID);
	if (subObjDB == NULL)
	{  /* There is no object with this BSSID & nr_BSSID ==> create a new one */
		printf("%s; NOT FOUND RECORD ==> Create a new one\n", __FUNCTION__);
		subObjDB = help_addObjList(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSID", BSSID, 0, 0);
	}

	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSID", BSSID, 0, 0);
	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "nr_BSSID", nr_BSSID, 0, 0);
	printf("%s; BSSIDInfo= '%s'\n", __FUNCTION__, BSSIDInfo);
	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSIDInfo", BSSIDInfo, 0, 0);
	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "opClass", opClass, 0, 0);
	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "channelNumber", channelNumber, 0, 0);
	printf("%s; PhyType= '%s'\n", __FUNCTION__, PhyType);
	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "PhyType", PhyType, 0, 0);
	HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "priority", priority, 0, 0);

	if (oce != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "oce", oce, 0, 0);
	}

	if (tbtt_offset != NULL)
	{
		HELP_EDIT_SELF_NODE(subObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "tbtt_offset", tbtt_offset, 0, 0);
	}

	printf("%s; Data Base Object to be stored ('%s'):\n", __FUNCTION__, fullPath);
	setLog(ifname, wlObjDB, 0);

	/* update the data-base: write the updated data back into the data-base */
	if (HELP_STORELOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_STORELOCALDB Error!\n", __FUNCTION__);
	}

	HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
	HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
	HELP_DELETE_OBJ(wlObjGeneralDB, SOPT_OBJVALUE, FREE_OBJLIST);

	return res;
}


int ap_manager_lite_btm_params_set(const char *ifname,
								   char *bssTermDuration,   /* Optional Param - can be 'NULL' */
								   char *disassocImminent,  /* Optional Param - can be 'NULL' */
                                   char *btmReqTermBit,     /* Optional Param - can be 'NULL' */
								   char *disassocTimer,     /* Optional Param - can be 'NULL' */
								   char *reassocDelay,      /* Optional Param - can be 'NULL' */
								   char *bssTermTSF)        /* Optional Param - can be 'NULL' */
{
	ObjList *wlObjDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char    fullPath[MAX_LEN_PARAM_VALUE];

	printf("%s; ifname= '%s', bssTermDuration= '%s', disassocImminent= '%s', btmReqTermBit= '%s', disassocTimer= '%s', reassocDelay= '%s', bssTermTSF= '%s'\n",
	       __FUNCTION__, ifname, bssTermDuration, disassocImminent, btmReqTermBit, disassocTimer, reassocDelay, bssTermTSF);
	
	if (wlObjDB == NULL)
	{
		printf("%s; create DB object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	/* update the data-base: read the data from the data-base */
	sprintf(fullPath, "/tmp/wlan_wave/%s_%s", "BeeRock.BTM.Params", ifname);
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	
	if (HELP_LOADLOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found. Create it!\n", __FUNCTION__, fullPath);
		help_addObjList(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	}
	printf("%s; wlObjDB= 0x%x\n", __FUNCTION__, (unsigned int)wlObjDB);

	if (bssTermDuration != NULL)
		HELP_EDIT_NODE(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssTermDuration", bssTermDuration, 0, 0);
	else
		printf("%s; bssTermDuration is '%s' ==> do NOT update the field's data-base!\n", __FUNCTION__, disassocImminent);

	if (disassocImminent != NULL)
		HELP_EDIT_NODE(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "disassocImminent", disassocImminent, 0, 0);
	else
		printf("%s; disassocImminent is '%s' ==> do NOT update the field's data-base!\n", __FUNCTION__, disassocImminent);

	if (btmReqTermBit != NULL)
		HELP_EDIT_NODE(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "btmReqTermBit", btmReqTermBit, 0, 0);
	else
		printf("%s; btmReqTermBit is '%s' ==> do NOT update the field's data-base!\n", __FUNCTION__, btmReqTermBit);

	if (disassocTimer != NULL)
		HELP_EDIT_NODE(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "disassocTimer", disassocTimer, 0, 0);
	else
		printf("%s; disassocTimer is '%s' ==> do NOT update the field's data-base!\n", __FUNCTION__, disassocTimer);

	if (reassocDelay != NULL)
		HELP_EDIT_NODE(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "reassocDelay", reassocDelay, 0, 0);
	else
		printf("%s; reassocDelay is '%s' ==> do NOT update the field's data-base!\n", __FUNCTION__, reassocDelay);

	if (bssTermTSF != NULL)
		HELP_EDIT_NODE(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssTermTSF", bssTermTSF, 0, 0);
	else
		printf("%s; bssTermTSF is '%s' ==> do NOT update the field's data-base!\n", __FUNCTION__, bssTermTSF);

	printf("%s; Data Base Object to be stored ('%s'):\n", __FUNCTION__, fullPath);
	setLog(ifname, wlObjDB, 0);

	/* update the data-base: write the updated data back into the data-base */
	if (HELP_STORELOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
		printf("%s; HELP_STORELOCALDB Error!\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}


int ap_manager_lite_association_disallow(const char *ifname, char *reassocCode)
{
	int     res;
	char    BSSID[] = "00:11:22:33:44:55";
	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	printf("%s; ifname= '%s', reassocCode= '%s'\n", __FUNCTION__, ifname, reassocCode);

	if (wlObj == NULL)
	{
		printf("%s; create object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if (reassocCode == NULL)
	{
		printf("%s; parameters ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	/* Read the BSSID from the internal data-base */
	if (bssidGet(ifname, BSSID) == UGW_FAILURE)
	{
		printf("%s; bssidGet() ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}
	printf("%s; BSSID= '%s'\n", __FUNCTION__, BSSID);

	help_addObjList(wlObj, "Device.WiFi.SSID", 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID", "BSSID", BSSID, 0, 0);

	printf("%s; Data Base Object to be sent to mbo_sta_allow():\n", __FUNCTION__);
	setLog(ifname, wlObj, 0);

	if (!strcmp(reassocCode, "0"))
	{
		res = fapi_wlan_mbo_sta_allow(ifname, wlObj, 0);
	}
	else
	{
		help_addObjList(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MboBssAssociationReason", reassocCode, 0, 0);

		res = fapi_wlan_mbo_sta_deny(ifname, wlObj, 0);
	}

	HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);

	return res;
}


int ap_manager_lite_disassociate_set(const char *ifname, char *MACAddress)
{
	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	printf("%s; ifname= '%s', MACAddress= '%s'\n", __FUNCTION__, ifname, MACAddress);

	if (wlObj == NULL)
	{
		printf("%s; create object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	help_addObjList(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddress", MACAddress, 0, 0);

	printf("%s; Data Base Object to be sent to sta_disassociate():\n", __FUNCTION__);
	setLog(ifname, wlObj, 0);

	if (fapi_wlan_sta_disassociate(ifname, wlObj, 0) == UGW_FAILURE)
	{
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
	return UGW_SUCCESS;
}


int ap_manager_lite_btm_req(const char *ifname,
							char *MACAddress,
							char *dialog_token,
							char *pref,
							char *reason,
							char *disassocTimer,  /* Optional Param - can be 'NULL' */
							char *abridged,       /* Optional Param - can be 'NULL' */
							char *valid_int)      /* Optional Param - can be 'NULL' */
{
	ObjList   *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	ObjList   *wlObjDB, *wlObjGeneralDB, *wlObjStaDB = NULL, *wlObjNeighborDB, *subObjDB, *subObjStaDB;
	int       res;
	char      *mbo;  /* mbo=<reason>:<reassoc_delay>:<cell_pref> */
	char      fullPath[MAX_LEN_PARAM_VALUE];
	char      fullPathGeneral[MAX_LEN_PARAM_VALUE];
	char      fullPathNeighbor[MAX_LEN_PARAM_VALUE];
	char      fullPathSTA[MAX_LEN_PARAM_VALUE];
	char      BSSID[] = "00:11:22:33:44:55";
	char      *nr_BSSID;
	char      reassocDelayBuf[4];  /* used in order to allocate 1 character - will be used if reassocDelay needs to be set to '0' */
	char      *reassocDelay = reassocDelayBuf;
	char      bssTermination[32];
	char      *bssTermDuration = NULL, *bssTermTSF = NULL;
	char      *disassocImminent;
	char      *cellPref;
	char      disassocTimerLocal[4];
	int       sizeOfNeighbor;
	char      *neighbor = NULL, *BSSIDInfo, *opClass, *channelNumber, *PhyType, *priority, *non_pref_chan = NULL, *stationOpClass, *stationChannelNumber, *stationPref, *btmReqTermBit, *bssTerminationTimer;
	/* neighbor=<nr_BSSID>,<BSSID Information>,<Operating Class>,<Channel Number>,<PHY Type>,<priority for this BSS> */
	ParamList *param = NULL;
	int       idx, neighborIdx = 0, timerSeconds = 0;
	char      fieldName[32];
	bool      isUseNeighbor = true, isAtLeastOneNeighborToSend = false;
#if defined PRIORITY_INTERNAL_SET
	int       priorityIdx = 0;
#endif

	if (bssidGet(ifname, BSSID) == UGW_FAILURE)
	{
		printf("%s; bssidGet() ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	printf("%s; ifname= '%s', MACAddress= '%s', BSSID= '%s', pref= '%s', reason= '%s', disassocTimer= '%s', abridged= '%s', valid_int= '%s'\n",
	       __FUNCTION__, ifname, MACAddress, BSSID, pref, reason, disassocTimer, abridged, valid_int);

	if (wlObj == NULL)
	{
		printf("%s; create object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if ( (MACAddress == NULL) || (pref == NULL) || (reason == NULL) )  /* disassocTimer & valid_int can be NULL - in case that disassocTimer==NULL, take it from "BeeRock.BTM.Params" and if not present then in "BeeRock.General.db" */
	{
		printf("%s; parameters ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	/* Here read the stored data, stored when receiving RRM-BEACON-REP-RECEIVED */
	wlObjDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjDB == NULL)
	{
		printf("%s; create DB object ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	/* read the data from the data-base */
	sprintf(fullPath, "/tmp/wlan_wave/%s_%s", "BeeRock.BTM.Params", ifname);
	printf("%s; fullPath= '%s'\n", __FUNCTION__, fullPath);
	if (HELP_LOADLOCALDB(wlObjDB, fullPath) == UGW_FAILURE)
	{
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPath);
	}

	printf("%s; Stored Data Base Object ('%s'):\n", __FUNCTION__, fullPath);
	setLog(ifname, wlObjDB, 0);

	wlObjGeneralDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjGeneralDB == NULL)
	{
		printf("%s; create General-DB object ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	/* Set the General db full-path */
	sprintf(fullPathGeneral, "/usr/lib/%s_%s", "BeeRock.General.db", ifname);
	printf("%s; fullPathGeneral= '%s'\n", __FUNCTION__, fullPathGeneral);
	if (HELP_LOADLOCALDB(wlObjGeneralDB, fullPathGeneral) == UGW_FAILURE)
	{
		/* BeeRock.General.db does NOT exist; Do not abortl Continue and try using BeeRock.General.db */
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPathGeneral);
	}

	printf("%s; Stored Data Base Object ('%s'):\n", __FUNCTION__, fullPathGeneral);
	setLog(ifname, wlObjGeneralDB, 0);

	disassocImminent = paramValueFromObjGet(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "disassocImminent");
	printf("%s; [0] disassocImminent= '%s'\n", __FUNCTION__, disassocImminent);
	if (disassocImminent == NULL)
	{  /* In case it is not present in "BeeRock.BTM.Params" get it from "BeeRock.General.db" */
		printf("%s; 'disassocImminent' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPath, fullPathGeneral);
		disassocImminent = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "disassocImminent");
		printf("%s; disassocImminent ('%s') found in '%s'\n", __FUNCTION__, disassocImminent, fullPathGeneral);
	}

	/* Get the params (reassocDelay, bssTermDuration, disassocImminent, disassocTimer) from "BeeRock.BTM.Params";
	   if not present, get it from "BeeRock.General.db" */
	if (!strcmp(disassocImminent, "0"))
	{  /* In case that disassocImminent is '0', set reassocDelay to '0' as well */
		strcpy(reassocDelay, "0");
		disassocTimer = disassocTimerLocal;  /* allocate */
		strcpy(disassocTimer, "0");
		printf("%s; [1] disassocImminent is '0' ==> set reassocDelay & disassocTimer to '0' (reassocDelay= '%s', disassocTimer= '%s')\n", __FUNCTION__, reassocDelay, disassocTimer);
	}
	else
	{
		reassocDelay = paramValueFromObjGet(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "reassocDelay");
		printf("%s; [2] reassocDelay= '%s'\n", __FUNCTION__, reassocDelay);
		if (reassocDelay == NULL)
		{  /* In case it is not present in "BeeRock.BTM.Params" get it from "BeeRock.General.db" */
			printf("%s; 'reassocDelay' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPath, fullPathGeneral);
			reassocDelay = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "reassocDelay");
			printf("%s; reassocDelay ('%s') found in '%s'\n", __FUNCTION__, reassocDelay, fullPathGeneral);
		}
	}

	btmReqTermBit = paramValueFromObjGet(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "btmReqTermBit");
	printf("%s; btmReqTermBit= '%s'\n", __FUNCTION__, btmReqTermBit);
	if ( (btmReqTermBit != NULL) && (!strcmp(btmReqTermBit, "1")) )
	{
		/* In case that btmReqTermBit is '1', set timerSeconds to bssTerminationTimer (in seconds) */
		bssTerminationTimer = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssTerminationTimer");  /* in 100 ms units */
		printf("%s; Special Case: btmReqTermBit= '1'; bssTerminationTimer= '%s'\n", __FUNCTION__, bssTerminationTimer);
		if (bssTerminationTimer == NULL)
		{
			printf("%s; bssTerminationTimer is not present in '%s' ==> Abort!\n", __FUNCTION__, fullPathGeneral);
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
			HELP_DELETE_OBJ(wlObjGeneralDB, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_FAILURE;
		}

		timerSeconds = atoi(bssTerminationTimer) / 10;  /* convert bssTerminationTimer to seconds */
		printf("%s; timerSeconds= %d\n", __FUNCTION__, timerSeconds);

		if (strlen(bssTerminationTimer) > 4 /* max size of string */)
		{
			printf("%s; Illegal bssTerminationTimer ('%s') ==> Abort!\n", __FUNCTION__, bssTerminationTimer);
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
			HELP_DELETE_OBJ(wlObjGeneralDB, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_FAILURE;
		}

		sprintf(disassocTimerLocal, "%d", (atoi(bssTerminationTimer) - 5));  /* Set disassocTimerLocal to bssTerminationTimer minus half a second (in 100 ms units) */
		printf("%s; set disassocTimerLocal= '%s'\n", __FUNCTION__, disassocTimerLocal);
		disassocTimer = disassocTimerLocal;  /* allocate */
		printf("%s; set 'disassocTimer' (%s) to 'bssTerminationTimer' (%s) minus half a second\n", __FUNCTION__, disassocTimer, bssTerminationTimer);

		/* Set the value of bssTermDuration ONLY if btmReqTermBit was set to '1' */
		bssTermDuration = paramValueFromObjGet(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssTermDuration");
		printf("%s; [3] bssTermDuration= '%s'\n", __FUNCTION__, bssTermDuration);
		if (bssTermDuration == NULL)
		{  /* In case it is not present in "BeeRock.BTM.Params" get it from "BeeRock.General.db" */
			printf("%s; 'bssTermDuration' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPath, fullPathGeneral);
			bssTermDuration = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssTermDuration");
			printf("%s; bssTermDuration ('%s') found in '%s'\n", __FUNCTION__, bssTermDuration, fullPathGeneral);
		}
	}
	else
	{
		printf("%s; btmReqTermBit= '%s' ==> bssTermDuration= '%s' ==> do NOT use 'bssTermination' in BSS_TM_REQ command\n", __FUNCTION__, btmReqTermBit, bssTermDuration);
	}

	if (disassocTimer != NULL)
	{
		printf("%s; [4] 'disassocTimer' ('%s') received as a function input parameter, or set above due to disassocImminent= '0' or due to btmReqTermBit= '1'\n", __FUNCTION__, disassocTimer);
	}
	else
	{
		/* disassocTimer is NULL - try getting it from "BeeRock.BTM.Params" */
		printf("%s; 'disassocTimer' is NULL ==> get it from '%s' (NOT received as a function input parameter)\n", __FUNCTION__, fullPath);
		disassocTimer = paramValueFromObjGet(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "disassocTimer");
		printf("%s; [5] disassocTimer= '%s'\n", __FUNCTION__, disassocTimer);
		if (disassocTimer == NULL)
		{  /* In case it is not present in "BeeRock.BTM.Params" get it from "BeeRock.General.db" */
			printf("%s; 'disassocTimer' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPath, fullPathGeneral);
			disassocTimer = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "disassocTimer");
			printf("%s; [6] disassocTimer ('%s') found in '%s'\n", __FUNCTION__, disassocTimer, fullPathGeneral);
		}
	}

	cellPref = paramValueFromObjGet(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "cellPref");
	if (cellPref == NULL)
	{  /* In case it is not present in "BeeRock.BTM.Params" get it from "BeeRock.General.db" */
		printf("%s; 'cellPref' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPath, fullPathGeneral);
		cellPref = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "cellPref");
		printf("%s; cellPref ('%s') found in '%s'\n", __FUNCTION__, cellPref, fullPathGeneral);
	}

	/* The below fields being used in order to build "neighbor" field - use it ONLY if all of them are available::
	   <nr_BSSID>,<BSSID Information>,<Operating Class>,<Channel Number>,<PHY Type>,<priority for this BSS> */

	wlObjNeighborDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjNeighborDB == NULL)
	{
		printf("%s; create General-DB object ERROR\n", __FUNCTION__);
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
		HELP_DELETE_OBJ(wlObjGeneralDB, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	/* Set the Neighbor db full-path */
	sprintf(fullPathNeighbor, "/tmp/wlan_wave/%s_%s", "BeeRock.BSS.Neighbor", ifname);
	printf("%s; fullPathNeighbor= '%s'\n", __FUNCTION__, fullPathNeighbor);
	if (HELP_LOADLOCALDB(wlObjNeighborDB, fullPathNeighbor) == UGW_FAILURE)
	{
		printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPathNeighbor);
	}

	printf("%s; Stored Data Base Object ('%s'):\n", __FUNCTION__, fullPathNeighbor);
	setLog(ifname, wlObjNeighborDB, 0);

	if (ownBssNeighborDataBaseUpdate(ifname, wlObjNeighborDB, BSSID) == UGW_FAILURE)
	{
		printf("%s; ownBssNeighborDataBaseUpdate returned ERROR\n", __FUNCTION__);
	}

	printf("%s; [7] reason= '%s'\n", __FUNCTION__, reason);
	/* 'reason' can be in the range of 1 to 9; if it's outside of range - take it from "BeeRock.General.db" */
	if ( (atoi(reason) < 1) || (atoi(reason) > 9) )
	{
		printf("%s; 'reason' is not between '1' and '9' ==> get it from '%s'\n", __FUNCTION__, fullPathGeneral);
		reason = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "reason");
		printf("%s; [8] reason= '%s'\n", __FUNCTION__, reason);
	}

	printf("%s; strlen(reason)= %d\n", __FUNCTION__, strlen(reason));
	printf("%s; [9] reassocDelay= '%s'\n", __FUNCTION__, reassocDelay);
	printf("%s; strlen(reassocDelay)= %d\n", __FUNCTION__, strlen(reassocDelay));
	printf("%s; [10] cellPref= '%s'\n", __FUNCTION__, cellPref);
	printf("%s; strlen(cellPref)= %d\n", __FUNCTION__, strlen(cellPref));

	printf("%s; (strlen(reason) + strlen(reassocDelay) + strlen(cellPref))= %d\n", __FUNCTION__, (strlen(reason) + strlen(reassocDelay) + strlen(cellPref)));

	/* assemble mbo; <reason>:<reassoc_delay>:<cell_pref> */
	mbo = (void *)malloc((size_t)(strlen(reason) + strlen(reassocDelay) + strlen(cellPref)));
	sprintf(mbo, "%s:%s:%s", reason, reassocDelay, cellPref);
	printf("%s; mbo= '%s'\n", __FUNCTION__, mbo);

	/* Prepare the data-base to be sent to bss_transition_management_req() */
	help_addObjList(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress", MACAddress, 0, 0);  // Station's MAC Address

	help_addObjList(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "dialog_token", dialog_token, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "pref", pref, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "mbo", mbo, 0, 0);

	if (bssTermDuration == NULL)
	{
		printf("%s; bssTermDuration= '%s' ==> do NOT use 'bssTermination' in BSS_TM_REQ command\n", __FUNCTION__, bssTermDuration);
	}
	else
	{  /* Assemble bssTermination from TSF + Duration */
		/* disassocTimer is NULL - try getting it from "BeeRock.BTM.Params" */
		bssTermTSF = paramValueFromObjGet(wlObjDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssTermTSF");
		if (bssTermTSF == NULL)
		{
			printf("%s; 'bssTermTSF' is NULL ==> get it from '%s' (NOT received as a function input parameter)\n", __FUNCTION__, fullPath);

			bssTermTSF = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssTermTSF");
			printf("%s; bssTermTSF ('%s') got from '%s'\n", __FUNCTION__, bssTermTSF, fullPathGeneral);
		}
		else
		{
			printf("%s; bssTermTSF ('%s') got from '%s'\n", __FUNCTION__, bssTermTSF, fullPath);
		}

		sprintf(bssTermination, "%s,%s", bssTermTSF, bssTermDuration);
		printf("%s; bssTermTSF= '%s', bssTermDuration= '%s' ==> bssTermination= '%s'\n", __FUNCTION__, bssTermTSF, bssTermDuration, bssTermination);

		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "bss_termination", bssTermination, 0, 0);
	}

	HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "disassoc_imminent", disassocImminent, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "disassoc_timer", disassocTimer, 0, 0);

	if (abridged != NULL)
	{  /* use this optional param ONLY if it was supplied as a valid function param */
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "abridged", abridged, 0, 0);
	}

	if (valid_int == NULL)
	{  /* In case it is not valid, get it from "BeeRock.General.db" */
		printf("%s; 'valid_int' is not valid ==> get it from '%s'\n", __FUNCTION__, fullPathGeneral);
		valid_int = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "validityInterval");
		printf("%s; valid_int ('%s') found in '%s'\n", __FUNCTION__, valid_int, fullPathGeneral);
	}

	if (valid_int != NULL)
	{
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "valid_int", valid_int, 0, 0);
	}

	/* get the neighbor parameters from "BeeRock.BSS.Neighbor" */
	printf("\n%s; run over 'BeeRock.BSS.Neighbor' data-base - START\n", __FUNCTION__);
	FOR_EACH_OBJ(wlObjNeighborDB, subObjDB)
	{
		if ( ! strcmp(GET_OBJ_NAME(subObjDB), "Device.WiFi.Radio.X_LANTIQ_COM_Vendor") )
		{
			printf("%s; GET_OBJ_NAME(subObjDB)= '%s'\n", __FUNCTION__, GET_OBJ_NAME(subObjDB));
			FOR_EACH_PARAM(subObjDB, param)
			{
				if (!strcmp(GET_PARAM_NAME(param), "BSSID"))
				{
					printf("%s; GET_PARAM_NAME(param)= '%s'\n", __FUNCTION__, GET_PARAM_NAME(param));
					if (!strcmp(GET_PARAM_VALUE(param), BSSID))
					{
						printf("%s; found matching BSSID ('%s') in '%s'\n", __FUNCTION__, BSSID, fullPathNeighbor);
						nr_BSSID = paramValueFromSubObjGet(subObjDB, "nr_BSSID");
						printf("%s; nr_BSSID= '%s'\n", __FUNCTION__, nr_BSSID);
						
						/* In case of Own-BSS, do NOT use it as a neighbor in case of:
						   disassocImminent is '1' and/or  btmReqTermBit is '1' */
						if ( (!strcmp(BSSID, nr_BSSID)) &&
						     ( (!strcmp(disassocImminent, "1")) || ( (btmReqTermBit != NULL) && (!strcmp(btmReqTermBit, "1")) ) ) )
						{
							printf("%s; disassocImminent= '%s', btmReqTermBit= '%s'; BSSID= '%s', nr_BSSID= '%s' ==> Own-BSS & (disassocImminent= '1' or btmReqTermBit= '1')==> do NOT use this neighbor ('%s')\n",
							       __FUNCTION__, disassocImminent, btmReqTermBit, BSSID, nr_BSSID, nr_BSSID);
						}
						else
						{
							if (nr_BSSID == NULL)
							{
								printf("%s; nr_BSSID is NULL ==> no 'neighbor' field!\n", __FUNCTION__);
							}
							else
							{
								sizeOfNeighbor = strlen(nr_BSSID) + 1;
								printf("%s; nr_BSSID= '%s'; sizeOfNeighbor= %d\n", __FUNCTION__, nr_BSSID, sizeOfNeighbor);
								BSSIDInfo = paramValueFromSubObjGet(subObjDB, "BSSIDInfo");
								if (BSSIDInfo == NULL)
								{  /* In case it is not present in "BeeRock.BSS.Neighbor" get it from "BeeRock.General.db" */
									printf("%s; 'BSSIDInfo' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPathNeighbor, fullPathGeneral);
									if (!strcmp(BSSID, nr_BSSID))
									{
										printf("%s; BSSID= '%s', nr_BSSID= '%s' ==> this is Own-BSS ==> get 'BSSIDInfoOwnBSS' from '%s'\n", __FUNCTION__, BSSID, nr_BSSID, fullPathGeneral);
										BSSIDInfo = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSIDInfoOwnBSS");
									}
									else
									{
										BSSIDInfo = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSIDInfo");
									}

									printf("%s; BSSIDInfo ('%s') found in '%s'\n", __FUNCTION__, BSSIDInfo, fullPathGeneral);
								}

								if (BSSIDInfo == NULL)
								{
									printf("%s; BSSIDInfo is NULL ==> no 'neighbor' field!\n", __FUNCTION__);
								}
								else
								{
									sprintf(BSSIDInfo, "%d", atoi(BSSIDInfo));  /* Convert the string into numeric representation - remove '0' before the number */
									sizeOfNeighbor += strlen(BSSIDInfo) + 1;
									printf("%s; BSSIDInfo= '%s'; sizeOfNeighbor= %d\n", __FUNCTION__, BSSIDInfo, sizeOfNeighbor);
									opClass = paramValueFromSubObjGet(subObjDB, "opClass");
									if (opClass == NULL)
									{
										printf("%s; opClass is NULL ==> no 'neighbor' field!\n", __FUNCTION__);
									}
									else
									{
										sizeOfNeighbor += strlen(opClass) + 1;
										printf("%s; opClass= '%s'; sizeOfNeighbor= %d\n", __FUNCTION__, opClass, sizeOfNeighbor);
										channelNumber = paramValueFromSubObjGet(subObjDB, "channelNumber");
										if (channelNumber == NULL)
										{
											printf("%s; channelNumber is NULL ==> no 'neighbor' field!\n", __FUNCTION__);
										}
										else
										{
											sizeOfNeighbor += strlen(channelNumber) + 1;
											printf("%s; channelNumber= '%s'; sizeOfNeighbor= %d\n", __FUNCTION__, channelNumber, sizeOfNeighbor);
											PhyType = paramValueFromSubObjGet(subObjDB, "PhyType");
											if (PhyType == NULL)
											{  /* In case it is not present in "BeeRock.BSS.Neighbor" get it from "BeeRock.General.db" */
												printf("%s; 'PhyType' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPathNeighbor, fullPathGeneral);
												PhyType = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "PhyType");
												printf("%s; PhyType ('%s') found in '%s'\n", __FUNCTION__, PhyType, fullPathGeneral);
											}

											if (PhyType == NULL)
											{
												printf("%s; PhyType is NULL ==> no 'neighbor' field!\n", __FUNCTION__);
											}
											else
											{
												sprintf(PhyType, "%d", atoi(PhyType));  /* Convert the string into numeric representation - remove '0' before the number */
												sizeOfNeighbor += strlen(PhyType) + 1;
												printf("%s; PhyType= '%s'; sizeOfNeighbor= %d\n", __FUNCTION__, PhyType, sizeOfNeighbor);

												printf("%s; BSSID= '%s', nr_BSSID= '%s'\n", __FUNCTION__, BSSID, nr_BSSID);
												priority = paramValueFromSubObjGet(subObjDB, "priority");
												if (priority == NULL)
												{
													/* In case it is not present in "BeeRock.BSS.Neighbor" get it from "BeeRock.General.db" */
													printf("%s; 'priority' is not present in '%s' ==> get it from '%s'\n", __FUNCTION__, fullPath, fullPathGeneral);
													priority = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "priority");
													printf("%s; priority= '%s'\n", __FUNCTION__, priority);
												}
												else
												{
													printf("%s; 'priority' is present in '%s'\n", __FUNCTION__, fullPath);
												}

												if (priority == NULL)
												{
													printf("%s; priority is NULL ==> no 'neighbor' field!\n", __FUNCTION__);
												}
												else
												{
#if defined PRIORITY_INTERNAL_SET
													sizeOfNeighbor += 3;  // max priority number is '255'
													printf("%s; priority= '%s' (but use starting from '255' going down); sizeOfNeighbor= %d\n", __FUNCTION__, priority, sizeOfNeighbor);
													neighbor = (char *)malloc(sizeOfNeighbor);
													sprintf(neighbor, "%s,%s,%s,%s,%s", nr_BSSID, BSSIDInfo, opClass, channelNumber, PhyType);
													printf("%s; neighbor (without priority)= '%s'\n", __FUNCTION__, neighbor);
#else
													sizeOfNeighbor += strlen(priority);
													printf("%s; priority= '%s'; sizeOfNeighbor= %d\n", __FUNCTION__, priority, sizeOfNeighbor);
													neighbor = (char *)malloc(sizeOfNeighbor);
													sprintf(neighbor, "%s,%s,%s,%s,%s,%s", nr_BSSID, BSSIDInfo, opClass, channelNumber, PhyType, priority);
													printf("%s; neighbor= '%s'\n", __FUNCTION__, neighbor);
#endif

													/* For this MAC Address, check if the neighbor is in the list of 'non_pref_chan' received inside AP-STA-CONNECTED report */
													wlObjStaDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
													if (wlObjStaDB == NULL)
													{
														printf("%s; create STA object ERROR\n", __FUNCTION__);
														HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
														HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
														HELP_DELETE_OBJ(wlObjGeneralDB, SOPT_OBJVALUE, FREE_OBJLIST);
														HELP_DELETE_OBJ(wlObjNeighborDB, SOPT_OBJVALUE, FREE_OBJLIST);
														return UGW_FAILURE;
													}

													/* Set the STA db full-path */
													sprintf(fullPathSTA, "/tmp/wlan_wave/%s_%s", "BeeRock.STA.Params", ifname);
													printf("%s; fullPathSTA= '%s'\n", __FUNCTION__, fullPathSTA);
													if (HELP_LOADLOCALDB(wlObjStaDB, fullPathSTA) == UGW_FAILURE)
													{
														printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPathSTA);
													}
													else
													{
														printf("%s; Stored Data Base Object ('%s'):\n", __FUNCTION__, fullPathSTA);
														setLog(ifname, wlObjStaDB, 0);
													}

													isUseNeighbor = true;
													subObjStaDB = HELP_GETOBJPTR(wlObjStaDB, "MACAddress", MACAddress);
													if (subObjStaDB != NULL)
													{
														printf("%s; MACAddress '%s' found in %s ==> check it\n", __FUNCTION__, MACAddress, fullPathSTA);

														idx = 0;
														while (idx < 99)
														{
															non_pref_chan = NULL;
															sprintf(fieldName, "%d_non_pref_chan", idx);
															non_pref_chan = paramValueFromSubObjGet(subObjStaDB, fieldName);
															printf("%s; (idx= %d) non_pref_chan= '%s'\n", __FUNCTION__, idx, non_pref_chan);
															printf("%s; Neighbor Params: opClass= '%s', channelNumber= '%s'\n", __FUNCTION__, opClass, channelNumber);

															if (non_pref_chan == NULL)
															{
																printf("%s; fieldName= '%s' (idx= %d) ==> no more neighbors\n", __FUNCTION__, fieldName, idx);
																break;
															}

															/* non_pref_chan = <opClass : channelNumber : pref : reason> */
															stationOpClass = strtok(non_pref_chan, ":");
															stationChannelNumber = strtok(NULL, ":");
															stationPref = strtok(NULL, ":");
															printf("%s; STA Params: opClass= '%s', channelNumber= '%s', stationPref= '%s'\n", __FUNCTION__, stationOpClass, stationChannelNumber, stationPref);

															/* Workaround - switch reason with channelNumber: use <opClass : reason : pref : channelNumber> */
															//stationChannelNumber = strtok(NULL, ":");
															//stationChannelNumber = strtok(NULL, ":");
															//printf("%s; *** AFTER Work-Around *** STA Params: opClass= '%s', channelNumber= '%s'\n", __FUNCTION__, stationOpClass, stationChannelNumber);

															if ( (!strcmp(stationOpClass, opClass)) && (!strcmp(stationChannelNumber, channelNumber)) && (strcmp(stationPref, "255")) )
															{
																printf("%s; STA Params MATCH the non_pref_chan (and stationPref= '%s' is NOT '255') ==> do NOT use it!\n", __FUNCTION__, stationPref);
																isUseNeighbor = false;  /* It'll also cause exiting from the 'while' loop */
															}

															if (!isUseNeighbor)
															{
																printf("%s; fieldName= '%s' (idx= %d); STA Params MATCH match found ==> exit neighbors check\n", __FUNCTION__, fieldName, idx);
																break;
															}

															idx++;
														}
													}
													else
													{
														printf("%s; MACAddress '%s' NOT found in %s ==> check it\n", __FUNCTION__, MACAddress, fullPathSTA);
													}

													if (isUseNeighbor)
													{
														isAtLeastOneNeighborToSend = true;
#if defined PRIORITY_INTERNAL_SET
														if (!strcmp(priority, "0"))
														{
															sprintf(neighbor, "%s,0", neighbor);
															printf("%s; neighbor (with '0' priority)= '%s'\n", __FUNCTION__, neighbor);
														}
														else
														{
															sprintf(neighbor, "%s,%d", neighbor, (255-priorityIdx));
															priorityIdx++;
															printf("%s; neighbor (with NEW priority)= '%s'\n", __FUNCTION__, neighbor);
														}

														sprintf(fieldName, "%d_neighbor", neighborIdx++);
#else
														sprintf(fieldName, "%d_neighbor", neighborIdx++);
#endif
														HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", fieldName, neighbor, 0, 0);
													}

													free((void *)neighbor);
												}
											}
										}
									}
								}
							}
						}
					}
					else
						printf("%s; NO MATCH!!!; GET_PARAM_VALUE(param)= '%s', BSSID= '%s'\n", __FUNCTION__, GET_PARAM_VALUE(param), BSSID);
				}
			}
		}
	}
	printf("%s; run over 'BeeRock.BSS.Neighbor' data-base - END\n\n", __FUNCTION__);

	if (isAtLeastOneNeighborToSend == false)
	{
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "pref", "0", 0, 0);
		printf("%s; isAtLeastOneNeighborToSend= %d ==> set 'pref' to '0'\n", __FUNCTION__, isAtLeastOneNeighborToSend);
	}

	printf("%s; Data Base Object to be sent to bss_transition_management_req():\n", __FUNCTION__);
	setLog(ifname, wlObj, 0);

	res = fapi_wlan_bss_transition_management_req(ifname, wlObj, 0);

	HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
	HELP_DELETE_OBJ(wlObjDB, SOPT_OBJVALUE, FREE_OBJLIST);
	HELP_DELETE_OBJ(wlObjGeneralDB, SOPT_OBJVALUE, FREE_OBJLIST);

	if (wlObjStaDB != NULL)
	{
		HELP_DELETE_OBJ(wlObjStaDB, SOPT_OBJVALUE, FREE_OBJLIST);
	}

	HELP_DELETE_OBJ(wlObjNeighborDB, SOPT_OBJVALUE, FREE_OBJLIST);

	printf("%s; timerSeconds= %d ==> if greater than '0', disable radio!\n", __FUNCTION__, timerSeconds);
	if (timerSeconds > 0)
	{
		printf("%s; timerSeconds= %d ==> disable '%s' radio!\n", __FUNCTION__, timerSeconds, ifname);

		/* Set few second timer */
	    struct sigevent sev;
		struct itimerspec its;
		struct sigaction sa;

		printf("Establishing handler for signal %d\n", SIGUSR1);
		sa.sa_flags = SA_SIGINFO;
		sa.sa_sigaction = timerExpireCB;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGUSR1, &sa, NULL);

		/* Create the timer */
		sev.sigev_notify = SIGEV_SIGNAL;
		sev.sigev_signo = SIGUSR1;
		sev.sigev_value.sival_ptr = &timerid;
		timer_create(CLOCK_REALTIME, &sev, &timerid);

		/* Start the timer */
		its.it_value.tv_sec = timerSeconds;
		its.it_value.tv_nsec = 0;
		its.it_interval.tv_sec = 0;  /* without interval, otherwise it is cyclic */
		its.it_interval.tv_nsec = 0;

		timer_settime(timerid, 0, &its, NULL);

		ifnameOfTimerIdSet(timerid, ifname);  /* Save the timer ID inside the internal database */

		printf("%s; sleep 15 seconds starts\n", __FUNCTION__);
		sleep(15);
		printf("%s; sleep 15 seconds ends\n", __FUNCTION__);
	}

	return res;
}


int ap_manager_lite_beacon_req_get(const char *ifname,
                                   char *MACAddress,
								   char *numOfRepetitions,
								   char *durationMandatory,
								   char *opClass,
								   char *Channel,
								   char *randInt,
								   char *duration,
								   char *mode,
								   char *bssid,
								   char *ssid,         /* Optional Param - can be 'NULL' */
								   char *repDetail,    /* Optional Param - can be 'NULL' */
								   char *repCond,      /* Optional Param - can be 'NULL' */
								   char *apChReport,   /* Optional Param - can be 'NULL' */
								   char *reqElements)  /* Optional Param - can be 'NULL' */
{
	int     res;
	char    measurementReqMode[4];
	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	printf("%s; ifname= '%s', MACAddress= '%s', numOfRepetitions= '%s', durationMandatory= '%s', opClass= '%s', Channel= '%s', randInt= '%s', duration= '%s', mode= '%s', bssid= '%s'\n",
	       __FUNCTION__, ifname, MACAddress, numOfRepetitions, durationMandatory, opClass, Channel, randInt, duration, mode, bssid);

	if (wlObj == NULL)
	{
		printf("%s; create object ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if ( (MACAddress == NULL)        ||
	     (numOfRepetitions == NULL)  ||
	     (durationMandatory == NULL) ||
	     (opClass == NULL)           ||
	     (Channel == NULL)           ||
	     (randInt == NULL)           ||
	     (duration == NULL)          ||
	     (mode == NULL)              ||
	     (bssid == NULL) )
	{
		printf("%s; parameters ERROR\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	sprintf(measurementReqMode, "%d", ((atoi(durationMandatory) & 0x01) << 4));  /* move 4 bits to the left, towards bit #4 */
	printf("%s; measurementReqMode= '%s'\n", __FUNCTION__, measurementReqMode);

	help_addObjList(wlObj, "Device.WiFi.Radio", 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio", "Channel", Channel, 0, 0);

	help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", MACAddress, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "num_of_repetitions", numOfRepetitions, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_req_mode", measurementReqMode, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "op_class", opClass, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rand_int", randInt, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", duration, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "mode", mode, 0, 0);
	HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssid", bssid, 0, 0);

	/* Optional Params */
	if (ssid != NULL)
	{
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ssid", ssid, 0, 0);
	}

	if (repDetail != NULL)
	{
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rep_detail", repDetail, 0, 0);
	}

	if (repCond != NULL)
	{
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rep_cond", repCond, 0, 0);
	}

	if (apChReport != NULL)
	{
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ap_ch_report", apChReport, 0, 0);
	}

	if (reqElements != NULL)
	{
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "req_elements", reqElements, 0, 0);
	}

	printf("%s; Data Base Object to be sent to beacon_request():\n", __FUNCTION__);
	setLog(ifname, wlObj, 0);

	res = fapi_wlan_beacon_request(ifname, wlObj, 0);

	HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);

	return res;
}


void beerock_aml_cli(char *ifname, char **argv, int argc)
{
	int       i;
	char      CellPref[] = "255";
	char      MBOCellAware[] = "0";
	char      *localSSID = NULL;
	char      *rep_detail = NULL;
	char      *rep_cond = NULL;
	char      *ap_ch_report = NULL;
	char      *req_elements = NULL;
	char      *bssTermDuration = NULL;
	char      *disassocImminent = NULL;
	char      *btmReqTermBit = NULL;
	char      *disassocTimer = NULL;
	char      *reassocDelay = NULL;
	char      *bssTermTSF = NULL;
	char      *abridged = NULL;
	char      *valid_int = NULL;
	char      *oce = NULL;
	char      *tbtt_offset = NULL;

	printf("%s Entry; argc= %d\n", __FUNCTION__, argc);

	if (!strcmp(argv[1], "AML_CELL_CONFIG_SET"))
	{
		/*
		Format:   AML_CELL_CONFIG_SET ifname MBOCellAware CellPref
		Examples: AML_CELL_CONFIG_SET wlan0 1 1
		          AML_CELL_CONFIG_SET wlan0 0 NULL
		          AML_CELL_CONFIG_SET wlan0 NULL 255
		*/

		if (argc <= 4)
		{
			printf("ap_manager_lite_cellular_config_set - wrong format\n");
		}
		else
		{
			if (strlen(argv[3]) <= strlen(MBOCellAware))
			{
				strcpy(MBOCellAware, argv[3]);
				if (strlen(argv[4]) <= strlen(CellPref))
				{
					strcpy(CellPref, argv[4]);

					printf("ap_manager_lite_cellular_config_set test; ifname= '%s', MBOCellAware= '%s', CellPref= '%s'\n", ifname, MBOCellAware, CellPref);

					if (ap_manager_lite_cellular_config_set(ifname, MBOCellAware, CellPref) == UGW_FAILURE)
					{
						printf("ap_manager_lite_cellular_config_set ERROR\n");
					}
				}
			}
		}
	}
	else if (!strcmp(argv[1], "AML_BSS_NEIGHBOR_SET"))
	{
		/*
		Format:   AML_BSS_NEIGHBOR_SET ifname nr_BSSID opClass=<> channelNumber=<> priority=<> [oce=<>] [tbtt_offset=<>]
		Examples: AML_BSS_NEIGHBOR_SET wlan2 55:44:33:22:11:00 opClass=51 channelNumber=1 priority=1 oce=1 tbtt_offset=1
		          AML_BSS_NEIGHBOR_SET wlan2 11:22:33:44:55:66 opClass=53 channelNumber=36 priority=100
		          AML_BSS_NEIGHBOR_SET wlan2 11:22:33:44:55:66 opClass=52 channelNumber=157 priority=255
		Note:     Both opClass & channelNumber should get values in decimal.
		          Conversion to 2-character hexadecimal will be performed inside ap_manager_lite_bss_neighbor_set()
		*/

		if (argc <= 6)
		{
			printf("ap_manager_lite_bss_neighbor_set - wrong format\n");
		}
		else
		{
			printf("ap_manager_lite_bss_neighbor_set test; ifname= '%s', nr_BSSID= '%s', opClass= '%s', channelNumber= '%s', priority= '%s'\n",
			       ifname, argv[3], argv[4]+strlen("opClass="), argv[5]+strlen("channelNumber="), argv[6]+strlen("priority="));

			/* Optional Params */
			printf("Optional Params: ");
			for (i=6; i < argc; i++)
			{
				if (!strncmp(argv[i], "oce=", strlen("oce=")))
				{
					oce = (char *)malloc((size_t)(strlen(argv[i]) - strlen("oce=")));
					strcpy(oce, argv[i]+strlen("oce="));
					printf("oce= '%s'; ", oce);
				}
				else if (!strncmp(argv[i], "tbtt_offset=", strlen("tbtt_offset=")))
				{
					tbtt_offset = (char *)malloc((size_t)(strlen(argv[i]) - strlen("tbtt_offset=")));
					strcpy(tbtt_offset, argv[i]+strlen("tbtt_offset="));
					printf("tbtt_offset= '%s'; ", tbtt_offset);
				}
			}
			printf("\n");

			if (ap_manager_lite_bss_neighbor_set(ifname, argv[3], argv[4]+strlen("opClass="), argv[5]+strlen("channelNumber="), argv[6]+strlen("priority="), oce, tbtt_offset) == UGW_FAILURE)
			{
				printf("ap_manager_lite_bss_neighbor_set ERROR\n");
			}

			/* free all allocated fields */
			if (oce != NULL)
			{
				free((void *)oce);
				oce = NULL;
			}

			if (tbtt_offset != NULL)
			{
				free((void *)tbtt_offset);
				tbtt_offset = NULL;
			}
		}
	}
	else if (!strcmp(argv[1], "AML_BTM_PARAMS_SET"))
	{
		/*
		Format:   AML_BTM_PARAMS_SET ifname [bssTermDuration=<>] [disassocImminent=<>] [btmReqTermBit=<>] [disassocTimer=<>] [reassocDelay=<>] [bssTermTSF=<>]
		Examples: AML_BTM_PARAMS_SET wlan2 bssTermDuration=3 disassocImminent=1 btmReqTermBit=0 disassocTimer=1 reassocDelay=2 bssTermTSF=5000
		          AML_BTM_PARAMS_SET wlan2 bssTermDuration=3 disassocImminent=1 btmReqTermBit=0 disassocTimer=1 reassocDelay=2
		          AML_BTM_PARAMS_SET wlan2 bssTermDuration=5 disassocImminent=1 btmReqTermBit=0 disassocTimer=1 reassocDelay=2
		          AML_BTM_PARAMS_SET wlan2 bssTermDuration=7 disassocImminent=1 btmReqTermBit=1 disassocTimer=1 reassocDelay=2
		          AML_BTM_PARAMS_SET wlan2 bssTermDuration=7 disassocImminent=1 btmReqTermBit=1 disassocTimer=1 reassocDelay=2
		          AML_BTM_PARAMS_SET wlan2 bssTermDuration=7 disassocImminent=1 disassocTimer=1
		          AML_BTM_PARAMS_SET wlan2 bssTermDuration=7 disassocImminent=1
		*/

		if (argc <= 3)
		{
			printf("ap_manager_lite_btm_params_set - wrong format\n");
		}
		else
		{
			printf("ap_manager_lite_btm_params_set test; ifname= '%s'\n", ifname);

			/* Optional Params */
			printf("Optional Params: ");
			for (i=3; i < argc; i++)
			{
				if (!strncmp(argv[i], "bssTermDuration=", strlen("bssTermDuration=")))
				{
					bssTermDuration = (char *)malloc((size_t)(strlen(argv[i]) - strlen("bssTermDuration=")));
					strcpy(bssTermDuration, argv[i]+strlen("bssTermDuration="));
					printf("bssTermDuration= '%s'; ", bssTermDuration);
				}
				else if (!strncmp(argv[i], "disassocImminent=", strlen("disassocImminent=")))
				{
					disassocImminent = (char *)malloc((size_t)(strlen(argv[i]) - strlen("disassocImminent=")));
					strcpy(disassocImminent, argv[i]+strlen("disassocImminent="));
					printf("disassocImminent= '%s'; ", disassocImminent);
				}
				else if (!strncmp(argv[i], "btmReqTermBit=", strlen("btmReqTermBit=")))
				{
					btmReqTermBit = (char *)malloc((size_t)(strlen(argv[i]) - strlen("btmReqTermBit=")));
					strcpy(btmReqTermBit, argv[i]+strlen("btmReqTermBit="));
					printf("btmReqTermBit= '%s'; ", btmReqTermBit);
				}
				else if (!strncmp(argv[i], "disassocTimer=", strlen("disassocTimer=")))
				{
					disassocTimer = (char *)malloc((size_t)(strlen(argv[i]) - strlen("disassocTimer=")));
					strcpy(disassocTimer, argv[i]+strlen("disassocTimer="));
					printf("disassocTimer= '%s'; ", disassocTimer);
				}
				else if (!strncmp(argv[i], "reassocDelay=", strlen("reassocDelay=")))
				{
					reassocDelay = (char *)malloc((size_t)(strlen(argv[i]) - strlen("reassocDelay=")));
					strcpy(reassocDelay, argv[i]+strlen("reassocDelay="));
					printf("reassocDelay= '%s'; ", reassocDelay);
				}
				else if (!strncmp(argv[i], "bssTermTSF=", strlen("bssTermTSF=")))
				{
					bssTermTSF = (char *)malloc((size_t)(strlen(argv[i]) - strlen("bssTermTSF=")));
					strcpy(bssTermTSF, argv[i]+strlen("bssTermTSF="));
					printf("bssTermTSF= '%s'; ", bssTermTSF);
				}
			}
			printf("\n");

			if (ap_manager_lite_btm_params_set(ifname, bssTermDuration, disassocImminent, btmReqTermBit, disassocTimer, reassocDelay, bssTermTSF) == UGW_FAILURE)
			{
				printf("ap_manager_lite_btm_params_set ERROR\n");
			}

			/* free all allocated fields */
			if (bssTermDuration != NULL)
			{
				free((void *)bssTermDuration);
				bssTermDuration = NULL;
			}

			if (disassocImminent != NULL)
			{
				free((void *)disassocImminent);
				disassocImminent = NULL;
			}

			if (btmReqTermBit != NULL)
			{
				free((void *)btmReqTermBit);
				btmReqTermBit = NULL;
			}

			if (disassocTimer != NULL)
			{
				free((void *)disassocTimer);
				disassocTimer = NULL;
			}

			if (reassocDelay != NULL)
			{
				free((void *)reassocDelay);
				reassocDelay = NULL;
			}
		}
	}
	else if (!strcmp(argv[1], "AML_ASSOC_DISALLOW"))
	{
		/*
		Format:   AML_ASSOC_DISALLOW ifname Reason-Code
		Examples: AML_ASSOC_DISALLOW wlan2 1
		          AML_ASSOC_DISALLOW wlan0 2
		Notes:    Reason-Code: 1 - 5
		          When using '0' it will allow it back
		*/

		if (argc <= 3)
		{
			printf("ap_manager_lite_association_disallow - wrong format\n");
		}
		else
		{
			printf("ap_manager_lite_association_disallow test; ifname= '%s', ReasonCode= '%s'\n", ifname, argv[3]);

			if (ap_manager_lite_association_disallow(ifname, argv[3]) == UGW_FAILURE)
			{
				printf("ap_manager_lite_association_disallow ERROR\n");
			}
		}
	}
	else if (!strcmp(argv[1], "AML_DISASSOC_SET"))
	{
		/*
		Format:   AML_DISASSOC_SET ifname MACAddress
		Examples: AML_DISASSOC_SET wlan2 6c:72:20:02:e8:33
		          AML_DISASSOC_SET wlan0 6c:72:20:02:e8:33
		*/

		if (argc <= 3)
		{
			printf("ap_manager_lite_disassociate_set - wrong format\n");
		}
		else
		{
			printf("ap_manager_lite_disassociate_set test; ifname= '%s', MACAddress= '%s'\n", ifname, argv[3]);

			if (ap_manager_lite_disassociate_set(ifname, argv[3]) == UGW_FAILURE)
			{
				printf("ap_manager_lite_disassociate_set ERROR\n");
			}
		}
	}
	else if (!strcmp(argv[1], "AML_BTM_REQ"))
	{
		/*
		Format:   AML_BTM_REQ ifname MACAddress pref=<> reason=<> [disassocTimer=<>] [abridged=<>] [valid_int=<>]
		Examples: AML_BTM_REQ wlan0 44:85:00:C5:6A:1B pref=1 reason=5 disassocTimer=1 abridged=0 valid_int=10
		          AML_BTM_REQ wlan0 44:85:00:C5:6A:1B pref=1 reason=5 disassocTimer=1 abridged=0
		          AML_BTM_REQ wlan0 44:85:00:C5:6A:1B pref=1 reason=5 disassocTimer=1
		          AML_BTM_REQ wlan0 44:85:00:C5:6A:1B pref=1 reason=5
		*/

		if (argc <= 5)
		{
			printf("ap_manager_lite_btm_req - wrong format\n");
		}
		else
		{
			printf("ap_manager_lite_btm_req test; ifname= '%s', MACAddress= '%s', dialog_token='0' (hard-coded), pref='%s', reason='%s'\n",
			       ifname, argv[3], argv[4]+strlen("pref="), argv[5]+strlen("reason="));

			/* Optional Params */
			printf("Optional Param: ");
			for (i=6; i < argc; i++)
			{
				if (!strncmp(argv[i], "disassocTimer=", strlen("disassocTimer=")))
				{
					disassocTimer = (char *)malloc((size_t)(strlen(argv[i]) - strlen("disassocTimer=")));
					strcpy(disassocTimer, argv[i]+strlen("disassocTimer="));
					printf("disassocTimer= '%s'; ", disassocTimer);
				}
				else if (!strncmp(argv[i], "abridged=", strlen("abridged=")))
				{
					abridged = (char *)malloc((size_t)(strlen(argv[i]) - strlen("abridged=")));
					strcpy(abridged, argv[i]+strlen("abridged="));
					printf("abridged= '%s'; ", abridged);
				}
				else if (!strncmp(argv[i], "valid_int=", strlen("valid_int=")))
				{
					valid_int = (char *)malloc((size_t)(strlen(argv[i]) - strlen("valid_int=")));
					strcpy(valid_int, argv[i]+strlen("valid_int="));
					printf("valid_int= '%s'; ", valid_int);
				}
			}
			printf("\n");

			/* When called NOT upon BSS-TM-QUERY report, set the dialog_token to '0' */
			if (ap_manager_lite_btm_req(ifname, argv[3], "0" /*dialog_token*/, argv[4]+strlen("pref="), argv[5]+strlen("reason="), disassocTimer, abridged, valid_int) == UGW_FAILURE)
			{
				printf("ap_manager_lite_btm_req ERROR\n");
			}

			/* free all allocated fields */
			if (disassocTimer != NULL)
			{
				free((void *)disassocTimer);
				disassocTimer = NULL;
			}
		}
	}
	else if (!strcmp(argv[1], "AML_BEACON_REQ_GET"))
	{
		/*
		Format:   AML_BEACON_REQ_GET ifname MACAddress numOfRepetitions=<> durationMandatory=<> opClass=<> Channel=<> randInt=<> duration=<> mode=<> bssid=<>
		                         [ssid=<>] [rep_detail=<>] [rep_cond=<>] [ap_ch_report=<>] [req_elements=<>]
		Examples: AML_BEACON_REQ_GET wlan2 44:85:00:C5:6A:1B numOfRepetitions=0 durationMandatory=0 opClass=0 Channel=1 randInt=1000 duration=50 mode=passive bssid=00:0A:1B:0E:04:60
		                         'ssid="widan test"' rep_detail=2 rep_cond=0 ap_ch_report=1,2,3 req_elements=0,51
		Notes:
		1) durationMandatory is bit #4 of measurementReqMode, and will create it inside ap_manager_lite_beacon_req_get()
		2) the last 6 parameters are optional.
		*/

		if (argc <= 11)
		{
			printf("ap_manager_lite_beacon_req_get - wrong format\n");
		}
		else
		{
			printf("ap_manager_lite_beacon_req_get test; ifname= '%s', MACAddress= '%s', numOfRepetitions= '%s', durationMandatory= '%s', opClass= '%s', Channel= '%s', randInt= '%s', duration= '%s', mode= '%s', bssid= '%s'\n",
			       ifname, argv[3], argv[4]+strlen("numOfRepetitions="), argv[5]+strlen("durationMandatory="), argv[6]+strlen("opClass="), argv[7]+strlen("Channel="),
				   argv[8]+strlen("randInt="), argv[9]+strlen("duration="), argv[10]+strlen("mode="), argv[11]+strlen("bssid="));

			/* Optional Params */
			printf("Optional Params: ");
			for (i=12; i < argc; i++)
			{
				if (!strncmp(argv[i], "ssid=", strlen("ssid=")))
				{
					localSSID = (char *)malloc((size_t)(strlen(argv[i]) - strlen("ssid=")));
					strcpy(localSSID, argv[i]+strlen("ssid="));
					printf("ssid= '%s'; ", localSSID);
				}
				else if (!strncmp(argv[i], "rep_detail=", strlen("rep_detail=")))
				{
					rep_detail = (char *)malloc((size_t)(strlen(argv[i]) - strlen("rep_detail=")));
					strcpy(rep_detail, argv[i]+strlen("rep_detail="));
					printf("rep_detail= '%s'; ", rep_detail);
				}
				else if (!strncmp(argv[i], "rep_cond=", strlen("rep_cond=")))
				{
					rep_cond = (char *)malloc((size_t)(strlen(argv[i]) - strlen("rep_cond=")));
					strcpy(rep_cond, argv[i]+strlen("rep_cond="));
					printf("rep_cond= '%s'; ", rep_cond);
				}
				else if (!strncmp(argv[i], "ap_ch_report=", strlen("ap_ch_report=")))
				{
					ap_ch_report = (char *)malloc((size_t)(strlen(argv[i]) - strlen("ap_ch_report=")));
					strcpy(ap_ch_report, argv[i]+strlen("ap_ch_report="));
					printf("ap_ch_report= '%s'; ", ap_ch_report);
				}
				else if (!strncmp(argv[i], "req_elements=", strlen("req_elements=")))
				{
					req_elements = (char *)malloc((size_t)(strlen(argv[i]) - strlen("req_elements=")));
					strcpy(req_elements, argv[i]+strlen("req_elements="));
					printf("req_elements= '%s'; ", req_elements);
				}
			}
			printf("\n");

			if (ap_manager_lite_beacon_req_get(ifname,
			                                   argv[3],
											   argv[4]+strlen("numOfRepetitions="),
											   argv[5]+strlen("durationMandatory="),
											   argv[6]+strlen("opClass="),
											   argv[7]+strlen("Channel="),
											   argv[8]+strlen("randInt="),
											   argv[9]+strlen("duration="),
											   argv[10]+strlen("mode="),
											   argv[11]+strlen("bssid="),
											   localSSID,
											   rep_detail,
											   rep_cond,
											   ap_ch_report,
											   req_elements) == UGW_FAILURE)
			{
				printf("ap_manager_lite_beacon_req_get ERROR\n");
			}

			/* free all allocated fields */
			if (localSSID != NULL)
			{
				free((void *)localSSID);
				localSSID = NULL;
			}

			if (rep_detail != NULL)
			{
				free((void *)rep_detail);
				rep_detail = NULL;
			}

			if (rep_cond != NULL)
			{
				free((void *)rep_cond);
				rep_cond = NULL;
			}

			if (ap_ch_report != NULL)
			{
				free((void *)ap_ch_report);
				ap_ch_report = NULL;
			}

			if (req_elements != NULL)
			{
				free((void *)req_elements);
				req_elements = NULL;
			}
		}
	}
}


int fapiWlanRefCallBackFunc(const char *ifname, char *buf)
{
	printf("%s; ifname= '%s'; \nbuf= '%s'\n", __FUNCTION__, ifname, buf);

	return UGW_SUCCESS;
}


int fapiWlanCallBackApManagerLiteFunc(char *opCode, const char *ifname, ObjList *wlObj, unsigned int flags, void *context)
{
	(void)flags;
	(void)context;

	printf("%s Entry; opCode= '%s', wlObj= 0x%x\n", __FUNCTION__, opCode, (unsigned int)wlObj);

	if (!strcmp(opCode, "AP-STA-CONNECTED"))
	{
		if (fapiWlanStaConnectReportHandle(ifname, wlObj) == UGW_FAILURE)
			printf("%s; fapiWlanStaConnectReportHandle returned ERROR\n", __FUNCTION__);
	}
	else if (!strcmp(opCode, "AP-STA-WNM-NOTIF"))
	{
		if (fapiWlanStaWnmNotifHandle(ifname, wlObj) == UGW_FAILURE)
			printf("%s; fapiWlanStaWnmNotifHandle returned ERROR\n", __FUNCTION__);
	}
	else if (!strcmp(opCode, "RRM-BEACON-REP-RECEIVED"))
	{
		if (fapiWlanBeaconReportHandle(ifname, wlObj) == UGW_FAILURE)
			printf("%s; fapiWlanBeaconReportHandle returned ERROR\n", __FUNCTION__);
	}
	else if (!strcmp(opCode, "BSS-TM-QUERY"))
	{
		if (fapiWlanBssTmQueryReportHandle(ifname, wlObj) == UGW_FAILURE)
			printf("%s; fapiWlanBssTmQueryReportHandle returned ERROR\n", __FUNCTION__);
	}

	return UGW_SUCCESS;
}

