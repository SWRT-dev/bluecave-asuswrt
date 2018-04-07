/*
**	adv_string
**
**
**
*/
#ifndef ADVSTRINGh
#define ADVSTRINGh
#if __WIN32__ 
#include <conio.h>
#endif	/* __WIN32__ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef NULL
#define NULL	0
#endif	/* NULL */
//---------------------------------------------------------------------------
/*
static int AdvSplitStr_Count(
	int start,
	char *szSource,
	const char *compare)
{
	int i, Ret = 1, len = 0;
	char *s = szSource;

	if (szSource == NULL) return 0;
	if ((len = strlen(szSource)) <= 0) return 0;
	if (strlen(compare) <= 0) return 0;
	if (start < 0 || start > len) return 0;

	for (i=start-1; i<len; i++) {
		if (strncmp(s, compare, strlen(compare)) == 0) {
			i += strlen(compare);
			s += strlen(compare);
			Ret ++;
		}
        s++;
	}

	return Ret;
}
*/
//---------------------------------------------------------------------------
/*
static void AdvSplitStr(
	int start,
	char *szSource,
	char *compare,
	char *buffer,
	int MaxRow,
	int RowSize)
{
	int i, RowCount = 0, CopySize = 0, len = 0;
	char *s = szSource, *buf = buffer, *ss = buffer;

	if (szSource == NULL) return;
	if (compare == NULL) return;
	if (buffer == NULL) return;
	if (MaxRow <= 0) return;
	if (RowSize <= 0) return;
	if (strlen(compare) <= 0) return;
	if ((len = strlen(szSource)) <= 0) return;
	if (start < 0 || start > len) return;
	
	s += start-1;
	for (i = start-1; i < len && RowCount < MaxRow; i++) {
		if (strncmp(s, compare, strlen(compare)) == 0) {
			CopySize = 0;
			RowCount++;
			buf += RowSize;
			s += strlen(compare);
			ss = buf;
			continue;
		}
		if (++CopySize <= RowSize)  {
			*ss = *s;
			ss++;
		}
		s++;
	}
	return;
}
*/
//---------------------------------------------------------------------------
/*
static char *AdvTrim_Right(
	char *szSource)
{
#if 0	
	char *pStr = 0;

	pStr = szSource + strlen(szSource) - 1;
	while((pStr >= szSource) && (*pStr == ' '))
		*pStr-- = '\0';

	return szSource;
#else
	char* back = szSource + strlen(szSource);
	while (isspace(*--back));
	*(back+1) = '\0';
	return szSource;	
#endif	
}
*/
//---------------------------------------------------------------------------
/*
static char *AdvTrim_Left(
	char *szSource)
{
#if 0
	char *pStr = 0;

	pStr = szSource;
	while ((*pStr == ' ') && (pStr >= szSource))
		pStr++;

	return pStr;
#else
	while (isspace(*szSource)) szSource++;
	return szSource;
#endif	
}
*/
//---------------------------------------------------------------------------
/*
static char *AdvTrim(
	char *szSource)
{
	return AdvTrim_Right(AdvTrim_Left(szSource));
}
*/
//---------------------------------------------------------------------------
/*
static char *AdvLowerCase(
	char *str)
{
	int i;
	char *p = str;
	
	for (i=0; i<(int)strlen(str) && p != NULL; i++, p++)
		*p = (char)tolower((int)*p);
	return str;
}
*/
//---------------------------------------------------------------------------
/*
static char *AdvUpperCase(
	char *str)
{
	char *p = str;
	int i;
	
	for (i=0; i<(int)strlen(str) && p != NULL; i++, p++)
		*p = (char)toupper((int)*p);
	return str;
}
*/
//---------------------------------------------------------------------------
/*
static char* dumpHEX(
	unsigned char *src,
	unsigned long src_size)
{
	int c;
	unsigned char *s = NULL, *ss = NULL;
	char *P = NULL, *PP = NULL, sss[33];
	unsigned long alloc_size = 0;
	
	if (src == NULL || src_size <= 0)
	{
		printf("\n");
		return NULL;
	}
	
	alloc_size = src_size * 4;
	s = &src[0];
	ss = &src[src_size];
	P = (char *)malloc(alloc_size);
	
	if (P == NULL)
	{
		printf("\n");
		return NULL;
	}
	
	memset(P, 0, alloc_size);
	for (c=0, PP=&P[0]; s<ss; s++)
	{
		memset(sss, 0, sizeof(sss));
		snprintf(sss, sizeof(sss)-1, "%02X%c", *s, (c>=19)?'\x0a':'\x20');
		strncpy(PP, sss, strlen(sss));
		PP += strlen(sss);
		if (c++>=19) c = 0;	
	}
	printf("%s\n", P);
	free(P);
	return NULL;	
}
*/
//---------------------------------------------------------------------------
/*
static void replaceStr(
	char *orig, 
	char *rep, 
	char *src, 
	unsigned long src_size, 
	char *dst, 
	unsigned long dst_size)
{
    char *s = NULL, *ss = NULL;
    char *d = NULL, *dd = NULL;
    if (orig == NULL ||
        rep == NULL ||
        src == NULL ||
        dst == NULL ||
        src_size <= 0 ||
        dst_size <= 0 ||
        strlen(orig) > src_size ||
        strlen(rep) > dst_size)
    {
        return;
    }

    s = &src[0];
    ss = &src[src_size];
    d = &dst[0];
    dd = &dst[dst_size];

    if (!strstr(s, orig))
    {
        memcpy(d, s, (src_size>dst_size)?dst_size:src_size);
        return;
    }

    for (; s<=ss && d<=dd ;)
    {
        if (strncmp(s, orig, strlen(orig)) == 0)
        {
            strncpy(d, rep, strlen(rep));
            d += strlen(rep);
            s += strlen(orig);
        }
        else
        {
            *d = *s;
            d ++;
            s ++;
        }
    }

    return;
}
*/
//---------------------------------------------------------------------------
static void AdvSplit_CombineStr(char *source, char *dim, char *header, char *unit, char *output)
{
	int len_128=128;
	char *tmp=strtok(source, dim);
	char result[len_128];
	memset(result, '\0', len_128);

	snprintf(result, sizeof(result), "%s%s%s", result, header, unit);

	while (tmp != NULL)
	{
		tmp = strtok(NULL, dim);
		if (tmp!=NULL)
		{
			strcat(result, dim);
			strcat(result, tmp);
		}
		else
			memcpy(output, result, strlen(result));
	}

	return;
}
//---------------------------------------------------------------------------
#endif 	/* ADVSTRINGh */
