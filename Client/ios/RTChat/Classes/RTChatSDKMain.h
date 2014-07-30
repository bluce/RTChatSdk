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

class NetDataManager;
class MediaSample;

class RTChatSDKMain {
public:
    RTChatSDKMain();
    virtual ~RTChatSDKMain();
    
    static RTChatSDKMain& sharedInstance();
    
    void initSDK(const std::string& uniqueid);
    
    //请求登录
    void requestLogin();
    
    //创建房间
    void createRoom();
    
    //加入房间
    void joinRoom(uint64_t roomid);
    
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
    
private:
    std::string         _uniqueid;
    NetDataManager*     _netDataManager;
    MediaSample*        _mediaSample;
    uint64_t            _currentRoomID;
    bool                _isMute;
};

#endif /* defined(__RTChat__RTChatSDKMain__) */
