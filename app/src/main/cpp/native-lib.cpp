#include <jni.h>
#include <string>
#include "log/log_tool.h"

static JNIEnv *pEnv;
static jobject jObj;
static jmethodID setDeviceRotId;

extern "C" JNIEXPORT jstring JNICALL
Java_com_ewan_exportlib_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject  obj) {
    pEnv = env;
    jObj = obj;
    std::string hello = "Hello from C++";
    LOGD("hi.");
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jfloatArray JNICALL
Java_com_ewan_exportlib_MainActivity_sendDeviceRot(
        JNIEnv* env,
        jobject obj,
        jfloatArray array) {
    /**
     * Get the length of array that passed by Java call
     */
    int length = env->GetArrayLength(array);
    /**
     * Get the point of array (Cpp array)
     */
    jfloat* jf = env->GetFloatArrayElements(array, 0);
    /**
     * Per element of array plus one
     */
    for (int i = 0; i < length; i++) {
        *(jf + i) += 1.0f;
    }
    /**
     * Set Cpp array to Java array
     */
    env->SetFloatArrayRegion(array, 0, length, jf);
    /**
     * If GetFloatArrayElements , must release it!
     */
    env->ReleaseFloatArrayElements(array, jf, 0);
    /**
     * Return Java array
     */
    return array;
}
