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

class NetDataManager;
class MediaSample;

typedef std::function<void (long long roomid)> pMsgCallFunc;

class RTChatSDKMain {
public:
    typedef std::map<int64_t, Cmd::stRoomInfo> RoomInfoMap;
    
public:
    RTChatSDKMain();
    virtual ~RTChatSDKMain();
    
    static RTChatSDKMain& sharedInstance();
    
    void initSDK(const std::string& uniqueid);
    
    void registerMsgCallback(const pMsgCallFunc& func);
    
    //请求登录
    void requestLogin();
    
    //申请房间列表
    void requestRoomList();
    
    //创建房间
    void createRoom();
    
    //收到网络线程消息
    void onRecvMsg(char* data, int len);
    
    //获取当前的输入mic静音状态
    bool getMuteSelf();
    
    //设置本人Mac静音
    void setMuteSelf(bool isMute);
    
protected:
    void startTalk();
    
    void stopTalk();
    
    //语音引擎连接语音房间
    void connectVoiceRoom(const std::string& ip, unsigned int port);
    
    //加入房间
    void joinRoom(uint64_t roomid);
    
    //随机进入一个房间
    void randomJoinRoom();
    
private:
    //刷新房间列表信息
    void refreshRoomInfoMap(const Cmd::cmdNotifyRoomList& protomsg);
    
private:
    std::string         _uniqueid;
    NetDataManager*     _netDataManager;
    MediaSample*        _mediaSample;
    uint64_t            _currentRoomID;
    bool                _isMute;
    RoomInfoMap         _roomInfoMap;
    
    pMsgCallFunc        _func;
};

#endif /* defined(__RTChat__RTChatSDKMain__) */
