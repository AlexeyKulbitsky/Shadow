#include "GLRenderTarget.h"
#include "GLTexture.h"
#include "../../Device.h"

namespace sh
{
	namespace video
	{
		GLRenderTarget::GLRenderTarget()
		{
			glGenFramebuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		GLRenderTarget::~GLRenderTarget()
		{ 
			if (m_depthRenderbuffer != 0U)
			{
				glDeleteRenderbuffers(1, &m_depthRenderbuffer);
			}
			glDeleteFramebuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		void GLRenderTarget::AddColorTexture(const TexturePtr& texture)
		{
			m_colorTexture = texture;
		}

		////////////////////////////////////////////////////////////////////////

		void GLRenderTarget::AddDepthTexture(const TexturePtr& texture)
		{
			m_depthTexture = texture;
		}

		////////////////////////////////////////////////////////////////////////

		void GLRenderTarget::Init()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_glID);

			Driver *driver = Device::GetInstance()->GetDriver();
			const math::Vector4& viewport = driver->GetViewPort();
			u32 width = viewport.z;
			u32 height = viewport.w;
			GLTexture *colorTexture = static_cast<GLTexture*>(m_colorTexture.get());
			GLTexture *depthTexture = static_cast<GLTexture*>(m_depthTexture.get());

			//colorTexture->SetType(TextureType::TEX_TYPE_TEXTURE_2D);
			//colorTexture->Bind();
			//colorTexture->LoadData(0, width, height, nullptr);
			//colorTexture->SetFiltering(TextureFiltering::TEX_FILT_NEAREST);
			//colorTexture->SetTiling(TextureTiling::TEX_TILING_CLAMP_TO_EDGE, TextureTiling::TEX_TILING_CLAMP_TO_EDGE);		
			//colorTexture->Unbind();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->GetGLId(), 0);

			if (depthTexture)
			{
				//depthTexture->SetType(TextureType::TEX_TYPE_TEXTURE_2D);
				//depthTexture->Bind();
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
				//depthTexture->SetFiltering(TextureFiltering::TEX_FILT_NEAREST);
				//depthTexture->SetTiling(TextureTiling::TEX_TILING_CLAMP_TO_EDGE, TextureTiling::TEX_TILING_CLAMP_TO_EDGE);
				//depthTexture->Unbind();			
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

		void GLRenderTarget::Bind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		void GLRenderTarget::Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		////////////////////////////////////////////////////////////////////////

	}
}