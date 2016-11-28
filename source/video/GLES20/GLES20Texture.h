#ifndef SHADOW_GLES20_TEXTURE_INCLUDE
#define SHADOW_GLES20_TEXTURE_INCLUDE

#include "../Texture.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		class GLES20Texture : public Texture
		{
		public:
			GLES20Texture();
			virtual ~GLES20Texture();

			virtual void Load(const char* filename) override;
			virtual void SetType(Type type) override;
			virtual void SetTiling(Tiling tilingU, Tiling tilingV) override;
			virtual void SetFiltering(Filtering filtering) override;

			// GLES interface
			u32 GetGLId() const { return m_glID; }
			u32 GetGLType() const { return m_glType; }

		private:
			u32 m_glID = 0U;
			u32 m_glType = GL_TEXTURE_2D;
		};
	}
}

#endif