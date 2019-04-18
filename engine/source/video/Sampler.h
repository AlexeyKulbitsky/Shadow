#ifndef SHADOW_SAMPLER_INCLUDE
#define SHADOW_SAMPLER_INCLUDE

#include "prerequisities/Prerequisities.h"
#include "video/Common.h"

namespace sh
{

namespace video
{
	
    struct SHADOW_API SamplerDescription
    {
        GpuParamObjectType type = GPOT_SAMPLER_2D;
        TextureFiltering minFilter = TEX_FILT_LINEAR;
        TextureFiltering magFilter = TEX_FILT_LINEAR;
        TextureFiltering mipFilter = TEX_FILT_NONE;
        TextureTiling tilingU = TEX_TILING_REPEAT;
        TextureTiling tilingV = TEX_TILING_REPEAT;
        TextureTiling tilingW = TEX_TILING_REPEAT;
    };

    class SHADOW_API Sampler
    {
    public:
        Sampler(const SamplerDescription& description);
        virtual ~Sampler();

        virtual void Set(TexturePtr texture);
        const TexturePtr& GetTexture() const;
        virtual void Load(int textureChannel);

        const SamplerDescription& GetDescription() const;

        static SamplerPtr Create(const SamplerDescription& description);

    protected:
        SamplerDescription m_description;
        TexturePtr m_texture;
    };

} // video

} // sh

#endif
