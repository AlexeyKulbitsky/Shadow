#include "VulkanTexture.h"
#include "VulkanDriver.h"
#include "../../Device.h"

namespace sh
{

namespace video
{

	void VulkanTexture::SetData(u32 mipLevel, void* data)
	{
		VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
		VkDevice device = driver->GetVulkanDevice();

		size_t channelsCount = m_description.format == TextureFormat::RGBA ? 4 : 3;
		VkDeviceSize imageSize = m_description.width * m_description.height * channelsCount;

		VkImage stagingImage;
		VkDeviceMemory stagingImageMemory;

		createImage(m_description.width, m_description.height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
					 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingImage, stagingImageMemory);

		VkImageSubresource subresource = {};
		subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subresource.mipLevel = 0;
		subresource.arrayLayer = 0;

		VkSubresourceLayout stagingImageLayout;
		vkGetImageSubresourceLayout(device, stagingImage, &subresource, &stagingImageLayout);

		void* dataInternal;
		vkMapMemory(device, stagingImageMemory, 0, imageSize, 0, &dataInternal);

		u8* externalData = static_cast<u8*>(data);

		if (stagingImageLayout.rowPitch == m_description.width * channelsCount) 
		{
			memcpy(dataInternal, externalData, (size_t) imageSize);
		} 
		else 
		{
			uint8_t* dataBytes = reinterpret_cast<uint8_t*>(dataInternal);

			for (int y = 0; y < m_description.height; y++) 
			{
				memcpy(&dataBytes[y * stagingImageLayout.rowPitch], &externalData[y * m_description.width * channelsCount], m_description.width * channelsCount);
			}
		}

		vkUnmapMemory(device, stagingImageMemory);

		// Create final texture
		createImage(
			m_description.width, m_description.height,
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_IMAGE_TILING_OPTIMAL,
			VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_textureImage,
			m_textureImageMemory
		);


		driver->transitionImageLayout(stagingImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
        driver->transitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
        driver->copyImage(stagingImage, m_textureImage, m_description.width, m_description.height);

        driver->transitionImageLayout(m_textureImage, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		



		VkImageViewCreateInfo viewInfo = {};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = m_textureImage;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		VkResult res = vkCreateImageView(device, &viewInfo, nullptr, &m_imageView); 
		SH_ASSERT(res == VK_SUCCESS, "failed to create image view!");




		// Sampler
		VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = 16;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        res = vkCreateSampler(device, &samplerInfo, nullptr, &m_sampler);
		SH_ASSERT(res == VK_SUCCESS, "failed to create texture sampler!");

	}

	void VulkanTexture::SetFaceData(TextureFace face, u32 mipLevel, const void* data)
	{

	}

	void VulkanTexture::createImage(
			uint32_t width, uint32_t height,
			VkFormat format,
			VkImageTiling tiling,
			VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties,
			VkImage& image,
			VkDeviceMemory& imageMemory)
		{

			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			VkImageCreateInfo imageInfo = {};
			imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			imageInfo.imageType = VK_IMAGE_TYPE_2D;
			imageInfo.extent.width = width;
			imageInfo.extent.height = height;
			imageInfo.extent.depth = 1;
			imageInfo.mipLevels = 1;
			imageInfo.arrayLayers = 1;
			imageInfo.format = format;
			imageInfo.tiling = tiling;
			imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
			imageInfo.usage = usage;
			imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			VkResult res = vkCreateImage(device, &imageInfo, nullptr, &image);
			SH_ASSERT(res == VK_SUCCESS, "failed to create image!");

			VkMemoryRequirements memRequirements;
			vkGetImageMemoryRequirements(device, image, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = driver->findMemoryType(memRequirements.memoryTypeBits, properties);

			res = vkAllocateMemory(device, &allocInfo, nullptr, &imageMemory);
			SH_ASSERT(res == VK_SUCCESS, "failed to allocate image memory!");

			res = vkBindImageMemory(device, image, imageMemory, 0);
			SH_ASSERT(res == VK_SUCCESS, "failed to bind image memory!");
		}

	VulkanTexture::VulkanTexture(const TextureDescription& description)
		: Texture(description)
	{
		
	}

} // video

} // sh