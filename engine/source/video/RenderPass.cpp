#include "RenderPass.h"
#include "ShaderProgram.h"
#include "UniformBuffer.h"
#include "RenderState.h"
#include "DepthStencilState.h"
#include "RasterizationState.h"
#include "DepthStencilState.h"
#include "../Device.h"

namespace sh
{
	namespace video
	{
		RenderPass::RenderPass()
			: m_layer(Layer::MAIN)
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		RenderPass::~RenderPass()
		{

		}

		RenderPass* RenderPass::Clone()
		{
			RenderPass* result = new RenderPass();
			result->m_shaderProgram = m_shaderProgram;
			result->m_uniformBuffer = m_uniformBuffer->Clone();
			result->m_renderState = m_renderState->Clone();
			result->m_depthStencilState = m_depthStencilState->Clone();
			result->m_blendingState = m_blendingState->Clone();
			result->m_rasterizationState = m_rasterizationState->Clone();
			result->m_vertexInputDeclaration = m_vertexInputDeclaration->Clone();
			result->m_name = m_name;
			result->m_layer = m_layer;
			return result;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPass::Load(const pugi::xml_node &node)
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

			// Load render state
			m_renderState = driver->CreateRenderState();
			pugi::xml_node renderstateNode = node.child("renderstate");
			if (!renderstateNode.empty())
			{				
				m_renderState->Load(renderstateNode);
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
	}
}