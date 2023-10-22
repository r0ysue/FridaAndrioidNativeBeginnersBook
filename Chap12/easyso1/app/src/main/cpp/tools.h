//
// Created by ting on 2019-09-17.
//

#ifndef AESTEST_TOOLS_H
#define AESTEST_TOOLS_H


#define getJString           o0OoOOOO
#define getJByteArray        o000OO0O


#ifdef __cplusplus
extern "C" {
#endif


jstring getJString(JNIEnv *env, const char *src);

jbyteArray getJByteArray(JNIEnv *env, const char *src);


#ifdef __cplusplus
}
#endif


#endif //AESTEST_TOOLS_H
