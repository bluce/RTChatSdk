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
    void requestLogin(const char* uniqueid);
    
    //申请房间列表
    void requestRoomList();
    
    //创建房间
    void createRoom(enRoomType roomType, enRoomReason reason);
    
    //加入房间
    void joinRoom(uint64_t roomid);
    
    //离开房间
    void leaveRoom();
    
    //加入麦序
    void requestInsertMicQueue();
    
    //离开麦序
    void leaveMicQueue();
    
    //是否接收随机聊天，临时增加的接口
    void returnRandChatRes(bool isAccept, uint64_t srctempid);
};

#endif
