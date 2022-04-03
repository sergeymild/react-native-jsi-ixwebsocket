#include "iostream"

#include "Logger.h"
#include "JsiWs.h"

using namespace facebook;
namespace jsiWs {


JsiWs::JsiWs(
             jsi::Runtime *rt,
             std::shared_ptr<facebook::react::CallInvoker> jsCallInvoker)
: runtime_(rt),
jsCallInvoker_(std::move(jsCallInvoker)) {}


void JsiWs::installJSIBindings(std::string certPath) {
    this->certPath_ = certPath;
    auto connect = jsi::Function::createFromHostFunction(*runtime_,
                                                         jsi::PropNameID::forUtf8(*runtime_,
                                                                                  "connect"),
                                                         1,
                                                         [this](jsi::Runtime &runtime,
                                                                const jsi::Value &thisArg,
                                                                const jsi::Value *args,
                                                                size_t count) -> jsi::Value {
        
        auto params = args[0].asObject(runtime);
        auto endpoint = params.getProperty(runtime,"endpoint").asString(runtime).utf8(runtime);
        jsi::Value rawHeaders = params.getProperty(runtime, "headers");
        
        ix::WebSocketHttpHeaders headers;
        if (!rawHeaders.isUndefined() && !rawHeaders.isNull()) {
            auto headersObject = rawHeaders.asObject(runtime);
            jsi::Array propertyNames = headersObject.getPropertyNames(runtime);
            size_t size = propertyNames.size(runtime);
            for (size_t i = 0; i < size; i++) {
                jsi::String name = propertyNames.getValueAtIndex(runtime, i).getString(runtime);
                jsi::String value = headersObject.getProperty(runtime, name).asString(runtime);
                headers[name.utf8(runtime)] = value.utf8(runtime);
            }
        }
        
        wsConnect(endpoint, headers);
        
        
        return jsi::Value::undefined();
    });
    
    auto close = jsi::Function::createFromHostFunction(*runtime_,
                                                       jsi::PropNameID::forUtf8(*runtime_,
                                                                                "close"),
                                                       1,
                                                       [this](jsi::Runtime &runtime,
                                                              const jsi::Value &thisArg,
                                                              const jsi::Value *args,
                                                              size_t count) -> jsi::Value {
        
        wsClose();
        return jsi::Value::undefined();
    });
    
    auto sendMessage = jsi::Function::createFromHostFunction(*runtime_,
                                                             jsi::PropNameID::forUtf8(*runtime_,
                                                                                      "sendMessage"),
                                                             1,
                                                             [this](jsi::Runtime &runtime,
                                                                    const jsi::Value &thisArg,
                                                                    const jsi::Value *args,
                                                                    size_t count) -> jsi::Value {
        auto result = wsSendMessage(args[0].asString(runtime).utf8(runtime));
        return jsi::Value(result);
    });
    
    
    auto state = jsi::Function::createFromHostFunction(*runtime_,
                                                       jsi::PropNameID::forUtf8(*runtime_,
                                                                                "state"),
                                                       0,
                                                       [this](jsi::Runtime &runtime,
                                                              const jsi::Value &thisArg,
                                                              const jsi::Value *args,
                                                              size_t count) -> jsi::Value {
        return jsi::String::createFromUtf8(runtime, wsState());
    });
    
    auto registerCallback = jsi::Function::createFromHostFunction(
                                                                  *runtime_,
                                                                  jsi::PropNameID::forUtf8(*runtime_, "registerCallback"),
                                                                  2,
                                                                  [this](jsi::Runtime &runtime,
                                                                         const jsi::Value &thisArg,
                                                                         const jsi::Value *args,
                                                                         size_t count) -> jsi::Value {
                                                                             
                                                                             auto name = args[0].asString(runtime).utf8(runtime);
                                                                             auto callback = args[1].asObject(runtime).asFunction(runtime);
                                                                             callbacks[name] = std::make_shared<jsi::Function>(std::move(callback));
                                                                             return jsi::Value::undefined();
                                                                         });
    
    auto removeCallback = jsi::Function::createFromHostFunction(
                                                                *runtime_,
                                                                jsi::PropNameID::forUtf8(*runtime_, "removeCallback"),
                                                                1,
                                                                [=](jsi::Runtime &runtime,
                                                                    const jsi::Value &thisArg,
                                                                    const jsi::Value *args,
                                                                    size_t count) -> jsi::Value {
                                                                        
                                                                        auto name = args[0].asString(runtime).utf8(runtime);
                                                                        callbacks.erase(name);
                                                                        return jsi::Value::undefined();
                                                                    });
    
    jsi::Object jsObject = jsi::Object(*runtime_);
    jsObject.setProperty(*runtime_, "registerCallback", std::move(registerCallback));
    jsObject.setProperty(*runtime_, "removeCallback", std::move(removeCallback));
    jsObject.setProperty(*runtime_, "state", std::move(state));
    jsObject.setProperty(*runtime_, "connect", std::move(connect));
    jsObject.setProperty(*runtime_, "close", std::move(close));
    jsObject.setProperty(*runtime_, "sendMessage", std::move(sendMessage));
    runtime_->global().setProperty(*runtime_, "jsiIXWebSocket", std::move(jsObject));
}

void JsiWs::sendMessageToJs(const std::string messageType, const std::string message) {
    jsiWs::log("sendMessageToJs " + message);
    jsCallInvoker_->invokeAsync([=]() {
        std::shared_ptr<jsi::Function> c = callbacks[messageType];
        if (!c) return;
        c->call(*runtime_, message);
    });
}


void JsiWs::wsConnect(const std::string &endpoint, ix::WebSocketHttpHeaders headers) {
    webSocket_.setExtraHeaders(headers);
    webSocket_.setUrl(endpoint);
#if ONANDROID
    ix::SocketTLSOptions options;
    if (!certPath_.empty()) {
        options.caFile = certPath_;
    } else {
        options.caFile = "NONE";
    }
    webSocket_.setTLSOptions(options);
#endif
    jsiWs::log("Connecting to " + endpoint);
    
    webSocket_.setOnMessageCallback([this](const ix::WebSocketMessagePtr &msg) {
        
        if (msg->type == ix::WebSocketMessageType::Message) {
            sendMessageToJs("onMessage", msg->str);
        } else if (msg->type == ix::WebSocketMessageType::Open) {
            sendMessageToJs("onOpen", "");
        } else if (msg->type == ix::WebSocketMessageType::Error) {
            // Maybe SSL is not configured properly
            sendMessageToJs("onError", msg->errorInfo.reason);
        }
    });
    
    // Now that our callback is setup, we can start our background thread and receive messages
    webSocket_.start();
    jsiWs::log("complete connect");
}


void JsiWs::wsClose() {
    jsiWs::log("wsClose");
    webSocket_.stop();
}


std::string JsiWs::wsState() {
    jsiWs::log("wsState");
    return ix::WebSocket::readyStateToString(webSocket_.getReadyState());
}


bool JsiWs::wsSendMessage(const std::string &message) {
    if (webSocket_.getReadyState() == ix::ReadyState::Open) {
        jsiWs::log("wsSendMessage " + message);
        webSocket_.sendUtf8Text(message);
        return true;
    }
    
    return false;
}

}
