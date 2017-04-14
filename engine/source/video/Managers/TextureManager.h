#ifndef SHADOW_TEXTURE_MANAGER_INCLUDE
#define SHADOW_TEXTURE_MANAGER_INCLUDE

#include "../../Globals.h"
#include "../Texture.h"

namespace sh
{

namespace video
{

	class TextureManager : public Singleton < TextureManager >
	{
	public:
		virtual TexturePtr CreateTexture(const TextureDescription& description) = 0;
	};

} // video

} // sh

#endif