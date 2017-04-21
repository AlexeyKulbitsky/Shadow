#ifndef SHADOW_VULKAN_SAMPLER_INCLUDE
#define SHADOW_VULKAN_SAMPLER_INCLUDE

#include "../Sampler.h"
#include "VulkanCommon.h"

namespace sh
{

namespace video
{

	class VulkanSampler : public Sampler
	{
		friend class VulkanRenderStateManager;
	public:
		virtual void Set(TexturePtr texture) override;

		VkSampler GetVulkanId() const { return m_sampler; }

		const VkDescriptorImageInfo& GetDescriptor() const { return m_imageInfo; }

	private:
		VulkanSampler(const SamplerDescription& description);

		VkSampler m_sampler;
		VkDescriptorImageInfo m_imageInfo; // Info for descriptor
	};

} // video

} // sh

#endif