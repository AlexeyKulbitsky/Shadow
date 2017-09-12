#ifndef SHADOW_GPU_PARAM_DESCRIPTION_INCLUDE
#define SHADOW_GPU_PARAM_DESCRIPTION_INCLUDE

#include "../Globals.h"
#include "Sampler.h"

namespace sh
{
namespace video
{
	struct GpuParamDataDescription
	{
		String name = "";
		GpuParamDataType type;

		u32 size = 0U;
		u32 offset = 0U;
		u32 location = 0U;
	};

	struct GpuParamObjectDescription
	{
		String name = "";
		GpuParamObjectType type;

		u32 set = 0U;
		u32 binding = 0U;
	};

	////////////////////////////////////////////////////

	struct GpuParamsDescription
	{
		Map<String, GpuParamDataDescription> params;
		Map<String, GpuParamObjectDescription> samplers;
	};

} // video

} // sh

#endif
