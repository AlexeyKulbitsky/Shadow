#ifndef SHADOW_MESH_BASE_INCLUDE
#define SHADOW_MESH_BASE_INCLUDE

#include "../Globals.h"
#include "../video/RenderCommand.h"

namespace sh
{
	namespace scene
	{
		class MeshBase
		{
		public:
			MeshBase();
			~MeshBase();
			void Unload();

			bool IsUseIndices() { return m_useIndices; }

			void SetVertexDeclaration(sh::video::VertexDeclarationPtr vertexDeclaration);
			void SetTopology(sh::video::Topology topology);
			void SetVertexBuffer(video::VertexBufferPtr vertexBuffer);
			void SetIndexBuffer(video::IndexBufferPtr indexBuffer);

			void AddUniform(video::Uniform* uniform);
			void AddSampler(video::Sampler* sampler);

			size_t GetUniformsCount() const;
			size_t GetSamplersCount() const;
			video::Uniform* GetUniform(size_t index);
			video::Sampler* GetSampler(size_t index);

			video::VertexBuffer* GetVertexBuffer();
			video::IndexBuffer* GetIndexBuffer();
			video::VertexDeclaration* GetVertexDeclaration();
			sh::video::Topology GetTopology();

		private:
			video::VertexBufferPtr m_vertexBuffer;
			video::IndexBufferPtr m_indexBuffer;
			video::VertexDeclarationPtr m_vertexDeclaration;
			sh::video::Topology m_topology;
			bool m_useIndices;

			std::vector<video::Uniform*> m_uniforms;
			std::vector<video::Sampler*> m_samplers;
		};
	}
}

#endif