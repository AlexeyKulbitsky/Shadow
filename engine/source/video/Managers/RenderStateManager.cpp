#include "RenderStateManager.h"
#include "../GpuPipelineParamsInfo.h"

namespace sh
{

namespace video
{

	GpuPipelineParamsInfoPtr RenderStateManager::CreateGpuPipelineParamsInfo( const GpuPipelineParamsDescription& description )
	{
		GpuPipelineParamsInfoPtr result;
		result.reset(new GpuPipelineParamsInfo(description));
		return result;
	}

} // video

} // sh