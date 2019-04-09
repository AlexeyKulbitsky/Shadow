#ifndef PLATFORM_DEFINE_INCLUDE
#define PLATFORM_DEFINE_INCLUDE

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
    #pragma warning (disable: 4251)
    #ifndef SHADOW_WINDOWS
        #define SHADOW_WINDOWS
    #endif
    #define SH_WINDOWS_API
    #define NOMINMAX
#endif

#if defined(ANDROID) || defined(__ANDROID__)
    #define SHADOW_ANDROID
    #define SH_ANDROID_API
#endif

#if defined(__APPLE__)
    #define SHADOW_APPLE
    #define SH_APPLE_API
    #define SH_COMPILE_WITH_APPLE_DEVICE

    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define SHADOW_IOS
        #define SH_IOS_API
    #elif TARGET_OS_MAC
        #define SHADOW_MAC
        #define SH_MAC_API
    #endif
#endif

#ifdef SH_WINDOWS_API
    #ifndef SHADOW_STATIC_LIB
        #ifdef SHADOW_EXPORTS
            #define SHADOW_API __declspec(dllexport)
            #define SHADOW_EXTERN
        #else
            #define SHADOW_API __declspec(dllimport)
            #define SHADOW_EXTERN extern
        #endif 
    #else
        #define SHADOW_API
        #define SHADOW_EXTERN
    #endif 
#else
    #define SHADOW_API
#endif

#endif