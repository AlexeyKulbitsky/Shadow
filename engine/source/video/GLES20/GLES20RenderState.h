#ifndef SHADOW_GLES20_RENDER_STATE_INCLUDE
#define SHADOW_GLES20_RENDER_STATE_INCLUDE

#include "../RenderState.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		class GLES20RenderState : public RenderState
		{
		public:
			GLES20RenderState();
			virtual ~GLES20RenderState();
			virtual void Apply() override;
			virtual RenderState* Clone() override;

		private:
			// Maps for converting Shadow's engine state value to GL-specific values
			static GLenum const s_glWriteMask[];
			static GLenum const s_glFrontFace[];
			static GLenum const s_glCullFace[];
			static GLenum const s_glCompareFunction[];
			static GLenum const s_glStencilOperation[];
			static GLenum const s_glBlendFactor[];
			static GLenum const s_gBlendOperation[];
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		inline void GLES20RenderState::Apply()
		{
			// Rasterizer State -------------------------------------------------------------
			if (rasterizerState.cullFace != CullFace::NONE)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(s_glCullFace[static_cast<size_t>(rasterizerState.cullFace)]);
				glFrontFace(s_glFrontFace[static_cast<size_t>(rasterizerState.frontFace)]);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
			// Rasterizer State -------------------------------------------------------------

			
			// DepthStencil State -------------------------------------------------------------
			// Depth part
			if (depthStencilState.enableDepthTest)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(s_glCompareFunction[static_cast<size_t>(depthStencilState.depthCompareFunction)]);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
			glDepthMask(s_glWriteMask[static_cast<size_t>(depthStencilState.depthWriteMask)]);

			// Stencil part
			if (depthStencilState.enableStencilTest)
			{
				glEnable(GL_STENCIL_TEST);
			}
			else
			{
				glDisable(GL_STENCIL_TEST);

				glStencilFuncSeparate(GL_FRONT,
					s_glCompareFunction[static_cast<size_t>(depthStencilState.frontFace.compare)],
					depthStencilState.reference,
					depthStencilState.stencilReadMask);
				glStencilMaskSeparate(GL_FRONT, depthStencilState.stencilWriteMask);
				glStencilOpSeparate(GL_FRONT,
					s_glStencilOperation[static_cast<size_t>(depthStencilState.frontFace.fail)],
					s_glStencilOperation[static_cast<size_t>(depthStencilState.frontFace.depthFail)],
					s_glStencilOperation[static_cast<size_t>(depthStencilState.frontFace.pass)]);

				glStencilFuncSeparate(GL_BACK,
					s_glCompareFunction[static_cast<size_t>(depthStencilState.backFace.compare)],
					depthStencilState.reference,
					depthStencilState.stencilReadMask);
				glStencilMaskSeparate(GL_BACK, depthStencilState.stencilWriteMask);
				glStencilOpSeparate(GL_BACK,
					s_glStencilOperation[static_cast<size_t>(depthStencilState.backFace.fail)],
					s_glStencilOperation[static_cast<size_t>(depthStencilState.backFace.depthFail)],
					s_glStencilOperation[static_cast<size_t>(depthStencilState.backFace.pass)]);
			}	
			// DepthStencil State -------------------------------------------------------------
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	}
}

#endif