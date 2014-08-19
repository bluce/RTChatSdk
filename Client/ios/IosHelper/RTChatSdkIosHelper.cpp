//
//  RTChatSdkIosHelper.cpp
//  flappyball
//
//  Created by raymon_wang on 14-8-18.
//
//

#include "RTChatSdkIosHelper.h"

using namespace std::placeholders;

static RTChatSdkIosHelper* s_RTChatSdkIosHelper = NULL;

RTChatSdkIosHelper::RTChatSdkIosHelper() :
_voiceDelegate(NULL)
{
    pthread_mutex_init(&_mutexlock, PTHREAD_MUTEX_NORMAL);
}

RTChatSdkIosHelper::~RTChatSdkIosHelper()
{
    
}

void RTChatSdkIosHelper::initCallBack()
{
    RTChatSDKMain::sharedInstance().registerMsgCallback(std::bind(&RTChatSdkIosHelper::RTChatCallBack, this, _1, _2, _3, _4));
}

RTChatSdkIosHelper& RTChatSdkIosHelper::instance()
{
    if (!s_RTChatSdkIosHelper) {
        s_RTChatSdkIosHelper = new RTChatSdkIosHelper();
    }
    
    return *s_RTChatSdkIosHelper;
}

void RTChatSdkIosHelper::RTChatCallBack(SdkResponseCmd cmdType, SdkErrorCode error, const unsigned char* dataPtr, uint32_t dataSize)
{
    pthread_mutex_lock(&_mutexlock);
    
    StCallBackData* stdata = new StCallBackData;
    stdata->cmdType = cmdType;
    stdata->error = error;
    stdata->data = new char[dataSize];
    memcpy(stdata->data, dataPtr, dataSize);
    stdata->datasize = dataSize;
    _msgQueue.push(stdata);
    
    pthread_mutex_unlock(&_mutexlock);
}

void RTChatSdkIosHelper::setDelegate(RTChatSdkIosHelperDelegate* delegate)
{
    _voiceDelegate = delegate;
}

RTChatSdkIosHelperDelegate* RTChatSdkIosHelper::getDelegate()
{
    return _voiceDelegate;
}

void RTChatSdkIosHelper::visit()
{
    StCallBackData* stCallBackData = NULL;
    
    pthread_mutex_lock(&_mutexlock);
    
    if (_msgQueue.size() > 0) {
        stCallBackData = _msgQueue.front();
        _msgQueue.pop();
    }
    else {
        pthread_mutex_unlock(&_mutexlock);
        return;
    }
    
    pthread_mutex_unlock(&_mutexlock);
    
    if (!stCallBackData) {
        return;
    }
    
    switch (stCallBackData->cmdType) {
        case enNotifyLoginResult:
        {
            StNotifyLoginResult* data = (StNotifyLoginResult*)stCallBackData->data;
            if (_voiceDelegate) {
                _voiceDelegate->onNotifyLoginResult(stCallBackData->error, data->tempid);
            }
            break;
        }
        case enNotifyCreateResult:
        {
            StNotifyCreateResult* data = (StNotifyCreateResult*)stCallBackData->data;
            if (_voiceDelegate) {
                _voiceDelegate->onNotifyCreateResult(stCallBackData->error, data->roomid, data->roomtype);
            }
            break;
        }
        case enNotifyEnterResult:
        {
            StNotifyEnterResult* data = (StNotifyEnterResult*)stCallBackData->data;
            if (_voiceDelegate) {
                _voiceDelegate->onNotifyEnterResult(stCallBackData->error, data->roomid, data->roomtype);
            }
            break;
        }
        case enNotifyRoomList:
        {
            std::vector<StRoomInfo> infoVec;
            StNotifyRoomList* data = (StNotifyRoomList*)stCallBackData->data;
            for (int i = 0; i < data->size; i++) {
                infoVec.push_back(data->roominfo[i]);
            }
            if (_voiceDelegate) {
                _voiceDelegate->onNotifyRoomList(stCallBackData->error, infoVec);
            }
            break;
        }
//        case enNotifyAddVoiceUser:
//        {
//            std::vector<StVoiceUserInfo> infoVec;
//            StNotifyAddVoiceUser* data = (StNotifyAddVoiceUser*)stCallBackData->data;
//            for (int i = 0; i < data->size; i++) {
//                infoVec.push_back(data->userinfo[i]);
//            }
//            if (_voiceDelegate) {
//                _voiceDelegate->onNotifyAddVoiceUser(stCallBackData->error, infoVec);
//            }
//            break;
//        }
//        case enNotifyDelVoiceUser:
//        {
//            std::vector<StVoiceUserInfo> infoVec;
//            StNotifyDelVoiceUser* data = (StNotifyDelVoiceUser*)stCallBackData->data;
//            for (int i = 0; i < data->size; i++) {
//                infoVec.push_back(data->userinfo[i]);
//            }
//            if (_voiceDelegate) {
//                _voiceDelegate->onNotifyDelVoiceUser(stCallBackData->error, infoVec);
//            }
//            break;
//        }
        case enNotifyMicQueue:
        {
            std::vector<StMicInfo> infoVec;
            StNotifyMicQueue* data = (StNotifyMicQueue*)stCallBackData->data;
            for (int i = 0; i < data->size; i++) {
                infoVec.push_back(data->micinfo[i]);
            }
            if (_voiceDelegate) {
                _voiceDelegate->onNotifyMicQueue(stCallBackData->error, infoVec);
            }
            break;
        }
        case enNotifyTakeMic:
        {
            StNotifyTakeMic* data = (StNotifyTakeMic*)stCallBackData->data;
            if (_voiceDelegate) {
                _voiceDelegate->onNotifyTakeMic(stCallBackData->error, data->tempid, data->mtime);
            }
            break;
        }
        case enNotifySomeEnterRoom:
        {
            std::vector<stRoomUserInfo> infoVec;
            StNotifySomeEnterRoom* data = (StNotifySomeEnterRoom*)stCallBackData->data;
            for (int i = 0; i < data->size; i++) {
                infoVec.push_back(data->userinfo[i]);
            }
            if (_voiceDelegate) {
                _voiceDelegate->onNotifySomeEnterRoom(stCallBackData->error, infoVec);
            }
            break;
        }
        case enNotifySomeLeaveRoom:
        {
            StNotifySomeLeaveRoom* data = (StNotifySomeLeaveRoom*)stCallBackData->data;
            if (_voiceDelegate) {
                _voiceDelegate->onNotifySomeLeaveRoom(stCallBackData->error, data->tempid);
            }
            break;
        }
        case enNotifyRandChat:
        {
            StNotifyRandChat* data = (StNotifyRandChat*)stCallBackData->data;
            if (_voiceDelegate) {
                _voiceDelegate->onNotifyRandChat(stCallBackData->error, data->tempid, data->uniqueid, data->roomid);
            }

            break;
        }
        case enReturnRandChat:
        {
            if (_voiceDelegate) {
                _voiceDelegate->onReturnRandChat(stCallBackData->error);
            }
            break;
        }
        default:
            break;
    }
    
    
    //处理完数据后，删除数据，收回内存
    delete stCallBackData;
}







