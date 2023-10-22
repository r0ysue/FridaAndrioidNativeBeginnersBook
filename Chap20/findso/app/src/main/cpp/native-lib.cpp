#include <jni.h>
#include <string>
#include "findsym.h"
#include "ida.h"
#include "dlfcn.h"
#include "android/log.h"
#include "errno.h"
#include "findsobase.h"
//void *findsobase(const char *soname) {
//    char line[1024];
//    int *start;
//    int *end;
//    long* base;
//    int n = 1;
//    FILE *fp = fopen("/proc/self/maps", "r");
//    while (fgets(line, sizeof(line), fp)) {
//        if (strstr(line, "linker64")) {
//            __android_log_print(6, "r0ysue", "%s", line);
//            if (n == 1) {
//                start = reinterpret_cast<int *>(strtoul(strtok(line, "-"), NULL, 16));
//                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
//
//            } else {
//                strtok(line, "-");
//                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
//            }
//            n++;
//        }
//
//    }
//    int soheaderoff = findsym("/system/bin/linker64", "__dl__ZL6solist");
//    long *soheader = reinterpret_cast<long *>((char *) start + soheaderoff);
//    n=0;
//    for (_QWORD * result = reinterpret_cast<uint64 *>(*(_QWORD *) soheader); result; result = (_QWORD *)result[5] ){
//        if(*(_QWORD *) ((__int64) result + 408)!= 0&&strcmp(reinterpret_cast<const char *>(*(_QWORD *) ((__int64) result + 408)), soname)==0) {
//            base= reinterpret_cast<long *>(*(_QWORD *) ((char *) result + 16));
//            long* size= reinterpret_cast<long *>(*(_QWORD *) ((char *) result + 24));
//            long* load_bias= reinterpret_cast<long *>(*(_QWORD *) ((char *) result + 256));
//            const char* name= reinterpret_cast<const char *>(*(_QWORD *) ((__int64) result + 408));
//            __android_log_print(6, "r0ysue", " soname:%s ", name);
//            __android_log_print(6, "r0ysue", " base:%p ", *base);
//            __android_log_print(6, "r0ysue", " size:%x ",size);
//            __android_log_print(6, "r0ysue", " loadbias:%p ", load_bias);
//        }
//
//    }
//
//    return base;
//}


extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_findso_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    initlinker();
   long* libnative= static_cast<long *>(findsobase("libart.so"));

//    long myoff= reinterpret_cast<long>(soinfo_findsym(libnative));
//    __android_log_print(6,"r0ysue","%p",myoff);

    void* libc=dlopen("libc.so",RTLD_NOW);
    __android_log_print(6,"r0ysue"," dlopen %p",libc);
    long* mystrstr= static_cast<long *>(dlsym(libc, "strstr"));
    __android_log_print(6,"r0ysue","dlsym %p",mystrstr);
   void* libarthandle=getmyhandle(libnative);
    void* loadmethod=dlsym(libarthandle,"_ZN3art11ClassLinker10LoadMethodERKNS_7DexFileERKNS_21ClassDataItemIteratorENS_6HandleINS_6mirror5ClassEEEPNS_9ArtMethodE");
    __android_log_print(6,"r0ysue","dlsym %p",loadmethod);
    return env->NewStringUTF(hello.c_str());
}