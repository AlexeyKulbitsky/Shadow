#ifndef SHADOW_INDEX_BUFFER_INCLUDE
#define SHADOW_INDEX_BUFFER_INCLUDE

#include "../Globals.h"

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
			IndexBuffer(Usage usage) {}
			virtual ~IndexBuffer() {}

			virtual void SetData(size_t offset, size_t length, const void* data) { }
			virtual void GetData(size_t offset, size_t length, const void* data) { }

			// Get the type of index
			IndexType GetIndexType() const { return m_indexType; }
			void SetIndicesCount(size_t count) { m_indicesCount = count; }
			// Get the indices count
			size_t GetIndicesCount() const { return m_indicesCount; }
			// Get the size of index (in bytes)
			size_t GetIndexSize() const { return m_indexSize; }

			static IndexBufferPtr Create(const IndexBufferDescription& description);

		protected:
			IndexBuffer(const IndexBufferDescription& description);

		protected:
			IndexType m_indexType;
			size_t m_indicesCount;
			size_t m_indexSize = 4U;
			IndexBufferDescription m_description;
		};

	}
}
#endif // !SHADOW_INDEX_BUFFER_INCLUDE
