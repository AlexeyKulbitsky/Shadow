#include "GLES20Sampler.h"
#include "GLES20Texture.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		void GLES20Sampler::Set(Texture* texture)
		{
			GLES20Texture* glesTexture = static_cast<GLES20Texture*>(texture);
			m_glID = glesTexture->GetGLId();
			m_glType = glesTexture->GetGLType();
			m_hasMipMaps = glesTexture->HasMipMaps();
			// We must reset filtering for that case if mipmaping switched
			SetFiltering(m_filtering);
		}

		void GLES20Sampler::Load(int textureChannel)
		{
			glActiveTexture(GL_TEXTURE0 + textureChannel);
			glBindTexture(m_glType, m_glID);

			glTexParameteri(m_glType, GL_TEXTURE_WRAP_S, m_glTilingS);
			glTexParameteri(m_glType, GL_TEXTURE_WRAP_T, m_glTilingT);
			if (m_type == Texture::Type::TEXTURE_CUBE)
			{
				glTexParameteri(m_glType, GL_TEXTURE_WRAP_R, m_glTilingR);
			}
		
			glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, m_glMagFilter);
			glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, m_glMinFilter);
			
			glUniform1i(m_glID, textureChannel);
		}

		void GLES20Sampler::Init()
		{
			m_glID = glGetUniformLocation(m_shaderProgramId, m_name.c_str());
		}

		Sampler* GLES20Sampler::Clone()
		{
			GLES20Sampler* result = new GLES20Sampler(m_shaderProgramId);
			result->m_filtering = m_filtering;
			result->m_glID = m_glID;
			result->m_glMagFilter = m_glMagFilter;
			result->m_glMinFilter = m_glMinFilter;
			result->m_glTilingS = m_glTilingS;
			result->m_glTilingT = m_glTilingT;
			result->m_glTilingR = m_glTilingR;
			result->m_glType = m_glType;
			result->m_name = m_name;
			result->m_type = m_type;
			result->m_usage = m_usage;
			return result;
		}

		void GLES20Sampler::SetFiltering(Texture::Filtering filtering)
		{
			Sampler::SetFiltering(filtering);
			switch (filtering)
			{
			case sh::video::Texture::Filtering::NEAREST:
				m_glMagFilter = GL_NEAREST;
				m_glMinFilter = GL_NEAREST;
				break;
			case sh::video::Texture::Filtering::LINEAR:
				m_glMagFilter = GL_NEAREST;
				m_glMinFilter = GL_NEAREST_MIPMAP_NEAREST;
				break;
			case sh::video::Texture::Filtering::BILINEAR:
			{
				m_glMagFilter = GL_LINEAR;
				if (m_hasMipMaps)
				{
					m_glMinFilter = GL_LINEAR_MIPMAP_NEAREST;
				}
				else
				{
					m_glMinFilter = GL_LINEAR;
				}
			}								
				break;
				// Requires mip maps. If there are no mip maps than 
				// will be use bilinear filter
			case sh::video::Texture::Filtering::TRILINEAR:
			case sh::video::Texture::Filtering::ANISOTROPIC:
			{
				m_glMagFilter = GL_LINEAR;
				if (m_hasMipMaps)
				{
					m_glMinFilter = GL_LINEAR_MIPMAP_LINEAR;
				}
				else
				{
					m_glMinFilter = GL_LINEAR;
				}			
			}			
				break;
			default:
				break;
			}
		}

		void GLES20Sampler::SetTiling(Texture::Tiling tilingU, Texture::Tiling tilingV, Texture::Tiling tilingW)
		{
			Sampler::SetTiling(tilingU, tilingV, tilingW);

			switch (tilingU)
			{
			case sh::video::Texture::Tiling::REPEAT:
				m_glTilingS = GL_REPEAT;
				break;
			case sh::video::Texture::Tiling::MIRRORED_REPEAT:
				m_glTilingS = GL_MIRRORED_REPEAT;
				break;
			case sh::video::Texture::Tiling::CLAMP_TO_EDGE:
				m_glTilingS = GL_CLAMP_TO_EDGE;
				break;
			default:
				break;
			}

			switch (tilingV)
			{
			case sh::video::Texture::Tiling::REPEAT:
				m_glTilingT = GL_REPEAT;
				break;
			case sh::video::Texture::Tiling::MIRRORED_REPEAT:
				m_glTilingT = GL_MIRRORED_REPEAT;
				break;
			case sh::video::Texture::Tiling::CLAMP_TO_EDGE:
				m_glTilingT = GL_CLAMP_TO_EDGE;
				break;
			default:
				break;
			}

			switch (tilingW)
			{
			case sh::video::Texture::Tiling::REPEAT:
				m_glTilingR = GL_REPEAT;
				break;
			case sh::video::Texture::Tiling::MIRRORED_REPEAT:
				m_glTilingR = GL_MIRRORED_REPEAT;
				break;
			case sh::video::Texture::Tiling::CLAMP_TO_EDGE:
				m_glTilingR = GL_CLAMP_TO_EDGE;
				break;
			default:
				break;
			}
		}

		void GLES20Sampler::SetType(Texture::Type type)
		{
			Sampler::SetType(type);
			switch (type)
			{
			case Texture::Type::TEXTURE_2D:
				m_glType = GL_TEXTURE_2D;
				break;
			case Texture::Type::TEXTURE_CUBE:
				m_glType = GL_TEXTURE_CUBE_MAP;
				break;
			}
		}
	}
}