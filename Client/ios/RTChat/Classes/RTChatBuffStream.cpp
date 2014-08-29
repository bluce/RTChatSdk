//
//  RTChatBuffStream.cpp
//  RTChat
//
//  Created by raymon_wang on 14-8-26.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "RTChatBuffStream.h"

RTChatBuffStream::RTChatBuffStream(int maxbuffsize) :
_curSize(0)
{
    _buffVec.resize(maxbuffsize);
}

RTChatBuffStream::~RTChatBuffStream()
{
    _buffVec.clear();
}

bool RTChatBuffStream::Write(const void *buf, int len)
{
    if (len <= _buffVec.size() - _curSize) {
        bcopy(buf, &_buffVec[_curSize], len);
        _curSize += len;
        return true;
    }
    else {
        return false;
    }
}

int RTChatBuffStream::Read(void *buf, int len)
{
    return 0;
}

int RTChatBuffStream::Rewind()
{
    _curSize = 0;
    return 0;
}

int RTChatBuffStream::get_size()
{
    return _curSize;
}







