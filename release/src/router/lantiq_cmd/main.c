#include <stdio.h>
#include <fapi_wlan_private.h>
#include <fapi_wlan.h>
#include <help_objlist.h>
#include <wlan_config_api.h>
#include <wpa_ctrl.h>
     #include <sys/socket.h>
       #include <sys/un.h>
#define HOSTAPD_TO_FAPI_MSG_LENGTH              (4096 * 3)
typedef int(*FapiWlanGenericSetNativeFunc)(int index, ObjList *wlObj, unsigned int flags);

int wl_wave_unit(int unit);
//#include "lantiq_common.h"
int fapi_wlan_generic_set_native2(FapiWlanGenericSetNativeFunc fapiWlanGenericSetNativeFunc,
	int index,
	ObjList *wlObj,
	char *dbCliFileName,
	unsigned int flags)
{
	if (wlanLoadFromDB(dbCliFileName, "", wlObj) == UGW_SUCCESS)
	{
		//printf("%s %d\n",__FUNCTION__,__LINE__);
		setLog("wlan0", wlObj, 0);
		//printf("%s %d\n",__FUNCTION__,__LINE__);
		if (fapiWlanGenericSetNativeFunc(index, wlObj, flags) == UGW_SUCCESS)
		{
			//printf("%s %d\n",__FUNCTION__,__LINE__);
			return UGW_SUCCESS;
		}
		else
		{
			//printf("%s %d\n",__FUNCTION__,__LINE__);
			printf("FAPI_WLAN_CLI, fapiWlanGenericSetNativeFunc return with error\n");
			return UGW_FAILURE;
		}
	}
	else
	{
		//printf("%s %d\n",__FUNCTION__,__LINE__);
		printf("FAPI_WLAN_CLI, wlanLoadFromDB return with error\n");
		return UGW_FAILURE;
	}
}

