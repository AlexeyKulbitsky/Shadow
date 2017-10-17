#include "GLES20Sampler.h"
#include "GLES20Texture.h"
#include "../GLContext/EGLContextManager.h"

#include "../../TextureLoader/TextureLoader.h"

namespace sh
{
	namespace video
	{
		GLES20Sampler::GLES20Sampler(const SamplerDescription& description)
		{
			m_description = description;
			m_texture = TextureLoader::GetInstance()->GetCheckerTexture();
		}
		
	}
}