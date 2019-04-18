#include "Sampler.h"
#include "Managers/RenderStateManager.h"

namespace sh
{

namespace video
{
	Sampler::Sampler(const SamplerDescription& description)
		: m_description(description)
	{
		//m_texture = TextureLoader::GetInstance()->GetCheckerTexture();
	}

    Sampler::~Sampler() 
    {
    }

    void Sampler::Set(TexturePtr texture) 
    { 
        m_texture = texture; 
    }

    const TexturePtr& Sampler::GetTexture() const 
    { 
        return m_texture; 
    }

    void Sampler::Load(int textureChannel) 
    {
    }

    const SamplerDescription& Sampler::GetDescription() const 
    { 
        return m_description; 
    }

	SamplerPtr Sampler::Create(const SamplerDescription& description)
	{
		//return RenderStateManager::GetInstance()->CreateSampler(description);
        return SamplerPtr();
    }

} // video

} // sh