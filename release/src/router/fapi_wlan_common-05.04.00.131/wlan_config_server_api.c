/*##################################################################################################
# "Copyright (c) 2013 Intel Corporation                                                            #
# DISTRIBUTABLE AS SAMPLE SOURCE SOFTWARE                                                          #
# This Distributable As Sample Source Software is subject to the terms and conditions              #
# of the Intel Software License Agreement for the Intel(R) Cable and GW Software Development Kit"  #
##################################################################################################*/

/*! \file 	wlan_config_server_api.c
	\brief 	This file implements the WLAN FAPI RPC layer.
	\todo 	Add license header
	\todo   Move all system() function uses to clean C functions
	\todo   merge all db load\store functions to one
	\todo   Solve all mac addresses workarounds PUMA \ UBOOT  \ UGW
	\todo   Create script that align DB sources to UGW version and converts to RPC format, a must!!!!!
	\todo   Add null pointer verifications and clean-up
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdarg.h>
#include <dirent.h>

#include "fapi_wlan_private.h"
#include "fapi_wlan.h"
#include "fapi_wlan_tools.h"
#include "fapi_wlan_beerock.h"
#include "wlan_config_api.h"
#include "help_objlist.h"

#include "wlan_config_api.h"

typedef int(*FapiWlanQuery)(const char* ifname, ObjList *wlObj, unsigned int flags);
typedef int(*FapiWlanSetFunc)(const char* ifname, ObjList *wlObj, unsigned int flags);

extern ObjList *mapperObjPtr;

#define MAC_LENGTH 17

enum setVapMode
{
	initVap = 0,
	addVap
};

static bool verbose = 1;

/*! \def dbg_print
 *  \brief Log a debug message
 */
#define dbg_print(fmt, args...) do { if (verbose) fprintf(stdout, "%s " fmt, __func__, ##args); } while(0)

 /*! \def print_err
  *  \brief Log an error
  */
#define print_err(fmt, args...) do { if (verbose) fprintf(stderr, "Error: %s " fmt, __func__, ##args); } while(0)

static int strRemoveAll(char *src, char *key)
{
	while (*src)
	{
		char *k = key, *s = src;
		while (*k && *k == *s) ++k, ++s;
		if (!*k)
		{
			while (*s) *src++ = *s++;
			*src = 0;
			printf("strRemoveAll Removed %s new string: %s\n", key, src);
			return 1;
		}
		++src;
	}
	return 0;
}

static int checkLegalMac(char *str)
{
	const char digiLookUp[] = ":,0123456789abcdefABCDEF";
	char singleChar[2] = { '\0', '\0' };
	int i;

	if (strlen(str) < MAC_LENGTH)
	{
		printf("checkLegalMac:FAILED MACADDRESS LENGHT < MAC_LENGTH\n");
		return UGW_FAILURE;
	}

	for (i = 0; i < (int)strlen(str); i++)
	{
		singleChar[0] = str[i];
		if (strstr(digiLookUp, singleChar) == NULL)
		{
			printf("MACADDRESS has digit not in lookup -> ',:0123456789abcdefABCDEF' \n");
			return UGW_FAILURE;
		}
	}

	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int checkInterfaceIsRadio(int radioIndex)
**************************************************************************
*  \brief returns true if interface is radio
*  \param[in] int index - AP Index
*  \return 1 if radio, 0 else
***************************************************************************/
static int checkInterfaceIsRadio(int radioIndex)
{
	ObjList	  *tempLoopObj = NULL;
	ParamList *param = NULL;
	char      *radioName = NULL;
	char      ifName[MAX_LEN_PARAM_VALUE];

	fapiWlanFailSafeLoad();
	sprintf(ifName, "%s%d", "wlan", radioIndex);

	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		radioName = NULL;
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
			{
				radioName = GET_PARAM_VALUE(param);
				if (!strcmp(radioName, ifName))
				{
					return 1;
				}
			}
		}
	}

	printf("__FUNCTION__ ERROR: The interface is not radio\n");
	return 0;
}


/**************************************************************************/
/*! \fn int checkIfExists(int apIndex)
**************************************************************************
*  \brief returns true if interface is presented
*  \param[in] int index - AP Index
*  \return 1 if found, 0 else
***************************************************************************/
static int checkIfExists(int apIndex)
{
	ObjList	  *tempLoopObj = NULL;
	ParamList *param = NULL;
	char      *rpcIndex = NULL;
	char	rpcName[MAX_LEN_PARAM_VALUE];

	fapiWlanFailSafeLoad();
	sprintf(rpcName, "%s%d", "wlan", apIndex);

	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RPC_INDEX, GET_PARAM_NAME(param)))
			{
				rpcIndex = GET_PARAM_VALUE(param);
				if (!strcmp(rpcIndex, rpcName))
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

// ### HELPER FUNCTIONS ###
// TODO merge with FAPI parallel function
static int wlanStoreToDB(char *ifName, ObjList *ObjPtr)
{
	int return_status = 0;
	char fullPath[MAX_LEN_OBJNAME];
	char objectName[MAX_LEN_OBJNAME];

	ObjList *tmpHwInitObj = NULL;
	ObjList *tmpStoreObj = NULL;
	ObjList *fullDbObjPtr = NULL;

	FOR_EACH_OBJ(ObjPtr, tmpHwInitObj)
	{
		fullPath[0] = '\0';
		objectName[0] = '\0';

		strncpy(objectName, GET_OBJ_NAME(tmpHwInitObj), MAX_LEN_OBJNAME);
		snprintf(fullPath, sizeof(fullPath), "%s%s/%s", DB_PATH, ifName, objectName);

		fullDbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
		FAPI_WLAN_ASSERT(fullDbObjPtr);
		tmpStoreObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
		FAPI_WLAN_ASSERT(tmpStoreObj);

		if (HELP_LOADLOCALDB(fullDbObjPtr, fullPath) == UGW_FAILURE)
		{
			HELP_DELETE_OBJ(fullDbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_FAILURE;
		}

		HELP_COPY_OBJ(tmpStoreObj, tmpHwInitObj, SOPT_OBJVALUE, COPY_SINGLE_OBJ);

		// Merge the returned obj with the complete db file obj
		if (HELP_MERGE_OBJLIST(fullDbObjPtr, tmpStoreObj) == UGW_FAILURE)
		{
			HELP_DELETE_OBJ(fullDbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_FAILURE;
		}

		HELP_DELETE_OBJ(tmpStoreObj, SOPT_OBJVALUE, FREE_OBJLIST);

		// TODO, add clean merge API without dot and clean the duplicate code
		FOR_EACH_OBJ(fullDbObjPtr, tmpStoreObj)
		{
			if (tmpStoreObj->sObjName[strlen(tmpStoreObj->sObjName) - 1] == '.')
			{
				tmpStoreObj->sObjName[strlen(tmpStoreObj->sObjName) - 1] = '\0';
			}
		}

		if (HELP_STORELOCALDB(fullDbObjPtr, fullPath) == UGW_FAILURE)
		{
			return UGW_FAILURE;
		}
		else {
#if 0
			logger("FAPI_WLAN_COMMON, HELP_STORELOCALDB to '%s' STATUS '%d' \n", fullPath, return_status);
#else
			;
#endif
		}

		HELP_DELETE_OBJ(fullDbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
	}

	return UGW_SUCCESS;
}


/* Native FAPI - START */
static int fapi_wlan_beerock_generic_set(FapiWlanSetFunc fapi_wlan_set_func, int index, ObjList *dbObjPtr, unsigned int flags)
{
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	(void)flags;  //unused parameter
	
	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	if (dbObjPtr != NULL)
		setLog(interfaceName, dbObjPtr, 0);

	if (fapi_wlan_set_func(interfaceName, dbObjPtr, 0) != 0)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	return UGW_SUCCESS;
}

static int fapi_wlan_beerock_generic_get(FapiWlanSetFunc fapi_wlan_set_func, int index, ObjList *dbObjPtr, unsigned int flags)
{
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	(void)flags;  //unused parameter

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	if (fapi_wlan_set_func(interfaceName, dbObjPtr, 0) != 0)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	setLog(interfaceName, dbObjPtr, 1);

	return UGW_SUCCESS;
}

static int fapi_wlan_generic_set(FapiWlanSetFunc fapi_wlan_set_func, int index, ObjList *dbObjPtr, unsigned int flags)
{
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	(void)flags;  //unused parameter

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	if (fapi_wlan_set_func(interfaceName, dbObjPtr, 0) != 0)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	if (wlanStoreToDB(interfaceName, dbObjPtr) != UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	return UGW_SUCCESS;
}

//TODO IMPLEMENT
#if 0
static int wlanUpdateHwInitReturnedObjectToDB(ObjList *ObjPtr)
{
	/* Please note: there's a problem with running inside the object -
	   each sub-object (pxTmpObj) can't be accessed directly */
	ObjList *pxTmpObj;
	int     i = 0;
	char    ifName[MAX_LEN_PARAM_VALUE];

	FOR_EACH_OBJ(ObjPtr, pxTmpObj)
	{
		if (!strcmp(GET_OBJ_NAME(pxTmpObj), DEVICE_RADIO_VENDOR))
		{
			sprintf(ifName, "%s%d", "wlan", i);
			//wlanStoreToDB(DEVICE_RADIO_VENDOR, ifName, pxTmpObj);

			i++;
		}
		else if (!strcmp(GET_OBJ_NAME(pxTmpObj), DEVICE_WIFI_NAME))
		{
			//wlanStoreToDB(DEVICE_WIFI_NAME, '\0', pxTmpObj);
		}
		else if (!strcmp(GET_OBJ_NAME(pxTmpObj), DEVICE_RADIO))
		{
			sprintf(ifName, "%s%d", "wlan", i);
			//wlanStoreToDB(DEVICE_RADIO, ifName, pxTmpObj);

			i++;
		}

		i &= 0x01;
	}

	return UGW_SUCCESS;
}
#endif

static ObjList *dbObjPtrSet(ObjList *dbObjPtr, char *objName, char *field, char *string)
{
	ObjList *objPtr = NULL;

	if (dbObjPtr == NULL)
	{
		dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
		if (dbObjPtr == NULL)
		{
			return NULL;
		}

		objPtr = help_addObjList(dbObjPtr, objName, 0, 0, 0, 0);

		if (objPtr == NULL)
		{
			return NULL;
		}
	}

	if (HELP_EDIT_NODE(dbObjPtr, objName, field, string, 0, 0) == UGW_FAILURE)
	{
		return NULL;
	}

	return dbObjPtr;
}

int wlanLoadFromDB(char *pcName, char *ifName, ObjList *ObjPtr)
{
	int return_status = UGW_FAILURE;
	char fullPath[MAX_LEN_PARAM_VALUE] = { '\0' };
	if ((pcName[0] == '/') || ((pcName[0] == '"') && (pcName[1] == '/')))
	{
		strncpy(fullPath, pcName, MAX_LEN_PARAM_VALUE);
	}
	else {
		snprintf(fullPath, sizeof(fullPath), "%s%s/%s", DB_PATH, ifName, pcName);
	}

	ObjList *tmpObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(tmpObj);

	// Load the DB from FLASH to RAM
	if (file_exists(fullPath))
	{
		//TBD WORKAROUND UNTIL LIBHELPER FUNCTION FIXED
		if (!strcmp(pcName, WLAN_OBJECT_MAPPER) || !strcmp(pcName, LAST_SAVED_VAPS_DB))
		{
			return_status = HELP_LOADLOCALDB(ObjPtr, fullPath);
			debug_obj(ObjPtr);
		}
		else {
			return_status = HELP_LOADLOCALDB(tmpObj, fullPath);
			return_status += HELP_MERGE_OBJLIST(ObjPtr, tmpObj);
		}

		HELP_DELETE_OBJ(tmpObj, SOPT_OBJVALUE, FREE_OBJLIST);

		// merge API could add . at the end of the object
		FOR_EACH_OBJ(ObjPtr, tmpObj)
		{
			if (tmpObj->sObjName[strlen(tmpObj->sObjName) - 1] == '.')
			{
				tmpObj->sObjName[strlen(tmpObj->sObjName) - 1] = '\0';
			}
		}

#if 0
		logger("FAPI_WLAN_COMMON, HELP_LOADLOCALDB loading from '%s' STATUS '%d' \n", fullPath, return_status);
#endif
	}
	else
	{
		logger("FAPI_WLAN_COMMON: file '%s' does NOT exist! STATUS '%d'\n", fullPath, return_status);
	}

	return return_status;
}

static char *paramValueGet(ObjList *ObjPtr, int index, char *objName, char *paramName)
{
	char      *paramValue = NULL;
	char      interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if (ObjPtr == NULL)
	{
		printf("__FUNCTION__ ERROR: ObjPtr is NULL return failure!\n");
		return paramValue;
	}

	if (index != -1)
	{
		if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
		{
			return NULL;
		}
	}

	if (wlanLoadFromDB(objName, interfaceName, ObjPtr) != UGW_SUCCESS)
	{
		return NULL;
	}

	return paramValueFromObjBufGet(ObjPtr, objName, paramName);
}

/**************************************************************************/
/*! \fn int setVapHelper(int setVapMode, char *radioName, char *vapName, char *vapIndex, char *essid, bool hideSsid)
**************************************************************************
*  \brief helper function that supports addvap in different modes: addvap, init-vap for main\VAP interfaces
*  \return 0 if success, negative if error / timeout
***************************************************************************/
static int setVapHelper(int setVapMode, char *radioName, char *vapName, char *vapIndex, char *essid, bool hideSsid)
{
	ObjList   *objPtr = NULL;
	ParamList *param = NULL;
	char      *fapiVapName = NULL;
	ObjList   *wlObjRPC = NULL;
	char      command[MAX_LEN_PARAM_VALUE];
	char      fapiVapNameStr[MAX_LEN_PARAM_VALUE];
	char      boolString[] = "false";
	int       i;

	wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(wlObjRPC);
	FAPI_WLAN_ASSERT(!wlanLoadFromDB(DEVICE_SSID, vapName, wlObjRPC));

	if (setVapMode == addVap)
	{
		FAPI_WLAN_ASSERT(!HELP_EDIT_NODE(wlObjRPC, DEVICE_SSID, "SSID", essid, 0, 0));
	}

	fapi_wlan_ssid_add(radioName, wlObjRPC, 0);

	FOR_EACH_OBJ(wlObjRPC, objPtr)
	{
		if (!strcmp(GET_OBJ_NAME(objPtr), DEVICE_SSID))
		{
			FOR_EACH_PARAM(objPtr, param)
			{
				if (!strcmp(INTERFACE_NAME_FIELD, GET_PARAM_NAME(param)))
				{
					fapiVapName = strncpy(fapiVapNameStr, GET_PARAM_VALUE(param), MAX_LEN_PARAM_VALUE);
					FAPI_WLAN_ASSERT(fapiVapName);
				}
			}
		}
	}

	if (!fapiVapName){
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		FAPI_WLAN_ASSERT(fapiVapName);
	}
	if (setVapMode == addVap)
	{
		snprintf(command, sizeof(command), "mv %s%s %s%s", DB_PATH, NEW_VAP, DB_PATH, fapiVapName);
		system(command); //TODO REPLACE BY CLEAN C FUNCTION
	}

	objPtr = HELP_GETOBJPTR(mapperObjPtr, DB_IFNAME, fapiVapName);
	FAPI_WLAN_ASSERT(!HELP_EDIT_SELF_NODE(objPtr, WLAN_OBJECT_MAPPER, DB_RPC_INDEX, vapIndex, 0, 0));

	fapi_wlan_ssid_set(fapiVapName, wlObjRPC, 0);

	FAPI_WLAN_ASSERT(wlanStoreToDB(fapiVapName, wlObjRPC) == UGW_SUCCESS);
	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

	wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(wlObjRPC);
	FAPI_WLAN_ASSERT(!wlanLoadFromDB(DEVICE_ACCESS_POINT, fapiVapName, wlObjRPC));
	FAPI_WLAN_ASSERT(!wlanLoadFromDB(DEVICE_ACCESS_POINT_VENDOR, fapiVapName, wlObjRPC));

	if (setVapMode == addVap)
	{
		if (!hideSsid)
		{
			strcpy(boolString, "true");
		}
		dbObjPtrSet(wlObjRPC, DEVICE_SSID, "SSIDAdvertisementEnabled", boolString);
	}

	fapi_wlan_ap_set(fapiVapName, wlObjRPC, 0);
	FAPI_WLAN_ASSERT(wlanStoreToDB(fapiVapName, wlObjRPC) == UGW_SUCCESS);
	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

	wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(wlObjRPC);
	FAPI_WLAN_ASSERT(!wlanLoadFromDB(DEVICE_ACCESS_POINT_SECURITY, fapiVapName, wlObjRPC));
	FAPI_WLAN_ASSERT(!wlanLoadFromDB(DEVICE_ACCESS_POINT_SECURITY_VENDOR, fapiVapName, wlObjRPC));
	fapi_wlan_security_set(fapiVapName, wlObjRPC, 0);
	FAPI_WLAN_ASSERT(wlanStoreToDB(fapiVapName, wlObjRPC) == UGW_SUCCESS);
	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

	wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(wlObjRPC);
	FAPI_WLAN_ASSERT(!wlanLoadFromDB(DEVICE_ACCESS_POINT_WPS, fapiVapName, wlObjRPC));
	FAPI_WLAN_ASSERT(!wlanLoadFromDB(DEVICE_INFO, "", wlObjRPC));
	FAPI_WLAN_ASSERT(!wlanLoadFromDB(DEVICE_RADIO_WPS_VENDOR, radioName, wlObjRPC));
	fapi_wlan_wps_set(fapiVapName, wlObjRPC, 0);
	//Disable Storing until full WPS feature mergeed from 05.04.00.70+
	//The BUG is that objects from Device info was storred to vap\radio
	//FAPI_WLAN_ASSERT(wlanStoreToDB(fapiVapName, wlObjRPC) == UGW_SUCCESS);
	
	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

	/* When adding VAP, AC inherits from the main radio */
	wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(wlObjRPC);
	for (i = 0; i < NUM_OF_ACCESS_CATEGORIES; i++)
	{
		sprintf(command, "%s.%d", DEVICE_ACCESS_POINT_AC, i + 1);
		if (wlanLoadFromDB(command, fapiVapName, wlObjRPC) == UGW_FAILURE)
		{
			HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_FAILURE;
		}
	}
	fapi_wlan_wmm_ap_set(fapiVapName, wlObjRPC, 0);
	FAPI_WLAN_ASSERT(wlanStoreToDB(fapiVapName, wlObjRPC) == UGW_SUCCESS);
	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}


int securityStateClear(char *ifName)
{
	char command[MAX_LEN_PARAM_VALUE];
	char fullPath[MAX_LEN_PARAM_VALUE];

	snprintf(fullPath, sizeof(fullPath), "%s%s/%s.db", DB_PATH, ifName, DEVICE_SECURITY_STATE);
	snprintf(command, sizeof(command), "rm -rf %s", fullPath);
	system(command);

	return UGW_SUCCESS;
}


int getInterfaceName(int rpc_index, char *interfaceName)
{
	ObjList	  *tempLoopObj = NULL, *rpcIndexObj = NULL;
	ParamList *param = NULL;
	char      vapIndex[MAX_LEN_PARAM_VALUE];

	if (fapiWlanFailSafeLoad() != UGW_SUCCESS)
	{
		return UGW_FAILURE;
	}

	sprintf(vapIndex, "%s%d", "wlan", rpc_index);

	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RPC_INDEX, GET_PARAM_NAME(param)))
			{
				if (!strcmp(vapIndex, GET_PARAM_VALUE(param)))
				{
					rpcIndexObj = tempLoopObj;
				}
			}
		}
	}

	if (rpcIndexObj == NULL)
	{
		return UGW_FAILURE;
	}

	FOR_EACH_PARAM(rpcIndexObj, param)
	{
		if (!strcmp(DB_IFNAME, GET_PARAM_NAME(param)))
		{
			strcpy(interfaceName, GET_PARAM_VALUE(param));
		}
	}

	if (interfaceName[0] == '\0')
	{
		return UGW_FAILURE;
	}

	return UGW_SUCCESS;
}

int wlan_createInitialConfigFiles(char *xmlPath)
{
	char command[MAX_LEN_PARAM_VALUE];
	
	if ( xmlPath != NULL )
	{
		snprintf(command, sizeof(command), "%s/fapi_wlan_wave_createDB.sh %s", SCRIPTS_PATH, xmlPath);
	} else {
		snprintf(command, sizeof(command), "%s/fapi_wlan_wave_createDB.sh", SCRIPTS_PATH);
	}
	
	system(command);

	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_createVap(int vap,int radio, char *essid,bool hideSsid)
 **************************************************************************
 *  \brief create virtual access point
 *  \param[in] int vap - virtual access point
 *  \param[in] int radio - Radio Index
 *  \param[in] char *essid - SSID Name
 *  \param[in] bool hideSsid - true/false
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_createVap(int vap, int radio, char *essid, bool hideSsid)
{
	char command[MAX_LEN_PARAM_VALUE];

	//VAP Index, will be added to mapper DB
	char vapIndex[MAX_LEN_PARAM_VALUE];
	sprintf(vapIndex, "%s%d", "wlan", vap);

	//FAPI Radio Index
	char radioName[MAX_LEN_PARAM_VALUE];
	sprintf(radioName, "%s%d", "wlan", radio);

	fapiWlanFailSafeLoad();

	if (checkIfExists(vap))
	{
		wlan_setSSID(vap, essid);
		wlan_setSsidAdvertisementEnabled(vap, hideSsid);
		printf("VAP already exists in nvram, updating SSID and Advertisement");
		return UGW_SUCCESS;
	}

	sprintf(command, "cp -r %s%s %s%s", DB_PATH_DEFAULT, "vap", DB_PATH, NEW_VAP);
	system(command); //TODO REPLACE BY CLEAN C FUNCTION

	sprintf(command, "cp -r %s%s%d/Device.WiFi.AccessPoint.AC.* %s%s/.", DB_PATH_DEFAULT, "radio", radio, DB_PATH, NEW_VAP);
	system(command); //TODO REPLACE BY CLEAN C FUNCTION

	setVapHelper(addVap, radioName, NEW_VAP, vapIndex, essid, hideSsid);

	/* due to TCH requirement wont up automatically
	wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	fapi_wlan_up(radioName, wlObjRPC, 1); //Up the relevant radio
	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
	*/

	fapiWlanFailSafeStore();

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_deleteVap(int vap)
 **************************************************************************
 *  \brief delete virtual access point
 *  \param[in] int vap - virtual access point
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_deleteVap(int vap)
{
	ObjList   *wlObjRPC = NULL;
	ObjList	  *tempLoopObj = NULL;
	ParamList *param = NULL;

	char *currentIfName = NULL;
	char *currentVapIndex = NULL;
	char *currentRadioName = NULL;
	char *ifName = NULL;
	char *radioName = NULL;

	char command[MAX_LEN_PARAM_VALUE];
	char vapIndex[MAX_LEN_PARAM_VALUE];

	sprintf(vapIndex, "%s%d", "wlan", vap);

	fapiWlanFailSafeLoad();


	if (checkInterfaceIsRadio(vap) != UGW_SUCCESS)
	{
		return wlan_ifconfigDown(vap);
	}


	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		currentVapIndex = NULL;
		currentIfName = NULL;
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RPC_INDEX, GET_PARAM_NAME(param)))
			{
				currentVapIndex = GET_PARAM_VALUE(param);
			}

			if (!strcmp(DB_IFNAME, GET_PARAM_NAME(param)))
			{
				currentIfName = GET_PARAM_VALUE(param);
			}

			if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
			{
				currentRadioName = GET_PARAM_VALUE(param);
			}
		}

		if (!currentVapIndex)
			continue;
		if (!strcmp(vapIndex, currentVapIndex))
		{
			FAPI_WLAN_ASSERT(currentRadioName);
			if (!strcmp(currentRadioName, currentVapIndex))
			{
				return UGW_FAILURE;
			}
			else {
				ifName = currentIfName;
				radioName = currentRadioName;
			}
		}
	}

	FAPI_WLAN_ASSERT(ifName);

	wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	fapi_wlan_ssid_delete(ifName, wlObjRPC, 0);

	snprintf(command, sizeof(command), "rm -rf %s%s", DB_PATH, ifName);
	system(command); //TODO REPLACE BY CLEAN C FUNCTION

	fapi_wlan_up(radioName, wlObjRPC, 1); //Up the relevant radio

	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

	fapiWlanFailSafeStore();
	return UGW_SUCCESS;
}


