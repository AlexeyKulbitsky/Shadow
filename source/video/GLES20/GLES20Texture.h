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
			unsigned int GetGLId() const { return m_glID; }
			unsigned int GetGLType() const { return m_glType; }

		private:
			GLuint m_glID = 0U;
			GLenum m_glType = GL_TEXTURE_2D;
		};
	}
}

#endif