//
//  TimeCounter.h
//  RTChat
//
//  Created by raymon_wang on 14-8-27.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RTChat__TimeCounter__
#define __RTChat__TimeCounter__

#include <iostream>
#include <functional>
#include "webrtc/system_wrappers/interface/thread_wrapper.h"

using namespace webrtc;

typedef std::function<void (int)> callbackfunc;

class TimeCounter {
public:
    struct StCallBackInfo {
        StCallBackInfo(int id, int period, const callbackfunc& func){
            _id = id;
            _period = period;
            _func = func;
            _starttime = time(NULL);
        }
        int             _id;
        callbackfunc    _func;
        uint64_t        _starttime;
        int             _period;
    };
    typedef std::vector<StCallBackInfo> CallBackInfoVec;
    
public:
    TimeCounter();
    virtual ~TimeCounter();
    
    static TimeCounter& instance();
    
    int registerTimeOutCallBack(int period, const callbackfunc& func);
    
    void startCounter();
    
    void resetCallBackInfoTime(int id);
    
    void destroyCallBackInfo(int id);
    
protected:
    static bool Run(ThreadObj obj);
    bool Process();
    
private:
    ThreadWrapper*      _thread;
    pthread_mutex_t     _mutexlock;
    callbackfunc        _pfunc;
    CallBackInfoVec     _callBackInfoVec;
    int                 _infoid;
};

#endif /* defined(__RTChat__TimeCounter__) */
