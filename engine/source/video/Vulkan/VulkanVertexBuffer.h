#ifndef SHADOW_VULKAN_VERTEX_BUFFER_INCLUDE
#define SHADOW_VULKAN_VERTEX_BUFFER_INCLUDE

#include "../VertexBuffer.h"
#include "VulkanBuffer.h"

namespace sh
{
	namespace video
	{
		class VulkanVertexBuffer : public VertexBuffer, public VulkanBuffer
		{
		public:
			VulkanVertexBuffer(Usage usage);
			virtual ~VulkanVertexBuffer();

			virtual void SetData(size_t offset, size_t length, const void* data) override;
			virtual void GetData(size_t offset, size_t length, const void* data) override;

		};
	}
}
#endif // !VULKAN_VERTEX_BUFFER_INCLUDE
