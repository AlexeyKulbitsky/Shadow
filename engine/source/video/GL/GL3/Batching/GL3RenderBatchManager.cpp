#include "GL3RenderBatchManager.h"

#include "GL3RenderBatch.h"
#include "../../../Material.h"
#include "../../../RenderTechnique.h"
#include "../../../RenderPipeline.h"
#include "../../../../scene/Mesh.h"
#include "../../../../io/FileSystem.h"

namespace sh
{
	namespace video
	{
		GL3RenderBatchManager::~GL3RenderBatchManager()
		{

		}

		////////////////////////////////////////////////////////////////////////

		void GL3RenderBatchManager::AddMesh(const scene::MeshPtr& mesh)
		{
			const MaterialPtr& material = mesh->GetMaterial();
			String techniqueName = material->GetRenderTechnique()->GetName();
			RenderLayer layer = material->GetRenderPipeline(0)->GetRenderLayer();
			size_t layerIndex = (size_t)layer;

			if (m_renderBatches[layerIndex].find(material->GetFileInfo().lock()->name) == m_renderBatches[layerIndex].end())
			{
				GL3RenderBatchPtr renderBatch(new GL3RenderBatch());
				const RenderPipelinePtr& renderPipeline = material->GetRenderPipeline(0);
				renderBatch->SetTechniqueName(techniqueName);
				renderBatch->SetRenderPipeline(renderPipeline);
				renderBatch->SetGpuParams(material->GetCommonGpuParams());
				renderBatch->SetMaterialParams(material->GetParams());

				m_renderBatches[layerIndex][material->GetFileInfo().lock()->name] = renderBatch;
			}

			m_renderBatches[layerIndex][material->GetFileInfo().lock()->name]->AddMesh(mesh);
		}

		////////////////////////////////////////////////////////////////////////

		void GL3RenderBatchManager::RemoveMesh(const scene::MeshPtr& mesh)
		{
			const MaterialPtr& material = mesh->GetMaterial();
			String techniqueName = material->GetRenderTechnique()->GetName();
			RenderLayer layer = material->GetRenderPipeline(0)->GetRenderLayer();
			size_t layerIndex = (size_t)layer;

			m_renderBatches[layerIndex][material->GetFileInfo().lock()->name]->RemoveMesh(mesh);
		}

		////////////////////////////////////////////////////////////////////////

		void GL3RenderBatchManager::Submit()
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

		void GL3RenderBatchManager::Clear()
		{
			for (size_t i = 0; i < (size_t)RenderPipeline::Layer::COUNT; ++i)
			{
				m_renderBatches[i].clear();
			}
		}

		////////////////////////////////////////////////////////////////////////

		void GL3RenderBatchManager::UpdateLight(scene::Light* light)
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