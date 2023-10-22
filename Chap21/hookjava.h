//
// Created by root on 10/14/21.
//

#ifndef MYJAVAHOOK_HOOKJAVA_H
#define MYJAVAHOOK_HOOKJAVA_H


#include "ida.h"
#include "../../../../../../Android/Sdk/ndk/21.1.6352462/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include/jni.h"

void huifu(__int64 ,int);
void baocun(__int64 ,int);
void myhook(JNIEnv*,void* ,const char* ,const char* ,const char* ,int);
#endif //MYJAVAHOOK_HOOKJAVA_H
