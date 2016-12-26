#ifndef SHADOW_GLES20_RENDER_STATE_INCLUDE
#define SHADOW_GLES20_RENDER_STATE_INCLUDE

#include "RenderState.h"
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
			//virtual void Load(const pugi::xml_node &node) override;
			virtual void Apply() override;

			virtual void SetFrontFace(FrontFace frontFace) override;
			virtual void SetCullFace(CullFace cullFace) override;
			virtual void SetPolygonMode(PolygonMode polygonMode) override;
			virtual void SetPointSize(float pointSize) override;
			virtual void SetLineWidth(float lineWidth) override;

		private:
			u32 m_glFrontFace;
			u32 m_glCullFace;
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		inline void GLES20RenderState::Apply()
		{
			// Depth testing
			if (m_enableDepthTest)
				glEnable(GL_DEPTH_TEST);
			else
				glDisable(GL_DEPTH_TEST);

			// Blending
			if (m_enableBlending)
				glEnable(GL_BLEND);
			else
				glDisable(GL_BLEND);

			// Cull face
			if (m_enableCullFace)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);

			// Stencil testing
			if (m_enableStencilTest)
				glEnable(GL_STENCIL_TEST);
			else
				glDisable(GL_STENCIL_TEST);

			// Scissor testing]
			if (m_enableScissorTest)
				glEnable(GL_SCISSOR_TEST);
			else
				glDisable(GL_SCISSOR_TEST);



			glFrontFace(m_glFrontFace);
			glCullFace(m_glCullFace);

			//glLineWidth(m_lineWidth);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		inline void GLES20RenderState::SetFrontFace(FrontFace frontFace) 
		{
			RenderState::SetFrontFace(frontFace);
			switch (frontFace)
			{
			case FrontFace::CLOCKWISE:
				m_glFrontFace = GL_CW;
				break;
			case FrontFace::COUNTER_CLOCKWISE:
				m_glFrontFace = GL_CCW;
				break;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		inline void GLES20RenderState::SetCullFace(CullFace cullFace)
		{ 
			RenderState::SetCullFace(cullFace);
			switch (cullFace)
			{
			case CullFace::BACK:
				m_glCullFace = GL_BACK;
				break;
			case CullFace::FRONT:
				m_glCullFace = GL_FRONT;
				break;
			case CullFace::FRONT_AND_BACK:
				m_glCullFace = GL_FRONT_AND_BACK;
				break;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		inline void GLES20RenderState::SetPolygonMode(PolygonMode polygonMode) 
		{
			RenderState::SetPolygonMode(polygonMode);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		inline void GLES20RenderState::SetPointSize(float pointSize) 
		{ 
			RenderState::SetPointSize(pointSize);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////

		inline void GLES20RenderState::SetLineWidth(float lineWidth) 
		{
			RenderState::SetLineWidth(lineWidth);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}

#endif