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
		friend class VulkanCommandBufferManager;
	public:
		virtual ~VulkanCommandBuffer();

		virtual void Begin() override;
		virtual void End() override;
		void BeginRenderPass();
		void EndRenderPass();

		void Append(VulkanCommandBuffer* secondary);
		void Execute();

		VkCommandBuffer GetVulkanId() { return m_commandBuffer; }

	private:
		//void SetGpuParams(VulkanGpuParams* params) { }
		//void SetRenderPipeline(VulkanRenderPipeline* pipeline) { }

	private:
		VulkanCommandBuffer(const CommandBufferDescription& description);
		VulkanCommandBuffer() { }

	private:
		VkCommandBuffer m_commandBuffer;

		std::vector<VkCommandBuffer> m_secondaryCommandBuffers;

		struct
		{

		} m_cache;
	};

} // video

} // sh

#endif