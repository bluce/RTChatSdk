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
#include "RTChatBuffStream.h"
#include "RTChatBuffStreamPool.h"
#include "defines.h"
#include "public.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace rtchatsdk {

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
_sdkOpState(SdkControlUnConnected),
_sdkTempID(0),
_appid(""),
_appkey(""),
_token(""),
_uniqueid(""),
_gateWayIP(""),
_gateWayPort(0),
_func(NULL),
_buffStream(NULL),
_playBuffPool(NULL),
_isrecording(false),
_recordstarttime(0),
_recordduration(0),
_workThread(NULL)
{
    _netDataManager = new NetDataManager;
    
    //创建语音引擎
    _mediaSample = new MediaSample;
    
    //创建录音缓冲区
    _buffStream = new RTChatBuffStream(128000);
    
    //创建播放缓冲池
    _playBuffPool = new RTChatBuffStreamPool(5);
    if (_playBuffPool) {
        _playBuffPool->init();
    }
    
    _workThread = ThreadWrapper::CreateThread(RTChatSDKMain::Run, this, kNormalPriority, "MainWorkThread");
    
    initMutexLock();
}

RTChatSDKMain::~RTChatSDKMain()
{
    SAFE_DELETE(_netDataManager);
    SAFE_DELETE(_mediaSample);
    SAFE_DELETE(_buffStream);
    SAFE_DELETE(_playBuffPool);
    
    if (_workThread) {
        _workThread->Stop();
    }
    SAFE_DELETE(_workThread);
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
    
    HttpProcess::instance().registerCallBack(std::bind(&RTChatSDKMain::httpRequestCallBack, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

//注册消息回调
void RTChatSDKMain::registerMsgCallback(const pMsgCallFunc& func)
{
    _func = func;
}

//激活SDK
void RTChatSDKMain::activateSDK()
{
    Public::sdklog("激活SDK");
    if (_func == NULL) {
        return;
    }
    
    SdkOpState currentstate = getSdkState();
    
    if (currentstate < SdkGateWaySocketUnConnected) {
        openControlConnection();
    }
    else {
        openGateWayConnection();
    }
    
    if (_mediaSample) {
        _mediaSample->init();
    }
    
    unsigned int id = 0;
    if (_workThread) {
        _workThread->Start(id);
    }
}

//当应用最小化时需要调用这个，清理数据
void RTChatSDKMain::deActivateSDK()
{
    Public::sdklog("冻结SDK");
    if (_netDataManager) {
//        _netDataManager->deactive();
        _netDataManager->destroyWebSocket();
    }
    
    if (_mediaSample) {
        _mediaSample->closeVoiceEngine();
    }
    
//    _uniqueid = "";
//    _sdkTempID = 0;
    _currentRoomID = 0;
    _isMute = false;
    
    SdkOpState currentstate = getSdkState();
    
    if (currentstate >= SdkGateWaySocketUnConnected) {
        set_SdkOpState(SdkGateWaySocketUnConnected);
    }
    else {
        set_SdkOpState(SdkControlUnConnected);
    }
    
    if (_workThread) {
        _workThread->Stop();
    }
}

//获取SDK当前操作状态
SdkOpState RTChatSDKMain::getSdkState()
{
    SdkOpState currentstate;
    pthread_mutex_lock(&_mutexLock);
    currentstate = _sdkOpState;
    pthread_mutex_unlock(&_mutexLock);
    return currentstate;
}

/// 申请房间列表(主线程)
SdkErrorCode RTChatSDKMain::requestRoomList()
{
    tryConnectServer();
    
    SdkOpState currentstate = getSdkState();
    if (currentstate < SdkUserLogined) {
        return OPERATION_FAILED;
    }
    
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestRoomList;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char *)&cmd, cmd.getSize());
    }
    
    return OPERATION_OK;
}

//创建房间(主线程)
SdkErrorCode RTChatSDKMain::createRoom(enRoomType roomType, enRoomReason reason)
{
    tryConnectServer();
    
    SdkOpState currentstate = getSdkState();
    if (currentstate != SdkUserLogined) {
        return OPERATION_FAILED;
    }
    
    Cmd::cmdRequestCreateRoom msg;
    msg.set_type((Cmd::enRoomType)roomType);
    msg.set_reason((Cmd::enRoomReason)reason);
    
    set_SdkOpState(SdkUserCreatingRoom);
    
    SendProtoMsg(msg, Cmd::enRequestCreateRoom);
    
    return OPERATION_OK;
}

