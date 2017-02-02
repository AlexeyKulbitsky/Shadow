#include <jni.h>
#include <string>

extern "C"
jstring
Java_com_shadow_alexeykulbitsky_myapplication_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
