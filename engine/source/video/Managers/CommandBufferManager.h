#ifndef SHADOW_COMMAND_BUFFER_MANAGER_INCLUDE
#define SHADOW_COMMAND_BUFFER_MANAGER_INCLUDE

#include "../../Globals.h"
#include "../CommandBuffer.h"

namespace sh
{

namespace video
{

	class CommandBufferManager : public Singleton<CommandBufferManager>
	{
	public:
		virtual CommandBufferPtr CreateCommandBuffer(const CommandBufferDescription& description) = 0;
	};

} // video

} // sh

#endif