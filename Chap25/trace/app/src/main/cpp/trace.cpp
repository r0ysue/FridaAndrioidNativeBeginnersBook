//
// Created by root on 12/21/21.
//

#include <jni.h>
#include <string>
#include <stdint.h>
#include <unistd.h>
#include "capstone/capstone.h"
#include "android/log.h"
#include "sys/mman.h"
#include "findsym.h"
#include "sys/mman.h"
#include "pthread.h"
#include "fcntl.h"
#include "errno.h"
#include "findsobase.h"
#include "trace.h"
#define PAGE_START(x)  ((x) & PAGE_MASK)
#define PAGE_OFFSET(x) ((x) & ~PAGE_MASK)
#define PAGE_END(x)    PAGE_START((x) + (PAGE_SIZE-1))
FILE *fp;

long x[30]={0};
int change[30];
long x30ret;
long x30ret2;
void *caller_addr;
int isadrp;
int isbl;
long adrp;
long blad;
int istet;
int isx30;
int conti;
int isb;
int isb2;
long x30;
void *c;
int istb;
int index=0;
long address;
int isbr;
int mybr;
int isx18;
int isadrpx18;
int issp;




static void test(unsigned int yy, uint64_t address1) {

    isadrp = 0;
    isbl = 0;
    istet = 0;
    isx30 = 0;
    isb = 0;
    isb2 = 0;
    istb=0;
    isbr=0;
    isx18=0;
    issp=0;
    unsigned char *stc = static_cast<unsigned char *>(malloc(8));
    char code[40];
    memcpy(stc, &yy, 8);

    csh handle;
    cs_insn *insn;
    size_t count;
    cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle);
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
    count = cs_disasm(handle, stc, sizeof(stc) - 1, address1, 0, &insn);



    if (count) {


        sprintf(code, "%s %s", insn[0].mnemonic, insn[0].op_str);
//        __android_log_print(6,"r0ysue","%s",code);
        if(strcmp(insn[0].mnemonic,"br")==0){
            isbr=1;
            strtok(code, "x");
            int xs = strtoul(strtok(NULL, ""), NULL, 10);
            mybr=xs;
//            __android_log_print(6,"r0ysue","address : %p",address);
        }

        else if(strcmp(insn[0].mnemonic,"b")==0){
            isb=1;
            strtok(code, "#0x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
//            __android_log_print(6,"r0ysue","address : %p",address);
        }else  if(strcmp(insn[0].mnemonic,"bl")==0){
            isbl=1;
            strtok(code, "#0x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
//            __android_log_print(6,"r0ysue","address : %p",address);
        }else  if(strstr(insn[0].mnemonic,"b.")||strcmp(insn[0].mnemonic,"cbz")==0||strcmp(insn[0].mnemonic,"cbnz")==0){
            isb2=1;
            strtok(code, "#0x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
//            __android_log_print(6,"r0ysue","address : %p",address);
        }else  if(strcmp(insn[0].mnemonic,"tbz")==0||strcmp(insn[0].mnemonic,"tbnz")==0){
            istb=1;
            strtok(code, "#0x");
            strtok(NULL, "#0x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
//            __android_log_print(6,"r0ysue","address : %p",address);
        }
        else  if(strcmp(insn[0].mnemonic,"adrp")==0||strcmp(insn[0].mnemonic,"adr")==0||(yy>>24)==0x58){
//            if((yy>>24)==0x58)
//            __android_log_print(6,"r0ysue","%s %p %x",code,yy,yy&0x58000000);

            isadrp = 1;

            if(strstr(code,"x18")){

                isx18=1;
            }

            if(strstr(code,"x30")){

                isx30=1;
            }

            strtok(code, "x");
            strtok(NULL, "x");

            address = strtoul(strtok(NULL, ""), NULL, 16);


//            __android_log_print(6,"r0ysue","aaaaaaaaaaaaa : %p",address);
        }else  if(strstr(code,"sp, sp")){
            istet=1;

//            __android_log_print(6,"r0ysue","address : %s",code);
        }
//        else  if(strstr(code,"sp")){
//            issp=1;
//
//            __android_log_print(6,"r0ysue","address : %s",code);
//        }


        cs_free(insn, count);
    }
    cs_close(&handle);
    free(stc);
}


void jicunq(){
    int y[30];
    for(int n=0;n<30;n++){
        change[n]=0;
    }
    long* sp;
    __asm__("mov %[input_n], sp\r\n"
    :[result_m] "=r" (sp)
    :[input_n] "r" (sp)
    );
    sp= reinterpret_cast<long *>((char *) sp + 0x3b0-0x210);

    for(int n=0;n<30;n++){
        if(x[n]!=*(long*)(sp+n)&&n!=18){
            change[n]=1;
            x[n]=*(long*)(sp+n);
        }
    }
}

const char *test2_spr(const char *a, const char *b,long* address) {
    char code[200];
    sprintf(code, "%s %s  ", a, b);
    int tt=0;
    for(int n=0;n<30;n++){

        if(change[n]!=0&&n!=18&&tt<3){
            tt++;
            char add1[50];
            sprintf(add1," x%d = %p ",n,x[n]);
            strcat(code,add1);

        }

    }
    strcat(code,"\n");

//if(pt==1)
    __android_log_print(6, "r0ysue", "%s", code);
    fputs(code,fp);


    return code;
}

static void test2(unsigned int yy, uint64_t address1) {
    conti = 0;
    unsigned char *stc = static_cast<unsigned char *>(malloc(8));
    char code[40];
    memcpy(stc, &yy, 8);
    csh handle;
    cs_insn *insn;
    size_t count;
    cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle);
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
    count = cs_disasm(handle, stc, sizeof(stc) - 1, address, 0, &insn);
    if (count) {
        sprintf(code, "%s %s", insn[0].mnemonic, insn[0].op_str);


        jicunq();



        test2_spr(insn[0].mnemonic, insn[0].op_str, reinterpret_cast<long *>(address1));


        cs_free(insn, count);
    }
    cs_close(&handle);
    free(stc);
}


static void test3(unsigned int yy, uint64_t address1) {
    conti = 0;
    unsigned char *stc = static_cast<unsigned char *>(malloc(8));
    char code[40];
    memcpy(stc, &yy, 8);
    csh handle;
    cs_insn *insn;
    size_t count;
    cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle);
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
    count = cs_disasm(handle, stc, sizeof(stc) - 1, address, 0, &insn);
    if (count) {
        sprintf(code, "%s %s\n", insn[0].mnemonic, insn[0].op_str);

        __android_log_print(6,"r0ysue","%s",code);
        fputs(code,fp);

        cs_free(insn, count);
    }
    cs_close(&handle);
    free(stc);
}


extern "C" void myloadmethod1(void *a, void *b, void *c, void *d, void *e) {
    asm("sub SP, SP, #0x10000");
    asm("mrs x18, NZCV");
    asm("stp X0, X1, [SP,#0x0]");
    asm("stp X2, X3, [SP,#0x10]");
    asm("stp X4, X5, [SP,#0x20]");
    asm("stp X6, X7, [SP,#0x30]");
    asm("stp X8, X9, [SP,#0x40]");
    asm("stp X10, X11, [SP,#0x50]");
    asm("stp X12, X13, [SP,#0x60]");
    asm("stp X14, X15, [SP,#0x70]");
    asm("stp X16, X17, [SP,#0x80]");
    asm("stp X18, X19, [SP,#0x90]");
    asm("stp X20, X21, [SP,#0xa0]");
    asm("stp X22, X23, [SP,#0xb0]");
    asm("stp X24, X25, [SP,#0xc0]");
    asm("stp X26, X27, [SP,#0xd0]");
    asm("stp X28, X29, [SP,#0xe0]");
    asm("stp q0, q1, [SP,#0x160]");

    asm("sub SP, SP, #0x20");
    long *addr;
    __asm__("mov %[input_n], x30\r\n"
    :[result_m] "=r"(addr)
    :[input_n] "r"(addr)
    );
    int code = *(int *) ((char *) addr - 24);
    long address = reinterpret_cast<long>((char *) addr - 24);

    test2(code, address);
    asm("add SP, SP, #0x20");

    asm("ldp X0, X1, [SP,#0x0]");
    asm("ldp X2, X3, [SP,#0x10]");
    asm("ldp X4, X5, [SP,#0x20]");
    asm("ldp X6, X7, [SP,#0x30]");
    asm("ldp X8, X9, [SP,#0x40]");
    asm("ldp X10, X11, [SP,#0x50]");
    asm("ldp X12, X13, [SP,#0x60]");
    asm("ldp X14, X15, [SP,#0x70]");
    asm("ldp X16, X17, [SP,#0x80]");
    asm("ldp X18, X19, [SP,#0x90]");
    asm("ldp X20, X21, [SP,#0xa0]");
    asm("ldp X22, X23, [SP,#0xb0]");
    asm("ldp X24, X25, [SP,#0xc0]");
    asm("ldp X26, X27, [SP,#0xd0]");
    asm("ldp X28, X29, [SP,#0xe0]");
    asm("msr NZCV, x18 ");
    asm("ldp q0, q1, [SP,#0x160]");
//    asm("ldp X16, X17, [SP,#0x10]");




    asm("add SP, SP, #0x10000");

}






const char *adr_sprintf(int a, long b,long c) {

    char code[40];
    sprintf(code, "adrp x%d,#%p %p\n", a, b,c);
    __android_log_print(6, "r0ysue", "%s", code);
    fputs(code,fp);
    return code;



}

void print_adrp() {

    asm("sub SP, SP, #0x10000");
    asm("mrs x18, NZCV");
    asm("stp X0, X1, [SP,#0x0]");
    asm("stp X2, X3, [SP,#0x10]");
    asm("stp X4, X5, [SP,#0x20]");
    asm("stp X6, X7, [SP,#0x30]");
    asm("stp X8, X9, [SP,#0x40]");
    asm("stp X10, X11, [SP,#0x50]");
    asm("stp X12, X13, [SP,#0x60]");
    asm("stp X14, X15, [SP,#0x70]");
    asm("stp X16, X17, [SP,#0x80]");
    asm("stp X18, X19, [SP,#0x90]");
    asm("stp X20, X21, [SP,#0x100]");
    asm("stp X22, X23, [SP,#0x110]");
    asm("stp X24, X25, [SP,#0x120]");
    asm("stp X26, X27, [SP,#0x130]");
    asm("stp X28, X29, [SP,#0x140]");
    asm("stp q0, q1, [SP,#0x160]");

    asm("sub SP, SP, #0x20");

    long *addr;
    __asm__("mov %[input_n], x30\r\n"
    :[result_m] "=r"(addr)
    :[input_n] "r"(addr)
    );
    long addr1 = (*(long *) ((char *) addr - 40)) & 0x1f;
    long addr2 = (*(long *) ((char *) addr - 32));

    adr_sprintf(addr1, addr2, reinterpret_cast<long>(addr));
    asm("add SP, SP, #0x20");

    asm("ldp X0, X1, [SP,#0x0]");
    asm("ldp X2, X3, [SP,#0x10]");
    asm("ldp X4, X5, [SP,#0x20]");
    asm("ldp X6, X7, [SP,#0x30]");
    asm("ldp X8, X9, [SP,#0x40]");
    asm("ldp X10, X11, [SP,#0x50]");
    asm("ldp X12, X13, [SP,#0x60]");
    asm("ldp X14, X15, [SP,#0x70]");
    asm("ldp X16, X17, [SP,#0x80]");
    asm("ldp X18, X19, [SP,#0x90]");
    asm("ldp X20, X21, [SP,#0x100]");
    asm("ldp X22, X23, [SP,#0x110]");
    asm("ldp X24, X25, [SP,#0x120]");
    asm("ldp X26, X27, [SP,#0x130]");
    asm("ldp X28, X29, [SP,#0x140]");
    asm("msr NZCV, x18 ");
    asm("ldp q0, q1, [SP,#0x160]");
//    asm("ldp X16, X17, [SP,#0x10]");




    asm("add SP, SP, #0x10000");


}

void print_b1( int a,int b,long c){
    char code[40];
    if(b==1)
        sprintf(code,"b 0x%x\n",c);
    else
        sprintf(code,"b -0x%x\n",c);

    __android_log_print(6,"r0ysue","%s",code);
    fputs(code,fp);
}

void b1(void *a, void *b, void *c, void *d, void *e) {
    asm("sub SP, SP, #0x10000");
    asm("mrs x18, NZCV");
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
    asm("sub SP, SP, #0x20");
    long *addr;
    __asm__("mov %[input_n], x30\r\n"
    :[result_m] "=r"(addr)
    :[input_n] "r"(addr)
    );
    int code = *(int *) ((char *) addr +12);
    long address = reinterpret_cast<long>((char *) addr + 12);
    int flag=0;
    int off=0;
    if ((code & 0x17000000) == 0x17000000) {
        off = (unsigned int) 0x18000000 - (unsigned int) code;

    } else {
        off = ((unsigned int) code) & 0xfffff;
        flag=1;

    }

    print_b1(off,flag,address);
    asm("add SP, SP, #0x20");
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
    asm("msr NZCV, x18 ");
//    asm("ldp X16, X17, [SP,#0x10]");
    asm("ldp X0, X1, [SP,#0x20]");
    asm("ldp X2, X3, [SP,#0x30]");
    asm("ldp X4, X5, [SP,#0x40]");
    asm("ldp X6, X7, [SP,#0x50]");
    asm("ldp X29, X30, [SP,#0x10]");




    asm("add SP, SP, #0x10000");

}

void print_bl(long address){
    char code[50];
    __android_log_print(6,"r0ysue","bl %p",address);
sprintf(code,"bl %p\n",address);
    fputs(code,fp);


}

void bl(void *a, void *b, void *c, void *d, void *e) {
    asm("sub SP, SP, #0x10000");
    asm("mrs x18, NZCV");
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
    asm("sub SP, SP, #0x20");
    long *addr;
    __asm__("mov %[input_n], x30\r\n"
    :[result_m] "=r"(addr)
    :[input_n] "r"(addr)
    );

    long address = *reinterpret_cast<long*>((char *) addr - 24);

    print_bl(address);
    asm("add SP, SP, #0x20");
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
    asm("msr NZCV, x18 ");
//    asm("ldp X16, X17, [SP,#0x10]");
    asm("ldp X0, X1, [SP,#0x20]");
    asm("ldp X2, X3, [SP,#0x30]");
    asm("ldp X4, X5, [SP,#0x40]");
    asm("ldp X6, X7, [SP,#0x50]");
    asm("ldp X29, X30, [SP,#0x10]");




    asm("add SP, SP, #0x10000");

}


void print_b2(int code,long address){
    test2(code,address);
}


void b2(void *a, void *b, void *c, void *d, void *e) {
    asm("sub SP, SP, #0x10000");
    asm("mrs x18, NZCV");
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
    asm("sub SP, SP, #0x20");
    long *addr;
    __asm__("mov %[input_n], x30\r\n"
    :[result_m] "=r"(addr)
    :[input_n] "r"(addr)
    );
    int code = *(int *) ((char *) addr +12);
    long address = reinterpret_cast<long>((char *) addr + 12);


    test3(code,address);
    asm("add SP, SP, #0x20");
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
    asm("msr NZCV, x18 ");
//    asm("ldp X16, X17, [SP,#0x10]");
    asm("ldp X0, X1, [SP,#0x20]");
    asm("ldp X2, X3, [SP,#0x30]");
    asm("ldp X4, X5, [SP,#0x40]");
    asm("ldp X6, X7, [SP,#0x50]");
    asm("ldp X29, X30, [SP,#0x10]");




    asm("add SP, SP, #0x10000");

}



void mydealbr(){
    asm("sub SP, SP, #0x10000");
    asm("stp X16, X17, [SP,#0x10]");
    asm("ldr x16,12");
    asm("sub x18,x18,x16");
    asm("b 12");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x16,14");
    asm("mul x18,x18,x16");
    asm("ldr x16,12");
    asm("add x18,x16,x18");
    asm("b 12");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("ldp X16, X17, [SP,#0x10]");
    asm("add SP, SP, #0x10000");


}


void* deal_with(void* begin,int size,const char* path){
//    fp = fp1;
    fp=fopen(path,"wt+");
    void* myx30=malloc(16);
    void* myx18=malloc(16);
    void* c = malloc(size * 0x15);
    //malloc memory for myreplacefunction
    int mod = (size * 0x15) / 0x1000+1;
    mprotect((void *) PAGE_START((long) c), mod * PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);

//long size=(long)end-(long)start;
//    memcpy(c, begin, size);
    for (int n = 0; n < size * 14; n = n + 56) {
        int code = *(int *) ((char *)begin+ n / 14);
        test(code, reinterpret_cast<uint64_t>((char *) begin + n / 14));

        if(n==0 ) {

//we must deal with x30 when we go into function first
            *(int *) ((char *) c + n )=code;
            //old code
            *(int *) ((char *) c + n+4 ) = 0xd1400bff;//sub sp,sp,0x2000 for malloc Stack
            *(int *) ((char *) (c) + n +8) = 0xa91f7bf2;//stp x18,x30 ,[sp,0x1f0] we must save x18 and x30 because of using x18 and x30 for
             *(int *) ((char *) (c) + n + 12) = 0x91400bff;//add


            *(int *) ((char *) (c) + n + 16) = 0x58000072;
            *(int *) ((char *) (c) + n + 20) = 0xd63f0240;
            *(int *) ((char *) (c) + n + 24) = 0x14000003;
            *(long *) ((char *) (c) + n + 28) = reinterpret_cast<long >(myloadmethod1);;

            *(int *) ((char *) (c) + n + 36) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 40) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 44) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 48) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 52) = 0xaa0003e0;


