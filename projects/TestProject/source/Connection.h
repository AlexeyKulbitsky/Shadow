#ifndef CONNECTION_INCLUDE
#define CONNECTION_INCLUDE

#include <Shadow.h>

extern "C" __declspec(dllexport) sh::Application* CreateGameModule();
extern "C" __declspec(dllexport) void DestroyGameModule();
extern "C" __declspec(dllexport) void InitGameModule();

#endif
