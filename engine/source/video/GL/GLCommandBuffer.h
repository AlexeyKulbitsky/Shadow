#ifndef SHADOW_GL_COMMAND_BUFFER_INCLUDE
#define SHADOW_GL_COMMAND_BUFFER_INCLUDE

#include "../CommandBuffer.h"

namespace sh
{
	namespace video
	{
		class GLCommandBuffer : public CommandBuffer
		{
		public:

			GLCommandBuffer(){}
			virtual ~GLCommandBuffer(){}
		};
	}
}


#endif // !SHADOW_GLES20_COMMAND_BUFFER_INCLUDE