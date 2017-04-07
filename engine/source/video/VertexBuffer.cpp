#include "VertexBuffer.h"

#include "Managers/HardwareBufferManager.h"

namespace sh
{

namespace video
{

	VertexBufferPtr VertexBuffer::Create(const VertexBufferDecription& description)
	{
		return HardwareBufferManager::GetInstance()->CreateVertexBuffer(description);
	}

	VertexBuffer::VertexBuffer(const VertexBufferDecription& description)
		: m_description(description)
		, m_size(0U)
		, m_verticesCount(0U)
	{
	}

} // video

} // sh