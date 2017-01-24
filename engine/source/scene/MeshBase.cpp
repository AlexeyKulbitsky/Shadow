#include "MeshBase.h"
#include "../Device.h"
#include "../video/Material.h"
#include "../video/RenderTechnique.h"
#include "../video/ShaderProgram.h"
#include "../video/RenderPipeline.h"
#include "../video/RenderCommand.h"
#include "../video/UniformBuffer.h"
#include "../video/Uniform.h"
#include "../scene/SceneManager.h"
#include "../scene/Camera.h"

namespace sh
{
	namespace scene
	{
		MeshBase::MeshBase()
			: m_useIndices(false)
		{
		}

		/////////////////////////////////////////////////////////////////////////////////////

		MeshBase::~MeshBase()
		{
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void MeshBase::SetVertexDeclaration(sh::video::VertexDeclaration* vertexDeclaration)
		{
			m_vertexDeclaration = vertexDeclaration;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void MeshBase::SetTopology(sh::video::Topology topology)
		{
			m_topology = topology;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void MeshBase::SetVertexBuffer(video::VertexBuffer* vertexBuffer)
		{
			m_vertexBuffer = vertexBuffer;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void MeshBase::SetIndexBuffer(video::IndexBuffer* indexBuffer)
		{
			m_indexBuffer = indexBuffer;
			m_useIndices = true;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void MeshBase::AddUniform(video::Uniform* uniform)
		{
			m_uniforms.push_back(uniform);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void MeshBase::AddSampler(video::Sampler* sampler)
		{
			m_samplers.push_back(sampler);
		}

		size_t MeshBase::GetUniformsCount() const
		{
			return m_uniforms.size();
		}

		/////////////////////////////////////////////////////////////////////////////////////

		size_t MeshBase::GetSamplersCount() const
		{
			return m_samplers.size();
		}

		/////////////////////////////////////////////////////////////////////////////////////

		video::Uniform* MeshBase::GetUniform(size_t index)
		{
			return m_uniforms[index];
		}

		/////////////////////////////////////////////////////////////////////////////////////

		video::Sampler* MeshBase::GetSampler(size_t index)
		{
			return m_samplers[index];
		}

		/////////////////////////////////////////////////////////////////////////////////////

		video::VertexBuffer* MeshBase::GetVertexBuffer()
		{
			return m_vertexBuffer;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		video::IndexBuffer* MeshBase::GetIndexBuffer()
		{
			return m_indexBuffer;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		video::VertexDeclaration* MeshBase::GetVertexDeclaration()
		{
			return m_vertexDeclaration;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		video::Topology MeshBase::GetTopology()
		{
			return m_topology;
		}
	}
}