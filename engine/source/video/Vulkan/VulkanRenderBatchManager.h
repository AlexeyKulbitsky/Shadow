#ifndef SHADOW_VULKAN_RENDER_BATCH_MANAGER_INCLUDE
#define SHADOW_VULKAN_RENDER_BATCH_MANAGER_INCLUDE

#include "../RenderBatchManager.h"

namespace sh
{

namespace video
{

	class VulkanRenderBatchManager : public RenderBatchManager
	{
	public:
		VulkanRenderBatchManager();
		virtual ~VulkanRenderBatchManager();
		virtual void AddMesh(const scene::MeshPtr& mesh) override;
		virtual void Submit() override;
		virtual void Clear() override;

	private:
		std::vector<scene::MeshPtr> m_meshes;
		CommandBufferPtr m_commandBuffer;
	};

} // video

} // sh

#endif
