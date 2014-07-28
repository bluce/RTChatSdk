//
//  command.h
//  RTChat
//
//  Created by raymon_wang on 14-7-28.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef RTChat_command_h
#define RTChat_command_h

#include "../Engine/Engine.h"

struct stBaseCmd
{
    WORD cmdid;
    DWORD cmdlen;
	char data[0];
    DWORD getSize() const {return sizeof(stBaseCmd)+ sizeof(char)*cmdlen;}
};

#endif
