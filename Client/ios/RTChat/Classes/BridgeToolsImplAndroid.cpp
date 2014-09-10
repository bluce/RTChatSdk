//
//  BridgeImplAndroid.cpp
//  flappyball
//
//  Created by wang3140@hotmail.com on 14-6-19.
//
//

#include "BridgeToolsImplAndroid.h"

#ifdef WEBRTC_ANDROID
#include "../libs/cocos2dx/platform/android/jni/JniHelper.h"
#include <android/log.h>

using namespace cocos2d;

BridgeToolsImplAndroid::BridgeToolsImplAndroid()
{
    
}

BridgeToolsImplAndroid::~BridgeToolsImplAndroid()
{
    
}

std::string BridgeToolsImplAndroid::des(const char* ptr, int datasize)
{
    return "";
}

bool BridgeToolsImplAndroid::des(const unsigned char* ptr, int datasize, unsigned char* outptr, int& outsize, bool encORdec/* 加密吗 */)
{
    return false;
}

BridgeToolsImpl* getBridgeToolsImpl()
{
    static BridgeToolsImplAndroid s_bridgeToolsImplAndroid;
    return &s_bridgeToolsImplAndroid;
}

#endif /* #ifdef WEBRTC_ANDROID */



