//
//  netdatamanager.cpp
//  RTChat
//
//  Created by raymon_wang on 14-7-28.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "netdatamanager.h"
#include "NetProcess/command.h"
#include "proto/game.pb.h"
#include "RTChatSDKMain.h"

NetDataManager::NetDataManager() :
_socket(NULL)
{
    
}

NetDataManager::~NetDataManager()
{
    
}

void NetDataManager::init(const std::string &controlserver)
{
    _controlServerStr = controlserver;
    
    connectControlServer();
}

void NetDataManager::sendClientMsg(const unsigned char *msg, unsigned int len)
{
    if (!_socket) {
        connectControlServer();
    }
    else if (getWebSocketState() == WebSocket::kStateClosed) {
        SAFE_DELETE(_socket);
        connectControlServer();
    }
    
    if (_socket) {
        _socket->send(msg, len);
    }
}

WebSocket::State NetDataManager::getWebSocketState()
{
    if (_socket) {
        return _socket->getReadyState();
    }
    
    return WebSocket::kStateClosed;
}

//连接控制服务器
void NetDataManager::connectControlServer()
{
    if (!_socket) {
        _socket = new WebSocket();
    }
    _socket->init(*this, _controlServerStr);
}

//关闭websocket
void NetDataManager::closeWebSocket()
{
    SAFE_DELETE(_socket);
}

//发送心跳消息
void NetDataManager::sendHelloMsg()
{
    stHelloCmd cmd;
    _socket->send((const unsigned char*)&cmd, sizeof(stHelloCmd));
}

void NetDataManager::onOpen(WebSocket* ws)
{
    RTChatSDKMain::sharedInstance().set_SdkOpState(SdkSocketConnected);
    RTChatSDKMain::sharedInstance().requestLogin();
}

void NetDataManager::onMessage(WebSocket* ws, const WebSocket::Data& data)
{
    if (data.len == 2) {    //心跳消息
        sendHelloMsg();
        return;
    }
    
    RTChatSDKMain::sharedInstance().onRecvMsg(data.bytes, data.len);
}

void NetDataManager::onClose(WebSocket* ws)
{
    
}

void NetDataManager::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
    
}






