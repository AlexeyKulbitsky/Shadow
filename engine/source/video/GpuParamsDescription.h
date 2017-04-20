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
		String name;
		GpuParamDataType type;

		u32 size;
		u32 offset;
		u32 location;
	};

	struct GpuParamSamplerDescription
	{
		String name;
		SamplerDescription samplerDesc;

		u32 location;
	};

	struct GpuParamObjectDescription
	{
		String name;
		GpuParamObjectType type;

		u32 set;
		u32 binding;
	};

	////////////////////////////////////////////////////

	struct GpuParamsDescription
	{
		Map<String, GpuParamDataDescription> params;
		Map<String, GpuParamSamplerDescription> samplers;
		Map<String, GpuParamObjectDescription> _samplers;
	};

} // video

} // sh

#endif
