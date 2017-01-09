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
		Model* GeometryGenerator::GetLineModel(const math::Vector3f& start, const math::Vector3f& end)
		{
			// Set vertex data
			std::vector<float> vertexArray;
			size_t verticesCount = 6;
			vertexArray.resize(verticesCount);
			vertexArray[0] = start.x;
			vertexArray[1] = start.y;
			vertexArray[2] = start.z;

			vertexArray[3] = end.x;
			vertexArray[4] = end.y;
			vertexArray[5] = end.z;

			// Create vertex declaration
			sh::video::VertexDeclaration* vertexDeclaration = new sh::video::VertexDeclaration();
			sh::video::Attribute positionAttribute(sh::video::AttributeSemantic::POSITION, sh::video::AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			// Create vertex buffer
			const void* verticesPointer = vertexArray.data();
			size_t verticesDataSize = vertexArray.size() * sizeof(float);
			sh::video::VertexBuffer* vertexBuffer = Device::GetInstance()->GetDriver()->CreateVertexBuffer(verticesPointer, verticesDataSize);
			vertexBuffer->SetVerticesData(vertexArray);
			vertexBuffer->SetVerticesCount(verticesCount / 3);
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

		//////////////////////////////////////////////////////////////////

		Model* GeometryGenerator::GetConeModel(const math::Vector3f& start, const math::Vector3f& axis, const math::Vector3f& vtx, const math::Vector3f& vty)
		{
			float r = 0.25f;
			float height = 1.0f;
			std::vector<float> vertexArray;
			
			for (size_t i = 0; i <= 30; ++i)
			{
				math::Vector3f pt;
				pt = vtx * sh::math::Cos(((2 * sh::math::k_pi) / 30.0f) * i) * r;
				pt += vty * sh::math::Sin(((2 * sh::math::k_pi) / 30.0f) * i) * r;
				//pt += axis * height;
				pt += start;
				vertexArray.push_back(pt.x);
				vertexArray.push_back(pt.y);
				vertexArray.push_back(pt.z);

				pt = vtx * sh::math::Cos(((2 * sh::math::k_pi) / 30.0f) * (i + 1)) * r;
				pt += vty * sh::math::Sin(((2 * sh::math::k_pi) / 30.0f) * (i + 1)) * r;
				//pt += axis * height;
				pt += start;
				vertexArray.push_back(pt.x);
				vertexArray.push_back(pt.y);
				vertexArray.push_back(pt.z);

				vertexArray.push_back(start.x + axis.x * height);
				vertexArray.push_back(start.y + axis.y * height);
				vertexArray.push_back(start.z + axis.z * height);

				

				/*
				vertexArray.push_back(start.x + 0.0f);
				vertexArray.push_back(start.y + height);
				vertexArray.push_back(start.z + 0.0f);

				float x1 = start.x + sh::math::Cos(((2 * sh::math::k_pi) / 30.0f) * i) * r;
				float y1 = start.y + 0.0f;
				float z1 = start.z + sh::math::Sin(((2 * sh::math::k_pi) / 30.0f) * i) * r;
				vertexArray.push_back(x1);
				vertexArray.push_back(y1);
				vertexArray.push_back(z1);

				float x2 = start.x + sh::math::Cos(((2 * sh::math::k_pi) / 30.0f) * (i + 1)) * r;
				float y2 = start.y + 0.0f;
				float z2 = start.z + sh::math::Sin(((2 * sh::math::k_pi) / 30.0f) * (i + 1)) * r;
				vertexArray.push_back(x2);
				vertexArray.push_back(y2);
				vertexArray.push_back(z2);	

				vertexArray.push_back(start.x);
				vertexArray.push_back(start.y);
				vertexArray.push_back(start.z);

				vertexArray.push_back(x2);
				vertexArray.push_back(y2);
				vertexArray.push_back(z2);	

				vertexArray.push_back(x1);
				vertexArray.push_back(y1);
				vertexArray.push_back(z1);
				*/
			}

			size_t verticesCount = vertexArray.size() / 3;

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
			mesh->SetTopology(sh::video::Topology::TRIANGLE_LIST);

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