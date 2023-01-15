#include <jni.h>
#include <vector>
#include "runtime.hpp"

static std::vector<std::vector<uint8_t>> counters;
const static int size = 64 * 1024;
const static int HOB_SIZE = 1024;
static int* HOB_CACHE = nullptr;
//static int* HOB_CACHE = []() -> int* {
//    int* cache = new int[HOB_SIZE];
//    for (int i = 0; i < HOB_SIZE; i++) {
//        if (i == 0) {
//            cache[i] = 0;
//            continue;
//        }
//
//        int ret = 1;
//        while ((i >>= 1) != 0) {
//            ret <<= 1;
//        }
//
//        cache[i] = ret;
//    }
//    return cache;
//}();

int hob(int x) {
    if (x < HOB_SIZE) {
        return HOB_CACHE[x];
    } else {
        return HOB_CACHE[HOB_SIZE - 1];
    }
}



JNIEXPORT jint JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_initNative(JNIEnv *env) {
    counters.emplace_back(size, 0);
}

JNIEXPORT jint JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_getNonZeroSizeNative(JNIEnv *env, jint index) {
    jint count = 0;
    for (auto v: counters[index]) {
        if (v != 0) {
            count++;
        }
    }
    return count;
}

JNIEXPORT jint JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_updateBitsNative(JNIEnv *env, jint a, jint b) {
    jint changed = 0;
    for (int i = 0; i < size; i++) {
        auto before = counters[a][i];
        auto after = before | hob(counters[b][i]);
        if (before != after) {
            counters[a][i] = after;
            changed |= 1;
        }
        if (before == 0) {
            changed |= 2;
        }
    }
    return changed;
}