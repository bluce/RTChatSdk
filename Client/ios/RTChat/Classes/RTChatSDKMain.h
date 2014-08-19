//
//  RTChatSDKMain.h
//  RTChat
//
//  Created by raymon_wang on 14-7-29.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RTChat__RTChatSDKMain__
#define __RTChat__RTChatSDKMain__

#include <iostream>
#include <map>
#include "proto/public.pb.h"
#include "proto/game.pb.h"
#include "RTChatCommonTypes.h"

class NetDataManager;
class MediaSample;

typedef std::function<void (SdkResponseCmd cmdType, SdkErrorCode error, const unsigned char* dataPtr, uint32_t dataSize)> pMsgCallFunc;

class RTChatSDKMain {
public:
    typedef std::map<int64_t, Cmd::stRoomInfo> RoomInfoMap;
    
public:
    RTChatSDKMain();
    virtual ~RTChatSDKMain();
    
    static RTChatSDKMain& sharedInstance();
    
    /*******************需要暴露给用户的接口开始**********************/
    
    //sdk初始化，只能调用一次
    void initSDK(const std::string& appid, const std::string& key, const char* uniqueid = NULL);
    
    //注册消息回调
    void registerMsgCallback(const pMsgCallFunc& func);
    
    //激活SDK
    void activateSDK();
    
    //当应用最小化时需要调用这个，清理数据
    void deActivateSDK();
    
    //获取SDK当前操作状态，用户发起操作前可以检测一下状态判断可否继续
    SdkOpState getSdkState();
    
    //请求登录
    void requestLogin(const char* uniqueid = NULL);
    
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
    
    /*******************需要暴露给用户的接口结束**********************/
    
    //请求逻辑服务器地址
    void requestLogicServerInfo(const std::string& appid, const std::string& key);
    
    //收到网络线程消息
    void onRecvMsg(char* data, int len);
    
    //获取当前的输入mic静音状态
    bool getMuteSelf();
    
    //设置本人Mac静音
    void setMuteSelf(bool isMute);
    
    //注意。这个接口不应该暴露给上层应用使用
    void set_SdkOpState(SdkOpState state) {
        pthread_mutex_lock(&_mutexLock);
        _sdkOpState = state;
        pthread_mutex_unlock(&_mutexLock);
    };
    
    uint64_t get_sdkTempID() {
        return _sdkTempID;
    };
    
protected:
    void startTalk();
    
    void stopTalk();
    
    //语音引擎连接语音房间
    void connectVoiceRoom(const std::string& ip, unsigned int port);
    
    //随机进入一个房间
    void randomJoinRoom();
    
private:
    //刷新房间列表信息
    void refreshRoomInfoMap(const Cmd::cmdNotifyRoomList& protomsg);
    
    //初始化互斥锁
    void initMutexLock();
    
private:
    NetDataManager*     _netDataManager;    //数据管理器
    MediaSample*        _mediaSample;       //音频管理器
    
private:
    std::string         _appid;
    std::string         _appkey;
    std::string         _uniqueid;
    std::string         _token;
    std::string         _logicIP;       //逻辑服务器IP
    uint32_t            _logicPort;     //逻辑服务器PORT
    
    uint64_t            _sdkTempID;     //服务器下发的sdk唯一标识符
    
    uint64_t            _currentRoomID; //当前进入的房间ID
    
    bool                _isMute;
    
    RoomInfoMap         _roomInfoMap;   //保存房间列表
    
    SdkOpState          _sdkOpState;    //保存SDK操作状态
    
    pthread_mutex_t     _mutexLock;      //读写锁
    
    pMsgCallFunc        _func;          //回调函数
};

#endif /* defined(__RTChat__RTChatSDKMain__) */
