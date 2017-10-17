#ifndef SHADOW_GLES20_COMMAND_BUFFER_MANAGER_INCLUDE
#define SHADOW_GLES20_COMMAND_BUFFER_MANAGER_INCLUDE

#include "../../../Managers/CommandBufferManager.h"

namespace sh
{

namespace video
{
	
	class GLES20CommandBufferManager : public CommandBufferManager
	{
	public:
		virtual CommandBufferPtr CreateCommandBuffer(const CommandBufferDescription& description) override;
	};

} // video

} // sh

#endif