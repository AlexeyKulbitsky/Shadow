#include "GLES20RenderPipeline.h"
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

		void GLES20RenderPipeline::ParseAttributes()
		{
			const u32 bufferSize = 256;
			char buffer[bufferSize];

			SPtr<GLES20VertexDeclaration> vertexDeclaration(new GLES20VertexDeclaration());

			s32 attributesCount = 0;
			glGetProgramiv(m_programID, GL_ACTIVE_ATTRIBUTES, &attributesCount);
			for (s32 i = 0; i < attributesCount; ++i)
			{
				GLsizei readedBytes = 0;
				s32 attributeSize = 0;
				GLenum attributeType;
				glGetActiveAttrib(m_programID, i, bufferSize, &readedBytes, &attributeSize, &attributeType, buffer);

				GLES20VertexAttribute attribute;
				attribute.index = i;
				attribute.name = buffer;
				attribute.type = GL_FLOAT;
				attribute.semantic = AttributeSemantic::UNDEFINED;

				switch (attributeType)
				{
				case GL_FLOAT:
					attribute.size = 1;
					break;
				case GL_FLOAT_VEC2:
					attribute.size = 2;
					break;
				case GL_FLOAT_VEC3:
					attribute.size = 3;
					break;
				case GL_FLOAT_VEC4:
					attribute.size = 4;
					break;
				case GL_FLOAT_MAT2:
					attribute.size = 4;
					break;
				case GL_FLOAT_MAT3:
					attribute.size = 9;
					break;
				case GL_FLOAT_MAT4:
					attribute.size = 16;
					break;
				default:
					break;
				}

				auto it = attributeSemanticMap.find(buffer);
				if (it != attributeSemanticMap.end())
				{
					attribute.semantic = it->second;
				}

				vertexDeclaration->AddAttribute(attribute);
			}
			m_description.vertexDeclaration = vertexDeclaration;
		}

		///////////////////////////////////////////////////////////////////////////////////

		void GLES20RenderPipeline::ParseUniforms()
		{
			const u32 bufferSize = 256;
			char buffer[bufferSize];

			// [Test] Reading uniforms and locations directly from shader
			s32 uniformsCount = 0;
			glGetProgramiv(m_programID, GL_ACTIVE_UNIFORMS, &uniformsCount);
			for (s32 i = 0; i < uniformsCount; ++i)
			{
				GLsizei readedBytes = 0;
				s32 uniformSize = 0;
				GLenum uniformType;
				glGetActiveUniform(m_programID, i, bufferSize, &readedBytes, &uniformSize, &uniformType, buffer);
				switch (uniformType)
				{
				case GL_FLOAT:
					break;
				case GL_FLOAT_VEC2:
					break;
				case GL_FLOAT_VEC3:
					break;
				case GL_FLOAT_VEC4:
					break;
				case GL_INT:
					break;
				case GL_INT_VEC2:
					break;
				case GL_INT_VEC3:
					break;
				case GL_INT_VEC4:
					break;
				case GL_BOOL:
					break;
				case GL_BOOL_VEC2:
					break;
				case GL_BOOL_VEC3:
					break;
				case GL_BOOL_VEC4:
					break;
				case GL_FLOAT_MAT2:
					break;
				case GL_FLOAT_MAT3:
					break;
				case GL_FLOAT_MAT4:
					break;
				case GL_SAMPLER_2D:
					break;
				case GL_SAMPLER_CUBE:
					break;
				default:
					break;
				}
			}
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

			ParseAttributes();
		}

		///////////////////////////////////////////////////////////////////////////////////
	}
}