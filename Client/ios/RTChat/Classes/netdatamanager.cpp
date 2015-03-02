//
//  netdatamanager.cpp
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-7-28.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#include "netdatamanager.h"
#include "NetProcess/command.h"
#include "proto/game.pb.h"
#include "RTChatSDKMain_Ios.h"
#include "public.h"
#include <unistd.h>
#include "BridgeTools.h"

static int maxretrycount = 10;

namespace rtchatsdk {

NetDataManager::NetDataManager() :
_haveInited(false),
_socket(NULL),
_retrycount(0),
_needCloseConnection(false),
_cryptobuffer(NULL),
_decryptobuffer(NULL)
{
    pthread_mutex_init(&_mutexlock, 0);
    
    _cryptobuffer = new unsigned char[1024*64];
    _decryptobuffer = new unsigned char[1024*64];
    
    std::string path = BridgeTools::getWritablePath();
    
    Public::sdklog("%s", path.c_str());
}

NetDataManager::~NetDataManager()
{
    SAFE_DELETE(_socket);
    SAFE_DELETEARRAY(_cryptobuffer);
    SAFE_DELETEARRAY(_decryptobuffer);
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
        _needCloseConnection = false;
        return true;
    }
    
    if (!_socket && _haveInited && !haveReachMaxRetryCount()) {
        connectControlServer();
    }
    
    //调用websocket主循环
    if (_socket) {
        _socket->onSubThreadLoop();
    }
    else {
        sleep(1);
        return true;
    }
    
    return true;
}

void NetDataManager::sendClientMsg(const unsigned char *msg, unsigned int len)
{
    int outsize = 0;
    bool result = BridgeTools::des(msg, len, _cryptobuffer, outsize, true);
    if (result && _socket) {
        pthread_mutex_lock(&_mutexlock);
        _socket->send(_cryptobuffer, outsize);
        pthread_mutex_unlock(&_mutexlock);
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
        pthread_mutex_lock(&_mutexlock);
        _socket = new WebSocket();
        pthread_mutex_unlock(&_mutexlock);
    }
    
    pthread_mutex_unlock(&_mutexlock);
    _socket->init(*this, _controlServerStr);
    _socket->onSubThreadStarted();
    _retrycount++;
    pthread_mutex_unlock(&_mutexlock);
    
    Public::sdklog("  第%d次连接", _retrycount);
}

///关闭websocket
void NetDataManager::destroyWebSocket()
{
    Public::sdklog("  In destroyWebSocket");
    pthread_mutex_lock(&_mutexlock);
    SAFE_DELETE(_socket);
    pthread_mutex_unlock(&_mutexlock);
}

//发送心跳消息
void NetDataManager::sendHelloMsg()
{
    Public::sdklog("  发送心跳消息%d", time(NULL));
    stHelloCmd cmd;
    
    pthread_mutex_lock(&_mutexlock);
    _socket->send((const unsigned char*)&cmd, sizeof(stHelloCmd));
    pthread_mutex_unlock(&_mutexlock);
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
    int outsize = 0;
    
    if (data.len == 2) {    //心跳消息
        sendHelloMsg();
        TimeCounter::instance().resetCallBackInfoTime("nettimeout");
        return;
    }
    else {
        //解密数据
        bool result = BridgeTools::des((const unsigned char*)data.bytes, data.len, _decryptobuffer, outsize, false);
        if (result) {
            //    RTChatSDKMain::sharedInstance().onRecvMsg(data.bytes, data.len);
            RTChatSDKMain::sharedInstance().onRecvMsg((char*)_decryptobuffer, outsize);
        }
        else {
            Public::sdklog("数据解密失败");
        }
    }
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
    
}






