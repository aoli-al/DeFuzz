#include <jvmti.h>
#include <string>
#include <map>

#include "plog/Appenders/ColorConsoleAppender.h"
#include "plog/Formatters/FuncMessageFormatter.h"
#include "plog/Init.h"
#include "plog/Log.h"
#include "breakpoint_client.hpp"
#include "runtime.hpp"

jvmtiEnv *jvmti;
std::mutex mu;

static defuzz::BreakPointClient *client;
std::map<std::string, defuzz::BreakPointResponse> breakpoints;

void JNICALL ClassPrepare(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jclass clazz)
{
    char *signature;
    jvmti_env->GetClassSignature(clazz, &signature, NULL);
    std::string sig = signature;
    jvmti_env->Deallocate((unsigned char *)signature);
    PLOG_INFO << "ClassPrepare entered:" << sig;

    defuzz::BreakPointResponse response;
    {
        std::unique_lock<std::mutex> lock(mu);
        if (breakpoints.find(sig) != breakpoints.end())
        {
            response = breakpoints[sig];
            breakpoints.erase(sig);
        }
    }
    for (auto breakpoint : response.breakpoints())
    {
        PLOG_INFO << "Set breakpoint: " << breakpoint.classname() << " " << breakpoint.methodname() << " " << breakpoint.methoddescriptor() << " " << breakpoint.location();
        if (breakpoint.isstatic())
        {
            jmethodID method = jni_env->GetStaticMethodID(clazz, breakpoint.methodname().c_str(), breakpoint.methoddescriptor().c_str());
            jvmti_env->SetBreakpoint(method, breakpoint.location());
        }
        else
        {
            jmethodID method = jni_env->GetMethodID(clazz, breakpoint.methodname().c_str(), breakpoint.methoddescriptor().c_str());
            jvmti_env->SetBreakpoint(method, breakpoint.location());
        }
    }
}

void JNICALL ClassFileLoadHook(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jclass class_being_redefined, jobject loader, const char *name, jobject protection_domain, jint class_data_len, const unsigned char *class_data, jint *new_class_data_len, unsigned char **new_class_data)
{
    PLOG_INFO << "ClassFileLoadHook entered: " << name;
    if (!fuzzer_started)
    {
        return;
    }
    std::string class_name = "L" + std::string(name) + ";";
    if (!class_name.starts_with("Lorg/apache/bcel"))
    {
        return;
    }

    if (auto response = client->GetBreakPoints(class_data, class_data_len))
    {
        std::unique_lock<std::mutex> lock(mu);
        breakpoints[class_name] = *response;
    }
}

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved)
{
    client = new defuzz::BreakPointClient(grpc::CreateChannel("localhost:8980", grpc::InsecureChannelCredentials()));
    vm->GetEnv((void **)&jvmti, JVMTI_VERSION_1_0);
    jvmtiCapabilities capabilities = {0};
    capabilities.can_generate_breakpoint_events = 1;
    capabilities.can_generate_all_class_hook_events = 1;
    capabilities.can_get_bytecodes = 1;
    jvmti->AddCapabilities(&capabilities);

    jvmtiEventCallbacks callbacks = {0};
    callbacks.ClassFileLoadHook = &ClassFileLoadHook;
    callbacks.ClassPrepare = &ClassPrepare;
    callbacks.Breakpoint = &Breakpoint;
    jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_BREAKPOINT, NULL);
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_PREPARE, NULL);

    static plog::ColorConsoleAppender<plog::FuncMessageFormatter>
        console_appender;
    plog::init(plog::error, &console_appender);

    PLOG_INFO << "Agent loaded";

    return 0;
}