#ifndef SHADOW_GLES20_COMMAND_BUFFER_INCLUDE
#define SHADOW_GLES20_COMMAND_BUFFER_INCLUDE

#include "../CommandBuffer.h"

namespace sh
{
	namespace video
	{
		class GLES20RenderCommand;
		class GLES20RenderState;
		class GLES20ShderProgram;

		class GLES20CommandBuffer : public CommandBuffer
		{
		public:

			GLES20CommandBuffer(){}
			virtual ~GLES20CommandBuffer(){}
			virtual void SetRenderState(RenderState* renderState) override;
			virtual void SetShaderProgram(ShaderProgram* shaderProgram) override;
			virtual void AddCommand(RenderCommand* renderCommand) override;
			virtual void RemoveCommand(RenderCommand* renderCommand) override;

		private:
			std::vector<GLES20RenderCommand*> m_commands;
		};
	}
}


#endif // !SHADOW_GLES20_COMMAND_BUFFER_INCLUDE