SdkErrorCode RTChatSDKMain::joinRoom(uint64_t roomid)
{
    tryConnectServer();
    
    SdkOpState currentstate = getSdkState();
    if (currentstate != SdkUserLogined) {
        return OPERATION_FAILED;
    }
    
    Cmd::cmdRequestEnterRoom msg;
    msg.set_roomid(roomid);
    
    set_SdkOpState(SdkUserjoiningRoom);
    
    SendProtoMsg(msg, Cmd::enRequestEnterRoom);
    
    return OPERATION_OK;
}

//离开房间
SdkErrorCode RTChatSDKMain::leaveRoom()
{
    tryConnectServer();
    
    SdkOpState currentstate = getSdkState();
    if (currentstate < SdkUserjoiningRoom) {
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
    
    set_SdkOpState(SdkUserLogined);
    
    return OPERATION_OK;
}

//加入麦序
SdkErrorCode RTChatSDKMain::requestInsertMicQueue()
{
    tryConnectServer();
    
    SdkOpState currentstate = getSdkState();
    if (currentstate != SdkUserJoinedRoom) {
        return OPERATION_FAILED;
    }
    
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestJoinMicQueue;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char*)&cmd, cmd.getSize());
    }
    
    set_SdkOpState(SdkUserWaitingToken);
    
    return OPERATION_OK;
}

//离开麦序
SdkErrorCode RTChatSDKMain::leaveMicQueue()
{
    tryConnectServer();
    
    SdkOpState currentstate = getSdkState();
    if (currentstate < SdkUserWaitingToken) {
        return OPERATION_FAILED;
    }
    
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestLeaveMicQueue;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char*)&cmd, cmd.getSize());
    }
    
    set_SdkOpState(SdkUserJoinedRoom);
    
    return OPERATION_OK;
    
//    if (_mediaSample) {
//        _mediaSample->setMuteMic(true);
//    }
}

//是否接收随机聊天，临时增加的接口
void RTChatSDKMain::returnRandChatRes(bool isAccept, uint64_t srctempid)
{
    SdkOpState currentstate = getSdkState();
    if (currentstate != SdkUserLogined) {
        return;
    }
    
    Cmd::cmdReturnRandChat msg;
    msg.set_isok(isAccept);
    msg.set_tempid(srctempid);
    
    SendProtoMsg(msg, Cmd::enReturnRandChat);
}

/// 设置本人Mac静音(主线程)
void RTChatSDKMain::setMuteSelf(bool isMute)
{
    if (_mediaSample) {
        _mediaSample->setMuteMic(isMute);
        _isMute = isMute;
    }
}

/// 开始录制麦克风数据
bool RTChatSDKMain::startRecordVoice()
{
    if (_isrecording) {
        return false;
    }
    
    if (_mediaSample) {
        pthread_mutex_lock(&_mutexLock);
        _isrecording = true;
        pthread_mutex_unlock(&_mutexLock);
        
        TimeCounter::instance().registerTimeOutCallBack("record", 30, std::bind(&RTChatSDKMain::recordTimeExceed, this, std::placeholders::_1));
        _recordstarttime = time(NULL);
        _buffStream->resetBuffStream();
        return _mediaSample->startRecordVoice(_buffStream);
    }
    else {
        return false;
    }
}

/// 停止录制麦克风数据
bool RTChatSDKMain::stopRecordVoice()
{
    bool isrecording;
    
    pthread_mutex_lock(&_mutexLock);
    isrecording = _isrecording;
    pthread_mutex_unlock(&_mutexLock);
    
    if (!isrecording) {
        return false;
    }
    
    bool result = false;
    if (_mediaSample) {
        if ( (result = _mediaSample->stopRecordVoice()) ) {
            uploadVoiceData();
            
            pthread_mutex_lock(&_mutexLock);
            _recordduration = time(NULL) - _recordstarttime;
            _isrecording = false;
            pthread_mutex_unlock(&_mutexLock);
        }
    }
    else {
        return false;
    }
    
    return result;
}

