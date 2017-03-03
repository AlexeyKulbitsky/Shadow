#ifndef SHADOW_VULKAN_VERTEX_BUFFER_INCLUDE
#define SHADOW_VULKAN_VERTEX_BUFFER_INCLUDE

#include "../VertexBuffer.h"
#include "VulkanDeleter.h"

namespace sh
{
	namespace video
	{
		class VulkanVertexBuffer : public VertexBuffer
		{
		public:
			VulkanVertexBuffer(Usage usage);
			virtual ~VulkanVertexBuffer();

			virtual void SetData(size_t offset, size_t length, const void* data) {}
			virtual void GetData(size_t offset, size_t length, const void* data) {}

		private:
			void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
			uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		private:
			VkDevice m_device = VK_NULL_HANDLE;
			VkBuffer m_vertexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory m_vertexBufferMemory = VK_NULL_HANDLE;
		};
	}
}
#endif // !VULKAN_VERTEX_BUFFER_INCLUDE
