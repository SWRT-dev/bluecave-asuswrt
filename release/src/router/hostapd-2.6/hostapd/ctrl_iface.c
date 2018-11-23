/*
 * hostapd / UNIX domain socket -based control interface
 * Copyright (c) 2004-2015, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "utils/includes.h"

#ifndef CONFIG_NATIVE_WINDOWS

#ifdef CONFIG_TESTING_OPTIONS
#include <net/ethernet.h>
#include <netinet/ip.h>
#endif /* CONFIG_TESTING_OPTIONS */

#include <sys/un.h>
#include <sys/stat.h>
#include <stddef.h>

#ifdef CONFIG_CTRL_IFACE_UDP
#include <netdb.h>
#endif /* CONFIG_CTRL_IFACE_UDP */

#include "utils/common.h"
#include "utils/eloop.h"
#include "utils/module_tests.h"
#include "common/wpa_ctrl.h"
#include "common/version.h"
#include "common/ieee802_11_defs.h"
#include "common/ieee802_11_common.h"
#include "common/ctrl_iface_common.h"
#include "crypto/tls.h"
#include "drivers/driver.h"
#include "eapol_auth/eapol_auth_sm.h"
#include "radius/radius_client.h"
#include "radius/radius_server.h"
#include "l2_packet/l2_packet.h"
#include "ap/hostapd.h"
#include "ap/ap_config.h"
#include "ap/ieee802_1x.h"
#include "ap/wpa_auth.h"
#include "ap/ieee802_11.h"
#include "ap/sta_info.h"
#include "ap/wps_hostapd.h"
#include "ap/ctrl_iface_ap.h"
#include "ap/ap_drv_ops.h"
#include "ap/hs20.h"
#include "ap/wnm_ap.h"
#include "ap/wpa_auth.h"
#include "ap/acs.h"
#include "ap/bss_load.h"
#include "ap/hw_features.h"
#include "ap/beacon.h"
#include "ap/neighbor_db.h"
#include "ap/rrm.h"
#include "wps/wps_defs.h"
#include "wps/wps.h"
#include "fst/fst_ctrl_iface.h"
#include "config_file.h"
#include "ctrl_iface.h"
#include "drivers/nl80211_copy.h"
#include "ap/atf.h"


#define HOSTAPD_CLI_DUP_VALUE_MAX_LEN 256

#ifdef CONFIG_CTRL_IFACE_UDP
#define COOKIE_LEN 8
static unsigned char cookie[COOKIE_LEN];
static unsigned char gcookie[COOKIE_LEN];
#define HOSTAPD_CTRL_IFACE_PORT		8877
#define HOSTAPD_CTRL_IFACE_PORT_LIMIT	50
#define HOSTAPD_GLOBAL_CTRL_IFACE_PORT		8878
#define HOSTAPD_GLOBAL_CTRL_IFACE_PORT_LIMIT	50
#endif /* CONFIG_CTRL_IFACE_UDP */

static void hostapd_ctrl_iface_send(struct hostapd_data *hapd, int level,
				    enum wpa_msg_type type,
				    const char *buf, size_t len);


static int hostapd_ctrl_iface_attach(struct hostapd_data *hapd,
				     struct sockaddr_storage *from,
				     socklen_t fromlen)
{
	return ctrl_iface_attach(&hapd->ctrl_dst, from, fromlen);
}


static int hostapd_ctrl_iface_detach(struct hostapd_data *hapd,
				     struct sockaddr_storage *from,
				     socklen_t fromlen)
{
	return ctrl_iface_detach(&hapd->ctrl_dst, from, fromlen);
}


static int hostapd_ctrl_iface_level(struct hostapd_data *hapd,
				    struct sockaddr_storage *from,
				    socklen_t fromlen,
				    char *level)
{
	return ctrl_iface_level(&hapd->ctrl_dst, from, fromlen, level);
}


static int hostapd_ctrl_iface_new_sta(struct hostapd_data *hapd,
				      const char *txtaddr)
{
	u8 addr[ETH_ALEN];
	struct sta_info *sta;

	wpa_printf(MSG_DEBUG, "CTRL_IFACE NEW_STA %s", txtaddr);

	if (hwaddr_aton(txtaddr, addr))
		return -1;

	sta = ap_get_sta(hapd, addr);
	if (sta)
		return 0;

	wpa_printf(MSG_DEBUG, "Add new STA " MACSTR " based on ctrl_iface "
		   "notification", MAC2STR(addr));
	sta = ap_sta_add(hapd, addr);
	if (sta == NULL)
		return -1;

	hostapd_new_assoc_sta(hapd, sta, 0);
	return 0;
}


#ifdef CONFIG_IEEE80211W
#ifdef NEED_AP_MLME
static int hostapd_ctrl_iface_sa_query(struct hostapd_data *hapd,
				       const char *txtaddr)
{
	u8 addr[ETH_ALEN];
	u8 trans_id[WLAN_SA_QUERY_TR_ID_LEN];

	wpa_printf(MSG_DEBUG, "CTRL_IFACE SA_QUERY %s", txtaddr);

	if (hwaddr_aton(txtaddr, addr) ||
	    os_get_random(trans_id, WLAN_SA_QUERY_TR_ID_LEN) < 0)
		return -1;

	ieee802_11_send_sa_query_req(hapd, addr, trans_id);

	return 0;
}
#endif /* NEED_AP_MLME */
#endif /* CONFIG_IEEE80211W */


#ifdef CONFIG_WPS
static int hostapd_ctrl_iface_wps_pin(struct hostapd_data *hapd, char *txt)
{
	char *pin, *uuid, *timeout_txt;
	int timeout;
	u8 addr_buf[ETH_ALEN], *addr = NULL;
	char *pos;

	hapd = get_bss_index(txt, hapd->iface);
	if (hapd == NULL) {
		return -1;
	}

	uuid = os_strchr(txt, ' ');
	if (uuid == NULL)
		return -1;
	uuid++;

	pin = os_strchr(uuid, ' ');
	if (pin == NULL)
		return -1;
	*pin++ = '\0';

	timeout_txt = os_strchr(pin, ' ');
	if (timeout_txt) {
		*timeout_txt++ = '\0';
		timeout = atoi(timeout_txt);
		pos = os_strchr(timeout_txt, ' ');
		if (pos) {
			*pos++ = '\0';
			if (hwaddr_aton(pos, addr_buf) == 0)
				addr = addr_buf;
		}
	} else
		timeout = 0;

	return hostapd_wps_add_pin(hapd, addr, uuid, pin, timeout);
}


static int hostapd_ctrl_iface_wps_check_pin(
	struct hostapd_data *hapd, char *cmd, char *buf, size_t buflen)
{
	char pin[9];
	size_t len;
	char *pos;
	int ret;

	wpa_hexdump_ascii_key(MSG_DEBUG, "WPS_CHECK_PIN",
			      (u8 *) cmd, os_strlen(cmd));
	for (pos = cmd, len = 0; *pos != '\0'; pos++) {
		if (*pos < '0' || *pos > '9')
			continue;
		pin[len++] = *pos;
		if (len == 9) {
			wpa_printf(MSG_DEBUG, "WPS: Too long PIN");
			return -1;
		}
	}
	if (len != 4 && len != 8) {
		wpa_printf(MSG_DEBUG, "WPS: Invalid PIN length %d", (int) len);
		return -1;
	}
	pin[len] = '\0';

	if (len == 8) {
		unsigned int pin_val;
		pin_val = atoi(pin);
		if (!wps_pin_valid(pin_val)) {
			wpa_printf(MSG_DEBUG, "WPS: Invalid checksum digit");
			ret = os_snprintf(buf, buflen, "FAIL-CHECKSUM\n");
			if (os_snprintf_error(buflen, ret))
				return -1;
			return ret;
		}
	}

	ret = os_snprintf(buf, buflen, "%s", pin);
	if (os_snprintf_error(buflen, ret))
		return -1;

	return ret;
}


#ifdef CONFIG_WPS_NFC
static int hostapd_ctrl_iface_wps_nfc_tag_read(struct hostapd_data *hapd,
					       char *pos)
{
	size_t len;
	struct wpabuf *buf;
	int ret;

	len = os_strlen(pos);
	if (len & 0x01)
		return -1;
	len /= 2;

	buf = wpabuf_alloc(len);
	if (buf == NULL)
		return -1;
	if (hexstr2bin(pos, wpabuf_put(buf, len), len) < 0) {
		wpabuf_free(buf);
		return -1;
	}

	ret = hostapd_wps_nfc_tag_read(hapd, buf);
	wpabuf_free(buf);

	return ret;
}


static int hostapd_ctrl_iface_wps_nfc_config_token(struct hostapd_data *hapd,
						   char *cmd, char *reply,
						   size_t max_len)
{
	int ndef;
	struct wpabuf *buf;
	int res;

	if (os_strcmp(cmd, "WPS") == 0)
		ndef = 0;
	else if (os_strcmp(cmd, "NDEF") == 0)
		ndef = 1;
	else
		return -1;

	buf = hostapd_wps_nfc_config_token(hapd, ndef);
	if (buf == NULL)
		return -1;

	res = wpa_snprintf_hex_uppercase(reply, max_len, wpabuf_head(buf),
					 wpabuf_len(buf));
	reply[res++] = '\n';
	reply[res] = '\0';

	wpabuf_free(buf);

	return res;
}


static int hostapd_ctrl_iface_wps_nfc_token_gen(struct hostapd_data *hapd,
						char *reply, size_t max_len,
						int ndef)
{
	struct wpabuf *buf;
	int res;

	buf = hostapd_wps_nfc_token_gen(hapd, ndef);
	if (buf == NULL)
		return -1;

	res = wpa_snprintf_hex_uppercase(reply, max_len, wpabuf_head(buf),
					 wpabuf_len(buf));
	reply[res++] = '\n';
	reply[res] = '\0';

	wpabuf_free(buf);

	return res;
}


static int hostapd_ctrl_iface_wps_nfc_token(struct hostapd_data *hapd,
					    char *cmd, char *reply,
					    size_t max_len)
{
	if (os_strcmp(cmd, "WPS") == 0)
		return hostapd_ctrl_iface_wps_nfc_token_gen(hapd, reply,
							    max_len, 0);

	if (os_strcmp(cmd, "NDEF") == 0)
		return hostapd_ctrl_iface_wps_nfc_token_gen(hapd, reply,
							    max_len, 1);

	if (os_strcmp(cmd, "enable") == 0)
		return hostapd_wps_nfc_token_enable(hapd);

	if (os_strcmp(cmd, "disable") == 0) {
		hostapd_wps_nfc_token_disable(hapd);
		return 0;
	}

	return -1;
}


static int hostapd_ctrl_iface_nfc_get_handover_sel(struct hostapd_data *hapd,
						   char *cmd, char *reply,
						   size_t max_len)
{
	struct wpabuf *buf;
	int res;
	char *pos;
	int ndef;

	pos = os_strchr(cmd, ' ');
	if (pos == NULL)
		return -1;
	*pos++ = '\0';

	if (os_strcmp(cmd, "WPS") == 0)
		ndef = 0;
	else if (os_strcmp(cmd, "NDEF") == 0)
		ndef = 1;
	else
		return -1;

	if (os_strcmp(pos, "WPS-CR") == 0)
		buf = hostapd_wps_nfc_hs_cr(hapd, ndef);
	else
		buf = NULL;
	if (buf == NULL)
		return -1;

	res = wpa_snprintf_hex_uppercase(reply, max_len, wpabuf_head(buf),
					 wpabuf_len(buf));
	reply[res++] = '\n';
	reply[res] = '\0';

	wpabuf_free(buf);

	return res;
}


static int hostapd_ctrl_iface_nfc_report_handover(struct hostapd_data *hapd,
						  char *cmd)
{
	size_t len;
	struct wpabuf *req, *sel;
	int ret;
	char *pos, *role, *type, *pos2;

	role = cmd;
	pos = os_strchr(role, ' ');
	if (pos == NULL)
		return -1;
	*pos++ = '\0';

	type = pos;
	pos = os_strchr(type, ' ');
	if (pos == NULL)
		return -1;
	*pos++ = '\0';

	pos2 = os_strchr(pos, ' ');
	if (pos2 == NULL)
		return -1;
	*pos2++ = '\0';

	len = os_strlen(pos);
	if (len & 0x01)
		return -1;
	len /= 2;

	req = wpabuf_alloc(len);
	if (req == NULL)
		return -1;
	if (hexstr2bin(pos, wpabuf_put(req, len), len) < 0) {
		wpabuf_free(req);
		return -1;
	}

	len = os_strlen(pos2);
	if (len & 0x01) {
		wpabuf_free(req);
		return -1;
	}
	len /= 2;

	sel = wpabuf_alloc(len);
	if (sel == NULL) {
		wpabuf_free(req);
		return -1;
	}
	if (hexstr2bin(pos2, wpabuf_put(sel, len), len) < 0) {
		wpabuf_free(req);
		wpabuf_free(sel);
		return -1;
	}

	if (os_strcmp(role, "RESP") == 0 && os_strcmp(type, "WPS") == 0) {
		ret = hostapd_wps_nfc_report_handover(hapd, req, sel);
	} else {
		wpa_printf(MSG_DEBUG, "NFC: Unsupported connection handover "
			   "reported: role=%s type=%s", role, type);
		ret = -1;
	}
	wpabuf_free(req);
	wpabuf_free(sel);

	return ret;
}

#endif /* CONFIG_WPS_NFC */


static int hostapd_ctrl_iface_wps_ap_pin(struct hostapd_data *hapd, char *txt,
					 char *buf, size_t buflen)
{
	int timeout = 300;
	char *pos;
	const char *pin_txt;

	hapd = get_bss_index(txt, hapd->iface);
	if (NULL == hapd) {
		return -1;
	}

	txt = os_strchr(txt, ' ');
	if (NULL == txt) {
		return -1;
	}
	txt++;

	pos = os_strchr(txt, ' ');
	if (pos)
		*pos++ = '\0';

	if (os_strcmp(txt, "disable") == 0) {
		hostapd_wps_ap_pin_disable(hapd);
		return os_snprintf(buf, buflen, "OK\n");
	}

	if (os_strcmp(txt, "random") == 0) {
		if (pos)
			timeout = atoi(pos);
		pin_txt = hostapd_wps_ap_pin_random(hapd, timeout);
		if (pin_txt == NULL)
			return -1;
		return os_snprintf(buf, buflen, "%s\n", pin_txt);
	}

	if (os_strcmp(txt, "get") == 0) {
		pin_txt = hostapd_wps_ap_pin_get(hapd);
		if (pin_txt == NULL)
			return -1;
		return os_snprintf(buf, buflen, "%s\n", pin_txt);
	}

	if (os_strcmp(txt, "set") == 0) {
		char *pin;
		if (pos == NULL)
			return -1;
		pin = pos;
		pos = os_strchr(pos, ' ');
		if (pos) {
			*pos++ = '\0';
			timeout = atoi(pos);
		}
		if (os_strlen(pin) > buflen)
			return -1;
		if (hostapd_wps_ap_pin_set(hapd, pin, timeout) < 0)
			return -1;
		return os_snprintf(buf, buflen, "%s\n", pin);
	}

	return -1;
}


static int hostapd_ctrl_iface_wps_config(struct hostapd_data *hapd, char *txt)
{
	char *pos;
	char *ssid, *auth, *encr = NULL, *key = NULL;

	hapd = get_bss_index(txt, hapd->iface);
	if (hapd == NULL) {
		return -1;
	}
	txt = os_strchr(txt, ' ');
	if (NULL == txt) {
		return -1;
	}
	txt++;

	ssid = txt;
	pos = os_strchr(txt, ' ');
	if (!pos)
		return -1;
	*pos++ = '\0';

	auth = pos;
	pos = os_strchr(pos, ' ');
	if (pos) {
		*pos++ = '\0';
		encr = pos;
		pos = os_strchr(pos, ' ');
		if (pos) {
			*pos++ = '\0';
			key = pos;
		}
	}

	return hostapd_wps_config_ap(hapd, ssid, auth, encr, key);
}


static const char * pbc_status_str(enum pbc_status status)
{
	switch (status) {
	case WPS_PBC_STATUS_DISABLE:
		return "Disabled";
	case WPS_PBC_STATUS_ACTIVE:
		return "Active";
	case WPS_PBC_STATUS_TIMEOUT:
		return "Timed-out";
	case WPS_PBC_STATUS_OVERLAP:
		return "Overlap";
	default:
		return "Unknown";
	}
}


static int hostapd_ctrl_iface_wps_get_status(struct hostapd_data *hapd,
					     const char *bss_name, char *buf, size_t buflen)
{
	int ret;
	char *pos, *end;

	pos = buf;
	end = buf + buflen;

	hapd = get_bss_index(bss_name, hapd->iface);
	if (hapd == NULL) {
		return -1;
	}

	ret = os_snprintf(pos, end - pos, "PBC Status: %s\n",
			  pbc_status_str(hapd->wps_stats.pbc_status));

	if (os_snprintf_error(end - pos, ret))
		return pos - buf;
	pos += ret;

	ret = os_snprintf(pos, end - pos, "Last WPS result: %s\n",
			  (hapd->wps_stats.status == WPS_STATUS_SUCCESS ?
			   "Success":
			   (hapd->wps_stats.status == WPS_STATUS_FAILURE ?
			    "Failed" : "None")));

	if (os_snprintf_error(end - pos, ret))
		return pos - buf;
	pos += ret;

	/* If status == Failure - Add possible Reasons */
	if(hapd->wps_stats.status == WPS_STATUS_FAILURE &&
	   hapd->wps_stats.failure_reason > 0) {
		ret = os_snprintf(pos, end - pos,
				  "Failure Reason: %s\n",
				  wps_ei_str(hapd->wps_stats.failure_reason));

		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}

	if (hapd->wps_stats.status) {
		ret = os_snprintf(pos, end - pos, "Peer Address: " MACSTR "\n",
				  MAC2STR(hapd->wps_stats.peer_addr));

		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}

	return pos - buf;
}

#endif /* CONFIG_WPS */

#ifdef CONFIG_HS20

static int hostapd_ctrl_iface_hs20_wnm_notif(struct hostapd_data *hapd,
					     const char *cmd)
{
	u8 addr[ETH_ALEN];
	const char *url;

	hapd = get_bss_index(cmd, hapd->iface);
	if (NULL == hapd) {
		return -1;
	}

	cmd = os_strchr(cmd, ' ');
	if (NULL == cmd) {
		return -1;
	}
	cmd++;

	if (hwaddr_aton(cmd, addr))
		return -1;
	url = cmd + 17;
	if (*url == '\0') {
		url = NULL;
	} else {
		if (*url != ' ')
			return -1;
		url++;
		if (*url == '\0')
			url = NULL;
	}

	return hs20_send_wnm_notification(hapd, addr, 1, url);
}


static int hostapd_ctrl_iface_hs20_deauth_req(struct hostapd_data *hapd,
					      const char *cmd)
{
	u8 addr[ETH_ALEN];
	int code, reauth_delay, ret;
	const char *pos;
	size_t url_len;
	struct wpabuf *req;

	hapd = get_bss_index(cmd, hapd->iface);
	if (NULL == hapd) {
		return -1;
	}

	cmd = os_strchr(cmd, ' ');
	if (NULL == cmd) {
		return -1;
	}
	cmd++;

	/* <STA MAC Addr> <Code(0/1)> <Re-auth-Delay(sec)> [URL] */
	if (hwaddr_aton(cmd, addr))
		return -1;

	pos = os_strchr(cmd, ' ');
	if (pos == NULL)
		return -1;
	pos++;
	code = atoi(pos);

	pos = os_strchr(pos, ' ');
	if (pos == NULL)
		return -1;
	pos++;
	reauth_delay = atoi(pos);

	url_len = 0;
	pos = os_strchr(pos, ' ');
	if (pos) {
		pos++;
		url_len = os_strlen(pos);
	}

	req = wpabuf_alloc(4 + url_len);
	if (req == NULL)
		return -1;
	wpabuf_put_u8(req, code);
	wpabuf_put_le16(req, reauth_delay);
	wpabuf_put_u8(req, url_len);
	if (pos)
		wpabuf_put_data(req, pos, url_len);

	wpa_printf(MSG_DEBUG, "HS 2.0: Send WNM-Notification to " MACSTR
		   " to indicate imminent deauthentication (code=%d "
		   "reauth_delay=%d)", MAC2STR(addr), code, reauth_delay);
	ret = hs20_send_wnm_notification_deauth_req(hapd, addr, req);
	wpabuf_free(req);
	return ret;
}

#endif /* CONFIG_HS20 */

static int hostapd_ctrl_iface_update_wan_metrics(struct hostapd_data *hapd,
						const char *cmd)
{
	char buf[128];

	hapd = get_bss_index(cmd, hapd->iface);
	if (NULL == hapd) {
		return -1;
	}

	cmd = os_strchr(cmd, ' ');
	if (NULL == cmd) {
		return -1;
	}
	cmd++;

	os_strlcpy(buf, cmd, 128);
	wpa_printf(MSG_DEBUG, "hostapd_ctrl_iface_update_wan_metrics, bss=%s", hapd->conf->iface);

	if (hs20_parse_wan_metrics(hapd->conf, buf, 0) != 0)
		return -1;

	return 0;
}

#ifdef CONFIG_INTERWORKING

static int hostapd_ctrl_iface_set_qos_map_set(struct hostapd_data *hapd,
					      const char *cmd)
{
	u8 qos_map_set[16 + 2 * 21], count = 0;
	const char *pos = cmd;
	int val, ret;

	hapd = get_bss_index(pos, hapd->iface);
	if (NULL == hapd) {
		return -1;
	}

	pos = os_strchr(pos, ' ');
	if (NULL == pos) {
		return -1;
	}
	pos++;

	for (;;) {
		if (count == sizeof(qos_map_set)) {
			wpa_printf(MSG_ERROR, "Too many qos_map_set parameters");
			return -1;
		}

		val = atoi(pos);
		if (val < 0 || val > 255) {
			wpa_printf(MSG_INFO, "Invalid QoS Map Set");
			return -1;
		}

		qos_map_set[count++] = val;
		pos = os_strchr(pos, ',');
		if (!pos)
			break;
		pos++;
	}

	if (count < 16 || count & 1) {
		wpa_printf(MSG_INFO, "Invalid QoS Map Set");
		return -1;
	}

	ret = hostapd_drv_set_qos_map(hapd, qos_map_set, count);
	if (ret) {
		wpa_printf(MSG_INFO, "Failed to set QoS Map Set");
		return -1;
	}

	os_memcpy(hapd->conf->qos_map_set, qos_map_set, count);
	hapd->conf->qos_map_set_len = count;

	return 0;
}


static int hostapd_ctrl_iface_send_qos_map_conf(struct hostapd_data *hapd,
						const char *cmd)
{
	u8 addr[ETH_ALEN];
	struct sta_info *sta;
	struct wpabuf *buf;
	u8 *qos_map_set = hapd->conf->qos_map_set;
	u8 qos_map_set_len = hapd->conf->qos_map_set_len;
	int ret;

	hapd = get_bss_index(cmd, hapd->iface);
	if (NULL == hapd) {
		return -1;
	}

	cmd = os_strchr(cmd, ' ');
	if (NULL == cmd) {
		return -1;
	}
	cmd++;

	if (!qos_map_set_len) {
		wpa_printf(MSG_INFO, "QoS Map Set is not set");
		return -1;
	}

	if (hwaddr_aton(cmd, addr))
		return -1;

	sta = ap_get_sta(hapd, addr);
	if (sta == NULL) {
		wpa_printf(MSG_DEBUG, "Station " MACSTR " not found "
			   "for QoS Map Configuration message",
			   MAC2STR(addr));
		return -1;
	}

	if (!sta->qos_map_enabled) {
		wpa_printf(MSG_DEBUG, "Station " MACSTR " did not indicate "
			   "support for QoS Map", MAC2STR(addr));
		return -1;
	}

	buf = wpabuf_alloc(2 + 2 + qos_map_set_len);
	if (buf == NULL)
		return -1;

	wpabuf_put_u8(buf, WLAN_ACTION_QOS);
	wpabuf_put_u8(buf, QOS_QOS_MAP_CONFIG);

	/* QoS Map Set Element */
	wpabuf_put_u8(buf, WLAN_EID_QOS_MAP_SET);
	wpabuf_put_u8(buf, qos_map_set_len);
	wpabuf_put_data(buf, qos_map_set, qos_map_set_len);

	ret = hostapd_drv_send_action(hapd, hapd->iface->freq, 0, addr,
				      wpabuf_head(buf), wpabuf_len(buf));
	wpabuf_free(buf);

	return ret;
}

#endif /* CONFIG_INTERWORKING */


#ifdef CONFIG_WNM

static int hostapd_ctrl_iface_disassoc_imminent(struct hostapd_data *hapd,
						const char *cmd)
{
	u8 addr[ETH_ALEN];
	int disassoc_timer;
	struct sta_info *sta;

	if (hwaddr_aton(cmd, addr))
		return -1;
	if (cmd[17] != ' ')
		return -1;
	disassoc_timer = atoi(cmd + 17);

	sta = ap_get_sta(hapd, addr);
	if (sta == NULL) {
		wpa_printf(MSG_DEBUG, "Station " MACSTR
			   " not found for disassociation imminent message",
			   MAC2STR(addr));
		return -1;
	}

	return wnm_send_disassoc_imminent(hapd, sta, disassoc_timer);
}


static int hostapd_ctrl_iface_ess_disassoc(struct hostapd_data *hapd,
					   const char *cmd)
{
	u8 addr[ETH_ALEN];
	const char *url, *timerstr;
	int disassoc_timer;
	struct sta_info *sta;

	if (hwaddr_aton(cmd, addr))
		return -1;

	sta = ap_get_sta(hapd, addr);
	if (sta == NULL) {
		wpa_printf(MSG_DEBUG, "Station " MACSTR
			   " not found for ESS disassociation imminent message",
			   MAC2STR(addr));
		return -1;
	}

	timerstr = cmd + 17;
	if (*timerstr != ' ')
		return -1;
	timerstr++;
	disassoc_timer = atoi(timerstr);
	if (disassoc_timer < 0 || disassoc_timer > 65535)
		return -1;

	url = os_strchr(timerstr, ' ');
	if (url == NULL)
		return -1;
	url++;

	return wnm_send_ess_disassoc_imminent(hapd, sta, url, disassoc_timer);
}


