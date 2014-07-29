//
//  RTChatSDKMain.cpp
//  RTChat
//
//  Created by raymon_wang on 14-7-29.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "RTChatSDKMain.h"
#include "netdatamanager.h"
#include "public.pb.h"
#include "NetProcess/command.h"

static  RTChatSDKMain* s_RTChatSDKMain = NULL;

RTChatSDKMain::RTChatSDKMain() :
_netDataManager(NULL)
{
    _netDataManager = new NetDataManager;
}

RTChatSDKMain::~RTChatSDKMain()
{
    
}

const RTChatSDKMain& RTChatSDKMain::sharedInstance()
{
    if (!s_RTChatSDKMain) {
        s_RTChatSDKMain = new RTChatSDKMain();
    }
    
    return *s_RTChatSDKMain;
}

void RTChatSDKMain::initSDK(const std::string &uniqueid)
{
    _uniqueid = uniqueid;
    
    if (_netDataManager) {
        _netDataManager->init("ws://180.168.126.249:16008");
    }
}

void RTChatSDKMain::createRoom()
{
    stBaseCmd cmd;
    cmd.cmdid = Cmd::enRequestCreateRoom;
    
    if (_netDataManager) {
        _netDataManager->sendClientMsg((const unsigned char*)&cmd, cmd.getSize());
    }
}




