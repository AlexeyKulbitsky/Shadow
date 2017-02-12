#include "GLES20IndexBuffer.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		GLES20IndexBuffer::GLES20IndexBuffer()
		{
			glGenBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		GLES20IndexBuffer::GLES20IndexBuffer(const void* data, size_t size)
		{
			glGenBuffers(1, &m_glID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0U);
		}

		////////////////////////////////////////////////////////////////////////

		GLES20IndexBuffer::~GLES20IndexBuffer()
		{
			glDeleteBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20IndexBuffer::SetIndexType(IndexType indexType)
		{
			IndexBuffer::SetIndexType(indexType);
			switch (indexType)
			{
			case IndexType::UNSIGNED_16_BIT:
				m_glIndexType = GL_UNSIGNED_SHORT;
				break;
			case IndexType::UNSIGNED_32_BIT:
				m_glIndexType = GL_UNSIGNED_INT;
				break;
			}
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20IndexBuffer::SetIndicesData(const void* data, size_t size)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0U);
		}

		////////////////////////////////////////////////////////////////////////


	}
}