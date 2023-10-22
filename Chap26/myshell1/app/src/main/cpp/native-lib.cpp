#include <jni.h>
#include <string>
#include <unistd.h>
#include "dlfcn.h"
#include "android/log.h"
#include "sys/stat.h"
#include "sys/mman.h"
#include "fcntl.h"
#include "elf.h"
#include "errno.h"
 #define PAGE_START(x) ((x) & PAGE_MASK)
 #define PAGE_OFFSET(x) ((x) & ~PAGE_MASK)
 #define PAGE_END(x) PAGE_START((x) + (PAGE_SIZE-1))

#define MAYBE_MAP_FLAG(x, from, to)  (((x) & (from)) ? (to) : 0)
#define PFLAGS_TO_PROT(x)            (MAYBE_MAP_FLAG((x), PF_X, PROT_EXEC) | \
                                   MAYBE_MAP_FLAG((x), PF_R, PROT_READ) | \
                                    MAYBE_MAP_FLAG((x), PF_W, PROT_WRITE))

constexpr unsigned kLibraryAlignmentBits = 18;
constexpr size_t kLibraryAlignment = 1UL << kLibraryAlignmentBits;



void dump(void* begin,int size){
    int fd=open("/sdcard/mydump.so",O_CREAT|O_WRONLY,0777);
    write(fd,begin,0x40000);
    close(fd);

    __android_log_print(6,"r0ysue","i dump success %p %x %s",begin,size,strerror(errno));

}
size_t phdr_table_get_load_size(const Elf64_Phdr* phdr_table, size_t phdr_count,
        Elf64_Addr* out_min_vaddr,
Elf64_Addr* out_max_vaddr) {
    Elf64_Addr min_vaddr = UINTPTR_MAX;
    Elf64_Addr max_vaddr = 0;

bool found_pt_load = false;
for (size_t i = 0; i < phdr_count; ++i) {
const Elf64_Phdr* phdr = &phdr_table[i];

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
if (out_max_vaddr != nullptr) {
*out_max_vaddr = max_vaddr;
}
return max_vaddr - min_vaddr;
}



static constexpr uintptr_t align_down(uintptr_t p, size_t align) {
    return p & ~(align - 1);
    }

 static constexpr uintptr_t align_up(uintptr_t p, size_t align) {
    return (p + align - 1) & ~(align - 1);
 }
template <typename T>
static inline T* align_down(T* p, size_t align) {
      return reinterpret_cast<T*>(align_down(reinterpret_cast<uintptr_t>(p), align));
    }

 template <typename T>
 static inline T* align_up(T* p, size_t align) {
    return reinterpret_cast<T*>(align_up(reinterpret_cast<uintptr_t>(p), align));
  }


static void* ReserveAligned(void* hint, size_t size, size_t align) {
    int mmap_flags = MAP_PRIVATE | MAP_ANONYMOUS;
    if (align == PAGE_SIZE || hint != nullptr) {
        void* mmap_ptr = mmap(hint, size, PROT_NONE, mmap_flags, -1, 0);
        if (mmap_ptr == MAP_FAILED) {
            return nullptr;
        }
        return mmap_ptr;
    }

    // Allocate enough space so that the end of the desired region aligned up is still inside the
    // mapping.
    size_t mmap_size = align_up(size, align) + align - PAGE_SIZE;
    uint8_t* mmap_ptr =
            reinterpret_cast<uint8_t*>(mmap(nullptr, mmap_size, PROT_NONE, mmap_flags, -1, 0));
    if (mmap_ptr == MAP_FAILED) {
        return nullptr;
    }

    uint8_t* first = align_up(mmap_ptr, align);
    uint8_t* last = align_down(mmap_ptr + mmap_size, align) - size;
    size_t n = arc4random_uniform((last - first) / PAGE_SIZE + 1);
    uint8_t* start = first + n * PAGE_SIZE;
    munmap(mmap_ptr, start - mmap_ptr);
    munmap(start + size, mmap_ptr + mmap_size - (start + size));
    return start;
}



 class myread{

 public:
    void* mybegin;
    Elf64_Ehdr header;
     size_t phdr_num_;
     Elf64_Phdr* phdr_table_;
     Elf64_Shdr * shdr_table_;
     size_t shdr_num_;
     const Elf64_Dyn* dynamic_;
     const char* strtab_;
      size_t strtab_size_;
     size_t load_size_;
     void* load_start_;
     const Elf64_Phdr* loaded_phdr_;

  Elf64_Addr load_bias_;
     myread(void* mysostart){
         mybegin=mysostart;

     }

     bool CheckPhdr(Elf64_Addr loaded) {
         const Elf64_Phdr* phdr_limit = phdr_table_ + phdr_num_;
         Elf64_Addr loaded_end = loaded + (phdr_num_ * sizeof(Elf64_Phdr));
         for (const Elf64_Phdr* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
             if (phdr->p_type != PT_LOAD) {
                 continue;
             }
             Elf64_Addr seg_start = phdr->p_vaddr + load_bias_;
             Elf64_Addr seg_end = phdr->p_filesz + seg_start;
             if (seg_start <= loaded && loaded_end <= seg_end) {
                 loaded_phdr_ = reinterpret_cast<const Elf64_Phdr*>(loaded);
                 return true;
             }
         }

         return false;
     }

        void loadheader(){
            memcpy(&header,mybegin,sizeof(Elf64_Ehdr));
        }

     void loadphdr(){
       phdr_num_=header.e_phnum;

         phdr_table_= reinterpret_cast<Elf64_Phdr *>((char *) mybegin + header.e_phoff);

     }


     void loadDynamic(){
         shdr_num_ = header.e_shnum;
         shdr_table_= reinterpret_cast<Elf64_Shdr *>((char *) mybegin + header.e_shoff);
         const Elf64_Shdr* dynamic_shdr = nullptr;
         for (size_t i = 0; i < shdr_num_; ++i) {
             if (shdr_table_[i].sh_type == SHT_DYNAMIC) {
                 dynamic_shdr = &shdr_table_ [i];
                 break;
             }
         }

         size_t pt_dynamic_offset = 0;
         size_t pt_dynamic_filesz = 0;
         for (size_t i = 0; i < phdr_num_; ++i) {
             const Elf64_Phdr * phdr = &phdr_table_[i];
             if (phdr->p_type == PT_DYNAMIC) {
                 pt_dynamic_offset = phdr->p_offset;
                 pt_dynamic_filesz = phdr->p_filesz;
             }
         }

         const Elf64_Shdr * strtab_shdr = &shdr_table_[dynamic_shdr->sh_link];

         dynamic_ = reinterpret_cast<const Elf64_Dyn *>((char *) mybegin + pt_dynamic_offset);
         strtab_ = static_cast<const char*>((char *) mybegin + strtab_shdr->sh_offset);
         strtab_size_ =strtab_shdr->sh_size;

     }
     void ReserveAddressSpace(){
         Elf64_Addr min_vaddr;
         load_size_ = phdr_table_get_load_size(phdr_table_, phdr_num_, &min_vaddr, nullptr);
         uint8_t* addr = reinterpret_cast<uint8_t*>(min_vaddr);
         void* start;
         size_t reserved_size = 0;
         bool reserved_hint = true;
         bool strict_hint = false;
         void* mmap_hint = nullptr;
         start = ReserveAligned(mmap_hint, load_size_, kLibraryAlignment);
         load_start_ = start;
         load_bias_ = reinterpret_cast<uint8_t*>(start) - addr;
        
     }
     void loadsegment(){
         for (size_t i = 0; i < phdr_num_; ++i) {
             const Elf64_Phdr *phdr = &phdr_table_[i];
             if (phdr->p_type != PT_LOAD) {
                 continue;
             }
             Elf64_Addr seg_start = phdr->p_vaddr + load_bias_;
             Elf64_Addr seg_end   = seg_start + phdr->p_memsz;
             Elf64_Addr seg_page_start = PAGE_START(seg_start);
             Elf64_Addr seg_page_end   = PAGE_END(seg_end);
             Elf64_Addr seg_file_end   = seg_start + phdr->p_filesz;
             Elf64_Addr file_start = phdr->p_offset;
             Elf64_Addr file_end   = file_start + phdr->p_filesz;
             Elf64_Addr file_page_start = PAGE_START(file_start);
             Elf64_Addr file_length = file_end - file_page_start;
             int prot = PFLAGS_TO_PROT(phdr->p_flags);
//             void* seg_addr=malloc(PAGE_END(file_length));
             mprotect(reinterpret_cast<void*>(seg_page_start),PAGE_END(file_length),PROT_WRITE|PROT_READ|PROT_EXEC);
             __android_log_print(6,"r0ysue","%p %p %x",seg_page_start,load_start_,load_size_);
             memcpy(reinterpret_cast<void*>(seg_page_start),mybegin,file_length);



             if ((phdr->p_flags & PF_W) != 0 && PAGE_OFFSET(seg_file_end) > 0) {
                 memset(reinterpret_cast<void*>(seg_file_end), 0, PAGE_SIZE - PAGE_OFFSET(seg_file_end));
             }
             seg_file_end = PAGE_END(seg_file_end);

             if (seg_page_end > seg_file_end) {
                 size_t zeromap_size = seg_page_end - seg_file_end;
                 void* zeromap = mmap(reinterpret_cast<void*>(seg_file_end),
                                      zeromap_size,
                                      PFLAGS_TO_PROT(phdr->p_flags),
                                      MAP_FIXED|MAP_ANONYMOUS|MAP_PRIVATE,
                                      -1,
                                      0);



             }


         }




     }


     bool FindPhdr() {
         const Elf64_Phdr* phdr_limit = phdr_table_ + phdr_num_;

         // If there is a PT_PHDR, use it directly.
         for (const Elf64_Phdr* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
             if (phdr->p_type == PT_PHDR) {
                 return CheckPhdr(load_bias_ + phdr->p_vaddr);
             }
         }

         // Otherwise, check the first loadable segment. If its file offset
         // is 0, it starts with the ELF header, and we can trivially find the
         // loaded program header from it.
         for (const Elf64_Phdr* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
             if (phdr->p_type == PT_LOAD) {
                 if (phdr->p_offset == 0) {
                     Elf64_Addr  elf_addr = load_bias_ + phdr->p_vaddr;
                     const Elf64_Ehdr* ehdr = reinterpret_cast<const Elf64_Ehdr*>(elf_addr);
                     Elf64_Addr  offset = ehdr->e_phoff;
                     return CheckPhdr(reinterpret_cast<Elf64_Addr>(ehdr) + offset);
                 }
                 break;
             }
         }


         return false;
     }



};



typedef int (*sub)(int,int) ;
int add(int a,int b){


    return a+b;
}

void load(const char* soname){
    int fd;
    void *start;
    struct stat sb;
    fd = open(soname, O_RDONLY);
    fstat(fd, &sb);
    start = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    myread myso=myread(start);
    myso.loadheader();
    myso.loadphdr();
    myso.loadDynamic();
    myso.ReserveAddressSpace();
    myso.loadsegment();
    myso.FindPhdr();
    dump(myso.load_start_,myso.load_size_);


}


extern "C" int mysb(int u ){
    if(0x1000<u<0x5000)
        u=12;
    return u;



}


extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_myshell1_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    load("/data/local/tmp/chajian.so");

//    int a=add(1,2);
//    void* mychajianso=dlopen("/data/local/tmp/chajian.so",RTLD_NOW);
//    __android_log_print(6,"r0ysue","%p",mychajianso);
//    void* mysub=dlsym(mychajianso,"sub");
//    sub myfuncsub=(sub)mysub;
//    __android_log_print(6,"r0ysue","%p",myfuncsub(10,5));
    return env->NewStringUTF(hello.c_str());
}
