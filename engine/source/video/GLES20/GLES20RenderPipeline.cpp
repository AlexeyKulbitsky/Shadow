#include "GLES20RenderPIpeline.h"
#include "../../Device.h"
#include "../Driver.h"
#include "../DepthStencilState.h"
#include "../BlendingState.h"
#include "../RasterizationState.h"
#include "../VertexDeclaration.h"
#include "../GpuParamsDescription.h"
#include "../GpuParams.h"
#include "GLES20Shader.h"
#include "GLES20VertexDeclaration.h"

namespace sh
{
	namespace video
	{
		GLES20RenderPipeline::GLES20RenderPipeline()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		GLES20RenderPipeline::~GLES20RenderPipeline()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		void GLES20RenderPipeline::Load(const pugi::xml_node &node)
		{
		}

		///////////////////////////////////////////////////////////////////////////////////
		
		void GLES20RenderPipeline::Unload()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		const VertexInputDeclarationPtr& GLES20RenderPipeline::GetVertexInputDeclaration() const { return m_description.vertexDeclaration; }
		
		///////////////////////////////////////////////////////////////////////////////////

		void GLES20RenderPipeline::LoadConstants(const pugi::xml_node &node)
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		void GLES20RenderPipeline::LoadShaders(const pugi::xml_node &node)
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		GLES20RenderPipeline::GLES20RenderPipeline(const RenderPipelineDescription& description)
		{
			m_description = description;

			// Create shader program
			m_programID = glCreateProgram();

			GLES20Shader* vertShader = static_cast<GLES20Shader*>(m_description.vertexShader.get());
			GLES20Shader* fragShader = static_cast<GLES20Shader*>(m_description.fragmentShader.get());
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

			// For GLES20 there are only two type of shaders : vertex and fragment
			// Vertex shader
			auto vertexParams = m_description.vertexShader->GetParamsDescription();
			for (auto& param : vertexParams->params)
			{
				GLint location = glGetUniformLocation(m_programID, param.second.name.c_str());
				if (location != -1)
				{
					param.second.location = location;
				}
			}
			for (auto& sampler : vertexParams->samplers)
			{
				GLint location = glGetUniformLocation(m_programID, sampler.second.name.c_str());
				if (location != -1)
				{
					sampler.second.location = location;
				}
			}
			auto autoVertexParams = m_description.vertexShader->GetAutoParamsDescription();
			for (auto& param : autoVertexParams->params)
			{
				GLint location = glGetUniformLocation(m_programID, param.second.name.c_str());
				if (location != -1)
				{
					param.second.location = location;
				}
			}

			// Fragment shader
			auto fragmentParams = m_description.fragmentShader->GetParamsDescription();
			for (auto& param : fragmentParams->params)
			{
				GLint location = glGetUniformLocation(m_programID, param.second.name.c_str());
				if (location != -1)
				{
					param.second.location = location;
				}
			}
			for (auto& sampler : fragmentParams->samplers)
			{
				GLint location = glGetUniformLocation(m_programID, sampler.second.name.c_str());
				if (location != -1)
				{
					sampler.second.location = location;
				}
			}
			auto autoFragmentParams = m_description.fragmentShader->GetAutoParamsDescription();
			for (auto& param : autoFragmentParams->params)
			{
				GLint location = glGetUniformLocation(m_programID, param.second.name.c_str());
				if (location != -1)
				{
					param.second.location = location;
				}
			}

			// Init vertex attributes
			GLES20VertexDeclaration* glDeclaration = static_cast<GLES20VertexDeclaration*>(m_description.vertexDeclaration.get());
			auto& attributes = glDeclaration->GetAttributes();
			for (u32 i = 0; i < attributes.size(); ++i)
			{
				attributes[i].index = glGetAttribLocation(m_programID, attributes[i].name.c_str());
			}

		}

		///////////////////////////////////////////////////////////////////////////////////
	}
}