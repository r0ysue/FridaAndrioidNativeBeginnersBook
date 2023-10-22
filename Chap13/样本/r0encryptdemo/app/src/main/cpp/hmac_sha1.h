//
// Created by pr0214 on 2021/4/27.
//

#ifndef R0ENCRYPTDEMO_HMAC_SHA1_H
#define R0ENCRYPTDEMO_HMAC_SHA1_H

typedef unsigned char sha1_digest_t[20];

#ifdef __cplusplus
extern "C" {
void hmac_sha1(
        unsigned char*  text,                /* pointer to data stream */
        int             text_len,            /* length of data stream */
        unsigned char*  key,                 /* pointer to authentication key */
        int             key_len,             /* length of authentication key */
        sha1_digest_t   digest);            /* caller digest to be filled in */
}
#endif

#endif //R0ENCRYPTDEMO_HMAC_SHA1_H
