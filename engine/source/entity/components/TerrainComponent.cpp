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

namespace sh
{

	TerrainComponent::~TerrainComponent()
	{

	}

	void TerrainComponent::Load(const pugi::xml_node &node)
	{
		video::Image heightmap;
		pugi::xml_node heightmapNode = node.child("heightmap");
		if (heightmapNode)
		{
			const String hmSource = heightmapNode.attribute("val").as_string();
			heightmap.Load(hmSource);
		}

		const u32 width = heightmap.GetWidth();
		const u32 height = heightmap.GetHeight();
		const float step = 0.1f;

		std::vector<float> vertexArray;
		std::vector<unsigned int> indexArray;
		u32 verticesCount = 0U;

		f32 maxH = 0.0f;
		f32 minH = 0.0f;

		for (u32 i = 0U; i < width; ++i)
		{
			for (u32 j = 0U; j < height; ++j)
			{
				video::Color color = heightmap.GetPixel(i, j);

				const f32 height = color.red;
				const float x = j * step;
				const float z = i * step;

				vertexArray.push_back(x);
				vertexArray.push_back(height);
				vertexArray.push_back(z);

				verticesCount++;
			}
		}
		
		for (u32 i = 0U; i < width - 1; ++i)
		{
			for (u32 j = 0U; j < height - 1; ++j)
			{
				indexArray.push_back(j * width + i);
				indexArray.push_back((j + 1) * width + i);
				indexArray.push_back(j * width + i + 1);
				
				indexArray.push_back(j * width + i + 1);
				indexArray.push_back((j + 1) * width + i);
				indexArray.push_back((j + 1) * width + i + 1);
			}
		}

		sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());

		sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
		vertexDeclaration->AddAttribute(positionAttribute);


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

		video::MaterialPtr material(new video::Material());
		material->SetRenderTechnique("terrain.xml");

		m_model->SetMaterial(material);
	}

	void TerrainComponent::Save(pugi::xml_node &parent)
	{

	}


} // sh