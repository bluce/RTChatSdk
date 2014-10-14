//
//  BridgeTools.cpp
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-9-9.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#include "BridgeTools.h"
#include "crypto/3Des.h"
#include "defines.h"
#include "BridgeToolsImpl.h"

BridgeTools::BridgeTools()
{
    
}

BridgeTools::~BridgeTools()
{
    
}

std::string BridgeTools::des(const char* ptr, int datasize)
{
    return getBridgeToolsImpl()->des(ptr, datasize);
}

bool BridgeTools::des(const unsigned char *ptr, int datasize, unsigned char *outptr, int &outsize, bool encORdec)
{
    return getBridgeToolsImpl()->des(ptr, datasize, outptr, outsize, encORdec);
}

float BridgeTools::getOperationSystemVersion()
{
    return getBridgeToolsImpl()->getOperationSystemVersion();
}

std::string BridgeTools::getWritablePath()
{
    return getBridgeToolsImpl()->getWritablePath();
}



