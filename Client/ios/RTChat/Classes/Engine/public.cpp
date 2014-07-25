#include "public.h"
#include <stdio.h> 
#include <stdlib.h> 

const char *avar(const char *pszFmt,...)
{
	static  char szBuffer[1024];
	va_list ap;
	va_start(ap,pszFmt);
	vsnprintf(szBuffer,1024,pszFmt,ap);
	va_end(ap);
	return szBuffer;
}
