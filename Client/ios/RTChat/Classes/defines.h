//
//  SdkPublic.h
//  RTChat
//
//  Created by raymon_wang on 14-8-5.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef RTChat_SdkPublic_h
#define RTChat_SdkPublic_h


#define SAFE_DELETE(p) if(p) {delete p; p = NULL;}

#define SAFE_DELETEARRAY(p) if(p) {delete [] p; p = NULL;}

#define MAX_BUFFER_SIZE 1024

typedef unsigned int DWORD;
typedef unsigned short WORD;

template <class T>
inline void constructDynamic(T *ptr) {
    new ((void*)ptr) T();
};

#define BUFFER_CMD(cmd, name, len) char buffer##name[len]; \
    cmd* name = (cmd*)buffer##name; \
    constructDynamic(name); \

#define VoiceUpLoadUrlHead  "http://uploadchat.ztgame.com.cn:10000/wangpan.php"

//#define ControlServerAddr   "rtchat.ztgame.com.cn:18000"
#define ControlServerAddr   "180.168.126.249:18000"


#endif
