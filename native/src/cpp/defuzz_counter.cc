#include <jni.h>
#include <vector>
#include "runtime.hpp"
#include <plog/Log.h>
#include <iostream>
#include <mutex>


static std::vector<std::vector<uint8_t>> counters;
static int working_counter = -1;
const static int size = 64 * 1024 - 1;
const static int HOB_SIZE = 1024;
static int *HOB_CACHE = []() -> int *
{
    int *cache = new int[HOB_SIZE];
    for (int i = 0; i < HOB_SIZE; i++)
    {
        if (i == 0)
        {
            cache[i] = 0;
            continue;
        }

        int ret = 1;
        int j = i;
        while ((j >>= 1) != 0)
        {
            ret <<= 1;
        }

        cache[i] = ret;
    }
    return cache;
}();

int hob(int x)
{
    if (x < HOB_SIZE)
    {
        return HOB_CACHE[x];
    }
    else
    {
        return HOB_CACHE[HOB_SIZE - 1];
    }
}

JNIEXPORT void JNICALL Java_al_aoli_defuzz_fuzzer_runtime_DeFuzzRuntime_registerCounter(JNIEnv *env, jclass clazz, jint index)
{
    working_counter = index;
}

JNIEXPORT jint JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_initNative(JNIEnv *env, jclass clazz)
{
    counters.emplace_back(size, 0);
    return counters.size() - 1;
}

JNIEXPORT jintArray JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_getNonZeroIndicesNative(JNIEnv *env, jclass clazz, jint index)
{
    std::vector<jint> non_zero_indices;
    jint count = 0;
    for (auto v : counters[index])
    {
        if (v != 0)
        {
            non_zero_indices.push_back(count);
        }
    }
    jintArray jarray =  env->NewIntArray(non_zero_indices.size());
    env->SetIntArrayRegion(jarray, 0, non_zero_indices.size(), non_zero_indices.data());
    return jarray;
}

JNIEXPORT jintArray JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_computeNewCoverageNative(JNIEnv *env, jclass clazz, jint thisIndex, jint thatIndex)
{
    std::vector<jint> new_coverage;
    jint count = 0;
    for (int i = 0; i < size; i++)
    {
        if (counters[thatIndex][i] == 0 && counters[thisIndex][i] != 0)
        {
            new_coverage.push_back(i);
        }
    }
    jintArray jarray =  env->NewIntArray(new_coverage.size());
    env->SetIntArrayRegion(jarray, 0, new_coverage.size(), new_coverage.data());
    return jarray;

}


std::mutex mtx;

JNIEXPORT jint JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_updateBitsNative(JNIEnv *env, jclass clazz, jint a, jint b)
{
    std::unique_lock<std::mutex> lck(mtx);
    jint changed = 0;
    for (int i = 0; i < size; i++)
    {
        auto before = counters[a][i];
        auto after = before | hob(counters[b][i]);
        if (before != after)
        {
            counters[a][i] = after;
            changed |= 1;
        }
        if (before == 0)
        {
            changed |= 2;
        }
    }
    return changed;
}

JNIEXPORT void JNICALL Java_al_aoli_defuzz_fuzzer_runners_jqf_DeFuzzCounter_clearNative(JNIEnv *env, jclass clazz, jint index) {
    PLOG_INFO << "Counter cleared at index " << index;
    std::unique_lock<std::mutex> lck(mtx);
    counters[index].clear();
}

void JNICALL Breakpoint(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method, jlocation location)
{
    if (working_counter == -1)
    {
        return;
    }
    int idx = ((((location * 31) + (long) method) % size) + size) % size;
    PLOG_INFO << "Breakpoint triggered at " << location << " in method " << method << " at index " << idx;
    std::unique_lock<std::mutex> lck(mtx);
    counters[working_counter][idx] += 1;
}
