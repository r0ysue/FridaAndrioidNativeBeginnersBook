#include <jni.h>
#include <string>
#include <unistd.h>
#include "android/log.h"
#include "findsobase.h"
#include "findsym.h"
#include "trace.h"
#include "pthread.h"
#include "myinlinehook.h"
#define PAGE_START(x)  ((x) & PAGE_MASK)
#include "sys/mman.h"
#include "ida.h"
typedef void*(*my_invoke)(void*,int,void*) ;
typedef void*(*my_invoke2)(int) ;
int  myfunc(int a){
int n=0;

for(n=1;n<10;) {
    __android_log_print(5, "r0ysue", "whilke ");
    n=n*2;
}
if(a==1)
    __android_log_print(5,"r0ysue","i am from if ");


__android_log_print(5,"r0ysue","111111");
    __android_log_print(5,"r0ysue","22222222");
    __android_log_print(5,"r0ysue","33333333");
    __android_log_print(5,"r0ysue","444444444");
    __android_log_print(5,"r0ysue","555555555");


    return n-1;

}


void hexdump( const void *in, size_t len) {
    const uint8_t *data = reinterpret_cast<const uint8_t*>(in);
    char code[2000];

    for (size_t i = 0; i < len; i++) {
        sprintf(code, "%s %02x", code,data[i]);
        if(i%0x10==0)
            sprintf(code, "%s \n", code);
    }
    __android_log_print(6,"r0ysue","%s",(const char*)code);

}


extern "C" JNIEXPORT jstring JNICALL
Java_com_r0ysue_trace_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

//myfunc(1);
    return env->NewStringUTF(hello.c_str());
}


const char *getprocessname(JNIEnv *env) {

    jclass ActivityThread = env->FindClass("android/app/ActivityThread");
    jmethodID currentProcessName = env->GetStaticMethodID(ActivityThread, "currentProcessName",
                                                          "()Ljava/lang/String;");

    jstring name = static_cast<jstring>(env->CallStaticObjectMethod(ActivityThread,
                                                                    currentProcessName));
    const char *name1 = env->GetStringUTFChars(name, 0);
    return name1;

}

void myin() {

//    sleep(10);
    const char *a1 = "0123456789abcdef";
    int a2 = 16;
    const char *a3 = "www.pediy.com&kanxue";
    int a4 = 20;
    void *a5 = malloc(100);
    mprotect((void *) PAGE_START((long) a1), PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
    mprotect((void *) PAGE_START((long) a3), PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
    initlinker();
    long *start= static_cast<long *>(findsobase("libhello-jni.so"));
    __android_log_print(6, "r0ysue", "xxxxxx %p", start);
    long *sub_494C4 = reinterpret_cast<long *>((char *) start + 0x1494C4);
    mprotect((void *) PAGE_START((long) sub_494C4), PAGE_SIZE, PROT_WRITE | PROT_READ | PROT_EXEC);
//    4B8C4
//    __android_log_print(6, "r0ysue", "xxxxxx %p", sub_494C4);
//    FILE * as=fopen("/data/data/com.example.hellojni_sign2/a.txt","wt+");
//   my_invoke kk=(my_invoke)deal_with(sub_494C4, 0x13f1c,as);
//    __android_log_print(6, "r0ysue", "xxxxxx %p", kk);
//    sleep(10);
//    __android_log_print(6, "r0ysue", "xxxxxx %p", *(long*)a5);
//   kk((void *) a1, a2, (void *) a5);
//    __android_log_print(6, "r0ysue", "success %p",*(long*)a5);
//    fclose(as);
}

int sss=0;
void myin2(){
//    sleep(10);
    char line[1024];
    int *start;
    int *end;
    int n=1;
    FILE *fp=fopen("/proc/self/maps","r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "r-xp 00000000 00:00 0") ) {
            __android_log_print(6,"r0ysue","%s", line);
            if(n==1){
                start = reinterpret_cast<int *>(strtoul(strtok(line, "-"), NULL, 16));
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));


            }
            else{
                strtok(line, "-");
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
            }
            n++;
        }
    }
//sleep(20);



mprotect((void*)PAGE_START((long)((char*)start+0x4B8C4)),20*PAGE_SIZE,PROT_EXEC|PROT_WRITE|PROT_READ);


    FILE * as=fopen("/data/data/com.r0ysue.mytest/a.txt","wt+");
    my_invoke kk=(my_invoke)deal_with((char*)start+0x5A78C, 0x20000,"/data/data/com.r0ysue.mytest/a.txt");

    int code1 = 0x58000052;//ldr x16,8
    int code2 = 0xd61f0240;
    long code3 = reinterpret_cast<long>(kk);
    *( int*) ( (char*)start+0x5A78C)=code1;
    *( int*) ( (char*)start+0x5A78C+4)=code2;
    *( long*) ( (char*)start+0x5A78C+8)=code3;
    __android_log_print(6, "r0ysue", "xxxxxx %p", kk);
    fclose(as);
}


void mydump(long* a){
    hexdump((void*)((*a)-16),100);
//    __android_log_print(6,"r0ysue","%s")


}

void myin3(){
//    sleep(10);
    char line[1024];
    int *start;
    int *end;
    int n=1;
    FILE *fp=fopen("/proc/self/maps","r");
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "r-xp 00000000 00:00 0") ) {
            __android_log_print(6,"r0ysue","%s", line);
            if(n==1){
                start = reinterpret_cast<int *>(strtoul(strtok(line, "-"), NULL, 16));
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));


            }
            else{
                strtok(line, "-");
                end = reinterpret_cast<int *>(strtoul(strtok(NULL, " "), NULL, 16));
            }
            n++;
        }
    }
