#ifndef SHADOW_VERTEX_BUFFER_INCLUDE
#define SHADOW_VERTEX_BUFFER_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{				
		struct VertexBufferDecription
		{
			u32 size;
			Usage usage;
		};

		class VertexBuffer
		{
		public:
			virtual ~VertexBuffer() {}

			virtual void SetData(size_t offset, size_t length, const void* data) { }
			virtual void GetData(size_t offset, size_t length, const void* data) { }

			// Set vertices count
			void SetVerticesCount(size_t count) { m_verticesCount = count; }
			// Get vertices count
			size_t GetVerticesCount() const { return m_verticesCount; }
			// Set the size of vertex (in bytes)
			void SetVertexSize(size_t size) { m_vertexSize = size; }
			// Get the size of vertex (in bytes)
			size_t GetVertexSize() const { return m_vertexSize; }
			
			void SetVertexDeclaration(const VertexDeclarationPtr& vertexDeclaration) { m_vertexDeclaration = vertexDeclaration; }
			const VertexDeclarationPtr& GetVertexDeclaration() const { return m_vertexDeclaration; }

			static VertexBufferPtr Create(const VertexBufferDecription& description);

		protected:
			VertexBuffer(const VertexBufferDecription& description);

		protected:
			size_t m_size;
			size_t m_verticesCount;
			size_t m_vertexSize;
			VertexDeclarationPtr m_vertexDeclaration;
			VertexBufferDecription m_description;
		};
	}
}
#endif // !SHADOW_VERTEX_BUFFER_INCLUDE
