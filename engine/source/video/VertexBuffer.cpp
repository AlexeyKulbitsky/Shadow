#include "VertexBuffer.h"
#include "../Device.h"
#include "Driver.h"

namespace sh
{

namespace video
{

	VertexBufferPtr Create(const VertexBufferDecription& description)
	{
		Driver* driver = Device::GetInstance()->GetDriver();
		return driver->CreateVertexBuffer(description);
	}

	VertexBuffer::VertexBuffer(const VertexBufferDecription& description)
		: HardwareBuffer(description.usage)
		, m_description(description)
	{
	}

} // video

} // sh