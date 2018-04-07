/*
 *	ble data encrypt
*/

#include "bleencrypt.h"

#ifdef ENCRYPT
#define SSL_VERSION     (SSLeay_version(SSLEAY_VERSION))
#else
#define SSL_VERSION     ""
#endif

struct cmd_handler_svr cmd_handlers_svr[] = {
	{ "reqpublickey",	BLE_COMMAND_REQ_PUBLICKEY,		UnpackBLECommandReq,			PackBLEResponseReqPublicKey },
	{ "reqservernonce",	BLE_COMMAND_REQ_SERVERNONCE,		UnpackBLECommandReqServerNonce,		PackBLEResponseReqServerNonce },
	{ "apply",		BLE_COMMAND_APPLY,			UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "reset",		BLE_COMMAND_RESET,			UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "getwanstatus",	BLE_COMMAND_GET_WAN_STATUS,		UnpackBLECommandReq,			PackBLEResponseGetWanStatus },
	{ "getwifistatus",	BLE_COMMAND_GET_WIFI_STATUS,		UnpackBLECommandReq,			PackBLEResponseGetWifiStatus },
	{ "setwantype",		BLE_COMMAND_SET_WAN_TYPE,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setwanpppoename",	BLE_COMMAND_SET_WAN_PPPOE_NAME,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setwanpppoepwd",	BLE_COMMAND_SET_WAN_PPPOE_PWD,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setwanipaddr",	BLE_COMMAND_SET_WAN_IPADDR,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setwanipsubnetmask",	BLE_COMMAND_SET_WAN_SUBNET_MASK,	UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setwandefgateway",	BLE_COMMAND_SET_WAN_GATEWAY,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setwandns1",		BLE_COMMAND_SET_WAN_DNS1,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setwandns2",		BLE_COMMAND_SET_WAN_DNS2,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setlanport",		BLE_COMMAND_SET_WAN_PORT,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setwifiname",	BLE_COMMAND_SET_WIFI_NAME,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setwifipwd",		BLE_COMMAND_SET_WIFI_PWD,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setgroupid",		BLE_COMMAND_SET_GROUP_ID,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setadminname",	BLE_COMMAND_SET_ADMIN_NAME,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setadminpwd",	BLE_COMMAND_SET_ADMIN_PWD,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setuserlocation",	BLE_COMMAND_SET_USER_LOCATION,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setuserplace",	BLE_COMMAND_SET_USER_PLACE,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setswmode",		BLE_COMMAND_SET_SW_MODE,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "setwandnsenable",	BLE_COMMAND_SET_WAN_DNS_ENABLE,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ "getmacbleversion",	BLE_COMMAND_GET_MAC_BLE_VERSION,	UnpackBLECommandReq,			PackBLEResponseGetMacBleVersion },
#if defined(RTCONFIG_QCA)
	{ "getath1chan",	BLE_COMMAND_GET_ATH1_CHAN,		UnpackBLECommandReq,			PackBLEResponseGetAth1Chan },
#endif
	{ "setath1chan",	BLE_COMMAND_SET_ATH1_CHAN,		UnpackBLEDataToNvram,			PackBLEResponseOnly },
	{ NULL,			-1,					NULL,					NULL }
};

