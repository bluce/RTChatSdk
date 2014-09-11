//
//  public.h
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-8-14.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#ifndef __RTChat__public__
#define __RTChat__public__

#include <iostream>
#include <fstream>

using namespace std;

class Public {
public:
    Public();
    virtual ~Public();
    
    void init(const char* uid);
    
    static std::string SdkAvar(const char *pszFmt,...);
    
    static void sdklog(const char *pszFmt,...);
    
    void writeLogToDisk(char* buff);
    
    bool isLogfileExit();
    
    void uploadLogFile(const char* uid);
    
private:
    std::string     _logFullName;
    std::string     _logBackFullName;
    ofstream        _logfilestream;
    pthread_mutex_t _mutexLock;
};

#endif /* defined(__RTChat__public__) */
