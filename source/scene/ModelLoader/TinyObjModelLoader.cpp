#include "TinyObjModelLoader.h"
#include "../Model.h"
#include "../Mesh.h"
#include "../video/VertexDeclaration.h"
#include "../../Device.h"

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

		Model* TinyObjModelLoader::Load(const char* path)
		{
			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			std::string err;

			if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path))
			{
				throw std::runtime_error(err);
			}

			Model* model = new Model();

			for (const auto& shape : shapes)
			{
				std::vector<float> vertexArray;
				sh::video::VertexDeclaration vertexDeclaration;

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
										
					// uv
					vertexArray.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
					vertexArray.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);
									
				}

				sh::video::Attribute positionAttribute(sh::video::AttributeSemantic::POSITION, sh::video::AttributeType::FLOAT, 3U);
				sh::video::Attribute uvAttribute(sh::video::AttributeSemantic::UV, sh::video::AttributeType::FLOAT, 2U);
				vertexDeclaration.AddAttribute(positionAttribute);				
				vertexDeclaration.AddAttribute(uvAttribute);

				Mesh* mesh = new Mesh();	
				mesh->SetVertexData(vertexArray);
				mesh->SetVertexDeclaration(vertexDeclaration);

				model->AddMesh(mesh);
			}

			return model;
		}

		Model* TinyObjModelLoader::Load(const std::string &path)
		{
			return Load(path.c_str());
		}
	}
}