#include "Sampler.h"
#include "Managers/RenderStateManager.h"
#include "TextureLoader/TextureLoader.h"

namespace sh
{

namespace video
{
	Sampler::Sampler(const SamplerDescription& description)
		: m_description(description)
	{
		m_texture = TextureLoader::GetInstance()->GetCheckerTexture();
	}

	SamplerPtr Sampler::Create(const SamplerDescription& description)
	{
		return RenderStateManager::GetInstance()->CreateSampler(description);
	}

} // video

} // sh