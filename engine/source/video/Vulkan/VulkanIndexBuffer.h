#ifndef SHADOW_VULKAN_INDEX_BUFFER_INCLUDE
#define SHADOW_VULKAN_INDEX_BUFFER_INCLUDE

#include "../IndexBuffer.h"
#include "VulkanBuffer.h"

namespace sh
{
	namespace video
	{
		class VulkanIndexBuffer : public IndexBuffer, public VulkanBuffer
		{
		public:
			VulkanIndexBuffer(Usage usage);
			virtual ~VulkanIndexBuffer();

			virtual void SetData(size_t offset, size_t length, const void* data) override;
			virtual void GetData(size_t offset, size_t length, const void* data) override;
		};
	}
}

#endif
