/******************************************************************************

                         Copyright (c) 2015
                        Lantiq Beteiligungs-GmbH & Co. KG

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

/*! \file 	fapi_wlan.c
    \brief 	This file implements the WLAN FAPI generic layer.
    \todo 	Add license header
	\todo 	return dll_uninit when issues of calling in loop will be solved
 */
 
#include <stdio.h>
#include <dlfcn.h> //For dynamic loading of shared libs
#include <stdlib.h>
#include "help_objlist.h"
#include "fapi_wlan.h"
#include "fapi_wlan_private.h"
#include "fapi_wlan_tools.h"

#ifndef YOCTO
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
#endif

fapi_wlan_vendor_ops_t * fapi_wlan_vendor_ops[FAPI_WLAN_MAX_VENDORS] = {NULL};
ObjList *mapperObjPtr = NULL;

/*! \brief LTQ Wave implementation of the initial vendor ops struct
	\todo Add one minimal struct for all possible supported vendors
	\todo TBD Do not use relative path ./ in filename - make sure .so files are in lib path

	This is a reference of a vendor implementation of the initial vendor ops struct.
	This init struct should contain only the vendor name and the vendor lib filename
	All init structs will be in the generic layer, so that it knows where to look for the library files.
	If a vendor is not supported in this distribution, it is acceptable that the vendor lib file won't exist.
	Another option that will provide more flexibility but slightly more complex design and implementation:
	Read the vendor names and .so filenames from a config file, and the vendor installer will append to this file.
 */
fapi_wlan_vendor_ops_t fapi_wlan_vendor_ops_init_wave = {
	.vendor_name = "Intel WAVE 5.x.x",
#if defined YOCTO
	.vendor_filename = "/usr/lib/libfapiwave.so",
#else
	.vendor_filename = "/opt/lantiq/lib/libfapiwave.so",
#endif
};

fapi_wlan_vendor_ops_t fapi_wlan_vendor_ops_init_qca = {
	.vendor_name = "QCA",
	.vendor_filename = "/opt/lantiq/lib/libfapiwlanqca.so",
};

fapi_wlan_vendor_ops_t fapi_wlan_vendor_ops_init_mtk = {
	.vendor_name = "MediaTek",
	.vendor_filename = "/opt/lantiq/lib/libfapiwlanmtk.so",
};

void dll_init(void) {
	void* vendor_handle;
	fapi_wlan_vendors_t i;
	fapi_wlan_vendor_ops_t* (*get_vendor_ops)();
	
	// logger("FAPI_WLAN_COMMON dll_init called \n");
	
	// All implemented vendor init structs must be listed here.
	fapi_wlan_vendor_ops[FAPI_WLAN_LTQ_WAVE] = &fapi_wlan_vendor_ops_init_wave;
	fapi_wlan_vendor_ops[FAPI_WLAN_QCA] = &fapi_wlan_vendor_ops_init_qca;
	fapi_wlan_vendor_ops[FAPI_WLAN_MEDIATEK] = &fapi_wlan_vendor_ops_init_mtk;

	for (i = FAPI_WLAN_LTQ_WAVE; i < FAPI_WLAN_MAX_VENDORS; i++) {
		if (fapi_wlan_vendor_ops[i] && fapi_wlan_vendor_ops[i]->vendor_filename) {

			vendor_handle = NULL;

			if (file_exists(fapi_wlan_vendor_ops[i]->vendor_filename)) {
				vendor_handle = dlopen(fapi_wlan_vendor_ops[i]->vendor_filename, RTLD_LAZY);
			}
			else {
				//logger("Warning: Skipping vendor '%s', unable to find so file '%s' \n", fapi_wlan_vendor_ops[i]->vendor_name, fapi_wlan_vendor_ops[i]->vendor_filename);
				continue;
			}

			if (!vendor_handle) {
				// handle failed  - probably vendor is not supported with this distribution
				// logger("Warning: Skipping vendor '%s', unable to open lib '%s' \n", fapi_wlan_vendor_ops[i]->vendor_name, fapi_wlan_vendor_ops[i]->vendor_filename);
				continue;
			}
#if 0
			else {
				logger("FAPI_WLAN_COMMON: vendor '%s', opened lib '%s' \n", fapi_wlan_vendor_ops[i]->vendor_name, fapi_wlan_vendor_ops[i]->vendor_filename);
			}
#endif

			// Load the common function that returns the vendor ops pointer
			get_vendor_ops = dlsym(vendor_handle, "get_vendor_ops");
			if (!get_vendor_ops) {
				logger("Warning: Skipping vendor '%s', unable to get initializing function \n", fapi_wlan_vendor_ops[i]->vendor_name);
				dlclose(vendor_handle);
				continue;
			}

			// Get the full struct from the vendor file and save in the global array, instead of the empty init struct
			fapi_wlan_vendor_ops[i] = get_vendor_ops();

			// Make sure the struct was read
			if (!fapi_wlan_vendor_ops[i]) {
				logger("Warning: Can't read fapi_wlan_vendor_ops struct \n");
				FAPI_WLAN_ASSERT(fapi_wlan_vendor_ops[i]);
				continue;
			}

			// Save the vendor handle inside the struct, it will be needed by uninit to free this memory
			fapi_wlan_vendor_ops[i]->vendor_handle = vendor_handle;
		}
	}
}

