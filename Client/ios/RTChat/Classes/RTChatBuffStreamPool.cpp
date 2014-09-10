//
//  RTChatBuffStreamPool.cpp
//  RTChat
//
//  Created by raymon_wang on 14-9-1.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "RTChatBuffStreamPool.h"

RTChatBuffStreamPool::RTChatBuffStreamPool(int poolsize) :
_currentwriteindex(0)
{
    _poolVec.resize(poolsize);
    _poolsize = poolsize;
}

RTChatBuffStreamPool::~RTChatBuffStreamPool()
{
    clear_data();
}

void RTChatBuffStreamPool::init()
{
    for (int i = 0; i < _poolVec.size(); i++) {
        _poolVec[i] = new StBuffInfo(128000);
    }
}

RTChatBuffStreamPool::StBuffInfo* RTChatBuffStreamPool::getAvailableBuff(const char *name)
{
    if (!name) {
        return NULL;
    }
    
    int index = haveTheBuffByName(name);
    if (index != -1) {
        StBuffInfo* info = _poolVec[index];
        info->needDownload = false;
        return info;
    }
    
    StBuffInfo* info = _poolVec[_currentwriteindex];
    info->needDownload = true;
    return info;
}

//更新当前位置的缓冲区内存
RTChatBuffStream* RTChatBuffStreamPool::updateCurrentBuff(const char* name, const char* ptr, int size)
{
    StBuffInfo* info = _poolVec[_currentwriteindex];
    
    //更改下次覆盖的内存索引位
    _currentwriteindex = (_currentwriteindex + 1 == _poolsize) ? 0 : (_currentwriteindex + 1);
    
    if (!info) {
        return NULL;
    }
    strncpy(info->buffname, name, sizeof(info->buffname));
    if (info->buffstream) {
        info->buffstream->resetBuffStream();
        info->buffstream->Write(ptr, size);
        info->needDownload = false;
    }
    
    return info->buffstream;
}

void RTChatBuffStreamPool::clear_data()
{
    for (auto it = _poolVec.begin(); it != _poolVec.end(); ++it) {
        SAFE_DELETE(*it);
    }
    
    _poolVec.clear();
}

int RTChatBuffStreamPool::haveTheBuffByName(const char* name)
{
    int res = -1;
    
    int i = 0;
    for (auto it = _poolVec.begin(); it != _poolVec.end(); ++it) {
        StBuffInfo* info = *it;
        if (!info) {
            i++;
            continue;
        }
        if (!strncasecmp(info->buffname, name, sizeof(info->buffname))) {
            res = i;
            break;
        }
    }
    
    return res;
}







