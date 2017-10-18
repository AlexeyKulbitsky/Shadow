#include "Sampler.h"
#include "Managers/RenderStateManager.h"

namespace sh
{

namespace video
{
	Sampler::Sampler(const SamplerDescription& description)
		: m_description(description)
	{
	}

	SamplerPtr Sampler::Create(const SamplerDescription& description)
	{
		return RenderStateManager::GetInstance()->CreateSampler(description);
	}

} // video

} // sh