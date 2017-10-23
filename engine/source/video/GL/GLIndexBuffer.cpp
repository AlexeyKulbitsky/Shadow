#include "GLIndexBuffer.h"
//#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		GLIndexBuffer::~GLIndexBuffer()
		{
			GL_CALL(glDeleteBuffers(1, &m_glID));
		}

		////////////////////////////////////////////////////////////////////////

		void GLIndexBuffer::SetData(size_t offset, size_t length, const void* data)
		{
			GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glID));

			GLenum usage = s_glUsage[m_description.usage];

			if (offset == 0 && (length >= m_size || m_size == 0))
			{
				m_size = length;
				GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, data, usage));
			}
			else
			{
				GL_CALL(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, length, data));
				m_size = offset + length;
			}

			GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		}

		////////////////////////////////////////////////////////////////////////

		void GLIndexBuffer::GetData(size_t offset, size_t length, const void* data)
		{

		}

		////////////////////////////////////////////////////////////////////////

		GLIndexBuffer::GLIndexBuffer(const IndexBufferDescription& description)
			: IndexBuffer(description)
			, m_size(0U)
		{
			GL_CALL(glGenBuffers(1, &m_glID));
		}
	}
}