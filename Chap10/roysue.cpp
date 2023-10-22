//
// Created by ting on 2019-09-17.
//

#include <jni.h>
#include <string>
#include "aes_utils.h"
#include "tools.h"
#include "junk.h"
#include<android/log.h>
#include <unistd.h>
#include <bitset>
#include <fstream>


#define TAG "roysuejni" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型


#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

JNIEXPORT jstring JNICALL method02(JNIEnv *env, jclass jcls, jstring str_) {
    if (str_ == nullptr) return nullptr;

    const char *str = env->GetStringUTFChars(str_, JNI_FALSE);
    char *result = AES_128_CBC_PKCS5_Decrypt(str);

    env->ReleaseStringUTFChars(str_, str);

    jstring jResult = getJString(env, result);
    free(result);

    return jResult;
}


bool function_check_tracerPID() {
    bool b = false ;
    int pid = getpid();
    std::string file_name = "/proc/pid/status";
    std::string line;
    file_name.replace(file_name.find("pid"), 3, std::to_string(pid));
    LOGE("replace file name => %s", file_name.c_str());
    std::ifstream myfile(file_name, std::ios::in);
    if (myfile.is_open()) {
        while (getline(myfile, line)) {
            size_t TracerPid_pos = line.find("TracerPid");
            if (TracerPid_pos == 0) {
                line = line.substr(line.find(":") + 1);
                LOGE("file line => %s", line.c_str());
                if (std::stoi(line.c_str()) != 0) {
                    LOGE("trace pid => %s, i want to exit.", line.c_str());
                    b = true ;
//                    kill(pid, 9);
                    break;
                }
            }
        }
        myfile.close();
    }
    return b ;
}

bool system_getproperty_check() {
    char man[256], mod[156];
    /* A length 0 value indicates that the property is not defined */
    int lman = __system_property_get("ro.product.manufacturer", man);
    int lmod = __system_property_get("ro.product.model", mod);
    int len = lman + lmod;
    char *pname = NULL;
    if (len > 0) {
        pname = static_cast<char *>(malloc(len + 2));
        snprintf(pname, len + 2, "%s/%s", lman > 0 ? man : "", lmod > 0 ? mod : "");
    }

    bool b = false;
    if(strstr(pname,"Google"))b=true;
    LOGE("[roysue device]: [%s] result is => %d\n", pname ? pname : "N/A",b);
    return b;
}

JNIEXPORT jstring JNICALL fuck(JNIEnv *env, jclass jcls, jstring str_) {
    if (str_ == nullptr) return nullptr;

    char *str = const_cast<char *>(env->GetStringUTFChars(str_, JNI_FALSE));

    char* sign = "REAL";

    jclass buildClazz = env->FindClass("android/os/Build");
    jfieldID FINGERPRINT = env->GetStaticFieldID(buildClazz,"FINGERPRINT","Ljava/lang/String;");
    jstring fingerprint = static_cast<jstring>(env->GetStaticObjectField(buildClazz, FINGERPRINT));

    if( function_check_tracerPID() ||  system_getproperty_check() || strstr( env->GetStringUTFChars(fingerprint, JNI_FALSE),"aosp")){
        sign = "FAKE";
    }

//    rightsign : 18d1fb4c2bac56c180f763e359d1d717500787ee90b32ed12f588e51f60e458d08303dcff7dd08a39b13c75e3f1ba9d9
//
// before entering aes => requestUserInfoFAKE
// wrongsign:bc57f886b8822f0465fb87bedd58c4ba9bad4efc9e7556df925701f0427c72cb8b23a1c25ff9241f8199cb6db5437fe2

    strcat(str,sign);
    jstring jstr = getJString(env, str);


    LOGI("before entering aes => %s",str);

    ////    byte[] bytes = MessageDigest.getInstance("MD5").digest(input.getBytes());

    jclass MessageGigest = env->FindClass("java/security/MessageDigest");
    jmethodID getInstance = env->GetStaticMethodID(MessageGigest, "getInstance",
                                                   "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    jobject md5 = env->CallStaticObjectMethod(MessageGigest, getInstance,
                                              getJString(env, "MD5"));
    jmethodID digest = env->GetMethodID(MessageGigest, "digest", "([B)[B");

    jclass stringClazz = env->FindClass("java/lang/String");
    jmethodID getbytes = env->GetMethodID(stringClazz, "getBytes", "()[B");
    jbyteArray jba = static_cast<jbyteArray>(env->CallObjectMethod(jstr, getbytes));
    jbyteArray md5result = static_cast<jbyteArray>(env->CallObjectMethod(md5, digest, jba));

    char *cmd5 = reinterpret_cast<char *>(env->GetByteArrayElements(md5result, 0));

    char md5string[33];
    for(int i = 0; i < 16; ++i)
        sprintf(&md5string[i*2], "%02x", (unsigned int)cmd5[i]);

    char *result = AES_128_CBC_PKCS5_Encrypt(str);
//    strcat(result,"XXXXXX");
    strcat(result,md5string);

    jstring finalResult = getJString(env, result);

    env->ReleaseStringUTFChars(str_, str);

    free(result);

    return finalResult;
}


static JNINativeMethod method_table[] = {
        {"decrypt", "(Ljava/lang/String;)Ljava/lang/String;", (void *) method02},
        {"Sign", "(Ljava/lang/String;)Ljava/lang/String;", (void *) fuck},
//        (参数1类型标示；参数2类型标示；参数3类型标示...)返回值类型标示
};

static int registerMethods(JNIEnv *env, const char *className,
                           JNINativeMethod *gMethods, int numMethods) {
    jclass clazz = env->FindClass(className);
    if (clazz == nullptr) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jstring JNICALL Java_com_roysue_easyso1_MainActivity_method01(JNIEnv *env, jclass jcls, jstring str_) {
    if (str_ == nullptr) return nullptr;

    const char *str = env->GetStringUTFChars(str_, JNI_FALSE);
    char *result = AES_128_CBC_PKCS5_Encrypt(str);

    env->ReleaseStringUTFChars(str_, str);

    jstring jResult = getJString(env, result);
    free(result);

    return jResult;
}


JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    _JUNK_FUN_0

    JNIEnv *env = nullptr;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }
    assert(env != nullptr);

// 注册native方法
    if (!registerMethods(env, "com/roysue/easyso1/MainActivity", method_table,
                         NELEM(method_table))) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}


#ifdef __cplusplus
}
#endif