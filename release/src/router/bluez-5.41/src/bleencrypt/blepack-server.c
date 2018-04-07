#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "blepack.h"
#include "include/adv_string.h"
#include "utility.h"
#include <shutils.h>

static char do_rc_service[DEF_LEN_256];

typedef struct TLV_Header_t
{
        unsigned int len;
} __attribute__((__packed__)) TLV_Header;

static unsigned long getFileSize(char *FileName)
{
	unsigned long length = 0, curpos = 0;
	FILE *stream = NULL;

	if ((stream = fopen(FileName, "rb")) == NULL)
		return length;

	curpos = ftell(stream);
	fseek(stream, 0L, SEEK_END);
	length = ftell(stream);
	fseek(stream, curpos, SEEK_SET);
	fclose(stream);
	return length;
}

// Key Exchange Related function

// KeyReset: Reset myData structure
// KeyInit: read public/private key from file
KeyData_s keyData_s;

void KeyInit_S(char *public_key, char *private_key)
{
	FILE *hFile;

	memset((void *)&keyData_s, 0, sizeof(KeyData_s));

	if(!public_key||!private_key)
		return;

	if ((keyData_s.ku_len = getFileSize(public_key)) <= 0)
	{
		printf("Failed\ncheck public_key PEM file size failed, size : %zu\n", keyData_s.ku_len);
		return;
	}

	if ((keyData_s.ku = (unsigned char *)malloc(keyData_s.ku_len)) == NULL)
	{
		printf("Failed\nMemory allocate failed ...\n");
		return;
	}

	if ((hFile = fopen(public_key, "rb")) == NULL)
	{
		printf("Failed\n");
		return;
	}

	fseek(hFile, 0L, SEEK_SET);
	memset(keyData_s.ku, 0, keyData_s.ku_len);
	if (fread(keyData_s.ku, 1, keyData_s.ku_len, hFile) != keyData_s.ku_len)
	{
		printf("Failed\n");
		goto exit;
	}
	fclose(hFile);
	hFile = NULL;

	if ((keyData_s.kp_len = getFileSize(private_key)) <= 0)
	{
		printf("Failed\ncheck private_key PEM file size failed, size : %zu\n", keyData_s.kp_len);
		return;
	}

	if ((keyData_s.kp = (unsigned char *)malloc(keyData_s.kp_len)) == NULL)
	{
		printf("Failed\nMemory allocate failed ...\n");
		return;
	}

	if ((hFile = fopen(private_key, "rb")) == NULL)
	{
		printf("Failed\n");
		return;
	}

	fseek(hFile, 0L, SEEK_SET);
	memset(keyData_s.kp, 0, keyData_s.kp_len);
	if (fread(keyData_s.kp, 1, keyData_s.kp_len, hFile) != keyData_s.kp_len)
	{
		printf("Failed\n");
		goto exit;
	}
exit:
	fclose(hFile);
}

void KeyReset_S()
{
	if (!IsNULL_PTR(keyData_s.kp)) MFREE(keyData_s.kp);
	if (!IsNULL_PTR(keyData_s.ku)) MFREE(keyData_s.ku);
	if (!IsNULL_PTR(keyData_s.km)) MFREE(keyData_s.km);
	if (!IsNULL_PTR(keyData_s.ns)) MFREE(keyData_s.ns);
	if (!IsNULL_PTR(keyData_s.nc)) MFREE(keyData_s.nc);
	if (!IsNULL_PTR(keyData_s.ks)) MFREE(keyData_s.ks);
	if (!IsNULL_PTR(keyData_s.iv)) MFREE(keyData_s.iv);
}

/*Function: UnpackBLEDataToNvram
 *Parameter:
 * @param_handler: 
 * @data: input data
 * @datalen: size of data
 *
 * @return:
 * */