struct param_handler_svr param_handlers_svr[] = {
	{ BLE_COMMAND_REQ_PUBLICKEY,		NULL,				NULL,				BLE_DATA_TYPE_NULL },
	{ BLE_COMMAND_REQ_SERVERNONCE,		NULL,				NULL,				BLE_DATA_TYPE_NULL },
	{ BLE_COMMAND_APPLY,			NULL,				NULL,				BLE_DATA_TYPE_NULL },
	{ BLE_COMMAND_RESET,			NULL,				NULL,				BLE_DATA_TYPE_NULL },
	{ BLE_COMMAND_GET_WAN_STATUS,		NULL,				NULL,				BLE_DATA_TYPE_NULL },
	{ BLE_COMMAND_GET_WIFI_STATUS,		NULL,				NULL,				BLE_DATA_TYPE_NULL },
	{ BLE_COMMAND_SET_WAN_TYPE,		"wan_proto",			NULL,				BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_WAN_PPPOE_NAME,	"wan_pppoe_username",		"restart_wan_if 0",		BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_WAN_PPPOE_PWD,	"wan_pppoe_passwd",		"restart_wan_if 0",		BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_WAN_IPADDR,		"wan_ipaddr_x",			"restart_wan_if 0",		BLE_DATA_TYPE_IP },
	{ BLE_COMMAND_SET_WAN_SUBNET_MASK,	"wan_netmask_x",		"restart_wan_if 0",		BLE_DATA_TYPE_IP },
	{ BLE_COMMAND_SET_WAN_GATEWAY,		"wan_gateway_x",		"restart_wan_if 0",		BLE_DATA_TYPE_IP },
	{ BLE_COMMAND_SET_WAN_DNS1,		"wan_dns1_x",			"restart_wan_if 0",		BLE_DATA_TYPE_IP },
	{ BLE_COMMAND_SET_WAN_DNS2,		"wan_dns2_x",			"restart_wan_if 0",		BLE_DATA_TYPE_IP },
	{ BLE_COMMAND_SET_WAN_PORT,		"bt_wanport",			NULL,				BLE_DATA_TYPE_INTEGER },
	{ BLE_COMMAND_SET_WIFI_NAME,		"wlc_ssid",			"restart_wireless",		BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_WIFI_PWD,		"wlc_wpa_psk",			"restart_wireless",		BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_GROUP_ID,		"cfg_group",			"restart_cfgsync",		BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_ADMIN_NAME,		"http_username",		"chpass restart_ftpsamba",	BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_ADMIN_PWD,		"http_passwd",			"chpass restart_ftpsamba",	BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_USER_LOCATION,	"cfg_alias",			NULL,				BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_USER_PLACE,		"bt_user_place",		NULL,				BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_SW_MODE,		"sw_mode",			"ble_qis_done",			BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_SET_WAN_DNS_ENABLE,	"wan_dnsenable_x",		"restart_wan_if 0",		BLE_DATA_TYPE_STRING },
	{ BLE_COMMAND_GET_MAC_BLE_VERSION,	NULL,				NULL,				BLE_DATA_TYPE_NULL },
	{ BLE_COMMAND_GET_ATH1_CHAN,		NULL,				NULL,				BLE_DATA_TYPE_NULL },
	{ BLE_COMMAND_SET_ATH1_CHAN,		NULL,				"restart_wireless",             BLE_DATA_TYPE_STRING },
	{ -1,					NULL,				NULL,				-1 }
};

static char* dumpHEX(
	unsigned char *src,
	unsigned long src_size)
{
	int c;
	unsigned char *s = NULL, *ss = NULL;
	char *P = NULL, *PP = NULL, sss[33];
	unsigned long alloc_size = 0;
	
	if (src == NULL || src_size <= 0)
	{
		printf("\n");
		return NULL;
	}
	
	alloc_size = src_size * 4;
	s = &src[0];
	ss = &src[src_size];
	P = (char *)malloc(alloc_size);
	
	if (P == NULL)
	{
		printf("\n");
		return NULL;
	}
	
	memset(P, 0, alloc_size);
	for (c=0, PP=&P[0]; s<ss; s++)
	{
		memset(sss, 0, sizeof(sss));
		snprintf(sss, sizeof(sss)-1, "%02X%c", *s, (c>=19)?'\x0a':'\x20');
		strncpy(PP, sss, strlen(sss));
		PP += strlen(sss);
		if (c++>=19) c = 0;	
	}
	printf("%s\n", P);
	free(P);
	return NULL;	
}

/*
 * @func: Check if the file exists
 * @FileName: search the name of file.
 *
 * @return: 1=exist, 0=no file
 * */
static int fileExists(char *FileName)
{
	int ret = 0;
	FILE *stream = NULL;

	stream = fopen(FileName, "r");
	ret = (stream == NULL) ? 0 : 1;
	if (ret == 1) fclose(stream);
	return ret;
}

void print_data_topic(char *topic, int length)
{
	char str_tmp[MAX_PACKET_SIZE];
	int index;

	memset(str_tmp, '\0', MAX_PACKET_SIZE);
	snprintf(str_tmp, sizeof(str_tmp), "%s[%4s] ", str_tmp, "");

	for(index=0; index<MAX_LE_DATALEN; index++)
			snprintf(str_tmp, sizeof(str_tmp), "%s%2d%c", str_tmp, index, ' ');

	if(GATTDATABASE_DEBUG)
		printf("\n[%s total data length]: %d\n%s\n", topic, length, str_tmp);

	logmessage("BLUEZ", "[%s total data length]: %d\n", topic, length);
	logmessage("BLUEZ", "%s\n", str_tmp);

	return;
}

