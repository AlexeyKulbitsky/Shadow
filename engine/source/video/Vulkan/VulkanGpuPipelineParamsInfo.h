#ifndef SHADOW_VULKAN_GPU_PIPELINE_PARAMS_INFO_INCLUDE
#define SHADOW_VULKAN_GPU_PIPELINE_PARAMS_INFO_INCLUDE

#include "../GpuPipelineParamsInfo.h"
#include "VulkanCommon.h"

namespace sh
{

namespace video
{

	class VulkanGpuPipelineParamsInfo : public GpuPipelineParamsInfo
	{
	private:
		VkPipelineLayout m_pipelineLayout;
	};

} // video

} // sh

#endif