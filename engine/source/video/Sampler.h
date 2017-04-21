#ifndef SHADOW_SAMPLER_INCLUDE
#define SHADOW_SAMPLER_INCLUDE

#include "../Globals.h"
#include "Texture.h"

namespace sh
{
	namespace video
	{
		struct SamplerDescription
		{
			GpuParamObjectType type;
			TextureFiltering minFilter = TEX_FILT_NONE;
			TextureFiltering magFilter = TEX_FILT_NONE;
			TextureFiltering mipFilter = TEX_FILT_NONE;
			TextureTiling tilingU = TEX_TILING_NONE;
			TextureTiling tilingV = TEX_TILING_NONE;
			TextureTiling tilingW = TEX_TILING_NONE;
		};

		class Sampler
		{
		public:
			virtual ~Sampler(){}

			virtual void Set(TexturePtr texture) { m_texture = texture; }
			const TexturePtr& GetTexture() const { return m_texture; }
			virtual void Load(int textureChannel){}

			const SamplerDescription& GetDescription() const { return m_description; }

			static SamplerPtr Create(const SamplerDescription& description);
		
		protected:
			SamplerDescription m_description;
			TexturePtr m_texture;
		};
	}
}

#endif
