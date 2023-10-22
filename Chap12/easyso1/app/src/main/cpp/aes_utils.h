//
// Created by ting on 2019-09-17.
//

#ifndef AESTEST_AES_UTILS_H
#define AESTEST_AES_UTILS_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "aes.h"


#define AES_128_CBC_PKCS5_Encrypt  ll11l1l1ll
#define AES_128_CBC_PKCS5_Decrypt  ll11lll11l
#define getKey                     ll11lll1l1
#define getIV                      ll11l1l1l1
#define getPaddingInput            ll11l1l11l
#define findPaddingIndex           lll1l1l1l1
#define removePadding              ll11l1llll


#ifdef __cplusplus
extern "C" {
#endif


/** AES加密, CBC, PKCS5Padding */
char *AES_128_CBC_PKCS5_Encrypt(const char *input);

/** AES解密, CBC, PKCS5Padding */
char *AES_128_CBC_PKCS5_Decrypt(const char *input);


#ifdef __cplusplus
}
#endif


#endif //AESTEST_AES_UTILS_H
