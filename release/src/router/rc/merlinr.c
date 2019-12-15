/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 *
 * Copyright 2019, paldier <paldier@hotmail.com>.
 * All Rights Reserved.
 * 
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "merlinr.h"
#include "rc.h"
#include <shared.h>
#include <shutils.h>
#if defined(RTCONFIG_LANTIQ)
#include <lantiq.h>
#endif

#include <curl/curl.h>


int str_split(char* buf, char** s, int s_size) {
	int curr = 0;
	char* token = strtok(buf, ".");
	while(token && curr < s_size) {
		s[curr++] = token;
		token = strtok(NULL, ".");
	}
	return curr;
}

int versioncmp(char *cur_fwver, char *fwver) {
	char buf1[20] = {0}, buf2[20] = {0};
	char* s1[20] = {0};
	char* s2[20] = {0};
	char* end;
	int i, n1, n2, s1_len, s2_len, s_max, rlt;
	strncpy(buf1, cur_fwver, strlen(cur_fwver));
	strncpy(buf2, fwver, strlen(fwver));
	//fprintf(stderr, "%s\n", buf1);
	//fprintf(stderr, "%s\n", buf2);

	s1_len = str_split(buf1, s1, 10);
	s2_len = str_split(buf2, s2, 10);

	//fprintf(stderr, "s1=%d, s2=%d\n", s1_len, s2_len);

	s_max = (s1_len > s2_len ? s1_len: s2_len);

	rlt = 0;
	for(i = 0; i < s_max; i++) {
		if(!s1[i]) {
			s1[i] = "0";
		}
		if(!s2[i]) {
			s2[i] = "0";
		}

		n1 = strtol(s1[i], &end, 10);
		if(*end) {
			//parse error, but not break hear
			rlt = 1;
		}

		n2 = strtol(s2[i], &end, 10);
		if(*end) {
			if (1 == rlt) {
				//both error
				rlt = 0;
				break;
			}
			rlt = -1;
			break;
		}

		if(1 == rlt) {
			break;
		}

		if (n1 == n2) {
			continue;
		} else if(n1 < n2) {
			rlt = 1;
			break;
		} else {
			rlt = -1;
			break;
		}
	}
	return rlt;
}

size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb, void *stream) {
	int r;
	long len = 0;

	/* _snscanf() is Win32 specific */
	// r = _snscanf(ptr, size * nmemb, "Content-Length: %ld\n", &len);
	r = sscanf(ptr, "Content-Length: %ld\n", &len);
	if (r) /* Microsoft: we don't read the specs */
		*((long *) stream) = len;

	return size * nmemb;
}

size_t wirtefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return fwrite(ptr, size, nmemb, stream);
}
#if 0
size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	FILE *f = stream;
	size_t n;

	if (ferror(f))
		return CURL_READFUNC_ABORT;

	n = fread(ptr, size, nmemb, f) * size;

	return n;
}
#endif
int curl_download_file(CURL *curlhandle, const char * remotepath, const char * localpath, long timeout, long tries)
{
	FILE *f;
	curl_off_t local_file_len = -1 ;
	long filesize =0 ;

	CURLcode r = CURLE_GOT_NOTHING;
	//int c;
	struct stat file_info;
	int use_resume = 0;
	//if(access(localpath,F_OK) ==0)

	if(stat(localpath, &file_info) == 0) 
	{
		local_file_len =  file_info.st_size;
		use_resume  = 1;
	}
	f = fopen(localpath, "ab+"); 
	if (f == NULL) {
		perror(NULL);
		return 0;
	}

	//curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);

	curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
	curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &filesize);
	curl_easy_setopt(curlhandle, CURLOPT_RESUME_FROM_LARGE, use_resume?local_file_len:0);
	curl_easy_setopt(curlhandle, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curlhandle, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(curlhandle, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1); 
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, f);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, wirtefunc);

	//curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
	//curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1);

	r = curl_easy_perform(curlhandle);
	fclose(f);

	if (r == CURLE_OK)
		return 1;
	else {
		fprintf(stderr, "%s\n", curl_easy_strerror(r));
		return 0;
	}
}

