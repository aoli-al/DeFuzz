#pragma once

#include <jni.h>
#include <jvmti.h>
#include <set>

extern bool fuzzer_started;

#ifdef __cplusplus
extern "C"
{
    JNIEXPORT jint JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_initNative(JNIEnv *env, jclass clazz);
    JNIEXPORT jintArray JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_getNonZeroIndicesNative(JNIEnv *env, jclass clazz, jint index);
    JNIEXPORT jint JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_updateBitsNative(JNIEnv *env, jclass clazz, jint a, jint b);
    JNIEXPORT void JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_clearNative(JNIEnv *env, jclass clazz, jint index);
    JNIEXPORT jintArray JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_computeNewCoverageNative(JNIEnv *env, jclass clazz, jint thisIndex, jint thatIndex);
    JNIEXPORT void JNICALL Java_al_aoli_defuzz_fuzzer_runtime_BreakPointMethodVisitor_registerBreakPoint(JNIEnv *env, jclass clazz, jstring class_name, jstring method_name, jstring method_descriptor, int loc);
    JNIEXPORT void JNICALL Java_al_aoli_defuzz_fuzzer_cli_Options_fuzzerStarted(JNIEnv *env, jclass clazz);
    JNIEXPORT void JNICALL Java_al_aoli_defuzz_fuzzer_runtime_DeFuzzRuntime_registerCounter(JNIEnv *env, jclass clazz, jint index);
}

extern void JNICALL Breakpoint(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method, jlocation location);
#endif