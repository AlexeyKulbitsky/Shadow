#ifndef SHADOW_CREATION_PARAMETERS_INCLUDE
#define SHADOW_CREATION_PARAMETERS_INCLUDE

#include "prerequisities/Prerequisities.h"

namespace sh
{
    enum class DriverType
    {
        OpenGL_ES_2_0
    };

	struct SHADOW_API CreationParameters
	{
		uint32_t width = 800U;
		uint32_t height = 600U;
		DriverType driverType = DriverType::OpenGL_ES_2_0;
		void* WinId = nullptr;
	};
}

#endif
