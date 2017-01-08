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
			static GLenum const s_glFrontFace[];
			static GLenum const s_glCullFace[];
			static GLenum const s_glCompareFunction[];
			static GLenum const s_glStencilOperation[];
			static GLenum const s_glBlendFactor[];
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		inline void GLES20RenderState::Apply()
		{
			glFrontFace(s_glFrontFace[static_cast<size_t>(rasterizerState.frontFace)]);
			
			if (rasterizerState.cullFace != CullFace::NONE)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(s_glCullFace[static_cast<size_t>(rasterizerState.cullFace)]);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}

			
			// DepthStencil State
			if (depthStencilState.enableDepthTest)
			{
				glEnable(GL_DEPTH_TEST);
				//glDepthFunc(s_glCompareFunction[static_cast<size_t>(depthStencilState.depthCompareFunction)]);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
			//glDepthMask(mWriteMask);

			if (depthStencilState.enableStencilTest)
			{
				glEnable(GL_STENCIL_TEST);

			}
			else
			{
				glDisable(GL_STENCIL_TEST);

				//glStencilFuncSeparate(GL_FRONT, mFrontFace.comparison, mReference, mStencilReadMask);				
				//glStencilMaskSeparate(GL_FRONT, mStencilWriteMask);
				//glStencilOpSeparate(GL_FRONT, mFrontFace.onFail, mFrontFace.onZFail, mFrontFace.onZPass);
				//glStencilFuncSeparate(GL_BACK, mBackFace.comparison, mReference, mStencilReadMask);
				//glStencilMaskSeparate(GL_BACK, mStencilWriteMask);
				//glStencilOpSeparate(GL_BACK, mBackFace.onFail, mBackFace.onZFail, mBackFace.onZPass);
			}			
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////
	
	}
}

#endif