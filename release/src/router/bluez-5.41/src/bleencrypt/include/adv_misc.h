/*
**	adv_misc.h
**
**
**
**
*/
#ifndef __ADVMISCH__
#define __ADVMISCH__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//---------------------------------------------------------------------------
#ifndef NULL
#define NULL	0
#endif	/* !NULL */

#ifndef MALLOC
#define MALLOC(__PTR, __VARTYPE, __ALLOCSIZE) do {\
	__PTR = NULL; \
	__PTR = (__VARTYPE *)malloc(__ALLOCSIZE); \
	if (__PTR != NULL) \
	{ \
		memset(__PTR, 0, __ALLOCSIZE); \
	} \
}while(0)
#endif	/* !MALLOC */

#ifndef MFREE
#define MFREE(__PTR) do {\
	if (__PTR != NULL) \
	{ \
		free(__PTR); \
		__PTR = NULL; \
	} \
}while(0)
#endif	/* !MFREE */

#ifndef IsNULL_PTR 
#define IsNULL_PTR(__PTR) ((__PTR == NULL))
#endif	/* !IsNULL_PTR */

#ifndef IsEmpty_STR
#define IsEmpty_STR(__STR) ((strlen(__STR) <= 0))
#endif	/* !IsEmpty_STR */

#ifndef IsZERO_MAC
#define IsZERO_MAC(__MAC) ((memcmp(__MAC, "\x00\x00\x00\x00\x00\x00", 6) == 0))
#endif	/* !IsZERO_MAC */
//---------------------------------------------------------------------------
#endif	/* __ADVMISCH__ */
