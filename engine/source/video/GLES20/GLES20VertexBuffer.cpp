#include "GLES20VertexBuffer.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		GLES20VertexBuffer::~GLES20VertexBuffer()
		{
			glDeleteBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20VertexBuffer::SetData(size_t offset, size_t length, const void* data)
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_glID);

			GLenum usage = s_glUsage[m_description.usage];

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


			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20VertexBuffer::GetData(size_t offset, size_t length, const void* data)
		{
			SH_ASSERT(0, "Unimplemented yet!");
		}

		////////////////////////////////////////////////////////////////////////

		GLES20VertexBuffer::GLES20VertexBuffer(const VertexBufferDecription& description)
			: VertexBuffer(description)
		{
			glGenBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

	}
}