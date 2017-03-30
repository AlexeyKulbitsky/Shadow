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

			virtual void Load(const String& filePath) override;
			virtual void Load(const std::vector<String>& fileNames) override;
			virtual void Unload() override;

			virtual void SetType(TextureType type) override;
			virtual void SetTiling(TextureTiling tilingU, TextureTiling tilingV) override;
			virtual void SetFiltering(TextureFiltering filtering) override;
			virtual void Bind() override;
			virtual void Unbind() override;
			virtual void LoadData(u32 mipLevel, s32 width, s32 height, const void* data) override;
			virtual void LoadFaceData(TextureFace face, u32 mipLevel, s32 width, s32 height, const void* data) override;
			virtual void GenerateMipMaps() override;

			// GLES interface
			u32 GetGLId() const { return m_glID; }
			u32 GetGLType() const { return m_glType; }

			static TextureFormat GetFormatFromGL(GLenum glFormat);

		private:
			u32 m_glID = 0U;
			u32 m_glType = GL_TEXTURE_2D;
		};
	}
}

#endif