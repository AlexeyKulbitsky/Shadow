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
			TextureFiltering minFilter;
			TextureFiltering magFilter;
			TextureFiltering mipFilter;
			TextureTiling tilingU;
			TextureTiling tilingV;
			TextureTiling tilingW;
		};

		class Sampler
		{
		public:
			virtual ~Sampler(){}

			void Set(TexturePtr texture) { m_texture = texture; }
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
