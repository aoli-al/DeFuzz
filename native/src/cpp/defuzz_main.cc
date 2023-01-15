#include <jni.h>
#include "runtime.hpp"

bool fuzzer_started = false;

JNIEXPORT void JNICALL Java_al_aoli_defuzz_fuzzer_cli_Options_fuzzerStarted(JNIEnv *env) {
    fuzzer_started = true;
}