static int hostapd_ctrl_iface_bss_tm_req(struct hostapd_data *hapd,
					 const char *cmd, char *buf, size_t buflen)
{
	u8 addr[ETH_ALEN];
	const char *pos, *end;
	int disassoc_timer = 0;
	struct sta_info *sta = NULL;
	u8 req_mode = 0, valid_int = 0x01;
	u8 bss_term_dur[12];
	char *url = NULL;
	int i, ret;
	u8 nei_rep[1000];
	u8 *nei_pos = nei_rep;
	u8 mbo[10];
	size_t mbo_len = 0;
	int dialog_token_to_send = 0, ret_dialog_token;

	if (hwaddr_aton(cmd, addr)) {
		wpa_printf(MSG_DEBUG, "Invalid STA MAC address");
		return -1;
	}

	for (i = 0; i < hapd->iface->num_bss; i++) {
		sta = ap_get_sta(hapd->iface->bss[i], addr);
		if (sta) {
			hapd = hapd->iface->bss[i];
			break;
		}
	}

	if (!sta) {
		wpa_printf(MSG_DEBUG, "Station " MACSTR
			" not found for BSS TM Request message",
			   MAC2STR(addr));
		return -1;
	}

	pos = os_strstr(cmd, " dialog_token=");
	if (pos) {
		pos += 14;
		dialog_token_to_send = atoi(pos);
		if (dialog_token_to_send < 0 || dialog_token_to_send > 0xff) {
			wpa_printf(MSG_DEBUG, "Invalid dialog_token");
			return -1;
		}
	}

	pos = os_strstr(cmd, " disassoc_timer=");
	if (pos) {
		pos += 16;
		disassoc_timer = atoi(pos);
		if (disassoc_timer < 0 || disassoc_timer > 65535) {
			wpa_printf(MSG_DEBUG, "Invalid disassoc_timer");
			return -1;
		}
	}

	pos = os_strstr(cmd, " valid_int=");
	if (pos) {
		pos += 11;
		valid_int = atoi(pos);
	}

	pos = os_strstr(cmd, " bss_term=");
	if (pos) {
		pos += 10;
		req_mode |= WNM_BSS_TM_REQ_BSS_TERMINATION_INCLUDED;
		/* TODO: TSF configurable/learnable */
		bss_term_dur[0] = 4; /* Subelement ID */
		bss_term_dur[1] = 10; /* Length */

		//BSS Termination TSF (value of the TSF timer when BSS termination will occur)
		errno = 0;
		u64 tsf = strtoull (pos, NULL, 10);
		if (errno) {
			wpa_printf(MSG_DEBUG, "Invalid TSF value in bss_term data");
			return -1;
		}
		WPA_PUT_LE64(&bss_term_dur[2], tsf);

		//Duration (unsigned 2-octet integer that indicates the number of minutes for which the BSS is not present).
		end = os_strchr(pos, ',');
		if (end == NULL) {
			wpa_printf(MSG_DEBUG, "Invalid bss_term data");
			return -1;
		}
		end++;
		WPA_PUT_LE16(&bss_term_dur[10], (short)atoi(end));
	}


	/*
	 * BSS Transition Candidate List Entries - Neighbor Report elements
	 * neighbor=<BSSID>,<BSSID Information>,<Operating Class>,
	 * <Channel Number>,<PHY Type>[,<hexdump of Optional Subelements>]
	 */
	pos = cmd;
	while (pos) {
		u8 *nei_start;
		long int val;
		char *endptr, *tmp;

		pos = os_strstr(pos, " neighbor=");
		if (!pos)
			break;
		if (nei_pos + 15 > nei_rep + sizeof(nei_rep)) {
			wpa_printf(MSG_DEBUG,
				   "Not enough room for additional neighbor");
			return -1;
		}
		pos += 10;

		nei_start = nei_pos;
		*nei_pos++ = WLAN_EID_NEIGHBOR_REPORT;
		nei_pos++; /* length to be filled in */

		if (hwaddr_aton(pos, nei_pos)) {
			wpa_printf(MSG_DEBUG, "Invalid BSSID");
			return -1;
		}
		nei_pos += ETH_ALEN;
		pos += 17;
		if (*pos != ',') {
			wpa_printf(MSG_DEBUG, "Missing BSSID Information");
			return -1;
		}
		pos++;

		val = strtol(pos, &endptr, 0);
		WPA_PUT_LE32(nei_pos, val);
		nei_pos += 4;
		if (*endptr != ',') {
			wpa_printf(MSG_DEBUG, "Missing Operating Class");
			return -1;
		}
		pos = endptr + 1;

		*nei_pos++ = atoi(pos); /* Operating Class */
		pos = os_strchr(pos, ',');
		if (pos == NULL) {
			wpa_printf(MSG_DEBUG, "Missing Channel Number");
			return -1;
		}
		pos++;

		*nei_pos++ = atoi(pos); /* Channel Number */
		pos = os_strchr(pos, ',');
		if (pos == NULL) {
			wpa_printf(MSG_DEBUG, "Missing PHY Type");
			return -1;
		}
		pos++;

		*nei_pos++ = atoi(pos); /* PHY Type */

		end = os_strchr(pos, ' ');
		tmp = os_strchr(pos, ',');

		if (tmp && (!end || tmp < end)) {
			/* Optional Neighbor Preference */
			pos = os_strchr(pos, ',');
			pos ++;

			//Check if Neighbor preference is valid.
			//More than 3 characters indicates the last parameter is the sub-elements hexdump and not preference
			char * next_delim = os_strchr(pos, ',');
			if ((os_strlen(pos)<=3) || (end && (end - (tmp) <= 4)) || (next_delim && (next_delim - tmp <=4))){

				int cand_pref = atoi(pos);
				if (cand_pref < 0 || cand_pref > 255){
					wpa_printf(MSG_DEBUG, "Invalid BSS Transition Candidate Preference value");
					return -1;
				}

				*nei_pos++ = WNM_NEIGHBOR_BSS_TRANSITION_CANDIDATE;
				*nei_pos++ = 1;
				*nei_pos++ = cand_pref;

				tmp = os_strchr(pos, ',');
			}

			if (tmp && (!end || tmp < end)) {
				/* Optional Subelements (hexdump) */
				size_t len;

				pos = tmp + 1;
				end = os_strchr(pos, ' ');
				if (end)
					len = end - pos;
				else
					len = os_strlen(pos);
				if (nei_pos + len / 2 > nei_rep + sizeof(nei_rep)) {
					wpa_printf(MSG_DEBUG,
						   "Not enough room for neighbor subelements");
					return -1;
				}
				if (len & 0x01 ||
					hexstr2bin(pos, nei_pos, len / 2) < 0) {
					wpa_printf(MSG_DEBUG,
						   "Invalid neighbor subelement info");
					return -1;
				}
				nei_pos += len / 2;
				pos = end;
			}
		}

		nei_start[1] = nei_pos - nei_start - 2;
	}

	pos = os_strstr(cmd, " url=");
	if (pos) {
		size_t len;
		pos += 5;
		end = os_strchr(pos, ' ');
		if (end)
			len = end - pos;
		else
			len = os_strlen(pos);
		url = os_malloc(len + 1);
		if (url == NULL)
			return -1;
		os_memcpy(url, pos, len);
		url[len] = '\0';
		req_mode |= WNM_BSS_TM_REQ_ESS_DISASSOC_IMMINENT;
	}

	if (os_strstr(cmd, " pref=1"))
		req_mode |= WNM_BSS_TM_REQ_PREF_CAND_LIST_INCLUDED;
	if (os_strstr(cmd, " abridged=1"))
		req_mode |= WNM_BSS_TM_REQ_ABRIDGED;
	if (os_strstr(cmd, " disassoc_imminent=1"))
		req_mode |= WNM_BSS_TM_REQ_DISASSOC_IMMINENT;

#ifdef CONFIG_MBO
	pos = os_strstr(cmd, "mbo=");
	if (pos) {

		if (hapd->conf->mbo_enabled && sta->mbo_supported){

			unsigned int mbo_reason, reassoc_delay;
			int cell_pref;
			u8 *mbo_pos = mbo;

			ret = sscanf(pos, "mbo=%u:%u:%d", &mbo_reason,
					 &reassoc_delay, &cell_pref);
			if (ret < 3) {
				wpa_printf(MSG_DEBUG,
					   "BTM request: MBO requires three arguments: mbo=<reason>:<reassoc_delay>:<cell_pref>");
				return -1;
			}

			if (mbo_reason > MBO_TRANSITION_REASON_PREMIUM_AP) {
				wpa_printf(MSG_DEBUG,
					   "BTM request: Invalid MBO transition reason code %u",
					   mbo_reason);
				return -1;
			}

			/* Valid values for Cellular preference are: 0, 1, 255 */
			/* A value of -1 indicates that Cellular Data Connection Preference Attribute will not be added to MBO IE */
			if (cell_pref != -1 && cell_pref != MBO_CELL_PREF_EXCLUDED && cell_pref != MBO_CELL_PREF_NO_USE && cell_pref != MBO_CELL_PREF_USE) {
				wpa_printf(MSG_DEBUG,
					   "BTM request:Invalid MBO cellular capability %u",
					   cell_pref);
				return -1;
			}

			if (reassoc_delay > 65535 ||
				(reassoc_delay &&
				 !(req_mode & WNM_BSS_TM_REQ_DISASSOC_IMMINENT))) {
				wpa_printf(MSG_DEBUG,
					   "BTM request (MBO): Assoc retry delay is only valid in disassoc imminent mode");
				return -1;
			}

			*mbo_pos++ = MBO_ATTR_ID_TRANSITION_REASON;
			*mbo_pos++ = 1;
			*mbo_pos++ = mbo_reason;
			if (cell_pref != -1){
				if (!hapd->conf->mbo_cell_aware)
					wpa_printf(MSG_DEBUG, "BTM request (MBO) Cell preference set, but VAP is not cellular aware");
				else{
					*mbo_pos++ = MBO_ATTR_ID_CELL_DATA_PREF;
					*mbo_pos++ = 1;
					*mbo_pos++ = cell_pref;
				}
			}

			if (reassoc_delay) {
				*mbo_pos++ = MBO_ATTR_ID_ASSOC_RETRY_DELAY;
				*mbo_pos++ = 2;
				WPA_PUT_LE16(mbo_pos, reassoc_delay);
				mbo_pos += 2;
			}

			mbo_len = mbo_pos - mbo;
		} else{
			wpa_printf(MSG_DEBUG,
				   "BTM request: MBO set but VAP is not MBO enabled or STA does not support MBO.");
		}
	}
#endif /* CONFIG_MBO */

	ret_dialog_token = wnm_send_bss_tm_req(hapd, sta, dialog_token_to_send, req_mode,
			disassoc_timer, valid_int, bss_term_dur, url,
			nei_pos > nei_rep ? nei_rep : NULL, nei_pos - nei_rep,
			mbo_len ? mbo : NULL, mbo_len);
	os_free(url);

	if (ret_dialog_token < 0)
		return -1;
	return os_snprintf(buf, buflen, "dialog_token=%d\n", ret_dialog_token);
}

#endif /* CONFIG_WNM */


static int hostapd_ctrl_iface_get_key_mgmt(struct hostapd_data *hapd,
					   char *buf, size_t buflen)
{
	int ret = 0;
	char *pos, *end;

	pos = buf;
	end = buf + buflen;

	WPA_ASSERT(hapd->conf->wpa_key_mgmt);

	if (hapd->conf->wpa_key_mgmt & WPA_KEY_MGMT_PSK) {
		ret = os_snprintf(pos, end - pos, "WPA-PSK ");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}
	if (hapd->conf->wpa_key_mgmt & WPA_KEY_MGMT_IEEE8021X) {
		ret = os_snprintf(pos, end - pos, "WPA-EAP ");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}
#ifdef CONFIG_IEEE80211R
	if (hapd->conf->wpa_key_mgmt & WPA_KEY_MGMT_FT_PSK) {
		ret = os_snprintf(pos, end - pos, "FT-PSK ");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}
	if (hapd->conf->wpa_key_mgmt & WPA_KEY_MGMT_FT_IEEE8021X) {
		ret = os_snprintf(pos, end - pos, "FT-EAP ");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}
#ifdef CONFIG_SAE
	if (hapd->conf->wpa_key_mgmt & WPA_KEY_MGMT_FT_SAE) {
		ret = os_snprintf(pos, end - pos, "FT-SAE ");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}
#endif /* CONFIG_SAE */
#endif /* CONFIG_IEEE80211R */
#ifdef CONFIG_IEEE80211W
	if (hapd->conf->wpa_key_mgmt & WPA_KEY_MGMT_PSK_SHA256) {
		ret = os_snprintf(pos, end - pos, "WPA-PSK-SHA256 ");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}
	if (hapd->conf->wpa_key_mgmt & WPA_KEY_MGMT_IEEE8021X_SHA256) {
		ret = os_snprintf(pos, end - pos, "WPA-EAP-SHA256 ");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}
#endif /* CONFIG_IEEE80211W */
#ifdef CONFIG_SAE
	if (hapd->conf->wpa_key_mgmt & WPA_KEY_MGMT_SAE) {
		ret = os_snprintf(pos, end - pos, "SAE ");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}
#endif /* CONFIG_SAE */
	if (hapd->conf->wpa_key_mgmt & WPA_KEY_MGMT_IEEE8021X_SUITE_B) {
		ret = os_snprintf(pos, end - pos, "WPA-EAP-SUITE-B ");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}
	if (hapd->conf->wpa_key_mgmt &
	    WPA_KEY_MGMT_IEEE8021X_SUITE_B_192) {
		ret = os_snprintf(pos, end - pos,
				  "WPA-EAP-SUITE-B-192 ");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}

	if (pos > buf && *(pos - 1) == ' ') {
		*(pos - 1) = '\0';
		pos--;
	}

	return pos - buf;
}


static int hostapd_ctrl_iface_get_config(struct hostapd_data *hapd,
					 const char *bss_name, char *buf, size_t buflen)
{
	int ret;
	char *pos, *end;

	pos = buf;
	end = buf + buflen;

	hapd = get_bss_index(bss_name, hapd->iface);
	if (hapd == NULL) {
		return -1;
	}

	ret = os_snprintf(pos, end - pos, "bssid=" MACSTR "\n"
			  "ssid=%s\n",
			  MAC2STR(hapd->own_addr),
			  wpa_ssid_txt(hapd->conf->ssid.ssid,
				       hapd->conf->ssid.ssid_len));
	if (os_snprintf_error(end - pos, ret))
		return pos - buf;
	pos += ret;

#ifdef CONFIG_WPS
	ret = os_snprintf(pos, end - pos, "wps_state=%s\n",
			  hapd->conf->wps_state == 0 ? "disabled" :
			  (hapd->conf->wps_state == 1 ? "not configured" :
			   "configured"));
	if (os_snprintf_error(end - pos, ret))
		return pos - buf;
	pos += ret;

	if (hapd->conf->wps_state && hapd->conf->wpa &&
	    hapd->conf->ssid.wpa_passphrase) {
		ret = os_snprintf(pos, end - pos, "passphrase=%s\n",
				  hapd->conf->ssid.wpa_passphrase);
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}

	if (hapd->conf->wps_state && hapd->conf->wpa &&
	    hapd->conf->ssid.wpa_psk &&
	    hapd->conf->ssid.wpa_psk->group) {
		char hex[PMK_LEN * 2 + 1];
		wpa_snprintf_hex(hex, sizeof(hex),
				 hapd->conf->ssid.wpa_psk->psk, PMK_LEN);
		ret = os_snprintf(pos, end - pos, "psk=%s\n", hex);
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}
#endif /* CONFIG_WPS */

	if (hapd->conf->wpa) {
		ret = os_snprintf(pos, end - pos, "wpa=%d\n", hapd->conf->wpa);
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}

	if (hapd->conf->wpa && hapd->conf->wpa_key_mgmt) {
		ret = os_snprintf(pos, end - pos, "key_mgmt=");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;

		pos += hostapd_ctrl_iface_get_key_mgmt(hapd, pos, end - pos);

		ret = os_snprintf(pos, end - pos, "\n");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}

	if (hapd->conf->wpa) {
		ret = os_snprintf(pos, end - pos, "group_cipher=%s\n",
				  wpa_cipher_txt(hapd->conf->wpa_group));
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}

	if ((hapd->conf->wpa & WPA_PROTO_RSN) && hapd->conf->rsn_pairwise) {
		ret = os_snprintf(pos, end - pos, "rsn_pairwise_cipher=");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;

		ret = wpa_write_ciphers(pos, end, hapd->conf->rsn_pairwise,
					" ");
		if (ret < 0)
			return pos - buf;
		pos += ret;

		ret = os_snprintf(pos, end - pos, "\n");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}

	if ((hapd->conf->wpa & WPA_PROTO_WPA) && hapd->conf->wpa_pairwise) {
		ret = os_snprintf(pos, end - pos, "wpa_pairwise_cipher=");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;

		ret = wpa_write_ciphers(pos, end, hapd->conf->wpa_pairwise,
					" ");
		if (ret < 0)
			return pos - buf;
		pos += ret;

		ret = os_snprintf(pos, end - pos, "\n");
		if (os_snprintf_error(end - pos, ret))
			return pos - buf;
		pos += ret;
	}

	return pos - buf;
}


static int hostapd_ctrl_iface_set(struct hostapd_data *hapd, char *cmd)
{
	char *value;
	int ret = 0;

	value = os_strchr(cmd, ' ');
	if (value == NULL)
		return -1;
	*value++ = '\0';

	wpa_printf(MSG_DEBUG, "CTRL_IFACE SET '%s'='%s'", cmd, value);
	if (0) {
#ifdef CONFIG_WPS_TESTING
	} else if (os_strcasecmp(cmd, "wps_version_number") == 0) {
		long int val;
		val = strtol(value, NULL, 0);
		if (val < 0 || val > 0xff) {
			ret = -1;
			wpa_printf(MSG_DEBUG, "WPS: Invalid "
				   "wps_version_number %ld", val);
		} else {
			wps_version_number = val;
			wpa_printf(MSG_DEBUG, "WPS: Testing - force WPS "
				   "version %u.%u",
				   (wps_version_number & 0xf0) >> 4,
				   wps_version_number & 0x0f);
			hostapd_wps_update_ie(hapd);
		}
	} else if (os_strcasecmp(cmd, "wps_testing_dummy_cred") == 0) {
		wps_testing_dummy_cred = atoi(value);
		wpa_printf(MSG_DEBUG, "WPS: Testing - dummy_cred=%d",
			   wps_testing_dummy_cred);
	} else if (os_strcasecmp(cmd, "wps_corrupt_pkhash") == 0) {
		wps_corrupt_pkhash = atoi(value);
		wpa_printf(MSG_DEBUG, "WPS: Testing - wps_corrupt_pkhash=%d",
			   wps_corrupt_pkhash);
#endif /* CONFIG_WPS_TESTING */
#ifdef CONFIG_INTERWORKING
	} else if (os_strcasecmp(cmd, "gas_frag_limit") == 0) {
		int val = atoi(value);
		if (val <= 0)
			ret = -1;
		else
			hapd->gas_frag_limit = val;
#endif /* CONFIG_INTERWORKING */
#ifdef CONFIG_TESTING_OPTIONS
	} else if (os_strcasecmp(cmd, "ext_mgmt_frame_handling") == 0) {
		hapd->ext_mgmt_frame_handling = atoi(value);
	} else if (os_strcasecmp(cmd, "ext_eapol_frame_io") == 0) {
		hapd->ext_eapol_frame_io = atoi(value);
#endif /* CONFIG_TESTING_OPTIONS */
#ifdef CONFIG_MBO
	} else if (os_strcasecmp(cmd, "mbo_assoc_disallow") == 0) {
		int val;

		if (!hapd->conf->mbo_enabled)
			return -1;

		val = atoi(value);
		if (val < 0 || val > 1)
			return -1;

		hapd->mbo_assoc_disallow = val;
		ieee802_11_update_beacons(hapd->iface);

		/*
		 * TODO: Need to configure drivers that do AP MLME offload with
		 * disallowing station logic.
		 */
	} else if (os_strcasecmp(cmd, "mbo_cell_aware") == 0) {
		int val;

		if (!hapd->conf->mbo_enabled) {
			wpa_printf(MSG_DEBUG, "MBO: mbo is not enabled");
			return -1;
		}

		val = atoi(value);
		if (val < 0 || val > 1) {
			wpa_printf(MSG_DEBUG,
				"MBO: mbo_cell_aware value is out of range (can be 0 or 1)");
			return -1;
		}

		hapd->conf->mbo_cell_aware = val;
		ieee802_11_update_beacons(hapd->iface);
#endif /* CONFIG_MBO */
	} else {
		struct sta_info *sta;
		struct vlan_description vlan_id;

		ret = hostapd_set_iface(hapd->iconf, hapd->conf, cmd, value);
		if (ret)
			return ret;

		if (os_strcasecmp(cmd, "deny_mac_file") == 0) {
			for (sta = hapd->sta_list; sta; sta = sta->next) {
				if (hostapd_maclist_found(
					    hapd->conf->deny_mac,
					    hapd->conf->num_deny_mac, sta->addr,
					    &vlan_id) &&
				    (!vlan_id.notempty ||
				     !vlan_compare(&vlan_id, sta->vlan_desc)))
					ap_sta_disconnect(
						hapd, sta, sta->addr,
						WLAN_REASON_UNSPECIFIED);
			}
		} else if (hapd->conf->macaddr_acl == DENY_UNLESS_ACCEPTED &&
			   os_strcasecmp(cmd, "accept_mac_file") == 0) {
			for (sta = hapd->sta_list; sta; sta = sta->next) {
				if (!hostapd_maclist_found(
					    hapd->conf->accept_mac,
					    hapd->conf->num_accept_mac,
					    sta->addr, &vlan_id) ||
				    (vlan_id.notempty &&
				     vlan_compare(&vlan_id, sta->vlan_desc)))
					ap_sta_disconnect(
						hapd, sta, sta->addr,
						WLAN_REASON_UNSPECIFIED);
			}
		}
	}

	return ret;
}


static int hostapd_ctrl_iface_get(struct hostapd_data *hapd, char *cmd,
				  char *buf, size_t buflen)
{
	int res;

	wpa_printf(MSG_DEBUG, "CTRL_IFACE GET '%s'", cmd);

	if (os_strcmp(cmd, "version") == 0) {
		res = os_snprintf(buf, buflen, "%s", VERSION_STR);
		if (os_snprintf_error(buflen, res))
			return -1;
		return res;
	} else if (os_strcmp(cmd, "tls_library") == 0) {
		res = tls_get_library_version(buf, buflen);
		if (os_snprintf_error(buflen, res))
			return -1;
		return res;
	}

	return -1;
}


static int hostapd_ctrl_iface_enable(struct hostapd_iface *iface)
{
	if (hostapd_enable_iface(iface) < 0) {
		wpa_printf(MSG_ERROR, "Enabling of interface failed");
		return -1;
	}
	return 0;
}


static int hostapd_ctrl_iface_reload(struct hostapd_iface *iface)
{
	if (hostapd_reload_iface(iface) < 0) {
		wpa_printf(MSG_ERROR, "Reloading of interface failed");
		return -1;
	}
	return 0;
}


static int hostapd_ctrl_iface_reconf(struct hostapd_iface *iface,
		char *param)
{
	int i, bss_idx = -1;

	while (param[0] == ' ')
		param++;

	if (os_strlen(param) > 0) {
		for (i = 0; i < iface->num_bss; i++) {
			struct hostapd_data *bss = iface->bss[i];

			if (!strncmp(bss->conf->iface, param, IFNAMSIZ)) {
				bss_idx = i;
				break;
			}
		}

		if (bss_idx == 0) {
			wpa_printf(MSG_ERROR, "Reconfig of master BSS is illegal");
			return -1;
		}
	}

	if (hostapd_reconf_iface(iface, bss_idx) < 0) {
		wpa_printf(MSG_ERROR, "Reconfig of interface failed");
		return -1;
	}

	return 0;
}


static int hostapd_ctrl_iface_disable(struct hostapd_iface *iface)
{
	if (hostapd_disable_iface(iface) < 0) {
		wpa_printf(MSG_ERROR, "Disabling of interface failed");
		return -1;
	}
	return 0;
}


#ifdef CONFIG_TESTING_OPTIONS

static int hostapd_ctrl_iface_radar(struct hostapd_data *hapd, char *cmd)
{
	union wpa_event_data data;
	char *pos, *param;
	enum wpa_event_type event;

	wpa_printf(MSG_DEBUG, "RADAR TEST: %s", cmd);

	os_memset(&data, 0, sizeof(data));

	param = os_strchr(cmd, ' ');
	if (param == NULL)
		return -1;
	*param++ = '\0';

	if (os_strcmp(cmd, "DETECTED") == 0)
		event = EVENT_DFS_RADAR_DETECTED;
	else if (os_strcmp(cmd, "CAC-FINISHED") == 0)
		event = EVENT_DFS_CAC_FINISHED;
	else if (os_strcmp(cmd, "CAC-ABORTED") == 0)
		event = EVENT_DFS_CAC_ABORTED;
	else if (os_strcmp(cmd, "NOP-FINISHED") == 0)
		event = EVENT_DFS_NOP_FINISHED;
	else {
		wpa_printf(MSG_DEBUG, "Unsupported RADAR test command: %s",
			   cmd);
		return -1;
	}

	pos = os_strstr(param, "freq=");
	if (pos)
		data.dfs_event.freq = atoi(pos + 5);

	pos = os_strstr(param, "ht_enabled=1");
	if (pos)
		data.dfs_event.ht_enabled = 1;

	pos = os_strstr(param, "chan_offset=");
	if (pos)
		data.dfs_event.chan_offset = atoi(pos + 12);

	pos = os_strstr(param, "chan_width=");
	if (pos)
		data.dfs_event.chan_width = atoi(pos + 11);

	pos = os_strstr(param, "cf1=");
	if (pos)
		data.dfs_event.cf1 = atoi(pos + 4);

	pos = os_strstr(param, "cf2=");
	if (pos)
		data.dfs_event.cf2 = atoi(pos + 4);

	wpa_supplicant_event(hapd, event, &data);

	return 0;
}


static int hostapd_ctrl_iface_mgmt_tx(struct hostapd_data *hapd, char *cmd)
{
	size_t len;
	u8 *buf;
	int res;

	wpa_printf(MSG_DEBUG, "External MGMT TX: %s", cmd);

	len = os_strlen(cmd);
	if (len & 1)
		return -1;
	len /= 2;

	buf = os_malloc(len);
	if (buf == NULL)
		return -1;

	if (hexstr2bin(cmd, buf, len) < 0) {
		os_free(buf);
		return -1;
	}

	res = hostapd_drv_send_mlme(hapd, buf, len, 0);
	os_free(buf);
	return res;
}


static int hostapd_ctrl_iface_eapol_rx(struct hostapd_data *hapd, char *cmd)
{
	char *pos;
	u8 src[ETH_ALEN], *buf;
	int used;
	size_t len;

	wpa_printf(MSG_DEBUG, "External EAPOL RX: %s", cmd);

	pos = cmd;
	used = hwaddr_aton2(pos, src);
	if (used < 0)
		return -1;
	pos += used;
	while (*pos == ' ')
		pos++;

	len = os_strlen(pos);
	if (len & 1)
		return -1;
	len /= 2;

	buf = os_malloc(len);
	if (buf == NULL)
		return -1;

	if (hexstr2bin(pos, buf, len) < 0) {
		os_free(buf);
		return -1;
	}

	ieee802_1x_receive(hapd, src, buf, len);
	os_free(buf);

	return 0;
}


static u16 ipv4_hdr_checksum(const void *buf, size_t len)
{
	size_t i;
	u32 sum = 0;
	const u16 *pos = buf;

	for (i = 0; i < len / 2; i++)
		sum += *pos++;

	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);

	return sum ^ 0xffff;
}


#define HWSIM_PACKETLEN 1500
#define HWSIM_IP_LEN (HWSIM_PACKETLEN - sizeof(struct ether_header))

static void hostapd_data_test_rx(void *ctx, const u8 *src_addr, const u8 *buf,
				 size_t len)
{
	struct hostapd_data *hapd = ctx;
	const struct ether_header *eth;
	struct iphdr ip;
	const u8 *pos;
	unsigned int i;

	if (len != HWSIM_PACKETLEN)
		return;

	eth = (const struct ether_header *) buf;
	os_memcpy(&ip, eth + 1, sizeof(ip));
	pos = &buf[sizeof(*eth) + sizeof(ip)];

	if (ip.ihl != 5 || ip.version != 4 ||
	    ntohs(ip.tot_len) != HWSIM_IP_LEN)
		return;

	for (i = 0; i < HWSIM_IP_LEN - sizeof(ip); i++) {
		if (*pos != (u8) i)
			return;
		pos++;
	}

	wpa_msg(hapd->msg_ctx, MSG_INFO, "DATA-TEST-RX " MACSTR " " MACSTR,
		MAC2STR(eth->ether_dhost), MAC2STR(eth->ether_shost));
}


static int hostapd_ctrl_iface_data_test_config(struct hostapd_data *hapd,
					       char *cmd)
{
	int enabled = atoi(cmd);
	char *pos;
	const char *ifname;

	if (!enabled) {
		if (hapd->l2_test) {
			l2_packet_deinit(hapd->l2_test);
			hapd->l2_test = NULL;
			wpa_dbg(hapd->msg_ctx, MSG_DEBUG,
				"test data: Disabled");
		}
		return 0;
	}

	if (hapd->l2_test)
		return 0;

	pos = os_strstr(cmd, " ifname=");
	if (pos)
		ifname = pos + 8;
	else
		ifname = hapd->conf->iface;

	hapd->l2_test = l2_packet_init(ifname, hapd->own_addr,
					ETHERTYPE_IP, hostapd_data_test_rx,
					hapd, 1);
	if (hapd->l2_test == NULL)
		return -1;

	wpa_dbg(hapd->msg_ctx, MSG_DEBUG, "test data: Enabled");

	return 0;
}


static int hostapd_ctrl_iface_data_test_tx(struct hostapd_data *hapd, char *cmd)
{
	u8 dst[ETH_ALEN], src[ETH_ALEN];
	char *pos;
	int used;
	long int val;
	u8 tos;
	u8 buf[2 + HWSIM_PACKETLEN];
	struct ether_header *eth;
	struct iphdr *ip;
	u8 *dpos;
	unsigned int i;

	if (hapd->l2_test == NULL)
		return -1;

	/* format: <dst> <src> <tos> */

	pos = cmd;
	used = hwaddr_aton2(pos, dst);
	if (used < 0)
		return -1;
	pos += used;
	while (*pos == ' ')
		pos++;
	used = hwaddr_aton2(pos, src);
	if (used < 0)
		return -1;
	pos += used;

	val = strtol(pos, NULL, 0);
	if (val < 0 || val > 0xff)
		return -1;
	tos = val;

	eth = (struct ether_header *) &buf[2];
	os_memcpy(eth->ether_dhost, dst, ETH_ALEN);
	os_memcpy(eth->ether_shost, src, ETH_ALEN);
	eth->ether_type = htons(ETHERTYPE_IP);
	ip = (struct iphdr *) (eth + 1);
	os_memset(ip, 0, sizeof(*ip));
	ip->ihl = 5;
	ip->version = 4;
	ip->ttl = 64;
	ip->tos = tos;
	ip->tot_len = htons(HWSIM_IP_LEN);
	ip->protocol = 1;
	ip->saddr = htonl(192U << 24 | 168 << 16 | 1 << 8 | 1);
	ip->daddr = htonl(192U << 24 | 168 << 16 | 1 << 8 | 2);
	ip->check = ipv4_hdr_checksum(ip, sizeof(*ip));
	dpos = (u8 *) (ip + 1);
	for (i = 0; i < HWSIM_IP_LEN - sizeof(*ip); i++)
		*dpos++ = i;

	if (l2_packet_send(hapd->l2_test, dst, ETHERTYPE_IP, &buf[2],
			   HWSIM_PACKETLEN) < 0)
		return -1;

	wpa_dbg(hapd->msg_ctx, MSG_DEBUG, "test data: TX dst=" MACSTR
		" src=" MACSTR " tos=0x%x", MAC2STR(dst), MAC2STR(src), tos);

	return 0;
}


static int hostapd_ctrl_iface_data_test_frame(struct hostapd_data *hapd,
					      char *cmd)
{
	u8 *buf;
	struct ether_header *eth;
	struct l2_packet_data *l2 = NULL;
	size_t len;
	u16 ethertype;
	int res = -1;
	const char *ifname = hapd->conf->iface;

	if (os_strncmp(cmd, "ifname=", 7) == 0) {
		cmd += 7;
		ifname = cmd;
		cmd = os_strchr(cmd, ' ');
		if (cmd == NULL)
			return -1;
		*cmd++ = '\0';
	}

	len = os_strlen(cmd);
	if (len & 1 || len < ETH_HLEN * 2)
		return -1;
	len /= 2;

	buf = os_malloc(len);
	if (buf == NULL)
		return -1;

	if (hexstr2bin(cmd, buf, len) < 0)
		goto done;

	eth = (struct ether_header *) buf;
	ethertype = ntohs(eth->ether_type);

	l2 = l2_packet_init(ifname, hapd->own_addr, ethertype,
			    hostapd_data_test_rx, hapd, 1);
	if (l2 == NULL)
		goto done;

	res = l2_packet_send(l2, eth->ether_dhost, ethertype, buf, len);
	wpa_dbg(hapd->msg_ctx, MSG_DEBUG, "test data: TX frame res=%d", res);
done:
	if (l2)
		l2_packet_deinit(l2);
	os_free(buf);

	return res < 0 ? -1 : 0;
}


static int hostapd_ctrl_test_alloc_fail(struct hostapd_data *hapd, char *cmd)
{
#ifdef WPA_TRACE_BFD
	char *pos;

	wpa_trace_fail_after = atoi(cmd);
	pos = os_strchr(cmd, ':');
	if (pos) {
		pos++;
		os_strlcpy(wpa_trace_fail_func, pos,
			   sizeof(wpa_trace_fail_func));
	} else {
		wpa_trace_fail_after = 0;
	}

	return 0;
#else /* WPA_TRACE_BFD */
	return -1;
#endif /* WPA_TRACE_BFD */
}


static int hostapd_ctrl_get_alloc_fail(struct hostapd_data *hapd,
				       char *buf, size_t buflen)
{
#ifdef WPA_TRACE_BFD
	return os_snprintf(buf, buflen, "%u:%s", wpa_trace_fail_after,
			   wpa_trace_fail_func);
#else /* WPA_TRACE_BFD */
	return -1;
#endif /* WPA_TRACE_BFD */
}


