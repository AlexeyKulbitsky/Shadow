#ifndef SHADOW_GL_INDEX_BUFFER_INCLUDE
#define SHADOW_GL_INDEX_BUFFER_INCLUDE

#include "../IndexBuffer.h"
#include "GLCommon.h"

namespace sh
{
	namespace video
	{
		class GLIndexBuffer : public IndexBuffer
		{
			//friend class GLES20Driver;
			friend class GLHardwareBufferManager;
		public:
			virtual ~GLIndexBuffer();

			virtual void SetData(size_t offset, size_t length, const void* data) override;
			virtual void GetData(size_t offset, size_t length, const void* data) override;

			GLuint GetGLId() const { return m_glID; }

		private:
			GLIndexBuffer(const IndexBufferDescription& description);

		private:
			size_t m_size;
			GLuint m_glID = 0U;
		};
	}
}
#endif
