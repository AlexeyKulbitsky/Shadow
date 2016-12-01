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
		}		

		///////////////////////////////////////////////////////////////////////////////////////////////////

		void RenderPass::LoadRenderState(const pugi::xml_node &node)
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////////////////
	}
}