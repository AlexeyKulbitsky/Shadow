#include "GLES20RenderBatchManager.h"

#include "GLES20RenderBatch.h"
#include "../../Material.h"
#include "../../RenderTechnique.h"
#include "../../RenderPipeline.h"
#include "../../../scene/Mesh.h"

namespace sh
{
	namespace video
	{
		GLES20RenderBatchManager::~GLES20RenderBatchManager()
		{

		}

		////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatchManager::AddMesh(const scene::MeshPtr& mesh)
		{
			const MaterialPtr& material = mesh->GetMaterial();
			String techniqueName = material->GetRenderTechnique()->GetName();
			RenderLayer layer = material->GetRenderPipeline(0)->GetRenderLayer();
			size_t layerIndex = (size_t)layer;

			if (m_renderBatches[layerIndex].find(techniqueName) == m_renderBatches[layerIndex].end())
			{
				GLES20RenderBatchPtr renderBatch(new GLES20RenderBatch());
				const RenderPipelinePtr& renderPipeline = material->GetRenderPipeline(0);
				renderBatch->SetTechniqueName(techniqueName);
				renderBatch->SetRenderPipeline(renderPipeline);
				renderBatch->SetGpuParams(material->GetCommonGpuParams());
				renderBatch->SetMaterialParams(material->GetParams());

				m_renderBatches[layerIndex][techniqueName] = renderBatch;
			}

			m_renderBatches[layerIndex][techniqueName]->AddMesh(mesh);
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatchManager::Submit()
		{
			for (size_t i = 0; i < (size_t)RenderPipeline::Layer::COUNT; ++i)
			{
				for (auto renderBatch : m_renderBatches[i])
				{
					renderBatch.second->Submit();
				}
			}
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatchManager::Clear()
		{
			for (size_t i = 0; i < (size_t)RenderPipeline::Layer::COUNT; ++i)
			{
				m_renderBatches[i].clear();
			}
		}

		////////////////////////////////////////////////////////////////////////

		void GLES20RenderBatchManager::UpdateLight(scene::Light* light)
		{
			for (size_t i = 0; i < (size_t)RenderPipeline::Layer::COUNT; ++i)
			{
				for (auto renderBatch : m_renderBatches[i])
				{
					renderBatch.second->UpdateLight(light);
				}
			}
		}
	}
}