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
			: m_useIndices(false)
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
			resultMatrix.Transpose();

			mvp->Set(resultMatrix);
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			driver->Render(m_renderCommand);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::SetVertexDeclaration(sh::video::VertexDeclaration vertexDeclaration)
		{
			m_renderCommand->GetVertexBuffer()->SetVertexDeclaration(vertexDeclaration);
			m_vertexDeclaration = vertexDeclaration;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::SetVertexData(std::vector<float> data)
		{
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			m_vertexBuffer = driver->CreateVertexBuffer(data.data(), data.size() * sizeof(float));
			m_renderCommand->SetVertexBuffer(m_vertexBuffer);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::SetIndexData(std::vector<unsigned int> data)
		{
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			m_indexBuffer = driver->CreateIndexBuffer(data.data(), data.size() * sizeof(unsigned int));
			m_useIndices = true;
			m_renderCommand->SetIndexBuffer(m_indexBuffer);
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

		/////////////////////////////////////////////////////////////////////////////////////

		video::VertexBuffer* Mesh::GetVertexBuffer()
		{
			return m_vertexBuffer;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		video::IndexBuffer* Mesh::GetIndexBuffer()
		{
			return m_indexBuffer;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		video::VertexDeclaration Mesh::GetVertexDeclaration()
		{
			return m_vertexDeclaration;
		}
	}
}


