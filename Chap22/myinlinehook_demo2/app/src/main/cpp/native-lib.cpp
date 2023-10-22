#include <jni.h>
#include <string>
#include <unistd.h>
#include "findsym.h"
#include "android/log.h"
#include "sys/mman.h"
#include "pthread.h"
#include "fcntl.h"
#define PAGE_START(x)  ((x) & PAGE_MASK)


pthread_mutex_t mutex;


long retaddress;
long old_code1;
long old_code2;
long old_code3;
long old_code4;

typedef void *(*func1)();

int n = 0;
int flag = 0;

void dump_dex(long *begin, int size) {
    char path[100];
    sprintf(path, "/sdcard/%d.dex", size);
    if (access(path, F_OK) != -1)
        return;
    int fp = open(path, O_CREAT | O_APPEND | O_RDWR, 0666);
    write(fp, (void *) begin, size);
    fsync(fp);
    close(fp);
    __android_log_print(6, "r0ysue", "i dump success %d.dex", size);
}

void myr0() {


    __android_log_print(6, "r0ysue", "i am from myr0");
}

void myloadmethod(long *a, long *b) {

    long *begin = reinterpret_cast<long *>(*(b + 1));
    int size = *(int *) (b + 2);
    dump_dex(begin, size);
//    __android_log_print(6,"r0ysue","i am from myloadmethod %p %x",*begin,size);

}


extern "C" void myreplace();

//void huifu() {
//
//    return;
////sleep(1);
//}


void func() {

    func1 sb = reinterpret_cast<func1>(myloadmethod);
    sb();
//    huifu();
//    sleep(1);

//   memcpy((char*)myreplace + 0x98, reinterpret_cast<const void *>(&old_code1), 4);
//    memcpy((char*)myreplace + 0x9c, reinterpret_cast<const void *>(&old_code2), 4);
//    memcpy((char*)myreplace + 0xa0, reinterpret_cast<const void *>(&old_code3), 4);
//    memcpy((char*)myreplace + 0xa4, reinterpret_cast<const void *>(&old_code4), 4);
//
//
//    memcpy((char*)myreplace + 0x98,&old_code1,4);
//    memcpy((char*)myreplace + 0x9c,&old_code2,4);
//    memcpy((char*)myreplace + 0xa0,&old_code3,4);
//    memcpy((char*)myreplace + 0xa4,&old_code4,4);
//

//    if (flag == 0) {
//        sleep(1);
//        flag = 1;
//    }

//    return;


}


extern "C" void myreplace() {
    asm("sub SP, SP, #0x200");
    asm("mrs x16, NZCV");
    asm("stp X29, X30, [SP,#0x10]");
    asm("stp X0, X1, [SP,#0x20]");
    asm("stp X2, X3, [SP,#0x30]");
    asm("stp X4, X5, [SP,#0x40]");
    asm("stp X6, X7, [SP,#0x50]");
    asm("stp X8, X9, [SP,#0x60]");
    asm("stp X10, X11, [SP,#0x70]");
    asm("stp X12, X13, [SP,#0x80]");
    asm("stp X14, X15, [SP,#0x90]");
    asm("stp X16, X17, [SP,#0x100]");
    asm("stp X28, X19, [SP,#0x110]");
    asm("stp X20, X21, [SP,#0x120]");
    asm("stp X22, X23, [SP,#0x130]");
    asm("stp X24, X25, [SP,#0x140]");
    asm("stp X26, X27, [SP,#0x150]");

//    asm("sub SP, SP, #0x20");

//    long addr=0;
//    __asm__("mov %[input_n], x30\r\n"
//    :[result_m] "=r"(addr)
//    :[input_n] "r"(addr)
//    );

    func();
//    pthread_mutex_unlock(&mutex);

//    asm("add SP, SP, #0x20");

    asm("ldp X16, X17, [SP,#0x100]");
    asm("ldp X28, X19, [SP,#0x110]");
    asm("ldp X20, X21, [SP,#0x120]");
    asm("ldp X22, X23, [SP,#0x130]");
    asm("ldp X24, X25, [SP,#0x140]");
    asm("ldp X26, X27, [SP,#0x150]");
    asm("ldp X8, X9, [SP,#0x60]");
    asm("ldp X10, X11, [SP,#0x70]");
    asm("ldp X12, X13, [SP,#0x80]");
    asm("ldp X14, X15, [SP,#0x90]");
    asm("msr NZCV, x16");
//    asm("ldp X16, X17, [SP,#0x10]");
    asm("ldp X0, X1, [SP,#0x20]");
    asm("ldp X2, X3, [SP,#0x30]");
    asm("ldp X4, X5, [SP,#0x40]");
    asm("ldp X6, X7, [SP,#0x50]");
    asm("ldp X29, X30, [SP,#0x10]");
    asm("add SP, SP, #0x200");
    asm("ldp    X29, X30, [SP],#0x10");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("ldr x16,8");
    asm("br x16");
    asm("mov x0,x0");
    asm("mov x0,x0");

}


