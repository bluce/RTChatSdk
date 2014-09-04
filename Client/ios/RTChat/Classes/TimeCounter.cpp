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

TimeCounter::TimeCounter() :
_thread(NULL),
_infoid(0)
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

void TimeCounter::resetCallBackInfoTime(int id)
{
    pthread_mutex_lock(&_mutexlock);
    for (auto it = _callBackInfoVec.begin(); it != _callBackInfoVec.end(); ++it) {
        StCallBackInfo& info = *it;
        if (info._id == id) {
            info._starttime = time(NULL);
            break;
        }
    }
    pthread_mutex_unlock(&_mutexlock);
}

void TimeCounter::destroyCallBackInfo(int id)
{
    pthread_mutex_lock(&_mutexlock);
    for (auto it = _callBackInfoVec.begin(); it != _callBackInfoVec.end();) {
        StCallBackInfo& info = *it;
        if (info._id == id) {
            _callBackInfoVec.erase(it);
            break;
        }
    }
    pthread_mutex_unlock(&_mutexlock);
}

int TimeCounter::registerTimeOutCallBack(int period, const callbackfunc &func)
{
    int ret;
    pthread_mutex_lock(&_mutexlock);
    ret = _infoid;
    _callBackInfoVec.push_back(StCallBackInfo(_infoid++, period, func));
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
//    pthread_mutex_lock(&_mutexlock);
//    _ticker++;
//    if (_ticker > _period) {
//        _pfunc();
//        stopCounter();
//    }
//    pthread_mutex_unlock(&_mutexlock);
    
    pthread_mutex_lock(&_mutexlock);
    time_t currenttime = time(NULL);
    for (auto it = _callBackInfoVec.begin(); it != _callBackInfoVec.end();) {
        const StCallBackInfo& info = *it;
        if (currenttime - info._starttime >= info._period) {
            info._func(info._period);
            _callBackInfoVec.erase(it);
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