//            *(int *) ((char *) c + n ) = 0xd1400bff;//sub
//
//            *(int *) ((char *) (c) + n + 4) = 0xa95f7bf2;//ldp
//            *(int *) ((char *) (c) + n + 8)=0x91400bff;
//            FF830091



//            *(int *) ((char *) c + n + 12)=code;
//
//            *(int *) ((char *) c + n+16 ) = 0xd1400bff;//sub
//
//            *(int *) ((char *) (c) + n +20) = 0xa91f7bf2;//ldp
//
//            *(int *) ((char *) (c) + n + 24) = 0x91400bff;//add
//
//
//            *(int *) ((char *) c + n + 28) = 0x58000072;
//            *(int *) ((char *) c + n + 32) = 0xd63f0240;
//            *(int *) ((char *) (c) + n + 36) = 0x14000003;
//            *(long **) ((char *) c + n + 40) = reinterpret_cast<long *>(myloadmethod1);
//            *(int *) ((char *) (c) + n + 48) = 0xaa0003e0;
//            *(int *) ((char *) (c) + n + 52) = 0xaa0003e0;


        }else if (isadrp == 1) {

//            FF8300D1
//            *(int *) ((char *) c + n ) = 0xaa0003e0;
////            FF0308D1
////            F27B40A9
////            F27B5FA9
//            *(int *) ((char *) (c) + n + 4) = 0xaa0003e0;
            *(int *) ((char *) (c) + n+0) = 0x58000072;
            unsigned long x =( (unsigned long) code) % 0x20;
            *(int *) ((char *) (c) + n + 4) =((unsigned long) 0xaa1203e0) +(unsigned long)x;
            *(int *) ((char *) (c) + n + 8) = 0x14000003;
            *(long *) ((char *) (c) + n + 12) = address;

            *(int *) ((char *) c + n + 36) = 0x58000072;
            *(int *) ((char *) c + n + 40) = 0xd63f0240;
            *(int *) ((char *) (c) + n + 44) = 0x14000003;
            *(long **) ((char *) c + n + 48) = reinterpret_cast<long *>(print_adrp);

            *(int *) ((char *) (c) + n + 20) = 0xaa0003e0;
//          FF830091
            *(int *) ((char *) (c) + n + 24) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 28) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 32) = 0xaa0003e0;

