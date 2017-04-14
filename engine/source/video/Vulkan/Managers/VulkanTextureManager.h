#ifndef SHADOW_VULKAN_TEXTURE_MANAGER_INCLUDE
#define SHADOW_VULKAN_TEXTURE_MANAGER_INCLUDE

#include "../../Managers/TextureManager.h"

namespace sh
{

namespace video
{
	
	class VulkanTextureManager : public TextureManager
	{
	public:
		virtual TexturePtr CreateTexture(const TextureDescription& description) override;
	};

} // video

} // sh

#endif