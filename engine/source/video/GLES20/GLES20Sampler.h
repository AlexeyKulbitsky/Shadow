#ifndef SHADOW_GLES20_SAMPLER_INCLUDE
#define SHADOW_GLES20_SAMPLER_INCLUDE

#include "../Sampler.h"
#include "../../Types.h"          

namespace sh
{
	namespace video
	{
		class GLES20Sampler : public Sampler
		{
		public:
			GLES20Sampler(unsigned int shaderProgramId) : m_shaderProgramId(shaderProgramId) {}
			virtual void Set(TexturePtr texture) override;
			virtual void Load(int textureChannel) override;
			virtual void Init() override;
			virtual Sampler* Clone() override;

			virtual void SetFiltering(TextureFiltering filtering) override;
			virtual void SetTiling(TextureTiling tilingU, TextureTiling tilingV, TextureTiling tilingW = TextureTiling::REPEAT) override;
			virtual void SetType(TextureType type) override;

			// GLES 2 - specific interface
			void SetGLId(s32 id);

		private:
			s32 m_glID;
			u32 m_shaderProgramId;

			u32 m_glType;
			s32 m_glTilingS;
			s32 m_glTilingT;
			s32 m_glTilingR;
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
