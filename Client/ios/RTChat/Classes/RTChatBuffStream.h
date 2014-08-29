//
//  RTChatBuffStream.h
//  RTChat
//
//  Created by raymon_wang on 14-8-26.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RTChat__RTChatBuffStream__
#define __RTChat__RTChatBuffStream__

#include <iostream>
#include <common_types.h>

class RTChatBuffStream : public webrtc::OutStream, public webrtc::InStream {
public:
    RTChatBuffStream(int maxbuffsize);
    virtual ~RTChatBuffStream();
    
    virtual bool Write(const void *buf,int len);
    
    virtual int Read(void *buf,int len);
    
    virtual int Rewind();
    
    int get_size();
    
private:
    std::vector<unsigned char>      _buffVec;
    int                             _curSize;
};

#endif /* defined(__RTChat__RTChatBuffStream__) */
