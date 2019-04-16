#ifndef SHADOW_GL_VERTEX_BUFFER_INCLUDE
#define SHADOW_GL_VERTEX_BUFFER_INCLUDE

#include "video/VertexBuffer.h"
#include "video/GL/GLCommon.h"

namespace sh
{
	namespace video
	{
		class GLVertexBuffer : public VertexBuffer
		{
			friend class GLHardwareBufferManager;
		public:
			virtual ~GLVertexBuffer();

			void SetData(size_t offset, size_t length, const void* data) override final;
			void GetData(size_t offset, size_t length, const void* data) override final;

            GLuint GetGLId() const;

		private:
			GLVertexBuffer(const VertexBufferDecription& description);

		private:
			GLuint m_glID = 0U;
		};
	}
}

#endif