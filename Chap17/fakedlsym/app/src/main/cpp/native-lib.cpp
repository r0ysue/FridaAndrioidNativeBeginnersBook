#include <jni.h>
#include <string>
#include <fcntl.h>
#include "dlfcn.h"
#include "errno.h"
#include "android/log.h"
#include "findsym.h"
#include "elf.h"
#include "link.h"
#include "sys/stat.h"
#include "sys/mman.h"
#include "ida.h"
typedef const char *(*getartmethodname)(void *);

int findsymoff();
int findsymofffromhash();
int findsymofffromsection();
extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_fakedlsym_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


//    void* a=dlopen("/system/lib/libart.so",RTLD_NOW);
//    __android_log_print(6,"r0ysue","%s",strerror(errno));
//    char line[1024];
//    int *startr;
//    int *end;
//    int n = 1;
//    FILE *fp = fopen("/proc/self/maps", "r");
//    while (fgets(line, sizeof(line), fp)) {
//        if (strstr(line, "libart.so")) {
//            __android_log_print(6, "r0ysue", "%s", line);
//            if (n == 1) {
//                startr = reinterpret_cast<int *>(strtoul(strtok(line, "-"), NULL, 16));
//                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
//            } else {
//                strtok(line, "-");
//                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
//            }
//            n++;
//        }
//    }
//
//    int a = findsym("/system/lib64/libart.so", "mygetmethodname");
//    long *mygetmethodname = reinterpret_cast<long *>((char *) startr + a - 0x25000);
//    jclass myclass=env->FindClass("com/r0ysue/fakedlsym/MainActivity");
//    jmethodID testmethod=env->GetMethodID(myclass,"testmethod", "()I");
//    getartmethodname aa= reinterpret_cast<getartmethodname>(mygetmethodname);
//    const char* methodname1=aa(testmethod);
//
//    __android_log_print(6, "r0ysue", "%s ", methodname1);

//    findsymoff();
    findsymofffromhash();
//    findsymofffromsection();
    return env->NewStringUTF(hello.c_str());
}


const char* get_string(ElfW(Word) index,const char* strtab_){

    return strtab_ + index;
}


