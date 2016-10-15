#ifndef SHADOW_MESH_INCLUDE
#define SHADOW_MESH_INCLUDE

#include "../video/HardwareBuffer.h"
#include "../Types.h"
namespace sh
{
	namespace scene
	{		
		class Mesh
		{
		public:
			Mesh() {}
			void Init(const void* data, u32 size);

		private:
			video::HardwareBuffer m_buffer;
		};
	}
}
#endif