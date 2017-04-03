#include "TextureManager.h"

namespace sh
{

namespace video
{

	TextureManager* TextureManager::GetInstance()
	{
		static TextureManager s_instance;
		return &s_instance;
	}

	TexturePtr TextureManager::CreateTexture(const TextureDescription& description)
	{
		TexturePtr result;
		//result.reset(new Texture(description));
		return result;
	}

} // video

} // sh