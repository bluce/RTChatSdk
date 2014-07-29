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

class RTChatSDKMain {
public:
    RTChatSDKMain();
    virtual ~RTChatSDKMain();
    
    static const RTChatSDKMain& sharedInstance();
    
    void initSDK(const std::string& uniqueid);
    
    void createRoom();
    
private:
    std::string         _uniqueid;
    NetDataManager*     _netDataManager;
};

#endif /* defined(__RTChat__RTChatSDKMain__) */
