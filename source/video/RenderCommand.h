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
		class RenderState;
		class ShaderProgram;
		class UniformBuffer;

		class RenderCommand
		{
		public:
			RenderCommand(){}
			virtual ~RenderCommand(){}

			virtual void Init() = 0;

			virtual void SetUseIndices(bool useIndices) = 0;
			virtual void SetVertexBuffer(VertexBuffer* vertexBuffer) = 0;
			virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0;
			virtual void SetUniformBuffer(UniformBuffer* uniformBuffer) = 0;
			virtual void SetRenderState(RenderState* renderState) = 0;
			virtual void SetShaderProgram(ShaderProgram* shaderProgram) = 0;
			virtual void SetTopology(Topology topology) = 0;

			virtual bool IsUseIndices() const = 0;
			virtual VertexBuffer* GetVertexBuffer() = 0;
			virtual IndexBuffer* GetIndexBuffer() = 0;
			virtual UniformBuffer* GetUniformBuffer() = 0;
			virtual RenderState* GetRenderState() = 0;
			virtual ShaderProgram* GetShaderProgram() = 0;
			virtual Topology GetTopology() const = 0;
		};
	}
}

#endif