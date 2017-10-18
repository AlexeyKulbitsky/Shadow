#ifndef SHADOW_GL_TEXTURE_INCLUDE
#define SHADOW_GL_TEXTURE_INCLUDE

#include "../Texture.h"
#include "GLCommon.h"

namespace sh
{
	namespace video
	{
		class GLTexture : public Texture
		{
			//friend class GLES20Driver;
			friend class GLTextureManager;
		public:
			virtual ~GLTexture();

			virtual void SetData(u32 mipLevel, void* data) override;
			virtual void SetFaceData(TextureFace face, u32 mipLevel, const void* data) override;

			// GLES interface
			u32 GetGLId() const { return m_glID; }

		protected:
			GLTexture(const TextureDescription& description);

		private:
			u32 m_glID = 0U;
		};
	}
}

#endif