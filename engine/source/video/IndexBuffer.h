#ifndef SHADOW_INDEX_BUFFER_INCLUDE
#define SHADOW_INDEX_BUFFER_INCLUDE

#include "common/Prerequisities.h"
#include "video/Common.h"

namespace sh
{

namespace video
{		

    struct SHADOW_API IndexBufferDescription
    {
        IndexType indexType;
        Usage usage;
    };

    class SHADOW_API IndexBuffer
    {
    public:
        IndexBuffer(Usage usage);
        virtual ~IndexBuffer();

        virtual void SetData(size_t offset, size_t length, const void* data);
        virtual void GetData(size_t offset, size_t length, const void* data);

        IndexType GetIndexType() const;
        void SetIndicesCount(size_t count);
        size_t GetIndicesCount() const;
        // Get the size of index (in bytes)
        size_t GetIndexSize() const;

        static IndexBufferPtr Create(const IndexBufferDescription& description);

    protected:
        IndexBuffer(const IndexBufferDescription& description);

    protected:
        IndexType m_indexType;
        size_t m_indicesCount;
        size_t m_indexSize = 4U;
        IndexBufferDescription m_description;
    };

} // video

} // sh

#endif // !SHADOW_INDEX_BUFFER_INCLUDE
