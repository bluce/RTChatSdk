//
//  BridgeTools.h
//  RTChat
//
//  Created by raymon_wang on 14-9-9.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RTChat__BridgeTools__
#define __RTChat__BridgeTools__

#include <iostream>

class BridgeTools {
public:
    BridgeTools();
    virtual ~BridgeTools();
    
    static std::string des(const char* ptr, int datasize);
    
    static bool des(const unsigned char* ptr, int datasize, unsigned char* outptr, int& outsize, bool encORdec/* 加密吗 */);
};

#endif /* defined(__RTChat__BridgeTools__) */
