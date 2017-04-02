#include "GLES20Texture.h"
#include "../../pempek_assert.h"

namespace sh
{
	namespace video
	{

		GLES20Texture::GLES20Texture()
		{		
			glGenTextures(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////

		GLES20Texture::~GLES20Texture()
		{
			
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::Load(const String& filePath)
		{
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::Load(const std::vector<String>& fileNames)
		{
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::Unload()
		{
			glDeleteTextures(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::SetType(TextureType type)
		{
			switch (type)
			{
				case sh::TextureType::TEX_TYPE_TEXTURE_1D:
				SH_ASSERT(0, "Texture type TEXTURE_1D is not supported by driver");
				break;
				case sh::TextureType::TEX_TYPE_TEXTURE_2D:
				m_glType = GL_TEXTURE_2D;
				break;
				case sh::TextureType::TEX_TYPE_TEXTURE_3D:
				SH_ASSERT(0, "Texture type TEXTURE_3D is not supported by driver");
				break;
				case sh::TextureType::TEX_TYPE_TEXTURE_CUBE:
				m_glType = GL_TEXTURE_CUBE_MAP;
				break;
			}
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::SetTiling(TextureTiling tilingU, TextureTiling tilingV)
		{
			GLint textureTilingU = GL_REPEAT;
			GLint textureTilingV = GL_REPEAT;

			switch (tilingU)
			{
				case sh::TextureTiling::TEX_TILING_REPEAT:
				textureTilingU = GL_REPEAT;
				break;
			case sh::TextureTiling::TEX_TILING_MIRRORED_REPEAT:
				textureTilingU = GL_MIRRORED_REPEAT;
				break;
			case sh::TextureTiling::TEX_TILING_CLAMP_TO_EDGE:
				textureTilingU = GL_CLAMP_TO_EDGE;
				break;
			default:
				break;
			}

			switch (tilingV)
			{
			case sh::TextureTiling::TEX_TILING_REPEAT:
				textureTilingV = GL_REPEAT;
				break;
			case sh::TextureTiling::TEX_TILING_MIRRORED_REPEAT:
				textureTilingV = GL_MIRRORED_REPEAT;
				break;
			case sh::TextureTiling::TEX_TILING_CLAMP_TO_EDGE:
				textureTilingV = GL_CLAMP_TO_EDGE;
				break;
			default:
				break;
			}

			//glBindTexture(m_glType, m_glID);

			glTexParameteri(m_glType, GL_TEXTURE_WRAP_S, textureTilingU);
			glTexParameteri(m_glType, GL_TEXTURE_WRAP_T, textureTilingV);
			if (m_glType == GL_TEXTURE_CUBE_MAP)
			{
				//glTexParameteri(m_glType, GL_TEXTURE_WRAP_R, textureTiling);
			}

			//glBindTexture(m_glType, 0U);
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::SetFiltering(TextureFiltering filtering)
		{
			GLint filter;
			switch (filtering)
			{
			case sh::TextureFiltering::TEX_FILT_NEAREST:
				filter = GL_NEAREST;
				break;
			case sh::TextureFiltering::TEX_FILT_LINEAR:
				filter = GL_NEAREST_MIPMAP_NEAREST;
				break;
			case sh::TextureFiltering::TEX_FILT_BILINEAR:
				filter = GL_LINEAR_MIPMAP_NEAREST;
				break;
			case sh::TextureFiltering::TEX_FILT_TRILINEAR:
				filter = GL_NEAREST_MIPMAP_LINEAR;
				break;
			case sh::TextureFiltering::TEX_FILT_ANISOTROPIC:
				filter = GL_LINEAR_MIPMAP_LINEAR;
				break;
			default:
				break;
			}

			glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, filter);
			glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, filter);
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::Bind()
		{
			glBindTexture(m_glType, m_glID);
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::Unbind()
		{
			glBindTexture(m_glType, 0U);
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::LoadData(u32 mipLevel, s32 width, s32 height, const void* data)
		{
			glTexImage2D(m_glType, mipLevel, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::LoadFaceData(TextureFace face, u32 mipLevel, s32 width, s32 height, const void* data)
		{

			GLenum target;
			switch (face)
			{
				case TextureFace::TEX_FRONT_FACE:
					target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
					break;
				case TextureFace::TEX_BACK_FACE:
					target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
					break;
				case TextureFace::TEX_RIGHT_FACE:
					target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
					break;
				case TextureFace::TEX_LEFT_FACE:
					target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
					break;
				case TextureFace::TEX_TOP_FACE:
					target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
					break;
				case TextureFace::TEX_BOTTOM_FACE:
					target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
					break;
			}

			glTexImage2D(target, mipLevel, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::GenerateMipMaps()
		{
			glGenerateMipmap(m_glType);
			m_hasMipMaps = true;
		}

		////////////////////////////////////////////////////////////////////

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

		////////////////////////////////////////////////////////////////////

		GLES20Texture::GLES20Texture(const TextureDescription& description)
			: Texture(description)
		{
			glGenTextures(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////
	}
}