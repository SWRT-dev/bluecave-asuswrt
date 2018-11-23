#include "includes.h"
#include "common.h"
#include "drivers/driver.h"
#include "hostapd.h"
#include "sta_info.h"
#include "atf.h"

static struct atf_vap_config* add_vap_to_atf_config(struct atf_config* atf_cfg)
{
	struct atf_vap_config* new_vap;

	/* Allocate space for an increased vap_cfg array */
	int old_len = sizeof(struct atf_vap_config) * atf_cfg->n_vaps;
	int new_len = old_len + sizeof(struct atf_vap_config);
	struct atf_vap_config* new_vap_cfg = os_malloc(new_len);
	if (new_vap_cfg == NULL)
		return NULL;

	/* Move the existing data to the newly allocated array */
	if (old_len > 0) {
		os_memcpy (new_vap_cfg, atf_cfg->vap_cfg, old_len);
		os_free (atf_cfg->vap_cfg);
	}
	atf_cfg->vap_cfg = new_vap_cfg;

	new_vap = new_vap_cfg + atf_cfg->n_vaps;
	atf_cfg->n_vaps++;
	memset (new_vap, 0, sizeof(struct atf_vap_config));
	return new_vap;
}

static struct atf_sta_grant* add_sta_to_vap_config(struct atf_vap_config* vap_cfg)
{
	struct atf_sta_grant* new_sta;

	/* Allocate space for an increased vap_cfg array */
	int old_len = sizeof(struct atf_vap_config) * vap_cfg->n_stations;
	int new_len = old_len + sizeof(struct atf_vap_config);
	struct atf_sta_grant* new_sta_grants = os_malloc(new_len);

	if (new_sta_grants == NULL)
		return NULL;

	/* Move the existing data to the newly allocated array */
	if (old_len > 0) {
		os_memcpy (new_sta_grants, vap_cfg->sta_grants, old_len);
		os_free (vap_cfg->sta_grants);
	}
	vap_cfg->sta_grants = new_sta_grants;

	new_sta = new_sta_grants + vap_cfg->n_stations;
	vap_cfg->n_stations++;
	memset (new_sta, 0, sizeof(struct atf_sta_grant));
	return new_sta;
}

/* Free allocated memory and reset contents */
void hostapd_atf_clean_config(struct atf_config* atf_cfg)
{
	int n_vap;

	for (n_vap = 0; n_vap < atf_cfg->n_vaps; n_vap++) {
		struct atf_vap_config* vap = &atf_cfg->vap_cfg[n_vap];
		os_free(vap->vap_name);
		os_free(vap->sta_grants);
	}

	os_free (atf_cfg->vap_cfg);
	memset (atf_cfg, 0, sizeof(struct atf_config));
}

/* Flush ATF data for all stations, e.g. after recovery when stations have not
 * been removed from Driver cleanly. */
void hostapd_atf_clean_stations(struct hostapd_data *hapd)
{
	struct hostapd_iface *iface = hapd->iface;
	memset(iface->atf_sta_in_driver, 0, sizeof(iface->atf_sta_in_driver));
	memset(iface->atf_sta_has_quota, 0, sizeof(iface->atf_sta_has_quota));
}

/* Find a token in the given character string. Besides the characters recognized as
 * spaces by isspace (including '\t' and '\n'), additional delimiters are passed
 * by 'delimiters' argument.
 */
static void read_token (char **p, char** token_start, int* token_len, char* delimiters)
{
	/* skip possible spaces before token */
	while (isspace(**p))
		(*p)++;
	*token_start = *p;

	/* find the end of the token */
	while (**p != '\0' && !isspace(**p) && os_strchr(delimiters, **p) == NULL)
		(*p)++;
	*token_len = *p - *token_start;

	/* skip possible spaces after token */
	while (isspace(**p))
		(*p)++;
}