/// 开始播放录制数据
bool RTChatSDKMain::startPlayLocalVoice(const char *voiceUrl)
{
    if (!voiceUrl) {
        return false;
    }
    
    stopPlayLocalVoice();
    
    RTChatBuffStream* instream = NULL;
    
    if (_playBuffPool) {
        RTChatBuffStreamPool::StBuffInfo* info = _playBuffPool->getAvailableBuff(voiceUrl);
        if (info && !info->needDownload) {
            instream = info->buffstream;
            instream->resetReadPosition();
            playVoiceStream(instream);
        }
        else {
            HttpProcess::instance().requestContent(voiceUrl);
            _downloadingfileurl = voiceUrl;
        }
    }
    else {
        return false;
    }
    
    return true;
}

/// 停止播放数据(主线程)
bool RTChatSDKMain::stopPlayLocalVoice()
{
    if (_mediaSample) {
        return _mediaSample->stopPlayLocalStream();
    }
    else {
        return false;
    }
}

///请求更改排麦房权限(主线程)
bool RTChatSDKMain::requestUpdatePower(uint64_t othertempid, enPowerType powertype)
{
    tryConnectServer();
    
    SdkOpState currentstate = getSdkState();
    if (currentstate < SdkUserJoinedRoom) {
        return OPERATION_FAILED;
    }
    
    Cmd::cmdRequestUpdatePower msg;
    msg.set_tempid(othertempid);
    msg.set_power((Cmd::enPowerType)powertype);
    
    SendProtoMsg(msg, Cmd::enRequestCreateRoom);
    
    return OPERATION_OK;
}

/// 分配麦(主线程)
bool RTChatSDKMain::requestAssignMic(uint64_t othertempid)
{
    tryConnectServer();
    
    SdkOpState currentstate = getSdkState();
    if (currentstate < SdkUserJoinedRoom) {
        return OPERATION_FAILED;
    }
    
    Cmd::cmdRequestAssignMic msg;
    msg.set_tempid(othertempid);
    
    SendProtoMsg(msg, Cmd::enRequestAssignMic);
    
    return OPERATION_OK;
}

///打开控制连接
void RTChatSDKMain::openControlConnection()
{
    if (_netDataManager) {
        _netDataManager->init(Public::SdkAvar("ws://%s", ControlServerAddr));
        _netDataManager->activity();
    }
    set_SdkOpState(SdkControlConnecting);
}

///关闭控制连接
void RTChatSDKMain::closeControlConnection()
{
    if (_netDataManager) {
//        _netDataManager->deactive();
        _netDataManager->destroyWebSocket();
    }
}

///打开网关服务器连接
void RTChatSDKMain::openGateWayConnection()
{
    if (_gateWayIP == "" || _gateWayPort == 0) {
        return;
    }
    
    if (_netDataManager) {
        _netDataManager->init(Public::SdkAvar("ws://%s:%u", _gateWayIP.c_str(), _gateWayPort));
    }
    
    set_SdkOpState(SdkGateWaySocketConnecting);
}

//上传录制的语音数据
void RTChatSDKMain::uploadVoiceData()
{
    const RTChatBuffStream::BuffVec& buffvec = _buffStream->getBuffVec();
    HttpProcess::instance().postContent(VoiceUpLoadUrlHead, &buffvec[0], _buffStream->get_datasize());
}

//调用底层引擎播放流
void RTChatSDKMain::playVoiceStream(RTChatBuffStream *instream)
{
    if (_mediaSample && instream) {
        _mediaSample->startPlayLocalStream(instream);
    }
}

//录音超过最大时间回调
void RTChatSDKMain::recordTimeExceed(int time)
{
    Public::sdklog("录音时间超长%d", time);
    
    stopRecordVoice();
}

/// 请求发送漂流瓶(测试用接口)
bool RTChatSDKMain::requestRandPlay(const std::string& url)
{
    SdkOpState currentstate = getSdkState();
    if (currentstate >= SdkUserCreatingRoom) {
        return OPERATION_FAILED;
    }
    
    Cmd::cmdRequestRandPlay msg;
    msg.set_url(url);
    
    SendProtoMsg(msg, Cmd::enRequestRandPlay);
    
    return OPERATION_OK;
}

