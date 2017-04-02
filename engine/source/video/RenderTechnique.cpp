#include "RenderTechnique.h"
#include "DepthStencilState.h"
#include "BlendingState.h"
#include "RasterizationState.h"
#include "RenderPipeline.h"
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
				//RenderPipelinePtr renderPipeline = RenderPipelinePtr(new RenderPipeline());
				renderPipeline->Load(renderPipelineNode);

				m_renderPipelines.push_back(renderPipeline);
				
				renderPipelineNode = renderPipelineNode.next_sibling();
			}
			
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

			RenderPipelineDescription pipelineDesc;
			pipelineDesc.depthStencilState = LoadDepthStencilState(techniqueNode);
			pipelineDesc.rasterizationState = LoadRasterizationState(techniqueNode);
			pipelineDesc.blendingState = LoadBlendingState(techniqueNode);

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
	}
}