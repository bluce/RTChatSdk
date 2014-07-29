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

NetDataManager::NetDataManager() :
_socket(nullptr)
{
    _socket = new WebSocket();
    if (_socket) {
        _socket->init(*this, "ws://180.168.126.249:16008");
    }
    
    sendSampleMsg();
}

NetDataManager::~NetDataManager()
{
    
}

void NetDataManager::sendSampleMsg()
{
    Cmd::cmdLogin msg;
    msg.set_id("111");
    
    char buff[1024] = {0};
    msg.SerializeToArray(buff, msg.ByteSize());
    
    BUFFER_CMD(stBaseCmd, basecmd, 80);
    basecmd->cmdid = Cmd::enLogin;
    basecmd->cmdlen = msg.ByteSize();
    memcpy(basecmd->data, buff, msg.ByteSize());
    _socket->send((const unsigned char*)basecmd, basecmd->getSize());
}

void NetDataManager::onOpen(WebSocket* ws)
{
    int i = 9;
    
    sendSampleMsg();
}

void NetDataManager::onMessage(WebSocket* ws, const WebSocket::Data& data)
{
    int i = 9;
}

void NetDataManager::onClose(WebSocket* ws)
{
    int i = 9;
}

void NetDataManager::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
    int i = 9;
}






