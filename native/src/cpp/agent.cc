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

static defuzz::BreakPointClient *client;
std::map<std::string, defuzz::BreakPointResponse> breakpoints;

void JNICALL ClassLoad(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jclass clazz)
{
    char *signature;
    jvmti_env->GetClassSignature(clazz, &signature, NULL);
    std::string sig = signature;
    jvmti_env->Deallocate((unsigned char *)signature);
    PLOG_INFO << "Class loaded: " << sig;
    if (breakpoints.find(sig) != breakpoints.end()) {
        PLOG_INFO << "break points found for class: " << sig;
        for (auto breakpoint : breakpoints[sig].breakpoints())
        {
            PLOG_INFO << "Breakpoint: " << breakpoint.classname() << " " << breakpoint.methodname() << " " << breakpoint.methoddescriptor() << " " << breakpoint.location();
            if (breakpoint.isstatic()) {
                jmethodID method = jni_env->GetStaticMethodID(clazz, breakpoint.methodname().c_str(), breakpoint.methoddescriptor().c_str());
            } else {
                jmethodID method = jni_env->GetMethodID(clazz, breakpoint.methodname().c_str(), breakpoint.methoddescriptor().c_str());

            // jvmti_env->SetBreakpoint(method, breakpoint.location());
            }
        }
    }
    breakpoints.erase(sig);
}

void JNICALL ClassFileLoadHook(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jclass class_being_redefined, jobject loader, const char *name, jobject protection_domain, jint class_data_len, const unsigned char *class_data, jint *new_class_data_len, unsigned char **new_class_data)
{
    if (!fuzzer_started) {
        return;
    }
    std::string class_name = "L" + std::string(name) + ";";
    PLOG_INFO << "Class file loaded: " << class_name;
    if (class_name.starts_with("al/aoli/defuzz")) {
        return;
    }

    if (auto response = client->GetBreakPoints(class_data, class_data_len)) {
        PLOG_INFO << "Breakpoints found for class: " << response->breakpoints_size();
        breakpoints[class_name] = *response;
        // for (auto breakpoint : (*response).breakpoints())
        // {
        //     PLOG_INFO << "Breakpoint: " << breakpoint.classname() << " " << breakpoint.methodname() << " " << breakpoint.methoddescriptor() << " " << breakpoint.location();
        //     static jclass runtime_class = jni_env->FindClass(breakpoint.classname().c_str());
        //     static jmethodID method = jni_env->GetStaticMethodID(runtime_class, breakpoint.methodname().c_str(), breakpoint.methoddescriptor().c_str());
        //     jvmti_env->SetBreakpoint(method, breakpoint.location());
        // }
    }
}


void JNICALL Breakpoint(jvmtiEnv *jvmti_env, JNIEnv *jni_env, jthread thread, jmethodID method, jlocation location)
{
    PLOG_INFO << "Breakpoint triggered " << method << " " << location;
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
    callbacks.ClassLoad = &ClassLoad;
    callbacks.Breakpoint = &Breakpoint;
    jvmti->SetEventCallbacks(&callbacks, sizeof(callbacks));
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_BREAKPOINT, NULL);
    jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_LOAD, NULL);

    static plog::ColorConsoleAppender<plog::FuncMessageFormatter>
        console_appender;
    plog::init(plog::info, &console_appender);

    PLOG_INFO << "Agent loaded";

    return 0;
}