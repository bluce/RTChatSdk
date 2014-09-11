//
//  BridgeTools.h
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-9-9.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#ifndef __RTChat__BridgeTools__
#define __RTChat__BridgeTools__

#include <iostream>

class BridgeToolsImpl;

class BridgeTools {
public:
    BridgeTools();
    virtual ~BridgeTools();
    
    static std::string des(const char* ptr, int datasize);
    
    static bool des(const unsigned char* ptr, int datasize, unsigned char* outptr, int& outsize, bool encORdec/* 加密吗 */);
    
    static std::string getWritablePath();
};

#endif /* defined(__RTChat__BridgeTools__) */
