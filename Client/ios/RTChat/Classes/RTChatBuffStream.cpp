//
//  RTChatBuffStream.cpp
//  RTChat
//
//  Created by raymon_wang on 14-8-26.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "RTChatBuffStream.h"

RTChatBuffStream::RTChatBuffStream(int maxbuffsize) :
_curWriteSize(0),
_curReadSize(0)
{
    _buffVec.resize(maxbuffsize);
}

RTChatBuffStream::~RTChatBuffStream()
{
    _buffVec.clear();
}

bool RTChatBuffStream::Write(const void *buf, int len)
{
    if (len <= _buffVec.size() - _curWriteSize) {
        bcopy(buf, &_buffVec[_curWriteSize], len);
        _curWriteSize += len;
        return true;
    }
    else {
        return false;
    }
}

int RTChatBuffStream::Read(void *buf, int len)
{
    if (len < (_curWriteSize - _curReadSize)) {
        bcopy(&_buffVec[_curReadSize], buf, len);
        _curReadSize += len;
        return len;
    }
    else {
        bcopy(&_buffVec[_curReadSize], buf, (_curWriteSize - _curReadSize));
        return _curWriteSize - _curReadSize;
    }
}

//int RTChatBuffStream::Rewind()
//{
//    _curReadSize = 0;
//    _curWriteSize = 0;
//    return 0;
//}

const RTChatBuffStream::BuffVec& RTChatBuffStream::getBuffVec()
{
    return _buffVec;
}

///获得实际数据长度
int RTChatBuffStream::get_datasize()
{
    return _curWriteSize;
}







