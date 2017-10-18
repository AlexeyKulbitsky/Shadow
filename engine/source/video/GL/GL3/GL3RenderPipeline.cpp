#include "GL3RenderPipeline.h"
#include "../../../Device.h"
#include "../../Driver.h"
#include "../../DepthStencilState.h"
#include "../../BlendingState.h"
#include "../../RasterizationState.h"
#include "../../VertexDeclaration.h"
#include "../../GpuParamsDescription.h"
#include "../../GpuParams.h"
#include "../GLShader.h"

namespace sh
{
	namespace video
	{
		GL3RenderPipeline::GL3RenderPipeline()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		GL3RenderPipeline::~GL3RenderPipeline()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		void GL3RenderPipeline::Load(const pugi::xml_node &node)
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		void GL3RenderPipeline::Unload()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		const VertexInputDeclarationPtr& GL3RenderPipeline::GetVertexInputDeclaration() const { return m_vertexDeclaration; }

		///////////////////////////////////////////////////////////////////////////////////

		void GL3RenderPipeline::LoadConstants(const pugi::xml_node &node)
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		void GL3RenderPipeline::LoadShaders(const pugi::xml_node &node)
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		void GL3RenderPipeline::ParseAttributes()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		void GL3RenderPipeline::ParseUniforms()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		GL3RenderPipeline::GL3RenderPipeline(const RenderPipelineDescription& description)
		{
			m_description = description;

			// Create shader program
			m_programID = glCreateProgram();

			GLShader* vertShader = static_cast<GLShader*>(m_description.vertexShader.get());
			GLShader* fragShader = static_cast<GLShader*>(m_description.fragmentShader.get());
			glAttachShader(m_programID, vertShader->GetGLId());
			glAttachShader(m_programID, fragShader->GetGLId());

			glLinkProgram(m_programID);

			GLint linked;
			glGetProgramiv(m_programID, GL_LINK_STATUS, &linked);
			if (!linked)
			{
				GLsizei len;
				glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &len);

				char* log = new char[len + 1];
				glGetProgramInfoLog(m_programID, len, &len, log);

				SH_ASSERT(0, "Error while linking GLES20 shader program! Error message: %s", log);
			}

			ParseAttributes();
			ParseUniforms();
		}

		///////////////////////////////////////////////////////////////////////////////////
	}
}