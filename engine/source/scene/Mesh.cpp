#include "Mesh.h"
#include "MeshBase.h"
#include "../Device.h"
#include "../video/Material.h"
#include "../video/RenderTechnique.h"
#include "../video/ShaderProgram.h"
#include "../video/RenderPass.h"
#include "../video/RenderCommand.h"
#include "../video/UniformBuffer.h"
#include "../video/Uniform.h"
#include "../scene/SceneManager.h"
#include "../scene/Camera.h"

namespace sh
{
	namespace scene
	{
		Mesh::Mesh(MeshBase* meshBase)
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
			wvp.Transpose();
			//driver->GetGlobalUniform(video::GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX)->Set(wvp);

			for (auto renderCommand : m_renderCommands)
			{
				size_t uniformsCount = renderCommand->GetUniformBuffer()->GetUniformsCount();
				for (size_t i = 0; i < uniformsCount; ++i)
				{
					video::Uniform* uniform = renderCommand->GetUniformBuffer()->GetUniform(i);
					if (uniform->GetGlobalUniformName() == video::GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX)
					{
						uniform->Set(wvp);
					}
					else if (uniform->GetGlobalUniformName() == video::GlobalUniformName::MODEL_WORLD_MATRIX)
					{
						uniform->Set(m_worldMatrix);
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

		void Mesh::SetMaterial(sh::video::Material* material)
		{
			m_material = material;	
			size_t passesSize = material->GetRenderPassesCount();
			if (m_renderCommands.size() != passesSize)
			{
				m_renderCommands.resize(passesSize);

				for (size_t i = 0; i < passesSize; ++i)
				{
					sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
					m_renderCommands[i] = driver->CreateRenderCommand();
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
				}			
			}


			for (size_t i = 0, sz = m_material->GetRenderPassesCount(); i < sz; ++i)
			{
				video::RenderPass* renderPass = m_material->GetRenderPass(i);
				m_renderCommands[i]->SetUniformBuffer(renderPass->GetUniformBuffer());

				video::VertexInputDeclaration* inputDeclaration = renderPass->GetVertexInputDeclaration();
				inputDeclaration->Assemble(*(m_meshBase->GetVertexDeclaration()));
				m_renderCommands[i]->SetVertexInputDeclaration(inputDeclaration);
			}			
		}

		/////////////////////////////////////////////////////////////////////////////////////
				
		video::Material* Mesh::GetMaterial()
		{
			return m_material;
		}

		/////////////////////////////////////////////////////////////////////////////////////

	}
}


