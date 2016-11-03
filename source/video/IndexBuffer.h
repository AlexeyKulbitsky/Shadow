#ifndef SHADOW_INDEX_BUFFER_INCLUDE
#define SHADOW_INDEX_BUFFER_INCLUDE

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
		};
	}
}
#endif // !SHADOW_INDEX_BUFFER_INCLUDE
