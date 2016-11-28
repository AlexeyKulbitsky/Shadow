#ifndef SHADOW_GLES20_SAMPLER_INCLUDE
#define SHADOW_GLES20_SAMPLER_INCLUDE

#include "../Sampler.h"

namespace sh
{
	namespace video
	{
		class GLES20Sampler : public Sampler
		{
		public:
			GLES20Sampler(unsigned int shaderProgramId) : m_shaderProgramId(shaderProgramId) {}
			virtual void Set(Texture* texture) override;
			virtual void Load(int textureChannel) override;
			virtual void Init() override;

			virtual void SetFiltering(Texture::Filtering filtering) override;
			virtual void SetTiling(Texture::Tiling tilingU, Texture::Tiling tilingV) override;
			virtual void SetType(Texture::Type type) override;

			// GLES 2 - specific interface
			void SetGLId(s32 id);

		private:
			s32 m_glID;
			u32 m_shaderProgramId;

			u32 m_glType;
			s32 m_glTilingU;
			s32 m_glTilingV;
			s32 m_glMagFilter;
			s32 m_glMinFilter;
		};

		inline void GLES20Sampler::SetGLId(int id)
		{
			m_shaderProgramId = id;
		}
	}
}

#endif
