//
//  RTChatSdkIosHelper.h
//  flappyball
//
//  Created by raymon_wang on 14-8-18.
//
//

#ifndef __flappyball__RTChatSdkIosHelper__
#define __flappyball__RTChatSdkIosHelper__

#include <iostream>
#include <thread>
#include <queue>
#include "RTChat/RTChatSdk.h"

class RTChatSdkIosHelperDelegate {
public:
    virtual void onNotifyLoginResult(SdkErrorCode error, uint64_t tempid) {};
    
    virtual void onNotifyCreateResult(SdkErrorCode error, uint64_t roomid, enRoomType roomtype) {};
    
    virtual void onNotifyEnterResult(SdkErrorCode error, uint64_t roomid, enRoomType roomtype) {};
    
    virtual void onNotifyRoomList(SdkErrorCode error, const std::vector<StRoomInfo> infoVec) {};
    
    virtual void onNotifyAddVoiceUser(SdkErrorCode error, const std::vector<StVoiceUserInfo> infoVec) {};
    
    virtual void onNotifyDelVoiceUser(SdkErrorCode error, const std::vector<StVoiceUserInfo> infoVec) {};
    
    virtual void onNotifyMicQueue(SdkErrorCode error, const std::vector<StMicInfo> infoVec) {};
    
    virtual void onNotifyTakeMic(SdkErrorCode error, uint64_t tempid, uint32_t mtime) {};
};

class RTChatSdkIosHelper {
public:
    RTChatSdkIosHelper();
    virtual ~RTChatSdkIosHelper();
    
public:
    struct StCallBackData {
        StCallBackData() {
            bzero(this, sizeof(*this));
        }
        ~StCallBackData() {
            if(data)
                delete data;
            data = NULL;
        }
        SdkResponseCmd  cmdType;
        SdkErrorCode    error;
        char*           data;
        uint32_t        datasize;
    };
    typedef std::queue<StCallBackData*> MsgQueue;
    
    static RTChatSdkIosHelper& instance();
    
    void initCallBack();
    
    void RTChatCallBack(SdkResponseCmd cmdType, SdkErrorCode error = OPERATION_OK, const unsigned char* dataPtr = NULL, uint32_t dataSize = 0);
    
    void setDelegate(RTChatSdkIosHelperDelegate* delegate);
    
    RTChatSdkIosHelperDelegate* getDelegate();
    
    void visit();
    
protected:
    pthread_mutex_t         _mutexlock;     //SDK写数据线程锁
    uint64_t                _sdktempid;
    bool                    _needRefresh;
    MsgQueue                _msgQueue;
    
private:
    RTChatSdkIosHelperDelegate*          _voiceDelegate;
    
};

#endif /* defined(__flappyball__RTChatSdkIosHelper__) */
