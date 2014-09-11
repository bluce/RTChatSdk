//
//  RTChatSdkIosHelper.h
//  RTChatIOSDemo
//
//  Created by wang3140@hotmail.com on 14-9-5.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RTChatSdk.h"

using namespace rtchatsdk;

class RTChatSdkHelp;

@interface CallBackObject : NSObject

@property enum SdkResponseCmd    cmdType;
@property enum SdkErrorCode      error;
@property NSData*           data;

@end

@protocol RTChatSdkIosHelperDelegate <NSObject>

@optional

- (void)onRecvRTChatSDkMsg:(id)object;

@end

@interface RTChatSdkIosHelper : NSObject {
    RTChatSdkHelp*  rtchatsdkhelper;
}

@property (nonatomic, assign)   id <RTChatSdkIosHelperDelegate>   delegate;

+ (RTChatSdkIosHelper *) sharedInstance;

-(void) initHelper:(NSString*)appid key:(NSString*)key uniqueid:(NSString*)uniqueid;

-(void) activateSDK;

-(void) deActivateSDK;

/// 申请房间列表(主线程)
-(SdkErrorCode) requestRoomList;

/// 创建房间(主线程)
-(SdkErrorCode) createRoom:(enRoomType)roomType reason:(enRoomReason)reason;

/// 加入房间(主线程)
-(SdkErrorCode) joinRoom:(uint64_t)roomid;

/// 离开房间(主线程)
-(SdkErrorCode) leaveRoom;

/// 加入麦序(主线程)
-(SdkErrorCode) requestInsertMicQueue;

/// 离开麦序(主线程)
-(SdkErrorCode) leaveMicQueue;

///是否接收随机聊天，临时增加的接口(主线程)
-(void) returnRandChatRes:(bool)isAccept srctempid:(uint64_t)srctempid;

/// 设置本人Mac静音(主线程)
-(void) setMuteSelf:(bool)isMute;

/// 开始录制麦克风数据(主线程)
-(bool) startRecordVoice;

/// 停止录制麦克风数据(主线程)
-(bool) stopRecordVoice;

/// 开始播放录制数据(主线程)
-(bool) startPlayLocalVoice:(const char*)voiceUrl;

/// 停止播放数据(主线程)
-(bool) stopPlayLocalVoice;

/// 请求更改排麦房权限(主线程)
-(bool) requestUpdatePower:(uint64_t)othertempid powertype:(enPowerType)powertype;

/// 分配麦
-(bool) requestAssignMic:(uint64_t) othertempid;

-(void)CallBack:(SdkResponseCmd)cmdType error:(SdkErrorCode)error dataptr:(const unsigned char*)dataPtr dataSize:(uint32_t)dataSize;

-(void)onRTChatMsg:(id)object;

@end
