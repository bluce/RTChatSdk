//
//  command.h
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-7-28.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#ifndef RTChat_command_h
#define RTChat_command_h

#include "../defines.h"

#pragma pack(push, 1)

namespace rtchatsdk {

struct stBaseCmd
{
    stBaseCmd() {
        cmdid = 0;
        cmdlen = 0;
    }
    WORD cmdid;
    DWORD cmdlen;
	char data[0];
    DWORD getSize() const {return sizeof(stBaseCmd)+ sizeof(char)*cmdlen;}
};

struct stHelloCmd
{
    stHelloCmd() {
        cmdid = 0;
    }
    WORD cmdid;
};
    
}

#pragma pack(pop)

#endif
