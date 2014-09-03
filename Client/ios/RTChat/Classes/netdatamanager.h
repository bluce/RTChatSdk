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
#include "TimeCounter.h"

class NetDataManager : public WebSocket::Delegate {
public:
    NetDataManager();
    virtual ~NetDataManager();
    
    void init(const std::string& controlserver);
    
    void activity();
    
    void deactive();
    
    void sendClientMsg(const unsigned char* msg, unsigned int len);
    
    //销毁websocket
    void destroyWebSocket();
    
    //获取底层websocket状态
    WebSocket::State getWebSocketState();
    
    virtual void onOpen(WebSocket* ws);
    virtual void onMessage(WebSocket* ws, const WebSocket::Data& data);
    virtual void onClose(WebSocket* ws);
    virtual void onError(WebSocket* ws, const WebSocket::ErrorCode& error);
    
protected:
    static bool Run(ThreadObj obj);
    bool Process();
    
    //超时回调
    void connnectionTimeOut(int period);
    
private:
    //连接控制服务器
    void connectControlServer();
    
    //发送心跳消息
    void sendHelloMsg();
    
    //是否达到最大重连次数
    bool haveReachMaxRetryCount();
    
private:
    bool                _haveInited;
    bool                _needCloseConnection;
    ThreadWrapper*      _workThread;
    pthread_mutex_t     _mutexlock;
    WebSocket*          _socket;
    std::string         _controlServerStr;
    int                 _retrycount;    //重连次数
    int                 _callBackID; //回调函数注册ID
};

#endif /* defined(__RTChat__netdatamanager__) */