static int hostapd_ctrl_test_fail(struct hostapd_data *hapd, char *cmd)
{
#ifdef WPA_TRACE_BFD
	char *pos;

	wpa_trace_test_fail_after = atoi(cmd);
	pos = os_strchr(cmd, ':');
	if (pos) {
		pos++;
		os_strlcpy(wpa_trace_test_fail_func, pos,
			   sizeof(wpa_trace_test_fail_func));
	} else {
		wpa_trace_test_fail_after = 0;
	}

	return 0;
#else /* WPA_TRACE_BFD */
	return -1;
#endif /* WPA_TRACE_BFD */
}


static int hostapd_ctrl_get_fail(struct hostapd_data *hapd,
				 char *buf, size_t buflen)
{
#ifdef WPA_TRACE_BFD
	return os_snprintf(buf, buflen, "%u:%s", wpa_trace_test_fail_after,
			   wpa_trace_test_fail_func);
#else /* WPA_TRACE_BFD */
	return -1;
#endif /* WPA_TRACE_BFD */
}

#endif /* CONFIG_TESTING_OPTIONS */

void
set_iface_conf(struct hostapd_iface *iface,
        const struct hostapd_freq_params *freq_params)
{
  int vht_seg0_idx = 0, vht_seg1_idx = 0, vht_bw = VHT_CHANWIDTH_USE_HT;

  if (freq_params->center_freq1)
    vht_seg0_idx = 36 + (freq_params->center_freq1 - 5180) / 5;
  if (freq_params->center_freq2)
    vht_seg1_idx = 36 + (freq_params->center_freq2 - 5180) / 5;

  switch (freq_params->bandwidth) {
  case 0:
  case 20:
  case 40:
    vht_bw = VHT_CHANWIDTH_USE_HT;
    break;
  case 80:
    if (freq_params->center_freq2)
      vht_bw = VHT_CHANWIDTH_80P80MHZ;
    else
      vht_bw = VHT_CHANWIDTH_80MHZ;
    break;
  case 160:
    vht_bw = VHT_CHANWIDTH_160MHZ;
    break;
  default:
    wpa_printf(MSG_WARNING, "Unknown CSA bandwidth: %d",
         freq_params->bandwidth);
    break;
  }

  iface->freq = freq_params->freq;
  iface->conf->channel = freq_params->channel;
  iface->conf->secondary_channel = freq_params->sec_channel_offset;
  if (freq_params->bandwidth > 20 && iface->conf->secondary_channel == 0) {
    if (vht_seg0_idx > iface->conf->channel)
      iface->conf->secondary_channel = 1;
    else
      iface->conf->secondary_channel = -1;
  }
  iface->conf->vht_oper_centr_freq_seg0_idx = vht_seg0_idx;
  iface->conf->vht_oper_centr_freq_seg1_idx = vht_seg1_idx;
  iface->conf->vht_oper_chwidth = vht_bw;
}


static int hostapd_ctrl_iface_chan_switch(struct hostapd_iface *iface,
					  char *pos)
{
#ifdef NEED_AP_MLME
	struct csa_settings settings;
	int ret;
	unsigned int i;
	struct hostapd_channel_data *channel;
	int tx_ant_mask = -1, rx_ant_mask = -1;
	char *str, *pos2;
	Boolean switch_type_scan = FALSE;

	ret = hostapd_parse_csa_settings(pos, &settings);
	if (ret)
		return ret;

	str = os_strstr(pos, "tx_ant_mask=");
  if (str) {
    str += strlen("tx_ant_mask=");

    tx_ant_mask = strtol(str, &pos2, 10);
    if (str == pos2 || tx_ant_mask < 0) {
      wpa_printf(MSG_ERROR, "chan_switch: invalid tx_ant_mask provided");
      return -1;
    }
  }
  str = os_strstr(pos, "rx_ant_mask=");
  if (str) {
    str += strlen("rx_ant_mask=");

    rx_ant_mask = strtol(str, &pos2, 10);
    if (str == pos2 || rx_ant_mask < 0) {
      wpa_printf(MSG_ERROR, "chan_switch: invalid rx_ant_mask provided");
      return -1;
    }
  }
  if ((tx_ant_mask >= 0 && rx_ant_mask < 0) || (rx_ant_mask >= 0 && tx_ant_mask < 0)) {
    wpa_printf(MSG_ERROR, "chan_switch: changing only TX or RX "
      "antenna mask is not possible");
    return -1;
  }
  str = os_strstr(pos, "switch_type=");
  if (str) {
    str += strlen("switch_type=");
    if (!strncmp(str, "scan", strlen("scan")))
      switch_type_scan = TRUE;
  }

  /* Check if active CAC */
  if (iface->cac_started)
    return -1;

  if (tx_ant_mask >= 0) {
      ret = hostapd_drv_set_antenna(iface->bss[0], tx_ant_mask, rx_ant_mask);
      if (ret < 0) {
        wpa_printf(MSG_ERROR, "chan_switch: setting antenna mask failed: %s",
          strerror(errno));
      }
  }

	/* ACS */
	if (settings.freq_params.freq == 0) {
	  if (acs_init(iface) == HOSTAPD_CHAN_ACS)
	    return 0;
	  else
	    return -1;
	}

  channel = hostapd_hw_get_channel_data(iface, settings.freq_params.freq);
  if (!channel)
    return -1;
  /* check if usable */
  if (channel->flag & HOSTAPD_CHAN_DISABLED)
    return -1;

  iface->block_tx = FALSE;
  /* check CAC required */
  if ((channel->flag & HOSTAPD_CHAN_RADAR) &&
      ((channel->flag & HOSTAPD_CHAN_DFS_MASK) != HOSTAPD_CHAN_DFS_AVAILABLE))
  {
    if (switch_type_scan && iface->bss[0]->driver && iface->bss[0]->driver->block_tx) {
      iface->bss[0]->driver->block_tx(iface->bss[0]->drv_priv);
      iface->block_tx = TRUE;
    }
    settings.freq_params.channel = channel->chan;
    set_iface_conf(iface, &settings.freq_params);
    hostapd_setup_interface_complete(iface, 0);
    return 0;
  }

	for (i = 0; i < iface->num_bss; i++) {
		ret = hostapd_switch_channel(iface->bss[i], &settings);
		if (ret) {
			/* FIX: What do we do if CSA fails in the middle of
			 * submitting multi-BSS CSA requests? */
			return ret;
		}
	}

	return 0;
#else /* NEED_AP_MLME */
	return -1;
#endif /* NEED_AP_MLME */
}


static int hostapd_ctrl_iface_mib(struct hostapd_data *hapd, char *reply,
				  int reply_size, const char *param)
{
#ifdef RADIUS_SERVER
	if (os_strcmp(param, "radius_server") == 0) {
		return radius_server_get_mib(hapd->radius_srv, reply,
					     reply_size);
	}
#endif /* RADIUS_SERVER */
	return -1;
}


static int hostapd_ctrl_iface_vendor(struct hostapd_data *hapd, char *cmd,
				     char *buf, size_t buflen)
{
	int ret;
	char *pos;
	u8 *data = NULL;
	unsigned int vendor_id, subcmd;
	struct wpabuf *reply;
	size_t data_len = 0;

	/* cmd: <vendor id> <subcommand id> [<hex formatted data>] */
	vendor_id = strtoul(cmd, &pos, 16);
	if (!isblank((unsigned char) *pos))
		return -EINVAL;

	subcmd = strtoul(pos, &pos, 10);

	if (*pos != '\0') {
		if (!isblank((unsigned char) *pos++))
			return -EINVAL;
		data_len = os_strlen(pos);
	}

	if (data_len) {
		data_len /= 2;
		data = os_malloc(data_len);
		if (!data)
			return -ENOBUFS;

		if (hexstr2bin(pos, data, data_len)) {
			wpa_printf(MSG_DEBUG,
				   "Vendor command: wrong parameter format");
			os_free(data);
			return -EINVAL;
		}
	}

	reply = wpabuf_alloc((buflen - 1) / 2);
	if (!reply) {
		os_free(data);
		return -ENOBUFS;
	}

	ret = hostapd_drv_vendor_cmd(hapd, vendor_id, subcmd, data, data_len,
				     reply);

	if (ret == 0)
		ret = wpa_snprintf_hex(buf, buflen, wpabuf_head_u8(reply),
				       wpabuf_len(reply));

	wpabuf_free(reply);
	os_free(data);

	return ret;
}


static int hostapd_ctrl_iface_eapol_reauth(struct hostapd_data *hapd,
					   const char *cmd)
{
	u8 addr[ETH_ALEN];
	struct sta_info *sta;

	if (hwaddr_aton(cmd, addr))
		return -1;

	sta = ap_get_sta(hapd, addr);
	if (!sta || !sta->eapol_sm)
		return -1;

	eapol_auth_reauthenticate(sta->eapol_sm);
	return 0;
}


static int hostapd_ctrl_iface_eapol_set(struct hostapd_data *hapd, char *cmd)
{
	u8 addr[ETH_ALEN];
	struct sta_info *sta;
	char *pos = cmd, *param;

	if (hwaddr_aton(pos, addr) || pos[17] != ' ')
		return -1;
	pos += 18;
	param = pos;
	pos = os_strchr(pos, ' ');
	if (!pos)
		return -1;
	*pos++ = '\0';

	sta = ap_get_sta(hapd, addr);
	if (!sta || !sta->eapol_sm)
		return -1;

	return eapol_auth_set_conf(sta->eapol_sm, param, pos);
}


static int hostapd_ctrl_iface_log_level(struct hostapd_data *hapd, char *cmd,
					char *buf, size_t buflen)
{
	char *pos, *end, *stamp;
	int ret;

	/* cmd: "LOG_LEVEL [<level>]" */
	if (*cmd == '\0') {
		pos = buf;
		end = buf + buflen;
		ret = os_snprintf(pos, end - pos, "Current level: %s\n"
				  "Timestamp: %d\n",
				  debug_level_str(wpa_debug_level),
				  wpa_debug_timestamp);
		if (os_snprintf_error(end - pos, ret))
			ret = 0;

		return ret;
	}

	while (*cmd == ' ')
		cmd++;

	stamp = os_strchr(cmd, ' ');
	if (stamp) {
		*stamp++ = '\0';
		while (*stamp == ' ') {
			stamp++;
		}
	}

	if (os_strlen(cmd)) {
		int level = str_to_debug_level(cmd);
		if (level < 0)
			return -1;
		wpa_debug_level = level;
	}

	if (stamp && os_strlen(stamp))
		wpa_debug_timestamp = atoi(stamp);

	os_memcpy(buf, "OK\n", 3);
	return 3;
}


#ifdef NEED_AP_MLME
static int hostapd_ctrl_iface_track_sta_list(struct hostapd_data *hapd,
					     char *buf, size_t buflen)
{
	struct hostapd_iface *iface = hapd->iface;
	char *pos, *end;
	struct hostapd_sta_info *info;
	struct os_reltime now;

	if (!iface->num_sta_seen)
		return 0;

	sta_track_expire(iface, 0);

	pos = buf;
	end = buf + buflen;

	os_get_reltime(&now);
	dl_list_for_each_reverse(info, &iface->sta_seen,
				 struct hostapd_sta_info, list) {
		struct os_reltime age;
		int ret;

		os_reltime_sub(&now, &info->last_seen, &age);
		ret = os_snprintf(pos, end - pos, MACSTR " %u\n",
				  MAC2STR(info->addr), (unsigned int) age.sec);
		if (os_snprintf_error(end - pos, ret))
			break;
		pos += ret;
	}

	return pos - buf;
}
#endif /* NEED_AP_MLME */

static int hostapd_ctrl_iface_acs_recalc(struct hostapd_iface *iface)
{
  struct channel_data ch_data;

  memset(&ch_data, 0, sizeof(ch_data));
  hostapd_ltq_update_channel_data(iface, (const u8 *)&ch_data, sizeof(ch_data));
  return 0;
}

static int hostapd_ctrl_iface_req_lci(struct hostapd_data *hapd,
				      const char *cmd)
{
	u8 addr[ETH_ALEN];

	if (hwaddr_aton(cmd, addr)) {
		wpa_printf(MSG_INFO, "CTRL: REQ_LCI: Invalid MAC address");
		return -1;
	}

	return hostapd_send_lci_req(hapd, addr);
}


static int hostapd_ctrl_iface_req_range(struct hostapd_data *hapd, char *cmd)
{
	u8 addr[ETH_ALEN];
	char *token, *context = NULL;
	int random_interval, min_ap;
	u8 responders[ETH_ALEN * RRM_RANGE_REQ_MAX_RESPONDERS];
	unsigned int n_responders;

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_INFO,
			   "CTRL: REQ_RANGE - Bad destination address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token)
		return -1;

	random_interval = atoi(token);
	if (random_interval < 0 || random_interval > 0xffff)
		return -1;

	token = str_token(cmd, " ", &context);
	if (!token)
		return -1;

	min_ap = atoi(token);
	if (min_ap <= 0 || min_ap > WLAN_RRM_RANGE_REQ_MAX_MIN_AP)
		return -1;

	n_responders = 0;
	while ((token = str_token(cmd, " ", &context))) {
		if (n_responders == RRM_RANGE_REQ_MAX_RESPONDERS) {
			wpa_printf(MSG_INFO,
				   "CTRL: REQ_RANGE: Too many responders");
			return -1;
		}

		if (hwaddr_aton(token, responders + n_responders * ETH_ALEN)) {
			wpa_printf(MSG_INFO,
				   "CTRL: REQ_RANGE: Bad responder address");
			return -1;
		}

		n_responders++;
	}

	if (!n_responders) {
		wpa_printf(MSG_INFO,
			   "CTRL: REQ_RANGE - No FTM responder address");
		return -1;
	}

	return hostapd_send_range_req(hapd, addr, random_interval, min_ap,
				      responders, n_responders);
}

#ifdef CONFIG_MBO
static int hostapd_ctrl_iface_mbo_bss_assoc_disallow(struct hostapd_data *hapd, char *cmd)
{
	char *token, *context = NULL;
	int val, i;
	u8 bssid[ETH_ALEN];
	struct hostapd_data *target_bss = NULL;

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, bssid)) {
		wpa_printf(MSG_INFO,
				"CTRL: MBO_BSS_ASSOC_DISALLOW - Bad bssid address");
		return -1;
	}

	for (i = 0; i < hapd->iface->num_bss; i++) {
		if (os_memcmp(bssid, hapd->iface->bss[i]->own_addr, ETH_ALEN) == 0) {
			target_bss = hapd->iface->bss[i];
			break;
		}
	}

	if (!target_bss) {
		wpa_printf(MSG_INFO,
				"CTRL: MBO_BSS_ASSOC_DISALLOW - there is no iface with the given bssid");
		return -1;
	}

	if (!target_bss->conf->mbo_enabled) {
		wpa_printf(MSG_INFO,
				"CTRL: MBO_BSS_ASSOC_DISALLOW - mbo is not enabled");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_INFO,
				"CTRL: MBO_BSS_ASSOC_DISALLOW - enable/disable is missing");
		return -1;
	}

	val = atoi(token);
	if (val < 0 || val > 5) {
		wpa_printf(MSG_INFO,
				"CTRL: MBO_BSS_ASSOC_DISALLOW - enable/disable value is out of range");
		return -1;
	}

	target_bss->mbo_bss_assoc_disallow = val;
	ieee802_11_update_beacons(target_bss->iface);

	return 0;
}
#endif /* CONFIG_MBO */

static int hostapd_ctrl_iface_req_beacon(struct hostapd_data *hapd, char *cmd,
		char *buf, size_t buflen)
{
	u8 addr[ETH_ALEN], bssid[ETH_ALEN];
	char *token, *context = NULL;
	char *pos, *cmd_end;
	int op_class, channel, random_interval, mode, measurement_duration;
	struct wpa_ssid_value ssid, *ssid_p = NULL;
	int num_of_repetitions, measurement_request_mode;
	int ret_dialog_token;

	u8 rep_cond, rep_cond_threshold;
	u8* rep_cond_p = NULL, *rep_cond_threshold_p = NULL;
	u8 rep_detail;
	u8* rep_detail_p = NULL;
	u8 ap_ch_rep[255];
	u8* ap_ch_rep_p = NULL;
	unsigned int ap_ch_rep_len = 0;
	u8 req_elem[255];
	u8* req_elem_p = NULL;
	unsigned int req_elem_len = 0;

	u8 channel_width, channel_center_frequency0, channel_center_frequency1;
	u8 *channel_width_p = NULL;
	u8 *channel_center_frequency0_p = NULL, *channel_center_frequency1_p = NULL;

	cmd_end = cmd + strlen(cmd);

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_BEACON - Bad destination address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
				"CTRL: REQ_BEACON - number of repetitions is missing");
		return -1;
	}
	num_of_repetitions = atoi(token);
	if (num_of_repetitions < 0 || num_of_repetitions > 0xffff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_BEACON - number of repetitions is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_BEACON - measurement request mode is missing");
		return -1;
	}
	measurement_request_mode = atoi(token);
	if (measurement_request_mode < 0 || measurement_request_mode > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_BEACON - measurement request mode is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_BEACON - operating class is missing");
		return -1;
	}
	op_class = atoi(token);
	if (op_class < 0 || op_class > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_BEACON - operation class is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_BEACON - channel is missing");
		return -1;
	}
	channel = atoi(token);
	if (channel < 0 || channel > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_BEACON - channel value is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_BEACON - random interval is missing");
		return -1;
	}
	random_interval = atoi(token);
	if (random_interval < 0 || random_interval > 0xffff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_BEACON - random interval out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_BEACON - measurement duration is missing");
		return -1;
	}
	measurement_duration = atoi(token);
	if (measurement_duration < 0 || measurement_duration > 0xffff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REQ_BEACON - measurement duration out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_BEACON - mode is missing");
		return -1;
	}
	if (!os_strcmp(token, "passive")) {
		mode = 0;
	} else if (!os_strcmp(token, "active")) {
		mode = 1;
	} else if (!os_strcmp(token, "table")) {
		mode = 2;
	} else {
		wpa_printf(MSG_ERROR, "CTRL: REQ_BEACON - mode is invalid");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, bssid)) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_BEACON - bad BSSID address");
		return -1;
	}

	/* optional tokens */
	token = token + strlen(token) + 1;
	if (token >= cmd_end) {
		/* we've reached the end of command (no optinal arguments) */
		goto exit;
	}

	pos = os_strstr(token, "ssid=");
	if (pos) {
		pos += 5;
		if (ssid_parse(pos, &ssid)) {
			wpa_printf(MSG_ERROR, "CTRL: REQ_BEACON - bad SSID");
			return -1;
		}
		ssid_p = &ssid;
	}

	pos = os_strstr(token, "beacon_rep=");
	if (pos) {
		int cond, threshold;
		char *end, *comma;
		pos += 11;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma = os_strchr(pos, ',');
		if (!comma || comma == pos || comma >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_BEACON - report condition format is invalid");
			return -1;
		}
		cond = atoi(pos);
		if (cond < 0 || cond > 10) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_BEACON - report condition is invalid");
			return -1;
		}
		comma++;
		threshold = atoi(comma);
		if (threshold < 0 || threshold > 255) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_BEACON - report condition threshold is invalid");
			return -1;
		}
		rep_cond = (u8) cond;
		rep_cond_p = &rep_cond;
		rep_cond_threshold = (u8) threshold;
		rep_cond_threshold_p = &rep_cond_threshold;
	}

	pos = os_strstr(token, "rep_detail=");
	if (pos) {
		pos += 11;
		if ((pos[1] != ' ' && pos[1] != '\0') ||
			(pos[0] != '0' && pos[0] != '1' && pos[0] != '2')) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_BEACON - report detail value is invalid");
			return -1;
		}
		rep_detail = *pos - '0';
		rep_detail_p = &rep_detail;
	}

	pos = os_strstr(token, "ap_ch_report=");
	if (pos) {
		int ch;
		char *end;
		pos += 13;
		ap_ch_rep_len = 0;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		while (pos && pos < end) {
			ch = atoi(pos);
			ap_ch_rep[ap_ch_rep_len++] = ch;
			pos = os_strchr(pos, ',');
			if (pos)
				pos++;
		}
		if (ap_ch_rep_len) {
			ap_ch_rep_p = ap_ch_rep;
		} else {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_BEACON - ap channel report invalid values");
			return -1;
		}
	}

	pos = os_strstr(token, "req_elements=");
	if (pos) {
		int elem;
		char* end;
		pos += 13;
		req_elem_len = 0;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		while (pos && pos < end) {
			elem = atoi(pos);
			req_elem[req_elem_len++] = elem;
			pos = os_strchr(pos, ',');
			if (pos)
				pos++;
		}
		if (req_elem_len) {
			req_elem_p = req_elem;
		}
	}

	pos = os_strstr(token, "wide_band_ch_switch=");
	if (pos) {
		int ch_width, ch_center_freq0, ch_center_freq1;
		char *end, *comma1, *comma2;
		pos += 20;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma1 = os_strchr(pos, ',');
		if (!comma1 || comma1 == pos || comma1 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_BEACON - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma1++;
		comma2 = os_strchr(comma1, ',');
		if (!comma2 || comma2 == comma1 || comma2 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_BEACON - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma2++;
		ch_width = atoi(pos);
		if (ch_width < 0 || ch_width > 3) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_BEACON - channel width is invalid");
			return -1;
		}
		ch_center_freq0 = atoi(comma1);
		if (ch_center_freq0 < 0 || ch_center_freq0 > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_BEACON - channel center frequency 0 is invalid");
			return -1;
		}
		ch_center_freq1 = atoi(comma2);
		if (ch_center_freq1 < 0 || ch_center_freq1 > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_BEACON - channel center frequency 1 is invalid");
			return -1;
		}
		channel_width = (u8) ch_width;
		channel_width_p = &channel_width;
		channel_center_frequency0 = (u8) ch_center_freq0;
		channel_center_frequency0_p = &channel_center_frequency0;
		channel_center_frequency1 = (u8) ch_center_freq1;
		channel_center_frequency1_p = &channel_center_frequency1;
	}

exit:
	ret_dialog_token = hostapd_send_beacon_req(hapd, addr, num_of_repetitions,
			measurement_request_mode, op_class, channel, random_interval,
			measurement_duration, mode, bssid, ssid_p, rep_cond_p,
			rep_cond_threshold_p,rep_detail_p, ap_ch_rep_p, ap_ch_rep_len,
			req_elem_p, req_elem_len, channel_width_p,
			channel_center_frequency0_p, channel_center_frequency1_p);

	if (ret_dialog_token < 0)
		return -1;
	return os_snprintf(buf, buflen, "dialog_token=%d\n", ret_dialog_token);
}

static int hostapd_ctrl_iface_report_beacon(struct hostapd_data *hapd,
		char *cmd)
{
	char *token, *context = NULL;
	char *pos, *cmd_end;
	u8 addr[ETH_ALEN], bssid[ETH_ALEN];
	int op_class, channel, measurement_duration, dialog_token;
	int measurement_token, measurement_rep_mode;
	int frame_info, rcpi, rsni, antenna_id, parent_tsf;
	unsigned long long start_time;

	u8 channel_width, channel_center_frequency0, channel_center_frequency1;
	u8 *channel_width_p = NULL;
	u8 *channel_center_frequency0_p = NULL, *channel_center_frequency1_p = NULL;
	u8 rep_frame_body_length,  rep_frame_body[256];
	u8 *rep_frame_body_p = NULL, *rep_frame_body_length_p = NULL;

	cmd_end = cmd + strlen(cmd);

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_ERROR, "CTRL: REP_BEACON - Bad destination address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - dialog token is missing");
		return -1;
	}
	dialog_token = atoi(token);
	if (dialog_token < 0 || dialog_token > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - dialog token is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - measurement token is missing");
		return -1;
	}
	measurement_token = atoi(token);
	if (measurement_token < 0 || measurement_token > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - measurement token is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - measurement mode is missing");
		return -1;
	}
	measurement_rep_mode = atoi(token);
	if (measurement_rep_mode < 0 || measurement_rep_mode > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - measurement mode is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - operation class is missing");
		return -1;
	}
	op_class = atoi(token);
	if (op_class < 0 || op_class > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - operation class is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - channel is missing");
		return -1;
	}
	channel = atoi(token);
	if (channel < 0 || channel > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - channel is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - start time is missing");
		return -1;
	}
	start_time = strtoull(token, NULL, 10);
	if (start_time < 0) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - start time is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - measurement duration is missing");
		return -1;
	}
	measurement_duration = atoi(token);
	if (measurement_duration < 0 || measurement_duration > 0xffff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REQ_BEACON - measurement duration out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - frame info is missing");
		return -1;
	}
	frame_info = atoi(token);
	if (frame_info < 0 || frame_info > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - frame info is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - rcpi is missing");
		return -1;
	}
	rcpi = atoi(token);
	if (rcpi < 0 || rcpi > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - rcpi is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - rsni is missing");
		return -1;
	}
	rsni = atoi(token);
	if (rsni < 0 || rsni > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - rsni is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, bssid)) {
		wpa_printf(MSG_ERROR, "CTRL: REP_BEACON - bad BSSID address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - antenna id is missing");
		return -1;
	}
	antenna_id = atoi(token);
	if (antenna_id < 0 || antenna_id > 255) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - antenna id is out of range");
		return -1;
	}


	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - parent tsf is missing");
		return -1;
	}
	parent_tsf = atoi(token);
	if (parent_tsf < 0 || parent_tsf > 0xffffffff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REP_BEACON - parent tsf is out of range");
		return -1;
	}

	/* optional tokens */
	token = token + strlen(token) + 1;
	if (token >= cmd_end) {
		/* we've reached the end of command (no optinal arguments) */
		goto exit;
	}

	pos = os_strstr(token, "rep_frame_body=");
	if (pos) {
		int length;
		char *end, *comma;
		pos += 15;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma = os_strchr(pos, ',');
		if (!comma || comma == pos || comma >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REP_BEACON - reported frame body format is invalid");
			return -1;
		}
		comma++;
		length = atoi(pos);
		if (length < 0 || length > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REP_BEACON - reported frame body length is invalid");
			return -1;
		}
		os_memcpy(rep_frame_body, comma, length);
		rep_frame_body_length = (u8) length;
		rep_frame_body_length_p = &rep_frame_body_length;
		rep_frame_body_p = (u8 *)rep_frame_body;
	}

	pos = os_strstr(token, "wide_band_ch_switch=");
	if (pos) {
		int ch_width, ch_center_freq0, ch_center_freq1;
		char *end, *comma1, *comma2;
		pos += 20;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma1 = os_strchr(pos, ',');
		if (!comma1 || comma1 == pos || comma1 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REP_BEACON - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma1++;
		comma2 = os_strchr(comma1, ',');
		if (!comma2 || comma2 == comma1 || comma2 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REP_BEACON - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma2++;
		ch_width = atoi(pos);
		if (ch_width < 0 || ch_width > 3) {
			wpa_printf(MSG_ERROR,
					"CTRL: REP_BEACON - channel width is invalid");
			return -1;
		}
		ch_center_freq0 = atoi(comma1);
		if (ch_center_freq0 < 0 || ch_center_freq0 > 255) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_CHANNEL_LOAD - channel center frequency 0 is invalid");
			return -1;
		}
		ch_center_freq1 = atoi(comma2);
		if (ch_center_freq1 < 0 || ch_center_freq1 > 255) {
			wpa_printf(MSG_ERROR,
					"CTRL: REP_BEACON - channel center frequency 1 is invalid");
			return -1;
		}
		channel_width = (u8) ch_width;
		channel_width_p = &channel_width;
		channel_center_frequency0 = (u8) ch_center_freq0;
		channel_center_frequency0_p = &channel_center_frequency0;
		channel_center_frequency1 = (u8) ch_center_freq1;
		channel_center_frequency1_p = &channel_center_frequency1;
	}

exit:
	return hostapd_send_beacon_report_resp(hapd, addr, dialog_token,
			measurement_token, measurement_rep_mode, op_class, channel,
			start_time, measurement_duration, frame_info, rcpi, rsni,
			bssid, antenna_id, parent_tsf, channel_width_p,
			channel_center_frequency0_p, channel_center_frequency1_p,
			rep_frame_body_p, rep_frame_body_length_p);
}

static int hostapd_ctrl_iface_req_self_beacon(struct hostapd_data *hapd,
		char *cmd)
{
	char *token, *context = NULL;
	int random_interval, mode, measurement_duration;

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_SELF_BEACON - random interval is missing");
		return -1;
	}
	random_interval = atoi(token);
	if (random_interval < 0 || random_interval > 0xffff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_SELF_BEACON - random interval out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_SELF_BEACON - measurement duration is missing");
		return -1;
	}
	measurement_duration = atoi(token);
	if (measurement_duration < 0 || measurement_duration > 0xffff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REQ_SELF_BEACON - measurement duration out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_SELF_BEACON - mode is missing");
		return -1;
	}
	if (!os_strcmp(token, "passive")) {
		mode = 0;
	} else if (!os_strcmp(token, "active")) {
		mode = 1;
	} else if (!os_strcmp(token, "table")) {
		mode = 2;
	} else {
		wpa_printf(MSG_ERROR, "CTRL: REQ_BEACON - mode is invalid");
		return -1;
	}

	return hostapd_handle_self_beacon_req(hapd, random_interval,
			measurement_duration, mode);
}

static int hostapd_ctrl_iface_req_link_measurement(struct hostapd_data *hapd,
		char *cmd, char *buf, size_t buflen)
{
	u8 addr[ETH_ALEN];
	char *token, *context = NULL;
	int ret_dialog_token;

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_LINK_MEASUREMENT - Bad destination address");
		return -1;
	}

	ret_dialog_token = hostapd_send_link_measurement_req(hapd, addr);

	if (ret_dialog_token < 0)
		return -1;
	return os_snprintf(buf, buflen, "dialog_token=%d\n", ret_dialog_token);
}

