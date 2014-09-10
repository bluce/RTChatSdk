//
//  public.cpp
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-8-14.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#include "public.h"

std::string Public::SdkAvar(const char *pszFmt,...)
{
	char szBuffer[1024] = {0};
	va_list ap;
	va_start(ap,pszFmt);
	vsnprintf(szBuffer,1024,pszFmt,ap);
	va_end(ap);
	return szBuffer;
};

void Public::sdklog(const char *pszFmt,...)
{
#ifdef DEBUG
	static  char szBuffer[1024];
	va_list ap;
	va_start(ap,pszFmt);
	vsnprintf(szBuffer,1024,pszFmt,ap);
	va_end(ap);
	
    printf("%s\n", szBuffer);
#endif
};