/*
void dll_uninit(void) {
	logger("FAPI_WLAN_COMMON dll_uninit called \n");
}
*/

int fapi_wlan_hw_init(ObjList *wlObj, unsigned int flags) {
	ObjList * tmpHwInitObj;
	ObjList *vendorsList = NULL;

	int numOfVendorRadios, numOfMovedEntries, radioNumOfEntries = 0;
	fapi_wlan_vendors_t i;
	char radioStr[MAX_LEN_PARAM_VALUE];

	fapi_wlan_trace_in(__FUNCTION__, "ALL_Interfaces", wlObj);

	vendorsList = HELP_CREATE_OBJ(SOPT_OBJVALUE);    // head initialization done here

	for (i = FAPI_WLAN_LTQ_WAVE; i < FAPI_WLAN_MAX_VENDORS; i++) {
		if (fapi_wlan_vendor_ops[i] && (fapi_wlan_vendor_ops[i]->fapi_wlan_hw_init)) {
			// Call the vendor init.	
			// It will probe to see if the HW exists and then insmod drivers and return created radio n interface names
			numOfVendorRadios = fapi_wlan_vendor_ops[i]->fapi_wlan_hw_init(wlObj, flags);
			
			if (numOfVendorRadios > 0) {
				numOfMovedEntries = 0;
				tmpHwInitObj = NULL;
				addMapObj(wlObj, DEVICE_RADIO, i, NULL);
				
				while (!HELP_MOVEOBJLIST(vendorsList, wlObj, DEVICE_RADIO, SOPT_OBJVALUE)) {
					numOfMovedEntries++;
				}

				FAPI_WLAN_ASSERT(numOfVendorRadios == numOfMovedEntries);

				// Delete Device.WiFi Object
				FOR_EACH_OBJ(wlObj, tmpHwInitObj) {
					if (!strcmp(GET_OBJ_NAME(tmpHwInitObj), DEVICE_WIFI_NAME)) {
						HELP_DELETE_CURRENT_OBJ(wlObj, GET_OBJ_NAME(tmpHwInitObj), tmpHwInitObj, SOPT_OBJVALUE);
						//	HELP_DELOBJPTR(wlObj, tmpHwInitObj);
					}
				}

				radioNumOfEntries += numOfVendorRadios;
				logger("FAPI_WLAN_COMMON INIT DONE '%s' VENDOR '%d' radioNumOfEntries '%d' \n", fapi_wlan_vendor_ops[i]->vendor_name, i, radioNumOfEntries);
			}
			else {
				logger("FAPI_WLAN_COMMON INIT skipped for '%s' \n", fapi_wlan_vendor_ops[i]->vendor_name);
				//todo return this code after code freeze
				//dlclose(fapi_wlan_vendor_ops[i]->vendor_handle);
				//fapi_wlan_vendor_ops[i]->vendor_handle == NULL;
			}
		}
	}

	help_addObjList(wlObj, DEVICE_WIFI_NAME, 0, 0, 0, 0);
	snprintf(radioStr, MAX_LEN_PARAM_VALUE, "%d", radioNumOfEntries);
	HELP_EDIT_NODE(wlObj, DEVICE_WIFI_NAME, RADIO_NUM_OF_ENTRIES, radioStr, 0, 0);

	// Merge temporary vendors list to main wlObj
	HELP_COPY_OBJ(wlObj, vendorsList, SOPT_OBJVALUE, COPY_COMPLETE_OBJ);

	HELP_DELETE_OBJ(vendorsList, SOPT_OBJVALUE, FREE_OBJLIST);

	fapi_wlan_trace_out(__FUNCTION__, "ALL_Interfaces", wlObj, UGW_SUCCESS);

	return UGW_SUCCESS;
}

