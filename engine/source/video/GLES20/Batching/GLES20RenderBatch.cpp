#include "GLES20RenderBatch.h"
#include "../../IndexBuffer.h"
#include "../../Driver.h"
#include "../../../scene/Mesh.h"
#include "../../../scene/Camera.h"
#include "../../../scene/SceneManager.h"
#include "../../Renderable.h"
#include "../../../Device.h"
#include "../../MaterialParams.h"

#include "../../../scene/Light.h"


namespace sh
{
	namespace video
	{

		void GLES20RenderBatch::SetRenderPipeline(const RenderPipelinePtr& pipeline)
		{
			m_pipeline = pipeline;
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::SetGpuParams(const GpuParamsPtr& gpuParams)
		{
			m_gpuParams = gpuParams;
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::SetMaterialParams(const MaterialParamsPtr& materialParams)
		{
			m_materialParams = materialParams;
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
				driver->SetTopology(renderable->GetTopology());
				driver->DrawIndexed(0, renderable->GetIndexBuffer()->GetIndicesCount());
			}
			
		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::Clear()
		{

		}

		//////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatch::UpdateLight(scene::Light* light)
		{
			auto type = light->GetType();
			switch (type)
			{
				case scene::Light::Type::DIRECTIONAL:
				{
					for (size_t paramIdx = 0; paramIdx < m_materialParams->GetParamsCount(); ++paramIdx)
					{
						auto& param = m_materialParams->GetParam(paramIdx);
						switch (param.GetType())
						{
							case MaterialParamType::DirectionalLightDirection:
								param.Set(light->GetDirection());
								break;
							case MaterialParamType::DirectionalLightColor:
								param.Set(light->GetColor());
								break;
							default:
								break;
						}
					}
				}
					break;
				case scene::Light::Type::POINT:
					break;
				case scene::Light::Type::AMBIENT:
					break;
				case scene::Light::Type::SPOT:
					break;
				default:
					break;
			}
		}

		//////////////////////////////////////////////////////////////////////////

	}
}