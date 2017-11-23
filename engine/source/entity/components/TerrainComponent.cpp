#include "TerrainComponent.h"

#include "../../video/Image.h"
#include "../../video/VertexDeclaration.h"
#include "../../video/VertexBuffer.h"
#include "../../video/IndexBuffer.h"
#include "../../video/Material.h"
#include "../../scene/MeshBase.h"
#include "../../scene/ModelBase.h"
#include "../../scene/Mesh.h"
#include "../../scene/Model.h"
#include "../../Device.h"
#include "../../resources/ResourceManager.h"
#include "../../serialization/ObjectFactory.h"

namespace sh
{

	TerrainComponent::~TerrainComponent()
	{

	}

	////////////////////////////////////////////////////////////////////////////////

	void TerrainComponent::RegisterObject()
	{
		ObjectFactory::GetInstance()->RegisterFactory<TerrainComponent>();
		S_ACCESSOR_PROPERTY("Heightmap", GetHeightmapProperty, SetHeightmapProperty);
	}

	////////////////////////////////////////////////////////////////////////////////

	void TerrainComponent::SetHeightmapProperty(const ResourceRef& value)
	{
		video::Image heightmap;
		heightmap.Load(value.name);
		m_heightmapName = value.name;

		SetHightMapFromImage(heightmap);
	}

	////////////////////////////////////////////////////////////////////////////////

	ResourceRef TerrainComponent::GetHeightmapProperty() const
	{
		return ResourceRef("Textures", m_heightmapName);
	}

	////////////////////////////////////////////////////////////////////////////////

	void TerrainComponent::SetHightMapFromImage(const video::Image& image)
	{
		const u32 width = image.GetWidth();
		const u32 height = image.GetHeight();
		const float step = 1.0f;

		std::vector<float> vertexArray;
		std::vector<unsigned int> indexArray;
		u32 verticesCount = 0U;

		for (u32 i = 0U; i < height; ++i)
		{
			for (u32 j = 0U; j < width; ++j)
			{
				video::Color color = image.GetPixel(i, j);

				const float x = j * step;
				const float y = color.red * 50.0f;
				const float z = i * step;

				// Positions
				vertexArray.push_back(x);
				vertexArray.push_back(y);
				vertexArray.push_back(z);

				// Normals
				vertexArray.push_back(0.0f);
				vertexArray.push_back(0.0f);
				vertexArray.push_back(0.0f);

				verticesCount++;
			}
		}

		for (u32 i = 0U; i < height - 1; ++i)
		{
			for (u32 j = 0U; j < width - 1; ++j)
			{
				indexArray.push_back(i * width + j);
				indexArray.push_back((i + 1) * width + j);
				indexArray.push_back(i * width + j + 1);

				indexArray.push_back(i * width + j + 1);
				indexArray.push_back((i + 1) * width + j);
				indexArray.push_back((i + 1) * width + j + 1);
			}
		}

		sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());

		sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
		sh::video::Attribute normalAttribute(AttributeSemantic::NORMAL, AttributeType::FLOAT, 3U);
		vertexDeclaration->AddAttribute(positionAttribute);
		vertexDeclaration->AddAttribute(normalAttribute);

		CalculateNormals(vertexArray, indexArray, vertexDeclaration);


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

		scene::MeshBasePtr meshBase(new scene::MeshBase());

		meshBase->SetVertexBuffer(vertexBuffer);
		meshBase->SetIndexBuffer(indexBuffer);
		meshBase->SetTopology(TOP_TRIANGLE_LIST);

		scene::ModelBasePtr modelBase(new scene::ModelBase());
		modelBase->AddMesh(meshBase);

		m_model.reset(new scene::Model(modelBase));

		//video::MaterialPtr material(new video::Material());
		//material->SetRenderTechnique("terrain.rt");

		video::MaterialPtr material = Device::GetInstance()->GetResourceManager()->GetMaterial("terrain.mat");

		m_model->SetMaterial(material);
	}

	////////////////////////////////////////////////////////////////////////////////

	void TerrainComponent::CalculateNormals(std::vector<float>& vertexArray,
											const std::vector<u32>& indexArray,
											const video::VertexDeclarationPtr& vertexDeclaration)
	{
		const u32 stride = vertexDeclaration->GetStride() / sizeof(float);
		auto posAttr = vertexDeclaration->GetAttribute(AttributeSemantic::POSITION);
		auto normAttr = vertexDeclaration->GetAttribute(AttributeSemantic::NORMAL);

		for (u32 i = 0; i < indexArray.size() - 3; i += 3)
		{
			const u32 i0 = indexArray[i];
			const u32 i1 = indexArray[i + 1];
			const u32 i2 = indexArray[i + 2];

			u32 index = i0 * stride + posAttr->offset / (u32)posAttr->type;
			math::Vector3f p0;
			p0.x = vertexArray[index];
			p0.y = vertexArray[index + 1];
			p0.z = vertexArray[index + 2];

			index = i1 * stride + posAttr->offset / (u32)posAttr->type;
			math::Vector3f p1;
			p1.x = vertexArray[index];
			p1.y = vertexArray[index + 1];
			p1.z = vertexArray[index + 2];

			index = i2 * stride + posAttr->offset / (u32)posAttr->type;
			math::Vector3f p2;
			p2.x = vertexArray[index];
			p2.y = vertexArray[index + 1];
			p2.z = vertexArray[index + 2];

			math::Vector3f normal = (p0 - p1).Cross(p2 - p1);
			normal.Normalize();

			index = i0 * stride + normAttr->offset / (u32)normAttr->type;
			vertexArray[index] += normal.x;
			vertexArray[index + 1] += normal.y;
			vertexArray[index + 2] += normal.z;

			index = i1 * stride + normAttr->offset / (u32)normAttr->type;
			vertexArray[index] += normal.x;
			vertexArray[index + 1] += normal.y;
			vertexArray[index + 2] += normal.z;

			index = i2 * stride + normAttr->offset / (u32)normAttr->type;
			vertexArray[index] += normal.x;
			vertexArray[index + 1] += normal.y;
			vertexArray[index + 2] += normal.z;
		}

		// Normalize all normals
		for (u32 i = 0; i < indexArray.size() - 3; i += 3)
		{
			u32 index = indexArray[i] * stride + normAttr->offset / (u32)normAttr->type;
			math::Vector3f normal;
			normal.x = vertexArray[index];
			normal.y = vertexArray[index + 1];
			normal.z = vertexArray[index + 2];

			normal.Normalize();

			vertexArray[index] = normal.x;
			vertexArray[index + 1] = normal.y;
			vertexArray[index + 2] = normal.z;
		}
	}

	////////////////////////////////////////////////////////////////////////////////


} // sh