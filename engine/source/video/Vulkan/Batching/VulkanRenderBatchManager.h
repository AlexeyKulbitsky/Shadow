#ifndef SHADOW_VULKAN_RENDER_BATCH_MANAGER_INCLUDE
#define SHADOW_VULKAN_RENDER_BATCH_MANAGER_INCLUDE

#include "../../RenderBatchManager.h"
#include "VulkanRenderBatch.h"
#include "VulkanRenderPipeline.h"

namespace sh
{

namespace video
{
	DEFINE_CLASS_PTR(VulkanRenderBatch)

	class VulkanRenderBatchManager : public RenderBatchManager
	{
	public:
		VulkanRenderBatchManager();
		virtual ~VulkanRenderBatchManager();
		virtual void AddMesh(const scene::MeshPtr& mesh) override;
		virtual void Submit() override;
		virtual void Clear() override;

	private:
		typedef Map<String, VulkanRenderBatchPtr> PerTechniqueRenderBatchesMap;

		PerTechniqueRenderBatchesMap m_renderBatches[(size_t)RenderPipeline::Layer::COUNT];
	};

} // video

} // sh

#endif
