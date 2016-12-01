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
		Mesh::Mesh()
		{
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			m_renderCommand = driver->CreateRenderCommand();
		}

		/////////////////////////////////////////////////////////////////////////////////////

		Mesh::~Mesh()
		{

		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::Init()
		{
			m_renderCommand->Init();
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::Render()
		{
			sh::video::Uniform* mvp = m_renderCommand->GetShaderProgram()->GetUniformBuffer()->GetUniform("matMVP");
			math::Matrix4f modelMatrix;
			modelMatrix.SetIdentity();
			modelMatrix.SetTranslation(math::Vector3f(0.0f, 0.0f, -10.0f));

			Camera* camera = Device::GetInstance()->GetSceneManager()->GetCamera();
			sh::math::Matrix4f viewMatrix = camera->GetViewMatrix();
			sh::math::Matrix4f projectionMatrix = camera->GetProjectionMatrix();

			sh::math::Matrix4f resultMatrix = projectionMatrix * viewMatrix * modelMatrix;
			//sh::math::Matrix4f resultMatrix = modelMatrix * viewMatrix * projectionMatrix;
			//resultMatrix.Transpose();

			mvp->Set(resultMatrix);
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			driver->Render(m_renderCommand);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::SetVertexDeclaration(sh::video::VertexDeclaration vertexDeclaration)
		{
			m_renderCommand->GetVertexBuffer()->SetVertexDeclaration(vertexDeclaration);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::SetVertexData(std::vector<float> data)
		{
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			sh::video::VertexBuffer* vertexBuffer = driver->CreateVertexBuffer(data.data(), data.size() * sizeof(float));
			m_renderCommand->SetVertexBuffer(vertexBuffer);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::SetIndexData(std::vector<unsigned int> data)
		{
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			sh::video::IndexBuffer* indexBuffer = driver->CreateIndexBuffer(data.data(), data.size() * sizeof(unsigned int));
			m_renderCommand->SetIndexBuffer(indexBuffer);
			m_renderCommand->SetUseIndices(true);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::SetMaterial(sh::video::Material* material)
		{
			m_material = material;
			//sh::video::ShaderProgram* shaderProgram = m_material->GetRenderTechnique()->GetRenderPass(0)->GetShaderProgram();
			sh::video::ShaderProgram* shaderProgram = m_material->GetRenderPass(0)->GetShaderProgram();
			m_renderCommand->SetShaderProgram(shaderProgram);

		}
	}
}


