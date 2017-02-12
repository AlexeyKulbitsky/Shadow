#ifndef SHADOW_VERTEX_BUFFER_INCLUDE
#define SHADOW_VERTEX_BUFFER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{				
		class VertexBuffer
		{
		public:
			VertexBuffer() {}
			virtual ~VertexBuffer() {}

			virtual void Bind() {}
			virtual void Unbind() {}
			virtual void Unload() {}
			// Set data for buffer
			virtual void SetVerticesData(const std::vector<float>& data);
			virtual void SetVerticesData(const void* data, size_t size) { }
			// Set vertices count
			void SetVerticesCount(size_t count) { m_verticesCount = count; }
			// Get vertices count
			size_t GetVerticesCount() const { return m_verticesCount; }
			// Set the size of vertex (in bytes)
			void SetVertexSize(size_t size) { m_vertexSize = size; }
			// Set the size of vertex (in bytes)
			size_t GetVertexSize() const { return m_vertexSize; }
			// Get pointer to vertex data
			//const void* GetVerticesPointer() const { return m_vertices.data(); }

		private:
			size_t m_verticesCount;
			size_t m_vertexSize;
			//std::vector<float> m_vertices;
		};

		inline void VertexBuffer::SetVerticesData(const std::vector<float>& data)
		{
			SetVerticesData(data.data(), sizeof(float) * data.size());
		}
	}
}
#endif // !SHADOW_VERTEX_BUFFER_INCLUDE
