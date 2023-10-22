//
// Created by ting on 2019-09-17.
//

#ifndef AESTEST_HEX_UTILS_H
#define AESTEST_HEX_UTILS_H


#define hex_encode     bbddbbdbb
#define hex_decode     bdddbddbb
#define hex_hex2Char   bbbddbdbb


#ifdef __cplusplus
extern "C" {
#endif


char *hex_encode(const unsigned char *input, size_t len);
unsigned char *hex_decode(const char *hex);


#ifdef __cplusplus
}
#endif

#endif //AESTEST_HEX_UTILS_H