void hook() {
//    pthread_mutex_lock(&mutex);
    char line[1024];
    int *start;
    int *end;
    int n = 1;
    FILE *fp = fopen("/proc/self/maps", "r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "libart.so")) {
            __android_log_print(6, "r0ysue", "%s", line);
            if (n == 1) {
                start = reinterpret_cast<int *>(strtoul(strtok(line, "-"), NULL, 16));
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));

            } else {
                strtok(line, "-");
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
            }
            n++;


        }


    }


    int loadmethod_off = findsym("/system/lib64/libart.so",
                                 "_ZN3art11ClassLinker10LoadMethodERKNS_7DexFileERKNS_21ClassDataItemIteratorENS_6HandleINS_6mirror5ClassEEEPNS_9ArtMethodE");
    long *loadmethod = reinterpret_cast<long *>((char *) start + loadmethod_off - 0x25000);
    int flag = 0;
    long offsit = 0;
    if ((long) loadmethod > (long) myreplace) {
        offsit = (long) loadmethod - (long) myreplace;
    } else {
        offsit = (long) myreplace - (long) loadmethod;
        flag = 1;
    }

    int code;
    if (flag == 1) {
        code = 0x14000000 + offsit / 4;

    } else {
        code = 0x18000000 - offsit / 4;


    }
    __android_log_print(6, "r0ysue", "%p %p", offsit, loadmethod);

    mprotect((void *) PAGE_START((long) loadmethod), PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
//    mprotect((void*)PAGE_START((long)myreplace),PAGE_SIZE,PROT_WRITE|PROT_READ|PROT_EXEC);
    old_code1 = *(int *) ((char *) loadmethod);
    old_code2 = *(int *) ((char *) loadmethod + 4);
    old_code3 = *(int *) ((char *) loadmethod + 8);
    old_code4 = *(int *) ((char *) loadmethod + 12);
    int code1 = 0x58000050;//ldr x16,8
    int code2 = 0xd61f0200;
    long code3 = reinterpret_cast<long>(myreplace);
    retaddress = reinterpret_cast<long>((char *) loadmethod + 0x10);


    mprotect((void *) PAGE_START((long) myreplace), PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
    *(int *) ((char *) myreplace + 0x98) = old_code1;
    *(int *) ((char *) myreplace + 0x9c) = old_code2;
    *(int *) ((char *) myreplace + 0xa0) = old_code3;
    *(int *) ((char *) myreplace + 0xa4) = old_code4;

    *(long *) ((char *) myreplace + 0xb0) = retaddress;
//
//    retaddress= reinterpret_cast<long>((char*)loadmethod+0x10);
//    *(long*)((char*)myreplace+ 0xb0)=retaddress;


    *(int *) ((char *) loadmethod) = code1;
    *(int *) ((char *) loadmethod + 4) = code2;
    *(long *) ((char *) loadmethod + 8) = code3;

//    pthread_mutex_unlock(&mutex);


//    30000058




}

extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_myinlinehook_1demo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    pthread_t thread;
//    pthread_mutex_init(&mutex, NULL);
//pthread_create(&thread, nullptr, reinterpret_cast<void *(*)(void *)>(hook), nullptr);
    hook();
sleep(1);
//    myr0();

    return env->NewStringUTF(hello.c_str());
}