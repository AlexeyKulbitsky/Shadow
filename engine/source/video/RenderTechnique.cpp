#include "RenderTechnique.h"
#include "DepthStencilState.h"
#include "BlendingState.h"
#include "RasterizationState.h"
#include "RenderPipeline.h"
#include "Shader.h"
#include "VertexDeclaration.h"
#include "Driver.h"
#include "../Device.h"
#include <pugixml.hpp>

namespace sh
{
	namespace video
	{
		RenderTechnique::RenderTechnique()
		{

		}

		//////////////////////////////////////////////////////////////////////////////////////

		RenderTechnique::~RenderTechnique()
		{

		}

		//////////////////////////////////////////////////////////////////////////////////////

		void RenderTechnique::Load(const String& filePath)
		{
			/*
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filePath.c_str());
			pugi::xml_node techniqueNode = doc.first_child();
			pugi::xml_attribute nameAttr = techniqueNode.attribute("name");
			if (nameAttr)
			{
				m_name = nameAttr.as_string();
			}

			pugi::xml_node renderPipelineNode = techniqueNode.child("pipeline");
			while (renderPipelineNode)
			{		
				RenderPipelinePtr renderPipeline = Device::GetInstance()->GetDriver()->CreateRenderPipeline();
				renderPipeline->Load(renderPipelineNode);

				m_renderPipelines.push_back(renderPipeline);
				
				renderPipelineNode = renderPipelineNode.next_sibling();
			}
			*/

			LoadAlternative(filePath);
		}

		//////////////////////////////////////////////////////////////////////////////////////

		void RenderTechnique::LoadAlternative(const String& filePath)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filePath.c_str());
			pugi::xml_node techniqueNode = doc.first_child();
			pugi::xml_attribute nameAttr = techniqueNode.attribute("name");
			if (nameAttr)
			{
				m_name = nameAttr.as_string();
			}

			// Load pipeline
			pugi::xml_node pipelineNode = techniqueNode.child("pipeline");

			RenderPipelineDescription pipelineDesc;
			pipelineDesc.depthStencilState = LoadDepthStencilState(pipelineNode);
			pipelineDesc.rasterizationState = LoadRasterizationState(pipelineNode);
			pipelineDesc.blendingState = LoadBlendingState(pipelineNode);

			// Load attributes
			pugi::xml_node attributesNode = pipelineNode.child("attributes");
			if (!attributesNode.empty())
			{
				Driver* driver = Device::GetInstance()->GetDriver();
				VertexInputDeclarationPtr vertDeclaration = driver->CreateVertexInputDeclaration();
				vertDeclaration->Load(attributesNode);
				pipelineDesc.vertexDeclaration = vertDeclaration;
			}

			// Load shaders
			pugi::xml_node shadersNode = pipelineNode.child("shader");
			String language = shadersNode.attribute("language").as_string();
			String api = shadersNode.attribute("api").as_string();

			while (api != Device::GetInstance()->GetDriver()->GetApiName())
			{
				shadersNode = shadersNode.next_sibling();
				language = shadersNode.attribute("language").as_string();
				api = shadersNode.attribute("api").as_string();
			}

			io::FileSystem* fs = Device::GetInstance()->GetFileSystem();

			// Load constants
			pugi::xml_node paramsNode = shadersNode.child("constants");
			pipelineDesc.paramsDescription = LoadParamsDescription(paramsNode);


			// Load vertex shader data
			pugi::xml_node vertexShaderSrcNode = shadersNode.child("vertexShader");
			if (vertexShaderSrcNode)
			{
				pugi::xml_node constantsNode = vertexShaderSrcNode.child("constants");
				pugi::xml_node sourceNode = vertexShaderSrcNode.child("source");

				SPtr<GpuParamDescription> paramsDescription;
				
				ShaderDescription shaderDesc;
				shaderDesc.source = sourceNode.child_value();
				shaderDesc.entryPoint = "main";
				shaderDesc.language = language;
				shaderDesc.type = ST_VERTEX;
				shaderDesc.paramsDescription = paramsDescription;
				pipelineDesc.vertexShader = Shader::Create(shaderDesc);
			}

