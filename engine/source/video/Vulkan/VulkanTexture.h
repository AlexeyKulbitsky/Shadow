#ifndef SHADOW_VULKAN_TEXTURE_INCLUDE
#define SHADOW_VULKAN_TEXTURE_INCLUDE

#include "../Texture.h"
#include "VulkanCommon.h"

namespace sh
{

namespace video
{
	
	class VulkanTexture : public Texture
	{
		friend class VulkanTextureManager;
	public:
		virtual void SetData(u32 mipLevel, void* data) override;
		virtual void SetFaceData(TextureFace face, u32 mipLevel, const void* data) override;

	private:
		void createImage(
				uint32_t width, uint32_t height,
				VkFormat format,
				VkImageTiling tiling,
				VkImageUsageFlags usage,
				VkMemoryPropertyFlags properties,
				VkImage& image,		
				VkDeviceMemory& imageMemory);

	protected:
		VulkanTexture(const TextureDescription& description);
	};

} // video

} // sh

#endif