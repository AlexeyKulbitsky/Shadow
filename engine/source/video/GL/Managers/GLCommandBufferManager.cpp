#include "GLCommandBufferManager.h"
#include "../GLCommandBuffer.h"

namespace sh
{

namespace video
{
	
	CommandBufferPtr GLCommandBufferManager::CreateCommandBuffer( const CommandBufferDescription& description )
	{
		CommandBufferPtr result;
		result.reset(new GLCommandBuffer());
		return result;
	}

} // video

} // sh