int fapi_wlan_hw_uninit(ObjList *wlObj, unsigned int flags) {
	fapi_wlan_vendors_t vendor;
	int returnStatus = 0;

	fapi_wlan_trace_in(__FUNCTION__, "ALL_Interfaces", wlObj);

	for (vendor = FAPI_WLAN_LTQ_WAVE; vendor < FAPI_WLAN_MAX_VENDORS; vendor++) {
		if (fapi_wlan_vendor_ops[vendor]) {
			if (fapi_wlan_vendor_ops[vendor]->fapi_wlan_hw_uninit && fapi_wlan_vendor_ops[vendor]->vendor_handle)
			{
				returnStatus += fapi_wlan_vendor_ops[vendor]->fapi_wlan_hw_uninit(wlObj, flags);
			}
		}
	}

	fapi_wlan_trace_out(__FUNCTION__, "ALL_Interfaces", wlObj, returnStatus);

	return returnStatus;
}

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_capability_query);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_radio_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_radio_stats_query);

int fapi_wlan_ssid_add(const char *ifname, ObjList *wlObj, unsigned int flags) {
	int ret = UGW_SUCCESS;

	fapi_wlan_trace_in(__FUNCTION__, ifname, wlObj);

	fapi_wlan_vendors_t vendor = fapi_wlan_get_vendor(ifname);
	if ( (int)vendor > 0 ) {
		FAPI_WLAN_ASSERT(vendor < FAPI_WLAN_MAX_VENDORS  && fapi_wlan_vendor_ops[vendor] && fapi_wlan_vendor_ops[vendor]->fapi_wlan_ssid_add);

		if ( ! fapi_wlan_vendor_ops[vendor]->fapi_wlan_ssid_add(ifname, wlObj, flags) ) {
			ret = addMapObj(wlObj, DEVICE_SSID, vendor, ifname);
		}
		else {
			ret = UGW_FAILURE;
		}
	}
	else {
		ret = UGW_FAILURE;
	}

	fapi_wlan_trace_out(__FUNCTION__, ifname, wlObj, ret);

	return ret;
}

int fapi_wlan_ssid_delete(const char *ifname, ObjList *wlObj, unsigned int flags) {
	int ret = UGW_SUCCESS;
	fapi_wlan_vendors_t vendor = fapi_wlan_get_vendor(ifname);

	fapi_wlan_trace_in(__FUNCTION__, ifname, wlObj);

	if ( (int)vendor > 0 ) {
		FAPI_WLAN_ASSERT(vendor < FAPI_WLAN_MAX_VENDORS  && fapi_wlan_vendor_ops[vendor] && fapi_wlan_vendor_ops[vendor]->fapi_wlan_ssid_delete);

		if ( ! fapi_wlan_vendor_ops[vendor]->fapi_wlan_ssid_delete(ifname, wlObj, flags) ) {
			ret = deleteMapObj(ifname);
		}
		else {
			ret = UGW_FAILURE;
		}
	}
	else {
		ret = UGW_FAILURE;
	}

	fapi_wlan_trace_out(__FUNCTION__, ifname, wlObj, ret);

	return ret;
}

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_ssid_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_ssid_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_ssid_stats_query);

int fapi_wlan_ap_set(const char *ifname, ObjList *wlObj, unsigned int flags) {
	int ret = UGW_SUCCESS;
	fapi_wlan_vendors_t vendor = fapi_wlan_get_vendor(ifname);

	fapi_wlan_trace_in(__FUNCTION__, ifname, wlObj);

	if ( (int)vendor > 0 ) {
		FAPI_WLAN_ASSERT(vendor < FAPI_WLAN_MAX_VENDORS  && fapi_wlan_vendor_ops[vendor] && fapi_wlan_vendor_ops[vendor]->fapi_wlan_ap_set);

		if ( ! fapi_wlan_vendor_ops[vendor]->fapi_wlan_ap_set(ifname, wlObj, flags) ) {
			ret = updateMapObj(wlObj, ifname);
		}
		else {
			ret = UGW_FAILURE;
		}
	}
	else {
		ret = UGW_FAILURE;
	}

	fapi_wlan_trace_out(__FUNCTION__, ifname, wlObj, ret);

	return ret;
}

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_ap_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_security_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_security_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wps_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wms_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wps_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_assoc_devices_query);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_assoc_devices_stats_query);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wmm_sta_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wmm_sta_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wmm_sta_stats_query);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wmm_ap_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wmm_ap_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wmm_ap_stats_query);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wds_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_wds_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_hotspot_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_hotspot_get);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_endpoint_set);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_endpoint_wps_set);