static int wlan_security_set(int index, char *securityMode)
{
	ObjList *wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char    *modeEnabledValue = NULL;
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if (wlObjRPC == NULL)
	{
		return UGW_FAILURE;
	}

	modeEnabledValue = paramValueGet(wlObjRPC, index, DEVICE_ACCESS_POINT_SECURITY, "ModeEnabled");

	if (modeEnabledValue == NULL)
	{
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	strcpy(modeEnabledValue, securityMode);

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	if (fapi_wlan_security_set(interfaceName, wlObjRPC, 0) != 0)
	{
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	// update the data-base
	if (wlanStoreToDB(interfaceName, wlObjRPC) != UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}


static int modeEnabledValueGet(char *ifName, char *encryptionMode, char *authenticationMode, char **modeEnabledValue)
{
	ObjList *wlObjSecurity = NULL;

	if ((encryptionMode == NULL) || (authenticationMode == NULL))
	{
		return UGW_FAILURE;
	}

	if ((encryptionMode[0] == '\0') && (authenticationMode[0] == '\0'))
	{
		return UGW_FAILURE;
	}

	wlObjSecurity = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlObjSecurity == NULL)
	{
		return UGW_FAILURE;
	}

	if (wlanLoadFromDB(DEVICE_SECURITY_STATE, ifName, wlObjSecurity) == UGW_FAILURE)
	{  // file not exists. Create it.
		help_addObjList(wlObjSecurity, DEVICE_SECURITY_STATE, 0, 0, 0, 0);
	}

	if (encryptionMode[0] == '\0')
	{  // encryptionMode did NOT receive ==> try getting it from the data-base
		HELP_EDIT_NODE(wlObjSecurity, DEVICE_SECURITY_STATE, "AuthenticationMode", authenticationMode, 0, 0);
		encryptionMode = paramValueFromObjBufGet(wlObjSecurity, DEVICE_SECURITY_STATE, "EncryptionMode");
	}
	else if (authenticationMode[0] == '\0')
	{  // authenticationMode did NOT receive ==> try getting it from the data-base
		HELP_EDIT_NODE(wlObjSecurity, DEVICE_SECURITY_STATE, "EncryptionMode", encryptionMode, 0, 0);
		authenticationMode = paramValueFromObjBufGet(wlObjSecurity, DEVICE_SECURITY_STATE, "AuthenticationMode");
	}

	// write the data into the data-base
	wlanStoreToDB(ifName, wlObjSecurity);

	HELP_DELETE_OBJ(wlObjSecurity, SOPT_OBJVALUE, FREE_OBJLIST);

	if ((encryptionMode == NULL) || (authenticationMode == NULL))
	{  // the missing field was NOT found in the data-base
		return UGW_SUCCESS;
	}

	if (!strcmp(encryptionMode, "ENC_TKIP"))
	{
		if (!strcmp(authenticationMode, "AUTH_PSK"))
		{
			strcpy(*modeEnabledValue, "WPA-Personal");
		}
		else if (!strcmp(authenticationMode, "AUTH_EAP"))
		{
			strcpy(*modeEnabledValue, "WPA-Enterprise");
		}
	}
	else if (!strcmp(encryptionMode, "ENC_AES"))
	{
		if (!strcmp(authenticationMode, "AUTH_PSK"))
		{
			strcpy(*modeEnabledValue, "WPA2-Personal");
		}
		else if (!strcmp(authenticationMode, "AUTH_EAP"))
		{
			strcpy(*modeEnabledValue, "WPA2-Enterprise");
		}
	}
	else if (!strcmp(encryptionMode, "ENC_TKIP_AND_AES"))
	{
		if (!strcmp(authenticationMode, "AUTH_PSK"))
		{
			strcpy(*modeEnabledValue, "WPA-WPA2-Personal");
		}
		else if (!strcmp(authenticationMode, "AUTH_EAP"))
		{
			strcpy(*modeEnabledValue, "WPA-WPA2-Enterprise");
		}
	}

#if 0  /* Enable this part if you intend to reset the "history" of security settings when sending the security params */
	securityStateClear(ifName);
#endif	

	return UGW_SUCCESS;
}


static int securityModeGet(int index, char *securityMode, char *fieldName, char *securityModeName)
{
	int ret = UGW_FAILURE;
	ObjList *wlObjSecurity = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char *value = paramValueGet(wlObjSecurity, index, DEVICE_SECURITY_STATE, fieldName);
	if (value) {
		strncpy(securityMode, value, MAX_LEN_PARAM_VALUE);
		securityMode[MAX_LEN_PARAM_VALUE-1] = '\0';
	printf("%s; %s= '%s'\n", __FUNCTION__, securityModeName, securityMode);
		ret = UGW_SUCCESS;
	} else {
		printf("%s; ERROR: Parameter (%s) for index(%d) not found!\n", __FUNCTION__, fieldName, index);
	}
	HELP_DELETE_OBJ(wlObjSecurity, SOPT_OBJVALUE, FREE_OBJLIST);

	return ret;
}


static void securityModeSet(ObjList *wlObjSecurity, char *interfaceName, char *securityMode, char *fieldName)
{
	if (wlanLoadFromDB(DEVICE_SECURITY_STATE, interfaceName, wlObjSecurity) == UGW_FAILURE)
	{  // file not exists. Create it.
		help_addObjList(wlObjSecurity, DEVICE_SECURITY_STATE, 0, 0, 0, 0);
	}

	HELP_EDIT_NODE(wlObjSecurity, DEVICE_SECURITY_STATE, fieldName, securityMode, 0, 0);

	// write the data into the data-base
	wlanStoreToDB(interfaceName, wlObjSecurity);
}

static int stringValueSet(FapiWlanSetFunc fapi_wlan_set_func, int index, char *varString, char *pcName, char *fieldName)
{
	ObjList *dbObjPtr = NULL;
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	dbObjPtr = dbObjPtrSet(dbObjPtr, pcName, fieldName, varString);

	if (dbObjPtr == NULL)
	{
		return UGW_FAILURE;
	}

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	// Check if we actually want to run FAPI function
	if (fapi_wlan_set_func != NULL)
	{
		if (fapi_wlan_set_func(interfaceName, dbObjPtr, 0) != 0)
		{
			HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_FAILURE;
		}
	}
	// rewrite the db file with the returned values of the object
	if (wlanStoreToDB(interfaceName, dbObjPtr) != UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}

static int stringValueGet(const char *functionName, int index, char *varString, char *objName, char *fieldName)
{
	ObjList *wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	char *paramValue = paramValueGet(wlObjRPC, index, objName, fieldName);

	if (paramValue != NULL)
	{
		strcpy(varString, paramValue);
		printf("%s: %s= '%s'\n", functionName, fieldName, paramValue);
	}
	else
	{
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
	return UGW_SUCCESS;
}

static int getIndexFromBssid(char *bssid, int *index)
{
	ObjList	*tempLoopObj = NULL;
	ParamList *param = NULL;
	char vapIndex[MAX_LEN_PARAM_VALUE];
	char bssidMapper[MAX_LEN_PARAM_VALUE];
	int indexMapper =-1;
	
	FAPI_WLAN_ASSERT(index);
	*index =-1;
	
	if (fapiWlanFailSafeLoad() != UGW_SUCCESS)
	{
		return UGW_FAILURE;
	}

	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RPC_INDEX, GET_PARAM_NAME(param)))
			{
				strncpy(vapIndex,GET_PARAM_VALUE(param),sizeof(vapIndex));
				indexMapper=(int)strtol((const char*)&(vapIndex[4]), NULL, 10);
				if (stringValueGet(__FUNCTION__,indexMapper,bssidMapper,DEVICE_SSID,"BSSID") == UGW_SUCCESS)
				{
					if (!strncmp(bssid, bssidMapper, 17)) // Macaddress length in database
					{
						*index=indexMapper;
						break;
					}
				} else {
					return UGW_FAILURE;
				}
			}
		}
	}
	
	return UGW_SUCCESS;
}

// For the case that dB is in Radio but FAPI is VAP.
// WPS is an example:
//   dB is at radio object (except enable/disable).
// Action: Call script with index, set dB with radioIndex
static int boolValueSetDbRadio(FapiWlanSetFunc fapi_wlan_set_func, int index, bool boolValue, char *pcName, char *fieldName)
{
	ObjList *dbObjPtr = NULL;
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    boolString[] = "false";
	int		radioIndex=-1;

	// Convert data:
	if (boolValue)
	{
		strcpy(boolString, "true");
	}
	
	wlan_getRadioIndex(index, &radioIndex);

	dbObjPtr = dbObjPtrSet(dbObjPtr, pcName, fieldName, boolString);

	if (dbObjPtr == NULL)
	{
		return UGW_FAILURE;
	}

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	// Check if we actually want to run FAPI function
	if (fapi_wlan_set_func != NULL)
	{
		if (fapi_wlan_set_func(interfaceName, dbObjPtr, 0) != 0)
		{
			HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_FAILURE;
		}
	}
	
	// rewrite the db file with the returned values of the object
	if (wlanStoreToDB(interfaceName, dbObjPtr) != UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}

// For the case that dB is in Radio but FAPI is VAP.
// WPS is an example:
//   dB is at radio object (except enable/disable).
// Action: Call script with index, set dB with radioIndex
static int intValueSetDbRadio(FapiWlanSetFunc fapi_wlan_set_func, int index, int varInt, char *pcName, char *fieldName)
{
	ObjList *dbObjPtr = NULL;
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    radioName[MAX_LEN_PARAM_VALUE] = { '\0' };
	char 	varString[MAX_LEN_PARAM_VALUE];
	int		radioIndex=-1;

	// Convert data:
	sprintf(varString, "%d", varInt);
	
	dbObjPtr = dbObjPtrSet(dbObjPtr, pcName, fieldName, varString);

	if (dbObjPtr == NULL)
	{
		return UGW_FAILURE;
	}
	wlan_getRadioIndex(index, &radioIndex);

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}
	// Check if we actually want to run FAPI function
	if (fapi_wlan_set_func != NULL)
	{
		if (fapi_wlan_set_func(interfaceName, dbObjPtr, 0) != 0)
		{
			HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_FAILURE;
		}
	}
	sprintf(radioName, "%s%d", "wlan", radioIndex);
	// rewrite the db file with the returned values of the object
	if (wlanStoreToDB(radioName, dbObjPtr) != UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}

// For the case that dB is in Radio but FAPI is VAP.
// WPS is an example:
//   dB is at radio object (except enable/disable).
// Action: Call script with index, set dB with radioIndex
static int stringValueSetDbRadio(FapiWlanSetFunc fapi_wlan_set_func, int index, char *varString, char *pcName, char *fieldName)
{
	ObjList *dbObjPtr = NULL;
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    radioName[MAX_LEN_PARAM_VALUE] = { '\0' };
	int		radioIndex=-1;

	dbObjPtr = dbObjPtrSet(dbObjPtr, pcName, fieldName, varString);

	if (dbObjPtr == NULL)
	{
		return UGW_FAILURE;
	}

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}
	wlan_getRadioIndex(index, &radioIndex);

	// Check if we actually want to run FAPI function
	if (fapi_wlan_set_func != NULL)
	{
		if (fapi_wlan_set_func(interfaceName, dbObjPtr, 0) != 0)
		{
			HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_FAILURE;
		}
	}
	sprintf(radioName, "%s%d", "wlan", radioIndex);
	// rewrite the db file with the returned values of the object
	if (wlanStoreToDB(radioName, dbObjPtr) != UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}

static int boolValueSet(FapiWlanSetFunc fapi_wlan_set_func, int index, bool boolValue, char *pcName, char *fieldName)
{
	char    boolString[] = "false";

	if (boolValue)
	{
		strcpy(boolString, "true");
	}

	return stringValueSet(fapi_wlan_set_func,
		index,
		boolString,
		pcName,
		fieldName);
}

static int boolValueGet(const char *functionName, int index, bool *boolValue, char *objName, char *fieldName)
{
	ObjList *wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	int intBoolVal;
	char *boolString = paramValueGet(wlObjRPC, index, objName, fieldName);

	if (boolString != NULL)
	{
		intBoolVal = (!strcmp(boolString, "true")) ? (int)true : (!strcmp(boolString, "false")) ? (int)false : (-1);
	}
	else {
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	if (intBoolVal == (-1))
	{
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	*boolValue = (bool)intBoolVal;
	printf("%s; %s= %d\n", functionName, fieldName, *boolValue);
	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
	return UGW_SUCCESS;
}

static int intValueSet(FapiWlanSetFunc fapi_wlan_set_func, int index, int varInt, char *pcName, char *fieldName)
{
	char str[MAX_LEN_PARAM_VALUE];
	sprintf(str, "%d", varInt);
	return stringValueSet(fapi_wlan_set_func, index, (char*)str, pcName, fieldName);
}

static int intValueGet(const char *functionName, int index, int *varNum, char *objName, char *fieldName)
{
	ObjList *wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char *numString = paramValueGet(wlObjRPC, index, objName, fieldName);

	if (numString == NULL)
	{
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	*varNum = strtol(numString, NULL, 10);
	printf("%s: %s= '%d'\n", functionName, fieldName, *varNum);

	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
	return UGW_SUCCESS;
}

static int wlan_applyObjectForAllRadios(FapiWlanSetFunc fapi_wlan_set_func, ObjList *dbObjPtr)
{
	ObjList	*tempLoopObj = NULL;
	ParamList *param = NULL;
	char *radioName = NULL;
	char *ifName = NULL;

	if (mapperObjPtr == NULL )
	{
		fapiWlanFailSafeLoad();
	}

	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		radioName = NULL;
		ifName = NULL;
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
			{
				radioName = GET_PARAM_VALUE(param);
			}

			if (!strcmp(DB_IFNAME, GET_PARAM_NAME(param)))
			{
				ifName = GET_PARAM_VALUE(param);
			}
		}

		if (!radioName || !ifName)
			continue;
		if (!strcmp(radioName, ifName))
		{
			if (fapi_wlan_set_func != NULL)
			{
				if (fapi_wlan_set_func(ifName, dbObjPtr, 1) != UGW_SUCCESS)
				{
					return UGW_FAILURE;
				}
			}
			else
			{
				return UGW_FAILURE;
			}
		}
	}

	return UGW_SUCCESS;
}

// ### HELPER FUNCTIONS END ###

/**************************************************************************/
/*! \fn int wlan_load(void)
 **************************************************************************
 *  \brief pre initialize the WLAN AP.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_load()
{
	ObjList	*dbObjPtr = NULL;
	dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	if (system("lsmod | grep mtlk") == UGW_SUCCESS)
	{
		logger("WAVE WLAN RPC INIT: driver already insmoded using apply\n");
		if (wlan_applyObjectForAllRadios(fapi_wlan_up, dbObjPtr) == UGW_SUCCESS)
		{
			HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_SUCCESS;
		}

		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	} 	else {
		logger("WAVE WLAN RPC LOAD: driver not found in lsmod, statring WLAN INIT\n");
		return wlan_init();
	}
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_uninit(void)
**************************************************************************
*  \brief un-initialize the WLAN AP
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_uninit()
{
	ObjList		*wlObjRPC = NULL;

	wlan_stopHostapd();
	wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	/* call fapi_wlan_hw_uninit() to init hardware */
	if (fapi_wlan_hw_uninit(wlObjRPC, 0) != UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
	HELP_DELETE_OBJ(mapperObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_init(void)
 **************************************************************************
 *  \brief initialize the WLAN AP and start forwarding data.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_init()
{
	DIR       *dir;
	ObjList	  *tempLoopObj = NULL;
	ParamList *param = NULL;
	char      *radioName = NULL;
	char      *ifName = NULL;
	char      *vapIndex = NULL;
	char      numOfRadioString[MAX_LEN_PARAM_VALUE] = { '\0' };

	ObjList   *wlObjRPC = NULL;
	ObjList   *tmpHwInitObj = NULL;
	ObjList	  *savedVapsObj = NULL;
	bool firstINIT = true;

	unsigned int i;
	unsigned long numOfRadios, numOfHwRadios;
	int sysRetVal;

	char command[MAX_LEN_VALID_VALUE] = { '\0' };
	char fapiSeqDebug[MAX_LEN_VALID_VALUE] = { '\0' };
	char path[MAX_LEN_VALID_VALUE];

	sprintf(command, "touch %s", FAPI_RPC_MODE);
	system(command); //TODO REPLACE BY CLEAN C FUNCTION
	
	sprintf(command, "mkdir -p %s", DB_TEMP_WAVE_PATH);
	system(command); //TODO REPLACE BY CLEAN C FUNCTION
	
	sprintf(fapiSeqDebug, "echo wlan_init called $(date) >> %s/fapi_wlan_wave.log", DB_TEMP_WAVE_PATH);
	system(fapiSeqDebug);
	
	sysRetVal = system("true");
	logger("Check shell true status sysRetVal= %d\n", sysRetVal);
	
	sysRetVal = system("lsmod | grep mtlk");
	logger("lsmod | grep mtlk sysRetVal= %d\n", sysRetVal);

#if defined YOCTO_PUMA6
	if (sysRetVal == UGW_SUCCESS)
	{
		ObjList	*dbObjPtr = NULL;
		dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
		logger("WAVE WLAN RPC INIT: driver already insmoded \n");
		if (wlan_applyObjectForAllRadios(fapi_wlan_up, dbObjPtr) == UGW_SUCCESS)
		{
			HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_SUCCESS;
		}
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	} else {
		logger("WAVE WLAN RPC INIT: driver not found in lsmod, statring INIT \n");
	}
	
	if (system("ifconfig br-lan") != UGW_SUCCESS)
	{
	/* set up the bridge */
		system("vconfig add eth0 2");
		system("ifconfig eth0.2 up");
		system("brctl addbr br-lan");
		system("brctl addif br-lan eth0.2");
		system("ifconfig br-lan up");
		system("ifconfig br-lan 192.168.1.20");
	}
#endif
			
	dir = opendir("/nvram/wave_scripts/");
	if ( dir != NULL )
	{  /* Directory exists, possible to check if mapper exists instead */
		closedir(dir);
		system("mount -t tmpfs /tmp/scripts/ /etc/wave/scripts/");
		system("cp /nvram/wave_scripts/* /etc/wave/scripts");
	}

	/* Copy (if needed) 'Device.WiFi' (without 'Device.WiFi.NeighboringWiFiDiagnostic') to '/db/instances' */
	dir = NULL;	
	dir = opendir(DB_PATH);
	if ( dir != NULL )
	{  /* Directory exists, possible to check if mapper exists instead */
		closedir(dir);
		sprintf(command, "mv %s%s %s%s", DB_PATH, WLAN_OBJECT_MAPPER, DB_PATH, LAST_SAVED_VAPS_DB);
		system(command); //TODO REPLACE BY CLEAN C FUNCTION	
		firstINIT = false;
		logger("WLAN INIT: SAVED MAPPER CONFIGURATION FOR VAP UPLOADING\n");
	}

	//HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
	fapiWlanFailSafeLoad();

	/* get number of radios from default DB */
	tmpHwInitObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	if ( firstINIT == true )
	{
		sprintf(path, "%s/%s", DB_PATH_DEFAULT, DEVICE_WIFI_NAME);
	} else {
		sprintf(path, "%s/%s", DB_PATH, DEVICE_WIFI_NAME);
	}
	
	FAPI_WLAN_ASSERT(!wlanLoadFromDB(path, "", tmpHwInitObj));

	strcpy(numOfRadioString, paramValueFromObjBufGet(tmpHwInitObj, "Device.WiFi", "RadioNumberOfEntries"));
	numOfRadios = atoi(numOfRadioString);

	if (numOfRadios < 1)
	{
		printf("%s; non-valid number of radios (%d) ==> ABORT!\n", __FUNCTION__, (int)numOfRadios);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(tmpHwInitObj, SOPT_OBJVALUE, EMPTY_OBJLIST);
	wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(wlObjRPC);

	for (i = 0; i < numOfRadios; i++)
	{
		int j=i*2;
		if ( firstINIT == true )
		{
			sprintf(path, "%sradio%d/%s", DB_PATH_DEFAULT, j, DEVICE_RADIO_VENDOR);
		} else {
			sprintf(path, "%swlan%d/%s", DB_PATH, j, DEVICE_RADIO_VENDOR);
		}
		FAPI_WLAN_ASSERT(!wlanLoadFromDB(path, "", tmpHwInitObj));
		HELP_COPY_OBJ(wlObjRPC, tmpHwInitObj, SOPT_OBJVALUE, COPY_COMPLETE_OBJ);
	}

	fapi_wlan_hw_init(wlObjRPC, 0);

	strncpy(numOfRadioString, paramValueFromObjBufGet(wlObjRPC, "Device.WiFi", "RadioNumberOfEntries"), MAX_LEN_PARAM_VALUE-1);

	HELP_DELETE_OBJ(tmpHwInitObj, SOPT_OBJVALUE, FREE_OBJLIST);

	numOfHwRadios = atoi(numOfRadioString);

	/* if numOfRadios in DB is smaller then actual radio HW I/F then abort */
	if (numOfRadios < numOfHwRadios)
	{
		sprintf(fapiSeqDebug, "echo $(date) numOfRadios in nvram DB=%d < numOfHwRadios =%d ==> ABORT!\n >> %s/fapi_wlan_wave.log", (int)numOfRadios, (int)numOfHwRadios, DB_TEMP_WAVE_PATH);
		system(fapiSeqDebug);
		return UGW_FAILURE;
	}

	/*
	 Check if we are in the first init (SOD).
	 if we don't have nvram DB then SOD,
	 if we have then check:
	 number HW radios == number of nvram radios?
	 if not (HW radios were changed ) need to re-init.
	*/
	if ( firstINIT == false )
	{
		logger("FAPI_WLAN_RPC INIT: DB exsists, continue\n");
		if (numOfRadios != numOfHwRadios)
		{
			logger("FAPI_WLAN_RPC INIT: num of radios changed, refacrtoring\n");
			sprintf(command, "%s%s", "rm -rf ", DB_PATH);
			system(command); //TODO REPLACE BY CLEAN C FUNCTION
			system("rm -rf /tmp/wlan_wave/"); //TODO REPLACE BY CLEAN C FUNCTION
			sprintf(command, "mkdir -p %s", DB_PATH);
			system(command); //TODO REPLACE BY CLEAN C FUNCTION
			sprintf(command, "cp %s%s %s.", DB_PATH_DEFAULT, DEVICE_WIFI_NAME, DB_PATH);
			system(command); //TODO REPLACE BY CLEAN C FUNCTION
			sprintf(command, "cp %s%s %s.", DB_PATH_DEFAULT, DEVICE_INFO, DB_PATH);
			system(command); //TODO REPLACE BY CLEAN C FUNCTION
		}
		else { /* all is OK , init was done before, continue with existing nvram DB*/
			logger("FAPI_WLAN_RPC INIT: num of radios is the same continue\n");
		}
	} else {
		logger("FAPI_WLAN_RPC INIT: database not found, creating\n");
		sprintf(command, "mkdir -p %s", DB_PATH);
		system(command); //TODO REPLACE BY CLEAN C FUNCTION
		sprintf(command, "cp %s%s %s.", DB_PATH_DEFAULT, DEVICE_WIFI_NAME, DB_PATH);
		system(command); //TODO REPLACE BY CLEAN C FUNCTION
		sprintf(command, "cp %s%s %s.", DB_PATH_DEFAULT, DEVICE_INFO, DB_PATH);
		system(command); //TODO REPLACE BY CLEAN C FUNCTION
	}

	/* we first need to create the nvram DB*/
	for (i = 0; i < numOfHwRadios; i++)
	{
		int j=i*2;
		sprintf(path, "%s%s%d", DB_PATH, "wlan", j);
		dir = NULL;
		dir = opendir(path);
		if ( dir == NULL )
		{
			/* Directory does NOT exist ==> copy to '/db/instance' ONLY the active radios (wlan0/1) */
			snprintf(command, sizeof(command),"mkdir -p %s", path);
			system(command); //TODO REPLACE BY CLEAN C FUNCTION

			snprintf(command, sizeof(command), "cp -r %s%s%d/* %s", DB_PATH_DEFAULT, "radio", j, path);
			system(command); //TODO REPLACE BY CLEAN C FUNCTION
		}
	}

	/* update the nvram DB according to the HW init updated obj*/
	tmpHwInitObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (!tmpHwInitObj){
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		FAPI_WLAN_ASSERT(tmpHwInitObj);
	}
	i = 0;
	FOR_EACH_OBJ(wlObjRPC, tempLoopObj)
	{
		if (!strcmp(GET_OBJ_NAME(tempLoopObj), DEVICE_RADIO_VENDOR))
		{
			sprintf(path, "%swlan%d/%s", DB_PATH, i, DEVICE_RADIO_VENDOR);
			HELP_COPY_OBJ(tmpHwInitObj, tempLoopObj, SOPT_OBJVALUE, COPY_SINGLE_OBJ);
			help_storeLocalDB(tmpHwInitObj, path);
			HELP_DELETE_OBJ(tmpHwInitObj, SOPT_OBJVALUE, EMPTY_OBJLIST);
			i+=2;
		}
	}
	HELP_DELETE_OBJ(tmpHwInitObj, SOPT_OBJVALUE, FREE_OBJLIST);
	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

	/* update "Device.WiFi" in nvram DB_PATH with the updated (from the HW) numOfHwRadios */
	tmpHwInitObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(tmpHwInitObj);
	sprintf(path, "%s%s", DB_PATH, DEVICE_WIFI_NAME);
	FAPI_WLAN_ASSERT(!wlanLoadFromDB(path, "", tmpHwInitObj));

	HELP_EDIT_NODE(tmpHwInitObj, DEVICE_WIFI_NAME, RADIO_NUM_OF_ENTRIES, numOfRadioString, 0, 0);
	help_storeLocalDB(tmpHwInitObj, path);
	HELP_DELETE_OBJ(tmpHwInitObj, SOPT_OBJVALUE, FREE_OBJLIST);

	i = 0;
	/* Set the Radio Settings */
	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		radioName = NULL;
		ifName = NULL;
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
			{
				radioName = GET_PARAM_VALUE(param);
			}

			if (!strcmp(DB_IFNAME, GET_PARAM_NAME(param)))
			{
				ifName = GET_PARAM_VALUE(param);
			}
		}

		if ( !strcmp(radioName, ifName) && (i < numOfHwRadios) )
		{
			i++; // Add protection from initiating more than the number of radios
			logger("RPC WLAN INIT: numOfHwRadios='%d' set radio num='%d'\n", numOfHwRadios, i);

			wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE); // head initialization done here
			FAPI_WLAN_ASSERT(!wlanLoadFromDB(DEVICE_RADIO, radioName, wlObjRPC));
			FAPI_WLAN_ASSERT(!wlanLoadFromDB(DEVICE_RADIO_VENDOR, radioName, wlObjRPC));

			fapi_wlan_radio_set(radioName, wlObjRPC, 0);
			FAPI_WLAN_ASSERT(wlanStoreToDB(radioName, wlObjRPC) == UGW_SUCCESS);
			HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

			setVapHelper(initVap, radioName, radioName, radioName, NULL, 0);	
		}
	}

	/* Set the VAP's Settings */
	sprintf(command, "%s%s", DB_PATH, LAST_SAVED_VAPS_DB);
	if (file_exists(command))
	{
		logger("FAPI_WLAN_RPC INIT: FOUND LAST_SAVED_VAPS_DB\n");
		savedVapsObj = HELP_CREATE_OBJ(SOPT_OBJVALUE); // head initialization done here
		FAPI_WLAN_ASSERT(!wlanLoadFromDB(LAST_SAVED_VAPS_DB, "", savedVapsObj));
		sprintf(command, "rm -f %s%s", DB_PATH, LAST_SAVED_VAPS_DB);
		system(command); //TODO REPLACE BY CLEAN C FUNCTION	
		FOR_EACH_OBJ(savedVapsObj, tempLoopObj)
		{
			radioName = NULL;
			ifName = NULL;
			vapIndex = NULL;
			FOR_EACH_PARAM(tempLoopObj, param)
			{
				if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
				{
					radioName = GET_PARAM_VALUE(param);
				}

				if (!strcmp(DB_IFNAME, GET_PARAM_NAME(param)))
				{
					ifName = GET_PARAM_VALUE(param);
				}

				if (!strcmp(DB_RPC_INDEX, GET_PARAM_NAME(param)))
				{
					vapIndex = GET_PARAM_VALUE(param);
				}
			}

			//Object is VAP
			if (!radioName || !ifName)
				continue;
			if (strcmp(radioName, ifName))
			{
				setVapHelper(initVap, radioName, ifName, vapIndex, NULL, 0);
			}
		}

		HELP_DELETE_OBJ(savedVapsObj, SOPT_OBJVALUE, FREE_OBJLIST);
	}

	/* Up the relevant radios */
	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		radioName = NULL;
		ifName = NULL;
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
			{
				radioName = GET_PARAM_VALUE(param);
			}

			if (!strcmp(DB_IFNAME, GET_PARAM_NAME(param)))
			{
				ifName = GET_PARAM_VALUE(param);
			}
		}

		if (!radioName || !ifName)
			continue;
		if (!strcmp(radioName, ifName))
		{  /* object is radio ==> UP */
			wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
			FAPI_WLAN_ASSERT(wlObjRPC);
			fapi_wlan_up(radioName, wlObjRPC, 1);
			HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

			wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
			FAPI_WLAN_ASSERT(wlObjRPC);
			fapi_wlan_capability_query(radioName, wlObjRPC, 0);
			FAPI_WLAN_ASSERT(wlanStoreToDB(radioName, wlObjRPC) == UGW_SUCCESS);
			HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		}
	}

	fapiWlanFailSafeStore();

	return UGW_SUCCESS;
}


int fapi_wlan_notify(char *ifDriverName, ObjList *wlObj, char *dbCliFileName)
{
	if (wlanLoadFromDB(dbCliFileName, "", wlObj) == UGW_SUCCESS)
	{
		if (wlanStoreToDB(ifDriverName, wlObj) == UGW_SUCCESS)
		{
			return UGW_SUCCESS;
		}
	}

	return UGW_FAILURE;
}

