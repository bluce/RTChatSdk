//
//  BridgeImplAndroid.h
//  flappyball
//
//  Created by wang3140@hotmail.com on 14-6-19.
//
//

#ifndef __flappyball__BridgeImplAndroid__
#define __flappyball__BridgeImplAndroid__

#include "cocos2d.h"
#include <iostream>
#include "BridgeImpl.h"

#ifdef WEBRTC_ANDROID

class BridgeToolsImplAndroid : public BridgeToolsImpl {
public:
    BridgeToolsImplAndroid();
    virtual ~BridgeToolsImplAndroid();
    
    virtual std::string des(const char* ptr, int datasize);
    
    virtual bool des(const unsigned char* ptr, int datasize, unsigned char* outptr, int& outsize, bool encORdec/* 加密吗 */);
    
    virtual std::string getWritablePath();
    
    virtual float getOperationSystemVersion();
};

#endif /* #ifdef WEBRTC_ANDROID */

#endif /* defined(__flappyball__BridgeImplAndroid__) */
