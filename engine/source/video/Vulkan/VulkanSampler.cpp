#include "VulkanSampler.h"
#include "VulkanTexture.h"
#include "../../Device.h"
#include "VulkanDriver.h"

namespace sh
{

namespace video
{

	void VulkanSampler::Set( TexturePtr texture )
	{
		Sampler::Set(texture);
		VulkanTexture* vkTexture = static_cast<VulkanTexture*>(texture.get());

		m_imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		m_imageInfo.imageView = vkTexture->GetImageView();
		m_imageInfo.sampler = m_sampler;
	}

	VulkanSampler::VulkanSampler(const SamplerDescription& description)
	{
		VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
		VkDevice device = driver->GetVulkanDevice();

		VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR; //s_vkTextureFiltering[description.magFilter];
        samplerInfo.minFilter = VK_FILTER_LINEAR; //s_vkTextureFiltering[description.minFilter];
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT; //s_vkTiling[description.tilingU];
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT; //s_vkTiling[description.tilingV];
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT; //s_vkTiling[description.tilingW];
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 1.0f;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        VkResult res = vkCreateSampler(device, &samplerInfo, nullptr, &m_sampler);
		SH_ASSERT(res == VK_SUCCESS, "Failed to create texture sampler!");
	}

} // video

} // sh