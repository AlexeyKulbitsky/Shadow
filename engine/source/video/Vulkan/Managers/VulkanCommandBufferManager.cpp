#include "VulkanCommandBufferManager.h"
#include "../VulkanCommandBuffer.h"

namespace sh
{

namespace video
{

	CommandBufferPtr VulkanCommandBufferManager::CreateCommandBuffer(const CommandBufferDescription& description)
	{
		CommandBufferPtr result;
		result.reset(new VulkanCommandBuffer(description));
		return result;
	}

} // video

} // sh