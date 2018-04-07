/*
**	debug.h
**
**
*/
#ifndef __DEBUGH_INT__
#define __DEBUGH_INT__

////////////////////////////////////////////////////////////////////////////////
//
// Debug Message 	
//
////////////////////////////////////////////////////////////////////////////////
static char *args2str(char *buf, int buf_size, char *s, ...)
{
	va_list args;
	
	if (IsNULL_PTR(buf) || IsNULL_PTR(s) || buf_size <= 0)
	{
		return NULL;
	}
	
	va_start(args, s);
	vsnprintf(buf, buf_size, s, args);
	va_end(args);
	return buf;
}

#define DEBUG_ERR(__TITLE__,...)	AdvDBG_ODS(ADVDBG_ERR,"%s[%s(LINE:%d)]:%s\n",((__TITLE__==NULL)?"":__TITLE__),__FILE__,__LINE__,__VA_ARGS__);
#define DEBUG_INFO(__TITLE__,...)	AdvDBG_ODS(ADVDBG_INFO,"%s[%s(LINE:%d)]:%s\n",((__TITLE__==NULL)?"":__TITLE__),__FILE__,__LINE__,__VA_ARGS__);
#define DEBUG_NOTICE(__TITLE__,...)	AdvDBG_ODS(ADVDBG_NOTICE,"%s[%s(LINE:%d)]:%s\n",((__TITLE__==NULL)?"":__TITLE__),__FILE__,__LINE__,__VA_ARGS__);
#define DEBUG_WARNING(__TITLE__,...)	AdvDBG_ODS(ADVDBG_WARNING,"%s[%s(LINE:%d)]:%s\n",((__TITLE__==NULL)?"":__TITLE__),__FILE__,__LINE__,__VA_ARGS__);

#endif	// !__DEBUGH_INT__

