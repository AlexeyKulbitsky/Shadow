#include "IndexBuffer.h"

#include "Managers/HardwareBufferManager.h"

namespace sh
{

namespace video
{

	IndexBufferPtr IndexBuffer::Create(const IndexBufferDescription& description)
	{
		return HardwareBufferManager::GetInstance()->CreateIndexBuffer(description);
	}

	IndexBuffer::IndexBuffer(const IndexBufferDescription& description)
		: m_description(description)
	{	
	}

} // video

} // sh