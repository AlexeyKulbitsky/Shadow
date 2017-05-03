#include "GLES20CommandBufferManager.h"
#include "../GLES20CommandBuffer.h"

namespace sh
{

namespace video
{
	
	CommandBufferPtr GLES20CommandBufferManager::CreateCommandBuffer( const CommandBufferDescription& description )
	{
		CommandBufferPtr result;
		result.reset(new GLES20CommandBuffer());
		return result;
	}

} // video

} // sh