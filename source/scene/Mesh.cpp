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
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			m_renderCommand = driver->CreateRenderCommand();
			m_renderCommand->SetVertexBuffer(m_meshBase->GetVertexBuffer());
			if (m_meshBase->IsUseIndices())
			{
				m_renderCommand->SetIndexBuffer(m_meshBase->GetIndexBuffer());
				m_renderCommand->SetUseIndices(true);
			}
			else
			{
				m_renderCommand->SetUseIndices(false);
			}
			m_renderCommand->SetTopology(meshBase->GetTopology());

			m_worldMatrix.SetIdentity();
			m_worldMatrix.SetTranslation(math::Vector3f(0.0f, 0.0f, -10.0f));
		}

		/////////////////////////////////////////////////////////////////////////////////////

		Mesh::~Mesh()
		{

		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::Render()
		{			
			Camera* camera = Device::GetInstance()->GetSceneManager()->GetCamera();
			sh::math::Matrix4f viewMatrix = camera->GetViewMatrix();
			sh::math::Matrix4f projectionMatrix = camera->GetProjectionMatrix();

			video::Driver* driver = Device::GetInstance()->GetDriver();
			math::Matrix4f wvp = projectionMatrix * viewMatrix * m_worldMatrix;
			wvp.Transpose();
			driver->GetGlobalUniform(video::GlobalUniformName::MODEL_WORLD_VIEW_PROJECTION_MATRIX)->Set(wvp);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::SetMaterial(sh::video::Material* material)
		{
			m_material = material;			

			video::RenderPass* renderPass = m_material->GetRenderPass(0);
			m_renderCommand->SetUniformBuffer(renderPass->GetUniformBuffer()->Clone());

			video::VertexInputDeclaration* inputDeclaration = renderPass->GetVertexInputDeclaration();
			inputDeclaration->Assemble(*(m_meshBase->GetVertexDeclaration()));
			m_renderCommand->SetVertexInputDeclaration(inputDeclaration);

		}

		/////////////////////////////////////////////////////////////////////////////////////
				
		video::Material* Mesh::GetMaterial()
		{
			return m_material;
		}

		/////////////////////////////////////////////////////////////////////////////////////

	}
}


