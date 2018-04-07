/*##################################################################################################
# "Copyright (c) 2013 Intel Corporation                                                            #
# DISTRIBUTABLE AS SAMPLE SOURCE SOFTWARE                                                          #
# This Distributable As Sample Source Software is subject to the terms and conditions              #
# of the Intel Software License Agreement for the Intel(R) Cable and GW Software Development Kit"  #
##################################################################################################*/

#ifndef __FAPI_WLAN_BEEROCK_AML_H_
#define __FAPI_WLAN_BEEROCK_AML_H_

#include <stdio.h>
#include <stdlib.h>

#include "help_proto.h"
#include "help_defs.h"

#include "help_objlist.h"
#include "fapi_wlan.h"
#include "fapi_wlan_private.h"

#define ASUS_BSD
#ifdef ASUS_BSD
#include <net/ethernet.h>
#include <netinet/ether.h>
#endif

/** \defgroup FAPI_WLAN_COMMON_BEEROCK_AML FAPI Beerock interfaces
*   @ingroup FAPI_WLAN
\brief It provides the set of API to support the Intel Beerock implementation (AP manager).
*/

/** \addtogroup FAPI_WLAN_COMMON_BEEROCK_AML */
/* @{ */

// AP Manager Lite
int ap_manager_lite_cellular_config_set(const char *ifname, char *MBOCellAware, char *CellPref);
int ap_manager_lite_bss_neighbor_set(const char *ifname, char *nr_BSSID, char *opClass, char *channelNumber, char *priority, char *oce, char *tbtt_offset);
int ap_manager_lite_btm_params_set(const char *ifname,
								   char *bssTermination,
								   char *disassocImminent,  /* Optional Param - can be 'NULL' */
                                   char *btmReqTermBit,     /* Optional Param - can be 'NULL' */
								   char *disassocTimer,     /* Optional Param - can be 'NULL' */
								   char *reassocDelay,      /* Optional Param - can be 'NULL' */
								   char *bssTermTSF);       /* Optional Param - can be 'NULL' */
int ap_manager_lite_association_disallow(const char *ifname, char *reassocCode);  /* BSSID will be get internally */
int ap_manager_lite_disassociate_set(const char *ifname, char *MACAddress);
int ap_manager_lite_btm_req(const char *ifname,
							char *MACAddress,
							char *dialog_token,
							char *pref,
							char *reason,
							char *disassocTimer,  /* Optional Param - can be 'NULL' */
							char *abridged,       /* Optional Param - can be 'NULL' */
							char *valid_int);     /* Optional Param - can be 'NULL' */
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
								   char *ssid,          /* Optional Param - can be 'NULL' */
								   char *repDetail,     /* Optional Param - can be 'NULL' */
								   char *repCond,       /* Optional Param - can be 'NULL' */
								   char *apChReport,    /* Optional Param - can be 'NULL' */
								   char *reqElements);  /* Optional Param - can be 'NULL' */
void beerock_aml_cli(char *ifname, char **argv, int argc);
int ap_manager_lite_band_steering_init(void);
int fapiWlanRefCallBackFunc(const char *ifname, char *buf);
int fapiWlanCallBackBandSteeringFunc(char *opCode, const char *ifname, ObjList *wlObj, unsigned int flags, void *context);
int ap_manager_lite_band_steering_perform(int signalStrengthThreshold_2_4, int signalStrengthThreshold_5, int intervalInSeconds, int toleranceInSeconds, int numOfTicksAllowedForSteering);
int fapiWlanCallBackApManagerLiteFunc(char *opCode, const char *ifname, ObjList *wlObj, unsigned int flags, void *context);

#ifdef ASUS_BSD
#define ASUS_INFO(fmt, arg...) \
	do {    \
		printf("[ASUS]"fmt, ##arg); \
	} while (0)

#define MAX_IFNUM 6
#define STA_INFO_PATH "/tmp/ltq_bsd_sta_list"
#define STA_TIMEOUT 30
#define THRES_IDLE_RATE 100 /*Kbps*/
#define THRES_RSSI_COUNT 3
#define THRES_IDLE_COUNT 3
#if 1
#define BOUNCE_WINODW           180
#define BOUNCE_COUNT            3
#define BOUNCE_DWELL            3600
#define BOUNCE_HASH_SIZE        32
#define STA_MAC_HASH(ea)	(((ea).ether_addr_octet[4]+(ea).ether_addr_octet[5]) % BOUNCE_HASH_SIZE)
#endif

typedef struct sta_info
{
	char mac[64];
	time_t active;
	struct sta_info *prev, *next;
	float datarate; /* Kbps */
	int rssi;
	unsigned long rssi_hit_count;
	unsigned long idle_hit_count;
	unsigned long last_txrx_bytes;
} sta_info_t;

typedef enum
{
	BOUNCE_INIT = 0,
	BOUNCE_WINDOW_STATE,
	BOUNCE_DWELL_STATE,
	BOUNCE_CLEAN_STATE
} bounce_state_t;

typedef struct bounce_detect
{
	unsigned int window;    /* window time in seconds */
	unsigned int cnt;       /* counts */
	unsigned int dwell_time;        /* dwell time in seconds */
} bounce_detect_t;


typedef struct sta_bounce_detect
{
	char mac[64];
	time_t timestamp;               /* timestamp of first steering time */
	bounce_detect_t b_setting;      /* sta bounce detect running counts */
	bounce_state_t b_state;
	struct sta_bounce_detect *next;
} sta_bounce_detect_t;

sta_info_t *bsd_assoclist[MAX_IFNUM];
sta_bounce_detect_t *sta_bounce_table[BOUNCE_HASH_SIZE];
sta_info_t *asus_bsd_add_to_assoclist(int ifidx, char *stamac);
sta_info_t *asus_bsd_remove_from_assoclist(int ifidx, char *stamac);
void asus_bsd_timeout_sta(int ifidx);
void asus_bsd_update_sta_info(char *ifname, int rssi_thres, int gl);
int asus_bsd_allow_steer(int ifidx, char *stamac);
void asus_bsd_add_sta_to_bounce_table(char *stamac);
int asus_bsd_check_bouncing_sta(char *stamac);
void asus_bsd_update_bounce_table(void);
void asus_bsd_cleanup_bounce_table(void);
#endif

#endif  //__FAPI_WLAN_BEEROCK_AML_H_
