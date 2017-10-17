#ifndef SHADOW_GLES20_INDEX_BUFFER_INCLUDE
#define SHADOW_GLES20_INDEX_BUFFER_INCLUDE

#include "../../IndexBuffer.h"
#include "GLES20Common.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		class GLES20IndexBuffer : public IndexBuffer
		{
			friend class GLES20Driver;
			friend class GLES20HardwareBufferManager;
		public:
			virtual ~GLES20IndexBuffer();

			virtual void SetData(size_t offset, size_t length, const void* data) override;
			virtual void GetData(size_t offset, size_t length, const void* data) override;

			GLuint GetGLId() const { return m_glID; }

		private:
			GLES20IndexBuffer(const IndexBufferDescription& description);

		private:
			size_t m_size;
			GLuint m_glID = 0U;
		};
	}
}
#endif
