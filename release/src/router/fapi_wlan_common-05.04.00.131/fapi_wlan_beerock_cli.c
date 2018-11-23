/*  ***************************************************************************** 
 *         File Name    : fapi_wlan_beerock_cli.c                             	*
 *         Description  : test utility to test FAPI wlan control interface 		* 
 *                                                                              *
 *  *****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/select.h>
#include "fapi_wlan_beerock.h"
#include "fapi_wlan_tools.h"
#include "fapi_wlan_beerock_aml.h"
#include "fapi_wlan_band_steering.h"
#include "wpa_ctrl.h"


extern int numOfActiveApInterfaces;

typedef struct
{
	char *name;
	char operationMode[8];
	char wpaCtrlName[32];
	struct wpa_ctrl *wpaSocket;
} RadioInterface;


static RadioInterface radioInterface[] = { { "wlan0", "NONE", "NONE", NULL },
                                           { "wlan1", "NONE", "NONE", NULL },
                                           { "wlan2", "NONE", "NONE", NULL },
                                           { "wlan3", "NONE", "NONE", NULL },
                                           { "wlan4", "NONE", "NONE", NULL },
                                           { "wlan5", "NONE", "NONE", NULL } };
static bool    isCliRunning = true;
static bool    _isOneShot = false;

/* Supported commands */
static const char* s_arrFAPICommands[][2] =
{
    { "help",                                 "CLI help"                               },
    { "exit",                                 "Exit the CLI"                           },
    { "quit",                                 "Exit the CLI"                           },
	{ "GENERAL_COMMAND",                      "Send the hostapd command"               },
	{ "INTERFACE_INFO_GET",                   "Get the i/f info - AP/STA, 2.4/5"       },
	{ "STA_STEER",                            "Steer Station"                          },
	{ "BSS_TRANS_MANAG_REQ",                  "Request BSS transition management"      },
	{ "STA_ALLOW",                            "Allow Station"                          },
	{ "STA_DENY",                             "Deny Station"                           },
	{ "STA_DISASSOCIATE",                     "Disassociate Station"                   },
	{ "STA_DEAUTHENTICATE",                   "Deauthenticate Station"                 },
	{ "CHANNEL_SWITCH",                       "Channel switch"                         },
	{ "UNASSOC_DEVICE_INFO_REQ",              "Request unassociated device info"       },
	{ "RESTRICTED_CHANNELS_SET",              "Set restricted channel"                 },
	{ "FAILSAFE_CHANNEL_SET",                 "Set failsafe channel"                   },
	{ "MBO_STA_ALLOW",                        "Allow MBO Station"                      },
	{ "MBO_STA_DENY",                         "Deny MBO Station"                       },
	{ "NEIGHBOR_SET",                         "Set neighbor"                           },
	{ "NEIGHBOR_REMOVE",                      "Remove neighbor"                        },
	{ "CELLULAR_PREF_SET",                    "Set cellular preference"                },
	{ "CELLULAR_AWARE_SET",                   "Set cellular aware"                     },
	{ "CHANNEL_LOAD_REQ",                     "Request channel load"                   },
	{ "CHANNEL_LOAD_RESP",                    "Channel load response"                  },
	{ "BEACON_REQ",                           "Request beacon"                         },
	{ "BEACON_REPORT_RESP",                   "Beacon report response"                 },
	{ "SELF_BEACON_REPORT_REQ",               "Request self beacon report"             },
	{ "STA_STATISTICS_REQ",                   "Request Station statistics"             },
	{ "STA_STATISTICS_RESP",                  "Station statistics response"            },
	{ "STA_LINK_MEASUREMENT_REQ",             "Request Station link measurement"       },
	{ "LINK_MEASUREMENT_RESP",                "Link measurement response"              },
	{ "NOISE_HISTOGRAM_REQ",                  "Request noise histogram"                },
	{ "NOISE_HISTOGRAM_RESP",                 "Noise histogram response"               },
	{ "RADIO_STATE_GET",                      "Get the radio state web-ui setting"     },
	{ "MAC_ADDRESS_GET",                      "Get the BSSID mac address"              },
	{ "CTRL_A_FILTER_BANK_SET",               "Set the radio's a filter bank"          },
	{ "CTRL_A_FILTER_BANK_GET",               "Get the radio's a filter bank"          },
	{ "STA_MEASUREMENT_GET",                  "Get Station measurement"                },
	{ "VAP_MEASUREMENT_GET",                  "Get VAP measurement"                    },
	{ "RADIO_INFO_GET",                       "Get radio info"                         },
	{ "ACS_REPORT_GET",                       "Get ACS report"                         },
	{ "FAILSAFE_CHANNEL_GET",                 "Get failsafe channel"                   },
	{ "RESTRICTED_CHANNEL_GET",               "Get restricted channel"                 },
	{ "BEACON_REPORT_GET",                    "Get beacon report"                      },
	{ "CONNECTION_STATUS_GET",                "Get the connection status"              },
	{ "METRIC_GET",                           "Get some metrics"                       },
	{ "OCE_WAN_METRICS_SET",                  "Set OCE WAN metrics"                    },
	{ "OCE_ASSOC_REJECT_SET",                 "Set OCE associated reject"              },
	{ "CLIENT_MODE_NETWORK_ADD",              "Add Network - client mode"              },
	{ "CLIENT_MODE_NETWORK_SET",              "Set Network - client mode"              },
	{ "CLIENT_MODE_NETWORK_ENABLE",           "Enable Network - client mode"           },
	{ "CLIENT_MODE_NETWORK_SELECT",           "Select Network - client mode"           },
	{ "CLIENT_MODE_NETWORK_DISCONNECT",       "Disconnect Network - client mode"       },
	{ "CLIENT_MODE_NETWORK_REMOVE",           "Remove Network - client mode"           },
	{ "CLIENT_MODE_SCAN_START",               "Start scan - client mode"               },
	{ "CLIENT_MODE_SCAN_RESULTS_GET",         "Scan Results Get - client mode"         },
	{ "CLIENT_MODE_CONNECTION_STATUS_GET",    "Get Connection Status - client mode"    },
	{ "CLIENT_MODE_DATA_ROUTING_MODE_SET",    "Set Data Routing Mode - client mode"    },
	{ "CLIENT_MODE_DATA_ROUTING_MODE_GET",    "Get Data Routing Mode - client mode"    },
	{ "CLIENT_MODE_RADIO_INFO_GET",           "Get Radio Info - client mode"           },

	/* Must be at the end */
    { "__END__",                           "MUST BE THE LAST ENTRY!"                   }
};

extern ObjList *mapperObjPtr;


static void radioInterfaceDataBaseUpdate(void)
{
	size_t  numOfInterfaces = sizeof(radioInterface) / sizeof(RadioInterface);
	int     i;
	char    wpaCtrlName[32];

	for (i=0; i < (int)numOfInterfaces; i++)
	{
		/* check if '/var/run/hostapd/wlanX' or '/var/run/wpa_supplicant/wlanX' exists */
		sprintf(wpaCtrlName, "%s%s", "/var/run/hostapd/", radioInterface[i].name);
		if (access(wpaCtrlName, F_OK) == 0)
		{
			printf("%s; Radio '%s' exists - AP Mode\n", __FUNCTION__, radioInterface[i].name);
			strcpy(radioInterface[i].operationMode, "AP");
			strcpy(radioInterface[i].wpaCtrlName, wpaCtrlName);
			numOfActiveApInterfaces++;
		}
		else
		{
			sprintf(wpaCtrlName, "%s%s", "/var/run/wpa_supplicant/", radioInterface[i].name);
			if (access(wpaCtrlName, F_OK) == 0)
			{
				printf("%s; Radio '%s' exists - STA Mode\n", __FUNCTION__, radioInterface[i].name);
				strcpy(radioInterface[i].operationMode, "STA");
				strcpy(radioInterface[i].wpaCtrlName, wpaCtrlName);
			}
		}
	}
}


static int interfaceIndexGet(char *ifname)
{
	int     i;
	size_t  numOfInterfaces = sizeof(radioInterface) / sizeof(RadioInterface);

	for (i=0; i < (int)numOfInterfaces; i++)
	{
		if (!strncmp(ifname, radioInterface[i].name, 5))
			return i;
	}

	return -1;
}


static int fapiWlanCallBackLocalFunc(char *opCode, const char *ifname, ObjList *wlObj, unsigned int flags, void *context)
{
	(void)ifname;
	(void)flags;
	(void)wlObj;
	(void)context;

	//printf("%s Entry; opCode= '%s', wlObj= 0x%x\n", __FUNCTION__, opCode, (unsigned int)wlObj);

#if 0
	if (wlObj != NULL)
	{  /* NULL is a possible value - in that case, skip the trace! */
		setLog(ifname, wlObj, 0);
	}
#endif

	/* After hostapd reset, reset the AP (without reset the hostapd) to prevent any 'unregistered' AP Manager activities */
	if (!strcmp(opCode, "WLAN-DISCONNECTED"))
	{
		printf("%s; 'WLAN-DISCONNECTED' received\n", __FUNCTION__);
		// here AP MAnager needs to reset its data-base, etc.
	}

#if 0  // the below part is causing the Add VAP not to work - configurations after hostapd reconnect are being corrupted!
	/* After hostapd reset, reset the AP (without reset the hostapd) to prevent any 'unregistered' AP Manager activities */
	if (!strcmp(opCode, "WLAN-RECONNECTED"))
	{
		fapi_wlan_iface_disable(ifname, NULL, 0);
		fapi_wlan_iface_enable(ifname, NULL, 0);
	}
#endif

	return UGW_SUCCESS;
}


static int ap_manager_init(char *ifname)
{
	ObjList *wlObjtmp = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	printf("%s Entry; ifname= '%s'\n", __FUNCTION__, ifname);

	if (wlObjtmp == NULL)
	{
		return UGW_FAILURE;
	}

	/* create object for enabling the radio back */
	help_addObjList(wlObjtmp, "Device.WiFi.Radio", 0, 0, 0, 0);
	HELP_EDIT_NODE(wlObjtmp, "Device.WiFi.Radio", "Name", ifname, 0, 0);
	HELP_EDIT_NODE(wlObjtmp, "Device.WiFi.Radio", "Enable", "true", 0, 0);

	/* register for all notifications */
	if (fapi_wlan_hostapd_callback_register(ifname, fapiWlanCallBackLocalFunc, NULL) == UGW_FAILURE)
		printf("%s; fapi_wlan_hostapd_callback_register ERROR (ifname= '%s')\n", __FUNCTION__, ifname);

	HELP_DELETE_OBJ(wlObjtmp, SOPT_OBJVALUE, FREE_OBJLIST);

	return UGW_SUCCESS;
}


static char *fapi_beerock_cli_tab_completion_entry(const char *text , int start)
{
	(void)text;
	(void)start;

	return (NULL);
}


static void sigterm_handler(void)
{
    isCliRunning = false;
    fclose(stdin);
    printf("\n");
}


static void sigint_handler(void)
{
    isCliRunning = false;
    fclose(stdin);
    printf("\n");
}


static void init_signals(void)
{
    struct sigaction sigterm_action;
    sigterm_action.sa_handler = (__sighandler_t)sigterm_handler;
    sigemptyset(&sigterm_action.sa_mask);
    sigterm_action.sa_flags = 0;
    sigaction(SIGTERM, &sigterm_action, NULL);

    struct sigaction sigint_action;
    sigint_action.sa_handler = (__sighandler_t)sigint_handler;
    sigemptyset(&sigint_action.sa_mask);
    sigint_action.sa_flags = 0;
    sigaction(SIGINT, &sigint_action, NULL);
}


static char *dupstr (const char *s)
{
	char *r;

	if (!(r = (char *)malloc((size_t)(strlen (s) + 1))))
	{
		printf("Error: Out of memory. Exiting\n");
		sigterm_handler();    
	}

	strcpy (r, s);

	return (r);
}


static char *fapi_beerock_cli_generator(const char *text, int state)
{
	static int list_index, len;
	const char *name;

	if (!state)
	{
		list_index = 0;
		len = strlen (text);
	}

	while (strncmp(s_arrFAPICommands[list_index][0], "__END__", 7))
	{
		name = s_arrFAPICommands[list_index++][0];

		if (strncmp(name, text, len) == 0)
			return (dupstr(name));
	}

	return NULL;
}


static char **fapi_beerock_cli_tab_completion(const char *text , int start,  int end)
{
	char **matches = NULL;

	(void)end;

	if (start == 0)
		matches = rl_completion_matches(text, fapi_beerock_cli_generator);
	else
		matches = (char **)NULL;

	return (matches);
}


static void fapi_beerock_cli_show_help(void)
{
	int i = 0;

	printf("Supported commands:\n");

	/* run till the end of the list */
	while (strncmp(s_arrFAPICommands[i][0], "__END__", 7))
	{
		printf("%-26s - %s\n", s_arrFAPICommands[i][0], s_arrFAPICommands[i][1]);
		i++;
	}

	printf("\n");
}


