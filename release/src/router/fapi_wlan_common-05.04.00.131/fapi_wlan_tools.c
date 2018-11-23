/*
** =============================================================================
**   FILE NAME        : fapi_wlan_tools.c
**   PROJECT          : UGW WLAN FAPI
**   DATE             : 02-Aug-2016
**   AUTHOR           : WLAN Subsystem
**   DESCRIPTION      : This file contains the WLAN FAPI tools prototypes.
**   REFERENCES       :
**   COPYRIGHT        : Copyright (c) 2015
**                      Intel
**
**   Any use of this software is subject to the conclusion of a respective
**   License agreement. Without such a License agreement no rights to the
**   software are granted

**   HISTORY          :
**   $Date   02-Aug-2016         $Author  yhanin                     $Comment
**
** ============================================================================
*/

/*! \file 	fapi_wlan_tools.c
	\brief 	This file implements the WLAN FAPI tools.
	\todo 	Add license header
	\todo 	return dll_uninit when issues of calling in loop will be solved
 */
/* for the ftw - using nftw function*/
#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>
#include <ftw.h>				/* for using ftw - removeDir*/
#include <unistd.h> 			/* for using access */
#include <sys/stat.h> 			/* for chmod */
#include "fapi_wlan_private.h" 	/* for FLOG*/
#include "help_error.h"			/* #define UGW_SUCCESS 0 , #define UGW_FAILURE -1 */
#include "help_objlist.h"

#include "fapi_wlan_tools.h"


char globalEmptyString[1] = { '\0' };

/* statics */
/* for ftw removeDir */
static int removeDirActionCB(const char *path, const struct stat *stat_p, int flag , struct FTW *info_P);

static int removeDirActionCB(const char *path, const struct stat *stat_p, int flag , struct FTW *info_P)
{
	(void)flag;

	int retVal = 0;
	/* if flag is FTW_NS, ptr contains undefined values because
     ftw couldn't stat path */
	(void)stat_p;
	(void)info_P;

	FLOG(FLOG_HELP_FUNC,"path = |%s|\n", path);

	retVal = remove(path);

	return retVal;
}

