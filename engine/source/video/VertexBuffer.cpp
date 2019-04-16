#include "video/VertexBuffer.h"
#include "video/Managers/HardwareBufferManager.h"

namespace sh
{

namespace video
{

	VertexBufferPtr VertexBuffer::Create(const VertexBufferDecription& description)
	{
		return HardwareBufferManager::GetInstance()->CreateVertexBuffer(description);
	}

    VertexBuffer::VertexBuffer(const VertexBufferDecription& description)
        : m_description(description)
        , m_size(0U)
        , m_verticesCount(0U)
    {
    }

    VertexBuffer::~VertexBuffer() 
    {
    }

    void VertexBuffer::SetData(size_t offset, size_t length, const void* data) 
    {
    }

    void VertexBuffer::GetData(size_t offset, size_t length, const void* data) 
    { 
    }

    void VertexBuffer::SetVerticesCount(size_t count) 
    { 
        m_verticesCount = count; 
    }

    size_t VertexBuffer::GetVerticesCount() const 
    { 
        return m_verticesCount; 
    }

    void VertexBuffer::SetVertexSize(size_t size) 
    { 
        m_vertexSize = size; 
    }

    size_t VertexBuffer::GetVertexSize() const 
    { 
        return m_vertexSize; 
    }

} // video

} // sh