#include <jni.h>
#include <string>
#include <filesystem>
#include "ida.h"
#include "elf.h"
#include "android/log.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "findsym.h"

int flag=0;
_QWORD nativ[10];
long jump[10];
int *startr;
long old1[10];
long old2[10];
void baocun(__int64 a,int n){


 old1[n]=*((long *)a + 5);
 old2[n]=*(_QWORD *)(a + 4);

    *((long *)a + 5)= jump[n];
//    *(_QWORD *)(a + 4)=0x1d8ee408000101;
    *(_QWORD *)(a + 4)= nativ[n]|0x80100;
}

void huifu(__int64 a,int n){

    *((long *)a + 5)=old1[n];
    *(_QWORD *)(a + 4)=old2[n];

}




void myhook(JNIEnv* env,void* func,const char* classname,const char* methodname,const char* shoty,int ns){

     JNINativeMethod getMethods[] = {
            {methodname, shoty,(void*)func}
    };
if(flag==0) {
    char line[1024];

    int *end;
    int n = 1;
    FILE *fp = fopen("/proc/self/maps", "r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "libart.so")) {
            __android_log_print(6, "r0ysue", "%s", line);
            if (n == 1) {
                startr = reinterpret_cast<int *>(strtoul(strtok(line, "-"), NULL, 16));
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));


            } else {
                strtok(line, "-");
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
            }
            n++;


        }


    }
    flag++;

}

    int so=findsym("/system/lib64/libart.so","art_quick_generic_jni_trampoline");

    jclass a=env->FindClass(classname);
    jmethodID b=env->GetMethodID(a,methodname, shoty);
    __int64 a1= reinterpret_cast<long long int>(b);
    nativ[ns]=*(_QWORD *)(a1 + 4);
    jump[ns]=*((long *)a1 + 5);
    *(_QWORD *)(a1 + 4)= *(_QWORD *)(a1 + 4)^0x80100 ;

    *((long *)a1 + 5)= reinterpret_cast<long>((char *) startr +so - 0x25000);
    *(_QWORD *)(a1 + 32)= reinterpret_cast<uint64>(func);
//    env->RegisterNatives(a,getMethods,1);




}