/**************************************************************************/
/*! \fn int wlan_reset(void)
 **************************************************************************
 *  \brief  Put the AP in reset, restarts the driver with the same configuration
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_reset()
{
	wlan_uninit();
	return wlan_init();
}


int wlan_getNumberOfEntries(int *numEntries)
{
	(void)numEntries;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_addEntry(int *entries)
 **************************************************************************
 *  \brief return the number of WLAN entries.
 *  \param[out] int *entries - new number of WLAN logical or physical networks.
 *  The newly added entry holds the last index and it is be default disabled
 *  and populated with defaults.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_addEntry(int *entries)
{
	(void)entries;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_delEntry(int index, int *entries)
 **************************************************************************
 *  \brief Delete WLAN entry.
 *  \param[in] int index - The index of the entry to be deleted
 *  \param[out] int *entries - new number of WLAN logical or physical networks.
 *  Since the entries are held constitutively, deleting one index causes all
 *  following ones to collapse one index down.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_delEntry(int index, int *entries)
{
	(void)index;    // unused parameter
	(void)entries;  // unused parameter
	return UGW_SUCCESS;
}



/**************************************************************************/
/*! \fn int wlan_getBssIdPort(char *bssid, int *port, int *index)
 **************************************************************************
 *  \brief Get the port which is associated with a BSSID.
 *  \param[in] char *bssid - MAC address of the BSS interface
 *  \param[out] int *port - L2 switch port on which packets from the BSSID are sent:
 *  SW_PORT_APP, SW_PORT_WLAN1, SW_PORT_WLAN2
 *  \param[out] int *index - The WLAN index of this BSSID
 *  Notes: BSS to port mapping is maintained by the APP
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getBssIdPort(char *bssid, int *port, int *index)
{
	int retVal = UGW_SUCCESS;
	int radioIndex = -1;
	*port= -1;
	*index= -1;
	
	retVal += getIndexFromBssid(bssid, index);
	retVal += wlan_getRadioIndex(*index, &radioIndex);
	
	retVal += intValueGet(__FUNCTION__,
		radioIndex,
		port,
		DEVICE_RADIO_VENDOR,
		"UdmaL2SWPort");
	
	return retVal;
}


/**************************************************************************/
/*! \fn int wlan_associateVlan(char *bssid, unsigned short vlan)
 **************************************************************************
 *  \brief Associate a VLAN ID for a given BSSID
 *  \param[in] char *bssid - MAC address of the BSS interface
 *  \param[in] unsigned short vlan - VLAN id for the BSS interface
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_associateVlan(char *bssid, unsigned short vlan)
{
	int index=-1;
	if ( getIndexFromBssid(bssid, &index) == UGW_SUCCESS )
	{
		return wlan_setVlanId(index, (int)vlan);
	}
	
	return UGW_FAILURE;
}


int wlan_getBaseBSSID(int index, char *bssid)
{
	return stringValueGet(__FUNCTION__,
		index,
		bssid,
		DEVICE_SSID,
		"BSSID");
}


int wlan_getApSecurityModeEnabled(int index, char *modeEnabled)
{
	//TODO, currently returns TR181,in the future will be required TR98
	return stringValueGet(__FUNCTION__,
		index,
		modeEnabled,
		DEVICE_ACCESS_POINT_SECURITY,
		"ModeEnabled");
}

int wlan_getNumberOfRadios(int *radios)
{
	return intValueGet(__FUNCTION__,
		-1,
		radios,
		DEVICE_WIFI_NAME,
		"RadioNumberOfEntries");
}

int wlan_setAccelerationMode(int index, bool mode)
{
	return boolValueSet(fapi_wlan_radio_set,
		index,
		mode,
		DEVICE_RADIO_VENDOR,
		"WaveFastpathEnabled");
}

int wlan_getAccelerationMode(int index, bool *mode)
{
	return boolValueGet(__FUNCTION__,
		index,
		mode,
		DEVICE_RADIO_VENDOR,
		"WaveFastpathEnabled");
}

/**************************************************************************/
/*! \fn int wlan_setRouterEnable(int index, bool routerEnabled)
**************************************************************************
*  \brief set Router Enable
*  \param[in] int index - AP Index
*  \param[in] bool routerEnabled - true/false
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setRouterEnable(int index, bool routerEnabled)
{
	char command[256];
	char interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	if (routerEnabled == true)
	{
		snprintf(command, sizeof(command), "touch /tmp/wlan_wave/%s_EnableRouter", interfaceName);
		system(command);
	}
	else
	{
		snprintf(command, sizeof(command), "rm -f /tmp/wlan_wave/%s_EnableRouter", interfaceName);
		system(command);
	}

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getRouterEnable(int index, bool * routerEnabled)
**************************************************************************
*  \brief get Router Enable
*  \param[in] int index - AP Index
*  \param[out] bool * routerEnabled - true/false
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getRouterEnable(int index, bool * routerEnabled)
{
	char path[256];
	char interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	snprintf(path, sizeof(path), "/tmp/wlan_wave/%s_EnableRouter", interfaceName);
	if (file_exists(path))
	{
		printf("Router Enabled - True\n");
		*routerEnabled = true;
	}
	else
	{
		printf("Router Enabled - False\n");
		*routerEnabled = false;
	}

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getEnable(int index, bool *enable)
 **************************************************************************
 *  \brief Enable/disable WLAN get.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool *enable - the value of enable.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getEnable(int index, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_SSID,
		"Enable");
}


int wlan_AutoChannelEnable(int index, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_RADIO,
		"AutoChannelEnable");
}


//TBD CLEAN DUPLICATE function - use function pointer for ifconfig Down\Up
/**************************************************************************/
/*! \fn int wlan_ifConfigDown(int index)
**************************************************************************
*  \brief set VAP ENABLE TO down state and restart hostapd
*  \param[in] int index - AP Index
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_ifconfigDown(int index)
{
	ObjList *dbObjPtr = NULL;
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (dbObjPtr == NULL)
	{
		return UGW_FAILURE;
	}

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	if (fapi_wlan_down(interfaceName, dbObjPtr, 0) != 0)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_ifconfigUp(int vap)
**************************************************************************
*  \brief up the virtual access point
*  \param[in] int vap - virtual access point
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_ifconfigUp(int vap)
{
	ObjList *dbObjPtr = NULL;
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	/* TODO - Delete After Verify that it works in scripts level
	bool EnableOnline = false;
	bool RouterEnabled = false;

	if (wlan_getEnableOnline(vap, &EnableOnline) == UGW_SUCCESS)
	{
		if (EnableOnline)
		{
			wlan_getRouterEnable(vap, &RouterEnabled);
			if (!RouterEnabled)
			{
				printf("__FUNCTION__:EnableOnline=True but RouterEnabled=False, interface Will Be Down\n");
				return wlan_ifconfigDown(vap);
			}
		}
	}
	else {
		printf("__FUNCTION__:VAP doesn't have EnableOnline parameter\n");
	}
	*/

	dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (dbObjPtr == NULL)
	{
		return UGW_FAILURE;
	}

	if (getInterfaceName(vap, interfaceName) == UGW_FAILURE)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	if (fapi_wlan_up(interfaceName, dbObjPtr, 1) != 0)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
	return UGW_SUCCESS;
}


int wlan_getWpaRekeyInterval(int vap, int *rekeyInterval)
{
	return intValueGet(__FUNCTION__,
		vap,
		rekeyInterval,
		DEVICE_ACCESS_POINT_SECURITY,
		"RekeyingInterval");
}


int wlan_setWpaRekeyInterval(int vap, int rekeyInterval)
{
	return intValueSet(fapi_wlan_security_set,
		vap,
		rekeyInterval,
		DEVICE_ACCESS_POINT_SECURITY,
		"RekeyingInterval");
}


/**************************************************************************/
/*! \fn int wlan_setEnable(int index, bool enable)
 **************************************************************************
 *  \brief Enable/disable VAP WLAN set.
 *  \param[in] int index - The index of the entry
 *  \param[in] bool enable - the value of enable.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setEnable(int index, bool enable)
{
	if (checkInterfaceIsRadio(index))
	{
		printf("__FUNCTION__ : doesn't supported on physical radios");
		return UGW_FAILURE;
	}
	int ret_val = 0;
	//TDO clean object so both values will be written to script layer
	// Currently the only value interesting scripts is in DEVICE_ACCESS_POINT
	ret_val += boolValueSet(NULL,
		index,
		enable,
		DEVICE_SSID,
		"Enable");

	ret_val += boolValueSet(fapi_wlan_ap_set,
		index,
		enable,
		DEVICE_ACCESS_POINT,
		"Enable");

	return ret_val;
}

/**************************************************************************/
/*! \fn int wlan_getRadioActive(int index, bool *active)
**************************************************************************
*  \brief get Radio is active or not.
*  \param[in] int index - Radio Index
*  \param[out] bool *active - True if radio is enabled
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getRadioActive(int index, bool *active)
{

	ObjList	  *tempLoopObj = NULL;
	ParamList *param = NULL;
	char      *radioName = NULL;
	char      *rpcName = NULL;
	char      inputRadio[MAX_LEN_PARAM_VALUE];
	char      ifIndex[MAX_LEN_PARAM_VALUE];

	fapiWlanFailSafeLoad();
	sprintf(inputRadio, "%s%d", "wlan", index);

	*active = false;

	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		radioName = NULL;
		rpcName = NULL;
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
			{
				radioName = GET_PARAM_VALUE(param);
			}

			if (!strcmp(DB_RPC_INDEX, GET_PARAM_NAME(param)))
			{
				rpcName = GET_PARAM_VALUE(param);
			}
		}

		if (!radioName)
			continue;
		if (!strcmp(inputRadio, radioName))
		{
			if (!rpcName)
				continue;
			strncpy(ifIndex, rpcName, MAX_LEN_PARAM_VALUE);
			strRemoveAll(ifIndex, "wlan");
			boolValueGet(__FUNCTION__,
				atoi(ifIndex),
				active,
				DEVICE_SSID,
				"Enable");

			if (*active) {
				return UGW_SUCCESS;
			}
		}
	}

	return UGW_SUCCESS;

}
/**************************************************************************/
/*! \fn int wlan_getStatus(int index, char *status)
 **************************************************************************
 *  \brief get WLAN VAP status.
 *  \param[in] int vap - vap index
 *  \param[out] char *status - the value of the status, "Up", "Error" or "Disabled".
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getStatus(int vap, char *status)
{
	return stringValueGet(__FUNCTION__,
		vap,
		status,
		DEVICE_SSID,
		"Status");
}

/**************************************************************************/
/*! \fn int wlan_getWifiEnableStatus(int index, char *status)
**************************************************************************
*  \brief get WLAN Radio status.
*  \param[in] int index - The index of the entry
*  \param[out] char *status - the value of the status, "Up", "Error" or "Disabled".
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getWifiEnableStatus(int index, char *status)
{
	return stringValueGet(__FUNCTION__,
		index,
		status,
		DEVICE_RADIO,
		"Status");
}


int wlan_getWifiEnableStatusBool(int index, bool *status)
{
	char statusArr[MAX_LEN_PARAM_NAME];
	char *arrPtr = &statusArr[0];
	if (wlan_getWifiEnableStatus(index, arrPtr) == UGW_SUCCESS)
	{
		if (!strcmp(arrPtr, "Up")) {
			*status = true;
		}
		else {
			*status = false;
		}
		return UGW_SUCCESS;
	}
	else {
		return UGW_FAILURE;
	}
}


/**************************************************************************/
/*! \fn int wlan_getName(int index, char *name)
 **************************************************************************
 *  \brief get WLAN Network name.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *name - a 16 byte or less name of the interface associated
 *  with the network (for instance wlan0).
 *  String buffer is pre-allocated by the caller.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getName(int index, char *name)
{
	(void)index;  // unused parameter
	(void)name;   // unused parameter
	return UGW_SUCCESS;
}


int wlan_getBSSID(int index, char *bssid)
{
	return stringValueGet(__FUNCTION__,
		index,
		bssid,
		DEVICE_SSID,
		"BSSID");
}


/**************************************************************************/
/*! \fn int wlan_getSupportedFrequencyBands(int radioIndex,
*                                                    char *frequencyBands)
**************************************************************************
*  \brief get the supported frequency band
*  \param[in] int radioIndex - Radio Index Number
*  \param[out] char *frequencyBands - frequency Bands
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getSupportedFrequencyBands(int radioIndex, char *frequencyBands)
{
	return stringValueGet(__FUNCTION__,
		radioIndex,
		frequencyBands,
		DEVICE_RADIO,
		"SupportedFrequencyBands");
}


/**************************************************************************/
/*! \fn int wlan_getMaxBitRate(int index, char *maxBitRate)
 **************************************************************************
 *  \brief get maximum upstream and downstream bit rate.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *maxBitRate - The maximum upstream and downstream bit rate available
 *  to this connection in Mbps. Either Auto, or the largest of
 *  the OperationalDataTransmitRates values.
 *  Buffer is pre-allocated by the caller and is four bytes long.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getMaxBitRate(int index, char *maxBitRate)
{
	return stringValueGet(__FUNCTION__,
		index,
		maxBitRate,
		DEVICE_RADIO,
		"MaxBitRate");
}


/**************************************************************************/
/*! \fn int wlan_setMaxBitRate(int index, char * maxBitRate)
 **************************************************************************
 *  \brief set maximum upstream and downstream bit rate.
 *  \param[in] int index - The index of the entry
 *  \param[in] char * maxBitRate - The maximum upstream and downstream bit rate available
 *  to this connection in Mbps. Either Auto, or the largest of
 *  the OperationalDataTransmitRates values.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setMaxBitRate(int index, char * maxBitRate)
{
	(void)index;       // unused parameter
	(void)maxBitRate;  // unused parameter
	/* support Auto for now ==> just return OK */
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getChannel(int index, u_int_32 *channel)
 **************************************************************************
 *  \brief get radio channel.
 *  \param[in] int index - The index of the entry
 *  \param[out] u_int_32 *channel - radio channel (0-255); used either by setting
 *  explicitly or by applying auth auto channel selection.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getChannel(int index, u_int_32 *channel)
{
	return intValueGet(__FUNCTION__,
		index,
		(int*)channel,
		DEVICE_RADIO,
		"Channel");
}


/**************************************************************************/
/*! \fn int wlan_setChannel(int index, u_int_32 channel)
 **************************************************************************
 *  \brief set radio channel.
 *  \param[in] int index - The index of the entry
 *  \param[in] u_int_32 channel - radio channel (0-255)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setChannel(int index, u_int_32 channel)
{
	char    extensionChannel[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    band[MAX_LEN_PARAM_VALUE] = { '\0' };
	int retVal = 0;

	stringValueGet(__FUNCTION__,
		index,
		band,
		DEVICE_RADIO,
		"SupportedFrequencyBands");

	if (!strcmp(band, "2.4GHz"))
	{
		// TODO: channel in 2.4G can be up to 14.
		// Instead of checking upper bounds, add assert that channel is legal
		if ((channel >= 1) && (channel <= 4))
		{
			strcpy(extensionChannel, "AboveControlChannel");
		}
		else if ((channel >= 7) && (channel <= 11))
		{
			strcpy(extensionChannel, "BelowControlChannel");
		}
	} else if (!strcmp(band, "5GHz")) {
		if ((channel >= 36) && (channel <= 64))
		{
			if (((channel - 36) % 8) == 0)
			{
				strcpy(extensionChannel, "AboveControlChannel");
			}
			else if (((channel - 40) % 8) == 0)
			{
				strcpy(extensionChannel, "BelowControlChannel");
			}
			else
			{
				return UGW_FAILURE;
			}
		}
		else if ((channel >= 100) && (channel <= 144))
		{
			if (((channel - 100) % 8) == 0)
			{
				strcpy(extensionChannel, "AboveControlChannel");
			}
			else if (((channel - 104) % 8) == 0)
			{
				strcpy(extensionChannel, "BelowControlChannel");
			}
			else
			{
				return UGW_FAILURE;
			}
		}
		else if ((channel >= 149) && (channel <= 161))
		{
			if (((channel - 149) % 8) == 0)
			{
				strcpy(extensionChannel, "AboveControlChannel");
			}
			else if (((channel - 153) % 8) == 0)
			{
				strcpy(extensionChannel, "BelowControlChannel");
			}
			else
			{
				return UGW_FAILURE;
			}
		}
	} else {
		return UGW_FAILURE;
	}

	/* Turn off the "Auto Channel" mode */
	retVal =+ boolValueSet(fapi_wlan_radio_set,
		index,
		false,
		DEVICE_RADIO,
		"AutoChannelEnable");

	/* Set the extension-channel */
	retVal =+ stringValueSet(fapi_wlan_radio_set,
		index,
		extensionChannel,
		DEVICE_RADIO,
		"ExtensionChannel");

	retVal =+ intValueSet(fapi_wlan_radio_set,
		index,
		channel,
		DEVICE_RADIO,
		"Channel");

	return retVal;
}

int wlan_getAutoChannelEnable(int index, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_RADIO,
		"AutoChannelEnable");
}


/**************************************************************************/
/*! \fn int wlan_getEnableOnline(int index, bool * enabled)
**************************************************************************
*  \brief get Enable online
*  \param[in] int index - AP Index
*  \param[in] bool *enabled - true/false
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getEnableOnline(int index, bool * enabled)
{
	return boolValueGet(__FUNCTION__,
		index,
		enabled,
		DEVICE_ACCESS_POINT_VENDOR,
		"EnableOnLine");
}

/**************************************************************************/
/*! \fn int wlan_setEnableOnline(int index, bool enabled)
**************************************************************************
*  \brief set Enable Online
*  \param[in] int index - AP Index
*  \param[in] bool routerEnabled - true/false
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setEnableOnline(int index, bool enabled)
{
	return boolValueSet(fapi_wlan_ap_set,
		index,
		enabled,
		DEVICE_ACCESS_POINT_VENDOR,
		"EnableOnLine");
}

/**************************************************************************/
/*! \fn int wlan_setAutoChannelEnable(int index, bool enable)
 **************************************************************************
 *  \brief set auto channel enabled/disabled flag.
 *  \param[in] int index - The index of the entry
 *  \param[in] bool enable - Auto channel enabled flag
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setAutoChannelEnable(int index, bool enable)
{
	return boolValueSet(fapi_wlan_radio_set,
		index,
		enable,
		DEVICE_RADIO,
		"AutoChannelEnable");
}

/**************************************************************************/
/*! \fn int wlan_getUdmaEnabled(int index, bool * enable)
**************************************************************************
*  \brief get UdmaEnabled enable disable the UDMA
*  \param[in] int index - AP index
*  \param[out]  bool *enable true or false
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getUdmaEnabled(int index, bool * enable)
{
	// TODO: Wrap with ifdef YOCTO_PUMA6
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_ACCESS_POINT_VENDOR,
		"UdmaEnabled");
}

/**************************************************************************/
/*! \fn int wlan_setUdmaEnabled(int index, bool enable)
 **************************************************************************
 *  \brief set UdmaEnabled enable disable the UDMA
 *  \param[in] int index - AP index
 *  \param[in] bool enable true or false
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setUdmaEnabled(int index, bool enable)
{
	// TODO: Wrap with ifdef YOCTO_PUMA6
	return boolValueSet(fapi_wlan_ap_set,
		index,
		enable,
		DEVICE_ACCESS_POINT_VENDOR,
		"UdmaEnabled");
}

/**************************************************************************/
/*! \fn int wlan_getVlanId(int index, int *vlanId)
 **************************************************************************
 *  \brief get vlanID associated with ap index
 *  \param[in] int index - AP Index
 *  \param[out] int *vlanId - vlan ID
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getVlanId(int index, int *vlanId)
{
	return intValueGet(__FUNCTION__,
		index,
		vlanId,
		DEVICE_ACCESS_POINT_VENDOR,
		"UdmaVlanId");
}

/**************************************************************************/
/*! \fn int wlan_setVlanId(int index, int vlanId)
 **************************************************************************
 *  \brief set vlanID associated with ap index
 *  \param[in] int index - AP Index
 *  \param[out] int vlanId - vlan ID
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setVlanId(int index, int vlanId)
{
	return intValueSet(fapi_wlan_ap_set,
		index,
		vlanId,
		DEVICE_ACCESS_POINT_VENDOR,
		"UdmaVlanId");
}

int wlan_getIndexFromInterface(char *inputIfName, int *nIndex)
{
	int wlan_string_length = strlen("wlan");
	char *ptrInputIfName = inputIfName;
	
	if (strlen(ptrInputIfName) > 4 && (strstr(ptrInputIfName, "wlan"))) {
		ptrInputIfName += wlan_string_length;
		*nIndex = atoi(ptrInputIfName);
		return UGW_SUCCESS;
	}
	else {
		return UGW_FAILURE;
	}
}

int wlan_getIndexFromName(char *inputAthName, int *sSIDIndex)
{
	ObjList *tempLoopObj = NULL;
	ParamList *param = NULL;
	char vapIndex[MAX_LEN_PARAM_VALUE];
	char tmpSSID[MAX_LEN_PARAM_VALUE];
	bool foundSSID = false;

	fapiWlanFailSafeLoad();

	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		if (foundSSID == true)
		{
			break;
		}

		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RPC_INDEX, GET_PARAM_NAME(param)))
			{
				strncpy(vapIndex, GET_PARAM_VALUE(param), MAX_LEN_PARAM_VALUE);
				strRemoveAll(vapIndex, "wlan");
				wlan_getSSID(atoi(vapIndex), tmpSSID);
				if (!strcmp(inputAthName, tmpSSID))
				{
					foundSSID = true;
					*sSIDIndex = atoi(vapIndex);
				}
			}
		}
	}

	if (foundSSID == true)
	{
		return UGW_SUCCESS;
	}
	else {
		return UGW_FAILURE;
	}
}

int wlan_getRadioIndex(int index, int *radio_idx)
{
	ObjList *tempLoopObj = NULL;
	ParamList *param = NULL;
	char strIndex[MAX_LEN_PARAM_VALUE];
	bool foundIndex = false;

	fapiWlanFailSafeLoad();

	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		if (foundIndex == true)
		{
			break;
		}

		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RPC_INDEX, GET_PARAM_NAME(param)))
			{
				sprintf(strIndex, "%s%d", "wlan", index);
				if (!strcmp(strIndex, GET_PARAM_VALUE(param)))
				{
					foundIndex = true;
				}
			}

			if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
			{
				strncpy(strIndex, GET_PARAM_VALUE(param), MAX_LEN_PARAM_VALUE);
				strRemoveAll(strIndex, "wlan");
				*radio_idx = atoi(strIndex);
			}
		}
	}

	if (foundIndex == true)
	{
		return UGW_SUCCESS;
	}
	else {
		return UGW_FAILURE;
	}
}

/**************************************************************************/
/*! \fn int wlan_getSSID(int index, char *ssid)
 **************************************************************************
 *  \brief get SSID.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *ssid - SSID string (max 32 bytes)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getSSID(int index, char *ssid)
{
	return stringValueGet(__FUNCTION__,
		index,
		ssid,
		DEVICE_SSID,
		"SSID");
}


/**************************************************************************/
/*! \fn int wlan_setSSID(int index, char *ssid)
 **************************************************************************
 *  \brief set SSID.
 *  \param[in] int index - The index of the entry
 *  \param[in] char *out_str - SSID string (max 32 bytes)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setSSID(int index, char *ssid)
{
	return stringValueSet(fapi_wlan_ssid_set,
		index,
		ssid,
		DEVICE_SSID,
		"SSID");
}


/**************************************************************************/
/*! \fn int wlan_setChannelMode(int index, char *channelMode,bool gOnlyFlag,
* 				bool nOnlyFlag,bool acOnlyFlag)
**************************************************************************
*  \brief get Channel Mode
*  \param[in] int index - AP index
*  \param[in] char *channelMode - channel Mode
*  \param[in] bool gOnlyFlag
*  \param[in] bool nOnlyFlag
*  \param[in] bool acOnlyFlag
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setChannelMode(int index, char *channelMode, bool gOnlyFlag, bool nOnlyFlag, bool acOnlyFlag)
{
	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(wlObj);

	if ((gOnlyFlag == true) || (acOnlyFlag == true) || (nOnlyFlag == true))
	{
		printf(" __FUNCTION__ : hostapd doesn't support only mode\n");
		return UGW_FAILURE;
	}

	if ((!strcmp(channelMode, "11A")) || (!strcmp(channelMode, "11G")) || (!strcmp(channelMode, "11B")))
	{
		printf(" __FUNCTION__ : hostapd doesn't support only mode\n");
		return UGW_FAILURE;
	}

	help_addObjList(wlObj, DEVICE_RADIO, 0, 0, 0, 0);

	if (!strcmp(channelMode, "11NGHT20"))
	{
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingStandards", "n,g", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingFrequencyBand", "2.4GHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingChannelBandwidth", "20MHz", 0, 0);
	}

	if (!strcmp(channelMode, "11NGHT40PLUS"))
	{
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingStandards", "n,g", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingFrequencyBand", "2.4GHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingChannelBandwidth", "40MHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "ExtensionChannel", "AboveControlChannel", 0, 0);
	}

	if (!strcmp(channelMode, "11NGHT40MINUS"))
	{
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingStandards", "n,g", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingFrequencyBand", "2.4GHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingChannelBandwidth", "40MHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "ExtensionChannel", "BelowControlChannel", 0, 0);
	}


	if (!strcmp(channelMode, "11NAHT40PLUS"))
	{
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingStandards", "n,a", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingFrequencyBand", "5GHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingChannelBandwidth", "40MHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "ExtensionChannel", "AboveControlChannel", 0, 0);
	}

	if (!strcmp(channelMode, "11NAHT40MINUS"))
	{
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingStandards", "n,a", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingFrequencyBand", "5GHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingChannelBandwidth", "40MHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "ExtensionChannel", "BelowControlChannel", 0, 0);
	}

	if (!strcmp(channelMode, "11ACVHT20"))
	{
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingStandards", "n,a,ac", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingFrequencyBand", "5GHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingChannelBandwidth", "20MHz", 0, 0);
	}

	if (!strcmp(channelMode, "11ACVHT40PLUS"))
	{
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingStandards", "n,a,ac", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingFrequencyBand", "5GHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingChannelBandwidth", "40MHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "ExtensionChannel", "AboveControlChannel", 0, 0);
	}

	if (!strcmp(channelMode, "11ACVHT40MINUS"))
	{
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingStandards", "n,a,ac", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingFrequencyBand", "5GHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingChannelBandwidth", "40MHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "ExtensionChannel", "BelowControlChannel", 0, 0);
	}

	if (!strcmp(channelMode, "11ACVHT80"))
	{
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingStandards", "n,a,ac", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingFrequencyBand", "5GHz", 0, 0);
		HELP_EDIT_NODE(wlObj, DEVICE_RADIO, "OperatingChannelBandwidth", "80MHz", 0, 0);
	}

	if (fapi_wlan_generic_set(fapi_wlan_radio_set, index, wlObj, 0) == UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_SUCCESS;
	}
	else {
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}
}


int wlan_getAllAssociatedDeviceDetail(int apIndex, unsigned long *output_ulong, wifi_device_fapi_t ***output_struct)
{
	int numOfAssociatedDevices = -1;
	int i = 0;

	ObjList *wlObjTmp = NULL;
	ParamList *param = NULL;
	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	if (fapi_wlan_generic_set(fapi_wlan_assoc_devices_query, apIndex, wlObj, 0) != UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	debug_obj(wlObj);
	//Count number of objects

	FOR_EACH_OBJ(wlObj, wlObjTmp)
	{
		if (!strcmp(GET_OBJ_NAME(wlObjTmp), DEVICE_ACCESS_POINT))
		{
			FOR_EACH_PARAM(wlObjTmp, param)
			{
				if (!strcmp("AssociatedDeviceNumberOfEntries", GET_PARAM_NAME(param)))
				{
					numOfAssociatedDevices = atoi(GET_PARAM_VALUE(param));
					break;
				}
			}
		}
	}

	*output_ulong = numOfAssociatedDevices;

	if (numOfAssociatedDevices < 0)
	{
		printf("ERROR: %s can't get numOfAssociatedDevices from lower level. \n", __FUNCTION__);
		return UGW_FAILURE;
	}
	else if (numOfAssociatedDevices == 0) {
		printf("%s numOfAssociatedDevices = 0 \n", __FUNCTION__);
		return UGW_SUCCESS;
	}

	*output_struct = (wifi_device_fapi_t**)calloc(numOfAssociatedDevices, sizeof(wifi_device_fapi_t*));

	param = NULL;
	wlObjTmp = NULL;

	FOR_EACH_OBJ(wlObj, wlObjTmp)
	{
		if (!strcmp(GET_OBJ_NAME(wlObjTmp), DEVICE_ACCESS_POINT_AD))
		{
			(*output_struct)[i] = (wifi_device_fapi_t*)calloc(1, sizeof(wifi_device_fapi_t));
			i++;
			FOR_EACH_PARAM(wlObjTmp, param)
			{
				//TODO add support for two API's
				//unsigned char wifi_devMacAddress[6];
				//char wifi_devIPAddress[64];
				//if (!strcmp("MACAddress", GET_PARAM_NAME(param)))

				if (!strcmp("AuthenticationState", GET_PARAM_NAME(param)))
				{
					if (!strcmp(GET_PARAM_VALUE(param), "true"))
					{
						(*output_struct)[i]->wifi_devAssociatedDeviceAuthentiationState = true;
					}
					else if (!strcmp(GET_PARAM_VALUE(param), "false")) {
						(*output_struct)[i]->wifi_devAssociatedDeviceAuthentiationState = false;
					}
					else {
						printf("%s; ERROR: AuthenticationState is bool type but has other than false\true value.\n", __FUNCTION__);
						return UGW_FAILURE;
					}
				}

				if (!strcmp("SignalStrength", GET_PARAM_NAME(param)))
				{
					(*output_struct[i])->wifi_devSignalStrength = atoi(GET_PARAM_VALUE(param));
				}

				if (!strcmp("LastDataUplinkRate", GET_PARAM_NAME(param)))
				{
					(*output_struct[i])->wifi_devRxRate = atoi(GET_PARAM_VALUE(param));
				}

				if (!strcmp("LastDataDownlinkRate", GET_PARAM_NAME(param)))
				{
					(*output_struct[i])->wifi_devTxRate = atoi(GET_PARAM_VALUE(param));
				}

				if (!strcmp("OperatingStandard", GET_PARAM_NAME(param)))
				{
					strncmp((char *)(*output_struct[i])->wifi_devMode, GET_PARAM_VALUE(param), 2);
				}
			}
		}
	}
	HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
	return UGW_SUCCESS;
}


int wlan_getAssociatedDeviceDetail(int apIndex, int devIndex, wifi_device_fapi_t *output_struct)
{
	(void)apIndex;        // unused parameter
	(void)devIndex;       // unused parameter
	(void)output_struct;  // unused parameter
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_getChannelMode(int index, char *channelMode)
**************************************************************************
*  \brief get Channel Mode
*  \param[in] int index - AP index
*  \param[out] char *channelMode - channel Mode
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getChannelMode(int index, char *channelMode)
{
	char radio[MAX_LEN_PARAM_VALUE];
	char netMode[MAX_LEN_PARAM_VALUE];
	char exChannel[MAX_LEN_PARAM_VALUE];
	char chBandwith[MAX_LEN_PARAM_VALUE];
	int ret_val = 0;

	ret_val += wlan_getStandard(index, (char*)netMode);
	ret_val += wlan_getOperatingFrequencyBand(index, (char*)radio);
	ret_val += wlan_getChannelBandwidth(index, (char*)chBandwith);
	ret_val += wlan_getExtensionChannel(index, (char*)exChannel);

	if (ret_val != 0)
	{
		printf("wlan_getChannelMode: error returned from get functions\n");
		return UGW_FAILURE;
	}

	if (!strcmp(netMode, "a"))
	{
		if (!strcmp(radio, "5GHz"))
		{
			strcpy(channelMode, "11A");
			return UGW_SUCCESS;
		}
	}

	if (!strcmp(netMode, "b"))
	{
		if (!strcmp(radio, "2.4GHz"))
		{
			strcpy(channelMode, "11B");
			return UGW_SUCCESS;
		}
	}

	if (!strcmp(netMode, "g"))
	{
		if (!strcmp(radio, "2.4GHz"))
		{
			strcpy(channelMode, "11G");
			return UGW_SUCCESS;
		}
	}

	if ((!strcmp(netMode, "n,a")) || (!strcmp(netMode, "a,n")))
	{
		if ((!strcmp(radio, "5GHz")) && (!strcmp(chBandwith, "20MHz")))
		{
			strcpy(channelMode, "11NAHT20");
			return UGW_SUCCESS;
		}
		else if ((!strcmp(radio, "5GHz")) && (!strcmp(chBandwith, "40MHz")))
		{
			if (!strcmp(exChannel, "AboveControlChannel"))
			{
				strcpy(channelMode, "11NAHT40PLUS");
				return UGW_SUCCESS;
			}
			else if (!strcmp(exChannel, "BelowControlChannel")) {
				strcpy(channelMode, "11NAHT40MINUS");
				return UGW_SUCCESS;
			}
		}
		/*else if ((!strcmp(radio, "5GHz")) && (!strcmp(chBandwith, "Auto")))
		{
			strcpy(channelMode, "11NAHT40AUTO");
			return UGW_SUCCESS;
		}*/
	}

	if ((strstr(netMode, "n")) && (strstr(netMode, "g")) && (strstr(netMode, "b")))
	{
		if ((!strcmp(radio, "2.4GHz")) && (!strcmp(chBandwith, "20MHz")))
		{
			strcpy(channelMode, "11NGHT20");
			return UGW_SUCCESS;
		}
		else if ((!strcmp(radio, "2.4GHz")) && (!strcmp(chBandwith, "40MHz")))
		{
			if (!strcmp(exChannel, "AboveControlChannel"))
			{
				strcpy(channelMode, "11NGHT40PLUS");
				return UGW_SUCCESS;
			}
			else if (!strcmp(exChannel, "BelowControlChannel")) {
				strcpy(channelMode, "11NGHT40MINUS");
				return UGW_SUCCESS;
			}
		}
		/*else if ((!strcmp(radio, "2.4GHz")) && (!strcmp(chBandwith, "Auto")))
		{
			strcpy(channelMode, "11NGHTAUTO");
			return UGW_SUCCESS;
		}*/
	}

	if ((strstr(netMode, "n")) && (strstr(netMode, "g")))
	{
		if ((!strcmp(radio, "2.4GHz")) && (!strcmp(chBandwith, "20MHz")))
		{
			strcpy(channelMode, "11NGHT20");
			return UGW_SUCCESS;
		}
		else if ((!strcmp(radio, "2.4GHz")) && (!strcmp(chBandwith, "40MHz")))
		{
			if (!strcmp(exChannel, "AboveControlChannel"))
			{
				strcpy(channelMode, "11NGHT40PLUS");
				return UGW_SUCCESS;
			}
			else if (!strcmp(exChannel, "BelowControlChannel")) {
				strcpy(channelMode, "11NGHT40MINUS");
				return UGW_SUCCESS;
			}
		}
		/*else if ((!strcmp(radio, "2.4GHz")) && (!strcmp(chBandwith, "Auto")))
		{
			strcpy(channelMode, "11NGHTAUTO");
			return UGW_SUCCESS;
		}*/
	}

	if ((strstr(netMode, "ac")) && (strstr(netMode, "a")) && (strstr(netMode, "n")))
	{
		if (!strcmp(radio, "5GHz"))
		{
			if (!strcmp(chBandwith, "20MHz"))
			{
				strcpy(channelMode, "11ACVHT20");
				return UGW_SUCCESS;
			}
			else if (!strcmp(chBandwith, "80MHz"))
			{
				strcpy(channelMode, "11ACVHT80");
				return UGW_SUCCESS;
			}
			else if (!strcmp(chBandwith, "40MHz"))
			{
				if (!strcmp(exChannel, "AboveControlChannel"))
				{
					strcpy(channelMode, "11ACVHT40PLUS");
					return UGW_SUCCESS;
				}
				else if (!strcmp(exChannel, "BelowControlChannel"))
				{
					strcpy(channelMode, "11ACVHT40MINUS");
					return UGW_SUCCESS;
				}
			}
			/*else if (!strcmp(chBandwith, "Auto"))
			{
				strcpy(channelMode, "11ACVHTAUTO");
				return UGW_SUCCESS;
			}*/
		}
	}

	return UGW_FAILURE;
}


