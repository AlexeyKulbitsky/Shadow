#include "Mesh.h"
#include "../Device.h"
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
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Mesh::Render()
		{

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
	}
}