void UnpackBLEDataToNvram(struct param_handler_svr *param_handler, unsigned char *data, int datalen)
{
	char *str_data=malloc(DEF_LEN_128);
	char word[DEF_LEN_128*2], tmp[DEF_LEN_128], *next;
	char prefix[DEF_LEN_128];
	char *delim=";", *str_service, *delim_1=",";
	char countryCode[3];
	int unit=0, chk_service=0, list=0;
	int is_change_lanip;
	struct in_addr lan_addr, dhcp_start_addr, dhcp_end_addr;
	
	memset(str_data, '\0', DEF_LEN_128);
	memset(prefix, '\0', DEF_LEN_128);
	memset(tmp, '\0', DEF_LEN_128);

	//the data type of conversion
	switch (param_handler->t_type)
	{
		case BLE_DATA_TYPE_STRING:
			snprintf(str_data, datalen+1, "%s", data);
			break;
		case BLE_DATA_TYPE_INTEGER:
			snprintf(str_data, datalen+1, "%d", (int)data[0]);
			break;
		case BLE_DATA_TYPE_IP:
			AdvInet_NCtoA(data, str_data);
			break;
		case BLE_DATA_TYPE_NULL:
			break;
		default:
			free(str_data);
			return;
	}

	//store service 
	if (param_handler->do_rc_service!=NULL && strlen(param_handler->do_rc_service) && strstr(do_rc_service, param_handler->do_rc_service)==NULL)
	{
		if(strlen(do_rc_service))
			strcat(do_rc_service, delim);
		strncat(do_rc_service, param_handler->do_rc_service, strlen(param_handler->do_rc_service)+1);
	}

	//set nvram
	if (param_handler->nvram!=NULL && strlen(param_handler->nvram))
		nvram_set(param_handler->nvram, str_data);

	if (BLEPACKET_DEBUG)
	{
		printf("[rc service]: %s \n", param_handler->do_rc_service!=NULL?param_handler->do_rc_service:"(NULL)");
		printf("[Nvram parameter]: %s \n", param_handler->nvram!=NULL?param_handler->nvram:"(NULL)");
		printf("[Store value]: %s \n", strlen(str_data)?str_data:"(NULL)");
	}
	
	//other Setting
	switch (param_handler->cmdno)
	{
		case BLE_COMMAND_SET_WIFI_NAME:
		case BLE_COMMAND_SET_WIFI_PWD: /* Setting wifi parameter*/
			foreach (word, nvram_safe_get("wl_ifnames"), next) {
				snprintf(prefix, sizeof(prefix), "wl%d_", unit);

				if (param_handler->cmdno==BLE_COMMAND_SET_WIFI_NAME)
					nvram_set(strcat_r(prefix, "ssid", tmp), str_data);
				else if (param_handler->cmdno==BLE_COMMAND_SET_WIFI_PWD)
					nvram_set(strcat_r(prefix, "wpa_psk", tmp), str_data);

				nvram_set(strcat_r(prefix, "auth_mode_x", tmp), "psk2");
				nvram_set(strcat_r(prefix, "crypto", tmp), "aes");
				if (nvram_match("sw_mode", "1"))
					nvram_set(strcat_r(prefix, "channel", tmp), "0");
				unit++;
			}
#ifdef RTCONFIG_LANTIQ
			nvram_set_int("wave_flag", WAVE_FLAG_QIS);
#endif

			break;
		case BLE_COMMAND_SET_WAN_PORT:
				notify_rc_and_wait("restart_wan_if 0");
			break;
		case BLE_COMMAND_SET_WAN_TYPE:
			if(!strncmp(str_data, "static", sizeof(str_data)))
			{
				nvram_set("wan_dhcpenable_x", "0");
				nvram_set("wan_nat_x", "1");
				nvram_set("wan0_dhcpenable_x", "0");
				nvram_set("wan0_nat_x", "1");
			}
		case BLE_COMMAND_SET_WAN_PPPOE_NAME:
		case BLE_COMMAND_SET_WAN_PPPOE_PWD:
		case BLE_COMMAND_SET_WAN_IPADDR:
		case BLE_COMMAND_SET_WAN_SUBNET_MASK:
		case BLE_COMMAND_SET_WAN_GATEWAY:
		case BLE_COMMAND_SET_WAN_DNS_ENABLE:
		case BLE_COMMAND_SET_WAN_DNS1:
		case BLE_COMMAND_SET_WAN_DNS2:
			strncpy(prefix, param_handler->nvram, strlen(param_handler->nvram)); 
			AdvSplit_CombineStr(prefix, "_", "wan", "0", tmp);
			nvram_set(tmp, str_data);
			break;
		case BLE_COMMAND_SET_ADMIN_NAME:
			snprintf(tmp, sizeof(tmp), "%s>%s", str_data, nvram_safe_get("http_passwd"));
			nvram_set("acc_list",tmp);
			break;
		case BLE_COMMAND_SET_ADMIN_PWD:
			snprintf(tmp, sizeof(tmp), "%s>%s", nvram_safe_get("http_username"), str_data);
			nvram_set("acc_list",tmp);
			break;
		case BLE_COMMAND_SET_ATH1_CHAN:
			unit = 1;

			memset(countryCode, 0, sizeof(countryCode));

			snprintf(prefix, sizeof(prefix), "wl%d_", unit);
			strncpy(countryCode, nvram_safe_get(strcat_r(prefix, "country_code", tmp)), 2);

			list = get_channel_list_via_driver(unit, word, sizeof(word));
			if (list<=0 && countryCode[0] != 0xff && countryCode[1] != 0xff) {   // 0xffff is default
				list = get_channel_list_via_country(unit, countryCode, word, sizeof(word));
			}

			if (list>0) {
				list=0;
				next = strtok(word, delim_1);
				while (next!=NULL) {
					if (!strncmp(next, str_data, strlen(next))) {
						list=1;
						break;
					}
					next = strtok(NULL, delim_1);
				}

				if (list)
					nvram_set(strcat_r(prefix, "channel", tmp), str_data);
			}
			break;
		case BLE_COMMAND_SET_SW_MODE:
		case BLE_COMMAND_SET_GROUP_ID:
		case BLE_COMMAND_SET_USER_PLACE:
		case BLE_COMMAND_SET_USER_LOCATION:
		case BLE_COMMAND_RESET:
			break;
		case BLE_COMMAND_APPLY:
			is_change_lanip = 0;
#ifdef RTCONFIG_LANTIQ
			while(nvram_get_int("wave_ready") == 0){
				fprintf(stderr, "[BLE] wireless not ready, waiting...\n");
				sleep(5);
			}
			nvram_set_int("wave_action", 3);
#endif
			if (nvram_match("sw_mode", "3"))
			{

				if (nvram_match("x_Setting", "0"))
				{
					nvram_set("lan_proto", "dhcp");
					nvram_set("lan_dnsenable_x", "1");
					chk_service = 3;
				}
				nvram_set("hive_re_autoconf", "1");
				nvram_unset("cfg_master");
			}
			else // router mode
			{
				//////// IP conflict detection
				int wan_state, wan_sbstate, wan_auxstate;
				char *lan_ipaddr, *lan_netmask;
				char *wan_ipaddr, *wan_netmask;
				in_addr_t lan_mask, tmp_ip;


				wan_state = nvram_get_int("wan0_state_t");
				wan_sbstate = nvram_get_int("wan0_sbstate_t");
				wan_auxstate = nvram_get_int("wan0_auxstate_t");
				if (wan_state == 4 && wan_sbstate == 4 && wan_auxstate == 0)
				{
					lan_ipaddr = nvram_safe_get("lan_ipaddr");
					lan_netmask = nvram_safe_get("lan_netmask");

					wan_ipaddr = nvram_safe_get("wan0_ipaddr");
					wan_netmask = nvram_safe_get("wan0_netmask");

					if (inet_deconflict(lan_ipaddr, lan_netmask, wan_ipaddr, wan_netmask, &lan_addr)) {
						printf("[IP conflict]: change lan IP to %s\n", inet_ntoa(lan_addr));
						lan_mask = inet_network(lan_netmask);
						tmp_ip = ntohl(lan_addr.s_addr);
						dhcp_start_addr.s_addr = htonl(tmp_ip + 1);
						dhcp_end_addr.s_addr = htonl((tmp_ip | ~lan_mask) & 0xfffffffe);
						is_change_lanip = 1;
					}
				}
				nvram_set("cfg_master", "1");
			}

			if(strlen(do_rc_service))
			{
				/* do rc_service */
				if (strstr(do_rc_service, "ble_qis_done"))
				{
					if (chk_service==3)
					{
						if (strstr(do_rc_service, "chpass"))
						{
							notify_rc_and_wait("chpass");
							notify_rc_and_wait("restart_ftpsamba");
						}
 
						memset(do_rc_service, '\0', DEF_LEN_256);
						if (!strlen(do_rc_service))
							snprintf(do_rc_service, sizeof(do_rc_service), "%s%s", do_rc_service, "ble_qis_done");
						else
							snprintf(do_rc_service, sizeof(do_rc_service), "%s%s%s", do_rc_service, delim, "ble_qis_done");
					}
					else
						chk_service = 1;
#ifdef RTCONFIG_LANTIQ
					nvram_set("x_Setting", "1");
					notify_rc_and_wait("chpass");
#endif
				}
				else
				{
					str_service = strtok(do_rc_service, delim);
					while (str_service!=NULL)
					{
						if (BLEPACKET_DEBUG) printf("[rc do service]: %s \n", str_service);

						notify_rc_and_wait(str_service);
						str_service = strtok(NULL, delim);
					}
				}

				if (chk_service)
				{
					if ( chk_service == 1 )
					{
#ifdef RTCONFIG_BWDPI
#ifndef RTCONFIG_LANTIQ
						nvram_set("wrs_protect_enable", "1");
						nvram_set("wrs_mals_t", "0");
						nvram_set("wrs_cc_t", "0");
						nvram_set("wrs_vp_t", "0");
						nvram_set("bwdpi_db_enable", "1");
						nvram_set("apps_analysis", "1");
						nvram_set("TM_EULA", "1");
#endif
#endif
					} else {
						eval("iwconfig", "ath1", "channel", nvram_safe_get("wl1_channel"));

						eval("modprobe", "-r", "shortcut_fe_cm");
						eval("modprobe", "-r", "shortcut_fe_ipv6");
						eval("modprobe", "-r", "shortcut_fe");
					}

					if ( is_change_lanip )
					{
						nvram_set("lan_ipaddr", inet_ntoa(lan_addr));
						nvram_set("dhcp_start", inet_ntoa(dhcp_start_addr));
						nvram_set("dhcp_end", inet_ntoa(dhcp_end_addr));
					}

					if ( is_change_lanip )
					{
						logmessage("BLUEZ", "[IP conflict]: restart_net_and_phy\n");
						snprintf(do_rc_service, sizeof(do_rc_service), "%s%s%s", do_rc_service, delim, "restart_net_and_phy");
					}

					if(chk_service==1)
					{
						snprintf(do_rc_service, sizeof(do_rc_service), "%s%s%s", do_rc_service, delim, "start_hyfi_process");
#ifdef RTCONFIG_BWDPI
#ifndef RTCONFIG_LANTIQ
						snprintf(do_rc_service, sizeof(do_rc_service), "%s%s%s", do_rc_service, delim, "restart_wrs");
#endif
#endif
						snprintf(do_rc_service, sizeof(do_rc_service), "%s%s%s", do_rc_service, delim, "restart_firewall");
					}
					else if(chk_service==3)
						snprintf(do_rc_service, sizeof(do_rc_service), "%s%s%s", do_rc_service, delim, "restart_allnet");

					logmessage("BLUEZ", "[service]: %s\n", do_rc_service);
					nvram_set("bt_turn_off_service", do_rc_service);
				}
			}
			memset(do_rc_service, '\0', DEF_LEN_256);

			break;
		default:
			break;
	}

	free(str_data);
}

