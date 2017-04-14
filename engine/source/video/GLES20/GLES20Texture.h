#ifndef SHADOW_GLES20_TEXTURE_INCLUDE
#define SHADOW_GLES20_TEXTURE_INCLUDE

#include "../Texture.h"
#include "GLES20Common.h"

namespace sh
{
	namespace video
	{
		class GLES20Texture : public Texture
		{
			friend class GLES20Driver;
			friend class GLES20TextureManager;
		public:
			virtual ~GLES20Texture();

			virtual void SetData(u32 mipLevel, const void* data) override;
			virtual void SetFaceData(TextureFace face, u32 mipLevel, const void* data) override;

			// GLES interface
			u32 GetGLId() const { return m_glID; }

		protected:
			GLES20Texture(const TextureDescription& description);

		private:
			u32 m_glID = 0U;
		};
	}
}

#endif