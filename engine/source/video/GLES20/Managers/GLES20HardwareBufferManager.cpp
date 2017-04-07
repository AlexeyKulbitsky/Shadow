#include "GLES20HardwareBufferManager.h"

#include "../GLES20VertexBuffer.h"
#include "../GLES20IndexBuffer.h"

namespace sh
{

namespace video
{

	VertexBufferPtr GLES20HardwareBufferManager::CreateVertexBuffer( const VertexBufferDecription& description )
	{
		VertexBufferPtr result;
		result.reset(new GLES20VertexBuffer(description));
		return result;
	}

	IndexBufferPtr GLES20HardwareBufferManager::CreateIndexBuffer( const IndexBufferDescription& description )
	{
		IndexBufferPtr result;
		result.reset(new GLES20IndexBuffer(description));
		return result;
	}

	void GLES20HardwareBufferManager::CreateInstance()
	{
		s_instance = new GLES20HardwareBufferManager();
	}

	void GLES20HardwareBufferManager::DestroyInstance()
	{
		delete s_instance;
		s_instance = nullptr;
	}

} // video

} // sh