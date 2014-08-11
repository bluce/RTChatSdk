//
//  command.h
//  RTChat
//
//  Created by raymon_wang on 14-7-28.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef RTChat_command_h
#define RTChat_command_h

#include "../SdkPublic.h"

#pragma pack(push, 1)

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

#pragma pack(pop)

#endif
