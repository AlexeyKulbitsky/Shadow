#ifndef SHADOW_GLES20_INDEX_BUFFER_INCLUDE
#define SHADOW_GLES20_INDEX_BUFFER_INCLUDE

#include "../IndexBuffer.h"

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
