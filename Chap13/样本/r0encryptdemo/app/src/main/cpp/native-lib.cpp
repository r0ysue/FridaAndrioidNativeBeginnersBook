#include <jni.h>
#include <string>
#include "md5.h"
#include "sha1.h"
#include "sha256.h"
#include "sha512.h"
#include "hmac_md5.h"
#include "hmac_sha1.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_r0encryptdemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_r0ysue_r0encryptdemo_MainActivity_md5(JNIEnv *env, jobject thiz, jstring str) {
    char *szText;
    // 通过JNI API 获得 java层传入字符串
    szText = (char *) (*env).GetStringUTFChars(str, nullptr);

    // 初始化MD5的上下文结构体
    MD5_CTX context = {0};
    MD5Init(&context);
    // 传入待处理内容以及长度
    MD5Update(&context, reinterpret_cast<unsigned char *>(szText), strlen(szText));
    // 收尾和输出
    unsigned char dest[16] = {0};
    MD5Final(dest, &context);
    // 根据NDK开发指南，GET的资源必须Release释放掉
    (*env).ReleaseStringUTFChars(str, szText);

    // 转成十六进制字符串
    int i = 0;
    char szMd5[33] = {0};
    for (i = 0; i < 16; i++) {
        sprintf(szMd5, "%s%02x", szMd5, dest[i]);
    }
    // 传回Java层
    return (*env).NewStringUTF(szMd5);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_r0ysue_r0encryptdemo_MainActivity_sha1(JNIEnv *env, jobject thiz, jstring str) {
    char *szText;
    // 通过JNI API 获得 java层传入字符串
    szText = (char *) (*env).GetStringUTFChars(str, nullptr);
    SHA_CTX context = {0};
    SHA_Init(&context);
    SHA_Update(&context, szText, strlen(szText));
    // 收尾和输出
    unsigned char dest[20] = {0};
    SHA_Final(dest, &context);
    // 根据NDK开发指南，GET的资源必须Release释放掉
    (*env).ReleaseStringUTFChars(str, szText);

    // 转成十六进制字符串
    int i = 0;
    char szSha1[41] = {0};
    for (i = 0; i < 20; i++) {
        sprintf(szSha1, "%s%02x", szSha1, dest[i]);
    }
    // 传回Java层
    return (*env).NewStringUTF(szSha1);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_r0ysue_r0encryptdemo_MainActivity_sha256(JNIEnv *env, jobject thiz, jstring str) {
    char *szText;
    szText = (char *) (*env).GetStringUTFChars(str, nullptr);

    sha256_context context = {0};
    sha256_init(&context);
    sha256_update(&context, reinterpret_cast<uint8_t *>(szText), strlen(szText));
    unsigned char dest[32] = {0};
    sha256_final(&context, dest);
    (*env).ReleaseStringUTFChars(str, szText);

    int i = 0;
    char szSha1[65] = {0};
    for (i = 0; i < 32; i++) {
        sprintf(szSha1, "%s%02x", szSha1, dest[i]);
    }
    return (*env).NewStringUTF(szSha1);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_r0ysue_r0encryptdemo_MainActivity_sha512(JNIEnv *env, jobject thiz, jstring str) {
    char *szText;
    szText = (char *) (*env).GetStringUTFChars(str, nullptr);
    SHA512_CB context = {0};
    SHA512Init(&context);
    SHA512Update(&context, reinterpret_cast<unsigned char *>(szText), strlen(szText));
    unsigned char dest[64] = {0};
    SHA512Final(&context, dest);
    (*env).ReleaseStringUTFChars(str, szText);

    int i = 0;
    char szSha1[129] = {0};
    for (i = 0; i < 64; i++) {
        sprintf(szSha1, "%s%02x", szSha1, dest[i]);
    }
    return (*env).NewStringUTF(szSha1);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_r0ysue_r0encryptdemo_MainActivity_hmacmd5(JNIEnv *env, jobject thiz, jstring str, jstring key) {
    char *szText;
    char *keyText;
    szText = (char *) (*env).GetStringUTFChars(str, nullptr);
    keyText = (char *) (*env).GetStringUTFChars(key, nullptr);
    unsigned char dest[16] = {0};
    hmac_md5(reinterpret_cast<unsigned char *>(szText), strlen(szText),
             reinterpret_cast<unsigned char *>(keyText), strlen(keyText),dest);
    int i = 0;
    char szMd5[33] = {0};
    for (i = 0; i < 16; i++) {
        sprintf(szMd5, "%s%02x", szMd5, dest[i]);
    }
    return (*env).NewStringUTF(szMd5);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_r0ysue_r0encryptdemo_MainActivity_hmacsha1(JNIEnv *env, jobject thiz, jstring str, jstring key) {
    char *szText;
    char *keyText;
    szText = (char *) (*env).GetStringUTFChars(str, nullptr);
    keyText = (char *) (*env).GetStringUTFChars(key, nullptr);
    unsigned char dest[20] = {0};
    hmac_sha1(reinterpret_cast<unsigned char *>(szText), strlen(szText),
             reinterpret_cast<unsigned char *>(keyText), strlen(keyText),dest);
    int i = 0;
    char szMd5[41] = {0};
    for (i = 0; i < 20; i++) {
        sprintf(szMd5, "%s%02x", szMd5, dest[i]);
    }
    return (*env).NewStringUTF(szMd5);
}extern "C"
JNIEXPORT jstring JNICALL
Java_com_r0ysue_r0encryptdemo_MainActivity_saltmd5(JNIEnv *env, jobject thiz, jstring str) {
    char *szText;
    char *saltText = "r0ysue";
    // 通过JNI API 获得 java层传入字符串
    szText = (char *) (*env).GetStringUTFChars(str, nullptr);
    // 初始化MD5的上下文结构体
    MD5_CTX context = {0};
    MD5Init(&context);
    // 传入待处理内容以及长度
    MD5Update(&context, reinterpret_cast<unsigned char *>(szText), strlen(szText));
    MD5Update(&context, reinterpret_cast<unsigned char *>(saltText), strlen(saltText));
    // 收尾和输出
    unsigned char dest[16] = {0};
    MD5Final(dest, &context);
    // 根据NDK开发指南，GET的资源必须Release释放掉
    (*env).ReleaseStringUTFChars(str, szText);

    // 转成十六进制字符串
    int i = 0;
    char szMd5[33] = {0};
    for (i = 0; i < 16; i++) {
        sprintf(szMd5, "%s%02x", szMd5, dest[i]);
    }
    // 传回Java层
    return (*env).NewStringUTF(szMd5);
}extern "C"
JNIEXPORT jstring JNICALL
Java_com_r0ysue_r0encryptdemo_MainActivity_md5half(JNIEnv *env, jobject thiz, jstring str) {
    char *szText;
    // 通过JNI API 获得 java层传入字符串
    szText = (char *) (*env).GetStringUTFChars(str, nullptr);
    // 初始化MD5的上下文结构体
    MD5_CTX context = {0};
    MD5Init(&context);
    // 传入待处理内容以及长度
    MD5Update(&context, reinterpret_cast<unsigned char *>(szText), strlen(szText));
    // 收尾和输出
    unsigned char dest[16] = {0};
    MD5Final(dest, &context);
    // 根据NDK开发指南，GET的资源必须Release释放掉
    (*env).ReleaseStringUTFChars(str, szText);

    // 转成十六进制字符串
    int i = 0;
    char szMd5[17] = {0};
    for (i = 4; i < 12; i++) {
        sprintf(szMd5, "%s%02x", szMd5, dest[i]);
    }
    // 传回Java层
    return (*env).NewStringUTF(szMd5);
}