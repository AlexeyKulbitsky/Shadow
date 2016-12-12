#ifndef SHADOW_GLES20_COMMAND_BUFFER_INCLUDE
#define SHADOW_GLES20_COMMAND_BUFFER_INCLUDE

#include "../CommandBuffer.h"

namespace sh
{
	namespace video
	{
		class GLES20CommandBuffer : public CommandBuffer
		{
		public:

			GLES20CommandBuffer(){}
			virtual ~GLES20CommandBuffer(){}
		};
	}
}


#endif // !SHADOW_GLES20_COMMAND_BUFFER_INCLUDE