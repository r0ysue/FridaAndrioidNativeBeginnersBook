//
// Created by root on 11/20/21.
//

#include "findsobase.h"
#include <jni.h>
#include <string>
#include "findsym.h"
#include "ida.h"
#include "dlfcn.h"
#include "android/log.h"
#include "errno.h"
#include "elf.h"

long *soheader;
typedef void*(*to_handle)(void*) ;
int *start;

void initlinker(){

    char line[1024];

    int *end;
    long* base;
    int n = 1;
    FILE *fp = fopen("/proc/self/maps", "r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "linker64")) {
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
    int soheaderoff = findsym("/system/bin/linker64", "__dl__ZL6solist");
    soheader = reinterpret_cast<long *>((char *) start + soheaderoff);



}

void* soinfo_findsym(void* soinfo){
//    long* strtab_= reinterpret_cast<long *>(*(long*)((char *) soinfo + 56));
//    long* symtab_= reinterpret_cast<long *>(*(long*)((char *) soinfo + 64));
//    long strsz= reinterpret_cast<long>(*(long*)((char *) soinfo + 336));
//    int result;
//    __android_log_print(6,"r0ysue","%p %p %p",strtab_,symtab_,strsz);
//    char strtab[strsz];
//    memcpy(&strtab, strtab_, strsz);
//    Elf64_Sym mytmpsym;
//    for (int n = 0; n < (long) strtab_ - (long) symtab_; n = n + sizeof(Elf64_Sym)) {
//        memcpy(&mytmpsym,(char*)symtab_+n,sizeof(Elf64_Sym));
////        __android_log_print(6,"r0ysue","%p %s",mytmpsym.st_name,strtab+mytmpsym.st_name);
//        if(strstr(reinterpret_cast<const char *>(strtab + mytmpsym.st_name), "artFindNativeMethod"))
//            result=mytmpsym.st_value;
//
//
//    }


//return reinterpret_cast<void *>(result);
}


void* getmyhandle(void* soinfo){
int handleoff=findsym("/system/bin/linker64","__dl__ZN6soinfo9to_handleEv");

    to_handle func= reinterpret_cast<to_handle>((char *) start + handleoff);
    void* myhandle=func(soinfo);


    return myhandle;

}




void *findsobase(const char *soname) {

    long* base;
    long* soinfo;
   int  n=0;
    long* load_bias;
    for (_QWORD * result = reinterpret_cast<uint64 *>(*(_QWORD *) soheader); result; result = (_QWORD *)result[5] ){
        if(*(_QWORD *) ((__int64) result + 408)!= 0&&strcmp(reinterpret_cast<const char *>(*(_QWORD *) ((__int64) result + 408)), soname)==0) {
            base= reinterpret_cast<long *>(*(_QWORD *) ((char *) result + 16));
            long* size= reinterpret_cast<long *>(*(_QWORD *) ((char *) result + 24));
            load_bias= reinterpret_cast<long *>(*(_QWORD *) ((char *) result + 256));
            const char* name= reinterpret_cast<const char *>(*(_QWORD *) ((__int64) result + 408));
            __android_log_print(6, "r0ysue", " soname:%s ", name);
            __android_log_print(6, "r0ysue", " base:%p ", *base);
            __android_log_print(6, "r0ysue", " size:%x ",size);
            __android_log_print(6, "r0ysue", " loadbias:%p ", load_bias);
            soinfo= reinterpret_cast<long *>(result);
            break;
        }

    }

    return load_bias;
}