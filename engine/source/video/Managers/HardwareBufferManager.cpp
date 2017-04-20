#include "HardwareBufferManager.h"
#include "../GpuParams.h"

namespace sh
{

namespace video
{

	GpuParamsPtr HardwareBufferManager::CreateGpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo)
	{
		GpuParamsPtr result;
		result.reset(new GpuParams(pipelineParamsInfo));
		return result;
	}

} // video

} // sh
