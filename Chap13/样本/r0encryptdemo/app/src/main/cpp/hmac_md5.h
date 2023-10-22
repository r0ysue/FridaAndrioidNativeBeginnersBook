//
// Created by Lenovo on 2021-04-11.
//

#ifndef ENCRYPTDEMO_HMAC_MD5_H
#define ENCRYPTDEMO_HMAC_MD5_H


typedef unsigned char md5_digest_t[16];

#ifdef __cplusplus
extern "C" {
void hmac_md5(
        unsigned char*  text,                /* pointer to data stream */
        int             text_len,            /* length of data stream */
        unsigned char*  key,                 /* pointer to authentication key */
        int             key_len,             /* length of authentication key */
        md5_digest_t    digest);            /* caller digest to be filled in */
}
#endif

#endif //ENCRYPTDEMO_HMAC_MD5_H