DEFINE_FAPI_WLAN_EMPTY_FUNC(fapi_wlan_endpoint_stats_query);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_profile_delete);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_profile_query);

DEFINE_FAPI_WLAN_EMPTY_FUNC(fapi_wlan_wmm_endpoint_stats_query);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_add_to_bridge);

int fapi_wlan_factory_reset() {
	int ret = UGW_SUCCESS;
	fapi_wlan_vendors_t vendor_index;

	fapi_wlan_trace_in(__FUNCTION__, "ALL_Interfaces", NULL /* There's no meaning for this object */);

	for (vendor_index = FAPI_WLAN_LTQ_WAVE; vendor_index < FAPI_WLAN_MAX_VENDORS; vendor_index++) {
		if (fapi_wlan_vendor_ops[vendor_index] && (fapi_wlan_vendor_ops[vendor_index]->fapi_wlan_factory_reset) ) {
			/* Call the vendor init. */
			ret = fapi_wlan_vendor_ops[vendor_index]->fapi_wlan_factory_reset();
		}
	}

	fapi_wlan_trace_out(__FUNCTION__, "ALL_Interfaces", NULL /* There's no meaning for this object */, ret);

	return ret;
}
DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_up);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_down);

DEFINE_FAPI_WLAN_STANDARD_FUNC(fapi_wlan_hostapd_stop);

int fapi_wlan_beerock_init(ObjList *wlObj, unsigned int flags) {
	int ret = UGW_SUCCESS;
	fapi_wlan_vendors_t vendor_index;

	fapi_wlan_trace_in(__FUNCTION__, "ALL_Interfaces", NULL /* There's no meaning for this object */);

	for (vendor_index = FAPI_WLAN_LTQ_WAVE; vendor_index < FAPI_WLAN_MAX_VENDORS; vendor_index++) {
		if (fapi_wlan_vendor_ops[vendor_index] && (fapi_wlan_vendor_ops[vendor_index]->fapi_wlan_beerock_init) ) {
			/* Call the vendor init. */
			ret = fapi_wlan_vendor_ops[vendor_index]->fapi_wlan_beerock_init(wlObj, flags);
		}
	}

	fapi_wlan_trace_out(__FUNCTION__, "ALL_Interfaces", NULL /* There's no meaning for this object */, ret);

	return ret;
}

int fapi_wlan_object_printf(ObjList *wlObj, FILE *pFile)
{
	ObjList *currentObj;
	ParamList *currentParam;

	if (wlObj == NULL)
		return UGW_SUCCESS;

	FOR_EACH_OBJ(wlObj, currentObj)
	{
		fprintf(pFile, "OBJECT NAME: '%s'\n", GET_OBJ_NAME(currentObj));
		FOR_EACH_PARAM(currentObj, currentParam)
		{
			fprintf(pFile, "PARAM: '%s=%s'\n", GET_PARAM_NAME(currentParam), GET_PARAM_VALUE(currentParam));
		}
	}

	return UGW_SUCCESS;
}

void fapi_wlan_trace_in(const char *funcName, const char *ifname, ObjList *wlObj)
{
#if 1
	return;
#else
	FILE *pFile;

	if (LOGLEVEL <= 5)
		return;

	pFile = fopen("/tmp/log_fapi_in.txt", "a");
	fprintf(pFile, "\n%s Entry; ifname= '%s'\n", funcName, ifname);
	fapi_wlan_object_printf(wlObj, pFile);
	fclose(pFile);
#endif
}

