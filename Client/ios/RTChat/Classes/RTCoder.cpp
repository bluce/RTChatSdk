//
//  RTCoder.cpp
//  RTChat
//
//  Created by raymon_wang on 14-7-11.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "RTCoder.h"

#define BLOCKL_MAX			240
#define ILBCNOOFWORDS_MAX	25

RTCoder::RTCoder() :
_enc_Inst(NULL),
_dec_Inst(NULL),
_frameLen(0)
{
    WebRtcIlbcfix_EncoderCreate(&_enc_Inst);
    WebRtcIlbcfix_DecoderCreate(&_dec_Inst);
}

RTCoder::~RTCoder()
{
    WebRtcIlbcfix_EncoderFree(_enc_Inst);
    WebRtcIlbcfix_DecoderFree(_dec_Inst);
}

bool RTCoder::initCoder(short mode)
{
    if (!_enc_Inst) {
        return false;
    }
    
    if( !WebRtcIlbcfix_EncoderInit(_enc_Inst, mode) ) {
        return false;
    }
    
    if (!WebRtcIlbcfix_DecoderInit(_dec_Inst, mode)) {
        return false;
    }
    
    _frameLen = mode * 8;
    
    return true;
}

//编码数据
void RTCoder::enCodeData(const unsigned char *buff, int buffSize, char* enBuff)
{
    int16_t len = 0;
    int16_t data[BLOCKL_MAX];
    int16_t encoded_data[ILBCNOOFWORDS_MAX];
    int leftLength = buffSize;
    int sumEncoded = 0;
    
    if (!_enc_Inst) {
        return;
    }
    
    while (leftLength > 0) {
        len = WebRtcIlbcfix_Encode(_enc_Inst, data, (int16_t)_frameLen, encoded_data);
        bcopy(encoded_data, enBuff+sumEncoded, len);
        sumEncoded += len;
    }
}

//解码数据
void RTCoder::deCodeData()
{
    
}








