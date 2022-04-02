//
// Created by Sergei Golishnikov on 06/03/2022.
//

#include "WsHelper.h"
#include <react/jni/WritableNativeMap.h>

#include <utility>
#include "iostream"

using namespace facebook;
using namespace facebook::jni;

using TSelf = local_ref<HybridClass<WsHelper>::jhybriddata>;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
    return facebook::jni::initialize(vm, [] {
        WsHelper::registerNatives();
    });
}

// JNI binding
void WsHelper::registerNatives() {
    __android_log_print(ANDROID_LOG_VERBOSE, "😇", "registerNatives");
    registerHybrid({
                           makeNativeMethod("initHybrid",
                                            WsHelper::initHybrid),
                           makeNativeMethod("installJSIBindings",
                                            WsHelper::installJSIBindings),
                   });
}





void WsHelper::installJSIBindings(jstring certPath) {
    __android_log_print(ANDROID_LOG_VERBOSE, "😇", "registerJsiBindings");
    jsi::Object webSocketsObject = jsi::Object(*runtime_);
    //std::shared_ptr<jsi::Object> sharedObj = std::make_shared<jsi::Object>(webSocketsObject);

    ws = new jsiWs::JsiWs(runtime_, jsCallInvoker_);
    ws->installJSIBindings(make_local(certPath)->toStdString(), &webSocketsObject);


    runtime_->global().setProperty(*runtime_, "jsiIXWebSocket", std::move(webSocketsObject));
}


WsHelper::WsHelper(
        jni::alias_ref<WsHelper::javaobject> jThis,
        jsi::Runtime *rt,
        std::shared_ptr<facebook::react::CallInvoker> jsCallInvoker)
        : runtime_(rt),
          jsCallInvoker_(std::move(jsCallInvoker)) {}

// JNI init
TSelf WsHelper::initHybrid(
        alias_ref<jhybridobject> jThis,
        jlong jsContext,
        jni::alias_ref<facebook::react::CallInvokerHolder::javaobject>
        jsCallInvokerHolder) {

    __android_log_write(ANDROID_LOG_INFO, "🥲", "initHybrid...");
    auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();
    return makeCxxInstance(jThis, (jsi::Runtime *) jsContext, jsCallInvoker);
}
