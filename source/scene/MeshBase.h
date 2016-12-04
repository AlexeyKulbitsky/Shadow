#ifndef SHADOW_SUB_ENTITY_INCLUDE
#define SHADOW_SUB_ENTITY_INCLUDE

#include "../Globals.h"
#include "../video/VertexBuffer.h"
#include "../video/IndexBuffer.h"
#include "../video/RenderCommand.h"

namespace sh
{
	namespace video
	{
		class Uniform;
		class Sampler;
	}

	namespace scene
	{
		

		class MeshBase
		{
		public:
			MeshBase();
			~MeshBase();

			bool IsUseIndices() { return m_useIndices; }

			void SetVertexDeclaration(sh::video::VertexDeclaration* vertexDeclaration);
			void SetTopology(sh::video::Topology topology);
			void SetVertexBuffer(video::VertexBuffer* vertexBuffer);
			void SetIndexBuffer(video::IndexBuffer* indexBuffer);

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
			video::VertexBuffer* m_vertexBuffer;
			video::IndexBuffer* m_indexBuffer;
			video::VertexDeclaration* m_vertexDeclaration;
			sh::video::Topology m_topology;
			bool m_useIndices;

			std::vector<video::Uniform*> m_uniforms;
			std::vector<video::Sampler*> m_samplers;
		};
	}
}

#endif