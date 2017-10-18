#ifndef SHADOW_GL_TEXTURE_MANAGER_INCLUDE
#define SHADOW_GL_TEXTURE_MANAGER_INCLUDE

#include "../../Managers/TextureManager.h"

namespace sh
{

namespace video
{
	
	class GLTextureManager : public TextureManager
	{
	public:
		virtual TexturePtr CreateTexture(const TextureDescription& description) override;
	};

} // video

} // sh

#endif