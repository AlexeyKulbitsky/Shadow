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
			void SetGLId(int id);

		private:
			int m_glID;
			unsigned int m_shaderProgramId;

			unsigned int m_glType;
			int m_glTilingU;
			int m_glTilingV;
			int m_glMagFilter;
			int m_glMinFilter;
		};

		inline void GLES20Sampler::SetGLId(int id)
		{
			m_shaderProgramId = id;
		}
	}
}

#endif