/**************************************************************************/
/*! \fn int wlan_getBeaconType(int index, char *beaconType)
 **************************************************************************
 *  \brief get Beacon type.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *beaconType - Beacon type string:
 *  "None", "Basic", "WPA", "11i" (OPTIONAL), "WPAand11i" (OPTIONAL)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getBeaconType(int index, char *beaconType)
{
	return securityModeGet(index, beaconType, "BeaconType", "beaconType");
}

/**************************************************************************/
/*! \fn int wlan_setObssCoexistenceEnable(int index, bool enabled)
**************************************************************************
*  \brief Enable / Disable Obss Coexistence
*  \param[in] int index - AP Index
*  \param[in] bool enable
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setObssCoexistenceEnable(int index, bool enabled)
{
	char    operatingChannelBandwidth[] = "20MHz";

	if (enabled)
	{
		strcpy(operatingChannelBandwidth, "Auto");
	}

	return stringValueSet(fapi_wlan_radio_set,
		index,
		operatingChannelBandwidth,
		DEVICE_RADIO,
		"OperatingChannelBandwidth");
}


/**************************************************************************/
/*! \fn int wlan_setBeaconType(int index, char *beaconType)
 **************************************************************************
 *  \brief set Beacon type.
 *  \param[in] int index - The index of the entry
 *  \param[in] char *beaconType - Beacon type string:
 *  "None", "Basic", "WPA", "11i" (OPTIONAL), "WPAand11i" (OPTIONAL)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setBeaconType(int index, char *beaconType)
{
	char    modeEnabledValue[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    *modeEnabledValuePtr = modeEnabledValue;
	char    encryptionMode[MAX_LEN_PARAM_VALUE] = "ENC_NON_VALID";
	char    authenticationMode[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };
	ObjList *wlObjSecurity = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	if (wlObjSecurity == NULL)
	{
		return UGW_FAILURE;
	}

	if (!strcmp(beaconType, "None"))
	{
		strcpy(modeEnabledValue, "None");
	}
	else if (!strcmp(beaconType, "Basic"))
	{
		strcpy(modeEnabledValue, "WEP-64");  // WEP-128 is NOT supported!
	}
	/* From here below, set encryptionMode */
	else if (!strcmp(beaconType, "WPA"))
	{
		strcpy(encryptionMode, "ENC_TKIP");
	}
	else if (!strcmp(beaconType, "11i"))
	{
		strcpy(encryptionMode, "ENC_AES");
	}
	else if (!strcmp(beaconType, "WPAand11i"))
	{
		strcpy(encryptionMode, "ENC_TKIP_AND_AES");
	}
	else
	{
		return UGW_FAILURE;
	}

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	/* Store the Beacon Type in the security file - will be used in the 'get' function */
	securityModeSet(wlObjSecurity, interfaceName, beaconType, "BeaconType");

	HELP_DELETE_OBJ(wlObjSecurity, SOPT_OBJVALUE, FREE_OBJLIST);

	if (modeEnabledValue[0] != '\0')
	{
		return (wlan_security_set(index, modeEnabledValue));
	}

	if (modeEnabledValueGet(interfaceName, encryptionMode, authenticationMode, &modeEnabledValuePtr) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	if (modeEnabledValue[0] != '\0')
	{
		return (wlan_security_set(index, modeEnabledValue));
	}

	return UGW_SUCCESS;
}

int wlan_getMacAddressControlEnabled(int index, bool *enabled)
{
	return boolValueGet(__FUNCTION__,
		index,
		enabled,
		DEVICE_ACCESS_POINT_VENDOR,
		"MACAddressControlEnabled");
}

int wlan_getRadioAutoChannelRefreshPeriodSupported(int index, bool *supported)
{
	(void)index;  // unused parameter
	*supported = false;
	return UGW_SUCCESS;
}

int wlan_getRadioAutoChannelRefreshPeriod(int index, u_int_32 *refreshPeriod)
{
	return intValueGet(__FUNCTION__,
		index,
		(int*)refreshPeriod,
		DEVICE_RADIO,
		"AutoChannelRefreshPeriod");
}

int wlan_setRadioAutoChannelRefreshPeriod(int index, u_int_32 refreshPeriod)
{
	(void)index;          // unused parameter
	(void)refreshPeriod;  // unused parameter
	return UGW_FAILURE;
}


/**************************************************************************/
/*! \fn int wlan_setMacAddressControlEnabled(int index, bool enabled)
 **************************************************************************
 *  \brief set MAC Address Control Enable flag.
 *  \param[in] int index - The index of the entry
 *  \param[in] bool enabled - enable flag. When enabled, a MAC address list
 *  of allowed stations is provided by proprietary API
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setMacAddressControlEnabled(int index, bool enabled)
{
	return boolValueSet(fapi_wlan_ap_set,
		index,
		enabled,
		DEVICE_ACCESS_POINT_VENDOR,
		"MACAddressControlEnabled");
}


/**************************************************************************/
/*! \fn int wlan_getStandard(int index, char *standard)
 **************************************************************************
 *  \brief get Standard.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *standard - Either one of the following standard represented as a string -
 *  "a","b","g" (b and g clients supported),"g-only" (only g clients supported),"n".
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getStandard(int index, char *standard)
{
	return stringValueGet(__FUNCTION__,
		index,
		standard,
		DEVICE_RADIO,
		"OperatingStandards");
}

int wlan_getChannelBandwidth(int radioIndex, char *channelBandwidth)
{

	return stringValueGet(__FUNCTION__,
		radioIndex,
		channelBandwidth,
		DEVICE_RADIO,
		"OperatingChannelBandwidth");
}

int wlan_getOperatingFrequencyBand(int radioIndex, char *operatingFrequencyBand)
{

	return stringValueGet(__FUNCTION__,
		radioIndex,
		operatingFrequencyBand,
		DEVICE_RADIO,
		"OperatingFrequencyBand");
}

int wlan_getExtensionChannel(int radioIndex, char *extensionChannel)
{

	return stringValueGet(__FUNCTION__,
		radioIndex,
		extensionChannel,
		DEVICE_RADIO,
		"ExtensionChannel");
}

int wlan_getApWMMCapability(int index, bool *output)
{

	return boolValueGet(__FUNCTION__,
		index,
		output,
		DEVICE_ACCESS_POINT,
		"WMMCapability");
}

int wlan_getApUAPSDCapability(int index, bool *output)
{

	return boolValueGet(__FUNCTION__,
		index,
		output,
		DEVICE_ACCESS_POINT,
		"UAPSDCapability");
}

int wlan_setApIsolationEnable(int index, bool enable)
{
	return boolValueSet(fapi_wlan_ap_set,
		index,
		enable,
		DEVICE_ACCESS_POINT,
		"IsolationEnable");
}

int wlan_getApIsolationEnable(int index, bool *output)
{

	return boolValueGet(__FUNCTION__,
		index,
		output,
		DEVICE_ACCESS_POINT,
		"IsolationEnable");
}

int wlan_getApWmmUapsdEnable(int index, bool *output)
{

	return boolValueGet(__FUNCTION__,
		index,
		output,
		DEVICE_ACCESS_POINT,
		"UAPSDEnable");
}

int wlan_setApWmmUapsdEnable(int index, bool enable)
{
	return boolValueSet(fapi_wlan_ap_set,
		index,
		enable,
		DEVICE_ACCESS_POINT,
		"UAPSDEnable");
}

int wlan_getApSecurityRadiusServerIPAddr(int index, char *output)
{
	return stringValueGet(__FUNCTION__,
		index,
		output,
		DEVICE_ACCESS_POINT_SECURITY,
		"RadiusServerIPAddr");
}

int wlan_setApSecurityRadiusServerIPAddr(int index, char *IPAddress)
{
	return stringValueSet(fapi_wlan_security_set,
		index,
		IPAddress,
		DEVICE_ACCESS_POINT_SECURITY,
		"RadiusServerIPAddr");
}

int wlan_getApSecurityRadiusServerPort(int index, unsigned int *output)
{
	return intValueGet(__FUNCTION__,
		index,
		(int*)output,
		DEVICE_ACCESS_POINT_SECURITY,
		"RadiusServerPort");
}

int wlan_setApSecurityRadiusServerPort(int index, unsigned int port)
{
	return intValueSet(fapi_wlan_security_set,
		index,
		port,
		DEVICE_ACCESS_POINT_SECURITY,
		"RadiusServerPort");
}

int wlan_getApSecurityRadiusServer(int index, char *IP_output, unsigned int *Port_output)
{
	int retVal = 0;

	retVal += intValueGet(__FUNCTION__,
		index,
		(int*)Port_output,
		DEVICE_ACCESS_POINT_SECURITY,
		"RadiusServerPort");

	retVal += stringValueGet(__FUNCTION__,
		index,
		IP_output,
		DEVICE_ACCESS_POINT_SECURITY,
		"RadiusServerIPAddr");

	return retVal;
}

int wlan_setApSecurityRadiusServer(int index, char *IPAddress, unsigned int port)
{
	if (stringValueSet(fapi_wlan_security_set,
		index,
		IPAddress,
		DEVICE_ACCESS_POINT_SECURITY,
		"RadiusServerIPAddr") != UGW_SUCCESS)
	{
		return UGW_FAILURE;
	}

	return intValueSet(fapi_wlan_security_set,
		index,
		port,
		DEVICE_ACCESS_POINT_SECURITY,
		"RadiusServerPort");
}

int wlan_getRadiusSecret(int index, char *radiusSecret)
{
	return stringValueGet(__FUNCTION__,
		index,
		radiusSecret,
		DEVICE_ACCESS_POINT_SECURITY,
		"RadiusSecret");
}
int wlan_setRadiusSecret(int index, char *radiusSecret)
{
	return stringValueSet(fapi_wlan_security_set,
		index,
		radiusSecret,
		DEVICE_ACCESS_POINT_SECURITY,
		"RadiusSecret");
}
/**************************************************************************/
/*! \fn int wlan_setApBridging(int index, bool enable)
**************************************************************************
*  \brief Enable /Disable AP bridging
*  \param[in] int index - AP Index
*  \param[in] int enable - true/false
*  \return 0 if success, negative if error / timeout
*  This function uses AP isolation for configuring AP forwarding, so invert the input
***************************************************************************/
int wlan_setApBridging(int index, bool enable)
{
	enable = !enable;
	return boolValueSet(fapi_wlan_ap_set,
		index,
		enable,
		DEVICE_ACCESS_POINT,
		"IsolationEnable");
}

/**************************************************************************/
/*! \fn int wlan_getWepKeyIndex(int index, u_int_32 *keyIndex)
 **************************************************************************
 *  \brief get WEP key index.
 *  \param[in] int index - The index of the entry
 *  \param[out] u_int_32 *keyIndex - WEP key index (value 1-4)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWepKeyIndex(int index, u_int_32 *keyIndex)
{
	(void)index;  // unused parameter
	*keyIndex = 1;  // We allow settings for one key only!
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setWepKeyIndex(int index, u_int_32 keyIndex)
 **************************************************************************
 *  \brief set WEP key index.
 *  \param[in] int index - The index of the entry
 *  \param[out] u_int_32 keyIndex - WEP key index (value 1-4)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWepKeyIndex(int index, u_int_32 keyIndex)
{
	(void)index;  // unused parameter
	// We allow settings for one key only ==> just return OK
	if (keyIndex == 1)
	{
		return UGW_SUCCESS;
	}
	else {
		return UGW_FAILURE;
	}

}


/**************************************************************************/
/*! \fn int wlan_getWepKeyPassphrase(int index, char *passphrase)
 **************************************************************************
 *  \brief get WEP key passphrase.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *passphrase - The passphrase (max 63 characters)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWepKeyPassphrase(int index, char *passphrase)
{
	return stringValueGet(__FUNCTION__,
		index,
		passphrase,
		DEVICE_ACCESS_POINT_SECURITY,
		"PreSharedKey");
}


/**************************************************************************/
/*! \fn int wlan_setWepKeyPassphrase(int index, char *passphrase)
 **************************************************************************
 *  \brief set WEP key passphrase.
 *  \param[in] int index - The index of the entry
 *  \param[in] char *passphrase - The passphrase (max 63 characters)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWepKeyPassphrase(int index, char *passphrase)
{
	return stringValueSet(fapi_wlan_security_set,
		index,
		passphrase,
		DEVICE_ACCESS_POINT_SECURITY,
		"PreSharedKey");
}


/**************************************************************************/
/*! \fn int wlan_getWepEncryptionLevel(int index, char *encLevel)
 **************************************************************************
 *  \brief get WEP Encryption Level.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *encLevel - A comma separated list of the supported levels:
 *  "Disabled", "40-bit", "104-bit"
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWepEncryptionLevel(int index, char *encLevel)
{
	(void)index;     // unused parameter
	(void)encLevel;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getBasicEncryptionModes(int index, char *encModes)
 **************************************************************************
 *  \brief get Basic Encryption Modes.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *encModes - Either "None" or "WEPEncryption"
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getBasicEncryptionModes(int index, char *encModes)
{
	return securityModeGet(index, encModes, "BasicEncModes", "encModes");
}


/**************************************************************************/
/*! \fn int wlan_setBasicEncryptionModes(int index, char *encModes)
 **************************************************************************
 *  \brief set Basic Encryption Modes.
 *  \param[in] int index - The index of the entry
 *  \param[in] char *encModes - Either "None" or "WEPEncryption"
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setBasicEncryptionModes(int index, char *encModes)
{
	char modeEnabledValue[MAX_LEN_PARAM_VALUE] = { '\0' };
	ObjList *wlObjSecurity = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if (wlObjSecurity == NULL)
	{
		return UGW_FAILURE;
	}

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	/* Store the Basic Encryption Mode in the security file - will be used in the 'get' function */
	securityModeSet(wlObjSecurity, interfaceName, encModes, "BasicEncModes");

	HELP_DELETE_OBJ(wlObjSecurity, SOPT_OBJVALUE, FREE_OBJLIST);

	if (!strcmp(encModes, "None"))
		strcpy(modeEnabledValue, "None");
	else if (!strcmp(encModes, "WEPEncryption"))
		strcpy(modeEnabledValue, "WEP-64");  // WEP-128 is NOT supported!

	return (wlan_security_set(index, modeEnabledValue));
}


/**************************************************************************/
/*! \fn int wlan_getBasicAuthenticationModes(int index, char *authMode)
 **************************************************************************
 *  \brief get Basic Authentication Modes.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *authMode - Either one of "None" (Open authentication),
 *  "EAPAuthentication" (OPTIONAL), "SharedAuthentication" (OPTIONAL)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getBasicAuthenticationModes(int index, char *authMode)
{
	return securityModeGet(index, authMode, "BasicAuthMode", "authMode");
}


/**************************************************************************/
/*! \fn int wlan_setBasicAuthenticationModes(int index, char *authMode)
 **************************************************************************
 *  \brief set Basic Authentication Modes.
 *  \param[in] int index - The index of the entry
 *  \param[in] char *authMode - Either one of "None" (Open authentication),
 *  "EAPAuthentication" (OPTIONAL), "SharedAuthentication" (OPTIONAL)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setBasicAuthenticationModes(int index, char *authMode)
{
	char    modeEnabledValue[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    *modeEnabledValuePtr = modeEnabledValue;
	char    encryptionMode[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    authenticationMode[MAX_LEN_PARAM_VALUE] = "AUTH_NON_VALID";
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };
	ObjList *wlObjSecurity = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	if (wlObjSecurity == NULL)
	{
		return UGW_FAILURE;
	}

	if (!strcmp(authMode, "None"))
	{
		strcpy(authenticationMode, "AUTH_PSK");
	}
	else if (!strcmp(authMode, "EAPAuthentication"))
	{
		strcpy(authenticationMode, "AUTH_EAP");
	}
	else if (strcmp(authMode, "SharedAuthentication"))
	{  // Ignore "SharedAuthentication"
		return UGW_FAILURE;
	}

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	/* Store the Authentication Mode in the security file - will be used in the 'get' function */
	securityModeSet(wlObjSecurity, interfaceName, authMode, "BasicAuthMode");

	HELP_DELETE_OBJ(wlObjSecurity, SOPT_OBJVALUE, FREE_OBJLIST);

	modeEnabledValueGet(interfaceName, encryptionMode, authenticationMode, &modeEnabledValuePtr);

	if (modeEnabledValue[0] != '\0')
	{
		return (wlan_security_set(index, modeEnabledValue));
	}

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpaEncryptionModes(int index, char *encModes)
 **************************************************************************
 *  \brief get WPA Encryption Modes.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *encModes - Either one of "TKIPEncryption",
 *  "AESEncryption" (OPTIONAL), "TKIPandAESEncryption" (OPTIONAL)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpaEncryptionModes(int index, char *encModes)
{
	return securityModeGet(index, encModes, "WpaEncMode", "encModes");
}


/**************************************************************************/
/*! \fn int wlan_setWpaEncryptionModes(int index, char *encModes)
 **************************************************************************
 *  \brief set WPA Encryption Modes.
 *  \param[in] int index - The index of the entry
 *  \param[in] char *encModes - Either one of "TKIPEncryption",
 *  "AESEncryption" (OPTIONAL), "TKIPandAESEncryption" (OPTIONAL)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWpaEncryptionModes(int index, char *encModes)
{
	char    modeEnabledValue[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    *modeEnabledValuePtr = modeEnabledValue;
	char    encryptionMode[MAX_LEN_PARAM_VALUE] = "ENC_NON_VALID";
	char    authenticationMode[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };
	ObjList *wlObjSecurity = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	if (wlObjSecurity == NULL)
	{
		return UGW_FAILURE;
	}

	if (!strcmp(encModes, "TKIPEncryption"))
	{
		strcpy(encryptionMode, "ENC_TKIP");
	}
	else if (!strcmp(encModes, "AESEncryption"))
	{
		strcpy(encryptionMode, "ENC_AES");
	}
	else if (!strcmp(encModes, "TKIPandAESEncryption"))
	{
		strcpy(encryptionMode, "ENC_TKIP_AND_AES");
	}
	else
	{
		return UGW_FAILURE;
	}

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	/* Store the Encryption Mode in the security file - will be used in the 'get' function */
	securityModeSet(wlObjSecurity, interfaceName, encModes, "WpaEncMode");

	HELP_DELETE_OBJ(wlObjSecurity, SOPT_OBJVALUE, FREE_OBJLIST);

	modeEnabledValueGet(interfaceName, encryptionMode, authenticationMode, &modeEnabledValuePtr);

	if (modeEnabledValue[0] != '\0')
	{
		return (wlan_security_set(index, modeEnabledValue));
	}

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpaAuthenticationModes(int index, char *authMode)
 **************************************************************************
 *  \brief get WPA Authentication Modes.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *authMode - Either one of "PSKAuthentication",
 *  "EAPAuthentication" (OPTIONAL)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpaAuthenticationModes(int index, char *authMode)
{
	return securityModeGet(index, authMode, "WpaBasicAuthMode", "authMode");
}


