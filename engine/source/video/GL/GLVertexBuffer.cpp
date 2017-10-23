#include "GLVertexBuffer.h"
//#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		GLVertexBuffer::~GLVertexBuffer()
		{
			GL_CALL(glDeleteBuffers(1, &m_glID));
		}

		////////////////////////////////////////////////////////////////////////

		void GLVertexBuffer::SetData(size_t offset, size_t length, const void* data)
		{
			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_glID));

			GLenum usage = s_glUsage[m_description.usage];

			if (offset == 0 && (length >= m_size || m_size == 0))
			{
				m_size = length;
				GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_size, data, usage));
			}
			else
			{
				GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, offset, length, data));
				m_size = offset + length;
			}


			GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
		}

		////////////////////////////////////////////////////////////////////////

		void GLVertexBuffer::GetData(size_t offset, size_t length, const void* data)
		{
			SH_ASSERT(0, "Unimplemented yet!");
		}

		////////////////////////////////////////////////////////////////////////

		GLVertexBuffer::GLVertexBuffer(const VertexBufferDecription& description)
			: VertexBuffer(description)
		{
			GL_CALL(glGenBuffers(1, &m_glID));
		}

		////////////////////////////////////////////////////////////////////////

	}
}