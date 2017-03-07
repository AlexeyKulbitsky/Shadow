#include "VulkanRenderPipeline.h"
#include "VulkanDeleter.h"
#include "../../Device.h"
#include "../Driver.h"
#include "../UniformBuffer.h"
#include "../DepthStencilState.h"
#include "../BlendingState.h"
#include "../RasterizationState.h"
#include "../VertexDeclaration.h"
#include "../ShaderProgram.h"


namespace sh
{
	namespace video	
	{
		VulkanRenderPipeline::VulkanRenderPipeline()
		{

		}

		///////////////////////////////////////////////////////////////////////////////////////

		VulkanRenderPipeline::~VulkanRenderPipeline()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////////

		RenderPipelinePtr VulkanRenderPipeline::Clone()
		{
			RenderPipelinePtr result;

			VulkanRenderPipeline* pipeline = new VulkanRenderPipeline();
			pipeline->m_shaderProgram = m_shaderProgram;
			pipeline->m_uniformBuffer = UniformBufferPtr(m_uniformBuffer->Clone());
			pipeline->m_depthStencilState = DepthStencilStatePtr(m_depthStencilState->Clone());
			pipeline->m_blendingState = BlendingStatePtr(m_blendingState->Clone());
			pipeline->m_rasterizationState = RasterizationStatePtr(m_rasterizationState->Clone());
			pipeline->m_vertexInputDeclaration = m_vertexInputDeclaration->Clone();
			pipeline->m_name = m_name;
			pipeline->m_layer = m_layer;

			result.reset(pipeline);
			return result;
		}

		///////////////////////////////////////////////////////////////////////////////////////

		void VulkanRenderPipeline::Load(const pugi::xml_node &node)
		{
			RenderPipeline::LoadStates(node);

			Driver* driver = Device::GetInstance()->GetDriver();


			// Load shader program
			pugi::xml_node shaderProgramNode = node.child("shader");

			while (shaderProgramNode.attribute("api").as_string() != String("Vulkan"))
			{
				shaderProgramNode = shaderProgramNode.next_sibling();
			}
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
		}

		///////////////////////////////////////////////////////////////////////////////////////

		void VulkanRenderPipeline::Unload()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////////
	}
}