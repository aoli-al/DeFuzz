#pragma once

#include <jni.h>
#include <jvmti.h>
#include <set>

extern bool fuzzer_started;

#ifdef __cplusplus
extern "C"
{
    JNIEXPORT jint JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_initNative(JNIEnv *env);
    JNIEXPORT jint JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_getNonZeroSizeNative(JNIEnv *env, jint index);
    JNIEXPORT jint JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_updateBitsNative(JNIEnv *env, jint a, jint b);
    JNIEXPORT void JNICALL Java_al_aoli_defuzz_fuzzer_runtime_BreakPointMethodVisitor_registerBreakPoint(JNIEnv *env, jstring class_name, jstring method_name, jstring method_descriptor, int loc);
    JNIEXPORT void JNICALL Java_al_aoli_defuzz_fuzzer_cli_Options_fuzzerStarted(JNIEnv *env);
}
#endif