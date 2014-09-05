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
#include "public.h"
#include <unistd.h>

static int maxretrycount = 10;

NetDataManager::NetDataManager() :
_haveInited(false),
_socket(NULL),
_retrycount(0),
_needCloseConnection(false)
{
    pthread_mutex_init(&_mutexlock, 0);
}

NetDataManager::~NetDataManager()
{
    SAFE_DELETE(_socket);
}

void NetDataManager::init(const std::string &controlserver)
{
    _controlServerStr = controlserver;
    
    _haveInited = true;
}

void NetDataManager::activity()
{

}

void NetDataManager::deactive()
{
    
}

bool NetDataManager::Process()
{
    int needcloseconnection;
    
    pthread_mutex_lock(&_mutexlock);
    needcloseconnection = _needCloseConnection;
    pthread_mutex_unlock(&_mutexlock);
    
    if (needcloseconnection && _socket) {
        _socket->close();
        pthread_mutex_lock(&_mutexlock);
        _needCloseConnection = false;
        pthread_mutex_unlock(&_mutexlock);
        return true;
    }
    
    if (!_socket && _haveInited && !haveReachMaxRetryCount()) {
        connectControlServer();
    }
    
    if (_socket && _socket->onSubThreadLoop() == 0) {
        return true;
    }
    else {
        sleep(1);
        return true;
    }
}

void NetDataManager::sendClientMsg(const unsigned char *msg, unsigned int len)
{
//    if (!_socket) {
//        connectControlServer();
//    }
//    else if (getWebSocketState() == WebSocket::State::CLOSED) {
//        SAFE_DELETE(_socket);
//        connectControlServer();
//    }
    if (_socket) {
        _socket->send(msg, len);
    }
}

WebSocket::State NetDataManager::getWebSocketState()
{
    if (_socket) {
        return _socket->getReadyState();
    }
    
    return WebSocket::State::CLOSED;
}

/// 重置重连次数
void NetDataManager::resetRetryCount()
{
    pthread_mutex_lock(&_mutexlock);
    _retrycount = 0;
    pthread_mutex_unlock(&_mutexlock);
}

/// 获得重连次数
int NetDataManager::getRetryCount()
{
    int count;
    pthread_mutex_lock(&_mutexlock);
    count = _retrycount;
    pthread_mutex_unlock(&_mutexlock);
    return count;
}

//连接控制服务器
void NetDataManager::connectControlServer()
{
    Public::sdklog("  In connectControlServer");
    
    if (!_socket) {
        _socket = new WebSocket();
    }
    _socket->init(*this, _controlServerStr);
    
    _socket->onSubThreadStarted();
    
    pthread_mutex_unlock(&_mutexlock);
    _retrycount++;
    pthread_mutex_unlock(&_mutexlock);
    
    Public::sdklog("  第%d次连接", _retrycount);
}

///关闭websocket
void NetDataManager::destroyWebSocket()
{
    Public::sdklog("  In destroyWebSocket");
    SAFE_DELETE(_socket);
}

//发送心跳消息
void NetDataManager::sendHelloMsg()
{
    Public::sdklog("  发送心跳消息%d", time(NULL));
    stHelloCmd cmd;
    _socket->send((const unsigned char*)&cmd, sizeof(stHelloCmd));
}

//是否达到最大重连次数
bool NetDataManager::haveReachMaxRetryCount()
{
    return getRetryCount() >= maxretrycount;
}

void NetDataManager::onOpen(WebSocket* ws)
{
    Public::sdklog("  连接已打开");
    
    resetRetryCount();
    
    if (RTChatSDKMain::sharedInstance().getSdkState() < SdkGateWaySocketUnConnected) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkControlConnected);
    }
    else if (RTChatSDKMain::sharedInstance().getSdkState() >= SdkGateWaySocketUnConnected) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkGateWaySocketConnected);
    }
    
    TimeCounter::instance().registerTimeOutCallBack("nettimeout", 60, std::bind(&NetDataManager::connnectionTimeOut, this, std::placeholders::_1));
}

//超时回调
void NetDataManager::connnectionTimeOut(int period)
{
//    Public::sdklog("  心跳超时回调%d", time(NULL));
    
    pthread_mutex_lock(&_mutexlock);
    _needCloseConnection = true;
    pthread_mutex_unlock(&_mutexlock);
}

void NetDataManager::onMessage(WebSocket* ws, const WebSocket::Data& data)
{
    if (data.len == 2) {    //心跳消息
        sendHelloMsg();
        
        TimeCounter::instance().resetCallBackInfoTime("nettimeout");
        
        return;
    }
    
    RTChatSDKMain::sharedInstance().onRecvMsg(data.bytes, data.len);
}

void NetDataManager::onClose(WebSocket* ws)
{
    Public::sdklog("  连接被关闭");
    
    TimeCounter::instance().destroyCallBackInfo("nettimeout");
    
    destroyWebSocket();
    
    if (RTChatSDKMain::sharedInstance().getSdkState() < SdkGateWaySocketUnConnected) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkControlUnConnected);
    }
    else if (RTChatSDKMain::sharedInstance().getSdkState() > SdkGateWaySocketUnConnected) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkGateWaySocketUnConnected);
    }
}

void NetDataManager::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
    Public::sdklog("  连接发生错误");
    destroyWebSocket();
    if (RTChatSDKMain::sharedInstance().getSdkState() < SdkGateWaySocketUnConnected) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkControlUnConnected);
    }
    else if (RTChatSDKMain::sharedInstance().getSdkState() > SdkGateWaySocketUnConnected) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkGateWaySocketUnConnected);
    }
}






