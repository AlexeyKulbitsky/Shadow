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
		virtual ShaderPtr CreateShader(const ShaderDescription& description) override;

	};

} // video

} // sh

#endif