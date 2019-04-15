#ifndef SHADOW_CREATION_PARAMETERS_INCLUDE
#define SHADOW_CREATION_PARAMETERS_INCLUDE

#include "common/Prerequisities.h"

namespace sh
{
    enum class DriverType
    {
        OpenGL_ES_2_0
    };

	struct SHADOW_API CreationParameters
	{
		unsigned width = 800U;
        unsigned height = 600U;
		DriverType driverType = DriverType::OpenGL_ES_2_0;
		void* WinId = nullptr;
	};
}

#endif