/* Produce a copy of original cfg file -- to verify that configuration has been correctly read */
/* Only used for debugging purposes */
static void hostapd_atf_dbg_print_config(struct atf_config* atf_cfg, const char* fname)
{
	char buf[256];
	int i_vap;
	FILE *f;

	os_snprintf (buf, sizeof(buf), "%s.tst", fname);
	f = fopen(buf, "w");
	if (f == NULL)
		return;

	fprintf(f, "distr_type = %d\n", atf_cfg->distr_type);
	fprintf(f, "algo_type = %d\n", atf_cfg->algo_type);
	fprintf(f, "weighted_type = %d\n", atf_cfg->weighted_type);
	fprintf(f, "interval = %d\n", atf_cfg->interval);
	fprintf(f, "free_time = %d\n", atf_cfg->free_time);

	for (i_vap = 0; i_vap < atf_cfg->n_vaps; i_vap++) {
		int i_sta;
		struct atf_vap_config *vap = atf_cfg->vap_cfg + i_vap;
		fprintf(f, "\n[%s]\n", vap->vap_name);

		fprintf(f, "vap_grant = %d\n", vap->vap_grant);
		for (i_sta = 0; i_sta < vap->n_stations; i_sta++) {
			struct atf_sta_grant *sta = vap->sta_grants + i_sta;
			fprintf(f, "sta = " MACSTR ", %d\n",
					MAC2STR(sta->sta_mac), sta->sta_grant);
		}
	}

	fclose (f);
}

