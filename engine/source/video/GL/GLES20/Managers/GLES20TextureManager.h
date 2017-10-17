#ifndef SHADOW_GLES20_TEXTURE_MANAGER_INCLUDE
#define SHADOW_GLES20_TEXTURE_MANAGER_INCLUDE

#include "../../../Managers/TextureManager.h"

namespace sh
{

namespace video
{
	
	class GLES20TextureManager : public TextureManager
	{
	public:
		virtual TexturePtr CreateTexture(const TextureDescription& description) override;
	};

} // video

} // sh

#endif