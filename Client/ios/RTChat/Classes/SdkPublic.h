//
//  SdkPublic.h
//  RTChat
//
//  Created by raymon_wang on 14-8-5.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef RTChat_SdkPublic_h
#define RTChat_SdkPublic_h

const char *avar(const char *pszFmt,...)
{
	static  char szBuffer[1024];
	va_list ap;
	va_start(ap,pszFmt);
	vsnprintf(szBuffer,1024,pszFmt,ap);
	va_end(ap);
	return szBuffer;
}

#endif
