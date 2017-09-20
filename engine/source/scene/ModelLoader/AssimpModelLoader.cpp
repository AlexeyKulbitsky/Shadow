#include "AssimpModelLoader.h"
#if defined (SHADOW_ASSIMP_LOADER)
#include "../ModelBase.h"
#include "../MeshBase.h"
#include "../../video/VertexBuffer.h"
#include "../../video/IndexBuffer.h"
#include "../../video/VertexDeclaration.h"
#include "../../Device.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace sh
{

namespace scene
{

	ModelBasePtr AssimpModelLoader::Load(const std::string &path)
	{
		io::File file = io::FileSystem::GetInstance()->LoadFile(path);

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(file.GetData().data(), file.GetData().size(), 
			aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			SH_ASSERT(0, "ERROR::ASSIMP::%s", importer.GetErrorString());
			return nullptr;
		}

		ModelBasePtr model(new ModelBase());

		ProcessNode(scene->mRootNode, scene, model);

		return model;
	}

	void AssimpModelLoader::ProcessNode(aiNode* node, const aiScene* scene, const ModelBasePtr& model)
	{
		// Process all the node's meshes (if any)
		for (u32 i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			MeshBasePtr childMesh = ProcessMesh(mesh, scene);

			model->AddMesh(childMesh);
		}
		// Then do the same for each of its children
		for (u32 i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene, model);
		}
	}

	MeshBasePtr AssimpModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<float> vertexArray;
		std::vector<unsigned int> indexArray;
		math::AABBf boundingBox(math::Vector3f(0.0f));

		const bool hasNormals = mesh->HasNormals();
		const bool hasUVs = mesh->HasTextureCoords(0);
		const bool hasTangentsBitangents = mesh->HasTangentsAndBitangents();
		
		for (u32 i = 0; i < mesh->mNumVertices; i++)
		{
			// Positions
			vertexArray.push_back(mesh->mVertices[i].x);
			vertexArray.push_back(mesh->mVertices[i].y);
			vertexArray.push_back(mesh->mVertices[i].z);

			boundingBox.AddPoint(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);

			// Normals
			if (hasNormals)
			{
				vertexArray.push_back(mesh->mNormals[i].x);
				vertexArray.push_back(mesh->mNormals[i].y);
				vertexArray.push_back(mesh->mNormals[i].z);
			}

			

			// Color
			//if (mesh->HasVertexColors(0))
			//{
			//	vertexArray.push_back(mesh->mColors[0][i].r);
			//	vertexArray.push_back(mesh->mColors[0][i].g);
			//	vertexArray.push_back(mesh->mColors[0][i].b);
			//}

			vertexArray.push_back(0.0f);
			vertexArray.push_back(0.0f);
			vertexArray.push_back(0.0f);

			// UV
			if (hasUVs)
			{
				vertexArray.push_back(mesh->mTextureCoords[0][i].x);
				vertexArray.push_back(mesh->mTextureCoords[0][i].y);
			}

			// Tangents and bitangents
			if (hasTangentsBitangents)
			{
				// Tangents
				vertexArray.push_back(mesh->mTangents[i].x);
				vertexArray.push_back(mesh->mTangents[i].y);
				vertexArray.push_back(mesh->mTangents[i].z);

				// Bitangents
				vertexArray.push_back(mesh->mBitangents[i].x);
				vertexArray.push_back(mesh->mBitangents[i].y);
				vertexArray.push_back(mesh->mBitangents[i].z);
			}
		}

		for (u32 i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (u32 j = 0; j < face.mNumIndices; j++)
				indexArray.push_back(face.mIndices[j]);
		}


		sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
		size_t verticesCount = mesh->mNumVertices;

		sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
		vertexDeclaration->AddAttribute(positionAttribute);
		if (hasNormals)
		{
			sh::video::Attribute normalAttribute(AttributeSemantic::NORMAL, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(normalAttribute);
		}
		sh::video::Attribute colorAttribute(AttributeSemantic::COLOR, AttributeType::FLOAT, 3U);
		vertexDeclaration->AddAttribute(colorAttribute);
		if (hasUVs)
		{
			sh::video::Attribute uvAttribute(AttributeSemantic::UV, AttributeType::FLOAT, 2U);
			vertexDeclaration->AddAttribute(uvAttribute);
		}
		if (hasTangentsBitangents)
		{
			sh::video::Attribute tangentAttribute(AttributeSemantic::TANGENT, AttributeType::FLOAT, 3U);
			sh::video::Attribute binormalAttribute(AttributeSemantic::BINORMAL, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(tangentAttribute);
			vertexDeclaration->AddAttribute(binormalAttribute);
		}
		
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

		MeshBasePtr result(new MeshBase());

		result->SetVertexBuffer(vertexBuffer);
		result->SetIndexBuffer(indexBuffer);
		result->SetTopology(Topology::TOP_TRIANGLE_LIST);
		result->SetBoundingBox(boundingBox);
		result->SetName(mesh->mName.C_Str());
		
		return result;
	}

} // scene

} // sh

#endif