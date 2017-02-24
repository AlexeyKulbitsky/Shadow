#include "RenderBatch.h"
#include "ShaderProgram.h"
#include "UniformBuffer.h"
#include "Driver.h"
#include "../Device.h"


namespace sh
{
	namespace video
	{
		void RenderBatch::SetDepthStencilState(const DepthStencilStatePtr& depthStencilState)
		{
			m_depthStencilState = depthStencilState;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetRasterizationState(const RasterizationStatePtr& rasterizationState)
		{
			m_rasterizationState = rasterizationState;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetBlendingState(const BlendingStatePtr& blendingState)
		{
			m_blendingState = blendingState;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetShaderProgram(const ShaderProgramPtr& shaderProgram)
		{
			m_program = shaderProgram;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetUniformBuffer(const UniformBufferPtr& uniformBuffer)
		{
			m_uniformBuffer = uniformBuffer;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::AddCommand(const RenderCommandPtr& renderCommand)
		{
			m_commands.push_back(renderCommand);
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::RemoveCommand(const RenderCommandPtr& renderCommand)
		{

		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::Submit()
		{
			Driver* driver = Device::GetInstance()->GetDriver();

			driver->SetDepthStencilState(m_depthStencilState);
			driver->SetBlendingState(m_blendingState);
			driver->SetRasterizationState(m_rasterizationState);
			m_program->BindProgram();

			m_uniformBuffer->Upload();

			for (auto command : m_commands)
			{
				driver->Render(command);
			}

			m_program->UnbindProgram();
		}

		//////////////////////////////////////////////////////////////////////////

	}
}