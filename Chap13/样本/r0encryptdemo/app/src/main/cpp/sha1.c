//
// Created by Lenovo on 2021-04-11.
//

#include <stdio.h>
#include <string.h>
#include "sha1.h"


#define HASH_MAKE_STRING(c,s)   do {	\
	unsigned long ll;		\
	ll=(c)->h0; (void)HOST_l2c(ll,(s));	\
	ll=(c)->h1; (void)HOST_l2c(ll,(s));	\
	ll=(c)->h2; (void)HOST_l2c(ll,(s));	\
	ll=(c)->h3; (void)HOST_l2c(ll,(s));	\
	ll=(c)->h4; (void)HOST_l2c(ll,(s));	\
	} while (0)


#define MD32_REG_T long
#define X(i)	XX##i

#define K_00_19	0x5a827999UL
#define K_20_39 0x6ed9eba1UL
#define K_40_59 0x8f1bbcdcUL
#define K_60_79 0xca62c1d6UL

#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))

#define HOST_c2l(c,l)	(l =(((unsigned long)(*((c)++)))<<24),		\
			 l|=(((unsigned long)(*((c)++)))<<16),		\
			 l|=(((unsigned long)(*((c)++)))<< 8),		\
			 l|=(((unsigned long)(*((c)++)))       ),	    \
			 l)
#define HOST_l2c(l,c)	(*((c)++)=(unsigned char)(((l)>>24)&0xff),	\
			 *((c)++)=(unsigned char)(((l)>>16)&0xff),	\
			 *((c)++)=(unsigned char)(((l)>> 8)&0xff),	\
			 *((c)++)=(unsigned char)(((l)    )&0xff),	\
			 l)

//			 sha0
//# define Xupdate(a,ix,ia,ib,ic,id)	(ix=(a)=(ia^ib^ic^id))
//           sha1
#  define Xupdate(a,ix,ia,ib,ic,id)	do { (a)=(ia^ib^ic^id); ix=(a)=ROTATE((a),1);} while (0)
#define	F_00_19(b,c,d)	((((c) ^ (d)) & (b)) ^ (d))
#define	F_20_39(b,c,d)	((b) ^ (c) ^ (d))
#define F_40_59(b,c,d)	(((b) & (c)) | (((b)|(c)) & (d)))
#define	F_60_79(b,c,d)	F_20_39(b,c,d)