/* Read ATF configuration from file */
int hostapd_atf_read_config(struct atf_config* atf_cfg, const char* fname)
{
	char buf[256], *pos;
	int line = 0;
	struct atf_vap_config* curr_vap = NULL;
	FILE *f = fopen(fname, "r");
	if (f == NULL) {
		wpa_printf(MSG_ERROR, "ATF: Could not open config file '%s'.", fname);
		return -1;
	}

	hostapd_atf_clean_config(atf_cfg);

	/* We don't use any special handling for '\n' because isspace('\n') returns true. */
	while (fgets(buf, sizeof(buf), f)) {
		line++;

		/* Skip leading spaces. */
		pos = buf;
		while (isspace(*pos))
			pos++;

		if (atf_cfg->per_vap && *pos == '[') { /* New section (VAP name in square brackets) */
			char *name = pos;
			int  name_len = 0;

			/* Extract VAP name from square brackets */
			pos++; /* go past '[' */
			read_token (&pos, &name, &name_len, "[]=,#");
			if (*pos != ']' || name_len == 0)
				goto syntax_error;

			curr_vap = add_vap_to_atf_config(atf_cfg);
			if (curr_vap == NULL)
				goto mem_error;
			curr_vap->vap_name = os_malloc (name_len+1);
			if (curr_vap->vap_name == NULL)
				goto mem_error;
			os_memcpy(curr_vap->vap_name, name, name_len);
			curr_vap->vap_name[name_len] = 0;
		}
		else if (*pos != '\0' && *pos != '#') { /* <name> = <value> */
			char *name, *value, *endptr;
			int  name_len, value_len;
			read_token (&pos, &name, &name_len, "=");
			if(*pos != '=')
				goto syntax_error;
			pos++;

			/* Value is the rest of the line, it can contain spaces */
			/* However, cut off the leading and trailing spaces */
			while (isspace(*pos))
				(pos)++;
			value = pos;
			value_len = 0;
			while (*pos != '\0' && *pos != '#') {
				if (!isspace(*pos))
					value_len = pos - value + 1;
				pos++;
			}

			/* Make name and value null-terminated strings */
			name[name_len] = 0;
			value[value_len] = 0;

			/* We are before [<vap_name>] header. Per-radio parameters expected */
			if (strcmp(name, "debug") == 0) {
				atf_cfg->debug = strtol(value, &endptr, 10);
				if (*endptr != '\0')
					goto syntax_error;
			}
			else if (strcmp(name, "distr_type") == 0) {
				atf_cfg->distr_type = strtol(value, &endptr, 10);
				if (*endptr != '\0')
					goto syntax_error;
			}
			else if (strcmp(name, "algo_type") == 0) {
				atf_cfg->algo_type = strtol(value, &endptr, 10);
				if (*endptr != '\0')
					goto syntax_error;
			}
			else if (strcmp(name, "weighted_type") == 0) {
				atf_cfg->weighted_type = strtol(value, &endptr, 10);
				if (*endptr != '\0')
					goto syntax_error;
			}
			else if (strcmp(name, "interval") == 0) {
				atf_cfg->interval = strtol(value, &endptr, 10);
				if (*endptr != '\0')
					goto syntax_error;
			}
			else if (strcmp(name, "free_time") == 0) {
				atf_cfg->free_time = strtol(value, &endptr, 10);
				if (*endptr != '\0')
					goto syntax_error;
			}
			else if (strcmp(name, "vap_enabled") == 0) {
				atf_cfg->per_vap = strtol(value, &endptr, 10);
				if (*endptr != '\0')
					goto syntax_error;
			}
			else if (strcmp(name, "station_enabled") == 0) {
				atf_cfg->per_station = strtol(value, &endptr, 10);
				if (*endptr != '\0')
					goto syntax_error;
			}
			else if (curr_vap && strcmp(name, "vap_grant") == 0) {
				curr_vap->vap_grant = strtol(value, &endptr, 10);
				if (*endptr != '\0')
					goto syntax_error;
			}
			else if (strcmp(name, "sta") == 0) {
				char *sta_mac, *sta_grant, *endptr;
				int  sta_mac_len, sta_grant_len;
				struct atf_sta_grant* sta;

				if (atf_cfg->per_station == 0)
					continue; /* ignore it */

				if (curr_vap == NULL) {
					if (atf_cfg->per_vap)
						goto syntax_error; /* per VAP mode, but didn't see [VAP] above */

					/* NULL value of curr_vap->vap_name indicates that it is per radio */
					curr_vap = add_vap_to_atf_config(atf_cfg);
					if (curr_vap == NULL)
						goto mem_error;
				}

				/* split the value to two comma-separated parts */
				read_token (&value, &sta_mac, &sta_mac_len, ",");
				if (*value != ',')
					goto syntax_error;
				value++;
				read_token (&value, &sta_grant, &sta_grant_len, ",");

				/* Make the MAC and grant parts null-terminated */
				sta_mac[sta_mac_len] = 0;
				sta_grant[sta_grant_len] = 0;

				sta = add_sta_to_vap_config(curr_vap);
				if (hwaddr_aton(sta_mac, sta->sta_mac))
					goto syntax_error;
				sta->sta_grant = strtol(sta_grant, &endptr, 10);
				if (*endptr != '\0')
					goto syntax_error;
			}
			else /* Unknown parameter name */
				goto syntax_error;
		}
	}

	fclose (f);
	if (atf_cfg->debug)
		hostapd_atf_dbg_print_config(atf_cfg, fname);

	/* Validate ATF configuration */
	if (atf_cfg->per_vap)
	{
		int total_vap_grant = 0;
		int n_vap;

		for (n_vap = 0; n_vap < atf_cfg->n_vaps; n_vap++) {
			struct atf_vap_config *vap_cfg = atf_cfg->vap_cfg + n_vap;
			if (vap_cfg->vap_grant < ATF_MIN_VAP_GRANT) {
				wpa_printf(MSG_ERROR, "ATF: Invalid data in %s, VAP %s got grant "
						"below %d (min=%d)", fname, vap_cfg->vap_name,
						vap_cfg->vap_grant, ATF_MIN_VAP_GRANT);
				hostapd_atf_clean_config(atf_cfg);
				return -1;
			}

			total_vap_grant += vap_cfg->vap_grant;
		}

		if (total_vap_grant > ATF_GRANT_SCALE) {
			wpa_printf(MSG_ERROR, "ATF: Invalid data in %s, total VAP grant "
					"%d exceeds %d", fname, total_vap_grant, ATF_GRANT_SCALE);
			hostapd_atf_clean_config(atf_cfg);
			return -1;
		}
	}

	wpa_printf(MSG_DEBUG, "ATF: Reading config file %s successful (%d lines)", fname, line);
	return 0; /* success */

syntax_error:
	fclose (f);
	wpa_printf(MSG_ERROR, "ATF: Syntax error in config file '%s', line %d.", fname, line);
	hostapd_atf_clean_config(atf_cfg);
	return -1;

mem_error:
	fclose (f);
	wpa_printf(MSG_ERROR, "ATF: Insufficient memory to parse config file '%s'.", fname);
	hostapd_atf_clean_config(atf_cfg);
	return -1;
}

