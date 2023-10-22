#include <jni.h>
#include <string>
#include <link.h>
#include <dlfcn.h>
#include "elf.h"
#include "sys/stat.h"
#include "fcntl.h"
#include "sys/mman.h"
#include "android/log.h"
#include "errno.h"

#include "ida.h"
#define FLAG_LINKED     0x00000001
#define FLAG_EXE        0x00000004 // The main executable
#define FLAG_LINKER     0x00000010 // The linker itself
#define FLAG_GNU_HASH   0x00000040 // uses gnu hash
#define FLAG_NEW_SOINFO 0x40000000 // new soinfo format
#define SUPPORTED_DT_FLAGS_1 (DF_1_NOW | DF_1_GLOBAL | DF_1_NODELETE)
#define SOINFO_VERSION 2
#if defined(__aarch64__) || defined(__x86_64__)
#define USE_RELA 1
#endif
#define MAYBE_MAP_FLAG(x, from, to)  (((x) & (from)) ? (to) : 0)
#define PFLAGS_TO_PROT(x)            (MAYBE_MAP_FLAG((x), PF_X, PROT_EXEC) | \
                                   MAYBE_MAP_FLAG((x), PF_R, PROT_READ) | \
                              MAYBE_MAP_FLAG((x), PF_W, PROT_WRITE))
#define PAGE_START(x) ((x) & PAGE_MASK)
#define PAGE_START(x)  ((x) & PAGE_MASK)
#define PAGE_OFFSET(x) ((x) & ~PAGE_MASK)
#define PAGE_END(x)    PAGE_START((x) + (PAGE_SIZE-1))





#define FLAG_NEW_SOINFO 0x40000000
#define SOINFO_VERSION 2
typedef void (*linker_function_t)();
typedef ElfW(Rela) rel_t;
class plain_reloc_iterator {

public:
    plain_reloc_iterator(rel_t* rel_array, size_t count)
            : begin_(rel_array), end_(begin_ + count), current_(begin_) {}

    bool has_next() {
        return current_ < end_;
    }

    rel_t* next() {
        return current_++;
    }
public:
    rel_t* const begin_;
    rel_t* const end_;
    rel_t* current_;


};




size_t phdr_table_get_load_size(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                ElfW(Addr)* out_min_vaddr) {
    ElfW(Addr) min_vaddr = UINTPTR_MAX;
    ElfW(Addr) max_vaddr = 0;

    bool found_pt_load = false;
    for (size_t i = 0; i < phdr_count; ++i) {
        const ElfW(Phdr)* phdr = &phdr_table[i];

        if (phdr->p_type != PT_LOAD) {
            continue;
        }
        found_pt_load = true;

        if (phdr->p_vaddr < min_vaddr) {
            min_vaddr = phdr->p_vaddr;
        }

        if (phdr->p_vaddr + phdr->p_memsz > max_vaddr) {
            max_vaddr = phdr->p_vaddr + phdr->p_memsz;
        }
    }
    if (!found_pt_load) {
        min_vaddr = 0;
    }

    min_vaddr = PAGE_START(min_vaddr);
    max_vaddr = PAGE_END(max_vaddr);

    if (out_min_vaddr != nullptr) {
        *out_min_vaddr = min_vaddr;
    }

    return max_vaddr - min_vaddr;
}

class load {
public:
    const char *name_;
    int fd_;

    ElfW(Ehdr) header_;
    size_t phdr_num_;

    void *phdr_mmap_;
    ElfW(Phdr) *phdr_table_;
    ElfW(Addr) phdr_size_;


    void *load_start_;
    // Size in bytes of reserved address space.
    size_t load_size_;
// Load bias.
    ElfW(Addr) load_bias_;
// Loaded phdr.
    const ElfW(Phdr) *loaded_phdr_;
    void *st;
    char *path;
public:


    load(void *sta)
            : phdr_num_(0), phdr_mmap_(NULL), phdr_table_(NULL), phdr_size_(0),
              load_start_(NULL), load_size_(0), load_bias_(0),
              loaded_phdr_(NULL), st(sta) {
    }

