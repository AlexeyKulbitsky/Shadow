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
			GLES20IndexBuffer(Usage usage);
			virtual ~GLES20IndexBuffer();

			virtual void SetData(size_t offset, size_t length, const void* data) override;
			virtual void GetData(size_t offset, size_t length, const void* data) override;

			virtual void Bind() override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glID); }
			virtual void Unbind() override { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

			virtual void SetIndexType(IndexType indexType) override;

			u32 GetGLId() const { return m_glID; }
			u32 GetGLIndexType() const { return m_glIndexType; }
		private:
			u32 m_glID = 0U;
			u32 m_glIndexType;
		};
	}
}
#endif