static inline u16 sta_id_in_driver(struct sta_info* sta)
{
	return sta->aid - 1; /* station indexes in Driver are 0-based */
}

static int update_atf_active_status_for_station (struct sta_info* sta,
		int in_driver, int not_in_driver,
		u32* sta_in_driver, u32* sta_has_quota)
{
	u32 idx = sta->aid / 32;
	u32 bit = BIT(sta->aid % 32);

	if (in_driver)
		sta_in_driver[idx] |= bit;
	if (not_in_driver)
		sta_in_driver[idx] &= ~bit;

	if ((sta_has_quota[idx] & bit) == 0) {
		/* STA does not have ATF quota yet. Shall we give it?*/
		/* Give quota if STA is in Driver and it has an 'authorized' flag. */
		if ((sta_in_driver[idx] & bit) != 0 &&
				(sta->flags & WLAN_STA_AUTHORIZED) != 0) {
			sta_has_quota[idx] |= bit;
			return 1; /* Station changed its status */
		}
	}
	else if (not_in_driver) {
		/* Switch STA status to inactive when it is removed from Driver */
		sta_has_quota[idx] &= ~bit;
		return 1; /* Station changed its status */
	}

	return 0;
}

static inline int is_sta_active(struct sta_info* sta, u32* sta_has_quota)
{
	u32 idx = sta->aid / 32;
	u32 bit = BIT(sta->aid % 32);

	return (sta_has_quota[idx] & bit) != 0;
 }

static int count_active_stations_in_bss(struct hostapd_data *bss,
		u32* sta_has_quota)
{
	int count = 0;
	struct sta_info* sta;

	for (sta = bss->sta_list; sta != NULL; sta = sta->next) {
		if (is_sta_active(sta, sta_has_quota))
			count++;
	}
	return count;
}

static void distribute_sta_quotas_per_radio(
		struct atf_vap_config* vap_cfg, /* per radio station list */
		size_t num_bss, struct hostapd_data **bss, /* Configured BSS-es */
		uint16_t* sta_grant, /* per-station quotas */
		u32* sta_has_quota)
{
	int n_bss, n_sta, total_stations = 0, remaining_quota = ATF_GRANT_SCALE;

	for (n_bss = 0; n_bss < num_bss; n_bss++)
		total_stations += count_active_stations_in_bss(bss[n_bss], sta_has_quota);
	if (total_stations == 0)
		return;

	/* Distribute the configured quotas for connected stations */
	if (vap_cfg) {
		for (n_sta = 0; n_sta < vap_cfg->n_stations; n_sta++)
		{
			struct atf_sta_grant* sta_cfg = vap_cfg->sta_grants + n_sta;
			struct sta_info* sta = NULL;

			/* Find a station given in configuration. Search through all BSS */
			for (n_bss = 0; n_bss < num_bss; n_bss++) {
				sta = ap_get_sta(bss[n_bss], sta_cfg->sta_mac);
				if (sta && is_sta_active(sta, sta_has_quota)) {

					wpa_printf(MSG_DEBUG, "ATF: STA " MACSTR " (id = %d) gets grant %d",
							MAC2STR(sta->addr), sta_id_in_driver(sta), sta_cfg->sta_grant);

					sta_grant[sta_id_in_driver(sta)] = sta_cfg->sta_grant;
					remaining_quota -= (int)sta_cfg->sta_grant;
				}
			}
		}
	}

	/* Distribute the remaining quota equally between stations */
	if (remaining_quota > 0) {
		uint16_t remaining_per_sta = remaining_quota / total_stations;

		wpa_printf(MSG_DEBUG, "ATF: Distributing remaining grant equally per radio. "
				"Each of %d stations gets %d.", total_stations, remaining_per_sta);

		for (n_bss = 0; n_bss < num_bss; n_bss++) {
			struct sta_info *sta;

			for (sta = bss[n_bss]->sta_list; sta != NULL; sta = sta->next) {
				if (is_sta_active(sta, sta_has_quota))
					sta_grant[sta_id_in_driver(sta)] += remaining_per_sta;
			}
		}
	}
}

