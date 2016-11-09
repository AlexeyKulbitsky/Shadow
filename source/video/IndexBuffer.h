#ifndef SHADOW_INDEX_BUFFER_INCLUDE
#define SHADOW_INDEX_BUFFER_INCLUDE

#include <vector>

namespace sh
{
	namespace video
	{
		class IndexBuffer
		{
		public:
			enum class IndexType
			{
				UNSIGNED_16_BIT,
				UNSIGNED_32_BIT
			};

		public:
			IndexBuffer() {}
			virtual ~IndexBuffer() {}

			virtual void SetIndexType(IndexType indexType) { m_indexType = indexType; }
			// Set data for buffer
			void SetIndicesData(const void* data, size_t size);
			// Get the type of index
			IndexType GetIndexType() const { return m_indexType; }
			// Get the indices count
			size_t GetIndicesCount() const { return m_indicesCount; }
			// Get the size of index (in bytes)
			size_t GetIndexSize() const { return m_indexSize; }

		protected:
			IndexType m_indexType;
			size_t m_indicesCount;
			size_t m_indexSize;
			std::vector<unsigned int> m_indices;
		};

		inline void IndexBuffer::SetIndicesData(const void* data, size_t size)
		{
			m_indices.resize(size);
			std::copy(static_cast<const unsigned int*>(data), static_cast<const unsigned int*>(data)+size, m_indices.begin());
		}
	}
}
#endif // !SHADOW_INDEX_BUFFER_INCLUDE
