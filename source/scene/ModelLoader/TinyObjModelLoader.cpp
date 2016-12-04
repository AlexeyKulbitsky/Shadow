#include "TinyObjModelLoader.h"
#include "../ModelBase.h"
#include "../MeshBase.h"
#include "../video/VertexDeclaration.h"
#include "../../Device.h"
#include "../../video/Sampler.h"
#include "../../video/Uniform.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace sh
{
	namespace scene
	{
		TinyObjModelLoader::TinyObjModelLoader()
		{

		}

		TinyObjModelLoader::~TinyObjModelLoader()
		{

		}

		ModelBase* TinyObjModelLoader::Load(const char* path)
		{
			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string err;

			if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path, "../data/models/nanosuit/"))
			{
				throw std::runtime_error(err);
			}

			ModelBase* model = new ModelBase();

			for (const auto& shape : shapes)
			{
				std::vector<float> vertexArray;
				std::vector<unsigned int> indexArray;
				sh::video::VertexDeclaration* vertexDeclaration = new sh::video::VertexDeclaration();
				size_t verticesCount = 0;

				

				for (const auto& index : shape.mesh.indices)
				{					
					// position
					vertexArray.push_back(attrib.vertices[3 * index.vertex_index + 0]);
					vertexArray.push_back(attrib.vertices[3 * index.vertex_index + 1]);
					vertexArray.push_back(attrib.vertices[3 * index.vertex_index + 2]);
					
					// normal
					//vertexArray.push_back(attrib.normals[3 * index.vertex_index + 0]);
					//vertexArray.push_back(attrib.normals[3 * index.vertex_index + 1]);
					//vertexArray.push_back(attrib.normals[3 * index.vertex_index + 2]);

					// color
					vertexArray.push_back(0.5f);
					vertexArray.push_back(0.5f);
					vertexArray.push_back(0.5f);
										
					// uv
					vertexArray.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
					vertexArray.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);

					indexArray.push_back(indexArray.size());
					verticesCount++;
				}

				sh::video::Attribute positionAttribute(sh::video::AttributeSemantic::POSITION, sh::video::AttributeType::FLOAT, 3U);
				sh::video::Attribute colorAttribute(sh::video::AttributeSemantic::COLOR, sh::video::AttributeType::FLOAT, 3U);
				sh::video::Attribute uvAttribute(sh::video::AttributeSemantic::UV, sh::video::AttributeType::FLOAT, 2U);
				vertexDeclaration->AddAttribute(positionAttribute);	
				vertexDeclaration->AddAttribute(colorAttribute);
				vertexDeclaration->AddAttribute(uvAttribute);

				MeshBase* mesh = new MeshBase();

				sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();

				// Create vertex buffer
				const void* verticesPointer = vertexArray.data();
				size_t verticesDataSize = vertexArray.size() * sizeof(float);
				sh::video::VertexBuffer* vertexBuffer = driver->CreateVertexBuffer(verticesPointer, verticesDataSize);
				vertexBuffer->SetVerticesData(vertexArray);
				vertexBuffer->SetVerticesCount(verticesCount);
				vertexBuffer->SetVertexSize(vertexDeclaration->GetStride());

				// Create index buffer
				const void* indicesPointer = indexArray.data();
				size_t indicesDataSize = indexArray.size() * sizeof(unsigned int);
				sh::video::IndexBuffer* indexBuffer = driver->CreateIndexBuffer(indicesPointer, indicesDataSize);
				indexBuffer->SetIndicesData(indexArray);
				indexBuffer->SetIndexType(sh::video::IndexBuffer::IndexType::UNSIGNED_32_BIT);
				indexBuffer->SetIndicesCount(indexArray.size());
				
				mesh->SetVertexBuffer(vertexBuffer);
				mesh->SetIndexBuffer(indexBuffer);
				mesh->SetVertexDeclaration(vertexDeclaration);
				mesh->SetTopology(sh::video::Topology::TRIANGLE_LIST);

				// Set base material
				int materialId = shape.mesh.material_ids[0];
				String diffuseTexName = materials[materialId].diffuse_texname;
				sh::video::Sampler* diffuseSampler = new sh::video::Sampler();
				diffuseSampler->SetTexureName(diffuseTexName);
				diffuseSampler->SetUsage(video::Sampler::Usage::DIFFUSE_MAP);

				mesh->AddSampler(diffuseSampler);
				
				model->AddMesh(mesh);
			}

			return model;
		}

		ModelBase* TinyObjModelLoader::Load(const std::string &path)
		{
			return Load(path.c_str());
		}
	}
}