/**************************************************************************/
/*! \fn int wlan_setWpaBasicAuthenticationModes(int index, char *authMode)
 **************************************************************************
 *  \brief set WPA Authentication Modes.
 *  \param[in] int index - The index of the entry
 *  \param[in] char *authMode - Either one of "PSKAuthentication",
 *  "EAPAuthentication" (OPTIONAL)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWpaBasicAuthenticationModes(int index, char *authMode)
{
	char    modeEnabledValue[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    *modeEnabledValuePtr = modeEnabledValue;
	char    encryptionMode[MAX_LEN_PARAM_VALUE] = { '\0' };
	char    authenticationMode[MAX_LEN_PARAM_VALUE] = "AUTH_NON_VALID";
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };
	ObjList *wlObjSecurity = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	if (wlObjSecurity == NULL)
	{
		return UGW_FAILURE;
	}

	if (!strcmp(authMode, "PSKAuthentication"))
	{
		strcpy(authenticationMode, "AUTH_PSK");
	}
	else if (!strcmp(authMode, "EAPAuthentication"))
	{
		strcpy(authenticationMode, "AUTH_EAP");
	}
	else
	{
		return UGW_FAILURE;
	}

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		return UGW_FAILURE;
	}

	/* Store the Authentication Mode in the security file - will be used in the 'get' function */
	securityModeSet(wlObjSecurity, interfaceName, authMode, "WpaBasicAuthMode");

	HELP_DELETE_OBJ(wlObjSecurity, SOPT_OBJVALUE, FREE_OBJLIST);

	modeEnabledValueGet(interfaceName, encryptionMode, authenticationMode, &modeEnabledValuePtr);

	if (modeEnabledValue[0] != '\0')
	{
		return (wlan_security_set(index, modeEnabledValue));
	}

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getPossibleChannels(int index, char *channels)
 **************************************************************************
 *  \brief get Possible Channels.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *channels - A comma separated list of possible channel scopes like 1-11
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getPossibleChannels(int index, char *channels)
{
	return stringValueGet(__FUNCTION__,
		index,
		channels,
		DEVICE_RADIO,
		"PossibleChannels");
}


/**************************************************************************/
/*! \fn int wlan_getBasicDataTransmitRates(int index, char *txRates)
 **************************************************************************
 *  \brief get Basic Data Transmit Rates as comma-separated list of the maximum access point
 *  data transmit rates in Mbps for unicast, multicast and broadcast frames.
 *  For instance: 1,2 indicates that 1 and 2 MbPS rates are supported
 *  \param[in] int index - The index of the entry
 *  \param[out] char *txRates - the maximum access point data transmit rates in Mbps.
 *  For instance: 1,2 indicates that 1 and 2 MbPS rates are supported
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getBasicDataTransmitRates(int index, char *txRates)
{
	return stringValueGet(__FUNCTION__,
		index,
		txRates,
		DEVICE_RADIO,
		"BasicDataTransmitRates");
}


/**************************************************************************/
/*! \fn int wlan_setBasicDataTransmitRates(int index, char *txRates)
 **************************************************************************
 *  \brief set Basic Data Transmit Rates as Comma-separated list of the maximum access point
 *  data transmit rates in Mbps for unicast, multicast and broadcast frames.
 *  For instance: 1,2 indicates that 1 and 2 MbPS rates are supported
 *  \param[in] int index - The index of the entry
 *  \param[in] char *txRates - the maximum access point data transmit rates in Mbps.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setBasicDataTransmitRates(int index, char *txRates)
{
	(void)index;    // unused parameter
	(void)txRates;  // unused parameter
	/* support 1,2,5.5,11,6,9,12,18,24,36,48,54 by default */
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getOperationalDataTransmitRates(int index, char *out_str)
 **************************************************************************
 *  \brief get Operational Data Transmit Rates as comma-separated list of the maximum access point
 *  data transmit rates in Mbps for unicast frames (a superset of BasicDataTransmitRates).
 *  \param[in] int index - The index of the entry
 *  \param[out] char *out_str - the maximum access point data transmit rates in Mbps.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getOperationalDataTransmitRates(int index, char *txRates)
{
	return stringValueGet(__FUNCTION__,
		index,
		txRates,
		DEVICE_RADIO,
		"OperationalDataTransmitRates");
}


/**************************************************************************/
/*! \fn int wlan_setOperationalDataTransmitRates(int index, char *txRates)
 **************************************************************************
 *  \brief set Operational Data Transmit Rates as comma-separated list of the maximum access point
 *  data transmit rates in Mbps for unicast frames (a superset of BasicDataTransmitRates).
 *  \param[in] int index - The index of the entry
 *  \param[in] char *txRates - the maximum access point data transmit rates in Mbps.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setOperationalDataTransmitRates(int index, char *txRates)
{
	(void)index;    // unused parameter
	(void)txRates;  // unused parameter
	/* support 1,2,5.5,11,6,9,12,18,24,36,48,54 by default */
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getPossibleDataTransmitRates(int index, char *txRates)
 **************************************************************************
 *  \brief get Possible Data Transmit Rates.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *txRates - Comma-separated list of the data transmit rates
 *  for unicast frames at which the access point will permit a station to connect
 *  (a superset of OperationalDataTransmitRates).
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getPossibleDataTransmitRates(int index, char *txRates)
{
	return stringValueGet(__FUNCTION__,
		index,
		txRates,
		DEVICE_RADIO,
		"SupportedDataTransmitRates");
}


int wlan_getSsidAdvertisementEnabled(int index, bool *enabled)
{
	return boolValueGet(__FUNCTION__,
		index,
		enabled,
		DEVICE_ACCESS_POINT,
		"SSIDAdvertisementEnabled");
}


int wlan_setSsidAdvertisementEnabled(int index, bool enabled)
{
	return boolValueSet(fapi_wlan_ap_set,
		index,
		enabled,
		DEVICE_ACCESS_POINT,
		"SSIDAdvertisementEnabled");
}


int wlan_getRadioEnabled(int index, bool *enabled)
{
	return boolValueGet(__FUNCTION__,
		index,
		enabled,
		DEVICE_RADIO,
		"Enable");
}


/**************************************************************************/
/*! \fn int wlan_setRadioEnabled(int index, bool enabled)
 **************************************************************************
 *  \brief set Radio enable flag.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool enabled - Radio enabled flag
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setRadioEnabled(int index, bool enabled)
{
	return boolValueSet(fapi_wlan_radio_set,
		index,
		enabled,
		DEVICE_RADIO,
		"Enable");
}


/**************************************************************************/
/*! \fn int wlan_setMaxStations(int index, int maxStations)
**************************************************************************
*  \brief Set Max supported station for apIndex
*  \param[in] int index - Index Number
*  \param[in] int maxStations - Max Number of stations supported
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setMaxStations(int index, unsigned int maxStations)
{
	return intValueSet(fapi_wlan_ap_set,
		index,
		maxStations,
		DEVICE_ACCESS_POINT,
		"MaxAssociatedDevices");
}


/**************************************************************************/
/*! \fn int wlan_getMaxStations(int index, int maxStations)
**************************************************************************
*  \brief Set Max supported station for apIndex
*  \param[in] int index - Index Number
*  \param[in] int maxStations - Max Number of stations supported
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getMaxStations(int index, unsigned int *maxStations)
{
	return intValueGet(__FUNCTION__,
		index,
		(int*)maxStations,
		DEVICE_ACCESS_POINT,
		"MaxAssociatedDevices");
}

int wlan_setWpsDevicePIN(int index, unsigned long pin)
{
	return intValueSetDbRadio(fapi_wlan_wps_set,
		index,
		pin,
		DEVICE_RADIO_WPS_VENDOR,
		"PIN");
}

int wlan_getWpsDevicePIN(int index, unsigned long *output_ulong)
{
	int radioIndex=-1;
	
	wlan_getRadioIndex(index, &radioIndex);
	
	return intValueGet(__FUNCTION__,
		radioIndex,
		(int*)output_ulong,
		DEVICE_RADIO_WPS_VENDOR,
		"PIN");
}


int wlan_getWpsStatus(int index, char *Status)
{
	int radioIndex=-1;
	
	wlan_getRadioIndex(index, &radioIndex);
	return stringValueGet(__FUNCTION__,
		radioIndex,
		Status,
		DEVICE_RADIO_WPS_VENDOR,
		"Status");
}
/**************************************************************************/
/*! \fn int wlan_getAclDeviceNum(int index, unsigned int *devNum)
**************************************************************************
*  \brief set Router Enable
*  \param[in] int index - AP Index
*  \param[out] unsigned int *devNum - Number of devices
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAclDeviceNum(int index, unsigned int *devNum)
{
	char macList[MAX_LEN_PARAM_VALUE] = { '\0' };
	int i;
	unsigned int strLen = 0;

	*devNum = 0;

	stringValueGet(__FUNCTION__,
		index,
		macList,
		DEVICE_ACCESS_POINT_VENDOR,
		"MACAddressControlList");

	strLen = strlen(macList);
	for (i = 0; i < (int)strLen; i++)
	{
		if (*(macList + i) == ',')
		{
			(*devNum)++;
		}
	}

	if (strLen >= MAC_LENGTH)
	{
		(*devNum)++;
	}

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getTransmitPowerSupported(int index, char *power)
 **************************************************************************
 *  \brief get supported transmit power levels as percentages of full power.
 *  Each value MUST be an integer in the range 0 to 100 inclusive. For example, 0,25,50,75,100.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *power - Comma-separated list of the supported transmit power levels
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getTransmitPowerSupported(int index, char *power)
{
	return stringValueGet(__FUNCTION__,
		index,
		power,
		DEVICE_RADIO,
		"TransmitPowerSupported");
}


/**************************************************************************/
/*! \fn int wlan_getTransmitPower(int index, u_int_32 *power)
 **************************************************************************
 *  \brief get the current transmit power level. Indicates the current transmit power level
 *  as a percentage of full power.
 *  \param[in] int index - The index of the entry
 *  \param[out] u_int_32 *power - The current transmit power level
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getTransmitPower(int index, u_int_32 *power)
{
	return intValueGet(__FUNCTION__,
		index,
		(int*)power,
		DEVICE_RADIO,
		"TransmitPower");
}


/**************************************************************************/
/*! \fn int wlan_getChannelsInUse(int radioIndex, char *channelsInUse)
**************************************************************************
*  \brief get Channel Is in use
*  \param[in] int RadioIndex - Radio index
*  \param[out] char *channelsInUse
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getChannelsInUse(int radioIndex, char *channelsInUse)
{
	return stringValueGet(__FUNCTION__,
		radioIndex,
		channelsInUse,
		DEVICE_RADIO,
		"ChannelsInUse");
}

/**************************************************************************/
/*! \fn int wlan_setBeaconInterval(int index, int bintval)
**************************************************************************
*  \brief Set Beacon Interval
*  \param[in] int index - Index Number
*  \param[in] int bintval - Beacon Interval
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setBeaconInterval(int index, int bintval)
{
	(void)index;    // unused parameter
	(void)bintval;  // unused parameter
	return UGW_SUCCESS;
}

int wlan_setRadioBeaconPeriod(int index, int bintval)
{
	return intValueSet(fapi_wlan_radio_set,
		index,
		bintval,
		DEVICE_RADIO,
		"BeaconPeriod");
}

int wlan_getRadioBeaconPeriod(int index, int *bintval)
{
	return intValueGet("fapi_wlan_radio_set",
		index,
		bintval,
		DEVICE_RADIO,
		"BeaconPeriod");
}

/**************************************************************************/
/*! \fn int wlan_scanApChannels(int radioIndex, char *scanData)
**************************************************************************
*  \brief scans the Access Point channels
*  \param[in] int radioIndex - Radio Index Number
*  \param[out] char *scanData  - scanned data
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_scanApChannels(int radioIndex, char *scanData)
{
	(void)radioIndex;  // unused parameter
	(void)scanData;    // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_pushDefaultValues(int index)
**************************************************************************
*  \brief push Default values to the hardware
*  \param[in] int index - Radio Index Number
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_pushDefaultValues(int index)
{
	(void)index;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_pushChannel(int athIndex, unsigned int channel)
**************************************************************************
*  \brief push channel No.
*  \param[in] int index - Radio Index Number
*  \param[in] unsigned int channel - channel No
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_pushChannel(int athIndex, unsigned int channel)
{
	(void)athIndex;  // unused parameter
	(void)channel;   // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_pushSSID(int index, char *ssid)
**************************************************************************
*  \brief push SSID to hardware
*  \param[in] int index - AP Index
*  \param[in] char *ssid - SSID
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_pushSSID(int index, char *ssid)
{
	(void)index;  // unused parameter
	(void)ssid;   // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_pushChannelMode(int athIndex)
**************************************************************************
*  \brief push channel mode to hardware
*  \param[in] int index - AP Index
*  \param[in] char *ssid - SSID
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_pushChannelMode(int athIndex)
{
	(void)athIndex;  // unused parameter
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_setRTSThresh(int index, unsigned int threshold)
**************************************************************************
*  \brief set RTS Threshold
*  \param[in] int index - AP Index
*  \param[in] unsigned int threshold - value of RTS threshold to be set
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setRTSThresh(int index, unsigned int threshold)
{
	(void)index;      // unused parameter
	(void)threshold;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setMacAddressControlMode(int index, int mode)
**************************************************************************
*  \brief set Mac Address Control Mode
*  \param[in] int index - AP Index
*  \param[in] int mode - Mode
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setMacAddressControlMode(int index, int mode)
{
	char macControlMode[MAX_LEN_PARAM_VALUE] = { '\0' };

	if (mode == 0)
	{
		strcpy(macControlMode, "Disabled");
	}
	else if (mode == 1) {
		strcpy(macControlMode, "Allow");
	}
	else if (mode == 2) {
		strcpy(macControlMode, "Deny");
	}
	else
	{
		return UGW_FAILURE;
	}

	return stringValueSet(fapi_wlan_ap_set,
		index,
		macControlMode,
		DEVICE_ACCESS_POINT_VENDOR,
		"MACAddressControlMode");
}


/**************************************************************************/
/*! \fn int wlan_getMacAddressControlMode(int index, int *mode)
**************************************************************************
*  \brief get Mac Address Control Mode
*  \param[in] int index - AP Index
*  \param[in] int *mode - Mode
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getMacAddressControlMode(int index, int *mode)
{
	char macControlMode[MAX_LEN_PARAM_VALUE] = { '\0' };

	stringValueGet(__FUNCTION__,
	index,
	macControlMode,
	DEVICE_ACCESS_POINT_VENDOR,
	"MACAddressControlMode");

	if (!strcmp(macControlMode,"Disabled"))
	{
		*mode = 0;
	}
	else if (!strcmp(macControlMode,"Allow")) {
		*mode = 1;
	}
	else if (!strcmp(macControlMode,"Deny")) {
		*mode = 2;
	}
	else
	{
		*mode = -1;
		return UGW_FAILURE;
	}
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_addAclDevice(int index, char *dev)
**************************************************************************
*  \brief add device to list of acl devices
*  \param[in] int index - AP Index
*  \param[in] char *dev - Dev Mac address
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_addAclDevice(int index, char *dev)
{
	char macList[MAX_LEN_PARAM_VALUE] = { '\0' };

	if (checkLegalMac(dev))
	{
		return UGW_FAILURE;
	}

	stringValueGet(__FUNCTION__,
		index,
		macList,
		DEVICE_ACCESS_POINT_VENDOR,
		"MACAddressControlList");

	if (strlen(macList) >= MAC_LENGTH)
	{
		strcat(macList, ",");
	}

	strncat(macList, dev, MAX_LEN_PARAM_VALUE);

	return stringValueSet(fapi_wlan_ap_set,
		index,
		macList,
		DEVICE_ACCESS_POINT_VENDOR,
		"MACAddressControlList");
}

int wlan_getApAclDevices(int index, char  ***macArray, unsigned int *buf_size)
{
	char macList[MAX_LEN_PARAM_VALUE] = { '\0' };
	char *s = macList;
	int i;

	stringValueGet(__FUNCTION__,
		index,
		macList,
		DEVICE_ACCESS_POINT_VENDOR,
		"MACAddressControlList");

	wlan_getAclDeviceNum(index, buf_size);
	if ((((int)*buf_size * MAC_LENGTH) + 1) > MAX_LEN_PARAM_VALUE)
		return UGW_FAILURE;

	*macArray = (char **)calloc(*buf_size, sizeof(char*));
	FAPI_WLAN_ASSERT(*macArray);

	for (i = 0; i < (int)*buf_size; i++)
	{
		(*macArray)[i] = calloc((MAC_LENGTH + 1), sizeof(char));
		strncpy((*macArray)[i], s, MAC_LENGTH);
		s = &macList[(i + 1)*MAC_LENGTH + 1];
	}

	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_delAclDevice(int index, char *dev)
**************************************************************************
*  \brief delete device from list of acl devices
*  \param[in] int index - AP Index
*  \param[in] char *dev - Dev Mac address
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_delAclDevice(int index, char *dev)
{
	char macList[MAX_LEN_PARAM_VALUE] = { '\0' };

	if (checkLegalMac(dev))
	{
		return UGW_FAILURE;
	}

	stringValueGet(__FUNCTION__,
		index,
		macList,
		DEVICE_ACCESS_POINT_VENDOR,
		"MACAddressControlList");

	strcat(dev, ",");
	if (strRemoveAll(macList, dev) == UGW_SUCCESS)
	{
		dev[strlen(dev) - 1] = '\0';
		if (strRemoveAll(macList, dev) == UGW_SUCCESS)
		{
			return UGW_FAILURE;
		}
	}

	return stringValueSet(fapi_wlan_ap_set,
		index,
		macList,
		DEVICE_ACCESS_POINT_VENDOR,
		"MACAddressControlList");

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_cancelWps(int index)
**************************************************************************
*  \brief Cancel WPS
*  \param[in] int index - AP Index
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_cancelWps(int index)
{
	return stringValueSetDbRadio(fapi_wlan_wps_set,
		index,
		"CancelWPS",
		DEVICE_RADIO_WPS_VENDOR,
		"WPSAction");
}

int wlan_setWpsEnrolleePin(int index, char *pin)
{
	int retVal = 0;

	retVal += stringValueSetDbRadio(fapi_wlan_wps_set,
		index,
		pin,
		DEVICE_RADIO_WPS_VENDOR,
		"EndpointPIN");

	retVal += stringValueSetDbRadio(fapi_wlan_wps_set,
		index,
		"PIN",
		DEVICE_RADIO_WPS_VENDOR,
		"WPSAction");

	return retVal;
}



/**************************************************************************/
/*! \fn int wlan_setTransmitPower(int index, u_int_32 power)
 **************************************************************************
 *  \brief get the current transmit power level. Indicates the current transmit power level
 *  as a percentage of full power. The value MUST be one of the values reported
 *  by the TransmitPowerSupported parameter.
 *  \param[in] int index - The index of the entry
 *  \param[in] u_int_32 power - The current transmit power level
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setTransmitPower(int index, u_int_32 power)
{
	return intValueSet(fapi_wlan_radio_set,
		index,
		power,
		DEVICE_RADIO,
		"TransmitPower");
}


/**************************************************************************/
/*! \fn int wlan_getAutoRateFallbackEnable(int index, bool *enable)
 **************************************************************************
 *  \brief get automatic rate fallback enable flag.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool *enable - Automatic rate fallback enable flag.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getAutoRateFallbackEnable(int index, bool *enable)
{
	(void)index;  // unused parameter
	*enable = true;  // Not exposed in our TR181 API today. Return enabled.

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setAutoRateFallbackEnable(int index, bool enable)
 **************************************************************************
 *  \brief set automatic rate fallback enable flag.
 *  \param[in] int index - The index of the entry
 *  \param[in] bool enable - Automatic rate fallback enable flag.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setAutoRateFallbackEnable(int index, bool enable)
{
	(void)index;   // unused parameter
	(void)enable;  // unused parameter
	return UGW_SUCCESS;
}

/* Output: a structure of WLAN logical network basic statistics. */
int
#if defined(WLAN_STANDALONE_TEST) || !defined YOCTO
wlan_getBasicStats(int index, void *stats)
#else
wlan_getBasicStats(int index, wlan_basicStats_t *stats)
#endif
{
	(void)index;  // unused parameter
	(void)stats;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getStats(int index, wlan_stats_t *stats)
 **************************************************************************
 *  \brief get WLAN logical network basic statistics.
 *  \param[in] int index - The index of the entry
 *  \param[out] wlan_stats_t *stats -  a structure of WLAN logical network statistics.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int
#if defined(WLAN_STANDALONE_TEST) || !defined YOCTO
wlan_getStats(int index, void *stats)
#else
wlan_getStats(int index, wlan_stats_t *stats)
#endif
{
	(void)index;  // unused parameter
	(void)stats;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getAssocDevicesNum(int index, u_int_32 *devNum)
 **************************************************************************
 *  \brief get number of WLAN associated devices.
 *  \param[in] int index - The index of the entry
 *  \param[out] u_int_32 *devNum -  number of associated devices.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getAssocDevicesNum(int index, u_int_32 *devNum)
{
	return intValueGet(__FUNCTION__,
		index,
		(int*)devNum,
		DEVICE_ACCESS_POINT,
		"AssociatedDeviceNumberOfEntries");
}


/**************************************************************************/
/*! \fn int wlan_getAssocDevice(int index, int devIndex, wlan_device_t *dev)
 **************************************************************************
 *  \brief get WLAN associated device.
 *  \param[in] int index - The index of the entry
 *  \param[in] int devIndex - The index of an associated device
 *  \param[out] wlan_device_t *dev -  a structure parameters belonging to an associated device
 *  held at index "devIndex" in AP tables.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int
#if defined(WLAN_STANDALONE_TEST) || !defined YOCTO
wlan_getAssocDevice(int index, int devIndex, void *dev)
#else
wlan_getAssocDevice(int index, int devIndex, wlan_device_t *dev)
#endif
{
	(void)index;     // unused parameter
	(void)devIndex;  // unused parameter
	(void)dev;       // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWepKey(int index, int keyIndex, char *key)
 **************************************************************************
 *  \brief get WEP key
 *  \param[in] int index - The WLAN config index
 *  \param[in] int keyIndex - Key index (1:4)
 *  \param[out] char *key -  WEP key.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWepKey(int index, int keyIndex, char *key)
{
	(void)keyIndex;  // unused parameter
	return stringValueGet(__FUNCTION__,
		index,
		key,
		DEVICE_ACCESS_POINT_SECURITY,
		"WEPKey");
}


/**************************************************************************/
/*! \fn int wlan_setWepKey(int index, int keyIndex, char *key)
 **************************************************************************
 *  \brief set WEP key
 *  \param[in] int index - The WLAN config index
 *  \param[in] int keyIndex - Key index (1:4)
 *  \param[in] char *key -  WEP key, maximum 128 characters.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWepKey(int index, int keyIndex, char *key)
{
	(void)keyIndex;  // unused parameter
	return stringValueSet(fapi_wlan_security_set,
		index,
		key,
		DEVICE_ACCESS_POINT_SECURITY,
		"WEPKey");
}

// TODO THERE IS A BUG IN THE API FOR TR98 CUSTOMERS DELETE AFTER CLEANUP
int wlan_getPreSharedKey(int index, int keyIndex, char *psk)
{
	(void)keyIndex;  // unused parameter
	return stringValueGet(__FUNCTION__,
		index,
		psk,
		DEVICE_ACCESS_POINT_SECURITY,
		"PreSharedKey");
}


/**************************************************************************/
/*! \fn int wlan_setPreSharedKey(int index, int keyIndex, char *psk)
 **************************************************************************
 *  \brief set Pre-shared key
 *  \param[in] int index - The WLAN config index
 *  \param[in] int keyIndex - Key index (1:4)
 *  \param[in] char *psk -  Pre-shared key represented as a hexadecimal string.
 *  Max 64 characters
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setPreSharedKey(int index, int keyIndex, char *psk)
{
	/* Ignore keyIndex - looks like a param for WEP key.
	Looks like there is a switch with wlan_setWepKeyPassphrase()...
	*/

	(void)keyIndex;  // unused parameter

	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(wlObj);
	help_addObjList(wlObj, DEVICE_ACCESS_POINT_SECURITY, 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, DEVICE_ACCESS_POINT_SECURITY, "PreSharedKey", psk, 0, 0);
	HELP_EDIT_NODE(wlObj, DEVICE_ACCESS_POINT_SECURITY, "KeyPassphrase", "", 0, 0);

	if (fapi_wlan_generic_set(fapi_wlan_security_set, index, wlObj, 0) == UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_SUCCESS;
	}
	else {
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}
}


/**************************************************************************/
/*! \fn int wlan_getKeyPassphrase(int index, int keyIndex, char *psk)
 **************************************************************************
 *  \brief get Key passphrase
 *  \param[in] int index - The WLAN config index
 *  \param[in] int keyIndex - Key index (1:4)
 *  \param[out] char *psk -  Key passphrase string.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getKeyPassphrase(int index, int keyIndex, char *psk)
{
	(void)keyIndex;  // unused parameter
	return stringValueGet(__FUNCTION__,
		index,
		psk,
		DEVICE_ACCESS_POINT_SECURITY,
		"KeyPassphrase");
}


/**************************************************************************/
/*! \fn int wlan_setKeyPassphrase(int index, int keyIndex, char *psk)
 **************************************************************************
 *  \brief set Key passphrase. Notes that setting both the Key passphrase and the Pre-Shared Key
 *  directly is not allowed and the result is undefined. Either one of the two techniques must be used.
 *  \param[in] int index - The WLAN config index
 *  \param[in] int keyIndex - Key index (1:4)
 *  \param[in] char *psk -  Key passphrase string. Max 63 characters.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setKeyPassphrase(int index, int keyIndex, char *psk)
{
	/* Ignore keyIndex - looks like a param for WEP key.
	Looks like there is a switch with wlan_setWepKeyPassphrase()...
	*/

	(void)keyIndex;  // unused parameter

	ObjList *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	FAPI_WLAN_ASSERT(wlObj);
	help_addObjList(wlObj, DEVICE_ACCESS_POINT_SECURITY, 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObj, DEVICE_ACCESS_POINT_SECURITY, "PreSharedKey", "", 0, 0);
	HELP_EDIT_NODE(wlObj, DEVICE_ACCESS_POINT_SECURITY, "KeyPassphrase", psk, 0, 0);

	if (fapi_wlan_generic_set(fapi_wlan_security_set, index, wlObj, 0) == UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_SUCCESS;
	}
	else {
		HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}
}