void fapi_wlan_trace_out(const char *funcName, const char *ifname, ObjList *wlObj, int ret)
{
#if 1
	return;
#else
	FILE *pFile;
	if (LOGLEVEL <= 5)
		return;

	pFile = fopen("/tmp/log_fapi_out.txt", "a");
	fprintf(pFile, "\n%s Exit; ifname= '%s', ret= %d\n", funcName, ifname, ret);
	fapi_wlan_object_printf(wlObj, pFile);
	fclose(pFile);
#endif
}

int addMapObj(ObjList *wlObj, const char* ObjectName, fapi_wlan_vendors_t i, const char *ifname) {
	ObjList *newMapperObj = NULL;
	ObjList *tempLoopObj = NULL;

	ParamList *param;
	char *vapName = NULL;
	char sVendor[MAX_LEN_PARAM_VALUE];
	bool foundAtLeastOneName = false;

	FOR_EACH_OBJ(wlObj, tempLoopObj) {
		if (!strcmp(GET_OBJ_NAME(tempLoopObj), ObjectName)) {
			FOR_EACH_PARAM(tempLoopObj, param) {
				if (!strcmp(INTERFACE_NAME_FIELD, GET_PARAM_NAME(param))) {
					vapName = GET_PARAM_VALUE(param);
					FAPI_WLAN_ASSERT(vapName);

					newMapperObj = NULL;
					if (ifname) {
						newMapperObj = HELP_GETOBJPTR(mapperObjPtr, DB_IFNAME, vapName);
					}

					if (newMapperObj == NULL) {
						newMapperObj = help_addObjList(mapperObjPtr, WLAN_OBJECT_MAPPER, 0, 0, 0, 0);
					}

					if (!strcmp(ObjectName, DEVICE_RADIO)) {
						HELP_EDIT_SELF_NODE(newMapperObj, WLAN_OBJECT_MAPPER, DB_RADIO, vapName, 0, 0);
						HELP_EDIT_SELF_NODE(newMapperObj, WLAN_OBJECT_MAPPER, DB_IFNAME, vapName, 0, 0);
					}
					else {
						FAPI_WLAN_ASSERT(ifname);
						HELP_EDIT_SELF_NODE(newMapperObj, WLAN_OBJECT_MAPPER, DB_RADIO, (char *)ifname, 0, 0);
						HELP_EDIT_SELF_NODE(newMapperObj, WLAN_OBJECT_MAPPER, DB_IFNAME, vapName, 0, 0);
					}

					snprintf(sVendor, MAX_LEN_PARAM_VALUE, "%d", i);
					HELP_EDIT_SELF_NODE(newMapperObj, WLAN_OBJECT_MAPPER, DB_VENDOR, sVendor, 0, 0);
					logger("FAPI_WLAN_COMMON, addMapObj DB_NAME '%s' VENDOR '%s' vapName '%s' \n", WLAN_OBJECT_MAPPER, sVendor, vapName);

					foundAtLeastOneName = true;
				}
			}
		}
	}
	
	FAPI_WLAN_ASSERT(foundAtLeastOneName);
	return UGW_SUCCESS;
}

int updateMapObj(ObjList *wlObj, const char *ifname) {
	char *objName, *paramName, *enableFieldValue;
	objName = paramName = enableFieldValue = NULL;

	ObjList *objToUpdate;
	ParamList *param;

	FOR_EACH_OBJ(wlObj, objToUpdate) {
		objName = GET_OBJ_NAME(objToUpdate);
		if (!strcmp(objName, DEVICE_ACCESS_POINT)) {
			FOR_EACH_PARAM(objToUpdate, param) {
				paramName = GET_PARAM_NAME(param);
				if (!strcmp(paramName, "Enable")) {
					enableFieldValue = GET_PARAM_VALUE(param);
					logger("Device.WiFi.AccessPoint '%s' '%s' \n", paramName, enableFieldValue);
				}
			}
		}
	}

	if ( ! enableFieldValue ) {
		return UGW_SUCCESS;	
	}
	
	objToUpdate = HELP_GETOBJPTR(mapperObjPtr, DB_IFNAME, ifname);
	HELP_EDIT_SELF_NODE(objToUpdate, WLAN_OBJECT_MAPPER, DB_STATUS, enableFieldValue, 0, 0);
	return UGW_SUCCESS;
}

int deleteMapObj(const char *ifname) {
	ObjList *objToDel;
		
	objToDel = HELP_GETOBJPTR(mapperObjPtr, DB_IFNAME, ifname);
	FAPI_WLAN_ASSERT(objToDel);
	HELP_DELOBJPTR(mapperObjPtr, objToDel);
	return UGW_SUCCESS;
}

