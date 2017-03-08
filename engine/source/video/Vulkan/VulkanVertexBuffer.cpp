#include "VulkanVertexBuffer.h"
#include "../../Device.h"
#include "VulkanDriver.h"


namespace sh
{
	namespace video
	{
		VulkanVertexBuffer::VulkanVertexBuffer(Usage usage) : VertexBuffer(usage)
		{
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VulkanVertexBuffer::~VulkanVertexBuffer()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			vkFreeMemory(device, m_vertexBufferMemory, nullptr);
			vkDestroyBuffer(device, m_vertexBuffer, nullptr);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanVertexBuffer::SetData(size_t offset, size_t length, const void* data)
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			// Create temporary buffer
			VkDeviceSize bufferSize = length;
			VkBuffer stagingBuffer;
			VkDeviceMemory stagingBufferMemory;
			createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

			// Fill temporary buffer with data
			void* bufferData;
			vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &bufferData);
			memcpy(bufferData, data, (size_t)bufferSize);
			vkUnmapMemory(device, stagingBufferMemory);

			// Create fina buffer
			createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);
			// Copy data from temporary buffer to final buffer
			copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

			// Free temporary buffer
			vkFreeMemory(device, stagingBufferMemory, nullptr);
			vkDestroyBuffer(device, stagingBuffer, nullptr);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanVertexBuffer::GetData(size_t offset, size_t length, const void* data)
		{

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanVertexBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			VkBufferCreateInfo bufferInfo = {};
			bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferInfo.size = size;
			bufferInfo.usage = usage;
			bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			SH_ASSERT(vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) == VK_SUCCESS, "failed to create buffer!");

			VkMemoryRequirements memRequirements;
			vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

			VkMemoryAllocateInfo allocInfo = {};
			allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			allocInfo.allocationSize = memRequirements.size;
			allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

			SH_ASSERT(vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) == VK_SUCCESS, "failed to allocate vertex buffer memory!");

			vkBindBufferMemory(device, buffer, bufferMemory, 0);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanVertexBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkCommandBuffer commandBuffer = driver->beginSingleTimeCommands();

			VkBufferCopy copyRegion = {};
			copyRegion.size = size;
			vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

			driver->endSingleTimeCommands(commandBuffer);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		uint32_t VulkanVertexBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkPhysicalDevice physicalDevice = driver->GetVulkanPhysicalDevice();

			VkPhysicalDeviceMemoryProperties memProperties;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

			for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
			{
				if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
				{
					return i;
				}
			}

			SH_ASSERT(0, "failed to find suitable memory type!");
			return 0;
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}