static int hostapd_ctrl_iface_link_measurement_report(struct hostapd_data *hapd,
		char *cmd)
{
	u8 addr[ETH_ALEN];
	char *token, *context = NULL;
	char *pos, *cmd_end;

	struct rrm_link_measurement_report link_msmt_report;
	struct dmg_link_margin dmg_margin, *dmg_margin_p = NULL;
	struct dmg_link_adaptation_ack dmg_ack, *dmg_ack_p = NULL;

	os_memset(&link_msmt_report, 0, sizeof(link_msmt_report));
	os_memset(&dmg_margin, 0, sizeof(dmg_margin));
	os_memset(&dmg_ack, 0, sizeof(dmg_ack));

	cmd_end = cmd + strlen(cmd);

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_ERROR, "CTRL: LINK_MEASUREMENT_REPORT - Bad destination address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: LINK_MEASUREMENT_REPORT - dialog token is missing");
		return -1;
	}
	link_msmt_report.dialog_token = atoi(token);
	if (link_msmt_report.dialog_token <= 0
		|| link_msmt_report.dialog_token > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: LINK_MEASUREMENT_REPORT - dialog token is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: LINK_MEASUREMENT_REPORT - rx_ant_id is missing");
		return -1;
	}
	link_msmt_report.rx_ant_id = atoi(token);
	if (link_msmt_report.rx_ant_id <= 0 || link_msmt_report.rx_ant_id > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: LINK_MEASUREMENT_REPORT - rx_ant_id is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: LINK_MEASUREMENT_REPORT - tx_ant_id is missing");
		return -1;
	}
	link_msmt_report.tx_ant_id = atoi(token);
	if (link_msmt_report.tx_ant_id <= 0 || link_msmt_report.tx_ant_id > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: LINK_MEASUREMENT_REPORT - tx_ant_id is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR, "CTRL: LINK_MEASUREMENT_REPORT - rcpi is missing");
		return -1;
	}
	link_msmt_report.rcpi = atoi(token);
	if (link_msmt_report.rcpi < 0 || link_msmt_report.rcpi > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: LINK_MEASUREMENT_REPORT - rcpi is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: LINK_MEASUREMENT_REPORT - rsni is missing");
		return -1;
	}
	link_msmt_report.rsni = atoi(token);
	if (link_msmt_report.rsni < 0 || link_msmt_report.rsni > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: LINK_MEASUREMENT_REPORT - rsni out of range");
		return -1;
	}

	token = token + strlen(token) + 1;
	if (token >= cmd_end) {
		wpa_printf(MSG_ERROR,
			   "CTRL: LINK_MEASUREMENT_REPORT - tpc_report is missing");
		return -1;
	}

	pos = os_strstr(token, "tpc_report=");
	if (pos) {
		char *end, *comma;
		pos += 11;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma = os_strchr(pos, ',');
		if (!comma || comma == pos || comma >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - tpc_report format is invalid");
			return -1;
		}
		link_msmt_report.tpc.tx_power = atoi(pos);
		if (link_msmt_report.tpc.tx_power < 0
			|| link_msmt_report.tpc.tx_power > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - tpc_report.tx_power is invalid");
			return -1;
		}
		comma++;
		link_msmt_report.tpc.link_margin = atoi(comma);
		if (link_msmt_report.tpc.link_margin < 0
			|| link_msmt_report.tpc.link_margin > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - tpc_report.link_margin is invalid");
			return -1;
		}
		link_msmt_report.tpc.eid = WLAN_EID_TPC_REPORT;
		link_msmt_report.tpc.len = 2;
	}

	/* optional tokens */
	pos = os_strstr(token, "dmg_link_margin=");
	if (pos) {
		char *end, *comma1, *comma2, *comma3, *comma4;
		pos += 16;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma1 = os_strchr(pos, ',');
		if (!comma1 || comma1 == pos || comma1 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_link_margin format is invalid");
			return -1;
		}
		comma1++;
		comma2 = os_strchr(comma1, ',');
		if (!comma2 || comma2 == comma1 || comma2 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_link_margin format is invalid");
			return -1;
		}
		comma2++;
		comma3 = os_strchr(comma2, ',');
		if (!comma3 || comma3 == comma2 || comma3 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_link_margin format is invalid");
			return -1;
		}
		comma3++;
		comma4 = os_strchr(comma3, ',');
		if (!comma4 || comma4 == comma3 || comma4 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_link_margin format is invalid");
			return -1;
		}
		comma4++;
		dmg_margin.activity = atoi(pos);
		if (dmg_margin.activity < 0 || dmg_margin.activity > 6) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_margin.activity is invalid");
			return -1;
		}
		dmg_margin.mcs = atoi(comma1);
		if (dmg_margin.mcs < 0 || dmg_margin.mcs > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_margin.mcs is invalid");
			return -1;
		}
		dmg_margin.link_margin = atoi(comma2);
		if (dmg_margin.link_margin < -128 || dmg_margin.link_margin > 127) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_margin.link_margin is invalid");
			return -1;
		}
		dmg_margin.snr = atoi(comma3);
		if (dmg_margin.snr < 0 || dmg_margin.snr > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_margin.snr is invalid");
			return -1;
		}
		dmg_margin.ref_timestamp = atoi(comma4);
		if (dmg_margin.ref_timestamp < 0) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_margin.ref_timestamp is invalid");
			return -1;
		}
		dmg_margin.eid = WLAN_EID_DMG_LINK_MARGIN;
		dmg_margin.len = 8;
		dmg_margin_p = &dmg_margin;
	}

	pos = os_strstr(token, "dmg_link_adapt_ack=");
	if (pos) {
		char *end, *comma;
		pos += 19;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma = os_strchr(pos, ',');
		if (!comma || comma == pos || comma >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_link_adapt_ack format is invalid");
			return -1;
		}
		dmg_ack.activity = atoi(pos);
		if (dmg_ack.activity < 0 || dmg_ack.activity > 6) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_ack.activity is invalid");
			return -1;
		}
		comma++;
		dmg_ack.ref_timestamp = atoi(comma);
		if (dmg_ack.ref_timestamp < 0) {
			wpa_printf(MSG_ERROR,
					"CTRL: LINK_MEASUREMENT_REPORT - dmg_ack.ref_timestamp is invalid");
			return -1;
		}
		dmg_ack.eid = WLAN_EID_DMG_LINK_ADAPTATION_ACK;
		dmg_ack.len = 5;
		dmg_ack_p = &dmg_ack;
	}

	wpa_printf(MSG_DEBUG, MACSTR
			" dialog_token=%d rx_ant_id=%d tx_ant_id=%d rcpi=%d rsni=%d tpc_report=%d,%d dmg_margin=%d,%d,%d,%d,%d dmg_ack=%d,%d",
			MAC2STR(addr), link_msmt_report.dialog_token,
			link_msmt_report.rx_ant_id, link_msmt_report.tx_ant_id,
			link_msmt_report.rcpi, link_msmt_report.rsni,
			link_msmt_report.tpc.tx_power, link_msmt_report.tpc.link_margin,
			dmg_margin.activity, dmg_margin.mcs, dmg_margin.link_margin,
			dmg_margin.snr, dmg_margin.ref_timestamp, dmg_ack.activity,
			dmg_ack.ref_timestamp);

	return hostapd_send_link_measurement_report(hapd, addr, link_msmt_report,
			dmg_margin_p, dmg_ack_p);
}

/* return -1 = error
 * return 1 = optional element exist
 * return 0 = optional element does not exist
 */
static int parse_trigger_reporting_counters(char *token, char *pos, char *str,
		struct rrm_trig_rep_counters *trig_rep_counters)
{
	pos = os_strstr(token, str);
	if (pos) {
		int i;
		char *end, *comma1, *comma2, *counter_pos;
		pos += 22;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma1 = os_strchr(pos, ',');
		if (!comma1 || comma1 == pos || comma1 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_STA_STATISTICS - trig_rep_counters format is invalid");
			return -1;
		}
		comma1++;
		comma2 = os_strchr(comma1, ',');
		if (!comma2 || comma2 == comma1 || comma2 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_STA_STATISTICS - trig_rep_counters format is invalid");
			return -1;
		}
		comma2++;
		(*trig_rep_counters).measurement_count = atoi(pos);
		if ((*trig_rep_counters).measurement_count < 0
				|| (*trig_rep_counters).measurement_count > 0xffffffff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_STA_STATISTICS - measurement_count is invalid");
			return -1;
		}
		(*trig_rep_counters).trigger_timeout = atoi(comma1);
		if ((*trig_rep_counters).trigger_timeout < 0
				|| (*trig_rep_counters).trigger_timeout > 0xffff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_STA_STATISTICS - trigger_timeout is invalid");
			return -1;
		}
		(*trig_rep_counters).counters_trigger_condition = atoi(comma2);
		if ((*trig_rep_counters).counters_trigger_condition < 0
				|| (*trig_rep_counters).counters_trigger_condition > 0xffff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_STA_STATISTICS - counter_trigger_condition is invalid");
			return -1;
		}

		counter_pos = comma2;
		for (i = 0; i < 7; i++) {
			if (((*trig_rep_counters).counters_trigger_condition >> i) & 1) {
				counter_pos = os_strchr(counter_pos, ',');
				if (!counter_pos || counter_pos == pos || counter_pos >= end) {
					wpa_printf(MSG_ERROR,
							"CTRL: REQ_STA_STATISTICS - rep_sta_counters.counters format is invalid");
					return -1;
				}
				counter_pos++;
				(*trig_rep_counters).counters[i] = atoi(counter_pos);
				(*trig_rep_counters).num_of_counters++;
			}
		}

		return 1;
	}

	return 0;
}

static int hostapd_ctrl_iface_req_sta_statistics(struct hostapd_data *hapd,
		char *cmd, char *buf, size_t buflen)
{
	u8 addr[ETH_ALEN], peer_addr[ETH_ALEN];
	char *token, *context = NULL;
	int group_identity, random_interval, measurement_duration;
	int num_of_repetitions, measurement_request_mode, ret;
	char *pos, *cmd_end;
	int ret_dialog_token;

	struct rrm_trig_rep_counters trig_rep_sta_counters;
	struct rrm_trig_rep_counters *trig_rep_sta_counters_p = NULL;
	struct rrm_trig_rep_counters trig_rep_qos_sta_counters;
	struct rrm_trig_rep_counters *trig_rep_qos_sta_counters_p = NULL;
	struct rrm_trig_rep_counters trig_rep_rsna_counters;
	struct rrm_trig_rep_counters *trig_rep_rsna_counters_p = NULL;

	os_memset(&trig_rep_sta_counters, 0, sizeof(trig_rep_sta_counters));
	os_memset(&trig_rep_qos_sta_counters, 0, sizeof(trig_rep_qos_sta_counters));
	os_memset(&trig_rep_rsna_counters, 0, sizeof(trig_rep_rsna_counters));

	cmd_end = cmd + strlen(cmd);
	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_STA_STATISTICS - Bad destination address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_STA_STATISTICS - number of repetitions is missing");
		return -1;
	}
	num_of_repetitions = atoi(token);
	if (num_of_repetitions < 0 || num_of_repetitions > 0xffff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_STA_STATISTICS - number of repetitions is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_STA_STATISTICS - measurement request mode is missing");
		return -1;
	}
	measurement_request_mode = atoi(token);
	if (measurement_request_mode < 0 || measurement_request_mode > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_STA_STATISTICS - measurement request mode is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, peer_addr)) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_STA_STATISTICS - Bad peer address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_STA_STATISTICS - random interval is missing");
		return -1;
	}
	random_interval = atoi(token);
	if (random_interval < 0 || random_interval > 0xffff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_STA_STATISTICS - random interval out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_STA_STATISTICS - measurement duration is missing");
		return -1;
	}
	measurement_duration = atoi(token);
	if (measurement_duration < 0 || measurement_duration > 0xffff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REQ_STA_STATISTICS - measurement duration out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_STA_STATISTICS - channel is missing");
		return -1;
	}
	group_identity = atoi(token);
	if (group_identity < 0 || group_identity > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_STA_STATISTICS - group_identity value is out of range");
		return -1;
	}

	/* optional tokens */
	token = token + strlen(token) + 1;
	if (token >= cmd_end) {
		/* we've reached the end of command (no optinal arguments) */
		goto exit;
	}

	ret = parse_trigger_reporting_counters(token, pos, "trig_rep_sta_counters=",
				&trig_rep_sta_counters);
	if (ret == -1)
		return ret;
	else if (ret == 1)
		trig_rep_sta_counters_p = &trig_rep_sta_counters;

	ret = parse_trigger_reporting_counters(token, pos, "trig_rep_qos_sta_counters=",
			&trig_rep_qos_sta_counters);
	if (ret == -1)
		return ret;
	else if (ret == 1)
		trig_rep_qos_sta_counters_p = &trig_rep_qos_sta_counters;

	ret = parse_trigger_reporting_counters(token, pos, "trig_rep_rsna_counters=",
			&trig_rep_rsna_counters);
	if (ret == -1)
		return ret;
	else if (ret == 1)
		trig_rep_rsna_counters_p = &trig_rep_rsna_counters;

exit:
	ret_dialog_token =  hostapd_send_sta_statistics_req(hapd, addr,
			num_of_repetitions, measurement_request_mode, peer_addr,
			random_interval, measurement_duration, group_identity,
			trig_rep_sta_counters_p, trig_rep_qos_sta_counters_p,
			trig_rep_rsna_counters_p);

	if (ret_dialog_token < 0)
		return -1;
	return os_snprintf(buf, buflen, "dialog_token=%d\n", ret_dialog_token);
}

static int hostapd_ctrl_iface_report_sta_statistics(struct hostapd_data *hapd,
		char *cmd)
{
	u8 addr[ETH_ALEN];
	char *token, *context = NULL;
	char *pos, *cmd_end, *counter_pos;
	int group_identity, measurement_duration, i;
	int dialog_token, measurement_token, measurement_rep_mode;
	u32 statistics_group_data[RRM_STA_STATISTICS_GROUP_DATA_SIZE] = {};
	u32 statistics_group_data_len = 0;

	u8 rep_reason = 0, *rep_reason_p = NULL;

	cmd_end = cmd + strlen(cmd);

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_ERROR, "CTRL: REPORT_STA_STATISTICS - Bad destination address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - dialog_token is missing");
		return -1;
	}
	dialog_token = atoi(token);
	if (dialog_token <= 0 || dialog_token > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - dialog_token is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - measurement_token is missing");
		return -1;
	}
	measurement_token = atoi(token);
	if (measurement_token <= 0 || measurement_token > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - measurement_token is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - measurement_rep_mode is missing");
		return -1;
	}
	measurement_rep_mode = atoi(token);
	if (measurement_rep_mode < 0 || measurement_rep_mode > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - measurement_rep_mode is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - measurement duration is missing");
		return -1;
	}
	measurement_duration = atoi(token);
	if (measurement_duration < 0 || measurement_duration > 0xffff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REPORT_STA_STATISTICS - measurement duration out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - group_identity is missing");
		return -1;
	}
	group_identity = atoi(token);
	if (group_identity < 0 || group_identity > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - group_identity is out of range");
		return -1;
	}

	statistics_group_data_len =
			hostapd_get_statistics_group_data_len(group_identity);

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - statistics_group_data[0] is missing");
		return -1;
	}
	statistics_group_data[0] = atoi(token);
	if ((statistics_group_data[0] < 0)
			|| (statistics_group_data[0] > 0xffffffff)) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_STA_STATISTICS - statistics_group_data[0] is out of range");
		return -1;
	}

	counter_pos = token;
	for (i = 1; i < statistics_group_data_len; i++) {
		counter_pos = os_strchr(counter_pos, ',');
		if (!counter_pos) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_STA_STATISTICS - statistics_group_data[%d] is missing", i);
			return -1;
		}
		counter_pos++;
		if (((group_identity == 11) && ((i == 4) || (i == 8)))
		  ||((group_identity == 12) && ((i == 2) || (i == 6)))) {
			u64 tmp_counter = strtoull(counter_pos, NULL, 10);
			os_memcpy(&statistics_group_data[i], &tmp_counter,
					sizeof(tmp_counter));
			i++;
		} else {
			statistics_group_data[i] = atoi(counter_pos);
			if ((statistics_group_data[i] < 0)
					|| (statistics_group_data[i] > 0xffffffff)) {
				wpa_printf(MSG_ERROR,
					   "CTRL: REPORT_STA_STATISTICS - statistics_group_data[%d] is out of range", i);
				return -1;
			}
		}
	}

	/* optional tokens */
	token = token + strlen(token) + 1;
	if (token >= cmd_end) {
		/* we've reached the end of command (no optinal arguments) */
		goto exit;
	}

	pos = os_strstr(token, "rep_reason=");
	if (pos) {
		char *end;
		pos += 11;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		rep_reason = atoi(pos);
		if (rep_reason < 0 || rep_reason > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_STA_STATISTICS - rep_reason is invalid");
			return -1;
		}
		rep_reason_p = &rep_reason;
	}

exit:
	return hostapd_send_sta_statistics_report(hapd, addr, dialog_token,
			measurement_token, measurement_rep_mode, measurement_duration,
			group_identity, statistics_group_data, statistics_group_data_len,
			rep_reason_p);
}

static int hostapd_ctrl_iface_req_channel_load(struct hostapd_data *hapd,
		char *cmd, char *buf, size_t buflen)
{
	u8 addr[ETH_ALEN];
	char *token, *context = NULL;
	char *pos, *cmd_end;
	int op_class, channel, random_interval, measurement_duration;
	int num_of_repetitions, measurement_request_mode;
	int ret_dialog_token;

	u8 rep_cond, ch_load_ref_val;
	u8 *rep_cond_p = NULL, *ch_load_ref_val_p = NULL;

	u8 channel_width, channel_center_frequency0, channel_center_frequency1;
	u8 *channel_width_p = NULL;
	u8 *channel_center_frequency0_p = NULL, *channel_center_frequency1_p = NULL;

	cmd_end = cmd + strlen(cmd);

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_CHANNEL_LOAD - Bad destination address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_CHANNEL_LOAD - number of repetitions is missing");
		return -1;
	}
	num_of_repetitions = atoi(token);
	if (num_of_repetitions < 0 || num_of_repetitions > 0xffff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_CHANNEL_LOAD - number of repetitions is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_CHANNEL_LOAD - measurement request mode is missing");
		return -1;
	}
	measurement_request_mode = atoi(token);
	if (measurement_request_mode < 0 || measurement_request_mode > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_CHANNEL_LOAD - measurement request mode is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_CHANNEL_LOAD - operating class is missing");
		return -1;
	}
	op_class = atoi(token);
	if (op_class <= 0 || op_class > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_CHANNEL_LOAD - operation class is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_CHANNEL_LOAD - channel is missing");
		return -1;
	}
	channel = atoi(token);
	if (channel <= 0 || channel > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_CHANNEL_LOAD - channel value is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_CHANNEL_LOAD - random interval is missing");
		return -1;
	}
	random_interval = atoi(token);
	if (random_interval < 0 || random_interval > 0xffff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_CHANNEL_LOAD - random interval out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_CHANNEL_LOAD - measurement duration is missing");
		return -1;
	}
	measurement_duration = atoi(token);
	if (measurement_duration < 0 || measurement_duration > 0xffff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REQ_CHANNEL_LOAD - measurement duration out of range");
		return -1;
	}

	/* optional tokens */
	token = token + strlen(token) + 1;
	if (token >= cmd_end) {
		/* we've reached the end of command (no optinal arguments) */
		goto exit;
	}

	pos = os_strstr(token, "ch_load_rep=");
	if (pos) {
		int cond, ref_val;
		char *end, *comma;
		pos += 12;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma = os_strchr(pos, ',');
		if (!comma || comma == pos || comma >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_CHANNEL_LOAD - channel load reporting format is invalid");
			return -1;
		}
		cond = atoi(pos);
		if (cond < 0 || cond > 2) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_CHANNEL_LOAD - channel load reporting condition is invalid");
			return -1;
		}
		comma++;
		ref_val = atoi(comma);
		if (ref_val < 0 || ref_val > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_CHANNEL_LOAD - channel load reporting ref val is invalid");
			return -1;
		}
		rep_cond = (u8) cond;
		rep_cond_p = &rep_cond;
		ch_load_ref_val = (u8) ref_val;
		ch_load_ref_val_p = &ch_load_ref_val;
	}

	pos = os_strstr(token, "wide_band_ch_switch=");
	if (pos) {
		int ch_width, ch_center_freq0, ch_center_freq1;
		char *end, *comma1, *comma2;
		pos += 20;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma1 = os_strchr(pos, ',');
		if (!comma1 || comma1 == pos || comma1 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_CHANNEL_LOAD - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma1++;
		comma2 = os_strchr(comma1, ',');
		if (!comma2 || comma2 == comma1 || comma2 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_CHANNEL_LOAD - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma2++;
		ch_width = atoi(pos);
		if (ch_width < 0 || ch_width > 3) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_CHANNEL_LOAD - channel width is invalid");
			return -1;
		}
		ch_center_freq0 = atoi(comma1);
		if (ch_center_freq0 < 0 || ch_center_freq0 > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_CHANNEL_LOAD - channel center frequency 0 is invalid");
			return -1;
		}
		ch_center_freq1 = atoi(comma2);
		if (ch_center_freq1 < 0 || ch_center_freq1 > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_CHANNEL_LOAD - channel center frequency 1 is invalid");
			return -1;
		}
		channel_width = (u8) ch_width;
		channel_width_p = &channel_width;
		channel_center_frequency0 = (u8) ch_center_freq0;
		channel_center_frequency0_p = &channel_center_frequency0;
		channel_center_frequency1 = (u8) ch_center_freq1;
		channel_center_frequency1_p = &channel_center_frequency1;
	}

exit:
	ret_dialog_token = hostapd_send_channel_load_req(hapd, addr,
			num_of_repetitions, measurement_request_mode, op_class, channel,
			random_interval, measurement_duration, rep_cond_p,
			ch_load_ref_val_p, channel_width_p, channel_center_frequency0_p,
			channel_center_frequency1_p);

	if (ret_dialog_token < 0)
		return -1;
	return os_snprintf(buf, buflen, "dialog_token=%d\n", ret_dialog_token);
}

static int hostapd_ctrl_iface_report_channel_load(struct hostapd_data *hapd,
		char *cmd)
{
	u8 addr[ETH_ALEN];
	char *token, *context = NULL;
	char *pos, *cmd_end;
	int op_class, channel, measurement_duration;
	int dialog_token, measurement_token, measurement_rep_mode, channel_load;
	u64 start_time;

	u8 channel_width, channel_center_frequency0, channel_center_frequency1;
	u8 *channel_width_p = NULL;
	u8 *channel_center_frequency0_p = NULL, *channel_center_frequency1_p = NULL;

	cmd_end = cmd + strlen(cmd);

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_ERROR, "CTRL: REPORT_CHANNEL_LOAD - Bad destination address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - dialog_token is missing");
		return -1;
	}
	dialog_token = atoi(token);
	if (dialog_token <= 0 || dialog_token > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - dialog_token is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - measurement_token is missing");
		return -1;
	}
	measurement_token = atoi(token);
	if (measurement_token <= 0 || measurement_token > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - measurement_token is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - measurement_rep_mode is missing");
		return -1;
	}
	measurement_rep_mode = atoi(token);
	if (measurement_rep_mode < 0 || measurement_rep_mode > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - measurement_rep_mode is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - operating class is missing");
		return -1;
	}
	op_class = atoi(token);
	if (op_class <= 0 || op_class > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - operation class is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR, "CTRL: REPORT_CHANNEL_LOAD - channel is missing");
		return -1;
	}
	channel = atoi(token);
	if (channel <= 0 || channel > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - channel value is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - start_time is missing");
		return -1;
	}
	start_time = strtoull (token, NULL, 10);
	if (start_time <= 0) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - start_time out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - measurement duration is missing");
		return -1;
	}
	measurement_duration = atoi(token);
	if (measurement_duration < 0 || measurement_duration > 0xffff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REPORT_CHANNEL_LOAD - measurement duration out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_CHANNEL_LOAD - channel_load is missing");
		return -1;
	}
	channel_load = atoi(token);
	if (channel_load < 0 || channel_load > 0xff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REPORT_CHANNEL_LOAD - channel_load out of range");
		return -1;
	}

	/* optional tokens */
	token = token + strlen(token) + 1;
	if (token >= cmd_end) {
		/* we've reached the end of command (no optinal arguments) */
		goto exit;
	}

	pos = os_strstr(token, "wide_band_ch_switch=");
	if (pos) {
		int ch_width, ch_center_freq0, ch_center_freq1;
		char *end, *comma1, *comma2;
		pos += 20;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma1 = os_strchr(pos, ',');
		if (!comma1 || comma1 == pos || comma1 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_CHANNEL_LOAD - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma1++;
		comma2 = os_strchr(comma1, ',');
		if (!comma2 || comma2 == comma1 || comma2 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_CHANNEL_LOAD - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma2++;
		ch_width = atoi(pos);
		if (ch_width < 0 || ch_width > 3) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_CHANNEL_LOAD - channel width is invalid");
			return -1;
		}
		ch_center_freq0 = atoi(comma1);
		if (ch_center_freq0 < 0 || ch_center_freq0 > 255) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_CHANNEL_LOAD - channel center frequency 0 is invalid");
			return -1;
		}
		ch_center_freq1 = atoi(comma2);
		if (ch_center_freq1 < 0 || ch_center_freq1 > 255) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_CHANNEL_LOAD - channel center frequency 1 is invalid");
			return -1;
		}
		channel_width = (u8) ch_width;
		channel_width_p = &channel_width;
		channel_center_frequency0 = (u8) ch_center_freq0;
		channel_center_frequency0_p = &channel_center_frequency0;
		channel_center_frequency1 = (u8) ch_center_freq1;
		channel_center_frequency1_p = &channel_center_frequency1;
	}

exit:
	return hostapd_send_channel_load_report(hapd, addr, dialog_token,
			measurement_token, measurement_rep_mode, op_class, channel,
			start_time, measurement_duration, channel_load, channel_width_p,
			channel_center_frequency0_p, channel_center_frequency1_p);
}

static int hostapd_ctrl_iface_req_noise_histogram(struct hostapd_data *hapd,
		char *cmd, char *buf, size_t buflen)
{
	u8 addr[ETH_ALEN];
	char *token, *context = NULL;
	char *pos, *cmd_end;
	int op_class, channel, random_interval, measurement_duration;
	int num_of_repetitions, measurement_request_mode;
	int ret_dialog_token;

	u8 rep_cond, anpi_ref_val;
	u8 *rep_cond_p = NULL, *anpi_ref_val_p = NULL;

	u8 channel_width, channel_center_frequency0, channel_center_frequency1;
	u8 *channel_width_p = NULL;
	u8 *channel_center_frequency0_p = NULL, *channel_center_frequency1_p = NULL;

	cmd_end = cmd + strlen(cmd);

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_NOISE_HISTOGRAM - Bad destination address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_NOISE_HISTOGRAM - number of repetitions is missing");
		return -1;
	}
	num_of_repetitions = atoi(token);
	if (num_of_repetitions < 0 || num_of_repetitions > 0xffff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_NOISE_HISTOGRAM - number of repetitions is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_NOISE_HISTOGRAM - measurement request mode is missing");
		return -1;
	}
	measurement_request_mode = atoi(token);
	if (measurement_request_mode < 0 || measurement_request_mode > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_NOISE_HISTOGRAM - measurement request mode is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_NOISE_HISTOGRAM - operating class is missing");
		return -1;
	}
	op_class = atoi(token);
	if (op_class <= 0 || op_class > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_NOISE_HISTOGRAM - operation class is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR, "CTRL: REQ_NOISE_HISTOGRAM - channel is missing");
		return -1;
	}
	channel = atoi(token);
	if (channel <= 0 || channel > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_NOISE_HISTOGRAM - channel value is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_NOISE_HISTOGRAM - random interval is missing");
		return -1;
	}
	random_interval = atoi(token);
	if (random_interval < 0 || random_interval > 0xffff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_NOISE_HISTOGRAM - random interval out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REQ_NOISE_HISTOGRAM - measurement duration is missing");
		return -1;
	}
	measurement_duration = atoi(token);
	if (measurement_duration < 0 || measurement_duration > 0xffff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REQ_NOISE_HISTOGRAM - measurement duration out of range");
		return -1;
	}

	/* optional tokens */
	token = token + strlen(token) + 1;
	if (token >= cmd_end) {
		/* we've reached the end of command (no optinal arguments) */
		goto exit;
	}

	pos = os_strstr(token, "noise_histogram_rep=");
	if (pos) {
		int cond, ref_val;
		char *end, *comma;
		pos += 20;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma = os_strchr(pos, ',');
		if (!comma || comma == pos || comma >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_NOISE_HISTOGRAM - noise histogram reporting format is invalid");
			return -1;
		}
		cond = atoi(pos);
		if (cond < 0 || cond > 2) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_NOISE_HISTOGRAM - noise histogram reporting condition is invalid");
			return -1;
		}
		comma++;
		ref_val = atoi(comma);
		if (ref_val < 0 || ref_val > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_NOISE_HISTOGRAM - channel load reporting ref val is invalid");
			return -1;
		}
		rep_cond = (u8) cond;
		rep_cond_p = &rep_cond;
		anpi_ref_val = (u8) ref_val;
		anpi_ref_val_p = &anpi_ref_val;
	}

	pos = os_strstr(token, "wide_band_ch_switch=");
	if (pos) {
		int ch_width, ch_center_freq0, ch_center_freq1;
		char *end, *comma1, *comma2;
		pos += 20;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma1 = os_strchr(pos, ',');
		if (!comma1 || comma1 == pos || comma1 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_NOISE_HISTOGRAM - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma1++;
		comma2 = os_strchr(comma1, ',');
		if (!comma2 || comma2 == comma1 || comma2 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_NOISE_HISTOGRAM - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma2++;
		ch_width = atoi(pos);
		if (ch_width < 0 || ch_width > 3) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_NOISE_HISTOGRAM - channel width is invalid");
			return -1;
		}
		ch_center_freq0 = atoi(comma1);
		if (ch_center_freq0 < 0 || ch_center_freq0 > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_NOISE_HISTOGRAM - channel center frequency 0 is invalid");
			return -1;
		}
		ch_center_freq1 = atoi(comma2);
		if (ch_center_freq1 < 0 || ch_center_freq1 > 0xff) {
			wpa_printf(MSG_ERROR,
					"CTRL: REQ_NOISE_HISTOGRAM - channel center frequency 1 is invalid");
			return -1;
		}
		channel_width = (u8) ch_width;
		channel_width_p = &channel_width;
		channel_center_frequency0 = (u8) ch_center_freq0;
		channel_center_frequency0_p = &channel_center_frequency0;
		channel_center_frequency1 = (u8) ch_center_freq1;
		channel_center_frequency1_p = &channel_center_frequency1;
	}

exit:
	ret_dialog_token = hostapd_send_noise_histogram_req(hapd, addr,
			num_of_repetitions, measurement_request_mode, op_class, channel,
			random_interval, measurement_duration, rep_cond_p, anpi_ref_val_p,
			channel_width_p, channel_center_frequency0_p,
			channel_center_frequency1_p);

	if (ret_dialog_token < 0)
		return -1;
	return os_snprintf(buf, buflen, "dialog_token=%d\n", ret_dialog_token);
}

