#ifndef SHADOW_GLES20_UNIFORM_BUFFER
#define SHADOW_GLES20_UNIFORM_BUFFER

#include "../UniformBuffer.h"
#include <vector>

namespace sh
{
	namespace video
	{
		class GLES20UniformBuffer : public UniformBuffer
		{
		public:
			GLES20UniformBuffer() {}
			virtual ~GLES20UniformBuffer() {}
		};
	}
}



#endif
