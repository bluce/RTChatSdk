//
//  public.h
//  RTChat
//
//  Created by raymon_wang on 14-8-14.
//  Copyright (c) 2014年 yunwei. All rights reserved.
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
