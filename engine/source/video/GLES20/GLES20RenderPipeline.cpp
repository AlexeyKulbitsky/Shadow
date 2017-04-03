#include "GLES20RenderPIpeline.h"
#include "../../Device.h"
#include "../Driver.h"
#include "../UniformBuffer.h"
#include "../DepthStencilState.h"
#include "../BlendingState.h"
#include "../RasterizationState.h"
#include "../VertexDeclaration.h"
#include "../ShaderProgram.h"
#include "../GpuParamDescription.h"
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
			RenderPipeline::LoadStates(node);

			Driver* driver = Device::GetInstance()->GetDriver();

			// Read name
			pugi::xml_attribute nameAttr = node.attribute("name");
			if (nameAttr)
			{
				m_name = nameAttr.as_string();
			}

			// Read render layer
			pugi::xml_attribute layerAttr = node.attribute("layer");
			if (layerAttr)
			{
				String layerName = layerAttr.as_string();
				if (layerName == "main")
				{
					m_layer = Layer::MAIN;
				}
				else if (layerName == "background")
				{
					m_layer = Layer::BACKGROUND;
				}
			}

			LoadStates(node);

			// Load shader program
			pugi::xml_node shaderProgramNode = node.child("shader");
			if (!shaderProgramNode.empty())
			{
				m_shaderProgram = driver->CreateShaderProgram();
				m_shaderProgram->Load(shaderProgramNode);
			}

			LoadShaders(shaderProgramNode);

			// Load attributes
			pugi::xml_node attributesNode = node.child("attributes");
			if (!attributesNode.empty())
			{
				m_vertexInputDeclaration = driver->CreateVertexInputDeclaration();
				m_vertexInputDeclaration->SetShaderProgram(m_shaderProgram.get());
				m_vertexInputDeclaration->Load(attributesNode);
				m_vertexInputDeclaration->Init();
			}

			// Load uniforms
			pugi::xml_node uniformsNode = node.child("constants");
			if (!uniformsNode.empty())
			{
				m_uniformBuffer = driver->CreateUniformBuffer();
				m_uniformBuffer->SetShaderProgram(m_shaderProgram.get());
				m_uniformBuffer->Load(uniformsNode);
				m_uniformBuffer->Init();
			}

			LoadConstants(uniformsNode);

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

				SH_ASSERT(0, "ERROR");
			}


			for (auto& param : m_paramsDesc.params)
			{
				GLint location = glGetUniformLocation(m_programID, param.second.name.c_str());
				if (location != -1)
				{
					param.second.location = location;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////////
		
		void GLES20RenderPipeline::Unload()
		{
			m_uniformBuffer->Unload();
		}

		///////////////////////////////////////////////////////////////////////////////////

		const ShaderProgramPtr& GLES20RenderPipeline::GetShaderProgram() const { return m_shaderProgram; }

		///////////////////////////////////////////////////////////////////////////////////

		const VertexInputDeclarationPtr& GLES20RenderPipeline::GetVertexInputDeclaration() const { return m_description.vertexDeclaration; }
		
		///////////////////////////////////////////////////////////////////////////////////
		
		const UniformBufferPtr& GLES20RenderPipeline::GetUniformBuffer() const { return m_uniformBuffer; }
		
		///////////////////////////////////////////////////////////////////////////////////

		const UniformBufferPtr& GLES20RenderPipeline::GetTransformUniformBuffer() const { return m_transformUniformBuffer; }
		
		///////////////////////////////////////////////////////////////////////////////////



		void GLES20RenderPipeline::LoadConstants(const pugi::xml_node &node)
		{
			pugi::xml_node childNode = node.first_child();

			while (childNode)
			{
				String name = childNode.name();

				if (name == "constant")
				{
					GpuParamDataDescription desc;

					// Read type
					pugi::xml_attribute typeAttr = childNode.attribute("type");
					if (typeAttr)
					{
						String typeStr = typeAttr.as_string();
						if (typeStr == "float")
						{
							desc.type = GPDT_FLOAT1;
							desc.size = sizeof(float);
						}
						else if (typeStr == "vec2")
						{
							desc.type = GPDT_FLOAT2;
							desc.size = sizeof(math::Vector2f);
						}
						else if (typeStr == "vec3")
						{
							desc.type = GPDT_FLOAT3;
							desc.size = sizeof(math::Vector3f);
						}
						else if (typeStr == "mat4")
						{
							desc.type = GPDT_MATRIX4;
							desc.size = sizeof(math::Matrix4f);
						}
						
					}

					// Read name
					pugi::xml_attribute nameAttr = childNode.attribute("name");
					if (nameAttr)
					{
						String constantName = nameAttr.as_string();
						desc.name = constantName;
					}

					// Add param to map
					m_paramsDesc.params[desc.name] = desc;
				}

				childNode = childNode.next_sibling();
			}
		}

		///////////////////////////////////////////////////////////////////////////////////

		void GLES20RenderPipeline::LoadShaders(const pugi::xml_node &node)
		{
			// Load vertex shader data
			pugi::xml_node vertexShaderSrcNode = node.child("vertexShader");
			if (vertexShaderSrcNode)
			{
				const char* vertexShaderSource = vertexShaderSrcNode.child_value();
				ShaderDescription desc;
				desc.language = "glsl";
				desc.entryPoint = "main";
				desc.source = vertexShaderSource;
				desc.type = ST_VERTEX;
				ShaderPtr shader = Shader::Create(desc);
				m_description.vertexShader = shader;
			}

			// Load fragment shader data
			pugi::xml_node fragmentShaderSrcNode = node.child("fragmentShader");
			if (fragmentShaderSrcNode)
			{
				const char* fragmentShaderSource = fragmentShaderSrcNode.child_value();
				ShaderDescription desc;
				desc.language = "glsl";
				desc.entryPoint = "main";
				desc.source = fragmentShaderSource;
				desc.type = ST_FRAGMENT;
				ShaderPtr shader = Shader::Create(desc);
				m_description.fragmentShader = shader;
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

				SH_ASSERT(0, "ERROR");
			}

			for (auto& param : m_description.paramsDescription.params)
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