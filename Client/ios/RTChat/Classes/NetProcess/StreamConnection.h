//
//  StreamConnection.h
//  RTChat
//
//  Created by raymon_wang on 14-7-11.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RTChat__StreamConnection__
#define __RTChat__StreamConnection__

#include <iostream>
#include "rtpsession.h"

using namespace jrtplib;

class ClientTask;

class StreamConnection {
public:
    StreamConnection();
    virtual ~StreamConnection();
    
    virtual bool initConnection(const char* destipstr, uint16_t destport);
    
    virtual bool reConnect();
    
    void sendLoginMsg();
    
    bool sendData(const char* ptr, int size);
    
    RTPSession& get_rtpSession() {
        return _session;
    };
    
    void OnReceiveMsg();
    
private:
    RTPSession              _session;
    ClientTask*             _clientTask;
};

StreamConnection* getStreamConnection();

#endif /* defined(__RTChat__StreamConnection__) */
