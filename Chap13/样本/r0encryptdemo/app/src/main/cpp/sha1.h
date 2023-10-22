//
// Created by Lenovo on 2021-04-11.
//

#ifndef ENCRYPTDEMO_SHA1_H
#define ENCRYPTDEMO_SHA1_H

//
// Created by Lenovo on 2021-04-11.
//

#ifndef MY_SHA1_SHA1_H
#define MY_SHA1_SHA1_H

#define SHA_DIGEST_LENGTH 20
#define INIT_DATA_h0 0x67452301UL
#define INIT_DATA_h1 0xefcdab89UL
#define INIT_DATA_h2 0x98badcfeUL
#define INIT_DATA_h3 0x10325476UL
#define INIT_DATA_h4 0xc3d2e1f0UL

#define SHA_LONG unsigned long
#define SHA_LBLOCK	16
#define SHA_CBLOCK	(SHA_LBLOCK*4)

#define DATA_ORDER_IS_BIG_ENDIAN

typedef struct SHAstate_st{
    SHA_LONG h0,h1,h2,h3,h4;
    SHA_LONG Nl,Nh;
    SHA_LONG data[SHA_LBLOCK];
    unsigned int num;
}SHA_CTX;

#ifdef __cplusplus
extern "C" {
#endif
int SHA_Init(SHA_CTX *c);
int SHA_Update(SHA_CTX *c, const void *data_, unsigned int len);
int SHA_Final(unsigned char *md, SHA_CTX *c);
#ifdef __cplusplus
}
#endif //MY_SHA1_SHA1_H
#endif //ENCRYPTDEMO_SHA1_H
#endif
