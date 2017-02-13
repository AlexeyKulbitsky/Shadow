#ifndef SHADOW_VERTEX_BUFFER_INCLUDE
#define SHADOW_VERTEX_BUFFER_INCLUDE

#include "../Globals.h"
#include "HardwareBuffer.h"

namespace sh
{
	namespace video
	{				
		class VertexBuffer : public HardwareBuffer
		{
		public:
			VertexBuffer(Usage usage) : HardwareBuffer(usage) {}
			virtual ~VertexBuffer() {}

			virtual void Bind() {}
			virtual void Unbind() {}
			virtual void Unload() {}
			// Set vertices count
			void SetVerticesCount(size_t count) { m_verticesCount = count; }
			// Get vertices count
			size_t GetVerticesCount() const { return m_verticesCount; }
			// Set the size of vertex (in bytes)
			void SetVertexSize(size_t size) { m_vertexSize = size; }
			// Set the size of vertex (in bytes)
			size_t GetVertexSize() const { return m_vertexSize; }

		private:
			size_t m_verticesCount;
			size_t m_vertexSize;
		};
	}
}
#endif // !SHADOW_VERTEX_BUFFER_INCLUDE
