#ifndef SHADOW_GL_INDEX_BUFFER_INCLUDE
#define SHADOW_GL_INDEX_BUFFER_INCLUDE

#include "video/IndexBuffer.h"
#include "video/GL/GLCommon.h"

namespace sh
{

namespace video
{
	
    class GLIndexBuffer : public IndexBuffer
    {
        friend class GLHardwareBufferManager;
    public:
        virtual ~GLIndexBuffer();

        void SetData(size_t offset, size_t length, const void* data) override final;
        void GetData(size_t offset, size_t length, const void* data) override final;

        GLuint GetGLId() const;

    private:
        GLIndexBuffer(const IndexBufferDescription& description);

    private:
        size_t m_size;
        GLuint m_glID = 0U;
    };

} // video

} // sh

#endif
