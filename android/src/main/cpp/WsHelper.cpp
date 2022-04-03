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
    registerHybrid({
                           makeNativeMethod("initHybrid",
                                            WsHelper::initHybrid),
                           makeNativeMethod("installJSIBindings",
                                            WsHelper::installJSIBindings),
                   });
}





void WsHelper::installJSIBindings(jstring certPath) {
    ws = new jsiWs::JsiWs(runtime_, jsCallInvoker_);
    ws->installJSIBindings(make_local(certPath)->toStdString());
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

    auto jsCallInvoker = jsCallInvokerHolder->cthis()->getCallInvoker();
    return makeCxxInstance(jThis, (jsi::Runtime *) jsContext, jsCallInvoker);
}
