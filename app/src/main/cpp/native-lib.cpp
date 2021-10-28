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

static JNIEnv* g_pEnv;
static jobject g_jobj;

/**
 * 1.保存下JNIEnv和jobject，没有返回值，不传参
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ewan_exportlib_MainActivity_initialize(JNIEnv* env, jobject obj) {
    g_pEnv = env;
    g_jobj = obj;
    LOGD("no pass value no return");
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
    LOGD("passIntArray: %i,%i,%i",intArray[0], intArray[1], intArray[2]);
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
    return jfArray;
}

/**
 * 4.c++ call Java
 * 无返回值，无参数
 */
static void Initialize() {
    jclass clazz = g_pEnv->GetObjectClass(g_jobj);
    jmethodID id = g_pEnv->GetMethodID(clazz, "Initialize", "()V");
    g_pEnv->CallVoidMethod(g_jobj, id);
}

/**
 * 5.c++ call Java
 * 无返回值，参数为std::string
 * @param funcName
 */
static void PassStringValue(std::string funcName) {
    jclass clazz = g_pEnv->GetObjectClass(g_jobj);
    jmethodID id = g_pEnv->GetMethodID(clazz, "PassStringValue", "(Ljava/lang/String;)V");
    jstring str = g_pEnv->NewStringUTF(funcName.c_str());
    g_pEnv->CallVoidMethod(g_jobj, id, str);
}

/**
 * 6.c++ call Java
 * 返回值为string，无参数
 * @return
 */
static std::string ReturnString() {
    jclass clazz = g_pEnv->GetObjectClass(g_jobj);
    jmethodID id = g_pEnv->GetMethodID(clazz, "ReturnString", "()Ljava/lang/String;");
    jstring str = (jstring)g_pEnv->CallObjectMethod(g_jobj, id);
    const char* c_str = nullptr;
    c_str = g_pEnv->GetStringUTFChars(str, 0);
    g_pEnv->ReleaseStringUTFChars(str, c_str);
    return std::string (c_str);
}

/**
 * 被Java调起，用来调cpp的函数
 */
extern "C" JNIEXPORT void JNICALL
Java_com_ewan_exportlib_MainActivity_callNative(JNIEnv* env, jobject obj) {
    /**
     * 4. c++ call Java
     */
    Initialize();

    /**
     * 5. c++ call Java
     */
    std::string funcName = "PassStringValue";
    PassStringValue(funcName);

    /**
     * 6. c++ call Java
     */
    std::string str = ReturnString();
    LOGD("%s", str.c_str());
}



