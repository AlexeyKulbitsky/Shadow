#ifndef SHADOW_GL_VERTEX_BUFFER_INCLUDE
#define SHADOW_GL_VERTEX_BUFFER_INCLUDE

#include "../VertexBuffer.h"
#include "GLCommon.h"

namespace sh
{
	namespace video
	{
		class GLVertexBuffer : public VertexBuffer
		{
			//friend class GLES20Driver;
			friend class GLHardwareBufferManager;
		public:
			virtual ~GLVertexBuffer();

			virtual void SetData(size_t offset, size_t length, const void* data) override;
			virtual void GetData(size_t offset, size_t length, const void* data) override;

			unsigned int GetGLId() const { return m_glID; }

		private:
			GLVertexBuffer(const VertexBufferDecription& description);

		private:
			GLuint m_glID = 0U;
		};
	}
}

#endif