int merlinr_firmware_check_update_main(int argc, char *argv[])
{
	//char notetxt[]="/tmp/release_note.txt";
	//char downloadphp[]="download.php";
	//char sqdownloadphp[]="SQ_download.php";
	//char wlanutdatetxt[]="/tmp/wlan_utdate.txt";
	//char wlan_update_mrflag1[]="wlan_update_mrflag1.zip";
	FILE *fpupdate;
	int download;
	char url[100];
	char log[200];
	char serverurl[]="https://update.paldier.com";
	char serverupdate[]="wlan_update_v2.zip";
	char localupdate[]="/tmp/wlan_update.txt";
	char releasenote[]="/tmp/release_note0.txt";
	char model[20], modelname[20], fsver[10], fwver[10], tag[10];
	char cur_fwver[10];
	char *tmp_fwver=nvram_get("extendno");
	char info[100];
	nvram_set("webs_state_update", "0");
	nvram_set("webs_state_flag", "0");
	nvram_set("webs_state_error", "0");
	nvram_set("webs_state_odm", "0");
	nvram_set("webs_state_url", "");
	unlink("/tmp/webs_upgrade.log");
	unlink("/tmp/wlan_update.txt");
	unlink("/tmp/release_note0.txt");
	sscanf(tmp_fwver, "%*[A-Z0-9]_%[A-Z0-9.]-%*[a-z0-9]", cur_fwver);
	CURL *curlhandle = NULL;

	curl_global_init(CURL_GLOBAL_ALL);
	curlhandle = curl_easy_init();
	snprintf(url, sizeof(url), "%s/%s", serverurl, serverupdate);
	snprintf(log, sizeof(log), "echo \"[FWUPDATE]---- update dl_path_info for general %s/%s ----\" >> /tmp/webs_upgrade.log", serverurl, serverupdate);
	download=curl_download_file(curlhandle , url,localupdate,8,3);
	system(log);
	_dprintf("%d\n",download);
	if(download)
	{
		fpupdate = fopen(localupdate, "r");
		if (!fpupdate)
			goto GODONE;
		//BLUECAVE#K3C#3004384#R7.0#g13e704e
		char buffer[1024];
		while(NULL!=fgets(buffer,sizeof(buffer),fpupdate)){
			sscanf(buffer,"%[A-Z0-9-]#%[A-Z0-9]#%[0-9]#%[A-Z0-9.]#%[a-z0-9]",model,modelname,fsver,fwver,tag);
			_dprintf("%s#%s#%s#%s\n",model,modelname,fsver,fwver);
			if(!strcmp(model, nvram_get("productid")) && !strcmp(modelname, nvram_safe_get("modelname"))){
				if((strstr(cur_fwver, "B") && strstr(fwver, "B"))||(strstr(cur_fwver, "R") && strstr(fwver, "R"))){
					//_dprintf("%s#%s\n",fwver,cur_fwver);
					if(versioncmp((cur_fwver+1),(fwver+1))==1){
						nvram_set("webs_state_url", "");
#if defined(SBRAC3200P) || defined(RTACRH17) || defined(RTAC3200)
						snprintf(info,sizeof(info),"3004_382_%s_%s-%s",modelname,fwver,tag);
#else
						snprintf(info,sizeof(info),"3004_384_%s_%s-%s",modelname,fwver,tag);
#endif
						nvram_set("webs_state_info", info);
						nvram_set("webs_state_REQinfo", info);
						nvram_set("webs_state_flag", "1");
						nvram_set("webs_state_update", "1");
#ifdef RTCONFIG_AMAS
//aimesh has other upgrade tools(one or more),non-mesh network status:rc,mesh network status:cfg_server
//						nvram_set("cfg_check", "9");
//						nvram_set("cfg_upgrade", "0");
#endif
						memset(url,'\0',sizeof(url));
						memset(log,'\0',sizeof(log));
						char releasenote_file[100];
						snprintf(releasenote_file, sizeof(releasenote_file), "%s_%s_%s_note.zip", nvram_get("productid"), nvram_get("webs_state_info"),nvram_get("preferred_lang"));
						snprintf(url, sizeof(url), "%s/%s", serverurl, releasenote_file);
						snprintf(log, sizeof(log), "echo \"[FWUPDATE]---- download real release note %s/%s ----\" >> /tmp/webs_upgrade.log", serverurl, releasenote_file);
						system(log);
						download=curl_download_file(curlhandle , url,releasenote,8,3);
						if(download ==0 ){
							memset(url,'\0',sizeof(url));
							snprintf(releasenote_file, sizeof(releasenote_file), "%s_%s_US_note.zip", nvram_get("productid"), nvram_get("webs_state_info"));
							snprintf(url, sizeof(url), "%s/%s", serverurl, releasenote_file);
							snprintf(log, sizeof(log), "echo \"[FWUPDATE]---- download real release note %s/%s ----\" >> /tmp/webs_upgrade.log", serverurl, releasenote_file);
							system(log);
							curl_download_file(curlhandle , url,releasenote,8,3);
						}

						return 0;
#if 0
						if(strstr(nt_center,"nt_center")){
							if((!nvram_get("webs_last_info"))||(strcmp(nvram_get("webs_last_info"), nvram_get("webs_state_info")))){
								system("Notify_Event2NC \"$SYS_FW_NWE_VERSION_AVAILABLE_EVENT\" \"{\\\"fw_ver\\\":\\\"$update_webs_state_info\\\"}\"");
								nvram_set("webs_last_info", nvram_get("webs_state_info"));
							}
						}
#endif
					}
				}
			}
		}
	}

	curl_easy_cleanup(curlhandle);
	curl_global_cleanup();

GODONE:
#if defined(SBRAC3200P) || defined(RTACRH17) || defined(RTAC3200)
	snprintf(info,sizeof(info),"3004_382_%s",nvram_get("extendno"));
//elif defined(RTAC68U)
//	snprintf(info,sizeof(info),"3004_385_%s",nvram_get("extendno"));
#else
	snprintf(info,sizeof(info),"3004_384_%s",nvram_get("extendno"));
#endif
	nvram_set("webs_state_url", "");
	nvram_set("webs_state_flag", "0");
	nvram_set("webs_state_error", "1");
	nvram_set("webs_state_odm", "0");
	nvram_set("webs_state_update", "1");
	nvram_set("webs_state_info", info);
	nvram_set("webs_state_REQinfo", info);
	nvram_set("webs_state_upgrade", "");
#ifdef RTCONFIG_AMAS
	nvram_set("cfg_check", "9");
	nvram_set("cfg_upgrade", "0");
#endif

	return 0;
}

