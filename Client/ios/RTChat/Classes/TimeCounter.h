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

typedef std::function<void (void)> callbackfunc;

class TimeCounter {
public:
    TimeCounter();
    virtual ~TimeCounter();
    
    void resetTicker();
    
    void registerTimeOutCallBack(int period, const callbackfunc& func);
    
    void startCounter();
    
    void stopCounter();
    
protected:
    static bool Run(ThreadObj obj);
    bool Process();
    
private:
    int                 _ticker;
    int                 _period;
    ThreadWrapper*      _thread;
    pthread_mutex_t     _mutexlock;
    callbackfunc        _pfunc;
};

#endif /* defined(__RTChat__TimeCounter__) */
