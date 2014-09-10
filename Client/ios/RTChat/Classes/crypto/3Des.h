//
//  3Des.h
//  test
//
//  Created by asiainfo-linkage on 11-3-28.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CommonCrypto/CommonCryptor.h>

@interface _Des : NSObject {

}

+(NSString*)TripleDES:(NSString*)plainText encryptOrDecrypt:(CCOperation)encryptOrDecrypt key:(NSString*)key;

+(NSString*)TripleDES:(NSString*)plainText encryptOrDecrypt:(CCOperation)encryptOrDecrypt key:(NSString*)key andIV:(NSString*)initVec;

+(NSString*)DES:(NSString*)aPlainText encryptOrDecrypt:(CCOperation)encryptOrDecrypt key:(NSString*)aKey;
+(NSData*)DES:(NSData*)inputData encryptOrDecrypt:(CCOperation)encryptOrDecrypt key:(NSString*)aKey needBase64:(BOOL)needBase64;

+(NSString*)HexStringOfData:(NSString*)data encryptedWithKey:(NSString*)key usePadding:(BOOL)usePadding;
+(NSString*)DataOfHexString:(NSString*)hexString decryptedWithKey:(NSString*)key;


@end

