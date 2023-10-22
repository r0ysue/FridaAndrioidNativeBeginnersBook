//
// Created by Lenovo on 2021-04-11.
//

#include "hmac_md5.h"
#include "md5.h"
#include <strings.h>

/*** Function: hmac_md5
*/

void
hmac_md5(text, text_len, key, key_len, digest)
        unsigned char*  text;                /* pointer to data stream */
        int             text_len;            /* length of data stream */
        unsigned char*  key;                 /* pointer to authentication key */
        int             key_len;             /* length of authentication key */
        md5_digest_t    digest;              /* caller digest to be filled in */

{
    MD5_CTX context;
    unsigned char k_ipad[65];    /* inner padding -
                                      * key XORd with ipad
                                      */
    unsigned char k_opad[65];    /* outer padding -
                                      * key XORd with opad
                                      */
    unsigned char tk[16];
    int i;

    /* if key is longer than 64 bytes reset it to key=MD5(key) */
    // 检查密钥K的长度。如果K的长度大于B则先使用摘要算法计算出一个长度为L的新密钥
    if (key_len > 64) {

        MD5_CTX      tctx;

        MD5Init(&tctx);
        MD5Update(&tctx, key, key_len);
        MD5Final(tk, &tctx);

        key = tk;
        key_len = 16;
    }

    /*
     * the HMAC_MD5 transform looks like:
     *
     * MD5(K XOR opad, MD5(K XOR ipad, text))
     *
     * where K is an n byte key
     * ipad is the byte 0x36 repeated 64 times
     * opad is the byte 0x5c repeated 64 times
     * and text is the data being protected
     */

    /* start out by storing key in pads */
    // 将分配的这块内存清零
    bzero( k_ipad, sizeof k_ipad);
    bzero( k_opad, sizeof k_opad);
    // 如果长度小于B，则在其后面追加0来使其长度达到B。
    bcopy( key, k_ipad, key_len);
    bcopy( key, k_opad, key_len);

    /* XOR key with ipad and opad values */
    // 这边没有额外搞一个ipad或者opad数组，而是直接异或完事，也挺好的。
    for (i=0; i<64; i++) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }
    /*
     * perform inner MD5
     */
    // H（(K⊕ipad) + M）k_ipad 就是 k⊕ipad
    MD5Init(&context);                   /* init context for 1st
                                              * pass */
    MD5Update(&context, k_ipad, 64);     /* start with inner pad */
    MD5Update(&context, text, text_len); /* then text of datagram */
    MD5Final(digest, &context);          /* finish up 1st pass */
    /*
     * perform outer MD5
     */
    // HMAC（K，M）=H（K⊕opad + H（K⊕ipad + M））
    MD5Init(&context);                   /* init context for 2nd
                                              * pass */
    MD5Update(&context, k_opad, 64);     /* start with outer pad */
    MD5Update(&context, digest, 16);     /* then results of 1st
                                              * hash */
    MD5Final(digest, &context);          /* finish up 2nd pass */
}