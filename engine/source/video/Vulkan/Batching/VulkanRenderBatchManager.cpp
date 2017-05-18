#include "VulkanRenderBatchManager.h"

#include "../../../scene/Mesh.h"
#include "../../../Device.h"
#include "../../Material.h"
#include "../../Renderable.h"
#include "../../RenderTechnique.h"
#include "../VulkanDriver.h"
#include "../VulkanCommandBuffer.h"

namespace sh
{

namespace video
{
	VulkanRenderBatchManager::VulkanRenderBatchManager()
	{
	}

	VulkanRenderBatchManager::~VulkanRenderBatchManager()
	{
	}

	void VulkanRenderBatchManager::AddMesh(const scene::MeshPtr& mesh)
	{
		const MaterialPtr& material = mesh->GetMaterial();
		String techniqueName = material->GetRenderTechnique()->GetName();
		RenderPipeline::Layer layer = material->GetRenderPipeline(0)->GetLayer();
		size_t layerIndex = (size_t)layer;

		if (m_renderBatches[layerIndex].find(techniqueName) == m_renderBatches[layerIndex].end())
		{
			VulkanRenderBatchPtr renderBatch(new VulkanRenderBatch());
			const RenderPipelinePtr& renderPipeline = material->GetRenderPipeline(0);
			renderBatch->SetTechniqueName(techniqueName);
			renderBatch->SetRenderPipeline(renderPipeline);
			renderBatch->SetGpuParams(material->GetCommonGpuParams());

			m_renderBatches[layerIndex][techniqueName] = renderBatch;
		}

		m_renderBatches[layerIndex][techniqueName]->AddMesh(mesh);
	}

	void VulkanRenderBatchManager::Submit()
	{
		for (size_t i = 0; i < (size_t)RenderPipeline::Layer::COUNT; ++i)
		{
			for (auto renderBatch : m_renderBatches[i])
			{
				renderBatch.second->Submit();
			}
		}
	}

	void VulkanRenderBatchManager::Clear()
	{
		for (size_t i = 0; i < (size_t)RenderPipeline::Layer::COUNT; ++i)
		{
			m_renderBatches[i].clear();
		}
	}

} // video

} // sh