/*
**	utility.h
**
**
**
*/
#ifndef __UTILITYH__
#define __UTILITYH__
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <adv_misc.h>
#include <adv_debug.h>
#include <adv_verify.h>
#include <adv_inet.h>
#include <Debug-Int.h>
#include "encrypt.h"

static int debug = 1;

////////////////////////////////////////////////////////////////////////////////
//
// Debug Message 	
//
////////////////////////////////////////////////////////////////////////////////
#define MAX_DBGMSG_LENGTH	4097

#ifdef __SERVER__
#define LIBRARY_NAME	"<<SERVER>>\0"
#else	
#define LIBRARY_NAME	"<<CLIENT>>\0"
#endif

#define DBG_ERR(...) do {\
	if (debug == 1)\
	{\
		char __BUF__[MAX_DBGMSG_LENGTH];\
		memset(__BUF__,0,sizeof(__BUF__));\
		DEBUG_ERR(LIBRARY_NAME,args2str(__BUF__,sizeof(__BUF__)-1,__VA_ARGS__));\
	}\
}while(0)

#define DBG_INFO(...) do {\
	if (debug == 1)\
	{\
		char __BUF__[MAX_DBGMSG_LENGTH];\
		memset(__BUF__,0,sizeof(__BUF__));\
		DEBUG_INFO(LIBRARY_NAME,args2str(__BUF__,sizeof(__BUF__)-1,__VA_ARGS__));\
	}\
}while(0)

#define DBG_NOTICE(...) do {\
	if (debug == 1)\
	{\
		char __BUF__[MAX_DBGMSG_LENGTH];\
		memset(__BUF__,0,sizeof(__BUF__));\
		DEBUG_NOTICE(LIBRARY_NAME,args2str(__BUF__,sizeof(__BUF__)-1,__VA_ARGS__));\
	}\
}while(0)

#define DBG_WARNING(...) do {\
	if (debug == 1)\
	{\
		char __BUF__[MAX_DBGMSG_LENGTH];\
		memset(__BUF__,0,sizeof(__BUF__));\
		DEBUG_WARNING(LIBRARY_NAME,args2str(__BUF__,sizeof(__BUF__)-1,__VA_ARGS__));\
	}\
}while(0)

#define DBG_TRACE_LINE do {\
	if (debug == 1)\
	{\
		char __BUF__[MAX_DBGMSG_LENGTH];\
		memset(__BUF__,0,sizeof(__BUF__));\
		DEBUG_INFO(LIBRARY_NAME,args2str(__BUF__,sizeof(__BUF__)-1,"TRACE LINE!!!"));\
	}\
}while(0)

#endif	// __ENCRYPT_MAINH__ 