/**************************************************************************/
/*! \fn int wlan_getWmmSupported(int index, bool *supported)
 **************************************************************************
 *  \brief get WMM supported flag.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool *supported - True if WMM is supported
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmSupported(int index, bool *supported)
{
	return boolValueGet(__FUNCTION__,
		index,
		supported,
		DEVICE_ACCESS_POINT,
		"WMMCapability");
}


/**************************************************************************/
/*! \fn int wlan_getWmmUaspdSupported(int index, bool *supported)
 **************************************************************************
 *  \brief get WMM Unscheduled Automatic Power Save Delivery supported flag.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool *supported - True if WMM Unscheduled Automatic Power Save Delivery is supported
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmUaspdSupported(int index, bool *supported)
{
	return boolValueGet(__FUNCTION__,
		index,
		supported,
		DEVICE_ACCESS_POINT,
		"UAPSDCapability");
}


/**************************************************************************/
/*! \fn int wlan_getWmmEnable(int index, bool *enable)
 **************************************************************************
 *  \brief get WMM enable flag.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool *enable - True if WMM is enabled
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmEnable(int index, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_ACCESS_POINT,
		"WMMEnable");
}


/**************************************************************************/
/*! \fn int wlan_setWmmEnable(int index, bool enable)
 **************************************************************************
 *  \brief set WMM enable flag.
 *  \param[in] int index - The index of the entry
 *  \param[in] bool enable - WMM enable flag. This is valid only if WMM is supported.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmEnable(int index, bool enable)
{
	(void)index;  // unused parameters
	if (enable)
	{
		return UGW_SUCCESS;
	}
	else
	{
		return UGW_FAILURE;
	}
}


/**************************************************************************/
/*! \fn int wlan_getWmmUaspdEnable(int index, bool *enable)
 **************************************************************************
 *  \brief get WMM Unscheduled Automatic Power Save Delivery flag.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool *enable - True if WMM Unscheduled Automatic Power Save Delivery flag enabled
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmUaspdEnable(int index, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_ACCESS_POINT,
		"UAPSDEnable");
}


/**************************************************************************/
/*! \fn int wlan_setWmmUaspdEnable(int index, bool enable)
 **************************************************************************
 *  \brief set WMM Unscheduled Automatic Power Save Delivery flag.
 *  \param[in] int index - The index of the entry
 *  \param[in] bool enable - WMM Automatic Power Save Delivery flag. This is valid only
 *  if WMM and UAPSD are supported.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmUaspdEnable(int index, bool enable)
{
	return boolValueSet(fapi_wlan_ap_set,
		index,
		enable,
		DEVICE_ACCESS_POINT,
		"UAPSDEnable");
}


/**************************************************************************/
/*! \fn int wlan_getWmmOgAifsn(int index, int class, u_int_32 *aifsn)
 **************************************************************************
 *  \brief get Arbitration Inter Frame Spacing of Outgoing Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[out] u_int_32 *aifsn -  Arbitration Inter Frame Spacing (Number)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmOgAifsn(int index, int cl, u_int_32 *aifsn)
{
	char acCategory[MAX_LEN_PARAM_VALUE];
	sprintf(acCategory, "%s.%d", DEVICE_ACCESS_POINT_AC, cl);

	return intValueGet(__FUNCTION__,
		index,
		(int*)aifsn,
		acCategory,
		"AIFSN");
}


/**************************************************************************/
/*! \fn int wlan_setWmmOgAifsn(int index, int class, u_int_32 aifsn)
 **************************************************************************
 *  \brief set Arbitration Inter Frame Spacing of Outgoing Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[in] u_int_32 aifsn -  Arbitration Inter Frame Spacing (Number), (2-15)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmOgAifsn(int index, int cl, u_int_32 aifsn)
{
	char acCategory[MAX_LEN_PARAM_VALUE];
	sprintf(acCategory, "%s.%d", DEVICE_ACCESS_POINT_AC, cl);

	return intValueSet(fapi_wlan_wmm_ap_set,
		index,
		aifsn,
		acCategory,
		"AIFSN");
}


/**************************************************************************/
/*! \fn int wlan_getWmmOgEcwMin(int index, int class, u_int_32 *ecwMin)
 **************************************************************************
 *  \brief get Exponent of Contention Window (Minimum) of Outgoing Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[out] u_int_32 *ecwMin -  Exponent of Contention Window (Minimum)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmOgEcwMin(int index, int cl, u_int_32 *ecwMin)
{
	char acCategory[MAX_LEN_PARAM_VALUE];
	sprintf(acCategory, "%s.%d", DEVICE_ACCESS_POINT_AC, cl);

	return intValueGet(__FUNCTION__,
		index,
		(int*)ecwMin,
		acCategory,
		"ECWMin");
}


/**************************************************************************/
/*! \fn int wlan_setWmmOgEcwMin(int index, int class, u_int_32 ecwMin)
 **************************************************************************
 *  \brief set Exponent of Contention Window (Minimum) of Outgoing Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[in] u_int_32 ecwMin -  Exponent of Contention Window (Minimum), (0-15)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmOgEcwMin(int index, int cl, u_int_32 ecwMin)
{
	char acCategory[MAX_LEN_PARAM_VALUE];
	sprintf(acCategory, "%s.%d", DEVICE_ACCESS_POINT_AC, cl);

	return intValueSet(fapi_wlan_wmm_ap_set,
		index,
		ecwMin,
		acCategory,
		"ECWMin");
}


/**************************************************************************/
/*! \fn int wlan_getWmmOgEcwMax(int index, int class, u_int_32 *ecwMax)
 **************************************************************************
 *  \brief get Exponent of Contention Window (Maximum) of Outgoing Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[out] u_int_32 *ecwMax - Exponent of Contention Window (Maximum)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmOgEcwMax(int index, int cl, u_int_32 *ecwMax)
{
	char acCategory[MAX_LEN_PARAM_VALUE];
	sprintf(acCategory, "%s.%d", DEVICE_ACCESS_POINT_AC, cl);

	return intValueGet(__FUNCTION__,
		index,
		(int*)ecwMax,
		acCategory,
		"ECWMax");
}


/**************************************************************************/
/*! \fn int wlan_setWmmOgEcwMax(int index, int class, u_int_32 ecwMax)
 **************************************************************************
 *  \brief set Exponent of Contention Window (Maximum) of Outgoing Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[in] u_int_32 ecwMax -  Exponent of Contention Window (Maximum), (0-15). Must be >= EcwMin.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmOgEcwMax(int index, int cl, u_int_32 ecwMax)
{
	char acCategory[MAX_LEN_PARAM_VALUE];
	sprintf(acCategory, "%s.%d", DEVICE_ACCESS_POINT_AC, cl);

	return intValueSet(fapi_wlan_wmm_ap_set,
		index,
		ecwMax,
		acCategory,
		"ECWMax");
}


/**************************************************************************/
/*! \fn int wlan_getWmmOgTxOp(int index, int class, u_int_32 *txOp)
 **************************************************************************
 *  \brief get Transmit Opportunity of Outgoing Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[out] u_int_32 *txOp - Transmit Opportunity, in multiples of 32 microseconds
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmOgTxOp(int index, int cl, u_int_32 *txOp)
{
	char acCategory[MAX_LEN_PARAM_VALUE];
	sprintf(acCategory, "%s.%d", DEVICE_ACCESS_POINT_AC, cl);

	return intValueGet(__FUNCTION__,
		index,
		(int*)txOp,
		acCategory,
		"TxOpMax");
}


/**************************************************************************/
/*! \fn int wlan_setWmmOgTxOp(int index, int class, u_int_32 txOp)
 **************************************************************************
 *  \brief set Transmit Opportunity of Outgoing Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[in] u_int_32 txOp -  Transmit Opportunity, in multiples of 32 microseconds
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmOgTxOp(int index, int cl, u_int_32 txOp)
{
	char acCategory[MAX_LEN_PARAM_VALUE];
	sprintf(acCategory, "%s.%d", DEVICE_ACCESS_POINT_AC, cl);

	return intValueSet(fapi_wlan_wmm_ap_set,
		index,
		txOp,
		acCategory,
		"TxOpMax");
}


/**************************************************************************/
/*! \fn int wlan_getWmmOgAckPolicy(int index, int class, bool *policy)
 **************************************************************************
 *  \brief get Ack Policy of Outgoing Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[out] bool *policy -  Ack Policy, where False=Do Not Acknowledge and True=Acknowledge
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmOgAckPolicy(int index, int cl, bool *policy)
{
	(void)index;   // unused parameters
	(void)cl;      // unused parameters
	(void)policy;  // unused parameters
	return UGW_FAILURE;
}


/**************************************************************************/
/*! \fn int wlan_setWmmOgAckPolicy(int index, int class, bool policy)
 **************************************************************************
 *  \brief set Ack Policy of Outgoing Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[in] bool policy -  Ack Policy, where False=Do Not Acknowledge and True=Acknowledge
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmOgAckPolicy(int index, int cl, bool policy)
{
	(void)index;   // unused parameters
	(void)cl;      // unused parameters
	(void)policy;  // unused parameters
	return UGW_FAILURE;
}


/**************************************************************************/
/*! \fn int wlan_getWmmIcAifsn(int index, int class, u_int_32 *aifsn)
 **************************************************************************
 *  \brief get Arbitration Inter Frame Spacing of Incoming Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[out] u_int_32 *aifsn -  Arbitration Inter Frame Spacing (Number)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmIcAifsn(int index, int cl, u_int_32 *aifsn)
{
	(void)index;  // unused parameters
	(void)cl;     // unused parameters
	(void)aifsn;  // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setWmmIcAifsn(int index, int class, u_int_32 aifsn)
 **************************************************************************
 *  \brief set Arbitration Inter Frame Spacing of Incoming Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[in] u_int_32 aifsn -  Arbitration Inter Frame Spacing (Number), (2-15)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmIcAifsn(int index, int cl, u_int_32 aifsn)
{
	(void)index;  // unused parameters
	(void)cl;     // unused parameters
	(void)aifsn;  // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWmmIcEcwMin(int index, int class, u_int_32 *ecwMin)
 **************************************************************************
 *  \brief get Exponent of Contention Window (Minimum) of Incoming Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[out] u_int_32 *ecwMin -  Exponent of Contention Window (Minimum)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmIcEcwMin(int index, int cl, u_int_32 *ecwMin)
{
	(void)index;   // unused parameters
	(void)cl;      // unused parameters
	(void)ecwMin;  // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setWmmIcEcwMin(int index, int class, u_int_32 ecwMin)
 **************************************************************************
 *  \brief set Exponent of Contention Window (Minimum) of Incoming Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[in] u_int_32 ecwMin -  Exponent of Contention Window (Minimum), (0-15)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmIcEcwMin(int index, int cl, u_int_32 ecwMin)
{
	(void)index;   // unused parameters
	(void)cl;      // unused parameters
	(void)ecwMin;  // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWmmIcEcwMax(int index, int class, u_int_32 *ecwMax)
 **************************************************************************
 *  \brief get Exponent of Contention Window (Maximum) of Incoming Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[out] u_int_32 *ecwMax -  Exponent of Contention Window (Maximum)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmIcEcwMax(int index, int cl, u_int_32 *ecwMax)
{
	(void)index;   // unused parameters
	(void)cl;      // unused parameters
	(void)ecwMax;  // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setWmmIcEcwMax(int index, int class, u_int_32 ecwMax)
 **************************************************************************
 *  \brief set Exponent of Contention Window (Maximum) of Incoming Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[in] u_int_32 ecwMax - Exponent of Contention Window (Maximum), (0-15). Must be >= EcwMin.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmIcEcwMax(int index, int cl, u_int_32 ecwMax)
{
	(void)index;   // unused parameters
	(void)cl;      // unused parameters
	(void)ecwMax;  // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWmmIcTxOp(int index, int class, u_int_32 *txOp)
 **************************************************************************
 *  \brief get Transmit Opportunity of Incommoding Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[out] u_int_32 *txOp -  Transmit Opportunity, in multiples of 32 microseconds
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmIcTxOp(int index, int cl, u_int_32 *txOp)
{
	(void)index;  // unused parameters
	(void)cl;     // unused parameters
	(void)txOp;   // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setWmmIcTxOp(int index, int class, u_int_32 txOp)
 **************************************************************************
 *  \brief set Arbitration Inter Frame Spacing of Incommoding Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[in] u_int_32 txOp - Transmit Opportunity, in multiples of 32 microseconds
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmIcTxOp(int index, int cl, u_int_32 txOp)
{
	(void)index;  // unused parameters
	(void)cl;     // unused parameters
	(void)txOp;   // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWmmIcAckPolicy(int index, int class, bool *policy)
 **************************************************************************
 *  \brief get Ack Policy of Incommoding Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[out] bool *policy - Ack Policy, where False=Do Not Acknowledge and True=Acknowledge
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWmmIcAckPolicy(int index, int cl, bool *policy)
{
	(void)index;   // unused parameters
	(void)cl;      // unused parameters
	(void)policy;  // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setWmmIcAckPolicy(int index, int class, bool policy)
 **************************************************************************
 *  \brief set Ack Policy of Incommoding Traffic.
 *  \param[in] int index - The index of the entry
 *  \param[in] int class - Priority class level (1-Best Effort, 2-Background, 3-Video, 4-Voice)
 *  \param[in] bool policy - Ack Policy, where False=Do Not Acknowledge and True=Acknowledge
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWmmIcAckPolicy(int index, int cl, bool policy)
{
	(void)index;   // unused parameters
	(void)cl;      // unused parameters
	(void)policy;  // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpsEnable(int index, bool *enable)
 **************************************************************************
 *  \brief get WPS enable flag.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool *enable - True if WPS enabled
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsEnable(int index, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_ACCESS_POINT_WPS,
		"Enable");
}


/**************************************************************************/
/*! \fn int wlan_setWpsEnable(int index, bool enable)
 **************************************************************************
 *  \brief set WPS enable flag.
 *  \param[in] int index - The index of the entry
 *  \param[in] bool enable - True if WPS enabled
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWpsEnable(int index, bool enable)
{
	ObjList *tempLoopObj = NULL;
	ParamList *param = NULL;
	char strVapIndex[MAX_LEN_PARAM_VALUE] = { '\0' };
	int foundVap = 0;
	bool enableVap, bWpsDualBandsOut;
	int activeWpsRadio = 0, activeWpsRadioCurrent = 0, nWpsIf = 0;
	int ifIndex[WLAN_MAX_NUM_VAP];
	char ifNames[WLAN_MAX_NUM_VAP][MAX_LEN_PARAM_VALUE];
	char sWpsDualBandsOut[MAX_LEN_PARAM_VALUE];
	char *currentVapIndex, *currentIfName, *currentRadioName;
	char sWpsDualBandsDb[MAX_LEN_PARAM_VALUE], sSsidIfNameOnScript[MAX_LEN_PARAM_VALUE];
	int tmpRadioIndex;
	char strTmpRadioIndex[WLAN_MAX_NUM_VAP];
	int i, paramVal, instance;
	ObjList *wlObjRPC;
	char strIndex[MAX_LEN_PARAM_VALUE];
	char strRadioIndex[MAX_LEN_PARAM_VALUE];
	int radioIndex=-1, vapIndex=-1, testWps = 0;

	//printf("Arad: COMMON: wlan_setWpsEnable: index=%d, enable=%d\n", index, (int)enable);

	if (wlan_getRadioIndex(index, &radioIndex) != UGW_SUCCESS) {
		printf("ERROR: wlan_getRadioIndex failed on index %d\n",index);
		return UGW_FAILURE;
	}
	//printf("Arad: COMMON: wlan_setWpsEnable: wlan_getRadioIndex returns radioIndex %d\n", radioIndex);
	
	sprintf(strRadioIndex, "%s%d", "wlan", radioIndex); //wlan0/wlan1 only (also wlan2 for 3 radios)
	sprintf(strIndex, "%s%d", "wlan", index); //wlan0/wlan1/wlan2/wlan3 ...etc.
	//printf("Arad: COMMON: wlan_setWpsEnable: strRadioIndex %s, strIndex %s\n", strRadioIndex, strIndex);

	/////////////////////////////////////////////////////////////////////////
	//If enable=true: disable WPS on any other interfaces of this radio, then server the command
	//- get index of radio
	//- loop on mapper
	// -- for each vap of this radio:
	//  --- if wps == true
	//   ---- change to false
	//   ---- call fapi_wlan_wps_set
	//   ---- break loop
	
	if (enable == true) {
		//printf("Arad: COMMON: wlan_setWpsEnable: strIndex=%s, strRadioIndex=%s\n", strIndex, strRadioIndex);
		
		////////////////////////////////////////////////////////
		//Loop over mapper, for each VAP of the radio, test if WPS is enable

		fapiWlanFailSafeLoad();

		FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
		{	
			FOR_EACH_PARAM(tempLoopObj, param)
			{
				//printf("Arad: COMMON: wlan_setWpsEnable: FOR_EACH_PARAM: param = %s\n", GET_PARAM_NAME(param));
				if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
				{
					//printf("Arad: COMMON: wlan_setWpsEnable: compare vap radio to %s\n",strRadioIndex);
					if (!strcmp(strRadioIndex/*wlan1*/, GET_PARAM_VALUE(param)))
					{
						//This vap belongs to the radio
						//printf("Arad: COMMON: wlan_setWpsEnable: This vap belongs to radio %s\n", strRadioIndex);
						foundVap = 1;
					}
				}

				if (!strcmp(DB_RPC_INDEX, GET_PARAM_NAME(param)))
				{
					strncpy(strVapIndex, GET_PARAM_VALUE(param), MAX_LEN_PARAM_VALUE);
					//printf("Arad: COMMON: wlan_setWpsEnable: found DB_RPC_INDEX, copy ifName (%s) to strVapIndex\n", strVapIndex);
				}
			}
			//printf("Arad: COMMON: wlan_setWpsEnable: foundVap=%d,  strIndex = %s, strVapIndex = %s\n", foundVap, strIndex, strVapIndex);
			if ((foundVap == 1) && (strcmp(strIndex, strVapIndex)))
			{
				////////////////////////////////////////////////////
				// Same Radio (foundVap=1) and differemt VAP index (compare DB_RPC_INDEX in mapping and API index)

				//printf("Arad: COMMON: wlan_setWpsEnable: foundVap of radio that is different than API input\n");
				if (/*wlan_getIndexFromName*/wlan_getIndexFromInterface(strVapIndex, &vapIndex) == UGW_FAILURE) {
					printf("ERROR: wlan_getIndexFromInterface return error with strVapIndex = %s\n",strVapIndex);
					return UGW_FAILURE;
				}
				//printf("Arad: COMMON: wlan_setWpsEnable: get index %d from %s\n", vapIndex, strVapIndex);
				boolValueGet(__FUNCTION__,
					vapIndex,
					&enableVap,
					DEVICE_ACCESS_POINT_WPS,
					"Enable");
					//printf("Arad: COMMON: wlan_setWpsEnable: enableVap=%d\n", (int)enableVap);
				if (enableVap == true)
				{
					//printf("Arad: COMMON: wlan_setWpsEnable: call fapi_wlan_wps_set with Enable=false for vapIndex %d \n",vapIndex);
					if (boolValueSet(fapi_wlan_wps_set,
						vapIndex,
						0/*false*/,
						DEVICE_ACCESS_POINT_WPS,
						"Enable") != UGW_SUCCESS)
						{
							//Roll back dB setting??? or first update scripts?
							printf("ERROR: boolValueSet failed on DEVICE_ACCESS_POINT_WPS, param=Enable, vapIndex %d\n",vapIndex);
							return UGW_FAILURE;
						}
				}
			}
			else
			{
				//Vap not belong to radio or same vap as need to enable. Clear strVapIndex:
				memset(strVapIndex, 0, MAX_LEN_PARAM_VALUE);
			}
			foundVap = 0;
		}
	}

	////////////////////////////////////////
	// Handle WPS RF Bands:
	// Define WPS RF Bands value from cuurent WPS status.
	// If different that new value: configure dB and FAPI (Scripts).

	//printf("Arad: COMMON: Handle WPS RF Bands\n");
	
	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		currentVapIndex = NULL;
		currentIfName = NULL;
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RPC_INDEX, GET_PARAM_NAME(param)))
			{
				currentVapIndex = GET_PARAM_VALUE(param);
			}

			if (!strcmp(DB_IFNAME, GET_PARAM_NAME(param)))
			{
				currentIfName = GET_PARAM_VALUE(param);
			}

			if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
			{
				currentRadioName = GET_PARAM_VALUE(param);
			}
		}
		
		// Get if WPS is enabled
		// Getting dB configuration doesn't include current session.
		
		FAPI_WLAN_ASSERT(currentVapIndex);
		if (wlan_getIndexFromInterface(/*strVapIndex*/currentVapIndex, &vapIndex) == UGW_FAILURE) {
			printf("ERROR: wlan_getIndexFromInterface return error with currentVapIndex = %s\n",currentVapIndex);
			return UGW_FAILURE;
		}
		if (boolValueGet(__FUNCTION__, vapIndex, &enableVap, DEVICE_ACCESS_POINT_WPS, "Enable") == UGW_FAILURE)
		{
			printf("ERROR: boolValueGet failed on DEVICE_ACCESS_POINT_WPS for param=Enable, vapIndex %d\n",vapIndex);
			return UGW_FAILURE;
		}
		if (enableVap == true) {
			//////////////////////////////////////////
			// Prepare parameters:
			// Example - WPS is enabled for wlan0.0:
			// 	activeWpsRadio = 0x02
			// 	ifNames[] = 0, wlan0.0, 0
			// 	ifIndex[] = 2, 0, 0
			//////////////////////////////////////////
			wlan_getRadioIndex(vapIndex, &tmpRadioIndex);
			activeWpsRadio = activeWpsRadio | (1 << (tmpRadioIndex)); //Prepare bit mask for active WPS radios
			strncpy(ifNames[tmpRadioIndex], currentVapIndex, MAX_LEN_PARAM_VALUE); // Save VAP name
			ifIndex[tmpRadioIndex] = vapIndex;
			nWpsIf++;
		}
	}

	if (enable == true) {
		activeWpsRadioCurrent = 1 << radioIndex;
		// Delete radio of the current command VAP if exist
		if ((activeWpsRadioCurrent & activeWpsRadio) == activeWpsRadioCurrent) {
			activeWpsRadioCurrent = ~activeWpsRadioCurrent;
			activeWpsRadio = activeWpsRadio & activeWpsRadioCurrent;
		}
		if (activeWpsRadio != WPS_ENABLE_BANDS_NONE) {
			// WPS is enabled in other interface
			strncpy(sWpsDualBandsOut, "true", MAX_LEN_PARAM_VALUE);
		}
		else {
			// WPS is not enabled in other interface
			strncpy(sWpsDualBandsOut, "false", MAX_LEN_PARAM_VALUE);
		}
	}
	else {
		if (activeWpsRadio == WPS_ENABLE_BANDS_WLAN0_WLAN1_WLAN2) {
			strncpy(sWpsDualBandsOut, "true", MAX_LEN_PARAM_VALUE);
		}
		else {
			strncpy(sWpsDualBandsOut, "false", MAX_LEN_PARAM_VALUE);
		}
	}
	
	// Get current config of WPS RF Bands:
	if (boolValueGet(__FUNCTION__, radioIndex, &bWpsDualBandsOut, DEVICE_RADIO_WPS_VENDOR, "WpsDualBands") == UGW_FAILURE)
	{
		printf("ERROR: boolValueGet failed on DEVICE_RADIO_WPS_VENDOR for param=WpsDualBands, radioIndex %d\n",radioIndex);
		return UGW_FAILURE;
	}
	if (bWpsDualBandsOut == 0)
	{
		strncpy(sWpsDualBandsDb, "false", MAX_LEN_PARAM_VALUE);
	}
	else
	{
		strncpy(sWpsDualBandsDb, "true", MAX_LEN_PARAM_VALUE);
	}

	if (strncmp(sWpsDualBandsDb, sWpsDualBandsOut, MAX_LEN_PARAM_VALUE)) {
		
		/////////////////////////////////////////////////////////
		// Different, update about configuration change
		/////////////////////////////////////////////////////////

		////////////////////////////////////////////////////
		// Update current interface scripts and dB:
		
		paramVal = strncmp(sWpsDualBandsOut, "false", MAX_LEN_PARAM_VALUE);

		if (boolValueSet(fapi_wlan_wps_set, radioIndex, paramVal, DEVICE_RADIO_WPS_VENDOR, "WpsDualBands") != UGW_SUCCESS)
		{
			printf("ERROR: boolValueSet failed on DEVICE_RADIO_WPS_VENDOR for param=WpsDualBands, radioIndex %d\n",radioIndex);
			return UGW_FAILURE;
		}

		wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
		if (wlObjRPC == NULL)
		{
			printf("ERROR: HELP_CREATE_OBJ failed \n");
			return UGW_FAILURE;
		}
		if (wlanLoadFromDB(DEVICE_RADIO_WPS_VENDOR, strRadioIndex, wlObjRPC) == UGW_FAILURE)
		{
			HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_FAILURE;
		}
		HELP_EDIT_NODE(wlObjRPC, DEVICE_RADIO_WPS_VENDOR, "WpsDualBands", sWpsDualBandsOut, 0, 0);
		FAPI_WLAN_ASSERT(wlanStoreToDB(strRadioIndex, wlObjRPC) == UGW_SUCCESS);
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

		if (nWpsIf > 0)
		{
			////////////////////////////////////////////////////
			// Update other interfaces scripts and dB:
			// Loop over all radio, serve only if WPS is enabled:
			
			for (i = 0; i < 3; i++) {
				
				// Serve interface only if found instance:
				instance = -1;
				
				if ((i == 0) && (activeWpsRadio & WPS_ENABLE_BANDS_WLAN0)) { //First loop handles wlan0
					strncpy(sSsidIfNameOnScript, "wlan0", MAX_LEN_PARAM_VALUE);
					instance = ifIndex[0];
				}
				if ((i == 1) && (activeWpsRadio & WPS_ENABLE_BANDS_WLAN1)) {
					strncpy(sSsidIfNameOnScript, "wlan1", MAX_LEN_PARAM_VALUE);
					instance = ifIndex[1];
				}
				if ((i == 2) && (activeWpsRadio & WPS_ENABLE_BANDS_WLAN2)) {
					strncpy(sSsidIfNameOnScript, "wlan2", MAX_LEN_PARAM_VALUE);
					instance = ifIndex[2];
				}
				if (instance != -1)
				{
					// Get the radio instance from vap instance:
					wlan_getRadioIndex(instance, &tmpRadioIndex);
					sprintf(/*ifNameTmpRadio*/strTmpRadioIndex, "%s%d", "wlan", tmpRadioIndex); //wlan0/wlan1/wlan2
				
					if (boolValueSet(fapi_wlan_wps_set, tmpRadioIndex, paramVal, DEVICE_RADIO_WPS_VENDOR, "WpsDualBands") != UGW_SUCCESS)
					{
						printf("ERROR: boolValueSet failed on DEVICE_RADIO_WPS_VENDOR for param=WpsDualBands, tmpRadioIndex %d\n",tmpRadioIndex);
						return UGW_FAILURE;
					}
					wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
					if (wlObjRPC == NULL)
					{
						printf("ERROR: HELP_CREATE_OBJ failed \n");
						return UGW_FAILURE;
					}
					if (wlanLoadFromDB(DEVICE_RADIO_WPS_VENDOR, strTmpRadioIndex, wlObjRPC) == UGW_FAILURE)
					{
						HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
						return UGW_FAILURE;
					}
					HELP_EDIT_NODE(wlObjRPC, DEVICE_RADIO_WPS_VENDOR, "WpsDualBands", sWpsDualBandsOut, 0, 0);
					FAPI_WLAN_ASSERT(wlanStoreToDB(strTmpRadioIndex, wlObjRPC) == UGW_SUCCESS);
					HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
				}
			}
		}
	}
	
	// Serve the API:
	return boolValueSet(fapi_wlan_wps_set,
		index,
		enable,
		DEVICE_ACCESS_POINT_WPS,
		"Enable");
}


