#ifndef CONNECTION_INCLUDE
#define CONNECTION_INCLUDE

#include <Shadow.h>

extern "C" __declspec(dllexport) sh::Application* GetGameModule();
extern "C" __declspec(dllexport) void SetDevice(sh::Device* device);

#endif
