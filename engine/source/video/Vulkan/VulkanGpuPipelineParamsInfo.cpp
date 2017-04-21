#include "VulkanGpuPipelineParamsInfo.h"
#include "VulkanDriver.h"
#include "../../Device.h"

namespace sh
{

namespace video
{

	VulkanGpuPipelineParamsInfo::~VulkanGpuPipelineParamsInfo()
	{

	}

	VulkanGpuPipelineParamsInfo::VulkanGpuPipelineParamsInfo(const GpuPipelineParamsDescription& description)
		: GpuPipelineParamsInfo(description)
	{
		u32 paramsSize = 0U;
		u32 samplersCount = 0U;
		// For each params description in each stage
		for( size_t i = 0; i < 6; ++i )
		{
			if(!m_paramsDescription[i] )
				continue;

			// Collect size of data params
			for( auto& param : m_paramsDescription[i]->params )
			{
				paramsSize += param.second.size;
			}

			// Collect samplers count
			samplersCount += m_paramsDescription[i]->samplers.size();
		}


		VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
		VkDevice device = driver->GetVulkanDevice();
		
		// Push constants
		if( paramsSize > 0U )
		{
			VkPushConstantRange pushConstantRange = {};
			pushConstantRange.offset = 0;
			pushConstantRange.size = paramsSize;
			pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		
			m_pushConstantRanges.push_back(pushConstantRange);
		}
		
		// Descriptor set layouts
		if( samplersCount > 0U )
		{
			VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
			samplerLayoutBinding.binding = 0;
			samplerLayoutBinding.descriptorCount = samplersCount;
			samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			samplerLayoutBinding.pImmutableSamplers = nullptr;
			samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

			std::array<VkDescriptorSetLayoutBinding, 1> bindings = { samplerLayoutBinding };
			VkDescriptorSetLayoutCreateInfo layoutInfo = {};
			layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			layoutInfo.bindingCount = bindings.size();
			layoutInfo.pBindings = bindings.data();

			VkDescriptorSetLayout descriptorSetLayout;
			VkResult res = vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout);
			SH_ASSERT(res == VK_SUCCESS, "failed to create descriptor set layout!");

			m_descriptorSetLayouts.push_back(descriptorSetLayout);
		}	
	}


} // video

} // sh