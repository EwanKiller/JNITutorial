#include <jni.h>
#include <string>
#include "log/log_tool.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_ewan_exportlib_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject  obj) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

//-------------------------

/**
 * 保存一个JavaVM
 */
static JavaVM* g_jvm;
/**
 * 保存MainActivity这个对象
 */
static jobject g_obj;


JNIEnv* get_env(int* attach) {
    if (g_jvm == NULL) return NULL;
    JNIEnv* env = NULL;
    int status = g_jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
    if (status == JNI_EDETACHED || env == NULL)
    {
        status = g_jvm->AttachCurrentThread(&env, NULL);
        if (status < 0)
        {
            env = NULL;
        } else {
            *attach = 1;
        }
    }
    return env;
}

void del_env() {
    g_jvm->DetachCurrentThread();
}


/**
 * 1.保存下JNIEnv，没有返回值，不传参
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ewan_exportlib_MainActivity_initialize(JNIEnv* env, jobject obj) {
    LOGV("[CPP] Java call C++, no return, no param");
    g_obj = env->NewGlobalRef(obj);
}

/**
 * 2.没有返回值，传参 int数组
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ewan_exportlib_MainActivity_passIntArray(JNIEnv* env, jobject obj, jintArray array) {
    jint* pInt = env->GetIntArrayElements(array, 0);
    int length = env->GetArrayLength(array);
    int intArray[length];
    for (int i = 0; i < length; i++) {
        intArray[i] = *(pInt + i);
    }
    LOGV("[CPP] Java call C++, no return , pass int array : %i,%i,%i", intArray[0], intArray[1], intArray[2]);
}

/**
 * 3.返回值为float数组，不传参
 */
extern "C" JNIEXPORT jfloatArray JNICALL
Java_com_ewan_exportlib_MainActivity_returnFloatArray(JNIEnv* env, jobject obj) {
    float value[3] = { 1.0f, 2.0f, 3.0f};
    int length = std::end(value) - std::begin(value);
    jfloatArray jfArray = env->NewFloatArray(length);
    env->SetFloatArrayRegion(jfArray, 0, length, value);
    LOGV("[CPP] Java call C++, return float array: %f, %f, %f , no param", value[0], value[1], value[2]);
    return jfArray;
}

/**
 * 4.c++ call Java
 * 无返回值，无参数
 */
static void Initialize() {
    int* attach = 0;
    JNIEnv* env = get_env(attach);
    jclass clazz = env->GetObjectClass(g_obj);
    jmethodID id = env->GetMethodID(clazz, "Initialize", "()V");
    env->CallVoidMethod(g_obj, id);
    LOGV("[CPP] C++ call Java, no return, no param");
}

/**
 * 5.c++ call Java
 * 无返回值，参数为std::string
 * @param funcName
 */
static void PassStringValue(std::string funcName) {
    int* attach = 0;
    JNIEnv* env = get_env(attach);
    jclass clazz = env->GetObjectClass(g_obj);
    jmethodID id = env->GetMethodID(clazz, "PassStringValue", "(Ljava/lang/String;)V");
    jstring str = env->NewStringUTF(funcName.c_str());
    env->CallVoidMethod(g_obj, id, str);
}

/**
 * 6.c++ call Java
 * 返回值为string，无参数
 * @return
 */
static std::string ReturnString() {
    int* attach = 0;
    JNIEnv* env = get_env(attach);
    jclass clazz = env->GetObjectClass(g_obj);
    jmethodID id = env->GetMethodID(clazz, "ReturnString", "()Ljava/lang/String;");
    jstring str = (jstring)env->CallObjectMethod(g_obj, id);
    const char* c_str = nullptr;
    c_str = env->GetStringUTFChars(str, 0);
    env->ReleaseStringUTFChars(str, c_str);
    return std::string (c_str);
}

/**
 * 被Java调起，用来调cpp的函数
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ewan_exportlib_MainActivity_callNative(JNIEnv* env, jobject obj) {

    LOGV("[CPP] C++ call Java, no return, no param");
    Initialize();

    LOGV("[CPP] C++ call Java, no return, pass string");
    std::string funcName = "PassStringValue";
    PassStringValue(funcName);

    LOGV("[CPP] C++ call Java, return string, no param");
    std::string str = ReturnString();
    LOGV("[CPP] C++ call Java , return string : %s", str.c_str());
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void *reserved)
{
    g_jvm= vm;
    //--显示注册Native methods,可以预先检查符合是否存在，可以获得更小运行更快的库
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
    {
        return JNI_ERR;
    }
    jclass clz = env->FindClass("com/ewan/exportlib/MainActivity");
    if (clz == nullptr)
    {
        return JNI_ERR;
    }
    static const JNINativeMethod methods[] = {
            {"initialize", "()V", reinterpret_cast<void*>(Java_com_ewan_exportlib_MainActivity_initialize)},
            {"passIntArray","([I)V", reinterpret_cast<void*>(Java_com_ewan_exportlib_MainActivity_passIntArray)},
            {"returnFloatArray","()[F", reinterpret_cast<void*>(Java_com_ewan_exportlib_MainActivity_returnFloatArray)},
            {"callNative", "()V", reinterpret_cast<void*>(Java_com_ewan_exportlib_MainActivity_callNative)},
    };
    int rc = env->RegisterNatives(clz, methods, sizeof(methods)/sizeof(JNINativeMethod));
    if (rc != JNI_OK) return rc;

    return JNI_VERSION_1_6;
}


