#ifndef SHADOW_GLES20_RENDER_TARGET_INCLUDE
#define SHADOW_GLES20_RENDER_TARGET_INCLUDE

#include "../../RenderTarget.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		class GLES20RenderTarget : public RenderTarget
		{
		public:
			GLES20RenderTarget();
			virtual ~GLES20RenderTarget();
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