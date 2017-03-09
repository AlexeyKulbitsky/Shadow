#include "GLES20RenderPIpeline.h"
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
		GLES20RenderPipeline::GLES20RenderPipeline()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		GLES20RenderPipeline::~GLES20RenderPipeline()
		{
		}

		///////////////////////////////////////////////////////////////////////////////////

		void GLES20RenderPipeline::Load(const pugi::xml_node &node)
		{
			RenderPipeline::LoadStates(node);

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
		}

		///////////////////////////////////////////////////////////////////////////////////

		RenderPipelinePtr GLES20RenderPipeline::Clone()
		{
			RenderPipelinePtr result;

			GLES20RenderPipeline* pipeline = new GLES20RenderPipeline();
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

		///////////////////////////////////////////////////////////////////////////////////
		
		void GLES20RenderPipeline::Unload()
		{
			delete m_vertexInputDeclaration;
			m_vertexInputDeclaration = nullptr;

			m_uniformBuffer->Unload();
		}

		///////////////////////////////////////////////////////////////////////////////////

		const ShaderProgramPtr& GLES20RenderPipeline::GetShaderProgram() const { return m_shaderProgram; }

		///////////////////////////////////////////////////////////////////////////////////

		VertexInputDeclaration* GLES20RenderPipeline::GetVertexInputDeclaration() const { return m_vertexInputDeclaration; }
		
		///////////////////////////////////////////////////////////////////////////////////
		
		const UniformBufferPtr& GLES20RenderPipeline::GetUniformBuffer() const { return m_uniformBuffer; }
		
		///////////////////////////////////////////////////////////////////////////////////

		const UniformBufferPtr& GLES20RenderPipeline::GetTransformUniformBuffer() const { return m_transformUniformBuffer; }
		
		///////////////////////////////////////////////////////////////////////////////////
	}
}