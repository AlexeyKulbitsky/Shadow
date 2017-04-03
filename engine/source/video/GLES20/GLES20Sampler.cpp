#include "GLES20Sampler.h"
#include "GLES20Texture.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		GLES20Sampler::GLES20Sampler(const SamplerDescription& description)
		{
			m_description = description;
		}
		
	}
}