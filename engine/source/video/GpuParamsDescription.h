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

	struct GpuObjectDescription
	{
		String name;
		//Texture::Type type;

		u32 layout;
		u32 binding;
	};

	struct GpuParamsBlockDescription
	{
		String name;

		u32 layout;
		u32 binding;
		u32 size;
	};


	////////////////////////////////////////////////////

	struct GpuParamsDescription
	{
		Map<String, GpuParamsBlockDescription> paramsBlocks;
		Map<String, GpuParamDataDescription> params;

		Map<String, GpuObjectDescription> samplers;
		Map<String, GpuObjectDescription> buffers;
	};

} // video

} // sh

#endif
