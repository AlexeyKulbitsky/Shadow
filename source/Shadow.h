#ifndef SHADOW_INCLUDE
#define SHADOW_INCLUDE

#include "CompileConfig.h"
#include "Device.h"
#include "video/Driver.h"

namespace sh
{
	 SHADOW_API Device* SH_CALLCONV CreateDevice
	 (
		video::DriverType driverType = video::DriverType::OPENGL_ES_2_0,
		u32 width = 640, u32 height = 480
	 );
}

#endif
