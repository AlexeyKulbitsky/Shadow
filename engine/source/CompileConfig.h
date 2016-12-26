#ifndef SHADOW_COMPILE_CONFIG_INCLUDE
#define SHADOW_COMPILE_CONFIG_INCLUDE



//#define SHADOW_EXPORT

///////////////////////////////////////////////////////////////////////


#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#define SHADOW_WINDOWS
#define SH_WINDOWS_API
#define SH_COMPILE_WITH_WINDOWS_DEVICE
#endif

#if defined(ANDROID)
#define SHADOW_ANDROID
#define SH_ANDROID_API
#define SH_COMPILE_WITH_ANDROID_DEVICE
#endif

#if defined(__APPLE__)
#define SHADOW_APPLE
#define SH_APPLE_API
#define SH_COMPILE_WITH_APPLE_DEVICE
#endif

#ifndef SHADOW_STATIC_LIB
#ifdef SHADOW_EXPORT
#define SHADOW_API //__declspec(dllexport)
#else
#define SHADOW_API //__declspec(dllimport)
#endif 
#else
#define IRRLICHT_API
#endif 


// Declare the calling convention.
#if defined(STDCALL_SUPPORTED)
#define SH_CALLCONV //__stdcall
#else
#define SH_CALLCONV //__cdecl
#endif // STDCALL_SUPPORTED

#endif
