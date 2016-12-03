#include "SubEntity.h"
#include "Mesh.h"
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
		SubEntity::SubEntity(Mesh* mesh)
		{
			m_mesh = mesh;

			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			m_renderCommand = driver->CreateRenderCommand();

			// Init render command with geometry
			m_renderCommand->SetVertexBuffer(m_mesh->GetVertexBuffer());
			if (m_mesh->IsUseIndices())
			{
				m_renderCommand->SetIndexBuffer(m_mesh->GetIndexBuffer());
				m_renderCommand->SetUseIndices(true);
			}
			m_renderCommand->GetVertexBuffer()->SetVertexDeclaration(mesh->GetVertexDeclaration());

		}

		void SubEntity::Render()
		{
			sh::video::Uniform* mvp = m_renderCommand->GetShaderProgram()->GetUniformBuffer()->GetUniform("matMVP");
			math::Matrix4f modelMatrix;
			modelMatrix.SetIdentity();
			modelMatrix.SetTranslation(math::Vector3f(0.0f, 0.0f, -10.0f));

			Camera* camera = Device::GetInstance()->GetSceneManager()->GetCamera();
			sh::math::Matrix4f viewMatrix = camera->GetViewMatrix();
			sh::math::Matrix4f projectionMatrix = camera->GetProjectionMatrix();

			sh::math::Matrix4f resultMatrix = projectionMatrix * viewMatrix * modelMatrix;
			resultMatrix.Transpose();

			mvp->Set(resultMatrix);
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			driver->Render(m_renderCommand);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////

		void SubEntity::SetMaterial(sh::video::Material* material)
		{
			m_material = material;
			sh::video::ShaderProgram* shaderProgram = m_material->GetRenderPass(0)->GetShaderProgram();
			m_renderCommand->SetShaderProgram(shaderProgram);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////


	}
}