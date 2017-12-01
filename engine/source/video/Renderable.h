#ifndef SHADOW_RENDERABLE_INCLUDE
#define SHADOW_RENDERABLE_INCLUDE

#include "../Globals.h"

namespace sh
{
	namespace video
	{
		class SHADOW_API Renderable
		{
			friend class scene::Mesh;
			friend class RenderSystem;
		public:
			bool HasIndices() const { return m_indexBuffer != nullptr; }
			const VertexBufferPtr& GetVertexBuffer() const { return m_vertexBuffer; }
			const IndexBufferPtr GetIndexBuffer() const { return m_indexBuffer; }
			Topology GetTopology() const { return m_topology; }
			const VertexInputDeclarationPtr GetVertexInputDeclaration() const { return m_vertexDeclaration; }
			const GpuParamsPtr& GetAutoGpuParams() const { return m_transfromsGpuParams; }
			const MaterialParamsPtr& GetAutoParams() const { return m_transformParams; }
			scene::Mesh* GetParent() { return m_parent; }
			const math::Matrix4f& GetMatrix() const { return *m_matrix; }

		protected:
			scene::Mesh* m_parent = nullptr;
			VertexInputDeclarationPtr m_vertexDeclaration;
			MaterialParamsPtr m_transformParams;
			GpuParamsPtr m_transfromsGpuParams;
			VertexBufferPtr m_vertexBuffer;
			IndexBufferPtr m_indexBuffer;
			Topology m_topology;
			math::Matrix4f* m_matrix = nullptr;
		};
	}
}

#endif