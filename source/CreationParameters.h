#ifndef SHADOW_CREATION_PARAMETERS_INCLUDE
#define SHADOW_CREATION_PARAMETERS_INCLUDE

#include "Types.h"
#include "video/Driver.h"

namespace sh
{
	struct CreationParameters
	{
		u32 width;
		u32 height;
		video::DriverType driverType;
		void* WinId;
	};
}

#endif
