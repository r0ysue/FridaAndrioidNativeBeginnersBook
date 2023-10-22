//
// Created by Lenovo on 2021-04-11.
//

#ifndef MY_SHA2_SHA256_H
#define MY_SHA2_SHA256_H
#ifdef _MSC_VER
#ifndef uint8_t
typedef unsigned __int8 uint8_t;
#endif
#ifndef uint32_t
typedef unsigned __int32 uint32_t;
#endif
#ifndef uint64_t
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#endif
#else
#include <stdint.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct {
    uint32_t buf[16];
    uint32_t hash[8];
    uint32_t len[2];
} sha256_context;

void sha256_init(sha256_context *);
void sha256_update(sha256_context *, uint8_t * /* data */, uint32_t /* len */);
void sha256_final(sha256_context *, uint8_t * /* hash */);

#ifdef __cplusplus
}
#endif

#endif //MY_SHA2_SHA256_H
