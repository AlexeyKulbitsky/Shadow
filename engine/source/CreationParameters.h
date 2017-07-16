#ifndef SHADOW_CREATION_PARAMETERS_INCLUDE
#define SHADOW_CREATION_PARAMETERS_INCLUDE

#include "Types.h"
#include "video/Driver.h"

namespace sh
{
	struct CreationParameters
	{
		u32 width = 800U;
		u32 height = 600U;
		video::DriverType driverType = video::DriverType::OPENGL_ES_2_0;
		void* WinId = nullptr;
	};
}

#endif