struct wpa_ctrl {
	int s;
	struct sockaddr_un local;
	struct sockaddr_un dest;
};
static bool fieldValuesGet(char *buf, char *stringOfValues, const char *stringToSearch, char *endFieldName[])
{  /* handles list of fields, one by one in the same row */
        char *stringStart;
        char *stringFraction;
        char *localBuf = NULL;
        char *localStringToSearch = NULL;
        int  i;

        localBuf = (char *)malloc((size_t)(strlen(buf) + 1));
        if (localBuf == NULL)
        {
                printf("%s; malloc failed ==> ABORT!\n", __FUNCTION__);
                return false;
        }

        /* Add ' ' at the beginning of a string - to handle a case in which the buf starts with the
           value of stringToSearch, like buf= 'candidate=d8:fe:e3:3e:bd:14,2178,83,5,7,255 candidate=...' */
        sprintf(localBuf, " %s", buf);

        /* localStringToSearch set to stringToSearch with addition of " " at the beginning -
           it is a MUST in order to differentiate between "ssid" and "bssid" */
        localStringToSearch = (char *)malloc(strlen(stringToSearch) + 1);
        if(localStringToSearch == NULL)
        {
                printf("%s; localStringToSearch is NULL ==> Abort!\n", __FUNCTION__);
                free((void *)localBuf);
                return false;
        }

        sprintf(localStringToSearch, " %s", stringToSearch);
        stringStart = strstr(localBuf, localStringToSearch);
        if (stringStart == NULL)
        {
                free((void *)localBuf);
                free((void *)localStringToSearch);
                return false;
        }

        /* Get the first value of the field */
        stringFraction = strtok(stringStart, " ");
        if (stringFraction == NULL)
        {
                free((void *)localBuf);
                free((void *)localStringToSearch);
                return false;
        }

        stringFraction += strlen(stringToSearch);
        strcpy(stringOfValues, stringFraction);

        while (1)
        {
                stringFraction = strtok(NULL, " ");

                if (stringFraction == NULL)
                {  /* end of string reached ==> finish */
                        free((void *)localBuf);
                        free((void *)localStringToSearch);
                        return true;
                }

                i = 0;
                while (strcmp(endFieldName[i], "\n"))
                {  /* run over all field names in the string */
                        if (!strncmp(stringFraction, endFieldName[i], strlen(endFieldName[i])))
                        {  /* field name reached ==> finish */
                                free((void *)localBuf);
                                free((void *)localStringToSearch);
                                return true;
                        }

                        i++;
                }

                sprintf(stringOfValues, "%s %s", stringOfValues, stringFraction);  /* add the following value to the list */
        }

        free((void *)localBuf);
        free((void *)localStringToSearch);

        return true;
}
/* return 0 means get nothing */
static int unconnected_sta_rssi_parse_test(char *buf)
{
	char    *opCode;
	char    *VAPName;
	char    *MACAddress;
	char    *rx_bytes;
	char    *rx_packets;
	char    stringOfValues[128];
	char    *completeBuf = strdup(buf);
	char    *endFieldName[] =
	{ "rssi",
	"\n" };

	int rssi_ret=0,rssi_1=0,rssi_2=0,rssi_3=0,rssi_4=0;

	if (completeBuf == NULL) {
		printf("%s; strdup() failed ==> ABORT!\n", __FUNCTION__);
		return rssi_ret;
	}

	opCode = strtok(buf, " ");
	if (strstr(opCode, "UNCONNECTED-STA-RSSI") == NULL) {
		printf("%s; wrong opCode ('%s') ==> Abort!\n", __FUNCTION__, opCode);
		free((void *)completeBuf);
		return rssi_ret;
	}

	VAPName = strtok(NULL, " ");
	if (strncmp(VAPName, "wlan", 4)) {
		printf("%s; VAP Name ('%s') is NOT supported ==> Abort!\n", __FUNCTION__, VAPName);
		free((void *)completeBuf);
		return rssi_ret;
	}

	MACAddress = strtok(NULL, " ");
	rx_bytes   = strtok(NULL, " ") + strlen("rx_bytes=");
	rx_packets = strtok(NULL, " ") + strlen("rx_packets=");

	if (fieldValuesGet(completeBuf, stringOfValues, "rssi=", endFieldName)) {
		if(  (strncmp("-128 -128 -128 -128",stringOfValues,19) == 0) ) {
			free((void *)completeBuf);
			printf("[%d]unconnected_sta_rssi_parse_test %d\n",__LINE__,rssi_ret);
			return rssi_ret;
		} else {
			sscanf(stringOfValues,"%d %d %d %d",&rssi_1,&rssi_2,&rssi_3,&rssi_4);
			rssi_ret = rssi_1;
			rssi_ret = rssi_ret > rssi_2 ? rssi_ret : rssi_2;
			rssi_ret = rssi_ret > rssi_3 ? rssi_ret : rssi_3;
			rssi_ret = rssi_ret > rssi_4 ? rssi_ret : rssi_4;
			printf("[%d]unconnected_sta_rssi_parse_test %d\n",__LINE__,rssi_ret);
			free((void *)completeBuf);
			return rssi_ret;
		}
	}

	printf("[%d]unconnected_sta_rssi_parse_test %d\n",__LINE__,rssi_ret);
	free((void *)completeBuf);
	return rssi_ret;
}

/* return value : rssi value; 0 means error or gets nothing */
int report_check(const char *iface,struct wpa_ctrl *wpaCtrlPtr)
{
	char    *buf;
	size_t  len = HOSTAPD_TO_FAPI_MSG_LENGTH * sizeof(char);
	int     rssi_ret=0;

	if (wpaCtrlPtr == NULL) {
		return rssi_ret;
	}

	buf = (char *)malloc((size_t)(HOSTAPD_TO_FAPI_MSG_LENGTH * sizeof(char)));
	if (buf == NULL) {printf("%s\n",wpaCtrlPtr->local.sun_path);
		printf("[%s]%s; malloc error ==> ABORT!\n",wpaCtrlPtr->local.sun_path, __FUNCTION__);
		return rssi_ret;
	}

	memset(buf, 0, HOSTAPD_TO_FAPI_MSG_LENGTH * sizeof(char));  /* Clear the output buffer */
	if (wpa_ctrl_recv(wpaCtrlPtr, buf, &len) == 0) {
		if (len <= 5) {
			printf("[%s]%s; '%s' is NOT a report - continue!\n",wpaCtrlPtr->local.sun_path, __FUNCTION__, buf);
			free((void *)buf);
			return rssi_ret;
		}
		printf("[%s][%s]\n",wpaCtrlPtr->local.sun_path,buf);
		rssi_ret = unconnected_sta_rssi_parse_test( buf);
		free((void *)buf);
		return rssi_ret;
	} else {
		printf("[%s]%s; wpa_ctrl_recv() returned ERROR\n",wpaCtrlPtr->local.sun_path, __FUNCTION__);
		free((void *)buf);
		return rssi_ret;
	}

	free((void *)buf);

	return rssi_ret;
}


