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
			wvp.Transpose();

			for (auto renderCommand : m_renderCommands)
			{
				size_t autoUniformsCount = renderCommand->GetUniformBuffer()->GetAutoUniformsCount();
								
				for (size_t i = 0; i < autoUniformsCount; ++i)
				{
					video::Uniform* uniform = renderCommand->GetUniformBuffer()->GetAutoUniform(i);
					switch (uniform->GetGlobalUniformName())
					{
						case video::GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX:
							uniform->Set(wvp);
							break;
						case video::GlobalUniformName::MODEL_WORLD_MATRIX:
							uniform->Set(m_worldMatrix.GetTransposed());
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
			size_t passesSize = material->GetRenderPipelinesCount();
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


			for (size_t i = 0, sz = m_material->GetRenderPipelinesCount(); i < sz; ++i)
			{
				video::RenderPipeline* renderPipeline = m_material->GetRenderPipeline(i);
				m_renderCommands[i]->SetUniformBuffer(renderPipeline->GetUniformBuffer().get());

				video::VertexInputDeclaration* inputDeclaration = renderPipeline->GetVertexInputDeclaration();
				inputDeclaration->Assemble(*(m_renderCommands[i]->GetVertexBuffer()->GetVertexDeclaration()));
				m_renderCommands[i]->SetVertexInputDeclaration(inputDeclaration);
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


