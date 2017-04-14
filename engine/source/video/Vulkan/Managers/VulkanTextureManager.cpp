#include "VulkanTextureManager.h"
#include "../VulkanTexture.h"

namespace sh
{

namespace video
{
	
	TexturePtr VulkanTextureManager::CreateTexture(const TextureDescription& description)
	{
		TexturePtr result;
		result.reset(new VulkanTexture(description));
		return result;
	}

} // video

} // sh