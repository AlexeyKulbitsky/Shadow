#include "VulkanIndexBuffer.h"
#include "../../Device.h"
#include "VulkanDriver.h"

namespace sh
{
	namespace video
	{
		VulkanIndexBuffer::VulkanIndexBuffer(Usage usage) : IndexBuffer(usage)
		{

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VulkanIndexBuffer::~VulkanIndexBuffer()
		{
			VulkanDriver* driver = static_cast<VulkanDriver*>(Device::GetInstance()->GetDriver());
			VkDevice device = driver->GetVulkanDevice();

			vkFreeMemory(device, m_bufferMemory, nullptr);
			vkDestroyBuffer(device, m_buffer, nullptr);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanIndexBuffer::SetData(size_t offset, size_t length, const void* data)
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
			createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_buffer, m_bufferMemory);
			// Copy data from temporary buffer to final buffer
			copyBuffer(stagingBuffer, m_buffer, bufferSize);

			// Free temporary buffer
			vkFreeMemory(device, stagingBufferMemory, nullptr);
			vkDestroyBuffer(device, stagingBuffer, nullptr);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

		void VulkanIndexBuffer::GetData(size_t offset, size_t length, const void* data)
		{

		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}