static void fapi_beerock_cli_readline_callback(char *strLine)
{
	int       i, idx, argc = 2;
	char      fields[32][64];
	char      **argv = (char **)fields;
	char      field[64] = "\0";
	char      *tempField = field;
	char      *stringFraction;
	ObjList   *wlObj = HELP_CREATE_OBJ(SOPT_OBJVALUE);
	char      *ifname;
	char      BSSID[]      = "00:0A:1B:0E:04:60";
	char      MACAddress[] = "d8:fe:e3:3e:bd:14";
	char      CenterFrequency[] = "5745";
	char      WaveVhtCenterFrequency[] = "5775";
	char      OperatingChannelBandwidth[] = "80";
	char      ExtensionChannel[] = "AboveControlChannel";
	char      *tx_ant_mask = NULL, *rx_ant_mask = NULL;
	char      isOn[] = "false";
	char      ssid[32] = "\"widan_test\"";
	char      NeighborReport[32] = "00112233445500000000510107";
	char      MboBssAssociationReason[] = "0";
	char      CellPref[] = "255";
	char      MBOCellAware[] = "0";
	char      fieldName[32];

	if (!strcmp(strLine, ""))
		return;

	/* Add the command to the history */
	idx = history_search_pos(strLine, 0, 0);
	if(idx != (-1))
	{
		HIST_ENTRY *entry = remove_history(idx);
		if (entry)
		{
			free (entry->line);
			free (entry);
		}                        
	}
	
	add_history(strLine);

	tempField = strtok(strLine, " ");
	argv[1] = tempField;

	if (argv[1] == NULL)
	{
		//printf("%s; command ('%s') is NULL! ABORT!\n", __FUNCTION__, argv[1]);
		return;
	}

	/* Exit CLI */
    if (!strncmp(argv[1], "exit", 4) || !strncmp(argv[1], "quit", 4))
    {
		isCliRunning = false;
        return;
    }

	/* CLI Help */
    if ((*argv[1] == '?') || !strncmp(argv[1], "help", 4))
    {
        fapi_beerock_cli_show_help();
        return;
    }

	/* Get the number of parameters, and concatenate values between ' ' into one argv field */
	while ((tempField = strtok(NULL, " ")) != NULL)
	{
		if (!strncmp(tempField, "'", 1))
		{
			if (strlen(tempField) > 1)
			{  /* the string is ' followed by a character, for example '3 */
				argv[argc] = &tempField[1];

				if (tempField[strlen(tempField)-1] == '\'')
				{
					tempField[strlen(tempField)-1] = '\0';
				}
				else
				{  /* the last character is NOT ' */
					if ( ((stringFraction = strtok(NULL, "'")) == NULL) &&
						 (tempField[strlen(tempField)-1] != '\'') )
					{
						printf("%s; Non-valid command ==> Abort!\n", __FUNCTION__);
						return;
					}

					sprintf(argv[argc], "%s %s", argv[argc], stringFraction);
				}
			}
			else
			{  /* the string is ' followed by a blank */
				if ((stringFraction = strtok(NULL, "'")) == NULL)
				{
					printf("%s; Non-valid command ==> Abort!\n", __FUNCTION__);
					return;
				}

				/* Remove the blanks at the beginning of the string after the " character */
				for (idx=0; stringFraction[idx] == ' '; idx++);  /* Get the index of the 1st non-blank character */
				argv[argc] = &stringFraction[idx];
			}

			/* Remove the blanks at the end of the string before the ' character */
			for (idx = strlen(argv[argc]) - 1; argv[argc][idx] == ' '; idx--)
			{
				argv[argc][idx] = '\0';
			}
		}
		else
		{
			argv[argc] = tempField;
		}

		argc++;
	}

	/* Get the interface name */
	if (argc > 2)
	{
		ifname = argv[2];
	}
	else
	{
		printf("%s; non-valid ifname ==> exit\n", __FUNCTION__);
		return;
	}

/**********************/
/* 0) AP Manager Lite */
/**********************/
	if (!strncmp(argv[1], "AML_", 4))
	{
		beerock_aml_cli(ifname, argv, argc);
	}

/*****************/
/* 1.2) Controls */
/*****************/
	else if (!strcmp(argv[1], "GENERAL_COMMAND"))
	{
		char *report = (char *)malloc((size_t)(4096 * sizeof(char)));
		/*
		Format: GENERAL_COMMAND ifname 'command'

		Examples:
		GENERAL_COMMAND wlan0 'ENABLE'
		GENERAL_COMMAND wlan0 'GET_ACS_REPORT'
		GENERAL_COMMAND wlan1 'STA_STEER 8c:3a:e3:45:d1:ae 00:0A:1B:0E:04:70'
		GENERAL_COMMAND wlan0 'REQ_BEACON 44:85:00:C5:6A:1B 0 0 0 255 1000 50 passive 00:0A:1B:0E:04:60 beacon_rep=1,123 rep_detail=2 ssid="AA BB" ap_ch_report=1,2,3 req_elements=0,51 wide_band_ch_switch=1,1,1'
		GENERAL_COMMAND wlan0 'REQ_SELF_BEACON 1000 50 passive'
		GENERAL_COMMAND wlan0 'RESTRICTED_CHANNELS 1 6 11-13'
		*/

		printf("fapi_wlan_general_command_set test\n");

		if (argc <= 3)
		{
			printf("fapi_wlan_general_command_set - wrong format\n");
			free((void *)report);
		}
		else if (report == NULL)
		{
			printf("fapi_wlan_general_command_set - malloc error\n");
		}
		else
		{
			printf("fapi_wlan_general_command_set test; ifname= '%s', Command= '%s'\n", ifname, argv[3]);

			if (fapi_wlan_general_command_set(ifname, argv[3], report) == UGW_FAILURE)
				printf("fapi_wlan_general_command_set ERROR\n");
			else
			{
				printf("fapi_wlan_general_command_set - output report (if exists):\n%s\n", report);
				setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
			}

			free((void *)report);
		}
	}
	else if (!strcmp(argv[1], "INTERFACE_INFO_GET"))
	{
		/*
		Format:  INTERFACE_INFO_GET ifname
		Example: INTERFACE_INFO_GET wlan0
		*/

		printf("fapi_wlan_interface_info_get test; ifname= '%s'\n", ifname);

		if (fapi_wlan_interface_info_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_interface_info_get ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "STA_STEER"))
	{
		/* Check the connected stations:
		 iw wlan0 station dump OR iw wlan1 station dump
		 iwlist wlan0 peers OR iwlist wlan1 peers

		Format: STA_STEER ifname MACAddress BSSID-Dest.

		Examples:
		band steering, wlan0 -> wlan1
		STA_STEER wlan1 8c:3a:e3:45:d1:ae 00:0A:1B:0E:04:70
		STA_STEER wlan0 8c:3a:e3:45:d1:ae 00:0A:1B:0E:04:70

		band steering, wlan1 -> wlan0
		STA_STEER wlan0 8c:3a:e3:45:d1:ae 00:0A:1B:0E:04:60
		STA_STEER wlan1 8c:3a:e3:45:d1:ae 00:0A:1B:0E:04:60

		station steering (AKA mobility): moving station from wlan0 to wlan0.1 (wlan0.1 BSSID = 00:0A:1B:0E:04:61)
		STA_STEER wlan0 8c:3a:e3:45:d1:ae 00:0A:1B:0E:04:61
		
		band steering, wlan0 -> wlan1
		STA_STEER wlan0 44:85:00:C5:6A:1B 00:0A:1B:0E:04:70 mode=2 bss_termination=12345 url=www.google.com pref=1 abridged=1 disassoc_imminent=1 neighbor=00:0A:1B:0E:04:70,0,32,1,7,1 disassoc_timer=20 mbo=4:0:-1 
		*/

		printf("fapi_wlan_sta_steer test\n");

		if (argc > 3)
		{
			strcpy(MACAddress, argv[3]);
		}

		if (argc > 4)
		{
			strcpy(BSSID, argv[4]);
		}

		printf("fapi_wlan_sta_steer test; ifname= '%s', MACAddress= '%s', BSSID= '%s'\n", ifname, MACAddress, BSSID);

		help_addObjList(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress", MACAddress, 0, 0);  // Station's MAC Address

		help_addObjList(wlObj, "Device.WiFi.SSID", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID", "BSSID", BSSID, 0, 0);  // Dest AP BSSID

		/* Optional Params */
		help_addObjList(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		printf("Optional Params: ");
		for (i=5; i < argc; i++)
		{
			if (!strncmp(argv[i], "mode=", strlen("mode=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "mode", argv[i]+strlen("mode="), 0, 0);
				printf("mode= '%s'; ", argv[i]+strlen("mode="));
			}
			else if (!strncmp(argv[i], "bss_termination=", strlen("bss_termination=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "bss_termination", argv[i]+strlen("bss_termination="), 0, 0);
				printf("bss_termination= '%s'; ", argv[i]+strlen("bss_termination="));
			}
			else if (!strncmp(argv[i], "url=", strlen("url=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "url", argv[i]+strlen("url="), 0, 0);
				printf("url= '%s'; ", argv[i]+strlen("url="));
			}
			else if (!strncmp(argv[i], "pref=", strlen("pref=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "pref", argv[i]+strlen("pref="), 0, 0);
				printf("pref= '%s'; ", argv[i]+strlen("pref="));
			}
			else if (!strncmp(argv[i], "abridged=", strlen("abridged=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "abridged", argv[i]+strlen("abridged="), 0, 0);
				printf("abridged= '%s'; ", argv[i]+strlen("abridged="));
			}
			else if (!strncmp(argv[i], "disassoc_imminent=", strlen("disassoc_imminent=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "disassoc_imminent", argv[i]+strlen("disassoc_imminent="), 0, 0);
				printf("disassoc_imminent= '%s'; ", argv[i]+strlen("disassoc_imminent="));
			}
			else if (!strncmp(argv[i], "neighbor=", strlen("neighbor=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "neighbor", argv[i]+strlen("neighbor="), 0, 0);
				printf("neighbor= '%s'; ", argv[i]+strlen("neighbor="));
			}
			else if (!strncmp(argv[i], "disassoc_timer=", strlen("disassoc_timer=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "disassoc_timer", argv[i]+strlen("disassoc_timer="), 0, 0);
				printf("disassoc_timer= '%s'; ", argv[i]+strlen("disassoc_timer="));
			}
			else if (!strncmp(argv[i], "mbo=", strlen("mbo=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "mbo", argv[i]+strlen("mbo="), 0, 0);
				printf("mbo= '%s'; ", argv[i]+strlen("mbo="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_sta_steer(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_sta_steer ERROR\n");
	}
	else if (!strcmp(argv[1], "BSS_TRANS_MANAG_REQ"))
	{
		/*
		Format: BSS_TRANS_MANAG_REQ ifname MACAddress [dialog_token=<>] [Mode=<>] [bss_termination=<>]
				[url=<>] [pref=<>] [abridged=<>] [disassoc_imminent=<>] [neighbor=<> neighbor=<> neighbor=<> ...] [disassoc_timer=<>] [mbo=<>] [valid_int=<>]

		Example:
		BSS_TRANS_MANAG_REQ wlan0 8c:3a:e3:45:d1:ae dialog_token=10 Mode=1 bss_termination=2,3 url=www.google.com pref=1 abridged=1 disassoc_imminent=1 neighbor=00:0A:1B:0E:04:70,1,2,3,4,5 disassoc_timer=1 mbo=1:2:3 valid_int=15
		*/

		printf("fapi_wlan_bss_transition_management_req test\n");

		if (argc > 3)
		{
			strcpy(MACAddress, argv[3]);
		}

		printf("fapi_wlan_bss_transition_management_req test; ifname= '%s', MACAddress= '%s'\n", ifname, MACAddress);

		help_addObjList(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress", MACAddress, 0, 0);  // Station's MAC Address

		if (argc > 4)
		{  /* this object is needed ONLY if there is at least one optional parameter */
			help_addObjList(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		}

		/* Optional Params */
		idx = 0;
		printf("Optional Params: ");
		for (i=4; i < argc; i++)
		{
			if (!strncmp(argv[i], "dialog_token=", strlen("dialog_token=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "dialog_token", argv[i]+strlen("dialog_token="), 0, 0);
				printf("dialog_token= '%s'; ", argv[i]+strlen("dialog_token="));
			}
			else if (!strncmp(argv[i], "Mode=", strlen("Mode=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "Mode", argv[i]+strlen("Mode="), 0, 0);
				printf("Mode= '%s'; ", argv[i]+strlen("Mode="));
			}
			else if (!strncmp(argv[i], "bss_termination=", strlen("bss_termination=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "bss_termination", argv[i]+strlen("bss_termination="), 0, 0);
				printf("bss_termination= '%s'; ", argv[i]+strlen("bss_termination="));
			}
			else if (!strncmp(argv[i], "url=", strlen("url=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "Url", argv[i]+strlen("url="), 0, 0);
				printf("url= '%s'; ", argv[i]+strlen("url="));
			}
			else if (!strncmp(argv[i], "pref=", strlen("pref=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "pref", argv[i]+strlen("pref="), 0, 0);
				printf("pref= '%s'; ", argv[i]+strlen("pref="));
			}
			else if (!strncmp(argv[i], "abridged=", strlen("abridged=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "abridged", argv[i]+strlen("abridged="), 0, 0);
				printf("abridged= '%s'; ", argv[i]+strlen("abridged="));
			}
			else if (!strncmp(argv[i], "disassoc_imminent=", strlen("disassoc_imminent=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "disassoc_imminent", argv[i]+strlen("disassoc_imminent="), 0, 0);
				printf("disassoc_imminent= '%s'; ", argv[i]+strlen("disassoc_imminent="));
			}
			else if (!strncmp(argv[i], "neighbor=", strlen("neighbor=")))
			{
				sprintf(fieldName, "%d_neighbor", idx++);  /* Add "index_" as a prefix */
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", fieldName, argv[i]+strlen("neighbor="), 0, 0);
				printf("%s= '%s'; ", fieldName, argv[i]+strlen("neighbor="));
			}
			else if (!strncmp(argv[i], "disassoc_timer=", strlen("disassoc_timer=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "disassoc_timer", argv[i]+strlen("disassoc_timer="), 0, 0);
				printf("disassoc_timer= '%s'; ", argv[i]+strlen("disassoc_timer="));
			}
			else if (!strncmp(argv[i], "mbo=", strlen("mbo=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "mbo", argv[i]+strlen("mbo="), 0, 0);
				printf("mbo= '%s'; ", argv[i]+strlen("mbo="));
			}
			else if (!strncmp(argv[i], "valid_int=", strlen("valid_int=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "valid_int", argv[i]+strlen("valid_int="), 0, 0);
				printf("valid_int= '%s'; ", argv[i]+strlen("valid_int="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_bss_transition_management_req(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_bss_transition_management_req ERROR\n");
	}
	else if (!strcmp(argv[1], "STA_ALLOW"))
	{
		/*
		Format:  STA_ALLOW ifname MACAddress
		Example: STA_ALLOW wlan0 d8:fe:e3:3e:bd:14
				 for clearing the deny list:
				 STA_ALLOW wlan0 or STA_ALLOW wlan1
		*/

		if (argc <= 3)
		{
			printf("fapi_wlan_sta_allow test; ifname= '%s'\n", ifname);

			if (fapi_wlan_sta_allow(ifname, NULL, 0) == UGW_FAILURE)
				printf("fapi_wlan_sta_allow ERROR\n");
		}

		if (argc > 3)
		{
			strcpy(MACAddress, argv[3]);

			printf("fapi_wlan_sta_allow test; ifname= '%s', MACAddressControlList= '%s'\n", ifname, MACAddress);

			help_addObjList(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddressControlList", MACAddress, 0, 0);

			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

			if (fapi_wlan_sta_allow(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_sta_allow ERROR\n");
		}
	}
	else if (!strcmp(argv[1], "STA_DENY"))
	{
		/*
		Format:  STA_DENY ifname MACAddress
		Example: STA_DENY wlan0 d8:fe:e3:3e:bd:14
		*/

		if (argc > 3)
		{
			strcpy(MACAddress, argv[3]);
		}

		printf("fapi_wlan_sta_deny test; ifname= '%s', MACAddressControlList= '%s'\n", ifname, MACAddress);

		help_addObjList(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddressControlList", MACAddress, 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_sta_deny(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_sta_deny ERROR\n");
	}
	else if (!strcmp(argv[1], "STA_DISASSOCIATE"))
	{
		/*
		Format:  STA_DISASSOCIATE ifname MACAddress
		Example: STA_DISASSOCIATE wlan0 d8:fe:e3:3e:bd:14
		*/

		if (argc > 3)
		{
			strcpy(MACAddress, argv[3]);
		}

		printf("fapi_wlan_sta_disassociate test; ifname= '%s', MACAddress= '%s'\n", ifname, MACAddress);

		help_addObjList(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddress", MACAddress, 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_sta_disassociate(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_sta_disassociate ERROR\n");
	}
	else if (!strcmp(argv[1], "STA_DEAUTHENTICATE"))
	{
		/*
		Format:  STA_DEAUTHENTICATE ifname MACAddress
		Example: STA_DEAUTHENTICATE wlan0 d8:fe:e3:3e:bd:14
		*/

		if (argc > 3)
		{
			strcpy(MACAddress, argv[3]);
		}

		printf("fapi_wlan_sta_deauthenticate test; ifname= '%s', MACAddress= '%s'\n", ifname, MACAddress);

		help_addObjList(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MACAddress", MACAddress, 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_sta_deauthenticate(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_sta_deauthenticate ERROR\n");
	}
	else if (!strcmp(argv[1], "CHANNEL_SWITCH"))
	{
		bool is_2_4_band = false;

		/* Check the actual channel:
		cat /proc/net/mtlk/wlan0/channel OR cat /proc/net/mtlk/wlan1/channel
		iwlist wlan0 channel OR iwlist wlan1 channel

		Format:   CHANNEL_SWITCH ifname CenterFrequency WaveVhtCenterFrequency OperatingChannelBandwidth [tx_ant_mask=<> rx_ant_mask=<>] [switch_type=<>]
		Examples: set channel 149: CHANNEL_SWITCH wlan1 5745 5775 80 AboveControlChannel
				  set channel 36:  CHANNEL_SWITCH wlan1 5180 5210 80 AboveControlChannel
				  set channel 161: CHANNEL_SWITCH wlan1 5805 5775 80 BelowControlChannel tx_ant_mask=2 rx_ant_mask=2 switch_type=normal
				  set ACS:         CHANNEL_SWITCH wlan1 0
				  set channel 6:   CHANNEL_SWITCH wlan0 2437 20
				  set channel 3:   CHANNEL_SWITCH wlan0 2422 40 AboveControlChannel
				  set channel 11:  CHANNEL_SWITCH wlan0 2422 40 BelowControlChannel
				  set ACS:         CHANNEL_SWITCH wlan0 0
				  Note: switch_type can be normal or scan
		*/

		if (argc > 3)
		{
			strcpy(CenterFrequency, argv[3]);

			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "CenterFrequency", CenterFrequency, 0, 0);

			if (atoi(CenterFrequency) < 5035)
				is_2_4_band = true;
		}

		if (argc > 4)
		{
			if (is_2_4_band)
				strcpy(OperatingChannelBandwidth, argv[4]);
			else
				strcpy(WaveVhtCenterFrequency, argv[4]);
		}

		if (argc > 5)
		{
			if (is_2_4_band)
				strcpy(ExtensionChannel, argv[5]);
			else
				strcpy(OperatingChannelBandwidth, argv[5]);  // 20/40/80/auto
		}

		if (argc > 7)
		{  /* must have parameter #6 and #7 both present */
			if (is_2_4_band)
			{
				printf("Optional Params: ");
				for (i=6; i < argc; i++)
				{
					if (!strncmp(argv[i], "tx_ant_mask=", strlen("tx_ant_mask=")))
					{
						tx_ant_mask = argv[i]+strlen("tx_ant_mask=");
						printf("tx_ant_mask= '%s'; ", tx_ant_mask);
					}
					else if (!strncmp(argv[i], "rx_ant_mask=", strlen("rx_ant_mask=")))
					{
						rx_ant_mask = argv[i]+strlen("rx_ant_mask=");
						printf("rx_ant_mask= '%s'; ", rx_ant_mask);
					}
					else if (!strncmp(argv[i], "switch_type=", strlen("switch_type=")))
					{
						HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "switch_type", argv[i]+strlen("switch_type="), 0, 0);
						printf("switch_type= '%s'; ", argv[i]+strlen("switch_type="));
					}
					printf("\n");
				}
			}
			else
				strcpy(ExtensionChannel, argv[6]);
		}

		if (strcmp(OperatingChannelBandwidth, "20"))
		{  /* Not relevant for 20 MHz */
			help_addObjList(wlObj, "Device.WiFi.Radio", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio", "ExtensionChannel", ExtensionChannel, 0, 0);
		}

		if (is_2_4_band)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio", "SupportedStandards", "b, g, n", 0, 0);
		}
		else
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio", "SupportedStandards", "a, n, ac", 0, 0);
		}

		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio", "OperatingChannelBandwidth", OperatingChannelBandwidth, 0, 0);

		if (is_2_4_band == false)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "WaveVhtCenterFrequency", WaveVhtCenterFrequency, 0, 0);
		}

		printf("fapi_wlan_channel_switch test; ifname= '%s', CenterFrequency= '%s', WaveVhtCenterFrequency= '%s', OperatingChannelBandwidth= '%s', ExtensionChannel= '%s'\n",
			   ifname, CenterFrequency, WaveVhtCenterFrequency, OperatingChannelBandwidth, ExtensionChannel);

		/* Optional Params */
		if (argc > 8)
		{  /* must have parameter #7 and #8 both present */
			printf("Optional Params: ");
			for (i=7; i < argc; i++)
			{
				if (!strncmp(argv[i], "tx_ant_mask=", strlen("tx_ant_mask=")))
				{
					tx_ant_mask = argv[i]+strlen("tx_ant_mask=");
					printf("tx_ant_mask= '%s'; ", tx_ant_mask);
				}
				else if (!strncmp(argv[i], "rx_ant_mask=", strlen("rx_ant_mask=")))
				{
					rx_ant_mask = argv[i]+strlen("rx_ant_mask=");
					printf("rx_ant_mask= '%s'; ", rx_ant_mask);
				}
				else if (!strncmp(argv[i], "switch_type=", strlen("switch_type=")))
				{
					HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "switch_type", argv[i]+strlen("switch_type="), 0, 0);
					printf("switch_type= '%s'; ", argv[i]+strlen("switch_type="));
				}
				printf("\n");
			}
		}

		if ( (tx_ant_mask != NULL) && (rx_ant_mask != NULL) )
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "tx_ant_mask", tx_ant_mask, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rx_ant_mask", rx_ant_mask, 0, 0);
		}
		else if ( !((tx_ant_mask == NULL) && (rx_ant_mask == NULL)) )
		{
			/* only warn if user set one param but not the other */
			printf("fapi_wlan_channel_switch; only one of tx_ant_mask or rx_ant_mask is set. Ignoring.\n");
		}

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_channel_switch(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_channel_switch ERROR\n");
	}
	else if (!strcmp(argv[1], "UNASSOC_DEVICE_INFO_REQ"))
	{
		/* 
		Format:  UNASSOC_DEVICE_INFO_REQ ifname MACAddress OperatingChannelBandwidth CenterFrequency WaveVhtCenterFrequency
		Example: UNASSOC_DEVICE_INFO_REQ wlan1 d8:fe:e3:3e:bd:14 80 5220 5210
		*/

		if (argc > 3)
		{
			strcpy(MACAddress, argv[3]);
		}

		if (argc > 4)
		{
			strcpy(OperatingChannelBandwidth, argv[4]);
		}

		if (argc > 5)
		{
			strcpy(CenterFrequency, argv[5]);
		}

		if (argc > 6)
		{
			strcpy(WaveVhtCenterFrequency, argv[6]);
		}

		printf("fapi_wlan_unassociated_devices_info_req test; ifname= '%s', MACAddress= '%s', OperatingChannelBandwidth= '%s', CenterFrequency= '%s', WaveVhtCenterFrequency= '%s'\n",
		ifname, MACAddress, OperatingChannelBandwidth, CenterFrequency, WaveVhtCenterFrequency);

		help_addObjList(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress", MACAddress, 0, 0);

		help_addObjList(wlObj, "Device.WiFi.Radio", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio", "OperatingChannelBandwidth", OperatingChannelBandwidth, 0, 0);

		help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "CenterFrequency", CenterFrequency, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "WaveVhtCenterFrequency", WaveVhtCenterFrequency, 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_unassociated_devices_info_req(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_unassociated_devices_info_req ERROR\n");
	}
	else if (!strcmp(argv[1], "RESTRICTED_CHANNELS_SET"))
	{
		/*
		Format:   RESTRICTED_CHANNELS_SET ifname Channel Channel Channel-Channel Channel...
		Examples: Set channels 1, 6, 11, 12, 13:            RESTRICTED_CHANNELS_SET wlan0 '1 6 11-13'
				  Set channels 7:                           RESTRICTED_CHANNELS_SET wlan0 7
				  Remove all channels from restricted list: RESTRICTED_CHANNELS_SET wlan0
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ChannelList", argv[3], 0, 0);

			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

			printf("fapi_wlan_restricted_channels_set test; ifname= '%s', ChannelList= '%s'\n", ifname, argv[3]);
		}
		else
		{
			printf("fapi_wlan_restricted_channels_set test; ifname= '%s'\n", ifname);
		}

		if (fapi_wlan_restricted_channels_set(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_restricted_channels_set ERROR\n");
	}
	else if (!strcmp(argv[1], "FAILSAFE_CHANNEL_SET"))
	{
		/*
		Format:   FAILSAFE_CHANNEL_SET ifname CenterFrequency WaveVhtCenterFrequency OperatingChannelBandwidth [tx_ant_mask=<> rx_ant_mask=<>]
		Examples: FAILSAFE_CHANNEL_SET wlan1 5745 5775 80 tx_ant_mask=3 rx_ant_mask=3
		          FAILSAFE_CHANNEL_SET wlan1 5745 5775 80
				  FAILSAFE_CHANNEL_SET wlan1 0 - Remove all channels from failsafe channel list
		How to test:
				  http://192.168.1.1/#/tableform/wifi5_basic - make sure that "Use DFS Channels" in turned on
				  set channel 140 (must be DFS ch):   CHANNEL_SWITCH wlan1 5700 5700 20
				  Check the channel:                  iwlist wlan1 channel
				  set failsafe channel to 149:        FAILSAFE_CHANNEL_SET wlan1 5745 5775 80
				  get failsafe channel (149):         FAILSAFE_CHANNEL_GET wlan1
				  Simulate radar:                     iwpriv wlan1 s11hEmulatRadar 1
				  Check the channel (should be 149):  iwlist wlan1 channel
		*/

		if (argc > 3)
		{
			strcpy(CenterFrequency, argv[3]);
		}
		else
		{  // Set the channel default value to "0" - low level channel selection algorithm
			strcpy(CenterFrequency, "0");
		}

		if (argc > 4)
		{
			strcpy(WaveVhtCenterFrequency, argv[4]);
		}

		if (argc > 5)
		{
			strcpy(OperatingChannelBandwidth, argv[5]);
		}

		printf("fapi_wlan_failsafe_channel_set test; ifname= '%s', Channel= '%s', CenterFrequency= '%s', OperatingChannelBandwidth= '%s'\n",
		       ifname, CenterFrequency, WaveVhtCenterFrequency, OperatingChannelBandwidth);

		help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "Channel", CenterFrequency, 0, 0);
		if (strcmp(argv[3], "0"))  // Channel != '0'
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "CenterFrequency", WaveVhtCenterFrequency, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "OperatingChannelBandwidth", OperatingChannelBandwidth, 0, 0);
		}

		/* Optional Params */
		printf("Optional Params: ");
		if (argc > 7)
		{  /* must have parameter #6 and #7 both present */
			for (i=6; i < argc; i++)
			{
				if (!strncmp(argv[i], "tx_ant_mask=", strlen("tx_ant_mask=")))
				{
					tx_ant_mask = argv[i]+strlen("tx_ant_mask=");
					printf("tx_ant_mask= '%s'; ", tx_ant_mask);
				}
				else if (!strncmp(argv[i], "rx_ant_mask=", strlen("rx_ant_mask=")))
				{
					rx_ant_mask = argv[i]+strlen("rx_ant_mask=");
					printf("rx_ant_mask= '%s'; ", rx_ant_mask);
				}
			}
		}
		printf("\n");

		if ( (tx_ant_mask != NULL) && (rx_ant_mask != NULL) )
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "tx_ant_mask", tx_ant_mask, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rx_ant_mask", rx_ant_mask, 0, 0);
		}
		else if ( !((tx_ant_mask == NULL) && (rx_ant_mask == NULL)) )
		{
			/* only warn if user set one param but not the other */
			printf("fapi_wlan_failsafe_channel_set; only one of tx_ant_mask or rx_ant_mask is set. Ignoring.\n");
		}

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_failsafe_channel_set(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_failsafe_channel_set ERROR\n");
	}
	else if (!strcmp(argv[1], "MBO_STA_ALLOW"))
	{
		/*
		Format:  MBO_STA_ALLOW ifname Own-BSSID
		Example: MBO_STA_ALLOW wlan1 00:0A:1B:0E:04:70
		*/

		if (argc > 3)
		{
			strcpy(BSSID, argv[3]);
		}

		printf("fapi_wlan_mbo_sta_allow test; ifname= '%s', BSSID= '%s'\n", ifname, BSSID);

		help_addObjList(wlObj, "Device.WiFi.SSID", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID", "BSSID", BSSID, 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_mbo_sta_allow(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_mbo_sta_allow ERROR\n");
	}
	else if (!strcmp(argv[1], "MBO_STA_DENY"))
	{
		/*
		Format:  MBO_STA_DENY ifname Own-BSSID Reason-Code (reason code: 1 - 5)
		Example: MBO_STA_DENY wlan1 00:0A:1B:0E:04:70 1
		*/

		if (argc > 3)
		{
			strcpy(BSSID, argv[3]);
		}

		if (argc > 4)
		{
			strcpy(MboBssAssociationReason, argv[4]);
		}

		printf("fapi_wlan_mbo_sta_deny test; ifname= '%s', BSSID= '%s', MboBssAssociationReason= '%s'\n",
		       ifname, BSSID, MboBssAssociationReason);

		help_addObjList(wlObj, "Device.WiFi.SSID", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID", "BSSID", BSSID, 0, 0);

		help_addObjList(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.X_LANTIQ_COM_Vendor", "MboBssAssociationReason", MboBssAssociationReason, 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_mbo_sta_deny(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_mbo_sta_deny ERROR\n");
	}
	else if (!strcmp(argv[1], "NEIGHBOR_SET"))
	{
		/*
		Format:   NEIGHBOR_SET ifname BSSID ssid NeighborReport [oce=<>] [tbtt_offset=<>]
		Examples: NEIGHBOR_SET wlan1 d8:fe:e3:3e:bd:14 '"widan_test"' 00112233445500000000510107 oce=1 tbtt_offset=1
		          NEIGHBOR_SET wlan1 d8:fe:e3:3e:bd:14 '"widan_test"' 00112233445500000000510107
		Note:     oce= 0:1; tbtt_offset= 0:255
		*/

		if (argc > 3)
		{
			strcpy(BSSID, argv[3]);
		}

		if (argc > 4)
		{
			strcpy(ssid, argv[4]);
		}

		if (argc > 5)
		{
			strcpy(NeighborReport, argv[5]);
		}

		printf("fapi_wlan_neighbor_set test; ifname= '%s', BSSID= '%s', ssid= '%s', NeighborReport= '%s'\n", ifname, BSSID, ssid, NeighborReport);

		help_addObjList(wlObj, "Device.WiFi.SSID", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID", "BSSID", BSSID, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID", "SSID", ssid, 0, 0);

		help_addObjList(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "NeighborReport", NeighborReport, 0, 0);

		/* Optional Params */
		printf("Optional Params: ");
		for (i=6; i < argc; i++)
		{
			if (!strncmp(argv[i], "oce=", strlen("oce=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "oce", argv[i]+strlen("oce="), 0, 0);
				printf("oce= '%s'; ", argv[i]+strlen("oce="));
			}
			else if (!strncmp(argv[i], "tbtt_offset=", strlen("tbtt_offset=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID.X_LANTIQ_COM_Vendor", "tbtt_offset", argv[i]+strlen("tbtt_offset="), 0, 0);
				printf("tbtt_offset= '%s'; ", argv[i]+strlen("tbtt_offset="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_neighbor_set(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_neighbor_set ERROR\n");
	}
	else if (!strcmp(argv[1], "NEIGHBOR_REMOVE"))
	{
		/*
		Format:  NEIGHBOR_REMOVE ifname BSSID ssid
		Example: NEIGHBOR_REMOVE wlan1 d8:fe:e3:3e:bd:14 '"widan test"'
		*/

		if (argc > 3)
		{
			strcpy(BSSID, argv[3]);
		}

		if (argc > 4)
		{
			strcpy(ssid, argv[4]);
		}

		printf("fapi_wlan_neighbor_remove test; ifname= '%s', BSSID= '%s', ssid= '%s'\n", ifname, BSSID, ssid);

		help_addObjList(wlObj, "Device.WiFi.SSID", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID", "BSSID", BSSID, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.SSID", "SSID", ssid, 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_neighbor_remove(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_neighbor_remove ERROR\n");
	}
	else if (!strcmp(argv[1], "CELLULAR_PREF_SET"))
	{
		/*
		Format:  CELLULAR_PREF_SET ifname CellPref
		Example: CELLULAR_PREF_SET wlan0 1
		*/

		if (argc > 3)
		{
			strcpy(CellPref, argv[3]);
		}

		printf("fapi_wlan_cellular_pref_set test; ifname= '%s', CellPref= '%s'\n", ifname, CellPref);

		help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "CellPref", CellPref, 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_cellular_pref_set(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_cellular_pref_set ERROR\n");
	}
	else if (!strcmp(argv[1], "CELLULAR_AWARE_SET"))
	{
		/*
		Format:  CELLULAR_AWARE_SET ifname MBOCellAware 
		Example: CELLULAR_AWARE_SET wlan0 1
		*/

		if (argc > 3)
		{
			strcpy(MBOCellAware, argv[3]);
		}

		printf("fapi_wlan_cellular_aware_set test; ifname= '%s', MBOCellAware= '%s'\n", ifname, MBOCellAware);

		help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "MBOCellAware", MBOCellAware, 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_cellular_aware_set(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_cellular_aware_set ERROR\n");
	}
	else if (!strcmp(argv[1], "SET_COMMAND_SEND"))
	{
		/*
		Format:   SET_COMMAND_SEND ifname paramName paramValue 
		Examples: 1) Enable OCE RSSI Association rejection
		             Range: 0 - disabled, 1 - enabled
		             SET_COMMAND_SEND wlan0 oce_assoc_reject_enabled 1
		          2) OCE Association rejection min RSSI treshold value
				     Range: -254 - 0
		             SET_COMMAND_SEND wlan0 oce_assoc_reject_min_rssi_threshold -70
		          3) OCE Association rejection retry delay value in seconds
				     Range: 0 - 255
		             SET_COMMAND_SEND wlan0 oce_assoc_reject_retry_delay 30
		          4) OCE downlink available capacity for the reduced wan metrics IE
				     Range: 0 - 15
		             SET_COMMAND_SEND wlan0 oce_downlink_available_capacity 7
		          5) OCE uplink available capacity for the reduced wan metrics IE
				     Range: 0 - 15
		             SET_COMMAND_SEND wlan0 oce_uplink_available_capacity 7
		*/

		if (argc <= 4)
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		printf("fapi_wlan_set_command_send test; ifname= '%s', paramName= '%s', paramValue= '%s'\n", ifname, argv[3], argv[4]);

		help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "paramName", argv[3], 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "paramValue", argv[4], 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_set_command_send(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_set_command_send ERROR\n");
	}
	else if (!strcmp(argv[1], "CHANNEL_LOAD_REQ"))
	{
		/* Format:  CHANNEL_LOAD_REQ <ifname> <dst> <num_of_repetitions> <measurement_req_mode> <op_class> <channel>
					<rand_int> <duration> [ch_load_rep=<0-2>,<0-255>]
					[wide_band_ch_switch=<new channel width>,<new channel center frequency segment 0>,< new channel center frequency segment 1>]

		   Example: CHANNEL_LOAD_REQ wlan0 44:85:00:C5:6A:1B 10 0128 11 10 50 50 wide_band_ch_switch=1,1,1
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", argv[3], 0, 0);
		}

		if (argc > 4)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "num_of_repetitions", argv[4], 0, 0);
		}

		if (argc > 5)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_req_mode", argv[5], 0, 0);
		}

		if (argc > 6)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "op_class", argv[6], 0, 0);
		}

		if (argc > 7)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio", "Channel", argv[7], 0, 0);
		}

		if (argc > 8)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rand_int", argv[8], 0, 0);
		}

		if (argc > 9)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", argv[9], 0, 0);

			printf("fapi_wlan_channel_load_request test; ifname= '%s', dst= '%s', num_of_repetitions= '%s', measurement_req_mode= '%s', op_class= '%s', channel= '%s', rand_int= '%s', duration= '%s'\n",
			       argv[2], argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]);
		}
		else
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Optional Params */
		printf("Optional Params: ");
		for (i=10; i < argc; i++)
		{
			if (!strncmp(argv[i], "ch_load_rep=", strlen("ch_load_rep=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ch_load_rep", argv[i]+strlen("ch_load_rep="), 0, 0);
				printf("ch_load_rep= '%s'; ", argv[i]+strlen("ch_load_rep="));
			}
			else if (!strncmp(argv[i], "wide_band_ch_switch=", strlen("wide_band_ch_switch=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "wide_band_ch_switch", argv[i]+strlen("wide_band_ch_switch="), 0, 0);
				printf("wide_band_ch_switch= '%s'; ", argv[i]+strlen("wide_band_ch_switch="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_channel_load_request(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_channel_load_request ERROR\n");
		else
		{
			printf("fapi_wlan_channel_load_request - output object:\n");
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "CHANNEL_LOAD_RESP"))
	{
		/* Format:  CHANNEL_LOAD_RESP <ifname> <dst> <dialog_token> <measurement_token> <measurement_rep_mode> <op_class> <channel>
					<start_time> <duration> <channel_load>
					[wide_band_ch_switch=<new channel width>,<new channel center frequency segment 0>,< new channel center frequency segment 1>]

		   Example: CHANNEL_LOAD_RESP wlan0 44:85:00:C5:6A:1B 10 11 0 128 6 1234567892947293847 50 36 wide_band_ch_switch=1,1,1
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", argv[3], 0, 0);
		}

		if (argc > 4)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dialog_token", argv[4], 0, 0);
		}

		if (argc > 5)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_token", argv[5], 0, 0);
		}

		if (argc > 6)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_rep_mode", argv[6], 0, 0);
		}

		if (argc > 7)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "op_class", argv[7], 0, 0);
		}

		if (argc > 8)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio", "Channel", argv[8], 0, 0);
		}

		if (argc > 9)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "start_time", argv[9], 0, 0);
		}

		if (argc > 10)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", argv[10], 0, 0);
		}

		if (argc > 11)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "channel_load", argv[11], 0, 0);

			printf("fapi_wlan_channel_load_response test; dst= '%s', dialog_token= '%s', measurement_token= '%s', measurement_rep_mode= '%s', op_class= '%s', Channel= '%s', start_time= '%s', duration= '%s', channel_load= '%s'\n",
			       argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11]);
		}
		else
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Optional Params */
		printf("Optional Params: ");
		if (argc > 12)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "wide_band_ch_switch", argv[12]+strlen("wide_band_ch_switch="), 0, 0);
			printf("wide_band_ch_switch= '%s'; ", argv[12]+strlen("wide_band_ch_switch="));
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_channel_load_response(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_channel_load_response ERROR\n");
	}
	else if (!strcmp(argv[1], "BEACON_REQ"))
	{
		/* Format:  BEACON_REQ <ifname> <dst> <num_of_repetitions> <measurement_req_mode> <op_class>
					<Channel> <rand_int> <duration> <mode> <bssid>
					[ssid=<ssid>] [rep_detail=<0-2>] [rep_cond=<0-10>,<0-255>] [ap_ch_report=[<channel 1>[,<channel 2>[..,<channel N>]]]
					[req_elements=[<element 1>[,<element 2>[..,<element N>]]]
					[wide_band_ch_switch=<new channel width>,<new channel center frequency segment 0>,< new channel center frequency segment 1>]

		   Example: BEACON_REQ wlan0 44:85:00:C5:6A:1B 0 0 0 1 1000 50 passive 00:0A:1B:0E:04:60 beacon_rep=1,123 rep_detail=2 'ssid="AA BB"' ap_ch_report=1,2,3 req_elements=0,51 wide_band_ch_switch=1,1,1
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", argv[3], 0, 0);
		}

		if (argc > 4)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "num_of_repetitions", argv[4], 0, 0);
		}

		if (argc > 5)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_req_mode", argv[5], 0, 0);
		}

		if (argc > 6)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "op_class", argv[6], 0, 0);
		}

		if (argc > 7)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio", "Channel", argv[7], 0, 0);
		}

		if (argc > 8)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rand_int", argv[8], 0, 0);
		}

		if (argc > 9)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", argv[9], 0, 0);
		}

		if (argc > 10)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "mode", argv[10], 0, 0);
		}

		if (argc > 11)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssid", argv[11], 0, 0);

			printf("fapi_wlan_beacon_request test; dst= '%s', num_of_repetitions= '%s', measurement_req_mode= '%s', op_class= '%s', Channel= '%s', rand_int= '%s', duration= '%s', mode= '%s', bssid= '%s'\n",
			       argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11]);
		}
		else
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Optional Params */
		printf("Optional Params: ");
		for (i=12; i < argc; i++)
		{
			if (!strncmp(argv[i], "beacon_rep=", strlen("beacon_rep=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "beacon_rep", argv[i]+strlen("beacon_rep="), 0, 0);
				printf("beacon_rep= '%s'; ", argv[i]+strlen("beacon_rep="));
			}
			else if (!strncmp(argv[i], "rep_detail=", strlen("rep_detail=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rep_detail", argv[i]+strlen("rep_detail="), 0, 0);
				printf("rep_detail= '%s'; ", argv[i]+strlen("rep_detail="));
			}
			else if (!strncmp(argv[i], "ssid=", strlen("ssid=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ssid", argv[i]+strlen("ssid="), 0, 0);
				printf("ssid= '%s'; ", argv[i]+strlen("ssid="));
			}
			else if (!strncmp(argv[i], "rep_detail=", strlen("rep_detail=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rep_detail", argv[i]+strlen("rep_detail="), 0, 0);
				printf("rep_detail= '%s'; ", argv[i]+strlen("rep_detail="));
			}
			else if (!strncmp(argv[i], "ap_ch_report=", strlen("ap_ch_report=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ap_ch_report", argv[i]+strlen("ap_ch_report="), 0, 0);
				printf("ap_ch_report= '%s'; ", argv[i]+strlen("ap_ch_report="));
			}
			else if (!strncmp(argv[i], "req_elements=", strlen("req_elements=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "req_elements", argv[i]+strlen("req_elements="), 0, 0);
				printf("req_elements= '%s'; ", argv[i]+strlen("req_elements="));
			}
			else if (!strncmp(argv[i], "wide_band_ch_switch=", strlen("wide_band_ch_switch=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "wide_band_ch_switch", argv[i]+strlen("wide_band_ch_switch="), 0, 0);
				printf("wide_band_ch_switch= '%s'; ", argv[i]+strlen("wide_band_ch_switch="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_beacon_request(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_beacon_request ERROR\n");
		else
		{
			printf("fapi_wlan_beacon_request - output object:\n");
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "BEACON_REPORT_RESP"))
	{
		/* Format:  BEACON_REPORT_RESP <ifname> <dst> <dialog_token> <measurement_token> <measurement_rep_mode> <op_class>
					<Channel> <start_time> <duration> <frame_info> <rcpi> <rsni> <bssid> <ant_id> <tsf>
					[wide_band_ch_switch=<new channel width>,<new channel center frequency segment 0>,< new channel center frequency segment 1>]
					[rep_frame_body=<length>,<frame_body hexdump>]

		   Example: BEACON_REPORT_RESP wlan0 44:85:00:C5:6A:1B 10 11 0 128 11 1234567892947293847 50 10 11 12 12:34:56:78:00:00 23 00012345 wide_band_ch_switch=1,1,1 rep_frame_body=16,0123456789ABCDEF
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", argv[3], 0, 0);
		}

		if (argc > 4)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dialog_token", argv[4], 0, 0);
		}

		if (argc > 5)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_token", argv[5], 0, 0);
		}

		if (argc > 6)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_rep_mode", argv[6], 0, 0);
		}

		if (argc > 7)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "op_class", argv[7], 0, 0);
		}

		if (argc > 8)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio", "Channel", argv[8], 0, 0);
		}

		if (argc > 9)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "start_time", argv[9], 0, 0);
		}

		if (argc > 10)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", argv[10], 0, 0);
		}

		if (argc > 11)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "frame_info", argv[11], 0, 0);
		}

		if (argc > 12)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rcpi", argv[12], 0, 0);
		}

		if (argc > 13)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rsni", argv[13], 0, 0);
		}

		if (argc > 14)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssid", argv[14], 0, 0);
		}

		if (argc > 15)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ant_id", argv[15], 0, 0);
		}

		if (argc > 16)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "tsf", argv[16], 0, 0);

			printf("fapi_wlan_beacon_report_response test; dst= '%s', dialog_token= '%s', measurement_token= '%s', measurement_rep_mode= '%s', op_class= '%s', Channel= '%s', start_time= '%s', duration= '%s', frame_info= '%s', rcpi= '%s', rsni= '%s', bssid= '%s', ant_id= '%s', tsf= '%s'\n",
				   argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11], argv[12], argv[13], argv[14], argv[15], argv[16]);
		}
		else
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Optional Params */
		printf("Optional Params: ");
		for (i=17; i < argc; i++)
		{
			if (!strncmp(argv[i], "wide_band_ch_switch=", strlen("wide_band_ch_switch=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "wide_band_ch_switch", argv[i]+strlen("wide_band_ch_switch="), 0, 0);
				printf("wide_band_ch_switch= '%s'; ", argv[i]+strlen("wide_band_ch_switch="));
			}
			else if (!strncmp(argv[i], "rep_frame_body=", strlen("rep_frame_body=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rep_frame_body", argv[i]+strlen("rep_frame_body="), 0, 0);
				printf("rep_frame_body= '%s'; ", argv[i]+strlen("rep_frame_body="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_beacon_report_response(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_beacon_report_response ERROR\n");
	}
	else if (!strcmp(argv[1], "SELF_BEACON_REPORT_REQ"))
	{
		/* Format:  SELF_BEACON_REPORT_REQ ifname <rand_int> <duration> <mode-"active"/"passive"/"table">
		   Example: SELF_BEACON_REPORT_REQ wlan0 1000 50 passive
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rand_int", argv[3], 0, 0);
		}

		if (argc > 4)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", argv[4], 0, 0);
		}

		if (argc > 5)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "mode", argv[5], 0, 0);

			printf("fapi_wlan_self_beacon_request test; rand_int= '%s', duration= '%s', mode= '%s'\n",
			       argv[3], argv[4], argv[5]);
		}
		else
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_self_beacon_request(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_self_beacon_request ERROR\n");
	}
	else if (!strcmp(argv[1], "STA_STATISTICS_REQ"))
	{
		/* Format:  STA_STATISTICS_REQ <ifname> <dst> <num_of_repetitions> <measurement_req_mode> <peer_mac_addr> <rand_int> <duration> <group_identity>
					[trig_rep_sta_counters=<>] [trig_rep_qos_sta_counters=<>] [trig_rep_rsna_counters=<>]
		   Example: STA_STATISTICS_REQ wlan0 44:85:00:C5:6A:1B 0 0 11:22:33:44:55:66 1000 50 1 trig_rep_sta_counters=150,123,127,1,2,3,4,5,6,7 trig_rep_qos_sta_counters=1234,100,109,1,2,3,4,5 trig_rep_rsna_counters=300,20,55,1,2,3,4,5
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", argv[3], 0, 0);
		}

		if (argc > 4)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "num_of_repetitions", argv[4], 0, 0);
		}

		if (argc > 5)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_req_mode", argv[5], 0, 0);
		}

		if (argc > 6)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "peer_mac_addr", argv[6], 0, 0);
		}

		if (argc > 7)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rand_int", argv[7], 0, 0);
		}

		if (argc > 8)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", argv[8], 0, 0);
		}

		if (argc > 9)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "group_identity", argv[9], 0, 0);

			printf("fapi_wlan_sta_statistics_request test; dst= '%s', num_of_repetitions= '%s', measurement_req_mode= '%s', peer_mac_addr= '%s', rand_int= '%s', duration= '%s', group_identity= '%s'\n",
			       argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]);
		}
		else
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Optional Params */
		printf("Optional Params: ");
		for (i=10; i < argc; i++)
		{
			if (!strncmp(argv[i], "trig_rep_sta_counters=", strlen("trig_rep_sta_counters=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "trig_rep_sta_counters", argv[i]+strlen("trig_rep_sta_counters="), 0, 0);
				printf("trig_rep_sta_counters= '%s'; ", argv[i]+strlen("trig_rep_sta_counters="));
			}
			else if (!strncmp(argv[i], "trig_rep_qos_sta_counters=", strlen("trig_rep_qos_sta_counters=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "trig_rep_qos_sta_counters", argv[i]+strlen("trig_rep_qos_sta_counters="), 0, 0);
				printf("trig_rep_qos_sta_counters= '%s'; ", argv[i]+strlen("trig_rep_qos_sta_counters="));
			}
			else if (!strncmp(argv[i], "trig_rep_rsna_counters=", strlen("trig_rep_rsna_counters=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "trig_rep_rsna_counters", argv[i]+strlen("trig_rep_rsna_counters="), 0, 0);
				printf("trig_rep_rsna_counters= '%s'; ", argv[i]+strlen("trig_rep_rsna_counters="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_sta_statistics_request(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_sta_statistics_request ERROR\n");
		else
		{
			printf("fapi_wlan_sta_statistics_request - output object:\n");
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "STA_STATISTICS_RESP"))
	{
		/* Format:  STA_STATISTICS_RESP <ifname> <dst> <dialog_token> <measurement_token> <measurement_rep_mode>
					<duration> <group_identity> <statistics_group_data> [rep_reason=<>]
		   Example: STA_STATISTICS_RESP wlan0 44:85:00:C5:6A:1B 10 11 0 50 1 1,2,3,4,5,6 rep_reason=15
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", argv[3], 0, 0);
		}

		if (argc > 4)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dialog_token", argv[4], 0, 0);
		}

		if (argc > 5)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_token", argv[5], 0, 0);
		}

		if (argc > 6)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_rep_mode", argv[6], 0, 0);
		}

		if (argc > 7)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", argv[7], 0, 0);
		}

		if (argc > 8)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "group_identity", argv[8], 0, 0);
		}

		if (argc > 9)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "statistics_group_data", argv[9], 0, 0);

			printf("fapi_wlan_sta_statistics_response test; dst= '%s', dialog_token= '%s', measurement_token= '%s', measurement_rep_mode= '%s', duration= '%s', group_identity= '%s', statistics_group_data= '%s'\n",
			       argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]);
		}
		else
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Optional Params */
		printf("Optional Params: ");
		if (argc > 10)
		{
			if (!strncmp(argv[10], "rep_reason=", strlen("rep_reason=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rep_reason", argv[10]+strlen("rep_reason="), 0, 0);
				printf("rep_reason= '%s'; ", argv[10]+strlen("rep_reason="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_sta_statistics_response(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_sta_statistics_response ERROR\n");
	}
	else if (!strcmp(argv[1], "STA_LINK_MEASUREMENT_REQ"))
	{
		/* Format:  STA_LINK_MEASUREMENT_REQ <ifname> <dst>
		   Example: STA_LINK_MEASUREMENT_REQ wlan0 44:85:00:C5:6A:1B
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", argv[3], 0, 0);
		}

		printf("fapi_wlan_sta_link_measurements_request test; dst= '%s'\n", argv[3]);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_sta_link_measurements_request(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_sta_link_measurements_request ERROR\n");
		else
		{
			printf("fapi_wlan_sta_link_measurements_request - output object:\n");
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "LINK_MEASUREMENT_RESP"))
	{
		/* Format:  LINK_MEASUREMENT_RESP <ifname> dst, dialog_token, rx_ant_id, tx_ant_id, rcpi, rsni, tpc_report=<>,
					[dmg_link_margin=<>]  [dmg_link_adapt_ack=<>]
		   Example: LINK_MEASUREMENT_RESP wlan0 44:85:00:C5:6A:1B 10 12 34 56 78 tpc_report=100,50 dmg_link_margin=1,3,5,7,1000 dmg_link_adapt_ack=2,2000
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", argv[3], 0, 0);
		}

		if (argc > 4)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dialog_token", argv[4], 0, 0);
		}

		if (argc > 5)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rx_ant_id", argv[5], 0, 0);
		}

		if (argc > 6)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "tx_ant_id", argv[6], 0, 0);
		}

		if (argc > 7)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rcpi", argv[7], 0, 0);
		}

		if (argc > 8)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rsni", argv[8], 0, 0);

			printf("fapi_wlan_sta_link_measurements_response test; dst= '%s', dialog_token= '%s', rx_ant_id= '%s', tx_ant_id= '%s', rcpi= '%s', rsni= '%s'\n",
			       argv[3], argv[4], argv[5], argv[6], argv[7], argv[8]);
		}
		else
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Optional Params */
		printf("Optional Params: ");
		for (i=9; i < argc; i++)
		{
			if (!strncmp(argv[i], "tpc_report=", strlen("tpc_report=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "tpc_report", argv[i]+strlen("tpc_report="), 0, 0);
				printf("tpc_report= '%s'; ", argv[i]+strlen("tpc_report="));
			}
			else if (!strncmp(argv[i], "dmg_link_margin=", strlen("dmg_link_margin=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dmg_link_margin", argv[i]+strlen("dmg_link_margin="), 0, 0);
				printf("dmg_link_margin= '%s'; ", argv[i]+strlen("dmg_link_margin="));
			}
			else if (!strncmp(argv[i], "dmg_link_adapt_ack=", strlen("dmg_link_adapt_ack=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dmg_link_adapt_ack", argv[i]+strlen("dmg_link_adapt_ack="), 0, 0);
				printf("dmg_link_adapt_ack= '%s'; ", argv[i]+strlen("dmg_link_adapt_ack="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_sta_link_measurements_response(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_sta_link_measurements_response ERROR\n");
	}
	else if (!strcmp(argv[1], "NOISE_HISTOGRAM_REQ"))
	{
		/* Format:  NOISE_HISTOGRAM_REQ <ifname> <dst> <num_of_repetitions> <measurement_req_mode>  <op_class> <channel> <rand_int> <duration>
		            [noise_histogram_rep=<0-2>,<0-255>]
		            [wide_band_ch_switch=<new channel width>,<new channel center frequency segment 0>,< new channel center frequency segment 1>]
		
		   Example: NOISE_HISTOGRAM_REQ wlan0 44:85:00:C5:6A:1B 0 0 10 36 1000 50 noise_histogram_rep=0,51 wide_band_ch_switch=1,1,1
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", argv[3], 0, 0);
		}

		if (argc > 4)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "num_of_repetitions", argv[4], 0, 0);
		}

		if (argc > 5)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_req_mode", argv[5], 0, 0);
		}

		if (argc > 6)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "op_class", argv[6], 0, 0);
		}

		if (argc > 7)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "channel", argv[7], 0, 0);
		}

		if (argc > 8)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "rand_int", argv[8], 0, 0);
		}

		if (argc > 9)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", argv[9], 0, 0);

			printf("fapi_wlan_noise_histigram_request test; dst= '%s', num_of_repetitions= '%s', measurement_req_mode= '%s', op_class= '%s', channel= '%s', rand_int= '%s', duration= '%s'\n",
			       argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9]);
		}
		else
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Optional Params */
		printf("Optional Params: ");
		for (i=10; i < argc; i++)
		{
			if (!strncmp(argv[i], "noise_histogram_rep=", strlen("noise_histogram_rep=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "noise_histogram_rep", argv[i]+strlen("noise_histogram_rep="), 0, 0);
				printf("noise_histogram_rep= '%s'; ", argv[i]+strlen("noise_histogram_rep="));
			}
			else if (!strncmp(argv[i], "wide_band_ch_switch=", strlen("wide_band_ch_switch=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "wide_band_ch_switch", argv[i]+strlen("wide_band_ch_switch="), 0, 0);
				printf("wide_band_ch_switch= '%s'; ", argv[i]+strlen("wide_band_ch_switch="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_noise_histigram_request(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_noise_histigram_request ERROR\n");
		else
		{
			printf("fapi_wlan_noise_histigram_request - output object:\n");
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "NOISE_HISTOGRAM_RESP"))
	{
		/* Format:  NOISE_HISTOGRAM_RESP <ifname> <dst> <dialog_token> <measurement_token> <measurement_rep_mode> <op_class> <channel> <start_time> <duration> <ant_id> <anpi> <ipi>
                    [wide_band_ch_switch=<new channel width>,<new channel center frequency segment 0>,< new channel center frequency segment 1>]
		
		   Example: NOISE_HISTOGRAM_RESP wlan0 44:85:00:C5:6A:1B 10 11 0 128 11 1234567892947293847 50 33 44 0,1,2,3,4,5,6,7,8,9,10 wide_band_ch_switch=1,1,1
		*/

		if (argc > 3)
		{
			help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dst", argv[3], 0, 0);
		}

		if (argc > 4)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "dialog_token", argv[4], 0, 0);
		}

		if (argc > 5)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_token", argv[5], 0, 0);
		}

		if (argc > 6)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "measurement_rep_mode", argv[6], 0, 0);
		}

		if (argc > 7)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "op_class", argv[7], 0, 0);
		}

		if (argc > 8)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "channel", argv[8], 0, 0);
		}

		if (argc > 9)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "start_time", argv[9], 0, 0);
		}

		if (argc > 10)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "duration", argv[10], 0, 0);
		}

		if (argc > 11)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ant_id", argv[11], 0, 0);
		}

		if (argc > 12)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "anpi", argv[12], 0, 0);
		}

		if (argc > 13)
		{
			HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ipi", argv[13], 0, 0);

			printf("fapi_wlan_noise_histogram_response test; dst= '%s', dialog_token= '%s', measurement_token= '%s', measurement_rep_mode= '%s', op_class= '%s', channel= '%s', start_time= '%s', duration= '%s', ant_id= '%s', anpi= '%s', ipi= '%s'\n",
			       argv[3], argv[4], argv[5], argv[6], argv[7], argv[8], argv[9], argv[10], argv[11], argv[12], argv[13]);
		}
		else
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		/* Optional Params */
		printf("Optional Params: ");
		if (argc > 14)
		{
			if (!strncmp(argv[14], "wide_band_ch_switch=", strlen("wide_band_ch_switch=")))
			{
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "wide_band_ch_switch", argv[14]+strlen("wide_band_ch_switch="), 0, 0);
				printf("wide_band_ch_switch= '%s'; ", argv[14]+strlen("wide_band_ch_switch="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_noise_histogram_response(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_noise_histogram_response ERROR\n");
	}
	else if (!strcmp(argv[1], "OCE_WAN_METRICS_SET"))
	{
		/*
		Format:   OCE_WAN_METRICS_SET <ifname> [uplink_capacity=<>] [downlink_capacity=<>]
		Examples: OCE_WAN_METRICS_SET wlan0 uplink_capacity=7 downlink_capacity=5
		          OCE_WAN_METRICS_SET wlan0 downlink_capacity=5
		          uplink_capacity, downlink_capacity in Mbps units
		*/

		if (argc <= 3)
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		printf("fapi_wlan_oce_wan_metrics_set test; ifname= '%s'\n", ifname);

		/* Optional Params */
		printf("Optional Params: ");
		for (i=3; i < argc; i++)
		{
			if (!strncmp(argv[i], "uplink_capacity=", strlen("uplink_capacity=")))
			{
				addObjListIfNotExist(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor");
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "uplink_capacity", argv[i]+strlen("uplink_capacity="), 0, 0);
				printf("uplink_capacity= '%s'; ", argv[i]+strlen("uplink_capacity="));
			}
			else if (!strncmp(argv[i], "downlink_capacity=", strlen("downlink_capacity=")))
			{
				addObjListIfNotExist(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor");
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "downlink_capacity", argv[i]+strlen("downlink_capacity="), 0, 0);
				printf("downlink_capacity= '%s'; ", argv[i]+strlen("downlink_capacity="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_oce_wan_metrics_set(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_oce_wan_metrics_set ERROR\n");
	}
	else if (!strcmp(argv[1], "OCE_ASSOC_REJECT_SET"))
	{
		/*
		Format:  OCE_ASSOC_REJECT_SET <ifname> <enable/disable> [<min_rssi_threshold>=<>] [<retry_delay>=<>]
		Example: OCE_ASSOC_REJECT_SET wlan0 1
		         OCE_ASSOC_REJECT_SET wlan0 1 min_rssi_threshold=-50
		         OCE_ASSOC_REJECT_SET wlan0 1 retry_delay=20
		         OCE_ASSOC_REJECT_SET wlan0 1 min_rssi_threshold=-50 retry_delay=20
		         enable/disable     - Range: 0 - 1
				 min_rssi_threshold - Range: -254 - 0
				 retry_delay        - Range: 0 - 255
		*/

		if (argc <= 3)
		{
			printf("Invalid Input Parameters!!!\n");
			HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
			return;
		}

		printf("fapi_wlan_oce_assoc_reject_set test; ifname= '%s', enableDisable= '%s'\n", ifname, argv[3]);

		help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "enableDisable", argv[3], 0, 0);

		/* Optional Params */
		printf("Optional Params: ");
		for (i=4; i < argc; i++)
		{
			if (!strncmp(argv[i], "min_rssi_threshold=", strlen("min_rssi_threshold=")))
			{
				addObjListIfNotExist(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor");
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "min_rssi_threshold", argv[i]+strlen("min_rssi_threshold="), 0, 0);
				printf("min_rssi_threshold= '%s'; ", argv[i]+strlen("min_rssi_threshold="));
			}
			else if (!strncmp(argv[i], "retry_delay=", strlen("retry_delay=")))
			{
				addObjListIfNotExist(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor");
				HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "retry_delay", argv[i]+strlen("retry_delay="), 0, 0);
				printf("retry_delay= '%s'; ", argv[i]+strlen("retry_delay="));
			}
		}
		printf("\n");

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_oce_assoc_reject_set(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_oce_assoc_reject_set ERROR\n");
	}
	else if (!strcmp(argv[1], "RADIO_STATE_GET"))
	{
		/* Format:  RADIO_STATE_GET <ifname>
		   Example: RADIO_STATE_GET wlan0
		*/

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_radio_state_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_radio_state_get ERROR\n");
		else
		{
			printf("fapi_wlan_radio_state_get - output object:\n");
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "MAC_ADDRESS_GET"))
	{
		/* Format:  MAC_ADDRESS_GET <ifname>
		   Example: MAC_ADDRESS_GET wlan0
		*/

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_mac_address_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_mac_address_get ERROR\n");
		else
		{
			printf("fapi_wlan_mac_address_get - output object:\n");
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "CHANNEL_GET"))
	{
		/* Format:  CHANNEL_GET <ifname>
		   Example: CHANNEL_GET wlan0
		*/

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_channel_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_channel_get ERROR\n");
		else
		{
			printf("fapi_wlan_channel_get - output object:\n");
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "CTRL_A_FILTER_BANK_SET"))
	{
		/*
		Format:   CTRL_A_FILTER_BANK_SET ifname isOn
		Examples: CTRL_A_FILTER_BANK_SET wlan1 0
				  CTRL_A_FILTER_BANK_SET wlan1 1
		*/

		if (argc > 3)
		{
			strcpy(isOn, argv[3]);
		}
		else
		{  // Set the isOn default value to "1"
			strcpy(isOn, "1");
		}

		printf("fapi_wlan_radio_ctrl_a_filter_bank_set test; ifname= '%s', isOn= '%s'\n", ifname, isOn);

		/* create object for setting radio filter on/off */
		help_addObjList(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "filter_bank", isOn, 0, 0);

		setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

		if (fapi_wlan_radio_ctrl_a_filter_bank_set(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_radio_ctrl_a_filter_bank_set ERROR\n");
	}
	else if (!strcmp(argv[1], "CTRL_A_FILTER_BANK_GET"))
	{
		/* Format:  CTRL_A_FILTER_BANK_GET <ifname>
		   Example: CTRL_A_FILTER_BANK_GET wlan1
		*/

		if (fapi_wlan_radio_ctrl_a_filter_bank_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_radio_ctrl_a_filter_bank_get ERROR\n");
		else
		{
			printf("fapi_wlan_radio_ctrl_a_filter_bank_get - output object:\n");
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}


/*********************/
/* 1.3) Measurements */
/*********************/
	else if (!strcmp(argv[1], "STA_MEASUREMENT_GET"))
	{
		/*
		Format:  STA_MEASUREMENT_GET ifname MACAddress
		Example: STA_MEASUREMENT_GET wlan0 d8:fe:e3:3e:bd:14
		*/

		if (argc > 3)
		{
			strcpy(MACAddress, argv[3]);
		}

		printf("Get STA Measurements test; ifname= '%s', MACAddress= '%s'\n", ifname, MACAddress);

		help_addObjList(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", 0, 0, 0, 0);
		HELP_EDIT_NODE(wlObj, "Device.WiFi.AccessPoint.AssociatedDevice", "MACAddress", MACAddress, 0, 0);

		if (fapi_wlan_sta_measurement_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_sta_measurement_get ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "VAP_MEASUREMENT_GET"))
	{
		/*
		Format:  VAP_MEASUREMENT_GET ifname
		Example: VAP_MEASUREMENT_GET wlan0
		*/

		printf("Get VAP Measurements test; ifname= '%s'\n", ifname);

		if (fapi_wlan_vap_measurement_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_vap_measurement_get ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "RADIO_INFO_GET"))
	{
		/*
		Format:  RADIO_INFO_GET ifname
		Example: RADIO_INFO_GET wlan0
		*/

		printf("Get Radio info test; ifname= '%s'\n", ifname);

		if (fapi_wlan_radio_info_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_radio_info_get ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "ACS_REPORT_GET"))
	{
		/*
		Format:  ACS_REPORT_GET ifname
		Example: ACS_REPORT_GET wlan0
		*/

		printf("Get ACS report test; ifname= '%s'\n", ifname);

		if (fapi_wlan_acs_report_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_acs_report_get ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "FAILSAFE_CHANNEL_GET"))
	{
		/*
		Format:  FAILSAFE_CHANNEL_GET ifname
		Example: FAILSAFE_CHANNEL_GET wlan1
		*/

		printf("fapi_wlan_failsafe_channel_get test; ifname= '%s'\n", ifname);

		if (fapi_wlan_failsafe_channel_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_failsafe_channel_get ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "RESTRICTED_CHANNEL_GET"))
	{
		/*
		Format:  RESTRICTED_CHANNEL_GET ifname
		Example: RESTRICTED_CHANNEL_GET wlan0
		*/

		printf("fapi_wlan_restricted_channels_get test; ifname= '%s'\n", ifname);

		if (fapi_wlan_restricted_channels_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_restricted_channels_get ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "BEACON_REPORT_GET"))
	{
		/*
		Format:  BEACON_REPORT_GET ifname
		Example: BEACON_REPORT_GET wlan0
		*/

		printf("fapi_wlan_beacon_report_get test; ifname= '%s'\n", ifname);

		if (fapi_wlan_beacon_report_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_beacon_report_get ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "CONNECTION_STATUS_GET"))
	{
		/*
		Format:  CONNECTION_STATUS_GET ifname
		Example: CONNECTION_STATUS_GET wlan0
		*/

		printf("fapi_wlan_connection_status_get test; ifname= '%s'\n", ifname);

		if (fapi_wlan_connection_status_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_connection_status_get ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "METRIC_GET"))
	{
		/*
		Format:  METRIC_GET ifname
		Example: METRIC_GET wlan0
		*/

		printf("fapi_wlan_metrics_get test; ifname= '%s'\n", ifname);

		if (fapi_wlan_metrics_get(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_metrics_get ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_NETWORK_ADD"))
	{
		/*
		Format:  CLIENT_MODE_NETWORK_ADD ifname
		Example: CLIENT_MODE_NETWORK_ADD wlan1
		*/

		printf("fapi_wlan_client_mode_network_add test; ifname= '%s'\n", ifname);

		if (fapi_wlan_client_mode_network_add(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_client_mode_network_add ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_NETWORK_SET"))
	{
		/*
		Format:   CLIENT_MODE_NETWORK_SET ifname network_id ModeEnabled=<ModeEnabled> [ssid=<ssid>] [scan_ssid=<scan_ssid>] [bssid=<bssid>] ['KeyPassphrase="<KeyPassphrase>"'] [WEPKey=<WEPKey>]

		supported params: ssid bssid bssid_blacklist bssid_whitelist mode frequency proto scan_ssid key_mgmt wep_key0 wep_key1 wep_key2 wep_tx_keyidx pairwise
        group eap pin pcsc identity anonymous_identity password phase1 pac_file ca_cert client_cert private_key private_key_passwd
        eapol_flags psk phase2 ca_cert2 client_cert2 private_key2 private_key2_passwd priority auth_alg wpa_ptk_rekey macsec_policy

		Examples: CLIENT_MODE_NETWORK_SET wlan1 1 ModeEnabled=None 'ssid="widan_test"'
		          CLIENT_MODE_NETWORK_SET wlan1 1 ModeEnabled=None 'ssid="widan test"' scan_ssid=1 bssid=00:11:22:33:44:55 'KeyPassphrase="12345678"' WEPKey=FEDCBA9876543210
		          CLIENT_MODE_NETWORK_SET wlan1 1 ModeEnabled=WEP-64 'ssid="widan test"' bssid=00:11:22:33:44:55 'KeyPassphrase="12345678"' WEPKey=FEDCBA9876543210
		          CLIENT_MODE_NETWORK_SET wlan1 1 ModeEnabled=WEP-128 'ssid="widan test"' bssid=00:11:22:33:44:55 'KeyPassphrase="12345678"' WEPKey=FEDCBA9876543210
		          CLIENT_MODE_NETWORK_SET wlan1 1 ModeEnabled=WPA-Personal 'ssid="widan test"' bssid=00:11:22:33:44:55 'KeyPassphrase="12345678"' WEPKey=FEDCBA9876543210
		          CLIENT_MODE_NETWORK_SET wlan1 1 ModeEnabled=WPA2-Personal 'ssid="widan test"' bssid=00:11:22:33:44:55 'KeyPassphrase="12345678"' WEPKey=FEDCBA9876543210 scan_ssid=0
		          CLIENT_MODE_NETWORK_SET wlan1 1 ModeEnabled=WPA-WPA2-Personal 'ssid="widan test"' bssid=00:11:22:33:44:55 'KeyPassphrase="12345678"' WEPKey=FEDCBA9876543210

		Notes: Illegal parts of the commands are being ignored. Providind WEPKey in WPA modes, and KeyPassphrase in WEP modes are being ignored!
		*/

		if (argc > 3)
		{
			printf("fapi_wlan_client_mode_network_set test; ifname= '%s', NetworkID= '%s', ModeEnabled= '%s'\n", ifname, argv[3], argv[4]+strlen("ModeEnabled="));

			help_addObjList(wlObj, "Device.WiFi.EndPoint", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint", "X_LANTIQ_COM_Vendor_NetworkId", argv[3], 0, 0);

			help_addObjList(wlObj, "Device.WiFi.EndPoint.Profile.Security", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint.Profile.Security", "ModeEnabled", argv[4]+strlen("ModeEnabled="), 0, 0);

			/* Optional Params */
			printf("Optional Params: ");
			for (i=5; i < argc; i++)
			{
				if (!strncmp(argv[i], "ssid=", strlen("ssid=")))
				{
					addObjListIfNotExist(wlObj, "Device.WiFi.EndPoint.Profile");
					HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint.Profile", "SSID", argv[i]+strlen("ssid="), 0, 0);
					printf("ssid= '%s'; ", argv[i]+strlen("ssid="));
				}
				else if (!strncmp(argv[i], "scan_ssid=", strlen("scan_ssid=")))
				{
					HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint.Profile", "X_LANTIQ_COM_Vendor_scan_ssid", argv[i]+strlen("scan_ssid="), 0, 0);
					printf("scan_ssid= '%s'; ", argv[i]+strlen("scan_ssid="));
				}
				else if (!strncmp(argv[i], "bssid=", strlen("bssid=")))
				{
					addObjListIfNotExist(wlObj, "Device.WiFi.EndPoint.Profile");
					HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint.Profile", "X_LANTIQ_COM_Vendor_BSSID", argv[i]+strlen("bssid="), 0, 0);
					printf("bssid= '%s'; ", argv[i]+strlen("bssid="));
				}
				else if (!strncmp(argv[i], "KeyPassphrase=", strlen("KeyPassphrase=")))
				{
					HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint.Profile.Security", "KeyPassphrase", argv[i]+strlen("KeyPassphrase="), 0, 0);
					printf("KeyPassphrase= '%s'; ", argv[i]+strlen("KeyPassphrase="));
				}
				else if (!strncmp(argv[i], "WEPKey=", strlen("WEPKey=")))
				{
					HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint.Profile.Security", "WEPKey", argv[i]+strlen("WEPKey="), 0, 0);
					printf("WEPKey= '%s'; ", argv[i]+strlen("WEPKey="));
				}
			}
			printf("\n");

			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));

			if (fapi_wlan_client_mode_network_set(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_client_mode_network_set ERROR\n");
		}
		else
		{
			printf("fapi_wlan_client_mode_network_set test; not enough parameters (%d) ==> Abort!\n", argc);
		}
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_NETWORK_ENABLE"))
	{
		/*
		Format:   CLIENT_MODE_NETWORK_ENABLE ifname <network_id>
		Examples: CLIENT_MODE_NETWORK_ENABLE wlan1 1
		          CLIENT_MODE_NETWORK_ENABLE wlan1 all
		*/

		if (argc > 3)
		{
			printf("fapi_wlan_client_mode_network_enable test; ifname= '%s', NetworkID= '%s'\n", ifname, argv[3]);

			help_addObjList(wlObj, "Device.WiFi.EndPoint", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint", "X_LANTIQ_COM_Vendor_NetworkId", argv[3], 0, 0);

			if (fapi_wlan_client_mode_network_enable(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_client_mode_network_enable ERROR\n");
		}
		else
		{
			printf("fapi_wlan_client_mode_network_enable test; not enough parameters (%d) ==> Abort!\n", argc);
		}
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_NETWORK_SELECT"))
	{
		/*
		Format:   CLIENT_MODE_NETWORK_SELECT ifname <network_id> [frequency=<frequency>]
		Examples: CLIENT_MODE_NETWORK_SELECT wlan1 1
		          CLIENT_MODE_NETWORK_SELECT wlan1 1 frequency=2412-2432,2462,5000-6000
		*/

		if (argc > 3)
		{
			printf("fapi_wlan_client_mode_network_select test; ifname= '%s', NetworkID= '%s'\n", ifname, argv[3]);

			help_addObjList(wlObj, "Device.WiFi.EndPoint", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint", "X_LANTIQ_COM_Vendor_NetworkId", argv[3], 0, 0);

			printf("fapi_wlan_client_mode_network_select test; ifname= '%s', NetworkID= '%s'\n", ifname, argv[3]);

			/* Optional Params */
			if (argc > 4)
			{
				printf("Optional Params: ");
				if (!strncmp(argv[4], "frequency=", strlen("frequency=")))
				{
					help_addObjList(wlObj, "Device.WiFi.EndPoint.Profile", 0, 0, 0, 0);
					HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint.Profile", "X_LANTIQ_COM_Vendor_Frequency", argv[4]+strlen("frequency="), 0, 0);
					printf("frequency= '%s'; ", argv[4]+strlen("frequency="));
				}
				printf("\n");
			}

			if (fapi_wlan_client_mode_network_select(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_client_mode_network_select ERROR\n");
		}
		else
		{
			printf("fapi_wlan_client_mode_network_select test; not enough parameters (%d) ==> Abort!\n", argc);
		}
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_NETWORK_DISCONNECT"))
	{
		/*
		Format:  CLIENT_MODE_NETWORK_DISCONNECT ifname
		Example: CLIENT_MODE_NETWORK_DISCONNECT wlan1
		*/

		printf("fapi_wlan_client_mode_network_disconnect test; ifname= '%s'\n", ifname);

		if (fapi_wlan_client_mode_network_disconnect(ifname, wlObj, 0) == UGW_FAILURE)
			printf("fapi_wlan_client_mode_network_disconnect ERROR\n");
		else
			setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_NETWORK_REMOVE"))
	{
		/*
		Format:   CLIENT_MODE_NETWORK_REMOVE ifname <network_id>
		Examples: CLIENT_MODE_NETWORK_REMOVE wlan1 1
		          CLIENT_MODE_NETWORK_REMOVE wlan1 all
		*/

		if (argc > 3)
		{
			printf("fapi_wlan_client_mode_network_remove test; ifname= '%s', NetworkID= '%s'\n", ifname, argv[3]);

			help_addObjList(wlObj, "Device.WiFi.EndPoint", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint", "X_LANTIQ_COM_Vendor_NetworkId", argv[3], 0, 0);

			if (fapi_wlan_client_mode_network_remove(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_client_mode_network_remove ERROR\n");
		}
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_SCAN_START"))
	{
		/*
		Format:  CLIENT_MODE_SCAN_START ifname
		Example: CLIENT_MODE_SCAN_START wlan3
		*/

		if (argc > 2)
		{
			printf("fapi_wlan_client_mode_scan_start test; ifname= '%s'\n", ifname);

			if (fapi_wlan_client_mode_scan_start(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_client_mode_scan_start ERROR\n");
		}
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_SCAN_RESULTS_GET"))
	{
		/*
		Format:  CLIENT_MODE_SCAN_RESULTS_GET ifname
		Example: CLIENT_MODE_SCAN_RESULTS_GET wlan3
		*/

		if (argc > 2)
		{
			printf("fapi_wlan_client_mode_scan_results_get test; ifname= '%s'\n", ifname);

			if (fapi_wlan_client_mode_scan_results_get(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_client_mode_scan_results_get ERROR\n");
			else
				setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_CONNECTION_STATUS_GET"))
	{
		/*
		Format:  CLIENT_MODE_CONNECTION_STATUS_GET ifname
		Example: CLIENT_MODE_CONNECTION_STATUS_GET wlan3
		*/

		if (argc > 2)
		{
			printf("fapi_wlan_client_mode_connection_status_get test; ifname= '%s'\n", ifname);

			if (fapi_wlan_client_mode_connection_status_get(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_client_mode_connection_status_get ERROR\n");
			else
				setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_DATA_ROUTING_MODE_SET"))
	{
		/*
		Format:   CLIENT_MODE_DATA_ROUTING_MODE_SET ifname mode
		Examples: CLIENT_MODE_DATA_ROUTING_MODE_SET wlan3 3_ADDRESS_MODE
		          CLIENT_MODE_DATA_ROUTING_MODE_SET wlan3 4_ADDRESS_MODE
		*/

		if (argc > 3)
		{
			printf("fapi_wlan_client_mode_data_routing_mode_set test; ifname= '%s', mode= '%s'\n", ifname, argv[3]);

			help_addObjList(wlObj, "Device.WiFi.EndPoint.Profile", 0, 0, 0, 0);
			HELP_EDIT_NODE(wlObj, "Device.WiFi.EndPoint.Profile", "X_LANTIQ_COM_Vendor_RoutingMode", argv[3], 0, 0);

			if (fapi_wlan_client_mode_data_routing_mode_set(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_client_mode_data_routing_mode_set ERROR\n");
		}
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_DATA_ROUTING_MODE_GET"))
	{
		/*
		Format:  CLIENT_MODE_DATA_ROUTING_MODE_GET ifname
		Example: CLIENT_MODE_DATA_ROUTING_MODE_GET wlan3
		*/

		if (argc > 2)
		{
			printf("fapi_wlan_client_mode_data_routing_mode_get test; ifname= '%s'\n", ifname);

			if (fapi_wlan_client_mode_data_routing_mode_get(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_client_mode_data_routing_mode_get ERROR\n");
			else
				setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else if (!strcmp(argv[1], "CLIENT_MODE_RADIO_INFO_GET"))
	{
		/*
		Format:  CLIENT_MODE_RADIO_INFO_GET ifname
		Example: CLIENT_MODE_RADIO_INFO_GET wlan3
		*/

		if (argc > 2)
		{
			printf("fapi_wlan_client_mode_radio_info_get test; ifname= '%s'\n", ifname);

			if (fapi_wlan_client_mode_radio_info_get(ifname, wlObj, 0) == UGW_FAILURE)
				printf("fapi_wlan_client_mode_radio_info_get ERROR\n");
			else
				setLog(ifname, wlObj, strtol(argv[1], NULL, 10));
		}
	}
	else
	{
		printf("'%s' command is not supported!\n", argv[1]);
	}

	HELP_DELETE_OBJ(wlObj, SOPT_OBJVALUE, FREE_OBJLIST);
}


static int wpaCtrlReconnectHandle(const char *ifname, struct wpa_ctrl *wpaSocket, int highestValFD)
{
	int fd, count = 0;
	FapiWlanCallBack fapiWlanCallBackFuncToUse = fapiWlanCallBackLocalFunc;

	/* Notify the client that the WLAN has disconnected */
	if (_isOneShot)
	{
		printf("%s; Special Case: hostapd try reconnecting, using one-shot mode (_isOneShot= %d)\n", __FUNCTION__, _isOneShot);
		fapiWlanCallBackFuncToUse = fapiWlanCallBackApManagerLiteFunc;
	}

	fapiWlanCallBackFuncToUse("WLAN-DISCONNECTED", ifname, NULL, 0, NULL);

	/* wait for the hostapd - it removes the fd */
	sleep(1);

	fapi_wlan_stop_monitoring(ifname, NULL, 0);

	while (1)
	{
		if (fapi_wlan_start_monitoring(ifname, NULL, 1) == UGW_SUCCESS)
		{
			printf("%s; connection i/f is back! (ifname= '%s')\n", __FUNCTION__, ifname);
			break;
		}

		usleep (350);

		if (count++ >= 5000)
		{
			printf("%s; trying to reconnect (ifname= '%s')\n", __FUNCTION__, ifname);
			count = 0;
		}
	}

	if (fapi_wlan_hostapd_socket_get(ifname, (void**)&wpaSocket) == UGW_FAILURE)
		printf("%s; ERROR: fapi_wlan_hostapd_socket_get on '%s' failed!\n", __FUNCTION__, ifname);
	else if (wpa_ctrl_attach(wpaSocket) != 0)
	{
		printf("%s; ERROR: wpa_ctrl_attach for '%s' failed!\n", __FUNCTION__, ifname);
	}
	else
	{
		fd = wpa_ctrl_get_fd(wpaSocket);
		highestValFD = (fd > highestValFD)? fd : highestValFD;  /* find the highest value fd */
	}

	if (fapi_wlan_hostapd_callback_register(ifname, fapiWlanCallBackFuncToUse, NULL) == UGW_FAILURE)
		printf("fapi_wlan_hostapd_callback_register AP-Manager-Lite ERROR (ifname= '%s')\n", ifname);

	/* Notify the client that the WLAN has reconnected */
	printf("%s; call fapiWlanCallBackFuncToUse with 'WLAN-RECONNECTED' (ifname= '%s', _isOneShot= %d)\n", __FUNCTION__, ifname, _isOneShot);
	fapiWlanCallBackFuncToUse("WLAN-RECONNECTED", ifname, NULL, 0, NULL);
	
	return highestValFD;
}


static void fapiBeerokCliStart(bool isOneShot, char *command)
{
	size_t  numOfInterfaces = sizeof(radioInterface) / sizeof(RadioInterface);
	int     i, interfaceIdx, highestValFD = 0, fd[numOfInterfaces], res, numOfActiveInterfaces = 0, res_wpa_ctrl;
	char    *interfaces[numOfInterfaces];
	char    fullPathGeneral[MAX_LEN_PARAM_VALUE];
	char    *priorityOwnBSS, *opClass;
	fd_set  rfds;
	bool    isCloseListeners = false, isCloseListenersExists = false, isKeyboardEventActivate = true;
	struct  wpa_ctrl *wpaSocket[numOfInterfaces];
	ObjList *wlObjGeneralDB;

	printf("%s; START; isOneShot= %d, command= '%s'\n", __FUNCTION__, isOneShot, command);

	_isOneShot = isOneShot;  /* save this for case of hostapd reconnect */

    /* Init signals */
    init_signals();

    /* Read history file */
    read_history("/tmp/fapi_beerock_cli_history");

	/* Update the data-base with the info regarding of the active radio interfaces */
	radioInterfaceDataBaseUpdate();

	for (i=0; i < (int)numOfInterfaces; i++)
	{
		if (!strcmp(radioInterface[i].operationMode, "NONE"))
		{
			printf("%s; radio interface '%s' not present ==> Continue\n", __FUNCTION__, radioInterface[i].name);
			continue;
		}

		printf("%s; supportedInterfaces[%d]= '%s'\n", __FUNCTION__, i, radioInterface[i].name);

		/* register for all notifications */
		if (isOneShot)
		{
			if (command == NULL)
			{  /* handle the listener start */
				char BSSID[]   = "00:11:22:33:44:55";
				char channel[] = "1234";

				printf("%s; fapi_wlan_hostapd_callback_register (ifname= '%s') ==> register to AP-Manager-Lite call-back\n", __FUNCTION__, radioInterface[i].name);
				if (fapi_wlan_hostapd_callback_register(radioInterface[i].name, fapiWlanCallBackApManagerLiteFunc, NULL) == UGW_FAILURE)
					printf("fapi_wlan_hostapd_callback_register AP-Manager-Lite ERROR (ifname= '%s')\n", radioInterface[i].name);

				/* Create internal data-base for AP-Manager-Lite usage (if not present!) */
				sprintf(fullPathGeneral, "/usr/lib/%s_%s", "BeeRock.General.db", radioInterface[i].name);
				if (!file_exists(fullPathGeneral))
				{
					printf("%s; file '%s' does NOT exist ==> create it!\n", __FUNCTION__, fullPathGeneral);

					wlObjGeneralDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
					if (wlObjGeneralDB == NULL)
					{
						printf("%s; create General-DB object ERROR\n", __FUNCTION__);
						return;
					}

					help_addObjList(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", 0, 0, 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "ssid", "\"test_widan\"", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSIDInfo", "00000000", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "BSSIDInfoOwnBSS", "00000000", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "PhyType", "09", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "reassocDelay", "2", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssTermDuration", "1", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssTermTSF", "36000", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "pref", "1", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "disassocImminent", "0", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "disassocTimer", "20", 0, 0);  /* 20 * 100ms = 2 sec */
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "cellPref", "1", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "priority", "1", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "priorityOwnBSS", "255", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "bssTerminationTimer", "50", 0, 0);  /* 50 * 100ms = 5 sec */
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "reason", "4", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "opClass", "115", 0, 0);
					HELP_EDIT_NODE(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "validityInterval", "15", 0, 0);

					/* write the data into the data-base */
					if (HELP_STORELOCALDB(wlObjGeneralDB, fullPathGeneral) == UGW_FAILURE)
					{
						printf("%s; HELP_STORELOCALDB Error!\n", __FUNCTION__);
						return;
					}

					HELP_DELETE_OBJ(wlObjGeneralDB, SOPT_OBJVALUE, FREE_OBJLIST);
				}
				else
					printf("%s; file '%s' exists ==> do not update it!\n", __FUNCTION__, fullPathGeneral);

				/* Create Own-BSS data-base for AP-Manager-Lite usage, ONLY for "wlan1" */
				if ( (!strcmp(radioInterface[i].name, "wlan0")) || (!strcmp(radioInterface[i].name, "wlan2")) )
				{
					printf("%s; Create Own-BSS data-base\n", __FUNCTION__);

					wlObjGeneralDB = HELP_CREATE_OBJ(SOPT_OBJVALUE);
					if (wlObjGeneralDB == NULL)
					{
						printf("%s; create General-DB object ERROR ==> Abort!\n", __FUNCTION__);
						return;
					}

					if (HELP_LOADLOCALDB(wlObjGeneralDB, fullPathGeneral) == UGW_FAILURE)
					{
						printf("%s; HELP_LOADLOCALDB ==> file '%s' not found!\n", __FUNCTION__, fullPathGeneral);
					}

					printf("%s; Stored Data Base Object ('%s'):\n", __FUNCTION__, fullPathGeneral);
					setLog(radioInterface[i].name, wlObjGeneralDB, 0);

					opClass = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "opClass");
					priorityOwnBSS = paramValueFromObjGet(wlObjGeneralDB, "Device.WiFi.Radio.X_LANTIQ_COM_Vendor", "priorityOwnBSS");
					printf("%s; opClass= '%s', priorityOwnBSS= '%s'\n", __FUNCTION__, opClass, priorityOwnBSS);

					if (bssidGet(radioInterface[i].name, BSSID) == UGW_FAILURE)
					{
						printf("%s; bssidGet ERROR ==> Abort!\n", __FUNCTION__);
						return;
					}

					if (channelNumberGet(radioInterface[i].name, channel) == UGW_FAILURE)
					{
						printf("%s; channelNumberGet ERROR ==> Abort!\n", __FUNCTION__);
						return;
					}

					printf("%s; BSSID= '%s', channel= '%s'\n", __FUNCTION__, BSSID, channel);
					//ap_manager_lite_btm_params_set();  /* Use parameters from the above general data-base */
					if (ap_manager_lite_bss_neighbor_set(radioInterface[i].name, BSSID, opClass, channel, priorityOwnBSS, NULL, NULL) == UGW_FAILURE)
					{
						printf("ap_manager_lite_bss_neighbor_set ERROR\n");
					}
				}
			}
		}
		else if (fapi_wlan_hostapd_callback_register(radioInterface[i].name, fapiWlanCallBackLocalFunc, NULL) == UGW_FAILURE)
			printf("fapi_wlan_hostapd_callback_register ERROR (ifname= '%s')\n", radioInterface[i].name);

		if (fapi_wlan_start_monitoring(radioInterface[i].name, NULL, 0) == UGW_FAILURE)
			printf("fapi_wlan_start_monitoring ERROR (ifname= '%s')\n", radioInterface[i].name);

		/* Read the hostapd socket fd */
		wpaSocket[i] = NULL;  /* reset the Socket */
		if (fapi_wlan_hostapd_socket_get(radioInterface[i].name, (void**)&wpaSocket[i]) == UGW_FAILURE)
		{
			printf("%s; ERROR: fapi_wlan_hostapd_socket_get on '%s' failed!\n", __FUNCTION__, radioInterface[i].name);
		}
		else if (wpa_ctrl_attach(wpaSocket[i]) != 0)
		{
			printf("%s; ERROR: wpa_ctrl_attach for '%s' failed!\n", __FUNCTION__, radioInterface[i].name);
		}
		else
		{
			fd[numOfActiveInterfaces] = wpa_ctrl_get_fd(wpaSocket[i]);
			highestValFD = (fd[numOfActiveInterfaces] > highestValFD)? fd[numOfActiveInterfaces] : highestValFD;  /* find the highest value fd */
			interfaces[numOfActiveInterfaces++] = radioInterface[i].name;  /* store the ifname vs. its index */
		}
	}

	if (numOfActiveInterfaces == 0)
	{
		printf("\nBeeRock CLI: No active interfaces! ==> Quit BeeRock CLI\n");
		isCliRunning = false;
	}
	else
	{
		if (isOneShot)
		{
			if (command == NULL)
			{  /* handle the listener start */
				isKeyboardEventActivate = false;
			}
			else
			{  /* handle one-shot command */
				isCliRunning = false;
				system("touch /tmp/isCloseListeners");  /* turn off all listeners */
				fapi_beerock_cli_readline_callback(command);
				system("rm /tmp/isCloseListeners");  /* turn back on all listeners */
			}
		}
		else
		{
			/* Readline completion function */
			rl_completion_entry_function = fapi_beerock_cli_tab_completion_entry;
			rl_attempted_completion_function = fapi_beerock_cli_tab_completion;

			/* Enable TAB auto-complete */
			rl_bind_key('\t', rl_complete);

			/* Register readline handler */
			rl_callback_handler_install("(BeeRocks)>> ", fapi_beerock_cli_readline_callback);
		}
	}

	/* Main event loop */
	while (isCliRunning)
	{
		//printf("%s; [0]\n", __FUNCTION__);
		FD_ZERO(&rfds);
		if (!isOneShot)
		{
			FD_SET(STDIN_FILENO, &rfds);
		}

		for (i=0; i < numOfActiveInterfaces; i++)
		{
			FD_SET(fd[i], &rfds);
		}

		res = select(highestValFD + 1, &rfds, NULL, NULL, NULL);
		if (res < 0)
		{
			printf("%s; select() return value= %d ==> CONTINUE!!!\n", __FUNCTION__, res);
			continue;
		}

		//printf("%s; call 'continue' ==> CONTINUE!!!\n", __FUNCTION__, res);
		//continue;
		//printf("%s; [1]\n", __FUNCTION__);

		isCloseListenersExists = (access("/tmp/isCloseListeners", F_OK) == 0)? true : false;
		if (isCloseListeners || isCloseListenersExists)
		{
			usleep(10);
			continue;
		}

		if (isKeyboardEventActivate)
		{  /* stdin (keyboard event) */
			if (FD_ISSET(STDIN_FILENO, &rfds))
			{
				rl_callback_read_char();
			}
		}

		for (i=0; i < numOfActiveInterfaces; i++)
		{
			if (FD_ISSET(fd[i], &rfds))
			{
				//printf("%s; *** [%s] FAPI RECEIVED FROM HOSTAPD SOMETHING!!! ***\n", __FUNCTION__, interfaces[i]);
				interfaceIdx = interfaceIndexGet(interfaces[i]);
				if (interfaceIdx == (-1))
				{
					printf("ERROR: interfaceIdx of '%s' is %d ==> Skip this interface\n", interfaces[i], interfaceIdx);
					continue;
				}

				while (1)
				{
					res_wpa_ctrl = wpa_ctrl_pending(wpaSocket[interfaceIdx]);
					if (res_wpa_ctrl != 1)
						break;  /* quit the 'while' loop */

					res = fapi_wlan_hostapd_report_process(interfaces[i], NULL, NULL);
					if (res == UGW_FAILURE)
						printf("ERROR: fapi_wlan_hostapd_report_process failed!\n");
					else if (res == WPA_CTRL_DISCONNECT)
					{  /* handle hostapd recover mechanism */
						printf("wpa-ctrl disconnected - try reconnecting...\n");
						isCloseListeners = true;  /* turn off all listeners */
						highestValFD = wpaCtrlReconnectHandle(interfaces[i], wpaSocket[interfaceIdx], highestValFD);

						i = numOfActiveInterfaces;  /* quit the 'for' loop */
						isCloseListeners = false;  /* turn back on all listeners */
						break;  /* quit the 'while' loop */
					}
				}

				if ( res_wpa_ctrl == (-1) )
				{  /* ERROR - issue a trace */
					printf("wpa_ctrl_pending() returned ERROR\n");
				}
			}
		}

		//printf("%s; [2]\n", __FUNCTION__);
	}

	printf("\n");

    /* Save history file */
	if (!isOneShot)
	{
		write_history("/tmp/fapi_beerock_cli_history");
	}

	/* Cleanup */
	printf("FAPI BeeRock CLI cleanup...\n");
	rl_callback_handler_remove();

	for (i=0; i < numOfActiveInterfaces; i++)
	{
		interfaceIdx = interfaceIndexGet(interfaces[i]);
		if (interfaceIdx == (-1))
		{
			printf("ERROR: interfaceIdx of '%s' is %d ==> Skip this interface\n", interfaces[i], interfaceIdx);
			continue;
		}

		if (wpaSocket[interfaceIdx] != NULL)
			wpa_ctrl_detach(wpaSocket[interfaceIdx]);

		fapi_wlan_stop_monitoring(interfaces[i], NULL, 0);
	}

	printf("Bye!\n");
}


static void fapiWlanRefSpecialCallBackFunc(char *buf, size_t len)
{
	char *ifname;
	char *localMsg = strdup(buf);

	printf("%s; len= %d, buf= '%s'\n", __FUNCTION__, len, buf);

	if (localMsg == NULL)
	{
		printf("%s; strdup() failed ==> ABORT!\n", __FUNCTION__);
		return;
	}

	ifname = strtok(localMsg, " ");  /* <X>opCode */
	ifname = strtok(NULL, " ");  /* iface */

	fapiWlanRefCallBackFunc(ifname, buf);

	free((void *)localMsg);
}


static void fapiWlanCommandSend(char *ifname, char *command)
{
	int    ret;
	char   *localBuf;
	int    idx = interfaceIndexGet(ifname);
	bool   isWpaSocketRelease = false;
	size_t len = (4096 * 3) * sizeof(char);
	size_t numOfInterfaces = sizeof(radioInterface) / sizeof(RadioInterface);
	struct wpa_ctrl *wpaSocket[numOfInterfaces];

	printf("%s Entry; ifname= '%s', command= '%s'\n", __FUNCTION__, ifname, command);

	if (idx == (-1))
	{
		printf("%s; Illegal interface index (%d) ==> ABORT!\n", __FUNCTION__, idx);
		return;
	}

	if ((command == NULL) || (ifname == NULL))
	{
		printf("%s; Input parameters ERROR (command= '%s', ifname= '%s') ==> ABORT!\n", __FUNCTION__, command, ifname);
		return;
	}

	wpaSocket[idx] = NULL;  /* reset the Socket */

	/* Use wpa_ctrl_open() instead of fapi_wlan_start_monitoring() and fapi_wlan_hostapd_socket_get() */
	if (radioInterface[idx].wpaSocket == NULL)
	{
		radioInterface[idx].wpaSocket = wpa_ctrl_open(radioInterface[idx].wpaCtrlName);
		isWpaSocketRelease = true;
	}

	wpaSocket[idx] = radioInterface[idx].wpaSocket;
	if (wpaSocket[idx] == NULL)
		printf("%s; ERROR: fapi_wlan_hostapd_socket_get on '%s' failed!\n", __FUNCTION__, radioInterface[idx].name);

	if (wpaSocket[idx] == NULL)
	{
		printf("%s; Input parameters ERROR - non supported ifname ('%s') ==> ABORT!\n", __FUNCTION__, ifname);
		return;
	}

	localBuf = (char *)malloc((size_t)((4096 * 3) * sizeof(char)));
	if (localBuf == NULL)
	{
		printf("%s; malloc error ==> ABORT!\n", __FUNCTION__);
		return;
	}

	memset((void *)localBuf, 0, (size_t)len);  /* Clear the output buffer */

	ret = wpa_ctrl_request(wpaSocket[idx], command, strlen(command), localBuf, &len, fapiWlanRefSpecialCallBackFunc);
	/* In case the return value is not OK, we needed to recover the connection, as we do after hostapd reset */

	if (isWpaSocketRelease)
	{
		if (wpaSocket[idx] != NULL)
		{
			wpa_ctrl_detach(wpaSocket[idx]);
			wpa_ctrl_close(wpaSocket[idx]);
			wpaSocket[idx] = NULL;
		}
	}

	if (ret < 0)
	{
		free((void *)localBuf);
		return;
	}

	printf("%s; localBuf:\n%.*s\n", __FUNCTION__, len, localBuf);

	free((void *)localBuf);
}


static void fapiHostapdInterfaceSet(void)
{
	size_t  numOfInterfaces = sizeof(radioInterface) / sizeof(RadioInterface);
	int     i, interfaceIdx, highestValFD = 0, fd[numOfInterfaces], res, numOfActiveInterfaces = 0, res_wpa_ctrl;
	char    *interfaces[numOfInterfaces], *buf;
	fd_set  rfds;
	size_t  len = (4096 * 3) * sizeof(char);
	struct  wpa_ctrl *wpaSocket[numOfInterfaces];
	struct  timeval tv;

	printf("%s Entry\n", __FUNCTION__);

	/* Update the data-base with the info regarding of the active radio interfaces */
	radioInterfaceDataBaseUpdate();

	for (i=0; i < (int)numOfInterfaces; i++)
	{
		if (!strcmp(radioInterface[i].operationMode, "NONE"))
		{
			printf("%s; radio interface '%s' not present ==> Continue\n", __FUNCTION__, radioInterface[i].name);
			continue;
		}

		printf("%s; supportedInterfaces[%d]= '%s'\n", __FUNCTION__, i, radioInterface[i].name);

		/* Use wpa_ctrl_open() instead of fapi_wlan_start_monitoring() and fapi_wlan_hostapd_socket_get() */
		if (radioInterface[i].wpaSocket == NULL)
			radioInterface[i].wpaSocket = wpa_ctrl_open(radioInterface[i].wpaCtrlName);

		wpaSocket[i] = radioInterface[i].wpaSocket;
		if (wpaSocket[i] == NULL)
			printf("%s; ERROR: fapi_wlan_hostapd_socket_get on '%s' failed!\n", __FUNCTION__, radioInterface[i].name);
		else if (wpa_ctrl_attach(wpaSocket[i]) != 0)
		{
			printf("%s; ERROR: wpa_ctrl_attach for '%s' failed!\n", __FUNCTION__, radioInterface[i].name);
		}
		else
		{
			fd[numOfActiveInterfaces] = wpa_ctrl_get_fd(wpaSocket[i]);
			highestValFD = (fd[numOfActiveInterfaces] > highestValFD)? fd[numOfActiveInterfaces] : highestValFD;  /* find the highest value fd */
			interfaces[numOfActiveInterfaces++] = radioInterface[i].name;  /* store the ifname vs. its index */
		}
	}

	if (numOfActiveInterfaces == 0)
	{
		printf("%s; \nBeeRock CLI: No active interfaces! ==> Quit the app.\n", __FUNCTION__);
		return;
	}

	buf = (char *)malloc((size_t)((4096 * 3) * sizeof(char)));
	if (buf == NULL)
	{
		printf("%s; malloc error ==> ABORT!\n", __FUNCTION__);
		return;
	}

	/* Main event loop */
	while (true)
	{
		for (i=0; i < numOfActiveInterfaces; i++)
		{
			FD_SET(fd[i], &rfds);
		}

		/* Interval of time in which the select() will be released */
		tv.tv_sec = 5;
		tv.tv_usec = 0;

		res = select(highestValFD + 1, &rfds, NULL, NULL, &tv);
		if (res < 0)
		{
			printf("%s; select() return value= %d ==> CONTINUE!!!\n", __FUNCTION__, res);
			continue;
		}

		/* From here (we will get here every 5 seconds or whenever an event will be received), it is possible to send commands to the hostapd, using fapiWlanCommandSend() */

		for (i=0; i < numOfActiveInterfaces; i++)
		{
			if (FD_ISSET(fd[i], &rfds))
			{
				//printf("%s; *** [%s] FAPI RECEIVED FROM HOSTAPD SOMETHING!!! ***\n", __FUNCTION__, interfaces[i]);
				interfaceIdx = interfaceIndexGet(interfaces[i]);
				if (interfaceIdx == (-1))
				{
					printf("ERROR: interfaceIdx of '%s' is %d ==> Skip this interface\n", interfaces[i], interfaceIdx);
					continue;
				}

				while (true)
				{
					res_wpa_ctrl = wpa_ctrl_pending(wpaSocket[interfaceIdx]);
					if (res_wpa_ctrl != 1)
						break;  /* quit the 'while' loop */

					memset(buf, 0, (4096 * 3) * sizeof(char));  /* Clear the output buffer */
					if (wpa_ctrl_recv(wpaSocket[interfaceIdx], buf, &len) == 0)
					{
						printf("%s; len= %d\nbuf= '%s'\n", __FUNCTION__, len, buf);
						if (len <= 5)
						{
							printf("%s; '%s' is NOT a report - continue!\n", __FUNCTION__, buf);
						}
						else
						{
							fapiWlanRefCallBackFunc(interfaces[i], buf);
						}
					}
				}

				if ( res_wpa_ctrl == (-1) )
				{  /* ERROR - issue a trace */
					printf("wpa_ctrl_pending() returned ERROR\n");
				}
			}
		}
	}

	/* Cleanup */
	printf("\nFAPI BeeRock CLI cleanup...\n");
	free((void *)buf);

	for (i=0; i < numOfActiveInterfaces; i++)
	{
		interfaceIdx = interfaceIndexGet(interfaces[i]);
		if (interfaceIdx == (-1))
		{
			printf("ERROR: interfaceIdx of '%s' is %d ==> Skip this interface\n", interfaces[i], interfaceIdx);
			continue;
		}

		if (wpaSocket[interfaceIdx] != NULL)
		{
			wpa_ctrl_detach(wpaSocket[interfaceIdx]);
			wpa_ctrl_close(wpaSocket[interfaceIdx]);
			wpaSocket[interfaceIdx] = NULL;
		}
	}

	printf("Bye!\n");
}


#if 0
static void concutenatedBlankRemove(char *buf)
{
	int i, k, numOfBlanks;

	for (i=0; i < (int)(strlen(buf)+1); i++)
	{
		if (buf[i] == ' ')
		{  /* 1st blank found */
			numOfBlanks = 1;
			
			while ((buf[i + numOfBlanks] == ' ') && ((i + numOfBlanks) < (int)(strlen(buf) + 1)))
			{
				numOfBlanks++;
			}

			if (numOfBlanks == 1)
			{  /* no more then one blank in a row ==> continue */
				continue;
			}

			/* "shrink" the buffer */
			for (k = i + 1; k < (int)(strlen(buf) + 1); k++)
			{
				buf[k] = buf[k + numOfBlanks - 1];
			}
		}
	}
}
#endif


int main(int argc, char *argv[])
{
	int i;

	printf("FAPI BeeRock CLI Function Entry; argc= %d, argv[0]= '%s', argv[1]= '%s', argv[2]= '%s'\n", argc, argv[0], argv[1], argv[2]);

	/* Load the mapper from the data-base */
	if (!mapperObjPtr)
		fapiWlanFailSafeLoad();

	if (argc > 1)
	{
/*****************************************/
/* 1) AP Manager to FAPI - API Functions */
/*****************************************/
/*************/
/* 1.1) Init */
/*************/
		/* Handle the INIT phase */
		if (!strcmp(argv[1], "INIT"))
		{
			size_t numOfInterfaces = sizeof(radioInterface) / sizeof(RadioInterface);
			char   wpaCtrlName[32];

			for (i=0; i < (int)numOfInterfaces; i++)
			{
				/* check if '/var/run/hostapd/wlanX' or '/var/run/wpa_supplicant/wlanX' exists */
				sprintf(wpaCtrlName, "%s%s", "/var/run/hostapd/", radioInterface[i].name);
				if (access(wpaCtrlName, F_OK) == 0)
				{  /* hostapd, meaning AP interface */
					printf("%s; INIT; ifname= '%s'\n", __FUNCTION__, radioInterface[i].name);

					if (ap_manager_init(radioInterface[i].name) == UGW_FAILURE)
						printf("ap_manager_init ERROR\n");
				}
			}
		}

/**************************************/
/* 2) HostAPD to FAPI - API Functions */
/**************************************/
		/* Handle COMMAND ("one shot" mode) */
		else if (!strcmp(argv[1], "COMMAND"))
		{
			/*
			Format:   ./fapi_wlan_beerock_cli COMMAND 'command ...'
			Examples: ./fapi_wlan_beerock_cli COMMAND 'FAILSAFE_CHANNEL_SET wlan1 5745 5775 80'
					  ./fapi_wlan_beerock_cli COMMAND 'FAILSAFE_CHANNEL_SET wlan1 0'
					  ./fapi_wlan_beerock_cli COMMAND 'ACS_REPORT_GET wlan0'
					  ./fapi_wlan_beerock_cli COMMAND "RESTRICTED_CHANNELS_SET wlan0 '1 6 11-13'"
					  ./fapi_wlan_beerock_cli COMMAND "BEACON_REQ wlan0 44:85:00:C5:6A:1B 0 0 0 255 1000 50 passive 00:0A:1B:0E:04:60 beacon_rep=1,123 rep_detail=2 'ssid=\"AA BB\"' ap_ch_report=1,2,3 req_elements=0,51 wide_band_ch_switch=1,1,1"
					  ./fapi_wlan_beerock_cli COMMAND 'AML_BSS_NEIGHBOR_SET wlan2 55:44:33:22:11:00 opClass=51 channelNumber=1 priority=1 oce=1 tbtt_offset=1'
					  ./fapi_wlan_beerock_cli COMMAND 'AML_BSS_NEIGHBOR_SET wlan2 55:44:33:22:11:00 opClass=51 channelNumber=1 priority=1'
					  ./fapi_wlan_beerock_cli COMMAND "AML_BEACON_REQ_GET wlan1 44:85:00:C5:6A:1B numOfRepetitions=0 durationMandatory=0 opClass=0 Channel=1 randInt=1000 duration=50 mode=passive bssid=00:0A:1B:0E:04:60 'ssid=widan test' rep_detail=2 rep_cond=0 ap_ch_report=1,2,3 req_elements=0,51"
					  ./fapi_wlan_beerock_cli COMMAND 'MAC_ADDRESS_GET wlan0'
					  ./fapi_wlan_beerock_cli COMMAND 'AML_ASSOC_DISALLOW wlan1 1'
					  ./fapi_wlan_beerock_cli COMMAND 'AML_ASSOC_DISALLOW wlan0 0'
					  ./fapi_wlan_beerock_cli COMMAND 'AML_BTM_REQ wlan0 24:77:03:80:5d:90 00:0A:1B:0E:04:60 pref=1 reason=5 disassocTimer=1'
					  ./fapi_wlan_beerock_cli COMMAND 'AML_BTM_PARAMS_SET wlan0 bssTermination=2,3 disassocImminent=1 disassocTimer=1 reassocDelay=2'
					  ./fapi_wlan_beerock_cli COMMAND "GENERAL_COMMAND wlan0 'REQ_BEACON 44:85:00:C5:6A:1B 0 0 0 255 1000 50 passive 00:0A:1B:0E:04:60 beacon_rep=1,123 rep_detail=2'"
					  ./fapi_wlan_beerock_cli COMMAND "CLIENT_MODE_NETWORK_SET wlan1 0 ModeEnabled=None 'ssid=\"test_ssid\"'"
			*/
			if (argc > 2)
			{
				fapiBeerokCliStart(true, argv[2]);
			}
			else
			{
				printf("COMMAND Error ==> Exit!\n");
			}
		}
		/* Handle listener (for "one shot" mode) */
		else if (!strcmp(argv[1], "HOSTAPD_RECV"))
		{
			/*
			Format: ./fapi_wlan_beerock_cli HOSTAPD_RECV &
			*/
			printf("%s; (HOSTAPD_RECV) clean temporary AP-Manager-Lite data-base from '/tmp/wlan_wave'\n", __FUNCTION__);
			system("rm -rf /tmp/wlan_wave/Bee*");
			system("rm /tmp/isCloseListeners");  /* in case it was off, turn back on all listeners */

			fapiBeerokCliStart(true, NULL);
		}
		/* Handle START */
		else if (!strcmp(argv[1], "START"))
		{
			fapiBeerokCliStart(false, NULL);
		}
		/* Handle Band-Steering */
		else if (!strcmp(argv[1], "BAND_STEERING"))
		{
			/*
			Format: ./fapi_wlan_beerock_cli BAND_STEERING &
			        ./fapi_wlan_beerock_cli BAND_STEERING -30 -70 5 10 15 &
			*/

			int signalStrengthThreshold_2_4 = -40, signalStrengthThreshold_5 = -60, intervalInSeconds = 5, toleranceInSeconds = 15, numOfTicksAllowedForSteering = 15;

			if (argc > 5)
			{
				signalStrengthThreshold_2_4 = atoi(argv[2]);
				signalStrengthThreshold_5   = atoi(argv[3]);
				intervalInSeconds           = atoi(argv[4]);
				toleranceInSeconds          = atoi(argv[5]);

				if (argc > 6)
				{
					numOfTicksAllowedForSteering = atoi(argv[6]);
				}
				else
				{
					printf("%s; BAND_STEERING; last parameter (numOfTicksAllowedForSteering) is missing ==> use default (%d) value\n", __FUNCTION__, numOfTicksAllowedForSteering);
				}
			}
			else
			{
				printf("%s; BAND_STEERING; 4 mandatory parameters are not present ==> use default values:\n", __FUNCTION__);
			}


			printf("%s; BAND_STEERING; signalStrengthThreshold_2_4= %d, signalStrengthThreshold_5= %d, intervalInSeconds= %d, toleranceInSeconds= %d, numOfTicksAllowedForSteering= %d\n",
			       __FUNCTION__, signalStrengthThreshold_2_4, signalStrengthThreshold_5, intervalInSeconds, toleranceInSeconds, numOfTicksAllowedForSteering);

			fapiBandSteeringAppStart(signalStrengthThreshold_2_4, signalStrengthThreshold_5, intervalInSeconds, toleranceInSeconds, numOfTicksAllowedForSteering);
		}
		/* Handle generic init function */
		else if (!strcmp(argv[1], "INIT_REF"))
		{
			/*
			Format: ./fapi_wlan_beerock_cli INIT_REF &
			*/

			printf("%s; INIT_REF\n", __FUNCTION__);

			fapiHostapdInterfaceSet();
		}
		/* Handle COMMAND_REF */
		else if (!strcmp(argv[1], "COMMAND_REF"))
		{
			/*
			Format: ./fapi_wlan_beerock_cli COMMAND_REF ifname 'command'

			Examples:
			./fapi_wlan_beerock_cli COMMAND_REF wlan0 'ENABLE'
			./fapi_wlan_beerock_cli COMMAND_REF wlan0 'GET_ACS_REPORT'
			./fapi_wlan_beerock_cli COMMAND_REF wlan1 'STA_STEER 8c:3a:e3:45:d1:ae 00:0A:1B:0E:04:70'
			./fapi_wlan_beerock_cli COMMAND_REF wlan0 'REQ_BEACON 44:85:00:C5:6A:1B 0 0 0 255 1000 50 passive 00:0A:1B:0E:04:60 beacon_rep=1,123 rep_detail=2 ssid="AA BB" ap_ch_report=1,2,3 req_elements=0,51 wide_band_ch_switch=1,1,1'
			./fapi_wlan_beerock_cli COMMAND_REF wlan0 'REQ_SELF_BEACON 1000 50 passive'
			./fapi_wlan_beerock_cli COMMAND_REF wlan0 'RESTRICTED_CHANNELS 1 6 11-13'
			*/

			printf("%s; COMMAND_REF\n", __FUNCTION__);

			if (argc <= 3)
			{
				printf("%s; COMMAND_REF - wrong format\n", __FUNCTION__);
			}
			else
			{
				printf("%s; COMMAND_REF test; ifname= '%s', Command= '%s'\n", __FUNCTION__, argv[2], argv[3]);

				/* Update the data-base with the info regarding of the active radio interfaces */
				radioInterfaceDataBaseUpdate();

				fapiWlanCommandSend(argv[2], argv[3]);
			}
		}
		else
		{
			printf("FAPI CTRL I/F Function ==> ERROR!\n");
		}
	}

	return UGW_SUCCESS;
}
