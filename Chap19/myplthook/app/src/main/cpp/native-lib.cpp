#include <jni.h>
#include <string>
#include "android/log.h"
#include "elf.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "fcntl.h"
#include <unistd.h>
#include "errno.h"

#define PAGE_START(x)  ((x) & PAGE_MASK)
JNIEnv *myenv;

void myr0() {
    __android_log_print(6, "r0ysue", "i am from myr0");
}


void myr1() {
    __android_log_print(6, "r0ysue", "i am from myr1");
}


void secplthook() {
    char line[1024];
    int *startr;
    int *end;
    int n = 1;
    FILE *fp = fopen("/proc/self/maps", "r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "libnative-lib.so")) {
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

    int fd;
    void *start;
    struct stat sb;
    jclass myclass = myenv->FindClass("com/r0ysue/myplthook/MainActivity");
    jfieldID myfile = myenv->GetStaticFieldID(myclass, "filesPath", "Ljava/lang/String;");
    jstring myfilepath = static_cast<jstring>(myenv->GetStaticObjectField(myclass, myfile));
    const char *mylibpath = myenv->GetStringUTFChars(myfilepath, 0);
    fd = open(mylibpath, O_RDONLY);
    __android_log_print(6, "r0ysue", "mylibpath: %s", mylibpath);
    fstat(fd, &sb);
    start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    __android_log_print(6, "r0ysue", "%p", start);
    Elf64_Ehdr header;
    memcpy(&header, start, sizeof(Elf64_Ehdr));
    int secoff = header.e_shoff;
    int secsize = header.e_shentsize;
    int secnum = header.e_shnum;
    int secstr = header.e_shstrndx;
    Elf64_Shdr strtab;
    memcpy(&strtab, (char *) start + secoff + secstr * secsize, sizeof(Elf64_Shdr));
    int strtaboff = strtab.sh_offset;
    char strtabchar[strtab.sh_size];

    memcpy(&strtabchar, (char *) start + strtaboff, strtab.sh_size);
    Elf64_Shdr enumsec;
    int pltoff = 0;
    int pltsize = 0;
    int relaoff = 0;
    int relasize = 0;
    int thoff = 0;
    for (int n = 0; n < secnum; n++) {

        memcpy(&enumsec, (char *) start + secoff + n * secsize, sizeof(Elf64_Shdr));


        if (strcmp(&strtabchar[enumsec.sh_name], ".plt") == 0) {


            pltoff = enumsec.sh_addr;
            pltsize = enumsec.sh_size;
            __android_log_print(6, "r0ysue", "%p", pltoff);

        }
        if (strcmp(&strtabchar[enumsec.sh_name], ".rela.plt") == 0) {


            relaoff = enumsec.sh_addr;
            relasize = enumsec.sh_size;
            __android_log_print(6, "r0ysue", "%p", relaoff);

        }


    }
    Elf64_Rela tmp;
    for (int n = 0; n < relasize; n = n + sizeof(Elf64_Rela)) {
        memcpy(&tmp, (char *) startr + relaoff + n, sizeof(Elf64_Rela));
        long *ptr = reinterpret_cast<long *>((char *) startr + tmp.r_offset);
        if (*ptr == (long) myr0) {

            thoff = n / sizeof(Elf64_Rela);
            __android_log_print(6, "r0ysue", "%d ", thoff);
        }


    }


    long *myr0plt = reinterpret_cast<long *>((char *) startr + pltoff + 0x20 + thoff * 0x10);
    mprotect((void *) PAGE_START((long) myr0plt), PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
    __android_log_print(6, "r0ysue", "%s", strerror(errno));
    int faniloff = 0;
    int flag;
    if ((unsigned long) myr0 > (unsigned long) myr1) {
        faniloff = (unsigned long) myr0plt - (unsigned long) myr1;
        flag = 0;

    } else {
        faniloff = (unsigned long) myr1 - (unsigned long) myr0plt;
        flag = 1;
    }
//    010000D4
    unsigned long code;
    if (flag == 1) {
        code = 0x14000000 + faniloff / 4;

    } else {
        code = 0x18000000 - faniloff / 4;


    }
    __android_log_print(6, "r0ysue", "%p", code);
    *(myr0plt) = code;


}

void proplthook() {
    char line[1024];
    int *start;
    int *end;
    int n = 1;
    FILE *fp = fopen("/proc/self/maps", "r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "libnative-lib.so")) {
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
    Elf64_Ehdr header;
    memcpy(&header, start, sizeof(Elf64_Ehdr));
    int phoff = header.e_phoff;
    int phsize = header.e_phentsize;
    int phnum = header.e_phnum;
    Elf64_Phdr tmp;
    int reloff = 0;
    int relsize = 0;
    int pltoff = 0;

    for (int n = 0; n < phnum; n++) {
        memcpy(&tmp, (char *) start + phoff + n * phsize, sizeof(Elf64_Phdr));
        if (tmp.p_type == 2) {
            Elf64_Dyn tmp1;
//            __android_log_print(6,"r0ysue","%x",tmp.p_memsz);
            for (int n = 0; n < tmp.p_filesz; n = n + sizeof(Elf64_Dyn)) {
                memcpy(&tmp1, (char *) start + tmp.p_paddr + n, sizeof(Elf64_Dyn));
                if (tmp1.d_tag == DT_JMPREL) {//DT_JMPREL==0X17
                    reloff = tmp1.d_un.d_val;

                }
                if (tmp1.d_tag == DT_PLTRELSZ) {//DT_PLTRELSZ==2
                    relsize = tmp1.d_un.d_val;

                }

            }


        }


    }

    pltoff = (reloff + relsize);
    if (pltoff % 0x10 != 0) {
        pltoff = pltoff + 8;

    }
    int thoff=0;
    Elf64_Rela tmp2;
    for (int n = 0; n < relsize; n = n + sizeof(Elf64_Rela)) {
        memcpy(&tmp2, (char *) start + reloff + n, sizeof(Elf64_Rela));
        long *ptr = reinterpret_cast<long *>((char *) start + tmp2.r_offset);
        if (*ptr == (long) myr0) {
            thoff = n / sizeof(Elf64_Rela);

        }


    }

    long *myr0plt = reinterpret_cast<long *>((char *) start + pltoff + 0x20 + thoff * 0x10);
    mprotect((void *) PAGE_START((long) myr0plt), PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
    __android_log_print(6, "r0ysue", "%s", strerror(errno));
    int faniloff = 0;
    int flag;
    if ((unsigned long) myr0 > (unsigned long) myr1) {
        faniloff = (unsigned long) myr0plt - (unsigned long) myr1;
        flag = 0;

    } else {
        faniloff = (unsigned long) myr1 - (unsigned long) myr0plt;
        flag = 1;
    }
//    010000D4
    unsigned long code;
    if (flag == 1) {
        code = 0x14000000 + faniloff / 4;

    } else {
        code = 0x18000000 - faniloff / 4;


    }
    __android_log_print(6, "r0ysue", "%p", code);
    *(myr0plt) = code;


}


extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_myplthook_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    myenv = env;
    proplthook();
//    secplthook();
//    sleep(20);
    myr0();


    return env->NewStringUTF(hello.c_str());
}