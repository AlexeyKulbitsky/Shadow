#include "Mesh.h"
#include "MeshBase.h"
#include "../Device.h"
#include "../video/Material.h"
#include "../video/RenderTechnique.h"
#include "../video/ShaderProgram.h"
#include "../video/RenderPipeline.h"
#include "../video/VertexDeclaration.h"
#include "../video/UniformBuffer.h"
#include "../video/VertexBuffer.h"
#include "../video/Uniform.h"
#include "../scene/SceneManager.h"
#include "../scene/Camera.h"
#include "../video/GpuParams.h"
#include "../video/GpuParamsDescription.h"
#include "../video/MaterialParam.h"
#include "../video/Renderable.h"

#include "../video/Vulkan/VulkanRenderCommand.h"
#include "../video/Vulkan/VulkanRenderPipeline.h"

namespace sh
{
	namespace scene
	{
		Mesh::Mesh(MeshBasePtr meshBase)
			: m_meshBase(meshBase)
		{
			m_worldMatrix.SetIdentity();

			m_renderable.reset(new video::Renderable());
			m_renderable->m_vertexBuffer = meshBase->GetVertexBuffer();
			m_renderable->m_indexBuffer = meshBase->GetIndexBuffer();
			m_renderable->m_matrix = &m_worldMatrix;
			m_renderable->m_parent = this;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		Mesh::~Mesh()
		{
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::SetWorldMatrix(const math::Matrix4f& matrix)
		{
			m_worldMatrix = matrix;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::SetMaterial(const sh::video::MaterialPtr& material)
		{
			m_material = material;

			// Assemble vertex input declaration for render command
			const video::RenderPipelinePtr& renderPipeline = m_material->GetRenderPipeline();

			video::VertexInputDeclarationPtr inputDeclaration = renderPipeline->GetVertexInputDeclaration()->Clone();
			inputDeclaration->Assemble(*(m_renderable->GetVertexBuffer()->GetVertexDeclaration().get()));
			
			m_renderable->m_vertexDeclaration = inputDeclaration;
			m_renderable->m_transfromsGpuParams = video::GpuParams::Create(renderPipeline->GetAutoParamsInfo());

			m_renderable->m_transformParams.reset(new video::MaterialParams(m_renderable->m_transfromsGpuParams));
			
			renderPipeline->Init(inputDeclaration);			
		}

		/////////////////////////////////////////////////////////////////////////////////////
				
		const video::MaterialPtr& Mesh::GetMaterial()
		{
			return m_material;
		}

		/////////////////////////////////////////////////////////////////////////////////////

	}
}


