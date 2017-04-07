#include "VulkanHardwareBufferManager.h"

#include "../VulkanVertexBuffer.h"
#include "../VulkanIndexBuffer.h"

namespace sh
{

namespace video
{

	VertexBufferPtr VulkanHardwareBufferManager::CreateVertexBuffer( const VertexBufferDecription& description )
	{
		VertexBufferPtr result;
		result.reset(new VulkanVertexBuffer(description));
		return result;
	}

	IndexBufferPtr VulkanHardwareBufferManager::CreateIndexBuffer( const IndexBufferDescription& description )
	{
		IndexBufferPtr result;
		result.reset(new VulkanIndexBuffer(description));
		return result;
	}

	void VulkanHardwareBufferManager::CreateInstance()
	{
		s_instance = new VulkanHardwareBufferManager();
	}

	void VulkanHardwareBufferManager::DestroyInstance()
	{
		delete s_instance;
		s_instance = nullptr;
	}

} // video

} // sh