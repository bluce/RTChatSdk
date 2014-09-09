//
//  RTChatSdkIosHelper.m
//  RTChatIOSDemo
//
//  Created by raymon_wang on 14-9-5.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#import "RTChatSdkIosHelper.h"
#import <string>

using namespace std::placeholders;

class RTChatSdkHelp {
public:
    RTChatSdkHelp() {};
    virtual ~RTChatSdkHelp() {};
    
    //帮助类初始化
    void init(const std::string& appid, const std::string& key, const char* uniqueid) {
        initCallBack();
        RTChatSDKMain::sharedInstance().initSDK(appid, key, uniqueid);
    };
    
    //初始化回调函数
    void initCallBack() {
        RTChatSDKMain::sharedInstance().registerMsgCallback(std::bind(&RTChatSdkHelp::RTChatCallBack, this, _1, _2, _3, _4));
    };
    
    //语音引擎回调函数
    void RTChatCallBack(SdkResponseCmd cmdType, SdkErrorCode error = OPERATION_OK, const unsigned char* dataPtr = NULL, uint32_t dataSize = 0){
        [[RTChatSdkIosHelper sharedInstance] CallBack:cmdType error:error dataptr:dataPtr dataSize:dataSize];
    };
};

@implementation CallBackObject

@end

@implementation RTChatSdkIosHelper

+(RTChatSdkIosHelper *)sharedInstance
{
    static RTChatSdkIosHelper *sharedInstance = nil;
    
    static dispatch_once_t predicate;
    dispatch_once(&predicate, ^{
        sharedInstance = [[self alloc] init];
    });
    
    return sharedInstance;
}

-(id)init
{
    id ret = [super init];
    if (!ret) {
        return nil;
    }
    
    if (!rtchatsdkhelper) {
        rtchatsdkhelper = new RTChatSdkHelp;
    }
    
    return ret;
}

-(void)initHelper:(NSString *)appid key:(NSString *)key uniqueid:(NSString *)uniqueid
{
    if (rtchatsdkhelper) {
        rtchatsdkhelper->init([appid UTF8String], [key UTF8String], [uniqueid UTF8String]);
    }
}

-(void) activateSDK
{
    RTChatSDKMain::sharedInstance().activateSDK();
}

-(void) deActivateSDK
{
    RTChatSDKMain::sharedInstance().deActivateSDK();
}

/// 申请房间列表(主线程)
-(SdkErrorCode) requestRoomList
{
    return RTChatSDKMain::sharedInstance().requestRoomList();
}

/// 创建房间(主线程)
-(SdkErrorCode) createRoom:(enRoomType)roomType reason:(enRoomReason)reason
{
    return RTChatSDKMain::sharedInstance().createRoom(roomType, reason);
}

/// 加入房间(主线程)
-(SdkErrorCode) joinRoom:(uint64_t)roomid
{
    return RTChatSDKMain::sharedInstance().joinRoom(roomid);
}

/// 离开房间(主线程)
-(SdkErrorCode) leaveRoom
{
    return RTChatSDKMain::sharedInstance().leaveRoom();
}

/// 加入麦序(主线程)
-(SdkErrorCode) requestInsertMicQueue
{
    return RTChatSDKMain::sharedInstance().requestInsertMicQueue();
}

/// 离开麦序(主线程)
-(SdkErrorCode) leaveMicQueue
{
    return RTChatSDKMain::sharedInstance().leaveMicQueue();
}

///是否接收随机聊天，临时增加的接口(主线程)
-(void) returnRandChatRes:(bool)isAccept srctempid:(uint64_t)srctempid
{
    RTChatSDKMain::sharedInstance().returnRandChatRes(isAccept, srctempid);
}

/// 设置本人Mac静音(主线程)
-(void) setMuteSelf:(bool)isMute
{
    return RTChatSDKMain::sharedInstance().setMuteSelf(isMute);
}

/// 开始录制麦克风数据(主线程)
-(bool) startRecordVoice;
{
    return RTChatSDKMain::sharedInstance().startRecordVoice();
}

/// 停止录制麦克风数据(主线程)
-(bool) stopRecordVoice
{
    return RTChatSDKMain::sharedInstance().stopRecordVoice();
}

/// 开始播放录制数据(主线程)
-(bool) startPlayLocalVoice:(const char*)voiceUrl
{
    return RTChatSDKMain::sharedInstance().startPlayLocalVoice(voiceUrl);
}

/// 停止播放数据(主线程)
-(bool) stopPlayLocalVoice
{
    return RTChatSDKMain::sharedInstance().stopPlayLocalVoice();
}

/// 请求更改排麦房权限(主线程)
-(bool) requestUpdatePower:(uint64_t)othertempid powertype:(enPowerType)powertype
{
    return RTChatSDKMain::sharedInstance().requestUpdatePower(othertempid, powertype);
}

/// 分配麦
-(bool) requestAssignMic:(uint64_t) othertempid
{
    return RTChatSDKMain::sharedInstance().requestAssignMic(othertempid);
}

-(void)CallBack:(SdkResponseCmd)cmdType error:(SdkErrorCode)error dataptr:(const unsigned char*)dataPtr dataSize:(uint32_t)dataSize
{
    CallBackObject* object = [[CallBackObject alloc]init];
    NSData* data = [NSData dataWithBytes:dataPtr length:dataSize];
    [object setCmdType:cmdType];
    [object setError:error];
    [object setData:data];
    
    [self performSelectorOnMainThread:@selector(onRTChatMsg:) withObject:object waitUntilDone:YES];
}

-(void)onRTChatMsg:(id)object
{
    [[self delegate] onRecvRTChatSDkMsg:object];
}

@end






