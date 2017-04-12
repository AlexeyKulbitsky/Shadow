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
			wvp.m[1][1] *= -1.0f;
				
			auto transformParams = m_material->GetTransformParams();

			for (const auto& param : transformParams)
			{
				switch (param->GetType())
				{
					case MaterialParamType::MatrixWorld:
						param->Set(m_worldMatrix);
						break;
					case MaterialParamType::MatrixView:
						param->Set(viewMatrix);
						break;
					case MaterialParamType::MatrixProjection:
						param->Set(projectionMatrix);
						break;
					case MaterialParamType::MatrixWorldViewProjection:
						param->Set(wvp);
						break;
				}
			}


			if (m_worldViewdProjectionMatrixParam)
				m_worldViewdProjectionMatrixParam.Set(wvp);
			if (m_worldMatrixParam)
				m_worldMatrixParam.Set(m_worldMatrix);
			if (m_viewMatrixParam)
				m_viewMatrixParam.Set(viewMatrix);
			if (m_projectionMatrixParam)
				m_projectionMatrixParam.Set(projectionMatrix);

			if (m_lightDirectionParam)
				m_lightDirectionParam.Set(math::Vector3f(0.0f, 0.0f, -1.0f));
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
			m_autoGpuParams.resize(pipelinesCount);

			for (size_t i = 0; i < 1; ++i)
			{
				// Assemble vertex input declaration for render command
				const video::RenderPipelinePtr& renderPipeline = m_material->GetRenderPipeline(i);

				video::VertexInputDeclarationPtr inputDeclaration = renderPipeline->GetVertexInputDeclaration()->Clone();
				inputDeclaration->Assemble(*(m_vertexBuffer->GetVertexDeclaration().get()));

				m_vertexDeclaration[i] = inputDeclaration;
				m_gpuParams[i] = video::GpuParams::Create(renderPipeline->GetParamsDescription());
				m_autoGpuParams[i] = video::GpuParams::Create(renderPipeline->GetAutoParamsDescription());

				
				m_autoGpuParams[i]->GetParam("matWorld", m_worldMatrixParam);
				m_autoGpuParams[i]->GetParam("matView", m_viewMatrixParam);
				m_autoGpuParams[i]->GetParam("matProjection", m_projectionMatrixParam);
				m_autoGpuParams[i]->GetParam("matWVP", m_worldViewdProjectionMatrixParam);
				m_autoGpuParams[i]->GetParam("lightDirection", m_lightDirectionParam);

				renderPipeline->Init(inputDeclaration);
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


