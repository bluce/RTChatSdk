//
//  BridgeTools.cpp
//  RTChat
//
//  Created by raymon_wang on 14-9-9.
//  Copyright (c) 2014年 yunwei. All rights reserved.
//

#include "BridgeTools.h"
#include "crypto/3Des.h"
#include "defines.h"

BridgeTools::BridgeTools()
{
    
}

BridgeTools::~BridgeTools()
{
    
}

std::string BridgeTools::des(const char* ptr, int datasize)
{
    NSString* result = [_Des DES:[NSString stringWithUTF8String:ptr] encryptOrDecrypt:kCCEncrypt key:@CryptoKey];
    if (result) {
        return [result UTF8String];
    }
    return "";
}

bool BridgeTools::des(const unsigned char *ptr, int datasize, unsigned char *outptr, int &outsize, bool encORdec)
{
    NSData* result = [_Des DES:[NSData dataWithBytes:ptr length:datasize] encryptOrDecrypt:(encORdec?kCCEncrypt:kCCDecrypt) key:@CryptoKey needBase64:NO];
    if (result) {
        memcpy(outptr, [result bytes], [result length]);
        outsize = [result length];
        return true;
    }
    
    return false;
}



