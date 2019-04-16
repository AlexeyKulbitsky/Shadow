#ifndef SHADOW_GL_HARDWARE_BUFFER_MANAGER_INCLUDE
#define SHADOW_GL_HARDWARE_BUFFER_MANAGER_INCLUDE

#include "video/Managers/HardwareBufferManager.h"

namespace sh
{

namespace video
{
	
	class GLHardwareBufferManager : public HardwareBufferManager
	{
	public:
		VertexBufferPtr CreateVertexBuffer(const VertexBufferDecription& description) override final;
		IndexBufferPtr CreateIndexBuffer(const IndexBufferDescription& description) override final;
	};

} // video

} // sh

#endif