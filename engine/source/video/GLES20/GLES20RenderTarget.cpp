#include "GLES20RenderTarget.h"
#include "GLES20Texture.h"
#include "GLES20Driver.h"
#include "../../Device.h"

namespace sh
{
	namespace video
	{
		GLES20RenderTarget::GLES20RenderTarget()
		{
			glGenFramebuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		GLES20RenderTarget::~GLES20RenderTarget()
		{ 
			if (m_depthRenderbuffer != 0U)
			{
				glDeleteRenderbuffers(1, &m_depthRenderbuffer);
			}
			glDeleteFramebuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20RenderTarget::AddColorTexture(const TexturePtr& texture)
		{
			m_colorTexture = texture;
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20RenderTarget::AddDepthTexture(const TexturePtr& texture)
		{
			m_depthTexture = texture;
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20RenderTarget::Init()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_glID);

			Driver *driver = Device::GetInstance()->GetDriver();
			const math::Vector4u& viewport = driver->GetViewPort();
			u32 width = viewport.z;
			u32 height = viewport.w;
			GLES20Texture *colorTexture = static_cast<GLES20Texture*>(m_colorTexture.get());
			GLES20Texture *depthTexture = static_cast<GLES20Texture*>(m_depthTexture.get());

			colorTexture->SetType(Texture::Type::TEXTURE_2D);
			colorTexture->Bind();
			colorTexture->LoadData(0, width, height, nullptr);
			colorTexture->SetFiltering(Texture::Filtering::NEAREST);
			colorTexture->SetTiling(Texture::Tiling::CLAMP_TO_EDGE, Texture::Tiling::CLAMP_TO_EDGE);		
			colorTexture->Unbind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->GetGLId(), 0);

			if (depthTexture)
			{
				depthTexture->SetType(Texture::Type::TEXTURE_2D);
				depthTexture->Bind();
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
				depthTexture->SetFiltering(Texture::Filtering::NEAREST);
				depthTexture->SetTiling(Texture::Tiling::CLAMP_TO_EDGE, Texture::Tiling::CLAMP_TO_EDGE);
				depthTexture->Unbind();			
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->GetGLId(), 0);
			}
			else
			{
				glGenRenderbuffers(1, &m_depthRenderbuffer);
				glBindRenderbuffer(GL_RENDERBUFFER, m_depthRenderbuffer);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
				glBindRenderbuffer(GL_RENDERBUFFER, 0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthRenderbuffer);
			}
			

			GLenum error = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (error != GL_FRAMEBUFFER_COMPLETE)
			{				
				SH_ASSERT(0, "Can not create GLES 2.0 framebuffer");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0U);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20RenderTarget::Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20RenderTarget::Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		////////////////////////////////////////////////////////////////////////

	}
}