// Server Receive Command
//
// UnpackBLECommandData				
// UnpackBLECommandReq				:
// UnpackBLECommandReqNonce			: Send command to get nonce
//
int UnpackBLECommandData(unsigned char *pdu, int pdulen, int *cmdno, unsigned char *data, unsigned int *datalen)
{
	BLE_CHUNK_T *chunk;

	unsigned char payload[MAX_PACKET_SIZE];
	unsigned char *final_data;
	unsigned short payloadlen;
	unsigned short payloadleft;
	unsigned short crc16;
	unsigned short chunkcount;
	unsigned short i;
	unsigned short size, offset;
	size_t final_datalen;

	chunk=(BLE_CHUNK_T *)pdu;
	crc16 = ntohs(chunk->u.firstcmd.csum);
	chunk->u.firstcmd.csum=htons(0);

	if(Adv_CRC16(pdu, pdulen)!=crc16)
		return BLE_RESULT_CHECKSUM_INVALID;

	offset = 0;

	chunkcount = pdulen/BLE_MAX_MTU_SIZE;
	if((pdulen%BLE_MAX_MTU_SIZE)!=0)
		chunkcount++;

	payloadlen = pdulen - BLE_COMMAND_CODE_SIZE - BLE_COMMAND_SEQNO_SIZE - BLE_COMMAND_LEN_SIZE - BLE_COMMAND_CSUM_SIZE;

	payloadleft = payloadlen;

	for(i=0;i<chunkcount;i++)
	{
		chunk=(BLE_CHUNK_T *)&pdu[i*BLE_MAX_MTU_SIZE];
		if(i==0) {
			*cmdno = chunk->u.firstcmd.cmdno;
			size = sizeof(chunk->u.firstcmd.chunkdata)>payloadleft?payloadleft:sizeof(chunk->u.firstcmd.chunkdata);
			memcpy(payload+offset, chunk->u.firstcmd.chunkdata, size);
			offset += size;
			payloadleft -= size;
		}
		else {
			size = sizeof(chunk->u.other.chunkdata)>payloadleft?payloadleft:sizeof(chunk->u.other.chunkdata);

			memcpy(payload+offset, chunk->u.other.chunkdata, size);
			offset += size;
			payloadleft -= size;
		}
	}

#ifdef ENCRYPT
	if(*cmdno&BLE_COMMAND_WITH_ENCRYPT) {
		final_data = aes_decrypt(keyData_s.ks, payload, payloadlen, &final_datalen);
		if(!final_data) return BLE_RESULT_KEY_INVALID;

		*datalen = (unsigned int)final_datalen;
		memcpy(data, final_data, *datalen);
	}
        else 
	{
		switch (*cmdno)
		{
		case BLE_COMMAND_REQ_PUBLICKEY:
		case BLE_COMMAND_REQ_SERVERNONCE:
			*datalen = offset;
			memcpy(data, payload, *datalen);
			break;
		default:
			return BLE_RESULT_INVALID;
		}
	}
#else
	*datalen = offset;
	memcpy(data, payload, *datalen);
#endif
	*cmdno = *cmdno &(~BLE_COMMAND_WITH_ENCRYPT);
	return (BLE_RESULT_OK);
}