int rast_stamon_get_rssi(int unit , struct ether_addr *addr)
{
	char *ifname=NULL;
	//char wif_buf[32]={0};
	char macaddr_str[32]={0};
	int width=0,center_freq1=0,center_freq=0;
	struct wpa_ctrl *wpaCtrlPtr=NULL;
	int  fd,res,res_wpa_ctrl;
	fd_set rfds;
	struct timeval timeout;
	int notfirsttimeout=0;
	size_t len;
	int retry_times=0;
	char localBuf[HOSTAPD_TO_FAPI_MSG_LENGTH]={0};

	int retry_max=0;

	int rssi_sum=0;

	int rssi_cnt=0;

	char command[] = "UNCONNECTED_STA_RSSI xx:xx:xx:xx:xx:xx xxxx center_freq1=xxxx bandwidth=xx";

	//if(nvram_get_int("rico_rssi_test_retry_max"))
		//retry_max = nvram_get_int("rico_rssi_test_retry_max");
	//else
		retry_max = 5;

	//ifname = strdup(get_wififname(unit ));
	//if (!ifname){
	// 	goto rast_stamon_get_rssi_return;
//
	/* MAC address */
	//sprintf(macaddr_str,MACF,ETHERP_TO_MACF(addr));
	/* freq & bandwidth */
	//if( get_channel(ifname,&width,&center_freq1,&center_freq) ){
	//	goto rast_stamon_get_rssi_return;
	//}
/*
width:                    80
center_freq1:           5210
chan.center_freq:       5220
channum_from:             36
channum_to:               48
primary_channel:          44
primary_chan_idx:          2
*/
	sprintf(command,"UNCONNECTED_STA_RSSI 98:FE:94:40:AD:FE 5220 center_freq1=5210 bandwidth=80");

	//if(unit  == 1)
		wpaCtrlPtr = wpa_ctrl_open("/var/run/hostapd/wlan2");
	//else
	//	wpaCtrlPtr = wpa_ctrl_open("/var/run/hostapd/wlan0");

	if (wpaCtrlPtr == NULL) {
		printf("%s; ERROR: hostapd_socket_get on band '%d' failed!\n", __FUNCTION__, unit );
		goto rast_stamon_get_rssi_return;
	} else if (wpa_ctrl_attach(wpaCtrlPtr) != 0) {
		printf("%s; ERROR: wpa_ctrl_attach for band '%d' failed!\n", __FUNCTION__, unit );
		wpa_ctrl_close(wpaCtrlPtr);
		goto rast_stamon_get_rssi_return;
	} else {
		fd = wpa_ctrl_get_fd(wpaCtrlPtr);
	}
    /* Main event loop */

    timeout.tv_sec = 0;
    timeout.tv_usec= 0;

    while (1)
    {
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		res = select(fd + 1, &rfds, NULL, NULL, &timeout);
		if (res < 0)
		{
			printf("%s; select() return value= %d ==> CONTINUE!!!\n", __FUNCTION__, res);
			continue;
		} else if( res == 0 ) {			
			if(!notfirsttimeout)
			{				
				notfirsttimeout = 1;
				timeout.tv_sec = 0;
				timeout.tv_usec= 100000;
				len=HOSTAPD_TO_FAPI_MSG_LENGTH;
				memset(localBuf,0,(4096 * 3) );                             
				wpa_ctrl_request(wpaCtrlPtr, command, strlen(command), localBuf, &len, NULL);
				retry_times ++;
				printf("select timeout\n");                 
			} else {				
				timeout.tv_sec = 0;
				timeout.tv_usec= 100000;                            
			}
			continue;
		}
		if (FD_ISSET(fd, &rfds)) {		
			//while (1) {
			retry_times ++;
				//if(nvram_get_int("rico_usleep"))
				//	usleep(nvram_get_int("rico_usleep"));

			res_wpa_ctrl = wpa_ctrl_pending(wpaCtrlPtr);
			if (res_wpa_ctrl != 1)
				break;  /* quit the 'while' loop */
			//if ( retry_times > retry_max )
			//	break;
			res = report_check(NULL,wpaCtrlPtr);
			if ( res == 0 ) {				
				len=HOSTAPD_TO_FAPI_MSG_LENGTH;
				memset(localBuf,0,(4096 * 3) );     
				wpa_ctrl_request(wpaCtrlPtr, command, strlen(command), localBuf, &len, NULL);
				//break;
			} else {
				if(!rssi_sum) rssi_sum=res;
				rssi_sum = res > rssi_sum ? res : rssi_sum;
				rssi_cnt ++;

				len=HOSTAPD_TO_FAPI_MSG_LENGTH;
				memset(localBuf,0,(4096 * 3) );     
				wpa_ctrl_request(wpaCtrlPtr, command, strlen(command), localBuf, &len, NULL);
				//break;
			}				
		}
		if ( res_wpa_ctrl == (-1) ) {  
		/* ERROR - issue a trace */
			printf("wpa_ctrl_pending() returned ERROR\n");
		}
		if ( retry_times > retry_max )
			break;
	}

	if (wpaCtrlPtr != NULL)
		wpa_ctrl_detach(wpaCtrlPtr);

	wpa_ctrl_close(wpaCtrlPtr);
	wpaCtrlPtr = NULL;

rast_stamon_get_rssi_return:

	//if(ifname)
	//	free(ifname);

	printf("best:%d,got %d\n",rssi_sum,rssi_cnt);

	if(rssi_cnt>0)
	{
		return rssi_sum;
	}

	return 0;
}


