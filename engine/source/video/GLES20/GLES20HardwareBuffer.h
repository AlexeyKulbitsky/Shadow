#ifndef SHADOW_GLES20_HARDWARE_BUFFER_INCLUDE
#define SHADOW_GLES20_HARDWARE_BUFFER_INCLUDE

#include "GLContext/EGLContextManager.h"
#include "../HardwareBuffer.h"
#include "../../Types.h"

namespace sh
{
	namespace video
	{
		class GLES20VertexDeclaration;

		struct GLES20HardwareBuffer : public HardwareBuffer
		{
			// Info about all attbites for this buffer
			GLES20VertexDeclaration* glVertexDeclaration;

			// GLES 2.0 - specefic members
			GLuint glVertexBufferID = 0;
			GLuint glIndexBufferID = 0;
			GLenum glVerticesUsageType = GL_STATIC_DRAW;
			GLenum glIndicesUsageType = GL_STATIC_DRAW;
			GLenum glTopology = GL_TRIANGLES;
			GLsizei glVerticesSize = 0;
			GLsizei glIindicesSize = 0;
		};
	}
}
#endif