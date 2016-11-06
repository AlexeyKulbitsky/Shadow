#include "GLES20Texture.h"
#include "../../pempek_assert.h"

//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace sh
{
	namespace video
	{

		GLES20Texture::GLES20Texture()
		{		
			glGenTextures(1, &m_glID);
		}

		GLES20Texture::~GLES20Texture()
		{
			glDeleteTextures(1, &m_glID);
		}

		void GLES20Texture::Load(const char* filename)
		{
			int texWidth, texHeight, texChannels;
			stbi_uc* pixels = stbi_load(filename, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
			SH_ASSERT(pixels, "failed to load texture image!");

			glBindTexture(m_glType, m_glID);

			glTexImage2D(m_glType, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

			glBindTexture(m_glType, 0U);

			stbi_image_free(pixels);
		}

		void GLES20Texture::SetType(Type type)
		{
			switch (type)
			{
			case sh::video::Texture::Type::TEXTURE_1D:
				SH_ASSERT(0, "texture type TEXTURE_1D is not supported by driver");
				break;
			case sh::video::Texture::Type::TEXTURE_2D:
				m_glType = GL_TEXTURE_2D;
				break;
			case sh::video::Texture::Type::TEXTURE_3D:
				SH_ASSERT(0, "texture type TEXTURE_3D is not supported by driver");
				break;
			case sh::video::Texture::Type::TEXTURE_CUBE:
				m_glType = GL_TEXTURE_CUBE_MAP;
				break;
			}
		}

		void GLES20Texture::SetTiling(Tiling tiling)
		{
			GLint textureTiling = GL_REPEAT;			
			switch (tiling)
			{
			case sh::video::Texture::Tiling::REPEAT:
				textureTiling = GL_REPEAT;
				break;
			case sh::video::Texture::Tiling::MIRRORED_REPEAT:
				textureTiling = GL_MIRRORED_REPEAT;
				break;
			case sh::video::Texture::Tiling::CLAMP_TO_EDGE:
				textureTiling = GL_CLAMP_TO_EDGE;
				break;
			default:
				break;
			}
			glBindTexture(m_glType, m_glID);

			glTexParameteri(m_glType, GL_TEXTURE_WRAP_S, textureTiling);
			glTexParameteri(m_glType, GL_TEXTURE_WRAP_T, textureTiling);
			if (m_glType == GL_TEXTURE_CUBE_MAP)
			{
				glTexParameteri(m_glType, GL_TEXTURE_WRAP_R, textureTiling);
			}

			glBindTexture(m_glType, 0U);
		}

		void GLES20Texture::SetFiltering(Filtering filtering)
		{
			GLint filter;
			switch (filtering)
			{
			case sh::video::Texture::Filtering::LINEAR:
				filter = GL_NEAREST_MIPMAP_NEAREST;
				break;
			case sh::video::Texture::Filtering::BILINEAR:
				filter = GL_LINEAR_MIPMAP_NEAREST;
				break;
			case sh::video::Texture::Filtering::TRILINEAR:
				filter = GL_NEAREST_MIPMAP_LINEAR;
				break;
			case sh::video::Texture::Filtering::ANISOTROPIC:
				filter = GL_LINEAR_MIPMAP_LINEAR;
				break;
			default:
				break;
			}

			glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, filter);
		}
	}
}