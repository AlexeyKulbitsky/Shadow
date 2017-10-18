#ifndef SHADOW_GL_HARDWARE_BUFFER_MANAGER_INCLUDE
#define SHADOW_GL_HARDWARE_BUFFER_MANAGER_INCLUDE

#include "../../Managers/HardwareBufferManager.h"

namespace sh
{

namespace video
{
	
	class GLHardwareBufferManager : public HardwareBufferManager
	{
		//friend class GLES20Driver;
	public:
		virtual VertexBufferPtr CreateVertexBuffer(const VertexBufferDecription& description) override;
		virtual IndexBufferPtr CreateIndexBuffer(const IndexBufferDescription& description) override;

	};

} // video

} // sh

#endif