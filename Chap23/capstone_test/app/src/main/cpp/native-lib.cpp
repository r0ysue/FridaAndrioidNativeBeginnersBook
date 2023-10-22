#include <jni.h>
#include <string>
#include "android/log.h"
#include "sys/mman.h"
#include "unistd.h"
#include "capstone/capstone.h"

#define PAGE_START(x)  ((x) & PAGE_MASK)

typedef void *(*myfunc)();

int isret;
int isb;
int isbl;
int isb2;
int istb;
int isx18;
int isx30;
int isadrp;
long address;

static void test2(unsigned int yy, uint64_t add) {
    isb = 0;
    isbl = 0;
    isb2 = 0;
    istb = 0;
    isadrp=0;
    isx18=0;
    isx30=0;
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
        __android_log_print(6, "r0ysue", "%s", code);
        if (strcmp(insn[0].mnemonic, "b") == 0) {
            isb = 1;
            strtok(code, "#0x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
            __android_log_print(6, "r0ysue", "address : %p", address);
        } else if (strcmp(insn[0].mnemonic, "bl") == 0) {
            isbl = 1;
            strtok(code, "#0x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
            __android_log_print(6, "r0ysue", "address : %p", address);
        } else if (strstr(insn[0].mnemonic, "b.") || strcmp(insn[0].mnemonic, "cbz") == 0 ||
                   strcmp(insn[0].mnemonic, "cbnz") == 0) {
            isb2 = 1;
            strtok(code, "#0x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
            __android_log_print(6, "r0ysue", "address : %p", address);
        } else if (strcmp(insn[0].mnemonic, "tbz") == 0 || strcmp(insn[0].mnemonic, "tbnz") == 0) {
            istb = 1;
            strtok(code, "#0x");
            strtok(NULL, "#0x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
            __android_log_print(6, "r0ysue", "address : %p", address);
        } else if (strcmp(insn[0].mnemonic, "adrp") == 0) {

            if(strstr(code,"x18")){

                isx18=1;
            }
            if(strstr(code,"x30")){

                isx30=1;
            }



            isadrp = 1;
            strtok(code, "x");
            strtok(NULL, "x");
            address = strtoul(strtok(NULL, ""), NULL, 16);
            __android_log_print(6, "r0ysue", "address : %p", address);
        } else if (strcmp(insn[0].mnemonic, "ret") == 0) {
            isret = 1;
            __android_log_print(6, "r0ysue", "address : %s", code);
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


static void test3(unsigned int yy, uint64_t add) {
    isb = 0;
    isbl = 0;
    isb2 = 0;
    istb = 0;
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
        __android_log_print(6, "r0ysue", "my invoke code %s", code);


        cs_free(insn, count);
    }
    cs_close(&handle);
    free(stc);
}


int func() {
int s=1;
for(int n=0;n<5;n++)
    __android_log_print(5,"r0ysue"," i am from for %x",n);
if(s==1)
    __android_log_print(5,"r0ysue"," 1111111111111111");
    __android_log_print(5,"r0ysue"," 2222");
    __android_log_print(5,"r0ysue"," 3333");
    __android_log_print(5,"r0ysue"," 4444");

    return s*(s+9);


}


void my_real_print(long* x30){
int* addr=(int*)((char*)(*x30)-0x18);
    test3(*(int*)(addr),(long)(addr));
//__android_log_print(6,"r0ysue","%x",*(int*)(addr));
}

void my_real_print1(long* x30){

    int myx=(*(int*)((char*)(*x30)-24))&0x1f;
    long myaddress=*(long*)((char*)(*x30)-16);
__android_log_print(6,"r0ysue","adrp  x%x,%p",myx,myaddress);
}

void my_real_print2(long* x30){

//    long* address=((long *)((char*)(*x30)+12));

    test3(*(int *)((char*)(*x30)+12), reinterpret_cast<uint64_t>((int *) ((char *) (*x30) + 12)));

}


void my_real_print_bl(long* x30){

    long* address1=((long *)((char*)(*x30)-16));


    __android_log_print(6,"r0ysue","bl %p",*address1);


}
//void my_real_printb2(long* x30){
//
//    long* address1=((long *)((char*)(*x30)+12));
//
//
//    test
//
//
//}





void my_print(){
    long *addr;
    __asm__("mov %[input_n], sp\r\n"
    :[result_m] "=r"(addr)
    :[input_n] "r"(addr)
    );

    my_real_print(reinterpret_cast<long *>((char *) addr + 0x110));
}


void my_print1(){
    long *addr;
    __asm__("mov %[input_n], sp\r\n"
    :[result_m] "=r"(addr)
    :[input_n] "r"(addr)
    );

    my_real_print1(reinterpret_cast<long *>((char *) addr + 0x110));
}

void my_print2(){
    long *addr;
    __asm__("mov %[input_n], sp\r\n"
    :[result_m] "=r"(addr)
    :[input_n] "r"(addr)
    );

    my_real_print2(reinterpret_cast<long *>((char *) addr + 0x110));
}

void my_printbl(){
    long *addr;
    __asm__("mov %[input_n], sp\r\n"
    :[result_m] "=r"(addr)
    :[input_n] "r"(addr)
    );

    my_real_print_bl(reinterpret_cast<long *>((char *) addr + 0x110));
}

void my_printb2(){
    long *addr;
    __asm__("mov %[input_n], sp\r\n"
    :[result_m] "=r"(addr)
    :[input_n] "r"(addr)
    );

    my_real_print2(reinterpret_cast<long *>((char *) addr + 0x110));
}




void print_code() {

    asm("sub SP, SP, #0x200");
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
    asm("stp X30, X30, [SP,#0xf0]");
    asm("stp q0, q1, [SP,#0x160]");
    my_print();
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
    asm("add SP, SP, #0x200");






}


void print_adrp(){
    asm("sub SP, SP, #0x200");
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
    asm("stp X30, X30, [SP,#0xf0]");
    asm("stp q0, q1, [SP,#0x160]");
    my_print1();
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
    asm("add SP, SP, #0x200");




}

void b1(){

    asm("sub SP, SP, #0x200");
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
    asm("stp X30, X30, [SP,#0xf0]");
    asm("stp q0, q1, [SP,#0x160]");
    my_print2();
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
    asm("add SP, SP, #0x200");


}

void bl(){

    asm("sub SP, SP, #0x200");
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
    asm("stp X30, X30, [SP,#0xf0]");
    asm("stp q0, q1, [SP,#0x160]");
    my_printbl();
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
    asm("add SP, SP, #0x200");


}


void b2(){

    asm("sub SP, SP, #0x200");
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
    asm("stp X30, X30, [SP,#0xf0]");
    asm("stp q0, q1, [SP,#0x160]");
    my_printb2();
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
    asm("add SP, SP, #0x200");


}







void *deal_with(void *begin, int size) {
    void *my_new_func = malloc(size * 15);
    int mod = ((size * 15) / 0x1000) + 1;
    mprotect((void *) PAGE_START((long) my_new_func), mod * PAGE_SIZE,
             PROT_EXEC | PROT_WRITE | PROT_READ);
    for (int n = 0; n < size * 14; n = n + 56) {
        test2(*(int *) ((char *) begin + n / 14), (long) ((char *) begin + n / 14));
        int code=*(int *) ((char *) begin + n / 14);




        if(n==0){
            *(int *) ((char *) my_new_func + n + 0)=0xaa0003e0;


            *(int *) ((char *) my_new_func + n + 4) = 0xaa0003e0;
            *(int *) ((char *) my_new_func + n + 8) = 0xaa0003e0;

            *(int *) ((char *) my_new_func + n + 12) = code;
            *(int *) ((char *) my_new_func + n + 16) = 0xd14043ff;
            *(int *) ((char *) my_new_func + n + 20) = 0xa9007bf2;
            *(int *) ((char *) my_new_func + n + 24) = 0x914043ff;


            *(int *) ((char *) my_new_func + n + 28) = 0x58000070;
            *(int *) ((char *) my_new_func + n + 32) = 0xd63f0200;
            *(int *) ((char *) my_new_func + n + 36) = 0x14000003;
            *(long *) ((char *) my_new_func + n + 40) = reinterpret_cast<long>(print_code);

            *(int *) ((char *) my_new_func + n + 48) = 0xaa0003e0;
            *(int *) ((char *) my_new_func + n + 52) = 0xaa0003e0;



        }else if(isadrp==1){
//            72000058
            *(int *) ((char *) my_new_func + n + 0)=0x58000072;
            unsigned int myx=((unsigned int)code)%0x20;
//            E00312AA
            *(int *) ((char *) my_new_func + n + 4)=0xaa1203e0^myx;
            *(int *) ((char *) my_new_func + n + 8)=0x14000003;
            *(long* )((char *) my_new_func + n + 12)=address;

            *(int *) ((char *) my_new_func + n + 20) = 0x58000072;
            *(int *) ((char *) my_new_func + n + 24) = 0xd63f0240;
            *(int *) ((char *) my_new_func + n + 28) = 0x14000003;
            *(long *) ((char *) my_new_func + n + 32) = reinterpret_cast<long>(print_adrp);
            *(int *) ((char *) my_new_func + n + 40) = 0xaa0003e0;
            *(int *) ((char *) my_new_func + n + 44) = 0xaa0003e0;
            *(int *) ((char *) my_new_func + n + 48) = 0xaa0003e0;
            *(int *) ((char *) my_new_func + n + 52) = 0xaa0003e0;

            if(isx18==1){

                *(int *) ((char *) my_new_func + n + 0)=0xd14043ff;
                *(int *) ((char *) my_new_func + n + 4) = 0xa9407bf2;
                *(int *) ((char *) my_new_func + n + 8) = 0x914043ff;
                *(int *) ((char *) my_new_func + n + 12)=0x58000072;
                unsigned int myx=((unsigned int)code)%0x20;
//            E00312AA
                *(int *) ((char *) my_new_func + n + 16)=0xaa1203e0^myx;
                *(int *) ((char *) my_new_func + n + 20)=0x14000003;
                *(long* )((char *) my_new_func + n + 24)=address;
                *(int *) ((char *) my_new_func + n + 32) = 0xd14043ff;
                *(int *) ((char *) my_new_func + n + 36) = 0xa9007bf2;
                *(int *) ((char *) my_new_func + n + 40) = 0x914043ff;
            }

            if(isx30==1){
                *(int *) ((char *) my_new_func + n + 0)=0xd14043ff;
                *(int *) ((char *) my_new_func + n + 4) = 0xa9407bf2;
                *(int *) ((char *) my_new_func + n + 8) = 0x914043ff;
                *(int *) ((char *) my_new_func + n + 12)=0x5800007e;
                unsigned int myx=((unsigned int)code)%0x20;
//            E00312AA
                *(int *) ((char *) my_new_func + n + 16)=0xaa1e03e0^myx;
                *(int *) ((char *) my_new_func + n + 20)=0x14000003;
                *(long* )((char *) my_new_func + n + 24)=address;
                *(int *) ((char *) my_new_func + n + 32) = 0xd14043ff;
                *(int *) ((char *) my_new_func + n + 36) = 0xa9007bf2;
                *(int *) ((char *) my_new_func + n + 40) = 0x914043ff;



            }




        }else if(isb==1){

            if ((code & 0x17000000) == 0x17000000) {
                int off = (unsigned int) 0x18000000 - (unsigned int) code;
                *(int *) ((char *) my_new_func + n+20) = (unsigned int) 0x18000000 - ((unsigned int) off) * 14-5;
            } else {

                int off = ((unsigned int) code) & 0xfffff;
                *(int *) ((char *) my_new_func + n+20) = (unsigned int) 0x14000000 + off * 14-5;

            }

            *(int *) ((char *) my_new_func + n + 0) = 0x58000072;
            *(int *) ((char *) my_new_func + n + 4) = 0xd63f0240;
            *(int *) ((char *) (my_new_func) + n + 8) = 0x14000003;
            *(long **) ((char *) my_new_func + n + 12) = reinterpret_cast<long *>(b1);

        } else if(isbl==1){

            *(int *) ((char *) my_new_func + n + 0) = 0x58000072;
            *(int *) ((char *) my_new_func + n + 4) = 0xd63f0240;
            *(int *) ((char *) (my_new_func) + n + 8) = 0x14000003;
            *(long **) ((char *) my_new_func + n + 12) = reinterpret_cast<long *>(address);



            *(int *) ((char *) my_new_func + n + 20) = 0x58000072;
            *(int *) ((char *) my_new_func + n + 24) = 0xd63f0240;
            *(int *) ((char *) (my_new_func) + n + 28) = 0x14000003;
            *(long **) ((char *) my_new_func + n + 32) = reinterpret_cast<long *>(bl);


            *(int *) ((char *) my_new_func + n + 40) = 0xaa0003e0;
            *(int *) ((char *) my_new_func + n + 44) = 0xaa0003e0;
            *(int *) ((char *) my_new_func + n + 48) = 0xaa0003e0;
            *(int *) ((char *) my_new_func + n + 52) = 0xaa0003e0;


        }else if(isb2==1){
            unsigned int off = ((unsigned int) code >> 4) & 0x000ffffe;
            int flag=0;
            if ((off & 0x00f0000) == 0xf0000) {
                off = 0xfffff - 14 * (0xfffff - off + 1) ;
                off=0xfffff-off+0xc+2;
            } else {
                off = 14 * off-0xc;

                flag=1;
//                __android_log_print(6,"r0ysue","%p",off);
            }


            off=off*2/4;
            int code1=(code&0xff00001f)^0x40;
            *(int *) ((char *) my_new_func + n ) = 0xaa0003e0;
            *(int *) ((char *) (my_new_func) + n + 4) = 0xaa0003e0;
            *(int *) ((char *) my_new_func + n+28) = code1;
            *(int *) ((char *) my_new_func + n + 8) = 0x58000072;
            *(int *) ((char *) my_new_func + n + 12) = 0xd63f0240;
            *(int *) ((char *) (my_new_func) + n + 16) = 0x14000003;
            *(long **) ((char *) my_new_func + n + 20) = reinterpret_cast<long *>(b2);
            *(int *) ((char *) (my_new_func) + n + 32) = 0x14000004;

            int code2=0;
            if(flag==1){
                code2=0x14000000^(off-1);
            }else{
                code2=(unsigned int )0x18000000-(unsigned int)off;

            }

            *(int *) ((char *) (my_new_func) + n + 36) = code2;
            *(int *) ((char *) (my_new_func) + n + 40)=0xaa0003e0;
            *(int *) ((char *) (my_new_func) + n + 44) = 0xaa0003e0;
            *(int *) ((char *) (my_new_func) + n + 48) = 0xaa0003e0;
            *(int *) ((char *) (my_new_func) + n + 52)=0xaa0003e0;






        }else if(istb==1){
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

            *(int *) ((char *) my_new_func + n+20) = code1;
            *(int *) ((char *) my_new_func + n + 0) = 0x58000072;
            *(int *) ((char *) my_new_func + n + 4) = 0xd63f0240;
            *(int *) ((char *) (my_new_func) + n + 8) = 0x14000003;
            *(long **) ((char *) my_new_func + n + 12) = reinterpret_cast<long *>(b2);
            *(int *) ((char *) (my_new_func) + n + 24) = 0x14000004;
            int code2=0;
            if(flag==1){
                code2=0x14000000^(off-1);
            }else{
                code2=(unsigned int )0x18000000-(unsigned int)off;

            }

            *(int *) ((char *) (my_new_func) + n + 28) = code2;
            *(int *) ((char *) my_new_func + n +32) = 0xaa0003e0;
            *(int *) ((char *) (my_new_func) + n + 36) = 0xaa0003e0;
            *(int *) ((char *) (my_new_func) + n + 40)=0xaa0003e0;
            *(int *) ((char *) (my_new_func) + n + 44) = 0xaa0003e0;
            *(int *) ((char *) (my_new_func) + n + 48) = 0xaa0003e0;
            *(int *) ((char *) (my_new_func) + n + 52)=0xaa0003e0;


        }


        else {
            *(int *) ((char *) my_new_func + n) = *(int *) ((char *) begin + n / 14);
//        E00300AA


            *(int *) ((char *) my_new_func + n + 0)=0xd14043ff;


            *(int *) ((char *) my_new_func + n + 4) = 0xa9407bf2;
            *(int *) ((char *) my_new_func + n + 8) = 0x914043ff;

            *(int *) ((char *) my_new_func + n + 12) = code;

            *(int *) ((char *) my_new_func + n + 16) = 0xd14043ff;

            *(int *) ((char *) my_new_func + n + 20) = 0xa9007bf2;
            *(int *) ((char *) my_new_func + n + 24) = 0x914043ff;



            *(int *) ((char *) my_new_func + n + 28) = 0x58000072;
            *(int *) ((char *) my_new_func + n + 32) = 0xd63f0240;
            *(int *) ((char *) my_new_func + n + 36) = 0x14000003;
            *(long *) ((char *) my_new_func + n + 40) = reinterpret_cast<long>(print_code);

            *(int *) ((char *) my_new_func + n + 48) = 0xaa0003e0;
            *(int *) ((char *) my_new_func + n + 52) = 0xaa0003e0;

        }


    }
    return my_new_func;


}


extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_class23_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    int a = func();
    void *my_new = malloc(0x2000);
    mprotect((void *) PAGE_START((long) my_new), PAGE_SIZE, PROT_EXEC | PROT_WRITE | PROT_READ);
    memcpy(my_new, (void *) (func), 0x200);

    long *aa = static_cast<long *>(deal_with((void *) func, 0x200));
    __android_log_print(6, "r0ysue", "%p", aa);

//   sleep(20);
    mprotect((void *) PAGE_START((long) func), PAGE_SIZE, PROT_EXEC | PROT_WRITE | PROT_READ);
    *(int*)((char*)func+0)=0x58000050;
//    00021FD6
    *(int*)((char*)func+4)=0xd61f0200;
    *(long*)((char*)func+8)= reinterpret_cast<long>(my_new);

    myfunc myinvoke = (myfunc) aa;
        myinvoke();
sleep(1);
func();
    return env->NewStringUTF(hello.c_str());
}