cmake_minimum_required(VERSION 3.10)

set (CMAKE_VERBOSE_MAKEFILE ON)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-DFOLLY_NO_CONFIG=1 -DFOLLY_HAVE_CLOCK_GETTIME=1 -DFOLLY_HAVE_MEMRCHR=1 -DFOLLY_USE_LIBCPP=1 -DFOLLY_MOBILE=1 -DON_ANDROID -DONANDROID -DFOR_HERMES=${FOR_HERMES}")

set (PACKAGE_NAME "react-native-jsi-ixwebsocket")
set (BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set (CPPHTTPLIB_ZLIB_SUPPORT 0)
set (RN_SO_DIR ${NODE_MODULES_DIR}/react-native/ReactAndroid/src/main/jni/first-party/react/jni)
#add_definitions(-DCPPHTTPLIB_OPENSSL_SUPPORT)
add_definitions(-DIXWEBSOCKET_USE_OPEN_SSL)
add_definitions(-DIXWEBSOCKET_USE_TLS)
# JSI-WS shared

if(${REACT_NATIVE_VERSION} LESS 66)
        set (
                INCLUDE_JSI_CPP
                "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi/jsi/jsi.cpp"
        )
        set (
                INCLUDE_JSIDYNAMIC_CPP
                "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi/jsi/JSIDynamic.cpp"
        )
endif()

add_library(
        ${PACKAGE_NAME}
        SHARED
        src/main/cpp/WsHelper.cpp
        ../cpp/JsiWs.cpp
        ../cpp/Logger.cpp

        ../cpp/ixwebsocket/IXBench.cpp
        ../cpp/ixwebsocket/IXCancellationRequest.cpp
        ../cpp/ixwebsocket/IXConnectionState.cpp
        ../cpp/ixwebsocket/IXDNSLookup.cpp
        ../cpp/ixwebsocket/IXExponentialBackoff.cpp
        ../cpp/ixwebsocket/IXGetFreePort.cpp
        ../cpp/ixwebsocket/IXGzipCodec.cpp
        ../cpp/ixwebsocket/IXHttp.cpp
        ../cpp/ixwebsocket/IXHttpClient.cpp
        ../cpp/ixwebsocket/IXHttpServer.cpp
        ../cpp/ixwebsocket/IXNetSystem.cpp
        ../cpp/ixwebsocket/IXSelectInterrupt.cpp
        ../cpp/ixwebsocket/IXSelectInterruptEvent.cpp
        ../cpp/ixwebsocket/IXSelectInterruptFactory.cpp
        ../cpp/ixwebsocket/IXSelectInterruptPipe.cpp
        ../cpp/ixwebsocket/IXSetThreadName.cpp
        ../cpp/ixwebsocket/IXSocket.cpp
        ../cpp/ixwebsocket/IXSocketAppleSSL.cpp
        ../cpp/ixwebsocket/IXSocketConnect.cpp
        ../cpp/ixwebsocket/IXSocketFactory.cpp
        ../cpp/ixwebsocket/IXSocketMbedTLS.cpp
        ../cpp/ixwebsocket/IXSocketOpenSSL.cpp
        ../cpp/ixwebsocket/IXSocketServer.cpp
        ../cpp/ixwebsocket/IXSocketTLSOptions.cpp
        ../cpp/ixwebsocket/IXStrCaseCompare.cpp
        ../cpp/ixwebsocket/IXUdpSocket.cpp
        ../cpp/ixwebsocket/IXUrlParser.cpp
        ../cpp/ixwebsocket/IXUserAgent.cpp
        ../cpp/ixwebsocket/IXUuid.cpp
        ../cpp/ixwebsocket/IXWebSocket.cpp
        ../cpp/ixwebsocket/IXWebSocketCloseConstants.cpp
        ../cpp/ixwebsocket/IXWebSocketHandshake.cpp
        ../cpp/ixwebsocket/IXWebSocketHttpHeaders.cpp
        ../cpp/ixwebsocket/IXWebSocketPerMessageDeflate.cpp
        ../cpp/ixwebsocket/IXWebSocketPerMessageDeflateCodec.cpp
        ../cpp/ixwebsocket/IXWebSocketPerMessageDeflateOptions.cpp
        ../cpp/ixwebsocket/IXWebSocketProxyServer.cpp
        ../cpp/ixwebsocket/IXWebSocketServer.cpp
        ../cpp/ixwebsocket/IXWebSocketTransport.cpp

)

# includes

file (GLOB LIBFBJNI_INCLUDE_DIR "${BUILD_DIR}/fbjni-*-headers.jar/")

target_include_directories(
        ${PACKAGE_NAME}
        PRIVATE
        # --- fbjni ---
        "${LIBFBJNI_INCLUDE_DIR}"
        # --- Third Party (required by RN) ---
        "${BUILD_DIR}/third-party-ndk/boost"
        "${BUILD_DIR}/third-party-ndk/double-conversion"
        "${BUILD_DIR}/third-party-ndk/folly"
        "${BUILD_DIR}/third-party-ndk/glog"
        # --- React Native ---
        "${NODE_MODULES_DIR}/react-native/React"
        "${NODE_MODULES_DIR}/react-native/React/Base"
        "${NODE_MODULES_DIR}/react-native/ReactAndroid/src/main/jni"
        "${NODE_MODULES_DIR}/react-native/ReactAndroid/src/main/java/com/facebook/react/turbomodule/core/jni"
        "${NODE_MODULES_DIR}/react-native/ReactCommon"
        "${NODE_MODULES_DIR}/react-native/ReactCommon/callinvoker"
        "${NODE_MODULES_DIR}/react-native/ReactCommon/jsi"
        "${NODE_MODULES_DIR}/hermes-engine/android/include/"
        ${INCLUDE_JSI_CPP} # only on older RN versions
        ${INCLUDE_JSIDYNAMIC_CPP} # only on older RN versions
        "src/main/cpp"
        "../cpp"
)

include_directories(
        "${CMAKE_CURRENT_SOURCE_DIR}/libs/openssl/include"
)

# find libraries

file (GLOB LIBRN_DIR "${BUILD_DIR}/react-native-0*/jni/${ANDROID_ABI}")

if(${FOR_HERMES})
    file (GLOB LIBHERMES_DIR "${BUILD_DIR}/third-party-ndk/hermes/jni/${ANDROID_ABI}")
    # Use Hermes
    find_library(
            JS_ENGINE_LIB
            hermes
            PATHS ${LIBHERMES_DIR}
            NO_CMAKE_FIND_ROOT_PATH
    )

else()
    file (GLOB LIBJSC_DIR "${BUILD_DIR}/android-jsc*.aar/jni/${ANDROID_ABI}")
    # Use JSC
    find_library(
            JS_ENGINE_LIB
            jscexecutor
            PATHS ${LIBRN_DIR}
            NO_CMAKE_FIND_ROOT_PATH
    )

endif()

find_library(
        FBJNI_LIB
        fbjni
        PATHS ${LIBRN_DIR}
        NO_CMAKE_FIND_ROOT_PATH
)
find_library(
        FOLLY_JSON_LIB
        folly_json
        PATHS ${LIBRN_DIR}
        NO_CMAKE_FIND_ROOT_PATH
)

find_library(
        REACT_NATIVE_JNI_LIB
        reactnativejni
        PATHS ${LIBRN_DIR}
        NO_CMAKE_FIND_ROOT_PATH
)
if(${REACT_NATIVE_VERSION} LESS 66)
        # JSI lib didn't exist on RN 0.65 and before. Simply omit it.
        set (JSI_LIB "")
else()
        # RN 0.66 distributes libjsi.so, can be used instead of compiling jsi.cpp manually.
        find_library(
                JSI_LIB
                jsi
                PATHS ${LIBRN_DIR}
                NO_CMAKE_FIND_ROOT_PATH
        )
endif()


add_library(local_crypto STATIC IMPORTED)
add_library(local_openssl STATIC IMPORTED)
set_target_properties(local_openssl PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/openssl/${ANDROID_ABI}/libssl.a)
set_target_properties(local_crypto PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libs/openssl/${ANDROID_ABI}/libcrypto.a)

find_library(
        LOG_LIB
        log
)
find_library(z-lib z)

message(WARNING "JSI-WS linking: CMAKE_CURRENT_SOURCE_DIR=${ANDROID_ABI}")
target_link_libraries(
        ${PACKAGE_NAME}
        ${LOG_LIB}
        ${JSI_LIB}
        ${JS_ENGINE_LIB} # <-- Hermes or JSC
        ${REACT_NATIVE_JNI_LIB}
        ${FBJNI_LIB}
        ${FOLLY_JSON_LIB}
        local_openssl
        local_crypto
        ${z-lib}
        android
)
