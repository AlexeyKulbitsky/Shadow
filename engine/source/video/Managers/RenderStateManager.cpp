#include "video/Managers/RenderStateManager.h"

namespace sh
{

namespace video
{

    RenderStateManager::~RenderStateManager()
    {
    }

	/*SamplerPtr RenderStateManager::CreateSampler(const SamplerDescription& description)
	{
		SamplerPtr result;
		result.reset(new Sampler(description));
		return result;
	}

	GpuPipelineParamsInfoPtr RenderStateManager::CreateGpuPipelineParamsInfo( const GpuPipelineParamsDescription& description )
	{
		GpuPipelineParamsInfoPtr result;
		result.reset(new GpuPipelineParamsInfo(description));
		return result;
	}*/

} // video

} // sh