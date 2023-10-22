//
// Created by ting on 2019-09-17.
//

#include <jni.h>
#include <string>
#include "tools.h"


jbyteArray getJByteArray(JNIEnv *env, const char *src) {
    jsize length = static_cast<jsize>(strlen(src));
    jbyteArray byteArray = env->NewByteArray(length);
    jbyte *srcBytes = (jbyte *) src;
    env->SetByteArrayRegion(byteArray, 0, length, srcBytes);
    return byteArray;
}


jstring getJString(JNIEnv *env, const char *src) {
    jclass stringClazz = env->FindClass("java/lang/String");
    jmethodID methodID = env->GetMethodID(stringClazz, "<init>", "([BLjava/lang/String;)V");
    jstring encoding = env->NewStringUTF("utf-8");

    jsize length = static_cast<jsize>(strlen(src));
    jbyteArray bytes = env->NewByteArray(length);
    env->SetByteArrayRegion(bytes, 0, length, (jbyte *) src);
    jstring resultString = (jstring) env->NewObject(stringClazz, methodID, bytes, encoding);
//    new String(bytes,"utf-8")

    env->DeleteLocalRef(stringClazz);
    env->DeleteLocalRef(encoding);
    env->DeleteLocalRef(bytes);

    return resultString;
}