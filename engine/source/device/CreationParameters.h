#ifndef SHADOW_CREATION_PARAMETERS_INCLUDE
#define SHADOW_CREATION_PARAMETERS_INCLUDE

#include "common/Prerequisities.h"

namespace sh
{
	struct SHADOW_API CreationParameters
	{
		unsigned width = 800U;
        unsigned height = 600U;
		//video::DriverType driverType = video::DriverType::OPENGL_ES_2_0;
		void* WinId = nullptr;
	};
}

#endif
