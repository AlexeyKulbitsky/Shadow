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
		VkSampler GetVulkanId() const { return m_sampler; }

	private:
		VulkanSampler(const SamplerDescription& description);

		VkSampler m_sampler;
	};

} // video

} // sh

#endif