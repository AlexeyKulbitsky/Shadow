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

			virtual void SetData(size_t offset, size_t length, const void* data) {}
			virtual void GetData(size_t offset, size_t length, const void* data) {}

		private:
			VkDevice m_device = VK_NULL_HANDLE;
			VkBuffer m_indexBuffer = VK_NULL_HANDLE;
			VkDeviceMemory m_indexBufferMemory = VK_NULL_HANDLE;
		};
	}
}

#endif
