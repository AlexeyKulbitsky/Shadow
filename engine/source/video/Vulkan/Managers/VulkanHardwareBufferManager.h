#ifndef SHADOW_VULKAN_HARDWARE_BUFFER_MANAGER_INCLUDE
#define SHADOW_VULKAN_HARDWARE_BUFFER_MANAGER_INCLUDE

#include "../../Managers/HardwareBufferManager.h"

namespace sh
{

namespace video
{
	
	class VulkanHardwareBufferManager : public HardwareBufferManager
	{
		friend class VulkanDriver;
	public:
		virtual VertexBufferPtr CreateVertexBuffer(const VertexBufferDecription& description) override;
		virtual IndexBufferPtr CreateIndexBuffer(const IndexBufferDescription& description) override;

	private:
		static void CreateInstance();
		static void DestroyInstance();
	};

} // video

} // sh

#endif