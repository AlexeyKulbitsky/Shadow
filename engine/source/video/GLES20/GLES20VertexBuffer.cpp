#include "GLES20VertexBuffer.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		GLES20VertexBuffer::GLES20VertexBuffer(Usage usage) : VertexBuffer(usage)
		{
			glGenBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		GLES20VertexBuffer::~GLES20VertexBuffer()
		{
			glDeleteBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20VertexBuffer::SetData(size_t offset, size_t length, const void* data)
		{
			Bind();

			GLenum usage;
			switch (m_usage)
			{
			case STATIC:
				usage = GL_STATIC_DRAW;
				break;
			case DYNAMIC:
				usage = GL_DYNAMIC_DRAW;
				break;
			default:
				SH_ASSERT(0, "Unknown hardware buffer usage in OpenGL ES 2.0 API");
				break;
			}

			if (offset == 0 && (length == m_size || m_size == 0))
			{
				m_size = length;
				glBufferData(GL_ARRAY_BUFFER, m_size, data, usage);
			}
			else
			{
				glBufferSubData(GL_ARRAY_BUFFER, offset, length, data);
				m_size = offset + length;
			}

			Unbind();
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20VertexBuffer::GetData(size_t offset, size_t length, const void* data)
		{
			SH_ASSERT(0, "Unimplemented yet!");
		}

		////////////////////////////////////////////////////////////////////////

	}
}