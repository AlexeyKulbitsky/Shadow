#ifndef SHADOW_INCLUDE
#define SHADOW_INCLUDE

#include "CompileConfig.h"
#include "Device.h"
//#include "CreationParameters.h"
#include "video/Driver.h"

namespace sh
{
	// Create usual device
	/*SHADOW_API*/ Device* /*SH_CALLCONV*/ CreateDevice
	(
		video::DriverType driverType = video::DriverType::OPENGL_ES_2_0,
		u32 width = 800, u32 height = 600
	);

	// Create device with more extended configuration
	/*SHADOW_API*/ Device* /*SH_CALLCONV*/ CreateDevice(const CreationParameters& parameters);
}

#endif
