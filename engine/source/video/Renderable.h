#ifndef SHADOW_RENDERABLE_INCLUDE
#define SHADOW_RENDERABLE_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class VertexDeclaration;
		class VertexInputDeclaration;

		class Renderable
		{
		public:
			Renderable(){}
			virtual ~Renderable(){}

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