static int hostapd_ctrl_iface_report_noise_histogram(struct hostapd_data *hapd,
		char *cmd)
{
	u8 addr[ETH_ALEN];
	char *token, *context = NULL, *ipi_density_pos;
	char *pos, *cmd_end;
	int op_class, channel, measurement_duration, anpi, ant_id;
	int dialog_token, measurement_token, measurement_rep_mode, i;
	u64 start_time;
	u8 ipi_density[RRM_NOISE_HISTO_IPI_DENSITY_SIZE] = {};

	u8 channel_width, channel_center_frequency0, channel_center_frequency1;
	u8 *channel_width_p = NULL;
	u8 *channel_center_frequency0_p = NULL, *channel_center_frequency1_p = NULL;

	cmd_end = cmd + strlen(cmd);

	token = str_token(cmd, " ", &context);
	if (!token || hwaddr_aton(token, addr)) {
		wpa_printf(MSG_ERROR, "CTRL: REPORT_NOISE_HISTOGRAM - Bad destination address");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - dialog_token is missing");
		return -1;
	}
	dialog_token = atoi(token);
	if (dialog_token <= 0 || dialog_token > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - dialog_token is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - measurement_token is missing");
		return -1;
	}
	measurement_token = atoi(token);
	if (measurement_token <= 0 || measurement_token > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - measurement_token is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - measurement_rep_mode is missing");
		return -1;
	}
	measurement_rep_mode = atoi(token);
	if (measurement_rep_mode < 0 || measurement_rep_mode > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - measurement_rep_mode is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - operating class is missing");
		return -1;
	}
	op_class = atoi(token);
	if (op_class <= 0 || op_class > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - operation class is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR, "CTRL: REPORT_NOISE_HISTOGRAM - channel is missing");
		return -1;
	}
	channel = atoi(token);
	if (channel <= 0 || channel > 0xff) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - channel value is out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - start_time is missing");
		return -1;
	}
	start_time = strtoull (token, NULL, 10);
	if (start_time <= 0) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - start_time out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - measurement duration is missing");
		return -1;
	}
	measurement_duration = atoi(token);
	if (measurement_duration < 0 || measurement_duration > 0xffff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REPORT_NOISE_HISTOGRAM - measurement duration out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - ant_id is missing");
		return -1;
	}
	ant_id = atoi(token);
	if (ant_id < 0 || ant_id > 0xff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REPORT_NOISE_HISTOGRAM - ant_id out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - anpi is missing");
		return -1;
	}
	anpi = atoi(token);
	if (anpi < 0 || anpi > 0xff) {
		wpa_printf(MSG_ERROR,
				"CTRL: REPORT_NOISE_HISTOGRAM - anpi out of range");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - ipi_density[0] is missing");
		return -1;
	}
	ipi_density[0] = atoi(token);
	if ((ipi_density[0] < 0) || (ipi_density[0] > 0xff)) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REPORT_NOISE_HISTOGRAM - ipi_density[0] is out of range");
		return -1;
	}

	ipi_density_pos = token;
	for (i = 1; i < RRM_NOISE_HISTO_IPI_DENSITY_SIZE; i++) {
		ipi_density_pos = os_strchr(ipi_density_pos, ',');
		if (!ipi_density_pos) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_NOISE_HISTOGRAM - ipi_density[%d] is missing", i);
			return -1;
		}
		ipi_density_pos++;
		ipi_density[i] = atoi(ipi_density_pos);
		if ((ipi_density[i] < 0) || (ipi_density[i] > 0xff)) {
			wpa_printf(MSG_ERROR,
				   "CTRL: REPORT_NOISE_HISTOGRAM - ipi_density[%d] is out of range", i);
			return -1;
		}
	}

	/* optional tokens */
	token = token + strlen(token) + 1;
	if (token >= cmd_end) {
		/* we've reached the end of command (no optinal arguments) */
		goto exit;
	}

	pos = os_strstr(token, "wide_band_ch_switch=");
	if (pos) {
		int ch_width, ch_center_freq0, ch_center_freq1;
		char *end, *comma1, *comma2;
		pos += 20;
		end = os_strchr(pos, ' ');
		if (!end) {
			end = pos + os_strlen(pos);
		}
		comma1 = os_strchr(pos, ',');
		if (!comma1 || comma1 == pos || comma1 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_NOISE_HISTOGRAM - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma1++;
		comma2 = os_strchr(comma1, ',');
		if (!comma2 || comma2 == comma1 || comma2 >= end) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_NOISE_HISTOGRAM - wide_band_ch_switch format is invalid");
			return -1;
		}
		comma2++;
		ch_width = atoi(pos);
		if (ch_width < 0 || ch_width > 3) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_NOISE_HISTOGRAM - channel width is invalid");
			return -1;
		}
		ch_center_freq0 = atoi(comma1);
		if (ch_center_freq0 < 0 || ch_center_freq0 > 255) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_NOISE_HISTOGRAM - channel center frequency 0 is invalid");
			return -1;
		}
		ch_center_freq1 = atoi(comma2);
		if (ch_center_freq1 < 0 || ch_center_freq1 > 255) {
			wpa_printf(MSG_ERROR,
					"CTRL: REPORT_NOISE_HISTOGRAM - channel center frequency 1 is invalid");
			return -1;
		}
		channel_width = (u8) ch_width;
		channel_width_p = &channel_width;
		channel_center_frequency0 = (u8) ch_center_freq0;
		channel_center_frequency0_p = &channel_center_frequency0;
		channel_center_frequency1 = (u8) ch_center_freq1;
		channel_center_frequency1_p = &channel_center_frequency1;
	}

exit:
	return hostapd_send_noise_histogram_report(hapd, addr, dialog_token,
			measurement_token, measurement_rep_mode, op_class, channel,
			start_time, measurement_duration, ant_id, anpi, ipi_density,
			channel_width_p, channel_center_frequency0_p,
			channel_center_frequency1_p);
}

static int hostapd_ctrl_iface_set_neighbor(struct hostapd_data *hapd, char *buf)
{
	struct wpa_ssid_value ssid;
	u8 bssid[ETH_ALEN];
	struct wpabuf *nr, *lci = NULL, *civic = NULL;
	char *tmp;
	int ret;

	if (!(hapd->conf->radio_measurements[0] &
	      WLAN_RRM_CAPS_NEIGHBOR_REPORT)) {
		wpa_printf(MSG_ERROR,
			   "CTRL: SET_NEIGHBOR: Neighbor report is not enabled");
		return -1;
	}

	if (hwaddr_aton(buf, bssid)) {
		wpa_printf(MSG_ERROR, "CTRL: SET_NEIGHBOR: Bad BSSID");
		return -1;
	}

	tmp = os_strstr(buf, "ssid=");
	if (!tmp || ssid_parse(tmp + 5, &ssid)) {
		wpa_printf(MSG_ERROR,
			   "CTRL: SET_NEIGHBOR: Bad or missing SSID");
		return -1;
	}
	buf = os_strchr(tmp + 6, tmp[5] == '"' ? '"' : ' ');
	if (!buf)
		return -1;

	tmp = os_strstr(buf, "nr=");
	if (!tmp) {
		wpa_printf(MSG_ERROR,
			   "CTRL: SET_NEIGHBOR: Missing Neighbor Report element");
		return -1;
	}

	buf = os_strchr(tmp, ' ');
	if (buf)
		*buf++ = '\0';

	nr = wpabuf_parse_bin(tmp + 3);
	if (!nr) {
		wpa_printf(MSG_ERROR,
			   "CTRL: SET_NEIGHBOR: Bad Neighbor Report element");
		return -1;
	}

	if (!buf)
		goto set;

	tmp = os_strstr(buf, "lci=");
	if (tmp) {
		buf = os_strchr(tmp, ' ');
		if (buf)
			*buf++ = '\0';
		lci = wpabuf_parse_bin(tmp + 4);
		if (!lci) {
			wpa_printf(MSG_ERROR,
				   "CTRL: SET_NEIGHBOR: Bad LCI subelement");
			wpabuf_free(nr);
			return -1;
		}
	}

	if (!buf)
		goto set;

	tmp = os_strstr(buf, "civic=");
	if (tmp) {
		buf = os_strchr(tmp, ' ');
		if (buf)
			*buf++ = '\0';
		civic = wpabuf_parse_bin(tmp + 6);
		if (!civic) {
			wpa_printf(MSG_ERROR,
				   "CTRL: SET_NEIGHBOR: Bad civic subelement");
			wpabuf_free(nr);
			wpabuf_free(lci);
			return -1;
		}
	}

set:
	ret = hostapd_neighbor_set(hapd, bssid, &ssid, nr, lci, civic);

	wpabuf_free(nr);
	wpabuf_free(lci);
	wpabuf_free(civic);

	return ret;
}


static int hostapd_ctrl_iface_remove_neighbor(struct hostapd_data *hapd,
					      char *buf)
{
	struct wpa_ssid_value ssid;
	u8 bssid[ETH_ALEN];
	char *tmp;

	if (hwaddr_aton(buf, bssid)) {
		wpa_printf(MSG_ERROR, "CTRL: REMOVE_NEIGHBOR: Bad BSSID");
		return -1;
	}

	tmp = os_strstr(buf, "ssid=");
	if (!tmp || ssid_parse(tmp + 5, &ssid)) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REMOVE_NEIGHBORr: Bad or missing SSID");
		return -1;
	}

	return hostapd_neighbor_remove(hapd, bssid, &ssid);
}


static int hostapd_ctrl_driver_flags(struct hostapd_iface *iface, char *buf,
				     size_t buflen)
{
	int ret, i;
	char *pos, *end;

	ret = os_snprintf(buf, buflen, "%016llX:\n",
			  (long long unsigned) iface->drv_flags);
	if (os_snprintf_error(buflen, ret))
		return -1;

	pos = buf + ret;
	end = buf + buflen;

	for (i = 0; i < 64; i++) {
		if (iface->drv_flags & (1LLU << i)) {
			ret = os_snprintf(pos, end - pos, "%s\n",
					  driver_flag_to_string(1LLU << i));
			if (os_snprintf_error(end - pos, ret))
				return -1;
			pos += ret;
		}
	}

	return pos - buf;
}


int hostapd_ctrl_iface_deny_mac(struct hostapd_data *hapd, const char *cmd)
{
  u8 addr[ETH_ALEN];
  const char *pos;
  struct hostapd_iface *iface = hapd->iface;
  u8 remove = 0;
  int ret, i;

  if (hwaddr_aton(cmd, addr))
    return -1;

  pos = os_strchr(cmd, ' ');
  if (pos) {
    pos++;
    if (os_strcmp(pos, "1") == 0)
      remove = 1;
  }

  for (i = 0; i < iface->num_bss; i++) {
    struct hostapd_data *bss = iface->bss[i];

    ret = hostapd_drv_set_deny_mac(bss, addr, remove);
    if (ret < 0)
      return ret;
  }
  return 0;
}

int hostapd_ctrl_iface_sta_steer(struct hostapd_data *hapd, const char *cmd)
{
  u8 sta_addr[ETH_ALEN], bssid[ETH_ALEN];
  const char *pos, *btm_parameters = NULL;
  struct hostapd_iface *iface = hapd->iface;
  struct sta_info *sta = NULL;
  int ret, i;

  if (hwaddr_aton(cmd, sta_addr)){
    wpa_printf(MSG_DEBUG, "Steer STA, invalid station MAC Address.");
    return -1;
  }

  os_memset(bssid, 0, sizeof(bssid));
  pos = os_strchr(cmd, ' ');
  if (pos && (pos+1) != '\0') {
      pos++;
    if (hwaddr_aton(pos, bssid)){
      btm_parameters = pos;
    } else{
      btm_parameters = os_strchr(pos + 1, ' ');
    }
    }
  for (i = 0; i < hapd->iface->num_bss; i++) {
    sta = ap_get_sta(hapd->iface->bss[i], sta_addr);
    if (sta) {
      hapd = hapd->iface->bss[i];
      break;
    }
  }

  if (!sta || !sta->btm_supported) {
    /*Station is not associated or does not support BSS Transition Management.
    Use black list mechanism .*/

    wpa_printf(MSG_DEBUG, "Steer STA " MACSTR " , station is not associated to BSS "
         "or does not support BTM.", MAC2STR(sta_addr));

    ret = hostapd_drv_sta_steer(hapd, sta_addr, bssid);
    for (i = 0; i < iface->num_bss; i++) {
      struct hostapd_data *bss = iface->bss[i];

      /* remove STA from all BSS, except the specified one */
      if (os_memcmp(bss->own_addr, bssid, sizeof(bssid))) {
        hostapd_drv_sta_disassoc(bss, sta_addr, WLAN_REASON_UNSPECIFIED);
        sta = ap_get_sta(bss, sta_addr);
        if (sta)
          ap_sta_disassociate(bss, sta, WLAN_REASON_UNSPECIFIED);
      }
    }
  } else{
    /* Station supports 802.11v BTM, send BSS Transition Management
    Request instead of black listing station. */
    char buf[2000], ret_buf[32];
    wpa_printf(MSG_DEBUG, "Steer STA " MACSTR ", station supports BTM.", MAC2STR(sta_addr));

    os_snprintf(buf, sizeof(buf), MACSTR " %s", MAC2STR(sta_addr), btm_parameters ? btm_parameters : "");

    ret = hostapd_ctrl_iface_bss_tm_req(hapd, buf, ret_buf, 32);

    if (ret > 0)
      ret = 0;
  }

  return ret;
}

int hostapd_ctrl_iface_set_vsie(struct hostapd_data *hapd, const char *cmd)
{
  int ret = 0, mfid=-1;
  char *pos;

  wpa_printf(MSG_DEBUG, "%s:%s:%d: cmd: %s",
    __FILE__, __FUNCTION__, __LINE__, cmd);

  if (cmd[0] == ' ')
    cmd++;
  if (strlen(cmd) > 0) {
    pos = cmd;
    wpa_printf(MSG_DEBUG, "%s:%s:%d: pos: %s",
      __FILE__, __FUNCTION__, __LINE__, pos);
    mfid = atoi(pos);
    wpa_printf(MSG_DEBUG, "%s:%s:%d: mfid: %d",
      __FILE__, __FUNCTION__, __LINE__, mfid);

    pos = os_strchr(pos, ' ');

    switch (mfid) {
      case 0:
        wpa_printf(MSG_DEBUG, "%s:%s:%d: beacon vsie: %s",
          __FILE__, __FUNCTION__, __LINE__, pos);
        if (hapd->conf->vendor_elements) {
  			struct wpabuf *wpatmp;
  			wpatmp = hapd->conf->vendor_elements;
  			hapd->conf->vendor_elements = NULL;
        	wpabuf_free(wpatmp);
        }
		hapd->conf->vendor_elements = wpabuf_parse_bin(pos+1);
		if (ieee802_11_set_beacon(hapd))
          ret = -1;
        break;
      case 1:
        wpa_printf(MSG_DEBUG, "%s:%s:%d: probe resp vsie: %s",
          __FILE__, __FUNCTION__, __LINE__, pos);
        if (hapd->conf->proberesp_elements) {
  			struct wpabuf *wpatmp = hapd->conf->proberesp_elements;
        	hapd->conf->proberesp_elements = NULL;
        	wpabuf_free(wpatmp);
        }
        hapd->conf->proberesp_elements = wpabuf_parse_bin(pos+1);
        if (ieee802_11_set_beacon(hapd))
          ret = -1;
        break;
      default:
        wpa_printf(MSG_DEBUG, "%s:%s:%d: mfid: %d not supported",
          __FILE__, __FUNCTION__, __LINE__, mfid);
        ret = -1;
        break;
    }
  }

  return ret;
}

int hostapd_ctrl_iface_del_vsie(struct hostapd_data *hapd, const char *cmd)
{
  int ret = 0, mfid=-1;
  char *pos;
  struct wpabuf *parsed_buf;

  wpa_printf(MSG_DEBUG, "%s:%s:%d: cmd: %s",
    __FILE__, __FUNCTION__, __LINE__, cmd);

  if (cmd[0] == ' ')
    cmd++;
  if (strlen(cmd) > 0) {
    pos = cmd;
    wpa_printf(MSG_DEBUG, "%s:%s:%d: pos: %s",
      __FILE__, __FUNCTION__, __LINE__, pos);
    mfid = atoi(pos);
    wpa_printf(MSG_DEBUG, "%s:%s:%d: mfid: %d",
      __FILE__, __FUNCTION__, __LINE__, mfid);

    pos = os_strchr(pos, ' ');

    switch (mfid) {
      case 0:
        wpa_printf(MSG_DEBUG, "%s:%s:%d: beacon vsie: %s",
          __FILE__, __FUNCTION__, __LINE__, pos);
        parsed_buf = wpabuf_parse_bin(pos+1);
		if (hapd->conf->vendor_elements && 
			memcmp(hapd->conf->vendor_elements->buf, parsed_buf->buf, hapd->conf->vendor_elements->size) == 0) {
  			struct wpabuf *wpatmp;
			wpatmp = hapd->conf->vendor_elements;
			hapd->conf->vendor_elements = NULL;
			wpabuf_free(wpatmp);
			if (ieee802_11_set_beacon(hapd))
	          ret = -1;
		} else {
	        wpa_printf(MSG_DEBUG, "%s:%s:%d: beacon vsie doesn't match: %s",
	          __FILE__, __FUNCTION__, __LINE__, pos);
			ret = -1;
		}
		wpabuf_free(parsed_buf);
        break;
      case 1:
        wpa_printf(MSG_DEBUG, "%s:%s:%d: probe resp vsie: %s",
          __FILE__, __FUNCTION__, __LINE__, pos);
        parsed_buf = wpabuf_parse_bin(pos+1);
		if (hapd->conf->proberesp_elements && 
			memcmp(hapd->conf->proberesp_elements->buf, parsed_buf->buf, hapd->conf->proberesp_elements->size) == 0) {
  			struct wpabuf *wpatmp;
			wpatmp = hapd->conf->proberesp_elements;
			hapd->conf->proberesp_elements = NULL;
			wpabuf_free(wpatmp);
	        if (ieee802_11_set_beacon(hapd))
	          ret = -1;
		} else {
	        wpa_printf(MSG_DEBUG, "%s:%s:%d: probe resp vsie doesn't match: %s",
	          __FILE__, __FUNCTION__, __LINE__, pos);
			ret = -1;
		}
		wpabuf_free(parsed_buf);
        break;
      default:
        wpa_printf(MSG_DEBUG, "%s:%s:%d: mfid: %d not supported",
          __FILE__, __FUNCTION__, __LINE__, mfid);
        ret = -1;
        break;
    }
  }

  return ret;
}


struct hostapd_data *get_bss_index(const char *cmd, struct hostapd_iface *iface)
{
  char *pos_end;
  char bss_name[IFNAMSIZ + 1];
  int i;

  pos_end = os_strchr(cmd, ' ');
  if (!pos_end)
    pos_end = (char *)cmd + strlen(cmd);
  memcpy(bss_name, cmd, pos_end - cmd);
  bss_name[pos_end - cmd] = '\0';
  for (i = 0; i < iface->num_bss; i++) {
    struct hostapd_data *bss = iface->bss[i];

    if (!strncmp(bss->conf->iface, bss_name, IFNAMSIZ))
      return bss;
  }
  return NULL;
}

int hostapd_ctrl_iface_sta_allow(struct hostapd_data *hapd, const char *cmd)
{
  u8 sta_addr[ETH_ALEN];
  int i, ret = 0, count = 0;
  const char *pos;
  struct hostapd_iface *iface = hapd->iface;
  u8 *stations = NULL;

    if (cmd[0] == ' ')
      cmd++;
    if (strlen(cmd) > 0) {
      pos = cmd;
      /* count the number of stations and verify the validity */
      while (pos && pos != '\0') {
        if (hwaddr_aton(pos, sta_addr))
          return -1;
        count++;
        pos = os_strchr(pos, ' ');
        if (pos)
          pos++;
      }
      if (count == 0)
        return -1;
      stations = os_malloc(count * ETH_ALEN);
      if (!stations)
        return -ENOMEM;
      pos = cmd;
      for(i = 0; i < count; i++) {
        hwaddr_aton(pos, stations + i * ETH_ALEN);
        pos = os_strchr(pos, ' ') + 1;
      }
    }


    for (i = 0; i < iface->num_bss; i++) {
      struct hostapd_data *bss = iface->bss[i];

      ret = hostapd_drv_sta_allow(bss, stations, count);
      if (ret < 0)
        break;
    }

  os_free(stations);
  return ret;
}

int hostapd_ctrl_iface_set_bss_load(struct hostapd_data *hapd,
  const char *cmd, char *buf, size_t buflen)
{
  int ret;
  u8 is_enable;

  hapd = get_bss_index(cmd, hapd->iface);
  if (hapd == NULL) {
    ret = os_snprintf(buf, buflen, "FAIL\n");
    if (ret < 0 || (size_t)ret >= buflen)
      return 0;
    return ret;
  }

  cmd = os_strchr(cmd, ' ');
  if (cmd) {
    cmd++;
    is_enable = atoi(cmd);
    if (is_enable > 1) {
      wpa_printf(MSG_ERROR, "set_bss_load: invalid value");
      return -1;
    }
  }
  else {
    ret = os_snprintf(buf, buflen, "FAIL\n");
    if (ret < 0 || (size_t)ret >= buflen)
      return 0;
    return ret;
  }

  ret = bss_load_enable(hapd, is_enable);
  if (ret) {
    ret = os_snprintf(buf, buflen, "FAIL\n");
    if (ret < 0 || (size_t)ret >= buflen)
      return 0;
    return ret;
  }

  return 0;
}

int hostapd_ctrl_iface_get_sta_measurements(struct hostapd_data *hapd,
  const char *cmd, char *buf, size_t buflen)
{
  u8 sta_addr[ETH_ALEN];
  int ret, len = 0;
  mtlk_sta_info_t sta_info;
  struct sta_info *sta;

  hapd = get_bss_index(cmd, hapd->iface);
  if (hapd == NULL) {
    ret = os_snprintf(buf, buflen, "FAIL\n");
    if (ret < 0 || (size_t) ret >= buflen)
      return 0;
    return ret;
  }

  cmd = os_strchr(cmd, ' ');
  if (cmd)
    cmd++;
  else {
    ret = os_snprintf(buf, buflen, "FAIL\n");
    if (ret < 0 || (size_t) ret >= buflen)
      return 0;
    return ret;
  }

  if (hwaddr_aton(cmd, sta_addr)) {
    ret = os_snprintf(buf, buflen, "FAIL\n");
    if (ret < 0 || (size_t) ret >= buflen)
      return 0;
    return ret;
  }

  sta = ap_get_sta(hapd, sta_addr);
  if (sta == NULL) {
    ret = os_snprintf(buf, buflen, "FAIL\n");
    if (ret < 0 || (size_t) ret >= buflen)
      return 0;
    return ret;
  }

  ret = hostapd_drv_get_sta_measurements(hapd, sta_addr, &sta_info);
  if (ret) {
    ret = os_snprintf(buf, buflen, "FAIL\n");
    if (ret < 0 || (size_t) ret >= buflen)
      return 0;
    return ret;
  }

  /* Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}. */
  ret = os_snprintf(buf, buflen, "MACAddress="MACSTR"\n", MAC2STR(sta_addr));
  if (ret >= buflen || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "OperatingStandard=");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  if (sta_info.peer_stats.NetModesSupported & MTLK_WSSA_11A_SUPPORTED) {
    ret = os_snprintf(buf + len, buflen - len, "a ");
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;
  }
  if (sta_info.peer_stats.NetModesSupported & MTLK_WSSA_11B_SUPPORTED) {
    ret = os_snprintf(buf + len, buflen - len, "b ");
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;
  }
  if (sta_info.peer_stats.NetModesSupported & MTLK_WSSA_11G_SUPPORTED) {
    ret = os_snprintf(buf + len, buflen - len, "g ");
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;
  }
  if (sta_info.peer_stats.NetModesSupported & MTLK_WSSA_11N_SUPPORTED) {
    ret = os_snprintf(buf + len, buflen - len, "n ");
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;
  }
  if (sta_info.peer_stats.NetModesSupported & MTLK_WSSA_11AC_SUPPORTED) {
    ret = os_snprintf(buf + len, buflen - len, "ac ");
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;
  }
  /* replace the last space with newline */
  buf[len - 1] = '\n';
  ret = os_snprintf(buf + len, buflen - len,
    "AuthenticationState=%s\n",
    sta ? (sta->flags & WLAN_STA_AUTH ? "1" : "0") : "UNKNOWN");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "LastDataDownlinkRate=%d\n",
    sta_info.peer_stats.LastDataDownlinkRate);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "LastDataUplinkRate=%d\n",
    sta_info.peer_stats.LastDataUplinkRate);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "SignalStrength=%d\n",
    sta_info.peer_stats.SignalStrength);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "ShortTermRSSIAverage=%d %d %d %d\n",
  sta_info.peer_stats.ShortTermRSSIAverage[0],
  sta_info.peer_stats.ShortTermRSSIAverage[1],
  sta_info.peer_stats.ShortTermRSSIAverage[2],
  sta_info.peer_stats.ShortTermRSSIAverage[3]);
  if (ret >= buflen - len || ret < 0)
	return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "Retransmissions=%d\n",
    sta_info.peer_stats.Retransmissions);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "Active=%s\n", sta ? "1" : "0");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  /* Device.WiFi.AccessPoint.{i}.AssociatedDevice.{i}.Stats. */
  ret = os_snprintf(buf + len, buflen - len, "BytesSent=%d\n",
    sta_info.peer_stats.traffic_stats.BytesSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "BytesReceived=%d\n",
    sta_info.peer_stats.traffic_stats.BytesReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "PacketsSent=%d\n",
    sta_info.peer_stats.traffic_stats.PacketsSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "PacketsReceived=%d\n",
    sta_info.peer_stats.traffic_stats.PacketsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "ErrorsSent=UNKNOWN\n");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "RetransCount=UNKNOWN\n");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "FailedRetransCount=UNKNOWN\n");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "RetryCount=UNKNOWN\n");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "MultipleRetryCount=UNKNOWN\n");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  /* ret = os_snprintf(buf + len, buflen - len, "PsState=%d\n",
    sta_info.stationInfo & STA_INFO_PS_STATE);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;*/

  return len;
}


int hostapd_ctrl_iface_get_vap_measurements(struct hostapd_data *hapd,
  const char *cmd, char *buf, size_t buflen)
{
  int ret, len = 0;
  struct mtlk_vap_info vap_info;

  hapd = get_bss_index(cmd, hapd->iface);
  if (hapd == NULL) {
    ret = os_snprintf(buf, buflen, "FAIL: bss index\n");
    if (ret < 0 || (size_t) ret >= buflen)
      return 0;
    return ret;
  }

  ret = hostapd_drv_get_vap_measurements(hapd, &vap_info);
  if (ret) {
    ret = os_snprintf(buf, buflen, "FAIL: vap measurements\n");
    if (ret < 0 || (size_t) ret >= buflen)
      return 0;
    return ret;
  }

  ret = os_snprintf(buf + len, buflen - len, "Name=%s\n",
    hapd->conf->iface);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "Enable=%d\n",
    hapd->started);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "BSSID="MACSTR"\n",
    MAC2STR(hapd->own_addr));
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "SSID=%s\n",
    wpa_ssid_txt(hapd->conf->ssid.ssid,
      hapd->conf->ssid.ssid_len));
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  /* Device.WiFi.SSID.{i}.Stats. */
  ret = os_snprintf(buf + len, buflen - len, "BytesSent=%d\n",
		  vap_info.vap_stats.traffic_stats.BytesSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "BytesReceived=%d\n",
		  vap_info.vap_stats.traffic_stats.BytesReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "PacketsSent=%d\n",
		  vap_info.vap_stats.traffic_stats.PacketsSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "PacketsReceived=%d\n",
		  vap_info.vap_stats.traffic_stats.PacketsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "ErrorsSent=%d\n",
		  vap_info.vap_stats.error_stats.ErrorsSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "RetransCount=%d\n",
		  vap_info.vap_stats.RetransCount);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "FailedRetransCount=%d\n",
		  vap_info.vap_stats.FailedRetransCount);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "RetryCount=%d\n",
		  vap_info.vap_stats.RetryCount);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "MultipleRetryCount=%d\n",
		  vap_info.vap_stats.MultipleRetryCount);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "ACKFailureCount=%d\n",
		  vap_info.vap_stats.ACKFailureCount);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "AggregatedPacketCount=%d\n",
		  vap_info.vap_stats.AggregatedPacketCount);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "ErrorsReceived=%d\n",
		  vap_info.vap_stats.error_stats.ErrorsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "UnicastPacketsSent=%d\n",
		  vap_info.vap_stats.traffic_stats.UnicastPacketsSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "UnicastPacketsReceived=%d\n",
		  vap_info.vap_stats.traffic_stats.UnicastPacketsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "DiscardPacketsSent=%d\n",
		  vap_info.vap_stats.error_stats.DiscardPacketsSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "DiscardPacketsReceived=%d\n",
		  vap_info.vap_stats.error_stats.DiscardPacketsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "MulticastPacketsSent=%d\n",
		  vap_info.vap_stats.traffic_stats.MulticastPacketsSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len,
    "MulticastPacketsReceived=%d\n",
	vap_info.vap_stats.traffic_stats.MulticastPacketsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len,
    "BroadcastPacketsSent=%d\n",
	vap_info.vap_stats.traffic_stats.BroadcastPacketsSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len,
    "BroadcastPacketsReceived=%d\n",
	vap_info.vap_stats.traffic_stats.BroadcastPacketsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len,
    "UnknownProtoPacketsReceived=%d\n",
	vap_info.vap_stats.UnknownProtoPacketsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;

  return len;
}

int hostapd_ctrl_iface_get_radio_state (enum hostapd_iface_state state)
{
	switch (state) {
	case HAPD_IFACE_ENABLED:
		return 1;
	case HAPD_IFACE_ACS_DONE:
		return 2;
	default:
		return 0;
	}
}

