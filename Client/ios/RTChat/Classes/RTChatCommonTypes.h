//
//  RTChatCommonTypes.h
//  RTChat
//
//  Created by raymon_wang on 14-8-7.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef RTChat_RTChatCommonTypes_h
#define RTChat_RTChatCommonTypes_h

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
	enAddVoiceUser = 11,
    
	/// 加入麦序
	enJoinMicQueue = 12,
    
	/// 离开麦序
	enLeaveMicQueue = 13,
    
	/// 返回麦序
	enNotifyMicQueue = 14,
    
	/// 到某人聊天了
	enTakeMic = 15,
    
	/// 离开房间
	enRequestLeaveRoom = 16,
};

/******************回调字符串JSON格式******************/

// enNotifyLoginResult, 返回登录结果
// Json: {"data":{sdkid:"111"}}

// enNotifyCreateResult, 创建房间返回
// Json: {"data":{res:"true",roomid:"111",ip:"192.168.1.1",port:"8080"}}

// enNotifyEnterResult, 加入房间返回
// Json: {"data":{res:"true",ip:"192.168.1.1",port:"8080"}}

// enNotifyRoomList, 返回房间列表
// Json: {"data":[{roomid:"111"},{roomid:"222"}]}

// enNotifyRoomList, 返回返回麦序
// Json: {"data":[{sdkid:"111"},{sdkid:"222"}]}

// enTakeMic, 谁拿到了当前的麦
// Json: {"data":{"tempid":"1111", "mtime"="300"}}

#endif
