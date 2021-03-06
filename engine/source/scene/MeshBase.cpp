#include "MeshBase.h"
#include "../Device.h"
#include "../video/Material.h"
#include "../video/RenderTechnique.h"
#include "../video/RenderPipeline.h"
#include "../video/VertexBuffer.h"
#include "../video/IndexBuffer.h"
#include "../video/VertexDeclaration.h"
#include "../scene/SceneManager.h"
#include "../scene/Camera.h"

namespace sh
{
	namespace scene
	{
		MeshBase::MeshBase()
			: m_useIndices(false)
			, m_boundingBox(math::Vector3(0.0f))
		{
		}

		/////////////////////////////////////////////////////////////////////////////////////

		MeshBase::~MeshBase()
		{
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void MeshBase::Unload()
		{
// 			for (size_t i = 0; i < m_samplers.size(); ++i)
// 			{
// 				delete m_samplers[i];
// 				m_samplers[i] = nullptr;
// 			}
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void MeshBase::SetTopology(Topology topology)
		{
			m_topology = topology;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void MeshBase::SetVertexBuffer(video::VertexBufferPtr vertexBuffer)
		{
			m_vertexBuffer = vertexBuffer;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void MeshBase::SetIndexBuffer(video::IndexBufferPtr indexBuffer)
		{
			m_indexBuffer = indexBuffer;
			m_useIndices = true;
		}

		/////////////////////////////////////////////////////////////////////////////////////

// 		void MeshBase::AddUniform(video::Uniform* uniform)
// 		{
// 			m_uniforms.push_back(uniform);
// 		}

		/////////////////////////////////////////////////////////////////////////////////////

// 		void MeshBase::AddSampler(video::Sampler* sampler)
// 		{
// 			m_samplers.push_back(sampler);
// 		}
// 
// 		size_t MeshBase::GetUniformsCount() const
// 		{
// 			return m_uniforms.size();
// 		}

		/////////////////////////////////////////////////////////////////////////////////////

// 		size_t MeshBase::GetSamplersCount() const
// 		{
// 			return m_samplers.size();
// 		}

		/////////////////////////////////////////////////////////////////////////////////////

// 		video::Uniform* MeshBase::GetUniform(size_t index)
// 		{
// 			return m_uniforms[index];
// 		}

		/////////////////////////////////////////////////////////////////////////////////////

// 		video::Sampler* MeshBase::GetSampler(size_t index)
// 		{
// 			return m_samplers[index];
// 		}

		/////////////////////////////////////////////////////////////////////////////////////

		const video::VertexBufferPtr& MeshBase::GetVertexBuffer() const
		{
			return m_vertexBuffer;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		const video::IndexBufferPtr& MeshBase::GetIndexBuffer() const
		{
			return m_indexBuffer;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		Topology MeshBase::GetTopology()
		{
			return m_topology;
		}

		/////////////////////////////////////////////////////////////////////////////////////
	}
}