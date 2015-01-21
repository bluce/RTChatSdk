//
//  RTChatSdkCocos2dxHelper.cpp
//  flappyball
//
//  Created by raymon_wang on 14-8-18.
//
//

#include "RTChatSdkCocos2dxHelper.h"

using namespace std::placeholders;

static RTChatSdkCocos2dxHelper* s_RTChatSdkCocos2dxHelper = NULL;

RTChatSdkCocos2dxHelper::RTChatSdkCocos2dxHelper() :
_voiceDelegate(NULL)
{
    pthread_mutex_init(&_mutexlock, 0);
}

RTChatSdkCocos2dxHelper::~RTChatSdkCocos2dxHelper()
{
    
}

void RTChatSdkCocos2dxHelper::initCallBack()
{
    RTChatSDKMain::sharedInstance().registerMsgCallback(std::bind(&RTChatSdkCocos2dxHelper::RTChatCallBack, this, _1, _2, _3, _4));
}

RTChatSdkCocos2dxHelper& RTChatSdkCocos2dxHelper::instance()
{
    if (!s_RTChatSdkCocos2dxHelper) {
        s_RTChatSdkCocos2dxHelper = new RTChatSdkCocos2dxHelper();
    }
    
    return *s_RTChatSdkCocos2dxHelper;
}

SdkErrorCode RTChatSdkCocos2dxHelper::init(const std::string& appid, const std::string& key, const char* uniqueid)
{
    initCallBack();
    
    return RTChatSDKMain::sharedInstance().initSDK(appid, key, uniqueid);
}

void RTChatSdkCocos2dxHelper::RTChatCallBack(SdkResponseCmd cmdType, SdkErrorCode error, const unsigned char* dataPtr, uint32_t dataSize)
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

void RTChatSdkCocos2dxHelper::setDelegate(RTChatSdkCocos2dxHelperDelegate* delegate)
{
    _voiceDelegate = delegate;
}

RTChatSdkCocos2dxHelperDelegate* RTChatSdkCocos2dxHelper::getDelegate()
{
    return _voiceDelegate;
}

void RTChatSdkCocos2dxHelper::visit()
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
            if (data->needClearOld) {
                infoVec.clear();
            }
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
        case enRequestRec:
        {
            StRequestRec* data = (StRequestRec*)stCallBackData->data;
            if (_voiceDelegate) {
                if (!data) {
                    _voiceDelegate->onNotifyRequestRec(stCallBackData->error, "", 0);
                }
                else {
                    _voiceDelegate->onNotifyRequestRec(stCallBackData->error, data->urlbuf, data->voicetime);
                }
            }
            break;
        }
        case enRequestPlay:
        {
            if (_voiceDelegate) {
                _voiceDelegate->onNotifyRequestPlay(stCallBackData->error, "");
            }
            break;
        }
        case enNotifyUpdatePower:
        {
            StNotifyUpdatePower* data = (StNotifyUpdatePower*)stCallBackData->data;
            if (_voiceDelegate) {
                _voiceDelegate->onNotifyUpdatePower(stCallBackData->error, data->tempid, data->power);
            }
            break;
        }
        case enNotifyAssignResult:
        {
            StNotifyAssignResult* data = (StNotifyAssignResult*)stCallBackData->data;
            if (_voiceDelegate) {
                _voiceDelegate->onNotifyAssignResult(stCallBackData->error, data->result);
            }
            break;
        }
        default:
            break;
    }
    
    
    //处理完数据后，删除数据，收回内存
    delete stCallBackData;
}







