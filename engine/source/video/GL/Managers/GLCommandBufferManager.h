#ifndef SHADOW_GL_COMMAND_BUFFER_MANAGER_INCLUDE
#define SHADOW_GL_COMMAND_BUFFER_MANAGER_INCLUDE

#include "../../Managers/CommandBufferManager.h"

namespace sh
{

namespace video
{
	
	class GLCommandBufferManager : public CommandBufferManager
	{
	public:
		virtual CommandBufferPtr CreateCommandBuffer(const CommandBufferDescription& description) override;
	};

} // video

} // sh

#endif