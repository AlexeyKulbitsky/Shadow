#ifndef SHADOW_GLES20_VERTEX_BUFFER_INCLUDE
#define SHADOW_GLES20_VERTEX_BUFFER_INCLUDE

#include "../VertexBuffer.h"

namespace sh
{
	namespace video
	{
		class GLES20VertexBuffer : public VertexBuffer
		{
			friend class GLES20Driver;
		public:
			GLES20VertexBuffer();
			virtual ~GLES20VertexBuffer();

		private:
			unsigned int m_glID = 0U;
		};
	}
}

#endif