#include "GLES20VertexBuffer.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		GLES20VertexBuffer::GLES20VertexBuffer()
		{
			glGenBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		GLES20VertexBuffer::GLES20VertexBuffer(const void* data, size_t size)
		{
			glGenBuffers(1, &m_glID);
			glBindBuffer(GL_ARRAY_BUFFER, m_glID);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0U);
		}

		////////////////////////////////////////////////////////////////////////

		GLES20VertexBuffer::~GLES20VertexBuffer()
		{
			glDeleteBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20VertexBuffer::SetVerticesData(const void* data, size_t size)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_glID);
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0U);
		}

		////////////////////////////////////////////////////////////////////////

	}
}