int removeDir(const char *src_dir)
{
	if (src_dir == NULL) {
		printf("%s:dir operation failed - invalid parameters\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if((nftw(src_dir, removeDirActionCB, DIR_MAX_NUM, FTW_DEPTH | FTW_PHYS)) == -1) {
		printf("%s:ftw failed\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	if (rmdir(src_dir) == -1) {
		printf("%s:operation failed\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	FLOG(FLOG_HELP_FUNC,"operation completed on %s\n", src_dir);

	return UGW_SUCCESS;
}

int touchFile(char *file_name)
{
	FILE *fd;

	fd = fopen(file_name, "w+");

	if(fd == NULL) {
        printf("%s: fopen() Error!!!\n", __FUNCTION__);
        return UGW_FAILURE;
    }

	fclose(fd);

	FLOG(FLOG_HELP_FUNC,"touch success[%s]\n", file_name);

	return UGW_SUCCESS;
}

int mvFile(char *src_path, char *trg_path)
{
	int retVal = UGW_SUCCESS;

	retVal = copyFile(src_path, trg_path);
	retVal = remove(src_path);

	FLOG(FLOG_HELP_FUNC,"src_path=%s trg_path=%s retVal=%d\n", src_path, trg_path, retVal);

	return retVal;
}

int copyFile(char *src_path, char *trg_path)
{
    FILE  *src_fd, *trg_fd;
	struct stat st;
	size_t elements;
	int retVal = UGW_SUCCESS;
	char *buf;

	if ((src_path == NULL) || (trg_path == NULL)){
		printf("%s:%d: operation failed - invalid parameters\n", __FUNCTION__, __LINE__);
		return UGW_FAILURE;
	}

	FLOG(FLOG_HELP_FUNC,"src_path=|%s|  trg_path=|%s|\n", src_path, trg_path);

    src_fd = fopen(src_path, "r");
    trg_fd = fopen(trg_path, "w+");

	/*debug*/
	if (src_fd) {
		FLOG(FLOG_HELP_FUNC,"src_fd= 0x%x \n", (unsigned int)src_fd);
	}
	if (trg_fd) {
		FLOG(FLOG_HELP_FUNC,"trg_fd= 0x%x \n", (unsigned int)trg_fd);
	}

	/* check that we got valid file descriptors*/
	if ((src_fd == NULL) || (trg_fd == NULL)) {
		printf("%s: open file failed or file not exist\n", __FUNCTION__);
		return UGW_FAILURE;
	}

	stat(src_path, &st);
	elements = st.st_size;
	FLOG(FLOG_HELP_FUNC,"elements = %d mode = %lo\n", elements, (long unsigned int)st.st_mode);

	buf = calloc(elements, sizeof(char));
	if (buf == NULL){
		printf("%s:%d: fail to allocate elements = %d\n", __FUNCTION__, __LINE__, elements);
		retVal = UGW_FAILURE;
		goto finish;
	}

	FLOG(FLOG_HELP_FUNC,"elements = %d  sizeof(char) = %d buf=0x%x\n", elements, sizeof(char), (unsigned int)buf);

	retVal = fread(buf, 1, elements , src_fd);
	if (retVal == -1) {
		printf("%s: Error reading file or file empty ret=%d \n", __FUNCTION__, retVal);
		retVal = UGW_FAILURE;
		goto finish;
	} else if (retVal == 0) {
		FLOG(FLOG_HELP_FUNC,"ret=%d \n", retVal);
	}

	retVal = fwrite(buf, 1, elements, trg_fd);
	if (retVal == -1) {
		printf("%s: Error writing to file\n", __FUNCTION__);
		retVal = UGW_FAILURE;
		goto finish;
	}

	retVal = UGW_SUCCESS;

finish:
	free(buf);
    fclose(src_fd);
    fclose(trg_fd);

	if (retVal == UGW_SUCCESS) {
		retVal = chmod(trg_path, st.st_mode);
		retVal = (retVal == 0) ? UGW_SUCCESS : UGW_FAILURE;
		FLOG(FLOG_HELP_FUNC,"change mode to file:%s retVal[%d]\n", trg_path, retVal);
	}

	FLOG(FLOG_HELP_FUNC,"!!!completed ret=%d\n",retVal);

	return retVal;
}


int copyDir(const char *dirsrc, const char *dirtar, const char *file_name_str)
{
	DIR		*dir_src_p, *dir_trg_p;	/* the directory */
	struct dirent	*dirent_p;	/* each entry	 */
	char source_file[MAX_LEN_PARAM_VALUE];
	char target_file[MAX_LEN_PARAM_VALUE];
	int retVal = UGW_SUCCESS;

	FLOG(FLOG_HELP_FUNC,"dirsrc=%s, dirtar=%s \n", dirsrc, dirtar);

	if ( ( dir_trg_p = opendir(dirtar) ) == NULL ) {
		/* dirtar does not exist lets create it*/
		FLOG(FLOG_HELP_FUNC,"dirtar=|%s| doesn't exist, create it\n", dirtar);
		retVal = mkdir(dirtar, (mode_t)(S_IRWXO | S_IRWXG | S_IRWXU));
		if (retVal) {
			printf("%s:ERROR, mkdir() failed for unexpected reason\n",__FUNCTION__);
			return UGW_FAILURE;
		}
	}

	if ( ( dir_src_p = opendir(dirsrc) ) == NULL ) {
		printf("%s:open_dir: cannot open %s\n", __FUNCTION__, dirsrc);
		retVal = UGW_FAILURE;
	} else {
		while ( ( dirent_p = readdir(dir_src_p) ) != NULL ){
		/* if file_name_str == '*' then copy all or copy selective files*/
		if( (!strncmp(file_name_str, "*", sizeof(char))) ||
				(strstr(dirent_p->d_name, file_name_str) != NULL) ){
					sprintf(source_file,"%s%s", dirsrc, dirent_p->d_name);
					sprintf(target_file,"%s%s", dirtar, dirent_p->d_name);
					FLOG(FLOG_HELP_FUNC,"source_file=%s, target_file=%s \n", source_file, target_file);
					copyFile( source_file, target_file);
			}
		}
		closedir(dir_src_p);
	}

	if (dir_trg_p)
		closedir(dir_trg_p);

	return retVal;
}

#if 0 //delete after merge
int file_exists(const char *fname)
{
	if (access(fname, F_OK) != -1) {
		FLOG(FLOG_HELP_FUNC,"fname exist= |%s|\n", fname);
		return 1;
	} else {
		FLOG(FLOG_HELP_FUNC,"fname doesn't exist= |%s|\n", fname);
	}
	return 0;
}
#else
int file_exists(const char *fname) {
	FILE *file = fopen(fname, "r");
	if (file)
	{
		fclose(file);
		return 1;
	}
	return 0;
}
#endif

char *paramValueFromObjGet(ObjList *objPtr, char *objName, char *paramName)
{
	ObjList   *tmpObj = NULL;
	ParamList *param = NULL;
	char      *paramValue = NULL;

	if(objPtr == NULL)
	{
		printf("%s; Illegal objPtr (0x%x) ==> ABORT!\n", __FUNCTION__, (unsigned int)objPtr);
		return NULL;
	}

	FOR_EACH_OBJ(objPtr, tmpObj)
	{
		if (!strcmp(GET_OBJ_NAME(tmpObj), objName))
		{
			FOR_EACH_PARAM(tmpObj, param)
			{
				if (!strcmp(GET_PARAM_NAME(param), paramName))
				{
					paramValue = GET_PARAM_VALUE(param);
					return paramValue;
				}
			}
		}
	}

	return NULL;
}

char *paramValueFromObjBufGet(ObjList *objPtr, char *objName, char *paramName)
{
	char *paramValue = paramValueFromObjGet(objPtr, objName, paramName);

	if (paramValue == NULL)
	{
		printf("GET PARAMTER FROM DB FAILED - EMPTY PARAMTER\n");
		return globalEmptyString;
	}
	else
	{
		return paramValue;
	}
}

char *paramValueFromSubObjGet(ObjList *subObjDB, char *paramName)
{
	ParamList *param = NULL;
	char      *paramValue = NULL;

	FOR_EACH_PARAM(subObjDB, param)
	{
		if (!strcmp(GET_PARAM_NAME(param), paramName))
		{
			paramValue = GET_PARAM_VALUE(param);
			return paramValue;
		}
	}

	return paramValue;
}

void addObjListIfNotExist(ObjList *objPtr, char *objName)
{
	ObjList *tmpObj = NULL;

	FOR_EACH_OBJ(objPtr, tmpObj)
	{
		if (!strcmp(GET_OBJ_NAME(tmpObj), objName))
		{
			printf("%s; Object '%s' already exists ==> Cont...\n", __FUNCTION__, objName);
			return;
		}
	}

	help_addObjList(objPtr, objName, 0, 0, 0, 0);
}

/*
 C library function
 ------------------
 ret = rename(oldname, newname);
 ret = remove(filename);
 ret = mkdir(path,(mode_t)(S_IRWXO | S_IRWXG | S_IRWXU));
 ret = chmod(filename, S_IRWXU|S_IRWXG|S_IROTH|S_IWOTH);
*/