static void distribute_sta_quotas_per_vap(
		struct atf_vap_config* vap_cfg, /* per-vap cfg, includes station list */
		struct hostapd_data *bss, /* BSS from hostapd config */
		uint16_t* sta_grant, /* per-station quotas */
		u32* sta_has_quota)
{
	int n_sta, remaining_in_vap = vap_cfg->vap_grant;
	int count_in_bss = count_active_stations_in_bss(bss, sta_has_quota);

	/* Cycle by stations in configuration and see which of them are connected */
	for (n_sta = 0; n_sta < vap_cfg->n_stations; n_sta++)
	{
		struct atf_sta_grant* sta_cfg = vap_cfg->sta_grants + n_sta;
		struct sta_info* sta = ap_get_sta(bss, sta_cfg->sta_mac);

		if (sta && is_sta_active(sta, sta_has_quota)) {
			uint16_t station_grant = ((uint32_t)(vap_cfg->vap_grant) * sta_cfg->sta_grant
					+ ATF_GRANT_SCALE / 2) / ATF_GRANT_SCALE;
			if (station_grant == 0)
				station_grant = 1; /* Ensure a positive value */

			wpa_printf(MSG_DEBUG, "ATF: STA %s/" MACSTR " (id = %d) gets grant %d",
					vap_cfg->vap_name, MAC2STR(sta->addr), sta_id_in_driver(sta), station_grant);

			sta_grant[sta_id_in_driver(sta)] = station_grant;
			remaining_in_vap -= (int)station_grant;
		}
	}

	/* Split the remaining VAP grant equally between stations in this VAP */
	if (remaining_in_vap > 0 && count_in_bss) {
		uint16_t remaining_per_sta = remaining_in_vap / count_in_bss;
		struct sta_info *sta;

		wpa_printf(MSG_DEBUG, "ATF: Distributing remaining grant equally. Each of %d stations gets %d.",
				count_in_bss, remaining_per_sta);

		for (sta = bss->sta_list; sta != NULL; sta = sta->next) {
			if (is_sta_active(sta, sta_has_quota))
				sta_grant[sta_id_in_driver(sta)] += remaining_per_sta;
		}
	}
}

