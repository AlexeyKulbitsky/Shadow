#include "GLES20RenderState.h"

namespace sh
{
	namespace video
	{	
		GLenum const GLES20RenderState::s_glWriteMask[] =
		{
			GL_FALSE,
			GL_TRUE
		};

		GLenum const GLES20RenderState::s_glFrontFace[] =
		{
			GL_CW,
			GL_CCW
		};

		GLenum const GLES20RenderState::s_glCullFace[] =
		{
			GL_FRONT,
			GL_BACK,
			GL_NONE
		};

		GLenum const GLES20RenderState::s_glCompareFunction[] =
		{
			GL_LESS,
			GL_LEQUAL,
			GL_EQUAL,
			GL_GEQUAL,
			GL_GREATER,
			GL_NOTEQUAL,
			GL_ALWAYS,
			GL_NEVER
		};

		GLenum const GLES20RenderState::s_glStencilOperation[] =
		{
			GL_KEEP,
			GL_ZERO,
			GL_REPLACE,
			GL_INVERT,
			GL_INCR,
			GL_DECR,
			GL_INCR_WRAP,
			GL_DECR_WRAP
		};

		GLenum const GLES20RenderState::s_glBlendFactor[] =
		{
			GL_ZERO,
			GL_ONE,
			GL_SRC_COLOR,
			GL_ONE_MINUS_SRC_COLOR,
			GL_DST_COLOR,
			GL_ONE_MINUS_DST_COLOR,
			GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA,
			GL_DST_ALPHA,
			GL_ONE_MINUS_DST_ALPHA,
			GL_SRC_ALPHA_SATURATE
		};
		
		GLenum const GLES20RenderState::s_gBlendOperation[] =
		{
			GL_FUNC_ADD,
			GL_FUNC_SUBTRACT,
			GL_FUNC_REVERSE_SUBTRACT,
			GL_MIN,
			GL_MAX
		};

		////////////////////////////////////////////////

		GLES20RenderState::GLES20RenderState()
		{
		}

		////////////////////////////////////////////////

		GLES20RenderState::~GLES20RenderState()
		{
		}

		////////////////////////////////////////////////

		RenderState* GLES20RenderState::Clone()
		{
			GLES20RenderState* renderState = new GLES20RenderState();

			renderState->depthStencilState = depthStencilState;
			renderState->rasterizerState = rasterizerState;
			renderState->blendingState = blendingState;

			return renderState;
		}

		////////////////////////////////////////////////
	}
}