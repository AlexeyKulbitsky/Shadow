#ifndef SHADOW_VULKAN_COMMAND_BUFFER_MANAGER_INCLUDE
#define SHADOW_VULKAN_COMMAND_BUFFER_MANAGER_INCLUDE

#include "../../Managers/CommandBufferManager.h"

namespace sh
{

namespace video
{
	
	class VulkanCommandBufferManager : public CommandBufferManager
	{
	public:
		virtual CommandBufferPtr CreateCommandBuffer(const CommandBufferDescription& description) override;
	};

} // video

} // sh

#endif