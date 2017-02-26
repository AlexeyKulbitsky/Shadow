#ifndef SHADOW_RENDER_COMMAND_INCLUDE
#define SHADOW_RENDER_COMMAND_INCLUDE

#include "../Globals.h"

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

		class VertexDeclaration;
		class VertexInputDeclaration;

		class RenderCommand
		{
		public:
			RenderCommand(){}
			virtual ~RenderCommand(){}

			virtual void Init() = 0;

			virtual void SetUseIndices(bool useIndices) = 0;
			virtual void SetVertexBuffer(VertexBuffer* vertexBuffer) = 0;
			virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0;
			virtual void SetTopology(Topology topology) = 0;
			virtual void SetVertexInputDeclaration(VertexInputDeclaration* declaration) = 0;
			void SetAutoUniformsBatch(const UniformsBatchPtr& unifomsBatch) { m_autoUniformsBatch = unifomsBatch; }

			virtual bool IsUseIndices() const = 0;
			virtual VertexBuffer* GetVertexBuffer() = 0;
			virtual IndexBuffer* GetIndexBuffer() = 0;
			virtual Topology GetTopology() const = 0;
			virtual VertexInputDeclaration* GetVertexInputDeclaration() const = 0;
			const UniformsBatchPtr& GetAutoUniformsBatch() const { return m_autoUniformsBatch; }

		protected:
			UniformsBatchPtr m_autoUniformsBatch;
		};
	}
}

#endif