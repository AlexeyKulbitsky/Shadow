#ifndef SHADOW_TEXTURE_MANAGER_INCLUDE
#define SHADOW_TEXTURE_MANAGER_INCLUDE

#include "../Globals.h"
#include "Texture.h"

namespace sh
{

namespace video
{

	class SHADOW_API TextureManager
	{
	public:
		static TextureManager* GetInstance();

		TexturePtr CreateTexture(const TextureDescription& description);
	};

} // video

} // sh

#endif
