#ifndef SHADOW_VERTEX_BUFFER_INCLUDE
#define SHADOW_VERTEX_BUFFER_INCLUDE

#include <vector>
#include "VertexDeclaration.h"

namespace sh
{
	namespace video
	{				
		class VertexBuffer
		{
		public:
			VertexBuffer() {}
			VertexBuffer(const VertexDecalaration& declaration)
				: m_vertexDeclaration(declaration)
			{
			}
			virtual ~VertexBuffer() {}

			void SetVerticesData(const void* data, size_t size);
			const void* GetVerticesPointer() const { return m_vertices.data(); }

		private:
			std::vector<float> m_vertices;

			VertexDecalaration m_vertexDeclaration;
		};

		inline void VertexBuffer::SetVerticesData(const void* data, size_t size)
		{
			m_vertices.resize(size);
			std::copy(static_cast<const float*>(data), static_cast<const float*>(data)+size, m_vertices.begin());
		}
	}
}
#endif // !SHADOW_VERTEX_BUFFER_INCLUDE
