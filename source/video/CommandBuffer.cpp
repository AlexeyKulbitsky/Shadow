#include "CommandBuffer.h"
#include "RenderState.h"
#include "ShaderProgram.h"
#include "Driver.h"
#include "../Device.h"


namespace sh
{
	namespace video
	{
		void CommandBuffer::SetRenderState(RenderState* renderState)
		{
			m_state = renderState;
		}

		//////////////////////////////////////////////////////////////////////////

		void CommandBuffer::SetShaderProgram(ShaderProgram* shaderProgram)
		{
			m_program = shaderProgram;
		}

		//////////////////////////////////////////////////////////////////////////

		void CommandBuffer::AddCommand(RenderCommand* renderCommand)
		{
			m_commands.push_back(renderCommand);
		}

		//////////////////////////////////////////////////////////////////////////

		void CommandBuffer::RemoveCommand(RenderCommand* renderCommand)
		{

		}

		//////////////////////////////////////////////////////////////////////////

		void CommandBuffer::Submit()
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