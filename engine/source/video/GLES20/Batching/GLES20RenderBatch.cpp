#include "GLES20RenderBatch.h"
#include "IndexBuffer.h"
#include "Driver.h"
#include "../scene/Mesh.h"
#include "../../../scene/Camera.h"
#include "../../../scene/SceneManager.h"
#include "../../Renderable.h"
#include "../Device.h"


namespace sh
{
	namespace video
	{

		void GLES20RenderBatch::SetRenderPipeline(const RenderPipelinePtr& pipeline)
		{
			m_pipeline = pipeline;
		}

		void GLES20RenderBatch::SetGpuParams(const GpuParamsPtr& gpuParams)
		{
			m_gpuParams = gpuParams;
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::AddMesh(const scene::MeshPtr& mesh)
		{
			m_renderables.push_back(mesh->GetRanderable());
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::Submit()
		{
			Driver* driver = Device::GetInstance()->GetDriver();

			driver->SetRenderPipeline(m_pipeline);
			driver->SetGpuParams(m_gpuParams);

			for (const auto& renderable : m_renderables)
			{
				driver->SetGpuParams(renderable->GetAutoGpuParams());
				driver->SetVertexBuffer(renderable->GetVertexBuffer());
				driver->SetVertexDeclaration(renderable->GetVertexInputDeclaration());
				driver->SetIndexBuffer(renderable->GetIndexBuffer());				
				driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount());
			}
			
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::Clear()
		{

		}

		//////////////////////////////////////////////////////////////////////////

	}
}