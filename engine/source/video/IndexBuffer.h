#ifndef SHADOW_INDEX_BUFFER_INCLUDE
#define SHADOW_INDEX_BUFFER_INCLUDE

#include "../Globals.h"

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

			virtual void Bind() {}
			virtual void Unbind() {}
			virtual void Unload() {}

			virtual void SetIndexType(IndexType indexType) { m_indexType = indexType; }
			// Set data for buffer
			virtual void SetIndicesData(const std::vector<unsigned int>& data);
			virtual void SetIndicesData(const void* data, size_t size) { }
			// Get the type of index
			IndexType GetIndexType() const { return m_indexType; }
			void SetIndicesCount(size_t count) { m_indicesCount = count; }
			// Get the indices count
			size_t GetIndicesCount() const { return m_indicesCount; }
			// Get the size of index (in bytes)
			size_t GetIndexSize() const { return m_indexSize; }

		protected:
			IndexType m_indexType;
			size_t m_indicesCount;
			size_t m_indexSize = 4U;
		};

		inline void IndexBuffer::SetIndicesData(const std::vector<unsigned int>& data)
		{
			SetIndicesData(data.data(), data.size() * m_indexSize);
		}
	}
}
#endif // !SHADOW_INDEX_BUFFER_INCLUDE