void UnpackBLECommandReq(struct param_handler_svr *param_handler, unsigned char *data, int datalen)
{
}
void UnpackBLECommandReqServerNonce(struct param_handler_svr *param_handler, unsigned char *data, int datalen)
{
#ifdef ENCRYPT
	unsigned char *P1 = NULL, *dec = NULL, decode[MAX_PACKET_SIZE];
	size_t decode_len=0;
	TLV_Header tlv_hdr;
	int tlv_len;
	memset(decode, 0, sizeof(decode));
	decode_len = rsa_decrypt(data, datalen, keyData_s.kp, keyData_s.kp_len, decode, sizeof(decode), 0);
	if (decode_len<=0)
	{
		DBG_ERR("Failed to aes_decrypt() !!!");
		return;
	}

	P1 = (unsigned char *)&decode[0];
	memset(&tlv_hdr, 0, sizeof(tlv_hdr));
	memcpy(&tlv_hdr, P1, sizeof(tlv_hdr));
	tlv_len = ntohl(tlv_hdr.len);
	if (tlv_len <= 0)
	{
		DBG_ERR("Parsing data error !!!");
		MFREE(dec);
		return;
	}

	P1 += sizeof(TLV_Header);
	decode_len -= sizeof(TLV_Header);

	if ((unsigned int)tlv_len > decode_len)
	{
		DBG_ERR("Parsing data error !!!");
		MFREE(dec);
		return;
	}

	keyData_s.km_len = tlv_len;
	MALLOC(keyData_s.km, unsigned char, keyData_s.km_len);
	if (IsNULL_PTR(keyData_s.km))
				{
		DBG_ERR("Failed to MALLOC() !!!");
		return;
	}

	memcpy((unsigned char *)&keyData_s.km[0], (unsigned char *)P1, keyData_s.km_len);
	P1 += tlv_len;
	decode_len -= tlv_hdr.len;

	if (sizeof(TLV_Header) > decode_len)
	{
		DBG_ERR("Parsing data error !!!");
		MFREE(dec);
		return;
	}


	memset(&tlv_hdr, 0, sizeof(tlv_hdr));
	memcpy(&tlv_hdr, P1, sizeof(tlv_hdr));

	tlv_len = ntohl(tlv_hdr.len);
	if (tlv_len <= 0)
	{
		DBG_ERR("Parsing data error !!!");
		MFREE(dec);
		return;
	}

	P1 += sizeof(TLV_Header);
	decode_len -= sizeof(TLV_Header);
	
	if ((unsigned int)tlv_len > decode_len)
	{
		DBG_ERR("Parsing data error !!!");
		MFREE(dec);
		return;
	}

	keyData_s.nc_len = tlv_len;
	MALLOC(keyData_s.nc, unsigned char, keyData_s.nc_len);
	
	memcpy((unsigned char *)&keyData_s.nc[0], (unsigned char *)P1, keyData_s.nc_len);


	keyData_s.ns = gen_rand((size_t *)&keyData_s.ns_len);
	if (IsNULL_PTR(keyData_s.ns))
	{
		DBG_ERR("Failed to gen_rand() !!!");
		MFREE(dec);                
	}


	keyData_s.ks = gen_session_key(keyData_s.km, keyData_s.km_len, keyData_s.ns, keyData_s.ns_len, keyData_s.nc, keyData_s.nc_len, (size_t *)&keyData_s.ks_len);

	if (IsNULL_PTR(keyData_s.ks))
	{
		DBG_ERR("Failed to gen_session_key() !!!");
		MFREE(dec);
		return;
	}
#endif
}

