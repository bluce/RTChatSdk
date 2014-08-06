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
_isMute(false),
_sdkOpState(SdkSocketUnConnected),
_sdkTempID(0)
{
    _netDataManager = new NetDataManager;
    _mediaSample = new MediaSample;
    
    initMutexLock();
}

RTChatSDKMain::~RTChatSDKMain()
{
    SAFE_DELETE(_netDataManager);
    SAFE_DELETE(_mediaSample);
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
//        _netDataManager->init("ws://180.168.126.249:16001");
        _netDataManager->init("ws://180.168.126.253:16001");
    }
    
    if (_mediaSample) {
        _mediaSample->init();
    }
}

//当应用最小化时需要调用这个，清理数据
void RTChatSDKMain::freezeSDK()
{
    if (_netDataManager) {
        _netDataManager->closeWebSocket();
    }
    
    if (_mediaSample) {
        _mediaSample->closeVoiceEngine();
    }
}

void RTChatSDKMain::registerMsgCallback(const pMsgCallFunc& func)
{
    _func = func;
}

//获取SDK当前操作状态
SdkOpState RTChatSDKMain::getSdkState()
{
    return _sdkOpState;
}

void RTChatSDKMain::requestLogin()
{
    if (_sdkOpState < SdkSocketConnected) {
        return;
    }
    
    _sdkOpState = SdkUserLogining;
    
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

void RTChatSDKMain::createRoom(Cmd::enRoomType roomType)
{
    if (_sdkOpState != SdkUserLogined) {
        return;
    }
    
    Cmd::cmdRequestCreateRoom msg;
    msg.set_type(roomType);
    
    _sdkOpState = SdkUserCreatingRoom;
    
    SendProtoMsg(msg, Cmd::enRequestCreateRoom);
}

void RTChatSDKMain::joinRoom(uint64_t roomid)
{
    if (_sdkOpState != SdkUserLogined) {
        return;
    }
    
    Cmd::cmdRequestEnterRoom msg;
    msg.set_roomid(roomid);
    
    _sdkOpState = SdkUserjoiningRoom;
    
    SendProtoMsg(msg, Cmd::enRequestEnterRoom);
}

//离开房间
void RTChatSDKMain::leaveRoom()
{
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestLeaveRoom;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char*)&cmd, cmd.getSize());
    }
    
    _sdkOpState = SdkUserLogined;
}

//加入麦序
void RTChatSDKMain::requestInsertMicQueue()
{
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enJoinMicQueue;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char*)&cmd, cmd.getSize());
    }
    
    _sdkOpState = SdkUserWaitingToken;
}

//离开麦序
void RTChatSDKMain::leaveMicQueue()
{
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enLeaveMicQueue;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char*)&cmd, cmd.getSize());
    }
    
    _sdkOpState = SdkUserJoinedRoom;
}

//随机进入一个房间
void RTChatSDKMain::randomJoinRoom()
{
    if (_roomInfoMap.size() != 0) {
        joinRoom(_roomInfoMap.rbegin()->first);
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
            
            _sdkOpState = SdkUserLogined;
            _sdkTempID = protomsg.tempid();
            
            break;
        }
        case Cmd::enNotifyCreateResult:
        {
            Cmd::cmdNotifyCreateResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.isok()) {
                connectVoiceRoom(protomsg.ip(), protomsg.port());
                
                _sdkOpState = SdkUserCreatedRoom;
                
                //这里需要加锁吗，待处理
                _currentRoomID = protomsg.roomid();
                
                _func(_currentRoomID);
            }
            else {
                _sdkOpState = SdkSocketConnected;
            }
            break;
        }
        case Cmd::enNotifyEnterResult:
        {
            Cmd::cmdNotifyEnterResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.isok()) {
                connectVoiceRoom(protomsg.ip(), protomsg.port());
                
                _sdkOpState = SdkUserJoinedRoom;
            }
            else {
                _sdkOpState = SdkSocketConnected;
            }
            
            break;
        }
        case Cmd::enNotifyRoomList:
        {
            Cmd::cmdNotifyRoomList protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            refreshRoomInfoMap(protomsg);
            
            randomJoinRoom();
            
            break;
        }
        case Cmd::enAddVoiceUser:
        {
            Cmd::cmdAddVoiceUser protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            for (int i = 0; i < protomsg.info_size(); i++) {
                if (_mediaSample) {
                    _mediaSample->onCreateChannel(protomsg.info(i).id(), MediaSample::data_in);
                }
            }
            
            break;
        }
        case Cmd::enNotifyMicQueue:
        {
            break;
        }
        case Cmd::enTakeMic:
        {
            break;
            Cmd::cmdTakeMic protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.tempid() == _sdkTempID) {
                //轮到本人说话
                if (_mediaSample) {
                    _mediaSample->setMuteMic(false);
                    _sdkOpState = SdkUserSpeaking;
                }
            }
            else {
                //轮到他人说话
                if (_mediaSample) {
                    _mediaSample->setMuteMic(true);
                }
            }
            
            break;
        }
        default:
            break;
    }
}

//语音引擎连接语音房间
void RTChatSDKMain::connectVoiceRoom(const std::string& ip, unsigned int port)
{
    if (_mediaSample) {
        _mediaSample->connectRoom(ip, port, _sdkTempID);
//        _mediaSample->connectRoom("192.168.82.191", 16001);
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

//初始化互斥锁
void RTChatSDKMain::initMutexLock()
{
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&_mutexLock, &attr);
}




