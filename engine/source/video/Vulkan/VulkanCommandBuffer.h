#ifndef SHADOW_VULKAN_COMMAND_BUFFER_INCLUDE
#define SHADOW_VULKAN_COMMAND_BUFFER_INCLUDE

#include "../CommandBuffer.h"
#include "VulkanCommon.h" 

namespace sh
{

namespace video
{
	
	class VulkanCommandBuffer : public CommandBuffer
	{
		friend class VulkanDriver;
	public:
		virtual ~VulkanCommandBuffer();

		void Begin();
		void End();
		void BeginRenderPass();
		void EndRenderPass();

		VkCommandBuffer GetVulkanId() { return m_commandBuffer; }

	private:
		VulkanCommandBuffer(const CommandBufferDescription& description);

	private:
		VkCommandBuffer m_commandBuffer;
	};

} // video

} // sh

#endif