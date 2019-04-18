#ifndef SHADOW_GPU_PARAM_DESCRIPTION_INCLUDE
#define SHADOW_GPU_PARAM_DESCRIPTION_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/Common.h"

namespace sh
{
namespace video
{
	struct SHADOW_API GpuParamDataDescription
	{
		std::string name = "";
		GpuParamDataType type;

		uint32_t size = 0U;
        uint32_t offset = 0U;
        uint32_t location = 0U;
	};

	struct SHADOW_API GpuParamObjectDescription
	{
		std::string name = "";
		GpuParamObjectType type;

        uint32_t set = 0U;
        uint32_t binding = 0U;
	};

	struct SHADOW_API GpuParamsDescription
	{
		std::map<std::string, GpuParamDataDescription> params;
        std::map<std::string, GpuParamObjectDescription> samplers;
	};

} // video

} // sh

#endif
