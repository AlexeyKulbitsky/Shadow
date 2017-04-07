#include "VulkanRenderStateManager.h"

#include "../VulkanRenderPipeline.h"

namespace sh
{

namespace video
{

	RenderPipelinePtr VulkanRenderStateManager::CreateRenderPipeline(const RenderPipelineDescription& description)
	{
		RenderPipelinePtr result = nullptr;
		result.reset(new VulkanRenderPipeline(description));
		return result;
	}

	void VulkanRenderStateManager::CreateInstance()
	{
		s_instance = new VulkanRenderStateManager();
	}

	void VulkanRenderStateManager::DestroyInstance()
	{
		delete s_instance;
		s_instance = nullptr;
	}

} // video

} // sh