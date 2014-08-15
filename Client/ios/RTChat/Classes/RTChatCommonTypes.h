//
//  RTChatCommonTypes.h
//  RTChat
//
//  Created by raymon_wang on 14-8-7.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef RTChat_RTChatCommonTypes_h
#define RTChat_RTChatCommonTypes_h

#include <strings.h>
#include <stdint.h>

enum enRoomType
{
    ROOM_TYPE_MIN = 0,
    ROOM_TYPE_QUEUE = 1,
    ROOM_TYPE_FREE = 2,
    ROOM_TYPE_MAX = 3,
};

enum SdkOpState {
    SdkSocketUnConnected = 0,
    SdkSocketConnected,
    SdkUserLogining,
    SdkUserLogined,
    SdkUserCreatingRoom,
    SdkUserCreatedRoom,
    SdkUserjoiningRoom,
    SdkUserJoinedRoom,
    SdkUserWaitingToken,
    SdkUserSpeaking,
};

enum SdkResponseCmd {
    enNone = 0,
    
	/// 请求逻辑服务器信息
	enRequestLogicInfo = 1,
    
	/// 返回需要连接的逻辑服务器信息
	enNotifyLogicInfo = 2,
    
	/// 请求登录
	enRequestLogin = 3,
    
	/// 返回登录结果
	enNotifyLoginResult = 4,
    
	/// 创建房间
	enRequestCreateRoom = 5,
    
	/// 创建房间返回
	enNotifyCreateResult = 6,
    
	/// 加入房间
	enRequestEnterRoom = 7,
    
	/// 加入房间返回
	enNotifyEnterResult = 8,
    
	/// 申请房间列表
	enRequestRoomList = 9,
    
	/// 返回房间列表
	enNotifyRoomList = 10,
    
	/// 增加收听通道
	enNotifyAddVoiceUser = 11,
    
	/// 加入麦序
	enJoinMicQueue = 12,
    
	/// 离开麦序
	enLeaveMicQueue = 13,
    
	/// 返回麦序
	enNotifyMicQueue = 14,
    
	/// 到某人聊天了
	enNotifyTakeMic = 15,
    
	/// 离开房间
	enRequestLeaveRoom = 16,
    
    /// 删除一个通道
	enNotifyDelVoiceUser = 17,
};

enum SdkErrorCode {
    OPERATION_OK = 0,   //通用成功消息
    OPERATION_FAILED,   //通用失败消息

    LOGIC_RESULT_APPID_NOEXITS,
    LOGIC_RESULT_KEY_ERROR,
    LOGIC_RESULT_SYS_ERROR,
    LOGIC_RESULT_OK,
    
    LOGIN_RESULT_TOKEN_ERROR,
    LOGIN_RESULT_OK,
    
    ENTER_RESULT_NOEXITS,
    ENTER_RESULT_FULL,
    ENTER_RESULT_OK,
    ENTER_RESULT_ERROR,
    
    
};

struct StNotifyLoginResult {
    StNotifyLoginResult(bool res, uint64_t id) {
        isok = res;
        tempid = id;
    }
    bool isok;
    uint64_t tempid;
};

struct StNotifyCreateResult {
    StNotifyCreateResult(bool ok, uint64_t id) {
        isok = ok;
        roomid = id;
    }
    bool isok;
    uint64_t roomid;
};

struct StNotifyEnterResult {
    StNotifyEnterResult(uint64_t id, enRoomType type) {
        roomid = id;
        roomtype = type;
    };
    uint64_t roomid;
    enRoomType roomtype;
};

struct StRoomInfo {
    uint64_t roomid;
};

struct StNotifyRoomList {
    uint32_t size;
    StRoomInfo roominfo[0];
    uint32_t getSize() const {return sizeof(StNotifyRoomList)+ sizeof(StRoomInfo)*size;}
};

//收听用户信息
struct stVoiceUserInfo
{
	uint64_t userid;
};

//增加收听用户，即房间进入新用户
struct StNotifyAddVoiceUser {
    uint32_t size;
    stVoiceUserInfo userinfo[0];
    uint32_t getSize() const {return sizeof(StNotifyAddVoiceUser)+ sizeof(stVoiceUserInfo)*size;}
};

struct StMicInfo {
    StMicInfo() {
        userid = 0;
    }
    uint64_t userid;
};

struct StNotifyMicQueue {
    StNotifyMicQueue() {
        bzero(this, sizeof(*this));
    }
    uint32_t size;
    StMicInfo micinfo[0];
    uint32_t getSize() const {return sizeof(StNotifyMicQueue)+ sizeof(StMicInfo)*size;}
};

struct StNotifyTakeMic {
    StNotifyTakeMic(uint64_t id, uint32_t time) {
        tempid = id;
        mtime = time;
    }
    uint64_t tempid;    //持有麦的用户SDKID;
    uint32_t mtime; //麦序持续时间
};

//删除收听用户，即房间进入新用户
struct StNotifyDelVoiceUser {
    uint32_t size;
    stVoiceUserInfo userinfo[0];
    uint32_t getSize() const {return sizeof(StNotifyDelVoiceUser)+ sizeof(stVoiceUserInfo)*size;}
};

/******************回调字符串JSON格式******************/

// enNotifyLoginResult, 返回登录结果
// Json: {"data":{sdkid:"111"}}

// enNotifyCreateResult, 创建房间返回
// Json: {"data":{res:"true",roomid:"111",ip:"192.168.1.1",port:"8080"}}

// enEnterResult, 加入房间返回
// Json: {"data":{res:"true",ip:"192.168.1.1",port:"8080"}}

// enNotifyRoomList, 返回房间列表
// Json: {"data":[{roomid:"111"},{roomid:"222"}]}

// enNotifyRoomList, 返回返回麦序
// Json: {"data":[{sdkid:"111"},{sdkid:"222"}]}

// enTakeMic, 谁拿到了当前的麦
// Json: {"data":{"tempid":"1111", "mtime"="300"}}

#endif
