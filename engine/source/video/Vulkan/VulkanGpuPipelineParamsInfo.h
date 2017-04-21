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
		friend class VulkanRenderStateManager;
	public:
		virtual ~VulkanGpuPipelineParamsInfo();

		const std::vector<VkPushConstantRange>& GetPushConstantRanges() const { return m_pushConstantRanges; }
		const std::vector<VkDescriptorSetLayout>& GetDescriptorSetLayouts() const { return m_descriptorSetLayouts; }

	private:
		VulkanGpuPipelineParamsInfo(const GpuPipelineParamsDescription& description);

	private:
		std::vector<VkPushConstantRange> m_pushConstantRanges;
		std::vector<VkDescriptorSetLayout> m_descriptorSetLayouts;
	};

} // video

} // sh

#endif