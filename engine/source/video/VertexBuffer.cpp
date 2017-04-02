#include "VertexBuffer.h"
#include "../Device.h"
#include "Driver.h"

namespace sh
{

namespace video
{

	VertexBufferPtr VertexBuffer::Create(const VertexBufferDecription& description)
	{
		Driver* driver = Device::GetInstance()->GetDriver();
		return driver->CreateVertexBuffer(description);
	}

	VertexBuffer::VertexBuffer(const VertexBufferDecription& description)
		: m_description(description)
		, m_size(0U)
		, m_verticesCount(0U)
	{
	}

} // video

} // sh