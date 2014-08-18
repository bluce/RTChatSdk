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
    virtual void onNotifyLoginResult() = 0;
    
    virtual void onNotifyCreateResult() = 0;
    
    virtual void onNotifyEnterResult() = 0;
    
    virtual void onNotifyRoomList() = 0;
    
    virtual void onNotifyMicQueue() = 0;
    
    virtual void onNotifyTakeMic() = 0;
    
    virtual void onNotifyDelVoiceUser() = 0;
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
