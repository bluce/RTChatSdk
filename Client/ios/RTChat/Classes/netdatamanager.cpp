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
}

NetDataManager::~NetDataManager()
{
    
}

void NetDataManager::init(const std::string &controlserver)
{
    if (_socket) {
        _socket->init(*this, controlserver);
    }
}

void NetDataManager::sendSampleMsg()
{
    Cmd::cmdRequestLogin msg;
    msg.set_uniqueid("111");
    
    printf("%lu\n", sizeof(WORD));
    printf("%lu\n", sizeof(DWORD));
    printf("%lu\n", sizeof(stBaseCmd));
    
    char buff[1024] = {0};
    msg.SerializeToArray(buff, msg.ByteSize());
    
    BUFFER_CMD(stBaseCmd, basecmd, 80);
    basecmd->cmdid = Cmd::enRequestLogin;
    basecmd->cmdlen = msg.ByteSize();
    memcpy(basecmd->data, buff, msg.ByteSize());
    _socket->send((const unsigned char*)basecmd, basecmd->getSize());
    
    _media = new MediaSample();
}

void NetDataManager::sendClientMsg(const unsigned char *msg, unsigned int len)
{
    if (_socket) {
        _socket->send(msg, len);
    }
}

//发送心跳消息
void NetDataManager::sendHelloMsg()
{
    stHelloCmd cmd;
    _socket->send((const unsigned char*)&cmd, sizeof(stHelloCmd));
}

void NetDataManager::onOpen(WebSocket* ws)
{
    sendSampleMsg();
}

void NetDataManager::onMessage(WebSocket* ws, const WebSocket::Data& data)
{
    if (data.len == 2) {    //心跳消息
        sendHelloMsg();
        return;
    }
    
    BUFFER_CMD(stBaseCmd, cmd, 1024)
    
    
    cmd = (stBaseCmd*)data.bytes;
    switch (cmd->cmdid) {
        case Cmd::enNotifyLoginResult:
        {
            Cmd::cmdNotifyLoginResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            printf("%llu\n", protomsg.tempid());
            break;
        }
        default:
            break;
    }
}

void NetDataManager::onClose(WebSocket* ws)
{
    int i = 9;
}

void NetDataManager::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
    int i = 9;
}