int findsymofffromsection(){

    int fd;
    void *start;
    struct stat sb;
    fd = open("/system/lib64/libart.so", O_RDONLY);
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
    int symoff = 0;
    int symsize = 0;
    int strtabsize = 0;
    int stroff = 0;
    for (int n = 0; n < secnum; n++) {
        memcpy(&enumsec, (char *) start + secoff + n * secsize, sizeof(Elf64_Shdr));
        __android_log_print(6,"r0ysue","%s",&strtabchar[enumsec.sh_name]);
        if (strcmp(&strtabchar[enumsec.sh_name], ".symtab") == 0) {
            symoff = enumsec.sh_offset;
            symsize = enumsec.sh_size;
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
    for (int n = 0; n < symsize; n = n + sizeof(Elf64_Sym)) {
        memcpy(&tmp, (char *)start + symoff+n, sizeof(Elf64_Sym));
//        __android_log_print(6, "r0ysue", "%x",gotoff+n);
        if(tmp.st_name!=0&&strstr(relstr+tmp.st_name,"artFindNativeMethod"))
            __android_log_print(6,"r0ysue","%x",tmp.st_value);


    }
return 1;


}



int findsymofffromhash(){

    char line[1024];
    int *startr;
    int *end;
 const char* libname="/system/lib64/libart.so";


    int n=1;
    FILE *fp=fopen("/proc/self/maps","r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, libname) ) {
            __android_log_print(6,"r0ysue","");
            if(n==1){
                startr = reinterpret_cast<int *>(strtoul(strtok(line, "-"), NULL, 16));
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
            }
            else{
                strtok(line, "-");
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
            }
            n++;
        }
    }

    size_t gnu_nbucket_ = 0;
    // skip symndx
    uint32_t gnu_maskwords_ = 0;
    uint32_t gnu_shift2_ = 0;

    ElfW(Addr) *gnu_bloom_filter_ = nullptr;
    uint32_t *gnu_bucket_ = nullptr;
    uint32_t *gnu_chain_ = nullptr;


    int phof = 0;


    Elf64_Ehdr header;
    memcpy(&header, startr, sizeof(Elf64_Ehdr));
    uint64 rel = 0;
    size_t size = 0;
    long *plt = nullptr;


    char *strtab_ = nullptr;
    Elf64_Sym *symtab_ = nullptr;
    Elf64_Phdr cc;

    memcpy(&cc, ((char *) (startr) + header.e_phoff), sizeof(Elf64_Phdr));

    for (int y = 0; y < header.e_phnum; y++) {
        memcpy(&cc, (char *) (startr) + header.e_phoff + sizeof(Elf64_Phdr) * y,
               sizeof(Elf64_Phdr));
        if (cc.p_type == 6) {

            phof = cc.p_paddr - cc.p_offset;


        }

    }


    for (int y = 0; y < header.e_phnum; y++) {
        memcpy(&cc, (char *) (startr) + header.e_phoff + sizeof(Elf64_Phdr) * y,
               sizeof(Elf64_Phdr));

        if (cc.p_type == 2) {

            Elf64_Dyn dd;

            for (y = 0; y == 0 || dd.d_tag != 0; y++) {
                memcpy(&dd, (char *) (startr) + cc.p_offset + y * sizeof(Elf64_Dyn) + 0x1000,
                       sizeof(Elf64_Dyn));

                if (dd.d_tag == 0x6ffffef5) {

                    gnu_nbucket_ = reinterpret_cast<uint32_t *>((char *) startr + dd.d_un.d_ptr -
                                                                phof)[0];
                    // skip symndx
                    gnu_maskwords_ = reinterpret_cast<uint32_t *>((char *) startr + dd.d_un.d_ptr -
                                                                  phof)[2];
                    gnu_shift2_ = reinterpret_cast<uint32_t *>((char *) startr + dd.d_un.d_ptr -
                                                               phof)[3];

                    gnu_bloom_filter_ = reinterpret_cast<ElfW(Addr) *>((char *) startr +
                                                                       dd.d_un.d_ptr + 16 - phof);
                    gnu_bucket_ = reinterpret_cast<uint32_t *>(gnu_bloom_filter_ + gnu_maskwords_);
                    // amend chain for symndx = header[1]
                    gnu_chain_ = reinterpret_cast<uint32_t *>( gnu_bucket_ +
                                                               gnu_nbucket_ -
                                                               reinterpret_cast<uint32_t *>(
                                                                       (char *) startr +
                                                                       dd.d_un.d_ptr - phof)[1]);


                }

                if (dd.d_tag == 0x17) {
                    rel = reinterpret_cast<uint64 >(
                            (char *) startr + dd.d_un.d_ptr);

                }
                if (dd.d_tag == 2) {
                    size = dd.d_un.d_val / sizeof(ElfW(Rela));

                }
                if (dd.d_tag == 5) {
                    strtab_ = reinterpret_cast< char *>((char *) startr + dd.d_un.d_ptr - phof);
                }
                if (dd.d_tag == 6) {
                    symtab_ = reinterpret_cast<Elf64_Sym *>((
                            (char *) startr + dd.d_un.d_ptr - phof));
                }
                if (dd.d_tag == 3) {
                    plt = reinterpret_cast<long *>((
                            (char *) startr + dd.d_un.d_val - phof));

                }


            }


        }


    }

   const char* name_="artFindNativeMethod";
    uint32_t h = 5381;
    const uint8_t* name = reinterpret_cast<const uint8_t*>(name_);
    while (*name != 0) {
        h += (h << 5) + *name++; // h*33 + c = h + h * 32 + c = h + h << 5 + c
    }
    int index=0;
    uint32_t h2 = h >> gnu_shift2_;
    uint32_t bloom_mask_bits = sizeof(ElfW(Addr))*8;
    uint32_t word_num = (h / bloom_mask_bits) & gnu_maskwords_;
    ElfW(Addr) bloom_word = gnu_bloom_filter_[word_num];
    n = gnu_bucket_[h % gnu_nbucket_];
    do {


        Elf64_Sym * s = symtab_ + n;


        char * sb=strtab_+ s->st_name;

        if (strcmp(sb ,reinterpret_cast<const char *>(name_)) == 0 ) {


            break;
        }
    } while ((gnu_chain_[n++] & 1) == 0);

    Elf64_Sym * sb=symtab_+n;
    long finaladdr= reinterpret_cast<long>(sb->st_value + (char *) startr-phof);
    __android_log_print(6,"r0ysue","%x",sb->st_value);


return 1;



}




int findsymoff() {
    char line[1024];
    int *startr;
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
//    startr=(int*)((char*)startr);
    char *strtab_ = nullptr;
    Elf64_Sym *symtab_ = nullptr;

    Elf64_Ehdr header;
    memcpy(&header, startr, sizeof(Elf64_Ehdr));
    int phoff = header.e_phoff;
    int phsize = header.e_phentsize;
    int phof = 0x25000;

    int phnum = header.e_phnum;
    int strsz = 0;
    Elf64_Phdr myphdr;
    memcpy(&myphdr, (char *) startr + phoff, sizeof(Elf64_Phdr));
    for (int n = 0; n < phnum; n++) {
        memcpy(&myphdr, (char *) (startr) + header.e_phoff + sizeof(Elf64_Phdr) * n,
               sizeof(Elf64_Phdr));

        if (myphdr.p_type == 2) {
            Elf64_Dyn dd;
            for (int y = 0; y == 0 || dd.d_tag != 0; y++) {
                memcpy(&dd, (char *) (startr) + myphdr.p_offset + y * sizeof(Elf64_Dyn) + 0x1000,
                       sizeof(Elf64_Dyn));
                if (dd.d_tag == 5) {
                    strtab_ = reinterpret_cast< char *>((char *) startr + dd.d_un.d_ptr - phof);
                }
                if (dd.d_tag == 6) {
                    symtab_ = reinterpret_cast<Elf64_Sym *>((
                            (char *) startr + dd.d_un.d_ptr - phof));
                }

                if (dd.d_tag == 10) {
                    strsz = dd.d_un.d_val;
                }


            }
        }


    }
    char strtab[strsz];
    memcpy(&strtab, strtab_, strsz);
    Elf64_Sym mytmpsym;
    for (n = 0; n < (long) strtab_ - (long) symtab_; n = n + sizeof(Elf64_Sym)) {
    memcpy(&mytmpsym,(char*)symtab_+n,sizeof(Elf64_Sym));
 if(strstr(strtab_+mytmpsym.st_name,"artFindNativeMethod"))
     __android_log_print(6,"r0ysue","%p %s",mytmpsym.st_value,strtab_+mytmpsym.st_name);

    }
    return 1;
}