/*
 * @func: printf uint8 value 
 * @value:
 * @val_len:
 * @row_len:
 * @val_index:
 * @flag:
 *
 * @return:
 * */
void print_data_info(uint8_t *value, int val_len, int row_len, int val_index, int flag)
{
	uint8_t *val_tmp=malloc(MAX_PACKET_SIZE);
	char str_tmp[MAX_PACKET_SIZE];
	int index;

	memset(val_tmp, '\0', val_len);
	memset(str_tmp, '\0', MAX_PACKET_SIZE);
	memcpy(val_tmp, value, val_len);

	if (flag == 1)
		snprintf(str_tmp, sizeof(str_tmp), "Data value:\n%s\n", str_tmp);
	else
		snprintf(str_tmp, sizeof(str_tmp), "[%4d] ", val_index);

	for(index=0; index<val_len; index++) if (flag == 1)
			snprintf(str_tmp, sizeof(str_tmp), "%s%02x%c", str_tmp, val_tmp[index], (((index+1)%row_len)==0 && index!=0)?'\n':' ');
		else
			snprintf(str_tmp, sizeof(str_tmp), "%s%02x%c", str_tmp, val_tmp[index], ' ');

	if(GATTDATABASE_DEBUG)
		printf("%s\n", str_tmp);
	logmessage("BLUEZ", "%s\n", str_tmp);

	free(val_tmp);

	return;
}

/*
 * @func: server and client key init/reset action 
 * @type: Server or Client
 * @action: do Init or Reset
 *
 * @return:
 * */
void ble_key_act(char *type,  char *action)
{
	int typenum = !strcmp(type, "Server")? 1 : !strcmp(type, "Client")? 0: 2;
	int actionnum = !strcmp(action, "Init")? 1 : !strcmp(action, "Reset")? 0: 2;

	if (fileExists(DEFAULT_PUBLIC_PEM_FILE) == 0) {
		printf("Not found file %s \n", DEFAULT_PUBLIC_PEM_FILE);
		goto err;
	}

	if (fileExists(DEFAULT_PRIVATE_PEM_FILE) == 0) {
		printf("Not found file %s]n", DEFAULT_PRIVATE_PEM_FILE);
		goto err;
	}

	switch (typenum) {
	case 0:
		goto err;
	case 1:
		if (actionnum == 0)
			KeyReset_S();
		else if (actionnum == 1)
			KeyInit_S(DEFAULT_PUBLIC_PEM_FILE, DEFAULT_PRIVATE_PEM_FILE);
		else
			goto err;
	}

err:
	return ;
}

int ble_encrypt_svr(unsigned char *input, unsigned char *output, size_t input_len)
{
	unsigned char *pdu=malloc(MAX_PACKET_SIZE);
	unsigned char *data=malloc(MAX_PACKET_SIZE);
	unsigned int datalen;
	int ret, cmdno, pdulen=0;
	struct cmd_handler_svr *handler;
	struct param_handler_svr *param_hander=param_handlers_svr;

	memset(pdu, '\0', MAX_PACKET_SIZE);
	memset(data, '\0', MAX_PACKET_SIZE);

	pdulen = (int)input_len;
	memcpy(pdu, input, input_len);

	ret=UnpackBLECommandData(pdu, pdulen, &cmdno, data, &datalen);

	if(BLEENCRYPT_DEBUG) {
		printf("o---v\n");
		printf("UnPack info:\n");
		dumpHEX(data, datalen);
	}

	if(ret==BLE_RESULT_OK) {
		if(cmdno==BLE_COMMAND_GET_NVRAM) {
			UnPackBLEExceptionGetNvram(cmdno, BLE_RESULT_OK, data, datalen, (unsigned char *)pdu, &pdulen);
		}
		else {
			for(handler = &cmd_handlers_svr[0]; handler->command; handler++, param_hander++)
				if(handler->cmdno==cmdno) break;

			if(handler->command!=NULL) {
				handler->unpack(param_hander, data, datalen);
				handler->pack(cmdno, BLE_RESULT_OK, (unsigned char *)pdu, &pdulen);
			}
		}
	}
	else { // error
		PackBLEResponseData(cmdno, ret, NULL, 0, (unsigned char *)pdu, &pdulen, BLE_RESPONSE_FLAGS);
	}

	if(BLEENCRYPT_DEBUG) {
		printf("Pack info:\n");
		dumpHEX(pdu, pdulen);
	}

	memcpy(output, pdu, pdulen);
	free(pdu);
	free(data);

	return pdulen;
}
