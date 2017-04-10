#include "CommandBuffer.h"

#include "Managers/CommandBufferManager.h"

namespace sh
{

namespace video
{
		
	CommandBufferPtr CommandBuffer::Create(const CommandBufferDescription& description)
	{
		return CommandBufferManager::GetInstance()->CreateCommandBuffer(description);
	}

} // video

} // sh
