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

		void GLES20Texture::SetType(Type type)
		{
			switch (type)
			{
			case sh::video::Texture::Type::TEXTURE_1D:
				SH_ASSERT(0, "Texture type TEXTURE_1D is not supported by driver");
				break;
			case sh::video::Texture::Type::TEXTURE_2D:
				m_glType = GL_TEXTURE_2D;
				break;
			case sh::video::Texture::Type::TEXTURE_3D:
				SH_ASSERT(0, "Texture type TEXTURE_3D is not supported by driver");
				break;
			case sh::video::Texture::Type::TEXTURE_CUBE:
				m_glType = GL_TEXTURE_CUBE_MAP;
				break;
			}
		}

		////////////////////////////////////////////////////////////////////

		void GLES20Texture::SetTiling(Tiling tilingU, Tiling tilingV)
		{
			GLint textureTilingU = GL_REPEAT;
			GLint textureTilingV = GL_REPEAT;

			switch (tilingU)
			{
			case sh::video::Texture::Tiling::REPEAT:
				textureTilingU = GL_REPEAT;
				break;
			case sh::video::Texture::Tiling::MIRRORED_REPEAT:
				textureTilingU = GL_MIRRORED_REPEAT;
				break;
			case sh::video::Texture::Tiling::CLAMP_TO_EDGE:
				textureTilingU = GL_CLAMP_TO_EDGE;
				break;
			default:
				break;
			}

			switch (tilingV)
			{
			case sh::video::Texture::Tiling::REPEAT:
				textureTilingV = GL_REPEAT;
				break;
			case sh::video::Texture::Tiling::MIRRORED_REPEAT:
				textureTilingV = GL_MIRRORED_REPEAT;
				break;
			case sh::video::Texture::Tiling::CLAMP_TO_EDGE:
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

		void GLES20Texture::SetFiltering(Filtering filtering)
		{
			GLint filter;
			switch (filtering)
			{
			case sh::video::Texture::Filtering::NEAREST:
				filter = GL_NEAREST;
				break;
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

		void GLES20Texture::LoadFaceData(Face face, u32 mipLevel, s32 width, s32 height, const void* data)
		{

			GLenum target;
			switch (face)
			{
				case Face::FRONT_FACE:
					target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
					break;
				case Face::BACK_FACE:
					target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
					break;
				case Face::RIGHT_FACE:
					target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
					break;
				case Face::LEFT_FACE:
					target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
					break;
				case Face::TOP_FACE:
					target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
					break;
				case Face::BOTTOM_FACE:
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

		Texture::Format GLES20Texture::GetFormatFromGL(GLenum glFormat)
		{
			Format result = Format::UNDEFINED;

			switch (glFormat)
			{
			case GL_COMPRESSED_RGB_S3TC_DXT1_EXT:
				result = Format::DXT1_RGB;
				break;
			case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
				result = Format::DXT1_RGBA;
				break;
			default:
				break;
			}
			
			return result;
		}
	}
}