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
			m_linesVertexArray.reserve(5000);
			Driver* driver = Device::GetInstance()->GetDriver();

			// Create lines batch
			video::VertexBufferDecription desc;
			desc.usage = USAGE_DYNAMIC;
			m_linesRenderable.vertexBuffer = video::VertexBuffer::Create(desc);

			sh::video::VertexDeclarationPtr vertexDeclaration = sh::video::VertexDeclarationPtr(new sh::video::VertexDeclaration());
			sh::video::Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			sh::video::Attribute colorAttribute(AttributeSemantic::COLOR, AttributeType::FLOAT, 3U);
			vertexDeclaration->AddAttribute(positionAttribute);
			vertexDeclaration->AddAttribute(colorAttribute);

			m_linesRenderable.vertexBuffer->SetVertexSize(vertexDeclaration->GetStride());
			m_linesRenderable.vertexBuffer->SetVertexDeclaration(vertexDeclaration);

			sh::video::CommandBufferDescription commandBufferDesc;
			commandBufferDesc.type = sh::COMMAND_BUFFER_TYPE_SECONDARY;
			m_linesRenderable.commandBuffer = sh::video::CommandBuffer::Create(commandBufferDesc);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::SetMaterial(const MaterialPtr& material)
		{
			m_material = material;

			m_linesBatch.inputDeclaration = m_material->GetRenderPipeline()->GetVertexInputDeclaration()->Clone();
			m_linesBatch.inputDeclaration->Assemble(*(m_linesRenderable.vertexBuffer->GetVertexDeclaration().get()));
			m_material->GetRenderPipeline()->Init(m_linesBatch.inputDeclaration);

			const auto& info = m_material->GetRenderPipeline()->GetAutoParamsInfo();

			m_linesRenderable.params = sh::video::GpuParams::Create(info);
			m_linesRenderable.params->GetParam("matWVP", m_linesRenderable.wvpMatrix);
		}

		/////////////////////////////////////////////////////////////////////////////////////
		
		void Painter::SetCamera(const scene::CameraPtr& camera)
		{
			m_camera = camera;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawLine(const math::Vector3f& a, const math::Vector3f& b)
		{
			// Start point position
			m_linesVertexArray.push_back(a.x);
			m_linesVertexArray.push_back(a.y);
			m_linesVertexArray.push_back(a.z);

			// Start point color
			m_linesVertexArray.push_back(1.0f);
			m_linesVertexArray.push_back(0.0f);
			m_linesVertexArray.push_back(0.0f);

			// End point position
			m_linesVertexArray.push_back(b.x);
			m_linesVertexArray.push_back(b.y);
			m_linesVertexArray.push_back(b.z);

			// End point color
			m_linesVertexArray.push_back(1.0f);
			m_linesVertexArray.push_back(0.0f);
			m_linesVertexArray.push_back(0.0f);

			m_linesBatch.verticesCount += 2U;
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

		void Painter::DrawBox()
		{

		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawText()
		{

		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::Flush()
		{
			const void* verticesPointer = m_linesVertexArray.data();
			size_t verticesDataSize = m_linesVertexArray.size() * sizeof(float);
			m_linesRenderable.vertexBuffer->SetData(0U, verticesDataSize, verticesPointer);
			m_linesRenderable.vertexBuffer->SetVerticesCount(m_linesBatch.verticesCount);


			sh::video::Driver* driver = sh::Device::GetInstance()->GetDriver();
			sh::scene::Camera* camera = sh::Device::GetInstance()->GetSceneManager()->GetCamera();
			sh::math::Matrix4f viewMatrix = camera->GetViewMatrix();
			sh::math::Matrix4f projectionMatrix = camera->GetProjectionMatrix();

			sh::math::Matrix4f wvpMatrix = projectionMatrix * viewMatrix;
			wvpMatrix.Transpose();
			m_linesRenderable.wvpMatrix.Set(wvpMatrix);

			m_linesRenderable.commandBuffer->Begin();

			driver->SetRenderPipeline(m_material->GetRenderPipeline(), m_linesRenderable.commandBuffer);
			driver->SetGpuParams(m_linesRenderable.params, m_linesRenderable.commandBuffer);
			driver->SetVertexBuffer(m_linesRenderable.vertexBuffer, m_linesRenderable.commandBuffer);
			driver->SetVertexDeclaration(m_linesBatch.inputDeclaration, m_linesRenderable.commandBuffer);
			driver->SetTopology(TOP_LINE_LIST, m_linesRenderable.commandBuffer);
			driver->Draw(0, m_linesRenderable.vertexBuffer->GetVerticesCount(), 1U, m_linesRenderable.commandBuffer);

			m_linesRenderable.commandBuffer->End();

			m_linesVertexArray.clear();
		}

		/////////////////////////////////////////////////////////////////////////////////////


	}
}