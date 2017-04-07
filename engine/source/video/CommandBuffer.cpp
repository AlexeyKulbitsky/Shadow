#include "CommandBuffer.h"

#include "../Device.h"
#include "Driver.h"

namespace sh
{

namespace video
{
		
	CommandBufferPtr CommandBuffer::Create(const CommandBufferDescription& description)
	{
		Driver* driver = Device::GetInstance()->GetDriver();
		return driver->CreateCommandBuffer(description);
	}

} // video

} // sh
