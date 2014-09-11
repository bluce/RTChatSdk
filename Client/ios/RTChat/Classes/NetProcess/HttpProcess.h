//
//  HttpProcess.h
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-9-2.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#ifndef __RTChat__HttpProcess__
#define __RTChat__HttpProcess__

#include <iostream>
#include <map>

enum HttpDirection {
    HttpProcess_DownLoad,
    HttpProcess_Upload,
};

typedef std::function<void (HttpDirection direction, const char* ptr, int size)> CallBackFunc;

class HttpProcess {
public:
    HttpProcess();
    virtual ~HttpProcess();
    
    static HttpProcess& instance();
    
    void registerCallBack(const CallBackFunc& func);
    
    void postContent(const char* urlstr, const unsigned char* data, int datasize, std::map<const char*, const char*>& params, bool needcallback = true);
    
    void requestContent(const char *urlstr);
    
private:
    bool            _isrunning;
    CallBackFunc    _func;
};

#endif /* defined(__RTChat__HttpProcess__) */
