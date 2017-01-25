#include "RenderBatch.h"
#include "ShaderProgram.h"
#include "Driver.h"
#include "../Device.h"


namespace sh
{
	namespace video
	{
		void RenderBatch::SetDepthStencilState(DepthStencilState* depthStencilState)
		{
			m_depthStencilState = depthStencilState;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetRasterizationState(RasterizationState* rasterizationState)
		{
			m_rasterizationState = rasterizationState;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetBlendingState(BlendingState* blendingState)
		{
			m_blendingState = blendingState;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetShaderProgram(ShaderProgram* shaderProgram)
		{
			m_program = shaderProgram;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::AddCommand(RenderCommand* renderCommand)
		{
			m_commands.push_back(renderCommand);
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::RemoveCommand(RenderCommand* renderCommand)
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

			for (auto command : m_commands)
			{
				driver->Render(command);
			}

			m_program->UnbindProgram();
		}

		//////////////////////////////////////////////////////////////////////////

	}
}