int hostapd_ctrl_iface_get_radio_info(struct hostapd_data *hapd,
  const char *cmd, char *buf, size_t buflen)
{
  int ret = 0, len = 0;
  mtlk_radio_info_t radio_info;
  enum hostapd_iface_state state = hapd->iface->state;

  ret = hostapd_drv_get_radio_info(hapd, &radio_info);
  if (ret) {
    ret = os_snprintf(buf, buflen, "FAIL\n");
    if (ret < 0 || (size_t) ret >= buflen)
      return 0;
    return ret;
  }

  ret = os_snprintf(buf + len, buflen - len, "Name=%s\n",
    hapd->conf->iface);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "HostapdEnabled=%d\n",
      hostapd_ctrl_iface_get_radio_state(state));
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  /* Device.WiFi.Radio.{i}. */
  ret = os_snprintf(buf + len, buflen - len, "TxEnabled=%d\n",
      radio_info.hw.Enable);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "Channel=%d\n",
      radio_info.hw.Channel);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  /* Device.WiFi.Radio.{i}.Stats. */
  ret = os_snprintf(buf + len, buflen - len, "BytesSent=%d\n",
    radio_info.hw_stats.traffic_stats.BytesSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "BytesReceived=%d\n",
    radio_info.hw_stats.traffic_stats.BytesReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "PacketsSent=%d\n",
    radio_info.hw_stats.traffic_stats.PacketsSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "PacketsReceived=%d\n",
    radio_info.hw_stats.traffic_stats.PacketsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "ErrorsSent=%d\n",
    radio_info.hw_stats.error_stats.ErrorsSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "ErrorsReceived=%d\n",
    radio_info.hw_stats.error_stats.ErrorsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "DiscardPacketsSent=%d\n",
    radio_info.hw_stats.error_stats.DiscardPacketsSent);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len,
    "DiscardPacketsReceived=%d\n",
    radio_info.hw_stats.error_stats.DiscardPacketsReceived);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "PLCPErrorCount=UNKNOWN\n");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "FCSErrorCount=%d\n",
    radio_info.hw_stats.FCSErrorCount);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len,
    "InvalidMACCount=UNKNOWN\n");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len,
    "PacketsOtherReceived=UNKNOWN\n");
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "Noise=%d\n",
    radio_info.hw_stats.Noise);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "BSS load=%d\n",
      radio_info.load);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "TxPower=%.2f\n",
      radio_info.tx_pwr_cfg / 100.);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "RxAntennas=%d\n",
    radio_info.num_rx_antennas);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  ret = os_snprintf(buf + len, buflen - len, "TxAntennas=%d\n",
    radio_info.num_tx_antennas);
  if (ret >= buflen - len || ret < 0)
    return len;
  len += ret;
  if (!radio_info.primary_center_freq) {
    ret = os_snprintf(buf + len, buflen - len, "Freq=UNKNOWN\n");
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;

    ret = os_snprintf(buf + len, buflen - len, "OperatingChannelBandwidt=UNKNOWN\n");
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;

    ret = os_snprintf(buf + len, buflen - len, "Cf1=UNKNOWN\n");
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;

    ret = os_snprintf(buf + len, buflen - len, "Dfs_chan=UNKNOWN\n");
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;
  } else {
    ret = os_snprintf(buf + len, buflen - len, "Freq=%d\n",
      radio_info.primary_center_freq);
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;

    ret = os_snprintf(buf + len, buflen - len, "OperatingChannelBandwidt=%d\n",
      radio_info.width);
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;

    ret = os_snprintf(buf + len, buflen - len, "Cf1=%d\n",
      radio_info.center_freq1);
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;

    ret = os_snprintf(buf + len, buflen - len, "Dfs_chan=%d\n",
      ieee80211_is_dfs(radio_info.primary_center_freq));
    if (ret >= buflen - len || ret < 0)
      return len;
    len += ret;
  }

  return len;
}

int hostapd_ctrl_iface_update_atf_cfg(struct hostapd_data *hapd)
{
	int ret_code = hostapd_drv_send_atf_quotas(hapd, NULL /*reread cfg file*/, 0, 0);
	return ret_code;
}

#ifdef CONFIG_MBO
static int hostapd_ctrl_iface_cellular_pref_set(struct hostapd_data *hapd, char *cmd)
{
	char *token, *context = NULL;
	int val;

        hapd = get_bss_index(cmd, hapd->iface);
        if (!hapd) {
                wpa_printf(MSG_INFO,
                                "CTRL: CELLULAR_PREF_SET - there is no iface with the given name");
                return -1;
        }

	if (!hapd->conf->mbo_enabled) {
		wpa_printf(MSG_INFO,
				"CTRL: CELLULAR_PREF_SET - mbo is not enabled");
		return -1;
	}

	if (!hapd->conf->mbo_cell_aware) {
		wpa_printf(MSG_INFO,
				"CTRL: CELLULAR_PREF_SET - AP is not cellular aware");
		return -1;
	}

	cmd = os_strchr(cmd, ' ');
	if (cmd)
		cmd++;
	else {
		wpa_printf(MSG_INFO,
				"CTRL: CELLULAR_PREF_SET - Cellular Preference is missing");
		return -1;
	}

	token = str_token(cmd, " ", &context);
	if (!token) {
		wpa_printf(MSG_INFO,
				"CTRL: CELLULAR_PREF_SET - Cellular Preference is missing");
		return -1;
	}

	val = atoi(token);
	if (val != MBO_CELL_PREF_EXCLUDED && val != MBO_CELL_PREF_NO_USE && val != MBO_CELL_PREF_USE) {
		wpa_printf(MSG_INFO,
				"CTRL: CELLULAR_PREF_SET - Cellular Preference value is out of range (can be 0, 1 or 255)");
		return -1;
	}

	hapd->mbo_cell_pref = val;

	return 0;
}
#endif /* CONFIG_MBO */

static int hostapd_ctrl_iface_set_neighbor_per_vap(struct hostapd_data *hapd, char *buf)
{
	struct wpa_ssid_value ssid;
	u8 bssid[ETH_ALEN];
	struct wpabuf *nr, *lci = NULL, *civic = NULL;
	char *tmp;
	int ret;

	hapd = get_bss_index(buf, hapd->iface);
	if (!hapd) {
		wpa_printf(MSG_INFO,
			"CTRL: SET_NEIGHBOR_PER_VAP - there is no iface with the given name");
		return -1;
	}

	if (!(hapd->conf->radio_measurements[0] &
	      WLAN_RRM_CAPS_NEIGHBOR_REPORT)) {
		wpa_printf(MSG_ERROR,
			   "CTRL: SET_NEIGHBOR_PER_VAP: Neighbor report is not enabled");
		return -1;
	}

	buf = os_strchr(buf, ' ');
	if (buf)
		buf++;
	else {
		wpa_printf(MSG_INFO,
				"CTRL: SET_NEIGHBOR_PER_VAP - neighbor BSSID is missing");
		return -1;
	}

	if (hwaddr_aton(buf, bssid)) {
		wpa_printf(MSG_ERROR, "CTRL: SET_NEIGHBOR_PER_VAP: Bad BSSID");
		return -1;
	}

	tmp = os_strstr(buf, "ssid=");
	if (!tmp || ssid_parse(tmp + 5, &ssid)) {
		wpa_printf(MSG_ERROR,
			   "CTRL: SET_NEIGHBOR_PER_VAP: Bad or missing SSID");
		return -1;
	}
	buf = os_strchr(tmp + 6, tmp[5] == '"' ? '"' : ' ');
	if (!buf)
		return -1;

	tmp = os_strstr(buf, "nr=");
	if (!tmp) {
		wpa_printf(MSG_ERROR,
			   "CTRL: SET_NEIGHBOR_PER_VAP: Missing Neighbor Report element");
		return -1;
	}

	buf = os_strchr(tmp, ' ');
	if (buf)
		*buf++ = '\0';

	nr = wpabuf_parse_bin(tmp + 3);
	if (!nr) {
		wpa_printf(MSG_ERROR,
			   "CTRL: SET_NEIGHBOR_PER_VAP: Bad Neighbor Report element");
		return -1;
	}

	if (!buf)
		goto set;

	tmp = os_strstr(buf, "lci=");
	if (tmp) {
		buf = os_strchr(tmp, ' ');
		if (buf)
			*buf++ = '\0';
		lci = wpabuf_parse_bin(tmp + 4);
		if (!lci) {
			wpa_printf(MSG_ERROR,
				   "CTRL: SET_NEIGHBOR_PER_VAP: Bad LCI subelement");
			wpabuf_free(nr);
			return -1;
		}
	}

	if (!buf)
		goto set;

	tmp = os_strstr(buf, "civic=");
	if (tmp) {
		buf = os_strchr(tmp, ' ');
		if (buf)
			*buf++ = '\0';
		civic = wpabuf_parse_bin(tmp + 6);
		if (!civic) {
			wpa_printf(MSG_ERROR,
				   "CTRL: SET_NEIGHBOR_PER_VAP: Bad civic subelement");
			wpabuf_free(nr);
			wpabuf_free(lci);
			return -1;
		}
	}

set:
	ret = hostapd_neighbor_set(hapd, bssid, &ssid, nr, lci, civic);

	wpabuf_free(nr);
	wpabuf_free(lci);
	wpabuf_free(civic);

	return ret;
}

static int hostapd_ctrl_iface_remove_neighbor_per_vap(struct hostapd_data *hapd,
					      char *buf)
{
	struct wpa_ssid_value ssid;
	u8 bssid[ETH_ALEN];
	char *tmp;

	hapd = get_bss_index(buf, hapd->iface);
	if (!hapd) {
		wpa_printf(MSG_INFO,
				"CTRL: REMOVE_NEIGHBOR_PER_VAP - there is no iface with the given name");
		return -1;
	}

	buf = os_strchr(buf, ' ');
	if (buf)
		buf++;
	else {
		wpa_printf(MSG_INFO,
				"CTRL: REMOVE_NEIGHBOR_PER_VAP - neighbor BSSID is missing");
		return -1;
	}

	if (hwaddr_aton(buf, bssid)) {
		wpa_printf(MSG_ERROR, "CTRL: REMOVE_NEIGHBOR_PER_VAP: Bad BSSID");
		return -1;
	}

	tmp = os_strstr(buf, "ssid=");
	if (!tmp || ssid_parse(tmp + 5, &ssid)) {
		wpa_printf(MSG_ERROR,
			   "CTRL: REMOVE_NEIGHBOR_PER_VAP: Bad or missing SSID");
		return -1;
	}

	return hostapd_neighbor_remove(hapd, bssid, &ssid);
}

int hostapd_ctrl_iface_unconnected_sta(struct hostapd_data *hapd, const char *cmd)
{
  struct hostapd_unconnected_sta_req_params req;
  char *pos;
  struct sta_info *sta;

  os_memset(&req, 0, sizeof(req));

  if (hwaddr_aton(cmd, req.addr))
    return -1;

  pos = os_strstr(cmd, " ");
  if (pos) {
    pos++;
    req.freq = atoi(pos);
    if (req.freq == 0){
      wpa_printf(MSG_ERROR, "unconnected_sta: invalid freq provided");
      return -1;
    }
  } else {
    wpa_printf(MSG_ERROR, "unconnected_sta: freq not specified");
    return -1;
  }

  pos = os_strstr(cmd, "center_freq1=");
  if (pos) {
    pos += strlen("center_freq1=");
    req.center_freq1 = atoi(pos);
    if (req.center_freq1 == 0){
      wpa_printf(MSG_ERROR, "unconnected_sta: invalid center_freq1 provided");
      return -1;
    }
  } else {
    wpa_printf(MSG_ERROR, "unconnected_sta: center_freq1 not specified");
    return -1;
  }

  pos = os_strstr(cmd, "center_freq2=");
  if (pos) {
    pos += strlen("center_freq2=");
    req.center_freq2 = atoi(pos);
    if (req.center_freq2 == 0){
      wpa_printf(MSG_ERROR, "unconnected_sta: invalid center_freq2 provided");
      return -1;
    }
  }

  pos = os_strstr(cmd, "bandwidth=");
  if (pos) {
    pos += strlen("bandwidth=");
    req.bandwidth = atoi(pos);
    if (req.bandwidth == 0){
      wpa_printf(MSG_ERROR, "unconnected_sta: invalid bandwidth provided");
      return -1;
    }
  } else {
    wpa_printf(MSG_ERROR, "unconnected_sta: bandwidth not specified");
    return -1;
  }
  switch (req.bandwidth) {
  case 20:
    req.bandwidth = NL80211_CHAN_WIDTH_20;
    break;
  case 40:
    req.bandwidth = NL80211_CHAN_WIDTH_40;
    break;
  case 80:
    if (req.center_freq2)
      req.bandwidth = NL80211_CHAN_WIDTH_80P80;
    else
      req.bandwidth = NL80211_CHAN_WIDTH_80;
    break;
  case 160:
    req.bandwidth = NL80211_CHAN_WIDTH_160;
    break;
  default:
    wpa_printf(MSG_ERROR, "unconnected_sta: invalid bandwidth provided");
    return -1;
  }

  sta = ap_get_sta(hapd, req.addr);
  if (sta) {
    wpa_printf(MSG_ERROR, "unconnected_sta: disconnect STA "MACSTR" first",
      MAC2STR(req.addr));
    return -1;
  }

  return hostapd_drv_unconnected_sta(hapd, &req);
}


int hostapd_ctrl_iface_set_failsafe_chan(struct hostapd_iface *iface, const char *cmd)
{
  char *pos, *pos2;
  struct hostapd_channel_data *channel;
  struct hostapd_failsafe_freq_params old_freq_params;
  int tx_ant_mask = -1, rx_ant_mask = -1;

  memcpy(&old_freq_params, &iface->failsafe, sizeof(iface->failsafe));
  iface->failsafe.freq = atoi(cmd);

  /* ACS requested, the rest of parameters are irrelevant */
  if (iface->failsafe.freq == 0)
    return 0;

  channel = hostapd_hw_get_channel_data(iface, iface->failsafe.freq);
  if (!channel) {
    wpa_printf(MSG_ERROR, "set_failsafe_chan: invalid frequency %d provided, "
      "channel not found", iface->failsafe.freq);
    goto err;
  }
  if (channel->flag & HOSTAPD_CHAN_RADAR) {
    wpa_printf(MSG_ERROR, "set_failsafe_chan: channel %d is DFS channel. It "
      "cannot be failsafe", channel->chan);
    goto err;
  }

  pos = os_strstr(cmd, "center_freq1=");
  if (pos) {
    pos += strlen("center_freq1=");
    iface->failsafe.center_freq1 = atoi(pos);
    if (iface->failsafe.center_freq1 == 0) {
      wpa_printf(MSG_ERROR, "set_failsafe_chan: invalid center_freq1 provided");
      goto err;
    }
  } else {
    wpa_printf(MSG_ERROR, "set_failsafe_chan: center_freq1 not specified");
    goto err;
  }

  pos = os_strstr(cmd, "center_freq2=");
  if (pos) {
    pos += strlen("center_freq2=");
    iface->failsafe.center_freq2 = atoi(pos);
    if (iface->failsafe.center_freq2 == 0) {
      wpa_printf(MSG_ERROR, "set_failsafe_chan: invalid center_freq2 provided");
      goto err;
    }
  }

  pos = os_strstr(cmd, "bandwidth=");
  if (pos) {
    pos += strlen("bandwidth=");
    iface->failsafe.bandwidth = atoi(pos);
    if (iface->failsafe.bandwidth == 0) {
      wpa_printf(MSG_ERROR, "set_failsafe_chan: invalid bandwidth provided");
      goto err;
    }
    if (iface->failsafe.bandwidth != 20 &&
      iface->failsafe.bandwidth != 40 &&
      iface->failsafe.bandwidth != 80 &&
      iface->failsafe.bandwidth != 160) {
      wpa_printf(MSG_ERROR, "set_failsafe_chan: invalid bandwidth %d provided",
        iface->failsafe.bandwidth);
      goto err;
    }
  } else {
    wpa_printf(MSG_ERROR, "set_failsafe_chan: bandwidth not specified");
    goto err;
  }

  pos = os_strstr(cmd, "tx_ant_mask=");
  if (pos) {
    pos += strlen("tx_ant_mask=");
    tx_ant_mask = strtol(pos, &pos2, 10);
    if (pos == pos2 || tx_ant_mask < 0) {
      wpa_printf(MSG_ERROR, "set_failsafe_chan: invalid tx_ant_mask provided");
      return -1;
    }
  }
  pos = os_strstr(cmd, "rx_ant_mask=");
  if (pos) {
    pos += strlen("rx_ant_mask=");
    rx_ant_mask = strtol(pos, &pos2, 10);
    if (pos == pos2 || rx_ant_mask < 0) {
      wpa_printf(MSG_ERROR, "set_failsafe_chan: invalid rx_ant_mask provided");
      return -1;
    }
  }
  if ((tx_ant_mask >= 0 && rx_ant_mask < 0) || (rx_ant_mask >= 0 && tx_ant_mask < 0)) {
    wpa_printf(MSG_ERROR, "set_failsafe_chan: changing only TX or RX "
      "antenna mask is not possible");
    goto err;
  }
  iface->failsafe.tx_ant = tx_ant_mask;
  iface->failsafe.rx_ant = rx_ant_mask;

  return 0;

err:
  memcpy(&iface->failsafe, &old_freq_params, sizeof(iface->failsafe));
  return -1;
}


int hostapd_ctrl_iface_get_failsafe_chan(struct hostapd_iface *iface,
  const char *cmd, char *buf, size_t buflen)
{
  int ret = 0, len = 0;

  if (iface->failsafe.freq == 0)
    return os_snprintf(buf + len, buflen - len, "UNSPECIFIED\n");

  ret = os_snprintf(buf + len, buflen - len, "%d center_freq1=%d "
    "center_freq2=%d bandwidth=%d",
    iface->failsafe.freq,
    iface->failsafe.center_freq1,
    iface->failsafe.center_freq2,
    iface->failsafe.bandwidth);

  if (ret >= buflen - len || ret < 0)
    return ret;
  len += ret;

  if (iface->failsafe.tx_ant > 0) {
    ret = os_snprintf(buf + len, buflen - len, " tx_ant=%d rx_ant=%d",
      iface->failsafe.tx_ant,
      iface->failsafe.rx_ant);
    if (ret >= buflen - len || ret < 0)
      return ret;
    len += ret;
  }

  ret = os_snprintf(buf + len, buflen - len, "\n");
  if (ret >= buflen - len || ret < 0)
    return ret;
  len += ret;

  return len;
}


int hostapd_ctrl_iface_acs_report(struct hostapd_iface *iface,
  const char *cmd, char *buf, size_t buflen)
{
  int ret = 0, len = 0, i;
  struct hostapd_hw_modes *mode;

  if (!iface->current_mode)
    return -1;
  mode = iface->current_mode;

  for (i = 0; i < mode->num_candidates; i++) {
    ret = os_snprintf(buf + len, buflen - len, "Ch=%d BW=%d",
      mode->candidates[i].primary,
      mode->candidates[i].width);
    if (ret >= buflen - len || ret < 0)
      break;
    len += ret;

    ret = os_snprintf(buf + len, buflen - len, " DFS=%d",
      mode->candidates[i].radar_affected);
    if (ret >= buflen - len || ret < 0)
      break;
    len += ret;

    if (mode->candidates[i].filled_mask & CHDATA_TX_POWER) {
      ret = os_snprintf(buf + len, buflen - len, " pow=%d",
        mode->candidates[i].tx_power);
      if (ret >= buflen - len || ret < 0)
        break;
      len += ret;
    }

    if (mode->candidates[i].filled_mask & CHDATA_NOISE_FLOOR) {
      ret = os_snprintf(buf + len, buflen - len, " NF=%d",
        mode->candidates[i].noise_floor);
      if (ret >= buflen - len || ret < 0)
        break;
      len += ret;
    }

    ret = os_snprintf(buf + len, buflen - len, " bss=%d",
      mode->candidates[i].num_bss);
    if (ret >= buflen - len || ret < 0)
      break;
    len += ret;
    ret = os_snprintf(buf + len, buflen - len, " pri=%d",
      mode->candidates[i].num_bss_pri);
    if (ret >= buflen - len || ret < 0)
      break;
    len += ret;

    if (mode->candidates[i].filled_mask & CHDATA_LOAD) {
      ret = os_snprintf(buf + len, buflen - len, " load=%d",
        mode->candidates[i].channel_load);
      if (ret >= buflen - len || ret < 0)
        break;
      len += ret;
    }
    ret = os_snprintf(buf + len, buflen - len, "\n");
    if (ret >= buflen - len || ret < 0)
      break;
    len += ret;
  }

  return len;
}

/* checks weather channel exists */
static int hostapd_ctrl_iface_restricted_chan_number_check_cb(struct hostapd_iface *iface,
  int channel)
{
  if (!hostapd_hw_get_channel_data_from_ch(iface, channel))
    return -1;
  return 0;
}

/* Sets channel as restricted. Ignores channels that are already disabled. */
static int hostapd_ctrl_iface_restricted_chan_number_cb(struct hostapd_iface *iface,
  int channel)
{
  struct hostapd_channel_data* ch_data;

  ch_data = hostapd_hw_get_channel_data_from_ch(iface, channel);
  if (!ch_data)
    return -1;
  if (!(ch_data->flag & HOSTAPD_CHAN_DISABLED)) {
    ch_data->flag |= HOSTAPD_CHAN_DISABLED;
    ch_data->flag |= HOSTAPD_CHAN_RESTRICTED;
  }

  return 0;
}

/* checks weather any channel in given range exists */
static int hostapd_ctrl_iface_restricted_chan_range_check_cb(struct hostapd_iface *iface,
  int channel_from, int channel_to)
{
  Boolean exists = FALSE;
  int i;

  for (i = channel_from; i <= channel_to; i++) {
    if (hostapd_hw_get_channel_data_from_ch(iface, i)) {
      exists = TRUE;
      break;
    }
  }
  if (exists)
    return 0;
  else
    return -1;
}

/* Sets channels in given range as restricted. Ignores channels that are already
 * disabled. */
static int hostapd_ctrl_iface_restricted_chan_range_cb(struct hostapd_iface *iface,
  int channel_from, int channel_to)
{
  Boolean exists = FALSE;
  int i;
  struct hostapd_channel_data* ch_data;

  for (i = channel_from; i <= channel_to; i++) {
    ch_data = hostapd_hw_get_channel_data_from_ch(iface, i);
    if (!ch_data)
      continue;
    exists = TRUE;
    if (!(ch_data->flag & HOSTAPD_CHAN_DISABLED)) {
      ch_data->flag |= HOSTAPD_CHAN_DISABLED;
      ch_data->flag |= HOSTAPD_CHAN_RESTRICTED;
    }
  }
  if (exists)
    return 0;
  else
    return -1;
}


int hostapd_ctrl_iface_set_restricted_chan(struct hostapd_iface *iface, const char *cmd)
{
  char *end;
  struct hostapd_hw_modes *mode;
  int i;
  struct hostapd_channel_data *ch;

  if (!iface->current_mode)
    return -1;
  mode = iface->current_mode;

  /* check list for validity */
  if (hostapd_get_list(cmd, &end, iface,
    hostapd_ctrl_iface_restricted_chan_number_check_cb,
    hostapd_ctrl_iface_restricted_chan_range_check_cb))
    return -1;

  /* clear restricted flag on all channels */
  for (i = 0; i < mode->num_channels; i++) {
    ch = &iface->current_mode->channels[i];
    if ((ch->flag & HOSTAPD_CHAN_DISABLED) &&
      (ch->flag & HOSTAPD_CHAN_RESTRICTED)) {
      ch->flag &= ~HOSTAPD_CHAN_DISABLED;
      ch->flag &= ~HOSTAPD_CHAN_RESTRICTED;
    }
  }

  /* set restricted flag on specified channels */
  return hostapd_get_list(cmd, &end, iface,
    hostapd_ctrl_iface_restricted_chan_number_cb,
    hostapd_ctrl_iface_restricted_chan_range_cb);
}


int hostapd_ctrl_iface_get_restricted_chan(struct hostapd_iface *iface,
  const char *cmd, char *buf, size_t buflen)
{
  int ret = 0, len = 0, i;
  struct hostapd_hw_modes *mode;
  struct hostapd_channel_data *ch;

  if (!iface->current_mode)
    return -1;
  mode = iface->current_mode;

  for (i = 0; i < mode->num_channels; i++) {
    ch = &iface->current_mode->channels[i];
    if ((ch->flag & HOSTAPD_CHAN_DISABLED) &&
      (ch->flag & HOSTAPD_CHAN_RESTRICTED)) {
        ret = os_snprintf(buf + len, buflen - len, "%d ", ch->chan);
        if (ret >= buflen - len || ret < 0)
          break;
        len += ret;
    }
  }
  if (len > 0)
    buf[len - 1] = '\n';

  return len;
}


int hostapd_ctrl_iface_get_hw_features(struct hostapd_iface *iface,
  const char *cmd, char *buf, size_t buflen)
{
  int ret = 0, len = 0;
  struct hostapd_data *hapd = iface->bss[0];
  int i, j;

  for (i = 0; i < iface->num_hw_features; i++) {
    struct hostapd_hw_modes *feature = &iface->hw_features[i];
    int dfs_enabled = hapd->iconf->ieee80211h &&
      (iface->drv_flags & WPA_DRIVER_FLAGS_RADAR);

    for (j = 0; j < feature->num_channels; j++) {
      int dfs = 0;

      if ((feature->channels[j].flag &
           HOSTAPD_CHAN_RADAR) && dfs_enabled) {
        dfs = 1;
      }

      if (feature->channels[j].flag & HOSTAPD_CHAN_DISABLED)
        continue;

      ret = os_snprintf(buf + len, buflen - len, "Allowed channel: mode=%d "
           "chan=%d freq=%d MHz max_tx_power=%d dBm%s\n",
           feature->mode,
           feature->channels[j].chan,
           feature->channels[j].freq,
           feature->channels[j].max_tx_power,
           dfs ? dfs_info(&feature->channels[j]) : "");
      if (ret >= buflen - len || ret < 0)
        return len;
      len += ret;
    }
  }

  return len;
}


