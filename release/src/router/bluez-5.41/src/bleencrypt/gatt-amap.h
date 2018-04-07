/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2015  Google Inc.
 */

#include "bleencrypt.h"

#define UUID_AMAP     0xAB00  
#define GATT_CHARAC_AMAP_PRODUCT                      0xAB10

#define MAX_AMAP_SERVICE	4

void amap_gatt_service(struct btd_gatt_database *database);
