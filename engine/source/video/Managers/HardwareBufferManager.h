#ifndef SHADOW_HARDWARE_BUFFER_MANAGER_INCLUDE
#define SHADOW_HARDWARE_BUFFER_MANAGER_INCLUDE

#include "../../Globals.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"

namespace sh
{

namespace video
{
	
	class HardwareBufferManager
	{
	public:
		static HardwareBufferManager* GetInstance() { return s_instance; }

		virtual VertexBufferPtr CreateVertexBuffer(const VertexBufferDecription& description) = 0;
		virtual IndexBufferPtr CreateIndexBuffer(const IndexBufferDescription& description) = 0;

	protected:
		static HardwareBufferManager* s_instance;
	};

} // video

} // sh

#endif