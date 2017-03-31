#include "IndexBuffer.h"
#include "../Device.h"
#include "Driver.h"

namespace sh
{

namespace video
{

	IndexBufferPtr IndexBuffer::Create(const IndexBufferDescription& description)
	{
		Driver* driver = Device::GetInstance()->GetDriver();
		return driver->CreateIndexBuffer(description);
	}

	IndexBuffer::IndexBuffer(const IndexBufferDescription& description)
		: HardwareBuffer(description.usage)
		, m_description(description)
		
	{	
	}

} // video

} // sh