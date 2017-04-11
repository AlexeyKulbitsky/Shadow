#ifndef SHADOW_GPU_PARAMS_BLOCK_INCLUDE
#define SHADOW_GPU_PARAMS_BLOCK_INCLUDE

#include "GpuParamsDescription.h"

namespace sh
{

namespace video
{
	class GpuParamsBlock
	{
	public:
		Map<String, GpuParamDataDescription> params;
	};

} // video

} // sh

#endif