//          F27B00A9
if(isx18==1) {

//    *(int *) ((char *) c + n ) = 0xd1400bff;//sub
//
//    *(int *) ((char *) (c) + n + 4) = 0xa95f7bf2;//ldp
//    *(int *) ((char *) (c) + n + 8)=0x91400bff;

    *(int *) ((char *) (c) + n + 20) = 0xd1400bff;
    *(int *) ((char *) (c) + n + 24) = 0xa91f7bf2;
//          FF830091
    *(int *) ((char *) (c) + n + 28) = 0x91400bff;
}
            if(isx30==1) {
                *(int *) ((char *) (c) + n + 0) = 0xd1400bff;
                *(int *) ((char *) (c) + n + 4) = 0xa95f7bf2;

                *(int *) ((char *) (c) + n+8) = 0x5800007e;
                *(int *) ((char *) (c) + n + 12) =((unsigned long) 0xaa1e03e0) +(unsigned long)x;

                *(int *) ((char *) (c) + n + 16) = 0x14000003;
                *(long *) ((char *) (c) + n + 20) = address;
                *(int *) ((char *) (c) + n + 28) = 0xa91f7bf2;
                *(int *) ((char *) (c) + n + 32) = 0x91400bff;
                *(int *) ((char *) (c) + n + 36) = 0xaa0003e0;
//          FF830091

                *(int *) ((char *) (c) + n + 40) = 0xaa0003e0;
                *(int *) ((char *) (c) + n + 44) = 0xaa0003e0;
                *(int *) ((char *) (c) + n + 48) = 0xaa0003e0;
                *(int *) ((char *) (c) + n + 52) = 0xaa0003e0;


            }



        }else if (isbl == 1) {

            *(int *) ((char *) c + n ) = 0xaa0003e0;
//            FF0308D1
//            F27B40A9
//            F27B5FA9
            *(int *) ((char *) (c) + n + 4) = 0xaa0003e0;


            *(int *) ((char *) (c) + n + 28) = 0xaa0003e0;
//          FF830091
            *(int *) ((char *) (c) + n + 32) = 0xaa0003e0;


            *(int *) ((char *) c + n+8) = 0x58000072;
            *(int *) ((char *) c + n + 12) = 0xd63f0240;
            *(int *) ((char *) (c) + n + 16) = 0x14000003;
            *(long **) ((char *) c + n + 20) = reinterpret_cast<long *>(address);

            *(int *) ((char *) c + n + 36) = 0x58000072;
            *(int *) ((char *) c + n + 40) = 0xd63f0240;
            *(int *) ((char *) (c) + n + 44) = 0x14000003;
            *(long **) ((char *) c + n + 48) = reinterpret_cast<long *>(bl);



        }else if (isb == 1) {
//            int code = *(int *) ((char *) c + n);


            if ((code & 0x17000000) == 0x17000000) {
                int off = (unsigned int) 0x18000000 - (unsigned int) code;
                *(int *) ((char *) c + n+20) = (unsigned int) 0x18000000 - ((unsigned int) off) * 14-5;
            } else {

                int off = ((unsigned int) code) & 0xfffff;
                *(int *) ((char *) c + n+20) = (unsigned int) 0x14000000 + off * 14-5;

            }

            *(int *) ((char *) c + n + 0) = 0x58000072;
            *(int *) ((char *) c + n + 4) = 0xd63f0240;
            *(int *) ((char *) (c) + n + 8) = 0x14000003;
            *(long **) ((char *) c + n + 12) = reinterpret_cast<long *>(b1);

            *(long *) ((char *) (c) + n + 24) = 0xaa0003e0;
            *(long *) ((char *) (c) + n + 28) = 0xaa0003e0;
            *(long *) ((char *) (c) + n + 32) = 0xaa0003e0;

            *(long *) ((char *) (c) + n + 36) = 0xaa0003e0;


        }else if (isb2 == 1) {
//            int code = *(int *) ((char *) c + n);
//            __android_log_print(6,"r0ysue","%p",code);
            unsigned int off = ((unsigned int) code >> 4) & 0x000ffffe;

//            __android_log_print(6,"r0ysue","%p",off);
            int flag=0;
            if ((off & 0x00f0000) == 0xf0000) {
                off = 0xfffff - 14 * (0xfffff - off + 1) ;
                off=0xfffff-off+0xc+2;
            } else {
                off = 14 * off-0xc;

                flag=1;
//                __android_log_print(6,"r0ysue","%p",off);
            }

//            __android_log_print(6,"r0ysue","%p",off);
            off=off*2/4;




            int code1=(code&0xff00001f)^0x40;

            *(int *) ((char *) c + n ) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 4) = 0xaa0003e0;

            *(int *) ((char *) c + n+28) = code1;
            *(int *) ((char *) c + n + 8) = 0x58000072;
            *(int *) ((char *) c + n + 12) = 0xd63f0240;
            *(int *) ((char *) (c) + n + 16) = 0x14000003;
            *(long **) ((char *) c + n + 20) = reinterpret_cast<long *>(b2);
            *(int *) ((char *) (c) + n + 32) = 0x14000006;
            int code2=0;
            if(flag==1){
                code2=0x14000000^(off-1-2);
            }else{
                code2=(unsigned int )0x18000000-(unsigned int)off-(unsigned int)2;

            }

            *(int *) ((char *) (c) + n + 36) = code2;
            *(int *) ((char *) (c) + n + 40)=0xaa0003e0;
            *(int *) ((char *) (c) + n + 44) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 48) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 52)=0xaa0003e0;

        }else if (istb == 1) {
//            int code = *(int *) ((char *) c + n);
//            __android_log_print(6,"r0ysue","tbnz %p",code);
            unsigned int off = ((unsigned int) code >> 4) & 0x0000fffe;

//            __android_log_print(6,"r0ysue","tbnz %p",off);
            int flag=0;
            if ((off & 0x00f0000) == 0xf0000) {
                off = 0xfffff - 15 * (0xfffff - off + 1) ;
                off=0xfffff-off+0xc+2;
            } else {
                off = 14 * off-0xc;

                flag=1;
            }
            off=off*2/4;
            int code1=(code&0xfff0001f)^0x40;

//            *(int *) ((char *) c + n ) = 0xaa0003e0;
//            *(int *) ((char *) (c) + n + 4) = 0xaa0003e0;

            *(int *) ((char *) c + n+20) = code1;
            *(int *) ((char *) c + n + 0) = 0x58000072;
            *(int *) ((char *) c + n + 4) = 0xd63f0240;
            *(int *) ((char *) (c) + n + 8) = 0x14000003;
            *(long **) ((char *) c + n + 12) = reinterpret_cast<long *>(b2);
            *(int *) ((char *) (c) + n + 24) = 0x14000004;
            int code2=0;
            if(flag==1){
                code2=0x14000000^(off-1);
            }else{
                code2=(unsigned int )0x18000000-(unsigned int)off;

            }

            *(int *) ((char *) (c) + n + 28) = code2;
                        *(int *) ((char *) c + n +32) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 36) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 40)=0xaa0003e0;
            *(int *) ((char *) (c) + n + 44) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 48) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 52)=0xaa0003e0;

        }else if(isbr==1){

            unsigned int cs=((unsigned int)mybr)<<16;
            *(int *) ((char *) c + n)=((unsigned int)0xaa0003f2)^cs;
            *(int *) ((char *) c + n + 4) = (unsigned int)(0x58000060)^((unsigned int)mybr);
            unsigned int f=((unsigned int)mybr*2)<<4;
            *(int *) ((char *) c + n + 8) = ((unsigned int)0xd63f0000)^((unsigned int)f);
            *(int *) ((char *) (c) + n + 12) = 0x14000003;
            *(long **) ((char *) c + n + 16) = reinterpret_cast<long *>(mydealbr);
            *(int*) ((char *) c + n + 24)=0xd61f0240;
            *(long *) ((char *) (c) + n + 28) = 0xaa0003e0;
            *(long *) ((char *) (c) + n + 32) = 0xaa0003e0;
            *(long *) ((char *) (c) + n + 36) = 0xaa0003e0;
            *(long *) ((char *) (c) + n + 40) = 0xaa0003e0;
            *(long *) ((char *) (c) + n + 44) = 0xaa0003e0;
            *(long *) ((char *) (c) + n + 48) = 0xaa0003e0;
            *(long *) ((char *) (c) + n + 52) = 0xaa0003e0;

        }
            else {




//            int code = *(int *) ((char *) c + n);
            *(int *) ((char *) c + n ) = 0xd1400bff;//sub

            *(int *) ((char *) (c) + n + 4) = 0xa95f7bf2;//ldp
            *(int *) ((char *) (c) + n + 8)=0x91400bff;
//            FF830091



            *(int *) ((char *) c + n + 12)=code;

            *(int *) ((char *) c + n+16 ) = 0xd1400bff;//sub

            *(int *) ((char *) (c) + n +20) = 0xa91f7bf2;//ldp

            *(int *) ((char *) (c) + n + 24) = 0x91400bff;//add


            *(int *) ((char *) c + n + 28) = 0x58000072;
            *(int *) ((char *) c + n + 32) = 0xd63f0240;
            *(int *) ((char *) (c) + n + 36) = 0x14000003;
            *(long **) ((char *) c + n + 40) = reinterpret_cast<long *>(myloadmethod1);
            *(int *) ((char *) (c) + n + 48) = 0xaa0003e0;
            *(int *) ((char *) (c) + n + 52) = 0xaa0003e0;

        }
