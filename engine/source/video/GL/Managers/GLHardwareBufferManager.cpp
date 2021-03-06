#include "GLHardwareBufferManager.h"

#include "../GLVertexBuffer.h"
#include "../GLIndexBuffer.h"

namespace sh
{

namespace video
{

	VertexBufferPtr GLHardwareBufferManager::CreateVertexBuffer( const VertexBufferDecription& description )
	{
		VertexBufferPtr result;
		result.reset(new GLVertexBuffer(description));
		return result;
	}

	IndexBufferPtr GLHardwareBufferManager::CreateIndexBuffer( const IndexBufferDescription& description )
	{
		IndexBufferPtr result;
		result.reset(new GLIndexBuffer(description));
		return result;
	}

} // video

} // sh