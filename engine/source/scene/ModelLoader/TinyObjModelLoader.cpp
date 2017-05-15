#include "TinyObjModelLoader.h"
#include "../ModelBase.h"
#include "../MeshBase.h"
#include "../../video/VertexBuffer.h"
#include "../../video/IndexBuffer.h"
#include "../../video/VertexDeclaration.h"
#include "../../Device.h"
#include "../../video/Sampler.h"

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

		ModelBasePtr TinyObjModelLoader::Load(const std::string &filename)
		{
			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string err;

			//String fullPath(path);
			//String::size_type pos = String(path).find_last_of("\\/");
			//String materialFolder = String(fullPath).substr(0, pos) + "/";

			io::File file = io::FileSystem::GetInstance()->LoadFile(filename);
			std::stringstream stringBuffer;
			stringBuffer << file.GetData().data();

			bool res = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &stringBuffer);

			SH_ASSERT(res, "Can not load model %s", filename.c_str());
			
			//if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path, materialFolder.c_str()))
			//{
			//	throw std::runtime_error(err);
			//}

			ModelBasePtr model(new ModelBase());

			for (const auto& shape : shapes)
			{
				std::vector<float> vertexArray;
				std::vector<unsigned int> indexArray;
				sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
				size_t verticesCount = 0;

				

				for (const auto& index : shape.mesh.indices)
				{					
					// position
					vertexArray.push_back(attrib.vertices[3 * index.vertex_index + 0]);
					vertexArray.push_back(attrib.vertices[3 * index.vertex_index + 1]);
					vertexArray.push_back(attrib.vertices[3 * index.vertex_index + 2]);
					
					// normal
					vertexArray.push_back(attrib.normals[3 * index.normal_index + 0]);
					vertexArray.push_back(attrib.normals[3 * index.normal_index + 1]);
					vertexArray.push_back(attrib.normals[3 * index.normal_index + 2]);

					// color
					vertexArray.push_back(0.5f);
					vertexArray.push_back(0.5f);
					vertexArray.push_back(0.5f);
										
					// uv
					vertexArray.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
					vertexArray.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);
					//vertexArray.push_back(0.0f);
					//vertexArray.push_back(0.0f);

					indexArray.push_back(indexArray.size());
					verticesCount++;
				}

				sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
				sh::video::Attribute normalAttribute(AttributeSemantic::NORMAL, AttributeType::FLOAT, 3U);
				sh::video::Attribute colorAttribute(AttributeSemantic::COLOR, AttributeType::FLOAT, 3U);
				sh::video::Attribute uvAttribute(AttributeSemantic::UV, AttributeType::FLOAT, 2U);
				vertexDeclaration->AddAttribute(positionAttribute);	
				vertexDeclaration->AddAttribute(normalAttribute);
				vertexDeclaration->AddAttribute(colorAttribute);
				vertexDeclaration->AddAttribute(uvAttribute);

				MeshBasePtr mesh(new MeshBase());

				sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();

				// Create vertex buffer
				const void* verticesPointer = vertexArray.data();
				size_t verticesDataSize = vertexArray.size() * sizeof(float);
				video::VertexBufferDecription desc;
				desc.usage = USAGE_STATIC;
				sh::video::VertexBufferPtr vertexBuffer = video::VertexBuffer::Create(desc);
				vertexBuffer->SetData(0, verticesDataSize, verticesPointer);
				vertexBuffer->SetVerticesCount(verticesCount);
				vertexBuffer->SetVertexSize(vertexDeclaration->GetStride());
				vertexBuffer->SetVertexDeclaration(vertexDeclaration);

				// Create index buffer
				const void* indicesPointer = indexArray.data();
				size_t indicesDataSize = indexArray.size() * sizeof(unsigned int);
				video::IndexBufferDescription indexDesc;
				indexDesc.indexType = IT_32_BIT;
				indexDesc.usage = USAGE_STATIC;
				sh::video::IndexBufferPtr indexBuffer = video::IndexBuffer::Create(indexDesc);
				indexBuffer->SetData(0, indicesDataSize, indicesPointer);
				indexBuffer->SetIndicesCount(indexArray.size());
				
				mesh->SetVertexBuffer(vertexBuffer);
				mesh->SetIndexBuffer(indexBuffer);
				mesh->SetTopology(Topology::TOP_TRIANGLE_LIST);

				// Set base material
				int materialId = shape.mesh.material_ids[0];
				if (materialId >= 0)
				{
					String diffuseTexName = materials[materialId].diffuse_texname;
				}
				
				
				model->AddMesh(mesh);
			}

			return model;
		}

		ModelBasePtr TinyObjModelLoader::Load(const char* path)
		{
			return Load(String(path));
		}
	}
}