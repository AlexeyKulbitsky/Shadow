#ifndef SHADOW_VULKAN_RENDER_STATE_MANAGER_INCLUDE
#define SHADOW_VULKAN_RENDER_STATE_MANAGER_INCLUDE

#include "../../Managers/RenderStateManager.h"

namespace sh
{

namespace video
{

	class VulkanRenderStateManager : public RenderStateManager
	{
		friend class VulkanDriver;
	public:
		virtual RenderPipelinePtr CreateRenderPipeline(const RenderPipelineDescription& description) override;

	private:
		static void CreateInstance();
		static void DestroyInstance();
	};

} // video

} // sh

#endif