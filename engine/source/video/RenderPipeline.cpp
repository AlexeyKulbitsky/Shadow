#include "RenderPipeline.h"
#include "DepthStencilState.h"
#include "RasterizationState.h"
#include "BlendingState.h"
#include "../Device.h"
#include "Driver.h"

#include "Managers/RenderStateManager.h"

#include <pugixml.hpp>

namespace sh
{
	namespace video
	{
		RenderPipeline::RenderPipeline()
			: m_layer(Layer::MAIN)
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		RenderPipeline::~RenderPipeline()
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		/*
		RenderPipelinePtr RenderPipeline::Clone()
		{
		RenderPipelinePtr result(new RenderPipeline());
		result->m_shaderProgram = m_shaderProgram;
		result->m_uniformBuffer = UniformBufferPtr(m_uniformBuffer->Clone());
		result->m_depthStencilState = DepthStencilStatePtr(m_depthStencilState->Clone());
		result->m_blendingState = BlendingStatePtr(m_blendingState->Clone());
		result->m_rasterizationState = RasterizationStatePtr(m_rasterizationState->Clone());
		result->m_vertexInputDeclaration = m_vertexInputDeclaration->Clone();
		result->m_name = m_name;
		result->m_layer = m_layer;
		return result;
		}
		*/

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPipeline::Load(const pugi::xml_node &node)
		{
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPipeline::Unload()
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPipeline::LoadStates(const pugi::xml_node& node)
		{
			// Load depth/stencil state
			m_description.depthStencilState.reset(new DepthStencilState());
			pugi::xml_node depthStencilStateNode = node.child("depthstencilstate");
			if (!depthStencilStateNode.empty())
			{
				m_description.depthStencilState->Load(depthStencilStateNode);
			}

			// Load rasterization state
			m_description.rasterizationState.reset(new RasterizationState());
			pugi::xml_node rasterizationStateNode = node.child("rasterizationstate");
			if (!rasterizationStateNode.empty())
			{
				m_description.rasterizationState->Load(rasterizationStateNode);
			}

			// Load blending state
			m_description.blendingState.reset(new BlendingState());
			pugi::xml_node blendingStateNode = node.child("blendingstate");
			if (!blendingStateNode.empty())
			{
				m_description.blendingState->Load(blendingStateNode);
				m_description.blendingState->enabled = true;
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		RenderPipelinePtr RenderPipeline::Create(const RenderPipelineDescription& description)
		{
			RenderPipelinePtr result = RenderStateManager::GetInstance()->CreateRenderPipeline(description);
			result->m_renderLayer = description.layer;

			return result;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////
	}
}