#import "JsiIxwebsocket.h"
#import <React/RCTBlobManager.h>
#import <React/RCTUIManager.h>
#import <React/RCTBridge+Private.h>
#import <ReactCommon/RCTTurboModule.h>
#import <jsi/jsi.h>
#import "JsiWs.h"

using namespace facebook;

@implementation JsiIxwebsocket

jsiWs::JsiWs *ws;

RCT_EXPORT_MODULE()

RCT_EXPORT_BLOCKING_SYNCHRONOUS_METHOD(install) {
    NSLog(@"Installing JsiWebsockets polyfill Bindings...");
    auto _bridge = [RCTBridge currentBridge];
    auto _cxxBridge = (RCTCxxBridge*)_bridge;
    if (_cxxBridge == nil) return @false;
    jsi::Runtime* _runtime = (jsi::Runtime*) _cxxBridge.runtime;
    if (_runtime == nil) return @false;
    
    jsi::Object webSocketsObject = jsi::Object(*_runtime);
    
    ws = new jsiWs::JsiWs(_runtime, _bridge.jsCallInvoker);
    ws->installJSIBindings("", &webSocketsObject);


    _runtime->global().setProperty(*_runtime, "jsiIXWebSocket", std::move(webSocketsObject));

    return @true;
}

@end
