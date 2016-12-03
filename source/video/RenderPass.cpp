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
			result->m_shaderProgram = m_shaderProgram->Clone();
			//result->m_renderState = m_renderState->Clone();
			return result;
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPass::Load(const pugi::xml_node &node)
		{
			// Load render state
			pugi::xml_node renderstateNode = node.child("renderstate");
			if (!renderstateNode.empty())
			{
				LoadRenderState(renderstateNode);
			}

			// Load shader program
			pugi::xml_node shaderProgramNode = node.child("shader");
			if (!shaderProgramNode.empty())
			{
				m_shaderProgram = Device::GetInstance()->GetDriver()->CreateShaderProgram();
				m_shaderProgram->Load(shaderProgramNode);
			}			

			// Load attributes
			pugi::xml_node attributesNode = node.child("attributes");
			LoadAttributes(attributesNode);
		}		

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPass::LoadRenderState(const pugi::xml_node &node)
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPass::LoadUniforms(const pugi::xml_node &node)
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPass::LoadAttributes(const pugi::xml_node &node)
		{
			m_vertexInputDeclaration = Device::GetInstance()->GetDriver()->CreateVertexInputDeclaration();
			m_vertexInputDeclaration->Load(node);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPass::LoadSamplers(const pugi::xml_node &node)
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////
	}
}