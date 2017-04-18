#ifndef SHADOW_GPU_PARAM_DESCRIPTION_INCLUDE
#define SHADOW_GPU_PARAM_DESCRIPTION_INCLUDE

#include "../Globals.h"

namespace sh
{
namespace video
{
	struct GpuParamDataDescription
	{
		String name;
		GpuParamDataType type;

		u32 size;
		u32 offset;
		u32 location;
	};

	struct GpuParamObjectDescription
	{
		String name;
		GpuParamObjectType type;

		u32 location;
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
