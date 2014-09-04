//
//  RTChatSdk.h
//  RTChat
//
//  Created by raymon_wang on 14-7-29.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef RTChat_RTChatSdk_h
#define RTChat_RTChatSdk_h

#include "RTChatCommonTypes.h"
#include <functional>

typedef std::function<void (SdkResponseCmd cmdType, SdkErrorCode error, const unsigned char* dataPtr, uint32_t dataSize)> pMsgCallFunc;

class RTChatSDKMain {
public:
    static RTChatSDKMain& sharedInstance();
    
    //sdk初始化，只能调用一次
    void initSDK(const std::string& appid, const std::string& key, const char* uniqueid = NULL);
    
    //注册消息回调
    void registerMsgCallback(const pMsgCallFunc& func);
    
    //激活SDK
    void activateSDK();
    
    //当应用最小化时需要调用这个，清理数据
    void deActivateSDK();
    
    //请求登录
    SdkErrorCode requestLogin(const char* uniqueid);
    
    //申请房间列表
    SdkErrorCode requestRoomList();
    
    //创建房间
    SdkErrorCode createRoom(enRoomType roomType, enRoomReason reason);
    
    //加入房间
    SdkErrorCode joinRoom(uint64_t roomid);
    
    //离开房间
    SdkErrorCode leaveRoom();
    
    //加入麦序
    SdkErrorCode requestInsertMicQueue();
    
    //离开麦序
    SdkErrorCode leaveMicQueue();
    
    //是否接收随机聊天，临时增加的接口
    void returnRandChatRes(bool isAccept, uint64_t srctempid);
    
    //设置本人Mac静音
    void setMuteSelf(bool isMute);
    
    //开始录制麦克风数据
    bool startRecordVoice();
    
    //停止录制麦克风数据
    bool stopRecordVoice();
    
    //开始播放录制数据
    bool startPlayLocalVoice(const char* voiceUrl);
    
    //停止播放数据
    bool stopPlayLocalVoice();
    
    //请求更改排麦房权限
    bool requestUpdatePower(uint64_t othertempid, enPowerType powertype);
    
    /// 分配麦
    bool requestAssignMic(uint64_t othertempid);
};

#endif
