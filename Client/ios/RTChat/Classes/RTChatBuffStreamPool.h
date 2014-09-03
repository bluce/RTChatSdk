//
//  RTChatBuffStreamPool.h
//  RTChat
//
//  Created by raymon_wang on 14-9-1.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RTChat__RTChatBuffStreamPool__
#define __RTChat__RTChatBuffStreamPool__

#include <iostream>
#include "RTChatBuffStream.h"
#include "defines.h"

class RTChatBuffStreamPool {
public:
    struct StBuffInfo{
        StBuffInfo(int maxbuffsize){
            bzero(buffname, sizeof(buffname));
            buffstream = new RTChatBuffStream(maxbuffsize);
        }
        ~StBuffInfo(){
            SAFE_DELETE(buffstream);
        }
        char buffname[64];
        RTChatBuffStream* buffstream;
        bool needDownload;
    };
    
    typedef std::vector<StBuffInfo*>  PoolVec;
    
public:
    RTChatBuffStreamPool(int poolsize);
    virtual ~RTChatBuffStreamPool();
    
    void init();
    
    StBuffInfo* getAvailableBuff(const char* name);
    
    //更新当前位置的缓冲区内存
    RTChatBuffStream* updateCurrentBuff(const char* name, const char* ptr, int size);
    
    void clear_data();
    
protected:
    int haveTheBuffByName(const char* name);
    
private:
    PoolVec     _poolVec;
    int         _currentwriteindex;
    int         _poolsize;
};

#endif /* defined(__RTChat__RTChatBuffStreamPool__) */
