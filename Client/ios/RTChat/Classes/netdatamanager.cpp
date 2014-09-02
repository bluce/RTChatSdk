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
_workThread(NULL),
_retrycount(0),
_needCloseConnection(false)
{
    pthread_mutex_init(&_mutexlock, 0);
    _workThread = ThreadWrapper::CreateThread(NetDataManager::Run, this, kNormalPriority, "WorkThread");
}

NetDataManager::~NetDataManager()
{
    if (_workThread) {
        _workThread->Stop();
    }
    SAFE_DELETE(_workThread);
    SAFE_DELETE(_socket);
}

void NetDataManager::init(const std::string &controlserver)
{
    _controlServerStr = controlserver;
    
    _haveInited = true;
    
    activity();
}

void NetDataManager::activity()
{
    unsigned int id = 0;
    
    if (_workThread) {
        _workThread->Start(id);
    }
}

void NetDataManager::deactive()
{
    if (_workThread) {
        _workThread->Stop();
    }
}

bool NetDataManager::Run(ThreadObj obj)
{
    NetDataManager* mgr = static_cast<NetDataManager*>(obj);
    
    return mgr->Process();
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
    if (_haveInited && !_socket) {
        _retrycount = 0;
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
    
    return WebSocket::State::CLOSED;
}

//连接控制服务器
void NetDataManager::connectControlServer()
{
    Public::sdklog("In connectControlServer");
    
    if (!_socket) {
        _socket = new WebSocket();
    }
    _socket->init(*this, _controlServerStr);
    
    _socket->onSubThreadStarted();
    
    _retrycount++;
    Public::sdklog("第%d次连接", _retrycount);
}

//关闭websocket
void NetDataManager::destroyWebSocket()
{
    Public::sdklog("In destroyWebSocket");
    SAFE_DELETE(_socket);
}

//发送心跳消息
void NetDataManager::sendHelloMsg()
{
//    Public::sdklog("发送心跳消息");
    stHelloCmd cmd;
    _socket->send((const unsigned char*)&cmd, sizeof(stHelloCmd));
}

//是否达到最大重连次数
bool NetDataManager::haveReachMaxRetryCount()
{
    return _retrycount >= maxretrycount;
}

void NetDataManager::onOpen(WebSocket* ws)
{
    Public::sdklog("连接已打开");
    
    _retrycount = 0;
    
    if (RTChatSDKMain::sharedInstance().getSdkState() == SdkControlConnecting) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkControlConnected);
        RTChatSDKMain::sharedInstance().requestLogicInfo();
    }
    else if (RTChatSDKMain::sharedInstance().getSdkState() == SdkGateWaySocketConnecting) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkGateWaySocketConnected);
        RTChatSDKMain::sharedInstance().requestLogin();
    }
    
    _counter.startCounter();
    _counter.resetTicker();
    _counter.registerTimeOutCallBack(20, std::bind(&NetDataManager::connnectionTimeOut, this));
}

void NetDataManager::connnectionTimeOut()
{
    Public::sdklog("心跳超时回调");
    
    pthread_mutex_lock(&_mutexlock);
    _needCloseConnection = true;
    pthread_mutex_unlock(&_mutexlock);
}

void NetDataManager::onMessage(WebSocket* ws, const WebSocket::Data& data)
{
    if (data.len == 2) {    //心跳消息
        sendHelloMsg();
        
        //重置计时器
        _counter.resetTicker();
        return;
    }
    
    RTChatSDKMain::sharedInstance().onRecvMsg(data.bytes, data.len);
}

void NetDataManager::onClose(WebSocket* ws)
{
    Public::sdklog("连接被关闭");
    destroyWebSocket();
    if (RTChatSDKMain::sharedInstance().getSdkState() == SdkControlConnected) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkControlUnConnected);
    }
    else if (RTChatSDKMain::sharedInstance().getSdkState() >= SdkGateWaySocketConnected) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkControlConnected);
    }
}

void NetDataManager::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
    Public::sdklog("连接发生错误");
    destroyWebSocket();
    if (RTChatSDKMain::sharedInstance().getSdkState() == SdkControlConnected) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkControlUnConnected);
    }
    else if (RTChatSDKMain::sharedInstance().getSdkState() >= SdkGateWaySocketConnected) {
        RTChatSDKMain::sharedInstance().set_SdkOpState(SdkControlConnected);
    }
}






