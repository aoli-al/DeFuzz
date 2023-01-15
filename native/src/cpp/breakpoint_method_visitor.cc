#include <jni.h>
#include "runtime.hpp"
#include "agent.hpp"


JNIEXPORT void JNICALL Java_al_aoli_defuzz_fuzzer_runtime_BreakPointMethodVisitor_registerBreakPoint(JNIEnv *env, jstring class_name, jstring method_name, jstring method_descriptor, int loc) {
    char *class_name_c = (char *) env->GetStringUTFChars(class_name, JNI_FALSE);
    char *method_name_c = (char *) env->GetStringUTFChars(method_name, JNI_FALSE);
    char *method_descriptor_c = (char *) env->GetStringUTFChars(method_descriptor, JNI_FALSE);
    jclass clazz = env->FindClass(class_name_c);
    jmethodID methodId = env->GetMethodID(clazz, method_name_c, method_descriptor_c);
    jvmti->SetBreakpoint(methodId, loc);
}