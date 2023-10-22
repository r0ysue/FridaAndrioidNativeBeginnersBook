#include <jni.h>
#include <string>
#include <unistd.h>
#include "findsym.h"
#include "android/log.h"
#include "sys/mman.h"
#define PAGE_START(x)  ((x) & PAGE_MASK)


void func(){

__android_log_print(6,"r0ysue","i am from myreplace");

}
void myreplace() {
    asm("sub SP, SP, #0x100");
    asm("mrs x15, NZCV");
    asm("stp X29, X30, [SP,#0x10]");
    asm("stp X0, X1, [SP,#0x20]");
    asm("stp X2, X3, [SP,#0x30]");
    asm("stp X4, X5, [SP,#0x40]");
    asm("stp X6, X7, [SP,#0x50]");
    asm("stp X8, X9, [SP,#0x60]");
    asm("stp X10, X11, [SP,#0x70]");
    asm("stp X12, X13, [SP,#0x80]");
    asm("stp X14, X15, [SP,#0x90]");

    func();

    asm("ldp X8, X9, [SP,#0x60]");
    asm("ldp X10, X11, [SP,#0x70]");
    asm("ldp X12, X13, [SP,#0x80]");
    asm("ldp X14, X15, [SP,#0x90]");
    asm("msr NZCV, x15 ");
    asm("ldp X16, X17, [SP,#0x10]");
    asm("ldp X0, X1, [SP,#0x20]");
    asm("ldp X2, X3, [SP,#0x30]");
    asm("ldp X4, X5, [SP,#0x40]");
    asm("ldp X6, X7, [SP,#0x50]");
    asm("ldp X29, X30, [SP,#0x10]");
    asm("add SP, SP, #0x100");






}

void myr0(){


    __android_log_print(6,"r0ysue","i am from myr0");
}



void hook() {

//    char line[1024];
//    int *start;
//    int *end;
//    int n = 1;
//    FILE *fp = fopen("/proc/self/maps", "r");
//    while (fgets(line, sizeof(line), fp)) {
//        if (strstr(line, "libart.so")) {
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
//
//
//        }
//
//
//    }
//
//
//    int loadmethod_off = findsym("/system/lib64/libart.so",
//                                 "_ZN3art11ClassLinker10LoadMethodERKNS_7DexFileERKNS_21ClassDataItemIteratorENS_6HandleINS_6mirror5ClassEEEPNS_9ArtMethodE");
//    long *loadmethod = reinterpret_cast<long *>((char *) start + loadmethod_off-0x25000);
//    int flag = 0;
//    long offsit = 0;
//    if ((long) loadmethod > (long) myreplace) {
//        offsit = (long) loadmethod - (long) myreplace;
//    } else {
//        offsit = (long) myreplace - (long) loadmethod;
//        flag=1;
//    }
//
//    int code;
//    if (flag == 1) {
//        code = 0x14000000 + offsit / 4;
//
//    } else {
//        code = 0x18000000 - offsit / 4;
//
//
//    }
//    __android_log_print(6, "r0ysue", "%p %p", offsit, loadmethod);

    mprotect((void*)PAGE_START((long)myr0),PAGE_SIZE,PROT_WRITE|PROT_READ|PROT_EXEC);
    int code1=0x58000050;//ldr x17,8
    int code2=0xd61f0200;
    long code3= reinterpret_cast<long>(myreplace);

    *(int*)((char*)myr0)=code1;
    *(int*)((char*)myr0+4)=code2;
    *(long*)((char*)myr0+8)=code3;
//    30000058
    sleep(20);



}

extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_myinlinehook_1demo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    hook();

    myr0();
    return env->NewStringUTF(hello.c_str());
}