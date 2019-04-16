#include "video/GL/GLVertexBuffer.h"
#include "video/GL/GLDebug.h"

namespace sh
{
	namespace video
	{
		GLVertexBuffer::~GLVertexBuffer()
		{
			GL_CALL(glDeleteBuffers(1, &m_glID));
		}

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

		void GLVertexBuffer::GetData(size_t, size_t, const void*)
		{
			assert(0 && "Unimplemented yet!");
		}

        GLuint GLVertexBuffer::GetGLId() const
        { 
            return m_glID; 
        }

		GLVertexBuffer::GLVertexBuffer(const VertexBufferDecription& description)
			: VertexBuffer(description)
		{
			GL_CALL(glGenBuffers(1, &m_glID));
		}

	}
}