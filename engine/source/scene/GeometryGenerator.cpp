#include "GeometryGenerator.h"
#include "../video/VertexDeclaration.h"
#include "../video/VertexBuffer.h"
#include "../Device.h"
#include "../scene/MeshBase.h"
#include "../scene/ModelBase.h"
#include "../scene/Model.h"
#include "../scene/Mesh.h"
#include "../resources/ResourceManager.h"
#include "../video/Material.h"

namespace sh
{
	namespace scene
	{
		Model* GeometryGenerator::GetLineModel()
		{
			// Set vertex data
			std::vector<float> vertexArray;
			size_t verticesCount = 6;
			vertexArray.resize(verticesCount);
			vertexArray[0] = 0.0f;
			vertexArray[1] = -1.5f;
			vertexArray[2] = 0.0f;

			vertexArray[3] = 0.0f;
			vertexArray[4] = 1.5f;
			vertexArray[5] = 0.0f;

			// Create vertex declaration
			sh::video::VertexDeclaration* vertexDeclaration = new sh::video::VertexDeclaration();
			sh::video::Attribute positionAttribute(sh::video::AttributeSemantic::POSITION, sh::video::AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			// Create vertex buffer
			const void* verticesPointer = vertexArray.data();
			size_t verticesDataSize = vertexArray.size() * sizeof(float);
			sh::video::VertexBuffer* vertexBuffer = Device::GetInstance()->GetDriver()->CreateVertexBuffer(verticesPointer, verticesDataSize);
			vertexBuffer->SetVerticesData(vertexArray);
			vertexBuffer->SetVerticesCount(verticesCount);
			vertexBuffer->SetVertexSize(vertexDeclaration->GetStride());

			sh::scene::MeshBase* mesh = new sh::scene::MeshBase();

			mesh->SetVertexBuffer(vertexBuffer);
			mesh->SetVertexDeclaration(vertexDeclaration);
			mesh->SetTopology(sh::video::Topology::LINE_LIST);

			sh::scene::ModelBase* model = new sh::scene::ModelBase();
			model->AddMesh(mesh);

			sh::scene::Model* resultModel = new sh::scene::Model(model);

			sh::video::Material* material = new sh::video::Material();			
			sh::video::RenderTechnique* rt = Device::GetInstance()->GetResourceManager()->GetRenderTechnique("base.xml");			
			material->SetRenderTechnique(rt);

			size_t meshesCount = resultModel->GetMeshesCount();
			for (size_t i = 0; i < meshesCount; ++i)
			{
				resultModel->GetMesh(i)->SetMaterial(material);
			}

			return resultModel;
		}
	}
}