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
#include "defines.h"
#include "public.h"

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
_currentRoomID(0),
_isMute(false),
_sdkOpState(SdkSocketUnConnected),
_sdkTempID(0),
_appid(""),
_appkey(""),
_token(""),
_uniqueid(""),
_logicIP(""),
_logicPort(0),
_func(NULL)
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

void RTChatSDKMain::initSDK(const std::string &appid, const std::string &key, const char* uniqueid)
{
    _appid = appid;
    _appkey = key;
    
    if (uniqueid != NULL) {
        _uniqueid = uniqueid;
    }
    
    activateSDK();
}

//激活SDK
void RTChatSDKMain::activateSDK()
{
    if (_netDataManager) {
        _netDataManager->init("ws://180.168.126.249:16001");
//        _netDataManager->init("ws://122.11.47.93:16001");
    }
    
    if (_mediaSample) {
        _mediaSample->init();
    }
}

//当应用最小化时需要调用这个，清理数据
void RTChatSDKMain::deActivateSDK()
{
    if (_netDataManager) {
        _netDataManager->destroyWebSocket();
    }
    
    if (_mediaSample) {
        _mediaSample->closeVoiceEngine();
    }
    
//    _uniqueid = "";
//    _sdkTempID = 0;
    _currentRoomID = 0;
    _isMute = false;
    _sdkOpState = SdkSocketUnConnected;
//    _func = NULL;
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

SdkErrorCode RTChatSDKMain::requestLogin(const char* uniqueid)
{
    if (uniqueid != NULL) {
        _uniqueid = uniqueid;
    }
    if (_uniqueid == "") {
        return OPERATION_FAILED;
    }
    
    if (_sdkOpState < SdkSocketConnected) {
        return OPERATION_FAILED;
    }
    
    _sdkOpState = SdkUserLogining;
    
    Cmd::cmdRequestLogin msg;
    msg.set_uniqueid(_uniqueid);
    msg.set_token("yuew89341huidy89iuh1");
    msg.set_tempid(_sdkTempID);
    
    SendProtoMsg(msg, Cmd::enRequestLogin);
    
    Public::sdklog("发送登录消息完成");
    
    return OPERATION_OK;
}

//请求逻辑服务器地址
void RTChatSDKMain::requestLogicServerInfo(const std::string& appid, const std::string& key)
{
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestLogicInfo;
    
    
}

//申请房间列表
SdkErrorCode RTChatSDKMain::requestRoomList()
{
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestRoomList;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char *)&cmd, cmd.getSize());
    }
    
    return OPERATION_OK;
}

SdkErrorCode RTChatSDKMain::createRoom(enRoomType roomType, enRoomReason reason)
{
    if (_sdkOpState != SdkUserLogined) {
        return OPERATION_FAILED;
    }
    
    Cmd::cmdRequestCreateRoom msg;
    msg.set_type((Cmd::enRoomType)roomType);
    msg.set_reason((Cmd::enRoomReason)reason);
    
    _sdkOpState = SdkUserCreatingRoom;
    
    SendProtoMsg(msg, Cmd::enRequestCreateRoom);
    
    return OPERATION_OK;
}

SdkErrorCode RTChatSDKMain::joinRoom(uint64_t roomid)
{
    if (_sdkOpState != SdkUserLogined) {
        return OPERATION_FAILED;
    }
    
    Cmd::cmdRequestEnterRoom msg;
    msg.set_roomid(roomid);
    
    _sdkOpState = SdkUserjoiningRoom;
    
    SendProtoMsg(msg, Cmd::enRequestEnterRoom);
    
    return OPERATION_OK;
}

//离开房间
SdkErrorCode RTChatSDKMain::leaveRoom()
{
    if (_sdkOpState < SdkUserjoiningRoom) {
        return OPERATION_FAILED;
    }
    
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestLeaveRoom;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char*)&cmd, cmd.getSize());
    }
    
    if (_mediaSample) {
        _mediaSample->leaveCurrentRoom();
    }
    
    _sdkOpState = SdkUserLogined;
    
    return OPERATION_OK;
}

