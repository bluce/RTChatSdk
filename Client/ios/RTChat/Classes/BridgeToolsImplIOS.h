//
//  BridgeImplIOS.h
//  flappyball
//
//  Created by wang3140@hotmail.com on 14-6-19.
//
//

#ifndef __flappyball__BridgeImplIOS__
#define __flappyball__BridgeImplIOS__

#ifdef WEBRTC_IOS

#include "BridgeToolsImpl.h"
#include <iostream>

class BridgeToolsImplIOS : public BridgeToolsImpl {
public:
    BridgeToolsImplIOS();
    virtual ~BridgeToolsImplIOS();
    
    virtual std::string des(const char* ptr, int datasize);
    
    virtual bool des(const unsigned char* ptr, int datasize, unsigned char* outptr, int& outsize, bool encORdec/* 加密吗 */);
    
    virtual std::string getWritablePath();
};

#endif /* #ifdef WEBRTC_IOS */

#endif
