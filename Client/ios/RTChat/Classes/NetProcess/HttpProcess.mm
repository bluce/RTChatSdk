//
//  HttpProcess.cpp
//  RTChat
//
//  Created by wang3140@hotmail.com on 14-9-2.
//  Copyright (c) 2014年 RTChatTeam. All rights reserved.
//

#include "HttpProcess.h"
#include "CmdHandler.h"
#include "../public.h"

namespace rtchatsdk {

static  HttpProcess* s_HttpProcess = NULL;

HttpProcess::HttpProcess()
{
    
}

HttpProcess::~HttpProcess()
{
    
}

HttpProcess& HttpProcess::instance()
{
    if (!s_HttpProcess) {
        s_HttpProcess = new HttpProcess();
    }

    return *s_HttpProcess;
}

void HttpProcess::registerCallBack(const CallBackFunc &func)
{
    Public::sdklog("    注册http请求回调函数");
    _func = func;
}

void HttpProcess::postContent(const char *urlstr, const unsigned char *data, int datasize, std::map<const char*, const char*>& params, bool needcallback)
{
    Public::sdklog("    in httppostcontent, 上传数据");
    _isrunning = true;
    
    NSString* url = [NSString stringWithFormat:@"%s", urlstr];
    NSData* content = [NSData dataWithBytes:data length:datasize];
    
    NSMutableDictionary* dict = nil;
    if (params.size() > 0) {
        dict = [[NSMutableDictionary alloc]init];
        for (auto it = params.begin(); it != params.end(); ++it) {
            [dict setObject:[NSString stringWithUTF8String:it->second] forKey:[NSString stringWithUTF8String:it->first]];
        }
    }
    
    [[CmdHandler sharedInstance]postFile:url reqParams:dict data:content completBlock:^(id res) {
        if (res == nil) {
            //上传失败
            if (needcallback) {
                _func(HttpProcess_Upload, NULL, 0);
            }
        }
        else {
            //上传成功
            if (needcallback) {
                NSString* string = res;
                _func(HttpProcess_Upload, [string UTF8String], [string length]);
            }
        }
        _isrunning = false;
    }];
}

void HttpProcess::requestContent(const char *urlstr)
{
    _isrunning = true;
    
    NSString* url = [NSString stringWithUTF8String:urlstr];
    [[CmdHandler sharedInstance] getFile:url reqParams:nil completBlock:^(id res) {
        NSData* data = (NSData*)res;
        if (res == nil) {
            //下载失败
            _func(HttpProcess_DownLoad, NULL, 0);
        }
        else {
            //下载成功
            _func(HttpProcess_DownLoad, (const char*)[data bytes], [data length]);
        }
        _isrunning = false;
    }];
}
    
}




