#include "GLTextureManager.h"
#include "../GLTexture.h"

namespace sh
{

namespace video
{
	
	TexturePtr GLTextureManager::CreateTexture(const TextureDescription& description)
	{
		TexturePtr result;
		result.reset(new GLTexture(description));
		return result;
	}

} // video

} // sh