    bool ReadProgramHeader() {
        phdr_num_ = header_.e_phnum;
        ElfW(Addr) page_min = PAGE_START(header_.e_phoff);
        ElfW(Addr) page_max = PAGE_END(header_.e_phoff + (phdr_num_ * sizeof(ElfW(Phdr))));
        ElfW(Addr) page_offset = PAGE_OFFSET(header_.e_phoff);
        void **c = reinterpret_cast<void **>((char *) (st) + page_min);


        phdr_table_ = reinterpret_cast<ElfW(Phdr) *>(reinterpret_cast<char *>(c) +
                                                     page_offset);
        return true;
    };

    bool ReserveAddressSpace() {

        ElfW(Addr) min_vaddr;
        load_size_ = phdr_table_get_load_size(phdr_table_, phdr_num_, &min_vaddr);

        uint8_t *addr = reinterpret_cast<uint8_t *>(min_vaddr);
        void *start;

        int mmap_flags = MAP_PRIVATE | MAP_ANONYMOUS;



        start = start = mmap(nullptr, load_size_, PROT_NONE, mmap_flags, -1, 0);


        load_start_ = start;
        load_bias_ = reinterpret_cast<uint8_t *>(load_start_) - addr;
        __android_log_print(6,"r0ysue","%p %p111111  %x",load_start_,load_bias_,addr);
        return true;

    };

    bool LoadSegments() {
        for (size_t i = 0; i < phdr_num_; ++i) {
            const ElfW(Phdr) *phdr = &phdr_table_[i];
            if (phdr->p_type != PT_LOAD) {
                continue;
            }
            // Segment addresses in memory.
            ElfW(Addr) seg_start = phdr->p_vaddr + load_bias_;
            ElfW(Addr) seg_end = seg_start + phdr->p_memsz;
            ElfW(Addr) seg_page_start = PAGE_START(seg_start);
            ElfW(Addr) seg_page_end = PAGE_END(seg_end);
            ElfW(Addr) seg_file_end = seg_start + phdr->p_filesz;
            // File offsets.
            ElfW(Addr) file_start = phdr->p_offset;
            ElfW(Addr) file_end = file_start + phdr->p_filesz;

            ElfW(Addr) file_page_start = PAGE_START(file_start);
            ElfW(Addr) file_length = file_end - file_page_start;
            long* pp= reinterpret_cast<long *>(seg_page_start);
            __android_log_print(6,"r0ysue","%p 111111",load_bias_);
            __android_log_print(6,"r0ysue","%p 111111",seg_page_end);
            mprotect(reinterpret_cast<void *>(seg_page_start), seg_page_end-seg_page_start, PROT_WRITE);

            if (file_length != 0) {
                void* c=(char*)st+file_page_start;

                memcpy(reinterpret_cast<void *>(seg_page_start), c, file_length);
            }
            if ((phdr->p_flags & PF_W) != 0 && PAGE_OFFSET(seg_file_end) > 0) {
                memset(reinterpret_cast<void*>(seg_file_end), 0, PAGE_SIZE - PAGE_OFFSET(seg_file_end));
            }



//
            seg_file_end = PAGE_END(seg_file_end);

            if (seg_page_end > seg_file_end) {
                void* zeromap = mmap(reinterpret_cast<void*>(seg_file_end),
                                     seg_page_end - seg_file_end,
                                     PFLAGS_TO_PROT(phdr->p_flags),
                                     MAP_FIXED|MAP_ANONYMOUS|MAP_PRIVATE,
                                     -1,
                                     0);
                __android_log_print(6,"r0ysue","duiqi %p ",zeromap);

            }
//            __android_log_print(6,"r0ysue","%p 111111",seg_file_end);




        }

        return true;
    };

