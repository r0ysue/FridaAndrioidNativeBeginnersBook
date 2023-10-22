#include <jni.h>
#include <string>
#include "android/log.h"
#include "sys/mman.h"
#include "errno.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "elf.h"
#include "linux/elf.h"
void myr1() {

    __android_log_print(6, "r0ysue", "i am from myr1");
}

void myr0() {

    __android_log_print(6, "r0ysue", "i am from myr0");


}

__attribute__((__constructor__)) void kk() {


}

void progothook() {

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
    mprotect(startr, (long) end - (long) startr, PROT_WRITE | PROT_READ | PROT_EXEC);
    Elf64_Ehdr header;
    memcpy(&header, startr, sizeof(Elf64_Ehdr));
    int phnum = header.e_phnum;
    int phsize = header.e_phentsize;
    int phoff = header.e_phoff;
    Elf64_Phdr enumprg;
    int gotoff=0;
    int gotsize=0;


    for (int n = 0; n < phnum; n++) {
        memcpy(&enumprg, (char *) startr + phoff + n * phsize, sizeof(Elf64_Phdr));
//        __android_log_print(6, "r0ysue", "%x", enumprg.p_type);
        if (enumprg.p_type == 2) {
            int dynamicoff = enumprg.p_paddr;
            int dynamicsize=enumprg.p_filesz;
            Elf64_Dyn tmp;

            for(int n=0;n<dynamicsize;n=n+sizeof(Elf64_Dyn)){
                memcpy(&tmp, (char *) startr + dynamicoff+n, sizeof(Elf64_Dyn));
              if(tmp.d_tag==DT_PLTGOT){//DT_PLTGOT==3
                    int off=(int)(tmp.d_un.d_val);
                    off=(off-off%0x1000)+0x1000;
                    gotoff=tmp.d_un.d_val;
                    gotsize=off-gotoff;
                  __android_log_print(6,"r0ysue","%x",gotoff);
                  __android_log_print(6,"r0ysue","%x",gotsize);

              }

            }

        }
    }
    for(n=0;n<gotsize;n=n+8){
        long* tmp= reinterpret_cast<long *>((char *) startr + gotoff + n);
        if(*tmp==(long)myr0){
//            __android_log_print(6,"r0ysue","%d",n);
            *tmp= reinterpret_cast<long>(myr1);


        }


    }






}


void secgothook() {
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
    fd = open("/data/local/tmp/gothook.so", O_RDONLY);
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
    int gotoff = 0;
    int gotsize = 0;
    for (int n = 0; n < secnum; n++) {

        memcpy(&enumsec, (char *) start + secoff + n * secsize, sizeof(Elf64_Shdr));


        if (strcmp(&strtabchar[enumsec.sh_name], ".got") == 0) {


            gotoff = enumsec.sh_addr;
            gotsize = enumsec.sh_size;


        }

    }
    for (int n = 0; n < gotsize; n = n + 8) {
        long *tmp = reinterpret_cast<long *>((char *) startr + gotoff + n);
        if (*tmp == (long) myr0) {
            mprotect(reinterpret_cast<void *>((long) tmp & 0xfffffff000), PAGE_SIZE,
                     PROT_WRITE | PROT_READ | PROT_EXEC);

            __android_log_print(6, "r0ysue", "%s", strerror(errno));


            *tmp = reinterpret_cast<long>(myr1);
        }

    }


}

extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_mygothook_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
//secgothook();



    progothook();
    myr0();


    return env->NewStringUTF(hello.c_str());
}