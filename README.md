# react-native-jsi-ixwebsocket

React Native JSI wrapper for [IXWebSocket](https://github.com/sergeymild/react-native-jsi-ixwebsocket) websocket library

###Features
- High performance because everything is written in C++ (even the JS functions have C++ bodies!)
- iOS, Android support

## Installation

```sh
#add to package.json
"react-native-jsi-ixwebsocket":"sergeymild/react-native-jsi-ixwebsocket#0.7.0"
# after that make yarn install
# and npx pod-install
```

### Import
```typescript
import { ixWebSocket } from 'react-native-jsi-ixwebsocket';
```

### Types
```typescript
export type JsiIXWebSocketState = 'CONNECTING' | 'OPEN' | 'CLOSING' | 'CLOSED';
export type JsiIXWebSocketCallback =
  | 'onOpen'
  | 'onMessage'
  | 'onClose'
  | 'onError';
export type JsiIXWebSocketError = string;

export type ConnectParams = {
  endpoint: string
  headers?: Record<string, string>
}

type OnErrorCallback = (error: JsiIXWebSocketError) => void;
type OnMessageCallback = (message: string) => void;
type OnOpenCallback = () => void;
type OnCloseCallback = () => void;
```

### Callbacks
```typescript
ixWebSocket.onError((reason: JsiIXWebSocketError) => {});
ixWebSocket.onOpen(() => {});
ixWebSocket.onMessage((message: string) => {});
ixWebSocket.onClose(() => {});
```

### Methods
```typescript
// unregister callbacks
ixWebSocket.unsubscribeAll();
ixWebSocket.unsubscribe(name: JsiIXWebSocketCallback)
// open connection
ixWebSocket.connect(params: ConnectParams)
// close connection
ixWebSocket.close();
// get current connection state will return JsiIXWebSocketState
ixWebSocket.state()
// send message
ixWebSocket.sendMessage(string)
```

## Contributing

See the [contributing guide](CONTRIBUTING.md) to learn how to contribute to the repository and the development workflow.

## License

MIT