static int hostapd_ctrl_iface_receive_process(struct hostapd_data *hapd,
					      char *buf, char *reply,
					      int reply_size,
					      struct sockaddr_storage *from,
					      socklen_t fromlen)
{
	int reply_len, res;

	os_memcpy(reply, "OK\n", 3);
	reply_len = 3;

	if (os_strcmp(buf, "PING") == 0) {
		os_memcpy(reply, "PONG\n", 5);
		reply_len = 5;
	} else if (os_strncmp(buf, "RELOG", 5) == 0) {
		if (wpa_debug_reopen_file() < 0)
			reply_len = -1;
	} else if (os_strcmp(buf, "STATUS") == 0) {
		reply_len = hostapd_ctrl_iface_status(hapd, reply,
						      reply_size);
	} else if (os_strcmp(buf, "STATUS-DRIVER") == 0) {
		reply_len = hostapd_drv_status(hapd, reply, reply_size);
	} else if (os_strcmp(buf, "MIB") == 0) {
		reply_len = ieee802_11_get_mib(hapd, reply, reply_size);
		if (reply_len >= 0) {
			res = wpa_get_mib(hapd->wpa_auth, reply + reply_len,
					  reply_size - reply_len);
			if (res < 0)
				reply_len = -1;
			else
				reply_len += res;
		}
		if (reply_len >= 0) {
			res = ieee802_1x_get_mib(hapd, reply + reply_len,
						 reply_size - reply_len);
			if (res < 0)
				reply_len = -1;
			else
				reply_len += res;
		}
#ifndef CONFIG_NO_RADIUS
		if (reply_len >= 0) {
			res = radius_client_get_mib(hapd->radius,
						    reply + reply_len,
						    reply_size - reply_len);
			if (res < 0)
				reply_len = -1;
			else
				reply_len += res;
		}
#endif /* CONFIG_NO_RADIUS */
	} else if (os_strncmp(buf, "MIB ", 4) == 0) {
		reply_len = hostapd_ctrl_iface_mib(hapd, reply, reply_size,
						   buf + 4);
	} else if (os_strcmp(buf, "STA-FIRST") == 0) {
		reply_len = hostapd_ctrl_iface_sta_first(hapd, reply,
							 reply_size);
	} else if (os_strncmp(buf, "STA ", 4) == 0) {
		reply_len = hostapd_ctrl_iface_sta(hapd, buf + 4, reply,
						   reply_size);
	} else if (os_strncmp(buf, "STA-NEXT ", 9) == 0) {
		reply_len = hostapd_ctrl_iface_sta_next(hapd, buf + 9, reply,
							reply_size);
	} else if (os_strcmp(buf, "ATTACH") == 0) {
		if (hostapd_ctrl_iface_attach(hapd, from, fromlen))
			reply_len = -1;
	} else if (os_strcmp(buf, "DETACH") == 0) {
		if (hostapd_ctrl_iface_detach(hapd, from, fromlen))
			reply_len = -1;
	} else if (os_strncmp(buf, "LEVEL ", 6) == 0) {
		if (hostapd_ctrl_iface_level(hapd, from, fromlen,
						    buf + 6))
			reply_len = -1;
	} else if (os_strncmp(buf, "NEW_STA ", 8) == 0) {
		if (hostapd_ctrl_iface_new_sta(hapd, buf + 8))
			reply_len = -1;
	} else if (os_strncmp(buf, "DEAUTHENTICATE ", 15) == 0) {
		if (hostapd_ctrl_iface_deauthenticate(hapd, buf + 15))
			reply_len = -1;
	} else if (os_strncmp(buf, "DISASSOCIATE ", 13) == 0) {
		if (hostapd_ctrl_iface_disassociate(hapd, buf + 13))
			reply_len = -1;
#ifdef CONFIG_TAXONOMY
	} else if (os_strncmp(buf, "SIGNATURE ", 10) == 0) {
		reply_len = hostapd_ctrl_iface_signature(hapd, buf + 10,
							 reply, reply_size);
#endif /* CONFIG_TAXONOMY */
	} else if (os_strncmp(buf, "POLL_STA ", 9) == 0) {
		if (hostapd_ctrl_iface_poll_sta(hapd, buf + 9))
			reply_len = -1;
	} else if (os_strcmp(buf, "STOP_AP") == 0) {
		if (hostapd_ctrl_iface_stop_ap(hapd))
			reply_len = -1;
#ifdef CONFIG_IEEE80211W
#ifdef NEED_AP_MLME
	} else if (os_strncmp(buf, "SA_QUERY ", 9) == 0) {
		if (hostapd_ctrl_iface_sa_query(hapd, buf + 9))
			reply_len = -1;
#endif /* NEED_AP_MLME */
#endif /* CONFIG_IEEE80211W */
#ifdef CONFIG_WPS
	} else if (os_strncmp(buf, "WPS_PIN ", 8) == 0) {
		if (hostapd_ctrl_iface_wps_pin(hapd, buf + 8))
			reply_len = -1;
	} else if (os_strncmp(buf, "WPS_CHECK_PIN ", 14) == 0) {
		reply_len = hostapd_ctrl_iface_wps_check_pin(
			hapd, buf + 14, reply, reply_size);
	} else if (os_strncmp(buf, "WPS_PBC ", 8) == 0) {
		if (hostapd_wps_vap_button_pushed(hapd, buf + 8))
			reply_len = -1;
	} else if (os_strncmp(buf, "WPS_CANCEL ", 11) == 0) {
		if (hostapd_wps_cancel(hapd, buf + 11))
			reply_len = -1;
	} else if (os_strncmp(buf, "WPS_AP_PIN ", 11) == 0) {
		reply_len = hostapd_ctrl_iface_wps_ap_pin(hapd, buf + 11,
							  reply, reply_size);
	} else if (os_strncmp(buf, "WPS_CONFIG ", 11) == 0) {
		if (hostapd_ctrl_iface_wps_config(hapd, buf + 11) < 0)
			reply_len = -1;
	} else if (os_strncmp(buf, "WPS_GET_STATUS ", 15) == 0) {
		reply_len = hostapd_ctrl_iface_wps_get_status(hapd, buf + 15, reply,
							      reply_size);
#ifdef CONFIG_WPS_NFC
	} else if (os_strncmp(buf, "WPS_NFC_TAG_READ ", 17) == 0) {
		if (hostapd_ctrl_iface_wps_nfc_tag_read(hapd, buf + 17))
			reply_len = -1;
	} else if (os_strncmp(buf, "WPS_NFC_CONFIG_TOKEN ", 21) == 0) {
		reply_len = hostapd_ctrl_iface_wps_nfc_config_token(
			hapd, buf + 21, reply, reply_size);
	} else if (os_strncmp(buf, "WPS_NFC_TOKEN ", 14) == 0) {
		reply_len = hostapd_ctrl_iface_wps_nfc_token(
			hapd, buf + 14, reply, reply_size);
	} else if (os_strncmp(buf, "NFC_GET_HANDOVER_SEL ", 21) == 0) {
		reply_len = hostapd_ctrl_iface_nfc_get_handover_sel(
			hapd, buf + 21, reply, reply_size);
	} else if (os_strncmp(buf, "NFC_REPORT_HANDOVER ", 20) == 0) {
		if (hostapd_ctrl_iface_nfc_report_handover(hapd, buf + 20))
			reply_len = -1;
#endif /* CONFIG_WPS_NFC */
#endif /* CONFIG_WPS */
	} else if (os_strncmp(buf, "UPDATE_WAN_METRICS ", 19) == 0) {
		if (hostapd_ctrl_iface_update_wan_metrics(hapd, buf + 19))
			reply_len = -1;
#ifdef CONFIG_INTERWORKING
	} else if (os_strncmp(buf, "SET_QOS_MAP_SET ", 16) == 0) {
		if (hostapd_ctrl_iface_set_qos_map_set(hapd, buf + 16))
			reply_len = -1;
	} else if (os_strncmp(buf, "SEND_QOS_MAP_CONF ", 18) == 0) {
		if (hostapd_ctrl_iface_send_qos_map_conf(hapd, buf + 18))
			reply_len = -1;
#endif /* CONFIG_INTERWORKING */
#ifdef CONFIG_HS20
	} else if (os_strncmp(buf, "HS20_WNM_NOTIF ", 15) == 0) {
		if (hostapd_ctrl_iface_hs20_wnm_notif(hapd, buf + 15))
			reply_len = -1;
	} else if (os_strncmp(buf, "HS20_DEAUTH_REQ ", 16) == 0) {
		if (hostapd_ctrl_iface_hs20_deauth_req(hapd, buf + 16))
			reply_len = -1;
#endif /* CONFIG_HS20 */
#ifdef CONFIG_WNM
	} else if (os_strncmp(buf, "DISASSOC_IMMINENT ", 18) == 0) {
		if (hostapd_ctrl_iface_disassoc_imminent(hapd, buf + 18))
			reply_len = -1;
	} else if (os_strncmp(buf, "ESS_DISASSOC ", 13) == 0) {
		if (hostapd_ctrl_iface_ess_disassoc(hapd, buf + 13))
			reply_len = -1;
	} else if (os_strncmp(buf, "BSS_TM_REQ ", 11) == 0) {
		reply_len = hostapd_ctrl_iface_bss_tm_req(hapd, buf + 11,
				reply, reply_size);
#endif /* CONFIG_WNM */
	} else if (os_strncmp(buf, "GET_CONFIG ", 11) == 0) {
		reply_len = hostapd_ctrl_iface_get_config(hapd, buf + 11, reply,
							  reply_size);
	} else if (os_strncmp(buf, "SET ", 4) == 0) {
		if (hostapd_ctrl_iface_set(hapd, buf + 4))
			reply_len = -1;
	} else if (os_strncmp(buf, "GET ", 4) == 0) {
		reply_len = hostapd_ctrl_iface_get(hapd, buf + 4, reply,
						   reply_size);
	} else if (os_strncmp(buf, "ENABLE", 6) == 0) {
		if (hostapd_ctrl_iface_enable(hapd->iface))
			reply_len = -1;
	} else if (os_strncmp(buf, "RELOAD", 6) == 0) {
		if (hostapd_ctrl_iface_reload(hapd->iface))
			reply_len = -1;
	} else if (os_strncmp(buf, "RECONF", 6) == 0) {
		if (hostapd_ctrl_iface_reconf(hapd->iface, buf + 6))
			reply_len = -1;
	} else if (os_strncmp(buf, "DISABLE", 7) == 0) {
		if (hostapd_ctrl_iface_disable(hapd->iface))
			reply_len = -1;
	} else if (os_strcmp(buf, "UPDATE_BEACON") == 0) {
		if (ieee802_11_set_beacon(hapd))
			reply_len = -1;
#ifdef CONFIG_TESTING_OPTIONS
	} else if (os_strncmp(buf, "RADAR ", 6) == 0) {
		if (hostapd_ctrl_iface_radar(hapd, buf + 6))
			reply_len = -1;
	} else if (os_strncmp(buf, "MGMT_TX ", 8) == 0) {
		if (hostapd_ctrl_iface_mgmt_tx(hapd, buf + 8))
			reply_len = -1;
	} else if (os_strncmp(buf, "EAPOL_RX ", 9) == 0) {
		if (hostapd_ctrl_iface_eapol_rx(hapd, buf + 9) < 0)
			reply_len = -1;
	} else if (os_strncmp(buf, "DATA_TEST_CONFIG ", 17) == 0) {
		if (hostapd_ctrl_iface_data_test_config(hapd, buf + 17) < 0)
			reply_len = -1;
	} else if (os_strncmp(buf, "DATA_TEST_TX ", 13) == 0) {
		if (hostapd_ctrl_iface_data_test_tx(hapd, buf + 13) < 0)
			reply_len = -1;
	} else if (os_strncmp(buf, "DATA_TEST_FRAME ", 16) == 0) {
		if (hostapd_ctrl_iface_data_test_frame(hapd, buf + 16) < 0)
			reply_len = -1;
	} else if (os_strncmp(buf, "TEST_ALLOC_FAIL ", 16) == 0) {
		if (hostapd_ctrl_test_alloc_fail(hapd, buf + 16) < 0)
			reply_len = -1;
	} else if (os_strcmp(buf, "GET_ALLOC_FAIL") == 0) {
		reply_len = hostapd_ctrl_get_alloc_fail(hapd, reply,
							reply_size);
	} else if (os_strncmp(buf, "TEST_FAIL ", 10) == 0) {
		if (hostapd_ctrl_test_fail(hapd, buf + 10) < 0)
			reply_len = -1;
	} else if (os_strcmp(buf, "GET_FAIL") == 0) {
		reply_len = hostapd_ctrl_get_fail(hapd, reply, reply_size);
#endif /* CONFIG_TESTING_OPTIONS */
	} else if (os_strncmp(buf, "CHAN_SWITCH ", 12) == 0) {
		if (hostapd_ctrl_iface_chan_switch(hapd->iface, buf + 12))
			reply_len = -1;
	} else if (os_strncmp(buf, "VENDOR ", 7) == 0) {
		reply_len = hostapd_ctrl_iface_vendor(hapd, buf + 7, reply,
						      reply_size);
	} else if (os_strncmp(buf, "ACS_RECALC", 10) == 0) {
		if (hostapd_ctrl_iface_acs_recalc(hapd->iface))
			reply_len = -1;
	} else if (os_strncmp(buf, "DENY_MAC ", 9) == 0) {
		if (hostapd_ctrl_iface_deny_mac(hapd, buf + 9))
			reply_len = -1;
	} else if (os_strncmp(buf, "STA_STEER ", 10) == 0) {
		printf("%s; *** Received from FAPI: 'STA_STEER' (buf= '%s') ***\n", __FUNCTION__, buf);
		if (hostapd_ctrl_iface_sta_steer(hapd, buf + 10))
			reply_len = -1;
	} else if (os_strncmp(buf, "STA_ALLOW", 9) == 0) {
		printf("%s; *** Received from FAPI: 'STA_ALLOW' (buf= '%s') ***\n", __FUNCTION__, buf);
		if (hostapd_ctrl_iface_sta_allow(hapd, buf + 9))
			reply_len = -1;
    } else if (os_strncmp(buf, "SET_BSS_LOAD ", 13) == 0) {
        printf("%s; *** Received: 'SET_BSS_LOAD' (buf= '%s') ***\n", __FUNCTION__, buf);
        reply_len = hostapd_ctrl_iface_set_bss_load(hapd, buf + 13, reply,
                    reply_size);
	} else if (os_strncmp(buf, "GET_STA_MEASUREMENTS ", 21) == 0) {
		printf("%s; *** Received from FAPI: 'GET_STA_MEASUREMENTS' (buf= '%s') ***\n", __FUNCTION__, buf);
		reply_len = hostapd_ctrl_iface_get_sta_measurements(hapd, buf + 21, reply,
					reply_size);
	} else if (os_strncmp(buf, "GET_VAP_MEASUREMENTS ", 21) == 0) {
		printf("%s; *** Received from FAPI: 'GET_VAP_MEASUREMENTS' (buf= '%s') ***\n", __FUNCTION__, buf);
		reply_len = hostapd_ctrl_iface_get_vap_measurements(hapd, buf + 21, reply,
					reply_size);
	} else if (os_strncmp(buf, "GET_RADIO_INFO", 14) == 0) {
		printf("%s; *** Received from FAPI: 'GET_RADIO_INFO' (buf= '%s') ***\n", __FUNCTION__, buf);
		reply_len = hostapd_ctrl_iface_get_radio_info(hapd, NULL, reply,
					reply_size);
	} else if (os_strncmp(buf, "UPDATE_ATF_CFG", 14) == 0) {
		if (hostapd_ctrl_iface_update_atf_cfg(hapd))
			reply_len = -1;
	} else if (os_strncmp(buf, "UNCONNECTED_STA_RSSI ", 21) == 0) {
		if (hostapd_ctrl_iface_unconnected_sta(hapd, buf + 21))
			reply_len = -1;
	} else if (os_strncmp(buf, "SET_FAILSAFE_CHAN ", 18) == 0) {
		if (hostapd_ctrl_iface_set_failsafe_chan(hapd->iface, buf + 18))
			reply_len = -1;
	} else if (os_strncmp(buf, "GET_FAILSAFE_CHAN", 17) == 0) {
		reply_len = hostapd_ctrl_iface_get_failsafe_chan(hapd->iface, NULL, reply,
					reply_size);
	} else if (os_strncmp(buf, "GET_ACS_REPORT", 14) == 0) {
		reply_len = hostapd_ctrl_iface_acs_report(hapd->iface, NULL, reply,
					 reply_size);
	} else if (os_strncmp(buf, "RESTRICTED_CHANNELS", 19) == 0) {
		if (hostapd_ctrl_iface_set_restricted_chan(hapd->iface, buf + 19))
			reply_len = -1;
	} else if (os_strncmp(buf, "GET_RESTRICTED_CHANNELS", 23) == 0) {
		reply_len = hostapd_ctrl_iface_get_restricted_chan(hapd->iface, NULL, reply,
					reply_size);
#ifdef CONFIG_MBO
	} else if (os_strncmp(buf, "MBO_BSS_ASSOC_DISALLOW ", 23) == 0) {
		if (hostapd_ctrl_iface_mbo_bss_assoc_disallow(hapd, buf + 23))
			reply_len = -1;
	} else if (os_strncmp(buf, "CELLULAR_PREF_SET ", 18) == 0) {
		if (hostapd_ctrl_iface_cellular_pref_set(hapd, buf + 18))
			reply_len = -1;
#endif /* CONFIG_MBO */
	} else if (os_strcmp(buf, "ERP_FLUSH") == 0) {
		ieee802_1x_erp_flush(hapd);
#ifdef RADIUS_SERVER
		radius_server_erp_flush(hapd->radius_srv);
#endif /* RADIUS_SERVER */
	} else if (os_strncmp(buf, "EAPOL_REAUTH ", 13) == 0) {
		if (hostapd_ctrl_iface_eapol_reauth(hapd, buf + 13))
			reply_len = -1;
	} else if (os_strncmp(buf, "EAPOL_SET ", 10) == 0) {
		if (hostapd_ctrl_iface_eapol_set(hapd, buf + 10))
			reply_len = -1;
	} else if (os_strncmp(buf, "LOG_LEVEL", 9) == 0) {
		reply_len = hostapd_ctrl_iface_log_level(
			hapd, buf + 9, reply, reply_size);
#ifdef NEED_AP_MLME
	} else if (os_strcmp(buf, "TRACK_STA_LIST") == 0) {
		reply_len = hostapd_ctrl_iface_track_sta_list(
			hapd, reply, reply_size);
#endif /* NEED_AP_MLME */
	} else if (os_strcmp(buf, "PMKSA") == 0) {
		reply_len = hostapd_ctrl_iface_pmksa_list(hapd, reply,
							  reply_size);
	} else if (os_strcmp(buf, "PMKSA_FLUSH") == 0) {
		hostapd_ctrl_iface_pmksa_flush(hapd);
	} else if (os_strncmp(buf, "SET_NEIGHBOR ", 13) == 0) {
		if (hostapd_ctrl_iface_set_neighbor(hapd, buf + 13))
			reply_len = -1;
	} else if (os_strncmp(buf, "SET_NEIGHBOR_PER_VAP ", 21) == 0) {
		if (hostapd_ctrl_iface_set_neighbor_per_vap(hapd, buf + 21))
			reply_len = -1;
	} else if (os_strncmp(buf, "REMOVE_NEIGHBOR ", 16) == 0) {
		if (hostapd_ctrl_iface_remove_neighbor(hapd, buf + 16))
			reply_len = -1;
	} else if (os_strncmp(buf, "REMOVE_NEIGHBOR_PER_VAP ", 24) == 0) {
		if (hostapd_ctrl_iface_remove_neighbor_per_vap(hapd, buf + 24))
			reply_len = -1;
	} else if (os_strncmp(buf, "REQ_LCI ", 8) == 0) {
		if (hostapd_ctrl_iface_req_lci(hapd, buf + 8))
			reply_len = -1;
	} else if (os_strncmp(buf, "REQ_RANGE ", 10) == 0) {
		if (hostapd_ctrl_iface_req_range(hapd, buf + 10))
			reply_len = -1;
	} else if (os_strncmp(buf, "REQ_BEACON ", 11) == 0) {
		reply_len = hostapd_ctrl_iface_req_beacon(hapd, buf + 11,
				reply, reply_size);
	} else if (os_strncmp(buf, "REPORT_BEACON ", 14) == 0) {
		if (hostapd_ctrl_iface_report_beacon(hapd, buf + 14))
			reply_len = -1;
	} else if (os_strncmp(buf, "REQ_SELF_BEACON ", 16) == 0) {
		if (hostapd_ctrl_iface_req_self_beacon(hapd, buf + 16))
			reply_len = -1;
	} else if (os_strncmp(buf, "REQ_CHANNEL_LOAD ", 17) == 0) {
		reply_len = hostapd_ctrl_iface_req_channel_load(hapd, buf + 17,
				reply, reply_size);
	} else if (os_strncmp(buf, "REPORT_CHANNEL_LOAD ", 20) == 0) {
		if (hostapd_ctrl_iface_report_channel_load(hapd, buf + 20))
			reply_len = -1;
	} else if (os_strncmp(buf, "REQ_NOISE_HISTOGRAM ", 20) == 0) {
		reply_len = hostapd_ctrl_iface_req_noise_histogram(hapd, buf + 20,
				reply, reply_size);
	} else if (os_strncmp(buf, "REPORT_NOISE_HISTOGRAM ", 23) == 0) {
		if (hostapd_ctrl_iface_report_noise_histogram(hapd, buf + 23))
			reply_len = -1;
	} else if (os_strncmp(buf, "REQ_LINK_MEASUREMENT ", 21) == 0) {
		reply_len = hostapd_ctrl_iface_req_link_measurement(hapd, buf + 21,
				reply, reply_size);
	} else if (os_strncmp(buf, "LINK_MEASUREMENT_REPORT ", 24) == 0) {
		if (hostapd_ctrl_iface_link_measurement_report(hapd, buf + 24))
			reply_len = -1;
	} else if (os_strncmp(buf, "REQ_STA_STATISTICS ", 19) == 0) {
		reply_len = hostapd_ctrl_iface_req_sta_statistics(hapd, buf + 19,
				reply, reply_size);
	} else if (os_strncmp(buf, "REPORT_STA_STATISTICS ", 22) == 0) {
		if (hostapd_ctrl_iface_report_sta_statistics(hapd, buf + 22))
			reply_len = -1;
	} else if (os_strcmp(buf, "DRIVER_FLAGS") == 0) {
		reply_len = hostapd_ctrl_driver_flags(hapd->iface, reply,
						      reply_size);
  } else if (os_strncmp(buf, "SET_VSIE ", 9) == 0) {
    if (hostapd_ctrl_iface_set_vsie(hapd, buf + 9))
      reply_len = -1;
  } else if (os_strncmp(buf, "DEL_VSIE ", 9) == 0) {
    if (hostapd_ctrl_iface_del_vsie(hapd, buf + 9))
      reply_len = -1;
  } else if (os_strncmp(buf, "GET_HW_FEATURES", 15) == 0) {
    reply_len = hostapd_ctrl_iface_get_hw_features(hapd->iface, NULL, reply,
      reply_size);
	} else {
		os_memcpy(reply, "UNKNOWN COMMAND\n", 16);
		reply_len = 16;
	}

	if (reply_len < 0) {
		os_memcpy(reply, "FAIL\n", 5);
		reply_len = 5;
	}

	return reply_len;
}



static void hostapd_ctrl_iface_receive(int sock, void *eloop_ctx,
				       void *sock_ctx)
{
	struct hostapd_data *hapd = eloop_ctx;
	char buf[4096];
	int res;
	struct sockaddr_storage from;
	socklen_t fromlen = sizeof(from);
	char *reply, *pos = buf;
	const int reply_size = 4096;
	int reply_len;
	int level = MSG_DEBUG;
#ifdef CONFIG_CTRL_IFACE_UDP
	unsigned char lcookie[COOKIE_LEN];
#endif /* CONFIG_CTRL_IFACE_UDP */

	res = recvfrom(sock, buf, sizeof(buf) - 1, 0,
		       (struct sockaddr *) &from, &fromlen);
	if (res < 0) {
		wpa_printf(MSG_ERROR, "recvfrom(ctrl_iface): %s",
			   strerror(errno));
		return;
	}
	buf[res] = '\0';

	reply = os_malloc(reply_size);
	if (reply == NULL) {
		if (sendto(sock, "FAIL\n", 5, 0, (struct sockaddr *) &from,
			   fromlen) < 0) {
			wpa_printf(MSG_DEBUG, "CTRL: sendto failed: %s",
				   strerror(errno));
		}
		return;
	}

#ifdef CONFIG_CTRL_IFACE_UDP
	if (os_strcmp(buf, "GET_COOKIE") == 0) {
		os_memcpy(reply, "COOKIE=", 7);
		wpa_snprintf_hex(reply + 7, 2 * COOKIE_LEN + 1,
				 cookie, COOKIE_LEN);
		reply_len = 7 + 2 * COOKIE_LEN;
		goto done;
	}

	if (os_strncmp(buf, "COOKIE=", 7) != 0 ||
	    hexstr2bin(buf + 7, lcookie, COOKIE_LEN) < 0) {
		wpa_printf(MSG_DEBUG,
			   "CTRL: No cookie in the request - drop request");
		os_free(reply);
		return;
	}

	if (os_memcmp(cookie, lcookie, COOKIE_LEN) != 0) {
		wpa_printf(MSG_DEBUG,
			   "CTRL: Invalid cookie in the request - drop request");
		os_free(reply);
		return;
	}

	pos = buf + 7 + 2 * COOKIE_LEN;
	while (*pos == ' ')
		pos++;
#endif /* CONFIG_CTRL_IFACE_UDP */

	if (os_strcmp(pos, "PING") == 0)
		level = MSG_EXCESSIVE;
	wpa_hexdump_ascii(level, "RX ctrl_iface", pos, res);

	reply_len = hostapd_ctrl_iface_receive_process(hapd, pos,
						       reply, reply_size,
						       &from, fromlen);

#ifdef CONFIG_CTRL_IFACE_UDP
done:
#endif /* CONFIG_CTRL_IFACE_UDP */
	if (sendto(sock, reply, reply_len, 0, (struct sockaddr *) &from,
		   fromlen) < 0) {
		wpa_printf(MSG_DEBUG, "CTRL: sendto failed: %s",
			   strerror(errno));
	}
	os_free(reply);
}


#ifndef CONFIG_CTRL_IFACE_UDP
static char * hostapd_ctrl_iface_path(struct hostapd_data *hapd)
{
	char *buf;
	size_t len;

	if (hapd->conf->ctrl_interface == NULL)
		return NULL;

	len = os_strlen(hapd->conf->ctrl_interface) +
		os_strlen(hapd->conf->iface) + 2;
	buf = os_malloc(len);
	if (buf == NULL)
		return NULL;

	os_snprintf(buf, len, "%s/%s",
		    hapd->conf->ctrl_interface, hapd->conf->iface);
	buf[len - 1] = '\0';
	return buf;
}
#endif /* CONFIG_CTRL_IFACE_UDP */


static void hostapd_ctrl_iface_msg_cb(void *ctx, int level,
				      enum wpa_msg_type type,
				      const char *txt, size_t len)
{
	struct hostapd_data *hapd = ctx;
	char *buf, *pos;
	size_t total, ifacelen;

	if (hapd == NULL)
		return;

	pos = os_strchr(txt, ' ');
	if (pos == NULL)
		total = len;
	else {
		total = pos - txt;
		pos++;
	}

	buf = os_malloc(len + sizeof(hapd->conf->iface));
	if (buf == NULL)
		return;

	ifacelen = strlen(hapd->conf->iface);
	os_memcpy(buf, txt, total);
	buf[total] = ' ';
	total++;
	os_memcpy(buf + total, hapd->conf->iface, ifacelen);
	total += ifacelen;
	if (pos) {
		buf[total] = ' ';
		total++;
		os_memcpy(buf + total, pos, len - (pos - txt));
	}

	/* send all events to the first VAP */
	hapd = hapd->iface->bss[0];

	hostapd_ctrl_iface_send(hapd, level, type, buf, len + ifacelen + 1);

	os_free(buf);
}


int hostapd_ctrl_iface_init(struct hostapd_data *hapd)
{
#ifdef CONFIG_CTRL_IFACE_UDP
	int port = HOSTAPD_CTRL_IFACE_PORT;
	char p[32] = { 0 };
	char port_str[40], *tmp;
	char *pos;
	struct addrinfo hints = { 0 }, *res, *saveres;
	int n;

	if (hapd->ctrl_sock > -1) {
		wpa_printf(MSG_DEBUG, "ctrl_iface already exists!");
		return 0;
	}

	if (hapd->conf->ctrl_interface == NULL)
		return 0;

	pos = os_strstr(hapd->conf->ctrl_interface, "udp:");
	if (pos) {
		pos += 4;
		port = atoi(pos);
		if (port <= 0) {
			wpa_printf(MSG_ERROR, "Invalid ctrl_iface UDP port");
			goto fail;
		}
	}

	dl_list_init(&hapd->ctrl_dst);
	hapd->ctrl_sock = -1;
	os_get_random(cookie, COOKIE_LEN);

#ifdef CONFIG_CTRL_IFACE_UDP_REMOTE
	hints.ai_flags = AI_PASSIVE;
#endif /* CONFIG_CTRL_IFACE_UDP_REMOTE */

#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
	hints.ai_family = AF_INET6;
#else /* CONFIG_CTRL_IFACE_UDP_IPV6 */
	hints.ai_family = AF_INET;
#endif /* CONFIG_CTRL_IFACE_UDP_IPV6 */
	hints.ai_socktype = SOCK_DGRAM;

try_again:
	os_snprintf(p, sizeof(p), "%d", port);
	n = getaddrinfo(NULL, p, &hints, &res);
	if (n) {
		wpa_printf(MSG_ERROR, "getaddrinfo(): %s", gai_strerror(n));
		goto fail;
	}

	saveres = res;
	hapd->ctrl_sock = socket(res->ai_family, res->ai_socktype,
				 res->ai_protocol);
	if (hapd->ctrl_sock < 0) {
		wpa_printf(MSG_ERROR, "socket(PF_INET): %s", strerror(errno));
		goto fail;
	}

	if (bind(hapd->ctrl_sock, res->ai_addr, res->ai_addrlen) < 0) {
		port--;
		if ((HOSTAPD_CTRL_IFACE_PORT - port) <
		    HOSTAPD_CTRL_IFACE_PORT_LIMIT && !pos)
			goto try_again;
		wpa_printf(MSG_ERROR, "bind(AF_INET): %s", strerror(errno));
		goto fail;
	}

	freeaddrinfo(saveres);

	os_snprintf(port_str, sizeof(port_str), "udp:%d", port);
	tmp = os_strdup(port_str);
	if (tmp) {
		os_free(hapd->conf->ctrl_interface);
		hapd->conf->ctrl_interface = tmp;
	}
	wpa_printf(MSG_DEBUG, "ctrl_iface_init UDP port: %d", port);

	if (eloop_register_read_sock(hapd->ctrl_sock,
				     hostapd_ctrl_iface_receive, hapd, NULL) <
	    0) {
		hostapd_ctrl_iface_deinit(hapd);
		return -1;
	}

	hapd->msg_ctx = hapd;
	wpa_msg_register_cb(hostapd_ctrl_iface_msg_cb);

	return 0;

fail:
	if (hapd->ctrl_sock >= 0)
		close(hapd->ctrl_sock);
	return -1;
#else /* CONFIG_CTRL_IFACE_UDP */
	struct sockaddr_un addr;
	int s = -1;
	char *fname = NULL;

	if (hapd->ctrl_sock > -1) {
		wpa_printf(MSG_DEBUG, "ctrl_iface already exists!");
		return 0;
	}

	dl_list_init(&hapd->ctrl_dst);

	if (hapd->conf->ctrl_interface == NULL)
		return 0;

	if (mkdir(hapd->conf->ctrl_interface, S_IRWXU | S_IRWXG) < 0) {
		if (errno == EEXIST) {
			wpa_printf(MSG_DEBUG, "Using existing control "
				   "interface directory.");
		} else {
			wpa_printf(MSG_ERROR, "mkdir[ctrl_interface]: %s",
				   strerror(errno));
			goto fail;
		}
	}

	if (hapd->conf->ctrl_interface_gid_set &&
	    chown(hapd->conf->ctrl_interface, -1,
		  hapd->conf->ctrl_interface_gid) < 0) {
		wpa_printf(MSG_ERROR, "chown[ctrl_interface]: %s",
			   strerror(errno));
		return -1;
	}

	if (!hapd->conf->ctrl_interface_gid_set &&
	    hapd->iface->interfaces->ctrl_iface_group &&
	    chown(hapd->conf->ctrl_interface, -1,
		  hapd->iface->interfaces->ctrl_iface_group) < 0) {
		wpa_printf(MSG_ERROR, "chown[ctrl_interface]: %s",
			   strerror(errno));
		return -1;
	}

#ifdef ANDROID
	/*
	 * Android is using umask 0077 which would leave the control interface
	 * directory without group access. This breaks things since Wi-Fi
	 * framework assumes that this directory can be accessed by other
	 * applications in the wifi group. Fix this by adding group access even
	 * if umask value would prevent this.
	 */
	if (chmod(hapd->conf->ctrl_interface, S_IRWXU | S_IRWXG) < 0) {
		wpa_printf(MSG_ERROR, "CTRL: Could not chmod directory: %s",
			   strerror(errno));
		/* Try to continue anyway */
	}
#endif /* ANDROID */

	if (os_strlen(hapd->conf->ctrl_interface) + 1 +
	    os_strlen(hapd->conf->iface) >= sizeof(addr.sun_path))
		goto fail;

	s = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (s < 0) {
		wpa_printf(MSG_ERROR, "socket(PF_UNIX): %s", strerror(errno));
		goto fail;
	}

	os_memset(&addr, 0, sizeof(addr));
#ifdef __FreeBSD__
	addr.sun_len = sizeof(addr);
#endif /* __FreeBSD__ */
	addr.sun_family = AF_UNIX;
	fname = hostapd_ctrl_iface_path(hapd);
	if (fname == NULL)
		goto fail;
	os_strlcpy(addr.sun_path, fname, sizeof(addr.sun_path));
	if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		wpa_printf(MSG_DEBUG, "ctrl_iface bind(PF_UNIX) failed: %s",
			   strerror(errno));
		if (connect(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
			wpa_printf(MSG_DEBUG, "ctrl_iface exists, but does not"
				   " allow connections - assuming it was left"
				   "over from forced program termination");
			if (unlink(fname) < 0) {
				wpa_printf(MSG_ERROR,
					   "Could not unlink existing ctrl_iface socket '%s': %s",
					   fname, strerror(errno));
				goto fail;
			}
			if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) <
			    0) {
				wpa_printf(MSG_ERROR,
					   "hostapd-ctrl-iface: bind(PF_UNIX): %s",
					   strerror(errno));
				goto fail;
			}
			wpa_printf(MSG_DEBUG, "Successfully replaced leftover "
				   "ctrl_iface socket '%s'", fname);
		} else {
			wpa_printf(MSG_INFO, "ctrl_iface exists and seems to "
				   "be in use - cannot override it");
			wpa_printf(MSG_INFO, "Delete '%s' manually if it is "
				   "not used anymore", fname);
			os_free(fname);
			fname = NULL;
			goto fail;
		}
	}

	if (hapd->conf->ctrl_interface_gid_set &&
	    chown(fname, -1, hapd->conf->ctrl_interface_gid) < 0) {
		wpa_printf(MSG_ERROR, "chown[ctrl_interface/ifname]: %s",
			   strerror(errno));
		goto fail;
	}

	if (!hapd->conf->ctrl_interface_gid_set &&
	    hapd->iface->interfaces->ctrl_iface_group &&
	    chown(fname, -1, hapd->iface->interfaces->ctrl_iface_group) < 0) {
		wpa_printf(MSG_ERROR, "chown[ctrl_interface/ifname]: %s",
			   strerror(errno));
		goto fail;
	}

	if (chmod(fname, S_IRWXU | S_IRWXG) < 0) {
		wpa_printf(MSG_ERROR, "chmod[ctrl_interface/ifname]: %s",
			   strerror(errno));
		goto fail;
	}
	os_free(fname);

	hapd->ctrl_sock = s;
	if (eloop_register_read_sock(s, hostapd_ctrl_iface_receive, hapd,
				     NULL) < 0) {
		hostapd_ctrl_iface_deinit(hapd);
		return -1;
	}
	hapd->msg_ctx = hapd;
	wpa_msg_register_cb(hostapd_ctrl_iface_msg_cb);

	return 0;

