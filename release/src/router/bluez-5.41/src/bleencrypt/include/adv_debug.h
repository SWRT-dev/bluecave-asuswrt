/*
**	adv_debug.h
**
**
**
*/
#ifndef ADVDEBUGh
#define ADVDEBUGh
#include <stdio.h> 
#include <stdlib.h>
 
#ifdef __WIN32__ 
#include <windows.h>	/* Win32 header file */
#include <time.h>		/* Win32 header file */
#elif __ANDROID__
#include <android/log.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#else
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#endif	/* __WIN32__ */

#ifndef NULL
#define NULL 	0
#endif	/* NULL */
//---------------------------------------------------------------------------
#define	ADVDBG_EMERG	0	/* system is unusable */
#define	ADVDBG_ALERT	1	/* action must be taken immediately */
#define	ADVDBG_CRIT		2	/* critical conditions */
#define	ADVDBG_ERR		3	/* error conditions */
#define	ADVDBG_WARNING	4	/* warning conditions */
#define	ADVDBG_NOTICE	5	/* normal but significant condition */
#define	ADVDBG_INFO		6	/* informational */
#define	ADVDBG_DEBUG	7	/* debug-level messages */

typedef struct _AdvDBG_CODE_t {
	char*	c_name;
	int		c_code;
} AdvDBG_CODE, *AdvDBG_CODE_PTR;

/*
static struct _AdvDBG_CODE_t __AdvDBG_CODE_TBL[] = {
	{	"emerg", 	ADVDBG_EMERG	},
	{	"alert", 	ADVDBG_ALERT	},
	{	"crit", 	ADVDBG_CRIT		},
	{	"err", 		ADVDBG_ERR		},
	{	"warning", 	ADVDBG_WARNING	},
	{	"notice", 	ADVDBG_NOTICE	},
	{	"info", 	ADVDBG_INFO		},
	{	"debug", 	ADVDBG_DEBUG	},
	{	NULL, 		-1				},
};
*/

#define __AdvDBG_Find_Code(code, name) do {	\
	AdvDBG_CODE_PTR pCodeTbl = &__AdvDBG_CODE_TBL[0]; \
	while (pCodeTbl->c_name != NULL && pCodeTbl->c_code != -1) \
	{ \
		if (pCodeTbl->c_code == code) \
		{ \
			name = pCodeTbl->c_name; \
			break; \
		} \
        pCodeTbl ++; \
	} \
} while(0)
//---------------------------------------------------------------------------
/*
static void __AdvDBG_OutputString(
	char *message, ...)
{
	char *msg = NULL;
	va_list args;
	unsigned long alloc_size = 0;
	
	if (message == NULL)
	{
		return;
	}
	
	alloc_size = strlen(message) + 1;	
	msg = (char *)malloc(alloc_size);
	if (msg == NULL)
	{
		return;
	}
	
	memset(msg, 0, alloc_size);
	va_start(args, message);
	vsnprintf(msg, alloc_size-1, message, args);
	va_end(args);
	
#ifdef __WIN32__
	OutputDebugStringA(msg);	// Win32 API
#elif __ANDROID__
	__android_log_print(ANDROID_LOG_DEBUG,"ANDROID_LOG","%s",msg);
#else
	printf("%s\n", msg);
#endif	// __WIN32__
	
	free(msg);
	return;		
}
*/
//---------------------------------------------------------------------------
#define ADVDBG_LOG_PID		0x01
#define ADVDBG_LOG_DATE		0x02
#define ADVDBG_LOG_TIME		0x04
#define ADVDBG_LOG_TITLE	0x08
#define ADVDBG_LOG_ALL		ADVDBG_LOG_PID | ADVDBG_LOG_DATE | ADVDBG_LOG_TIME | ADVDBG_LOG_TITLE

#define __AdvDBG_MAX_MSG_SIZE		8193
#define __AdvDBG_MAX_TITLE_SIZE		64

static int 	__AdvDBG_SET_PRIO = -1;
//static char	__AdvDBG_SET_LOGFLAG = 0x00;

#define AdvDBG_Enable(prio, flag) do { \
	__AdvDBG_SET_LOGFLAG = 0x00; \
	__AdvDBG_SET_LOGFLAG |= flag; \
	__AdvDBG_SET_PRIO = (prio < 0 && prio > 7) ? -1 : prio; \
} while(0)

#define AdvDBG_Disable do { \
	__AdvDBG_SET_LOGFLAG = 0x00; \
	__AdvDBG_SET_PRIO = -1; \
} while(0)

#define AdvDBG_SetFlag(flag) do { \
	if (__AdvDBG_SET_PRIO != -1) \
	{ \
		__AdvDBG_SET_LOGFLAG |= flag; \
	} \
} while(0)

#define AdvDBG_GetFlag(flag) do { \
	*flag = __AdvDBG_SET_LOGFLAG; \
} while(0)

#define AdvDBG_ClearFlag ( \
	__AdvDBG_SET_LOGFLAG = 0x00; \
)

#define AdvDBG_TRACE_ENTER(msg) do { \
	AdvDBG_ODS(ADVDBG_DEBUG, "(%s) >>>>>>>", msg); \
} while(0)

