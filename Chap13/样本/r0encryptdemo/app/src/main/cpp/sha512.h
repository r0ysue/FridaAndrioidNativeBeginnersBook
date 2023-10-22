//
// Created by Lenovo on 2021-04-11.
//

#ifndef ENCRYPTDEMO_SHA512_H
#define ENCRYPTDEMO_SHA512_H

#ifndef SHA512_H
#define SHA512_H

//////////////////////////////////////////////////////////
// SHA512_CB(control block) 							//
// SHA512_CB:SHA512控制块,包含算法运算过程中将用到的信息//
// count[2]:记录128位的数字长度（两个64位） 		    //
// state[8]:A-H八个初始常量(64bit)  					//
// buffer[128]:用于每次运算的1024bit  					//
//  												    //
//////////////////////////////////////////////////////////
typedef struct
{
    unsigned long long count[2];
    unsigned long long state[8];
    unsigned char buffer[128];
} SHA512_CB;


#ifdef __cplusplus
extern "C" {
// 初始化函数，初始化SHA_CB的各个值
void SHA512Init(SHA512_CB *context);

// 将数据加入
void SHA512Update(SHA512_CB *context, unsigned char *input, unsigned long long inputlen);

// 处理完最后再调用，这个处理尾数
void SHA512Final(SHA512_CB *context, unsigned char digest[32]);

// 加密处理函数：Hash加密的核心工厂
void SHA512Transform(unsigned long long state[8], unsigned char block[128]);

// 编码函数：将整型编码转为字符
void SHA512Encode(unsigned char *output, const unsigned long long *input, unsigned long long len);
void SHA512Decode(unsigned long long *output, unsigned char *input, unsigned long long len);
}
#endif
#endif
#endif //ENCRYPTDEMO_SHA512_H
