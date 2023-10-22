#include <jni.h>
#include <string>
#include <unistd.h>
#include "android/log.h"
#include "sys/ptrace.h"
#include "errno.h"
#include "pthread.h"
pthread_mutex_t mutex;
void anti1(){
    char line[1024];

    FILE *fp=fopen("/proc/net/tcp","r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, ":69A2") )
            __android_log_print(6,"r0ysue","i find frida from anti1 ");

        }

    }


void anti2() {
    if (ptrace(PTRACE_TRACEME, 0, 0, 0)<0){
        __android_log_print(6,"r0ysue","i find erro  %s",strerror(errno));
    }


}

void anti3(){
//    frida-agent
while (1) {
    pthread_mutex_lock(&mutex);
    char line[1024];

    FILE *fp = fopen("/proc/self/maps", "r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "frida"))
            __android_log_print(6, "r0ysue", "i find frida from anti3");

    }
    sleep(3);
    pthread_mutex_unlock(&mutex);
}

}


void anti4() {
    while (1) {
        int a = access("/data/local/tmp/re.frida.server", .0);
        if (a == 0)
            __android_log_print(6, "r0ysue", "i find frida from anti4 %d", a);
    }
}


extern "C" JNIEXPORT jstring JNICALL
Java_com_roysue_anti1_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

//anti1();
//anti2();
//    anti3();
//    anti4();
//    pthread_t thread;
//    pthread_create(&thread, nullptr, reinterpret_cast<void *(*)(void *)>(anti3), nullptr);

    pthread_t thread1;
    pthread_create(&thread1, nullptr, reinterpret_cast<void *(*)(void *)>(anti4), nullptr);



    return env->NewStringUTF(hello.c_str());
}extern "C"
JNIEXPORT void JNICALL
Java_com_roysue_anti1_MainActivity_antifrida(JNIEnv *env, jobject thiz) {
    int a = access("/data/local/tmp/re.frida.server", .0);
    if (a == 0)
        __android_log_print(6, "r0ysue", "i find frida from anti4 %d", a);
}