#include "video/IndexBuffer.h"
#include "video/Managers/HardwareBufferManager.h"

namespace sh
{

namespace video
{

    IndexBuffer::IndexBuffer(Usage usage) 
    {
    }

    IndexBuffer::IndexBuffer(const IndexBufferDescription& description)
        : m_description(description)
    {
    }

    IndexBuffer::~IndexBuffer() 
    {
    }

    void IndexBuffer::SetData(size_t, size_t, const void*) 
    { 
    }

    void IndexBuffer::GetData(size_t, size_t, const void*) 
    { 
    }

    IndexType IndexBuffer::GetIndexType() const 
    { 
        return m_indexType; 
    }

    void IndexBuffer::SetIndicesCount(size_t count) 
    { 
        m_indicesCount = count; 
    }

    size_t IndexBuffer::GetIndicesCount() const 
    { 
        return m_indicesCount; 
    }

    size_t IndexBuffer::GetIndexSize() const 
    { 
        return m_indexSize; 
    }

	IndexBufferPtr IndexBuffer::Create(const IndexBufferDescription& description)
	{
		return HardwareBufferManager::GetInstance()->CreateIndexBuffer(description);
	}

} // video

} // sh