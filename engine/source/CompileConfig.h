#ifndef SHADOW_COMPILE_CONFIG_INCLUDE
#define SHADOW_COMPILE_CONFIG_INCLUDE


//#define SHADOW_STATIC_LIB
//#define SHADOW_EXPORTS

///////////////////////////////////////////////////////////////////////

#pragma warning (disable: 4251)

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
	#define SHADOW_WINDOWS
	#define SH_WINDOWS_API
	#define SH_COMPILE_WITH_WINDOWS_DEVICE
	#define NOMINMAX
#endif

#if defined(ANDROID) || defined(__ANDROID__)
	#define SHADOW_ANDROID
	#define SH_ANDROID_API
	#define SH_COMPILE_WITH_ANDROID_DEVICE
#endif

#if defined(__APPLE__)
    #define SHADOW_APPLE
    #define SH_APPLE_API
    #define SH_COMPILE_WITH_APPLE_DEVICE

    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define SHADOW_IOS
        #define SH_IOS_API
        #define SH_COMPILE_WITH_IOS_DEVICE
    #elif TARGET_OS_MAC
        #define SHADOW_MAC
        #define SH_MAC_API
        #define SH_COMPILE_WITH_MAC_DEVICE
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


	// Declare the calling convention.
	#if defined(STDCALL_SUPPORTED)
		#define SH_CALLCONV __stdcall
	#else
		#define SH_CALLCONV __cdecl
	#endif // STDCALL_SUPPORTED

#else

	#if (__GNUC__ >= 4) && !defined(SHADOW_STATIC_LIB) && defined(SHADOW_EXPORTS)
		#define SHADOW_API __attribute__ ((visibility("default")))
	#else
		#define SHADOW_API
	#endif
	#define SH_CALLCONV

#endif

#endif
