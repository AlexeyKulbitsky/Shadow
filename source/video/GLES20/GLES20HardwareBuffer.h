#ifndef SHADOW_GLES20_HARDWARE_BUFFER_INCLUDE
#define SHADOW_GLES20_HARDWARE_BUFFER_INCLUDE

#include "../HardwareBuffer.h"
#include "../../Types.h"

namespace sh
{
	namespace video
	{
		struct GLES20HardwareBuffer : public HardwareBuffer
		{
			u32 vbo_verticesID; 
			u32 vbo_indicesID; 

			u32 vbo_verticesSize;
			u32 vbo_indicesSize;
		};
	}
}
#endif