// Server Return Response

// PackBLEResponseData			: Receive status for those status only command
// PackBLEResponseOnly
// PackBLEResponseGetWanStatus
// PackBLEResponseGetWifiStatus
// PackBLEResponseReqPublicKey		: Receive public key
// PackBLEResponseReqServerNonce	: Receive Server Nonce
// PackBLEResponseGetMacBleVersion

// 1. encrypt
// 2. add command/seq no/lenght
// 3. add checksum
void PackBLEResponseData(int cmdno, int status, unsigned char *data, int datalen, unsigned char *pdu, int *pdulen, int flag)
{
	BLE_CHUNK_T *chunk;
	unsigned char *final_data;
	unsigned char final_cmdno;
	unsigned short offset;
	unsigned short final_pdulen;
	size_t final_datalen;
	unsigned short final_dataleft;
	unsigned short final_chunkcount, i;
	unsigned short final_payloadlen;
	unsigned short size;

	final_cmdno = (unsigned char)cmdno;
	final_data = data;
	final_datalen = datalen;
#ifdef ENCRYPT
	if(flag&BLE_FLAG_WITH_ENCRYPT) {
		final_data = aes_encrypt(keyData_s.ks, data, datalen, &final_datalen);
		if (final_data==NULL) {
			final_data = data;
			final_datalen = datalen;
		}
		else final_cmdno |= BLE_COMMAND_WITH_ENCRYPT;
	}
#endif


	chunk=(BLE_CHUNK_T *)pdu;
	offset = 0;
	i = 0;

	final_payloadlen = final_datalen + BLE_COMMAND_CODE_SIZE + BLE_COMMAND_SEQNO_SIZE + BLE_COMMAND_LEN_SIZE + BLE_COMMAND_CSUM_SIZE + BLE_COMMAND_STATUS_SIZE;

	final_chunkcount = final_payloadlen/BLE_MAX_MTU_SIZE;
	if(final_payloadlen%BLE_MAX_MTU_SIZE) final_chunkcount++;

	final_pdulen = final_payloadlen;
	final_dataleft = final_datalen;

	for(i=0;i<final_chunkcount;i++)
	{
		chunk=(BLE_CHUNK_T *)&pdu[i*BLE_MAX_MTU_SIZE];
		if(i==0) {
			chunk->u.firstres.cmdno = final_cmdno;
			chunk->u.firstres.seqno = 0;
			chunk->u.firstres.length = htons(final_pdulen);
			chunk->u.firstres.csum = htons(0);
			chunk->u.firstres.status = status;
			size = sizeof(chunk->u.firstres.chunkdata)>final_dataleft?final_dataleft:sizeof(chunk->u.firstres.chunkdata);
			memcpy(chunk->u.firstres.chunkdata, final_data+offset, size);
		}
		else {
			size = sizeof(chunk->u.other.chunkdata)>final_dataleft?final_dataleft:sizeof(chunk->u.other.chunkdata);
			memcpy(chunk->u.other.chunkdata, final_data+offset, size);
		}
		offset += size;
		chunk += BLE_MAX_MTU_SIZE;
	}

	*pdulen = final_pdulen;

	chunk=(BLE_CHUNK_T *)pdu;
	chunk->u.firstres.csum = htons(Adv_CRC16(pdu, *pdulen));

	return;
}

void PackBLEResponseOnly(int cmdno, int status, unsigned char *pdu, int *pdulen)
{
	PackBLEResponseData(cmdno, status, NULL, 0, pdu, pdulen, BLE_RESPONSE_FLAGS);
}

