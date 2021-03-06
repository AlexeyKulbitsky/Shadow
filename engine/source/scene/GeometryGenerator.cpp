#include "GeometryGenerator.h"
#include "../video/VertexDeclaration.h"
#include "../video/VertexBuffer.h"
#include "../video/IndexBuffer.h"
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
		ModelPtr GeometryGenerator::GetLineModel(const math::Vector3& start, const math::Vector3& end)
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
			sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
			sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			return CreateModel(vertexArray, std::vector<u32>(), vertexDeclaration, Topology::TOP_TRIANGLE_LIST);
		}

		//////////////////////////////////////////////////////////////////

		ModelPtr GeometryGenerator::GetCircleModel(const math::Vector3& centre, const f32 radius, const math::Vector3& vtx, const math::Vector3& vty)
		{
			std::vector<float> vertexArray;

			for (int i = 0; i < 100; i++)
			{
				math::Vector3 vt;
				vt = vtx * sh::math::Cos((2 * math::k_pi / 100) * i) * radius;
				vt += vty * sh::math::Sin((2 * math::k_pi / 100) * i) * radius;
				vt += centre;

				vertexArray.push_back(vt.x);
				vertexArray.push_back(vt.y);
				vertexArray.push_back(vt.z);
			}

			size_t verticesCount = vertexArray.size();

			// Create vertex declaration
			sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
			sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			return CreateModel(vertexArray, std::vector<u32>(), vertexDeclaration, Topology::TOP_TRIANGLE_LIST);
		}

		//////////////////////////////////////////////////////////////////

		ModelPtr GeometryGenerator::GetTorusModel(const math::Vector3& start, const f32 radius, const f32 ringRadius, u32 sides, u32 rings, const math::Vector3& vtx, const math::Vector3& vty)
		{
			float verticalAngularStride = (math::k_pi * 2.0f) / (float)rings;
			float horizontalAngularStride = (math::k_pi * 2.0f) / (float)sides;

			u32 numVerticesPerRow = sides + 1;
			u32 numVerticesPerColumn = rings + 1;

			math::Vector3 vtz = vtx.Cross(vty);
			math::Matrix3 rotationMatrix;
			rotationMatrix.SetIdentity();
			rotationMatrix.m[2][0] = vtz.x;
			rotationMatrix.m[2][1] = vtz.y;
			rotationMatrix.m[2][2] = vtz.z;

			rotationMatrix.m[0][0] = vtx.x;
			rotationMatrix.m[0][1] = vtx.y;
			rotationMatrix.m[0][2] = vtx.z;

			rotationMatrix.m[1][0] = vty.x;
			rotationMatrix.m[1][1] = vty.y;
			rotationMatrix.m[1][2] = vty.z;

			std::vector<f32> vertexArray;
			for (u32 verticalIt = 0; verticalIt < numVerticesPerColumn; verticalIt++)
			{
				float theta = verticalAngularStride * verticalIt;

				for (u32 horizontalIt = 0; horizontalIt < numVerticesPerRow; horizontalIt++)
				{
					float phi = horizontalAngularStride * horizontalIt;

					// position
					float x = math::Cos(theta) * (radius + ringRadius * math::Cos(phi));
					float y = math::Sin(theta) * (radius + ringRadius * math::Cos(phi));
					float z = ringRadius * math::Sin(phi);

					
					math::Vector3 vt(x, y, z);
					
					vt = rotationMatrix * vt;

					vertexArray.push_back(vt.x);
					vertexArray.push_back(vt.y);
					vertexArray.push_back(vt.z);

					//vertexArray.push_back(x);
					//vertexArray.push_back(y);
					//vertexArray.push_back(z);
				}
			}
			size_t verticesCount = vertexArray.size() / 3;


			std::vector<u32> indexArray;		
			size_t indicesCount = sides * rings * 6;

			for (u32 verticalIt = 0; verticalIt < rings; verticalIt++)
			{
				for (u32 horizontalIt = 0; horizontalIt < sides; horizontalIt++)
				{
					short lt = (short)(horizontalIt + verticalIt * (numVerticesPerRow));
					short rt = (short)((horizontalIt + 1) + verticalIt * (numVerticesPerRow));

					short lb = (short)(horizontalIt + (verticalIt + 1) * (numVerticesPerRow));
					short rb = (short)((horizontalIt + 1) + (verticalIt + 1) * (numVerticesPerRow));
					
					indexArray.push_back(lt);
					indexArray.push_back(rt);
					indexArray.push_back(lb);

					indexArray.push_back(rt);
					indexArray.push_back(rb);
					indexArray.push_back(lb);
				}
			}
			


			// Create vertex declaration
			sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
			sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			return CreateModel(vertexArray, indexArray, vertexDeclaration, Topology::TOP_TRIANGLE_LIST);
		}

		//////////////////////////////////////////////////////////////////

		ModelPtr GeometryGenerator::GetTorusModel(const f32 radius, const f32 ringRadius, u32 sides, u32 rings, math::Matrix4 transform)
		{
			float verticalAngularStride = (math::k_pi * 2.0f) / (float)rings;
			float horizontalAngularStride = (math::k_pi * 2.0f) / (float)sides;

			u32 numVerticesPerRow = sides + 1;
			u32 numVerticesPerColumn = rings + 1;

			std::vector<f32> vertexArray;
			for (u32 verticalIt = 0; verticalIt < numVerticesPerColumn; verticalIt++)
			{
				float theta = verticalAngularStride * verticalIt;

				for (u32 horizontalIt = 0; horizontalIt < numVerticesPerRow; horizontalIt++)
				{
					float phi = horizontalAngularStride * horizontalIt;

					// position
					float x = math::Cos(theta) * (radius + ringRadius * math::Cos(phi));
					float y = math::Sin(theta) * (radius + ringRadius * math::Cos(phi));
					float z = ringRadius * math::Sin(phi);


					math::Vector4 vt(x, y, z, 1.0f);

					vt = transform * vt;

					vertexArray.push_back(vt.x);
					vertexArray.push_back(vt.y);
					vertexArray.push_back(vt.z);
				}
			}
			size_t verticesCount = vertexArray.size() / 3;


			std::vector<u32> indexArray;
			size_t indicesCount = sides * rings * 6;

			for (u32 verticalIt = 0; verticalIt < rings; verticalIt++)
			{
				for (u32 horizontalIt = 0; horizontalIt < sides; horizontalIt++)
				{
					short lt = (short)(horizontalIt + verticalIt * (numVerticesPerRow));
					short rt = (short)((horizontalIt + 1) + verticalIt * (numVerticesPerRow));

					short lb = (short)(horizontalIt + (verticalIt + 1) * (numVerticesPerRow));
					short rb = (short)((horizontalIt + 1) + (verticalIt + 1) * (numVerticesPerRow));

					indexArray.push_back(lt);
					indexArray.push_back(rt);
					indexArray.push_back(lb);

					indexArray.push_back(rt);
					indexArray.push_back(rb);
					indexArray.push_back(lb);
				}
			}



			// Create vertex declaration
			sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
			sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			return CreateModel(vertexArray, indexArray, vertexDeclaration, Topology::TOP_TRIANGLE_LIST);
		}

		//////////////////////////////////////////////////////////////////

		ModelPtr GeometryGenerator::GetHalfTorusModel(const f32 radius, const f32 ringRadius, u32 sides, u32 rings, math::Matrix4 transform)
		{
			float verticalAngularStride = (math::k_pi) / (float)rings;
			float horizontalAngularStride = (math::k_pi) / (float)sides;

			u32 numVerticesPerRow = sides + 1;
			u32 numVerticesPerColumn = rings + 1;

			std::vector<f32> vertexArray;
			for (u32 verticalIt = 0; verticalIt < numVerticesPerColumn; verticalIt++)
			{
				float theta = verticalAngularStride * verticalIt;

				for (u32 horizontalIt = 0; horizontalIt < numVerticesPerRow; horizontalIt++)
				{
					float phi = horizontalAngularStride * horizontalIt;

					// position
					float x = math::Cos(theta) * (radius + ringRadius * math::Cos(phi));
					float y = math::Sin(theta) * (radius + ringRadius * math::Cos(phi));
					float z = ringRadius * math::Sin(phi);

					math::Vector4 vt = transform * math::Vector4(x, y, z, 1.0f);

					vertexArray.push_back(vt.x);
					vertexArray.push_back(vt.y);
					vertexArray.push_back(vt.z);
				}
			}
			size_t verticesCount = vertexArray.size() / 3;

			std::vector<u32> indexArray;
			size_t indicesCount = sides * rings * 6;

			for (u32 verticalIt = 0; verticalIt < rings; verticalIt++)
			{
				for (u32 horizontalIt = 0; horizontalIt < sides; horizontalIt++)
				{
					short lt = (short)(horizontalIt + verticalIt * (numVerticesPerRow));
					short rt = (short)((horizontalIt + 1) + verticalIt * (numVerticesPerRow));

					short lb = (short)(horizontalIt + (verticalIt + 1) * (numVerticesPerRow));
					short rb = (short)((horizontalIt + 1) + (verticalIt + 1) * (numVerticesPerRow));

					indexArray.push_back(lt);
					indexArray.push_back(rt);
					indexArray.push_back(lb);

					indexArray.push_back(rt);
					indexArray.push_back(rb);
					indexArray.push_back(lb);
				}
			}

			// Create vertex declaration
			sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
			sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			return CreateModel(vertexArray, indexArray, vertexDeclaration, Topology::TOP_TRIANGLE_LIST);
		}

		ModelPtr GeometryGenerator::GetHalfTorusModel(const math::Vector3& start, const f32 radius, const f32 ringRadius, u32 sides, u32 rings, const math::Vector3& vtx, const math::Vector3& vty)
		{
			float verticalAngularStride = (math::k_pi) / (float)rings;
			float horizontalAngularStride = (math::k_pi) / (float)sides;

			u32 numVerticesPerRow = sides + 1;
			u32 numVerticesPerColumn = rings + 1;

			math::Vector3 vtz = vtx.Cross(vty);
			math::Matrix3 rotationMatrix;
			rotationMatrix.SetIdentity();
			rotationMatrix.m[2][0] = vtz.x;
			rotationMatrix.m[2][1] = vtz.y;
			rotationMatrix.m[2][2] = vtz.z;

			rotationMatrix.m[0][0] = vtx.x;
			rotationMatrix.m[0][1] = vtx.y;
			rotationMatrix.m[0][2] = vtx.z;

			rotationMatrix.m[1][0] = vty.x;
			rotationMatrix.m[1][1] = vty.y;
			rotationMatrix.m[1][2] = vty.z;

			std::vector<f32> vertexArray;
			for (u32 verticalIt = 0; verticalIt < numVerticesPerColumn; verticalIt++)
			{
				float theta = verticalAngularStride * verticalIt;

				for (u32 horizontalIt = 0; horizontalIt < numVerticesPerRow; horizontalIt++)
				{
					float phi = horizontalAngularStride * horizontalIt;

					// position
					float x = math::Cos(theta) * (radius + ringRadius * math::Cos(phi));
					float y = math::Sin(theta) * (radius + ringRadius * math::Cos(phi));
					float z = ringRadius * math::Sin(phi);

					
					math::Vector3 vt(x, y, z);
					
					vt = rotationMatrix * vt;

					vertexArray.push_back(vt.x);
					vertexArray.push_back(vt.y);
					vertexArray.push_back(vt.z);

					//vertexArray.push_back(x);
					//vertexArray.push_back(y);
					//vertexArray.push_back(z);
				}
			}
			size_t verticesCount = vertexArray.size() / 3;


			std::vector<u32> indexArray;		
			size_t indicesCount = sides * rings * 6;

			for (u32 verticalIt = 0; verticalIt < rings; verticalIt++)
			{
				for (u32 horizontalIt = 0; horizontalIt < sides; horizontalIt++)
				{
					short lt = (short)(horizontalIt + verticalIt * (numVerticesPerRow));
					short rt = (short)((horizontalIt + 1) + verticalIt * (numVerticesPerRow));

					short lb = (short)(horizontalIt + (verticalIt + 1) * (numVerticesPerRow));
					short rb = (short)((horizontalIt + 1) + (verticalIt + 1) * (numVerticesPerRow));
					
					indexArray.push_back(lt);
					indexArray.push_back(rt);
					indexArray.push_back(lb);

					indexArray.push_back(rt);
					indexArray.push_back(rb);
					indexArray.push_back(lb);
				}
			}
			


			// Create vertex declaration
			sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
			sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			return CreateModel(vertexArray, indexArray, vertexDeclaration, Topology::TOP_TRIANGLE_LIST);
		}

		//////////////////////////////////////////////////////////////////

		ModelPtr GeometryGenerator::GetCylinderModel(f32 height, f32 radius, u32 numberOfSides, math::Matrix4 transform)
		{
			std::vector<float> vertexArray;

			math::Vector3 srcVertex(0.0f);
			math::Vector3 resultVertex(0.0f);
			for (u32 i = 0; i < numberOfSides; i++)
			{
				srcVertex.x = sh::math::Cos((4 * math::k_pi / numberOfSides) * i) * radius;
				srcVertex.z = sh::math::Sin((4 * math::k_pi / numberOfSides) * i) * radius;
				srcVertex.y = -height / 2.0f;
				resultVertex = transform * srcVertex;

				vertexArray.push_back(resultVertex.x);
				vertexArray.push_back(resultVertex.y);
				vertexArray.push_back(resultVertex.z);

				srcVertex.y += height;
				resultVertex = transform * srcVertex;

				vertexArray.push_back(resultVertex.x);
				vertexArray.push_back(resultVertex.y);
				vertexArray.push_back(resultVertex.z);
			}

			std::vector<u32> indexArray;

			for (u32 i = 0; i < numberOfSides; i += 2)
			{
				indexArray.push_back(i);
				indexArray.push_back(i + 2);
				indexArray.push_back(i + 1);

				indexArray.push_back(i + 2);
				indexArray.push_back(i + 3);
				indexArray.push_back(i + 1);
			}

			// Create vertex declaration
			sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
			sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			return CreateModel(vertexArray, indexArray, vertexDeclaration, Topology::TOP_TRIANGLE_LIST);
		}

		//////////////////////////////////////////////////////////////////

		ModelPtr GeometryGenerator::GetConeModel(f32 radius, f32 height, math::Matrix4 transform)
		{
			std::vector<float> vertexArray;
			u32 numberOfSides = 30U;
			math::Vector3 srcVertex(0.0f);
			math::Vector3 resultVertex(0.0f);

			srcVertex.x = 0.0f;
			srcVertex.z = 0.0f;
			srcVertex.y = height;
			resultVertex = transform * srcVertex;

			vertexArray.push_back(resultVertex.x);
			vertexArray.push_back(resultVertex.y);
			vertexArray.push_back(resultVertex.z);

			for (size_t i = 0; i <= numberOfSides; ++i)
			{
				math::Vector3 pt;
				srcVertex.x = sh::math::Cos(((2 * sh::math::k_pi) / numberOfSides) * i) * radius;
				srcVertex.z = sh::math::Sin(((2 * sh::math::k_pi) / numberOfSides) * i) * radius;
				srcVertex.y = 0.0f;
				
				resultVertex = transform * srcVertex;
				vertexArray.push_back(resultVertex.x);
				vertexArray.push_back(resultVertex.y);
				vertexArray.push_back(resultVertex.z);
			}

			// Create vertex declaration
			sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
			sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			return CreateModel(vertexArray, std::vector<u32>(), vertexDeclaration, Topology::TOP_TRIANGLE_FAN);
		}

		//////////////////////////////////////////////////////////////////

		ModelPtr GeometryGenerator::GetPlaneModel(f32 width, f32 height, math::Matrix4 transform)
		{
			math::Vector3 srcVertex(0.0f);
			math::Vector3 resultVertex(0.0f);

			std::vector<float> vertexArray;

			srcVertex = sh::math::Vector3(-width / 2.0f, -height / 2.0f, 0.0f);
			resultVertex = transform * srcVertex;
			vertexArray.push_back(resultVertex.x);
			vertexArray.push_back(resultVertex.y);
			vertexArray.push_back(resultVertex.z); 

			srcVertex = sh::math::Vector3(width / 2.0f, -height / 2.0f, 0.0f);
			resultVertex = transform * srcVertex;
			vertexArray.push_back(resultVertex.x);
			vertexArray.push_back(resultVertex.y);
			vertexArray.push_back(resultVertex.z); 

			srcVertex = sh::math::Vector3(width / 2.0f, height / 2.0f, 0.0f);
			resultVertex = transform * srcVertex;
			vertexArray.push_back(resultVertex.x);
			vertexArray.push_back(resultVertex.y);
			vertexArray.push_back(resultVertex.z); 

			srcVertex = sh::math::Vector3(-width / 2.0f, height / 2.0f, 0.0f);
			resultVertex = transform * srcVertex;
			vertexArray.push_back(resultVertex.x);
			vertexArray.push_back(resultVertex.y);
			vertexArray.push_back(resultVertex.z); 

			std::vector<u32> indexArray;

			indexArray.push_back(0);
			indexArray.push_back(1);
			indexArray.push_back(2);

			indexArray.push_back(0);
			indexArray.push_back(2);
			indexArray.push_back(3);

			// Create vertex declaration
			sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
			sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);

			return CreateModel(vertexArray, indexArray, vertexDeclaration, Topology::TOP_TRIANGLE_LIST);
		}

		//////////////////////////////////////////////////////////////////

		ModelPtr GeometryGenerator::CreateModel(const std::vector<float>& vertexArray, const std::vector<u32>& indexArray, 
			const video::VertexDeclarationPtr& vertexDeclaration, Topology topology)
		{
			sh::scene::MeshBasePtr mesh(new sh::scene::MeshBase());

			// Create vertex buffer
			size_t verticesCount = vertexArray.size() * sizeof(f32) / vertexDeclaration->GetStride();
			const void* verticesPointer = vertexArray.data();
			size_t verticesDataSize = vertexArray.size() * sizeof(float);
			video::VertexBufferDecription vertexBufDesc;
			vertexBufDesc.usage = USAGE_STATIC;
			sh::video::VertexBufferPtr vertexBuffer = video::VertexBuffer::Create(vertexBufDesc);
			vertexBuffer->SetData(0, verticesDataSize, verticesPointer);
			vertexBuffer->SetVerticesCount(verticesCount);
			vertexBuffer->SetVertexSize(vertexDeclaration->GetStride());
			vertexBuffer->SetVertexDeclaration(vertexDeclaration);
			mesh->SetVertexBuffer(vertexBuffer);

			// Create index buffer
			if (indexArray.size() != 0)
			{
				const void* indicesPointer = indexArray.data();
				size_t indicesDataSize = indexArray.size() * sizeof(u32);
				video::IndexBufferDescription indexBufDesc;
				indexBufDesc.usage = USAGE_STATIC;
				indexBufDesc.indexType = IT_32_BIT;
				sh::video::IndexBufferPtr indexBuffer = video::IndexBuffer::Create(indexBufDesc);
				indexBuffer->SetData(0, indicesDataSize, indicesPointer);
				indexBuffer->SetIndicesCount(indexArray.size());
				mesh->SetIndexBuffer(indexBuffer);
			}
			
					
			mesh->SetTopology(topology);

			sh::scene::ModelBasePtr model(new sh::scene::ModelBase());
			model->AddMesh(mesh);

			sh::scene::ModelPtr resultModel(new sh::scene::Model(model));

			return resultModel;
		}

		//////////////////////////////////////////////////////////////////
	}
}