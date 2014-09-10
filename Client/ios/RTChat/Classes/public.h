//
//  public.h
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-8-14.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#ifndef __RTChat__public__
#define __RTChat__public__

#include <iostream>

class Public {
public:
    static std::string SdkAvar(const char *pszFmt,...);
    
    static void sdklog(const char *pszFmt,...);
};

#endif /* defined(__RTChat__public__) */
