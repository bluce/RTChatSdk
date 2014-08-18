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
            if (stCallBackData->error == OPERATION_OK) {
                _sdktempid = data->tempid;
            }
            else {
                printf("登录失败\n");
            }
            break;
        }
        default:
            break;
    }
    
    
    //处理完数据后，删除数据，收回内存
    delete stCallBackData;
}







