package com.reactnativejsiixwebsocket;

import android.util.Log;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.module.annotations.ReactModule;

@ReactModule(name = JsiIXWebSocketModule.NAME)
public class JsiIXWebSocketModule extends ReactContextBaseJavaModule {
  public static final String NAME = "JsiIxwebsocket";

  private WsHelper wsHelper;

  public JsiIXWebSocketModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @Override
  @NonNull
  public String getName() {
    return NAME;
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      System.loadLibrary("react-native-jsi-ixwebsocket");
      wsHelper = new WsHelper();
      wsHelper.install(getReactApplicationContext());
      return true;
    } catch (Exception exception) {
      Log.e(NAME, "Failed to install JSI Bindings!", exception);
      return false;
    }
  }
}
