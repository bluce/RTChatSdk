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
    //请求登录回调
    virtual void onNotifyLoginResult(SdkErrorCode error, uint64_t tempid) {};
    
    //创建房间回调
    virtual void onNotifyCreateResult(SdkErrorCode error, uint64_t roomid, enRoomType roomtype) {};
    
    //请求进入房间回调
    virtual void onNotifyEnterResult(SdkErrorCode error, uint64_t roomid, enRoomType roomtype) {};
    
    //服务器通知当前的房间列表
    virtual void onNotifyRoomList(SdkErrorCode error, const std::vector<StRoomInfo> infoVec) {};
    
    //服务器通知房间增加用户
    virtual void onNotifyAddVoiceUser(SdkErrorCode error, const std::vector<StVoiceUserInfo> infoVec) {};
    
    //服务器通知房间减少用户
    virtual void onNotifyDelVoiceUser(SdkErrorCode error, const std::vector<StVoiceUserInfo> infoVec) {};
    
    //服务器通知当前房间麦序
    virtual void onNotifyMicQueue(SdkErrorCode error, const std::vector<StMicInfo> infoVec) {};
    
    //服务器通知当前获取说话权限的用户
    virtual void onNotifyTakeMic(SdkErrorCode error, uint64_t tempid, uint32_t mtime) {};
    
    //通知有人进入房间
    virtual void onNotifySomeEnterRoom(SdkErrorCode error, const std::vector<stRoomUserInfo> infoVec) {};
    
    //通知有人离开房间
    virtual void onNotifySomeLeaveRoom(SdkErrorCode error, uint64_t tempid) {};
    
    //通知有人想和你随机聊天
    virtual void onNotifyRandChat(SdkErrorCode error, uint64_t srctempid, const std::string uniqueidstr, uint64_t roomid) {};
    
    //告知发起随机聊天结果
    virtual void onReturnRandChat(SdkErrorCode error) {};
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
    MsgQueue                _msgQueue;
    
private:
    RTChatSdkIosHelperDelegate*          _voiceDelegate;
    
};

#endif /* defined(__flappyball__RTChatSdkIosHelper__) */
