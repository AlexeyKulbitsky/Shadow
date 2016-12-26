#include "RenderBatch.h"
#include "RenderState.h"
#include "ShaderProgram.h"
#include "Driver.h"
#include "../Device.h"


namespace sh
{
	namespace video
	{
		void RenderBatch::SetRenderState(RenderState* renderState)
		{
			m_state = renderState;
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

			m_state->Apply();
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