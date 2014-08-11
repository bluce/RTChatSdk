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

namespace Cmd {
    enum enRoomType
    {
        ROOM_TYPE_MIN = 0,
        ROOM_TYPE_QUEUE = 1,
        ROOM_TYPE_FREE = 2,
        ROOM_TYPE_MAX = 3,
    };
};

typedef std::function<void (SdkResponseCmd cmdType, const unsigned char* dataPtr, uint32_t dataSize)> pMsgCallFunc;

class RTChatSDKMain {
public:
    static RTChatSDKMain& sharedInstance();
    
    //sdk初始化，只能调用一次
    void initSDK(const std::string& uniqueid);
    
    //注册消息回调
    void registerMsgCallback(const pMsgCallFunc& func);
    
    //请求登录
    void requestLogin();
    
    //申请房间列表
    void requestRoomList();
    
    //创建房间
    void createRoom(Cmd::enRoomType roomType);
    
    //加入房间
    void joinRoom(uint64_t roomid);
    
    //离开房间
    void leaveRoom();
    
    //加入麦序
    void requestInsertMicQueue();
    
    //离开麦序
    void leaveMicQueue();
};

#endif