			// Load fragment shader data
			pugi::xml_node fragmentShaderSrcNode = shadersNode.child("fragmentShader");
			if (fragmentShaderSrcNode)
			{
				pugi::xml_node constantsNode = fragmentShaderSrcNode.child("constants");
				pugi::xml_node sourceNode = fragmentShaderSrcNode.child("source");

				SPtr<GpuParamDescription> paramsDescription;

				ShaderDescription shaderDesc;
				shaderDesc.source = sourceNode.child_value();
				shaderDesc.entryPoint = "main";
				shaderDesc.language = language;
				shaderDesc.type = ST_FRAGMENT;
				shaderDesc.paramsDescription = paramsDescription;
				pipelineDesc.fragmentShader = Shader::Create(shaderDesc);
			}

			

			m_renderPipelines.push_back(RenderPipeline::Create(pipelineDesc));
		}

		//////////////////////////////////////////////////////////////////////////////////////

		void RenderTechnique::Unload()
		{
			for (size_t i = 0; i < m_renderPipelines.size(); ++i)
			{
				m_renderPipelines[i]->Unload();
			}
			m_renderPipelines.clear();
		}

		//////////////////////////////////////////////////////////////////////////////////////

		DepthStencilStatePtr RenderTechnique::LoadDepthStencilState(const pugi::xml_node& node)
		{
			DepthStencilStatePtr state(new DepthStencilState());
			pugi::xml_node stateNode;
			pugi::xml_attribute valAttr;

			// Depth test			
			stateNode = node.child("depthTest");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					bool value = valAttr.as_bool();
					state->enableDepthTest = value;
				}
			}

			// Stencil test
			stateNode = node.child("stencilTest");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					bool value = valAttr.as_bool();
					state->enableStencilTest = value;
				}
			}

			return state;
		}

		//////////////////////////////////////////////////////////////////////////////////////

		RasterizationStatePtr RenderTechnique::LoadRasterizationState(const pugi::xml_node& node)
		{
			RasterizationStatePtr state(new RasterizationState());

			pugi::xml_node stateNode;
			pugi::xml_attribute valAttr;

			// Front Face
			stateNode = node.child("frontFace");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();
					if (value == "clockWise")
					{
						state->frontFace = FF_CLOCKWISE;
					}
					else if (value == "counterClockWise")
					{
						state->frontFace = FF_COUNTER_CLOCKWISE;
					}
				}
			}

			// Cull face
			stateNode = node.child("cullFace");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();
					if (value == "front")
					{
						state->cullFace = CF_FRONT;
					}
					else if (value == "back")
					{
						state->cullFace = CF_BACK;
					}
					else if (value == "none")
					{
						state->cullFace = CF_NONE;
					}
				}
			}

			// Point size
			stateNode = node.child("pointSize");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					state->pointSize = valAttr.as_float();
				}
			}

			// Line width
			stateNode = node.child("lineWidth");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					state->lineWidth = valAttr.as_float();
				}
			}

			return state;
		}

		//////////////////////////////////////////////////////////////////////////////////////

		BlendingStatePtr RenderTechnique::LoadBlendingState(const pugi::xml_node& node)
		{
			BlendingStatePtr state(new BlendingState());

			pugi::xml_node stateNode;
			pugi::xml_attribute valAttr;
					
			stateNode = node.child("srcFactor");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();

					for (size_t i = 0U; i < BF_COUNT; ++i)
					{
						if (value == g_blendFactorMap[i])
						{
							state->srcAlpha = static_cast<BlendFactor>(i);
							break;
						}
					}
				}
			}

			stateNode = node.child("dstFactor");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();

					for (size_t i = 0U; i < BF_COUNT; ++i)
					{
						if (value == g_blendFactorMap[i])
						{
							state->dstAlpha = static_cast<BlendFactor>(i);
							break;
						}
					}
				}
			}

			stateNode = node.child("operation");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();

					for (size_t i = 0U; i < BF_COUNT; ++i)
					{
						if (value == g_blendOperationMap[i])
						{
							state->operationAlpha = static_cast<BlendOperation>(i);
							break;
						}
					}
				}
			}

			return state;
		}

		//////////////////////////////////////////////////////////////////////////////////////

		GpuParamsDescription RenderTechnique::LoadParamsDescription(const pugi::xml_node& node)
		{
			GpuParamsDescription paramsDesc;

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
					paramsDesc.params[desc.name] = desc;
				}

				childNode = childNode.next_sibling();
			}

			return paramsDesc;
		}

		//////////////////////////////////////////////////////////////////////////////////////
	}
}