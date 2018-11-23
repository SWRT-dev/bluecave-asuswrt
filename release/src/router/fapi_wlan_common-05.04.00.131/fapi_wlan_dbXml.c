#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "help_objlist.h"
#include "fapi_wlan_ezxml.h"

#ifndef LOG_LEVEL
uint16_t   LOGLEVEL = SYS_LOG_DEBUG + 1;
#else
uint16_t   LOGLEVEL = LOG_LEVEL + 1;
#endif
#ifndef LOG_TYPE
uint16_t   LOGTYPE = SYS_LOG_TYPE_FILE;
#else
uint16_t   LOGTYPE = LOG_TYPE;
#endif

#define MAXNODES 1000
#define INSTANCE "Instance"
#define DBSOURCE "defaultSource"

typedef struct xmlNode{
	ezxml_t xml;
	char index[MAX_LEN_PARAM_VALUE];
	char defaultSource[MAX_LEN_PARAM_VALUE];
	char objectName[MAX_LEN_PARAM_VALUE];
} xmlNode;

static int printParams(ezxml_t xml, ObjList **objlistPtr, char *objectStr)
{
	while (( xml->ordered != NULL ) && ( xml->child == NULL ))
	{
		if ( xml->child == NULL )
		{
			printf("NAME: %s VALUE: %s\n", ezxml_name(xml), ezxml_txt(xml));
			HELP_EDIT_NODE(*objlistPtr, objectStr, ezxml_name(xml), ezxml_txt(xml), 0, 0);
			xml = xml->ordered;
		}
	}
	
	if ( xml->child == NULL )
	{
		printf("NAME: %s VALUE: %s\n", ezxml_name(xml), ezxml_txt(xml));
		HELP_EDIT_NODE(*objlistPtr, objectStr, ezxml_name(xml), ezxml_txt(xml), 0, 0);
	}	
	return 0;
}

static int printObjectName(ezxml_t xml, char *objectStr)
{
	char tmp[MAX_LEN_PARAM_VALUE] = { '\0' };
	const char *attrValue = NULL;
	bool ending = true;
	const char *source;
	
	while ( xml->parent != NULL )
	{
		tmp[0] = '\0';
		
		source = ezxml_name(xml);
		
		if (source != NULL)
		{
			strcat(tmp, source);
		}
		
		if ( xml != NULL )
		{
			attrValue = ezxml_attr(xml, INSTANCE);
			if ( attrValue != NULL )
			{
				strcat(tmp, ".");
				strcat(tmp, attrValue);
			}
		}
		
		if ( ! ending )
		{
			strcat(tmp, ".");
		}
		
		strcat(tmp, objectStr);
		strcpy(objectStr, tmp);
		xml = xml->parent;
		ending = false;
	}

	//Add ROOT
	if ( ezxml_name(xml) != NULL ) 
	{
		tmp[0] = '\0';
		sprintf(tmp, "%s.%s", ezxml_name(xml), objectStr);
		strcpy(objectStr, tmp);
	}
	
	printf("OBJECT NAME: %s \n", objectStr);
	return 0;
}

int main(int argc, char **argv)
{
	ObjList *objlistPtr = NULL; 
	
	char objectStr[MAX_LEN_PARAM_VALUE] = { '\0' };
	char outputFile[MAX_LEN_PARAM_VALUE] = { '\0' };
	char outputPath[MAX_LEN_PARAM_VALUE] = { '\0' };
		
	xmlNode nodesArray[MAXNODES];
	ezxml_t xmlPtr;
	
	int i=0;
	int j=1;
	
	if (argc != 3) return fprintf(stderr, "usage: %s < XML file path > < output path >\n", argv[0]);
	strcpy(outputPath,argv[2]);
	xmlPtr = ezxml_parse_file(argv[1]);
	nodesArray[0].xml = xmlPtr;
	
	for ( i=0 ; i <= j ; i++)
	{	
		xmlPtr = nodesArray[i].xml;
		while ( xmlPtr != NULL )
		{
			if (xmlPtr->child != NULL )
			{
				objectStr[0] = '\0';
				outputFile[0] = '\0';
				
				objlistPtr = HELP_CREATE_OBJ(SOPT_OBJVALUE);
				nodesArray[j].xml = xmlPtr->child;
				
				printObjectName(xmlPtr, objectStr);
				help_addObjList(objlistPtr, objectStr, 0, 0, 0, 0);
				printParams(xmlPtr->child, &objlistPtr, objectStr);
				
				strcpy(outputFile, outputPath);
				strcat(outputFile,objectStr);
				HELP_STORELOCALDB(objlistPtr, outputFile);
				HELP_DELETE_OBJ(objlistPtr, SOPT_OBJVALUE, FREE_OBJLIST);
				
				j++;
			}
			
			xmlPtr = xmlPtr->ordered;
			//Check if MaxNodes reached
		}
	}
	
	i = fprintf(stderr, "%s", ezxml_error(xmlPtr));
	ezxml_free(xmlPtr);
	return (i) ? 1 : 0;
}