fail:
	if (s >= 0)
		close(s);
	if (fname) {
		unlink(fname);
		os_free(fname);
	}
	return -1;
#endif /* CONFIG_CTRL_IFACE_UDP */
}


void hostapd_ctrl_iface_deinit(struct hostapd_data *hapd)
{
	struct wpa_ctrl_dst *dst, *prev;

  wpa_msg(hapd->msg_ctx, MSG_INFO, HOSTAPD_DISCONNECTED);

	if (hapd->ctrl_sock > -1) {
#ifndef CONFIG_CTRL_IFACE_UDP
		char *fname;
#endif /* !CONFIG_CTRL_IFACE_UDP */

		eloop_unregister_read_sock(hapd->ctrl_sock);
		close(hapd->ctrl_sock);
		hapd->ctrl_sock = -1;
#ifndef CONFIG_CTRL_IFACE_UDP
		fname = hostapd_ctrl_iface_path(hapd);
		if (fname)
			unlink(fname);
		os_free(fname);

		if (hapd->conf->ctrl_interface &&
		    rmdir(hapd->conf->ctrl_interface) < 0) {
			if (errno == ENOTEMPTY) {
				wpa_printf(MSG_DEBUG, "Control interface "
					   "directory not empty - leaving it "
					   "behind");
			} else {
				wpa_printf(MSG_ERROR,
					   "rmdir[ctrl_interface=%s]: %s",
					   hapd->conf->ctrl_interface,
					   strerror(errno));
			}
		}
#endif /* !CONFIG_CTRL_IFACE_UDP */
	}

	dl_list_for_each_safe(dst, prev, &hapd->ctrl_dst, struct wpa_ctrl_dst,
			      list)
		os_free(dst);

#ifdef CONFIG_TESTING_OPTIONS
	l2_packet_deinit(hapd->l2_test);
	hapd->l2_test = NULL;
#endif /* CONFIG_TESTING_OPTIONS */
}


static int hostapd_ctrl_iface_add(struct hapd_interfaces *interfaces,
				  char *buf)
{
	if (hostapd_add_iface(interfaces, buf) < 0) {
		wpa_printf(MSG_ERROR, "Adding interface %s failed", buf);
		return -1;
	}
	return 0;
}


static int hostapd_ctrl_iface_remove(struct hapd_interfaces *interfaces,
				     char *buf)
{
	if (hostapd_remove_iface(interfaces, buf) < 0) {
		wpa_printf(MSG_ERROR, "Removing interface %s failed", buf);
		return -1;
	}
	return 0;
}


static int hostapd_global_ctrl_iface_attach(struct hapd_interfaces *interfaces,
					    struct sockaddr_storage *from,
					    socklen_t fromlen)
{
	return ctrl_iface_attach(&interfaces->global_ctrl_dst, from, fromlen);
}


static int hostapd_global_ctrl_iface_detach(struct hapd_interfaces *interfaces,
					    struct sockaddr_storage *from,
					    socklen_t fromlen)
{
	return ctrl_iface_detach(&interfaces->global_ctrl_dst, from, fromlen);
}


static void hostapd_ctrl_iface_flush(struct hapd_interfaces *interfaces)
{
#ifdef CONFIG_WPS_TESTING
	wps_version_number = 0x20;
	wps_testing_dummy_cred = 0;
	wps_corrupt_pkhash = 0;
#endif /* CONFIG_WPS_TESTING */
}


#ifdef CONFIG_FST

static int
hostapd_global_ctrl_iface_fst_attach(struct hapd_interfaces *interfaces,
				     const char *cmd)
{
	char ifname[IFNAMSIZ + 1];
	struct fst_iface_cfg cfg;
	struct hostapd_data *hapd;
	struct fst_wpa_obj iface_obj;

	if (!fst_parse_attach_command(cmd, ifname, sizeof(ifname), &cfg)) {
		hapd = hostapd_get_iface(interfaces, ifname);
		if (hapd) {
			if (hapd->iface->fst) {
				wpa_printf(MSG_INFO, "FST: Already attached");
				return -1;
			}
			fst_hostapd_fill_iface_obj(hapd, &iface_obj);
			hapd->iface->fst = fst_attach(ifname, hapd->own_addr,
						      &iface_obj, &cfg);
			if (hapd->iface->fst)
				return 0;
		}
	}

	return -EINVAL;
}


static int
hostapd_global_ctrl_iface_fst_detach(struct hapd_interfaces *interfaces,
				     const char *cmd)
{
	char ifname[IFNAMSIZ + 1];
	struct hostapd_data * hapd;

	if (!fst_parse_detach_command(cmd, ifname, sizeof(ifname))) {
		hapd = hostapd_get_iface(interfaces, ifname);
		if (hapd) {
			if (!fst_iface_detach(ifname)) {
				hapd->iface->fst = NULL;
				hapd->iface->fst_ies = NULL;
				return 0;
			}
		}
	}

	return -EINVAL;
}

#endif /* CONFIG_FST */


static struct hostapd_data *
hostapd_interfaces_get_hapd(struct hapd_interfaces *interfaces,
			    const char *ifname)
{
	size_t i, j;

	for (i = 0; i < interfaces->count; i++) {
		struct hostapd_iface *iface = interfaces->iface[i];

		for (j = 0; j < iface->num_bss; j++) {
			struct hostapd_data *hapd;

			hapd = iface->bss[j];
			if (os_strcmp(ifname, hapd->conf->iface) == 0)
				return hapd;
		}
	}

	return NULL;
}


static int hostapd_ctrl_iface_dup_param(struct hostapd_data *src_hapd,
					struct hostapd_data *dst_hapd,
					const char *param)
{
	int res;
	char *value;

	value = os_zalloc(HOSTAPD_CLI_DUP_VALUE_MAX_LEN);
	if (!value) {
		wpa_printf(MSG_ERROR,
			   "DUP: cannot allocate buffer to stringify %s",
			   param);
		goto error_return;
	}

	if (os_strcmp(param, "wpa") == 0) {
		os_snprintf(value, HOSTAPD_CLI_DUP_VALUE_MAX_LEN, "%d",
			    src_hapd->conf->wpa);
	} else if (os_strcmp(param, "wpa_key_mgmt") == 0 &&
		   src_hapd->conf->wpa_key_mgmt) {
		res = hostapd_ctrl_iface_get_key_mgmt(
			src_hapd, value, HOSTAPD_CLI_DUP_VALUE_MAX_LEN);
		if (os_snprintf_error(HOSTAPD_CLI_DUP_VALUE_MAX_LEN, res))
			goto error_stringify;
	} else if (os_strcmp(param, "wpa_pairwise") == 0 &&
		   src_hapd->conf->wpa_pairwise) {
		res = wpa_write_ciphers(value,
					value + HOSTAPD_CLI_DUP_VALUE_MAX_LEN,
					src_hapd->conf->wpa_pairwise, " ");
		if (res < 0)
			goto error_stringify;
	} else if (os_strcmp(param, "rsn_pairwise") == 0 &&
		   src_hapd->conf->rsn_pairwise) {
		res = wpa_write_ciphers(value,
					value + HOSTAPD_CLI_DUP_VALUE_MAX_LEN,
					src_hapd->conf->rsn_pairwise, " ");
		if (res < 0)
			goto error_stringify;
	} else if (os_strcmp(param, "wpa_passphrase") == 0 &&
		   src_hapd->conf->ssid.wpa_passphrase) {
		os_snprintf(value, HOSTAPD_CLI_DUP_VALUE_MAX_LEN, "%s",
			    src_hapd->conf->ssid.wpa_passphrase);
	} else if (os_strcmp(param, "wpa_psk") == 0 &&
		   src_hapd->conf->ssid.wpa_psk_set) {
		wpa_snprintf_hex(value, HOSTAPD_CLI_DUP_VALUE_MAX_LEN,
			src_hapd->conf->ssid.wpa_psk->psk, PMK_LEN);
	} else {
		wpa_printf(MSG_WARNING, "DUP: %s cannot be duplicated", param);
		goto error_return;
	}

	res = hostapd_set_iface(dst_hapd->iconf, dst_hapd->conf, param, value);
	os_free(value);
	return res;

error_stringify:
	wpa_printf(MSG_ERROR, "DUP: cannot stringify %s", param);
error_return:
	os_free(value);
	return -1;
}


static int
hostapd_global_ctrl_iface_interfaces(struct hapd_interfaces *interfaces,
				     const char *input,
				     char *reply, int reply_size)
{
	size_t i, j;
	int res;
	char *pos, *end;
	struct hostapd_iface *iface;
	int show_ctrl = 0;

	if (input)
		show_ctrl = !!os_strstr(input, "ctrl");

	pos = reply;
	end = reply + reply_size;

	for (i = 0; i < interfaces->count; i++) {
		iface = interfaces->iface[i];

		for (j = 0; j < iface->num_bss; j++) {
			struct hostapd_bss_config *conf;

			conf = iface->conf->bss[j];
			if (show_ctrl)
				res = os_snprintf(pos, end - pos,
						  "%s ctrl_iface=%s\n",
						  conf->iface,
						  conf->ctrl_interface ?
						  conf->ctrl_interface : "N/A");
			else
				res = os_snprintf(pos, end - pos, "%s\n",
						  conf->iface);
			if (os_snprintf_error(end - pos, res)) {
				*pos = '\0';
				return pos - reply;
			}
			pos += res;
		}
	}

	return pos - reply;
}


static int
hostapd_global_ctrl_iface_dup_network(struct hapd_interfaces *interfaces,
				      char *cmd)
{
	char *p_start = cmd, *p_end;
	struct hostapd_data *src_hapd, *dst_hapd;

	/* cmd: "<src ifname> <dst ifname> <variable name> */

	p_end = os_strchr(p_start, ' ');
	if (!p_end) {
		wpa_printf(MSG_ERROR, "DUP: no src ifname found in cmd: '%s'",
			   cmd);
		return -1;
	}

	*p_end = '\0';
	src_hapd = hostapd_interfaces_get_hapd(interfaces, p_start);
	if (!src_hapd) {
		wpa_printf(MSG_ERROR, "DUP: no src ifname found: '%s'",
			   p_start);
		return -1;
	}

	p_start = p_end + 1;
	p_end = os_strchr(p_start, ' ');
	if (!p_end) {
		wpa_printf(MSG_ERROR, "DUP: no dst ifname found in cmd: '%s'",
			   cmd);
		return -1;
	}

	*p_end = '\0';
	dst_hapd = hostapd_interfaces_get_hapd(interfaces, p_start);
	if (!dst_hapd) {
		wpa_printf(MSG_ERROR, "DUP: no dst ifname found: '%s'",
			   p_start);
		return -1;
	}

	p_start = p_end + 1;
	return hostapd_ctrl_iface_dup_param(src_hapd, dst_hapd, p_start);
}


static int hostapd_global_ctrl_iface_ifname(struct hapd_interfaces *interfaces,
					    const char *ifname,
					    char *buf, char *reply,
					    int reply_size,
					    struct sockaddr_storage *from,
					    socklen_t fromlen)
{
	struct hostapd_data *hapd;

	hapd = hostapd_interfaces_get_hapd(interfaces, ifname);
	if (hapd == NULL) {
		int res;

		res = os_snprintf(reply, reply_size, "FAIL-NO-IFNAME-MATCH\n");
		if (os_snprintf_error(reply_size, res))
			return -1;
		return res;
	}

	return hostapd_ctrl_iface_receive_process(hapd, buf, reply,reply_size,
						  from, fromlen);
}


static void hostapd_global_ctrl_iface_receive(int sock, void *eloop_ctx,
					      void *sock_ctx)
{
	void *interfaces = eloop_ctx;
	char buffer[256], *buf = buffer;
	int res;
	struct sockaddr_storage from;
	socklen_t fromlen = sizeof(from);
	char *reply;
	int reply_len;
	const int reply_size = 4096;
#ifdef CONFIG_CTRL_IFACE_UDP
	unsigned char lcookie[COOKIE_LEN];
#endif /* CONFIG_CTRL_IFACE_UDP */

	res = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
		       (struct sockaddr *) &from, &fromlen);
	if (res < 0) {
		wpa_printf(MSG_ERROR, "recvfrom(ctrl_iface): %s",
			   strerror(errno));
		return;
	}
	buf[res] = '\0';
	wpa_printf(MSG_DEBUG, "Global ctrl_iface command: %s", buf);

	reply = os_malloc(reply_size);
	if (reply == NULL) {
		if (sendto(sock, "FAIL\n", 5, 0, (struct sockaddr *) &from,
			   fromlen) < 0) {
			wpa_printf(MSG_DEBUG, "CTRL: sendto failed: %s",
				   strerror(errno));
		}
		return;
	}

	os_memcpy(reply, "OK\n", 3);
	reply_len = 3;

#ifdef CONFIG_CTRL_IFACE_UDP
	if (os_strcmp(buf, "GET_COOKIE") == 0) {
		os_memcpy(reply, "COOKIE=", 7);
		wpa_snprintf_hex(reply + 7, 2 * COOKIE_LEN + 1,
				 gcookie, COOKIE_LEN);
		reply_len = 7 + 2 * COOKIE_LEN;
		goto send_reply;
	}

	if (os_strncmp(buf, "COOKIE=", 7) != 0 ||
	    hexstr2bin(buf + 7, lcookie, COOKIE_LEN) < 0) {
		wpa_printf(MSG_DEBUG,
			   "CTRL: No cookie in the request - drop request");
		os_free(reply);
		return;
	}

	if (os_memcmp(gcookie, lcookie, COOKIE_LEN) != 0) {
		wpa_printf(MSG_DEBUG,
			   "CTRL: Invalid cookie in the request - drop request");
		os_free(reply);
		return;
	}

	buf += 7 + 2 * COOKIE_LEN;
	while (*buf == ' ')
		buf++;
#endif /* CONFIG_CTRL_IFACE_UDP */

	if (os_strncmp(buf, "IFNAME=", 7) == 0) {
		char *pos = os_strchr(buf + 7, ' ');

		if (pos) {
			*pos++ = '\0';
			reply_len = hostapd_global_ctrl_iface_ifname(
				interfaces, buf + 7, pos, reply, reply_size,
				&from, fromlen);
			goto send_reply;
		}
	}

	if (os_strcmp(buf, "PING") == 0) {
		os_memcpy(reply, "PONG\n", 5);
		reply_len = 5;
	} else if (os_strncmp(buf, "RELOG", 5) == 0) {
		if (wpa_debug_reopen_file() < 0)
			reply_len = -1;
	} else if (os_strcmp(buf, "FLUSH") == 0) {
		hostapd_ctrl_iface_flush(interfaces);
	} else if (os_strncmp(buf, "ADD ", 4) == 0) {
		if (hostapd_ctrl_iface_add(interfaces, buf + 4) < 0)
			reply_len = -1;
	} else if (os_strncmp(buf, "REMOVE ", 7) == 0) {
		if (hostapd_ctrl_iface_remove(interfaces, buf + 7) < 0)
			reply_len = -1;
	} else if (os_strcmp(buf, "ATTACH") == 0) {
		if (hostapd_global_ctrl_iface_attach(interfaces, &from,
						     fromlen))
			reply_len = -1;
	} else if (os_strcmp(buf, "DETACH") == 0) {
		if (hostapd_global_ctrl_iface_detach(interfaces, &from,
			fromlen))
			reply_len = -1;
#ifdef CONFIG_MODULE_TESTS
	} else if (os_strcmp(buf, "MODULE_TESTS") == 0) {
		if (hapd_module_tests() < 0)
			reply_len = -1;
#endif /* CONFIG_MODULE_TESTS */
#ifdef CONFIG_FST
	} else if (os_strncmp(buf, "FST-ATTACH ", 11) == 0) {
		if (!hostapd_global_ctrl_iface_fst_attach(interfaces, buf + 11))
			reply_len = os_snprintf(reply, reply_size, "OK\n");
		else
			reply_len = -1;
	} else if (os_strncmp(buf, "FST-DETACH ", 11) == 0) {
		if (!hostapd_global_ctrl_iface_fst_detach(interfaces, buf + 11))
			reply_len = os_snprintf(reply, reply_size, "OK\n");
		else
			reply_len = -1;
	} else if (os_strncmp(buf, "FST-MANAGER ", 12) == 0) {
		reply_len = fst_ctrl_iface_receive(buf + 12, reply, reply_size);
#endif /* CONFIG_FST */
	} else if (os_strncmp(buf, "DUP_NETWORK ", 12) == 0) {
		if (!hostapd_global_ctrl_iface_dup_network(interfaces,
							   buf + 12))
			reply_len = os_snprintf(reply, reply_size, "OK\n");
		else
			reply_len = -1;
	} else if (os_strncmp(buf, "INTERFACES", 10) == 0) {
		reply_len = hostapd_global_ctrl_iface_interfaces(
			interfaces, buf + 10, reply, sizeof(buffer));
	} else if (os_strcmp(buf, "TERMINATE") == 0) {
		eloop_terminate();
	} else {
		wpa_printf(MSG_DEBUG, "Unrecognized global ctrl_iface command "
			   "ignored");
		reply_len = -1;
	}

send_reply:
	if (reply_len < 0) {
		os_memcpy(reply, "FAIL\n", 5);
		reply_len = 5;
	}

	if (sendto(sock, reply, reply_len, 0, (struct sockaddr *) &from,
		   fromlen) < 0) {
		wpa_printf(MSG_DEBUG, "CTRL: sendto failed: %s",
			   strerror(errno));
	}
	os_free(reply);
}


#ifndef CONFIG_CTRL_IFACE_UDP
static char * hostapd_global_ctrl_iface_path(struct hapd_interfaces *interface)
{
	char *buf;
	size_t len;

	if (interface->global_iface_path == NULL)
		return NULL;

	len = os_strlen(interface->global_iface_path) +
		os_strlen(interface->global_iface_name) + 2;
	buf = os_malloc(len);
	if (buf == NULL)
		return NULL;

	os_snprintf(buf, len, "%s/%s", interface->global_iface_path,
		    interface->global_iface_name);
	buf[len - 1] = '\0';
	return buf;
}
#endif /* CONFIG_CTRL_IFACE_UDP */


int hostapd_global_ctrl_iface_init(struct hapd_interfaces *interface)
{
#ifdef CONFIG_CTRL_IFACE_UDP
	int port = HOSTAPD_GLOBAL_CTRL_IFACE_PORT;
	char p[32] = { 0 };
	char *pos;
	struct addrinfo hints = { 0 }, *res, *saveres;
	int n;

	if (interface->global_ctrl_sock > -1) {
		wpa_printf(MSG_DEBUG, "ctrl_iface already exists!");
		return 0;
	}

	if (interface->global_iface_path == NULL)
		return 0;

	pos = os_strstr(interface->global_iface_path, "udp:");
	if (pos) {
		pos += 4;
		port = atoi(pos);
		if (port <= 0) {
			wpa_printf(MSG_ERROR, "Invalid global ctrl UDP port");
			goto fail;
		}
	}

	dl_list_init(&interface->global_ctrl_dst);
	interface->global_ctrl_sock = -1;
	os_get_random(gcookie, COOKIE_LEN);

#ifdef CONFIG_CTRL_IFACE_UDP_REMOTE
	hints.ai_flags = AI_PASSIVE;
#endif /* CONFIG_CTRL_IFACE_UDP_REMOTE */

#ifdef CONFIG_CTRL_IFACE_UDP_IPV6
	hints.ai_family = AF_INET6;
#else /* CONFIG_CTRL_IFACE_UDP_IPV6 */
	hints.ai_family = AF_INET;
#endif /* CONFIG_CTRL_IFACE_UDP_IPV6 */
	hints.ai_socktype = SOCK_DGRAM;

try_again:
	os_snprintf(p, sizeof(p), "%d", port);
	n = getaddrinfo(NULL, p, &hints, &res);
	if (n) {
		wpa_printf(MSG_ERROR, "getaddrinfo(): %s", gai_strerror(n));
		goto fail;
	}

	saveres = res;
	interface->global_ctrl_sock = socket(res->ai_family, res->ai_socktype,
					     res->ai_protocol);
	if (interface->global_ctrl_sock < 0) {
		wpa_printf(MSG_ERROR, "socket(PF_INET): %s", strerror(errno));
		goto fail;
	}

	if (bind(interface->global_ctrl_sock, res->ai_addr, res->ai_addrlen) <
	    0) {
		port++;
		if ((port - HOSTAPD_GLOBAL_CTRL_IFACE_PORT) <
		    HOSTAPD_GLOBAL_CTRL_IFACE_PORT_LIMIT && !pos)
			goto try_again;
		wpa_printf(MSG_ERROR, "bind(AF_INET): %s", strerror(errno));
		goto fail;
	}

	freeaddrinfo(saveres);

	wpa_printf(MSG_DEBUG, "global ctrl_iface_init UDP port: %d", port);

	if (eloop_register_read_sock(interface->global_ctrl_sock,
				     hostapd_global_ctrl_iface_receive,
				     interface, NULL) < 0) {
		hostapd_global_ctrl_iface_deinit(interface);
		return -1;
	}

	return 0;

fail:
	if (interface->global_ctrl_sock >= 0)
		close(interface->global_ctrl_sock);
	return -1;
#else /* CONFIG_CTRL_IFACE_UDP */
	struct sockaddr_un addr;
	int s = -1;
	char *fname = NULL;

	if (interface->global_iface_path == NULL) {
		wpa_printf(MSG_DEBUG, "ctrl_iface not configured!");
		return 0;
	}

	if (mkdir(interface->global_iface_path, S_IRWXU | S_IRWXG) < 0) {
		if (errno == EEXIST) {
			wpa_printf(MSG_DEBUG, "Using existing control "
				   "interface directory.");
		} else {
			wpa_printf(MSG_ERROR, "mkdir[ctrl_interface]: %s",
				   strerror(errno));
			goto fail;
		}
	} else if (interface->ctrl_iface_group &&
		   chown(interface->global_iface_path, -1,
			 interface->ctrl_iface_group) < 0) {
		wpa_printf(MSG_ERROR, "chown[ctrl_interface]: %s",
			   strerror(errno));
		goto fail;
	}

	if (os_strlen(interface->global_iface_path) + 1 +
	    os_strlen(interface->global_iface_name) >= sizeof(addr.sun_path))
		goto fail;

	s = socket(PF_UNIX, SOCK_DGRAM, 0);
	if (s < 0) {
		wpa_printf(MSG_ERROR, "socket(PF_UNIX): %s", strerror(errno));
		goto fail;
	}

	os_memset(&addr, 0, sizeof(addr));
#ifdef __FreeBSD__
	addr.sun_len = sizeof(addr);
#endif /* __FreeBSD__ */
	addr.sun_family = AF_UNIX;
	fname = hostapd_global_ctrl_iface_path(interface);
	if (fname == NULL)
		goto fail;
	os_strlcpy(addr.sun_path, fname, sizeof(addr.sun_path));
	if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
		wpa_printf(MSG_DEBUG, "ctrl_iface bind(PF_UNIX) failed: %s",
			   strerror(errno));
		if (connect(s, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
			wpa_printf(MSG_DEBUG, "ctrl_iface exists, but does not"
				   " allow connections - assuming it was left"
				   "over from forced program termination");
			if (unlink(fname) < 0) {
				wpa_printf(MSG_ERROR,
					   "Could not unlink existing ctrl_iface socket '%s': %s",
					   fname, strerror(errno));
				goto fail;
			}
			if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) <
			    0) {
				wpa_printf(MSG_ERROR, "bind(PF_UNIX): %s",
					   strerror(errno));
				goto fail;
			}
			wpa_printf(MSG_DEBUG, "Successfully replaced leftover "
				   "ctrl_iface socket '%s'", fname);
		} else {
			wpa_printf(MSG_INFO, "ctrl_iface exists and seems to "
				   "be in use - cannot override it");
			wpa_printf(MSG_INFO, "Delete '%s' manually if it is "
				   "not used anymore", fname);
			os_free(fname);
			fname = NULL;
			goto fail;
		}
	}

	if (interface->ctrl_iface_group &&
	    chown(fname, -1, interface->ctrl_iface_group) < 0) {
		wpa_printf(MSG_ERROR, "chown[ctrl_interface]: %s",
			   strerror(errno));
		goto fail;
	}

	if (chmod(fname, S_IRWXU | S_IRWXG) < 0) {
		wpa_printf(MSG_ERROR, "chmod[ctrl_interface/ifname]: %s",
			   strerror(errno));
		goto fail;
	}
	os_free(fname);

	interface->global_ctrl_sock = s;
	eloop_register_read_sock(s, hostapd_global_ctrl_iface_receive,
				 interface, NULL);

	return 0;

fail:
	if (s >= 0)
		close(s);
	if (fname) {
		unlink(fname);
		os_free(fname);
	}
	return -1;
#endif /* CONFIG_CTRL_IFACE_UDP */
}


void hostapd_global_ctrl_iface_deinit(struct hapd_interfaces *interfaces)
{
#ifndef CONFIG_CTRL_IFACE_UDP
	char *fname = NULL;
#endif /* CONFIG_CTRL_IFACE_UDP */
	struct wpa_ctrl_dst *dst, *prev;

	if (interfaces->global_ctrl_sock > -1) {
		eloop_unregister_read_sock(interfaces->global_ctrl_sock);
		close(interfaces->global_ctrl_sock);
		interfaces->global_ctrl_sock = -1;
#ifndef CONFIG_CTRL_IFACE_UDP
		fname = hostapd_global_ctrl_iface_path(interfaces);
		if (fname) {
			unlink(fname);
			os_free(fname);
		}

		if (interfaces->global_iface_path &&
		    rmdir(interfaces->global_iface_path) < 0) {
			if (errno == ENOTEMPTY) {
				wpa_printf(MSG_DEBUG, "Control interface "
					   "directory not empty - leaving it "
					   "behind");
			} else {
				wpa_printf(MSG_ERROR,
					   "rmdir[ctrl_interface=%s]: %s",
					   interfaces->global_iface_path,
					   strerror(errno));
			}
		}
#endif /* CONFIG_CTRL_IFACE_UDP */
	}

	os_free(interfaces->global_iface_path);
	interfaces->global_iface_path = NULL;

	dl_list_for_each_safe(dst, prev, &interfaces->global_ctrl_dst,
			      struct wpa_ctrl_dst, list)
		os_free(dst);
}


static void hostapd_ctrl_iface_send(struct hostapd_data *hapd, int level,
				    enum wpa_msg_type type,
				    const char *buf, size_t len)
{
	struct wpa_ctrl_dst *dst, *next;
	struct dl_list *ctrl_dst;
	struct msghdr msg;
	int idx;
	struct iovec io[2];
	char levelstr[10];
	int s;

	if (type != WPA_MSG_ONLY_GLOBAL) {
		s = hapd->ctrl_sock;
		ctrl_dst = &hapd->ctrl_dst;
	} else {
		s = hapd->iface->interfaces->global_ctrl_sock;
		ctrl_dst = &hapd->iface->interfaces->global_ctrl_dst;
	}

	if (s < 0 || dl_list_empty(ctrl_dst))
		return;

	os_snprintf(levelstr, sizeof(levelstr), "<%d>", level);
	io[0].iov_base = levelstr;
	io[0].iov_len = os_strlen(levelstr);
	io[1].iov_base = (char *) buf;
	io[1].iov_len = len;
	os_memset(&msg, 0, sizeof(msg));
	msg.msg_iov = io;
	msg.msg_iovlen = 2;

	idx = 0;
	dl_list_for_each_safe(dst, next, ctrl_dst, struct wpa_ctrl_dst, list) {
		if (level >= dst->debug_level) {
			sockaddr_print(MSG_DEBUG, "CTRL_IFACE monitor send",
				       &dst->addr, dst->addrlen);
			msg.msg_name = &dst->addr;
			msg.msg_namelen = dst->addrlen;
			if (sendmsg(s, &msg, 0) < 0) {
				int _errno = errno;
				wpa_printf(MSG_INFO, "CTRL_IFACE monitor[%d]: "
					   "%d - %s",
					   idx, errno, strerror(errno));
				dst->errors++;
				if (dst->errors > 10 || _errno == ENOENT) {
					if (type != WPA_MSG_ONLY_GLOBAL)
						hostapd_ctrl_iface_detach(
							hapd, &dst->addr,
							dst->addrlen);
					else
						hostapd_global_ctrl_iface_detach(
							hapd->iface->interfaces,
							&dst->addr,
							dst->addrlen);
				}
			} else
				dst->errors = 0;
		}
		idx++;
	}
}

#endif /* CONFIG_NATIVE_WINDOWS */
