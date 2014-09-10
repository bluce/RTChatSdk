//
//  TimeCounter.h
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-8-27.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#ifndef __RTChat__TimeCounter__
#define __RTChat__TimeCounter__

#include <iostream>
#include <functional>
#include <map>
#include "webrtc/system_wrappers/interface/thread_wrapper.h"

using namespace webrtc;

typedef std::function<void (int)> callbackfunc;

class TimeCounter {
public:
    struct StCallBackInfo {
        StCallBackInfo(int period, const callbackfunc& func) {
            _period = period;
            _func = func;
            _starttime = time(NULL);
        };
        callbackfunc    _func;
        uint64_t        _starttime;
        int             _period;
    };
    
    typedef std::map<std::string , StCallBackInfo*> CallBackInfoVec;
    
public:
    TimeCounter();
    virtual ~TimeCounter();
    
    static TimeCounter& instance();
    
    int registerTimeOutCallBack(const char* keyname, int period, const callbackfunc& func);
    
    void startCounter();
    
    void resetCallBackInfoTime(const char* keyname);
    
    void destroyCallBackInfo(const char* keyname);
    
protected:
    static bool Run(ThreadObj obj);
    bool Process();
    
private:
    ThreadWrapper*      _thread;
    pthread_mutex_t     _mutexlock;
    CallBackInfoVec     _callBackInfoMap;
};

#endif /* defined(__RTChat__TimeCounter__) */