/**************************************************************************/
/*! \fn int wlan_getWpsDeviceName(int index, char *devName)
 **************************************************************************
 *  \brief get WPS device name.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *devName - User-friendly description of the device
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsDeviceName(int index, char *devName)
{
	int radioIndex=-1;
	
	wlan_getRadioIndex(index, &radioIndex);
	
	return stringValueGet(__FUNCTION__,
		radioIndex,
		devName,
		DEVICE_RADIO_WPS_VENDOR,
		"ModelName");
}


/**************************************************************************/
/*! \fn int wlan_getWpsDevicePassword(int index, u_int_32 *password)
 **************************************************************************
 *  \brief get WPS Device Password (PIN).
 *  \param[in] int index - The index of the entry
 *  \param[out] u_int_32 *password - device password (PIN)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsDevicePassword(int index, u_int_32 *password)
{
	int radioIndex=-1;
	
	wlan_getRadioIndex(index, &radioIndex);
	
	return intValueGet(__FUNCTION__,
		radioIndex,
		(int*)password,
		DEVICE_RADIO_WPS_VENDOR,
		"PIN");
}


/**************************************************************************/
/*! \fn int wlan_setWpsDevicePassword(int index, u_int_32 password)
 **************************************************************************
 *  \brief get WPS Device Password (PIN).
 *  \param[in] int index - The index of the entry
 *  \param[in] u_int_32 password - device password (PIN)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWpsDevicePassword(int index, u_int_32 password)
{
	(void)index;     // unused parameters
	(void)password;  // unused parameters
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpsUuid(int index, char *uuid)
 **************************************************************************
 *  \brief get Device UUID. UUID represented as specified in RFC 4122 [48],
 *  but omitting the leading urn:uuid:, e.g. f81d4fae-7dec-11d0-a765-00a0c91e6bf6.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *uuid - Device UUID
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsUuid(int index, char *uuid)
{
	int radioIndex=-1;
	
	wlan_getRadioIndex(index, &radioIndex);

	return stringValueGet(__FUNCTION__,
		radioIndex,
		uuid,
		DEVICE_RADIO_WPS_VENDOR,
		"UUID");
}


/**************************************************************************/
/*! \fn int wlan_getWpsVersion(int index, u_int_32 *ver)
 **************************************************************************
 *  \brief get the Wi-Fi Protected Setup version supported by the device.
 *  \param[in] int index - The index of the entry
 *  \param[out] u_int_32 *ver - The Wi-Fi Protected Setup version
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsVersion(int index, u_int_32 *ver)
{
	(void)index;  // unused parameters
	*ver = 2;  // we support WPS2.0

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpsConfigMethodsSupported(int index, char *methods)
 **************************************************************************
 *  \brief get WPS configuration methods supported by the device as comma-separated list
 *  of the WPS configuration methods supported by the device. Each entry in the list is an enumeration of:
 *  "USBFlashDrive", "Ethernet", "Label ", "Display", "ExternalNFCToken", "IntegratedNFCToken",
 *  "NFCInterface", "PushButton", "Keypad"
 *  \param[in] int index - The index of the entry
 *  \param[out] char *methods - The WPS configuration methods supported by the device.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsConfigMethodsSupported(int index, char *methods)
{
	int radioIndex=-1;
	
	wlan_getRadioIndex(index, &radioIndex);
	
	return stringValueGet(__FUNCTION__,
		radioIndex,
		methods,
		DEVICE_ACCESS_POINT_WPS,
		"ConfigMethodsSupported");
}

int wlan_getPassphrase(int index, char *methods)
{
	return stringValueGet(__FUNCTION__,
		index,
		methods,
		DEVICE_ACCESS_POINT_SECURITY,
		"KeyPassphrase");
}

/**************************************************************************/
/*! \fn int wlan_getWpsConfigMethodsEnabled(int index, char *methods)
 **************************************************************************
 *  \brief get WPS enabled configuration methods as comma-separated list of
 *  the WPS configuration methods currently enabled by the device.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *methods - The WPS enabled configuration methods.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsConfigMethodsEnabled(int index, char *methods)
{
	int radioIndex=-1;
	
	wlan_getRadioIndex(index, &radioIndex);
	
	return stringValueGet(__FUNCTION__,
		radioIndex,
		methods,
		DEVICE_RADIO_WPS_VENDOR,
		"WPS2ConfigMethodsEnabled");
}


/**************************************************************************/
/*! \fn int wlan_setWpsConfigMethodsEnabled(int index, char *methods)
 **************************************************************************
 *  \brief set WPS enabled configuration methods as comma-separated list of
 *  the WPS configuration methods currently enabled by the device.
 *  \param[in] int index - The index of the entry
 *  \param[out] char *methods - The WPS enabled configuration methods.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWpsConfigMethodsEnabled(int index, char *methods)
{
	int radioIndex=-1;
	
	wlan_getRadioIndex(index, &radioIndex);
	
	return stringValueSet(fapi_wlan_wps_set,
		radioIndex,
		methods,
		DEVICE_RADIO_WPS_VENDOR,
		"WPS2ConfigMethodsEnabled");
}


/**************************************************************************/
/*! \fn int wlan_getWpsSetupLockedState(int index, char *state)
 **************************************************************************
 *  \brief get WPS Setup Locked State as either one of "Unlocked", "LockedByLocalManagement",
 *  "LockedByRemoteManagement", "PINRetryLimitReached" values
 *  \param[in] int index - The index of the entry
 *  \param[out] char *state - The WPS Setup Locked State
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsSetupLockedState(int index, char *state)
{
	(void)index;  // unused parameter
	(void)state;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpsSetupLock(int index, bool *lock)
 **************************************************************************
 *  \brief get WPS Setup Lock value. True - the Access Point will refuse
 *  to accept new external registrars. False - the Access Point is enabled
 *  for configuration through an external registrar.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool *lock - The WPS Setup Lock value
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsSetupLock(int index, bool *lock)
{
	(void)index;  // unused parameter
	(void)lock;   // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setWpsSetupLock(int index, bool lock)
 **************************************************************************
 *  \brief set WPS Setup Lock value. When set to True, the Access Point will refuse
 *  to accept new external registrars; already established registrars will continue
 *  to be able to add new enrollees (the Setup Locked State becomes "LockedByRemoteManagement").
 *  When set to False, the Access Point is enabled for configuration through an external
 *  registrar (the Setup Locked State becomes "Unlocked").
 *  \param[in] int index - The index of the entry
 *  \param[in] bool lock - The WPS Setup Lock value
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWpsSetupLock(int index, bool lock)
{
	(void)index;  // unused parameter
	(void)lock;   // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpsConfigurationState(int index, char *state)
 **************************************************************************
 *  \brief get WPS Configuration State as either one of "Not configured"
 *  (WLAN interface is unconfigured: out-of-the box configuration) or
 *  "Configured" (WLAN interface is configured) values
 *  \param[in] int index - The index of the entry
 *  \param[out] char *state - The WPS Configuration State
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsConfigurationState(int index, char *state)
{
	int radioIndex=-1;
	
	wlan_getRadioIndex(index, &radioIndex);
	
	return stringValueGet(__FUNCTION__,
		radioIndex,
		state,
		DEVICE_RADIO_WPS_VENDOR,
		"ConfigState");
}


/**************************************************************************/
/*! \fn int wlan_getWpsLastConfigurationError(int index, char *error)
 **************************************************************************
 *  \brief get WPS Last Configuration Error as either one of "NoError", "DecryptionCRCFailure",
 *  "SignalTooWeak", "CouldntConnectToRegistrar", "RogueActivitySuspected", "DeviceBusy",
 *  "SetupLocked", "MessageTimeout", "RegistrationSessionTimeout", "DevicePasswordAuthFailure" values
 *  \param[in] int index - The index of the entry
 *  \param[out] char *error - The WPS Last Configuration Error
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsLastConfigurationError(int index, char *error)
{
	(void)index;  // unused parameter
	(void)error;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpsRegistrarNumEntries(int index, u_int_32 *numEntries)
 **************************************************************************
 *  \brief get Number of Registrars.
 *  \param[in] int index - The index of the entry
 *  \param[out] u_int_32 *numEntries - The number of Registrars.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsRegistrarNumEntries(int index, u_int_32 *numEntries)
{
	(void)index;       // unused parameter
	(void)numEntries;  // unused parameter
	return UGW_SUCCESS;
}


int wlan_addWpsRegistrarEntry(int index, unsigned int *numEntries)
{
	(void)index;       // unused parameter
	(void)numEntries;  // unused parameter
	return UGW_SUCCESS;
}


int wlan_delWpsRegistrarEntry(int index, int registrarIndex, int *numEntries)
{
	(void)index;           // unused parameter
	(void)registrarIndex;  // unused parameter
	(void)numEntries;      // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpsRegistrarEstablished(int index, bool *established)
 **************************************************************************
 *  \brief get Registrar established flag.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool *established - Registrar established flag.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsRegistrarEstablished(int index, bool *established)
{
	(void)index;        // unused parameter
	(void)established;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpsRegistrarEnabled(int index, int registrarIndex, bool *enabled)
 **************************************************************************
 *  \brief get Registrar enabled flag.
 *  \param[in] int index - The index of the WLAN entry
 *  \param[in] int registrarIndex - The index of the Registrar entry
 *  \param[out] bool *enabled - Registrar enabled flag.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsRegistrarEnabled(int index, int registrarIndex, bool *enabled)
{
	(void)index;           // unused parameter
	(void)registrarIndex;  // unused parameter
	(void)enabled;         // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setWpsRegistrarEnabled(int index, int registrarIndex, bool enabled)
 **************************************************************************
 *  \brief set Registrar enabled flag.
 *  \param[in] int index - The index of the WLAN entry
 *  \param[in] int registrarIndex - The index of the Registrar entry
 *  \param[in] bool enabled - Registrar enabled flag.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWpsRegistrarEnabled(int index, int registrarIndex, bool enabled)
{
	(void)index;           // unused parameter
	(void)registrarIndex;  // unused parameter
	(void)enabled;         // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpsRegistrarUuid(int index, int registrarIndex, char *uuid)
 **************************************************************************
 *  \brief get UUID of the registrar. This is represented as specified in RFC 4122 [48]
 *  but omitting the leading urn:uuid:, e.g. f81d4fae-7dec-11d0-a765-00a0c91e6bf6.
 *  \param[in] int index - The index of the WLAN entry
 *  \param[in] int registrarIndex - The index of the Registrar entry
 *  \param[out] char *uuid - UUID of the registrar.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsRegistrarUuid(int index, int registrarIndex, char *uuid)
{
	(void)index;           // unused parameter
	(void)registrarIndex;  // unused parameter
	(void)uuid;            // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getWpsRegistrarDeviceName(int index, int registrarIndex, char *name)
 **************************************************************************
 *  \brief get Device Name of the registrar.
 *  \param[in] int index - The index of the WLAN entry
 *  \param[in] int registrarIndex - The index of the Registrar entry
 *  \param[out] char *name - Device Name of the registrar.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWpsRegistrarDeviceName(int index, int registrarIndex, char *devName)
{
	(void)index;           // unused parameter
	(void)registrarIndex;  // unused parameter
	(void)devName;         // unused parameter
	return UGW_SUCCESS;
}


int wlan_setWpsPbcTrigger(int index)
{
	return stringValueSetDbRadio(fapi_wlan_wps_set,
		index,
		"PBC",
		DEVICE_RADIO_WPS_VENDOR,
		"WPSAction");
}

int wlan_kickAclAssociatedDevices(int index, bool enable)
{
	char boolString[] = "Allow";

	if (enable)
	{
		strcpy(boolString, "Deny");
	}

	return stringValueSet(fapi_wlan_ap_set,
		index,
		boolString,
		DEVICE_ACCESS_POINT_VENDOR,
		"MACAddressControlMode");
}

/**************************************************************************/
/*! \fn int wlan_getGreenApPowerSave(int index, bool *enabled, u_int_32 *timeout)
 **************************************************************************
 *  \brief get Green AP Power Save mode.
 *  \param[in] int index - The index of the entry
 *  \param[out] bool *enabled - True if Green AP Power Save mode enabled
 *  \param[out] u_int_32 *timeout - Timeout value
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getGreenApPowerSave(int index, bool *enabled, u_int_32 *timeout)
{
	(void)index;    // unused parameter
	(void)enabled;  // unused parameter
	(void)timeout;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setGreenApPowerSave(int index, bool enable, u_int_32 timeout)
 **************************************************************************
 *  \brief set Green AP Power Save mode.
 *  \param[in] int index - The index of the entry
 *  \param[in] bool enable - True for Green AP Power Save mode enable
 *  \param[in] u_int_32 timeout - Timeout value
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setGreenApPowerSave(int index, bool enable, u_int_32 timeout)
{
	(void)index;    // unused parameter
	(void)enable;   // unused parameter
	(void)timeout;  // unused parameter
	return UGW_SUCCESS;
}


int wlan_getSsidQosEnabled(int radio, bool *enable)
{
	(void)radio;   // unused parameter
	(void)enable;  // unused parameter
	return UGW_SUCCESS;
}


int wlan_setSsidQosEnabled(int radio, bool enabled)
{
	(void)radio;    // unused parameter
	(void)enabled;  // unused parameter
	return UGW_SUCCESS;
}


int wlan_getSsidQosLevel(int index, char *lvl)
{
	(void)index;  // unused parameter
	(void)lvl;    // unused parameter
	return UGW_SUCCESS;
}


int wlan_setSsidQosLevel(int index, char *lvl)
{
	(void)index;  // unused parameter
	(void)lvl;    // unused parameter
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_factoryReset(void)
 **************************************************************************
 *  \brief  Factory Reset wifi.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_factoryReset(void)
{
	char command[MAX_LEN_PARAM_VALUE];
	sprintf(command, "%s%s", "rm -rf ", DB_PATH);
	system(command);
	wlan_uninit();
	return wlan_init();
}


int wlan_clearWaveNvram(void)
{
	char command[MAX_LEN_PARAM_VALUE];
	char waveKnownPaths[MAX_LEN_PARAM_VALUE];
	sprintf(waveKnownPaths, "%s %s %s %s", DB_PATH, "/nvram/wave_scripts/", "/nvram/etc/wave_overlay", "/nvram/etc/wave_dumps");
	snprintf(command, sizeof(command), "%s %s", "rm -rf ", waveKnownPaths);
	system(command);
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_factoryComplete(void)
 **************************************************************************
 *  \brief  Factory Reset wifi.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_factoryComplete(void)
{
	char command[MAX_LEN_PARAM_VALUE];
	sprintf(command, "%s%s", "rm -rf ", DB_PATH);
	system(command);
	system("reboot -f");
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_disableRpcMode(void)
 **************************************************************************
 *  \brief  disableRpcMode and return to regular SDK (UGW)
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_disableRpcMode(void)
{
	char command[MAX_LEN_PARAM_VALUE];
	sprintf(command, "%s%s", "rm -rf ", FAPI_RPC_MODE);
	return system(command);
}

/**************************************************************************/
/*! \fn int wlan_factoryResetRadios(void)
**************************************************************************
*  \brief  Factory Reset wifi.
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_factoryResetRadios(void)
{
	ObjList	*tempLoopObj = NULL;
	ParamList *param = NULL;
	char *radioName = NULL;
	char *ifName = NULL;
	char strIndex[MAX_LEN_PARAM_VALUE];
	char command[MAX_LEN_PARAM_VALUE];

	fapiWlanFailSafeLoad();

	FOR_EACH_OBJ(mapperObjPtr, tempLoopObj)
	{
		radioName = NULL;
		ifName = NULL;
		FOR_EACH_PARAM(tempLoopObj, param)
		{
			if (!strcmp(DB_RADIO, GET_PARAM_NAME(param)))
			{
				radioName = GET_PARAM_VALUE(param);
			}

			if (!strcmp(DB_IFNAME, GET_PARAM_NAME(param)))
			{
				ifName = GET_PARAM_VALUE(param);
			}
		}

		if (!radioName || !ifName)
			continue;
		if (!strcmp(radioName, ifName))
		{
			strRemoveAll(strIndex, "wlan");
			sprintf(command, "cp %sradio%d/%s %swlan%d/", DB_PATH_DEFAULT, atoi(strIndex), DEVICE_RADIO, DB_PATH, atoi(strIndex));
			system(command); //TODO REPLACE BY CLEAN C FUNCTION
			sprintf(command, "cp %sradio%d/%s %swlan%d/", DB_PATH_DEFAULT, atoi(strIndex), DEVICE_RADIO_VENDOR, DB_PATH, atoi(strIndex));
			system(command); //TODO REPLACE BY CLEAN C FUNCTION
		}
	}

	wlan_uninit();
	return wlan_init();
}

/**************************************************************************/
/*! \fn int wlan_down(void)
 **************************************************************************
 *  \brief  Put the AP in down.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_down(void)
{
	ObjList	*dbObjPtr = NULL;
	dbObjPtr = dbObjPtrSet(dbObjPtr, DEVICE_RADIO, "Enable", "false");

	if (wlan_applyObjectForAllRadios(fapi_wlan_radio_set, dbObjPtr) == UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_SUCCESS;
	}

	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
	return UGW_FAILURE;
}

/**************************************************************************/
/*! \fn int wlan_apply(void)
 **************************************************************************
 *  \brief  Run ifconfigUp for all interafces.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_apply(void)
{
	ObjList	*dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	if (wlan_applyObjectForAllRadios(fapi_wlan_up, dbObjPtr) == UGW_SUCCESS)
		{
			HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
			return UGW_SUCCESS;
		}

	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
	return UGW_FAILURE;
}

int wlan_version(void)
{
	char command[MAX_LEN_PARAM_VALUE];
	sprintf(command, "cat %sCV.txt", BINS_PATH);
	system(command);
	logger("FAPI_WLAN_COMMON VERSION: %s\n", FAPI_COMMON_VERSION);
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_createCfgFile(void)
 **************************************************************************
 *  \brief  creates wifi configuration files.
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_createCfgFile()
{
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_initRadio(int index)
 **************************************************************************
 *  \brief initializes the specified radio.
 *  \param[in] int index - The index of the entry
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_initRadio(int index)
{
	ObjList *wlObjRPC = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if (wlObjRPC == NULL)
	{
		return UGW_FAILURE;
	}

	if (getInterfaceName(index, interfaceName) == UGW_FAILURE)
	{
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	wlanLoadFromDB(DEVICE_RADIO, interfaceName, wlObjRPC);

	if (fapi_wlan_radio_set(interfaceName, wlObjRPC, 0) != 0)
	{
		HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_FAILURE;
	}

	HELP_DELETE_OBJ(wlObjRPC, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getActiveAthIndex(int radioIndex, int *activeIndex)
 **************************************************************************
 *  \brief Find the first ath Index that is up on the given radio.
 *  \param[in] int radioIndex - The index of the entry
 *  \param[out] bool *activeIndex - True if radio is enabled
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getActiveAthIndex(int radioIndex, int *activeIndex)
{
	(void)radioIndex;   // unused parameter
	(void)activeIndex;  // unused parameter
	return UGW_FAILURE;
}


/**************************************************************************/
/*! \fn int wlan_setDTIMInterval(int index, int dtimPeriod)
 **************************************************************************
 *  \brief set DTIM Interval
 *  \param[in] int index - AP index
 *  \param[in] int dtimPeriod - value of dtim interval
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setDTIMInterval(int index, int dtimPeriod)
{
	(void)index;       // unused parameter
	(void)dtimPeriod;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setCtsProtectionEnable(int index, bool enabled)
 **************************************************************************
 *  \brief set CTS Protection
 *  \param[in] int index - AP index
 *  \param[in] int enable - true to enable false to disable
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setCtsProtectionEnable(int index, bool enabled)
{
	(void)index;    // unused parameter
	(void)enabled;  // unused parameter
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_getCountryCode(int index, char *code)
 **************************************************************************
 *  \brief get CountryCode
 *  \param[in] int index - Radio index
 *  \param[out] char *code - country code
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getCountryCode(int index, char *code)
{
	return stringValueGet(__FUNCTION__,
		index,
		code,
		DEVICE_RADIO,
		"RegulatoryDomain");
}

/**************************************************************************/
/*! \fn int wlan_setCountryCode(int index, char *code)
 **************************************************************************
 *  \brief set CountryCode
 *  \param[in] int index - Radio index
 *  \param[out] char *code - country code
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setCountryCode(int index, char *code)
{
	return stringValueSet(fapi_wlan_radio_set,
		index,
		code,
		DEVICE_RADIO,
		"RegulatoryDomain");
}

/**************************************************************************/
/*! \fn int wlan_setSTBCEnable(int index, bool enable)
 **************************************************************************
 *  \brief set STBC
 *  \param[in] int index - AP index
 *  \param[in] bool enable - true to enable false to disable
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setSTBCEnable(int index, bool enable)
{
	return boolValueSet(fapi_wlan_radio_set,
		index,
		enable,
		DEVICE_RADIO_VENDOR,
		"HtSTBCenabled");
}


/**************************************************************************/
/*! \fn int wlan_getAMSDUEnable(int index, bool * enable)
 **************************************************************************
 *  \brief get AMSDUEnable
 *  \param[in] int index - AP index
 *  \param[out] bool *enable - true if enable false if disable
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getAMSDUEnable(int index, bool * enable)
{
	return boolValueGet("fapi_wlan_ap_set",
		index,
		enable,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveAmsduEnabled");
}


/**************************************************************************/
/*! \fn int wlan_setAMSDUEnable(int index, bool enable)
 **************************************************************************
 *  \brief set AMSDUEnable
 *  \param[in] int index - AP index
 *  \param[in] bool enable - true to enable false to disable
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setAMSDUEnable(int index, bool enable)
{
	return boolValueSet(fapi_wlan_ap_set,
		index,
		enable,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveAmsduEnabled");
}


/**************************************************************************/
/*! \fn int wlan_getTxChainMask(int index, int *numStreams)
 **************************************************************************
 *  \brief get Tx chain mask
 *  \param[in] int index - AP index
 *  \param[out] int *numStreams
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getTxChainMask(int index, int *numStreams)
{
	return intValueGet(__FUNCTION__,
		index,
		numStreams,
		DEVICE_RADIO_VENDOR,
		"NumOfAntennas");
}


/**************************************************************************/
/*! \fn int wlan_setTxChainMask(int index, int numStreams)
 **************************************************************************
 *  \brief set Tx chain mask to hardware
 *  \param[in] int index - AP index
 *  \param[in] int numStreams
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setTxChainMask(int index, int numStreams)
{
	return intValueSet(fapi_wlan_radio_set,
		index,
		numStreams,
		DEVICE_RADIO_VENDOR,
		"NumOfAntennas");
}


/**************************************************************************/
/*! \fn int wlan_pushTxChainMask(int index, int numStreams)
 **************************************************************************
 *  \brief push Tx chain mask on hardware
 *  \param[in] int index - AP index
 *  \param[in] int numStreams
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_pushTxChainMask(int index, int numStreams)
{
	(void)index;       // unused parameter
	(void)numStreams;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_getRxChainMask(int index, int *numStreams)
 **************************************************************************
 *  \brief get Tx chain mask
 *  \param[in] int index - AP index
 *  \param[out] int *numStreams
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getRxChainMask(int index, int *numStreams)
{
	return intValueGet(__FUNCTION__,
		index,
		numStreams,
		DEVICE_RADIO_VENDOR,
		"NumOfAntennas");
}


/**************************************************************************/
/*! \fn int wlan_setRxChainMask(int index, int numStreams)
 **************************************************************************
 *  \brief set Rx chain mask to hardware
 *  \param[in] int index - AP index
 *  \param[in] int numStreams
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setRxChainMask(int index, int numStreams)
{
	return intValueSet(fapi_wlan_radio_set,
		index,
		numStreams,
		DEVICE_RADIO_VENDOR,
		"NumOfAntennas");
}


/**************************************************************************/
/*! \fn int wlan_pushRxChainMask(int index, int numStreams)
 **************************************************************************
 *  \brief push Rx chain mask on hardware
 *  \param[in] int index - AP index
 *  \param[in] int numStreams
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_pushRxChainMask(int index, int numStreams)
{
	(void)index;       // unused parameter
	(void)numStreams;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setShortGuardInterval(int index, bool enable)
 **************************************************************************
 *  \brief set Guard Interval
 *  \param[in] int index - AP Index
 *  \param[in] bool enable - true/false
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setShortGuardInterval(int index, bool enable)
{
	(void)index;   // unused parameter
	(void)enable;  // unused parameter
	return UGW_FAILURE;
}


/**************************************************************************/
/*! \fn int wlan_getRadioIEEE80211hEnabled(int radio, bool *enable)
**************************************************************************
*  \brief get RadioIEEE80211hEnabled field
*  \param[in] int index - AP Index
*  \param[in] bool enable - true/false
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getRadioIEEE80211hEnabled(int radio, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		radio,
		enable,
		DEVICE_RADIO,
		"IEEE80211hEnabled");
}


/**************************************************************************/
/*! \fn int wlan_getRadioIEEE80211hSupported(int radio, bool *enable)
**************************************************************************
*  \brief get RadioIEEE80211hSupported field
*  \param[in] int index - AP Index
*  \param[in] bool enable - true/false
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getRadioIEEE80211hSupported(int radio, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		radio,
		enable,
		DEVICE_RADIO,
		"IEEE80211hSupported");
}


/**************************************************************************/
/*! \fn int wlan_setGuardInterval(int index, char* guardInterval)
**************************************************************************
*  \brief set Guard Interval
*  \param[in] int index - AP Index
*  \param[in] char* guardInterval - guard Interval
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setGuardInterval(int index, char* guardInterval)
{
	return stringValueSet(fapi_wlan_radio_set,
		index,
		guardInterval,
		DEVICE_RADIO,
		"GuardInterval");
}

/**************************************************************************/
/*! \fn int wlan_getGuardInterval(int index, char* guardInterval)
**************************************************************************
*  \brief get Guard Interval
*  \param[in] int index - AP Index
*  \param[in] char* guardInterval - guard Interval
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getGuardInterval(int index, char* guardInterval)
{
	return stringValueGet(__FUNCTION__,
		index,
		guardInterval,
		DEVICE_RADIO,
		"GuardInterval");
}

int wlan_setBridgeName(int index, char *bridge)
{
	return stringValueSet(fapi_wlan_ap_set,
		index,
		bridge,
		DEVICE_SSID,
		"X_LANTIQ_COM_Vendor_BridgeName");
}

int wlan_getBridgeName(int index, char *bridge)
{
	return stringValueGet(__FUNCTION__,
		index,
		bridge,
		DEVICE_SSID,
		"X_LANTIQ_COM_Vendor_BridgeName");
}

/**************************************************************************/
/*! \fn int wlan_getBridgeInfo(int index,char *bridge,char *ip,char *subnet)
 **************************************************************************
 *  \brief get Bridge Information
 *  \param[in] int index - AP Index
 *  \param[out] char * bridge - bridge Name
 *  \param[out] char * ip - IP address
 *  \param[out] char * subnet - subnet mask
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getBridgeInfo(int index, char *bridge, char *ip, char *subnet)
{
	(void)index;   // unused parameter
	(void)bridge;  // unused parameter
	(void)ip;      // unused parameter
	(void)subnet;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_pushBridgeInfo(int index)
 **************************************************************************
 *  \brief push Bridge Info to hardware
 *  \param[in] int index - AP Index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_pushBridgeInfo(int index)
{
	(void)index;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setBridgeInfo(int index,char *bridge,char *ip,char *subnet)
 **************************************************************************
 *  \brief set Bridge Information
 *  \param[in] int index - AP Index
 *  \param[in] char * bridge - bridge Name
 *  \param[in] char * ip - IP address
 *  \param[in] char * subnet - subnet mask
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setBridgeInfo(int index, char *bridge, char *ip, char *subnet)
{
	(void)index;   // unused parameter
	(void)bridge;  // unused parameter
	(void)ip;      // unused parameter
	(void)subnet;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_resetVlanCfg(int index)
 **************************************************************************
 *  \brief reset Vlan Configuration
 *  \param[in] int index - AP Index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_resetVlanCfg(int index)
{
	(void)index;  // unused parameter
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_getWirelessOnOffButton(int index, bool * enable)
 **************************************************************************
 *  \brief get status of wirelessOnOffButton
 *  \param[in] int index - AP Index
 *  \param[out] bool *enable - true/false
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getWirelessOnOffButton(int index, bool *enable)
{
	(void)index;   // unused parameter
	(void)enable;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setWirelessOnOffButton(int index, bool enable)
 **************************************************************************
 *  \brief set status of wirelessOnOffButton
 *  \param[in] int index - AP Index
 *  \param[in] bool enable - true/false
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWirelessOnOffButton(int index, bool enable)
{
	(void)index;   // unused parameter
	(void)enable;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setRadioIndex(int index, int radio_idx)
 **************************************************************************
 *  \brief set Radio Index
 *  \param[in] int index - AP Index
 *  \param[in] int radio_idx - radio index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setRadioIndex(int index, int radio_idx)
{
	(void)index;      // unused parameter
	(void)radio_idx;  // unused parameter
	return UGW_SUCCESS;
}



/**************************************************************************/
/*! \fn int wlan_removeSecFromConfFile(int vap)
 **************************************************************************
 *  \brief remove security from config file
 *  \param[in] int vap - virtual access point
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_removeSecFromConfFile(int vap)
{
	(void)vap;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_createSecurityFile(int vap, bool createWpsCfg)
 **************************************************************************
 *  \brief create security file
 *  \param[in] int vap- virtual access point
 *  \param[in] bool createWpsCfg - true/false
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_createSecurityFile(int vap, bool createWpsCfg)
{
	(void)vap;           // unused parameter
	(void)createWpsCfg;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_setEncryptionOff(int vap)
 **************************************************************************
 *  \brief set Encryption off
 *  \param[in] int vap- virtual access point
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setEncryptionOff(int vap)
{
	return (wlan_security_set(vap, "None"));
}


/**************************************************************************/
/*! \fn int wlan_setAuthMode(int vap, int mode)
 **************************************************************************
 *  \brief set authentication mode of virtual access point
 *  \param[in] int vap - virtual access point
 *  \param[in] int mode - authentication Mode
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setAuthMode(int vap, int mode)
{
	(void)vap;   // unused parameter
	(void)mode;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_startHostapd()
 **************************************************************************
 *  \brief start hostapd
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_startHostapd()
{
	return UGW_SUCCESS;
}

/**************************************************************************/
/*! \fn int wlan_stopHostapd(int index)
**************************************************************************
*  \brief call to stop hostapd
*  \param[in] int index - AP Index
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_stopHostapd()
{
	ObjList	*dbObjPtr = NULL;
	dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	if (wlan_applyObjectForAllRadios(fapi_wlan_hostapd_stop, dbObjPtr) == UGW_SUCCESS)
	{
		HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
		return UGW_SUCCESS;
	}

	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
	return UGW_FAILURE;
}

/**************************************************************************/
/*! \fn int wlan_getVapEnable(int index, bool * enable)
 **************************************************************************
 *  \brief get virtual access point is enabled/ or not
 *  \param[in] int index - AP Index
 *  \param[out] bool * enable - true/false
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getVapEnable(int index, bool *enable)
{
	(void)index;   // unused parameter
	(void)enable;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_pushWepKeyIndex(int index, unsigned int keyIndex)
 **************************************************************************
 *  \brief push wep key index to hardware
 *  \param[in] int index - AP Index
 *  \param[in] unsigned int keyIndex - key index
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_pushWepKeyIndex(int index, unsigned int keyIndex)
{
	(void)index;     // unused parameter
	(void)keyIndex;  // unused parameter
	return UGW_SUCCESS;
}



/**************************************************************************/
/*! \fn int wlan_pushSsidAdvertisementEnabled(int index, bool enabled)
 **************************************************************************
 *  \brief Hide/ Un-hide SSID
 *  \param[in] int index - AP Index
 *  \param[in] bool enabled - true/false
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_pushSsidAdvertisementEnabled(int index, bool enabled)
{
	(void)index;    // unused parameter
	(void)enabled;  // unused parameter
	return UGW_SUCCESS;
}

#if 0
/**************************************************************************/
/*! \fn int wlan_kickAssocDevice(int index, wifi_device_t * dev)
 **************************************************************************
 *  \brief kick off wifi device from associated device list
 *  \param[in] int index - AP Index
 *  \param[in] wifi_device_t * dev - wifi device
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_kickAssocDevice(int index, wifi_device_t * dev)
{
	return UGW_SUCCESS;
}
#endif

/**************************************************************************/
/*! \fn int wlan_getAclDevice(int index, unsigned int devIndex,char *dev)
 **************************************************************************
 *  \brief get ACL devices
 *  \param[in] int index - AP Index
 *  \param[in] unsigned int devIndex - device index
 *  \param[out] char * dev - device mac address
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_getAclDevice(int index, unsigned int devIndex, char *dev)
{
	(void)index;     // unused parameter
	(void)devIndex;  // unused parameter
	(void)dev;       // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_kickAclAssocDevices(int index, bool denyList)
 **************************************************************************
 *  \brief kick off the devices from acl list
 *  \param[in] int index - AP Index
 *  \param[in] bool denyList - true/false
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_kickAclAssocDevices(int index, bool denyList)
{
	(void)index;     // unused parameter
	(void)denyList;  // unused parameter
	return UGW_SUCCESS;
}


/**************************************************************************/
/*! \fn int wlan_pushWepKey(int index, int key)
 **************************************************************************
 *  \brief push wep key to hardware
 *  \param[in] int index - AP Index
 *  \param[in] int key - wep key
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_pushWepKey(int index, int key)
{
	(void)index;  // unused parameter
	(void)key;    // unused parameter
	return UGW_SUCCESS;
}

/* ATF commands */
/**************************************************************************/
/*! \fn int wlan_setAtfEnable(int index, char* atfEnable)
**************************************************************************
*  \brief set atf enable
*  \param[in] int index - AP Index
*  \param[in] char* atfEnable - atf enable
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setAtfEnable(int index, bool enable)
{
	return boolValueSet(fapi_wlan_radio_set,
		index,
		enable,
		DEVICE_RADIO_VENDOR,
		"WaveAtfEnabled");
}
/**************************************************************************/
/*! \fn int wlan_getAtfEnable(int index, char* atfEnable)
**************************************************************************
*  \brief get atf enable
*  \param[in] int index - AP Index
*  \param[in] char* atfEnable - atf enable
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAtfEnable(int index, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_RADIO_VENDOR,
		"WaveAtfEnabled");
}

/**************************************************************************/
/*! \fn int wlan_setAtfDistributionType(int index, char* atfDistributionType)
**************************************************************************
*  \brief set Distribution Type
*  \param[in] int index - AP Index
*  \param[in] char* atfDistributionType - Static,Dynmic,Disable
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setAtfDistributionType(int index, char *atfDistributionType)
{
	return stringValueSet(fapi_wlan_radio_set,
		index,
		atfDistributionType,
		DEVICE_RADIO_VENDOR,
		"WaveAtfDistributionType");
}
/**************************************************************************/
/*! \fn int wlan_getAtfDistributionType(int index, char* atfDistributionType)
**************************************************************************
*  \brief get Distribution Type
*  \param[in] int index - AP Index
*  \param[in] char* atfDistributionType - Static,Dynmic,Disable
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAtfDistributionType(int index, char *atfDistributionType)
{
	return stringValueGet(__FUNCTION__,
		index,
		atfDistributionType,
		DEVICE_RADIO_VENDOR,
		"WaveAtfDistributionType");
}

/**************************************************************************/
/*! \fn int wlan_setAtfAlgoType(int index, char* atfAlgoType)
**************************************************************************
*  \brief set Distribution Type
*  \param[in] int index - AP Index
*  \param[in] char* atfAlgoType - Weighted,Global
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setAtfAlgoType(int index, char *atfAlgoType)
{
	return stringValueSet(fapi_wlan_radio_set,
		index,
		atfAlgoType,
		DEVICE_RADIO_VENDOR,
		"WaveAtfAlgoType");
}
/**************************************************************************/
/*! \fn int wlan_getAtfAlgoType(int index, char* atfAlgoType)
**************************************************************************
*  \brief get Distribution Type
*  \param[in] int index - AP Index
*  \param[in] char* atfAlgoType - Weighted,Global
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAtfAlgoType(int index, char *atfAlgoType)
{
	return stringValueGet(__FUNCTION__,
		index,
		atfAlgoType,
		DEVICE_RADIO_VENDOR,
		"WaveAtfAlgoType");
}
/**************************************************************************/
/*! \fn int wlan_setAtfInterval(int index, int atfInterval)
**************************************************************************
*  \brief set Distribution Type
*  \param[in] int index - AP Index
*  \param[in] int atfInterval - in msec
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setAtfInterval(int index, int atfInterval)
{
	return intValueSet(fapi_wlan_radio_set,
		index,
		atfInterval,
		DEVICE_RADIO_VENDOR,
		"WaveAtfInterval");
}
/**************************************************************************/
/*! \fn int wlan_getAtfInterval(int index, int* atfInterval)
**************************************************************************
*  \brief get Distribution Type
*  \param[in] int index - AP Index
*  \param[in] int* atfInterval - in msec
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAtfInterval(int index, int *atfInterval)
{
	return intValueGet(__FUNCTION__,
		index,
		atfInterval,
		DEVICE_RADIO_VENDOR,
		"WaveAtfInterval");
}
/**************************************************************************/
/*! \fn int wlan_setAtfFreeTime(int index, int atfFreeTime)
**************************************************************************
*  \brief set Distribution Type
*  \param[in] int index - AP Index
*  \param[in] int atfFreeTime - in msec
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setAtfFreeTime(int index, int atfFreeTime)
{
	return intValueSet(fapi_wlan_radio_set,
		index,
		atfFreeTime,
		DEVICE_RADIO_VENDOR,
		"WaveAtfFreeTime");
}
/**************************************************************************/
/*! \fn int wlan_getAtfFreeTime(int index, int* atfFreeTime)
**************************************************************************
*  \brief get Distribution Type
*  \param[in] int index - AP Index
*  \param[in] int* atfFreeTime - in msec
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAtfFreeTime(int index, int *atfFreeTime)
{
	return intValueGet(__FUNCTION__,
		index,
		atfFreeTime,
		DEVICE_RADIO_VENDOR,
		"WaveAtfFreeTime");
}
/**************************************************************************/
/*! \fn int wlan_setAtfVapEnabled(int index, char* atfVapEnabled)
**************************************************************************
*  \brief set VAP enable
*  \param[in] int index - AP Index
*  \param[in] char* atfVapEnabled - enable or disable
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setAtfVapEnabled(int index, bool enable)
{
	return boolValueSet(fapi_wlan_radio_set,
		index,
		enable,
		DEVICE_RADIO_VENDOR,
		"WaveAtfVapEnabled");
}
/**************************************************************************/
/*! \fn int wlan_getAtfVapEnabled(int index, char* atfVapEnabled)
**************************************************************************
*  \brief get VAP enable
*  \param[in] int index - AP Index
*  \param[in] char* atfVapEnabled -  enable or disable
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAtfVapEnabled(int index, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_RADIO_VENDOR,
		"WaveAtfVapEnabled");
}
/**************************************************************************/
/*! \fn int wlan_setAtfStationEnabled(int index, char* atfStationEnabled)
**************************************************************************
*  \brief set station enable
*  \param[in] int index - AP Index
*  \param[in] char* atfStationEnabled -  enable or disable
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setAtfStationEnabled(int index, bool enable)
{
	return boolValueSet(fapi_wlan_radio_set,
		index,
		enable,
		DEVICE_RADIO_VENDOR,
		"WaveAtfStationEnabled");
}

/**************************************************************************/
/*! \fn int wlan_getAtfStationEnabled(int index, char* atfStationEnabled)
**************************************************************************
*  \brief get station enable
*  \param[in] int index - AP Index
*  \param[in] char* atfStationEnabled - station enable
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAtfStationEnabled(int index, bool *enable)
{
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_RADIO_VENDOR,
		"WaveAtfStationEnabled");
}
/**************************************************************************/
/*! \fn int wlan_setAtfRadioStationsAndWeights(int index, char* atfRadioStationsAndWeights)
**************************************************************************
*  \brief set Radio Stations And Weights
*  \param[in] int index - AP Index
*  \param[in] char* atfRadioStationsAndWeights - MAC and Weight per STA
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setAtfRadioStationsAndWeights(int index, char *atfRadioStationsAndWeights)
{
	return stringValueSet(fapi_wlan_radio_set,
		index,
		atfRadioStationsAndWeights,
		DEVICE_RADIO_VENDOR,
		"WaveAtfRadioStationsAndWeights");
}
/**************************************************************************/
/*! \fn int wlan_getAtfRadioStationsAndWeights(int index, char* atfRadioStationsAndWeights)
**************************************************************************
*  \brief get Radio Stations And Weights
*  \param[in] int index - AP Index
*  \param[in] char* atfRadioStationsAndWeights - MAC and Weight per STA
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAtfRadioStationsAndWeights(int index, char *atfRadioStationsAndWeights)
{
	return stringValueGet(__FUNCTION__,
		index,
		atfRadioStationsAndWeights,
		DEVICE_RADIO_VENDOR,
		"WaveAtfRadioStationsAndWeights");
}
/**************************************************************************/
/*! \fn int wlan_setAtfVapWeight(int index, int atfVapWeight)
**************************************************************************
*  \brief set main Vap Weight
*  \param[in] int index - AP Index
*  \param[in] int atfVapWeight - 1-100
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setAtfVapWeight(int index, int atfVapWeight)
{
	return intValueSet(fapi_wlan_ssid_set,
		index,
		atfVapWeight,
		DEVICE_SSID,
		"X_LANTIQ_COM_Vendor_WaveAtfVapWeight");
}
/**************************************************************************/
/*! \fn int wlan_getAtfVapWeight(int index, int* atfVapWeight)
**************************************************************************
*  \brief get main Vap Weight
*  \param[in] int index - AP Index
*  \param[in] int* atfVapWeight - 1-100
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAtfVapWeight(int index, int *atfVapWeight)
{
	return intValueGet(__FUNCTION__,
		index,
		atfVapWeight,
		DEVICE_SSID,
		"X_LANTIQ_COM_Vendor_WaveAtfVapWeight");
}
/**************************************************************************/
/*! \fn int wlan_setAtfVapStationsAndWeights(int index, char* atfVapStationsAndWeights)
**************************************************************************
*  \brief set VAP Stations And Weights
*  \param[in] int index - AP Index
*  \param[in] char* atfVapStationsAndWeights - MAC and Weight per STA per VAP
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setAtfVapStationsAndWeights(int index, char *atfVapStationsAndWeights)
{
	return stringValueSet(fapi_wlan_ap_set,
		index,
		atfVapStationsAndWeights,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveAtfVapStationsAndWeights");
}
/**************************************************************************/
/*! \fn int wlan_getAtfVapStationsAndWeights(int index, char* atfVapStationsAndWeights)
**************************************************************************
*  \brief get VAP Stations And Weights
*  \param[in] int index - AP Index
*  \param[in] char* atfVapStationsAndWeights - MAC and Weight per STA per VAP
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getAtfVapStationsAndWeights(int index, char *atfVapStationsAndWeights)
{
	return stringValueGet(__FUNCTION__,
		index,
		atfVapStationsAndWeights,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveAtfVapStationsAndWeights");
}
/**************************************************************************/
/*! \fn int wlan_getWdsEnabled(int index, bool * enable)
**************************************************************************
*  \brief get WdsEnabled enable disable the WDS
*  \param[in] int index - AP index
*  \param[out]  bool *enable true or false
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getWdsEnabled(int index, bool * enable)
{
	return boolValueGet(__FUNCTION__,
		index,
		enable,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveWDSEnabled");
}
/**************************************************************************/
/*! \fn int wlan_setWdsEnabled(int index, bool enable)
 **************************************************************************
 *  \brief set WdsEnabled enable disable the WDS
 *  \param[in] int index - AP index
 *  \param[in] bool enable true or false
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWdsEnabled(int index, bool enable)
{
	return boolValueSet(fapi_wlan_ap_set,
		index,
		enable,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveWDSEnabled");
}

/**************************************************************************/
/*! \fn int wlan_getWdsSecurityMode(int index, char* strValue)
**************************************************************************
*  \brief get WdsSecurityMode WDS security mode 
*  \param[in] int index - AP index
*  \param[out] int *strValue
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getWdsSecurityMode(int index, char* strValue)
{
	return stringValueGet(__FUNCTION__,
		index,
		strValue,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveWDSSecurityMode");
}
/**************************************************************************/
/*! \fn int wlan_setWdsSecurityMode(int index, char* strValue)
 **************************************************************************
 *  \brief set WdsSecurityMode WDS security mode 
 *  \param[in] int index - AP index
 *  \param[in] char* strValue
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWdsSecurityMode(int index, char* strValue)
{
	return stringValueSet(fapi_wlan_ap_set,
		index,
		strValue,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveWDSSecurityMode");
}

/**************************************************************************/
/*! \fn int wlan_getWdsKey(int index, char* strValue)
**************************************************************************
*  \brief get WdsKey WDS key
*  \param[in] int index - AP index
*  \param[out] int *strValue
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getWdsKey(int index, char* strValue)
{
	return stringValueGet(__FUNCTION__,
		index,
		strValue,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveWDSKey");
}
/**************************************************************************/
/*! \fn int wlan_setWdsKey(int index, char* strValue)
 **************************************************************************
 *  \brief set WdsKey WDS key
 *  \param[in] int index - AP index
 *  \param[in] char* strValue
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWdsKey(int index, char* strValue)
{
	return stringValueSet(fapi_wlan_ap_set,
		index,
		strValue,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveWDSKey");
}

/**************************************************************************/
/*! \fn int wlan_getWdsPeers(int index, char* strValue)
**************************************************************************
*  \brief get WdsPeers WDS peers
*  \param[in] int index - AP index
*  \param[out] int *strValue
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getWdsPeers(int index, char* strValue)
{
	return stringValueGet(__FUNCTION__,
		index,
		strValue,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveWDSPeers");
}
/**************************************************************************/
/*! \fn int wlan_setWdsPeers(int index, char* strValue)
 **************************************************************************
 *  \brief set WdsPeers WDS peers
 *  \param[in] int index - AP index
 *  \param[in] char* strValue
 *  \return 0 if success, negative if error / timeout
 ***************************************************************************/
