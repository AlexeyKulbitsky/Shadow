#ifndef SHADOW_VULKAN_BUFFER_INCLUDE
#define SHADOW_VULKAN_BUFFER_INCLUDE

#include "VulkanDeleter.h"

namespace sh
{
	namespace video
	{
		class VulkanBuffer
		{
		public:
			VkBuffer GetVulkanId() const { return m_buffer; }
			VkDeviceMemory GetVulkanMemoryId() const { return m_bufferMemory; }

		protected:
			void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
			void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		protected:
			VkBuffer m_buffer = VK_NULL_HANDLE;
			VkDeviceMemory m_bufferMemory = VK_NULL_HANDLE;
		};
	}
}

#endif
