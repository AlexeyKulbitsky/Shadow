#include "Mesh.h"
#include "../Device.h"
#include "../video/Material.h"
#include "../video/RenderTechnique.h"
#include "../video/RenderPass.h"
#include "../video/RenderCommand.h"

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
			sh::video::ShaderProgram* shaderProgram = m_material->GetRenderTechnique()->GetRenderPass(0)->GetShaderProgram();
			m_renderCommand->SetShaderProgram(shaderProgram);
		}
	}
}