    bool loadhead(){


     return   memcpy(&(header_),st,sizeof(header_));//赋值elf头


    }
    bool CheckPhdr(ElfW(Addr) loaded) {
        const ElfW(Phdr) *phdr_limit = phdr_table_ + phdr_num_;
        ElfW(Addr) loaded_end = loaded + (phdr_num_ * sizeof(ElfW(Phdr)));
        for (ElfW(Phdr) *phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
            if (phdr->p_type != PT_LOAD) {
                continue;
            }
            ElfW(Addr) seg_start = phdr->p_vaddr + load_bias_;
            ElfW(Addr) seg_end = phdr->p_filesz + seg_start;
            if (seg_start <= loaded && loaded_end <= seg_end) {
                loaded_phdr_ = reinterpret_cast<const ElfW(Phdr) *>(loaded);
                return true;
            }
        }

        return false;
    };





    bool FindPHPtr(){
        const ElfW(Phdr)* phdr_limit = phdr_table_ + phdr_num_;

        //如果段类型是 PT_PHDR, 那么我们就直接使用该段的地址.
        for (const ElfW(Phdr)* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
            if (phdr->p_type == PT_PHDR) {
                return CheckPhdr(load_bias_ + phdr->p_vaddr);
            }
        }

        //否则，我们就检查第一个可加载段。如果该段的文件偏移值为0，那么就表示它是以ELF头开始的，我们就可以通过它来找到程序头表加载到内存的地址(虽然过程有点繁琐)。
        for (const ElfW(Phdr)* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
            if (phdr->p_type == PT_LOAD) {
                if (phdr->p_offset == 0) {
                    ElfW(Addr)  elf_addr = load_bias_ + phdr->p_vaddr;
                    const ElfW(Ehdr)* ehdr = reinterpret_cast<const ElfW(Ehdr)*>(elf_addr);
                    ElfW(Addr)  offset = ehdr->e_phoff;
                    return CheckPhdr((ElfW(Addr))ehdr + offset);
                }
                break;
            }
        }


        return false;

    };


};

static ElfW(Addr) get_addend(ElfW(Rela)* rela, ElfW(Addr) reloc_addr __unused) {
    return rela->r_addend;
}
const char* get_string(ElfW(Word) index,char * strtab_)  {
    return strtab_ + index;

}

void myloader();
_QWORD * getsoinfo(unsigned __int64 a1,void* base);
extern "C" JNIEXPORT jstring JNICALL
Java_com_roysue_elfso_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";




    myloader();
    return env->NewStringUTF(hello.c_str());
}



