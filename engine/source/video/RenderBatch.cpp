#include "RenderBatch.h"
#include "ShaderProgram.h"
#include "UniformBuffer.h"
#include "RenderCommand.h"
#include "IndexBuffer.h"
#include "Driver.h"
#include "../scene/Mesh.h"
#include "../Device.h"


namespace sh
{
	namespace video
	{
		void RenderBatch::SetDepthStencilState(const DepthStencilStatePtr& depthStencilState)
		{
			m_depthStencilState = depthStencilState;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetRasterizationState(const RasterizationStatePtr& rasterizationState)
		{
			m_rasterizationState = rasterizationState;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetBlendingState(const BlendingStatePtr& blendingState)
		{
			m_blendingState = blendingState;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetShaderProgram(const ShaderProgramPtr& shaderProgram)
		{
			m_program = shaderProgram;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetUniformBuffer(const UniformBufferPtr& uniformBuffer)
		{
			m_uniformBuffer = uniformBuffer;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::SetRenderPipeline(const RenderPipelinePtr& pipeline)
		{
			m_pipeline = pipeline;
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::AddMesh(const scene::MeshPtr& mesh)
		{
			m_meshes.push_back(mesh);
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::Submit()
		{
			Driver* driver = Device::GetInstance()->GetDriver();

			driver->SetRenderPipeline(m_pipeline);

			for (const auto& mesh : m_meshes)
			{
				driver->SetGpuParams(mesh->GetGpuParams());
				driver->SetVertexDeclaration(mesh->GetVertexDeclaration());
				driver->SetIndexBuffer(mesh->GetIndexBuffer());
				driver->SetVertexBuffer(mesh->GetVertexBuffer());
				driver->DrawIndexed(0, mesh->GetIndexBuffer()->GetIndicesCount());
			}
		}

		//////////////////////////////////////////////////////////////////////////

		void RenderBatch::Clear()
		{

		}

		//////////////////////////////////////////////////////////////////////////

	}
}