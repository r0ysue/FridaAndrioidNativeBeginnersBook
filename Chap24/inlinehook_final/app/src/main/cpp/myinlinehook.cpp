//
// Created by root on 12/7/21.
//

#include "myinlinehook.h"
#include <jni.h>
#include <string>
#include <unistd.h>
#include "android/log.h"
#include "sys/mman.h"
#include "pthread.h"
#include "fcntl.h"
#include "capstone/capstone.h"
#define PAGE_START(x)  ((x) & PAGE_MASK)
typedef  void*(*func1)();
func1 func;
int isb;
int isbl;
int isb2;
int istb;
long address;
void mychange(){

func();
}

extern "C" void myreplace() {
    asm("sub SP, SP, #0x200");
    asm("mrs x16, NZCV");
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

//    asm("sub SP, SP, #0x20");

//    long addr=0;
//    __asm__("mov %[input_n], x30\r\n"
//    :[result_m] "=r"(addr)
//    :[input_n] "r"(addr)
//    );

    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
//    pthread_mutex_unlock(&mutex);

//    asm("add SP, SP, #0x20");

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
    asm("msr NZCV, x16");
//    asm("ldp X16, X17, [SP,#0x10]");
    asm("ldp X0, X1, [SP,#0x20]");
    asm("ldp X2, X3, [SP,#0x30]");
    asm("ldp X4, X5, [SP,#0x40]");
    asm("ldp X6, X7, [SP,#0x50]");
    asm("ldp X29, X30, [SP,#0x10]");
    asm("add SP, SP, #0x200");

    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");

    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");

    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");

    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");
    asm("mov x0,x0");



    asm("ldr x16,8");
    asm("br x16");
    asm("mov x0,x0");
    asm("mov x0,x0");

}
static void test2(unsigned int yy, uint64_t add) {
    isb=0;
    isbl=0;
    isb2=0;
    istb=0;
    unsigned char *stc = static_cast<unsigned char *>(malloc(8));
    char code[40];
    memcpy(stc, &yy, 8);
    csh handle;
    cs_insn *insn;
    size_t count;
    cs_open(CS_ARCH_ARM64, CS_MODE_ARM, &handle);
    cs_option(handle, CS_OPT_DETAIL, CS_OPT_ON);
    count = cs_disasm(handle, stc, sizeof(stc) - 1, add, 0, &insn);
    if (count) {
        sprintf(code, "%s %s", insn[0].mnemonic, insn[0].op_str);
        __android_log_print(6,"r0ysue","%s",code);


        if(strcmp(insn[0].mnemonic,"b")==0){
            isb=1;
            strtok(code, "x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
        }else if(strcmp(insn[0].mnemonic,"bl")==0){
            isbl=1;
            strtok(code, "x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
        }else if(strstr(insn[0].mnemonic,"b.")||strcmp(insn[0].mnemonic,"cbz")==0){
            isb2=1;
            strtok(code, "x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
        }else if(strcmp(insn[0].mnemonic,"tbz")==0||strcmp(insn[0].mnemonic,"tbnz")==0){
            istb=1;
            strtok(code, "x");
            strtok(NULL, "x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
            __android_log_print(6,"r0ysue","%p",address);
        }


//        for (int i = 0; i < arm64->op_count; i++) {
//            cs_arm64_op *op = &(arm64->operands[i]);
//            __android_log_print(6,"r0ysue","ddddd %x %x",op->,ARM64_OP_REG);
//        }

//        test2_spr(insn[0].mnemonic, insn[0].op_str, reinterpret_cast<long *>(address));
//if(strstr(insn[0].mnemonic,"bl")&&!strstr(insn[0].mnemonic,"blr"))

        cs_free(insn, count);
    }
    cs_close(&handle);
    free(stc);
}
void register_hook(void* address1,void* func_replace){
    mprotect((void *) PAGE_START((long) myreplace), PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
void* a=malloc(0x11c);
memcpy(a, (void*)myreplace, 0x11c);
__android_log_print(6,"r0ysue","%p",a);
func= (func1)(func_replace);



void* loadmethod=address1;
    mprotect((void *) PAGE_START((long) loadmethod), PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
//    mprotect((void*)PAGE_START((long)myreplace),PAGE_SIZE,PROT_WRITE|PROT_READ|PROT_EXEC);
   int old_code1 = *(int *) ((char *) loadmethod);
    int  old_code2 = *(int *) ((char *) loadmethod + 4);
    int  old_code3 = *(int *) ((char *) loadmethod + 8);
    int  old_code4 = *(int *) ((char *) loadmethod + 12);
    int code1 = 0x58000050;//ldr x16,8
    int code2 = 0xd61f0200;
    long code3 = reinterpret_cast<long>(a);
  long  retaddress = reinterpret_cast<long>((char *) loadmethod + 0x10);

long base= reinterpret_cast<long>((char *) a + 0x4c);

    mprotect((void *) PAGE_START((long) a), PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
    int n=0x44;
    *(int *) ((char *) (a) + n) = 0x58000070;
    *(int *) ((char *) (a) + n+4) = 0xd63f0200;
    *(int *) ((char *) (a) + n+8) = 0x14000003;
    *(long *) ((char *) (a) + n+12) = reinterpret_cast<long>(func_replace);

long* huifu= static_cast<long *>(a);
    long* addr= static_cast<long *>(loadmethod);
    for(int n=0x9c;n<=0x9c+72;n=n+24){

        test2(*(int*)((char*)addr+(n-0x9c)/6), reinterpret_cast<uint64_t>((char*)addr+(n-0x9c)/6));
        if(isb==1){

            if((unsigned long)address-(unsigned long)addr<=0x10){
                address=((unsigned long)huifu+((unsigned long)address-(unsigned long)addr)*6)+(unsigned long )0x9c;

            }

            *(int *) ((char *) (huifu) + n) = 0x58000050;
            *(int *) ((char *) (huifu) + n+4) = 0xd61f0200;
            *(long *) ((char *) (huifu) + n+8) = address;
//        00021FD6
        } else if(isbl==1){

            if((unsigned long)address-(unsigned long)addr<=0x10){
                address=((unsigned long)huifu+((unsigned long)address-(unsigned long)addr)*6)+(unsigned long )0x9c;

            }

            *(int *) ((char *) (huifu) + n) = 0x58000070;
            *(int *) ((char *) (huifu) + n+4) = 0xd61f0200;
            *(int *) ((char *) (huifu) + n+8) = 0x14000003;
            *(long *) ((char *) (huifu) + n+12) = address;

        }else if(isbl==1){

            if((unsigned long)address-(unsigned long)addr<=0x10){
                address=((unsigned long)huifu+((unsigned long)address-(unsigned long)addr)*6)+(unsigned long )0x9c;

            }

            *(int *) ((char *) (huifu) + n) = 0x58000070;
            *(int *) ((char *) (huifu) + n+4) = 0xd63f0200;
//        00023FD6
            *(int *) ((char *) (huifu) + n+8) = 0x14000003;
            *(long *) ((char *) (huifu) + n+12) = address;

        }else if(isb2==1){

            if((unsigned long)address-(unsigned long)addr<=0x10){
                address=((unsigned long)huifu+((unsigned long)address-(unsigned long)addr)*6)+(unsigned long )0x9c;

            }

            *(int *) ((char *) (huifu) + n) =*(int*)((char*)addr+(n-0x9c)/6)&0xff00000f;
            *(int *) ((char *) (huifu) + n)=*(int *) ((char *) (huifu) + n)^0x40;
            *(int *) ((char *) (huifu) + n+4) = 0x14000005;
//        00023FD6
            *(int *) ((char *) (huifu) + n+8) = 0x58000050;
            *(int *) ((char *) (huifu) + n+12) = 0xd61f0200;
            *(long *) ((char *) (huifu) + n+16) = address;

        }else if(istb==1){

            if((unsigned long)address-(unsigned long)addr<=0x10){
                address=((unsigned long)huifu+((unsigned long)address-(unsigned long)addr)*6)+(unsigned long )0x9c;
                        __android_log_print(6,"r0ysue","%p %p %p",address,addr,huifu);
                }

                                   *(int *) ((char *) (huifu) + n) =*(int*)((char*)addr+(n-0x9c)/6)&0xffff000f;
                    *(int *) ((char *) (huifu) + n)=*(int *) ((char *) (huifu) + n)^0x40;
            *(int *) ((char *) (huifu) + n+4) = 0x14000005;
//        00023FD6
            *(int *) ((char *) (huifu) + n+8) = 0x58000050;
            *(int *) ((char *) (huifu) + n+12) = 0xd61f0200;
            *(long *) ((char *) (huifu) + n+16) = address;

        }else {
            *(int *) ((char *) (huifu) + n) =*(int*)((char*)addr+(n-0x9c)/6);

        }



    }




//    *(int *) ((char *) a + 0x9c) = old_code1;
//    *(int *) ((char *) a + 0xa0) = old_code2;
//    *(int *) ((char *) a + 0xa4) = old_code3;
//    *(int *) ((char *) a + 0xa8) = old_code4;



//    *(long *) ((char *) a + 0xb0+20) = retaddress;
//
    retaddress= reinterpret_cast<long>((char*)loadmethod+0x10);
    *(long*)((char*)a+ 0x104)=retaddress;


    *(int *) ((char *) loadmethod) = code1;
    *(int *) ((char *) loadmethod + 4) = code2;
    *(long *) ((char *) loadmethod + 8) = code3;
//sleep(20);

}