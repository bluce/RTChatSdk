//
//  SdkPublic.h
//  RTChat
//
//  Created by raymon_wang on 14-8-5.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef RTChat_SdkPublic_h
#define RTChat_SdkPublic_h

std::string avar(const char *pszFmt,...)
{
	char szBuffer[1024] = {0};
	va_list ap;
	va_start(ap,pszFmt);
	vsnprintf(szBuffer,1024,pszFmt,ap);
	va_end(ap);
	return szBuffer;
}

#define SAFE_DELETE(p) if(p) {delete p; p = NULL;}

#define SAFE_DELETEARRAY(p) if(p) {delete [] p; p = NULL;}

typedef unsigned int DWORD;
typedef unsigned short WORD;

template <class T>
inline void constructDynamic(T *ptr) {
    new ((void*)ptr) T();
};

#define BUFFER_CMD(cmd, name, len) char buffer##name[len]; \
    cmd* name = (cmd*)buffer##name; \
    constructDynamic(name); \

void sdklog(const char *pszFmt,...)
{
//#ifdef DEBUG
	static  char szBuffer[1024];
	va_list ap;
	va_start(ap,pszFmt);
	vsnprintf(szBuffer,1024,pszFmt,ap);
	va_end(ap);
	
    printf("%s\n", szBuffer);
//#endif
};

#endif
