#include "GLES20Texture.h"
#include "../../pempek_assert.h"

namespace sh
{
	namespace video
	{

		GLES20Texture::~GLES20Texture()
		{
			glDeleteTextures(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::SetData(u32 mipLevel, const void* data)
		{
			glBindTexture(s_glTextureType[m_description.type], m_glID);
			glTexImage2D(s_glTextureType[m_description.type], mipLevel, GL_RGBA, m_description.width, m_description.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glBindTexture(s_glTextureType[m_description.type], 0U);
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::SetFaceData(TextureFace face, u32 mipLevel, const void* data)
		{
			glBindTexture(s_glTextureType[m_description.type], m_glID);
			glTexImage2D(s_glTextureFace[face], mipLevel, GL_RGBA, m_description.width, m_description.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glBindTexture(s_glTextureType[m_description.type], 0U);
		}

		////////////////////////////////////////////////////////////////////

		/*
		TextureFormat GLES20Texture::GetFormatFromGL(GLenum glFormat)
		{
			TextureFormat result = TextureFormat::UNDEFINED;

			switch (glFormat)
			{
			case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
				result = TextureFormat::DXT1_RGB;
				break;
			case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
				result = TextureFormat::DXT1_RGBA;
				break;
			default:
				break;
			}
			
			return result;
		}
		*/
		////////////////////////////////////////////////////////////////////

		GLES20Texture::GLES20Texture(const TextureDescription& description)
			: Texture(description)
		{
			glGenTextures(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////
	}
}