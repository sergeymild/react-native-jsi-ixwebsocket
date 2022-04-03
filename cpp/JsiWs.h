#import <jsi/jsi.h>
#include "map"
#include <ReactCommon/CallInvoker.h>
#include "ixwebsocket/IXWebSocket.h"

using namespace facebook;
namespace jsiWs {

class JsiWs {

public:
    void installJSIBindings(std::string certPath);
    explicit JsiWs(
                facebook::jsi::Runtime *rt,
                std::shared_ptr<facebook::react::CallInvoker> jsCallInvoker);


private:
    std::map<std::string, std::shared_ptr<jsi::Function>> callbacks;
    std::shared_ptr<react::CallInvoker> jsCallInvoker_;
    facebook::jsi::Runtime *runtime_;
    std::string certPath_;
    ix::WebSocket webSocket_;

    void wsConnect(const std::string& endpoint, ix::WebSocketHttpHeaders headers);
    void wsClose();
    bool wsSendMessage(const std::string& message);
    std::string wsState();
    
    void sendMessageToJs(const std::string messageType, const std::string message);

};


}
