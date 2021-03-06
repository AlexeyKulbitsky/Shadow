#include "Painter.h"
#include "../Device.h"
#include "Material.h"
#include "RenderPipeline.h"
#include "VertexDeclaration.h"
#include "GpuParams.h"
#include "../scene/SceneManager.h"
#include "../scene/Camera.h"
#include "../CommonTypes.h"

namespace sh
{
	namespace video
	{
		Painter::Painter()
		{
			Driver* driver = Device::GetInstance()->GetDriver();

			sh::video::CommandBufferDescription commandBufferDesc;
			commandBufferDesc.type = sh::COMMAND_BUFFER_TYPE_SECONDARY;
			m_commandBuffer = sh::video::CommandBuffer::Create(commandBufferDesc);

			// Create lines data
			m_linesVertexArray.reserve(50000);
			video::VertexBufferDecription desc;
			desc.usage = USAGE_STREAM;
			m_linesVertexBuffer = video::VertexBuffer::Create(desc);
			m_lines.linesBatches.reserve(100U);
			m_lines.verticesCount = 0U;
			
			// Create triangles data
			m_trianglesVertexArray.reserve(50000);
			m_trianglesIndexArray.reserve(50000);
			m_trianglesVertexBuffer = video::VertexBuffer::Create(desc);
			video::IndexBufferDescription indexDesc;
			indexDesc.indexType = IT_32_BIT;
			indexDesc.usage = USAGE_STREAM;
			m_trianglesIndexBuffer = video::IndexBuffer::Create(indexDesc);
			m_triangles.trianglesBatches.reserve(100U);
			m_triangles.indicesCount = 0U;
            
            m_camera = Device::GetInstance()->GetSceneManager()->GetCamera();
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::SetMaterial(const MaterialPtr& material)
		{
			if (m_materials.size() != 0U && m_materials[m_materials.size() - 1U] == material)
				return;

			m_material = material;

			m_materials.push_back(material);

			// Check if previous lines batch has or not anything to render
			// If not then remove it from render list to avoid useless operations
			if (m_lines.linesBatches.size() > 0U && 
				m_lines.linesBatches[m_lines.linesBatches.size() - 1].verticesCount == 0U)
			{
				const u32 linesIdx = m_lines.linesBatches.size() - 1;

				m_lines.linesBatches[linesIdx].materialIndex = m_materials.size() - 1;
                m_lines.linesBatches[linesIdx].cameraIndex = m_cameras.size() - 1;
				m_lines.linesBatches[linesIdx].startIndex = m_lines.verticesCount;
				m_lines.linesBatches[linesIdx].verticesCount = 0U;
			}
			// If has then create new batch
			else
			{
				LinesBatch linesBatch;
				linesBatch.materialIndex = m_materials.size() - 1;
                linesBatch.cameraIndex = m_cameras.size() - 1;
				linesBatch.startIndex = m_lines.verticesCount;
				// If it is first batch then use viewport (no scissor rect)
				if (m_lines.linesBatches.size() == 0U)
				{
					linesBatch.clipRect = Device::GetInstance()->GetDriver()->GetViewport();
				}
				// Otherwise use previous clip rect
				else
				{
					const u32 lastBatchIdx = m_lines.linesBatches.size() - 1;
					linesBatch.clipRect = m_triangles.trianglesBatches[lastBatchIdx].clipRect;
				}
				m_lines.linesBatches.push_back(linesBatch);
			}
			
			// Check if previous triangles batch has or not anything to render
			// If not then remove it from render list to avoid useless operations
			if (m_triangles.trianglesBatches.size() > 0U &&
				m_triangles.trianglesBatches[m_triangles.trianglesBatches.size() - 1].verticesCount == 0U)
			{
				const u32 trianglesIdx = m_triangles.trianglesBatches.size() - 1;

				m_triangles.trianglesBatches[trianglesIdx].materialIndex = m_materials.size() - 1;
                m_triangles.trianglesBatches[trianglesIdx].cameraIndex = m_cameras.size() - 1;
				m_triangles.trianglesBatches[trianglesIdx].startIndex = m_triangles.indicesCount;
				m_triangles.trianglesBatches[trianglesIdx].verticesCount = 0U;
				m_triangles.trianglesBatches[trianglesIdx].indicesCount = 0U;
			}
			// If has then create new batch
			else
			{
				TrianglesBatch trianglesBatch;
				trianglesBatch.materialIndex = m_materials.size() - 1;
                trianglesBatch.cameraIndex = m_cameras.size() - 1;
				trianglesBatch.startIndex = m_triangles.indicesCount;
				// If it is first batch then use viewport (no scissor rect)
				if (m_triangles.trianglesBatches.size() == 0U)
				{
					trianglesBatch.clipRect = Device::GetInstance()->GetDriver()->GetViewport();
				}
				// Otherwise use previous clip rect
				else
				{
					const u32 lastBatchIdx = m_triangles.trianglesBatches.size() - 1;
					trianglesBatch.clipRect = m_triangles.trianglesBatches[lastBatchIdx].clipRect;
				}
				m_triangles.trianglesBatches.push_back(trianglesBatch);
			}
		}
        
        /////////////////////////////////////////////////////////////////////////////////////
        
        const MaterialPtr& Painter::GetMaterial() const
        {
            return m_material;
        }

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::SetCamera(const scene::CameraPtr& camera)
		{
            if (m_cameras.size() != 0U && m_cameras[m_cameras.size() - 1U] == camera)
                return;
            
			m_camera = camera;
            
            m_cameras.push_back(camera);
            
            // Check if previous lines batch has or not anything to render
            // If not then remove it from render list to avoid useless operations
            if (m_lines.linesBatches.size() > 0U &&
                m_lines.linesBatches[m_lines.linesBatches.size() - 1].verticesCount == 0U)
            {
                const u32 linesIdx = m_lines.linesBatches.size() - 1;
                
                m_lines.linesBatches[linesIdx].materialIndex = m_materials.size() - 1;
                m_lines.linesBatches[linesIdx].cameraIndex = m_cameras.size() - 1;
                m_lines.linesBatches[linesIdx].startIndex = m_lines.verticesCount;
                m_lines.linesBatches[linesIdx].verticesCount = 0U;
            }
            // If has then create new batch
            else
            {
                LinesBatch linesBatch;
                linesBatch.materialIndex = m_materials.size() - 1;
                linesBatch.cameraIndex = m_cameras.size() - 1;
                linesBatch.startIndex = m_lines.verticesCount;
                // If it is first batch then use viewport (no scissor rect)
                if (m_lines.linesBatches.size() == 0U)
                {
                    linesBatch.clipRect = Device::GetInstance()->GetDriver()->GetViewport();
                }
                // Otherwise use previous clip rect
                else
                {
                    const u32 lastBatchIdx = m_lines.linesBatches.size() - 1;
                    linesBatch.clipRect = m_triangles.trianglesBatches[lastBatchIdx].clipRect;
                }
                m_lines.linesBatches.push_back(linesBatch);
            }
            
            // Check if previous triangles batch has or not anything to render
            // If not then remove it from render list to avoid useless operations
            if (m_triangles.trianglesBatches.size() > 0U &&
                m_triangles.trianglesBatches[m_triangles.trianglesBatches.size() - 1].verticesCount == 0U)
            {
                const u32 trianglesIdx = m_triangles.trianglesBatches.size() - 1;
                
                m_triangles.trianglesBatches[trianglesIdx].materialIndex = m_materials.size() - 1;
                m_triangles.trianglesBatches[trianglesIdx].cameraIndex = m_cameras.size() - 1;
                m_triangles.trianglesBatches[trianglesIdx].startIndex = m_triangles.indicesCount;
                m_triangles.trianglesBatches[trianglesIdx].verticesCount = 0U;
                m_triangles.trianglesBatches[trianglesIdx].indicesCount = 0U;
            }
            // If has then create new batch
            else
            {
                TrianglesBatch trianglesBatch;
                trianglesBatch.materialIndex = m_materials.size() - 1;
                trianglesBatch.cameraIndex = m_cameras.size() - 1;
                trianglesBatch.startIndex = m_triangles.indicesCount;
                // If it is first batch then use viewport (no scissor rect)
                if (m_triangles.trianglesBatches.size() == 0U)
                {
                    trianglesBatch.clipRect = Device::GetInstance()->GetDriver()->GetViewport();
                }
                // Otherwise use previous clip rect
                else
                {
                    const u32 lastBatchIdx = m_triangles.trianglesBatches.size() - 1;
                    trianglesBatch.clipRect = m_triangles.trianglesBatches[lastBatchIdx].clipRect;
                }
                m_triangles.trianglesBatches.push_back(trianglesBatch);
            }
		}
        
        /////////////////////////////////////////////////////////////////////////////////////
        
        const scene::CameraPtr& Painter::GetCamera() const
        {
            return m_camera;
        }

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::SetClipRect(const math::Rect& rect)
		{
			// Set clip rect for lines batches
			const u32 linessIdx = m_lines.linesBatches.size() - 1U;
			if (m_lines.linesBatches[linessIdx].clipRect != rect)
			{
				if (m_lines.linesBatches[linessIdx].verticesCount == 0U)
				{
					m_lines.linesBatches[linessIdx].clipRect = rect;
				}
				else
				{
					const auto& currentBatch = m_lines.linesBatches[linessIdx];
					LinesBatch newBatch;
					newBatch.materialIndex = currentBatch.materialIndex;
                    newBatch.cameraIndex = currentBatch.cameraIndex;
					newBatch.startIndex = m_lines.verticesCount;
					newBatch.clipRect = rect;
					m_lines.linesBatches.push_back(newBatch);
				}
			}

			// Set clip rect for triangles batches
			const u32 triangleIdx = m_triangles.trianglesBatches.size() - 1U;
			if (m_triangles.trianglesBatches[triangleIdx].clipRect != rect)
			{
				if (m_triangles.trianglesBatches[triangleIdx].verticesCount == 0U)
				{
					m_triangles.trianglesBatches[triangleIdx].clipRect = rect;
				}
				else
				{
					const auto& currentBatch = m_triangles.trianglesBatches[triangleIdx];
					TrianglesBatch newBatch;
					newBatch.materialIndex = currentBatch.materialIndex;
                    newBatch.cameraIndex = currentBatch.cameraIndex;
					newBatch.startIndex = m_triangles.indicesCount;
					newBatch.clipRect = rect;
					m_triangles.trianglesBatches.push_back(newBatch);
				}
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////

		const math::Rect& Painter::GetClipRect() const
		{
			const u32 idx = m_triangles.trianglesBatches.size() - 1U;
			return m_triangles.trianglesBatches[idx].clipRect;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawLine(const math::Vector3& a, const math::Vector3& b)
		{
			const auto& declaration = m_material->GetRenderPipeline()->GetVertexInputDeclaration();
			const u32 attributesCount = declaration->GetAttributesCount();

			for (u32 i = 0U; i < attributesCount; ++i)
			{
				switch (declaration->GetAttribute(i).semantic)
				{
					case AttributeSemantic::POSITION:
					{
						// Start point position
						m_linesVertexArray.push_back(a.x);
						m_linesVertexArray.push_back(a.y);
						m_linesVertexArray.push_back(a.z);
					}
					break;
					case AttributeSemantic::COLOR:
					{
						// Start point color
						m_linesVertexArray.push_back(1.0f);
						m_linesVertexArray.push_back(0.0f);
						m_linesVertexArray.push_back(0.0f);
					}
					break;
					default:
						break;
				}
			}


			for (u32 i = 0U; i < attributesCount; ++i)
			{
				switch (declaration->GetAttribute(i).semantic)
				{
					case AttributeSemantic::POSITION:
					{
						// End point position
						m_linesVertexArray.push_back(b.x);
						m_linesVertexArray.push_back(b.y);
						m_linesVertexArray.push_back(b.z);
					}
					break;
					case AttributeSemantic::COLOR:
					{
						// End point color
						m_linesVertexArray.push_back(1.0f);
						m_linesVertexArray.push_back(0.0f);
						m_linesVertexArray.push_back(0.0f);
					}
					break;
					default:
						break;
				}
			}

			const u32 idx = m_lines.linesBatches.size() - 1;
			m_lines.linesBatches[idx].verticesCount += 2U;
			m_lines.verticesCount += 2U;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawRect(const math::Rect& rect, const gui::SpritePtr& sprite)
		{

		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawRect(const Painter::Vertex& upperLeft, const Painter::Vertex& downRight)
		{
			const auto& declaration = m_material->GetRenderPipeline()->GetVertexInputDeclaration();
			const u32 attributesCount = declaration->GetAttributesCount();
			const u32 idx = m_triangles.trianglesBatches.size() - 1;
			const u32 startVertex = m_triangles.verticesCount;

			for (u32 corner = 0U; corner < 4U; ++corner)
			{
				Vertex vertex;

				switch (corner)
				{
					case 0:
					{
						vertex = upperLeft;
					}
						break;
					case 1:
					{
						vertex.position.x = upperLeft.position.x;
						vertex.position.y = downRight.position.y;
						vertex.position.z = 0.0f;

						vertex.uv.x = upperLeft.uv.x;
						vertex.uv.y = downRight.uv.y;

						vertex.color = upperLeft.color;
					}
						break;
					case 2:
					{
						vertex = downRight;
					}
						break;
					case 3:
					{
						vertex.position.x = downRight.position.x;
						vertex.position.y = upperLeft.position.y;
						vertex.position.z = 0.0f;

						vertex.uv.x = downRight.uv.x;
						vertex.uv.y = upperLeft.uv.y;

						vertex.color = downRight.color;
					}
						break;
					default:
						break;
				}

				for (u32 i = 0U; i < attributesCount; ++i)
				{
					switch (declaration->GetAttribute(i).semantic)
					{
						case AttributeSemantic::POSITION:
						{
							m_trianglesVertexArray.push_back(vertex.position.x);
							m_trianglesVertexArray.push_back(vertex.position.y);
							m_trianglesVertexArray.push_back(vertex.position.z);
						}
						break;
						case AttributeSemantic::UV:
						{
							m_trianglesVertexArray.push_back(vertex.uv.x);
							m_trianglesVertexArray.push_back(vertex.uv.y);
						}
						break;
						case AttributeSemantic::COLOR:
						{
							m_trianglesVertexArray.push_back(vertex.color.x);
							m_trianglesVertexArray.push_back(vertex.color.y);
							m_trianglesVertexArray.push_back(vertex.color.z);
							m_trianglesVertexArray.push_back(vertex.color.w);
						}
						break;
						default:
							break;
					}
				}
			}

			
			m_trianglesIndexArray.push_back(startVertex);
			m_trianglesIndexArray.push_back(startVertex + 1U);
			m_trianglesIndexArray.push_back(startVertex + 2U);

			m_trianglesIndexArray.push_back(startVertex);
			m_trianglesIndexArray.push_back(startVertex + 2U);
			m_trianglesIndexArray.push_back(startVertex + 3U);

			m_triangles.trianglesBatches[idx].verticesCount += 4U;
			m_triangles.trianglesBatches[idx].indicesCount += 6U;
			m_triangles.verticesCount += 4U;
			m_triangles.indicesCount += 6U;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawQuad(const Vertex& downLeft, const Vertex& upperLeft, const Vertex& upperRight, const Vertex& downRight)
		{
			Painter::Vertex vertices[] = { downLeft, upperLeft, upperRight, downRight };

			const auto& declaration = m_material->GetRenderPipeline()->GetVertexInputDeclaration();
			const u32 attributesCount = declaration->GetAttributesCount();
			const u32 idx = m_triangles.trianglesBatches.size() - 1;
			const u32 startVertex = m_triangles.verticesCount;

			for (u32 vertexIdx = 0; vertexIdx < 4; ++vertexIdx)
			{
				const auto& vertex = vertices[vertexIdx];
				for (u32 i = 0U; i < attributesCount; ++i)
				{
					switch (declaration->GetAttribute(i).semantic)
					{
					case AttributeSemantic::POSITION:
					{
						m_trianglesVertexArray.push_back(vertex.position.x);
						m_trianglesVertexArray.push_back(vertex.position.y);
						m_trianglesVertexArray.push_back(vertex.position.z);
					}
					break;
					case AttributeSemantic::UV:
					{
						m_trianglesVertexArray.push_back(vertex.uv.x);
						m_trianglesVertexArray.push_back(vertex.uv.y);
					}
					break;
					case AttributeSemantic::COLOR:
					{
						m_trianglesVertexArray.push_back(vertex.color.x);
						m_trianglesVertexArray.push_back(vertex.color.y);
						m_trianglesVertexArray.push_back(vertex.color.z);
						m_trianglesVertexArray.push_back(vertex.color.w);
					}
					break;
					default:
						break;
					}
				}
			}

			m_trianglesIndexArray.push_back(startVertex);
			m_trianglesIndexArray.push_back(startVertex + 2U);
			m_trianglesIndexArray.push_back(startVertex + 1U);

			m_trianglesIndexArray.push_back(startVertex);
			m_trianglesIndexArray.push_back(startVertex + 3U);
			m_trianglesIndexArray.push_back(startVertex + 2U);

			m_triangles.trianglesBatches[idx].verticesCount += 4U;
			m_triangles.trianglesBatches[idx].indicesCount += 6U;
			m_triangles.verticesCount += 4U;
			m_triangles.indicesCount += 6U;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawCircle()
		{

		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawArc()
		{

		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawBox(const math::AABB& box)
		{
			const auto& a = box.minPoint;
			const auto& b = box.maxPoint;

			DrawLine(math::Vector3(a.x, a.y, a.z), math::Vector3(b.x, a.y, a.z));
			DrawLine(math::Vector3(b.x, a.y, a.z), math::Vector3(b.x, b.y, a.z));
			DrawLine(math::Vector3(b.x, b.y, a.z), math::Vector3(a.x, b.y, a.z));
			DrawLine(math::Vector3(a.x, b.y, a.z), math::Vector3(a.x, a.y, a.z));

			DrawLine(math::Vector3(b.x, b.y, b.z), math::Vector3(a.x, b.y, b.z));
			DrawLine(math::Vector3(a.x, b.y, b.z), math::Vector3(a.x, a.y, b.z));
			DrawLine(math::Vector3(a.x, a.y, b.z), math::Vector3(b.x, a.y, b.z));
			DrawLine(math::Vector3(b.x, a.y, b.z), math::Vector3(b.x, b.y, b.z));

			DrawLine(math::Vector3(a.x, a.y, a.z), math::Vector3(a.x, a.y, b.z));
			DrawLine(math::Vector3(a.x, b.y, a.z), math::Vector3(a.x, b.y, b.z));
			DrawLine(math::Vector3(b.x, b.y, a.z), math::Vector3(b.x, b.y, b.z));
			DrawLine(math::Vector3(b.x, a.y, a.z), math::Vector3(b.x, a.y, b.z));
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawText()
		{

		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawTriangleList(const std::vector<float>& vertices, const std::vector<u32>& indices, u32 verticesCount)
		{
			m_trianglesVertexArray.insert(m_trianglesVertexArray.end(), vertices.begin(), vertices.end());
			const u32 idx = m_triangles.trianglesBatches.size() - 1;
			const u32 startVertex = m_triangles.verticesCount;
			std::transform(indices.begin(), indices.end(), std::back_inserter(m_trianglesIndexArray),
						   [startVertex](u32 value){ return value + startVertex; });

			m_triangles.trianglesBatches[idx].verticesCount += verticesCount;
			m_triangles.trianglesBatches[idx].indicesCount += indices.size();
			m_triangles.verticesCount += verticesCount;
			m_triangles.indicesCount += indices.size();
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::Flush()
		{
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			sh::scene::Camera* camera;

			if (m_camera)
				camera = m_camera.get();
			else
				camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera().get();

			const sh::math::Matrix4& viewMatrix = camera->GetViewMatrix();
			const sh::math::Matrix4& projectionMatrix = camera->GetProjectionMatrix();
			const sh::math::Matrix4& projection2DMatrix = camera->Get2DProjectionMatrix();

			m_commandBuffer->Begin();

			size_t drawCallsCount = 0;
			// Render lines
			// Upload vertex data to lines vertex buffer
			const void* verticesPointer = m_linesVertexArray.data();
			size_t verticesDataSize = m_linesVertexArray.size() * sizeof(float);
			m_linesVertexBuffer->SetData(0U, verticesDataSize, verticesPointer);
			m_linesVertexBuffer->SetVerticesCount(m_lines.verticesCount);
			
			for (u32 i = 0U; i < m_lines.linesBatches.size(); ++i)
			{
				if (m_lines.linesBatches[i].verticesCount == 0)
					continue;
                
                //const auto& camera = m_cameras[m_lines.linesBatches[i].cameraIndex];
                const sh::math::Matrix4& viewMatrix = camera->GetViewMatrix();
                const sh::math::Matrix4& projectionMatrix = camera->GetProjectionMatrix();
                const sh::math::Matrix4& projection2DMatrix = camera->Get2DProjectionMatrix();

				// Update current batch material params
				const u32 materialIdx = m_lines.linesBatches[i].materialIndex;
				auto& params = m_materials[materialIdx]->GetAutoParams();
				for (u32 paramIdx = 0U; paramIdx < params->GetParamsCount(); ++paramIdx)
				{
					auto param = params->GetParam(paramIdx);
					switch (param->GetUsage())
					{
						case MaterialParamUsage::MatrixWorld:
							break;
						case MaterialParamUsage::MatrixView:
							param->Set(viewMatrix);
							break;
						case MaterialParamUsage::MatrixViewRotation:
						{
							param->Set(camera->GetRotationMatrix());
						}
						break;
						case MaterialParamUsage::MatrixViewRotationProjection:
							param->Set(( projectionMatrix * camera->GetRotationMatrix() ).GetTransposed());
							break;
						case MaterialParamUsage::MatrixProjection:
							param->Set(projectionMatrix);
							break;
						case MaterialParamUsage::MatrixViewProjection:
						{
							math::Matrix4 viewProjection = projectionMatrix * viewMatrix;
							param->Set(viewProjection);
						}
						break;
						case MaterialParamUsage::MatrixWorldViewProjection:
						{
							math::Matrix4 wvp = projectionMatrix * viewMatrix;
							wvp.Transpose();
							param->Set(wvp);
						}
						break;
						case MaterialParamUsage::Matrix2DProjection:
						{
							param->Set(projection2DMatrix);
						}
						break;
						default:
							break;
					}
				}

				// Render current line batch
                //driver->SetViewport(camera->GetViewport());
				driver->SetRenderPipeline(m_materials[materialIdx]->GetRenderPipeline(), m_commandBuffer);
				driver->SetGpuParams(m_materials[materialIdx]->GetCommonGpuParams(), m_commandBuffer);
				driver->SetGpuParams(m_materials[materialIdx]->GetAutoGpuParams(), m_commandBuffer);
				driver->SetVertexBuffer(m_linesVertexBuffer, m_commandBuffer);
				driver->SetVertexDeclaration(m_materials[materialIdx]->GetRenderPipeline()->GetVertexInputDeclaration(), m_commandBuffer);
				driver->SetTopology(TOP_LINE_LIST, m_commandBuffer);
				//driver->SetScissorRect(m_lines.linesBatches[i].clipRect, m_commandBuffer);
				driver->Draw(m_lines.linesBatches[i].startIndex, 
							 m_lines.linesBatches[i].verticesCount, 1U, m_commandBuffer);

				drawCallsCount++;

			}


			// Render triangles
			// Upload vertex data to triagles vertex and index buffers
			verticesPointer = m_trianglesVertexArray.data();
			const void* indicesPointer = m_trianglesIndexArray.data();
			verticesDataSize = m_trianglesVertexArray.size() * sizeof(float);
			size_t indicesDataSize = m_trianglesIndexArray.size() * sizeof(u32);
			m_trianglesVertexBuffer->SetData(0U, verticesDataSize, verticesPointer);
			m_trianglesVertexBuffer->SetVerticesCount(m_triangles.verticesCount);
			m_trianglesIndexBuffer->SetData(0U, indicesDataSize, indicesPointer);
			m_trianglesIndexBuffer->SetIndicesCount(m_triangles.indicesCount);

			for (u32 i = 0U; i < m_triangles.trianglesBatches.size(); ++i)
			{
				if (m_triangles.trianglesBatches[i].indicesCount == 0)
					continue;
                
                //const auto& camera = m_cameras[m_triangles.trianglesBatches[i].cameraIndex];
                const sh::math::Matrix4& viewMatrix = camera->GetViewMatrix();
                const sh::math::Matrix4& projectionMatrix = camera->GetProjectionMatrix();
                const sh::math::Matrix4& projection2DMatrix = camera->Get2DProjectionMatrix();

				// Update current batch material params
				const u32 materialIdx = m_triangles.trianglesBatches[i].materialIndex;
				auto& params = m_materials[materialIdx]->GetAutoParams();
				for (u32 paramIdx = 0U; paramIdx < params->GetParamsCount(); ++paramIdx)
				{
					auto param = params->GetParam(paramIdx);
					switch (param->GetUsage())
					{
						case MaterialParamUsage::MatrixWorld:
							break;
						case MaterialParamUsage::MatrixView:
							param->Set(viewMatrix);
							break;
						case MaterialParamUsage::MatrixViewRotation:
						{
							param->Set(camera->GetRotationMatrix());
						}
						break;
						case MaterialParamUsage::MatrixViewRotationProjection:
							param->Set(( projectionMatrix * camera->GetRotationMatrix() ).GetTransposed());
							break;
						case MaterialParamUsage::MatrixProjection:
							param->Set(projectionMatrix);
							break;
						case MaterialParamUsage::MatrixViewProjection:
						{
							math::Matrix4 viewProjection = projectionMatrix * viewMatrix;
							param->Set(viewProjection);
						}
						break;
						case MaterialParamUsage::MatrixWorldViewProjection:
						{
							math::Matrix4 wvp = projectionMatrix * viewMatrix;
							wvp.Transpose();
							param->Set(wvp);
						}
						break;
						case MaterialParamUsage::Matrix2DProjection:
						{
							param->Set(projection2DMatrix);
						}
						break;
						default:
							break;
					}
				}

				// Render current triangle batch
                //driver->SetViewport(camera->GetViewport());
				driver->SetRenderPipeline(m_materials[materialIdx]->GetRenderPipeline(), m_commandBuffer);
				driver->SetGpuParams(m_materials[materialIdx]->GetCommonGpuParams(), m_commandBuffer);
				driver->SetGpuParams(m_materials[materialIdx]->GetAutoGpuParams(), m_commandBuffer);
				driver->SetVertexBuffer(m_trianglesVertexBuffer, m_commandBuffer);
				driver->SetIndexBuffer(m_trianglesIndexBuffer, m_commandBuffer);
				driver->SetVertexDeclaration(m_materials[materialIdx]->GetRenderPipeline()->GetVertexInputDeclaration(), m_commandBuffer);
				driver->SetTopology(TOP_TRIANGLE_LIST, m_commandBuffer);
                // Check driver's viewport
				//driver->SetScissorRect(m_triangles.trianglesBatches[i].clipRect, m_commandBuffer);
				driver->DrawIndexed(m_triangles.trianglesBatches[i].startIndex,
									m_triangles.trianglesBatches[i].indicesCount,
									1U,
									m_commandBuffer);
				drawCallsCount++;
			}

			m_commandBuffer->End();

			driver->SubmitCommandBuffer(m_commandBuffer);

			// Clear lines data
			m_linesVertexArray.clear();
			m_lines.linesBatches.clear();
			m_lines.verticesCount = 0U;

			// Clear triangles data
			m_trianglesVertexArray.clear();
			m_trianglesIndexArray.clear();
			m_triangles.trianglesBatches.clear();
			m_triangles.indicesCount = 0U;
			m_triangles.verticesCount = 0U;

			m_materials.clear();
            m_cameras.clear();

			// We must have at least one active material and camera any time
			SetMaterial(m_material);
            SetCamera(m_camera);
		}

		/////////////////////////////////////////////////////////////////////////////////////


	}
}
