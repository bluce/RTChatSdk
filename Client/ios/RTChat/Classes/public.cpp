//
//  public.cpp
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-8-14.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#include "public.h"
#include "BridgeTools.h"
#include "NetProcess/HttpProcess.h"
#include "defines.h"
#include <sstream>
#include <unistd.h>

static Public* s_Public = NULL;

Public::Public()
{
    s_Public = this;
    
    pthread_mutex_init(&_mutexLock, 0);
}

Public::~Public()
{
    if (_logfilestream.is_open()) {
        _logfilestream.close();
    }
}

void Public::init(const char* uid)
{
#ifdef DEBUG
    _logFullName = BridgeTools::getWritablePath() + "rtchatsdklog.txt";
    _logBackFullName = BridgeTools::getWritablePath() + "rtchatsdklog.bak";
    if (isLogfileExit()) {
        rename(_logFullName.c_str(), _logBackFullName.c_str());
        
        uploadLogFile(uid);
    }
    
    _logfilestream.open(_logFullName);
#endif
}

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
    
    if (s_Public) {
        s_Public->writeLogToDisk(szBuffer);
    }
#endif
};

void Public::writeLogToDisk(char *buff)
{
    pthread_mutex_lock(&_mutexLock);
    if (_logfilestream.is_open()) {
        _logfilestream << buff << endl;
    }
    pthread_mutex_unlock(&_mutexLock);
}

bool Public::isLogfileExit()
{
    return !access(_logFullName.c_str(), F_OK);
}

void Public::uploadLogFile(const char* uid)
{
    ifstream reads(_logBackFullName);
    stringstream buffer;
    buffer << reads.rdbuf();
    std::string contents(buffer.str());
    
    std::map<const char*, const char*> params;
    params["uid"] = uid;
    params["os"] = "ios";
    HttpProcess::instance().postContent(LogUpLoadUrlHead, (const unsigned char*)contents.c_str(), contents.length(), params, false);
}


