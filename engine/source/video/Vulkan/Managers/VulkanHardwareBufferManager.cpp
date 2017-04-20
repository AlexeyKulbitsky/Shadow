#include "VulkanHardwareBufferManager.h"

#include "../VulkanVertexBuffer.h"
#include "../VulkanIndexBuffer.h"
#include "../VulkanGpuParams.h"

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

	GpuParamsPtr VulkanHardwareBufferManager::CreateGpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo)
	{
		GpuParamsPtr result;
		result.reset(new VulkanGpuParams(pipelineParamsInfo));
		return result;
	}
} // video

} // sh