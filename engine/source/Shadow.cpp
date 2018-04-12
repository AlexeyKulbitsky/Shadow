#include "Shadow.h"
#include "Device.h"
#include "CreationParameters.h"

#if defined SHADOW_WINDOWS
#include "platform/win32/Win32Device.h"
#elif defined SHADOW_ANDROID
#include "platform/android/AndroidDevice.h"
#elif defined SHADOW_MAC
#include "platform/mac/MacDevice.h"
#endif

namespace sh
{
	SHADOW_API Device* /*SH_CALLCONV*/ CreateDevice
	(
		video::DriverType driverType,
		u32 width, u32 height
	)
	{
		Device *device = nullptr;

		CreationParameters parameters;
		parameters.width = width;
		parameters.height = height;
		parameters.driverType = driverType;
		parameters.WinId = nullptr;
		
		return CreateDevice(parameters);
	}

	SHADOW_API Device* CreateDevice()
	{
		Device *device = nullptr;

#if defined SHADOW_WINDOWS
		device = new Win32Device();
#elif defined SHADOW_ANDROID
		device = new AndroidDevice();
#elif defined SHADOW_MAC
        device = new MacDevice();
#endif
		Device::SetInstance(device);

		return device;
	}

	SHADOW_API Device* /*SH_CALLCONV*/ CreateDevice(const CreationParameters& parameters)
	{
		Device *device = nullptr;

#if defined SHADOW_WINDOWS
		device = new Win32Device(parameters);
#elif defined SHADOW_ANDROID
		device = new AndroidDevice(parameters);
#elif defined SHADOW_MAC
        device = new MacDevice(parameters);
#endif
		Device::SetInstance(device);

		return device;
	}

}
