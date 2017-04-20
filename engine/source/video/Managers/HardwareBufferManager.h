#ifndef SHADOW_HARDWARE_BUFFER_MANAGER_INCLUDE
#define SHADOW_HARDWARE_BUFFER_MANAGER_INCLUDE

#include "../../Globals.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../GpuParams.h"

namespace sh
{

namespace video
{
	
	class HardwareBufferManager : public Singleton<HardwareBufferManager>
	{
	public:

		virtual VertexBufferPtr CreateVertexBuffer(const VertexBufferDecription& description) = 0;
		virtual IndexBufferPtr CreateIndexBuffer(const IndexBufferDescription& description) = 0;
		virtual GpuParamsPtr CreateGpuParams(const GpuPipelineParamsInfoPtr& pipelineParamsInfo);
	};

} // video

} // sh

#endif