/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include "lib/bluetooth.h"
#include "lib/sdp.h"
#include "lib/sdp_lib.h"
#include "lib/uuid.h"
#include "btio/btio.h"
#include "gdbus/gdbus.h"
#include "src/shared/util.h"
#include "src/shared/queue.h"
#include "src/shared/att.h"
#include "src/shared/gatt-db.h"
#include "src/shared/gatt-server.h"
#include "src/shared/timeout.h"
#include "src/log.h"
#include "src/error.h"
#include "src/adapter.h"
#include "src/device.h"
#include "src/gatt-database.h"
#include "src/dbus-common.h"
#include "gatt-amap.h"

int ble_data_len = 0;
int ble_count_len = 0;
unsigned char ble_data[MAX_PACKET_SIZE];

/*
static void print_uuid(const bt_uuid_t *uuid)
{
	char uuid_str[MAX_LEN_UUID_STR];
	bt_uuid_t uuid128;

	bt_uuid_to_uuid128(uuid, &uuid128);
	bt_uuid_to_string(&uuid128, uuid_str, sizeof(uuid_str));

	printf("%s\n", uuid_str);
}
*/

static bool amap_nvram_cb(void *user_data)
	
{
	struct btd_gatt_database *database = user_data;
	struct btd_device *device;
	struct bt_gatt_server *server;
	bdaddr_t bdaddr;
	uint8_t bdaddr_type, *value = NULL;
	size_t len = 0, index;

	if (!get_dst_info(database->amap_att, &bdaddr, &bdaddr_type))
		return false;

	device = btd_adapter_get_device(database->adapter, &bdaddr, bdaddr_type);
	if (!device)
		return false;

	server = btd_device_get_gatt_server(device);


	len = database->amap_value_len;

	if (len <= 0)
		return false;

	value = database->amap_value;
	//print_data_topic("TX", (int)len);

	for (index=0; index<=len; index+=MAX_LE_DATALEN) {

		if (index>0) value += MAX_LE_DATALEN;
		//print_data_info(value, (index+MAX_LE_DATALEN)>len ? (len-index):MAX_LE_DATALEN, MAX_LE_DATALEN, index, 0);

		bt_gatt_server_send_notification(
			server,
			database->amap_nvram_handle, value,
			(index+MAX_LE_DATALEN)>len ? (len-index):MAX_LE_DATALEN);
		usleep(50* MILLISEC);
	}

	return true;
}

static void amap_write_value(struct gatt_db_attribute *attrib,
					unsigned int id, uint16_t offset,
					const uint8_t *value, size_t len,
					uint8_t opcode, struct bt_att *att,
					void *user_data)
{
	struct btd_gatt_database *database = user_data;
	uint8_t error = 0;

	if ( !value || !len || len > MAX_LE_DATALEN) {
		error = BT_ATT_ERROR_INVALID_ATTRIBUTE_VALUE_LEN;
		goto done;
	}

	if (!ble_data_len) {
		ble_data_len = (int)value[2]*256 + (int)value[3];
		memset(ble_data, '\0', MAX_PACKET_SIZE);
		//print_data_topic("RX", ble_data_len);
	}

	memcpy(ble_data+ble_count_len, value, len);
	//print_data_info((uint8_t *)value, len, MAX_LE_DATALEN, ble_count_len, 0);
	ble_count_len += (int)len;

	if (ble_count_len < ble_data_len) {
		goto done2;
	}
	else if (ble_count_len > ble_data_len) {
		error = BT_ATT_ERROR_INVALID_ATTRIBUTE_VALUE_LEN;
		goto done;
	}

	if (ble_data_len) {
		unsigned char uchar_out[MAX_PACKET_SIZE];
		int len_out;

		memset(uchar_out, '\0', MAX_PACKET_SIZE);

		len_out = ble_encrypt_svr(ble_data, uchar_out, ble_data_len);
		if ( len_out < MAX_PACKET_SIZE )
			uchar_out[len_out] = '\0';

		if(len_out < 1) {
			error = BT_ATT_ERROR_INVALID_ATTRIBUTE_VALUE_LEN;
			goto done;
		}

		memset(database->amap_value, '\0', MAX_PACKET_SIZE);
		memcpy(database->amap_value, uchar_out, len_out);
		database->amap_value_len = len_out;
		database->amap_att = att;
	}

	amap_nvram_cb(database);
done:
	ble_data_len = 0;
	ble_count_len = 0;
done2:
	gatt_db_attribute_write_result(attrib, id, error);
}

void amap_gatt_service(struct btd_gatt_database *database)
{
	bt_uuid_t uuid;
	struct gatt_db_attribute *service;

	/* Service */
	bt_uuid16_create(&uuid, UUID_AMAP);
	service = gatt_db_add_service(database->db, &uuid, true, MAX_AMAP_SERVICE);
	database->amap_handle = database_add_record(database, UUID_AMAP,
						service,
						"AMAP Definition Profile");
	/* Product Value */
	bt_uuid16_create(&uuid, GATT_CHARAC_AMAP_PRODUCT);
	database->svc_chngd = gatt_db_service_add_characteristic(service, &uuid,
						BT_ATT_PERM_WRITE,
						BT_GATT_CHRC_PROP_WRITE | BT_GATT_CHRC_PROP_NOTIFY,
						NULL,
						amap_write_value,
						database);
	database->amap_nvram_handle = gatt_db_attribute_get_handle(database->svc_chngd);
	database->svc_chngd_ccc = service_add_ccc(service, database, NULL, NULL, NULL);

	ble_key_act("Server", "Init");
	gatt_db_service_set_active(service, true);
}
/*AMAP END*/
