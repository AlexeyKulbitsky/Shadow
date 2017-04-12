#include "GLES20RenderBatch.h"
#include "ShaderProgram.h"
#include "UniformBuffer.h"
#include "IndexBuffer.h"
#include "Driver.h"
#include "../scene/Mesh.h"
#include "../Device.h"


namespace sh
{
	namespace video
	{
		void GLES20RenderBatch::SetDepthStencilState(const DepthStencilStatePtr& depthStencilState)
		{
			m_depthStencilState = depthStencilState;
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::SetRasterizationState(const RasterizationStatePtr& rasterizationState)
		{
			m_rasterizationState = rasterizationState;
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::SetBlendingState(const BlendingStatePtr& blendingState)
		{
			m_blendingState = blendingState;
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::SetShaderProgram(const ShaderProgramPtr& shaderProgram)
		{
			m_program = shaderProgram;
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::SetUniformBuffer(const UniformBufferPtr& uniformBuffer)
		{
			m_uniformBuffer = uniformBuffer;
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::SetRenderPipeline(const RenderPipelinePtr& pipeline)
		{
			m_pipeline = pipeline;
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::AddMesh(const scene::MeshPtr& mesh)
		{
			m_meshes.push_back(mesh);
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::Submit()
		{
			Driver* driver = Device::GetInstance()->GetDriver();

			driver->SetRenderPipeline(m_pipeline);

			for (const auto& mesh : m_meshes)
			{
				driver->SetGpuParams(mesh->GetGpuParams());
				driver->SetGpuParams(mesh->GetAutoGpuParams());
				driver->SetVertexDeclaration(mesh->GetVertexDeclaration());
				driver->SetIndexBuffer(mesh->GetIndexBuffer());
				driver->SetVertexBuffer(mesh->GetVertexBuffer());
				driver->DrawIndexed(0, mesh->GetIndexBuffer()->GetIndicesCount());
			}
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::Clear()
		{

		}

		//////////////////////////////////////////////////////////////////////////

	}
}