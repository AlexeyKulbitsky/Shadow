#include "Sampler.h"
#include "Managers/RenderStateManager.h"

namespace sh
{

namespace video
{

	SamplerPtr Sampler::Create(const SamplerDescription& description)
	{
		return RenderStateManager::GetInstance()->CreateSampler(description);
	}

} // video

} // sh