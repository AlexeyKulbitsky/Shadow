#ifndef SHADOW_GL_RENDER_TARGET_INCLUDE
#define SHADOW_GL_RENDER_TARGET_INCLUDE

#include "../RenderTarget.h"
#include "GLCommon.h"

namespace sh
{
	namespace video
	{
		class GLRenderTarget : public RenderTarget
		{
		public:
			GLRenderTarget();
			virtual ~GLRenderTarget();
			virtual void AddColorTexture(const TexturePtr& texture) override;
			virtual void AddDepthTexture(const TexturePtr& texture) override;
			virtual void Init() override;
			virtual void Bind() override;
			virtual void Unbind() override;

		private:
			u32 m_glID = 0U;
			TexturePtr m_colorTexture;
			TexturePtr m_depthTexture;

			GLuint m_depthRenderbuffer = 0U;
		};
	}
}

#endif