//sleep(20);
    mprotect((void*)PAGE_START((long)((char*)start+0x7669C)),20*PAGE_SIZE,PROT_EXEC|PROT_WRITE|PROT_READ);

register_hook((char*)start+0x7669C, (void*)(mydump));


}



void new_strstr(const char* a,const char* b){

    if(strcmp(b,"myself")==0) {
        __android_log_print(6, "r0ysue", "  method is  %s", a);

    }
}


const char * getartmethod(unsigned int *a1){
    __int64 v12; // x20
    __int64 v13; // x0
    _QWORD *v14; // x8
    __int64 v15; // x9
    char *v16; // x8
    const char *result; // x0
    unsigned int **v18; // x8
    unsigned int *v19; // x9
    unsigned int *v20; // x9
    int v21; // w10
    const char *v22; // x9
    const char *v23; // x8
    unsigned int **v24; // x20
    __int64 v25; // x0
    __int64 v26; // x0
    unsigned int **v27; // x20
    __int64 v28; // x0
    __int64 v29; // x0
    unsigned int *v30; // [xsp+48h] [xbp+18h]
    unsigned int *v31; // [xsp+48h] [xbp+18h]
//    _ZN3art9ArtMethod19GetObsoleteDexCacheEv
    v12 = a1[3];
    if ( (a1[1] & 0x40000) != 0 ) {
        return "cxzcxzcxz";
    }
    else
        v13 = *(unsigned int *)(*a1 + 0x10LL);
    v14 = *(_QWORD **)(v13 + 16);
    v15 = *(unsigned int *)(v14[12] + 8 * v12 + 4);
    if ( (_DWORD)v15 == -1 )
        return 0LL;
    v16 = (char *)(v14[1] + *(unsigned int *)(v14[9] + 4 * v15));
    result = v16 + 1;
    if ( (*v16 & 0x80000000) != 0 )
    {
        if ( (v16[1] & 0x80000000) != 0 )
        {
            if ( (v16[2] & 0x80000000) != 0 )
            {
                v21 = v16[3];
                v22 = v16 + 4;
                v23 = v16 + 5;
                if ( v21 >= 0 )
                    result = v22;
                else
                    result = v23;
            }
            else
            {
                result = v16 + 3;
            }
        }
        else
        {
            result = v16 + 2;
        }
    }
    return result;




}
void regist(void* a,void* b,int c){
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    _QWORD e=**((_QWORD **)a + 20);
    const char* ds=getartmethod((unsigned int *)e);

//    __android_log_print(6,"r0ysue","%s %p",ds,e);

    if(ds!= nullptr&&strstr(ds,"onCreate")&&sss==0){
//        __android_log_print(6,"r0ysue","register %s",ds);
sss=1;

     myin2();
//myin3();
        sleep(20);
    }


}

void regist1(void* a,void* b,int c){
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");
    __android_log_print(6,"r0ysue","");

    const char* ds=getartmethod((unsigned int *)a);



    if(ds!= nullptr&&strstr(ds,"onCreate")&&sss==0){


        __android_log_print(6,"r0ysue","register %s %p",ds,b);
//        sss=1;

//        myin2();
//        sleep(20);
    }


}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *unused) {


//
//    mainfun("_ZN3art9ArtMethod14RegisterNativeEPKvb", "libart.so",
//            reinterpret_cast<void *>(regist));
    JNIEnv *env = nullptr;
    vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_4);
    const char *myname = getprocessname(env);
    __android_log_print(6, "r0ysue", " xxxxxxxxx i am from %s", myname);
//sleep(20);
//    pthread_t thread;
//    pthread_create(&thread, nullptr, reinterpret_cast<void *(*)(void *)>(myin2), nullptr);

    initlinker();
    long* base= static_cast<long *>(findsobase("libart.so"));

    int off=findsym("/system/lib64/libart.so","_ZN3art14JniMethodStartEPNS_6ThreadE");


    int off1=findsym("/system/lib64/libart.so","_ZN3art9ArtMethod14RegisterNativeEPKvb");


    long* addr= reinterpret_cast<long *>((char *) base + off );
    long* addr1= reinterpret_cast<long *>((char *) base + off1 );
    __android_log_print(6,"r0ysue","%p",addr);
    base= static_cast<long *>(findsobase("libc.so"));
    long* st= reinterpret_cast<long *>((char *) base + findsym("/system/lib64/libc.so","strstr"));
    __android_log_print(6,"r0ysue","%p",st);
//    __android_log_print(6,"r0ysue","%p",(long)strstr);

    register_hook(addr,(void*)regist);

    register_hook(addr1,(void*)regist1);

    register_hook(st, (void*)(new_strstr));
    sleep(1);

//    dumpso("libAppGuard");
//    jclass myjclass=env->FindClass("android/app/ActivityThread");
//        jmethodID dsadsa=env->GetMethodID(myjclass,"per", "(Ljava/lang/String;)Ljava/lang/Class;");
//    const char* xxx= getclassname(env,myjclass);
//    __android_log_print(6,"r0ysue","i am class %s",xxx);
//    myhook(env, (void *)(add), myjclass, "add", "()I",1);


//    myhook(env, (void *)(myloadclass), myjclass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;",0);
//    myhook(env, (void *)(PathClassLoaderinit), myjclass, "<init>", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V",0);


    return JNI_VERSION_1_4;

}