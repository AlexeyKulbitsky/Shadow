#ifndef SHADOW_VULKAN_INDEX_BUFFER_INCLUDE
#define SHADOW_VULKAN_INDEX_BUFFER_INCLUDE

#include "../IndexBuffer.h"
#include "VulkanDeleter.h"

namespace sh
{
	namespace video
	{
		class VulkanIndexBuffer : public IndexBuffer
		{
		public:
			VulkanIndexBuffer(Usage usage);
			virtual ~VulkanIndexBuffer();

			virtual void SetData(size_t offset, size_t length, const void* data) override;
			virtual void GetData(size_t offset, size_t length, const void* data) override;

			VkBuffer GetVulkanId() const { return m_indexBuffer; }

		private:
			void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
			void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		private:
			VkBuffer m_indexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory m_indexBufferMemory = VK_NULL_HANDLE;
		};
	}
}

#endif
