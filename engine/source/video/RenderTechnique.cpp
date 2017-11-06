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
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(filePath.c_str());

			LoadInternal(doc);
		}

		//////////////////////////////////////////////////////////////////////////////////////

		void RenderTechnique::Load( const std::vector<char>& data )
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_buffer(data.data(), data.size());

			LoadInternal(doc);
		}

		//////////////////////////////////////////////////////////////////////////////////////

		void RenderTechnique::LoadInternal(const pugi::xml_document& doc)
		{
			pugi::xml_node techniqueNode = doc.first_child();
			pugi::xml_attribute nameAttr = techniqueNode.attribute("name");
			if (nameAttr)
			{
				m_name = nameAttr.as_string();
			}
			
			// Load pipeline
			pugi::xml_node pipelineNode = techniqueNode.child("pipeline");

			RenderPipelineDescription pipelineDesc;
			pugi::xml_attribute layerAttr = pipelineNode.attribute("layer");
			if (layerAttr)
			{
				String layername = layerAttr.as_string();
				if (layername == "main")
				{
					pipelineDesc.layer = RenderLayer::Main;
				}
				else if (layername == "background")
				{
					pipelineDesc.layer = RenderLayer::Background;
				}
			}

			pipelineDesc.depthStencilState = LoadDepthStencilState(pipelineNode);
			pipelineDesc.rasterizationState = LoadRasterizationState(pipelineNode);
			pipelineDesc.blendingState = LoadBlendingState(pipelineNode);

			// Load shaders
			pugi::xml_node shadersNode = pipelineNode.child("shader");
			String language = shadersNode.attribute("language").as_string();
			String api = shadersNode.attribute("api").as_string();

			bool foundShaderForCurrentRenderAPI = false;

			while (shadersNode)
			{
				if (api == Device::GetInstance()->GetDriver()->GetApiName())
				{
					foundShaderForCurrentRenderAPI = true;
					break;
				}
				shadersNode = shadersNode.next_sibling("shader");
				language = shadersNode.attribute("language").as_string();
				api = shadersNode.attribute("api").as_string();
			}

			SH_ASSERT(foundShaderForCurrentRenderAPI, "Can not find %s shader for %s API", 
				m_name.c_str(), Device::GetInstance()->GetDriver()->GetApiName().c_str());

			io::FileSystem* fs = Device::GetInstance()->GetFileSystem();

			// Load vertex shader data
			pugi::xml_node vertexShaderSrcNode = shadersNode.child("vertexShader");
			if (vertexShaderSrcNode)
			{
				pugi::xml_node constantsNode = vertexShaderSrcNode.child("constants");
				pugi::xml_node sourceNode = vertexShaderSrcNode.child("source");
				
				ShaderDescription shaderDesc;
				shaderDesc.source = sourceNode.child_value();
				shaderDesc.entryPoint = "main";
				shaderDesc.language = language;
				shaderDesc.type = ST_VERTEX;
				pipelineDesc.vertexShader = Shader::Create(shaderDesc);
			}

			// Load fragment shader data
			pugi::xml_node fragmentShaderSrcNode = shadersNode.child("fragmentShader");
			if (fragmentShaderSrcNode)
			{
				pugi::xml_node constantsNode = fragmentShaderSrcNode.child("constants");
				SPtr<GpuParamsDescription> paramsDescription;
				SPtr<GpuParamsDescription> autoParamsDescription;
				pugi::xml_node sourceNode = fragmentShaderSrcNode.child("source");

				ShaderDescription shaderDesc;
				shaderDesc.source = sourceNode.child_value();
				shaderDesc.entryPoint = "main";
				shaderDesc.language = language;
				shaderDesc.type = ST_FRAGMENT;
				pipelineDesc.fragmentShader = Shader::Create(shaderDesc);
			}

			m_renderPipelines.push_back(RenderPipeline::Create(pipelineDesc));

			MaterialParamsDescription paramsDescription = LoadParams(pipelineNode);
			m_paramDescriptions.push_back(paramsDescription);
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
			pugi::xml_node parent = node.child("depthStencilState");
			if(!parent)
				return state;

			// Depth test			
			stateNode = parent.child("depthTest");
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
			stateNode = parent.child("stencilTest");
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
			pugi::xml_node parent = node.child("rasterizationState");
			if(!parent)
				return state;

			// Front Face
			stateNode = parent.child("frontFace");
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
			stateNode = parent.child("cullFace");
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

			// Fill mode
			stateNode = parent.child("fillMode");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					String value = valAttr.as_string();
					if (value == "solid")
					{
						state->fillMode = FM_SOLID;
					}
					else if (value == "wireframe")
					{
						state->fillMode = FM_WIREFRAME;
					}
					else if (value == "ponts")
					{
					}
				}
			}

			// Point size
			stateNode = parent.child("pointSize");
			if (!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if (valAttr)
				{
					state->pointSize = valAttr.as_float();
				}
			}

			// Line width
			stateNode = parent.child("lineWidth");
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
			pugi::xml_node parent = node.child("blendingState");
			if(!parent)
				return state;

			stateNode = parent.child("enabled");
			if(!stateNode.empty())
			{
				valAttr = stateNode.attribute("val");
				if( valAttr )
				{
					bool value = valAttr.as_bool();
					state->enabled = value;
				}
			}
					
			stateNode = parent.child("srcFactor");
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
							state->srcColor = static_cast<BlendFactor>(i);
							break;
						}
					}
				}
			}

			stateNode = parent.child("dstFactor");
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
							state->dstColor = static_cast<BlendFactor>(i);
							break;
						}
					}
				}
			}

			stateNode = parent.child("operation");
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
							state->operationColor = static_cast<BlendOperation>(i);
							break;
						}
					}
				}
			}

			return state;
		}

		//////////////////////////////////////////////////////////////////////////////////////

		MaterialParamsDescription RenderTechnique::LoadParams(const pugi::xml_node& node)
		{
			MaterialParamsDescription paramsDescription;

			pugi::xml_node paramsNode = node.child("params");
			if(!paramsNode)
				return paramsDescription;

			pugi::xml_node paramNode = paramsNode.child("param");
			pugi::xml_attribute attribute;
			while (paramNode)
			{
				MaterialParamDescription paramDesc;

				// name
				attribute = paramNode.attribute("name");
				if (attribute)
				{
					paramDesc.name = attribute.as_string();
				}
				// description
				attribute = paramNode.attribute("desc");
				if (attribute)
				{
					paramDesc.description = attribute.as_string();
				}
				// type
				attribute = paramNode.attribute("type");
				{
					auto it = materialParamTypeMap.find(attribute.as_string());
					if (it != materialParamTypeMap.end())
						paramDesc.type = it->second;
					else
						SH_ASSERT(0, "Can not determine Material Parameter Type for %s", paramDesc.name.c_str());
				}

				paramsDescription.params[paramDesc.name] = paramDesc;

				paramNode = paramNode.next_sibling();
			}

			return paramsDescription;
		}

		//////////////////////////////////////////////////////////////////////////////////////
	}
}