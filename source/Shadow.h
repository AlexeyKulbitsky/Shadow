#ifndef SHADOW_INCLUDE
#define SHADOW_INCLUDE

#include "CompileConfig.h"
#include "Device.h"
//#include "CreationParameters.h"
#include "video/Driver.h"

namespace sh
{
	// Create usual device
	SHADOW_API Device* SH_CALLCONV CreateDevice
	(
		video::DriverType driverType = video::DriverType::VULKAN,
		u32 width = 640, u32 height = 480
	);

	// Create device with more extended configuration
	SHADOW_API Device* SH_CALLCONV CreateDevice(const CreationParameters& parameters);
}

#endif
