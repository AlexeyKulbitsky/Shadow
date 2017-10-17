#ifndef SHADOW_GLES20_HARDWARE_BUFFER_MANAGER_INCLUDE
#define SHADOW_GLES20_HARDWARE_BUFFER_MANAGER_INCLUDE

#include "../../../Managers/HardwareBufferManager.h"

namespace sh
{

namespace video
{
	
	class GLES20HardwareBufferManager : public HardwareBufferManager
	{
		friend class GLES20Driver;
	public:
		virtual VertexBufferPtr CreateVertexBuffer(const VertexBufferDecription& description) override;
		virtual IndexBufferPtr CreateIndexBuffer(const IndexBufferDescription& description) override;

	};

} // video

} // sh

#endif