void myloader(){



    int fd;
    void *start;
    struct stat sb;
    fd = open("/data/local/tmp/1.so", O_RDONLY); /*打开/etc/passwd */
    fstat(fd, &sb); /* 取得文件大小 */
    start = static_cast<void **>(mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
    load a(start);
//    memcpy(&(a.header_),start,sizeof(a.header_));

    __android_log_print(6,"r0ysue","duiqi %x ", a.loadhead());

    __android_log_print(6,"r0ysue","duiqi %x ", a.ReadProgramHeader());
    __android_log_print(6,"r0ysue","duiqi %x ", a.ReserveAddressSpace());
    __android_log_print(6,"r0ysue","duiqi %x ",  a.LoadSegments());
    __android_log_print(6,"r0ysue","duiqi %x ",a.FindPHPtr());




//
//    soinfo si("myso",&sb,0,RTLD_NOW);
////
//    si.base = reinterpret_cast<ElfW(Addr)>(a.load_start_);
//    si.size = a.load_size_;
//    si.load_bias =reinterpret_cast<ElfW(Addr)>( a.load_bias_);
//    si.phnum = a.phdr_num_;
//    si.phdr = a.loaded_phdr_;
//    si.prelink_image();
//    si.call_constructors();



    void* ax=dlopen("libnative-lib.so",RTLD_NOW);
    __android_log_print(6,"r0ysue","%s",strerror(errno));
    char line[1024];
    int *startr;
    int *end;
    int n=1;
    FILE *fp=fopen("/proc/self/maps","r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "linker64") ) {
            __android_log_print(6,"r0ysue","%s", line);
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


//mprotect(startr,0x10000,PROT_WRITE|PROT_READ);

    void** old_soinfo= reinterpret_cast<void **>(getsoinfo((unsigned __int64) ax, startr));
    _QWORD *  base= reinterpret_cast<uint64 *>((char *) old_soinfo + 56);
    Elf64_Ehdr aa;
void* as= mmap(reinterpret_cast<void *>(a.load_bias_), sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    memcpy(&aa,start,sizeof(Elf64_Ehdr));
    int secoff= aa.e_shoff;
    int secsnum=aa.e_shnum;
    Elf64_Shdr bb;
    Elf64_Phdr cc;

    memcpy (&bb,((char*)(as)+aa.e_shoff+aa.e_shstrndx*aa.e_shentsize),sizeof(Elf64_Shdr));
    memcpy (&cc,((char*)(start)+aa.e_phoff),sizeof(Elf64_Phdr));

    char secstr[1024];
    char dynstr[10240];
    int needed=0;
    int  mynedd[20];

    memcpy(&secstr,(char*)(start)+bb.sh_offset,bb.sh_size);
    mprotect((void*)PAGE_START((ElfW(Addr))((char *)start)),a.load_size_,PROT_WRITE|PROT_READ|PROT_EXEC);
    __android_log_print(6,"r0ysue","size %s",strerror(errno));
    __android_log_print(6,"r0ysue","start %p",(void*)PAGE_START((ElfW(Addr))((char *)start)));

    mprotect((void*)PAGE_START((ElfW(Addr))((char *)old_soinfo + 16)),300,PROT_WRITE|PROT_READ);
    *reinterpret_cast<uint64 *>((char *) old_soinfo + 16) = reinterpret_cast<uint64>(a.load_start_);
    *(int*)((char*)(old_soinfo)+24)= a.load_size_;
    *reinterpret_cast<uint64 *>((char *) old_soinfo + 256) = reinterpret_cast<uint64>(start);
    *(int*)((char*)(old_soinfo)+8) = a.phdr_num_;
    *reinterpret_cast<uint64 *>((char *) old_soinfo )= (uint64) a.loaded_phdr_;


    for(int y=0;y<aa.e_phnum;y++){
        memcpy(&cc, (char *) (start) +aa.e_phoff+sizeof(Elf64_Phdr) * y, sizeof(Elf64_Phdr));

        if(cc.p_type==2){

            Elf64_Dyn dd;
            for(y=0;y==0||dd.d_tag!=0;y++){
                memcpy(&dd,(char *) (start) +cc.p_offset+y*sizeof(Elf64_Dyn),sizeof(Elf64_Dyn));
                __android_log_print(6,"r0ysue","nbucket_ %x",dd.d_tag);
//               if(dd.d_tag==4){
//                   uint32_t nbucket_ = reinterpret_cast<uint32_t*>((char*)(start)+ dd.d_un.d_ptr)[0];
//                   uint32_t  nchain_ = reinterpret_cast<uint32_t*>((char*)(start) + dd.d_un.d_ptr)[1];
//                   uint32_t *   bucket_ = reinterpret_cast<uint32_t*>((char*)(start)+ dd.d_un.d_ptr + 8);
//                   uint32_t * chain_ = reinterpret_cast<uint32_t*>((char*)(start) + dd.d_un.d_ptr + 8 + nbucket_ * 4);
//                   *reinterpret_cast<uint32_t *>((char *) old_soinfo + 72) = nbucket_;
//                   *reinterpret_cast<uint32_t *>((char *) old_soinfo + 80) = nchain_;
//                   *reinterpret_cast<uint32_t **>((char *) old_soinfo + 88) = static_cast<uint32_t *>((bucket_));
//                   *reinterpret_cast<uint32_t **>((char *) old_soinfo + 96) = static_cast<uint32_t *>((chain_));
//                   __android_log_print(6,"r0ysue","nbucket_ %p",nbucket_);
//                   __android_log_print(6,"r0ysue","nchain_ %p",nchain_);
//                   __android_log_print(6,"r0ysue","bucket_ %p",*bucket_);
//                   __android_log_print(6,"r0ysue","chain_ %p",*chain_);
//
//               }
               if(dd.d_tag==0x6ffffef5 ){
                   size_t   gnu_nbucket_ = reinterpret_cast<uint32_t*>((char*)start + dd.d_un.d_ptr)[0];
                   // skip symndx
                   uint32_t     gnu_maskwords_ = reinterpret_cast<uint32_t*>((char*)start + dd.d_un.d_ptr)[2];
                   uint32_t  gnu_shift2_ = reinterpret_cast<uint32_t*>((char*)start + dd.d_un.d_ptr)[3];

                   ElfW(Addr)*  gnu_bloom_filter_ = reinterpret_cast<ElfW(Addr)*>((char*)start + dd.d_un.d_ptr + 16);
                   uint32_t*  gnu_bucket_ = reinterpret_cast<uint32_t*>(gnu_bloom_filter_ + gnu_maskwords_);
                   // amend chain for symndx = header[1]
                   uint32_t*  gnu_chain_ = reinterpret_cast<uint32_t *>( gnu_bucket_ +
                                                                        gnu_nbucket_-reinterpret_cast<uint32_t *>(
                                                                                (char *) start +
                                                                                dd.d_un.d_ptr)[1]);



                   --gnu_maskwords_;
                   uint32_t  flags_ = FLAG_GNU_HASH|flags_;

//                   size_t gnu_nbucket_;
//                   310  uint32_t* gnu_bucket_;
//                   311  uint32_t* gnu_chain_;
//                   312  uint32_t gnu_maskwords_;
//                   313  uint32_t gnu_shift2_;
//                   314  ElfW(Addr)* gnu_bloom_filter_;
//



                   *reinterpret_cast<size_t *>((char *) old_soinfo + 344) = gnu_nbucket_;
                   *reinterpret_cast<uint32_t *>((char *) old_soinfo + 368) = gnu_maskwords_;
                   *reinterpret_cast<uint32_t *>((char *) old_soinfo + 372) = gnu_shift2_;
                   *reinterpret_cast<  ElfW(Addr)* *>((char *) old_soinfo +  376) = gnu_bloom_filter_;
                   *reinterpret_cast<uint32_t **>((char *) old_soinfo + 352) = gnu_bucket_;
                   *reinterpret_cast<uint32_t **>((char *) old_soinfo + 360) = gnu_chain_;
                   *reinterpret_cast<uint32_t *>((char *) old_soinfo + 48) = *reinterpret_cast<uint32_t *>((char *) old_soinfo + 48) |FLAG_GNU_HASH;
                   __android_log_print(6,"r0ysue","gnu_nbucket_ %p",gnu_nbucket_);
                   __android_log_print(6,"r0ysue","gnu_maskwords_ %p",gnu_maskwords_);
                   __android_log_print(6,"r0ysue","gnu_shift2_ %p",gnu_shift2_);
                   __android_log_print(6,"r0ysue","gnu_bloom_filter_ %p",gnu_bloom_filter_ );
                   __android_log_print(6,"r0ysue","gnu_bucket_ %p",gnu_bucket_);
                   __android_log_print(6,"r0ysue","gnu_chain_ %p",gnu_chain_);

               }
                if(dd.d_tag==2 ){
                   *reinterpret_cast<uint64 *>((char *) old_soinfo + 48)=dd.d_un.d_val /sizeof(ElfW(Rela));
                }
                if(dd.d_tag==0x17 ){
                    *reinterpret_cast<uint64 *>((char *) old_soinfo + 104)= reinterpret_cast<uint64>(
                            (char *) start + dd.d_un.d_ptr);
                }
                if(dd.d_tag==7){
                    *reinterpret_cast<uint64 *>((char *) old_soinfo + 120)= reinterpret_cast<uint64>(
                            (char *) start + dd.d_un.d_ptr);
                }
                if(dd.d_tag==5){
                    *reinterpret_cast<char **>((char *) old_soinfo + 56) = reinterpret_cast< char*>((char *) start+dd.d_un.d_ptr);
                }
                if(dd.d_tag==6){
                    *reinterpret_cast<uint64 *>((char *) old_soinfo + 64) = reinterpret_cast<uint64>(
                            (char *) start + dd.d_un.d_ptr);
                }
                if(dd.d_tag==10){
                    *reinterpret_cast<uint64 *>((char *) old_soinfo + 336) = reinterpret_cast<uint64>(
                            (char *) start + dd.d_un.d_ptr);
                }
                if(dd.d_tag==8){
                    *reinterpret_cast<uint64 *>((char *) old_soinfo + 128) =  dd.d_un.d_val / sizeof(ElfW(Rela));
                }
                if(dd.d_tag==1){
                    mynedd[needed]=dd.d_un.d_val;
                    needed++;

                }

                if(dd.d_tag==0x6ffffff0){
                    *reinterpret_cast<uint64 *>((char *) old_soinfo + 440) =  reinterpret_cast<uint64 >((char*)start + dd.d_un.d_ptr);
                }
                if(dd.d_tag==0x6fffffff){
                    *reinterpret_cast<uint64 *>((char *) old_soinfo + 472) =  dd.d_un.d_val;
                }

                if(dd.d_tag==0x6ffffffe){
                    *reinterpret_cast<uint64 *>((char *) old_soinfo + 464) = reinterpret_cast<uint64>(
                            (char *) start + dd.d_un.d_ptr);
                }



            }

        }


    }



    char* strtab_= *reinterpret_cast<char **>((char *) old_soinfo + 56) ;
    Elf64_Sym* symtab_= *reinterpret_cast<Elf64_Sym **>((char *) old_soinfo + 64);
    plain_reloc_iterator myit(
            reinterpret_cast<rel_t *>(*reinterpret_cast<uint64 *>(
                    (char *) old_soinfo + 104)), *reinterpret_cast<size_t *>((char *) old_soinfo + 48));
    __android_log_print(6,"r0ysue","finish xxx%x",*reinterpret_cast<size_t *>((char *) old_soinfo + 48));






#define ELFW(what) ELF64_ ## what

#define R_TYPE(sym) ((((Elf64_Xword)sym) << 32)
#define R_SYM(type) ((type) & 0xffffffff))


    for (size_t idx = 0; myit.has_next(); ++idx) {
        const auto rel = myit.next();


        ElfW(Word) type = ELFW(R_TYPE)(rel->r_info);
        ElfW(Word) sym = ELFW(R_SYM)(rel->r_info);

        ElfW(Addr) reloc = static_cast<ElfW(Addr)>(rel->r_offset + a.load_bias_);

        ElfW(Addr) sym_addr = 0;
        const char *sym_name = nullptr;


        const Elf64_Sym *s = nullptr;
        if (type == 0) {
            continue;
        }
        sym_name = reinterpret_cast<const char *>(strtab_+symtab_[sym].st_name);


        for(int s=0;s<needed;s++) {
            void* handle=dlopen(strtab_ + mynedd[s],RTLD_NOW);
            sym_addr= reinterpret_cast<Elf64_Addr>(dlsym(handle, sym_name));
            if(sym_addr==0)
                continue;
            else
//            __android_log_print(6, "r0ysue", "finish xxwwwwwwwwwwwwwwwx%p %s", sym_addr,sym_name);
break;
        }

        switch (type) {
            case 1026:
                *reinterpret_cast<uint64 *>((char *) start+ rel->r_offset)  = (sym_addr );
                break;

        }

    }




//
//    for(int y=0;y<secsnum;y++){
//        memcpy(&bb, (char *) (start) +aa.e_shoff+sizeof(Elf64_Shdr) * y, sizeof(Elf64_Shdr));
//        if(strcmp((char*)(secstr+bb.sh_name), ".dynstr") == 0)
//        {
//            memcpy(&dynstr,(char*)(start)+bb.sh_offset,bb.sh_size);
//        *reinterpret_cast<uint64 *>((char *) old_soinfo + 56) = reinterpret_cast<uint64>((char *) (start)+bb.sh_offset);
//            __android_log_print(6,"r0ysue","dynstr %p",(char *) (start)+bb.sh_offset);
//        }
//        if(strcmp((char*)(secstr+bb.sh_name), ".dynsym") == 0) {
//            *reinterpret_cast<uint64 *>((char *) old_soinfo + 64) = reinterpret_cast<uint64>((char *) (start)+bb.sh_offset);
//            __android_log_print(6,"r0ysue","dynstr %p",(char *) (start)+bb.sh_offset);
//        }
//
//    }




    __android_log_print(6,"r0ysue","finish xxx");
//    int size=*(int*)((char*)(old_soinfo)+24);
//    int phnum=*(int*)((char*)(old_soinfo)+8);
//    _QWORD *  mybase= reinterpret_cast<uint64 *>((char *) old_soinfo + 256);
//    _QWORD *  phdr= reinterpret_cast<uint64 *>((char *) old_soinfo );

//    a1+72=nbucket_;
//    a1+80=nchain_;
//    a1+88=bucket_;
//    a1+96=chain_;




}


_QWORD * getsoinfo(unsigned __int64 a1,void* base){
    unsigned int v2; // w19
    unsigned __int64 v3; // x11
    __int64 v4; // x9
    __int64 v5; // x10
    _QWORD *v6; // x12
    uint64 *bas1e= reinterpret_cast<uint64 *>((char *) base + 0xFD468);
    uint64 *bas2= reinterpret_cast<uint64 *>((char *) base + 0xFD460);
    _QWORD qword_FD468=*bas1e;
    _QWORD _dl_g_soinfo_handles_map=*bas2;
    unsigned __int64 v7; // x13
    __int64 v8; // x20
    __int64 v9; // x0
    __int64 v11; // [xsp+0h] [xbp-20h] BYREF
    char v12[8]; // [xsp+8h] [xbp-18h] BYREF
    if ( (a1 & 1) != 0 )
    {
        if ( qword_FD468 )
        {
            v3 = a1 - a1 / qword_FD468 * qword_FD468;
            v4 = qword_FD468 - 1;
            v5 = (qword_FD468 - 1) & qword_FD468;
            if ( qword_FD468 > a1 )
                v3 = a1;
            if ( !v5 )
                v3 = v4 & a1;
            v6 = *(_QWORD **)(_dl_g_soinfo_handles_map + 8 * v3);
            if ( v6 )
            {
                while ( 1 )
                {
                    v6 = (_QWORD *)*v6;
                    if ( !v6 )
                        break;
                    v7 = v6[1];
                    if ( v7 == a1 )
                    {
                        if ( v6[2] == a1 )
                        {
                            if ( v6[3] )

                                break;
                        }
                    }
                    else
                    {
                        if ( v5 )
                        {
                            if ( v7 >= qword_FD468 )
                                v7 -= v7 / qword_FD468 * qword_FD468;
                        }
                        else
                        {
                            v7 &= v4;
                        }
                        if ( v7 != v3 )
                            break;
                    }
                }
            }
        }
    }
    _QWORD * st= reinterpret_cast<uint64 *>((char *) (v6[3]) );
    return st;

}

extern "C"
JNIEXPORT jint JNICALL
Java_com_roysue_elfso_MainActivity_dddddd(JNIEnv *env, jobject thiz, jint s) {
    return 1;
}
//extern "C"
//JNIEXPORT jint JNICALL
//Java_com_roysue_elfso_MainActivity_add(JNIEnv *env, jobject thiz, jint a, jint b) {
////    printf("%dsadsadsa");
////    __android_log_print(6,"r0ysue","qqqqqqqqqqqqqqqqqqqqqqqqqqq");
//    return 1+1;
//}