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
			virtual ~VertexBuffer() {}

			virtual void Bind() {}
			virtual void Unbind() {}

			void SetVertexDeclaration(const VertexDeclaration& declaration) { m_vertexDeclaration = declaration; }
			VertexDeclaration* GetVertexDeclaration() { return &m_vertexDeclaration; }
			void SetVerticesData(const void* data, size_t size);
			const void* GetVerticesPointer() const { return m_vertices.data(); }

		private:
			std::vector<float> m_vertices;
			VertexDeclaration m_vertexDeclaration;
		};

		inline void VertexBuffer::SetVerticesData(const void* data, size_t size)
		{
			m_vertices.resize(size);
			std::copy(static_cast<const float*>(data), static_cast<const float*>(data)+size, m_vertices.begin());
		}
	}
}
#endif // !SHADOW_VERTEX_BUFFER_INCLUDE