void exec_uu_merlinr()
{
	FILE *fpmodel, *fpmac, *fpuu, *fpurl, *fpmd5, *fpcfg;
	char buf[128];
	int download,i;
	char *dup_pattern, *g, *gg;
	char p[10][100];
	if(nvram_get_int("sw_mode") == 1){
#if defined(BLUECAVE)
		if(strncmp(nvram_get("territory_code"), "CN", 2))
			return;
		add_rc_support("uu_accel");
#endif
		if ((fpmodel = fopen("/var/model", "w"))){
			fprintf(fpmodel, nvram_get("productid"));
			fclose(fpmodel);
		}
		if ((fpmac = fopen("/var/label_macaddr", "w"))){
			char *etmac=nvram_get("et2macaddr");
			toUpperCase(etmac);
			fprintf(fpmac, etmac);
			fclose(fpmac);
		}
		if ((fpuu = fopen("/var/uu_plugin_dir", "w"))){
			fprintf(fpuu, "/jffs");
			fclose(fpuu);
		}
		system("mkdir -p /tmp/uu");
		download = system("wget -t 2 -T 30 --dns-timeout=120 --header=Accept:text/plain -q --no-check-certificate 'https://router.uu.163.com/api/script/monitor?type=asuswrt-merlin' -O /tmp/uu/script_url");
		if (!download){
			_dprintf("download uuplugin script info successfully\n");
			if ((fpurl = fopen("/tmp/uu/script_url", "r"))!=NULL){
				fgets(buf, 128, fpurl);
				fclose(fpurl);
				unlink("/tmp/uu/script_url");
				i=0;
				g = dup_pattern = strdup(buf);
				gg = strtok( g, "," );
				while (gg != NULL)
				//for(g = strsep(&dup_pattern, ","); g != NULL; g = strsep(&dup_pattern, ","))
				{
					if (gg!=NULL){
						strcpy(p[i], gg);
						i++;
						++download;
						//logmessage("K3", "download: %d",download);
						gg = strtok( NULL, "," );
					}
				}
				if ( download > 0 )
				//if ( download == 2 )
				{
					_dprintf("URL: %s\n",p[0]);
					_dprintf("MD5: %s\n",p[1]);
					//logmessage("K3", "URL: %s,MD5: %s",p[0],p[1]);
					if ( !doSystem("wget -t 2 -T 30 --dns-timeout=120 --header=Accept:text/plain -q --no-check-certificate %s -O /tmp/uu/uuplugin_monitor.sh", p[0]))
					{
						_dprintf("download uuplugin script successfully\n");
						if ((fpcfg = fopen("/tmp/uu/uuplugin_monitor.config", "w"))){
							fprintf(fpcfg, "router=asuswrt-merlin\n");
							fprintf(fpcfg, "model=\n");
							fclose(fpcfg);
						}
						if((fpmd5=popen("md5sum /tmp/uu/uuplugin_monitor.sh | sed 's/[ ][ ]*/ /g' | cut -d' ' -f1", "r")))
						{
							memset(buf,'\0',sizeof(buf));
							if((fread(buf, 1, 128, fpmd5)))
							{
								buf[32]='\0';
								buf[33]='\0';
								//logmessage("K3", "MD5: %s,MD5: %s",buf,p[1]);
								if ( !strcasecmp(buf, p[1]))
								{
									pid_t pid;
									char *uu_argv[] = { "/tmp/uu/uuplugin_monitor.sh", NULL };
									_dprintf("prepare to execute uuplugin stript...\n");
									//logmessage("K3", "prepare to execute uuplugin stript...");
									chmod("/tmp/uu/uuplugin_monitor.sh", 0755);
									_eval(uu_argv, NULL, 0, &pid);
									//eval("/tmp/uu/uuplugin_monitor.sh");
								}
							}
							pclose(fpmd5);
						}
					}
				}
			}
			free(dup_pattern);
		}
	}
}
#if !defined(RTAC68U) && !defined(GTAC5300) && !defined(GTAC2900) && !defined(RTAC86U)
void start_sendfeedback(void)
{

}
#endif