int wlan_setWdsPeers(int index, char* strValue)
{
	return stringValueSet(fapi_wlan_ap_set,
		index,
		strValue,
		DEVICE_ACCESS_POINT_VENDOR,
		"WaveWDSPeers");
}


/**************************************************************************/
/*! \fn int wlan_getWdsMode (int index, char* strValue)
**************************************************************************
*  \brief get WdsMode WDS mode
*  \param[in] int index - AP index
*  \param[out] int *strValue
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_getWdsMode(int index, char* strValue)
{
                return stringValueGet(__FUNCTION__,
                                index,
                                strValue,
                                DEVICE_ACCESS_POINT_VENDOR,
                                "WaveWDSMode");
}
/**************************************************************************/
/*! \fn int wlan_ setWdsMode(int index, char* strValue)
**************************************************************************
*  \brief set WdsMode WDS mode
*  \param[in] int index - AP index
*  \param[in] char* strValue
*  \return 0 if success, negative if error / timeout
***************************************************************************/
int wlan_setWdsMode(int index, char* strValue)
{
                return stringValueSet(fapi_wlan_ap_set,
                                index,
                                strValue,
                                DEVICE_ACCESS_POINT_VENDOR,
                                "WaveWDSMode");
}

int wlan_getBfMode(int index, char* strValue)
{
                return stringValueGet(__FUNCTION__,
                                index,
                                strValue,
                                DEVICE_RADIO_VENDOR,
                                "WaveBfMode");
}

int wlan_setBfMode(int index, char* strValue)
{
                return stringValueSet(fapi_wlan_radio_set,
                                index,
                                strValue,
                                DEVICE_RADIO_VENDOR,
                                "WaveBfMode");
}
//#endif  /* Functions only for RDKB - END */


int fapi_wlan_radio_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_generic_set(fapi_wlan_radio_set, index, wlObj, flags);
}


int fapi_wlan_ssid_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_generic_set(fapi_wlan_ssid_set, index, wlObj, flags);
}


int fapi_wlan_ap_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_generic_set(fapi_wlan_ap_set, index, wlObj, flags);
}


int fapi_wlan_security_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_generic_set(fapi_wlan_security_set, index, wlObj, flags);
}


int fapi_wlan_wps_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_generic_set(fapi_wlan_wps_set, index, wlObj, flags);
}


int fapi_wlan_wmm_sta_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_generic_set(fapi_wlan_wmm_sta_set, index, wlObj, flags);
}


int fapi_wlan_wmm_ap_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_generic_set(fapi_wlan_wmm_ap_set, index, wlObj, flags);
}


int fapi_wlan_wds_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_generic_set(fapi_wlan_wds_set, index, wlObj, flags);
}

int fapi_wlan_hotspot_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_generic_set(fapi_wlan_hotspot_set, index, wlObj, flags);
}

/*int fapi_wlan_endpoint_set_native(int index, ObjList *wlObj, unsigned int flags);
{
	return fapi_wlan_generic_set(fapi_wlan_endpoint_set, index, wlObj, flags);
}*/
/* BeeRock APIs */
int fapi_wlan_start_monitoring_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_set(fapi_wlan_start_monitoring, index, wlObj, flags);
}

int fapi_wlan_sta_steer_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_set(fapi_wlan_sta_steer, index, wlObj, flags);
}

int wlan_staSteer(int index, char *MACAddress, char *BSSID)
{
	ObjList	*dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if ((dbObjPtr == NULL) || (getInterfaceName(index, interfaceName) == UGW_FAILURE))
	{
		return UGW_FAILURE;
	}

	help_addObjList(dbObjPtr, "Device.WiFi.AccessPoint.AssociatedDevice", 0, 0, 0, 0);
	HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress", MACAddress, 0, 0);  // Station's MAC Address

	help_addObjList(dbObjPtr, "Device.WiFi.SSID", 0, 0, 0, 0);
	HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.SSID", "BSSID", BSSID, 0, 0);  // Dest AP BSSID

	setLog(interfaceName, dbObjPtr, 0);

	return fapi_wlan_sta_steer(interfaceName, dbObjPtr, 0);
}

int fapi_wlan_sta_allow_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_set(fapi_wlan_sta_allow, index, wlObj, flags);
}

int wlan_staAllow(int index, char *MACAddressControlList)
{
	ObjList	*dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if ((dbObjPtr == NULL) || (getInterfaceName(index, interfaceName) == UGW_FAILURE))
	{
		return UGW_FAILURE;
	}

	help_addObjList(dbObjPtr, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddressControlList", MACAddressControlList, 0, 0);

	setLog(interfaceName, dbObjPtr, 0);

	return fapi_wlan_sta_allow(interfaceName, dbObjPtr, 0);
}

int fapi_wlan_channel_switch_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_set(fapi_wlan_channel_switch, index, wlObj, flags);
}

int wlan_channelSwitch(int index, char *centerFrequency, char *waveVhtCenterFrequency, char *operatingChannelBandwidth)
{
	ObjList	*dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if ((dbObjPtr == NULL) || (getInterfaceName(index, interfaceName) == UGW_FAILURE))
	{
		return UGW_FAILURE;
	}

	help_addObjList(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "CenterFrequency", centerFrequency, 0, 0);
	
	if (strcmp(centerFrequency, "0"))  //centerFrequency != 0
	{
		HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "WaveVhtCenterFrequency", waveVhtCenterFrequency, 0, 0);
		HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.Radio", "OperatingChannelBandwidth", operatingChannelBandwidth, 0, 0);
	}

	setLog(interfaceName, dbObjPtr, 0);

	return fapi_wlan_channel_switch(interfaceName, dbObjPtr, 0);
}

int fapi_wlan_unassociated_devices_info_req_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_set(fapi_wlan_unassociated_devices_info_req, index, wlObj, flags);
}

int wlan_unassociatedDevicesInfoReq(int index, char *MACAddress, char *operatingChannelBandwidth, char *centerFrequency, char *waveVhtCenterFrequency)
{
	ObjList	*dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if ((dbObjPtr == NULL) || (getInterfaceName(index, interfaceName) == UGW_FAILURE))
	{
		return UGW_FAILURE;
	}

	help_addObjList(dbObjPtr, "Device.WiFi.AccessPoint.AssociatedDevice", 0, 0, 0, 0);
	HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress", MACAddress, 0, 0);

	help_addObjList(dbObjPtr, "Device.WiFi.Radio", 0, 0, 0, 0);
	HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.Radio", "OperatingChannelBandwidth", operatingChannelBandwidth, 0, 0);

	help_addObjList(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "CenterFrequency", centerFrequency, 0, 0);
	HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "WaveVhtCenterFrequency", waveVhtCenterFrequency, 0, 0);

	setLog(interfaceName, dbObjPtr, 0);

	return fapi_wlan_unassociated_devices_info_req(interfaceName, dbObjPtr, 0);
}

int fapi_wlan_restricted_channels_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_set(fapi_wlan_restricted_channels_set, index, wlObj, flags);
}

int wlan_restrictedChannelsSet(int index, char *channelList)
{
	ObjList	*dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if ((dbObjPtr == NULL) || (getInterfaceName(index, interfaceName) == UGW_FAILURE))
	{
		return UGW_FAILURE;
	}

	help_addObjList(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ChannelList", channelList, 0, 0);

	setLog(interfaceName, dbObjPtr, 0);

	return fapi_wlan_restricted_channels_set(interfaceName, dbObjPtr, 0);
}

int fapi_wlan_failsafe_channel_set_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_set(fapi_wlan_failsafe_channel_set, index, wlObj, flags);
}

int wlan_failsafeChannelSet(int index, char *channel, char *centerFrequency, char *operatingChannelBandwidth)
{
	ObjList	*dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char    interfaceName[MAX_LEN_PARAM_VALUE] = { '\0' };

	if ((dbObjPtr == NULL) || (getInterfaceName(index, interfaceName) == UGW_FAILURE))
	{
		return UGW_FAILURE;
	}

	help_addObjList(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
	HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "Channel", channel, 0, 0);
	if (strcmp(channel, "0"))  // Channel != '0'
	{
		HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "CenterFrequency", centerFrequency, 0, 0);
		HELP_EDIT_NODE(dbObjPtr, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "OperatingChannelBandwidth", operatingChannelBandwidth, 0, 0);
	}

	setLog(interfaceName, dbObjPtr, 0);

	return fapi_wlan_failsafe_channel_set(interfaceName, dbObjPtr, 0);
}

int fapi_wlan_sta_measurement_get_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_get(fapi_wlan_sta_measurement_get, index, wlObj, flags);
}

int fapi_wlan_vap_measurement_get_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_get(fapi_wlan_vap_measurement_get, index, wlObj, flags);
}

int fapi_wlan_radio_info_get_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_get(fapi_wlan_radio_info_get, index, wlObj, flags);
}

int fapi_wlan_acs_report_get_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_get(fapi_wlan_acs_report_get, index, wlObj, flags);
}

int fapi_wlan_failsafe_channel_get_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_get(fapi_wlan_failsafe_channel_get, index, wlObj, flags);
}

int fapi_wlan_restricted_channels_get_native(int index, ObjList *wlObj, unsigned int flags)
{
	return fapi_wlan_beerock_generic_get(fapi_wlan_restricted_channels_get, index, wlObj, flags);
}

/* End of Native FAPI - END */
