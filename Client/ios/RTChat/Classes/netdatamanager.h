//
//  netdatamanager.h
//  RTChat
//
//  Created by raymon_wang on 14-7-28.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RTChat__netdatamanager__
#define __RTChat__netdatamanager__

#include <iostream>
#include "WebSocket.h"

class NetDataManager : public WebSocket::Delegate {
public:
    NetDataManager();
    virtual ~NetDataManager();
    
    void sendSampleMsg();
    
    virtual void onOpen(WebSocket* ws);
    virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);
    virtual void onClose(WebSocket* ws);
    virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);
    
private:
    WebSocket*      _socket;
};

#endif /* defined(__RTChat__netdatamanager__) */
