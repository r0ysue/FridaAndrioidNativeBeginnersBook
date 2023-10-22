#include <jni.h>
#include <string>
#include <android/log.h>
#include <unistd.h>
#include "myinlinehook.h"
#include "findsobase.h"
#include "findsym.h"
void func(void* a,void* b){
    __android_log_print(6,"r0ysue","%p",b);

}
void func1(void* a,void* b){
    __android_log_print(6,"r0ysue","i am from header%p",b);

}
extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_inlinehook_1final_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    initlinker();
    long* base= static_cast<long *>(findsobase("libart.so"));

    int off=findsym("/system/lib64/libart.so","_ZN3art11ClassLinker10LoadMethodERKNS_7DexFileERKNS_21ClassDataItemIteratorENS_6HandleINS_6mirror5ClassEEEPNS_9ArtMethodE");
    long* addr= reinterpret_cast<long *>((char *) base + off );
    __android_log_print(6,"r0ysue","%p",addr);

    long* st= reinterpret_cast<long *>((char *) base + 0x10D760 + 0x25000);
    __android_log_print(6,"r0ysue","%p",st);

    register_hook(st, (void*)(func));
    register_hook(addr,(void*)func1);
//    sleep(20);
    return env->NewStringUTF(hello.c_str());
}