/* Calculate quotas and format a message to be sent to Driver */
mtlk_atf_quotas_t* hostapd_atf_calc_quotas(struct atf_config* atf_cfg,
		struct hostapd_data *hapd, /* BSS the changed station belongs to */
		const u8* changed_sta, /* station that changes its status */
		int in_driver, int not_in_driver)
{
	uint16_t *sta_grant, *vap_grant;  /* variable addresses in mtlk_atf_quotas_t */
	int n_bss, data_len;
	mtlk_atf_quotas_t* atf_quotas;
	int max_stations = hapd->iconf->ap_max_num_sta;

	/* Check if quotas need to be recalculated. */
	if (changed_sta != NULL) { /* STA state changed */
		struct sta_info *sta = ap_get_sta(hapd, changed_sta);
		if (sta == NULL || sta->aid > max_stations)
			return NULL; /* The STA isn't (yet) under ATF control */

		if (!update_atf_active_status_for_station(sta, in_driver, not_in_driver,
				hapd->iface->atf_sta_in_driver, hapd->iface->atf_sta_has_quota))
			return NULL; /* 'active' state for station did not change */

		if (!hapd->iface->atf_enabled && !hapd->iconf->atf_cfg.distr_type)
			return NULL; /* ATF function is OFF */

		wpa_printf(MSG_DEBUG, "ATF: Recalculating quotas because STA " MACSTR
				" (aid %d) became %s", MAC2STR(changed_sta), sta->aid,
				is_sta_active(sta, hapd->iface->atf_sta_has_quota) ? "active" : "inactive");
	}
	else { /* ATF config changed */
		if (!hapd->iface->atf_enabled && !hapd->iconf->atf_cfg.distr_type)
			return NULL; /* ATF function is OFF and also was OFF before */
		hapd->iface->atf_enabled = hapd->iconf->atf_cfg.distr_type;

		wpa_printf(MSG_DEBUG, "ATF: Recalculating quotas because config has "
				"changed, num_bss=%d, max_sta=%d",
				hapd->iface->num_bss, max_stations);
	}

	/* Calculate extra data size and allocate memory for "SET_ATF_QUOTAS" message */
	data_len = sizeof(uint16_t) * (max_stations + hapd->iface->num_bss);
	atf_quotas = os_zalloc (sizeof(mtlk_atf_quotas_t) + data_len);
	if (atf_quotas == NULL)
		return NULL;
	vap_grant = atf_quotas->data;
	sta_grant = vap_grant + hapd->iface->num_bss;

	/* Fill in per-radio parameters */
	atf_quotas->debug         = atf_cfg->debug;
	atf_quotas->distr_type    = atf_cfg->distr_type;
	atf_quotas->algo_type     = atf_cfg->algo_type;
	atf_quotas->weighted_type = atf_cfg->weighted_type;
	atf_quotas->interval      = atf_cfg->interval;
	atf_quotas->free_time     = atf_cfg->free_time;
	atf_quotas->nof_bss       = hapd->iface->num_bss;
	atf_quotas->nof_sta       = max_stations;
	atf_quotas->data_len      = data_len;

	if (atf_cfg->per_vap)
	{
		for (n_bss = 0; n_bss < hapd->iface->num_bss; n_bss++) {
			int i;

			/* Find and apply ATF configuration for this BSS */
			for (i=0; i < atf_cfg->n_vaps; i++) {
				struct atf_vap_config *vap_cfg = atf_cfg->vap_cfg + i;
				struct hostapd_data *bss = hapd->iface->bss[n_bss];
				if (vap_cfg->vap_name && strcmp(bss->conf->iface, vap_cfg->vap_name) == 0) {

					wpa_printf(MSG_DEBUG, "ATF: Calculating quotas for %s (%d stations in DB)",
							bss->conf->iface, bss->num_sta);

					vap_grant[n_bss] = vap_cfg->vap_grant;
					distribute_sta_quotas_per_vap(vap_cfg, bss, sta_grant,
							hapd->iface->atf_sta_has_quota);
					break;
				}
			}
		}
	}
	else { /* per radio */

		wpa_printf(MSG_DEBUG, "ATF: Calculating quotas per radio");
		distribute_sta_quotas_per_radio(atf_cfg->vap_cfg,
				hapd->iface->num_bss, hapd->iface->bss,
				sta_grant, hapd->iface->atf_sta_has_quota);
	}

	return atf_quotas;
}


/* Does ATF configuration and capacity allow the given station to connect */
int hostapd_atf_is_sta_allowed(struct hostapd_data *bss, const u8 *address)
{
	struct atf_config* atf_cfg = &bss->iconf->atf_cfg;
	struct atf_vap_config* vap_cfg = NULL;
	int n_sta, total_quotas = 0;

	/* Find ATF configuration to use */
	if (atf_cfg->per_vap) {
		int i;

		/* Find ATF configuration for this BSS */
		for (i=0; i < atf_cfg->n_vaps; i++) {
			struct atf_vap_config *cfg = atf_cfg->vap_cfg + i;
			if (cfg->vap_name && strcmp(bss->conf->iface, cfg->vap_name) == 0) {
				vap_cfg = cfg;
				break;
			}
		}
	}
	else
		vap_cfg = bss->iconf->atf_cfg.vap_cfg; /* use per-radio configuration */

	if (vap_cfg == NULL)
		return 1; /* No stations nor VAPS (or an invalid ATF config) */

	/* Is this STA in the list? Is the sum of assigned quotas below 100%? */
	for (n_sta = 0; n_sta < vap_cfg->n_stations; n_sta++) {
		struct atf_sta_grant* sta_grant = vap_cfg->sta_grants + n_sta;
		if (memcmp(sta_grant->sta_mac, address, ETH_ALEN) == 0)
			return 1; /* listed stations are always allowed */
		total_quotas += sta_grant->sta_grant;
	}

	/* Unlisted stations are allowed if the total configured quota is below 100% */
	return total_quotas < ATF_GRANT_SCALE;
}
