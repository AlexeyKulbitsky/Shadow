#ifndef SHADOW_VULKAN_GPU_PARAMS_INCUDE
#define SHADOW_VULKAN_GPU_PARAMS_INCUDE

#include "../GpuParams.h"

#include "VulkanCommon.h"

namespace sh
{

namespace video
{

	class VulkanGpuParams : public GpuParams
	{
		friend class VulkanHardwareBufferManager;

	public:
		VkDescriptorSet GetDescriptorSet() const { return m_desctriptorSet; }

	private:
		VulkanGpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo);

	private:
		VkDescriptorSet m_desctriptorSet;
		VkDescriptorPool m_descriptorPool;
	};

} // video

} // sh

#endif