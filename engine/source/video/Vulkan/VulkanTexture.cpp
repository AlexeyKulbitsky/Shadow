#include "VulkanTexture.h"
#include "VulkanDriver.h"
#include "../../Device.h"

namespace sh
{

namespace video
{

	void VulkanTexture::SetData(u32 mipLevel, const void* data)
	{
		VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
		VkDevice device = driver->GetVulkanDevice();

		// Create temp image for transfering it to gpu final image
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent.width = m_description.width;
		imageInfo.extent.height = m_description.height;
		imageInfo.extent.depth = 1;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		imageInfo.tiling = VK_IMAGE_TILING_LINEAR;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.flags = 0; // Optional

		VkImage stagingImage;
		VkResult res = vkCreateImage(device, &imageInfo, nullptr, &stagingImage);
		SH_ASSERT(res == VK_SUCCESS, "Can not create Vulkan image!");

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device, stagingImage, &memRequirements);
	}

	void VulkanTexture::SetFaceData(TextureFace face, u32 mipLevel, const void* data)
	{

	}

	VulkanTexture::VulkanTexture(const TextureDescription& description)
		: Texture(description)
	{
		VkDeviceSize imageSize = description.width * description.height * 4;
	}

} // video

} // sh