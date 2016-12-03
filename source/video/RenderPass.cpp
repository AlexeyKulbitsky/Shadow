#include "RenderPass.h"
#include "ShaderProgram.h"
#include "UniformBuffer.h"
#include "RenderState.h"
#include "../Device.h"

namespace sh
{
	namespace video
	{
		RenderPass::RenderPass()
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
			result->m_vertexInputDeclaration = m_vertexInputDeclaration->Clone();
			return result;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPass::Load(const pugi::xml_node &node)
		{
			Driver* driver = Device::GetInstance()->GetDriver();

			// Load render state
			pugi::xml_node renderstateNode = node.child("renderstate");
			if (!renderstateNode.empty())
			{
				m_renderState = driver->CreateRenderState();
				m_renderState->Load(renderstateNode);
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

		void RenderPass::LoadRenderState(const pugi::xml_node &node)
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////
	}
}