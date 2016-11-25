#ifndef SHADOW_GLES20_COMMAND_BUFFER_INCLUDE
#define SHADOW_GLES20_COMMAND_BUFFER_INCLUDE

#include "../CommandBuffer.h"
#include <vector>

namespace sh
{
	namespace video
	{
		class GLES20RenderCommand;

		class GLES20CommandBuffer : public CommandBuffer
		{
		public:

			GLES20CommandBuffer(){}
			virtual ~GLES20CommandBuffer(){}
			virtual void Submit(RenderCommand* renderCommand) override;

		private:
			std::vector<GLES20RenderCommand*> m_commands;
		};
	}
}


#endif // !SHADOW_GLES20_COMMAND_BUFFER_INCLUDE