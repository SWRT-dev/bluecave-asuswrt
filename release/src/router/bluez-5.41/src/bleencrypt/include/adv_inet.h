/*
**	adv_inet.h
**
**
**
*/
#ifndef ADVINETh
#define ADVINETh
#ifdef __WIN32__
#include <stdlib.h>
#include <stdio.h>
#endif	/* __WIN32__ */

#ifndef NULL
#define NULL	0
#endif	/* NULL */

#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */

#define __htob(i, c) do {\
	if ('0' <= c && c <= '9') \
		*i = c - '0'; \
	else if ('a' <= c && c <= 'f') \
		*i = c - 'a' + 10; \
	else if ('A' <= c && c <= 'F') \
		*i = c - 'A' + 10; \
	else \
		*i = 0; \
} while(0)
//---------------------------------------------------------------------------
/*
static int AdvInet_IPAvailable(
	unsigned long inaddr,
	unsigned long inmask)
{
	return ((inaddr & inmask) == inaddr) ? 0 : 1;
}
*/
//---------------------------------------------------------------------------
/*
static int AdvInet_SubnetComp(
	unsigned long ip1, 
	unsigned long mip1, 
	unsigned long ip2, 
	unsigned long mip2)
{
	return ((ip1 & mip1) == (ip2 & mip2)) ? 1 : 0;
}
*/
//---------------------------------------------------------------------------
/*
static int AdvInet_MaskToInt(
	unsigned long inMask)
{
	int i, ret = 0;
	for (i = 0; i < 32; i++) {
		if ((inMask & (0x80000000 >> i)) == 0x00)
			break;
	}

	if (i < 31)
		ret = i;
	else
		i = 0;

	return ret;
}
*/
//---------------------------------------------------------------------------
/*
static unsigned long AdvInet_AtoN(
	const char *inaddr)
{
	int size = strlen(inaddr), c = 0;
	int i = 0, cnt = 0;
	unsigned long ret = 0, val = 0;
	char *p = (char *)inaddr;
	
	if (inaddr == NULL || strlen(inaddr) <= 0)
	{
		return ret;
	}
	
	for (i = 24, cnt = 0 ; c <= size;)
	{
		val = strtoul(p, &p, 10);
		if (val > 255) return 0;
		while (*p == ' ') p++;
		if (*p == '.')
		{
			ret |= (val << i);
			if (i > 0) i-=8;
			p++;
			cnt++;
		}
		else 
		{
			ret |= val;
			if (cnt >= 3) break;
		}
		c++;
	}

	return ret;
}
*/
//---------------------------------------------------------------------------
static char *AdvInet_NCtoA(
	unsigned char *inaddr,
	char *buff)
{
	char tmp[32];

	memset(tmp, '\0', sizeof(tmp));

	snprintf(tmp, sizeof(tmp), "%d.%d.%d.%d", 
		(inaddr[3] & 255), (inaddr[2] & 255), (inaddr[1] & 255), (inaddr[0] & 255));

	memcpy(buff, tmp, sizeof(tmp));
	return buff;
}
//---------------------------------------------------------------------------
/*
static void AdvInet_StrToMac(
	char *szString, 
	unsigned char *Mac)
{	
	int i, j, k, len;
	unsigned char *p = NULL;
	
	if (szString == NULL) return;
	if (Mac == NULL) return;
#if 0	// >>> Remove by MAX 2009/02/04
	if ((len = strlen(szString)) != 12) return;
#endif	// <<< Remove by MAX 2009/02/04
	len = strlen(szString);
	p = Mac;
	
	for (k = 0; k < len; k+=2) {
		i = j = 0;
		__htob(&i, szString[k]);
		__htob(&j, szString[k+1]);
		*p = (i << 4) | j;
		p++;
	}
	
	return;	
}
*/
#ifdef __cplusplus
}
#endif	/* __cplusplus */
//---------------------------------------------------------------------------
#endif	/* ADVINETh */
