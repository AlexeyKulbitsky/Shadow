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
			m_linesVertexArray.reserve(5000);
			video::VertexBufferDecription desc;
			desc.usage = USAGE_DYNAMIC;
			m_linesVertexBuffer = video::VertexBuffer::Create(desc);
			m_lines.linesBatches.reserve(100U);
			m_lines.verticesCount = 0U;
			
			// Create triangles data
			m_trianglesVertexArray.reserve(5000);
			m_trianglesIndexArray.reserve(5000);
			m_trianglesVertexBuffer = video::VertexBuffer::Create(desc);
			video::IndexBufferDescription indexDesc;
			indexDesc.indexType = IT_32_BIT;
			indexDesc.usage = USAGE_DYNAMIC;
			m_trianglesIndexBuffer = video::IndexBuffer::Create(indexDesc);
			m_triangles.trianglesBatches.reserve(100U);
			m_triangles.indicesCount = 0U;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::SetMaterial(const MaterialPtr& material)
		{
			m_material = material;

			m_materials.push_back(material);

			LinesBatch linesBatch;
			linesBatch.materialIndex = m_materials.size() - 1;
			linesBatch.startIndex = m_lines.verticesCount;
			m_lines.linesBatches.push_back(linesBatch);

			TrianglesBatch trianglesBatch;
			trianglesBatch.materialIndex = m_materials.size() - 1;
			trianglesBatch.startIndex = m_triangles.indicesCount;
			m_triangles.trianglesBatches.push_back(trianglesBatch);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::SetCamera(const scene::CameraPtr& camera)
		{
			m_camera = camera;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawLine(const math::Vector3f& a, const math::Vector3f& b)
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

		void Painter::DrawRect()
		{

		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawQuad()
		{

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

		void Painter::DrawBox(const math::AABBf& box)
		{
			const auto& a = box.minPoint;
			const auto& b = box.maxPoint;

			DrawLine(math::Vector3f(a.x, a.y, a.z), math::Vector3f(b.x, a.y, a.z));
			DrawLine(math::Vector3f(b.x, a.y, a.z), math::Vector3f(b.x, b.y, a.z));
			DrawLine(math::Vector3f(b.x, b.y, a.z), math::Vector3f(a.x, b.y, a.z));
			DrawLine(math::Vector3f(a.x, b.y, a.z), math::Vector3f(a.x, a.y, a.z));

			DrawLine(math::Vector3f(b.x, b.y, b.z), math::Vector3f(a.x, b.y, b.z));
			DrawLine(math::Vector3f(a.x, b.y, b.z), math::Vector3f(a.x, a.y, b.z));
			DrawLine(math::Vector3f(a.x, a.y, b.z), math::Vector3f(b.x, a.y, b.z));
			DrawLine(math::Vector3f(b.x, a.y, b.z), math::Vector3f(b.x, b.y, b.z));

			DrawLine(math::Vector3f(a.x, a.y, a.z), math::Vector3f(a.x, a.y, b.z));
			DrawLine(math::Vector3f(a.x, b.y, a.z), math::Vector3f(a.x, b.y, b.z));
			DrawLine(math::Vector3f(b.x, b.y, a.z), math::Vector3f(b.x, b.y, b.z));
			DrawLine(math::Vector3f(b.x, a.y, a.z), math::Vector3f(b.x, a.y, b.z));
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawText()
		{

		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::Flush()
		{
			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			sh::scene::Camera* camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();
			const sh::math::Matrix4f& viewMatrix = camera->GetViewMatrix();
			const sh::math::Matrix4f& projectionMatrix = camera->GetProjectionMatrix();

			m_commandBuffer->Begin();


			// Render lines
			const void* verticesPointer = m_linesVertexArray.data();
			size_t verticesDataSize = m_linesVertexArray.size() * sizeof(float);
			m_linesVertexBuffer->SetData(0U, verticesDataSize, verticesPointer);
			m_linesVertexBuffer->SetVerticesCount(m_lines.verticesCount);


			for (u32 i = 0U; i < m_lines.linesBatches.size(); ++i)
			{
				const u32 materialIdx = m_lines.linesBatches[i].materialIndex;
				auto& params = m_materials[materialIdx]->GetAutoParams();
				for (u32 paramIdx = 0U; paramIdx < params->GetParamsCount(); ++paramIdx)
				{
					auto& param = params->GetParam(paramIdx);
					switch (param.GetType())
					{
						case MaterialParamType::MatrixWorld:
							break;
						case MaterialParamType::MatrixView:
							param.Set(viewMatrix);
							break;
						case MaterialParamType::MatrixViewRotation:
						{
							param.Set(camera->GetRotationMatrix());
						}
						break;
						case MaterialParamType::MatrixViewRotationProjection:
							param.Set(( projectionMatrix * camera->GetRotationMatrix() ).GetTransposed());
							break;
						case MaterialParamType::MatrixProjection:
							param.Set(projectionMatrix);
							break;
						case MaterialParamType::MatrixViewProjection:
						{
							math::Matrix4f viewProjection = projectionMatrix * viewMatrix;
							param.Set(viewProjection);
						}
						break;
						case MaterialParamType::MatrixWorldViewProjection:
						{
							math::Matrix4f wvp = projectionMatrix * viewMatrix;
							wvp.Transpose();
							param.Set(wvp);
						}
						break;
						default:
							break;
					}
				}

				driver->SetRenderPipeline(m_materials[materialIdx]->GetRenderPipeline(), m_commandBuffer);
				driver->SetGpuParams(m_materials[materialIdx]->GetCommonGpuParams(), m_commandBuffer);
				driver->SetGpuParams(m_materials[materialIdx]->GetAutoGpuParams(), m_commandBuffer);
				driver->SetVertexBuffer(m_linesVertexBuffer, m_commandBuffer);
				driver->SetVertexDeclaration(m_materials[materialIdx]->GetRenderPipeline()->GetVertexInputDeclaration(), m_commandBuffer);
				driver->SetTopology(TOP_LINE_LIST, m_commandBuffer);
				driver->Draw(m_lines.linesBatches[i].startIndex, 
							 m_lines.linesBatches[i].verticesCount, 1U, m_commandBuffer);

			}





			/*

			const void* verticesPointer = m_linesVertexArray.data();
			size_t verticesDataSize = m_linesVertexArray.size() * sizeof(float);
			m_linesVertexBuffer->SetData(0U, verticesDataSize, verticesPointer);
			m_linesVertexBuffer->SetVerticesCount(m_linesBatch.verticesCount);


			


			auto& params = m_material->GetAutoParams();
			for (u32 paramIdx = 0U; paramIdx < params->GetParamsCount(); ++paramIdx)
			{
				auto& param = params->GetParam(paramIdx);
				switch (param.GetType())
				{
					case MaterialParamType::MatrixWorld:
						break;
					case MaterialParamType::MatrixView:
						param.Set(viewMatrix);
						break;
					case MaterialParamType::MatrixViewRotation:
					{
						param.Set(camera->GetRotationMatrix());
					}
					break;
					case MaterialParamType::MatrixViewRotationProjection:
						param.Set(( projectionMatrix * camera->GetRotationMatrix() ).GetTransposed());
						break;
					case MaterialParamType::MatrixProjection:
						param.Set(projectionMatrix);
						break;
					case MaterialParamType::MatrixViewProjection:
					{
						math::Matrix4f viewProjection = projectionMatrix * viewMatrix;
						param.Set(viewProjection);
					}
					break;
					case MaterialParamType::MatrixWorldViewProjection:
					{
						math::Matrix4f wvp = projectionMatrix * viewMatrix;
						wvp.Transpose();
						param.Set(wvp);
					}
					break;
					default:
						break;
				}
			}


			m_commandBuffer->Begin();

			driver->SetRenderPipeline(m_material->GetRenderPipeline(), m_commandBuffer);
			driver->SetGpuParams(m_material->GetCommonGpuParams(), m_commandBuffer);
			driver->SetGpuParams(m_material->GetAutoGpuParams(), m_commandBuffer);
			driver->SetVertexBuffer(m_linesVertexBuffer, m_commandBuffer);
			driver->SetVertexDeclaration(m_material->GetRenderPipeline()->GetVertexInputDeclaration(), m_commandBuffer);
			driver->SetTopology(TOP_LINE_LIST, m_commandBuffer);
			driver->Draw(0, m_linesVertexBuffer->GetVerticesCount(), 1U, m_commandBuffer);

			*/

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
			
			//m_linesBatch.verticesCount = 0U;
		}

		/////////////////////////////////////////////////////////////////////////////////////


	}
}