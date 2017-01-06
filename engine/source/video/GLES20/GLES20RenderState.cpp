#include "GLES20RenderState.h"

namespace sh
{
	namespace video
	{
		////////////////////////////////////////////////

		GLES20RenderState::GLES20RenderState()
		{
			Init();
		}

		////////////////////////////////////////////////

		GLES20RenderState::~GLES20RenderState()
		{

		}

		////////////////////////////////////////////////

		RenderState* GLES20RenderState::Clone()
		{
			GLES20RenderState* renderState =  new GLES20RenderState();

			renderState->m_frontFace = m_frontFace;
			renderState->m_cullFace = m_cullFace;
			renderState->m_polygoneMode = m_polygoneMode;
			renderState->m_pointSize = m_pointSize;
			renderState->m_lineWidth = m_lineWidth;
			renderState->m_enableDepthTest = m_enableDepthTest;
			renderState->m_enableScissorTest = m_enableScissorTest;
			renderState->m_enableBlending = m_enableBlending;
			renderState->m_enableCullFace = m_enableCullFace;
			renderState->m_enableStencilTest = m_enableStencilTest;

			renderState->m_glFrontFace = m_glFrontFace;
			renderState->m_glCullFace = m_glCullFace;

			return renderState;
		}

		////////////////////////////////////////////////
	}
}