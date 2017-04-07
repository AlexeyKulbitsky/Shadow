#include "Mesh.h"
#include "MeshBase.h"
#include "../Device.h"
#include "../video/Material.h"
#include "../video/RenderTechnique.h"
#include "../video/ShaderProgram.h"
#include "../video/RenderPipeline.h"
#include "../video/RenderPipeline.h"
#include "../video/VertexDeclaration.h"
#include "../video/UniformBuffer.h"
#include "../video/VertexBuffer.h"
#include "../video/Uniform.h"
#include "../scene/SceneManager.h"
#include "../scene/Camera.h"
#include "../video/GpuParams.h"

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
			m_vertexBuffer = meshBase->GetVertexBuffer();
			m_indexBuffer = meshBase->GetIndexBuffer();
		}

		/////////////////////////////////////////////////////////////////////////////////////

		Mesh::~Mesh()
		{
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::UpdateTransformationUniforms()
		{			
			Camera* camera = Device::GetInstance()->GetSceneManager()->GetCamera();
			sh::math::Matrix4f viewMatrix = camera->GetViewMatrix();
			sh::math::Matrix4f projectionMatrix = camera->GetProjectionMatrix();
			
			video::Driver* driver = Device::GetInstance()->GetDriver();
			math::Matrix4f wvp = projectionMatrix * viewMatrix * m_worldMatrix;
				
			m_matrices[0].Set(wvp);
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
			size_t pipelinesCount = m_material->GetRenderPipelinesCount();
			
			m_vertexDeclaration.resize(pipelinesCount);
			m_gpuParams.resize(pipelinesCount);
			m_matrices.resize(pipelinesCount);

			for (size_t i = 0; i < 1; ++i)
			{
				// Assemble vertex input declaration for render command
				const video::RenderPipelinePtr& renderPipeline = m_material->GetRenderPipeline(i);

				const video::VertexInputDeclarationPtr& inputDeclaration = renderPipeline->GetVertexInputDeclaration();
				inputDeclaration->Assemble(*(m_vertexBuffer->GetVertexDeclaration().get()));

				m_vertexDeclaration[i] = inputDeclaration;
				m_gpuParams[i] = video::GpuParams::Create(renderPipeline->GetGpuParamsDescription());
				m_gpuParams[i]->GetParam("matMVP", m_matrices[i]);

				renderPipeline->Init();
			}			
		}

		/////////////////////////////////////////////////////////////////////////////////////
				
		const video::MaterialPtr& Mesh::GetMaterial()
		{
			return m_material;
		}

		/////////////////////////////////////////////////////////////////////////////////////

	}
}