void PackBLEResponseGetMacBleVersion(int cmdno, int status, unsigned char *pdu, int *pdulen)
{
	char *delim="-";
#if defined(RTCONFIG_RGMII_BRCM5301X) || defined(RTCONFIG_QCA) || defined(RTAC3100)
	char *macaddr=nvram_safe_get("lan_hwaddr");
#else
	char *macaddr=nvram_safe_get("et0macaddr");
#endif
	char *groupid=nvram_safe_get("cfg_group");
	char tmp[DEF_LEN_128];
	memset(tmp, '\0', DEF_LEN_128);

	snprintf(tmp, sizeof(tmp), "%s%s%s%d", tmp, macaddr, delim, BLE_VERSION);
	snprintf(tmp, sizeof(tmp), "%s%s%s", tmp, delim, groupid);

	PackBLEResponseData(cmdno, status, (unsigned char*)tmp, sizeof(tmp), pdu, pdulen, BLE_RESPONSE_FLAGS|BLE_FLAG_WITH_ENCRYPT);
}

#if defined(RTCONFIG_QCA)
void PackBLEResponseGetAth1Chan(int cmdno, int status, unsigned char *pdu, int *pdulen)
{
	FILE *fp;
	char tmp[DEF_LEN_128], buf[DEF_LEN_128];
	char *pt1,*pt2;
	int band=1, cac=-1;
	int len, freq=-2;

	memset(tmp, '\0', sizeof(tmp));
	memset(buf, '\0', sizeof(buf));
	snprintf(tmp, sizeof(tmp), "%d", freq);

	if (nvram_get_int("sw_mode")==1) {
		sprintf(buf, "iwpriv %s get_cac_state", get_wififname(band));

		fp = popen(buf, "r");
		if (fp) {
			memset(buf, 0, sizeof(buf));
			len = fread(buf, 1, sizeof(buf), fp);
			pclose(fp);
			if (len > 1) {
				buf[len-1] = '\0';
				pt1 = strstr(buf, "get_cac_state:");
				if (pt1) {
					pt2 = pt1 + strlen("get_cac_state: ");
					chomp(pt2);
					cac = safe_atoi(pt2);
				}
			}
		}

		if (cac)
			snprintf(tmp, sizeof(tmp), "%d", cac);
		else {
			memset(buf, '\0', sizeof(buf));
			sprintf(buf, "iwconfig %s", get_wififname(band));

			fp = popen(buf, "r");
			if (fp) {
				memset(buf, 0, sizeof(buf));
				len = fread(buf, 1, sizeof(buf), fp);
				pclose(fp);
				if (len > 1) {
					buf[len-1] = '\0';
					pt1 = strstr(buf, "Frequency:");
					if (pt1) {
						pt2 = strstr(pt1, "GHz");
						if(pt2) {
							memset(tmp, '\0', sizeof(tmp));
							strncpy(tmp,pt1+strlen("Frequency:"),pt2-pt1-strlen("Frequency:"));
							chomp(tmp);
							freq=(int)(1000*atof(tmp));
							freq=(freq-5170)*2/10 + 34;
							memset(tmp, '\0', sizeof(tmp));
							snprintf(tmp, sizeof(tmp), "%s%d", tmp, freq);
						}
					}
				}
			}
		}
	}

	PackBLEResponseData(cmdno, status, (unsigned char*)tmp, sizeof(tmp), pdu, pdulen, BLE_RESPONSE_FLAGS|BLE_FLAG_WITH_ENCRYPT);
}
#endif

