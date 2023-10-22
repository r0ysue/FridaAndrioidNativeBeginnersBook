//
// Created by root on 10/14/21.
//

#include <jni.h>
#include <string>
#include <dlfcn.h>
#include "android/log.h"
#include "elf.h"
#include "ida.h"
#include "link.h"
#include "sys/mman.h"
#include "errno.h"
#include "findsym.h"
#include "sys/stat.h"
#include "fcntl.h"

int* enumsym(const char* lib,int size,int start1){


    int fd;
    void *start;
    struct stat sb;
    fd = open(lib, O_RDONLY); /*打开/etc/passwd */
    fstat(fd, &sb); /* 取得文件大小 */
    start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

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
    int strtabsize = 0;
    int stroff = 0;
    for (int n = 0; n < secnum; n++) {

        memcpy(&enumsec, (char *) start + secoff + n * secsize, sizeof(Elf64_Shdr));


        if (strcmp(&strtabchar[enumsec.sh_name], ".symtab") == 0) {
            gotoff = enumsec.sh_offset;
            gotsize = enumsec.sh_size;

        }
        if (strcmp(&strtabchar[enumsec.sh_name], ".strtab") == 0) {
            stroff = enumsec.sh_offset;
            strtabsize = enumsec.sh_size;

        }


    }
    int realoff=0;
    char relstr[strtabsize];
    Elf64_Sym tmp;
    memcpy(&relstr, (char *) start + stroff, strtabsize);
int* sdc= static_cast<int *>(malloc(gotsize / sizeof(Elf64_Sym)*sizeof(int*)));
    for (int n = 0; n < gotsize; n = n + sizeof(Elf64_Sym)) {
        memcpy(&tmp, (char *)start + gotoff+n, sizeof(Elf64_Sym));
//        __android_log_print(6, "r0ysue", "%x",gotoff+n);

            sdc[n/sizeof(Elf64_Sym)]=tmp.st_value;
            if((unsigned long)sdc[n/sizeof(Elf64_Sym)]>(unsigned long)size||(unsigned long)sdc[n/sizeof(Elf64_Sym)]<(unsigned long)start1)
                sdc[n/sizeof(Elf64_Sym)]=start1;

    }
    return sdc;


}



int getsymsize(const char* lib){


    int fd;
    void *start;
    struct stat sb;
    fd = open(lib, O_RDONLY); /*打开/etc/passwd */
    fstat(fd, &sb); /* 取得文件大小 */
    start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

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
    int strtabsize = 0;
    int stroff = 0;
    for (int n = 0; n < secnum; n++) {

        memcpy(&enumsec, (char *) start + secoff + n * secsize, sizeof(Elf64_Shdr));


        if (strcmp(&strtabchar[enumsec.sh_name], ".symtab") == 0) {
            gotoff = enumsec.sh_offset;
            gotsize = enumsec.sh_size;

        }
        if (strcmp(&strtabchar[enumsec.sh_name], ".strtab") == 0) {
            stroff = enumsec.sh_offset;
            strtabsize = enumsec.sh_size;

        }


    }

    return gotsize/sizeof(Elf64_Sym);


}




int findsym(const char* lib,const char* sym){


    int fd;
    void *start;
    struct stat sb;
    fd = open(lib, O_RDONLY);
    fstat(fd, &sb);
    start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

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
    int strtabsize = 0;
    int stroff = 0;
    for (int n = 0; n < secnum; n++) {

        memcpy(&enumsec, (char *) start + secoff + n * secsize, sizeof(Elf64_Shdr));


        if (strcmp(&strtabchar[enumsec.sh_name], ".symtab") == 0) {
            gotoff = enumsec.sh_offset;
            gotsize = enumsec.sh_size;

        }
        if (strcmp(&strtabchar[enumsec.sh_name], ".strtab") == 0) {
            stroff = enumsec.sh_offset;
            strtabsize = enumsec.sh_size;

        }


    }
    int realoff=0;
    char relstr[strtabsize];
    Elf64_Sym tmp;
    memcpy(&relstr, (char *) start + stroff, strtabsize);

    for (int n = 0; n < gotsize; n = n + sizeof(Elf64_Sym)) {
        memcpy(&tmp, (char *)start + gotoff+n, sizeof(Elf64_Sym));
//        __android_log_print(6, "r0ysue", "%x",gotoff+n);
        if(tmp.st_name!=0&&strstr(relstr+tmp.st_name,sym))
            realoff=tmp.st_value;


    }
    return realoff;


}