//
//  RTCoder.h
//  RTChat
//
//  Created by raymon_wang on 14-7-11.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#ifndef __RTChat__RTCoder__
#define __RTChat__RTCoder__

#include <iostream>

class RTCoder {
public:
    RTCoder();
    virtual ~RTCoder();
    
    bool initCoder(short mode);
    
    //编码数据
    void enCodeData(const unsigned char* buff, int buffSize, char* enBuff);
    
    //解码数据
    void deCodeData();
    
private:
    iLBC_encinst_t*     _enc_Inst;
    iLBC_decinst_t*     _dec_Inst;
    int                 _frameLen;
};

#endif /* defined(__RTChat__RTCoder__) */
