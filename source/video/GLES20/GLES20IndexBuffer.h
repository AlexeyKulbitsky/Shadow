#ifndef SHADOW_GLES20_INDEX_BUFFER_INCLUDE
#define SHADOW_GLES20_INDEX_BUFFER_INCLUDE

#include "../IndexBuffer.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		class GLES20IndexBuffer : public IndexBuffer
		{
			friend class GLES20Driver;
		public:
			GLES20IndexBuffer();
			GLES20IndexBuffer(const void* data, size_t size);
			virtual ~GLES20IndexBuffer();

			virtual void Bind() override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glID); }
			virtual void Unbind() override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

			virtual void SetIndexType(IndexType indexType) override;

			unsigned int GetGLId() const { return m_glID; }
			unsigned int GetGLIndexType() const { return m_glIndexType; }
		private:
			unsigned int m_glID = 0U;
			unsigned int m_glIndexType;
		};
	}
}
#endif
