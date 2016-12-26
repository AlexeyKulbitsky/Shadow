#ifndef SHADOW_COMMAND_BUFFER_INCLUDE
#define SHADOW_COMMAND_BUFFER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class RenderCommand;
		class RenderState;
		class ShaderProgram;

		class CommandBuffer
		{
		public:
			CommandBuffer() {}
			~CommandBuffer() {}
		};

	}
}
#endif // !SHADOW_COMMAND_BUFFER_INCLUDE