void PackBLEResponseGetWanStatus(int cmdno, int status, unsigned char *pdu, int *pdulen)
{
	char prefix[DEF_LEN_128];
	unsigned char wanss[2];
	int wanstatus=BLE_WAN_STATUS_ALL_DISCONN;
#if defined(RTCONFIG_DETWAN)
	char *detwan[] = {"detwan", NULL};
	int max_inf, value, conn=0;
	int wan_proto=-1, idx, conn_tmp=1;

	memset(prefix, '\0', sizeof(prefix));

	/* Check the port status */
	max_inf = nvram_get_int("detwan_max");
	for (idx = 0; idx < max_inf; idx++, conn_tmp=conn_tmp<<1) {
		snprintf(prefix, sizeof(prefix), "detwan_mask_%d", idx);
		if ((value = nvram_get_int(prefix)) != 0) {
			if (get_ports_status((unsigned int)value))
					conn |= conn_tmp;
		}
	}

	if ((conn&PHY_PORT0)&&(conn&PHY_PORT1))
		wanstatus = BLE_WAN_STATUS_ALL_UNKNOWN;
	else if (conn>0) {
		/* Check the link proto */
		nvram_unset("wan0_ifname");
		_eval(detwan, NULL, 0, NULL);
		idx = 0;
		while ((nvram_safe_get("wan0_ifname")[0] =='\0') && idx<10) {
			sleep(1);
			idx++;
		}
		memset(prefix, '\0', DEF_LEN_128);
		snprintf(prefix, sizeof(prefix), "%s", nvram_safe_get("wan0_ifname"));
		wan_proto = nvram_get_int("detwan_proto");

		/* Check the link status */
		notify_rc_and_wait("restart_wan_if 0");

		conn_tmp = nvram_get_int("link_internet");
		idx = 0;
		while ( wan_proto==3 && conn_tmp!=2 && idx<20) {
			sleep(1);
			conn_tmp = nvram_get_int("link_internet");
			idx++;
		}
		logmessage("BLUEZ", "wan:%s, proto:%d, internet:%d\n", prefix, wan_proto, conn_tmp);

		if (wan_proto == 3) {
			if (conn_tmp == 2) wan_proto = 1;
			else wan_proto = 2;
		}

		if (strlen(prefix)) {
#if defined(RTCONFIG_QCA953X) || defined(RTCONFIG_QCA956X)
			if (!strncmp(prefix, "vlan2", strlen(prefix))) {
#else
			if (!strncmp(prefix, "eth0", strlen(prefix))) {
#endif
				if (wan_proto == 1)
					wanstatus = BLE_WAN_STATUS_PORT0_DHCP;
				else if (wan_proto == 2)
					wanstatus = BLE_WAN_STATUS_PORT0_PPPOE;
				else
					wanstatus = BLE_WAN_STATUS_PORT0_UNKNOWN;
			}
			else {
				if (wan_proto == 1)
					wanstatus = BLE_WAN_STATUS_PORT1_DHCP;
				else if (wan_proto == 2)
					wanstatus = BLE_WAN_STATUS_PORT1_PPPOE;
				else
					wanstatus = BLE_WAN_STATUS_PORT1_UNKNOWN;
			}
		}
		else {
			if (conn&PHY_PORT0)
				wanstatus = BLE_WAN_STATUS_PORT0_UNKNOWN;
			else if (conn&PHY_PORT1)
				wanstatus = BLE_WAN_STATUS_PORT1_UNKNOWN;
		}
	}
#else
	char prefix2[DEF_LEN_128];
	char tmp[DEF_LEN_128], tmp2[DEF_LEN_128];
	int unit, idx=0;

	memset(prefix, '\0', sizeof(prefix));
	memset(prefix2, '\0', sizeof(prefix2));

	while (idx++<=5) sleep(1);

#ifndef RTCONFIG_LANTIQ
	for(unit = WAN_UNIT_FIRST; unit < WAN_UNIT_MAX; ++unit) {
		if(get_dualwan_by_unit(unit) != WANS_DUALWAN_IF_WAN && get_dualwan_by_unit(unit) != WANS_DUALWAN_IF_LAN)
			continue;
#else
	unit = 0;
#endif
		snprintf(prefix, sizeof(prefix), "wan%d_", unit);
		if(unit == WAN_UNIT_FIRST)
			snprintf(prefix2, sizeof(prefix2), "autodet_");
		else
			snprintf(prefix2, sizeof(prefix2), "autodet%d_", unit);
#ifdef RTCONFIG_LANTIQ
		int det_wait = 10;
		while ( !*nvram_safe_get(strcat_r(prefix2, "state", tmp2)) && det_wait--) {
			printf("\nwait %ds to get autodet results.\n", det_wait);
			notify_rc_after_period_wait("start_autodet", 0);
			sleep(1);
		}
		printf("\nthe autodet result:%d\n", nvram_get(strcat_r(prefix2, "state", tmp2))?nvram_get_int(strcat_r(prefix2, "state", tmp2)):-1);
#endif
		if (nvram_get_int(strcat_r(prefix2, "state", tmp2)) == AUTODET_STATE_FINISHED_NOLINK
#ifdef RTCONFIG_LANTIQ
			|| !*nvram_safe_get(strcat_r(prefix2, "state", tmp2)) 		
#endif
		) {
			if(nvram_get_int(strcat_r(prefix, "auxstate_t", tmp))==1) {
				nvram_set("autodet_state", "0");
				notify_rc_after_period_wait("start_autodet", 0);
			}
			wanstatus = BLE_WAN_STATUS_ALL_DISCONN;
		}
		else if (nvram_get_int(strcat_r(prefix2, "state", tmp2)) == AUTODET_STATE_FINISHED_WITHPPPOE
			|| nvram_get_int(strcat_r(prefix2, "auxstate", tmp2)) == AUTODET_STATE_FINISHED_WITHPPPOE) {
			if( ( nvram_get_int(strcat_r(prefix, "state_t", tmp))==2
				&& nvram_get_int(strcat_r(prefix, "sbstate_t", tmp))==0
				&& nvram_get_int(strcat_r(prefix, "auxstate_t", tmp))==0 ) 
			    &&
			    ( nvram_get_int("link_internet")==2
				|| nvram_get_int(strcat_r(prefix, "realip_state", tmp))==2 )
			   ) {
				wanstatus = BLE_WAN_STATUS_PORT0_DHCP;
			}
			else
				wanstatus = BLE_WAN_STATUS_PORT0_PPPOE;
		}
		else if( nvram_get_int(strcat_r(prefix, "state_t", tmp))==2
			&& nvram_get_int(strcat_r(prefix, "sbstate_t", tmp))==0
			&& nvram_get_int(strcat_r(prefix, "auxstate_t", tmp))==0 ) 
			wanstatus = BLE_WAN_STATUS_PORT0_DHCP;
		else if( nvram_get_int(strcat_r(prefix2, "state", tmp2))==2) {
			if ( nvram_get_int(strcat_r(prefix, "auxstate_t", tmp))!=1)
				wanstatus = BLE_WAN_STATUS_PORT0_DHCP;
			else if( nvram_get_int(strcat_r(prefix, "state_t", tmp))==4
				&& nvram_get_int(strcat_r(prefix, "sbstate_t", tmp))==4
				&& nvram_get_int(strcat_r(prefix, "auxstate_t", tmp))==0 ) 
				wanstatus = BLE_WAN_STATUS_PORT0_UNKNOWN;
		}
		else if( nvram_get_int(strcat_r(prefix, "state_t", tmp))==4
			&& nvram_get_int(strcat_r(prefix, "sbstate_t", tmp))==4 )
			wanstatus = BLE_WAN_STATUS_PORT0_DHCP;
		else
			wanstatus = BLE_WAN_STATUS_PORT0_UNKNOWN;
#ifndef RTCONFIG_LANTIQ
	}
#endif

	if (BLEPACKET_DEBUG)
		printf("%s, %s[%d], %s[%d], %s[%d], %s[%d]\n"
			"%s, %s[%d], %s[%d]\n"
			"%s, %s[%d]\n",
				prefix,
				"state", nvram_get_int(strcat_r(prefix, "state_t", tmp)),
				"sbstate", nvram_get_int(strcat_r(prefix, "sbstate_t", tmp)),
				"auxstate", nvram_get_int(strcat_r(prefix, "auxstate_t", tmp)),
				"realip_state", nvram_get_int(strcat_r(prefix, "realip_state", tmp)),
				prefix2,
				"state", nvram_get_int(strcat_r(prefix2, "state", tmp2)),
				"auxstate", nvram_get_int(strcat_r(prefix2, "auxstate", tmp2)),
				"None",
				"link_internet", nvram_get_int("link_internet")
		);

	logmessage("BLUEZ", "wan:%s, proto:%d, internet:%d\n", prefix, wanstatus, nvram_get_int("link_internet"));

#endif
	wanss[0]=wanstatus;
	PackBLEResponseData(cmdno, status, wanss, 1, pdu, pdulen, BLE_RESPONSE_FLAGS|BLE_FLAG_WITH_ENCRYPT);
}

void PackBLEResponseGetWifiStatus(int cmdno, int status, unsigned char *pdu, int *pdulen)
{
	unsigned char wifistatus[2];
	memset(wifistatus, '\0', sizeof(wifistatus));

	wifistatus[0]=0x01;
	PackBLEResponseData(cmdno, status, wifistatus, 1, pdu, pdulen, BLE_RESPONSE_FLAGS|BLE_FLAG_WITH_ENCRYPT);
}

void PackBLEResponseReqPublicKey(int cmdno, int status, unsigned char *pdu, int *pdulen)
{
	PackBLEResponseData(cmdno, status, (unsigned char *)&keyData_s.ku[0], keyData_s.ku_len, pdu, pdulen, BLE_RESPONSE_FLAGS);
}

void PackBLEResponseReqServerNonce(int cmdno, int status, unsigned char *pdu, int *pdulen)
{
#ifdef ENCRYPT
	unsigned char *PPP = NULL, *P1 = NULL, *enc = NULL;
	int enc_len;
	TLV_Header tlv_hdr;

	MALLOC(PPP, unsigned char, (sizeof(TLV_Header)+keyData_s.ns_len+sizeof(TLV_Header)+keyData_s.nc_len));
	if (IsNULL_PTR(PPP))
	{
		DBG_ERR("Failed to MALLOC() !!!");
		return;
	}

	P1 = &PPP[0];
	memset(&tlv_hdr, 0, sizeof(tlv_hdr));
	tlv_hdr.len = (unsigned int)htonl(keyData_s.ns_len);
	memcpy((unsigned char *)P1, (unsigned char *)&tlv_hdr, sizeof(tlv_hdr));
	P1 += sizeof(tlv_hdr);
	memcpy((unsigned char *)P1, (unsigned char *)&keyData_s.ns[0], keyData_s.ns_len);
	P1 += keyData_s.ns_len;

	memset(&tlv_hdr, 0, sizeof(tlv_hdr));
	tlv_hdr.len = htonl(keyData_s.nc_len);
	memcpy((unsigned char *)P1, (unsigned char *)&tlv_hdr, sizeof(tlv_hdr));
	P1 += sizeof(tlv_hdr);
	memcpy((unsigned char *)P1, (unsigned char *)&keyData_s.nc[0], keyData_s.nc_len);

	enc = aes_encrypt(keyData_s.km, PPP, sizeof(TLV_Header)+keyData_s.ns_len+sizeof(TLV_Header)+keyData_s.nc_len, (size_t *)&enc_len);

	if (enc_len <= 0)
	{
		DBG_ERR("Failed to aes_encrypt() !!!");
		MFREE(PPP);
	}

	PackBLEResponseData(cmdno, status, enc, enc_len, pdu, pdulen, BLE_RESPONSE_FLAGS); 

	MFREE(PPP);
	MFREE(enc);
#endif
}

void UnPackBLEExceptionGetNvram(int cmdno, int status, unsigned char *data, int datalen, unsigned char *pdu, int *pdulen)
{
	char value[DEF_LEN_256];
	

	memset(value, '\0', DEF_LEN_256);
	snprintf(value, sizeof(value), "%s", nvram_safe_get(data));

	printf("[nvram: value]: %s=%s , len:%d , size:%d \n", data, value, strlen(value), sizeof(value));

	PackBLEResponseData(cmdno, status, (unsigned char*)value, sizeof(value), pdu, pdulen, BLE_RESPONSE_FLAGS|BLE_FLAG_WITH_ENCRYPT);
}

