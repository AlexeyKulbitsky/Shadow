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
			//wvp.Transpose();
			//math::Matrix4f wvp = m_worldMatrix * viewMatrix * projectionMatrix.GetTransposed();
			
			size_t pipelinesCount = m_material->GetRenderPipelinesCount();
			for (size_t i = 0U; i < pipelinesCount; ++i)
			{
				size_t autoUniformsCount = m_renderCommands[i]->GetAutoUniformsBatch()->m_uniforms.size();
				for (size_t uniIdx = 0; uniIdx < autoUniformsCount; ++uniIdx)
				{
					video::Uniform* uniform = m_renderCommands[i]->GetAutoUniformsBatch()->m_uniforms[uniIdx];
					switch (uniform->GetGlobalUniformName())
					{
						case video::GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX:
							uniform->Set(wvp);
							break;
						case video::GlobalUniformName::MODEL_WORLD_MATRIX:
							//uniform->Set(m_worldMatrix.GetTransposed());
							uniform->Set(m_worldMatrix);
							break;
						default:
							break;
					}
				}
			}		
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
			if (m_renderCommands.size() != pipelinesCount)
			{
				m_renderCommands.resize(pipelinesCount);		
			}

			for (size_t i = 0; i < pipelinesCount; ++i)
			{
				// Update render command with buffer data
				if (!m_renderCommands[i])
				{
					sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
					m_renderCommands[i] = driver->CreateRenderCommand();
				}
				
				m_renderCommands[i]->SetVertexBuffer(m_meshBase->GetVertexBuffer());
				if (m_meshBase->IsUseIndices())
				{
					m_renderCommands[i]->SetIndexBuffer(m_meshBase->GetIndexBuffer());
					m_renderCommands[i]->SetUseIndices(true);
				}
				else
				{
					m_renderCommands[i]->SetUseIndices(false);
				}
				m_renderCommands[i]->SetTopology(m_meshBase->GetTopology());

				// Assemble vertex input declaration for render command
				const video::RenderPipelinePtr& renderPipeline = m_material->GetRenderPipeline(i);

				video::VertexInputDeclaration* inputDeclaration = renderPipeline->GetVertexInputDeclaration();
				inputDeclaration->Assemble(*(m_renderCommands[i]->GetVertexBuffer()->GetVertexDeclaration().get()));
				m_renderCommands[i]->SetVertexInputDeclaration(inputDeclaration);
				const video::UniformsBatchPtr& uniformsBatch = renderPipeline->GetUniformBuffer()->GetAutoUniformsBatch()->Clone();
				m_renderCommands[i]->SetAutoUniformsBatch(uniformsBatch);

				sh::video::VulkanRenderCommand* rc = dynamic_cast<sh::video::VulkanRenderCommand*>(m_renderCommands[i].get());
				if (rc)
				{
					sh::video::VulkanRenderPipeline* rp = dynamic_cast<sh::video::VulkanRenderPipeline*>(renderPipeline.get());
					rp->Init();
					rc->SetPipeline(rp);
				}

				m_renderCommands[i]->Init();
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


