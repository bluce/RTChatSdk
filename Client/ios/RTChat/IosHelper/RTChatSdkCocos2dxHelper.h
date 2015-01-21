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

using namespace rtchatsdk;

class RTChatSdkCocos2dxHelperDelegate {
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
    
    //通知录音返回
    virtual void onNotifyRequestRec(SdkErrorCode error, std::string urlstring, uint64_t duration) {};
    
    //通知播放返回
    virtual void onNotifyRequestPlay(SdkErrorCode error, std::string urlstring) {};
    
    /// 通知房间用户更新权限(更新显示)
    virtual void onNotifyUpdatePower(SdkErrorCode error, uint64_t tempid, enPowerType type) {};
    
    /// 分配麦返回结果
    virtual void onNotifyAssignResult(SdkErrorCode error, enAssignResult result) {};
};

class RTChatSdkCocos2dxHelper {
public:
    RTChatSdkCocos2dxHelper();
    virtual ~RTChatSdkCocos2dxHelper();
    
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
    
    static RTChatSdkCocos2dxHelper& instance();
    
    //帮助类初始化
    SdkErrorCode init(const std::string& appid, const std::string& key, const char* uniqueid = NULL);
    
    //初始化回调函数
    void initCallBack();
    
    //语音引擎回调函数
    void RTChatCallBack(SdkResponseCmd cmdType, SdkErrorCode error = OPERATION_OK, const unsigned char* dataPtr = NULL, uint32_t dataSize = 0);
    
    //设置代理指针
    void setDelegate(RTChatSdkCocos2dxHelperDelegate* delegate);
    
    //获取代理指针
    RTChatSdkCocos2dxHelperDelegate* getDelegate();
    
    //此函数需要在游戏帧循环调用
    void visit();
    
protected:
    pthread_mutex_t         _mutexlock;     //SDK写数据线程锁
    MsgQueue                _msgQueue;      //消息队列
    
private:
    RTChatSdkCocos2dxHelperDelegate*          _voiceDelegate;
    
};

#endif /* defined(__flappyball__RTChatSdkIosHelper__) */