//        else{
////            int code = *(int *) ((char *) c + n);
//            *(int *) ((char *) c + n ) = 0xd1400bff;//sub
//
//            *(int *) ((char *) (c) + n + 4) = 0xa95f7bf2;//ldp
//
//            *(int *) ((char *) c + n + 8)=code;
//            *(int *) ((char *) (c) + n +12) = 0xa91f7bf2;//ldp
//
//            *(int *) ((char *) (c) + n + 16) = 0x91400bff;//add
//
//            *(int *) ((char *) (c) + n + 20)=0xaa0003e0;
//
//            *(int *) ((char *) c + n + 24) = 0x58000072;
//            *(int *) ((char *) c + n + 28) = 0xd63f0240;
//            *(int *) ((char *) (c) + n + 32) = 0x14000003;
//            *(long **) ((char *) c + n + 36) = reinterpret_cast<long *>(myloadmethod1);
//
//
//            *(int *) ((char *) (c) + n + 44) = 0xaa0003e0;
//            *(int *) ((char *) (c) + n + 48) = 0xaa0003e0;
//            *(int *) ((char *) (c) + n + 52) = 0xaa0003e0;
//
//
//
//        }





    }

mprotect((void*)PAGE_START((long)mydealbr),PAGE_SIZE,PROT_EXEC|PROT_WRITE|PROT_READ);
    *(long*)((char*)mydealbr+20)= reinterpret_cast<long>(begin);
    *(long*)((char*)mydealbr+48)= reinterpret_cast<long>(c);

    return c;
}