void RTChatSDKMain::onRecvMsg(char *data, int len)
{
    stBaseCmd* cmd = (stBaseCmd*)data;
    
    Public::sdklog("cmdid=%u", cmd->cmdid);
    
    SdkOpState currentstate = getSdkState();
    
    switch (cmd->cmdid) {
        case Cmd::enNotifyLogicInfo:
        {
            Cmd::cmdNotifyLogicInfo protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            _gateWayIP = protomsg.gateip();
            _gateWayPort = protomsg.gateport();
            
            set_SdkOpState(SdkGateWaySocketUnConnected);
            
            closeControlConnection();
            
            openGateWayConnection();
            
            break;
        }
        case Cmd::enNotifyLoginResult:
        {
            Cmd::cmdNotifyLoginResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.result() == Cmd::cmdNotifyLoginResult::LOGIN_RESULT_OK) {
                set_SdkOpState(SdkUserLogined);
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
            if (currentstate == SdkUserJoinedRoom) {
                leaveRoom();
            }
            
            Cmd::cmdNotifyCreateResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.isok()) {
                connectVoiceRoom(protomsg.ip(), protomsg.port());
                
                set_SdkOpState(SdkUserJoinedRoom);
                
                //这里需要加锁吗，待处理
                _currentRoomID = protomsg.roomid();
                
                
                //回调应用数据
                StNotifyCreateResult callbackdata(true, protomsg.roomid(), (enRoomType)protomsg.type());
                _func(enNotifyCreateResult, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyCreateResult));
            }
            else {
                _sdkOpState = SdkGateWaySocketConnected;
                _func(enNotifyCreateResult, OPERATION_FAILED, NULL, 0);
            }
            break;
        }
        case Cmd::enNotifyEnterResult:
        {
            Public::sdklog("接收到进入房间指令");
            
            if (currentstate == SdkUserJoinedRoom) {
                leaveRoom();
            }
            
            Cmd::cmdNotifyEnterResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            if (protomsg.result() == Cmd::cmdNotifyEnterResult::ENTER_RESULT_OK) {
                if (_sdkOpState != SdkUserJoinedRoom) {
                    connectVoiceRoom(protomsg.ip(), protomsg.port());
                    
                    set_SdkOpState(SdkUserJoinedRoom);
                    
                    StNotifyEnterResult callbackdata(protomsg.roomid(), (enRoomType)protomsg.type());
                    _func(enNotifyEnterResult, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyEnterResult));
                }
            }
            else {
                set_SdkOpState(SdkUserLogined);
                
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
            
            if (currentstate >= SdkUserJoinedRoom) {
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
            
            if (currentstate >= SdkUserJoinedRoom) {
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
            
            if (currentstate >= SdkUserJoinedRoom) {
                Cmd::cmdNotifyTakeMic protomsg;
                protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
                
                if (protomsg.tempid() == _sdkTempID) {
                    //轮到本人说话
                    if (_mediaSample) {
                        _mediaSample->setWetherSendVoiceData(true);
                        set_SdkOpState(SdkUserSpeaking);
                    }
                }
                else {
                    //轮到他人说话
                    if (_mediaSample) {
                        _mediaSample->setWetherSendVoiceData(false);
                        set_SdkOpState(SdkUserJoinedRoom);
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
            
            if (currentstate >= SdkUserJoinedRoom) {
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
            Public::sdklog("接收到通知有用户进入房间指令");
            
            if (currentstate >= SdkUserJoinedRoom) {
                Cmd::cmdNotifySomeEnterRoom protomsg;
                protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
                
                BUFFER_CMD(StNotifySomeEnterRoom, userList, MAX_BUFFER_SIZE);
                userList->size = protomsg.info_size();
                userList->needClearOld = (protomsg.reason() == Cmd::cmdNotifySomeEnterRoom::ENTER_REASON_NEW) ? true : false;
                for (int i = 0; i < protomsg.info_size(); i++) {
                    stRoomUserInfo info;
                    info.tempid = protomsg.info(i).tempid();
                    info.power = (enPowerType)protomsg.info(i).power();
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
            
            if (currentstate >= SdkUserJoinedRoom) {
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
            
            if (currentstate == SdkUserLogined) {
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
        case Cmd::enNotifyUpdatePowerResult:
        {
            Public::sdklog("接收到操作反馈结果");
            break;
        }
        case Cmd::enNotifyUpdatePower:
        {
            Public::sdklog("接收到更改用户权限指令");
            
            Cmd::cmdNotifyUpdatePower protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            StNotifyUpdatePower callbackdata(protomsg.tempid(), (enPowerType)protomsg.power());
            _func(enNotifyUpdatePower, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StNotifyUpdatePower));
            
            break;
        }
        case Cmd::enNotifyAssignResult:
        {
            Public::sdklog("分配麦操作反馈结果");
            
            Cmd::cmdNotifyAssignResult protomsg;
            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
            StNotifyAssignResult callbackdata((enAssignResult)protomsg.result());
            _func(enNotifyAssignResult, OPERATION_FAILED, (const unsigned char*)&callbackdata, sizeof(StNotifyAssignResult));
            
            break;
        }
        case Cmd::enNotifyRandPlay:
        {
            Public::sdklog("收到漂流瓶");
            
//            Cmd::cmdNotifyRandPlay protomsg;
//            protomsg.ParseFromArray(cmd->data, cmd->cmdlen);
            
//            startPlayLocalVoice(protomsg.url().c_str());
            
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

/// 随机进入一个房间（测试用接口）
void RTChatSDKMain::randomJoinRoom()
{
    if (_roomInfoMap.size() != 0) {
        joinRoom(_roomInfoMap.rbegin()->first);
    }
}

//获取当前的输入mic静音状态
bool RTChatSDKMain::getMuteSelf()
{
    return _isMute;
}

//http请求回调函数(主线程工作)
void RTChatSDKMain::httpRequestCallBack(HttpDirection direction, const char *ptr, int size)
{
    if (direction == HttpProcess_Upload) {
        if (!ptr) {
            //失败
            _func(enRequestRec, OPERATION_FAILED, NULL, 0);
        }
        else {
            StRequestRec callbackdata;
            callbackdata.isok = true;
            callbackdata.voicetime = _recordduration;
            strncpy(callbackdata.urlbuf, ptr, size);
            _func(enRequestRec, OPERATION_OK, (const unsigned char*)&callbackdata, sizeof(StRequestRec));
            
            //测试代码
//            requestRandPlay(callbackdata.urlbuf);
        }
    }
    else {
        if (!ptr) {
            //失败
            //Todo...
            _func(enRequestPlay, OPERATION_FAILED, NULL, 0);
        }
        else {
            RTChatBuffStream* instream = _playBuffPool->updateCurrentBuff(_downloadingfileurl, ptr, size);
            if (instream) {
                playVoiceStream(instream);
                _func(enRequestPlay, OPERATION_OK, NULL, 0);
            }
            else {
                _func(enRequestPlay, OPERATION_FAILED, NULL, 0);
            }
        }
    }
}

bool RTChatSDKMain::Run(ThreadObj obj)
{
    RTChatSDKMain* mgr = static_cast<RTChatSDKMain*>(obj);
    
    return mgr->Process();
}

bool RTChatSDKMain::Process()
{
    SdkOpState currentstate = getSdkState();
    
    if (currentstate == SdkControlConnected) {
        requestLogicInfo();
        set_SdkOpState(SdkWaitingGateWayInfo);
    }
    if (currentstate == SdkGateWaySocketUnConnected) {
        
    }
    if (currentstate == SdkGateWaySocketConnected) {
        requestLogin();
    }
    
    if (_netDataManager) {
        _netDataManager->Process();
    }
    return true;
}

/// 尝试重连服务器
void RTChatSDKMain::tryConnectServer()
{
    SdkOpState currentstate = getSdkState();
    if (currentstate == SdkControlUnConnected || currentstate == SdkGateWaySocketUnConnected) {
        _netDataManager->resetRetryCount();
    }
}

///请求逻辑服务器地址
SdkErrorCode RTChatSDKMain::requestLogicInfo()
{
    if (_appid == "" || _appkey == "") {
        return OPERATION_FAILED;
    }
    
    Cmd::cmdRequestLogicInfo msg;
    msg.set_appid(_appid);
    msg.set_key(_appkey);
    
    SendProtoMsg(msg, Cmd::enRequestLogicInfo);
    
    return OPERATION_OK;
}

/// 请求登录(工作线程)
SdkErrorCode RTChatSDKMain::requestLogin(const char* uniqueid)
{
    if (uniqueid != NULL) {
        _uniqueid = uniqueid;
    }
    if (_uniqueid == "") {
        return OPERATION_FAILED;
    }
    
    SdkOpState currentstate = getSdkState();
    if (currentstate < SdkGateWaySocketConnected) {
        return OPERATION_FAILED;
    }
    
    set_SdkOpState(SdkUserLogining);
    
    Cmd::cmdRequestLogin msg;
    msg.set_uniqueid(_uniqueid);
    msg.set_token("yuew89341huidy89iuh1");
    msg.set_tempid(_sdkTempID);
    
    SendProtoMsg(msg, Cmd::enRequestLogin);
    
    Public::sdklog("发送登录消息完成");
    
    return OPERATION_OK;
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
    
}




