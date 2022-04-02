import { NativeModules, Platform } from 'react-native';

const LINKING_ERROR =
  `The package 'react-native-jsi-ixwebsocket' doesn't seem to be linked. Make sure: \n\n` +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo managed workflow\n';

const JsiIXWebSocket = NativeModules.JsiIxwebsocket
  ? NativeModules.JsiIxwebsocket
  : new Proxy(
      {},
      {
        get() {
          throw new Error(LINKING_ERROR);
        },
      }
    );

JsiIXWebSocket.install();

export type JsiIXWebSocketState = 'CONNECTING' | 'OPEN' | 'CLOSING' | 'CLOSED';
export type JsiIXWebSocketCallback =
  | 'onOpen'
  | 'onMessage'
  | 'onClose'
  | 'onError';
export type JsiIXWebSocketError = string;

type OnErrorCallback = (error: JsiIXWebSocketError) => void;
type OnMessageCallback = (message: string) => void;
type OnOpenCallback = () => void;
type OnCloseCallback = () => void;

declare global {
  var jsiIXWebSocket: {
    connect: (data: { endpoint: string }) => void;
    close: () => void;
    state: () => JsiIXWebSocketState;
    sendMessage: (message: string) => boolean;
    onOpen: (callback: OnOpenCallback) => void;
    onMessage: (callback: OnMessageCallback) => void;
    onClose: (callback: OnCloseCallback) => void;
    onError: (callback: OnErrorCallback) => void;
    unsubscribe: (callback: JsiIXWebSocketCallback) => void;
  };
}

class _ixWebSocket {
  static connect(endpoint: string) {
    global.jsiIXWebSocket.connect({ endpoint });
  }

  static close() {
    global.jsiIXWebSocket.close();
  }

  static state(): JsiIXWebSocketState {
    return global.jsiIXWebSocket.state();
  }

  static sendMessage(message: string): boolean {
    return global.jsiIXWebSocket.sendMessage(message);
  }

  static onOpen(callback: OnOpenCallback) {
    //@ts-ignore
    global.jsiIXWebSocket.registerCallback('onOpen', callback);
  }

  static onMessage(callback: OnMessageCallback) {
    //@ts-ignore
    global.jsiIXWebSocket.registerCallback('onMessage', callback);
  }

  static onClose(callback: OnCloseCallback) {
    //@ts-ignore
    global.jsiIXWebSocket.registerCallback('onClose', callback);
  }

  static onError(callback: OnErrorCallback) {
    //@ts-ignore
    global.jsiIXWebSocket.registerCallback('onError', callback);
  }

  static unsubscribe(callback: JsiIXWebSocketCallback) {
    //@ts-ignore
    global.jsiIXWebSocket.removeCallback(callback);
  }

  static unsubscribeAll() {
    //@ts-ignore
    global.jsiIXWebSocket.removeCallback('onOpen');
    //@ts-ignore
    global.jsiIXWebSocket.removeCallback('onMessage');
    //@ts-ignore
    global.jsiIXWebSocket.removeCallback('onClose');
    //@ts-ignore
    global.jsiIXWebSocket.removeCallback('onError');
  }
}

export { _ixWebSocket as ixWebSocket };
