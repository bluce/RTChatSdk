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
#include "MediaSample.h"

class NetDataManager : public WebSocket::Delegate {
public:
    NetDataManager();
    virtual ~NetDataManager();
    
    void init(const std::string& controlserver);
    
    void sendClientMsg(const unsigned char* msg, unsigned int len);
    
    virtual void onOpen(WebSocket* ws);
    virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);
    virtual void onClose(WebSocket* ws);
    virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);
    
protected:
    //发送心跳消息
    void sendHelloMsg();
    
private:
    WebSocket*      _socket;
    MediaSample*    _media;
};

#endif /* defined(__RTChat__netdatamanager__) */
