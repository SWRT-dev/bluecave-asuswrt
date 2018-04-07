/*
 *
 * bleencrypt.h
 *
*/
#ifndef __BLEENCRYPT_H__
#define __BLEENCRYPT_H___

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <unistd.h>
#include "blepack.h"

#define DEFAULT_PUBLIC_PEM_FILE		"/etc/bluetooth/public.pem\0"
#define DEFAULT_PRIVATE_PEM_FILE	"/etc/bluetooth/private.pem\0"

#define GATTDATABASE_DEBUG	0
#define BLEENCRYPT_DEBUG	0
#define MAX_LE_DATALEN		20
#define MAX_STR_LEN		128
#define MILLISEC		1000

struct cmd_handler_svr {
	char *command;
	int cmdno;
	void (*unpack)(struct param_handler_svr *param_handler, unsigned char *data, int datalen);
	void (*pack)(int cmdno, int status, unsigned char *pdu, int *pdulen);
};

extern void print_data_topic(char *topic, int length);
extern void print_data_info(uint8_t *value, int val_len, int row_len, int val_index, int flag);
extern int ble_encrypt_svr(unsigned char *input, unsigned char *output, size_t input_len);
extern void ble_key_act(char *type,  char *action);
#endif	//__BLEENCRYPT_H___
