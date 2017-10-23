#include "GLTexture.h"
//#include "../../../pempek_assert.h"

namespace sh
{
	namespace video
	{

		GLTexture::~GLTexture()
		{
			GL_CALL(glDeleteTextures(1, &m_glID));
		}

		////////////////////////////////////////////////////////////////////

		void GLTexture::SetData(u32 mipLevel, void* data)
		{
			GL_CALL(glBindTexture(s_glTextureType[m_description.type], m_glID));
			GL_CALL(glTexImage2D(s_glTextureType[m_description.type],
						  mipLevel, 
						  s_glTextureFormat[static_cast<size_t>(m_description.format)], 
						  m_description.width, 
						  m_description.height, 
						  0, 
						  s_glTextureFormat[static_cast<size_t>(m_description.format)], 
						  GL_UNSIGNED_BYTE, 
						  data));
			//glGenerateMipmap(s_glTextureType[m_description.type]);
			GL_CALL(glBindTexture(s_glTextureType[m_description.type], 0U));
		}

		////////////////////////////////////////////////////////////////////

		void GLTexture::SetFaceData(TextureFace face, u32 mipLevel, const void* data)
		{
			GL_CALL(glBindTexture(s_glTextureType[m_description.type], m_glID));
			GL_CALL(glTexImage2D(s_glTextureFace[face],
						  mipLevel, 
						  s_glTextureFormat[static_cast<size_t>(m_description.format)], 
						  m_description.width, 
						  m_description.height, 
						  0, 
						  s_glTextureFormat[static_cast<size_t>(m_description.format)], 
						  GL_UNSIGNED_BYTE, 
						  data));
			GL_CALL(glBindTexture(s_glTextureType[m_description.type], 0U));
		}

		////////////////////////////////////////////////////////////////////

		GLTexture::GLTexture(const TextureDescription& description)
			: Texture(description)
		{
			GL_CALL(glGenTextures(1, &m_glID));
		}

		////////////////////////////////////////////////////////////////////
	}
}