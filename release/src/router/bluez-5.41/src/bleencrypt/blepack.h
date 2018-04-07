/************************************************************/
/*  Version 1.4     by Yuhsin_Lee 2005/1/19 16:31           */
/************************************************************/

#ifndef __PACKET_H__
#define __PACKET_H__

#ifdef __cplusplus
extern "C" {
#endif
#define TYPEDEF_BOOL
#include <bcmnvram.h>
#include <bcmparams.h>
#include <shared.h>
#include <ctype.h>

#pragma pack(1)

/****************************************/
/*              FOR LINUX               */
/****************************************/
#ifndef  WIN32
#define ULONG   unsigned long
#define DWORD   unsigned long
#define BYTE    unsigned char
#define PBYTE   unsigned char *
#define WORD    unsigned short
#define INT     int
#endif //#ifndef  WIN32

#define BLE_VERSION		2
#define DEF_LEN_128 		128
#define DEF_LEN_256 		256

#define BLEPACKET_DEBUG		0
#define MAX_PACKET_SIZE		4096
#define BLE_MAX_MTU_SIZE	20
#define BLE_COMMAND_CODE_SIZE	1
#define BLE_COMMAND_SEQNO_SIZE	1
#define BLE_COMMAND_LEN_SIZE	2
#define BLE_COMMAND_CSUM_SIZE	2
#define BLE_COMMAND_STATUS_SIZE 1

#define BLE_FLAG_WITH_ENCRYPT 	0x1
#define BLE_FLAG_WITH_CHECKSUM  0x2
#define BLE_COMMAND_FLAGS	BLE_FLAG_WITH_CHECKSUM
#define BLE_RESPONSE_FLAGS	BLE_FLAG_WITH_CHECKSUM
#define BLE_COMMAND_WITH_ENCRYPT	0x80

#define BLE_PDU_SIZE		3584	//(packet_size - packet_size/20*2 - checksum length - packet lenght - status)/16%16
#define MAX_DETWAN 4

enum  BLE_COMMAND
{
	BLE_COMMAND_REQ_PUBLICKEY		= 0x00,
	BLE_COMMAND_REQ_SERVERNONCE		= 0x01,
	BLE_COMMAND_APPLY			= 0x02,
	BLE_COMMAND_RESET			= 0x03,
	BLE_COMMAND_GET_WAN_STATUS		= 0x04,
	BLE_COMMAND_GET_WIFI_STATUS		= 0x05,
	BLE_COMMAND_SET_WAN_TYPE		= 0x06,
	BLE_COMMAND_SET_WAN_PPPOE_NAME		= 0x07,
	BLE_COMMAND_SET_WAN_PPPOE_PWD		= 0x08,
	BLE_COMMAND_SET_WAN_IPADDR		= 0x09,
	BLE_COMMAND_SET_WAN_SUBNET_MASK		= 0x0a,
	BLE_COMMAND_SET_WAN_GATEWAY		= 0x0b,
	BLE_COMMAND_SET_WAN_DNS1		= 0x0c,
	BLE_COMMAND_SET_WAN_DNS2		= 0x0d,
	BLE_COMMAND_SET_WAN_PORT		= 0x0e,
	BLE_COMMAND_SET_WIFI_NAME		= 0x0f,
	BLE_COMMAND_SET_WIFI_PWD		= 0x10,
	BLE_COMMAND_SET_GROUP_ID		= 0x11,
	BLE_COMMAND_SET_ADMIN_NAME		= 0x12,
	BLE_COMMAND_SET_ADMIN_PWD		= 0x13,
	BLE_COMMAND_SET_USER_LOCATION		= 0x14,
	BLE_COMMAND_SET_USER_PLACE		= 0x15,
	BLE_COMMAND_SET_SW_MODE			= 0x16,
	BLE_COMMAND_SET_WAN_DNS_ENABLE		= 0x17,
	BLE_COMMAND_GET_MAC_BLE_VERSION		= 0x18,
	BLE_COMMAND_GET_ATH1_CHAN		= 0x19,
	BLE_COMMAND_SET_ATH1_CHAN		= 0x1a,
	BLE_COMMAND_GET_NVRAM			= 0x1b,
	BLE_COMMAND_END				= 0xff
};

enum  BLE_RESULT
{
        BLE_RESULT_OK = 0,
        BLE_RESULT_INVALID,
        BLE_RESULT_KEY_INVALID,
        BLE_RESULT_CHECKSUM_INVALID
};

enum BLE_PYH_STATUS
{
	PHY_PORT0	= 0x01,
	PHY_PORT1	= 0x02
};

enum BLE_WAN_STATUS
{
	BLE_WAN_STATUS_ALL_DISCONN=0,
	BLE_WAN_STATUS_ALL_UNKNOWN,
	BLE_WAN_STATUS_PORT0_DHCP,
	BLE_WAN_STATUS_PORT0_PPPOE,
	BLE_WAN_STATUS_PORT0_UNKNOWN,
	BLE_WAN_STATUS_PORT1_DHCP,
	BLE_WAN_STATUS_PORT1_PPPOE,
	BLE_WAN_STATUS_PORT1_UNKNOWN,
	BLE_WAN_END
};

enum  BLE_WIFI_STATUS
{
        BLE_WIFI_STATUS_CONNECTED = 0,
        BLE_WIFI_STATUS_PASSWORD_ERROR,
        BLE_WIFI_STATUS_UNKNOWN_ERROR
};

enum BLE_DATA_TYPE
{
	BLE_DATA_TYPE_NULL = 0,
	BLE_DATA_TYPE_STRING,
	BLE_DATA_TYPE_INTEGER,
	BLE_DATA_TYPE_IP
};

typedef struct ble_chunk_t        {
        union   {
                struct {
			BYTE cmdno;
			BYTE seqno; // unused now
                	WORD length;
			WORD csum;
			BYTE chunkdata[BLE_MAX_MTU_SIZE-BLE_COMMAND_CODE_SIZE-BLE_COMMAND_SEQNO_SIZE-BLE_COMMAND_LEN_SIZE-BLE_COMMAND_CSUM_SIZE];
		} firstcmd;

		struct {
			BYTE cmdno;
			BYTE seqno; // unused no
			WORD length;
			WORD csum;
			BYTE status;
			BYTE chunkdata[BLE_MAX_MTU_SIZE-BLE_COMMAND_CODE_SIZE-BLE_COMMAND_SEQNO_SIZE-BLE_COMMAND_LEN_SIZE-BLE_COMMAND_CSUM_SIZE-BLE_COMMAND_STATUS_SIZE];
		} firstres;

		struct {
			BYTE chunkdata[BLE_MAX_MTU_SIZE];
		} other;
        } u;
} BLE_CHUNK_T;

typedef struct KeyData_t
{
	unsigned char *ku, *kp, *km, *ns, *nc, *ks, *iv;
	size_t ku_len, kp_len, ns_len, km_len, nc_len, ks_len, iv_len;
} KeyData_s;

struct param_handler_svr {
	int cmdno;
	char *nvram;
	char *do_rc_service;
	int t_type;
};

extern void KeyInit_S(char *public_key, char *private_key);
extern void KeyReset_S();
extern int UnpackBLECommandData(unsigned char *pdu, int pdulen, int *cmdno, unsigned char *data, unsigned int *datalen);
extern void UnpackBLECommandReq(struct param_handler_svr *param_handler, unsigned char *data, int datalen);
extern void UnpackBLECommandReqServerNonce(struct param_handler_svr *param_handler, unsigned char *data, int datalen);
extern void UnpackBLEDataToNvram(struct param_handler_svr *param_handler, unsigned char *data, int datalen);

extern void PackBLEResponseData(int cmdno, int status, unsigned char *data, int datalen, unsigned char *pdu, int *pdulen, int flag);
extern void PackBLEResponseOnly(int cmdno, int status, unsigned char *pdu, int *pdulen);
extern void PackBLEResponseGetWanStatus(int cmdno, int status, unsigned char *pdu, int *pdulen);
extern void PackBLEResponseGetWifiStatus(int cmdno, int status, unsigned char *pdu, int *pdulen);
extern void PackBLEResponseReqPublicKey(int cmdno, int status, unsigned char *pdu, int *pdulen);
extern void PackBLEResponseReqServerNonce(int cmdno, int status, unsigned char *pdu, int *pdulen);
extern void PackBLEResponseGetMacBleVersion(int cmdno, int status, unsigned char *pdu, int *pdulen);
extern void PackBLEResponseGetAth1Chan(int cmdno, int status, unsigned char *pdu, int *pdulen);

extern void UnPackBLEExceptionGetNvram(int cmdno, int status, unsigned char *data, int datalen, unsigned char *pdu, int *pdulen);
#ifdef __cplusplus
}
#endif

#endif // #ifndef __PACKET_H__
 
