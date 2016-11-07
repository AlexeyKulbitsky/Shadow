#ifndef SHADOW_RENDER_COMMAND_INCLUDE
#define SHADOW_RENDER_COMMAND_INCLUDE

namespace sh
{
	namespace video
	{
		enum class Topology
		{
			LINE_LIST = 0,
			LINE_LOOP,
			LINE_STRIP,
			TRIANGLE_LIST,
			TRIANGLE_STRIP,
			TRIANGLE_FAN
		};

		class VertexBuffer;
		class IndexBuffer;
		class VertexDeclaration;

		class RenderCommand
		{
		public:
			RenderCommand(){}
			virtual ~RenderCommand(){}

			void SetUseIndices(bool useIndices) { m_isUseIndices = useIndices; }
			bool IsUseIndices() const { return m_isUseIndices; }
			void SetVertexDeclaration(VertexDeclaration* declaration) { m_vertexDeclaration = declaration; }
			VertexDeclaration* GetVertexDeclaration() { return m_vertexDeclaration; }

			void SetVertexBuffer(const VertexBuffer* vertexBuffer);
			VertexBuffer* GetVertexBuffer();
			void SetIndexBuffer(const IndexBuffer* indexBuffer);
			IndexBuffer* GetIndexBuffer();

		protected:
			bool m_isUseIndices;
			VertexDeclaration* m_vertexDeclaration;
		};
	}
}

#endif