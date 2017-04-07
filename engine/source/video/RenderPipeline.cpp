#include "RenderPipeline.h"
#include "DepthStencilState.h"
#include "RasterizationState.h"
#include "BlendingState.h"
#include "../Device.h"
#include "Driver.h"

#include "Managers/RenderStateManager.h"

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
#if 0
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
			
			// Load depth/stencil state
			m_depthStencilState.reset(new DepthStencilState());
			pugi::xml_node depthStencilStateNode = node.child("depthstencilstate");
			if (!depthStencilStateNode.empty())
			{
				m_depthStencilState->Load(depthStencilStateNode);
			}

			// Load rasterization state
			m_rasterizationState.reset(new RasterizationState());
			pugi::xml_node rasterizationStateNode = node.child("rasterizationstate");
			if (!rasterizationStateNode.empty())
			{
				m_rasterizationState->Load(rasterizationStateNode);
			}

			// Load blending state
			m_blendingState.reset(new BlendingState());
			pugi::xml_node blendingStateNode = node.child("blendingstate");
			if (!blendingStateNode.empty())
			{			
				m_blendingState->Load(blendingStateNode);
				m_blendingState->enabled = true;
			}

			// Load shader program
			pugi::xml_node shaderProgramNode = node.child("shader");
			if (!shaderProgramNode.empty())
			{
				m_shaderProgram = driver->CreateShaderProgram();
				m_shaderProgram->Load(shaderProgramNode);
			}			

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
#endif
		}		

		///////////////////////////////////////////////////////////////////////////////////////////////////
	
		void RenderPipeline::Unload()
		{
			/*
			m_shaderProgram->Unload();

			delete m_vertexInputDeclaration;
			m_vertexInputDeclaration = nullptr;

			m_uniformBuffer->Unload();
			*/
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
			//Driver* driver = Device::GetInstance()->GetDriver();
			//return driver->CreateRenderPipeline(description);

			return RenderStateManager::GetInstance()->CreateRenderPipeline(description);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////
	}
}