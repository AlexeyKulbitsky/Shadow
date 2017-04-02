#include "GLES20IndexBuffer.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		GLES20IndexBuffer::~GLES20IndexBuffer()
		{
			glDeleteBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20IndexBuffer::SetData(size_t offset, size_t length, const void* data)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glID);

			GLenum usage = s_glUsage[m_description.usage];

			if (offset == 0 && (length == m_size || m_size == 0))
			{
				m_size = length;
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, data, usage);
			}
			else
			{
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, length, data);
				m_size = offset + length;
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20IndexBuffer::GetData(size_t offset, size_t length, const void* data)
		{

		}

		////////////////////////////////////////////////////////////////////////

		GLES20IndexBuffer::GLES20IndexBuffer(const IndexBufferDescription& description)
			: IndexBuffer(description)
			, m_size(0U)
		{
			glGenBuffers(1, &m_glID);
		}
	}
}