#define BODY_00_15(i,a,b,c,d,e,f,xi) \
	(f)=xi+(e)+K_00_19+ROTATE((a),5)+F_00_19((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_16_19(i,a,b,c,d,e,f,xi,xa,xb,xc,xd) \
	Xupdate(f,xi,xa,xb,xc,xd); \
	(f)+=(e)+K_00_19+ROTATE((a),5)+F_00_19((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_20_31(i,a,b,c,d,e,f,xi,xa,xb,xc,xd) \
	Xupdate(f,xi,xa,xb,xc,xd); \
	(f)+=(e)+K_20_39+ROTATE((a),5)+F_20_39((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_32_39(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	Xupdate(f,xa,xa,xb,xc,xd); \
	(f)+=(e)+K_20_39+ROTATE((a),5)+F_20_39((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_40_59(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	Xupdate(f,xa,xa,xb,xc,xd); \
	(f)+=(e)+K_40_59+ROTATE((a),5)+F_40_59((b),(c),(d)); \
	(b)=ROTATE((b),30);

#define BODY_60_79(i,a,b,c,d,e,f,xa,xb,xc,xd) \
	Xupdate(f,xa,xa,xb,xc,xd); \
	(f)=xa+(e)+K_60_79+ROTATE((a),5)+F_60_79((b),(c),(d)); \
	(b)=ROTATE((b),30);


int SHA_Init(SHA_CTX *c){
    memset (c,0,sizeof(*c));
    c->h0=INIT_DATA_h0;
    c->h1=INIT_DATA_h1;
    c->h2=INIT_DATA_h2;
    c->h3=INIT_DATA_h3;
    c->h4=INIT_DATA_h4;
    return 1;
}


static void HASH_BLOCK_DATA_ORDER (SHA_CTX *c, const void *p, size_t num){
    const unsigned char *data=p;
    register unsigned MD32_REG_T A,B,C,D,E,T,l;
    unsigned MD32_REG_T	 XX0, XX1, XX2, XX3, XX4, XX5, XX6, XX7,XX8, XX9,XX10,XX11,XX12,XX13,XX14,XX15;

    A=c->h0;
    B=c->h1;
    C=c->h2;
    D=c->h3;
    E=c->h4;

    for (;;){
        const union { long one; char little; } is_endian = {1};

        if (!is_endian.little && sizeof(SHA_LONG)==4 && ((size_t)p%4)==0){
            const SHA_LONG *W=(const SHA_LONG *)data;

            X( 0) = W[0];				X( 1) = W[ 1];
            BODY_00_15( 0,A,B,C,D,E,T,X( 0));	X( 2) = W[ 2];
            BODY_00_15( 1,T,A,B,C,D,E,X( 1));	X( 3) = W[ 3];
            BODY_00_15( 2,E,T,A,B,C,D,X( 2));	X( 4) = W[ 4];
            BODY_00_15( 3,D,E,T,A,B,C,X( 3));	X( 5) = W[ 5];
            BODY_00_15( 4,C,D,E,T,A,B,X( 4));	X( 6) = W[ 6];
            BODY_00_15( 5,B,C,D,E,T,A,X( 5));	X( 7) = W[ 7];
            BODY_00_15( 6,A,B,C,D,E,T,X( 6));	X( 8) = W[ 8];
            BODY_00_15( 7,T,A,B,C,D,E,X( 7));	X( 9) = W[ 9];
            BODY_00_15( 8,E,T,A,B,C,D,X( 8));	X(10) = W[10];
            BODY_00_15( 9,D,E,T,A,B,C,X( 9));	X(11) = W[11];
            BODY_00_15(10,C,D,E,T,A,B,X(10));	X(12) = W[12];
            BODY_00_15(11,B,C,D,E,T,A,X(11));	X(13) = W[13];
            BODY_00_15(12,A,B,C,D,E,T,X(12));	X(14) = W[14];
            BODY_00_15(13,T,A,B,C,D,E,X(13));	X(15) = W[15];
            BODY_00_15(14,E,T,A,B,C,D,X(14));
            BODY_00_15(15,D,E,T,A,B,C,X(15));
            data += SHA_CBLOCK;
        }else{
            (void)HOST_c2l(data,l); X( 0)=l;	(void)HOST_c2l(data,l); X( 1)=l;
            BODY_00_15( 0,A,B,C,D,E,T,X( 0));	(void)HOST_c2l(data,l); X( 2)=l;
            BODY_00_15( 1,T,A,B,C,D,E,X( 1));	(void)HOST_c2l(data,l); X( 3)=l;
            BODY_00_15( 2,E,T,A,B,C,D,X( 2));	(void)HOST_c2l(data,l); X( 4)=l;
            BODY_00_15( 3,D,E,T,A,B,C,X( 3));	(void)HOST_c2l(data,l); X( 5)=l;
            BODY_00_15( 4,C,D,E,T,A,B,X( 4));	(void)HOST_c2l(data,l); X( 6)=l;
            BODY_00_15( 5,B,C,D,E,T,A,X( 5));	(void)HOST_c2l(data,l); X( 7)=l;
            BODY_00_15( 6,A,B,C,D,E,T,X( 6));	(void)HOST_c2l(data,l); X( 8)=l;
            BODY_00_15( 7,T,A,B,C,D,E,X( 7));	(void)HOST_c2l(data,l); X( 9)=l;
            BODY_00_15( 8,E,T,A,B,C,D,X( 8));	(void)HOST_c2l(data,l); X(10)=l;
            BODY_00_15( 9,D,E,T,A,B,C,X( 9));	(void)HOST_c2l(data,l); X(11)=l;
            BODY_00_15(10,C,D,E,T,A,B,X(10));	(void)HOST_c2l(data,l); X(12)=l;
            BODY_00_15(11,B,C,D,E,T,A,X(11));	(void)HOST_c2l(data,l); X(13)=l;
            BODY_00_15(12,A,B,C,D,E,T,X(12));	(void)HOST_c2l(data,l); X(14)=l;
            BODY_00_15(13,T,A,B,C,D,E,X(13));	(void)HOST_c2l(data,l); X(15)=l;
            BODY_00_15(14,E,T,A,B,C,D,X(14));
            BODY_00_15(15,D,E,T,A,B,C,X(15));
        }

        BODY_16_19(16,C,D,E,T,A,B,X( 0),X( 0),X( 2),X( 8),X(13));
        BODY_16_19(17,B,C,D,E,T,A,X( 1),X( 1),X( 3),X( 9),X(14));
        BODY_16_19(18,A,B,C,D,E,T,X( 2),X( 2),X( 4),X(10),X(15));
        BODY_16_19(19,T,A,B,C,D,E,X( 3),X( 3),X( 5),X(11),X( 0));

        BODY_20_31(20,E,T,A,B,C,D,X( 4),X( 4),X( 6),X(12),X( 1));
        BODY_20_31(21,D,E,T,A,B,C,X( 5),X( 5),X( 7),X(13),X( 2));
        BODY_20_31(22,C,D,E,T,A,B,X( 6),X( 6),X( 8),X(14),X( 3));
        BODY_20_31(23,B,C,D,E,T,A,X( 7),X( 7),X( 9),X(15),X( 4));
        BODY_20_31(24,A,B,C,D,E,T,X( 8),X( 8),X(10),X( 0),X( 5));
        BODY_20_31(25,T,A,B,C,D,E,X( 9),X( 9),X(11),X( 1),X( 6));
        BODY_20_31(26,E,T,A,B,C,D,X(10),X(10),X(12),X( 2),X( 7));
        BODY_20_31(27,D,E,T,A,B,C,X(11),X(11),X(13),X( 3),X( 8));
        BODY_20_31(28,C,D,E,T,A,B,X(12),X(12),X(14),X( 4),X( 9));
        BODY_20_31(29,B,C,D,E,T,A,X(13),X(13),X(15),X( 5),X(10));
        BODY_20_31(30,A,B,C,D,E,T,X(14),X(14),X( 0),X( 6),X(11));
        BODY_20_31(31,T,A,B,C,D,E,X(15),X(15),X( 1),X( 7),X(12));

        BODY_32_39(32,E,T,A,B,C,D,X( 0),X( 2),X( 8),X(13));
        BODY_32_39(33,D,E,T,A,B,C,X( 1),X( 3),X( 9),X(14));
        BODY_32_39(34,C,D,E,T,A,B,X( 2),X( 4),X(10),X(15));
        BODY_32_39(35,B,C,D,E,T,A,X( 3),X( 5),X(11),X( 0));
        BODY_32_39(36,A,B,C,D,E,T,X( 4),X( 6),X(12),X( 1));
        BODY_32_39(37,T,A,B,C,D,E,X( 5),X( 7),X(13),X( 2));
        BODY_32_39(38,E,T,A,B,C,D,X( 6),X( 8),X(14),X( 3));
        BODY_32_39(39,D,E,T,A,B,C,X( 7),X( 9),X(15),X( 4));

        BODY_40_59(40,C,D,E,T,A,B,X( 8),X(10),X( 0),X( 5));
        BODY_40_59(41,B,C,D,E,T,A,X( 9),X(11),X( 1),X( 6));
        BODY_40_59(42,A,B,C,D,E,T,X(10),X(12),X( 2),X( 7));
        BODY_40_59(43,T,A,B,C,D,E,X(11),X(13),X( 3),X( 8));
        BODY_40_59(44,E,T,A,B,C,D,X(12),X(14),X( 4),X( 9));
        BODY_40_59(45,D,E,T,A,B,C,X(13),X(15),X( 5),X(10));
        BODY_40_59(46,C,D,E,T,A,B,X(14),X( 0),X( 6),X(11));
        BODY_40_59(47,B,C,D,E,T,A,X(15),X( 1),X( 7),X(12));
        BODY_40_59(48,A,B,C,D,E,T,X( 0),X( 2),X( 8),X(13));
        BODY_40_59(49,T,A,B,C,D,E,X( 1),X( 3),X( 9),X(14));
        BODY_40_59(50,E,T,A,B,C,D,X( 2),X( 4),X(10),X(15));
        BODY_40_59(51,D,E,T,A,B,C,X( 3),X( 5),X(11),X( 0));
        BODY_40_59(52,C,D,E,T,A,B,X( 4),X( 6),X(12),X( 1));
        BODY_40_59(53,B,C,D,E,T,A,X( 5),X( 7),X(13),X( 2));
        BODY_40_59(54,A,B,C,D,E,T,X( 6),X( 8),X(14),X( 3));
        BODY_40_59(55,T,A,B,C,D,E,X( 7),X( 9),X(15),X( 4));
        BODY_40_59(56,E,T,A,B,C,D,X( 8),X(10),X( 0),X( 5));
        BODY_40_59(57,D,E,T,A,B,C,X( 9),X(11),X( 1),X( 6));
        BODY_40_59(58,C,D,E,T,A,B,X(10),X(12),X( 2),X( 7));
        BODY_40_59(59,B,C,D,E,T,A,X(11),X(13),X( 3),X( 8));

        BODY_60_79(60,A,B,C,D,E,T,X(12),X(14),X( 4),X( 9));
        BODY_60_79(61,T,A,B,C,D,E,X(13),X(15),X( 5),X(10));
        BODY_60_79(62,E,T,A,B,C,D,X(14),X( 0),X( 6),X(11));
        BODY_60_79(63,D,E,T,A,B,C,X(15),X( 1),X( 7),X(12));
        BODY_60_79(64,C,D,E,T,A,B,X( 0),X( 2),X( 8),X(13));
        BODY_60_79(65,B,C,D,E,T,A,X( 1),X( 3),X( 9),X(14));
        BODY_60_79(66,A,B,C,D,E,T,X( 2),X( 4),X(10),X(15));
        BODY_60_79(67,T,A,B,C,D,E,X( 3),X( 5),X(11),X( 0));
        BODY_60_79(68,E,T,A,B,C,D,X( 4),X( 6),X(12),X( 1));
        BODY_60_79(69,D,E,T,A,B,C,X( 5),X( 7),X(13),X( 2));
        BODY_60_79(70,C,D,E,T,A,B,X( 6),X( 8),X(14),X( 3));
        BODY_60_79(71,B,C,D,E,T,A,X( 7),X( 9),X(15),X( 4));
        BODY_60_79(72,A,B,C,D,E,T,X( 8),X(10),X( 0),X( 5));
        BODY_60_79(73,T,A,B,C,D,E,X( 9),X(11),X( 1),X( 6));
        BODY_60_79(74,E,T,A,B,C,D,X(10),X(12),X( 2),X( 7));
        BODY_60_79(75,D,E,T,A,B,C,X(11),X(13),X( 3),X( 8));
        BODY_60_79(76,C,D,E,T,A,B,X(12),X(14),X( 4),X( 9));
        BODY_60_79(77,B,C,D,E,T,A,X(13),X(15),X( 5),X(10));
        BODY_60_79(78,A,B,C,D,E,T,X(14),X( 0),X( 6),X(11));
        BODY_60_79(79,T,A,B,C,D,E,X(15),X( 1),X( 7),X(12));

        c->h0=(c->h0+E)&0xffffffffL;
        c->h1=(c->h1+T)&0xffffffffL;
        c->h2=(c->h2+A)&0xffffffffL;
        c->h3=(c->h3+B)&0xffffffffL;
        c->h4=(c->h4+C)&0xffffffffL;

        if (--num == 0) break;

        A=c->h0;
        B=c->h1;
        C=c->h2;
        D=c->h3;
        E=c->h4;
    }
}

int SHA_Update(SHA_CTX *c, const void *data_, unsigned int len){
    const unsigned char *data=data_;
    unsigned char *p;
    SHA_LONG l;
    unsigned int n;

    if (len==0) return 1;

    l=(c->Nl+(((SHA_LONG)len)<<3))&0xffffffffUL;//乘以8（1个char型==8 bits）
    if (l < c->Nl)
        c->Nh++;
    c->Nh+=(SHA_LONG)(len>>29);	/* might cause compiler warning on 16-bit */
    c->Nl=l;

    n = c->num;
    if (n != 0){
        p=(unsigned char *)c->data;

        if (len >= SHA_CBLOCK || len+n >= SHA_CBLOCK){
            memcpy (p+n,data,SHA_CBLOCK-n);
            HASH_BLOCK_DATA_ORDER (c,p,1);
            n      = SHA_CBLOCK-n;
            data  += n;
            len   -= n;
            c->num = 0;
            memset (p,0,SHA_CBLOCK);	/* keep it zeroed */
        }else{
            memcpy (p+n,data,len);
            c->num += (unsigned int)len;
            return 1;
        }
    }

    n = len/SHA_CBLOCK;
    if (n > 0){
        HASH_BLOCK_DATA_ORDER (c,data,n);
        n    *= SHA_CBLOCK;
        data += n;
        len  -= n;
    }

    if (len != 0){
        p = (unsigned char *)c->data;
        c->num = (unsigned int)len;
        memcpy(p,data,len);
    }
    return 1;
}

int SHA_Final(unsigned char *md, SHA_CTX *c){
    unsigned char *p = (unsigned char *)c->data;
    size_t n = c->num;

    p[n] = 0x80; /* there is always room for one */
    n++;

    if (n > (SHA_CBLOCK-8)){
        memset (p+n,0,SHA_CBLOCK-n);
        n=0;
        HASH_BLOCK_DATA_ORDER (c,p,1);
    }
    memset (p+n,0,SHA_CBLOCK-8-n);

    p += SHA_CBLOCK-8;
#if   defined(DATA_ORDER_IS_BIG_ENDIAN)
    (void)HOST_l2c(c->Nh,p);
    (void)HOST_l2c(c->Nl,p);
#elif defined(DATA_ORDER_IS_LITTLE_ENDIAN)
    (void)HOST_l2c(c->Nl,p);
	(void)HOST_l2c(c->Nh,p);
#endif
    p -= SHA_CBLOCK;
    HASH_BLOCK_DATA_ORDER (c,p,1);
    c->num=0;
    memset (p,0,SHA_CBLOCK);

#ifndef HASH_MAKE_STRING
#error "HASH_MAKE_STRING must be defined!"
#else
    HASH_MAKE_STRING(c,md);
#endif
    return 1;
}