#define AdvDBG_TRACE_EXIT(msg) do { \
	AdvDBG_ODS(ADVDBG_DEBUG, "(%s) <<<<<<<", msg); \
} while(0)


// >>> Add by MAX 2015.06.11, Fix memory usage issue
#ifdef __WIN32__
#define AdvDBG_ODS(LEVEL,...) do {\
	char __SS__[10241];\
	memset(__SS__,0,sizeof(__SS__));\
	_snprintf(__SS__,sizeof(__SS__)-1,__VA_ARGS__);\
	OutputDebugStringA(__SS__);\
}while(0)
#elif __ANDROID__
#define AdvDBG_ODS(LEVEL,...) if ((__AdvDBG_SET_PRIO>=0&&__AdvDBG_SET_PRIO<=7)&&LEVEL<=__AdvDBG_SET_PRIO) { __android_log_print(ANDROID_LOG_DEBUG,"ANDROID_LOG",__VA_ARGS__); }
#else
#define AdvDBG_ODS(LEVEL,...) if ((__AdvDBG_SET_PRIO>=0&&__AdvDBG_SET_PRIO<=7)&&LEVEL<=__AdvDBG_SET_PRIO) { printf(__VA_ARGS__); }
#endif	// __WIN32__

// <<< Add by MAX 2015.06.11, Fix memory usage issue

/*
static void AdvDBG_ODS1(
	int level,
	char *msg, ...)
{
	char MSGBUF[__AdvDBG_MAX_MSG_SIZE], TITLE[__AdvDBG_MAX_TITLE_SIZE];
	char *slevel = NULL, sdate[12], stime[12], spid[33];
	time_t ti;
	struct tm *cur_tm;
	va_list args;

	if ((__AdvDBG_SET_PRIO >= 0 && __AdvDBG_SET_PRIO <= 7) && level <= __AdvDBG_SET_PRIO)
	{
		time(&ti);
		cur_tm = (struct tm *)localtime(&ti);

		memset(spid, 0x00, sizeof(spid));
		if ((__AdvDBG_SET_LOGFLAG & ADVDBG_LOG_PID) == ADVDBG_LOG_PID)
		{
#ifdef __LINUX__
			snprintf(spid, sizeof(spid), "[%d]", (int)getpid());
#elif __DARWIN__
			snprintf(spid, sizeof(spid), "[%d]", (int)getpid());
#endif 			
		}

		memset(sdate, 0x00, sizeof(sdate));
		if ((__AdvDBG_SET_LOGFLAG & ADVDBG_LOG_DATE) == ADVDBG_LOG_DATE)
		{
#ifndef __WIN32__
			snprintf(sdate, sizeof(sdate), "%04d/%02d/%02d ",
				1900+cur_tm->tm_year, cur_tm->tm_mon+1, cur_tm->tm_mday);
#else	// !__WIN32__
			_snprintf(sdate, sizeof(sdate), "%04d/%02d/%02d ",
				1900+cur_tm->tm_year, cur_tm->tm_mon+1, cur_tm->tm_mday);
#endif	// !__WIN32__
		}

		memset(stime, 0x00, sizeof(stime));
		if ((__AdvDBG_SET_LOGFLAG & ADVDBG_LOG_TIME) == ADVDBG_LOG_TIME)
		{
#ifndef __WIN32__
			snprintf(stime, sizeof(stime), "%02d:%02d:%02d ",
				cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec);			
#else	// !__WIN32__
			_snprintf(stime, sizeof(stime), "%02d:%02d:%02d ",
				cur_tm->tm_hour, cur_tm->tm_min, cur_tm->tm_sec);
#endif	// !__WIN32__
		}

		slevel = NULL;
		if ((__AdvDBG_SET_LOGFLAG & ADVDBG_LOG_TITLE) == ADVDBG_LOG_TITLE)
		{
			__AdvDBG_Find_Code(level, slevel);
		}

		memset(TITLE, 0x00, sizeof(TITLE));
#ifndef __WIN32__
		snprintf(TITLE, sizeof(TITLE), "%s%s%s%s : ",
			(strlen(sdate) > 0) ? sdate : "", (strlen(stime) > 0) ? stime : "",
			(strlen(spid) > 0) ? spid : "", (slevel != NULL) ? slevel : "");
#else	// !__WIN32__
		_snprintf(TITLE, sizeof(TITLE), "%s%s%s%s : ",
			(strlen(sdate) > 0) ? sdate : "", (strlen(stime) > 0) ? stime : "",
			(strlen(spid) > 0) ? spid : "", (slevel != NULL) ? slevel : "");
#endif	// !__WIN32__

		memset(MSGBUF, 0x00, sizeof(MSGBUF));
		va_start(args, msg);
		vsnprintf(MSGBUF, sizeof(MSGBUF), msg, args);
		va_end(args);

		__AdvDBG_OutputString("%s%s", (strlen(TITLE) > 0) ? TITLE : "", MSGBUF);
	}

	return;
}
*/
//---------------------------------------------------------------------------
#endif	/* ADVDEBUGh */
