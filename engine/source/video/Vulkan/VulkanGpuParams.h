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
		VkPipelineLayout GetPipelineLayput() const { return m_pipelineLayout; }

	private:
		VulkanGpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo);

	private:
		VkDescriptorSet m_desctriptorSet;
		VkDescriptorPool m_descriptorPool;

		VkPipelineLayout m_pipelineLayout;
	};

} // video

} // sh

#endif