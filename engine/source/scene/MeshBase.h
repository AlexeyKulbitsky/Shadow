#ifndef SHADOW_MESH_BASE_INCLUDE
#define SHADOW_MESH_BASE_INCLUDE

#include "../Globals.h"

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

			void SetTopology(sh::Topology topology);
			void SetVertexBuffer(video::VertexBufferPtr vertexBuffer);
			void SetIndexBuffer(video::IndexBufferPtr indexBuffer);
			void SetBoundingBox(const math::AABBf& boundingBox) { m_boundingBox = boundingBox; }
			void SetName(const String& name) { m_name = name; }
			const String& GetName() const { return m_name; }

//			void AddUniform(video::Uniform* uniform);
//			void AddSampler(video::Sampler* sampler);

//			size_t GetUniformsCount() const;
//			size_t GetSamplersCount() const;
//			video::Uniform* GetUniform(size_t index);
//			video::Sampler* GetSampler(size_t index);

			const video::VertexBufferPtr& GetVertexBuffer() const;
			const video::IndexBufferPtr& GetIndexBuffer() const;
			sh::Topology GetTopology();
			const math::AABBf& GetBoundingBox() const { return m_boundingBox; }

		private:
			math::AABBf m_boundingBox;
			video::VertexBufferPtr m_vertexBuffer;
			video::IndexBufferPtr m_indexBuffer;
			sh::Topology m_topology;
			bool m_useIndices;
			String m_name;

//			std::vector<video::Uniform*> m_uniforms;
//			std::vector<video::Sampler*> m_samplers;
		};
	}
}

#endif