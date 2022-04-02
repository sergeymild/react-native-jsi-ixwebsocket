//
// Created by Sergei Golishnikov on 06/03/2022.
//

#include <fbjni/fbjni.h>
#include <jsi/jsi.h>
#include <ReactCommon/CallInvokerHolder.h>
#include <map>
#include "JsiWs.h"

class WsHelper : public facebook::jni::HybridClass<WsHelper> {

public:
    static constexpr auto kJavaDescriptor = "Lcom/reactnativejsiixwebsocket/WsHelper;";
    static facebook::jni::local_ref<jhybriddata> initHybrid(
            facebook::jni::alias_ref<jhybridobject> jThis,
            jlong jsContext,
            facebook::jni::alias_ref<facebook::react::CallInvokerHolder::javaobject> jsCallInvokerHolder);

    static void registerNatives();

    void installJSIBindings(jstring certPath);

private:
    friend HybridBase;
    facebook::jsi::Runtime *runtime_;
    jsiWs::JsiWs *ws;
    std::shared_ptr<facebook::react::CallInvoker> jsCallInvoker_;
    explicit WsHelper(
            facebook::jni::alias_ref<WsHelper::jhybridobject> jThis,
            facebook::jsi::Runtime *rt,
            std::shared_ptr<facebook::react::CallInvoker> jsCallInvoker);
};


