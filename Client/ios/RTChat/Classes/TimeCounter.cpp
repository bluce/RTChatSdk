//
//  TimeCounter.cpp
//  RTChat
//
//  Created by raymon_wang on 14-8-27.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "TimeCounter.h"
#include "public.h"

static TimeCounter* s_TimeCounter = NULL;

TimeCounter::TimeCounter():
_thread(NULL)
{
    pthread_mutex_init(&_mutexlock, 0);
    
    _thread = ThreadWrapper::CreateThread(TimeCounter::Run, this, kNormalPriority, "TimeCounterThread");
    
    startCounter();
}

TimeCounter::~TimeCounter()
{
    if (_thread) {
        _thread->Stop();
        delete _thread;
        _thread = NULL;
    }
}

TimeCounter& TimeCounter::instance()
{
    if (!s_TimeCounter) {
        s_TimeCounter = new TimeCounter();
    }
    
    return *s_TimeCounter;
}

void TimeCounter::startCounter()
{
    unsigned int id = 0;
    if (_thread) {
        _thread->Start(id);
    }
}

void TimeCounter::resetCallBackInfoTime(const char *keyname)
{
    pthread_mutex_lock(&_mutexlock);
    
    auto it = _callBackInfoMap.find(keyname);
    if (it != _callBackInfoMap.end()) {
        StCallBackInfo& info = *it->second;
        info._starttime = time(NULL);
    }
    
    pthread_mutex_unlock(&_mutexlock);
}

void TimeCounter::destroyCallBackInfo(const char *keyname)
{
    pthread_mutex_lock(&_mutexlock);
    
    auto it = _callBackInfoMap.find(keyname);
    if (it != _callBackInfoMap.end()) {
        _callBackInfoMap.erase(it);
    }
    pthread_mutex_unlock(&_mutexlock);
}

int TimeCounter::registerTimeOutCallBack(const char *keyname, int period, const callbackfunc &func)
{
    int ret = 0;
    pthread_mutex_lock(&_mutexlock);
    
    _callBackInfoMap[keyname] = new StCallBackInfo(period, func);

    pthread_mutex_unlock(&_mutexlock);
    
    return ret;
}

bool TimeCounter::Run(ThreadObj obj)
{
    TimeCounter* mgr = static_cast<TimeCounter*>(obj);
    
    return mgr->Process();
}

bool TimeCounter::Process()
{
    pthread_mutex_lock(&_mutexlock);
    time_t currenttime = time(NULL);
    for (auto it = _callBackInfoMap.begin(); it != _callBackInfoMap.end();) {
        const StCallBackInfo& info = *it->second;
        if (currenttime - info._starttime >= info._period) {
            info._func(info._period);
            _callBackInfoMap.erase(it++);
        }
        else {
            it++;
        }
    }
    pthread_mutex_unlock(&_mutexlock);
    
    timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 1000;
    nanosleep(&t, NULL);
    
    return true;
}





