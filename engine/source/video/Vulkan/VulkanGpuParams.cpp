#include "VulkanGpuParams.h"
#include "VulkanGpuPipelineParamsInfo.h"
#include "../../Device.h"
#include "VulkanDriver.h"

namespace sh
{

namespace video
{

	VulkanGpuParams::VulkanGpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo)
		: GpuParams(pipelineParamsInfo)
	{
		VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
		VkDevice device = driver->GetVulkanDevice();

		VulkanGpuPipelineParamsInfo* vulkanPipelineParamsInfo = static_cast<VulkanGpuPipelineParamsInfo*>(pipelineParamsInfo.get());

		const auto& descriptorSetLayouts = vulkanPipelineParamsInfo->GetDescriptorSetLayouts();
		if( descriptorSetLayouts.size() == 0U )
			return;
		
		// Create descriptor pool for future descriptor sets allocation
		VkDescriptorPoolSize poolSize = {};
		poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize.descriptorCount = 1U;

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = 1;

		VkResult res = vkCreateDescriptorPool(device, &poolInfo, nullptr, &m_descriptorPool);
		SH_ASSERT(res == VK_SUCCESS, "failed to create descriptor pool!");

		// Create descriptor set
		VkDescriptorSetLayout setLayout = descriptorSetLayouts[0];
		VkDescriptorSetLayout layouts[] = { setLayout };

		VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = m_descriptorPool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = layouts;

		res = vkAllocateDescriptorSets(device, &allocInfo, &m_desctriptorSet);
		SH_ASSERT(res == VK_SUCCESS, "failed to allocate descriptor set!");

		// Fill descriptor set with data
		//VkDescriptorImageInfo imageInfo = {};
        //imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        //imageInfo.imageView = textureImageView;
        //imageInfo.sampler = textureSampler;
	}

} // video

} // sh