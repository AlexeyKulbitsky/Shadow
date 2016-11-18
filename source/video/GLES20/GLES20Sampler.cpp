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
		}

		void GLES20Sampler::Load(int textureChannel)
		{
			glActiveTexture(GL_TEXTURE0 + textureChannel);
			glBindTexture(m_glType, m_glID);
			glTexParameteri(m_glType, GL_TEXTURE_WRAP_S, m_glTilingU);
			glTexParameteri(m_glType, GL_TEXTURE_WRAP_T, m_glTilingV);
			glUniform1i(m_glID, textureChannel);
		}

		void GLES20Sampler::Init()
		{
			m_glID = glGetUniformLocation(m_shaderProgramId, m_name.c_str());
		}

		void GLES20Sampler::SetFiltering(Texture::Filtering filtering)
		{
			Sampler::SetFiltering(filtering);
			switch (filtering)
			{
			case sh::video::Texture::Filtering::LINEAR:
				m_glFilter = GL_NEAREST_MIPMAP_NEAREST;
				break;
			case sh::video::Texture::Filtering::BILINEAR:
				m_glFilter = GL_LINEAR_MIPMAP_NEAREST;
				break;
			case sh::video::Texture::Filtering::TRILINEAR:
				m_glFilter = GL_NEAREST_MIPMAP_LINEAR;
				break;
			case sh::video::Texture::Filtering::ANISOTROPIC:
				m_glFilter = GL_LINEAR_MIPMAP_LINEAR;
				break;
			default:
				break;
			}
		}

		void GLES20Sampler::SetTiling(Texture::Tiling tilingU, Texture::Tiling tilingV)
		{
			Sampler::SetTiling(tilingU, tilingV);

			switch (tilingU)
			{
			case sh::video::Texture::Tiling::REPEAT:
				m_glTilingU = GL_REPEAT;
				break;
			case sh::video::Texture::Tiling::MIRRORED_REPEAT:
				m_glTilingU = GL_MIRRORED_REPEAT;
				break;
			case sh::video::Texture::Tiling::CLAMP_TO_EDGE:
				m_glTilingU = GL_CLAMP_TO_EDGE;
				break;
			default:
				break;
			}

			switch (tilingV)
			{
			case sh::video::Texture::Tiling::REPEAT:
				m_glTilingV = GL_REPEAT;
				break;
			case sh::video::Texture::Tiling::MIRRORED_REPEAT:
				m_glTilingV = GL_MIRRORED_REPEAT;
				break;
			case sh::video::Texture::Tiling::CLAMP_TO_EDGE:
				m_glTilingV = GL_CLAMP_TO_EDGE;
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