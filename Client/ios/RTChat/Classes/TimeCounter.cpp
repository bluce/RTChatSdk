//
//  TimeCounter.cpp
//  RTChat
//
//  Created by raymon_wang on 14-8-27.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "TimeCounter.h"
#include "public.h"

TimeCounter::TimeCounter() :
_ticker(0),
_period(0),
_thread(NULL)
{
    pthread_mutex_init(&_mutexlock, 0);
    
    _thread = ThreadWrapper::CreateThread(TimeCounter::Run, this, kNormalPriority, "TimeCounterThread");
}

TimeCounter::~TimeCounter()
{
    if (_thread) {
        _thread->Stop();
        delete _thread;
        _thread = NULL;
    }
}

void TimeCounter::resetTicker()
{
    pthread_mutex_lock(&_mutexlock);
    _ticker = 0;
    pthread_mutex_unlock(&_mutexlock);
}

void TimeCounter::startCounter()
{
    unsigned int id = 0;
    if (_thread) {
        _thread->Start(id);
    }
}

void TimeCounter::stopCounter()
{
    if (_thread) {
        _thread->Stop();
    }
}

void TimeCounter::registerTimeOutCallBack(int period, const callbackfunc &func)
{
    _pfunc = func;
    _period = period;
}

bool TimeCounter::Run(ThreadObj obj)
{
    TimeCounter* mgr = static_cast<TimeCounter*>(obj);
    
    return mgr->Process();
}

bool TimeCounter::Process()
{
    timespec t;
    t.tv_sec = 1;
    t.tv_nsec = 0;
    nanosleep(&t, NULL);
    
    pthread_mutex_lock(&_mutexlock);
    _ticker++;
    if (_ticker > _period) {
        _pfunc();
        stopCounter();
    }
    pthread_mutex_unlock(&_mutexlock);
    
    return true;
}





