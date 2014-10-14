//
//  BridgeToolsImplIOS.m
//  flappyball
//
//  Created by wang3140@hotmail.com on 14-6-19.
//
//

#include "BridgeToolsImplIOS.h"
#import <UIKit/UIKit.h>

#ifdef WEBRTC_IOS
#include "crypto/3Des.h"
#include "defines.h"

BridgeToolsImplIOS::BridgeToolsImplIOS()
{
    
}

BridgeToolsImplIOS::~BridgeToolsImplIOS()
{
    
}

std::string BridgeToolsImplIOS::des(const char* ptr, int datasize)
{
    NSString* result = [_Des DES:[NSString stringWithUTF8String:ptr] encryptOrDecrypt:kCCEncrypt key:@CryptoKey];
    if (result) {
        return [result UTF8String];
    }
    return "";
}

bool BridgeToolsImplIOS::des(const unsigned char* ptr, int datasize, unsigned char* outptr, int& outsize, bool encORdec/* 加密吗 */)
{
    NSData* result = [_Des DES:[NSData dataWithBytes:ptr length:datasize] encryptOrDecrypt:(encORdec?kCCEncrypt:kCCDecrypt) key:@CryptoKey needBase64:NO];
    if (result) {
        memcpy(outptr, [result bytes], [result length]);
        outsize = [result length];
        return true;
    }
    
    return false;
}

std::string BridgeToolsImplIOS::getWritablePath()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    return strRet;
}

float BridgeToolsImplIOS::getOperationSystemVersion()
{
    return [[[UIDevice currentDevice] systemVersion] floatValue];
}

BridgeToolsImpl* getBridgeToolsImpl()
{
    static BridgeToolsImplIOS s_BridgeToolsImplIOS;
    return &s_BridgeToolsImplIOS;
}



#endif /* #ifdef WEBRTC_IOS */