#define AP_CONFIG_FILE_ONOFF1 "/tmp/wlan_wave/set_radio_onoff11.txt"
int main(int argc, char const *argv[])
{
	char new_value[100];
	int retVal;
	ObjList *dbObjPtr = NULL;
	FILE *fp=NULL;
	int wave_unit;

	int unit,  onoff;
	printf("test v2\n");
	if(atoi(argv[1]) == 1)
	{
		unit = 0;
		//subunit = 0;
		onoff = 0;
	} else if(atoi(argv[1]) == 2)
	{
		unit = 0;
		//subunit = 0;
		onoff = 1;
	} else if(atoi(argv[1]) == 3)
	{
		unit = 1;
		//subunit = 0;
		onoff = 0;
	} else if(atoi(argv[1]) == 4)
	{
		unit = 1;
		//subunit = 0;
		onoff = 1;
	} else if(atoi(argv[1]) == 5)
	{	int  fd,res,res_wpa_ctrl;
		struct wpa_ctrl *wpaCtrlPtr=NULL;
		fd_set rfds;
		struct timeval timeout;
		int notfirsttimeout=0;
		size_t len;
		int retry_times=0;
		char localBuf[HOSTAPD_TO_FAPI_MSG_LENGTH]={0};

		wpaCtrlPtr = wpa_ctrl_open("/var/run/hostapd/wlan0");
		if(wpaCtrlPtr==NULL){
			printf("/var/run/hostapd/wlan0 open not good\n");
			return 0;
		}
		else if (wpa_ctrl_attach(wpaCtrlPtr) != 0) {
			printf("%s; ERROR: wpa_ctrl_attach for band '%d' failed!\n", __FUNCTION__, unit );
			wpa_ctrl_close(wpaCtrlPtr);
			return 0;
		} else {
			fd = wpa_ctrl_get_fd(wpaCtrlPtr);
		}
/*
width:                    80
center_freq1:           5210
chan.center_freq:       5220
channum_from:             36
channum_to:               48
primary_channel:          44
primary_chan_idx:          2
*/
printf("wpaCtrlPtr->s %d\n",wpaCtrlPtr->s);
printf("%s\n",wpaCtrlPtr->local.sun_path);
printf("%s\n",wpaCtrlPtr->dest.sun_path);
printf("fd %d\n",fd);

#if 0
    	timeout.tv_sec = 0;
    	timeout.tv_usec= 0;

	    while (1)
	    {
			FD_ZERO(&rfds);
			FD_SET(fd, &rfds);
			res = select(fd + 1, &rfds, NULL, NULL, &timeout);
			if (res < 0)
			{
				printf("%s; select() return value= %d ==> CONTINUE!!!\n", __FUNCTION__, res);
				continue;
			} else if( res == 0 ) {			
				if(!notfirsttimeout)
				{				
					notfirsttimeout = 1;
					timeout.tv_sec = 0;
					timeout.tv_usec= 100000;
					len=HOSTAPD_TO_FAPI_MSG_LENGTH;
					memset(localBuf,0,(4096 * 3) );                             
					wpa_ctrl_request(wpaCtrlPtr, command, strlen(command), localBuf, &len, NULL);
					retry_times ++;
					_dprintf("select timeout\n");                 
				} else {				
					timeout.tv_sec = 0;
					timeout.tv_usec= 100000;                            
				}
				continue;
			}
			if (FD_ISSET(fd, &rfds)) {		
				//while (1) {
				//retry_times ++;
					//if(nvram_get_int("rico_usleep"))
					//	usleep(nvram_get_int("rico_usleep"));

				res_wpa_ctrl = wpa_ctrl_pending(wpaCtrlPtr);
				if (res_wpa_ctrl != 1)
					break;  /* quit the 'while' loop */
				//if ( retry_times > retry_max )
				//	break;
				res = report_check(ifname,wpaCtrlPtr);

			}
			if ( res_wpa_ctrl == (-1) ) {  
			/* ERROR - issue a trace */
				printf("wpa_ctrl_pending() returned ERROR\n");
			}
			//if ( retry_times > retry_max )
			//	break;
		}
#endif
			sleep(300);
			wpa_ctrl_close(wpaCtrlPtr);
		
		return 0;
	} else if(atoi(argv[1]) == 6)
	{
		int  fd,res,res_wpa_ctrl;
		struct wpa_ctrl *wpaCtrlPtr=NULL;
		fd_set rfds;
		struct timeval timeout;
		int notfirsttimeout=0;
		size_t len;
		int retry_times=0;
		char localBuf[HOSTAPD_TO_FAPI_MSG_LENGTH]={0};

		wpaCtrlPtr = wpa_ctrl_open("/var/run/hostapd/wlan2");
		if(wpaCtrlPtr==NULL){
			printf("/var/run/hostapd/wlan2 open not good\n");
			return 0;
		}
		else if (wpa_ctrl_attach(wpaCtrlPtr) != 0) {
			printf("%s; ERROR: wpa_ctrl_attach for band '%d' failed!\n", __FUNCTION__, unit );
			wpa_ctrl_close(wpaCtrlPtr);
			return 0;
		} else {
			fd = wpa_ctrl_get_fd(wpaCtrlPtr);
		}
/*
width:                    80
center_freq1:           5210
chan.center_freq:       5220
channum_from:             36
channum_to:               48
primary_channel:          44
primary_chan_idx:          2
*/
printf("wpaCtrlPtr->s %d\n",wpaCtrlPtr->s);
printf("%s\n",wpaCtrlPtr->local.sun_path);
printf("%s\n",wpaCtrlPtr->dest.sun_path);
printf("fd %d\n",fd);

#if 1
    	timeout.tv_sec = 0;
    	timeout.tv_usec= 0;

	    while (1)
	    {
			FD_ZERO(&rfds);
			FD_SET(fd, &rfds);
			res = select(fd + 1, &rfds, NULL, NULL, &timeout);
			if (res < 0)
			{
				printf("%s; select() return value= %d ==> CONTINUE!!!\n", __FUNCTION__, res);
				continue;
			} else if( res == 0 ) {
				//printf("seletct time out (6)\n");					
				timeout.tv_sec = 10;
				timeout.tv_usec= 100000;                            
				continue;
			}
			if (FD_ISSET(fd, &rfds)) {		
				res_wpa_ctrl = wpa_ctrl_pending(wpaCtrlPtr);
				if (res_wpa_ctrl != 1)
					break;  /* quit the 'while' loop */
				res = report_check(NULL,wpaCtrlPtr);

			}
			if ( res_wpa_ctrl == (-1) ) {  
			/* ERROR - issue a trace */
				printf("wpa_ctrl_pending() returned ERROR\n");
			}
			//if ( retry_times > retry_max )
			//	break;
		}
#endif
			sleep(300);
			wpa_ctrl_close(wpaCtrlPtr);
				
		return 0;
	} else if(atoi(argv[1]) == 7)
	{
		int  fd,res,res_wpa_ctrl;
		struct wpa_ctrl *wpaCtrlPtr=NULL;
		fd_set rfds;
		struct timeval timeout;
		int notfirsttimeout=0;
		size_t len;
		int retry_times=0;
		char localBuf[HOSTAPD_TO_FAPI_MSG_LENGTH]={0};

		wpaCtrlPtr = wpa_ctrl_open("/var/run/hostapd/wlan2");
		if(wpaCtrlPtr==NULL){
			printf("/var/run/hostapd/wlan2 open not good\n");
			return 0;
		}
		else if (wpa_ctrl_attach(wpaCtrlPtr) != 0) {
			printf("%s; ERROR: wpa_ctrl_attach for band '%d' failed!\n", __FUNCTION__, unit );
			wpa_ctrl_close(wpaCtrlPtr);
			return 0;
		} else {
			fd = wpa_ctrl_get_fd(wpaCtrlPtr);
		}
/*
width:                    80
center_freq1:           5210
chan.center_freq:       5220
channum_from:             36
channum_to:               48
primary_channel:          44
primary_chan_idx:          2
*/
printf("wpaCtrlPtr->s %d\n",wpaCtrlPtr->s);
printf("%s\n",wpaCtrlPtr->local.sun_path);
printf("%s\n",wpaCtrlPtr->dest.sun_path);
printf("fd %d\n",fd);

#if 1
    	timeout.tv_sec = 0;
    	timeout.tv_usec= 0;

	    while (1)
	    {
			FD_ZERO(&rfds);
			FD_SET(fd, &rfds);
			res = select(fd + 1, &rfds, NULL, NULL, &timeout);
			if (res < 0)
			{
				printf("%s; select() return value= %d ==> CONTINUE!!!\n", __FUNCTION__, res);
				continue;
			} else if( res == 0 ) {
				//printf("seletct time out (7)\n");					
				timeout.tv_sec = 10;
				timeout.tv_usec= 100000;                            
				continue;
			}
			if (FD_ISSET(fd, &rfds)) {		
				res_wpa_ctrl = wpa_ctrl_pending(wpaCtrlPtr);
				if (res_wpa_ctrl != 1)
					break;  /* quit the 'while' loop */
				res = report_check(NULL,wpaCtrlPtr);

			}
			if ( res_wpa_ctrl == (-1) ) {  
			/* ERROR - issue a trace */
				printf("wpa_ctrl_pending() returned ERROR\n");
			}
			//if ( retry_times > retry_max )
			//	break;
		}
#endif
			sleep(300);
			wpa_ctrl_close(wpaCtrlPtr);
				
		return 0;
	} else if(atoi(argv[1]) == 8) { 
		rast_stamon_get_rssi(1,argv[2]);
	} else
		return 0;

	//wl_nvprefix(prefix, sizeof(prefix), unit, subunit);
	wave_unit = wl_wave_unit(unit);

	dbObjPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);

	/* Object_0=Device.WiFi.AccessPoint */
	fprintf(fp, "Object_0=Device.WiFi.AccessPoint\n");

	if(onoff == 1){
		/* enable */
		//printf("%s: %s(wave_unit %d) Enable.\n", __func__, prefix,wave_unit);
		fprintf(fp, "Enable_0=true\n");
		snprintf(new_value, sizeof(new_value), "true\n");
		//nvram_set_int(strcat_r(prefix, "radio", tmp), 1);
	}else{
		/* disable */
		//printf("%s: %s(wave_unit %d) Disable.\n", __func__, prefix,wave_unit);
		fprintf(fp, "Enable_0=false\n");
		snprintf(new_value, sizeof(new_value), "false\n");
		//nvram_set_int(strcat_r(prefix, "radio", tmp), 0);
	}

	fclose(fp);
	//printf("before fapi_wlan_generic_set_native2 test with sleep\n");

	if(1) {
		retVal = fapi_wlan_generic_set_native2(fapi_wlan_ap_set_native,
		  wave_unit, dbObjPtr, AP_CONFIG_FILE_ONOFF1, 0 ); 
	}

	//printf("fapi_wlan_generic_set_native2 %d\n",retVal);
	HELP_DELETE_OBJ(dbObjPtr, SOPT_OBJVALUE, FREE_OBJLIST);

	wlan_ifconfigUp(wave_unit);

	return 0;
}
