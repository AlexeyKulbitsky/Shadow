#ifndef SHADOW_VERTEX_BUFFER_INCLUDE
#define SHADOW_VERTEX_BUFFER_INCLUDE

#include "common/Prerequisities.h"
#include "video/Common.h"

namespace sh
{

namespace video
{				
    struct SHADOW_API VertexBufferDecription
    {
        uint32_t size;
        Usage usage;
    };

    class SHADOW_API VertexBuffer
    {
    public:
        virtual ~VertexBuffer();

        virtual void SetData(size_t offset, size_t length, const void* data);
        virtual void GetData(size_t offset, size_t length, const void* data);

        void SetVerticesCount(size_t count);
        size_t GetVerticesCount() const;
        // Set the size of vertex (in bytes)
        void SetVertexSize(size_t size);
        // Get the size of vertex (in bytes)
        size_t GetVertexSize() const;

        //void SetVertexDeclaration(const VertexDeclarationPtr& vertexDeclaration) { m_vertexDeclaration = vertexDeclaration; }
        //const VertexDeclarationPtr& GetVertexDeclaration() const { return m_vertexDeclaration; }

        static VertexBufferPtr Create(const VertexBufferDecription& description);

    protected:
        VertexBuffer(const VertexBufferDecription& description);

    protected:
        size_t m_size;
        size_t m_verticesCount;
        size_t m_vertexSize;
        //VertexDeclarationPtr m_vertexDeclaration;
        VertexBufferDecription m_description;
    };

} // video

} // sh

#endif // !SHADOW_VERTEX_BUFFER_INCLUDE
