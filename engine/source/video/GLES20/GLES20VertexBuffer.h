#ifndef SHADOW_GLES20_VERTEX_BUFFER_INCLUDE
#define SHADOW_GLES20_VERTEX_BUFFER_INCLUDE

#include "../VertexBuffer.h"
#include "GLES20Common.h"
#include "../GLContext/EGLContextManager.h"

namespace sh
{
	namespace video
	{
		class GLES20VertexBuffer : public VertexBuffer
		{
			friend class GLES20Driver;
		public:
			virtual ~GLES20VertexBuffer();

			virtual void SetData(size_t offset, size_t length, const void* data) override;
			virtual void GetData(size_t offset, size_t length, const void* data) override;

			unsigned int GetGLId() const { return m_glID; }

		private:
			GLES20VertexBuffer(const VertexBufferDecription& description);

		private:
			GLuint m_glID = 0U;
		};
	}
}

#endif