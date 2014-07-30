//
//  RTChatSDKMain.cpp
//  RTChat
//
//  Created by raymon_wang on 14-7-29.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "RTChatSDKMain.h"
#include "netdatamanager.h"
#include "NetProcess/command.h"
#include "MediaSample.h"

static  RTChatSDKMain* s_RTChatSDKMain = NULL;

#define MaxBufferSize   1024

#define SendProtoMsg(MSG, TypeID)   \
    char buff[MaxBufferSize] = {0}; \
    MSG.SerializeToArray(buff, msg.ByteSize()); \
    BUFFER_CMD(stBaseCmd, basecmd, MaxBufferSize);  \
    basecmd->cmdid = TypeID;    \
    basecmd->cmdlen = MSG.ByteSize();   \
    memcpy(basecmd->data, buff, msg.ByteSize());    \
    if (_netDataManager) {  \
        _netDataManager->sendClientMsg((const unsigned char*)basecmd, basecmd->getSize());  \
    }   \


RTChatSDKMain::RTChatSDKMain() :
_netDataManager(NULL),
_mediaSample(NULL),
_currentRoomID(-1),
_isMute(false)
{
    _netDataManager = new NetDataManager;
    _mediaSample = new MediaSample;
}

RTChatSDKMain::~RTChatSDKMain()
{
    
}

RTChatSDKMain& RTChatSDKMain::sharedInstance()
{
    if (!s_RTChatSDKMain) {
        s_RTChatSDKMain = new RTChatSDKMain();
    }
    
    return *s_RTChatSDKMain;
}

void RTChatSDKMain::initSDK(const std::string &uniqueid)
{
    _uniqueid = uniqueid;
    
    if (_netDataManager) {
        _netDataManager->init("ws://180.168.126.249:16001");
//        _netDataManager->init("ws://180.168.126.249:16008");
    }
    
    if (_mediaSample) {
        _mediaSample->init();
    }
}

void RTChatSDKMain::registerMsgCallback(const pMsgCallFunc& func)
{
    _func = func;
}

void RTChatSDKMain::requestLogin()
{
    Cmd::cmdRequestLogin msg;
    msg.set_uniqueid(_uniqueid);
    
    SendProtoMsg(msg, Cmd::enRequestLogin);
}

//申请房间列表
void RTChatSDKMain::requestRoomList()
{
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestRoomList;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char *)&cmd, cmd.getSize());
    }
}

void RTChatSDKMain::createRoom()
{
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestCreateRoom;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char*)&cmd, cmd.getSize());
    }
}

void RTChatSDKMain::joinRoom(uint64_t roomid)
{
    Cmd::cmdRequestEnterRoom msg;
    msg.set_roomid(roomid);
    
    SendProtoMsg(msg, Cmd::enRequestEnterRoom);
}

//随机进入一个房间
void RTChatSDKMain::randomJoinRoom()
{
    if (_roomInfoMap.size() != 0) {
        joinRoom(_roomInfoMap.begin()->first);
    }
}

void RTChatSDKMain::startTalk()
{
    
}

void RTChatSDKMain::stopTalk()
{
    
}

void RTChatSDKMain::onRecvMsg(char *data, int len)
{
    stBaseCmd* cmd = (stBaseCmd*)data;
    
    switch (cmd->cmdid) {
        case Cmd::enNotifyLoginResult:
        {
            Cmd::cmdNotifyLoginResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            break;
        }
        case Cmd::enNotifyCreateResult:
        {
            Cmd::cmdNotifyCreateResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.isok()) {
                connectVoiceRoom(protomsg.ip(), protomsg.port());
                
                //这里需要加锁吗，待处理
                _currentRoomID = protomsg.roomid();
                
                _func(_currentRoomID);
            }
            break;
        }
        case Cmd::enNotifyEnterResult:
        {
            Cmd::cmdNotifyEnterResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.isok()) {
                connectVoiceRoom(protomsg.ip(), protomsg.port());
            }
            
            break;
        }
        case Cmd::enNotifyRoomList:
        {
            Cmd::cmdNotifyRoomList protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            refreshRoomInfoMap(protomsg);
            
            randomJoinRoom();
        }
        default:
            break;
    }
}

//语音引擎连接语音房间
void RTChatSDKMain::connectVoiceRoom(const std::string& ip, unsigned int port)
{
    if (_mediaSample) {
        _mediaSample->connectRoom(ip, port);
    }
}

//获取当前的输入mic静音状态
bool RTChatSDKMain::getMuteSelf()
{
    return _isMute;
}

//设置本人Mac静音
void RTChatSDKMain::setMuteSelf(bool isMute)
{
    if (_mediaSample) {
        _mediaSample->setMuteMic(isMute);
        _isMute = isMute;
    }
}

//刷新房间列表信息
void RTChatSDKMain::refreshRoomInfoMap(const Cmd::cmdNotifyRoomList& protomsg)
{
    _roomInfoMap.clear();
    for (int i = 0; i < protomsg.info_size(); i++) {
        _roomInfoMap[protomsg.info(i).roomid()] = protomsg.info(i);
    }
}




