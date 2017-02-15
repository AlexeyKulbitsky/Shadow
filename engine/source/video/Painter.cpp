#include "Painter.h"
#include "../Device.h"
#include "RenderCommand.h"
#include "Material.h"
#include "RenderPipeline.h"
#include "VertexDeclaration.h"
#include "ShaderProgram.h"

namespace sh
{
	namespace video
	{
		Painter::Painter()
		{
			m_linesVertexArray.reserve(5000);
			Driver* driver = Device::GetInstance()->GetDriver();
			m_linesRenderCommand = driver->CreateRenderCommand();
			m_linesRenderCommand->SetVertexBuffer(m_linesBuffer.get());
			m_linesRenderCommand->SetTopology(Topology::LINE_LIST);
			m_linesRenderCommand->SetUseIndices(false);
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::SetMaterial(const MaterialPtr& material)
		{
			m_material = material;

			RenderPipeline* renderPipeline = m_material->GetRenderPipeline(0);

			const UniformBufferPtr& uniformBuffer = renderPipeline->GetUniformBuffer();
			m_linesRenderCommand->SetUniformBuffer(uniformBuffer.get());

			VertexDeclaration vertexDeclaration;
			Attribute positionAttribute(AttributeSemantic::POSITION, AttributeType::FLOAT, 3U);
			vertexDeclaration.AddAttribute(positionAttribute);

			VertexInputDeclaration* inputDeclaration = renderPipeline->GetVertexInputDeclaration();
			inputDeclaration->Assemble(vertexDeclaration);
			m_linesRenderCommand->SetVertexInputDeclaration(inputDeclaration);
		}

		/////////////////////////////////////////////////////////////////////////////////////
		
		void Painter::SetCamera(const scene::CameraPtr& camera)
		{
			m_camera = camera;
		}

		/////////////////////////////////////////////////////////////////////////////////////

		void Painter::DrawLine(const math::Vector3f& a, const math::Vector3f& b)
		{
			m_linesVertexArray.push_back(a.x);
			m_linesVertexArray.push_back(a.y);
			m_linesVertexArray.push_back(a.z);

			m_linesVertexArray.push_back(b.x);
			m_linesVertexArray.push_back(b.y);
			m_linesVertexArray.push_back(b.z);
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
			Driver* driver = Device::GetInstance()->GetDriver();
			RenderPipeline* renderPipeline = m_material->GetRenderPipeline(0);

			driver->SetDepthStencilState(renderPipeline->GetDepthStencilState());
			driver->SetBlendingState(renderPipeline->GetBlendingState());
			driver->SetRasterizationState(renderPipeline->GetRasterizationState());

			renderPipeline->GetShaderProgram()->BindProgram();

			driver->Render(m_linesRenderCommand);

			renderPipeline->GetShaderProgram()->UnbindProgram();

			m_linesVertexArray.clear();
		}

		/////////////////////////////////////////////////////////////////////////////////////


	}
}