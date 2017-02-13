#ifndef SHADOW_INDEX_BUFFER_INCLUDE
#define SHADOW_INDEX_BUFFER_INCLUDE

#include "../Globals.h"
#include "HardwareBuffer.h"

namespace sh
{
	namespace video
	{
		class IndexBuffer : public HardwareBuffer
		{
		public:
			enum class IndexType
			{
				UNSIGNED_16_BIT,
				UNSIGNED_32_BIT
			};

		public:
			IndexBuffer(Usage usage) : HardwareBuffer(usage) {}
			virtual ~IndexBuffer() {}

			virtual void Bind() {}
			virtual void Unbind() {}
			virtual void Unload() {}

			virtual void SetIndexType(IndexType indexType) { m_indexType = indexType; }
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
	}
}
#endif // !SHADOW_INDEX_BUFFER_INCLUDE