//加入麦序
SdkErrorCode RTChatSDKMain::requestInsertMicQueue()
{
    if (_sdkOpState != SdkUserJoinedRoom) {
        return OPERATION_FAILED;
    }
    
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestJoinMicQueue;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char*)&cmd, cmd.getSize());
    }
    
    _sdkOpState = SdkUserWaitingToken;
    
    return OPERATION_OK;
}

//离开麦序
SdkErrorCode RTChatSDKMain::leaveMicQueue()
{
    if (_sdkOpState != SdkUserJoinedRoom) {
        return OPERATION_FAILED;
    }
    
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestLeaveMicQueue;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char*)&cmd, cmd.getSize());
    }
    
    _sdkOpState = SdkUserJoinedRoom;
    
    return OPERATION_OK;
    
//    if (_mediaSample) {
//        _mediaSample->setMuteMic(true);
//    }
}

//是否接收随机聊天，临时增加的接口
void RTChatSDKMain::returnRandChatRes(bool isAccept, uint64_t srctempid)
{
    if (_sdkOpState != SdkUserLogined) {
        return;
    }
    
    Cmd::cmdReturnRandChat msg;
    msg.set_isok(isAccept);
    msg.set_tempid(srctempid);
    
    SendProtoMsg(msg, Cmd::enReturnRandChat);
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
    
    Public::sdklog("cmdid=%u", cmd->cmdid);
    
    switch (cmd->cmdid) {
        case Cmd::enNotifyLoginResult:
        {
            Cmd::cmdNotifyLoginResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.result() == Cmd::cmdNotifyLoginResult::LOGIN_RESULT_OK) {
                _sdkOpState = SdkUserLogined;
                _sdkTempID = protomsg.tempid();
                
                StNotifyLoginResult callbackdata(protomsg.result(), protomsg.tempid());
                _func(enNotifyLoginResult, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyLoginResult));
            }
            else {
                _func(enNotifyLoginResult, LOGIN_RESULT_TOKEN_ERROR, NULL, 0);
            }
            
            
            break;
        }
        case Cmd::enNotifyCreateResult:
        {
            if (_sdkOpState == SdkUserJoinedRoom) {
                leaveRoom();
            }
            
            Cmd::cmdNotifyCreateResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.isok()) {
                connectVoiceRoom(protomsg.ip(), protomsg.port());
                
                _sdkOpState = SdkUserJoinedRoom;
                
                //这里需要加锁吗，待处理
                _currentRoomID = protomsg.roomid();
                
                
                //回调应用数据
                StNotifyCreateResult callbackdata(true, protomsg.roomid(), (enRoomType)protomsg.type());
                _func(enNotifyCreateResult, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyCreateResult));
            }
            else {
                _sdkOpState = SdkSocketConnected;
                _func(enNotifyCreateResult, OPERATION_FAILED, NULL, 0);
            }
            break;
        }
        case Cmd::enNotifyEnterResult:
        {
            Public::sdklog("接收到进入房间指令");
            
            if (_sdkOpState == SdkUserJoinedRoom) {
                leaveRoom();
            }
            
            Cmd::cmdNotifyEnterResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.result() == Cmd::cmdNotifyEnterResult::ENTER_RESULT_OK) {
                if (_sdkOpState != SdkUserJoinedRoom) {
                    connectVoiceRoom(protomsg.ip(), protomsg.port());
                    
                    _sdkOpState = SdkUserJoinedRoom;
                    
                    if (protomsg.type() == Cmd::ROOM_TYPE_QUEUE) {
                        StNotifyEnterResult callbackdata(protomsg.roomid(), ROOM_TYPE_QUEUE);
                        _func(enNotifyEnterResult, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyEnterResult));
                    }
                    else if (protomsg.type() == Cmd::ROOM_TYPE_FREE) {
                        StNotifyEnterResult callbackdata(protomsg.roomid(), ROOM_TYPE_FREE);
                        _func(enNotifyEnterResult, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyEnterResult));
                    }
                }
            }
            else {
                _sdkOpState = SdkUserLogined;
                
                _func(enNotifyEnterResult, ENTER_RESULT_ERROR, NULL, 0);
            }
            
            break;
        }
        case Cmd::enNotifyRoomList:
        {
            Public::sdklog("接收到通知房间列表指令");
            
            Cmd::cmdNotifyRoomList protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            refreshRoomInfoMap(protomsg);
            
            BUFFER_CMD(StNotifyRoomList, roomList, MAX_BUFFER_SIZE);
            roomList->size = protomsg.info_size();
            for (int i = 0; i < protomsg.info_size(); i++) {
                StRoomInfo info;
                info.roomid = protomsg.info(i).roomid();
                info.roomtype = (enRoomType)protomsg.info(i).roomtype();
                info.num = protomsg.info(i).num();
//                bcopy(&info, &roomList->roominfo[i], sizeof(StRoomInfo));
                roomList->roominfo[i] = info;
            }
            _func(enNotifyRoomList, OPERATION_OK, (const unsigned char*)roomList, roomList->getSize());
            
            break;
        }
        case Cmd::enNotifyAddVoiceUser:
        {
            Public::sdklog("接收到增加通道指令");
            
            if (_sdkOpState == SdkUserJoinedRoom) {
                Cmd::cmdNotifyAddVoiceUser protomsg;
                protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
                for (int i = 0; i < protomsg.info_size(); i++) {
                    if (_mediaSample) {
                        _mediaSample->onCreateChannel(protomsg.info(i).id(), MediaSample::data_in);
                    }
                }
                
                BUFFER_CMD(StNotifyAddVoiceUser, userList, MAX_BUFFER_SIZE);
                userList->size = protomsg.info_size();
                for (int i = 0; i < protomsg.info_size(); i++) {
                    StVoiceUserInfo info;
                    info.userid = protomsg.info(i).id();
                    userList->userinfo[i] = info;
                }
                
                _func(enNotifyAddVoiceUser, OPERATION_OK, (const unsigned char*)userList, userList->getSize());
            }

            break;
        }
        case Cmd::enNotifyMicQueue:
        {
            Public::sdklog("接收到通知麦序指令");
            
            if (_sdkOpState == SdkUserJoinedRoom) {
                Cmd::cmdNotifyMicQueue protomsg;
                protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
                
                BUFFER_CMD(StNotifyMicQueue, micList, MAX_BUFFER_SIZE);
                micList->size = protomsg.info_size();
                for (int i = 0; i < protomsg.info_size(); i++) {
                    StMicInfo info;
                    info.tempid = protomsg.info(i).tempid();
                    bcopy(protomsg.info(i).uniqueid().c_str(), info.uniqueid, sizeof(info.uniqueid));
                    micList->micinfo[i] = info;
                }
                
                _func(enNotifyAddVoiceUser, OPERATION_OK, (const unsigned char*)micList, micList->getSize());
            }
            
            break;
        }
        case Cmd::enNotifyTakeMic:
        {
            Public::sdklog("接收到某人获得麦序指令");
            
            if (_sdkOpState == SdkUserJoinedRoom) {
                Cmd::cmdNotifyTakeMic protomsg;
                protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
                
                if (protomsg.tempid() == _sdkTempID) {
                    //轮到本人说话
                    if (_mediaSample) {
                        _mediaSample->setWetherSendVoiceData(true);
                        _sdkOpState = SdkUserSpeaking;
                    }
                }
                else {
                    //轮到他人说话
                    if (_mediaSample) {
                        _mediaSample->setWetherSendVoiceData(false);
                        _sdkOpState = SdkUserJoinedRoom;
                    }
                }
                
                StNotifyTakeMic micdata(protomsg.tempid(), protomsg.uniqueid().c_str(), protomsg.mtime());
                _func(enNotifyTakeMic, OPERATION_OK, (const unsigned char*)&micdata, sizeof(StNotifyTakeMic));
            }

            break;
        }
        case Cmd::enNotifyDelVoiceUser:
        {
            Public::sdklog("接收到删除语音通道指令");
            
            if (_sdkOpState == SdkUserJoinedRoom) {
                Cmd::cmdNotifyDelVoiceUser protomsg;
                protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
                for (int i = 0; i < protomsg.info_size(); i++) {
                    if (_mediaSample) {
                        _mediaSample->onDeleteChannel(protomsg.info(i).id(), MediaSample::data_in);
                    }
                }
            }
            
            break;
        }
        case Cmd::enNotifySomeEnterRoom:
        {
            Public::sdklog("接收到用户进入指令");
            
            if (_sdkOpState == SdkUserJoinedRoom) {
                Cmd::cmdNotifySomeEnterRoom protomsg;
                protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
                
                BUFFER_CMD(StNotifySomeEnterRoom, userList, MAX_BUFFER_SIZE);
                userList->size = protomsg.info_size();
                userList->needClearOld = (protomsg.reason() == Cmd::cmdNotifySomeEnterRoom::ENTER_REASON_NEW) ? true : false;
                for (int i = 0; i < protomsg.info_size(); i++) {
                    stRoomUserInfo info;
                    info.tempid = protomsg.info(i).tempid();
                    bcopy(protomsg.info(i).uniqueid().c_str(), info.uniqueid, sizeof(info.uniqueid));
                    userList->userinfo[i] = info;
                }
                
                _func(enNotifySomeEnterRoom, OPERATION_OK, (const unsigned char*)userList, userList->getSize());
            }
            
            break;
        }
        case Cmd::enNotifySomeLeaveRoom:
        {
            Public::sdklog("接收到用户离开指令");
            
            if (_sdkOpState == SdkUserJoinedRoom) {
                Cmd::cmdNotifySomeLeaveRoom protomsg;
                protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
                
                StNotifySomeLeaveRoom callbackdata;
                callbackdata.tempid = protomsg.tempid();
                
                _func(enNotifySomeLeaveRoom, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifySomeLeaveRoom));
            }
            
            break;
        }
        case Cmd::enNotifyRandChat:
        {
            Public::sdklog("接收到随机聊天请求指令");
            
            if (_sdkOpState == SdkUserLogined) {
                Cmd::cmdNotifyRandChat protomsg;
                protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
                
                StNotifyRandChat callbackdata(protomsg.tempid(), protomsg.uniqueid().c_str(), protomsg.roomid());
                _func(enNotifyRandChat, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyRandChat));
            }
            
            break;
        }
        case Cmd::enReturnRandChat:
        {
            Public::sdklog("接收到随机聊天响应指令");
            
            Cmd::cmdReturnRandChat protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            StReturnRandChat callbackdata(protomsg.isok(), protomsg.tempid());
            if (protomsg.isok()) {
                _func(enReturnRandChat, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyRandChat));
            }
            else {
                _func(enReturnRandChat, OPERATION_FAILED, (const unsigned char*)&callbackdata, sizeof(StNotifyRandChat));
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

//开始录制麦克风数据
bool RTChatSDKMain::startRecordVoice(const char* filename)
{
    if (_mediaSample) {
        return _mediaSample->startRecordVoice(filename);
    }
    else {
        return false;
    }
}

//停止录制麦克风数据
bool RTChatSDKMain::stopRecordVoice()
{
    if (_mediaSample) {
        return _mediaSample->stopRecordVoice();
    }
    else {
        return false;
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




