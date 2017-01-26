#include "RenderPipeline.h"
#include "ShaderProgram.h"
#include "UniformBuffer.h"
#include "DepthStencilState.h"
#include "RasterizationState.h"
#include "BlendingState.h"
#include "../Device.h"

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

		RenderPipeline* RenderPipeline::Clone()
		{
			RenderPipeline* result = new RenderPipeline();
			result->m_shaderProgram = m_shaderProgram;
			result->m_uniformBuffer = m_uniformBuffer->Clone();
			result->m_depthStencilState = m_depthStencilState->Clone();
			result->m_blendingState = m_blendingState->Clone();
			result->m_rasterizationState = m_rasterizationState->Clone();
			result->m_vertexInputDeclaration = m_vertexInputDeclaration->Clone();
			result->m_name = m_name;
			result->m_layer = m_layer;
			return result;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPipeline::Load(const pugi::xml_node &node)
		{
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
			m_depthStencilState = new DepthStencilState();
			pugi::xml_node depthStencilStateNode = node.child("depthstencilstate");
			if (!depthStencilStateNode.empty())
			{
				m_depthStencilState->Load(depthStencilStateNode);
			}

			// Load rasterization state
			m_rasterizationState = new RasterizationState();
			pugi::xml_node rasterizationStateNode = node.child("rasterizationstate");
			if (!rasterizationStateNode.empty())
			{
				m_rasterizationState->Load(rasterizationStateNode);
			}

			// Load blending state
			m_blendingState = new BlendingState();
			pugi::xml_node blendingStateNode = node.child("blendingstate");
			if (!blendingStateNode.empty())
			{
				m_blendingState->Load(blendingStateNode);
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
				m_vertexInputDeclaration->SetShaderProgram(m_shaderProgram);
				m_vertexInputDeclaration->Load(attributesNode);
				m_vertexInputDeclaration->Init();
			}

			// Load uniforms
			pugi::xml_node uniformsNode = node.child("constants");
			if (!uniformsNode.empty())
			{
				m_uniformBuffer = driver->CreateUniformBuffer();
				m_uniformBuffer->SetShaderProgram(m_shaderProgram);
				m_uniformBuffer->Load(uniformsNode);
				m_uniformBuffer->Init();
			}
		}		

		///////////////////////////////////////////////////////////////////////////////////////////////////
	
		void RenderPipeline::Unload()
		{
			m_shaderProgram->Unload();
			delete m_shaderProgram;
			m_shaderProgram = nullptr;

			delete m_vertexInputDeclaration;
			m_vertexInputDeclaration = nullptr;

			m_uniformBuffer->Unload();
			delete m_uniformBuffer;
			m_uniformBuffer = nullptr;

			delete m_depthStencilState;
			m_depthStencilState = nullptr;
			delete m_rasterizationState;
			m_rasterizationState = nullptr;
			delete m_blendingState;
			m_blendingState = nullptr;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////
	}
}