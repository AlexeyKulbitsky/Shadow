#include "GLES20IndexBuffer.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		GLES20IndexBuffer::GLES20IndexBuffer(Usage usage) : IndexBuffer(usage)
		{
			glGenBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		GLES20IndexBuffer::~GLES20IndexBuffer()
		{
			glDeleteBuffers(1, &m_glID);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20IndexBuffer::SetData(size_t offset, size_t length, const void* data)
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
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, data, usage);
			}
			else
			{
				glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, length, data);
				m_size = offset + length;
			}

			Unbind();
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20IndexBuffer::GetData(size_t offset, size_t length, const void* data)
		{

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

	}
}