//
//  RTChatSdk.h
//  RTChat
//
//  Created by raymon_wang on 14-7-29.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef RTChat_RTChatSdk_h
#define RTChat_RTChatSdk_h

namespace Cmd {
    enum enRoomType
    {
        ROOM_TYPE_MIN = 0,
        ROOM_TYPE_QUEUE = 1,
        ROOM_TYPE_FREE = 2,
        ROOM_TYPE_MAX = 3,
    };
}

typedef std::function<void (long long roomid)> pMsgCallFunc;

class RTChatSDKMain {
public:
    static RTChatSDKMain& sharedInstance();
    
    void initSDK(const std::string& uniqueid);
    
    void registerMsgCallback(const pMsgCallFunc& func);
    
    //创建房间
    void createRoom(Cmd::enRoomType roomType);
    
    //加入房间
    void joinRoom(uint64_t roomid);
    
    //请求排麦
    void requestInsertMicQueue();
    
    //获取当前的输入mic静音状态
    bool getMuteSelf();
    
    //设置本人Mac静音
    void setMuteSelf(bool isMute);
    
    //申请房间列表
    void requestRoomList();
};

#endif