fapi_wlan_vendors_t fapi_wlan_get_vendor(const char* ifname) {
	ObjList *ifnameObjPtr;
	ParamList *vendorParam;
	char *paramName = NULL, *paramValue = NULL;
	int vendor;
	
	FAPI_WLAN_ASSERT(ifname);
	ifnameObjPtr = HELP_GETOBJPTR(mapperObjPtr, DB_IFNAME, ifname);
	
	if ( ! ifnameObjPtr ) {
		logger("FAPI_WLAN_COMMON GET_VENDOR WARNING!!! could not find interface '%s'. \n", ifname);
		return UGW_FAILURE;	
	}
	
	FOR_EACH_PARAM(ifnameObjPtr, vendorParam) {
		paramName = GET_PARAM_NAME(vendorParam);
		if ( ! strcmp(paramName, DB_VENDOR) ) {
			paramValue = GET_PARAM_VALUE(vendorParam);
		}
	}
	
	FAPI_WLAN_ASSERT(paramValue);
	vendor = atoi(paramValue);
	FAPI_WLAN_ASSERT(vendor > 0);
	
	return (fapi_wlan_vendors_t)vendor;
}

#ifdef FILE_EXISTS
int file_exists(const char *fname) {
	FILE *file;
	if (file = fopen(fname, "r"))
	{
		fclose(file);
		return 1;
	}
	return 0;
}
#endif

int debug_obj(ObjList *debug_obj) {
	ObjList * currentObj;
	ParamList * currentParam;

	FOR_EACH_OBJ(debug_obj, currentObj) {
		printf("OBJECT NAME: '%s' \n", GET_OBJ_NAME(currentObj));
		FOR_EACH_PARAM(currentObj, currentParam) {
			printf("PARAM: '%s=%s' \n", GET_PARAM_NAME(currentParam), GET_PARAM_VALUE(currentParam));
		}
	}
	return UGW_SUCCESS;
}

int fapiWlanFailSafeLoad(void) {
	int return_status = 0;
	char fapi_db_path[MAX_LEN_PARAM_VALUE];

	if (!mapperObjPtr) {
		mapperObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);    // head initialization done here
	}
	else
	{
		return UGW_SUCCESS;
	}

	if (file_exists(FAPI_RPC_MODE)){
		sprintf(fapi_db_path, "%s%s", DB_PATH, WLAN_OBJECT_MAPPER);
	} else {
		sprintf(fapi_db_path, "%s%s", "/tmp/", WLAN_OBJECT_MAPPER);
	}
	
	// Load the DB from FLASH to RAM	
	if (file_exists(fapi_db_path)) {
		return_status = HELP_LOADLOCALDB(mapperObjPtr, fapi_db_path);
	}
	else {
		logger("FAPI_WLAN_COMMON, HELP_LOADLOCALDB couldn't load from path '%s' STATUS '%d' \n", fapi_db_path, return_status);
		return UGW_FAILURE;
	}

	return return_status;
}


int fapiWlanFailSafeStore(void) {
	char command[MAX_LEN_PARAM_VALUE];
	char fapi_db_path[MAX_LEN_PARAM_VALUE];
	int return_status = 0;

	if (!mapperObjPtr)
	{  // Could happen in case of calling this function twice in a raw, or without calling fapiWlanFailSafeLoad() first
		return UGW_FAILURE;
	}

	if (file_exists(FAPI_RPC_MODE)){
		sprintf(fapi_db_path, "%s%s", DB_PATH, WLAN_OBJECT_MAPPER);
	} else {
		sprintf(fapi_db_path, "%s%s", "/tmp/", WLAN_OBJECT_MAPPER);
	}

	sprintf(command, "rm -f %s", fapi_db_path);
	system(command);

	return_status += HELP_STORELOCALDB(mapperObjPtr, fapi_db_path);
	HELP_DELETE_OBJ(mapperObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);
	return return_status;
}

void setLog(const char* ifname, ObjList *wlObj, int logIndex)
{
	char logFile[] = "/tmp/log_0_wlan999";
	
	sprintf(logFile, "/tmp/log_%d_%s", logIndex, ifname);
	debug_obj(wlObj);
}
