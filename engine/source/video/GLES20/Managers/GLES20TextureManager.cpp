#include "GLES20TextureManager.h"
#include "../GLES20Texture.h"

namespace sh
{

namespace video
{
	
	TexturePtr GLES20TextureManager::CreateTexture(const TextureDescription& description)
	{
		TexturePtr result;
		result.reset(new GLES20Texture(description));
		return result;
	}

} // video

} // sh