#include "Shadow.h"
#include "Device.h"
#include "CreationParameters.h"
#include "platform/Win32Device.h"

namespace sh
{
	SHADOW_API Device* SH_CALLCONV CreateDevice
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
		
#if defined SHADOW_WINDOWS
		device = new Win32Device(parameters);
#endif
		return device;
	}
}