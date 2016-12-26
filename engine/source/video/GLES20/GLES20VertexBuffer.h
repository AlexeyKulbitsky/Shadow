#ifndef SHADOW_GLES20_VERTEX_BUFFER_INCLUDE
#define SHADOW_GLES20_VERTEX_BUFFER_INCLUDE

#include "../VertexBuffer.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		class GLES20VertexBuffer : public VertexBuffer
		{
			friend class GLES20Driver;
		public:
			GLES20VertexBuffer();
			GLES20VertexBuffer(const void* data, size_t size);
			virtual ~GLES20VertexBuffer();

			virtual void Bind() override { glBindBuffer(GL_ARRAY_BUFFER, m_glID); }
			virtual void Unbind() override { glBindBuffer(GL_ARRAY_BUFFER, 0); }

			unsigned int GetGLId() const { return m_glID; }
		private:
			u32 m_glID = 0U;
		};
	}
}

#endif