//
//  BridgeImpl.h
//  flappyball
//
//  Created by wang3140@hotmail.com on 14-6-19.
//
//

#ifndef flappyball_BridgeImpl_h
#define flappyball_BridgeImpl_h

#include <iostream>
#include "BridgeTools.h"

class BridgeToolsImpl {
public:
    BridgeToolsImpl() {} ;
    virtual ~BridgeToolsImpl() {};
    
    virtual std::string des(const char* ptr, int datasize) = 0;
    
    virtual bool des(const unsigned char* ptr, int datasize, unsigned char* outptr, int& outsize, bool encORdec/* 加密吗 */) = 0;
    
    virtual std::string getWritablePath() = 0;
    
    virtual float getOperationSystemVersion() = 0;
};

extern BridgeToolsImpl* getBridgeToolsImpl();

#endif
