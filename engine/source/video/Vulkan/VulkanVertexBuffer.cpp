#include "VulkanVertexBuffer.h"

namespace sh
{
	namespace video
	{
		VulkanVertexBuffer::VulkanVertexBuffer()
		{

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VulkanVertexBuffer::~VulkanVertexBuffer()
		{
			vkFreeMemory(m_device, m_vertexBufferMemory, nullptr);
			vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanVertexBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
		{
			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = size;
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			SH_ASSERT(vkCreateBuffer(m_device, &bufferInfo, nullptr, &buffer) == VK_SUCCESS, "failed to create buffer!");

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(m_device, buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

			SH_ASSERT(vkAllocateMemory(m_device, &allocInfo, nullptr, &bufferMemory) == VK_SUCCESS, "failed to allocate vertex buffer memory!");

			vkBindBufferMemory(m_device, buffer, bufferMemory, 0);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		uint32_t VulkanVertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
		{
			return 0U;
		}

	}
}




